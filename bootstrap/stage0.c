#include "gem.h"
double atof(const char*);
static GemVal gem_fn__mod_string_str_eq_at(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_split(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_index_of(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_join(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_trim(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_starts_with(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_ends_with(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_upper(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_lower(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_contains(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_string_repeat(void *_env, GemVal *args, int argc);
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
static GemVal gem_fn_make_export(void *_env, GemVal *args, int argc);
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
static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc);
static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc);
static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc);
static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc);
static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn_rename_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn_rename_stmts(void *_env, GemVal *args, int argc);
static GemVal gem_fn_transform_module(void *_env, GemVal *args, int argc);
static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc);
static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc);
static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc);
static GemVal gem_fn_atof(void *_env, GemVal *args, int argc);
GemVal gem_v_string;
GemVal gem_v_install_root;
GemVal gem_v_argc;
GemVal gem_v_cli;
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

static GemVal gem_fn__mod_string_str_eq_at(void *_env, GemVal *args, int argc) {
#line 3 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_pos = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_needle = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_string_str_eq_at", "compiler/main.gem", 3);
#line 4 "compiler/main.gem"
    GemVal _t1[] = {gem_v_needle};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t1, 1);
#line 5 "compiler/main.gem"
    GemVal _t2[] = {gem_v_s};
    if (gem_truthy(gem_gt(gem_add(gem_v_pos, gem_v_nlen), gem_len_fn(NULL, _t2, 1)))) {
#line 6 "compiler/main.gem"
        GemVal _t3 = gem_bool(0);
        gem_pop_frame();
        return _t3;
    }
#line 8 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 9 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_nlen))) break;
#line 10 "compiler/main.gem"
    GemVal _t4[] = {gem_v_s, gem_add(gem_v_pos, gem_v_i)};
    GemVal _t5[] = {gem_v_needle, gem_v_i};
        if (gem_truthy(gem_neq(gem_ord_fn(NULL, _t4, 2), gem_ord_fn(NULL, _t5, 2)))) {
#line 11 "compiler/main.gem"
            GemVal _t6 = gem_bool(0);
            gem_pop_frame();
            return _t6;
        }
#line 13 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t7 = gem_bool(1);
    gem_pop_frame();
    return _t7;
}

static GemVal gem_fn__mod_string_split(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_split", "compiler/main.gem", 18);
#line 19 "compiler/main.gem"
    GemVal _t8 = gem_table_new();
    GemVal gem_v_result = _t8;
#line 20 "compiler/main.gem"
    GemVal _t9[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t9, 1);
#line 21 "compiler/main.gem"
    GemVal _t10[] = {gem_v_delim};
    GemVal gem_v_dlen = gem_len_fn(NULL, _t10, 1);
#line 22 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_dlen, gem_int(0)))) {
#line 23 "compiler/main.gem"
    GemVal _t11[] = {gem_v_result, gem_v_s};
        (void)(gem_push_fn(NULL, _t11, 2));
#line 24 "compiler/main.gem"
        GemVal _t12 = gem_v_result;
        gem_pop_frame();
        return _t12;
    }
#line 26 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 27 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 28 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) break;
#line 29 "compiler/main.gem"
    GemVal _t13[] = {gem_v_s, gem_v_i, gem_v_delim};
        if (gem_truthy(gem_fn__mod_string_str_eq_at(NULL, _t13, 3))) {
#line 30 "compiler/main.gem"
    GemVal _t14[] = {gem_v_buf};
    GemVal _t15[] = {gem_v_result, gem_buf_str_fn(NULL, _t14, 1)};
            (void)(gem_push_fn(NULL, _t15, 2));
#line 31 "compiler/main.gem"
            gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 32 "compiler/main.gem"
            gem_v_i = gem_add(gem_v_i, gem_v_dlen);
        } else {
#line 34 "compiler/main.gem"
    GemVal _t16[] = {gem_v_s, gem_v_i};
    GemVal _t17[] = {gem_ord_fn(NULL, _t16, 2)};
    GemVal _t18[] = {gem_v_buf, gem_chr_fn(NULL, _t17, 1)};
            (void)(gem_buf_push_fn(NULL, _t18, 2));
#line 35 "compiler/main.gem"
            gem_v_i = gem_add(gem_v_i, gem_int(1));
        }
    }
#line 38 "compiler/main.gem"
    GemVal _t19[] = {gem_v_buf};
    GemVal _t20[] = {gem_v_result, gem_buf_str_fn(NULL, _t19, 1)};
    (void)(gem_push_fn(NULL, _t20, 2));
    GemVal _t21 = gem_v_result;
    gem_pop_frame();
    return _t21;
}

static GemVal gem_fn__mod_string_index_of(void *_env, GemVal *args, int argc) {
#line 42 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_needle = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_index_of", "compiler/main.gem", 42);
#line 43 "compiler/main.gem"
    GemVal _t22[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t22, 1);
#line 44 "compiler/main.gem"
    GemVal _t23[] = {gem_v_needle};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t23, 1);
#line 45 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_nlen, gem_int(0)))) {
#line 46 "compiler/main.gem"
        GemVal _t24 = gem_int(0);
        gem_pop_frame();
        return _t24;
    }
#line 48 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_v_slen))) {
#line 49 "compiler/main.gem"
        GemVal _t25 = gem_neg(gem_int(1));
        gem_pop_frame();
        return _t25;
    }
#line 51 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 52 "compiler/main.gem"
    GemVal gem_v_limit = gem_sub(gem_v_slen, gem_v_nlen);
#line 53 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_le(gem_v_i, gem_v_limit))) break;
#line 54 "compiler/main.gem"
    GemVal _t26[] = {gem_v_s, gem_v_i, gem_v_needle};
        if (gem_truthy(gem_fn__mod_string_str_eq_at(NULL, _t26, 3))) {
#line 55 "compiler/main.gem"
            GemVal _t27 = gem_v_i;
            gem_pop_frame();
            return _t27;
        }
#line 57 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t28 = gem_neg(gem_int(1));
    gem_pop_frame();
    return _t28;
}

static GemVal gem_fn__mod_string_join(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_join", "compiler/main.gem", 62);
#line 63 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 64 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 65 "compiler/main.gem"
    GemVal _t29[] = {gem_v_arr};
    GemVal gem_v_n = gem_len_fn(NULL, _t29, 1);
#line 66 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_n))) break;
#line 67 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 68 "compiler/main.gem"
    GemVal _t30[] = {gem_v_buf, gem_v_delim};
            (void)(gem_buf_push_fn(NULL, _t30, 2));
        }
#line 70 "compiler/main.gem"
    GemVal _t31[] = {gem_v_buf, gem_table_get(gem_v_arr, gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t31, 2));
#line 71 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 73 "compiler/main.gem"
    GemVal _t32[] = {gem_v_buf};
    GemVal _t33 = gem_buf_str_fn(NULL, _t32, 1);
    gem_pop_frame();
    return _t33;
}

static GemVal gem_fn__mod_string_trim(void *_env, GemVal *args, int argc) {
#line 76 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_string_trim", "compiler/main.gem", 76);
#line 77 "compiler/main.gem"
    GemVal _t34[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t34, 1);
#line 78 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_slen, gem_int(0)))) {
#line 79 "compiler/main.gem"
        GemVal _t35 = gem_v_s;
        gem_pop_frame();
        return _t35;
    }
#line 81 "compiler/main.gem"
    GemVal gem_v_start = gem_int(0);
#line 82 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_start, gem_v_slen))) break;
#line 83 "compiler/main.gem"
    GemVal _t36[] = {gem_v_s, gem_v_start};
        GemVal gem_v_c = gem_ord_fn(NULL, _t36, 2);
#line 84 "compiler/main.gem"
    GemVal _t37;
    if (!gem_truthy(gem_neq(gem_v_c, gem_int(32)))) {
        _t37 = gem_neq(gem_v_c, gem_int(32));
    } else {
        _t37 = gem_neq(gem_v_c, gem_int(9));
    }
    GemVal _t38;
    if (!gem_truthy(_t37)) {
        _t38 = _t37;
    } else {
        _t38 = gem_neq(gem_v_c, gem_int(10));
    }
    GemVal _t39;
    if (!gem_truthy(_t38)) {
        _t39 = _t38;
    } else {
        _t39 = gem_neq(gem_v_c, gem_int(13));
    }
        if (gem_truthy(_t39)) {
            break;
        }
#line 87 "compiler/main.gem"
        gem_v_start = gem_add(gem_v_start, gem_int(1));
    }
#line 89 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_start, gem_v_slen))) {
#line 90 "compiler/main.gem"
        GemVal _t40 = gem_string("");
        gem_pop_frame();
        return _t40;
    }
#line 92 "compiler/main.gem"
    GemVal gem_v_stop = gem_sub(gem_v_slen, gem_int(1));
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_stop, gem_v_start))) break;
#line 94 "compiler/main.gem"
    GemVal _t41[] = {gem_v_s, gem_v_stop};
        GemVal gem_v_c = gem_ord_fn(NULL, _t41, 2);
#line 95 "compiler/main.gem"
    GemVal _t42;
    if (!gem_truthy(gem_neq(gem_v_c, gem_int(32)))) {
        _t42 = gem_neq(gem_v_c, gem_int(32));
    } else {
        _t42 = gem_neq(gem_v_c, gem_int(9));
    }
    GemVal _t43;
    if (!gem_truthy(_t42)) {
        _t43 = _t42;
    } else {
        _t43 = gem_neq(gem_v_c, gem_int(10));
    }
    GemVal _t44;
    if (!gem_truthy(_t43)) {
        _t44 = _t43;
    } else {
        _t44 = gem_neq(gem_v_c, gem_int(13));
    }
        if (gem_truthy(_t44)) {
            break;
        }
#line 98 "compiler/main.gem"
        gem_v_stop = gem_sub(gem_v_stop, gem_int(1));
    }
#line 100 "compiler/main.gem"
    GemVal _t45[] = {gem_v_s, gem_v_start, gem_add(gem_sub(gem_v_stop, gem_v_start), gem_int(1))};
    GemVal _t46 = gem_substr_fn(NULL, _t45, 3);
    gem_pop_frame();
    return _t46;
}

static GemVal gem_fn__mod_string_starts_with(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_starts_with", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal _t47[] = {gem_v_s, gem_int(0), gem_v_prefix};
    GemVal _t48 = gem_fn__mod_string_str_eq_at(NULL, _t47, 3);
    gem_pop_frame();
    return _t48;
}

static GemVal gem_fn__mod_string_ends_with(void *_env, GemVal *args, int argc) {
#line 107 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_suffix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_ends_with", "compiler/main.gem", 107);
#line 108 "compiler/main.gem"
    GemVal _t49[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t49, 1);
#line 109 "compiler/main.gem"
    GemVal _t50[] = {gem_v_suffix};
    GemVal gem_v_plen = gem_len_fn(NULL, _t50, 1);
#line 110 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_plen, gem_v_slen))) {
#line 111 "compiler/main.gem"
        GemVal _t51 = gem_bool(0);
        gem_pop_frame();
        return _t51;
    }
#line 113 "compiler/main.gem"
    GemVal _t52[] = {gem_v_s, gem_sub(gem_v_slen, gem_v_plen), gem_v_suffix};
    GemVal _t53 = gem_fn__mod_string_str_eq_at(NULL, _t52, 3);
    gem_pop_frame();
    return _t53;
}

static GemVal gem_fn__mod_string_upper(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_string_upper", "compiler/main.gem", 116);
#line 117 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 118 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 118 "compiler/main.gem"
    GemVal _t54[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t54, 1);
#line 118 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 118 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 118 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 119 "compiler/main.gem"
    GemVal _t55[] = {gem_v_s, gem_v_i};
        GemVal gem_v_c = gem_ord_fn(NULL, _t55, 2);
#line 120 "compiler/main.gem"
    GemVal _t56;
    if (!gem_truthy(gem_ge(gem_v_c, gem_int(97)))) {
        _t56 = gem_ge(gem_v_c, gem_int(97));
    } else {
        _t56 = gem_le(gem_v_c, gem_int(122));
    }
        if (gem_truthy(_t56)) {
#line 121 "compiler/main.gem"
    GemVal _t57[] = {gem_sub(gem_v_c, gem_int(32))};
    GemVal _t58[] = {gem_v_buf, gem_chr_fn(NULL, _t57, 1)};
            (void)(gem_buf_push_fn(NULL, _t58, 2));
        } else {
#line 123 "compiler/main.gem"
    GemVal _t59[] = {gem_v_c};
    GemVal _t60[] = {gem_v_buf, gem_chr_fn(NULL, _t59, 1)};
            (void)(gem_buf_push_fn(NULL, _t60, 2));
        }
    }

#line 126 "compiler/main.gem"
    GemVal _t61[] = {gem_v_buf};
    GemVal _t62 = gem_buf_str_fn(NULL, _t61, 1);
    gem_pop_frame();
    return _t62;
}

static GemVal gem_fn__mod_string_lower(void *_env, GemVal *args, int argc) {
#line 129 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_string_lower", "compiler/main.gem", 129);
#line 130 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 131 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t63[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t63, 1);
#line 131 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 131 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 131 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 132 "compiler/main.gem"
    GemVal _t64[] = {gem_v_s, gem_v_i};
        GemVal gem_v_c = gem_ord_fn(NULL, _t64, 2);
#line 133 "compiler/main.gem"
    GemVal _t65;
    if (!gem_truthy(gem_ge(gem_v_c, gem_int(65)))) {
        _t65 = gem_ge(gem_v_c, gem_int(65));
    } else {
        _t65 = gem_le(gem_v_c, gem_int(90));
    }
        if (gem_truthy(_t65)) {
#line 134 "compiler/main.gem"
    GemVal _t66[] = {gem_add(gem_v_c, gem_int(32))};
    GemVal _t67[] = {gem_v_buf, gem_chr_fn(NULL, _t66, 1)};
            (void)(gem_buf_push_fn(NULL, _t67, 2));
        } else {
#line 136 "compiler/main.gem"
    GemVal _t68[] = {gem_v_c};
    GemVal _t69[] = {gem_v_buf, gem_chr_fn(NULL, _t68, 1)};
            (void)(gem_buf_push_fn(NULL, _t69, 2));
        }
    }

#line 139 "compiler/main.gem"
    GemVal _t70[] = {gem_v_buf};
    GemVal _t71 = gem_buf_str_fn(NULL, _t70, 1);
    gem_pop_frame();
    return _t71;
}

static GemVal gem_fn__mod_string_contains(void *_env, GemVal *args, int argc) {
#line 142 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_needle = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_contains", "compiler/main.gem", 142);
    GemVal _t72[] = {gem_v_s, gem_v_needle};
    GemVal _t73 = gem_ge(gem_fn__mod_string_index_of(NULL, _t72, 2), gem_int(0));
    gem_pop_frame();
    return _t73;
}

static GemVal gem_fn__mod_string_repeat(void *_env, GemVal *args, int argc) {
#line 146 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_repeat", "compiler/main.gem", 146);
#line 147 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 148 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 148 "compiler/main.gem"
    GemVal gem_v__for_limit_3 = gem_v_n;
#line 148 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 148 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_3;
#line 148 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 149 "compiler/main.gem"
    GemVal _t74[] = {gem_v_buf, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t74, 2));
    }

#line 151 "compiler/main.gem"
    GemVal _t75[] = {gem_v_buf};
    GemVal _t76 = gem_buf_str_fn(NULL, _t75, 1);
    gem_pop_frame();
    return _t76;
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 8 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_program", "compiler/main.gem", 8);
    GemVal _t77 = gem_table_new();
    gem_table_set(_t77, gem_string("tag"), gem_string("program"));
    gem_table_set(_t77, gem_string("stmts"), gem_v_stmts);
    GemVal _t78 = _t77;
    gem_pop_frame();
    return _t78;
}

static GemVal gem_fn_make_int(void *_env, GemVal *args, int argc) {
#line 14 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_int", "compiler/main.gem", 14);
    GemVal _t79 = gem_table_new();
    gem_table_set(_t79, gem_string("tag"), gem_string("int"));
    gem_table_set(_t79, gem_string("value"), gem_v_value);
    GemVal _t80 = _t79;
    gem_pop_frame();
    return _t80;
}

static GemVal gem_fn_make_float(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_float", "compiler/main.gem", 18);
    GemVal _t81 = gem_table_new();
    gem_table_set(_t81, gem_string("tag"), gem_string("float"));
    gem_table_set(_t81, gem_string("value"), gem_v_value);
    GemVal _t82 = _t81;
    gem_pop_frame();
    return _t82;
}

static GemVal gem_fn_make_string(void *_env, GemVal *args, int argc) {
#line 22 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_string", "compiler/main.gem", 22);
    GemVal _t83 = gem_table_new();
    gem_table_set(_t83, gem_string("tag"), gem_string("string"));
    gem_table_set(_t83, gem_string("value"), gem_v_value);
    GemVal _t84 = _t83;
    gem_pop_frame();
    return _t84;
}

static GemVal gem_fn_make_bool(void *_env, GemVal *args, int argc) {
#line 26 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_bool", "compiler/main.gem", 26);
    GemVal _t85 = gem_table_new();
    gem_table_set(_t85, gem_string("tag"), gem_string("bool"));
    gem_table_set(_t85, gem_string("value"), gem_v_value);
    GemVal _t86 = _t85;
    gem_pop_frame();
    return _t86;
}

static GemVal gem_fn_make_nil_node(void *_env, GemVal *args, int argc) {
#line 30 "compiler/main.gem"
    gem_push_frame("make_nil_node", "compiler/main.gem", 30);
    GemVal _t87 = gem_table_new();
    gem_table_set(_t87, gem_string("tag"), gem_string("nil"));
    GemVal _t88 = _t87;
    gem_pop_frame();
    return _t88;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 34 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 34);
    GemVal _t89 = gem_table_new();
    gem_table_set(_t89, gem_string("tag"), gem_string("var"));
    gem_table_set(_t89, gem_string("name"), gem_v_name);
    GemVal _t90 = _t89;
    gem_pop_frame();
    return _t90;
}

static GemVal gem_fn_make_binop(void *_env, GemVal *args, int argc) {
#line 38 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_left = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_right = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_binop", "compiler/main.gem", 38);
    GemVal _t91 = gem_table_new();
    gem_table_set(_t91, gem_string("tag"), gem_string("binop"));
    gem_table_set(_t91, gem_string("op"), gem_v_op);
    gem_table_set(_t91, gem_string("left"), gem_v_left);
    gem_table_set(_t91, gem_string("right"), gem_v_right);
    GemVal _t92 = _t91;
    gem_pop_frame();
    return _t92;
}

static GemVal gem_fn_make_unop(void *_env, GemVal *args, int argc) {
#line 42 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_expr = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_unop", "compiler/main.gem", 42);
    GemVal _t93 = gem_table_new();
    gem_table_set(_t93, gem_string("tag"), gem_string("unop"));
    gem_table_set(_t93, gem_string("op"), gem_v_op);
    gem_table_set(_t93, gem_string("expr"), gem_v_expr);
    GemVal _t94 = _t93;
    gem_pop_frame();
    return _t94;
}

static GemVal gem_fn_make_call(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_func = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_args = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_call", "compiler/main.gem", 46);
    GemVal _t95 = gem_table_new();
    gem_table_set(_t95, gem_string("tag"), gem_string("call"));
    gem_table_set(_t95, gem_string("func"), gem_v_func);
    gem_table_set(_t95, gem_string("args"), gem_v_args);
    gem_table_set(_t95, gem_string("line"), gem_v_line);
    GemVal _t96 = _t95;
    gem_pop_frame();
    return _t96;
}

static GemVal gem_fn_make_dot(void *_env, GemVal *args, int argc) {
#line 50 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_dot", "compiler/main.gem", 50);
    GemVal _t97 = gem_table_new();
    gem_table_set(_t97, gem_string("tag"), gem_string("dot"));
    gem_table_set(_t97, gem_string("object"), gem_v_object);
    gem_table_set(_t97, gem_string("field"), gem_v_field);
    GemVal _t98 = _t97;
    gem_pop_frame();
    return _t98;
}

static GemVal gem_fn_make_index(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_index", "compiler/main.gem", 54);
    GemVal _t99 = gem_table_new();
    gem_table_set(_t99, gem_string("tag"), gem_string("index"));
    gem_table_set(_t99, gem_string("object"), gem_v_object);
    gem_table_set(_t99, gem_string("key"), gem_v_key);
    GemVal _t100 = _t99;
    gem_pop_frame();
    return _t100;
}

static GemVal gem_fn_make_anon_fn(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_params = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rest_param = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_block_param = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_body = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_anon_fn", "compiler/main.gem", 58);
    GemVal _t101 = gem_table_new();
    gem_table_set(_t101, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t101, gem_string("params"), gem_v_params);
    gem_table_set(_t101, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t101, gem_string("block_param"), gem_v_block_param);
    gem_table_set(_t101, gem_string("body"), gem_v_body);
    GemVal _t102 = _t101;
    gem_pop_frame();
    return _t102;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 62);
    GemVal _t103 = gem_table_new();
    gem_table_set(_t103, gem_string("tag"), gem_string("table"));
    gem_table_set(_t103, gem_string("entries"), gem_v_entries);
    GemVal _t104 = _t103;
    gem_pop_frame();
    return _t104;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 66);
    GemVal _t105 = gem_table_new();
    gem_table_set(_t105, gem_string("key"), gem_v_key);
    gem_table_set(_t105, gem_string("value"), gem_v_value);
    GemVal _t106 = _t105;
    gem_pop_frame();
    return _t106;
}

static GemVal gem_fn_make_array(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_elements = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_array", "compiler/main.gem", 70);
    GemVal _t107 = gem_table_new();
    gem_table_set(_t107, gem_string("tag"), gem_string("array"));
    gem_table_set(_t107, gem_string("elements"), gem_v_elements);
    GemVal _t108 = _t107;
    gem_pop_frame();
    return _t108;
}

static GemVal gem_fn_make_interp(void *_env, GemVal *args, int argc) {
#line 74 "compiler/main.gem"
    GemVal gem_v_parts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_interp", "compiler/main.gem", 74);
    GemVal _t109 = gem_table_new();
    gem_table_set(_t109, gem_string("tag"), gem_string("interp"));
    gem_table_set(_t109, gem_string("parts"), gem_v_parts);
    GemVal _t110 = _t109;
    gem_pop_frame();
    return _t110;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 80);
    GemVal _t111 = gem_table_new();
    gem_table_set(_t111, gem_string("tag"), gem_string("let"));
    gem_table_set(_t111, gem_string("name"), gem_v_name);
    gem_table_set(_t111, gem_string("value"), gem_v_value);
    gem_table_set(_t111, gem_string("line"), gem_v_line);
    GemVal _t112 = _t111;
    gem_pop_frame();
    return _t112;
}

static GemVal gem_fn_make_assign(void *_env, GemVal *args, int argc) {
#line 84 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_assign", "compiler/main.gem", 84);
    GemVal _t113 = gem_table_new();
    gem_table_set(_t113, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t113, gem_string("name"), gem_v_name);
    gem_table_set(_t113, gem_string("value"), gem_v_value);
    gem_table_set(_t113, gem_string("line"), gem_v_line);
    GemVal _t114 = _t113;
    gem_pop_frame();
    return _t114;
}

static GemVal gem_fn_make_dot_assign(void *_env, GemVal *args, int argc) {
#line 88 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_dot_assign", "compiler/main.gem", 88);
    GemVal _t115 = gem_table_new();
    gem_table_set(_t115, gem_string("tag"), gem_string("dot_assign"));
    gem_table_set(_t115, gem_string("object"), gem_v_object);
    gem_table_set(_t115, gem_string("field"), gem_v_field);
    gem_table_set(_t115, gem_string("value"), gem_v_value);
    gem_table_set(_t115, gem_string("line"), gem_v_line);
    GemVal _t116 = _t115;
    gem_pop_frame();
    return _t116;
}

static GemVal gem_fn_make_index_assign(void *_env, GemVal *args, int argc) {
#line 92 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_index_assign", "compiler/main.gem", 92);
    GemVal _t117 = gem_table_new();
    gem_table_set(_t117, gem_string("tag"), gem_string("index_assign"));
    gem_table_set(_t117, gem_string("object"), gem_v_object);
    gem_table_set(_t117, gem_string("key"), gem_v_key);
    gem_table_set(_t117, gem_string("value"), gem_v_value);
    gem_table_set(_t117, gem_string("line"), gem_v_line);
    GemVal _t118 = _t117;
    gem_pop_frame();
    return _t118;
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
    GemVal _t119 = gem_table_new();
    gem_table_set(_t119, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t119, gem_string("name"), gem_v_name);
    gem_table_set(_t119, gem_string("params"), gem_v_params);
    gem_table_set(_t119, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t119, gem_string("block_param"), gem_v_block_param);
    gem_table_set(_t119, gem_string("body"), gem_v_body);
    gem_table_set(_t119, gem_string("line"), gem_v_line);
    GemVal _t120 = _t119;
    gem_pop_frame();
    return _t120;
}

static GemVal gem_fn_make_if(void *_env, GemVal *args, int argc) {
#line 100 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_if", "compiler/main.gem", 100);
    GemVal _t121 = gem_table_new();
    gem_table_set(_t121, gem_string("tag"), gem_string("if"));
    gem_table_set(_t121, gem_string("cond"), gem_v_cond);
    gem_table_set(_t121, gem_string("then"), gem_v_then_body);
    gem_table_set(_t121, gem_string("else"), gem_v_else_body);
    gem_table_set(_t121, gem_string("line"), gem_v_line);
    GemVal _t122 = _t121;
    gem_pop_frame();
    return _t122;
}

static GemVal gem_fn_make_while(void *_env, GemVal *args, int argc) {
#line 104 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_while", "compiler/main.gem", 104);
    GemVal _t123 = gem_table_new();
    gem_table_set(_t123, gem_string("tag"), gem_string("while"));
    gem_table_set(_t123, gem_string("cond"), gem_v_cond);
    gem_table_set(_t123, gem_string("body"), gem_v_body);
    gem_table_set(_t123, gem_string("line"), gem_v_line);
    GemVal _t124 = _t123;
    gem_pop_frame();
    return _t124;
}

static GemVal gem_fn_make_match(void *_env, GemVal *args, int argc) {
#line 108 "compiler/main.gem"
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_target_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("make_match", "compiler/main.gem", 108);
    GemVal _t125 = gem_table_new();
    gem_table_set(_t125, gem_string("tag"), gem_string("match"));
    gem_table_set(_t125, gem_string("target"), gem_v_target);
    gem_table_set(_t125, gem_string("whens"), gem_v_whens);
    gem_table_set(_t125, gem_string("else"), gem_v_else_body);
    gem_table_set(_t125, gem_string("line"), gem_v_line);
    gem_table_set(_t125, gem_string("target_var"), gem_v_target_var);
    GemVal _t126 = _t125;
    gem_pop_frame();
    return _t126;
}

static GemVal gem_fn_make_when(void *_env, GemVal *args, int argc) {
#line 112 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_bindings = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_when", "compiler/main.gem", 112);
    GemVal _t127 = gem_table_new();
    gem_table_set(_t127, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t127, gem_string("value"), gem_v_value);
    gem_table_set(_t127, gem_string("body"), gem_v_body);
    gem_table_set(_t127, gem_string("bindings"), gem_v_bindings);
    GemVal _t128 = _t127;
    gem_pop_frame();
    return _t128;
}

static GemVal gem_fn_make_receive_match(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_arms = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_after_ms = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_after_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_recv_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("make_receive_match", "compiler/main.gem", 116);
    GemVal _t129 = gem_table_new();
    gem_table_set(_t129, gem_string("tag"), gem_string("receive_match"));
    gem_table_set(_t129, gem_string("arms"), gem_v_arms);
    gem_table_set(_t129, gem_string("after_ms"), gem_v_after_ms);
    gem_table_set(_t129, gem_string("after_body"), gem_v_after_body);
    gem_table_set(_t129, gem_string("line"), gem_v_line);
    gem_table_set(_t129, gem_string("recv_var"), gem_v_recv_var);
    GemVal _t130 = _t129;
    gem_pop_frame();
    return _t130;
}

static GemVal gem_fn_make_return(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_return", "compiler/main.gem", 120);
    GemVal _t131 = gem_table_new();
    gem_table_set(_t131, gem_string("tag"), gem_string("return"));
    gem_table_set(_t131, gem_string("value"), gem_v_value);
    gem_table_set(_t131, gem_string("line"), gem_v_line);
    GemVal _t132 = _t131;
    gem_pop_frame();
    return _t132;
}

static GemVal gem_fn_make_break(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    gem_push_frame("make_break", "compiler/main.gem", 124);
    GemVal _t133 = gem_table_new();
    gem_table_set(_t133, gem_string("tag"), gem_string("break"));
    GemVal _t134 = _t133;
    gem_pop_frame();
    return _t134;
}

static GemVal gem_fn_make_continue(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    gem_push_frame("make_continue", "compiler/main.gem", 128);
    GemVal _t135 = gem_table_new();
    gem_table_set(_t135, gem_string("tag"), gem_string("continue"));
    GemVal _t136 = _t135;
    gem_pop_frame();
    return _t136;
}

static GemVal gem_fn_make_load(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_load", "compiler/main.gem", 132);
    GemVal _t137 = gem_table_new();
    gem_table_set(_t137, gem_string("tag"), gem_string("load"));
    gem_table_set(_t137, gem_string("path"), gem_v_path);
    GemVal _t138 = _t137;
    gem_pop_frame();
    return _t138;
}

static GemVal gem_fn_make_export(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_names = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_export", "compiler/main.gem", 136);
    GemVal _t139 = gem_table_new();
    gem_table_set(_t139, gem_string("tag"), gem_string("export"));
    gem_table_set(_t139, gem_string("names"), gem_v_names);
    GemVal _t140 = _t139;
    gem_pop_frame();
    return _t140;
}

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_is_blocking = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("make_extern_fn", "compiler/main.gem", 140);
    GemVal _t141 = gem_table_new();
    gem_table_set(_t141, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t141, gem_string("name"), gem_v_name);
    gem_table_set(_t141, gem_string("params"), gem_v_params);
    gem_table_set(_t141, gem_string("ret_type"), gem_v_ret_type);
    gem_table_set(_t141, gem_string("blocking"), gem_v_is_blocking);
    GemVal _t142 = _t141;
    gem_pop_frame();
    return _t142;
}

static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_extern_param", "compiler/main.gem", 144);
    GemVal _t143 = gem_table_new();
    gem_table_set(_t143, gem_string("name"), gem_v_name);
    gem_table_set(_t143, gem_string("type"), gem_v_type_name);
    GemVal _t144 = _t143;
    gem_pop_frame();
    return _t144;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
#line 148 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_extern_include", "compiler/main.gem", 148);
    GemVal _t145 = gem_table_new();
    gem_table_set(_t145, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t145, gem_string("path"), gem_v_path);
    GemVal _t146 = _t145;
    gem_pop_frame();
    return _t146;
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
    GemVal _t147[] = {gem_v_source};
    GemVal gem_v_slen = gem_len_fn(NULL, _t147, 1);
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
                GemVal _t148;
                if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) {
                                _t148 = gem_lt(gem_v_i, gem_v_slen);
                } else {
                                _t148 = gem_neq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n"));
                }
                if (!gem_truthy(_t148)) break;
#line 11 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 13 "compiler/main.gem"
    GemVal _t149[] = {gem_v_source, gem_v_start, gem_sub(gem_v_i, gem_v_start)};
            GemVal _t150 = gem_substr_fn(NULL, _t149, 3);
            gem_pop_frame();
            return _t150;
        }
#line 15 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n")))) {
#line 16 "compiler/main.gem"
            gem_v_current = gem_add(gem_v_current, gem_int(1));
        }
#line 18 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t151 = gem_string("");
    gem_pop_frame();
    return _t151;
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

    GemVal _t152 = gem_v_result;
    gem_pop_frame();
    return _t152;
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
    GemVal _t153;
    if (gem_truthy(gem_eq(gem_v_source, GEM_NIL))) {
        _t153 = gem_eq(gem_v_source, GEM_NIL);
    } else {
        _t153 = gem_eq(gem_v_line, GEM_NIL);
    }
    if (gem_truthy(_t153)) {
#line 36 "compiler/main.gem"
    GemVal _t154[] = {gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"))};
        (void)(gem_eprint_fn(NULL, _t154, 1));
#line 37 "compiler/main.gem"
    GemVal _t155[] = {gem_int(1)};
        (void)(gem_exit_process_fn(NULL, _t155, 1));
    }
#line 39 "compiler/main.gem"
    GemVal _t156;
    if (gem_truthy(gem_eq(gem_v_col, GEM_NIL))) {
        _t156 = gem_eq(gem_v_col, GEM_NIL);
    } else {
        _t156 = gem_lt(gem_v_col, gem_int(1));
    }
    if (gem_truthy(_t156)) {
#line 40 "compiler/main.gem"
        gem_v_col = gem_int(1);
    }
#line 43 "compiler/main.gem"
    GemVal _t157[] = {gem_v_line};
    GemVal gem_v_line_str = gem_to_string_fn(NULL, _t157, 1);
#line 44 "compiler/main.gem"
    GemVal _t158[] = {gem_v_line_str};
    GemVal gem_v_gutter_width = gem_len_fn(NULL, _t158, 1);
#line 45 "compiler/main.gem"
    GemVal _t159[] = {gem_string(" "), gem_v_gutter_width};
    GemVal gem_v_padding = gem_fn_repeat_char(NULL, _t159, 2);
#line 46 "compiler/main.gem"
    GemVal _t160[] = {gem_v_source, gem_v_line};
    GemVal gem_v_src_line = gem_fn_get_source_line(NULL, _t160, 2);
#line 48 "compiler/main.gem"
    GemVal gem_v_actual_span = gem_v_span_len;
#line 49 "compiler/main.gem"
    GemVal _t161;
    if (gem_truthy(gem_eq(gem_v_actual_span, GEM_NIL))) {
        _t161 = gem_eq(gem_v_actual_span, GEM_NIL);
    } else {
        _t161 = gem_lt(gem_v_actual_span, gem_int(1));
    }
    if (gem_truthy(_t161)) {
#line 50 "compiler/main.gem"
        gem_v_actual_span = gem_int(1);
    }
#line 53 "compiler/main.gem"
    GemVal gem_v_out = gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"));
#line 54 "compiler/main.gem"
    GemVal _t162[] = {gem_v_col};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string("  --> ")), gem_v_file), gem_string(":")), gem_v_line_str), gem_string(":")), gem_to_string_fn(NULL, _t162, 1)), gem_string("\n"));
#line 55 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 56 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_line_str), gem_string(" | ")), gem_v_src_line), gem_string("\n"));
#line 57 "compiler/main.gem"
    GemVal _t163[] = {gem_string(" "), gem_sub(gem_v_col, gem_int(1))};
    GemVal _t164[] = {gem_string("^"), gem_v_actual_span};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" | ")), gem_fn_repeat_char(NULL, _t163, 2)), gem_fn_repeat_char(NULL, _t164, 2)), gem_string("\n"));
#line 58 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 60 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_hint, GEM_NIL))) {
#line 61 "compiler/main.gem"
        gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string("Hint: ")), gem_v_hint), gem_string("\n"));
    }
#line 64 "compiler/main.gem"
    GemVal _t165[] = {gem_v_out};
    (void)(gem_eprint_fn(NULL, _t165, 1));
#line 65 "compiler/main.gem"
    GemVal _t166[] = {gem_int(1)};
    GemVal _t167 = gem_exit_process_fn(NULL, _t166, 1);
    gem_pop_frame();
    return _t167;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
#line 15 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_digit", "compiler/main.gem", 15);
    GemVal _t168;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t168 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t168 = gem_le(gem_v_ch, gem_string("9"));
    }
    GemVal _t169 = _t168;
    gem_pop_frame();
    return _t169;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alpha", "compiler/main.gem", 19);
    GemVal _t170;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t170 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t170 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t172;
    if (gem_truthy(_t170)) {
        _t172 = _t170;
    } else {
        GemVal _t171;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t171 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t171 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t172 = _t171;
    }
    GemVal _t173;
    if (gem_truthy(_t172)) {
        _t173 = _t172;
    } else {
        _t173 = gem_eq(gem_v_ch, gem_string("_"));
    }
    GemVal _t174 = _t173;
    gem_pop_frame();
    return _t174;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alnum", "compiler/main.gem", 23);
    GemVal _t175[] = {gem_v_ch};
    GemVal _t177;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t175, 1))) {
        _t177 = gem_fn_is_alpha(NULL, _t175, 1);
    } else {
        GemVal _t176[] = {gem_v_ch};
        _t177 = gem_fn_is_digit(NULL, _t176, 1);
    }
    GemVal _t178 = _t177;
    gem_pop_frame();
    return _t178;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
#line 29 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("digit_val", "compiler/main.gem", 29);
#line 30 "compiler/main.gem"
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t179 = gem_int(0);
        gem_pop_frame();
        return _t179;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t180 = gem_int(1);
        gem_pop_frame();
        return _t180;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t181 = gem_int(2);
        gem_pop_frame();
        return _t181;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t182 = gem_int(3);
        gem_pop_frame();
        return _t182;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t183 = gem_int(4);
        gem_pop_frame();
        return _t183;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t184 = gem_int(5);
        gem_pop_frame();
        return _t184;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t185 = gem_int(6);
        gem_pop_frame();
        return _t185;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t186 = gem_int(7);
        gem_pop_frame();
        return _t186;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t187 = gem_int(8);
        gem_pop_frame();
        return _t187;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t188 = gem_int(9);
        gem_pop_frame();
        return _t188;
    } else {
#line 52 "compiler/main.gem"
    GemVal _t189[] = {gem_v_ch};
    GemVal _t190[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t189, 1))};
        GemVal _t191 = gem_error_at_fn("compiler/main.gem", 52, _t190, 1);
        gem_pop_frame();
        return _t191;
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
    GemVal _t192[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t192, 1);
#line 58 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 58 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 58 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 59 "compiler/main.gem"
    GemVal _t193[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t193, 1));
    }

    GemVal _t194 = gem_v_result;
    gem_pop_frame();
    return _t194;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 67 "compiler/main.gem"
    GemVal _t195[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t195, 1);
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
            GemVal _t196 = gem_bool(1);
            gem_pop_frame();
            return _t196;
        }
    }

    GemVal _t197 = gem_bool(0);
    gem_pop_frame();
    return _t197;
}

static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("apply_escape", "compiler/main.gem", 80);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 82 "compiler/main.gem"
        GemVal _t198 = gem_string("\n");
        gem_pop_frame();
        return _t198;
    } else {
#line 83 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 84 "compiler/main.gem"
    GemVal _t199[] = {gem_int(13)};
            GemVal _t200 = gem_chr_fn(NULL, _t199, 1);
            gem_pop_frame();
            return _t200;
        } else {
#line 85 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 86 "compiler/main.gem"
                GemVal _t201 = gem_string("\t");
                gem_pop_frame();
                return _t201;
            } else {
#line 87 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 88 "compiler/main.gem"
                    GemVal _t202 = gem_string("\\");
                    gem_pop_frame();
                    return _t202;
                } else {
#line 89 "compiler/main.gem"
    GemVal _t203;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t203 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t203 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t203)) {
#line 90 "compiler/main.gem"
    GemVal _t204[] = {gem_int(0)};
                        GemVal _t205 = gem_chr_fn(NULL, _t204, 1);
                        gem_pop_frame();
                        return _t205;
                    } else {
#line 91 "compiler/main.gem"
    GemVal _t206;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t206 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t206 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t206)) {
#line 92 "compiler/main.gem"
                            GemVal _t207 = gem_string("\"");
                            gem_pop_frame();
                            return _t207;
                        } else {
#line 93 "compiler/main.gem"
    GemVal _t208;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t208 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t208 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t208)) {
#line 94 "compiler/main.gem"
                                GemVal _t209 = gem_string("{");
                                gem_pop_frame();
                                return _t209;
                            } else {
#line 95 "compiler/main.gem"
    GemVal _t210;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t210 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t210 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t210)) {
#line 96 "compiler/main.gem"
                                    GemVal _t211 = gem_string("}");
                                    gem_pop_frame();
                                    return _t211;
                                } else {
#line 97 "compiler/main.gem"
    GemVal _t212;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t212 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t212 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t212)) {
#line 98 "compiler/main.gem"
                                        GemVal _t213 = gem_string("'");
                                        gem_pop_frame();
                                        return _t213;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t214 = GEM_NIL;
    gem_pop_frame();
    return _t214;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t215 = gem_table_new();
    GemVal gem_v_t = _t215;
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
    gem_table_set(gem_v_t, gem_string("export"), gem_bool(1));
#line 128 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("extern"), gem_bool(1));
#line 129 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("blocking"), gem_bool(1));
#line 130 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("for"), gem_bool(1));
#line 131 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("in"), gem_bool(1));
#line 132 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("after"), gem_bool(1));
#line 133 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("then"), gem_bool(1));
    GemVal _t216 = gem_v_t;
    gem_pop_frame();
    return _t216;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 137 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 137);
#line 138 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 139 "compiler/main.gem"
    GemVal _t217 = gem_table_new();
    GemVal gem_v_tokens = _t217;
#line 140 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 141 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 142 "compiler/main.gem"
    GemVal gem_v_line_start = gem_int(0);
#line 143 "compiler/main.gem"
    GemVal _t218[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t218, 1);
#line 144 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 145 "compiler/main.gem"
    GemVal _t219 = gem_table_new();
    GemVal gem_v_interp_brace = _t219;
#line 146 "compiler/main.gem"
    GemVal _t220[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t220, 1);
#line 147 "compiler/main.gem"
    GemVal _t221[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t221, 1);
#line 148 "compiler/main.gem"
    GemVal gem_v_in_triple_dq = gem_bool(0);
#line 149 "compiler/main.gem"
    GemVal gem_v_triple_dq_indent = gem_int(0);
#line 150 "compiler/main.gem"
    GemVal gem_v_tstart = gem_int(0);
#line 152 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 153 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 154 "compiler/main.gem"
        gem_v_tstart = gem_v_pos;
#line 157 "compiler/main.gem"
    GemVal _t222;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t222 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t222 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t222)) {
#line 158 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 163 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 164 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t223;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t223 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t223 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t223)) break;
#line 165 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 171 "compiler/main.gem"
    GemVal _t224;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t224 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t224 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t224)) {
#line 172 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 173 "compiler/main.gem"
    GemVal _t225[] = {gem_v_tokens};
    GemVal _t227;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t225, 1), gem_int(0)))) {
        _t227 = gem_gt(gem_len_fn(NULL, _t225, 1), gem_int(0));
    } else {
        GemVal _t226 = gem_table_get(gem_v_tokens, gem_neg(gem_int(1)));
        _t227 = gem_eq(gem_table_get(_t226, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t227)) {
#line 174 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 176 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 177 "compiler/main.gem"
    GemVal _t228 = gem_table_new();
    gem_table_set(_t228, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t228, gem_string("value"), gem_string("\n"));
    gem_table_set(_t228, gem_string("line"), gem_v_line);
    gem_table_set(_t228, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t229[] = {gem_v_tokens, _t228};
                (void)(gem_push_fn(NULL, _t229, 2));
            }
#line 179 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 180 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 181 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
            }
#line 183 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 185 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 186 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 187 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 188 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 189 "compiler/main.gem"
                    gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 190 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 191 "compiler/main.gem"
    GemVal _t230;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t230 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t230 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t231;
    if (gem_truthy(_t230)) {
        _t231 = _t230;
    } else {
        _t231 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t231)) {
#line 192 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 193 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 194 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t232;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t232 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t232 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t232)) break;
#line 195 "compiler/main.gem"
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
#line 205 "compiler/main.gem"
    GemVal _t233[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t233, 1))) {
#line 206 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 207 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t235;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t235 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t234[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t235 = gem_fn_is_digit(NULL, _t234, 1);
                }
                if (!gem_truthy(_t235)) break;
#line 208 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 209 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 211 "compiler/main.gem"
    GemVal _t236;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t236 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t236 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t236;
#line 212 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 213 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 214 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 215 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t238;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t238 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t237[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t238 = gem_fn_is_digit(NULL, _t237, 1);
                    }
                    if (!gem_truthy(_t238)) break;
#line 216 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 217 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 220 "compiler/main.gem"
    GemVal _t239 = gem_table_new();
    gem_table_set(_t239, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t239, gem_string("value"), gem_v_val);
    gem_table_set(_t239, gem_string("line"), gem_v_line);
    gem_table_set(_t239, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t240[] = {gem_v_tokens, _t239};
            (void)(gem_push_fn(NULL, _t240, 2));
            continue;
        }
#line 225 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 226 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 229 "compiler/main.gem"
    GemVal _t241;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t241 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t241 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t242;
    if (!gem_truthy(_t241)) {
        _t242 = _t241;
    } else {
        _t242 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t242)) {
#line 230 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 232 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t244;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t244 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t243;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t243 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t243 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t244 = _t243;
                    }
                    if (!gem_truthy(_t244)) break;
#line 233 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 235 "compiler/main.gem"
    GemVal _t245;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t245 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t245 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t245)) {
#line 236 "compiler/main.gem"
    GemVal _t246[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t246, 7));
                }
#line 238 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 239 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 240 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 243 "compiler/main.gem"
                GemVal gem_v_tq_scan = gem_v_pos;
#line 244 "compiler/main.gem"
                GemVal gem_v_tq_bi = gem_int(0);
#line 245 "compiler/main.gem"
                GemVal gem_v_tq_found = gem_bool(0);
#line 246 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t247;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t247 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t247 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t247)) break;
#line 247 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 248 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t248;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t248 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t248 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t248)) break;
#line 249 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 250 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 252 "compiler/main.gem"
    GemVal _t249;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t249 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t249 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t250;
    if (!gem_truthy(_t249)) {
        _t250 = _t249;
    } else {
        _t250 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t251;
    if (!gem_truthy(_t250)) {
        _t251 = _t250;
    } else {
        _t251 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t251)) {
#line 253 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 254 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 256 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t252;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t252 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t252 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t252)) break;
#line 257 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
#line 259 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
#line 260 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
                    }
                }
#line 264 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_tq_found))) {
#line 265 "compiler/main.gem"
    GemVal _t253[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t253, 7));
                }
#line 267 "compiler/main.gem"
                gem_v_triple_dq_indent = gem_v_tq_bi;
#line 270 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 271 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 272 "compiler/main.gem"
                GemVal gem_v_tq_als = gem_bool(1);
#line 274 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 275 "compiler/main.gem"
                    if (gem_truthy(gem_v_tq_als)) {
#line 276 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(0);
#line 277 "compiler/main.gem"
                        GemVal gem_v_tq_skip = gem_int(0);
#line 278 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t254;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t254 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t254 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t255;
                            if (!gem_truthy(_t254)) {
                                                        _t255 = _t254;
                            } else {
                                                        _t255 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t255)) break;
#line 279 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 280 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 283 "compiler/main.gem"
    GemVal _t256;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t256 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t256 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t257;
    if (!gem_truthy(_t256)) {
        _t257 = _t256;
    } else {
        _t257 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t258;
    if (!gem_truthy(_t257)) {
        _t258 = _t257;
    } else {
        _t258 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t258)) {
#line 284 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 286 "compiler/main.gem"
    GemVal _t259[] = {gem_v_val};
    GemVal _t260;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t259, 1), gem_int(0)))) {
        _t260 = gem_gt(gem_len_fn(NULL, _t259, 1), gem_int(0));
    } else {
        _t260 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t260)) {
#line 287 "compiler/main.gem"
    GemVal _t261[] = {gem_v_val};
    GemVal _t262[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t261, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t262, 3);
                            }
                            break;
                        }
                    }
#line 293 "compiler/main.gem"
                    GemVal gem_v_tqc = gem_table_get(gem_v_source, gem_v_pos);
#line 294 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\n")))) {
#line 295 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 296 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 297 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 298 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 299 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(1);
                        continue;
                    }
#line 302 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\\")))) {
#line 303 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 304 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 305 "compiler/main.gem"
    GemVal _t263[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t263, 7));
                        }
#line 307 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 308 "compiler/main.gem"
    GemVal _t264[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t264, 2);
#line 309 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 310 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 312 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqe);
                        }
#line 314 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 317 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("{")))) {
#line 318 "compiler/main.gem"
    GemVal _t265 = gem_table_new();
    gem_table_set(_t265, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t265, gem_string("value"), gem_string(""));
    gem_table_set(_t265, gem_string("line"), gem_v_line);
    gem_table_set(_t265, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t266[] = {gem_v_tokens, _t265};
                        (void)(gem_push_fn(NULL, _t266, 2));
#line 319 "compiler/main.gem"
    GemVal _t267 = gem_table_new();
    gem_table_set(_t267, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t267, gem_string("value"), gem_v_val);
    gem_table_set(_t267, gem_string("line"), gem_v_line);
    gem_table_set(_t267, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t268[] = {gem_v_tokens, _t267};
                        (void)(gem_push_fn(NULL, _t268, 2));
#line 320 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 321 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 322 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 323 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 324 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 325 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 328 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqc);
#line 329 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 332 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 333 "compiler/main.gem"
    GemVal _t269 = gem_table_new();
    gem_table_set(_t269, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t269, gem_string("value"), gem_v_val);
    gem_table_set(_t269, gem_string("line"), gem_v_line);
    gem_table_set(_t269, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t270[] = {gem_v_tokens, _t269};
                    (void)(gem_push_fn(NULL, _t270, 2));
                }
                continue;
            }
#line 339 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 340 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 341 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t271;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t271 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t271 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t271)) break;
#line 342 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 343 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 344 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 345 "compiler/main.gem"
    GemVal _t272[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t272, 7));
                    }
#line 347 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 348 "compiler/main.gem"
    GemVal _t273[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t273, 2);
#line 349 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 350 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 352 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 354 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 357 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 358 "compiler/main.gem"
    GemVal _t274 = gem_table_new();
    gem_table_set(_t274, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t274, gem_string("value"), gem_string(""));
    gem_table_set(_t274, gem_string("line"), gem_v_line);
    gem_table_set(_t274, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t275[] = {gem_v_tokens, _t274};
                    (void)(gem_push_fn(NULL, _t275, 2));
#line 359 "compiler/main.gem"
    GemVal _t276 = gem_table_new();
    gem_table_set(_t276, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t276, gem_string("value"), gem_v_val);
    gem_table_set(_t276, gem_string("line"), gem_v_line);
    gem_table_set(_t276, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t277[] = {gem_v_tokens, _t276};
                    (void)(gem_push_fn(NULL, _t277, 2));
#line 360 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 361 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 362 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 363 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 364 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 367 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 368 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 370 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 371 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 372 "compiler/main.gem"
    GemVal _t278[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
                    (void)(gem_fn_compile_error(NULL, _t278, 7));
                }
#line 374 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 375 "compiler/main.gem"
    GemVal _t279 = gem_table_new();
    gem_table_set(_t279, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t279, gem_string("value"), gem_v_val);
    gem_table_set(_t279, gem_string("line"), gem_v_line);
    gem_table_set(_t279, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t280[] = {gem_v_tokens, _t279};
                (void)(gem_push_fn(NULL, _t280, 2));
            }
            continue;
        }
#line 381 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 382 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 385 "compiler/main.gem"
    GemVal _t281;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t281 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t281 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t282;
    if (!gem_truthy(_t281)) {
        _t282 = _t281;
    } else {
        _t282 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t282)) {
#line 386 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 388 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t284;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t284 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t283;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t283 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t283 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t284 = _t283;
                    }
                    if (!gem_truthy(_t284)) break;
#line 389 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 391 "compiler/main.gem"
    GemVal _t285;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t285 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t285 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t285)) {
#line 392 "compiler/main.gem"
    GemVal _t286[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t286, 7));
                }
#line 394 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 395 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 396 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 399 "compiler/main.gem"
                GemVal gem_v_sq_scan = gem_v_pos;
#line 400 "compiler/main.gem"
                GemVal gem_v_sq_bi = gem_int(0);
#line 401 "compiler/main.gem"
                GemVal gem_v_sq_found = gem_bool(0);
#line 402 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t287;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t287 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t287 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t287)) break;
#line 403 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 404 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t288;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t288 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t288 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t288)) break;
#line 405 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 406 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 408 "compiler/main.gem"
    GemVal _t289;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t289 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t289 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t290;
    if (!gem_truthy(_t289)) {
        _t290 = _t289;
    } else {
        _t290 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t291;
    if (!gem_truthy(_t290)) {
        _t291 = _t290;
    } else {
        _t291 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t291)) {
#line 409 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 410 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 412 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t292;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t292 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t292 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t292)) break;
#line 413 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
#line 415 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
#line 416 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
                    }
                }
#line 420 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_sq_found))) {
#line 421 "compiler/main.gem"
    GemVal _t293[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t293, 7));
                }
#line 425 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 426 "compiler/main.gem"
                GemVal gem_v_sq_als = gem_bool(1);
#line 428 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 429 "compiler/main.gem"
                    if (gem_truthy(gem_v_sq_als)) {
#line 430 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(0);
#line 431 "compiler/main.gem"
                        GemVal gem_v_sq_skip = gem_int(0);
#line 432 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t294;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t294 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t294 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t295;
                            if (!gem_truthy(_t294)) {
                                                        _t295 = _t294;
                            } else {
                                                        _t295 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t295)) break;
#line 433 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 434 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 437 "compiler/main.gem"
    GemVal _t296;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t296 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t296 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t297;
    if (!gem_truthy(_t296)) {
        _t297 = _t296;
    } else {
        _t297 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t298;
    if (!gem_truthy(_t297)) {
        _t298 = _t297;
    } else {
        _t298 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t298)) {
#line 438 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 440 "compiler/main.gem"
    GemVal _t299[] = {gem_v_val};
    GemVal _t300;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t299, 1), gem_int(0)))) {
        _t300 = gem_gt(gem_len_fn(NULL, _t299, 1), gem_int(0));
    } else {
        _t300 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t300)) {
#line 441 "compiler/main.gem"
    GemVal _t301[] = {gem_v_val};
    GemVal _t302[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t301, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t302, 3);
                            }
                            break;
                        }
                    }
#line 447 "compiler/main.gem"
                    GemVal gem_v_sqc = gem_table_get(gem_v_source, gem_v_pos);
#line 448 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\n")))) {
#line 449 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 450 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 451 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 452 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 453 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(1);
                        continue;
                    }
#line 456 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\\")))) {
#line 457 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 458 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 459 "compiler/main.gem"
    GemVal _t303[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t303, 7));
                        }
#line 461 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 462 "compiler/main.gem"
    GemVal _t304[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t304, 2);
#line 463 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 464 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 466 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_sqe);
                        }
#line 468 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 471 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_sqc);
#line 472 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 475 "compiler/main.gem"
    GemVal _t305 = gem_table_new();
    gem_table_set(_t305, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t305, gem_string("value"), gem_v_val);
    gem_table_set(_t305, gem_string("line"), gem_v_line);
    gem_table_set(_t305, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t306[] = {gem_v_tokens, _t305};
                (void)(gem_push_fn(NULL, _t306, 2));
                continue;
            }
#line 480 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 481 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t307;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t307 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t307 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t307)) break;
#line 482 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 483 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 484 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 485 "compiler/main.gem"
    GemVal _t308[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t308, 7));
                    }
#line 487 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 488 "compiler/main.gem"
    GemVal _t309[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t309, 2);
#line 489 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 490 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 492 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 494 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 497 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 498 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 500 "compiler/main.gem"
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 501 "compiler/main.gem"
    GemVal _t310[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
                (void)(gem_fn_compile_error(NULL, _t310, 7));
            }
#line 503 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 504 "compiler/main.gem"
    GemVal _t311 = gem_table_new();
    gem_table_set(_t311, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t311, gem_string("value"), gem_v_val);
    gem_table_set(_t311, gem_string("line"), gem_v_line);
    gem_table_set(_t311, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t312[] = {gem_v_tokens, _t311};
            (void)(gem_push_fn(NULL, _t312, 2));
            continue;
        }
#line 509 "compiler/main.gem"
    GemVal _t313[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t313, 1))) {
#line 510 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 511 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t315;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t315 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t314[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t315 = gem_fn_is_alnum(NULL, _t314, 1);
                }
                if (!gem_truthy(_t315)) break;
#line 512 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 513 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 515 "compiler/main.gem"
    GemVal _t316[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_in_fn(NULL, _t316, 2))) {
#line 516 "compiler/main.gem"
    GemVal _t317 = gem_table_new();
    gem_table_set(_t317, gem_string("type"), gem_v_val);
    gem_table_set(_t317, gem_string("value"), gem_v_val);
    gem_table_set(_t317, gem_string("line"), gem_v_line);
    gem_table_set(_t317, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t318[] = {gem_v_tokens, _t317};
                (void)(gem_push_fn(NULL, _t318, 2));
            } else {
#line 518 "compiler/main.gem"
    GemVal _t319 = gem_table_new();
    gem_table_set(_t319, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t319, gem_string("value"), gem_v_val);
    gem_table_set(_t319, gem_string("line"), gem_v_line);
    gem_table_set(_t319, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t320[] = {gem_v_tokens, _t319};
                (void)(gem_push_fn(NULL, _t320, 2));
            }
            continue;
        }
#line 524 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 525 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 526 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 529 "compiler/main.gem"
    GemVal _t321;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t321 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t321 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t321)) {
#line 530 "compiler/main.gem"
    GemVal _t322 = gem_table_new();
    gem_table_set(_t322, gem_string("type"), gem_string("=="));
    gem_table_set(_t322, gem_string("value"), gem_string("=="));
    gem_table_set(_t322, gem_string("line"), gem_v_line);
    gem_table_set(_t322, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t323[] = {gem_v_tokens, _t322};
            (void)(gem_push_fn(NULL, _t323, 2));
#line 531 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 534 "compiler/main.gem"
    GemVal _t324;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t324 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t324 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t324)) {
#line 535 "compiler/main.gem"
    GemVal _t325 = gem_table_new();
    gem_table_set(_t325, gem_string("type"), gem_string("!="));
    gem_table_set(_t325, gem_string("value"), gem_string("!="));
    gem_table_set(_t325, gem_string("line"), gem_v_line);
    gem_table_set(_t325, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t326[] = {gem_v_tokens, _t325};
            (void)(gem_push_fn(NULL, _t326, 2));
#line 536 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 539 "compiler/main.gem"
    GemVal _t327;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t327 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t327 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t327)) {
#line 540 "compiler/main.gem"
    GemVal _t328 = gem_table_new();
    gem_table_set(_t328, gem_string("type"), gem_string("<="));
    gem_table_set(_t328, gem_string("value"), gem_string("<="));
    gem_table_set(_t328, gem_string("line"), gem_v_line);
    gem_table_set(_t328, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t329[] = {gem_v_tokens, _t328};
            (void)(gem_push_fn(NULL, _t329, 2));
#line 541 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 544 "compiler/main.gem"
    GemVal _t330;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t330 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t330 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t330)) {
#line 545 "compiler/main.gem"
    GemVal _t331 = gem_table_new();
    gem_table_set(_t331, gem_string("type"), gem_string(">="));
    gem_table_set(_t331, gem_string("value"), gem_string(">="));
    gem_table_set(_t331, gem_string("line"), gem_v_line);
    gem_table_set(_t331, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t332[] = {gem_v_tokens, _t331};
            (void)(gem_push_fn(NULL, _t332, 2));
#line 546 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 549 "compiler/main.gem"
    GemVal _t333;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t333 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t333 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t333)) {
#line 550 "compiler/main.gem"
    GemVal _t334 = gem_table_new();
    gem_table_set(_t334, gem_string("type"), gem_string("+="));
    gem_table_set(_t334, gem_string("value"), gem_string("+="));
    gem_table_set(_t334, gem_string("line"), gem_v_line);
    gem_table_set(_t334, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t335[] = {gem_v_tokens, _t334};
            (void)(gem_push_fn(NULL, _t335, 2));
#line 551 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 554 "compiler/main.gem"
    GemVal _t336;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t336 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t336 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t336)) {
#line 555 "compiler/main.gem"
    GemVal _t337 = gem_table_new();
    gem_table_set(_t337, gem_string("type"), gem_string("-="));
    gem_table_set(_t337, gem_string("value"), gem_string("-="));
    gem_table_set(_t337, gem_string("line"), gem_v_line);
    gem_table_set(_t337, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t338[] = {gem_v_tokens, _t337};
            (void)(gem_push_fn(NULL, _t338, 2));
#line 556 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 559 "compiler/main.gem"
    GemVal _t339;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t339 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t339 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t339)) {
#line 560 "compiler/main.gem"
    GemVal _t340 = gem_table_new();
    gem_table_set(_t340, gem_string("type"), gem_string("*="));
    gem_table_set(_t340, gem_string("value"), gem_string("*="));
    gem_table_set(_t340, gem_string("line"), gem_v_line);
    gem_table_set(_t340, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t341[] = {gem_v_tokens, _t340};
            (void)(gem_push_fn(NULL, _t341, 2));
#line 561 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 564 "compiler/main.gem"
    GemVal _t342;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t342 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t342 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t342)) {
#line 565 "compiler/main.gem"
    GemVal _t343 = gem_table_new();
    gem_table_set(_t343, gem_string("type"), gem_string("/="));
    gem_table_set(_t343, gem_string("value"), gem_string("/="));
    gem_table_set(_t343, gem_string("line"), gem_v_line);
    gem_table_set(_t343, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t344[] = {gem_v_tokens, _t343};
            (void)(gem_push_fn(NULL, _t344, 2));
#line 566 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 569 "compiler/main.gem"
    GemVal _t345;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t345 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t345 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t345)) {
#line 570 "compiler/main.gem"
    GemVal _t346 = gem_table_new();
    gem_table_set(_t346, gem_string("type"), gem_string("->"));
    gem_table_set(_t346, gem_string("value"), gem_string("->"));
    gem_table_set(_t346, gem_string("line"), gem_v_line);
    gem_table_set(_t346, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t347[] = {gem_v_tokens, _t346};
            (void)(gem_push_fn(NULL, _t347, 2));
#line 571 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 576 "compiler/main.gem"
    GemVal _t348;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t348 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t348 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t349;
    if (gem_truthy(_t348)) {
        _t349 = _t348;
    } else {
        _t349 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t350;
    if (gem_truthy(_t349)) {
        _t350 = _t349;
    } else {
        _t350 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t351;
    if (gem_truthy(_t350)) {
        _t351 = _t350;
    } else {
        _t351 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t351)) {
#line 577 "compiler/main.gem"
    GemVal _t352 = gem_table_new();
    gem_table_set(_t352, gem_string("type"), gem_v_ch);
    gem_table_set(_t352, gem_string("value"), gem_v_ch);
    gem_table_set(_t352, gem_string("line"), gem_v_line);
    gem_table_set(_t352, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t353[] = {gem_v_tokens, _t352};
            (void)(gem_push_fn(NULL, _t353, 2));
#line 578 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 581 "compiler/main.gem"
    GemVal _t354;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t354 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t354 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t355;
    if (gem_truthy(_t354)) {
        _t355 = _t354;
    } else {
        _t355 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t355)) {
#line 582 "compiler/main.gem"
    GemVal _t356 = gem_table_new();
    gem_table_set(_t356, gem_string("type"), gem_v_ch);
    gem_table_set(_t356, gem_string("value"), gem_v_ch);
    gem_table_set(_t356, gem_string("line"), gem_v_line);
    gem_table_set(_t356, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t357[] = {gem_v_tokens, _t356};
            (void)(gem_push_fn(NULL, _t357, 2));
#line 583 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 587 "compiler/main.gem"
    GemVal _t358;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t358 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t358 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t358)) {
#line 588 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 589 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 590 "compiler/main.gem"
    GemVal _t359 = gem_table_new();
    gem_table_set(_t359, gem_string("type"), gem_string("{"));
    gem_table_set(_t359, gem_string("value"), gem_string("{"));
    gem_table_set(_t359, gem_string("line"), gem_v_line);
    gem_table_set(_t359, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t360[] = {gem_v_tokens, _t359};
            (void)(gem_push_fn(NULL, _t360, 2));
#line 591 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 594 "compiler/main.gem"
    GemVal _t361;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t361 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t361 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t361)) {
#line 595 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 596 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 597 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 598 "compiler/main.gem"
    GemVal _t362 = gem_table_new();
    gem_table_set(_t362, gem_string("type"), gem_string("}"));
    gem_table_set(_t362, gem_string("value"), gem_string("}"));
    gem_table_set(_t362, gem_string("line"), gem_v_line);
    gem_table_set(_t362, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t363[] = {gem_v_tokens, _t362};
                (void)(gem_push_fn(NULL, _t363, 2));
#line 599 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 603 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 604 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 607 "compiler/main.gem"
            if (gem_truthy(gem_v_in_triple_dq)) {
#line 608 "compiler/main.gem"
                gem_v_in_triple_dq = gem_bool(0);
#line 609 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 610 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 611 "compiler/main.gem"
                GemVal gem_v_tqr_als = gem_bool(0);
#line 613 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 614 "compiler/main.gem"
                    if (gem_truthy(gem_v_tqr_als)) {
#line 615 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(0);
#line 616 "compiler/main.gem"
                        GemVal gem_v_tqr_skip = gem_int(0);
#line 617 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t364;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t364 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t364 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t365;
                            if (!gem_truthy(_t364)) {
                                                        _t365 = _t364;
                            } else {
                                                        _t365 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t365)) break;
#line 618 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 619 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 622 "compiler/main.gem"
    GemVal _t366;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t366 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t366 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t367;
    if (!gem_truthy(_t366)) {
        _t367 = _t366;
    } else {
        _t367 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t368;
    if (!gem_truthy(_t367)) {
        _t368 = _t367;
    } else {
        _t368 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t368)) {
#line 623 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 624 "compiler/main.gem"
    GemVal _t369[] = {gem_v_val};
    GemVal _t370;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t369, 1), gem_int(0)))) {
        _t370 = gem_gt(gem_len_fn(NULL, _t369, 1), gem_int(0));
    } else {
        _t370 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t370)) {
#line 625 "compiler/main.gem"
    GemVal _t371[] = {gem_v_val};
    GemVal _t372[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t371, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t372, 3);
                            }
                            break;
                        }
                    }
#line 631 "compiler/main.gem"
                    GemVal gem_v_tqrc = gem_table_get(gem_v_source, gem_v_pos);
#line 632 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\n")))) {
#line 633 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 634 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 635 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 636 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 637 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(1);
                        continue;
                    }
#line 640 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\\")))) {
#line 641 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 642 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 643 "compiler/main.gem"
    GemVal _t373[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t373, 7));
                        }
#line 645 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 646 "compiler/main.gem"
    GemVal _t374[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t374, 2);
#line 647 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 648 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 650 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqre);
                        }
#line 652 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 655 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("{")))) {
#line 656 "compiler/main.gem"
    GemVal _t375 = gem_table_new();
    gem_table_set(_t375, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t375, gem_string("value"), gem_v_val);
    gem_table_set(_t375, gem_string("line"), gem_v_line);
    gem_table_set(_t375, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t376[] = {gem_v_tokens, _t375};
                        (void)(gem_push_fn(NULL, _t376, 2));
#line 657 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 658 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 659 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 660 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 661 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 662 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 665 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqrc);
#line 666 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 669 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 670 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 671 "compiler/main.gem"
    GemVal _t377[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string interpolation"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t377, 7));
                    }
#line 673 "compiler/main.gem"
    GemVal _t378 = gem_table_new();
    gem_table_set(_t378, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t378, gem_string("value"), gem_v_val);
    gem_table_set(_t378, gem_string("line"), gem_v_line);
    gem_table_set(_t378, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t379[] = {gem_v_tokens, _t378};
                    (void)(gem_push_fn(NULL, _t379, 2));
#line 674 "compiler/main.gem"
    GemVal _t380 = gem_table_new();
    gem_table_set(_t380, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t380, gem_string("value"), gem_string(""));
    gem_table_set(_t380, gem_string("line"), gem_v_line);
    gem_table_set(_t380, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t381[] = {gem_v_tokens, _t380};
                    (void)(gem_push_fn(NULL, _t381, 2));
                }
                continue;
            }
#line 680 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 681 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 682 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t382;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t382 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t382 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t382)) break;
#line 683 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 684 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 685 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 686 "compiler/main.gem"
    GemVal _t383[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t383, 7));
                    }
#line 688 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 689 "compiler/main.gem"
    GemVal _t384[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t384, 2);
#line 690 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 691 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 693 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 695 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 698 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 700 "compiler/main.gem"
    GemVal _t385 = gem_table_new();
    gem_table_set(_t385, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t385, gem_string("value"), gem_v_val);
    gem_table_set(_t385, gem_string("line"), gem_v_line);
    gem_table_set(_t385, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t386[] = {gem_v_tokens, _t385};
                    (void)(gem_push_fn(NULL, _t386, 2));
#line 701 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 702 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 703 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 704 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 705 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 708 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 709 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 711 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 712 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 713 "compiler/main.gem"
    GemVal _t387[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string interpolation"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t387, 7));
                }
#line 715 "compiler/main.gem"
    GemVal _t388 = gem_table_new();
    gem_table_set(_t388, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t388, gem_string("value"), gem_v_val);
    gem_table_set(_t388, gem_string("line"), gem_v_line);
    gem_table_set(_t388, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t389[] = {gem_v_tokens, _t388};
                (void)(gem_push_fn(NULL, _t389, 2));
#line 716 "compiler/main.gem"
    GemVal _t390 = gem_table_new();
    gem_table_set(_t390, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t390, gem_string("value"), gem_string(""));
    gem_table_set(_t390, gem_string("line"), gem_v_line);
    gem_table_set(_t390, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t391[] = {gem_v_tokens, _t390};
                (void)(gem_push_fn(NULL, _t391, 2));
#line 717 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 722 "compiler/main.gem"
    GemVal _t392;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t392 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t392 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t393;
    if (gem_truthy(_t392)) {
        _t393 = _t392;
    } else {
        _t393 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t394;
    if (gem_truthy(_t393)) {
        _t394 = _t393;
    } else {
        _t394 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t395;
    if (gem_truthy(_t394)) {
        _t395 = _t394;
    } else {
        _t395 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t396;
    if (gem_truthy(_t395)) {
        _t396 = _t395;
    } else {
        _t396 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t396)) {
#line 723 "compiler/main.gem"
    GemVal _t397 = gem_table_new();
    gem_table_set(_t397, gem_string("type"), gem_v_ch);
    gem_table_set(_t397, gem_string("value"), gem_v_ch);
    gem_table_set(_t397, gem_string("line"), gem_v_line);
    gem_table_set(_t397, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t398[] = {gem_v_tokens, _t397};
            (void)(gem_push_fn(NULL, _t398, 2));
#line 724 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 727 "compiler/main.gem"
    GemVal _t399;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t399 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t399 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t400;
    if (!gem_truthy(_t399)) {
        _t400 = _t399;
    } else {
        _t400 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t401;
    if (!gem_truthy(_t400)) {
        _t401 = _t400;
    } else {
        _t401 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t402;
    if (!gem_truthy(_t401)) {
        _t402 = _t401;
    } else {
        _t402 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t402)) {
#line 728 "compiler/main.gem"
    GemVal _t403 = gem_table_new();
    gem_table_set(_t403, gem_string("type"), gem_string("..."));
    gem_table_set(_t403, gem_string("value"), gem_string("..."));
    gem_table_set(_t403, gem_string("line"), gem_v_line);
    gem_table_set(_t403, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t404[] = {gem_v_tokens, _t403};
            (void)(gem_push_fn(NULL, _t404, 2));
#line 729 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 732 "compiler/main.gem"
    GemVal _t405;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t405 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t405 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t406;
    if (gem_truthy(_t405)) {
        _t406 = _t405;
    } else {
        _t406 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t407;
    if (gem_truthy(_t406)) {
        _t407 = _t406;
    } else {
        _t407 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t407)) {
#line 733 "compiler/main.gem"
    GemVal _t408 = gem_table_new();
    gem_table_set(_t408, gem_string("type"), gem_v_ch);
    gem_table_set(_t408, gem_string("value"), gem_v_ch);
    gem_table_set(_t408, gem_string("line"), gem_v_line);
    gem_table_set(_t408, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t409[] = {gem_v_tokens, _t408};
            (void)(gem_push_fn(NULL, _t409, 2));
#line 734 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 738 "compiler/main.gem"
    GemVal _t410[] = {gem_v_ch};
    GemVal _t411[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t410, 1)), gem_string("'")), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t411, 7));
    }
#line 741 "compiler/main.gem"
    GemVal _t412 = gem_table_new();
    gem_table_set(_t412, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t412, gem_string("value"), gem_string(""));
    gem_table_set(_t412, gem_string("line"), gem_v_line);
    gem_table_set(_t412, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t413[] = {gem_v_tokens, _t412};
    (void)(gem_push_fn(NULL, _t413, 2));
    GemVal _t414 = gem_v_tokens;
    gem_pop_frame();
    return _t414;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 11 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 11);
#line 12 "compiler/main.gem"
    GemVal _t415[] = {gem_v_source, gem_v_file};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t415, 2);
#line 13 "compiler/main.gem"
    GemVal _t416[] = {gem_v_tokens, gem_v_source, gem_v_file};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t416, 3);
#line 14 "compiler/main.gem"
    GemVal _t417 = gem_v_p;
    GemVal _t418 = gem_table_get(_t417, gem_string("parse"));
    GemVal _t419 = _t418.fn(_t418.env, NULL, 0);
    gem_pop_frame();
    return _t419;
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
    GemVal _t420 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t420;
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
    GemVal _t422 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t422;
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
    GemVal _t424 = gem_v_t;
    gem_pop_frame();
    return _t424;
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
    GemVal _t426 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t427 = gem_eq(gem_table_get(_t426, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t427;
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
    GemVal _t429 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t429, gem_string("type")), gem_v_tp))) {
#line 47 "compiler/main.gem"
    GemVal _t430 = gem_v_t;
    GemVal _t431 = gem_v_t;
    GemVal _t432 = gem_v_t;
    GemVal _t433[] = {gem_table_get(_t432, gem_string("value"))};
    GemVal _t434[] = {gem_v_tp};
    GemVal _t435 = gem_v_t;
    GemVal _t436[] = {gem_table_get(_t435, gem_string("type"))};
    GemVal _t437[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t430, gem_string("line")), gem_table_get(_t431, gem_string("col")), gem_len_fn(NULL, _t433, 1), gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t434, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t436, 1)), gem_string("'")), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t437, 7));
    }
#line 49 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t438 = gem_v_t;
    gem_pop_frame();
    return _t438;
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
        GemVal _t440 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t440, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t442 = gem_table_new();
    GemVal gem_v_params = _t442;
#line 67 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 68 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 70 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t443 = (*gem_v_peek);
        GemVal _t444 = _t443.fn(_t443.env, NULL, 0);
        GemVal _t447;
        if (gem_truthy(gem_eq(gem_table_get(_t444, gem_string("type")), gem_string("NAME")))) {
                _t447 = gem_eq(gem_table_get(_t444, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t445 = (*gem_v_peek);
                GemVal _t446 = _t445.fn(_t445.env, NULL, 0);
                _t447 = gem_eq(gem_table_get(_t446, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t447)) break;
#line 71 "compiler/main.gem"
    GemVal _t448 = (*gem_v_peek);
    GemVal _t449 = _t448.fn(_t448.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t449, gem_string("type")), gem_string("...")))) {
#line 72 "compiler/main.gem"
    GemVal _t450 = (*gem_v_advance);
            (void)(_t450.fn(_t450.env, NULL, 0));
#line 73 "compiler/main.gem"
    GemVal _t451[] = {gem_string("NAME")};
    GemVal _t452 = (*gem_v_expect);
    GemVal _t453 = _t452.fn(_t452.env, _t451, 1);
            gem_v_rest_param = gem_table_get(_t453, gem_string("value"));
#line 74 "compiler/main.gem"
    GemVal _t454 = (*gem_v_peek);
    GemVal _t455 = _t454.fn(_t454.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t455, gem_string("type")), gem_string(",")))) {
#line 75 "compiler/main.gem"
    GemVal _t456 = (*gem_v_advance);
                (void)(_t456.fn(_t456.env, NULL, 0));
#line 76 "compiler/main.gem"
    GemVal _t457 = (*gem_v_peek);
    GemVal _t458 = _t457.fn(_t457.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t458, gem_string("type")), gem_string("NAME")))) {
#line 77 "compiler/main.gem"
    GemVal _t459 = (*gem_v_peek);
    GemVal _t460 = _t459.fn(_t459.env, NULL, 0);
    GemVal _t461 = (*gem_v_peek);
    GemVal _t462 = _t461.fn(_t461.env, NULL, 0);
    GemVal _t463 = (*gem_v_peek);
    GemVal _t464 = _t463.fn(_t463.env, NULL, 0);
    GemVal _t465[] = {gem_table_get(_t464, gem_string("value"))};
    GemVal _t466[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t460, gem_string("line")), gem_table_get(_t462, gem_string("col")), gem_len_fn(NULL, _t465, 1), gem_string("expected parameter name after rest param"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t466, 7));
                }
#line 79 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
    GemVal _t468 = _t467.fn(_t467.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t468, gem_string("value"));
#line 80 "compiler/main.gem"
    GemVal _t469 = (*gem_v_peek);
    GemVal _t470 = _t469.fn(_t469.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t470, gem_string("type")), gem_string(",")))) {
#line 81 "compiler/main.gem"
    GemVal _t471 = (*gem_v_peek);
    GemVal _t472 = _t471.fn(_t471.env, NULL, 0);
    GemVal _t473 = (*gem_v_peek);
    GemVal _t474 = _t473.fn(_t473.env, NULL, 0);
    GemVal _t475 = (*gem_v_peek);
    GemVal _t476 = _t475.fn(_t475.env, NULL, 0);
    GemVal _t477[] = {gem_table_get(_t476, gem_string("value"))};
    GemVal _t478[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t472, gem_string("line")), gem_table_get(_t474, gem_string("col")), gem_len_fn(NULL, _t477, 1), gem_string("only one parameter is allowed after a rest param"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t478, 7));
                }
            }
            break;
        }
#line 86 "compiler/main.gem"
    GemVal _t479 = (*gem_v_advance);
    GemVal _t480 = _t479.fn(_t479.env, NULL, 0);
    GemVal _t481[] = {gem_v_params, gem_table_get(_t480, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t481, 2));
#line 87 "compiler/main.gem"
    GemVal _t482 = (*gem_v_peek);
    GemVal _t483 = _t482.fn(_t482.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t483, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 90 "compiler/main.gem"
    GemVal _t484 = (*gem_v_advance);
        (void)(_t484.fn(_t484.env, NULL, 0));
    }
    GemVal _t485 = gem_table_new();
    gem_table_set(_t485, gem_string("params"), gem_v_params);
    gem_table_set(_t485, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t485, gem_string("block_param"), gem_v_block_param);
    GemVal _t486 = _t485;
    gem_pop_frame();
    return _t486;
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
    GemVal _t488 = gem_table_new();
    GemVal gem_v_stmts = _t488;
#line 110 "compiler/main.gem"
    GemVal _t489 = (*gem_v_skip_nl);
    (void)(_t489.fn(_t489.env, NULL, 0));
#line 111 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t490 = (*gem_v_peek);
        GemVal _t491 = _t490.fn(_t490.env, NULL, 0);
        GemVal _t494;
        if (!gem_truthy(gem_neq(gem_table_get(_t491, gem_string("type")), gem_string("end")))) {
                _t494 = gem_neq(gem_table_get(_t491, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t492 = (*gem_v_peek);
                GemVal _t493 = _t492.fn(_t492.env, NULL, 0);
                _t494 = gem_neq(gem_table_get(_t493, gem_string("type")), gem_string("elif"));
        }
        GemVal _t497;
        if (!gem_truthy(_t494)) {
                _t497 = _t494;
        } else {
                GemVal _t495 = (*gem_v_peek);
                GemVal _t496 = _t495.fn(_t495.env, NULL, 0);
                _t497 = gem_neq(gem_table_get(_t496, gem_string("type")), gem_string("else"));
        }
        GemVal _t500;
        if (!gem_truthy(_t497)) {
                _t500 = _t497;
        } else {
                GemVal _t498 = (*gem_v_peek);
                GemVal _t499 = _t498.fn(_t498.env, NULL, 0);
                _t500 = gem_neq(gem_table_get(_t499, gem_string("type")), gem_string("when"));
        }
        GemVal _t503;
        if (!gem_truthy(_t500)) {
                _t503 = _t500;
        } else {
                GemVal _t501 = (*gem_v_peek);
                GemVal _t502 = _t501.fn(_t501.env, NULL, 0);
                _t503 = gem_neq(gem_table_get(_t502, gem_string("type")), gem_string("after"));
        }
        GemVal _t506;
        if (!gem_truthy(_t503)) {
                _t506 = _t503;
        } else {
                GemVal _t504 = (*gem_v_peek);
                GemVal _t505 = _t504.fn(_t504.env, NULL, 0);
                _t506 = gem_neq(gem_table_get(_t505, gem_string("type")), gem_string("then"));
        }
        GemVal _t508;
        if (!gem_truthy(_t506)) {
                _t508 = _t506;
        } else {
                GemVal _t507 = (*gem_v_at_end);
                _t508 = gem_not(_t507.fn(_t507.env, NULL, 0));
        }
        if (!gem_truthy(_t508)) break;
#line 112 "compiler/main.gem"
    GemVal _t509 = (*gem_v_parse_stmt);
    GemVal _t510[] = {gem_v_stmts, _t509.fn(_t509.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t510, 2));
#line 113 "compiler/main.gem"
    GemVal _t511 = (*gem_v_skip_nl);
        (void)(_t511.fn(_t511.env, NULL, 0));
    }
    GemVal _t512 = gem_v_stmts;
    gem_pop_frame();
    return _t512;
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
    GemVal _t514 = gem_table_new();
    GemVal gem_v_stmts = _t514;
#line 123 "compiler/main.gem"
    GemVal _t515 = (*gem_v_skip_nl);
    (void)(_t515.fn(_t515.env, NULL, 0));
#line 124 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t516 = (*gem_v_peek);
        GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
        GemVal _t519;
        if (!gem_truthy(gem_neq(gem_table_get(_t517, gem_string("type")), gem_string("end")))) {
                _t519 = gem_neq(gem_table_get(_t517, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t518 = (*gem_v_at_end);
                _t519 = gem_not(_t518.fn(_t518.env, NULL, 0));
        }
        if (!gem_truthy(_t519)) break;
#line 125 "compiler/main.gem"
    GemVal _t520 = (*gem_v_parse_stmt);
    GemVal _t521[] = {gem_v_stmts, _t520.fn(_t520.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t521, 2));
#line 126 "compiler/main.gem"
    GemVal _t522 = (*gem_v_skip_nl);
        (void)(_t522.fn(_t522.env, NULL, 0));
    }
    GemVal _t523 = gem_v_stmts;
    gem_pop_frame();
    return _t523;
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
    GemVal _t525 = (*gem_v_peek);
    GemVal gem_v_t = _t525.fn(_t525.env, NULL, 0);
#line 138 "compiler/main.gem"
    GemVal _t526 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t526, gem_string("type")), gem_string("NUMBER")))) {
#line 139 "compiler/main.gem"
    GemVal _t527 = (*gem_v_advance);
        (void)(_t527.fn(_t527.env, NULL, 0));
#line 140 "compiler/main.gem"
    GemVal _t528 = gem_v_t;
    GemVal _t529[] = {gem_table_get(_t528, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t529, 1))) {
#line 141 "compiler/main.gem"
    GemVal _t530 = gem_v_t;
    GemVal _t531[] = {gem_table_get(_t530, gem_string("value"))};
    GemVal _t532[] = {gem_fn_atof(NULL, _t531, 1)};
            GemVal _t533 = gem_fn_make_float(NULL, _t532, 1);
            gem_pop_frame();
            return _t533;
        }
#line 143 "compiler/main.gem"
    GemVal _t534 = gem_v_t;
    GemVal _t535[] = {gem_table_get(_t534, gem_string("value"))};
    GemVal _t536[] = {gem_fn_str_to_int(NULL, _t535, 1)};
        GemVal _t537 = gem_fn_make_int(NULL, _t536, 1);
        gem_pop_frame();
        return _t537;
    }
#line 147 "compiler/main.gem"
    GemVal _t538 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t538, gem_string("type")), gem_string("STRING")))) {
#line 148 "compiler/main.gem"
    GemVal _t539 = (*gem_v_advance);
        (void)(_t539.fn(_t539.env, NULL, 0));
#line 149 "compiler/main.gem"
    GemVal _t540 = gem_v_t;
    GemVal _t541[] = {gem_table_get(_t540, gem_string("value"))};
        GemVal _t542 = gem_fn_make_string(NULL, _t541, 1);
        gem_pop_frame();
        return _t542;
    }
#line 153 "compiler/main.gem"
    GemVal _t543 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t543, gem_string("type")), gem_string("INTERP_START")))) {
#line 154 "compiler/main.gem"
    GemVal _t544 = (*gem_v_advance);
        (void)(_t544.fn(_t544.env, NULL, 0));
#line 155 "compiler/main.gem"
    GemVal _t545 = gem_table_new();
        GemVal gem_v_parts = _t545;
#line 156 "compiler/main.gem"
    GemVal _t546 = (*gem_v_skip_nl);
        (void)(_t546.fn(_t546.env, NULL, 0));
#line 157 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t547 = (*gem_v_peek);
            GemVal _t548 = _t547.fn(_t547.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t548, gem_string("type")), gem_string("INTERP_END")))) break;
#line 158 "compiler/main.gem"
    GemVal _t549 = (*gem_v_peek);
    GemVal _t550 = _t549.fn(_t549.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t550, gem_string("type")), gem_string("STRING")))) {
#line 159 "compiler/main.gem"
    GemVal _t551 = (*gem_v_advance);
    GemVal _t552 = _t551.fn(_t551.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t552, gem_string("value"));
#line 160 "compiler/main.gem"
    GemVal _t553[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t553, 1), gem_int(0)))) {
#line 161 "compiler/main.gem"
    GemVal _t554[] = {gem_v_sval};
    GemVal _t555[] = {gem_v_parts, gem_fn_make_string(NULL, _t554, 1)};
                    (void)(gem_push_fn(NULL, _t555, 2));
                }
            } else {
#line 164 "compiler/main.gem"
    GemVal _t556 = (*gem_v_parse_expr);
    GemVal _t557[] = {gem_v_parts, _t556.fn(_t556.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t557, 2));
            }
#line 166 "compiler/main.gem"
    GemVal _t558 = (*gem_v_skip_nl);
            (void)(_t558.fn(_t558.env, NULL, 0));
        }
#line 168 "compiler/main.gem"
    GemVal _t559[] = {gem_string("INTERP_END")};
    GemVal _t560 = (*gem_v_expect);
        (void)(_t560.fn(_t560.env, _t559, 1));
#line 169 "compiler/main.gem"
    GemVal _t561[] = {gem_v_parts};
        GemVal _t562 = gem_fn_make_interp(NULL, _t561, 1);
        gem_pop_frame();
        return _t562;
    }
#line 173 "compiler/main.gem"
    GemVal _t563 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t563, gem_string("type")), gem_string("true")))) {
#line 174 "compiler/main.gem"
    GemVal _t564 = (*gem_v_advance);
        (void)(_t564.fn(_t564.env, NULL, 0));
#line 175 "compiler/main.gem"
    GemVal _t565[] = {gem_bool(1)};
        GemVal _t566 = gem_fn_make_bool(NULL, _t565, 1);
        gem_pop_frame();
        return _t566;
    }
#line 177 "compiler/main.gem"
    GemVal _t567 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t567, gem_string("type")), gem_string("false")))) {
#line 178 "compiler/main.gem"
    GemVal _t568 = (*gem_v_advance);
        (void)(_t568.fn(_t568.env, NULL, 0));
#line 179 "compiler/main.gem"
    GemVal _t569[] = {gem_bool(0)};
        GemVal _t570 = gem_fn_make_bool(NULL, _t569, 1);
        gem_pop_frame();
        return _t570;
    }
#line 183 "compiler/main.gem"
    GemVal _t571 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t571, gem_string("type")), gem_string("nil")))) {
#line 184 "compiler/main.gem"
    GemVal _t572 = (*gem_v_advance);
        (void)(_t572.fn(_t572.env, NULL, 0));
#line 185 "compiler/main.gem"
        GemVal _t573 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t573;
    }
#line 189 "compiler/main.gem"
    GemVal _t574 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t574, gem_string("type")), gem_string("fn")))) {
#line 190 "compiler/main.gem"
    GemVal _t575 = (*gem_v_advance);
        (void)(_t575.fn(_t575.env, NULL, 0));
#line 191 "compiler/main.gem"
    GemVal _t576[] = {gem_string("(")};
    GemVal _t577 = (*gem_v_expect);
        (void)(_t577.fn(_t577.env, _t576, 1));
#line 192 "compiler/main.gem"
    GemVal _t578 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t578.fn(_t578.env, NULL, 0);
#line 193 "compiler/main.gem"
    GemVal _t579[] = {gem_string(")")};
    GemVal _t580 = (*gem_v_expect);
        (void)(_t580.fn(_t580.env, _t579, 1));
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t581 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t581.fn(_t581.env, NULL, 0);
#line 196 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 197 "compiler/main.gem"
    GemVal _t582[] = {gem_string("end")};
    GemVal _t583 = (*gem_v_expect);
        (void)(_t583.fn(_t583.env, _t582, 1));
#line 198 "compiler/main.gem"
    GemVal _t584 = gem_v_pr;
    GemVal _t585 = gem_v_pr;
    GemVal _t586 = gem_v_pr;
    GemVal _t587[] = {gem_table_get(_t584, gem_string("params")), gem_table_get(_t585, gem_string("rest_param")), gem_table_get(_t586, gem_string("block_param")), gem_v_body};
        GemVal _t588 = gem_fn_make_anon_fn(NULL, _t587, 4);
        gem_pop_frame();
        return _t588;
    }
#line 203 "compiler/main.gem"
    GemVal _t589 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t589, gem_string("type")), gem_string("{")))) {
#line 204 "compiler/main.gem"
    GemVal _t590 = (*gem_v_advance);
        (void)(_t590.fn(_t590.env, NULL, 0));
#line 205 "compiler/main.gem"
    GemVal _t591 = (*gem_v_skip_nl);
        (void)(_t591.fn(_t591.env, NULL, 0));
#line 206 "compiler/main.gem"
    GemVal _t592 = gem_table_new();
        GemVal gem_v_entries = _t592;
#line 207 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t593 = (*gem_v_peek);
            GemVal _t594 = _t593.fn(_t593.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t594, gem_string("type")), gem_string("}")))) break;
#line 208 "compiler/main.gem"
    GemVal _t595 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t595.fn(_t595.env, NULL, 0);
#line 209 "compiler/main.gem"
    GemVal _t596 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t596, gem_string("type")), gem_string("NAME")))) {
#line 210 "compiler/main.gem"
    GemVal _t597 = (*gem_v_advance);
                (void)(_t597.fn(_t597.env, NULL, 0));
            } else {
#line 211 "compiler/main.gem"
    GemVal _t598 = (*gem_v_at_end);
    GemVal _t602;
    if (!gem_truthy(gem_not(_t598.fn(_t598.env, NULL, 0)))) {
        _t602 = gem_not(_t598.fn(_t598.env, NULL, 0));
    } else {
        GemVal _t599[] = {gem_int(1)};
        GemVal _t600 = (*gem_v_peek_at);
        GemVal _t601 = _t600.fn(_t600.env, _t599, 1);
        _t602 = gem_eq(gem_table_get(_t601, gem_string("type")), gem_string(":"));
    }
                if (gem_truthy(_t602)) {
#line 213 "compiler/main.gem"
    GemVal _t603 = (*gem_v_advance);
                    (void)(_t603.fn(_t603.env, NULL, 0));
                } else {
#line 215 "compiler/main.gem"
    GemVal _t604 = gem_v_key_tok;
    GemVal _t605 = gem_v_key_tok;
    GemVal _t606 = gem_v_key_tok;
    GemVal _t607[] = {gem_table_get(_t606, gem_string("value"))};
    GemVal _t608 = gem_v_key_tok;
    GemVal _t609[] = {gem_table_get(_t608, gem_string("type"))};
    GemVal _t610[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t604, gem_string("line")), gem_table_get(_t605, gem_string("col")), gem_len_fn(NULL, _t607, 1), gem_add(gem_add(gem_string("expected '}' or table key but got '"), gem_to_string_fn(NULL, _t609, 1)), gem_string("'")), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t610, 7));
                }
            }
#line 217 "compiler/main.gem"
    GemVal _t611[] = {gem_string(":")};
    GemVal _t612 = (*gem_v_expect);
            (void)(_t612.fn(_t612.env, _t611, 1));
#line 218 "compiler/main.gem"
    GemVal _t613 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t613.fn(_t613.env, NULL, 0);
#line 219 "compiler/main.gem"
    GemVal _t614 = gem_v_key_tok;
    GemVal _t615[] = {gem_table_get(_t614, gem_string("value")), gem_v_val};
    GemVal _t616[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t615, 2)};
            (void)(gem_push_fn(NULL, _t616, 2));
#line 220 "compiler/main.gem"
    GemVal _t617 = (*gem_v_skip_nl);
            (void)(_t617.fn(_t617.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t618 = (*gem_v_peek);
    GemVal _t619 = _t618.fn(_t618.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t619, gem_string("type")), gem_string(",")))) {
#line 222 "compiler/main.gem"
    GemVal _t620 = (*gem_v_advance);
                (void)(_t620.fn(_t620.env, NULL, 0));
#line 223 "compiler/main.gem"
    GemVal _t621 = (*gem_v_skip_nl);
                (void)(_t621.fn(_t621.env, NULL, 0));
            }
        }
#line 226 "compiler/main.gem"
    GemVal _t622[] = {gem_string("}")};
    GemVal _t623 = (*gem_v_expect);
        (void)(_t623.fn(_t623.env, _t622, 1));
#line 227 "compiler/main.gem"
    GemVal _t624[] = {gem_v_entries};
        GemVal _t625 = gem_fn_make_table(NULL, _t624, 1);
        gem_pop_frame();
        return _t625;
    }
#line 231 "compiler/main.gem"
    GemVal _t626 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t626, gem_string("type")), gem_string("[")))) {
#line 232 "compiler/main.gem"
    GemVal _t627 = (*gem_v_advance);
        (void)(_t627.fn(_t627.env, NULL, 0));
#line 233 "compiler/main.gem"
    GemVal _t628 = (*gem_v_skip_nl);
        (void)(_t628.fn(_t628.env, NULL, 0));
#line 234 "compiler/main.gem"
    GemVal _t629 = gem_table_new();
        GemVal gem_v_elements = _t629;
#line 235 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t630 = (*gem_v_peek);
            GemVal _t631 = _t630.fn(_t630.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t631, gem_string("type")), gem_string("]")))) break;
#line 236 "compiler/main.gem"
    GemVal _t632 = (*gem_v_parse_expr);
    GemVal _t633[] = {gem_v_elements, _t632.fn(_t632.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t633, 2));
#line 237 "compiler/main.gem"
    GemVal _t634 = (*gem_v_skip_nl);
            (void)(_t634.fn(_t634.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t635 = (*gem_v_peek);
    GemVal _t636 = _t635.fn(_t635.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t636, gem_string("type")), gem_string(",")))) {
#line 239 "compiler/main.gem"
    GemVal _t637 = (*gem_v_advance);
                (void)(_t637.fn(_t637.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t638 = (*gem_v_skip_nl);
                (void)(_t638.fn(_t638.env, NULL, 0));
            }
        }
#line 243 "compiler/main.gem"
    GemVal _t639[] = {gem_string("]")};
    GemVal _t640 = (*gem_v_expect);
        (void)(_t640.fn(_t640.env, _t639, 1));
#line 244 "compiler/main.gem"
    GemVal _t641[] = {gem_v_elements};
        GemVal _t642 = gem_fn_make_array(NULL, _t641, 1);
        gem_pop_frame();
        return _t642;
    }
#line 248 "compiler/main.gem"
    GemVal _t643 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t643, gem_string("type")), gem_string("(")))) {
#line 249 "compiler/main.gem"
    GemVal _t644 = (*gem_v_advance);
        (void)(_t644.fn(_t644.env, NULL, 0));
#line 250 "compiler/main.gem"
    GemVal _t645 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t645.fn(_t645.env, NULL, 0);
#line 251 "compiler/main.gem"
    GemVal _t646[] = {gem_string(")")};
    GemVal _t647 = (*gem_v_expect);
        (void)(_t647.fn(_t647.env, _t646, 1));
#line 252 "compiler/main.gem"
        GemVal _t648 = gem_v_e;
        gem_pop_frame();
        return _t648;
    }
#line 256 "compiler/main.gem"
    GemVal _t649 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t649, gem_string("type")), gem_string("NAME")))) {
#line 257 "compiler/main.gem"
    GemVal _t650 = (*gem_v_advance);
        (void)(_t650.fn(_t650.env, NULL, 0));
#line 258 "compiler/main.gem"
    GemVal _t651 = gem_v_t;
    GemVal _t652[] = {gem_table_get(_t651, gem_string("value"))};
        GemVal _t653 = gem_fn_make_var(NULL, _t652, 1);
        gem_pop_frame();
        return _t653;
    }
#line 261 "compiler/main.gem"
    GemVal _t654 = gem_v_t;
    GemVal _t655 = gem_v_t;
    GemVal _t656 = gem_v_t;
    GemVal _t657[] = {gem_table_get(_t656, gem_string("value"))};
    GemVal _t658 = gem_v_t;
    GemVal _t659[] = {gem_table_get(_t658, gem_string("type"))};
    GemVal _t660[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t654, gem_string("line")), gem_table_get(_t655, gem_string("col")), gem_len_fn(NULL, _t657, 1), gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t659, 1)), gem_string("' (expected expression)")), GEM_NIL};
    GemVal _t661 = gem_fn_compile_error(NULL, _t660, 7);
    gem_pop_frame();
    return _t661;
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
    GemVal _t663 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t663.fn(_t663.env, NULL, 0);
#line 268 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 270 "compiler/main.gem"
    GemVal _t664 = (*gem_v_peek);
    GemVal _t665 = _t664.fn(_t664.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t665, gem_string("type")), gem_string("(")))) {
#line 271 "compiler/main.gem"
    GemVal _t666 = (*gem_v_peek);
    GemVal _t667 = _t666.fn(_t666.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t667, gem_string("line"));
#line 272 "compiler/main.gem"
    GemVal _t668 = (*gem_v_advance);
            (void)(_t668.fn(_t668.env, NULL, 0));
#line 273 "compiler/main.gem"
    GemVal _t669 = (*gem_v_skip_nl);
            (void)(_t669.fn(_t669.env, NULL, 0));
#line 274 "compiler/main.gem"
    GemVal _t670 = gem_table_new();
            GemVal gem_v_args = _t670;
#line 275 "compiler/main.gem"
    GemVal _t671 = (*gem_v_peek);
    GemVal _t672 = _t671.fn(_t671.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t672, gem_string("type")), gem_string(")")))) {
#line 276 "compiler/main.gem"
    GemVal _t673 = (*gem_v_parse_expr);
    GemVal _t674[] = {gem_v_args, _t673.fn(_t673.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t674, 2));
#line 277 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t675 = (*gem_v_peek);
                    GemVal _t676 = _t675.fn(_t675.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t676, gem_string("type")), gem_string(",")))) break;
#line 278 "compiler/main.gem"
    GemVal _t677 = (*gem_v_advance);
                    (void)(_t677.fn(_t677.env, NULL, 0));
#line 279 "compiler/main.gem"
    GemVal _t678 = (*gem_v_skip_nl);
                    (void)(_t678.fn(_t678.env, NULL, 0));
#line 280 "compiler/main.gem"
    GemVal _t679 = (*gem_v_parse_expr);
    GemVal _t680[] = {gem_v_args, _t679.fn(_t679.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t680, 2));
                }
            }
#line 283 "compiler/main.gem"
    GemVal _t681 = (*gem_v_skip_nl);
            (void)(_t681.fn(_t681.env, NULL, 0));
#line 284 "compiler/main.gem"
    GemVal _t682[] = {gem_string(")")};
    GemVal _t683 = (*gem_v_expect);
            (void)(_t683.fn(_t683.env, _t682, 1));
#line 287 "compiler/main.gem"
    GemVal _t684 = (*gem_v_peek);
    GemVal _t685 = _t684.fn(_t684.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t685, gem_string("type")), gem_string("do")))) {
#line 288 "compiler/main.gem"
    GemVal _t686 = (*gem_v_advance);
                (void)(_t686.fn(_t686.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t687 = gem_table_new();
                GemVal gem_v_bparams = _t687;
#line 290 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 291 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 292 "compiler/main.gem"
    GemVal _t688 = (*gem_v_peek);
    GemVal _t689 = _t688.fn(_t688.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t689, gem_string("type")), gem_string("|")))) {
#line 293 "compiler/main.gem"
    GemVal _t690 = (*gem_v_advance);
                    (void)(_t690.fn(_t690.env, NULL, 0));
#line 294 "compiler/main.gem"
    GemVal _t691 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t691.fn(_t691.env, NULL, 0);
#line 295 "compiler/main.gem"
    GemVal _t692 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t692, gem_string("params"));
#line 296 "compiler/main.gem"
    GemVal _t693 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t693, gem_string("rest_param"));
#line 297 "compiler/main.gem"
    GemVal _t694 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t694, gem_string("block_param"));
#line 298 "compiler/main.gem"
    GemVal _t695[] = {gem_string("|")};
    GemVal _t696 = (*gem_v_expect);
                    (void)(_t696.fn(_t696.env, _t695, 1));
                }
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t697 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t697.fn(_t697.env, NULL, 0);
#line 302 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 303 "compiler/main.gem"
    GemVal _t698[] = {gem_string("end")};
    GemVal _t699 = (*gem_v_expect);
                (void)(_t699.fn(_t699.env, _t698, 1));
#line 304 "compiler/main.gem"
    GemVal _t700[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t701[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t700, 4)};
                (void)(gem_push_fn(NULL, _t701, 2));
            } else {
#line 308 "compiler/main.gem"
    GemVal _t702 = (*gem_v_peek);
    GemVal _t703 = _t702.fn(_t702.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t703, gem_string("type")), gem_string("{")))) {
#line 309 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 310 "compiler/main.gem"
    GemVal _t704[] = {gem_int(1)};
    GemVal _t705 = (*gem_v_peek_at);
    GemVal _t706 = _t705.fn(_t705.env, _t704, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t706, gem_string("type")), gem_string("|")))) {
#line 311 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 312 "compiler/main.gem"
    GemVal _t707[] = {gem_int(1)};
    GemVal _t708 = (*gem_v_peek_at);
    GemVal _t709 = _t708.fn(_t708.env, _t707, 1);
    GemVal _t717;
    if (gem_truthy(gem_eq(gem_table_get(_t709, gem_string("type")), gem_string("}")))) {
        _t717 = gem_eq(gem_table_get(_t709, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t710[] = {gem_int(1)};
        GemVal _t711 = (*gem_v_peek_at);
        GemVal _t712 = _t711.fn(_t711.env, _t710, 1);
        GemVal _t716;
        if (!gem_truthy(gem_eq(gem_table_get(_t712, gem_string("type")), gem_string("NAME")))) {
                _t716 = gem_eq(gem_table_get(_t712, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t713[] = {gem_int(2)};
                GemVal _t714 = (*gem_v_peek_at);
                GemVal _t715 = _t714.fn(_t714.env, _t713, 1);
                _t716 = gem_eq(gem_table_get(_t715, gem_string("type")), gem_string(":"));
        }
        _t717 = _t716;
    }
                        if (gem_truthy(_t717)) {
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
    GemVal _t718 = (*gem_v_advance);
                        (void)(_t718.fn(_t718.env, NULL, 0));
#line 319 "compiler/main.gem"
    GemVal _t719 = gem_table_new();
                        GemVal gem_v_bparams = _t719;
#line 320 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 321 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 322 "compiler/main.gem"
    GemVal _t720 = (*gem_v_peek);
    GemVal _t721 = _t720.fn(_t720.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t721, gem_string("type")), gem_string("|")))) {
#line 323 "compiler/main.gem"
    GemVal _t722 = (*gem_v_advance);
                            (void)(_t722.fn(_t722.env, NULL, 0));
#line 324 "compiler/main.gem"
    GemVal _t723 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t723.fn(_t723.env, NULL, 0);
#line 325 "compiler/main.gem"
    GemVal _t724 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t724, gem_string("params"));
#line 326 "compiler/main.gem"
    GemVal _t725 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t725, gem_string("rest_param"));
#line 327 "compiler/main.gem"
    GemVal _t726 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t726, gem_string("block_param"));
#line 328 "compiler/main.gem"
    GemVal _t727[] = {gem_string("|")};
    GemVal _t728 = (*gem_v_expect);
                            (void)(_t728.fn(_t728.env, _t727, 1));
                        }
#line 330 "compiler/main.gem"
    GemVal _t729 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t729.fn(_t729.env, NULL, 0);
#line 331 "compiler/main.gem"
    GemVal _t730[] = {gem_string("}")};
    GemVal _t731 = (*gem_v_expect);
                        (void)(_t731.fn(_t731.env, _t730, 1));
#line 332 "compiler/main.gem"
    GemVal _t732 = gem_table_new();
    gem_table_set(_t732, gem_int(0), gem_v_bexpr);
    GemVal _t733[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t732};
    GemVal _t734[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t733, 4)};
                        (void)(gem_push_fn(NULL, _t734, 2));
                    }
                }
            }
#line 337 "compiler/main.gem"
    GemVal _t735[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t735, 3);
            continue;
        }
#line 342 "compiler/main.gem"
    GemVal _t736 = (*gem_v_peek);
    GemVal _t737 = _t736.fn(_t736.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t737, gem_string("type")), gem_string(".")))) {
#line 343 "compiler/main.gem"
    GemVal _t738 = (*gem_v_advance);
            (void)(_t738.fn(_t738.env, NULL, 0));
#line 344 "compiler/main.gem"
    GemVal _t739 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t739.fn(_t739.env, NULL, 0);
#line 345 "compiler/main.gem"
    GemVal _t740 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t740, gem_string("type")), gem_string("NAME")))) {
#line 346 "compiler/main.gem"
    GemVal _t741 = (*gem_v_advance);
                (void)(_t741.fn(_t741.env, NULL, 0));
            } else {
#line 349 "compiler/main.gem"
    GemVal _t742 = (*gem_v_advance);
                (void)(_t742.fn(_t742.env, NULL, 0));
            }
#line 351 "compiler/main.gem"
    GemVal _t743 = gem_v_field_tok;
    GemVal _t744[] = {gem_v_node, gem_table_get(_t743, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t744, 2);
            continue;
        }
#line 356 "compiler/main.gem"
    GemVal _t745 = (*gem_v_peek);
    GemVal _t746 = _t745.fn(_t745.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t746, gem_string("type")), gem_string("[")))) {
#line 357 "compiler/main.gem"
    GemVal _t747 = (*gem_v_advance);
            (void)(_t747.fn(_t747.env, NULL, 0));
#line 358 "compiler/main.gem"
    GemVal _t748 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t748.fn(_t748.env, NULL, 0);
#line 359 "compiler/main.gem"
    GemVal _t749[] = {gem_string("]")};
    GemVal _t750 = (*gem_v_expect);
            (void)(_t750.fn(_t750.env, _t749, 1));
#line 360 "compiler/main.gem"
    GemVal _t751[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t751, 2);
            continue;
        }
        break;
    }
    GemVal _t752 = gem_v_node;
    gem_pop_frame();
    return _t752;
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
    GemVal _t754 = (*gem_v_peek);
    GemVal _t755 = _t754.fn(_t754.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t755, gem_string("type")), gem_string("-")))) {
#line 373 "compiler/main.gem"
    GemVal _t756 = (*gem_v_advance);
        (void)(_t756.fn(_t756.env, NULL, 0));
#line 374 "compiler/main.gem"
    GemVal _t757 = (*gem_v_parse_unary);
    GemVal _t758[] = {gem_string("-"), _t757.fn(_t757.env, NULL, 0)};
        GemVal _t759 = gem_fn_make_unop(NULL, _t758, 2);
        gem_pop_frame();
        return _t759;
    }
#line 376 "compiler/main.gem"
    GemVal _t760 = (*gem_v_parse_call);
    GemVal _t761 = _t760.fn(_t760.env, NULL, 0);
    gem_pop_frame();
    return _t761;
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
    GemVal _t763 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t763.fn(_t763.env, NULL, 0);
#line 382 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t764 = (*gem_v_peek);
        GemVal _t765 = _t764.fn(_t764.env, NULL, 0);
        GemVal _t768;
        if (gem_truthy(gem_eq(gem_table_get(_t765, gem_string("type")), gem_string("*")))) {
                _t768 = gem_eq(gem_table_get(_t765, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t766 = (*gem_v_peek);
                GemVal _t767 = _t766.fn(_t766.env, NULL, 0);
                _t768 = gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("/"));
        }
        GemVal _t771;
        if (gem_truthy(_t768)) {
                _t771 = _t768;
        } else {
                GemVal _t769 = (*gem_v_peek);
                GemVal _t770 = _t769.fn(_t769.env, NULL, 0);
                _t771 = gem_eq(gem_table_get(_t770, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t771)) break;
#line 383 "compiler/main.gem"
    GemVal _t772 = (*gem_v_advance);
    GemVal _t773 = _t772.fn(_t772.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t773, gem_string("type"));
#line 384 "compiler/main.gem"
    GemVal _t774 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t774.fn(_t774.env, NULL, 0);
#line 385 "compiler/main.gem"
    GemVal _t775[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t775, 3);
    }
    GemVal _t776 = gem_v_left;
    gem_pop_frame();
    return _t776;
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
    GemVal _t778 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t778.fn(_t778.env, NULL, 0);
#line 393 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t779 = (*gem_v_peek);
        GemVal _t780 = _t779.fn(_t779.env, NULL, 0);
        GemVal _t783;
        if (gem_truthy(gem_eq(gem_table_get(_t780, gem_string("type")), gem_string("+")))) {
                _t783 = gem_eq(gem_table_get(_t780, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t781 = (*gem_v_peek);
                GemVal _t782 = _t781.fn(_t781.env, NULL, 0);
                _t783 = gem_eq(gem_table_get(_t782, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t783)) break;
#line 394 "compiler/main.gem"
    GemVal _t784 = (*gem_v_advance);
    GemVal _t785 = _t784.fn(_t784.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t785, gem_string("type"));
#line 395 "compiler/main.gem"
    GemVal _t786 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t786.fn(_t786.env, NULL, 0);
#line 396 "compiler/main.gem"
    GemVal _t787[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t787, 3);
    }
    GemVal _t788 = gem_v_left;
    gem_pop_frame();
    return _t788;
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
    GemVal _t790 = (*gem_v_parse_add);
    GemVal gem_v_left = _t790.fn(_t790.env, NULL, 0);
#line 404 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t791 = (*gem_v_peek);
        GemVal _t792 = _t791.fn(_t791.env, NULL, 0);
        GemVal _t795;
        if (gem_truthy(gem_eq(gem_table_get(_t792, gem_string("type")), gem_string("==")))) {
                _t795 = gem_eq(gem_table_get(_t792, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t793 = (*gem_v_peek);
                GemVal _t794 = _t793.fn(_t793.env, NULL, 0);
                _t795 = gem_eq(gem_table_get(_t794, gem_string("type")), gem_string("!="));
        }
        GemVal _t798;
        if (gem_truthy(_t795)) {
                _t798 = _t795;
        } else {
                GemVal _t796 = (*gem_v_peek);
                GemVal _t797 = _t796.fn(_t796.env, NULL, 0);
                _t798 = gem_eq(gem_table_get(_t797, gem_string("type")), gem_string("<"));
        }
        GemVal _t801;
        if (gem_truthy(_t798)) {
                _t801 = _t798;
        } else {
                GemVal _t799 = (*gem_v_peek);
                GemVal _t800 = _t799.fn(_t799.env, NULL, 0);
                _t801 = gem_eq(gem_table_get(_t800, gem_string("type")), gem_string(">"));
        }
        GemVal _t804;
        if (gem_truthy(_t801)) {
                _t804 = _t801;
        } else {
                GemVal _t802 = (*gem_v_peek);
                GemVal _t803 = _t802.fn(_t802.env, NULL, 0);
                _t804 = gem_eq(gem_table_get(_t803, gem_string("type")), gem_string("<="));
        }
        GemVal _t807;
        if (gem_truthy(_t804)) {
                _t807 = _t804;
        } else {
                GemVal _t805 = (*gem_v_peek);
                GemVal _t806 = _t805.fn(_t805.env, NULL, 0);
                _t807 = gem_eq(gem_table_get(_t806, gem_string("type")), gem_string(">="));
        }
        GemVal _t810;
        if (gem_truthy(_t807)) {
                _t810 = _t807;
        } else {
                GemVal _t808 = (*gem_v_peek);
                GemVal _t809 = _t808.fn(_t808.env, NULL, 0);
                _t810 = gem_eq(gem_table_get(_t809, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t810)) break;
#line 405 "compiler/main.gem"
    GemVal _t811 = (*gem_v_advance);
    GemVal _t812 = _t811.fn(_t811.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t812, gem_string("type"));
#line 406 "compiler/main.gem"
    GemVal _t813 = (*gem_v_parse_add);
        GemVal gem_v_right = _t813.fn(_t813.env, NULL, 0);
#line 407 "compiler/main.gem"
    GemVal _t814[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t814, 3);
    }
    GemVal _t815 = gem_v_left;
    gem_pop_frame();
    return _t815;
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
    GemVal _t817 = (*gem_v_peek);
    GemVal _t818 = _t817.fn(_t817.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t818, gem_string("type")), gem_string("not")))) {
#line 415 "compiler/main.gem"
    GemVal _t819 = (*gem_v_advance);
        (void)(_t819.fn(_t819.env, NULL, 0));
#line 416 "compiler/main.gem"
    GemVal _t820 = (*gem_v_parse_not);
    GemVal _t821[] = {gem_string("not"), _t820.fn(_t820.env, NULL, 0)};
        GemVal _t822 = gem_fn_make_unop(NULL, _t821, 2);
        gem_pop_frame();
        return _t822;
    }
#line 418 "compiler/main.gem"
    GemVal _t823 = (*gem_v_parse_compare);
    GemVal _t824 = _t823.fn(_t823.env, NULL, 0);
    gem_pop_frame();
    return _t824;
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
    GemVal _t826 = (*gem_v_parse_not);
    GemVal gem_v_left = _t826.fn(_t826.env, NULL, 0);
#line 424 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t827 = (*gem_v_peek);
        GemVal _t828 = _t827.fn(_t827.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t828, gem_string("type")), gem_string("and")))) break;
#line 425 "compiler/main.gem"
    GemVal _t829 = (*gem_v_advance);
        (void)(_t829.fn(_t829.env, NULL, 0));
#line 426 "compiler/main.gem"
    GemVal _t830 = (*gem_v_parse_not);
        GemVal gem_v_right = _t830.fn(_t830.env, NULL, 0);
#line 427 "compiler/main.gem"
    GemVal _t831[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t831, 3);
    }
    GemVal _t832 = gem_v_left;
    gem_pop_frame();
    return _t832;
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
    GemVal _t834 = (*gem_v_parse_and);
    GemVal gem_v_left = _t834.fn(_t834.env, NULL, 0);
#line 435 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t835 = (*gem_v_peek);
        GemVal _t836 = _t835.fn(_t835.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t836, gem_string("type")), gem_string("or")))) break;
#line 436 "compiler/main.gem"
    GemVal _t837 = (*gem_v_advance);
        (void)(_t837.fn(_t837.env, NULL, 0));
#line 437 "compiler/main.gem"
    GemVal _t838 = (*gem_v_parse_and);
        GemVal gem_v_right = _t838.fn(_t838.env, NULL, 0);
#line 438 "compiler/main.gem"
    GemVal _t839[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t839, 3);
    }
    GemVal _t840 = gem_v_left;
    gem_pop_frame();
    return _t840;
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
    GemVal _t842 = (*gem_v_peek);
    GemVal _t843 = _t842.fn(_t842.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t843, gem_string("line"));
#line 446 "compiler/main.gem"
    GemVal _t844 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t844.fn(_t844.env, NULL, 0);
#line 449 "compiler/main.gem"
    GemVal _t845 = (*gem_v_peek);
    GemVal _t846 = _t845.fn(_t845.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t846, gem_string("type")), gem_string("=")))) {
#line 450 "compiler/main.gem"
    GemVal _t847 = (*gem_v_advance);
        (void)(_t847.fn(_t847.env, NULL, 0));
#line 451 "compiler/main.gem"
    GemVal _t848 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t848.fn(_t848.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t849 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t849, gem_string("tag")), gem_string("var")))) {
#line 453 "compiler/main.gem"
    GemVal _t850 = gem_v_lhs;
    GemVal _t851[] = {gem_table_get(_t850, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t852 = gem_fn_make_assign(NULL, _t851, 3);
            gem_pop_frame();
            return _t852;
        }
#line 455 "compiler/main.gem"
    GemVal _t853 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t853, gem_string("tag")), gem_string("dot")))) {
#line 456 "compiler/main.gem"
    GemVal _t854 = gem_v_lhs;
    GemVal _t855 = gem_v_lhs;
    GemVal _t856[] = {gem_table_get(_t854, gem_string("object")), gem_table_get(_t855, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t857 = gem_fn_make_dot_assign(NULL, _t856, 4);
            gem_pop_frame();
            return _t857;
        }
#line 458 "compiler/main.gem"
    GemVal _t858 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t858, gem_string("tag")), gem_string("index")))) {
#line 459 "compiler/main.gem"
    GemVal _t859 = gem_v_lhs;
    GemVal _t860 = gem_v_lhs;
    GemVal _t861[] = {gem_table_get(_t859, gem_string("object")), gem_table_get(_t860, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t862 = gem_fn_make_index_assign(NULL, _t861, 4);
            gem_pop_frame();
            return _t862;
        }
#line 461 "compiler/main.gem"
    GemVal _t863[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t863, 7));
    }
#line 465 "compiler/main.gem"
    GemVal _t864 = (*gem_v_peek);
    GemVal _t865 = _t864.fn(_t864.env, NULL, 0);
    GemVal _t868;
    if (gem_truthy(gem_eq(gem_table_get(_t865, gem_string("type")), gem_string("+=")))) {
        _t868 = gem_eq(gem_table_get(_t865, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t866 = (*gem_v_peek);
        GemVal _t867 = _t866.fn(_t866.env, NULL, 0);
        _t868 = gem_eq(gem_table_get(_t867, gem_string("type")), gem_string("-="));
    }
    GemVal _t871;
    if (gem_truthy(_t868)) {
        _t871 = _t868;
    } else {
        GemVal _t869 = (*gem_v_peek);
        GemVal _t870 = _t869.fn(_t869.env, NULL, 0);
        _t871 = gem_eq(gem_table_get(_t870, gem_string("type")), gem_string("*="));
    }
    GemVal _t874;
    if (gem_truthy(_t871)) {
        _t874 = _t871;
    } else {
        GemVal _t872 = (*gem_v_peek);
        GemVal _t873 = _t872.fn(_t872.env, NULL, 0);
        _t874 = gem_eq(gem_table_get(_t873, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t874)) {
#line 466 "compiler/main.gem"
    GemVal _t875 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t875.fn(_t875.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t876 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t876, gem_string("value")), gem_int(0));
#line 468 "compiler/main.gem"
    GemVal _t877 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t877.fn(_t877.env, NULL, 0);
#line 469 "compiler/main.gem"
    GemVal _t878 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t878, gem_string("tag")), gem_string("var")))) {
#line 470 "compiler/main.gem"
    GemVal _t879 = gem_v_op_tok;
    GemVal _t880 = gem_v_op_tok;
    GemVal _t881 = gem_v_op_tok;
    GemVal _t882[] = {gem_table_get(_t881, gem_string("value"))};
    GemVal _t883[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t879, gem_string("line")), gem_table_get(_t880, gem_string("col")), gem_len_fn(NULL, _t882, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
            (void)(gem_fn_compile_error(NULL, _t883, 7));
        }
#line 472 "compiler/main.gem"
    GemVal _t884 = gem_v_lhs;
    GemVal _t885 = gem_v_lhs;
    GemVal _t886[] = {gem_table_get(_t885, gem_string("name"))};
    GemVal _t887[] = {gem_v_base_op, gem_fn_make_var(NULL, _t886, 1), gem_v_value};
    GemVal _t888[] = {gem_table_get(_t884, gem_string("name")), gem_fn_make_binop(NULL, _t887, 3), gem_v_start_line};
        GemVal _t889 = gem_fn_make_assign(NULL, _t888, 3);
        gem_pop_frame();
        return _t889;
    }
    GemVal _t890 = gem_v_lhs;
    gem_pop_frame();
    return _t890;
}

struct _closure__anon_21 {
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 485 "compiler/main.gem"
    GemVal _t892[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t892, 1), gem_int(0)))) {
#line 486 "compiler/main.gem"
    GemVal _t893[] = {gem_bool(1)};
        GemVal _t894 = gem_fn_make_bool(NULL, _t893, 1);
        gem_pop_frame();
        return _t894;
    }
#line 488 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 489 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(1);
#line 489 "compiler/main.gem"
    GemVal _t895[] = {gem_v_conditions};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t895, 1);
#line 489 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 489 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 489 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 490 "compiler/main.gem"
    GemVal _t896[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
        gem_v_result = gem_fn_make_binop(NULL, _t896, 3);
    }

    GemVal _t897 = gem_v_result;
    gem_pop_frame();
    return _t897;
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
    GemVal _t898 = (*gem_v_peek);
    GemVal gem_v_t = _t898.fn(_t898.env, NULL, 0);
#line 497 "compiler/main.gem"
    GemVal _t899 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t899, gem_string("type")), gem_string("{")))) {
#line 498 "compiler/main.gem"
    GemVal _t900 = (*gem_v_advance);
        (void)(_t900.fn(_t900.env, NULL, 0));
#line 499 "compiler/main.gem"
    GemVal _t901 = (*gem_v_skip_nl);
        (void)(_t901.fn(_t901.env, NULL, 0));
#line 500 "compiler/main.gem"
    GemVal _t902 = gem_table_new();
    GemVal _t903[] = {gem_string("type")};
    GemVal _t904 = gem_table_new();
    gem_table_set(_t904, gem_int(0), gem_v_target_expr);
    GemVal _t905[] = {gem_fn_make_var(NULL, _t903, 1), _t904, gem_int(0)};
    GemVal _t906[] = {gem_string("table")};
    GemVal _t907[] = {gem_string("=="), gem_fn_make_call(NULL, _t905, 3), gem_fn_make_string(NULL, _t906, 1)};
    gem_table_set(_t902, gem_int(0), gem_fn_make_binop(NULL, _t907, 3));
        GemVal gem_v_conditions = _t902;
#line 501 "compiler/main.gem"
    GemVal _t908 = gem_table_new();
        GemVal gem_v_bindings = _t908;
#line 502 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t909 = (*gem_v_peek);
            GemVal _t910 = _t909.fn(_t909.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t910, gem_string("type")), gem_string("}")))) break;
#line 503 "compiler/main.gem"
    GemVal _t911 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t911.fn(_t911.env, NULL, 0);
#line 504 "compiler/main.gem"
    GemVal _t912 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t912, gem_string("type")), gem_string("NAME")))) {
#line 505 "compiler/main.gem"
    GemVal _t913 = (*gem_v_advance);
                (void)(_t913.fn(_t913.env, NULL, 0));
            } else {
#line 506 "compiler/main.gem"
    GemVal _t914[] = {gem_int(1)};
    GemVal _t915 = (*gem_v_peek_at);
    GemVal _t916 = _t915.fn(_t915.env, _t914, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t916, gem_string("type")), gem_string(":")))) {
#line 508 "compiler/main.gem"
    GemVal _t917 = (*gem_v_advance);
                    (void)(_t917.fn(_t917.env, NULL, 0));
                } else {
#line 510 "compiler/main.gem"
    GemVal _t918 = gem_v_key_tok;
    GemVal _t919 = gem_v_key_tok;
    GemVal _t920 = gem_v_key_tok;
    GemVal _t921[] = {gem_table_get(_t920, gem_string("value"))};
    GemVal _t922[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t918, gem_string("line")), gem_table_get(_t919, gem_string("col")), gem_len_fn(NULL, _t921, 1), gem_string("expected key name in table pattern"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t922, 7));
                }
            }
#line 512 "compiler/main.gem"
    GemVal _t923 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t923, gem_string("value"));
#line 513 "compiler/main.gem"
    GemVal _t924[] = {gem_v_key_name};
            GemVal gem_v_key_str = gem_fn_make_string(NULL, _t924, 1);
#line 514 "compiler/main.gem"
    GemVal _t925[] = {gem_string("has_key")};
    GemVal _t926 = gem_table_new();
    gem_table_set(_t926, gem_int(0), gem_v_target_expr);
    gem_table_set(_t926, gem_int(1), gem_v_key_str);
    GemVal _t927[] = {gem_fn_make_var(NULL, _t925, 1), _t926, gem_int(0)};
    GemVal _t928[] = {gem_v_conditions, gem_fn_make_call(NULL, _t927, 3)};
            (void)(gem_push_fn(NULL, _t928, 2));
#line 515 "compiler/main.gem"
    GemVal _t929[] = {gem_v_target_expr, gem_v_key_str};
            GemVal gem_v_sub_target = gem_fn_make_index(NULL, _t929, 2);
#line 516 "compiler/main.gem"
    GemVal _t930 = (*gem_v_peek);
    GemVal _t931 = _t930.fn(_t930.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t931, gem_string("type")), gem_string(":")))) {
#line 517 "compiler/main.gem"
    GemVal _t932 = (*gem_v_advance);
                (void)(_t932.fn(_t932.env, NULL, 0));
#line 518 "compiler/main.gem"
    GemVal _t933 = (*gem_v_skip_nl);
                (void)(_t933.fn(_t933.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t934[] = {gem_v_sub_target};
    GemVal _t935 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t935.fn(_t935.env, _t934, 1);
#line 520 "compiler/main.gem"
    GemVal _t936 = gem_v_sub;
    GemVal _t937 = gem_table_get(_t936, gem_string("condition"));
    GemVal _t940;
    if (gem_truthy(gem_neq(gem_table_get(_t937, gem_string("tag")), gem_string("bool")))) {
        _t940 = gem_neq(gem_table_get(_t937, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t938 = gem_v_sub;
        GemVal _t939 = gem_table_get(_t938, gem_string("condition"));
        _t940 = gem_neq(gem_table_get(_t939, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t940)) {
#line 521 "compiler/main.gem"
    GemVal _t941 = gem_v_sub;
    GemVal _t942[] = {gem_v_conditions, gem_table_get(_t941, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t942, 2));
                }
#line 523 "compiler/main.gem"
    GemVal _t943 = gem_v_sub;
                GemVal gem_v__for_items_2 = gem_table_get(_t943, gem_string("bindings"));
#line 523 "compiler/main.gem"
                GemVal gem_v__for_i_2 = gem_int(0);
#line 523 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t944[] = {gem_v__for_items_2};
                    if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t944, 1)))) break;
#line 523 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 523 "compiler/main.gem"
                    gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 524 "compiler/main.gem"
    GemVal _t945[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t945, 2));
                }

            } else {
#line 528 "compiler/main.gem"
    GemVal _t946[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t947[] = {gem_v_bindings, gem_fn_make_let(NULL, _t946, 3)};
                (void)(gem_push_fn(NULL, _t947, 2));
            }
#line 530 "compiler/main.gem"
    GemVal _t948 = (*gem_v_skip_nl);
            (void)(_t948.fn(_t948.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t949 = (*gem_v_peek);
    GemVal _t950 = _t949.fn(_t949.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t950, gem_string("type")), gem_string(",")))) {
#line 532 "compiler/main.gem"
    GemVal _t951 = (*gem_v_advance);
                (void)(_t951.fn(_t951.env, NULL, 0));
#line 533 "compiler/main.gem"
    GemVal _t952 = (*gem_v_skip_nl);
                (void)(_t952.fn(_t952.env, NULL, 0));
            }
        }
#line 536 "compiler/main.gem"
    GemVal _t953[] = {gem_string("}")};
    GemVal _t954 = (*gem_v_expect);
        (void)(_t954.fn(_t954.env, _t953, 1));
#line 537 "compiler/main.gem"
    GemVal _t955 = gem_table_new();
    GemVal _t956[] = {gem_v_conditions};
    GemVal _t957 = gem_v_and_chain;
    gem_table_set(_t955, gem_string("condition"), _t957.fn(_t957.env, _t956, 1));
    gem_table_set(_t955, gem_string("bindings"), gem_v_bindings);
        GemVal _t958 = _t955;
        gem_pop_frame();
        return _t958;
    }
#line 541 "compiler/main.gem"
    GemVal _t959 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t959, gem_string("type")), gem_string("[")))) {
#line 542 "compiler/main.gem"
    GemVal _t960 = (*gem_v_advance);
        (void)(_t960.fn(_t960.env, NULL, 0));
#line 543 "compiler/main.gem"
    GemVal _t961 = (*gem_v_skip_nl);
        (void)(_t961.fn(_t961.env, NULL, 0));
#line 544 "compiler/main.gem"
    GemVal _t962 = gem_table_new();
        GemVal gem_v_sub_patterns = _t962;
#line 545 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 546 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t963 = (*gem_v_peek);
            GemVal _t964 = _t963.fn(_t963.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t964, gem_string("type")), gem_string("]")))) break;
#line 547 "compiler/main.gem"
    GemVal _t965[] = {gem_v_idx};
    GemVal _t966[] = {gem_v_target_expr, gem_fn_make_int(NULL, _t965, 1)};
    GemVal _t967[] = {gem_fn_make_index(NULL, _t966, 2)};
    GemVal _t968 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t968.fn(_t968.env, _t967, 1);
#line 548 "compiler/main.gem"
    GemVal _t969[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t969, 2));
#line 549 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t970 = (*gem_v_skip_nl);
            (void)(_t970.fn(_t970.env, NULL, 0));
#line 551 "compiler/main.gem"
    GemVal _t971 = (*gem_v_peek);
    GemVal _t972 = _t971.fn(_t971.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t972, gem_string("type")), gem_string(",")))) {
#line 552 "compiler/main.gem"
    GemVal _t973 = (*gem_v_advance);
                (void)(_t973.fn(_t973.env, NULL, 0));
#line 553 "compiler/main.gem"
    GemVal _t974 = (*gem_v_skip_nl);
                (void)(_t974.fn(_t974.env, NULL, 0));
            }
        }
#line 556 "compiler/main.gem"
    GemVal _t975[] = {gem_string("]")};
    GemVal _t976 = (*gem_v_expect);
        (void)(_t976.fn(_t976.env, _t975, 1));
#line 557 "compiler/main.gem"
    GemVal _t977 = gem_table_new();
    GemVal _t978[] = {gem_string("type")};
    GemVal _t979 = gem_table_new();
    gem_table_set(_t979, gem_int(0), gem_v_target_expr);
    GemVal _t980[] = {gem_fn_make_var(NULL, _t978, 1), _t979, gem_int(0)};
    GemVal _t981[] = {gem_string("table")};
    GemVal _t982[] = {gem_string("=="), gem_fn_make_call(NULL, _t980, 3), gem_fn_make_string(NULL, _t981, 1)};
    gem_table_set(_t977, gem_int(0), gem_fn_make_binop(NULL, _t982, 3));
        GemVal gem_v_conditions = _t977;
#line 558 "compiler/main.gem"
    GemVal _t983[] = {gem_string("len")};
    GemVal _t984 = gem_table_new();
    gem_table_set(_t984, gem_int(0), gem_v_target_expr);
    GemVal _t985[] = {gem_fn_make_var(NULL, _t983, 1), _t984, gem_int(0)};
    GemVal _t986[] = {gem_v_idx};
    GemVal _t987[] = {gem_string("=="), gem_fn_make_call(NULL, _t985, 3), gem_fn_make_int(NULL, _t986, 1)};
    GemVal _t988[] = {gem_v_conditions, gem_fn_make_binop(NULL, _t987, 3)};
        (void)(gem_push_fn(NULL, _t988, 2));
#line 559 "compiler/main.gem"
    GemVal _t989 = gem_table_new();
        GemVal gem_v_bindings = _t989;
#line 560 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 560 "compiler/main.gem"
    GemVal _t990[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t990, 1);
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
    GemVal _t991 = gem_v_sub;
    GemVal _t992 = gem_table_get(_t991, gem_string("condition"));
    GemVal _t995;
    if (gem_truthy(gem_neq(gem_table_get(_t992, gem_string("tag")), gem_string("bool")))) {
        _t995 = gem_neq(gem_table_get(_t992, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t993 = gem_v_sub;
        GemVal _t994 = gem_table_get(_t993, gem_string("condition"));
        _t995 = gem_neq(gem_table_get(_t994, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t995)) {
#line 563 "compiler/main.gem"
    GemVal _t996 = gem_v_sub;
    GemVal _t997[] = {gem_v_conditions, gem_table_get(_t996, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t997, 2));
            }
#line 565 "compiler/main.gem"
    GemVal _t998 = gem_v_sub;
            GemVal gem_v__for_items_3 = gem_table_get(_t998, gem_string("bindings"));
#line 565 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 565 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t999[] = {gem_v__for_items_3};
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t999, 1)))) break;
#line 565 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_3, gem_v__for_i_3);
#line 565 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t1000[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t1000, 2));
            }

        }

#line 569 "compiler/main.gem"
    GemVal _t1001 = gem_table_new();
    GemVal _t1002[] = {gem_v_conditions};
    GemVal _t1003 = gem_v_and_chain;
    gem_table_set(_t1001, gem_string("condition"), _t1003.fn(_t1003.env, _t1002, 1));
    gem_table_set(_t1001, gem_string("bindings"), gem_v_bindings);
        GemVal _t1004 = _t1001;
        gem_pop_frame();
        return _t1004;
    }
#line 573 "compiler/main.gem"
    GemVal _t1005 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1005, gem_string("type")), gem_string("NUMBER")))) {
#line 574 "compiler/main.gem"
    GemVal _t1006 = (*gem_v_advance);
        (void)(_t1006.fn(_t1006.env, NULL, 0));
#line 575 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 576 "compiler/main.gem"
    GemVal _t1007 = gem_v_t;
    GemVal _t1008[] = {gem_table_get(_t1007, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t1008, 1))) {
#line 577 "compiler/main.gem"
    GemVal _t1009 = gem_v_t;
    GemVal _t1010[] = {gem_table_get(_t1009, gem_string("value"))};
    GemVal _t1011[] = {gem_fn_atof(NULL, _t1010, 1)};
            gem_v_lit = gem_fn_make_float(NULL, _t1011, 1);
        } else {
#line 579 "compiler/main.gem"
    GemVal _t1012 = gem_v_t;
    GemVal _t1013[] = {gem_table_get(_t1012, gem_string("value"))};
    GemVal _t1014[] = {gem_fn_str_to_int(NULL, _t1013, 1)};
            gem_v_lit = gem_fn_make_int(NULL, _t1014, 1);
        }
#line 581 "compiler/main.gem"
    GemVal _t1015 = gem_table_new();
    GemVal _t1016[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    gem_table_set(_t1015, gem_string("condition"), gem_fn_make_binop(NULL, _t1016, 3));
    GemVal _t1017 = gem_table_new();
    gem_table_set(_t1015, gem_string("bindings"), _t1017);
        GemVal _t1018 = _t1015;
        gem_pop_frame();
        return _t1018;
    }
#line 583 "compiler/main.gem"
    GemVal _t1019 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1019, gem_string("type")), gem_string("STRING")))) {
#line 584 "compiler/main.gem"
    GemVal _t1020 = (*gem_v_advance);
        (void)(_t1020.fn(_t1020.env, NULL, 0));
#line 585 "compiler/main.gem"
    GemVal _t1021 = gem_table_new();
    GemVal _t1022 = gem_v_t;
    GemVal _t1023[] = {gem_table_get(_t1022, gem_string("value"))};
    GemVal _t1024[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_string(NULL, _t1023, 1)};
    gem_table_set(_t1021, gem_string("condition"), gem_fn_make_binop(NULL, _t1024, 3));
    GemVal _t1025 = gem_table_new();
    gem_table_set(_t1021, gem_string("bindings"), _t1025);
        GemVal _t1026 = _t1021;
        gem_pop_frame();
        return _t1026;
    }
#line 587 "compiler/main.gem"
    GemVal _t1027 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1027, gem_string("type")), gem_string("true")))) {
#line 588 "compiler/main.gem"
    GemVal _t1028 = (*gem_v_advance);
        (void)(_t1028.fn(_t1028.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t1029 = gem_table_new();
    GemVal _t1030[] = {gem_bool(1)};
    GemVal _t1031[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t1030, 1)};
    gem_table_set(_t1029, gem_string("condition"), gem_fn_make_binop(NULL, _t1031, 3));
    GemVal _t1032 = gem_table_new();
    gem_table_set(_t1029, gem_string("bindings"), _t1032);
        GemVal _t1033 = _t1029;
        gem_pop_frame();
        return _t1033;
    }
#line 591 "compiler/main.gem"
    GemVal _t1034 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1034, gem_string("type")), gem_string("false")))) {
#line 592 "compiler/main.gem"
    GemVal _t1035 = (*gem_v_advance);
        (void)(_t1035.fn(_t1035.env, NULL, 0));
#line 593 "compiler/main.gem"
    GemVal _t1036 = gem_table_new();
    GemVal _t1037[] = {gem_bool(0)};
    GemVal _t1038[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t1037, 1)};
    gem_table_set(_t1036, gem_string("condition"), gem_fn_make_binop(NULL, _t1038, 3));
    GemVal _t1039 = gem_table_new();
    gem_table_set(_t1036, gem_string("bindings"), _t1039);
        GemVal _t1040 = _t1036;
        gem_pop_frame();
        return _t1040;
    }
#line 595 "compiler/main.gem"
    GemVal _t1041 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1041, gem_string("type")), gem_string("nil")))) {
#line 596 "compiler/main.gem"
    GemVal _t1042 = (*gem_v_advance);
        (void)(_t1042.fn(_t1042.env, NULL, 0));
#line 597 "compiler/main.gem"
    GemVal _t1043 = gem_table_new();
    GemVal _t1044[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_nil_node(NULL, NULL, 0)};
    gem_table_set(_t1043, gem_string("condition"), gem_fn_make_binop(NULL, _t1044, 3));
    GemVal _t1045 = gem_table_new();
    gem_table_set(_t1043, gem_string("bindings"), _t1045);
        GemVal _t1046 = _t1043;
        gem_pop_frame();
        return _t1046;
    }
#line 601 "compiler/main.gem"
    GemVal _t1047 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1047, gem_string("type")), gem_string("NAME")))) {
#line 602 "compiler/main.gem"
    GemVal _t1048 = (*gem_v_advance);
        (void)(_t1048.fn(_t1048.env, NULL, 0));
#line 603 "compiler/main.gem"
    GemVal _t1049 = gem_table_new();
    GemVal _t1050[] = {gem_bool(1)};
    gem_table_set(_t1049, gem_string("condition"), gem_fn_make_bool(NULL, _t1050, 1));
    GemVal _t1051 = gem_table_new();
    GemVal _t1052 = gem_v_t;
    GemVal _t1053[] = {gem_table_get(_t1052, gem_string("value")), gem_v_target_expr, gem_int(0)};
    gem_table_set(_t1051, gem_int(0), gem_fn_make_let(NULL, _t1053, 3));
    gem_table_set(_t1049, gem_string("bindings"), _t1051);
        GemVal _t1054 = _t1049;
        gem_pop_frame();
        return _t1054;
    }
#line 606 "compiler/main.gem"
    GemVal _t1055 = gem_v_t;
    GemVal _t1056 = gem_v_t;
    GemVal _t1057 = gem_v_t;
    GemVal _t1058[] = {gem_table_get(_t1057, gem_string("value"))};
    GemVal _t1059 = gem_v_t;
    GemVal _t1060[] = {gem_table_get(_t1059, gem_string("type"))};
    GemVal _t1061[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1055, gem_string("line")), gem_table_get(_t1056, gem_string("col")), gem_len_fn(NULL, _t1058, 1), gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t1060, 1)), gem_string("'")), GEM_NIL};
    GemVal _t1062 = gem_fn_compile_error(NULL, _t1061, 7);
    gem_pop_frame();
    return _t1062;
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
    GemVal _t1064 = (*gem_v_peek);
    GemVal gem_v_t = _t1064.fn(_t1064.env, NULL, 0);
#line 615 "compiler/main.gem"
    GemVal _t1065 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1065, gem_string("type")), gem_string("let")))) {
#line 616 "compiler/main.gem"
    GemVal _t1066 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1066, gem_string("line"));
#line 617 "compiler/main.gem"
    GemVal _t1067 = (*gem_v_advance);
        (void)(_t1067.fn(_t1067.env, NULL, 0));
#line 620 "compiler/main.gem"
    GemVal _t1068 = (*gem_v_peek);
    GemVal _t1069 = _t1068.fn(_t1068.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1069, gem_string("type")), gem_string("{")))) {
#line 621 "compiler/main.gem"
    GemVal _t1070 = (*gem_v_advance);
            (void)(_t1070.fn(_t1070.env, NULL, 0));
#line 622 "compiler/main.gem"
    GemVal _t1071 = gem_table_new();
            GemVal gem_v_names = _t1071;
#line 623 "compiler/main.gem"
    GemVal _t1072 = (*gem_v_skip_nl);
            (void)(_t1072.fn(_t1072.env, NULL, 0));
#line 624 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1073 = (*gem_v_peek);
                GemVal _t1074 = _t1073.fn(_t1073.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1074, gem_string("type")), gem_string("}")))) break;
#line 625 "compiler/main.gem"
    GemVal _t1075 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1075.fn(_t1075.env, NULL, 0);
#line 626 "compiler/main.gem"
    GemVal _t1076 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t1076, gem_string("type")), gem_string("NAME")))) {
#line 627 "compiler/main.gem"
    GemVal _t1077 = (*gem_v_advance);
                    (void)(_t1077.fn(_t1077.env, NULL, 0));
                } else {
#line 630 "compiler/main.gem"
    GemVal _t1078 = (*gem_v_advance);
                    (void)(_t1078.fn(_t1078.env, NULL, 0));
                }
#line 632 "compiler/main.gem"
    GemVal _t1079 = gem_v_field_tok;
    GemVal _t1080[] = {gem_v_names, gem_table_get(_t1079, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1080, 2));
#line 633 "compiler/main.gem"
    GemVal _t1081 = (*gem_v_skip_nl);
                (void)(_t1081.fn(_t1081.env, NULL, 0));
#line 634 "compiler/main.gem"
    GemVal _t1082 = (*gem_v_peek);
    GemVal _t1083 = _t1082.fn(_t1082.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1083, gem_string("type")), gem_string(",")))) {
#line 635 "compiler/main.gem"
    GemVal _t1084 = (*gem_v_advance);
                    (void)(_t1084.fn(_t1084.env, NULL, 0));
#line 636 "compiler/main.gem"
    GemVal _t1085 = (*gem_v_skip_nl);
                    (void)(_t1085.fn(_t1085.env, NULL, 0));
                }
            }
#line 639 "compiler/main.gem"
    GemVal _t1086[] = {gem_string("}")};
    GemVal _t1087 = (*gem_v_expect);
            (void)(_t1087.fn(_t1087.env, _t1086, 1));
#line 640 "compiler/main.gem"
    GemVal _t1088[] = {gem_string("=")};
    GemVal _t1089 = (*gem_v_expect);
            (void)(_t1089.fn(_t1089.env, _t1088, 1));
#line 641 "compiler/main.gem"
    GemVal _t1090 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1090.fn(_t1090.env, NULL, 0);
#line 642 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 643 "compiler/main.gem"
    GemVal _t1091[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1091, 1));
#line 644 "compiler/main.gem"
    GemVal _t1092 = gem_table_new();
    GemVal _t1093[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1092, gem_int(0), gem_fn_make_let(NULL, _t1093, 3));
            GemVal gem_v_stmts = _t1092;
#line 645 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 645 "compiler/main.gem"
    GemVal _t1094[] = {gem_v_names};
            GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t1094, 1);
#line 645 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 645 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_5;
#line 645 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 646 "compiler/main.gem"
    GemVal _t1095[] = {gem_v_tmp};
    GemVal _t1096[] = {gem_fn_make_var(NULL, _t1095, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1097[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t1096, 2), gem_v_line};
    GemVal _t1098[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1097, 3)};
                (void)(gem_push_fn(NULL, _t1098, 2));
            }

#line 648 "compiler/main.gem"
    GemVal _t1099 = gem_table_new();
    gem_table_set(_t1099, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1099, gem_string("stmts"), gem_v_stmts);
            GemVal _t1100 = _t1099;
            gem_pop_frame();
            return _t1100;
        }
#line 652 "compiler/main.gem"
    GemVal _t1101 = (*gem_v_peek);
    GemVal _t1102 = _t1101.fn(_t1101.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1102, gem_string("type")), gem_string("[")))) {
#line 653 "compiler/main.gem"
    GemVal _t1103 = (*gem_v_advance);
            (void)(_t1103.fn(_t1103.env, NULL, 0));
#line 654 "compiler/main.gem"
    GemVal _t1104 = gem_table_new();
            GemVal gem_v_names = _t1104;
#line 655 "compiler/main.gem"
    GemVal _t1105 = (*gem_v_skip_nl);
            (void)(_t1105.fn(_t1105.env, NULL, 0));
#line 656 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1106 = (*gem_v_peek);
                GemVal _t1107 = _t1106.fn(_t1106.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1107, gem_string("type")), gem_string("]")))) break;
#line 657 "compiler/main.gem"
    GemVal _t1108[] = {gem_string("NAME")};
    GemVal _t1109 = (*gem_v_expect);
    GemVal _t1110 = _t1109.fn(_t1109.env, _t1108, 1);
    GemVal _t1111[] = {gem_v_names, gem_table_get(_t1110, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1111, 2));
#line 658 "compiler/main.gem"
    GemVal _t1112 = (*gem_v_skip_nl);
                (void)(_t1112.fn(_t1112.env, NULL, 0));
#line 659 "compiler/main.gem"
    GemVal _t1113 = (*gem_v_peek);
    GemVal _t1114 = _t1113.fn(_t1113.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1114, gem_string("type")), gem_string(",")))) {
#line 660 "compiler/main.gem"
    GemVal _t1115 = (*gem_v_advance);
                    (void)(_t1115.fn(_t1115.env, NULL, 0));
#line 661 "compiler/main.gem"
    GemVal _t1116 = (*gem_v_skip_nl);
                    (void)(_t1116.fn(_t1116.env, NULL, 0));
                }
            }
#line 664 "compiler/main.gem"
    GemVal _t1117[] = {gem_string("]")};
    GemVal _t1118 = (*gem_v_expect);
            (void)(_t1118.fn(_t1118.env, _t1117, 1));
#line 665 "compiler/main.gem"
    GemVal _t1119[] = {gem_string("=")};
    GemVal _t1120 = (*gem_v_expect);
            (void)(_t1120.fn(_t1120.env, _t1119, 1));
#line 666 "compiler/main.gem"
    GemVal _t1121 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1121.fn(_t1121.env, NULL, 0);
#line 667 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 668 "compiler/main.gem"
    GemVal _t1122[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1122, 1));
#line 669 "compiler/main.gem"
    GemVal _t1123 = gem_table_new();
    GemVal _t1124[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1123, gem_int(0), gem_fn_make_let(NULL, _t1124, 3));
            GemVal gem_v_stmts = _t1123;
#line 670 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 670 "compiler/main.gem"
    GemVal _t1125[] = {gem_v_names};
            GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t1125, 1);
#line 670 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 670 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_6;
#line 670 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t1126[] = {gem_v_tmp};
    GemVal _t1127[] = {gem_v_di};
    GemVal _t1128[] = {gem_fn_make_var(NULL, _t1126, 1), gem_fn_make_int(NULL, _t1127, 1)};
    GemVal _t1129[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t1128, 2), gem_v_line};
    GemVal _t1130[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1129, 3)};
                (void)(gem_push_fn(NULL, _t1130, 2));
            }

#line 673 "compiler/main.gem"
    GemVal _t1131 = gem_table_new();
    gem_table_set(_t1131, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1131, gem_string("stmts"), gem_v_stmts);
            GemVal _t1132 = _t1131;
            gem_pop_frame();
            return _t1132;
        }
#line 676 "compiler/main.gem"
    GemVal _t1133[] = {gem_string("NAME")};
    GemVal _t1134 = (*gem_v_expect);
    GemVal _t1135 = _t1134.fn(_t1134.env, _t1133, 1);
        GemVal gem_v_name = gem_table_get(_t1135, gem_string("value"));
#line 677 "compiler/main.gem"
    GemVal _t1136[] = {gem_string("=")};
    GemVal _t1137 = (*gem_v_expect);
        (void)(_t1137.fn(_t1137.env, _t1136, 1));
#line 678 "compiler/main.gem"
    GemVal _t1138 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1138.fn(_t1138.env, NULL, 0);
#line 679 "compiler/main.gem"
    GemVal _t1139[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t1140 = gem_fn_make_let(NULL, _t1139, 3);
        gem_pop_frame();
        return _t1140;
    }
#line 683 "compiler/main.gem"
    GemVal _t1141 = gem_v_t;
    GemVal _t1145;
    if (!gem_truthy(gem_eq(gem_table_get(_t1141, gem_string("type")), gem_string("fn")))) {
        _t1145 = gem_eq(gem_table_get(_t1141, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1142[] = {gem_int(1)};
        GemVal _t1143 = (*gem_v_peek_at);
        GemVal _t1144 = _t1143.fn(_t1143.env, _t1142, 1);
        _t1145 = gem_eq(gem_table_get(_t1144, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1145)) {
#line 684 "compiler/main.gem"
    GemVal _t1146 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1146, gem_string("line"));
#line 685 "compiler/main.gem"
    GemVal _t1147 = (*gem_v_advance);
        (void)(_t1147.fn(_t1147.env, NULL, 0));
#line 686 "compiler/main.gem"
    GemVal _t1148[] = {gem_string("NAME")};
    GemVal _t1149 = (*gem_v_expect);
    GemVal _t1150 = _t1149.fn(_t1149.env, _t1148, 1);
        GemVal gem_v_name = gem_table_get(_t1150, gem_string("value"));
#line 687 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 688 "compiler/main.gem"
    GemVal _t1151[] = {gem_v_name};
    GemVal _t1152[] = {gem_v_name};
    GemVal _t1153[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1151, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_add(gem_add(gem_string("use: let "), gem_to_string_fn(NULL, _t1152, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_fn_compile_error(NULL, _t1153, 7));
        }
#line 690 "compiler/main.gem"
    GemVal _t1154[] = {gem_string("(")};
    GemVal _t1155 = (*gem_v_expect);
        (void)(_t1155.fn(_t1155.env, _t1154, 1));
#line 691 "compiler/main.gem"
    GemVal _t1156 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1156.fn(_t1156.env, NULL, 0);
#line 692 "compiler/main.gem"
    GemVal _t1157[] = {gem_string(")")};
    GemVal _t1158 = (*gem_v_expect);
        (void)(_t1158.fn(_t1158.env, _t1157, 1));
#line 693 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 694 "compiler/main.gem"
    GemVal _t1159 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1159.fn(_t1159.env, NULL, 0);
#line 695 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 696 "compiler/main.gem"
    GemVal _t1160[] = {gem_string("end")};
    GemVal _t1161 = (*gem_v_expect);
        (void)(_t1161.fn(_t1161.env, _t1160, 1));
#line 697 "compiler/main.gem"
    GemVal _t1162 = gem_v_pr;
    GemVal _t1163 = gem_v_pr;
    GemVal _t1164 = gem_v_pr;
    GemVal _t1165[] = {gem_v_name, gem_table_get(_t1162, gem_string("params")), gem_table_get(_t1163, gem_string("rest_param")), gem_table_get(_t1164, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t1166 = gem_fn_make_fn_def(NULL, _t1165, 6);
        gem_pop_frame();
        return _t1166;
    }
#line 701 "compiler/main.gem"
    GemVal _t1167 = gem_v_t;
    GemVal _t1169;
    if (gem_truthy(gem_eq(gem_table_get(_t1167, gem_string("type")), gem_string("if")))) {
        _t1169 = gem_eq(gem_table_get(_t1167, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1168 = gem_v_t;
        _t1169 = gem_eq(gem_table_get(_t1168, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1169)) {
#line 702 "compiler/main.gem"
    GemVal _t1170 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1170, gem_string("line"));
#line 703 "compiler/main.gem"
    GemVal _t1171 = (*gem_v_advance);
        (void)(_t1171.fn(_t1171.env, NULL, 0));
#line 704 "compiler/main.gem"
    GemVal _t1172 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1172.fn(_t1172.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t1173 = (*gem_v_peek);
    GemVal _t1174 = _t1173.fn(_t1173.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1174, gem_string("type")), gem_string("then")))) {
#line 706 "compiler/main.gem"
    GemVal _t1175 = (*gem_v_advance);
            (void)(_t1175.fn(_t1175.env, NULL, 0));
        }
#line 708 "compiler/main.gem"
    GemVal _t1176 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1176.fn(_t1176.env, NULL, 0);
#line 709 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 710 "compiler/main.gem"
    GemVal _t1177 = (*gem_v_peek);
    GemVal _t1178 = _t1177.fn(_t1177.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1178, gem_string("type")), gem_string("elif")))) {
#line 712 "compiler/main.gem"
    GemVal _t1179 = gem_table_new();
    GemVal _t1180 = (*gem_v_parse_stmt);
    gem_table_set(_t1179, gem_int(0), _t1180.fn(_t1180.env, NULL, 0));
            gem_v_else_body = _t1179;
        } else {
#line 713 "compiler/main.gem"
    GemVal _t1181 = (*gem_v_peek);
    GemVal _t1182 = _t1181.fn(_t1181.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1182, gem_string("type")), gem_string("else")))) {
#line 714 "compiler/main.gem"
    GemVal _t1183 = (*gem_v_advance);
                (void)(_t1183.fn(_t1183.env, NULL, 0));
#line 715 "compiler/main.gem"
    GemVal _t1184 = (*gem_v_parse_body);
                gem_v_else_body = _t1184.fn(_t1184.env, NULL, 0);
            }
        }
#line 718 "compiler/main.gem"
    GemVal _t1185 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1185, gem_string("type")), gem_string("if")))) {
#line 719 "compiler/main.gem"
    GemVal _t1186[] = {gem_string("end")};
    GemVal _t1187 = (*gem_v_expect);
            (void)(_t1187.fn(_t1187.env, _t1186, 1));
        }
#line 721 "compiler/main.gem"
    GemVal _t1188[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t1189 = gem_fn_make_if(NULL, _t1188, 4);
        gem_pop_frame();
        return _t1189;
    }
#line 725 "compiler/main.gem"
    GemVal _t1190 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1190, gem_string("type")), gem_string("while")))) {
#line 726 "compiler/main.gem"
    GemVal _t1191 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1191, gem_string("line"));
#line 727 "compiler/main.gem"
    GemVal _t1192 = (*gem_v_advance);
        (void)(_t1192.fn(_t1192.env, NULL, 0));
#line 728 "compiler/main.gem"
    GemVal _t1193 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1193.fn(_t1193.env, NULL, 0);
#line 729 "compiler/main.gem"
    GemVal _t1194 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1194.fn(_t1194.env, NULL, 0);
#line 730 "compiler/main.gem"
    GemVal _t1195[] = {gem_string("end")};
    GemVal _t1196 = (*gem_v_expect);
        (void)(_t1196.fn(_t1196.env, _t1195, 1));
#line 731 "compiler/main.gem"
    GemVal _t1197[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t1198 = gem_fn_make_while(NULL, _t1197, 3);
        gem_pop_frame();
        return _t1198;
    }
#line 735 "compiler/main.gem"
    GemVal _t1199 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1199, gem_string("type")), gem_string("for")))) {
#line 736 "compiler/main.gem"
    GemVal _t1200 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1200, gem_string("line"));
#line 737 "compiler/main.gem"
    GemVal _t1201 = (*gem_v_advance);
        (void)(_t1201.fn(_t1201.env, NULL, 0));
#line 738 "compiler/main.gem"
    GemVal _t1202[] = {gem_string("NAME")};
    GemVal _t1203 = (*gem_v_expect);
    GemVal _t1204 = _t1203.fn(_t1203.env, _t1202, 1);
        GemVal gem_v_var_name = gem_table_get(_t1204, gem_string("value"));
#line 740 "compiler/main.gem"
    GemVal _t1205 = (*gem_v_peek);
    GemVal _t1206 = _t1205.fn(_t1205.env, NULL, 0);
    GemVal _t1210;
    if (!gem_truthy(gem_eq(gem_table_get(_t1206, gem_string("type")), gem_string(",")))) {
        _t1210 = gem_eq(gem_table_get(_t1206, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1207[] = {gem_int(1)};
        GemVal _t1208 = (*gem_v_peek_at);
        GemVal _t1209 = _t1208.fn(_t1208.env, _t1207, 1);
        _t1210 = gem_eq(gem_table_get(_t1209, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1210)) {
#line 742 "compiler/main.gem"
    GemVal _t1211 = (*gem_v_advance);
            (void)(_t1211.fn(_t1211.env, NULL, 0));
#line 743 "compiler/main.gem"
    GemVal _t1212[] = {gem_string("NAME")};
    GemVal _t1213 = (*gem_v_expect);
    GemVal _t1214 = _t1213.fn(_t1213.env, _t1212, 1);
            GemVal gem_v_val_name = gem_table_get(_t1214, gem_string("value"));
#line 744 "compiler/main.gem"
    GemVal _t1215[] = {gem_string("in")};
    GemVal _t1216 = (*gem_v_expect);
            (void)(_t1216.fn(_t1216.env, _t1215, 1));
#line 745 "compiler/main.gem"
    GemVal _t1217 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1217.fn(_t1217.env, NULL, 0);
#line 746 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 747 "compiler/main.gem"
    GemVal _t1218[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1218, 1));
#line 748 "compiler/main.gem"
    GemVal _t1219[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t1219, 1));
#line 749 "compiler/main.gem"
    GemVal _t1220[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1220, 1));
#line 750 "compiler/main.gem"
    GemVal _t1221 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1221.fn(_t1221.env, NULL, 0);
#line 751 "compiler/main.gem"
    GemVal _t1222[] = {gem_string("end")};
    GemVal _t1223 = (*gem_v_expect);
            (void)(_t1223.fn(_t1223.env, _t1222, 1));
#line 752 "compiler/main.gem"
    GemVal _t1224 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1224;
#line 753 "compiler/main.gem"
    GemVal _t1225[] = {gem_v_keys_var};
    GemVal _t1226[] = {gem_v_idx_var};
    GemVal _t1227[] = {gem_fn_make_var(NULL, _t1225, 1), gem_fn_make_var(NULL, _t1226, 1)};
    GemVal _t1228[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1227, 2), gem_v_line};
    GemVal _t1229[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1228, 3)};
            (void)(gem_push_fn(NULL, _t1229, 2));
#line 754 "compiler/main.gem"
    GemVal _t1230[] = {gem_v_tbl_var};
    GemVal _t1231[] = {gem_v_keys_var};
    GemVal _t1232[] = {gem_v_idx_var};
    GemVal _t1233[] = {gem_fn_make_var(NULL, _t1231, 1), gem_fn_make_var(NULL, _t1232, 1)};
    GemVal _t1234[] = {gem_fn_make_var(NULL, _t1230, 1), gem_fn_make_index(NULL, _t1233, 2)};
    GemVal _t1235[] = {gem_v_val_name, gem_fn_make_index(NULL, _t1234, 2), gem_v_line};
    GemVal _t1236[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1235, 3)};
            (void)(gem_push_fn(NULL, _t1236, 2));
#line 755 "compiler/main.gem"
    GemVal _t1237[] = {gem_v_idx_var};
    GemVal _t1238[] = {gem_int(1)};
    GemVal _t1239[] = {gem_string("+"), gem_fn_make_var(NULL, _t1237, 1), gem_fn_make_int(NULL, _t1238, 1)};
    GemVal _t1240[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1239, 3), gem_v_line};
    GemVal _t1241[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1240, 3)};
            (void)(gem_push_fn(NULL, _t1241, 2));
#line 756 "compiler/main.gem"
            GemVal gem_v__for_i_7 = gem_int(0);
#line 756 "compiler/main.gem"
    GemVal _t1242[] = {gem_v_fbody};
            GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1242, 1);
#line 756 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 756 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_7;
#line 756 "compiler/main.gem"
                gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 757 "compiler/main.gem"
    GemVal _t1243[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1243, 2));
            }

#line 759 "compiler/main.gem"
    GemVal _t1244[] = {gem_v_idx_var};
    GemVal _t1245[] = {gem_string("len")};
    GemVal _t1246 = gem_table_new();
    GemVal _t1247[] = {gem_v_keys_var};
    gem_table_set(_t1246, gem_int(0), gem_fn_make_var(NULL, _t1247, 1));
    GemVal _t1248[] = {gem_fn_make_var(NULL, _t1245, 1), _t1246, gem_int(0)};
    GemVal _t1249[] = {gem_string("<"), gem_fn_make_var(NULL, _t1244, 1), gem_fn_make_call(NULL, _t1248, 3)};
    GemVal _t1250[] = {gem_fn_make_binop(NULL, _t1249, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1250, 3);
#line 764 "compiler/main.gem"
    GemVal _t1251 = gem_table_new();
    gem_table_set(_t1251, gem_string("tag"), gem_string("block"));
    GemVal _t1252 = gem_table_new();
    GemVal _t1253[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t1252, gem_int(0), gem_fn_make_let(NULL, _t1253, 3));
    GemVal _t1254[] = {gem_string("keys")};
    GemVal _t1255 = gem_table_new();
    GemVal _t1256[] = {gem_v_tbl_var};
    gem_table_set(_t1255, gem_int(0), gem_fn_make_var(NULL, _t1256, 1));
    GemVal _t1257[] = {gem_fn_make_var(NULL, _t1254, 1), _t1255, gem_int(0)};
    GemVal _t1258[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t1257, 3), gem_v_line};
    gem_table_set(_t1252, gem_int(1), gem_fn_make_let(NULL, _t1258, 3));
    GemVal _t1259[] = {gem_int(0)};
    GemVal _t1260[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1259, 1), gem_v_line};
    gem_table_set(_t1252, gem_int(2), gem_fn_make_let(NULL, _t1260, 3));
    gem_table_set(_t1252, gem_int(3), gem_v_while_node);
    gem_table_set(_t1251, gem_string("stmts"), _t1252);
            GemVal _t1261 = _t1251;
            gem_pop_frame();
            return _t1261;
        } else {
#line 770 "compiler/main.gem"
    GemVal _t1262 = (*gem_v_peek);
    GemVal _t1263 = _t1262.fn(_t1262.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1263, gem_string("type")), gem_string("in")))) {
#line 772 "compiler/main.gem"
    GemVal _t1264 = (*gem_v_advance);
                (void)(_t1264.fn(_t1264.env, NULL, 0));
#line 773 "compiler/main.gem"
    GemVal _t1265 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1265.fn(_t1265.env, NULL, 0);
#line 774 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 775 "compiler/main.gem"
    GemVal _t1266[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1266, 1));
#line 776 "compiler/main.gem"
    GemVal _t1267[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1267, 1));
#line 777 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1268.fn(_t1268.env, NULL, 0);
#line 778 "compiler/main.gem"
    GemVal _t1269[] = {gem_string("end")};
    GemVal _t1270 = (*gem_v_expect);
                (void)(_t1270.fn(_t1270.env, _t1269, 1));
#line 779 "compiler/main.gem"
    GemVal _t1271 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1271;
#line 780 "compiler/main.gem"
    GemVal _t1272[] = {gem_v_items_var};
    GemVal _t1273[] = {gem_v_idx_var};
    GemVal _t1274[] = {gem_fn_make_var(NULL, _t1272, 1), gem_fn_make_var(NULL, _t1273, 1)};
    GemVal _t1275[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1274, 2), gem_v_line};
    GemVal _t1276[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1275, 3)};
                (void)(gem_push_fn(NULL, _t1276, 2));
#line 781 "compiler/main.gem"
    GemVal _t1277[] = {gem_v_idx_var};
    GemVal _t1278[] = {gem_int(1)};
    GemVal _t1279[] = {gem_string("+"), gem_fn_make_var(NULL, _t1277, 1), gem_fn_make_int(NULL, _t1278, 1)};
    GemVal _t1280[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1279, 3), gem_v_line};
    GemVal _t1281[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1280, 3)};
                (void)(gem_push_fn(NULL, _t1281, 2));
#line 782 "compiler/main.gem"
                GemVal gem_v__for_i_8 = gem_int(0);
#line 782 "compiler/main.gem"
    GemVal _t1282[] = {gem_v_fbody};
                GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1282, 1);
#line 782 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 782 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_8;
#line 782 "compiler/main.gem"
                    gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 783 "compiler/main.gem"
    GemVal _t1283[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1283, 2));
                }

#line 785 "compiler/main.gem"
    GemVal _t1284[] = {gem_v_idx_var};
    GemVal _t1285[] = {gem_string("len")};
    GemVal _t1286 = gem_table_new();
    GemVal _t1287[] = {gem_v_items_var};
    gem_table_set(_t1286, gem_int(0), gem_fn_make_var(NULL, _t1287, 1));
    GemVal _t1288[] = {gem_fn_make_var(NULL, _t1285, 1), _t1286, gem_int(0)};
    GemVal _t1289[] = {gem_string("<"), gem_fn_make_var(NULL, _t1284, 1), gem_fn_make_call(NULL, _t1288, 3)};
    GemVal _t1290[] = {gem_fn_make_binop(NULL, _t1289, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1290, 3);
#line 790 "compiler/main.gem"
    GemVal _t1291 = gem_table_new();
    gem_table_set(_t1291, gem_string("tag"), gem_string("block"));
    GemVal _t1292 = gem_table_new();
    GemVal _t1293[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t1292, gem_int(0), gem_fn_make_let(NULL, _t1293, 3));
    GemVal _t1294[] = {gem_int(0)};
    GemVal _t1295[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1294, 1), gem_v_line};
    gem_table_set(_t1292, gem_int(1), gem_fn_make_let(NULL, _t1295, 3));
    gem_table_set(_t1292, gem_int(2), gem_v_while_node);
    gem_table_set(_t1291, gem_string("stmts"), _t1292);
                GemVal _t1296 = _t1291;
                gem_pop_frame();
                return _t1296;
            } else {
#line 795 "compiler/main.gem"
    GemVal _t1297 = (*gem_v_peek);
    GemVal _t1298 = _t1297.fn(_t1297.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1298, gem_string("type")), gem_string("=")))) {
#line 797 "compiler/main.gem"
    GemVal _t1299 = (*gem_v_advance);
                    (void)(_t1299.fn(_t1299.env, NULL, 0));
#line 798 "compiler/main.gem"
    GemVal _t1300 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1300.fn(_t1300.env, NULL, 0);
#line 799 "compiler/main.gem"
    GemVal _t1301[] = {gem_string(",")};
    GemVal _t1302 = (*gem_v_expect);
                    (void)(_t1302.fn(_t1302.env, _t1301, 1));
#line 800 "compiler/main.gem"
    GemVal _t1303 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1303.fn(_t1303.env, NULL, 0);
#line 801 "compiler/main.gem"
    GemVal _t1304 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1304.fn(_t1304.env, NULL, 0);
#line 802 "compiler/main.gem"
    GemVal _t1305[] = {gem_string("end")};
    GemVal _t1306 = (*gem_v_expect);
                    (void)(_t1306.fn(_t1306.env, _t1305, 1));
#line 803 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 804 "compiler/main.gem"
    GemVal _t1307[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1307, 1));
#line 805 "compiler/main.gem"
    GemVal _t1308[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1308, 1));
#line 806 "compiler/main.gem"
    GemVal _t1309 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1309;
#line 807 "compiler/main.gem"
    GemVal _t1310[] = {gem_v_idx_var};
    GemVal _t1311[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1310, 1), gem_v_line};
    GemVal _t1312[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1311, 3)};
                    (void)(gem_push_fn(NULL, _t1312, 2));
#line 808 "compiler/main.gem"
    GemVal _t1313[] = {gem_v_idx_var};
    GemVal _t1314[] = {gem_int(1)};
    GemVal _t1315[] = {gem_string("+"), gem_fn_make_var(NULL, _t1313, 1), gem_fn_make_int(NULL, _t1314, 1)};
    GemVal _t1316[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1315, 3), gem_v_line};
    GemVal _t1317[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1316, 3)};
                    (void)(gem_push_fn(NULL, _t1317, 2));
#line 809 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 809 "compiler/main.gem"
    GemVal _t1318[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1318, 1);
#line 809 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 809 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_9;
#line 809 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 810 "compiler/main.gem"
    GemVal _t1319[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1319, 2));
                    }

#line 812 "compiler/main.gem"
    GemVal _t1320[] = {gem_v_idx_var};
    GemVal _t1321[] = {gem_v_limit_var};
    GemVal _t1322[] = {gem_string("<"), gem_fn_make_var(NULL, _t1320, 1), gem_fn_make_var(NULL, _t1321, 1)};
    GemVal _t1323[] = {gem_fn_make_binop(NULL, _t1322, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1323, 3);
#line 817 "compiler/main.gem"
    GemVal _t1324 = gem_table_new();
    gem_table_set(_t1324, gem_string("tag"), gem_string("block"));
    GemVal _t1325 = gem_table_new();
    GemVal _t1326[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1325, gem_int(0), gem_fn_make_let(NULL, _t1326, 3));
    GemVal _t1327[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1325, gem_int(1), gem_fn_make_let(NULL, _t1327, 3));
    gem_table_set(_t1325, gem_int(2), gem_v_while_node);
    gem_table_set(_t1324, gem_string("stmts"), _t1325);
                    GemVal _t1328 = _t1324;
                    gem_pop_frame();
                    return _t1328;
                } else {
#line 823 "compiler/main.gem"
    GemVal _t1329 = (*gem_v_peek);
    GemVal _t1330 = _t1329.fn(_t1329.env, NULL, 0);
    GemVal _t1331 = (*gem_v_peek);
    GemVal _t1332 = _t1331.fn(_t1331.env, NULL, 0);
    GemVal _t1333 = (*gem_v_peek);
    GemVal _t1334 = _t1333.fn(_t1333.env, NULL, 0);
    GemVal _t1335[] = {gem_table_get(_t1334, gem_string("value"))};
    GemVal _t1336[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1330, gem_string("line")), gem_table_get(_t1332, gem_string("col")), gem_len_fn(NULL, _t1335, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t1336, 7));
                }
            }
        }
    }
#line 828 "compiler/main.gem"
    GemVal _t1337 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1337, gem_string("type")), gem_string("match")))) {
#line 829 "compiler/main.gem"
    GemVal _t1338 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1338, gem_string("line"));
#line 830 "compiler/main.gem"
    GemVal _t1339 = (*gem_v_advance);
        (void)(_t1339.fn(_t1339.env, NULL, 0));
#line 831 "compiler/main.gem"
    GemVal _t1340 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1340.fn(_t1340.env, NULL, 0);
#line 832 "compiler/main.gem"
    GemVal _t1341 = (*gem_v_skip_nl);
        (void)(_t1341.fn(_t1341.env, NULL, 0));
#line 833 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 834 "compiler/main.gem"
    GemVal _t1342[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1342, 1));
#line 835 "compiler/main.gem"
    GemVal _t1343[] = {gem_v_match_var};
        GemVal gem_v_target_var_expr = gem_fn_make_var(NULL, _t1343, 1);
#line 836 "compiler/main.gem"
    GemVal _t1344 = gem_table_new();
        GemVal gem_v_whens = _t1344;
#line 837 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1345 = (*gem_v_peek);
            GemVal _t1346 = _t1345.fn(_t1345.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1346, gem_string("type")), gem_string("when")))) break;
#line 838 "compiler/main.gem"
    GemVal _t1347 = (*gem_v_advance);
            (void)(_t1347.fn(_t1347.env, NULL, 0));
#line 840 "compiler/main.gem"
    GemVal _t1348 = (*gem_v_peek);
    GemVal _t1349 = _t1348.fn(_t1348.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1349, gem_string("type"));
#line 841 "compiler/main.gem"
    GemVal _t1350[] = {gem_int(1)};
    GemVal _t1351 = (*gem_v_peek_at);
    GemVal _t1352 = _t1351.fn(_t1351.env, _t1350, 1);
            GemVal gem_v_next = gem_table_get(_t1352, gem_string("type"));
#line 842 "compiler/main.gem"
    GemVal _t1353;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1353 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1353 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1353)) {
#line 843 "compiler/main.gem"
    GemVal _t1354[] = {gem_v_target_var_expr};
    GemVal _t1355 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1355.fn(_t1355.env, _t1354, 1);
#line 844 "compiler/main.gem"
    GemVal _t1356 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1356.fn(_t1356.env, NULL, 0);
#line 845 "compiler/main.gem"
    GemVal _t1357 = gem_v_pat;
    GemVal _t1358 = gem_v_pat;
    GemVal _t1359[] = {gem_table_get(_t1357, gem_string("condition")), gem_v_wbody, gem_table_get(_t1358, gem_string("bindings"))};
    GemVal _t1360[] = {gem_v_whens, gem_fn_make_when(NULL, _t1359, 3)};
                (void)(gem_push_fn(NULL, _t1360, 2));
            } else {
#line 846 "compiler/main.gem"
    GemVal _t1365;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1365 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1361;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1361 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1361 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1362;
        if (gem_truthy(_t1361)) {
                _t1362 = _t1361;
        } else {
                _t1362 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1363;
        if (gem_truthy(_t1362)) {
                _t1363 = _t1362;
        } else {
                _t1363 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1364;
        if (gem_truthy(_t1363)) {
                _t1364 = _t1363;
        } else {
                _t1364 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1365 = _t1364;
    }
                if (gem_truthy(_t1365)) {
#line 848 "compiler/main.gem"
    GemVal _t1366[] = {gem_v_target_var_expr};
    GemVal _t1367 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1367.fn(_t1367.env, _t1366, 1);
#line 849 "compiler/main.gem"
    GemVal _t1368 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1368.fn(_t1368.env, NULL, 0);
#line 850 "compiler/main.gem"
    GemVal _t1369 = gem_v_pat;
    GemVal _t1370 = gem_v_pat;
    GemVal _t1371[] = {gem_table_get(_t1369, gem_string("condition")), gem_v_wbody, gem_table_get(_t1370, gem_string("bindings"))};
    GemVal _t1372[] = {gem_v_whens, gem_fn_make_when(NULL, _t1371, 3)};
                    (void)(gem_push_fn(NULL, _t1372, 2));
                } else {
#line 853 "compiler/main.gem"
    GemVal _t1373 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1373.fn(_t1373.env, NULL, 0);
#line 854 "compiler/main.gem"
    GemVal _t1374 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1374.fn(_t1374.env, NULL, 0);
#line 855 "compiler/main.gem"
    GemVal _t1375[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1376 = gem_table_new();
    GemVal _t1377[] = {gem_fn_make_binop(NULL, _t1375, 3), gem_v_wbody, _t1376};
    GemVal _t1378[] = {gem_v_whens, gem_fn_make_when(NULL, _t1377, 3)};
                    (void)(gem_push_fn(NULL, _t1378, 2));
                }
            }
#line 857 "compiler/main.gem"
    GemVal _t1379 = (*gem_v_skip_nl);
            (void)(_t1379.fn(_t1379.env, NULL, 0));
        }
#line 859 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 860 "compiler/main.gem"
    GemVal _t1380 = (*gem_v_peek);
    GemVal _t1381 = _t1380.fn(_t1380.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1381, gem_string("type")), gem_string("else")))) {
#line 861 "compiler/main.gem"
    GemVal _t1382 = (*gem_v_advance);
            (void)(_t1382.fn(_t1382.env, NULL, 0));
#line 862 "compiler/main.gem"
    GemVal _t1383 = (*gem_v_parse_body);
            gem_v_else_body = _t1383.fn(_t1383.env, NULL, 0);
        }
#line 864 "compiler/main.gem"
    GemVal _t1384[] = {gem_string("end")};
    GemVal _t1385 = (*gem_v_expect);
        (void)(_t1385.fn(_t1385.env, _t1384, 1));
#line 865 "compiler/main.gem"
    GemVal _t1386[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
        GemVal _t1387 = gem_fn_make_match(NULL, _t1386, 5);
        gem_pop_frame();
        return _t1387;
    }
#line 869 "compiler/main.gem"
    GemVal _t1388 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1388, gem_string("type")), gem_string("return")))) {
#line 870 "compiler/main.gem"
    GemVal _t1389 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1389, gem_string("line"));
#line 871 "compiler/main.gem"
    GemVal _t1390 = (*gem_v_advance);
        (void)(_t1390.fn(_t1390.env, NULL, 0));
#line 872 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 873 "compiler/main.gem"
    GemVal _t1391 = (*gem_v_peek);
    GemVal _t1392 = _t1391.fn(_t1391.env, NULL, 0);
    GemVal _t1395;
    if (!gem_truthy(gem_neq(gem_table_get(_t1392, gem_string("type")), gem_string("NEWLINE")))) {
        _t1395 = gem_neq(gem_table_get(_t1392, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1393 = (*gem_v_peek);
        GemVal _t1394 = _t1393.fn(_t1393.env, NULL, 0);
        _t1395 = gem_neq(gem_table_get(_t1394, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1398;
    if (!gem_truthy(_t1395)) {
        _t1398 = _t1395;
    } else {
        GemVal _t1396 = (*gem_v_peek);
        GemVal _t1397 = _t1396.fn(_t1396.env, NULL, 0);
        _t1398 = gem_neq(gem_table_get(_t1397, gem_string("type")), gem_string("end"));
    }
    GemVal _t1401;
    if (!gem_truthy(_t1398)) {
        _t1401 = _t1398;
    } else {
        GemVal _t1399 = (*gem_v_peek);
        GemVal _t1400 = _t1399.fn(_t1399.env, NULL, 0);
        _t1401 = gem_neq(gem_table_get(_t1400, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1404;
    if (!gem_truthy(_t1401)) {
        _t1404 = _t1401;
    } else {
        GemVal _t1402 = (*gem_v_peek);
        GemVal _t1403 = _t1402.fn(_t1402.env, NULL, 0);
        _t1404 = gem_neq(gem_table_get(_t1403, gem_string("type")), gem_string("else"));
    }
    GemVal _t1407;
    if (!gem_truthy(_t1404)) {
        _t1407 = _t1404;
    } else {
        GemVal _t1405 = (*gem_v_peek);
        GemVal _t1406 = _t1405.fn(_t1405.env, NULL, 0);
        _t1407 = gem_neq(gem_table_get(_t1406, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1407)) {
#line 874 "compiler/main.gem"
    GemVal _t1408 = (*gem_v_parse_expr);
            gem_v_value = _t1408.fn(_t1408.env, NULL, 0);
        }
#line 876 "compiler/main.gem"
    GemVal _t1409[] = {gem_v_value, gem_v_line};
        GemVal _t1410 = gem_fn_make_return(NULL, _t1409, 2);
        gem_pop_frame();
        return _t1410;
    }
#line 880 "compiler/main.gem"
    GemVal _t1411 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1411, gem_string("type")), gem_string("break")))) {
#line 881 "compiler/main.gem"
    GemVal _t1412 = (*gem_v_advance);
        (void)(_t1412.fn(_t1412.env, NULL, 0));
#line 882 "compiler/main.gem"
        GemVal _t1413 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1413;
    }
#line 886 "compiler/main.gem"
    GemVal _t1414 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1414, gem_string("type")), gem_string("continue")))) {
#line 887 "compiler/main.gem"
    GemVal _t1415 = (*gem_v_advance);
        (void)(_t1415.fn(_t1415.env, NULL, 0));
#line 888 "compiler/main.gem"
        GemVal _t1416 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1416;
    }
#line 892 "compiler/main.gem"
    GemVal _t1417 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1417, gem_string("type")), gem_string("load")))) {
#line 893 "compiler/main.gem"
    GemVal _t1418 = (*gem_v_advance);
        (void)(_t1418.fn(_t1418.env, NULL, 0));
#line 894 "compiler/main.gem"
    GemVal _t1419[] = {gem_string("STRING")};
    GemVal _t1420 = (*gem_v_expect);
    GemVal _t1421 = _t1420.fn(_t1420.env, _t1419, 1);
        GemVal gem_v_path = gem_table_get(_t1421, gem_string("value"));
#line 895 "compiler/main.gem"
        GemVal gem_v_alias_name = GEM_NIL;
#line 896 "compiler/main.gem"
        GemVal gem_v_selective = GEM_NIL;
#line 897 "compiler/main.gem"
    GemVal _t1422 = (*gem_v_peek);
    GemVal _t1423 = _t1422.fn(_t1422.env, NULL, 0);
    GemVal _t1426;
    if (!gem_truthy(gem_eq(gem_table_get(_t1423, gem_string("type")), gem_string("NAME")))) {
        _t1426 = gem_eq(gem_table_get(_t1423, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1424 = (*gem_v_peek);
        GemVal _t1425 = _t1424.fn(_t1424.env, NULL, 0);
        _t1426 = gem_eq(gem_table_get(_t1425, gem_string("value")), gem_string("as"));
    }
        if (gem_truthy(_t1426)) {
#line 898 "compiler/main.gem"
    GemVal _t1427 = (*gem_v_advance);
            (void)(_t1427.fn(_t1427.env, NULL, 0));
#line 899 "compiler/main.gem"
    GemVal _t1428[] = {gem_string("NAME")};
    GemVal _t1429 = (*gem_v_expect);
    GemVal _t1430 = _t1429.fn(_t1429.env, _t1428, 1);
            gem_v_alias_name = gem_table_get(_t1430, gem_string("value"));
        } else {
#line 900 "compiler/main.gem"
    GemVal _t1431 = (*gem_v_peek);
    GemVal _t1432 = _t1431.fn(_t1431.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1432, gem_string("type")), gem_string("(")))) {
#line 901 "compiler/main.gem"
    GemVal _t1433 = (*gem_v_advance);
                (void)(_t1433.fn(_t1433.env, NULL, 0));
#line 902 "compiler/main.gem"
    GemVal _t1434 = gem_table_new();
    GemVal _t1435[] = {gem_string("NAME")};
    GemVal _t1436 = (*gem_v_expect);
    GemVal _t1437 = _t1436.fn(_t1436.env, _t1435, 1);
    gem_table_set(_t1434, gem_int(0), gem_table_get(_t1437, gem_string("value")));
                gem_v_selective = _t1434;
#line 903 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1438 = (*gem_v_peek);
                    GemVal _t1439 = _t1438.fn(_t1438.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1439, gem_string("type")), gem_string(",")))) break;
#line 904 "compiler/main.gem"
    GemVal _t1440 = (*gem_v_advance);
                    (void)(_t1440.fn(_t1440.env, NULL, 0));
#line 905 "compiler/main.gem"
    GemVal _t1441[] = {gem_string("NAME")};
    GemVal _t1442 = (*gem_v_expect);
    GemVal _t1443 = _t1442.fn(_t1442.env, _t1441, 1);
    GemVal _t1444[] = {gem_v_selective, gem_table_get(_t1443, gem_string("value"))};
                    (void)(gem_push_fn(NULL, _t1444, 2));
                }
#line 907 "compiler/main.gem"
    GemVal _t1445[] = {gem_string(")")};
    GemVal _t1446 = (*gem_v_expect);
                (void)(_t1446.fn(_t1446.env, _t1445, 1));
            }
        }
#line 909 "compiler/main.gem"
    GemVal _t1447 = gem_table_new();
    gem_table_set(_t1447, gem_string("tag"), gem_string("load"));
    gem_table_set(_t1447, gem_string("path"), gem_v_path);
    gem_table_set(_t1447, gem_string("alias"), gem_v_alias_name);
    gem_table_set(_t1447, gem_string("selective"), gem_v_selective);
        GemVal _t1448 = _t1447;
        gem_pop_frame();
        return _t1448;
    }
#line 913 "compiler/main.gem"
    GemVal _t1449 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1449, gem_string("type")), gem_string("export")))) {
#line 914 "compiler/main.gem"
    GemVal _t1450 = (*gem_v_advance);
        (void)(_t1450.fn(_t1450.env, NULL, 0));
#line 915 "compiler/main.gem"
    GemVal _t1451 = gem_table_new();
    GemVal _t1452[] = {gem_string("NAME")};
    GemVal _t1453 = (*gem_v_expect);
    GemVal _t1454 = _t1453.fn(_t1453.env, _t1452, 1);
    gem_table_set(_t1451, gem_int(0), gem_table_get(_t1454, gem_string("value")));
        GemVal gem_v_names = _t1451;
#line 916 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1455 = (*gem_v_peek);
            GemVal _t1456 = _t1455.fn(_t1455.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1456, gem_string("type")), gem_string(",")))) break;
#line 917 "compiler/main.gem"
    GemVal _t1457 = (*gem_v_advance);
            (void)(_t1457.fn(_t1457.env, NULL, 0));
#line 918 "compiler/main.gem"
    GemVal _t1458[] = {gem_string("NAME")};
    GemVal _t1459 = (*gem_v_expect);
    GemVal _t1460 = _t1459.fn(_t1459.env, _t1458, 1);
    GemVal _t1461[] = {gem_v_names, gem_table_get(_t1460, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t1461, 2));
        }
#line 920 "compiler/main.gem"
    GemVal _t1462[] = {gem_v_names};
        GemVal _t1463 = gem_fn_make_export(NULL, _t1462, 1);
        gem_pop_frame();
        return _t1463;
    }
#line 924 "compiler/main.gem"
    GemVal _t1464 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1464, gem_string("type")), gem_string("extern")))) {
#line 925 "compiler/main.gem"
    GemVal _t1465 = (*gem_v_advance);
        (void)(_t1465.fn(_t1465.env, NULL, 0));
#line 926 "compiler/main.gem"
        GemVal gem_v_is_blocking = gem_bool(0);
#line 927 "compiler/main.gem"
    GemVal _t1466 = (*gem_v_peek);
    GemVal _t1467 = _t1466.fn(_t1466.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1467, gem_string("type")), gem_string("blocking")))) {
#line 928 "compiler/main.gem"
            gem_v_is_blocking = gem_bool(1);
#line 929 "compiler/main.gem"
    GemVal _t1468 = (*gem_v_advance);
            (void)(_t1468.fn(_t1468.env, NULL, 0));
        }
#line 931 "compiler/main.gem"
    GemVal _t1469 = (*gem_v_peek);
    GemVal _t1470 = _t1469.fn(_t1469.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1470, gem_string("type")), gem_string("fn")))) {
#line 932 "compiler/main.gem"
    GemVal _t1471 = (*gem_v_advance);
            (void)(_t1471.fn(_t1471.env, NULL, 0));
#line 933 "compiler/main.gem"
    GemVal _t1472[] = {gem_string("NAME")};
    GemVal _t1473 = (*gem_v_expect);
    GemVal _t1474 = _t1473.fn(_t1473.env, _t1472, 1);
            GemVal gem_v_name = gem_table_get(_t1474, gem_string("value"));
#line 934 "compiler/main.gem"
    GemVal _t1475[] = {gem_string("(")};
    GemVal _t1476 = (*gem_v_expect);
            (void)(_t1476.fn(_t1476.env, _t1475, 1));
#line 935 "compiler/main.gem"
    GemVal _t1477 = gem_table_new();
            GemVal gem_v_eparams = _t1477;
#line 936 "compiler/main.gem"
    GemVal _t1478 = (*gem_v_peek);
    GemVal _t1479 = _t1478.fn(_t1478.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1479, gem_string("type")), gem_string(")")))) {
#line 937 "compiler/main.gem"
    GemVal _t1480[] = {gem_string("NAME")};
    GemVal _t1481 = (*gem_v_expect);
    GemVal _t1482 = _t1481.fn(_t1481.env, _t1480, 1);
                GemVal gem_v_pname = gem_table_get(_t1482, gem_string("value"));
#line 938 "compiler/main.gem"
    GemVal _t1483[] = {gem_string(":")};
    GemVal _t1484 = (*gem_v_expect);
                (void)(_t1484.fn(_t1484.env, _t1483, 1));
#line 939 "compiler/main.gem"
    GemVal _t1485[] = {gem_string("NAME")};
    GemVal _t1486 = (*gem_v_expect);
    GemVal _t1487 = _t1486.fn(_t1486.env, _t1485, 1);
                GemVal gem_v_ptype = gem_table_get(_t1487, gem_string("value"));
#line 940 "compiler/main.gem"
    GemVal _t1488[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1489[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1488, 2)};
                (void)(gem_push_fn(NULL, _t1489, 2));
#line 941 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1490 = (*gem_v_peek);
                    GemVal _t1491 = _t1490.fn(_t1490.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1491, gem_string("type")), gem_string(",")))) break;
#line 942 "compiler/main.gem"
    GemVal _t1492 = (*gem_v_advance);
                    (void)(_t1492.fn(_t1492.env, NULL, 0));
#line 943 "compiler/main.gem"
    GemVal _t1493[] = {gem_string("NAME")};
    GemVal _t1494 = (*gem_v_expect);
    GemVal _t1495 = _t1494.fn(_t1494.env, _t1493, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1495, gem_string("value"));
#line 944 "compiler/main.gem"
    GemVal _t1496[] = {gem_string(":")};
    GemVal _t1497 = (*gem_v_expect);
                    (void)(_t1497.fn(_t1497.env, _t1496, 1));
#line 945 "compiler/main.gem"
    GemVal _t1498[] = {gem_string("NAME")};
    GemVal _t1499 = (*gem_v_expect);
    GemVal _t1500 = _t1499.fn(_t1499.env, _t1498, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1500, gem_string("value"));
#line 946 "compiler/main.gem"
    GemVal _t1501[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1502[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1501, 2)};
                    (void)(gem_push_fn(NULL, _t1502, 2));
                }
            }
#line 949 "compiler/main.gem"
    GemVal _t1503[] = {gem_string(")")};
    GemVal _t1504 = (*gem_v_expect);
            (void)(_t1504.fn(_t1504.env, _t1503, 1));
#line 950 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 951 "compiler/main.gem"
    GemVal _t1505 = (*gem_v_peek);
    GemVal _t1506 = _t1505.fn(_t1505.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1506, gem_string("type")), gem_string("->")))) {
#line 952 "compiler/main.gem"
    GemVal _t1507 = (*gem_v_advance);
                (void)(_t1507.fn(_t1507.env, NULL, 0));
#line 953 "compiler/main.gem"
    GemVal _t1508[] = {gem_string("NAME")};
    GemVal _t1509 = (*gem_v_expect);
    GemVal _t1510 = _t1509.fn(_t1509.env, _t1508, 1);
                gem_v_ret_type = gem_table_get(_t1510, gem_string("value"));
            }
#line 955 "compiler/main.gem"
    GemVal _t1511[] = {gem_v_name, gem_v_eparams, gem_v_ret_type, gem_v_is_blocking};
            GemVal _t1512 = gem_fn_make_extern_fn(NULL, _t1511, 4);
            gem_pop_frame();
            return _t1512;
        } else {
#line 956 "compiler/main.gem"
    GemVal _t1515;
    if (!gem_truthy(gem_not(gem_v_is_blocking))) {
        _t1515 = gem_not(gem_v_is_blocking);
    } else {
        GemVal _t1513 = (*gem_v_peek);
        GemVal _t1514 = _t1513.fn(_t1513.env, NULL, 0);
        _t1515 = gem_eq(gem_table_get(_t1514, gem_string("type")), gem_string("NAME"));
    }
    GemVal _t1518;
    if (!gem_truthy(_t1515)) {
        _t1518 = _t1515;
    } else {
        GemVal _t1516 = (*gem_v_peek);
        GemVal _t1517 = _t1516.fn(_t1516.env, NULL, 0);
        _t1518 = gem_eq(gem_table_get(_t1517, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1518)) {
#line 957 "compiler/main.gem"
    GemVal _t1519 = (*gem_v_advance);
                (void)(_t1519.fn(_t1519.env, NULL, 0));
#line 958 "compiler/main.gem"
    GemVal _t1520[] = {gem_string("STRING")};
    GemVal _t1521 = (*gem_v_expect);
    GemVal _t1522 = _t1521.fn(_t1521.env, _t1520, 1);
                GemVal gem_v_path = gem_table_get(_t1522, gem_string("value"));
#line 959 "compiler/main.gem"
    GemVal _t1523[] = {gem_v_path};
                GemVal _t1524 = gem_fn_make_extern_include(NULL, _t1523, 1);
                gem_pop_frame();
                return _t1524;
            } else {
#line 961 "compiler/main.gem"
    GemVal _t1525 = (*gem_v_peek);
    GemVal _t1526 = _t1525.fn(_t1525.env, NULL, 0);
    GemVal _t1527 = (*gem_v_peek);
    GemVal _t1528 = _t1527.fn(_t1527.env, NULL, 0);
    GemVal _t1529 = (*gem_v_peek);
    GemVal _t1530 = _t1529.fn(_t1529.env, NULL, 0);
    GemVal _t1531[] = {gem_table_get(_t1530, gem_string("value"))};
    GemVal _t1532[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1526, gem_string("line")), gem_table_get(_t1528, gem_string("col")), gem_len_fn(NULL, _t1531, 1), gem_string("expected 'fn' after 'extern blocking'"), GEM_NIL};
                (void)(gem_fn_compile_error(NULL, _t1532, 7));
            }
        }
    }
#line 966 "compiler/main.gem"
    GemVal _t1533 = gem_v_t;
    GemVal _t1535;
    if (!gem_truthy(gem_eq(gem_table_get(_t1533, gem_string("type")), gem_string("NAME")))) {
        _t1535 = gem_eq(gem_table_get(_t1533, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1534 = gem_v_t;
        _t1535 = gem_eq(gem_table_get(_t1534, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1535)) {
#line 967 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 968 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1536[] = {gem_v_rla};
            GemVal _t1537 = (*gem_v_peek_at);
            GemVal _t1538 = _t1537.fn(_t1537.env, _t1536, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1538, gem_string("type")), gem_string("NEWLINE")))) break;
#line 969 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 971 "compiler/main.gem"
    GemVal _t1539[] = {gem_v_rla};
    GemVal _t1540 = (*gem_v_peek_at);
    GemVal _t1541 = _t1540.fn(_t1540.env, _t1539, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1541, gem_string("type")), gem_string("when")))) {
#line 972 "compiler/main.gem"
    GemVal _t1542 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1542, gem_string("line"));
#line 973 "compiler/main.gem"
    GemVal _t1543 = (*gem_v_advance);
            (void)(_t1543.fn(_t1543.env, NULL, 0));
#line 974 "compiler/main.gem"
    GemVal _t1544 = (*gem_v_skip_nl);
            (void)(_t1544.fn(_t1544.env, NULL, 0));
#line 975 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 976 "compiler/main.gem"
    GemVal _t1545[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_add(gem_string("_recv_"), gem_to_string_fn(NULL, _t1545, 1));
#line 977 "compiler/main.gem"
    GemVal _t1546[] = {gem_v_recv_var};
            GemVal gem_v_recv_var_expr = gem_fn_make_var(NULL, _t1546, 1);
#line 978 "compiler/main.gem"
    GemVal _t1547 = gem_table_new();
            GemVal gem_v_rarms = _t1547;
#line 979 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1548 = (*gem_v_peek);
                GemVal _t1549 = _t1548.fn(_t1548.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1549, gem_string("type")), gem_string("when")))) break;
#line 980 "compiler/main.gem"
    GemVal _t1550 = (*gem_v_advance);
                (void)(_t1550.fn(_t1550.env, NULL, 0));
#line 981 "compiler/main.gem"
    GemVal _t1551 = (*gem_v_peek);
    GemVal _t1552 = _t1551.fn(_t1551.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1552, gem_string("type"));
#line 982 "compiler/main.gem"
    GemVal _t1553[] = {gem_int(1)};
    GemVal _t1554 = (*gem_v_peek_at);
    GemVal _t1555 = _t1554.fn(_t1554.env, _t1553, 1);
                GemVal gem_v_rnext = gem_table_get(_t1555, gem_string("type"));
#line 983 "compiler/main.gem"
    GemVal _t1556;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1556 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1556 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1556)) {
#line 984 "compiler/main.gem"
    GemVal _t1557[] = {gem_v_recv_var_expr};
    GemVal _t1558 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1558.fn(_t1558.env, _t1557, 1);
#line 985 "compiler/main.gem"
    GemVal _t1559 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1559.fn(_t1559.env, NULL, 0);
#line 986 "compiler/main.gem"
    GemVal _t1560 = gem_table_new();
    gem_table_set(_t1560, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1560, gem_string("body"), gem_v_rwbody);
    GemVal _t1561[] = {gem_v_rarms, _t1560};
                    (void)(gem_push_fn(NULL, _t1561, 2));
                } else {
#line 987 "compiler/main.gem"
    GemVal _t1567;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1567 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1562;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1562 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1562 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1563;
        if (gem_truthy(_t1562)) {
                _t1563 = _t1562;
        } else {
                _t1563 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1564;
        if (gem_truthy(_t1563)) {
                _t1564 = _t1563;
        } else {
                _t1564 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1565;
        if (gem_truthy(_t1564)) {
                _t1565 = _t1564;
        } else {
                _t1565 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1566;
        if (gem_truthy(_t1565)) {
                _t1566 = _t1565;
        } else {
                _t1566 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1567 = _t1566;
    }
                    if (gem_truthy(_t1567)) {
#line 988 "compiler/main.gem"
    GemVal _t1568[] = {gem_v_recv_var_expr};
    GemVal _t1569 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1569.fn(_t1569.env, _t1568, 1);
#line 989 "compiler/main.gem"
    GemVal _t1570 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1570.fn(_t1570.env, NULL, 0);
#line 990 "compiler/main.gem"
    GemVal _t1571 = gem_table_new();
    gem_table_set(_t1571, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1571, gem_string("body"), gem_v_rwbody);
    GemVal _t1572[] = {gem_v_rarms, _t1571};
                        (void)(gem_push_fn(NULL, _t1572, 2));
                    } else {
#line 992 "compiler/main.gem"
    GemVal _t1573 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1573.fn(_t1573.env, NULL, 0);
#line 993 "compiler/main.gem"
    GemVal _t1574 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1574.fn(_t1574.env, NULL, 0);
#line 994 "compiler/main.gem"
    GemVal _t1575 = gem_table_new();
    GemVal _t1576 = gem_table_new();
    GemVal _t1577[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    gem_table_set(_t1576, gem_string("condition"), gem_fn_make_binop(NULL, _t1577, 3));
    GemVal _t1578 = gem_table_new();
    gem_table_set(_t1576, gem_string("bindings"), _t1578);
    gem_table_set(_t1575, gem_string("pattern"), _t1576);
    gem_table_set(_t1575, gem_string("body"), gem_v_rwbody);
    GemVal _t1579[] = {gem_v_rarms, _t1575};
                        (void)(gem_push_fn(NULL, _t1579, 2));
                    }
                }
#line 996 "compiler/main.gem"
    GemVal _t1580 = (*gem_v_skip_nl);
                (void)(_t1580.fn(_t1580.env, NULL, 0));
            }
#line 998 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 999 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 1000 "compiler/main.gem"
    GemVal _t1581 = (*gem_v_peek);
    GemVal _t1582 = _t1581.fn(_t1581.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1582, gem_string("type")), gem_string("after")))) {
#line 1001 "compiler/main.gem"
    GemVal _t1583 = (*gem_v_advance);
                (void)(_t1583.fn(_t1583.env, NULL, 0));
#line 1002 "compiler/main.gem"
    GemVal _t1584 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1584.fn(_t1584.env, NULL, 0);
#line 1003 "compiler/main.gem"
    GemVal _t1585 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1585.fn(_t1585.env, NULL, 0);
            }
#line 1005 "compiler/main.gem"
    GemVal _t1586[] = {gem_string("end")};
    GemVal _t1587 = (*gem_v_expect);
            (void)(_t1587.fn(_t1587.env, _t1586, 1));
#line 1006 "compiler/main.gem"
    GemVal _t1588[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
            GemVal _t1589 = gem_fn_make_receive_match(NULL, _t1588, 5);
            gem_pop_frame();
            return _t1589;
        }
    }
#line 1011 "compiler/main.gem"
    GemVal _t1590 = (*gem_v_parse_expr);
    GemVal _t1591 = _t1590.fn(_t1590.env, NULL, 0);
    gem_pop_frame();
    return _t1591;
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
#line 1020 "compiler/main.gem"
    GemVal _t1593 = gem_table_new();
    GemVal gem_v_stmts = _t1593;
#line 1021 "compiler/main.gem"
    GemVal _t1594 = (*gem_v_skip_nl);
    (void)(_t1594.fn(_t1594.env, NULL, 0));
#line 1022 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1595 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1595.fn(_t1595.env, NULL, 0)))) break;
#line 1023 "compiler/main.gem"
    GemVal _t1596 = (*gem_v_parse_stmt);
    GemVal _t1597[] = {gem_v_stmts, _t1596.fn(_t1596.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1597, 2));
#line 1024 "compiler/main.gem"
    GemVal _t1598 = (*gem_v_skip_nl);
        (void)(_t1598.fn(_t1598.env, NULL, 0));
    }
#line 1026 "compiler/main.gem"
    GemVal _t1599[] = {gem_v_stmts};
    GemVal _t1600 = gem_fn_make_program(NULL, _t1599, 1);
    gem_pop_frame();
    return _t1600;
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
    struct _closure__anon_1 *_t421 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t421->gem_v_pos = gem_v_pos;
    _t421->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t421);
#line 30 "compiler/main.gem"
    struct _closure__anon_2 *_t423 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t423->gem_v_pos = gem_v_pos;
    _t423->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t423);
#line 34 "compiler/main.gem"
    struct _closure__anon_3 *_t425 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t425->gem_v_pos = gem_v_pos;
    _t425->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t425);
#line 40 "compiler/main.gem"
    struct _closure__anon_4 *_t428 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t428->gem_v_pos = gem_v_pos;
    _t428->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t428);
#line 44 "compiler/main.gem"
    struct _closure__anon_5 *_t439 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t439->gem_v_file = gem_v_file;
    _t439->gem_v_pos = gem_v_pos;
    _t439->gem_v_source = gem_v_source;
    _t439->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t439);
#line 53 "compiler/main.gem"
    struct _closure__anon_6 *_t441 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t441->gem_v_pos = gem_v_pos;
    _t441->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t441);
#line 65 "compiler/main.gem"
    struct _closure__anon_7 *_t487 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t487->gem_v_advance = gem_v_advance;
    _t487->gem_v_expect = gem_v_expect;
    _t487->gem_v_file = gem_v_file;
    _t487->gem_v_peek = gem_v_peek;
    _t487->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t487);
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
    struct _closure__anon_8 *_t513 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t513->gem_v_at_end = gem_v_at_end;
    _t513->gem_v_parse_stmt = gem_v_parse_stmt;
    _t513->gem_v_peek = gem_v_peek;
    _t513->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t513);
#line 121 "compiler/main.gem"
    struct _closure__anon_9 *_t524 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t524->gem_v_at_end = gem_v_at_end;
    _t524->gem_v_parse_stmt = gem_v_parse_stmt;
    _t524->gem_v_peek = gem_v_peek;
    _t524->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t524);
#line 134 "compiler/main.gem"
    struct _closure__anon_10 *_t662 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t662->gem_v_advance = gem_v_advance;
    _t662->gem_v_at_end = gem_v_at_end;
    _t662->gem_v_expect = gem_v_expect;
    _t662->gem_v_file = gem_v_file;
    _t662->gem_v_fn_depth = gem_v_fn_depth;
    _t662->gem_v_parse_expr = gem_v_parse_expr;
    _t662->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t662->gem_v_parse_params = gem_v_parse_params;
    _t662->gem_v_peek = gem_v_peek;
    _t662->gem_v_peek_at = gem_v_peek_at;
    _t662->gem_v_skip_nl = gem_v_skip_nl;
    _t662->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t662);
#line 265 "compiler/main.gem"
    struct _closure__anon_11 *_t753 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t753->gem_v_advance = gem_v_advance;
    _t753->gem_v_expect = gem_v_expect;
    _t753->gem_v_fn_depth = gem_v_fn_depth;
    _t753->gem_v_parse_atom = gem_v_parse_atom;
    _t753->gem_v_parse_expr = gem_v_parse_expr;
    _t753->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t753->gem_v_parse_params = gem_v_parse_params;
    _t753->gem_v_peek = gem_v_peek;
    _t753->gem_v_peek_at = gem_v_peek_at;
    _t753->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t753);
#line 371 "compiler/main.gem"
    struct _closure__anon_12 *_t762 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t762->gem_v_advance = gem_v_advance;
    _t762->gem_v_parse_call = gem_v_parse_call;
    _t762->gem_v_parse_unary = gem_v_parse_unary;
    _t762->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t762);
#line 380 "compiler/main.gem"
    struct _closure__anon_13 *_t777 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t777->gem_v_advance = gem_v_advance;
    _t777->gem_v_parse_unary = gem_v_parse_unary;
    _t777->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t777);
#line 391 "compiler/main.gem"
    struct _closure__anon_14 *_t789 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t789->gem_v_advance = gem_v_advance;
    _t789->gem_v_parse_mul = gem_v_parse_mul;
    _t789->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t789);
#line 402 "compiler/main.gem"
    struct _closure__anon_15 *_t816 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t816->gem_v_advance = gem_v_advance;
    _t816->gem_v_parse_add = gem_v_parse_add;
    _t816->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t816);
#line 413 "compiler/main.gem"
    struct _closure__anon_16 *_t825 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t825->gem_v_advance = gem_v_advance;
    _t825->gem_v_parse_compare = gem_v_parse_compare;
    _t825->gem_v_parse_not = gem_v_parse_not;
    _t825->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t825);
#line 422 "compiler/main.gem"
    struct _closure__anon_17 *_t833 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t833->gem_v_advance = gem_v_advance;
    _t833->gem_v_parse_not = gem_v_parse_not;
    _t833->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t833);
#line 433 "compiler/main.gem"
    struct _closure__anon_18 *_t841 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t841->gem_v_advance = gem_v_advance;
    _t841->gem_v_parse_and = gem_v_parse_and;
    _t841->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t841);
#line 444 "compiler/main.gem"
    struct _closure__anon_19 *_t891 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t891->gem_v_advance = gem_v_advance;
    _t891->gem_v_file = gem_v_file;
    _t891->gem_v_parse_expr = gem_v_parse_expr;
    _t891->gem_v_parse_or = gem_v_parse_or;
    _t891->gem_v_peek = gem_v_peek;
    _t891->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t891);
#line 479 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 483 "compiler/main.gem"
    struct _closure__anon_20 *_t1063 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1063->gem_v_advance = gem_v_advance;
    _t1063->gem_v_expect = gem_v_expect;
    _t1063->gem_v_file = gem_v_file;
    _t1063->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1063->gem_v_peek = gem_v_peek;
    _t1063->gem_v_peek_at = gem_v_peek_at;
    _t1063->gem_v_skip_nl = gem_v_skip_nl;
    _t1063->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t1063);
#line 611 "compiler/main.gem"
    struct _closure__anon_22 *_t1592 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1592->gem_v_advance = gem_v_advance;
    _t1592->gem_v_expect = gem_v_expect;
    _t1592->gem_v_file = gem_v_file;
    _t1592->gem_v_fn_depth = gem_v_fn_depth;
    _t1592->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1592->gem_v_parse_body = gem_v_parse_body;
    _t1592->gem_v_parse_expr = gem_v_parse_expr;
    _t1592->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1592->gem_v_parse_params = gem_v_parse_params;
    _t1592->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1592->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1592->gem_v_peek = gem_v_peek;
    _t1592->gem_v_peek_at = gem_v_peek_at;
    _t1592->gem_v_skip_nl = gem_v_skip_nl;
    _t1592->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1592);
#line 1015 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 1019 "compiler/main.gem"
    struct _closure__anon_23 *_t1601 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1601->gem_v_at_end = gem_v_at_end;
    _t1601->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1601->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1601);
    GemVal _t1602 = gem_table_new();
    gem_table_set(_t1602, gem_string("parse"), gem_v_parse);
    GemVal _t1603 = _t1602;
    gem_pop_frame();
    return _t1603;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1604[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1604, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1605[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1605, 1);
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
    GemVal _t1606[] = {gem_v_b, gem_string("\\\\")};
            (void)(gem_buf_push_fn(NULL, _t1606, 2));
        } else {
#line 16 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 17 "compiler/main.gem"
    GemVal _t1607[] = {gem_v_b, gem_string("\\\"")};
                (void)(gem_buf_push_fn(NULL, _t1607, 2));
            } else {
#line 18 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 19 "compiler/main.gem"
    GemVal _t1608[] = {gem_v_b, gem_string("\\n")};
                    (void)(gem_buf_push_fn(NULL, _t1608, 2));
                } else {
#line 20 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 21 "compiler/main.gem"
    GemVal _t1609[] = {gem_v_b, gem_string("\\r")};
                        (void)(gem_buf_push_fn(NULL, _t1609, 2));
                    } else {
#line 22 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 23 "compiler/main.gem"
    GemVal _t1610[] = {gem_v_b, gem_string("\\t")};
                            (void)(gem_buf_push_fn(NULL, _t1610, 2));
                        } else {
#line 24 "compiler/main.gem"
    GemVal _t1611[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1611, 1), gem_int(0)))) {
#line 25 "compiler/main.gem"
    GemVal _t1612[] = {gem_v_b, gem_string("\\0")};
                                (void)(gem_buf_push_fn(NULL, _t1612, 2));
                            } else {
#line 27 "compiler/main.gem"
    GemVal _t1613[] = {gem_v_b, gem_v_ch};
                                (void)(gem_buf_push_fn(NULL, _t1613, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 30 "compiler/main.gem"
    GemVal _t1614[] = {gem_v_b};
    GemVal _t1615 = gem_buf_str_fn(NULL, _t1614, 1);
    gem_pop_frame();
    return _t1615;
}

static GemVal gem_fn_repeat_str(void *_env, GemVal *args, int argc) {
#line 33 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("repeat_str", "compiler/main.gem", 33);
#line 34 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
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
    GemVal _t1616[] = {gem_v_b, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t1616, 2));
    }

#line 38 "compiler/main.gem"
    GemVal _t1617[] = {gem_v_b};
    GemVal _t1618 = gem_buf_str_fn(NULL, _t1617, 1);
    gem_pop_frame();
    return _t1618;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1619[] = {gem_string("    "), gem_v_indent};
    GemVal _t1620 = gem_fn_repeat_str(NULL, _t1619, 2);
    gem_pop_frame();
    return _t1620;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1621[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1621, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1622[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1622, 1);
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
    GemVal _t1623 = gem_v_s;
    gem_pop_frame();
    return _t1623;
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
    GemVal _t1624 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1624;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1625 = gem_table_new();
    GemVal gem_v_result = _t1625;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1626[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1626, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1627[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1627, 1)))) break;
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
    GemVal _t1628[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1628, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1629[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1629, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1630 = gem_v_result;
    gem_pop_frame();
    return _t1630;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1631 = gem_table_new();
    GemVal gem_v_result = _t1631;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1632[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1632, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1633[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1633, 1)))) break;
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

    GemVal _t1634 = gem_v_result;
    gem_pop_frame();
    return _t1634;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1635 = gem_table_new();
    GemVal gem_v_s = _t1635;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1636[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1636, 1)))) break;
#line 95 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 95 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1637 = gem_v_s;
    gem_pop_frame();
    return _t1637;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 101);
    GemVal _t1638[] = {gem_v_node};
    GemVal _t1640;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1638, 1), gem_string("table")))) {
        _t1640 = gem_eq(gem_type_fn(NULL, _t1638, 1), gem_string("table"));
    } else {
        GemVal _t1639 = gem_v_node;
        _t1640 = gem_eq(gem_table_get(_t1639, gem_string("tag")), gem_v_tag);
    }
    GemVal _t1641 = _t1640;
    gem_pop_frame();
    return _t1641;
}

static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 107 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_block_lets", "compiler/main.gem", 107);
#line 108 "compiler/main.gem"
    GemVal _t1642[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn_is_node(NULL, _t1642, 2))) {
        {
#line 109 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 109 "compiler/main.gem"
    GemVal _t1643 = gem_v_stmt;
    GemVal _t1644[] = {gem_table_get(_t1643, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1644, 1);
#line 109 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 109 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 109 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 110 "compiler/main.gem"
    GemVal _t1645 = gem_v_stmt;
    GemVal _t1646[] = {gem_table_get(gem_table_get(_t1645, gem_string("stmts")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1646, 2))) {
#line 111 "compiler/main.gem"
    GemVal _t1647 = gem_v_stmt;
    GemVal _t1648 = gem_table_get(gem_table_get(_t1647, gem_string("stmts")), gem_v_j);
    GemVal _t1649[] = {gem_v_result, gem_table_get(_t1648, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1649, 2));
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
#line 117 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_top_let_names", "compiler/main.gem", 117);
    {
#line 118 "compiler/main.gem"
        GemVal gem_v__for_i_9 = gem_int(0);
#line 118 "compiler/main.gem"
    GemVal _t1650[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1650, 1);
#line 118 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 118 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_9;
#line 118 "compiler/main.gem"
            gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 119 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v_stmts, gem_v_i);
#line 120 "compiler/main.gem"
    GemVal _t1651[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t1651, 2))) {
#line 121 "compiler/main.gem"
    GemVal _t1652 = gem_v_s;
    GemVal _t1653[] = {gem_v_result, gem_table_get(_t1652, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1653, 2));
            }
#line 123 "compiler/main.gem"
    GemVal _t1654[] = {gem_v_s, gem_v_result};
            (void)(gem_fn_collect_block_lets(NULL, _t1654, 2));
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
#line 230 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1667[] = {(*gem_v_tmp_counter)};
    GemVal _t1668 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1667, 1));
    gem_pop_frame();
    return _t1668;
}

struct _closure__anon_25 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 235 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1670[] = {(*gem_v_anon_counter)};
    GemVal _t1671 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1670, 1));
    gem_pop_frame();
    return _t1671;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1673[] = {gem_v_name};
    GemVal _t1674 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1673, 1));
    gem_pop_frame();
    return _t1674;
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
#line 249 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 250 "compiler/main.gem"
        GemVal _t1675 = GEM_NIL;
        gem_pop_frame();
        return _t1675;
    }
#line 252 "compiler/main.gem"
    GemVal _t1676[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1676, 1), gem_string("table")))) {
#line 253 "compiler/main.gem"
        GemVal _t1677 = GEM_NIL;
        gem_pop_frame();
        return _t1677;
    }
#line 255 "compiler/main.gem"
    GemVal _t1678 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1678, gem_string("tag")), GEM_NIL))) {
#line 256 "compiler/main.gem"
        GemVal _t1679 = GEM_NIL;
        gem_pop_frame();
        return _t1679;
    }
#line 259 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1680[] = {gem_v__match_11};
    GemVal _t1682;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1680, 1), gem_string("table")))) {
        _t1682 = gem_eq(gem_type_fn(NULL, _t1680, 1), gem_string("table"));
    } else {
        GemVal _t1681[] = {gem_v__match_11, gem_string("tag")};
        _t1682 = gem_has_key_fn(NULL, _t1681, 2);
    }
    GemVal _t1683;
    if (!gem_truthy(_t1682)) {
        _t1683 = _t1682;
    } else {
        _t1683 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1685;
    if (!gem_truthy(_t1683)) {
        _t1685 = _t1683;
    } else {
        GemVal _t1684[] = {gem_v__match_11, gem_string("name")};
        _t1685 = gem_has_key_fn(NULL, _t1684, 2);
    }
    if (gem_truthy(_t1685)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 261 "compiler/main.gem"
    GemVal _t1686[] = {gem_v_defined, gem_v_name};
    GemVal _t1690;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1686, 2)))) {
        _t1690 = gem_not(gem_fn_set_contains(NULL, _t1686, 2));
    } else {
        GemVal _t1687[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1689;
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1687, 2)))) {
                _t1689 = gem_not(gem_fn_set_contains(NULL, _t1687, 2));
        } else {
                GemVal _t1688[] = {(*gem_v_local_names), gem_v_name};
                _t1689 = gem_fn_set_contains(NULL, _t1688, 2);
        }
        _t1690 = _t1689;
    }
    GemVal _t1692;
    if (!gem_truthy(_t1690)) {
        _t1692 = _t1690;
    } else {
        GemVal _t1691[] = {(*gem_v_defined_fns), gem_v_name};
        _t1692 = gem_not(gem_fn_set_contains(NULL, _t1691, 2));
    }
        if (gem_truthy(_t1692)) {
#line 262 "compiler/main.gem"
    GemVal _t1693[] = {gem_v_free, gem_v_name};
            GemVal _t1694 = gem_fn_set_add(NULL, _t1693, 2);
            gem_pop_frame();
            return _t1694;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1695[] = {gem_v__match_11};
    GemVal _t1697;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1695, 1), gem_string("table")))) {
        _t1697 = gem_eq(gem_type_fn(NULL, _t1695, 1), gem_string("table"));
    } else {
        GemVal _t1696[] = {gem_v__match_11, gem_string("tag")};
        _t1697 = gem_has_key_fn(NULL, _t1696, 2);
    }
    GemVal _t1698;
    if (!gem_truthy(_t1697)) {
        _t1698 = _t1697;
    } else {
        _t1698 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1700;
    if (!gem_truthy(_t1698)) {
        _t1700 = _t1698;
    } else {
        GemVal _t1699[] = {gem_v__match_11, gem_string("value")};
        _t1700 = gem_has_key_fn(NULL, _t1699, 2);
    }
    if (gem_truthy(_t1700)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 265 "compiler/main.gem"
    GemVal _t1701[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1702 = (*gem_v_collect_free_node);
        GemVal _t1703 = _t1702.fn(_t1702.env, _t1701, 3);
        gem_pop_frame();
        return _t1703;
    } else {
    GemVal _t1704[] = {gem_v__match_11};
    GemVal _t1706;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1704, 1), gem_string("table")))) {
        _t1706 = gem_eq(gem_type_fn(NULL, _t1704, 1), gem_string("table"));
    } else {
        GemVal _t1705[] = {gem_v__match_11, gem_string("tag")};
        _t1706 = gem_has_key_fn(NULL, _t1705, 2);
    }
    GemVal _t1707;
    if (!gem_truthy(_t1706)) {
        _t1707 = _t1706;
    } else {
        _t1707 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1709;
    if (!gem_truthy(_t1707)) {
        _t1709 = _t1707;
    } else {
        GemVal _t1708[] = {gem_v__match_11, gem_string("name")};
        _t1709 = gem_has_key_fn(NULL, _t1708, 2);
    }
    GemVal _t1711;
    if (!gem_truthy(_t1709)) {
        _t1711 = _t1709;
    } else {
        GemVal _t1710[] = {gem_v__match_11, gem_string("value")};
        _t1711 = gem_has_key_fn(NULL, _t1710, 2);
    }
    if (gem_truthy(_t1711)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 267 "compiler/main.gem"
    GemVal _t1712[] = {gem_v_defined, gem_v_name};
    GemVal _t1716;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1712, 2)))) {
        _t1716 = gem_not(gem_fn_set_contains(NULL, _t1712, 2));
    } else {
        GemVal _t1713[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1715;
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1713, 2)))) {
                _t1715 = gem_not(gem_fn_set_contains(NULL, _t1713, 2));
        } else {
                GemVal _t1714[] = {(*gem_v_local_names), gem_v_name};
                _t1715 = gem_fn_set_contains(NULL, _t1714, 2);
        }
        _t1716 = _t1715;
    }
    GemVal _t1718;
    if (!gem_truthy(_t1716)) {
        _t1718 = _t1716;
    } else {
        GemVal _t1717[] = {(*gem_v_defined_fns), gem_v_name};
        _t1718 = gem_not(gem_fn_set_contains(NULL, _t1717, 2));
    }
        if (gem_truthy(_t1718)) {
#line 268 "compiler/main.gem"
    GemVal _t1719[] = {gem_v_free, gem_v_name};
            (void)(gem_fn_set_add(NULL, _t1719, 2));
        }
#line 270 "compiler/main.gem"
    GemVal _t1720[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1721 = (*gem_v_collect_free_node);
        GemVal _t1722 = _t1721.fn(_t1721.env, _t1720, 3);
        gem_pop_frame();
        return _t1722;
    } else {
    GemVal _t1723[] = {gem_v__match_11};
    GemVal _t1725;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1723, 1), gem_string("table")))) {
        _t1725 = gem_eq(gem_type_fn(NULL, _t1723, 1), gem_string("table"));
    } else {
        GemVal _t1724[] = {gem_v__match_11, gem_string("tag")};
        _t1725 = gem_has_key_fn(NULL, _t1724, 2);
    }
    GemVal _t1726;
    if (!gem_truthy(_t1725)) {
        _t1726 = _t1725;
    } else {
        _t1726 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1728;
    if (!gem_truthy(_t1726)) {
        _t1728 = _t1726;
    } else {
        GemVal _t1727[] = {gem_v__match_11, gem_string("params")};
        _t1728 = gem_has_key_fn(NULL, _t1727, 2);
    }
    GemVal _t1730;
    if (!gem_truthy(_t1728)) {
        _t1730 = _t1728;
    } else {
        GemVal _t1729[] = {gem_v__match_11, gem_string("rest_param")};
        _t1730 = gem_has_key_fn(NULL, _t1729, 2);
    }
    GemVal _t1732;
    if (!gem_truthy(_t1730)) {
        _t1732 = _t1730;
    } else {
        GemVal _t1731[] = {gem_v__match_11, gem_string("block_param")};
        _t1732 = gem_has_key_fn(NULL, _t1731, 2);
    }
    GemVal _t1734;
    if (!gem_truthy(_t1732)) {
        _t1734 = _t1732;
    } else {
        GemVal _t1733[] = {gem_v__match_11, gem_string("body")};
        _t1734 = gem_has_key_fn(NULL, _t1733, 2);
    }
    if (gem_truthy(_t1734)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_11, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 272 "compiler/main.gem"
    GemVal _t1735[] = {gem_v_params};
    GemVal _t1736[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1735, 1)};
        GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1736, 2);
#line 273 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 274 "compiler/main.gem"
    GemVal _t1737[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1737, 2));
        }
#line 276 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 277 "compiler/main.gem"
    GemVal _t1738[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1738, 2));
        }
#line 279 "compiler/main.gem"
    GemVal _t1739[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1740 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1740.fn(_t1740.env, _t1739, 2);
        {
#line 280 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 280 "compiler/main.gem"
    GemVal _t1741[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_keys_12 = gem_keys_fn(NULL, _t1741, 1);
#line 280 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 280 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1742[] = {gem_v__for_keys_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1742, 1)))) break;
#line 280 "compiler/main.gem"
                GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_12, gem_v__for_i_12);
#line 280 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_12, gem_table_get(gem_v__for_keys_12, gem_v__for_i_12));
#line 280 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 281 "compiler/main.gem"
    GemVal _t1743[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn_set_add(NULL, _t1743, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1744[] = {gem_v__match_11};
    GemVal _t1746;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1744, 1), gem_string("table")))) {
        _t1746 = gem_eq(gem_type_fn(NULL, _t1744, 1), gem_string("table"));
    } else {
        GemVal _t1745[] = {gem_v__match_11, gem_string("tag")};
        _t1746 = gem_has_key_fn(NULL, _t1745, 2);
    }
    GemVal _t1747;
    if (!gem_truthy(_t1746)) {
        _t1747 = _t1746;
    } else {
        _t1747 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1747)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1748[] = {gem_v__match_11};
    GemVal _t1750;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1748, 1), gem_string("table")))) {
        _t1750 = gem_eq(gem_type_fn(NULL, _t1748, 1), gem_string("table"));
    } else {
        GemVal _t1749[] = {gem_v__match_11, gem_string("tag")};
        _t1750 = gem_has_key_fn(NULL, _t1749, 2);
    }
    GemVal _t1751;
    if (!gem_truthy(_t1750)) {
        _t1751 = _t1750;
    } else {
        _t1751 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1753;
    if (!gem_truthy(_t1751)) {
        _t1753 = _t1751;
    } else {
        GemVal _t1752[] = {gem_v__match_11, gem_string("cond")};
        _t1753 = gem_has_key_fn(NULL, _t1752, 2);
    }
    GemVal _t1755;
    if (!gem_truthy(_t1753)) {
        _t1755 = _t1753;
    } else {
        GemVal _t1754[] = {gem_v__match_11, gem_string("then")};
        _t1755 = gem_has_key_fn(NULL, _t1754, 2);
    }
    GemVal _t1757;
    if (!gem_truthy(_t1755)) {
        _t1757 = _t1755;
    } else {
        GemVal _t1756[] = {gem_v__match_11, gem_string("else")};
        _t1757 = gem_has_key_fn(NULL, _t1756, 2);
    }
    if (gem_truthy(_t1757)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 286 "compiler/main.gem"
    GemVal _t1758[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1759 = (*gem_v_collect_free_node);
        (void)(_t1759.fn(_t1759.env, _t1758, 3));
#line 287 "compiler/main.gem"
    GemVal _t1760 = gem_table_new();
    GemVal _t1761[] = {gem_v_defined, _t1760};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1761, 2);
#line 288 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 288 "compiler/main.gem"
    GemVal _t1762[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1762, 1);
#line 288 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 288 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 288 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 289 "compiler/main.gem"
    GemVal _t1763[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1764 = (*gem_v_collect_free_node);
            (void)(_t1764.fn(_t1764.env, _t1763, 3));
#line 290 "compiler/main.gem"
    GemVal _t1765[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t1765, 2))) {
#line 291 "compiler/main.gem"
    GemVal _t1766 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1767[] = {gem_v_d, gem_table_get(_t1766, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1767, 2));
            }
        }

#line 294 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 295 "compiler/main.gem"
    GemVal _t1768 = gem_table_new();
    GemVal _t1769[] = {gem_v_defined, _t1768};
            gem_v_d = gem_fn_set_union(NULL, _t1769, 2);
            {
#line 296 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 296 "compiler/main.gem"
    GemVal _t1770[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1770, 1);
#line 296 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 296 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 296 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 297 "compiler/main.gem"
    GemVal _t1771[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1772 = (*gem_v_collect_free_node);
                    (void)(_t1772.fn(_t1772.env, _t1771, 3));
#line 298 "compiler/main.gem"
    GemVal _t1773[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1773, 2))) {
#line 299 "compiler/main.gem"
    GemVal _t1774 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1775[] = {gem_v_d, gem_table_get(_t1774, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1775, 2));
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
    GemVal _t1776[] = {gem_v__match_11};
    GemVal _t1778;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1776, 1), gem_string("table")))) {
        _t1778 = gem_eq(gem_type_fn(NULL, _t1776, 1), gem_string("table"));
    } else {
        GemVal _t1777[] = {gem_v__match_11, gem_string("tag")};
        _t1778 = gem_has_key_fn(NULL, _t1777, 2);
    }
    GemVal _t1779;
    if (!gem_truthy(_t1778)) {
        _t1779 = _t1778;
    } else {
        _t1779 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1781;
    if (!gem_truthy(_t1779)) {
        _t1781 = _t1779;
    } else {
        GemVal _t1780[] = {gem_v__match_11, gem_string("cond")};
        _t1781 = gem_has_key_fn(NULL, _t1780, 2);
    }
    GemVal _t1783;
    if (!gem_truthy(_t1781)) {
        _t1783 = _t1781;
    } else {
        GemVal _t1782[] = {gem_v__match_11, gem_string("body")};
        _t1783 = gem_has_key_fn(NULL, _t1782, 2);
    }
    if (gem_truthy(_t1783)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 304 "compiler/main.gem"
    GemVal _t1784[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1785 = (*gem_v_collect_free_node);
        (void)(_t1785.fn(_t1785.env, _t1784, 3));
#line 305 "compiler/main.gem"
    GemVal _t1786 = gem_table_new();
    GemVal _t1787[] = {gem_v_defined, _t1786};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1787, 2);
        {
#line 306 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 306 "compiler/main.gem"
    GemVal _t1788[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1788, 1);
#line 306 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 306 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 306 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 307 "compiler/main.gem"
    GemVal _t1789[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1790 = (*gem_v_collect_free_node);
                (void)(_t1790.fn(_t1790.env, _t1789, 3));
#line 308 "compiler/main.gem"
    GemVal _t1791[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1791, 2))) {
#line 309 "compiler/main.gem"
    GemVal _t1792 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1793[] = {gem_v_d, gem_table_get(_t1792, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1793, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1794[] = {gem_v__match_11};
    GemVal _t1796;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1794, 1), gem_string("table")))) {
        _t1796 = gem_eq(gem_type_fn(NULL, _t1794, 1), gem_string("table"));
    } else {
        GemVal _t1795[] = {gem_v__match_11, gem_string("tag")};
        _t1796 = gem_has_key_fn(NULL, _t1795, 2);
    }
    GemVal _t1797;
    if (!gem_truthy(_t1796)) {
        _t1797 = _t1796;
    } else {
        _t1797 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1799;
    if (!gem_truthy(_t1797)) {
        _t1799 = _t1797;
    } else {
        GemVal _t1798[] = {gem_v__match_11, gem_string("target")};
        _t1799 = gem_has_key_fn(NULL, _t1798, 2);
    }
    GemVal _t1801;
    if (!gem_truthy(_t1799)) {
        _t1801 = _t1799;
    } else {
        GemVal _t1800[] = {gem_v__match_11, gem_string("target_var")};
        _t1801 = gem_has_key_fn(NULL, _t1800, 2);
    }
    GemVal _t1803;
    if (!gem_truthy(_t1801)) {
        _t1803 = _t1801;
    } else {
        GemVal _t1802[] = {gem_v__match_11, gem_string("whens")};
        _t1803 = gem_has_key_fn(NULL, _t1802, 2);
    }
    GemVal _t1805;
    if (!gem_truthy(_t1803)) {
        _t1805 = _t1803;
    } else {
        GemVal _t1804[] = {gem_v__match_11, gem_string("else")};
        _t1805 = gem_has_key_fn(NULL, _t1804, 2);
    }
    if (gem_truthy(_t1805)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 313 "compiler/main.gem"
    GemVal _t1806[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1807 = (*gem_v_collect_free_node);
        (void)(_t1807.fn(_t1807.env, _t1806, 3));
#line 314 "compiler/main.gem"
    GemVal _t1808 = gem_table_new();
    GemVal _t1809[] = {gem_v_defined, _t1808};
        GemVal gem_v_match_def = gem_fn_set_union(NULL, _t1809, 2);
#line 315 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 316 "compiler/main.gem"
    GemVal _t1810[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn_set_add(NULL, _t1810, 2));
        }
#line 318 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 318 "compiler/main.gem"
    GemVal _t1811[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1811, 1);
#line 318 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 318 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 318 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 319 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 320 "compiler/main.gem"
    GemVal _t1812 = gem_v_w;
    GemVal _t1813[] = {gem_table_get(_t1812, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1814 = (*gem_v_collect_free_node);
            (void)(_t1814.fn(_t1814.env, _t1813, 3));
#line 321 "compiler/main.gem"
    GemVal _t1815 = gem_table_new();
    GemVal _t1816[] = {gem_v_match_def, _t1815};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1816, 2);
#line 322 "compiler/main.gem"
    GemVal _t1817 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1817, gem_string("bindings")), GEM_NIL))) {
#line 323 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 323 "compiler/main.gem"
    GemVal _t1818 = gem_v_w;
    GemVal _t1819[] = {gem_table_get(_t1818, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1819, 1);
#line 323 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 323 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 323 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 324 "compiler/main.gem"
    GemVal _t1820 = gem_v_w;
    GemVal _t1821[] = {gem_table_get(gem_table_get(_t1820, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1822 = (*gem_v_collect_free_node);
                    (void)(_t1822.fn(_t1822.env, _t1821, 3));
#line 325 "compiler/main.gem"
    GemVal _t1823 = gem_v_w;
    GemVal _t1824[] = {gem_table_get(gem_table_get(_t1823, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1824, 2))) {
#line 326 "compiler/main.gem"
    GemVal _t1825 = gem_v_w;
    GemVal _t1826 = gem_table_get(gem_table_get(_t1825, gem_string("bindings")), gem_v_bi);
    GemVal _t1827[] = {gem_v_d, gem_table_get(_t1826, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1827, 2));
                    }
                }

            }
#line 330 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 330 "compiler/main.gem"
    GemVal _t1828 = gem_v_w;
    GemVal _t1829[] = {gem_table_get(_t1828, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1829, 1);
#line 330 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 330 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 330 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 331 "compiler/main.gem"
    GemVal _t1830 = gem_v_w;
    GemVal _t1831[] = {gem_table_get(gem_table_get(_t1830, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1832 = (*gem_v_collect_free_node);
                (void)(_t1832.fn(_t1832.env, _t1831, 3));
#line 332 "compiler/main.gem"
    GemVal _t1833 = gem_v_w;
    GemVal _t1834[] = {gem_table_get(gem_table_get(_t1833, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1834, 2))) {
#line 333 "compiler/main.gem"
    GemVal _t1835 = gem_v_w;
    GemVal _t1836 = gem_table_get(gem_table_get(_t1835, gem_string("body")), gem_v_j);
    GemVal _t1837[] = {gem_v_d, gem_table_get(_t1836, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1837, 2));
                }
            }

        }

#line 337 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 338 "compiler/main.gem"
    GemVal _t1838 = gem_table_new();
    GemVal _t1839[] = {gem_v_defined, _t1838};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1839, 2);
            {
#line 339 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 339 "compiler/main.gem"
    GemVal _t1840[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1840, 1);
#line 339 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 339 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 339 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 340 "compiler/main.gem"
    GemVal _t1841[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1842 = (*gem_v_collect_free_node);
                    (void)(_t1842.fn(_t1842.env, _t1841, 3));
#line 341 "compiler/main.gem"
    GemVal _t1843[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1843, 2))) {
#line 342 "compiler/main.gem"
    GemVal _t1844 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1845[] = {gem_v_d, gem_table_get(_t1844, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1845, 2));
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
    GemVal _t1846[] = {gem_v__match_11};
    GemVal _t1848;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1846, 1), gem_string("table")))) {
        _t1848 = gem_eq(gem_type_fn(NULL, _t1846, 1), gem_string("table"));
    } else {
        GemVal _t1847[] = {gem_v__match_11, gem_string("tag")};
        _t1848 = gem_has_key_fn(NULL, _t1847, 2);
    }
    GemVal _t1849;
    if (!gem_truthy(_t1848)) {
        _t1849 = _t1848;
    } else {
        _t1849 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t1851;
    if (!gem_truthy(_t1849)) {
        _t1851 = _t1849;
    } else {
        GemVal _t1850[] = {gem_v__match_11, gem_string("value")};
        _t1851 = gem_has_key_fn(NULL, _t1850, 2);
    }
    GemVal _t1853;
    if (!gem_truthy(_t1851)) {
        _t1853 = _t1851;
    } else {
        GemVal _t1852[] = {gem_v__match_11, gem_string("bindings")};
        _t1853 = gem_has_key_fn(NULL, _t1852, 2);
    }
    GemVal _t1855;
    if (!gem_truthy(_t1853)) {
        _t1855 = _t1853;
    } else {
        GemVal _t1854[] = {gem_v__match_11, gem_string("body")};
        _t1855 = gem_has_key_fn(NULL, _t1854, 2);
    }
    if (gem_truthy(_t1855)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 347 "compiler/main.gem"
    GemVal _t1856[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1857 = (*gem_v_collect_free_node);
        (void)(_t1857.fn(_t1857.env, _t1856, 3));
#line 348 "compiler/main.gem"
    GemVal _t1858 = gem_table_new();
    GemVal _t1859[] = {gem_v_defined, _t1858};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1859, 2);
#line 349 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 350 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 350 "compiler/main.gem"
    GemVal _t1860[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1860, 1);
#line 350 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 350 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 350 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 351 "compiler/main.gem"
    GemVal _t1861[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1862 = (*gem_v_collect_free_node);
                (void)(_t1862.fn(_t1862.env, _t1861, 3));
#line 352 "compiler/main.gem"
    GemVal _t1863[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1863, 2))) {
#line 353 "compiler/main.gem"
    GemVal _t1864 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t1865[] = {gem_v_d, gem_table_get(_t1864, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1865, 2));
                }
            }

        }
        {
#line 357 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 357 "compiler/main.gem"
    GemVal _t1866[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1866, 1);
#line 357 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 357 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 357 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 358 "compiler/main.gem"
    GemVal _t1867[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1868 = (*gem_v_collect_free_node);
                (void)(_t1868.fn(_t1868.env, _t1867, 3));
#line 359 "compiler/main.gem"
    GemVal _t1869[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1869, 2))) {
#line 360 "compiler/main.gem"
    GemVal _t1870 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1871[] = {gem_v_d, gem_table_get(_t1870, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1871, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1872[] = {gem_v__match_11};
    GemVal _t1874;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1872, 1), gem_string("table")))) {
        _t1874 = gem_eq(gem_type_fn(NULL, _t1872, 1), gem_string("table"));
    } else {
        GemVal _t1873[] = {gem_v__match_11, gem_string("tag")};
        _t1874 = gem_has_key_fn(NULL, _t1873, 2);
    }
    GemVal _t1875;
    if (!gem_truthy(_t1874)) {
        _t1875 = _t1874;
    } else {
        _t1875 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t1877;
    if (!gem_truthy(_t1875)) {
        _t1877 = _t1875;
    } else {
        GemVal _t1876[] = {gem_v__match_11, gem_string("arms")};
        _t1877 = gem_has_key_fn(NULL, _t1876, 2);
    }
    GemVal _t1879;
    if (!gem_truthy(_t1877)) {
        _t1879 = _t1877;
    } else {
        GemVal _t1878[] = {gem_v__match_11, gem_string("after_ms")};
        _t1879 = gem_has_key_fn(NULL, _t1878, 2);
    }
    GemVal _t1881;
    if (!gem_truthy(_t1879)) {
        _t1881 = _t1879;
    } else {
        GemVal _t1880[] = {gem_v__match_11, gem_string("after_body")};
        _t1881 = gem_has_key_fn(NULL, _t1880, 2);
    }
    if (gem_truthy(_t1881)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 364 "compiler/main.gem"
    GemVal _t1882 = gem_table_new();
    GemVal _t1883[] = {gem_v_defined, _t1882};
        GemVal gem_v_recv_def = gem_fn_set_union(NULL, _t1883, 2);
#line 365 "compiler/main.gem"
    GemVal _t1884 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1884, gem_string("recv_var")), GEM_NIL))) {
#line 366 "compiler/main.gem"
    GemVal _t1885 = gem_v_node;
    GemVal _t1886[] = {gem_v_recv_def, gem_table_get(_t1885, gem_string("recv_var"))};
            (void)(gem_fn_set_add(NULL, _t1886, 2));
        }
#line 368 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 368 "compiler/main.gem"
    GemVal _t1887[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1887, 1);
#line 368 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 368 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 368 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 369 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 370 "compiler/main.gem"
    GemVal _t1888 = gem_v_arm;
    GemVal _t1889 = gem_table_get(_t1888, gem_string("pattern"));
    GemVal _t1890[] = {gem_table_get(_t1889, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t1891 = (*gem_v_collect_free_node);
            (void)(_t1891.fn(_t1891.env, _t1890, 3));
#line 371 "compiler/main.gem"
    GemVal _t1892 = gem_table_new();
    GemVal _t1893[] = {gem_v_recv_def, _t1892};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1893, 2);
#line 372 "compiler/main.gem"
    GemVal _t1894 = gem_v_arm;
    GemVal _t1895 = gem_table_get(_t1894, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t1895, gem_string("bindings")), GEM_NIL))) {
#line 373 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 373 "compiler/main.gem"
    GemVal _t1896 = gem_v_arm;
    GemVal _t1897 = gem_table_get(_t1896, gem_string("pattern"));
    GemVal _t1898[] = {gem_table_get(_t1897, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1898, 1);
#line 373 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 373 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 373 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 374 "compiler/main.gem"
    GemVal _t1899 = gem_v_arm;
    GemVal _t1900 = gem_table_get(_t1899, gem_string("pattern"));
    GemVal _t1901[] = {gem_table_get(gem_table_get(_t1900, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1902 = (*gem_v_collect_free_node);
                    (void)(_t1902.fn(_t1902.env, _t1901, 3));
#line 375 "compiler/main.gem"
    GemVal _t1903 = gem_v_arm;
    GemVal _t1904 = gem_table_get(_t1903, gem_string("pattern"));
    GemVal _t1905[] = {gem_table_get(gem_table_get(_t1904, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1905, 2))) {
#line 376 "compiler/main.gem"
    GemVal _t1906 = gem_v_arm;
    GemVal _t1907 = gem_table_get(_t1906, gem_string("pattern"));
    GemVal _t1908 = gem_table_get(gem_table_get(_t1907, gem_string("bindings")), gem_v_bi);
    GemVal _t1909[] = {gem_v_d, gem_table_get(_t1908, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1909, 2));
                    }
                }

            }
#line 380 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 380 "compiler/main.gem"
    GemVal _t1910 = gem_v_arm;
    GemVal _t1911[] = {gem_table_get(_t1910, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1911, 1);
#line 380 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 380 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 380 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 381 "compiler/main.gem"
    GemVal _t1912 = gem_v_arm;
    GemVal _t1913[] = {gem_table_get(gem_table_get(_t1912, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1914 = (*gem_v_collect_free_node);
                (void)(_t1914.fn(_t1914.env, _t1913, 3));
#line 382 "compiler/main.gem"
    GemVal _t1915 = gem_v_arm;
    GemVal _t1916[] = {gem_table_get(gem_table_get(_t1915, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1916, 2))) {
#line 383 "compiler/main.gem"
    GemVal _t1917 = gem_v_arm;
    GemVal _t1918 = gem_table_get(gem_table_get(_t1917, gem_string("body")), gem_v_j);
    GemVal _t1919[] = {gem_v_d, gem_table_get(_t1918, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1919, 2));
                }
            }

        }

#line 387 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 388 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t1921 = (*gem_v_collect_free_node);
            (void)(_t1921.fn(_t1921.env, _t1920, 3));
        }
#line 390 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 391 "compiler/main.gem"
    GemVal _t1922 = gem_table_new();
    GemVal _t1923[] = {gem_v_recv_def, _t1922};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1923, 2);
            {
#line 392 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 392 "compiler/main.gem"
    GemVal _t1924[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t1924, 1);
#line 392 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 392 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 392 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 393 "compiler/main.gem"
    GemVal _t1925[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1926 = (*gem_v_collect_free_node);
                    (void)(_t1926.fn(_t1926.env, _t1925, 3));
#line 394 "compiler/main.gem"
    GemVal _t1927[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1927, 2))) {
#line 395 "compiler/main.gem"
    GemVal _t1928 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t1929[] = {gem_v_d, gem_table_get(_t1928, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1929, 2));
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
    GemVal _t1930[] = {gem_v__match_11};
    GemVal _t1932;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1930, 1), gem_string("table")))) {
        _t1932 = gem_eq(gem_type_fn(NULL, _t1930, 1), gem_string("table"));
    } else {
        GemVal _t1931[] = {gem_v__match_11, gem_string("tag")};
        _t1932 = gem_has_key_fn(NULL, _t1931, 2);
    }
    GemVal _t1933;
    if (!gem_truthy(_t1932)) {
        _t1933 = _t1932;
    } else {
        _t1933 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t1935;
    if (!gem_truthy(_t1933)) {
        _t1935 = _t1933;
    } else {
        GemVal _t1934[] = {gem_v__match_11, gem_string("entries")};
        _t1935 = gem_has_key_fn(NULL, _t1934, 2);
    }
    if (gem_truthy(_t1935)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 400 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 400 "compiler/main.gem"
    GemVal _t1936[] = {gem_v_entries};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1936, 1);
#line 400 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 400 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 400 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 401 "compiler/main.gem"
    GemVal _t1937 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t1938[] = {gem_table_get(_t1937, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1939 = (*gem_v_collect_free_node);
                (void)(_t1939.fn(_t1939.env, _t1938, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1940[] = {gem_v__match_11};
    GemVal _t1942;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1940, 1), gem_string("table")))) {
        _t1942 = gem_eq(gem_type_fn(NULL, _t1940, 1), gem_string("table"));
    } else {
        GemVal _t1941[] = {gem_v__match_11, gem_string("tag")};
        _t1942 = gem_has_key_fn(NULL, _t1941, 2);
    }
    GemVal _t1943;
    if (!gem_truthy(_t1942)) {
        _t1943 = _t1942;
    } else {
        _t1943 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t1945;
    if (!gem_truthy(_t1943)) {
        _t1945 = _t1943;
    } else {
        GemVal _t1944[] = {gem_v__match_11, gem_string("elements")};
        _t1945 = gem_has_key_fn(NULL, _t1944, 2);
    }
    if (gem_truthy(_t1945)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 404 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 404 "compiler/main.gem"
    GemVal _t1946[] = {gem_v_elements};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t1946, 1);
#line 404 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 404 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 404 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 405 "compiler/main.gem"
    GemVal _t1947[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t1948 = (*gem_v_collect_free_node);
                (void)(_t1948.fn(_t1948.env, _t1947, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1949[] = {gem_v__match_11};
    GemVal _t1951;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1949, 1), gem_string("table")))) {
        _t1951 = gem_eq(gem_type_fn(NULL, _t1949, 1), gem_string("table"));
    } else {
        GemVal _t1950[] = {gem_v__match_11, gem_string("tag")};
        _t1951 = gem_has_key_fn(NULL, _t1950, 2);
    }
    GemVal _t1952;
    if (!gem_truthy(_t1951)) {
        _t1952 = _t1951;
    } else {
        _t1952 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1954;
    if (!gem_truthy(_t1952)) {
        _t1954 = _t1952;
    } else {
        GemVal _t1953[] = {gem_v__match_11, gem_string("stmts")};
        _t1954 = gem_has_key_fn(NULL, _t1953, 2);
    }
    if (gem_truthy(_t1954)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 408 "compiler/main.gem"
    GemVal _t1955 = gem_table_new();
    GemVal _t1956[] = {gem_v_defined, _t1955};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1956, 2);
        {
#line 409 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 409 "compiler/main.gem"
    GemVal _t1957[] = {gem_v_stmts};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1957, 1);
#line 409 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 409 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 409 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 410 "compiler/main.gem"
    GemVal _t1958[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1959 = (*gem_v_collect_free_node);
                (void)(_t1959.fn(_t1959.env, _t1958, 3));
#line 411 "compiler/main.gem"
    GemVal _t1960[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1960, 2))) {
#line 412 "compiler/main.gem"
    GemVal _t1961 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1962[] = {gem_v_d, gem_table_get(_t1961, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1962, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1963[] = {gem_v__match_11};
    GemVal _t1965;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1963, 1), gem_string("table")))) {
        _t1965 = gem_eq(gem_type_fn(NULL, _t1963, 1), gem_string("table"));
    } else {
        GemVal _t1964[] = {gem_v__match_11, gem_string("tag")};
        _t1965 = gem_has_key_fn(NULL, _t1964, 2);
    }
    GemVal _t1966;
    if (!gem_truthy(_t1965)) {
        _t1966 = _t1965;
    } else {
        _t1966 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1968;
    if (!gem_truthy(_t1966)) {
        _t1968 = _t1966;
    } else {
        GemVal _t1967[] = {gem_v__match_11, gem_string("value")};
        _t1968 = gem_has_key_fn(NULL, _t1967, 2);
    }
    if (gem_truthy(_t1968)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 416 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 417 "compiler/main.gem"
    GemVal _t1969[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1970 = (*gem_v_collect_free_node);
            GemVal _t1971 = _t1970.fn(_t1970.env, _t1969, 3);
            gem_pop_frame();
            return _t1971;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 421 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 421 "compiler/main.gem"
    GemVal _t1972[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_keys_29 = gem_keys_fn(NULL, _t1972, 1);
#line 421 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 421 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1973[] = {gem_v__for_keys_29};
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_len_fn(NULL, _t1973, 1)))) break;
#line 421 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_29, gem_v__for_i_29);
#line 421 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_29, gem_table_get(gem_v__for_keys_29, gem_v__for_i_29));
#line 421 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 422 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 423 "compiler/main.gem"
    GemVal _t1974[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1974, 1), gem_string("table")))) {
#line 424 "compiler/main.gem"
    GemVal _t1975[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1975, 1);
#line 425 "compiler/main.gem"
    GemVal _t1976[] = {gem_v_vks};
    GemVal _t1978;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1976, 1), gem_int(0)))) {
        _t1978 = gem_gt(gem_len_fn(NULL, _t1976, 1), gem_int(0));
    } else {
        GemVal _t1977[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1978 = gem_eq(gem_type_fn(NULL, _t1977, 1), gem_string("int"));
    }
                        if (gem_truthy(_t1978)) {
#line 426 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 427 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t1979[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1979, 1)))) break;
#line 428 "compiler/main.gem"
    GemVal _t1980[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1980, 1), gem_string("table")))) {
#line 429 "compiler/main.gem"
    GemVal _t1981[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1982 = (*gem_v_collect_free_node);
                                    (void)(_t1982.fn(_t1982.env, _t1981, 3));
                                }
#line 431 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 434 "compiler/main.gem"
    GemVal _t1983[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1984 = (*gem_v_collect_free_node);
                            (void)(_t1984.fn(_t1984.env, _t1983, 3));
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
#line 443 "compiler/main.gem"
    GemVal _t1986 = gem_table_new();
    GemVal gem_v_free = _t1986;
#line 444 "compiler/main.gem"
    GemVal _t1987 = gem_table_new();
    GemVal _t1988[] = {gem_v_defined, _t1987};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1988, 2);
#line 445 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 445 "compiler/main.gem"
    GemVal _t1989[] = {gem_v_stmts};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1989, 1);
#line 445 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 445 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 445 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 446 "compiler/main.gem"
    GemVal _t1990[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1991 = (*gem_v_collect_free_node);
        (void)(_t1991.fn(_t1991.env, _t1990, 3));
#line 447 "compiler/main.gem"
    GemVal _t1992[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn_is_node(NULL, _t1992, 2))) {
#line 448 "compiler/main.gem"
    GemVal _t1993 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1994[] = {gem_v_d, gem_table_get(_t1993, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1994, 2));
        }
#line 450 "compiler/main.gem"
    GemVal _t1995[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn_collect_block_lets(NULL, _t1995, 2));
    }

    GemVal _t1996 = gem_v_free;
    gem_pop_frame();
    return _t1996;
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
#line 461 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 462 "compiler/main.gem"
        GemVal _t1998 = GEM_NIL;
        gem_pop_frame();
        return _t1998;
    }
#line 464 "compiler/main.gem"
    GemVal _t1999[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1999, 1), gem_string("table")))) {
#line 465 "compiler/main.gem"
        GemVal _t2000 = GEM_NIL;
        gem_pop_frame();
        return _t2000;
    }
#line 467 "compiler/main.gem"
    GemVal _t2001 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t2001, gem_string("tag")), GEM_NIL))) {
#line 468 "compiler/main.gem"
        GemVal _t2002 = GEM_NIL;
        gem_pop_frame();
        return _t2002;
    }
#line 471 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
    GemVal _t2003[] = {gem_v__match_31};
    GemVal _t2005;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2003, 1), gem_string("table")))) {
        _t2005 = gem_eq(gem_type_fn(NULL, _t2003, 1), gem_string("table"));
    } else {
        GemVal _t2004[] = {gem_v__match_31, gem_string("tag")};
        _t2005 = gem_has_key_fn(NULL, _t2004, 2);
    }
    GemVal _t2006;
    if (!gem_truthy(_t2005)) {
        _t2006 = _t2005;
    } else {
        _t2006 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t2008;
    if (!gem_truthy(_t2006)) {
        _t2008 = _t2006;
    } else {
        GemVal _t2007[] = {gem_v__match_31, gem_string("params")};
        _t2008 = gem_has_key_fn(NULL, _t2007, 2);
    }
    GemVal _t2010;
    if (!gem_truthy(_t2008)) {
        _t2010 = _t2008;
    } else {
        GemVal _t2009[] = {gem_v__match_31, gem_string("rest_param")};
        _t2010 = gem_has_key_fn(NULL, _t2009, 2);
    }
    GemVal _t2012;
    if (!gem_truthy(_t2010)) {
        _t2012 = _t2010;
    } else {
        GemVal _t2011[] = {gem_v__match_31, gem_string("block_param")};
        _t2012 = gem_has_key_fn(NULL, _t2011, 2);
    }
    GemVal _t2014;
    if (!gem_truthy(_t2012)) {
        _t2014 = _t2012;
    } else {
        GemVal _t2013[] = {gem_v__match_31, gem_string("body")};
        _t2014 = gem_has_key_fn(NULL, _t2013, 2);
    }
    if (gem_truthy(_t2014)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_31, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_31, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_31, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 473 "compiler/main.gem"
    GemVal _t2015[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t2015, 1);
#line 474 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 475 "compiler/main.gem"
    GemVal _t2016[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t2016, 2));
        }
#line 477 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 478 "compiler/main.gem"
    GemVal _t2017[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t2017, 2));
        }
#line 485 "compiler/main.gem"
    GemVal _t2018[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t2019 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t2019.fn(_t2019.env, _t2018, 2);
#line 486 "compiler/main.gem"
    GemVal _t2020[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t2020, 2);
        {
#line 487 "compiler/main.gem"
            GemVal gem_v__for_tbl_32 = gem_v_inter;
#line 487 "compiler/main.gem"
    GemVal _t2021[] = {gem_v__for_tbl_32};
            GemVal gem_v__for_keys_32 = gem_keys_fn(NULL, _t2021, 1);
#line 487 "compiler/main.gem"
            GemVal gem_v__for_i_32 = gem_int(0);
#line 487 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2022[] = {gem_v__for_keys_32};
                if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t2022, 1)))) break;
#line 487 "compiler/main.gem"
                GemVal gem_v_ik = gem_table_get(gem_v__for_keys_32, gem_v__for_i_32);
#line 487 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_32, gem_table_get(gem_v__for_keys_32, gem_v__for_i_32));
#line 487 "compiler/main.gem"
                gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 488 "compiler/main.gem"
    GemVal _t2023[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn_set_add(NULL, _t2023, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2024[] = {gem_v__match_31};
    GemVal _t2026;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2024, 1), gem_string("table")))) {
        _t2026 = gem_eq(gem_type_fn(NULL, _t2024, 1), gem_string("table"));
    } else {
        GemVal _t2025[] = {gem_v__match_31, gem_string("tag")};
        _t2026 = gem_has_key_fn(NULL, _t2025, 2);
    }
    GemVal _t2027;
    if (!gem_truthy(_t2026)) {
        _t2027 = _t2026;
    } else {
        _t2027 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2027)) {
        gem_pop_frame();
        return GEM_NIL;
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
        _t2031 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2033;
    if (!gem_truthy(_t2031)) {
        _t2033 = _t2031;
    } else {
        GemVal _t2032[] = {gem_v__match_31, gem_string("cond")};
        _t2033 = gem_has_key_fn(NULL, _t2032, 2);
    }
    GemVal _t2035;
    if (!gem_truthy(_t2033)) {
        _t2035 = _t2033;
    } else {
        GemVal _t2034[] = {gem_v__match_31, gem_string("then")};
        _t2035 = gem_has_key_fn(NULL, _t2034, 2);
    }
    GemVal _t2037;
    if (!gem_truthy(_t2035)) {
        _t2037 = _t2035;
    } else {
        GemVal _t2036[] = {gem_v__match_31, gem_string("else")};
        _t2037 = gem_has_key_fn(NULL, _t2036, 2);
    }
    if (gem_truthy(_t2037)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_31, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 493 "compiler/main.gem"
    GemVal _t2038[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2039 = (*gem_v_walk_captures_node);
        (void)(_t2039.fn(_t2039.env, _t2038, 3));
#line 494 "compiler/main.gem"
    GemVal _t2040[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t2041 = (*gem_v_walk_captures);
        (void)(_t2041.fn(_t2041.env, _t2040, 3));
#line 495 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 496 "compiler/main.gem"
    GemVal _t2042[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2043 = (*gem_v_walk_captures);
            GemVal _t2044 = _t2043.fn(_t2043.env, _t2042, 3);
            gem_pop_frame();
            return _t2044;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2045[] = {gem_v__match_31};
    GemVal _t2047;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2045, 1), gem_string("table")))) {
        _t2047 = gem_eq(gem_type_fn(NULL, _t2045, 1), gem_string("table"));
    } else {
        GemVal _t2046[] = {gem_v__match_31, gem_string("tag")};
        _t2047 = gem_has_key_fn(NULL, _t2046, 2);
    }
    GemVal _t2048;
    if (!gem_truthy(_t2047)) {
        _t2048 = _t2047;
    } else {
        _t2048 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("while"));
    }
    GemVal _t2050;
    if (!gem_truthy(_t2048)) {
        _t2050 = _t2048;
    } else {
        GemVal _t2049[] = {gem_v__match_31, gem_string("cond")};
        _t2050 = gem_has_key_fn(NULL, _t2049, 2);
    }
    GemVal _t2052;
    if (!gem_truthy(_t2050)) {
        _t2052 = _t2050;
    } else {
        GemVal _t2051[] = {gem_v__match_31, gem_string("body")};
        _t2052 = gem_has_key_fn(NULL, _t2051, 2);
    }
    if (gem_truthy(_t2052)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 499 "compiler/main.gem"
    GemVal _t2053[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2054 = (*gem_v_walk_captures_node);
        (void)(_t2054.fn(_t2054.env, _t2053, 3));
#line 500 "compiler/main.gem"
    GemVal _t2055[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2056 = (*gem_v_walk_captures);
        GemVal _t2057 = _t2056.fn(_t2056.env, _t2055, 3);
        gem_pop_frame();
        return _t2057;
    } else {
    GemVal _t2058[] = {gem_v__match_31};
    GemVal _t2060;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2058, 1), gem_string("table")))) {
        _t2060 = gem_eq(gem_type_fn(NULL, _t2058, 1), gem_string("table"));
    } else {
        GemVal _t2059[] = {gem_v__match_31, gem_string("tag")};
        _t2060 = gem_has_key_fn(NULL, _t2059, 2);
    }
    GemVal _t2061;
    if (!gem_truthy(_t2060)) {
        _t2061 = _t2060;
    } else {
        _t2061 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2063;
    if (!gem_truthy(_t2061)) {
        _t2063 = _t2061;
    } else {
        GemVal _t2062[] = {gem_v__match_31, gem_string("target")};
        _t2063 = gem_has_key_fn(NULL, _t2062, 2);
    }
    GemVal _t2065;
    if (!gem_truthy(_t2063)) {
        _t2065 = _t2063;
    } else {
        GemVal _t2064[] = {gem_v__match_31, gem_string("whens")};
        _t2065 = gem_has_key_fn(NULL, _t2064, 2);
    }
    GemVal _t2067;
    if (!gem_truthy(_t2065)) {
        _t2067 = _t2065;
    } else {
        GemVal _t2066[] = {gem_v__match_31, gem_string("else")};
        _t2067 = gem_has_key_fn(NULL, _t2066, 2);
    }
    if (gem_truthy(_t2067)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_31, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_31, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 502 "compiler/main.gem"
    GemVal _t2068[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2069 = (*gem_v_walk_captures_node);
        (void)(_t2069.fn(_t2069.env, _t2068, 3));
#line 503 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 503 "compiler/main.gem"
    GemVal _t2070[] = {gem_v_whens};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t2070, 1);
#line 503 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 503 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 503 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 504 "compiler/main.gem"
    GemVal _t2071 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2072[] = {gem_table_get(_t2071, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2073 = (*gem_v_walk_captures_node);
            (void)(_t2073.fn(_t2073.env, _t2072, 3));
#line 505 "compiler/main.gem"
    GemVal _t2074 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t2074, gem_string("bindings")), GEM_NIL))) {
#line 506 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 506 "compiler/main.gem"
    GemVal _t2075 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2076[] = {gem_table_get(_t2075, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t2076, 1);
#line 506 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 506 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 506 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 507 "compiler/main.gem"
    GemVal _t2077 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2078[] = {gem_table_get(gem_table_get(_t2077, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2079 = (*gem_v_walk_captures_node);
                    (void)(_t2079.fn(_t2079.env, _t2078, 3));
                }

            }
#line 510 "compiler/main.gem"
    GemVal _t2080 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2081[] = {gem_table_get(_t2080, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2082 = (*gem_v_walk_captures);
            (void)(_t2082.fn(_t2082.env, _t2081, 3));
        }

#line 512 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 513 "compiler/main.gem"
    GemVal _t2083[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2084 = (*gem_v_walk_captures);
            GemVal _t2085 = _t2084.fn(_t2084.env, _t2083, 3);
            gem_pop_frame();
            return _t2085;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2086[] = {gem_v__match_31};
    GemVal _t2088;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2086, 1), gem_string("table")))) {
        _t2088 = gem_eq(gem_type_fn(NULL, _t2086, 1), gem_string("table"));
    } else {
        GemVal _t2087[] = {gem_v__match_31, gem_string("tag")};
        _t2088 = gem_has_key_fn(NULL, _t2087, 2);
    }
    GemVal _t2089;
    if (!gem_truthy(_t2088)) {
        _t2089 = _t2088;
    } else {
        _t2089 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2091;
    if (!gem_truthy(_t2089)) {
        _t2091 = _t2089;
    } else {
        GemVal _t2090[] = {gem_v__match_31, gem_string("arms")};
        _t2091 = gem_has_key_fn(NULL, _t2090, 2);
    }
    GemVal _t2093;
    if (!gem_truthy(_t2091)) {
        _t2093 = _t2091;
    } else {
        GemVal _t2092[] = {gem_v__match_31, gem_string("after_ms")};
        _t2093 = gem_has_key_fn(NULL, _t2092, 2);
    }
    GemVal _t2095;
    if (!gem_truthy(_t2093)) {
        _t2095 = _t2093;
    } else {
        GemVal _t2094[] = {gem_v__match_31, gem_string("after_body")};
        _t2095 = gem_has_key_fn(NULL, _t2094, 2);
    }
    if (gem_truthy(_t2095)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_31, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_31, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_31, gem_string("after_body"));
#line 516 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 516 "compiler/main.gem"
    GemVal _t2096[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2096, 1);
#line 516 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 516 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 516 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 517 "compiler/main.gem"
    GemVal _t2097 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2098 = gem_table_get(_t2097, gem_string("pattern"));
    GemVal _t2099[] = {gem_table_get(_t2098, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2100 = (*gem_v_walk_captures_node);
            (void)(_t2100.fn(_t2100.env, _t2099, 3));
#line 518 "compiler/main.gem"
    GemVal _t2101 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2102 = gem_table_get(_t2101, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2102, gem_string("bindings")), GEM_NIL))) {
#line 519 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 519 "compiler/main.gem"
    GemVal _t2103 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2104 = gem_table_get(_t2103, gem_string("pattern"));
    GemVal _t2105[] = {gem_table_get(_t2104, gem_string("bindings"))};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2105, 1);
#line 519 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 519 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 519 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 520 "compiler/main.gem"
    GemVal _t2106 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2107 = gem_table_get(_t2106, gem_string("pattern"));
    GemVal _t2108[] = {gem_table_get(gem_table_get(_t2107, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2109 = (*gem_v_walk_captures_node);
                    (void)(_t2109.fn(_t2109.env, _t2108, 3));
                }

            }
#line 523 "compiler/main.gem"
    GemVal _t2110 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2111[] = {gem_table_get(_t2110, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2112 = (*gem_v_walk_captures);
            (void)(_t2112.fn(_t2112.env, _t2111, 3));
        }

#line 525 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 526 "compiler/main.gem"
    GemVal _t2113[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2114 = (*gem_v_walk_captures_node);
            (void)(_t2114.fn(_t2114.env, _t2113, 3));
        }
#line 528 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 529 "compiler/main.gem"
    GemVal _t2115[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2116 = (*gem_v_walk_captures);
            GemVal _t2117 = _t2116.fn(_t2116.env, _t2115, 3);
            gem_pop_frame();
            return _t2117;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2118[] = {gem_v__match_31};
    GemVal _t2120;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2118, 1), gem_string("table")))) {
        _t2120 = gem_eq(gem_type_fn(NULL, _t2118, 1), gem_string("table"));
    } else {
        GemVal _t2119[] = {gem_v__match_31, gem_string("tag")};
        _t2120 = gem_has_key_fn(NULL, _t2119, 2);
    }
    GemVal _t2121;
    if (!gem_truthy(_t2120)) {
        _t2121 = _t2120;
    } else {
        _t2121 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2123;
    if (!gem_truthy(_t2121)) {
        _t2123 = _t2121;
    } else {
        GemVal _t2122[] = {gem_v__match_31, gem_string("entries")};
        _t2123 = gem_has_key_fn(NULL, _t2122, 2);
    }
    if (gem_truthy(_t2123)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_31, gem_string("entries"));
        {
#line 532 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 532 "compiler/main.gem"
    GemVal _t2124[] = {gem_v_entries};
            GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2124, 1);
#line 532 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 532 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 532 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 533 "compiler/main.gem"
    GemVal _t2125 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2126[] = {gem_table_get(_t2125, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2127 = (*gem_v_walk_captures_node);
                (void)(_t2127.fn(_t2127.env, _t2126, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2128[] = {gem_v__match_31};
    GemVal _t2130;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2128, 1), gem_string("table")))) {
        _t2130 = gem_eq(gem_type_fn(NULL, _t2128, 1), gem_string("table"));
    } else {
        GemVal _t2129[] = {gem_v__match_31, gem_string("tag")};
        _t2130 = gem_has_key_fn(NULL, _t2129, 2);
    }
    GemVal _t2131;
    if (!gem_truthy(_t2130)) {
        _t2131 = _t2130;
    } else {
        _t2131 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2133;
    if (!gem_truthy(_t2131)) {
        _t2133 = _t2131;
    } else {
        GemVal _t2132[] = {gem_v__match_31, gem_string("elements")};
        _t2133 = gem_has_key_fn(NULL, _t2132, 2);
    }
    if (gem_truthy(_t2133)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_31, gem_string("elements"));
        {
#line 536 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 536 "compiler/main.gem"
    GemVal _t2134[] = {gem_v_elements};
            GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2134, 1);
#line 536 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 536 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 536 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 537 "compiler/main.gem"
    GemVal _t2135[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2136 = (*gem_v_walk_captures_node);
                (void)(_t2136.fn(_t2136.env, _t2135, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2137[] = {gem_v__match_31};
    GemVal _t2139;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2137, 1), gem_string("table")))) {
        _t2139 = gem_eq(gem_type_fn(NULL, _t2137, 1), gem_string("table"));
    } else {
        GemVal _t2138[] = {gem_v__match_31, gem_string("tag")};
        _t2139 = gem_has_key_fn(NULL, _t2138, 2);
    }
    GemVal _t2140;
    if (!gem_truthy(_t2139)) {
        _t2140 = _t2139;
    } else {
        _t2140 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2142;
    if (!gem_truthy(_t2140)) {
        _t2142 = _t2140;
    } else {
        GemVal _t2141[] = {gem_v__match_31, gem_string("stmts")};
        _t2142 = gem_has_key_fn(NULL, _t2141, 2);
    }
    if (gem_truthy(_t2142)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_31, gem_string("stmts"));
#line 540 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2144 = (*gem_v_walk_captures);
        GemVal _t2145 = _t2144.fn(_t2144.env, _t2143, 3);
        gem_pop_frame();
        return _t2145;
    } else {
    GemVal _t2146[] = {gem_v__match_31};
    GemVal _t2148;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2146, 1), gem_string("table")))) {
        _t2148 = gem_eq(gem_type_fn(NULL, _t2146, 1), gem_string("table"));
    } else {
        GemVal _t2147[] = {gem_v__match_31, gem_string("tag")};
        _t2148 = gem_has_key_fn(NULL, _t2147, 2);
    }
    GemVal _t2149;
    if (!gem_truthy(_t2148)) {
        _t2149 = _t2148;
    } else {
        _t2149 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2151;
    if (!gem_truthy(_t2149)) {
        _t2151 = _t2149;
    } else {
        GemVal _t2150[] = {gem_v__match_31, gem_string("value")};
        _t2151 = gem_has_key_fn(NULL, _t2150, 2);
    }
    if (gem_truthy(_t2151)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 542 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 543 "compiler/main.gem"
    GemVal _t2152[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2153 = (*gem_v_walk_captures_node);
            GemVal _t2154 = _t2153.fn(_t2153.env, _t2152, 3);
            gem_pop_frame();
            return _t2154;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2155[] = {gem_v__match_31};
    GemVal _t2157;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2155, 1), gem_string("table")))) {
        _t2157 = gem_eq(gem_type_fn(NULL, _t2155, 1), gem_string("table"));
    } else {
        GemVal _t2156[] = {gem_v__match_31, gem_string("tag")};
        _t2157 = gem_has_key_fn(NULL, _t2156, 2);
    }
    GemVal _t2158;
    if (!gem_truthy(_t2157)) {
        _t2158 = _t2157;
    } else {
        _t2158 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2160;
    if (!gem_truthy(_t2158)) {
        _t2160 = _t2158;
    } else {
        GemVal _t2159[] = {gem_v__match_31, gem_string("value")};
        _t2160 = gem_has_key_fn(NULL, _t2159, 2);
    }
    if (gem_truthy(_t2160)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 546 "compiler/main.gem"
    GemVal _t2161[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2162 = (*gem_v_walk_captures_node);
        GemVal _t2163 = _t2162.fn(_t2162.env, _t2161, 3);
        gem_pop_frame();
        return _t2163;
    } else {
    GemVal _t2164[] = {gem_v__match_31};
    GemVal _t2166;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2164, 1), gem_string("table")))) {
        _t2166 = gem_eq(gem_type_fn(NULL, _t2164, 1), gem_string("table"));
    } else {
        GemVal _t2165[] = {gem_v__match_31, gem_string("tag")};
        _t2166 = gem_has_key_fn(NULL, _t2165, 2);
    }
    GemVal _t2167;
    if (!gem_truthy(_t2166)) {
        _t2167 = _t2166;
    } else {
        _t2167 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2169;
    if (!gem_truthy(_t2167)) {
        _t2169 = _t2167;
    } else {
        GemVal _t2168[] = {gem_v__match_31, gem_string("value")};
        _t2169 = gem_has_key_fn(NULL, _t2168, 2);
    }
    if (gem_truthy(_t2169)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 548 "compiler/main.gem"
    GemVal _t2170[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2171 = (*gem_v_walk_captures_node);
        GemVal _t2172 = _t2171.fn(_t2171.env, _t2170, 3);
        gem_pop_frame();
        return _t2172;
    } else {
        {
#line 551 "compiler/main.gem"
            GemVal gem_v__for_tbl_39 = gem_v_node;
#line 551 "compiler/main.gem"
    GemVal _t2173[] = {gem_v__for_tbl_39};
            GemVal gem_v__for_keys_39 = gem_keys_fn(NULL, _t2173, 1);
#line 551 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 551 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2174[] = {gem_v__for_keys_39};
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_len_fn(NULL, _t2174, 1)))) break;
#line 551 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_39, gem_v__for_i_39);
#line 551 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_39, gem_table_get(gem_v__for_keys_39, gem_v__for_i_39));
#line 551 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 552 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 553 "compiler/main.gem"
    GemVal _t2175[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2175, 1), gem_string("table")))) {
#line 554 "compiler/main.gem"
    GemVal _t2176[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2176, 1);
#line 555 "compiler/main.gem"
    GemVal _t2177[] = {gem_v_vks};
    GemVal _t2179;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2177, 1), gem_int(0)))) {
        _t2179 = gem_gt(gem_len_fn(NULL, _t2177, 1), gem_int(0));
    } else {
        GemVal _t2178[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2179 = gem_eq(gem_type_fn(NULL, _t2178, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2179)) {
#line 556 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 557 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2180[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2180, 1)))) break;
#line 558 "compiler/main.gem"
    GemVal _t2181[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2181, 1), gem_string("table")))) {
#line 559 "compiler/main.gem"
    GemVal _t2182[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2183 = (*gem_v_walk_captures_node);
                                    (void)(_t2183.fn(_t2183.env, _t2182, 3));
                                }
#line 561 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 564 "compiler/main.gem"
    GemVal _t2184[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2185 = (*gem_v_walk_captures_node);
                            (void)(_t2185.fn(_t2185.env, _t2184, 3));
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
#line 573 "compiler/main.gem"
    GemVal _t2187 = gem_table_new();
    GemVal _t2188[] = {gem_v_scope_vars, _t2187};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t2188, 2);
    {
#line 574 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 574 "compiler/main.gem"
    GemVal _t2189[] = {gem_v_stmts};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2189, 1);
#line 574 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 574 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 574 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 575 "compiler/main.gem"
    GemVal _t2190[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2191 = (*gem_v_walk_captures_node);
            (void)(_t2191.fn(_t2191.env, _t2190, 3));
#line 576 "compiler/main.gem"
    GemVal _t2192[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t2192, 2))) {
#line 577 "compiler/main.gem"
    GemVal _t2193 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2194[] = {gem_v_sv, gem_table_get(_t2193, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2194, 2));
            }
#line 579 "compiler/main.gem"
    GemVal _t2195[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn_collect_block_lets(NULL, _t2195, 2));
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
#line 584 "compiler/main.gem"
    GemVal _t2197 = gem_table_new();
    GemVal gem_v_captured = _t2197;
#line 585 "compiler/main.gem"
    GemVal _t2198[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2199 = (*gem_v_walk_captures);
    (void)(_t2199.fn(_t2199.env, _t2198, 3));
    GemVal _t2200 = gem_v_captured;
    gem_pop_frame();
    return _t2200;
}

struct _closure__anon_32 {
    GemVal *gem_v_has_self_tail_call;
    GemVal *gem_v_is_self_tail_call;
};
static GemVal _anon_32(void *_env, GemVal *args, int argc) {
    struct _closure__anon_32 *_cls = (struct _closure__anon_32 *)_env;
    GemVal *gem_v_has_self_tail_call = _cls->gem_v_has_self_tail_call;
    GemVal *gem_v_is_self_tail_call = _cls->gem_v_is_self_tail_call;
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_node = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_32", "compiler/main.gem", 0);
#line 616 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 617 "compiler/main.gem"
        GemVal _t2202 = gem_bool(0);
        gem_pop_frame();
        return _t2202;
    }
#line 619 "compiler/main.gem"
    GemVal _t2203[] = {gem_v_node};
    GemVal _t2205;
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2203, 1), gem_string("table")))) {
        _t2205 = gem_neq(gem_type_fn(NULL, _t2203, 1), gem_string("table"));
    } else {
        GemVal _t2204 = gem_v_node;
        _t2205 = gem_eq(gem_table_get(_t2204, gem_string("tag")), GEM_NIL);
    }
    if (gem_truthy(_t2205)) {
#line 620 "compiler/main.gem"
        GemVal _t2206 = gem_bool(0);
        gem_pop_frame();
        return _t2206;
    }
#line 622 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2207[] = {gem_v__match_41};
    GemVal _t2209;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2207, 1), gem_string("table")))) {
        _t2209 = gem_eq(gem_type_fn(NULL, _t2207, 1), gem_string("table"));
    } else {
        GemVal _t2208[] = {gem_v__match_41, gem_string("tag")};
        _t2209 = gem_has_key_fn(NULL, _t2208, 2);
    }
    GemVal _t2210;
    if (!gem_truthy(_t2209)) {
        _t2210 = _t2209;
    } else {
        _t2210 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    GemVal _t2212;
    if (!gem_truthy(_t2210)) {
        _t2212 = _t2210;
    } else {
        GemVal _t2211[] = {gem_v__match_41, gem_string("func")};
        _t2212 = gem_has_key_fn(NULL, _t2211, 2);
    }
    if (gem_truthy(_t2212)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_func = gem_table_get(gem_v__match_41, gem_string("func"));
#line 624 "compiler/main.gem"
    GemVal _t2213[] = {gem_v_func, gem_string("var")};
    GemVal _t2215;
    if (!gem_truthy(gem_fn_is_node(NULL, _t2213, 2))) {
        _t2215 = gem_fn_is_node(NULL, _t2213, 2);
    } else {
        GemVal _t2214 = gem_v_func;
        _t2215 = gem_eq(gem_table_get(_t2214, gem_string("name")), gem_v_fn_name);
    }
        GemVal _t2216 = _t2215;
        gem_pop_frame();
        return _t2216;
    } else {
    GemVal _t2217[] = {gem_v__match_41};
    GemVal _t2219;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2217, 1), gem_string("table")))) {
        _t2219 = gem_eq(gem_type_fn(NULL, _t2217, 1), gem_string("table"));
    } else {
        GemVal _t2218[] = {gem_v__match_41, gem_string("tag")};
        _t2219 = gem_has_key_fn(NULL, _t2218, 2);
    }
    GemVal _t2220;
    if (!gem_truthy(_t2219)) {
        _t2220 = _t2219;
    } else {
        _t2220 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2222;
    if (!gem_truthy(_t2220)) {
        _t2222 = _t2220;
    } else {
        GemVal _t2221[] = {gem_v__match_41, gem_string("value")};
        _t2222 = gem_has_key_fn(NULL, _t2221, 2);
    }
    if (gem_truthy(_t2222)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 626 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 627 "compiler/main.gem"
    GemVal _t2223[] = {gem_v_fn_name, gem_v_value};
    GemVal _t2224 = (*gem_v_is_self_tail_call);
            GemVal _t2225 = _t2224.fn(_t2224.env, _t2223, 2);
            gem_pop_frame();
            return _t2225;
        }
#line 629 "compiler/main.gem"
        GemVal _t2226 = gem_bool(0);
        gem_pop_frame();
        return _t2226;
    } else {
    GemVal _t2227[] = {gem_v__match_41};
    GemVal _t2229;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2227, 1), gem_string("table")))) {
        _t2229 = gem_eq(gem_type_fn(NULL, _t2227, 1), gem_string("table"));
    } else {
        GemVal _t2228[] = {gem_v__match_41, gem_string("tag")};
        _t2229 = gem_has_key_fn(NULL, _t2228, 2);
    }
    GemVal _t2230;
    if (!gem_truthy(_t2229)) {
        _t2230 = _t2229;
    } else {
        _t2230 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2232;
    if (!gem_truthy(_t2230)) {
        _t2232 = _t2230;
    } else {
        GemVal _t2231[] = {gem_v__match_41, gem_string("then")};
        _t2232 = gem_has_key_fn(NULL, _t2231, 2);
    }
    GemVal _t2234;
    if (!gem_truthy(_t2232)) {
        _t2234 = _t2232;
    } else {
        GemVal _t2233[] = {gem_v__match_41, gem_string("else")};
        _t2234 = gem_has_key_fn(NULL, _t2233, 2);
    }
    if (gem_truthy(_t2234)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_then_body = gem_table_get(gem_v__match_41, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 631 "compiler/main.gem"
    GemVal _t2235[] = {gem_v_fn_name, gem_v_then_body};
    GemVal _t2236 = (*gem_v_has_self_tail_call);
        if (gem_truthy(_t2236.fn(_t2236.env, _t2235, 2))) {
#line 632 "compiler/main.gem"
            GemVal _t2237 = gem_bool(1);
            gem_pop_frame();
            return _t2237;
        }
#line 634 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 635 "compiler/main.gem"
    GemVal _t2238[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2239 = (*gem_v_has_self_tail_call);
            GemVal _t2240 = _t2239.fn(_t2239.env, _t2238, 2);
            gem_pop_frame();
            return _t2240;
        }
#line 637 "compiler/main.gem"
        GemVal _t2241 = gem_bool(0);
        gem_pop_frame();
        return _t2241;
    } else {
    GemVal _t2242[] = {gem_v__match_41};
    GemVal _t2244;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2242, 1), gem_string("table")))) {
        _t2244 = gem_eq(gem_type_fn(NULL, _t2242, 1), gem_string("table"));
    } else {
        GemVal _t2243[] = {gem_v__match_41, gem_string("tag")};
        _t2244 = gem_has_key_fn(NULL, _t2243, 2);
    }
    GemVal _t2245;
    if (!gem_truthy(_t2244)) {
        _t2245 = _t2244;
    } else {
        _t2245 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2247;
    if (!gem_truthy(_t2245)) {
        _t2247 = _t2245;
    } else {
        GemVal _t2246[] = {gem_v__match_41, gem_string("whens")};
        _t2247 = gem_has_key_fn(NULL, _t2246, 2);
    }
    GemVal _t2249;
    if (!gem_truthy(_t2247)) {
        _t2249 = _t2247;
    } else {
        GemVal _t2248[] = {gem_v__match_41, gem_string("else")};
        _t2249 = gem_has_key_fn(NULL, _t2248, 2);
    }
    if (gem_truthy(_t2249)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_41, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 639 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 639 "compiler/main.gem"
    GemVal _t2250[] = {gem_v_whens};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2250, 1);
#line 639 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 639 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 639 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 640 "compiler/main.gem"
    GemVal _t2251 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2252[] = {gem_v_fn_name, gem_table_get(_t2251, gem_string("body"))};
    GemVal _t2253 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2253.fn(_t2253.env, _t2252, 2))) {
#line 641 "compiler/main.gem"
                GemVal _t2254 = gem_bool(1);
                gem_pop_frame();
                return _t2254;
            }
        }

#line 644 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 645 "compiler/main.gem"
    GemVal _t2255[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2256 = (*gem_v_has_self_tail_call);
            GemVal _t2257 = _t2256.fn(_t2256.env, _t2255, 2);
            gem_pop_frame();
            return _t2257;
        }
#line 647 "compiler/main.gem"
        GemVal _t2258 = gem_bool(0);
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
        _t2262 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2264;
    if (!gem_truthy(_t2262)) {
        _t2264 = _t2262;
    } else {
        GemVal _t2263[] = {gem_v__match_41, gem_string("arms")};
        _t2264 = gem_has_key_fn(NULL, _t2263, 2);
    }
    GemVal _t2266;
    if (!gem_truthy(_t2264)) {
        _t2266 = _t2264;
    } else {
        GemVal _t2265[] = {gem_v__match_41, gem_string("after_body")};
        _t2266 = gem_has_key_fn(NULL, _t2265, 2);
    }
    if (gem_truthy(_t2266)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_arms = gem_table_get(gem_v__match_41, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_after_body = gem_table_get(gem_v__match_41, gem_string("after_body"));
#line 649 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(0);
#line 649 "compiler/main.gem"
    GemVal _t2267[] = {gem_v_arms};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2267, 1);
#line 649 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 649 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 649 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 650 "compiler/main.gem"
    GemVal _t2268 = gem_table_get(gem_v_arms, gem_v_i);
    GemVal _t2269[] = {gem_v_fn_name, gem_table_get(_t2268, gem_string("body"))};
    GemVal _t2270 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2270.fn(_t2270.env, _t2269, 2))) {
#line 651 "compiler/main.gem"
                GemVal _t2271 = gem_bool(1);
                gem_pop_frame();
                return _t2271;
            }
        }

#line 654 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 655 "compiler/main.gem"
    GemVal _t2272[] = {gem_v_fn_name, gem_v_after_body};
    GemVal _t2273 = (*gem_v_has_self_tail_call);
            GemVal _t2274 = _t2273.fn(_t2273.env, _t2272, 2);
            gem_pop_frame();
            return _t2274;
        }
#line 657 "compiler/main.gem"
        GemVal _t2275 = gem_bool(0);
        gem_pop_frame();
        return _t2275;
    } else {
    GemVal _t2276[] = {gem_v__match_41};
    GemVal _t2278;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2276, 1), gem_string("table")))) {
        _t2278 = gem_eq(gem_type_fn(NULL, _t2276, 1), gem_string("table"));
    } else {
        GemVal _t2277[] = {gem_v__match_41, gem_string("tag")};
        _t2278 = gem_has_key_fn(NULL, _t2277, 2);
    }
    GemVal _t2279;
    if (!gem_truthy(_t2278)) {
        _t2279 = _t2278;
    } else {
        _t2279 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2281;
    if (!gem_truthy(_t2279)) {
        _t2281 = _t2279;
    } else {
        GemVal _t2280[] = {gem_v__match_41, gem_string("stmts")};
        _t2281 = gem_has_key_fn(NULL, _t2280, 2);
    }
    if (gem_truthy(_t2281)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_41, gem_string("stmts"));
#line 659 "compiler/main.gem"
    GemVal _t2282[] = {gem_v_fn_name, gem_v_stmts};
    GemVal _t2283 = (*gem_v_has_self_tail_call);
        GemVal _t2284 = _t2283.fn(_t2283.env, _t2282, 2);
        gem_pop_frame();
        return _t2284;
    }
    }
    }
    }
    }
    }
#line 661 "compiler/main.gem"
    GemVal _t2285 = gem_bool(0);
    gem_pop_frame();
    return _t2285;
}

struct _closure__anon_33 {
    GemVal *gem_v_is_self_tail_call;
};
static GemVal _anon_33(void *_env, GemVal *args, int argc) {
    struct _closure__anon_33 *_cls = (struct _closure__anon_33 *)_env;
    GemVal *gem_v_is_self_tail_call = _cls->gem_v_is_self_tail_call;
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_stmts = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_33", "compiler/main.gem", 0);
#line 665 "compiler/main.gem"
    GemVal _t2287[] = {gem_v_stmts};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2287, 1), gem_int(0)))) {
#line 666 "compiler/main.gem"
        GemVal _t2288 = gem_bool(0);
        gem_pop_frame();
        return _t2288;
    }
#line 668 "compiler/main.gem"
    GemVal _t2289[] = {gem_v_fn_name, gem_table_get(gem_v_stmts, gem_neg(gem_int(1)))};
    GemVal _t2290 = (*gem_v_is_self_tail_call);
    GemVal _t2291 = _t2290.fn(_t2290.env, _t2289, 2);
    gem_pop_frame();
    return _t2291;
}

struct _closure__anon_34 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tco_captured;
    GemVal *gem_v_tco_params;
    GemVal *gem_v_tmp;
};
static GemVal _anon_34(void *_env, GemVal *args, int argc) {
    struct _closure__anon_34 *_cls = (struct _closure__anon_34 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tco_captured = _cls->gem_v_tco_captured;
    GemVal *gem_v_tco_params = _cls->gem_v_tco_params;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_call_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_34", "compiler/main.gem", 0);
#line 672 "compiler/main.gem"
    GemVal _t2293[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2293, 1);
#line 673 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 674 "compiler/main.gem"
    GemVal _t2294 = gem_v_call_node;
    GemVal gem_v_args = gem_table_get(_t2294, gem_string("args"));
#line 675 "compiler/main.gem"
    GemVal _t2295 = gem_table_new();
    GemVal gem_v_arg_temps = _t2295;
#line 676 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 676 "compiler/main.gem"
    GemVal _t2296[] = {gem_v_args};
    GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2296, 1);
#line 676 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 676 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_44;
#line 676 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 677 "compiler/main.gem"
    GemVal _t2297[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2298 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2298.fn(_t2298.env, _t2297, 1);
#line 678 "compiler/main.gem"
    GemVal _t2299 = (*gem_v_tmp);
        GemVal gem_v_t = _t2299.fn(_t2299.env, NULL, 0);
#line 679 "compiler/main.gem"
    GemVal _t2300 = gem_v_r;
    GemVal _t2301[] = {gem_v_b, gem_table_get(_t2300, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2301, 2));
#line 680 "compiler/main.gem"
    GemVal _t2302[] = {gem_v_p};
    GemVal _t2303[] = {gem_v_t};
    GemVal _t2304 = gem_v_r;
    GemVal _t2305[] = {gem_table_get(_t2304, gem_string("expr"))};
    GemVal _t2306[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2302, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2303, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2305, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t2306, 2));
#line 681 "compiler/main.gem"
    GemVal _t2307[] = {gem_v_arg_temps, gem_v_t};
        (void)(gem_push_fn(NULL, _t2307, 2));
    }

#line 683 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 683 "compiler/main.gem"
    GemVal _t2308[] = {(*gem_v_tco_params)};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2308, 1);
#line 683 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 683 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 683 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 684 "compiler/main.gem"
    GemVal _t2309[] = {gem_table_get((*gem_v_tco_params), gem_v_i)};
    GemVal _t2310 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2310.fn(_t2310.env, _t2309, 1);
#line 685 "compiler/main.gem"
        GemVal gem_v_val = gem_string("GEM_NIL");
#line 686 "compiler/main.gem"
    GemVal _t2311[] = {gem_v_arg_temps};
        if (gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t2311, 1)))) {
#line 687 "compiler/main.gem"
            gem_v_val = gem_table_get(gem_v_arg_temps, gem_v_i);
        }
#line 689 "compiler/main.gem"
    GemVal _t2312[] = {(*gem_v_tco_captured), gem_table_get((*gem_v_tco_params), gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2312, 2))) {
#line 690 "compiler/main.gem"
    GemVal _t2313[] = {gem_v_p};
    GemVal _t2314[] = {gem_v_mp};
    GemVal _t2315[] = {gem_v_val};
    GemVal _t2316[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2313, 1), gem_string("*")), gem_to_string_fn(NULL, _t2314, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2315, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2316, 2));
        } else {
#line 692 "compiler/main.gem"
    GemVal _t2317[] = {gem_v_p};
    GemVal _t2318[] = {gem_v_mp};
    GemVal _t2319[] = {gem_v_val};
    GemVal _t2320[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2317, 1), gem_to_string_fn(NULL, _t2318, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2319, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2320, 2));
        }
    }

#line 695 "compiler/main.gem"
    GemVal _t2321[] = {gem_v_p};
    GemVal _t2322[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t2321, 1), gem_string("continue;"))};
    (void)(gem_buf_push_fn(NULL, _t2322, 2));
#line 696 "compiler/main.gem"
    GemVal _t2323[] = {gem_v_b};
    GemVal _t2324 = gem_buf_str_fn(NULL, _t2323, 1);
    gem_pop_frame();
    return _t2324;
}

struct _closure__anon_35 {
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
static GemVal _anon_35(void *_env, GemVal *args, int argc) {
    struct _closure__anon_35 *_cls = (struct _closure__anon_35 *)_env;
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
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 702 "compiler/main.gem"
    GemVal gem_v__match_46 = gem_v_node;
    GemVal _t2326[] = {gem_v__match_46};
    GemVal _t2328;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2326, 1), gem_string("table")))) {
        _t2328 = gem_eq(gem_type_fn(NULL, _t2326, 1), gem_string("table"));
    } else {
        GemVal _t2327[] = {gem_v__match_46, gem_string("tag")};
        _t2328 = gem_has_key_fn(NULL, _t2327, 2);
    }
    GemVal _t2329;
    if (!gem_truthy(_t2328)) {
        _t2329 = _t2328;
    } else {
        _t2329 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2331;
    if (!gem_truthy(_t2329)) {
        _t2331 = _t2329;
    } else {
        GemVal _t2330[] = {gem_v__match_46, gem_string("value")};
        _t2331 = gem_has_key_fn(NULL, _t2330, 2);
    }
    if (gem_truthy(_t2331)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 704 "compiler/main.gem"
    GemVal _t2332 = gem_table_new();
    GemVal _t2333[] = {gem_v_value};
    gem_table_set(_t2332, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t2333, 1)), gem_string(")")));
    gem_table_set(_t2332, gem_string("setup"), gem_string(""));
        GemVal _t2334 = _t2332;
        gem_pop_frame();
        return _t2334;
    } else {
    GemVal _t2335[] = {gem_v__match_46};
    GemVal _t2337;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2335, 1), gem_string("table")))) {
        _t2337 = gem_eq(gem_type_fn(NULL, _t2335, 1), gem_string("table"));
    } else {
        GemVal _t2336[] = {gem_v__match_46, gem_string("tag")};
        _t2337 = gem_has_key_fn(NULL, _t2336, 2);
    }
    GemVal _t2338;
    if (!gem_truthy(_t2337)) {
        _t2338 = _t2337;
    } else {
        _t2338 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2340;
    if (!gem_truthy(_t2338)) {
        _t2340 = _t2338;
    } else {
        GemVal _t2339[] = {gem_v__match_46, gem_string("value")};
        _t2340 = gem_has_key_fn(NULL, _t2339, 2);
    }
    if (gem_truthy(_t2340)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 706 "compiler/main.gem"
    GemVal _t2341 = gem_table_new();
    GemVal _t2342[] = {gem_v_value};
    GemVal _t2343[] = {gem_fn_format_float(NULL, _t2342, 1)};
    gem_table_set(_t2341, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t2343, 1)), gem_string(")")));
    gem_table_set(_t2341, gem_string("setup"), gem_string(""));
        GemVal _t2344 = _t2341;
        gem_pop_frame();
        return _t2344;
    } else {
    GemVal _t2345[] = {gem_v__match_46};
    GemVal _t2347;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2345, 1), gem_string("table")))) {
        _t2347 = gem_eq(gem_type_fn(NULL, _t2345, 1), gem_string("table"));
    } else {
        GemVal _t2346[] = {gem_v__match_46, gem_string("tag")};
        _t2347 = gem_has_key_fn(NULL, _t2346, 2);
    }
    GemVal _t2348;
    if (!gem_truthy(_t2347)) {
        _t2348 = _t2347;
    } else {
        _t2348 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2350;
    if (!gem_truthy(_t2348)) {
        _t2350 = _t2348;
    } else {
        GemVal _t2349[] = {gem_v__match_46, gem_string("value")};
        _t2350 = gem_has_key_fn(NULL, _t2349, 2);
    }
    if (gem_truthy(_t2350)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 708 "compiler/main.gem"
    GemVal _t2351[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2351, 1);
#line 709 "compiler/main.gem"
    GemVal _t2352 = gem_table_new();
    GemVal _t2353[] = {gem_v_escaped};
    gem_table_set(_t2352, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t2353, 1)), gem_string("\")")));
    gem_table_set(_t2352, gem_string("setup"), gem_string(""));
        GemVal _t2354 = _t2352;
        gem_pop_frame();
        return _t2354;
    } else {
    GemVal _t2355[] = {gem_v__match_46};
    GemVal _t2357;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2355, 1), gem_string("table")))) {
        _t2357 = gem_eq(gem_type_fn(NULL, _t2355, 1), gem_string("table"));
    } else {
        GemVal _t2356[] = {gem_v__match_46, gem_string("tag")};
        _t2357 = gem_has_key_fn(NULL, _t2356, 2);
    }
    GemVal _t2358;
    if (!gem_truthy(_t2357)) {
        _t2358 = _t2357;
    } else {
        _t2358 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2360;
    if (!gem_truthy(_t2358)) {
        _t2360 = _t2358;
    } else {
        GemVal _t2359[] = {gem_v__match_46, gem_string("parts")};
        _t2360 = gem_has_key_fn(NULL, _t2359, 2);
    }
    if (gem_truthy(_t2360)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_46, gem_string("parts"));
#line 711 "compiler/main.gem"
    GemVal _t2361[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2361, 1), gem_int(0)))) {
#line 712 "compiler/main.gem"
    GemVal _t2362 = gem_table_new();
    gem_table_set(_t2362, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2362, gem_string("setup"), gem_string(""));
            GemVal _t2363 = _t2362;
            gem_pop_frame();
            return _t2363;
        }
#line 714 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 715 "compiler/main.gem"
    GemVal _t2364 = gem_table_new();
        GemVal gem_v_compiled = _t2364;
#line 716 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 716 "compiler/main.gem"
    GemVal _t2365[] = {gem_v_parts};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2365, 1);
#line 716 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 716 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_47;
#line 716 "compiler/main.gem"
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 717 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 718 "compiler/main.gem"
    GemVal _t2366[] = {gem_v_part};
    GemVal _t2367 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2367.fn(_t2367.env, _t2366, 1);
#line 719 "compiler/main.gem"
    GemVal _t2368 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2368, gem_string("setup")));
#line 720 "compiler/main.gem"
    GemVal _t2369 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t2369, gem_string("tag")), gem_string("string")))) {
#line 721 "compiler/main.gem"
    GemVal _t2370 = gem_v_r;
    GemVal _t2371[] = {gem_v_compiled, gem_table_get(_t2370, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t2371, 2));
            } else {
#line 723 "compiler/main.gem"
    GemVal _t2372 = (*gem_v_tmp);
                GemVal gem_v_ts = _t2372.fn(_t2372.env, NULL, 0);
#line 724 "compiler/main.gem"
    GemVal _t2373[] = {gem_v_ts};
    GemVal _t2374 = gem_v_r;
    GemVal _t2375[] = {gem_table_get(_t2374, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2373, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2375, 1)), gem_string("};\n")));
#line 725 "compiler/main.gem"
    GemVal _t2376[] = {gem_v_ts};
    GemVal _t2377[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t2376, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t2377, 2));
            }
        }

#line 728 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 729 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(1);
#line 729 "compiler/main.gem"
    GemVal _t2378[] = {gem_v_compiled};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2378, 1);
#line 729 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 729 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 729 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 730 "compiler/main.gem"
    GemVal _t2379[] = {gem_v_acc};
    GemVal _t2380[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2379, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2380, 1)), gem_string(")"));
        }

#line 732 "compiler/main.gem"
    GemVal _t2381 = gem_table_new();
    gem_table_set(_t2381, gem_string("expr"), gem_v_acc);
    gem_table_set(_t2381, gem_string("setup"), gem_v_setup);
        GemVal _t2382 = _t2381;
        gem_pop_frame();
        return _t2382;
    } else {
    GemVal _t2383[] = {gem_v__match_46};
    GemVal _t2385;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2383, 1), gem_string("table")))) {
        _t2385 = gem_eq(gem_type_fn(NULL, _t2383, 1), gem_string("table"));
    } else {
        GemVal _t2384[] = {gem_v__match_46, gem_string("tag")};
        _t2385 = gem_has_key_fn(NULL, _t2384, 2);
    }
    GemVal _t2386;
    if (!gem_truthy(_t2385)) {
        _t2386 = _t2385;
    } else {
        _t2386 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2388;
    if (!gem_truthy(_t2386)) {
        _t2388 = _t2386;
    } else {
        GemVal _t2387[] = {gem_v__match_46, gem_string("value")};
        _t2388 = gem_has_key_fn(NULL, _t2387, 2);
    }
    if (gem_truthy(_t2388)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 734 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 735 "compiler/main.gem"
    GemVal _t2389 = gem_table_new();
    gem_table_set(_t2389, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2389, gem_string("setup"), gem_string(""));
            GemVal _t2390 = _t2389;
            gem_pop_frame();
            return _t2390;
        }
#line 737 "compiler/main.gem"
    GemVal _t2391 = gem_table_new();
    gem_table_set(_t2391, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2391, gem_string("setup"), gem_string(""));
        GemVal _t2392 = _t2391;
        gem_pop_frame();
        return _t2392;
    } else {
    GemVal _t2393[] = {gem_v__match_46};
    GemVal _t2395;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2393, 1), gem_string("table")))) {
        _t2395 = gem_eq(gem_type_fn(NULL, _t2393, 1), gem_string("table"));
    } else {
        GemVal _t2394[] = {gem_v__match_46, gem_string("tag")};
        _t2395 = gem_has_key_fn(NULL, _t2394, 2);
    }
    GemVal _t2396;
    if (!gem_truthy(_t2395)) {
        _t2396 = _t2395;
    } else {
        _t2396 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2396)) {
#line 739 "compiler/main.gem"
    GemVal _t2397 = gem_table_new();
    gem_table_set(_t2397, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2397, gem_string("setup"), gem_string(""));
        GemVal _t2398 = _t2397;
        gem_pop_frame();
        return _t2398;
    } else {
    GemVal _t2399[] = {gem_v__match_46};
    GemVal _t2401;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2399, 1), gem_string("table")))) {
        _t2401 = gem_eq(gem_type_fn(NULL, _t2399, 1), gem_string("table"));
    } else {
        GemVal _t2400[] = {gem_v__match_46, gem_string("tag")};
        _t2401 = gem_has_key_fn(NULL, _t2400, 2);
    }
    GemVal _t2402;
    if (!gem_truthy(_t2401)) {
        _t2402 = _t2401;
    } else {
        _t2402 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2404;
    if (!gem_truthy(_t2402)) {
        _t2404 = _t2402;
    } else {
        GemVal _t2403[] = {gem_v__match_46, gem_string("name")};
        _t2404 = gem_has_key_fn(NULL, _t2403, 2);
    }
    if (gem_truthy(_t2404)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_46, gem_string("name"));
#line 741 "compiler/main.gem"
    GemVal _t2405[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t2405, 2)))) {
#line 742 "compiler/main.gem"
    GemVal _t2406[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2406, 2))) {
#line 743 "compiler/main.gem"
    GemVal _t2407 = gem_table_new();
    GemVal _t2408[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t2407, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2408, 1)), gem_string(", NULL)")));
    gem_table_set(_t2407, gem_string("setup"), gem_string(""));
                GemVal _t2409 = _t2407;
                gem_pop_frame();
                return _t2409;
            } else {
#line 744 "compiler/main.gem"
    GemVal _t2410[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t2410, 2))) {
#line 745 "compiler/main.gem"
    GemVal _t2411 = gem_table_new();
    GemVal _t2412[] = {gem_v_name};
    gem_table_set(_t2411, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t2412, 1)), gem_string(", NULL)")));
    gem_table_set(_t2411, gem_string("setup"), gem_string(""));
                    GemVal _t2413 = _t2411;
                    gem_pop_frame();
                    return _t2413;
                }
            }
        }
#line 748 "compiler/main.gem"
    GemVal _t2414[] = {gem_v_name};
    GemVal _t2415 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2415.fn(_t2415.env, _t2414, 1);
#line 749 "compiler/main.gem"
    GemVal _t2416[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2418;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2416, 2))) {
        _t2418 = gem_fn_set_contains(NULL, _t2416, 2);
    } else {
        GemVal _t2417[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2418 = gem_fn_set_contains(NULL, _t2417, 2);
    }
        if (gem_truthy(_t2418)) {
#line 750 "compiler/main.gem"
    GemVal _t2419 = gem_table_new();
    GemVal _t2420[] = {gem_v_mname};
    gem_table_set(_t2419, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t2420, 1)), gem_string(")")));
    gem_table_set(_t2419, gem_string("setup"), gem_string(""));
            GemVal _t2421 = _t2419;
            gem_pop_frame();
            return _t2421;
        }
#line 752 "compiler/main.gem"
    GemVal _t2422 = gem_table_new();
    gem_table_set(_t2422, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2422, gem_string("setup"), gem_string(""));
        GemVal _t2423 = _t2422;
        gem_pop_frame();
        return _t2423;
    } else {
    GemVal _t2424[] = {gem_v__match_46};
    GemVal _t2426;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2424, 1), gem_string("table")))) {
        _t2426 = gem_eq(gem_type_fn(NULL, _t2424, 1), gem_string("table"));
    } else {
        GemVal _t2425[] = {gem_v__match_46, gem_string("tag")};
        _t2426 = gem_has_key_fn(NULL, _t2425, 2);
    }
    GemVal _t2427;
    if (!gem_truthy(_t2426)) {
        _t2427 = _t2426;
    } else {
        _t2427 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2427)) {
#line 754 "compiler/main.gem"
    GemVal _t2428[] = {gem_v_node};
    GemVal _t2429 = (*gem_v_compile_call);
        GemVal _t2430 = _t2429.fn(_t2429.env, _t2428, 1);
        gem_pop_frame();
        return _t2430;
    } else {
    GemVal _t2431[] = {gem_v__match_46};
    GemVal _t2433;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2431, 1), gem_string("table")))) {
        _t2433 = gem_eq(gem_type_fn(NULL, _t2431, 1), gem_string("table"));
    } else {
        GemVal _t2432[] = {gem_v__match_46, gem_string("tag")};
        _t2433 = gem_has_key_fn(NULL, _t2432, 2);
    }
    GemVal _t2434;
    if (!gem_truthy(_t2433)) {
        _t2434 = _t2433;
    } else {
        _t2434 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2434)) {
#line 756 "compiler/main.gem"
    GemVal _t2435[] = {gem_v_node};
    GemVal _t2436 = (*gem_v_compile_binop);
        GemVal _t2437 = _t2436.fn(_t2436.env, _t2435, 1);
        gem_pop_frame();
        return _t2437;
    } else {
    GemVal _t2438[] = {gem_v__match_46};
    GemVal _t2440;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2438, 1), gem_string("table")))) {
        _t2440 = gem_eq(gem_type_fn(NULL, _t2438, 1), gem_string("table"));
    } else {
        GemVal _t2439[] = {gem_v__match_46, gem_string("tag")};
        _t2440 = gem_has_key_fn(NULL, _t2439, 2);
    }
    GemVal _t2441;
    if (!gem_truthy(_t2440)) {
        _t2441 = _t2440;
    } else {
        _t2441 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2443;
    if (!gem_truthy(_t2441)) {
        _t2443 = _t2441;
    } else {
        GemVal _t2442[] = {gem_v__match_46, gem_string("expr")};
        _t2443 = gem_has_key_fn(NULL, _t2442, 2);
    }
    GemVal _t2445;
    if (!gem_truthy(_t2443)) {
        _t2445 = _t2443;
    } else {
        GemVal _t2444[] = {gem_v__match_46, gem_string("op")};
        _t2445 = gem_has_key_fn(NULL, _t2444, 2);
    }
    if (gem_truthy(_t2445)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_46, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_46, gem_string("op"));
#line 758 "compiler/main.gem"
    GemVal _t2446[] = {gem_v_uexpr};
    GemVal _t2447 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2447.fn(_t2447.env, _t2446, 1);
#line 759 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 760 "compiler/main.gem"
    GemVal _t2448 = gem_table_new();
    GemVal _t2449 = gem_v_r;
    GemVal _t2450[] = {gem_table_get(_t2449, gem_string("expr"))};
    gem_table_set(_t2448, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t2450, 1)), gem_string(")")));
    GemVal _t2451 = gem_v_r;
    gem_table_set(_t2448, gem_string("setup"), gem_table_get(_t2451, gem_string("setup")));
            GemVal _t2452 = _t2448;
            gem_pop_frame();
            return _t2452;
        } else {
#line 761 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 762 "compiler/main.gem"
    GemVal _t2453 = gem_table_new();
    GemVal _t2454 = gem_v_r;
    GemVal _t2455[] = {gem_table_get(_t2454, gem_string("expr"))};
    gem_table_set(_t2453, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2455, 1)), gem_string(")")));
    GemVal _t2456 = gem_v_r;
    gem_table_set(_t2453, gem_string("setup"), gem_table_get(_t2456, gem_string("setup")));
                GemVal _t2457 = _t2453;
                gem_pop_frame();
                return _t2457;
            }
        }
#line 764 "compiler/main.gem"
    GemVal _t2458[] = {gem_v_op};
    GemVal _t2459[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2458, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 764, _t2459, 1));
    } else {
    GemVal _t2460[] = {gem_v__match_46};
    GemVal _t2462;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2460, 1), gem_string("table")))) {
        _t2462 = gem_eq(gem_type_fn(NULL, _t2460, 1), gem_string("table"));
    } else {
        GemVal _t2461[] = {gem_v__match_46, gem_string("tag")};
        _t2462 = gem_has_key_fn(NULL, _t2461, 2);
    }
    GemVal _t2463;
    if (!gem_truthy(_t2462)) {
        _t2463 = _t2462;
    } else {
        _t2463 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2463)) {
#line 766 "compiler/main.gem"
    GemVal _t2464[] = {gem_v_node};
    GemVal _t2465 = (*gem_v_compile_anon_fn);
        GemVal _t2466 = _t2465.fn(_t2465.env, _t2464, 1);
        gem_pop_frame();
        return _t2466;
    } else {
    GemVal _t2467[] = {gem_v__match_46};
    GemVal _t2469;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2467, 1), gem_string("table")))) {
        _t2469 = gem_eq(gem_type_fn(NULL, _t2467, 1), gem_string("table"));
    } else {
        GemVal _t2468[] = {gem_v__match_46, gem_string("tag")};
        _t2469 = gem_has_key_fn(NULL, _t2468, 2);
    }
    GemVal _t2470;
    if (!gem_truthy(_t2469)) {
        _t2470 = _t2469;
    } else {
        _t2470 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2470)) {
#line 768 "compiler/main.gem"
    GemVal _t2471[] = {gem_v_node};
    GemVal _t2472 = (*gem_v_compile_table);
        GemVal _t2473 = _t2472.fn(_t2472.env, _t2471, 1);
        gem_pop_frame();
        return _t2473;
    } else {
    GemVal _t2474[] = {gem_v__match_46};
    GemVal _t2476;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2474, 1), gem_string("table")))) {
        _t2476 = gem_eq(gem_type_fn(NULL, _t2474, 1), gem_string("table"));
    } else {
        GemVal _t2475[] = {gem_v__match_46, gem_string("tag")};
        _t2476 = gem_has_key_fn(NULL, _t2475, 2);
    }
    GemVal _t2477;
    if (!gem_truthy(_t2476)) {
        _t2477 = _t2476;
    } else {
        _t2477 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2477)) {
#line 770 "compiler/main.gem"
    GemVal _t2478[] = {gem_v_node};
    GemVal _t2479 = (*gem_v_compile_array);
        GemVal _t2480 = _t2479.fn(_t2479.env, _t2478, 1);
        gem_pop_frame();
        return _t2480;
    } else {
    GemVal _t2481[] = {gem_v__match_46};
    GemVal _t2483;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2481, 1), gem_string("table")))) {
        _t2483 = gem_eq(gem_type_fn(NULL, _t2481, 1), gem_string("table"));
    } else {
        GemVal _t2482[] = {gem_v__match_46, gem_string("tag")};
        _t2483 = gem_has_key_fn(NULL, _t2482, 2);
    }
    GemVal _t2484;
    if (!gem_truthy(_t2483)) {
        _t2484 = _t2483;
    } else {
        _t2484 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2486;
    if (!gem_truthy(_t2484)) {
        _t2486 = _t2484;
    } else {
        GemVal _t2485[] = {gem_v__match_46, gem_string("object")};
        _t2486 = gem_has_key_fn(NULL, _t2485, 2);
    }
    GemVal _t2488;
    if (!gem_truthy(_t2486)) {
        _t2488 = _t2486;
    } else {
        GemVal _t2487[] = {gem_v__match_46, gem_string("field")};
        _t2488 = gem_has_key_fn(NULL, _t2487, 2);
    }
    if (gem_truthy(_t2488)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_46, gem_string("field"));
#line 772 "compiler/main.gem"
    GemVal _t2489[] = {gem_v_object};
    GemVal _t2490 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2490.fn(_t2490.env, _t2489, 1);
#line 773 "compiler/main.gem"
    GemVal _t2491 = (*gem_v_tmp);
        GemVal gem_v_t = _t2491.fn(_t2491.env, NULL, 0);
#line 774 "compiler/main.gem"
    GemVal _t2492 = gem_v_r;
    GemVal _t2493[] = {gem_v_t};
    GemVal _t2494 = gem_v_r;
    GemVal _t2495[] = {gem_table_get(_t2494, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2492, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2493, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2495, 1)), gem_string(";\n")));
#line 775 "compiler/main.gem"
    GemVal _t2496[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2496, 1);
#line 776 "compiler/main.gem"
    GemVal _t2497 = gem_table_new();
    GemVal _t2498[] = {gem_v_t};
    GemVal _t2499[] = {gem_v_escaped};
    gem_table_set(_t2497, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2498, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2499, 1)), gem_string("\"))")));
    gem_table_set(_t2497, gem_string("setup"), gem_v_setup);
        GemVal _t2500 = _t2497;
        gem_pop_frame();
        return _t2500;
    } else {
    GemVal _t2501[] = {gem_v__match_46};
    GemVal _t2503;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2501, 1), gem_string("table")))) {
        _t2503 = gem_eq(gem_type_fn(NULL, _t2501, 1), gem_string("table"));
    } else {
        GemVal _t2502[] = {gem_v__match_46, gem_string("tag")};
        _t2503 = gem_has_key_fn(NULL, _t2502, 2);
    }
    GemVal _t2504;
    if (!gem_truthy(_t2503)) {
        _t2504 = _t2503;
    } else {
        _t2504 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2506;
    if (!gem_truthy(_t2504)) {
        _t2506 = _t2504;
    } else {
        GemVal _t2505[] = {gem_v__match_46, gem_string("object")};
        _t2506 = gem_has_key_fn(NULL, _t2505, 2);
    }
    GemVal _t2508;
    if (!gem_truthy(_t2506)) {
        _t2508 = _t2506;
    } else {
        GemVal _t2507[] = {gem_v__match_46, gem_string("key")};
        _t2508 = gem_has_key_fn(NULL, _t2507, 2);
    }
    if (gem_truthy(_t2508)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_46, gem_string("key"));
#line 778 "compiler/main.gem"
    GemVal _t2509[] = {gem_v_object};
    GemVal _t2510 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2510.fn(_t2510.env, _t2509, 1);
#line 779 "compiler/main.gem"
    GemVal _t2511[] = {gem_v_key};
    GemVal _t2512 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2512.fn(_t2512.env, _t2511, 1);
#line 780 "compiler/main.gem"
    GemVal _t2513 = gem_table_new();
    GemVal _t2514 = gem_v_obj_r;
    GemVal _t2515[] = {gem_table_get(_t2514, gem_string("expr"))};
    GemVal _t2516 = gem_v_key_r;
    GemVal _t2517[] = {gem_table_get(_t2516, gem_string("expr"))};
    gem_table_set(_t2513, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2515, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2517, 1)), gem_string(")")));
    GemVal _t2518 = gem_v_obj_r;
    GemVal _t2519 = gem_v_key_r;
    gem_table_set(_t2513, gem_string("setup"), gem_add(gem_table_get(_t2518, gem_string("setup")), gem_table_get(_t2519, gem_string("setup"))));
        GemVal _t2520 = _t2513;
        gem_pop_frame();
        return _t2520;
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
#line 783 "compiler/main.gem"
    GemVal _t2521 = gem_v_node;
    GemVal _t2522[] = {gem_table_get(_t2521, gem_string("tag"))};
    GemVal _t2523[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2522, 1))};
    GemVal _t2524 = gem_error_at_fn("compiler/main.gem", 783, _t2523, 1);
    gem_pop_frame();
    return _t2524;
}

struct _closure__anon_36 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_36", "compiler/main.gem", 0);
#line 789 "compiler/main.gem"
    GemVal _t2526 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2526, gem_string("entries"));
#line 790 "compiler/main.gem"
    GemVal _t2527 = (*gem_v_tmp);
    GemVal gem_v_t = _t2527.fn(_t2527.env, NULL, 0);
#line 791 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 792 "compiler/main.gem"
    GemVal _t2528[] = {gem_v_t};
    GemVal _t2529[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2528, 1)), gem_string(" = gem_table_new();\n"))};
    (void)(gem_buf_push_fn(NULL, _t2529, 2));
#line 793 "compiler/main.gem"
    GemVal gem_v__for_items_49 = gem_v_entries;
#line 793 "compiler/main.gem"
    GemVal gem_v__for_i_49 = gem_int(0);
#line 793 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2530[] = {gem_v__for_items_49};
        if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2530, 1)))) break;
#line 793 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 793 "compiler/main.gem"
        gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 794 "compiler/main.gem"
    GemVal _t2531 = gem_v_entry;
    GemVal _t2532[] = {gem_table_get(_t2531, gem_string("value"))};
    GemVal _t2533 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2533.fn(_t2533.env, _t2532, 1);
#line 795 "compiler/main.gem"
    GemVal _t2534 = gem_v_entry;
    GemVal _t2535[] = {gem_table_get(_t2534, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2535, 1);
#line 796 "compiler/main.gem"
    GemVal _t2536 = gem_v_val_r;
    GemVal _t2537[] = {gem_v_b, gem_table_get(_t2536, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2537, 2));
#line 797 "compiler/main.gem"
    GemVal _t2538[] = {gem_v_t};
    GemVal _t2539[] = {gem_v_escaped};
    GemVal _t2540 = gem_v_val_r;
    GemVal _t2541[] = {gem_table_get(_t2540, gem_string("expr"))};
    GemVal _t2542[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2538, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2539, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2541, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t2542, 2));
    }

    GemVal _t2543 = gem_table_new();
    gem_table_set(_t2543, gem_string("expr"), gem_v_t);
    GemVal _t2544[] = {gem_v_b};
    gem_table_set(_t2543, gem_string("setup"), gem_buf_str_fn(NULL, _t2544, 1));
    GemVal _t2545 = _t2543;
    gem_pop_frame();
    return _t2545;
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
#line 805 "compiler/main.gem"
    GemVal _t2547 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2547, gem_string("elements"));
#line 806 "compiler/main.gem"
    GemVal _t2548 = (*gem_v_tmp);
    GemVal gem_v_t = _t2548.fn(_t2548.env, NULL, 0);
#line 807 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 808 "compiler/main.gem"
    GemVal _t2549[] = {gem_v_t};
    GemVal _t2550[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2549, 1)), gem_string(" = gem_table_new();\n"))};
    (void)(gem_buf_push_fn(NULL, _t2550, 2));
#line 809 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 809 "compiler/main.gem"
    GemVal _t2551[] = {gem_v_elements};
    GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2551, 1);
#line 809 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 809 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_50;
#line 809 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 810 "compiler/main.gem"
    GemVal _t2552[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2553 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2553.fn(_t2553.env, _t2552, 1);
#line 811 "compiler/main.gem"
    GemVal _t2554 = gem_v_elem_r;
    GemVal _t2555[] = {gem_v_b, gem_table_get(_t2554, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2555, 2));
#line 812 "compiler/main.gem"
    GemVal _t2556[] = {gem_v_t};
    GemVal _t2557[] = {gem_v_i};
    GemVal _t2558 = gem_v_elem_r;
    GemVal _t2559[] = {gem_table_get(_t2558, gem_string("expr"))};
    GemVal _t2560[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2556, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2557, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2559, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t2560, 2));
    }

    GemVal _t2561 = gem_table_new();
    gem_table_set(_t2561, gem_string("expr"), gem_v_t);
    GemVal _t2562[] = {gem_v_b};
    gem_table_set(_t2561, gem_string("setup"), gem_buf_str_fn(NULL, _t2562, 1));
    GemVal _t2563 = _t2561;
    gem_pop_frame();
    return _t2563;
}

struct _closure__anon_38 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_38(void *_env, GemVal *args, int argc) {
    struct _closure__anon_38 *_cls = (struct _closure__anon_38 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn = _cls->gem_v_compile_closure_fn;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 820 "compiler/main.gem"
    GemVal _t2565 = gem_v_node;
    GemVal _t2566[] = {gem_table_get(_t2565, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t2566, 1);
#line 821 "compiler/main.gem"
    GemVal _t2567 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2567, gem_string("rest_param")), GEM_NIL))) {
#line 822 "compiler/main.gem"
    GemVal _t2568 = gem_v_node;
    GemVal _t2569[] = {gem_v_inner_defined, gem_table_get(_t2568, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2569, 2));
    }
#line 824 "compiler/main.gem"
    GemVal _t2570 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2570, gem_string("block_param")), GEM_NIL))) {
#line 825 "compiler/main.gem"
    GemVal _t2571 = gem_v_node;
    GemVal _t2572[] = {gem_v_inner_defined, gem_table_get(_t2571, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2572, 2));
    }
#line 827 "compiler/main.gem"
    GemVal _t2573 = gem_v_node;
    GemVal _t2574[] = {gem_table_get(_t2573, gem_string("body")), gem_v_inner_defined};
    GemVal _t2575 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2575.fn(_t2575.env, _t2574, 2);
#line 828 "compiler/main.gem"
    GemVal _t2576[] = {gem_v_free};
    GemVal _t2577[] = {gem_keys_fn(NULL, _t2576, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2577, 1);
#line 830 "compiler/main.gem"
    GemVal _t2578[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2578, 1), gem_int(0)))) {
#line 831 "compiler/main.gem"
    GemVal _t2579 = gem_table_new();
    GemVal _t2580[] = {gem_v_node, _t2579};
    GemVal _t2581 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2581.fn(_t2581.env, _t2580, 2);
#line 832 "compiler/main.gem"
    GemVal _t2582 = gem_table_new();
    GemVal _t2583 = gem_v_result;
    GemVal _t2584[] = {gem_table_get(_t2583, gem_string("fn_name"))};
    gem_table_set(_t2582, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2584, 1)), gem_string(", NULL)")));
    gem_table_set(_t2582, gem_string("setup"), gem_string(""));
        GemVal _t2585 = _t2582;
        gem_pop_frame();
        return _t2585;
    }
#line 835 "compiler/main.gem"
    GemVal _t2586[] = {gem_v_node, gem_v_captures};
    GemVal _t2587 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2587.fn(_t2587.env, _t2586, 2);
#line 837 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 838 "compiler/main.gem"
    GemVal _t2588 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2588.fn(_t2588.env, NULL, 0);
#line 839 "compiler/main.gem"
    GemVal _t2589 = gem_v_result;
    GemVal _t2590[] = {gem_table_get(_t2589, gem_string("struct_name"))};
    GemVal _t2591[] = {gem_v_env_tmp};
    GemVal _t2592 = gem_v_result;
    GemVal _t2593[] = {gem_table_get(_t2592, gem_string("struct_name"))};
    GemVal _t2594[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2590, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2591, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2593, 1)), gem_string("));\n"))};
    (void)(gem_buf_push_fn(NULL, _t2594, 2));
#line 840 "compiler/main.gem"
    GemVal gem_v__for_items_51 = gem_v_captures;
#line 840 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 840 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2595[] = {gem_v__for_items_51};
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t2595, 1)))) break;
#line 840 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 840 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 841 "compiler/main.gem"
    GemVal _t2596[] = {gem_v_cap};
    GemVal _t2597 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2597.fn(_t2597.env, _t2596, 1);
#line 842 "compiler/main.gem"
    GemVal _t2598[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2598, 2))) {
#line 843 "compiler/main.gem"
    GemVal _t2599[] = {gem_v_env_tmp};
    GemVal _t2600[] = {gem_v_mc};
    GemVal _t2601[] = {gem_v_mc};
    GemVal _t2602[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2599, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2600, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2601, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2602, 2));
        } else {
#line 845 "compiler/main.gem"
    GemVal _t2603[] = {gem_v_env_tmp};
    GemVal _t2604[] = {gem_v_mc};
    GemVal _t2605[] = {gem_v_mc};
    GemVal _t2606[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2603, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2604, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2605, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2606, 2));
        }
    }

    GemVal _t2607 = gem_table_new();
    GemVal _t2608 = gem_v_result;
    GemVal _t2609[] = {gem_table_get(_t2608, gem_string("fn_name"))};
    GemVal _t2610[] = {gem_v_env_tmp};
    gem_table_set(_t2607, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2609, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2610, 1)), gem_string(")")));
    GemVal _t2611[] = {gem_v_b};
    gem_table_set(_t2607, gem_string("setup"), gem_buf_str_fn(NULL, _t2611, 1));
    GemVal _t2612 = _t2607;
    gem_pop_frame();
    return _t2612;
}

struct _closure__anon_39 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_39(void *_env, GemVal *args, int argc) {
    struct _closure__anon_39 *_cls = (struct _closure__anon_39 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_39", "compiler/main.gem", 0);
#line 855 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 856 "compiler/main.gem"
    GemVal _t2614 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2614;
#line 857 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 857 "compiler/main.gem"
    GemVal _t2615[] = {gem_v_args};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2615, 1);
#line 857 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 857 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 857 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 858 "compiler/main.gem"
    GemVal _t2616[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2617 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2617.fn(_t2617.env, _t2616, 1);
#line 859 "compiler/main.gem"
    GemVal _t2618 = gem_v_r;
    GemVal _t2619[] = {gem_v_b, gem_table_get(_t2618, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2619, 2));
#line 860 "compiler/main.gem"
    GemVal _t2620 = gem_v_r;
    GemVal _t2621[] = {gem_v_arg_exprs, gem_table_get(_t2620, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2621, 2));
    }

#line 862 "compiler/main.gem"
    GemVal _t2622[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2622, 1);
#line 863 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 864 "compiler/main.gem"
    GemVal _t2623 = gem_table_new();
    GemVal _t2624[] = {gem_v_b};
    gem_table_set(_t2623, gem_string("setup"), gem_buf_str_fn(NULL, _t2624, 1));
    gem_table_set(_t2623, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2623, gem_string("argc"), gem_int(0));
        GemVal _t2625 = _t2623;
        gem_pop_frame();
        return _t2625;
    }
#line 866 "compiler/main.gem"
    GemVal _t2626 = (*gem_v_tmp);
    GemVal gem_v_t = _t2626.fn(_t2626.env, NULL, 0);
#line 867 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 868 "compiler/main.gem"
    GemVal gem_v__for_i_53 = gem_int(1);
#line 868 "compiler/main.gem"
    GemVal gem_v__for_limit_53 = gem_v_argc;
#line 868 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_v__for_limit_53))) break;
#line 868 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_53;
#line 868 "compiler/main.gem"
        gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 869 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2627 = gem_table_new();
    GemVal _t2628[] = {gem_v_b};
    GemVal _t2629[] = {gem_v_t};
    GemVal _t2630[] = {gem_v_arr};
    gem_table_set(_t2627, gem_string("setup"), gem_add(gem_buf_str_fn(NULL, _t2628, 1), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2629, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2630, 1)), gem_string("};\n"))));
    gem_table_set(_t2627, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2627, gem_string("argc"), gem_v_argc);
    GemVal _t2631 = _t2627;
    gem_pop_frame();
    return _t2631;
}

struct _closure__anon_40 {
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 876 "compiler/main.gem"
    GemVal _t2633 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2633, gem_string("argc")), gem_int(0)))) {
#line 877 "compiler/main.gem"
    GemVal _t2634 = gem_table_new();
    GemVal _t2635[] = {gem_v_fn_name};
    GemVal _t2636[] = {gem_v_env};
    gem_table_set(_t2634, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2635, 1), gem_string("(")), gem_to_string_fn(NULL, _t2636, 1)), gem_string(", NULL, 0)")));
    GemVal _t2637 = gem_v_ca;
    gem_table_set(_t2634, gem_string("setup"), gem_table_get(_t2637, gem_string("setup")));
        GemVal _t2638 = _t2634;
        gem_pop_frame();
        return _t2638;
    }
    GemVal _t2639 = gem_table_new();
    GemVal _t2640[] = {gem_v_fn_name};
    GemVal _t2641[] = {gem_v_env};
    GemVal _t2642 = gem_v_ca;
    GemVal _t2643[] = {gem_table_get(_t2642, gem_string("arr_name"))};
    GemVal _t2644 = gem_v_ca;
    GemVal _t2645[] = {gem_table_get(_t2644, gem_string("argc"))};
    gem_table_set(_t2639, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2640, 1), gem_string("(")), gem_to_string_fn(NULL, _t2641, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2643, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2645, 1)), gem_string(")")));
    GemVal _t2646 = gem_v_ca;
    gem_table_set(_t2639, gem_string("setup"), gem_table_get(_t2646, gem_string("setup")));
    GemVal _t2647 = _t2639;
    gem_pop_frame();
    return _t2647;
}

struct _closure__anon_41 {
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_args;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call;
    GemVal *gem_v_local_names;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_41(void *_env, GemVal *args, int argc) {
    struct _closure__anon_41 *_cls = (struct _closure__anon_41 *)_env;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_args = _cls->gem_v_compile_args;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call = _cls->gem_v_emit_direct_call;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_41", "compiler/main.gem", 0);
#line 885 "compiler/main.gem"
    GemVal gem_v__d54 = gem_v_node;
#line 885 "compiler/main.gem"
    GemVal _t2648 = gem_v__d54;
    GemVal gem_v_func = gem_table_get(_t2648, gem_string("func"));
#line 885 "compiler/main.gem"
    GemVal _t2649 = gem_v__d54;
    GemVal gem_v_args = gem_table_get(_t2649, gem_string("args"));

#line 888 "compiler/main.gem"
    GemVal _t2650 = gem_v_func;
    GemVal _t2652;
    if (!gem_truthy(gem_eq(gem_table_get(_t2650, gem_string("tag")), gem_string("dot")))) {
        _t2652 = gem_eq(gem_table_get(_t2650, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2651 = gem_v_func;
        _t2652 = gem_eq(gem_table_get(_t2651, gem_string("field")), gem_string("len"));
    }
    GemVal _t2654;
    if (!gem_truthy(_t2652)) {
        _t2654 = _t2652;
    } else {
        GemVal _t2653[] = {gem_v_args};
        _t2654 = gem_eq(gem_len_fn(NULL, _t2653, 1), gem_int(0));
    }
    if (gem_truthy(_t2654)) {
#line 889 "compiler/main.gem"
    GemVal _t2655 = gem_v_func;
    GemVal _t2656[] = {gem_table_get(_t2655, gem_string("object"))};
    GemVal _t2657 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2657.fn(_t2657.env, _t2656, 1);
#line 890 "compiler/main.gem"
    GemVal _t2658 = gem_table_new();
    GemVal _t2659 = gem_v_obj_r;
    GemVal _t2660[] = {gem_table_get(_t2659, gem_string("expr"))};
    gem_table_set(_t2658, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2660, 1)), gem_string(")")));
    GemVal _t2661 = gem_v_obj_r;
    gem_table_set(_t2658, gem_string("setup"), gem_table_get(_t2661, gem_string("setup")));
        GemVal _t2662 = _t2658;
        gem_pop_frame();
        return _t2662;
    }
#line 894 "compiler/main.gem"
    GemVal _t2663 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2663, gem_string("tag")), gem_string("var")))) {
#line 895 "compiler/main.gem"
    GemVal _t2664 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2664, gem_string("name"));
#line 897 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 898 "compiler/main.gem"
    GemVal _t2665 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2665, gem_string("line"));
#line 899 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 900 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 902 "compiler/main.gem"
    GemVal _t2666[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t2666, 1);
#line 903 "compiler/main.gem"
    GemVal _t2667[] = {gem_v_args};
    GemVal _t2668 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2668.fn(_t2668.env, _t2667, 1);
#line 904 "compiler/main.gem"
    GemVal _t2669 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2669, gem_string("argc")), gem_int(0)))) {
#line 905 "compiler/main.gem"
    GemVal _t2670 = gem_table_new();
    GemVal _t2671[] = {gem_v_escaped_file};
    GemVal _t2672[] = {gem_v_line};
    gem_table_set(_t2670, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2671, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2672, 1)), gem_string(", NULL, 0)")));
    GemVal _t2673 = gem_v_ca;
    gem_table_set(_t2670, gem_string("setup"), gem_table_get(_t2673, gem_string("setup")));
                GemVal _t2674 = _t2670;
                gem_pop_frame();
                return _t2674;
            }
#line 907 "compiler/main.gem"
    GemVal _t2675 = gem_table_new();
    GemVal _t2676[] = {gem_v_escaped_file};
    GemVal _t2677[] = {gem_v_line};
    GemVal _t2678 = gem_v_ca;
    GemVal _t2679[] = {gem_table_get(_t2678, gem_string("arr_name"))};
    GemVal _t2680 = gem_v_ca;
    GemVal _t2681[] = {gem_table_get(_t2680, gem_string("argc"))};
    gem_table_set(_t2675, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2676, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2677, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2679, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2681, 1)), gem_string(")")));
    GemVal _t2682 = gem_v_ca;
    gem_table_set(_t2675, gem_string("setup"), gem_table_get(_t2682, gem_string("setup")));
            GemVal _t2683 = _t2675;
            gem_pop_frame();
            return _t2683;
        } else {
#line 908 "compiler/main.gem"
    GemVal _t2684[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2686;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2684, 2))) {
        _t2686 = gem_has_key_fn(NULL, _t2684, 2);
    } else {
        GemVal _t2685[] = {(*gem_v_local_names), gem_v_name};
        _t2686 = gem_not(gem_fn_set_contains(NULL, _t2685, 2));
    }
            if (gem_truthy(_t2686)) {
#line 909 "compiler/main.gem"
    GemVal _t2687[] = {gem_v_args};
    GemVal _t2688 = (*gem_v_compile_args);
    GemVal _t2689[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2688.fn(_t2688.env, _t2687, 1)};
    GemVal _t2690 = (*gem_v_emit_direct_call);
                GemVal _t2691 = _t2690.fn(_t2690.env, _t2689, 3);
                gem_pop_frame();
                return _t2691;
            } else {
#line 910 "compiler/main.gem"
    GemVal _t2692[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2694;
    if (!gem_truthy(gem_fn_set_contains(NULL, _t2692, 2))) {
        _t2694 = gem_fn_set_contains(NULL, _t2692, 2);
    } else {
        GemVal _t2693[] = {(*gem_v_local_names), gem_v_name};
        _t2694 = gem_not(gem_fn_set_contains(NULL, _t2693, 2));
    }
                if (gem_truthy(_t2694)) {
#line 911 "compiler/main.gem"
    GemVal _t2695[] = {gem_v_name};
    GemVal _t2696[] = {gem_v_args};
    GemVal _t2697 = (*gem_v_compile_args);
    GemVal _t2698[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2695, 1)), gem_string("NULL"), _t2697.fn(_t2697.env, _t2696, 1)};
    GemVal _t2699 = (*gem_v_emit_direct_call);
                    GemVal _t2700 = _t2699.fn(_t2699.env, _t2698, 3);
                    gem_pop_frame();
                    return _t2700;
                }
            }
        }
    }
#line 916 "compiler/main.gem"
    GemVal _t2701[] = {gem_v_func};
    GemVal _t2702 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2702.fn(_t2702.env, _t2701, 1);
#line 917 "compiler/main.gem"
    GemVal _t2703[] = {gem_v_args};
    GemVal _t2704 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2704.fn(_t2704.env, _t2703, 1);
#line 918 "compiler/main.gem"
    GemVal _t2705 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2705.fn(_t2705.env, NULL, 0);
#line 919 "compiler/main.gem"
    GemVal _t2706 = gem_v_func_r;
    GemVal _t2707 = gem_v_ca;
    GemVal _t2708[] = {gem_v_tmp_fn};
    GemVal _t2709 = gem_v_func_r;
    GemVal _t2710[] = {gem_table_get(_t2709, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2706, gem_string("setup")), gem_table_get(_t2707, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2708, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2710, 1)), gem_string(";\n")));
#line 920 "compiler/main.gem"
    GemVal _t2711 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2711, gem_string("argc")), gem_int(0)))) {
#line 921 "compiler/main.gem"
    GemVal _t2712 = gem_table_new();
    GemVal _t2713[] = {gem_v_tmp_fn};
    GemVal _t2714[] = {gem_v_tmp_fn};
    gem_table_set(_t2712, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2713, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2714, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2712, gem_string("setup"), gem_v_setup);
        GemVal _t2715 = _t2712;
        gem_pop_frame();
        return _t2715;
    }
    GemVal _t2716 = gem_table_new();
    GemVal _t2717[] = {gem_v_tmp_fn};
    GemVal _t2718[] = {gem_v_tmp_fn};
    GemVal _t2719 = gem_v_ca;
    GemVal _t2720[] = {gem_table_get(_t2719, gem_string("arr_name"))};
    GemVal _t2721 = gem_v_ca;
    GemVal _t2722[] = {gem_table_get(_t2721, gem_string("argc"))};
    gem_table_set(_t2716, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2717, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2718, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2720, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2722, 1)), gem_string(")")));
    gem_table_set(_t2716, gem_string("setup"), gem_v_setup);
    GemVal _t2723 = _t2716;
    gem_pop_frame();
    return _t2723;
}

struct _closure__anon_42 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_42(void *_env, GemVal *args, int argc) {
    struct _closure__anon_42 *_cls = (struct _closure__anon_42 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_42", "compiler/main.gem", 0);
#line 929 "compiler/main.gem"
    GemVal _t2725 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2725, gem_string("op"));
#line 930 "compiler/main.gem"
    GemVal _t2726 = gem_v_node;
    GemVal _t2727[] = {gem_table_get(_t2726, gem_string("left"))};
    GemVal _t2728 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2728.fn(_t2728.env, _t2727, 1);
#line 931 "compiler/main.gem"
    GemVal _t2729 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2729, gem_string("expr"));
#line 932 "compiler/main.gem"
    GemVal _t2730 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2730, gem_string("setup"));
#line 934 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 935 "compiler/main.gem"
    GemVal _t2731 = gem_v_node;
    GemVal _t2732[] = {gem_table_get(_t2731, gem_string("right"))};
    GemVal _t2733 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2733.fn(_t2733.env, _t2732, 1);
#line 936 "compiler/main.gem"
    GemVal _t2734 = gem_table_new();
    GemVal _t2735[] = {gem_v_lc};
    GemVal _t2736 = gem_v_rc_r;
    GemVal _t2737[] = {gem_table_get(_t2736, gem_string("expr"))};
    gem_table_set(_t2734, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2735, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2737, 1)), gem_string(")")));
    GemVal _t2738 = gem_v_rc_r;
    gem_table_set(_t2734, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2738, gem_string("setup"))));
        GemVal _t2739 = _t2734;
        gem_pop_frame();
        return _t2739;
    } else {
#line 937 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 938 "compiler/main.gem"
    GemVal _t2740 = gem_v_node;
    GemVal _t2741[] = {gem_table_get(_t2740, gem_string("right"))};
    GemVal _t2742 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2742.fn(_t2742.env, _t2741, 1);
#line 939 "compiler/main.gem"
    GemVal _t2743 = gem_table_new();
    GemVal _t2744[] = {gem_v_lc};
    GemVal _t2745 = gem_v_rc_r;
    GemVal _t2746[] = {gem_table_get(_t2745, gem_string("expr"))};
    gem_table_set(_t2743, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2744, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2746, 1)), gem_string(")")));
    GemVal _t2747 = gem_v_rc_r;
    gem_table_set(_t2743, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2747, gem_string("setup"))));
            GemVal _t2748 = _t2743;
            gem_pop_frame();
            return _t2748;
        } else {
#line 940 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 941 "compiler/main.gem"
    GemVal _t2749 = gem_v_node;
    GemVal _t2750[] = {gem_table_get(_t2749, gem_string("right"))};
    GemVal _t2751 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2751.fn(_t2751.env, _t2750, 1);
#line 942 "compiler/main.gem"
    GemVal _t2752 = gem_table_new();
    GemVal _t2753[] = {gem_v_lc};
    GemVal _t2754 = gem_v_rc_r;
    GemVal _t2755[] = {gem_table_get(_t2754, gem_string("expr"))};
    gem_table_set(_t2752, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2753, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2755, 1)), gem_string(")")));
    GemVal _t2756 = gem_v_rc_r;
    gem_table_set(_t2752, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2756, gem_string("setup"))));
                GemVal _t2757 = _t2752;
                gem_pop_frame();
                return _t2757;
            } else {
#line 943 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 944 "compiler/main.gem"
    GemVal _t2758 = gem_v_node;
    GemVal _t2759[] = {gem_table_get(_t2758, gem_string("right"))};
    GemVal _t2760 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2760.fn(_t2760.env, _t2759, 1);
#line 945 "compiler/main.gem"
    GemVal _t2761 = gem_table_new();
    GemVal _t2762[] = {gem_v_lc};
    GemVal _t2763 = gem_v_rc_r;
    GemVal _t2764[] = {gem_table_get(_t2763, gem_string("expr"))};
    gem_table_set(_t2761, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2762, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2764, 1)), gem_string(")")));
    GemVal _t2765 = gem_v_rc_r;
    gem_table_set(_t2761, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2765, gem_string("setup"))));
                    GemVal _t2766 = _t2761;
                    gem_pop_frame();
                    return _t2766;
                } else {
#line 946 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 947 "compiler/main.gem"
    GemVal _t2767 = gem_v_node;
    GemVal _t2768[] = {gem_table_get(_t2767, gem_string("right"))};
    GemVal _t2769 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2769.fn(_t2769.env, _t2768, 1);
#line 948 "compiler/main.gem"
    GemVal _t2770 = gem_table_new();
    GemVal _t2771[] = {gem_v_lc};
    GemVal _t2772 = gem_v_rc_r;
    GemVal _t2773[] = {gem_table_get(_t2772, gem_string("expr"))};
    gem_table_set(_t2770, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2771, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2773, 1)), gem_string(")")));
    GemVal _t2774 = gem_v_rc_r;
    gem_table_set(_t2770, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2774, gem_string("setup"))));
                        GemVal _t2775 = _t2770;
                        gem_pop_frame();
                        return _t2775;
                    } else {
#line 949 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 950 "compiler/main.gem"
    GemVal _t2776 = gem_v_node;
    GemVal _t2777[] = {gem_table_get(_t2776, gem_string("right"))};
    GemVal _t2778 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2778.fn(_t2778.env, _t2777, 1);
#line 951 "compiler/main.gem"
    GemVal _t2779 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2779.fn(_t2779.env, NULL, 0);
#line 952 "compiler/main.gem"
    GemVal _t2780 = gem_v_rc_r;
    GemVal _t2781[] = {gem_v_t};
    GemVal _t2782 = gem_v_rc_r;
    GemVal _t2783[] = {gem_table_get(_t2782, gem_string("expr"))};
    GemVal _t2784[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2780, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2781, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2783, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2784, 1)), gem_string("};\n")));
#line 953 "compiler/main.gem"
    GemVal _t2785 = gem_table_new();
    GemVal _t2786[] = {gem_v_t};
    gem_table_set(_t2785, gem_string("expr"), gem_add(gem_add(gem_string("gem_in_fn(NULL, "), gem_to_string_fn(NULL, _t2786, 1)), gem_string(", 2)")));
    gem_table_set(_t2785, gem_string("setup"), gem_v_setup);
                            GemVal _t2787 = _t2785;
                            gem_pop_frame();
                            return _t2787;
                        } else {
#line 954 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 955 "compiler/main.gem"
    GemVal _t2788 = gem_v_node;
    GemVal _t2789[] = {gem_table_get(_t2788, gem_string("right"))};
    GemVal _t2790 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2790.fn(_t2790.env, _t2789, 1);
#line 956 "compiler/main.gem"
    GemVal _t2791 = gem_table_new();
    GemVal _t2792[] = {gem_v_lc};
    GemVal _t2793 = gem_v_rc_r;
    GemVal _t2794[] = {gem_table_get(_t2793, gem_string("expr"))};
    gem_table_set(_t2791, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2792, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2794, 1)), gem_string(")")));
    GemVal _t2795 = gem_v_rc_r;
    gem_table_set(_t2791, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2795, gem_string("setup"))));
                                GemVal _t2796 = _t2791;
                                gem_pop_frame();
                                return _t2796;
                            } else {
#line 957 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 958 "compiler/main.gem"
    GemVal _t2797 = gem_v_node;
    GemVal _t2798[] = {gem_table_get(_t2797, gem_string("right"))};
    GemVal _t2799 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2799.fn(_t2799.env, _t2798, 1);
#line 959 "compiler/main.gem"
    GemVal _t2800 = gem_table_new();
    GemVal _t2801[] = {gem_v_lc};
    GemVal _t2802 = gem_v_rc_r;
    GemVal _t2803[] = {gem_table_get(_t2802, gem_string("expr"))};
    gem_table_set(_t2800, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t2801, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2803, 1)), gem_string(")")));
    GemVal _t2804 = gem_v_rc_r;
    gem_table_set(_t2800, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2804, gem_string("setup"))));
                                    GemVal _t2805 = _t2800;
                                    gem_pop_frame();
                                    return _t2805;
                                } else {
#line 960 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 961 "compiler/main.gem"
    GemVal _t2806 = gem_v_node;
    GemVal _t2807[] = {gem_table_get(_t2806, gem_string("right"))};
    GemVal _t2808 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2808.fn(_t2808.env, _t2807, 1);
#line 962 "compiler/main.gem"
    GemVal _t2809 = gem_table_new();
    GemVal _t2810[] = {gem_v_lc};
    GemVal _t2811 = gem_v_rc_r;
    GemVal _t2812[] = {gem_table_get(_t2811, gem_string("expr"))};
    gem_table_set(_t2809, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t2810, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2812, 1)), gem_string(")")));
    GemVal _t2813 = gem_v_rc_r;
    gem_table_set(_t2809, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2813, gem_string("setup"))));
                                        GemVal _t2814 = _t2809;
                                        gem_pop_frame();
                                        return _t2814;
                                    } else {
#line 963 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 964 "compiler/main.gem"
    GemVal _t2815 = gem_v_node;
    GemVal _t2816[] = {gem_table_get(_t2815, gem_string("right"))};
    GemVal _t2817 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2817.fn(_t2817.env, _t2816, 1);
#line 965 "compiler/main.gem"
    GemVal _t2818 = gem_table_new();
    GemVal _t2819[] = {gem_v_lc};
    GemVal _t2820 = gem_v_rc_r;
    GemVal _t2821[] = {gem_table_get(_t2820, gem_string("expr"))};
    gem_table_set(_t2818, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t2819, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2821, 1)), gem_string(")")));
    GemVal _t2822 = gem_v_rc_r;
    gem_table_set(_t2818, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2822, gem_string("setup"))));
                                            GemVal _t2823 = _t2818;
                                            gem_pop_frame();
                                            return _t2823;
                                        } else {
#line 966 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 967 "compiler/main.gem"
    GemVal _t2824 = gem_v_node;
    GemVal _t2825[] = {gem_table_get(_t2824, gem_string("right"))};
    GemVal _t2826 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2826.fn(_t2826.env, _t2825, 1);
#line 968 "compiler/main.gem"
    GemVal _t2827 = gem_table_new();
    GemVal _t2828[] = {gem_v_lc};
    GemVal _t2829 = gem_v_rc_r;
    GemVal _t2830[] = {gem_table_get(_t2829, gem_string("expr"))};
    gem_table_set(_t2827, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t2828, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2830, 1)), gem_string(")")));
    GemVal _t2831 = gem_v_rc_r;
    gem_table_set(_t2827, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2831, gem_string("setup"))));
                                                GemVal _t2832 = _t2827;
                                                gem_pop_frame();
                                                return _t2832;
                                            } else {
#line 969 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 970 "compiler/main.gem"
    GemVal _t2833 = gem_v_node;
    GemVal _t2834[] = {gem_table_get(_t2833, gem_string("right"))};
    GemVal _t2835 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t2835.fn(_t2835.env, _t2834, 1);
#line 971 "compiler/main.gem"
    GemVal _t2836 = gem_table_new();
    GemVal _t2837[] = {gem_v_lc};
    GemVal _t2838 = gem_v_rc_r;
    GemVal _t2839[] = {gem_table_get(_t2838, gem_string("expr"))};
    gem_table_set(_t2836, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t2837, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2839, 1)), gem_string(")")));
    GemVal _t2840 = gem_v_rc_r;
    gem_table_set(_t2836, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2840, gem_string("setup"))));
                                                    GemVal _t2841 = _t2836;
                                                    gem_pop_frame();
                                                    return _t2841;
                                                } else {
#line 972 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 973 "compiler/main.gem"
    GemVal _t2842 = gem_v_node;
    GemVal _t2843[] = {gem_table_get(_t2842, gem_string("right"))};
    GemVal _t2844 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t2844.fn(_t2844.env, _t2843, 1);
#line 974 "compiler/main.gem"
    GemVal _t2845 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t2845.fn(_t2845.env, NULL, 0);
#line 975 "compiler/main.gem"
                                                        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 976 "compiler/main.gem"
    GemVal _t2846[] = {gem_v_b, gem_v_ls};
                                                        (void)(gem_buf_push_fn(NULL, _t2846, 2));
#line 977 "compiler/main.gem"
    GemVal _t2847[] = {gem_v_t};
    GemVal _t2848[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2847, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t2848, 2));
#line 978 "compiler/main.gem"
    GemVal _t2849[] = {gem_v_lc};
    GemVal _t2850[] = {gem_v_b, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t2849, 1)), gem_string(")) {\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t2850, 2));
#line 979 "compiler/main.gem"
    GemVal _t2851[] = {gem_v_t};
    GemVal _t2852[] = {gem_v_lc};
    GemVal _t2853[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2851, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2852, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t2853, 2));
#line 980 "compiler/main.gem"
    GemVal _t2854[] = {gem_v_b, gem_string("    } else {\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t2854, 2));
#line 981 "compiler/main.gem"
    GemVal _t2855 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t2855, gem_string("setup")), gem_string("")))) {
#line 982 "compiler/main.gem"
    GemVal _t2856 = gem_v_rc_r;
    GemVal _t2857[] = {gem_table_get(_t2856, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t2858[] = {gem_v_b, gem_str_replace_fn(NULL, _t2857, 3)};
                                                            (void)(gem_buf_push_fn(NULL, _t2858, 2));
                                                        }
#line 984 "compiler/main.gem"
    GemVal _t2859[] = {gem_v_t};
    GemVal _t2860 = gem_v_rc_r;
    GemVal _t2861[] = {gem_table_get(_t2860, gem_string("expr"))};
    GemVal _t2862[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2859, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2861, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t2862, 2));
#line 985 "compiler/main.gem"
    GemVal _t2863[] = {gem_v_b, gem_string("    }\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t2863, 2));
#line 986 "compiler/main.gem"
    GemVal _t2864 = gem_table_new();
    gem_table_set(_t2864, gem_string("expr"), gem_v_t);
    GemVal _t2865[] = {gem_v_b};
    gem_table_set(_t2864, gem_string("setup"), gem_buf_str_fn(NULL, _t2865, 1));
                                                        GemVal _t2866 = _t2864;
                                                        gem_pop_frame();
                                                        return _t2866;
                                                    } else {
#line 987 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 988 "compiler/main.gem"
    GemVal _t2867 = gem_v_node;
    GemVal _t2868[] = {gem_table_get(_t2867, gem_string("right"))};
    GemVal _t2869 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t2869.fn(_t2869.env, _t2868, 1);
#line 989 "compiler/main.gem"
    GemVal _t2870 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t2870.fn(_t2870.env, NULL, 0);
#line 990 "compiler/main.gem"
                                                            GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 991 "compiler/main.gem"
    GemVal _t2871[] = {gem_v_b, gem_v_ls};
                                                            (void)(gem_buf_push_fn(NULL, _t2871, 2));
#line 992 "compiler/main.gem"
    GemVal _t2872[] = {gem_v_t};
    GemVal _t2873[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2872, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t2873, 2));
#line 993 "compiler/main.gem"
    GemVal _t2874[] = {gem_v_lc};
    GemVal _t2875[] = {gem_v_b, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t2874, 1)), gem_string(")) {\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t2875, 2));
#line 994 "compiler/main.gem"
    GemVal _t2876[] = {gem_v_t};
    GemVal _t2877[] = {gem_v_lc};
    GemVal _t2878[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2876, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2877, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t2878, 2));
#line 995 "compiler/main.gem"
    GemVal _t2879[] = {gem_v_b, gem_string("    } else {\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t2879, 2));
#line 996 "compiler/main.gem"
    GemVal _t2880 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2880, gem_string("setup")), gem_string("")))) {
#line 997 "compiler/main.gem"
    GemVal _t2881 = gem_v_rc_r;
    GemVal _t2882[] = {gem_table_get(_t2881, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t2883[] = {gem_v_b, gem_str_replace_fn(NULL, _t2882, 3)};
                                                                (void)(gem_buf_push_fn(NULL, _t2883, 2));
                                                            }
#line 999 "compiler/main.gem"
    GemVal _t2884[] = {gem_v_t};
    GemVal _t2885 = gem_v_rc_r;
    GemVal _t2886[] = {gem_table_get(_t2885, gem_string("expr"))};
    GemVal _t2887[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2884, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2886, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t2887, 2));
#line 1000 "compiler/main.gem"
    GemVal _t2888[] = {gem_v_b, gem_string("    }\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t2888, 2));
#line 1001 "compiler/main.gem"
    GemVal _t2889 = gem_table_new();
    gem_table_set(_t2889, gem_string("expr"), gem_v_t);
    GemVal _t2890[] = {gem_v_b};
    gem_table_set(_t2889, gem_string("setup"), gem_buf_str_fn(NULL, _t2890, 1));
                                                            GemVal _t2891 = _t2889;
                                                            gem_pop_frame();
                                                            return _t2891;
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
#line 1004 "compiler/main.gem"
    GemVal _t2892[] = {gem_v_op};
    GemVal _t2893[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t2892, 1))};
    GemVal _t2894 = gem_error_at_fn("compiler/main.gem", 1004, _t2893, 1);
    gem_pop_frame();
    return _t2894;
}

struct _closure__anon_43 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_43(void *_env, GemVal *args, int argc) {
    struct _closure__anon_43 *_cls = (struct _closure__anon_43 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_43", "compiler/main.gem", 0);
#line 1010 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1011 "compiler/main.gem"
    GemVal gem_v__for_items_55 = gem_v_stmts;
#line 1011 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 1011 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2896[] = {gem_v__for_items_55};
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t2896, 1)))) break;
#line 1011 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1011 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1012 "compiler/main.gem"
    GemVal _t2897[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2898 = (*gem_v_compile_stmt);
    GemVal _t2899[] = {gem_v_b, _t2898.fn(_t2898.env, _t2897, 2)};
        (void)(gem_buf_push_fn(NULL, _t2899, 2));
#line 1013 "compiler/main.gem"
    GemVal _t2900[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t2900, 2));
    }

#line 1015 "compiler/main.gem"
    GemVal _t2901[] = {gem_v_b};
    GemVal _t2902 = gem_buf_str_fn(NULL, _t2901, 1);
    gem_pop_frame();
    return _t2902;
}

struct _closure__anon_44 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 1019 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_return_pos))) {
#line 1020 "compiler/main.gem"
    GemVal _t2904[] = {gem_v_stmts, gem_v_indent};
    GemVal _t2905 = (*gem_v_compile_stmts);
        GemVal _t2906 = _t2905.fn(_t2905.env, _t2904, 2);
        gem_pop_frame();
        return _t2906;
    }
#line 1022 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1023 "compiler/main.gem"
    GemVal _t2907[] = {gem_v_stmts};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t2907, 1), gem_int(0)))) {
#line 1024 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1024 "compiler/main.gem"
    GemVal _t2908[] = {gem_v_stmts};
        GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t2908, 1), gem_int(1));
#line 1024 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1024 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_56;
#line 1024 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1025 "compiler/main.gem"
    GemVal _t2909[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_indent};
    GemVal _t2910 = (*gem_v_compile_stmt);
    GemVal _t2911[] = {gem_v_b, _t2910.fn(_t2910.env, _t2909, 2)};
            (void)(gem_buf_push_fn(NULL, _t2911, 2));
#line 1026 "compiler/main.gem"
    GemVal _t2912[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t2912, 2));
        }

#line 1028 "compiler/main.gem"
    GemVal _t2913[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t2914 = (*gem_v_compile_stmt_return);
    GemVal _t2915[] = {gem_v_b, _t2914.fn(_t2914.env, _t2913, 2)};
        (void)(gem_buf_push_fn(NULL, _t2915, 2));
#line 1029 "compiler/main.gem"
    GemVal _t2916[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t2916, 2));
    } else {
#line 1031 "compiler/main.gem"
    GemVal _t2917[] = {gem_v_indent};
        GemVal gem_v_p = gem_fn_pad_for(NULL, _t2917, 1);
#line 1032 "compiler/main.gem"
    GemVal _t2918[] = {gem_v_p};
    GemVal _t2919[] = {gem_v_p};
    GemVal _t2920[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2918, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2919, 1)), gem_string("return GEM_NIL;\n"))};
        (void)(gem_buf_push_fn(NULL, _t2920, 2));
    }
#line 1034 "compiler/main.gem"
    GemVal _t2921[] = {gem_v_b};
    GemVal _t2922 = gem_buf_str_fn(NULL, _t2921, 1);
    gem_pop_frame();
    return _t2922;
}

struct _closure__anon_45 {
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
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
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
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 1040 "compiler/main.gem"
    GemVal _t2924[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2924, 1);
#line 1041 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1042 "compiler/main.gem"
    GemVal _t2925 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2925, gem_string("line")), GEM_NIL))) {
#line 1043 "compiler/main.gem"
    GemVal _t2926 = gem_v_node;
    GemVal _t2927[] = {gem_table_get(_t2926, gem_string("line"))};
    GemVal _t2928[] = {(*gem_v_source_name)};
    GemVal _t2929[] = {gem_fn_escape_c_string(NULL, _t2928, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2927, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2929, 1)), gem_string("\"\n"));
    }
#line 1046 "compiler/main.gem"
    GemVal gem_v__match_57 = gem_v_node;
    GemVal _t2930[] = {gem_v__match_57};
    GemVal _t2932;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2930, 1), gem_string("table")))) {
        _t2932 = gem_eq(gem_type_fn(NULL, _t2930, 1), gem_string("table"));
    } else {
        GemVal _t2931[] = {gem_v__match_57, gem_string("tag")};
        _t2932 = gem_has_key_fn(NULL, _t2931, 2);
    }
    GemVal _t2933;
    if (!gem_truthy(_t2932)) {
        _t2933 = _t2932;
    } else {
        _t2933 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2935;
    if (!gem_truthy(_t2933)) {
        _t2935 = _t2933;
    } else {
        GemVal _t2934[] = {gem_v__match_57, gem_string("name")};
        _t2935 = gem_has_key_fn(NULL, _t2934, 2);
    }
    GemVal _t2937;
    if (!gem_truthy(_t2935)) {
        _t2937 = _t2935;
    } else {
        GemVal _t2936[] = {gem_v__match_57, gem_string("value")};
        _t2937 = gem_has_key_fn(NULL, _t2936, 2);
    }
    if (gem_truthy(_t2937)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1048 "compiler/main.gem"
    GemVal _t2938[] = {gem_v_value};
    GemVal _t2939 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2939.fn(_t2939.env, _t2938, 1);
#line 1049 "compiler/main.gem"
    GemVal _t2940[] = {gem_v_name};
    GemVal _t2941 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2941.fn(_t2941.env, _t2940, 1);
#line 1050 "compiler/main.gem"
    GemVal _t2943;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2943 = (*gem_v_in_top_level);
    } else {
        GemVal _t2942[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2943 = gem_fn_set_contains(NULL, _t2942, 2);
    }
        if (gem_truthy(_t2943)) {
#line 1051 "compiler/main.gem"
    GemVal _t2944[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2944, 2))) {
#line 1052 "compiler/main.gem"
    GemVal _t2945[] = {gem_v_line_dir};
    GemVal _t2946 = gem_v_r;
    GemVal _t2947[] = {gem_table_get(_t2946, gem_string("setup"))};
    GemVal _t2948[] = {gem_v_p};
    GemVal _t2949[] = {gem_v_mname};
    GemVal _t2950[] = {gem_v_p};
    GemVal _t2951[] = {gem_v_mname};
    GemVal _t2952 = gem_v_r;
    GemVal _t2953[] = {gem_table_get(_t2952, gem_string("expr"))};
                GemVal _t2954 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2945, 1), gem_to_string_fn(NULL, _t2947, 1)), gem_to_string_fn(NULL, _t2948, 1)), gem_to_string_fn(NULL, _t2949, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2950, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2951, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2953, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2954;
            }
#line 1054 "compiler/main.gem"
    GemVal _t2955[] = {gem_v_line_dir};
    GemVal _t2956 = gem_v_r;
    GemVal _t2957[] = {gem_table_get(_t2956, gem_string("setup"))};
    GemVal _t2958[] = {gem_v_p};
    GemVal _t2959[] = {gem_v_mname};
    GemVal _t2960 = gem_v_r;
    GemVal _t2961[] = {gem_table_get(_t2960, gem_string("expr"))};
            GemVal _t2962 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2955, 1), gem_to_string_fn(NULL, _t2957, 1)), gem_to_string_fn(NULL, _t2958, 1)), gem_to_string_fn(NULL, _t2959, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2961, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2962;
        }
#line 1056 "compiler/main.gem"
    GemVal _t2963[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2963, 2))) {
#line 1057 "compiler/main.gem"
    GemVal _t2964[] = {gem_v_line_dir};
    GemVal _t2965 = gem_v_r;
    GemVal _t2966[] = {gem_table_get(_t2965, gem_string("setup"))};
    GemVal _t2967[] = {gem_v_p};
    GemVal _t2968[] = {gem_v_mname};
    GemVal _t2969[] = {gem_v_p};
    GemVal _t2970[] = {gem_v_mname};
    GemVal _t2971 = gem_v_r;
    GemVal _t2972[] = {gem_table_get(_t2971, gem_string("expr"))};
            GemVal _t2973 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2964, 1), gem_to_string_fn(NULL, _t2966, 1)), gem_to_string_fn(NULL, _t2967, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2968, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2969, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2970, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2972, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2973;
        }
#line 1059 "compiler/main.gem"
    GemVal _t2974[] = {gem_v_line_dir};
    GemVal _t2975 = gem_v_r;
    GemVal _t2976[] = {gem_table_get(_t2975, gem_string("setup"))};
    GemVal _t2977[] = {gem_v_p};
    GemVal _t2978[] = {gem_v_mname};
    GemVal _t2979 = gem_v_r;
    GemVal _t2980[] = {gem_table_get(_t2979, gem_string("expr"))};
        GemVal _t2981 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2974, 1), gem_to_string_fn(NULL, _t2976, 1)), gem_to_string_fn(NULL, _t2977, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2978, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2980, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2981;
    } else {
    GemVal _t2982[] = {gem_v__match_57};
    GemVal _t2984;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2982, 1), gem_string("table")))) {
        _t2984 = gem_eq(gem_type_fn(NULL, _t2982, 1), gem_string("table"));
    } else {
        GemVal _t2983[] = {gem_v__match_57, gem_string("tag")};
        _t2984 = gem_has_key_fn(NULL, _t2983, 2);
    }
    GemVal _t2985;
    if (!gem_truthy(_t2984)) {
        _t2985 = _t2984;
    } else {
        _t2985 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2987;
    if (!gem_truthy(_t2985)) {
        _t2987 = _t2985;
    } else {
        GemVal _t2986[] = {gem_v__match_57, gem_string("name")};
        _t2987 = gem_has_key_fn(NULL, _t2986, 2);
    }
    GemVal _t2989;
    if (!gem_truthy(_t2987)) {
        _t2989 = _t2987;
    } else {
        GemVal _t2988[] = {gem_v__match_57, gem_string("value")};
        _t2989 = gem_has_key_fn(NULL, _t2988, 2);
    }
    if (gem_truthy(_t2989)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1061 "compiler/main.gem"
    GemVal _t2990[] = {gem_v_value};
    GemVal _t2991 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2991.fn(_t2991.env, _t2990, 1);
#line 1062 "compiler/main.gem"
    GemVal _t2992[] = {gem_v_name};
    GemVal _t2993 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2993.fn(_t2993.env, _t2992, 1);
#line 1063 "compiler/main.gem"
    GemVal _t2994[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2996;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2994, 2))) {
        _t2996 = gem_fn_set_contains(NULL, _t2994, 2);
    } else {
        GemVal _t2995[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2996 = gem_fn_set_contains(NULL, _t2995, 2);
    }
        if (gem_truthy(_t2996)) {
#line 1064 "compiler/main.gem"
    GemVal _t2997[] = {gem_v_line_dir};
    GemVal _t2998 = gem_v_r;
    GemVal _t2999[] = {gem_table_get(_t2998, gem_string("setup"))};
    GemVal _t3000[] = {gem_v_p};
    GemVal _t3001[] = {gem_v_mname};
    GemVal _t3002 = gem_v_r;
    GemVal _t3003[] = {gem_table_get(_t3002, gem_string("expr"))};
            GemVal _t3004 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2997, 1), gem_to_string_fn(NULL, _t2999, 1)), gem_to_string_fn(NULL, _t3000, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3001, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3003, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3004;
        }
#line 1066 "compiler/main.gem"
    GemVal _t3005[] = {gem_v_line_dir};
    GemVal _t3006 = gem_v_r;
    GemVal _t3007[] = {gem_table_get(_t3006, gem_string("setup"))};
    GemVal _t3008[] = {gem_v_p};
    GemVal _t3009[] = {gem_v_mname};
    GemVal _t3010 = gem_v_r;
    GemVal _t3011[] = {gem_table_get(_t3010, gem_string("expr"))};
        GemVal _t3012 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3005, 1), gem_to_string_fn(NULL, _t3007, 1)), gem_to_string_fn(NULL, _t3008, 1)), gem_to_string_fn(NULL, _t3009, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3011, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3012;
    } else {
    GemVal _t3013[] = {gem_v__match_57};
    GemVal _t3015;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3013, 1), gem_string("table")))) {
        _t3015 = gem_eq(gem_type_fn(NULL, _t3013, 1), gem_string("table"));
    } else {
        GemVal _t3014[] = {gem_v__match_57, gem_string("tag")};
        _t3015 = gem_has_key_fn(NULL, _t3014, 2);
    }
    GemVal _t3016;
    if (!gem_truthy(_t3015)) {
        _t3016 = _t3015;
    } else {
        _t3016 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t3018;
    if (!gem_truthy(_t3016)) {
        _t3018 = _t3016;
    } else {
        GemVal _t3017[] = {gem_v__match_57, gem_string("object")};
        _t3018 = gem_has_key_fn(NULL, _t3017, 2);
    }
    GemVal _t3020;
    if (!gem_truthy(_t3018)) {
        _t3020 = _t3018;
    } else {
        GemVal _t3019[] = {gem_v__match_57, gem_string("value")};
        _t3020 = gem_has_key_fn(NULL, _t3019, 2);
    }
    GemVal _t3022;
    if (!gem_truthy(_t3020)) {
        _t3022 = _t3020;
    } else {
        GemVal _t3021[] = {gem_v__match_57, gem_string("field")};
        _t3022 = gem_has_key_fn(NULL, _t3021, 2);
    }
    if (gem_truthy(_t3022)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_57, gem_string("field"));
#line 1068 "compiler/main.gem"
    GemVal _t3023[] = {gem_v_object};
    GemVal _t3024 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3024.fn(_t3024.env, _t3023, 1);
#line 1069 "compiler/main.gem"
    GemVal _t3025[] = {gem_v_value};
    GemVal _t3026 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3026.fn(_t3026.env, _t3025, 1);
#line 1070 "compiler/main.gem"
    GemVal _t3027[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t3027, 1);
#line 1071 "compiler/main.gem"
    GemVal _t3028[] = {gem_v_line_dir};
    GemVal _t3029 = gem_v_obj_r;
    GemVal _t3030[] = {gem_table_get(_t3029, gem_string("setup"))};
    GemVal _t3031 = gem_v_val_r;
    GemVal _t3032[] = {gem_table_get(_t3031, gem_string("setup"))};
    GemVal _t3033[] = {gem_v_p};
    GemVal _t3034 = gem_v_obj_r;
    GemVal _t3035[] = {gem_table_get(_t3034, gem_string("expr"))};
    GemVal _t3036[] = {gem_v_escaped};
    GemVal _t3037 = gem_v_val_r;
    GemVal _t3038[] = {gem_table_get(_t3037, gem_string("expr"))};
        GemVal _t3039 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3028, 1), gem_to_string_fn(NULL, _t3030, 1)), gem_to_string_fn(NULL, _t3032, 1)), gem_to_string_fn(NULL, _t3033, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t3035, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t3036, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t3038, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3039;
    } else {
    GemVal _t3040[] = {gem_v__match_57};
    GemVal _t3042;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3040, 1), gem_string("table")))) {
        _t3042 = gem_eq(gem_type_fn(NULL, _t3040, 1), gem_string("table"));
    } else {
        GemVal _t3041[] = {gem_v__match_57, gem_string("tag")};
        _t3042 = gem_has_key_fn(NULL, _t3041, 2);
    }
    GemVal _t3043;
    if (!gem_truthy(_t3042)) {
        _t3043 = _t3042;
    } else {
        _t3043 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t3045;
    if (!gem_truthy(_t3043)) {
        _t3045 = _t3043;
    } else {
        GemVal _t3044[] = {gem_v__match_57, gem_string("object")};
        _t3045 = gem_has_key_fn(NULL, _t3044, 2);
    }
    GemVal _t3047;
    if (!gem_truthy(_t3045)) {
        _t3047 = _t3045;
    } else {
        GemVal _t3046[] = {gem_v__match_57, gem_string("key")};
        _t3047 = gem_has_key_fn(NULL, _t3046, 2);
    }
    GemVal _t3049;
    if (!gem_truthy(_t3047)) {
        _t3049 = _t3047;
    } else {
        GemVal _t3048[] = {gem_v__match_57, gem_string("value")};
        _t3049 = gem_has_key_fn(NULL, _t3048, 2);
    }
    if (gem_truthy(_t3049)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_57, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1073 "compiler/main.gem"
    GemVal _t3050[] = {gem_v_object};
    GemVal _t3051 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3051.fn(_t3051.env, _t3050, 1);
#line 1074 "compiler/main.gem"
    GemVal _t3052[] = {gem_v_key};
    GemVal _t3053 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t3053.fn(_t3053.env, _t3052, 1);
#line 1075 "compiler/main.gem"
    GemVal _t3054[] = {gem_v_value};
    GemVal _t3055 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3055.fn(_t3055.env, _t3054, 1);
#line 1076 "compiler/main.gem"
    GemVal _t3056[] = {gem_v_line_dir};
    GemVal _t3057 = gem_v_obj_r;
    GemVal _t3058[] = {gem_table_get(_t3057, gem_string("setup"))};
    GemVal _t3059 = gem_v_key_r;
    GemVal _t3060[] = {gem_table_get(_t3059, gem_string("setup"))};
    GemVal _t3061 = gem_v_val_r;
    GemVal _t3062[] = {gem_table_get(_t3061, gem_string("setup"))};
    GemVal _t3063[] = {gem_v_p};
    GemVal _t3064 = gem_v_obj_r;
    GemVal _t3065[] = {gem_table_get(_t3064, gem_string("expr"))};
    GemVal _t3066 = gem_v_key_r;
    GemVal _t3067[] = {gem_table_get(_t3066, gem_string("expr"))};
    GemVal _t3068 = gem_v_val_r;
    GemVal _t3069[] = {gem_table_get(_t3068, gem_string("expr"))};
        GemVal _t3070 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3056, 1), gem_to_string_fn(NULL, _t3058, 1)), gem_to_string_fn(NULL, _t3060, 1)), gem_to_string_fn(NULL, _t3062, 1)), gem_to_string_fn(NULL, _t3063, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t3065, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3067, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3069, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3070;
    } else {
    GemVal _t3071[] = {gem_v__match_57};
    GemVal _t3073;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3071, 1), gem_string("table")))) {
        _t3073 = gem_eq(gem_type_fn(NULL, _t3071, 1), gem_string("table"));
    } else {
        GemVal _t3072[] = {gem_v__match_57, gem_string("tag")};
        _t3073 = gem_has_key_fn(NULL, _t3072, 2);
    }
    GemVal _t3074;
    if (!gem_truthy(_t3073)) {
        _t3074 = _t3073;
    } else {
        _t3074 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3074)) {
#line 1078 "compiler/main.gem"
    GemVal _t3075[] = {gem_v_line_dir};
    GemVal _t3076[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3077 = (*gem_v_compile_if);
        GemVal _t3078 = gem_add(gem_to_string_fn(NULL, _t3075, 1), _t3077.fn(_t3077.env, _t3076, 3));
        gem_pop_frame();
        return _t3078;
    } else {
    GemVal _t3079[] = {gem_v__match_57};
    GemVal _t3081;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3079, 1), gem_string("table")))) {
        _t3081 = gem_eq(gem_type_fn(NULL, _t3079, 1), gem_string("table"));
    } else {
        GemVal _t3080[] = {gem_v__match_57, gem_string("tag")};
        _t3081 = gem_has_key_fn(NULL, _t3080, 2);
    }
    GemVal _t3082;
    if (!gem_truthy(_t3081)) {
        _t3082 = _t3081;
    } else {
        _t3082 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3082)) {
#line 1080 "compiler/main.gem"
    GemVal _t3083[] = {gem_v_line_dir};
    GemVal _t3084[] = {gem_v_node, gem_v_indent};
    GemVal _t3085 = (*gem_v_compile_while);
        GemVal _t3086 = gem_add(gem_to_string_fn(NULL, _t3083, 1), _t3085.fn(_t3085.env, _t3084, 2));
        gem_pop_frame();
        return _t3086;
    } else {
    GemVal _t3087[] = {gem_v__match_57};
    GemVal _t3089;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3087, 1), gem_string("table")))) {
        _t3089 = gem_eq(gem_type_fn(NULL, _t3087, 1), gem_string("table"));
    } else {
        GemVal _t3088[] = {gem_v__match_57, gem_string("tag")};
        _t3089 = gem_has_key_fn(NULL, _t3088, 2);
    }
    GemVal _t3090;
    if (!gem_truthy(_t3089)) {
        _t3090 = _t3089;
    } else {
        _t3090 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3090)) {
#line 1082 "compiler/main.gem"
    GemVal _t3091[] = {gem_v_line_dir};
    GemVal _t3092[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3093 = (*gem_v_compile_match);
        GemVal _t3094 = gem_add(gem_to_string_fn(NULL, _t3091, 1), _t3093.fn(_t3093.env, _t3092, 3));
        gem_pop_frame();
        return _t3094;
    } else {
    GemVal _t3095[] = {gem_v__match_57};
    GemVal _t3097;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3095, 1), gem_string("table")))) {
        _t3097 = gem_eq(gem_type_fn(NULL, _t3095, 1), gem_string("table"));
    } else {
        GemVal _t3096[] = {gem_v__match_57, gem_string("tag")};
        _t3097 = gem_has_key_fn(NULL, _t3096, 2);
    }
    GemVal _t3098;
    if (!gem_truthy(_t3097)) {
        _t3098 = _t3097;
    } else {
        _t3098 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3098)) {
#line 1084 "compiler/main.gem"
    GemVal _t3099[] = {gem_v_line_dir};
    GemVal _t3100[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3101 = (*gem_v_compile_receive_match);
        GemVal _t3102 = gem_add(gem_to_string_fn(NULL, _t3099, 1), _t3101.fn(_t3101.env, _t3100, 3));
        gem_pop_frame();
        return _t3102;
    } else {
    GemVal _t3103[] = {gem_v__match_57};
    GemVal _t3105;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3103, 1), gem_string("table")))) {
        _t3105 = gem_eq(gem_type_fn(NULL, _t3103, 1), gem_string("table"));
    } else {
        GemVal _t3104[] = {gem_v__match_57, gem_string("tag")};
        _t3105 = gem_has_key_fn(NULL, _t3104, 2);
    }
    GemVal _t3106;
    if (!gem_truthy(_t3105)) {
        _t3106 = _t3105;
    } else {
        _t3106 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3106)) {
#line 1086 "compiler/main.gem"
        GemVal _t3107 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t3107;
    } else {
    GemVal _t3108[] = {gem_v__match_57};
    GemVal _t3110;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3108, 1), gem_string("table")))) {
        _t3110 = gem_eq(gem_type_fn(NULL, _t3108, 1), gem_string("table"));
    } else {
        GemVal _t3109[] = {gem_v__match_57, gem_string("tag")};
        _t3110 = gem_has_key_fn(NULL, _t3109, 2);
    }
    GemVal _t3111;
    if (!gem_truthy(_t3110)) {
        _t3111 = _t3110;
    } else {
        _t3111 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3111)) {
#line 1088 "compiler/main.gem"
        GemVal _t3112 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t3112;
    } else {
    GemVal _t3113[] = {gem_v__match_57};
    GemVal _t3115;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3113, 1), gem_string("table")))) {
        _t3115 = gem_eq(gem_type_fn(NULL, _t3113, 1), gem_string("table"));
    } else {
        GemVal _t3114[] = {gem_v__match_57, gem_string("tag")};
        _t3115 = gem_has_key_fn(NULL, _t3114, 2);
    }
    GemVal _t3116;
    if (!gem_truthy(_t3115)) {
        _t3116 = _t3115;
    } else {
        _t3116 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3118;
    if (!gem_truthy(_t3116)) {
        _t3118 = _t3116;
    } else {
        GemVal _t3117[] = {gem_v__match_57, gem_string("value")};
        _t3118 = gem_has_key_fn(NULL, _t3117, 2);
    }
    if (gem_truthy(_t3118)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1090 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1091 "compiler/main.gem"
    GemVal _t3119[] = {gem_v_value};
    GemVal _t3120 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3120.fn(_t3120.env, _t3119, 1);
#line 1092 "compiler/main.gem"
    GemVal _t3121 = (*gem_v_tmp);
            GemVal gem_v_t = _t3121.fn(_t3121.env, NULL, 0);
#line 1093 "compiler/main.gem"
    GemVal _t3122[] = {gem_v_line_dir};
    GemVal _t3123 = gem_v_r;
    GemVal _t3124[] = {gem_table_get(_t3123, gem_string("setup"))};
    GemVal _t3125[] = {gem_v_p};
    GemVal _t3126[] = {gem_v_t};
    GemVal _t3127 = gem_v_r;
    GemVal _t3128[] = {gem_table_get(_t3127, gem_string("expr"))};
    GemVal _t3129[] = {gem_v_p};
    GemVal _t3130[] = {gem_v_p};
    GemVal _t3131[] = {gem_v_t};
            GemVal _t3132 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3122, 1), gem_to_string_fn(NULL, _t3124, 1)), gem_to_string_fn(NULL, _t3125, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3126, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3128, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3129, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3130, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3131, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3132;
        }
#line 1095 "compiler/main.gem"
    GemVal _t3133[] = {gem_v_line_dir};
    GemVal _t3134[] = {gem_v_p};
    GemVal _t3135[] = {gem_v_p};
        GemVal _t3136 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3133, 1), gem_to_string_fn(NULL, _t3134, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3135, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3136;
    } else {
    GemVal _t3137[] = {gem_v__match_57};
    GemVal _t3139;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3137, 1), gem_string("table")))) {
        _t3139 = gem_eq(gem_type_fn(NULL, _t3137, 1), gem_string("table"));
    } else {
        GemVal _t3138[] = {gem_v__match_57, gem_string("tag")};
        _t3139 = gem_has_key_fn(NULL, _t3138, 2);
    }
    GemVal _t3140;
    if (!gem_truthy(_t3139)) {
        _t3140 = _t3139;
    } else {
        _t3140 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t3140)) {
#line 1097 "compiler/main.gem"
    GemVal _t3141[] = {gem_v_node};
    GemVal _t3142 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3142.fn(_t3142.env, _t3141, 1);
#line 1098 "compiler/main.gem"
    GemVal _t3143[] = {gem_v_line_dir};
    GemVal _t3144 = gem_v_r;
    GemVal _t3145[] = {gem_table_get(_t3144, gem_string("setup"))};
    GemVal _t3146[] = {gem_v_p};
    GemVal _t3147 = gem_v_r;
    GemVal _t3148[] = {gem_table_get(_t3147, gem_string("expr"))};
        GemVal _t3149 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3143, 1), gem_to_string_fn(NULL, _t3145, 1)), gem_to_string_fn(NULL, _t3146, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t3148, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3149;
    } else {
    GemVal _t3150[] = {gem_v__match_57};
    GemVal _t3152;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3150, 1), gem_string("table")))) {
        _t3152 = gem_eq(gem_type_fn(NULL, _t3150, 1), gem_string("table"));
    } else {
        GemVal _t3151[] = {gem_v__match_57, gem_string("tag")};
        _t3152 = gem_has_key_fn(NULL, _t3151, 2);
    }
    GemVal _t3153;
    if (!gem_truthy(_t3152)) {
        _t3153 = _t3152;
    } else {
        _t3153 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3153)) {
#line 1100 "compiler/main.gem"
        GemVal _t3154 = gem_string("");
        gem_pop_frame();
        return _t3154;
    } else {
    GemVal _t3155[] = {gem_v__match_57};
    GemVal _t3157;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3155, 1), gem_string("table")))) {
        _t3157 = gem_eq(gem_type_fn(NULL, _t3155, 1), gem_string("table"));
    } else {
        GemVal _t3156[] = {gem_v__match_57, gem_string("tag")};
        _t3157 = gem_has_key_fn(NULL, _t3156, 2);
    }
    GemVal _t3158;
    if (!gem_truthy(_t3157)) {
        _t3158 = _t3157;
    } else {
        _t3158 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3160;
    if (!gem_truthy(_t3158)) {
        _t3160 = _t3158;
    } else {
        GemVal _t3159[] = {gem_v__match_57, gem_string("stmts")};
        _t3160 = gem_has_key_fn(NULL, _t3159, 2);
    }
    if (gem_truthy(_t3160)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_57, gem_string("stmts"));
#line 1102 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1103 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_stmts;
#line 1103 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1103 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3161[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t3161, 1)))) break;
#line 1103 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1103 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1104 "compiler/main.gem"
    GemVal _t3162[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3163 = (*gem_v_compile_stmt);
    GemVal _t3164[] = {gem_v_b, _t3163.fn(_t3163.env, _t3162, 2)};
            (void)(gem_buf_push_fn(NULL, _t3164, 2));
#line 1105 "compiler/main.gem"
    GemVal _t3165[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3165, 2));
        }

#line 1107 "compiler/main.gem"
    GemVal _t3166[] = {gem_v_b};
        GemVal _t3167 = gem_buf_str_fn(NULL, _t3166, 1);
        gem_pop_frame();
        return _t3167;
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
#line 1110 "compiler/main.gem"
    GemVal _t3168[] = {gem_v_node};
    GemVal _t3169 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3169.fn(_t3169.env, _t3168, 1);
    GemVal _t3170[] = {gem_v_line_dir};
    GemVal _t3171 = gem_v_r;
    GemVal _t3172[] = {gem_table_get(_t3171, gem_string("setup"))};
    GemVal _t3173[] = {gem_v_p};
    GemVal _t3174 = gem_v_r;
    GemVal _t3175[] = {gem_table_get(_t3174, gem_string("expr"))};
    GemVal _t3176 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3170, 1), gem_to_string_fn(NULL, _t3172, 1)), gem_to_string_fn(NULL, _t3173, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t3175, 1)), gem_string(");"));
    gem_pop_frame();
    return _t3176;
}

struct _closure__anon_46 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if;
    GemVal *gem_v_compile_match;
    GemVal *gem_v_compile_receive_match;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_compile_while;
    GemVal *gem_v_emit_tco_continue;
    GemVal *gem_v_local_names;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tco_fn_name;
    GemVal *gem_v_tmp;
    GemVal *gem_v_top_level_boxed;
};
static GemVal _anon_46(void *_env, GemVal *args, int argc) {
    struct _closure__anon_46 *_cls = (struct _closure__anon_46 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_if = _cls->gem_v_compile_if;
    GemVal *gem_v_compile_match = _cls->gem_v_compile_match;
    GemVal *gem_v_compile_receive_match = _cls->gem_v_compile_receive_match;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_compile_while = _cls->gem_v_compile_while;
    GemVal *gem_v_emit_tco_continue = _cls->gem_v_emit_tco_continue;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tco_fn_name = _cls->gem_v_tco_fn_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 1117 "compiler/main.gem"
    GemVal _t3178[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3178, 1);
#line 1118 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1119 "compiler/main.gem"
    GemVal _t3179 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3179, gem_string("line")), GEM_NIL))) {
#line 1120 "compiler/main.gem"
    GemVal _t3180 = gem_v_node;
    GemVal _t3181[] = {gem_table_get(_t3180, gem_string("line"))};
    GemVal _t3182[] = {(*gem_v_source_name)};
    GemVal _t3183[] = {gem_fn_escape_c_string(NULL, _t3182, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3181, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3183, 1)), gem_string("\"\n"));
    }
#line 1123 "compiler/main.gem"
    if (gem_truthy(gem_neq((*gem_v_tco_fn_name), GEM_NIL))) {
#line 1124 "compiler/main.gem"
    GemVal _t3184[] = {gem_v_node, gem_string("call")};
    GemVal _t3187;
    if (!gem_truthy(gem_fn_is_node(NULL, _t3184, 2))) {
        _t3187 = gem_fn_is_node(NULL, _t3184, 2);
    } else {
        GemVal _t3185 = gem_v_node;
        GemVal _t3186[] = {gem_table_get(_t3185, gem_string("func")), gem_string("var")};
        _t3187 = gem_fn_is_node(NULL, _t3186, 2);
    }
    GemVal _t3190;
    if (!gem_truthy(_t3187)) {
        _t3190 = _t3187;
    } else {
        GemVal _t3188 = gem_v_node;
        GemVal _t3189 = gem_table_get(_t3188, gem_string("func"));
        _t3190 = gem_eq(gem_table_get(_t3189, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3192;
    if (!gem_truthy(_t3190)) {
        _t3192 = _t3190;
    } else {
        GemVal _t3191[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3192 = gem_not(gem_fn_set_contains(NULL, _t3191, 2));
    }
        if (gem_truthy(_t3192)) {
#line 1125 "compiler/main.gem"
    GemVal _t3193[] = {gem_v_node, gem_v_indent};
    GemVal _t3194 = (*gem_v_emit_tco_continue);
            GemVal _t3195 = gem_add(gem_v_line_dir, _t3194.fn(_t3194.env, _t3193, 2));
            gem_pop_frame();
            return _t3195;
        }
#line 1127 "compiler/main.gem"
    GemVal _t3196[] = {gem_v_node, gem_string("return")};
    GemVal _t3198;
    if (!gem_truthy(gem_fn_is_node(NULL, _t3196, 2))) {
        _t3198 = gem_fn_is_node(NULL, _t3196, 2);
    } else {
        GemVal _t3197 = gem_v_node;
        _t3198 = gem_neq(gem_table_get(_t3197, gem_string("value")), GEM_NIL);
    }
    GemVal _t3201;
    if (!gem_truthy(_t3198)) {
        _t3201 = _t3198;
    } else {
        GemVal _t3199 = gem_v_node;
        GemVal _t3200[] = {gem_table_get(_t3199, gem_string("value")), gem_string("call")};
        _t3201 = gem_fn_is_node(NULL, _t3200, 2);
    }
    GemVal _t3205;
    if (!gem_truthy(_t3201)) {
        _t3205 = _t3201;
    } else {
        GemVal _t3202 = gem_v_node;
        GemVal _t3203 = gem_table_get(_t3202, gem_string("value"));
        GemVal _t3204[] = {gem_table_get(_t3203, gem_string("func")), gem_string("var")};
        _t3205 = gem_fn_is_node(NULL, _t3204, 2);
    }
    GemVal _t3209;
    if (!gem_truthy(_t3205)) {
        _t3209 = _t3205;
    } else {
        GemVal _t3206 = gem_v_node;
        GemVal _t3207 = gem_table_get(_t3206, gem_string("value"));
        GemVal _t3208 = gem_table_get(_t3207, gem_string("func"));
        _t3209 = gem_eq(gem_table_get(_t3208, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3211;
    if (!gem_truthy(_t3209)) {
        _t3211 = _t3209;
    } else {
        GemVal _t3210[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3211 = gem_not(gem_fn_set_contains(NULL, _t3210, 2));
    }
        if (gem_truthy(_t3211)) {
#line 1128 "compiler/main.gem"
    GemVal _t3212 = gem_v_node;
    GemVal _t3213[] = {gem_table_get(_t3212, gem_string("value")), gem_v_indent};
    GemVal _t3214 = (*gem_v_emit_tco_continue);
            GemVal _t3215 = gem_add(gem_v_line_dir, _t3214.fn(_t3214.env, _t3213, 2));
            gem_pop_frame();
            return _t3215;
        }
    }
#line 1132 "compiler/main.gem"
    GemVal gem_v__match_59 = gem_v_node;
    GemVal _t3216[] = {gem_v__match_59};
    GemVal _t3218;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3216, 1), gem_string("table")))) {
        _t3218 = gem_eq(gem_type_fn(NULL, _t3216, 1), gem_string("table"));
    } else {
        GemVal _t3217[] = {gem_v__match_59, gem_string("tag")};
        _t3218 = gem_has_key_fn(NULL, _t3217, 2);
    }
    GemVal _t3219;
    if (!gem_truthy(_t3218)) {
        _t3219 = _t3218;
    } else {
        _t3219 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3221;
    if (!gem_truthy(_t3219)) {
        _t3221 = _t3219;
    } else {
        GemVal _t3220[] = {gem_v__match_59, gem_string("name")};
        _t3221 = gem_has_key_fn(NULL, _t3220, 2);
    }
    GemVal _t3223;
    if (!gem_truthy(_t3221)) {
        _t3223 = _t3221;
    } else {
        GemVal _t3222[] = {gem_v__match_59, gem_string("value")};
        _t3223 = gem_has_key_fn(NULL, _t3222, 2);
    }
    if (gem_truthy(_t3223)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1134 "compiler/main.gem"
    GemVal _t3224[] = {gem_v_value};
    GemVal _t3225 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3225.fn(_t3225.env, _t3224, 1);
#line 1135 "compiler/main.gem"
    GemVal _t3226[] = {gem_v_name};
    GemVal _t3227 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3227.fn(_t3227.env, _t3226, 1);
#line 1136 "compiler/main.gem"
    GemVal _t3228[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3228, 2))) {
#line 1137 "compiler/main.gem"
    GemVal _t3229[] = {gem_v_line_dir};
    GemVal _t3230 = gem_v_r;
    GemVal _t3231[] = {gem_table_get(_t3230, gem_string("setup"))};
    GemVal _t3232[] = {gem_v_p};
    GemVal _t3233[] = {gem_v_mname};
    GemVal _t3234[] = {gem_v_p};
    GemVal _t3235[] = {gem_v_mname};
    GemVal _t3236 = gem_v_r;
    GemVal _t3237[] = {gem_table_get(_t3236, gem_string("expr"))};
    GemVal _t3238[] = {gem_v_p};
    GemVal _t3239[] = {gem_v_p};
            GemVal _t3240 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3229, 1), gem_to_string_fn(NULL, _t3231, 1)), gem_to_string_fn(NULL, _t3232, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t3233, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3234, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3235, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3237, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3238, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3239, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t3240;
        }
#line 1139 "compiler/main.gem"
    GemVal _t3241[] = {gem_v_line_dir};
    GemVal _t3242 = gem_v_r;
    GemVal _t3243[] = {gem_table_get(_t3242, gem_string("setup"))};
    GemVal _t3244[] = {gem_v_p};
    GemVal _t3245[] = {gem_v_mname};
    GemVal _t3246 = gem_v_r;
    GemVal _t3247[] = {gem_table_get(_t3246, gem_string("expr"))};
    GemVal _t3248[] = {gem_v_p};
    GemVal _t3249[] = {gem_v_p};
        GemVal _t3250 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3241, 1), gem_to_string_fn(NULL, _t3243, 1)), gem_to_string_fn(NULL, _t3244, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3245, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3247, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3248, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3249, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3250;
    } else {
    GemVal _t3251[] = {gem_v__match_59};
    GemVal _t3253;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3251, 1), gem_string("table")))) {
        _t3253 = gem_eq(gem_type_fn(NULL, _t3251, 1), gem_string("table"));
    } else {
        GemVal _t3252[] = {gem_v__match_59, gem_string("tag")};
        _t3253 = gem_has_key_fn(NULL, _t3252, 2);
    }
    GemVal _t3254;
    if (!gem_truthy(_t3253)) {
        _t3254 = _t3253;
    } else {
        _t3254 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3256;
    if (!gem_truthy(_t3254)) {
        _t3256 = _t3254;
    } else {
        GemVal _t3255[] = {gem_v__match_59, gem_string("name")};
        _t3256 = gem_has_key_fn(NULL, _t3255, 2);
    }
    GemVal _t3258;
    if (!gem_truthy(_t3256)) {
        _t3258 = _t3256;
    } else {
        GemVal _t3257[] = {gem_v__match_59, gem_string("value")};
        _t3258 = gem_has_key_fn(NULL, _t3257, 2);
    }
    if (gem_truthy(_t3258)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1141 "compiler/main.gem"
    GemVal _t3259[] = {gem_v_value};
    GemVal _t3260 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3260.fn(_t3260.env, _t3259, 1);
#line 1142 "compiler/main.gem"
    GemVal _t3261 = (*gem_v_tmp);
        GemVal gem_v_t = _t3261.fn(_t3261.env, NULL, 0);
#line 1143 "compiler/main.gem"
    GemVal _t3262[] = {gem_v_name};
    GemVal _t3263 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3263.fn(_t3263.env, _t3262, 1);
#line 1144 "compiler/main.gem"
    GemVal _t3264[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3266;
    if (gem_truthy(gem_fn_set_contains(NULL, _t3264, 2))) {
        _t3266 = gem_fn_set_contains(NULL, _t3264, 2);
    } else {
        GemVal _t3265[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3266 = gem_fn_set_contains(NULL, _t3265, 2);
    }
        if (gem_truthy(_t3266)) {
#line 1145 "compiler/main.gem"
    GemVal _t3267[] = {gem_v_line_dir};
    GemVal _t3268 = gem_v_r;
    GemVal _t3269[] = {gem_table_get(_t3268, gem_string("setup"))};
    GemVal _t3270[] = {gem_v_p};
    GemVal _t3271[] = {gem_v_t};
    GemVal _t3272 = gem_v_r;
    GemVal _t3273[] = {gem_table_get(_t3272, gem_string("expr"))};
    GemVal _t3274[] = {gem_v_p};
    GemVal _t3275[] = {gem_v_mname};
    GemVal _t3276[] = {gem_v_t};
    GemVal _t3277[] = {gem_v_p};
    GemVal _t3278[] = {gem_v_p};
    GemVal _t3279[] = {gem_v_t};
            GemVal _t3280 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3267, 1), gem_to_string_fn(NULL, _t3269, 1)), gem_to_string_fn(NULL, _t3270, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3271, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3273, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3274, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3275, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3276, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3277, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3278, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3279, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3280;
        }
#line 1147 "compiler/main.gem"
    GemVal _t3281[] = {gem_v_line_dir};
    GemVal _t3282 = gem_v_r;
    GemVal _t3283[] = {gem_table_get(_t3282, gem_string("setup"))};
    GemVal _t3284[] = {gem_v_p};
    GemVal _t3285[] = {gem_v_t};
    GemVal _t3286 = gem_v_r;
    GemVal _t3287[] = {gem_table_get(_t3286, gem_string("expr"))};
    GemVal _t3288[] = {gem_v_p};
    GemVal _t3289[] = {gem_v_mname};
    GemVal _t3290[] = {gem_v_t};
    GemVal _t3291[] = {gem_v_p};
    GemVal _t3292[] = {gem_v_p};
    GemVal _t3293[] = {gem_v_t};
        GemVal _t3294 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3281, 1), gem_to_string_fn(NULL, _t3283, 1)), gem_to_string_fn(NULL, _t3284, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3285, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3287, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3288, 1)), gem_to_string_fn(NULL, _t3289, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3290, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3291, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3292, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3293, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3294;
    } else {
    GemVal _t3295[] = {gem_v__match_59};
    GemVal _t3297;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3295, 1), gem_string("table")))) {
        _t3297 = gem_eq(gem_type_fn(NULL, _t3295, 1), gem_string("table"));
    } else {
        GemVal _t3296[] = {gem_v__match_59, gem_string("tag")};
        _t3297 = gem_has_key_fn(NULL, _t3296, 2);
    }
    GemVal _t3298;
    if (!gem_truthy(_t3297)) {
        _t3298 = _t3297;
    } else {
        _t3298 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3300;
    if (!gem_truthy(_t3298)) {
        _t3300 = _t3298;
    } else {
        GemVal _t3299[] = {gem_v__match_59, gem_string("value")};
        _t3300 = gem_has_key_fn(NULL, _t3299, 2);
    }
    if (gem_truthy(_t3300)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1149 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1150 "compiler/main.gem"
    GemVal _t3301[] = {gem_v_value};
    GemVal _t3302 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3302.fn(_t3302.env, _t3301, 1);
#line 1151 "compiler/main.gem"
    GemVal _t3303 = (*gem_v_tmp);
            GemVal gem_v_t = _t3303.fn(_t3303.env, NULL, 0);
#line 1152 "compiler/main.gem"
    GemVal _t3304[] = {gem_v_line_dir};
    GemVal _t3305 = gem_v_r;
    GemVal _t3306[] = {gem_table_get(_t3305, gem_string("setup"))};
    GemVal _t3307[] = {gem_v_p};
    GemVal _t3308[] = {gem_v_t};
    GemVal _t3309 = gem_v_r;
    GemVal _t3310[] = {gem_table_get(_t3309, gem_string("expr"))};
    GemVal _t3311[] = {gem_v_p};
    GemVal _t3312[] = {gem_v_p};
    GemVal _t3313[] = {gem_v_t};
            GemVal _t3314 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3304, 1), gem_to_string_fn(NULL, _t3306, 1)), gem_to_string_fn(NULL, _t3307, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3308, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3310, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3311, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3312, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3313, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3314;
        }
#line 1154 "compiler/main.gem"
    GemVal _t3315[] = {gem_v_line_dir};
    GemVal _t3316[] = {gem_v_p};
    GemVal _t3317[] = {gem_v_p};
        GemVal _t3318 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3315, 1), gem_to_string_fn(NULL, _t3316, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3317, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3318;
    } else {
    GemVal _t3319[] = {gem_v__match_59};
    GemVal _t3321;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3319, 1), gem_string("table")))) {
        _t3321 = gem_eq(gem_type_fn(NULL, _t3319, 1), gem_string("table"));
    } else {
        GemVal _t3320[] = {gem_v__match_59, gem_string("tag")};
        _t3321 = gem_has_key_fn(NULL, _t3320, 2);
    }
    GemVal _t3322;
    if (!gem_truthy(_t3321)) {
        _t3322 = _t3321;
    } else {
        _t3322 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3322)) {
#line 1156 "compiler/main.gem"
    GemVal _t3323[] = {gem_v_line_dir};
    GemVal _t3324[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3325 = (*gem_v_compile_if);
        GemVal _t3326 = gem_add(gem_to_string_fn(NULL, _t3323, 1), _t3325.fn(_t3325.env, _t3324, 3));
        gem_pop_frame();
        return _t3326;
    } else {
    GemVal _t3327[] = {gem_v__match_59};
    GemVal _t3329;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3327, 1), gem_string("table")))) {
        _t3329 = gem_eq(gem_type_fn(NULL, _t3327, 1), gem_string("table"));
    } else {
        GemVal _t3328[] = {gem_v__match_59, gem_string("tag")};
        _t3329 = gem_has_key_fn(NULL, _t3328, 2);
    }
    GemVal _t3330;
    if (!gem_truthy(_t3329)) {
        _t3330 = _t3329;
    } else {
        _t3330 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3330)) {
#line 1158 "compiler/main.gem"
    GemVal _t3331[] = {gem_v_line_dir};
    GemVal _t3332[] = {gem_v_node, gem_v_indent};
    GemVal _t3333 = (*gem_v_compile_while);
    GemVal _t3334[] = {_t3333.fn(_t3333.env, _t3332, 2)};
    GemVal _t3335[] = {gem_v_p};
    GemVal _t3336[] = {gem_v_p};
        GemVal _t3337 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3331, 1), gem_to_string_fn(NULL, _t3334, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t3335, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3336, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3337;
    } else {
    GemVal _t3338[] = {gem_v__match_59};
    GemVal _t3340;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3338, 1), gem_string("table")))) {
        _t3340 = gem_eq(gem_type_fn(NULL, _t3338, 1), gem_string("table"));
    } else {
        GemVal _t3339[] = {gem_v__match_59, gem_string("tag")};
        _t3340 = gem_has_key_fn(NULL, _t3339, 2);
    }
    GemVal _t3341;
    if (!gem_truthy(_t3340)) {
        _t3341 = _t3340;
    } else {
        _t3341 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3341)) {
#line 1160 "compiler/main.gem"
    GemVal _t3342[] = {gem_v_line_dir};
    GemVal _t3343[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3344 = (*gem_v_compile_match);
        GemVal _t3345 = gem_add(gem_to_string_fn(NULL, _t3342, 1), _t3344.fn(_t3344.env, _t3343, 3));
        gem_pop_frame();
        return _t3345;
    } else {
    GemVal _t3346[] = {gem_v__match_59};
    GemVal _t3348;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3346, 1), gem_string("table")))) {
        _t3348 = gem_eq(gem_type_fn(NULL, _t3346, 1), gem_string("table"));
    } else {
        GemVal _t3347[] = {gem_v__match_59, gem_string("tag")};
        _t3348 = gem_has_key_fn(NULL, _t3347, 2);
    }
    GemVal _t3349;
    if (!gem_truthy(_t3348)) {
        _t3349 = _t3348;
    } else {
        _t3349 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3349)) {
#line 1162 "compiler/main.gem"
    GemVal _t3350[] = {gem_v_line_dir};
    GemVal _t3351[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3352 = (*gem_v_compile_receive_match);
        GemVal _t3353 = gem_add(gem_to_string_fn(NULL, _t3350, 1), _t3352.fn(_t3352.env, _t3351, 3));
        gem_pop_frame();
        return _t3353;
    } else {
    GemVal _t3354[] = {gem_v__match_59};
    GemVal _t3356;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3354, 1), gem_string("table")))) {
        _t3356 = gem_eq(gem_type_fn(NULL, _t3354, 1), gem_string("table"));
    } else {
        GemVal _t3355[] = {gem_v__match_59, gem_string("tag")};
        _t3356 = gem_has_key_fn(NULL, _t3355, 2);
    }
    GemVal _t3357;
    if (!gem_truthy(_t3356)) {
        _t3357 = _t3356;
    } else {
        _t3357 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3359;
    if (!gem_truthy(_t3357)) {
        _t3359 = _t3357;
    } else {
        GemVal _t3358[] = {gem_v__match_59, gem_string("stmts")};
        _t3359 = gem_has_key_fn(NULL, _t3358, 2);
    }
    if (gem_truthy(_t3359)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_59, gem_string("stmts"));
#line 1164 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1165 "compiler/main.gem"
    GemVal _t3360[] = {gem_v_p};
    GemVal _t3361[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3360, 1), gem_string("{\n"))};
        (void)(gem_buf_push_fn(NULL, _t3361, 2));
#line 1166 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1166 "compiler/main.gem"
    GemVal _t3362[] = {gem_v_stmts};
        GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3362, 1), gem_int(1));
#line 1166 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1166 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1166 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1167 "compiler/main.gem"
    GemVal _t3363[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3364 = (*gem_v_compile_stmt);
    GemVal _t3365[] = {gem_v_b, _t3364.fn(_t3364.env, _t3363, 2)};
            (void)(gem_buf_push_fn(NULL, _t3365, 2));
#line 1168 "compiler/main.gem"
    GemVal _t3366[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3366, 2));
        }

#line 1170 "compiler/main.gem"
    GemVal _t3367[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3367, 1), gem_int(0)))) {
#line 1171 "compiler/main.gem"
    GemVal _t3368[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3369 = (*gem_v_compile_stmt_return);
    GemVal _t3370[] = {gem_v_b, _t3369.fn(_t3369.env, _t3368, 2)};
            (void)(gem_buf_push_fn(NULL, _t3370, 2));
#line 1172 "compiler/main.gem"
    GemVal _t3371[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3371, 2));
        }
#line 1174 "compiler/main.gem"
    GemVal _t3372[] = {gem_v_p};
    GemVal _t3373[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3372, 1), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3373, 2));
#line 1175 "compiler/main.gem"
    GemVal _t3374[] = {gem_v_b};
        GemVal _t3375 = gem_buf_str_fn(NULL, _t3374, 1);
        gem_pop_frame();
        return _t3375;
    } else {
    GemVal _t3376[] = {gem_v__match_59};
    GemVal _t3378;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3376, 1), gem_string("table")))) {
        _t3378 = gem_eq(gem_type_fn(NULL, _t3376, 1), gem_string("table"));
    } else {
        GemVal _t3377[] = {gem_v__match_59, gem_string("tag")};
        _t3378 = gem_has_key_fn(NULL, _t3377, 2);
    }
    GemVal _t3379;
    if (!gem_truthy(_t3378)) {
        _t3379 = _t3378;
    } else {
        _t3379 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3379)) {
#line 1177 "compiler/main.gem"
    GemVal _t3380[] = {gem_v_node, gem_v_indent};
    GemVal _t3381 = (*gem_v_compile_stmt);
    GemVal _t3382[] = {_t3381.fn(_t3381.env, _t3380, 2)};
    GemVal _t3383[] = {gem_v_p};
    GemVal _t3384[] = {gem_v_p};
        GemVal _t3385 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3382, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3383, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3384, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3385;
    } else {
    GemVal _t3386[] = {gem_v__match_59};
    GemVal _t3388;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3386, 1), gem_string("table")))) {
        _t3388 = gem_eq(gem_type_fn(NULL, _t3386, 1), gem_string("table"));
    } else {
        GemVal _t3387[] = {gem_v__match_59, gem_string("tag")};
        _t3388 = gem_has_key_fn(NULL, _t3387, 2);
    }
    GemVal _t3389;
    if (!gem_truthy(_t3388)) {
        _t3389 = _t3388;
    } else {
        _t3389 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3389)) {
#line 1179 "compiler/main.gem"
    GemVal _t3390[] = {gem_v_node, gem_v_indent};
    GemVal _t3391 = (*gem_v_compile_stmt);
    GemVal _t3392[] = {_t3391.fn(_t3391.env, _t3390, 2)};
    GemVal _t3393[] = {gem_v_p};
    GemVal _t3394[] = {gem_v_p};
        GemVal _t3395 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3392, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3393, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3394, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3395;
    } else {
    GemVal _t3396[] = {gem_v__match_59};
    GemVal _t3398;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3396, 1), gem_string("table")))) {
        _t3398 = gem_eq(gem_type_fn(NULL, _t3396, 1), gem_string("table"));
    } else {
        GemVal _t3397[] = {gem_v__match_59, gem_string("tag")};
        _t3398 = gem_has_key_fn(NULL, _t3397, 2);
    }
    GemVal _t3399;
    if (!gem_truthy(_t3398)) {
        _t3399 = _t3398;
    } else {
        _t3399 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3399)) {
#line 1181 "compiler/main.gem"
    GemVal _t3400[] = {gem_v_node, gem_v_indent};
    GemVal _t3401 = (*gem_v_compile_stmt);
    GemVal _t3402[] = {_t3401.fn(_t3401.env, _t3400, 2)};
    GemVal _t3403[] = {gem_v_p};
    GemVal _t3404[] = {gem_v_p};
        GemVal _t3405 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3402, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3403, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3404, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3405;
    } else {
    GemVal _t3406[] = {gem_v__match_59};
    GemVal _t3408;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3406, 1), gem_string("table")))) {
        _t3408 = gem_eq(gem_type_fn(NULL, _t3406, 1), gem_string("table"));
    } else {
        GemVal _t3407[] = {gem_v__match_59, gem_string("tag")};
        _t3408 = gem_has_key_fn(NULL, _t3407, 2);
    }
    GemVal _t3409;
    if (!gem_truthy(_t3408)) {
        _t3409 = _t3408;
    } else {
        _t3409 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3409)) {
#line 1183 "compiler/main.gem"
    GemVal _t3410[] = {gem_v_node, gem_v_indent};
    GemVal _t3411 = (*gem_v_compile_stmt);
    GemVal _t3412[] = {_t3411.fn(_t3411.env, _t3410, 2)};
    GemVal _t3413[] = {gem_v_p};
    GemVal _t3414[] = {gem_v_p};
        GemVal _t3415 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3412, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3413, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3414, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3415;
    } else {
    GemVal _t3416[] = {gem_v__match_59};
    GemVal _t3418;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3416, 1), gem_string("table")))) {
        _t3418 = gem_eq(gem_type_fn(NULL, _t3416, 1), gem_string("table"));
    } else {
        GemVal _t3417[] = {gem_v__match_59, gem_string("tag")};
        _t3418 = gem_has_key_fn(NULL, _t3417, 2);
    }
    GemVal _t3419;
    if (!gem_truthy(_t3418)) {
        _t3419 = _t3418;
    } else {
        _t3419 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3419)) {
#line 1185 "compiler/main.gem"
    GemVal _t3420[] = {gem_v_node, gem_v_indent};
    GemVal _t3421 = (*gem_v_compile_stmt);
    GemVal _t3422[] = {_t3421.fn(_t3421.env, _t3420, 2)};
    GemVal _t3423[] = {gem_v_p};
    GemVal _t3424[] = {gem_v_p};
        GemVal _t3425 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3422, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3423, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3424, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3425;
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
#line 1188 "compiler/main.gem"
    GemVal _t3426[] = {gem_v_node};
    GemVal _t3427 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3427.fn(_t3427.env, _t3426, 1);
#line 1189 "compiler/main.gem"
    GemVal _t3428 = (*gem_v_tmp);
    GemVal gem_v_t = _t3428.fn(_t3428.env, NULL, 0);
    GemVal _t3429[] = {gem_v_line_dir};
    GemVal _t3430 = gem_v_r;
    GemVal _t3431[] = {gem_table_get(_t3430, gem_string("setup"))};
    GemVal _t3432[] = {gem_v_p};
    GemVal _t3433[] = {gem_v_t};
    GemVal _t3434 = gem_v_r;
    GemVal _t3435[] = {gem_table_get(_t3434, gem_string("expr"))};
    GemVal _t3436[] = {gem_v_p};
    GemVal _t3437[] = {gem_v_p};
    GemVal _t3438[] = {gem_v_t};
    GemVal _t3439 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3429, 1), gem_to_string_fn(NULL, _t3431, 1)), gem_to_string_fn(NULL, _t3432, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3433, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3435, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3436, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3437, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3438, 1)), gem_string(";"));
    gem_pop_frame();
    return _t3439;
}

struct _closure__anon_47 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts_tail;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1196 "compiler/main.gem"
    GemVal _t3441[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3441, 1);
#line 1197 "compiler/main.gem"
    GemVal _t3442 = gem_v_node;
    GemVal _t3443[] = {gem_table_get(_t3442, gem_string("cond"))};
    GemVal _t3444 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3444.fn(_t3444.env, _t3443, 1);
#line 1198 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1199 "compiler/main.gem"
    GemVal _t3445 = gem_v_cond_r;
    GemVal _t3446[] = {gem_v_b, gem_table_get(_t3445, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3446, 2));
#line 1200 "compiler/main.gem"
    GemVal _t3447[] = {gem_v_p};
    GemVal _t3448 = gem_v_cond_r;
    GemVal _t3449[] = {gem_table_get(_t3448, gem_string("expr"))};
    GemVal _t3450[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3447, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3449, 1)), gem_string(")) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3450, 2));
#line 1201 "compiler/main.gem"
    GemVal _t3451 = gem_v_node;
    GemVal _t3452[] = {gem_table_get(_t3451, gem_string("then")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3453 = (*gem_v_compile_stmts_tail);
    GemVal _t3454[] = {gem_v_b, _t3453.fn(_t3453.env, _t3452, 3)};
    (void)(gem_buf_push_fn(NULL, _t3454, 2));
#line 1202 "compiler/main.gem"
    GemVal _t3455 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3455, gem_string("else")), GEM_NIL))) {
#line 1203 "compiler/main.gem"
    GemVal _t3456[] = {gem_v_p};
    GemVal _t3457[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3456, 1), gem_string("} else {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3457, 2));
#line 1204 "compiler/main.gem"
    GemVal _t3458 = gem_v_node;
    GemVal _t3459[] = {gem_table_get(_t3458, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3460 = (*gem_v_compile_stmts_tail);
    GemVal _t3461[] = {gem_v_b, _t3460.fn(_t3460.env, _t3459, 3)};
        (void)(gem_buf_push_fn(NULL, _t3461, 2));
    } else {
#line 1205 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1206 "compiler/main.gem"
    GemVal _t3462[] = {gem_v_p};
    GemVal _t3463[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3462, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3463, 2));
#line 1207 "compiler/main.gem"
    GemVal _t3464 = gem_table_new();
    GemVal _t3465[] = {_t3464, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3466 = (*gem_v_compile_stmts_tail);
    GemVal _t3467[] = {gem_v_b, _t3466.fn(_t3466.env, _t3465, 3)};
            (void)(gem_buf_push_fn(NULL, _t3467, 2));
        }
    }
#line 1209 "compiler/main.gem"
    GemVal _t3468[] = {gem_v_p};
    GemVal _t3469[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3468, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3469, 2));
#line 1210 "compiler/main.gem"
    GemVal _t3470[] = {gem_v_b};
    GemVal _t3471 = gem_buf_str_fn(NULL, _t3470, 1);
    gem_pop_frame();
    return _t3471;
}

struct _closure__anon_48 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1216 "compiler/main.gem"
    GemVal _t3473[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3473, 1);
#line 1217 "compiler/main.gem"
    GemVal _t3474 = gem_v_node;
    GemVal _t3475[] = {gem_table_get(_t3474, gem_string("cond"))};
    GemVal _t3476 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3476.fn(_t3476.env, _t3475, 1);
#line 1218 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1219 "compiler/main.gem"
    GemVal _t3477[] = {gem_v_p};
    GemVal _t3478[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3477, 1), gem_string("while (1) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3478, 2));
#line 1220 "compiler/main.gem"
    GemVal _t3479[] = {gem_v_p};
    GemVal _t3480[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3479, 1), gem_string("    gem_yield_check();\n"))};
    (void)(gem_buf_push_fn(NULL, _t3480, 2));
#line 1221 "compiler/main.gem"
    GemVal _t3481 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3481, gem_string("setup")), gem_string("")))) {
#line 1222 "compiler/main.gem"
    GemVal _t3482 = gem_v_cond_r;
    GemVal _t3483[] = {gem_v_p};
    GemVal _t3484[] = {gem_table_get(_t3482, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t3483, 1), gem_string("    "))};
    GemVal _t3485[] = {gem_v_b, gem_str_replace_fn(NULL, _t3484, 3)};
        (void)(gem_buf_push_fn(NULL, _t3485, 2));
    }
#line 1224 "compiler/main.gem"
    GemVal _t3486[] = {gem_v_p};
    GemVal _t3487 = gem_v_cond_r;
    GemVal _t3488[] = {gem_table_get(_t3487, gem_string("expr"))};
    GemVal _t3489[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3486, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t3488, 1)), gem_string(")) break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3489, 2));
#line 1225 "compiler/main.gem"
    GemVal _t3490 = gem_v_node;
    GemVal _t3491[] = {gem_table_get(_t3490, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3492 = (*gem_v_compile_stmts);
    GemVal _t3493[] = {gem_v_b, _t3492.fn(_t3492.env, _t3491, 2)};
    (void)(gem_buf_push_fn(NULL, _t3493, 2));
#line 1226 "compiler/main.gem"
    GemVal _t3494[] = {gem_v_p};
    GemVal _t3495[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3494, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3495, 2));
#line 1227 "compiler/main.gem"
    GemVal _t3496[] = {gem_v_b};
    GemVal _t3497 = gem_buf_str_fn(NULL, _t3496, 1);
    gem_pop_frame();
    return _t3497;
}

struct _closure__anon_49 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1233 "compiler/main.gem"
    GemVal _t3499[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3499, 1);
#line 1234 "compiler/main.gem"
    GemVal _t3500 = gem_v_node;
    GemVal _t3501[] = {gem_table_get(_t3500, gem_string("target"))};
    GemVal _t3502 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3502.fn(_t3502.env, _t3501, 1);
#line 1235 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1236 "compiler/main.gem"
    GemVal _t3503 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3503, gem_string("target_var")), GEM_NIL))) {
#line 1237 "compiler/main.gem"
    GemVal _t3504 = gem_v_node;
    GemVal _t3505[] = {gem_table_get(_t3504, gem_string("target_var"))};
    GemVal _t3506 = (*gem_v_mangle);
        gem_v_t = _t3506.fn(_t3506.env, _t3505, 1);
    } else {
#line 1239 "compiler/main.gem"
    GemVal _t3507 = (*gem_v_tmp);
        gem_v_t = _t3507.fn(_t3507.env, NULL, 0);
    }
#line 1241 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1242 "compiler/main.gem"
    GemVal _t3508 = gem_v_target_r;
    GemVal _t3509[] = {gem_v_b, gem_table_get(_t3508, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3509, 2));
#line 1243 "compiler/main.gem"
    GemVal _t3510[] = {gem_v_p};
    GemVal _t3511[] = {gem_v_t};
    GemVal _t3512 = gem_v_target_r;
    GemVal _t3513[] = {gem_table_get(_t3512, gem_string("expr"))};
    GemVal _t3514[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3510, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3511, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3513, 1)), gem_string(";\n"))};
    (void)(gem_buf_push_fn(NULL, _t3514, 2));
#line 1244 "compiler/main.gem"
    GemVal _t3515 = gem_v_node;
    GemVal _t3516[] = {gem_table_get(_t3515, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3516, 1), gem_int(0)))) {
#line 1245 "compiler/main.gem"
    GemVal _t3517 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3517, gem_string("else")), GEM_NIL))) {
#line 1246 "compiler/main.gem"
    GemVal _t3518 = gem_v_node;
    GemVal _t3519[] = {gem_table_get(_t3518, gem_string("else")), gem_v_indent, gem_v_return_pos};
    GemVal _t3520 = (*gem_v_compile_stmts_tail);
    GemVal _t3521[] = {gem_v_b, _t3520.fn(_t3520.env, _t3519, 3)};
            (void)(gem_buf_push_fn(NULL, _t3521, 2));
        } else {
#line 1247 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1248 "compiler/main.gem"
    GemVal _t3522[] = {gem_v_p};
    GemVal _t3523[] = {gem_v_p};
    GemVal _t3524[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3522, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3523, 1)), gem_string("return GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3524, 2));
            }
        }
#line 1250 "compiler/main.gem"
    GemVal _t3525[] = {gem_v_p};
    GemVal _t3526[] = {gem_v_t};
    GemVal _t3527[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3525, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3526, 1)), gem_string(";"))};
        (void)(gem_buf_push_fn(NULL, _t3527, 2));
#line 1251 "compiler/main.gem"
    GemVal _t3528[] = {gem_v_b};
        GemVal _t3529 = gem_buf_str_fn(NULL, _t3528, 1);
        gem_pop_frame();
        return _t3529;
    }
#line 1253 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1253 "compiler/main.gem"
    GemVal _t3530 = gem_v_node;
    GemVal _t3531[] = {gem_table_get(_t3530, gem_string("whens"))};
    GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t3531, 1);
#line 1253 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1253 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_62;
#line 1253 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1254 "compiler/main.gem"
    GemVal _t3532 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3532, gem_string("whens")), gem_v_i);
#line 1255 "compiler/main.gem"
    GemVal _t3533 = gem_v_w;
    GemVal _t3534[] = {gem_table_get(_t3533, gem_string("value"))};
    GemVal _t3535 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3535.fn(_t3535.env, _t3534, 1);
#line 1256 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1257 "compiler/main.gem"
    GemVal _t3536 = gem_v_val_r;
    GemVal _t3537[] = {gem_v_b, gem_table_get(_t3536, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3537, 2));
#line 1258 "compiler/main.gem"
    GemVal _t3538[] = {gem_v_p};
    GemVal _t3539 = gem_v_val_r;
    GemVal _t3540[] = {gem_table_get(_t3539, gem_string("expr"))};
    GemVal _t3541[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3538, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3540, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3541, 2));
        } else {
#line 1260 "compiler/main.gem"
    GemVal _t3542[] = {gem_v_p};
    GemVal _t3543[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3542, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3543, 2));
#line 1261 "compiler/main.gem"
    GemVal _t3544 = gem_v_val_r;
    GemVal _t3545[] = {gem_v_b, gem_table_get(_t3544, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3545, 2));
#line 1262 "compiler/main.gem"
    GemVal _t3546[] = {gem_v_p};
    GemVal _t3547 = gem_v_val_r;
    GemVal _t3548[] = {gem_table_get(_t3547, gem_string("expr"))};
    GemVal _t3549[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3546, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3548, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3549, 2));
        }
#line 1264 "compiler/main.gem"
    GemVal _t3550 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3550, gem_string("bindings")), GEM_NIL))) {
#line 1265 "compiler/main.gem"
    GemVal _t3551 = gem_v_w;
            GemVal gem_v__for_items_61 = gem_table_get(_t3551, gem_string("bindings"));
#line 1265 "compiler/main.gem"
            GemVal gem_v__for_i_61 = gem_int(0);
#line 1265 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3552[] = {gem_v__for_items_61};
                if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3552, 1)))) break;
#line 1265 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1265 "compiler/main.gem"
                gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1266 "compiler/main.gem"
    GemVal _t3553[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3554 = (*gem_v_compile_stmt);
    GemVal _t3555[] = {gem_v_b, _t3554.fn(_t3554.env, _t3553, 2)};
                (void)(gem_buf_push_fn(NULL, _t3555, 2));
#line 1267 "compiler/main.gem"
    GemVal _t3556[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3556, 2));
            }

        }
#line 1270 "compiler/main.gem"
    GemVal _t3557 = gem_v_w;
    GemVal _t3558[] = {gem_table_get(_t3557, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3559 = (*gem_v_compile_stmts_tail);
    GemVal _t3560[] = {gem_v_b, _t3559.fn(_t3559.env, _t3558, 3)};
        (void)(gem_buf_push_fn(NULL, _t3560, 2));
    }

#line 1272 "compiler/main.gem"
    GemVal _t3561 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3561, gem_string("else")), GEM_NIL))) {
#line 1273 "compiler/main.gem"
    GemVal _t3562[] = {gem_v_p};
    GemVal _t3563[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3562, 1), gem_string("} else {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3563, 2));
#line 1274 "compiler/main.gem"
    GemVal _t3564 = gem_v_node;
    GemVal _t3565[] = {gem_table_get(_t3564, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3566 = (*gem_v_compile_stmts_tail);
    GemVal _t3567[] = {gem_v_b, _t3566.fn(_t3566.env, _t3565, 3)};
        (void)(gem_buf_push_fn(NULL, _t3567, 2));
    } else {
#line 1275 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1276 "compiler/main.gem"
    GemVal _t3568[] = {gem_v_p};
    GemVal _t3569[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3568, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3569, 2));
#line 1277 "compiler/main.gem"
    GemVal _t3570 = gem_table_new();
    GemVal _t3571[] = {_t3570, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3572 = (*gem_v_compile_stmts_tail);
    GemVal _t3573[] = {gem_v_b, _t3572.fn(_t3572.env, _t3571, 3)};
            (void)(gem_buf_push_fn(NULL, _t3573, 2));
        }
    }
#line 1279 "compiler/main.gem"
    GemVal _t3574[] = {gem_v_p};
    GemVal _t3575[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3574, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3575, 2));
#line 1280 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(1);
#line 1280 "compiler/main.gem"
    GemVal _t3576 = gem_v_node;
    GemVal _t3577[] = {gem_table_get(_t3576, gem_string("whens"))};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3577, 1);
#line 1280 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1280 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1280 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1281 "compiler/main.gem"
    GemVal _t3578[] = {gem_v_p};
    GemVal _t3579[] = {gem_v_b, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3578, 1)), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3579, 2));
    }

#line 1283 "compiler/main.gem"
    GemVal _t3580[] = {gem_v_b};
    GemVal _t3581 = gem_buf_str_fn(NULL, _t3580, 1);
    gem_pop_frame();
    return _t3581;
}

struct _closure__anon_50 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_50(void *_env, GemVal *args, int argc) {
    struct _closure__anon_50 *_cls = (struct _closure__anon_50 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_50", "compiler/main.gem", 0);
#line 1289 "compiler/main.gem"
    GemVal _t3583[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3583, 1);
#line 1290 "compiler/main.gem"
    GemVal _t3584 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3584, gem_string("arms"));
#line 1291 "compiler/main.gem"
    GemVal _t3585 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3585, gem_string("after_ms"));
#line 1292 "compiler/main.gem"
    GemVal _t3586 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3586, gem_string("after_body"));
#line 1294 "compiler/main.gem"
    GemVal _t3587 = gem_v_node;
    GemVal _t3588[] = {gem_table_get(_t3587, gem_string("recv_var"))};
    GemVal _t3589 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3589.fn(_t3589.env, _t3588, 1);
#line 1295 "compiler/main.gem"
    GemVal _t3590 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3590.fn(_t3590.env, NULL, 0);
#line 1296 "compiler/main.gem"
    GemVal _t3591 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3591.fn(_t3591.env, NULL, 0);
#line 1297 "compiler/main.gem"
    GemVal _t3592 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3592.fn(_t3592.env, NULL, 0);
#line 1298 "compiler/main.gem"
    GemVal _t3593 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3593.fn(_t3593.env, NULL, 0);
#line 1299 "compiler/main.gem"
    GemVal _t3594 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3594.fn(_t3594.env, NULL, 0);
#line 1301 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1303 "compiler/main.gem"
    GemVal _t3595[] = {gem_v_p};
    GemVal _t3596[] = {gem_v_recv_tmp};
    GemVal _t3597[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3595, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3596, 1)), gem_string(" = GEM_NIL;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3597, 2));
#line 1304 "compiler/main.gem"
    GemVal _t3598[] = {gem_v_p};
    GemVal _t3599[] = {gem_v_matched_tmp};
    GemVal _t3600[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3598, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3599, 1)), gem_string(" = -1;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3600, 2));
#line 1306 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1307 "compiler/main.gem"
    GemVal _t3601[] = {gem_v_after_ms};
    GemVal _t3602 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3602.fn(_t3602.env, _t3601, 1);
#line 1308 "compiler/main.gem"
    GemVal _t3603 = gem_v_ms_r;
    GemVal _t3604[] = {gem_v_b, gem_table_get(_t3603, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t3604, 2));
#line 1309 "compiler/main.gem"
    GemVal _t3605[] = {gem_v_p};
    GemVal _t3606[] = {gem_v_deadline_tmp};
    GemVal _t3607 = gem_v_ms_r;
    GemVal _t3608[] = {gem_table_get(_t3607, gem_string("expr"))};
    GemVal _t3609[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3605, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3606, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3608, 1)), gem_string(").ival;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3609, 2));
    }
#line 1312 "compiler/main.gem"
    GemVal _t3610[] = {gem_v_p};
    GemVal _t3611[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3610, 1), gem_string("while (1) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3611, 2));
#line 1313 "compiler/main.gem"
    GemVal _t3612[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3612, 1);
#line 1315 "compiler/main.gem"
    GemVal _t3613[] = {gem_v_p2};
    GemVal _t3614[] = {gem_v_proc_tmp};
    GemVal _t3615[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3613, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3614, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n"))};
    (void)(gem_buf_push_fn(NULL, _t3615, 2));
#line 1316 "compiler/main.gem"
    GemVal _t3616[] = {gem_v_p2};
    GemVal _t3617[] = {gem_v_prev_tmp};
    GemVal _t3618[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3616, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3617, 1)), gem_string(" = NULL;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3618, 2));
#line 1317 "compiler/main.gem"
    GemVal _t3619[] = {gem_v_p2};
    GemVal _t3620[] = {gem_v_cur_tmp};
    GemVal _t3621[] = {gem_v_proc_tmp};
    GemVal _t3622[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3619, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3620, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3621, 1)), gem_string("->mailbox.head;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3622, 2));
#line 1318 "compiler/main.gem"
    GemVal _t3623[] = {gem_v_p2};
    GemVal _t3624[] = {gem_v_matched_tmp};
    GemVal _t3625[] = {gem_v_b, gem_add(gem_add(gem_to_string_fn(NULL, _t3623, 1), gem_to_string_fn(NULL, _t3624, 1)), gem_string(" = -1;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3625, 2));
#line 1319 "compiler/main.gem"
    GemVal _t3626[] = {gem_v_p2};
    GemVal _t3627[] = {gem_v_cur_tmp};
    GemVal _t3628[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3626, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3627, 1)), gem_string(") {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3628, 2));
#line 1320 "compiler/main.gem"
    GemVal _t3629[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3629, 1);
#line 1321 "compiler/main.gem"
    GemVal _t3630[] = {gem_v_p3};
    GemVal _t3631[] = {gem_v_recv_tmp};
    GemVal _t3632[] = {gem_v_cur_tmp};
    GemVal _t3633[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3630, 1), gem_to_string_fn(NULL, _t3631, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3632, 1)), gem_string("->value;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3633, 2));
#line 1323 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1323 "compiler/main.gem"
    GemVal _t3634[] = {gem_v_arms};
    GemVal gem_v__for_limit_64 = gem_len_fn(NULL, _t3634, 1);
#line 1323 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1323 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_64;
#line 1323 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1324 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1325 "compiler/main.gem"
    GemVal _t3635 = gem_v_arm;
    GemVal _t3636 = gem_table_get(_t3635, gem_string("pattern"));
    GemVal _t3637[] = {gem_table_get(_t3636, gem_string("condition"))};
    GemVal _t3638 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3638.fn(_t3638.env, _t3637, 1);
#line 1326 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1327 "compiler/main.gem"
    GemVal _t3639 = gem_v_cond_r;
    GemVal _t3640[] = {gem_v_b, gem_table_get(_t3639, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3640, 2));
#line 1328 "compiler/main.gem"
    GemVal _t3641[] = {gem_v_p3};
    GemVal _t3642 = gem_v_cond_r;
    GemVal _t3643[] = {gem_table_get(_t3642, gem_string("expr"))};
    GemVal _t3644[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3641, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3643, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3644, 2));
        } else {
#line 1330 "compiler/main.gem"
    GemVal _t3645[] = {gem_v_p3};
    GemVal _t3646[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3645, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3646, 2));
#line 1331 "compiler/main.gem"
    GemVal _t3647 = gem_v_cond_r;
    GemVal _t3648[] = {gem_v_b, gem_table_get(_t3647, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3648, 2));
#line 1332 "compiler/main.gem"
    GemVal _t3649[] = {gem_v_p3};
    GemVal _t3650 = gem_v_cond_r;
    GemVal _t3651[] = {gem_table_get(_t3650, gem_string("expr"))};
    GemVal _t3652[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3649, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3651, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3652, 2));
        }
#line 1334 "compiler/main.gem"
    GemVal _t3653[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3654[] = {gem_fn_pad_for(NULL, _t3653, 1)};
    GemVal _t3655[] = {gem_v_matched_tmp};
    GemVal _t3656[] = {gem_v_i};
    GemVal _t3657[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3654, 1), gem_to_string_fn(NULL, _t3655, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3656, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3657, 2));
    }

#line 1336 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1336 "compiler/main.gem"
    GemVal _t3658[] = {gem_v_arms};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3658, 1);
#line 1336 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1336 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1336 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1337 "compiler/main.gem"
    GemVal _t3659[] = {gem_v_p3};
    GemVal _t3660[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3659, 1), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3660, 2));
#line 1338 "compiler/main.gem"
    GemVal _t3661[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3661, 1), gem_int(1))))) {
#line 1339 "compiler/main.gem"
    GemVal _t3662[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3662, 2));
        }
    }

#line 1342 "compiler/main.gem"
    GemVal _t3663[] = {gem_v_b, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t3663, 2));
#line 1344 "compiler/main.gem"
    GemVal _t3664[] = {gem_v_p3};
    GemVal _t3665[] = {gem_v_matched_tmp};
    GemVal _t3666[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3664, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3665, 1)), gem_string(" >= 0) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3666, 2));
#line 1345 "compiler/main.gem"
    GemVal _t3667[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3667, 1);
#line 1346 "compiler/main.gem"
    GemVal _t3668[] = {gem_v_p4};
    GemVal _t3669[] = {gem_v_proc_tmp};
    GemVal _t3670[] = {gem_v_prev_tmp};
    GemVal _t3671[] = {gem_v_cur_tmp};
    GemVal _t3672[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3668, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3669, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3670, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3671, 1)), gem_string(");\n"))};
    (void)(gem_buf_push_fn(NULL, _t3672, 2));
#line 1347 "compiler/main.gem"
    GemVal _t3673[] = {gem_v_p4};
    GemVal _t3674[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3673, 1), gem_string("break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3674, 2));
#line 1348 "compiler/main.gem"
    GemVal _t3675[] = {gem_v_p3};
    GemVal _t3676[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3675, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3676, 2));
#line 1350 "compiler/main.gem"
    GemVal _t3677[] = {gem_v_p3};
    GemVal _t3678[] = {gem_v_prev_tmp};
    GemVal _t3679[] = {gem_v_cur_tmp};
    GemVal _t3680[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3677, 1), gem_to_string_fn(NULL, _t3678, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3679, 1)), gem_string(";\n"))};
    (void)(gem_buf_push_fn(NULL, _t3680, 2));
#line 1351 "compiler/main.gem"
    GemVal _t3681[] = {gem_v_p3};
    GemVal _t3682[] = {gem_v_cur_tmp};
    GemVal _t3683[] = {gem_v_cur_tmp};
    GemVal _t3684[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3681, 1), gem_to_string_fn(NULL, _t3682, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3683, 1)), gem_string("->next;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3684, 2));
#line 1352 "compiler/main.gem"
    GemVal _t3685[] = {gem_v_p2};
    GemVal _t3686[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3685, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3686, 2));
#line 1354 "compiler/main.gem"
    GemVal _t3687[] = {gem_v_p2};
    GemVal _t3688[] = {gem_v_matched_tmp};
    GemVal _t3689[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3687, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3688, 1)), gem_string(" >= 0) break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3689, 2));
#line 1355 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1356 "compiler/main.gem"
    GemVal _t3690[] = {gem_v_p2};
    GemVal _t3691[] = {gem_v_deadline_tmp};
    GemVal _t3692[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3690, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3691, 1)), gem_string(") {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3692, 2));
#line 1357 "compiler/main.gem"
    GemVal _t3693[] = {gem_v_p2};
    GemVal _t3694[] = {gem_v_matched_tmp};
    GemVal _t3695[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3693, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3694, 1)), gem_string(" = -2;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3695, 2));
#line 1358 "compiler/main.gem"
    GemVal _t3696[] = {gem_v_p2};
    GemVal _t3697[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3696, 1), gem_string("    break;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3697, 2));
#line 1359 "compiler/main.gem"
    GemVal _t3698[] = {gem_v_p2};
    GemVal _t3699[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3698, 1), gem_string("}\n"))};
        (void)(gem_buf_push_fn(NULL, _t3699, 2));
#line 1360 "compiler/main.gem"
    GemVal _t3700[] = {gem_v_p2};
    GemVal _t3701[] = {gem_v_deadline_tmp};
    GemVal _t3702[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3700, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3701, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t3702, 2));
#line 1361 "compiler/main.gem"
    GemVal _t3703[] = {gem_v_p2};
    GemVal _t3704[] = {gem_v_proc_tmp};
    GemVal _t3705[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3703, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3704, 1)), gem_string("->timed_out) {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3705, 2));
#line 1362 "compiler/main.gem"
    GemVal _t3706[] = {gem_v_p2};
    GemVal _t3707[] = {gem_v_matched_tmp};
    GemVal _t3708[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3706, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3707, 1)), gem_string(" = -2;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3708, 2));
#line 1363 "compiler/main.gem"
    GemVal _t3709[] = {gem_v_p2};
    GemVal _t3710[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3709, 1), gem_string("    break;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3710, 2));
#line 1364 "compiler/main.gem"
    GemVal _t3711[] = {gem_v_p2};
    GemVal _t3712[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3711, 1), gem_string("}\n"))};
        (void)(gem_buf_push_fn(NULL, _t3712, 2));
    } else {
#line 1366 "compiler/main.gem"
    GemVal _t3713[] = {gem_v_p2};
    GemVal _t3714[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3713, 1), gem_string("gem_selective_yield(-1);\n"))};
        (void)(gem_buf_push_fn(NULL, _t3714, 2));
    }
#line 1369 "compiler/main.gem"
    GemVal _t3715[] = {gem_v_p};
    GemVal _t3716[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3715, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3716, 2));
#line 1372 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1372 "compiler/main.gem"
    GemVal _t3717[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3717, 1);
#line 1372 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1372 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1372 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1373 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1374 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1375 "compiler/main.gem"
    GemVal _t3718[] = {gem_v_p};
    GemVal _t3719[] = {gem_v_matched_tmp};
    GemVal _t3720[] = {gem_v_i};
    GemVal _t3721[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3718, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3719, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3720, 1)), gem_string(") {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3721, 2));
        } else {
#line 1377 "compiler/main.gem"
    GemVal _t3722[] = {gem_v_p};
    GemVal _t3723[] = {gem_v_matched_tmp};
    GemVal _t3724[] = {gem_v_i};
    GemVal _t3725[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3722, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3723, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3724, 1)), gem_string(") {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3725, 2));
        }
#line 1379 "compiler/main.gem"
    GemVal _t3726 = gem_v_arm;
    GemVal _t3727 = gem_table_get(_t3726, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3727, gem_string("bindings")), GEM_NIL))) {
#line 1380 "compiler/main.gem"
    GemVal _t3728 = gem_v_arm;
    GemVal _t3729 = gem_table_get(_t3728, gem_string("pattern"));
            GemVal gem_v__for_items_66 = gem_table_get(_t3729, gem_string("bindings"));
#line 1380 "compiler/main.gem"
            GemVal gem_v__for_i_66 = gem_int(0);
#line 1380 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3730[] = {gem_v__for_items_66};
                if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t3730, 1)))) break;
#line 1380 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1380 "compiler/main.gem"
                gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1381 "compiler/main.gem"
    GemVal _t3731[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3732 = (*gem_v_compile_stmt);
    GemVal _t3733[] = {gem_v_b, _t3732.fn(_t3732.env, _t3731, 2)};
                (void)(gem_buf_push_fn(NULL, _t3733, 2));
#line 1382 "compiler/main.gem"
    GemVal _t3734[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3734, 2));
            }

        }
#line 1385 "compiler/main.gem"
    GemVal _t3735 = gem_v_arm;
    GemVal _t3736[] = {gem_table_get(_t3735, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3737 = (*gem_v_compile_stmts_tail);
    GemVal _t3738[] = {gem_v_b, _t3737.fn(_t3737.env, _t3736, 3)};
        (void)(gem_buf_push_fn(NULL, _t3738, 2));
    }

#line 1389 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1390 "compiler/main.gem"
    GemVal _t3739[] = {gem_v_p};
    GemVal _t3740[] = {gem_v_matched_tmp};
    GemVal _t3741[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3739, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3740, 1)), gem_string(" == -2) {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3741, 2));
#line 1391 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1392 "compiler/main.gem"
    GemVal _t3742[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3743 = (*gem_v_compile_stmts_tail);
    GemVal _t3744[] = {gem_v_b, _t3743.fn(_t3743.env, _t3742, 3)};
            (void)(gem_buf_push_fn(NULL, _t3744, 2));
        } else {
#line 1393 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1394 "compiler/main.gem"
    GemVal _t3745 = gem_table_new();
    GemVal _t3746[] = {_t3745, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3747 = (*gem_v_compile_stmts_tail);
    GemVal _t3748[] = {gem_v_b, _t3747.fn(_t3747.env, _t3746, 3)};
                (void)(gem_buf_push_fn(NULL, _t3748, 2));
            }
        }
    } else {
#line 1396 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1397 "compiler/main.gem"
    GemVal _t3749[] = {gem_v_p};
    GemVal _t3750[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3749, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3750, 2));
#line 1398 "compiler/main.gem"
    GemVal _t3751 = gem_table_new();
    GemVal _t3752[] = {_t3751, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3753 = (*gem_v_compile_stmts_tail);
    GemVal _t3754[] = {gem_v_b, _t3753.fn(_t3753.env, _t3752, 3)};
            (void)(gem_buf_push_fn(NULL, _t3754, 2));
        }
    }
#line 1401 "compiler/main.gem"
    GemVal _t3755[] = {gem_v_p};
    GemVal _t3756[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3755, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3756, 2));
#line 1402 "compiler/main.gem"
    GemVal _t3757[] = {gem_v_b};
    GemVal _t3758 = gem_buf_str_fn(NULL, _t3757, 1);
    gem_pop_frame();
    return _t3758;
}

struct _closure__anon_51 {
    GemVal *gem_v_extern_c_decls;
};
static GemVal _anon_51(void *_env, GemVal *args, int argc) {
    struct _closure__anon_51 *_cls = (struct _closure__anon_51 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_51", "compiler/main.gem", 0);
#line 1409 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1410 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1411 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1412 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1413 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1414 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1415 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1416 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1417 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1418 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1419 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1420 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1421 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1424 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1425 "compiler/main.gem"
    GemVal _t3760[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3760, 1), gem_int(0)))) {
#line 1426 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1428 "compiler/main.gem"
        GemVal gem_v__for_i_68 = gem_int(0);
#line 1428 "compiler/main.gem"
    GemVal _t3761[] = {gem_v_params};
        GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t3761, 1);
#line 1428 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1428 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_68;
#line 1428 "compiler/main.gem"
            gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1429 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1430 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1432 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1433 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1434 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1435 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1436 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1437 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1438 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1439 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1440 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1441 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1442 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1443 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1444 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1446 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1451 "compiler/main.gem"
    GemVal _t3762[] = {gem_v_c_ret};
    GemVal _t3763[] = {gem_v_name};
    GemVal _t3764[] = {gem_v_c_params};
    GemVal _t3765[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3762, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3763, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3764, 1)), gem_string(");"))};
    GemVal _t3766 = gem_push_fn(NULL, _t3765, 2);
    gem_pop_frame();
    return _t3766;
}

struct _closure__anon_52 {
    GemVal *gem_v_compile_blocking_extern_fn;
    GemVal *gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_52(void *_env, GemVal *args, int argc) {
    struct _closure__anon_52 *_cls = (struct _closure__anon_52 *)_env;
    GemVal *gem_v_compile_blocking_extern_fn = _cls->gem_v_compile_blocking_extern_fn;
    GemVal *gem_v_emit_extern_c_decl = _cls->gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_52", "compiler/main.gem", 0);
#line 1455 "compiler/main.gem"
    GemVal gem_v__d69 = gem_v_node;
#line 1455 "compiler/main.gem"
    GemVal _t3768 = gem_v__d69;
    GemVal gem_v_name = gem_table_get(_t3768, gem_string("name"));
#line 1455 "compiler/main.gem"
    GemVal _t3769 = gem_v__d69;
    GemVal gem_v_params = gem_table_get(_t3769, gem_string("params"));
#line 1455 "compiler/main.gem"
    GemVal _t3770 = gem_v__d69;
    GemVal gem_v_ret_type = gem_table_get(_t3770, gem_string("ret_type"));

#line 1457 "compiler/main.gem"
    GemVal _t3771 = gem_v_node;
    if (gem_truthy(gem_table_get(_t3771, gem_string("blocking")))) {
#line 1458 "compiler/main.gem"
    GemVal _t3772[] = {gem_v_node};
    GemVal _t3773 = (*gem_v_compile_blocking_extern_fn);
        GemVal _t3774 = _t3773.fn(_t3773.env, _t3772, 1);
        gem_pop_frame();
        return _t3774;
    }
#line 1461 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1462 "compiler/main.gem"
    GemVal _t3775[] = {gem_v_name};
    GemVal _t3776[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3775, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3776, 2));
#line 1464 "compiler/main.gem"
    GemVal _t3777 = gem_table_new();
    GemVal gem_v_c_args = _t3777;
#line 1465 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1465 "compiler/main.gem"
    GemVal _t3778[] = {gem_v_params};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3778, 1);
#line 1465 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1465 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1465 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1466 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1467 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1468 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1469 "compiler/main.gem"
    GemVal _t3779[] = {gem_v_i};
    GemVal _t3780[] = {gem_v_i};
    GemVal _t3781[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3779, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3780, 1)), gem_string("].ival;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3781, 2));
#line 1470 "compiler/main.gem"
    GemVal _t3782[] = {gem_v_i};
    GemVal _t3783[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3782, 1))};
            (void)(gem_push_fn(NULL, _t3783, 2));
        } else {
#line 1471 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1472 "compiler/main.gem"
    GemVal _t3784[] = {gem_v_i};
    GemVal _t3785[] = {gem_v_i};
    GemVal _t3786[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3784, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3785, 1)), gem_string("].fval;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3786, 2));
#line 1473 "compiler/main.gem"
    GemVal _t3787[] = {gem_v_i};
    GemVal _t3788[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3787, 1))};
                (void)(gem_push_fn(NULL, _t3788, 2));
            } else {
#line 1474 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1475 "compiler/main.gem"
    GemVal _t3789[] = {gem_v_i};
    GemVal _t3790[] = {gem_v_i};
    GemVal _t3791[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3789, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3790, 1)), gem_string("].sval;\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3791, 2));
#line 1476 "compiler/main.gem"
    GemVal _t3792[] = {gem_v_i};
    GemVal _t3793[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3792, 1))};
                    (void)(gem_push_fn(NULL, _t3793, 2));
                } else {
#line 1477 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1478 "compiler/main.gem"
    GemVal _t3794[] = {gem_v_i};
    GemVal _t3795[] = {gem_v_i};
    GemVal _t3796[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3794, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3795, 1)), gem_string("].bval;\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3796, 2));
#line 1479 "compiler/main.gem"
    GemVal _t3797[] = {gem_v_i};
    GemVal _t3798[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3797, 1))};
                        (void)(gem_push_fn(NULL, _t3798, 2));
                    } else {
#line 1480 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1481 "compiler/main.gem"
    GemVal _t3799[] = {gem_v_i};
    GemVal _t3800[] = {gem_v_i};
    GemVal _t3801[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3799, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3800, 1)), gem_string("].ival;\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3801, 2));
#line 1482 "compiler/main.gem"
    GemVal _t3802[] = {gem_v_i};
    GemVal _t3803[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3802, 1))};
                            (void)(gem_push_fn(NULL, _t3803, 2));
                        } else {
#line 1483 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1484 "compiler/main.gem"
    GemVal _t3804[] = {gem_v_i};
    GemVal _t3805[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3804, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3805, 2));
                            } else {
#line 1486 "compiler/main.gem"
    GemVal _t3806[] = {gem_v_i};
    GemVal _t3807[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3806, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3807, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1491 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1492 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1492 "compiler/main.gem"
    GemVal _t3808[] = {gem_v_c_args};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3808, 1);
#line 1492 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1492 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1492 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1493 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1494 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1496 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1499 "compiler/main.gem"
    GemVal _t3809[] = {gem_v_name};
    GemVal _t3810[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3809, 1), gem_string("(")), gem_to_string_fn(NULL, _t3810, 1)), gem_string(")"));
#line 1501 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1502 "compiler/main.gem"
    GemVal _t3811[] = {gem_v_call_expr};
    GemVal _t3812[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3811, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3812, 2));
#line 1503 "compiler/main.gem"
    GemVal _t3813[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3813, 2));
    } else {
#line 1504 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1505 "compiler/main.gem"
    GemVal _t3814[] = {gem_v_call_expr};
    GemVal _t3815[] = {gem_v_b, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t3814, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t3815, 2));
        } else {
#line 1506 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1507 "compiler/main.gem"
    GemVal _t3816[] = {gem_v_call_expr};
    GemVal _t3817[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3816, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t3817, 2));
#line 1508 "compiler/main.gem"
    GemVal _t3818[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3818, 2));
            } else {
#line 1509 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1510 "compiler/main.gem"
    GemVal _t3819[] = {gem_v_call_expr};
    GemVal _t3820[] = {gem_v_b, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3819, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3820, 2));
#line 1511 "compiler/main.gem"
    GemVal _t3821[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3821, 2));
                } else {
#line 1512 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1513 "compiler/main.gem"
    GemVal _t3822[] = {gem_v_call_expr};
    GemVal _t3823[] = {gem_v_b, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3822, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3823, 2));
#line 1514 "compiler/main.gem"
    GemVal _t3824[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3824, 2));
                    } else {
#line 1515 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1516 "compiler/main.gem"
    GemVal _t3825[] = {gem_v_call_expr};
    GemVal _t3826[] = {gem_v_b, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3825, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3826, 2));
#line 1517 "compiler/main.gem"
    GemVal _t3827[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                            (void)(gem_buf_push_fn(NULL, _t3827, 2));
                        } else {
#line 1518 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1519 "compiler/main.gem"
    GemVal _t3828[] = {gem_v_call_expr};
    GemVal _t3829[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3828, 1)), gem_string(";\n"))};
                                (void)(gem_buf_push_fn(NULL, _t3829, 2));
#line 1520 "compiler/main.gem"
    GemVal _t3830[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3830, 2));
                            } else {
#line 1522 "compiler/main.gem"
    GemVal _t3831[] = {gem_v_call_expr};
    GemVal _t3832[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3831, 1)), gem_string(";\n"))};
                                (void)(gem_buf_push_fn(NULL, _t3832, 2));
#line 1523 "compiler/main.gem"
    GemVal _t3833[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3833, 2));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1526 "compiler/main.gem"
    GemVal _t3834[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3834, 2));
#line 1528 "compiler/main.gem"
    GemVal _t3835[] = {gem_v_name};
    GemVal _t3836[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3835, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t3836, 2));
#line 1529 "compiler/main.gem"
    GemVal _t3837[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3838 = (*gem_v_emit_extern_c_decl);
    (void)(_t3838.fn(_t3838.env, _t3837, 3));
#line 1531 "compiler/main.gem"
    GemVal _t3839[] = {gem_v_b};
    GemVal _t3840 = gem_buf_str_fn(NULL, _t3839, 1);
    gem_pop_frame();
    return _t3840;
}

struct _closure__anon_53 {
    GemVal *gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_53(void *_env, GemVal *args, int argc) {
    struct _closure__anon_53 *_cls = (struct _closure__anon_53 *)_env;
    GemVal *gem_v_emit_extern_c_decl = _cls->gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_53", "compiler/main.gem", 0);
#line 1538 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1538 "compiler/main.gem"
    GemVal _t3842 = gem_v__d72;
    GemVal gem_v_name = gem_table_get(_t3842, gem_string("name"));
#line 1538 "compiler/main.gem"
    GemVal _t3843 = gem_v__d72;
    GemVal gem_v_params = gem_table_get(_t3843, gem_string("params"));
#line 1538 "compiler/main.gem"
    GemVal _t3844 = gem_v__d72;
    GemVal gem_v_ret_type = gem_table_get(_t3844, gem_string("ret_type"));

#line 1539 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1542 "compiler/main.gem"
    GemVal _t3845[] = {gem_v_name};
    GemVal _t3846[] = {gem_v_b, gem_add(gem_add(gem_string("struct _blk_"), gem_to_string_fn(NULL, _t3845, 1)), gem_string("_args {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3846, 2));
#line 1543 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1543 "compiler/main.gem"
    GemVal _t3847[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t3847, 1);
#line 1543 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1543 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1543 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1544 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1545 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1546 "compiler/main.gem"
    GemVal _t3848[] = {gem_v_i};
    GemVal _t3849[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3848, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t3849, 2));
        } else {
#line 1547 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1548 "compiler/main.gem"
    GemVal _t3850[] = {gem_v_i};
    GemVal _t3851[] = {gem_v_b, gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3850, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t3851, 2));
            } else {
#line 1549 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1550 "compiler/main.gem"
    GemVal _t3852[] = {gem_v_i};
    GemVal _t3853[] = {gem_v_b, gem_add(gem_add(gem_string("    char *_p"), gem_to_string_fn(NULL, _t3852, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3853, 2));
                } else {
#line 1551 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1552 "compiler/main.gem"
    GemVal _t3854[] = {gem_v_i};
    GemVal _t3855[] = {gem_v_b, gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3854, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3855, 2));
                    } else {
#line 1553 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1554 "compiler/main.gem"
    GemVal _t3856[] = {gem_v_i};
    GemVal _t3857[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_p"), gem_to_string_fn(NULL, _t3856, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3857, 2));
                        }
                    }
                }
            }
        }
    }

#line 1557 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1558 "compiler/main.gem"
    GemVal _t3858[] = {gem_v_b, gem_string("    int64_t _result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3858, 2));
    } else {
#line 1559 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1560 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_b, gem_string("    double _result;\n")};
            (void)(gem_buf_push_fn(NULL, _t3859, 2));
        } else {
#line 1561 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1562 "compiler/main.gem"
    GemVal _t3860[] = {gem_v_b, gem_string("    char *_result;\n")};
                (void)(gem_buf_push_fn(NULL, _t3860, 2));
            } else {
#line 1563 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1564 "compiler/main.gem"
    GemVal _t3861[] = {gem_v_b, gem_string("    int _result;\n")};
                    (void)(gem_buf_push_fn(NULL, _t3861, 2));
                } else {
#line 1565 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1566 "compiler/main.gem"
    GemVal _t3862[] = {gem_v_b, gem_string("    void *_result;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3862, 2));
                    }
                }
            }
        }
    }
#line 1568 "compiler/main.gem"
    GemVal _t3863[] = {gem_v_b, gem_string("};\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3863, 2));
#line 1571 "compiler/main.gem"
    GemVal _t3864[] = {gem_v_name};
    GemVal _t3865[] = {gem_v_b, gem_add(gem_add(gem_string("static void _blk_"), gem_to_string_fn(NULL, _t3864, 1)), gem_string("_worker(void *_a) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3865, 2));
#line 1572 "compiler/main.gem"
    GemVal _t3866[] = {gem_v_name};
    GemVal _t3867[] = {gem_v_name};
    GemVal _t3868[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    struct _blk_"), gem_to_string_fn(NULL, _t3866, 1)), gem_string("_args *a = (struct _blk_")), gem_to_string_fn(NULL, _t3867, 1)), gem_string("_args *)_a;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3868, 2));
#line 1574 "compiler/main.gem"
    GemVal gem_v_w_args = gem_string("");
#line 1575 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1575 "compiler/main.gem"
    GemVal _t3869[] = {gem_v_params};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3869, 1);
#line 1575 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1575 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1575 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1576 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1577 "compiler/main.gem"
            gem_v_w_args = gem_add(gem_v_w_args, gem_string(", "));
        }
#line 1579 "compiler/main.gem"
    GemVal _t3870[] = {gem_v_i};
        gem_v_w_args = gem_add(gem_v_w_args, gem_add(gem_string("a->_p"), gem_to_string_fn(NULL, _t3870, 1)));
    }

#line 1581 "compiler/main.gem"
    GemVal _t3871[] = {gem_v_name};
    GemVal _t3872[] = {gem_v_w_args};
    GemVal gem_v_w_call = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3871, 1), gem_string("(")), gem_to_string_fn(NULL, _t3872, 1)), gem_string(")"));
#line 1583 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1584 "compiler/main.gem"
    GemVal _t3873[] = {gem_v_w_call};
    GemVal _t3874[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3873, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3874, 2));
    } else {
#line 1586 "compiler/main.gem"
    GemVal _t3875[] = {gem_v_w_call};
    GemVal _t3876[] = {gem_v_b, gem_add(gem_add(gem_string("    a->_result = "), gem_to_string_fn(NULL, _t3875, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3876, 2));
    }
#line 1588 "compiler/main.gem"
    GemVal _t3877[] = {gem_v_b, gem_string("}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3877, 2));
#line 1591 "compiler/main.gem"
    GemVal _t3878[] = {gem_v_name};
    GemVal _t3879[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3878, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3879, 2));
#line 1594 "compiler/main.gem"
    GemVal gem_v__for_i_75 = gem_int(0);
#line 1594 "compiler/main.gem"
    GemVal _t3880[] = {gem_v_params};
    GemVal gem_v__for_limit_75 = gem_len_fn(NULL, _t3880, 1);
#line 1594 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1594 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_75;
#line 1594 "compiler/main.gem"
        gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1595 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1596 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1597 "compiler/main.gem"
    GemVal _t3881[] = {gem_v_i};
    GemVal _t3882[] = {gem_v_i};
    GemVal _t3883[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3881, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3882, 1)), gem_string("].ival;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3883, 2));
        } else {
#line 1598 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1599 "compiler/main.gem"
    GemVal _t3884[] = {gem_v_i};
    GemVal _t3885[] = {gem_v_i};
    GemVal _t3886[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3884, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3885, 1)), gem_string("].fval;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3886, 2));
            } else {
#line 1600 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1601 "compiler/main.gem"
    GemVal _t3887[] = {gem_v_i};
    GemVal _t3888[] = {gem_v_i};
    GemVal _t3889[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3887, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3888, 1)), gem_string("].sval;\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3889, 2));
                } else {
#line 1602 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1603 "compiler/main.gem"
    GemVal _t3890[] = {gem_v_i};
    GemVal _t3891[] = {gem_v_i};
    GemVal _t3892[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3890, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3891, 1)), gem_string("].bval;\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3892, 2));
                    } else {
#line 1604 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1605 "compiler/main.gem"
    GemVal _t3893[] = {gem_v_i};
    GemVal _t3894[] = {gem_v_i};
    GemVal _t3895[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3893, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3894, 1)), gem_string("].ival;\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3895, 2));
                        }
                    }
                }
            }
        }
    }

#line 1610 "compiler/main.gem"
    GemVal _t3896[] = {gem_v_b, gem_string("    if (gem_current_pid >= 0) {\n")};
    (void)(gem_buf_push_fn(NULL, _t3896, 2));
#line 1611 "compiler/main.gem"
    GemVal _t3897[] = {gem_v_name};
    GemVal _t3898[] = {gem_v_name};
    GemVal _t3899[] = {gem_v_name};
    GemVal _t3900[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("        struct _blk_"), gem_to_string_fn(NULL, _t3897, 1)), gem_string("_args *_ba = (struct _blk_")), gem_to_string_fn(NULL, _t3898, 1)), gem_string("_args *)malloc(sizeof(struct _blk_")), gem_to_string_fn(NULL, _t3899, 1)), gem_string("_args));\n"))};
    (void)(gem_buf_push_fn(NULL, _t3900, 2));
#line 1614 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1614 "compiler/main.gem"
    GemVal _t3901[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t3901, 1);
#line 1614 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1614 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1614 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1615 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1616 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1617 "compiler/main.gem"
    GemVal _t3902[] = {gem_v_i};
    GemVal _t3903[] = {gem_v_i};
    GemVal _t3904[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        _ba->_p"), gem_to_string_fn(NULL, _t3902, 1)), gem_string(" = strdup(_p")), gem_to_string_fn(NULL, _t3903, 1)), gem_string(");\n"))};
            (void)(gem_buf_push_fn(NULL, _t3904, 2));
        } else {
#line 1619 "compiler/main.gem"
    GemVal _t3905[] = {gem_v_i};
    GemVal _t3906[] = {gem_v_i};
    GemVal _t3907[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        _ba->_p"), gem_to_string_fn(NULL, _t3905, 1)), gem_string(" = _p")), gem_to_string_fn(NULL, _t3906, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t3907, 2));
        }
    }

#line 1623 "compiler/main.gem"
    GemVal _t3908[] = {gem_v_name};
    GemVal _t3909[] = {gem_v_b, gem_add(gem_add(gem_string("        GemIORequest *_req = gem_io_submit_extern(_blk_"), gem_to_string_fn(NULL, _t3908, 1)), gem_string("_worker, _ba);\n"))};
    (void)(gem_buf_push_fn(NULL, _t3909, 2));
#line 1624 "compiler/main.gem"
    GemVal _t3910[] = {gem_v_name};
    GemVal _t3911[] = {gem_v_b, gem_add(gem_add(gem_string("        if (!_req) { free(_ba); gem_error(\""), gem_to_string_fn(NULL, _t3910, 1)), gem_string(": I/O queue full\"); }\n"))};
    (void)(gem_buf_push_fn(NULL, _t3911, 2));
#line 1625 "compiler/main.gem"
    GemVal _t3912[] = {gem_v_b, gem_string("        GemProcess *_proc = &gem_proc_table[gem_current_pid];\n")};
    (void)(gem_buf_push_fn(NULL, _t3912, 2));
#line 1626 "compiler/main.gem"
    GemVal _t3913[] = {gem_v_b, gem_string("        _proc->io_request = _req;\n")};
    (void)(gem_buf_push_fn(NULL, _t3913, 2));
#line 1627 "compiler/main.gem"
    GemVal _t3914[] = {gem_v_b, gem_string("        gem_io_pool_yield();\n")};
    (void)(gem_buf_push_fn(NULL, _t3914, 2));
#line 1628 "compiler/main.gem"
    GemVal _t3915[] = {gem_v_b, gem_string("        _proc->io_request = NULL;\n")};
    (void)(gem_buf_push_fn(NULL, _t3915, 2));
#line 1631 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1632 "compiler/main.gem"
    GemVal _t3916[] = {gem_v_b, gem_string("        char *_raw = _ba->_result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3916, 2));
    } else {
#line 1633 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_ret_type, gem_string("Nil")))) {
#line 1634 "compiler/main.gem"
            GemVal gem_v_c_type = gem_string("int64_t");
#line 1635 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1636 "compiler/main.gem"
                gem_v_c_type = gem_string("double");
            } else {
#line 1637 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1638 "compiler/main.gem"
                    gem_v_c_type = gem_string("int");
                } else {
#line 1639 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1640 "compiler/main.gem"
                        gem_v_c_type = gem_string("void *");
                    }
                }
            }
#line 1642 "compiler/main.gem"
    GemVal _t3917[] = {gem_v_c_type};
    GemVal _t3918[] = {gem_v_b, gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3917, 1)), gem_string(" _ret = _ba->_result;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3918, 2));
        }
    }
#line 1646 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1646 "compiler/main.gem"
    GemVal _t3919[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t3919, 1);
#line 1646 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1646 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1646 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1647 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type")), gem_string("String")))) {
#line 1648 "compiler/main.gem"
    GemVal _t3920[] = {gem_v_i};
    GemVal _t3921[] = {gem_v_b, gem_add(gem_add(gem_string("        free(_ba->_p"), gem_to_string_fn(NULL, _t3920, 1)), gem_string(");\n"))};
            (void)(gem_buf_push_fn(NULL, _t3921, 2));
        }
    }

#line 1652 "compiler/main.gem"
    GemVal _t3922[] = {gem_v_b, gem_string("        free(_ba);\n")};
    (void)(gem_buf_push_fn(NULL, _t3922, 2));
#line 1653 "compiler/main.gem"
    GemVal _t3923[] = {gem_v_b, gem_string("        gem_io_free_request(_req);\n")};
    (void)(gem_buf_push_fn(NULL, _t3923, 2));
#line 1656 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1657 "compiler/main.gem"
    GemVal _t3924[] = {gem_v_b, gem_string("        return gem_int(_ret);\n")};
        (void)(gem_buf_push_fn(NULL, _t3924, 2));
    } else {
#line 1658 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1659 "compiler/main.gem"
    GemVal _t3925[] = {gem_v_b, gem_string("        return gem_float(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3925, 2));
        } else {
#line 1660 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1661 "compiler/main.gem"
    GemVal _t3926[] = {gem_v_b, gem_string("        size_t _len = _raw ? strlen(_raw) : 0;\n")};
                (void)(gem_buf_push_fn(NULL, _t3926, 2));
#line 1662 "compiler/main.gem"
    GemVal _t3927[] = {gem_v_b, gem_string("        char *_gc = (char *)GC_MALLOC_ATOMIC(_len + 1);\n")};
                (void)(gem_buf_push_fn(NULL, _t3927, 2));
#line 1663 "compiler/main.gem"
    GemVal _t3928[] = {gem_v_b, gem_string("        if (_raw) { memcpy(_gc, _raw, _len + 1); free(_raw); } else { _gc[0] = '\\0'; }\n")};
                (void)(gem_buf_push_fn(NULL, _t3928, 2));
#line 1664 "compiler/main.gem"
    GemVal _t3929[] = {gem_v_b, gem_string("        return gem_string(_gc);\n")};
                (void)(gem_buf_push_fn(NULL, _t3929, 2));
            } else {
#line 1665 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1666 "compiler/main.gem"
    GemVal _t3930[] = {gem_v_b, gem_string("        return gem_bool(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3930, 2));
                } else {
#line 1667 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1668 "compiler/main.gem"
    GemVal _t3931[] = {gem_v_b, gem_string("        GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3931, 2));
                    } else {
#line 1670 "compiler/main.gem"
    GemVal _t3932[] = {gem_v_b, gem_string("        return GEM_NIL;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3932, 2));
                    }
                }
            }
        }
    }
#line 1672 "compiler/main.gem"
    GemVal _t3933[] = {gem_v_b, gem_string("    }\n")};
    (void)(gem_buf_push_fn(NULL, _t3933, 2));
#line 1675 "compiler/main.gem"
    GemVal gem_v_d_args = gem_string("");
#line 1676 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1676 "compiler/main.gem"
    GemVal _t3934[] = {gem_v_params};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t3934, 1);
#line 1676 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1676 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1676 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1677 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1678 "compiler/main.gem"
            gem_v_d_args = gem_add(gem_v_d_args, gem_string(", "));
        }
#line 1680 "compiler/main.gem"
    GemVal _t3935[] = {gem_v_i};
        gem_v_d_args = gem_add(gem_v_d_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3935, 1)));
    }

#line 1682 "compiler/main.gem"
    GemVal _t3936[] = {gem_v_name};
    GemVal _t3937[] = {gem_v_d_args};
    GemVal gem_v_d_call = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3936, 1), gem_string("(")), gem_to_string_fn(NULL, _t3937, 1)), gem_string(")"));
#line 1684 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1685 "compiler/main.gem"
    GemVal _t3938[] = {gem_v_d_call};
    GemVal _t3939[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3938, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3939, 2));
#line 1686 "compiler/main.gem"
    GemVal _t3940[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3940, 2));
    } else {
#line 1687 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1688 "compiler/main.gem"
    GemVal _t3941[] = {gem_v_d_call};
    GemVal _t3942[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3941, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t3942, 2));
#line 1689 "compiler/main.gem"
    GemVal _t3943[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3943, 2));
        } else {
#line 1690 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1691 "compiler/main.gem"
    GemVal _t3944[] = {gem_v_d_call};
    GemVal _t3945[] = {gem_v_b, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3944, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t3945, 2));
#line 1692 "compiler/main.gem"
    GemVal _t3946[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3946, 2));
            } else {
#line 1693 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1694 "compiler/main.gem"
    GemVal _t3947[] = {gem_v_d_call};
    GemVal _t3948[] = {gem_v_b, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3947, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3948, 2));
#line 1695 "compiler/main.gem"
    GemVal _t3949[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3949, 2));
                } else {
#line 1696 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1697 "compiler/main.gem"
    GemVal _t3950[] = {gem_v_d_call};
    GemVal _t3951[] = {gem_v_b, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3950, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3951, 2));
#line 1698 "compiler/main.gem"
    GemVal _t3952[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3952, 2));
                    } else {
#line 1699 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1700 "compiler/main.gem"
    GemVal _t3953[] = {gem_v_d_call};
    GemVal _t3954[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3953, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3954, 2));
#line 1701 "compiler/main.gem"
    GemVal _t3955[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3955, 2));
                        } else {
#line 1703 "compiler/main.gem"
    GemVal _t3956[] = {gem_v_d_call};
    GemVal _t3957[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3956, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3957, 2));
#line 1704 "compiler/main.gem"
    GemVal _t3958[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3958, 2));
                        }
                    }
                }
            }
        }
    }
#line 1707 "compiler/main.gem"
    GemVal _t3959[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3959, 2));
#line 1709 "compiler/main.gem"
    GemVal _t3960[] = {gem_v_name};
    GemVal _t3961[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3960, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t3961, 2));
#line 1710 "compiler/main.gem"
    GemVal _t3962[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3963 = (*gem_v_emit_extern_c_decl);
    (void)(_t3963.fn(_t3963.env, _t3962, 3));
#line 1712 "compiler/main.gem"
    GemVal _t3964[] = {gem_v_b};
    GemVal _t3965 = gem_buf_str_fn(NULL, _t3964, 1);
    gem_pop_frame();
    return _t3965;
}

struct _closure__anon_54 {
    GemVal *gem_v_mangle;
};
static GemVal _anon_54(void *_env, GemVal *args, int argc) {
    struct _closure__anon_54 *_cls = (struct _closure__anon_54 *)_env;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_cap_set = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_54", "compiler/main.gem", 0);
#line 1718 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1720 "compiler/main.gem"
    GemVal gem_v__for_i_79 = gem_int(0);
#line 1720 "compiler/main.gem"
    GemVal _t3967[] = {gem_v_params};
    GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t3967, 1);
#line 1720 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1720 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_79;
#line 1720 "compiler/main.gem"
        gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1721 "compiler/main.gem"
    GemVal _t3968[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3969 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3969.fn(_t3969.env, _t3968, 1);
#line 1722 "compiler/main.gem"
    GemVal _t3970[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3970, 2))) {
#line 1723 "compiler/main.gem"
    GemVal _t3971[] = {gem_v_mp};
    GemVal _t3972[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3971, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
            (void)(gem_buf_push_fn(NULL, _t3972, 2));
#line 1724 "compiler/main.gem"
    GemVal _t3973[] = {gem_v_mp};
    GemVal _t3974[] = {gem_v_i};
    GemVal _t3975[] = {gem_v_i};
    GemVal _t3976[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3973, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3974, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3975, 1)), gem_string("] : GEM_NIL;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3976, 2));
        } else {
#line 1726 "compiler/main.gem"
    GemVal _t3977[] = {gem_v_mp};
    GemVal _t3978[] = {gem_v_i};
    GemVal _t3979[] = {gem_v_i};
    GemVal _t3980[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3977, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3978, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3979, 1)), gem_string("] : GEM_NIL;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3980, 2));
        }
    }

#line 1731 "compiler/main.gem"
    GemVal _t3981 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3981, gem_string("rest_param")), GEM_NIL))) {
#line 1732 "compiler/main.gem"
    GemVal _t3982[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3982, 1);
#line 1733 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1734 "compiler/main.gem"
    GemVal _t3983 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3983, gem_string("block_param")), GEM_NIL))) {
#line 1735 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1737 "compiler/main.gem"
    GemVal _t3984 = gem_v_node;
    GemVal _t3985[] = {gem_table_get(_t3984, gem_string("rest_param"))};
    GemVal _t3986 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3986.fn(_t3986.env, _t3985, 1);
#line 1738 "compiler/main.gem"
    GemVal _t3987 = gem_v_node;
    GemVal _t3988[] = {gem_v_cap_set, gem_table_get(_t3987, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3988, 2))) {
#line 1739 "compiler/main.gem"
    GemVal _t3989[] = {gem_v_mp_rest};
    GemVal _t3990[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3989, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
            (void)(gem_buf_push_fn(NULL, _t3990, 2));
#line 1740 "compiler/main.gem"
    GemVal _t3991[] = {gem_v_mp_rest};
    GemVal _t3992[] = {gem_v_b, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3991, 1)), gem_string(" = gem_table_new();\n"))};
            (void)(gem_buf_push_fn(NULL, _t3992, 2));
#line 1741 "compiler/main.gem"
    GemVal _t3993[] = {gem_v_N};
    GemVal _t3994[] = {gem_v_end_count};
    GemVal _t3995[] = {gem_v_mp_rest};
    GemVal _t3996[] = {gem_v_N};
    GemVal _t3997[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3993, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3994, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t3995, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3996, 1)), gem_string("), args[_ri]);\n"))};
            (void)(gem_buf_push_fn(NULL, _t3997, 2));
        } else {
#line 1743 "compiler/main.gem"
    GemVal _t3998[] = {gem_v_mp_rest};
    GemVal _t3999[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3998, 1)), gem_string(" = gem_table_new();\n"))};
            (void)(gem_buf_push_fn(NULL, _t3999, 2));
#line 1744 "compiler/main.gem"
    GemVal _t4000[] = {gem_v_N};
    GemVal _t4001[] = {gem_v_end_count};
    GemVal _t4002[] = {gem_v_mp_rest};
    GemVal _t4003[] = {gem_v_N};
    GemVal _t4004[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t4000, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t4001, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t4002, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t4003, 1)), gem_string("), args[_ri]);\n"))};
            (void)(gem_buf_push_fn(NULL, _t4004, 2));
        }
#line 1746 "compiler/main.gem"
    GemVal _t4005 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4005, gem_string("block_param")), GEM_NIL))) {
#line 1747 "compiler/main.gem"
    GemVal _t4006 = gem_v_node;
    GemVal _t4007[] = {gem_table_get(_t4006, gem_string("block_param"))};
    GemVal _t4008 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t4008.fn(_t4008.env, _t4007, 1);
#line 1748 "compiler/main.gem"
    GemVal _t4009 = gem_v_node;
    GemVal _t4010[] = {gem_v_cap_set, gem_table_get(_t4009, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t4010, 2))) {
#line 1749 "compiler/main.gem"
    GemVal _t4011[] = {gem_v_mp_block};
    GemVal _t4012[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4011, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
                (void)(gem_buf_push_fn(NULL, _t4012, 2));
#line 1750 "compiler/main.gem"
    GemVal _t4013[] = {gem_v_mp_block};
    GemVal _t4014[] = {gem_v_b, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t4013, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t4014, 2));
            } else {
#line 1752 "compiler/main.gem"
    GemVal _t4015[] = {gem_v_mp_block};
    GemVal _t4016[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t4015, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t4016, 2));
            }
        }
    }
#line 1756 "compiler/main.gem"
    GemVal _t4017[] = {gem_v_b};
    GemVal _t4018 = gem_buf_str_fn(NULL, _t4017, 1);
    gem_pop_frame();
    return _t4018;
}

struct _closure__anon_55 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_55(void *_env, GemVal *args, int argc) {
    struct _closure__anon_55 *_cls = (struct _closure__anon_55 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_55", "compiler/main.gem", 0);
#line 1762 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1763 "compiler/main.gem"
    GemVal _t4020[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4020, 1), gem_int(0)))) {
#line 1764 "compiler/main.gem"
        GemVal gem_v__for_i_80 = gem_int(0);
#line 1764 "compiler/main.gem"
    GemVal _t4021[] = {gem_v_body};
        GemVal gem_v__for_limit_80 = gem_sub(gem_len_fn(NULL, _t4021, 1), gem_int(1));
#line 1764 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1764 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_80;
#line 1764 "compiler/main.gem"
            gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1765 "compiler/main.gem"
    GemVal _t4022[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t4023 = (*gem_v_compile_stmt);
    GemVal _t4024[] = {gem_v_b, _t4023.fn(_t4023.env, _t4022, 2)};
            (void)(gem_buf_push_fn(NULL, _t4024, 2));
#line 1766 "compiler/main.gem"
    GemVal _t4025[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4025, 2));
        }

#line 1768 "compiler/main.gem"
    GemVal _t4026[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t4027 = (*gem_v_compile_stmt_return);
    GemVal _t4028[] = {gem_v_b, _t4027.fn(_t4027.env, _t4026, 2)};
        (void)(gem_buf_push_fn(NULL, _t4028, 2));
#line 1769 "compiler/main.gem"
    GemVal _t4029[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t4029, 2));
    } else {
#line 1771 "compiler/main.gem"
    GemVal _t4030[] = {gem_v_b, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4030, 2));
    }
#line 1773 "compiler/main.gem"
    GemVal _t4031[] = {gem_v_b};
    GemVal _t4032 = gem_buf_str_fn(NULL, _t4031, 1);
    gem_pop_frame();
    return _t4032;
}

struct _closure__anon_56 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope;
    GemVal *gem_v_has_self_tail_call;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_local_names;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tco_captured;
    GemVal *gem_v_tco_fn_name;
    GemVal *gem_v_tco_params;
};
static GemVal _anon_56(void *_env, GemVal *args, int argc) {
    struct _closure__anon_56 *_cls = (struct _closure__anon_56 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = _cls->gem_v_emit_fn_body;
    GemVal *gem_v_emit_param_bindings = _cls->gem_v_emit_param_bindings;
    GemVal *gem_v_find_captured_in_scope = _cls->gem_v_find_captured_in_scope;
    GemVal *gem_v_has_self_tail_call = _cls->gem_v_has_self_tail_call;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tco_captured = _cls->gem_v_tco_captured;
    GemVal *gem_v_tco_fn_name = _cls->gem_v_tco_fn_name;
    GemVal *gem_v_tco_params = _cls->gem_v_tco_params;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_56", "compiler/main.gem", 0);
#line 1779 "compiler/main.gem"
    GemVal gem_v__d81 = gem_v_node;
#line 1779 "compiler/main.gem"
    GemVal _t4034 = gem_v__d81;
    GemVal gem_v_params = gem_table_get(_t4034, gem_string("params"));
#line 1779 "compiler/main.gem"
    GemVal _t4035 = gem_v__d81;
    GemVal gem_v_body = gem_table_get(_t4035, gem_string("body"));
#line 1779 "compiler/main.gem"
    GemVal _t4036 = gem_v__d81;
    GemVal gem_v_name = gem_table_get(_t4036, gem_string("name"));

#line 1782 "compiler/main.gem"
    GemVal _t4037[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t4037, 1);
#line 1783 "compiler/main.gem"
    GemVal _t4038 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4038, gem_string("rest_param")), GEM_NIL))) {
#line 1784 "compiler/main.gem"
    GemVal _t4039 = gem_v_node;
    GemVal _t4040[] = {gem_v_scope_vars, gem_table_get(_t4039, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t4040, 2));
    }
#line 1786 "compiler/main.gem"
    GemVal _t4041 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4041, gem_string("block_param")), GEM_NIL))) {
#line 1787 "compiler/main.gem"
    GemVal _t4042 = gem_v_node;
    GemVal _t4043[] = {gem_v_scope_vars, gem_table_get(_t4042, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t4043, 2));
    }
#line 1789 "compiler/main.gem"
    GemVal _t4044[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn_collect_top_let_names(NULL, _t4044, 2));
#line 1791 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1792 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1793 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1794 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1796 "compiler/main.gem"
    GemVal _t4045[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t4046 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t4046.fn(_t4046.env, _t4045, 2);
#line 1798 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1799 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1801 "compiler/main.gem"
    GemVal gem_v_use_tco = gem_bool(0);
#line 1802 "compiler/main.gem"
    GemVal _t4047 = gem_v_node;
    GemVal _t4049;
    if (!gem_truthy(gem_eq(gem_table_get(_t4047, gem_string("rest_param")), GEM_NIL))) {
        _t4049 = gem_eq(gem_table_get(_t4047, gem_string("rest_param")), GEM_NIL);
    } else {
        GemVal _t4048 = gem_v_node;
        _t4049 = gem_eq(gem_table_get(_t4048, gem_string("block_param")), GEM_NIL);
    }
    GemVal _t4051;
    if (!gem_truthy(_t4049)) {
        _t4051 = _t4049;
    } else {
        GemVal _t4050[] = {gem_v_scope_vars, gem_v_name};
        _t4051 = gem_not(gem_fn_set_contains(NULL, _t4050, 2));
    }
    if (gem_truthy(_t4051)) {
#line 1803 "compiler/main.gem"
    GemVal _t4052[] = {gem_v_name, gem_v_body};
    GemVal _t4053 = (*gem_v_has_self_tail_call);
        gem_v_use_tco = _t4053.fn(_t4053.env, _t4052, 2);
    }
#line 1805 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1806 "compiler/main.gem"
        *gem_v_tco_fn_name = gem_v_name;
#line 1807 "compiler/main.gem"
        *gem_v_tco_params = gem_v_params;
#line 1808 "compiler/main.gem"
        *gem_v_tco_captured = gem_v_captured;
    }
#line 1811 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1812 "compiler/main.gem"
    GemVal _t4054[] = {gem_v_name};
    GemVal _t4055[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4054, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4055, 2));
#line 1814 "compiler/main.gem"
    GemVal _t4056 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4056, gem_string("line")), GEM_NIL))) {
#line 1815 "compiler/main.gem"
    GemVal _t4057 = gem_v_node;
    GemVal _t4058[] = {gem_table_get(_t4057, gem_string("line"))};
    GemVal _t4059[] = {(*gem_v_source_name)};
    GemVal _t4060[] = {gem_fn_escape_c_string(NULL, _t4059, 1)};
    GemVal _t4061[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t4058, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t4060, 1)), gem_string("\"\n"))};
        (void)(gem_buf_push_fn(NULL, _t4061, 2));
    }
#line 1818 "compiler/main.gem"
    GemVal _t4062[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t4063 = (*gem_v_emit_param_bindings);
    GemVal _t4064[] = {gem_v_b, _t4063.fn(_t4063.env, _t4062, 3)};
    (void)(gem_buf_push_fn(NULL, _t4064, 2));
#line 1820 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1821 "compiler/main.gem"
    GemVal _t4065 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4065, gem_string("line")), GEM_NIL))) {
#line 1822 "compiler/main.gem"
    GemVal _t4066 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t4066, gem_string("line"));
    }
#line 1824 "compiler/main.gem"
    GemVal _t4067[] = {gem_v_name};
    GemVal _t4068[] = {gem_fn_escape_c_string(NULL, _t4067, 1)};
    GemVal _t4069[] = {(*gem_v_source_name)};
    GemVal _t4070[] = {gem_fn_escape_c_string(NULL, _t4069, 1)};
    GemVal _t4071[] = {gem_v_fn_line};
    GemVal _t4072[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t4068, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t4070, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t4071, 1)), gem_string(");\n"))};
    (void)(gem_buf_push_fn(NULL, _t4072, 2));
#line 1826 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1827 "compiler/main.gem"
    GemVal _t4073[] = {gem_v_b, gem_string("    while(1) {\n")};
        (void)(gem_buf_push_fn(NULL, _t4073, 2));
#line 1828 "compiler/main.gem"
    GemVal _t4074[] = {gem_v_b, gem_string("        gem_yield_check();\n")};
        (void)(gem_buf_push_fn(NULL, _t4074, 2));
#line 1829 "compiler/main.gem"
    GemVal _t4075[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4075, 1), gem_int(0)))) {
#line 1830 "compiler/main.gem"
            GemVal gem_v__for_i_82 = gem_int(0);
#line 1830 "compiler/main.gem"
    GemVal _t4076[] = {gem_v_body};
            GemVal gem_v__for_limit_82 = gem_sub(gem_len_fn(NULL, _t4076, 1), gem_int(1));
#line 1830 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_v__for_limit_82))) break;
#line 1830 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_82;
#line 1830 "compiler/main.gem"
                gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1831 "compiler/main.gem"
    GemVal _t4077[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(2)};
    GemVal _t4078 = (*gem_v_compile_stmt);
    GemVal _t4079[] = {gem_v_b, _t4078.fn(_t4078.env, _t4077, 2)};
                (void)(gem_buf_push_fn(NULL, _t4079, 2));
#line 1832 "compiler/main.gem"
    GemVal _t4080[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t4080, 2));
            }

#line 1834 "compiler/main.gem"
    GemVal _t4081[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(2)};
    GemVal _t4082 = (*gem_v_compile_stmt_return);
    GemVal _t4083[] = {gem_v_b, _t4082.fn(_t4082.env, _t4081, 2)};
            (void)(gem_buf_push_fn(NULL, _t4083, 2));
#line 1835 "compiler/main.gem"
    GemVal _t4084[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4084, 2));
        } else {
#line 1837 "compiler/main.gem"
    GemVal _t4085[] = {gem_v_b, gem_string("        gem_pop_frame();\n        return GEM_NIL;\n")};
            (void)(gem_buf_push_fn(NULL, _t4085, 2));
        }
#line 1839 "compiler/main.gem"
    GemVal _t4086[] = {gem_v_b, gem_string("    }\n")};
        (void)(gem_buf_push_fn(NULL, _t4086, 2));
    } else {
#line 1841 "compiler/main.gem"
    GemVal _t4087[] = {gem_v_body};
    GemVal _t4088 = (*gem_v_emit_fn_body);
    GemVal _t4089[] = {gem_v_b, _t4088.fn(_t4088.env, _t4087, 1)};
        (void)(gem_buf_push_fn(NULL, _t4089, 2));
    }
#line 1844 "compiler/main.gem"
    GemVal _t4090[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4090, 2));
#line 1846 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1847 "compiler/main.gem"
        *gem_v_tco_fn_name = GEM_NIL;
#line 1848 "compiler/main.gem"
        *gem_v_tco_params = GEM_NIL;
#line 1849 "compiler/main.gem"
        *gem_v_tco_captured = GEM_NIL;
    }
#line 1852 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1853 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1854 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1855 "compiler/main.gem"
    GemVal _t4091[] = {gem_v_b};
    GemVal _t4092 = gem_buf_str_fn(NULL, _t4091, 1);
    gem_pop_frame();
    return _t4092;
}

struct _closure__anon_57 {
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
static GemVal _anon_57(void *_env, GemVal *args, int argc) {
    struct _closure__anon_57 *_cls = (struct _closure__anon_57 *)_env;
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
    gem_push_frame("_anon_57", "compiler/main.gem", 0);
#line 1861 "compiler/main.gem"
    GemVal gem_v__d83 = gem_v_node;
#line 1861 "compiler/main.gem"
    GemVal _t4094 = gem_v__d83;
    GemVal gem_v_params = gem_table_get(_t4094, gem_string("params"));
#line 1861 "compiler/main.gem"
    GemVal _t4095 = gem_v__d83;
    GemVal gem_v_body = gem_table_get(_t4095, gem_string("body"));

#line 1862 "compiler/main.gem"
    GemVal _t4096 = (*gem_v_anon_name);
    GemVal gem_v_name = _t4096.fn(_t4096.env, NULL, 0);
#line 1865 "compiler/main.gem"
    GemVal _t4097[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t4097, 1);
#line 1866 "compiler/main.gem"
    GemVal _t4098 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4098, gem_string("rest_param")), GEM_NIL))) {
#line 1867 "compiler/main.gem"
    GemVal _t4099 = gem_v_node;
    GemVal _t4100[] = {gem_v_inner_scope, gem_table_get(_t4099, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t4100, 2));
    }
#line 1869 "compiler/main.gem"
    GemVal _t4101 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4101, gem_string("block_param")), GEM_NIL))) {
#line 1870 "compiler/main.gem"
    GemVal _t4102 = gem_v_node;
    GemVal _t4103[] = {gem_v_inner_scope, gem_table_get(_t4102, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t4103, 2));
    }
#line 1872 "compiler/main.gem"
    GemVal _t4104[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn_collect_top_let_names(NULL, _t4104, 2));
#line 1874 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1875 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1876 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1877 "compiler/main.gem"
    GemVal _t4105[] = {gem_v_captures};
    GemVal _t4106[] = {gem_v_inner_scope, gem_fn_sorted_array_to_set(NULL, _t4105, 1)};
    *gem_v_local_names = gem_fn_set_union(NULL, _t4106, 2);
#line 1879 "compiler/main.gem"
    GemVal _t4107[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t4108 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t4108.fn(_t4108.env, _t4107, 2);
#line 1881 "compiler/main.gem"
    GemVal _t4109[] = {gem_v_captures};
    GemVal _t4110[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t4109, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t4110, 2);
#line 1882 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1885 "compiler/main.gem"
    GemVal _t4111[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t4111, 1));
#line 1886 "compiler/main.gem"
    GemVal gem_v_sb = gem_buf_new_fn(NULL, NULL, 0);
#line 1887 "compiler/main.gem"
    GemVal _t4112[] = {gem_v_struct_name};
    GemVal _t4113[] = {gem_v_sb, gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t4112, 1)), gem_string(" {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4113, 2));
#line 1888 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_captures;
#line 1888 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1888 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4114[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t4114, 1)))) break;
#line 1888 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1888 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1889 "compiler/main.gem"
    GemVal _t4115[] = {gem_v_cap};
    GemVal _t4116 = (*gem_v_mangle);
    GemVal _t4117[] = {_t4116.fn(_t4116.env, _t4115, 1)};
    GemVal _t4118[] = {gem_v_sb, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4117, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4118, 2));
    }

#line 1891 "compiler/main.gem"
    GemVal _t4119[] = {gem_v_sb, gem_string("};\n")};
    (void)(gem_buf_push_fn(NULL, _t4119, 2));
#line 1892 "compiler/main.gem"
    GemVal _t4120[] = {gem_v_sb};
    GemVal gem_v_struct_def = gem_buf_str_fn(NULL, _t4120, 1);
#line 1895 "compiler/main.gem"
    GemVal gem_v_fb = gem_buf_new_fn(NULL, NULL, 0);
#line 1896 "compiler/main.gem"
    GemVal _t4121[] = {gem_v_name};
    GemVal _t4122[] = {gem_v_fb, gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t4121, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4122, 2));
#line 1897 "compiler/main.gem"
    GemVal _t4123[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4123, 1), gem_int(0)))) {
#line 1898 "compiler/main.gem"
    GemVal _t4124[] = {gem_v_struct_name};
    GemVal _t4125[] = {gem_v_struct_name};
    GemVal _t4126[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t4124, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t4125, 1)), gem_string(" *)_env;\n"))};
        (void)(gem_buf_push_fn(NULL, _t4126, 2));
#line 1899 "compiler/main.gem"
        GemVal gem_v__for_items_85 = gem_v_captures;
#line 1899 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1899 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4127[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t4127, 1)))) break;
#line 1899 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1899 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1900 "compiler/main.gem"
    GemVal _t4128[] = {gem_v_cap};
    GemVal _t4129 = (*gem_v_mangle);
            GemVal gem_v_mc = _t4129.fn(_t4129.env, _t4128, 1);
#line 1901 "compiler/main.gem"
    GemVal _t4130[] = {gem_v_mc};
    GemVal _t4131[] = {gem_v_mc};
    GemVal _t4132[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4130, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t4131, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4132, 2));
        }

    }
#line 1905 "compiler/main.gem"
    GemVal _t4133[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t4134 = (*gem_v_emit_param_bindings);
    GemVal _t4135[] = {gem_v_fb, _t4134.fn(_t4134.env, _t4133, 3)};
    (void)(gem_buf_push_fn(NULL, _t4135, 2));
#line 1906 "compiler/main.gem"
    GemVal _t4136[] = {gem_v_name};
    GemVal _t4137[] = {gem_fn_escape_c_string(NULL, _t4136, 1)};
    GemVal _t4138[] = {(*gem_v_source_name)};
    GemVal _t4139[] = {gem_fn_escape_c_string(NULL, _t4138, 1)};
    GemVal _t4140[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t4137, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t4139, 1)), gem_string("\", 0);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4140, 2));
#line 1907 "compiler/main.gem"
    GemVal _t4141[] = {gem_v_body};
    GemVal _t4142 = (*gem_v_emit_fn_body);
    GemVal _t4143[] = {gem_v_fb, _t4142.fn(_t4142.env, _t4141, 1)};
    (void)(gem_buf_push_fn(NULL, _t4143, 2));
#line 1908 "compiler/main.gem"
    GemVal _t4144[] = {gem_v_fb, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4144, 2));
#line 1910 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1911 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1912 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1915 "compiler/main.gem"
    GemVal _t4145[] = {gem_v_fb};
    GemVal _t4146[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_buf_str_fn(NULL, _t4145, 1))};
    (void)(gem_push_fn(NULL, _t4146, 2));
    GemVal _t4147 = gem_table_new();
    gem_table_set(_t4147, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t4147, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t4147, gem_string("captures"), gem_v_captures);
    GemVal _t4148 = _t4147;
    gem_pop_frame();
    return _t4148;
}

struct _closure__anon_58 {
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
static GemVal _anon_58(void *_env, GemVal *args, int argc) {
    struct _closure__anon_58 *_cls = (struct _closure__anon_58 *)_env;
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
    gem_push_frame("_anon_58", "compiler/main.gem", 0);
#line 1924 "compiler/main.gem"
    GemVal _t4150 = gem_table_new();
    GemVal gem_v_fn_defs = _t4150;
#line 1925 "compiler/main.gem"
    GemVal _t4151 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t4151;
#line 1926 "compiler/main.gem"
    GemVal _t4152 = gem_table_new();
    GemVal gem_v_extern_includes = _t4152;
#line 1927 "compiler/main.gem"
    GemVal _t4153 = gem_table_new();
    GemVal gem_v_top_stmts = _t4153;
#line 1929 "compiler/main.gem"
    GemVal _t4154 = gem_v_ast;
    GemVal gem_v__for_items_86 = gem_table_get(_t4154, gem_string("stmts"));
#line 1929 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1929 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4155[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t4155, 1)))) break;
#line 1929 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1929 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1930 "compiler/main.gem"
    GemVal _t4156 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t4156, gem_string("tag")), gem_string("fn_def")))) {
#line 1931 "compiler/main.gem"
    GemVal _t4157[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t4157, 2));
        } else {
#line 1932 "compiler/main.gem"
    GemVal _t4158 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t4158, gem_string("tag")), gem_string("extern_fn")))) {
#line 1933 "compiler/main.gem"
    GemVal _t4159[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t4159, 2));
            } else {
#line 1934 "compiler/main.gem"
    GemVal _t4160 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t4160, gem_string("tag")), gem_string("extern_include")))) {
#line 1935 "compiler/main.gem"
    GemVal _t4161[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t4161, 2));
                } else {
#line 1936 "compiler/main.gem"
    GemVal _t4162 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t4162, gem_string("tag")), gem_string("export")))) {
                    } else {
#line 1939 "compiler/main.gem"
    GemVal _t4163[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t4163, 2));
                    }
                }
            }
        }
    }

#line 1944 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_extern_fns_list;
#line 1944 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1944 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4164[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t4164, 1)))) break;
#line 1944 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1944 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1945 "compiler/main.gem"
    GemVal _t4165 = gem_v_ef;
    GemVal _t4166[] = {(*gem_v_defined_fns), gem_table_get(_t4165, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t4166, 2));
    }

#line 1949 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_fn_defs;
#line 1949 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1949 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4167[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t4167, 1)))) break;
#line 1949 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1949 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1950 "compiler/main.gem"
    GemVal _t4168 = gem_v_fd;
    GemVal _t4169[] = {(*gem_v_defined_fns), gem_table_get(_t4168, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t4169, 2));
#line 1951 "compiler/main.gem"
    GemVal _t4170 = gem_v_fd;
    GemVal _t4171[] = {gem_table_get(_t4170, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4171, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1952 "compiler/main.gem"
    GemVal _t4172[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t4172, 2));
    }

#line 1956 "compiler/main.gem"
    GemVal _t4173[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t4173, 2));
#line 1957 "compiler/main.gem"
    GemVal _t4174 = gem_table_new();
    GemVal _t4175[] = {gem_v_top_stmts, _t4174};
    GemVal _t4176 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t4176.fn(_t4176.env, _t4175, 2);
#line 1960 "compiler/main.gem"
    GemVal gem_v__for_items_89 = gem_v_extern_fns_list;
#line 1960 "compiler/main.gem"
    GemVal gem_v__for_i_89 = gem_int(0);
#line 1960 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4177[] = {gem_v__for_items_89};
        if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t4177, 1)))) break;
#line 1960 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1960 "compiler/main.gem"
        gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1961 "compiler/main.gem"
    GemVal _t4178[] = {gem_v_ef};
    GemVal _t4179 = (*gem_v_compile_extern_fn);
    GemVal _t4180[] = {(*gem_v_functions), _t4179.fn(_t4179.env, _t4178, 1)};
        (void)(gem_push_fn(NULL, _t4180, 2));
    }

#line 1965 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_fn_defs;
#line 1965 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1965 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4181[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t4181, 1)))) break;
#line 1965 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1965 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1966 "compiler/main.gem"
    GemVal _t4182[] = {gem_v_fd};
    GemVal _t4183 = (*gem_v_compile_fn);
    GemVal _t4184[] = {(*gem_v_functions), _t4183.fn(_t4183.env, _t4182, 1)};
        (void)(gem_push_fn(NULL, _t4184, 2));
    }

#line 1970 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1971 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1972 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1973 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1974 "compiler/main.gem"
    GemVal _t4185 = gem_table_new();
    *gem_v_local_names = _t4185;
#line 1975 "compiler/main.gem"
    GemVal gem_v__for_tbl_91 = (*gem_v_top_level_vars);
#line 1975 "compiler/main.gem"
    GemVal _t4186[] = {gem_v__for_tbl_91};
    GemVal gem_v__for_keys_91 = gem_keys_fn(NULL, _t4186, 1);
#line 1975 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1975 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4187[] = {gem_v__for_keys_91};
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_len_fn(NULL, _t4187, 1)))) break;
#line 1975 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_91, gem_v__for_i_91);
#line 1975 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_91, gem_table_get(gem_v__for_keys_91, gem_v__for_i_91));
#line 1975 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1976 "compiler/main.gem"
    GemVal _t4188[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn_set_add(NULL, _t4188, 2));
    }

#line 1978 "compiler/main.gem"
    GemVal _t4189[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t4190 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t4190.fn(_t4190.env, _t4189, 2);
#line 1979 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1980 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1981 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1983 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1984 "compiler/main.gem"
    GemVal gem_v__for_items_92 = gem_v_fn_defs;
#line 1984 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1984 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4191[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t4191, 1)))) break;
#line 1984 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1984 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1985 "compiler/main.gem"
    GemVal _t4192 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t4192, gem_string("name")), gem_string("main")))) {
#line 1986 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1990 "compiler/main.gem"
    GemVal gem_v_mb = gem_buf_new_fn(NULL, NULL, 0);
#line 1991 "compiler/main.gem"
    GemVal _t4193[] = {gem_v_mb, gem_string("int main(int argc, char **argv) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4193, 2));
#line 1992 "compiler/main.gem"
    GemVal _t4194[] = {gem_v_mb, gem_string("    GC_INIT();\n")};
    (void)(gem_buf_push_fn(NULL, _t4194, 2));
#line 1993 "compiler/main.gem"
    GemVal _t4195[] = {gem_v_mb, gem_string("    gem_init(argc, argv);\n")};
    (void)(gem_buf_push_fn(NULL, _t4195, 2));
#line 1994 "compiler/main.gem"
    GemVal _t4196[] = {gem_v_mb, gem_string("    gem_scheduler_init();\n")};
    (void)(gem_buf_push_fn(NULL, _t4196, 2));
#line 1995 "compiler/main.gem"
    GemVal _t4197[] = {(*gem_v_source_name)};
    GemVal _t4198[] = {gem_fn_escape_c_string(NULL, _t4197, 1)};
    GemVal _t4199[] = {gem_v_mb, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t4198, 1)), gem_string("\", 0);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4199, 2));
#line 1996 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1997 "compiler/main.gem"
    GemVal _t4200[] = {gem_v_mb, gem_v_main_body};
        (void)(gem_buf_push_fn(NULL, _t4200, 2));
    }
#line 1999 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 2000 "compiler/main.gem"
    GemVal _t4201[] = {gem_v_mb, gem_string("    GemVal _margs[1] = {GEM_NIL};\n")};
        (void)(gem_buf_push_fn(NULL, _t4201, 2));
#line 2001 "compiler/main.gem"
    GemVal _t4202[] = {gem_v_mb, gem_string("    gem_fn_main(NULL, _margs, 0);\n")};
        (void)(gem_buf_push_fn(NULL, _t4202, 2));
    }
#line 2003 "compiler/main.gem"
    GemVal _t4203[] = {gem_v_mb, gem_string("    gem_run_scheduler();\n")};
    (void)(gem_buf_push_fn(NULL, _t4203, 2));
#line 2004 "compiler/main.gem"
    GemVal _t4204[] = {gem_v_mb, gem_string("    gem_pop_frame();\n    return 0;\n}\n")};
    (void)(gem_buf_push_fn(NULL, _t4204, 2));
#line 2005 "compiler/main.gem"
    GemVal _t4205[] = {gem_v_mb};
    GemVal gem_v_main_c = gem_buf_str_fn(NULL, _t4205, 1);
#line 2008 "compiler/main.gem"
    GemVal gem_v_ob = gem_buf_new_fn(NULL, NULL, 0);
#line 2009 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_includes;
#line 2009 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 2009 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4206[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t4206, 1)))) break;
#line 2009 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 2009 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 2010 "compiler/main.gem"
    GemVal _t4207 = gem_v_ei;
    GemVal _t4208[] = {gem_table_get(_t4207, gem_string("path"))};
    GemVal _t4209[] = {gem_v_ob, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t4208, 1)), gem_string("\"\n"))};
        (void)(gem_buf_push_fn(NULL, _t4209, 2));
    }

#line 2012 "compiler/main.gem"
    GemVal _t4210[] = {gem_v_ob, gem_string("#include \"gem.h\"\n")};
    (void)(gem_buf_push_fn(NULL, _t4210, 2));
#line 2016 "compiler/main.gem"
    GemVal _t4211[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t4211, 1), gem_int(0)))) {
#line 2017 "compiler/main.gem"
        GemVal gem_v__for_i_94 = gem_int(0);
#line 2017 "compiler/main.gem"
    GemVal _t4212[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_94 = gem_len_fn(NULL, _t4212, 1);
#line 2017 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_v__for_limit_94))) break;
#line 2017 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_94;
#line 2017 "compiler/main.gem"
            gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 2018 "compiler/main.gem"
    GemVal _t4213[] = {gem_v_ob, gem_table_get((*gem_v_extern_c_decls), gem_v_i)};
            (void)(gem_buf_push_fn(NULL, _t4213, 2));
#line 2019 "compiler/main.gem"
    GemVal _t4214[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4214, 2));
        }

    }
#line 2024 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 2024 "compiler/main.gem"
    GemVal _t4215[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_95 = gem_len_fn(NULL, _t4215, 1);
#line 2024 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_v__for_limit_95))) break;
#line 2024 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_95;
#line 2024 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 2025 "compiler/main.gem"
    GemVal _t4216[] = {gem_v_ob, gem_table_get((*gem_v_forward_decls), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4216, 2));
#line 2026 "compiler/main.gem"
    GemVal _t4217[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4217, 1), gem_int(1))))) {
#line 2027 "compiler/main.gem"
    GemVal _t4218[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4218, 2));
        }
    }

#line 2030 "compiler/main.gem"
    GemVal _t4219[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4219, 2));
#line 2033 "compiler/main.gem"
    GemVal _t4220[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t4220, 1);
#line 2034 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 2034 "compiler/main.gem"
    GemVal _t4221[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_96 = gem_len_fn(NULL, _t4221, 1);
#line 2034 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_v__for_limit_96))) break;
#line 2034 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_96;
#line 2034 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 2035 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 2036 "compiler/main.gem"
    GemVal _t4222[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t4222, 2))) {
#line 2037 "compiler/main.gem"
    GemVal _t4223[] = {gem_v_vname};
    GemVal _t4224[] = {gem_v_ob, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t4223, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4224, 2));
        } else {
#line 2039 "compiler/main.gem"
    GemVal _t4225[] = {gem_v_vname};
    GemVal _t4226[] = {gem_v_ob, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t4225, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4226, 2));
        }
    }

#line 2042 "compiler/main.gem"
    GemVal _t4227[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4227, 2));
#line 2045 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 2045 "compiler/main.gem"
    GemVal _t4228[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_97 = gem_len_fn(NULL, _t4228, 1);
#line 2045 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_limit_97))) break;
#line 2045 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_97;
#line 2045 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 2046 "compiler/main.gem"
    GemVal _t4229[] = {gem_v_ob, gem_table_get((*gem_v_functions), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4229, 2));
#line 2047 "compiler/main.gem"
    GemVal _t4230[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4230, 1), gem_int(1))))) {
#line 2048 "compiler/main.gem"
    GemVal _t4231[] = {gem_v_ob, gem_string("\n\n")};
            (void)(gem_buf_push_fn(NULL, _t4231, 2));
        }
    }

#line 2051 "compiler/main.gem"
    GemVal _t4232[] = {gem_v_ob, gem_string("\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4232, 2));
#line 2053 "compiler/main.gem"
    GemVal _t4233[] = {gem_v_ob, gem_v_main_c};
    (void)(gem_buf_push_fn(NULL, _t4233, 2));
#line 2054 "compiler/main.gem"
    GemVal _t4234[] = {gem_v_ob};
    GemVal _t4235 = gem_buf_str_fn(NULL, _t4234, 1);
    gem_pop_frame();
    return _t4235;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 127 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 127);
#line 128 "compiler/main.gem"
    GemVal _t1655 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1655;
#line 129 "compiler/main.gem"
    GemVal _t1656 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1656;
#line 130 "compiler/main.gem"
    GemVal _t1657 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1657;
#line 131 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 132 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 133 "compiler/main.gem"
    GemVal _t1658 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1658;
#line 134 "compiler/main.gem"
    GemVal _t1659 = gem_table_new();
    gem_table_set(_t1659, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1659, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1659, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1659, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1659, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1659, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1659, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1659, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1659, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1659, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1659, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1659, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1659, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1659, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1659, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1659, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1659, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1659, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1659, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1659, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1659, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1659, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1659, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1659, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1659, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1659, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1659, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1659, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1659, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1659, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1659, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1659, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1659, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1659, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1659, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1659, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1659, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1659, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1659, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1659, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1659, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1659, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1659, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1659, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1659, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1659, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1659, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1659, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1659, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1659, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1659, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1659, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1659, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1659, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1659, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1659, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1659, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1659, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1659, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1659, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1659, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1659, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1659, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1659, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1659, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1659, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1659, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1659, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1659, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1659, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1659, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    gem_table_set(_t1659, gem_string("exec"), gem_string("gem_exec_fn"));
    gem_table_set(_t1659, gem_string("tcp_connect"), gem_string("gem_tcp_connect_fn"));
    gem_table_set(_t1659, gem_string("tcp_listen"), gem_string("gem_tcp_listen_fn"));
    gem_table_set(_t1659, gem_string("tcp_accept"), gem_string("gem_tcp_accept_fn"));
    gem_table_set(_t1659, gem_string("tcp_read"), gem_string("gem_tcp_read_fn"));
    gem_table_set(_t1659, gem_string("tcp_write"), gem_string("gem_tcp_write_fn"));
    gem_table_set(_t1659, gem_string("tcp_close"), gem_string("gem_tcp_close_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1659;
#line 214 "compiler/main.gem"
    GemVal _t1660 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1660;
#line 215 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 215 "compiler/main.gem"
    GemVal _t1661[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1661, 1);
#line 215 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 215 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1662[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1662, 1)))) break;
#line 215 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 215 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 215 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 216 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 218 "compiler/main.gem"
    GemVal _t1663 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1663;
#line 219 "compiler/main.gem"
    GemVal _t1664 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1664;
#line 220 "compiler/main.gem"
    GemVal _t1665 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1665;
#line 221 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 222 "compiler/main.gem"
    GemVal _t1666 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1666;
#line 223 "compiler/main.gem"
    GemVal *gem_v_tco_fn_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_fn_name = GEM_NIL;
#line 224 "compiler/main.gem"
    GemVal *gem_v_tco_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_params = GEM_NIL;
#line 225 "compiler/main.gem"
    GemVal *gem_v_tco_captured = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_captured = GEM_NIL;
#line 229 "compiler/main.gem"
    struct _closure__anon_24 *_t1669 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1669->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1669);
#line 234 "compiler/main.gem"
    struct _closure__anon_25 *_t1672 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1672->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1672);
#line 239 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_26, NULL);
#line 245 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 246 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 248 "compiler/main.gem"
    struct _closure__anon_27 *_t1985 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1985->gem_v_builtins = gem_v_builtins;
    _t1985->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1985->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1985->gem_v_defined_fns = gem_v_defined_fns;
    _t1985->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t1985);
#line 442 "compiler/main.gem"
    struct _closure__anon_28 *_t1997 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1997->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t1997);
#line 457 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 458 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 460 "compiler/main.gem"
    struct _closure__anon_29 *_t2186 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2186->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2186->gem_v_walk_captures = gem_v_walk_captures;
    _t2186->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2186);
#line 572 "compiler/main.gem"
    struct _closure__anon_30 *_t2196 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2196->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2196);
#line 583 "compiler/main.gem"
    struct _closure__anon_31 *_t2201 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2201->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2201);
#line 591 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 592 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 593 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 594 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 595 "compiler/main.gem"
    GemVal *gem_v_compile_stmts_tail = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts_tail = GEM_NIL;
#line 596 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 597 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 598 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 599 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 600 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 601 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 602 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 603 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 604 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 605 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 606 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 607 "compiler/main.gem"
    GemVal *gem_v_compile_blocking_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_blocking_extern_fn = GEM_NIL;
#line 608 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 609 "compiler/main.gem"
    GemVal *gem_v_is_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_self_tail_call = GEM_NIL;
#line 610 "compiler/main.gem"
    GemVal *gem_v_has_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_has_self_tail_call = GEM_NIL;
#line 611 "compiler/main.gem"
    GemVal *gem_v_emit_tco_continue = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_tco_continue = GEM_NIL;
#line 615 "compiler/main.gem"
    struct _closure__anon_32 *_t2286 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2286->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t2286->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_is_self_tail_call = gem_make_fn(_anon_32, _t2286);
#line 664 "compiler/main.gem"
    struct _closure__anon_33 *_t2292 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2292->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_has_self_tail_call = gem_make_fn(_anon_33, _t2292);
#line 671 "compiler/main.gem"
    struct _closure__anon_34 *_t2325 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2325->gem_v_compile_expr = gem_v_compile_expr;
    _t2325->gem_v_mangle = gem_v_mangle;
    _t2325->gem_v_tco_captured = gem_v_tco_captured;
    _t2325->gem_v_tco_params = gem_v_tco_params;
    _t2325->gem_v_tmp = gem_v_tmp;
    *gem_v_emit_tco_continue = gem_make_fn(_anon_34, _t2325);
#line 701 "compiler/main.gem"
    struct _closure__anon_35 *_t2525 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2525->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2525->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2525->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2525->gem_v_compile_array = gem_v_compile_array;
    _t2525->gem_v_compile_binop = gem_v_compile_binop;
    _t2525->gem_v_compile_call = gem_v_compile_call;
    _t2525->gem_v_compile_expr = gem_v_compile_expr;
    _t2525->gem_v_compile_table = gem_v_compile_table;
    _t2525->gem_v_defined_fns = gem_v_defined_fns;
    _t2525->gem_v_local_names = gem_v_local_names;
    _t2525->gem_v_mangle = gem_v_mangle;
    _t2525->gem_v_tmp = gem_v_tmp;
    _t2525->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_35, _t2525);
#line 788 "compiler/main.gem"
    struct _closure__anon_36 *_t2546 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2546->gem_v_compile_expr = gem_v_compile_expr;
    _t2546->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_36, _t2546);
#line 804 "compiler/main.gem"
    struct _closure__anon_37 *_t2564 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2564->gem_v_compile_expr = gem_v_compile_expr;
    _t2564->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_37, _t2564);
#line 819 "compiler/main.gem"
    struct _closure__anon_38 *_t2613 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2613->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2613->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2613->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2613->gem_v_mangle = gem_v_mangle;
    _t2613->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_38, _t2613);
#line 854 "compiler/main.gem"
    struct _closure__anon_39 *_t2632 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2632->gem_v_compile_expr = gem_v_compile_expr;
    _t2632->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_39, _t2632);
#line 875 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_40, NULL);
#line 884 "compiler/main.gem"
    struct _closure__anon_41 *_t2724 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2724->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2724->gem_v_compile_args = gem_v_compile_args;
    _t2724->gem_v_compile_expr = gem_v_compile_expr;
    _t2724->gem_v_defined_fns = gem_v_defined_fns;
    _t2724->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2724->gem_v_local_names = gem_v_local_names;
    _t2724->gem_v_source_name = gem_v_source_name;
    _t2724->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_41, _t2724);
#line 928 "compiler/main.gem"
    struct _closure__anon_42 *_t2895 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2895->gem_v_compile_expr = gem_v_compile_expr;
    _t2895->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_42, _t2895);
#line 1009 "compiler/main.gem"
    struct _closure__anon_43 *_t2903 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2903->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_43, _t2903);
#line 1018 "compiler/main.gem"
    struct _closure__anon_44 *_t2923 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2923->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2923->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t2923->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_stmts_tail = gem_make_fn(_anon_44, _t2923);
#line 1039 "compiler/main.gem"
    struct _closure__anon_45 *_t3177 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3177->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3177->gem_v_compile_expr = gem_v_compile_expr;
    _t3177->gem_v_compile_if = gem_v_compile_if;
    _t3177->gem_v_compile_match = gem_v_compile_match;
    _t3177->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3177->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3177->gem_v_compile_while = gem_v_compile_while;
    _t3177->gem_v_in_top_level = gem_v_in_top_level;
    _t3177->gem_v_mangle = gem_v_mangle;
    _t3177->gem_v_source_name = gem_v_source_name;
    _t3177->gem_v_tmp = gem_v_tmp;
    _t3177->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3177->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_45, _t3177);
#line 1116 "compiler/main.gem"
    struct _closure__anon_46 *_t3440 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3440->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3440->gem_v_compile_expr = gem_v_compile_expr;
    _t3440->gem_v_compile_if = gem_v_compile_if;
    _t3440->gem_v_compile_match = gem_v_compile_match;
    _t3440->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3440->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3440->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3440->gem_v_compile_while = gem_v_compile_while;
    _t3440->gem_v_emit_tco_continue = gem_v_emit_tco_continue;
    _t3440->gem_v_local_names = gem_v_local_names;
    _t3440->gem_v_mangle = gem_v_mangle;
    _t3440->gem_v_source_name = gem_v_source_name;
    _t3440->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3440->gem_v_tmp = gem_v_tmp;
    _t3440->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_46, _t3440);
#line 1195 "compiler/main.gem"
    struct _closure__anon_47 *_t3472 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3472->gem_v_compile_expr = gem_v_compile_expr;
    _t3472->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    *gem_v_compile_if = gem_make_fn(_anon_47, _t3472);
#line 1215 "compiler/main.gem"
    struct _closure__anon_48 *_t3498 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3498->gem_v_compile_expr = gem_v_compile_expr;
    _t3498->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_48, _t3498);
#line 1232 "compiler/main.gem"
    struct _closure__anon_49 *_t3582 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3582->gem_v_compile_expr = gem_v_compile_expr;
    _t3582->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3582->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3582->gem_v_mangle = gem_v_mangle;
    _t3582->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_49, _t3582);
#line 1288 "compiler/main.gem"
    struct _closure__anon_50 *_t3759 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3759->gem_v_compile_expr = gem_v_compile_expr;
    _t3759->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3759->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3759->gem_v_mangle = gem_v_mangle;
    _t3759->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_50, _t3759);
#line 1408 "compiler/main.gem"
    struct _closure__anon_51 *_t3767 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3767->gem_v_extern_c_decls = gem_v_extern_c_decls;
    GemVal *gem_v_emit_extern_c_decl = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_extern_c_decl = gem_make_fn(_anon_51, _t3767);
#line 1454 "compiler/main.gem"
    struct _closure__anon_52 *_t3841 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3841->gem_v_compile_blocking_extern_fn = gem_v_compile_blocking_extern_fn;
    _t3841->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3841->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_52, _t3841);
#line 1537 "compiler/main.gem"
    struct _closure__anon_53 *_t3966 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3966->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3966->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_blocking_extern_fn = gem_make_fn(_anon_53, _t3966);
#line 1717 "compiler/main.gem"
    struct _closure__anon_54 *_t4019 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t4019->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_54, _t4019);
#line 1761 "compiler/main.gem"
    struct _closure__anon_55 *_t4033 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t4033->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4033->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_55, _t4033);
#line 1778 "compiler/main.gem"
    struct _closure__anon_56 *_t4093 = GC_MALLOC(sizeof(struct _closure__anon_56));
    _t4093->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4093->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4093->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t4093->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4093->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4093->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4093->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t4093->gem_v_in_top_level = gem_v_in_top_level;
    _t4093->gem_v_local_names = gem_v_local_names;
    _t4093->gem_v_source_name = gem_v_source_name;
    _t4093->gem_v_tco_captured = gem_v_tco_captured;
    _t4093->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t4093->gem_v_tco_params = gem_v_tco_params;
    *gem_v_compile_fn = gem_make_fn(_anon_56, _t4093);
#line 1860 "compiler/main.gem"
    struct _closure__anon_57 *_t4149 = GC_MALLOC(sizeof(struct _closure__anon_57));
    _t4149->gem_v_anon_name = gem_v_anon_name;
    _t4149->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4149->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4149->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4149->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4149->gem_v_functions = gem_v_functions;
    _t4149->gem_v_in_top_level = gem_v_in_top_level;
    _t4149->gem_v_local_names = gem_v_local_names;
    _t4149->gem_v_mangle = gem_v_mangle;
    _t4149->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_57, _t4149);
#line 1922 "compiler/main.gem"
    struct _closure__anon_58 *_t4236 = GC_MALLOC(sizeof(struct _closure__anon_58));
    _t4236->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4236->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t4236->gem_v_compile_fn = gem_v_compile_fn;
    _t4236->gem_v_compile_stmts = gem_v_compile_stmts;
    _t4236->gem_v_defined_fns = gem_v_defined_fns;
    _t4236->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t4236->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4236->gem_v_forward_decls = gem_v_forward_decls;
    _t4236->gem_v_functions = gem_v_functions;
    _t4236->gem_v_in_top_level = gem_v_in_top_level;
    _t4236->gem_v_local_names = gem_v_local_names;
    _t4236->gem_v_source_name = gem_v_source_name;
    _t4236->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t4236->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_58, _t4236);
    GemVal _t4237 = gem_table_new();
    gem_table_set(_t4237, gem_string("compile"), gem_v_compile);
    GemVal _t4238 = _t4237;
    gem_pop_frame();
    return _t4238;
}

static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc) {
#line 20 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("module_name_from_path", "compiler/main.gem", 20);
#line 21 "compiler/main.gem"
    GemVal _t4239[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4239, 1);
#line 22 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 23 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 24 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 25 "compiler/main.gem"
    GemVal _t4240[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4240, 2), gem_int(47)))) {
#line 26 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 28 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 30 "compiler/main.gem"
    GemVal _t4241[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4241, 3);
#line 31 "compiler/main.gem"
    GemVal _t4242[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4242, 1);
#line 32 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 33 "compiler/main.gem"
    GemVal _t4243 = gem_v_string;
    GemVal _t4244[] = {gem_v_name, gem_string(".gem")};
    GemVal _t4245 = gem_table_get(_t4243, gem_string("ends_with"));
        if (gem_truthy(_t4245.fn(_t4245.env, _t4244, 2))) {
#line 34 "compiler/main.gem"
    GemVal _t4246[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4246, 3);
        }
    }
    GemVal _t4247 = gem_v_name;
    gem_pop_frame();
    return _t4247;
}

static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("find_export_node", "compiler/main.gem", 40);
#line 41 "compiler/main.gem"
    GemVal _t4248[] = {gem_v_stmts};
    GemVal gem_v_i = gem_sub(gem_len_fn(NULL, _t4248, 1), gem_int(1));
#line 42 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_i, gem_int(0)))) break;
#line 43 "compiler/main.gem"
    GemVal _t4249[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("export")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4249, 2))) {
#line 44 "compiler/main.gem"
            GemVal _t4250 = gem_table_get(gem_v_stmts, gem_v_i);
            gem_pop_frame();
            return _t4250;
        }
#line 46 "compiler/main.gem"
        gem_v_i = gem_sub(gem_v_i, gem_int(1));
    }
    GemVal _t4251 = GEM_NIL;
    gem_pop_frame();
    return _t4251;
}

static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc) {
#line 51 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("collect_module_top_names", "compiler/main.gem", 51);
#line 52 "compiler/main.gem"
    GemVal _t4252 = gem_table_new();
    GemVal gem_v_names = _t4252;
#line 53 "compiler/main.gem"
    GemVal gem_v__for_items_1 = gem_v_stmts;
#line 53 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 53 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4253[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t4253, 1)))) break;
#line 53 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 53 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 54 "compiler/main.gem"
    GemVal _t4254[] = {gem_v_stmt, gem_string("fn_def")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4254, 2))) {
#line 55 "compiler/main.gem"
    GemVal _t4255 = gem_v_stmt;
            gem_table_set(gem_v_names, gem_table_get(_t4255, gem_string("name")), gem_bool(1));
        } else {
#line 56 "compiler/main.gem"
    GemVal _t4256[] = {gem_v_stmt, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t4256, 2))) {
#line 57 "compiler/main.gem"
    GemVal _t4257 = gem_v_stmt;
                gem_table_set(gem_v_names, gem_table_get(_t4257, gem_string("name")), gem_bool(1));
            }
        }
    }

    GemVal _t4258 = gem_v_names;
    gem_pop_frame();
    return _t4258;
}

static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc) {
#line 63 "compiler/main.gem"
    GemVal gem_v_top_names = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("build_rename_map", "compiler/main.gem", 63);
#line 64 "compiler/main.gem"
    GemVal _t4259 = gem_table_new();
    GemVal gem_v_rmap = _t4259;
#line 65 "compiler/main.gem"
    GemVal gem_v__for_tbl_2 = gem_v_top_names;
#line 65 "compiler/main.gem"
    GemVal _t4260[] = {gem_v__for_tbl_2};
    GemVal gem_v__for_keys_2 = gem_keys_fn(NULL, _t4260, 1);
#line 65 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 65 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4261[] = {gem_v__for_keys_2};
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t4261, 1)))) break;
#line 65 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__for_keys_2, gem_v__for_i_2);
#line 65 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_2, gem_table_get(gem_v__for_keys_2, gem_v__for_i_2));
#line 65 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 66 "compiler/main.gem"
        gem_table_set(gem_v_rmap, gem_v_name, gem_add(gem_v_prefix, gem_v_name));
    }

    GemVal _t4262 = gem_v_rmap;
    gem_pop_frame();
    return _t4262;
}

static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc) {
#line 71 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("copy_set", "compiler/main.gem", 71);
#line 72 "compiler/main.gem"
    GemVal _t4263 = gem_table_new();
    GemVal gem_v_result = _t4263;
#line 73 "compiler/main.gem"
    GemVal gem_v__for_tbl_3 = gem_v_s;
#line 73 "compiler/main.gem"
    GemVal _t4264[] = {gem_v__for_tbl_3};
    GemVal gem_v__for_keys_3 = gem_keys_fn(NULL, _t4264, 1);
#line 73 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 73 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4265[] = {gem_v__for_keys_3};
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t4265, 1)))) break;
#line 73 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_3, gem_v__for_i_3);
#line 73 "compiler/main.gem"
        GemVal gem_v_v = gem_table_get(gem_v__for_tbl_3, gem_table_get(gem_v__for_keys_3, gem_v__for_i_3));
#line 73 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 74 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_v_v);
    }

    GemVal _t4266 = gem_v_result;
    gem_pop_frame();
    return _t4266;
}

static GemVal gem_fn_rename_node(void *_env, GemVal *args, int argc) {
#line 79 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rmap = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_shadowed = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("rename_node", "compiler/main.gem", 79);
    while(1) {
        gem_yield_check();
#line 80 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 81 "compiler/main.gem"
            GemVal _t4267 = GEM_NIL;
            gem_pop_frame();
            return _t4267;
        }
#line 83 "compiler/main.gem"
    GemVal _t4268[] = {gem_v_node};
        if (gem_truthy(gem_neq(gem_type_fn(NULL, _t4268, 1), gem_string("table")))) {
#line 84 "compiler/main.gem"
            GemVal _t4269 = GEM_NIL;
            gem_pop_frame();
            return _t4269;
        }
#line 86 "compiler/main.gem"
    GemVal _t4270 = gem_v_node;
        if (gem_truthy(gem_eq(gem_table_get(_t4270, gem_string("tag")), GEM_NIL))) {
#line 87 "compiler/main.gem"
            GemVal _t4271 = GEM_NIL;
            gem_pop_frame();
            return _t4271;
        }
#line 90 "compiler/main.gem"
        GemVal gem_v__match_4 = gem_v_node;
    GemVal _t4272[] = {gem_v__match_4};
    GemVal _t4274;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4272, 1), gem_string("table")))) {
        _t4274 = gem_eq(gem_type_fn(NULL, _t4272, 1), gem_string("table"));
    } else {
        GemVal _t4273[] = {gem_v__match_4, gem_string("tag")};
        _t4274 = gem_has_key_fn(NULL, _t4273, 2);
    }
    GemVal _t4275;
    if (!gem_truthy(_t4274)) {
        _t4275 = _t4274;
    } else {
        _t4275 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("var"));
    }
    GemVal _t4277;
    if (!gem_truthy(_t4275)) {
        _t4277 = _t4275;
    } else {
        GemVal _t4276[] = {gem_v__match_4, gem_string("name")};
        _t4277 = gem_has_key_fn(NULL, _t4276, 2);
    }
        if (gem_truthy(_t4277)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 92 "compiler/main.gem"
    GemVal _t4278[] = {gem_v_rmap, gem_v_name};
    GemVal _t4280;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4278, 2))) {
        _t4280 = gem_has_key_fn(NULL, _t4278, 2);
    } else {
        GemVal _t4279[] = {gem_v_shadowed, gem_v_name};
        _t4280 = gem_not(gem_has_key_fn(NULL, _t4279, 2));
    }
            if (gem_truthy(_t4280)) {
#line 93 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4281[] = {gem_v__match_4};
    GemVal _t4283;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4281, 1), gem_string("table")))) {
        _t4283 = gem_eq(gem_type_fn(NULL, _t4281, 1), gem_string("table"));
    } else {
        GemVal _t4282[] = {gem_v__match_4, gem_string("tag")};
        _t4283 = gem_has_key_fn(NULL, _t4282, 2);
    }
    GemVal _t4284;
    if (!gem_truthy(_t4283)) {
        _t4284 = _t4283;
    } else {
        _t4284 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("fn_def"));
    }
    GemVal _t4286;
    if (!gem_truthy(_t4284)) {
        _t4286 = _t4284;
    } else {
        GemVal _t4285[] = {gem_v__match_4, gem_string("name")};
        _t4286 = gem_has_key_fn(NULL, _t4285, 2);
    }
    GemVal _t4288;
    if (!gem_truthy(_t4286)) {
        _t4288 = _t4286;
    } else {
        GemVal _t4287[] = {gem_v__match_4, gem_string("params")};
        _t4288 = gem_has_key_fn(NULL, _t4287, 2);
    }
    GemVal _t4290;
    if (!gem_truthy(_t4288)) {
        _t4290 = _t4288;
    } else {
        GemVal _t4289[] = {gem_v__match_4, gem_string("body")};
        _t4290 = gem_has_key_fn(NULL, _t4289, 2);
    }
        if (gem_truthy(_t4290)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 96 "compiler/main.gem"
    GemVal _t4291[] = {gem_v_rmap, gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t4291, 2))) {
#line 97 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
            }
#line 99 "compiler/main.gem"
    GemVal _t4292[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4292, 1);
#line 100 "compiler/main.gem"
            GemVal gem_v__for_items_5 = gem_v_params;
#line 100 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 100 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4293[] = {gem_v__for_items_5};
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t4293, 1)))) break;
#line 100 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 100 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 101 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 103 "compiler/main.gem"
    GemVal _t4294 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4294, gem_string("rest_param")), GEM_NIL))) {
#line 104 "compiler/main.gem"
    GemVal _t4295 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4295, gem_string("rest_param")), gem_bool(1));
            }
#line 106 "compiler/main.gem"
    GemVal _t4296 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4296, gem_string("block_param")), GEM_NIL))) {
#line 107 "compiler/main.gem"
    GemVal _t4297 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4297, gem_string("block_param")), gem_bool(1));
            }
#line 109 "compiler/main.gem"
    GemVal _t4298[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4299 = gem_fn_rename_stmts(NULL, _t4298, 3);
            gem_pop_frame();
            return _t4299;
        } else {
    GemVal _t4300[] = {gem_v__match_4};
    GemVal _t4302;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4300, 1), gem_string("table")))) {
        _t4302 = gem_eq(gem_type_fn(NULL, _t4300, 1), gem_string("table"));
    } else {
        GemVal _t4301[] = {gem_v__match_4, gem_string("tag")};
        _t4302 = gem_has_key_fn(NULL, _t4301, 2);
    }
    GemVal _t4303;
    if (!gem_truthy(_t4302)) {
        _t4303 = _t4302;
    } else {
        _t4303 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t4305;
    if (!gem_truthy(_t4303)) {
        _t4305 = _t4303;
    } else {
        GemVal _t4304[] = {gem_v__match_4, gem_string("params")};
        _t4305 = gem_has_key_fn(NULL, _t4304, 2);
    }
    GemVal _t4307;
    if (!gem_truthy(_t4305)) {
        _t4307 = _t4305;
    } else {
        GemVal _t4306[] = {gem_v__match_4, gem_string("body")};
        _t4307 = gem_has_key_fn(NULL, _t4306, 2);
    }
        if (gem_truthy(_t4307)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 111 "compiler/main.gem"
    GemVal _t4308[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4308, 1);
#line 112 "compiler/main.gem"
            GemVal gem_v__for_items_6 = gem_v_params;
#line 112 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 112 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4309[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t4309, 1)))) break;
#line 112 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 112 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 113 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 115 "compiler/main.gem"
    GemVal _t4310 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4310, gem_string("rest_param")), GEM_NIL))) {
#line 116 "compiler/main.gem"
    GemVal _t4311 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4311, gem_string("rest_param")), gem_bool(1));
            }
#line 118 "compiler/main.gem"
    GemVal _t4312 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4312, gem_string("block_param")), GEM_NIL))) {
#line 119 "compiler/main.gem"
    GemVal _t4313 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4313, gem_string("block_param")), gem_bool(1));
            }
#line 121 "compiler/main.gem"
    GemVal _t4314[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4315 = gem_fn_rename_stmts(NULL, _t4314, 3);
            gem_pop_frame();
            return _t4315;
        } else {
    GemVal _t4316[] = {gem_v__match_4};
    GemVal _t4318;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4316, 1), gem_string("table")))) {
        _t4318 = gem_eq(gem_type_fn(NULL, _t4316, 1), gem_string("table"));
    } else {
        GemVal _t4317[] = {gem_v__match_4, gem_string("tag")};
        _t4318 = gem_has_key_fn(NULL, _t4317, 2);
    }
    GemVal _t4319;
    if (!gem_truthy(_t4318)) {
        _t4319 = _t4318;
    } else {
        _t4319 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("let"));
    }
    GemVal _t4321;
    if (!gem_truthy(_t4319)) {
        _t4321 = _t4319;
    } else {
        GemVal _t4320[] = {gem_v__match_4, gem_string("name")};
        _t4321 = gem_has_key_fn(NULL, _t4320, 2);
    }
    GemVal _t4323;
    if (!gem_truthy(_t4321)) {
        _t4323 = _t4321;
    } else {
        GemVal _t4322[] = {gem_v__match_4, gem_string("value")};
        _t4323 = gem_has_key_fn(NULL, _t4322, 2);
    }
        if (gem_truthy(_t4323)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 123 "compiler/main.gem"
    GemVal _t4324[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4324, 3));
#line 124 "compiler/main.gem"
    GemVal _t4325[] = {gem_v_rmap, gem_v_name};
    GemVal _t4327;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4325, 2))) {
        _t4327 = gem_has_key_fn(NULL, _t4325, 2);
    } else {
        GemVal _t4326[] = {gem_v_shadowed, gem_v_name};
        _t4327 = gem_not(gem_has_key_fn(NULL, _t4326, 2));
    }
            if (gem_truthy(_t4327)) {
#line 125 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4328[] = {gem_v__match_4};
    GemVal _t4330;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4328, 1), gem_string("table")))) {
        _t4330 = gem_eq(gem_type_fn(NULL, _t4328, 1), gem_string("table"));
    } else {
        GemVal _t4329[] = {gem_v__match_4, gem_string("tag")};
        _t4330 = gem_has_key_fn(NULL, _t4329, 2);
    }
    GemVal _t4331;
    if (!gem_truthy(_t4330)) {
        _t4331 = _t4330;
    } else {
        _t4331 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t4333;
    if (!gem_truthy(_t4331)) {
        _t4333 = _t4331;
    } else {
        GemVal _t4332[] = {gem_v__match_4, gem_string("name")};
        _t4333 = gem_has_key_fn(NULL, _t4332, 2);
    }
    GemVal _t4335;
    if (!gem_truthy(_t4333)) {
        _t4335 = _t4333;
    } else {
        GemVal _t4334[] = {gem_v__match_4, gem_string("value")};
        _t4335 = gem_has_key_fn(NULL, _t4334, 2);
    }
        if (gem_truthy(_t4335)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 128 "compiler/main.gem"
    GemVal _t4336[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4336, 3));
#line 129 "compiler/main.gem"
    GemVal _t4337[] = {gem_v_rmap, gem_v_name};
    GemVal _t4339;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4337, 2))) {
        _t4339 = gem_has_key_fn(NULL, _t4337, 2);
    } else {
        GemVal _t4338[] = {gem_v_shadowed, gem_v_name};
        _t4339 = gem_not(gem_has_key_fn(NULL, _t4338, 2));
    }
            if (gem_truthy(_t4339)) {
#line 130 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4340[] = {gem_v__match_4};
    GemVal _t4342;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4340, 1), gem_string("table")))) {
        _t4342 = gem_eq(gem_type_fn(NULL, _t4340, 1), gem_string("table"));
    } else {
        GemVal _t4341[] = {gem_v__match_4, gem_string("tag")};
        _t4342 = gem_has_key_fn(NULL, _t4341, 2);
    }
    GemVal _t4343;
    if (!gem_truthy(_t4342)) {
        _t4343 = _t4342;
    } else {
        _t4343 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("call"));
    }
    GemVal _t4345;
    if (!gem_truthy(_t4343)) {
        _t4345 = _t4343;
    } else {
        GemVal _t4344[] = {gem_v__match_4, gem_string("func")};
        _t4345 = gem_has_key_fn(NULL, _t4344, 2);
    }
    GemVal _t4347;
    if (!gem_truthy(_t4345)) {
        _t4347 = _t4345;
    } else {
        GemVal _t4346[] = {gem_v__match_4, gem_string("args")};
        _t4347 = gem_has_key_fn(NULL, _t4346, 2);
    }
        if (gem_truthy(_t4347)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_func = gem_table_get(gem_v__match_4, gem_string("func"));
#line 0 "compiler/main.gem"
            GemVal gem_v_args = gem_table_get(gem_v__match_4, gem_string("args"));
#line 133 "compiler/main.gem"
    GemVal _t4348[] = {gem_v_func, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4348, 3));
            {
#line 134 "compiler/main.gem"
                GemVal gem_v__for_items_7 = gem_v_args;
#line 134 "compiler/main.gem"
                GemVal gem_v__for_i_7 = gem_int(0);
#line 134 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4349[] = {gem_v__for_items_7};
                    if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t4349, 1)))) break;
#line 134 "compiler/main.gem"
                    GemVal gem_v_a = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 134 "compiler/main.gem"
                    gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 135 "compiler/main.gem"
    GemVal _t4350[] = {gem_v_a, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4350, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4351[] = {gem_v__match_4};
    GemVal _t4353;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4351, 1), gem_string("table")))) {
        _t4353 = gem_eq(gem_type_fn(NULL, _t4351, 1), gem_string("table"));
    } else {
        GemVal _t4352[] = {gem_v__match_4, gem_string("tag")};
        _t4353 = gem_has_key_fn(NULL, _t4352, 2);
    }
    GemVal _t4354;
    if (!gem_truthy(_t4353)) {
        _t4354 = _t4353;
    } else {
        _t4354 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("binop"));
    }
    GemVal _t4356;
    if (!gem_truthy(_t4354)) {
        _t4356 = _t4354;
    } else {
        GemVal _t4355[] = {gem_v__match_4, gem_string("left")};
        _t4356 = gem_has_key_fn(NULL, _t4355, 2);
    }
    GemVal _t4358;
    if (!gem_truthy(_t4356)) {
        _t4358 = _t4356;
    } else {
        GemVal _t4357[] = {gem_v__match_4, gem_string("right")};
        _t4358 = gem_has_key_fn(NULL, _t4357, 2);
    }
        if (gem_truthy(_t4358)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_left = gem_table_get(gem_v__match_4, gem_string("left"));
#line 0 "compiler/main.gem"
            GemVal gem_v_right = gem_table_get(gem_v__match_4, gem_string("right"));
#line 138 "compiler/main.gem"
    GemVal _t4359[] = {gem_v_left, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4359, 3));
#line 139 "compiler/main.gem"
            GemVal _t4360 = gem_v_right;
            GemVal _t4361 = gem_v_rmap;
            GemVal _t4362 = gem_v_shadowed;
            gem_v_node = _t4360;
            gem_v_rmap = _t4361;
            gem_v_shadowed = _t4362;
            continue;
        } else {
    GemVal _t4363[] = {gem_v__match_4};
    GemVal _t4365;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4363, 1), gem_string("table")))) {
        _t4365 = gem_eq(gem_type_fn(NULL, _t4363, 1), gem_string("table"));
    } else {
        GemVal _t4364[] = {gem_v__match_4, gem_string("tag")};
        _t4365 = gem_has_key_fn(NULL, _t4364, 2);
    }
    GemVal _t4366;
    if (!gem_truthy(_t4365)) {
        _t4366 = _t4365;
    } else {
        _t4366 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t4368;
    if (!gem_truthy(_t4366)) {
        _t4368 = _t4366;
    } else {
        GemVal _t4367[] = {gem_v__match_4, gem_string("expr")};
        _t4368 = gem_has_key_fn(NULL, _t4367, 2);
    }
        if (gem_truthy(_t4368)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_expr = gem_table_get(gem_v__match_4, gem_string("expr"));
#line 141 "compiler/main.gem"
            GemVal _t4369 = gem_v_expr;
            GemVal _t4370 = gem_v_rmap;
            GemVal _t4371 = gem_v_shadowed;
            gem_v_node = _t4369;
            gem_v_rmap = _t4370;
            gem_v_shadowed = _t4371;
            continue;
        } else {
    GemVal _t4372[] = {gem_v__match_4};
    GemVal _t4374;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4372, 1), gem_string("table")))) {
        _t4374 = gem_eq(gem_type_fn(NULL, _t4372, 1), gem_string("table"));
    } else {
        GemVal _t4373[] = {gem_v__match_4, gem_string("tag")};
        _t4374 = gem_has_key_fn(NULL, _t4373, 2);
    }
    GemVal _t4375;
    if (!gem_truthy(_t4374)) {
        _t4375 = _t4374;
    } else {
        _t4375 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t4377;
    if (!gem_truthy(_t4375)) {
        _t4377 = _t4375;
    } else {
        GemVal _t4376[] = {gem_v__match_4, gem_string("object")};
        _t4377 = gem_has_key_fn(NULL, _t4376, 2);
    }
        if (gem_truthy(_t4377)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 143 "compiler/main.gem"
            GemVal _t4378 = gem_v_object;
            GemVal _t4379 = gem_v_rmap;
            GemVal _t4380 = gem_v_shadowed;
            gem_v_node = _t4378;
            gem_v_rmap = _t4379;
            gem_v_shadowed = _t4380;
            continue;
        } else {
    GemVal _t4381[] = {gem_v__match_4};
    GemVal _t4383;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4381, 1), gem_string("table")))) {
        _t4383 = gem_eq(gem_type_fn(NULL, _t4381, 1), gem_string("table"));
    } else {
        GemVal _t4382[] = {gem_v__match_4, gem_string("tag")};
        _t4383 = gem_has_key_fn(NULL, _t4382, 2);
    }
    GemVal _t4384;
    if (!gem_truthy(_t4383)) {
        _t4384 = _t4383;
    } else {
        _t4384 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index"));
    }
    GemVal _t4386;
    if (!gem_truthy(_t4384)) {
        _t4386 = _t4384;
    } else {
        GemVal _t4385[] = {gem_v__match_4, gem_string("object")};
        _t4386 = gem_has_key_fn(NULL, _t4385, 2);
    }
    GemVal _t4388;
    if (!gem_truthy(_t4386)) {
        _t4388 = _t4386;
    } else {
        GemVal _t4387[] = {gem_v__match_4, gem_string("key")};
        _t4388 = gem_has_key_fn(NULL, _t4387, 2);
    }
        if (gem_truthy(_t4388)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 145 "compiler/main.gem"
    GemVal _t4389[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4389, 3));
#line 146 "compiler/main.gem"
            GemVal _t4390 = gem_v_key;
            GemVal _t4391 = gem_v_rmap;
            GemVal _t4392 = gem_v_shadowed;
            gem_v_node = _t4390;
            gem_v_rmap = _t4391;
            gem_v_shadowed = _t4392;
            continue;
        } else {
    GemVal _t4393[] = {gem_v__match_4};
    GemVal _t4395;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4393, 1), gem_string("table")))) {
        _t4395 = gem_eq(gem_type_fn(NULL, _t4393, 1), gem_string("table"));
    } else {
        GemVal _t4394[] = {gem_v__match_4, gem_string("tag")};
        _t4395 = gem_has_key_fn(NULL, _t4394, 2);
    }
    GemVal _t4396;
    if (!gem_truthy(_t4395)) {
        _t4396 = _t4395;
    } else {
        _t4396 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t4398;
    if (!gem_truthy(_t4396)) {
        _t4398 = _t4396;
    } else {
        GemVal _t4397[] = {gem_v__match_4, gem_string("object")};
        _t4398 = gem_has_key_fn(NULL, _t4397, 2);
    }
    GemVal _t4400;
    if (!gem_truthy(_t4398)) {
        _t4400 = _t4398;
    } else {
        GemVal _t4399[] = {gem_v__match_4, gem_string("value")};
        _t4400 = gem_has_key_fn(NULL, _t4399, 2);
    }
        if (gem_truthy(_t4400)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 148 "compiler/main.gem"
    GemVal _t4401[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4401, 3));
#line 149 "compiler/main.gem"
            GemVal _t4402 = gem_v_value;
            GemVal _t4403 = gem_v_rmap;
            GemVal _t4404 = gem_v_shadowed;
            gem_v_node = _t4402;
            gem_v_rmap = _t4403;
            gem_v_shadowed = _t4404;
            continue;
        } else {
    GemVal _t4405[] = {gem_v__match_4};
    GemVal _t4407;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4405, 1), gem_string("table")))) {
        _t4407 = gem_eq(gem_type_fn(NULL, _t4405, 1), gem_string("table"));
    } else {
        GemVal _t4406[] = {gem_v__match_4, gem_string("tag")};
        _t4407 = gem_has_key_fn(NULL, _t4406, 2);
    }
    GemVal _t4408;
    if (!gem_truthy(_t4407)) {
        _t4408 = _t4407;
    } else {
        _t4408 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t4410;
    if (!gem_truthy(_t4408)) {
        _t4410 = _t4408;
    } else {
        GemVal _t4409[] = {gem_v__match_4, gem_string("object")};
        _t4410 = gem_has_key_fn(NULL, _t4409, 2);
    }
    GemVal _t4412;
    if (!gem_truthy(_t4410)) {
        _t4412 = _t4410;
    } else {
        GemVal _t4411[] = {gem_v__match_4, gem_string("key")};
        _t4412 = gem_has_key_fn(NULL, _t4411, 2);
    }
    GemVal _t4414;
    if (!gem_truthy(_t4412)) {
        _t4414 = _t4412;
    } else {
        GemVal _t4413[] = {gem_v__match_4, gem_string("value")};
        _t4414 = gem_has_key_fn(NULL, _t4413, 2);
    }
        if (gem_truthy(_t4414)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 151 "compiler/main.gem"
    GemVal _t4415[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4415, 3));
#line 152 "compiler/main.gem"
    GemVal _t4416[] = {gem_v_key, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4416, 3));
#line 153 "compiler/main.gem"
            GemVal _t4417 = gem_v_value;
            GemVal _t4418 = gem_v_rmap;
            GemVal _t4419 = gem_v_shadowed;
            gem_v_node = _t4417;
            gem_v_rmap = _t4418;
            gem_v_shadowed = _t4419;
            continue;
        } else {
    GemVal _t4420[] = {gem_v__match_4};
    GemVal _t4422;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4420, 1), gem_string("table")))) {
        _t4422 = gem_eq(gem_type_fn(NULL, _t4420, 1), gem_string("table"));
    } else {
        GemVal _t4421[] = {gem_v__match_4, gem_string("tag")};
        _t4422 = gem_has_key_fn(NULL, _t4421, 2);
    }
    GemVal _t4423;
    if (!gem_truthy(_t4422)) {
        _t4423 = _t4422;
    } else {
        _t4423 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("if"));
    }
    GemVal _t4425;
    if (!gem_truthy(_t4423)) {
        _t4425 = _t4423;
    } else {
        GemVal _t4424[] = {gem_v__match_4, gem_string("cond")};
        _t4425 = gem_has_key_fn(NULL, _t4424, 2);
    }
        if (gem_truthy(_t4425)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 155 "compiler/main.gem"
    GemVal _t4426[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4426, 3));
#line 156 "compiler/main.gem"
    GemVal _t4427 = gem_v_node;
    GemVal _t4428[] = {gem_table_get(_t4427, gem_string("then")), gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_stmts(NULL, _t4428, 3));
#line 157 "compiler/main.gem"
    GemVal _t4429 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4429, gem_string("else")), GEM_NIL))) {
#line 158 "compiler/main.gem"
    GemVal _t4430 = gem_v_node;
    GemVal _t4431[] = {gem_table_get(_t4430, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4432 = gem_fn_rename_stmts(NULL, _t4431, 3);
                gem_pop_frame();
                return _t4432;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4433[] = {gem_v__match_4};
    GemVal _t4435;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4433, 1), gem_string("table")))) {
        _t4435 = gem_eq(gem_type_fn(NULL, _t4433, 1), gem_string("table"));
    } else {
        GemVal _t4434[] = {gem_v__match_4, gem_string("tag")};
        _t4435 = gem_has_key_fn(NULL, _t4434, 2);
    }
    GemVal _t4436;
    if (!gem_truthy(_t4435)) {
        _t4436 = _t4435;
    } else {
        _t4436 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("while"));
    }
    GemVal _t4438;
    if (!gem_truthy(_t4436)) {
        _t4438 = _t4436;
    } else {
        GemVal _t4437[] = {gem_v__match_4, gem_string("cond")};
        _t4438 = gem_has_key_fn(NULL, _t4437, 2);
    }
    GemVal _t4440;
    if (!gem_truthy(_t4438)) {
        _t4440 = _t4438;
    } else {
        GemVal _t4439[] = {gem_v__match_4, gem_string("body")};
        _t4440 = gem_has_key_fn(NULL, _t4439, 2);
    }
        if (gem_truthy(_t4440)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 161 "compiler/main.gem"
    GemVal _t4441[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4441, 3));
#line 162 "compiler/main.gem"
    GemVal _t4442[] = {gem_v_body, gem_v_rmap, gem_v_shadowed};
            GemVal _t4443 = gem_fn_rename_stmts(NULL, _t4442, 3);
            gem_pop_frame();
            return _t4443;
        } else {
    GemVal _t4444[] = {gem_v__match_4};
    GemVal _t4446;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4444, 1), gem_string("table")))) {
        _t4446 = gem_eq(gem_type_fn(NULL, _t4444, 1), gem_string("table"));
    } else {
        GemVal _t4445[] = {gem_v__match_4, gem_string("tag")};
        _t4446 = gem_has_key_fn(NULL, _t4445, 2);
    }
    GemVal _t4447;
    if (!gem_truthy(_t4446)) {
        _t4447 = _t4446;
    } else {
        _t4447 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("match"));
    }
    GemVal _t4449;
    if (!gem_truthy(_t4447)) {
        _t4449 = _t4447;
    } else {
        GemVal _t4448[] = {gem_v__match_4, gem_string("target")};
        _t4449 = gem_has_key_fn(NULL, _t4448, 2);
    }
    GemVal _t4451;
    if (!gem_truthy(_t4449)) {
        _t4451 = _t4449;
    } else {
        GemVal _t4450[] = {gem_v__match_4, gem_string("whens")};
        _t4451 = gem_has_key_fn(NULL, _t4450, 2);
    }
        if (gem_truthy(_t4451)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_target = gem_table_get(gem_v__match_4, gem_string("target"));
#line 0 "compiler/main.gem"
            GemVal gem_v_whens = gem_table_get(gem_v__match_4, gem_string("whens"));
#line 164 "compiler/main.gem"
    GemVal _t4452[] = {gem_v_target, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4452, 3));
#line 165 "compiler/main.gem"
            GemVal gem_v__for_items_8 = gem_v_whens;
#line 165 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 165 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4453[] = {gem_v__for_items_8};
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_len_fn(NULL, _t4453, 1)))) break;
#line 165 "compiler/main.gem"
                GemVal gem_v_w = gem_table_get(gem_v__for_items_8, gem_v__for_i_8);
#line 165 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 166 "compiler/main.gem"
    GemVal _t4454 = gem_v_w;
    GemVal _t4455[] = {gem_table_get(_t4454, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4455, 3));
#line 167 "compiler/main.gem"
    GemVal _t4456 = gem_v_w;
                if (gem_truthy(gem_neq(gem_table_get(_t4456, gem_string("bindings")), GEM_NIL))) {
#line 168 "compiler/main.gem"
    GemVal _t4457 = gem_v_w;
                    GemVal gem_v__for_items_9 = gem_table_get(_t4457, gem_string("bindings"));
#line 168 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 168 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4458[] = {gem_v__for_items_9};
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t4458, 1)))) break;
#line 168 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_9, gem_v__for_i_9);
#line 168 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 169 "compiler/main.gem"
    GemVal _t4459[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4459, 3));
                    }

                }
#line 172 "compiler/main.gem"
    GemVal _t4460 = gem_v_w;
    GemVal _t4461[] = {gem_table_get(_t4460, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4461, 3));
            }

#line 174 "compiler/main.gem"
    GemVal _t4462 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4462, gem_string("else")), GEM_NIL))) {
#line 175 "compiler/main.gem"
    GemVal _t4463 = gem_v_node;
    GemVal _t4464[] = {gem_table_get(_t4463, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4465 = gem_fn_rename_stmts(NULL, _t4464, 3);
                gem_pop_frame();
                return _t4465;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4466[] = {gem_v__match_4};
    GemVal _t4468;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4466, 1), gem_string("table")))) {
        _t4468 = gem_eq(gem_type_fn(NULL, _t4466, 1), gem_string("table"));
    } else {
        GemVal _t4467[] = {gem_v__match_4, gem_string("tag")};
        _t4468 = gem_has_key_fn(NULL, _t4467, 2);
    }
    GemVal _t4469;
    if (!gem_truthy(_t4468)) {
        _t4469 = _t4468;
    } else {
        _t4469 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t4471;
    if (!gem_truthy(_t4469)) {
        _t4471 = _t4469;
    } else {
        GemVal _t4470[] = {gem_v__match_4, gem_string("arms")};
        _t4471 = gem_has_key_fn(NULL, _t4470, 2);
    }
        if (gem_truthy(_t4471)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_arms = gem_table_get(gem_v__match_4, gem_string("arms"));
#line 178 "compiler/main.gem"
            GemVal gem_v__for_items_10 = gem_v_arms;
#line 178 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 178 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4472[] = {gem_v__for_items_10};
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t4472, 1)))) break;
#line 178 "compiler/main.gem"
                GemVal gem_v_arm = gem_table_get(gem_v__for_items_10, gem_v__for_i_10);
#line 178 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 179 "compiler/main.gem"
    GemVal _t4473 = gem_v_arm;
    GemVal _t4474 = gem_table_get(_t4473, gem_string("pattern"));
    GemVal _t4475[] = {gem_table_get(_t4474, gem_string("condition")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4475, 3));
#line 180 "compiler/main.gem"
    GemVal _t4476 = gem_v_arm;
    GemVal _t4477 = gem_table_get(_t4476, gem_string("pattern"));
                if (gem_truthy(gem_neq(gem_table_get(_t4477, gem_string("bindings")), GEM_NIL))) {
#line 181 "compiler/main.gem"
    GemVal _t4478 = gem_v_arm;
    GemVal _t4479 = gem_table_get(_t4478, gem_string("pattern"));
                    GemVal gem_v__for_items_11 = gem_table_get(_t4479, gem_string("bindings"));
#line 181 "compiler/main.gem"
                    GemVal gem_v__for_i_11 = gem_int(0);
#line 181 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4480[] = {gem_v__for_items_11};
                        if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t4480, 1)))) break;
#line 181 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_11, gem_v__for_i_11);
#line 181 "compiler/main.gem"
                        gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 182 "compiler/main.gem"
    GemVal _t4481[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4481, 3));
                    }

                }
#line 185 "compiler/main.gem"
    GemVal _t4482 = gem_v_arm;
    GemVal _t4483[] = {gem_table_get(_t4482, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4483, 3));
            }

#line 187 "compiler/main.gem"
    GemVal _t4484 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4484, gem_string("after_ms")), GEM_NIL))) {
#line 188 "compiler/main.gem"
    GemVal _t4485 = gem_v_node;
    GemVal _t4486[] = {gem_table_get(_t4485, gem_string("after_ms")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4486, 3));
            }
#line 190 "compiler/main.gem"
    GemVal _t4487 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4487, gem_string("after_body")), GEM_NIL))) {
#line 191 "compiler/main.gem"
    GemVal _t4488 = gem_v_node;
    GemVal _t4489[] = {gem_table_get(_t4488, gem_string("after_body")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4490 = gem_fn_rename_stmts(NULL, _t4489, 3);
                gem_pop_frame();
                return _t4490;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4491[] = {gem_v__match_4};
    GemVal _t4493;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4491, 1), gem_string("table")))) {
        _t4493 = gem_eq(gem_type_fn(NULL, _t4491, 1), gem_string("table"));
    } else {
        GemVal _t4492[] = {gem_v__match_4, gem_string("tag")};
        _t4493 = gem_has_key_fn(NULL, _t4492, 2);
    }
    GemVal _t4494;
    if (!gem_truthy(_t4493)) {
        _t4494 = _t4493;
    } else {
        _t4494 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("return"));
    }
    GemVal _t4496;
    if (!gem_truthy(_t4494)) {
        _t4496 = _t4494;
    } else {
        GemVal _t4495[] = {gem_v__match_4, gem_string("value")};
        _t4496 = gem_has_key_fn(NULL, _t4495, 2);
    }
        if (gem_truthy(_t4496)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 194 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 195 "compiler/main.gem"
                GemVal _t4497 = gem_v_value;
                GemVal _t4498 = gem_v_rmap;
                GemVal _t4499 = gem_v_shadowed;
                gem_v_node = _t4497;
                gem_v_rmap = _t4498;
                gem_v_shadowed = _t4499;
                continue;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4500[] = {gem_v__match_4};
    GemVal _t4502;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4500, 1), gem_string("table")))) {
        _t4502 = gem_eq(gem_type_fn(NULL, _t4500, 1), gem_string("table"));
    } else {
        GemVal _t4501[] = {gem_v__match_4, gem_string("tag")};
        _t4502 = gem_has_key_fn(NULL, _t4501, 2);
    }
    GemVal _t4503;
    if (!gem_truthy(_t4502)) {
        _t4503 = _t4502;
    } else {
        _t4503 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("table"));
    }
    GemVal _t4505;
    if (!gem_truthy(_t4503)) {
        _t4505 = _t4503;
    } else {
        GemVal _t4504[] = {gem_v__match_4, gem_string("entries")};
        _t4505 = gem_has_key_fn(NULL, _t4504, 2);
    }
        if (gem_truthy(_t4505)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_entries = gem_table_get(gem_v__match_4, gem_string("entries"));
            {
#line 198 "compiler/main.gem"
                GemVal gem_v__for_items_12 = gem_v_entries;
#line 198 "compiler/main.gem"
                GemVal gem_v__for_i_12 = gem_int(0);
#line 198 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4506[] = {gem_v__for_items_12};
                    if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t4506, 1)))) break;
#line 198 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 198 "compiler/main.gem"
                    gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 199 "compiler/main.gem"
    GemVal _t4507 = gem_v_e;
    GemVal _t4508[] = {gem_table_get(_t4507, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4508, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4509[] = {gem_v__match_4};
    GemVal _t4511;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4509, 1), gem_string("table")))) {
        _t4511 = gem_eq(gem_type_fn(NULL, _t4509, 1), gem_string("table"));
    } else {
        GemVal _t4510[] = {gem_v__match_4, gem_string("tag")};
        _t4511 = gem_has_key_fn(NULL, _t4510, 2);
    }
    GemVal _t4512;
    if (!gem_truthy(_t4511)) {
        _t4512 = _t4511;
    } else {
        _t4512 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("array"));
    }
    GemVal _t4514;
    if (!gem_truthy(_t4512)) {
        _t4514 = _t4512;
    } else {
        GemVal _t4513[] = {gem_v__match_4, gem_string("elements")};
        _t4514 = gem_has_key_fn(NULL, _t4513, 2);
    }
        if (gem_truthy(_t4514)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_elements = gem_table_get(gem_v__match_4, gem_string("elements"));
            {
#line 202 "compiler/main.gem"
                GemVal gem_v__for_items_13 = gem_v_elements;
#line 202 "compiler/main.gem"
                GemVal gem_v__for_i_13 = gem_int(0);
#line 202 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4515[] = {gem_v__for_items_13};
                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_len_fn(NULL, _t4515, 1)))) break;
#line 202 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_13, gem_v__for_i_13);
#line 202 "compiler/main.gem"
                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 203 "compiler/main.gem"
    GemVal _t4516[] = {gem_v_e, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4516, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4517[] = {gem_v__match_4};
    GemVal _t4519;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4517, 1), gem_string("table")))) {
        _t4519 = gem_eq(gem_type_fn(NULL, _t4517, 1), gem_string("table"));
    } else {
        GemVal _t4518[] = {gem_v__match_4, gem_string("tag")};
        _t4519 = gem_has_key_fn(NULL, _t4518, 2);
    }
    GemVal _t4520;
    if (!gem_truthy(_t4519)) {
        _t4520 = _t4519;
    } else {
        _t4520 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t4522;
    if (!gem_truthy(_t4520)) {
        _t4522 = _t4520;
    } else {
        GemVal _t4521[] = {gem_v__match_4, gem_string("parts")};
        _t4522 = gem_has_key_fn(NULL, _t4521, 2);
    }
        if (gem_truthy(_t4522)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_parts = gem_table_get(gem_v__match_4, gem_string("parts"));
            {
#line 206 "compiler/main.gem"
                GemVal gem_v__for_items_14 = gem_v_parts;
#line 206 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 206 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4523[] = {gem_v__for_items_14};
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_len_fn(NULL, _t4523, 1)))) break;
#line 206 "compiler/main.gem"
                    GemVal gem_v_p = gem_table_get(gem_v__for_items_14, gem_v__for_i_14);
#line 206 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 207 "compiler/main.gem"
    GemVal _t4524[] = {gem_v_p, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4524, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4525[] = {gem_v__match_4};
    GemVal _t4527;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4525, 1), gem_string("table")))) {
        _t4527 = gem_eq(gem_type_fn(NULL, _t4525, 1), gem_string("table"));
    } else {
        GemVal _t4526[] = {gem_v__match_4, gem_string("tag")};
        _t4527 = gem_has_key_fn(NULL, _t4526, 2);
    }
    GemVal _t4528;
    if (!gem_truthy(_t4527)) {
        _t4528 = _t4527;
    } else {
        _t4528 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("block"));
    }
    GemVal _t4530;
    if (!gem_truthy(_t4528)) {
        _t4530 = _t4528;
    } else {
        GemVal _t4529[] = {gem_v__match_4, gem_string("stmts")};
        _t4530 = gem_has_key_fn(NULL, _t4529, 2);
    }
        if (gem_truthy(_t4530)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_stmts = gem_table_get(gem_v__match_4, gem_string("stmts"));
#line 210 "compiler/main.gem"
    GemVal _t4531[] = {gem_v_stmts, gem_v_rmap, gem_v_shadowed};
            GemVal _t4532 = gem_fn_rename_stmts(NULL, _t4531, 3);
            gem_pop_frame();
            return _t4532;
        } else {
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
        }
        }
        }
        }
        }
    }
}

static GemVal gem_fn_rename_stmts(void *_env, GemVal *args, int argc) {
#line 214 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rmap = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_shadowed = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("rename_stmts", "compiler/main.gem", 214);
#line 215 "compiler/main.gem"
    GemVal _t4533[] = {gem_v_shadowed};
    GemVal gem_v_current = gem_fn_copy_set(NULL, _t4533, 1);
    {
#line 216 "compiler/main.gem"
        GemVal gem_v__for_items_15 = gem_v_stmts;
#line 216 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 216 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4534[] = {gem_v__for_items_15};
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_len_fn(NULL, _t4534, 1)))) break;
#line 216 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_15, gem_v__for_i_15);
#line 216 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 217 "compiler/main.gem"
    GemVal _t4535[] = {gem_v_stmt, gem_v_rmap, gem_v_current};
            (void)(gem_fn_rename_node(NULL, _t4535, 3));
#line 218 "compiler/main.gem"
    GemVal _t4536[] = {gem_v_stmt, gem_string("let")};
    GemVal _t4539;
    if (!gem_truthy(gem_fn_is_node(NULL, _t4536, 2))) {
        _t4539 = gem_fn_is_node(NULL, _t4536, 2);
    } else {
        GemVal _t4537 = gem_v_stmt;
        GemVal _t4538[] = {gem_v_rmap, gem_table_get(_t4537, gem_string("name"))};
        _t4539 = gem_not(gem_has_key_fn(NULL, _t4538, 2));
    }
            if (gem_truthy(_t4539)) {
#line 219 "compiler/main.gem"
    GemVal _t4540 = gem_v_stmt;
                gem_table_set(gem_v_current, gem_table_get(_t4540, gem_string("name")), gem_bool(1));
            }
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_transform_module(void *_env, GemVal *args, int argc) {
#line 224 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mod_name = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_export_node = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_load_node = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("transform_module", "compiler/main.gem", 224);
#line 225 "compiler/main.gem"
    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_v_mod_name), gem_string("_"));
#line 226 "compiler/main.gem"
    GemVal _t4541[] = {gem_v_stmts};
    GemVal gem_v_top_names = gem_fn_collect_module_top_names(NULL, _t4541, 1);
#line 227 "compiler/main.gem"
    GemVal _t4542[] = {gem_v_top_names, gem_v_prefix};
    GemVal gem_v_rmap = gem_fn_build_rename_map(NULL, _t4542, 2);
#line 229 "compiler/main.gem"
    GemVal _t4543 = gem_table_new();
    GemVal _t4544[] = {gem_v_stmts, gem_v_rmap, _t4543};
    (void)(gem_fn_rename_stmts(NULL, _t4544, 3));
#line 231 "compiler/main.gem"
    GemVal _t4545 = gem_table_new();
    GemVal gem_v_result = _t4545;
#line 232 "compiler/main.gem"
    GemVal gem_v__for_items_16 = gem_v_stmts;
#line 232 "compiler/main.gem"
    GemVal gem_v__for_i_16 = gem_int(0);
#line 232 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4546[] = {gem_v__for_items_16};
        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t4546, 1)))) break;
#line 232 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_16, gem_v__for_i_16);
#line 232 "compiler/main.gem"
        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 233 "compiler/main.gem"
    GemVal _t4547[] = {gem_v_stmt, gem_string("export")};
        if (gem_truthy(gem_not(gem_fn_is_node(NULL, _t4547, 2)))) {
#line 234 "compiler/main.gem"
    GemVal _t4548[] = {gem_v_result, gem_v_stmt};
            (void)(gem_push_fn(NULL, _t4548, 2));
        }
    }

#line 238 "compiler/main.gem"
    GemVal _t4549 = gem_v_export_node;
    GemVal gem_v_exported = gem_table_get(_t4549, gem_string("names"));
#line 240 "compiler/main.gem"
    GemVal _t4550 = gem_v_load_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4550, gem_string("selective")), GEM_NIL))) {
#line 241 "compiler/main.gem"
    GemVal _t4551 = gem_v_load_node;
        GemVal gem_v__for_items_17 = gem_table_get(_t4551, gem_string("selective"));
#line 241 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 241 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4552[] = {gem_v__for_items_17};
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_len_fn(NULL, _t4552, 1)))) break;
#line 241 "compiler/main.gem"
            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_17, gem_v__for_i_17);
#line 241 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 242 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_sel_name);
#line 243 "compiler/main.gem"
    GemVal _t4553[] = {gem_v_prefixed};
    GemVal _t4554[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4553, 1), gem_int(0)};
    GemVal _t4555[] = {gem_v_result, gem_fn_make_let(NULL, _t4554, 3)};
            (void)(gem_push_fn(NULL, _t4555, 2));
        }

    } else {
#line 246 "compiler/main.gem"
        GemVal gem_v_binding_name = gem_v_mod_name;
#line 247 "compiler/main.gem"
    GemVal _t4556 = gem_v_load_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4556, gem_string("alias")), GEM_NIL))) {
#line 248 "compiler/main.gem"
    GemVal _t4557 = gem_v_load_node;
            gem_v_binding_name = gem_table_get(_t4557, gem_string("alias"));
        }
#line 250 "compiler/main.gem"
    GemVal _t4558 = gem_table_new();
        GemVal gem_v_entries = _t4558;
#line 251 "compiler/main.gem"
        GemVal gem_v__for_items_18 = gem_v_exported;
#line 251 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 251 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4559[] = {gem_v__for_items_18};
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_len_fn(NULL, _t4559, 1)))) break;
#line 251 "compiler/main.gem"
            GemVal gem_v_ename = gem_table_get(gem_v__for_items_18, gem_v__for_i_18);
#line 251 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 252 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_ename);
#line 253 "compiler/main.gem"
    GemVal _t4560[] = {gem_v_prefixed};
    GemVal _t4561[] = {gem_v_ename, gem_fn_make_var(NULL, _t4560, 1)};
    GemVal _t4562[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4561, 2)};
            (void)(gem_push_fn(NULL, _t4562, 2));
        }

#line 255 "compiler/main.gem"
    GemVal _t4563[] = {gem_v_entries};
    GemVal _t4564[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4563, 1), gem_int(0)};
    GemVal _t4565[] = {gem_v_result, gem_fn_make_let(NULL, _t4564, 3)};
        (void)(gem_push_fn(NULL, _t4565, 2));
    }
    GemVal _t4566 = gem_v_result;
    gem_pop_frame();
    return _t4566;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 263 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 263);
#line 264 "compiler/main.gem"
    GemVal _t4567 = gem_table_new();
    GemVal gem_v_new_stmts = _t4567;
#line 265 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 266 "compiler/main.gem"
    GemVal _t4568 = gem_v_ast;
    GemVal gem_v__for_items_19 = gem_table_get(_t4568, gem_string("stmts"));
#line 266 "compiler/main.gem"
    GemVal gem_v__for_i_19 = gem_int(0);
#line 266 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4569[] = {gem_v__for_items_19};
        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t4569, 1)))) break;
#line 266 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
#line 266 "compiler/main.gem"
        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 267 "compiler/main.gem"
    GemVal _t4570[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4570, 2))) {
#line 268 "compiler/main.gem"
    GemVal _t4571 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t4571, gem_string("path"));
#line 270 "compiler/main.gem"
    GemVal _t4572 = gem_v_string;
    GemVal _t4573[] = {gem_v_path, gem_string(".gem")};
    GemVal _t4574 = gem_table_get(_t4572, gem_string("ends_with"));
            if (gem_truthy(gem_not(_t4574.fn(_t4574.env, _t4573, 2)))) {
#line 271 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 275 "compiler/main.gem"
    GemVal _t4575[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t4575, 2);
#line 278 "compiler/main.gem"
    GemVal _t4576[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t4576, 1)))) {
#line 279 "compiler/main.gem"
    GemVal _t4577[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t4577, 2);
            }
#line 282 "compiler/main.gem"
    GemVal _t4578[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t4578, 1);
#line 285 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 288 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 290 "compiler/main.gem"
    GemVal _t4579[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t4579, 1);
#line 292 "compiler/main.gem"
    GemVal _t4580[] = {gem_v_source, gem_v_full_path};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t4580, 2);
#line 293 "compiler/main.gem"
    GemVal _t4581[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t4581, 1);
#line 294 "compiler/main.gem"
    GemVal _t4582[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t4582, 3);
#line 296 "compiler/main.gem"
    GemVal _t4583 = gem_v_resolved;
    GemVal _t4584[] = {gem_table_get(_t4583, gem_string("stmts"))};
            GemVal gem_v_export_node = gem_fn_find_export_node(NULL, _t4584, 1);
#line 297 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_export_node, GEM_NIL))) {
#line 298 "compiler/main.gem"
    GemVal _t4585 = gem_v_stmt;
    GemVal _t4586[] = {gem_table_get(_t4585, gem_string("path"))};
                GemVal gem_v_mod_name = gem_fn_module_name_from_path(NULL, _t4586, 1);
#line 299 "compiler/main.gem"
    GemVal _t4587 = gem_v_resolved;
    GemVal _t4588[] = {gem_table_get(_t4587, gem_string("stmts")), gem_v_mod_name, gem_v_export_node, gem_v_stmt};
                GemVal gem_v_mod_stmts = gem_fn_transform_module(NULL, _t4588, 4);
#line 300 "compiler/main.gem"
                GemVal gem_v__for_items_20 = gem_v_mod_stmts;
#line 300 "compiler/main.gem"
                GemVal gem_v__for_i_20 = gem_int(0);
#line 300 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4589[] = {gem_v__for_items_20};
                    if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t4589, 1)))) break;
#line 300 "compiler/main.gem"
                    GemVal gem_v_ms = gem_table_get(gem_v__for_items_20, gem_v__for_i_20);
#line 300 "compiler/main.gem"
                    gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 301 "compiler/main.gem"
                    gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_ms);
#line 302 "compiler/main.gem"
                    gem_v_count = gem_add(gem_v_count, gem_int(1));
                }

            } else {
#line 305 "compiler/main.gem"
    GemVal _t4590 = gem_v_resolved;
                GemVal gem_v__for_items_21 = gem_table_get(_t4590, gem_string("stmts"));
#line 305 "compiler/main.gem"
                GemVal gem_v__for_i_21 = gem_int(0);
#line 305 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4591[] = {gem_v__for_items_21};
                    if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t4591, 1)))) break;
#line 305 "compiler/main.gem"
                    GemVal gem_v_rs = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 305 "compiler/main.gem"
                    gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 306 "compiler/main.gem"
                    gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
#line 307 "compiler/main.gem"
                    gem_v_count = gem_add(gem_v_count, gem_int(1));
                }

            }
        } else {
#line 311 "compiler/main.gem"
            gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 312 "compiler/main.gem"
            gem_v_count = gem_add(gem_v_count, gem_int(1));
        }
    }

#line 315 "compiler/main.gem"
    GemVal _t4592[] = {gem_v_new_stmts};
    GemVal _t4593 = gem_fn_make_program(NULL, _t4592, 1);
    gem_pop_frame();
    return _t4593;
}

static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc) {
#line 320 "compiler/main.gem"
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_args", "compiler/main.gem", 320);
#line 321 "compiler/main.gem"
    GemVal _t4594 = gem_table_new();
    gem_table_set(_t4594, gem_string("src_path"), GEM_NIL);
    gem_table_set(_t4594, gem_string("out_name"), GEM_NIL);
    gem_table_set(_t4594, gem_string("emit_c"), gem_bool(0));
    gem_table_set(_t4594, gem_string("run"), gem_bool(0));
    GemVal _t4595 = gem_table_new();
    gem_table_set(_t4594, gem_string("run_args"), _t4595);
    GemVal gem_v_result = _t4594;
#line 329 "compiler/main.gem"
    GemVal _t4596[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t4596, 1);
#line 330 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 331 "compiler/main.gem"
    GemVal gem_v_run_arg_count = gem_int(0);
#line 332 "compiler/main.gem"
    GemVal gem_v_found_run = gem_bool(0);
#line 334 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_argc))) break;
#line 335 "compiler/main.gem"
        GemVal gem_v_arg = gem_table_get(gem_v_args, gem_v_i);
#line 336 "compiler/main.gem"
        if (gem_truthy(gem_v_found_run)) {
#line 337 "compiler/main.gem"
    GemVal _t4597 = gem_v_result;
            gem_table_set(gem_table_get(_t4597, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
#line 338 "compiler/main.gem"
            gem_v_run_arg_count = gem_add(gem_v_run_arg_count, gem_int(1));
        } else {
#line 339 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_arg, gem_string("--emit-c")))) {
#line 340 "compiler/main.gem"
                gem_table_set(gem_v_result, gem_string("emit_c"), gem_bool(1));
            } else {
#line 341 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_arg, gem_string("--run")))) {
#line 342 "compiler/main.gem"
                    gem_table_set(gem_v_result, gem_string("run"), gem_bool(1));
#line 343 "compiler/main.gem"
                    gem_v_found_run = gem_bool(1);
                } else {
#line 344 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_arg, gem_string("-o")))) {
#line 345 "compiler/main.gem"
                        gem_v_i = gem_add(gem_v_i, gem_int(1));
#line 346 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_i, gem_v_argc))) {
#line 347 "compiler/main.gem"
    GemVal _t4598[] = {gem_string("gem: -o requires an argument")};
                            (void)(gem_error_at_fn("compiler/main.gem", 347, _t4598, 1));
                        }
#line 349 "compiler/main.gem"
                        gem_table_set(gem_v_result, gem_string("out_name"), gem_table_get(gem_v_args, gem_v_i));
                    } else {
#line 350 "compiler/main.gem"
    GemVal _t4599 = gem_v_result;
                        if (gem_truthy(gem_eq(gem_table_get(_t4599, gem_string("src_path")), GEM_NIL))) {
#line 351 "compiler/main.gem"
                            gem_table_set(gem_v_result, gem_string("src_path"), gem_v_arg);
                        } else {
#line 354 "compiler/main.gem"
    GemVal _t4600 = gem_v_result;
                            gem_table_set(gem_table_get(_t4600, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
#line 355 "compiler/main.gem"
                            gem_v_run_arg_count = gem_add(gem_v_run_arg_count, gem_int(1));
                        }
                    }
                }
            }
        }
#line 357 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t4601 = gem_v_result;
    gem_pop_frame();
    return _t4601;
}

static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc) {
#line 365 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("basename_no_ext", "compiler/main.gem", 365);
#line 367 "compiler/main.gem"
    GemVal _t4602[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4602, 1);
#line 368 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 369 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 370 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 371 "compiler/main.gem"
    GemVal _t4603[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4603, 2), gem_int(47)))) {
#line 372 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 374 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 376 "compiler/main.gem"
    GemVal _t4604[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4604, 3);
#line 378 "compiler/main.gem"
    GemVal _t4605[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4605, 1);
#line 379 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 380 "compiler/main.gem"
    GemVal _t4606[] = {gem_v_name, gem_sub(gem_v_nlen, gem_int(4)), gem_int(4)};
        GemVal gem_v_maybe_ext = gem_substr_fn(NULL, _t4606, 3);
#line 381 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_maybe_ext, gem_string(".gem")))) {
#line 382 "compiler/main.gem"
    GemVal _t4607[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4607, 3);
        }
    }
    GemVal _t4608 = gem_v_name;
    gem_pop_frame();
    return _t4608;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_push_frame("main", "compiler/main.gem", 0);
#line 0 "compiler/main.gem"
    GemVal _t4609 = gem_table_new();
    gem_table_set(_t4609, gem_string("split"), gem_make_fn(gem_fn__mod_string_split, NULL));
    gem_table_set(_t4609, gem_string("index_of"), gem_make_fn(gem_fn__mod_string_index_of, NULL));
    gem_table_set(_t4609, gem_string("join"), gem_make_fn(gem_fn__mod_string_join, NULL));
    gem_table_set(_t4609, gem_string("trim"), gem_make_fn(gem_fn__mod_string_trim, NULL));
    gem_table_set(_t4609, gem_string("starts_with"), gem_make_fn(gem_fn__mod_string_starts_with, NULL));
    gem_table_set(_t4609, gem_string("ends_with"), gem_make_fn(gem_fn__mod_string_ends_with, NULL));
    gem_table_set(_t4609, gem_string("upper"), gem_make_fn(gem_fn__mod_string_upper, NULL));
    gem_table_set(_t4609, gem_string("lower"), gem_make_fn(gem_fn__mod_string_lower, NULL));
    gem_table_set(_t4609, gem_string("contains"), gem_make_fn(gem_fn__mod_string_contains, NULL));
    gem_table_set(_t4609, gem_string("repeat"), gem_make_fn(gem_fn__mod_string_repeat, NULL));
    gem_v_string = _t4609;
#line 16 "compiler/main.gem"
    GemVal _t4610[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t4611[] = {gem_dirname_fn(NULL, _t4610, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t4611, 1);
#line 390 "compiler/main.gem"
    GemVal _t4612[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t4612, 1);
#line 391 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 392 "compiler/main.gem"
    GemVal _t4613[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 392, _t4613, 1));
    }
#line 395 "compiler/main.gem"
    GemVal _t4614[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_cli = gem_fn_parse_args(NULL, _t4614, 1);
#line 397 "compiler/main.gem"
    GemVal _t4615 = gem_v_cli;
    if (gem_truthy(gem_eq(gem_table_get(_t4615, gem_string("src_path")), GEM_NIL))) {
#line 398 "compiler/main.gem"
    GemVal _t4616[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 398, _t4616, 1));
    }
#line 401 "compiler/main.gem"
    GemVal _t4617 = gem_v_cli;
    gem_v_src_path = gem_table_get(_t4617, gem_string("src_path"));
#line 402 "compiler/main.gem"
    GemVal _t4618[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t4618, 1);
#line 404 "compiler/main.gem"
    GemVal _t4619[] = {gem_v_source, gem_v_src_path};
    gem_v_ast = gem_fn_parse_source(NULL, _t4619, 2);
#line 405 "compiler/main.gem"
    GemVal _t4620[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t4620, 1);
#line 406 "compiler/main.gem"
    GemVal _t4621 = gem_table_new();
    gem_v_loaded = _t4621;
#line 407 "compiler/main.gem"
    GemVal _t4622[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t4622, 3);
#line 409 "compiler/main.gem"
    GemVal _t4623[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t4623, 1);
#line 410 "compiler/main.gem"
    GemVal _t4624 = gem_v_cg;
    GemVal _t4625[] = {gem_v_resolved_ast};
    GemVal _t4626 = gem_table_get(_t4624, gem_string("compile"));
    gem_v_c_code = _t4626.fn(_t4626.env, _t4625, 1);
#line 414 "compiler/main.gem"
    GemVal _t4627 = gem_v_cli;
    if (gem_truthy(gem_table_get(_t4627, gem_string("emit_c")))) {
#line 415 "compiler/main.gem"
    GemVal _t4628[] = {gem_v_c_code};
        (void)(gem_print(NULL, _t4628, 1));
    } else {
#line 419 "compiler/main.gem"
    GemVal _t4629[] = {gem_v_src_path};
        GemVal gem_v_base = gem_fn_basename_no_ext(NULL, _t4629, 1);
#line 420 "compiler/main.gem"
        GemVal gem_v_tmp_c = gem_add(gem_add(gem_string("/tmp/gem_"), gem_v_base), gem_string(".c"));
#line 423 "compiler/main.gem"
    GemVal _t4630 = gem_v_cli;
        GemVal gem_v_out_name = gem_table_get(_t4630, gem_string("out_name"));
#line 424 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_out_name, GEM_NIL))) {
#line 425 "compiler/main.gem"
            gem_v_out_name = gem_add(gem_string("./"), gem_v_base);
        }
#line 429 "compiler/main.gem"
    GemVal _t4631[] = {gem_v_tmp_c, gem_v_c_code};
        (void)(gem_write_file_fn(NULL, _t4631, 2));
#line 432 "compiler/main.gem"
    GemVal _t4632[] = {gem_v_install_root, gem_string("runtime")};
        GemVal gem_v_runtime_dir = gem_path_join_fn(NULL, _t4632, 2);
#line 435 "compiler/main.gem"
        GemVal gem_v_gc_flags = gem_string("-lgc");
#line 436 "compiler/main.gem"
    GemVal _t4633[] = {gem_string("pkg-config --cflags --libs bdw-gc > /tmp/gem_gc_flags.txt 2>/dev/null")};
        GemVal gem_v_pkg_ret = gem_exec_fn(NULL, _t4633, 1);
#line 437 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_pkg_ret, gem_int(0)))) {
#line 438 "compiler/main.gem"
    GemVal _t4634[] = {gem_string("/tmp/gem_gc_flags.txt")};
            gem_v_gc_flags = gem_read_file_fn(NULL, _t4634, 1);
#line 440 "compiler/main.gem"
    GemVal _t4635[] = {gem_v_gc_flags, gem_string("\n"), gem_string("")};
            gem_v_gc_flags = gem_str_replace_fn(NULL, _t4635, 3);
        }
#line 443 "compiler/main.gem"
        GemVal gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" ")), gem_v_runtime_dir), gem_string("/gem_*.c -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread"));
#line 445 "compiler/main.gem"
    GemVal _t4636[] = {gem_v_cc_cmd};
        GemVal gem_v_cc_ret = gem_exec_fn(NULL, _t4636, 1);
#line 446 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_cc_ret, gem_int(0)))) {
#line 447 "compiler/main.gem"
    GemVal _t4637[] = {gem_v_cc_ret};
    GemVal _t4638[] = {gem_add(gem_add(gem_string("gem: compilation failed (cc exited with "), gem_to_string_fn(NULL, _t4637, 1)), gem_string(")"))};
            (void)(gem_error_at_fn("compiler/main.gem", 447, _t4638, 1));
        }
#line 452 "compiler/main.gem"
    GemVal _t4639 = gem_v_cli;
        if (gem_truthy(gem_table_get(_t4639, gem_string("run")))) {
#line 453 "compiler/main.gem"
            GemVal gem_v_run_cmd = gem_v_out_name;
#line 454 "compiler/main.gem"
            GemVal gem_v_i = gem_int(0);
#line 455 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4640 = gem_v_cli;
                GemVal _t4641[] = {gem_table_get(_t4640, gem_string("run_args"))};
                if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t4641, 1)))) break;
#line 456 "compiler/main.gem"
    GemVal _t4642 = gem_v_cli;
                gem_v_run_cmd = gem_add(gem_add(gem_v_run_cmd, gem_string(" ")), gem_table_get(gem_table_get(_t4642, gem_string("run_args")), gem_v_i));
#line 457 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 459 "compiler/main.gem"
    GemVal _t4643[] = {gem_v_run_cmd};
            GemVal gem_v_run_ret = gem_exec_fn(NULL, _t4643, 1);
#line 460 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_run_ret, gem_int(0)))) {
#line 461 "compiler/main.gem"
    GemVal _t4644[] = {gem_v_run_ret};
                (void)(gem_exit_process_fn(NULL, _t4644, 1));
            }
        }
    }
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

