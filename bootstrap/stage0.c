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

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t17 = gem_table_new();
    gem_table_set(_t17, gem_string("tag"), gem_string("let"));
    gem_table_set(_t17, gem_string("name"), gem_v_name);
    gem_table_set(_t17, gem_string("value"), gem_v_value);
    return _t17;
}

static GemVal gem_fn_make_assign(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t18 = gem_table_new();
    gem_table_set(_t18, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t18, gem_string("name"), gem_v_name);
    gem_table_set(_t18, gem_string("value"), gem_v_value);
    return _t18;
}

static GemVal gem_fn_make_dot_assign(void *_env, GemVal *args, int argc) {
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t19 = gem_table_new();
    gem_table_set(_t19, gem_string("tag"), gem_string("dot_assign"));
    gem_table_set(_t19, gem_string("object"), gem_v_object);
    gem_table_set(_t19, gem_string("field"), gem_v_field);
    gem_table_set(_t19, gem_string("value"), gem_v_value);
    return _t19;
}

static GemVal gem_fn_make_index_assign(void *_env, GemVal *args, int argc) {
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t20 = gem_table_new();
    gem_table_set(_t20, gem_string("tag"), gem_string("index_assign"));
    gem_table_set(_t20, gem_string("object"), gem_v_object);
    gem_table_set(_t20, gem_string("key"), gem_v_key);
    gem_table_set(_t20, gem_string("value"), gem_v_value);
    return _t20;
}

static GemVal gem_fn_make_fn_def(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t21 = gem_table_new();
    gem_table_set(_t21, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t21, gem_string("name"), gem_v_name);
    gem_table_set(_t21, gem_string("params"), gem_v_params);
    gem_table_set(_t21, gem_string("body"), gem_v_body);
    return _t21;
}

static GemVal gem_fn_make_if(void *_env, GemVal *args, int argc) {
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t22 = gem_table_new();
    gem_table_set(_t22, gem_string("tag"), gem_string("if"));
    gem_table_set(_t22, gem_string("cond"), gem_v_cond);
    gem_table_set(_t22, gem_string("then"), gem_v_then_body);
    GemVal gem_v_node = _t22;
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    return gem_v_node;
}

static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc) {
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t23 = gem_table_new();
    gem_table_set(_t23, gem_string("tag"), gem_string("while"));
    gem_table_set(_t23, gem_string("cond"), gem_v_cond);
    gem_table_set(_t23, gem_string("body"), gem_v_body);
    return _t23;
}

static GemVal gem_fn_make_match(void *_env, GemVal *args, int argc) {
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t24 = gem_table_new();
    gem_table_set(_t24, gem_string("tag"), gem_string("match"));
    gem_table_set(_t24, gem_string("target"), gem_v_target);
    gem_table_set(_t24, gem_string("whens"), gem_v_whens);
    GemVal gem_v_node = _t24;
    gem_table_set(gem_v_node, gem_string("else"), gem_v_else_body);
    return gem_v_node;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t25 = gem_table_new();
    gem_table_set(_t25, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t25, gem_string("value"), gem_v_value);
    gem_table_set(_t25, gem_string("body"), gem_v_body);
    return _t25;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t26 = gem_table_new();
    gem_table_set(_t26, gem_string("tag"), gem_string("return"));
    gem_table_set(_t26, gem_string("value"), gem_v_value);
    return _t26;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
    GemVal _t27 = gem_table_new();
    gem_table_set(_t27, gem_string("tag"), gem_string("break"));
    return _t27;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
    GemVal _t28 = gem_table_new();
    gem_table_set(_t28, gem_string("tag"), gem_string("continue"));
    return _t28;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t29 = gem_table_new();
    gem_table_set(_t29, gem_string("tag"), gem_string("load"));
    gem_table_set(_t29, gem_string("path"), gem_v_path);
    return _t29;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    GemVal _t30 = gem_table_new();
    gem_table_set(_t30, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t30, gem_string("name"), gem_v_name);
    gem_table_set(_t30, gem_string("params"), gem_v_params);
    gem_table_set(_t30, gem_string("ret_type"), gem_v_ret_type);
    return _t30;
}

static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t31 = gem_table_new();
    gem_table_set(_t31, gem_string("name"), gem_v_name);
    gem_table_set(_t31, gem_string("type"), gem_v_type_name);
    return _t31;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t32 = gem_table_new();
    gem_table_set(_t32, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t32, gem_string("path"), gem_v_path);
    return _t32;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t33;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t33 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t33 = gem_le(gem_v_ch, gem_string("9"));
    }
    return _t33;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t34;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t34 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t34 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t36;
    if (gem_truthy(_t34)) {
        _t36 = _t34;
    } else {
        GemVal _t35;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t35 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t35 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t36 = _t35;
    }
    GemVal _t37;
    if (gem_truthy(_t36)) {
        _t37 = _t36;
    } else {
        _t37 = gem_eq(gem_v_ch, gem_string("_"));
    }
    return _t37;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t38[] = {gem_v_ch};
    GemVal _t40;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t38, 1))) {
        _t40 = gem_fn_is_alpha(NULL, _t38, 1);
    } else {
        GemVal _t39[] = {gem_v_ch};
        _t40 = gem_fn_is_digit(NULL, _t39, 1);
    }
    return _t40;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t41 = gem_v_ch;
    if (gem_truthy(gem_eq(_t41, gem_string("0")))) {
        return gem_int(0);
    } else if (gem_truthy(gem_eq(_t41, gem_string("1")))) {
        return gem_int(1);
    } else if (gem_truthy(gem_eq(_t41, gem_string("2")))) {
        return gem_int(2);
    } else if (gem_truthy(gem_eq(_t41, gem_string("3")))) {
        return gem_int(3);
    } else if (gem_truthy(gem_eq(_t41, gem_string("4")))) {
        return gem_int(4);
    } else if (gem_truthy(gem_eq(_t41, gem_string("5")))) {
        return gem_int(5);
    } else if (gem_truthy(gem_eq(_t41, gem_string("6")))) {
        return gem_int(6);
    } else if (gem_truthy(gem_eq(_t41, gem_string("7")))) {
        return gem_int(7);
    } else if (gem_truthy(gem_eq(_t41, gem_string("8")))) {
        return gem_int(8);
    } else if (gem_truthy(gem_eq(_t41, gem_string("9")))) {
        return gem_int(9);
    } else {
    GemVal _t42[] = {gem_add(gem_string("not a digit: "), gem_v_ch)};
        return gem_error_at_fn("compiler/main.gem", 50, _t42, 1);
    }
}

static GemVal gem_fn_str_to_int(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = gem_int(0);
    {
        GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t43[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t43, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
            GemVal gem_v_i = gem_v__for_i_1;
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t44[] = {gem_table_get(gem_v_s, gem_v_i)};
            gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t44, 1));
        }
    }
    return gem_v_result;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    {
        GemVal gem_v__for_i_2 = gem_int(0);
    GemVal _t45[] = {gem_v_s};
        GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t45, 1);
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
    GemVal _t46 = gem_table_new();
    GemVal gem_v_t = _t46;
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
    GemVal _t47 = gem_table_new();
    GemVal gem_v_tokens = _t47;
    GemVal gem_v_pos = gem_int(0);
    GemVal gem_v_line = gem_int(1);
    GemVal _t48[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t48, 1);
    while (1) {
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
    GemVal _t49;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t49 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t49 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t49)) {
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
            while (1) {
                GemVal _t50;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t50 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t50 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t50)) break;
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
    GemVal _t51;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t51 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t51 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t51)) {
            GemVal gem_v_should_add = gem_bool(1);
    GemVal _t52[] = {gem_v_tokens};
    GemVal _t55;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t52, 1), gem_int(0)))) {
        _t55 = gem_gt(gem_len_fn(NULL, _t52, 1), gem_int(0));
    } else {
        GemVal _t53[] = {gem_v_tokens};
        GemVal _t54 = gem_table_get(gem_v_tokens, gem_sub(gem_len_fn(NULL, _t53, 1), gem_int(1)));
        _t55 = gem_eq(gem_table_get(_t54, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t55)) {
                gem_v_should_add = gem_bool(0);
            }
            if (gem_truthy(gem_v_should_add)) {
    GemVal _t56 = gem_table_new();
    gem_table_set(_t56, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t56, gem_string("value"), gem_string("\n"));
    gem_table_set(_t56, gem_string("line"), gem_v_line);
    GemVal _t57[] = {gem_v_tokens, _t56};
                (void)(gem_push_fn(NULL, _t57, 2));
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
    GemVal _t58;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t58 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t58 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t59;
    if (gem_truthy(_t58)) {
        _t59 = _t58;
    } else {
        _t59 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t59)) {
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
                            while (1) {
                                GemVal _t60;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t60 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t60 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t60)) break;
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
    GemVal _t61[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t61, 1))) {
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t63;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t63 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t62[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t63 = gem_fn_is_digit(NULL, _t62, 1);
                }
                if (!gem_truthy(_t63)) break;
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
    GemVal _t64;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t64 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t64 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t64;
            if (gem_truthy(gem_v_has_dot)) {
                gem_v_val = gem_add(gem_v_val, gem_string("."));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
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
            }
    GemVal _t67 = gem_table_new();
    gem_table_set(_t67, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t67, gem_string("value"), gem_v_val);
    gem_table_set(_t67, gem_string("line"), gem_v_line);
    GemVal _t68[] = {gem_v_tokens, _t67};
            (void)(gem_push_fn(NULL, _t68, 2));
            continue;
        }
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t69;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t69 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t69 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t69)) break;
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t70[] = {gem_v_line};
    GemVal _t71[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t70, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 187, _t71, 1));
                    }
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
                    GemVal _t72 = gem_v_esc;
                    if (gem_truthy(gem_eq(_t72, gem_string("n")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else if (gem_truthy(gem_eq(_t72, gem_string("t")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                    } else if (gem_truthy(gem_eq(_t72, gem_string("\\")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                    } else if (gem_truthy(gem_eq(_t72, gem_string("\"")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\""));
                    } else {
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
                } else {
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                }
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t73[] = {gem_v_line};
    GemVal _t74[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t73, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 208, _t74, 1));
            }
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
    GemVal _t75 = gem_table_new();
    gem_table_set(_t75, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t75, gem_string("value"), gem_v_val);
    gem_table_set(_t75, gem_string("line"), gem_v_line);
    GemVal _t76[] = {gem_v_tokens, _t75};
            (void)(gem_push_fn(NULL, _t76, 2));
            continue;
        }
    GemVal _t77[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t77, 1))) {
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t79;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t79 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t78[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t79 = gem_fn_is_alnum(NULL, _t78, 1);
                }
                if (!gem_truthy(_t79)) break;
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            if (gem_truthy(gem_eq(gem_table_get(gem_v_kw, gem_v_val), gem_bool(1)))) {
    GemVal _t80 = gem_table_new();
    gem_table_set(_t80, gem_string("type"), gem_v_val);
    gem_table_set(_t80, gem_string("value"), gem_v_val);
    gem_table_set(_t80, gem_string("line"), gem_v_line);
    GemVal _t81[] = {gem_v_tokens, _t80};
                (void)(gem_push_fn(NULL, _t81, 2));
            } else {
    GemVal _t82 = gem_table_new();
    gem_table_set(_t82, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t82, gem_string("value"), gem_v_val);
    gem_table_set(_t82, gem_string("line"), gem_v_line);
    GemVal _t83[] = {gem_v_tokens, _t82};
                (void)(gem_push_fn(NULL, _t83, 2));
            }
            continue;
        }
        GemVal gem_v_next = gem_string("");
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
    GemVal _t84;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t84 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t84 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t84)) {
    GemVal _t85 = gem_table_new();
    gem_table_set(_t85, gem_string("type"), gem_string("=="));
    gem_table_set(_t85, gem_string("value"), gem_string("=="));
    gem_table_set(_t85, gem_string("line"), gem_v_line);
    GemVal _t86[] = {gem_v_tokens, _t85};
            (void)(gem_push_fn(NULL, _t86, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t87;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t87 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t87 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t87)) {
    GemVal _t88 = gem_table_new();
    gem_table_set(_t88, gem_string("type"), gem_string("!="));
    gem_table_set(_t88, gem_string("value"), gem_string("!="));
    gem_table_set(_t88, gem_string("line"), gem_v_line);
    GemVal _t89[] = {gem_v_tokens, _t88};
            (void)(gem_push_fn(NULL, _t89, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t90;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t90 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t90 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t90)) {
    GemVal _t91 = gem_table_new();
    gem_table_set(_t91, gem_string("type"), gem_string("<="));
    gem_table_set(_t91, gem_string("value"), gem_string("<="));
    gem_table_set(_t91, gem_string("line"), gem_v_line);
    GemVal _t92[] = {gem_v_tokens, _t91};
            (void)(gem_push_fn(NULL, _t92, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t93;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t93 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t93 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t93)) {
    GemVal _t94 = gem_table_new();
    gem_table_set(_t94, gem_string("type"), gem_string(">="));
    gem_table_set(_t94, gem_string("value"), gem_string(">="));
    gem_table_set(_t94, gem_string("line"), gem_v_line);
    GemVal _t95[] = {gem_v_tokens, _t94};
            (void)(gem_push_fn(NULL, _t95, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t96;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t96 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t96 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t96)) {
    GemVal _t97 = gem_table_new();
    gem_table_set(_t97, gem_string("type"), gem_string("+="));
    gem_table_set(_t97, gem_string("value"), gem_string("+="));
    gem_table_set(_t97, gem_string("line"), gem_v_line);
    GemVal _t98[] = {gem_v_tokens, _t97};
            (void)(gem_push_fn(NULL, _t98, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t99;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t99 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t99 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t99)) {
    GemVal _t100 = gem_table_new();
    gem_table_set(_t100, gem_string("type"), gem_string("-="));
    gem_table_set(_t100, gem_string("value"), gem_string("-="));
    gem_table_set(_t100, gem_string("line"), gem_v_line);
    GemVal _t101[] = {gem_v_tokens, _t100};
            (void)(gem_push_fn(NULL, _t101, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t102;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t102 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t102 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t102)) {
    GemVal _t103 = gem_table_new();
    gem_table_set(_t103, gem_string("type"), gem_string("*="));
    gem_table_set(_t103, gem_string("value"), gem_string("*="));
    gem_table_set(_t103, gem_string("line"), gem_v_line);
    GemVal _t104[] = {gem_v_tokens, _t103};
            (void)(gem_push_fn(NULL, _t104, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t105;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t105 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t105 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t105)) {
    GemVal _t106 = gem_table_new();
    gem_table_set(_t106, gem_string("type"), gem_string("/="));
    gem_table_set(_t106, gem_string("value"), gem_string("/="));
    gem_table_set(_t106, gem_string("line"), gem_v_line);
    GemVal _t107[] = {gem_v_tokens, _t106};
            (void)(gem_push_fn(NULL, _t107, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t108;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t108 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t108 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t108)) {
    GemVal _t109 = gem_table_new();
    gem_table_set(_t109, gem_string("type"), gem_string("->"));
    gem_table_set(_t109, gem_string("value"), gem_string("->"));
    gem_table_set(_t109, gem_string("line"), gem_v_line);
    GemVal _t110[] = {gem_v_tokens, _t109};
            (void)(gem_push_fn(NULL, _t110, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t111;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t111 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t111 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t112;
    if (gem_truthy(_t111)) {
        _t112 = _t111;
    } else {
        _t112 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t113;
    if (gem_truthy(_t112)) {
        _t113 = _t112;
    } else {
        _t113 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t114;
    if (gem_truthy(_t113)) {
        _t114 = _t113;
    } else {
        _t114 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t114)) {
    GemVal _t115 = gem_table_new();
    gem_table_set(_t115, gem_string("type"), gem_v_ch);
    gem_table_set(_t115, gem_string("value"), gem_v_ch);
    gem_table_set(_t115, gem_string("line"), gem_v_line);
    GemVal _t116[] = {gem_v_tokens, _t115};
            (void)(gem_push_fn(NULL, _t116, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t117;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t117 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t117 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t118;
    if (gem_truthy(_t117)) {
        _t118 = _t117;
    } else {
        _t118 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t118)) {
    GemVal _t119 = gem_table_new();
    gem_table_set(_t119, gem_string("type"), gem_v_ch);
    gem_table_set(_t119, gem_string("value"), gem_v_ch);
    gem_table_set(_t119, gem_string("line"), gem_v_line);
    GemVal _t120[] = {gem_v_tokens, _t119};
            (void)(gem_push_fn(NULL, _t120, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t121;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t121 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t121 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t122;
    if (gem_truthy(_t121)) {
        _t122 = _t121;
    } else {
        _t122 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t123;
    if (gem_truthy(_t122)) {
        _t123 = _t122;
    } else {
        _t123 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t124;
    if (gem_truthy(_t123)) {
        _t124 = _t123;
    } else {
        _t124 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t125;
    if (gem_truthy(_t124)) {
        _t125 = _t124;
    } else {
        _t125 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t125)) {
    GemVal _t126 = gem_table_new();
    gem_table_set(_t126, gem_string("type"), gem_v_ch);
    gem_table_set(_t126, gem_string("value"), gem_v_ch);
    gem_table_set(_t126, gem_string("line"), gem_v_line);
    GemVal _t127[] = {gem_v_tokens, _t126};
            (void)(gem_push_fn(NULL, _t127, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t128;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t128 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t128 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t129;
    if (gem_truthy(_t128)) {
        _t129 = _t128;
    } else {
        _t129 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t130;
    if (gem_truthy(_t129)) {
        _t130 = _t129;
    } else {
        _t130 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t130)) {
    GemVal _t131 = gem_table_new();
    gem_table_set(_t131, gem_string("type"), gem_v_ch);
    gem_table_set(_t131, gem_string("value"), gem_v_ch);
    gem_table_set(_t131, gem_string("line"), gem_v_line);
    GemVal _t132[] = {gem_v_tokens, _t131};
            (void)(gem_push_fn(NULL, _t132, 2));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t133[] = {gem_v_line};
    GemVal _t134[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_v_ch), gem_string("' at line ")), gem_to_string_fn(NULL, _t133, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 304, _t134, 1));
    }
    GemVal _t135 = gem_table_new();
    gem_table_set(_t135, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t135, gem_string("value"), gem_string(""));
    gem_table_set(_t135, gem_string("line"), gem_v_line);
    GemVal _t136[] = {gem_v_tokens, _t135};
    (void)(gem_push_fn(NULL, _t136, 2));
    return gem_v_tokens;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t137[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t137, 1);
    GemVal _t138[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t138, 1);
    GemVal _t139 = gem_v_p;
    GemVal _t140 = gem_table_get(_t139, gem_string("parse"));
    return _t140.fn(_t140.env, NULL, 0);
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
    GemVal _t144 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    return gem_eq(gem_table_get(_t144, gem_string("type")), gem_string("EOF"));
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
    GemVal _t146 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t146, gem_string("type")), gem_v_tp))) {
    GemVal _t147 = gem_v_t;
    GemVal _t148 = gem_v_t;
    GemVal _t149[] = {gem_table_get(_t148, gem_string("line"))};
    GemVal _t150[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_v_tp), gem_string("' but got '")), gem_table_get(_t147, gem_string("type"))), gem_string("' at line ")), gem_to_string_fn(NULL, _t149, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 45, _t150, 1));
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
        GemVal _t152 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t152, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t154 = gem_table_new();
    GemVal gem_v_params = _t154;
    GemVal _t155 = (*gem_v_peek);
    GemVal _t156 = _t155.fn(_t155.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t156, gem_string("type")), gem_string("NAME")))) {
    GemVal _t157 = (*gem_v_advance);
    GemVal _t158 = _t157.fn(_t157.env, NULL, 0);
    GemVal _t159[] = {gem_v_params, gem_table_get(_t158, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t159, 2));
        while (1) {
            GemVal _t160 = (*gem_v_peek);
            GemVal _t161 = _t160.fn(_t160.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t161, gem_string("type")), gem_string(",")))) break;
    GemVal _t162 = (*gem_v_advance);
            (void)(_t162.fn(_t162.env, NULL, 0));
    GemVal _t163 = (*gem_v_expect);
    GemVal _t164[] = {gem_string("NAME")};
    GemVal _t165 = _t163.fn(_t163.env, _t164, 1);
    GemVal _t166[] = {gem_v_params, gem_table_get(_t165, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t166, 2));
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
    GemVal _t168 = gem_table_new();
    GemVal gem_v_stmts = _t168;
    GemVal _t169 = (*gem_v_skip_nl);
    (void)(_t169.fn(_t169.env, NULL, 0));
    while (1) {
        GemVal _t170 = (*gem_v_peek);
        GemVal _t171 = _t170.fn(_t170.env, NULL, 0);
        GemVal _t174;
        if (!gem_truthy(gem_neq(gem_table_get(_t171, gem_string("type")), gem_string("end")))) {
                _t174 = gem_neq(gem_table_get(_t171, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t172 = (*gem_v_peek);
                GemVal _t173 = _t172.fn(_t172.env, NULL, 0);
                _t174 = gem_neq(gem_table_get(_t173, gem_string("type")), gem_string("elif"));
        }
        GemVal _t177;
        if (!gem_truthy(_t174)) {
                _t177 = _t174;
        } else {
                GemVal _t175 = (*gem_v_peek);
                GemVal _t176 = _t175.fn(_t175.env, NULL, 0);
                _t177 = gem_neq(gem_table_get(_t176, gem_string("type")), gem_string("else"));
        }
        GemVal _t180;
        if (!gem_truthy(_t177)) {
                _t180 = _t177;
        } else {
                GemVal _t178 = (*gem_v_peek);
                GemVal _t179 = _t178.fn(_t178.env, NULL, 0);
                _t180 = gem_neq(gem_table_get(_t179, gem_string("type")), gem_string("when"));
        }
        GemVal _t182;
        if (!gem_truthy(_t180)) {
                _t182 = _t180;
        } else {
                GemVal _t181 = (*gem_v_at_end);
                _t182 = gem_not(_t181.fn(_t181.env, NULL, 0));
        }
        if (!gem_truthy(_t182)) break;
    GemVal _t183 = (*gem_v_parse_stmt);
    GemVal _t184[] = {gem_v_stmts, _t183.fn(_t183.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t184, 2));
    GemVal _t185 = (*gem_v_skip_nl);
        (void)(_t185.fn(_t185.env, NULL, 0));
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
    GemVal _t187 = gem_table_new();
    GemVal gem_v_stmts = _t187;
    GemVal _t188 = (*gem_v_skip_nl);
    (void)(_t188.fn(_t188.env, NULL, 0));
    while (1) {
        GemVal _t189 = (*gem_v_peek);
        GemVal _t190 = _t189.fn(_t189.env, NULL, 0);
        GemVal _t192;
        if (!gem_truthy(gem_neq(gem_table_get(_t190, gem_string("type")), gem_string("end")))) {
                _t192 = gem_neq(gem_table_get(_t190, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t191 = (*gem_v_at_end);
                _t192 = gem_not(_t191.fn(_t191.env, NULL, 0));
        }
        if (!gem_truthy(_t192)) break;
    GemVal _t193 = (*gem_v_parse_stmt);
    GemVal _t194[] = {gem_v_stmts, _t193.fn(_t193.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t194, 2));
    GemVal _t195 = (*gem_v_skip_nl);
        (void)(_t195.fn(_t195.env, NULL, 0));
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
    GemVal _t197 = (*gem_v_peek);
    GemVal gem_v_t = _t197.fn(_t197.env, NULL, 0);
    GemVal _t198 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t198, gem_string("type")), gem_string("NUMBER")))) {
    GemVal _t199 = (*gem_v_advance);
        (void)(_t199.fn(_t199.env, NULL, 0));
    GemVal _t200 = gem_v_t;
    GemVal _t201[] = {gem_table_get(_t200, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t201, 1))) {
    GemVal _t202 = gem_v_t;
    GemVal _t203[] = {gem_table_get(_t202, gem_string("value"))};
    GemVal _t204[] = {gem_fn_atof(NULL, _t203, 1)};
            return gem_fn_make_float(NULL, _t204, 1);
        }
    GemVal _t205 = gem_v_t;
    GemVal _t206[] = {gem_table_get(_t205, gem_string("value"))};
    GemVal _t207[] = {gem_fn_str_to_int(NULL, _t206, 1)};
        return gem_fn_make_int(NULL, _t207, 1);
    }
    GemVal _t208 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t208, gem_string("type")), gem_string("STRING")))) {
    GemVal _t209 = (*gem_v_advance);
        (void)(_t209.fn(_t209.env, NULL, 0));
    GemVal _t210 = gem_v_t;
    GemVal _t211[] = {gem_table_get(_t210, gem_string("value"))};
        return gem_fn_make_string(NULL, _t211, 1);
    }
    GemVal _t212 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t212, gem_string("type")), gem_string("true")))) {
    GemVal _t213 = (*gem_v_advance);
        (void)(_t213.fn(_t213.env, NULL, 0));
    GemVal _t214[] = {gem_bool(1)};
        return gem_fn_make_bool(NULL, _t214, 1);
    }
    GemVal _t215 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t215, gem_string("type")), gem_string("false")))) {
    GemVal _t216 = (*gem_v_advance);
        (void)(_t216.fn(_t216.env, NULL, 0));
    GemVal _t217[] = {gem_bool(0)};
        return gem_fn_make_bool(NULL, _t217, 1);
    }
    GemVal _t218 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t218, gem_string("type")), gem_string("nil")))) {
    GemVal _t219 = (*gem_v_advance);
        (void)(_t219.fn(_t219.env, NULL, 0));
        return gem_fn_make_nil_node(NULL, NULL, 0);
    }
    GemVal _t220 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t220, gem_string("type")), gem_string("fn")))) {
    GemVal _t221 = (*gem_v_advance);
        (void)(_t221.fn(_t221.env, NULL, 0));
    GemVal _t222 = (*gem_v_expect);
    GemVal _t223[] = {gem_string("(")};
        (void)(_t222.fn(_t222.env, _t223, 1));
    GemVal _t224 = (*gem_v_parse_params);
        GemVal gem_v_params = _t224.fn(_t224.env, NULL, 0);
    GemVal _t225 = (*gem_v_expect);
    GemVal _t226[] = {gem_string(")")};
        (void)(_t225.fn(_t225.env, _t226, 1));
    GemVal _t227 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t227.fn(_t227.env, NULL, 0);
    GemVal _t228 = (*gem_v_expect);
    GemVal _t229[] = {gem_string("end")};
        (void)(_t228.fn(_t228.env, _t229, 1));
    GemVal _t230[] = {gem_v_params, gem_v_body};
        return gem_fn_make_anon_fn(NULL, _t230, 2);
    }
    GemVal _t231 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t231, gem_string("type")), gem_string("{")))) {
    GemVal _t232 = (*gem_v_advance);
        (void)(_t232.fn(_t232.env, NULL, 0));
    GemVal _t233 = (*gem_v_skip_nl);
        (void)(_t233.fn(_t233.env, NULL, 0));
    GemVal _t234 = gem_table_new();
        GemVal gem_v_entries = _t234;
        while (1) {
            GemVal _t235 = (*gem_v_peek);
            GemVal _t236 = _t235.fn(_t235.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t236, gem_string("type")), gem_string("}")))) break;
    GemVal _t237 = (*gem_v_expect);
    GemVal _t238[] = {gem_string("NAME")};
    GemVal _t239 = _t237.fn(_t237.env, _t238, 1);
            GemVal gem_v_key = gem_table_get(_t239, gem_string("value"));
    GemVal _t240 = (*gem_v_expect);
    GemVal _t241[] = {gem_string(":")};
            (void)(_t240.fn(_t240.env, _t241, 1));
    GemVal _t242 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t242.fn(_t242.env, NULL, 0);
    GemVal _t243[] = {gem_v_key, gem_v_val};
    GemVal _t244[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t243, 2)};
            (void)(gem_push_fn(NULL, _t244, 2));
    GemVal _t245 = (*gem_v_skip_nl);
            (void)(_t245.fn(_t245.env, NULL, 0));
    GemVal _t246 = (*gem_v_peek);
    GemVal _t247 = _t246.fn(_t246.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t247, gem_string("type")), gem_string(",")))) {
    GemVal _t248 = (*gem_v_advance);
                (void)(_t248.fn(_t248.env, NULL, 0));
    GemVal _t249 = (*gem_v_skip_nl);
                (void)(_t249.fn(_t249.env, NULL, 0));
            }
        }
    GemVal _t250 = (*gem_v_expect);
    GemVal _t251[] = {gem_string("}")};
        (void)(_t250.fn(_t250.env, _t251, 1));
    GemVal _t252[] = {gem_v_entries};
        return gem_fn_make_table(NULL, _t252, 1);
    }
    GemVal _t253 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t253, gem_string("type")), gem_string("[")))) {
    GemVal _t254 = (*gem_v_advance);
        (void)(_t254.fn(_t254.env, NULL, 0));
    GemVal _t255 = (*gem_v_skip_nl);
        (void)(_t255.fn(_t255.env, NULL, 0));
    GemVal _t256 = gem_table_new();
        GemVal gem_v_elements = _t256;
        while (1) {
            GemVal _t257 = (*gem_v_peek);
            GemVal _t258 = _t257.fn(_t257.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t258, gem_string("type")), gem_string("]")))) break;
    GemVal _t259 = (*gem_v_parse_expr);
    GemVal _t260[] = {gem_v_elements, _t259.fn(_t259.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t260, 2));
    GemVal _t261 = (*gem_v_skip_nl);
            (void)(_t261.fn(_t261.env, NULL, 0));
    GemVal _t262 = (*gem_v_peek);
    GemVal _t263 = _t262.fn(_t262.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t263, gem_string("type")), gem_string(",")))) {
    GemVal _t264 = (*gem_v_advance);
                (void)(_t264.fn(_t264.env, NULL, 0));
    GemVal _t265 = (*gem_v_skip_nl);
                (void)(_t265.fn(_t265.env, NULL, 0));
            }
        }
    GemVal _t266 = (*gem_v_expect);
    GemVal _t267[] = {gem_string("]")};
        (void)(_t266.fn(_t266.env, _t267, 1));
    GemVal _t268[] = {gem_v_elements};
        return gem_fn_make_array(NULL, _t268, 1);
    }
    GemVal _t269 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t269, gem_string("type")), gem_string("(")))) {
    GemVal _t270 = (*gem_v_advance);
        (void)(_t270.fn(_t270.env, NULL, 0));
    GemVal _t271 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t271.fn(_t271.env, NULL, 0);
    GemVal _t272 = (*gem_v_expect);
    GemVal _t273[] = {gem_string(")")};
        (void)(_t272.fn(_t272.env, _t273, 1));
        return gem_v_e;
    }
    GemVal _t274 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t274, gem_string("type")), gem_string("NAME")))) {
    GemVal _t275 = (*gem_v_advance);
        (void)(_t275.fn(_t275.env, NULL, 0));
    GemVal _t276 = gem_v_t;
    GemVal _t277[] = {gem_table_get(_t276, gem_string("value"))};
        return gem_fn_make_var(NULL, _t277, 1);
    }
    GemVal _t278 = gem_v_t;
    GemVal _t279 = gem_v_t;
    GemVal _t280[] = {gem_table_get(_t279, gem_string("line"))};
    GemVal _t281[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_table_get(_t278, gem_string("type"))), gem_string("' at line ")), gem_to_string_fn(NULL, _t280, 1))};
    return gem_error_at_fn("compiler/main.gem", 204, _t281, 1);
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
    GemVal _t283 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t283.fn(_t283.env, NULL, 0);
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
    GemVal _t284 = (*gem_v_peek);
    GemVal _t285 = _t284.fn(_t284.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t285, gem_string("type")), gem_string("(")))) {
    GemVal _t286 = (*gem_v_peek);
    GemVal _t287 = _t286.fn(_t286.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t287, gem_string("line"));
    GemVal _t288 = (*gem_v_advance);
            (void)(_t288.fn(_t288.env, NULL, 0));
    GemVal _t289 = gem_table_new();
            GemVal gem_v_args = _t289;
    GemVal _t290 = (*gem_v_peek);
    GemVal _t291 = _t290.fn(_t290.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t291, gem_string("type")), gem_string(")")))) {
    GemVal _t292 = (*gem_v_parse_expr);
    GemVal _t293[] = {gem_v_args, _t292.fn(_t292.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t293, 2));
                while (1) {
                    GemVal _t294 = (*gem_v_peek);
                    GemVal _t295 = _t294.fn(_t294.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t295, gem_string("type")), gem_string(",")))) break;
    GemVal _t296 = (*gem_v_advance);
                    (void)(_t296.fn(_t296.env, NULL, 0));
    GemVal _t297 = (*gem_v_parse_expr);
    GemVal _t298[] = {gem_v_args, _t297.fn(_t297.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t298, 2));
                }
            }
    GemVal _t299 = (*gem_v_expect);
    GemVal _t300[] = {gem_string(")")};
            (void)(_t299.fn(_t299.env, _t300, 1));
    GemVal _t301 = (*gem_v_peek);
    GemVal _t302 = _t301.fn(_t301.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t302, gem_string("type")), gem_string("do")))) {
    GemVal _t303 = (*gem_v_advance);
                (void)(_t303.fn(_t303.env, NULL, 0));
    GemVal _t304 = gem_table_new();
                GemVal gem_v_bparams = _t304;
    GemVal _t305 = (*gem_v_peek);
    GemVal _t306 = _t305.fn(_t305.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t306, gem_string("type")), gem_string("|")))) {
    GemVal _t307 = (*gem_v_advance);
                    (void)(_t307.fn(_t307.env, NULL, 0));
    GemVal _t308 = (*gem_v_parse_params);
                    gem_v_bparams = _t308.fn(_t308.env, NULL, 0);
    GemVal _t309 = (*gem_v_expect);
    GemVal _t310[] = {gem_string("|")};
                    (void)(_t309.fn(_t309.env, _t310, 1));
                }
    GemVal _t311 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t311.fn(_t311.env, NULL, 0);
    GemVal _t312 = (*gem_v_expect);
    GemVal _t313[] = {gem_string("end")};
                (void)(_t312.fn(_t312.env, _t313, 1));
    GemVal _t314[] = {gem_v_bparams, gem_v_body};
    GemVal _t315[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t314, 2)};
                (void)(gem_push_fn(NULL, _t315, 2));
            } else {
    GemVal _t316 = (*gem_v_peek);
    GemVal _t317 = _t316.fn(_t316.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t317, gem_string("type")), gem_string("{")))) {
                    GemVal gem_v_is_block = gem_bool(0);
    GemVal _t318 = (*gem_v_peek_at);
    GemVal _t319[] = {gem_int(1)};
    GemVal _t320 = _t318.fn(_t318.env, _t319, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t320, gem_string("type")), gem_string("|")))) {
                        gem_v_is_block = gem_bool(1);
                    } else {
    GemVal _t321 = (*gem_v_peek_at);
    GemVal _t322[] = {gem_int(1)};
    GemVal _t323 = _t321.fn(_t321.env, _t322, 1);
    GemVal _t331;
    if (gem_truthy(gem_eq(gem_table_get(_t323, gem_string("type")), gem_string("}")))) {
        _t331 = gem_eq(gem_table_get(_t323, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t324 = (*gem_v_peek_at);
        GemVal _t325[] = {gem_int(1)};
        GemVal _t326 = _t324.fn(_t324.env, _t325, 1);
        GemVal _t330;
        if (!gem_truthy(gem_eq(gem_table_get(_t326, gem_string("type")), gem_string("NAME")))) {
                _t330 = gem_eq(gem_table_get(_t326, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t327 = (*gem_v_peek_at);
                GemVal _t328[] = {gem_int(2)};
                GemVal _t329 = _t327.fn(_t327.env, _t328, 1);
                _t330 = gem_eq(gem_table_get(_t329, gem_string("type")), gem_string(":"));
        }
        _t331 = _t330;
    }
                        if (gem_truthy(_t331)) {
                            gem_v_is_block = gem_bool(0);
                        } else {
                            gem_v_is_block = gem_bool(1);
                        }
                    }
                    if (gem_truthy(gem_v_is_block)) {
    GemVal _t332 = (*gem_v_advance);
                        (void)(_t332.fn(_t332.env, NULL, 0));
    GemVal _t333 = gem_table_new();
                        GemVal gem_v_bparams = _t333;
    GemVal _t334 = (*gem_v_peek);
    GemVal _t335 = _t334.fn(_t334.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t335, gem_string("type")), gem_string("|")))) {
    GemVal _t336 = (*gem_v_advance);
                            (void)(_t336.fn(_t336.env, NULL, 0));
    GemVal _t337 = (*gem_v_parse_params);
                            gem_v_bparams = _t337.fn(_t337.env, NULL, 0);
    GemVal _t338 = (*gem_v_expect);
    GemVal _t339[] = {gem_string("|")};
                            (void)(_t338.fn(_t338.env, _t339, 1));
                        }
    GemVal _t340 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t340.fn(_t340.env, NULL, 0);
    GemVal _t341 = (*gem_v_expect);
    GemVal _t342[] = {gem_string("}")};
                        (void)(_t341.fn(_t341.env, _t342, 1));
    GemVal _t343 = gem_table_new();
    gem_table_set(_t343, gem_int(0), gem_v_bexpr);
    GemVal _t344[] = {gem_v_bparams, _t343};
    GemVal _t345[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t344, 2)};
                        (void)(gem_push_fn(NULL, _t345, 2));
                    }
                }
            }
    GemVal _t346[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t346, 3);
            continue;
        }
    GemVal _t347 = (*gem_v_peek);
    GemVal _t348 = _t347.fn(_t347.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t348, gem_string("type")), gem_string(".")))) {
    GemVal _t349 = (*gem_v_advance);
            (void)(_t349.fn(_t349.env, NULL, 0));
    GemVal _t350 = (*gem_v_expect);
    GemVal _t351[] = {gem_string("NAME")};
    GemVal _t352 = _t350.fn(_t350.env, _t351, 1);
            GemVal gem_v_field = gem_table_get(_t352, gem_string("value"));
    GemVal _t353[] = {gem_v_node, gem_v_field};
            gem_v_node = gem_fn_make_dot(NULL, _t353, 2);
            continue;
        }
    GemVal _t354 = (*gem_v_peek);
    GemVal _t355 = _t354.fn(_t354.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t355, gem_string("type")), gem_string("[")))) {
    GemVal _t356 = (*gem_v_advance);
            (void)(_t356.fn(_t356.env, NULL, 0));
    GemVal _t357 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t357.fn(_t357.env, NULL, 0);
    GemVal _t358 = (*gem_v_expect);
    GemVal _t359[] = {gem_string("]")};
            (void)(_t358.fn(_t358.env, _t359, 1));
    GemVal _t360[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t360, 2);
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
    GemVal _t362 = (*gem_v_peek);
    GemVal _t363 = _t362.fn(_t362.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t363, gem_string("type")), gem_string("-")))) {
    GemVal _t364 = (*gem_v_advance);
        (void)(_t364.fn(_t364.env, NULL, 0));
    GemVal _t365 = (*gem_v_parse_unary);
    GemVal _t366[] = {gem_string("-"), _t365.fn(_t365.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t366, 2);
    }
    GemVal _t367 = (*gem_v_parse_call);
    return _t367.fn(_t367.env, NULL, 0);
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
    GemVal _t369 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t369.fn(_t369.env, NULL, 0);
    while (1) {
        GemVal _t370 = (*gem_v_peek);
        GemVal _t371 = _t370.fn(_t370.env, NULL, 0);
        GemVal _t374;
        if (gem_truthy(gem_eq(gem_table_get(_t371, gem_string("type")), gem_string("*")))) {
                _t374 = gem_eq(gem_table_get(_t371, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t372 = (*gem_v_peek);
                GemVal _t373 = _t372.fn(_t372.env, NULL, 0);
                _t374 = gem_eq(gem_table_get(_t373, gem_string("type")), gem_string("/"));
        }
        GemVal _t377;
        if (gem_truthy(_t374)) {
                _t377 = _t374;
        } else {
                GemVal _t375 = (*gem_v_peek);
                GemVal _t376 = _t375.fn(_t375.env, NULL, 0);
                _t377 = gem_eq(gem_table_get(_t376, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t377)) break;
    GemVal _t378 = (*gem_v_advance);
    GemVal _t379 = _t378.fn(_t378.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t379, gem_string("type"));
    GemVal _t380 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t380.fn(_t380.env, NULL, 0);
    GemVal _t381[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t381, 3);
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
    GemVal _t383 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t383.fn(_t383.env, NULL, 0);
    while (1) {
        GemVal _t384 = (*gem_v_peek);
        GemVal _t385 = _t384.fn(_t384.env, NULL, 0);
        GemVal _t388;
        if (gem_truthy(gem_eq(gem_table_get(_t385, gem_string("type")), gem_string("+")))) {
                _t388 = gem_eq(gem_table_get(_t385, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t386 = (*gem_v_peek);
                GemVal _t387 = _t386.fn(_t386.env, NULL, 0);
                _t388 = gem_eq(gem_table_get(_t387, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t388)) break;
    GemVal _t389 = (*gem_v_advance);
    GemVal _t390 = _t389.fn(_t389.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t390, gem_string("type"));
    GemVal _t391 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t391.fn(_t391.env, NULL, 0);
    GemVal _t392[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t392, 3);
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
    GemVal _t394 = (*gem_v_parse_add);
    GemVal gem_v_left = _t394.fn(_t394.env, NULL, 0);
    while (1) {
        GemVal _t395 = (*gem_v_peek);
        GemVal _t396 = _t395.fn(_t395.env, NULL, 0);
        GemVal _t399;
        if (gem_truthy(gem_eq(gem_table_get(_t396, gem_string("type")), gem_string("==")))) {
                _t399 = gem_eq(gem_table_get(_t396, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t397 = (*gem_v_peek);
                GemVal _t398 = _t397.fn(_t397.env, NULL, 0);
                _t399 = gem_eq(gem_table_get(_t398, gem_string("type")), gem_string("!="));
        }
        GemVal _t402;
        if (gem_truthy(_t399)) {
                _t402 = _t399;
        } else {
                GemVal _t400 = (*gem_v_peek);
                GemVal _t401 = _t400.fn(_t400.env, NULL, 0);
                _t402 = gem_eq(gem_table_get(_t401, gem_string("type")), gem_string("<"));
        }
        GemVal _t405;
        if (gem_truthy(_t402)) {
                _t405 = _t402;
        } else {
                GemVal _t403 = (*gem_v_peek);
                GemVal _t404 = _t403.fn(_t403.env, NULL, 0);
                _t405 = gem_eq(gem_table_get(_t404, gem_string("type")), gem_string(">"));
        }
        GemVal _t408;
        if (gem_truthy(_t405)) {
                _t408 = _t405;
        } else {
                GemVal _t406 = (*gem_v_peek);
                GemVal _t407 = _t406.fn(_t406.env, NULL, 0);
                _t408 = gem_eq(gem_table_get(_t407, gem_string("type")), gem_string("<="));
        }
        GemVal _t411;
        if (gem_truthy(_t408)) {
                _t411 = _t408;
        } else {
                GemVal _t409 = (*gem_v_peek);
                GemVal _t410 = _t409.fn(_t409.env, NULL, 0);
                _t411 = gem_eq(gem_table_get(_t410, gem_string("type")), gem_string(">="));
        }
        if (!gem_truthy(_t411)) break;
    GemVal _t412 = (*gem_v_advance);
    GemVal _t413 = _t412.fn(_t412.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t413, gem_string("type"));
    GemVal _t414 = (*gem_v_parse_add);
        GemVal gem_v_right = _t414.fn(_t414.env, NULL, 0);
    GemVal _t415[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t415, 3);
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
    GemVal _t417 = (*gem_v_peek);
    GemVal _t418 = _t417.fn(_t417.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t418, gem_string("type")), gem_string("not")))) {
    GemVal _t419 = (*gem_v_advance);
        (void)(_t419.fn(_t419.env, NULL, 0));
    GemVal _t420 = (*gem_v_parse_not);
    GemVal _t421[] = {gem_string("not"), _t420.fn(_t420.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t421, 2);
    }
    GemVal _t422 = (*gem_v_parse_compare);
    return _t422.fn(_t422.env, NULL, 0);
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
    GemVal _t424 = (*gem_v_parse_not);
    GemVal gem_v_left = _t424.fn(_t424.env, NULL, 0);
    while (1) {
        GemVal _t425 = (*gem_v_peek);
        GemVal _t426 = _t425.fn(_t425.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t426, gem_string("type")), gem_string("and")))) break;
    GemVal _t427 = (*gem_v_advance);
        (void)(_t427.fn(_t427.env, NULL, 0));
    GemVal _t428 = (*gem_v_parse_not);
        GemVal gem_v_right = _t428.fn(_t428.env, NULL, 0);
    GemVal _t429[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t429, 3);
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
    GemVal _t431 = (*gem_v_parse_and);
    GemVal gem_v_left = _t431.fn(_t431.env, NULL, 0);
    while (1) {
        GemVal _t432 = (*gem_v_peek);
        GemVal _t433 = _t432.fn(_t432.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t433, gem_string("type")), gem_string("or")))) break;
    GemVal _t434 = (*gem_v_advance);
        (void)(_t434.fn(_t434.env, NULL, 0));
    GemVal _t435 = (*gem_v_parse_and);
        GemVal gem_v_right = _t435.fn(_t435.env, NULL, 0);
    GemVal _t436[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t436, 3);
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
    GemVal _t438 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t438.fn(_t438.env, NULL, 0);
    GemVal _t439 = (*gem_v_peek);
    GemVal _t440 = _t439.fn(_t439.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t440, gem_string("type")), gem_string("=")))) {
    GemVal _t441 = (*gem_v_advance);
        (void)(_t441.fn(_t441.env, NULL, 0));
    GemVal _t442 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t442.fn(_t442.env, NULL, 0);
    GemVal _t443 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t443, gem_string("tag")), gem_string("var")))) {
    GemVal _t444 = gem_v_lhs;
    GemVal _t445[] = {gem_table_get(_t444, gem_string("name")), gem_v_value};
            return gem_fn_make_assign(NULL, _t445, 2);
        }
    GemVal _t446 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t446, gem_string("tag")), gem_string("dot")))) {
    GemVal _t447 = gem_v_lhs;
    GemVal _t448 = gem_v_lhs;
    GemVal _t449[] = {gem_table_get(_t447, gem_string("object")), gem_table_get(_t448, gem_string("field")), gem_v_value};
            return gem_fn_make_dot_assign(NULL, _t449, 3);
        }
    GemVal _t450 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t450, gem_string("tag")), gem_string("index")))) {
    GemVal _t451 = gem_v_lhs;
    GemVal _t452 = gem_v_lhs;
    GemVal _t453[] = {gem_table_get(_t451, gem_string("object")), gem_table_get(_t452, gem_string("key")), gem_v_value};
            return gem_fn_make_index_assign(NULL, _t453, 3);
        }
    GemVal _t454 = (*gem_v_peek);
    GemVal _t455 = _t454.fn(_t454.env, NULL, 0);
    GemVal _t456[] = {gem_table_get(_t455, gem_string("line"))};
    GemVal _t457[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t456, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 382, _t457, 1));
    }
    GemVal _t458 = (*gem_v_peek);
    GemVal _t459 = _t458.fn(_t458.env, NULL, 0);
    GemVal _t462;
    if (gem_truthy(gem_eq(gem_table_get(_t459, gem_string("type")), gem_string("+=")))) {
        _t462 = gem_eq(gem_table_get(_t459, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t460 = (*gem_v_peek);
        GemVal _t461 = _t460.fn(_t460.env, NULL, 0);
        _t462 = gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("-="));
    }
    GemVal _t465;
    if (gem_truthy(_t462)) {
        _t465 = _t462;
    } else {
        GemVal _t463 = (*gem_v_peek);
        GemVal _t464 = _t463.fn(_t463.env, NULL, 0);
        _t465 = gem_eq(gem_table_get(_t464, gem_string("type")), gem_string("*="));
    }
    GemVal _t468;
    if (gem_truthy(_t465)) {
        _t468 = _t465;
    } else {
        GemVal _t466 = (*gem_v_peek);
        GemVal _t467 = _t466.fn(_t466.env, NULL, 0);
        _t468 = gem_eq(gem_table_get(_t467, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t468)) {
    GemVal _t469 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t469.fn(_t469.env, NULL, 0);
    GemVal _t470 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t470, gem_string("value")), gem_int(0));
    GemVal _t471 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t471.fn(_t471.env, NULL, 0);
    GemVal _t472 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t472, gem_string("tag")), gem_string("var")))) {
    GemVal _t473 = gem_v_op_tok;
    GemVal _t474[] = {gem_table_get(_t473, gem_string("line"))};
    GemVal _t475[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t474, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 391, _t475, 1));
        }
    GemVal _t476 = gem_v_lhs;
    GemVal _t477 = gem_v_lhs;
    GemVal _t478[] = {gem_table_get(_t477, gem_string("name"))};
    GemVal _t479[] = {gem_v_base_op, gem_fn_make_var(NULL, _t478, 1), gem_v_value};
    GemVal _t480[] = {gem_table_get(_t476, gem_string("name")), gem_fn_make_binop(NULL, _t479, 3)};
        return gem_fn_make_assign(NULL, _t480, 2);
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
    GemVal _t482 = (*gem_v_peek);
    GemVal gem_v_t = _t482.fn(_t482.env, NULL, 0);
    GemVal _t483 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t483, gem_string("type")), gem_string("let")))) {
    GemVal _t484 = (*gem_v_advance);
        (void)(_t484.fn(_t484.env, NULL, 0));
    GemVal _t485 = (*gem_v_expect);
    GemVal _t486[] = {gem_string("NAME")};
    GemVal _t487 = _t485.fn(_t485.env, _t486, 1);
        GemVal gem_v_name = gem_table_get(_t487, gem_string("value"));
    GemVal _t488 = (*gem_v_expect);
    GemVal _t489[] = {gem_string("=")};
        (void)(_t488.fn(_t488.env, _t489, 1));
    GemVal _t490 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t490.fn(_t490.env, NULL, 0);
    GemVal _t491[] = {gem_v_name, gem_v_value};
        return gem_fn_make_let(NULL, _t491, 2);
    }
    GemVal _t492 = gem_v_t;
    GemVal _t496;
    if (!gem_truthy(gem_eq(gem_table_get(_t492, gem_string("type")), gem_string("fn")))) {
        _t496 = gem_eq(gem_table_get(_t492, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t493 = (*gem_v_peek_at);
        GemVal _t494[] = {gem_int(1)};
        GemVal _t495 = _t493.fn(_t493.env, _t494, 1);
        _t496 = gem_eq(gem_table_get(_t495, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t496)) {
    GemVal _t497 = (*gem_v_advance);
        (void)(_t497.fn(_t497.env, NULL, 0));
    GemVal _t498 = (*gem_v_expect);
    GemVal _t499[] = {gem_string("NAME")};
    GemVal _t500 = _t498.fn(_t498.env, _t499, 1);
        GemVal gem_v_name = gem_table_get(_t500, gem_string("value"));
    GemVal _t501 = (*gem_v_expect);
    GemVal _t502[] = {gem_string("(")};
        (void)(_t501.fn(_t501.env, _t502, 1));
    GemVal _t503 = (*gem_v_parse_params);
        GemVal gem_v_params = _t503.fn(_t503.env, NULL, 0);
    GemVal _t504 = (*gem_v_expect);
    GemVal _t505[] = {gem_string(")")};
        (void)(_t504.fn(_t504.env, _t505, 1));
    GemVal _t506 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t506.fn(_t506.env, NULL, 0);
    GemVal _t507 = (*gem_v_expect);
    GemVal _t508[] = {gem_string("end")};
        (void)(_t507.fn(_t507.env, _t508, 1));
    GemVal _t509[] = {gem_v_name, gem_v_params, gem_v_body};
        return gem_fn_make_fn_def(NULL, _t509, 3);
    }
    GemVal _t510 = gem_v_t;
    GemVal _t512;
    if (gem_truthy(gem_eq(gem_table_get(_t510, gem_string("type")), gem_string("if")))) {
        _t512 = gem_eq(gem_table_get(_t510, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t511 = gem_v_t;
        _t512 = gem_eq(gem_table_get(_t511, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t512)) {
    GemVal _t513 = (*gem_v_advance);
        (void)(_t513.fn(_t513.env, NULL, 0));
    GemVal _t514 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t514.fn(_t514.env, NULL, 0);
    GemVal _t515 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t515.fn(_t515.env, NULL, 0);
        GemVal gem_v_else_body = GEM_NIL;
    GemVal _t516 = (*gem_v_peek);
    GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t517, gem_string("type")), gem_string("elif")))) {
    GemVal _t518 = gem_table_new();
    GemVal _t519 = (*gem_v_parse_stmt);
    gem_table_set(_t518, gem_int(0), _t519.fn(_t519.env, NULL, 0));
            gem_v_else_body = _t518;
        } else {
    GemVal _t520 = (*gem_v_peek);
    GemVal _t521 = _t520.fn(_t520.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t521, gem_string("type")), gem_string("else")))) {
    GemVal _t522 = (*gem_v_advance);
                (void)(_t522.fn(_t522.env, NULL, 0));
    GemVal _t523 = (*gem_v_parse_body);
                gem_v_else_body = _t523.fn(_t523.env, NULL, 0);
            }
        }
    GemVal _t524 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t524, gem_string("type")), gem_string("if")))) {
    GemVal _t525 = (*gem_v_expect);
    GemVal _t526[] = {gem_string("end")};
            (void)(_t525.fn(_t525.env, _t526, 1));
        }
    GemVal _t527[] = {gem_v_cond, gem_v_then_body, gem_v_else_body};
        return gem_fn_make_if(NULL, _t527, 3);
    }
    GemVal _t528 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t528, gem_string("type")), gem_string("while")))) {
    GemVal _t529 = (*gem_v_advance);
        (void)(_t529.fn(_t529.env, NULL, 0));
    GemVal _t530 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t530.fn(_t530.env, NULL, 0);
    GemVal _t531 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t531.fn(_t531.env, NULL, 0);
    GemVal _t532 = (*gem_v_expect);
    GemVal _t533[] = {gem_string("end")};
        (void)(_t532.fn(_t532.env, _t533, 1));
    GemVal _t534[] = {gem_v_cond, gem_v_wbody};
        return gem_fn_make_while(NULL, _t534, 2);
    }
    GemVal _t535 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t535, gem_string("type")), gem_string("for")))) {
    GemVal _t536 = (*gem_v_advance);
        (void)(_t536.fn(_t536.env, NULL, 0));
    GemVal _t537 = (*gem_v_expect);
    GemVal _t538[] = {gem_string("NAME")};
    GemVal _t539 = _t537.fn(_t537.env, _t538, 1);
        GemVal gem_v_var_name = gem_table_get(_t539, gem_string("value"));
    GemVal _t540 = (*gem_v_peek);
    GemVal _t541 = _t540.fn(_t540.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t541, gem_string("type")), gem_string("in")))) {
    GemVal _t542 = (*gem_v_advance);
            (void)(_t542.fn(_t542.env, NULL, 0));
    GemVal _t543 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t543.fn(_t543.env, NULL, 0);
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
    GemVal _t544[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t544, 1));
    GemVal _t545[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t545, 1));
    GemVal _t546 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t546.fn(_t546.env, NULL, 0);
    GemVal _t547 = (*gem_v_expect);
    GemVal _t548[] = {gem_string("end")};
            (void)(_t547.fn(_t547.env, _t548, 1));
    GemVal _t549 = gem_table_new();
            GemVal gem_v_inner_stmts = _t549;
    GemVal _t550[] = {gem_v_items_var};
    GemVal _t551[] = {gem_v_idx_var};
    GemVal _t552[] = {gem_fn_make_var(NULL, _t550, 1), gem_fn_make_var(NULL, _t551, 1)};
    GemVal _t553[] = {gem_v_var_name, gem_fn_make_index(NULL, _t552, 2)};
    GemVal _t554[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t553, 2)};
            (void)(gem_push_fn(NULL, _t554, 2));
    GemVal _t555[] = {gem_v_idx_var};
    GemVal _t556[] = {gem_int(1)};
    GemVal _t557[] = {gem_string("+"), gem_fn_make_var(NULL, _t555, 1), gem_fn_make_int(NULL, _t556, 1)};
    GemVal _t558[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t557, 3)};
    GemVal _t559[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t558, 2)};
            (void)(gem_push_fn(NULL, _t559, 2));
            {
                GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t560[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t560, 1);
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
                    GemVal gem_v_bi = gem_v__for_i_1;
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t561[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t561, 2));
                }
            }
    GemVal _t562[] = {gem_v_idx_var};
    GemVal _t563[] = {gem_string("len")};
    GemVal _t564 = gem_table_new();
    GemVal _t565[] = {gem_v_items_var};
    gem_table_set(_t564, gem_int(0), gem_fn_make_var(NULL, _t565, 1));
    GemVal _t566[] = {gem_fn_make_var(NULL, _t563, 1), _t564, gem_int(0)};
    GemVal _t567[] = {gem_string("<"), gem_fn_make_var(NULL, _t562, 1), gem_fn_make_call(NULL, _t566, 3)};
    GemVal _t568[] = {gem_fn_make_binop(NULL, _t567, 3), gem_v_inner_stmts};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t568, 2);
    GemVal _t569 = gem_table_new();
    gem_table_set(_t569, gem_string("tag"), gem_string("block"));
    GemVal _t570 = gem_table_new();
    GemVal _t571[] = {gem_v_items_var, gem_v_items_expr};
    gem_table_set(_t570, gem_int(0), gem_fn_make_let(NULL, _t571, 2));
    GemVal _t572[] = {gem_int(0)};
    GemVal _t573[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t572, 1)};
    gem_table_set(_t570, gem_int(1), gem_fn_make_let(NULL, _t573, 2));
    gem_table_set(_t570, gem_int(2), gem_v_while_node);
    gem_table_set(_t569, gem_string("stmts"), _t570);
            return _t569;
        } else {
    GemVal _t574 = (*gem_v_peek);
    GemVal _t575 = _t574.fn(_t574.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t575, gem_string("type")), gem_string("=")))) {
    GemVal _t576 = (*gem_v_advance);
                (void)(_t576.fn(_t576.env, NULL, 0));
    GemVal _t577 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t577.fn(_t577.env, NULL, 0);
    GemVal _t578 = (*gem_v_expect);
    GemVal _t579[] = {gem_string(",")};
                (void)(_t578.fn(_t578.env, _t579, 1));
    GemVal _t580 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t580.fn(_t580.env, NULL, 0);
    GemVal _t581 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t581.fn(_t581.env, NULL, 0);
    GemVal _t582 = (*gem_v_expect);
    GemVal _t583[] = {gem_string("end")};
                (void)(_t582.fn(_t582.env, _t583, 1));
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
    GemVal _t584[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t584, 1));
    GemVal _t585[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t585, 1));
    GemVal _t586 = gem_table_new();
                GemVal gem_v_inner_stmts = _t586;
    GemVal _t587[] = {gem_v_idx_var};
    GemVal _t588[] = {gem_v_var_name, gem_fn_make_var(NULL, _t587, 1)};
    GemVal _t589[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t588, 2)};
                (void)(gem_push_fn(NULL, _t589, 2));
    GemVal _t590[] = {gem_v_idx_var};
    GemVal _t591[] = {gem_int(1)};
    GemVal _t592[] = {gem_string("+"), gem_fn_make_var(NULL, _t590, 1), gem_fn_make_int(NULL, _t591, 1)};
    GemVal _t593[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t592, 3)};
    GemVal _t594[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t593, 2)};
                (void)(gem_push_fn(NULL, _t594, 2));
                {
                    GemVal gem_v__for_i_2 = gem_int(0);
    GemVal _t595[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t595, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
                        GemVal gem_v_bi = gem_v__for_i_2;
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
    GemVal _t596[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t596, 2));
                    }
                }
    GemVal _t597[] = {gem_v_idx_var};
    GemVal _t598[] = {gem_v_limit_var};
    GemVal _t599[] = {gem_string("<"), gem_fn_make_var(NULL, _t597, 1), gem_fn_make_var(NULL, _t598, 1)};
    GemVal _t600[] = {gem_fn_make_binop(NULL, _t599, 3), gem_v_inner_stmts};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t600, 2);
    GemVal _t601 = gem_table_new();
    gem_table_set(_t601, gem_string("tag"), gem_string("block"));
    GemVal _t602 = gem_table_new();
    GemVal _t603[] = {gem_v_idx_var, gem_v_start_expr};
    gem_table_set(_t602, gem_int(0), gem_fn_make_let(NULL, _t603, 2));
    GemVal _t604[] = {gem_v_limit_var, gem_v_end_expr};
    gem_table_set(_t602, gem_int(1), gem_fn_make_let(NULL, _t604, 2));
    gem_table_set(_t602, gem_int(2), gem_v_while_node);
    gem_table_set(_t601, gem_string("stmts"), _t602);
                return _t601;
            } else {
    GemVal _t605 = (*gem_v_peek);
    GemVal _t606 = _t605.fn(_t605.env, NULL, 0);
    GemVal _t607[] = {gem_table_get(_t606, gem_string("line"))};
    GemVal _t608[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t607, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 499, _t608, 1));
            }
        }
    }
    GemVal _t609 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t609, gem_string("type")), gem_string("match")))) {
    GemVal _t610 = (*gem_v_advance);
        (void)(_t610.fn(_t610.env, NULL, 0));
    GemVal _t611 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t611.fn(_t611.env, NULL, 0);
    GemVal _t612 = (*gem_v_skip_nl);
        (void)(_t612.fn(_t612.env, NULL, 0));
    GemVal _t613 = gem_table_new();
        GemVal gem_v_whens = _t613;
        while (1) {
            GemVal _t614 = (*gem_v_peek);
            GemVal _t615 = _t614.fn(_t614.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t615, gem_string("type")), gem_string("when")))) break;
    GemVal _t616 = (*gem_v_advance);
            (void)(_t616.fn(_t616.env, NULL, 0));
    GemVal _t617 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t617.fn(_t617.env, NULL, 0);
    GemVal _t618 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t618.fn(_t618.env, NULL, 0);
    GemVal _t619[] = {gem_v_wval, gem_v_wbody};
    GemVal _t620[] = {gem_v_whens, gem_fn_make_when(NULL, _t619, 2)};
            (void)(gem_push_fn(NULL, _t620, 2));
    GemVal _t621 = (*gem_v_skip_nl);
            (void)(_t621.fn(_t621.env, NULL, 0));
        }
        GemVal gem_v_else_body = GEM_NIL;
    GemVal _t622 = (*gem_v_peek);
    GemVal _t623 = _t622.fn(_t622.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t623, gem_string("type")), gem_string("else")))) {
    GemVal _t624 = (*gem_v_advance);
            (void)(_t624.fn(_t624.env, NULL, 0));
    GemVal _t625 = (*gem_v_parse_body);
            gem_v_else_body = _t625.fn(_t625.env, NULL, 0);
        }
    GemVal _t626 = (*gem_v_expect);
    GemVal _t627[] = {gem_string("end")};
        (void)(_t626.fn(_t626.env, _t627, 1));
    GemVal _t628[] = {gem_v_target, gem_v_whens, gem_v_else_body};
        return gem_fn_make_match(NULL, _t628, 3);
    }
    GemVal _t629 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t629, gem_string("type")), gem_string("return")))) {
    GemVal _t630 = (*gem_v_advance);
        (void)(_t630.fn(_t630.env, NULL, 0));
        GemVal gem_v_value = GEM_NIL;
    GemVal _t631 = (*gem_v_peek);
    GemVal _t632 = _t631.fn(_t631.env, NULL, 0);
    GemVal _t635;
    if (!gem_truthy(gem_neq(gem_table_get(_t632, gem_string("type")), gem_string("NEWLINE")))) {
        _t635 = gem_neq(gem_table_get(_t632, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t633 = (*gem_v_peek);
        GemVal _t634 = _t633.fn(_t633.env, NULL, 0);
        _t635 = gem_neq(gem_table_get(_t634, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t638;
    if (!gem_truthy(_t635)) {
        _t638 = _t635;
    } else {
        GemVal _t636 = (*gem_v_peek);
        GemVal _t637 = _t636.fn(_t636.env, NULL, 0);
        _t638 = gem_neq(gem_table_get(_t637, gem_string("type")), gem_string("end"));
    }
    GemVal _t641;
    if (!gem_truthy(_t638)) {
        _t641 = _t638;
    } else {
        GemVal _t639 = (*gem_v_peek);
        GemVal _t640 = _t639.fn(_t639.env, NULL, 0);
        _t641 = gem_neq(gem_table_get(_t640, gem_string("type")), gem_string("elif"));
    }
    GemVal _t644;
    if (!gem_truthy(_t641)) {
        _t644 = _t641;
    } else {
        GemVal _t642 = (*gem_v_peek);
        GemVal _t643 = _t642.fn(_t642.env, NULL, 0);
        _t644 = gem_neq(gem_table_get(_t643, gem_string("type")), gem_string("else"));
    }
    GemVal _t647;
    if (!gem_truthy(_t644)) {
        _t647 = _t644;
    } else {
        GemVal _t645 = (*gem_v_peek);
        GemVal _t646 = _t645.fn(_t645.env, NULL, 0);
        _t647 = gem_neq(gem_table_get(_t646, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t647)) {
    GemVal _t648 = (*gem_v_parse_expr);
            gem_v_value = _t648.fn(_t648.env, NULL, 0);
        }
    GemVal _t649[] = {gem_v_value};
        return gem_fn_make_return(NULL, _t649, 1);
    }
    GemVal _t650 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t650, gem_string("type")), gem_string("break")))) {
    GemVal _t651 = (*gem_v_advance);
        (void)(_t651.fn(_t651.env, NULL, 0));
        return gem_fn_make_break(NULL, NULL, 0);
    }
    GemVal _t652 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("continue")))) {
    GemVal _t653 = (*gem_v_advance);
        (void)(_t653.fn(_t653.env, NULL, 0));
        return gem_fn_make_continue(NULL, NULL, 0);
    }
    GemVal _t654 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t654, gem_string("type")), gem_string("load")))) {
    GemVal _t655 = (*gem_v_advance);
        (void)(_t655.fn(_t655.env, NULL, 0));
    GemVal _t656 = (*gem_v_expect);
    GemVal _t657[] = {gem_string("STRING")};
    GemVal _t658 = _t656.fn(_t656.env, _t657, 1);
        GemVal gem_v_path = gem_table_get(_t658, gem_string("value"));
    GemVal _t659[] = {gem_v_path};
        return gem_fn_make_load(NULL, _t659, 1);
    }
    GemVal _t660 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t660, gem_string("type")), gem_string("extern")))) {
    GemVal _t661 = (*gem_v_advance);
        (void)(_t661.fn(_t661.env, NULL, 0));
    GemVal _t662 = (*gem_v_peek);
    GemVal _t663 = _t662.fn(_t662.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t663, gem_string("type")), gem_string("fn")))) {
    GemVal _t664 = (*gem_v_advance);
            (void)(_t664.fn(_t664.env, NULL, 0));
    GemVal _t665 = (*gem_v_expect);
    GemVal _t666[] = {gem_string("NAME")};
    GemVal _t667 = _t665.fn(_t665.env, _t666, 1);
            GemVal gem_v_name = gem_table_get(_t667, gem_string("value"));
    GemVal _t668 = (*gem_v_expect);
    GemVal _t669[] = {gem_string("(")};
            (void)(_t668.fn(_t668.env, _t669, 1));
    GemVal _t670 = gem_table_new();
            GemVal gem_v_eparams = _t670;
    GemVal _t671 = (*gem_v_peek);
    GemVal _t672 = _t671.fn(_t671.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t672, gem_string("type")), gem_string(")")))) {
    GemVal _t673 = (*gem_v_expect);
    GemVal _t674[] = {gem_string("NAME")};
    GemVal _t675 = _t673.fn(_t673.env, _t674, 1);
                GemVal gem_v_pname = gem_table_get(_t675, gem_string("value"));
    GemVal _t676 = (*gem_v_expect);
    GemVal _t677[] = {gem_string(":")};
                (void)(_t676.fn(_t676.env, _t677, 1));
    GemVal _t678 = (*gem_v_expect);
    GemVal _t679[] = {gem_string("NAME")};
    GemVal _t680 = _t678.fn(_t678.env, _t679, 1);
                GemVal gem_v_ptype = gem_table_get(_t680, gem_string("value"));
    GemVal _t681[] = {gem_v_pname, gem_v_ptype};
    GemVal _t682[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t681, 2)};
                (void)(gem_push_fn(NULL, _t682, 2));
                while (1) {
                    GemVal _t683 = (*gem_v_peek);
                    GemVal _t684 = _t683.fn(_t683.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t684, gem_string("type")), gem_string(",")))) break;
    GemVal _t685 = (*gem_v_advance);
                    (void)(_t685.fn(_t685.env, NULL, 0));
    GemVal _t686 = (*gem_v_expect);
    GemVal _t687[] = {gem_string("NAME")};
    GemVal _t688 = _t686.fn(_t686.env, _t687, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t688, gem_string("value"));
    GemVal _t689 = (*gem_v_expect);
    GemVal _t690[] = {gem_string(":")};
                    (void)(_t689.fn(_t689.env, _t690, 1));
    GemVal _t691 = (*gem_v_expect);
    GemVal _t692[] = {gem_string("NAME")};
    GemVal _t693 = _t691.fn(_t691.env, _t692, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t693, gem_string("value"));
    GemVal _t694[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t695[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t694, 2)};
                    (void)(gem_push_fn(NULL, _t695, 2));
                }
            }
    GemVal _t696 = (*gem_v_expect);
    GemVal _t697[] = {gem_string(")")};
            (void)(_t696.fn(_t696.env, _t697, 1));
            GemVal gem_v_ret_type = gem_string("Nil");
    GemVal _t698 = (*gem_v_peek);
    GemVal _t699 = _t698.fn(_t698.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t699, gem_string("type")), gem_string("->")))) {
    GemVal _t700 = (*gem_v_advance);
                (void)(_t700.fn(_t700.env, NULL, 0));
    GemVal _t701 = (*gem_v_expect);
    GemVal _t702[] = {gem_string("NAME")};
    GemVal _t703 = _t701.fn(_t701.env, _t702, 1);
                gem_v_ret_type = gem_table_get(_t703, gem_string("value"));
            }
    GemVal _t704[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            return gem_fn_make_extern_fn(NULL, _t704, 3);
        } else {
    GemVal _t705 = (*gem_v_peek);
    GemVal _t706 = _t705.fn(_t705.env, NULL, 0);
    GemVal _t709;
    if (!gem_truthy(gem_eq(gem_table_get(_t706, gem_string("type")), gem_string("NAME")))) {
        _t709 = gem_eq(gem_table_get(_t706, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t707 = (*gem_v_peek);
        GemVal _t708 = _t707.fn(_t707.env, NULL, 0);
        _t709 = gem_eq(gem_table_get(_t708, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t709)) {
    GemVal _t710 = (*gem_v_advance);
                (void)(_t710.fn(_t710.env, NULL, 0));
    GemVal _t711 = (*gem_v_expect);
    GemVal _t712[] = {gem_string("STRING")};
    GemVal _t713 = _t711.fn(_t711.env, _t712, 1);
                GemVal gem_v_path = gem_table_get(_t713, gem_string("value"));
    GemVal _t714[] = {gem_v_path};
                return gem_fn_make_extern_include(NULL, _t714, 1);
            } else {
    GemVal _t715 = (*gem_v_peek);
    GemVal _t716 = _t715.fn(_t715.env, NULL, 0);
    GemVal _t717[] = {gem_table_get(_t716, gem_string("line"))};
    GemVal _t718[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t717, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 587, _t718, 1));
            }
        }
    }
    GemVal _t719 = (*gem_v_parse_expr);
    return _t719.fn(_t719.env, NULL, 0);
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
    GemVal _t721 = gem_table_new();
    GemVal gem_v_stmts = _t721;
    GemVal _t722 = (*gem_v_skip_nl);
    (void)(_t722.fn(_t722.env, NULL, 0));
    while (1) {
        GemVal _t723 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t723.fn(_t723.env, NULL, 0)))) break;
    GemVal _t724 = (*gem_v_parse_stmt);
    GemVal _t725[] = {gem_v_stmts, _t724.fn(_t724.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t725, 2));
    GemVal _t726 = (*gem_v_skip_nl);
        (void)(_t726.fn(_t726.env, NULL, 0));
    }
    GemVal _t727[] = {gem_v_stmts};
    return gem_fn_make_program(NULL, _t727, 1);
}

static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc) {
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
    struct _closure__anon_1 *_t141 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t141->gem_v_pos = gem_v_pos;
    _t141->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t141);
    struct _closure__anon_2 *_t142 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t142->gem_v_pos = gem_v_pos;
    _t142->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t142);
    struct _closure__anon_3 *_t143 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t143->gem_v_pos = gem_v_pos;
    _t143->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t143);
    struct _closure__anon_4 *_t145 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t145->gem_v_pos = gem_v_pos;
    _t145->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t145);
    struct _closure__anon_5 *_t151 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t151->gem_v_pos = gem_v_pos;
    _t151->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t151);
    struct _closure__anon_6 *_t153 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t153->gem_v_pos = gem_v_pos;
    _t153->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t153);
    struct _closure__anon_7 *_t167 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t167->gem_v_advance = gem_v_advance;
    _t167->gem_v_expect = gem_v_expect;
    _t167->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t167);
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
    struct _closure__anon_8 *_t186 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t186->gem_v_at_end = gem_v_at_end;
    _t186->gem_v_parse_stmt = gem_v_parse_stmt;
    _t186->gem_v_peek = gem_v_peek;
    _t186->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t186);
    struct _closure__anon_9 *_t196 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t196->gem_v_at_end = gem_v_at_end;
    _t196->gem_v_parse_stmt = gem_v_parse_stmt;
    _t196->gem_v_peek = gem_v_peek;
    _t196->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t196);
    struct _closure__anon_10 *_t282 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t282->gem_v_advance = gem_v_advance;
    _t282->gem_v_expect = gem_v_expect;
    _t282->gem_v_parse_expr = gem_v_parse_expr;
    _t282->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t282->gem_v_parse_params = gem_v_parse_params;
    _t282->gem_v_peek = gem_v_peek;
    _t282->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t282);
    struct _closure__anon_11 *_t361 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t361->gem_v_advance = gem_v_advance;
    _t361->gem_v_expect = gem_v_expect;
    _t361->gem_v_parse_atom = gem_v_parse_atom;
    _t361->gem_v_parse_expr = gem_v_parse_expr;
    _t361->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t361->gem_v_parse_params = gem_v_parse_params;
    _t361->gem_v_peek = gem_v_peek;
    _t361->gem_v_peek_at = gem_v_peek_at;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t361);
    struct _closure__anon_12 *_t368 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t368->gem_v_advance = gem_v_advance;
    _t368->gem_v_parse_call = gem_v_parse_call;
    _t368->gem_v_parse_unary = gem_v_parse_unary;
    _t368->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t368);
    struct _closure__anon_13 *_t382 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t382->gem_v_advance = gem_v_advance;
    _t382->gem_v_parse_unary = gem_v_parse_unary;
    _t382->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t382);
    struct _closure__anon_14 *_t393 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t393->gem_v_advance = gem_v_advance;
    _t393->gem_v_parse_mul = gem_v_parse_mul;
    _t393->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t393);
    struct _closure__anon_15 *_t416 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t416->gem_v_advance = gem_v_advance;
    _t416->gem_v_parse_add = gem_v_parse_add;
    _t416->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t416);
    struct _closure__anon_16 *_t423 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t423->gem_v_advance = gem_v_advance;
    _t423->gem_v_parse_compare = gem_v_parse_compare;
    _t423->gem_v_parse_not = gem_v_parse_not;
    _t423->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t423);
    struct _closure__anon_17 *_t430 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t430->gem_v_advance = gem_v_advance;
    _t430->gem_v_parse_not = gem_v_parse_not;
    _t430->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t430);
    struct _closure__anon_18 *_t437 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t437->gem_v_advance = gem_v_advance;
    _t437->gem_v_parse_and = gem_v_parse_and;
    _t437->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t437);
    struct _closure__anon_19 *_t481 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t481->gem_v_advance = gem_v_advance;
    _t481->gem_v_parse_expr = gem_v_parse_expr;
    _t481->gem_v_parse_or = gem_v_parse_or;
    _t481->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t481);
    *gem_v_parse_expr = gem_v_parse_assign_fn;
    struct _closure__anon_20 *_t720 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t720->gem_v_advance = gem_v_advance;
    _t720->gem_v_expect = gem_v_expect;
    _t720->gem_v_gensym_counter = gem_v_gensym_counter;
    _t720->gem_v_parse_body = gem_v_parse_body;
    _t720->gem_v_parse_expr = gem_v_parse_expr;
    _t720->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t720->gem_v_parse_params = gem_v_parse_params;
    _t720->gem_v_parse_stmt = gem_v_parse_stmt;
    _t720->gem_v_peek = gem_v_peek;
    _t720->gem_v_peek_at = gem_v_peek_at;
    _t720->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t720);
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
    struct _closure__anon_21 *_t728 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t728->gem_v_at_end = gem_v_at_end;
    _t728->gem_v_parse_stmt = gem_v_parse_stmt;
    _t728->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t728);
    GemVal _t729 = gem_table_new();
    gem_table_set(_t729, gem_string("parse"), gem_v_parse);
    return _t729;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    {
        GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t730[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t730, 1);
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
    GemVal _t731[] = {gem_string("    "), gem_v_indent};
    return gem_fn_repeat_str(NULL, _t731, 2);
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t732[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t732, 1);
    GemVal gem_v_has_dot = gem_bool(0);
    {
        GemVal gem_v__for_i_3 = gem_int(0);
    GemVal _t733[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t733, 1);
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
    GemVal _t734[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t734, 1);
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
    GemVal _t735 = gem_table_new();
    GemVal gem_v_result = _t735;
    GemVal _t736[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t736, 1);
    GemVal gem_v_i = gem_int(0);
    while (1) {
        GemVal _t737[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t737, 1)))) break;
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t738[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_fn_keys(NULL, _t738, 1);
    GemVal gem_v_j = gem_int(0);
    while (1) {
        GemVal _t739[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t739, 1)))) break;
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t740 = gem_table_new();
    GemVal gem_v_result = _t740;
    GemVal _t741[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t741, 1);
    GemVal gem_v_i = gem_int(0);
    while (1) {
        GemVal _t742[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t742, 1)))) break;
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t743[] = {gem_v_s};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t743, 1);
    GemVal gem_v_i = gem_int(1);
    while (1) {
        GemVal _t744[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t744, 1)))) break;
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
        while (1) {
            GemVal _t745;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t745 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t745 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t745)) break;
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
    GemVal _t746 = gem_table_new();
    GemVal gem_v_s = _t746;
    {
        GemVal gem_v__for_items_5 = gem_v_arr;
        GemVal gem_v__for_i_5 = gem_int(0);
        while (1) {
            GemVal _t747[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t747, 1)))) break;
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
    GemVal _t753[] = {(*gem_v_tmp_counter)};
    return gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t753, 1));
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t755[] = {(*gem_v_anon_counter)};
    return gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t755, 1));
}

struct _closure__anon_24 {
};
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    return gem_add(gem_string("gem_v_"), gem_v_name);
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
    GemVal _t757[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t757, 1), gem_string("table")))) {
        return GEM_NIL;
    }
    GemVal _t758 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t758, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
        return GEM_NIL;
    }
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
    GemVal _t759 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t759, gem_string("name"));
    GemVal _t760[] = {gem_v_defined, gem_v_name};
    GemVal _t762;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t760, 2)))) {
        _t762 = gem_not(gem_fn_set_contains(NULL, _t760, 2));
    } else {
        GemVal _t761[] = {(*gem_v_builtins), gem_v_name};
        _t762 = gem_not(gem_fn_set_contains(NULL, _t761, 2));
    }
    GemVal _t764;
    if (!gem_truthy(_t762)) {
        _t764 = _t762;
    } else {
        GemVal _t763[] = {(*gem_v_defined_fns), gem_v_name};
        _t764 = gem_not(gem_fn_set_contains(NULL, _t763, 2));
    }
        if (gem_truthy(_t764)) {
    GemVal _t765[] = {gem_v_free, gem_v_name};
            return gem_fn_set_add(NULL, _t765, 2);
        } else {
            return GEM_NIL;
        }
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t766 = gem_v_node;
    GemVal _t767 = (*gem_v_collect_free_node);
    GemVal _t768[] = {gem_table_get(_t766, gem_string("value")), gem_v_defined, gem_v_free};
            return _t767.fn(_t767.env, _t768, 3);
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t769 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t769, gem_string("name"));
    GemVal _t770[] = {gem_v_defined, gem_v_name};
    GemVal _t772;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t770, 2)))) {
        _t772 = gem_not(gem_fn_set_contains(NULL, _t770, 2));
    } else {
        GemVal _t771[] = {(*gem_v_builtins), gem_v_name};
        _t772 = gem_not(gem_fn_set_contains(NULL, _t771, 2));
    }
    GemVal _t774;
    if (!gem_truthy(_t772)) {
        _t774 = _t772;
    } else {
        GemVal _t773[] = {(*gem_v_defined_fns), gem_v_name};
        _t774 = gem_not(gem_fn_set_contains(NULL, _t773, 2));
    }
                if (gem_truthy(_t774)) {
    GemVal _t775[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t775, 2));
                }
    GemVal _t776 = gem_v_node;
    GemVal _t777 = (*gem_v_collect_free_node);
    GemVal _t778[] = {gem_table_get(_t776, gem_string("value")), gem_v_defined, gem_v_free};
                return _t777.fn(_t777.env, _t778, 3);
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t779 = gem_v_node;
    GemVal _t780[] = {gem_table_get(_t779, gem_string("params"))};
    GemVal _t781[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t780, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t781, 2);
    GemVal _t782 = gem_v_node;
    GemVal _t783 = (*gem_v_collect_free_vars);
    GemVal _t784[] = {gem_table_get(_t782, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t783.fn(_t783.env, _t784, 2);
    GemVal _t785[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_fn_keys(NULL, _t785, 1);
                    GemVal gem_v_ifi = gem_int(0);
                    while (1) {
                        GemVal _t786[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t786, 1)))) break;
    GemVal _t787[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t787, 2));
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    return GEM_NIL;
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        return GEM_NIL;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t788 = gem_v_node;
    GemVal _t789 = (*gem_v_collect_free_node);
    GemVal _t790[] = {gem_table_get(_t788, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t789.fn(_t789.env, _t790, 3));
    GemVal _t791 = gem_table_new();
    GemVal _t792[] = {gem_v_defined, _t791};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t792, 2);
                            GemVal gem_v_thens = gem_table_get(gem_v_node, gem_string("then"));
                            {
                                GemVal gem_v__for_i_6 = gem_int(0);
    GemVal _t793[] = {gem_v_thens};
                                GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t793, 1);
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
                                    GemVal gem_v_i = gem_v__for_i_6;
                                    gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
    GemVal _t794 = (*gem_v_collect_free_node);
    GemVal _t795[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t794.fn(_t794.env, _t795, 3));
    GemVal _t796[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t798;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t796, 1), gem_string("table")))) {
        _t798 = gem_eq(gem_type_fn(NULL, _t796, 1), gem_string("table"));
    } else {
        GemVal _t797 = gem_table_get(gem_v_thens, gem_v_i);
        _t798 = gem_eq(gem_table_get(_t797, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t798)) {
    GemVal _t799 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t800[] = {gem_v_d, gem_table_get(_t799, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t800, 2));
                                    }
                                }
                            }
                            GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
    GemVal _t801 = gem_table_new();
    GemVal _t802[] = {gem_v_defined, _t801};
                                gem_v_d = gem_fn_set_union(NULL, _t802, 2);
                                {
                                    GemVal gem_v__for_i_7 = gem_int(0);
    GemVal _t803[] = {gem_v_el};
                                    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t803, 1);
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
                                        GemVal gem_v_i = gem_v__for_i_7;
                                        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
    GemVal _t804 = (*gem_v_collect_free_node);
    GemVal _t805[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t804.fn(_t804.env, _t805, 3));
    GemVal _t806[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t808;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t806, 1), gem_string("table")))) {
        _t808 = gem_eq(gem_type_fn(NULL, _t806, 1), gem_string("table"));
    } else {
        GemVal _t807 = gem_table_get(gem_v_el, gem_v_i);
        _t808 = gem_eq(gem_table_get(_t807, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t808)) {
    GemVal _t809 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t810[] = {gem_v_d, gem_table_get(_t809, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t810, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                return GEM_NIL;
                            }
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t811 = gem_v_node;
    GemVal _t812 = (*gem_v_collect_free_node);
    GemVal _t813[] = {gem_table_get(_t811, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t812.fn(_t812.env, _t813, 3));
    GemVal _t814 = gem_table_new();
    GemVal _t815[] = {gem_v_defined, _t814};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t815, 2);
                                {
                                    GemVal gem_v__for_i_8 = gem_int(0);
    GemVal _t816 = gem_v_node;
    GemVal _t817[] = {gem_table_get(_t816, gem_string("body"))};
                                    GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t817, 1);
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
                                        GemVal gem_v_i = gem_v__for_i_8;
                                        gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
    GemVal _t818 = gem_v_node;
    GemVal _t819 = (*gem_v_collect_free_node);
    GemVal _t820[] = {gem_table_get(gem_table_get(_t818, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t819.fn(_t819.env, _t820, 3));
    GemVal _t821 = gem_v_node;
    GemVal _t822[] = {gem_table_get(gem_table_get(_t821, gem_string("body")), gem_v_i)};
    GemVal _t825;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t822, 1), gem_string("table")))) {
        _t825 = gem_eq(gem_type_fn(NULL, _t822, 1), gem_string("table"));
    } else {
        GemVal _t823 = gem_v_node;
        GemVal _t824 = gem_table_get(gem_table_get(_t823, gem_string("body")), gem_v_i);
        _t825 = gem_eq(gem_table_get(_t824, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t825)) {
    GemVal _t826 = gem_v_node;
    GemVal _t827 = gem_table_get(gem_table_get(_t826, gem_string("body")), gem_v_i);
    GemVal _t828[] = {gem_v_d, gem_table_get(_t827, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t828, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t829 = gem_v_node;
    GemVal _t830 = (*gem_v_collect_free_node);
    GemVal _t831[] = {gem_table_get(_t829, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t830.fn(_t830.env, _t831, 3));
                                    {
                                        GemVal gem_v__for_i_10 = gem_int(0);
    GemVal _t832 = gem_v_node;
    GemVal _t833[] = {gem_table_get(_t832, gem_string("whens"))};
                                        GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t833, 1);
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
                                            GemVal gem_v_i = gem_v__for_i_10;
                                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
    GemVal _t834 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t834, gem_string("whens")), gem_v_i);
    GemVal _t835 = gem_v_w;
    GemVal _t836 = (*gem_v_collect_free_node);
    GemVal _t837[] = {gem_table_get(_t835, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t836.fn(_t836.env, _t837, 3));
    GemVal _t838 = gem_table_new();
    GemVal _t839[] = {gem_v_defined, _t838};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t839, 2);
                                            {
                                                GemVal gem_v__for_i_9 = gem_int(0);
    GemVal _t840 = gem_v_w;
    GemVal _t841[] = {gem_table_get(_t840, gem_string("body"))};
                                                GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t841, 1);
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
                                                    GemVal gem_v_j = gem_v__for_i_9;
                                                    gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
    GemVal _t842 = gem_v_w;
    GemVal _t843 = (*gem_v_collect_free_node);
    GemVal _t844[] = {gem_table_get(gem_table_get(_t842, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t843.fn(_t843.env, _t844, 3));
    GemVal _t845 = gem_v_w;
    GemVal _t846[] = {gem_table_get(gem_table_get(_t845, gem_string("body")), gem_v_j)};
    GemVal _t849;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t846, 1), gem_string("table")))) {
        _t849 = gem_eq(gem_type_fn(NULL, _t846, 1), gem_string("table"));
    } else {
        GemVal _t847 = gem_v_w;
        GemVal _t848 = gem_table_get(gem_table_get(_t847, gem_string("body")), gem_v_j);
        _t849 = gem_eq(gem_table_get(_t848, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t849)) {
    GemVal _t850 = gem_v_w;
    GemVal _t851 = gem_table_get(gem_table_get(_t850, gem_string("body")), gem_v_j);
    GemVal _t852[] = {gem_v_d, gem_table_get(_t851, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t852, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
    GemVal _t853 = gem_table_new();
    GemVal _t854[] = {gem_v_defined, _t853};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t854, 2);
                                        {
                                            GemVal gem_v__for_i_11 = gem_int(0);
    GemVal _t855[] = {gem_v_el};
                                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t855, 1);
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
                                                GemVal gem_v_j = gem_v__for_i_11;
                                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
    GemVal _t856 = (*gem_v_collect_free_node);
    GemVal _t857[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t856.fn(_t856.env, _t857, 3));
    GemVal _t858[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t860;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t858, 1), gem_string("table")))) {
        _t860 = gem_eq(gem_type_fn(NULL, _t858, 1), gem_string("table"));
    } else {
        GemVal _t859 = gem_table_get(gem_v_el, gem_v_j);
        _t860 = gem_eq(gem_table_get(_t859, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t860)) {
    GemVal _t861 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t862[] = {gem_v_d, gem_table_get(_t861, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t862, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        return GEM_NIL;
                                    }
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
    GemVal _t863 = gem_v_node;
    GemVal _t864 = (*gem_v_collect_free_node);
    GemVal _t865[] = {gem_table_get(_t863, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t864.fn(_t864.env, _t865, 3));
    GemVal _t866 = gem_table_new();
    GemVal _t867[] = {gem_v_defined, _t866};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t867, 2);
                                        {
                                            GemVal gem_v__for_i_12 = gem_int(0);
    GemVal _t868 = gem_v_node;
    GemVal _t869[] = {gem_table_get(_t868, gem_string("body"))};
                                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t869, 1);
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
                                                GemVal gem_v_i = gem_v__for_i_12;
                                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
    GemVal _t870 = gem_v_node;
    GemVal _t871 = (*gem_v_collect_free_node);
    GemVal _t872[] = {gem_table_get(gem_table_get(_t870, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t871.fn(_t871.env, _t872, 3));
    GemVal _t873 = gem_v_node;
    GemVal _t874[] = {gem_table_get(gem_table_get(_t873, gem_string("body")), gem_v_i)};
    GemVal _t877;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t874, 1), gem_string("table")))) {
        _t877 = gem_eq(gem_type_fn(NULL, _t874, 1), gem_string("table"));
    } else {
        GemVal _t875 = gem_v_node;
        GemVal _t876 = gem_table_get(gem_table_get(_t875, gem_string("body")), gem_v_i);
        _t877 = gem_eq(gem_table_get(_t876, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t877)) {
    GemVal _t878 = gem_v_node;
    GemVal _t879 = gem_table_get(gem_table_get(_t878, gem_string("body")), gem_v_i);
    GemVal _t880[] = {gem_v_d, gem_table_get(_t879, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t880, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t881 = gem_table_new();
    GemVal _t882[] = {gem_v_defined, _t881};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t882, 2);
                                            {
                                                GemVal gem_v__for_i_13 = gem_int(0);
    GemVal _t883 = gem_v_node;
    GemVal _t884[] = {gem_table_get(_t883, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t884, 1);
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
                                                    GemVal gem_v_i = gem_v__for_i_13;
                                                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
    GemVal _t885 = gem_v_node;
    GemVal _t886 = (*gem_v_collect_free_node);
    GemVal _t887[] = {gem_table_get(gem_table_get(_t885, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t886.fn(_t886.env, _t887, 3));
    GemVal _t888 = gem_v_node;
    GemVal _t889[] = {gem_table_get(gem_table_get(_t888, gem_string("stmts")), gem_v_i)};
    GemVal _t892;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t889, 1), gem_string("table")))) {
        _t892 = gem_eq(gem_type_fn(NULL, _t889, 1), gem_string("table"));
    } else {
        GemVal _t890 = gem_v_node;
        GemVal _t891 = gem_table_get(gem_table_get(_t890, gem_string("stmts")), gem_v_i);
        _t892 = gem_eq(gem_table_get(_t891, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t892)) {
    GemVal _t893 = gem_v_node;
    GemVal _t894 = gem_table_get(gem_table_get(_t893, gem_string("stmts")), gem_v_i);
    GemVal _t895[] = {gem_v_d, gem_table_get(_t894, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t895, 2));
                                                    }
                                                }
                                                return GEM_NIL;
                                            }
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t896 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t896, gem_string("value")), GEM_NIL))) {
    GemVal _t897 = gem_v_node;
    GemVal _t898 = (*gem_v_collect_free_node);
    GemVal _t899[] = {gem_table_get(_t897, gem_string("value")), gem_v_defined, gem_v_free};
                                                    return _t898.fn(_t898.env, _t899, 3);
                                                } else {
                                                    return GEM_NIL;
                                                }
                                            } else {
    GemVal _t900[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_fn_keys(NULL, _t900, 1);
                                                GemVal gem_v_nki = gem_int(0);
                                                while (1) {
                                                    GemVal _t901[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t901, 1)))) break;
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
    GemVal _t902[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t902, 1), gem_string("table")))) {
    GemVal _t903[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_fn_keys(NULL, _t903, 1);
    GemVal _t904[] = {gem_v_vks};
    GemVal _t906;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t904, 1), gem_int(0)))) {
        _t906 = gem_gt(gem_len_fn(NULL, _t904, 1), gem_int(0));
    } else {
        GemVal _t905[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t906 = gem_eq(gem_type_fn(NULL, _t905, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t906)) {
                                                                GemVal gem_v_vi = gem_int(0);
                                                                while (1) {
                                                                    GemVal _t907[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t907, 1)))) break;
    GemVal _t908[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t908, 1), gem_string("table")))) {
    GemVal _t909 = (*gem_v_collect_free_node);
    GemVal _t910[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t909.fn(_t909.env, _t910, 3));
                                                                    }
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
    GemVal _t911 = (*gem_v_collect_free_node);
    GemVal _t912[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t911.fn(_t911.env, _t912, 3));
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
    GemVal _t914 = gem_table_new();
    GemVal gem_v_free = _t914;
    GemVal _t915 = gem_table_new();
    GemVal _t916[] = {gem_v_defined, _t915};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t916, 2);
    {
        GemVal gem_v__for_i_15 = gem_int(0);
    GemVal _t917[] = {gem_v_stmts};
        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t917, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
            GemVal gem_v_i = gem_v__for_i_15;
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
    GemVal _t918 = (*gem_v_collect_free_node);
    GemVal _t919[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t918.fn(_t918.env, _t919, 3));
    GemVal _t920[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t922;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t920, 1), gem_string("table")))) {
        _t922 = gem_eq(gem_type_fn(NULL, _t920, 1), gem_string("table"));
    } else {
        GemVal _t921 = gem_table_get(gem_v_stmts, gem_v_i);
        _t922 = gem_eq(gem_table_get(_t921, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t922)) {
    GemVal _t923 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t924[] = {gem_v_d, gem_table_get(_t923, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t924, 2));
            }
    GemVal _t925[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t927;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t925, 1), gem_string("table")))) {
        _t927 = gem_eq(gem_type_fn(NULL, _t925, 1), gem_string("table"));
    } else {
        GemVal _t926 = gem_table_get(gem_v_stmts, gem_v_i);
        _t927 = gem_eq(gem_table_get(_t926, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t927)) {
                {
                    GemVal gem_v__for_i_14 = gem_int(0);
    GemVal _t928 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t929[] = {gem_table_get(_t928, gem_string("stmts"))};
                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t929, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
                        GemVal gem_v_j = gem_v__for_i_14;
                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
    GemVal _t930 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t931[] = {gem_table_get(gem_table_get(_t930, gem_string("stmts")), gem_v_j)};
    GemVal _t934;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t931, 1), gem_string("table")))) {
        _t934 = gem_eq(gem_type_fn(NULL, _t931, 1), gem_string("table"));
    } else {
        GemVal _t932 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t933 = gem_table_get(gem_table_get(_t932, gem_string("stmts")), gem_v_j);
        _t934 = gem_eq(gem_table_get(_t933, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t934)) {
    GemVal _t935 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t936 = gem_table_get(gem_table_get(_t935, gem_string("stmts")), gem_v_j);
    GemVal _t937[] = {gem_v_d, gem_table_get(_t936, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t937, 2));
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
    GemVal _t939[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t939, 1), gem_string("table")))) {
        return GEM_NIL;
    }
    GemVal _t940 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t940, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
        return GEM_NIL;
    }
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t941 = gem_v_node;
    GemVal _t942[] = {gem_table_get(_t941, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t942, 1);
    GemVal _t943 = gem_v_node;
    GemVal _t944 = (*gem_v_collect_free_vars);
    GemVal _t945[] = {gem_table_get(_t943, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t944.fn(_t944.env, _t945, 2);
    GemVal _t946[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t946, 2);
    GemVal _t947[] = {gem_v_inter};
        GemVal gem_v_iks = gem_fn_keys(NULL, _t947, 1);
        GemVal gem_v_iki = gem_int(0);
        while (1) {
            GemVal _t948[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t948, 1)))) break;
    GemVal _t949[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t949, 2));
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
    GemVal _t950 = gem_v_node;
    GemVal _t951 = gem_v_node;
    GemVal _t952[] = {gem_table_get(_t951, gem_string("params"))};
    GemVal _t953[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t952, 1)};
    GemVal _t954 = (*gem_v_walk_captures);
    GemVal _t955[] = {gem_table_get(_t950, gem_string("body")), gem_fn_set_union(NULL, _t953, 2), gem_v_captured};
        return _t954.fn(_t954.env, _t955, 3);
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            return GEM_NIL;
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t956 = gem_v_node;
    GemVal _t957 = (*gem_v_walk_captures_node);
    GemVal _t958[] = {gem_table_get(_t956, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t957.fn(_t957.env, _t958, 3));
    GemVal _t959 = (*gem_v_walk_captures);
    GemVal _t960[] = {gem_table_get(gem_v_node, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t959.fn(_t959.env, _t960, 3));
                if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t961 = (*gem_v_walk_captures);
    GemVal _t962[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    return _t961.fn(_t961.env, _t962, 3);
                } else {
                    return GEM_NIL;
                }
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t963 = gem_v_node;
    GemVal _t964 = (*gem_v_walk_captures_node);
    GemVal _t965[] = {gem_table_get(_t963, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t964.fn(_t964.env, _t965, 3));
    GemVal _t966 = gem_v_node;
    GemVal _t967 = (*gem_v_walk_captures);
    GemVal _t968[] = {gem_table_get(_t966, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    return _t967.fn(_t967.env, _t968, 3);
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t969 = gem_v_node;
    GemVal _t970 = (*gem_v_walk_captures_node);
    GemVal _t971[] = {gem_table_get(_t969, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t970.fn(_t970.env, _t971, 3));
                        {
                            GemVal gem_v__for_i_16 = gem_int(0);
    GemVal _t972 = gem_v_node;
    GemVal _t973[] = {gem_table_get(_t972, gem_string("whens"))};
                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t973, 1);
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
                                GemVal gem_v_i = gem_v__for_i_16;
                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
    GemVal _t974 = gem_v_node;
    GemVal _t975 = gem_table_get(gem_table_get(_t974, gem_string("whens")), gem_v_i);
    GemVal _t976 = (*gem_v_walk_captures_node);
    GemVal _t977[] = {gem_table_get(_t975, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t976.fn(_t976.env, _t977, 3));
    GemVal _t978 = gem_v_node;
    GemVal _t979 = gem_table_get(gem_table_get(_t978, gem_string("whens")), gem_v_i);
    GemVal _t980 = (*gem_v_walk_captures);
    GemVal _t981[] = {gem_table_get(_t979, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t980.fn(_t980.env, _t981, 3));
                            }
                        }
                        if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t982 = (*gem_v_walk_captures);
    GemVal _t983[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            return _t982.fn(_t982.env, _t983, 3);
                        } else {
                            return GEM_NIL;
                        }
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t984 = gem_v_node;
    GemVal _t985 = (*gem_v_walk_captures);
    GemVal _t986[] = {gem_table_get(_t984, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            return _t985.fn(_t985.env, _t986, 3);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t987 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t987, gem_string("value")), GEM_NIL))) {
    GemVal _t988 = gem_v_node;
    GemVal _t989 = (*gem_v_walk_captures_node);
    GemVal _t990[] = {gem_table_get(_t988, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t989.fn(_t989.env, _t990, 3);
                                } else {
                                    return GEM_NIL;
                                }
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t991 = gem_v_node;
    GemVal _t992 = (*gem_v_walk_captures_node);
    GemVal _t993[] = {gem_table_get(_t991, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t992.fn(_t992.env, _t993, 3);
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t994 = gem_v_node;
    GemVal _t995 = (*gem_v_walk_captures_node);
    GemVal _t996[] = {gem_table_get(_t994, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        return _t995.fn(_t995.env, _t996, 3);
                                    } else {
    GemVal _t997[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_fn_keys(NULL, _t997, 1);
                                        GemVal gem_v_nki = gem_int(0);
                                        while (1) {
                                            GemVal _t998[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t998, 1)))) break;
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
    GemVal _t999[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t999, 1), gem_string("table")))) {
    GemVal _t1000[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_fn_keys(NULL, _t1000, 1);
    GemVal _t1001[] = {gem_v_vks};
    GemVal _t1003;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1001, 1), gem_int(0)))) {
        _t1003 = gem_gt(gem_len_fn(NULL, _t1001, 1), gem_int(0));
    } else {
        GemVal _t1002[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1003 = gem_eq(gem_type_fn(NULL, _t1002, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t1003)) {
                                                        GemVal gem_v_vi = gem_int(0);
                                                        while (1) {
                                                            GemVal _t1004[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1004, 1)))) break;
    GemVal _t1005[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1005, 1), gem_string("table")))) {
    GemVal _t1006 = (*gem_v_walk_captures_node);
    GemVal _t1007[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t1006.fn(_t1006.env, _t1007, 3));
                                                            }
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
    GemVal _t1008 = (*gem_v_walk_captures_node);
    GemVal _t1009[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t1008.fn(_t1008.env, _t1009, 3));
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
    GemVal _t1011 = gem_table_new();
    GemVal _t1012[] = {gem_v_scope_vars, _t1011};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t1012, 2);
    {
        GemVal gem_v__for_i_18 = gem_int(0);
    GemVal _t1013[] = {gem_v_stmts};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1013, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
            GemVal gem_v_i = gem_v__for_i_18;
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
    GemVal _t1014 = (*gem_v_walk_captures_node);
    GemVal _t1015[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t1014.fn(_t1014.env, _t1015, 3));
    GemVal _t1016[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1018;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1016, 1), gem_string("table")))) {
        _t1018 = gem_eq(gem_type_fn(NULL, _t1016, 1), gem_string("table"));
    } else {
        GemVal _t1017 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1018 = gem_eq(gem_table_get(_t1017, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1018)) {
    GemVal _t1019 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1020[] = {gem_v_sv, gem_table_get(_t1019, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1020, 2));
            }
    GemVal _t1021[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t1023;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1021, 1), gem_string("table")))) {
        _t1023 = gem_eq(gem_type_fn(NULL, _t1021, 1), gem_string("table"));
    } else {
        GemVal _t1022 = gem_table_get(gem_v_stmts, gem_v_i);
        _t1023 = gem_eq(gem_table_get(_t1022, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1023)) {
                {
                    GemVal gem_v__for_i_17 = gem_int(0);
    GemVal _t1024 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1025[] = {gem_table_get(_t1024, gem_string("stmts"))};
                    GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1025, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
                        GemVal gem_v_j = gem_v__for_i_17;
                        gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
    GemVal _t1026 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1027[] = {gem_table_get(gem_table_get(_t1026, gem_string("stmts")), gem_v_j)};
    GemVal _t1030;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1027, 1), gem_string("table")))) {
        _t1030 = gem_eq(gem_type_fn(NULL, _t1027, 1), gem_string("table"));
    } else {
        GemVal _t1028 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t1029 = gem_table_get(gem_table_get(_t1028, gem_string("stmts")), gem_v_j);
        _t1030 = gem_eq(gem_table_get(_t1029, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1030)) {
    GemVal _t1031 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1032 = gem_table_get(gem_table_get(_t1031, gem_string("stmts")), gem_v_j);
    GemVal _t1033[] = {gem_v_sv, gem_table_get(_t1032, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1033, 2));
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
    GemVal _t1035 = gem_table_new();
    GemVal gem_v_captured = _t1035;
    GemVal _t1036 = (*gem_v_walk_captures);
    GemVal _t1037[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t1036.fn(_t1036.env, _t1037, 3));
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
    GemVal _t1039 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1039, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
    GemVal _t1040 = gem_table_new();
    GemVal _t1041 = gem_v_node;
    GemVal _t1042[] = {gem_table_get(_t1041, gem_string("value"))};
    gem_table_set(_t1040, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1042, 1)), gem_string(")")));
    gem_table_set(_t1040, gem_string("setup"), gem_string(""));
        return _t1040;
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
    GemVal _t1043 = gem_table_new();
    GemVal _t1044 = gem_v_node;
    GemVal _t1045[] = {gem_table_get(_t1044, gem_string("value"))};
    gem_table_set(_t1043, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_fn_format_float(NULL, _t1045, 1)), gem_string(")")));
    gem_table_set(_t1043, gem_string("setup"), gem_string(""));
            return _t1043;
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
    GemVal _t1046 = gem_v_node;
    GemVal _t1047[] = {gem_table_get(_t1046, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1047, 1);
    GemVal _t1048 = gem_table_new();
    gem_table_set(_t1048, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_v_escaped), gem_string("\")")));
    gem_table_set(_t1048, gem_string("setup"), gem_string(""));
                return _t1048;
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
    GemVal _t1049 = gem_v_node;
                    if (gem_truthy(gem_table_get(_t1049, gem_string("value")))) {
    GemVal _t1050 = gem_table_new();
    gem_table_set(_t1050, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1050, gem_string("setup"), gem_string(""));
                        return _t1050;
                    }
    GemVal _t1051 = gem_table_new();
    gem_table_set(_t1051, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1051, gem_string("setup"), gem_string(""));
                    return _t1051;
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
    GemVal _t1052 = gem_table_new();
    gem_table_set(_t1052, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1052, gem_string("setup"), gem_string(""));
                        return _t1052;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
    GemVal _t1053 = gem_v_node;
                            GemVal gem_v_name = gem_table_get(_t1053, gem_string("name"));
                            if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
    GemVal _t1054 = gem_table_new();
    gem_table_set(_t1054, gem_string("expr"), gem_string("gem_make_fn(gem_print, NULL)"));
    gem_table_set(_t1054, gem_string("setup"), gem_string(""));
                                return _t1054;
                            } else {
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
    GemVal _t1055 = gem_table_new();
    gem_table_set(_t1055, gem_string("expr"), gem_string("gem_make_fn(gem_error_fn, NULL)"));
    gem_table_set(_t1055, gem_string("setup"), gem_string(""));
                                    return _t1055;
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
    GemVal _t1056 = gem_table_new();
    gem_table_set(_t1056, gem_string("expr"), gem_string("gem_make_fn(gem_len_fn, NULL)"));
    gem_table_set(_t1056, gem_string("setup"), gem_string(""));
                                        return _t1056;
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
    GemVal _t1057 = gem_table_new();
    gem_table_set(_t1057, gem_string("expr"), gem_string("gem_make_fn(gem_type_fn, NULL)"));
    gem_table_set(_t1057, gem_string("setup"), gem_string(""));
                                            return _t1057;
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
    GemVal _t1058 = gem_table_new();
    gem_table_set(_t1058, gem_string("expr"), gem_string("gem_make_fn(gem_to_string_fn, NULL)"));
    gem_table_set(_t1058, gem_string("setup"), gem_string(""));
                                                return _t1058;
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_name, gem_string("push")))) {
    GemVal _t1059 = gem_table_new();
    gem_table_set(_t1059, gem_string("expr"), gem_string("gem_make_fn(gem_push_fn, NULL)"));
    gem_table_set(_t1059, gem_string("setup"), gem_string(""));
                                                    return _t1059;
                                                } else {
    GemVal _t1060[] = {(*gem_v_defined_fns), gem_v_name};
                                                    if (gem_truthy(gem_fn_set_contains(NULL, _t1060, 2))) {
    GemVal _t1061 = gem_table_new();
    gem_table_set(_t1061, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_v_name), gem_string(", NULL)")));
    gem_table_set(_t1061, gem_string("setup"), gem_string(""));
                                                        return _t1061;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
    GemVal _t1062 = (*gem_v_mangle);
    GemVal _t1063[] = {gem_v_name};
                            GemVal gem_v_mname = _t1062.fn(_t1062.env, _t1063, 1);
    GemVal _t1064[] = {(*gem_v_boxed_vars), gem_v_name};
                            if (gem_truthy(gem_fn_set_contains(NULL, _t1064, 2))) {
    GemVal _t1065 = gem_table_new();
    gem_table_set(_t1065, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_v_mname), gem_string(")")));
    gem_table_set(_t1065, gem_string("setup"), gem_string(""));
                                return _t1065;
                            }
    GemVal _t1066 = gem_table_new();
    gem_table_set(_t1066, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1066, gem_string("setup"), gem_string(""));
                            return _t1066;
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
    GemVal _t1067 = (*gem_v_compile_call);
    GemVal _t1068[] = {gem_v_node};
                                return _t1067.fn(_t1067.env, _t1068, 1);
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
    GemVal _t1069 = (*gem_v_compile_binop);
    GemVal _t1070[] = {gem_v_node};
                                    return _t1069.fn(_t1069.env, _t1070, 1);
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
    GemVal _t1071 = gem_v_node;
    GemVal _t1072 = (*gem_v_compile_expr);
    GemVal _t1073[] = {gem_table_get(_t1071, gem_string("expr"))};
                                        GemVal gem_v_r = _t1072.fn(_t1072.env, _t1073, 1);
    GemVal _t1074 = gem_v_node;
                                        if (gem_truthy(gem_eq(gem_table_get(_t1074, gem_string("op")), gem_string("-")))) {
    GemVal _t1075 = gem_table_new();
    GemVal _t1076 = gem_v_r;
    gem_table_set(_t1075, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_table_get(_t1076, gem_string("expr"))), gem_string(")")));
    GemVal _t1077 = gem_v_r;
    gem_table_set(_t1075, gem_string("setup"), gem_table_get(_t1077, gem_string("setup")));
                                            return _t1075;
                                        } else {
    GemVal _t1078 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1078, gem_string("op")), gem_string("not")))) {
    GemVal _t1079 = gem_table_new();
    GemVal _t1080 = gem_v_r;
    gem_table_set(_t1079, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_table_get(_t1080, gem_string("expr"))), gem_string(")")));
    GemVal _t1081 = gem_v_r;
    gem_table_set(_t1079, gem_string("setup"), gem_table_get(_t1081, gem_string("setup")));
                                                return _t1079;
                                            }
                                        }
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t1082 = (*gem_v_compile_anon_fn);
    GemVal _t1083[] = {gem_v_node};
                                            return _t1082.fn(_t1082.env, _t1083, 1);
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
    GemVal _t1084 = (*gem_v_compile_table);
    GemVal _t1085[] = {gem_v_node};
                                                return _t1084.fn(_t1084.env, _t1085, 1);
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
    GemVal _t1086 = (*gem_v_compile_array);
    GemVal _t1087[] = {gem_v_node};
                                                    return _t1086.fn(_t1086.env, _t1087, 1);
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
    GemVal _t1088 = gem_v_node;
    GemVal _t1089 = (*gem_v_compile_expr);
    GemVal _t1090[] = {gem_table_get(_t1088, gem_string("object"))};
                                                        GemVal gem_v_r = _t1089.fn(_t1089.env, _t1090, 1);
    GemVal _t1091 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1091.fn(_t1091.env, NULL, 0);
    GemVal _t1092 = gem_v_r;
    GemVal _t1093 = gem_v_r;
                                                        GemVal gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1092, gem_string("setup")), gem_string("    GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1093, gem_string("expr"))), gem_string(";\n"));
    GemVal _t1094 = gem_v_node;
    GemVal _t1095[] = {gem_table_get(_t1094, gem_string("field"))};
                                                        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1095, 1);
    GemVal _t1096 = gem_table_new();
    gem_table_set(_t1096, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_v_t), gem_string(", gem_string(\"")), gem_v_escaped), gem_string("\"))")));
    gem_table_set(_t1096, gem_string("setup"), gem_v_setup);
                                                        return _t1096;
                                                    } else {
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
    GemVal _t1097 = gem_v_node;
    GemVal _t1098 = (*gem_v_compile_expr);
    GemVal _t1099[] = {gem_table_get(_t1097, gem_string("object"))};
                                                            GemVal gem_v_obj_r = _t1098.fn(_t1098.env, _t1099, 1);
    GemVal _t1100 = gem_v_node;
    GemVal _t1101 = (*gem_v_compile_expr);
    GemVal _t1102[] = {gem_table_get(_t1100, gem_string("key"))};
                                                            GemVal gem_v_key_r = _t1101.fn(_t1101.env, _t1102, 1);
    GemVal _t1103 = gem_table_new();
    GemVal _t1104 = gem_v_obj_r;
    GemVal _t1105 = gem_v_key_r;
    gem_table_set(_t1103, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_table_get(_t1104, gem_string("expr"))), gem_string(", ")), gem_table_get(_t1105, gem_string("expr"))), gem_string(")")));
    GemVal _t1106 = gem_v_obj_r;
    GemVal _t1107 = gem_v_key_r;
    gem_table_set(_t1103, gem_string("setup"), gem_add(gem_table_get(_t1106, gem_string("setup")), gem_table_get(_t1107, gem_string("setup"))));
                                                            return _t1103;
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
    GemVal _t1108 = gem_table_new();
    gem_table_set(_t1108, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1108, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    return _t1108;
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
    GemVal _t1110 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1110, gem_string("entries"));
    GemVal _t1111 = (*gem_v_tmp);
    GemVal gem_v_t = _t1111.fn(_t1111.env, NULL, 0);
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_v_t), gem_string(" = gem_table_new();\n"));
    {
        GemVal gem_v__for_items_19 = gem_v_entries;
        GemVal gem_v__for_i_19 = gem_int(0);
        while (1) {
            GemVal _t1112[] = {gem_v__for_items_19};
            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t1112, 1)))) break;
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
    GemVal _t1113 = gem_v_entry;
    GemVal _t1114 = (*gem_v_compile_expr);
    GemVal _t1115[] = {gem_table_get(_t1113, gem_string("value"))};
            GemVal gem_v_val_r = _t1114.fn(_t1114.env, _t1115, 1);
    GemVal _t1116 = gem_v_entry;
    GemVal _t1117[] = {gem_table_get(_t1116, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1117, 1);
    GemVal _t1118 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1118, gem_string("setup")));
    GemVal _t1119 = gem_v_val_r;
            gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    gem_table_set(")), gem_v_t), gem_string(", gem_string(\"")), gem_v_escaped), gem_string("\"), ")), gem_table_get(_t1119, gem_string("expr"))), gem_string(");\n"));
        }
    }
    GemVal _t1120 = gem_table_new();
    gem_table_set(_t1120, gem_string("expr"), gem_v_t);
    gem_table_set(_t1120, gem_string("setup"), gem_v_setup);
    return _t1120;
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
    GemVal _t1122 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1122, gem_string("elements"));
    GemVal _t1123 = (*gem_v_tmp);
    GemVal gem_v_t = _t1123.fn(_t1123.env, NULL, 0);
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_v_t), gem_string(" = gem_table_new();\n"));
    {
        GemVal gem_v__for_i_20 = gem_int(0);
    GemVal _t1124[] = {gem_v_elements};
        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1124, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
            GemVal gem_v_i = gem_v__for_i_20;
            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
    GemVal _t1125 = (*gem_v_compile_expr);
    GemVal _t1126[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1125.fn(_t1125.env, _t1126, 1);
    GemVal _t1127 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1127, gem_string("setup")));
    GemVal _t1128[] = {gem_v_i};
    GemVal _t1129 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    gem_table_set(")), gem_v_t), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1128, 1)), gem_string("), ")), gem_table_get(_t1129, gem_string("expr"))), gem_string(");\n"));
        }
    }
    GemVal _t1130 = gem_table_new();
    gem_table_set(_t1130, gem_string("expr"), gem_v_t);
    gem_table_set(_t1130, gem_string("setup"), gem_v_setup);
    return _t1130;
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
    GemVal _t1132 = gem_v_node;
    GemVal _t1133[] = {gem_table_get(_t1132, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1133, 1);
    GemVal _t1134 = gem_v_node;
    GemVal _t1135 = (*gem_v_collect_free_vars);
    GemVal _t1136[] = {gem_table_get(_t1134, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1135.fn(_t1135.env, _t1136, 2);
    GemVal _t1137[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1137, 1);
    GemVal _t1138[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1138, 1), gem_int(0)))) {
    GemVal _t1139 = gem_table_new();
    GemVal _t1140 = (*gem_v_compile_closure_fn);
    GemVal _t1141[] = {gem_v_node, _t1139};
        GemVal gem_v_result = _t1140.fn(_t1140.env, _t1141, 2);
    GemVal _t1142 = gem_table_new();
    GemVal _t1143 = gem_v_result;
    gem_table_set(_t1142, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_table_get(_t1143, gem_string("fn_name"))), gem_string(", NULL)")));
    gem_table_set(_t1142, gem_string("setup"), gem_string(""));
        return _t1142;
    }
    GemVal _t1144 = (*gem_v_compile_closure_fn);
    GemVal _t1145[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1144.fn(_t1144.env, _t1145, 2);
    GemVal gem_v_setup = gem_string("");
    GemVal _t1146 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1146.fn(_t1146.env, NULL, 0);
    GemVal _t1147 = gem_v_result;
    GemVal _t1148 = gem_v_result;
    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    struct ")), gem_table_get(_t1147, gem_string("struct_name"))), gem_string(" *")), gem_v_env_tmp), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_table_get(_t1148, gem_string("struct_name"))), gem_string("));\n"));
    {
        GemVal gem_v__for_items_21 = gem_v_captures;
        GemVal gem_v__for_i_21 = gem_int(0);
        while (1) {
            GemVal _t1149[] = {gem_v__for_items_21};
            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t1149, 1)))) break;
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
    GemVal _t1150 = (*gem_v_mangle);
    GemVal _t1151[] = {gem_v_cap};
            GemVal gem_v_mc = _t1150.fn(_t1150.env, _t1151, 1);
    GemVal _t1152[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1152, 2))) {
                gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    ")), gem_v_env_tmp), gem_string("->")), gem_v_mc), gem_string(" = ")), gem_v_mc), gem_string(";\n"));
            } else {
                gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    ")), gem_v_env_tmp), gem_string("->")), gem_v_mc), gem_string(" = &")), gem_v_mc), gem_string(";\n"));
            }
        }
    }
    GemVal _t1153 = gem_table_new();
    GemVal _t1154 = gem_v_result;
    gem_table_set(_t1153, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_table_get(_t1154, gem_string("fn_name"))), gem_string(", ")), gem_v_env_tmp), gem_string(")")));
    gem_table_set(_t1153, gem_string("setup"), gem_v_setup);
    return _t1153;
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
    GemVal _t1156 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1156, gem_string("func"));
    GemVal _t1157 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1157, gem_string("args"));
    GemVal _t1158 = gem_v_func;
    GemVal _t1160;
    if (!gem_truthy(gem_eq(gem_table_get(_t1158, gem_string("tag")), gem_string("dot")))) {
        _t1160 = gem_eq(gem_table_get(_t1158, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1159 = gem_v_func;
        _t1160 = gem_eq(gem_table_get(_t1159, gem_string("field")), gem_string("len"));
    }
    GemVal _t1162;
    if (!gem_truthy(_t1160)) {
        _t1162 = _t1160;
    } else {
        GemVal _t1161[] = {gem_v_args};
        _t1162 = gem_eq(gem_len_fn(NULL, _t1161, 1), gem_int(0));
    }
    if (gem_truthy(_t1162)) {
    GemVal _t1163 = gem_v_func;
    GemVal _t1164 = (*gem_v_compile_expr);
    GemVal _t1165[] = {gem_table_get(_t1163, gem_string("object"))};
        GemVal gem_v_obj_r = _t1164.fn(_t1164.env, _t1165, 1);
    GemVal _t1166 = gem_table_new();
    GemVal _t1167 = gem_v_obj_r;
    gem_table_set(_t1166, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_table_get(_t1167, gem_string("expr"))), gem_string(")")));
    GemVal _t1168 = gem_v_obj_r;
    gem_table_set(_t1166, gem_string("setup"), gem_table_get(_t1168, gem_string("setup")));
        return _t1166;
    }
    GemVal _t1169 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1169, gem_string("tag")), gem_string("var")))) {
    GemVal _t1170 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1170, gem_string("name"));
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
    GemVal _t1171 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1171, gem_string("line"));
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
                gem_v_line = gem_int(0);
            }
    GemVal _t1172[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1172, 1);
            GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1173 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1173;
            {
                GemVal gem_v__for_i_22 = gem_int(0);
    GemVal _t1174[] = {gem_v_args};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1174, 1);
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
                    GemVal gem_v_i = gem_v__for_i_22;
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
    GemVal _t1175 = (*gem_v_compile_expr);
    GemVal _t1176[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1175.fn(_t1175.env, _t1176, 1);
    GemVal _t1177 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1177, gem_string("setup")));
    GemVal _t1178 = gem_v_r;
    GemVal _t1179[] = {gem_v_arg_exprs, gem_table_get(_t1178, gem_string("expr"))};
                    (void)(gem_push_fn(NULL, _t1179, 2));
                }
            }
    GemVal _t1180[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1180, 1);
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1181 = gem_table_new();
    GemVal _t1182[] = {gem_v_line};
    gem_table_set(_t1181, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_v_escaped_file), gem_string("\", ")), gem_to_string_fn(NULL, _t1182, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1181, gem_string("setup"), gem_v_arg_setups);
                return _t1181;
            }
    GemVal _t1183 = (*gem_v_tmp);
            GemVal gem_v_t = _t1183.fn(_t1183.env, NULL, 0);
            GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
            {
                GemVal gem_v__for_i_23 = gem_int(1);
                GemVal gem_v__for_limit_23 = gem_v_argc;
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
                    GemVal gem_v_i = gem_v__for_i_23;
                    gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
                    gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
                }
            }
            GemVal gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_arg_setups, gem_string("    GemVal ")), gem_v_t), gem_string("[] = {")), gem_v_arr), gem_string("};\n"));
    GemVal _t1184 = gem_table_new();
    GemVal _t1185[] = {gem_v_line};
    GemVal _t1186[] = {gem_v_argc};
    gem_table_set(_t1184, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_v_escaped_file), gem_string("\", ")), gem_to_string_fn(NULL, _t1185, 1)), gem_string(", ")), gem_v_t), gem_string(", ")), gem_to_string_fn(NULL, _t1186, 1)), gem_string(")")));
    gem_table_set(_t1184, gem_string("setup"), gem_v_setup);
            return _t1184;
        } else {
    GemVal _t1187;
    if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
        _t1187 = gem_eq(gem_v_name, gem_string("print"));
    } else {
        _t1187 = gem_eq(gem_v_name, gem_string("len"));
    }
    GemVal _t1188;
    if (gem_truthy(_t1187)) {
        _t1188 = _t1187;
    } else {
        _t1188 = gem_eq(gem_v_name, gem_string("type"));
    }
    GemVal _t1189;
    if (gem_truthy(_t1188)) {
        _t1189 = _t1188;
    } else {
        _t1189 = gem_eq(gem_v_name, gem_string("to_string"));
    }
    GemVal _t1190;
    if (gem_truthy(_t1189)) {
        _t1190 = _t1189;
    } else {
        _t1190 = gem_eq(gem_v_name, gem_string("push"));
    }
            if (gem_truthy(_t1190)) {
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
    GemVal _t1191 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1191;
                {
                    GemVal gem_v__for_i_24 = gem_int(0);
    GemVal _t1192[] = {gem_v_args};
                    GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1192, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
                        GemVal gem_v_i = gem_v__for_i_24;
                        gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
    GemVal _t1193 = (*gem_v_compile_expr);
    GemVal _t1194[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1193.fn(_t1193.env, _t1194, 1);
    GemVal _t1195 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1195, gem_string("setup")));
    GemVal _t1196 = gem_v_r;
    GemVal _t1197[] = {gem_v_arg_exprs, gem_table_get(_t1196, gem_string("expr"))};
                        (void)(gem_push_fn(NULL, _t1197, 2));
                    }
                }
    GemVal _t1198[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1198, 1);
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1199 = gem_table_new();
    gem_table_set(_t1199, gem_string("expr"), gem_add(gem_v_fn_name, gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1199, gem_string("setup"), gem_v_arg_setups);
                    return _t1199;
                }
    GemVal _t1200 = (*gem_v_tmp);
                GemVal gem_v_t = _t1200.fn(_t1200.env, NULL, 0);
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
                GemVal gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_arg_setups, gem_string("    GemVal ")), gem_v_t), gem_string("[] = {")), gem_v_arr), gem_string("};\n"));
    GemVal _t1201 = gem_table_new();
    GemVal _t1202[] = {gem_v_argc};
    gem_table_set(_t1201, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_fn_name, gem_string("(NULL, ")), gem_v_t), gem_string(", ")), gem_to_string_fn(NULL, _t1202, 1)), gem_string(")")));
    gem_table_set(_t1201, gem_string("setup"), gem_v_setup);
                return _t1201;
            } else {
    GemVal _t1203[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1203, 2))) {
                    GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1204 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1204;
                    {
                        GemVal gem_v__for_i_26 = gem_int(0);
    GemVal _t1205[] = {gem_v_args};
                        GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1205, 1);
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
                            GemVal gem_v_i = gem_v__for_i_26;
                            gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
    GemVal _t1206 = (*gem_v_compile_expr);
    GemVal _t1207[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1206.fn(_t1206.env, _t1207, 1);
    GemVal _t1208 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1208, gem_string("setup")));
    GemVal _t1209 = gem_v_r;
    GemVal _t1210[] = {gem_v_arg_exprs, gem_table_get(_t1209, gem_string("expr"))};
                            (void)(gem_push_fn(NULL, _t1210, 2));
                        }
                    }
    GemVal _t1211[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1211, 1);
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1212 = gem_table_new();
    gem_table_set(_t1212, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_v_name), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1212, gem_string("setup"), gem_v_arg_setups);
                        return _t1212;
                    }
    GemVal _t1213 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1213.fn(_t1213.env, NULL, 0);
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
                    GemVal gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_arg_setups, gem_string("    GemVal ")), gem_v_t), gem_string("[] = {")), gem_v_arr), gem_string("};\n"));
    GemVal _t1214 = gem_table_new();
    GemVal _t1215[] = {gem_v_argc};
    gem_table_set(_t1214, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_v_name), gem_string("(NULL, ")), gem_v_t), gem_string(", ")), gem_to_string_fn(NULL, _t1215, 1)), gem_string(")")));
    gem_table_set(_t1214, gem_string("setup"), gem_v_setup);
                    return _t1214;
                }
            }
        }
    }
    GemVal _t1216 = (*gem_v_compile_expr);
    GemVal _t1217[] = {gem_v_func};
    GemVal gem_v_func_r = _t1216.fn(_t1216.env, _t1217, 1);
    GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1218 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1218;
    {
        GemVal gem_v__for_i_28 = gem_int(0);
    GemVal _t1219[] = {gem_v_args};
        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1219, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
            GemVal gem_v_i = gem_v__for_i_28;
            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
    GemVal _t1220 = (*gem_v_compile_expr);
    GemVal _t1221[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1220.fn(_t1220.env, _t1221, 1);
    GemVal _t1222 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1222, gem_string("setup")));
    GemVal _t1223 = gem_v_r;
    GemVal _t1224[] = {gem_v_arg_exprs, gem_table_get(_t1223, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t1224, 2));
        }
    }
    GemVal _t1225 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1225, gem_string("setup")), gem_v_arg_setups);
    GemVal _t1226[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1226, 1);
    GemVal _t1227 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1227.fn(_t1227.env, NULL, 0);
    GemVal _t1228 = gem_v_func_r;
    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_tmp_fn), gem_string(" = ")), gem_table_get(_t1228, gem_string("expr"))), gem_string(";\n"));
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1229 = gem_table_new();
    gem_table_set(_t1229, gem_string("expr"), gem_add(gem_add(gem_add(gem_v_tmp_fn, gem_string(".fn(")), gem_v_tmp_fn), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1229, gem_string("setup"), gem_v_setup);
        return _t1229;
    }
    GemVal _t1230 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1230.fn(_t1230.env, NULL, 0);
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
    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_tmp_args), gem_string("[] = {")), gem_v_arr), gem_string("};\n"));
    GemVal _t1231 = gem_table_new();
    GemVal _t1232[] = {gem_v_argc};
    gem_table_set(_t1231, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_tmp_fn, gem_string(".fn(")), gem_v_tmp_fn), gem_string(".env, ")), gem_v_tmp_args), gem_string(", ")), gem_to_string_fn(NULL, _t1232, 1)), gem_string(")")));
    gem_table_set(_t1231, gem_string("setup"), gem_v_setup);
    return _t1231;
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
    GemVal _t1234 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1234, gem_string("op"));
    GemVal _t1235 = gem_v_node;
    GemVal _t1236 = (*gem_v_compile_expr);
    GemVal _t1237[] = {gem_table_get(_t1235, gem_string("left"))};
    GemVal gem_v_lc_r = _t1236.fn(_t1236.env, _t1237, 1);
    GemVal _t1238 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1238, gem_string("expr"));
    GemVal _t1239 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1239, gem_string("setup"));
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
    GemVal _t1240 = gem_v_node;
    GemVal _t1241 = (*gem_v_compile_expr);
    GemVal _t1242[] = {gem_table_get(_t1240, gem_string("right"))};
        GemVal gem_v_rc_r = _t1241.fn(_t1241.env, _t1242, 1);
    GemVal _t1243 = gem_table_new();
    GemVal _t1244 = gem_v_rc_r;
    gem_table_set(_t1243, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_v_lc), gem_string(", ")), gem_table_get(_t1244, gem_string("expr"))), gem_string(")")));
    GemVal _t1245 = gem_v_rc_r;
    gem_table_set(_t1243, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1245, gem_string("setup"))));
        return _t1243;
    } else {
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
    GemVal _t1246 = gem_v_node;
    GemVal _t1247 = (*gem_v_compile_expr);
    GemVal _t1248[] = {gem_table_get(_t1246, gem_string("right"))};
            GemVal gem_v_rc_r = _t1247.fn(_t1247.env, _t1248, 1);
    GemVal _t1249 = gem_table_new();
    GemVal _t1250 = gem_v_rc_r;
    gem_table_set(_t1249, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_v_lc), gem_string(", ")), gem_table_get(_t1250, gem_string("expr"))), gem_string(")")));
    GemVal _t1251 = gem_v_rc_r;
    gem_table_set(_t1249, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1251, gem_string("setup"))));
            return _t1249;
        } else {
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
    GemVal _t1252 = gem_v_node;
    GemVal _t1253 = (*gem_v_compile_expr);
    GemVal _t1254[] = {gem_table_get(_t1252, gem_string("right"))};
                GemVal gem_v_rc_r = _t1253.fn(_t1253.env, _t1254, 1);
    GemVal _t1255 = gem_table_new();
    GemVal _t1256 = gem_v_rc_r;
    gem_table_set(_t1255, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_v_lc), gem_string(", ")), gem_table_get(_t1256, gem_string("expr"))), gem_string(")")));
    GemVal _t1257 = gem_v_rc_r;
    gem_table_set(_t1255, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1257, gem_string("setup"))));
                return _t1255;
            } else {
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
    GemVal _t1258 = gem_v_node;
    GemVal _t1259 = (*gem_v_compile_expr);
    GemVal _t1260[] = {gem_table_get(_t1258, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1259.fn(_t1259.env, _t1260, 1);
    GemVal _t1261 = gem_table_new();
    GemVal _t1262 = gem_v_rc_r;
    gem_table_set(_t1261, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_v_lc), gem_string(", ")), gem_table_get(_t1262, gem_string("expr"))), gem_string(")")));
    GemVal _t1263 = gem_v_rc_r;
    gem_table_set(_t1261, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1263, gem_string("setup"))));
                    return _t1261;
                } else {
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
    GemVal _t1264 = gem_v_node;
    GemVal _t1265 = (*gem_v_compile_expr);
    GemVal _t1266[] = {gem_table_get(_t1264, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1265.fn(_t1265.env, _t1266, 1);
    GemVal _t1267 = gem_table_new();
    GemVal _t1268 = gem_v_rc_r;
    gem_table_set(_t1267, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_v_lc), gem_string(", ")), gem_table_get(_t1268, gem_string("expr"))), gem_string(")")));
    GemVal _t1269 = gem_v_rc_r;
    gem_table_set(_t1267, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1269, gem_string("setup"))));
                        return _t1267;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
    GemVal _t1270 = gem_v_node;
    GemVal _t1271 = (*gem_v_compile_expr);
    GemVal _t1272[] = {gem_table_get(_t1270, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1271.fn(_t1271.env, _t1272, 1);
    GemVal _t1273 = gem_table_new();
    GemVal _t1274 = gem_v_rc_r;
    gem_table_set(_t1273, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_v_lc), gem_string(", ")), gem_table_get(_t1274, gem_string("expr"))), gem_string(")")));
    GemVal _t1275 = gem_v_rc_r;
    gem_table_set(_t1273, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1275, gem_string("setup"))));
                            return _t1273;
                        } else {
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
    GemVal _t1276 = gem_v_node;
    GemVal _t1277 = (*gem_v_compile_expr);
    GemVal _t1278[] = {gem_table_get(_t1276, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1277.fn(_t1277.env, _t1278, 1);
    GemVal _t1279 = gem_table_new();
    GemVal _t1280 = gem_v_rc_r;
    gem_table_set(_t1279, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_v_lc), gem_string(", ")), gem_table_get(_t1280, gem_string("expr"))), gem_string(")")));
    GemVal _t1281 = gem_v_rc_r;
    gem_table_set(_t1279, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1281, gem_string("setup"))));
                                return _t1279;
                            } else {
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
    GemVal _t1282 = gem_v_node;
    GemVal _t1283 = (*gem_v_compile_expr);
    GemVal _t1284[] = {gem_table_get(_t1282, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1283.fn(_t1283.env, _t1284, 1);
    GemVal _t1285 = gem_table_new();
    GemVal _t1286 = gem_v_rc_r;
    gem_table_set(_t1285, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_v_lc), gem_string(", ")), gem_table_get(_t1286, gem_string("expr"))), gem_string(")")));
    GemVal _t1287 = gem_v_rc_r;
    gem_table_set(_t1285, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1287, gem_string("setup"))));
                                    return _t1285;
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
    GemVal _t1288 = gem_v_node;
    GemVal _t1289 = (*gem_v_compile_expr);
    GemVal _t1290[] = {gem_table_get(_t1288, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1289.fn(_t1289.env, _t1290, 1);
    GemVal _t1291 = gem_table_new();
    GemVal _t1292 = gem_v_rc_r;
    gem_table_set(_t1291, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_v_lc), gem_string(", ")), gem_table_get(_t1292, gem_string("expr"))), gem_string(")")));
    GemVal _t1293 = gem_v_rc_r;
    gem_table_set(_t1291, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1293, gem_string("setup"))));
                                        return _t1291;
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
    GemVal _t1294 = gem_v_node;
    GemVal _t1295 = (*gem_v_compile_expr);
    GemVal _t1296[] = {gem_table_get(_t1294, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1295.fn(_t1295.env, _t1296, 1);
    GemVal _t1297 = gem_table_new();
    GemVal _t1298 = gem_v_rc_r;
    gem_table_set(_t1297, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_v_lc), gem_string(", ")), gem_table_get(_t1298, gem_string("expr"))), gem_string(")")));
    GemVal _t1299 = gem_v_rc_r;
    gem_table_set(_t1297, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1299, gem_string("setup"))));
                                            return _t1297;
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
    GemVal _t1300 = gem_v_node;
    GemVal _t1301 = (*gem_v_compile_expr);
    GemVal _t1302[] = {gem_table_get(_t1300, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1301.fn(_t1301.env, _t1302, 1);
    GemVal _t1303 = gem_table_new();
    GemVal _t1304 = gem_v_rc_r;
    gem_table_set(_t1303, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_v_lc), gem_string(", ")), gem_table_get(_t1304, gem_string("expr"))), gem_string(")")));
    GemVal _t1305 = gem_v_rc_r;
    gem_table_set(_t1303, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1305, gem_string("setup"))));
                                                return _t1303;
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
    GemVal _t1306 = gem_v_node;
    GemVal _t1307 = (*gem_v_compile_expr);
    GemVal _t1308[] = {gem_table_get(_t1306, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1307.fn(_t1307.env, _t1308, 1);
    GemVal _t1309 = (*gem_v_tmp);
                                                    GemVal gem_v_t = _t1309.fn(_t1309.env, NULL, 0);
                                                    GemVal gem_v_setup = gem_v_ls;
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_t), gem_string(";\n"));
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    if (!gem_truthy(")), gem_v_lc), gem_string(")) {\n"));
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_v_lc), gem_string(";\n"));
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
    GemVal _t1310 = gem_v_rc_r;
                                                    if (gem_truthy(gem_neq(gem_table_get(_t1310, gem_string("setup")), gem_string("")))) {
    GemVal _t1311 = gem_v_rc_r;
    GemVal _t1312[] = {gem_table_get(_t1311, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1312, 3));
                                                    }
    GemVal _t1313 = gem_v_rc_r;
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1313, gem_string("expr"))), gem_string(";\n"));
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
    GemVal _t1314 = gem_table_new();
    gem_table_set(_t1314, gem_string("expr"), gem_v_t);
    gem_table_set(_t1314, gem_string("setup"), gem_v_setup);
                                                    return _t1314;
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
    GemVal _t1315 = gem_v_node;
    GemVal _t1316 = (*gem_v_compile_expr);
    GemVal _t1317[] = {gem_table_get(_t1315, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1316.fn(_t1316.env, _t1317, 1);
    GemVal _t1318 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1318.fn(_t1318.env, NULL, 0);
                                                        GemVal gem_v_setup = gem_v_ls;
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_t), gem_string(";\n"));
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    if (gem_truthy(")), gem_v_lc), gem_string(")) {\n"));
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_v_lc), gem_string(";\n"));
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
    GemVal _t1319 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1319, gem_string("setup")), gem_string("")))) {
    GemVal _t1320 = gem_v_rc_r;
    GemVal _t1321[] = {gem_table_get(_t1320, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1321, 3));
                                                        }
    GemVal _t1322 = gem_v_rc_r;
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1322, gem_string("expr"))), gem_string(";\n"));
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
    GemVal _t1323 = gem_table_new();
    gem_table_set(_t1323, gem_string("expr"), gem_v_t);
    gem_table_set(_t1323, gem_string("setup"), gem_v_setup);
                                                        return _t1323;
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
    GemVal _t1324 = gem_v_node;
    GemVal _t1325 = (*gem_v_compile_expr);
    GemVal _t1326[] = {gem_table_get(_t1324, gem_string("right"))};
    GemVal gem_v_rc_r = _t1325.fn(_t1325.env, _t1326, 1);
    GemVal _t1327 = gem_table_new();
    gem_table_set(_t1327, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1328 = gem_v_rc_r;
    gem_table_set(_t1327, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1328, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    return _t1327;
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
        GemVal gem_v__for_items_30 = gem_v_stmts;
        GemVal gem_v__for_i_30 = gem_int(0);
        while (1) {
            GemVal _t1330[] = {gem_v__for_items_30};
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_len_fn(NULL, _t1330, 1)))) break;
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_30, gem_v__for_i_30);
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
    GemVal _t1331 = (*gem_v_compile_stmt);
    GemVal _t1332[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1331.fn(_t1331.env, _t1332, 2)), gem_string("\n"));
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
    GemVal _t1334[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1334, 1);
    GemVal _t1335 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1335, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1336 = gem_v_node;
    GemVal _t1337 = (*gem_v_compile_expr);
    GemVal _t1338[] = {gem_table_get(_t1336, gem_string("value"))};
        GemVal gem_v_r = _t1337.fn(_t1337.env, _t1338, 1);
    GemVal _t1339 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1339, gem_string("name"));
    GemVal _t1340 = (*gem_v_mangle);
    GemVal _t1341[] = {gem_v_name};
        GemVal gem_v_mname = _t1340.fn(_t1340.env, _t1341, 1);
    GemVal _t1342[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1342, 2))) {
    GemVal _t1343 = gem_v_r;
    GemVal _t1344 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1343, gem_string("setup")), gem_v_p), gem_string("GemVal *")), gem_v_mname), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1344, gem_string("expr"))), gem_string(";"));
        }
    GemVal _t1345 = gem_v_r;
    GemVal _t1346 = gem_v_r;
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1345, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1346, gem_string("expr"))), gem_string(";"));
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1347 = gem_v_node;
    GemVal _t1348 = (*gem_v_compile_expr);
    GemVal _t1349[] = {gem_table_get(_t1347, gem_string("value"))};
            GemVal gem_v_r = _t1348.fn(_t1348.env, _t1349, 1);
    GemVal _t1350 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1350, gem_string("name"));
    GemVal _t1351 = (*gem_v_mangle);
    GemVal _t1352[] = {gem_v_name};
            GemVal gem_v_mname = _t1351.fn(_t1351.env, _t1352, 1);
    GemVal _t1353[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1353, 2))) {
    GemVal _t1354 = gem_v_r;
    GemVal _t1355 = gem_v_r;
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1354, gem_string("setup")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1355, gem_string("expr"))), gem_string(";"));
            }
    GemVal _t1356 = gem_v_r;
    GemVal _t1357 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1356, gem_string("setup")), gem_v_p), gem_v_mname), gem_string(" = ")), gem_table_get(_t1357, gem_string("expr"))), gem_string(";"));
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
    GemVal _t1358 = gem_v_node;
    GemVal _t1359 = (*gem_v_compile_expr);
    GemVal _t1360[] = {gem_table_get(_t1358, gem_string("object"))};
                GemVal gem_v_obj_r = _t1359.fn(_t1359.env, _t1360, 1);
    GemVal _t1361 = gem_v_node;
    GemVal _t1362 = (*gem_v_compile_expr);
    GemVal _t1363[] = {gem_table_get(_t1361, gem_string("value"))};
                GemVal gem_v_val_r = _t1362.fn(_t1362.env, _t1363, 1);
    GemVal _t1364 = gem_v_node;
    GemVal _t1365[] = {gem_table_get(_t1364, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1365, 1);
    GemVal _t1366 = gem_v_obj_r;
    GemVal _t1367 = gem_v_val_r;
    GemVal _t1368 = gem_v_obj_r;
    GemVal _t1369 = gem_v_val_r;
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1366, gem_string("setup")), gem_table_get(_t1367, gem_string("setup"))), gem_v_p), gem_string("gem_table_set(")), gem_table_get(_t1368, gem_string("expr"))), gem_string(", gem_string(\"")), gem_v_escaped), gem_string("\"), ")), gem_table_get(_t1369, gem_string("expr"))), gem_string(");"));
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
    GemVal _t1370 = gem_v_node;
    GemVal _t1371 = (*gem_v_compile_expr);
    GemVal _t1372[] = {gem_table_get(_t1370, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1371.fn(_t1371.env, _t1372, 1);
    GemVal _t1373 = gem_v_node;
    GemVal _t1374 = (*gem_v_compile_expr);
    GemVal _t1375[] = {gem_table_get(_t1373, gem_string("key"))};
                    GemVal gem_v_key_r = _t1374.fn(_t1374.env, _t1375, 1);
    GemVal _t1376 = gem_v_node;
    GemVal _t1377 = (*gem_v_compile_expr);
    GemVal _t1378[] = {gem_table_get(_t1376, gem_string("value"))};
                    GemVal gem_v_val_r = _t1377.fn(_t1377.env, _t1378, 1);
    GemVal _t1379 = gem_v_obj_r;
    GemVal _t1380 = gem_v_key_r;
    GemVal _t1381 = gem_v_val_r;
    GemVal _t1382 = gem_v_obj_r;
    GemVal _t1383 = gem_v_key_r;
    GemVal _t1384 = gem_v_val_r;
                    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1379, gem_string("setup")), gem_table_get(_t1380, gem_string("setup"))), gem_table_get(_t1381, gem_string("setup"))), gem_v_p), gem_string("gem_table_set(")), gem_table_get(_t1382, gem_string("expr"))), gem_string(", ")), gem_table_get(_t1383, gem_string("expr"))), gem_string(", ")), gem_table_get(_t1384, gem_string("expr"))), gem_string(");"));
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1385 = (*gem_v_compile_if);
    GemVal _t1386[] = {gem_v_node, gem_v_indent};
                        return _t1385.fn(_t1385.env, _t1386, 2);
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1387 = (*gem_v_compile_while);
    GemVal _t1388[] = {gem_v_node, gem_v_indent};
                            return _t1387.fn(_t1387.env, _t1388, 2);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1389 = (*gem_v_compile_match);
    GemVal _t1390[] = {gem_v_node, gem_v_indent};
                                return _t1389.fn(_t1389.env, _t1390, 2);
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
                                    return gem_add(gem_v_p, gem_string("break;"));
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
                                        return gem_add(gem_v_p, gem_string("continue;"));
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1391 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1391, gem_string("value")), GEM_NIL))) {
    GemVal _t1392 = gem_v_node;
    GemVal _t1393 = (*gem_v_compile_expr);
    GemVal _t1394[] = {gem_table_get(_t1392, gem_string("value"))};
                                                GemVal gem_v_r = _t1393.fn(_t1393.env, _t1394, 1);
    GemVal _t1395 = gem_v_r;
    GemVal _t1396 = gem_v_r;
                                                return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1395, gem_string("setup")), gem_v_p), gem_string("return ")), gem_table_get(_t1396, gem_string("expr"))), gem_string(";"));
                                            }
                                            return gem_add(gem_v_p, gem_string("return GEM_NIL;"));
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
    GemVal _t1397 = (*gem_v_compile_expr);
    GemVal _t1398[] = {gem_v_node};
                                                GemVal gem_v_r = _t1397.fn(_t1397.env, _t1398, 1);
    GemVal _t1399 = gem_v_r;
    GemVal _t1400 = gem_v_r;
                                                return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1399, gem_string("setup")), gem_v_p), gem_string("(void)(")), gem_table_get(_t1400, gem_string("expr"))), gem_string(");"));
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                                                    return gem_string("");
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
                                                        GemVal gem_v_out = gem_add(gem_v_p, gem_string("{\n"));
                                                        {
    GemVal _t1401 = gem_v_node;
                                                            GemVal gem_v__for_items_31 = gem_table_get(_t1401, gem_string("stmts"));
                                                            GemVal gem_v__for_i_31 = gem_int(0);
                                                            while (1) {
                                                                GemVal _t1402[] = {gem_v__for_items_31};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_len_fn(NULL, _t1402, 1)))) break;
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_31, gem_v__for_i_31);
                                                                gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
    GemVal _t1403 = (*gem_v_compile_stmt);
    GemVal _t1404[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1403.fn(_t1403.env, _t1404, 2)), gem_string("\n"));
                                                            }
                                                        }
                                                        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
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
    GemVal _t1405 = (*gem_v_compile_expr);
    GemVal _t1406[] = {gem_v_node};
    GemVal gem_v_r = _t1405.fn(_t1405.env, _t1406, 1);
    GemVal _t1407 = gem_v_r;
    GemVal _t1408 = gem_v_r;
    return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1407, gem_string("setup")), gem_v_p), gem_string("(void)(")), gem_table_get(_t1408, gem_string("expr"))), gem_string(");"));
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
    GemVal _t1410[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1410, 1);
    GemVal _t1411 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1411, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1412 = gem_v_node;
    GemVal _t1413 = (*gem_v_compile_expr);
    GemVal _t1414[] = {gem_table_get(_t1412, gem_string("value"))};
        GemVal gem_v_r = _t1413.fn(_t1413.env, _t1414, 1);
    GemVal _t1415 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1415, gem_string("name"));
    GemVal _t1416 = (*gem_v_mangle);
    GemVal _t1417[] = {gem_v_name};
        GemVal gem_v_mname = _t1416.fn(_t1416.env, _t1417, 1);
    GemVal _t1418[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1418, 2))) {
    GemVal _t1419 = gem_v_r;
    GemVal _t1420 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1419, gem_string("setup")), gem_v_p), gem_string("GemVal *")), gem_v_mname), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1420, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_string("return GEM_NIL;"));
        }
    GemVal _t1421 = gem_v_r;
    GemVal _t1422 = gem_v_r;
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1421, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1422, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_string("return GEM_NIL;"));
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1423 = gem_v_node;
    GemVal _t1424 = (*gem_v_compile_expr);
    GemVal _t1425[] = {gem_table_get(_t1423, gem_string("value"))};
            GemVal gem_v_r = _t1424.fn(_t1424.env, _t1425, 1);
    GemVal _t1426 = (*gem_v_tmp);
            GemVal gem_v_t = _t1426.fn(_t1426.env, NULL, 0);
    GemVal _t1427 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1427, gem_string("name"));
    GemVal _t1428 = (*gem_v_mangle);
    GemVal _t1429[] = {gem_v_name};
            GemVal gem_v_mname = _t1428.fn(_t1428.env, _t1429, 1);
    GemVal _t1430[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1430, 2))) {
    GemVal _t1431 = gem_v_r;
    GemVal _t1432 = gem_v_r;
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1431, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1432, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_v_t), gem_string(";\n")), gem_v_p), gem_string("return ")), gem_v_t), gem_string(";"));
            }
    GemVal _t1433 = gem_v_r;
    GemVal _t1434 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1433, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1434, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_v_mname), gem_string(" = ")), gem_v_t), gem_string(";\n")), gem_v_p), gem_string("return ")), gem_v_t), gem_string(";"));
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1435 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1435, gem_string("value")), GEM_NIL))) {
    GemVal _t1436 = gem_v_node;
    GemVal _t1437 = (*gem_v_compile_expr);
    GemVal _t1438[] = {gem_table_get(_t1436, gem_string("value"))};
                    GemVal gem_v_r = _t1437.fn(_t1437.env, _t1438, 1);
    GemVal _t1439 = gem_v_r;
    GemVal _t1440 = gem_v_r;
                    return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1439, gem_string("setup")), gem_v_p), gem_string("return ")), gem_table_get(_t1440, gem_string("expr"))), gem_string(";"));
                }
                return gem_add(gem_v_p, gem_string("return GEM_NIL;"));
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1441 = (*gem_v_compile_if_return);
    GemVal _t1442[] = {gem_v_node, gem_v_indent};
                    return _t1441.fn(_t1441.env, _t1442, 2);
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1443 = (*gem_v_compile_while);
    GemVal _t1444[] = {gem_v_node, gem_v_indent};
                        return gem_add(gem_add(gem_add(_t1443.fn(_t1443.env, _t1444, 2), gem_string("\n")), gem_v_p), gem_string("return GEM_NIL;"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1445 = (*gem_v_compile_match_return);
    GemVal _t1446[] = {gem_v_node, gem_v_indent};
                            return _t1445.fn(_t1445.env, _t1446, 2);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
                                GemVal gem_v_out = gem_add(gem_v_p, gem_string("{\n"));
                                {
                                    GemVal gem_v__for_i_32 = gem_int(0);
    GemVal _t1447 = gem_v_node;
    GemVal _t1448[] = {gem_table_get(_t1447, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_32 = gem_sub(gem_len_fn(NULL, _t1448, 1), gem_int(1));
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
                                        GemVal gem_v_i = gem_v__for_i_32;
                                        gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
    GemVal _t1449 = gem_v_node;
    GemVal _t1450 = (*gem_v_compile_stmt);
    GemVal _t1451[] = {gem_table_get(gem_table_get(_t1449, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t1450.fn(_t1450.env, _t1451, 2)), gem_string("\n"));
                                    }
                                }
    GemVal _t1452 = gem_v_node;
    GemVal _t1453[] = {gem_table_get(_t1452, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1453, 1), gem_int(0)))) {
    GemVal _t1454 = gem_v_node;
    GemVal _t1455 = gem_v_node;
    GemVal _t1456[] = {gem_table_get(_t1455, gem_string("stmts"))};
    GemVal _t1457 = (*gem_v_compile_stmt_return);
    GemVal _t1458[] = {gem_table_get(gem_table_get(_t1454, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t1456, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t1457.fn(_t1457.env, _t1458, 2)), gem_string("\n"));
                                }
                                gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
                                return gem_v_out;
                            } else {
    GemVal _t1459;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t1459 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t1459 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t1460;
    if (gem_truthy(_t1459)) {
        _t1460 = _t1459;
    } else {
        _t1460 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t1461;
    if (gem_truthy(_t1460)) {
        _t1461 = _t1460;
    } else {
        _t1461 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t1462;
    if (gem_truthy(_t1461)) {
        _t1462 = _t1461;
    } else {
        _t1462 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t1462)) {
    GemVal _t1463 = (*gem_v_compile_stmt);
    GemVal _t1464[] = {gem_v_node, gem_v_indent};
                                    return gem_add(gem_add(gem_add(_t1463.fn(_t1463.env, _t1464, 2), gem_string("\n")), gem_v_p), gem_string("return GEM_NIL;"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t1465 = (*gem_v_compile_expr);
    GemVal _t1466[] = {gem_v_node};
    GemVal gem_v_r = _t1465.fn(_t1465.env, _t1466, 1);
    GemVal _t1467 = gem_v_r;
    GemVal _t1468 = gem_v_r;
    return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1467, gem_string("setup")), gem_v_p), gem_string("return ")), gem_table_get(_t1468, gem_string("expr"))), gem_string(";"));
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
    GemVal _t1470[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1470, 1);
    GemVal _t1471 = gem_v_node;
    GemVal _t1472 = (*gem_v_compile_expr);
    GemVal _t1473[] = {gem_table_get(_t1471, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1472.fn(_t1472.env, _t1473, 1);
    GemVal _t1474 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1474, gem_string("setup"));
    GemVal _t1475 = gem_v_cond_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("if (gem_truthy(")), gem_table_get(_t1475, gem_string("expr"))), gem_string(")) {\n"));
    GemVal _t1476 = (*gem_v_compile_stmts);
    GemVal _t1477[] = {gem_table_get(gem_v_node, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1476.fn(_t1476.env, _t1477, 2));
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
    GemVal _t1478 = (*gem_v_compile_stmts);
    GemVal _t1479[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1478.fn(_t1478.env, _t1479, 2));
    }
    gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
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
    GemVal _t1481[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1481, 1);
    GemVal _t1482 = gem_v_node;
    GemVal _t1483 = (*gem_v_compile_expr);
    GemVal _t1484[] = {gem_table_get(_t1482, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1483.fn(_t1483.env, _t1484, 1);
    GemVal _t1485 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1485, gem_string("setup"));
    GemVal _t1486 = gem_v_cond_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("if (gem_truthy(")), gem_table_get(_t1486, gem_string("expr"))), gem_string(")) {\n"));
    GemVal gem_v_then_body = gem_table_get(gem_v_node, gem_string("then"));
    GemVal _t1487[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1487, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_33 = gem_int(0);
    GemVal _t1488[] = {gem_v_then_body};
            GemVal gem_v__for_limit_33 = gem_sub(gem_len_fn(NULL, _t1488, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
                GemVal gem_v_i = gem_v__for_i_33;
                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
    GemVal _t1489 = (*gem_v_compile_stmt);
    GemVal _t1490[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1489.fn(_t1489.env, _t1490, 2)), gem_string("\n"));
            }
        }
    GemVal _t1491[] = {gem_v_then_body};
    GemVal _t1492 = (*gem_v_compile_stmt_return);
    GemVal _t1493[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t1491, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t1492.fn(_t1492.env, _t1493, 2)), gem_string("\n"));
    } else {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
    GemVal _t1494[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1494, 1), gem_int(0)))) {
            {
                GemVal gem_v__for_i_34 = gem_int(0);
    GemVal _t1495[] = {gem_v_else_body};
                GemVal gem_v__for_limit_34 = gem_sub(gem_len_fn(NULL, _t1495, 1), gem_int(1));
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
                    GemVal gem_v_i = gem_v__for_i_34;
                    gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
    GemVal _t1496 = (*gem_v_compile_stmt);
    GemVal _t1497[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1496.fn(_t1496.env, _t1497, 2)), gem_string("\n"));
                }
            }
    GemVal _t1498[] = {gem_v_else_body};
    GemVal _t1499 = (*gem_v_compile_stmt_return);
    GemVal _t1500[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1498, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1499.fn(_t1499.env, _t1500, 2)), gem_string("\n"));
        } else {
            gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
        }
    } else {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
    }
    gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
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
    GemVal _t1502[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1502, 1);
    GemVal _t1503 = gem_v_node;
    GemVal _t1504 = (*gem_v_compile_expr);
    GemVal _t1505[] = {gem_table_get(_t1503, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1504.fn(_t1504.env, _t1505, 1);
    GemVal gem_v_out = gem_add(gem_v_p, gem_string("while (1) {\n"));
    GemVal _t1506 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t1506, gem_string("setup")), gem_string("")))) {
    GemVal _t1507 = gem_v_cond_r;
    GemVal _t1508[] = {gem_table_get(_t1507, gem_string("setup")), gem_string("    "), gem_add(gem_v_p, gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_fn_str_replace_all(NULL, _t1508, 3));
    }
    GemVal _t1509 = gem_v_cond_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    if (!gem_truthy(")), gem_table_get(_t1509, gem_string("expr"))), gem_string(")) break;\n"));
    GemVal _t1510 = gem_v_node;
    GemVal _t1511 = (*gem_v_compile_stmts);
    GemVal _t1512[] = {gem_table_get(_t1510, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1511.fn(_t1511.env, _t1512, 2));
    gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
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
    GemVal _t1514[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1514, 1);
    GemVal _t1515 = gem_v_node;
    GemVal _t1516 = (*gem_v_compile_expr);
    GemVal _t1517[] = {gem_table_get(_t1515, gem_string("target"))};
    GemVal gem_v_target_r = _t1516.fn(_t1516.env, _t1517, 1);
    GemVal _t1518 = (*gem_v_tmp);
    GemVal gem_v_t = _t1518.fn(_t1518.env, NULL, 0);
    GemVal _t1519 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1519, gem_string("setup"));
    GemVal _t1520 = gem_v_target_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1520, gem_string("expr"))), gem_string(";\n"));
    {
        GemVal gem_v__for_i_35 = gem_int(0);
    GemVal _t1521 = gem_v_node;
    GemVal _t1522[] = {gem_table_get(_t1521, gem_string("whens"))};
        GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t1522, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
            GemVal gem_v_i = gem_v__for_i_35;
            gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
    GemVal _t1523 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1523, gem_string("whens")), gem_v_i);
    GemVal _t1524 = gem_v_w;
    GemVal _t1525 = (*gem_v_compile_expr);
    GemVal _t1526[] = {gem_table_get(_t1524, gem_string("value"))};
            GemVal gem_v_val_r = _t1525.fn(_t1525.env, _t1526, 1);
            GemVal gem_v_keyword = gem_string("if");
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_keyword = gem_string("} else if");
            }
    GemVal _t1527 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1527, gem_string("setup")));
    GemVal _t1528 = gem_v_val_r;
            gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_v_keyword), gem_string(" (gem_truthy(gem_eq(")), gem_v_t), gem_string(", ")), gem_table_get(_t1528, gem_string("expr"))), gem_string("))) {\n"));
    GemVal _t1529 = gem_v_w;
    GemVal _t1530 = (*gem_v_compile_stmts);
    GemVal _t1531[] = {gem_table_get(_t1529, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t1530.fn(_t1530.env, _t1531, 2));
        }
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
    GemVal _t1532 = (*gem_v_compile_stmts);
    GemVal _t1533[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1532.fn(_t1532.env, _t1533, 2));
    }
    gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
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
    GemVal _t1535[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1535, 1);
    GemVal _t1536 = gem_v_node;
    GemVal _t1537 = (*gem_v_compile_expr);
    GemVal _t1538[] = {gem_table_get(_t1536, gem_string("target"))};
    GemVal gem_v_target_r = _t1537.fn(_t1537.env, _t1538, 1);
    GemVal _t1539 = (*gem_v_tmp);
    GemVal gem_v_t = _t1539.fn(_t1539.env, NULL, 0);
    GemVal _t1540 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1540, gem_string("setup"));
    GemVal _t1541 = gem_v_target_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1541, gem_string("expr"))), gem_string(";\n"));
    {
        GemVal gem_v__for_i_37 = gem_int(0);
    GemVal _t1542 = gem_v_node;
    GemVal _t1543[] = {gem_table_get(_t1542, gem_string("whens"))};
        GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t1543, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
            GemVal gem_v_i = gem_v__for_i_37;
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
    GemVal _t1544 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1544, gem_string("whens")), gem_v_i);
    GemVal _t1545 = gem_v_w;
    GemVal _t1546 = (*gem_v_compile_expr);
    GemVal _t1547[] = {gem_table_get(_t1545, gem_string("value"))};
            GemVal gem_v_val_r = _t1546.fn(_t1546.env, _t1547, 1);
            GemVal gem_v_keyword = gem_string("if");
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_keyword = gem_string("} else if");
            }
    GemVal _t1548 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1548, gem_string("setup")));
    GemVal _t1549 = gem_v_val_r;
            gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_v_keyword), gem_string(" (gem_truthy(gem_eq(")), gem_v_t), gem_string(", ")), gem_table_get(_t1549, gem_string("expr"))), gem_string("))) {\n"));
    GemVal _t1550 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t1550, gem_string("body"));
    GemVal _t1551[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1551, 1), gem_int(0)))) {
                {
                    GemVal gem_v__for_i_36 = gem_int(0);
    GemVal _t1552[] = {gem_v_body};
                    GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t1552, 1), gem_int(1));
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
                        GemVal gem_v_j = gem_v__for_i_36;
                        gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
    GemVal _t1553 = (*gem_v_compile_stmt);
    GemVal _t1554[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t1553.fn(_t1553.env, _t1554, 2)), gem_string("\n"));
                    }
                }
    GemVal _t1555[] = {gem_v_body};
    GemVal _t1556 = (*gem_v_compile_stmt_return);
    GemVal _t1557[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1555, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1556.fn(_t1556.env, _t1557, 2)), gem_string("\n"));
            } else {
                gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
            }
        }
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
    GemVal _t1558[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1558, 1), gem_int(0)))) {
            {
                GemVal gem_v__for_i_38 = gem_int(0);
    GemVal _t1559[] = {gem_v_else_body};
                GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t1559, 1), gem_int(1));
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
                    GemVal gem_v_i = gem_v__for_i_38;
                    gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
    GemVal _t1560 = (*gem_v_compile_stmt);
    GemVal _t1561[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1560.fn(_t1560.env, _t1561, 2)), gem_string("\n"));
                }
            }
    GemVal _t1562[] = {gem_v_else_body};
    GemVal _t1563 = (*gem_v_compile_stmt_return);
    GemVal _t1564[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1562, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1563.fn(_t1563.env, _t1564, 2)), gem_string("\n"));
        } else {
            gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
        }
    } else {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
    }
    gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
    return gem_v_out;
}

struct _closure__anon_44 {
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1566 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1566, gem_string("name"));
    GemVal _t1567 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1567, gem_string("params"));
    GemVal _t1568 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t1568, gem_string("ret_type"));
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_v_name), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    GemVal _t1569 = gem_table_new();
    GemVal gem_v_c_args = _t1569;
    {
        GemVal gem_v__for_i_39 = gem_int(0);
    GemVal _t1570[] = {gem_v_params};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t1570, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
            GemVal gem_v_i = gem_v__for_i_39;
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
    GemVal _t1571[] = {gem_v_i};
    GemVal _t1572[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    int64_t _p")), gem_to_string_fn(NULL, _t1571, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1572, 1)), gem_string("].ival;\n"));
    GemVal _t1573[] = {gem_v_i};
    GemVal _t1574[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1573, 1))};
                (void)(gem_push_fn(NULL, _t1574, 2));
            } else {
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
    GemVal _t1575[] = {gem_v_i};
    GemVal _t1576[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    double _p")), gem_to_string_fn(NULL, _t1575, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1576, 1)), gem_string("].fval;\n"));
    GemVal _t1577[] = {gem_v_i};
    GemVal _t1578[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1577, 1))};
                    (void)(gem_push_fn(NULL, _t1578, 2));
                } else {
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
    GemVal _t1579[] = {gem_v_i};
    GemVal _t1580[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    char* _p")), gem_to_string_fn(NULL, _t1579, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1580, 1)), gem_string("].sval;\n"));
    GemVal _t1581[] = {gem_v_i};
    GemVal _t1582[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1581, 1))};
                        (void)(gem_push_fn(NULL, _t1582, 2));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
    GemVal _t1583[] = {gem_v_i};
    GemVal _t1584[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    int _p")), gem_to_string_fn(NULL, _t1583, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1584, 1)), gem_string("].bval;\n"));
    GemVal _t1585[] = {gem_v_i};
    GemVal _t1586[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1585, 1))};
                            (void)(gem_push_fn(NULL, _t1586, 2));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
    GemVal _t1587[] = {gem_v_i};
    GemVal _t1588[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    void* _p")), gem_to_string_fn(NULL, _t1587, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t1588, 1)), gem_string("].ival;\n"));
    GemVal _t1589[] = {gem_v_i};
    GemVal _t1590[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1589, 1))};
                                (void)(gem_push_fn(NULL, _t1590, 2));
                            } else {
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
    GemVal _t1591[] = {gem_v_i};
    GemVal _t1592[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1591, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t1592, 2));
                                } else {
    GemVal _t1593[] = {gem_v_i};
    GemVal _t1594[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1593, 1)), gem_string("]"))};
                                    (void)(gem_push_fn(NULL, _t1594, 2));
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
        GemVal gem_v__for_i_40 = gem_int(0);
    GemVal _t1595[] = {gem_v_c_args};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t1595, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
            GemVal gem_v_i = gem_v__for_i_40;
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
            }
            gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
        }
    }
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_v_name, gem_string("(")), gem_v_call_args), gem_string(")"));
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
        gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    ")), gem_v_call_expr), gem_string(";\n"));
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
            gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    return ")), gem_v_call_expr), gem_string(";\n"));
        } else {
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    int64_t _ret = ")), gem_v_call_expr), gem_string(";\n"));
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
                    gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    double _ret = ")), gem_v_call_expr), gem_string(";\n"));
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
                        gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    char* _ret = ")), gem_v_call_expr), gem_string(";\n"));
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
                            gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    int _ret = ")), gem_v_call_expr), gem_string(";\n"));
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
                                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    void *_ret = ")), gem_v_call_expr), gem_string(";\n"));
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
                                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    ")), gem_v_call_expr), gem_string(";\n"));
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
    GemVal _t1596[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_v_name), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t1596, 2));
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
    GemVal _t1598 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1598, gem_string("params"));
    GemVal _t1599 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1599, gem_string("body"));
    GemVal _t1600 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1600, gem_string("name"));
    GemVal _t1601[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t1601, 1);
    {
        GemVal gem_v__for_i_42 = gem_int(0);
    GemVal _t1602[] = {gem_v_body};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t1602, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
            GemVal gem_v_i = gem_v__for_i_42;
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
    GemVal _t1603[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1605;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1603, 1), gem_string("table")))) {
        _t1605 = gem_eq(gem_type_fn(NULL, _t1603, 1), gem_string("table"));
    } else {
        GemVal _t1604 = gem_table_get(gem_v_body, gem_v_i);
        _t1605 = gem_eq(gem_table_get(_t1604, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1605)) {
    GemVal _t1606 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1607[] = {gem_v_scope_vars, gem_table_get(_t1606, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1607, 2));
            }
    GemVal _t1608[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1610;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1608, 1), gem_string("table")))) {
        _t1610 = gem_eq(gem_type_fn(NULL, _t1608, 1), gem_string("table"));
    } else {
        GemVal _t1609 = gem_table_get(gem_v_body, gem_v_i);
        _t1610 = gem_eq(gem_table_get(_t1609, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1610)) {
                {
                    GemVal gem_v__for_i_41 = gem_int(0);
    GemVal _t1611 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1612[] = {gem_table_get(_t1611, gem_string("stmts"))};
                    GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t1612, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
                        GemVal gem_v_j = gem_v__for_i_41;
                        gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
    GemVal _t1613 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1614[] = {gem_table_get(gem_table_get(_t1613, gem_string("stmts")), gem_v_j)};
    GemVal _t1617;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1614, 1), gem_string("table")))) {
        _t1617 = gem_eq(gem_type_fn(NULL, _t1614, 1), gem_string("table"));
    } else {
        GemVal _t1615 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1616 = gem_table_get(gem_table_get(_t1615, gem_string("stmts")), gem_v_j);
        _t1617 = gem_eq(gem_table_get(_t1616, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1617)) {
    GemVal _t1618 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1619 = gem_table_get(gem_table_get(_t1618, gem_string("stmts")), gem_v_j);
    GemVal _t1620[] = {gem_v_scope_vars, gem_table_get(_t1619, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1620, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1621 = (*gem_v_find_captured_in_scope);
    GemVal _t1622[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t1621.fn(_t1621.env, _t1622, 2);
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    *gem_v_boxed_vars = gem_v_captured;
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_v_name), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    {
        GemVal gem_v__for_i_43 = gem_int(0);
    GemVal _t1623[] = {gem_v_params};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t1623, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
            GemVal gem_v_i = gem_v__for_i_43;
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
    GemVal _t1624 = (*gem_v_mangle);
    GemVal _t1625[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1624.fn(_t1624.env, _t1625, 1);
    GemVal _t1626[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1626, 2))) {
                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal *")), gem_v_mp), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"));
    GemVal _t1627[] = {gem_v_i};
    GemVal _t1628[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    *")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1627, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1628, 1)), gem_string("] : GEM_NIL;\n"));
            } else {
    GemVal _t1629[] = {gem_v_i};
    GemVal _t1630[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal ")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1629, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1630, 1)), gem_string("] : GEM_NIL;\n"));
            }
        }
    }
    GemVal _t1631[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1631, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_44 = gem_int(0);
    GemVal _t1632[] = {gem_v_body};
            GemVal gem_v__for_limit_44 = gem_sub(gem_len_fn(NULL, _t1632, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
                GemVal gem_v_i = gem_v__for_i_44;
                gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
    GemVal _t1633 = (*gem_v_compile_stmt);
    GemVal _t1634[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1633.fn(_t1633.env, _t1634, 2)), gem_string("\n"));
            }
        }
    GemVal _t1635[] = {gem_v_body};
    GemVal _t1636 = (*gem_v_compile_stmt_return);
    GemVal _t1637[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1635, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1636.fn(_t1636.env, _t1637, 2)), gem_string("\n"));
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
    GemVal _t1639 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1639, gem_string("params"));
    GemVal _t1640 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1640, gem_string("body"));
    GemVal _t1641 = (*gem_v_anon_name);
    GemVal gem_v_name = _t1641.fn(_t1641.env, NULL, 0);
    GemVal _t1642[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t1642, 1);
    {
        GemVal gem_v__for_i_46 = gem_int(0);
    GemVal _t1643[] = {gem_v_body};
        GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t1643, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
            GemVal gem_v_i = gem_v__for_i_46;
            gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
    GemVal _t1644[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1646;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1644, 1), gem_string("table")))) {
        _t1646 = gem_eq(gem_type_fn(NULL, _t1644, 1), gem_string("table"));
    } else {
        GemVal _t1645 = gem_table_get(gem_v_body, gem_v_i);
        _t1646 = gem_eq(gem_table_get(_t1645, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1646)) {
    GemVal _t1647 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1648[] = {gem_v_inner_scope, gem_table_get(_t1647, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1648, 2));
            }
    GemVal _t1649[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1651;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1649, 1), gem_string("table")))) {
        _t1651 = gem_eq(gem_type_fn(NULL, _t1649, 1), gem_string("table"));
    } else {
        GemVal _t1650 = gem_table_get(gem_v_body, gem_v_i);
        _t1651 = gem_eq(gem_table_get(_t1650, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1651)) {
                {
                    GemVal gem_v__for_i_45 = gem_int(0);
    GemVal _t1652 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1653[] = {gem_table_get(_t1652, gem_string("stmts"))};
                    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t1653, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
                        GemVal gem_v_j = gem_v__for_i_45;
                        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
    GemVal _t1654 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1655[] = {gem_table_get(gem_table_get(_t1654, gem_string("stmts")), gem_v_j)};
    GemVal _t1658;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1655, 1), gem_string("table")))) {
        _t1658 = gem_eq(gem_type_fn(NULL, _t1655, 1), gem_string("table"));
    } else {
        GemVal _t1656 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1657 = gem_table_get(gem_table_get(_t1656, gem_string("stmts")), gem_v_j);
        _t1658 = gem_eq(gem_table_get(_t1657, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1658)) {
    GemVal _t1659 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1660 = gem_table_get(gem_table_get(_t1659, gem_string("stmts")), gem_v_j);
    GemVal _t1661[] = {gem_v_inner_scope, gem_table_get(_t1660, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1661, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1662 = (*gem_v_find_captured_in_scope);
    GemVal _t1663[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t1662.fn(_t1662.env, _t1663, 2);
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    GemVal _t1664[] = {gem_v_captures};
    GemVal _t1665[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t1664, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t1665, 2);
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_v_name);
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_v_struct_name), gem_string(" {\n"));
    {
        GemVal gem_v__for_items_47 = gem_v_captures;
        GemVal gem_v__for_i_47 = gem_int(0);
        while (1) {
            GemVal _t1666[] = {gem_v__for_items_47};
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_len_fn(NULL, _t1666, 1)))) break;
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_47, gem_v__for_i_47);
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
    GemVal _t1667 = (*gem_v_mangle);
    GemVal _t1668[] = {gem_v_cap};
            gem_v_struct_def = gem_add(gem_add(gem_add(gem_v_struct_def, gem_string("    GemVal *")), _t1667.fn(_t1667.env, _t1668, 1)), gem_string(";\n"));
        }
    }
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_v_name), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    GemVal _t1669[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1669, 1), gem_int(0)))) {
        gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    struct ")), gem_v_struct_name), gem_string(" *_cls = (struct ")), gem_v_struct_name), gem_string(" *)_env;\n"));
        {
            GemVal gem_v__for_items_48 = gem_v_captures;
            GemVal gem_v__for_i_48 = gem_int(0);
            while (1) {
                GemVal _t1670[] = {gem_v__for_items_48};
                if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_len_fn(NULL, _t1670, 1)))) break;
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_48, gem_v__for_i_48);
                gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
    GemVal _t1671 = (*gem_v_mangle);
    GemVal _t1672[] = {gem_v_cap};
                GemVal gem_v_mc = _t1671.fn(_t1671.env, _t1672, 1);
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal *")), gem_v_mc), gem_string(" = _cls->")), gem_v_mc), gem_string(";\n"));
            }
        }
    }
    {
        GemVal gem_v__for_i_49 = gem_int(0);
    GemVal _t1673[] = {gem_v_params};
        GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t1673, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
            GemVal gem_v_i = gem_v__for_i_49;
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
    GemVal _t1674 = (*gem_v_mangle);
    GemVal _t1675[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1674.fn(_t1674.env, _t1675, 1);
    GemVal _t1676[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1676, 2))) {
                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal *")), gem_v_mp), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"));
    GemVal _t1677[] = {gem_v_i};
    GemVal _t1678[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    *")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1677, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1678, 1)), gem_string("] : GEM_NIL;\n"));
            } else {
    GemVal _t1679[] = {gem_v_i};
    GemVal _t1680[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal ")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1679, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1680, 1)), gem_string("] : GEM_NIL;\n"));
            }
        }
    }
    GemVal _t1681[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1681, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_50 = gem_int(0);
    GemVal _t1682[] = {gem_v_body};
            GemVal gem_v__for_limit_50 = gem_sub(gem_len_fn(NULL, _t1682, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
                GemVal gem_v_i = gem_v__for_i_50;
                gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
    GemVal _t1683 = (*gem_v_compile_stmt);
    GemVal _t1684[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1683.fn(_t1683.env, _t1684, 2)), gem_string("\n"));
            }
        }
    GemVal _t1685[] = {gem_v_body};
    GemVal _t1686 = (*gem_v_compile_stmt_return);
    GemVal _t1687[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1685, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1686.fn(_t1686.env, _t1687, 2)), gem_string("\n"));
    } else {
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    }
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal _t1688[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t1688, 2));
    GemVal _t1689 = gem_table_new();
    gem_table_set(_t1689, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t1689, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t1689, gem_string("captures"), gem_v_captures);
    return _t1689;
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
    GemVal _t1691 = gem_table_new();
    GemVal gem_v_fn_defs = _t1691;
    GemVal _t1692 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t1692;
    GemVal _t1693 = gem_table_new();
    GemVal gem_v_extern_includes = _t1693;
    GemVal _t1694 = gem_table_new();
    GemVal gem_v_top_stmts = _t1694;
    {
    GemVal _t1695 = gem_v_ast;
        GemVal gem_v__for_items_51 = gem_table_get(_t1695, gem_string("stmts"));
        GemVal gem_v__for_i_51 = gem_int(0);
        while (1) {
            GemVal _t1696[] = {gem_v__for_items_51};
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t1696, 1)))) break;
            GemVal gem_v_s = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
    GemVal _t1697 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t1697, gem_string("tag")), gem_string("fn_def")))) {
    GemVal _t1698[] = {gem_v_fn_defs, gem_v_s};
                (void)(gem_push_fn(NULL, _t1698, 2));
            } else {
    GemVal _t1699 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t1699, gem_string("tag")), gem_string("extern_fn")))) {
    GemVal _t1700[] = {gem_v_extern_fns_list, gem_v_s};
                    (void)(gem_push_fn(NULL, _t1700, 2));
                } else {
    GemVal _t1701 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t1701, gem_string("tag")), gem_string("extern_include")))) {
    GemVal _t1702[] = {gem_v_extern_includes, gem_v_s};
                        (void)(gem_push_fn(NULL, _t1702, 2));
                    } else {
    GemVal _t1703[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t1703, 2));
                    }
                }
            }
        }
    }
    {
        GemVal gem_v__for_items_52 = gem_v_extern_fns_list;
        GemVal gem_v__for_i_52 = gem_int(0);
        while (1) {
            GemVal _t1704[] = {gem_v__for_items_52};
            if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_len_fn(NULL, _t1704, 1)))) break;
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_52, gem_v__for_i_52);
            gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
    GemVal _t1705 = gem_v_ef;
    GemVal _t1706[] = {(*gem_v_defined_fns), gem_table_get(_t1705, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1706, 2));
        }
    }
    {
        GemVal gem_v__for_items_53 = gem_v_fn_defs;
        GemVal gem_v__for_i_53 = gem_int(0);
        while (1) {
            GemVal _t1707[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t1707, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
    GemVal _t1708 = gem_v_fd;
    GemVal _t1709[] = {(*gem_v_defined_fns), gem_table_get(_t1708, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1709, 2));
    GemVal _t1710 = gem_v_fd;
    GemVal _t1711[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_table_get(_t1710, gem_string("name"))), gem_string("(void *_env, GemVal *args, int argc);"))};
            (void)(gem_push_fn(NULL, _t1711, 2));
        }
    }
    {
        GemVal gem_v__for_items_54 = gem_v_extern_fns_list;
        GemVal gem_v__for_i_54 = gem_int(0);
        while (1) {
            GemVal _t1712[] = {gem_v__for_items_54};
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t1712, 1)))) break;
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
    GemVal _t1713 = (*gem_v_compile_extern_fn);
    GemVal _t1714[] = {gem_v_ef};
    GemVal _t1715[] = {(*gem_v_functions), _t1713.fn(_t1713.env, _t1714, 1)};
            (void)(gem_push_fn(NULL, _t1715, 2));
        }
    }
    {
        GemVal gem_v__for_items_55 = gem_v_fn_defs;
        GemVal gem_v__for_i_55 = gem_int(0);
        while (1) {
            GemVal _t1716[] = {gem_v__for_items_55};
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t1716, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
    GemVal _t1717 = (*gem_v_compile_fn);
    GemVal _t1718[] = {gem_v_fd};
    GemVal _t1719[] = {(*gem_v_functions), _t1717.fn(_t1717.env, _t1718, 1)};
            (void)(gem_push_fn(NULL, _t1719, 2));
        }
    }
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    GemVal _t1720 = gem_table_new();
    GemVal _t1721 = (*gem_v_find_captured_in_scope);
    GemVal _t1722[] = {gem_v_top_stmts, _t1720};
    *gem_v_boxed_vars = _t1721.fn(_t1721.env, _t1722, 2);
    GemVal _t1723 = (*gem_v_compile_stmts);
    GemVal _t1724[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t1723.fn(_t1723.env, _t1724, 2);
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal gem_v_has_main = gem_bool(0);
    {
        GemVal gem_v__for_items_56 = gem_v_fn_defs;
        GemVal gem_v__for_i_56 = gem_int(0);
        while (1) {
            GemVal _t1725[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t1725, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
    GemVal _t1726 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t1726, gem_string("name")), gem_string("main")))) {
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
        GemVal gem_v__for_items_57 = gem_v_extern_includes;
        GemVal gem_v__for_i_57 = gem_int(0);
        while (1) {
            GemVal _t1727[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t1727, 1)))) break;
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
    GemVal _t1728 = gem_v_ei;
            gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string("#include \"")), gem_table_get(_t1728, gem_string("path"))), gem_string("\"\n"));
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
    {
        GemVal gem_v__for_i_58 = gem_int(0);
    GemVal _t1729[] = {(*gem_v_forward_decls)};
        GemVal gem_v__for_limit_58 = gem_len_fn(NULL, _t1729, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
            GemVal gem_v_i = gem_v__for_i_58;
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
    GemVal _t1730[] = {(*gem_v_forward_decls)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t1730, 1), gem_int(1))))) {
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    {
        GemVal gem_v__for_i_59 = gem_int(0);
    GemVal _t1731[] = {(*gem_v_functions)};
        GemVal gem_v__for_limit_59 = gem_len_fn(NULL, _t1731, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
            GemVal gem_v_i = gem_v__for_i_59;
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
    GemVal _t1732[] = {(*gem_v_functions)};
            if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t1732, 1), gem_int(1))))) {
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
    GemVal _t748 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t748;
    GemVal _t749 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t749;
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
    GemVal _t750 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t750;
    GemVal _t751 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t751;
    gem_table_set((*gem_v_builtins), gem_string("print"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("error"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("len"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("type"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("to_string"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("push"), gem_bool(1));
    GemVal _t752 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t752;
    struct _closure__anon_22 *_t754 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t754->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t754);
    struct _closure__anon_23 *_t756 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t756->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t756);
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
    struct _closure__anon_25 *_t913 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t913->gem_v_builtins = gem_v_builtins;
    _t913->gem_v_collect_free_node = gem_v_collect_free_node;
    _t913->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t913->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t913);
    struct _closure__anon_26 *_t938 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t938->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t938);
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
    struct _closure__anon_27 *_t1010 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1010->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1010->gem_v_walk_captures = gem_v_walk_captures;
    _t1010->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t1010);
    struct _closure__anon_28 *_t1034 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1034->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t1034);
    struct _closure__anon_29 *_t1038 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1038->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t1038);
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
    struct _closure__anon_30 *_t1109 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1109->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1109->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1109->gem_v_compile_array = gem_v_compile_array;
    _t1109->gem_v_compile_binop = gem_v_compile_binop;
    _t1109->gem_v_compile_call = gem_v_compile_call;
    _t1109->gem_v_compile_expr = gem_v_compile_expr;
    _t1109->gem_v_compile_table = gem_v_compile_table;
    _t1109->gem_v_defined_fns = gem_v_defined_fns;
    _t1109->gem_v_mangle = gem_v_mangle;
    _t1109->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1109);
    struct _closure__anon_31 *_t1121 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1121->gem_v_compile_expr = gem_v_compile_expr;
    _t1121->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1121);
    struct _closure__anon_32 *_t1131 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1131->gem_v_compile_expr = gem_v_compile_expr;
    _t1131->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1131);
    struct _closure__anon_33 *_t1155 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1155->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1155->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1155->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1155->gem_v_mangle = gem_v_mangle;
    _t1155->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1155);
    struct _closure__anon_34 *_t1233 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1233->gem_v_compile_expr = gem_v_compile_expr;
    _t1233->gem_v_defined_fns = gem_v_defined_fns;
    _t1233->gem_v_source_name = gem_v_source_name;
    _t1233->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1233);
    struct _closure__anon_35 *_t1329 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1329->gem_v_compile_expr = gem_v_compile_expr;
    _t1329->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1329);
    struct _closure__anon_36 *_t1333 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1333->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1333);
    struct _closure__anon_37 *_t1409 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1409->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1409->gem_v_compile_expr = gem_v_compile_expr;
    _t1409->gem_v_compile_if = gem_v_compile_if;
    _t1409->gem_v_compile_match = gem_v_compile_match;
    _t1409->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1409->gem_v_compile_while = gem_v_compile_while;
    _t1409->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1409);
    struct _closure__anon_38 *_t1469 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t1469->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1469->gem_v_compile_expr = gem_v_compile_expr;
    _t1469->gem_v_compile_if_return = gem_v_compile_if_return;
    _t1469->gem_v_compile_match_return = gem_v_compile_match_return;
    _t1469->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1469->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1469->gem_v_compile_while = gem_v_compile_while;
    _t1469->gem_v_mangle = gem_v_mangle;
    _t1469->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t1469);
    struct _closure__anon_39 *_t1480 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t1480->gem_v_compile_expr = gem_v_compile_expr;
    _t1480->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t1480);
    struct _closure__anon_40 *_t1501 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t1501->gem_v_compile_expr = gem_v_compile_expr;
    _t1501->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1501->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t1501);
    struct _closure__anon_41 *_t1513 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t1513->gem_v_compile_expr = gem_v_compile_expr;
    _t1513->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t1513);
    struct _closure__anon_42 *_t1534 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t1534->gem_v_compile_expr = gem_v_compile_expr;
    _t1534->gem_v_compile_stmts = gem_v_compile_stmts;
    _t1534->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t1534);
    struct _closure__anon_43 *_t1565 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t1565->gem_v_compile_expr = gem_v_compile_expr;
    _t1565->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1565->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1565->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t1565);
    struct _closure__anon_44 *_t1597 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t1597->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t1597);
    struct _closure__anon_45 *_t1638 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t1638->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1638->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1638->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1638->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1638->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t1638);
    struct _closure__anon_46 *_t1690 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t1690->gem_v_anon_name = gem_v_anon_name;
    _t1690->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1690->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1690->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1690->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1690->gem_v_functions = gem_v_functions;
    _t1690->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t1690);
    struct _closure__anon_47 *_t1733 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t1733->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1733->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t1733->gem_v_compile_fn = gem_v_compile_fn;
    _t1733->gem_v_compile_stmts = gem_v_compile_stmts;
    _t1733->gem_v_defined_fns = gem_v_defined_fns;
    _t1733->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1733->gem_v_forward_decls = gem_v_forward_decls;
    _t1733->gem_v_functions = gem_v_functions;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t1733);
    GemVal _t1734 = gem_table_new();
    gem_table_set(_t1734, gem_string("compile"), gem_v_compile);
    return _t1734;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t1735 = gem_table_new();
    GemVal gem_v_result = _t1735;
    GemVal gem_v_current = gem_string("");
    {
        GemVal gem_v__for_i_60 = gem_int(0);
    GemVal _t1736[] = {gem_v_s};
        GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t1736, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
            GemVal gem_v_i = gem_v__for_i_60;
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
    GemVal _t1737[] = {gem_v_result, gem_v_current};
                (void)(gem_push_fn(NULL, _t1737, 2));
                gem_v_current = gem_string("");
            } else {
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
    GemVal _t1738[] = {gem_v_result, gem_v_current};
    (void)(gem_push_fn(NULL, _t1738, 2));
    return gem_v_result;
}

static GemVal gem_fn_str_replace_all(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    GemVal gem_v_i = gem_int(0);
    GemVal _t1739[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t1739, 1);
    GemVal _t1740[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t1740, 1);
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
    GemVal _t1741 = gem_table_new();
    GemVal gem_v_new_stmts = _t1741;
    GemVal gem_v_count = gem_int(0);
    {
    GemVal _t1742 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t1742, gem_string("stmts"));
        GemVal gem_v__for_i_1 = gem_int(0);
        while (1) {
            GemVal _t1743[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t1743, 1)))) break;
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t1744[] = {gem_v_stmt};
    GemVal _t1746;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1744, 1), gem_string("table")))) {
        _t1746 = gem_eq(gem_type_fn(NULL, _t1744, 1), gem_string("table"));
    } else {
        GemVal _t1745 = gem_v_stmt;
        _t1746 = gem_eq(gem_table_get(_t1745, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t1746)) {
    GemVal _t1747 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t1747, gem_string("path"));
    GemVal _t1748[] = {gem_v_path};
    GemVal _t1756;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t1748, 1), gem_int(4)))) {
        _t1756 = gem_lt(gem_len_fn(NULL, _t1748, 1), gem_int(4));
    } else {
        GemVal _t1749[] = {gem_v_path};
        GemVal _t1751;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1749, 1), gem_int(4))), gem_string(".")))) {
                _t1751 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1749, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t1750[] = {gem_v_path};
                _t1751 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1750, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t1753;
        if (gem_truthy(_t1751)) {
                _t1753 = _t1751;
        } else {
                GemVal _t1752[] = {gem_v_path};
                _t1753 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1752, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t1755;
        if (gem_truthy(_t1753)) {
                _t1755 = _t1753;
        } else {
                GemVal _t1754[] = {gem_v_path};
                _t1755 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1754, 1), gem_int(1))), gem_string("m"));
        }
        _t1756 = _t1755;
    }
                if (gem_truthy(_t1756)) {
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
    GemVal _t1757[] = {gem_v_base_dir, gem_v_path};
    GemVal _t1758[] = {gem_fn_gem_path_join(NULL, _t1757, 2)};
                GemVal gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t1758, 1);
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
    GemVal _t1759[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t1759, 1);
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
    GemVal _t1760[] = {gem_add(gem_add(gem_string("cannot load '"), gem_v_full_path), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 39, _t1760, 1));
                }
    GemVal _t1761[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t1761, 1);
    GemVal _t1762[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t1762, 1);
    GemVal _t1763[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t1763, 3);
                {
    GemVal _t1764 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t1764, gem_string("stmts"));
                    GemVal gem_v__for_i_2 = gem_int(0);
                    while (1) {
                        GemVal _t1765[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t1765, 1)))) break;
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
    GemVal _t1766[] = {gem_v_new_stmts};
    return gem_fn_make_program(NULL, _t1766, 1);
}

int main(void) {
    GemVal gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
    GemVal _t1767[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 62, _t1767, 1));
    }
    GemVal _t1768[] = {gem_int(1)};
    GemVal gem_v_src_path = gem_fn_gem_get_argv(NULL, _t1768, 1);
    GemVal _t1769[] = {gem_v_src_path};
    GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t1769, 1);
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
    GemVal _t1770[] = {gem_add(gem_add(gem_string("cannot read '"), gem_v_src_path), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t1770, 1));
    }
    GemVal _t1771[] = {gem_v_source};
    GemVal gem_v_ast = gem_fn_parse_source(NULL, _t1771, 1);
    GemVal _t1772[] = {gem_v_src_path};
    GemVal gem_v_base_dir = gem_fn_gem_dirname(NULL, _t1772, 1);
    GemVal _t1773 = gem_table_new();
    GemVal gem_v_loaded = _t1773;
    GemVal _t1774[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    GemVal gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t1774, 3);
    GemVal _t1775[] = {gem_v_src_path};
    GemVal gem_v_cg = gem_fn_make_codegen(NULL, _t1775, 1);
    GemVal _t1776 = gem_v_cg;
    GemVal _t1777 = gem_table_get(_t1776, gem_string("compile"));
    GemVal _t1778[] = {gem_v_resolved_ast};
    GemVal gem_v_c_code = _t1777.fn(_t1777.env, _t1778, 1);
    GemVal _t1779[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t1779, 1));
    return 0;
}

