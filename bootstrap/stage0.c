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
    GemVal gem_v_count = gem_int(0);
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
    GemVal _t53;
    if (!gem_truthy(gem_gt(gem_v_count, gem_int(0)))) {
        _t53 = gem_gt(gem_v_count, gem_int(0));
    } else {
        GemVal _t52 = gem_table_get(gem_v_tokens, gem_sub(gem_v_count, gem_int(1)));
        _t53 = gem_eq(gem_table_get(_t52, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t53)) {
                gem_v_should_add = gem_bool(0);
            }
            if (gem_truthy(gem_v_should_add)) {
    GemVal _t54 = gem_table_new();
    gem_table_set(_t54, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t54, gem_string("value"), gem_string("\n"));
    gem_table_set(_t54, gem_string("line"), gem_v_line);
                gem_table_set(gem_v_tokens, gem_v_count, _t54);
                gem_v_count = gem_add(gem_v_count, gem_int(1));
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
    GemVal _t55;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t55 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t55 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t56;
    if (gem_truthy(_t55)) {
        _t56 = _t55;
    } else {
        _t56 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t56)) {
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
                            while (1) {
                                GemVal _t57;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t57 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t57 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t57)) break;
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
    GemVal _t58[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t58, 1))) {
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t60;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t60 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t59[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t60 = gem_fn_is_digit(NULL, _t59, 1);
                }
                if (!gem_truthy(_t60)) break;
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
    GemVal _t61;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t61 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t61 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t61;
            if (gem_truthy(gem_v_has_dot)) {
                gem_v_val = gem_add(gem_v_val, gem_string("."));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
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
            }
    GemVal _t64 = gem_table_new();
    gem_table_set(_t64, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t64, gem_string("value"), gem_v_val);
    gem_table_set(_t64, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t64);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            continue;
        }
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t65;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t65 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t65 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t65)) break;
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
    GemVal _t66[] = {gem_v_line};
    GemVal _t67[] = {gem_add(gem_string("unterminated string escape at line "), gem_to_string_fn(NULL, _t66, 1))};
                        (void)(gem_error_at_fn("compiler/main.gem", 190, _t67, 1));
                    }
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
                    GemVal _t68 = gem_v_esc;
                    if (gem_truthy(gem_eq(_t68, gem_string("n")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
                    } else if (gem_truthy(gem_eq(_t68, gem_string("t")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\t"));
                    } else if (gem_truthy(gem_eq(_t68, gem_string("\\")))) {
                        gem_v_val = gem_add(gem_v_val, gem_string("\\"));
                    } else if (gem_truthy(gem_eq(_t68, gem_string("\"")))) {
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
    GemVal _t69[] = {gem_v_line};
    GemVal _t70[] = {gem_add(gem_string("unterminated string at line "), gem_to_string_fn(NULL, _t69, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 211, _t70, 1));
            }
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
    GemVal _t71 = gem_table_new();
    gem_table_set(_t71, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t71, gem_string("value"), gem_v_val);
    gem_table_set(_t71, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t71);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            continue;
        }
    GemVal _t72[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t72, 1))) {
            GemVal gem_v_val = gem_string("");
            while (1) {
                GemVal _t74;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t74 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t73[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t74 = gem_fn_is_alnum(NULL, _t73, 1);
                }
                if (!gem_truthy(_t74)) break;
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            if (gem_truthy(gem_eq(gem_table_get(gem_v_kw, gem_v_val), gem_bool(1)))) {
    GemVal _t75 = gem_table_new();
    gem_table_set(_t75, gem_string("type"), gem_v_val);
    gem_table_set(_t75, gem_string("value"), gem_v_val);
    gem_table_set(_t75, gem_string("line"), gem_v_line);
                gem_table_set(gem_v_tokens, gem_v_count, _t75);
            } else {
    GemVal _t76 = gem_table_new();
    gem_table_set(_t76, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t76, gem_string("value"), gem_v_val);
    gem_table_set(_t76, gem_string("line"), gem_v_line);
                gem_table_set(gem_v_tokens, gem_v_count, _t76);
            }
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            continue;
        }
        GemVal gem_v_next = gem_string("");
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
    GemVal _t77;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t77 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t77 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t77)) {
    GemVal _t78 = gem_table_new();
    gem_table_set(_t78, gem_string("type"), gem_string("=="));
    gem_table_set(_t78, gem_string("value"), gem_string("=="));
    gem_table_set(_t78, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t78);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t79;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t79 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t79 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t79)) {
    GemVal _t80 = gem_table_new();
    gem_table_set(_t80, gem_string("type"), gem_string("!="));
    gem_table_set(_t80, gem_string("value"), gem_string("!="));
    gem_table_set(_t80, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t80);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t81;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t81 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t81 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t81)) {
    GemVal _t82 = gem_table_new();
    gem_table_set(_t82, gem_string("type"), gem_string("<="));
    gem_table_set(_t82, gem_string("value"), gem_string("<="));
    gem_table_set(_t82, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t82);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t83;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t83 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t83 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t83)) {
    GemVal _t84 = gem_table_new();
    gem_table_set(_t84, gem_string("type"), gem_string(">="));
    gem_table_set(_t84, gem_string("value"), gem_string(">="));
    gem_table_set(_t84, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t84);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t85;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t85 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t85 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t85)) {
    GemVal _t86 = gem_table_new();
    gem_table_set(_t86, gem_string("type"), gem_string("+="));
    gem_table_set(_t86, gem_string("value"), gem_string("+="));
    gem_table_set(_t86, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t86);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t87;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t87 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t87 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t87)) {
    GemVal _t88 = gem_table_new();
    gem_table_set(_t88, gem_string("type"), gem_string("-="));
    gem_table_set(_t88, gem_string("value"), gem_string("-="));
    gem_table_set(_t88, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t88);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t89;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t89 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t89 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t89)) {
    GemVal _t90 = gem_table_new();
    gem_table_set(_t90, gem_string("type"), gem_string("*="));
    gem_table_set(_t90, gem_string("value"), gem_string("*="));
    gem_table_set(_t90, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t90);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t91;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t91 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t91 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t91)) {
    GemVal _t92 = gem_table_new();
    gem_table_set(_t92, gem_string("type"), gem_string("/="));
    gem_table_set(_t92, gem_string("value"), gem_string("/="));
    gem_table_set(_t92, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t92);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t93;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t93 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t93 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t93)) {
    GemVal _t94 = gem_table_new();
    gem_table_set(_t94, gem_string("type"), gem_string("->"));
    gem_table_set(_t94, gem_string("value"), gem_string("->"));
    gem_table_set(_t94, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t94);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
    GemVal _t95;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t95 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t95 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t96;
    if (gem_truthy(_t95)) {
        _t96 = _t95;
    } else {
        _t96 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t97;
    if (gem_truthy(_t96)) {
        _t97 = _t96;
    } else {
        _t97 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t98;
    if (gem_truthy(_t97)) {
        _t98 = _t97;
    } else {
        _t98 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t98)) {
    GemVal _t99 = gem_table_new();
    gem_table_set(_t99, gem_string("type"), gem_v_ch);
    gem_table_set(_t99, gem_string("value"), gem_v_ch);
    gem_table_set(_t99, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t99);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t100;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t100 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t100 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t101;
    if (gem_truthy(_t100)) {
        _t101 = _t100;
    } else {
        _t101 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t101)) {
    GemVal _t102 = gem_table_new();
    gem_table_set(_t102, gem_string("type"), gem_v_ch);
    gem_table_set(_t102, gem_string("value"), gem_v_ch);
    gem_table_set(_t102, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t102);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t103;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t103 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t103 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t104;
    if (gem_truthy(_t103)) {
        _t104 = _t103;
    } else {
        _t104 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t105;
    if (gem_truthy(_t104)) {
        _t105 = _t104;
    } else {
        _t105 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t106;
    if (gem_truthy(_t105)) {
        _t106 = _t105;
    } else {
        _t106 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t107;
    if (gem_truthy(_t106)) {
        _t107 = _t106;
    } else {
        _t107 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t107)) {
    GemVal _t108 = gem_table_new();
    gem_table_set(_t108, gem_string("type"), gem_v_ch);
    gem_table_set(_t108, gem_string("value"), gem_v_ch);
    gem_table_set(_t108, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t108);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t109;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t109 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t109 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t110;
    if (gem_truthy(_t109)) {
        _t110 = _t109;
    } else {
        _t110 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t111;
    if (gem_truthy(_t110)) {
        _t111 = _t110;
    } else {
        _t111 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t111)) {
    GemVal _t112 = gem_table_new();
    gem_table_set(_t112, gem_string("type"), gem_v_ch);
    gem_table_set(_t112, gem_string("value"), gem_v_ch);
    gem_table_set(_t112, gem_string("line"), gem_v_line);
            gem_table_set(gem_v_tokens, gem_v_count, _t112);
            gem_v_count = gem_add(gem_v_count, gem_int(1));
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
    GemVal _t113[] = {gem_v_line};
    GemVal _t114[] = {gem_add(gem_add(gem_add(gem_string("unexpected character: '"), gem_v_ch), gem_string("' at line ")), gem_to_string_fn(NULL, _t113, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 322, _t114, 1));
    }
    GemVal _t115 = gem_table_new();
    gem_table_set(_t115, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t115, gem_string("value"), gem_string(""));
    gem_table_set(_t115, gem_string("line"), gem_v_line);
    gem_table_set(gem_v_tokens, gem_v_count, _t115);
    return gem_v_tokens;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t116[] = {gem_v_source};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t116, 1);
    GemVal _t117[] = {gem_v_tokens};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t117, 1);
    GemVal _t118 = gem_v_p;
    GemVal _t119 = gem_table_get(_t118, gem_string("parse"));
    return _t119.fn(_t119.env, NULL, 0);
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
    GemVal _t123 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    return gem_eq(gem_table_get(_t123, gem_string("type")), gem_string("EOF"));
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
    GemVal _t125 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t125, gem_string("type")), gem_v_tp))) {
    GemVal _t126 = gem_v_t;
    GemVal _t127 = gem_v_t;
    GemVal _t128[] = {gem_table_get(_t127, gem_string("line"))};
    GemVal _t129[] = {gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_v_tp), gem_string("' but got '")), gem_table_get(_t126, gem_string("type"))), gem_string("' at line ")), gem_to_string_fn(NULL, _t128, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 45, _t129, 1));
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
        GemVal _t131 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t131, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t133 = gem_table_new();
    GemVal gem_v_params = _t133;
    GemVal gem_v_cnt = gem_int(0);
    GemVal _t134 = (*gem_v_peek);
    GemVal _t135 = _t134.fn(_t134.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t135, gem_string("type")), gem_string("NAME")))) {
    GemVal _t136 = (*gem_v_advance);
    GemVal _t137 = _t136.fn(_t136.env, NULL, 0);
        gem_table_set(gem_v_params, gem_int(0), gem_table_get(_t137, gem_string("value")));
        gem_v_cnt = gem_int(1);
        while (1) {
            GemVal _t138 = (*gem_v_peek);
            GemVal _t139 = _t138.fn(_t138.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t139, gem_string("type")), gem_string(",")))) break;
    GemVal _t140 = (*gem_v_advance);
            (void)(_t140.fn(_t140.env, NULL, 0));
    GemVal _t141 = (*gem_v_expect);
    GemVal _t142[] = {gem_string("NAME")};
    GemVal _t143 = _t141.fn(_t141.env, _t142, 1);
            gem_table_set(gem_v_params, gem_v_cnt, gem_table_get(_t143, gem_string("value")));
            gem_v_cnt = gem_add(gem_v_cnt, gem_int(1));
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
    GemVal _t145 = gem_table_new();
    GemVal gem_v_stmts = _t145;
    GemVal gem_v_cnt = gem_int(0);
    GemVal _t146 = (*gem_v_skip_nl);
    (void)(_t146.fn(_t146.env, NULL, 0));
    while (1) {
        GemVal _t147 = (*gem_v_peek);
        GemVal _t148 = _t147.fn(_t147.env, NULL, 0);
        GemVal _t151;
        if (!gem_truthy(gem_neq(gem_table_get(_t148, gem_string("type")), gem_string("end")))) {
                _t151 = gem_neq(gem_table_get(_t148, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t149 = (*gem_v_peek);
                GemVal _t150 = _t149.fn(_t149.env, NULL, 0);
                _t151 = gem_neq(gem_table_get(_t150, gem_string("type")), gem_string("elif"));
        }
        GemVal _t154;
        if (!gem_truthy(_t151)) {
                _t154 = _t151;
        } else {
                GemVal _t152 = (*gem_v_peek);
                GemVal _t153 = _t152.fn(_t152.env, NULL, 0);
                _t154 = gem_neq(gem_table_get(_t153, gem_string("type")), gem_string("else"));
        }
        GemVal _t157;
        if (!gem_truthy(_t154)) {
                _t157 = _t154;
        } else {
                GemVal _t155 = (*gem_v_peek);
                GemVal _t156 = _t155.fn(_t155.env, NULL, 0);
                _t157 = gem_neq(gem_table_get(_t156, gem_string("type")), gem_string("when"));
        }
        GemVal _t159;
        if (!gem_truthy(_t157)) {
                _t159 = _t157;
        } else {
                GemVal _t158 = (*gem_v_at_end);
                _t159 = gem_not(_t158.fn(_t158.env, NULL, 0));
        }
        if (!gem_truthy(_t159)) break;
    GemVal _t160 = (*gem_v_parse_stmt);
        gem_table_set(gem_v_stmts, gem_v_cnt, _t160.fn(_t160.env, NULL, 0));
        gem_v_cnt = gem_add(gem_v_cnt, gem_int(1));
    GemVal _t161 = (*gem_v_skip_nl);
        (void)(_t161.fn(_t161.env, NULL, 0));
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
    GemVal _t163 = gem_table_new();
    GemVal gem_v_stmts = _t163;
    GemVal gem_v_cnt = gem_int(0);
    GemVal _t164 = (*gem_v_skip_nl);
    (void)(_t164.fn(_t164.env, NULL, 0));
    while (1) {
        GemVal _t165 = (*gem_v_peek);
        GemVal _t166 = _t165.fn(_t165.env, NULL, 0);
        GemVal _t168;
        if (!gem_truthy(gem_neq(gem_table_get(_t166, gem_string("type")), gem_string("end")))) {
                _t168 = gem_neq(gem_table_get(_t166, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t167 = (*gem_v_at_end);
                _t168 = gem_not(_t167.fn(_t167.env, NULL, 0));
        }
        if (!gem_truthy(_t168)) break;
    GemVal _t169 = (*gem_v_parse_stmt);
        gem_table_set(gem_v_stmts, gem_v_cnt, _t169.fn(_t169.env, NULL, 0));
        gem_v_cnt = gem_add(gem_v_cnt, gem_int(1));
    GemVal _t170 = (*gem_v_skip_nl);
        (void)(_t170.fn(_t170.env, NULL, 0));
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
    GemVal _t172 = (*gem_v_peek);
    GemVal gem_v_t = _t172.fn(_t172.env, NULL, 0);
    GemVal _t173 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t173, gem_string("type")), gem_string("NUMBER")))) {
    GemVal _t174 = (*gem_v_advance);
        (void)(_t174.fn(_t174.env, NULL, 0));
    GemVal _t175 = gem_v_t;
    GemVal _t176[] = {gem_table_get(_t175, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t176, 1))) {
    GemVal _t177 = gem_v_t;
    GemVal _t178[] = {gem_table_get(_t177, gem_string("value"))};
    GemVal _t179[] = {gem_fn_atof(NULL, _t178, 1)};
            return gem_fn_make_float(NULL, _t179, 1);
        }
    GemVal _t180 = gem_v_t;
    GemVal _t181[] = {gem_table_get(_t180, gem_string("value"))};
    GemVal _t182[] = {gem_fn_str_to_int(NULL, _t181, 1)};
        return gem_fn_make_int(NULL, _t182, 1);
    }
    GemVal _t183 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t183, gem_string("type")), gem_string("STRING")))) {
    GemVal _t184 = (*gem_v_advance);
        (void)(_t184.fn(_t184.env, NULL, 0));
    GemVal _t185 = gem_v_t;
    GemVal _t186[] = {gem_table_get(_t185, gem_string("value"))};
        return gem_fn_make_string(NULL, _t186, 1);
    }
    GemVal _t187 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t187, gem_string("type")), gem_string("true")))) {
    GemVal _t188 = (*gem_v_advance);
        (void)(_t188.fn(_t188.env, NULL, 0));
    GemVal _t189[] = {gem_bool(1)};
        return gem_fn_make_bool(NULL, _t189, 1);
    }
    GemVal _t190 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t190, gem_string("type")), gem_string("false")))) {
    GemVal _t191 = (*gem_v_advance);
        (void)(_t191.fn(_t191.env, NULL, 0));
    GemVal _t192[] = {gem_bool(0)};
        return gem_fn_make_bool(NULL, _t192, 1);
    }
    GemVal _t193 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t193, gem_string("type")), gem_string("nil")))) {
    GemVal _t194 = (*gem_v_advance);
        (void)(_t194.fn(_t194.env, NULL, 0));
        return gem_fn_make_nil_node(NULL, NULL, 0);
    }
    GemVal _t195 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t195, gem_string("type")), gem_string("fn")))) {
    GemVal _t196 = (*gem_v_advance);
        (void)(_t196.fn(_t196.env, NULL, 0));
    GemVal _t197 = (*gem_v_expect);
    GemVal _t198[] = {gem_string("(")};
        (void)(_t197.fn(_t197.env, _t198, 1));
    GemVal _t199 = (*gem_v_parse_params);
        GemVal gem_v_params = _t199.fn(_t199.env, NULL, 0);
    GemVal _t200 = (*gem_v_expect);
    GemVal _t201[] = {gem_string(")")};
        (void)(_t200.fn(_t200.env, _t201, 1));
    GemVal _t202 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t202.fn(_t202.env, NULL, 0);
    GemVal _t203 = (*gem_v_expect);
    GemVal _t204[] = {gem_string("end")};
        (void)(_t203.fn(_t203.env, _t204, 1));
    GemVal _t205[] = {gem_v_params, gem_v_body};
        return gem_fn_make_anon_fn(NULL, _t205, 2);
    }
    GemVal _t206 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t206, gem_string("type")), gem_string("{")))) {
    GemVal _t207 = (*gem_v_advance);
        (void)(_t207.fn(_t207.env, NULL, 0));
    GemVal _t208 = (*gem_v_skip_nl);
        (void)(_t208.fn(_t208.env, NULL, 0));
    GemVal _t209 = gem_table_new();
        GemVal gem_v_entries = _t209;
        GemVal gem_v_cnt = gem_int(0);
        while (1) {
            GemVal _t210 = (*gem_v_peek);
            GemVal _t211 = _t210.fn(_t210.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t211, gem_string("type")), gem_string("}")))) break;
    GemVal _t212 = (*gem_v_expect);
    GemVal _t213[] = {gem_string("NAME")};
    GemVal _t214 = _t212.fn(_t212.env, _t213, 1);
            GemVal gem_v_key = gem_table_get(_t214, gem_string("value"));
    GemVal _t215 = (*gem_v_expect);
    GemVal _t216[] = {gem_string(":")};
            (void)(_t215.fn(_t215.env, _t216, 1));
    GemVal _t217 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t217.fn(_t217.env, NULL, 0);
    GemVal _t218[] = {gem_v_key, gem_v_val};
            gem_table_set(gem_v_entries, gem_v_cnt, gem_fn_make_table_entry(NULL, _t218, 2));
            gem_v_cnt = gem_add(gem_v_cnt, gem_int(1));
    GemVal _t219 = (*gem_v_skip_nl);
            (void)(_t219.fn(_t219.env, NULL, 0));
    GemVal _t220 = (*gem_v_peek);
    GemVal _t221 = _t220.fn(_t220.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t221, gem_string("type")), gem_string(",")))) {
    GemVal _t222 = (*gem_v_advance);
                (void)(_t222.fn(_t222.env, NULL, 0));
    GemVal _t223 = (*gem_v_skip_nl);
                (void)(_t223.fn(_t223.env, NULL, 0));
            }
        }
    GemVal _t224 = (*gem_v_expect);
    GemVal _t225[] = {gem_string("}")};
        (void)(_t224.fn(_t224.env, _t225, 1));
    GemVal _t226[] = {gem_v_entries};
        return gem_fn_make_table(NULL, _t226, 1);
    }
    GemVal _t227 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t227, gem_string("type")), gem_string("[")))) {
    GemVal _t228 = (*gem_v_advance);
        (void)(_t228.fn(_t228.env, NULL, 0));
    GemVal _t229 = (*gem_v_skip_nl);
        (void)(_t229.fn(_t229.env, NULL, 0));
    GemVal _t230 = gem_table_new();
        GemVal gem_v_elements = _t230;
        GemVal gem_v_cnt = gem_int(0);
        while (1) {
            GemVal _t231 = (*gem_v_peek);
            GemVal _t232 = _t231.fn(_t231.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t232, gem_string("type")), gem_string("]")))) break;
    GemVal _t233 = (*gem_v_parse_expr);
            gem_table_set(gem_v_elements, gem_v_cnt, _t233.fn(_t233.env, NULL, 0));
            gem_v_cnt = gem_add(gem_v_cnt, gem_int(1));
    GemVal _t234 = (*gem_v_skip_nl);
            (void)(_t234.fn(_t234.env, NULL, 0));
    GemVal _t235 = (*gem_v_peek);
    GemVal _t236 = _t235.fn(_t235.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t236, gem_string("type")), gem_string(",")))) {
    GemVal _t237 = (*gem_v_advance);
                (void)(_t237.fn(_t237.env, NULL, 0));
    GemVal _t238 = (*gem_v_skip_nl);
                (void)(_t238.fn(_t238.env, NULL, 0));
            }
        }
    GemVal _t239 = (*gem_v_expect);
    GemVal _t240[] = {gem_string("]")};
        (void)(_t239.fn(_t239.env, _t240, 1));
    GemVal _t241[] = {gem_v_elements};
        return gem_fn_make_array(NULL, _t241, 1);
    }
    GemVal _t242 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t242, gem_string("type")), gem_string("(")))) {
    GemVal _t243 = (*gem_v_advance);
        (void)(_t243.fn(_t243.env, NULL, 0));
    GemVal _t244 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t244.fn(_t244.env, NULL, 0);
    GemVal _t245 = (*gem_v_expect);
    GemVal _t246[] = {gem_string(")")};
        (void)(_t245.fn(_t245.env, _t246, 1));
        return gem_v_e;
    }
    GemVal _t247 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t247, gem_string("type")), gem_string("NAME")))) {
    GemVal _t248 = (*gem_v_advance);
        (void)(_t248.fn(_t248.env, NULL, 0));
    GemVal _t249 = gem_v_t;
    GemVal _t250[] = {gem_table_get(_t249, gem_string("value"))};
        return gem_fn_make_var(NULL, _t250, 1);
    }
    GemVal _t251 = gem_v_t;
    GemVal _t252 = gem_v_t;
    GemVal _t253[] = {gem_table_get(_t252, gem_string("line"))};
    GemVal _t254[] = {gem_add(gem_add(gem_add(gem_string("unexpected token '"), gem_table_get(_t251, gem_string("type"))), gem_string("' at line ")), gem_to_string_fn(NULL, _t253, 1))};
    return gem_error_at_fn("compiler/main.gem", 215, _t254, 1);
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
    GemVal _t256 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t256.fn(_t256.env, NULL, 0);
    while (1) {
        if (!gem_truthy(gem_bool(1))) break;
    GemVal _t257 = (*gem_v_peek);
    GemVal _t258 = _t257.fn(_t257.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t258, gem_string("type")), gem_string("(")))) {
    GemVal _t259 = (*gem_v_peek);
    GemVal _t260 = _t259.fn(_t259.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t260, gem_string("line"));
    GemVal _t261 = (*gem_v_advance);
            (void)(_t261.fn(_t261.env, NULL, 0));
    GemVal _t262 = gem_table_new();
            GemVal gem_v_args = _t262;
            GemVal gem_v_argc = gem_int(0);
    GemVal _t263 = (*gem_v_peek);
    GemVal _t264 = _t263.fn(_t263.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t264, gem_string("type")), gem_string(")")))) {
    GemVal _t265 = (*gem_v_parse_expr);
                gem_table_set(gem_v_args, gem_int(0), _t265.fn(_t265.env, NULL, 0));
                gem_v_argc = gem_int(1);
                while (1) {
                    GemVal _t266 = (*gem_v_peek);
                    GemVal _t267 = _t266.fn(_t266.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t267, gem_string("type")), gem_string(",")))) break;
    GemVal _t268 = (*gem_v_advance);
                    (void)(_t268.fn(_t268.env, NULL, 0));
    GemVal _t269 = (*gem_v_parse_expr);
                    gem_table_set(gem_v_args, gem_v_argc, _t269.fn(_t269.env, NULL, 0));
                    gem_v_argc = gem_add(gem_v_argc, gem_int(1));
                }
            }
    GemVal _t270 = (*gem_v_expect);
    GemVal _t271[] = {gem_string(")")};
            (void)(_t270.fn(_t270.env, _t271, 1));
    GemVal _t272 = (*gem_v_peek);
    GemVal _t273 = _t272.fn(_t272.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t273, gem_string("type")), gem_string("do")))) {
    GemVal _t274 = (*gem_v_advance);
                (void)(_t274.fn(_t274.env, NULL, 0));
    GemVal _t275 = gem_table_new();
                GemVal gem_v_bparams = _t275;
    GemVal _t276 = (*gem_v_peek);
    GemVal _t277 = _t276.fn(_t276.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t277, gem_string("type")), gem_string("|")))) {
    GemVal _t278 = (*gem_v_advance);
                    (void)(_t278.fn(_t278.env, NULL, 0));
    GemVal _t279 = (*gem_v_parse_params);
                    gem_v_bparams = _t279.fn(_t279.env, NULL, 0);
    GemVal _t280 = (*gem_v_expect);
    GemVal _t281[] = {gem_string("|")};
                    (void)(_t280.fn(_t280.env, _t281, 1));
                }
    GemVal _t282 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t282.fn(_t282.env, NULL, 0);
    GemVal _t283 = (*gem_v_expect);
    GemVal _t284[] = {gem_string("end")};
                (void)(_t283.fn(_t283.env, _t284, 1));
    GemVal _t285[] = {gem_v_bparams, gem_v_body};
                gem_table_set(gem_v_args, gem_v_argc, gem_fn_make_anon_fn(NULL, _t285, 2));
                gem_v_argc = gem_add(gem_v_argc, gem_int(1));
            } else {
    GemVal _t286 = (*gem_v_peek);
    GemVal _t287 = _t286.fn(_t286.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t287, gem_string("type")), gem_string("{")))) {
                    GemVal gem_v_is_block = gem_bool(0);
    GemVal _t288 = (*gem_v_peek_at);
    GemVal _t289[] = {gem_int(1)};
    GemVal _t290 = _t288.fn(_t288.env, _t289, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t290, gem_string("type")), gem_string("|")))) {
                        gem_v_is_block = gem_bool(1);
                    } else {
    GemVal _t291 = (*gem_v_peek_at);
    GemVal _t292[] = {gem_int(1)};
    GemVal _t293 = _t291.fn(_t291.env, _t292, 1);
    GemVal _t301;
    if (gem_truthy(gem_eq(gem_table_get(_t293, gem_string("type")), gem_string("}")))) {
        _t301 = gem_eq(gem_table_get(_t293, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t294 = (*gem_v_peek_at);
        GemVal _t295[] = {gem_int(1)};
        GemVal _t296 = _t294.fn(_t294.env, _t295, 1);
        GemVal _t300;
        if (!gem_truthy(gem_eq(gem_table_get(_t296, gem_string("type")), gem_string("NAME")))) {
                _t300 = gem_eq(gem_table_get(_t296, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t297 = (*gem_v_peek_at);
                GemVal _t298[] = {gem_int(2)};
                GemVal _t299 = _t297.fn(_t297.env, _t298, 1);
                _t300 = gem_eq(gem_table_get(_t299, gem_string("type")), gem_string(":"));
        }
        _t301 = _t300;
    }
                        if (gem_truthy(_t301)) {
                            gem_v_is_block = gem_bool(0);
                        } else {
                            gem_v_is_block = gem_bool(1);
                        }
                    }
                    if (gem_truthy(gem_v_is_block)) {
    GemVal _t302 = (*gem_v_advance);
                        (void)(_t302.fn(_t302.env, NULL, 0));
    GemVal _t303 = gem_table_new();
                        GemVal gem_v_bparams = _t303;
    GemVal _t304 = (*gem_v_peek);
    GemVal _t305 = _t304.fn(_t304.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t305, gem_string("type")), gem_string("|")))) {
    GemVal _t306 = (*gem_v_advance);
                            (void)(_t306.fn(_t306.env, NULL, 0));
    GemVal _t307 = (*gem_v_parse_params);
                            gem_v_bparams = _t307.fn(_t307.env, NULL, 0);
    GemVal _t308 = (*gem_v_expect);
    GemVal _t309[] = {gem_string("|")};
                            (void)(_t308.fn(_t308.env, _t309, 1));
                        }
    GemVal _t310 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t310.fn(_t310.env, NULL, 0);
    GemVal _t311 = (*gem_v_expect);
    GemVal _t312[] = {gem_string("}")};
                        (void)(_t311.fn(_t311.env, _t312, 1));
    GemVal _t313 = gem_table_new();
    gem_table_set(_t313, gem_int(0), gem_v_bexpr);
    GemVal _t314[] = {gem_v_bparams, _t313};
                        gem_table_set(gem_v_args, gem_v_argc, gem_fn_make_anon_fn(NULL, _t314, 2));
                        gem_v_argc = gem_add(gem_v_argc, gem_int(1));
                    }
                }
            }
    GemVal _t315[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t315, 3);
            continue;
        }
    GemVal _t316 = (*gem_v_peek);
    GemVal _t317 = _t316.fn(_t316.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t317, gem_string("type")), gem_string(".")))) {
    GemVal _t318 = (*gem_v_advance);
            (void)(_t318.fn(_t318.env, NULL, 0));
    GemVal _t319 = (*gem_v_expect);
    GemVal _t320[] = {gem_string("NAME")};
    GemVal _t321 = _t319.fn(_t319.env, _t320, 1);
            GemVal gem_v_field = gem_table_get(_t321, gem_string("value"));
    GemVal _t322[] = {gem_v_node, gem_v_field};
            gem_v_node = gem_fn_make_dot(NULL, _t322, 2);
            continue;
        }
    GemVal _t323 = (*gem_v_peek);
    GemVal _t324 = _t323.fn(_t323.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t324, gem_string("type")), gem_string("[")))) {
    GemVal _t325 = (*gem_v_advance);
            (void)(_t325.fn(_t325.env, NULL, 0));
    GemVal _t326 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t326.fn(_t326.env, NULL, 0);
    GemVal _t327 = (*gem_v_expect);
    GemVal _t328[] = {gem_string("]")};
            (void)(_t327.fn(_t327.env, _t328, 1));
    GemVal _t329[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t329, 2);
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
    GemVal _t331 = (*gem_v_peek);
    GemVal _t332 = _t331.fn(_t331.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t332, gem_string("type")), gem_string("-")))) {
    GemVal _t333 = (*gem_v_advance);
        (void)(_t333.fn(_t333.env, NULL, 0));
    GemVal _t334 = (*gem_v_parse_unary);
    GemVal _t335[] = {gem_string("-"), _t334.fn(_t334.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t335, 2);
    }
    GemVal _t336 = (*gem_v_parse_call);
    return _t336.fn(_t336.env, NULL, 0);
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
    GemVal _t338 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t338.fn(_t338.env, NULL, 0);
    while (1) {
        GemVal _t339 = (*gem_v_peek);
        GemVal _t340 = _t339.fn(_t339.env, NULL, 0);
        GemVal _t343;
        if (gem_truthy(gem_eq(gem_table_get(_t340, gem_string("type")), gem_string("*")))) {
                _t343 = gem_eq(gem_table_get(_t340, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t341 = (*gem_v_peek);
                GemVal _t342 = _t341.fn(_t341.env, NULL, 0);
                _t343 = gem_eq(gem_table_get(_t342, gem_string("type")), gem_string("/"));
        }
        GemVal _t346;
        if (gem_truthy(_t343)) {
                _t346 = _t343;
        } else {
                GemVal _t344 = (*gem_v_peek);
                GemVal _t345 = _t344.fn(_t344.env, NULL, 0);
                _t346 = gem_eq(gem_table_get(_t345, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t346)) break;
    GemVal _t347 = (*gem_v_advance);
    GemVal _t348 = _t347.fn(_t347.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t348, gem_string("type"));
    GemVal _t349 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t349.fn(_t349.env, NULL, 0);
    GemVal _t350[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t350, 3);
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
    GemVal _t352 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t352.fn(_t352.env, NULL, 0);
    while (1) {
        GemVal _t353 = (*gem_v_peek);
        GemVal _t354 = _t353.fn(_t353.env, NULL, 0);
        GemVal _t357;
        if (gem_truthy(gem_eq(gem_table_get(_t354, gem_string("type")), gem_string("+")))) {
                _t357 = gem_eq(gem_table_get(_t354, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t355 = (*gem_v_peek);
                GemVal _t356 = _t355.fn(_t355.env, NULL, 0);
                _t357 = gem_eq(gem_table_get(_t356, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t357)) break;
    GemVal _t358 = (*gem_v_advance);
    GemVal _t359 = _t358.fn(_t358.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t359, gem_string("type"));
    GemVal _t360 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t360.fn(_t360.env, NULL, 0);
    GemVal _t361[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t361, 3);
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
    GemVal _t363 = (*gem_v_parse_add);
    GemVal gem_v_left = _t363.fn(_t363.env, NULL, 0);
    while (1) {
        GemVal _t364 = (*gem_v_peek);
        GemVal _t365 = _t364.fn(_t364.env, NULL, 0);
        GemVal _t368;
        if (gem_truthy(gem_eq(gem_table_get(_t365, gem_string("type")), gem_string("==")))) {
                _t368 = gem_eq(gem_table_get(_t365, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t366 = (*gem_v_peek);
                GemVal _t367 = _t366.fn(_t366.env, NULL, 0);
                _t368 = gem_eq(gem_table_get(_t367, gem_string("type")), gem_string("!="));
        }
        GemVal _t371;
        if (gem_truthy(_t368)) {
                _t371 = _t368;
        } else {
                GemVal _t369 = (*gem_v_peek);
                GemVal _t370 = _t369.fn(_t369.env, NULL, 0);
                _t371 = gem_eq(gem_table_get(_t370, gem_string("type")), gem_string("<"));
        }
        GemVal _t374;
        if (gem_truthy(_t371)) {
                _t374 = _t371;
        } else {
                GemVal _t372 = (*gem_v_peek);
                GemVal _t373 = _t372.fn(_t372.env, NULL, 0);
                _t374 = gem_eq(gem_table_get(_t373, gem_string("type")), gem_string(">"));
        }
        GemVal _t377;
        if (gem_truthy(_t374)) {
                _t377 = _t374;
        } else {
                GemVal _t375 = (*gem_v_peek);
                GemVal _t376 = _t375.fn(_t375.env, NULL, 0);
                _t377 = gem_eq(gem_table_get(_t376, gem_string("type")), gem_string("<="));
        }
        GemVal _t380;
        if (gem_truthy(_t377)) {
                _t380 = _t377;
        } else {
                GemVal _t378 = (*gem_v_peek);
                GemVal _t379 = _t378.fn(_t378.env, NULL, 0);
                _t380 = gem_eq(gem_table_get(_t379, gem_string("type")), gem_string(">="));
        }
        if (!gem_truthy(_t380)) break;
    GemVal _t381 = (*gem_v_advance);
    GemVal _t382 = _t381.fn(_t381.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t382, gem_string("type"));
    GemVal _t383 = (*gem_v_parse_add);
        GemVal gem_v_right = _t383.fn(_t383.env, NULL, 0);
    GemVal _t384[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t384, 3);
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
    GemVal _t386 = (*gem_v_peek);
    GemVal _t387 = _t386.fn(_t386.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t387, gem_string("type")), gem_string("not")))) {
    GemVal _t388 = (*gem_v_advance);
        (void)(_t388.fn(_t388.env, NULL, 0));
    GemVal _t389 = (*gem_v_parse_not);
    GemVal _t390[] = {gem_string("not"), _t389.fn(_t389.env, NULL, 0)};
        return gem_fn_make_unop(NULL, _t390, 2);
    }
    GemVal _t391 = (*gem_v_parse_compare);
    return _t391.fn(_t391.env, NULL, 0);
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
    GemVal _t393 = (*gem_v_parse_not);
    GemVal gem_v_left = _t393.fn(_t393.env, NULL, 0);
    while (1) {
        GemVal _t394 = (*gem_v_peek);
        GemVal _t395 = _t394.fn(_t394.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t395, gem_string("type")), gem_string("and")))) break;
    GemVal _t396 = (*gem_v_advance);
        (void)(_t396.fn(_t396.env, NULL, 0));
    GemVal _t397 = (*gem_v_parse_not);
        GemVal gem_v_right = _t397.fn(_t397.env, NULL, 0);
    GemVal _t398[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t398, 3);
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
    GemVal _t400 = (*gem_v_parse_and);
    GemVal gem_v_left = _t400.fn(_t400.env, NULL, 0);
    while (1) {
        GemVal _t401 = (*gem_v_peek);
        GemVal _t402 = _t401.fn(_t401.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t402, gem_string("type")), gem_string("or")))) break;
    GemVal _t403 = (*gem_v_advance);
        (void)(_t403.fn(_t403.env, NULL, 0));
    GemVal _t404 = (*gem_v_parse_and);
        GemVal gem_v_right = _t404.fn(_t404.env, NULL, 0);
    GemVal _t405[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t405, 3);
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
    GemVal _t407 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t407.fn(_t407.env, NULL, 0);
    GemVal _t408 = (*gem_v_peek);
    GemVal _t409 = _t408.fn(_t408.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t409, gem_string("type")), gem_string("=")))) {
    GemVal _t410 = (*gem_v_advance);
        (void)(_t410.fn(_t410.env, NULL, 0));
    GemVal _t411 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t411.fn(_t411.env, NULL, 0);
    GemVal _t412 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t412, gem_string("tag")), gem_string("var")))) {
    GemVal _t413 = gem_v_lhs;
    GemVal _t414[] = {gem_table_get(_t413, gem_string("name")), gem_v_value};
            return gem_fn_make_assign(NULL, _t414, 2);
        }
    GemVal _t415 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t415, gem_string("tag")), gem_string("dot")))) {
    GemVal _t416 = gem_v_lhs;
    GemVal _t417 = gem_v_lhs;
    GemVal _t418[] = {gem_table_get(_t416, gem_string("object")), gem_table_get(_t417, gem_string("field")), gem_v_value};
            return gem_fn_make_dot_assign(NULL, _t418, 3);
        }
    GemVal _t419 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t419, gem_string("tag")), gem_string("index")))) {
    GemVal _t420 = gem_v_lhs;
    GemVal _t421 = gem_v_lhs;
    GemVal _t422[] = {gem_table_get(_t420, gem_string("object")), gem_table_get(_t421, gem_string("key")), gem_v_value};
            return gem_fn_make_index_assign(NULL, _t422, 3);
        }
    GemVal _t423 = (*gem_v_peek);
    GemVal _t424 = _t423.fn(_t423.env, NULL, 0);
    GemVal _t425[] = {gem_table_get(_t424, gem_string("line"))};
    GemVal _t426[] = {gem_add(gem_string("invalid assignment target at line "), gem_to_string_fn(NULL, _t425, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 398, _t426, 1));
    }
    GemVal _t427 = (*gem_v_peek);
    GemVal _t428 = _t427.fn(_t427.env, NULL, 0);
    GemVal _t431;
    if (gem_truthy(gem_eq(gem_table_get(_t428, gem_string("type")), gem_string("+=")))) {
        _t431 = gem_eq(gem_table_get(_t428, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t429 = (*gem_v_peek);
        GemVal _t430 = _t429.fn(_t429.env, NULL, 0);
        _t431 = gem_eq(gem_table_get(_t430, gem_string("type")), gem_string("-="));
    }
    GemVal _t434;
    if (gem_truthy(_t431)) {
        _t434 = _t431;
    } else {
        GemVal _t432 = (*gem_v_peek);
        GemVal _t433 = _t432.fn(_t432.env, NULL, 0);
        _t434 = gem_eq(gem_table_get(_t433, gem_string("type")), gem_string("*="));
    }
    GemVal _t437;
    if (gem_truthy(_t434)) {
        _t437 = _t434;
    } else {
        GemVal _t435 = (*gem_v_peek);
        GemVal _t436 = _t435.fn(_t435.env, NULL, 0);
        _t437 = gem_eq(gem_table_get(_t436, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t437)) {
    GemVal _t438 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t438.fn(_t438.env, NULL, 0);
    GemVal _t439 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t439, gem_string("value")), gem_int(0));
    GemVal _t440 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t440.fn(_t440.env, NULL, 0);
    GemVal _t441 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t441, gem_string("tag")), gem_string("var")))) {
    GemVal _t442 = gem_v_op_tok;
    GemVal _t443[] = {gem_table_get(_t442, gem_string("line"))};
    GemVal _t444[] = {gem_add(gem_string("compound assignment requires variable target at line "), gem_to_string_fn(NULL, _t443, 1))};
            (void)(gem_error_at_fn("compiler/main.gem", 407, _t444, 1));
        }
    GemVal _t445 = gem_v_lhs;
    GemVal _t446 = gem_v_lhs;
    GemVal _t447[] = {gem_table_get(_t446, gem_string("name"))};
    GemVal _t448[] = {gem_v_base_op, gem_fn_make_var(NULL, _t447, 1), gem_v_value};
    GemVal _t449[] = {gem_table_get(_t445, gem_string("name")), gem_fn_make_binop(NULL, _t448, 3)};
        return gem_fn_make_assign(NULL, _t449, 2);
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
    GemVal _t451 = (*gem_v_peek);
    GemVal gem_v_t = _t451.fn(_t451.env, NULL, 0);
    GemVal _t452 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t452, gem_string("type")), gem_string("let")))) {
    GemVal _t453 = (*gem_v_advance);
        (void)(_t453.fn(_t453.env, NULL, 0));
    GemVal _t454 = (*gem_v_expect);
    GemVal _t455[] = {gem_string("NAME")};
    GemVal _t456 = _t454.fn(_t454.env, _t455, 1);
        GemVal gem_v_name = gem_table_get(_t456, gem_string("value"));
    GemVal _t457 = (*gem_v_expect);
    GemVal _t458[] = {gem_string("=")};
        (void)(_t457.fn(_t457.env, _t458, 1));
    GemVal _t459 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t459.fn(_t459.env, NULL, 0);
    GemVal _t460[] = {gem_v_name, gem_v_value};
        return gem_fn_make_let(NULL, _t460, 2);
    }
    GemVal _t461 = gem_v_t;
    GemVal _t465;
    if (!gem_truthy(gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("fn")))) {
        _t465 = gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t462 = (*gem_v_peek_at);
        GemVal _t463[] = {gem_int(1)};
        GemVal _t464 = _t462.fn(_t462.env, _t463, 1);
        _t465 = gem_eq(gem_table_get(_t464, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t465)) {
    GemVal _t466 = (*gem_v_advance);
        (void)(_t466.fn(_t466.env, NULL, 0));
    GemVal _t467 = (*gem_v_expect);
    GemVal _t468[] = {gem_string("NAME")};
    GemVal _t469 = _t467.fn(_t467.env, _t468, 1);
        GemVal gem_v_name = gem_table_get(_t469, gem_string("value"));
    GemVal _t470 = (*gem_v_expect);
    GemVal _t471[] = {gem_string("(")};
        (void)(_t470.fn(_t470.env, _t471, 1));
    GemVal _t472 = (*gem_v_parse_params);
        GemVal gem_v_params = _t472.fn(_t472.env, NULL, 0);
    GemVal _t473 = (*gem_v_expect);
    GemVal _t474[] = {gem_string(")")};
        (void)(_t473.fn(_t473.env, _t474, 1));
    GemVal _t475 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t475.fn(_t475.env, NULL, 0);
    GemVal _t476 = (*gem_v_expect);
    GemVal _t477[] = {gem_string("end")};
        (void)(_t476.fn(_t476.env, _t477, 1));
    GemVal _t478[] = {gem_v_name, gem_v_params, gem_v_body};
        return gem_fn_make_fn_def(NULL, _t478, 3);
    }
    GemVal _t479 = gem_v_t;
    GemVal _t481;
    if (gem_truthy(gem_eq(gem_table_get(_t479, gem_string("type")), gem_string("if")))) {
        _t481 = gem_eq(gem_table_get(_t479, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t480 = gem_v_t;
        _t481 = gem_eq(gem_table_get(_t480, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t481)) {
    GemVal _t482 = (*gem_v_advance);
        (void)(_t482.fn(_t482.env, NULL, 0));
    GemVal _t483 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t483.fn(_t483.env, NULL, 0);
    GemVal _t484 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t484.fn(_t484.env, NULL, 0);
        GemVal gem_v_else_body = GEM_NIL;
    GemVal _t485 = (*gem_v_peek);
    GemVal _t486 = _t485.fn(_t485.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t486, gem_string("type")), gem_string("elif")))) {
    GemVal _t487 = gem_table_new();
    GemVal _t488 = (*gem_v_parse_stmt);
    gem_table_set(_t487, gem_int(0), _t488.fn(_t488.env, NULL, 0));
            gem_v_else_body = _t487;
        } else {
    GemVal _t489 = (*gem_v_peek);
    GemVal _t490 = _t489.fn(_t489.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t490, gem_string("type")), gem_string("else")))) {
    GemVal _t491 = (*gem_v_advance);
                (void)(_t491.fn(_t491.env, NULL, 0));
    GemVal _t492 = (*gem_v_parse_body);
                gem_v_else_body = _t492.fn(_t492.env, NULL, 0);
            }
        }
    GemVal _t493 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t493, gem_string("type")), gem_string("if")))) {
    GemVal _t494 = (*gem_v_expect);
    GemVal _t495[] = {gem_string("end")};
            (void)(_t494.fn(_t494.env, _t495, 1));
        }
    GemVal _t496[] = {gem_v_cond, gem_v_then_body, gem_v_else_body};
        return gem_fn_make_if(NULL, _t496, 3);
    }
    GemVal _t497 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t497, gem_string("type")), gem_string("while")))) {
    GemVal _t498 = (*gem_v_advance);
        (void)(_t498.fn(_t498.env, NULL, 0));
    GemVal _t499 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t499.fn(_t499.env, NULL, 0);
    GemVal _t500 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t500.fn(_t500.env, NULL, 0);
    GemVal _t501 = (*gem_v_expect);
    GemVal _t502[] = {gem_string("end")};
        (void)(_t501.fn(_t501.env, _t502, 1));
    GemVal _t503[] = {gem_v_cond, gem_v_wbody};
        return gem_fn_make_while(NULL, _t503, 2);
    }
    GemVal _t504 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t504, gem_string("type")), gem_string("for")))) {
    GemVal _t505 = (*gem_v_advance);
        (void)(_t505.fn(_t505.env, NULL, 0));
    GemVal _t506 = (*gem_v_expect);
    GemVal _t507[] = {gem_string("NAME")};
    GemVal _t508 = _t506.fn(_t506.env, _t507, 1);
        GemVal gem_v_var_name = gem_table_get(_t508, gem_string("value"));
    GemVal _t509 = (*gem_v_peek);
    GemVal _t510 = _t509.fn(_t509.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t510, gem_string("type")), gem_string("in")))) {
    GemVal _t511 = (*gem_v_advance);
            (void)(_t511.fn(_t511.env, NULL, 0));
    GemVal _t512 = (*gem_v_parse_expr);
            GemVal gem_v_items_expr = _t512.fn(_t512.env, NULL, 0);
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
    GemVal _t513[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t513, 1));
    GemVal _t514[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t514, 1));
    GemVal _t515 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t515.fn(_t515.env, NULL, 0);
    GemVal _t516 = (*gem_v_expect);
    GemVal _t517[] = {gem_string("end")};
            (void)(_t516.fn(_t516.env, _t517, 1));
    GemVal _t518 = gem_table_new();
            GemVal gem_v_inner_stmts = _t518;
            GemVal gem_v_ic = gem_int(0);
    GemVal _t519[] = {gem_v_items_var};
    GemVal _t520[] = {gem_v_idx_var};
    GemVal _t521[] = {gem_fn_make_var(NULL, _t519, 1), gem_fn_make_var(NULL, _t520, 1)};
    GemVal _t522[] = {gem_v_var_name, gem_fn_make_index(NULL, _t521, 2)};
            gem_table_set(gem_v_inner_stmts, gem_v_ic, gem_fn_make_let(NULL, _t522, 2));
            gem_v_ic = gem_add(gem_v_ic, gem_int(1));
    GemVal _t523[] = {gem_v_idx_var};
    GemVal _t524[] = {gem_int(1)};
    GemVal _t525[] = {gem_string("+"), gem_fn_make_var(NULL, _t523, 1), gem_fn_make_int(NULL, _t524, 1)};
    GemVal _t526[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t525, 3)};
            gem_table_set(gem_v_inner_stmts, gem_v_ic, gem_fn_make_assign(NULL, _t526, 2));
            gem_v_ic = gem_add(gem_v_ic, gem_int(1));
            {
                GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t527[] = {gem_v_fbody};
                GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t527, 1);
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
                    GemVal gem_v_bi = gem_v__for_i_1;
                    gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
                    gem_table_set(gem_v_inner_stmts, gem_v_ic, gem_table_get(gem_v_fbody, gem_v_bi));
                    gem_v_ic = gem_add(gem_v_ic, gem_int(1));
                }
            }
    GemVal _t528[] = {gem_v_idx_var};
    GemVal _t529[] = {gem_string("len")};
    GemVal _t530 = gem_table_new();
    GemVal _t531[] = {gem_v_items_var};
    gem_table_set(_t530, gem_int(0), gem_fn_make_var(NULL, _t531, 1));
    GemVal _t532[] = {gem_fn_make_var(NULL, _t529, 1), _t530, gem_int(0)};
    GemVal _t533[] = {gem_string("<"), gem_fn_make_var(NULL, _t528, 1), gem_fn_make_call(NULL, _t532, 3)};
    GemVal _t534[] = {gem_fn_make_binop(NULL, _t533, 3), gem_v_inner_stmts};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t534, 2);
    GemVal _t535 = gem_table_new();
    gem_table_set(_t535, gem_string("tag"), gem_string("block"));
    GemVal _t536 = gem_table_new();
    GemVal _t537[] = {gem_v_items_var, gem_v_items_expr};
    gem_table_set(_t536, gem_int(0), gem_fn_make_let(NULL, _t537, 2));
    GemVal _t538[] = {gem_int(0)};
    GemVal _t539[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t538, 1)};
    gem_table_set(_t536, gem_int(1), gem_fn_make_let(NULL, _t539, 2));
    gem_table_set(_t536, gem_int(2), gem_v_while_node);
    gem_table_set(_t535, gem_string("stmts"), _t536);
            return _t535;
        } else {
    GemVal _t540 = (*gem_v_peek);
    GemVal _t541 = _t540.fn(_t540.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t541, gem_string("type")), gem_string("=")))) {
    GemVal _t542 = (*gem_v_advance);
                (void)(_t542.fn(_t542.env, NULL, 0));
    GemVal _t543 = (*gem_v_parse_expr);
                GemVal gem_v_start_expr = _t543.fn(_t543.env, NULL, 0);
    GemVal _t544 = (*gem_v_expect);
    GemVal _t545[] = {gem_string(",")};
                (void)(_t544.fn(_t544.env, _t545, 1));
    GemVal _t546 = (*gem_v_parse_expr);
                GemVal gem_v_end_expr = _t546.fn(_t546.env, NULL, 0);
    GemVal _t547 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t547.fn(_t547.env, NULL, 0);
    GemVal _t548 = (*gem_v_expect);
    GemVal _t549[] = {gem_string("end")};
                (void)(_t548.fn(_t548.env, _t549, 1));
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
    GemVal _t550[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t550, 1));
    GemVal _t551[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t551, 1));
    GemVal _t552 = gem_table_new();
                GemVal gem_v_inner_stmts = _t552;
                GemVal gem_v_ic = gem_int(0);
    GemVal _t553[] = {gem_v_idx_var};
    GemVal _t554[] = {gem_v_var_name, gem_fn_make_var(NULL, _t553, 1)};
                gem_table_set(gem_v_inner_stmts, gem_v_ic, gem_fn_make_let(NULL, _t554, 2));
                gem_v_ic = gem_add(gem_v_ic, gem_int(1));
    GemVal _t555[] = {gem_v_idx_var};
    GemVal _t556[] = {gem_int(1)};
    GemVal _t557[] = {gem_string("+"), gem_fn_make_var(NULL, _t555, 1), gem_fn_make_int(NULL, _t556, 1)};
    GemVal _t558[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t557, 3)};
                gem_table_set(gem_v_inner_stmts, gem_v_ic, gem_fn_make_assign(NULL, _t558, 2));
                gem_v_ic = gem_add(gem_v_ic, gem_int(1));
                {
                    GemVal gem_v__for_i_2 = gem_int(0);
    GemVal _t559[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t559, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
                        GemVal gem_v_bi = gem_v__for_i_2;
                        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
                        gem_table_set(gem_v_inner_stmts, gem_v_ic, gem_table_get(gem_v_fbody, gem_v_bi));
                        gem_v_ic = gem_add(gem_v_ic, gem_int(1));
                    }
                }
    GemVal _t560[] = {gem_v_idx_var};
    GemVal _t561[] = {gem_v_limit_var};
    GemVal _t562[] = {gem_string("<"), gem_fn_make_var(NULL, _t560, 1), gem_fn_make_var(NULL, _t561, 1)};
    GemVal _t563[] = {gem_fn_make_binop(NULL, _t562, 3), gem_v_inner_stmts};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t563, 2);
    GemVal _t564 = gem_table_new();
    gem_table_set(_t564, gem_string("tag"), gem_string("block"));
    GemVal _t565 = gem_table_new();
    GemVal _t566[] = {gem_v_idx_var, gem_v_start_expr};
    gem_table_set(_t565, gem_int(0), gem_fn_make_let(NULL, _t566, 2));
    GemVal _t567[] = {gem_v_limit_var, gem_v_end_expr};
    gem_table_set(_t565, gem_int(1), gem_fn_make_let(NULL, _t567, 2));
    gem_table_set(_t565, gem_int(2), gem_v_while_node);
    gem_table_set(_t564, gem_string("stmts"), _t565);
                return _t564;
            } else {
    GemVal _t568 = (*gem_v_peek);
    GemVal _t569 = _t568.fn(_t568.env, NULL, 0);
    GemVal _t570[] = {gem_table_get(_t569, gem_string("line"))};
    GemVal _t571[] = {gem_add(gem_string("expected 'in' or '=' after for variable at line "), gem_to_string_fn(NULL, _t570, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 523, _t571, 1));
            }
        }
    }
    GemVal _t572 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t572, gem_string("type")), gem_string("match")))) {
    GemVal _t573 = (*gem_v_advance);
        (void)(_t573.fn(_t573.env, NULL, 0));
    GemVal _t574 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t574.fn(_t574.env, NULL, 0);
    GemVal _t575 = (*gem_v_skip_nl);
        (void)(_t575.fn(_t575.env, NULL, 0));
    GemVal _t576 = gem_table_new();
        GemVal gem_v_whens = _t576;
        GemVal gem_v_wcnt = gem_int(0);
        while (1) {
            GemVal _t577 = (*gem_v_peek);
            GemVal _t578 = _t577.fn(_t577.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t578, gem_string("type")), gem_string("when")))) break;
    GemVal _t579 = (*gem_v_advance);
            (void)(_t579.fn(_t579.env, NULL, 0));
    GemVal _t580 = (*gem_v_parse_expr);
            GemVal gem_v_wval = _t580.fn(_t580.env, NULL, 0);
    GemVal _t581 = (*gem_v_parse_body);
            GemVal gem_v_wbody = _t581.fn(_t581.env, NULL, 0);
    GemVal _t582[] = {gem_v_wval, gem_v_wbody};
            gem_table_set(gem_v_whens, gem_v_wcnt, gem_fn_make_when(NULL, _t582, 2));
            gem_v_wcnt = gem_add(gem_v_wcnt, gem_int(1));
    GemVal _t583 = (*gem_v_skip_nl);
            (void)(_t583.fn(_t583.env, NULL, 0));
        }
        GemVal gem_v_else_body = GEM_NIL;
    GemVal _t584 = (*gem_v_peek);
    GemVal _t585 = _t584.fn(_t584.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t585, gem_string("type")), gem_string("else")))) {
    GemVal _t586 = (*gem_v_advance);
            (void)(_t586.fn(_t586.env, NULL, 0));
    GemVal _t587 = (*gem_v_parse_body);
            gem_v_else_body = _t587.fn(_t587.env, NULL, 0);
        }
    GemVal _t588 = (*gem_v_expect);
    GemVal _t589[] = {gem_string("end")};
        (void)(_t588.fn(_t588.env, _t589, 1));
    GemVal _t590[] = {gem_v_target, gem_v_whens, gem_v_else_body};
        return gem_fn_make_match(NULL, _t590, 3);
    }
    GemVal _t591 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t591, gem_string("type")), gem_string("return")))) {
    GemVal _t592 = (*gem_v_advance);
        (void)(_t592.fn(_t592.env, NULL, 0));
        GemVal gem_v_value = GEM_NIL;
    GemVal _t593 = (*gem_v_peek);
    GemVal _t594 = _t593.fn(_t593.env, NULL, 0);
    GemVal _t597;
    if (!gem_truthy(gem_neq(gem_table_get(_t594, gem_string("type")), gem_string("NEWLINE")))) {
        _t597 = gem_neq(gem_table_get(_t594, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t595 = (*gem_v_peek);
        GemVal _t596 = _t595.fn(_t595.env, NULL, 0);
        _t597 = gem_neq(gem_table_get(_t596, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t600;
    if (!gem_truthy(_t597)) {
        _t600 = _t597;
    } else {
        GemVal _t598 = (*gem_v_peek);
        GemVal _t599 = _t598.fn(_t598.env, NULL, 0);
        _t600 = gem_neq(gem_table_get(_t599, gem_string("type")), gem_string("end"));
    }
    GemVal _t603;
    if (!gem_truthy(_t600)) {
        _t603 = _t600;
    } else {
        GemVal _t601 = (*gem_v_peek);
        GemVal _t602 = _t601.fn(_t601.env, NULL, 0);
        _t603 = gem_neq(gem_table_get(_t602, gem_string("type")), gem_string("elif"));
    }
    GemVal _t606;
    if (!gem_truthy(_t603)) {
        _t606 = _t603;
    } else {
        GemVal _t604 = (*gem_v_peek);
        GemVal _t605 = _t604.fn(_t604.env, NULL, 0);
        _t606 = gem_neq(gem_table_get(_t605, gem_string("type")), gem_string("else"));
    }
    GemVal _t609;
    if (!gem_truthy(_t606)) {
        _t609 = _t606;
    } else {
        GemVal _t607 = (*gem_v_peek);
        GemVal _t608 = _t607.fn(_t607.env, NULL, 0);
        _t609 = gem_neq(gem_table_get(_t608, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t609)) {
    GemVal _t610 = (*gem_v_parse_expr);
            gem_v_value = _t610.fn(_t610.env, NULL, 0);
        }
    GemVal _t611[] = {gem_v_value};
        return gem_fn_make_return(NULL, _t611, 1);
    }
    GemVal _t612 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t612, gem_string("type")), gem_string("break")))) {
    GemVal _t613 = (*gem_v_advance);
        (void)(_t613.fn(_t613.env, NULL, 0));
        return gem_fn_make_break(NULL, NULL, 0);
    }
    GemVal _t614 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t614, gem_string("type")), gem_string("continue")))) {
    GemVal _t615 = (*gem_v_advance);
        (void)(_t615.fn(_t615.env, NULL, 0));
        return gem_fn_make_continue(NULL, NULL, 0);
    }
    GemVal _t616 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t616, gem_string("type")), gem_string("load")))) {
    GemVal _t617 = (*gem_v_advance);
        (void)(_t617.fn(_t617.env, NULL, 0));
    GemVal _t618 = (*gem_v_expect);
    GemVal _t619[] = {gem_string("STRING")};
    GemVal _t620 = _t618.fn(_t618.env, _t619, 1);
        GemVal gem_v_path = gem_table_get(_t620, gem_string("value"));
    GemVal _t621[] = {gem_v_path};
        return gem_fn_make_load(NULL, _t621, 1);
    }
    GemVal _t622 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t622, gem_string("type")), gem_string("extern")))) {
    GemVal _t623 = (*gem_v_advance);
        (void)(_t623.fn(_t623.env, NULL, 0));
    GemVal _t624 = (*gem_v_peek);
    GemVal _t625 = _t624.fn(_t624.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t625, gem_string("type")), gem_string("fn")))) {
    GemVal _t626 = (*gem_v_advance);
            (void)(_t626.fn(_t626.env, NULL, 0));
    GemVal _t627 = (*gem_v_expect);
    GemVal _t628[] = {gem_string("NAME")};
    GemVal _t629 = _t627.fn(_t627.env, _t628, 1);
            GemVal gem_v_name = gem_table_get(_t629, gem_string("value"));
    GemVal _t630 = (*gem_v_expect);
    GemVal _t631[] = {gem_string("(")};
            (void)(_t630.fn(_t630.env, _t631, 1));
    GemVal _t632 = gem_table_new();
            GemVal gem_v_eparams = _t632;
            GemVal gem_v_pcnt = gem_int(0);
    GemVal _t633 = (*gem_v_peek);
    GemVal _t634 = _t633.fn(_t633.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t634, gem_string("type")), gem_string(")")))) {
    GemVal _t635 = (*gem_v_expect);
    GemVal _t636[] = {gem_string("NAME")};
    GemVal _t637 = _t635.fn(_t635.env, _t636, 1);
                GemVal gem_v_pname = gem_table_get(_t637, gem_string("value"));
    GemVal _t638 = (*gem_v_expect);
    GemVal _t639[] = {gem_string(":")};
                (void)(_t638.fn(_t638.env, _t639, 1));
    GemVal _t640 = (*gem_v_expect);
    GemVal _t641[] = {gem_string("NAME")};
    GemVal _t642 = _t640.fn(_t640.env, _t641, 1);
                GemVal gem_v_ptype = gem_table_get(_t642, gem_string("value"));
    GemVal _t643[] = {gem_v_pname, gem_v_ptype};
                gem_table_set(gem_v_eparams, gem_int(0), gem_fn_make_extern_param(NULL, _t643, 2));
                gem_v_pcnt = gem_int(1);
                while (1) {
                    GemVal _t644 = (*gem_v_peek);
                    GemVal _t645 = _t644.fn(_t644.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t645, gem_string("type")), gem_string(",")))) break;
    GemVal _t646 = (*gem_v_advance);
                    (void)(_t646.fn(_t646.env, NULL, 0));
    GemVal _t647 = (*gem_v_expect);
    GemVal _t648[] = {gem_string("NAME")};
    GemVal _t649 = _t647.fn(_t647.env, _t648, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t649, gem_string("value"));
    GemVal _t650 = (*gem_v_expect);
    GemVal _t651[] = {gem_string(":")};
                    (void)(_t650.fn(_t650.env, _t651, 1));
    GemVal _t652 = (*gem_v_expect);
    GemVal _t653[] = {gem_string("NAME")};
    GemVal _t654 = _t652.fn(_t652.env, _t653, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t654, gem_string("value"));
    GemVal _t655[] = {gem_v_pn2, gem_v_pt2};
                    gem_table_set(gem_v_eparams, gem_v_pcnt, gem_fn_make_extern_param(NULL, _t655, 2));
                    gem_v_pcnt = gem_add(gem_v_pcnt, gem_int(1));
                }
            }
    GemVal _t656 = (*gem_v_expect);
    GemVal _t657[] = {gem_string(")")};
            (void)(_t656.fn(_t656.env, _t657, 1));
            GemVal gem_v_ret_type = gem_string("Nil");
    GemVal _t658 = (*gem_v_peek);
    GemVal _t659 = _t658.fn(_t658.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t659, gem_string("type")), gem_string("->")))) {
    GemVal _t660 = (*gem_v_advance);
                (void)(_t660.fn(_t660.env, NULL, 0));
    GemVal _t661 = (*gem_v_expect);
    GemVal _t662[] = {gem_string("NAME")};
    GemVal _t663 = _t661.fn(_t661.env, _t662, 1);
                gem_v_ret_type = gem_table_get(_t663, gem_string("value"));
            }
    GemVal _t664[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            return gem_fn_make_extern_fn(NULL, _t664, 3);
        } else {
    GemVal _t665 = (*gem_v_peek);
    GemVal _t666 = _t665.fn(_t665.env, NULL, 0);
    GemVal _t669;
    if (!gem_truthy(gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("NAME")))) {
        _t669 = gem_eq(gem_table_get(_t666, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t667 = (*gem_v_peek);
        GemVal _t668 = _t667.fn(_t667.env, NULL, 0);
        _t669 = gem_eq(gem_table_get(_t668, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t669)) {
    GemVal _t670 = (*gem_v_advance);
                (void)(_t670.fn(_t670.env, NULL, 0));
    GemVal _t671 = (*gem_v_expect);
    GemVal _t672[] = {gem_string("STRING")};
    GemVal _t673 = _t671.fn(_t671.env, _t672, 1);
                GemVal gem_v_path = gem_table_get(_t673, gem_string("value"));
    GemVal _t674[] = {gem_v_path};
                return gem_fn_make_extern_include(NULL, _t674, 1);
            } else {
    GemVal _t675 = (*gem_v_peek);
    GemVal _t676 = _t675.fn(_t675.env, NULL, 0);
    GemVal _t677[] = {gem_table_get(_t676, gem_string("line"))};
    GemVal _t678[] = {gem_add(gem_string("expected 'fn' or 'include' after 'extern' at line "), gem_to_string_fn(NULL, _t677, 1))};
                (void)(gem_error_at_fn("compiler/main.gem", 616, _t678, 1));
            }
        }
    }
    GemVal _t679 = (*gem_v_parse_expr);
    return _t679.fn(_t679.env, NULL, 0);
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
    GemVal _t681 = gem_table_new();
    GemVal gem_v_stmts = _t681;
    GemVal gem_v_cnt = gem_int(0);
    GemVal _t682 = (*gem_v_skip_nl);
    (void)(_t682.fn(_t682.env, NULL, 0));
    while (1) {
        GemVal _t683 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t683.fn(_t683.env, NULL, 0)))) break;
    GemVal _t684 = (*gem_v_parse_stmt);
        gem_table_set(gem_v_stmts, gem_v_cnt, _t684.fn(_t684.env, NULL, 0));
        gem_v_cnt = gem_add(gem_v_cnt, gem_int(1));
    GemVal _t685 = (*gem_v_skip_nl);
        (void)(_t685.fn(_t685.env, NULL, 0));
    }
    GemVal _t686[] = {gem_v_stmts};
    return gem_fn_make_program(NULL, _t686, 1);
}

static GemVal gem_fn_make_parser(void *_env, GemVal *args, int argc) {
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
    struct _closure__anon_1 *_t120 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t120->gem_v_pos = gem_v_pos;
    _t120->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t120);
    struct _closure__anon_2 *_t121 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t121->gem_v_pos = gem_v_pos;
    _t121->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t121);
    struct _closure__anon_3 *_t122 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t122->gem_v_pos = gem_v_pos;
    _t122->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t122);
    struct _closure__anon_4 *_t124 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t124->gem_v_pos = gem_v_pos;
    _t124->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t124);
    struct _closure__anon_5 *_t130 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t130->gem_v_pos = gem_v_pos;
    _t130->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t130);
    struct _closure__anon_6 *_t132 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t132->gem_v_pos = gem_v_pos;
    _t132->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t132);
    struct _closure__anon_7 *_t144 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t144->gem_v_advance = gem_v_advance;
    _t144->gem_v_expect = gem_v_expect;
    _t144->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t144);
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
    struct _closure__anon_8 *_t162 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t162->gem_v_at_end = gem_v_at_end;
    _t162->gem_v_parse_stmt = gem_v_parse_stmt;
    _t162->gem_v_peek = gem_v_peek;
    _t162->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t162);
    struct _closure__anon_9 *_t171 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t171->gem_v_at_end = gem_v_at_end;
    _t171->gem_v_parse_stmt = gem_v_parse_stmt;
    _t171->gem_v_peek = gem_v_peek;
    _t171->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t171);
    struct _closure__anon_10 *_t255 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t255->gem_v_advance = gem_v_advance;
    _t255->gem_v_expect = gem_v_expect;
    _t255->gem_v_parse_expr = gem_v_parse_expr;
    _t255->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t255->gem_v_parse_params = gem_v_parse_params;
    _t255->gem_v_peek = gem_v_peek;
    _t255->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t255);
    struct _closure__anon_11 *_t330 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t330->gem_v_advance = gem_v_advance;
    _t330->gem_v_expect = gem_v_expect;
    _t330->gem_v_parse_atom = gem_v_parse_atom;
    _t330->gem_v_parse_expr = gem_v_parse_expr;
    _t330->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t330->gem_v_parse_params = gem_v_parse_params;
    _t330->gem_v_peek = gem_v_peek;
    _t330->gem_v_peek_at = gem_v_peek_at;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t330);
    struct _closure__anon_12 *_t337 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t337->gem_v_advance = gem_v_advance;
    _t337->gem_v_parse_call = gem_v_parse_call;
    _t337->gem_v_parse_unary = gem_v_parse_unary;
    _t337->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t337);
    struct _closure__anon_13 *_t351 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t351->gem_v_advance = gem_v_advance;
    _t351->gem_v_parse_unary = gem_v_parse_unary;
    _t351->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t351);
    struct _closure__anon_14 *_t362 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t362->gem_v_advance = gem_v_advance;
    _t362->gem_v_parse_mul = gem_v_parse_mul;
    _t362->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t362);
    struct _closure__anon_15 *_t385 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t385->gem_v_advance = gem_v_advance;
    _t385->gem_v_parse_add = gem_v_parse_add;
    _t385->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t385);
    struct _closure__anon_16 *_t392 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t392->gem_v_advance = gem_v_advance;
    _t392->gem_v_parse_compare = gem_v_parse_compare;
    _t392->gem_v_parse_not = gem_v_parse_not;
    _t392->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t392);
    struct _closure__anon_17 *_t399 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t399->gem_v_advance = gem_v_advance;
    _t399->gem_v_parse_not = gem_v_parse_not;
    _t399->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t399);
    struct _closure__anon_18 *_t406 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t406->gem_v_advance = gem_v_advance;
    _t406->gem_v_parse_and = gem_v_parse_and;
    _t406->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t406);
    struct _closure__anon_19 *_t450 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t450->gem_v_advance = gem_v_advance;
    _t450->gem_v_parse_expr = gem_v_parse_expr;
    _t450->gem_v_parse_or = gem_v_parse_or;
    _t450->gem_v_peek = gem_v_peek;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t450);
    *gem_v_parse_expr = gem_v_parse_assign_fn;
    struct _closure__anon_20 *_t680 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t680->gem_v_advance = gem_v_advance;
    _t680->gem_v_expect = gem_v_expect;
    _t680->gem_v_gensym_counter = gem_v_gensym_counter;
    _t680->gem_v_parse_body = gem_v_parse_body;
    _t680->gem_v_parse_expr = gem_v_parse_expr;
    _t680->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t680->gem_v_parse_params = gem_v_parse_params;
    _t680->gem_v_parse_stmt = gem_v_parse_stmt;
    _t680->gem_v_peek = gem_v_peek;
    _t680->gem_v_peek_at = gem_v_peek_at;
    _t680->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_20, _t680);
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
    struct _closure__anon_21 *_t687 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t687->gem_v_at_end = gem_v_at_end;
    _t687->gem_v_parse_stmt = gem_v_parse_stmt;
    _t687->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_21, _t687);
    GemVal _t688 = gem_table_new();
    gem_table_set(_t688, gem_string("parse"), gem_v_parse);
    return _t688;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    {
        GemVal gem_v__for_i_1 = gem_int(0);
    GemVal _t689[] = {gem_v_s};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t689, 1);
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
    GemVal _t690[] = {gem_string("    "), gem_v_indent};
    return gem_fn_repeat_str(NULL, _t690, 2);
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t691[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t691, 1);
    GemVal gem_v_has_dot = gem_bool(0);
    {
        GemVal gem_v__for_i_3 = gem_int(0);
    GemVal _t692[] = {gem_v_s};
        GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t692, 1);
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
    GemVal _t693[] = {gem_v_arr};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t693, 1);
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
    GemVal _t694 = gem_table_new();
    GemVal gem_v_result = _t694;
    GemVal _t695[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t695, 1);
    GemVal gem_v_i = gem_int(0);
    while (1) {
        GemVal _t696[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t696, 1)))) break;
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t697[] = {gem_v_b};
    GemVal gem_v_ks2 = gem_fn_keys(NULL, _t697, 1);
    GemVal gem_v_j = gem_int(0);
    while (1) {
        GemVal _t698[] = {gem_v_ks2};
        if (!gem_truthy(gem_lt(gem_v_j, gem_len_fn(NULL, _t698, 1)))) break;
        gem_table_set(gem_v_result, gem_table_get(gem_v_ks2, gem_v_j), gem_bool(1));
        gem_v_j = gem_add(gem_v_j, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t699 = gem_table_new();
    GemVal gem_v_result = _t699;
    GemVal _t700[] = {gem_v_a};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t700, 1);
    GemVal gem_v_i = gem_int(0);
    while (1) {
        GemVal _t701[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t701, 1)))) break;
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_table_get(gem_v_ks, gem_v_i)), gem_bool(1)))) {
            gem_table_set(gem_v_result, gem_table_get(gem_v_ks, gem_v_i), gem_bool(1));
        }
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    return gem_v_result;
}

static GemVal gem_fn_set_to_sorted_array(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t702[] = {gem_v_s};
    GemVal gem_v_ks = gem_fn_keys(NULL, _t702, 1);
    GemVal gem_v_i = gem_int(1);
    while (1) {
        GemVal _t703[] = {gem_v_ks};
        if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t703, 1)))) break;
        GemVal gem_v_key = gem_table_get(gem_v_ks, gem_v_i);
        GemVal gem_v_j = gem_sub(gem_v_i, gem_int(1));
        while (1) {
            GemVal _t704;
            if (!gem_truthy(gem_ge(gem_v_j, gem_int(0)))) {
                        _t704 = gem_ge(gem_v_j, gem_int(0));
            } else {
                        _t704 = gem_gt(gem_table_get(gem_v_ks, gem_v_j), gem_v_key);
            }
            if (!gem_truthy(_t704)) break;
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
    GemVal _t705 = gem_table_new();
    GemVal gem_v_s = _t705;
    {
        GemVal gem_v__for_items_5 = gem_v_arr;
        GemVal gem_v__for_i_5 = gem_int(0);
        while (1) {
            GemVal _t706[] = {gem_v__for_items_5};
            if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t706, 1)))) break;
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
    GemVal _t712[] = {(*gem_v_tmp_counter)};
    return gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t712, 1));
}

struct _closure__anon_23 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t714[] = {(*gem_v_anon_counter)};
    return gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t714, 1));
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
    GemVal _t716[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t716, 1), gem_string("table")))) {
        return GEM_NIL;
    }
    GemVal _t717 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t717, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
        return GEM_NIL;
    }
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
    GemVal _t718 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t718, gem_string("name"));
    GemVal _t719[] = {gem_v_defined, gem_v_name};
    GemVal _t721;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t719, 2)))) {
        _t721 = gem_not(gem_fn_set_contains(NULL, _t719, 2));
    } else {
        GemVal _t720[] = {(*gem_v_builtins), gem_v_name};
        _t721 = gem_not(gem_fn_set_contains(NULL, _t720, 2));
    }
    GemVal _t723;
    if (!gem_truthy(_t721)) {
        _t723 = _t721;
    } else {
        GemVal _t722[] = {(*gem_v_defined_fns), gem_v_name};
        _t723 = gem_not(gem_fn_set_contains(NULL, _t722, 2));
    }
        if (gem_truthy(_t723)) {
    GemVal _t724[] = {gem_v_free, gem_v_name};
            return gem_fn_set_add(NULL, _t724, 2);
        } else {
            return GEM_NIL;
        }
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t725 = gem_v_node;
    GemVal _t726 = (*gem_v_collect_free_node);
    GemVal _t727[] = {gem_table_get(_t725, gem_string("value")), gem_v_defined, gem_v_free};
            return _t726.fn(_t726.env, _t727, 3);
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t728 = gem_v_node;
                GemVal gem_v_name = gem_table_get(_t728, gem_string("name"));
    GemVal _t729[] = {gem_v_defined, gem_v_name};
    GemVal _t731;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t729, 2)))) {
        _t731 = gem_not(gem_fn_set_contains(NULL, _t729, 2));
    } else {
        GemVal _t730[] = {(*gem_v_builtins), gem_v_name};
        _t731 = gem_not(gem_fn_set_contains(NULL, _t730, 2));
    }
    GemVal _t733;
    if (!gem_truthy(_t731)) {
        _t733 = _t731;
    } else {
        GemVal _t732[] = {(*gem_v_defined_fns), gem_v_name};
        _t733 = gem_not(gem_fn_set_contains(NULL, _t732, 2));
    }
                if (gem_truthy(_t733)) {
    GemVal _t734[] = {gem_v_free, gem_v_name};
                    (void)(gem_fn_set_add(NULL, _t734, 2));
                }
    GemVal _t735 = gem_v_node;
    GemVal _t736 = (*gem_v_collect_free_node);
    GemVal _t737[] = {gem_table_get(_t735, gem_string("value")), gem_v_defined, gem_v_free};
                return _t736.fn(_t736.env, _t737, 3);
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t738 = gem_v_node;
    GemVal _t739[] = {gem_table_get(_t738, gem_string("params"))};
    GemVal _t740[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t739, 1)};
                    GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t740, 2);
    GemVal _t741 = gem_v_node;
    GemVal _t742 = (*gem_v_collect_free_vars);
    GemVal _t743[] = {gem_table_get(_t741, gem_string("body")), gem_v_inner_defined};
                    GemVal gem_v_inner_free = _t742.fn(_t742.env, _t743, 2);
    GemVal _t744[] = {gem_v_inner_free};
                    GemVal gem_v_ifks = gem_fn_keys(NULL, _t744, 1);
                    GemVal gem_v_ifi = gem_int(0);
                    while (1) {
                        GemVal _t745[] = {gem_v_ifks};
                        if (!gem_truthy(gem_lt(gem_v_ifi, gem_len_fn(NULL, _t745, 1)))) break;
    GemVal _t746[] = {gem_v_free, gem_table_get(gem_v_ifks, gem_v_ifi)};
                        (void)(gem_fn_set_add(NULL, _t746, 2));
                        gem_v_ifi = gem_add(gem_v_ifi, gem_int(1));
                    }
                    return GEM_NIL;
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                        return GEM_NIL;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t747 = gem_v_node;
    GemVal _t748 = (*gem_v_collect_free_node);
    GemVal _t749[] = {gem_table_get(_t747, gem_string("cond")), gem_v_defined, gem_v_free};
                            (void)(_t748.fn(_t748.env, _t749, 3));
    GemVal _t750 = gem_table_new();
    GemVal _t751[] = {gem_v_defined, _t750};
                            GemVal gem_v_d = gem_fn_set_union(NULL, _t751, 2);
                            GemVal gem_v_thens = gem_table_get(gem_v_node, gem_string("then"));
                            {
                                GemVal gem_v__for_i_6 = gem_int(0);
    GemVal _t752[] = {gem_v_thens};
                                GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t752, 1);
                                while (1) {
                                    if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
                                    GemVal gem_v_i = gem_v__for_i_6;
                                    gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
    GemVal _t753 = (*gem_v_collect_free_node);
    GemVal _t754[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
                                    (void)(_t753.fn(_t753.env, _t754, 3));
    GemVal _t755[] = {gem_table_get(gem_v_thens, gem_v_i)};
    GemVal _t757;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t755, 1), gem_string("table")))) {
        _t757 = gem_eq(gem_type_fn(NULL, _t755, 1), gem_string("table"));
    } else {
        GemVal _t756 = gem_table_get(gem_v_thens, gem_v_i);
        _t757 = gem_eq(gem_table_get(_t756, gem_string("tag")), gem_string("let"));
    }
                                    if (gem_truthy(_t757)) {
    GemVal _t758 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t759[] = {gem_v_d, gem_table_get(_t758, gem_string("name"))};
                                        (void)(gem_fn_set_add(NULL, _t759, 2));
                                    }
                                }
                            }
                            GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
                            if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
    GemVal _t760 = gem_table_new();
    GemVal _t761[] = {gem_v_defined, _t760};
                                gem_v_d = gem_fn_set_union(NULL, _t761, 2);
                                {
                                    GemVal gem_v__for_i_7 = gem_int(0);
    GemVal _t762[] = {gem_v_el};
                                    GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t762, 1);
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
                                        GemVal gem_v_i = gem_v__for_i_7;
                                        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
    GemVal _t763 = (*gem_v_collect_free_node);
    GemVal _t764[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t763.fn(_t763.env, _t764, 3));
    GemVal _t765[] = {gem_table_get(gem_v_el, gem_v_i)};
    GemVal _t767;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t765, 1), gem_string("table")))) {
        _t767 = gem_eq(gem_type_fn(NULL, _t765, 1), gem_string("table"));
    } else {
        GemVal _t766 = gem_table_get(gem_v_el, gem_v_i);
        _t767 = gem_eq(gem_table_get(_t766, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t767)) {
    GemVal _t768 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t769[] = {gem_v_d, gem_table_get(_t768, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t769, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                return GEM_NIL;
                            }
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t770 = gem_v_node;
    GemVal _t771 = (*gem_v_collect_free_node);
    GemVal _t772[] = {gem_table_get(_t770, gem_string("cond")), gem_v_defined, gem_v_free};
                                (void)(_t771.fn(_t771.env, _t772, 3));
    GemVal _t773 = gem_table_new();
    GemVal _t774[] = {gem_v_defined, _t773};
                                GemVal gem_v_d = gem_fn_set_union(NULL, _t774, 2);
                                {
                                    GemVal gem_v__for_i_8 = gem_int(0);
    GemVal _t775 = gem_v_node;
    GemVal _t776[] = {gem_table_get(_t775, gem_string("body"))};
                                    GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t776, 1);
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
                                        GemVal gem_v_i = gem_v__for_i_8;
                                        gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
    GemVal _t777 = gem_v_node;
    GemVal _t778 = (*gem_v_collect_free_node);
    GemVal _t779[] = {gem_table_get(gem_table_get(_t777, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                        (void)(_t778.fn(_t778.env, _t779, 3));
    GemVal _t780 = gem_v_node;
    GemVal _t781[] = {gem_table_get(gem_table_get(_t780, gem_string("body")), gem_v_i)};
    GemVal _t784;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t781, 1), gem_string("table")))) {
        _t784 = gem_eq(gem_type_fn(NULL, _t781, 1), gem_string("table"));
    } else {
        GemVal _t782 = gem_v_node;
        GemVal _t783 = gem_table_get(gem_table_get(_t782, gem_string("body")), gem_v_i);
        _t784 = gem_eq(gem_table_get(_t783, gem_string("tag")), gem_string("let"));
    }
                                        if (gem_truthy(_t784)) {
    GemVal _t785 = gem_v_node;
    GemVal _t786 = gem_table_get(gem_table_get(_t785, gem_string("body")), gem_v_i);
    GemVal _t787[] = {gem_v_d, gem_table_get(_t786, gem_string("name"))};
                                            (void)(gem_fn_set_add(NULL, _t787, 2));
                                        }
                                    }
                                    return GEM_NIL;
                                }
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t788 = gem_v_node;
    GemVal _t789 = (*gem_v_collect_free_node);
    GemVal _t790[] = {gem_table_get(_t788, gem_string("target")), gem_v_defined, gem_v_free};
                                    (void)(_t789.fn(_t789.env, _t790, 3));
                                    {
                                        GemVal gem_v__for_i_10 = gem_int(0);
    GemVal _t791 = gem_v_node;
    GemVal _t792[] = {gem_table_get(_t791, gem_string("whens"))};
                                        GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t792, 1);
                                        while (1) {
                                            if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
                                            GemVal gem_v_i = gem_v__for_i_10;
                                            gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
    GemVal _t793 = gem_v_node;
                                            GemVal gem_v_w = gem_table_get(gem_table_get(_t793, gem_string("whens")), gem_v_i);
    GemVal _t794 = gem_v_w;
    GemVal _t795 = (*gem_v_collect_free_node);
    GemVal _t796[] = {gem_table_get(_t794, gem_string("value")), gem_v_defined, gem_v_free};
                                            (void)(_t795.fn(_t795.env, _t796, 3));
    GemVal _t797 = gem_table_new();
    GemVal _t798[] = {gem_v_defined, _t797};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t798, 2);
                                            {
                                                GemVal gem_v__for_i_9 = gem_int(0);
    GemVal _t799 = gem_v_w;
    GemVal _t800[] = {gem_table_get(_t799, gem_string("body"))};
                                                GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t800, 1);
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
                                                    GemVal gem_v_j = gem_v__for_i_9;
                                                    gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
    GemVal _t801 = gem_v_w;
    GemVal _t802 = (*gem_v_collect_free_node);
    GemVal _t803[] = {gem_table_get(gem_table_get(_t801, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
                                                    (void)(_t802.fn(_t802.env, _t803, 3));
    GemVal _t804 = gem_v_w;
    GemVal _t805[] = {gem_table_get(gem_table_get(_t804, gem_string("body")), gem_v_j)};
    GemVal _t808;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t805, 1), gem_string("table")))) {
        _t808 = gem_eq(gem_type_fn(NULL, _t805, 1), gem_string("table"));
    } else {
        GemVal _t806 = gem_v_w;
        GemVal _t807 = gem_table_get(gem_table_get(_t806, gem_string("body")), gem_v_j);
        _t808 = gem_eq(gem_table_get(_t807, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t808)) {
    GemVal _t809 = gem_v_w;
    GemVal _t810 = gem_table_get(gem_table_get(_t809, gem_string("body")), gem_v_j);
    GemVal _t811[] = {gem_v_d, gem_table_get(_t810, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t811, 2));
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    GemVal gem_v_el = gem_table_get(gem_v_node, gem_string("else"));
                                    if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
    GemVal _t812 = gem_table_new();
    GemVal _t813[] = {gem_v_defined, _t812};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t813, 2);
                                        {
                                            GemVal gem_v__for_i_11 = gem_int(0);
    GemVal _t814[] = {gem_v_el};
                                            GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t814, 1);
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
                                                GemVal gem_v_j = gem_v__for_i_11;
                                                gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
    GemVal _t815 = (*gem_v_collect_free_node);
    GemVal _t816[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
                                                (void)(_t815.fn(_t815.env, _t816, 3));
    GemVal _t817[] = {gem_table_get(gem_v_el, gem_v_j)};
    GemVal _t819;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t817, 1), gem_string("table")))) {
        _t819 = gem_eq(gem_type_fn(NULL, _t817, 1), gem_string("table"));
    } else {
        GemVal _t818 = gem_table_get(gem_v_el, gem_v_j);
        _t819 = gem_eq(gem_table_get(_t818, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t819)) {
    GemVal _t820 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t821[] = {gem_v_d, gem_table_get(_t820, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t821, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        return GEM_NIL;
                                    }
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("when_clause")))) {
    GemVal _t822 = gem_v_node;
    GemVal _t823 = (*gem_v_collect_free_node);
    GemVal _t824[] = {gem_table_get(_t822, gem_string("value")), gem_v_defined, gem_v_free};
                                        (void)(_t823.fn(_t823.env, _t824, 3));
    GemVal _t825 = gem_table_new();
    GemVal _t826[] = {gem_v_defined, _t825};
                                        GemVal gem_v_d = gem_fn_set_union(NULL, _t826, 2);
                                        {
                                            GemVal gem_v__for_i_12 = gem_int(0);
    GemVal _t827 = gem_v_node;
    GemVal _t828[] = {gem_table_get(_t827, gem_string("body"))};
                                            GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t828, 1);
                                            while (1) {
                                                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
                                                GemVal gem_v_i = gem_v__for_i_12;
                                                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
    GemVal _t829 = gem_v_node;
    GemVal _t830 = (*gem_v_collect_free_node);
    GemVal _t831[] = {gem_table_get(gem_table_get(_t829, gem_string("body")), gem_v_i), gem_v_d, gem_v_free};
                                                (void)(_t830.fn(_t830.env, _t831, 3));
    GemVal _t832 = gem_v_node;
    GemVal _t833[] = {gem_table_get(gem_table_get(_t832, gem_string("body")), gem_v_i)};
    GemVal _t836;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t833, 1), gem_string("table")))) {
        _t836 = gem_eq(gem_type_fn(NULL, _t833, 1), gem_string("table"));
    } else {
        GemVal _t834 = gem_v_node;
        GemVal _t835 = gem_table_get(gem_table_get(_t834, gem_string("body")), gem_v_i);
        _t836 = gem_eq(gem_table_get(_t835, gem_string("tag")), gem_string("let"));
    }
                                                if (gem_truthy(_t836)) {
    GemVal _t837 = gem_v_node;
    GemVal _t838 = gem_table_get(gem_table_get(_t837, gem_string("body")), gem_v_i);
    GemVal _t839[] = {gem_v_d, gem_table_get(_t838, gem_string("name"))};
                                                    (void)(gem_fn_set_add(NULL, _t839, 2));
                                                }
                                            }
                                            return GEM_NIL;
                                        }
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t840 = gem_table_new();
    GemVal _t841[] = {gem_v_defined, _t840};
                                            GemVal gem_v_d = gem_fn_set_union(NULL, _t841, 2);
                                            {
                                                GemVal gem_v__for_i_13 = gem_int(0);
    GemVal _t842 = gem_v_node;
    GemVal _t843[] = {gem_table_get(_t842, gem_string("stmts"))};
                                                GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t843, 1);
                                                while (1) {
                                                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
                                                    GemVal gem_v_i = gem_v__for_i_13;
                                                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
    GemVal _t844 = gem_v_node;
    GemVal _t845 = (*gem_v_collect_free_node);
    GemVal _t846[] = {gem_table_get(gem_table_get(_t844, gem_string("stmts")), gem_v_i), gem_v_d, gem_v_free};
                                                    (void)(_t845.fn(_t845.env, _t846, 3));
    GemVal _t847 = gem_v_node;
    GemVal _t848[] = {gem_table_get(gem_table_get(_t847, gem_string("stmts")), gem_v_i)};
    GemVal _t851;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t848, 1), gem_string("table")))) {
        _t851 = gem_eq(gem_type_fn(NULL, _t848, 1), gem_string("table"));
    } else {
        GemVal _t849 = gem_v_node;
        GemVal _t850 = gem_table_get(gem_table_get(_t849, gem_string("stmts")), gem_v_i);
        _t851 = gem_eq(gem_table_get(_t850, gem_string("tag")), gem_string("let"));
    }
                                                    if (gem_truthy(_t851)) {
    GemVal _t852 = gem_v_node;
    GemVal _t853 = gem_table_get(gem_table_get(_t852, gem_string("stmts")), gem_v_i);
    GemVal _t854[] = {gem_v_d, gem_table_get(_t853, gem_string("name"))};
                                                        (void)(gem_fn_set_add(NULL, _t854, 2));
                                                    }
                                                }
                                                return GEM_NIL;
                                            }
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t855 = gem_v_node;
                                                if (gem_truthy(gem_neq(gem_table_get(_t855, gem_string("value")), GEM_NIL))) {
    GemVal _t856 = gem_v_node;
    GemVal _t857 = (*gem_v_collect_free_node);
    GemVal _t858[] = {gem_table_get(_t856, gem_string("value")), gem_v_defined, gem_v_free};
                                                    return _t857.fn(_t857.env, _t858, 3);
                                                } else {
                                                    return GEM_NIL;
                                                }
                                            } else {
    GemVal _t859[] = {gem_v_node};
                                                GemVal gem_v_nks = gem_fn_keys(NULL, _t859, 1);
                                                GemVal gem_v_nki = gem_int(0);
                                                while (1) {
                                                    GemVal _t860[] = {gem_v_nks};
                                                    if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t860, 1)))) break;
                                                    if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
                                                        GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
    GemVal _t861[] = {gem_v_v};
                                                        if (gem_truthy(gem_eq(gem_type_fn(NULL, _t861, 1), gem_string("table")))) {
    GemVal _t862[] = {gem_v_v};
                                                            GemVal gem_v_vks = gem_fn_keys(NULL, _t862, 1);
    GemVal _t863[] = {gem_v_vks};
    GemVal _t865;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t863, 1), gem_int(0)))) {
        _t865 = gem_gt(gem_len_fn(NULL, _t863, 1), gem_int(0));
    } else {
        GemVal _t864[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t865 = gem_eq(gem_type_fn(NULL, _t864, 1), gem_string("int"));
    }
                                                            if (gem_truthy(_t865)) {
                                                                GemVal gem_v_vi = gem_int(0);
                                                                while (1) {
                                                                    GemVal _t866[] = {gem_v_v};
                                                                    if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t866, 1)))) break;
    GemVal _t867[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t867, 1), gem_string("table")))) {
    GemVal _t868 = (*gem_v_collect_free_node);
    GemVal _t869[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
                                                                        (void)(_t868.fn(_t868.env, _t869, 3));
                                                                    }
                                                                    gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                                }
                                                            } else {
    GemVal _t870 = (*gem_v_collect_free_node);
    GemVal _t871[] = {gem_v_v, gem_v_defined, gem_v_free};
                                                                (void)(_t870.fn(_t870.env, _t871, 3));
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
    GemVal _t873 = gem_table_new();
    GemVal gem_v_free = _t873;
    GemVal _t874 = gem_table_new();
    GemVal _t875[] = {gem_v_defined, _t874};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t875, 2);
    {
        GemVal gem_v__for_i_15 = gem_int(0);
    GemVal _t876[] = {gem_v_stmts};
        GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t876, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
            GemVal gem_v_i = gem_v__for_i_15;
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
    GemVal _t877 = (*gem_v_collect_free_node);
    GemVal _t878[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
            (void)(_t877.fn(_t877.env, _t878, 3));
    GemVal _t879[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t881;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t879, 1), gem_string("table")))) {
        _t881 = gem_eq(gem_type_fn(NULL, _t879, 1), gem_string("table"));
    } else {
        GemVal _t880 = gem_table_get(gem_v_stmts, gem_v_i);
        _t881 = gem_eq(gem_table_get(_t880, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t881)) {
    GemVal _t882 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t883[] = {gem_v_d, gem_table_get(_t882, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t883, 2));
            }
    GemVal _t884[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t886;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t884, 1), gem_string("table")))) {
        _t886 = gem_eq(gem_type_fn(NULL, _t884, 1), gem_string("table"));
    } else {
        GemVal _t885 = gem_table_get(gem_v_stmts, gem_v_i);
        _t886 = gem_eq(gem_table_get(_t885, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t886)) {
                {
                    GemVal gem_v__for_i_14 = gem_int(0);
    GemVal _t887 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t888[] = {gem_table_get(_t887, gem_string("stmts"))};
                    GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t888, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
                        GemVal gem_v_j = gem_v__for_i_14;
                        gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
    GemVal _t889 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t890[] = {gem_table_get(gem_table_get(_t889, gem_string("stmts")), gem_v_j)};
    GemVal _t893;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t890, 1), gem_string("table")))) {
        _t893 = gem_eq(gem_type_fn(NULL, _t890, 1), gem_string("table"));
    } else {
        GemVal _t891 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t892 = gem_table_get(gem_table_get(_t891, gem_string("stmts")), gem_v_j);
        _t893 = gem_eq(gem_table_get(_t892, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t893)) {
    GemVal _t894 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t895 = gem_table_get(gem_table_get(_t894, gem_string("stmts")), gem_v_j);
    GemVal _t896[] = {gem_v_d, gem_table_get(_t895, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t896, 2));
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
    GemVal _t898[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t898, 1), gem_string("table")))) {
        return GEM_NIL;
    }
    GemVal _t899 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t899, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, GEM_NIL))) {
        return GEM_NIL;
    }
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t900 = gem_v_node;
    GemVal _t901[] = {gem_table_get(_t900, gem_string("params"))};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t901, 1);
    GemVal _t902 = gem_v_node;
    GemVal _t903 = (*gem_v_collect_free_vars);
    GemVal _t904[] = {gem_table_get(_t902, gem_string("body")), gem_v_inner_defined};
        GemVal gem_v_free = _t903.fn(_t903.env, _t904, 2);
    GemVal _t905[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t905, 2);
    GemVal _t906[] = {gem_v_inter};
        GemVal gem_v_iks = gem_fn_keys(NULL, _t906, 1);
        GemVal gem_v_iki = gem_int(0);
        while (1) {
            GemVal _t907[] = {gem_v_iks};
            if (!gem_truthy(gem_lt(gem_v_iki, gem_len_fn(NULL, _t907, 1)))) break;
    GemVal _t908[] = {gem_v_captured, gem_table_get(gem_v_iks, gem_v_iki)};
            (void)(gem_fn_set_add(NULL, _t908, 2));
            gem_v_iki = gem_add(gem_v_iki, gem_int(1));
        }
    GemVal _t909 = gem_v_node;
    GemVal _t910 = gem_v_node;
    GemVal _t911[] = {gem_table_get(_t910, gem_string("params"))};
    GemVal _t912[] = {gem_v_scope_vars, gem_fn_sorted_array_to_set(NULL, _t911, 1)};
    GemVal _t913 = (*gem_v_walk_captures);
    GemVal _t914[] = {gem_table_get(_t909, gem_string("body")), gem_fn_set_union(NULL, _t912, 2), gem_v_captured};
        return _t913.fn(_t913.env, _t914, 3);
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
            return GEM_NIL;
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t915 = gem_v_node;
    GemVal _t916 = (*gem_v_walk_captures_node);
    GemVal _t917[] = {gem_table_get(_t915, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                (void)(_t916.fn(_t916.env, _t917, 3));
    GemVal _t918 = (*gem_v_walk_captures);
    GemVal _t919[] = {gem_table_get(gem_v_node, gem_string("then")), gem_v_scope_vars, gem_v_captured};
                (void)(_t918.fn(_t918.env, _t919, 3));
                if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t920 = (*gem_v_walk_captures);
    GemVal _t921[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                    return _t920.fn(_t920.env, _t921, 3);
                } else {
                    return GEM_NIL;
                }
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t922 = gem_v_node;
    GemVal _t923 = (*gem_v_walk_captures_node);
    GemVal _t924[] = {gem_table_get(_t922, gem_string("cond")), gem_v_scope_vars, gem_v_captured};
                    (void)(_t923.fn(_t923.env, _t924, 3));
    GemVal _t925 = gem_v_node;
    GemVal _t926 = (*gem_v_walk_captures);
    GemVal _t927[] = {gem_table_get(_t925, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                    return _t926.fn(_t926.env, _t927, 3);
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t928 = gem_v_node;
    GemVal _t929 = (*gem_v_walk_captures_node);
    GemVal _t930[] = {gem_table_get(_t928, gem_string("target")), gem_v_scope_vars, gem_v_captured};
                        (void)(_t929.fn(_t929.env, _t930, 3));
                        {
                            GemVal gem_v__for_i_16 = gem_int(0);
    GemVal _t931 = gem_v_node;
    GemVal _t932[] = {gem_table_get(_t931, gem_string("whens"))};
                            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t932, 1);
                            while (1) {
                                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
                                GemVal gem_v_i = gem_v__for_i_16;
                                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
    GemVal _t933 = gem_v_node;
    GemVal _t934 = gem_table_get(gem_table_get(_t933, gem_string("whens")), gem_v_i);
    GemVal _t935 = (*gem_v_walk_captures_node);
    GemVal _t936[] = {gem_table_get(_t934, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t935.fn(_t935.env, _t936, 3));
    GemVal _t937 = gem_v_node;
    GemVal _t938 = gem_table_get(gem_table_get(_t937, gem_string("whens")), gem_v_i);
    GemVal _t939 = (*gem_v_walk_captures);
    GemVal _t940[] = {gem_table_get(_t938, gem_string("body")), gem_v_scope_vars, gem_v_captured};
                                (void)(_t939.fn(_t939.env, _t940, 3));
                            }
                        }
                        if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
    GemVal _t941 = (*gem_v_walk_captures);
    GemVal _t942[] = {gem_table_get(gem_v_node, gem_string("else")), gem_v_scope_vars, gem_v_captured};
                            return _t941.fn(_t941.env, _t942, 3);
                        } else {
                            return GEM_NIL;
                        }
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
    GemVal _t943 = gem_v_node;
    GemVal _t944 = (*gem_v_walk_captures);
    GemVal _t945[] = {gem_table_get(_t943, gem_string("stmts")), gem_v_scope_vars, gem_v_captured};
                            return _t944.fn(_t944.env, _t945, 3);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t946 = gem_v_node;
                                if (gem_truthy(gem_neq(gem_table_get(_t946, gem_string("value")), GEM_NIL))) {
    GemVal _t947 = gem_v_node;
    GemVal _t948 = (*gem_v_walk_captures_node);
    GemVal _t949[] = {gem_table_get(_t947, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t948.fn(_t948.env, _t949, 3);
                                } else {
                                    return GEM_NIL;
                                }
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t950 = gem_v_node;
    GemVal _t951 = (*gem_v_walk_captures_node);
    GemVal _t952[] = {gem_table_get(_t950, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                    return _t951.fn(_t951.env, _t952, 3);
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t953 = gem_v_node;
    GemVal _t954 = (*gem_v_walk_captures_node);
    GemVal _t955[] = {gem_table_get(_t953, gem_string("value")), gem_v_scope_vars, gem_v_captured};
                                        return _t954.fn(_t954.env, _t955, 3);
                                    } else {
    GemVal _t956[] = {gem_v_node};
                                        GemVal gem_v_nks = gem_fn_keys(NULL, _t956, 1);
                                        GemVal gem_v_nki = gem_int(0);
                                        while (1) {
                                            GemVal _t957[] = {gem_v_nks};
                                            if (!gem_truthy(gem_lt(gem_v_nki, gem_len_fn(NULL, _t957, 1)))) break;
                                            if (gem_truthy(gem_neq(gem_table_get(gem_v_nks, gem_v_nki), gem_string("tag")))) {
                                                GemVal gem_v_v = gem_table_get(gem_v_node, gem_table_get(gem_v_nks, gem_v_nki));
    GemVal _t958[] = {gem_v_v};
                                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t958, 1), gem_string("table")))) {
    GemVal _t959[] = {gem_v_v};
                                                    GemVal gem_v_vks = gem_fn_keys(NULL, _t959, 1);
    GemVal _t960[] = {gem_v_vks};
    GemVal _t962;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t960, 1), gem_int(0)))) {
        _t962 = gem_gt(gem_len_fn(NULL, _t960, 1), gem_int(0));
    } else {
        GemVal _t961[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t962 = gem_eq(gem_type_fn(NULL, _t961, 1), gem_string("int"));
    }
                                                    if (gem_truthy(_t962)) {
                                                        GemVal gem_v_vi = gem_int(0);
                                                        while (1) {
                                                            GemVal _t963[] = {gem_v_v};
                                                            if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t963, 1)))) break;
    GemVal _t964[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                                            if (gem_truthy(gem_eq(gem_type_fn(NULL, _t964, 1), gem_string("table")))) {
    GemVal _t965 = (*gem_v_walk_captures_node);
    GemVal _t966[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
                                                                (void)(_t965.fn(_t965.env, _t966, 3));
                                                            }
                                                            gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                                                        }
                                                    } else {
    GemVal _t967 = (*gem_v_walk_captures_node);
    GemVal _t968[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
                                                        (void)(_t967.fn(_t967.env, _t968, 3));
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
    GemVal _t970 = gem_table_new();
    GemVal _t971[] = {gem_v_scope_vars, _t970};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t971, 2);
    {
        GemVal gem_v__for_i_18 = gem_int(0);
    GemVal _t972[] = {gem_v_stmts};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t972, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
            GemVal gem_v_i = gem_v__for_i_18;
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
    GemVal _t973 = (*gem_v_walk_captures_node);
    GemVal _t974[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
            (void)(_t973.fn(_t973.env, _t974, 3));
    GemVal _t975[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t977;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t975, 1), gem_string("table")))) {
        _t977 = gem_eq(gem_type_fn(NULL, _t975, 1), gem_string("table"));
    } else {
        GemVal _t976 = gem_table_get(gem_v_stmts, gem_v_i);
        _t977 = gem_eq(gem_table_get(_t976, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t977)) {
    GemVal _t978 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t979[] = {gem_v_sv, gem_table_get(_t978, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t979, 2));
            }
    GemVal _t980[] = {gem_table_get(gem_v_stmts, gem_v_i)};
    GemVal _t982;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t980, 1), gem_string("table")))) {
        _t982 = gem_eq(gem_type_fn(NULL, _t980, 1), gem_string("table"));
    } else {
        GemVal _t981 = gem_table_get(gem_v_stmts, gem_v_i);
        _t982 = gem_eq(gem_table_get(_t981, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t982)) {
                {
                    GemVal gem_v__for_i_17 = gem_int(0);
    GemVal _t983 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t984[] = {gem_table_get(_t983, gem_string("stmts"))};
                    GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t984, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
                        GemVal gem_v_j = gem_v__for_i_17;
                        gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
    GemVal _t985 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t986[] = {gem_table_get(gem_table_get(_t985, gem_string("stmts")), gem_v_j)};
    GemVal _t989;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t986, 1), gem_string("table")))) {
        _t989 = gem_eq(gem_type_fn(NULL, _t986, 1), gem_string("table"));
    } else {
        GemVal _t987 = gem_table_get(gem_v_stmts, gem_v_i);
        GemVal _t988 = gem_table_get(gem_table_get(_t987, gem_string("stmts")), gem_v_j);
        _t989 = gem_eq(gem_table_get(_t988, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t989)) {
    GemVal _t990 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t991 = gem_table_get(gem_table_get(_t990, gem_string("stmts")), gem_v_j);
    GemVal _t992[] = {gem_v_sv, gem_table_get(_t991, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t992, 2));
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
    GemVal _t994 = gem_table_new();
    GemVal gem_v_captured = _t994;
    GemVal _t995 = (*gem_v_walk_captures);
    GemVal _t996[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    (void)(_t995.fn(_t995.env, _t996, 3));
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
    GemVal _t998 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t998, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("int")))) {
    GemVal _t999 = gem_table_new();
    GemVal _t1000 = gem_v_node;
    GemVal _t1001[] = {gem_table_get(_t1000, gem_string("value"))};
    gem_table_set(_t999, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t1001, 1)), gem_string(")")));
    gem_table_set(_t999, gem_string("setup"), gem_string(""));
        return _t999;
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("float")))) {
    GemVal _t1002 = gem_table_new();
    GemVal _t1003 = gem_v_node;
    GemVal _t1004[] = {gem_table_get(_t1003, gem_string("value"))};
    gem_table_set(_t1002, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_fn_format_float(NULL, _t1004, 1)), gem_string(")")));
    gem_table_set(_t1002, gem_string("setup"), gem_string(""));
            return _t1002;
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("string")))) {
    GemVal _t1005 = gem_v_node;
    GemVal _t1006[] = {gem_table_get(_t1005, gem_string("value"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1006, 1);
    GemVal _t1007 = gem_table_new();
    gem_table_set(_t1007, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_v_escaped), gem_string("\")")));
    gem_table_set(_t1007, gem_string("setup"), gem_string(""));
                return _t1007;
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("bool")))) {
    GemVal _t1008 = gem_v_node;
                    if (gem_truthy(gem_table_get(_t1008, gem_string("value")))) {
    GemVal _t1009 = gem_table_new();
    gem_table_set(_t1009, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t1009, gem_string("setup"), gem_string(""));
                        return _t1009;
                    }
    GemVal _t1010 = gem_table_new();
    gem_table_set(_t1010, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t1010, gem_string("setup"), gem_string(""));
                    return _t1010;
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("nil")))) {
    GemVal _t1011 = gem_table_new();
    gem_table_set(_t1011, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1011, gem_string("setup"), gem_string(""));
                        return _t1011;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("var")))) {
    GemVal _t1012 = gem_v_node;
                            GemVal gem_v_name = gem_table_get(_t1012, gem_string("name"));
                            if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
    GemVal _t1013 = gem_table_new();
    gem_table_set(_t1013, gem_string("expr"), gem_string("gem_make_fn(gem_print, NULL)"));
    gem_table_set(_t1013, gem_string("setup"), gem_string(""));
                                return _t1013;
                            } else {
                                if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
    GemVal _t1014 = gem_table_new();
    gem_table_set(_t1014, gem_string("expr"), gem_string("gem_make_fn(gem_error_fn, NULL)"));
    gem_table_set(_t1014, gem_string("setup"), gem_string(""));
                                    return _t1014;
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_name, gem_string("len")))) {
    GemVal _t1015 = gem_table_new();
    gem_table_set(_t1015, gem_string("expr"), gem_string("gem_make_fn(gem_len_fn, NULL)"));
    gem_table_set(_t1015, gem_string("setup"), gem_string(""));
                                        return _t1015;
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_name, gem_string("type")))) {
    GemVal _t1016 = gem_table_new();
    gem_table_set(_t1016, gem_string("expr"), gem_string("gem_make_fn(gem_type_fn, NULL)"));
    gem_table_set(_t1016, gem_string("setup"), gem_string(""));
                                            return _t1016;
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_name, gem_string("to_string")))) {
    GemVal _t1017 = gem_table_new();
    gem_table_set(_t1017, gem_string("expr"), gem_string("gem_make_fn(gem_to_string_fn, NULL)"));
    gem_table_set(_t1017, gem_string("setup"), gem_string(""));
                                                return _t1017;
                                            } else {
    GemVal _t1018[] = {(*gem_v_defined_fns), gem_v_name};
                                                if (gem_truthy(gem_fn_set_contains(NULL, _t1018, 2))) {
    GemVal _t1019 = gem_table_new();
    gem_table_set(_t1019, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_v_name), gem_string(", NULL)")));
    gem_table_set(_t1019, gem_string("setup"), gem_string(""));
                                                    return _t1019;
                                                }
                                            }
                                        }
                                    }
                                }
                            }
    GemVal _t1020 = (*gem_v_mangle);
    GemVal _t1021[] = {gem_v_name};
                            GemVal gem_v_mname = _t1020.fn(_t1020.env, _t1021, 1);
    GemVal _t1022[] = {(*gem_v_boxed_vars), gem_v_name};
                            if (gem_truthy(gem_fn_set_contains(NULL, _t1022, 2))) {
    GemVal _t1023 = gem_table_new();
    gem_table_set(_t1023, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_v_mname), gem_string(")")));
    gem_table_set(_t1023, gem_string("setup"), gem_string(""));
                                return _t1023;
                            }
    GemVal _t1024 = gem_table_new();
    gem_table_set(_t1024, gem_string("expr"), gem_v_mname);
    gem_table_set(_t1024, gem_string("setup"), gem_string(""));
                            return _t1024;
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
    GemVal _t1025 = (*gem_v_compile_call);
    GemVal _t1026[] = {gem_v_node};
                                return _t1025.fn(_t1025.env, _t1026, 1);
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("binop")))) {
    GemVal _t1027 = (*gem_v_compile_binop);
    GemVal _t1028[] = {gem_v_node};
                                    return _t1027.fn(_t1027.env, _t1028, 1);
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("unop")))) {
    GemVal _t1029 = gem_v_node;
    GemVal _t1030 = (*gem_v_compile_expr);
    GemVal _t1031[] = {gem_table_get(_t1029, gem_string("expr"))};
                                        GemVal gem_v_r = _t1030.fn(_t1030.env, _t1031, 1);
    GemVal _t1032 = gem_v_node;
                                        if (gem_truthy(gem_eq(gem_table_get(_t1032, gem_string("op")), gem_string("-")))) {
    GemVal _t1033 = gem_table_new();
    GemVal _t1034 = gem_v_r;
    gem_table_set(_t1033, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_table_get(_t1034, gem_string("expr"))), gem_string(")")));
    GemVal _t1035 = gem_v_r;
    gem_table_set(_t1033, gem_string("setup"), gem_table_get(_t1035, gem_string("setup")));
                                            return _t1033;
                                        } else {
    GemVal _t1036 = gem_v_node;
                                            if (gem_truthy(gem_eq(gem_table_get(_t1036, gem_string("op")), gem_string("not")))) {
    GemVal _t1037 = gem_table_new();
    GemVal _t1038 = gem_v_r;
    gem_table_set(_t1037, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_table_get(_t1038, gem_string("expr"))), gem_string(")")));
    GemVal _t1039 = gem_v_r;
    gem_table_set(_t1037, gem_string("setup"), gem_table_get(_t1039, gem_string("setup")));
                                                return _t1037;
                                            }
                                        }
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("anon_fn")))) {
    GemVal _t1040 = (*gem_v_compile_anon_fn);
    GemVal _t1041[] = {gem_v_node};
                                            return _t1040.fn(_t1040.env, _t1041, 1);
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("table")))) {
    GemVal _t1042 = (*gem_v_compile_table);
    GemVal _t1043[] = {gem_v_node};
                                                return _t1042.fn(_t1042.env, _t1043, 1);
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("array")))) {
    GemVal _t1044 = (*gem_v_compile_array);
    GemVal _t1045[] = {gem_v_node};
                                                    return _t1044.fn(_t1044.env, _t1045, 1);
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot")))) {
    GemVal _t1046 = gem_v_node;
    GemVal _t1047 = (*gem_v_compile_expr);
    GemVal _t1048[] = {gem_table_get(_t1046, gem_string("object"))};
                                                        GemVal gem_v_r = _t1047.fn(_t1047.env, _t1048, 1);
    GemVal _t1049 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1049.fn(_t1049.env, NULL, 0);
    GemVal _t1050 = gem_v_r;
    GemVal _t1051 = gem_v_r;
                                                        GemVal gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1050, gem_string("setup")), gem_string("    GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1051, gem_string("expr"))), gem_string(";\n"));
    GemVal _t1052 = gem_v_node;
    GemVal _t1053[] = {gem_table_get(_t1052, gem_string("field"))};
                                                        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1053, 1);
    GemVal _t1054 = gem_table_new();
    gem_table_set(_t1054, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_v_t), gem_string(", gem_string(\"")), gem_v_escaped), gem_string("\"))")));
    gem_table_set(_t1054, gem_string("setup"), gem_v_setup);
                                                        return _t1054;
                                                    } else {
                                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("index")))) {
    GemVal _t1055 = gem_v_node;
    GemVal _t1056 = (*gem_v_compile_expr);
    GemVal _t1057[] = {gem_table_get(_t1055, gem_string("object"))};
                                                            GemVal gem_v_obj_r = _t1056.fn(_t1056.env, _t1057, 1);
    GemVal _t1058 = gem_v_node;
    GemVal _t1059 = (*gem_v_compile_expr);
    GemVal _t1060[] = {gem_table_get(_t1058, gem_string("key"))};
                                                            GemVal gem_v_key_r = _t1059.fn(_t1059.env, _t1060, 1);
    GemVal _t1061 = gem_table_new();
    GemVal _t1062 = gem_v_obj_r;
    GemVal _t1063 = gem_v_key_r;
    gem_table_set(_t1061, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_table_get(_t1062, gem_string("expr"))), gem_string(", ")), gem_table_get(_t1063, gem_string("expr"))), gem_string(")")));
    GemVal _t1064 = gem_v_obj_r;
    GemVal _t1065 = gem_v_key_r;
    gem_table_set(_t1061, gem_string("setup"), gem_add(gem_table_get(_t1064, gem_string("setup")), gem_table_get(_t1065, gem_string("setup"))));
                                                            return _t1061;
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
    GemVal _t1066 = gem_table_new();
    gem_table_set(_t1066, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t1066, gem_string("setup"), gem_add(gem_add(gem_string("/* unknown node: "), gem_v_tag), gem_string(" */\n")));
    return _t1066;
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
    GemVal _t1068 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t1068, gem_string("entries"));
    GemVal _t1069 = (*gem_v_tmp);
    GemVal gem_v_t = _t1069.fn(_t1069.env, NULL, 0);
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_v_t), gem_string(" = gem_table_new();\n"));
    {
        GemVal gem_v__for_items_19 = gem_v_entries;
        GemVal gem_v__for_i_19 = gem_int(0);
        while (1) {
            GemVal _t1070[] = {gem_v__for_items_19};
            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t1070, 1)))) break;
            GemVal gem_v_entry = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
    GemVal _t1071 = gem_v_entry;
    GemVal _t1072 = (*gem_v_compile_expr);
    GemVal _t1073[] = {gem_table_get(_t1071, gem_string("value"))};
            GemVal gem_v_val_r = _t1072.fn(_t1072.env, _t1073, 1);
    GemVal _t1074 = gem_v_entry;
    GemVal _t1075[] = {gem_table_get(_t1074, gem_string("key"))};
            GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1075, 1);
    GemVal _t1076 = gem_v_val_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1076, gem_string("setup")));
    GemVal _t1077 = gem_v_val_r;
            gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    gem_table_set(")), gem_v_t), gem_string(", gem_string(\"")), gem_v_escaped), gem_string("\"), ")), gem_table_get(_t1077, gem_string("expr"))), gem_string(");\n"));
        }
    }
    GemVal _t1078 = gem_table_new();
    gem_table_set(_t1078, gem_string("expr"), gem_v_t);
    gem_table_set(_t1078, gem_string("setup"), gem_v_setup);
    return _t1078;
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
    GemVal _t1080 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t1080, gem_string("elements"));
    GemVal _t1081 = (*gem_v_tmp);
    GemVal gem_v_t = _t1081.fn(_t1081.env, NULL, 0);
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_v_t), gem_string(" = gem_table_new();\n"));
    {
        GemVal gem_v__for_i_20 = gem_int(0);
    GemVal _t1082[] = {gem_v_elements};
        GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1082, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
            GemVal gem_v_i = gem_v__for_i_20;
            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
    GemVal _t1083 = (*gem_v_compile_expr);
    GemVal _t1084[] = {gem_table_get(gem_v_elements, gem_v_i)};
            GemVal gem_v_elem_r = _t1083.fn(_t1083.env, _t1084, 1);
    GemVal _t1085 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t1085, gem_string("setup")));
    GemVal _t1086[] = {gem_v_i};
    GemVal _t1087 = gem_v_elem_r;
            gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    gem_table_set(")), gem_v_t), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t1086, 1)), gem_string("), ")), gem_table_get(_t1087, gem_string("expr"))), gem_string(");\n"));
        }
    }
    GemVal _t1088 = gem_table_new();
    gem_table_set(_t1088, gem_string("expr"), gem_v_t);
    gem_table_set(_t1088, gem_string("setup"), gem_v_setup);
    return _t1088;
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
    GemVal _t1090 = gem_v_node;
    GemVal _t1091[] = {gem_table_get(_t1090, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1091, 1);
    GemVal _t1092 = gem_v_node;
    GemVal _t1093 = (*gem_v_collect_free_vars);
    GemVal _t1094[] = {gem_table_get(_t1092, gem_string("body")), gem_v_inner_defined};
    GemVal gem_v_free = _t1093.fn(_t1093.env, _t1094, 2);
    GemVal _t1095[] = {gem_v_free};
    GemVal gem_v_captures = gem_fn_set_to_sorted_array(NULL, _t1095, 1);
    GemVal _t1096[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t1096, 1), gem_int(0)))) {
    GemVal _t1097 = gem_table_new();
    GemVal _t1098 = (*gem_v_compile_closure_fn);
    GemVal _t1099[] = {gem_v_node, _t1097};
        GemVal gem_v_result = _t1098.fn(_t1098.env, _t1099, 2);
    GemVal _t1100 = gem_table_new();
    GemVal _t1101 = gem_v_result;
    gem_table_set(_t1100, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_table_get(_t1101, gem_string("fn_name"))), gem_string(", NULL)")));
    gem_table_set(_t1100, gem_string("setup"), gem_string(""));
        return _t1100;
    }
    GemVal _t1102 = (*gem_v_compile_closure_fn);
    GemVal _t1103[] = {gem_v_node, gem_v_captures};
    GemVal gem_v_result = _t1102.fn(_t1102.env, _t1103, 2);
    GemVal gem_v_setup = gem_string("");
    GemVal _t1104 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t1104.fn(_t1104.env, NULL, 0);
    GemVal _t1105 = gem_v_result;
    GemVal _t1106 = gem_v_result;
    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    struct ")), gem_table_get(_t1105, gem_string("struct_name"))), gem_string(" *")), gem_v_env_tmp), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_table_get(_t1106, gem_string("struct_name"))), gem_string("));\n"));
    {
        GemVal gem_v__for_items_21 = gem_v_captures;
        GemVal gem_v__for_i_21 = gem_int(0);
        while (1) {
            GemVal _t1107[] = {gem_v__for_items_21};
            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t1107, 1)))) break;
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
    GemVal _t1108 = (*gem_v_mangle);
    GemVal _t1109[] = {gem_v_cap};
            GemVal gem_v_mc = _t1108.fn(_t1108.env, _t1109, 1);
    GemVal _t1110[] = {(*gem_v_boxed_vars), gem_v_cap};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1110, 2))) {
                gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    ")), gem_v_env_tmp), gem_string("->")), gem_v_mc), gem_string(" = ")), gem_v_mc), gem_string(";\n"));
            } else {
                gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    ")), gem_v_env_tmp), gem_string("->")), gem_v_mc), gem_string(" = &")), gem_v_mc), gem_string(";\n"));
            }
        }
    }
    GemVal _t1111 = gem_table_new();
    GemVal _t1112 = gem_v_result;
    gem_table_set(_t1111, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_table_get(_t1112, gem_string("fn_name"))), gem_string(", ")), gem_v_env_tmp), gem_string(")")));
    gem_table_set(_t1111, gem_string("setup"), gem_v_setup);
    return _t1111;
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
    GemVal _t1114 = gem_v_node;
    GemVal gem_v_func = gem_table_get(_t1114, gem_string("func"));
    GemVal _t1115 = gem_v_node;
    GemVal gem_v_args = gem_table_get(_t1115, gem_string("args"));
    GemVal _t1116 = gem_v_func;
    GemVal _t1118;
    if (!gem_truthy(gem_eq(gem_table_get(_t1116, gem_string("tag")), gem_string("dot")))) {
        _t1118 = gem_eq(gem_table_get(_t1116, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t1117 = gem_v_func;
        _t1118 = gem_eq(gem_table_get(_t1117, gem_string("field")), gem_string("len"));
    }
    GemVal _t1120;
    if (!gem_truthy(_t1118)) {
        _t1120 = _t1118;
    } else {
        GemVal _t1119[] = {gem_v_args};
        _t1120 = gem_eq(gem_len_fn(NULL, _t1119, 1), gem_int(0));
    }
    if (gem_truthy(_t1120)) {
    GemVal _t1121 = gem_v_func;
    GemVal _t1122 = (*gem_v_compile_expr);
    GemVal _t1123[] = {gem_table_get(_t1121, gem_string("object"))};
        GemVal gem_v_obj_r = _t1122.fn(_t1122.env, _t1123, 1);
    GemVal _t1124 = gem_table_new();
    GemVal _t1125 = gem_v_obj_r;
    gem_table_set(_t1124, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_table_get(_t1125, gem_string("expr"))), gem_string(")")));
    GemVal _t1126 = gem_v_obj_r;
    gem_table_set(_t1124, gem_string("setup"), gem_table_get(_t1126, gem_string("setup")));
        return _t1124;
    }
    GemVal _t1127 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t1127, gem_string("tag")), gem_string("var")))) {
    GemVal _t1128 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t1128, gem_string("name"));
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
    GemVal _t1129 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t1129, gem_string("line"));
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
                gem_v_line = gem_int(0);
            }
    GemVal _t1130[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t1130, 1);
            GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1131 = gem_table_new();
            GemVal gem_v_arg_exprs = _t1131;
            {
                GemVal gem_v__for_i_22 = gem_int(0);
    GemVal _t1132[] = {gem_v_args};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1132, 1);
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
                    GemVal gem_v_i = gem_v__for_i_22;
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
    GemVal _t1133 = (*gem_v_compile_expr);
    GemVal _t1134[] = {gem_table_get(gem_v_args, gem_v_i)};
                    GemVal gem_v_r = _t1133.fn(_t1133.env, _t1134, 1);
    GemVal _t1135 = gem_v_r;
                    gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1135, gem_string("setup")));
    GemVal _t1136 = gem_v_r;
                    gem_table_set(gem_v_arg_exprs, gem_v_i, gem_table_get(_t1136, gem_string("expr")));
                }
            }
    GemVal _t1137[] = {gem_v_args};
            GemVal gem_v_argc = gem_len_fn(NULL, _t1137, 1);
            if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1138 = gem_table_new();
    GemVal _t1139[] = {gem_v_line};
    gem_table_set(_t1138, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_v_escaped_file), gem_string("\", ")), gem_to_string_fn(NULL, _t1139, 1)), gem_string(", NULL, 0)")));
    gem_table_set(_t1138, gem_string("setup"), gem_v_arg_setups);
                return _t1138;
            }
    GemVal _t1140 = (*gem_v_tmp);
            GemVal gem_v_t = _t1140.fn(_t1140.env, NULL, 0);
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
    GemVal _t1141 = gem_table_new();
    GemVal _t1142[] = {gem_v_line};
    GemVal _t1143[] = {gem_v_argc};
    gem_table_set(_t1141, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_v_escaped_file), gem_string("\", ")), gem_to_string_fn(NULL, _t1142, 1)), gem_string(", ")), gem_v_t), gem_string(", ")), gem_to_string_fn(NULL, _t1143, 1)), gem_string(")")));
    gem_table_set(_t1141, gem_string("setup"), gem_v_setup);
            return _t1141;
        } else {
    GemVal _t1144;
    if (gem_truthy(gem_eq(gem_v_name, gem_string("print")))) {
        _t1144 = gem_eq(gem_v_name, gem_string("print"));
    } else {
        _t1144 = gem_eq(gem_v_name, gem_string("len"));
    }
    GemVal _t1145;
    if (gem_truthy(_t1144)) {
        _t1145 = _t1144;
    } else {
        _t1145 = gem_eq(gem_v_name, gem_string("type"));
    }
    GemVal _t1146;
    if (gem_truthy(_t1145)) {
        _t1146 = _t1145;
    } else {
        _t1146 = gem_eq(gem_v_name, gem_string("to_string"));
    }
            if (gem_truthy(_t1146)) {
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
                            }
                        }
                    }
                }
                GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1147 = gem_table_new();
                GemVal gem_v_arg_exprs = _t1147;
                {
                    GemVal gem_v__for_i_24 = gem_int(0);
    GemVal _t1148[] = {gem_v_args};
                    GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1148, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
                        GemVal gem_v_i = gem_v__for_i_24;
                        gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
    GemVal _t1149 = (*gem_v_compile_expr);
    GemVal _t1150[] = {gem_table_get(gem_v_args, gem_v_i)};
                        GemVal gem_v_r = _t1149.fn(_t1149.env, _t1150, 1);
    GemVal _t1151 = gem_v_r;
                        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1151, gem_string("setup")));
    GemVal _t1152 = gem_v_r;
                        gem_table_set(gem_v_arg_exprs, gem_v_i, gem_table_get(_t1152, gem_string("expr")));
                    }
                }
    GemVal _t1153[] = {gem_v_args};
                GemVal gem_v_argc = gem_len_fn(NULL, _t1153, 1);
                if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1154 = gem_table_new();
    gem_table_set(_t1154, gem_string("expr"), gem_add(gem_v_fn_name, gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1154, gem_string("setup"), gem_v_arg_setups);
                    return _t1154;
                }
    GemVal _t1155 = (*gem_v_tmp);
                GemVal gem_v_t = _t1155.fn(_t1155.env, NULL, 0);
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
    GemVal _t1156 = gem_table_new();
    GemVal _t1157[] = {gem_v_argc};
    gem_table_set(_t1156, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_fn_name, gem_string("(NULL, ")), gem_v_t), gem_string(", ")), gem_to_string_fn(NULL, _t1157, 1)), gem_string(")")));
    gem_table_set(_t1156, gem_string("setup"), gem_v_setup);
                return _t1156;
            } else {
    GemVal _t1158[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t1158, 2))) {
                    GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1159 = gem_table_new();
                    GemVal gem_v_arg_exprs = _t1159;
                    {
                        GemVal gem_v__for_i_26 = gem_int(0);
    GemVal _t1160[] = {gem_v_args};
                        GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1160, 1);
                        while (1) {
                            if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
                            GemVal gem_v_i = gem_v__for_i_26;
                            gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
    GemVal _t1161 = (*gem_v_compile_expr);
    GemVal _t1162[] = {gem_table_get(gem_v_args, gem_v_i)};
                            GemVal gem_v_r = _t1161.fn(_t1161.env, _t1162, 1);
    GemVal _t1163 = gem_v_r;
                            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1163, gem_string("setup")));
    GemVal _t1164 = gem_v_r;
                            gem_table_set(gem_v_arg_exprs, gem_v_i, gem_table_get(_t1164, gem_string("expr")));
                        }
                    }
    GemVal _t1165[] = {gem_v_args};
                    GemVal gem_v_argc = gem_len_fn(NULL, _t1165, 1);
                    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1166 = gem_table_new();
    gem_table_set(_t1166, gem_string("expr"), gem_add(gem_add(gem_string("gem_fn_"), gem_v_name), gem_string("(NULL, NULL, 0)")));
    gem_table_set(_t1166, gem_string("setup"), gem_v_arg_setups);
                        return _t1166;
                    }
    GemVal _t1167 = (*gem_v_tmp);
                    GemVal gem_v_t = _t1167.fn(_t1167.env, NULL, 0);
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
    GemVal _t1168 = gem_table_new();
    GemVal _t1169[] = {gem_v_argc};
    gem_table_set(_t1168, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_fn_"), gem_v_name), gem_string("(NULL, ")), gem_v_t), gem_string(", ")), gem_to_string_fn(NULL, _t1169, 1)), gem_string(")")));
    gem_table_set(_t1168, gem_string("setup"), gem_v_setup);
                    return _t1168;
                }
            }
        }
    }
    GemVal _t1170 = (*gem_v_compile_expr);
    GemVal _t1171[] = {gem_v_func};
    GemVal gem_v_func_r = _t1170.fn(_t1170.env, _t1171, 1);
    GemVal gem_v_arg_setups = gem_string("");
    GemVal _t1172 = gem_table_new();
    GemVal gem_v_arg_exprs = _t1172;
    {
        GemVal gem_v__for_i_28 = gem_int(0);
    GemVal _t1173[] = {gem_v_args};
        GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1173, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
            GemVal gem_v_i = gem_v__for_i_28;
            gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
    GemVal _t1174 = (*gem_v_compile_expr);
    GemVal _t1175[] = {gem_table_get(gem_v_args, gem_v_i)};
            GemVal gem_v_r = _t1174.fn(_t1174.env, _t1175, 1);
    GemVal _t1176 = gem_v_r;
            gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t1176, gem_string("setup")));
    GemVal _t1177 = gem_v_r;
            gem_table_set(gem_v_arg_exprs, gem_v_i, gem_table_get(_t1177, gem_string("expr")));
        }
    }
    GemVal _t1178 = gem_v_func_r;
    GemVal gem_v_setup = gem_add(gem_table_get(_t1178, gem_string("setup")), gem_v_arg_setups);
    GemVal _t1179[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t1179, 1);
    GemVal _t1180 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t1180.fn(_t1180.env, NULL, 0);
    GemVal _t1181 = gem_v_func_r;
    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_tmp_fn), gem_string(" = ")), gem_table_get(_t1181, gem_string("expr"))), gem_string(";\n"));
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
    GemVal _t1182 = gem_table_new();
    gem_table_set(_t1182, gem_string("expr"), gem_add(gem_add(gem_add(gem_v_tmp_fn, gem_string(".fn(")), gem_v_tmp_fn), gem_string(".env, NULL, 0)")));
    gem_table_set(_t1182, gem_string("setup"), gem_v_setup);
        return _t1182;
    }
    GemVal _t1183 = (*gem_v_tmp);
    GemVal gem_v_tmp_args = _t1183.fn(_t1183.env, NULL, 0);
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
    GemVal _t1184 = gem_table_new();
    GemVal _t1185[] = {gem_v_argc};
    gem_table_set(_t1184, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_tmp_fn, gem_string(".fn(")), gem_v_tmp_fn), gem_string(".env, ")), gem_v_tmp_args), gem_string(", ")), gem_to_string_fn(NULL, _t1185, 1)), gem_string(")")));
    gem_table_set(_t1184, gem_string("setup"), gem_v_setup);
    return _t1184;
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
    GemVal _t1187 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t1187, gem_string("op"));
    GemVal _t1188 = gem_v_node;
    GemVal _t1189 = (*gem_v_compile_expr);
    GemVal _t1190[] = {gem_table_get(_t1188, gem_string("left"))};
    GemVal gem_v_lc_r = _t1189.fn(_t1189.env, _t1190, 1);
    GemVal _t1191 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t1191, gem_string("expr"));
    GemVal _t1192 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t1192, gem_string("setup"));
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
    GemVal _t1193 = gem_v_node;
    GemVal _t1194 = (*gem_v_compile_expr);
    GemVal _t1195[] = {gem_table_get(_t1193, gem_string("right"))};
        GemVal gem_v_rc_r = _t1194.fn(_t1194.env, _t1195, 1);
    GemVal _t1196 = gem_table_new();
    GemVal _t1197 = gem_v_rc_r;
    gem_table_set(_t1196, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_v_lc), gem_string(", ")), gem_table_get(_t1197, gem_string("expr"))), gem_string(")")));
    GemVal _t1198 = gem_v_rc_r;
    gem_table_set(_t1196, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1198, gem_string("setup"))));
        return _t1196;
    } else {
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
    GemVal _t1199 = gem_v_node;
    GemVal _t1200 = (*gem_v_compile_expr);
    GemVal _t1201[] = {gem_table_get(_t1199, gem_string("right"))};
            GemVal gem_v_rc_r = _t1200.fn(_t1200.env, _t1201, 1);
    GemVal _t1202 = gem_table_new();
    GemVal _t1203 = gem_v_rc_r;
    gem_table_set(_t1202, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_v_lc), gem_string(", ")), gem_table_get(_t1203, gem_string("expr"))), gem_string(")")));
    GemVal _t1204 = gem_v_rc_r;
    gem_table_set(_t1202, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1204, gem_string("setup"))));
            return _t1202;
        } else {
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
    GemVal _t1205 = gem_v_node;
    GemVal _t1206 = (*gem_v_compile_expr);
    GemVal _t1207[] = {gem_table_get(_t1205, gem_string("right"))};
                GemVal gem_v_rc_r = _t1206.fn(_t1206.env, _t1207, 1);
    GemVal _t1208 = gem_table_new();
    GemVal _t1209 = gem_v_rc_r;
    gem_table_set(_t1208, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_v_lc), gem_string(", ")), gem_table_get(_t1209, gem_string("expr"))), gem_string(")")));
    GemVal _t1210 = gem_v_rc_r;
    gem_table_set(_t1208, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1210, gem_string("setup"))));
                return _t1208;
            } else {
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
    GemVal _t1211 = gem_v_node;
    GemVal _t1212 = (*gem_v_compile_expr);
    GemVal _t1213[] = {gem_table_get(_t1211, gem_string("right"))};
                    GemVal gem_v_rc_r = _t1212.fn(_t1212.env, _t1213, 1);
    GemVal _t1214 = gem_table_new();
    GemVal _t1215 = gem_v_rc_r;
    gem_table_set(_t1214, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_v_lc), gem_string(", ")), gem_table_get(_t1215, gem_string("expr"))), gem_string(")")));
    GemVal _t1216 = gem_v_rc_r;
    gem_table_set(_t1214, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1216, gem_string("setup"))));
                    return _t1214;
                } else {
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
    GemVal _t1217 = gem_v_node;
    GemVal _t1218 = (*gem_v_compile_expr);
    GemVal _t1219[] = {gem_table_get(_t1217, gem_string("right"))};
                        GemVal gem_v_rc_r = _t1218.fn(_t1218.env, _t1219, 1);
    GemVal _t1220 = gem_table_new();
    GemVal _t1221 = gem_v_rc_r;
    gem_table_set(_t1220, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_v_lc), gem_string(", ")), gem_table_get(_t1221, gem_string("expr"))), gem_string(")")));
    GemVal _t1222 = gem_v_rc_r;
    gem_table_set(_t1220, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1222, gem_string("setup"))));
                        return _t1220;
                    } else {
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
    GemVal _t1223 = gem_v_node;
    GemVal _t1224 = (*gem_v_compile_expr);
    GemVal _t1225[] = {gem_table_get(_t1223, gem_string("right"))};
                            GemVal gem_v_rc_r = _t1224.fn(_t1224.env, _t1225, 1);
    GemVal _t1226 = gem_table_new();
    GemVal _t1227 = gem_v_rc_r;
    gem_table_set(_t1226, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_v_lc), gem_string(", ")), gem_table_get(_t1227, gem_string("expr"))), gem_string(")")));
    GemVal _t1228 = gem_v_rc_r;
    gem_table_set(_t1226, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1228, gem_string("setup"))));
                            return _t1226;
                        } else {
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
    GemVal _t1229 = gem_v_node;
    GemVal _t1230 = (*gem_v_compile_expr);
    GemVal _t1231[] = {gem_table_get(_t1229, gem_string("right"))};
                                GemVal gem_v_rc_r = _t1230.fn(_t1230.env, _t1231, 1);
    GemVal _t1232 = gem_table_new();
    GemVal _t1233 = gem_v_rc_r;
    gem_table_set(_t1232, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_v_lc), gem_string(", ")), gem_table_get(_t1233, gem_string("expr"))), gem_string(")")));
    GemVal _t1234 = gem_v_rc_r;
    gem_table_set(_t1232, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1234, gem_string("setup"))));
                                return _t1232;
                            } else {
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
    GemVal _t1235 = gem_v_node;
    GemVal _t1236 = (*gem_v_compile_expr);
    GemVal _t1237[] = {gem_table_get(_t1235, gem_string("right"))};
                                    GemVal gem_v_rc_r = _t1236.fn(_t1236.env, _t1237, 1);
    GemVal _t1238 = gem_table_new();
    GemVal _t1239 = gem_v_rc_r;
    gem_table_set(_t1238, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_v_lc), gem_string(", ")), gem_table_get(_t1239, gem_string("expr"))), gem_string(")")));
    GemVal _t1240 = gem_v_rc_r;
    gem_table_set(_t1238, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1240, gem_string("setup"))));
                                    return _t1238;
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
    GemVal _t1241 = gem_v_node;
    GemVal _t1242 = (*gem_v_compile_expr);
    GemVal _t1243[] = {gem_table_get(_t1241, gem_string("right"))};
                                        GemVal gem_v_rc_r = _t1242.fn(_t1242.env, _t1243, 1);
    GemVal _t1244 = gem_table_new();
    GemVal _t1245 = gem_v_rc_r;
    gem_table_set(_t1244, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_v_lc), gem_string(", ")), gem_table_get(_t1245, gem_string("expr"))), gem_string(")")));
    GemVal _t1246 = gem_v_rc_r;
    gem_table_set(_t1244, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1246, gem_string("setup"))));
                                        return _t1244;
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
    GemVal _t1247 = gem_v_node;
    GemVal _t1248 = (*gem_v_compile_expr);
    GemVal _t1249[] = {gem_table_get(_t1247, gem_string("right"))};
                                            GemVal gem_v_rc_r = _t1248.fn(_t1248.env, _t1249, 1);
    GemVal _t1250 = gem_table_new();
    GemVal _t1251 = gem_v_rc_r;
    gem_table_set(_t1250, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_v_lc), gem_string(", ")), gem_table_get(_t1251, gem_string("expr"))), gem_string(")")));
    GemVal _t1252 = gem_v_rc_r;
    gem_table_set(_t1250, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1252, gem_string("setup"))));
                                            return _t1250;
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
    GemVal _t1253 = gem_v_node;
    GemVal _t1254 = (*gem_v_compile_expr);
    GemVal _t1255[] = {gem_table_get(_t1253, gem_string("right"))};
                                                GemVal gem_v_rc_r = _t1254.fn(_t1254.env, _t1255, 1);
    GemVal _t1256 = gem_table_new();
    GemVal _t1257 = gem_v_rc_r;
    gem_table_set(_t1256, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_v_lc), gem_string(", ")), gem_table_get(_t1257, gem_string("expr"))), gem_string(")")));
    GemVal _t1258 = gem_v_rc_r;
    gem_table_set(_t1256, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t1258, gem_string("setup"))));
                                                return _t1256;
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
    GemVal _t1259 = gem_v_node;
    GemVal _t1260 = (*gem_v_compile_expr);
    GemVal _t1261[] = {gem_table_get(_t1259, gem_string("right"))};
                                                    GemVal gem_v_rc_r = _t1260.fn(_t1260.env, _t1261, 1);
    GemVal _t1262 = (*gem_v_tmp);
                                                    GemVal gem_v_t = _t1262.fn(_t1262.env, NULL, 0);
                                                    GemVal gem_v_setup = gem_v_ls;
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_t), gem_string(";\n"));
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    if (!gem_truthy(")), gem_v_lc), gem_string(")) {\n"));
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_v_lc), gem_string(";\n"));
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
    GemVal _t1263 = gem_v_rc_r;
                                                    if (gem_truthy(gem_neq(gem_table_get(_t1263, gem_string("setup")), gem_string("")))) {
    GemVal _t1264 = gem_v_rc_r;
    GemVal _t1265[] = {gem_table_get(_t1264, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1265, 3));
                                                    }
    GemVal _t1266 = gem_v_rc_r;
                                                    gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1266, gem_string("expr"))), gem_string(";\n"));
                                                    gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
    GemVal _t1267 = gem_table_new();
    gem_table_set(_t1267, gem_string("expr"), gem_v_t);
    gem_table_set(_t1267, gem_string("setup"), gem_v_setup);
                                                    return _t1267;
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
    GemVal _t1268 = gem_v_node;
    GemVal _t1269 = (*gem_v_compile_expr);
    GemVal _t1270[] = {gem_table_get(_t1268, gem_string("right"))};
                                                        GemVal gem_v_rc_r = _t1269.fn(_t1269.env, _t1270, 1);
    GemVal _t1271 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t1271.fn(_t1271.env, NULL, 0);
                                                        GemVal gem_v_setup = gem_v_ls;
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    GemVal ")), gem_v_t), gem_string(";\n"));
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_v_setup, gem_string("    if (gem_truthy(")), gem_v_lc), gem_string(")) {\n"));
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_v_lc), gem_string(";\n"));
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
    GemVal _t1272 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t1272, gem_string("setup")), gem_string("")))) {
    GemVal _t1273 = gem_v_rc_r;
    GemVal _t1274[] = {gem_table_get(_t1273, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_fn_str_replace_all(NULL, _t1274, 3));
                                                        }
    GemVal _t1275 = gem_v_rc_r;
                                                        gem_v_setup = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_setup, gem_string("        ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1275, gem_string("expr"))), gem_string(";\n"));
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
    GemVal _t1276 = gem_table_new();
    gem_table_set(_t1276, gem_string("expr"), gem_v_t);
    gem_table_set(_t1276, gem_string("setup"), gem_v_setup);
                                                        return _t1276;
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
    GemVal _t1277 = gem_v_node;
    GemVal _t1278 = (*gem_v_compile_expr);
    GemVal _t1279[] = {gem_table_get(_t1277, gem_string("right"))};
    GemVal gem_v_rc_r = _t1278.fn(_t1278.env, _t1279, 1);
    GemVal _t1280 = gem_table_new();
    gem_table_set(_t1280, gem_string("expr"), gem_string("GEM_NIL"));
    GemVal _t1281 = gem_v_rc_r;
    gem_table_set(_t1280, gem_string("setup"), gem_add(gem_add(gem_add(gem_add(gem_v_ls, gem_table_get(_t1281, gem_string("setup"))), gem_string("/* unknown op: ")), gem_v_op), gem_string(" */\n")));
    return _t1280;
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
            GemVal _t1283[] = {gem_v__for_items_30};
            if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_len_fn(NULL, _t1283, 1)))) break;
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_30, gem_v__for_i_30);
            gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
    GemVal _t1284 = (*gem_v_compile_stmt);
    GemVal _t1285[] = {gem_v_stmt, gem_v_indent};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1284.fn(_t1284.env, _t1285, 2)), gem_string("\n"));
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
    GemVal _t1287[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1287, 1);
    GemVal _t1288 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1288, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1289 = gem_v_node;
    GemVal _t1290 = (*gem_v_compile_expr);
    GemVal _t1291[] = {gem_table_get(_t1289, gem_string("value"))};
        GemVal gem_v_r = _t1290.fn(_t1290.env, _t1291, 1);
    GemVal _t1292 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1292, gem_string("name"));
    GemVal _t1293 = (*gem_v_mangle);
    GemVal _t1294[] = {gem_v_name};
        GemVal gem_v_mname = _t1293.fn(_t1293.env, _t1294, 1);
    GemVal _t1295[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1295, 2))) {
    GemVal _t1296 = gem_v_r;
    GemVal _t1297 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1296, gem_string("setup")), gem_v_p), gem_string("GemVal *")), gem_v_mname), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1297, gem_string("expr"))), gem_string(";"));
        }
    GemVal _t1298 = gem_v_r;
    GemVal _t1299 = gem_v_r;
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1298, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1299, gem_string("expr"))), gem_string(";"));
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1300 = gem_v_node;
    GemVal _t1301 = (*gem_v_compile_expr);
    GemVal _t1302[] = {gem_table_get(_t1300, gem_string("value"))};
            GemVal gem_v_r = _t1301.fn(_t1301.env, _t1302, 1);
    GemVal _t1303 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1303, gem_string("name"));
    GemVal _t1304 = (*gem_v_mangle);
    GemVal _t1305[] = {gem_v_name};
            GemVal gem_v_mname = _t1304.fn(_t1304.env, _t1305, 1);
    GemVal _t1306[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1306, 2))) {
    GemVal _t1307 = gem_v_r;
    GemVal _t1308 = gem_v_r;
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1307, gem_string("setup")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1308, gem_string("expr"))), gem_string(";"));
            }
    GemVal _t1309 = gem_v_r;
    GemVal _t1310 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1309, gem_string("setup")), gem_v_p), gem_v_mname), gem_string(" = ")), gem_table_get(_t1310, gem_string("expr"))), gem_string(";"));
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
    GemVal _t1311 = gem_v_node;
    GemVal _t1312 = (*gem_v_compile_expr);
    GemVal _t1313[] = {gem_table_get(_t1311, gem_string("object"))};
                GemVal gem_v_obj_r = _t1312.fn(_t1312.env, _t1313, 1);
    GemVal _t1314 = gem_v_node;
    GemVal _t1315 = (*gem_v_compile_expr);
    GemVal _t1316[] = {gem_table_get(_t1314, gem_string("value"))};
                GemVal gem_v_val_r = _t1315.fn(_t1315.env, _t1316, 1);
    GemVal _t1317 = gem_v_node;
    GemVal _t1318[] = {gem_table_get(_t1317, gem_string("field"))};
                GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t1318, 1);
    GemVal _t1319 = gem_v_obj_r;
    GemVal _t1320 = gem_v_val_r;
    GemVal _t1321 = gem_v_obj_r;
    GemVal _t1322 = gem_v_val_r;
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1319, gem_string("setup")), gem_table_get(_t1320, gem_string("setup"))), gem_v_p), gem_string("gem_table_set(")), gem_table_get(_t1321, gem_string("expr"))), gem_string(", gem_string(\"")), gem_v_escaped), gem_string("\"), ")), gem_table_get(_t1322, gem_string("expr"))), gem_string(");"));
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("index_assign")))) {
    GemVal _t1323 = gem_v_node;
    GemVal _t1324 = (*gem_v_compile_expr);
    GemVal _t1325[] = {gem_table_get(_t1323, gem_string("object"))};
                    GemVal gem_v_obj_r = _t1324.fn(_t1324.env, _t1325, 1);
    GemVal _t1326 = gem_v_node;
    GemVal _t1327 = (*gem_v_compile_expr);
    GemVal _t1328[] = {gem_table_get(_t1326, gem_string("key"))};
                    GemVal gem_v_key_r = _t1327.fn(_t1327.env, _t1328, 1);
    GemVal _t1329 = gem_v_node;
    GemVal _t1330 = (*gem_v_compile_expr);
    GemVal _t1331[] = {gem_table_get(_t1329, gem_string("value"))};
                    GemVal gem_v_val_r = _t1330.fn(_t1330.env, _t1331, 1);
    GemVal _t1332 = gem_v_obj_r;
    GemVal _t1333 = gem_v_key_r;
    GemVal _t1334 = gem_v_val_r;
    GemVal _t1335 = gem_v_obj_r;
    GemVal _t1336 = gem_v_key_r;
    GemVal _t1337 = gem_v_val_r;
                    return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1332, gem_string("setup")), gem_table_get(_t1333, gem_string("setup"))), gem_table_get(_t1334, gem_string("setup"))), gem_v_p), gem_string("gem_table_set(")), gem_table_get(_t1335, gem_string("expr"))), gem_string(", ")), gem_table_get(_t1336, gem_string("expr"))), gem_string(", ")), gem_table_get(_t1337, gem_string("expr"))), gem_string(");"));
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1338 = (*gem_v_compile_if);
    GemVal _t1339[] = {gem_v_node, gem_v_indent};
                        return _t1338.fn(_t1338.env, _t1339, 2);
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1340 = (*gem_v_compile_while);
    GemVal _t1341[] = {gem_v_node, gem_v_indent};
                            return _t1340.fn(_t1340.env, _t1341, 2);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1342 = (*gem_v_compile_match);
    GemVal _t1343[] = {gem_v_node, gem_v_indent};
                                return _t1342.fn(_t1342.env, _t1343, 2);
                            } else {
                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("break")))) {
                                    return gem_add(gem_v_p, gem_string("break;"));
                                } else {
                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("continue")))) {
                                        return gem_add(gem_v_p, gem_string("continue;"));
                                    } else {
                                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1344 = gem_v_node;
                                            if (gem_truthy(gem_neq(gem_table_get(_t1344, gem_string("value")), GEM_NIL))) {
    GemVal _t1345 = gem_v_node;
    GemVal _t1346 = (*gem_v_compile_expr);
    GemVal _t1347[] = {gem_table_get(_t1345, gem_string("value"))};
                                                GemVal gem_v_r = _t1346.fn(_t1346.env, _t1347, 1);
    GemVal _t1348 = gem_v_r;
    GemVal _t1349 = gem_v_r;
                                                return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1348, gem_string("setup")), gem_v_p), gem_string("return ")), gem_table_get(_t1349, gem_string("expr"))), gem_string(";"));
                                            }
                                            return gem_add(gem_v_p, gem_string("return GEM_NIL;"));
                                        } else {
                                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("call")))) {
    GemVal _t1350 = (*gem_v_compile_expr);
    GemVal _t1351[] = {gem_v_node};
                                                GemVal gem_v_r = _t1350.fn(_t1350.env, _t1351, 1);
    GemVal _t1352 = gem_v_r;
    GemVal _t1353 = gem_v_r;
                                                return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1352, gem_string("setup")), gem_v_p), gem_string("(void)(")), gem_table_get(_t1353, gem_string("expr"))), gem_string(");"));
                                            } else {
                                                if (gem_truthy(gem_eq(gem_v_tag, gem_string("fn_def")))) {
                                                    return gem_string("");
                                                } else {
                                                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
                                                        GemVal gem_v_out = gem_add(gem_v_p, gem_string("{\n"));
                                                        {
    GemVal _t1354 = gem_v_node;
                                                            GemVal gem_v__for_items_31 = gem_table_get(_t1354, gem_string("stmts"));
                                                            GemVal gem_v__for_i_31 = gem_int(0);
                                                            while (1) {
                                                                GemVal _t1355[] = {gem_v__for_items_31};
                                                                if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_len_fn(NULL, _t1355, 1)))) break;
                                                                GemVal gem_v_stmt = gem_table_get(gem_v__for_items_31, gem_v__for_i_31);
                                                                gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
    GemVal _t1356 = (*gem_v_compile_stmt);
    GemVal _t1357[] = {gem_v_stmt, gem_add(gem_v_indent, gem_int(1))};
                                                                gem_v_out = gem_add(gem_add(gem_v_out, _t1356.fn(_t1356.env, _t1357, 2)), gem_string("\n"));
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
    GemVal _t1358 = (*gem_v_compile_expr);
    GemVal _t1359[] = {gem_v_node};
    GemVal gem_v_r = _t1358.fn(_t1358.env, _t1359, 1);
    GemVal _t1360 = gem_v_r;
    GemVal _t1361 = gem_v_r;
    return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1360, gem_string("setup")), gem_v_p), gem_string("(void)(")), gem_table_get(_t1361, gem_string("expr"))), gem_string(");"));
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
    GemVal _t1363[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1363, 1);
    GemVal _t1364 = gem_v_node;
    GemVal gem_v_tag = gem_table_get(_t1364, gem_string("tag"));
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("let")))) {
    GemVal _t1365 = gem_v_node;
    GemVal _t1366 = (*gem_v_compile_expr);
    GemVal _t1367[] = {gem_table_get(_t1365, gem_string("value"))};
        GemVal gem_v_r = _t1366.fn(_t1366.env, _t1367, 1);
    GemVal _t1368 = gem_v_node;
        GemVal gem_v_name = gem_table_get(_t1368, gem_string("name"));
    GemVal _t1369 = (*gem_v_mangle);
    GemVal _t1370[] = {gem_v_name};
        GemVal gem_v_mname = _t1369.fn(_t1369.env, _t1370, 1);
    GemVal _t1371[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t1371, 2))) {
    GemVal _t1372 = gem_v_r;
    GemVal _t1373 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1372, gem_string("setup")), gem_v_p), gem_string("GemVal *")), gem_v_mname), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1373, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_string("return GEM_NIL;"));
        }
    GemVal _t1374 = gem_v_r;
    GemVal _t1375 = gem_v_r;
        return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1374, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_mname), gem_string(" = ")), gem_table_get(_t1375, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_string("return GEM_NIL;"));
    } else {
        if (gem_truthy(gem_eq(gem_v_tag, gem_string("assign")))) {
    GemVal _t1376 = gem_v_node;
    GemVal _t1377 = (*gem_v_compile_expr);
    GemVal _t1378[] = {gem_table_get(_t1376, gem_string("value"))};
            GemVal gem_v_r = _t1377.fn(_t1377.env, _t1378, 1);
    GemVal _t1379 = (*gem_v_tmp);
            GemVal gem_v_t = _t1379.fn(_t1379.env, NULL, 0);
    GemVal _t1380 = gem_v_node;
            GemVal gem_v_name = gem_table_get(_t1380, gem_string("name"));
    GemVal _t1381 = (*gem_v_mangle);
    GemVal _t1382[] = {gem_v_name};
            GemVal gem_v_mname = _t1381.fn(_t1381.env, _t1382, 1);
    GemVal _t1383[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1383, 2))) {
    GemVal _t1384 = gem_v_r;
    GemVal _t1385 = gem_v_r;
                return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1384, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1385, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_string("*")), gem_v_mname), gem_string(" = ")), gem_v_t), gem_string(";\n")), gem_v_p), gem_string("return ")), gem_v_t), gem_string(";"));
            }
    GemVal _t1386 = gem_v_r;
    GemVal _t1387 = gem_v_r;
            return gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1386, gem_string("setup")), gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1387, gem_string("expr"))), gem_string(";\n")), gem_v_p), gem_v_mname), gem_string(" = ")), gem_v_t), gem_string(";\n")), gem_v_p), gem_string("return ")), gem_v_t), gem_string(";"));
        } else {
            if (gem_truthy(gem_eq(gem_v_tag, gem_string("return")))) {
    GemVal _t1388 = gem_v_node;
                if (gem_truthy(gem_neq(gem_table_get(_t1388, gem_string("value")), GEM_NIL))) {
    GemVal _t1389 = gem_v_node;
    GemVal _t1390 = (*gem_v_compile_expr);
    GemVal _t1391[] = {gem_table_get(_t1389, gem_string("value"))};
                    GemVal gem_v_r = _t1390.fn(_t1390.env, _t1391, 1);
    GemVal _t1392 = gem_v_r;
    GemVal _t1393 = gem_v_r;
                    return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1392, gem_string("setup")), gem_v_p), gem_string("return ")), gem_table_get(_t1393, gem_string("expr"))), gem_string(";"));
                }
                return gem_add(gem_v_p, gem_string("return GEM_NIL;"));
            } else {
                if (gem_truthy(gem_eq(gem_v_tag, gem_string("if")))) {
    GemVal _t1394 = (*gem_v_compile_if_return);
    GemVal _t1395[] = {gem_v_node, gem_v_indent};
                    return _t1394.fn(_t1394.env, _t1395, 2);
                } else {
                    if (gem_truthy(gem_eq(gem_v_tag, gem_string("while")))) {
    GemVal _t1396 = (*gem_v_compile_while);
    GemVal _t1397[] = {gem_v_node, gem_v_indent};
                        return gem_add(gem_add(gem_add(_t1396.fn(_t1396.env, _t1397, 2), gem_string("\n")), gem_v_p), gem_string("return GEM_NIL;"));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_tag, gem_string("match")))) {
    GemVal _t1398 = (*gem_v_compile_match_return);
    GemVal _t1399[] = {gem_v_node, gem_v_indent};
                            return _t1398.fn(_t1398.env, _t1399, 2);
                        } else {
                            if (gem_truthy(gem_eq(gem_v_tag, gem_string("block")))) {
                                GemVal gem_v_out = gem_add(gem_v_p, gem_string("{\n"));
                                {
                                    GemVal gem_v__for_i_32 = gem_int(0);
    GemVal _t1400 = gem_v_node;
    GemVal _t1401[] = {gem_table_get(_t1400, gem_string("stmts"))};
                                    GemVal gem_v__for_limit_32 = gem_sub(gem_len_fn(NULL, _t1401, 1), gem_int(1));
                                    while (1) {
                                        if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_limit_32))) break;
                                        GemVal gem_v_i = gem_v__for_i_32;
                                        gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
    GemVal _t1402 = gem_v_node;
    GemVal _t1403 = (*gem_v_compile_stmt);
    GemVal _t1404[] = {gem_table_get(gem_table_get(_t1402, gem_string("stmts")), gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                                        gem_v_out = gem_add(gem_add(gem_v_out, _t1403.fn(_t1403.env, _t1404, 2)), gem_string("\n"));
                                    }
                                }
    GemVal _t1405 = gem_v_node;
    GemVal _t1406[] = {gem_table_get(_t1405, gem_string("stmts"))};
                                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1406, 1), gem_int(0)))) {
    GemVal _t1407 = gem_v_node;
    GemVal _t1408 = gem_v_node;
    GemVal _t1409[] = {gem_table_get(_t1408, gem_string("stmts"))};
    GemVal _t1410 = (*gem_v_compile_stmt_return);
    GemVal _t1411[] = {gem_table_get(gem_table_get(_t1407, gem_string("stmts")), gem_sub(gem_len_fn(NULL, _t1409, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                                    gem_v_out = gem_add(gem_add(gem_v_out, _t1410.fn(_t1410.env, _t1411, 2)), gem_string("\n"));
                                }
                                gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("}"));
                                return gem_v_out;
                            } else {
    GemVal _t1412;
    if (gem_truthy(gem_eq(gem_v_tag, gem_string("dot_assign")))) {
        _t1412 = gem_eq(gem_v_tag, gem_string("dot_assign"));
    } else {
        _t1412 = gem_eq(gem_v_tag, gem_string("index_assign"));
    }
    GemVal _t1413;
    if (gem_truthy(_t1412)) {
        _t1413 = _t1412;
    } else {
        _t1413 = gem_eq(gem_v_tag, gem_string("break"));
    }
    GemVal _t1414;
    if (gem_truthy(_t1413)) {
        _t1414 = _t1413;
    } else {
        _t1414 = gem_eq(gem_v_tag, gem_string("continue"));
    }
    GemVal _t1415;
    if (gem_truthy(_t1414)) {
        _t1415 = _t1414;
    } else {
        _t1415 = gem_eq(gem_v_tag, gem_string("fn_def"));
    }
                                if (gem_truthy(_t1415)) {
    GemVal _t1416 = (*gem_v_compile_stmt);
    GemVal _t1417[] = {gem_v_node, gem_v_indent};
                                    return gem_add(gem_add(gem_add(_t1416.fn(_t1416.env, _t1417, 2), gem_string("\n")), gem_v_p), gem_string("return GEM_NIL;"));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t1418 = (*gem_v_compile_expr);
    GemVal _t1419[] = {gem_v_node};
    GemVal gem_v_r = _t1418.fn(_t1418.env, _t1419, 1);
    GemVal _t1420 = gem_v_r;
    GemVal _t1421 = gem_v_r;
    return gem_add(gem_add(gem_add(gem_add(gem_table_get(_t1420, gem_string("setup")), gem_v_p), gem_string("return ")), gem_table_get(_t1421, gem_string("expr"))), gem_string(";"));
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
    GemVal _t1423[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1423, 1);
    GemVal _t1424 = gem_v_node;
    GemVal _t1425 = (*gem_v_compile_expr);
    GemVal _t1426[] = {gem_table_get(_t1424, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1425.fn(_t1425.env, _t1426, 1);
    GemVal _t1427 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1427, gem_string("setup"));
    GemVal _t1428 = gem_v_cond_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("if (gem_truthy(")), gem_table_get(_t1428, gem_string("expr"))), gem_string(")) {\n"));
    GemVal _t1429 = (*gem_v_compile_stmts);
    GemVal _t1430[] = {gem_table_get(gem_v_node, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1429.fn(_t1429.env, _t1430, 2));
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
    GemVal _t1431 = (*gem_v_compile_stmts);
    GemVal _t1432[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1431.fn(_t1431.env, _t1432, 2));
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
    GemVal _t1434[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1434, 1);
    GemVal _t1435 = gem_v_node;
    GemVal _t1436 = (*gem_v_compile_expr);
    GemVal _t1437[] = {gem_table_get(_t1435, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1436.fn(_t1436.env, _t1437, 1);
    GemVal _t1438 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t1438, gem_string("setup"));
    GemVal _t1439 = gem_v_cond_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("if (gem_truthy(")), gem_table_get(_t1439, gem_string("expr"))), gem_string(")) {\n"));
    GemVal gem_v_then_body = gem_table_get(gem_v_node, gem_string("then"));
    GemVal _t1440[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1440, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_33 = gem_int(0);
    GemVal _t1441[] = {gem_v_then_body};
            GemVal gem_v__for_limit_33 = gem_sub(gem_len_fn(NULL, _t1441, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
                GemVal gem_v_i = gem_v__for_i_33;
                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
    GemVal _t1442 = (*gem_v_compile_stmt);
    GemVal _t1443[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1442.fn(_t1442.env, _t1443, 2)), gem_string("\n"));
            }
        }
    GemVal _t1444[] = {gem_v_then_body};
    GemVal _t1445 = (*gem_v_compile_stmt_return);
    GemVal _t1446[] = {gem_table_get(gem_v_then_body, gem_sub(gem_len_fn(NULL, _t1444, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_add(gem_v_out, _t1445.fn(_t1445.env, _t1446, 2)), gem_string("\n"));
    } else {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
    GemVal _t1447[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1447, 1), gem_int(0)))) {
            {
                GemVal gem_v__for_i_34 = gem_int(0);
    GemVal _t1448[] = {gem_v_else_body};
                GemVal gem_v__for_limit_34 = gem_sub(gem_len_fn(NULL, _t1448, 1), gem_int(1));
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
                    GemVal gem_v_i = gem_v__for_i_34;
                    gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
    GemVal _t1449 = (*gem_v_compile_stmt);
    GemVal _t1450[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1449.fn(_t1449.env, _t1450, 2)), gem_string("\n"));
                }
            }
    GemVal _t1451[] = {gem_v_else_body};
    GemVal _t1452 = (*gem_v_compile_stmt_return);
    GemVal _t1453[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1451, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1452.fn(_t1452.env, _t1453, 2)), gem_string("\n"));
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
    GemVal _t1455[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1455, 1);
    GemVal _t1456 = gem_v_node;
    GemVal _t1457 = (*gem_v_compile_expr);
    GemVal _t1458[] = {gem_table_get(_t1456, gem_string("cond"))};
    GemVal gem_v_cond_r = _t1457.fn(_t1457.env, _t1458, 1);
    GemVal gem_v_out = gem_add(gem_v_p, gem_string("while (1) {\n"));
    GemVal _t1459 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t1459, gem_string("setup")), gem_string("")))) {
    GemVal _t1460 = gem_v_cond_r;
    GemVal _t1461[] = {gem_table_get(_t1460, gem_string("setup")), gem_string("    "), gem_add(gem_v_p, gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_fn_str_replace_all(NULL, _t1461, 3));
    }
    GemVal _t1462 = gem_v_cond_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    if (!gem_truthy(")), gem_table_get(_t1462, gem_string("expr"))), gem_string(")) break;\n"));
    GemVal _t1463 = gem_v_node;
    GemVal _t1464 = (*gem_v_compile_stmts);
    GemVal _t1465[] = {gem_table_get(_t1463, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    gem_v_out = gem_add(gem_v_out, _t1464.fn(_t1464.env, _t1465, 2));
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
    GemVal _t1467[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1467, 1);
    GemVal _t1468 = gem_v_node;
    GemVal _t1469 = (*gem_v_compile_expr);
    GemVal _t1470[] = {gem_table_get(_t1468, gem_string("target"))};
    GemVal gem_v_target_r = _t1469.fn(_t1469.env, _t1470, 1);
    GemVal _t1471 = (*gem_v_tmp);
    GemVal gem_v_t = _t1471.fn(_t1471.env, NULL, 0);
    GemVal _t1472 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1472, gem_string("setup"));
    GemVal _t1473 = gem_v_target_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1473, gem_string("expr"))), gem_string(";\n"));
    {
        GemVal gem_v__for_i_35 = gem_int(0);
    GemVal _t1474 = gem_v_node;
    GemVal _t1475[] = {gem_table_get(_t1474, gem_string("whens"))};
        GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t1475, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
            GemVal gem_v_i = gem_v__for_i_35;
            gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
    GemVal _t1476 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1476, gem_string("whens")), gem_v_i);
    GemVal _t1477 = gem_v_w;
    GemVal _t1478 = (*gem_v_compile_expr);
    GemVal _t1479[] = {gem_table_get(_t1477, gem_string("value"))};
            GemVal gem_v_val_r = _t1478.fn(_t1478.env, _t1479, 1);
            GemVal gem_v_keyword = gem_string("if");
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_keyword = gem_string("} else if");
            }
    GemVal _t1480 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1480, gem_string("setup")));
    GemVal _t1481 = gem_v_val_r;
            gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_v_keyword), gem_string(" (gem_truthy(gem_eq(")), gem_v_t), gem_string(", ")), gem_table_get(_t1481, gem_string("expr"))), gem_string("))) {\n"));
    GemVal _t1482 = gem_v_w;
    GemVal _t1483 = (*gem_v_compile_stmts);
    GemVal _t1484[] = {gem_table_get(_t1482, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_v_out, _t1483.fn(_t1483.env, _t1484, 2));
        }
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
    GemVal _t1485 = (*gem_v_compile_stmts);
    GemVal _t1486[] = {gem_table_get(gem_v_node, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
        gem_v_out = gem_add(gem_v_out, _t1485.fn(_t1485.env, _t1486, 2));
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
    GemVal _t1488[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t1488, 1);
    GemVal _t1489 = gem_v_node;
    GemVal _t1490 = (*gem_v_compile_expr);
    GemVal _t1491[] = {gem_table_get(_t1489, gem_string("target"))};
    GemVal gem_v_target_r = _t1490.fn(_t1490.env, _t1491, 1);
    GemVal _t1492 = (*gem_v_tmp);
    GemVal gem_v_t = _t1492.fn(_t1492.env, NULL, 0);
    GemVal _t1493 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t1493, gem_string("setup"));
    GemVal _t1494 = gem_v_target_r;
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_string("GemVal ")), gem_v_t), gem_string(" = ")), gem_table_get(_t1494, gem_string("expr"))), gem_string(";\n"));
    {
        GemVal gem_v__for_i_37 = gem_int(0);
    GemVal _t1495 = gem_v_node;
    GemVal _t1496[] = {gem_table_get(_t1495, gem_string("whens"))};
        GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t1496, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
            GemVal gem_v_i = gem_v__for_i_37;
            gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
    GemVal _t1497 = gem_v_node;
            GemVal gem_v_w = gem_table_get(gem_table_get(_t1497, gem_string("whens")), gem_v_i);
    GemVal _t1498 = gem_v_w;
    GemVal _t1499 = (*gem_v_compile_expr);
    GemVal _t1500[] = {gem_table_get(_t1498, gem_string("value"))};
            GemVal gem_v_val_r = _t1499.fn(_t1499.env, _t1500, 1);
            GemVal gem_v_keyword = gem_string("if");
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
                gem_v_keyword = gem_string("} else if");
            }
    GemVal _t1501 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t1501, gem_string("setup")));
    GemVal _t1502 = gem_v_val_r;
            gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_v_p), gem_v_keyword), gem_string(" (gem_truthy(gem_eq(")), gem_v_t), gem_string(", ")), gem_table_get(_t1502, gem_string("expr"))), gem_string("))) {\n"));
    GemVal _t1503 = gem_v_w;
            GemVal gem_v_body = gem_table_get(_t1503, gem_string("body"));
    GemVal _t1504[] = {gem_v_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1504, 1), gem_int(0)))) {
                {
                    GemVal gem_v__for_i_36 = gem_int(0);
    GemVal _t1505[] = {gem_v_body};
                    GemVal gem_v__for_limit_36 = gem_sub(gem_len_fn(NULL, _t1505, 1), gem_int(1));
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
                        GemVal gem_v_j = gem_v__for_i_36;
                        gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
    GemVal _t1506 = (*gem_v_compile_stmt);
    GemVal _t1507[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
                        gem_v_out = gem_add(gem_add(gem_v_out, _t1506.fn(_t1506.env, _t1507, 2)), gem_string("\n"));
                    }
                }
    GemVal _t1508[] = {gem_v_body};
    GemVal _t1509 = (*gem_v_compile_stmt_return);
    GemVal _t1510[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1508, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
                gem_v_out = gem_add(gem_add(gem_v_out, _t1509.fn(_t1509.env, _t1510, 2)), gem_string("\n"));
            } else {
                gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("    return GEM_NIL;\n"));
            }
        }
    }
    if (gem_truthy(gem_neq(gem_table_get(gem_v_node, gem_string("else")), GEM_NIL))) {
        gem_v_out = gem_add(gem_add(gem_v_out, gem_v_p), gem_string("} else {\n"));
        GemVal gem_v_else_body = gem_table_get(gem_v_node, gem_string("else"));
    GemVal _t1511[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1511, 1), gem_int(0)))) {
            {
                GemVal gem_v__for_i_38 = gem_int(0);
    GemVal _t1512[] = {gem_v_else_body};
                GemVal gem_v__for_limit_38 = gem_sub(gem_len_fn(NULL, _t1512, 1), gem_int(1));
                while (1) {
                    if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
                    GemVal gem_v_i = gem_v__for_i_38;
                    gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
    GemVal _t1513 = (*gem_v_compile_stmt);
    GemVal _t1514[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
                    gem_v_out = gem_add(gem_add(gem_v_out, _t1513.fn(_t1513.env, _t1514, 2)), gem_string("\n"));
                }
            }
    GemVal _t1515[] = {gem_v_else_body};
    GemVal _t1516 = (*gem_v_compile_stmt_return);
    GemVal _t1517[] = {gem_table_get(gem_v_else_body, gem_sub(gem_len_fn(NULL, _t1515, 1), gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
            gem_v_out = gem_add(gem_add(gem_v_out, _t1516.fn(_t1516.env, _t1517, 2)), gem_string("\n"));
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
    GemVal *gem_v_fwd_count;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal *gem_v_fwd_count = _cls->gem_v_fwd_count;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1519 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1519, gem_string("name"));
    GemVal _t1520 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1520, gem_string("params"));
    GemVal _t1521 = gem_v_node;
    GemVal gem_v_ret_type = gem_table_get(_t1521, gem_string("ret_type"));
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_v_name), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    GemVal _t1522 = gem_table_new();
    GemVal gem_v_c_args = _t1522;
    {
        GemVal gem_v__for_i_39 = gem_int(0);
    GemVal _t1523[] = {gem_v_params};
        GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t1523, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
            GemVal gem_v_i = gem_v__for_i_39;
            gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
            GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
            GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
    GemVal _t1524[] = {gem_v_i};
    GemVal _t1525[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    int64_t _p")), gem_to_string_fn(NULL, _t1524, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1525, 1)), gem_string("].ival;\n"));
    GemVal _t1526[] = {gem_v_i};
                gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1526, 1)));
            } else {
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
    GemVal _t1527[] = {gem_v_i};
    GemVal _t1528[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    double _p")), gem_to_string_fn(NULL, _t1527, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1528, 1)), gem_string("].fval;\n"));
    GemVal _t1529[] = {gem_v_i};
                    gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1529, 1)));
                } else {
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
    GemVal _t1530[] = {gem_v_i};
    GemVal _t1531[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    char* _p")), gem_to_string_fn(NULL, _t1530, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1531, 1)), gem_string("].sval;\n"));
    GemVal _t1532[] = {gem_v_i};
                        gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1532, 1)));
                    } else {
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
    GemVal _t1533[] = {gem_v_i};
    GemVal _t1534[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    int _p")), gem_to_string_fn(NULL, _t1533, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t1534, 1)), gem_string("].bval;\n"));
    GemVal _t1535[] = {gem_v_i};
                            gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1535, 1)));
                        } else {
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
    GemVal _t1536[] = {gem_v_i};
    GemVal _t1537[] = {gem_v_i};
                                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    void* _p")), gem_to_string_fn(NULL, _t1536, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t1537, 1)), gem_string("].ival;\n"));
    GemVal _t1538[] = {gem_v_i};
                                gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t1538, 1)));
                            } else {
                                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
    GemVal _t1539[] = {gem_v_i};
                                    gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1539, 1)), gem_string("]")));
                                } else {
    GemVal _t1540[] = {gem_v_i};
                                    gem_table_set(gem_v_c_args, gem_v_i, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t1540, 1)), gem_string("]")));
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
    GemVal _t1541[] = {gem_v_c_args};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t1541, 1);
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
    gem_table_set((*gem_v_forward_decls), (*gem_v_fwd_count), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_v_name), gem_string("(void *_env, GemVal *args, int argc);")));
    *gem_v_fwd_count = gem_add((*gem_v_fwd_count), gem_int(1));
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
    GemVal _t1543 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1543, gem_string("params"));
    GemVal _t1544 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1544, gem_string("body"));
    GemVal _t1545 = gem_v_node;
    GemVal gem_v_name = gem_table_get(_t1545, gem_string("name"));
    GemVal _t1546[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t1546, 1);
    {
        GemVal gem_v__for_i_42 = gem_int(0);
    GemVal _t1547[] = {gem_v_body};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t1547, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
            GemVal gem_v_i = gem_v__for_i_42;
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
    GemVal _t1548[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1550;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1548, 1), gem_string("table")))) {
        _t1550 = gem_eq(gem_type_fn(NULL, _t1548, 1), gem_string("table"));
    } else {
        GemVal _t1549 = gem_table_get(gem_v_body, gem_v_i);
        _t1550 = gem_eq(gem_table_get(_t1549, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1550)) {
    GemVal _t1551 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1552[] = {gem_v_scope_vars, gem_table_get(_t1551, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1552, 2));
            }
    GemVal _t1553[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1555;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1553, 1), gem_string("table")))) {
        _t1555 = gem_eq(gem_type_fn(NULL, _t1553, 1), gem_string("table"));
    } else {
        GemVal _t1554 = gem_table_get(gem_v_body, gem_v_i);
        _t1555 = gem_eq(gem_table_get(_t1554, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1555)) {
                {
                    GemVal gem_v__for_i_41 = gem_int(0);
    GemVal _t1556 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1557[] = {gem_table_get(_t1556, gem_string("stmts"))};
                    GemVal gem_v__for_limit_41 = gem_len_fn(NULL, _t1557, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_limit_41))) break;
                        GemVal gem_v_j = gem_v__for_i_41;
                        gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
    GemVal _t1558 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1559[] = {gem_table_get(gem_table_get(_t1558, gem_string("stmts")), gem_v_j)};
    GemVal _t1562;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1559, 1), gem_string("table")))) {
        _t1562 = gem_eq(gem_type_fn(NULL, _t1559, 1), gem_string("table"));
    } else {
        GemVal _t1560 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1561 = gem_table_get(gem_table_get(_t1560, gem_string("stmts")), gem_v_j);
        _t1562 = gem_eq(gem_table_get(_t1561, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1562)) {
    GemVal _t1563 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1564 = gem_table_get(gem_table_get(_t1563, gem_string("stmts")), gem_v_j);
    GemVal _t1565[] = {gem_v_scope_vars, gem_table_get(_t1564, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1565, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1566 = (*gem_v_find_captured_in_scope);
    GemVal _t1567[] = {gem_v_body, gem_v_scope_vars};
    GemVal gem_v_captured = _t1566.fn(_t1566.env, _t1567, 2);
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    *gem_v_boxed_vars = gem_v_captured;
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_v_name), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    {
        GemVal gem_v__for_i_43 = gem_int(0);
    GemVal _t1568[] = {gem_v_params};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t1568, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
            GemVal gem_v_i = gem_v__for_i_43;
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
    GemVal _t1569 = (*gem_v_mangle);
    GemVal _t1570[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1569.fn(_t1569.env, _t1570, 1);
    GemVal _t1571[] = {gem_v_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1571, 2))) {
                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal *")), gem_v_mp), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"));
    GemVal _t1572[] = {gem_v_i};
    GemVal _t1573[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    *")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1572, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1573, 1)), gem_string("] : GEM_NIL;\n"));
            } else {
    GemVal _t1574[] = {gem_v_i};
    GemVal _t1575[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal ")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1574, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1575, 1)), gem_string("] : GEM_NIL;\n"));
            }
        }
    }
    GemVal _t1576[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1576, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_44 = gem_int(0);
    GemVal _t1577[] = {gem_v_body};
            GemVal gem_v__for_limit_44 = gem_sub(gem_len_fn(NULL, _t1577, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
                GemVal gem_v_i = gem_v__for_i_44;
                gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
    GemVal _t1578 = (*gem_v_compile_stmt);
    GemVal _t1579[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1578.fn(_t1578.env, _t1579, 2)), gem_string("\n"));
            }
        }
    GemVal _t1580[] = {gem_v_body};
    GemVal _t1581 = (*gem_v_compile_stmt_return);
    GemVal _t1582[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1580, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1581.fn(_t1581.env, _t1582, 2)), gem_string("\n"));
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
    GemVal *gem_v_fn_count;
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
    GemVal *gem_v_fn_count = _cls->gem_v_fn_count;
    GemVal *gem_v_functions = _cls->gem_v_functions;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t1584 = gem_v_node;
    GemVal gem_v_params = gem_table_get(_t1584, gem_string("params"));
    GemVal _t1585 = gem_v_node;
    GemVal gem_v_body = gem_table_get(_t1585, gem_string("body"));
    GemVal _t1586 = (*gem_v_anon_name);
    GemVal gem_v_name = _t1586.fn(_t1586.env, NULL, 0);
    GemVal _t1587[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t1587, 1);
    {
        GemVal gem_v__for_i_46 = gem_int(0);
    GemVal _t1588[] = {gem_v_body};
        GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t1588, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
            GemVal gem_v_i = gem_v__for_i_46;
            gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
    GemVal _t1589[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1591;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1589, 1), gem_string("table")))) {
        _t1591 = gem_eq(gem_type_fn(NULL, _t1589, 1), gem_string("table"));
    } else {
        GemVal _t1590 = gem_table_get(gem_v_body, gem_v_i);
        _t1591 = gem_eq(gem_table_get(_t1590, gem_string("tag")), gem_string("let"));
    }
            if (gem_truthy(_t1591)) {
    GemVal _t1592 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1593[] = {gem_v_inner_scope, gem_table_get(_t1592, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1593, 2));
            }
    GemVal _t1594[] = {gem_table_get(gem_v_body, gem_v_i)};
    GemVal _t1596;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1594, 1), gem_string("table")))) {
        _t1596 = gem_eq(gem_type_fn(NULL, _t1594, 1), gem_string("table"));
    } else {
        GemVal _t1595 = gem_table_get(gem_v_body, gem_v_i);
        _t1596 = gem_eq(gem_table_get(_t1595, gem_string("tag")), gem_string("block"));
    }
            if (gem_truthy(_t1596)) {
                {
                    GemVal gem_v__for_i_45 = gem_int(0);
    GemVal _t1597 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1598[] = {gem_table_get(_t1597, gem_string("stmts"))};
                    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t1598, 1);
                    while (1) {
                        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
                        GemVal gem_v_j = gem_v__for_i_45;
                        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
    GemVal _t1599 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1600[] = {gem_table_get(gem_table_get(_t1599, gem_string("stmts")), gem_v_j)};
    GemVal _t1603;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1600, 1), gem_string("table")))) {
        _t1603 = gem_eq(gem_type_fn(NULL, _t1600, 1), gem_string("table"));
    } else {
        GemVal _t1601 = gem_table_get(gem_v_body, gem_v_i);
        GemVal _t1602 = gem_table_get(gem_table_get(_t1601, gem_string("stmts")), gem_v_j);
        _t1603 = gem_eq(gem_table_get(_t1602, gem_string("tag")), gem_string("let"));
    }
                        if (gem_truthy(_t1603)) {
    GemVal _t1604 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1605 = gem_table_get(gem_table_get(_t1604, gem_string("stmts")), gem_v_j);
    GemVal _t1606[] = {gem_v_inner_scope, gem_table_get(_t1605, gem_string("name"))};
                            (void)(gem_fn_set_add(NULL, _t1606, 2));
                        }
                    }
                }
            }
        }
    }
    GemVal _t1607 = (*gem_v_find_captured_in_scope);
    GemVal _t1608[] = {gem_v_body, gem_v_inner_scope};
    GemVal gem_v_inner_captured = _t1607.fn(_t1607.env, _t1608, 2);
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    GemVal _t1609[] = {gem_v_captures};
    GemVal _t1610[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t1609, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t1610, 2);
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_v_name);
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_v_struct_name), gem_string(" {\n"));
    {
        GemVal gem_v__for_items_47 = gem_v_captures;
        GemVal gem_v__for_i_47 = gem_int(0);
        while (1) {
            GemVal _t1611[] = {gem_v__for_items_47};
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_len_fn(NULL, _t1611, 1)))) break;
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_47, gem_v__for_i_47);
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
    GemVal _t1612 = (*gem_v_mangle);
    GemVal _t1613[] = {gem_v_cap};
            gem_v_struct_def = gem_add(gem_add(gem_add(gem_v_struct_def, gem_string("    GemVal *")), _t1612.fn(_t1612.env, _t1613, 1)), gem_string(";\n"));
        }
    }
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_v_name), gem_string("(void *_env, GemVal *args, int argc) {\n"));
    GemVal _t1614[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1614, 1), gem_int(0)))) {
        gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    struct ")), gem_v_struct_name), gem_string(" *_cls = (struct ")), gem_v_struct_name), gem_string(" *)_env;\n"));
        {
            GemVal gem_v__for_items_48 = gem_v_captures;
            GemVal gem_v__for_i_48 = gem_int(0);
            while (1) {
                GemVal _t1615[] = {gem_v__for_items_48};
                if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_len_fn(NULL, _t1615, 1)))) break;
                GemVal gem_v_cap = gem_table_get(gem_v__for_items_48, gem_v__for_i_48);
                gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
    GemVal _t1616 = (*gem_v_mangle);
    GemVal _t1617[] = {gem_v_cap};
                GemVal gem_v_mc = _t1616.fn(_t1616.env, _t1617, 1);
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal *")), gem_v_mc), gem_string(" = _cls->")), gem_v_mc), gem_string(";\n"));
            }
        }
    }
    {
        GemVal gem_v__for_i_49 = gem_int(0);
    GemVal _t1618[] = {gem_v_params};
        GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t1618, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
            GemVal gem_v_i = gem_v__for_i_49;
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
    GemVal _t1619 = (*gem_v_mangle);
    GemVal _t1620[] = {gem_table_get(gem_v_params, gem_v_i)};
            GemVal gem_v_mp = _t1619.fn(_t1619.env, _t1620, 1);
    GemVal _t1621[] = {gem_v_inner_captured, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn_set_contains(NULL, _t1621, 2))) {
                gem_v_lines = gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal *")), gem_v_mp), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"));
    GemVal _t1622[] = {gem_v_i};
    GemVal _t1623[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    *")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1622, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1623, 1)), gem_string("] : GEM_NIL;\n"));
            } else {
    GemVal _t1624[] = {gem_v_i};
    GemVal _t1625[] = {gem_v_i};
                gem_v_lines = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_lines, gem_string("    GemVal ")), gem_v_mp), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t1624, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t1625, 1)), gem_string("] : GEM_NIL;\n"));
            }
        }
    }
    GemVal _t1626[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t1626, 1), gem_int(0)))) {
        {
            GemVal gem_v__for_i_50 = gem_int(0);
    GemVal _t1627[] = {gem_v_body};
            GemVal gem_v__for_limit_50 = gem_sub(gem_len_fn(NULL, _t1627, 1), gem_int(1));
            while (1) {
                if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
                GemVal gem_v_i = gem_v__for_i_50;
                gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
    GemVal _t1628 = (*gem_v_compile_stmt);
    GemVal _t1629[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
                gem_v_lines = gem_add(gem_add(gem_v_lines, _t1628.fn(_t1628.env, _t1629, 2)), gem_string("\n"));
            }
        }
    GemVal _t1630[] = {gem_v_body};
    GemVal _t1631 = (*gem_v_compile_stmt_return);
    GemVal _t1632[] = {gem_table_get(gem_v_body, gem_sub(gem_len_fn(NULL, _t1630, 1), gem_int(1))), gem_int(1)};
        gem_v_lines = gem_add(gem_add(gem_v_lines, _t1631.fn(_t1631.env, _t1632, 2)), gem_string("\n"));
    } else {
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    }
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
    *gem_v_boxed_vars = gem_v_saved_boxed;
    gem_table_set((*gem_v_functions), (*gem_v_fn_count), gem_add(gem_v_struct_def, gem_v_lines));
    *gem_v_fn_count = gem_add((*gem_v_fn_count), gem_int(1));
    GemVal _t1633 = gem_table_new();
    gem_table_set(_t1633, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t1633, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t1633, gem_string("captures"), gem_v_captures);
    return _t1633;
}

struct _closure__anon_47 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_extern_fn;
    GemVal *gem_v_compile_fn;
    GemVal *gem_v_compile_stmts;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_fn_count;
    GemVal *gem_v_forward_decls;
    GemVal *gem_v_functions;
    GemVal *gem_v_fwd_count;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_extern_fn = _cls->gem_v_compile_extern_fn;
    GemVal *gem_v_compile_fn = _cls->gem_v_compile_fn;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_fn_count = _cls->gem_v_fn_count;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal *gem_v_functions = _cls->gem_v_functions;
    GemVal *gem_v_fwd_count = _cls->gem_v_fwd_count;
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal _t1635 = gem_table_new();
    GemVal gem_v_fn_defs = _t1635;
    GemVal gem_v_fn_def_count = gem_int(0);
    GemVal _t1636 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t1636;
    GemVal gem_v_extern_fn_count = gem_int(0);
    GemVal _t1637 = gem_table_new();
    GemVal gem_v_extern_includes = _t1637;
    GemVal gem_v_extern_inc_count = gem_int(0);
    GemVal _t1638 = gem_table_new();
    GemVal gem_v_top_stmts = _t1638;
    GemVal gem_v_top_count = gem_int(0);
    {
    GemVal _t1639 = gem_v_ast;
        GemVal gem_v__for_items_51 = gem_table_get(_t1639, gem_string("stmts"));
        GemVal gem_v__for_i_51 = gem_int(0);
        while (1) {
            GemVal _t1640[] = {gem_v__for_items_51};
            if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t1640, 1)))) break;
            GemVal gem_v_s = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
            gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
    GemVal _t1641 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t1641, gem_string("tag")), gem_string("fn_def")))) {
                gem_table_set(gem_v_fn_defs, gem_v_fn_def_count, gem_v_s);
                gem_v_fn_def_count = gem_add(gem_v_fn_def_count, gem_int(1));
            } else {
    GemVal _t1642 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t1642, gem_string("tag")), gem_string("extern_fn")))) {
                    gem_table_set(gem_v_extern_fns_list, gem_v_extern_fn_count, gem_v_s);
                    gem_v_extern_fn_count = gem_add(gem_v_extern_fn_count, gem_int(1));
                } else {
    GemVal _t1643 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t1643, gem_string("tag")), gem_string("extern_include")))) {
                        gem_table_set(gem_v_extern_includes, gem_v_extern_inc_count, gem_v_s);
                        gem_v_extern_inc_count = gem_add(gem_v_extern_inc_count, gem_int(1));
                    } else {
                        gem_table_set(gem_v_top_stmts, gem_v_top_count, gem_v_s);
                        gem_v_top_count = gem_add(gem_v_top_count, gem_int(1));
                    }
                }
            }
        }
    }
    {
        GemVal gem_v__for_items_52 = gem_v_extern_fns_list;
        GemVal gem_v__for_i_52 = gem_int(0);
        while (1) {
            GemVal _t1644[] = {gem_v__for_items_52};
            if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_len_fn(NULL, _t1644, 1)))) break;
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_52, gem_v__for_i_52);
            gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
    GemVal _t1645 = gem_v_ef;
    GemVal _t1646[] = {(*gem_v_defined_fns), gem_table_get(_t1645, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1646, 2));
        }
    }
    {
        GemVal gem_v__for_items_53 = gem_v_fn_defs;
        GemVal gem_v__for_i_53 = gem_int(0);
        while (1) {
            GemVal _t1647[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t1647, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
    GemVal _t1648 = gem_v_fd;
    GemVal _t1649[] = {(*gem_v_defined_fns), gem_table_get(_t1648, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1649, 2));
    GemVal _t1650 = gem_v_fd;
            gem_table_set((*gem_v_forward_decls), (*gem_v_fwd_count), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_table_get(_t1650, gem_string("name"))), gem_string("(void *_env, GemVal *args, int argc);")));
            *gem_v_fwd_count = gem_add((*gem_v_fwd_count), gem_int(1));
        }
    }
    {
        GemVal gem_v__for_items_54 = gem_v_extern_fns_list;
        GemVal gem_v__for_i_54 = gem_int(0);
        while (1) {
            GemVal _t1651[] = {gem_v__for_items_54};
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t1651, 1)))) break;
            GemVal gem_v_ef = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
    GemVal _t1652 = (*gem_v_compile_extern_fn);
    GemVal _t1653[] = {gem_v_ef};
            gem_table_set((*gem_v_functions), (*gem_v_fn_count), _t1652.fn(_t1652.env, _t1653, 1));
            *gem_v_fn_count = gem_add((*gem_v_fn_count), gem_int(1));
        }
    }
    {
        GemVal gem_v__for_items_55 = gem_v_fn_defs;
        GemVal gem_v__for_i_55 = gem_int(0);
        while (1) {
            GemVal _t1654[] = {gem_v__for_items_55};
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t1654, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
    GemVal _t1655 = (*gem_v_compile_fn);
    GemVal _t1656[] = {gem_v_fd};
            gem_table_set((*gem_v_functions), (*gem_v_fn_count), _t1655.fn(_t1655.env, _t1656, 1));
            *gem_v_fn_count = gem_add((*gem_v_fn_count), gem_int(1));
        }
    }
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
    GemVal _t1657 = gem_table_new();
    GemVal _t1658 = (*gem_v_find_captured_in_scope);
    GemVal _t1659[] = {gem_v_top_stmts, _t1657};
    *gem_v_boxed_vars = _t1658.fn(_t1658.env, _t1659, 2);
    GemVal _t1660 = (*gem_v_compile_stmts);
    GemVal _t1661[] = {gem_v_top_stmts, gem_int(1)};
    GemVal gem_v_main_body = _t1660.fn(_t1660.env, _t1661, 2);
    *gem_v_boxed_vars = gem_v_saved_boxed;
    GemVal gem_v_has_main = gem_bool(0);
    {
        GemVal gem_v__for_items_56 = gem_v_fn_defs;
        GemVal gem_v__for_i_56 = gem_int(0);
        while (1) {
            GemVal _t1662[] = {gem_v__for_items_56};
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_len_fn(NULL, _t1662, 1)))) break;
            GemVal gem_v_fd = gem_table_get(gem_v__for_items_56, gem_v__for_i_56);
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
    GemVal _t1663 = gem_v_fd;
            if (gem_truthy(gem_eq(gem_table_get(_t1663, gem_string("name")), gem_string("main")))) {
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
            GemVal _t1664[] = {gem_v__for_items_57};
            if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t1664, 1)))) break;
            GemVal gem_v_ei = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
            gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
    GemVal _t1665 = gem_v_ei;
            gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string("#include \"")), gem_table_get(_t1665, gem_string("path"))), gem_string("\"\n"));
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
    {
        GemVal gem_v__for_i_58 = gem_int(0);
        GemVal gem_v__for_limit_58 = (*gem_v_fwd_count);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
            GemVal gem_v_i = gem_v__for_i_58;
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
            if (gem_truthy(gem_lt(gem_v_i, gem_sub((*gem_v_fwd_count), gem_int(1))))) {
                gem_v_out = gem_add(gem_v_out, gem_string("\n"));
            }
        }
    }
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
    {
        GemVal gem_v__for_i_59 = gem_int(0);
        GemVal gem_v__for_limit_59 = (*gem_v_fn_count);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
            GemVal gem_v_i = gem_v__for_i_59;
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
            gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
            if (gem_truthy(gem_lt(gem_v_i, gem_sub((*gem_v_fn_count), gem_int(1))))) {
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
    GemVal _t707 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t707;
    GemVal *gem_v_fn_count = GC_MALLOC(sizeof(GemVal));
    *gem_v_fn_count = gem_int(0);
    GemVal _t708 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t708;
    GemVal *gem_v_fwd_count = GC_MALLOC(sizeof(GemVal));
    *gem_v_fwd_count = gem_int(0);
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
    GemVal _t709 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t709;
    GemVal _t710 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t710;
    gem_table_set((*gem_v_builtins), gem_string("print"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("error"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("len"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("type"), gem_bool(1));
    gem_table_set((*gem_v_builtins), gem_string("to_string"), gem_bool(1));
    GemVal _t711 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t711;
    struct _closure__anon_22 *_t713 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t713->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_22, _t713);
    struct _closure__anon_23 *_t715 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t715->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_23, _t715);
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_24, NULL);
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
    struct _closure__anon_25 *_t872 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t872->gem_v_builtins = gem_v_builtins;
    _t872->gem_v_collect_free_node = gem_v_collect_free_node;
    _t872->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t872->gem_v_defined_fns = gem_v_defined_fns;
    *gem_v_collect_free_node = gem_make_fn(_anon_25, _t872);
    struct _closure__anon_26 *_t897 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t897->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_26, _t897);
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
    struct _closure__anon_27 *_t969 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t969->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t969->gem_v_walk_captures = gem_v_walk_captures;
    _t969->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_27, _t969);
    struct _closure__anon_28 *_t993 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t993->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_28, _t993);
    struct _closure__anon_29 *_t997 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t997->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_29, _t997);
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
    struct _closure__anon_30 *_t1067 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t1067->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1067->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t1067->gem_v_compile_array = gem_v_compile_array;
    _t1067->gem_v_compile_binop = gem_v_compile_binop;
    _t1067->gem_v_compile_call = gem_v_compile_call;
    _t1067->gem_v_compile_expr = gem_v_compile_expr;
    _t1067->gem_v_compile_table = gem_v_compile_table;
    _t1067->gem_v_defined_fns = gem_v_defined_fns;
    _t1067->gem_v_mangle = gem_v_mangle;
    _t1067->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_expr = gem_make_fn(_anon_30, _t1067);
    struct _closure__anon_31 *_t1079 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t1079->gem_v_compile_expr = gem_v_compile_expr;
    _t1079->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_31, _t1079);
    struct _closure__anon_32 *_t1089 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t1089->gem_v_compile_expr = gem_v_compile_expr;
    _t1089->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_32, _t1089);
    struct _closure__anon_33 *_t1113 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t1113->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1113->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1113->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t1113->gem_v_mangle = gem_v_mangle;
    _t1113->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_33, _t1113);
    struct _closure__anon_34 *_t1186 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t1186->gem_v_compile_expr = gem_v_compile_expr;
    _t1186->gem_v_defined_fns = gem_v_defined_fns;
    _t1186->gem_v_source_name = gem_v_source_name;
    _t1186->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_34, _t1186);
    struct _closure__anon_35 *_t1282 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t1282->gem_v_compile_expr = gem_v_compile_expr;
    _t1282->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_35, _t1282);
    struct _closure__anon_36 *_t1286 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t1286->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_36, _t1286);
    struct _closure__anon_37 *_t1362 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t1362->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1362->gem_v_compile_expr = gem_v_compile_expr;
    _t1362->gem_v_compile_if = gem_v_compile_if;
    _t1362->gem_v_compile_match = gem_v_compile_match;
    _t1362->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1362->gem_v_compile_while = gem_v_compile_while;
    _t1362->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_stmt = gem_make_fn(_anon_37, _t1362);
    struct _closure__anon_38 *_t1422 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t1422->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1422->gem_v_compile_expr = gem_v_compile_expr;
    _t1422->gem_v_compile_if_return = gem_v_compile_if_return;
    _t1422->gem_v_compile_match_return = gem_v_compile_match_return;
    _t1422->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1422->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1422->gem_v_compile_while = gem_v_compile_while;
    _t1422->gem_v_mangle = gem_v_mangle;
    _t1422->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_38, _t1422);
    struct _closure__anon_39 *_t1433 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t1433->gem_v_compile_expr = gem_v_compile_expr;
    _t1433->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_39, _t1433);
    struct _closure__anon_40 *_t1454 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t1454->gem_v_compile_expr = gem_v_compile_expr;
    _t1454->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1454->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_40, _t1454);
    struct _closure__anon_41 *_t1466 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t1466->gem_v_compile_expr = gem_v_compile_expr;
    _t1466->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_41, _t1466);
    struct _closure__anon_42 *_t1487 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t1487->gem_v_compile_expr = gem_v_compile_expr;
    _t1487->gem_v_compile_stmts = gem_v_compile_stmts;
    _t1487->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_42, _t1487);
    struct _closure__anon_43 *_t1518 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t1518->gem_v_compile_expr = gem_v_compile_expr;
    _t1518->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1518->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1518->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_43, _t1518);
    struct _closure__anon_44 *_t1542 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t1542->gem_v_forward_decls = gem_v_forward_decls;
    _t1542->gem_v_fwd_count = gem_v_fwd_count;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_44, _t1542);
    struct _closure__anon_45 *_t1583 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t1583->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1583->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1583->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1583->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1583->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_fn = gem_make_fn(_anon_45, _t1583);
    struct _closure__anon_46 *_t1634 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t1634->gem_v_anon_name = gem_v_anon_name;
    _t1634->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1634->gem_v_compile_stmt = gem_v_compile_stmt;
    _t1634->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t1634->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1634->gem_v_fn_count = gem_v_fn_count;
    _t1634->gem_v_functions = gem_v_functions;
    _t1634->gem_v_mangle = gem_v_mangle;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_46, _t1634);
    struct _closure__anon_47 *_t1666 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t1666->gem_v_boxed_vars = gem_v_boxed_vars;
    _t1666->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t1666->gem_v_compile_fn = gem_v_compile_fn;
    _t1666->gem_v_compile_stmts = gem_v_compile_stmts;
    _t1666->gem_v_defined_fns = gem_v_defined_fns;
    _t1666->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t1666->gem_v_fn_count = gem_v_fn_count;
    _t1666->gem_v_forward_decls = gem_v_forward_decls;
    _t1666->gem_v_functions = gem_v_functions;
    _t1666->gem_v_fwd_count = gem_v_fwd_count;
    GemVal gem_v_compile = gem_make_fn(_anon_47, _t1666);
    GemVal _t1667 = gem_table_new();
    gem_table_set(_t1667, gem_string("compile"), gem_v_compile);
    return _t1667;
}

static GemVal gem_fn_split_string(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    GemVal _t1668 = gem_table_new();
    GemVal gem_v_result = _t1668;
    GemVal gem_v_count = gem_int(0);
    GemVal gem_v_current = gem_string("");
    {
        GemVal gem_v__for_i_60 = gem_int(0);
    GemVal _t1669[] = {gem_v_s};
        GemVal gem_v__for_limit_60 = gem_len_fn(NULL, _t1669, 1);
        while (1) {
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
            GemVal gem_v_i = gem_v__for_i_60;
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
            if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_v_delim))) {
                gem_table_set(gem_v_result, gem_v_count, gem_v_current);
                gem_v_count = gem_add(gem_v_count, gem_int(1));
                gem_v_current = gem_string("");
            } else {
                gem_v_current = gem_add(gem_v_current, gem_table_get(gem_v_s, gem_v_i));
            }
        }
    }
    gem_table_set(gem_v_result, gem_v_count, gem_v_current);
    return gem_v_result;
}

static GemVal gem_fn_str_replace_all(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_result = gem_string("");
    GemVal gem_v_i = gem_int(0);
    GemVal _t1670[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t1670, 1);
    GemVal _t1671[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t1671, 1);
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
    GemVal _t1672 = gem_table_new();
    GemVal gem_v_new_stmts = _t1672;
    GemVal gem_v_count = gem_int(0);
    {
    GemVal _t1673 = gem_v_ast;
        GemVal gem_v__for_items_1 = gem_table_get(_t1673, gem_string("stmts"));
        GemVal gem_v__for_i_1 = gem_int(0);
        while (1) {
            GemVal _t1674[] = {gem_v__for_items_1};
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t1674, 1)))) break;
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
    GemVal _t1675[] = {gem_v_stmt};
    GemVal _t1677;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1675, 1), gem_string("table")))) {
        _t1677 = gem_eq(gem_type_fn(NULL, _t1675, 1), gem_string("table"));
    } else {
        GemVal _t1676 = gem_v_stmt;
        _t1677 = gem_eq(gem_table_get(_t1676, gem_string("tag")), gem_string("load"));
    }
            if (gem_truthy(_t1677)) {
    GemVal _t1678 = gem_v_stmt;
                GemVal gem_v_path = gem_table_get(_t1678, gem_string("path"));
    GemVal _t1679[] = {gem_v_path};
    GemVal _t1687;
    if (gem_truthy(gem_lt(gem_len_fn(NULL, _t1679, 1), gem_int(4)))) {
        _t1687 = gem_lt(gem_len_fn(NULL, _t1679, 1), gem_int(4));
    } else {
        GemVal _t1680[] = {gem_v_path};
        GemVal _t1682;
        if (gem_truthy(gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1680, 1), gem_int(4))), gem_string(".")))) {
                _t1682 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1680, 1), gem_int(4))), gem_string("."));
        } else {
                GemVal _t1681[] = {gem_v_path};
                _t1682 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1681, 1), gem_int(3))), gem_string("g"));
        }
        GemVal _t1684;
        if (gem_truthy(_t1682)) {
                _t1684 = _t1682;
        } else {
                GemVal _t1683[] = {gem_v_path};
                _t1684 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1683, 1), gem_int(2))), gem_string("e"));
        }
        GemVal _t1686;
        if (gem_truthy(_t1684)) {
                _t1686 = _t1684;
        } else {
                GemVal _t1685[] = {gem_v_path};
                _t1686 = gem_neq(gem_table_get(gem_v_path, gem_sub(gem_len_fn(NULL, _t1685, 1), gem_int(1))), gem_string("m"));
        }
        _t1687 = _t1686;
    }
                if (gem_truthy(_t1687)) {
                    gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
                }
    GemVal _t1688[] = {gem_v_base_dir, gem_v_path};
    GemVal _t1689[] = {gem_fn_gem_path_join(NULL, _t1688, 2)};
                GemVal gem_v_full_path = gem_fn_gem_normalize_path(NULL, _t1689, 1);
                if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                    continue;
                }
                gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
    GemVal _t1690[] = {gem_v_full_path};
                GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t1690, 1);
                if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
    GemVal _t1691[] = {gem_add(gem_add(gem_string("cannot load '"), gem_v_full_path), gem_string("'"))};
                    (void)(gem_error_at_fn("compiler/main.gem", 39, _t1691, 1));
                }
    GemVal _t1692[] = {gem_v_source};
                GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t1692, 1);
    GemVal _t1693[] = {gem_v_full_path};
                GemVal gem_v_loaded_dir = gem_fn_gem_dirname(NULL, _t1693, 1);
    GemVal _t1694[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
                GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t1694, 3);
                {
    GemVal _t1695 = gem_v_resolved;
                    GemVal gem_v__for_items_2 = gem_table_get(_t1695, gem_string("stmts"));
                    GemVal gem_v__for_i_2 = gem_int(0);
                    while (1) {
                        GemVal _t1696[] = {gem_v__for_items_2};
                        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t1696, 1)))) break;
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
    GemVal _t1697[] = {gem_v_new_stmts};
    return gem_fn_make_program(NULL, _t1697, 1);
}

int main(void) {
    GemVal gem_v_argc = gem_fn_gem_get_argc(NULL, NULL, 0);
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
    GemVal _t1698[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 62, _t1698, 1));
    }
    GemVal _t1699[] = {gem_int(1)};
    GemVal gem_v_src_path = gem_fn_gem_get_argv(NULL, _t1699, 1);
    GemVal _t1700[] = {gem_v_src_path};
    GemVal gem_v_source = gem_fn_gem_read_file(NULL, _t1700, 1);
    if (gem_truthy(gem_eq(gem_v_source, gem_string("")))) {
    GemVal _t1701[] = {gem_add(gem_add(gem_string("cannot read '"), gem_v_src_path), gem_string("'"))};
        (void)(gem_error_at_fn("compiler/main.gem", 68, _t1701, 1));
    }
    GemVal _t1702[] = {gem_v_source};
    GemVal gem_v_ast = gem_fn_parse_source(NULL, _t1702, 1);
    GemVal _t1703[] = {gem_v_src_path};
    GemVal gem_v_base_dir = gem_fn_gem_dirname(NULL, _t1703, 1);
    GemVal _t1704 = gem_table_new();
    GemVal gem_v_loaded = _t1704;
    GemVal _t1705[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    GemVal gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t1705, 3);
    GemVal _t1706[] = {gem_v_src_path};
    GemVal gem_v_cg = gem_fn_make_codegen(NULL, _t1706, 1);
    GemVal _t1707 = gem_v_cg;
    GemVal _t1708 = gem_table_get(_t1707, gem_string("compile"));
    GemVal _t1709[] = {gem_v_resolved_ast};
    GemVal gem_v_c_code = _t1708.fn(_t1708.env, _t1709, 1);
    GemVal _t1710[] = {gem_v_c_code};
    (void)(gem_fn_puts(NULL, _t1710, 1));
    return 0;
}

