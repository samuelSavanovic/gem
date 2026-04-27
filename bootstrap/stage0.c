#include "gem.h"
double atof(const char*);
static GemVal gem_fn__str_eq_at(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_split(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_index_of(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_join(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_trim(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_starts_with(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_ends_with(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_upper(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_lower(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_contains(void *_env, GemVal *args, int argc);
static GemVal gem_fn__string_repeat(void *_env, GemVal *args, int argc);
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
static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc);
static GemVal gem_fn_collect_top_let_names(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc);
static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc);
static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc);
static GemVal gem_fn_atof(void *_env, GemVal *args, int argc);
GemVal gem_v_string;
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

static GemVal gem_fn__str_eq_at(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_pos = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_needle = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_str_eq_at", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1[] = {gem_v_needle};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t1, 1);
#line 11 "compiler/main.gem"
    GemVal _t2[] = {gem_v_s};
    if (gem_truthy(gem_gt(gem_add(gem_v_pos, gem_v_nlen), gem_len_fn(NULL, _t2, 1)))) {
#line 12 "compiler/main.gem"
        GemVal _t3 = gem_bool(0);
        gem_pop_frame();
        return _t3;
    }
#line 14 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 15 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_nlen))) break;
#line 16 "compiler/main.gem"
    GemVal _t4[] = {gem_v_s, gem_add(gem_v_pos, gem_v_i)};
    GemVal _t5[] = {gem_v_needle, gem_v_i};
        if (gem_truthy(gem_neq(gem_ord_fn(NULL, _t4, 2), gem_ord_fn(NULL, _t5, 2)))) {
#line 17 "compiler/main.gem"
            GemVal _t6 = gem_bool(0);
            gem_pop_frame();
            return _t6;
        }
#line 19 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t7 = gem_bool(1);
    gem_pop_frame();
    return _t7;
}

static GemVal gem_fn__string_split(void *_env, GemVal *args, int argc) {
#line 26 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_split", "compiler/main.gem", 26);
#line 27 "compiler/main.gem"
    GemVal _t8 = gem_table_new();
    GemVal gem_v_result = _t8;
#line 28 "compiler/main.gem"
    GemVal _t9[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t9, 1);
#line 29 "compiler/main.gem"
    GemVal _t10[] = {gem_v_delim};
    GemVal gem_v_dlen = gem_len_fn(NULL, _t10, 1);
#line 30 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_dlen, gem_int(0)))) {
#line 31 "compiler/main.gem"
    GemVal _t11[] = {gem_v_result, gem_v_s};
        (void)(gem_push_fn(NULL, _t11, 2));
#line 32 "compiler/main.gem"
        GemVal _t12 = gem_v_result;
        gem_pop_frame();
        return _t12;
    }
#line 34 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 35 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 36 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) break;
#line 37 "compiler/main.gem"
    GemVal _t13[] = {gem_v_s, gem_v_i, gem_v_delim};
        if (gem_truthy(gem_fn__str_eq_at(NULL, _t13, 3))) {
#line 38 "compiler/main.gem"
    GemVal _t14[] = {gem_v_buf};
    GemVal _t15[] = {gem_v_result, gem_buf_str_fn(NULL, _t14, 1)};
            (void)(gem_push_fn(NULL, _t15, 2));
#line 39 "compiler/main.gem"
            gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 40 "compiler/main.gem"
            gem_v_i = gem_add(gem_v_i, gem_v_dlen);
        } else {
#line 42 "compiler/main.gem"
    GemVal _t16[] = {gem_v_s, gem_v_i};
    GemVal _t17[] = {gem_ord_fn(NULL, _t16, 2)};
    GemVal _t18[] = {gem_v_buf, gem_chr_fn(NULL, _t17, 1)};
            (void)(gem_buf_push_fn(NULL, _t18, 2));
#line 43 "compiler/main.gem"
            gem_v_i = gem_add(gem_v_i, gem_int(1));
        }
    }
#line 46 "compiler/main.gem"
    GemVal _t19[] = {gem_v_buf};
    GemVal _t20[] = {gem_v_result, gem_buf_str_fn(NULL, _t19, 1)};
    (void)(gem_push_fn(NULL, _t20, 2));
    GemVal _t21 = gem_v_result;
    gem_pop_frame();
    return _t21;
}

static GemVal gem_fn__string_index_of(void *_env, GemVal *args, int argc) {
#line 52 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_needle = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_index_of", "compiler/main.gem", 52);
#line 53 "compiler/main.gem"
    GemVal _t22[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t22, 1);
#line 54 "compiler/main.gem"
    GemVal _t23[] = {gem_v_needle};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t23, 1);
#line 55 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_nlen, gem_int(0)))) {
#line 56 "compiler/main.gem"
        GemVal _t24 = gem_int(0);
        gem_pop_frame();
        return _t24;
    }
#line 58 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_v_slen))) {
#line 59 "compiler/main.gem"
        GemVal _t25 = gem_neg(gem_int(1));
        gem_pop_frame();
        return _t25;
    }
#line 61 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 62 "compiler/main.gem"
    GemVal gem_v_limit = gem_sub(gem_v_slen, gem_v_nlen);
#line 63 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_le(gem_v_i, gem_v_limit))) break;
#line 64 "compiler/main.gem"
    GemVal _t26[] = {gem_v_s, gem_v_i, gem_v_needle};
        if (gem_truthy(gem_fn__str_eq_at(NULL, _t26, 3))) {
#line 65 "compiler/main.gem"
            GemVal _t27 = gem_v_i;
            gem_pop_frame();
            return _t27;
        }
#line 67 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t28 = gem_neg(gem_int(1));
    gem_pop_frame();
    return _t28;
}

static GemVal gem_fn__string_join(void *_env, GemVal *args, int argc) {
#line 74 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_join", "compiler/main.gem", 74);
#line 75 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 76 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 77 "compiler/main.gem"
    GemVal _t29[] = {gem_v_arr};
    GemVal gem_v_n = gem_len_fn(NULL, _t29, 1);
#line 78 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_n))) break;
#line 79 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 80 "compiler/main.gem"
    GemVal _t30[] = {gem_v_buf, gem_v_delim};
            (void)(gem_buf_push_fn(NULL, _t30, 2));
        }
#line 82 "compiler/main.gem"
    GemVal _t31[] = {gem_v_buf, gem_table_get(gem_v_arr, gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t31, 2));
#line 83 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 85 "compiler/main.gem"
    GemVal _t32[] = {gem_v_buf};
    GemVal _t33 = gem_buf_str_fn(NULL, _t32, 1);
    gem_pop_frame();
    return _t33;
}

static GemVal gem_fn__string_trim(void *_env, GemVal *args, int argc) {
#line 90 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_string_trim", "compiler/main.gem", 90);
#line 91 "compiler/main.gem"
    GemVal _t34[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t34, 1);
#line 92 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_slen, gem_int(0)))) {
#line 93 "compiler/main.gem"
        GemVal _t35 = gem_v_s;
        gem_pop_frame();
        return _t35;
    }
#line 95 "compiler/main.gem"
    GemVal gem_v_start = gem_int(0);
#line 96 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_start, gem_v_slen))) break;
#line 97 "compiler/main.gem"
    GemVal _t36[] = {gem_v_s, gem_v_start};
        GemVal gem_v_c = gem_ord_fn(NULL, _t36, 2);
#line 98 "compiler/main.gem"
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
#line 101 "compiler/main.gem"
        gem_v_start = gem_add(gem_v_start, gem_int(1));
    }
#line 103 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_start, gem_v_slen))) {
#line 104 "compiler/main.gem"
        GemVal _t40 = gem_string("");
        gem_pop_frame();
        return _t40;
    }
#line 106 "compiler/main.gem"
    GemVal gem_v_stop = gem_sub(gem_v_slen, gem_int(1));
#line 107 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_stop, gem_v_start))) break;
#line 108 "compiler/main.gem"
    GemVal _t41[] = {gem_v_s, gem_v_stop};
        GemVal gem_v_c = gem_ord_fn(NULL, _t41, 2);
#line 109 "compiler/main.gem"
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
#line 112 "compiler/main.gem"
        gem_v_stop = gem_sub(gem_v_stop, gem_int(1));
    }
#line 114 "compiler/main.gem"
    GemVal _t45[] = {gem_v_s, gem_v_start, gem_add(gem_sub(gem_v_stop, gem_v_start), gem_int(1))};
    GemVal _t46 = gem_substr_fn(NULL, _t45, 3);
    gem_pop_frame();
    return _t46;
}

static GemVal gem_fn__string_starts_with(void *_env, GemVal *args, int argc) {
#line 119 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_starts_with", "compiler/main.gem", 119);
#line 120 "compiler/main.gem"
    GemVal _t47[] = {gem_v_s, gem_int(0), gem_v_prefix};
    GemVal _t48 = gem_fn__str_eq_at(NULL, _t47, 3);
    gem_pop_frame();
    return _t48;
}

static GemVal gem_fn__string_ends_with(void *_env, GemVal *args, int argc) {
#line 123 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_suffix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_ends_with", "compiler/main.gem", 123);
#line 124 "compiler/main.gem"
    GemVal _t49[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t49, 1);
#line 125 "compiler/main.gem"
    GemVal _t50[] = {gem_v_suffix};
    GemVal gem_v_plen = gem_len_fn(NULL, _t50, 1);
#line 126 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_plen, gem_v_slen))) {
#line 127 "compiler/main.gem"
        GemVal _t51 = gem_bool(0);
        gem_pop_frame();
        return _t51;
    }
#line 129 "compiler/main.gem"
    GemVal _t52[] = {gem_v_s, gem_sub(gem_v_slen, gem_v_plen), gem_v_suffix};
    GemVal _t53 = gem_fn__str_eq_at(NULL, _t52, 3);
    gem_pop_frame();
    return _t53;
}

static GemVal gem_fn__string_upper(void *_env, GemVal *args, int argc) {
#line 134 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_string_upper", "compiler/main.gem", 134);
#line 135 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 136 "compiler/main.gem"
    GemVal _t54[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t54, 1);
#line 136 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 136 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 136 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 137 "compiler/main.gem"
    GemVal _t55[] = {gem_v_s, gem_v_i};
        GemVal gem_v_c = gem_ord_fn(NULL, _t55, 2);
#line 138 "compiler/main.gem"
    GemVal _t56;
    if (!gem_truthy(gem_ge(gem_v_c, gem_int(97)))) {
        _t56 = gem_ge(gem_v_c, gem_int(97));
    } else {
        _t56 = gem_le(gem_v_c, gem_int(122));
    }
        if (gem_truthy(_t56)) {
#line 139 "compiler/main.gem"
    GemVal _t57[] = {gem_sub(gem_v_c, gem_int(32))};
    GemVal _t58[] = {gem_v_buf, gem_chr_fn(NULL, _t57, 1)};
            (void)(gem_buf_push_fn(NULL, _t58, 2));
        } else {
#line 141 "compiler/main.gem"
    GemVal _t59[] = {gem_v_c};
    GemVal _t60[] = {gem_v_buf, gem_chr_fn(NULL, _t59, 1)};
            (void)(gem_buf_push_fn(NULL, _t60, 2));
        }
    }

#line 144 "compiler/main.gem"
    GemVal _t61[] = {gem_v_buf};
    GemVal _t62 = gem_buf_str_fn(NULL, _t61, 1);
    gem_pop_frame();
    return _t62;
}

static GemVal gem_fn__string_lower(void *_env, GemVal *args, int argc) {
#line 147 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_string_lower", "compiler/main.gem", 147);
#line 148 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 149 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 149 "compiler/main.gem"
    GemVal _t63[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t63, 1);
#line 149 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 149 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 149 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 150 "compiler/main.gem"
    GemVal _t64[] = {gem_v_s, gem_v_i};
        GemVal gem_v_c = gem_ord_fn(NULL, _t64, 2);
#line 151 "compiler/main.gem"
    GemVal _t65;
    if (!gem_truthy(gem_ge(gem_v_c, gem_int(65)))) {
        _t65 = gem_ge(gem_v_c, gem_int(65));
    } else {
        _t65 = gem_le(gem_v_c, gem_int(90));
    }
        if (gem_truthy(_t65)) {
#line 152 "compiler/main.gem"
    GemVal _t66[] = {gem_add(gem_v_c, gem_int(32))};
    GemVal _t67[] = {gem_v_buf, gem_chr_fn(NULL, _t66, 1)};
            (void)(gem_buf_push_fn(NULL, _t67, 2));
        } else {
#line 154 "compiler/main.gem"
    GemVal _t68[] = {gem_v_c};
    GemVal _t69[] = {gem_v_buf, gem_chr_fn(NULL, _t68, 1)};
            (void)(gem_buf_push_fn(NULL, _t69, 2));
        }
    }

#line 157 "compiler/main.gem"
    GemVal _t70[] = {gem_v_buf};
    GemVal _t71 = gem_buf_str_fn(NULL, _t70, 1);
    gem_pop_frame();
    return _t71;
}

static GemVal gem_fn__string_contains(void *_env, GemVal *args, int argc) {
#line 162 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_needle = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_contains", "compiler/main.gem", 162);
    GemVal _t72[] = {gem_v_s, gem_v_needle};
    GemVal _t73 = gem_ge(gem_fn__string_index_of(NULL, _t72, 2), gem_int(0));
    gem_pop_frame();
    return _t73;
}

static GemVal gem_fn__string_repeat(void *_env, GemVal *args, int argc) {
#line 168 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_string_repeat", "compiler/main.gem", 168);
#line 169 "compiler/main.gem"
    GemVal gem_v_buf = gem_buf_new_fn(NULL, NULL, 0);
#line 170 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 170 "compiler/main.gem"
    GemVal gem_v__for_limit_3 = gem_v_n;
#line 170 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 170 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_3;
#line 170 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 171 "compiler/main.gem"
    GemVal _t74[] = {gem_v_buf, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t74, 2));
    }

#line 173 "compiler/main.gem"
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

static GemVal gem_fn_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_extern_fn", "compiler/main.gem", 136);
    GemVal _t139 = gem_table_new();
    gem_table_set(_t139, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t139, gem_string("name"), gem_v_name);
    gem_table_set(_t139, gem_string("params"), gem_v_params);
    gem_table_set(_t139, gem_string("ret_type"), gem_v_ret_type);
    GemVal _t140 = _t139;
    gem_pop_frame();
    return _t140;
}

static GemVal gem_fn_make_extern_param(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_extern_param", "compiler/main.gem", 140);
    GemVal _t141 = gem_table_new();
    gem_table_set(_t141, gem_string("name"), gem_v_name);
    gem_table_set(_t141, gem_string("type"), gem_v_type_name);
    GemVal _t142 = _t141;
    gem_pop_frame();
    return _t142;
}

static GemVal gem_fn_make_extern_include(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_extern_include", "compiler/main.gem", 144);
    GemVal _t143 = gem_table_new();
    gem_table_set(_t143, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t143, gem_string("path"), gem_v_path);
    GemVal _t144 = _t143;
    gem_pop_frame();
    return _t144;
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
    GemVal _t145[] = {gem_v_source};
    GemVal gem_v_slen = gem_len_fn(NULL, _t145, 1);
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
                GemVal _t146;
                if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) {
                                _t146 = gem_lt(gem_v_i, gem_v_slen);
                } else {
                                _t146 = gem_neq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n"));
                }
                if (!gem_truthy(_t146)) break;
#line 11 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 13 "compiler/main.gem"
    GemVal _t147[] = {gem_v_source, gem_v_start, gem_sub(gem_v_i, gem_v_start)};
            GemVal _t148 = gem_substr_fn(NULL, _t147, 3);
            gem_pop_frame();
            return _t148;
        }
#line 15 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n")))) {
#line 16 "compiler/main.gem"
            gem_v_current = gem_add(gem_v_current, gem_int(1));
        }
#line 18 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t149 = gem_string("");
    gem_pop_frame();
    return _t149;
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

    GemVal _t150 = gem_v_result;
    gem_pop_frame();
    return _t150;
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
    GemVal _t151;
    if (gem_truthy(gem_eq(gem_v_source, GEM_NIL))) {
        _t151 = gem_eq(gem_v_source, GEM_NIL);
    } else {
        _t151 = gem_eq(gem_v_line, GEM_NIL);
    }
    if (gem_truthy(_t151)) {
#line 36 "compiler/main.gem"
    GemVal _t152[] = {gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"))};
        (void)(gem_eprint_fn(NULL, _t152, 1));
#line 37 "compiler/main.gem"
    GemVal _t153[] = {gem_int(1)};
        (void)(gem_exit_process_fn(NULL, _t153, 1));
    }
#line 39 "compiler/main.gem"
    GemVal _t154;
    if (gem_truthy(gem_eq(gem_v_col, GEM_NIL))) {
        _t154 = gem_eq(gem_v_col, GEM_NIL);
    } else {
        _t154 = gem_lt(gem_v_col, gem_int(1));
    }
    if (gem_truthy(_t154)) {
#line 40 "compiler/main.gem"
        gem_v_col = gem_int(1);
    }
#line 43 "compiler/main.gem"
    GemVal _t155[] = {gem_v_line};
    GemVal gem_v_line_str = gem_to_string_fn(NULL, _t155, 1);
#line 44 "compiler/main.gem"
    GemVal _t156[] = {gem_v_line_str};
    GemVal gem_v_gutter_width = gem_len_fn(NULL, _t156, 1);
#line 45 "compiler/main.gem"
    GemVal _t157[] = {gem_string(" "), gem_v_gutter_width};
    GemVal gem_v_padding = gem_fn_repeat_char(NULL, _t157, 2);
#line 46 "compiler/main.gem"
    GemVal _t158[] = {gem_v_source, gem_v_line};
    GemVal gem_v_src_line = gem_fn_get_source_line(NULL, _t158, 2);
#line 48 "compiler/main.gem"
    GemVal gem_v_actual_span = gem_v_span_len;
#line 49 "compiler/main.gem"
    GemVal _t159;
    if (gem_truthy(gem_eq(gem_v_actual_span, GEM_NIL))) {
        _t159 = gem_eq(gem_v_actual_span, GEM_NIL);
    } else {
        _t159 = gem_lt(gem_v_actual_span, gem_int(1));
    }
    if (gem_truthy(_t159)) {
#line 50 "compiler/main.gem"
        gem_v_actual_span = gem_int(1);
    }
#line 53 "compiler/main.gem"
    GemVal gem_v_out = gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"));
#line 54 "compiler/main.gem"
    GemVal _t160[] = {gem_v_col};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string("  --> ")), gem_v_file), gem_string(":")), gem_v_line_str), gem_string(":")), gem_to_string_fn(NULL, _t160, 1)), gem_string("\n"));
#line 55 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 56 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_line_str), gem_string(" | ")), gem_v_src_line), gem_string("\n"));
#line 57 "compiler/main.gem"
    GemVal _t161[] = {gem_string(" "), gem_sub(gem_v_col, gem_int(1))};
    GemVal _t162[] = {gem_string("^"), gem_v_actual_span};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" | ")), gem_fn_repeat_char(NULL, _t161, 2)), gem_fn_repeat_char(NULL, _t162, 2)), gem_string("\n"));
#line 58 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 60 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_hint, GEM_NIL))) {
#line 61 "compiler/main.gem"
        gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string("Hint: ")), gem_v_hint), gem_string("\n"));
    }
#line 64 "compiler/main.gem"
    GemVal _t163[] = {gem_v_out};
    (void)(gem_eprint_fn(NULL, _t163, 1));
#line 65 "compiler/main.gem"
    GemVal _t164[] = {gem_int(1)};
    GemVal _t165 = gem_exit_process_fn(NULL, _t164, 1);
    gem_pop_frame();
    return _t165;
}

static GemVal gem_fn_is_digit(void *_env, GemVal *args, int argc) {
#line 15 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_digit", "compiler/main.gem", 15);
    GemVal _t166;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t166 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t166 = gem_le(gem_v_ch, gem_string("9"));
    }
    GemVal _t167 = _t166;
    gem_pop_frame();
    return _t167;
}

static GemVal gem_fn_is_alpha(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alpha", "compiler/main.gem", 19);
    GemVal _t168;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t168 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t168 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t170;
    if (gem_truthy(_t168)) {
        _t170 = _t168;
    } else {
        GemVal _t169;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t169 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t169 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t170 = _t169;
    }
    GemVal _t171;
    if (gem_truthy(_t170)) {
        _t171 = _t170;
    } else {
        _t171 = gem_eq(gem_v_ch, gem_string("_"));
    }
    GemVal _t172 = _t171;
    gem_pop_frame();
    return _t172;
}

static GemVal gem_fn_is_alnum(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("is_alnum", "compiler/main.gem", 23);
    GemVal _t173[] = {gem_v_ch};
    GemVal _t175;
    if (gem_truthy(gem_fn_is_alpha(NULL, _t173, 1))) {
        _t175 = gem_fn_is_alpha(NULL, _t173, 1);
    } else {
        GemVal _t174[] = {gem_v_ch};
        _t175 = gem_fn_is_digit(NULL, _t174, 1);
    }
    GemVal _t176 = _t175;
    gem_pop_frame();
    return _t176;
}

static GemVal gem_fn_digit_val(void *_env, GemVal *args, int argc) {
#line 29 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("digit_val", "compiler/main.gem", 29);
#line 30 "compiler/main.gem"
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t177 = gem_int(0);
        gem_pop_frame();
        return _t177;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t178 = gem_int(1);
        gem_pop_frame();
        return _t178;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t179 = gem_int(2);
        gem_pop_frame();
        return _t179;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t180 = gem_int(3);
        gem_pop_frame();
        return _t180;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t181 = gem_int(4);
        gem_pop_frame();
        return _t181;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t182 = gem_int(5);
        gem_pop_frame();
        return _t182;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t183 = gem_int(6);
        gem_pop_frame();
        return _t183;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t184 = gem_int(7);
        gem_pop_frame();
        return _t184;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t185 = gem_int(8);
        gem_pop_frame();
        return _t185;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t186 = gem_int(9);
        gem_pop_frame();
        return _t186;
    } else {
#line 52 "compiler/main.gem"
    GemVal _t187[] = {gem_v_ch};
    GemVal _t188[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t187, 1))};
        GemVal _t189 = gem_error_at_fn("compiler/main.gem", 52, _t188, 1);
        gem_pop_frame();
        return _t189;
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
    GemVal _t190[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t190, 1);
#line 58 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 58 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 58 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 59 "compiler/main.gem"
    GemVal _t191[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn_digit_val(NULL, _t191, 1));
    }

    GemVal _t192 = gem_v_result;
    gem_pop_frame();
    return _t192;
}

static GemVal gem_fn_contains_dot(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("contains_dot", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 67 "compiler/main.gem"
    GemVal _t193[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t193, 1);
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
            GemVal _t194 = gem_bool(1);
            gem_pop_frame();
            return _t194;
        }
    }

    GemVal _t195 = gem_bool(0);
    gem_pop_frame();
    return _t195;
}

static GemVal gem_fn_apply_escape(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("apply_escape", "compiler/main.gem", 80);
#line 81 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 82 "compiler/main.gem"
        GemVal _t196 = gem_string("\n");
        gem_pop_frame();
        return _t196;
    } else {
#line 83 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 84 "compiler/main.gem"
    GemVal _t197[] = {gem_int(13)};
            GemVal _t198 = gem_chr_fn(NULL, _t197, 1);
            gem_pop_frame();
            return _t198;
        } else {
#line 85 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 86 "compiler/main.gem"
                GemVal _t199 = gem_string("\t");
                gem_pop_frame();
                return _t199;
            } else {
#line 87 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 88 "compiler/main.gem"
                    GemVal _t200 = gem_string("\\");
                    gem_pop_frame();
                    return _t200;
                } else {
#line 89 "compiler/main.gem"
    GemVal _t201;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t201 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t201 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t201)) {
#line 90 "compiler/main.gem"
    GemVal _t202[] = {gem_int(0)};
                        GemVal _t203 = gem_chr_fn(NULL, _t202, 1);
                        gem_pop_frame();
                        return _t203;
                    } else {
#line 91 "compiler/main.gem"
    GemVal _t204;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t204 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t204 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t204)) {
#line 92 "compiler/main.gem"
                            GemVal _t205 = gem_string("\"");
                            gem_pop_frame();
                            return _t205;
                        } else {
#line 93 "compiler/main.gem"
    GemVal _t206;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t206 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t206 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t206)) {
#line 94 "compiler/main.gem"
                                GemVal _t207 = gem_string("{");
                                gem_pop_frame();
                                return _t207;
                            } else {
#line 95 "compiler/main.gem"
    GemVal _t208;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t208 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t208 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t208)) {
#line 96 "compiler/main.gem"
                                    GemVal _t209 = gem_string("}");
                                    gem_pop_frame();
                                    return _t209;
                                } else {
#line 97 "compiler/main.gem"
    GemVal _t210;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t210 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t210 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t210)) {
#line 98 "compiler/main.gem"
                                        GemVal _t211 = gem_string("'");
                                        gem_pop_frame();
                                        return _t211;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t212 = GEM_NIL;
    gem_pop_frame();
    return _t212;
}

static GemVal gem_fn_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    gem_push_frame("build_keyword_set", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t213 = gem_table_new();
    GemVal gem_v_t = _t213;
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
    GemVal _t214 = gem_v_t;
    gem_pop_frame();
    return _t214;
}

static GemVal gem_fn_tokenize(void *_env, GemVal *args, int argc) {
#line 134 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("tokenize", "compiler/main.gem", 134);
#line 135 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn_build_keyword_set(NULL, NULL, 0);
#line 136 "compiler/main.gem"
    GemVal _t215 = gem_table_new();
    GemVal gem_v_tokens = _t215;
#line 137 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 138 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 139 "compiler/main.gem"
    GemVal gem_v_line_start = gem_int(0);
#line 140 "compiler/main.gem"
    GemVal _t216[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t216, 1);
#line 141 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 142 "compiler/main.gem"
    GemVal _t217 = gem_table_new();
    GemVal gem_v_interp_brace = _t217;
#line 143 "compiler/main.gem"
    GemVal _t218[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t218, 1);
#line 144 "compiler/main.gem"
    GemVal _t219[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t219, 1);
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
    GemVal _t220;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t220 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t220 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t220)) {
#line 155 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 160 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 161 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t221;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t221 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t221 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t221)) break;
#line 162 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 168 "compiler/main.gem"
    GemVal _t222;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t222 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t222 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t222)) {
#line 169 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 170 "compiler/main.gem"
    GemVal _t223[] = {gem_v_tokens};
    GemVal _t225;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t223, 1), gem_int(0)))) {
        _t225 = gem_gt(gem_len_fn(NULL, _t223, 1), gem_int(0));
    } else {
        GemVal _t224 = gem_table_get(gem_v_tokens, gem_neg(gem_int(1)));
        _t225 = gem_eq(gem_table_get(_t224, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t225)) {
#line 171 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 173 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 174 "compiler/main.gem"
    GemVal _t226 = gem_table_new();
    gem_table_set(_t226, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t226, gem_string("value"), gem_string("\n"));
    gem_table_set(_t226, gem_string("line"), gem_v_line);
    gem_table_set(_t226, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t227[] = {gem_v_tokens, _t226};
                (void)(gem_push_fn(NULL, _t227, 2));
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
    GemVal _t228;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t228 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t228 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t229;
    if (gem_truthy(_t228)) {
        _t229 = _t228;
    } else {
        _t229 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t229)) {
#line 189 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 190 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 191 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t230;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t230 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t230 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t230)) break;
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
    GemVal _t231[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_digit(NULL, _t231, 1))) {
#line 203 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 204 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t233;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t233 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t232[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t233 = gem_fn_is_digit(NULL, _t232, 1);
                }
                if (!gem_truthy(_t233)) break;
#line 205 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 206 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 208 "compiler/main.gem"
    GemVal _t234;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t234 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t234 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t234;
#line 209 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 210 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 211 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 212 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t236;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t236 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t235[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t236 = gem_fn_is_digit(NULL, _t235, 1);
                    }
                    if (!gem_truthy(_t236)) break;
#line 213 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 214 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 217 "compiler/main.gem"
    GemVal _t237 = gem_table_new();
    gem_table_set(_t237, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t237, gem_string("value"), gem_v_val);
    gem_table_set(_t237, gem_string("line"), gem_v_line);
    gem_table_set(_t237, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t238[] = {gem_v_tokens, _t237};
            (void)(gem_push_fn(NULL, _t238, 2));
            continue;
        }
#line 222 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 223 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 226 "compiler/main.gem"
    GemVal _t239;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t239 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t239 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t240;
    if (!gem_truthy(_t239)) {
        _t240 = _t239;
    } else {
        _t240 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t240)) {
#line 227 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 229 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t242;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t242 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t241;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t241 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t241 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t242 = _t241;
                    }
                    if (!gem_truthy(_t242)) break;
#line 230 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 232 "compiler/main.gem"
    GemVal _t243;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t243 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t243 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t243)) {
#line 233 "compiler/main.gem"
    GemVal _t244[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t244, 7));
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
                    GemVal _t245;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t245 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t245 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t245)) break;
#line 244 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 245 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t246;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t246 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t246 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t246)) break;
#line 246 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 247 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 249 "compiler/main.gem"
    GemVal _t247;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t247 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t247 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t248;
    if (!gem_truthy(_t247)) {
        _t248 = _t247;
    } else {
        _t248 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t249;
    if (!gem_truthy(_t248)) {
        _t249 = _t248;
    } else {
        _t249 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t249)) {
#line 250 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 251 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 253 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t250;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t250 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t250 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t250)) break;
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
    GemVal _t251[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t251, 7));
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
                            GemVal _t252;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t252 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t252 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t253;
                            if (!gem_truthy(_t252)) {
                                                        _t253 = _t252;
                            } else {
                                                        _t253 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t253)) break;
#line 276 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 277 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 280 "compiler/main.gem"
    GemVal _t254;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t254 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t254 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t255;
    if (!gem_truthy(_t254)) {
        _t255 = _t254;
    } else {
        _t255 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t256;
    if (!gem_truthy(_t255)) {
        _t256 = _t255;
    } else {
        _t256 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t256)) {
#line 281 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 283 "compiler/main.gem"
    GemVal _t257[] = {gem_v_val};
    GemVal _t258;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t257, 1), gem_int(0)))) {
        _t258 = gem_gt(gem_len_fn(NULL, _t257, 1), gem_int(0));
    } else {
        _t258 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t258)) {
#line 284 "compiler/main.gem"
    GemVal _t259[] = {gem_v_val};
    GemVal _t260[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t259, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t260, 3);
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
    GemVal _t261[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t261, 7));
                        }
#line 304 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 305 "compiler/main.gem"
    GemVal _t262[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t262, 2);
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
    GemVal _t263 = gem_table_new();
    gem_table_set(_t263, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t263, gem_string("value"), gem_string(""));
    gem_table_set(_t263, gem_string("line"), gem_v_line);
    gem_table_set(_t263, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t264[] = {gem_v_tokens, _t263};
                        (void)(gem_push_fn(NULL, _t264, 2));
#line 316 "compiler/main.gem"
    GemVal _t265 = gem_table_new();
    gem_table_set(_t265, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t265, gem_string("value"), gem_v_val);
    gem_table_set(_t265, gem_string("line"), gem_v_line);
    gem_table_set(_t265, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t266[] = {gem_v_tokens, _t265};
                        (void)(gem_push_fn(NULL, _t266, 2));
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
    GemVal _t267 = gem_table_new();
    gem_table_set(_t267, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t267, gem_string("value"), gem_v_val);
    gem_table_set(_t267, gem_string("line"), gem_v_line);
    gem_table_set(_t267, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t268[] = {gem_v_tokens, _t267};
                    (void)(gem_push_fn(NULL, _t268, 2));
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
                GemVal _t269;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t269 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t269 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t269)) break;
#line 339 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 340 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 341 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 342 "compiler/main.gem"
    GemVal _t270[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t270, 7));
                    }
#line 344 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 345 "compiler/main.gem"
    GemVal _t271[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t271, 2);
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
    GemVal _t272 = gem_table_new();
    gem_table_set(_t272, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t272, gem_string("value"), gem_string(""));
    gem_table_set(_t272, gem_string("line"), gem_v_line);
    gem_table_set(_t272, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t273[] = {gem_v_tokens, _t272};
                    (void)(gem_push_fn(NULL, _t273, 2));
#line 356 "compiler/main.gem"
    GemVal _t274 = gem_table_new();
    gem_table_set(_t274, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t274, gem_string("value"), gem_v_val);
    gem_table_set(_t274, gem_string("line"), gem_v_line);
    gem_table_set(_t274, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t275[] = {gem_v_tokens, _t274};
                    (void)(gem_push_fn(NULL, _t275, 2));
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
    GemVal _t276[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
                    (void)(gem_fn_compile_error(NULL, _t276, 7));
                }
#line 371 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t277 = gem_table_new();
    gem_table_set(_t277, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t277, gem_string("value"), gem_v_val);
    gem_table_set(_t277, gem_string("line"), gem_v_line);
    gem_table_set(_t277, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t278[] = {gem_v_tokens, _t277};
                (void)(gem_push_fn(NULL, _t278, 2));
            }
            continue;
        }
#line 378 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 379 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 382 "compiler/main.gem"
    GemVal _t279;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t279 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t279 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t280;
    if (!gem_truthy(_t279)) {
        _t280 = _t279;
    } else {
        _t280 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t280)) {
#line 383 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 385 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t282;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t282 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t281;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t281 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t281 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t282 = _t281;
                    }
                    if (!gem_truthy(_t282)) break;
#line 386 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 388 "compiler/main.gem"
    GemVal _t283;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t283 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t283 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t283)) {
#line 389 "compiler/main.gem"
    GemVal _t284[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t284, 7));
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
                    GemVal _t285;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t285 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t285 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t285)) break;
#line 400 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 401 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t286;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t286 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t286 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t286)) break;
#line 402 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 403 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 405 "compiler/main.gem"
    GemVal _t287;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t287 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t287 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t288;
    if (!gem_truthy(_t287)) {
        _t288 = _t287;
    } else {
        _t288 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t289;
    if (!gem_truthy(_t288)) {
        _t289 = _t288;
    } else {
        _t289 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t289)) {
#line 406 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 407 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 409 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t290;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t290 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t290 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t290)) break;
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
    GemVal _t291[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t291, 7));
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
                            GemVal _t292;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t292 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t292 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t293;
                            if (!gem_truthy(_t292)) {
                                                        _t293 = _t292;
                            } else {
                                                        _t293 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t293)) break;
#line 430 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 431 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 434 "compiler/main.gem"
    GemVal _t294;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t294 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t294 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t295;
    if (!gem_truthy(_t294)) {
        _t295 = _t294;
    } else {
        _t295 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t296;
    if (!gem_truthy(_t295)) {
        _t296 = _t295;
    } else {
        _t296 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t296)) {
#line 435 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 437 "compiler/main.gem"
    GemVal _t297[] = {gem_v_val};
    GemVal _t298;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t297, 1), gem_int(0)))) {
        _t298 = gem_gt(gem_len_fn(NULL, _t297, 1), gem_int(0));
    } else {
        _t298 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t298)) {
#line 438 "compiler/main.gem"
    GemVal _t299[] = {gem_v_val};
    GemVal _t300[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t299, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t300, 3);
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
    GemVal _t301[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t301, 7));
                        }
#line 458 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 459 "compiler/main.gem"
    GemVal _t302[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t302, 2);
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
    GemVal _t303 = gem_table_new();
    gem_table_set(_t303, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t303, gem_string("value"), gem_v_val);
    gem_table_set(_t303, gem_string("line"), gem_v_line);
    gem_table_set(_t303, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t304[] = {gem_v_tokens, _t303};
                (void)(gem_push_fn(NULL, _t304, 2));
                continue;
            }
#line 477 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 478 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t305;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t305 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t305 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t305)) break;
#line 479 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 480 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 481 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 482 "compiler/main.gem"
    GemVal _t306[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t306, 7));
                    }
#line 484 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 485 "compiler/main.gem"
    GemVal _t307[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t307, 2);
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
    GemVal _t308[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
                (void)(gem_fn_compile_error(NULL, _t308, 7));
            }
#line 500 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 501 "compiler/main.gem"
    GemVal _t309 = gem_table_new();
    gem_table_set(_t309, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t309, gem_string("value"), gem_v_val);
    gem_table_set(_t309, gem_string("line"), gem_v_line);
    gem_table_set(_t309, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t310[] = {gem_v_tokens, _t309};
            (void)(gem_push_fn(NULL, _t310, 2));
            continue;
        }
#line 506 "compiler/main.gem"
    GemVal _t311[] = {gem_v_ch};
        if (gem_truthy(gem_fn_is_alpha(NULL, _t311, 1))) {
#line 507 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 508 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t313;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t313 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t312[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t313 = gem_fn_is_alnum(NULL, _t312, 1);
                }
                if (!gem_truthy(_t313)) break;
#line 509 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 510 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 512 "compiler/main.gem"
    GemVal _t314[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_has_key_fn(NULL, _t314, 2))) {
#line 513 "compiler/main.gem"
    GemVal _t315 = gem_table_new();
    gem_table_set(_t315, gem_string("type"), gem_v_val);
    gem_table_set(_t315, gem_string("value"), gem_v_val);
    gem_table_set(_t315, gem_string("line"), gem_v_line);
    gem_table_set(_t315, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t316[] = {gem_v_tokens, _t315};
                (void)(gem_push_fn(NULL, _t316, 2));
            } else {
#line 515 "compiler/main.gem"
    GemVal _t317 = gem_table_new();
    gem_table_set(_t317, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t317, gem_string("value"), gem_v_val);
    gem_table_set(_t317, gem_string("line"), gem_v_line);
    gem_table_set(_t317, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t318[] = {gem_v_tokens, _t317};
                (void)(gem_push_fn(NULL, _t318, 2));
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
    GemVal _t319;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t319 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t319 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t319)) {
#line 527 "compiler/main.gem"
    GemVal _t320 = gem_table_new();
    gem_table_set(_t320, gem_string("type"), gem_string("=="));
    gem_table_set(_t320, gem_string("value"), gem_string("=="));
    gem_table_set(_t320, gem_string("line"), gem_v_line);
    gem_table_set(_t320, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t321[] = {gem_v_tokens, _t320};
            (void)(gem_push_fn(NULL, _t321, 2));
#line 528 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 531 "compiler/main.gem"
    GemVal _t322;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t322 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t322 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t322)) {
#line 532 "compiler/main.gem"
    GemVal _t323 = gem_table_new();
    gem_table_set(_t323, gem_string("type"), gem_string("!="));
    gem_table_set(_t323, gem_string("value"), gem_string("!="));
    gem_table_set(_t323, gem_string("line"), gem_v_line);
    gem_table_set(_t323, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t324[] = {gem_v_tokens, _t323};
            (void)(gem_push_fn(NULL, _t324, 2));
#line 533 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 536 "compiler/main.gem"
    GemVal _t325;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t325 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t325 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t325)) {
#line 537 "compiler/main.gem"
    GemVal _t326 = gem_table_new();
    gem_table_set(_t326, gem_string("type"), gem_string("<="));
    gem_table_set(_t326, gem_string("value"), gem_string("<="));
    gem_table_set(_t326, gem_string("line"), gem_v_line);
    gem_table_set(_t326, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t327[] = {gem_v_tokens, _t326};
            (void)(gem_push_fn(NULL, _t327, 2));
#line 538 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 541 "compiler/main.gem"
    GemVal _t328;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t328 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t328 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t328)) {
#line 542 "compiler/main.gem"
    GemVal _t329 = gem_table_new();
    gem_table_set(_t329, gem_string("type"), gem_string(">="));
    gem_table_set(_t329, gem_string("value"), gem_string(">="));
    gem_table_set(_t329, gem_string("line"), gem_v_line);
    gem_table_set(_t329, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t330[] = {gem_v_tokens, _t329};
            (void)(gem_push_fn(NULL, _t330, 2));
#line 543 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 546 "compiler/main.gem"
    GemVal _t331;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t331 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t331 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t331)) {
#line 547 "compiler/main.gem"
    GemVal _t332 = gem_table_new();
    gem_table_set(_t332, gem_string("type"), gem_string("+="));
    gem_table_set(_t332, gem_string("value"), gem_string("+="));
    gem_table_set(_t332, gem_string("line"), gem_v_line);
    gem_table_set(_t332, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t333[] = {gem_v_tokens, _t332};
            (void)(gem_push_fn(NULL, _t333, 2));
#line 548 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 551 "compiler/main.gem"
    GemVal _t334;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t334 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t334 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t334)) {
#line 552 "compiler/main.gem"
    GemVal _t335 = gem_table_new();
    gem_table_set(_t335, gem_string("type"), gem_string("-="));
    gem_table_set(_t335, gem_string("value"), gem_string("-="));
    gem_table_set(_t335, gem_string("line"), gem_v_line);
    gem_table_set(_t335, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t336[] = {gem_v_tokens, _t335};
            (void)(gem_push_fn(NULL, _t336, 2));
#line 553 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 556 "compiler/main.gem"
    GemVal _t337;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t337 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t337 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t337)) {
#line 557 "compiler/main.gem"
    GemVal _t338 = gem_table_new();
    gem_table_set(_t338, gem_string("type"), gem_string("*="));
    gem_table_set(_t338, gem_string("value"), gem_string("*="));
    gem_table_set(_t338, gem_string("line"), gem_v_line);
    gem_table_set(_t338, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t339[] = {gem_v_tokens, _t338};
            (void)(gem_push_fn(NULL, _t339, 2));
#line 558 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 561 "compiler/main.gem"
    GemVal _t340;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t340 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t340 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t340)) {
#line 562 "compiler/main.gem"
    GemVal _t341 = gem_table_new();
    gem_table_set(_t341, gem_string("type"), gem_string("/="));
    gem_table_set(_t341, gem_string("value"), gem_string("/="));
    gem_table_set(_t341, gem_string("line"), gem_v_line);
    gem_table_set(_t341, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t342[] = {gem_v_tokens, _t341};
            (void)(gem_push_fn(NULL, _t342, 2));
#line 563 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 566 "compiler/main.gem"
    GemVal _t343;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t343 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t343 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t343)) {
#line 567 "compiler/main.gem"
    GemVal _t344 = gem_table_new();
    gem_table_set(_t344, gem_string("type"), gem_string("->"));
    gem_table_set(_t344, gem_string("value"), gem_string("->"));
    gem_table_set(_t344, gem_string("line"), gem_v_line);
    gem_table_set(_t344, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t345[] = {gem_v_tokens, _t344};
            (void)(gem_push_fn(NULL, _t345, 2));
#line 568 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 573 "compiler/main.gem"
    GemVal _t346;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t346 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t346 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t347;
    if (gem_truthy(_t346)) {
        _t347 = _t346;
    } else {
        _t347 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t348;
    if (gem_truthy(_t347)) {
        _t348 = _t347;
    } else {
        _t348 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t349;
    if (gem_truthy(_t348)) {
        _t349 = _t348;
    } else {
        _t349 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t349)) {
#line 574 "compiler/main.gem"
    GemVal _t350 = gem_table_new();
    gem_table_set(_t350, gem_string("type"), gem_v_ch);
    gem_table_set(_t350, gem_string("value"), gem_v_ch);
    gem_table_set(_t350, gem_string("line"), gem_v_line);
    gem_table_set(_t350, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t351[] = {gem_v_tokens, _t350};
            (void)(gem_push_fn(NULL, _t351, 2));
#line 575 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 578 "compiler/main.gem"
    GemVal _t352;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t352 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t352 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t353;
    if (gem_truthy(_t352)) {
        _t353 = _t352;
    } else {
        _t353 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t353)) {
#line 579 "compiler/main.gem"
    GemVal _t354 = gem_table_new();
    gem_table_set(_t354, gem_string("type"), gem_v_ch);
    gem_table_set(_t354, gem_string("value"), gem_v_ch);
    gem_table_set(_t354, gem_string("line"), gem_v_line);
    gem_table_set(_t354, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t355[] = {gem_v_tokens, _t354};
            (void)(gem_push_fn(NULL, _t355, 2));
#line 580 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 584 "compiler/main.gem"
    GemVal _t356;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t356 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t356 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t356)) {
#line 585 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 586 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 587 "compiler/main.gem"
    GemVal _t357 = gem_table_new();
    gem_table_set(_t357, gem_string("type"), gem_string("{"));
    gem_table_set(_t357, gem_string("value"), gem_string("{"));
    gem_table_set(_t357, gem_string("line"), gem_v_line);
    gem_table_set(_t357, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t358[] = {gem_v_tokens, _t357};
            (void)(gem_push_fn(NULL, _t358, 2));
#line 588 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 591 "compiler/main.gem"
    GemVal _t359;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t359 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t359 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t359)) {
#line 592 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 593 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 594 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 595 "compiler/main.gem"
    GemVal _t360 = gem_table_new();
    gem_table_set(_t360, gem_string("type"), gem_string("}"));
    gem_table_set(_t360, gem_string("value"), gem_string("}"));
    gem_table_set(_t360, gem_string("line"), gem_v_line);
    gem_table_set(_t360, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t361[] = {gem_v_tokens, _t360};
                (void)(gem_push_fn(NULL, _t361, 2));
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
                            GemVal _t362;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t362 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t362 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t363;
                            if (!gem_truthy(_t362)) {
                                                        _t363 = _t362;
                            } else {
                                                        _t363 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t363)) break;
#line 615 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 616 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 619 "compiler/main.gem"
    GemVal _t364;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t364 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t364 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t365;
    if (!gem_truthy(_t364)) {
        _t365 = _t364;
    } else {
        _t365 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t366;
    if (!gem_truthy(_t365)) {
        _t366 = _t365;
    } else {
        _t366 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t366)) {
#line 620 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 621 "compiler/main.gem"
    GemVal _t367[] = {gem_v_val};
    GemVal _t368;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t367, 1), gem_int(0)))) {
        _t368 = gem_gt(gem_len_fn(NULL, _t367, 1), gem_int(0));
    } else {
        _t368 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t368)) {
#line 622 "compiler/main.gem"
    GemVal _t369[] = {gem_v_val};
    GemVal _t370[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t369, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t370, 3);
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
    GemVal _t371[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                            (void)(gem_fn_compile_error(NULL, _t371, 7));
                        }
#line 642 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 643 "compiler/main.gem"
    GemVal _t372[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t372, 2);
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
    GemVal _t373 = gem_table_new();
    gem_table_set(_t373, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t373, gem_string("value"), gem_v_val);
    gem_table_set(_t373, gem_string("line"), gem_v_line);
    gem_table_set(_t373, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t374[] = {gem_v_tokens, _t373};
                        (void)(gem_push_fn(NULL, _t374, 2));
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
    GemVal _t375[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string interpolation"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t375, 7));
                    }
#line 670 "compiler/main.gem"
    GemVal _t376 = gem_table_new();
    gem_table_set(_t376, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t376, gem_string("value"), gem_v_val);
    gem_table_set(_t376, gem_string("line"), gem_v_line);
    gem_table_set(_t376, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t377[] = {gem_v_tokens, _t376};
                    (void)(gem_push_fn(NULL, _t377, 2));
#line 671 "compiler/main.gem"
    GemVal _t378 = gem_table_new();
    gem_table_set(_t378, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t378, gem_string("value"), gem_string(""));
    gem_table_set(_t378, gem_string("line"), gem_v_line);
    gem_table_set(_t378, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t379[] = {gem_v_tokens, _t378};
                    (void)(gem_push_fn(NULL, _t379, 2));
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
                GemVal _t380;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t380 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t380 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t380)) break;
#line 680 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 681 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 682 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 683 "compiler/main.gem"
    GemVal _t381[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
                        (void)(gem_fn_compile_error(NULL, _t381, 7));
                    }
#line 685 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 686 "compiler/main.gem"
    GemVal _t382[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn_apply_escape(NULL, _t382, 2);
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
    GemVal _t383 = gem_table_new();
    gem_table_set(_t383, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t383, gem_string("value"), gem_v_val);
    gem_table_set(_t383, gem_string("line"), gem_v_line);
    gem_table_set(_t383, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t384[] = {gem_v_tokens, _t383};
                    (void)(gem_push_fn(NULL, _t384, 2));
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
    GemVal _t385[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string interpolation"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t385, 7));
                }
#line 712 "compiler/main.gem"
    GemVal _t386 = gem_table_new();
    gem_table_set(_t386, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t386, gem_string("value"), gem_v_val);
    gem_table_set(_t386, gem_string("line"), gem_v_line);
    gem_table_set(_t386, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t387[] = {gem_v_tokens, _t386};
                (void)(gem_push_fn(NULL, _t387, 2));
#line 713 "compiler/main.gem"
    GemVal _t388 = gem_table_new();
    gem_table_set(_t388, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t388, gem_string("value"), gem_string(""));
    gem_table_set(_t388, gem_string("line"), gem_v_line);
    gem_table_set(_t388, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t389[] = {gem_v_tokens, _t388};
                (void)(gem_push_fn(NULL, _t389, 2));
#line 714 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 719 "compiler/main.gem"
    GemVal _t390;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t390 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t390 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t391;
    if (gem_truthy(_t390)) {
        _t391 = _t390;
    } else {
        _t391 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t392;
    if (gem_truthy(_t391)) {
        _t392 = _t391;
    } else {
        _t392 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t393;
    if (gem_truthy(_t392)) {
        _t393 = _t392;
    } else {
        _t393 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t394;
    if (gem_truthy(_t393)) {
        _t394 = _t393;
    } else {
        _t394 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t394)) {
#line 720 "compiler/main.gem"
    GemVal _t395 = gem_table_new();
    gem_table_set(_t395, gem_string("type"), gem_v_ch);
    gem_table_set(_t395, gem_string("value"), gem_v_ch);
    gem_table_set(_t395, gem_string("line"), gem_v_line);
    gem_table_set(_t395, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t396[] = {gem_v_tokens, _t395};
            (void)(gem_push_fn(NULL, _t396, 2));
#line 721 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 724 "compiler/main.gem"
    GemVal _t397;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t397 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t397 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t398;
    if (!gem_truthy(_t397)) {
        _t398 = _t397;
    } else {
        _t398 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t399;
    if (!gem_truthy(_t398)) {
        _t399 = _t398;
    } else {
        _t399 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t400;
    if (!gem_truthy(_t399)) {
        _t400 = _t399;
    } else {
        _t400 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t400)) {
#line 725 "compiler/main.gem"
    GemVal _t401 = gem_table_new();
    gem_table_set(_t401, gem_string("type"), gem_string("..."));
    gem_table_set(_t401, gem_string("value"), gem_string("..."));
    gem_table_set(_t401, gem_string("line"), gem_v_line);
    gem_table_set(_t401, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t402[] = {gem_v_tokens, _t401};
            (void)(gem_push_fn(NULL, _t402, 2));
#line 726 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 729 "compiler/main.gem"
    GemVal _t403;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t403 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t403 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t404;
    if (gem_truthy(_t403)) {
        _t404 = _t403;
    } else {
        _t404 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t405;
    if (gem_truthy(_t404)) {
        _t405 = _t404;
    } else {
        _t405 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t405)) {
#line 730 "compiler/main.gem"
    GemVal _t406 = gem_table_new();
    gem_table_set(_t406, gem_string("type"), gem_v_ch);
    gem_table_set(_t406, gem_string("value"), gem_v_ch);
    gem_table_set(_t406, gem_string("line"), gem_v_line);
    gem_table_set(_t406, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t407[] = {gem_v_tokens, _t406};
            (void)(gem_push_fn(NULL, _t407, 2));
#line 731 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 735 "compiler/main.gem"
    GemVal _t408[] = {gem_v_ch};
    GemVal _t409[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t408, 1)), gem_string("'")), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t409, 7));
    }
#line 738 "compiler/main.gem"
    GemVal _t410 = gem_table_new();
    gem_table_set(_t410, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t410, gem_string("value"), gem_string(""));
    gem_table_set(_t410, gem_string("line"), gem_v_line);
    gem_table_set(_t410, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t411[] = {gem_v_tokens, _t410};
    (void)(gem_push_fn(NULL, _t411, 2));
    GemVal _t412 = gem_v_tokens;
    gem_pop_frame();
    return _t412;
}

static GemVal gem_fn_parse_source(void *_env, GemVal *args, int argc) {
#line 11 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("parse_source", "compiler/main.gem", 11);
#line 12 "compiler/main.gem"
    GemVal _t413[] = {gem_v_source, gem_v_file};
    GemVal gem_v_tokens = gem_fn_tokenize(NULL, _t413, 2);
#line 13 "compiler/main.gem"
    GemVal _t414[] = {gem_v_tokens, gem_v_source, gem_v_file};
    GemVal gem_v_p = gem_fn_make_parser(NULL, _t414, 3);
#line 14 "compiler/main.gem"
    GemVal _t415 = gem_v_p;
    GemVal _t416 = gem_table_get(_t415, gem_string("parse"));
    GemVal _t417 = _t416.fn(_t416.env, NULL, 0);
    gem_pop_frame();
    return _t417;
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
    GemVal _t418 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t418;
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
    GemVal _t420 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t420;
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
    GemVal _t422 = gem_v_t;
    gem_pop_frame();
    return _t422;
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
    GemVal _t424 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t425 = gem_eq(gem_table_get(_t424, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t425;
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
    GemVal _t427 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t427, gem_string("type")), gem_v_tp))) {
#line 47 "compiler/main.gem"
    GemVal _t428 = gem_v_t;
    GemVal _t429 = gem_v_t;
    GemVal _t430 = gem_v_t;
    GemVal _t431[] = {gem_table_get(_t430, gem_string("value"))};
    GemVal _t432[] = {gem_v_tp};
    GemVal _t433 = gem_v_t;
    GemVal _t434[] = {gem_table_get(_t433, gem_string("type"))};
    GemVal _t435[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t428, gem_string("line")), gem_table_get(_t429, gem_string("col")), gem_len_fn(NULL, _t431, 1), gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t432, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t434, 1)), gem_string("'")), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t435, 7));
    }
#line 49 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t436 = gem_v_t;
    gem_pop_frame();
    return _t436;
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
        GemVal _t438 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t438, gem_string("type")), gem_string("NEWLINE")))) break;
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
    GemVal _t440 = gem_table_new();
    GemVal gem_v_params = _t440;
#line 67 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 68 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 70 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t441 = (*gem_v_peek);
        GemVal _t442 = _t441.fn(_t441.env, NULL, 0);
        GemVal _t445;
        if (gem_truthy(gem_eq(gem_table_get(_t442, gem_string("type")), gem_string("NAME")))) {
                _t445 = gem_eq(gem_table_get(_t442, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t443 = (*gem_v_peek);
                GemVal _t444 = _t443.fn(_t443.env, NULL, 0);
                _t445 = gem_eq(gem_table_get(_t444, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t445)) break;
#line 71 "compiler/main.gem"
    GemVal _t446 = (*gem_v_peek);
    GemVal _t447 = _t446.fn(_t446.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t447, gem_string("type")), gem_string("...")))) {
#line 72 "compiler/main.gem"
    GemVal _t448 = (*gem_v_advance);
            (void)(_t448.fn(_t448.env, NULL, 0));
#line 73 "compiler/main.gem"
    GemVal _t449[] = {gem_string("NAME")};
    GemVal _t450 = (*gem_v_expect);
    GemVal _t451 = _t450.fn(_t450.env, _t449, 1);
            gem_v_rest_param = gem_table_get(_t451, gem_string("value"));
#line 74 "compiler/main.gem"
    GemVal _t452 = (*gem_v_peek);
    GemVal _t453 = _t452.fn(_t452.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t453, gem_string("type")), gem_string(",")))) {
#line 75 "compiler/main.gem"
    GemVal _t454 = (*gem_v_advance);
                (void)(_t454.fn(_t454.env, NULL, 0));
#line 76 "compiler/main.gem"
    GemVal _t455 = (*gem_v_peek);
    GemVal _t456 = _t455.fn(_t455.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t456, gem_string("type")), gem_string("NAME")))) {
#line 77 "compiler/main.gem"
    GemVal _t457 = (*gem_v_peek);
    GemVal _t458 = _t457.fn(_t457.env, NULL, 0);
    GemVal _t459 = (*gem_v_peek);
    GemVal _t460 = _t459.fn(_t459.env, NULL, 0);
    GemVal _t461 = (*gem_v_peek);
    GemVal _t462 = _t461.fn(_t461.env, NULL, 0);
    GemVal _t463[] = {gem_table_get(_t462, gem_string("value"))};
    GemVal _t464[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t458, gem_string("line")), gem_table_get(_t460, gem_string("col")), gem_len_fn(NULL, _t463, 1), gem_string("expected parameter name after rest param"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t464, 7));
                }
#line 79 "compiler/main.gem"
    GemVal _t465 = (*gem_v_advance);
    GemVal _t466 = _t465.fn(_t465.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t466, gem_string("value"));
#line 80 "compiler/main.gem"
    GemVal _t467 = (*gem_v_peek);
    GemVal _t468 = _t467.fn(_t467.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t468, gem_string("type")), gem_string(",")))) {
#line 81 "compiler/main.gem"
    GemVal _t469 = (*gem_v_peek);
    GemVal _t470 = _t469.fn(_t469.env, NULL, 0);
    GemVal _t471 = (*gem_v_peek);
    GemVal _t472 = _t471.fn(_t471.env, NULL, 0);
    GemVal _t473 = (*gem_v_peek);
    GemVal _t474 = _t473.fn(_t473.env, NULL, 0);
    GemVal _t475[] = {gem_table_get(_t474, gem_string("value"))};
    GemVal _t476[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t470, gem_string("line")), gem_table_get(_t472, gem_string("col")), gem_len_fn(NULL, _t475, 1), gem_string("only one parameter is allowed after a rest param"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t476, 7));
                }
            }
            break;
        }
#line 86 "compiler/main.gem"
    GemVal _t477 = (*gem_v_advance);
    GemVal _t478 = _t477.fn(_t477.env, NULL, 0);
    GemVal _t479[] = {gem_v_params, gem_table_get(_t478, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t479, 2));
#line 87 "compiler/main.gem"
    GemVal _t480 = (*gem_v_peek);
    GemVal _t481 = _t480.fn(_t480.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t481, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 90 "compiler/main.gem"
    GemVal _t482 = (*gem_v_advance);
        (void)(_t482.fn(_t482.env, NULL, 0));
    }
    GemVal _t483 = gem_table_new();
    gem_table_set(_t483, gem_string("params"), gem_v_params);
    gem_table_set(_t483, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t483, gem_string("block_param"), gem_v_block_param);
    GemVal _t484 = _t483;
    gem_pop_frame();
    return _t484;
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
    GemVal _t486 = gem_table_new();
    GemVal gem_v_stmts = _t486;
#line 110 "compiler/main.gem"
    GemVal _t487 = (*gem_v_skip_nl);
    (void)(_t487.fn(_t487.env, NULL, 0));
#line 111 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t488 = (*gem_v_peek);
        GemVal _t489 = _t488.fn(_t488.env, NULL, 0);
        GemVal _t492;
        if (!gem_truthy(gem_neq(gem_table_get(_t489, gem_string("type")), gem_string("end")))) {
                _t492 = gem_neq(gem_table_get(_t489, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t490 = (*gem_v_peek);
                GemVal _t491 = _t490.fn(_t490.env, NULL, 0);
                _t492 = gem_neq(gem_table_get(_t491, gem_string("type")), gem_string("elif"));
        }
        GemVal _t495;
        if (!gem_truthy(_t492)) {
                _t495 = _t492;
        } else {
                GemVal _t493 = (*gem_v_peek);
                GemVal _t494 = _t493.fn(_t493.env, NULL, 0);
                _t495 = gem_neq(gem_table_get(_t494, gem_string("type")), gem_string("else"));
        }
        GemVal _t498;
        if (!gem_truthy(_t495)) {
                _t498 = _t495;
        } else {
                GemVal _t496 = (*gem_v_peek);
                GemVal _t497 = _t496.fn(_t496.env, NULL, 0);
                _t498 = gem_neq(gem_table_get(_t497, gem_string("type")), gem_string("when"));
        }
        GemVal _t501;
        if (!gem_truthy(_t498)) {
                _t501 = _t498;
        } else {
                GemVal _t499 = (*gem_v_peek);
                GemVal _t500 = _t499.fn(_t499.env, NULL, 0);
                _t501 = gem_neq(gem_table_get(_t500, gem_string("type")), gem_string("after"));
        }
        GemVal _t503;
        if (!gem_truthy(_t501)) {
                _t503 = _t501;
        } else {
                GemVal _t502 = (*gem_v_at_end);
                _t503 = gem_not(_t502.fn(_t502.env, NULL, 0));
        }
        if (!gem_truthy(_t503)) break;
#line 112 "compiler/main.gem"
    GemVal _t504 = (*gem_v_parse_stmt);
    GemVal _t505[] = {gem_v_stmts, _t504.fn(_t504.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t505, 2));
#line 113 "compiler/main.gem"
    GemVal _t506 = (*gem_v_skip_nl);
        (void)(_t506.fn(_t506.env, NULL, 0));
    }
    GemVal _t507 = gem_v_stmts;
    gem_pop_frame();
    return _t507;
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
    GemVal _t509 = gem_table_new();
    GemVal gem_v_stmts = _t509;
#line 123 "compiler/main.gem"
    GemVal _t510 = (*gem_v_skip_nl);
    (void)(_t510.fn(_t510.env, NULL, 0));
#line 124 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t511 = (*gem_v_peek);
        GemVal _t512 = _t511.fn(_t511.env, NULL, 0);
        GemVal _t514;
        if (!gem_truthy(gem_neq(gem_table_get(_t512, gem_string("type")), gem_string("end")))) {
                _t514 = gem_neq(gem_table_get(_t512, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t513 = (*gem_v_at_end);
                _t514 = gem_not(_t513.fn(_t513.env, NULL, 0));
        }
        if (!gem_truthy(_t514)) break;
#line 125 "compiler/main.gem"
    GemVal _t515 = (*gem_v_parse_stmt);
    GemVal _t516[] = {gem_v_stmts, _t515.fn(_t515.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t516, 2));
#line 126 "compiler/main.gem"
    GemVal _t517 = (*gem_v_skip_nl);
        (void)(_t517.fn(_t517.env, NULL, 0));
    }
    GemVal _t518 = gem_v_stmts;
    gem_pop_frame();
    return _t518;
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
    GemVal _t520 = (*gem_v_peek);
    GemVal gem_v_t = _t520.fn(_t520.env, NULL, 0);
#line 138 "compiler/main.gem"
    GemVal _t521 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t521, gem_string("type")), gem_string("NUMBER")))) {
#line 139 "compiler/main.gem"
    GemVal _t522 = (*gem_v_advance);
        (void)(_t522.fn(_t522.env, NULL, 0));
#line 140 "compiler/main.gem"
    GemVal _t523 = gem_v_t;
    GemVal _t524[] = {gem_table_get(_t523, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t524, 1))) {
#line 141 "compiler/main.gem"
    GemVal _t525 = gem_v_t;
    GemVal _t526[] = {gem_table_get(_t525, gem_string("value"))};
    GemVal _t527[] = {gem_fn_atof(NULL, _t526, 1)};
            GemVal _t528 = gem_fn_make_float(NULL, _t527, 1);
            gem_pop_frame();
            return _t528;
        }
#line 143 "compiler/main.gem"
    GemVal _t529 = gem_v_t;
    GemVal _t530[] = {gem_table_get(_t529, gem_string("value"))};
    GemVal _t531[] = {gem_fn_str_to_int(NULL, _t530, 1)};
        GemVal _t532 = gem_fn_make_int(NULL, _t531, 1);
        gem_pop_frame();
        return _t532;
    }
#line 147 "compiler/main.gem"
    GemVal _t533 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t533, gem_string("type")), gem_string("STRING")))) {
#line 148 "compiler/main.gem"
    GemVal _t534 = (*gem_v_advance);
        (void)(_t534.fn(_t534.env, NULL, 0));
#line 149 "compiler/main.gem"
    GemVal _t535 = gem_v_t;
    GemVal _t536[] = {gem_table_get(_t535, gem_string("value"))};
        GemVal _t537 = gem_fn_make_string(NULL, _t536, 1);
        gem_pop_frame();
        return _t537;
    }
#line 153 "compiler/main.gem"
    GemVal _t538 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t538, gem_string("type")), gem_string("INTERP_START")))) {
#line 154 "compiler/main.gem"
    GemVal _t539 = (*gem_v_advance);
        (void)(_t539.fn(_t539.env, NULL, 0));
#line 155 "compiler/main.gem"
    GemVal _t540 = gem_table_new();
        GemVal gem_v_parts = _t540;
#line 156 "compiler/main.gem"
    GemVal _t541 = (*gem_v_skip_nl);
        (void)(_t541.fn(_t541.env, NULL, 0));
#line 157 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t542 = (*gem_v_peek);
            GemVal _t543 = _t542.fn(_t542.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t543, gem_string("type")), gem_string("INTERP_END")))) break;
#line 158 "compiler/main.gem"
    GemVal _t544 = (*gem_v_peek);
    GemVal _t545 = _t544.fn(_t544.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t545, gem_string("type")), gem_string("STRING")))) {
#line 159 "compiler/main.gem"
    GemVal _t546 = (*gem_v_advance);
    GemVal _t547 = _t546.fn(_t546.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t547, gem_string("value"));
#line 160 "compiler/main.gem"
    GemVal _t548[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t548, 1), gem_int(0)))) {
#line 161 "compiler/main.gem"
    GemVal _t549[] = {gem_v_sval};
    GemVal _t550[] = {gem_v_parts, gem_fn_make_string(NULL, _t549, 1)};
                    (void)(gem_push_fn(NULL, _t550, 2));
                }
            } else {
#line 164 "compiler/main.gem"
    GemVal _t551 = (*gem_v_parse_expr);
    GemVal _t552[] = {gem_v_parts, _t551.fn(_t551.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t552, 2));
            }
#line 166 "compiler/main.gem"
    GemVal _t553 = (*gem_v_skip_nl);
            (void)(_t553.fn(_t553.env, NULL, 0));
        }
#line 168 "compiler/main.gem"
    GemVal _t554[] = {gem_string("INTERP_END")};
    GemVal _t555 = (*gem_v_expect);
        (void)(_t555.fn(_t555.env, _t554, 1));
#line 169 "compiler/main.gem"
    GemVal _t556[] = {gem_v_parts};
        GemVal _t557 = gem_fn_make_interp(NULL, _t556, 1);
        gem_pop_frame();
        return _t557;
    }
#line 173 "compiler/main.gem"
    GemVal _t558 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t558, gem_string("type")), gem_string("true")))) {
#line 174 "compiler/main.gem"
    GemVal _t559 = (*gem_v_advance);
        (void)(_t559.fn(_t559.env, NULL, 0));
#line 175 "compiler/main.gem"
    GemVal _t560[] = {gem_bool(1)};
        GemVal _t561 = gem_fn_make_bool(NULL, _t560, 1);
        gem_pop_frame();
        return _t561;
    }
#line 177 "compiler/main.gem"
    GemVal _t562 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t562, gem_string("type")), gem_string("false")))) {
#line 178 "compiler/main.gem"
    GemVal _t563 = (*gem_v_advance);
        (void)(_t563.fn(_t563.env, NULL, 0));
#line 179 "compiler/main.gem"
    GemVal _t564[] = {gem_bool(0)};
        GemVal _t565 = gem_fn_make_bool(NULL, _t564, 1);
        gem_pop_frame();
        return _t565;
    }
#line 183 "compiler/main.gem"
    GemVal _t566 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t566, gem_string("type")), gem_string("nil")))) {
#line 184 "compiler/main.gem"
    GemVal _t567 = (*gem_v_advance);
        (void)(_t567.fn(_t567.env, NULL, 0));
#line 185 "compiler/main.gem"
        GemVal _t568 = gem_fn_make_nil_node(NULL, NULL, 0);
        gem_pop_frame();
        return _t568;
    }
#line 189 "compiler/main.gem"
    GemVal _t569 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t569, gem_string("type")), gem_string("fn")))) {
#line 190 "compiler/main.gem"
    GemVal _t570 = (*gem_v_advance);
        (void)(_t570.fn(_t570.env, NULL, 0));
#line 191 "compiler/main.gem"
    GemVal _t571[] = {gem_string("(")};
    GemVal _t572 = (*gem_v_expect);
        (void)(_t572.fn(_t572.env, _t571, 1));
#line 192 "compiler/main.gem"
    GemVal _t573 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t573.fn(_t573.env, NULL, 0);
#line 193 "compiler/main.gem"
    GemVal _t574[] = {gem_string(")")};
    GemVal _t575 = (*gem_v_expect);
        (void)(_t575.fn(_t575.env, _t574, 1));
#line 194 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 195 "compiler/main.gem"
    GemVal _t576 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t576.fn(_t576.env, NULL, 0);
#line 196 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 197 "compiler/main.gem"
    GemVal _t577[] = {gem_string("end")};
    GemVal _t578 = (*gem_v_expect);
        (void)(_t578.fn(_t578.env, _t577, 1));
#line 198 "compiler/main.gem"
    GemVal _t579 = gem_v_pr;
    GemVal _t580 = gem_v_pr;
    GemVal _t581 = gem_v_pr;
    GemVal _t582[] = {gem_table_get(_t579, gem_string("params")), gem_table_get(_t580, gem_string("rest_param")), gem_table_get(_t581, gem_string("block_param")), gem_v_body};
        GemVal _t583 = gem_fn_make_anon_fn(NULL, _t582, 4);
        gem_pop_frame();
        return _t583;
    }
#line 203 "compiler/main.gem"
    GemVal _t584 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t584, gem_string("type")), gem_string("{")))) {
#line 204 "compiler/main.gem"
    GemVal _t585 = (*gem_v_advance);
        (void)(_t585.fn(_t585.env, NULL, 0));
#line 205 "compiler/main.gem"
    GemVal _t586 = (*gem_v_skip_nl);
        (void)(_t586.fn(_t586.env, NULL, 0));
#line 206 "compiler/main.gem"
    GemVal _t587 = gem_table_new();
        GemVal gem_v_entries = _t587;
#line 207 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t588 = (*gem_v_peek);
            GemVal _t589 = _t588.fn(_t588.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t589, gem_string("type")), gem_string("}")))) break;
#line 208 "compiler/main.gem"
    GemVal _t590 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t590.fn(_t590.env, NULL, 0);
#line 209 "compiler/main.gem"
    GemVal _t591 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t591, gem_string("type")), gem_string("NAME")))) {
#line 210 "compiler/main.gem"
    GemVal _t592 = (*gem_v_advance);
                (void)(_t592.fn(_t592.env, NULL, 0));
            } else {
#line 211 "compiler/main.gem"
    GemVal _t593 = (*gem_v_at_end);
    GemVal _t597;
    if (!gem_truthy(gem_not(_t593.fn(_t593.env, NULL, 0)))) {
        _t597 = gem_not(_t593.fn(_t593.env, NULL, 0));
    } else {
        GemVal _t594[] = {gem_int(1)};
        GemVal _t595 = (*gem_v_peek_at);
        GemVal _t596 = _t595.fn(_t595.env, _t594, 1);
        _t597 = gem_eq(gem_table_get(_t596, gem_string("type")), gem_string(":"));
    }
                if (gem_truthy(_t597)) {
#line 213 "compiler/main.gem"
    GemVal _t598 = (*gem_v_advance);
                    (void)(_t598.fn(_t598.env, NULL, 0));
                } else {
#line 215 "compiler/main.gem"
    GemVal _t599 = gem_v_key_tok;
    GemVal _t600 = gem_v_key_tok;
    GemVal _t601 = gem_v_key_tok;
    GemVal _t602[] = {gem_table_get(_t601, gem_string("value"))};
    GemVal _t603 = gem_v_key_tok;
    GemVal _t604[] = {gem_table_get(_t603, gem_string("type"))};
    GemVal _t605[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t599, gem_string("line")), gem_table_get(_t600, gem_string("col")), gem_len_fn(NULL, _t602, 1), gem_add(gem_add(gem_string("expected '}' or table key but got '"), gem_to_string_fn(NULL, _t604, 1)), gem_string("'")), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t605, 7));
                }
            }
#line 217 "compiler/main.gem"
    GemVal _t606[] = {gem_string(":")};
    GemVal _t607 = (*gem_v_expect);
            (void)(_t607.fn(_t607.env, _t606, 1));
#line 218 "compiler/main.gem"
    GemVal _t608 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t608.fn(_t608.env, NULL, 0);
#line 219 "compiler/main.gem"
    GemVal _t609 = gem_v_key_tok;
    GemVal _t610[] = {gem_table_get(_t609, gem_string("value")), gem_v_val};
    GemVal _t611[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t610, 2)};
            (void)(gem_push_fn(NULL, _t611, 2));
#line 220 "compiler/main.gem"
    GemVal _t612 = (*gem_v_skip_nl);
            (void)(_t612.fn(_t612.env, NULL, 0));
#line 221 "compiler/main.gem"
    GemVal _t613 = (*gem_v_peek);
    GemVal _t614 = _t613.fn(_t613.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t614, gem_string("type")), gem_string(",")))) {
#line 222 "compiler/main.gem"
    GemVal _t615 = (*gem_v_advance);
                (void)(_t615.fn(_t615.env, NULL, 0));
#line 223 "compiler/main.gem"
    GemVal _t616 = (*gem_v_skip_nl);
                (void)(_t616.fn(_t616.env, NULL, 0));
            }
        }
#line 226 "compiler/main.gem"
    GemVal _t617[] = {gem_string("}")};
    GemVal _t618 = (*gem_v_expect);
        (void)(_t618.fn(_t618.env, _t617, 1));
#line 227 "compiler/main.gem"
    GemVal _t619[] = {gem_v_entries};
        GemVal _t620 = gem_fn_make_table(NULL, _t619, 1);
        gem_pop_frame();
        return _t620;
    }
#line 231 "compiler/main.gem"
    GemVal _t621 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t621, gem_string("type")), gem_string("[")))) {
#line 232 "compiler/main.gem"
    GemVal _t622 = (*gem_v_advance);
        (void)(_t622.fn(_t622.env, NULL, 0));
#line 233 "compiler/main.gem"
    GemVal _t623 = (*gem_v_skip_nl);
        (void)(_t623.fn(_t623.env, NULL, 0));
#line 234 "compiler/main.gem"
    GemVal _t624 = gem_table_new();
        GemVal gem_v_elements = _t624;
#line 235 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t625 = (*gem_v_peek);
            GemVal _t626 = _t625.fn(_t625.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t626, gem_string("type")), gem_string("]")))) break;
#line 236 "compiler/main.gem"
    GemVal _t627 = (*gem_v_parse_expr);
    GemVal _t628[] = {gem_v_elements, _t627.fn(_t627.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t628, 2));
#line 237 "compiler/main.gem"
    GemVal _t629 = (*gem_v_skip_nl);
            (void)(_t629.fn(_t629.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t630 = (*gem_v_peek);
    GemVal _t631 = _t630.fn(_t630.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t631, gem_string("type")), gem_string(",")))) {
#line 239 "compiler/main.gem"
    GemVal _t632 = (*gem_v_advance);
                (void)(_t632.fn(_t632.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t633 = (*gem_v_skip_nl);
                (void)(_t633.fn(_t633.env, NULL, 0));
            }
        }
#line 243 "compiler/main.gem"
    GemVal _t634[] = {gem_string("]")};
    GemVal _t635 = (*gem_v_expect);
        (void)(_t635.fn(_t635.env, _t634, 1));
#line 244 "compiler/main.gem"
    GemVal _t636[] = {gem_v_elements};
        GemVal _t637 = gem_fn_make_array(NULL, _t636, 1);
        gem_pop_frame();
        return _t637;
    }
#line 248 "compiler/main.gem"
    GemVal _t638 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t638, gem_string("type")), gem_string("(")))) {
#line 249 "compiler/main.gem"
    GemVal _t639 = (*gem_v_advance);
        (void)(_t639.fn(_t639.env, NULL, 0));
#line 250 "compiler/main.gem"
    GemVal _t640 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t640.fn(_t640.env, NULL, 0);
#line 251 "compiler/main.gem"
    GemVal _t641[] = {gem_string(")")};
    GemVal _t642 = (*gem_v_expect);
        (void)(_t642.fn(_t642.env, _t641, 1));
#line 252 "compiler/main.gem"
        GemVal _t643 = gem_v_e;
        gem_pop_frame();
        return _t643;
    }
#line 256 "compiler/main.gem"
    GemVal _t644 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t644, gem_string("type")), gem_string("NAME")))) {
#line 257 "compiler/main.gem"
    GemVal _t645 = (*gem_v_advance);
        (void)(_t645.fn(_t645.env, NULL, 0));
#line 258 "compiler/main.gem"
    GemVal _t646 = gem_v_t;
    GemVal _t647[] = {gem_table_get(_t646, gem_string("value"))};
        GemVal _t648 = gem_fn_make_var(NULL, _t647, 1);
        gem_pop_frame();
        return _t648;
    }
#line 261 "compiler/main.gem"
    GemVal _t649 = gem_v_t;
    GemVal _t650 = gem_v_t;
    GemVal _t651 = gem_v_t;
    GemVal _t652[] = {gem_table_get(_t651, gem_string("value"))};
    GemVal _t653 = gem_v_t;
    GemVal _t654[] = {gem_table_get(_t653, gem_string("type"))};
    GemVal _t655[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t649, gem_string("line")), gem_table_get(_t650, gem_string("col")), gem_len_fn(NULL, _t652, 1), gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t654, 1)), gem_string("' (expected expression)")), GEM_NIL};
    GemVal _t656 = gem_fn_compile_error(NULL, _t655, 7);
    gem_pop_frame();
    return _t656;
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
    GemVal _t658 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t658.fn(_t658.env, NULL, 0);
#line 268 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 270 "compiler/main.gem"
    GemVal _t659 = (*gem_v_peek);
    GemVal _t660 = _t659.fn(_t659.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t660, gem_string("type")), gem_string("(")))) {
#line 271 "compiler/main.gem"
    GemVal _t661 = (*gem_v_peek);
    GemVal _t662 = _t661.fn(_t661.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t662, gem_string("line"));
#line 272 "compiler/main.gem"
    GemVal _t663 = (*gem_v_advance);
            (void)(_t663.fn(_t663.env, NULL, 0));
#line 273 "compiler/main.gem"
    GemVal _t664 = (*gem_v_skip_nl);
            (void)(_t664.fn(_t664.env, NULL, 0));
#line 274 "compiler/main.gem"
    GemVal _t665 = gem_table_new();
            GemVal gem_v_args = _t665;
#line 275 "compiler/main.gem"
    GemVal _t666 = (*gem_v_peek);
    GemVal _t667 = _t666.fn(_t666.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t667, gem_string("type")), gem_string(")")))) {
#line 276 "compiler/main.gem"
    GemVal _t668 = (*gem_v_parse_expr);
    GemVal _t669[] = {gem_v_args, _t668.fn(_t668.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t669, 2));
#line 277 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t670 = (*gem_v_peek);
                    GemVal _t671 = _t670.fn(_t670.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t671, gem_string("type")), gem_string(",")))) break;
#line 278 "compiler/main.gem"
    GemVal _t672 = (*gem_v_advance);
                    (void)(_t672.fn(_t672.env, NULL, 0));
#line 279 "compiler/main.gem"
    GemVal _t673 = (*gem_v_skip_nl);
                    (void)(_t673.fn(_t673.env, NULL, 0));
#line 280 "compiler/main.gem"
    GemVal _t674 = (*gem_v_parse_expr);
    GemVal _t675[] = {gem_v_args, _t674.fn(_t674.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t675, 2));
                }
            }
#line 283 "compiler/main.gem"
    GemVal _t676 = (*gem_v_skip_nl);
            (void)(_t676.fn(_t676.env, NULL, 0));
#line 284 "compiler/main.gem"
    GemVal _t677[] = {gem_string(")")};
    GemVal _t678 = (*gem_v_expect);
            (void)(_t678.fn(_t678.env, _t677, 1));
#line 287 "compiler/main.gem"
    GemVal _t679 = (*gem_v_peek);
    GemVal _t680 = _t679.fn(_t679.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t680, gem_string("type")), gem_string("do")))) {
#line 288 "compiler/main.gem"
    GemVal _t681 = (*gem_v_advance);
                (void)(_t681.fn(_t681.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t682 = gem_table_new();
                GemVal gem_v_bparams = _t682;
#line 290 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 291 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 292 "compiler/main.gem"
    GemVal _t683 = (*gem_v_peek);
    GemVal _t684 = _t683.fn(_t683.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t684, gem_string("type")), gem_string("|")))) {
#line 293 "compiler/main.gem"
    GemVal _t685 = (*gem_v_advance);
                    (void)(_t685.fn(_t685.env, NULL, 0));
#line 294 "compiler/main.gem"
    GemVal _t686 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t686.fn(_t686.env, NULL, 0);
#line 295 "compiler/main.gem"
    GemVal _t687 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t687, gem_string("params"));
#line 296 "compiler/main.gem"
    GemVal _t688 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t688, gem_string("rest_param"));
#line 297 "compiler/main.gem"
    GemVal _t689 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t689, gem_string("block_param"));
#line 298 "compiler/main.gem"
    GemVal _t690[] = {gem_string("|")};
    GemVal _t691 = (*gem_v_expect);
                    (void)(_t691.fn(_t691.env, _t690, 1));
                }
#line 300 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 301 "compiler/main.gem"
    GemVal _t692 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t692.fn(_t692.env, NULL, 0);
#line 302 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 303 "compiler/main.gem"
    GemVal _t693[] = {gem_string("end")};
    GemVal _t694 = (*gem_v_expect);
                (void)(_t694.fn(_t694.env, _t693, 1));
#line 304 "compiler/main.gem"
    GemVal _t695[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t696[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t695, 4)};
                (void)(gem_push_fn(NULL, _t696, 2));
            } else {
#line 308 "compiler/main.gem"
    GemVal _t697 = (*gem_v_peek);
    GemVal _t698 = _t697.fn(_t697.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t698, gem_string("type")), gem_string("{")))) {
#line 309 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 310 "compiler/main.gem"
    GemVal _t699[] = {gem_int(1)};
    GemVal _t700 = (*gem_v_peek_at);
    GemVal _t701 = _t700.fn(_t700.env, _t699, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t701, gem_string("type")), gem_string("|")))) {
#line 311 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 312 "compiler/main.gem"
    GemVal _t702[] = {gem_int(1)};
    GemVal _t703 = (*gem_v_peek_at);
    GemVal _t704 = _t703.fn(_t703.env, _t702, 1);
    GemVal _t712;
    if (gem_truthy(gem_eq(gem_table_get(_t704, gem_string("type")), gem_string("}")))) {
        _t712 = gem_eq(gem_table_get(_t704, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t705[] = {gem_int(1)};
        GemVal _t706 = (*gem_v_peek_at);
        GemVal _t707 = _t706.fn(_t706.env, _t705, 1);
        GemVal _t711;
        if (!gem_truthy(gem_eq(gem_table_get(_t707, gem_string("type")), gem_string("NAME")))) {
                _t711 = gem_eq(gem_table_get(_t707, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t708[] = {gem_int(2)};
                GemVal _t709 = (*gem_v_peek_at);
                GemVal _t710 = _t709.fn(_t709.env, _t708, 1);
                _t711 = gem_eq(gem_table_get(_t710, gem_string("type")), gem_string(":"));
        }
        _t712 = _t711;
    }
                        if (gem_truthy(_t712)) {
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
    GemVal _t713 = (*gem_v_advance);
                        (void)(_t713.fn(_t713.env, NULL, 0));
#line 319 "compiler/main.gem"
    GemVal _t714 = gem_table_new();
                        GemVal gem_v_bparams = _t714;
#line 320 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 321 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 322 "compiler/main.gem"
    GemVal _t715 = (*gem_v_peek);
    GemVal _t716 = _t715.fn(_t715.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t716, gem_string("type")), gem_string("|")))) {
#line 323 "compiler/main.gem"
    GemVal _t717 = (*gem_v_advance);
                            (void)(_t717.fn(_t717.env, NULL, 0));
#line 324 "compiler/main.gem"
    GemVal _t718 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t718.fn(_t718.env, NULL, 0);
#line 325 "compiler/main.gem"
    GemVal _t719 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t719, gem_string("params"));
#line 326 "compiler/main.gem"
    GemVal _t720 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t720, gem_string("rest_param"));
#line 327 "compiler/main.gem"
    GemVal _t721 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t721, gem_string("block_param"));
#line 328 "compiler/main.gem"
    GemVal _t722[] = {gem_string("|")};
    GemVal _t723 = (*gem_v_expect);
                            (void)(_t723.fn(_t723.env, _t722, 1));
                        }
#line 330 "compiler/main.gem"
    GemVal _t724 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t724.fn(_t724.env, NULL, 0);
#line 331 "compiler/main.gem"
    GemVal _t725[] = {gem_string("}")};
    GemVal _t726 = (*gem_v_expect);
                        (void)(_t726.fn(_t726.env, _t725, 1));
#line 332 "compiler/main.gem"
    GemVal _t727 = gem_table_new();
    gem_table_set(_t727, gem_int(0), gem_v_bexpr);
    GemVal _t728[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t727};
    GemVal _t729[] = {gem_v_args, gem_fn_make_anon_fn(NULL, _t728, 4)};
                        (void)(gem_push_fn(NULL, _t729, 2));
                    }
                }
            }
#line 337 "compiler/main.gem"
    GemVal _t730[] = {gem_v_node, gem_v_args, gem_v_call_line};
            gem_v_node = gem_fn_make_call(NULL, _t730, 3);
            continue;
        }
#line 342 "compiler/main.gem"
    GemVal _t731 = (*gem_v_peek);
    GemVal _t732 = _t731.fn(_t731.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t732, gem_string("type")), gem_string(".")))) {
#line 343 "compiler/main.gem"
    GemVal _t733 = (*gem_v_advance);
            (void)(_t733.fn(_t733.env, NULL, 0));
#line 344 "compiler/main.gem"
    GemVal _t734 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t734.fn(_t734.env, NULL, 0);
#line 345 "compiler/main.gem"
    GemVal _t735 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t735, gem_string("type")), gem_string("NAME")))) {
#line 346 "compiler/main.gem"
    GemVal _t736 = (*gem_v_advance);
                (void)(_t736.fn(_t736.env, NULL, 0));
            } else {
#line 349 "compiler/main.gem"
    GemVal _t737 = (*gem_v_advance);
                (void)(_t737.fn(_t737.env, NULL, 0));
            }
#line 351 "compiler/main.gem"
    GemVal _t738 = gem_v_field_tok;
    GemVal _t739[] = {gem_v_node, gem_table_get(_t738, gem_string("value"))};
            gem_v_node = gem_fn_make_dot(NULL, _t739, 2);
            continue;
        }
#line 356 "compiler/main.gem"
    GemVal _t740 = (*gem_v_peek);
    GemVal _t741 = _t740.fn(_t740.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t741, gem_string("type")), gem_string("[")))) {
#line 357 "compiler/main.gem"
    GemVal _t742 = (*gem_v_advance);
            (void)(_t742.fn(_t742.env, NULL, 0));
#line 358 "compiler/main.gem"
    GemVal _t743 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t743.fn(_t743.env, NULL, 0);
#line 359 "compiler/main.gem"
    GemVal _t744[] = {gem_string("]")};
    GemVal _t745 = (*gem_v_expect);
            (void)(_t745.fn(_t745.env, _t744, 1));
#line 360 "compiler/main.gem"
    GemVal _t746[] = {gem_v_node, gem_v_key};
            gem_v_node = gem_fn_make_index(NULL, _t746, 2);
            continue;
        }
        break;
    }
    GemVal _t747 = gem_v_node;
    gem_pop_frame();
    return _t747;
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
    GemVal _t749 = (*gem_v_peek);
    GemVal _t750 = _t749.fn(_t749.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t750, gem_string("type")), gem_string("-")))) {
#line 373 "compiler/main.gem"
    GemVal _t751 = (*gem_v_advance);
        (void)(_t751.fn(_t751.env, NULL, 0));
#line 374 "compiler/main.gem"
    GemVal _t752 = (*gem_v_parse_unary);
    GemVal _t753[] = {gem_string("-"), _t752.fn(_t752.env, NULL, 0)};
        GemVal _t754 = gem_fn_make_unop(NULL, _t753, 2);
        gem_pop_frame();
        return _t754;
    }
#line 376 "compiler/main.gem"
    GemVal _t755 = (*gem_v_parse_call);
    GemVal _t756 = _t755.fn(_t755.env, NULL, 0);
    gem_pop_frame();
    return _t756;
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
    GemVal _t758 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t758.fn(_t758.env, NULL, 0);
#line 382 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t759 = (*gem_v_peek);
        GemVal _t760 = _t759.fn(_t759.env, NULL, 0);
        GemVal _t763;
        if (gem_truthy(gem_eq(gem_table_get(_t760, gem_string("type")), gem_string("*")))) {
                _t763 = gem_eq(gem_table_get(_t760, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t761 = (*gem_v_peek);
                GemVal _t762 = _t761.fn(_t761.env, NULL, 0);
                _t763 = gem_eq(gem_table_get(_t762, gem_string("type")), gem_string("/"));
        }
        GemVal _t766;
        if (gem_truthy(_t763)) {
                _t766 = _t763;
        } else {
                GemVal _t764 = (*gem_v_peek);
                GemVal _t765 = _t764.fn(_t764.env, NULL, 0);
                _t766 = gem_eq(gem_table_get(_t765, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t766)) break;
#line 383 "compiler/main.gem"
    GemVal _t767 = (*gem_v_advance);
    GemVal _t768 = _t767.fn(_t767.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t768, gem_string("type"));
#line 384 "compiler/main.gem"
    GemVal _t769 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t769.fn(_t769.env, NULL, 0);
#line 385 "compiler/main.gem"
    GemVal _t770[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t770, 3);
    }
    GemVal _t771 = gem_v_left;
    gem_pop_frame();
    return _t771;
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
    GemVal _t773 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t773.fn(_t773.env, NULL, 0);
#line 393 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t774 = (*gem_v_peek);
        GemVal _t775 = _t774.fn(_t774.env, NULL, 0);
        GemVal _t778;
        if (gem_truthy(gem_eq(gem_table_get(_t775, gem_string("type")), gem_string("+")))) {
                _t778 = gem_eq(gem_table_get(_t775, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t776 = (*gem_v_peek);
                GemVal _t777 = _t776.fn(_t776.env, NULL, 0);
                _t778 = gem_eq(gem_table_get(_t777, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t778)) break;
#line 394 "compiler/main.gem"
    GemVal _t779 = (*gem_v_advance);
    GemVal _t780 = _t779.fn(_t779.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t780, gem_string("type"));
#line 395 "compiler/main.gem"
    GemVal _t781 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t781.fn(_t781.env, NULL, 0);
#line 396 "compiler/main.gem"
    GemVal _t782[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t782, 3);
    }
    GemVal _t783 = gem_v_left;
    gem_pop_frame();
    return _t783;
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
    GemVal _t785 = (*gem_v_parse_add);
    GemVal gem_v_left = _t785.fn(_t785.env, NULL, 0);
#line 404 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t786 = (*gem_v_peek);
        GemVal _t787 = _t786.fn(_t786.env, NULL, 0);
        GemVal _t790;
        if (gem_truthy(gem_eq(gem_table_get(_t787, gem_string("type")), gem_string("==")))) {
                _t790 = gem_eq(gem_table_get(_t787, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t788 = (*gem_v_peek);
                GemVal _t789 = _t788.fn(_t788.env, NULL, 0);
                _t790 = gem_eq(gem_table_get(_t789, gem_string("type")), gem_string("!="));
        }
        GemVal _t793;
        if (gem_truthy(_t790)) {
                _t793 = _t790;
        } else {
                GemVal _t791 = (*gem_v_peek);
                GemVal _t792 = _t791.fn(_t791.env, NULL, 0);
                _t793 = gem_eq(gem_table_get(_t792, gem_string("type")), gem_string("<"));
        }
        GemVal _t796;
        if (gem_truthy(_t793)) {
                _t796 = _t793;
        } else {
                GemVal _t794 = (*gem_v_peek);
                GemVal _t795 = _t794.fn(_t794.env, NULL, 0);
                _t796 = gem_eq(gem_table_get(_t795, gem_string("type")), gem_string(">"));
        }
        GemVal _t799;
        if (gem_truthy(_t796)) {
                _t799 = _t796;
        } else {
                GemVal _t797 = (*gem_v_peek);
                GemVal _t798 = _t797.fn(_t797.env, NULL, 0);
                _t799 = gem_eq(gem_table_get(_t798, gem_string("type")), gem_string("<="));
        }
        GemVal _t802;
        if (gem_truthy(_t799)) {
                _t802 = _t799;
        } else {
                GemVal _t800 = (*gem_v_peek);
                GemVal _t801 = _t800.fn(_t800.env, NULL, 0);
                _t802 = gem_eq(gem_table_get(_t801, gem_string("type")), gem_string(">="));
        }
        GemVal _t805;
        if (gem_truthy(_t802)) {
                _t805 = _t802;
        } else {
                GemVal _t803 = (*gem_v_peek);
                GemVal _t804 = _t803.fn(_t803.env, NULL, 0);
                _t805 = gem_eq(gem_table_get(_t804, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t805)) break;
#line 405 "compiler/main.gem"
    GemVal _t806 = (*gem_v_advance);
    GemVal _t807 = _t806.fn(_t806.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t807, gem_string("type"));
#line 406 "compiler/main.gem"
    GemVal _t808 = (*gem_v_parse_add);
        GemVal gem_v_right = _t808.fn(_t808.env, NULL, 0);
#line 407 "compiler/main.gem"
    GemVal _t809[] = {gem_v_op, gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t809, 3);
    }
    GemVal _t810 = gem_v_left;
    gem_pop_frame();
    return _t810;
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
    GemVal _t812 = (*gem_v_peek);
    GemVal _t813 = _t812.fn(_t812.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t813, gem_string("type")), gem_string("not")))) {
#line 415 "compiler/main.gem"
    GemVal _t814 = (*gem_v_advance);
        (void)(_t814.fn(_t814.env, NULL, 0));
#line 416 "compiler/main.gem"
    GemVal _t815 = (*gem_v_parse_not);
    GemVal _t816[] = {gem_string("not"), _t815.fn(_t815.env, NULL, 0)};
        GemVal _t817 = gem_fn_make_unop(NULL, _t816, 2);
        gem_pop_frame();
        return _t817;
    }
#line 418 "compiler/main.gem"
    GemVal _t818 = (*gem_v_parse_compare);
    GemVal _t819 = _t818.fn(_t818.env, NULL, 0);
    gem_pop_frame();
    return _t819;
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
    GemVal _t821 = (*gem_v_parse_not);
    GemVal gem_v_left = _t821.fn(_t821.env, NULL, 0);
#line 424 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t822 = (*gem_v_peek);
        GemVal _t823 = _t822.fn(_t822.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t823, gem_string("type")), gem_string("and")))) break;
#line 425 "compiler/main.gem"
    GemVal _t824 = (*gem_v_advance);
        (void)(_t824.fn(_t824.env, NULL, 0));
#line 426 "compiler/main.gem"
    GemVal _t825 = (*gem_v_parse_not);
        GemVal gem_v_right = _t825.fn(_t825.env, NULL, 0);
#line 427 "compiler/main.gem"
    GemVal _t826[] = {gem_string("and"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t826, 3);
    }
    GemVal _t827 = gem_v_left;
    gem_pop_frame();
    return _t827;
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
    GemVal _t829 = (*gem_v_parse_and);
    GemVal gem_v_left = _t829.fn(_t829.env, NULL, 0);
#line 435 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t830 = (*gem_v_peek);
        GemVal _t831 = _t830.fn(_t830.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t831, gem_string("type")), gem_string("or")))) break;
#line 436 "compiler/main.gem"
    GemVal _t832 = (*gem_v_advance);
        (void)(_t832.fn(_t832.env, NULL, 0));
#line 437 "compiler/main.gem"
    GemVal _t833 = (*gem_v_parse_and);
        GemVal gem_v_right = _t833.fn(_t833.env, NULL, 0);
#line 438 "compiler/main.gem"
    GemVal _t834[] = {gem_string("or"), gem_v_left, gem_v_right};
        gem_v_left = gem_fn_make_binop(NULL, _t834, 3);
    }
    GemVal _t835 = gem_v_left;
    gem_pop_frame();
    return _t835;
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
    GemVal _t837 = (*gem_v_peek);
    GemVal _t838 = _t837.fn(_t837.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t838, gem_string("line"));
#line 446 "compiler/main.gem"
    GemVal _t839 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t839.fn(_t839.env, NULL, 0);
#line 449 "compiler/main.gem"
    GemVal _t840 = (*gem_v_peek);
    GemVal _t841 = _t840.fn(_t840.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t841, gem_string("type")), gem_string("=")))) {
#line 450 "compiler/main.gem"
    GemVal _t842 = (*gem_v_advance);
        (void)(_t842.fn(_t842.env, NULL, 0));
#line 451 "compiler/main.gem"
    GemVal _t843 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t843.fn(_t843.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t844 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t844, gem_string("tag")), gem_string("var")))) {
#line 453 "compiler/main.gem"
    GemVal _t845 = gem_v_lhs;
    GemVal _t846[] = {gem_table_get(_t845, gem_string("name")), gem_v_value, gem_v_start_line};
            GemVal _t847 = gem_fn_make_assign(NULL, _t846, 3);
            gem_pop_frame();
            return _t847;
        }
#line 455 "compiler/main.gem"
    GemVal _t848 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t848, gem_string("tag")), gem_string("dot")))) {
#line 456 "compiler/main.gem"
    GemVal _t849 = gem_v_lhs;
    GemVal _t850 = gem_v_lhs;
    GemVal _t851[] = {gem_table_get(_t849, gem_string("object")), gem_table_get(_t850, gem_string("field")), gem_v_value, gem_v_start_line};
            GemVal _t852 = gem_fn_make_dot_assign(NULL, _t851, 4);
            gem_pop_frame();
            return _t852;
        }
#line 458 "compiler/main.gem"
    GemVal _t853 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t853, gem_string("tag")), gem_string("index")))) {
#line 459 "compiler/main.gem"
    GemVal _t854 = gem_v_lhs;
    GemVal _t855 = gem_v_lhs;
    GemVal _t856[] = {gem_table_get(_t854, gem_string("object")), gem_table_get(_t855, gem_string("key")), gem_v_value, gem_v_start_line};
            GemVal _t857 = gem_fn_make_index_assign(NULL, _t856, 4);
            gem_pop_frame();
            return _t857;
        }
#line 461 "compiler/main.gem"
    GemVal _t858[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
        (void)(gem_fn_compile_error(NULL, _t858, 7));
    }
#line 465 "compiler/main.gem"
    GemVal _t859 = (*gem_v_peek);
    GemVal _t860 = _t859.fn(_t859.env, NULL, 0);
    GemVal _t863;
    if (gem_truthy(gem_eq(gem_table_get(_t860, gem_string("type")), gem_string("+=")))) {
        _t863 = gem_eq(gem_table_get(_t860, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t861 = (*gem_v_peek);
        GemVal _t862 = _t861.fn(_t861.env, NULL, 0);
        _t863 = gem_eq(gem_table_get(_t862, gem_string("type")), gem_string("-="));
    }
    GemVal _t866;
    if (gem_truthy(_t863)) {
        _t866 = _t863;
    } else {
        GemVal _t864 = (*gem_v_peek);
        GemVal _t865 = _t864.fn(_t864.env, NULL, 0);
        _t866 = gem_eq(gem_table_get(_t865, gem_string("type")), gem_string("*="));
    }
    GemVal _t869;
    if (gem_truthy(_t866)) {
        _t869 = _t866;
    } else {
        GemVal _t867 = (*gem_v_peek);
        GemVal _t868 = _t867.fn(_t867.env, NULL, 0);
        _t869 = gem_eq(gem_table_get(_t868, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t869)) {
#line 466 "compiler/main.gem"
    GemVal _t870 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t870.fn(_t870.env, NULL, 0);
#line 467 "compiler/main.gem"
    GemVal _t871 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t871, gem_string("value")), gem_int(0));
#line 468 "compiler/main.gem"
    GemVal _t872 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t872.fn(_t872.env, NULL, 0);
#line 469 "compiler/main.gem"
    GemVal _t873 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t873, gem_string("tag")), gem_string("var")))) {
#line 470 "compiler/main.gem"
    GemVal _t874 = gem_v_op_tok;
    GemVal _t875 = gem_v_op_tok;
    GemVal _t876 = gem_v_op_tok;
    GemVal _t877[] = {gem_table_get(_t876, gem_string("value"))};
    GemVal _t878[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t874, gem_string("line")), gem_table_get(_t875, gem_string("col")), gem_len_fn(NULL, _t877, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
            (void)(gem_fn_compile_error(NULL, _t878, 7));
        }
#line 472 "compiler/main.gem"
    GemVal _t879 = gem_v_lhs;
    GemVal _t880 = gem_v_lhs;
    GemVal _t881[] = {gem_table_get(_t880, gem_string("name"))};
    GemVal _t882[] = {gem_v_base_op, gem_fn_make_var(NULL, _t881, 1), gem_v_value};
    GemVal _t883[] = {gem_table_get(_t879, gem_string("name")), gem_fn_make_binop(NULL, _t882, 3), gem_v_start_line};
        GemVal _t884 = gem_fn_make_assign(NULL, _t883, 3);
        gem_pop_frame();
        return _t884;
    }
    GemVal _t885 = gem_v_lhs;
    gem_pop_frame();
    return _t885;
}

struct _closure__anon_21 {
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 485 "compiler/main.gem"
    GemVal _t887[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t887, 1), gem_int(0)))) {
#line 486 "compiler/main.gem"
    GemVal _t888[] = {gem_bool(1)};
        GemVal _t889 = gem_fn_make_bool(NULL, _t888, 1);
        gem_pop_frame();
        return _t889;
    }
#line 488 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 489 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(1);
#line 489 "compiler/main.gem"
    GemVal _t890[] = {gem_v_conditions};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t890, 1);
#line 489 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 489 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 489 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 490 "compiler/main.gem"
    GemVal _t891[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
        gem_v_result = gem_fn_make_binop(NULL, _t891, 3);
    }

    GemVal _t892 = gem_v_result;
    gem_pop_frame();
    return _t892;
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
    GemVal _t893 = (*gem_v_peek);
    GemVal gem_v_t = _t893.fn(_t893.env, NULL, 0);
#line 497 "compiler/main.gem"
    GemVal _t894 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t894, gem_string("type")), gem_string("{")))) {
#line 498 "compiler/main.gem"
    GemVal _t895 = (*gem_v_advance);
        (void)(_t895.fn(_t895.env, NULL, 0));
#line 499 "compiler/main.gem"
    GemVal _t896 = (*gem_v_skip_nl);
        (void)(_t896.fn(_t896.env, NULL, 0));
#line 500 "compiler/main.gem"
    GemVal _t897 = gem_table_new();
    GemVal _t898[] = {gem_string("type")};
    GemVal _t899 = gem_table_new();
    gem_table_set(_t899, gem_int(0), gem_v_target_expr);
    GemVal _t900[] = {gem_fn_make_var(NULL, _t898, 1), _t899, gem_int(0)};
    GemVal _t901[] = {gem_string("table")};
    GemVal _t902[] = {gem_string("=="), gem_fn_make_call(NULL, _t900, 3), gem_fn_make_string(NULL, _t901, 1)};
    gem_table_set(_t897, gem_int(0), gem_fn_make_binop(NULL, _t902, 3));
        GemVal gem_v_conditions = _t897;
#line 501 "compiler/main.gem"
    GemVal _t903 = gem_table_new();
        GemVal gem_v_bindings = _t903;
#line 502 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t904 = (*gem_v_peek);
            GemVal _t905 = _t904.fn(_t904.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t905, gem_string("type")), gem_string("}")))) break;
#line 503 "compiler/main.gem"
    GemVal _t906 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t906.fn(_t906.env, NULL, 0);
#line 504 "compiler/main.gem"
    GemVal _t907 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t907, gem_string("type")), gem_string("NAME")))) {
#line 505 "compiler/main.gem"
    GemVal _t908 = (*gem_v_advance);
                (void)(_t908.fn(_t908.env, NULL, 0));
            } else {
#line 506 "compiler/main.gem"
    GemVal _t909[] = {gem_int(1)};
    GemVal _t910 = (*gem_v_peek_at);
    GemVal _t911 = _t910.fn(_t910.env, _t909, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t911, gem_string("type")), gem_string(":")))) {
#line 508 "compiler/main.gem"
    GemVal _t912 = (*gem_v_advance);
                    (void)(_t912.fn(_t912.env, NULL, 0));
                } else {
#line 510 "compiler/main.gem"
    GemVal _t913 = gem_v_key_tok;
    GemVal _t914 = gem_v_key_tok;
    GemVal _t915 = gem_v_key_tok;
    GemVal _t916[] = {gem_table_get(_t915, gem_string("value"))};
    GemVal _t917[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t913, gem_string("line")), gem_table_get(_t914, gem_string("col")), gem_len_fn(NULL, _t916, 1), gem_string("expected key name in table pattern"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t917, 7));
                }
            }
#line 512 "compiler/main.gem"
    GemVal _t918 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t918, gem_string("value"));
#line 513 "compiler/main.gem"
    GemVal _t919[] = {gem_v_key_name};
            GemVal gem_v_key_str = gem_fn_make_string(NULL, _t919, 1);
#line 514 "compiler/main.gem"
    GemVal _t920[] = {gem_string("has_key")};
    GemVal _t921 = gem_table_new();
    gem_table_set(_t921, gem_int(0), gem_v_target_expr);
    gem_table_set(_t921, gem_int(1), gem_v_key_str);
    GemVal _t922[] = {gem_fn_make_var(NULL, _t920, 1), _t921, gem_int(0)};
    GemVal _t923[] = {gem_v_conditions, gem_fn_make_call(NULL, _t922, 3)};
            (void)(gem_push_fn(NULL, _t923, 2));
#line 515 "compiler/main.gem"
    GemVal _t924[] = {gem_v_target_expr, gem_v_key_str};
            GemVal gem_v_sub_target = gem_fn_make_index(NULL, _t924, 2);
#line 516 "compiler/main.gem"
    GemVal _t925 = (*gem_v_peek);
    GemVal _t926 = _t925.fn(_t925.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t926, gem_string("type")), gem_string(":")))) {
#line 517 "compiler/main.gem"
    GemVal _t927 = (*gem_v_advance);
                (void)(_t927.fn(_t927.env, NULL, 0));
#line 518 "compiler/main.gem"
    GemVal _t928 = (*gem_v_skip_nl);
                (void)(_t928.fn(_t928.env, NULL, 0));
#line 519 "compiler/main.gem"
    GemVal _t929[] = {gem_v_sub_target};
    GemVal _t930 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t930.fn(_t930.env, _t929, 1);
#line 520 "compiler/main.gem"
    GemVal _t931 = gem_v_sub;
    GemVal _t932 = gem_table_get(_t931, gem_string("condition"));
    GemVal _t935;
    if (gem_truthy(gem_neq(gem_table_get(_t932, gem_string("tag")), gem_string("bool")))) {
        _t935 = gem_neq(gem_table_get(_t932, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t933 = gem_v_sub;
        GemVal _t934 = gem_table_get(_t933, gem_string("condition"));
        _t935 = gem_neq(gem_table_get(_t934, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t935)) {
#line 521 "compiler/main.gem"
    GemVal _t936 = gem_v_sub;
    GemVal _t937[] = {gem_v_conditions, gem_table_get(_t936, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t937, 2));
                }
#line 523 "compiler/main.gem"
    GemVal _t938 = gem_v_sub;
                GemVal gem_v__for_items_2 = gem_table_get(_t938, gem_string("bindings"));
#line 523 "compiler/main.gem"
                GemVal gem_v__for_i_2 = gem_int(0);
#line 523 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t939[] = {gem_v__for_items_2};
                    if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t939, 1)))) break;
#line 523 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 523 "compiler/main.gem"
                    gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 524 "compiler/main.gem"
    GemVal _t940[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t940, 2));
                }

            } else {
#line 528 "compiler/main.gem"
    GemVal _t941[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t942[] = {gem_v_bindings, gem_fn_make_let(NULL, _t941, 3)};
                (void)(gem_push_fn(NULL, _t942, 2));
            }
#line 530 "compiler/main.gem"
    GemVal _t943 = (*gem_v_skip_nl);
            (void)(_t943.fn(_t943.env, NULL, 0));
#line 531 "compiler/main.gem"
    GemVal _t944 = (*gem_v_peek);
    GemVal _t945 = _t944.fn(_t944.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t945, gem_string("type")), gem_string(",")))) {
#line 532 "compiler/main.gem"
    GemVal _t946 = (*gem_v_advance);
                (void)(_t946.fn(_t946.env, NULL, 0));
#line 533 "compiler/main.gem"
    GemVal _t947 = (*gem_v_skip_nl);
                (void)(_t947.fn(_t947.env, NULL, 0));
            }
        }
#line 536 "compiler/main.gem"
    GemVal _t948[] = {gem_string("}")};
    GemVal _t949 = (*gem_v_expect);
        (void)(_t949.fn(_t949.env, _t948, 1));
#line 537 "compiler/main.gem"
    GemVal _t950 = gem_table_new();
    GemVal _t951[] = {gem_v_conditions};
    GemVal _t952 = gem_v_and_chain;
    gem_table_set(_t950, gem_string("condition"), _t952.fn(_t952.env, _t951, 1));
    gem_table_set(_t950, gem_string("bindings"), gem_v_bindings);
        GemVal _t953 = _t950;
        gem_pop_frame();
        return _t953;
    }
#line 541 "compiler/main.gem"
    GemVal _t954 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t954, gem_string("type")), gem_string("[")))) {
#line 542 "compiler/main.gem"
    GemVal _t955 = (*gem_v_advance);
        (void)(_t955.fn(_t955.env, NULL, 0));
#line 543 "compiler/main.gem"
    GemVal _t956 = (*gem_v_skip_nl);
        (void)(_t956.fn(_t956.env, NULL, 0));
#line 544 "compiler/main.gem"
    GemVal _t957 = gem_table_new();
        GemVal gem_v_sub_patterns = _t957;
#line 545 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 546 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t958 = (*gem_v_peek);
            GemVal _t959 = _t958.fn(_t958.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t959, gem_string("type")), gem_string("]")))) break;
#line 547 "compiler/main.gem"
    GemVal _t960[] = {gem_v_idx};
    GemVal _t961[] = {gem_v_target_expr, gem_fn_make_int(NULL, _t960, 1)};
    GemVal _t962[] = {gem_fn_make_index(NULL, _t961, 2)};
    GemVal _t963 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t963.fn(_t963.env, _t962, 1);
#line 548 "compiler/main.gem"
    GemVal _t964[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t964, 2));
#line 549 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t965 = (*gem_v_skip_nl);
            (void)(_t965.fn(_t965.env, NULL, 0));
#line 551 "compiler/main.gem"
    GemVal _t966 = (*gem_v_peek);
    GemVal _t967 = _t966.fn(_t966.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t967, gem_string("type")), gem_string(",")))) {
#line 552 "compiler/main.gem"
    GemVal _t968 = (*gem_v_advance);
                (void)(_t968.fn(_t968.env, NULL, 0));
#line 553 "compiler/main.gem"
    GemVal _t969 = (*gem_v_skip_nl);
                (void)(_t969.fn(_t969.env, NULL, 0));
            }
        }
#line 556 "compiler/main.gem"
    GemVal _t970[] = {gem_string("]")};
    GemVal _t971 = (*gem_v_expect);
        (void)(_t971.fn(_t971.env, _t970, 1));
#line 557 "compiler/main.gem"
    GemVal _t972 = gem_table_new();
    GemVal _t973[] = {gem_string("type")};
    GemVal _t974 = gem_table_new();
    gem_table_set(_t974, gem_int(0), gem_v_target_expr);
    GemVal _t975[] = {gem_fn_make_var(NULL, _t973, 1), _t974, gem_int(0)};
    GemVal _t976[] = {gem_string("table")};
    GemVal _t977[] = {gem_string("=="), gem_fn_make_call(NULL, _t975, 3), gem_fn_make_string(NULL, _t976, 1)};
    gem_table_set(_t972, gem_int(0), gem_fn_make_binop(NULL, _t977, 3));
        GemVal gem_v_conditions = _t972;
#line 558 "compiler/main.gem"
    GemVal _t978[] = {gem_string("len")};
    GemVal _t979 = gem_table_new();
    gem_table_set(_t979, gem_int(0), gem_v_target_expr);
    GemVal _t980[] = {gem_fn_make_var(NULL, _t978, 1), _t979, gem_int(0)};
    GemVal _t981[] = {gem_v_idx};
    GemVal _t982[] = {gem_string("=="), gem_fn_make_call(NULL, _t980, 3), gem_fn_make_int(NULL, _t981, 1)};
    GemVal _t983[] = {gem_v_conditions, gem_fn_make_binop(NULL, _t982, 3)};
        (void)(gem_push_fn(NULL, _t983, 2));
#line 559 "compiler/main.gem"
    GemVal _t984 = gem_table_new();
        GemVal gem_v_bindings = _t984;
#line 560 "compiler/main.gem"
        GemVal gem_v__for_i_4 = gem_int(0);
#line 560 "compiler/main.gem"
    GemVal _t985[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t985, 1);
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
    GemVal _t986 = gem_v_sub;
    GemVal _t987 = gem_table_get(_t986, gem_string("condition"));
    GemVal _t990;
    if (gem_truthy(gem_neq(gem_table_get(_t987, gem_string("tag")), gem_string("bool")))) {
        _t990 = gem_neq(gem_table_get(_t987, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t988 = gem_v_sub;
        GemVal _t989 = gem_table_get(_t988, gem_string("condition"));
        _t990 = gem_neq(gem_table_get(_t989, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t990)) {
#line 563 "compiler/main.gem"
    GemVal _t991 = gem_v_sub;
    GemVal _t992[] = {gem_v_conditions, gem_table_get(_t991, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t992, 2));
            }
#line 565 "compiler/main.gem"
    GemVal _t993 = gem_v_sub;
            GemVal gem_v__for_items_3 = gem_table_get(_t993, gem_string("bindings"));
#line 565 "compiler/main.gem"
            GemVal gem_v__for_i_3 = gem_int(0);
#line 565 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t994[] = {gem_v__for_items_3};
                if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t994, 1)))) break;
#line 565 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_3, gem_v__for_i_3);
#line 565 "compiler/main.gem"
                gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 566 "compiler/main.gem"
    GemVal _t995[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t995, 2));
            }

        }

#line 569 "compiler/main.gem"
    GemVal _t996 = gem_table_new();
    GemVal _t997[] = {gem_v_conditions};
    GemVal _t998 = gem_v_and_chain;
    gem_table_set(_t996, gem_string("condition"), _t998.fn(_t998.env, _t997, 1));
    gem_table_set(_t996, gem_string("bindings"), gem_v_bindings);
        GemVal _t999 = _t996;
        gem_pop_frame();
        return _t999;
    }
#line 573 "compiler/main.gem"
    GemVal _t1000 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1000, gem_string("type")), gem_string("NUMBER")))) {
#line 574 "compiler/main.gem"
    GemVal _t1001 = (*gem_v_advance);
        (void)(_t1001.fn(_t1001.env, NULL, 0));
#line 575 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 576 "compiler/main.gem"
    GemVal _t1002 = gem_v_t;
    GemVal _t1003[] = {gem_table_get(_t1002, gem_string("value"))};
        if (gem_truthy(gem_fn_contains_dot(NULL, _t1003, 1))) {
#line 577 "compiler/main.gem"
    GemVal _t1004 = gem_v_t;
    GemVal _t1005[] = {gem_table_get(_t1004, gem_string("value"))};
    GemVal _t1006[] = {gem_fn_atof(NULL, _t1005, 1)};
            gem_v_lit = gem_fn_make_float(NULL, _t1006, 1);
        } else {
#line 579 "compiler/main.gem"
    GemVal _t1007 = gem_v_t;
    GemVal _t1008[] = {gem_table_get(_t1007, gem_string("value"))};
    GemVal _t1009[] = {gem_fn_str_to_int(NULL, _t1008, 1)};
            gem_v_lit = gem_fn_make_int(NULL, _t1009, 1);
        }
#line 581 "compiler/main.gem"
    GemVal _t1010 = gem_table_new();
    GemVal _t1011[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    gem_table_set(_t1010, gem_string("condition"), gem_fn_make_binop(NULL, _t1011, 3));
    GemVal _t1012 = gem_table_new();
    gem_table_set(_t1010, gem_string("bindings"), _t1012);
        GemVal _t1013 = _t1010;
        gem_pop_frame();
        return _t1013;
    }
#line 583 "compiler/main.gem"
    GemVal _t1014 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1014, gem_string("type")), gem_string("STRING")))) {
#line 584 "compiler/main.gem"
    GemVal _t1015 = (*gem_v_advance);
        (void)(_t1015.fn(_t1015.env, NULL, 0));
#line 585 "compiler/main.gem"
    GemVal _t1016 = gem_table_new();
    GemVal _t1017 = gem_v_t;
    GemVal _t1018[] = {gem_table_get(_t1017, gem_string("value"))};
    GemVal _t1019[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_string(NULL, _t1018, 1)};
    gem_table_set(_t1016, gem_string("condition"), gem_fn_make_binop(NULL, _t1019, 3));
    GemVal _t1020 = gem_table_new();
    gem_table_set(_t1016, gem_string("bindings"), _t1020);
        GemVal _t1021 = _t1016;
        gem_pop_frame();
        return _t1021;
    }
#line 587 "compiler/main.gem"
    GemVal _t1022 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1022, gem_string("type")), gem_string("true")))) {
#line 588 "compiler/main.gem"
    GemVal _t1023 = (*gem_v_advance);
        (void)(_t1023.fn(_t1023.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t1024 = gem_table_new();
    GemVal _t1025[] = {gem_bool(1)};
    GemVal _t1026[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t1025, 1)};
    gem_table_set(_t1024, gem_string("condition"), gem_fn_make_binop(NULL, _t1026, 3));
    GemVal _t1027 = gem_table_new();
    gem_table_set(_t1024, gem_string("bindings"), _t1027);
        GemVal _t1028 = _t1024;
        gem_pop_frame();
        return _t1028;
    }
#line 591 "compiler/main.gem"
    GemVal _t1029 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1029, gem_string("type")), gem_string("false")))) {
#line 592 "compiler/main.gem"
    GemVal _t1030 = (*gem_v_advance);
        (void)(_t1030.fn(_t1030.env, NULL, 0));
#line 593 "compiler/main.gem"
    GemVal _t1031 = gem_table_new();
    GemVal _t1032[] = {gem_bool(0)};
    GemVal _t1033[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_bool(NULL, _t1032, 1)};
    gem_table_set(_t1031, gem_string("condition"), gem_fn_make_binop(NULL, _t1033, 3));
    GemVal _t1034 = gem_table_new();
    gem_table_set(_t1031, gem_string("bindings"), _t1034);
        GemVal _t1035 = _t1031;
        gem_pop_frame();
        return _t1035;
    }
#line 595 "compiler/main.gem"
    GemVal _t1036 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1036, gem_string("type")), gem_string("nil")))) {
#line 596 "compiler/main.gem"
    GemVal _t1037 = (*gem_v_advance);
        (void)(_t1037.fn(_t1037.env, NULL, 0));
#line 597 "compiler/main.gem"
    GemVal _t1038 = gem_table_new();
    GemVal _t1039[] = {gem_string("=="), gem_v_target_expr, gem_fn_make_nil_node(NULL, NULL, 0)};
    gem_table_set(_t1038, gem_string("condition"), gem_fn_make_binop(NULL, _t1039, 3));
    GemVal _t1040 = gem_table_new();
    gem_table_set(_t1038, gem_string("bindings"), _t1040);
        GemVal _t1041 = _t1038;
        gem_pop_frame();
        return _t1041;
    }
#line 601 "compiler/main.gem"
    GemVal _t1042 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1042, gem_string("type")), gem_string("NAME")))) {
#line 602 "compiler/main.gem"
    GemVal _t1043 = (*gem_v_advance);
        (void)(_t1043.fn(_t1043.env, NULL, 0));
#line 603 "compiler/main.gem"
    GemVal _t1044 = gem_table_new();
    GemVal _t1045[] = {gem_bool(1)};
    gem_table_set(_t1044, gem_string("condition"), gem_fn_make_bool(NULL, _t1045, 1));
    GemVal _t1046 = gem_table_new();
    GemVal _t1047 = gem_v_t;
    GemVal _t1048[] = {gem_table_get(_t1047, gem_string("value")), gem_v_target_expr, gem_int(0)};
    gem_table_set(_t1046, gem_int(0), gem_fn_make_let(NULL, _t1048, 3));
    gem_table_set(_t1044, gem_string("bindings"), _t1046);
        GemVal _t1049 = _t1044;
        gem_pop_frame();
        return _t1049;
    }
#line 606 "compiler/main.gem"
    GemVal _t1050 = gem_v_t;
    GemVal _t1051 = gem_v_t;
    GemVal _t1052 = gem_v_t;
    GemVal _t1053[] = {gem_table_get(_t1052, gem_string("value"))};
    GemVal _t1054 = gem_v_t;
    GemVal _t1055[] = {gem_table_get(_t1054, gem_string("type"))};
    GemVal _t1056[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1050, gem_string("line")), gem_table_get(_t1051, gem_string("col")), gem_len_fn(NULL, _t1053, 1), gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t1055, 1)), gem_string("'")), GEM_NIL};
    GemVal _t1057 = gem_fn_compile_error(NULL, _t1056, 7);
    gem_pop_frame();
    return _t1057;
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
    GemVal _t1059 = (*gem_v_peek);
    GemVal gem_v_t = _t1059.fn(_t1059.env, NULL, 0);
#line 615 "compiler/main.gem"
    GemVal _t1060 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1060, gem_string("type")), gem_string("let")))) {
#line 616 "compiler/main.gem"
    GemVal _t1061 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1061, gem_string("line"));
#line 617 "compiler/main.gem"
    GemVal _t1062 = (*gem_v_advance);
        (void)(_t1062.fn(_t1062.env, NULL, 0));
#line 620 "compiler/main.gem"
    GemVal _t1063 = (*gem_v_peek);
    GemVal _t1064 = _t1063.fn(_t1063.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1064, gem_string("type")), gem_string("{")))) {
#line 621 "compiler/main.gem"
    GemVal _t1065 = (*gem_v_advance);
            (void)(_t1065.fn(_t1065.env, NULL, 0));
#line 622 "compiler/main.gem"
    GemVal _t1066 = gem_table_new();
            GemVal gem_v_names = _t1066;
#line 623 "compiler/main.gem"
    GemVal _t1067 = (*gem_v_skip_nl);
            (void)(_t1067.fn(_t1067.env, NULL, 0));
#line 624 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1068 = (*gem_v_peek);
                GemVal _t1069 = _t1068.fn(_t1068.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1069, gem_string("type")), gem_string("}")))) break;
#line 625 "compiler/main.gem"
    GemVal _t1070 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1070.fn(_t1070.env, NULL, 0);
#line 626 "compiler/main.gem"
    GemVal _t1071 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t1071, gem_string("type")), gem_string("NAME")))) {
#line 627 "compiler/main.gem"
    GemVal _t1072 = (*gem_v_advance);
                    (void)(_t1072.fn(_t1072.env, NULL, 0));
                } else {
#line 630 "compiler/main.gem"
    GemVal _t1073 = (*gem_v_advance);
                    (void)(_t1073.fn(_t1073.env, NULL, 0));
                }
#line 632 "compiler/main.gem"
    GemVal _t1074 = gem_v_field_tok;
    GemVal _t1075[] = {gem_v_names, gem_table_get(_t1074, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1075, 2));
#line 633 "compiler/main.gem"
    GemVal _t1076 = (*gem_v_skip_nl);
                (void)(_t1076.fn(_t1076.env, NULL, 0));
#line 634 "compiler/main.gem"
    GemVal _t1077 = (*gem_v_peek);
    GemVal _t1078 = _t1077.fn(_t1077.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1078, gem_string("type")), gem_string(",")))) {
#line 635 "compiler/main.gem"
    GemVal _t1079 = (*gem_v_advance);
                    (void)(_t1079.fn(_t1079.env, NULL, 0));
#line 636 "compiler/main.gem"
    GemVal _t1080 = (*gem_v_skip_nl);
                    (void)(_t1080.fn(_t1080.env, NULL, 0));
                }
            }
#line 639 "compiler/main.gem"
    GemVal _t1081[] = {gem_string("}")};
    GemVal _t1082 = (*gem_v_expect);
            (void)(_t1082.fn(_t1082.env, _t1081, 1));
#line 640 "compiler/main.gem"
    GemVal _t1083[] = {gem_string("=")};
    GemVal _t1084 = (*gem_v_expect);
            (void)(_t1084.fn(_t1084.env, _t1083, 1));
#line 641 "compiler/main.gem"
    GemVal _t1085 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1085.fn(_t1085.env, NULL, 0);
#line 642 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 643 "compiler/main.gem"
    GemVal _t1086[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1086, 1));
#line 644 "compiler/main.gem"
    GemVal _t1087 = gem_table_new();
    GemVal _t1088[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1087, gem_int(0), gem_fn_make_let(NULL, _t1088, 3));
            GemVal gem_v_stmts = _t1087;
#line 645 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 645 "compiler/main.gem"
    GemVal _t1089[] = {gem_v_names};
            GemVal gem_v__for_limit_5 = gem_len_fn(NULL, _t1089, 1);
#line 645 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_limit_5))) break;
#line 645 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_5;
#line 645 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 646 "compiler/main.gem"
    GemVal _t1090[] = {gem_v_tmp};
    GemVal _t1091[] = {gem_fn_make_var(NULL, _t1090, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1092[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_dot(NULL, _t1091, 2), gem_v_line};
    GemVal _t1093[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1092, 3)};
                (void)(gem_push_fn(NULL, _t1093, 2));
            }

#line 648 "compiler/main.gem"
    GemVal _t1094 = gem_table_new();
    gem_table_set(_t1094, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1094, gem_string("stmts"), gem_v_stmts);
            GemVal _t1095 = _t1094;
            gem_pop_frame();
            return _t1095;
        }
#line 652 "compiler/main.gem"
    GemVal _t1096 = (*gem_v_peek);
    GemVal _t1097 = _t1096.fn(_t1096.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1097, gem_string("type")), gem_string("[")))) {
#line 653 "compiler/main.gem"
    GemVal _t1098 = (*gem_v_advance);
            (void)(_t1098.fn(_t1098.env, NULL, 0));
#line 654 "compiler/main.gem"
    GemVal _t1099 = gem_table_new();
            GemVal gem_v_names = _t1099;
#line 655 "compiler/main.gem"
    GemVal _t1100 = (*gem_v_skip_nl);
            (void)(_t1100.fn(_t1100.env, NULL, 0));
#line 656 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1101 = (*gem_v_peek);
                GemVal _t1102 = _t1101.fn(_t1101.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1102, gem_string("type")), gem_string("]")))) break;
#line 657 "compiler/main.gem"
    GemVal _t1103[] = {gem_string("NAME")};
    GemVal _t1104 = (*gem_v_expect);
    GemVal _t1105 = _t1104.fn(_t1104.env, _t1103, 1);
    GemVal _t1106[] = {gem_v_names, gem_table_get(_t1105, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1106, 2));
#line 658 "compiler/main.gem"
    GemVal _t1107 = (*gem_v_skip_nl);
                (void)(_t1107.fn(_t1107.env, NULL, 0));
#line 659 "compiler/main.gem"
    GemVal _t1108 = (*gem_v_peek);
    GemVal _t1109 = _t1108.fn(_t1108.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1109, gem_string("type")), gem_string(",")))) {
#line 660 "compiler/main.gem"
    GemVal _t1110 = (*gem_v_advance);
                    (void)(_t1110.fn(_t1110.env, NULL, 0));
#line 661 "compiler/main.gem"
    GemVal _t1111 = (*gem_v_skip_nl);
                    (void)(_t1111.fn(_t1111.env, NULL, 0));
                }
            }
#line 664 "compiler/main.gem"
    GemVal _t1112[] = {gem_string("]")};
    GemVal _t1113 = (*gem_v_expect);
            (void)(_t1113.fn(_t1113.env, _t1112, 1));
#line 665 "compiler/main.gem"
    GemVal _t1114[] = {gem_string("=")};
    GemVal _t1115 = (*gem_v_expect);
            (void)(_t1115.fn(_t1115.env, _t1114, 1));
#line 666 "compiler/main.gem"
    GemVal _t1116 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1116.fn(_t1116.env, NULL, 0);
#line 667 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 668 "compiler/main.gem"
    GemVal _t1117[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1117, 1));
#line 669 "compiler/main.gem"
    GemVal _t1118 = gem_table_new();
    GemVal _t1119[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    gem_table_set(_t1118, gem_int(0), gem_fn_make_let(NULL, _t1119, 3));
            GemVal gem_v_stmts = _t1118;
#line 670 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 670 "compiler/main.gem"
    GemVal _t1120[] = {gem_v_names};
            GemVal gem_v__for_limit_6 = gem_len_fn(NULL, _t1120, 1);
#line 670 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_limit_6))) break;
#line 670 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_6;
#line 670 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t1121[] = {gem_v_tmp};
    GemVal _t1122[] = {gem_v_di};
    GemVal _t1123[] = {gem_fn_make_var(NULL, _t1121, 1), gem_fn_make_int(NULL, _t1122, 1)};
    GemVal _t1124[] = {gem_table_get(gem_v_names, gem_v_di), gem_fn_make_index(NULL, _t1123, 2), gem_v_line};
    GemVal _t1125[] = {gem_v_stmts, gem_fn_make_let(NULL, _t1124, 3)};
                (void)(gem_push_fn(NULL, _t1125, 2));
            }

#line 673 "compiler/main.gem"
    GemVal _t1126 = gem_table_new();
    gem_table_set(_t1126, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1126, gem_string("stmts"), gem_v_stmts);
            GemVal _t1127 = _t1126;
            gem_pop_frame();
            return _t1127;
        }
#line 676 "compiler/main.gem"
    GemVal _t1128[] = {gem_string("NAME")};
    GemVal _t1129 = (*gem_v_expect);
    GemVal _t1130 = _t1129.fn(_t1129.env, _t1128, 1);
        GemVal gem_v_name = gem_table_get(_t1130, gem_string("value"));
#line 677 "compiler/main.gem"
    GemVal _t1131[] = {gem_string("=")};
    GemVal _t1132 = (*gem_v_expect);
        (void)(_t1132.fn(_t1132.env, _t1131, 1));
#line 678 "compiler/main.gem"
    GemVal _t1133 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1133.fn(_t1133.env, NULL, 0);
#line 679 "compiler/main.gem"
    GemVal _t1134[] = {gem_v_name, gem_v_value, gem_v_line};
        GemVal _t1135 = gem_fn_make_let(NULL, _t1134, 3);
        gem_pop_frame();
        return _t1135;
    }
#line 683 "compiler/main.gem"
    GemVal _t1136 = gem_v_t;
    GemVal _t1140;
    if (!gem_truthy(gem_eq(gem_table_get(_t1136, gem_string("type")), gem_string("fn")))) {
        _t1140 = gem_eq(gem_table_get(_t1136, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1137[] = {gem_int(1)};
        GemVal _t1138 = (*gem_v_peek_at);
        GemVal _t1139 = _t1138.fn(_t1138.env, _t1137, 1);
        _t1140 = gem_eq(gem_table_get(_t1139, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1140)) {
#line 684 "compiler/main.gem"
    GemVal _t1141 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1141, gem_string("line"));
#line 685 "compiler/main.gem"
    GemVal _t1142 = (*gem_v_advance);
        (void)(_t1142.fn(_t1142.env, NULL, 0));
#line 686 "compiler/main.gem"
    GemVal _t1143[] = {gem_string("NAME")};
    GemVal _t1144 = (*gem_v_expect);
    GemVal _t1145 = _t1144.fn(_t1144.env, _t1143, 1);
        GemVal gem_v_name = gem_table_get(_t1145, gem_string("value"));
#line 687 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 688 "compiler/main.gem"
    GemVal _t1146[] = {gem_v_name};
    GemVal _t1147[] = {gem_v_name};
    GemVal _t1148[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1146, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_add(gem_add(gem_string("use: let "), gem_to_string_fn(NULL, _t1147, 1)), gem_string(" = fn(...) ... end"))};
            (void)(gem_fn_compile_error(NULL, _t1148, 7));
        }
#line 690 "compiler/main.gem"
    GemVal _t1149[] = {gem_string("(")};
    GemVal _t1150 = (*gem_v_expect);
        (void)(_t1150.fn(_t1150.env, _t1149, 1));
#line 691 "compiler/main.gem"
    GemVal _t1151 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1151.fn(_t1151.env, NULL, 0);
#line 692 "compiler/main.gem"
    GemVal _t1152[] = {gem_string(")")};
    GemVal _t1153 = (*gem_v_expect);
        (void)(_t1153.fn(_t1153.env, _t1152, 1));
#line 693 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 694 "compiler/main.gem"
    GemVal _t1154 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1154.fn(_t1154.env, NULL, 0);
#line 695 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 696 "compiler/main.gem"
    GemVal _t1155[] = {gem_string("end")};
    GemVal _t1156 = (*gem_v_expect);
        (void)(_t1156.fn(_t1156.env, _t1155, 1));
#line 697 "compiler/main.gem"
    GemVal _t1157 = gem_v_pr;
    GemVal _t1158 = gem_v_pr;
    GemVal _t1159 = gem_v_pr;
    GemVal _t1160[] = {gem_v_name, gem_table_get(_t1157, gem_string("params")), gem_table_get(_t1158, gem_string("rest_param")), gem_table_get(_t1159, gem_string("block_param")), gem_v_body, gem_v_line};
        GemVal _t1161 = gem_fn_make_fn_def(NULL, _t1160, 6);
        gem_pop_frame();
        return _t1161;
    }
#line 701 "compiler/main.gem"
    GemVal _t1162 = gem_v_t;
    GemVal _t1164;
    if (gem_truthy(gem_eq(gem_table_get(_t1162, gem_string("type")), gem_string("if")))) {
        _t1164 = gem_eq(gem_table_get(_t1162, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1163 = gem_v_t;
        _t1164 = gem_eq(gem_table_get(_t1163, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1164)) {
#line 702 "compiler/main.gem"
    GemVal _t1165 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1165, gem_string("line"));
#line 703 "compiler/main.gem"
    GemVal _t1166 = (*gem_v_advance);
        (void)(_t1166.fn(_t1166.env, NULL, 0));
#line 704 "compiler/main.gem"
    GemVal _t1167 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1167.fn(_t1167.env, NULL, 0);
#line 705 "compiler/main.gem"
    GemVal _t1168 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1168.fn(_t1168.env, NULL, 0);
#line 706 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 707 "compiler/main.gem"
    GemVal _t1169 = (*gem_v_peek);
    GemVal _t1170 = _t1169.fn(_t1169.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1170, gem_string("type")), gem_string("elif")))) {
#line 709 "compiler/main.gem"
    GemVal _t1171 = gem_table_new();
    GemVal _t1172 = (*gem_v_parse_stmt);
    gem_table_set(_t1171, gem_int(0), _t1172.fn(_t1172.env, NULL, 0));
            gem_v_else_body = _t1171;
        } else {
#line 710 "compiler/main.gem"
    GemVal _t1173 = (*gem_v_peek);
    GemVal _t1174 = _t1173.fn(_t1173.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1174, gem_string("type")), gem_string("else")))) {
#line 711 "compiler/main.gem"
    GemVal _t1175 = (*gem_v_advance);
                (void)(_t1175.fn(_t1175.env, NULL, 0));
#line 712 "compiler/main.gem"
    GemVal _t1176 = (*gem_v_parse_body);
                gem_v_else_body = _t1176.fn(_t1176.env, NULL, 0);
            }
        }
#line 715 "compiler/main.gem"
    GemVal _t1177 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1177, gem_string("type")), gem_string("if")))) {
#line 716 "compiler/main.gem"
    GemVal _t1178[] = {gem_string("end")};
    GemVal _t1179 = (*gem_v_expect);
            (void)(_t1179.fn(_t1179.env, _t1178, 1));
        }
#line 718 "compiler/main.gem"
    GemVal _t1180[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
        GemVal _t1181 = gem_fn_make_if(NULL, _t1180, 4);
        gem_pop_frame();
        return _t1181;
    }
#line 722 "compiler/main.gem"
    GemVal _t1182 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1182, gem_string("type")), gem_string("while")))) {
#line 723 "compiler/main.gem"
    GemVal _t1183 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1183, gem_string("line"));
#line 724 "compiler/main.gem"
    GemVal _t1184 = (*gem_v_advance);
        (void)(_t1184.fn(_t1184.env, NULL, 0));
#line 725 "compiler/main.gem"
    GemVal _t1185 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1185.fn(_t1185.env, NULL, 0);
#line 726 "compiler/main.gem"
    GemVal _t1186 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1186.fn(_t1186.env, NULL, 0);
#line 727 "compiler/main.gem"
    GemVal _t1187[] = {gem_string("end")};
    GemVal _t1188 = (*gem_v_expect);
        (void)(_t1188.fn(_t1188.env, _t1187, 1));
#line 728 "compiler/main.gem"
    GemVal _t1189[] = {gem_v_cond, gem_v_wbody, gem_v_line};
        GemVal _t1190 = gem_fn_make_while(NULL, _t1189, 3);
        gem_pop_frame();
        return _t1190;
    }
#line 732 "compiler/main.gem"
    GemVal _t1191 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1191, gem_string("type")), gem_string("for")))) {
#line 733 "compiler/main.gem"
    GemVal _t1192 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1192, gem_string("line"));
#line 734 "compiler/main.gem"
    GemVal _t1193 = (*gem_v_advance);
        (void)(_t1193.fn(_t1193.env, NULL, 0));
#line 735 "compiler/main.gem"
    GemVal _t1194[] = {gem_string("NAME")};
    GemVal _t1195 = (*gem_v_expect);
    GemVal _t1196 = _t1195.fn(_t1195.env, _t1194, 1);
        GemVal gem_v_var_name = gem_table_get(_t1196, gem_string("value"));
#line 737 "compiler/main.gem"
    GemVal _t1197 = (*gem_v_peek);
    GemVal _t1198 = _t1197.fn(_t1197.env, NULL, 0);
    GemVal _t1202;
    if (!gem_truthy(gem_eq(gem_table_get(_t1198, gem_string("type")), gem_string(",")))) {
        _t1202 = gem_eq(gem_table_get(_t1198, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1199[] = {gem_int(1)};
        GemVal _t1200 = (*gem_v_peek_at);
        GemVal _t1201 = _t1200.fn(_t1200.env, _t1199, 1);
        _t1202 = gem_eq(gem_table_get(_t1201, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1202)) {
#line 739 "compiler/main.gem"
    GemVal _t1203 = (*gem_v_advance);
            (void)(_t1203.fn(_t1203.env, NULL, 0));
#line 740 "compiler/main.gem"
    GemVal _t1204[] = {gem_string("NAME")};
    GemVal _t1205 = (*gem_v_expect);
    GemVal _t1206 = _t1205.fn(_t1205.env, _t1204, 1);
            GemVal gem_v_val_name = gem_table_get(_t1206, gem_string("value"));
#line 741 "compiler/main.gem"
    GemVal _t1207[] = {gem_string("in")};
    GemVal _t1208 = (*gem_v_expect);
            (void)(_t1208.fn(_t1208.env, _t1207, 1));
#line 742 "compiler/main.gem"
    GemVal _t1209 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1209.fn(_t1209.env, NULL, 0);
#line 743 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 744 "compiler/main.gem"
    GemVal _t1210[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1210, 1));
#line 745 "compiler/main.gem"
    GemVal _t1211[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t1211, 1));
#line 746 "compiler/main.gem"
    GemVal _t1212[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1212, 1));
#line 747 "compiler/main.gem"
    GemVal _t1213 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1213.fn(_t1213.env, NULL, 0);
#line 748 "compiler/main.gem"
    GemVal _t1214[] = {gem_string("end")};
    GemVal _t1215 = (*gem_v_expect);
            (void)(_t1215.fn(_t1215.env, _t1214, 1));
#line 749 "compiler/main.gem"
    GemVal _t1216 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1216;
#line 750 "compiler/main.gem"
    GemVal _t1217[] = {gem_v_keys_var};
    GemVal _t1218[] = {gem_v_idx_var};
    GemVal _t1219[] = {gem_fn_make_var(NULL, _t1217, 1), gem_fn_make_var(NULL, _t1218, 1)};
    GemVal _t1220[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1219, 2), gem_v_line};
    GemVal _t1221[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1220, 3)};
            (void)(gem_push_fn(NULL, _t1221, 2));
#line 751 "compiler/main.gem"
    GemVal _t1222[] = {gem_v_tbl_var};
    GemVal _t1223[] = {gem_v_keys_var};
    GemVal _t1224[] = {gem_v_idx_var};
    GemVal _t1225[] = {gem_fn_make_var(NULL, _t1223, 1), gem_fn_make_var(NULL, _t1224, 1)};
    GemVal _t1226[] = {gem_fn_make_var(NULL, _t1222, 1), gem_fn_make_index(NULL, _t1225, 2)};
    GemVal _t1227[] = {gem_v_val_name, gem_fn_make_index(NULL, _t1226, 2), gem_v_line};
    GemVal _t1228[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1227, 3)};
            (void)(gem_push_fn(NULL, _t1228, 2));
#line 752 "compiler/main.gem"
    GemVal _t1229[] = {gem_v_idx_var};
    GemVal _t1230[] = {gem_int(1)};
    GemVal _t1231[] = {gem_string("+"), gem_fn_make_var(NULL, _t1229, 1), gem_fn_make_int(NULL, _t1230, 1)};
    GemVal _t1232[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1231, 3), gem_v_line};
    GemVal _t1233[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1232, 3)};
            (void)(gem_push_fn(NULL, _t1233, 2));
#line 753 "compiler/main.gem"
            GemVal gem_v__for_i_7 = gem_int(0);
#line 753 "compiler/main.gem"
    GemVal _t1234[] = {gem_v_fbody};
            GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1234, 1);
#line 753 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 753 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_7;
#line 753 "compiler/main.gem"
                gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 754 "compiler/main.gem"
    GemVal _t1235[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1235, 2));
            }

#line 756 "compiler/main.gem"
    GemVal _t1236[] = {gem_v_idx_var};
    GemVal _t1237[] = {gem_string("len")};
    GemVal _t1238 = gem_table_new();
    GemVal _t1239[] = {gem_v_keys_var};
    gem_table_set(_t1238, gem_int(0), gem_fn_make_var(NULL, _t1239, 1));
    GemVal _t1240[] = {gem_fn_make_var(NULL, _t1237, 1), _t1238, gem_int(0)};
    GemVal _t1241[] = {gem_string("<"), gem_fn_make_var(NULL, _t1236, 1), gem_fn_make_call(NULL, _t1240, 3)};
    GemVal _t1242[] = {gem_fn_make_binop(NULL, _t1241, 3), gem_v_inner_stmts, gem_v_line};
            GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1242, 3);
#line 761 "compiler/main.gem"
    GemVal _t1243 = gem_table_new();
    gem_table_set(_t1243, gem_string("tag"), gem_string("block"));
    GemVal _t1244 = gem_table_new();
    GemVal _t1245[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    gem_table_set(_t1244, gem_int(0), gem_fn_make_let(NULL, _t1245, 3));
    GemVal _t1246[] = {gem_string("keys")};
    GemVal _t1247 = gem_table_new();
    GemVal _t1248[] = {gem_v_tbl_var};
    gem_table_set(_t1247, gem_int(0), gem_fn_make_var(NULL, _t1248, 1));
    GemVal _t1249[] = {gem_fn_make_var(NULL, _t1246, 1), _t1247, gem_int(0)};
    GemVal _t1250[] = {gem_v_keys_var, gem_fn_make_call(NULL, _t1249, 3), gem_v_line};
    gem_table_set(_t1244, gem_int(1), gem_fn_make_let(NULL, _t1250, 3));
    GemVal _t1251[] = {gem_int(0)};
    GemVal _t1252[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1251, 1), gem_v_line};
    gem_table_set(_t1244, gem_int(2), gem_fn_make_let(NULL, _t1252, 3));
    gem_table_set(_t1244, gem_int(3), gem_v_while_node);
    gem_table_set(_t1243, gem_string("stmts"), _t1244);
            GemVal _t1253 = _t1243;
            gem_pop_frame();
            return _t1253;
        } else {
#line 767 "compiler/main.gem"
    GemVal _t1254 = (*gem_v_peek);
    GemVal _t1255 = _t1254.fn(_t1254.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1255, gem_string("type")), gem_string("in")))) {
#line 769 "compiler/main.gem"
    GemVal _t1256 = (*gem_v_advance);
                (void)(_t1256.fn(_t1256.env, NULL, 0));
#line 770 "compiler/main.gem"
    GemVal _t1257 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1257.fn(_t1257.env, NULL, 0);
#line 771 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 772 "compiler/main.gem"
    GemVal _t1258[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1258, 1));
#line 773 "compiler/main.gem"
    GemVal _t1259[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1259, 1));
#line 774 "compiler/main.gem"
    GemVal _t1260 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1260.fn(_t1260.env, NULL, 0);
#line 775 "compiler/main.gem"
    GemVal _t1261[] = {gem_string("end")};
    GemVal _t1262 = (*gem_v_expect);
                (void)(_t1262.fn(_t1262.env, _t1261, 1));
#line 776 "compiler/main.gem"
    GemVal _t1263 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1263;
#line 777 "compiler/main.gem"
    GemVal _t1264[] = {gem_v_items_var};
    GemVal _t1265[] = {gem_v_idx_var};
    GemVal _t1266[] = {gem_fn_make_var(NULL, _t1264, 1), gem_fn_make_var(NULL, _t1265, 1)};
    GemVal _t1267[] = {gem_v_var_name, gem_fn_make_index(NULL, _t1266, 2), gem_v_line};
    GemVal _t1268[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1267, 3)};
                (void)(gem_push_fn(NULL, _t1268, 2));
#line 778 "compiler/main.gem"
    GemVal _t1269[] = {gem_v_idx_var};
    GemVal _t1270[] = {gem_int(1)};
    GemVal _t1271[] = {gem_string("+"), gem_fn_make_var(NULL, _t1269, 1), gem_fn_make_int(NULL, _t1270, 1)};
    GemVal _t1272[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1271, 3), gem_v_line};
    GemVal _t1273[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1272, 3)};
                (void)(gem_push_fn(NULL, _t1273, 2));
#line 779 "compiler/main.gem"
                GemVal gem_v__for_i_8 = gem_int(0);
#line 779 "compiler/main.gem"
    GemVal _t1274[] = {gem_v_fbody};
                GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1274, 1);
#line 779 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 779 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_8;
#line 779 "compiler/main.gem"
                    gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 780 "compiler/main.gem"
    GemVal _t1275[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1275, 2));
                }

#line 782 "compiler/main.gem"
    GemVal _t1276[] = {gem_v_idx_var};
    GemVal _t1277[] = {gem_string("len")};
    GemVal _t1278 = gem_table_new();
    GemVal _t1279[] = {gem_v_items_var};
    gem_table_set(_t1278, gem_int(0), gem_fn_make_var(NULL, _t1279, 1));
    GemVal _t1280[] = {gem_fn_make_var(NULL, _t1277, 1), _t1278, gem_int(0)};
    GemVal _t1281[] = {gem_string("<"), gem_fn_make_var(NULL, _t1276, 1), gem_fn_make_call(NULL, _t1280, 3)};
    GemVal _t1282[] = {gem_fn_make_binop(NULL, _t1281, 3), gem_v_inner_stmts, gem_v_line};
                GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1282, 3);
#line 787 "compiler/main.gem"
    GemVal _t1283 = gem_table_new();
    gem_table_set(_t1283, gem_string("tag"), gem_string("block"));
    GemVal _t1284 = gem_table_new();
    GemVal _t1285[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    gem_table_set(_t1284, gem_int(0), gem_fn_make_let(NULL, _t1285, 3));
    GemVal _t1286[] = {gem_int(0)};
    GemVal _t1287[] = {gem_v_idx_var, gem_fn_make_int(NULL, _t1286, 1), gem_v_line};
    gem_table_set(_t1284, gem_int(1), gem_fn_make_let(NULL, _t1287, 3));
    gem_table_set(_t1284, gem_int(2), gem_v_while_node);
    gem_table_set(_t1283, gem_string("stmts"), _t1284);
                GemVal _t1288 = _t1283;
                gem_pop_frame();
                return _t1288;
            } else {
#line 792 "compiler/main.gem"
    GemVal _t1289 = (*gem_v_peek);
    GemVal _t1290 = _t1289.fn(_t1289.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1290, gem_string("type")), gem_string("=")))) {
#line 794 "compiler/main.gem"
    GemVal _t1291 = (*gem_v_advance);
                    (void)(_t1291.fn(_t1291.env, NULL, 0));
#line 795 "compiler/main.gem"
    GemVal _t1292 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1292.fn(_t1292.env, NULL, 0);
#line 796 "compiler/main.gem"
    GemVal _t1293[] = {gem_string(",")};
    GemVal _t1294 = (*gem_v_expect);
                    (void)(_t1294.fn(_t1294.env, _t1293, 1));
#line 797 "compiler/main.gem"
    GemVal _t1295 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1295.fn(_t1295.env, NULL, 0);
#line 798 "compiler/main.gem"
    GemVal _t1296 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1296.fn(_t1296.env, NULL, 0);
#line 799 "compiler/main.gem"
    GemVal _t1297[] = {gem_string("end")};
    GemVal _t1298 = (*gem_v_expect);
                    (void)(_t1298.fn(_t1298.env, _t1297, 1));
#line 800 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 801 "compiler/main.gem"
    GemVal _t1299[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1299, 1));
#line 802 "compiler/main.gem"
    GemVal _t1300[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1300, 1));
#line 803 "compiler/main.gem"
    GemVal _t1301 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1301;
#line 804 "compiler/main.gem"
    GemVal _t1302[] = {gem_v_idx_var};
    GemVal _t1303[] = {gem_v_var_name, gem_fn_make_var(NULL, _t1302, 1), gem_v_line};
    GemVal _t1304[] = {gem_v_inner_stmts, gem_fn_make_let(NULL, _t1303, 3)};
                    (void)(gem_push_fn(NULL, _t1304, 2));
#line 805 "compiler/main.gem"
    GemVal _t1305[] = {gem_v_idx_var};
    GemVal _t1306[] = {gem_int(1)};
    GemVal _t1307[] = {gem_string("+"), gem_fn_make_var(NULL, _t1305, 1), gem_fn_make_int(NULL, _t1306, 1)};
    GemVal _t1308[] = {gem_v_idx_var, gem_fn_make_binop(NULL, _t1307, 3), gem_v_line};
    GemVal _t1309[] = {gem_v_inner_stmts, gem_fn_make_assign(NULL, _t1308, 3)};
                    (void)(gem_push_fn(NULL, _t1309, 2));
#line 806 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 806 "compiler/main.gem"
    GemVal _t1310[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1310, 1);
#line 806 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 806 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_9;
#line 806 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 807 "compiler/main.gem"
    GemVal _t1311[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1311, 2));
                    }

#line 809 "compiler/main.gem"
    GemVal _t1312[] = {gem_v_idx_var};
    GemVal _t1313[] = {gem_v_limit_var};
    GemVal _t1314[] = {gem_string("<"), gem_fn_make_var(NULL, _t1312, 1), gem_fn_make_var(NULL, _t1313, 1)};
    GemVal _t1315[] = {gem_fn_make_binop(NULL, _t1314, 3), gem_v_inner_stmts, gem_v_line};
                    GemVal gem_v_while_node = gem_fn_make_while(NULL, _t1315, 3);
#line 814 "compiler/main.gem"
    GemVal _t1316 = gem_table_new();
    gem_table_set(_t1316, gem_string("tag"), gem_string("block"));
    GemVal _t1317 = gem_table_new();
    GemVal _t1318[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    gem_table_set(_t1317, gem_int(0), gem_fn_make_let(NULL, _t1318, 3));
    GemVal _t1319[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    gem_table_set(_t1317, gem_int(1), gem_fn_make_let(NULL, _t1319, 3));
    gem_table_set(_t1317, gem_int(2), gem_v_while_node);
    gem_table_set(_t1316, gem_string("stmts"), _t1317);
                    GemVal _t1320 = _t1316;
                    gem_pop_frame();
                    return _t1320;
                } else {
#line 820 "compiler/main.gem"
    GemVal _t1321 = (*gem_v_peek);
    GemVal _t1322 = _t1321.fn(_t1321.env, NULL, 0);
    GemVal _t1323 = (*gem_v_peek);
    GemVal _t1324 = _t1323.fn(_t1323.env, NULL, 0);
    GemVal _t1325 = (*gem_v_peek);
    GemVal _t1326 = _t1325.fn(_t1325.env, NULL, 0);
    GemVal _t1327[] = {gem_table_get(_t1326, gem_string("value"))};
    GemVal _t1328[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1322, gem_string("line")), gem_table_get(_t1324, gem_string("col")), gem_len_fn(NULL, _t1327, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
                    (void)(gem_fn_compile_error(NULL, _t1328, 7));
                }
            }
        }
    }
#line 825 "compiler/main.gem"
    GemVal _t1329 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1329, gem_string("type")), gem_string("match")))) {
#line 826 "compiler/main.gem"
    GemVal _t1330 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1330, gem_string("line"));
#line 827 "compiler/main.gem"
    GemVal _t1331 = (*gem_v_advance);
        (void)(_t1331.fn(_t1331.env, NULL, 0));
#line 828 "compiler/main.gem"
    GemVal _t1332 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1332.fn(_t1332.env, NULL, 0);
#line 829 "compiler/main.gem"
    GemVal _t1333 = (*gem_v_skip_nl);
        (void)(_t1333.fn(_t1333.env, NULL, 0));
#line 830 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 831 "compiler/main.gem"
    GemVal _t1334[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1334, 1));
#line 832 "compiler/main.gem"
    GemVal _t1335[] = {gem_v_match_var};
        GemVal gem_v_target_var_expr = gem_fn_make_var(NULL, _t1335, 1);
#line 833 "compiler/main.gem"
    GemVal _t1336 = gem_table_new();
        GemVal gem_v_whens = _t1336;
#line 834 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1337 = (*gem_v_peek);
            GemVal _t1338 = _t1337.fn(_t1337.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1338, gem_string("type")), gem_string("when")))) break;
#line 835 "compiler/main.gem"
    GemVal _t1339 = (*gem_v_advance);
            (void)(_t1339.fn(_t1339.env, NULL, 0));
#line 837 "compiler/main.gem"
    GemVal _t1340 = (*gem_v_peek);
    GemVal _t1341 = _t1340.fn(_t1340.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1341, gem_string("type"));
#line 838 "compiler/main.gem"
    GemVal _t1342[] = {gem_int(1)};
    GemVal _t1343 = (*gem_v_peek_at);
    GemVal _t1344 = _t1343.fn(_t1343.env, _t1342, 1);
            GemVal gem_v_next = gem_table_get(_t1344, gem_string("type"));
#line 839 "compiler/main.gem"
    GemVal _t1345;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1345 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1345 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1345)) {
#line 840 "compiler/main.gem"
    GemVal _t1346[] = {gem_v_target_var_expr};
    GemVal _t1347 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1347.fn(_t1347.env, _t1346, 1);
#line 841 "compiler/main.gem"
    GemVal _t1348 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1348.fn(_t1348.env, NULL, 0);
#line 842 "compiler/main.gem"
    GemVal _t1349 = gem_v_pat;
    GemVal _t1350 = gem_v_pat;
    GemVal _t1351[] = {gem_table_get(_t1349, gem_string("condition")), gem_v_wbody, gem_table_get(_t1350, gem_string("bindings"))};
    GemVal _t1352[] = {gem_v_whens, gem_fn_make_when(NULL, _t1351, 3)};
                (void)(gem_push_fn(NULL, _t1352, 2));
            } else {
#line 843 "compiler/main.gem"
    GemVal _t1357;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1357 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1353;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1353 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1353 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1354;
        if (gem_truthy(_t1353)) {
                _t1354 = _t1353;
        } else {
                _t1354 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1355;
        if (gem_truthy(_t1354)) {
                _t1355 = _t1354;
        } else {
                _t1355 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1356;
        if (gem_truthy(_t1355)) {
                _t1356 = _t1355;
        } else {
                _t1356 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1357 = _t1356;
    }
                if (gem_truthy(_t1357)) {
#line 845 "compiler/main.gem"
    GemVal _t1358[] = {gem_v_target_var_expr};
    GemVal _t1359 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1359.fn(_t1359.env, _t1358, 1);
#line 846 "compiler/main.gem"
    GemVal _t1360 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1360.fn(_t1360.env, NULL, 0);
#line 847 "compiler/main.gem"
    GemVal _t1361 = gem_v_pat;
    GemVal _t1362 = gem_v_pat;
    GemVal _t1363[] = {gem_table_get(_t1361, gem_string("condition")), gem_v_wbody, gem_table_get(_t1362, gem_string("bindings"))};
    GemVal _t1364[] = {gem_v_whens, gem_fn_make_when(NULL, _t1363, 3)};
                    (void)(gem_push_fn(NULL, _t1364, 2));
                } else {
#line 850 "compiler/main.gem"
    GemVal _t1365 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1365.fn(_t1365.env, NULL, 0);
#line 851 "compiler/main.gem"
    GemVal _t1366 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1366.fn(_t1366.env, NULL, 0);
#line 852 "compiler/main.gem"
    GemVal _t1367[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1368 = gem_table_new();
    GemVal _t1369[] = {gem_fn_make_binop(NULL, _t1367, 3), gem_v_wbody, _t1368};
    GemVal _t1370[] = {gem_v_whens, gem_fn_make_when(NULL, _t1369, 3)};
                    (void)(gem_push_fn(NULL, _t1370, 2));
                }
            }
#line 854 "compiler/main.gem"
    GemVal _t1371 = (*gem_v_skip_nl);
            (void)(_t1371.fn(_t1371.env, NULL, 0));
        }
#line 856 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 857 "compiler/main.gem"
    GemVal _t1372 = (*gem_v_peek);
    GemVal _t1373 = _t1372.fn(_t1372.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1373, gem_string("type")), gem_string("else")))) {
#line 858 "compiler/main.gem"
    GemVal _t1374 = (*gem_v_advance);
            (void)(_t1374.fn(_t1374.env, NULL, 0));
#line 859 "compiler/main.gem"
    GemVal _t1375 = (*gem_v_parse_body);
            gem_v_else_body = _t1375.fn(_t1375.env, NULL, 0);
        }
#line 861 "compiler/main.gem"
    GemVal _t1376[] = {gem_string("end")};
    GemVal _t1377 = (*gem_v_expect);
        (void)(_t1377.fn(_t1377.env, _t1376, 1));
#line 862 "compiler/main.gem"
    GemVal _t1378[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
        GemVal _t1379 = gem_fn_make_match(NULL, _t1378, 5);
        gem_pop_frame();
        return _t1379;
    }
#line 866 "compiler/main.gem"
    GemVal _t1380 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1380, gem_string("type")), gem_string("return")))) {
#line 867 "compiler/main.gem"
    GemVal _t1381 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1381, gem_string("line"));
#line 868 "compiler/main.gem"
    GemVal _t1382 = (*gem_v_advance);
        (void)(_t1382.fn(_t1382.env, NULL, 0));
#line 869 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 870 "compiler/main.gem"
    GemVal _t1383 = (*gem_v_peek);
    GemVal _t1384 = _t1383.fn(_t1383.env, NULL, 0);
    GemVal _t1387;
    if (!gem_truthy(gem_neq(gem_table_get(_t1384, gem_string("type")), gem_string("NEWLINE")))) {
        _t1387 = gem_neq(gem_table_get(_t1384, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1385 = (*gem_v_peek);
        GemVal _t1386 = _t1385.fn(_t1385.env, NULL, 0);
        _t1387 = gem_neq(gem_table_get(_t1386, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1390;
    if (!gem_truthy(_t1387)) {
        _t1390 = _t1387;
    } else {
        GemVal _t1388 = (*gem_v_peek);
        GemVal _t1389 = _t1388.fn(_t1388.env, NULL, 0);
        _t1390 = gem_neq(gem_table_get(_t1389, gem_string("type")), gem_string("end"));
    }
    GemVal _t1393;
    if (!gem_truthy(_t1390)) {
        _t1393 = _t1390;
    } else {
        GemVal _t1391 = (*gem_v_peek);
        GemVal _t1392 = _t1391.fn(_t1391.env, NULL, 0);
        _t1393 = gem_neq(gem_table_get(_t1392, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1396;
    if (!gem_truthy(_t1393)) {
        _t1396 = _t1393;
    } else {
        GemVal _t1394 = (*gem_v_peek);
        GemVal _t1395 = _t1394.fn(_t1394.env, NULL, 0);
        _t1396 = gem_neq(gem_table_get(_t1395, gem_string("type")), gem_string("else"));
    }
    GemVal _t1399;
    if (!gem_truthy(_t1396)) {
        _t1399 = _t1396;
    } else {
        GemVal _t1397 = (*gem_v_peek);
        GemVal _t1398 = _t1397.fn(_t1397.env, NULL, 0);
        _t1399 = gem_neq(gem_table_get(_t1398, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1399)) {
#line 871 "compiler/main.gem"
    GemVal _t1400 = (*gem_v_parse_expr);
            gem_v_value = _t1400.fn(_t1400.env, NULL, 0);
        }
#line 873 "compiler/main.gem"
    GemVal _t1401[] = {gem_v_value, gem_v_line};
        GemVal _t1402 = gem_fn_make_return(NULL, _t1401, 2);
        gem_pop_frame();
        return _t1402;
    }
#line 877 "compiler/main.gem"
    GemVal _t1403 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1403, gem_string("type")), gem_string("break")))) {
#line 878 "compiler/main.gem"
    GemVal _t1404 = (*gem_v_advance);
        (void)(_t1404.fn(_t1404.env, NULL, 0));
#line 879 "compiler/main.gem"
        GemVal _t1405 = gem_fn_make_break(NULL, NULL, 0);
        gem_pop_frame();
        return _t1405;
    }
#line 883 "compiler/main.gem"
    GemVal _t1406 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1406, gem_string("type")), gem_string("continue")))) {
#line 884 "compiler/main.gem"
    GemVal _t1407 = (*gem_v_advance);
        (void)(_t1407.fn(_t1407.env, NULL, 0));
#line 885 "compiler/main.gem"
        GemVal _t1408 = gem_fn_make_continue(NULL, NULL, 0);
        gem_pop_frame();
        return _t1408;
    }
#line 889 "compiler/main.gem"
    GemVal _t1409 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1409, gem_string("type")), gem_string("load")))) {
#line 890 "compiler/main.gem"
    GemVal _t1410 = (*gem_v_advance);
        (void)(_t1410.fn(_t1410.env, NULL, 0));
#line 891 "compiler/main.gem"
    GemVal _t1411[] = {gem_string("STRING")};
    GemVal _t1412 = (*gem_v_expect);
    GemVal _t1413 = _t1412.fn(_t1412.env, _t1411, 1);
        GemVal gem_v_path = gem_table_get(_t1413, gem_string("value"));
#line 892 "compiler/main.gem"
    GemVal _t1414[] = {gem_v_path};
        GemVal _t1415 = gem_fn_make_load(NULL, _t1414, 1);
        gem_pop_frame();
        return _t1415;
    }
#line 896 "compiler/main.gem"
    GemVal _t1416 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1416, gem_string("type")), gem_string("extern")))) {
#line 897 "compiler/main.gem"
    GemVal _t1417 = (*gem_v_advance);
        (void)(_t1417.fn(_t1417.env, NULL, 0));
#line 898 "compiler/main.gem"
    GemVal _t1418 = (*gem_v_peek);
    GemVal _t1419 = _t1418.fn(_t1418.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1419, gem_string("type")), gem_string("fn")))) {
#line 899 "compiler/main.gem"
    GemVal _t1420 = (*gem_v_advance);
            (void)(_t1420.fn(_t1420.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1421[] = {gem_string("NAME")};
    GemVal _t1422 = (*gem_v_expect);
    GemVal _t1423 = _t1422.fn(_t1422.env, _t1421, 1);
            GemVal gem_v_name = gem_table_get(_t1423, gem_string("value"));
#line 901 "compiler/main.gem"
    GemVal _t1424[] = {gem_string("(")};
    GemVal _t1425 = (*gem_v_expect);
            (void)(_t1425.fn(_t1425.env, _t1424, 1));
#line 902 "compiler/main.gem"
    GemVal _t1426 = gem_table_new();
            GemVal gem_v_eparams = _t1426;
#line 903 "compiler/main.gem"
    GemVal _t1427 = (*gem_v_peek);
    GemVal _t1428 = _t1427.fn(_t1427.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1428, gem_string("type")), gem_string(")")))) {
#line 904 "compiler/main.gem"
    GemVal _t1429[] = {gem_string("NAME")};
    GemVal _t1430 = (*gem_v_expect);
    GemVal _t1431 = _t1430.fn(_t1430.env, _t1429, 1);
                GemVal gem_v_pname = gem_table_get(_t1431, gem_string("value"));
#line 905 "compiler/main.gem"
    GemVal _t1432[] = {gem_string(":")};
    GemVal _t1433 = (*gem_v_expect);
                (void)(_t1433.fn(_t1433.env, _t1432, 1));
#line 906 "compiler/main.gem"
    GemVal _t1434[] = {gem_string("NAME")};
    GemVal _t1435 = (*gem_v_expect);
    GemVal _t1436 = _t1435.fn(_t1435.env, _t1434, 1);
                GemVal gem_v_ptype = gem_table_get(_t1436, gem_string("value"));
#line 907 "compiler/main.gem"
    GemVal _t1437[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1438[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1437, 2)};
                (void)(gem_push_fn(NULL, _t1438, 2));
#line 908 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1439 = (*gem_v_peek);
                    GemVal _t1440 = _t1439.fn(_t1439.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1440, gem_string("type")), gem_string(",")))) break;
#line 909 "compiler/main.gem"
    GemVal _t1441 = (*gem_v_advance);
                    (void)(_t1441.fn(_t1441.env, NULL, 0));
#line 910 "compiler/main.gem"
    GemVal _t1442[] = {gem_string("NAME")};
    GemVal _t1443 = (*gem_v_expect);
    GemVal _t1444 = _t1443.fn(_t1443.env, _t1442, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1444, gem_string("value"));
#line 911 "compiler/main.gem"
    GemVal _t1445[] = {gem_string(":")};
    GemVal _t1446 = (*gem_v_expect);
                    (void)(_t1446.fn(_t1446.env, _t1445, 1));
#line 912 "compiler/main.gem"
    GemVal _t1447[] = {gem_string("NAME")};
    GemVal _t1448 = (*gem_v_expect);
    GemVal _t1449 = _t1448.fn(_t1448.env, _t1447, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1449, gem_string("value"));
#line 913 "compiler/main.gem"
    GemVal _t1450[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1451[] = {gem_v_eparams, gem_fn_make_extern_param(NULL, _t1450, 2)};
                    (void)(gem_push_fn(NULL, _t1451, 2));
                }
            }
#line 916 "compiler/main.gem"
    GemVal _t1452[] = {gem_string(")")};
    GemVal _t1453 = (*gem_v_expect);
            (void)(_t1453.fn(_t1453.env, _t1452, 1));
#line 917 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 918 "compiler/main.gem"
    GemVal _t1454 = (*gem_v_peek);
    GemVal _t1455 = _t1454.fn(_t1454.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1455, gem_string("type")), gem_string("->")))) {
#line 919 "compiler/main.gem"
    GemVal _t1456 = (*gem_v_advance);
                (void)(_t1456.fn(_t1456.env, NULL, 0));
#line 920 "compiler/main.gem"
    GemVal _t1457[] = {gem_string("NAME")};
    GemVal _t1458 = (*gem_v_expect);
    GemVal _t1459 = _t1458.fn(_t1458.env, _t1457, 1);
                gem_v_ret_type = gem_table_get(_t1459, gem_string("value"));
            }
#line 922 "compiler/main.gem"
    GemVal _t1460[] = {gem_v_name, gem_v_eparams, gem_v_ret_type};
            GemVal _t1461 = gem_fn_make_extern_fn(NULL, _t1460, 3);
            gem_pop_frame();
            return _t1461;
        } else {
#line 923 "compiler/main.gem"
    GemVal _t1462 = (*gem_v_peek);
    GemVal _t1463 = _t1462.fn(_t1462.env, NULL, 0);
    GemVal _t1466;
    if (!gem_truthy(gem_eq(gem_table_get(_t1463, gem_string("type")), gem_string("NAME")))) {
        _t1466 = gem_eq(gem_table_get(_t1463, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1464 = (*gem_v_peek);
        GemVal _t1465 = _t1464.fn(_t1464.env, NULL, 0);
        _t1466 = gem_eq(gem_table_get(_t1465, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1466)) {
#line 924 "compiler/main.gem"
    GemVal _t1467 = (*gem_v_advance);
                (void)(_t1467.fn(_t1467.env, NULL, 0));
#line 925 "compiler/main.gem"
    GemVal _t1468[] = {gem_string("STRING")};
    GemVal _t1469 = (*gem_v_expect);
    GemVal _t1470 = _t1469.fn(_t1469.env, _t1468, 1);
                GemVal gem_v_path = gem_table_get(_t1470, gem_string("value"));
#line 926 "compiler/main.gem"
    GemVal _t1471[] = {gem_v_path};
                GemVal _t1472 = gem_fn_make_extern_include(NULL, _t1471, 1);
                gem_pop_frame();
                return _t1472;
            } else {
#line 928 "compiler/main.gem"
    GemVal _t1473 = (*gem_v_peek);
    GemVal _t1474 = _t1473.fn(_t1473.env, NULL, 0);
    GemVal _t1475 = (*gem_v_peek);
    GemVal _t1476 = _t1475.fn(_t1475.env, NULL, 0);
    GemVal _t1477 = (*gem_v_peek);
    GemVal _t1478 = _t1477.fn(_t1477.env, NULL, 0);
    GemVal _t1479[] = {gem_table_get(_t1478, gem_string("value"))};
    GemVal _t1480[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1474, gem_string("line")), gem_table_get(_t1476, gem_string("col")), gem_len_fn(NULL, _t1479, 1), gem_string("expected 'fn' or 'include' after 'extern'"), GEM_NIL};
                (void)(gem_fn_compile_error(NULL, _t1480, 7));
            }
        }
    }
#line 933 "compiler/main.gem"
    GemVal _t1481 = gem_v_t;
    GemVal _t1483;
    if (!gem_truthy(gem_eq(gem_table_get(_t1481, gem_string("type")), gem_string("NAME")))) {
        _t1483 = gem_eq(gem_table_get(_t1481, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1482 = gem_v_t;
        _t1483 = gem_eq(gem_table_get(_t1482, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1483)) {
#line 934 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 935 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1484[] = {gem_v_rla};
            GemVal _t1485 = (*gem_v_peek_at);
            GemVal _t1486 = _t1485.fn(_t1485.env, _t1484, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1486, gem_string("type")), gem_string("NEWLINE")))) break;
#line 936 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 938 "compiler/main.gem"
    GemVal _t1487[] = {gem_v_rla};
    GemVal _t1488 = (*gem_v_peek_at);
    GemVal _t1489 = _t1488.fn(_t1488.env, _t1487, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1489, gem_string("type")), gem_string("when")))) {
#line 939 "compiler/main.gem"
    GemVal _t1490 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1490, gem_string("line"));
#line 940 "compiler/main.gem"
    GemVal _t1491 = (*gem_v_advance);
            (void)(_t1491.fn(_t1491.env, NULL, 0));
#line 941 "compiler/main.gem"
    GemVal _t1492 = (*gem_v_skip_nl);
            (void)(_t1492.fn(_t1492.env, NULL, 0));
#line 942 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 943 "compiler/main.gem"
    GemVal _t1493[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_add(gem_string("_recv_"), gem_to_string_fn(NULL, _t1493, 1));
#line 944 "compiler/main.gem"
    GemVal _t1494[] = {gem_v_recv_var};
            GemVal gem_v_recv_var_expr = gem_fn_make_var(NULL, _t1494, 1);
#line 945 "compiler/main.gem"
    GemVal _t1495 = gem_table_new();
            GemVal gem_v_rarms = _t1495;
#line 946 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1496 = (*gem_v_peek);
                GemVal _t1497 = _t1496.fn(_t1496.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1497, gem_string("type")), gem_string("when")))) break;
#line 947 "compiler/main.gem"
    GemVal _t1498 = (*gem_v_advance);
                (void)(_t1498.fn(_t1498.env, NULL, 0));
#line 948 "compiler/main.gem"
    GemVal _t1499 = (*gem_v_peek);
    GemVal _t1500 = _t1499.fn(_t1499.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1500, gem_string("type"));
#line 949 "compiler/main.gem"
    GemVal _t1501[] = {gem_int(1)};
    GemVal _t1502 = (*gem_v_peek_at);
    GemVal _t1503 = _t1502.fn(_t1502.env, _t1501, 1);
                GemVal gem_v_rnext = gem_table_get(_t1503, gem_string("type"));
#line 950 "compiler/main.gem"
    GemVal _t1504;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1504 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1504 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1504)) {
#line 951 "compiler/main.gem"
    GemVal _t1505[] = {gem_v_recv_var_expr};
    GemVal _t1506 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1506.fn(_t1506.env, _t1505, 1);
#line 952 "compiler/main.gem"
    GemVal _t1507 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1507.fn(_t1507.env, NULL, 0);
#line 953 "compiler/main.gem"
    GemVal _t1508 = gem_table_new();
    gem_table_set(_t1508, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1508, gem_string("body"), gem_v_rwbody);
    GemVal _t1509[] = {gem_v_rarms, _t1508};
                    (void)(gem_push_fn(NULL, _t1509, 2));
                } else {
#line 954 "compiler/main.gem"
    GemVal _t1515;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1515 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1510;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1510 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1510 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1511;
        if (gem_truthy(_t1510)) {
                _t1511 = _t1510;
        } else {
                _t1511 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1512;
        if (gem_truthy(_t1511)) {
                _t1512 = _t1511;
        } else {
                _t1512 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1513;
        if (gem_truthy(_t1512)) {
                _t1513 = _t1512;
        } else {
                _t1513 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1514;
        if (gem_truthy(_t1513)) {
                _t1514 = _t1513;
        } else {
                _t1514 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1515 = _t1514;
    }
                    if (gem_truthy(_t1515)) {
#line 955 "compiler/main.gem"
    GemVal _t1516[] = {gem_v_recv_var_expr};
    GemVal _t1517 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1517.fn(_t1517.env, _t1516, 1);
#line 956 "compiler/main.gem"
    GemVal _t1518 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1518.fn(_t1518.env, NULL, 0);
#line 957 "compiler/main.gem"
    GemVal _t1519 = gem_table_new();
    gem_table_set(_t1519, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1519, gem_string("body"), gem_v_rwbody);
    GemVal _t1520[] = {gem_v_rarms, _t1519};
                        (void)(gem_push_fn(NULL, _t1520, 2));
                    } else {
#line 959 "compiler/main.gem"
    GemVal _t1521 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1521.fn(_t1521.env, NULL, 0);
#line 960 "compiler/main.gem"
    GemVal _t1522 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1522.fn(_t1522.env, NULL, 0);
#line 961 "compiler/main.gem"
    GemVal _t1523 = gem_table_new();
    GemVal _t1524 = gem_table_new();
    GemVal _t1525[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    gem_table_set(_t1524, gem_string("condition"), gem_fn_make_binop(NULL, _t1525, 3));
    GemVal _t1526 = gem_table_new();
    gem_table_set(_t1524, gem_string("bindings"), _t1526);
    gem_table_set(_t1523, gem_string("pattern"), _t1524);
    gem_table_set(_t1523, gem_string("body"), gem_v_rwbody);
    GemVal _t1527[] = {gem_v_rarms, _t1523};
                        (void)(gem_push_fn(NULL, _t1527, 2));
                    }
                }
#line 963 "compiler/main.gem"
    GemVal _t1528 = (*gem_v_skip_nl);
                (void)(_t1528.fn(_t1528.env, NULL, 0));
            }
#line 965 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 966 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 967 "compiler/main.gem"
    GemVal _t1529 = (*gem_v_peek);
    GemVal _t1530 = _t1529.fn(_t1529.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1530, gem_string("type")), gem_string("after")))) {
#line 968 "compiler/main.gem"
    GemVal _t1531 = (*gem_v_advance);
                (void)(_t1531.fn(_t1531.env, NULL, 0));
#line 969 "compiler/main.gem"
    GemVal _t1532 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1532.fn(_t1532.env, NULL, 0);
#line 970 "compiler/main.gem"
    GemVal _t1533 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1533.fn(_t1533.env, NULL, 0);
            }
#line 972 "compiler/main.gem"
    GemVal _t1534[] = {gem_string("end")};
    GemVal _t1535 = (*gem_v_expect);
            (void)(_t1535.fn(_t1535.env, _t1534, 1));
#line 973 "compiler/main.gem"
    GemVal _t1536[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
            GemVal _t1537 = gem_fn_make_receive_match(NULL, _t1536, 5);
            gem_pop_frame();
            return _t1537;
        }
    }
#line 978 "compiler/main.gem"
    GemVal _t1538 = (*gem_v_parse_expr);
    GemVal _t1539 = _t1538.fn(_t1538.env, NULL, 0);
    gem_pop_frame();
    return _t1539;
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
    GemVal _t1541 = gem_table_new();
    GemVal gem_v_stmts = _t1541;
#line 988 "compiler/main.gem"
    GemVal _t1542 = (*gem_v_skip_nl);
    (void)(_t1542.fn(_t1542.env, NULL, 0));
#line 989 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1543 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1543.fn(_t1543.env, NULL, 0)))) break;
#line 990 "compiler/main.gem"
    GemVal _t1544 = (*gem_v_parse_stmt);
    GemVal _t1545[] = {gem_v_stmts, _t1544.fn(_t1544.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1545, 2));
#line 991 "compiler/main.gem"
    GemVal _t1546 = (*gem_v_skip_nl);
        (void)(_t1546.fn(_t1546.env, NULL, 0));
    }
#line 993 "compiler/main.gem"
    GemVal _t1547[] = {gem_v_stmts};
    GemVal _t1548 = gem_fn_make_program(NULL, _t1547, 1);
    gem_pop_frame();
    return _t1548;
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
    struct _closure__anon_1 *_t419 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t419->gem_v_pos = gem_v_pos;
    _t419->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t419);
#line 30 "compiler/main.gem"
    struct _closure__anon_2 *_t421 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t421->gem_v_pos = gem_v_pos;
    _t421->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t421);
#line 34 "compiler/main.gem"
    struct _closure__anon_3 *_t423 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t423->gem_v_pos = gem_v_pos;
    _t423->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t423);
#line 40 "compiler/main.gem"
    struct _closure__anon_4 *_t426 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t426->gem_v_pos = gem_v_pos;
    _t426->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t426);
#line 44 "compiler/main.gem"
    struct _closure__anon_5 *_t437 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t437->gem_v_file = gem_v_file;
    _t437->gem_v_pos = gem_v_pos;
    _t437->gem_v_source = gem_v_source;
    _t437->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t437);
#line 53 "compiler/main.gem"
    struct _closure__anon_6 *_t439 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t439->gem_v_pos = gem_v_pos;
    _t439->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t439);
#line 65 "compiler/main.gem"
    struct _closure__anon_7 *_t485 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t485->gem_v_advance = gem_v_advance;
    _t485->gem_v_expect = gem_v_expect;
    _t485->gem_v_file = gem_v_file;
    _t485->gem_v_peek = gem_v_peek;
    _t485->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t485);
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
    struct _closure__anon_8 *_t508 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t508->gem_v_at_end = gem_v_at_end;
    _t508->gem_v_parse_stmt = gem_v_parse_stmt;
    _t508->gem_v_peek = gem_v_peek;
    _t508->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t508);
#line 121 "compiler/main.gem"
    struct _closure__anon_9 *_t519 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t519->gem_v_at_end = gem_v_at_end;
    _t519->gem_v_parse_stmt = gem_v_parse_stmt;
    _t519->gem_v_peek = gem_v_peek;
    _t519->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t519);
#line 134 "compiler/main.gem"
    struct _closure__anon_10 *_t657 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t657->gem_v_advance = gem_v_advance;
    _t657->gem_v_at_end = gem_v_at_end;
    _t657->gem_v_expect = gem_v_expect;
    _t657->gem_v_file = gem_v_file;
    _t657->gem_v_fn_depth = gem_v_fn_depth;
    _t657->gem_v_parse_expr = gem_v_parse_expr;
    _t657->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t657->gem_v_parse_params = gem_v_parse_params;
    _t657->gem_v_peek = gem_v_peek;
    _t657->gem_v_peek_at = gem_v_peek_at;
    _t657->gem_v_skip_nl = gem_v_skip_nl;
    _t657->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t657);
#line 265 "compiler/main.gem"
    struct _closure__anon_11 *_t748 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t748->gem_v_advance = gem_v_advance;
    _t748->gem_v_expect = gem_v_expect;
    _t748->gem_v_fn_depth = gem_v_fn_depth;
    _t748->gem_v_parse_atom = gem_v_parse_atom;
    _t748->gem_v_parse_expr = gem_v_parse_expr;
    _t748->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t748->gem_v_parse_params = gem_v_parse_params;
    _t748->gem_v_peek = gem_v_peek;
    _t748->gem_v_peek_at = gem_v_peek_at;
    _t748->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t748);
#line 371 "compiler/main.gem"
    struct _closure__anon_12 *_t757 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t757->gem_v_advance = gem_v_advance;
    _t757->gem_v_parse_call = gem_v_parse_call;
    _t757->gem_v_parse_unary = gem_v_parse_unary;
    _t757->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t757);
#line 380 "compiler/main.gem"
    struct _closure__anon_13 *_t772 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t772->gem_v_advance = gem_v_advance;
    _t772->gem_v_parse_unary = gem_v_parse_unary;
    _t772->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t772);
#line 391 "compiler/main.gem"
    struct _closure__anon_14 *_t784 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t784->gem_v_advance = gem_v_advance;
    _t784->gem_v_parse_mul = gem_v_parse_mul;
    _t784->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t784);
#line 402 "compiler/main.gem"
    struct _closure__anon_15 *_t811 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t811->gem_v_advance = gem_v_advance;
    _t811->gem_v_parse_add = gem_v_parse_add;
    _t811->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t811);
#line 413 "compiler/main.gem"
    struct _closure__anon_16 *_t820 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t820->gem_v_advance = gem_v_advance;
    _t820->gem_v_parse_compare = gem_v_parse_compare;
    _t820->gem_v_parse_not = gem_v_parse_not;
    _t820->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t820);
#line 422 "compiler/main.gem"
    struct _closure__anon_17 *_t828 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t828->gem_v_advance = gem_v_advance;
    _t828->gem_v_parse_not = gem_v_parse_not;
    _t828->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t828);
#line 433 "compiler/main.gem"
    struct _closure__anon_18 *_t836 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t836->gem_v_advance = gem_v_advance;
    _t836->gem_v_parse_and = gem_v_parse_and;
    _t836->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t836);
#line 444 "compiler/main.gem"
    struct _closure__anon_19 *_t886 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t886->gem_v_advance = gem_v_advance;
    _t886->gem_v_file = gem_v_file;
    _t886->gem_v_parse_expr = gem_v_parse_expr;
    _t886->gem_v_parse_or = gem_v_parse_or;
    _t886->gem_v_peek = gem_v_peek;
    _t886->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t886);
#line 479 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 483 "compiler/main.gem"
    struct _closure__anon_20 *_t1058 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1058->gem_v_advance = gem_v_advance;
    _t1058->gem_v_expect = gem_v_expect;
    _t1058->gem_v_file = gem_v_file;
    _t1058->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1058->gem_v_peek = gem_v_peek;
    _t1058->gem_v_peek_at = gem_v_peek_at;
    _t1058->gem_v_skip_nl = gem_v_skip_nl;
    _t1058->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t1058);
#line 611 "compiler/main.gem"
    struct _closure__anon_22 *_t1540 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1540->gem_v_advance = gem_v_advance;
    _t1540->gem_v_expect = gem_v_expect;
    _t1540->gem_v_file = gem_v_file;
    _t1540->gem_v_fn_depth = gem_v_fn_depth;
    _t1540->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1540->gem_v_parse_body = gem_v_parse_body;
    _t1540->gem_v_parse_expr = gem_v_parse_expr;
    _t1540->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1540->gem_v_parse_params = gem_v_parse_params;
    _t1540->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1540->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1540->gem_v_peek = gem_v_peek;
    _t1540->gem_v_peek_at = gem_v_peek_at;
    _t1540->gem_v_skip_nl = gem_v_skip_nl;
    _t1540->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1540);
#line 982 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 986 "compiler/main.gem"
    struct _closure__anon_23 *_t1549 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1549->gem_v_at_end = gem_v_at_end;
    _t1549->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1549->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1549);
    GemVal _t1550 = gem_table_new();
    gem_table_set(_t1550, gem_string("parse"), gem_v_parse);
    GemVal _t1551 = _t1550;
    gem_pop_frame();
    return _t1551;
}

static GemVal gem_fn_escape_c_string(void *_env, GemVal *args, int argc) {
#line 9 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("escape_c_string", "compiler/main.gem", 9);
#line 10 "compiler/main.gem"
    GemVal _t1552[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1552, 1);
#line 11 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 12 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 12 "compiler/main.gem"
    GemVal _t1553[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1553, 1);
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
    GemVal _t1554[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1554, 1), gem_int(0)))) {
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

    GemVal _t1555 = gem_v_result;
    gem_pop_frame();
    return _t1555;
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

    GemVal _t1556 = gem_v_result;
    gem_pop_frame();
    return _t1556;
}

static GemVal gem_fn_pad_for(void *_env, GemVal *args, int argc) {
#line 41 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("pad_for", "compiler/main.gem", 41);
#line 42 "compiler/main.gem"
    GemVal _t1557[] = {gem_string("    "), gem_v_indent};
    GemVal _t1558 = gem_fn_repeat_str(NULL, _t1557, 2);
    gem_pop_frame();
    return _t1558;
}

static GemVal gem_fn_format_float(void *_env, GemVal *args, int argc) {
#line 45 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("format_float", "compiler/main.gem", 45);
#line 46 "compiler/main.gem"
    GemVal _t1559[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1559, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 49 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 49 "compiler/main.gem"
    GemVal _t1560[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1560, 1);
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
    GemVal _t1561 = gem_v_s;
    gem_pop_frame();
    return _t1561;
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
    GemVal _t1562 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1562;
}

static GemVal gem_fn_set_union(void *_env, GemVal *args, int argc) {
#line 72 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_union", "compiler/main.gem", 72);
#line 73 "compiler/main.gem"
    GemVal _t1563 = gem_table_new();
    GemVal gem_v_result = _t1563;
#line 74 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 74 "compiler/main.gem"
    GemVal _t1564[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1564, 1);
#line 74 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 74 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1565[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1565, 1)))) break;
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
    GemVal _t1566[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1566, 1);
#line 77 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 77 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1567[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1567, 1)))) break;
#line 77 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 77 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 77 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 78 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1568 = gem_v_result;
    gem_pop_frame();
    return _t1568;
}

static GemVal gem_fn_set_intersect(void *_env, GemVal *args, int argc) {
#line 83 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("set_intersect", "compiler/main.gem", 83);
#line 84 "compiler/main.gem"
    GemVal _t1569 = gem_table_new();
    GemVal gem_v_result = _t1569;
#line 85 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 85 "compiler/main.gem"
    GemVal _t1570[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1570, 1);
#line 85 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 85 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1571[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1571, 1)))) break;
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

    GemVal _t1572 = gem_v_result;
    gem_pop_frame();
    return _t1572;
}

static GemVal gem_fn_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("sorted_array_to_set", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t1573 = gem_table_new();
    GemVal gem_v_s = _t1573;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1574[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1574, 1)))) break;
#line 95 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 95 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1575 = gem_v_s;
    gem_pop_frame();
    return _t1575;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 101);
    GemVal _t1576[] = {gem_v_node};
    GemVal _t1578;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1576, 1), gem_string("table")))) {
        _t1578 = gem_eq(gem_type_fn(NULL, _t1576, 1), gem_string("table"));
    } else {
        GemVal _t1577 = gem_v_node;
        _t1578 = gem_eq(gem_table_get(_t1577, gem_string("tag")), gem_v_tag);
    }
    GemVal _t1579 = _t1578;
    gem_pop_frame();
    return _t1579;
}

static GemVal gem_fn_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 107 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("collect_block_lets", "compiler/main.gem", 107);
#line 108 "compiler/main.gem"
    GemVal _t1580[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn_is_node(NULL, _t1580, 2))) {
        {
#line 109 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 109 "compiler/main.gem"
    GemVal _t1581 = gem_v_stmt;
    GemVal _t1582[] = {gem_table_get(_t1581, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1582, 1);
#line 109 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 109 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 109 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 110 "compiler/main.gem"
    GemVal _t1583 = gem_v_stmt;
    GemVal _t1584[] = {gem_table_get(gem_table_get(_t1583, gem_string("stmts")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1584, 2))) {
#line 111 "compiler/main.gem"
    GemVal _t1585 = gem_v_stmt;
    GemVal _t1586 = gem_table_get(gem_table_get(_t1585, gem_string("stmts")), gem_v_j);
    GemVal _t1587[] = {gem_v_result, gem_table_get(_t1586, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1587, 2));
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
    GemVal _t1588[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1588, 1);
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
    GemVal _t1589[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t1589, 2))) {
#line 121 "compiler/main.gem"
    GemVal _t1590 = gem_v_s;
    GemVal _t1591[] = {gem_v_result, gem_table_get(_t1590, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1591, 2));
            }
#line 123 "compiler/main.gem"
    GemVal _t1592[] = {gem_v_s, gem_v_result};
            (void)(gem_fn_collect_block_lets(NULL, _t1592, 2));
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
#line 220 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1605[] = {(*gem_v_tmp_counter)};
    GemVal _t1606 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1605, 1));
    gem_pop_frame();
    return _t1606;
}

struct _closure__anon_25 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 225 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1608[] = {(*gem_v_anon_counter)};
    GemVal _t1609 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1608, 1));
    gem_pop_frame();
    return _t1609;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1611[] = {gem_v_name};
    GemVal _t1612 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1611, 1));
    gem_pop_frame();
    return _t1612;
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
#line 239 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 240 "compiler/main.gem"
        GemVal _t1613 = GEM_NIL;
        gem_pop_frame();
        return _t1613;
    }
#line 242 "compiler/main.gem"
    GemVal _t1614[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1614, 1), gem_string("table")))) {
#line 243 "compiler/main.gem"
        GemVal _t1615 = GEM_NIL;
        gem_pop_frame();
        return _t1615;
    }
#line 245 "compiler/main.gem"
    GemVal _t1616 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1616, gem_string("tag")), GEM_NIL))) {
#line 246 "compiler/main.gem"
        GemVal _t1617 = GEM_NIL;
        gem_pop_frame();
        return _t1617;
    }
#line 249 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1618[] = {gem_v__match_11};
    GemVal _t1620;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1618, 1), gem_string("table")))) {
        _t1620 = gem_eq(gem_type_fn(NULL, _t1618, 1), gem_string("table"));
    } else {
        GemVal _t1619[] = {gem_v__match_11, gem_string("tag")};
        _t1620 = gem_has_key_fn(NULL, _t1619, 2);
    }
    GemVal _t1621;
    if (!gem_truthy(_t1620)) {
        _t1621 = _t1620;
    } else {
        _t1621 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1623;
    if (!gem_truthy(_t1621)) {
        _t1623 = _t1621;
    } else {
        GemVal _t1622[] = {gem_v__match_11, gem_string("name")};
        _t1623 = gem_has_key_fn(NULL, _t1622, 2);
    }
    if (gem_truthy(_t1623)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 251 "compiler/main.gem"
    GemVal _t1624[] = {gem_v_defined, gem_v_name};
    GemVal _t1628;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1624, 2)))) {
        _t1628 = gem_not(gem_fn_set_contains(NULL, _t1624, 2));
    } else {
        GemVal _t1625[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1627;
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1625, 2)))) {
                _t1627 = gem_not(gem_fn_set_contains(NULL, _t1625, 2));
        } else {
                GemVal _t1626[] = {(*gem_v_local_names), gem_v_name};
                _t1627 = gem_fn_set_contains(NULL, _t1626, 2);
        }
        _t1628 = _t1627;
    }
    GemVal _t1630;
    if (!gem_truthy(_t1628)) {
        _t1630 = _t1628;
    } else {
        GemVal _t1629[] = {(*gem_v_defined_fns), gem_v_name};
        _t1630 = gem_not(gem_fn_set_contains(NULL, _t1629, 2));
    }
        if (gem_truthy(_t1630)) {
#line 252 "compiler/main.gem"
    GemVal _t1631[] = {gem_v_free, gem_v_name};
            GemVal _t1632 = gem_fn_set_add(NULL, _t1631, 2);
            gem_pop_frame();
            return _t1632;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1633[] = {gem_v__match_11};
    GemVal _t1635;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1633, 1), gem_string("table")))) {
        _t1635 = gem_eq(gem_type_fn(NULL, _t1633, 1), gem_string("table"));
    } else {
        GemVal _t1634[] = {gem_v__match_11, gem_string("tag")};
        _t1635 = gem_has_key_fn(NULL, _t1634, 2);
    }
    GemVal _t1636;
    if (!gem_truthy(_t1635)) {
        _t1636 = _t1635;
    } else {
        _t1636 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1638;
    if (!gem_truthy(_t1636)) {
        _t1638 = _t1636;
    } else {
        GemVal _t1637[] = {gem_v__match_11, gem_string("value")};
        _t1638 = gem_has_key_fn(NULL, _t1637, 2);
    }
    if (gem_truthy(_t1638)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 255 "compiler/main.gem"
    GemVal _t1639[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1640 = (*gem_v_collect_free_node);
        GemVal _t1641 = _t1640.fn(_t1640.env, _t1639, 3);
        gem_pop_frame();
        return _t1641;
    } else {
    GemVal _t1642[] = {gem_v__match_11};
    GemVal _t1644;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1642, 1), gem_string("table")))) {
        _t1644 = gem_eq(gem_type_fn(NULL, _t1642, 1), gem_string("table"));
    } else {
        GemVal _t1643[] = {gem_v__match_11, gem_string("tag")};
        _t1644 = gem_has_key_fn(NULL, _t1643, 2);
    }
    GemVal _t1645;
    if (!gem_truthy(_t1644)) {
        _t1645 = _t1644;
    } else {
        _t1645 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1647;
    if (!gem_truthy(_t1645)) {
        _t1647 = _t1645;
    } else {
        GemVal _t1646[] = {gem_v__match_11, gem_string("name")};
        _t1647 = gem_has_key_fn(NULL, _t1646, 2);
    }
    GemVal _t1649;
    if (!gem_truthy(_t1647)) {
        _t1649 = _t1647;
    } else {
        GemVal _t1648[] = {gem_v__match_11, gem_string("value")};
        _t1649 = gem_has_key_fn(NULL, _t1648, 2);
    }
    if (gem_truthy(_t1649)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 257 "compiler/main.gem"
    GemVal _t1650[] = {gem_v_defined, gem_v_name};
    GemVal _t1654;
    if (!gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1650, 2)))) {
        _t1654 = gem_not(gem_fn_set_contains(NULL, _t1650, 2));
    } else {
        GemVal _t1651[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1653;
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t1651, 2)))) {
                _t1653 = gem_not(gem_fn_set_contains(NULL, _t1651, 2));
        } else {
                GemVal _t1652[] = {(*gem_v_local_names), gem_v_name};
                _t1653 = gem_fn_set_contains(NULL, _t1652, 2);
        }
        _t1654 = _t1653;
    }
    GemVal _t1656;
    if (!gem_truthy(_t1654)) {
        _t1656 = _t1654;
    } else {
        GemVal _t1655[] = {(*gem_v_defined_fns), gem_v_name};
        _t1656 = gem_not(gem_fn_set_contains(NULL, _t1655, 2));
    }
        if (gem_truthy(_t1656)) {
#line 258 "compiler/main.gem"
    GemVal _t1657[] = {gem_v_free, gem_v_name};
            (void)(gem_fn_set_add(NULL, _t1657, 2));
        }
#line 260 "compiler/main.gem"
    GemVal _t1658[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1659 = (*gem_v_collect_free_node);
        GemVal _t1660 = _t1659.fn(_t1659.env, _t1658, 3);
        gem_pop_frame();
        return _t1660;
    } else {
    GemVal _t1661[] = {gem_v__match_11};
    GemVal _t1663;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1661, 1), gem_string("table")))) {
        _t1663 = gem_eq(gem_type_fn(NULL, _t1661, 1), gem_string("table"));
    } else {
        GemVal _t1662[] = {gem_v__match_11, gem_string("tag")};
        _t1663 = gem_has_key_fn(NULL, _t1662, 2);
    }
    GemVal _t1664;
    if (!gem_truthy(_t1663)) {
        _t1664 = _t1663;
    } else {
        _t1664 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1666;
    if (!gem_truthy(_t1664)) {
        _t1666 = _t1664;
    } else {
        GemVal _t1665[] = {gem_v__match_11, gem_string("params")};
        _t1666 = gem_has_key_fn(NULL, _t1665, 2);
    }
    GemVal _t1668;
    if (!gem_truthy(_t1666)) {
        _t1668 = _t1666;
    } else {
        GemVal _t1667[] = {gem_v__match_11, gem_string("rest_param")};
        _t1668 = gem_has_key_fn(NULL, _t1667, 2);
    }
    GemVal _t1670;
    if (!gem_truthy(_t1668)) {
        _t1670 = _t1668;
    } else {
        GemVal _t1669[] = {gem_v__match_11, gem_string("block_param")};
        _t1670 = gem_has_key_fn(NULL, _t1669, 2);
    }
    GemVal _t1672;
    if (!gem_truthy(_t1670)) {
        _t1672 = _t1670;
    } else {
        GemVal _t1671[] = {gem_v__match_11, gem_string("body")};
        _t1672 = gem_has_key_fn(NULL, _t1671, 2);
    }
    if (gem_truthy(_t1672)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_11, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 262 "compiler/main.gem"
    GemVal _t1673[] = {gem_v_params};
    GemVal _t1674[] = {gem_v_defined, gem_fn_sorted_array_to_set(NULL, _t1673, 1)};
        GemVal gem_v_inner_defined = gem_fn_set_union(NULL, _t1674, 2);
#line 263 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 264 "compiler/main.gem"
    GemVal _t1675[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1675, 2));
        }
#line 266 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 267 "compiler/main.gem"
    GemVal _t1676[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1676, 2));
        }
#line 269 "compiler/main.gem"
    GemVal _t1677[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1678 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1678.fn(_t1678.env, _t1677, 2);
        {
#line 270 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 270 "compiler/main.gem"
    GemVal _t1679[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_keys_12 = gem_keys_fn(NULL, _t1679, 1);
#line 270 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 270 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1680[] = {gem_v__for_keys_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1680, 1)))) break;
#line 270 "compiler/main.gem"
                GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_12, gem_v__for_i_12);
#line 270 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_12, gem_table_get(gem_v__for_keys_12, gem_v__for_i_12));
#line 270 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 271 "compiler/main.gem"
    GemVal _t1681[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn_set_add(NULL, _t1681, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1682[] = {gem_v__match_11};
    GemVal _t1684;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1682, 1), gem_string("table")))) {
        _t1684 = gem_eq(gem_type_fn(NULL, _t1682, 1), gem_string("table"));
    } else {
        GemVal _t1683[] = {gem_v__match_11, gem_string("tag")};
        _t1684 = gem_has_key_fn(NULL, _t1683, 2);
    }
    GemVal _t1685;
    if (!gem_truthy(_t1684)) {
        _t1685 = _t1684;
    } else {
        _t1685 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1685)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1686[] = {gem_v__match_11};
    GemVal _t1688;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1686, 1), gem_string("table")))) {
        _t1688 = gem_eq(gem_type_fn(NULL, _t1686, 1), gem_string("table"));
    } else {
        GemVal _t1687[] = {gem_v__match_11, gem_string("tag")};
        _t1688 = gem_has_key_fn(NULL, _t1687, 2);
    }
    GemVal _t1689;
    if (!gem_truthy(_t1688)) {
        _t1689 = _t1688;
    } else {
        _t1689 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1691;
    if (!gem_truthy(_t1689)) {
        _t1691 = _t1689;
    } else {
        GemVal _t1690[] = {gem_v__match_11, gem_string("cond")};
        _t1691 = gem_has_key_fn(NULL, _t1690, 2);
    }
    GemVal _t1693;
    if (!gem_truthy(_t1691)) {
        _t1693 = _t1691;
    } else {
        GemVal _t1692[] = {gem_v__match_11, gem_string("then")};
        _t1693 = gem_has_key_fn(NULL, _t1692, 2);
    }
    GemVal _t1695;
    if (!gem_truthy(_t1693)) {
        _t1695 = _t1693;
    } else {
        GemVal _t1694[] = {gem_v__match_11, gem_string("else")};
        _t1695 = gem_has_key_fn(NULL, _t1694, 2);
    }
    if (gem_truthy(_t1695)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 276 "compiler/main.gem"
    GemVal _t1696[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1697 = (*gem_v_collect_free_node);
        (void)(_t1697.fn(_t1697.env, _t1696, 3));
#line 277 "compiler/main.gem"
    GemVal _t1698 = gem_table_new();
    GemVal _t1699[] = {gem_v_defined, _t1698};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1699, 2);
#line 278 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 278 "compiler/main.gem"
    GemVal _t1700[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1700, 1);
#line 278 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 278 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 278 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 279 "compiler/main.gem"
    GemVal _t1701[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1702 = (*gem_v_collect_free_node);
            (void)(_t1702.fn(_t1702.env, _t1701, 3));
#line 280 "compiler/main.gem"
    GemVal _t1703[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t1703, 2))) {
#line 281 "compiler/main.gem"
    GemVal _t1704 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1705[] = {gem_v_d, gem_table_get(_t1704, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t1705, 2));
            }
        }

#line 284 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 285 "compiler/main.gem"
    GemVal _t1706 = gem_table_new();
    GemVal _t1707[] = {gem_v_defined, _t1706};
            gem_v_d = gem_fn_set_union(NULL, _t1707, 2);
            {
#line 286 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 286 "compiler/main.gem"
    GemVal _t1708[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1708, 1);
#line 286 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 286 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 286 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 287 "compiler/main.gem"
    GemVal _t1709[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1710 = (*gem_v_collect_free_node);
                    (void)(_t1710.fn(_t1710.env, _t1709, 3));
#line 288 "compiler/main.gem"
    GemVal _t1711[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1711, 2))) {
#line 289 "compiler/main.gem"
    GemVal _t1712 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1713[] = {gem_v_d, gem_table_get(_t1712, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1713, 2));
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
    GemVal _t1714[] = {gem_v__match_11};
    GemVal _t1716;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1714, 1), gem_string("table")))) {
        _t1716 = gem_eq(gem_type_fn(NULL, _t1714, 1), gem_string("table"));
    } else {
        GemVal _t1715[] = {gem_v__match_11, gem_string("tag")};
        _t1716 = gem_has_key_fn(NULL, _t1715, 2);
    }
    GemVal _t1717;
    if (!gem_truthy(_t1716)) {
        _t1717 = _t1716;
    } else {
        _t1717 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1719;
    if (!gem_truthy(_t1717)) {
        _t1719 = _t1717;
    } else {
        GemVal _t1718[] = {gem_v__match_11, gem_string("cond")};
        _t1719 = gem_has_key_fn(NULL, _t1718, 2);
    }
    GemVal _t1721;
    if (!gem_truthy(_t1719)) {
        _t1721 = _t1719;
    } else {
        GemVal _t1720[] = {gem_v__match_11, gem_string("body")};
        _t1721 = gem_has_key_fn(NULL, _t1720, 2);
    }
    if (gem_truthy(_t1721)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 294 "compiler/main.gem"
    GemVal _t1722[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1723 = (*gem_v_collect_free_node);
        (void)(_t1723.fn(_t1723.env, _t1722, 3));
#line 295 "compiler/main.gem"
    GemVal _t1724 = gem_table_new();
    GemVal _t1725[] = {gem_v_defined, _t1724};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1725, 2);
        {
#line 296 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 296 "compiler/main.gem"
    GemVal _t1726[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1726, 1);
#line 296 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 296 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 296 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 297 "compiler/main.gem"
    GemVal _t1727[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1728 = (*gem_v_collect_free_node);
                (void)(_t1728.fn(_t1728.env, _t1727, 3));
#line 298 "compiler/main.gem"
    GemVal _t1729[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1729, 2))) {
#line 299 "compiler/main.gem"
    GemVal _t1730 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1731[] = {gem_v_d, gem_table_get(_t1730, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1731, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1732[] = {gem_v__match_11};
    GemVal _t1734;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1732, 1), gem_string("table")))) {
        _t1734 = gem_eq(gem_type_fn(NULL, _t1732, 1), gem_string("table"));
    } else {
        GemVal _t1733[] = {gem_v__match_11, gem_string("tag")};
        _t1734 = gem_has_key_fn(NULL, _t1733, 2);
    }
    GemVal _t1735;
    if (!gem_truthy(_t1734)) {
        _t1735 = _t1734;
    } else {
        _t1735 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1737;
    if (!gem_truthy(_t1735)) {
        _t1737 = _t1735;
    } else {
        GemVal _t1736[] = {gem_v__match_11, gem_string("target")};
        _t1737 = gem_has_key_fn(NULL, _t1736, 2);
    }
    GemVal _t1739;
    if (!gem_truthy(_t1737)) {
        _t1739 = _t1737;
    } else {
        GemVal _t1738[] = {gem_v__match_11, gem_string("target_var")};
        _t1739 = gem_has_key_fn(NULL, _t1738, 2);
    }
    GemVal _t1741;
    if (!gem_truthy(_t1739)) {
        _t1741 = _t1739;
    } else {
        GemVal _t1740[] = {gem_v__match_11, gem_string("whens")};
        _t1741 = gem_has_key_fn(NULL, _t1740, 2);
    }
    GemVal _t1743;
    if (!gem_truthy(_t1741)) {
        _t1743 = _t1741;
    } else {
        GemVal _t1742[] = {gem_v__match_11, gem_string("else")};
        _t1743 = gem_has_key_fn(NULL, _t1742, 2);
    }
    if (gem_truthy(_t1743)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 303 "compiler/main.gem"
    GemVal _t1744[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1745 = (*gem_v_collect_free_node);
        (void)(_t1745.fn(_t1745.env, _t1744, 3));
#line 304 "compiler/main.gem"
    GemVal _t1746 = gem_table_new();
    GemVal _t1747[] = {gem_v_defined, _t1746};
        GemVal gem_v_match_def = gem_fn_set_union(NULL, _t1747, 2);
#line 305 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 306 "compiler/main.gem"
    GemVal _t1748[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn_set_add(NULL, _t1748, 2));
        }
#line 308 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 308 "compiler/main.gem"
    GemVal _t1749[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1749, 1);
#line 308 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 308 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 308 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 309 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 310 "compiler/main.gem"
    GemVal _t1750 = gem_v_w;
    GemVal _t1751[] = {gem_table_get(_t1750, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1752 = (*gem_v_collect_free_node);
            (void)(_t1752.fn(_t1752.env, _t1751, 3));
#line 311 "compiler/main.gem"
    GemVal _t1753 = gem_table_new();
    GemVal _t1754[] = {gem_v_match_def, _t1753};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1754, 2);
#line 312 "compiler/main.gem"
    GemVal _t1755 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1755, gem_string("bindings")), GEM_NIL))) {
#line 313 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 313 "compiler/main.gem"
    GemVal _t1756 = gem_v_w;
    GemVal _t1757[] = {gem_table_get(_t1756, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1757, 1);
#line 313 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 313 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 313 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 314 "compiler/main.gem"
    GemVal _t1758 = gem_v_w;
    GemVal _t1759[] = {gem_table_get(gem_table_get(_t1758, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1760 = (*gem_v_collect_free_node);
                    (void)(_t1760.fn(_t1760.env, _t1759, 3));
#line 315 "compiler/main.gem"
    GemVal _t1761 = gem_v_w;
    GemVal _t1762[] = {gem_table_get(gem_table_get(_t1761, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1762, 2))) {
#line 316 "compiler/main.gem"
    GemVal _t1763 = gem_v_w;
    GemVal _t1764 = gem_table_get(gem_table_get(_t1763, gem_string("bindings")), gem_v_bi);
    GemVal _t1765[] = {gem_v_d, gem_table_get(_t1764, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1765, 2));
                    }
                }

            }
#line 320 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 320 "compiler/main.gem"
    GemVal _t1766 = gem_v_w;
    GemVal _t1767[] = {gem_table_get(_t1766, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1767, 1);
#line 320 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 320 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 320 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 321 "compiler/main.gem"
    GemVal _t1768 = gem_v_w;
    GemVal _t1769[] = {gem_table_get(gem_table_get(_t1768, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1770 = (*gem_v_collect_free_node);
                (void)(_t1770.fn(_t1770.env, _t1769, 3));
#line 322 "compiler/main.gem"
    GemVal _t1771 = gem_v_w;
    GemVal _t1772[] = {gem_table_get(gem_table_get(_t1771, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1772, 2))) {
#line 323 "compiler/main.gem"
    GemVal _t1773 = gem_v_w;
    GemVal _t1774 = gem_table_get(gem_table_get(_t1773, gem_string("body")), gem_v_j);
    GemVal _t1775[] = {gem_v_d, gem_table_get(_t1774, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1775, 2));
                }
            }

        }

#line 327 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 328 "compiler/main.gem"
    GemVal _t1776 = gem_table_new();
    GemVal _t1777[] = {gem_v_defined, _t1776};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1777, 2);
            {
#line 329 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 329 "compiler/main.gem"
    GemVal _t1778[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t1778, 1);
#line 329 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 329 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 329 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 330 "compiler/main.gem"
    GemVal _t1779[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1780 = (*gem_v_collect_free_node);
                    (void)(_t1780.fn(_t1780.env, _t1779, 3));
#line 331 "compiler/main.gem"
    GemVal _t1781[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1781, 2))) {
#line 332 "compiler/main.gem"
    GemVal _t1782 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t1783[] = {gem_v_d, gem_table_get(_t1782, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1783, 2));
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
    GemVal _t1784[] = {gem_v__match_11};
    GemVal _t1786;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1784, 1), gem_string("table")))) {
        _t1786 = gem_eq(gem_type_fn(NULL, _t1784, 1), gem_string("table"));
    } else {
        GemVal _t1785[] = {gem_v__match_11, gem_string("tag")};
        _t1786 = gem_has_key_fn(NULL, _t1785, 2);
    }
    GemVal _t1787;
    if (!gem_truthy(_t1786)) {
        _t1787 = _t1786;
    } else {
        _t1787 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t1789;
    if (!gem_truthy(_t1787)) {
        _t1789 = _t1787;
    } else {
        GemVal _t1788[] = {gem_v__match_11, gem_string("value")};
        _t1789 = gem_has_key_fn(NULL, _t1788, 2);
    }
    GemVal _t1791;
    if (!gem_truthy(_t1789)) {
        _t1791 = _t1789;
    } else {
        GemVal _t1790[] = {gem_v__match_11, gem_string("bindings")};
        _t1791 = gem_has_key_fn(NULL, _t1790, 2);
    }
    GemVal _t1793;
    if (!gem_truthy(_t1791)) {
        _t1793 = _t1791;
    } else {
        GemVal _t1792[] = {gem_v__match_11, gem_string("body")};
        _t1793 = gem_has_key_fn(NULL, _t1792, 2);
    }
    if (gem_truthy(_t1793)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 337 "compiler/main.gem"
    GemVal _t1794[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1795 = (*gem_v_collect_free_node);
        (void)(_t1795.fn(_t1795.env, _t1794, 3));
#line 338 "compiler/main.gem"
    GemVal _t1796 = gem_table_new();
    GemVal _t1797[] = {gem_v_defined, _t1796};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1797, 2);
#line 339 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 340 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 340 "compiler/main.gem"
    GemVal _t1798[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t1798, 1);
#line 340 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 340 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 340 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 341 "compiler/main.gem"
    GemVal _t1799[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1800 = (*gem_v_collect_free_node);
                (void)(_t1800.fn(_t1800.env, _t1799, 3));
#line 342 "compiler/main.gem"
    GemVal _t1801[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1801, 2))) {
#line 343 "compiler/main.gem"
    GemVal _t1802 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t1803[] = {gem_v_d, gem_table_get(_t1802, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1803, 2));
                }
            }

        }
        {
#line 347 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 347 "compiler/main.gem"
    GemVal _t1804[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t1804, 1);
#line 347 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 347 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 347 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 348 "compiler/main.gem"
    GemVal _t1805[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1806 = (*gem_v_collect_free_node);
                (void)(_t1806.fn(_t1806.env, _t1805, 3));
#line 349 "compiler/main.gem"
    GemVal _t1807[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1807, 2))) {
#line 350 "compiler/main.gem"
    GemVal _t1808 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1809[] = {gem_v_d, gem_table_get(_t1808, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1809, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1810[] = {gem_v__match_11};
    GemVal _t1812;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1810, 1), gem_string("table")))) {
        _t1812 = gem_eq(gem_type_fn(NULL, _t1810, 1), gem_string("table"));
    } else {
        GemVal _t1811[] = {gem_v__match_11, gem_string("tag")};
        _t1812 = gem_has_key_fn(NULL, _t1811, 2);
    }
    GemVal _t1813;
    if (!gem_truthy(_t1812)) {
        _t1813 = _t1812;
    } else {
        _t1813 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t1815;
    if (!gem_truthy(_t1813)) {
        _t1815 = _t1813;
    } else {
        GemVal _t1814[] = {gem_v__match_11, gem_string("arms")};
        _t1815 = gem_has_key_fn(NULL, _t1814, 2);
    }
    GemVal _t1817;
    if (!gem_truthy(_t1815)) {
        _t1817 = _t1815;
    } else {
        GemVal _t1816[] = {gem_v__match_11, gem_string("after_ms")};
        _t1817 = gem_has_key_fn(NULL, _t1816, 2);
    }
    GemVal _t1819;
    if (!gem_truthy(_t1817)) {
        _t1819 = _t1817;
    } else {
        GemVal _t1818[] = {gem_v__match_11, gem_string("after_body")};
        _t1819 = gem_has_key_fn(NULL, _t1818, 2);
    }
    if (gem_truthy(_t1819)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 354 "compiler/main.gem"
    GemVal _t1820 = gem_table_new();
    GemVal _t1821[] = {gem_v_defined, _t1820};
        GemVal gem_v_recv_def = gem_fn_set_union(NULL, _t1821, 2);
#line 355 "compiler/main.gem"
    GemVal _t1822 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t1822, gem_string("recv_var")), GEM_NIL))) {
#line 356 "compiler/main.gem"
    GemVal _t1823 = gem_v_node;
    GemVal _t1824[] = {gem_v_recv_def, gem_table_get(_t1823, gem_string("recv_var"))};
            (void)(gem_fn_set_add(NULL, _t1824, 2));
        }
#line 358 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 358 "compiler/main.gem"
    GemVal _t1825[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t1825, 1);
#line 358 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 358 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 358 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 359 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 360 "compiler/main.gem"
    GemVal _t1826 = gem_v_arm;
    GemVal _t1827 = gem_table_get(_t1826, gem_string("pattern"));
    GemVal _t1828[] = {gem_table_get(_t1827, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t1829 = (*gem_v_collect_free_node);
            (void)(_t1829.fn(_t1829.env, _t1828, 3));
#line 361 "compiler/main.gem"
    GemVal _t1830 = gem_table_new();
    GemVal _t1831[] = {gem_v_recv_def, _t1830};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1831, 2);
#line 362 "compiler/main.gem"
    GemVal _t1832 = gem_v_arm;
    GemVal _t1833 = gem_table_get(_t1832, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t1833, gem_string("bindings")), GEM_NIL))) {
#line 363 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 363 "compiler/main.gem"
    GemVal _t1834 = gem_v_arm;
    GemVal _t1835 = gem_table_get(_t1834, gem_string("pattern"));
    GemVal _t1836[] = {gem_table_get(_t1835, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t1836, 1);
#line 363 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 363 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 363 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 364 "compiler/main.gem"
    GemVal _t1837 = gem_v_arm;
    GemVal _t1838 = gem_table_get(_t1837, gem_string("pattern"));
    GemVal _t1839[] = {gem_table_get(gem_table_get(_t1838, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1840 = (*gem_v_collect_free_node);
                    (void)(_t1840.fn(_t1840.env, _t1839, 3));
#line 365 "compiler/main.gem"
    GemVal _t1841 = gem_v_arm;
    GemVal _t1842 = gem_table_get(_t1841, gem_string("pattern"));
    GemVal _t1843[] = {gem_table_get(gem_table_get(_t1842, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1843, 2))) {
#line 366 "compiler/main.gem"
    GemVal _t1844 = gem_v_arm;
    GemVal _t1845 = gem_table_get(_t1844, gem_string("pattern"));
    GemVal _t1846 = gem_table_get(gem_table_get(_t1845, gem_string("bindings")), gem_v_bi);
    GemVal _t1847[] = {gem_v_d, gem_table_get(_t1846, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1847, 2));
                    }
                }

            }
#line 370 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 370 "compiler/main.gem"
    GemVal _t1848 = gem_v_arm;
    GemVal _t1849[] = {gem_table_get(_t1848, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t1849, 1);
#line 370 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 370 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 370 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 371 "compiler/main.gem"
    GemVal _t1850 = gem_v_arm;
    GemVal _t1851[] = {gem_table_get(gem_table_get(_t1850, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1852 = (*gem_v_collect_free_node);
                (void)(_t1852.fn(_t1852.env, _t1851, 3));
#line 372 "compiler/main.gem"
    GemVal _t1853 = gem_v_arm;
    GemVal _t1854[] = {gem_table_get(gem_table_get(_t1853, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1854, 2))) {
#line 373 "compiler/main.gem"
    GemVal _t1855 = gem_v_arm;
    GemVal _t1856 = gem_table_get(gem_table_get(_t1855, gem_string("body")), gem_v_j);
    GemVal _t1857[] = {gem_v_d, gem_table_get(_t1856, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1857, 2));
                }
            }

        }

#line 377 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 378 "compiler/main.gem"
    GemVal _t1858[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t1859 = (*gem_v_collect_free_node);
            (void)(_t1859.fn(_t1859.env, _t1858, 3));
        }
#line 380 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 381 "compiler/main.gem"
    GemVal _t1860 = gem_table_new();
    GemVal _t1861[] = {gem_v_recv_def, _t1860};
            GemVal gem_v_d = gem_fn_set_union(NULL, _t1861, 2);
            {
#line 382 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 382 "compiler/main.gem"
    GemVal _t1862[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t1862, 1);
#line 382 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 382 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 382 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 383 "compiler/main.gem"
    GemVal _t1863[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t1864 = (*gem_v_collect_free_node);
                    (void)(_t1864.fn(_t1864.env, _t1863, 3));
#line 384 "compiler/main.gem"
    GemVal _t1865[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn_is_node(NULL, _t1865, 2))) {
#line 385 "compiler/main.gem"
    GemVal _t1866 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t1867[] = {gem_v_d, gem_table_get(_t1866, gem_string("name"))};
                        (void)(gem_fn_set_add(NULL, _t1867, 2));
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
    GemVal _t1868[] = {gem_v__match_11};
    GemVal _t1870;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1868, 1), gem_string("table")))) {
        _t1870 = gem_eq(gem_type_fn(NULL, _t1868, 1), gem_string("table"));
    } else {
        GemVal _t1869[] = {gem_v__match_11, gem_string("tag")};
        _t1870 = gem_has_key_fn(NULL, _t1869, 2);
    }
    GemVal _t1871;
    if (!gem_truthy(_t1870)) {
        _t1871 = _t1870;
    } else {
        _t1871 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t1873;
    if (!gem_truthy(_t1871)) {
        _t1873 = _t1871;
    } else {
        GemVal _t1872[] = {gem_v__match_11, gem_string("entries")};
        _t1873 = gem_has_key_fn(NULL, _t1872, 2);
    }
    if (gem_truthy(_t1873)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 390 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 390 "compiler/main.gem"
    GemVal _t1874[] = {gem_v_entries};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t1874, 1);
#line 390 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 390 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 390 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 391 "compiler/main.gem"
    GemVal _t1875 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t1876[] = {gem_table_get(_t1875, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t1877 = (*gem_v_collect_free_node);
                (void)(_t1877.fn(_t1877.env, _t1876, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1878[] = {gem_v__match_11};
    GemVal _t1880;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1878, 1), gem_string("table")))) {
        _t1880 = gem_eq(gem_type_fn(NULL, _t1878, 1), gem_string("table"));
    } else {
        GemVal _t1879[] = {gem_v__match_11, gem_string("tag")};
        _t1880 = gem_has_key_fn(NULL, _t1879, 2);
    }
    GemVal _t1881;
    if (!gem_truthy(_t1880)) {
        _t1881 = _t1880;
    } else {
        _t1881 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t1883;
    if (!gem_truthy(_t1881)) {
        _t1883 = _t1881;
    } else {
        GemVal _t1882[] = {gem_v__match_11, gem_string("elements")};
        _t1883 = gem_has_key_fn(NULL, _t1882, 2);
    }
    if (gem_truthy(_t1883)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 394 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 394 "compiler/main.gem"
    GemVal _t1884[] = {gem_v_elements};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t1884, 1);
#line 394 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 394 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 394 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 395 "compiler/main.gem"
    GemVal _t1885[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t1886 = (*gem_v_collect_free_node);
                (void)(_t1886.fn(_t1886.env, _t1885, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1887[] = {gem_v__match_11};
    GemVal _t1889;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1887, 1), gem_string("table")))) {
        _t1889 = gem_eq(gem_type_fn(NULL, _t1887, 1), gem_string("table"));
    } else {
        GemVal _t1888[] = {gem_v__match_11, gem_string("tag")};
        _t1889 = gem_has_key_fn(NULL, _t1888, 2);
    }
    GemVal _t1890;
    if (!gem_truthy(_t1889)) {
        _t1890 = _t1889;
    } else {
        _t1890 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t1892;
    if (!gem_truthy(_t1890)) {
        _t1892 = _t1890;
    } else {
        GemVal _t1891[] = {gem_v__match_11, gem_string("stmts")};
        _t1892 = gem_has_key_fn(NULL, _t1891, 2);
    }
    if (gem_truthy(_t1892)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 398 "compiler/main.gem"
    GemVal _t1893 = gem_table_new();
    GemVal _t1894[] = {gem_v_defined, _t1893};
        GemVal gem_v_d = gem_fn_set_union(NULL, _t1894, 2);
        {
#line 399 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 399 "compiler/main.gem"
    GemVal _t1895[] = {gem_v_stmts};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t1895, 1);
#line 399 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 399 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 399 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 400 "compiler/main.gem"
    GemVal _t1896[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1897 = (*gem_v_collect_free_node);
                (void)(_t1897.fn(_t1897.env, _t1896, 3));
#line 401 "compiler/main.gem"
    GemVal _t1898[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn_is_node(NULL, _t1898, 2))) {
#line 402 "compiler/main.gem"
    GemVal _t1899 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1900[] = {gem_v_d, gem_table_get(_t1899, gem_string("name"))};
                    (void)(gem_fn_set_add(NULL, _t1900, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1901[] = {gem_v__match_11};
    GemVal _t1903;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1901, 1), gem_string("table")))) {
        _t1903 = gem_eq(gem_type_fn(NULL, _t1901, 1), gem_string("table"));
    } else {
        GemVal _t1902[] = {gem_v__match_11, gem_string("tag")};
        _t1903 = gem_has_key_fn(NULL, _t1902, 2);
    }
    GemVal _t1904;
    if (!gem_truthy(_t1903)) {
        _t1904 = _t1903;
    } else {
        _t1904 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t1906;
    if (!gem_truthy(_t1904)) {
        _t1906 = _t1904;
    } else {
        GemVal _t1905[] = {gem_v__match_11, gem_string("value")};
        _t1906 = gem_has_key_fn(NULL, _t1905, 2);
    }
    if (gem_truthy(_t1906)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 406 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 407 "compiler/main.gem"
    GemVal _t1907[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1908 = (*gem_v_collect_free_node);
            GemVal _t1909 = _t1908.fn(_t1908.env, _t1907, 3);
            gem_pop_frame();
            return _t1909;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 411 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 411 "compiler/main.gem"
    GemVal _t1910[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_keys_29 = gem_keys_fn(NULL, _t1910, 1);
#line 411 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 411 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1911[] = {gem_v__for_keys_29};
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_len_fn(NULL, _t1911, 1)))) break;
#line 411 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_29, gem_v__for_i_29);
#line 411 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_29, gem_table_get(gem_v__for_keys_29, gem_v__for_i_29));
#line 411 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 412 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 413 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1912, 1), gem_string("table")))) {
#line 414 "compiler/main.gem"
    GemVal _t1913[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t1913, 1);
#line 415 "compiler/main.gem"
    GemVal _t1914[] = {gem_v_vks};
    GemVal _t1916;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t1914, 1), gem_int(0)))) {
        _t1916 = gem_gt(gem_len_fn(NULL, _t1914, 1), gem_int(0));
    } else {
        GemVal _t1915[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t1916 = gem_eq(gem_type_fn(NULL, _t1915, 1), gem_string("int"));
    }
                        if (gem_truthy(_t1916)) {
#line 416 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 417 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t1917[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t1917, 1)))) break;
#line 418 "compiler/main.gem"
    GemVal _t1918[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t1918, 1), gem_string("table")))) {
#line 419 "compiler/main.gem"
    GemVal _t1919[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t1920 = (*gem_v_collect_free_node);
                                    (void)(_t1920.fn(_t1920.env, _t1919, 3));
                                }
#line 421 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 424 "compiler/main.gem"
    GemVal _t1921[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t1922 = (*gem_v_collect_free_node);
                            (void)(_t1922.fn(_t1922.env, _t1921, 3));
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
#line 433 "compiler/main.gem"
    GemVal _t1924 = gem_table_new();
    GemVal gem_v_free = _t1924;
#line 434 "compiler/main.gem"
    GemVal _t1925 = gem_table_new();
    GemVal _t1926[] = {gem_v_defined, _t1925};
    GemVal gem_v_d = gem_fn_set_union(NULL, _t1926, 2);
#line 435 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 435 "compiler/main.gem"
    GemVal _t1927[] = {gem_v_stmts};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t1927, 1);
#line 435 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 435 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 435 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 436 "compiler/main.gem"
    GemVal _t1928[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1929 = (*gem_v_collect_free_node);
        (void)(_t1929.fn(_t1929.env, _t1928, 3));
#line 437 "compiler/main.gem"
    GemVal _t1930[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn_is_node(NULL, _t1930, 2))) {
#line 438 "compiler/main.gem"
    GemVal _t1931 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t1932[] = {gem_v_d, gem_table_get(_t1931, gem_string("name"))};
            (void)(gem_fn_set_add(NULL, _t1932, 2));
        }
#line 440 "compiler/main.gem"
    GemVal _t1933[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn_collect_block_lets(NULL, _t1933, 2));
    }

    GemVal _t1934 = gem_v_free;
    gem_pop_frame();
    return _t1934;
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
#line 451 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 452 "compiler/main.gem"
        GemVal _t1936 = GEM_NIL;
        gem_pop_frame();
        return _t1936;
    }
#line 454 "compiler/main.gem"
    GemVal _t1937[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1937, 1), gem_string("table")))) {
#line 455 "compiler/main.gem"
        GemVal _t1938 = GEM_NIL;
        gem_pop_frame();
        return _t1938;
    }
#line 457 "compiler/main.gem"
    GemVal _t1939 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1939, gem_string("tag")), GEM_NIL))) {
#line 458 "compiler/main.gem"
        GemVal _t1940 = GEM_NIL;
        gem_pop_frame();
        return _t1940;
    }
#line 461 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
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
        _t1944 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1946;
    if (!gem_truthy(_t1944)) {
        _t1946 = _t1944;
    } else {
        GemVal _t1945[] = {gem_v__match_31, gem_string("params")};
        _t1946 = gem_has_key_fn(NULL, _t1945, 2);
    }
    GemVal _t1948;
    if (!gem_truthy(_t1946)) {
        _t1948 = _t1946;
    } else {
        GemVal _t1947[] = {gem_v__match_31, gem_string("rest_param")};
        _t1948 = gem_has_key_fn(NULL, _t1947, 2);
    }
    GemVal _t1950;
    if (!gem_truthy(_t1948)) {
        _t1950 = _t1948;
    } else {
        GemVal _t1949[] = {gem_v__match_31, gem_string("block_param")};
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
        GemVal gem_v_params = gem_table_get(gem_v__match_31, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_31, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_31, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 463 "compiler/main.gem"
    GemVal _t1953[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t1953, 1);
#line 464 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 465 "compiler/main.gem"
    GemVal _t1954[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn_set_add(NULL, _t1954, 2));
        }
#line 467 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 468 "compiler/main.gem"
    GemVal _t1955[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn_set_add(NULL, _t1955, 2));
        }
#line 475 "compiler/main.gem"
    GemVal _t1956[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1957 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t1957.fn(_t1957.env, _t1956, 2);
#line 476 "compiler/main.gem"
    GemVal _t1958[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn_set_intersect(NULL, _t1958, 2);
        {
#line 477 "compiler/main.gem"
            GemVal gem_v__for_tbl_32 = gem_v_inter;
#line 477 "compiler/main.gem"
    GemVal _t1959[] = {gem_v__for_tbl_32};
            GemVal gem_v__for_keys_32 = gem_keys_fn(NULL, _t1959, 1);
#line 477 "compiler/main.gem"
            GemVal gem_v__for_i_32 = gem_int(0);
#line 477 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1960[] = {gem_v__for_keys_32};
                if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t1960, 1)))) break;
#line 477 "compiler/main.gem"
                GemVal gem_v_ik = gem_table_get(gem_v__for_keys_32, gem_v__for_i_32);
#line 477 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_32, gem_table_get(gem_v__for_keys_32, gem_v__for_i_32));
#line 477 "compiler/main.gem"
                gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 478 "compiler/main.gem"
    GemVal _t1961[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn_set_add(NULL, _t1961, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1962[] = {gem_v__match_31};
    GemVal _t1964;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1962, 1), gem_string("table")))) {
        _t1964 = gem_eq(gem_type_fn(NULL, _t1962, 1), gem_string("table"));
    } else {
        GemVal _t1963[] = {gem_v__match_31, gem_string("tag")};
        _t1964 = gem_has_key_fn(NULL, _t1963, 2);
    }
    GemVal _t1965;
    if (!gem_truthy(_t1964)) {
        _t1965 = _t1964;
    } else {
        _t1965 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1965)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1966[] = {gem_v__match_31};
    GemVal _t1968;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1966, 1), gem_string("table")))) {
        _t1968 = gem_eq(gem_type_fn(NULL, _t1966, 1), gem_string("table"));
    } else {
        GemVal _t1967[] = {gem_v__match_31, gem_string("tag")};
        _t1968 = gem_has_key_fn(NULL, _t1967, 2);
    }
    GemVal _t1969;
    if (!gem_truthy(_t1968)) {
        _t1969 = _t1968;
    } else {
        _t1969 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1971;
    if (!gem_truthy(_t1969)) {
        _t1971 = _t1969;
    } else {
        GemVal _t1970[] = {gem_v__match_31, gem_string("cond")};
        _t1971 = gem_has_key_fn(NULL, _t1970, 2);
    }
    GemVal _t1973;
    if (!gem_truthy(_t1971)) {
        _t1973 = _t1971;
    } else {
        GemVal _t1972[] = {gem_v__match_31, gem_string("then")};
        _t1973 = gem_has_key_fn(NULL, _t1972, 2);
    }
    GemVal _t1975;
    if (!gem_truthy(_t1973)) {
        _t1975 = _t1973;
    } else {
        GemVal _t1974[] = {gem_v__match_31, gem_string("else")};
        _t1975 = gem_has_key_fn(NULL, _t1974, 2);
    }
    if (gem_truthy(_t1975)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_31, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 483 "compiler/main.gem"
    GemVal _t1976[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1977 = (*gem_v_walk_captures_node);
        (void)(_t1977.fn(_t1977.env, _t1976, 3));
#line 484 "compiler/main.gem"
    GemVal _t1978[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t1979 = (*gem_v_walk_captures);
        (void)(_t1979.fn(_t1979.env, _t1978, 3));
#line 485 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 486 "compiler/main.gem"
    GemVal _t1980[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t1981 = (*gem_v_walk_captures);
            GemVal _t1982 = _t1981.fn(_t1981.env, _t1980, 3);
            gem_pop_frame();
            return _t1982;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1983[] = {gem_v__match_31};
    GemVal _t1985;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1983, 1), gem_string("table")))) {
        _t1985 = gem_eq(gem_type_fn(NULL, _t1983, 1), gem_string("table"));
    } else {
        GemVal _t1984[] = {gem_v__match_31, gem_string("tag")};
        _t1985 = gem_has_key_fn(NULL, _t1984, 2);
    }
    GemVal _t1986;
    if (!gem_truthy(_t1985)) {
        _t1986 = _t1985;
    } else {
        _t1986 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1988;
    if (!gem_truthy(_t1986)) {
        _t1988 = _t1986;
    } else {
        GemVal _t1987[] = {gem_v__match_31, gem_string("cond")};
        _t1988 = gem_has_key_fn(NULL, _t1987, 2);
    }
    GemVal _t1990;
    if (!gem_truthy(_t1988)) {
        _t1990 = _t1988;
    } else {
        GemVal _t1989[] = {gem_v__match_31, gem_string("body")};
        _t1990 = gem_has_key_fn(NULL, _t1989, 2);
    }
    if (gem_truthy(_t1990)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 489 "compiler/main.gem"
    GemVal _t1991[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t1992 = (*gem_v_walk_captures_node);
        (void)(_t1992.fn(_t1992.env, _t1991, 3));
#line 490 "compiler/main.gem"
    GemVal _t1993[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t1994 = (*gem_v_walk_captures);
        GemVal _t1995 = _t1994.fn(_t1994.env, _t1993, 3);
        gem_pop_frame();
        return _t1995;
    } else {
    GemVal _t1996[] = {gem_v__match_31};
    GemVal _t1998;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1996, 1), gem_string("table")))) {
        _t1998 = gem_eq(gem_type_fn(NULL, _t1996, 1), gem_string("table"));
    } else {
        GemVal _t1997[] = {gem_v__match_31, gem_string("tag")};
        _t1998 = gem_has_key_fn(NULL, _t1997, 2);
    }
    GemVal _t1999;
    if (!gem_truthy(_t1998)) {
        _t1999 = _t1998;
    } else {
        _t1999 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2001;
    if (!gem_truthy(_t1999)) {
        _t2001 = _t1999;
    } else {
        GemVal _t2000[] = {gem_v__match_31, gem_string("target")};
        _t2001 = gem_has_key_fn(NULL, _t2000, 2);
    }
    GemVal _t2003;
    if (!gem_truthy(_t2001)) {
        _t2003 = _t2001;
    } else {
        GemVal _t2002[] = {gem_v__match_31, gem_string("whens")};
        _t2003 = gem_has_key_fn(NULL, _t2002, 2);
    }
    GemVal _t2005;
    if (!gem_truthy(_t2003)) {
        _t2005 = _t2003;
    } else {
        GemVal _t2004[] = {gem_v__match_31, gem_string("else")};
        _t2005 = gem_has_key_fn(NULL, _t2004, 2);
    }
    if (gem_truthy(_t2005)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_31, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_31, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 492 "compiler/main.gem"
    GemVal _t2006[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2007 = (*gem_v_walk_captures_node);
        (void)(_t2007.fn(_t2007.env, _t2006, 3));
#line 493 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 493 "compiler/main.gem"
    GemVal _t2008[] = {gem_v_whens};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t2008, 1);
#line 493 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 493 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 493 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 494 "compiler/main.gem"
    GemVal _t2009 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2010[] = {gem_table_get(_t2009, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2011 = (*gem_v_walk_captures_node);
            (void)(_t2011.fn(_t2011.env, _t2010, 3));
#line 495 "compiler/main.gem"
    GemVal _t2012 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t2012, gem_string("bindings")), GEM_NIL))) {
#line 496 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 496 "compiler/main.gem"
    GemVal _t2013 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2014[] = {gem_table_get(_t2013, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t2014, 1);
#line 496 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 496 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 496 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 497 "compiler/main.gem"
    GemVal _t2015 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2016[] = {gem_table_get(gem_table_get(_t2015, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2017 = (*gem_v_walk_captures_node);
                    (void)(_t2017.fn(_t2017.env, _t2016, 3));
                }

            }
#line 500 "compiler/main.gem"
    GemVal _t2018 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2019[] = {gem_table_get(_t2018, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2020 = (*gem_v_walk_captures);
            (void)(_t2020.fn(_t2020.env, _t2019, 3));
        }

#line 502 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 503 "compiler/main.gem"
    GemVal _t2021[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2022 = (*gem_v_walk_captures);
            GemVal _t2023 = _t2022.fn(_t2022.env, _t2021, 3);
            gem_pop_frame();
            return _t2023;
        } else {
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
        _t2027 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2029;
    if (!gem_truthy(_t2027)) {
        _t2029 = _t2027;
    } else {
        GemVal _t2028[] = {gem_v__match_31, gem_string("arms")};
        _t2029 = gem_has_key_fn(NULL, _t2028, 2);
    }
    GemVal _t2031;
    if (!gem_truthy(_t2029)) {
        _t2031 = _t2029;
    } else {
        GemVal _t2030[] = {gem_v__match_31, gem_string("after_ms")};
        _t2031 = gem_has_key_fn(NULL, _t2030, 2);
    }
    GemVal _t2033;
    if (!gem_truthy(_t2031)) {
        _t2033 = _t2031;
    } else {
        GemVal _t2032[] = {gem_v__match_31, gem_string("after_body")};
        _t2033 = gem_has_key_fn(NULL, _t2032, 2);
    }
    if (gem_truthy(_t2033)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_31, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_31, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_31, gem_string("after_body"));
#line 506 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 506 "compiler/main.gem"
    GemVal _t2034[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2034, 1);
#line 506 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 506 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 506 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 507 "compiler/main.gem"
    GemVal _t2035 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2036 = gem_table_get(_t2035, gem_string("pattern"));
    GemVal _t2037[] = {gem_table_get(_t2036, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2038 = (*gem_v_walk_captures_node);
            (void)(_t2038.fn(_t2038.env, _t2037, 3));
#line 508 "compiler/main.gem"
    GemVal _t2039 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2040 = gem_table_get(_t2039, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2040, gem_string("bindings")), GEM_NIL))) {
#line 509 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 509 "compiler/main.gem"
    GemVal _t2041 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2042 = gem_table_get(_t2041, gem_string("pattern"));
    GemVal _t2043[] = {gem_table_get(_t2042, gem_string("bindings"))};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2043, 1);
#line 509 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 509 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 509 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 510 "compiler/main.gem"
    GemVal _t2044 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2045 = gem_table_get(_t2044, gem_string("pattern"));
    GemVal _t2046[] = {gem_table_get(gem_table_get(_t2045, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2047 = (*gem_v_walk_captures_node);
                    (void)(_t2047.fn(_t2047.env, _t2046, 3));
                }

            }
#line 513 "compiler/main.gem"
    GemVal _t2048 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2049[] = {gem_table_get(_t2048, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2050 = (*gem_v_walk_captures);
            (void)(_t2050.fn(_t2050.env, _t2049, 3));
        }

#line 515 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 516 "compiler/main.gem"
    GemVal _t2051[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2052 = (*gem_v_walk_captures_node);
            (void)(_t2052.fn(_t2052.env, _t2051, 3));
        }
#line 518 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 519 "compiler/main.gem"
    GemVal _t2053[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2054 = (*gem_v_walk_captures);
            GemVal _t2055 = _t2054.fn(_t2054.env, _t2053, 3);
            gem_pop_frame();
            return _t2055;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2056[] = {gem_v__match_31};
    GemVal _t2058;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2056, 1), gem_string("table")))) {
        _t2058 = gem_eq(gem_type_fn(NULL, _t2056, 1), gem_string("table"));
    } else {
        GemVal _t2057[] = {gem_v__match_31, gem_string("tag")};
        _t2058 = gem_has_key_fn(NULL, _t2057, 2);
    }
    GemVal _t2059;
    if (!gem_truthy(_t2058)) {
        _t2059 = _t2058;
    } else {
        _t2059 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2061;
    if (!gem_truthy(_t2059)) {
        _t2061 = _t2059;
    } else {
        GemVal _t2060[] = {gem_v__match_31, gem_string("entries")};
        _t2061 = gem_has_key_fn(NULL, _t2060, 2);
    }
    if (gem_truthy(_t2061)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_31, gem_string("entries"));
        {
#line 522 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 522 "compiler/main.gem"
    GemVal _t2062[] = {gem_v_entries};
            GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2062, 1);
#line 522 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 522 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 522 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 523 "compiler/main.gem"
    GemVal _t2063 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2064[] = {gem_table_get(_t2063, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2065 = (*gem_v_walk_captures_node);
                (void)(_t2065.fn(_t2065.env, _t2064, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2066[] = {gem_v__match_31};
    GemVal _t2068;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2066, 1), gem_string("table")))) {
        _t2068 = gem_eq(gem_type_fn(NULL, _t2066, 1), gem_string("table"));
    } else {
        GemVal _t2067[] = {gem_v__match_31, gem_string("tag")};
        _t2068 = gem_has_key_fn(NULL, _t2067, 2);
    }
    GemVal _t2069;
    if (!gem_truthy(_t2068)) {
        _t2069 = _t2068;
    } else {
        _t2069 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2071;
    if (!gem_truthy(_t2069)) {
        _t2071 = _t2069;
    } else {
        GemVal _t2070[] = {gem_v__match_31, gem_string("elements")};
        _t2071 = gem_has_key_fn(NULL, _t2070, 2);
    }
    if (gem_truthy(_t2071)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_31, gem_string("elements"));
        {
#line 526 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 526 "compiler/main.gem"
    GemVal _t2072[] = {gem_v_elements};
            GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2072, 1);
#line 526 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 526 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 526 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 527 "compiler/main.gem"
    GemVal _t2073[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2074 = (*gem_v_walk_captures_node);
                (void)(_t2074.fn(_t2074.env, _t2073, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2075[] = {gem_v__match_31};
    GemVal _t2077;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2075, 1), gem_string("table")))) {
        _t2077 = gem_eq(gem_type_fn(NULL, _t2075, 1), gem_string("table"));
    } else {
        GemVal _t2076[] = {gem_v__match_31, gem_string("tag")};
        _t2077 = gem_has_key_fn(NULL, _t2076, 2);
    }
    GemVal _t2078;
    if (!gem_truthy(_t2077)) {
        _t2078 = _t2077;
    } else {
        _t2078 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2080;
    if (!gem_truthy(_t2078)) {
        _t2080 = _t2078;
    } else {
        GemVal _t2079[] = {gem_v__match_31, gem_string("stmts")};
        _t2080 = gem_has_key_fn(NULL, _t2079, 2);
    }
    if (gem_truthy(_t2080)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_31, gem_string("stmts"));
#line 530 "compiler/main.gem"
    GemVal _t2081[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2082 = (*gem_v_walk_captures);
        GemVal _t2083 = _t2082.fn(_t2082.env, _t2081, 3);
        gem_pop_frame();
        return _t2083;
    } else {
    GemVal _t2084[] = {gem_v__match_31};
    GemVal _t2086;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2084, 1), gem_string("table")))) {
        _t2086 = gem_eq(gem_type_fn(NULL, _t2084, 1), gem_string("table"));
    } else {
        GemVal _t2085[] = {gem_v__match_31, gem_string("tag")};
        _t2086 = gem_has_key_fn(NULL, _t2085, 2);
    }
    GemVal _t2087;
    if (!gem_truthy(_t2086)) {
        _t2087 = _t2086;
    } else {
        _t2087 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2089;
    if (!gem_truthy(_t2087)) {
        _t2089 = _t2087;
    } else {
        GemVal _t2088[] = {gem_v__match_31, gem_string("value")};
        _t2089 = gem_has_key_fn(NULL, _t2088, 2);
    }
    if (gem_truthy(_t2089)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 532 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 533 "compiler/main.gem"
    GemVal _t2090[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2091 = (*gem_v_walk_captures_node);
            GemVal _t2092 = _t2091.fn(_t2091.env, _t2090, 3);
            gem_pop_frame();
            return _t2092;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2093[] = {gem_v__match_31};
    GemVal _t2095;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2093, 1), gem_string("table")))) {
        _t2095 = gem_eq(gem_type_fn(NULL, _t2093, 1), gem_string("table"));
    } else {
        GemVal _t2094[] = {gem_v__match_31, gem_string("tag")};
        _t2095 = gem_has_key_fn(NULL, _t2094, 2);
    }
    GemVal _t2096;
    if (!gem_truthy(_t2095)) {
        _t2096 = _t2095;
    } else {
        _t2096 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2098;
    if (!gem_truthy(_t2096)) {
        _t2098 = _t2096;
    } else {
        GemVal _t2097[] = {gem_v__match_31, gem_string("value")};
        _t2098 = gem_has_key_fn(NULL, _t2097, 2);
    }
    if (gem_truthy(_t2098)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 536 "compiler/main.gem"
    GemVal _t2099[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2100 = (*gem_v_walk_captures_node);
        GemVal _t2101 = _t2100.fn(_t2100.env, _t2099, 3);
        gem_pop_frame();
        return _t2101;
    } else {
    GemVal _t2102[] = {gem_v__match_31};
    GemVal _t2104;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2102, 1), gem_string("table")))) {
        _t2104 = gem_eq(gem_type_fn(NULL, _t2102, 1), gem_string("table"));
    } else {
        GemVal _t2103[] = {gem_v__match_31, gem_string("tag")};
        _t2104 = gem_has_key_fn(NULL, _t2103, 2);
    }
    GemVal _t2105;
    if (!gem_truthy(_t2104)) {
        _t2105 = _t2104;
    } else {
        _t2105 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2107;
    if (!gem_truthy(_t2105)) {
        _t2107 = _t2105;
    } else {
        GemVal _t2106[] = {gem_v__match_31, gem_string("value")};
        _t2107 = gem_has_key_fn(NULL, _t2106, 2);
    }
    if (gem_truthy(_t2107)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 538 "compiler/main.gem"
    GemVal _t2108[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2109 = (*gem_v_walk_captures_node);
        GemVal _t2110 = _t2109.fn(_t2109.env, _t2108, 3);
        gem_pop_frame();
        return _t2110;
    } else {
        {
#line 541 "compiler/main.gem"
            GemVal gem_v__for_tbl_39 = gem_v_node;
#line 541 "compiler/main.gem"
    GemVal _t2111[] = {gem_v__for_tbl_39};
            GemVal gem_v__for_keys_39 = gem_keys_fn(NULL, _t2111, 1);
#line 541 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 541 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2112[] = {gem_v__for_keys_39};
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_len_fn(NULL, _t2112, 1)))) break;
#line 541 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_39, gem_v__for_i_39);
#line 541 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_39, gem_table_get(gem_v__for_keys_39, gem_v__for_i_39));
#line 541 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 542 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 543 "compiler/main.gem"
    GemVal _t2113[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2113, 1), gem_string("table")))) {
#line 544 "compiler/main.gem"
    GemVal _t2114[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2114, 1);
#line 545 "compiler/main.gem"
    GemVal _t2115[] = {gem_v_vks};
    GemVal _t2117;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2115, 1), gem_int(0)))) {
        _t2117 = gem_gt(gem_len_fn(NULL, _t2115, 1), gem_int(0));
    } else {
        GemVal _t2116[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2117 = gem_eq(gem_type_fn(NULL, _t2116, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2117)) {
#line 546 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 547 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2118[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2118, 1)))) break;
#line 548 "compiler/main.gem"
    GemVal _t2119[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2119, 1), gem_string("table")))) {
#line 549 "compiler/main.gem"
    GemVal _t2120[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2121 = (*gem_v_walk_captures_node);
                                    (void)(_t2121.fn(_t2121.env, _t2120, 3));
                                }
#line 551 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 554 "compiler/main.gem"
    GemVal _t2122[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2123 = (*gem_v_walk_captures_node);
                            (void)(_t2123.fn(_t2123.env, _t2122, 3));
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
#line 563 "compiler/main.gem"
    GemVal _t2125 = gem_table_new();
    GemVal _t2126[] = {gem_v_scope_vars, _t2125};
    GemVal gem_v_sv = gem_fn_set_union(NULL, _t2126, 2);
    {
#line 564 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 564 "compiler/main.gem"
    GemVal _t2127[] = {gem_v_stmts};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2127, 1);
#line 564 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 564 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 564 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 565 "compiler/main.gem"
    GemVal _t2128[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2129 = (*gem_v_walk_captures_node);
            (void)(_t2129.fn(_t2129.env, _t2128, 3));
#line 566 "compiler/main.gem"
    GemVal _t2130[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t2130, 2))) {
#line 567 "compiler/main.gem"
    GemVal _t2131 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2132[] = {gem_v_sv, gem_table_get(_t2131, gem_string("name"))};
                (void)(gem_fn_set_add(NULL, _t2132, 2));
            }
#line 569 "compiler/main.gem"
    GemVal _t2133[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn_collect_block_lets(NULL, _t2133, 2));
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
#line 574 "compiler/main.gem"
    GemVal _t2135 = gem_table_new();
    GemVal gem_v_captured = _t2135;
#line 575 "compiler/main.gem"
    GemVal _t2136[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2137 = (*gem_v_walk_captures);
    (void)(_t2137.fn(_t2137.env, _t2136, 3));
    GemVal _t2138 = gem_v_captured;
    gem_pop_frame();
    return _t2138;
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
#line 604 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2140[] = {gem_v__match_41};
    GemVal _t2142;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2140, 1), gem_string("table")))) {
        _t2142 = gem_eq(gem_type_fn(NULL, _t2140, 1), gem_string("table"));
    } else {
        GemVal _t2141[] = {gem_v__match_41, gem_string("tag")};
        _t2142 = gem_has_key_fn(NULL, _t2141, 2);
    }
    GemVal _t2143;
    if (!gem_truthy(_t2142)) {
        _t2143 = _t2142;
    } else {
        _t2143 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2145;
    if (!gem_truthy(_t2143)) {
        _t2145 = _t2143;
    } else {
        GemVal _t2144[] = {gem_v__match_41, gem_string("value")};
        _t2145 = gem_has_key_fn(NULL, _t2144, 2);
    }
    if (gem_truthy(_t2145)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 606 "compiler/main.gem"
    GemVal _t2146 = gem_table_new();
    GemVal _t2147[] = {gem_v_value};
    gem_table_set(_t2146, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t2147, 1)), gem_string(")")));
    gem_table_set(_t2146, gem_string("setup"), gem_string(""));
        GemVal _t2148 = _t2146;
        gem_pop_frame();
        return _t2148;
    } else {
    GemVal _t2149[] = {gem_v__match_41};
    GemVal _t2151;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2149, 1), gem_string("table")))) {
        _t2151 = gem_eq(gem_type_fn(NULL, _t2149, 1), gem_string("table"));
    } else {
        GemVal _t2150[] = {gem_v__match_41, gem_string("tag")};
        _t2151 = gem_has_key_fn(NULL, _t2150, 2);
    }
    GemVal _t2152;
    if (!gem_truthy(_t2151)) {
        _t2152 = _t2151;
    } else {
        _t2152 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2154;
    if (!gem_truthy(_t2152)) {
        _t2154 = _t2152;
    } else {
        GemVal _t2153[] = {gem_v__match_41, gem_string("value")};
        _t2154 = gem_has_key_fn(NULL, _t2153, 2);
    }
    if (gem_truthy(_t2154)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 608 "compiler/main.gem"
    GemVal _t2155 = gem_table_new();
    GemVal _t2156[] = {gem_v_value};
    GemVal _t2157[] = {gem_fn_format_float(NULL, _t2156, 1)};
    gem_table_set(_t2155, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t2157, 1)), gem_string(")")));
    gem_table_set(_t2155, gem_string("setup"), gem_string(""));
        GemVal _t2158 = _t2155;
        gem_pop_frame();
        return _t2158;
    } else {
    GemVal _t2159[] = {gem_v__match_41};
    GemVal _t2161;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2159, 1), gem_string("table")))) {
        _t2161 = gem_eq(gem_type_fn(NULL, _t2159, 1), gem_string("table"));
    } else {
        GemVal _t2160[] = {gem_v__match_41, gem_string("tag")};
        _t2161 = gem_has_key_fn(NULL, _t2160, 2);
    }
    GemVal _t2162;
    if (!gem_truthy(_t2161)) {
        _t2162 = _t2161;
    } else {
        _t2162 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2164;
    if (!gem_truthy(_t2162)) {
        _t2164 = _t2162;
    } else {
        GemVal _t2163[] = {gem_v__match_41, gem_string("value")};
        _t2164 = gem_has_key_fn(NULL, _t2163, 2);
    }
    if (gem_truthy(_t2164)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 610 "compiler/main.gem"
    GemVal _t2165[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2165, 1);
#line 611 "compiler/main.gem"
    GemVal _t2166 = gem_table_new();
    GemVal _t2167[] = {gem_v_escaped};
    gem_table_set(_t2166, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t2167, 1)), gem_string("\")")));
    gem_table_set(_t2166, gem_string("setup"), gem_string(""));
        GemVal _t2168 = _t2166;
        gem_pop_frame();
        return _t2168;
    } else {
    GemVal _t2169[] = {gem_v__match_41};
    GemVal _t2171;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2169, 1), gem_string("table")))) {
        _t2171 = gem_eq(gem_type_fn(NULL, _t2169, 1), gem_string("table"));
    } else {
        GemVal _t2170[] = {gem_v__match_41, gem_string("tag")};
        _t2171 = gem_has_key_fn(NULL, _t2170, 2);
    }
    GemVal _t2172;
    if (!gem_truthy(_t2171)) {
        _t2172 = _t2171;
    } else {
        _t2172 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2174;
    if (!gem_truthy(_t2172)) {
        _t2174 = _t2172;
    } else {
        GemVal _t2173[] = {gem_v__match_41, gem_string("parts")};
        _t2174 = gem_has_key_fn(NULL, _t2173, 2);
    }
    if (gem_truthy(_t2174)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_41, gem_string("parts"));
#line 613 "compiler/main.gem"
    GemVal _t2175[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2175, 1), gem_int(0)))) {
#line 614 "compiler/main.gem"
    GemVal _t2176 = gem_table_new();
    gem_table_set(_t2176, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2176, gem_string("setup"), gem_string(""));
            GemVal _t2177 = _t2176;
            gem_pop_frame();
            return _t2177;
        }
#line 616 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 617 "compiler/main.gem"
    GemVal _t2178 = gem_table_new();
        GemVal gem_v_compiled = _t2178;
#line 618 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 618 "compiler/main.gem"
    GemVal _t2179[] = {gem_v_parts};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2179, 1);
#line 618 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 618 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 618 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 619 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 620 "compiler/main.gem"
    GemVal _t2180[] = {gem_v_part};
    GemVal _t2181 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2181.fn(_t2181.env, _t2180, 1);
#line 621 "compiler/main.gem"
    GemVal _t2182 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2182, gem_string("setup")));
#line 622 "compiler/main.gem"
    GemVal _t2183 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t2183, gem_string("tag")), gem_string("string")))) {
#line 623 "compiler/main.gem"
    GemVal _t2184 = gem_v_r;
    GemVal _t2185[] = {gem_v_compiled, gem_table_get(_t2184, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t2185, 2));
            } else {
#line 625 "compiler/main.gem"
    GemVal _t2186 = (*gem_v_tmp);
                GemVal gem_v_ts = _t2186.fn(_t2186.env, NULL, 0);
#line 626 "compiler/main.gem"
    GemVal _t2187[] = {gem_v_ts};
    GemVal _t2188 = gem_v_r;
    GemVal _t2189[] = {gem_table_get(_t2188, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2187, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2189, 1)), gem_string("};\n")));
#line 627 "compiler/main.gem"
    GemVal _t2190[] = {gem_v_ts};
    GemVal _t2191[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t2190, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t2191, 2));
            }
        }

#line 630 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 631 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(1);
#line 631 "compiler/main.gem"
    GemVal _t2192[] = {gem_v_compiled};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2192, 1);
#line 631 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 631 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 631 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 632 "compiler/main.gem"
    GemVal _t2193[] = {gem_v_acc};
    GemVal _t2194[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2193, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2194, 1)), gem_string(")"));
        }

#line 634 "compiler/main.gem"
    GemVal _t2195 = gem_table_new();
    gem_table_set(_t2195, gem_string("expr"), gem_v_acc);
    gem_table_set(_t2195, gem_string("setup"), gem_v_setup);
        GemVal _t2196 = _t2195;
        gem_pop_frame();
        return _t2196;
    } else {
    GemVal _t2197[] = {gem_v__match_41};
    GemVal _t2199;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2197, 1), gem_string("table")))) {
        _t2199 = gem_eq(gem_type_fn(NULL, _t2197, 1), gem_string("table"));
    } else {
        GemVal _t2198[] = {gem_v__match_41, gem_string("tag")};
        _t2199 = gem_has_key_fn(NULL, _t2198, 2);
    }
    GemVal _t2200;
    if (!gem_truthy(_t2199)) {
        _t2200 = _t2199;
    } else {
        _t2200 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2202;
    if (!gem_truthy(_t2200)) {
        _t2202 = _t2200;
    } else {
        GemVal _t2201[] = {gem_v__match_41, gem_string("value")};
        _t2202 = gem_has_key_fn(NULL, _t2201, 2);
    }
    if (gem_truthy(_t2202)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 636 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 637 "compiler/main.gem"
    GemVal _t2203 = gem_table_new();
    gem_table_set(_t2203, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2203, gem_string("setup"), gem_string(""));
            GemVal _t2204 = _t2203;
            gem_pop_frame();
            return _t2204;
        }
#line 639 "compiler/main.gem"
    GemVal _t2205 = gem_table_new();
    gem_table_set(_t2205, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2205, gem_string("setup"), gem_string(""));
        GemVal _t2206 = _t2205;
        gem_pop_frame();
        return _t2206;
    } else {
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
        _t2210 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2210)) {
#line 641 "compiler/main.gem"
    GemVal _t2211 = gem_table_new();
    gem_table_set(_t2211, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2211, gem_string("setup"), gem_string(""));
        GemVal _t2212 = _t2211;
        gem_pop_frame();
        return _t2212;
    } else {
    GemVal _t2213[] = {gem_v__match_41};
    GemVal _t2215;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2213, 1), gem_string("table")))) {
        _t2215 = gem_eq(gem_type_fn(NULL, _t2213, 1), gem_string("table"));
    } else {
        GemVal _t2214[] = {gem_v__match_41, gem_string("tag")};
        _t2215 = gem_has_key_fn(NULL, _t2214, 2);
    }
    GemVal _t2216;
    if (!gem_truthy(_t2215)) {
        _t2216 = _t2215;
    } else {
        _t2216 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2218;
    if (!gem_truthy(_t2216)) {
        _t2218 = _t2216;
    } else {
        GemVal _t2217[] = {gem_v__match_41, gem_string("name")};
        _t2218 = gem_has_key_fn(NULL, _t2217, 2);
    }
    if (gem_truthy(_t2218)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_41, gem_string("name"));
#line 643 "compiler/main.gem"
    GemVal _t2219[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn_set_contains(NULL, _t2219, 2)))) {
#line 644 "compiler/main.gem"
    GemVal _t2220[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2220, 2))) {
#line 645 "compiler/main.gem"
    GemVal _t2221 = gem_table_new();
    GemVal _t2222[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t2221, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2222, 1)), gem_string(", NULL)")));
    gem_table_set(_t2221, gem_string("setup"), gem_string(""));
                GemVal _t2223 = _t2221;
                gem_pop_frame();
                return _t2223;
            } else {
#line 646 "compiler/main.gem"
    GemVal _t2224[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn_set_contains(NULL, _t2224, 2))) {
#line 647 "compiler/main.gem"
    GemVal _t2225 = gem_table_new();
    GemVal _t2226[] = {gem_v_name};
    gem_table_set(_t2225, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t2226, 1)), gem_string(", NULL)")));
    gem_table_set(_t2225, gem_string("setup"), gem_string(""));
                    GemVal _t2227 = _t2225;
                    gem_pop_frame();
                    return _t2227;
                }
            }
        }
#line 650 "compiler/main.gem"
    GemVal _t2228[] = {gem_v_name};
    GemVal _t2229 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2229.fn(_t2229.env, _t2228, 1);
#line 651 "compiler/main.gem"
    GemVal _t2230[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2232;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2230, 2))) {
        _t2232 = gem_fn_set_contains(NULL, _t2230, 2);
    } else {
        GemVal _t2231[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2232 = gem_fn_set_contains(NULL, _t2231, 2);
    }
        if (gem_truthy(_t2232)) {
#line 652 "compiler/main.gem"
    GemVal _t2233 = gem_table_new();
    GemVal _t2234[] = {gem_v_mname};
    gem_table_set(_t2233, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t2234, 1)), gem_string(")")));
    gem_table_set(_t2233, gem_string("setup"), gem_string(""));
            GemVal _t2235 = _t2233;
            gem_pop_frame();
            return _t2235;
        }
#line 654 "compiler/main.gem"
    GemVal _t2236 = gem_table_new();
    gem_table_set(_t2236, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2236, gem_string("setup"), gem_string(""));
        GemVal _t2237 = _t2236;
        gem_pop_frame();
        return _t2237;
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
        _t2241 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2241)) {
#line 656 "compiler/main.gem"
    GemVal _t2242[] = {gem_v_node};
    GemVal _t2243 = (*gem_v_compile_call);
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
        _t2248 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2248)) {
#line 658 "compiler/main.gem"
    GemVal _t2249[] = {gem_v_node};
    GemVal _t2250 = (*gem_v_compile_binop);
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
        _t2255 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2257;
    if (!gem_truthy(_t2255)) {
        _t2257 = _t2255;
    } else {
        GemVal _t2256[] = {gem_v__match_41, gem_string("expr")};
        _t2257 = gem_has_key_fn(NULL, _t2256, 2);
    }
    GemVal _t2259;
    if (!gem_truthy(_t2257)) {
        _t2259 = _t2257;
    } else {
        GemVal _t2258[] = {gem_v__match_41, gem_string("op")};
        _t2259 = gem_has_key_fn(NULL, _t2258, 2);
    }
    if (gem_truthy(_t2259)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_41, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_41, gem_string("op"));
#line 660 "compiler/main.gem"
    GemVal _t2260[] = {gem_v_uexpr};
    GemVal _t2261 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2261.fn(_t2261.env, _t2260, 1);
#line 661 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 662 "compiler/main.gem"
    GemVal _t2262 = gem_table_new();
    GemVal _t2263 = gem_v_r;
    GemVal _t2264[] = {gem_table_get(_t2263, gem_string("expr"))};
    gem_table_set(_t2262, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t2264, 1)), gem_string(")")));
    GemVal _t2265 = gem_v_r;
    gem_table_set(_t2262, gem_string("setup"), gem_table_get(_t2265, gem_string("setup")));
            GemVal _t2266 = _t2262;
            gem_pop_frame();
            return _t2266;
        } else {
#line 663 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 664 "compiler/main.gem"
    GemVal _t2267 = gem_table_new();
    GemVal _t2268 = gem_v_r;
    GemVal _t2269[] = {gem_table_get(_t2268, gem_string("expr"))};
    gem_table_set(_t2267, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2269, 1)), gem_string(")")));
    GemVal _t2270 = gem_v_r;
    gem_table_set(_t2267, gem_string("setup"), gem_table_get(_t2270, gem_string("setup")));
                GemVal _t2271 = _t2267;
                gem_pop_frame();
                return _t2271;
            }
        }
#line 666 "compiler/main.gem"
    GemVal _t2272[] = {gem_v_op};
    GemVal _t2273[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2272, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 666, _t2273, 1));
    } else {
    GemVal _t2274[] = {gem_v__match_41};
    GemVal _t2276;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2274, 1), gem_string("table")))) {
        _t2276 = gem_eq(gem_type_fn(NULL, _t2274, 1), gem_string("table"));
    } else {
        GemVal _t2275[] = {gem_v__match_41, gem_string("tag")};
        _t2276 = gem_has_key_fn(NULL, _t2275, 2);
    }
    GemVal _t2277;
    if (!gem_truthy(_t2276)) {
        _t2277 = _t2276;
    } else {
        _t2277 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2277)) {
#line 668 "compiler/main.gem"
    GemVal _t2278[] = {gem_v_node};
    GemVal _t2279 = (*gem_v_compile_anon_fn);
        GemVal _t2280 = _t2279.fn(_t2279.env, _t2278, 1);
        gem_pop_frame();
        return _t2280;
    } else {
    GemVal _t2281[] = {gem_v__match_41};
    GemVal _t2283;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2281, 1), gem_string("table")))) {
        _t2283 = gem_eq(gem_type_fn(NULL, _t2281, 1), gem_string("table"));
    } else {
        GemVal _t2282[] = {gem_v__match_41, gem_string("tag")};
        _t2283 = gem_has_key_fn(NULL, _t2282, 2);
    }
    GemVal _t2284;
    if (!gem_truthy(_t2283)) {
        _t2284 = _t2283;
    } else {
        _t2284 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2284)) {
#line 670 "compiler/main.gem"
    GemVal _t2285[] = {gem_v_node};
    GemVal _t2286 = (*gem_v_compile_table);
        GemVal _t2287 = _t2286.fn(_t2286.env, _t2285, 1);
        gem_pop_frame();
        return _t2287;
    } else {
    GemVal _t2288[] = {gem_v__match_41};
    GemVal _t2290;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2288, 1), gem_string("table")))) {
        _t2290 = gem_eq(gem_type_fn(NULL, _t2288, 1), gem_string("table"));
    } else {
        GemVal _t2289[] = {gem_v__match_41, gem_string("tag")};
        _t2290 = gem_has_key_fn(NULL, _t2289, 2);
    }
    GemVal _t2291;
    if (!gem_truthy(_t2290)) {
        _t2291 = _t2290;
    } else {
        _t2291 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2291)) {
#line 672 "compiler/main.gem"
    GemVal _t2292[] = {gem_v_node};
    GemVal _t2293 = (*gem_v_compile_array);
        GemVal _t2294 = _t2293.fn(_t2293.env, _t2292, 1);
        gem_pop_frame();
        return _t2294;
    } else {
    GemVal _t2295[] = {gem_v__match_41};
    GemVal _t2297;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2295, 1), gem_string("table")))) {
        _t2297 = gem_eq(gem_type_fn(NULL, _t2295, 1), gem_string("table"));
    } else {
        GemVal _t2296[] = {gem_v__match_41, gem_string("tag")};
        _t2297 = gem_has_key_fn(NULL, _t2296, 2);
    }
    GemVal _t2298;
    if (!gem_truthy(_t2297)) {
        _t2298 = _t2297;
    } else {
        _t2298 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2300;
    if (!gem_truthy(_t2298)) {
        _t2300 = _t2298;
    } else {
        GemVal _t2299[] = {gem_v__match_41, gem_string("object")};
        _t2300 = gem_has_key_fn(NULL, _t2299, 2);
    }
    GemVal _t2302;
    if (!gem_truthy(_t2300)) {
        _t2302 = _t2300;
    } else {
        GemVal _t2301[] = {gem_v__match_41, gem_string("field")};
        _t2302 = gem_has_key_fn(NULL, _t2301, 2);
    }
    if (gem_truthy(_t2302)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_41, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_41, gem_string("field"));
#line 674 "compiler/main.gem"
    GemVal _t2303[] = {gem_v_object};
    GemVal _t2304 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2304.fn(_t2304.env, _t2303, 1);
#line 675 "compiler/main.gem"
    GemVal _t2305 = (*gem_v_tmp);
        GemVal gem_v_t = _t2305.fn(_t2305.env, NULL, 0);
#line 676 "compiler/main.gem"
    GemVal _t2306 = gem_v_r;
    GemVal _t2307[] = {gem_v_t};
    GemVal _t2308 = gem_v_r;
    GemVal _t2309[] = {gem_table_get(_t2308, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2306, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2307, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2309, 1)), gem_string(";\n")));
#line 677 "compiler/main.gem"
    GemVal _t2310[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2310, 1);
#line 678 "compiler/main.gem"
    GemVal _t2311 = gem_table_new();
    GemVal _t2312[] = {gem_v_t};
    GemVal _t2313[] = {gem_v_escaped};
    gem_table_set(_t2311, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2312, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2313, 1)), gem_string("\"))")));
    gem_table_set(_t2311, gem_string("setup"), gem_v_setup);
        GemVal _t2314 = _t2311;
        gem_pop_frame();
        return _t2314;
    } else {
    GemVal _t2315[] = {gem_v__match_41};
    GemVal _t2317;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2315, 1), gem_string("table")))) {
        _t2317 = gem_eq(gem_type_fn(NULL, _t2315, 1), gem_string("table"));
    } else {
        GemVal _t2316[] = {gem_v__match_41, gem_string("tag")};
        _t2317 = gem_has_key_fn(NULL, _t2316, 2);
    }
    GemVal _t2318;
    if (!gem_truthy(_t2317)) {
        _t2318 = _t2317;
    } else {
        _t2318 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2320;
    if (!gem_truthy(_t2318)) {
        _t2320 = _t2318;
    } else {
        GemVal _t2319[] = {gem_v__match_41, gem_string("object")};
        _t2320 = gem_has_key_fn(NULL, _t2319, 2);
    }
    GemVal _t2322;
    if (!gem_truthy(_t2320)) {
        _t2322 = _t2320;
    } else {
        GemVal _t2321[] = {gem_v__match_41, gem_string("key")};
        _t2322 = gem_has_key_fn(NULL, _t2321, 2);
    }
    if (gem_truthy(_t2322)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_41, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_41, gem_string("key"));
#line 680 "compiler/main.gem"
    GemVal _t2323[] = {gem_v_object};
    GemVal _t2324 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2324.fn(_t2324.env, _t2323, 1);
#line 681 "compiler/main.gem"
    GemVal _t2325[] = {gem_v_key};
    GemVal _t2326 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2326.fn(_t2326.env, _t2325, 1);
#line 682 "compiler/main.gem"
    GemVal _t2327 = gem_table_new();
    GemVal _t2328 = gem_v_obj_r;
    GemVal _t2329[] = {gem_table_get(_t2328, gem_string("expr"))};
    GemVal _t2330 = gem_v_key_r;
    GemVal _t2331[] = {gem_table_get(_t2330, gem_string("expr"))};
    gem_table_set(_t2327, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2329, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2331, 1)), gem_string(")")));
    GemVal _t2332 = gem_v_obj_r;
    GemVal _t2333 = gem_v_key_r;
    gem_table_set(_t2327, gem_string("setup"), gem_add(gem_table_get(_t2332, gem_string("setup")), gem_table_get(_t2333, gem_string("setup"))));
        GemVal _t2334 = _t2327;
        gem_pop_frame();
        return _t2334;
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
#line 685 "compiler/main.gem"
    GemVal _t2335 = gem_v_node;
    GemVal _t2336[] = {gem_table_get(_t2335, gem_string("tag"))};
    GemVal _t2337[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2336, 1))};
    GemVal _t2338 = gem_error_at_fn("compiler/main.gem", 685, _t2337, 1);
    gem_pop_frame();
    return _t2338;
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
#line 691 "compiler/main.gem"
    GemVal _t2340 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2340, gem_string("entries"));
#line 692 "compiler/main.gem"
    GemVal _t2341 = (*gem_v_tmp);
    GemVal gem_v_t = _t2341.fn(_t2341.env, NULL, 0);
#line 693 "compiler/main.gem"
    GemVal _t2342[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2342, 1)), gem_string(" = gem_table_new();\n"));
#line 694 "compiler/main.gem"
    GemVal gem_v__for_items_44 = gem_v_entries;
#line 694 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 694 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2343[] = {gem_v__for_items_44};
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_len_fn(NULL, _t2343, 1)))) break;
#line 694 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_44, gem_v__for_i_44);
#line 694 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 695 "compiler/main.gem"
    GemVal _t2344 = gem_v_entry;
    GemVal _t2345[] = {gem_table_get(_t2344, gem_string("value"))};
    GemVal _t2346 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2346.fn(_t2346.env, _t2345, 1);
#line 696 "compiler/main.gem"
    GemVal _t2347 = gem_v_entry;
    GemVal _t2348[] = {gem_table_get(_t2347, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2348, 1);
#line 697 "compiler/main.gem"
    GemVal _t2349 = gem_v_val_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2349, gem_string("setup")));
#line 698 "compiler/main.gem"
    GemVal _t2350[] = {gem_v_t};
    GemVal _t2351[] = {gem_v_escaped};
    GemVal _t2352 = gem_v_val_r;
    GemVal _t2353[] = {gem_table_get(_t2352, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2350, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2351, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2353, 1)), gem_string(");\n")));
    }

    GemVal _t2354 = gem_table_new();
    gem_table_set(_t2354, gem_string("expr"), gem_v_t);
    gem_table_set(_t2354, gem_string("setup"), gem_v_setup);
    GemVal _t2355 = _t2354;
    gem_pop_frame();
    return _t2355;
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
#line 706 "compiler/main.gem"
    GemVal _t2357 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2357, gem_string("elements"));
#line 707 "compiler/main.gem"
    GemVal _t2358 = (*gem_v_tmp);
    GemVal gem_v_t = _t2358.fn(_t2358.env, NULL, 0);
#line 708 "compiler/main.gem"
    GemVal _t2359[] = {gem_v_t};
    GemVal gem_v_setup = gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2359, 1)), gem_string(" = gem_table_new();\n"));
#line 709 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 709 "compiler/main.gem"
    GemVal _t2360[] = {gem_v_elements};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2360, 1);
#line 709 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 709 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 709 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 710 "compiler/main.gem"
    GemVal _t2361[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2362 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2362.fn(_t2362.env, _t2361, 1);
#line 711 "compiler/main.gem"
    GemVal _t2363 = gem_v_elem_r;
        gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2363, gem_string("setup")));
#line 712 "compiler/main.gem"
    GemVal _t2364[] = {gem_v_t};
    GemVal _t2365[] = {gem_v_i};
    GemVal _t2366 = gem_v_elem_r;
    GemVal _t2367[] = {gem_table_get(_t2366, gem_string("expr"))};
        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2364, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2365, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2367, 1)), gem_string(");\n")));
    }

    GemVal _t2368 = gem_table_new();
    gem_table_set(_t2368, gem_string("expr"), gem_v_t);
    gem_table_set(_t2368, gem_string("setup"), gem_v_setup);
    GemVal _t2369 = _t2368;
    gem_pop_frame();
    return _t2369;
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
#line 720 "compiler/main.gem"
    GemVal _t2371 = gem_v_node;
    GemVal _t2372[] = {gem_table_get(_t2371, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn_sorted_array_to_set(NULL, _t2372, 1);
#line 721 "compiler/main.gem"
    GemVal _t2373 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2373, gem_string("rest_param")), GEM_NIL))) {
#line 722 "compiler/main.gem"
    GemVal _t2374 = gem_v_node;
    GemVal _t2375[] = {gem_v_inner_defined, gem_table_get(_t2374, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t2375, 2));
    }
#line 724 "compiler/main.gem"
    GemVal _t2376 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2376, gem_string("block_param")), GEM_NIL))) {
#line 725 "compiler/main.gem"
    GemVal _t2377 = gem_v_node;
    GemVal _t2378[] = {gem_v_inner_defined, gem_table_get(_t2377, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t2378, 2));
    }
#line 727 "compiler/main.gem"
    GemVal _t2379 = gem_v_node;
    GemVal _t2380[] = {gem_table_get(_t2379, gem_string("body")), gem_v_inner_defined};
    GemVal _t2381 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2381.fn(_t2381.env, _t2380, 2);
#line 728 "compiler/main.gem"
    GemVal _t2382[] = {gem_v_free};
    GemVal _t2383[] = {gem_keys_fn(NULL, _t2382, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2383, 1);
#line 730 "compiler/main.gem"
    GemVal _t2384[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2384, 1), gem_int(0)))) {
#line 731 "compiler/main.gem"
    GemVal _t2385 = gem_table_new();
    GemVal _t2386[] = {gem_v_node, _t2385};
    GemVal _t2387 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2387.fn(_t2387.env, _t2386, 2);
#line 732 "compiler/main.gem"
    GemVal _t2388 = gem_table_new();
    GemVal _t2389 = gem_v_result;
    GemVal _t2390[] = {gem_table_get(_t2389, gem_string("fn_name"))};
    gem_table_set(_t2388, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2390, 1)), gem_string(", NULL)")));
    gem_table_set(_t2388, gem_string("setup"), gem_string(""));
        GemVal _t2391 = _t2388;
        gem_pop_frame();
        return _t2391;
    }
#line 735 "compiler/main.gem"
    GemVal _t2392[] = {gem_v_node, gem_v_captures};
    GemVal _t2393 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2393.fn(_t2393.env, _t2392, 2);
#line 737 "compiler/main.gem"
    GemVal gem_v_setup = gem_string("");
#line 738 "compiler/main.gem"
    GemVal _t2394 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2394.fn(_t2394.env, NULL, 0);
#line 739 "compiler/main.gem"
    GemVal _t2395 = gem_v_result;
    GemVal _t2396[] = {gem_table_get(_t2395, gem_string("struct_name"))};
    GemVal _t2397[] = {gem_v_env_tmp};
    GemVal _t2398 = gem_v_result;
    GemVal _t2399[] = {gem_table_get(_t2398, gem_string("struct_name"))};
    gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2396, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2397, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2399, 1)), gem_string("));\n")));
#line 740 "compiler/main.gem"
    GemVal gem_v__for_items_46 = gem_v_captures;
#line 740 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 740 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2400[] = {gem_v__for_items_46};
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_len_fn(NULL, _t2400, 1)))) break;
#line 740 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_46, gem_v__for_i_46);
#line 740 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 741 "compiler/main.gem"
    GemVal _t2401[] = {gem_v_cap};
    GemVal _t2402 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2402.fn(_t2402.env, _t2401, 1);
#line 742 "compiler/main.gem"
    GemVal _t2403[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2403, 2))) {
#line 743 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_env_tmp};
    GemVal _t2405[] = {gem_v_mc};
    GemVal _t2406[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2404, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2405, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2406, 1)), gem_string(";\n")));
        } else {
#line 745 "compiler/main.gem"
    GemVal _t2407[] = {gem_v_env_tmp};
    GemVal _t2408[] = {gem_v_mc};
    GemVal _t2409[] = {gem_v_mc};
            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2407, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2408, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2409, 1)), gem_string(";\n")));
        }
    }

    GemVal _t2410 = gem_table_new();
    GemVal _t2411 = gem_v_result;
    GemVal _t2412[] = {gem_table_get(_t2411, gem_string("fn_name"))};
    GemVal _t2413[] = {gem_v_env_tmp};
    gem_table_set(_t2410, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2412, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2413, 1)), gem_string(")")));
    gem_table_set(_t2410, gem_string("setup"), gem_v_setup);
    GemVal _t2414 = _t2410;
    gem_pop_frame();
    return _t2414;
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
#line 755 "compiler/main.gem"
    GemVal gem_v_arg_setups = gem_string("");
#line 756 "compiler/main.gem"
    GemVal _t2416 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2416;
#line 757 "compiler/main.gem"
    GemVal gem_v__for_i_47 = gem_int(0);
#line 757 "compiler/main.gem"
    GemVal _t2417[] = {gem_v_args};
    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2417, 1);
#line 757 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 757 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_47;
#line 757 "compiler/main.gem"
        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 758 "compiler/main.gem"
    GemVal _t2418[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2419 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2419.fn(_t2419.env, _t2418, 1);
#line 759 "compiler/main.gem"
    GemVal _t2420 = gem_v_r;
        gem_v_arg_setups = gem_add(gem_v_arg_setups, gem_table_get(_t2420, gem_string("setup")));
#line 760 "compiler/main.gem"
    GemVal _t2421 = gem_v_r;
    GemVal _t2422[] = {gem_v_arg_exprs, gem_table_get(_t2421, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2422, 2));
    }

#line 762 "compiler/main.gem"
    GemVal _t2423[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2423, 1);
#line 763 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 764 "compiler/main.gem"
    GemVal _t2424 = gem_table_new();
    gem_table_set(_t2424, gem_string("setup"), gem_v_arg_setups);
    gem_table_set(_t2424, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2424, gem_string("argc"), gem_int(0));
        GemVal _t2425 = _t2424;
        gem_pop_frame();
        return _t2425;
    }
#line 766 "compiler/main.gem"
    GemVal _t2426 = (*gem_v_tmp);
    GemVal gem_v_t = _t2426.fn(_t2426.env, NULL, 0);
#line 767 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 768 "compiler/main.gem"
    GemVal gem_v__for_i_48 = gem_int(1);
#line 768 "compiler/main.gem"
    GemVal gem_v__for_limit_48 = gem_v_argc;
#line 768 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 768 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_48;
#line 768 "compiler/main.gem"
        gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 769 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2427 = gem_table_new();
    GemVal _t2428[] = {gem_v_t};
    GemVal _t2429[] = {gem_v_arr};
    gem_table_set(_t2427, gem_string("setup"), gem_add(gem_v_arg_setups, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2428, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2429, 1)), gem_string("};\n"))));
    gem_table_set(_t2427, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2427, gem_string("argc"), gem_v_argc);
    GemVal _t2430 = _t2427;
    gem_pop_frame();
    return _t2430;
}

struct _closure__anon_37 {
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 776 "compiler/main.gem"
    GemVal _t2432 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2432, gem_string("argc")), gem_int(0)))) {
#line 777 "compiler/main.gem"
    GemVal _t2433 = gem_table_new();
    GemVal _t2434[] = {gem_v_fn_name};
    GemVal _t2435[] = {gem_v_env};
    gem_table_set(_t2433, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2434, 1), gem_string("(")), gem_to_string_fn(NULL, _t2435, 1)), gem_string(", NULL, 0)")));
    GemVal _t2436 = gem_v_ca;
    gem_table_set(_t2433, gem_string("setup"), gem_table_get(_t2436, gem_string("setup")));
        GemVal _t2437 = _t2433;
        gem_pop_frame();
        return _t2437;
    }
    GemVal _t2438 = gem_table_new();
    GemVal _t2439[] = {gem_v_fn_name};
    GemVal _t2440[] = {gem_v_env};
    GemVal _t2441 = gem_v_ca;
    GemVal _t2442[] = {gem_table_get(_t2441, gem_string("arr_name"))};
    GemVal _t2443 = gem_v_ca;
    GemVal _t2444[] = {gem_table_get(_t2443, gem_string("argc"))};
    gem_table_set(_t2438, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2439, 1), gem_string("(")), gem_to_string_fn(NULL, _t2440, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2442, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2444, 1)), gem_string(")")));
    GemVal _t2445 = gem_v_ca;
    gem_table_set(_t2438, gem_string("setup"), gem_table_get(_t2445, gem_string("setup")));
    GemVal _t2446 = _t2438;
    gem_pop_frame();
    return _t2446;
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
#line 785 "compiler/main.gem"
    GemVal gem_v__d49 = gem_v_node;
#line 785 "compiler/main.gem"
    GemVal _t2447 = gem_v__d49;
    GemVal gem_v_func = gem_table_get(_t2447, gem_string("func"));
#line 785 "compiler/main.gem"
    GemVal _t2448 = gem_v__d49;
    GemVal gem_v_args = gem_table_get(_t2448, gem_string("args"));

#line 788 "compiler/main.gem"
    GemVal _t2449 = gem_v_func;
    GemVal _t2451;
    if (!gem_truthy(gem_eq(gem_table_get(_t2449, gem_string("tag")), gem_string("dot")))) {
        _t2451 = gem_eq(gem_table_get(_t2449, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2450 = gem_v_func;
        _t2451 = gem_eq(gem_table_get(_t2450, gem_string("field")), gem_string("len"));
    }
    GemVal _t2453;
    if (!gem_truthy(_t2451)) {
        _t2453 = _t2451;
    } else {
        GemVal _t2452[] = {gem_v_args};
        _t2453 = gem_eq(gem_len_fn(NULL, _t2452, 1), gem_int(0));
    }
    if (gem_truthy(_t2453)) {
#line 789 "compiler/main.gem"
    GemVal _t2454 = gem_v_func;
    GemVal _t2455[] = {gem_table_get(_t2454, gem_string("object"))};
    GemVal _t2456 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2456.fn(_t2456.env, _t2455, 1);
#line 790 "compiler/main.gem"
    GemVal _t2457 = gem_table_new();
    GemVal _t2458 = gem_v_obj_r;
    GemVal _t2459[] = {gem_table_get(_t2458, gem_string("expr"))};
    gem_table_set(_t2457, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2459, 1)), gem_string(")")));
    GemVal _t2460 = gem_v_obj_r;
    gem_table_set(_t2457, gem_string("setup"), gem_table_get(_t2460, gem_string("setup")));
        GemVal _t2461 = _t2457;
        gem_pop_frame();
        return _t2461;
    }
#line 794 "compiler/main.gem"
    GemVal _t2462 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2462, gem_string("tag")), gem_string("var")))) {
#line 795 "compiler/main.gem"
    GemVal _t2463 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2463, gem_string("name"));
#line 797 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 798 "compiler/main.gem"
    GemVal _t2464 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2464, gem_string("line"));
#line 799 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 800 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 802 "compiler/main.gem"
    GemVal _t2465[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn_escape_c_string(NULL, _t2465, 1);
#line 803 "compiler/main.gem"
    GemVal _t2466[] = {gem_v_args};
    GemVal _t2467 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2467.fn(_t2467.env, _t2466, 1);
#line 804 "compiler/main.gem"
    GemVal _t2468 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2468, gem_string("argc")), gem_int(0)))) {
#line 805 "compiler/main.gem"
    GemVal _t2469 = gem_table_new();
    GemVal _t2470[] = {gem_v_escaped_file};
    GemVal _t2471[] = {gem_v_line};
    gem_table_set(_t2469, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2470, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2471, 1)), gem_string(", NULL, 0)")));
    GemVal _t2472 = gem_v_ca;
    gem_table_set(_t2469, gem_string("setup"), gem_table_get(_t2472, gem_string("setup")));
                GemVal _t2473 = _t2469;
                gem_pop_frame();
                return _t2473;
            }
#line 807 "compiler/main.gem"
    GemVal _t2474 = gem_table_new();
    GemVal _t2475[] = {gem_v_escaped_file};
    GemVal _t2476[] = {gem_v_line};
    GemVal _t2477 = gem_v_ca;
    GemVal _t2478[] = {gem_table_get(_t2477, gem_string("arr_name"))};
    GemVal _t2479 = gem_v_ca;
    GemVal _t2480[] = {gem_table_get(_t2479, gem_string("argc"))};
    gem_table_set(_t2474, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2475, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2476, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2478, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2480, 1)), gem_string(")")));
    GemVal _t2481 = gem_v_ca;
    gem_table_set(_t2474, gem_string("setup"), gem_table_get(_t2481, gem_string("setup")));
            GemVal _t2482 = _t2474;
            gem_pop_frame();
            return _t2482;
        } else {
#line 808 "compiler/main.gem"
    GemVal _t2483[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2485;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2483, 2))) {
        _t2485 = gem_has_key_fn(NULL, _t2483, 2);
    } else {
        GemVal _t2484[] = {(*gem_v_local_names), gem_v_name};
        _t2485 = gem_not(gem_fn_set_contains(NULL, _t2484, 2));
    }
            if (gem_truthy(_t2485)) {
#line 809 "compiler/main.gem"
    GemVal _t2486[] = {gem_v_args};
    GemVal _t2487 = (*gem_v_compile_args);
    GemVal _t2488[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2487.fn(_t2487.env, _t2486, 1)};
    GemVal _t2489 = (*gem_v_emit_direct_call);
                GemVal _t2490 = _t2489.fn(_t2489.env, _t2488, 3);
                gem_pop_frame();
                return _t2490;
            } else {
#line 810 "compiler/main.gem"
    GemVal _t2491[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2493;
    if (!gem_truthy(gem_fn_set_contains(NULL, _t2491, 2))) {
        _t2493 = gem_fn_set_contains(NULL, _t2491, 2);
    } else {
        GemVal _t2492[] = {(*gem_v_local_names), gem_v_name};
        _t2493 = gem_not(gem_fn_set_contains(NULL, _t2492, 2));
    }
                if (gem_truthy(_t2493)) {
#line 811 "compiler/main.gem"
    GemVal _t2494[] = {gem_v_name};
    GemVal _t2495[] = {gem_v_args};
    GemVal _t2496 = (*gem_v_compile_args);
    GemVal _t2497[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2494, 1)), gem_string("NULL"), _t2496.fn(_t2496.env, _t2495, 1)};
    GemVal _t2498 = (*gem_v_emit_direct_call);
                    GemVal _t2499 = _t2498.fn(_t2498.env, _t2497, 3);
                    gem_pop_frame();
                    return _t2499;
                }
            }
        }
    }
#line 816 "compiler/main.gem"
    GemVal _t2500[] = {gem_v_func};
    GemVal _t2501 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2501.fn(_t2501.env, _t2500, 1);
#line 817 "compiler/main.gem"
    GemVal _t2502[] = {gem_v_args};
    GemVal _t2503 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2503.fn(_t2503.env, _t2502, 1);
#line 818 "compiler/main.gem"
    GemVal _t2504 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2504.fn(_t2504.env, NULL, 0);
#line 819 "compiler/main.gem"
    GemVal _t2505 = gem_v_func_r;
    GemVal _t2506 = gem_v_ca;
    GemVal _t2507[] = {gem_v_tmp_fn};
    GemVal _t2508 = gem_v_func_r;
    GemVal _t2509[] = {gem_table_get(_t2508, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2505, gem_string("setup")), gem_table_get(_t2506, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2507, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2509, 1)), gem_string(";\n")));
#line 820 "compiler/main.gem"
    GemVal _t2510 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2510, gem_string("argc")), gem_int(0)))) {
#line 821 "compiler/main.gem"
    GemVal _t2511 = gem_table_new();
    GemVal _t2512[] = {gem_v_tmp_fn};
    GemVal _t2513[] = {gem_v_tmp_fn};
    gem_table_set(_t2511, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2512, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2513, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2511, gem_string("setup"), gem_v_setup);
        GemVal _t2514 = _t2511;
        gem_pop_frame();
        return _t2514;
    }
    GemVal _t2515 = gem_table_new();
    GemVal _t2516[] = {gem_v_tmp_fn};
    GemVal _t2517[] = {gem_v_tmp_fn};
    GemVal _t2518 = gem_v_ca;
    GemVal _t2519[] = {gem_table_get(_t2518, gem_string("arr_name"))};
    GemVal _t2520 = gem_v_ca;
    GemVal _t2521[] = {gem_table_get(_t2520, gem_string("argc"))};
    gem_table_set(_t2515, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2516, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2517, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2519, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2521, 1)), gem_string(")")));
    gem_table_set(_t2515, gem_string("setup"), gem_v_setup);
    GemVal _t2522 = _t2515;
    gem_pop_frame();
    return _t2522;
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
#line 829 "compiler/main.gem"
    GemVal _t2524 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2524, gem_string("op"));
#line 830 "compiler/main.gem"
    GemVal _t2525 = gem_v_node;
    GemVal _t2526[] = {gem_table_get(_t2525, gem_string("left"))};
    GemVal _t2527 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2527.fn(_t2527.env, _t2526, 1);
#line 831 "compiler/main.gem"
    GemVal _t2528 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2528, gem_string("expr"));
#line 832 "compiler/main.gem"
    GemVal _t2529 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2529, gem_string("setup"));
#line 834 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 835 "compiler/main.gem"
    GemVal _t2530 = gem_v_node;
    GemVal _t2531[] = {gem_table_get(_t2530, gem_string("right"))};
    GemVal _t2532 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2532.fn(_t2532.env, _t2531, 1);
#line 836 "compiler/main.gem"
    GemVal _t2533 = gem_table_new();
    GemVal _t2534[] = {gem_v_lc};
    GemVal _t2535 = gem_v_rc_r;
    GemVal _t2536[] = {gem_table_get(_t2535, gem_string("expr"))};
    gem_table_set(_t2533, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2534, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2536, 1)), gem_string(")")));
    GemVal _t2537 = gem_v_rc_r;
    gem_table_set(_t2533, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2537, gem_string("setup"))));
        GemVal _t2538 = _t2533;
        gem_pop_frame();
        return _t2538;
    } else {
#line 837 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 838 "compiler/main.gem"
    GemVal _t2539 = gem_v_node;
    GemVal _t2540[] = {gem_table_get(_t2539, gem_string("right"))};
    GemVal _t2541 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2541.fn(_t2541.env, _t2540, 1);
#line 839 "compiler/main.gem"
    GemVal _t2542 = gem_table_new();
    GemVal _t2543[] = {gem_v_lc};
    GemVal _t2544 = gem_v_rc_r;
    GemVal _t2545[] = {gem_table_get(_t2544, gem_string("expr"))};
    gem_table_set(_t2542, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2543, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2545, 1)), gem_string(")")));
    GemVal _t2546 = gem_v_rc_r;
    gem_table_set(_t2542, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2546, gem_string("setup"))));
            GemVal _t2547 = _t2542;
            gem_pop_frame();
            return _t2547;
        } else {
#line 840 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 841 "compiler/main.gem"
    GemVal _t2548 = gem_v_node;
    GemVal _t2549[] = {gem_table_get(_t2548, gem_string("right"))};
    GemVal _t2550 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2550.fn(_t2550.env, _t2549, 1);
#line 842 "compiler/main.gem"
    GemVal _t2551 = gem_table_new();
    GemVal _t2552[] = {gem_v_lc};
    GemVal _t2553 = gem_v_rc_r;
    GemVal _t2554[] = {gem_table_get(_t2553, gem_string("expr"))};
    gem_table_set(_t2551, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2552, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2554, 1)), gem_string(")")));
    GemVal _t2555 = gem_v_rc_r;
    gem_table_set(_t2551, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2555, gem_string("setup"))));
                GemVal _t2556 = _t2551;
                gem_pop_frame();
                return _t2556;
            } else {
#line 843 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 844 "compiler/main.gem"
    GemVal _t2557 = gem_v_node;
    GemVal _t2558[] = {gem_table_get(_t2557, gem_string("right"))};
    GemVal _t2559 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2559.fn(_t2559.env, _t2558, 1);
#line 845 "compiler/main.gem"
    GemVal _t2560 = gem_table_new();
    GemVal _t2561[] = {gem_v_lc};
    GemVal _t2562 = gem_v_rc_r;
    GemVal _t2563[] = {gem_table_get(_t2562, gem_string("expr"))};
    gem_table_set(_t2560, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2561, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2563, 1)), gem_string(")")));
    GemVal _t2564 = gem_v_rc_r;
    gem_table_set(_t2560, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2564, gem_string("setup"))));
                    GemVal _t2565 = _t2560;
                    gem_pop_frame();
                    return _t2565;
                } else {
#line 846 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 847 "compiler/main.gem"
    GemVal _t2566 = gem_v_node;
    GemVal _t2567[] = {gem_table_get(_t2566, gem_string("right"))};
    GemVal _t2568 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2568.fn(_t2568.env, _t2567, 1);
#line 848 "compiler/main.gem"
    GemVal _t2569 = gem_table_new();
    GemVal _t2570[] = {gem_v_lc};
    GemVal _t2571 = gem_v_rc_r;
    GemVal _t2572[] = {gem_table_get(_t2571, gem_string("expr"))};
    gem_table_set(_t2569, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2570, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2572, 1)), gem_string(")")));
    GemVal _t2573 = gem_v_rc_r;
    gem_table_set(_t2569, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2573, gem_string("setup"))));
                        GemVal _t2574 = _t2569;
                        gem_pop_frame();
                        return _t2574;
                    } else {
#line 849 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 850 "compiler/main.gem"
    GemVal _t2575 = gem_v_node;
    GemVal _t2576[] = {gem_table_get(_t2575, gem_string("right"))};
    GemVal _t2577 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2577.fn(_t2577.env, _t2576, 1);
#line 851 "compiler/main.gem"
    GemVal _t2578 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2578.fn(_t2578.env, NULL, 0);
#line 852 "compiler/main.gem"
    GemVal _t2579 = gem_v_rc_r;
    GemVal _t2580[] = {gem_v_t};
    GemVal _t2581 = gem_v_rc_r;
    GemVal _t2582[] = {gem_table_get(_t2581, gem_string("expr"))};
    GemVal _t2583[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2579, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2580, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2582, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2583, 1)), gem_string("};\n")));
#line 853 "compiler/main.gem"
    GemVal _t2584 = gem_table_new();
    GemVal _t2585[] = {gem_v_t};
    gem_table_set(_t2584, gem_string("expr"), gem_add(gem_add(gem_string("gem_has_key_fn(NULL, "), gem_to_string_fn(NULL, _t2585, 1)), gem_string(", 2)")));
    gem_table_set(_t2584, gem_string("setup"), gem_v_setup);
                            GemVal _t2586 = _t2584;
                            gem_pop_frame();
                            return _t2586;
                        } else {
#line 854 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 855 "compiler/main.gem"
    GemVal _t2587 = gem_v_node;
    GemVal _t2588[] = {gem_table_get(_t2587, gem_string("right"))};
    GemVal _t2589 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2589.fn(_t2589.env, _t2588, 1);
#line 856 "compiler/main.gem"
    GemVal _t2590 = gem_table_new();
    GemVal _t2591[] = {gem_v_lc};
    GemVal _t2592 = gem_v_rc_r;
    GemVal _t2593[] = {gem_table_get(_t2592, gem_string("expr"))};
    gem_table_set(_t2590, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2591, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2593, 1)), gem_string(")")));
    GemVal _t2594 = gem_v_rc_r;
    gem_table_set(_t2590, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2594, gem_string("setup"))));
                                GemVal _t2595 = _t2590;
                                gem_pop_frame();
                                return _t2595;
                            } else {
#line 857 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 858 "compiler/main.gem"
    GemVal _t2596 = gem_v_node;
    GemVal _t2597[] = {gem_table_get(_t2596, gem_string("right"))};
    GemVal _t2598 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2598.fn(_t2598.env, _t2597, 1);
#line 859 "compiler/main.gem"
    GemVal _t2599 = gem_table_new();
    GemVal _t2600[] = {gem_v_lc};
    GemVal _t2601 = gem_v_rc_r;
    GemVal _t2602[] = {gem_table_get(_t2601, gem_string("expr"))};
    gem_table_set(_t2599, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t2600, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2602, 1)), gem_string(")")));
    GemVal _t2603 = gem_v_rc_r;
    gem_table_set(_t2599, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2603, gem_string("setup"))));
                                    GemVal _t2604 = _t2599;
                                    gem_pop_frame();
                                    return _t2604;
                                } else {
#line 860 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 861 "compiler/main.gem"
    GemVal _t2605 = gem_v_node;
    GemVal _t2606[] = {gem_table_get(_t2605, gem_string("right"))};
    GemVal _t2607 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2607.fn(_t2607.env, _t2606, 1);
#line 862 "compiler/main.gem"
    GemVal _t2608 = gem_table_new();
    GemVal _t2609[] = {gem_v_lc};
    GemVal _t2610 = gem_v_rc_r;
    GemVal _t2611[] = {gem_table_get(_t2610, gem_string("expr"))};
    gem_table_set(_t2608, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t2609, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2611, 1)), gem_string(")")));
    GemVal _t2612 = gem_v_rc_r;
    gem_table_set(_t2608, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2612, gem_string("setup"))));
                                        GemVal _t2613 = _t2608;
                                        gem_pop_frame();
                                        return _t2613;
                                    } else {
#line 863 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 864 "compiler/main.gem"
    GemVal _t2614 = gem_v_node;
    GemVal _t2615[] = {gem_table_get(_t2614, gem_string("right"))};
    GemVal _t2616 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2616.fn(_t2616.env, _t2615, 1);
#line 865 "compiler/main.gem"
    GemVal _t2617 = gem_table_new();
    GemVal _t2618[] = {gem_v_lc};
    GemVal _t2619 = gem_v_rc_r;
    GemVal _t2620[] = {gem_table_get(_t2619, gem_string("expr"))};
    gem_table_set(_t2617, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t2618, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2620, 1)), gem_string(")")));
    GemVal _t2621 = gem_v_rc_r;
    gem_table_set(_t2617, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2621, gem_string("setup"))));
                                            GemVal _t2622 = _t2617;
                                            gem_pop_frame();
                                            return _t2622;
                                        } else {
#line 866 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 867 "compiler/main.gem"
    GemVal _t2623 = gem_v_node;
    GemVal _t2624[] = {gem_table_get(_t2623, gem_string("right"))};
    GemVal _t2625 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2625.fn(_t2625.env, _t2624, 1);
#line 868 "compiler/main.gem"
    GemVal _t2626 = gem_table_new();
    GemVal _t2627[] = {gem_v_lc};
    GemVal _t2628 = gem_v_rc_r;
    GemVal _t2629[] = {gem_table_get(_t2628, gem_string("expr"))};
    gem_table_set(_t2626, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t2627, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2629, 1)), gem_string(")")));
    GemVal _t2630 = gem_v_rc_r;
    gem_table_set(_t2626, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2630, gem_string("setup"))));
                                                GemVal _t2631 = _t2626;
                                                gem_pop_frame();
                                                return _t2631;
                                            } else {
#line 869 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 870 "compiler/main.gem"
    GemVal _t2632 = gem_v_node;
    GemVal _t2633[] = {gem_table_get(_t2632, gem_string("right"))};
    GemVal _t2634 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t2634.fn(_t2634.env, _t2633, 1);
#line 871 "compiler/main.gem"
    GemVal _t2635 = gem_table_new();
    GemVal _t2636[] = {gem_v_lc};
    GemVal _t2637 = gem_v_rc_r;
    GemVal _t2638[] = {gem_table_get(_t2637, gem_string("expr"))};
    gem_table_set(_t2635, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t2636, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2638, 1)), gem_string(")")));
    GemVal _t2639 = gem_v_rc_r;
    gem_table_set(_t2635, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2639, gem_string("setup"))));
                                                    GemVal _t2640 = _t2635;
                                                    gem_pop_frame();
                                                    return _t2640;
                                                } else {
#line 872 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 873 "compiler/main.gem"
    GemVal _t2641 = gem_v_node;
    GemVal _t2642[] = {gem_table_get(_t2641, gem_string("right"))};
    GemVal _t2643 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t2643.fn(_t2643.env, _t2642, 1);
#line 874 "compiler/main.gem"
    GemVal _t2644 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t2644.fn(_t2644.env, NULL, 0);
#line 875 "compiler/main.gem"
                                                        GemVal gem_v_setup = gem_v_ls;
#line 876 "compiler/main.gem"
    GemVal _t2645[] = {gem_v_t};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2645, 1)), gem_string(";\n")));
#line 877 "compiler/main.gem"
    GemVal _t2646[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t2646, 1)), gem_string(")) {\n")));
#line 878 "compiler/main.gem"
    GemVal _t2647[] = {gem_v_t};
    GemVal _t2648[] = {gem_v_lc};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2647, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2648, 1)), gem_string(";\n")));
#line 879 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 880 "compiler/main.gem"
    GemVal _t2649 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t2649, gem_string("setup")), gem_string("")))) {
#line 881 "compiler/main.gem"
    GemVal _t2650 = gem_v_rc_r;
    GemVal _t2651[] = {gem_table_get(_t2650, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2651, 3));
                                                        }
#line 883 "compiler/main.gem"
    GemVal _t2652[] = {gem_v_t};
    GemVal _t2653 = gem_v_rc_r;
    GemVal _t2654[] = {gem_table_get(_t2653, gem_string("expr"))};
                                                        gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2652, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2654, 1)), gem_string(";\n")));
#line 884 "compiler/main.gem"
                                                        gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 885 "compiler/main.gem"
    GemVal _t2655 = gem_table_new();
    gem_table_set(_t2655, gem_string("expr"), gem_v_t);
    gem_table_set(_t2655, gem_string("setup"), gem_v_setup);
                                                        GemVal _t2656 = _t2655;
                                                        gem_pop_frame();
                                                        return _t2656;
                                                    } else {
#line 886 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 887 "compiler/main.gem"
    GemVal _t2657 = gem_v_node;
    GemVal _t2658[] = {gem_table_get(_t2657, gem_string("right"))};
    GemVal _t2659 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t2659.fn(_t2659.env, _t2658, 1);
#line 888 "compiler/main.gem"
    GemVal _t2660 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t2660.fn(_t2660.env, NULL, 0);
#line 889 "compiler/main.gem"
                                                            GemVal gem_v_setup = gem_v_ls;
#line 890 "compiler/main.gem"
    GemVal _t2661[] = {gem_v_t};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2661, 1)), gem_string(";\n")));
#line 891 "compiler/main.gem"
    GemVal _t2662[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t2662, 1)), gem_string(")) {\n")));
#line 892 "compiler/main.gem"
    GemVal _t2663[] = {gem_v_t};
    GemVal _t2664[] = {gem_v_lc};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2663, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2664, 1)), gem_string(";\n")));
#line 893 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    } else {\n"));
#line 894 "compiler/main.gem"
    GemVal _t2665 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2665, gem_string("setup")), gem_string("")))) {
#line 895 "compiler/main.gem"
    GemVal _t2666 = gem_v_rc_r;
    GemVal _t2667[] = {gem_table_get(_t2666, gem_string("setup")), gem_string("    "), gem_string("        ")};
                                                                gem_v_setup = gem_add(gem_v_setup, gem_str_replace_fn(NULL, _t2667, 3));
                                                            }
#line 897 "compiler/main.gem"
    GemVal _t2668[] = {gem_v_t};
    GemVal _t2669 = gem_v_rc_r;
    GemVal _t2670[] = {gem_table_get(_t2669, gem_string("expr"))};
                                                            gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t2668, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2670, 1)), gem_string(";\n")));
#line 898 "compiler/main.gem"
                                                            gem_v_setup = gem_add(gem_v_setup, gem_string("    }\n"));
#line 899 "compiler/main.gem"
    GemVal _t2671 = gem_table_new();
    gem_table_set(_t2671, gem_string("expr"), gem_v_t);
    gem_table_set(_t2671, gem_string("setup"), gem_v_setup);
                                                            GemVal _t2672 = _t2671;
                                                            gem_pop_frame();
                                                            return _t2672;
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
#line 902 "compiler/main.gem"
    GemVal _t2673[] = {gem_v_op};
    GemVal _t2674[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t2673, 1))};
    GemVal _t2675 = gem_error_at_fn("compiler/main.gem", 902, _t2674, 1);
    gem_pop_frame();
    return _t2675;
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
#line 908 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 909 "compiler/main.gem"
    GemVal gem_v__for_items_50 = gem_v_stmts;
#line 909 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 909 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2677[] = {gem_v__for_items_50};
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_len_fn(NULL, _t2677, 1)))) break;
#line 909 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_50, gem_v__for_i_50);
#line 909 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 910 "compiler/main.gem"
    GemVal _t2678[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2679 = (*gem_v_compile_stmt);
        gem_v_out = gem_add(gem_add(gem_v_out, _t2679.fn(_t2679.env, _t2678, 2)), gem_string("\n"));
    }

    GemVal _t2680 = gem_v_out;
    gem_pop_frame();
    return _t2680;
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
#line 918 "compiler/main.gem"
    GemVal _t2682[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2682, 1);
#line 919 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 920 "compiler/main.gem"
    GemVal _t2683 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2683, gem_string("line")), GEM_NIL))) {
#line 921 "compiler/main.gem"
    GemVal _t2684 = gem_v_node;
    GemVal _t2685[] = {gem_table_get(_t2684, gem_string("line"))};
    GemVal _t2686[] = {(*gem_v_source_name)};
    GemVal _t2687[] = {gem_fn_escape_c_string(NULL, _t2686, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2685, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2687, 1)), gem_string("\"\n"));
    }
#line 924 "compiler/main.gem"
    GemVal gem_v__match_51 = gem_v_node;
    GemVal _t2688[] = {gem_v__match_51};
    GemVal _t2690;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2688, 1), gem_string("table")))) {
        _t2690 = gem_eq(gem_type_fn(NULL, _t2688, 1), gem_string("table"));
    } else {
        GemVal _t2689[] = {gem_v__match_51, gem_string("tag")};
        _t2690 = gem_has_key_fn(NULL, _t2689, 2);
    }
    GemVal _t2691;
    if (!gem_truthy(_t2690)) {
        _t2691 = _t2690;
    } else {
        _t2691 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2693;
    if (!gem_truthy(_t2691)) {
        _t2693 = _t2691;
    } else {
        GemVal _t2692[] = {gem_v__match_51, gem_string("name")};
        _t2693 = gem_has_key_fn(NULL, _t2692, 2);
    }
    GemVal _t2695;
    if (!gem_truthy(_t2693)) {
        _t2695 = _t2693;
    } else {
        GemVal _t2694[] = {gem_v__match_51, gem_string("value")};
        _t2695 = gem_has_key_fn(NULL, _t2694, 2);
    }
    if (gem_truthy(_t2695)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_51, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 926 "compiler/main.gem"
    GemVal _t2696[] = {gem_v_value};
    GemVal _t2697 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2697.fn(_t2697.env, _t2696, 1);
#line 927 "compiler/main.gem"
    GemVal _t2698[] = {gem_v_name};
    GemVal _t2699 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2699.fn(_t2699.env, _t2698, 1);
#line 928 "compiler/main.gem"
    GemVal _t2701;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t2701 = (*gem_v_in_top_level);
    } else {
        GemVal _t2700[] = {(*gem_v_top_level_vars), gem_v_name};
        _t2701 = gem_fn_set_contains(NULL, _t2700, 2);
    }
        if (gem_truthy(_t2701)) {
#line 929 "compiler/main.gem"
    GemVal _t2702[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn_set_contains(NULL, _t2702, 2))) {
#line 930 "compiler/main.gem"
    GemVal _t2703[] = {gem_v_line_dir};
    GemVal _t2704 = gem_v_r;
    GemVal _t2705[] = {gem_table_get(_t2704, gem_string("setup"))};
    GemVal _t2706[] = {gem_v_p};
    GemVal _t2707[] = {gem_v_mname};
    GemVal _t2708[] = {gem_v_p};
    GemVal _t2709[] = {gem_v_mname};
    GemVal _t2710 = gem_v_r;
    GemVal _t2711[] = {gem_table_get(_t2710, gem_string("expr"))};
                GemVal _t2712 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2703, 1), gem_to_string_fn(NULL, _t2705, 1)), gem_to_string_fn(NULL, _t2706, 1)), gem_to_string_fn(NULL, _t2707, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2708, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2709, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2711, 1)), gem_string(";"));
                gem_pop_frame();
                return _t2712;
            }
#line 932 "compiler/main.gem"
    GemVal _t2713[] = {gem_v_line_dir};
    GemVal _t2714 = gem_v_r;
    GemVal _t2715[] = {gem_table_get(_t2714, gem_string("setup"))};
    GemVal _t2716[] = {gem_v_p};
    GemVal _t2717[] = {gem_v_mname};
    GemVal _t2718 = gem_v_r;
    GemVal _t2719[] = {gem_table_get(_t2718, gem_string("expr"))};
            GemVal _t2720 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2713, 1), gem_to_string_fn(NULL, _t2715, 1)), gem_to_string_fn(NULL, _t2716, 1)), gem_to_string_fn(NULL, _t2717, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2719, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2720;
        }
#line 934 "compiler/main.gem"
    GemVal _t2721[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2721, 2))) {
#line 935 "compiler/main.gem"
    GemVal _t2722[] = {gem_v_line_dir};
    GemVal _t2723 = gem_v_r;
    GemVal _t2724[] = {gem_table_get(_t2723, gem_string("setup"))};
    GemVal _t2725[] = {gem_v_p};
    GemVal _t2726[] = {gem_v_mname};
    GemVal _t2727[] = {gem_v_p};
    GemVal _t2728[] = {gem_v_mname};
    GemVal _t2729 = gem_v_r;
    GemVal _t2730[] = {gem_table_get(_t2729, gem_string("expr"))};
            GemVal _t2731 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2722, 1), gem_to_string_fn(NULL, _t2724, 1)), gem_to_string_fn(NULL, _t2725, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2726, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2727, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2728, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2730, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2731;
        }
#line 937 "compiler/main.gem"
    GemVal _t2732[] = {gem_v_line_dir};
    GemVal _t2733 = gem_v_r;
    GemVal _t2734[] = {gem_table_get(_t2733, gem_string("setup"))};
    GemVal _t2735[] = {gem_v_p};
    GemVal _t2736[] = {gem_v_mname};
    GemVal _t2737 = gem_v_r;
    GemVal _t2738[] = {gem_table_get(_t2737, gem_string("expr"))};
        GemVal _t2739 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2732, 1), gem_to_string_fn(NULL, _t2734, 1)), gem_to_string_fn(NULL, _t2735, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2736, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2738, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2739;
    } else {
    GemVal _t2740[] = {gem_v__match_51};
    GemVal _t2742;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2740, 1), gem_string("table")))) {
        _t2742 = gem_eq(gem_type_fn(NULL, _t2740, 1), gem_string("table"));
    } else {
        GemVal _t2741[] = {gem_v__match_51, gem_string("tag")};
        _t2742 = gem_has_key_fn(NULL, _t2741, 2);
    }
    GemVal _t2743;
    if (!gem_truthy(_t2742)) {
        _t2743 = _t2742;
    } else {
        _t2743 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2745;
    if (!gem_truthy(_t2743)) {
        _t2745 = _t2743;
    } else {
        GemVal _t2744[] = {gem_v__match_51, gem_string("name")};
        _t2745 = gem_has_key_fn(NULL, _t2744, 2);
    }
    GemVal _t2747;
    if (!gem_truthy(_t2745)) {
        _t2747 = _t2745;
    } else {
        GemVal _t2746[] = {gem_v__match_51, gem_string("value")};
        _t2747 = gem_has_key_fn(NULL, _t2746, 2);
    }
    if (gem_truthy(_t2747)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_51, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 939 "compiler/main.gem"
    GemVal _t2748[] = {gem_v_value};
    GemVal _t2749 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2749.fn(_t2749.env, _t2748, 1);
#line 940 "compiler/main.gem"
    GemVal _t2750[] = {gem_v_name};
    GemVal _t2751 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2751.fn(_t2751.env, _t2750, 1);
#line 941 "compiler/main.gem"
    GemVal _t2752[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2754;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2752, 2))) {
        _t2754 = gem_fn_set_contains(NULL, _t2752, 2);
    } else {
        GemVal _t2753[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2754 = gem_fn_set_contains(NULL, _t2753, 2);
    }
        if (gem_truthy(_t2754)) {
#line 942 "compiler/main.gem"
    GemVal _t2755[] = {gem_v_line_dir};
    GemVal _t2756 = gem_v_r;
    GemVal _t2757[] = {gem_table_get(_t2756, gem_string("setup"))};
    GemVal _t2758[] = {gem_v_p};
    GemVal _t2759[] = {gem_v_mname};
    GemVal _t2760 = gem_v_r;
    GemVal _t2761[] = {gem_table_get(_t2760, gem_string("expr"))};
            GemVal _t2762 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2755, 1), gem_to_string_fn(NULL, _t2757, 1)), gem_to_string_fn(NULL, _t2758, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2759, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2761, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2762;
        }
#line 944 "compiler/main.gem"
    GemVal _t2763[] = {gem_v_line_dir};
    GemVal _t2764 = gem_v_r;
    GemVal _t2765[] = {gem_table_get(_t2764, gem_string("setup"))};
    GemVal _t2766[] = {gem_v_p};
    GemVal _t2767[] = {gem_v_mname};
    GemVal _t2768 = gem_v_r;
    GemVal _t2769[] = {gem_table_get(_t2768, gem_string("expr"))};
        GemVal _t2770 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2763, 1), gem_to_string_fn(NULL, _t2765, 1)), gem_to_string_fn(NULL, _t2766, 1)), gem_to_string_fn(NULL, _t2767, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2769, 1)), gem_string(";"));
        gem_pop_frame();
        return _t2770;
    } else {
    GemVal _t2771[] = {gem_v__match_51};
    GemVal _t2773;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2771, 1), gem_string("table")))) {
        _t2773 = gem_eq(gem_type_fn(NULL, _t2771, 1), gem_string("table"));
    } else {
        GemVal _t2772[] = {gem_v__match_51, gem_string("tag")};
        _t2773 = gem_has_key_fn(NULL, _t2772, 2);
    }
    GemVal _t2774;
    if (!gem_truthy(_t2773)) {
        _t2774 = _t2773;
    } else {
        _t2774 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t2776;
    if (!gem_truthy(_t2774)) {
        _t2776 = _t2774;
    } else {
        GemVal _t2775[] = {gem_v__match_51, gem_string("object")};
        _t2776 = gem_has_key_fn(NULL, _t2775, 2);
    }
    GemVal _t2778;
    if (!gem_truthy(_t2776)) {
        _t2778 = _t2776;
    } else {
        GemVal _t2777[] = {gem_v__match_51, gem_string("value")};
        _t2778 = gem_has_key_fn(NULL, _t2777, 2);
    }
    GemVal _t2780;
    if (!gem_truthy(_t2778)) {
        _t2780 = _t2778;
    } else {
        GemVal _t2779[] = {gem_v__match_51, gem_string("field")};
        _t2780 = gem_has_key_fn(NULL, _t2779, 2);
    }
    if (gem_truthy(_t2780)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_51, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_51, gem_string("field"));
#line 946 "compiler/main.gem"
    GemVal _t2781[] = {gem_v_object};
    GemVal _t2782 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2782.fn(_t2782.env, _t2781, 1);
#line 947 "compiler/main.gem"
    GemVal _t2783[] = {gem_v_value};
    GemVal _t2784 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2784.fn(_t2784.env, _t2783, 1);
#line 948 "compiler/main.gem"
    GemVal _t2785[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn_escape_c_string(NULL, _t2785, 1);
#line 949 "compiler/main.gem"
    GemVal _t2786[] = {gem_v_line_dir};
    GemVal _t2787 = gem_v_obj_r;
    GemVal _t2788[] = {gem_table_get(_t2787, gem_string("setup"))};
    GemVal _t2789 = gem_v_val_r;
    GemVal _t2790[] = {gem_table_get(_t2789, gem_string("setup"))};
    GemVal _t2791[] = {gem_v_p};
    GemVal _t2792 = gem_v_obj_r;
    GemVal _t2793[] = {gem_table_get(_t2792, gem_string("expr"))};
    GemVal _t2794[] = {gem_v_escaped};
    GemVal _t2795 = gem_v_val_r;
    GemVal _t2796[] = {gem_table_get(_t2795, gem_string("expr"))};
        GemVal _t2797 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2786, 1), gem_to_string_fn(NULL, _t2788, 1)), gem_to_string_fn(NULL, _t2790, 1)), gem_to_string_fn(NULL, _t2791, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2793, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2794, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2796, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2797;
    } else {
    GemVal _t2798[] = {gem_v__match_51};
    GemVal _t2800;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2798, 1), gem_string("table")))) {
        _t2800 = gem_eq(gem_type_fn(NULL, _t2798, 1), gem_string("table"));
    } else {
        GemVal _t2799[] = {gem_v__match_51, gem_string("tag")};
        _t2800 = gem_has_key_fn(NULL, _t2799, 2);
    }
    GemVal _t2801;
    if (!gem_truthy(_t2800)) {
        _t2801 = _t2800;
    } else {
        _t2801 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t2803;
    if (!gem_truthy(_t2801)) {
        _t2803 = _t2801;
    } else {
        GemVal _t2802[] = {gem_v__match_51, gem_string("object")};
        _t2803 = gem_has_key_fn(NULL, _t2802, 2);
    }
    GemVal _t2805;
    if (!gem_truthy(_t2803)) {
        _t2805 = _t2803;
    } else {
        GemVal _t2804[] = {gem_v__match_51, gem_string("key")};
        _t2805 = gem_has_key_fn(NULL, _t2804, 2);
    }
    GemVal _t2807;
    if (!gem_truthy(_t2805)) {
        _t2807 = _t2805;
    } else {
        GemVal _t2806[] = {gem_v__match_51, gem_string("value")};
        _t2807 = gem_has_key_fn(NULL, _t2806, 2);
    }
    if (gem_truthy(_t2807)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_51, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_51, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 951 "compiler/main.gem"
    GemVal _t2808[] = {gem_v_object};
    GemVal _t2809 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2809.fn(_t2809.env, _t2808, 1);
#line 952 "compiler/main.gem"
    GemVal _t2810[] = {gem_v_key};
    GemVal _t2811 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2811.fn(_t2811.env, _t2810, 1);
#line 953 "compiler/main.gem"
    GemVal _t2812[] = {gem_v_value};
    GemVal _t2813 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2813.fn(_t2813.env, _t2812, 1);
#line 954 "compiler/main.gem"
    GemVal _t2814[] = {gem_v_line_dir};
    GemVal _t2815 = gem_v_obj_r;
    GemVal _t2816[] = {gem_table_get(_t2815, gem_string("setup"))};
    GemVal _t2817 = gem_v_key_r;
    GemVal _t2818[] = {gem_table_get(_t2817, gem_string("setup"))};
    GemVal _t2819 = gem_v_val_r;
    GemVal _t2820[] = {gem_table_get(_t2819, gem_string("setup"))};
    GemVal _t2821[] = {gem_v_p};
    GemVal _t2822 = gem_v_obj_r;
    GemVal _t2823[] = {gem_table_get(_t2822, gem_string("expr"))};
    GemVal _t2824 = gem_v_key_r;
    GemVal _t2825[] = {gem_table_get(_t2824, gem_string("expr"))};
    GemVal _t2826 = gem_v_val_r;
    GemVal _t2827[] = {gem_table_get(_t2826, gem_string("expr"))};
        GemVal _t2828 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2814, 1), gem_to_string_fn(NULL, _t2816, 1)), gem_to_string_fn(NULL, _t2818, 1)), gem_to_string_fn(NULL, _t2820, 1)), gem_to_string_fn(NULL, _t2821, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t2823, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2825, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2827, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2828;
    } else {
    GemVal _t2829[] = {gem_v__match_51};
    GemVal _t2831;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2829, 1), gem_string("table")))) {
        _t2831 = gem_eq(gem_type_fn(NULL, _t2829, 1), gem_string("table"));
    } else {
        GemVal _t2830[] = {gem_v__match_51, gem_string("tag")};
        _t2831 = gem_has_key_fn(NULL, _t2830, 2);
    }
    GemVal _t2832;
    if (!gem_truthy(_t2831)) {
        _t2832 = _t2831;
    } else {
        _t2832 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t2832)) {
#line 956 "compiler/main.gem"
    GemVal _t2833[] = {gem_v_line_dir};
    GemVal _t2834[] = {gem_v_node, gem_v_indent};
    GemVal _t2835 = (*gem_v_compile_if);
        GemVal _t2836 = gem_add(gem_to_string_fn(NULL, _t2833, 1), _t2835.fn(_t2835.env, _t2834, 2));
        gem_pop_frame();
        return _t2836;
    } else {
    GemVal _t2837[] = {gem_v__match_51};
    GemVal _t2839;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2837, 1), gem_string("table")))) {
        _t2839 = gem_eq(gem_type_fn(NULL, _t2837, 1), gem_string("table"));
    } else {
        GemVal _t2838[] = {gem_v__match_51, gem_string("tag")};
        _t2839 = gem_has_key_fn(NULL, _t2838, 2);
    }
    GemVal _t2840;
    if (!gem_truthy(_t2839)) {
        _t2840 = _t2839;
    } else {
        _t2840 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t2840)) {
#line 958 "compiler/main.gem"
    GemVal _t2841[] = {gem_v_line_dir};
    GemVal _t2842[] = {gem_v_node, gem_v_indent};
    GemVal _t2843 = (*gem_v_compile_while);
        GemVal _t2844 = gem_add(gem_to_string_fn(NULL, _t2841, 1), _t2843.fn(_t2843.env, _t2842, 2));
        gem_pop_frame();
        return _t2844;
    } else {
    GemVal _t2845[] = {gem_v__match_51};
    GemVal _t2847;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2845, 1), gem_string("table")))) {
        _t2847 = gem_eq(gem_type_fn(NULL, _t2845, 1), gem_string("table"));
    } else {
        GemVal _t2846[] = {gem_v__match_51, gem_string("tag")};
        _t2847 = gem_has_key_fn(NULL, _t2846, 2);
    }
    GemVal _t2848;
    if (!gem_truthy(_t2847)) {
        _t2848 = _t2847;
    } else {
        _t2848 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t2848)) {
#line 960 "compiler/main.gem"
    GemVal _t2849[] = {gem_v_line_dir};
    GemVal _t2850[] = {gem_v_node, gem_v_indent};
    GemVal _t2851 = (*gem_v_compile_match);
        GemVal _t2852 = gem_add(gem_to_string_fn(NULL, _t2849, 1), _t2851.fn(_t2851.env, _t2850, 2));
        gem_pop_frame();
        return _t2852;
    } else {
    GemVal _t2853[] = {gem_v__match_51};
    GemVal _t2855;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2853, 1), gem_string("table")))) {
        _t2855 = gem_eq(gem_type_fn(NULL, _t2853, 1), gem_string("table"));
    } else {
        GemVal _t2854[] = {gem_v__match_51, gem_string("tag")};
        _t2855 = gem_has_key_fn(NULL, _t2854, 2);
    }
    GemVal _t2856;
    if (!gem_truthy(_t2855)) {
        _t2856 = _t2855;
    } else {
        _t2856 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t2856)) {
#line 962 "compiler/main.gem"
    GemVal _t2857[] = {gem_v_line_dir};
    GemVal _t2858[] = {gem_v_node, gem_v_indent};
    GemVal _t2859 = (*gem_v_compile_receive_match);
        GemVal _t2860 = gem_add(gem_to_string_fn(NULL, _t2857, 1), _t2859.fn(_t2859.env, _t2858, 2));
        gem_pop_frame();
        return _t2860;
    } else {
    GemVal _t2861[] = {gem_v__match_51};
    GemVal _t2863;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2861, 1), gem_string("table")))) {
        _t2863 = gem_eq(gem_type_fn(NULL, _t2861, 1), gem_string("table"));
    } else {
        GemVal _t2862[] = {gem_v__match_51, gem_string("tag")};
        _t2863 = gem_has_key_fn(NULL, _t2862, 2);
    }
    GemVal _t2864;
    if (!gem_truthy(_t2863)) {
        _t2864 = _t2863;
    } else {
        _t2864 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t2864)) {
#line 964 "compiler/main.gem"
        GemVal _t2865 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t2865;
    } else {
    GemVal _t2866[] = {gem_v__match_51};
    GemVal _t2868;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2866, 1), gem_string("table")))) {
        _t2868 = gem_eq(gem_type_fn(NULL, _t2866, 1), gem_string("table"));
    } else {
        GemVal _t2867[] = {gem_v__match_51, gem_string("tag")};
        _t2868 = gem_has_key_fn(NULL, _t2867, 2);
    }
    GemVal _t2869;
    if (!gem_truthy(_t2868)) {
        _t2869 = _t2868;
    } else {
        _t2869 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t2869)) {
#line 966 "compiler/main.gem"
        GemVal _t2870 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t2870;
    } else {
    GemVal _t2871[] = {gem_v__match_51};
    GemVal _t2873;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2871, 1), gem_string("table")))) {
        _t2873 = gem_eq(gem_type_fn(NULL, _t2871, 1), gem_string("table"));
    } else {
        GemVal _t2872[] = {gem_v__match_51, gem_string("tag")};
        _t2873 = gem_has_key_fn(NULL, _t2872, 2);
    }
    GemVal _t2874;
    if (!gem_truthy(_t2873)) {
        _t2874 = _t2873;
    } else {
        _t2874 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2876;
    if (!gem_truthy(_t2874)) {
        _t2876 = _t2874;
    } else {
        GemVal _t2875[] = {gem_v__match_51, gem_string("value")};
        _t2876 = gem_has_key_fn(NULL, _t2875, 2);
    }
    if (gem_truthy(_t2876)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_51, gem_string("value"));
#line 968 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 969 "compiler/main.gem"
    GemVal _t2877[] = {gem_v_value};
    GemVal _t2878 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2878.fn(_t2878.env, _t2877, 1);
#line 970 "compiler/main.gem"
    GemVal _t2879 = (*gem_v_tmp);
            GemVal gem_v_t = _t2879.fn(_t2879.env, NULL, 0);
#line 971 "compiler/main.gem"
    GemVal _t2880[] = {gem_v_line_dir};
    GemVal _t2881 = gem_v_r;
    GemVal _t2882[] = {gem_table_get(_t2881, gem_string("setup"))};
    GemVal _t2883[] = {gem_v_p};
    GemVal _t2884[] = {gem_v_t};
    GemVal _t2885 = gem_v_r;
    GemVal _t2886[] = {gem_table_get(_t2885, gem_string("expr"))};
    GemVal _t2887[] = {gem_v_p};
    GemVal _t2888[] = {gem_v_p};
    GemVal _t2889[] = {gem_v_t};
            GemVal _t2890 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2880, 1), gem_to_string_fn(NULL, _t2882, 1)), gem_to_string_fn(NULL, _t2883, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2884, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2886, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2887, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2888, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t2889, 1)), gem_string(";"));
            gem_pop_frame();
            return _t2890;
        }
#line 973 "compiler/main.gem"
    GemVal _t2891[] = {gem_v_line_dir};
    GemVal _t2892[] = {gem_v_p};
    GemVal _t2893[] = {gem_v_p};
        GemVal _t2894 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2891, 1), gem_to_string_fn(NULL, _t2892, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2893, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2894;
    } else {
    GemVal _t2895[] = {gem_v__match_51};
    GemVal _t2897;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2895, 1), gem_string("table")))) {
        _t2897 = gem_eq(gem_type_fn(NULL, _t2895, 1), gem_string("table"));
    } else {
        GemVal _t2896[] = {gem_v__match_51, gem_string("tag")};
        _t2897 = gem_has_key_fn(NULL, _t2896, 2);
    }
    GemVal _t2898;
    if (!gem_truthy(_t2897)) {
        _t2898 = _t2897;
    } else {
        _t2898 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2898)) {
#line 975 "compiler/main.gem"
    GemVal _t2899[] = {gem_v_node};
    GemVal _t2900 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2900.fn(_t2900.env, _t2899, 1);
#line 976 "compiler/main.gem"
    GemVal _t2901[] = {gem_v_line_dir};
    GemVal _t2902 = gem_v_r;
    GemVal _t2903[] = {gem_table_get(_t2902, gem_string("setup"))};
    GemVal _t2904[] = {gem_v_p};
    GemVal _t2905 = gem_v_r;
    GemVal _t2906[] = {gem_table_get(_t2905, gem_string("expr"))};
        GemVal _t2907 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2901, 1), gem_to_string_fn(NULL, _t2903, 1)), gem_to_string_fn(NULL, _t2904, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2906, 1)), gem_string(");"));
        gem_pop_frame();
        return _t2907;
    } else {
    GemVal _t2908[] = {gem_v__match_51};
    GemVal _t2910;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2908, 1), gem_string("table")))) {
        _t2910 = gem_eq(gem_type_fn(NULL, _t2908, 1), gem_string("table"));
    } else {
        GemVal _t2909[] = {gem_v__match_51, gem_string("tag")};
        _t2910 = gem_has_key_fn(NULL, _t2909, 2);
    }
    GemVal _t2911;
    if (!gem_truthy(_t2910)) {
        _t2911 = _t2910;
    } else {
        _t2911 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2911)) {
#line 978 "compiler/main.gem"
        GemVal _t2912 = gem_string("");
        gem_pop_frame();
        return _t2912;
    } else {
    GemVal _t2913[] = {gem_v__match_51};
    GemVal _t2915;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2913, 1), gem_string("table")))) {
        _t2915 = gem_eq(gem_type_fn(NULL, _t2913, 1), gem_string("table"));
    } else {
        GemVal _t2914[] = {gem_v__match_51, gem_string("tag")};
        _t2915 = gem_has_key_fn(NULL, _t2914, 2);
    }
    GemVal _t2916;
    if (!gem_truthy(_t2915)) {
        _t2916 = _t2915;
    } else {
        _t2916 = gem_eq(gem_table_get(gem_v__match_51, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2918;
    if (!gem_truthy(_t2916)) {
        _t2918 = _t2916;
    } else {
        GemVal _t2917[] = {gem_v__match_51, gem_string("stmts")};
        _t2918 = gem_has_key_fn(NULL, _t2917, 2);
    }
    if (gem_truthy(_t2918)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_51, gem_string("stmts"));
#line 980 "compiler/main.gem"
        GemVal gem_v_out = gem_string("");
#line 981 "compiler/main.gem"
        GemVal gem_v__for_items_52 = gem_v_stmts;
#line 981 "compiler/main.gem"
        GemVal gem_v__for_i_52 = gem_int(0);
#line 981 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t2919[] = {gem_v__for_items_52};
            if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_len_fn(NULL, _t2919, 1)))) break;
#line 981 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_52, gem_v__for_i_52);
#line 981 "compiler/main.gem"
            gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 982 "compiler/main.gem"
    GemVal _t2920[] = {gem_v_stmt, gem_v_indent};
    GemVal _t2921 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t2921.fn(_t2921.env, _t2920, 2)), gem_string("\n"));
        }

#line 984 "compiler/main.gem"
        GemVal _t2922 = gem_v_out;
        gem_pop_frame();
        return _t2922;
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
#line 987 "compiler/main.gem"
    GemVal _t2923[] = {gem_v_node};
    GemVal _t2924 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t2924.fn(_t2924.env, _t2923, 1);
    GemVal _t2925[] = {gem_v_line_dir};
    GemVal _t2926 = gem_v_r;
    GemVal _t2927[] = {gem_table_get(_t2926, gem_string("setup"))};
    GemVal _t2928[] = {gem_v_p};
    GemVal _t2929 = gem_v_r;
    GemVal _t2930[] = {gem_table_get(_t2929, gem_string("expr"))};
    GemVal _t2931 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2925, 1), gem_to_string_fn(NULL, _t2927, 1)), gem_to_string_fn(NULL, _t2928, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t2930, 1)), gem_string(");"));
    gem_pop_frame();
    return _t2931;
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
#line 994 "compiler/main.gem"
    GemVal _t2933[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t2933, 1);
#line 995 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 996 "compiler/main.gem"
    GemVal _t2934 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2934, gem_string("line")), GEM_NIL))) {
#line 997 "compiler/main.gem"
    GemVal _t2935 = gem_v_node;
    GemVal _t2936[] = {gem_table_get(_t2935, gem_string("line"))};
    GemVal _t2937[] = {(*gem_v_source_name)};
    GemVal _t2938[] = {gem_fn_escape_c_string(NULL, _t2937, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t2936, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t2938, 1)), gem_string("\"\n"));
    }
#line 1000 "compiler/main.gem"
    GemVal gem_v__match_53 = gem_v_node;
    GemVal _t2939[] = {gem_v__match_53};
    GemVal _t2941;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2939, 1), gem_string("table")))) {
        _t2941 = gem_eq(gem_type_fn(NULL, _t2939, 1), gem_string("table"));
    } else {
        GemVal _t2940[] = {gem_v__match_53, gem_string("tag")};
        _t2941 = gem_has_key_fn(NULL, _t2940, 2);
    }
    GemVal _t2942;
    if (!gem_truthy(_t2941)) {
        _t2942 = _t2941;
    } else {
        _t2942 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2944;
    if (!gem_truthy(_t2942)) {
        _t2944 = _t2942;
    } else {
        GemVal _t2943[] = {gem_v__match_53, gem_string("name")};
        _t2944 = gem_has_key_fn(NULL, _t2943, 2);
    }
    GemVal _t2946;
    if (!gem_truthy(_t2944)) {
        _t2946 = _t2944;
    } else {
        GemVal _t2945[] = {gem_v__match_53, gem_string("value")};
        _t2946 = gem_has_key_fn(NULL, _t2945, 2);
    }
    if (gem_truthy(_t2946)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_53, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_53, gem_string("value"));
#line 1002 "compiler/main.gem"
    GemVal _t2947[] = {gem_v_value};
    GemVal _t2948 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2948.fn(_t2948.env, _t2947, 1);
#line 1003 "compiler/main.gem"
    GemVal _t2949[] = {gem_v_name};
    GemVal _t2950 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2950.fn(_t2950.env, _t2949, 1);
#line 1004 "compiler/main.gem"
    GemVal _t2951[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn_set_contains(NULL, _t2951, 2))) {
#line 1005 "compiler/main.gem"
    GemVal _t2952[] = {gem_v_line_dir};
    GemVal _t2953 = gem_v_r;
    GemVal _t2954[] = {gem_table_get(_t2953, gem_string("setup"))};
    GemVal _t2955[] = {gem_v_p};
    GemVal _t2956[] = {gem_v_mname};
    GemVal _t2957[] = {gem_v_p};
    GemVal _t2958[] = {gem_v_mname};
    GemVal _t2959 = gem_v_r;
    GemVal _t2960[] = {gem_table_get(_t2959, gem_string("expr"))};
    GemVal _t2961[] = {gem_v_p};
    GemVal _t2962[] = {gem_v_p};
            GemVal _t2963 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2952, 1), gem_to_string_fn(NULL, _t2954, 1)), gem_to_string_fn(NULL, _t2955, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t2956, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t2957, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2958, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2960, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2961, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2962, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t2963;
        }
#line 1007 "compiler/main.gem"
    GemVal _t2964[] = {gem_v_line_dir};
    GemVal _t2965 = gem_v_r;
    GemVal _t2966[] = {gem_table_get(_t2965, gem_string("setup"))};
    GemVal _t2967[] = {gem_v_p};
    GemVal _t2968[] = {gem_v_mname};
    GemVal _t2969 = gem_v_r;
    GemVal _t2970[] = {gem_table_get(_t2969, gem_string("expr"))};
    GemVal _t2971[] = {gem_v_p};
    GemVal _t2972[] = {gem_v_p};
        GemVal _t2973 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2964, 1), gem_to_string_fn(NULL, _t2966, 1)), gem_to_string_fn(NULL, _t2967, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2968, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2970, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2971, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t2972, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t2973;
    } else {
    GemVal _t2974[] = {gem_v__match_53};
    GemVal _t2976;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2974, 1), gem_string("table")))) {
        _t2976 = gem_eq(gem_type_fn(NULL, _t2974, 1), gem_string("table"));
    } else {
        GemVal _t2975[] = {gem_v__match_53, gem_string("tag")};
        _t2976 = gem_has_key_fn(NULL, _t2975, 2);
    }
    GemVal _t2977;
    if (!gem_truthy(_t2976)) {
        _t2977 = _t2976;
    } else {
        _t2977 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2979;
    if (!gem_truthy(_t2977)) {
        _t2979 = _t2977;
    } else {
        GemVal _t2978[] = {gem_v__match_53, gem_string("name")};
        _t2979 = gem_has_key_fn(NULL, _t2978, 2);
    }
    GemVal _t2981;
    if (!gem_truthy(_t2979)) {
        _t2981 = _t2979;
    } else {
        GemVal _t2980[] = {gem_v__match_53, gem_string("value")};
        _t2981 = gem_has_key_fn(NULL, _t2980, 2);
    }
    if (gem_truthy(_t2981)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_53, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_53, gem_string("value"));
#line 1009 "compiler/main.gem"
    GemVal _t2982[] = {gem_v_value};
    GemVal _t2983 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2983.fn(_t2983.env, _t2982, 1);
#line 1010 "compiler/main.gem"
    GemVal _t2984 = (*gem_v_tmp);
        GemVal gem_v_t = _t2984.fn(_t2984.env, NULL, 0);
#line 1011 "compiler/main.gem"
    GemVal _t2985[] = {gem_v_name};
    GemVal _t2986 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2986.fn(_t2986.env, _t2985, 1);
#line 1012 "compiler/main.gem"
    GemVal _t2987[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2989;
    if (gem_truthy(gem_fn_set_contains(NULL, _t2987, 2))) {
        _t2989 = gem_fn_set_contains(NULL, _t2987, 2);
    } else {
        GemVal _t2988[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2989 = gem_fn_set_contains(NULL, _t2988, 2);
    }
        if (gem_truthy(_t2989)) {
#line 1013 "compiler/main.gem"
    GemVal _t2990[] = {gem_v_line_dir};
    GemVal _t2991 = gem_v_r;
    GemVal _t2992[] = {gem_table_get(_t2991, gem_string("setup"))};
    GemVal _t2993[] = {gem_v_p};
    GemVal _t2994[] = {gem_v_t};
    GemVal _t2995 = gem_v_r;
    GemVal _t2996[] = {gem_table_get(_t2995, gem_string("expr"))};
    GemVal _t2997[] = {gem_v_p};
    GemVal _t2998[] = {gem_v_mname};
    GemVal _t2999[] = {gem_v_t};
    GemVal _t3000[] = {gem_v_p};
    GemVal _t3001[] = {gem_v_p};
    GemVal _t3002[] = {gem_v_t};
            GemVal _t3003 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2990, 1), gem_to_string_fn(NULL, _t2992, 1)), gem_to_string_fn(NULL, _t2993, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2994, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2996, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t2997, 1)), gem_string("*")), gem_to_string_fn(NULL, _t2998, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2999, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3000, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3001, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3002, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3003;
        }
#line 1015 "compiler/main.gem"
    GemVal _t3004[] = {gem_v_line_dir};
    GemVal _t3005 = gem_v_r;
    GemVal _t3006[] = {gem_table_get(_t3005, gem_string("setup"))};
    GemVal _t3007[] = {gem_v_p};
    GemVal _t3008[] = {gem_v_t};
    GemVal _t3009 = gem_v_r;
    GemVal _t3010[] = {gem_table_get(_t3009, gem_string("expr"))};
    GemVal _t3011[] = {gem_v_p};
    GemVal _t3012[] = {gem_v_mname};
    GemVal _t3013[] = {gem_v_t};
    GemVal _t3014[] = {gem_v_p};
    GemVal _t3015[] = {gem_v_p};
    GemVal _t3016[] = {gem_v_t};
        GemVal _t3017 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3004, 1), gem_to_string_fn(NULL, _t3006, 1)), gem_to_string_fn(NULL, _t3007, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3008, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3010, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3011, 1)), gem_to_string_fn(NULL, _t3012, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3013, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3014, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3015, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3016, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3017;
    } else {
    GemVal _t3018[] = {gem_v__match_53};
    GemVal _t3020;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3018, 1), gem_string("table")))) {
        _t3020 = gem_eq(gem_type_fn(NULL, _t3018, 1), gem_string("table"));
    } else {
        GemVal _t3019[] = {gem_v__match_53, gem_string("tag")};
        _t3020 = gem_has_key_fn(NULL, _t3019, 2);
    }
    GemVal _t3021;
    if (!gem_truthy(_t3020)) {
        _t3021 = _t3020;
    } else {
        _t3021 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3023;
    if (!gem_truthy(_t3021)) {
        _t3023 = _t3021;
    } else {
        GemVal _t3022[] = {gem_v__match_53, gem_string("value")};
        _t3023 = gem_has_key_fn(NULL, _t3022, 2);
    }
    if (gem_truthy(_t3023)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_53, gem_string("value"));
#line 1017 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1018 "compiler/main.gem"
    GemVal _t3024[] = {gem_v_value};
    GemVal _t3025 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3025.fn(_t3025.env, _t3024, 1);
#line 1019 "compiler/main.gem"
    GemVal _t3026 = (*gem_v_tmp);
            GemVal gem_v_t = _t3026.fn(_t3026.env, NULL, 0);
#line 1020 "compiler/main.gem"
    GemVal _t3027[] = {gem_v_line_dir};
    GemVal _t3028 = gem_v_r;
    GemVal _t3029[] = {gem_table_get(_t3028, gem_string("setup"))};
    GemVal _t3030[] = {gem_v_p};
    GemVal _t3031[] = {gem_v_t};
    GemVal _t3032 = gem_v_r;
    GemVal _t3033[] = {gem_table_get(_t3032, gem_string("expr"))};
    GemVal _t3034[] = {gem_v_p};
    GemVal _t3035[] = {gem_v_p};
    GemVal _t3036[] = {gem_v_t};
            GemVal _t3037 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3027, 1), gem_to_string_fn(NULL, _t3029, 1)), gem_to_string_fn(NULL, _t3030, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3031, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3033, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3034, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3035, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3036, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3037;
        }
#line 1022 "compiler/main.gem"
    GemVal _t3038[] = {gem_v_line_dir};
    GemVal _t3039[] = {gem_v_p};
    GemVal _t3040[] = {gem_v_p};
        GemVal _t3041 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3038, 1), gem_to_string_fn(NULL, _t3039, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3040, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3041;
    } else {
    GemVal _t3042[] = {gem_v__match_53};
    GemVal _t3044;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3042, 1), gem_string("table")))) {
        _t3044 = gem_eq(gem_type_fn(NULL, _t3042, 1), gem_string("table"));
    } else {
        GemVal _t3043[] = {gem_v__match_53, gem_string("tag")};
        _t3044 = gem_has_key_fn(NULL, _t3043, 2);
    }
    GemVal _t3045;
    if (!gem_truthy(_t3044)) {
        _t3045 = _t3044;
    } else {
        _t3045 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3045)) {
#line 1024 "compiler/main.gem"
    GemVal _t3046[] = {gem_v_line_dir};
    GemVal _t3047[] = {gem_v_node, gem_v_indent};
    GemVal _t3048 = (*gem_v_compile_if_return);
        GemVal _t3049 = gem_add(gem_to_string_fn(NULL, _t3046, 1), _t3048.fn(_t3048.env, _t3047, 2));
        gem_pop_frame();
        return _t3049;
    } else {
    GemVal _t3050[] = {gem_v__match_53};
    GemVal _t3052;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3050, 1), gem_string("table")))) {
        _t3052 = gem_eq(gem_type_fn(NULL, _t3050, 1), gem_string("table"));
    } else {
        GemVal _t3051[] = {gem_v__match_53, gem_string("tag")};
        _t3052 = gem_has_key_fn(NULL, _t3051, 2);
    }
    GemVal _t3053;
    if (!gem_truthy(_t3052)) {
        _t3053 = _t3052;
    } else {
        _t3053 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3053)) {
#line 1026 "compiler/main.gem"
    GemVal _t3054[] = {gem_v_line_dir};
    GemVal _t3055[] = {gem_v_node, gem_v_indent};
    GemVal _t3056 = (*gem_v_compile_while);
    GemVal _t3057[] = {_t3056.fn(_t3056.env, _t3055, 2)};
    GemVal _t3058[] = {gem_v_p};
    GemVal _t3059[] = {gem_v_p};
        GemVal _t3060 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3054, 1), gem_to_string_fn(NULL, _t3057, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t3058, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3059, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3060;
    } else {
    GemVal _t3061[] = {gem_v__match_53};
    GemVal _t3063;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3061, 1), gem_string("table")))) {
        _t3063 = gem_eq(gem_type_fn(NULL, _t3061, 1), gem_string("table"));
    } else {
        GemVal _t3062[] = {gem_v__match_53, gem_string("tag")};
        _t3063 = gem_has_key_fn(NULL, _t3062, 2);
    }
    GemVal _t3064;
    if (!gem_truthy(_t3063)) {
        _t3064 = _t3063;
    } else {
        _t3064 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3064)) {
#line 1028 "compiler/main.gem"
    GemVal _t3065[] = {gem_v_line_dir};
    GemVal _t3066[] = {gem_v_node, gem_v_indent};
    GemVal _t3067 = (*gem_v_compile_match_return);
        GemVal _t3068 = gem_add(gem_to_string_fn(NULL, _t3065, 1), _t3067.fn(_t3067.env, _t3066, 2));
        gem_pop_frame();
        return _t3068;
    } else {
    GemVal _t3069[] = {gem_v__match_53};
    GemVal _t3071;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3069, 1), gem_string("table")))) {
        _t3071 = gem_eq(gem_type_fn(NULL, _t3069, 1), gem_string("table"));
    } else {
        GemVal _t3070[] = {gem_v__match_53, gem_string("tag")};
        _t3071 = gem_has_key_fn(NULL, _t3070, 2);
    }
    GemVal _t3072;
    if (!gem_truthy(_t3071)) {
        _t3072 = _t3071;
    } else {
        _t3072 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3072)) {
#line 1030 "compiler/main.gem"
    GemVal _t3073[] = {gem_v_line_dir};
    GemVal _t3074[] = {gem_v_node, gem_v_indent};
    GemVal _t3075 = (*gem_v_compile_receive_match_return);
        GemVal _t3076 = gem_add(gem_to_string_fn(NULL, _t3073, 1), _t3075.fn(_t3075.env, _t3074, 2));
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
        _t3080 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3082;
    if (!gem_truthy(_t3080)) {
        _t3082 = _t3080;
    } else {
        GemVal _t3081[] = {gem_v__match_53, gem_string("stmts")};
        _t3082 = gem_has_key_fn(NULL, _t3081, 2);
    }
    if (gem_truthy(_t3082)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_53, gem_string("stmts"));
#line 1032 "compiler/main.gem"
    GemVal _t3083[] = {gem_v_p};
        GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t3083, 1), gem_string("{\n"));
#line 1033 "compiler/main.gem"
        GemVal gem_v__for_i_54 = gem_int(0);
#line 1033 "compiler/main.gem"
    GemVal _t3084[] = {gem_v_stmts};
        GemVal gem_v__for_limit_54 = gem_sub(gem_len_fn(NULL, _t3084, 1), gem_int(1));
#line 1033 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_v__for_limit_54))) break;
#line 1033 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_54;
#line 1033 "compiler/main.gem"
            gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 1034 "compiler/main.gem"
    GemVal _t3085[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3086 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3086.fn(_t3086.env, _t3085, 2)), gem_string("\n"));
        }

#line 1036 "compiler/main.gem"
    GemVal _t3087[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3087, 1), gem_int(0)))) {
#line 1037 "compiler/main.gem"
    GemVal _t3088[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3089 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3089.fn(_t3089.env, _t3088, 2)), gem_string("\n"));
        }
#line 1039 "compiler/main.gem"
    GemVal _t3090[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3090, 1), gem_string("}")));
#line 1040 "compiler/main.gem"
        GemVal _t3091 = gem_v_out;
        gem_pop_frame();
        return _t3091;
    } else {
    GemVal _t3092[] = {gem_v__match_53};
    GemVal _t3094;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3092, 1), gem_string("table")))) {
        _t3094 = gem_eq(gem_type_fn(NULL, _t3092, 1), gem_string("table"));
    } else {
        GemVal _t3093[] = {gem_v__match_53, gem_string("tag")};
        _t3094 = gem_has_key_fn(NULL, _t3093, 2);
    }
    GemVal _t3095;
    if (!gem_truthy(_t3094)) {
        _t3095 = _t3094;
    } else {
        _t3095 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3095)) {
#line 1042 "compiler/main.gem"
    GemVal _t3096[] = {gem_v_node, gem_v_indent};
    GemVal _t3097 = (*gem_v_compile_stmt);
    GemVal _t3098[] = {_t3097.fn(_t3097.env, _t3096, 2)};
    GemVal _t3099[] = {gem_v_p};
    GemVal _t3100[] = {gem_v_p};
        GemVal _t3101 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3098, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3099, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3100, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3101;
    } else {
    GemVal _t3102[] = {gem_v__match_53};
    GemVal _t3104;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3102, 1), gem_string("table")))) {
        _t3104 = gem_eq(gem_type_fn(NULL, _t3102, 1), gem_string("table"));
    } else {
        GemVal _t3103[] = {gem_v__match_53, gem_string("tag")};
        _t3104 = gem_has_key_fn(NULL, _t3103, 2);
    }
    GemVal _t3105;
    if (!gem_truthy(_t3104)) {
        _t3105 = _t3104;
    } else {
        _t3105 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3105)) {
#line 1044 "compiler/main.gem"
    GemVal _t3106[] = {gem_v_node, gem_v_indent};
    GemVal _t3107 = (*gem_v_compile_stmt);
    GemVal _t3108[] = {_t3107.fn(_t3107.env, _t3106, 2)};
    GemVal _t3109[] = {gem_v_p};
    GemVal _t3110[] = {gem_v_p};
        GemVal _t3111 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3108, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3109, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3110, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3111;
    } else {
    GemVal _t3112[] = {gem_v__match_53};
    GemVal _t3114;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3112, 1), gem_string("table")))) {
        _t3114 = gem_eq(gem_type_fn(NULL, _t3112, 1), gem_string("table"));
    } else {
        GemVal _t3113[] = {gem_v__match_53, gem_string("tag")};
        _t3114 = gem_has_key_fn(NULL, _t3113, 2);
    }
    GemVal _t3115;
    if (!gem_truthy(_t3114)) {
        _t3115 = _t3114;
    } else {
        _t3115 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3115)) {
#line 1046 "compiler/main.gem"
    GemVal _t3116[] = {gem_v_node, gem_v_indent};
    GemVal _t3117 = (*gem_v_compile_stmt);
    GemVal _t3118[] = {_t3117.fn(_t3117.env, _t3116, 2)};
    GemVal _t3119[] = {gem_v_p};
    GemVal _t3120[] = {gem_v_p};
        GemVal _t3121 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3118, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3119, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3120, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3121;
    } else {
    GemVal _t3122[] = {gem_v__match_53};
    GemVal _t3124;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3122, 1), gem_string("table")))) {
        _t3124 = gem_eq(gem_type_fn(NULL, _t3122, 1), gem_string("table"));
    } else {
        GemVal _t3123[] = {gem_v__match_53, gem_string("tag")};
        _t3124 = gem_has_key_fn(NULL, _t3123, 2);
    }
    GemVal _t3125;
    if (!gem_truthy(_t3124)) {
        _t3125 = _t3124;
    } else {
        _t3125 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3125)) {
#line 1048 "compiler/main.gem"
    GemVal _t3126[] = {gem_v_node, gem_v_indent};
    GemVal _t3127 = (*gem_v_compile_stmt);
    GemVal _t3128[] = {_t3127.fn(_t3127.env, _t3126, 2)};
    GemVal _t3129[] = {gem_v_p};
    GemVal _t3130[] = {gem_v_p};
        GemVal _t3131 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3128, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3129, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3130, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3131;
    } else {
    GemVal _t3132[] = {gem_v__match_53};
    GemVal _t3134;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3132, 1), gem_string("table")))) {
        _t3134 = gem_eq(gem_type_fn(NULL, _t3132, 1), gem_string("table"));
    } else {
        GemVal _t3133[] = {gem_v__match_53, gem_string("tag")};
        _t3134 = gem_has_key_fn(NULL, _t3133, 2);
    }
    GemVal _t3135;
    if (!gem_truthy(_t3134)) {
        _t3135 = _t3134;
    } else {
        _t3135 = gem_eq(gem_table_get(gem_v__match_53, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3135)) {
#line 1050 "compiler/main.gem"
    GemVal _t3136[] = {gem_v_node, gem_v_indent};
    GemVal _t3137 = (*gem_v_compile_stmt);
    GemVal _t3138[] = {_t3137.fn(_t3137.env, _t3136, 2)};
    GemVal _t3139[] = {gem_v_p};
    GemVal _t3140[] = {gem_v_p};
        GemVal _t3141 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3138, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3139, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3140, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3141;
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
#line 1053 "compiler/main.gem"
    GemVal _t3142[] = {gem_v_node};
    GemVal _t3143 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3143.fn(_t3143.env, _t3142, 1);
#line 1054 "compiler/main.gem"
    GemVal _t3144 = (*gem_v_tmp);
    GemVal gem_v_t = _t3144.fn(_t3144.env, NULL, 0);
    GemVal _t3145[] = {gem_v_line_dir};
    GemVal _t3146 = gem_v_r;
    GemVal _t3147[] = {gem_table_get(_t3146, gem_string("setup"))};
    GemVal _t3148[] = {gem_v_p};
    GemVal _t3149[] = {gem_v_t};
    GemVal _t3150 = gem_v_r;
    GemVal _t3151[] = {gem_table_get(_t3150, gem_string("expr"))};
    GemVal _t3152[] = {gem_v_p};
    GemVal _t3153[] = {gem_v_p};
    GemVal _t3154[] = {gem_v_t};
    GemVal _t3155 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3145, 1), gem_to_string_fn(NULL, _t3147, 1)), gem_to_string_fn(NULL, _t3148, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3149, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3151, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3152, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3153, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3154, 1)), gem_string(";"));
    gem_pop_frame();
    return _t3155;
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
#line 1061 "compiler/main.gem"
    GemVal _t3157[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3157, 1);
#line 1062 "compiler/main.gem"
    GemVal _t3158 = gem_v_node;
    GemVal _t3159[] = {gem_table_get(_t3158, gem_string("cond"))};
    GemVal _t3160 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3160.fn(_t3160.env, _t3159, 1);
#line 1063 "compiler/main.gem"
    GemVal _t3161 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t3161, gem_string("setup"));
#line 1064 "compiler/main.gem"
    GemVal _t3162[] = {gem_v_p};
    GemVal _t3163 = gem_v_cond_r;
    GemVal _t3164[] = {gem_table_get(_t3163, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3162, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3164, 1)), gem_string(")) {\n")));
#line 1065 "compiler/main.gem"
    GemVal _t3165 = gem_v_node;
    GemVal _t3166[] = {gem_table_get(_t3165, gem_string("then")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3167 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t3167.fn(_t3167.env, _t3166, 2));
#line 1066 "compiler/main.gem"
    GemVal _t3168 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3168, gem_string("else")), GEM_NIL))) {
#line 1067 "compiler/main.gem"
    GemVal _t3169[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3169, 1), gem_string("} else {\n")));
#line 1068 "compiler/main.gem"
    GemVal _t3170 = gem_v_node;
    GemVal _t3171[] = {gem_table_get(_t3170, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3172 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3172.fn(_t3172.env, _t3171, 2));
    }
#line 1070 "compiler/main.gem"
    GemVal _t3173[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3173, 1), gem_string("}")));
    GemVal _t3174 = gem_v_out;
    gem_pop_frame();
    return _t3174;
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
#line 1077 "compiler/main.gem"
    GemVal _t3176[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3176, 1);
#line 1078 "compiler/main.gem"
    GemVal _t3177 = gem_v_node;
    GemVal _t3178[] = {gem_table_get(_t3177, gem_string("cond"))};
    GemVal _t3179 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3179.fn(_t3179.env, _t3178, 1);
#line 1079 "compiler/main.gem"
    GemVal _t3180 = gem_v_cond_r;
    GemVal gem_v_out = gem_table_get(_t3180, gem_string("setup"));
#line 1080 "compiler/main.gem"
    GemVal _t3181[] = {gem_v_p};
    GemVal _t3182 = gem_v_cond_r;
    GemVal _t3183[] = {gem_table_get(_t3182, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3181, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3183, 1)), gem_string(")) {\n")));
#line 1081 "compiler/main.gem"
    GemVal _t3184 = gem_v_node;
    GemVal gem_v_then_body = gem_table_get(_t3184, gem_string("then"));
#line 1082 "compiler/main.gem"
    GemVal _t3185[] = {gem_v_then_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3185, 1), gem_int(0)))) {
#line 1083 "compiler/main.gem"
        GemVal gem_v__for_i_55 = gem_int(0);
#line 1083 "compiler/main.gem"
    GemVal _t3186[] = {gem_v_then_body};
        GemVal gem_v__for_limit_55 = gem_sub(gem_len_fn(NULL, _t3186, 1), gem_int(1));
#line 1083 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 1083 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_55;
#line 1083 "compiler/main.gem"
            gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1084 "compiler/main.gem"
    GemVal _t3187[] = {gem_table_get(gem_v_then_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3188 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3188.fn(_t3188.env, _t3187, 2)), gem_string("\n"));
        }

#line 1086 "compiler/main.gem"
    GemVal _t3189[] = {gem_table_get(gem_v_then_body, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3190 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3190.fn(_t3190.env, _t3189, 2)), gem_string("\n"));
    } else {
#line 1088 "compiler/main.gem"
    GemVal _t3191[] = {gem_v_p};
    GemVal _t3192[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3191, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3192, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1090 "compiler/main.gem"
    GemVal _t3193 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3193, gem_string("else")), GEM_NIL))) {
#line 1091 "compiler/main.gem"
    GemVal _t3194[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3194, 1), gem_string("} else {\n")));
#line 1092 "compiler/main.gem"
    GemVal _t3195 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3195, gem_string("else"));
#line 1093 "compiler/main.gem"
    GemVal _t3196[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3196, 1), gem_int(0)))) {
#line 1094 "compiler/main.gem"
            GemVal gem_v__for_i_56 = gem_int(0);
#line 1094 "compiler/main.gem"
    GemVal _t3197[] = {gem_v_else_body};
            GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t3197, 1), gem_int(1));
#line 1094 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1094 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_56;
#line 1094 "compiler/main.gem"
                gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1095 "compiler/main.gem"
    GemVal _t3198[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3199 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3199.fn(_t3199.env, _t3198, 2)), gem_string("\n"));
            }

#line 1097 "compiler/main.gem"
    GemVal _t3200[] = {gem_table_get(gem_v_else_body, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3201 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3201.fn(_t3201.env, _t3200, 2)), gem_string("\n"));
        } else {
#line 1099 "compiler/main.gem"
    GemVal _t3202[] = {gem_v_p};
    GemVal _t3203[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3202, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3203, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1102 "compiler/main.gem"
    GemVal _t3204[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3204, 1), gem_string("} else {\n")));
#line 1103 "compiler/main.gem"
    GemVal _t3205[] = {gem_v_p};
    GemVal _t3206[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3205, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3206, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1105 "compiler/main.gem"
    GemVal _t3207[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3207, 1), gem_string("}")));
    GemVal _t3208 = gem_v_out;
    gem_pop_frame();
    return _t3208;
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
#line 1112 "compiler/main.gem"
    GemVal _t3210[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3210, 1);
#line 1113 "compiler/main.gem"
    GemVal _t3211 = gem_v_node;
    GemVal _t3212[] = {gem_table_get(_t3211, gem_string("cond"))};
    GemVal _t3213 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3213.fn(_t3213.env, _t3212, 1);
#line 1114 "compiler/main.gem"
    GemVal _t3214[] = {gem_v_p};
    GemVal gem_v_out = gem_add(gem_to_string_fn(NULL, _t3214, 1), gem_string("while (1) {\n"));
#line 1115 "compiler/main.gem"
    GemVal _t3215[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3215, 1), gem_string("    gem_yield_check();\n")));
#line 1116 "compiler/main.gem"
    GemVal _t3216 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3216, gem_string("setup")), gem_string("")))) {
#line 1117 "compiler/main.gem"
    GemVal _t3217 = gem_v_cond_r;
    GemVal _t3218[] = {gem_v_p};
    GemVal _t3219[] = {gem_table_get(_t3217, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t3218, 1), gem_string("    "))};
        gem_v_out = gem_add(gem_v_out, gem_str_replace_fn(NULL, _t3219, 3));
    }
#line 1119 "compiler/main.gem"
    GemVal _t3220[] = {gem_v_p};
    GemVal _t3221 = gem_v_cond_r;
    GemVal _t3222[] = {gem_table_get(_t3221, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3220, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t3222, 1)), gem_string(")) break;\n")));
#line 1120 "compiler/main.gem"
    GemVal _t3223 = gem_v_node;
    GemVal _t3224[] = {gem_table_get(_t3223, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3225 = (*gem_v_compile_stmts);
    gem_v_out = gem_add(gem_v_out, _t3225.fn(_t3225.env, _t3224, 2));
#line 1121 "compiler/main.gem"
    GemVal _t3226[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3226, 1), gem_string("}")));
    GemVal _t3227 = gem_v_out;
    gem_pop_frame();
    return _t3227;
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
#line 1128 "compiler/main.gem"
    GemVal _t3229[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3229, 1);
#line 1129 "compiler/main.gem"
    GemVal _t3230 = gem_v_node;
    GemVal _t3231[] = {gem_table_get(_t3230, gem_string("target"))};
    GemVal _t3232 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3232.fn(_t3232.env, _t3231, 1);
#line 1130 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1131 "compiler/main.gem"
    GemVal _t3233 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3233, gem_string("target_var")), GEM_NIL))) {
#line 1132 "compiler/main.gem"
    GemVal _t3234 = gem_v_node;
    GemVal _t3235[] = {gem_table_get(_t3234, gem_string("target_var"))};
    GemVal _t3236 = (*gem_v_mangle);
        gem_v_t = _t3236.fn(_t3236.env, _t3235, 1);
    } else {
#line 1134 "compiler/main.gem"
    GemVal _t3237 = (*gem_v_tmp);
        gem_v_t = _t3237.fn(_t3237.env, NULL, 0);
    }
#line 1136 "compiler/main.gem"
    GemVal _t3238 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3238, gem_string("setup"));
#line 1137 "compiler/main.gem"
    GemVal _t3239[] = {gem_v_p};
    GemVal _t3240[] = {gem_v_t};
    GemVal _t3241 = gem_v_target_r;
    GemVal _t3242[] = {gem_table_get(_t3241, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3239, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3240, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3242, 1)), gem_string(";\n")));
#line 1138 "compiler/main.gem"
    GemVal _t3243 = gem_v_node;
    GemVal _t3244[] = {gem_table_get(_t3243, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3244, 1), gem_int(0)))) {
#line 1140 "compiler/main.gem"
    GemVal _t3245 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3245, gem_string("else")), GEM_NIL))) {
#line 1141 "compiler/main.gem"
    GemVal _t3246 = gem_v_node;
    GemVal _t3247[] = {gem_table_get(_t3246, gem_string("else")), gem_v_indent};
    GemVal _t3248 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t3248.fn(_t3248.env, _t3247, 2));
        }
#line 1143 "compiler/main.gem"
    GemVal _t3249[] = {gem_v_p};
    GemVal _t3250[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3249, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3250, 1)), gem_string(";")));
#line 1144 "compiler/main.gem"
        GemVal _t3251 = gem_v_out;
        gem_pop_frame();
        return _t3251;
    }
#line 1146 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1146 "compiler/main.gem"
    GemVal _t3252 = gem_v_node;
    GemVal _t3253[] = {gem_table_get(_t3252, gem_string("whens"))};
    GemVal gem_v__for_limit_58 = gem_len_fn(NULL, _t3253, 1);
#line 1146 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_v__for_limit_58))) break;
#line 1146 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_58;
#line 1146 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1147 "compiler/main.gem"
    GemVal _t3254 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3254, gem_string("whens")), gem_v_i);
#line 1148 "compiler/main.gem"
    GemVal _t3255 = gem_v_w;
    GemVal _t3256[] = {gem_table_get(_t3255, gem_string("value"))};
    GemVal _t3257 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3257.fn(_t3257.env, _t3256, 1);
#line 1149 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1150 "compiler/main.gem"
    GemVal _t3258 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3258, gem_string("setup")));
#line 1151 "compiler/main.gem"
    GemVal _t3259[] = {gem_v_p};
    GemVal _t3260 = gem_v_val_r;
    GemVal _t3261[] = {gem_table_get(_t3260, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3259, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3261, 1)), gem_string(")) {\n")));
        } else {
#line 1153 "compiler/main.gem"
    GemVal _t3262[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3262, 1), gem_string("} else {\n")));
#line 1154 "compiler/main.gem"
    GemVal _t3263 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3263, gem_string("setup")));
#line 1155 "compiler/main.gem"
    GemVal _t3264[] = {gem_v_p};
    GemVal _t3265 = gem_v_val_r;
    GemVal _t3266[] = {gem_table_get(_t3265, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3264, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3266, 1)), gem_string(")) {\n")));
        }
#line 1157 "compiler/main.gem"
    GemVal _t3267 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3267, gem_string("bindings")), GEM_NIL))) {
#line 1158 "compiler/main.gem"
    GemVal _t3268 = gem_v_w;
            GemVal gem_v__for_items_57 = gem_table_get(_t3268, gem_string("bindings"));
#line 1158 "compiler/main.gem"
            GemVal gem_v__for_i_57 = gem_int(0);
#line 1158 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3269[] = {gem_v__for_items_57};
                if (!gem_truthy(gem_lt(gem_v__for_i_57, gem_len_fn(NULL, _t3269, 1)))) break;
#line 1158 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_57, gem_v__for_i_57);
#line 1158 "compiler/main.gem"
                gem_v__for_i_57 = gem_add(gem_v__for_i_57, gem_int(1));
#line 1159 "compiler/main.gem"
    GemVal _t3270[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3271 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3271.fn(_t3271.env, _t3270, 2)), gem_string("\n"));
            }

        }
#line 1162 "compiler/main.gem"
    GemVal _t3272 = gem_v_w;
    GemVal _t3273[] = {gem_table_get(_t3272, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3274 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3274.fn(_t3274.env, _t3273, 2));
    }

#line 1164 "compiler/main.gem"
    GemVal _t3275 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3275, gem_string("else")), GEM_NIL))) {
#line 1165 "compiler/main.gem"
    GemVal _t3276[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3276, 1), gem_string("} else {\n")));
#line 1166 "compiler/main.gem"
    GemVal _t3277 = gem_v_node;
    GemVal _t3278[] = {gem_table_get(_t3277, gem_string("else")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3279 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3279.fn(_t3279.env, _t3278, 2));
    }
#line 1169 "compiler/main.gem"
    GemVal _t3280[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3280, 1), gem_string("}")));
#line 1170 "compiler/main.gem"
    GemVal gem_v__for_i_59 = gem_int(1);
#line 1170 "compiler/main.gem"
    GemVal _t3281 = gem_v_node;
    GemVal _t3282[] = {gem_table_get(_t3281, gem_string("whens"))};
    GemVal gem_v__for_limit_59 = gem_len_fn(NULL, _t3282, 1);
#line 1170 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1170 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_59;
#line 1170 "compiler/main.gem"
        gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1171 "compiler/main.gem"
    GemVal _t3283[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3283, 1)), gem_string("}")));
    }

    GemVal _t3284 = gem_v_out;
    gem_pop_frame();
    return _t3284;
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
#line 1179 "compiler/main.gem"
    GemVal _t3286[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3286, 1);
#line 1180 "compiler/main.gem"
    GemVal _t3287 = gem_v_node;
    GemVal _t3288[] = {gem_table_get(_t3287, gem_string("target"))};
    GemVal _t3289 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3289.fn(_t3289.env, _t3288, 1);
#line 1181 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1182 "compiler/main.gem"
    GemVal _t3290 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3290, gem_string("target_var")), GEM_NIL))) {
#line 1183 "compiler/main.gem"
    GemVal _t3291 = gem_v_node;
    GemVal _t3292[] = {gem_table_get(_t3291, gem_string("target_var"))};
    GemVal _t3293 = (*gem_v_mangle);
        gem_v_t = _t3293.fn(_t3293.env, _t3292, 1);
    } else {
#line 1185 "compiler/main.gem"
    GemVal _t3294 = (*gem_v_tmp);
        gem_v_t = _t3294.fn(_t3294.env, NULL, 0);
    }
#line 1187 "compiler/main.gem"
    GemVal _t3295 = gem_v_target_r;
    GemVal gem_v_out = gem_table_get(_t3295, gem_string("setup"));
#line 1188 "compiler/main.gem"
    GemVal _t3296[] = {gem_v_p};
    GemVal _t3297[] = {gem_v_t};
    GemVal _t3298 = gem_v_target_r;
    GemVal _t3299[] = {gem_table_get(_t3298, gem_string("expr"))};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3296, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3297, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3299, 1)), gem_string(";\n")));
#line 1189 "compiler/main.gem"
    GemVal _t3300 = gem_v_node;
    GemVal _t3301[] = {gem_table_get(_t3300, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3301, 1), gem_int(0)))) {
#line 1191 "compiler/main.gem"
    GemVal _t3302 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3302, gem_string("else")), GEM_NIL))) {
#line 1192 "compiler/main.gem"
    GemVal _t3303 = gem_v_node;
            GemVal gem_v_else_body = gem_table_get(_t3303, gem_string("else"));
#line 1193 "compiler/main.gem"
    GemVal _t3304[] = {gem_v_else_body};
            if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3304, 1), gem_int(0)))) {
#line 1194 "compiler/main.gem"
                GemVal gem_v__for_i_60 = gem_int(0);
#line 1194 "compiler/main.gem"
    GemVal _t3305[] = {gem_v_else_body};
                GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3305, 1), gem_int(1));
#line 1194 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1194 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_60;
#line 1194 "compiler/main.gem"
                    gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1195 "compiler/main.gem"
    GemVal _t3306[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_v_indent};
    GemVal _t3307 = (*gem_v_compile_stmt);
                    gem_v_out = gem_add(gem_add(gem_v_out, _t3307.fn(_t3307.env, _t3306, 2)), gem_string("\n"));
                }

#line 1197 "compiler/main.gem"
    GemVal _t3308[] = {gem_table_get(gem_v_else_body, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t3309 = (*gem_v_compile_stmt_return);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3309.fn(_t3309.env, _t3308, 2)), gem_string("\n"));
            } else {
#line 1199 "compiler/main.gem"
    GemVal _t3310[] = {gem_v_p};
    GemVal _t3311[] = {gem_v_p};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3310, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3311, 1)), gem_string("return GEM_NIL;\n")));
            }
        } else {
#line 1202 "compiler/main.gem"
    GemVal _t3312[] = {gem_v_p};
    GemVal _t3313[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3312, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3313, 1)), gem_string("return GEM_NIL;\n")));
        }
#line 1204 "compiler/main.gem"
    GemVal _t3314[] = {gem_v_p};
    GemVal _t3315[] = {gem_v_t};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3314, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3315, 1)), gem_string(";")));
#line 1205 "compiler/main.gem"
        GemVal _t3316 = gem_v_out;
        gem_pop_frame();
        return _t3316;
    }
#line 1207 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(0);
#line 1207 "compiler/main.gem"
    GemVal _t3317 = gem_v_node;
    GemVal _t3318[] = {gem_table_get(_t3317, gem_string("whens"))};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3318, 1);
#line 1207 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1207 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1207 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1208 "compiler/main.gem"
    GemVal _t3319 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3319, gem_string("whens")), gem_v_i);
#line 1209 "compiler/main.gem"
    GemVal _t3320 = gem_v_w;
    GemVal _t3321[] = {gem_table_get(_t3320, gem_string("value"))};
    GemVal _t3322 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3322.fn(_t3322.env, _t3321, 1);
#line 1210 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1211 "compiler/main.gem"
    GemVal _t3323 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3323, gem_string("setup")));
#line 1212 "compiler/main.gem"
    GemVal _t3324[] = {gem_v_p};
    GemVal _t3325 = gem_v_val_r;
    GemVal _t3326[] = {gem_table_get(_t3325, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3324, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3326, 1)), gem_string(")) {\n")));
        } else {
#line 1214 "compiler/main.gem"
    GemVal _t3327[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3327, 1), gem_string("} else {\n")));
#line 1215 "compiler/main.gem"
    GemVal _t3328 = gem_v_val_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3328, gem_string("setup")));
#line 1216 "compiler/main.gem"
    GemVal _t3329[] = {gem_v_p};
    GemVal _t3330 = gem_v_val_r;
    GemVal _t3331[] = {gem_table_get(_t3330, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3329, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3331, 1)), gem_string(")) {\n")));
        }
#line 1218 "compiler/main.gem"
    GemVal _t3332 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3332, gem_string("bindings")), GEM_NIL))) {
#line 1219 "compiler/main.gem"
    GemVal _t3333 = gem_v_w;
            GemVal gem_v__for_items_61 = gem_table_get(_t3333, gem_string("bindings"));
#line 1219 "compiler/main.gem"
            GemVal gem_v__for_i_61 = gem_int(0);
#line 1219 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3334[] = {gem_v__for_items_61};
                if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3334, 1)))) break;
#line 1219 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1219 "compiler/main.gem"
                gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1220 "compiler/main.gem"
    GemVal _t3335[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3336 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3336.fn(_t3336.env, _t3335, 2)), gem_string("\n"));
            }

        }
#line 1223 "compiler/main.gem"
    GemVal _t3337 = gem_v_w;
        GemVal gem_v_body = gem_table_get(_t3337, gem_string("body"));
#line 1224 "compiler/main.gem"
    GemVal _t3338[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3338, 1), gem_int(0)))) {
#line 1225 "compiler/main.gem"
            GemVal gem_v__for_i_62 = gem_int(0);
#line 1225 "compiler/main.gem"
    GemVal _t3339[] = {gem_v_body};
            GemVal gem_v__for_limit_62 = gem_sub(gem_len_fn(NULL, _t3339, 1), gem_int(1));
#line 1225 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1225 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_62;
#line 1225 "compiler/main.gem"
                gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1226 "compiler/main.gem"
    GemVal _t3340[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3341 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3341.fn(_t3341.env, _t3340, 2)), gem_string("\n"));
            }

#line 1228 "compiler/main.gem"
    GemVal _t3342[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3343 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3343.fn(_t3343.env, _t3342, 2)), gem_string("\n"));
        } else {
#line 1230 "compiler/main.gem"
    GemVal _t3344[] = {gem_v_p};
    GemVal _t3345[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3344, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3345, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1233 "compiler/main.gem"
    GemVal _t3346 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3346, gem_string("else")), GEM_NIL))) {
#line 1234 "compiler/main.gem"
    GemVal _t3347[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3347, 1), gem_string("} else {\n")));
#line 1235 "compiler/main.gem"
    GemVal _t3348 = gem_v_node;
        GemVal gem_v_else_body = gem_table_get(_t3348, gem_string("else"));
#line 1236 "compiler/main.gem"
    GemVal _t3349[] = {gem_v_else_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3349, 1), gem_int(0)))) {
#line 1237 "compiler/main.gem"
            GemVal gem_v__for_i_64 = gem_int(0);
#line 1237 "compiler/main.gem"
    GemVal _t3350[] = {gem_v_else_body};
            GemVal gem_v__for_limit_64 = gem_sub(gem_len_fn(NULL, _t3350, 1), gem_int(1));
#line 1237 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1237 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_64;
#line 1237 "compiler/main.gem"
                gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1238 "compiler/main.gem"
    GemVal _t3351[] = {gem_table_get(gem_v_else_body, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3352 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3352.fn(_t3352.env, _t3351, 2)), gem_string("\n"));
            }

#line 1240 "compiler/main.gem"
    GemVal _t3353[] = {gem_table_get(gem_v_else_body, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3354 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3354.fn(_t3354.env, _t3353, 2)), gem_string("\n"));
        } else {
#line 1242 "compiler/main.gem"
    GemVal _t3355[] = {gem_v_p};
    GemVal _t3356[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3355, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3356, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1245 "compiler/main.gem"
    GemVal _t3357[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3357, 1), gem_string("} else {\n")));
#line 1246 "compiler/main.gem"
    GemVal _t3358[] = {gem_v_p};
    GemVal _t3359[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3358, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3359, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1248 "compiler/main.gem"
    GemVal _t3360[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3360, 1), gem_string("}")));
#line 1249 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(1);
#line 1249 "compiler/main.gem"
    GemVal _t3361 = gem_v_node;
    GemVal _t3362[] = {gem_table_get(_t3361, gem_string("whens"))};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3362, 1);
#line 1249 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1249 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1249 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1250 "compiler/main.gem"
    GemVal _t3363[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3363, 1)), gem_string("}")));
    }

    GemVal _t3364 = gem_v_out;
    gem_pop_frame();
    return _t3364;
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
#line 1258 "compiler/main.gem"
    GemVal _t3366[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3366, 1);
#line 1259 "compiler/main.gem"
    GemVal _t3367 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3367, gem_string("arms"));
#line 1260 "compiler/main.gem"
    GemVal _t3368 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3368, gem_string("after_ms"));
#line 1261 "compiler/main.gem"
    GemVal _t3369 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3369, gem_string("after_body"));
#line 1264 "compiler/main.gem"
    GemVal _t3370 = gem_v_node;
    GemVal _t3371[] = {gem_table_get(_t3370, gem_string("recv_var"))};
    GemVal _t3372 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3372.fn(_t3372.env, _t3371, 1);
#line 1265 "compiler/main.gem"
    GemVal _t3373 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3373.fn(_t3373.env, NULL, 0);
#line 1266 "compiler/main.gem"
    GemVal _t3374 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3374.fn(_t3374.env, NULL, 0);
#line 1267 "compiler/main.gem"
    GemVal _t3375 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3375.fn(_t3375.env, NULL, 0);
#line 1268 "compiler/main.gem"
    GemVal _t3376 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3376.fn(_t3376.env, NULL, 0);
#line 1269 "compiler/main.gem"
    GemVal _t3377 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3377.fn(_t3377.env, NULL, 0);
#line 1271 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1274 "compiler/main.gem"
    GemVal _t3378[] = {gem_v_p};
    GemVal _t3379[] = {gem_v_recv_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3378, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3379, 1)), gem_string(" = GEM_NIL;\n")));
#line 1275 "compiler/main.gem"
    GemVal _t3380[] = {gem_v_p};
    GemVal _t3381[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3380, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3381, 1)), gem_string(" = -1;\n")));
#line 1278 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1279 "compiler/main.gem"
    GemVal _t3382[] = {gem_v_after_ms};
    GemVal _t3383 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3383.fn(_t3383.env, _t3382, 1);
#line 1280 "compiler/main.gem"
    GemVal _t3384 = gem_v_ms_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t3384, gem_string("setup")));
#line 1281 "compiler/main.gem"
    GemVal _t3385[] = {gem_v_p};
    GemVal _t3386[] = {gem_v_deadline_tmp};
    GemVal _t3387 = gem_v_ms_r;
    GemVal _t3388[] = {gem_table_get(_t3387, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3385, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3386, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3388, 1)), gem_string(").ival;\n")));
    }
#line 1285 "compiler/main.gem"
    GemVal _t3389[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3389, 1), gem_string("while (1) {\n")));
#line 1286 "compiler/main.gem"
    GemVal _t3390[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3390, 1);
#line 1289 "compiler/main.gem"
    GemVal _t3391[] = {gem_v_p2};
    GemVal _t3392[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3391, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3392, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n")));
#line 1292 "compiler/main.gem"
    GemVal _t3393[] = {gem_v_p2};
    GemVal _t3394[] = {gem_v_prev_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3393, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3394, 1)), gem_string(" = NULL;\n")));
#line 1293 "compiler/main.gem"
    GemVal _t3395[] = {gem_v_p2};
    GemVal _t3396[] = {gem_v_cur_tmp};
    GemVal _t3397[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3395, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3396, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3397, 1)), gem_string("->mailbox.head;\n")));
#line 1294 "compiler/main.gem"
    GemVal _t3398[] = {gem_v_p2};
    GemVal _t3399[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_to_string_fn(NULL, _t3398, 1), gem_to_string_fn(NULL, _t3399, 1)), gem_string(" = -1;\n")));
#line 1295 "compiler/main.gem"
    GemVal _t3400[] = {gem_v_p2};
    GemVal _t3401[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3400, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3401, 1)), gem_string(") {\n")));
#line 1296 "compiler/main.gem"
    GemVal _t3402[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3402, 1);
#line 1297 "compiler/main.gem"
    GemVal _t3403[] = {gem_v_p3};
    GemVal _t3404[] = {gem_v_recv_tmp};
    GemVal _t3405[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3403, 1), gem_to_string_fn(NULL, _t3404, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3405, 1)), gem_string("->value;\n")));
#line 1300 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(0);
#line 1300 "compiler/main.gem"
    GemVal _t3406[] = {gem_v_arms};
    GemVal gem_v__for_limit_66 = gem_len_fn(NULL, _t3406, 1);
#line 1300 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_v__for_limit_66))) break;
#line 1300 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_66;
#line 1300 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1301 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1302 "compiler/main.gem"
    GemVal _t3407 = gem_v_arm;
    GemVal _t3408 = gem_table_get(_t3407, gem_string("pattern"));
    GemVal _t3409[] = {gem_table_get(_t3408, gem_string("condition"))};
    GemVal _t3410 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3410.fn(_t3410.env, _t3409, 1);
#line 1303 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1304 "compiler/main.gem"
    GemVal _t3411 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3411, gem_string("setup")));
#line 1305 "compiler/main.gem"
    GemVal _t3412[] = {gem_v_p3};
    GemVal _t3413 = gem_v_cond_r;
    GemVal _t3414[] = {gem_table_get(_t3413, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3412, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3414, 1)), gem_string(")) {\n")));
        } else {
#line 1307 "compiler/main.gem"
    GemVal _t3415[] = {gem_v_p3};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3415, 1), gem_string("} else {\n")));
#line 1308 "compiler/main.gem"
    GemVal _t3416 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3416, gem_string("setup")));
#line 1309 "compiler/main.gem"
    GemVal _t3417[] = {gem_v_p3};
    GemVal _t3418 = gem_v_cond_r;
    GemVal _t3419[] = {gem_table_get(_t3418, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3417, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3419, 1)), gem_string(")) {\n")));
        }
#line 1311 "compiler/main.gem"
    GemVal _t3420[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3421[] = {gem_fn_pad_for(NULL, _t3420, 1)};
    GemVal _t3422[] = {gem_v_matched_tmp};
    GemVal _t3423[] = {gem_v_i};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3421, 1), gem_to_string_fn(NULL, _t3422, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3423, 1)), gem_string(";\n")));
    }

#line 1314 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1314 "compiler/main.gem"
    GemVal _t3424[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3424, 1);
#line 1314 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1314 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1314 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1315 "compiler/main.gem"
    GemVal _t3425[] = {gem_v_p3};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3425, 1), gem_string("}")));
#line 1316 "compiler/main.gem"
    GemVal _t3426[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3426, 1), gem_int(1))))) {
#line 1317 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1320 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1323 "compiler/main.gem"
    GemVal _t3427[] = {gem_v_p3};
    GemVal _t3428[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3427, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3428, 1)), gem_string(" >= 0) {\n")));
#line 1324 "compiler/main.gem"
    GemVal _t3429[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3429, 1);
#line 1325 "compiler/main.gem"
    GemVal _t3430[] = {gem_v_p4};
    GemVal _t3431[] = {gem_v_proc_tmp};
    GemVal _t3432[] = {gem_v_prev_tmp};
    GemVal _t3433[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3430, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3431, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3432, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3433, 1)), gem_string(");\n")));
#line 1326 "compiler/main.gem"
    GemVal _t3434[] = {gem_v_p4};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3434, 1), gem_string("break;\n")));
#line 1327 "compiler/main.gem"
    GemVal _t3435[] = {gem_v_p3};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3435, 1), gem_string("}\n")));
#line 1330 "compiler/main.gem"
    GemVal _t3436[] = {gem_v_p3};
    GemVal _t3437[] = {gem_v_prev_tmp};
    GemVal _t3438[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3436, 1), gem_to_string_fn(NULL, _t3437, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3438, 1)), gem_string(";\n")));
#line 1331 "compiler/main.gem"
    GemVal _t3439[] = {gem_v_p3};
    GemVal _t3440[] = {gem_v_cur_tmp};
    GemVal _t3441[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3439, 1), gem_to_string_fn(NULL, _t3440, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3441, 1)), gem_string("->next;\n")));
#line 1332 "compiler/main.gem"
    GemVal _t3442[] = {gem_v_p2};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3442, 1), gem_string("}\n")));
#line 1335 "compiler/main.gem"
    GemVal _t3443[] = {gem_v_p2};
    GemVal _t3444[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3443, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3444, 1)), gem_string(" >= 0) break;\n")));
#line 1336 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1337 "compiler/main.gem"
    GemVal _t3445[] = {gem_v_p2};
    GemVal _t3446[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3445, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3446, 1)), gem_string(") {\n")));
#line 1338 "compiler/main.gem"
    GemVal _t3447[] = {gem_v_p2};
    GemVal _t3448[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3447, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3448, 1)), gem_string(" = -2;\n")));
#line 1339 "compiler/main.gem"
    GemVal _t3449[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3449, 1), gem_string("    break;\n")));
#line 1340 "compiler/main.gem"
    GemVal _t3450[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3450, 1), gem_string("}\n")));
#line 1341 "compiler/main.gem"
    GemVal _t3451[] = {gem_v_p2};
    GemVal _t3452[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3451, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3452, 1)), gem_string(");\n")));
#line 1343 "compiler/main.gem"
    GemVal _t3453[] = {gem_v_p2};
    GemVal _t3454[] = {gem_v_proc_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3453, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3454, 1)), gem_string("->timed_out) {\n")));
#line 1344 "compiler/main.gem"
    GemVal _t3455[] = {gem_v_p2};
    GemVal _t3456[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3455, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3456, 1)), gem_string(" = -2;\n")));
#line 1345 "compiler/main.gem"
    GemVal _t3457[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3457, 1), gem_string("    break;\n")));
#line 1346 "compiler/main.gem"
    GemVal _t3458[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3458, 1), gem_string("}\n")));
    } else {
#line 1348 "compiler/main.gem"
    GemVal _t3459[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3459, 1), gem_string("gem_selective_yield(-1);\n")));
    }
#line 1351 "compiler/main.gem"
    GemVal _t3460[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3460, 1), gem_string("}\n")));
#line 1354 "compiler/main.gem"
    GemVal gem_v__for_i_69 = gem_int(0);
#line 1354 "compiler/main.gem"
    GemVal _t3461[] = {gem_v_arms};
    GemVal gem_v__for_limit_69 = gem_len_fn(NULL, _t3461, 1);
#line 1354 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_v__for_limit_69))) break;
#line 1354 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_69;
#line 1354 "compiler/main.gem"
        gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1355 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1356 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1357 "compiler/main.gem"
    GemVal _t3462[] = {gem_v_p};
    GemVal _t3463[] = {gem_v_matched_tmp};
    GemVal _t3464[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3462, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3463, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3464, 1)), gem_string(") {\n")));
        } else {
#line 1359 "compiler/main.gem"
    GemVal _t3465[] = {gem_v_p};
    GemVal _t3466[] = {gem_v_matched_tmp};
    GemVal _t3467[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3465, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3466, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3467, 1)), gem_string(") {\n")));
        }
#line 1362 "compiler/main.gem"
    GemVal _t3468 = gem_v_arm;
    GemVal _t3469 = gem_table_get(_t3468, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3469, gem_string("bindings")), GEM_NIL))) {
#line 1363 "compiler/main.gem"
    GemVal _t3470 = gem_v_arm;
    GemVal _t3471 = gem_table_get(_t3470, gem_string("pattern"));
            GemVal gem_v__for_items_68 = gem_table_get(_t3471, gem_string("bindings"));
#line 1363 "compiler/main.gem"
            GemVal gem_v__for_i_68 = gem_int(0);
#line 1363 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3472[] = {gem_v__for_items_68};
                if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_len_fn(NULL, _t3472, 1)))) break;
#line 1363 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_68, gem_v__for_i_68);
#line 1363 "compiler/main.gem"
                gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1364 "compiler/main.gem"
    GemVal _t3473[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3474 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3474.fn(_t3474.env, _t3473, 2)), gem_string("\n"));
            }

        }
#line 1367 "compiler/main.gem"
    GemVal _t3475 = gem_v_arm;
    GemVal _t3476[] = {gem_table_get(_t3475, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3477 = (*gem_v_compile_stmts);
        gem_v_out = gem_add(gem_v_out, _t3477.fn(_t3477.env, _t3476, 2));
    }

#line 1371 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1372 "compiler/main.gem"
    GemVal _t3478[] = {gem_v_p};
    GemVal _t3479[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3478, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3479, 1)), gem_string(" == -2) {\n")));
#line 1373 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1374 "compiler/main.gem"
    GemVal _t3480[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3481 = (*gem_v_compile_stmts);
            gem_v_out = gem_add(gem_v_out, _t3481.fn(_t3481.env, _t3480, 2));
        }
    }
#line 1378 "compiler/main.gem"
    GemVal _t3482[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3482, 1), gem_string("}")));
    GemVal _t3483 = gem_v_out;
    gem_pop_frame();
    return _t3483;
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
#line 1383 "compiler/main.gem"
    GemVal _t3485[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn_pad_for(NULL, _t3485, 1);
#line 1384 "compiler/main.gem"
    GemVal _t3486 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3486, gem_string("arms"));
#line 1385 "compiler/main.gem"
    GemVal _t3487 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3487, gem_string("after_ms"));
#line 1386 "compiler/main.gem"
    GemVal _t3488 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3488, gem_string("after_body"));
#line 1388 "compiler/main.gem"
    GemVal _t3489 = gem_v_node;
    GemVal _t3490[] = {gem_table_get(_t3489, gem_string("recv_var"))};
    GemVal _t3491 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3491.fn(_t3491.env, _t3490, 1);
#line 1389 "compiler/main.gem"
    GemVal _t3492 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3492.fn(_t3492.env, NULL, 0);
#line 1390 "compiler/main.gem"
    GemVal _t3493 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3493.fn(_t3493.env, NULL, 0);
#line 1391 "compiler/main.gem"
    GemVal _t3494 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3494.fn(_t3494.env, NULL, 0);
#line 1392 "compiler/main.gem"
    GemVal _t3495 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3495.fn(_t3495.env, NULL, 0);
#line 1393 "compiler/main.gem"
    GemVal _t3496 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3496.fn(_t3496.env, NULL, 0);
#line 1395 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1397 "compiler/main.gem"
    GemVal _t3497[] = {gem_v_p};
    GemVal _t3498[] = {gem_v_recv_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3497, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3498, 1)), gem_string(" = GEM_NIL;\n")));
#line 1398 "compiler/main.gem"
    GemVal _t3499[] = {gem_v_p};
    GemVal _t3500[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3499, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3500, 1)), gem_string(" = -1;\n")));
#line 1400 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1401 "compiler/main.gem"
    GemVal _t3501[] = {gem_v_after_ms};
    GemVal _t3502 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3502.fn(_t3502.env, _t3501, 1);
#line 1402 "compiler/main.gem"
    GemVal _t3503 = gem_v_ms_r;
        gem_v_out = gem_add(gem_v_out, gem_table_get(_t3503, gem_string("setup")));
#line 1403 "compiler/main.gem"
    GemVal _t3504[] = {gem_v_p};
    GemVal _t3505[] = {gem_v_deadline_tmp};
    GemVal _t3506 = gem_v_ms_r;
    GemVal _t3507[] = {gem_table_get(_t3506, gem_string("expr"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3504, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3505, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3507, 1)), gem_string(").ival;\n")));
    }
#line 1406 "compiler/main.gem"
    GemVal _t3508[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3508, 1), gem_string("while (1) {\n")));
#line 1407 "compiler/main.gem"
    GemVal _t3509[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn_pad_for(NULL, _t3509, 1);
#line 1409 "compiler/main.gem"
    GemVal _t3510[] = {gem_v_p2};
    GemVal _t3511[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3510, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3511, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n")));
#line 1410 "compiler/main.gem"
    GemVal _t3512[] = {gem_v_p2};
    GemVal _t3513[] = {gem_v_prev_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3512, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3513, 1)), gem_string(" = NULL;\n")));
#line 1411 "compiler/main.gem"
    GemVal _t3514[] = {gem_v_p2};
    GemVal _t3515[] = {gem_v_cur_tmp};
    GemVal _t3516[] = {gem_v_proc_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3514, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3515, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3516, 1)), gem_string("->mailbox.head;\n")));
#line 1412 "compiler/main.gem"
    GemVal _t3517[] = {gem_v_p2};
    GemVal _t3518[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_to_string_fn(NULL, _t3517, 1), gem_to_string_fn(NULL, _t3518, 1)), gem_string(" = -1;\n")));
#line 1413 "compiler/main.gem"
    GemVal _t3519[] = {gem_v_p2};
    GemVal _t3520[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3519, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3520, 1)), gem_string(") {\n")));
#line 1414 "compiler/main.gem"
    GemVal _t3521[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn_pad_for(NULL, _t3521, 1);
#line 1415 "compiler/main.gem"
    GemVal _t3522[] = {gem_v_p3};
    GemVal _t3523[] = {gem_v_recv_tmp};
    GemVal _t3524[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3522, 1), gem_to_string_fn(NULL, _t3523, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3524, 1)), gem_string("->value;\n")));
#line 1417 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1417 "compiler/main.gem"
    GemVal _t3525[] = {gem_v_arms};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3525, 1);
#line 1417 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1417 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1417 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1418 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1419 "compiler/main.gem"
    GemVal _t3526 = gem_v_arm;
    GemVal _t3527 = gem_table_get(_t3526, gem_string("pattern"));
    GemVal _t3528[] = {gem_table_get(_t3527, gem_string("condition"))};
    GemVal _t3529 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3529.fn(_t3529.env, _t3528, 1);
#line 1420 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1421 "compiler/main.gem"
    GemVal _t3530 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3530, gem_string("setup")));
#line 1422 "compiler/main.gem"
    GemVal _t3531[] = {gem_v_p3};
    GemVal _t3532 = gem_v_cond_r;
    GemVal _t3533[] = {gem_table_get(_t3532, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3531, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3533, 1)), gem_string(")) {\n")));
        } else {
#line 1424 "compiler/main.gem"
    GemVal _t3534[] = {gem_v_p3};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3534, 1), gem_string("} else {\n")));
#line 1425 "compiler/main.gem"
    GemVal _t3535 = gem_v_cond_r;
            gem_v_out = gem_add(gem_v_out, gem_table_get(_t3535, gem_string("setup")));
#line 1426 "compiler/main.gem"
    GemVal _t3536[] = {gem_v_p3};
    GemVal _t3537 = gem_v_cond_r;
    GemVal _t3538[] = {gem_table_get(_t3537, gem_string("expr"))};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3536, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3538, 1)), gem_string(")) {\n")));
        }
#line 1428 "compiler/main.gem"
    GemVal _t3539[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3540[] = {gem_fn_pad_for(NULL, _t3539, 1)};
    GemVal _t3541[] = {gem_v_matched_tmp};
    GemVal _t3542[] = {gem_v_i};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3540, 1), gem_to_string_fn(NULL, _t3541, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3542, 1)), gem_string(";\n")));
    }

#line 1430 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1430 "compiler/main.gem"
    GemVal _t3543[] = {gem_v_arms};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3543, 1);
#line 1430 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1430 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1430 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1431 "compiler/main.gem"
    GemVal _t3544[] = {gem_v_p3};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3544, 1), gem_string("}")));
#line 1432 "compiler/main.gem"
    GemVal _t3545[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3545, 1), gem_int(1))))) {
#line 1433 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1436 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1438 "compiler/main.gem"
    GemVal _t3546[] = {gem_v_p3};
    GemVal _t3547[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3546, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3547, 1)), gem_string(" >= 0) {\n")));
#line 1439 "compiler/main.gem"
    GemVal _t3548[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn_pad_for(NULL, _t3548, 1);
#line 1440 "compiler/main.gem"
    GemVal _t3549[] = {gem_v_p4};
    GemVal _t3550[] = {gem_v_proc_tmp};
    GemVal _t3551[] = {gem_v_prev_tmp};
    GemVal _t3552[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3549, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3550, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3551, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3552, 1)), gem_string(");\n")));
#line 1441 "compiler/main.gem"
    GemVal _t3553[] = {gem_v_p4};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3553, 1), gem_string("break;\n")));
#line 1442 "compiler/main.gem"
    GemVal _t3554[] = {gem_v_p3};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3554, 1), gem_string("}\n")));
#line 1444 "compiler/main.gem"
    GemVal _t3555[] = {gem_v_p3};
    GemVal _t3556[] = {gem_v_prev_tmp};
    GemVal _t3557[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3555, 1), gem_to_string_fn(NULL, _t3556, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3557, 1)), gem_string(";\n")));
#line 1445 "compiler/main.gem"
    GemVal _t3558[] = {gem_v_p3};
    GemVal _t3559[] = {gem_v_cur_tmp};
    GemVal _t3560[] = {gem_v_cur_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3558, 1), gem_to_string_fn(NULL, _t3559, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3560, 1)), gem_string("->next;\n")));
#line 1446 "compiler/main.gem"
    GemVal _t3561[] = {gem_v_p2};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3561, 1), gem_string("}\n")));
#line 1448 "compiler/main.gem"
    GemVal _t3562[] = {gem_v_p2};
    GemVal _t3563[] = {gem_v_matched_tmp};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3562, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3563, 1)), gem_string(" >= 0) break;\n")));
#line 1449 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1450 "compiler/main.gem"
    GemVal _t3564[] = {gem_v_p2};
    GemVal _t3565[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3564, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3565, 1)), gem_string(") {\n")));
#line 1451 "compiler/main.gem"
    GemVal _t3566[] = {gem_v_p2};
    GemVal _t3567[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3566, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3567, 1)), gem_string(" = -2;\n")));
#line 1452 "compiler/main.gem"
    GemVal _t3568[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3568, 1), gem_string("    break;\n")));
#line 1453 "compiler/main.gem"
    GemVal _t3569[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3569, 1), gem_string("}\n")));
#line 1454 "compiler/main.gem"
    GemVal _t3570[] = {gem_v_p2};
    GemVal _t3571[] = {gem_v_deadline_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3570, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3571, 1)), gem_string(");\n")));
#line 1455 "compiler/main.gem"
    GemVal _t3572[] = {gem_v_p2};
    GemVal _t3573[] = {gem_v_proc_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3572, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3573, 1)), gem_string("->timed_out) {\n")));
#line 1456 "compiler/main.gem"
    GemVal _t3574[] = {gem_v_p2};
    GemVal _t3575[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3574, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3575, 1)), gem_string(" = -2;\n")));
#line 1457 "compiler/main.gem"
    GemVal _t3576[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3576, 1), gem_string("    break;\n")));
#line 1458 "compiler/main.gem"
    GemVal _t3577[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3577, 1), gem_string("}\n")));
    } else {
#line 1460 "compiler/main.gem"
    GemVal _t3578[] = {gem_v_p2};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3578, 1), gem_string("gem_selective_yield(-1);\n")));
    }
#line 1463 "compiler/main.gem"
    GemVal _t3579[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3579, 1), gem_string("}\n")));
#line 1466 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1466 "compiler/main.gem"
    GemVal _t3580[] = {gem_v_arms};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3580, 1);
#line 1466 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1466 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1466 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1467 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1468 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1469 "compiler/main.gem"
    GemVal _t3581[] = {gem_v_p};
    GemVal _t3582[] = {gem_v_matched_tmp};
    GemVal _t3583[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3581, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3582, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3583, 1)), gem_string(") {\n")));
        } else {
#line 1471 "compiler/main.gem"
    GemVal _t3584[] = {gem_v_p};
    GemVal _t3585[] = {gem_v_matched_tmp};
    GemVal _t3586[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3584, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3585, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3586, 1)), gem_string(") {\n")));
        }
#line 1473 "compiler/main.gem"
    GemVal _t3587 = gem_v_arm;
    GemVal _t3588 = gem_table_get(_t3587, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3588, gem_string("bindings")), GEM_NIL))) {
#line 1474 "compiler/main.gem"
    GemVal _t3589 = gem_v_arm;
    GemVal _t3590 = gem_table_get(_t3589, gem_string("pattern"));
            GemVal gem_v__for_items_72 = gem_table_get(_t3590, gem_string("bindings"));
#line 1474 "compiler/main.gem"
            GemVal gem_v__for_i_72 = gem_int(0);
#line 1474 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3591[] = {gem_v__for_items_72};
                if (!gem_truthy(gem_lt(gem_v__for_i_72, gem_len_fn(NULL, _t3591, 1)))) break;
#line 1474 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_72, gem_v__for_i_72);
#line 1474 "compiler/main.gem"
                gem_v__for_i_72 = gem_add(gem_v__for_i_72, gem_int(1));
#line 1475 "compiler/main.gem"
    GemVal _t3592[] = {gem_v_b, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3593 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3593.fn(_t3593.env, _t3592, 2)), gem_string("\n"));
            }

        }
#line 1478 "compiler/main.gem"
    GemVal _t3594 = gem_v_arm;
        GemVal gem_v_body = gem_table_get(_t3594, gem_string("body"));
#line 1479 "compiler/main.gem"
    GemVal _t3595[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3595, 1), gem_int(0)))) {
#line 1480 "compiler/main.gem"
            GemVal gem_v__for_i_73 = gem_int(0);
#line 1480 "compiler/main.gem"
    GemVal _t3596[] = {gem_v_body};
            GemVal gem_v__for_limit_73 = gem_sub(gem_len_fn(NULL, _t3596, 1), gem_int(1));
#line 1480 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1480 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_73;
#line 1480 "compiler/main.gem"
                gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1481 "compiler/main.gem"
    GemVal _t3597[] = {gem_table_get(gem_v_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3598 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3598.fn(_t3598.env, _t3597, 2)), gem_string("\n"));
            }

#line 1483 "compiler/main.gem"
    GemVal _t3599[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3600 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3600.fn(_t3600.env, _t3599, 2)), gem_string("\n"));
        } else {
#line 1485 "compiler/main.gem"
    GemVal _t3601[] = {gem_v_p};
    GemVal _t3602[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3601, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3602, 1)), gem_string("    return GEM_NIL;\n")));
        }
    }

#line 1489 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1490 "compiler/main.gem"
    GemVal _t3603[] = {gem_v_p};
    GemVal _t3604[] = {gem_v_matched_tmp};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3603, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3604, 1)), gem_string(" == -2) {\n")));
#line 1491 "compiler/main.gem"
    GemVal _t3606;
    if (!gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
        _t3606 = gem_neq(gem_v_after_body, GEM_NIL);
    } else {
        GemVal _t3605[] = {gem_v_after_body};
        _t3606 = gem_gt(gem_len_fn(NULL, _t3605, 1), gem_int(0));
    }
        if (gem_truthy(_t3606)) {
#line 1492 "compiler/main.gem"
            GemVal gem_v__for_i_75 = gem_int(0);
#line 1492 "compiler/main.gem"
    GemVal _t3607[] = {gem_v_after_body};
            GemVal gem_v__for_limit_75 = gem_sub(gem_len_fn(NULL, _t3607, 1), gem_int(1));
#line 1492 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1492 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_75;
#line 1492 "compiler/main.gem"
                gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1493 "compiler/main.gem"
    GemVal _t3608[] = {gem_table_get(gem_v_after_body, gem_v_j), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3609 = (*gem_v_compile_stmt);
                gem_v_out = gem_add(gem_add(gem_v_out, _t3609.fn(_t3609.env, _t3608, 2)), gem_string("\n"));
            }

#line 1495 "compiler/main.gem"
    GemVal _t3610[] = {gem_table_get(gem_v_after_body, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3611 = (*gem_v_compile_stmt_return);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3611.fn(_t3611.env, _t3610, 2)), gem_string("\n"));
        } else {
#line 1497 "compiler/main.gem"
    GemVal _t3612[] = {gem_v_p};
    GemVal _t3613[] = {gem_v_p};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3612, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3613, 1)), gem_string("    return GEM_NIL;\n")));
        }
    } else {
#line 1500 "compiler/main.gem"
    GemVal _t3614[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3614, 1), gem_string("} else {\n")));
#line 1501 "compiler/main.gem"
    GemVal _t3615[] = {gem_v_p};
    GemVal _t3616[] = {gem_v_p};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3615, 1), gem_string("    gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3616, 1)), gem_string("    return GEM_NIL;\n")));
    }
#line 1504 "compiler/main.gem"
    GemVal _t3617[] = {gem_v_p};
    gem_v_out = gem_add(gem_v_out, gem_add(gem_to_string_fn(NULL, _t3617, 1), gem_string("}")));
    GemVal _t3618 = gem_v_out;
    gem_pop_frame();
    return _t3618;
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
#line 1511 "compiler/main.gem"
    GemVal gem_v__d76 = gem_v_node;
#line 1511 "compiler/main.gem"
    GemVal _t3620 = gem_v__d76;
    GemVal gem_v_name = gem_table_get(_t3620, gem_string("name"));
#line 1511 "compiler/main.gem"
    GemVal _t3621 = gem_v__d76;
    GemVal gem_v_params = gem_table_get(_t3621, gem_string("params"));
#line 1511 "compiler/main.gem"
    GemVal _t3622 = gem_v__d76;
    GemVal gem_v_ret_type = gem_table_get(_t3622, gem_string("ret_type"));

#line 1513 "compiler/main.gem"
    GemVal _t3623[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3623, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1515 "compiler/main.gem"
    GemVal _t3624 = gem_table_new();
    GemVal gem_v_c_args = _t3624;
#line 1516 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1516 "compiler/main.gem"
    GemVal _t3625[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t3625, 1);
#line 1516 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1516 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1516 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1517 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1518 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1519 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1520 "compiler/main.gem"
    GemVal _t3626[] = {gem_v_i};
    GemVal _t3627[] = {gem_v_i};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3626, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3627, 1)), gem_string("].ival;\n")));
#line 1521 "compiler/main.gem"
    GemVal _t3628[] = {gem_v_i};
    GemVal _t3629[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3628, 1))};
            (void)(gem_push_fn(NULL, _t3629, 2));
        } else {
#line 1522 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1523 "compiler/main.gem"
    GemVal _t3630[] = {gem_v_i};
    GemVal _t3631[] = {gem_v_i};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3630, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3631, 1)), gem_string("].fval;\n")));
#line 1524 "compiler/main.gem"
    GemVal _t3632[] = {gem_v_i};
    GemVal _t3633[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3632, 1))};
                (void)(gem_push_fn(NULL, _t3633, 2));
            } else {
#line 1525 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1526 "compiler/main.gem"
    GemVal _t3634[] = {gem_v_i};
    GemVal _t3635[] = {gem_v_i};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3634, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3635, 1)), gem_string("].sval;\n")));
#line 1527 "compiler/main.gem"
    GemVal _t3636[] = {gem_v_i};
    GemVal _t3637[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3636, 1))};
                    (void)(gem_push_fn(NULL, _t3637, 2));
                } else {
#line 1528 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1529 "compiler/main.gem"
    GemVal _t3638[] = {gem_v_i};
    GemVal _t3639[] = {gem_v_i};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3638, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3639, 1)), gem_string("].bval;\n")));
#line 1530 "compiler/main.gem"
    GemVal _t3640[] = {gem_v_i};
    GemVal _t3641[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3640, 1))};
                        (void)(gem_push_fn(NULL, _t3641, 2));
                    } else {
#line 1531 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1532 "compiler/main.gem"
    GemVal _t3642[] = {gem_v_i};
    GemVal _t3643[] = {gem_v_i};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3642, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3643, 1)), gem_string("].ival;\n")));
#line 1533 "compiler/main.gem"
    GemVal _t3644[] = {gem_v_i};
    GemVal _t3645[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3644, 1))};
                            (void)(gem_push_fn(NULL, _t3645, 2));
                        } else {
#line 1534 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1535 "compiler/main.gem"
    GemVal _t3646[] = {gem_v_i};
    GemVal _t3647[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3646, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3647, 2));
                            } else {
#line 1537 "compiler/main.gem"
    GemVal _t3648[] = {gem_v_i};
    GemVal _t3649[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3648, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3649, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1542 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1543 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1543 "compiler/main.gem"
    GemVal _t3650[] = {gem_v_c_args};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t3650, 1);
#line 1543 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1543 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1543 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1544 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1545 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1547 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1550 "compiler/main.gem"
    GemVal _t3651[] = {gem_v_name};
    GemVal _t3652[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3651, 1), gem_string("(")), gem_to_string_fn(NULL, _t3652, 1)), gem_string(")"));
#line 1552 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1553 "compiler/main.gem"
    GemVal _t3653[] = {gem_v_call_expr};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3653, 1)), gem_string(";\n")));
#line 1554 "compiler/main.gem"
        gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
    } else {
#line 1555 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1556 "compiler/main.gem"
    GemVal _t3654[] = {gem_v_call_expr};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t3654, 1)), gem_string(";\n")));
        } else {
#line 1557 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1558 "compiler/main.gem"
    GemVal _t3655[] = {gem_v_call_expr};
                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3655, 1)), gem_string(";\n")));
#line 1559 "compiler/main.gem"
                gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_int(_ret);\n"));
            } else {
#line 1560 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1561 "compiler/main.gem"
    GemVal _t3656[] = {gem_v_call_expr};
                    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3656, 1)), gem_string(";\n")));
#line 1562 "compiler/main.gem"
                    gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_float(_ret);\n"));
                } else {
#line 1563 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1564 "compiler/main.gem"
    GemVal _t3657[] = {gem_v_call_expr};
                        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3657, 1)), gem_string(";\n")));
#line 1565 "compiler/main.gem"
                        gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_string(_ret);\n"));
                    } else {
#line 1566 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1567 "compiler/main.gem"
    GemVal _t3658[] = {gem_v_call_expr};
                            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3658, 1)), gem_string(";\n")));
#line 1568 "compiler/main.gem"
                            gem_v_lines = gem_add(gem_v_lines, gem_string("    return gem_bool(_ret);\n"));
                        } else {
#line 1569 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1570 "compiler/main.gem"
    GemVal _t3659[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3659, 1)), gem_string(";\n")));
#line 1571 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n"));
                            } else {
#line 1573 "compiler/main.gem"
    GemVal _t3660[] = {gem_v_call_expr};
                                gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3660, 1)), gem_string(";\n")));
#line 1574 "compiler/main.gem"
                                gem_v_lines = gem_add(gem_v_lines, gem_string("    return GEM_NIL;\n"));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1577 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1580 "compiler/main.gem"
    GemVal _t3661[] = {gem_v_name};
    GemVal _t3662[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3661, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t3662, 2));
#line 1583 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1584 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1585 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1586 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1587 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1588 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1589 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1590 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1591 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1592 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1593 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1594 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1595 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1598 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1599 "compiler/main.gem"
    GemVal _t3663[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3663, 1), gem_int(0)))) {
#line 1600 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1602 "compiler/main.gem"
        GemVal gem_v__for_i_79 = gem_int(0);
#line 1602 "compiler/main.gem"
    GemVal _t3664[] = {gem_v_params};
        GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t3664, 1);
#line 1602 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1602 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_79;
#line 1602 "compiler/main.gem"
            gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1603 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1604 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1606 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1607 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1608 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1609 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1610 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1611 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1612 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1613 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1614 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1615 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1616 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1617 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1618 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1620 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1625 "compiler/main.gem"
    GemVal _t3665[] = {gem_v_c_ret};
    GemVal _t3666[] = {gem_v_name};
    GemVal _t3667[] = {gem_v_c_params};
    GemVal _t3668[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3665, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3666, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3667, 1)), gem_string(");"))};
    (void)(gem_push_fn(NULL, _t3668, 2));
    GemVal _t3669 = gem_v_lines;
    gem_pop_frame();
    return _t3669;
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
#line 1633 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1635 "compiler/main.gem"
    GemVal gem_v__for_i_80 = gem_int(0);
#line 1635 "compiler/main.gem"
    GemVal _t3671[] = {gem_v_params};
    GemVal gem_v__for_limit_80 = gem_len_fn(NULL, _t3671, 1);
#line 1635 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1635 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_80;
#line 1635 "compiler/main.gem"
        gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1636 "compiler/main.gem"
    GemVal _t3672[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3673 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3673.fn(_t3673.env, _t3672, 1);
#line 1637 "compiler/main.gem"
    GemVal _t3674[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3674, 2))) {
#line 1638 "compiler/main.gem"
    GemVal _t3675[] = {gem_v_mp};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3675, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1639 "compiler/main.gem"
    GemVal _t3676[] = {gem_v_mp};
    GemVal _t3677[] = {gem_v_i};
    GemVal _t3678[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3676, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3677, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3678, 1)), gem_string("] : GEM_NIL;\n")));
        } else {
#line 1641 "compiler/main.gem"
    GemVal _t3679[] = {gem_v_mp};
    GemVal _t3680[] = {gem_v_i};
    GemVal _t3681[] = {gem_v_i};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3679, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t3680, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t3681, 1)), gem_string("] : GEM_NIL;\n")));
        }
    }

#line 1646 "compiler/main.gem"
    GemVal _t3682 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3682, gem_string("rest_param")), GEM_NIL))) {
#line 1647 "compiler/main.gem"
    GemVal _t3683[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3683, 1);
#line 1648 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1649 "compiler/main.gem"
    GemVal _t3684 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3684, gem_string("block_param")), GEM_NIL))) {
#line 1650 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1652 "compiler/main.gem"
    GemVal _t3685 = gem_v_node;
    GemVal _t3686[] = {gem_table_get(_t3685, gem_string("rest_param"))};
    GemVal _t3687 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3687.fn(_t3687.env, _t3686, 1);
#line 1653 "compiler/main.gem"
    GemVal _t3688 = gem_v_node;
    GemVal _t3689[] = {gem_v_cap_set, gem_table_get(_t3688, gem_string("rest_param"))};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3689, 2))) {
#line 1654 "compiler/main.gem"
    GemVal _t3690[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3690, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1655 "compiler/main.gem"
    GemVal _t3691[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3691, 1)), gem_string(" = gem_table_new();\n")));
#line 1656 "compiler/main.gem"
    GemVal _t3692[] = {gem_v_N};
    GemVal _t3693[] = {gem_v_end_count};
    GemVal _t3694[] = {gem_v_mp_rest};
    GemVal _t3695[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3692, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3693, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t3694, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3695, 1)), gem_string("), args[_ri]);\n")));
        } else {
#line 1658 "compiler/main.gem"
    GemVal _t3696[] = {gem_v_mp_rest};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3696, 1)), gem_string(" = gem_table_new();\n")));
#line 1659 "compiler/main.gem"
    GemVal _t3697[] = {gem_v_N};
    GemVal _t3698[] = {gem_v_end_count};
    GemVal _t3699[] = {gem_v_mp_rest};
    GemVal _t3700[] = {gem_v_N};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t3697, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t3698, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t3699, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t3700, 1)), gem_string("), args[_ri]);\n")));
        }
#line 1661 "compiler/main.gem"
    GemVal _t3701 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3701, gem_string("block_param")), GEM_NIL))) {
#line 1662 "compiler/main.gem"
    GemVal _t3702 = gem_v_node;
    GemVal _t3703[] = {gem_table_get(_t3702, gem_string("block_param"))};
    GemVal _t3704 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t3704.fn(_t3704.env, _t3703, 1);
#line 1663 "compiler/main.gem"
    GemVal _t3705 = gem_v_node;
    GemVal _t3706[] = {gem_v_cap_set, gem_table_get(_t3705, gem_string("block_param"))};
            if (gem_truthy(gem_fn_set_contains(NULL, _t3706, 2))) {
#line 1664 "compiler/main.gem"
    GemVal _t3707[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3707, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")));
#line 1665 "compiler/main.gem"
    GemVal _t3708[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t3708, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            } else {
#line 1667 "compiler/main.gem"
    GemVal _t3709[] = {gem_v_mp_block};
                gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3709, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n")));
            }
        }
    }
    GemVal _t3710 = gem_v_out;
    gem_pop_frame();
    return _t3710;
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
#line 1677 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1678 "compiler/main.gem"
    GemVal _t3712[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3712, 1), gem_int(0)))) {
#line 1679 "compiler/main.gem"
        GemVal gem_v__for_i_81 = gem_int(0);
#line 1679 "compiler/main.gem"
    GemVal _t3713[] = {gem_v_body};
        GemVal gem_v__for_limit_81 = gem_sub(gem_len_fn(NULL, _t3713, 1), gem_int(1));
#line 1679 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_81, gem_v__for_limit_81))) break;
#line 1679 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_81;
#line 1679 "compiler/main.gem"
            gem_v__for_i_81 = gem_add(gem_v__for_i_81, gem_int(1));
#line 1680 "compiler/main.gem"
    GemVal _t3714[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t3715 = (*gem_v_compile_stmt);
            gem_v_out = gem_add(gem_add(gem_v_out, _t3715.fn(_t3715.env, _t3714, 2)), gem_string("\n"));
        }

#line 1682 "compiler/main.gem"
    GemVal _t3716[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t3717 = (*gem_v_compile_stmt_return);
        gem_v_out = gem_add(gem_add(gem_v_out, _t3717.fn(_t3717.env, _t3716, 2)), gem_string("\n"));
    } else {
#line 1684 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n"));
    }
    GemVal _t3718 = gem_v_out;
    gem_pop_frame();
    return _t3718;
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
#line 1692 "compiler/main.gem"
    GemVal gem_v__d82 = gem_v_node;
#line 1692 "compiler/main.gem"
    GemVal _t3720 = gem_v__d82;
    GemVal gem_v_params = gem_table_get(_t3720, gem_string("params"));
#line 1692 "compiler/main.gem"
    GemVal _t3721 = gem_v__d82;
    GemVal gem_v_body = gem_table_get(_t3721, gem_string("body"));
#line 1692 "compiler/main.gem"
    GemVal _t3722 = gem_v__d82;
    GemVal gem_v_name = gem_table_get(_t3722, gem_string("name"));

#line 1695 "compiler/main.gem"
    GemVal _t3723[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn_sorted_array_to_set(NULL, _t3723, 1);
#line 1696 "compiler/main.gem"
    GemVal _t3724 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3724, gem_string("rest_param")), GEM_NIL))) {
#line 1697 "compiler/main.gem"
    GemVal _t3725 = gem_v_node;
    GemVal _t3726[] = {gem_v_scope_vars, gem_table_get(_t3725, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3726, 2));
    }
#line 1699 "compiler/main.gem"
    GemVal _t3727 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3727, gem_string("block_param")), GEM_NIL))) {
#line 1700 "compiler/main.gem"
    GemVal _t3728 = gem_v_node;
    GemVal _t3729[] = {gem_v_scope_vars, gem_table_get(_t3728, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3729, 2));
    }
#line 1702 "compiler/main.gem"
    GemVal _t3730[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn_collect_top_let_names(NULL, _t3730, 2));
#line 1704 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1705 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1706 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1707 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1709 "compiler/main.gem"
    GemVal _t3731[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t3732 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t3732.fn(_t3732.env, _t3731, 2);
#line 1711 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1712 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1714 "compiler/main.gem"
    GemVal _t3733[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3733, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1717 "compiler/main.gem"
    GemVal _t3734 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3734, gem_string("line")), GEM_NIL))) {
#line 1718 "compiler/main.gem"
    GemVal _t3735 = gem_v_node;
    GemVal _t3736[] = {gem_table_get(_t3735, gem_string("line"))};
    GemVal _t3737[] = {(*gem_v_source_name)};
    GemVal _t3738[] = {gem_fn_escape_c_string(NULL, _t3737, 1)};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3736, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3738, 1)), gem_string("\"\n")));
    }
#line 1721 "compiler/main.gem"
    GemVal _t3739[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t3740 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3740.fn(_t3740.env, _t3739, 3));
#line 1723 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1724 "compiler/main.gem"
    GemVal _t3741 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3741, gem_string("line")), GEM_NIL))) {
#line 1725 "compiler/main.gem"
    GemVal _t3742 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t3742, gem_string("line"));
    }
#line 1727 "compiler/main.gem"
    GemVal _t3743[] = {gem_v_name};
    GemVal _t3744[] = {gem_fn_escape_c_string(NULL, _t3743, 1)};
    GemVal _t3745[] = {(*gem_v_source_name)};
    GemVal _t3746[] = {gem_fn_escape_c_string(NULL, _t3745, 1)};
    GemVal _t3747[] = {gem_v_fn_line};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3744, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3746, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t3747, 1)), gem_string(");\n")));
#line 1729 "compiler/main.gem"
    GemVal _t3748[] = {gem_v_body};
    GemVal _t3749 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3749.fn(_t3749.env, _t3748, 1));
#line 1731 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1732 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1733 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1734 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
    GemVal _t3750 = gem_v_lines;
    gem_pop_frame();
    return _t3750;
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
#line 1741 "compiler/main.gem"
    GemVal gem_v__d83 = gem_v_node;
#line 1741 "compiler/main.gem"
    GemVal _t3752 = gem_v__d83;
    GemVal gem_v_params = gem_table_get(_t3752, gem_string("params"));
#line 1741 "compiler/main.gem"
    GemVal _t3753 = gem_v__d83;
    GemVal gem_v_body = gem_table_get(_t3753, gem_string("body"));

#line 1742 "compiler/main.gem"
    GemVal _t3754 = (*gem_v_anon_name);
    GemVal gem_v_name = _t3754.fn(_t3754.env, NULL, 0);
#line 1745 "compiler/main.gem"
    GemVal _t3755[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn_sorted_array_to_set(NULL, _t3755, 1);
#line 1746 "compiler/main.gem"
    GemVal _t3756 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3756, gem_string("rest_param")), GEM_NIL))) {
#line 1747 "compiler/main.gem"
    GemVal _t3757 = gem_v_node;
    GemVal _t3758[] = {gem_v_inner_scope, gem_table_get(_t3757, gem_string("rest_param"))};
        (void)(gem_fn_set_add(NULL, _t3758, 2));
    }
#line 1749 "compiler/main.gem"
    GemVal _t3759 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3759, gem_string("block_param")), GEM_NIL))) {
#line 1750 "compiler/main.gem"
    GemVal _t3760 = gem_v_node;
    GemVal _t3761[] = {gem_v_inner_scope, gem_table_get(_t3760, gem_string("block_param"))};
        (void)(gem_fn_set_add(NULL, _t3761, 2));
    }
#line 1752 "compiler/main.gem"
    GemVal _t3762[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn_collect_top_let_names(NULL, _t3762, 2));
#line 1754 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1755 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1756 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1757 "compiler/main.gem"
    GemVal _t3763[] = {gem_v_captures};
    GemVal _t3764[] = {gem_v_inner_scope, gem_fn_sorted_array_to_set(NULL, _t3763, 1)};
    *gem_v_local_names = gem_fn_set_union(NULL, _t3764, 2);
#line 1759 "compiler/main.gem"
    GemVal _t3765[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t3766 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t3766.fn(_t3766.env, _t3765, 2);
#line 1761 "compiler/main.gem"
    GemVal _t3767[] = {gem_v_captures};
    GemVal _t3768[] = {gem_v_inner_captured, gem_fn_sorted_array_to_set(NULL, _t3767, 1)};
    *gem_v_boxed_vars = gem_fn_set_union(NULL, _t3768, 2);
#line 1762 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1765 "compiler/main.gem"
    GemVal _t3769[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t3769, 1));
#line 1766 "compiler/main.gem"
    GemVal _t3770[] = {gem_v_struct_name};
    GemVal gem_v_struct_def = gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t3770, 1)), gem_string(" {\n"));
#line 1767 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_captures;
#line 1767 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1767 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3771[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t3771, 1)))) break;
#line 1767 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1767 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1768 "compiler/main.gem"
    GemVal _t3772[] = {gem_v_cap};
    GemVal _t3773 = (*gem_v_mangle);
    GemVal _t3774[] = {_t3773.fn(_t3773.env, _t3772, 1)};
        gem_v_struct_def = gem_add(gem_v_struct_def, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3774, 1)), gem_string(";\n")));
    }

#line 1770 "compiler/main.gem"
    gem_v_struct_def = gem_add(gem_v_struct_def, gem_string("};\n"));
#line 1773 "compiler/main.gem"
    GemVal _t3775[] = {gem_v_name};
    GemVal gem_v_lines = gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t3775, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"));
#line 1774 "compiler/main.gem"
    GemVal _t3776[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3776, 1), gem_int(0)))) {
#line 1775 "compiler/main.gem"
    GemVal _t3777[] = {gem_v_struct_name};
    GemVal _t3778[] = {gem_v_struct_name};
        gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t3777, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t3778, 1)), gem_string(" *)_env;\n")));
#line 1776 "compiler/main.gem"
        GemVal gem_v__for_items_85 = gem_v_captures;
#line 1776 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1776 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3779[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t3779, 1)))) break;
#line 1776 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1776 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1777 "compiler/main.gem"
    GemVal _t3780[] = {gem_v_cap};
    GemVal _t3781 = (*gem_v_mangle);
            GemVal gem_v_mc = _t3781.fn(_t3781.env, _t3780, 1);
#line 1778 "compiler/main.gem"
    GemVal _t3782[] = {gem_v_mc};
    GemVal _t3783[] = {gem_v_mc};
            gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t3782, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t3783, 1)), gem_string(";\n")));
        }

    }
#line 1782 "compiler/main.gem"
    GemVal _t3784[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t3785 = (*gem_v_emit_param_bindings);
    gem_v_lines = gem_add(gem_v_lines, _t3785.fn(_t3785.env, _t3784, 3));
#line 1784 "compiler/main.gem"
    GemVal _t3786[] = {gem_v_name};
    GemVal _t3787[] = {gem_fn_escape_c_string(NULL, _t3786, 1)};
    GemVal _t3788[] = {(*gem_v_source_name)};
    GemVal _t3789[] = {gem_fn_escape_c_string(NULL, _t3788, 1)};
    gem_v_lines = gem_add(gem_v_lines, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t3787, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t3789, 1)), gem_string("\", 0);\n")));
#line 1786 "compiler/main.gem"
    GemVal _t3790[] = {gem_v_body};
    GemVal _t3791 = (*gem_v_emit_fn_body);
    gem_v_lines = gem_add(gem_v_lines, _t3791.fn(_t3791.env, _t3790, 1));
#line 1788 "compiler/main.gem"
    gem_v_lines = gem_add(gem_v_lines, gem_string("}"));
#line 1789 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1790 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1791 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1794 "compiler/main.gem"
    GemVal _t3792[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_v_lines)};
    (void)(gem_push_fn(NULL, _t3792, 2));
    GemVal _t3793 = gem_table_new();
    gem_table_set(_t3793, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t3793, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t3793, gem_string("captures"), gem_v_captures);
    GemVal _t3794 = _t3793;
    gem_pop_frame();
    return _t3794;
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
#line 1803 "compiler/main.gem"
    GemVal _t3796 = gem_table_new();
    GemVal gem_v_fn_defs = _t3796;
#line 1804 "compiler/main.gem"
    GemVal _t3797 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t3797;
#line 1805 "compiler/main.gem"
    GemVal _t3798 = gem_table_new();
    GemVal gem_v_extern_includes = _t3798;
#line 1806 "compiler/main.gem"
    GemVal _t3799 = gem_table_new();
    GemVal gem_v_top_stmts = _t3799;
#line 1808 "compiler/main.gem"
    GemVal _t3800 = gem_v_ast;
    GemVal gem_v__for_items_86 = gem_table_get(_t3800, gem_string("stmts"));
#line 1808 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1808 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3801[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t3801, 1)))) break;
#line 1808 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1808 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1809 "compiler/main.gem"
    GemVal _t3802 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t3802, gem_string("tag")), gem_string("fn_def")))) {
#line 1810 "compiler/main.gem"
    GemVal _t3803[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t3803, 2));
        } else {
#line 1811 "compiler/main.gem"
    GemVal _t3804 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t3804, gem_string("tag")), gem_string("extern_fn")))) {
#line 1812 "compiler/main.gem"
    GemVal _t3805[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t3805, 2));
            } else {
#line 1813 "compiler/main.gem"
    GemVal _t3806 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t3806, gem_string("tag")), gem_string("extern_include")))) {
#line 1814 "compiler/main.gem"
    GemVal _t3807[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3807, 2));
                } else {
#line 1816 "compiler/main.gem"
    GemVal _t3808[] = {gem_v_top_stmts, gem_v_s};
                    (void)(gem_push_fn(NULL, _t3808, 2));
                }
            }
        }
    }

#line 1821 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_extern_fns_list;
#line 1821 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1821 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3809[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t3809, 1)))) break;
#line 1821 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1821 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1822 "compiler/main.gem"
    GemVal _t3810 = gem_v_ef;
    GemVal _t3811[] = {(*gem_v_defined_fns), gem_table_get(_t3810, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3811, 2));
    }

#line 1826 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_fn_defs;
#line 1826 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1826 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3812[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t3812, 1)))) break;
#line 1826 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1826 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1827 "compiler/main.gem"
    GemVal _t3813 = gem_v_fd;
    GemVal _t3814[] = {(*gem_v_defined_fns), gem_table_get(_t3813, gem_string("name"))};
        (void)(gem_fn_set_add(NULL, _t3814, 2));
#line 1828 "compiler/main.gem"
    GemVal _t3815 = gem_v_fd;
    GemVal _t3816[] = {gem_table_get(_t3815, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3816, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1829 "compiler/main.gem"
    GemVal _t3817[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t3817, 2));
    }

#line 1833 "compiler/main.gem"
    GemVal _t3818[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn_collect_top_let_names(NULL, _t3818, 2));
#line 1834 "compiler/main.gem"
    GemVal _t3819 = gem_table_new();
    GemVal _t3820[] = {gem_v_top_stmts, _t3819};
    GemVal _t3821 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t3821.fn(_t3821.env, _t3820, 2);
#line 1837 "compiler/main.gem"
    GemVal gem_v__for_items_89 = gem_v_extern_fns_list;
#line 1837 "compiler/main.gem"
    GemVal gem_v__for_i_89 = gem_int(0);
#line 1837 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3822[] = {gem_v__for_items_89};
        if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t3822, 1)))) break;
#line 1837 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1837 "compiler/main.gem"
        gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1838 "compiler/main.gem"
    GemVal _t3823[] = {gem_v_ef};
    GemVal _t3824 = (*gem_v_compile_extern_fn);
    GemVal _t3825[] = {(*gem_v_functions), _t3824.fn(_t3824.env, _t3823, 1)};
        (void)(gem_push_fn(NULL, _t3825, 2));
    }

#line 1842 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_fn_defs;
#line 1842 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1842 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3826[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t3826, 1)))) break;
#line 1842 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1842 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1843 "compiler/main.gem"
    GemVal _t3827[] = {gem_v_fd};
    GemVal _t3828 = (*gem_v_compile_fn);
    GemVal _t3829[] = {(*gem_v_functions), _t3828.fn(_t3828.env, _t3827, 1)};
        (void)(gem_push_fn(NULL, _t3829, 2));
    }

#line 1847 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1848 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1849 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1850 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1851 "compiler/main.gem"
    GemVal _t3830 = gem_table_new();
    *gem_v_local_names = _t3830;
#line 1852 "compiler/main.gem"
    GemVal gem_v__for_tbl_91 = (*gem_v_top_level_vars);
#line 1852 "compiler/main.gem"
    GemVal _t3831[] = {gem_v__for_tbl_91};
    GemVal gem_v__for_keys_91 = gem_keys_fn(NULL, _t3831, 1);
#line 1852 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1852 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3832[] = {gem_v__for_keys_91};
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_len_fn(NULL, _t3832, 1)))) break;
#line 1852 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_91, gem_v__for_i_91);
#line 1852 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_91, gem_table_get(gem_v__for_keys_91, gem_v__for_i_91));
#line 1852 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1853 "compiler/main.gem"
    GemVal _t3833[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn_set_add(NULL, _t3833, 2));
    }

#line 1855 "compiler/main.gem"
    GemVal _t3834[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t3835 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t3835.fn(_t3835.env, _t3834, 2);
#line 1856 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1857 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1858 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1860 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1861 "compiler/main.gem"
    GemVal gem_v__for_items_92 = gem_v_fn_defs;
#line 1861 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1861 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3836[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t3836, 1)))) break;
#line 1861 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1861 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1862 "compiler/main.gem"
    GemVal _t3837 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t3837, gem_string("name")), gem_string("main")))) {
#line 1863 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1867 "compiler/main.gem"
    GemVal gem_v_main_c = gem_string("int main(int argc, char **argv) {\n");
#line 1868 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GC_INIT();\n"));
#line 1869 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_init(argc, argv);\n"));
#line 1870 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_scheduler_init();\n"));
#line 1871 "compiler/main.gem"
    GemVal _t3838[] = {(*gem_v_source_name)};
    GemVal _t3839[] = {gem_fn_escape_c_string(NULL, _t3838, 1)};
    gem_v_main_c = gem_add(gem_v_main_c, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t3839, 1)), gem_string("\", 0);\n")));
#line 1872 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1873 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_v_main_body);
    }
#line 1875 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1876 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    GemVal _margs[1] = {GEM_NIL};\n"));
#line 1877 "compiler/main.gem"
        gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_fn_main(NULL, _margs, 0);\n"));
    }
#line 1879 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_run_scheduler();\n"));
#line 1880 "compiler/main.gem"
    gem_v_main_c = gem_add(gem_v_main_c, gem_string("    gem_pop_frame();\n    return 0;\n}\n"));
#line 1884 "compiler/main.gem"
    GemVal gem_v_out = gem_string("");
#line 1885 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_includes;
#line 1885 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 1885 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3840[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t3840, 1)))) break;
#line 1885 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 1885 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 1886 "compiler/main.gem"
    GemVal _t3841 = gem_v_ei;
    GemVal _t3842[] = {gem_table_get(_t3841, gem_string("path"))};
        gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t3842, 1)), gem_string("\"\n")));
    }

#line 1888 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("#include \"gem.h\"\n"));
#line 1892 "compiler/main.gem"
    GemVal _t3843[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3843, 1), gem_int(0)))) {
#line 1893 "compiler/main.gem"
        GemVal gem_v__for_i_94 = gem_int(0);
#line 1893 "compiler/main.gem"
    GemVal _t3844[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_94 = gem_len_fn(NULL, _t3844, 1);
#line 1893 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_v__for_limit_94))) break;
#line 1893 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_94;
#line 1893 "compiler/main.gem"
            gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 1894 "compiler/main.gem"
            gem_v_out = gem_add(gem_add(gem_v_out, gem_table_get((*gem_v_extern_c_decls), gem_v_i)), gem_string("\n"));
        }

    }
#line 1899 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 1899 "compiler/main.gem"
    GemVal _t3845[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_95 = gem_len_fn(NULL, _t3845, 1);
#line 1899 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_v__for_limit_95))) break;
#line 1899 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_95;
#line 1899 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 1900 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_forward_decls), gem_v_i));
#line 1901 "compiler/main.gem"
    GemVal _t3846[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3846, 1), gem_int(1))))) {
#line 1902 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n"));
        }
    }

#line 1905 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1908 "compiler/main.gem"
    GemVal _t3847[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t3847, 1);
#line 1909 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 1909 "compiler/main.gem"
    GemVal _t3848[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_96 = gem_len_fn(NULL, _t3848, 1);
#line 1909 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_v__for_limit_96))) break;
#line 1909 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_96;
#line 1909 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 1910 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 1911 "compiler/main.gem"
    GemVal _t3849[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn_set_contains(NULL, _t3849, 2))) {
#line 1912 "compiler/main.gem"
    GemVal _t3850[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t3850, 1)), gem_string(";\n")));
        } else {
#line 1914 "compiler/main.gem"
    GemVal _t3851[] = {gem_v_vname};
            gem_v_out = gem_add(gem_v_out, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t3851, 1)), gem_string(";\n")));
        }
    }

#line 1917 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n"));
#line 1920 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 1920 "compiler/main.gem"
    GemVal _t3852[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_97 = gem_len_fn(NULL, _t3852, 1);
#line 1920 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_limit_97))) break;
#line 1920 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_97;
#line 1920 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 1921 "compiler/main.gem"
        gem_v_out = gem_add(gem_v_out, gem_table_get((*gem_v_functions), gem_v_i));
#line 1922 "compiler/main.gem"
    GemVal _t3853[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3853, 1), gem_int(1))))) {
#line 1923 "compiler/main.gem"
            gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
        }
    }

#line 1926 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_string("\n\n"));
#line 1928 "compiler/main.gem"
    gem_v_out = gem_add(gem_v_out, gem_v_main_c);
    GemVal _t3854 = gem_v_out;
    gem_pop_frame();
    return _t3854;
}

static GemVal gem_fn_make_codegen(void *_env, GemVal *args, int argc) {
#line 127 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_codegen", "compiler/main.gem", 127);
#line 128 "compiler/main.gem"
    GemVal _t1593 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1593;
#line 129 "compiler/main.gem"
    GemVal _t1594 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1594;
#line 130 "compiler/main.gem"
    GemVal _t1595 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1595;
#line 131 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 132 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 133 "compiler/main.gem"
    GemVal _t1596 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1596;
#line 134 "compiler/main.gem"
    GemVal _t1597 = gem_table_new();
    gem_table_set(_t1597, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1597, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1597, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1597, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1597, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1597, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1597, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1597, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1597, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1597, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1597, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1597, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1597, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1597, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1597, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1597, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1597, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1597, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1597, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1597, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1597, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1597, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1597, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1597, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1597, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1597, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1597, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1597, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1597, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1597, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1597, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1597, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1597, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1597, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1597, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1597, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1597, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1597, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1597, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1597, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1597, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1597, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1597, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1597, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1597, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1597, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1597, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1597, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1597, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1597, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1597, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1597, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1597, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1597, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1597, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1597, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1597, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1597, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1597, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1597, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1597, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1597, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1597, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1597, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1597, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1597, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1597, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1597, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1597, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1597, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1597, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1597;
#line 207 "compiler/main.gem"
    GemVal _t1598 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1598;
#line 208 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 208 "compiler/main.gem"
    GemVal _t1599[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1599, 1);
#line 208 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 208 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1600[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1600, 1)))) break;
#line 208 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 208 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 208 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 209 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 211 "compiler/main.gem"
    GemVal _t1601 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1601;
#line 212 "compiler/main.gem"
    GemVal _t1602 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1602;
#line 213 "compiler/main.gem"
    GemVal _t1603 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1603;
#line 214 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 215 "compiler/main.gem"
    GemVal _t1604 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1604;
#line 219 "compiler/main.gem"
    struct _closure__anon_24 *_t1607 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1607->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1607);
#line 224 "compiler/main.gem"
    struct _closure__anon_25 *_t1610 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1610->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1610);
#line 229 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_26, NULL);
#line 235 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 236 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 238 "compiler/main.gem"
    struct _closure__anon_27 *_t1923 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1923->gem_v_builtins = gem_v_builtins;
    _t1923->gem_v_collect_free_node = gem_v_collect_free_node;
    _t1923->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t1923->gem_v_defined_fns = gem_v_defined_fns;
    _t1923->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t1923);
#line 432 "compiler/main.gem"
    struct _closure__anon_28 *_t1935 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1935->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t1935);
#line 447 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 448 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 450 "compiler/main.gem"
    struct _closure__anon_29 *_t2124 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2124->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2124->gem_v_walk_captures = gem_v_walk_captures;
    _t2124->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2124);
#line 562 "compiler/main.gem"
    struct _closure__anon_30 *_t2134 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2134->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2134);
#line 573 "compiler/main.gem"
    struct _closure__anon_31 *_t2139 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2139->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2139);
#line 581 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 582 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 583 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 584 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 585 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 586 "compiler/main.gem"
    GemVal *gem_v_compile_if_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if_return = GEM_NIL;
#line 587 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 588 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 589 "compiler/main.gem"
    GemVal *gem_v_compile_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match_return = GEM_NIL;
#line 590 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 591 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match_return = GEM_NIL;
#line 592 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 593 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 594 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 595 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 596 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 597 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 598 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 599 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 603 "compiler/main.gem"
    struct _closure__anon_32 *_t2339 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2339->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2339->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2339->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2339->gem_v_compile_array = gem_v_compile_array;
    _t2339->gem_v_compile_binop = gem_v_compile_binop;
    _t2339->gem_v_compile_call = gem_v_compile_call;
    _t2339->gem_v_compile_expr = gem_v_compile_expr;
    _t2339->gem_v_compile_table = gem_v_compile_table;
    _t2339->gem_v_defined_fns = gem_v_defined_fns;
    _t2339->gem_v_local_names = gem_v_local_names;
    _t2339->gem_v_mangle = gem_v_mangle;
    _t2339->gem_v_tmp = gem_v_tmp;
    _t2339->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_32, _t2339);
#line 690 "compiler/main.gem"
    struct _closure__anon_33 *_t2356 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2356->gem_v_compile_expr = gem_v_compile_expr;
    _t2356->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_33, _t2356);
#line 705 "compiler/main.gem"
    struct _closure__anon_34 *_t2370 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2370->gem_v_compile_expr = gem_v_compile_expr;
    _t2370->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_34, _t2370);
#line 719 "compiler/main.gem"
    struct _closure__anon_35 *_t2415 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2415->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2415->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2415->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2415->gem_v_mangle = gem_v_mangle;
    _t2415->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_35, _t2415);
#line 754 "compiler/main.gem"
    struct _closure__anon_36 *_t2431 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2431->gem_v_compile_expr = gem_v_compile_expr;
    _t2431->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_36, _t2431);
#line 775 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_37, NULL);
#line 784 "compiler/main.gem"
    struct _closure__anon_38 *_t2523 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2523->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2523->gem_v_compile_args = gem_v_compile_args;
    _t2523->gem_v_compile_expr = gem_v_compile_expr;
    _t2523->gem_v_defined_fns = gem_v_defined_fns;
    _t2523->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2523->gem_v_local_names = gem_v_local_names;
    _t2523->gem_v_source_name = gem_v_source_name;
    _t2523->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_38, _t2523);
#line 828 "compiler/main.gem"
    struct _closure__anon_39 *_t2676 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2676->gem_v_compile_expr = gem_v_compile_expr;
    _t2676->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_39, _t2676);
#line 907 "compiler/main.gem"
    struct _closure__anon_40 *_t2681 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2681->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_40, _t2681);
#line 917 "compiler/main.gem"
    struct _closure__anon_41 *_t2932 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2932->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2932->gem_v_compile_expr = gem_v_compile_expr;
    _t2932->gem_v_compile_if = gem_v_compile_if;
    _t2932->gem_v_compile_match = gem_v_compile_match;
    _t2932->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t2932->gem_v_compile_stmt = gem_v_compile_stmt;
    _t2932->gem_v_compile_while = gem_v_compile_while;
    _t2932->gem_v_in_top_level = gem_v_in_top_level;
    _t2932->gem_v_mangle = gem_v_mangle;
    _t2932->gem_v_source_name = gem_v_source_name;
    _t2932->gem_v_tmp = gem_v_tmp;
    _t2932->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t2932->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_41, _t2932);
#line 993 "compiler/main.gem"
    struct _closure__anon_42 *_t3156 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t3156->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3156->gem_v_compile_expr = gem_v_compile_expr;
    _t3156->gem_v_compile_if_return = gem_v_compile_if_return;
    _t3156->gem_v_compile_match_return = gem_v_compile_match_return;
    _t3156->gem_v_compile_receive_match_return = gem_v_compile_receive_match_return;
    _t3156->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3156->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3156->gem_v_compile_while = gem_v_compile_while;
    _t3156->gem_v_mangle = gem_v_mangle;
    _t3156->gem_v_source_name = gem_v_source_name;
    _t3156->gem_v_tmp = gem_v_tmp;
    _t3156->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_42, _t3156);
#line 1060 "compiler/main.gem"
    struct _closure__anon_43 *_t3175 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3175->gem_v_compile_expr = gem_v_compile_expr;
    _t3175->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_if = gem_make_fn(_anon_43, _t3175);
#line 1076 "compiler/main.gem"
    struct _closure__anon_44 *_t3209 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3209->gem_v_compile_expr = gem_v_compile_expr;
    _t3209->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3209->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    *gem_v_compile_if_return = gem_make_fn(_anon_44, _t3209);
#line 1111 "compiler/main.gem"
    struct _closure__anon_45 *_t3228 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3228->gem_v_compile_expr = gem_v_compile_expr;
    _t3228->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_45, _t3228);
#line 1127 "compiler/main.gem"
    struct _closure__anon_46 *_t3285 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3285->gem_v_compile_expr = gem_v_compile_expr;
    _t3285->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3285->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3285->gem_v_mangle = gem_v_mangle;
    _t3285->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_46, _t3285);
#line 1178 "compiler/main.gem"
    struct _closure__anon_47 *_t3365 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3365->gem_v_compile_expr = gem_v_compile_expr;
    _t3365->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3365->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3365->gem_v_mangle = gem_v_mangle;
    _t3365->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match_return = gem_make_fn(_anon_47, _t3365);
#line 1257 "compiler/main.gem"
    struct _closure__anon_48 *_t3484 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3484->gem_v_compile_expr = gem_v_compile_expr;
    _t3484->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3484->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3484->gem_v_mangle = gem_v_mangle;
    _t3484->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_48, _t3484);
#line 1382 "compiler/main.gem"
    struct _closure__anon_49 *_t3619 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3619->gem_v_compile_expr = gem_v_compile_expr;
    _t3619->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3619->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3619->gem_v_mangle = gem_v_mangle;
    _t3619->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match_return = gem_make_fn(_anon_49, _t3619);
#line 1510 "compiler/main.gem"
    struct _closure__anon_50 *_t3670 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3670->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3670->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_50, _t3670);
#line 1632 "compiler/main.gem"
    struct _closure__anon_51 *_t3711 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3711->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_51, _t3711);
#line 1676 "compiler/main.gem"
    struct _closure__anon_52 *_t3719 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3719->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3719->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_52, _t3719);
#line 1691 "compiler/main.gem"
    struct _closure__anon_53 *_t3751 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3751->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3751->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3751->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3751->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3751->gem_v_in_top_level = gem_v_in_top_level;
    _t3751->gem_v_local_names = gem_v_local_names;
    _t3751->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_fn = gem_make_fn(_anon_53, _t3751);
#line 1740 "compiler/main.gem"
    struct _closure__anon_54 *_t3795 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t3795->gem_v_anon_name = gem_v_anon_name;
    _t3795->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3795->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3795->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3795->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3795->gem_v_functions = gem_v_functions;
    _t3795->gem_v_in_top_level = gem_v_in_top_level;
    _t3795->gem_v_local_names = gem_v_local_names;
    _t3795->gem_v_mangle = gem_v_mangle;
    _t3795->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_54, _t3795);
#line 1801 "compiler/main.gem"
    struct _closure__anon_55 *_t3855 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t3855->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3855->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t3855->gem_v_compile_fn = gem_v_compile_fn;
    _t3855->gem_v_compile_stmts = gem_v_compile_stmts;
    _t3855->gem_v_defined_fns = gem_v_defined_fns;
    _t3855->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t3855->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3855->gem_v_forward_decls = gem_v_forward_decls;
    _t3855->gem_v_functions = gem_v_functions;
    _t3855->gem_v_in_top_level = gem_v_in_top_level;
    _t3855->gem_v_local_names = gem_v_local_names;
    _t3855->gem_v_source_name = gem_v_source_name;
    _t3855->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3855->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_55, _t3855);
    GemVal _t3856 = gem_table_new();
    gem_table_set(_t3856, gem_string("compile"), gem_v_compile);
    GemVal _t3857 = _t3856;
    gem_pop_frame();
    return _t3857;
}

static GemVal gem_fn_str_replace(void *_env, GemVal *args, int argc) {
#line 1938 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_old = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_new_s = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("str_replace", "compiler/main.gem", 1938);
#line 1939 "compiler/main.gem"
    GemVal gem_v_result = gem_string("");
#line 1940 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 1941 "compiler/main.gem"
    GemVal _t3858[] = {gem_v_old};
    GemVal gem_v_old_len = gem_len_fn(NULL, _t3858, 1);
#line 1942 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_s};
    GemVal gem_v_s_len = gem_len_fn(NULL, _t3859, 1);
#line 1943 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_s_len))) break;
#line 1944 "compiler/main.gem"
        if (gem_truthy(gem_le(gem_add(gem_v_i, gem_v_old_len), gem_v_s_len))) {
#line 1945 "compiler/main.gem"
            GemVal gem_v_found = gem_bool(1);
#line 1946 "compiler/main.gem"
            GemVal gem_v_j = gem_int(0);
#line 1947 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v_j, gem_v_old_len))) break;
#line 1948 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_add(gem_v_i, gem_v_j)), gem_table_get(gem_v_old, gem_v_j)))) {
#line 1949 "compiler/main.gem"
                    gem_v_found = gem_bool(0);
                    break;
                }
#line 1952 "compiler/main.gem"
                gem_v_j = gem_add(gem_v_j, gem_int(1));
            }
#line 1954 "compiler/main.gem"
            if (gem_truthy(gem_v_found)) {
#line 1955 "compiler/main.gem"
                gem_v_result = gem_add(gem_v_result, gem_v_new_s);
#line 1956 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_v_old_len);
                continue;
            }
        }
#line 1960 "compiler/main.gem"
        gem_v_result = gem_add(gem_v_result, gem_table_get(gem_v_s, gem_v_i));
#line 1961 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t3860 = gem_v_result;
    gem_pop_frame();
    return _t3860;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 14 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 14);
#line 15 "compiler/main.gem"
    GemVal _t3861 = gem_table_new();
    GemVal gem_v_new_stmts = _t3861;
#line 16 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 17 "compiler/main.gem"
    GemVal _t3862 = gem_v_ast;
    GemVal gem_v__for_items_1 = gem_table_get(_t3862, gem_string("stmts"));
#line 17 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 17 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3863[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t3863, 1)))) break;
#line 17 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 17 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 18 "compiler/main.gem"
    GemVal _t3864[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t3864, 2))) {
#line 19 "compiler/main.gem"
    GemVal _t3865 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t3865, gem_string("path"));
#line 21 "compiler/main.gem"
    GemVal _t3866 = gem_v_string;
    GemVal _t3867[] = {gem_v_path, gem_string(".gem")};
    GemVal _t3868 = gem_table_get(_t3866, gem_string("ends_with"));
            if (gem_truthy(gem_not(_t3868.fn(_t3868.env, _t3867, 2)))) {
#line 22 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 26 "compiler/main.gem"
    GemVal _t3869[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t3869, 2);
#line 29 "compiler/main.gem"
    GemVal _t3870[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t3870, 1)))) {
#line 30 "compiler/main.gem"
    GemVal _t3871[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t3871, 2);
            }
#line 33 "compiler/main.gem"
    GemVal _t3872[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t3872, 1);
#line 36 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_table_get(gem_v_loaded, gem_v_full_path), gem_bool(1)))) {
                continue;
            }
#line 39 "compiler/main.gem"
            gem_table_set(gem_v_loaded, gem_v_full_path, gem_bool(1));
#line 41 "compiler/main.gem"
    GemVal _t3873[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t3873, 1);
#line 43 "compiler/main.gem"
    GemVal _t3874[] = {gem_v_source, gem_v_full_path};
            GemVal gem_v_loaded_ast = gem_fn_parse_source(NULL, _t3874, 2);
#line 44 "compiler/main.gem"
    GemVal _t3875[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t3875, 1);
#line 45 "compiler/main.gem"
    GemVal _t3876[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t3876, 3);
#line 47 "compiler/main.gem"
    GemVal _t3877 = gem_v_resolved;
            GemVal gem_v__for_items_2 = gem_table_get(_t3877, gem_string("stmts"));
#line 47 "compiler/main.gem"
            GemVal gem_v__for_i_2 = gem_int(0);
#line 47 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3878[] = {gem_v__for_items_2};
                if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t3878, 1)))) break;
#line 47 "compiler/main.gem"
                GemVal gem_v_rs = gem_table_get(gem_v__for_items_2, gem_v__for_i_2);
#line 47 "compiler/main.gem"
                gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 48 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_rs);
#line 49 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }

        } else {
#line 52 "compiler/main.gem"
            gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 53 "compiler/main.gem"
            gem_v_count = gem_add(gem_v_count, gem_int(1));
        }
    }

#line 56 "compiler/main.gem"
    GemVal _t3879[] = {gem_v_new_stmts};
    GemVal _t3880 = gem_fn_make_program(NULL, _t3879, 1);
    gem_pop_frame();
    return _t3880;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_push_frame("main", "compiler/main.gem", 0);
#line 178 "compiler/main.gem"
    GemVal _t3881 = gem_table_new();
    gem_table_set(_t3881, gem_string("split"), gem_make_fn(gem_fn__string_split, NULL));
    gem_table_set(_t3881, gem_string("index_of"), gem_make_fn(gem_fn__string_index_of, NULL));
    gem_table_set(_t3881, gem_string("join"), gem_make_fn(gem_fn__string_join, NULL));
    gem_table_set(_t3881, gem_string("trim"), gem_make_fn(gem_fn__string_trim, NULL));
    gem_table_set(_t3881, gem_string("starts_with"), gem_make_fn(gem_fn__string_starts_with, NULL));
    gem_table_set(_t3881, gem_string("ends_with"), gem_make_fn(gem_fn__string_ends_with, NULL));
    gem_table_set(_t3881, gem_string("upper"), gem_make_fn(gem_fn__string_upper, NULL));
    gem_table_set(_t3881, gem_string("lower"), gem_make_fn(gem_fn__string_lower, NULL));
    gem_table_set(_t3881, gem_string("contains"), gem_make_fn(gem_fn__string_contains, NULL));
    gem_table_set(_t3881, gem_string("repeat"), gem_make_fn(gem_fn__string_repeat, NULL));
    gem_v_string = _t3881;
#line 10 "compiler/main.gem"
    GemVal _t3882[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t3883[] = {gem_dirname_fn(NULL, _t3882, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t3883, 1);
#line 61 "compiler/main.gem"
    GemVal _t3884[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t3884, 1);
#line 62 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 63 "compiler/main.gem"
    GemVal _t3885[] = {gem_string("usage: gem_compiler <file.gem>")};
        (void)(gem_error_at_fn("compiler/main.gem", 63, _t3885, 1));
    }
#line 66 "compiler/main.gem"
    gem_v_src_path = gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(1));
#line 67 "compiler/main.gem"
    GemVal _t3886[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t3886, 1);
#line 69 "compiler/main.gem"
    GemVal _t3887[] = {gem_v_source, gem_v_src_path};
    gem_v_ast = gem_fn_parse_source(NULL, _t3887, 2);
#line 70 "compiler/main.gem"
    GemVal _t3888[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t3888, 1);
#line 71 "compiler/main.gem"
    GemVal _t3889 = gem_table_new();
    gem_v_loaded = _t3889;
#line 72 "compiler/main.gem"
    GemVal _t3890[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t3890, 3);
#line 74 "compiler/main.gem"
    GemVal _t3891[] = {gem_v_src_path};
    gem_v_cg = gem_fn_make_codegen(NULL, _t3891, 1);
#line 75 "compiler/main.gem"
    GemVal _t3892 = gem_v_cg;
    GemVal _t3893[] = {gem_v_resolved_ast};
    GemVal _t3894 = gem_table_get(_t3892, gem_string("compile"));
    gem_v_c_code = _t3894.fn(_t3894.env, _t3893, 1);
#line 76 "compiler/main.gem"
    GemVal _t3895[] = {gem_v_c_code};
    (void)(gem_print(NULL, _t3895, 1));
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

