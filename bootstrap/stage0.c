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
static GemVal gem_fn__mod_ast_make_program(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_int(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_float(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_string(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_bool(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_nil_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_var(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_binop(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_unop(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_call(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_dot(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_index(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_anon_fn(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_table(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_table_entry(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_array(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_interp(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_let(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_assign(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_dot_assign(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_index_assign(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_fn_def(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_if(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_while(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_match(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_when(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_receive_match(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_return(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_break(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_continue(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_load(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_export(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_extern_fn(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_extern_param(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_ast_make_extern_include(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_errors_get_source_line(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_errors_repeat_char(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_errors_compile_error(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_lexer_is_digit(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_lexer_is_alpha(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_lexer_is_alnum(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_lexer_apply_escape(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_lexer_build_keyword_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_lexer_tokenize(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_parser_digit_val(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_parser_str_to_int(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_parser_contains_dot(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_parser_parse_source(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_parser_make_parser(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_escape_c_string(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_repeat_str(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_pad_for(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_format_float(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_set_add(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_set_contains(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_set_union(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_set_intersect(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_sorted_array_to_set(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_is_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_collect_block_lets(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_collect_top_let_names(void *_env, GemVal *args, int argc);
static GemVal gem_fn__mod_codegen_make_codegen(void *_env, GemVal *args, int argc);
static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc);
static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc);
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
GemVal gem_v__mod_parser_make_program;
GemVal gem_v__mod_parser_make_int;
GemVal gem_v__mod_parser_make_float;
GemVal gem_v__mod_parser_make_string;
GemVal gem_v__mod_parser_make_bool;
GemVal gem_v__mod_parser_make_nil_node;
GemVal gem_v__mod_parser_make_var;
GemVal gem_v__mod_parser_make_binop;
GemVal gem_v__mod_parser_make_unop;
GemVal gem_v__mod_parser_make_call;
GemVal gem_v__mod_parser_make_dot;
GemVal gem_v__mod_parser_make_index;
GemVal gem_v__mod_parser_make_anon_fn;
GemVal gem_v__mod_parser_make_table;
GemVal gem_v__mod_parser_make_table_entry;
GemVal gem_v__mod_parser_make_array;
GemVal gem_v__mod_parser_make_interp;
GemVal gem_v__mod_parser_make_let;
GemVal gem_v__mod_parser_make_assign;
GemVal gem_v__mod_parser_make_dot_assign;
GemVal gem_v__mod_parser_make_index_assign;
GemVal gem_v__mod_parser_make_fn_def;
GemVal gem_v__mod_parser_make_if;
GemVal gem_v__mod_parser_make_while;
GemVal gem_v__mod_parser_make_match;
GemVal gem_v__mod_parser_make_when;
GemVal gem_v__mod_parser_make_receive_match;
GemVal gem_v__mod_parser_make_return;
GemVal gem_v__mod_parser_make_break;
GemVal gem_v__mod_parser_make_continue;
GemVal gem_v__mod_parser_make_export;
GemVal gem_v__mod_parser_make_extern_fn;
GemVal gem_v__mod_parser_make_extern_param;
GemVal gem_v__mod_parser_make_extern_include;
GemVal gem_v__mod_lexer_compile_error;
GemVal gem_v__mod_parser_tokenize;
GemVal gem_v__mod_parser_compile_error;
GemVal gem_v_parse_source;
GemVal gem_v_make_codegen;
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

static GemVal gem_fn__mod_ast_make_program(void *_env, GemVal *args, int argc) {
#line 8 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_program", "compiler/main.gem", 8);
    GemVal _t77 = gem_table_new();
    gem_table_set(_t77, gem_string("tag"), gem_string("program"));
    gem_table_set(_t77, gem_string("stmts"), gem_v_stmts);
    GemVal _t78 = _t77;
    gem_pop_frame();
    return _t78;
}

static GemVal gem_fn__mod_ast_make_int(void *_env, GemVal *args, int argc) {
#line 14 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_int", "compiler/main.gem", 14);
    GemVal _t79 = gem_table_new();
    gem_table_set(_t79, gem_string("tag"), gem_string("int"));
    gem_table_set(_t79, gem_string("value"), gem_v_value);
    GemVal _t80 = _t79;
    gem_pop_frame();
    return _t80;
}

static GemVal gem_fn__mod_ast_make_float(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_float", "compiler/main.gem", 18);
    GemVal _t81 = gem_table_new();
    gem_table_set(_t81, gem_string("tag"), gem_string("float"));
    gem_table_set(_t81, gem_string("value"), gem_v_value);
    GemVal _t82 = _t81;
    gem_pop_frame();
    return _t82;
}

static GemVal gem_fn__mod_ast_make_string(void *_env, GemVal *args, int argc) {
#line 22 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_string", "compiler/main.gem", 22);
    GemVal _t83 = gem_table_new();
    gem_table_set(_t83, gem_string("tag"), gem_string("string"));
    gem_table_set(_t83, gem_string("value"), gem_v_value);
    GemVal _t84 = _t83;
    gem_pop_frame();
    return _t84;
}

static GemVal gem_fn__mod_ast_make_bool(void *_env, GemVal *args, int argc) {
#line 26 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_bool", "compiler/main.gem", 26);
    GemVal _t85 = gem_table_new();
    gem_table_set(_t85, gem_string("tag"), gem_string("bool"));
    gem_table_set(_t85, gem_string("value"), gem_v_value);
    GemVal _t86 = _t85;
    gem_pop_frame();
    return _t86;
}

static GemVal gem_fn__mod_ast_make_nil_node(void *_env, GemVal *args, int argc) {
#line 30 "compiler/main.gem"
    gem_push_frame("_mod_ast_make_nil_node", "compiler/main.gem", 30);
    GemVal _t87 = gem_table_new();
    gem_table_set(_t87, gem_string("tag"), gem_string("nil"));
    GemVal _t88 = _t87;
    gem_pop_frame();
    return _t88;
}

static GemVal gem_fn__mod_ast_make_var(void *_env, GemVal *args, int argc) {
#line 34 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_var", "compiler/main.gem", 34);
    GemVal _t89 = gem_table_new();
    gem_table_set(_t89, gem_string("tag"), gem_string("var"));
    gem_table_set(_t89, gem_string("name"), gem_v_name);
    GemVal _t90 = _t89;
    gem_pop_frame();
    return _t90;
}

static GemVal gem_fn__mod_ast_make_binop(void *_env, GemVal *args, int argc) {
#line 38 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_left = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_right = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_binop", "compiler/main.gem", 38);
    GemVal _t91 = gem_table_new();
    gem_table_set(_t91, gem_string("tag"), gem_string("binop"));
    gem_table_set(_t91, gem_string("op"), gem_v_op);
    gem_table_set(_t91, gem_string("left"), gem_v_left);
    gem_table_set(_t91, gem_string("right"), gem_v_right);
    GemVal _t92 = _t91;
    gem_pop_frame();
    return _t92;
}

static GemVal gem_fn__mod_ast_make_unop(void *_env, GemVal *args, int argc) {
#line 42 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_expr = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_unop", "compiler/main.gem", 42);
    GemVal _t93 = gem_table_new();
    gem_table_set(_t93, gem_string("tag"), gem_string("unop"));
    gem_table_set(_t93, gem_string("op"), gem_v_op);
    gem_table_set(_t93, gem_string("expr"), gem_v_expr);
    GemVal _t94 = _t93;
    gem_pop_frame();
    return _t94;
}

static GemVal gem_fn__mod_ast_make_call(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_func = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_args = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_call", "compiler/main.gem", 46);
    GemVal _t95 = gem_table_new();
    gem_table_set(_t95, gem_string("tag"), gem_string("call"));
    gem_table_set(_t95, gem_string("func"), gem_v_func);
    gem_table_set(_t95, gem_string("args"), gem_v_args);
    gem_table_set(_t95, gem_string("line"), gem_v_line);
    GemVal _t96 = _t95;
    gem_pop_frame();
    return _t96;
}

static GemVal gem_fn__mod_ast_make_dot(void *_env, GemVal *args, int argc) {
#line 50 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_dot", "compiler/main.gem", 50);
    GemVal _t97 = gem_table_new();
    gem_table_set(_t97, gem_string("tag"), gem_string("dot"));
    gem_table_set(_t97, gem_string("object"), gem_v_object);
    gem_table_set(_t97, gem_string("field"), gem_v_field);
    GemVal _t98 = _t97;
    gem_pop_frame();
    return _t98;
}

static GemVal gem_fn__mod_ast_make_index(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_index", "compiler/main.gem", 54);
    GemVal _t99 = gem_table_new();
    gem_table_set(_t99, gem_string("tag"), gem_string("index"));
    gem_table_set(_t99, gem_string("object"), gem_v_object);
    gem_table_set(_t99, gem_string("key"), gem_v_key);
    GemVal _t100 = _t99;
    gem_pop_frame();
    return _t100;
}

static GemVal gem_fn__mod_ast_make_anon_fn(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_params = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rest_param = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_block_param = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_body = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_anon_fn", "compiler/main.gem", 58);
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

static GemVal gem_fn__mod_ast_make_table(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_table", "compiler/main.gem", 62);
    GemVal _t103 = gem_table_new();
    gem_table_set(_t103, gem_string("tag"), gem_string("table"));
    gem_table_set(_t103, gem_string("entries"), gem_v_entries);
    GemVal _t104 = _t103;
    gem_pop_frame();
    return _t104;
}

static GemVal gem_fn__mod_ast_make_table_entry(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_table_entry", "compiler/main.gem", 66);
    GemVal _t105 = gem_table_new();
    gem_table_set(_t105, gem_string("key"), gem_v_key);
    gem_table_set(_t105, gem_string("value"), gem_v_value);
    GemVal _t106 = _t105;
    gem_pop_frame();
    return _t106;
}

static GemVal gem_fn__mod_ast_make_array(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_elements = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_array", "compiler/main.gem", 70);
    GemVal _t107 = gem_table_new();
    gem_table_set(_t107, gem_string("tag"), gem_string("array"));
    gem_table_set(_t107, gem_string("elements"), gem_v_elements);
    GemVal _t108 = _t107;
    gem_pop_frame();
    return _t108;
}

static GemVal gem_fn__mod_ast_make_interp(void *_env, GemVal *args, int argc) {
#line 74 "compiler/main.gem"
    GemVal gem_v_parts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_interp", "compiler/main.gem", 74);
    GemVal _t109 = gem_table_new();
    gem_table_set(_t109, gem_string("tag"), gem_string("interp"));
    gem_table_set(_t109, gem_string("parts"), gem_v_parts);
    GemVal _t110 = _t109;
    gem_pop_frame();
    return _t110;
}

static GemVal gem_fn__mod_ast_make_let(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_let", "compiler/main.gem", 80);
    GemVal _t111 = gem_table_new();
    gem_table_set(_t111, gem_string("tag"), gem_string("let"));
    gem_table_set(_t111, gem_string("name"), gem_v_name);
    gem_table_set(_t111, gem_string("value"), gem_v_value);
    gem_table_set(_t111, gem_string("line"), gem_v_line);
    GemVal _t112 = _t111;
    gem_pop_frame();
    return _t112;
}

static GemVal gem_fn__mod_ast_make_assign(void *_env, GemVal *args, int argc) {
#line 84 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_assign", "compiler/main.gem", 84);
    GemVal _t113 = gem_table_new();
    gem_table_set(_t113, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t113, gem_string("name"), gem_v_name);
    gem_table_set(_t113, gem_string("value"), gem_v_value);
    gem_table_set(_t113, gem_string("line"), gem_v_line);
    GemVal _t114 = _t113;
    gem_pop_frame();
    return _t114;
}

static GemVal gem_fn__mod_ast_make_dot_assign(void *_env, GemVal *args, int argc) {
#line 88 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_dot_assign", "compiler/main.gem", 88);
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

static GemVal gem_fn__mod_ast_make_index_assign(void *_env, GemVal *args, int argc) {
#line 92 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_index_assign", "compiler/main.gem", 92);
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

static GemVal gem_fn__mod_ast_make_fn_def(void *_env, GemVal *args, int argc) {
#line 96 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_rest_param = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_block_param = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_body = (argc > 4) ? args[4] : GEM_NIL;
    GemVal gem_v_line = (argc > 5) ? args[5] : GEM_NIL;
    gem_push_frame("_mod_ast_make_fn_def", "compiler/main.gem", 96);
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

static GemVal gem_fn__mod_ast_make_if(void *_env, GemVal *args, int argc) {
#line 100 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_if", "compiler/main.gem", 100);
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

static GemVal gem_fn__mod_ast_make_while(void *_env, GemVal *args, int argc) {
#line 104 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_while", "compiler/main.gem", 104);
    GemVal _t123 = gem_table_new();
    gem_table_set(_t123, gem_string("tag"), gem_string("while"));
    gem_table_set(_t123, gem_string("cond"), gem_v_cond);
    gem_table_set(_t123, gem_string("body"), gem_v_body);
    gem_table_set(_t123, gem_string("line"), gem_v_line);
    GemVal _t124 = _t123;
    gem_pop_frame();
    return _t124;
}

static GemVal gem_fn__mod_ast_make_match(void *_env, GemVal *args, int argc) {
#line 108 "compiler/main.gem"
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_target_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("_mod_ast_make_match", "compiler/main.gem", 108);
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

static GemVal gem_fn__mod_ast_make_when(void *_env, GemVal *args, int argc) {
#line 112 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_bindings = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_when", "compiler/main.gem", 112);
    GemVal _t127 = gem_table_new();
    gem_table_set(_t127, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t127, gem_string("value"), gem_v_value);
    gem_table_set(_t127, gem_string("body"), gem_v_body);
    gem_table_set(_t127, gem_string("bindings"), gem_v_bindings);
    GemVal _t128 = _t127;
    gem_pop_frame();
    return _t128;
}

static GemVal gem_fn__mod_ast_make_receive_match(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_arms = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_after_ms = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_after_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_recv_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("_mod_ast_make_receive_match", "compiler/main.gem", 116);
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

static GemVal gem_fn__mod_ast_make_return(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_return", "compiler/main.gem", 120);
    GemVal _t131 = gem_table_new();
    gem_table_set(_t131, gem_string("tag"), gem_string("return"));
    gem_table_set(_t131, gem_string("value"), gem_v_value);
    gem_table_set(_t131, gem_string("line"), gem_v_line);
    GemVal _t132 = _t131;
    gem_pop_frame();
    return _t132;
}

static GemVal gem_fn__mod_ast_make_break(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    gem_push_frame("_mod_ast_make_break", "compiler/main.gem", 124);
    GemVal _t133 = gem_table_new();
    gem_table_set(_t133, gem_string("tag"), gem_string("break"));
    GemVal _t134 = _t133;
    gem_pop_frame();
    return _t134;
}

static GemVal gem_fn__mod_ast_make_continue(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    gem_push_frame("_mod_ast_make_continue", "compiler/main.gem", 128);
    GemVal _t135 = gem_table_new();
    gem_table_set(_t135, gem_string("tag"), gem_string("continue"));
    GemVal _t136 = _t135;
    gem_pop_frame();
    return _t136;
}

static GemVal gem_fn__mod_ast_make_load(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_load", "compiler/main.gem", 132);
    GemVal _t137 = gem_table_new();
    gem_table_set(_t137, gem_string("tag"), gem_string("load"));
    gem_table_set(_t137, gem_string("path"), gem_v_path);
    GemVal _t138 = _t137;
    gem_pop_frame();
    return _t138;
}

static GemVal gem_fn__mod_ast_make_export(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_names = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_export", "compiler/main.gem", 136);
    GemVal _t139 = gem_table_new();
    gem_table_set(_t139, gem_string("tag"), gem_string("export"));
    gem_table_set(_t139, gem_string("names"), gem_v_names);
    GemVal _t140 = _t139;
    gem_pop_frame();
    return _t140;
}

static GemVal gem_fn__mod_ast_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_is_blocking = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_extern_fn", "compiler/main.gem", 140);
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

static GemVal gem_fn__mod_ast_make_extern_param(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_extern_param", "compiler/main.gem", 144);
    GemVal _t143 = gem_table_new();
    gem_table_set(_t143, gem_string("name"), gem_v_name);
    gem_table_set(_t143, gem_string("type"), gem_v_type_name);
    GemVal _t144 = _t143;
    gem_pop_frame();
    return _t144;
}

static GemVal gem_fn__mod_ast_make_extern_include(void *_env, GemVal *args, int argc) {
#line 148 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_extern_include", "compiler/main.gem", 148);
    GemVal _t145 = gem_table_new();
    gem_table_set(_t145, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t145, gem_string("path"), gem_v_path);
    GemVal _t146 = _t145;
    gem_pop_frame();
    return _t146;
}

static GemVal gem_fn__mod_errors_get_source_line(void *_env, GemVal *args, int argc) {
#line 3 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line_num = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_errors_get_source_line", "compiler/main.gem", 3);
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

static GemVal gem_fn__mod_errors_repeat_char(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_errors_repeat_char", "compiler/main.gem", 23);
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

static GemVal gem_fn__mod_errors_compile_error(void *_env, GemVal *args, int argc) {
#line 31 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_col = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_span_len = (argc > 4) ? args[4] : GEM_NIL;
    GemVal gem_v_msg = (argc > 5) ? args[5] : GEM_NIL;
    GemVal gem_v_hint = (argc > 6) ? args[6] : GEM_NIL;
    gem_push_frame("_mod_errors_compile_error", "compiler/main.gem", 31);
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
    GemVal gem_v_padding = gem_fn__mod_errors_repeat_char(NULL, _t159, 2);
#line 46 "compiler/main.gem"
    GemVal _t160[] = {gem_v_source, gem_v_line};
    GemVal gem_v_src_line = gem_fn__mod_errors_get_source_line(NULL, _t160, 2);
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
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" | ")), gem_fn__mod_errors_repeat_char(NULL, _t163, 2)), gem_fn__mod_errors_repeat_char(NULL, _t164, 2)), gem_string("\n"));
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

static GemVal gem_fn__mod_lexer_is_digit(void *_env, GemVal *args, int argc) {
#line 15 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_lexer_is_digit", "compiler/main.gem", 15);
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

static GemVal gem_fn__mod_lexer_is_alpha(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_lexer_is_alpha", "compiler/main.gem", 19);
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

static GemVal gem_fn__mod_lexer_is_alnum(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_lexer_is_alnum", "compiler/main.gem", 23);
    GemVal _t175[] = {gem_v_ch};
    GemVal _t177;
    if (gem_truthy(gem_fn__mod_lexer_is_alpha(NULL, _t175, 1))) {
        _t177 = gem_fn__mod_lexer_is_alpha(NULL, _t175, 1);
    } else {
        GemVal _t176[] = {gem_v_ch};
        _t177 = gem_fn__mod_lexer_is_digit(NULL, _t176, 1);
    }
    GemVal _t178 = _t177;
    gem_pop_frame();
    return _t178;
}

static GemVal gem_fn__mod_lexer_apply_escape(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_lexer_apply_escape", "compiler/main.gem", 32);
#line 33 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 34 "compiler/main.gem"
        GemVal _t179 = gem_string("\n");
        gem_pop_frame();
        return _t179;
    } else {
#line 35 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 36 "compiler/main.gem"
    GemVal _t180[] = {gem_int(13)};
            GemVal _t181 = gem_chr_fn(NULL, _t180, 1);
            gem_pop_frame();
            return _t181;
        } else {
#line 37 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 38 "compiler/main.gem"
                GemVal _t182 = gem_string("\t");
                gem_pop_frame();
                return _t182;
            } else {
#line 39 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 40 "compiler/main.gem"
                    GemVal _t183 = gem_string("\\");
                    gem_pop_frame();
                    return _t183;
                } else {
#line 41 "compiler/main.gem"
    GemVal _t184;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t184 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t184 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t184)) {
#line 42 "compiler/main.gem"
    GemVal _t185[] = {gem_int(0)};
                        GemVal _t186 = gem_chr_fn(NULL, _t185, 1);
                        gem_pop_frame();
                        return _t186;
                    } else {
#line 43 "compiler/main.gem"
    GemVal _t187;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t187 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t187 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t187)) {
#line 44 "compiler/main.gem"
                            GemVal _t188 = gem_string("\"");
                            gem_pop_frame();
                            return _t188;
                        } else {
#line 45 "compiler/main.gem"
    GemVal _t189;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t189 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t189 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t189)) {
#line 46 "compiler/main.gem"
                                GemVal _t190 = gem_string("{");
                                gem_pop_frame();
                                return _t190;
                            } else {
#line 47 "compiler/main.gem"
    GemVal _t191;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t191 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t191 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t191)) {
#line 48 "compiler/main.gem"
                                    GemVal _t192 = gem_string("}");
                                    gem_pop_frame();
                                    return _t192;
                                } else {
#line 49 "compiler/main.gem"
    GemVal _t193;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t193 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t193 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t193)) {
#line 50 "compiler/main.gem"
                                        GemVal _t194 = gem_string("'");
                                        gem_pop_frame();
                                        return _t194;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t195 = GEM_NIL;
    gem_pop_frame();
    return _t195;
}

static GemVal gem_fn__mod_lexer_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 57 "compiler/main.gem"
    gem_push_frame("_mod_lexer_build_keyword_set", "compiler/main.gem", 57);
#line 58 "compiler/main.gem"
    GemVal _t196 = gem_table_new();
    GemVal gem_v_t = _t196;
#line 59 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("fn"), gem_bool(1));
#line 60 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("end"), gem_bool(1));
#line 61 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("let"), gem_bool(1));
#line 62 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("if"), gem_bool(1));
#line 63 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("elif"), gem_bool(1));
#line 64 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("else"), gem_bool(1));
#line 65 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("while"), gem_bool(1));
#line 66 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("return"), gem_bool(1));
#line 67 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("break"), gem_bool(1));
#line 68 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("continue"), gem_bool(1));
#line 69 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("do"), gem_bool(1));
#line 70 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("match"), gem_bool(1));
#line 71 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("when"), gem_bool(1));
#line 72 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("and"), gem_bool(1));
#line 73 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("or"), gem_bool(1));
#line 74 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("not"), gem_bool(1));
#line 75 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("true"), gem_bool(1));
#line 76 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("false"), gem_bool(1));
#line 77 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("nil"), gem_bool(1));
#line 78 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("load"), gem_bool(1));
#line 79 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("export"), gem_bool(1));
#line 80 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("extern"), gem_bool(1));
#line 81 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("blocking"), gem_bool(1));
#line 82 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("for"), gem_bool(1));
#line 83 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("in"), gem_bool(1));
#line 84 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("after"), gem_bool(1));
#line 85 "compiler/main.gem"
    gem_table_set(gem_v_t, gem_string("then"), gem_bool(1));
    GemVal _t197 = gem_v_t;
    gem_pop_frame();
    return _t197;
}

static GemVal gem_fn__mod_lexer_tokenize(void *_env, GemVal *args, int argc) {
#line 89 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_lexer_tokenize", "compiler/main.gem", 89);
#line 90 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn__mod_lexer_build_keyword_set(NULL, NULL, 0);
#line 91 "compiler/main.gem"
    GemVal _t198 = gem_table_new();
    GemVal gem_v_tokens = _t198;
#line 92 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 93 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 94 "compiler/main.gem"
    GemVal gem_v_line_start = gem_int(0);
#line 95 "compiler/main.gem"
    GemVal _t199[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t199, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 97 "compiler/main.gem"
    GemVal _t200 = gem_table_new();
    GemVal gem_v_interp_brace = _t200;
#line 98 "compiler/main.gem"
    GemVal _t201[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t201, 1);
#line 99 "compiler/main.gem"
    GemVal _t202[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t202, 1);
#line 100 "compiler/main.gem"
    GemVal gem_v_in_triple_dq = gem_bool(0);
#line 101 "compiler/main.gem"
    GemVal gem_v_triple_dq_indent = gem_int(0);
#line 102 "compiler/main.gem"
    GemVal gem_v_tstart = gem_int(0);
#line 104 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 105 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_source, gem_v_pos);
#line 106 "compiler/main.gem"
        gem_v_tstart = gem_v_pos;
#line 109 "compiler/main.gem"
    GemVal _t203;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t203 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t203 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t203)) {
#line 110 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 115 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 116 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t204;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t204 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t204 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t204)) break;
#line 117 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 123 "compiler/main.gem"
    GemVal _t205;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t205 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t205 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t205)) {
#line 124 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 125 "compiler/main.gem"
    GemVal _t206[] = {gem_v_tokens};
    GemVal _t208;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t206, 1), gem_int(0)))) {
        _t208 = gem_gt(gem_len_fn(NULL, _t206, 1), gem_int(0));
    } else {
        GemVal _t207 = gem_table_get(gem_v_tokens, gem_neg(gem_int(1)));
        _t208 = gem_eq(gem_table_get(_t207, gem_string("type")), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t208)) {
#line 126 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 128 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 129 "compiler/main.gem"
    GemVal _t209 = gem_table_new();
    gem_table_set(_t209, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t209, gem_string("value"), gem_string("\n"));
    gem_table_set(_t209, gem_string("line"), gem_v_line);
    gem_table_set(_t209, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t210[] = {gem_v_tokens, _t209};
                (void)(gem_push_fn(NULL, _t210, 2));
            }
#line 131 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 132 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 133 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
            }
#line 135 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 137 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 138 "compiler/main.gem"
                GemVal gem_v_c2 = gem_table_get(gem_v_source, gem_v_pos);
#line 139 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_c2, gem_string("\n")))) {
#line 140 "compiler/main.gem"
                    gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 141 "compiler/main.gem"
                    gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 142 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                } else {
#line 143 "compiler/main.gem"
    GemVal _t211;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t211 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t211 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t212;
    if (gem_truthy(_t211)) {
        _t212 = _t211;
    } else {
        _t212 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t212)) {
#line 144 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 145 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 146 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t213;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t213 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t213 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t213)) break;
#line 147 "compiler/main.gem"
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
#line 157 "compiler/main.gem"
    GemVal _t214[] = {gem_v_ch};
        if (gem_truthy(gem_fn__mod_lexer_is_digit(NULL, _t214, 1))) {
#line 158 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 159 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t216;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t216 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t215[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t216 = gem_fn__mod_lexer_is_digit(NULL, _t215, 1);
                }
                if (!gem_truthy(_t216)) break;
#line 160 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 161 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 163 "compiler/main.gem"
    GemVal _t217;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t217 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t217 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t217;
#line 164 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 165 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 166 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 167 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t219;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t219 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t218[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t219 = gem_fn__mod_lexer_is_digit(NULL, _t218, 1);
                    }
                    if (!gem_truthy(_t219)) break;
#line 168 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 169 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 172 "compiler/main.gem"
    GemVal _t220 = gem_table_new();
    gem_table_set(_t220, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t220, gem_string("value"), gem_v_val);
    gem_table_set(_t220, gem_string("line"), gem_v_line);
    gem_table_set(_t220, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t221[] = {gem_v_tokens, _t220};
            (void)(gem_push_fn(NULL, _t221, 2));
            continue;
        }
#line 177 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 178 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 181 "compiler/main.gem"
    GemVal _t222;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t222 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t222 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t223;
    if (!gem_truthy(_t222)) {
        _t223 = _t222;
    } else {
        _t223 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t223)) {
#line 182 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 184 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t225;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t225 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t224;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t224 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t224 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t225 = _t224;
                    }
                    if (!gem_truthy(_t225)) break;
#line 185 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 187 "compiler/main.gem"
    GemVal _t226;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t226 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t226 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t226)) {
#line 188 "compiler/main.gem"
    GemVal _t227[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
    GemVal _t228 = gem_v__mod_lexer_compile_error;
                    (void)(_t228.fn(_t228.env, _t227, 7));
                }
#line 190 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 191 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 192 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 195 "compiler/main.gem"
                GemVal gem_v_tq_scan = gem_v_pos;
#line 196 "compiler/main.gem"
                GemVal gem_v_tq_bi = gem_int(0);
#line 197 "compiler/main.gem"
                GemVal gem_v_tq_found = gem_bool(0);
#line 198 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t229;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t229 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t229 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t229)) break;
#line 199 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 200 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t230;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t230 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t230 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t230)) break;
#line 201 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 202 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 204 "compiler/main.gem"
    GemVal _t231;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t231 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t231 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t232;
    if (!gem_truthy(_t231)) {
        _t232 = _t231;
    } else {
        _t232 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t233;
    if (!gem_truthy(_t232)) {
        _t233 = _t232;
    } else {
        _t233 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t233)) {
#line 205 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 206 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 208 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t234;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t234 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t234 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t234)) break;
#line 209 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
#line 211 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
#line 212 "compiler/main.gem"
                            gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                        }
                    }
                }
#line 216 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_tq_found))) {
#line 217 "compiler/main.gem"
    GemVal _t235[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
    GemVal _t236 = gem_v__mod_lexer_compile_error;
                    (void)(_t236.fn(_t236.env, _t235, 7));
                }
#line 219 "compiler/main.gem"
                gem_v_triple_dq_indent = gem_v_tq_bi;
#line 222 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 223 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 224 "compiler/main.gem"
                GemVal gem_v_tq_als = gem_bool(1);
#line 226 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 227 "compiler/main.gem"
                    if (gem_truthy(gem_v_tq_als)) {
#line 228 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(0);
#line 229 "compiler/main.gem"
                        GemVal gem_v_tq_skip = gem_int(0);
#line 230 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t237;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t237 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t237 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t238;
                            if (!gem_truthy(_t237)) {
                                                        _t238 = _t237;
                            } else {
                                                        _t238 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t238)) break;
#line 231 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 232 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 235 "compiler/main.gem"
    GemVal _t239;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t239 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t239 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t240;
    if (!gem_truthy(_t239)) {
        _t240 = _t239;
    } else {
        _t240 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t241;
    if (!gem_truthy(_t240)) {
        _t241 = _t240;
    } else {
        _t241 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t241)) {
#line 236 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 238 "compiler/main.gem"
    GemVal _t242[] = {gem_v_val};
    GemVal _t243;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t242, 1), gem_int(0)))) {
        _t243 = gem_gt(gem_len_fn(NULL, _t242, 1), gem_int(0));
    } else {
        _t243 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t243)) {
#line 239 "compiler/main.gem"
    GemVal _t244[] = {gem_v_val};
    GemVal _t245[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t244, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t245, 3);
                            }
                            break;
                        }
                    }
#line 245 "compiler/main.gem"
                    GemVal gem_v_tqc = gem_table_get(gem_v_source, gem_v_pos);
#line 246 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\n")))) {
#line 247 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 248 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 249 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 250 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 251 "compiler/main.gem"
                        gem_v_tq_als = gem_bool(1);
                        continue;
                    }
#line 254 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("\\")))) {
#line 255 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 256 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 257 "compiler/main.gem"
    GemVal _t246[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t247 = gem_v__mod_lexer_compile_error;
                            (void)(_t247.fn(_t247.env, _t246, 7));
                        }
#line 259 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 260 "compiler/main.gem"
    GemVal _t248[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t248, 2);
#line 261 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 262 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 264 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqe);
                        }
#line 266 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 269 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqc, gem_string("{")))) {
#line 270 "compiler/main.gem"
    GemVal _t249 = gem_table_new();
    gem_table_set(_t249, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t249, gem_string("value"), gem_string(""));
    gem_table_set(_t249, gem_string("line"), gem_v_line);
    gem_table_set(_t249, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t250[] = {gem_v_tokens, _t249};
                        (void)(gem_push_fn(NULL, _t250, 2));
#line 271 "compiler/main.gem"
    GemVal _t251 = gem_table_new();
    gem_table_set(_t251, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t251, gem_string("value"), gem_v_val);
    gem_table_set(_t251, gem_string("line"), gem_v_line);
    gem_table_set(_t251, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t252[] = {gem_v_tokens, _t251};
                        (void)(gem_push_fn(NULL, _t252, 2));
#line 272 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 273 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 274 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 275 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 276 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 277 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 280 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqc);
#line 281 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 284 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 285 "compiler/main.gem"
    GemVal _t253 = gem_table_new();
    gem_table_set(_t253, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t253, gem_string("value"), gem_v_val);
    gem_table_set(_t253, gem_string("line"), gem_v_line);
    gem_table_set(_t253, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t254[] = {gem_v_tokens, _t253};
                    (void)(gem_push_fn(NULL, _t254, 2));
                }
                continue;
            }
#line 291 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 292 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 293 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t255;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t255 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t255 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t255)) break;
#line 294 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 295 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 296 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 297 "compiler/main.gem"
    GemVal _t256[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t257 = gem_v__mod_lexer_compile_error;
                        (void)(_t257.fn(_t257.env, _t256, 7));
                    }
#line 299 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 300 "compiler/main.gem"
    GemVal _t258[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t258, 2);
#line 301 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 302 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 304 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 306 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 309 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 310 "compiler/main.gem"
    GemVal _t259 = gem_table_new();
    gem_table_set(_t259, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t259, gem_string("value"), gem_string(""));
    gem_table_set(_t259, gem_string("line"), gem_v_line);
    gem_table_set(_t259, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t260[] = {gem_v_tokens, _t259};
                    (void)(gem_push_fn(NULL, _t260, 2));
#line 311 "compiler/main.gem"
    GemVal _t261 = gem_table_new();
    gem_table_set(_t261, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t261, gem_string("value"), gem_v_val);
    gem_table_set(_t261, gem_string("line"), gem_v_line);
    gem_table_set(_t261, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t262[] = {gem_v_tokens, _t261};
                    (void)(gem_push_fn(NULL, _t262, 2));
#line 312 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 313 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 314 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 315 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 316 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 319 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 320 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 322 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 323 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 324 "compiler/main.gem"
    GemVal _t263[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
    GemVal _t264 = gem_v__mod_lexer_compile_error;
                    (void)(_t264.fn(_t264.env, _t263, 7));
                }
#line 326 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 327 "compiler/main.gem"
    GemVal _t265 = gem_table_new();
    gem_table_set(_t265, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t265, gem_string("value"), gem_v_val);
    gem_table_set(_t265, gem_string("line"), gem_v_line);
    gem_table_set(_t265, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t266[] = {gem_v_tokens, _t265};
                (void)(gem_push_fn(NULL, _t266, 2));
            }
            continue;
        }
#line 333 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 334 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 337 "compiler/main.gem"
    GemVal _t267;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t267 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t267 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t268;
    if (!gem_truthy(_t267)) {
        _t268 = _t267;
    } else {
        _t268 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t268)) {
#line 338 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 340 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t270;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t270 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t269;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t269 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t269 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t270 = _t269;
                    }
                    if (!gem_truthy(_t270)) break;
#line 341 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 343 "compiler/main.gem"
    GemVal _t271;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t271 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t271 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t271)) {
#line 344 "compiler/main.gem"
    GemVal _t272[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
    GemVal _t273 = gem_v__mod_lexer_compile_error;
                    (void)(_t273.fn(_t273.env, _t272, 7));
                }
#line 346 "compiler/main.gem"
                gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 347 "compiler/main.gem"
                gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 348 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 351 "compiler/main.gem"
                GemVal gem_v_sq_scan = gem_v_pos;
#line 352 "compiler/main.gem"
                GemVal gem_v_sq_bi = gem_int(0);
#line 353 "compiler/main.gem"
                GemVal gem_v_sq_found = gem_bool(0);
#line 354 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t274;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t274 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t274 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t274)) break;
#line 355 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 356 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t275;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t275 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t275 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t275)) break;
#line 357 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 358 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 360 "compiler/main.gem"
    GemVal _t276;
    if (!gem_truthy(gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length))) {
        _t276 = gem_lt(gem_add(gem_v_sq_scan, gem_int(2)), gem_v_length);
    } else {
        _t276 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("'"));
    }
    GemVal _t277;
    if (!gem_truthy(_t276)) {
        _t277 = _t276;
    } else {
        _t277 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(1))), gem_string("'"));
    }
    GemVal _t278;
    if (!gem_truthy(_t277)) {
        _t278 = _t277;
    } else {
        _t278 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_sq_scan, gem_int(2))), gem_string("'"));
    }
                    if (gem_truthy(_t278)) {
#line 361 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 362 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 364 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t279;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t279 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t279 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t279)) break;
#line 365 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
#line 367 "compiler/main.gem"
                        if (gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
#line 368 "compiler/main.gem"
                            gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                        }
                    }
                }
#line 372 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_sq_found))) {
#line 373 "compiler/main.gem"
    GemVal _t280[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
    GemVal _t281 = gem_v__mod_lexer_compile_error;
                    (void)(_t281.fn(_t281.env, _t280, 7));
                }
#line 377 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 378 "compiler/main.gem"
                GemVal gem_v_sq_als = gem_bool(1);
#line 380 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 381 "compiler/main.gem"
                    if (gem_truthy(gem_v_sq_als)) {
#line 382 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(0);
#line 383 "compiler/main.gem"
                        GemVal gem_v_sq_skip = gem_int(0);
#line 384 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t282;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t282 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t282 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t283;
                            if (!gem_truthy(_t282)) {
                                                        _t283 = _t282;
                            } else {
                                                        _t283 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t283)) break;
#line 385 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 386 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 389 "compiler/main.gem"
    GemVal _t284;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t284 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t284 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t285;
    if (!gem_truthy(_t284)) {
        _t285 = _t284;
    } else {
        _t285 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t286;
    if (!gem_truthy(_t285)) {
        _t286 = _t285;
    } else {
        _t286 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t286)) {
#line 390 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 392 "compiler/main.gem"
    GemVal _t287[] = {gem_v_val};
    GemVal _t288;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t287, 1), gem_int(0)))) {
        _t288 = gem_gt(gem_len_fn(NULL, _t287, 1), gem_int(0));
    } else {
        _t288 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t288)) {
#line 393 "compiler/main.gem"
    GemVal _t289[] = {gem_v_val};
    GemVal _t290[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t289, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t290, 3);
                            }
                            break;
                        }
                    }
#line 399 "compiler/main.gem"
                    GemVal gem_v_sqc = gem_table_get(gem_v_source, gem_v_pos);
#line 400 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\n")))) {
#line 401 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 402 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 403 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 404 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 405 "compiler/main.gem"
                        gem_v_sq_als = gem_bool(1);
                        continue;
                    }
#line 408 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_sqc, gem_string("\\")))) {
#line 409 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 410 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 411 "compiler/main.gem"
    GemVal _t291[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t292 = gem_v__mod_lexer_compile_error;
                            (void)(_t292.fn(_t292.env, _t291, 7));
                        }
#line 413 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 414 "compiler/main.gem"
    GemVal _t293[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t293, 2);
#line 415 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 416 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 418 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_sqe);
                        }
#line 420 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 423 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_sqc);
#line 424 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 427 "compiler/main.gem"
    GemVal _t294 = gem_table_new();
    gem_table_set(_t294, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t294, gem_string("value"), gem_v_val);
    gem_table_set(_t294, gem_string("line"), gem_v_line);
    gem_table_set(_t294, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t295[] = {gem_v_tokens, _t294};
                (void)(gem_push_fn(NULL, _t295, 2));
                continue;
            }
#line 432 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 433 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t296;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t296 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t296 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t296)) break;
#line 434 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 435 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 436 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 437 "compiler/main.gem"
    GemVal _t297[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t298 = gem_v__mod_lexer_compile_error;
                        (void)(_t298.fn(_t298.env, _t297, 7));
                    }
#line 439 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 440 "compiler/main.gem"
    GemVal _t299[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t299, 2);
#line 441 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 442 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 444 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 446 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 449 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 450 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 452 "compiler/main.gem"
            if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 453 "compiler/main.gem"
    GemVal _t300[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
    GemVal _t301 = gem_v__mod_lexer_compile_error;
                (void)(_t301.fn(_t301.env, _t300, 7));
            }
#line 455 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 456 "compiler/main.gem"
    GemVal _t302 = gem_table_new();
    gem_table_set(_t302, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t302, gem_string("value"), gem_v_val);
    gem_table_set(_t302, gem_string("line"), gem_v_line);
    gem_table_set(_t302, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t303[] = {gem_v_tokens, _t302};
            (void)(gem_push_fn(NULL, _t303, 2));
            continue;
        }
#line 461 "compiler/main.gem"
    GemVal _t304[] = {gem_v_ch};
        if (gem_truthy(gem_fn__mod_lexer_is_alpha(NULL, _t304, 1))) {
#line 462 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 463 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t306;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t306 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t305[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t306 = gem_fn__mod_lexer_is_alnum(NULL, _t305, 1);
                }
                if (!gem_truthy(_t306)) break;
#line 464 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 465 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 467 "compiler/main.gem"
    GemVal _t307[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_in_fn(NULL, _t307, 2))) {
#line 468 "compiler/main.gem"
    GemVal _t308 = gem_table_new();
    gem_table_set(_t308, gem_string("type"), gem_v_val);
    gem_table_set(_t308, gem_string("value"), gem_v_val);
    gem_table_set(_t308, gem_string("line"), gem_v_line);
    gem_table_set(_t308, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t309[] = {gem_v_tokens, _t308};
                (void)(gem_push_fn(NULL, _t309, 2));
            } else {
#line 470 "compiler/main.gem"
    GemVal _t310 = gem_table_new();
    gem_table_set(_t310, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t310, gem_string("value"), gem_v_val);
    gem_table_set(_t310, gem_string("line"), gem_v_line);
    gem_table_set(_t310, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t311[] = {gem_v_tokens, _t310};
                (void)(gem_push_fn(NULL, _t311, 2));
            }
            continue;
        }
#line 476 "compiler/main.gem"
        GemVal gem_v_next = gem_string("");
#line 477 "compiler/main.gem"
        if (gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
#line 478 "compiler/main.gem"
            gem_v_next = gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1)));
        }
#line 481 "compiler/main.gem"
    GemVal _t312;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t312 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t312 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t312)) {
#line 482 "compiler/main.gem"
    GemVal _t313 = gem_table_new();
    gem_table_set(_t313, gem_string("type"), gem_string("=="));
    gem_table_set(_t313, gem_string("value"), gem_string("=="));
    gem_table_set(_t313, gem_string("line"), gem_v_line);
    gem_table_set(_t313, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t314[] = {gem_v_tokens, _t313};
            (void)(gem_push_fn(NULL, _t314, 2));
#line 483 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 486 "compiler/main.gem"
    GemVal _t315;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t315 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t315 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t315)) {
#line 487 "compiler/main.gem"
    GemVal _t316 = gem_table_new();
    gem_table_set(_t316, gem_string("type"), gem_string("!="));
    gem_table_set(_t316, gem_string("value"), gem_string("!="));
    gem_table_set(_t316, gem_string("line"), gem_v_line);
    gem_table_set(_t316, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t317[] = {gem_v_tokens, _t316};
            (void)(gem_push_fn(NULL, _t317, 2));
#line 488 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 491 "compiler/main.gem"
    GemVal _t318;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t318 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t318 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t318)) {
#line 492 "compiler/main.gem"
    GemVal _t319 = gem_table_new();
    gem_table_set(_t319, gem_string("type"), gem_string("<="));
    gem_table_set(_t319, gem_string("value"), gem_string("<="));
    gem_table_set(_t319, gem_string("line"), gem_v_line);
    gem_table_set(_t319, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t320[] = {gem_v_tokens, _t319};
            (void)(gem_push_fn(NULL, _t320, 2));
#line 493 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 496 "compiler/main.gem"
    GemVal _t321;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t321 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t321 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t321)) {
#line 497 "compiler/main.gem"
    GemVal _t322 = gem_table_new();
    gem_table_set(_t322, gem_string("type"), gem_string(">="));
    gem_table_set(_t322, gem_string("value"), gem_string(">="));
    gem_table_set(_t322, gem_string("line"), gem_v_line);
    gem_table_set(_t322, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t323[] = {gem_v_tokens, _t322};
            (void)(gem_push_fn(NULL, _t323, 2));
#line 498 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 501 "compiler/main.gem"
    GemVal _t324;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t324 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t324 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t324)) {
#line 502 "compiler/main.gem"
    GemVal _t325 = gem_table_new();
    gem_table_set(_t325, gem_string("type"), gem_string("+="));
    gem_table_set(_t325, gem_string("value"), gem_string("+="));
    gem_table_set(_t325, gem_string("line"), gem_v_line);
    gem_table_set(_t325, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t326[] = {gem_v_tokens, _t325};
            (void)(gem_push_fn(NULL, _t326, 2));
#line 503 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 506 "compiler/main.gem"
    GemVal _t327;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t327 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t327 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t327)) {
#line 507 "compiler/main.gem"
    GemVal _t328 = gem_table_new();
    gem_table_set(_t328, gem_string("type"), gem_string("-="));
    gem_table_set(_t328, gem_string("value"), gem_string("-="));
    gem_table_set(_t328, gem_string("line"), gem_v_line);
    gem_table_set(_t328, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t329[] = {gem_v_tokens, _t328};
            (void)(gem_push_fn(NULL, _t329, 2));
#line 508 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 511 "compiler/main.gem"
    GemVal _t330;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t330 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t330 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t330)) {
#line 512 "compiler/main.gem"
    GemVal _t331 = gem_table_new();
    gem_table_set(_t331, gem_string("type"), gem_string("*="));
    gem_table_set(_t331, gem_string("value"), gem_string("*="));
    gem_table_set(_t331, gem_string("line"), gem_v_line);
    gem_table_set(_t331, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t332[] = {gem_v_tokens, _t331};
            (void)(gem_push_fn(NULL, _t332, 2));
#line 513 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 516 "compiler/main.gem"
    GemVal _t333;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t333 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t333 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t333)) {
#line 517 "compiler/main.gem"
    GemVal _t334 = gem_table_new();
    gem_table_set(_t334, gem_string("type"), gem_string("/="));
    gem_table_set(_t334, gem_string("value"), gem_string("/="));
    gem_table_set(_t334, gem_string("line"), gem_v_line);
    gem_table_set(_t334, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t335[] = {gem_v_tokens, _t334};
            (void)(gem_push_fn(NULL, _t335, 2));
#line 518 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 521 "compiler/main.gem"
    GemVal _t336;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t336 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t336 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t336)) {
#line 522 "compiler/main.gem"
    GemVal _t337 = gem_table_new();
    gem_table_set(_t337, gem_string("type"), gem_string("->"));
    gem_table_set(_t337, gem_string("value"), gem_string("->"));
    gem_table_set(_t337, gem_string("line"), gem_v_line);
    gem_table_set(_t337, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t338[] = {gem_v_tokens, _t337};
            (void)(gem_push_fn(NULL, _t338, 2));
#line 523 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 528 "compiler/main.gem"
    GemVal _t339;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t339 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t339 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t340;
    if (gem_truthy(_t339)) {
        _t340 = _t339;
    } else {
        _t340 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t341;
    if (gem_truthy(_t340)) {
        _t341 = _t340;
    } else {
        _t341 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t342;
    if (gem_truthy(_t341)) {
        _t342 = _t341;
    } else {
        _t342 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t342)) {
#line 529 "compiler/main.gem"
    GemVal _t343 = gem_table_new();
    gem_table_set(_t343, gem_string("type"), gem_v_ch);
    gem_table_set(_t343, gem_string("value"), gem_v_ch);
    gem_table_set(_t343, gem_string("line"), gem_v_line);
    gem_table_set(_t343, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t344[] = {gem_v_tokens, _t343};
            (void)(gem_push_fn(NULL, _t344, 2));
#line 530 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 533 "compiler/main.gem"
    GemVal _t345;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t345 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t345 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t346;
    if (gem_truthy(_t345)) {
        _t346 = _t345;
    } else {
        _t346 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t346)) {
#line 534 "compiler/main.gem"
    GemVal _t347 = gem_table_new();
    gem_table_set(_t347, gem_string("type"), gem_v_ch);
    gem_table_set(_t347, gem_string("value"), gem_v_ch);
    gem_table_set(_t347, gem_string("line"), gem_v_line);
    gem_table_set(_t347, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t348[] = {gem_v_tokens, _t347};
            (void)(gem_push_fn(NULL, _t348, 2));
#line 535 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 539 "compiler/main.gem"
    GemVal _t349;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t349 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t349 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t349)) {
#line 540 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 541 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 542 "compiler/main.gem"
    GemVal _t350 = gem_table_new();
    gem_table_set(_t350, gem_string("type"), gem_string("{"));
    gem_table_set(_t350, gem_string("value"), gem_string("{"));
    gem_table_set(_t350, gem_string("line"), gem_v_line);
    gem_table_set(_t350, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t351[] = {gem_v_tokens, _t350};
            (void)(gem_push_fn(NULL, _t351, 2));
#line 543 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 546 "compiler/main.gem"
    GemVal _t352;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t352 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t352 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t352)) {
#line 547 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 548 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 549 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 550 "compiler/main.gem"
    GemVal _t353 = gem_table_new();
    gem_table_set(_t353, gem_string("type"), gem_string("}"));
    gem_table_set(_t353, gem_string("value"), gem_string("}"));
    gem_table_set(_t353, gem_string("line"), gem_v_line);
    gem_table_set(_t353, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t354[] = {gem_v_tokens, _t353};
                (void)(gem_push_fn(NULL, _t354, 2));
#line 551 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                continue;
            }
#line 555 "compiler/main.gem"
            gem_v_interp_depth = gem_sub(gem_v_interp_depth, gem_int(1));
#line 556 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 559 "compiler/main.gem"
            if (gem_truthy(gem_v_in_triple_dq)) {
#line 560 "compiler/main.gem"
                gem_v_in_triple_dq = gem_bool(0);
#line 561 "compiler/main.gem"
                GemVal gem_v_val = gem_string("");
#line 562 "compiler/main.gem"
                GemVal gem_v_hit_interp = gem_bool(0);
#line 563 "compiler/main.gem"
                GemVal gem_v_tqr_als = gem_bool(0);
#line 565 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) break;
#line 566 "compiler/main.gem"
                    if (gem_truthy(gem_v_tqr_als)) {
#line 567 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(0);
#line 568 "compiler/main.gem"
                        GemVal gem_v_tqr_skip = gem_int(0);
#line 569 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t355;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t355 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t355 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t356;
                            if (!gem_truthy(_t355)) {
                                                        _t356 = _t355;
                            } else {
                                                        _t356 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t356)) break;
#line 570 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 571 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 574 "compiler/main.gem"
    GemVal _t357;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t357 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t357 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t358;
    if (!gem_truthy(_t357)) {
        _t358 = _t357;
    } else {
        _t358 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t359;
    if (!gem_truthy(_t358)) {
        _t359 = _t358;
    } else {
        _t359 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t359)) {
#line 575 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 576 "compiler/main.gem"
    GemVal _t360[] = {gem_v_val};
    GemVal _t361;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t360, 1), gem_int(0)))) {
        _t361 = gem_gt(gem_len_fn(NULL, _t360, 1), gem_int(0));
    } else {
        _t361 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t361)) {
#line 577 "compiler/main.gem"
    GemVal _t362[] = {gem_v_val};
    GemVal _t363[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t362, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t363, 3);
                            }
                            break;
                        }
                    }
#line 583 "compiler/main.gem"
                    GemVal gem_v_tqrc = gem_table_get(gem_v_source, gem_v_pos);
#line 584 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\n")))) {
#line 585 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_string("\n"));
#line 586 "compiler/main.gem"
                        gem_v_line = gem_add(gem_v_line, gem_int(1));
#line 587 "compiler/main.gem"
                        gem_v_line_start = gem_add(gem_v_pos, gem_int(1));
#line 588 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 589 "compiler/main.gem"
                        gem_v_tqr_als = gem_bool(1);
                        continue;
                    }
#line 592 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("\\")))) {
#line 593 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 594 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 595 "compiler/main.gem"
    GemVal _t364[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t365 = gem_v__mod_lexer_compile_error;
                            (void)(_t365.fn(_t365.env, _t364, 7));
                        }
#line 597 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 598 "compiler/main.gem"
    GemVal _t366[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t366, 2);
#line 599 "compiler/main.gem"
                        if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 600 "compiler/main.gem"
                            gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                        } else {
#line 602 "compiler/main.gem"
                            gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_tqre);
                        }
#line 604 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                        continue;
                    }
#line 607 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_tqrc, gem_string("{")))) {
#line 608 "compiler/main.gem"
    GemVal _t367 = gem_table_new();
    gem_table_set(_t367, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t367, gem_string("value"), gem_v_val);
    gem_table_set(_t367, gem_string("line"), gem_v_line);
    gem_table_set(_t367, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t368[] = {gem_v_tokens, _t367};
                        (void)(gem_push_fn(NULL, _t368, 2));
#line 609 "compiler/main.gem"
                        gem_v_val = gem_string("");
#line 610 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 611 "compiler/main.gem"
                        gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 612 "compiler/main.gem"
                        gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 613 "compiler/main.gem"
                        gem_v_in_triple_dq = gem_bool(1);
#line 614 "compiler/main.gem"
                        gem_v_hit_interp = gem_bool(1);
                        break;
                    }
#line 617 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_v_tqrc);
#line 618 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 621 "compiler/main.gem"
                if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 622 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 623 "compiler/main.gem"
    GemVal _t369[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string interpolation"), GEM_NIL};
    GemVal _t370 = gem_v__mod_lexer_compile_error;
                        (void)(_t370.fn(_t370.env, _t369, 7));
                    }
#line 625 "compiler/main.gem"
    GemVal _t371 = gem_table_new();
    gem_table_set(_t371, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t371, gem_string("value"), gem_v_val);
    gem_table_set(_t371, gem_string("line"), gem_v_line);
    gem_table_set(_t371, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t372[] = {gem_v_tokens, _t371};
                    (void)(gem_push_fn(NULL, _t372, 2));
#line 626 "compiler/main.gem"
    GemVal _t373 = gem_table_new();
    gem_table_set(_t373, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t373, gem_string("value"), gem_string(""));
    gem_table_set(_t373, gem_string("line"), gem_v_line);
    gem_table_set(_t373, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t374[] = {gem_v_tokens, _t373};
                    (void)(gem_push_fn(NULL, _t374, 2));
                }
                continue;
            }
#line 632 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 633 "compiler/main.gem"
            GemVal gem_v_hit_interp = gem_bool(0);
#line 634 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t375;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t375 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t375 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t375)) break;
#line 635 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 636 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 637 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 638 "compiler/main.gem"
    GemVal _t376[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t377 = gem_v__mod_lexer_compile_error;
                        (void)(_t377.fn(_t377.env, _t376, 7));
                    }
#line 640 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 641 "compiler/main.gem"
    GemVal _t378[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t378, 2);
#line 642 "compiler/main.gem"
                    if (gem_truthy(gem_neq(gem_v_esc_result, GEM_NIL))) {
#line 643 "compiler/main.gem"
                        gem_v_val = gem_add(gem_v_val, gem_v_esc_result);
                    } else {
#line 645 "compiler/main.gem"
                        gem_v_val = gem_add(gem_add(gem_v_val, gem_string("\\")), gem_v_esc);
                    }
#line 647 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    continue;
                }
#line 650 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("{")))) {
#line 652 "compiler/main.gem"
    GemVal _t379 = gem_table_new();
    gem_table_set(_t379, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t379, gem_string("value"), gem_v_val);
    gem_table_set(_t379, gem_string("line"), gem_v_line);
    gem_table_set(_t379, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t380[] = {gem_v_tokens, _t379};
                    (void)(gem_push_fn(NULL, _t380, 2));
#line 653 "compiler/main.gem"
                    gem_v_val = gem_string("");
#line 654 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 655 "compiler/main.gem"
                    gem_table_set(gem_v_interp_brace, gem_v_interp_depth, gem_int(0));
#line 656 "compiler/main.gem"
                    gem_v_interp_depth = gem_add(gem_v_interp_depth, gem_int(1));
#line 657 "compiler/main.gem"
                    gem_v_hit_interp = gem_bool(1);
                    break;
                }
#line 660 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 661 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 663 "compiler/main.gem"
            if (gem_truthy(gem_not(gem_v_hit_interp))) {
#line 664 "compiler/main.gem"
                if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 665 "compiler/main.gem"
    GemVal _t381[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string interpolation"), GEM_NIL};
    GemVal _t382 = gem_v__mod_lexer_compile_error;
                    (void)(_t382.fn(_t382.env, _t381, 7));
                }
#line 667 "compiler/main.gem"
    GemVal _t383 = gem_table_new();
    gem_table_set(_t383, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t383, gem_string("value"), gem_v_val);
    gem_table_set(_t383, gem_string("line"), gem_v_line);
    gem_table_set(_t383, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t384[] = {gem_v_tokens, _t383};
                (void)(gem_push_fn(NULL, _t384, 2));
#line 668 "compiler/main.gem"
    GemVal _t385 = gem_table_new();
    gem_table_set(_t385, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t385, gem_string("value"), gem_string(""));
    gem_table_set(_t385, gem_string("line"), gem_v_line);
    gem_table_set(_t385, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t386[] = {gem_v_tokens, _t385};
                (void)(gem_push_fn(NULL, _t386, 2));
#line 669 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 674 "compiler/main.gem"
    GemVal _t387;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t387 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t387 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t388;
    if (gem_truthy(_t387)) {
        _t388 = _t387;
    } else {
        _t388 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t389;
    if (gem_truthy(_t388)) {
        _t389 = _t388;
    } else {
        _t389 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t390;
    if (gem_truthy(_t389)) {
        _t390 = _t389;
    } else {
        _t390 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t391;
    if (gem_truthy(_t390)) {
        _t391 = _t390;
    } else {
        _t391 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t391)) {
#line 675 "compiler/main.gem"
    GemVal _t392 = gem_table_new();
    gem_table_set(_t392, gem_string("type"), gem_v_ch);
    gem_table_set(_t392, gem_string("value"), gem_v_ch);
    gem_table_set(_t392, gem_string("line"), gem_v_line);
    gem_table_set(_t392, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t393[] = {gem_v_tokens, _t392};
            (void)(gem_push_fn(NULL, _t393, 2));
#line 676 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 679 "compiler/main.gem"
    GemVal _t394;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t394 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t394 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t395;
    if (!gem_truthy(_t394)) {
        _t395 = _t394;
    } else {
        _t395 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t396;
    if (!gem_truthy(_t395)) {
        _t396 = _t395;
    } else {
        _t396 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t397;
    if (!gem_truthy(_t396)) {
        _t397 = _t396;
    } else {
        _t397 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t397)) {
#line 680 "compiler/main.gem"
    GemVal _t398 = gem_table_new();
    gem_table_set(_t398, gem_string("type"), gem_string("..."));
    gem_table_set(_t398, gem_string("value"), gem_string("..."));
    gem_table_set(_t398, gem_string("line"), gem_v_line);
    gem_table_set(_t398, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t399[] = {gem_v_tokens, _t398};
            (void)(gem_push_fn(NULL, _t399, 2));
#line 681 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 684 "compiler/main.gem"
    GemVal _t400;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t400 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t400 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t401;
    if (gem_truthy(_t400)) {
        _t401 = _t400;
    } else {
        _t401 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t402;
    if (gem_truthy(_t401)) {
        _t402 = _t401;
    } else {
        _t402 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t402)) {
#line 685 "compiler/main.gem"
    GemVal _t403 = gem_table_new();
    gem_table_set(_t403, gem_string("type"), gem_v_ch);
    gem_table_set(_t403, gem_string("value"), gem_v_ch);
    gem_table_set(_t403, gem_string("line"), gem_v_line);
    gem_table_set(_t403, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t404[] = {gem_v_tokens, _t403};
            (void)(gem_push_fn(NULL, _t404, 2));
#line 686 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 690 "compiler/main.gem"
    GemVal _t405[] = {gem_v_ch};
    GemVal _t406[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_add(gem_add(gem_string("unexpected character: '"), gem_to_string_fn(NULL, _t405, 1)), gem_string("'")), GEM_NIL};
    GemVal _t407 = gem_v__mod_lexer_compile_error;
        (void)(_t407.fn(_t407.env, _t406, 7));
    }
#line 693 "compiler/main.gem"
    GemVal _t408 = gem_table_new();
    gem_table_set(_t408, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t408, gem_string("value"), gem_string(""));
    gem_table_set(_t408, gem_string("line"), gem_v_line);
    gem_table_set(_t408, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t409[] = {gem_v_tokens, _t408};
    (void)(gem_push_fn(NULL, _t409, 2));
    GemVal _t410 = gem_v_tokens;
    gem_pop_frame();
    return _t410;
}

static GemVal gem_fn__mod_parser_digit_val(void *_env, GemVal *args, int argc) {
#line 12 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_parser_digit_val", "compiler/main.gem", 12);
#line 13 "compiler/main.gem"
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t411 = gem_int(0);
        gem_pop_frame();
        return _t411;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t412 = gem_int(1);
        gem_pop_frame();
        return _t412;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t413 = gem_int(2);
        gem_pop_frame();
        return _t413;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t414 = gem_int(3);
        gem_pop_frame();
        return _t414;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t415 = gem_int(4);
        gem_pop_frame();
        return _t415;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t416 = gem_int(5);
        gem_pop_frame();
        return _t416;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t417 = gem_int(6);
        gem_pop_frame();
        return _t417;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t418 = gem_int(7);
        gem_pop_frame();
        return _t418;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t419 = gem_int(8);
        gem_pop_frame();
        return _t419;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t420 = gem_int(9);
        gem_pop_frame();
        return _t420;
    } else {
#line 35 "compiler/main.gem"
    GemVal _t421[] = {gem_v_ch};
    GemVal _t422[] = {gem_add(gem_string("not a digit: "), gem_to_string_fn(NULL, _t421, 1))};
        GemVal _t423 = gem_error_at_fn("compiler/main.gem", 35, _t422, 1);
        gem_pop_frame();
        return _t423;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
}

static GemVal gem_fn__mod_parser_str_to_int(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_parser_str_to_int", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal gem_v_result = gem_int(0);
#line 41 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 41 "compiler/main.gem"
    GemVal _t424[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t424, 1);
#line 41 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 41 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 41 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 42 "compiler/main.gem"
    GemVal _t425[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn__mod_parser_digit_val(NULL, _t425, 1));
    }

    GemVal _t426 = gem_v_result;
    gem_pop_frame();
    return _t426;
}

static GemVal gem_fn__mod_parser_contains_dot(void *_env, GemVal *args, int argc) {
#line 47 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_parser_contains_dot", "compiler/main.gem", 47);
#line 48 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 48 "compiler/main.gem"
    GemVal _t427[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t427, 1);
#line 48 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 48 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_3;
#line 48 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 49 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 50 "compiler/main.gem"
            GemVal _t428 = gem_bool(1);
            gem_pop_frame();
            return _t428;
        }
    }

    GemVal _t429 = gem_bool(0);
    gem_pop_frame();
    return _t429;
}

static GemVal gem_fn__mod_parser_parse_source(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_parser_parse_source", "compiler/main.gem", 58);
#line 59 "compiler/main.gem"
    GemVal _t430[] = {gem_v_source, gem_v_file};
    GemVal _t431 = gem_v__mod_parser_tokenize;
    GemVal gem_v_tokens = _t431.fn(_t431.env, _t430, 2);
#line 60 "compiler/main.gem"
    GemVal _t432[] = {gem_v_tokens, gem_v_source, gem_v_file};
    GemVal gem_v_p = gem_fn__mod_parser_make_parser(NULL, _t432, 3);
#line 61 "compiler/main.gem"
    GemVal _t433 = gem_v_p;
    GemVal _t434 = gem_table_get(_t433, gem_string("parse"));
    GemVal _t435 = _t434.fn(_t434.env, NULL, 0);
    gem_pop_frame();
    return _t435;
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
    GemVal _t436 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t436;
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
    GemVal _t438 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t438;
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
#line 82 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 83 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t440 = gem_v_t;
    gem_pop_frame();
    return _t440;
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
    GemVal _t442 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    GemVal _t443 = gem_eq(gem_table_get(_t442, gem_string("type")), gem_string("EOF"));
    gem_pop_frame();
    return _t443;
}

struct _closure__anon_5 {
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v_file;
    GemVal *gem_v_pos;
    GemVal *gem_v_source;
    GemVal *gem_v_tokens;
};
static GemVal _anon_5(void *_env, GemVal *args, int argc) {
    struct _closure__anon_5 *_cls = (struct _closure__anon_5 *)_env;
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_source = _cls->gem_v_source;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal gem_v_tp = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_5", "compiler/main.gem", 0);
#line 92 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 93 "compiler/main.gem"
    GemVal _t445 = gem_v_t;
    if (gem_truthy(gem_neq(gem_table_get(_t445, gem_string("type")), gem_v_tp))) {
#line 94 "compiler/main.gem"
    GemVal _t446 = gem_v_t;
    GemVal _t447 = gem_v_t;
    GemVal _t448 = gem_v_t;
    GemVal _t449[] = {gem_table_get(_t448, gem_string("value"))};
    GemVal _t450[] = {gem_v_tp};
    GemVal _t451 = gem_v_t;
    GemVal _t452[] = {gem_table_get(_t451, gem_string("type"))};
    GemVal _t453[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t446, gem_string("line")), gem_table_get(_t447, gem_string("col")), gem_len_fn(NULL, _t449, 1), gem_add(gem_add(gem_add(gem_add(gem_string("expected '"), gem_to_string_fn(NULL, _t450, 1)), gem_string("' but got '")), gem_to_string_fn(NULL, _t452, 1)), gem_string("'")), GEM_NIL};
    GemVal _t454 = (*gem_v__mod_parser_compile_error);
        (void)(_t454.fn(_t454.env, _t453, 7));
    }
#line 96 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t455 = gem_v_t;
    gem_pop_frame();
    return _t455;
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
#line 101 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t457 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t457, gem_string("type")), gem_string("NEWLINE")))) break;
#line 102 "compiler/main.gem"
        *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    }
    gem_pop_frame();
    return GEM_NIL;
}

struct _closure__anon_7 {
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_file;
    GemVal *gem_v_peek;
    GemVal *gem_v_source;
};
static GemVal _anon_7(void *_env, GemVal *args, int argc) {
    struct _closure__anon_7 *_cls = (struct _closure__anon_7 *)_env;
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_7", "compiler/main.gem", 0);
#line 113 "compiler/main.gem"
    GemVal _t459 = gem_table_new();
    GemVal gem_v_params = _t459;
#line 114 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 115 "compiler/main.gem"
    GemVal gem_v_block_param = GEM_NIL;
#line 117 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t460 = (*gem_v_peek);
        GemVal _t461 = _t460.fn(_t460.env, NULL, 0);
        GemVal _t464;
        if (gem_truthy(gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("NAME")))) {
                _t464 = gem_eq(gem_table_get(_t461, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t462 = (*gem_v_peek);
                GemVal _t463 = _t462.fn(_t462.env, NULL, 0);
                _t464 = gem_eq(gem_table_get(_t463, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t464)) break;
#line 118 "compiler/main.gem"
    GemVal _t465 = (*gem_v_peek);
    GemVal _t466 = _t465.fn(_t465.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t466, gem_string("type")), gem_string("...")))) {
#line 119 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
            (void)(_t467.fn(_t467.env, NULL, 0));
#line 120 "compiler/main.gem"
    GemVal _t468[] = {gem_string("NAME")};
    GemVal _t469 = (*gem_v_expect);
    GemVal _t470 = _t469.fn(_t469.env, _t468, 1);
            gem_v_rest_param = gem_table_get(_t470, gem_string("value"));
#line 121 "compiler/main.gem"
    GemVal _t471 = (*gem_v_peek);
    GemVal _t472 = _t471.fn(_t471.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t472, gem_string("type")), gem_string(",")))) {
#line 122 "compiler/main.gem"
    GemVal _t473 = (*gem_v_advance);
                (void)(_t473.fn(_t473.env, NULL, 0));
#line 123 "compiler/main.gem"
    GemVal _t474 = (*gem_v_peek);
    GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
                if (gem_truthy(gem_neq(gem_table_get(_t475, gem_string("type")), gem_string("NAME")))) {
#line 124 "compiler/main.gem"
    GemVal _t476 = (*gem_v_peek);
    GemVal _t477 = _t476.fn(_t476.env, NULL, 0);
    GemVal _t478 = (*gem_v_peek);
    GemVal _t479 = _t478.fn(_t478.env, NULL, 0);
    GemVal _t480 = (*gem_v_peek);
    GemVal _t481 = _t480.fn(_t480.env, NULL, 0);
    GemVal _t482[] = {gem_table_get(_t481, gem_string("value"))};
    GemVal _t483[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t477, gem_string("line")), gem_table_get(_t479, gem_string("col")), gem_len_fn(NULL, _t482, 1), gem_string("expected parameter name after rest param"), GEM_NIL};
    GemVal _t484 = (*gem_v__mod_parser_compile_error);
                    (void)(_t484.fn(_t484.env, _t483, 7));
                }
#line 126 "compiler/main.gem"
    GemVal _t485 = (*gem_v_advance);
    GemVal _t486 = _t485.fn(_t485.env, NULL, 0);
                gem_v_block_param = gem_table_get(_t486, gem_string("value"));
#line 127 "compiler/main.gem"
    GemVal _t487 = (*gem_v_peek);
    GemVal _t488 = _t487.fn(_t487.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t488, gem_string("type")), gem_string(",")))) {
#line 128 "compiler/main.gem"
    GemVal _t489 = (*gem_v_peek);
    GemVal _t490 = _t489.fn(_t489.env, NULL, 0);
    GemVal _t491 = (*gem_v_peek);
    GemVal _t492 = _t491.fn(_t491.env, NULL, 0);
    GemVal _t493 = (*gem_v_peek);
    GemVal _t494 = _t493.fn(_t493.env, NULL, 0);
    GemVal _t495[] = {gem_table_get(_t494, gem_string("value"))};
    GemVal _t496[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t490, gem_string("line")), gem_table_get(_t492, gem_string("col")), gem_len_fn(NULL, _t495, 1), gem_string("only one parameter is allowed after a rest param"), GEM_NIL};
    GemVal _t497 = (*gem_v__mod_parser_compile_error);
                    (void)(_t497.fn(_t497.env, _t496, 7));
                }
            }
            break;
        }
#line 133 "compiler/main.gem"
    GemVal _t498 = (*gem_v_advance);
    GemVal _t499 = _t498.fn(_t498.env, NULL, 0);
    GemVal _t500[] = {gem_v_params, gem_table_get(_t499, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t500, 2));
#line 134 "compiler/main.gem"
    GemVal _t501 = (*gem_v_peek);
    GemVal _t502 = _t501.fn(_t501.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t502, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 137 "compiler/main.gem"
    GemVal _t503 = (*gem_v_advance);
        (void)(_t503.fn(_t503.env, NULL, 0));
    }
    GemVal _t504 = gem_table_new();
    gem_table_set(_t504, gem_string("params"), gem_v_params);
    gem_table_set(_t504, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t504, gem_string("block_param"), gem_v_block_param);
    GemVal _t505 = _t504;
    gem_pop_frame();
    return _t505;
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
#line 156 "compiler/main.gem"
    GemVal _t507 = gem_table_new();
    GemVal gem_v_stmts = _t507;
#line 157 "compiler/main.gem"
    GemVal _t508 = (*gem_v_skip_nl);
    (void)(_t508.fn(_t508.env, NULL, 0));
#line 158 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t509 = (*gem_v_peek);
        GemVal _t510 = _t509.fn(_t509.env, NULL, 0);
        GemVal _t513;
        if (!gem_truthy(gem_neq(gem_table_get(_t510, gem_string("type")), gem_string("end")))) {
                _t513 = gem_neq(gem_table_get(_t510, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t511 = (*gem_v_peek);
                GemVal _t512 = _t511.fn(_t511.env, NULL, 0);
                _t513 = gem_neq(gem_table_get(_t512, gem_string("type")), gem_string("elif"));
        }
        GemVal _t516;
        if (!gem_truthy(_t513)) {
                _t516 = _t513;
        } else {
                GemVal _t514 = (*gem_v_peek);
                GemVal _t515 = _t514.fn(_t514.env, NULL, 0);
                _t516 = gem_neq(gem_table_get(_t515, gem_string("type")), gem_string("else"));
        }
        GemVal _t519;
        if (!gem_truthy(_t516)) {
                _t519 = _t516;
        } else {
                GemVal _t517 = (*gem_v_peek);
                GemVal _t518 = _t517.fn(_t517.env, NULL, 0);
                _t519 = gem_neq(gem_table_get(_t518, gem_string("type")), gem_string("when"));
        }
        GemVal _t522;
        if (!gem_truthy(_t519)) {
                _t522 = _t519;
        } else {
                GemVal _t520 = (*gem_v_peek);
                GemVal _t521 = _t520.fn(_t520.env, NULL, 0);
                _t522 = gem_neq(gem_table_get(_t521, gem_string("type")), gem_string("after"));
        }
        GemVal _t525;
        if (!gem_truthy(_t522)) {
                _t525 = _t522;
        } else {
                GemVal _t523 = (*gem_v_peek);
                GemVal _t524 = _t523.fn(_t523.env, NULL, 0);
                _t525 = gem_neq(gem_table_get(_t524, gem_string("type")), gem_string("then"));
        }
        GemVal _t527;
        if (!gem_truthy(_t525)) {
                _t527 = _t525;
        } else {
                GemVal _t526 = (*gem_v_at_end);
                _t527 = gem_not(_t526.fn(_t526.env, NULL, 0));
        }
        if (!gem_truthy(_t527)) break;
#line 159 "compiler/main.gem"
    GemVal _t528 = (*gem_v_parse_stmt);
    GemVal _t529[] = {gem_v_stmts, _t528.fn(_t528.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t529, 2));
#line 160 "compiler/main.gem"
    GemVal _t530 = (*gem_v_skip_nl);
        (void)(_t530.fn(_t530.env, NULL, 0));
    }
    GemVal _t531 = gem_v_stmts;
    gem_pop_frame();
    return _t531;
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
#line 169 "compiler/main.gem"
    GemVal _t533 = gem_table_new();
    GemVal gem_v_stmts = _t533;
#line 170 "compiler/main.gem"
    GemVal _t534 = (*gem_v_skip_nl);
    (void)(_t534.fn(_t534.env, NULL, 0));
#line 171 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t535 = (*gem_v_peek);
        GemVal _t536 = _t535.fn(_t535.env, NULL, 0);
        GemVal _t538;
        if (!gem_truthy(gem_neq(gem_table_get(_t536, gem_string("type")), gem_string("end")))) {
                _t538 = gem_neq(gem_table_get(_t536, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t537 = (*gem_v_at_end);
                _t538 = gem_not(_t537.fn(_t537.env, NULL, 0));
        }
        if (!gem_truthy(_t538)) break;
#line 172 "compiler/main.gem"
    GemVal _t539 = (*gem_v_parse_stmt);
    GemVal _t540[] = {gem_v_stmts, _t539.fn(_t539.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t540, 2));
#line 173 "compiler/main.gem"
    GemVal _t541 = (*gem_v_skip_nl);
        (void)(_t541.fn(_t541.env, NULL, 0));
    }
    GemVal _t542 = gem_v_stmts;
    gem_pop_frame();
    return _t542;
}

struct _closure__anon_10 {
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_anon_fn;
    GemVal *gem_v__mod_parser_make_array;
    GemVal *gem_v__mod_parser_make_bool;
    GemVal *gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_float;
    GemVal *gem_v__mod_parser_make_int;
    GemVal *gem_v__mod_parser_make_interp;
    GemVal *gem_v__mod_parser_make_nil_node;
    GemVal *gem_v__mod_parser_make_string;
    GemVal *gem_v__mod_parser_make_table;
    GemVal *gem_v__mod_parser_make_table_entry;
    GemVal *gem_v__mod_parser_make_var;
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
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_anon_fn = _cls->gem_v__mod_parser_make_anon_fn;
    GemVal *gem_v__mod_parser_make_array = _cls->gem_v__mod_parser_make_array;
    GemVal *gem_v__mod_parser_make_bool = _cls->gem_v__mod_parser_make_bool;
    GemVal *gem_v__mod_parser_make_call = _cls->gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_float = _cls->gem_v__mod_parser_make_float;
    GemVal *gem_v__mod_parser_make_int = _cls->gem_v__mod_parser_make_int;
    GemVal *gem_v__mod_parser_make_interp = _cls->gem_v__mod_parser_make_interp;
    GemVal *gem_v__mod_parser_make_nil_node = _cls->gem_v__mod_parser_make_nil_node;
    GemVal *gem_v__mod_parser_make_string = _cls->gem_v__mod_parser_make_string;
    GemVal *gem_v__mod_parser_make_table = _cls->gem_v__mod_parser_make_table;
    GemVal *gem_v__mod_parser_make_table_entry = _cls->gem_v__mod_parser_make_table_entry;
    GemVal *gem_v__mod_parser_make_var = _cls->gem_v__mod_parser_make_var;
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
#line 182 "compiler/main.gem"
    GemVal _t544 = (*gem_v_peek);
    GemVal gem_v_t = _t544.fn(_t544.env, NULL, 0);
#line 185 "compiler/main.gem"
    GemVal _t545 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t545, gem_string("type")), gem_string("NUMBER")))) {
#line 186 "compiler/main.gem"
    GemVal _t546 = (*gem_v_advance);
        (void)(_t546.fn(_t546.env, NULL, 0));
#line 187 "compiler/main.gem"
    GemVal _t547 = gem_v_t;
    GemVal _t548[] = {gem_table_get(_t547, gem_string("value"))};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t548, 1))) {
#line 188 "compiler/main.gem"
    GemVal _t549 = gem_v_t;
    GemVal _t550[] = {gem_table_get(_t549, gem_string("value"))};
    GemVal _t551[] = {gem_fn_atof(NULL, _t550, 1)};
    GemVal _t552 = (*gem_v__mod_parser_make_float);
            GemVal _t553 = _t552.fn(_t552.env, _t551, 1);
            gem_pop_frame();
            return _t553;
        }
#line 190 "compiler/main.gem"
    GemVal _t554 = gem_v_t;
    GemVal _t555[] = {gem_table_get(_t554, gem_string("value"))};
    GemVal _t556[] = {gem_fn__mod_parser_str_to_int(NULL, _t555, 1)};
    GemVal _t557 = (*gem_v__mod_parser_make_int);
        GemVal _t558 = _t557.fn(_t557.env, _t556, 1);
        gem_pop_frame();
        return _t558;
    }
#line 194 "compiler/main.gem"
    GemVal _t559 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t559, gem_string("type")), gem_string("STRING")))) {
#line 195 "compiler/main.gem"
    GemVal _t560 = (*gem_v_advance);
        (void)(_t560.fn(_t560.env, NULL, 0));
#line 196 "compiler/main.gem"
    GemVal _t561 = gem_v_t;
    GemVal _t562[] = {gem_table_get(_t561, gem_string("value"))};
    GemVal _t563 = (*gem_v__mod_parser_make_string);
        GemVal _t564 = _t563.fn(_t563.env, _t562, 1);
        gem_pop_frame();
        return _t564;
    }
#line 200 "compiler/main.gem"
    GemVal _t565 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t565, gem_string("type")), gem_string("INTERP_START")))) {
#line 201 "compiler/main.gem"
    GemVal _t566 = (*gem_v_advance);
        (void)(_t566.fn(_t566.env, NULL, 0));
#line 202 "compiler/main.gem"
    GemVal _t567 = gem_table_new();
        GemVal gem_v_parts = _t567;
#line 203 "compiler/main.gem"
    GemVal _t568 = (*gem_v_skip_nl);
        (void)(_t568.fn(_t568.env, NULL, 0));
#line 204 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t569 = (*gem_v_peek);
            GemVal _t570 = _t569.fn(_t569.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t570, gem_string("type")), gem_string("INTERP_END")))) break;
#line 205 "compiler/main.gem"
    GemVal _t571 = (*gem_v_peek);
    GemVal _t572 = _t571.fn(_t571.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t572, gem_string("type")), gem_string("STRING")))) {
#line 206 "compiler/main.gem"
    GemVal _t573 = (*gem_v_advance);
    GemVal _t574 = _t573.fn(_t573.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t574, gem_string("value"));
#line 207 "compiler/main.gem"
    GemVal _t575[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t575, 1), gem_int(0)))) {
#line 208 "compiler/main.gem"
    GemVal _t576[] = {gem_v_sval};
    GemVal _t577 = (*gem_v__mod_parser_make_string);
    GemVal _t578[] = {gem_v_parts, _t577.fn(_t577.env, _t576, 1)};
                    (void)(gem_push_fn(NULL, _t578, 2));
                }
            } else {
#line 211 "compiler/main.gem"
    GemVal _t579 = (*gem_v_parse_expr);
    GemVal _t580[] = {gem_v_parts, _t579.fn(_t579.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t580, 2));
            }
#line 213 "compiler/main.gem"
    GemVal _t581 = (*gem_v_skip_nl);
            (void)(_t581.fn(_t581.env, NULL, 0));
        }
#line 215 "compiler/main.gem"
    GemVal _t582[] = {gem_string("INTERP_END")};
    GemVal _t583 = (*gem_v_expect);
        (void)(_t583.fn(_t583.env, _t582, 1));
#line 216 "compiler/main.gem"
    GemVal _t584[] = {gem_v_parts};
    GemVal _t585 = (*gem_v__mod_parser_make_interp);
        GemVal _t586 = _t585.fn(_t585.env, _t584, 1);
        gem_pop_frame();
        return _t586;
    }
#line 220 "compiler/main.gem"
    GemVal _t587 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t587, gem_string("type")), gem_string("true")))) {
#line 221 "compiler/main.gem"
    GemVal _t588 = (*gem_v_advance);
        (void)(_t588.fn(_t588.env, NULL, 0));
#line 222 "compiler/main.gem"
    GemVal _t589[] = {gem_bool(1)};
    GemVal _t590 = (*gem_v__mod_parser_make_bool);
        GemVal _t591 = _t590.fn(_t590.env, _t589, 1);
        gem_pop_frame();
        return _t591;
    }
#line 224 "compiler/main.gem"
    GemVal _t592 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t592, gem_string("type")), gem_string("false")))) {
#line 225 "compiler/main.gem"
    GemVal _t593 = (*gem_v_advance);
        (void)(_t593.fn(_t593.env, NULL, 0));
#line 226 "compiler/main.gem"
    GemVal _t594[] = {gem_bool(0)};
    GemVal _t595 = (*gem_v__mod_parser_make_bool);
        GemVal _t596 = _t595.fn(_t595.env, _t594, 1);
        gem_pop_frame();
        return _t596;
    }
#line 230 "compiler/main.gem"
    GemVal _t597 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t597, gem_string("type")), gem_string("nil")))) {
#line 231 "compiler/main.gem"
    GemVal _t598 = (*gem_v_advance);
        (void)(_t598.fn(_t598.env, NULL, 0));
#line 232 "compiler/main.gem"
    GemVal _t599 = (*gem_v__mod_parser_make_nil_node);
        GemVal _t600 = _t599.fn(_t599.env, NULL, 0);
        gem_pop_frame();
        return _t600;
    }
#line 236 "compiler/main.gem"
    GemVal _t601 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t601, gem_string("type")), gem_string("fn")))) {
#line 237 "compiler/main.gem"
    GemVal _t602 = (*gem_v_advance);
        (void)(_t602.fn(_t602.env, NULL, 0));
#line 238 "compiler/main.gem"
    GemVal _t603[] = {gem_string("(")};
    GemVal _t604 = (*gem_v_expect);
        (void)(_t604.fn(_t604.env, _t603, 1));
#line 239 "compiler/main.gem"
    GemVal _t605 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t605.fn(_t605.env, NULL, 0);
#line 240 "compiler/main.gem"
    GemVal _t606[] = {gem_string(")")};
    GemVal _t607 = (*gem_v_expect);
        (void)(_t607.fn(_t607.env, _t606, 1));
#line 241 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 242 "compiler/main.gem"
    GemVal _t608 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t608.fn(_t608.env, NULL, 0);
#line 243 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 244 "compiler/main.gem"
    GemVal _t609[] = {gem_string("end")};
    GemVal _t610 = (*gem_v_expect);
        (void)(_t610.fn(_t610.env, _t609, 1));
#line 245 "compiler/main.gem"
    GemVal _t611 = gem_v_pr;
    GemVal _t612 = gem_v_pr;
    GemVal _t613 = gem_v_pr;
    GemVal _t614[] = {gem_table_get(_t611, gem_string("params")), gem_table_get(_t612, gem_string("rest_param")), gem_table_get(_t613, gem_string("block_param")), gem_v_body};
    GemVal _t615 = (*gem_v__mod_parser_make_anon_fn);
        GemVal _t616 = _t615.fn(_t615.env, _t614, 4);
        gem_pop_frame();
        return _t616;
    }
#line 250 "compiler/main.gem"
    GemVal _t617 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t617, gem_string("type")), gem_string("{")))) {
#line 251 "compiler/main.gem"
    GemVal _t618 = (*gem_v_advance);
        (void)(_t618.fn(_t618.env, NULL, 0));
#line 252 "compiler/main.gem"
    GemVal _t619 = (*gem_v_skip_nl);
        (void)(_t619.fn(_t619.env, NULL, 0));
#line 253 "compiler/main.gem"
    GemVal _t620 = gem_table_new();
        GemVal gem_v_entries = _t620;
#line 254 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t621 = (*gem_v_peek);
            GemVal _t622 = _t621.fn(_t621.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t622, gem_string("type")), gem_string("}")))) break;
#line 255 "compiler/main.gem"
    GemVal _t623 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t623.fn(_t623.env, NULL, 0);
#line 256 "compiler/main.gem"
    GemVal _t624 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t624, gem_string("type")), gem_string("NAME")))) {
#line 257 "compiler/main.gem"
    GemVal _t625 = (*gem_v_advance);
                (void)(_t625.fn(_t625.env, NULL, 0));
            } else {
#line 258 "compiler/main.gem"
    GemVal _t626 = (*gem_v_at_end);
    GemVal _t630;
    if (!gem_truthy(gem_not(_t626.fn(_t626.env, NULL, 0)))) {
        _t630 = gem_not(_t626.fn(_t626.env, NULL, 0));
    } else {
        GemVal _t627[] = {gem_int(1)};
        GemVal _t628 = (*gem_v_peek_at);
        GemVal _t629 = _t628.fn(_t628.env, _t627, 1);
        _t630 = gem_eq(gem_table_get(_t629, gem_string("type")), gem_string(":"));
    }
                if (gem_truthy(_t630)) {
#line 260 "compiler/main.gem"
    GemVal _t631 = (*gem_v_advance);
                    (void)(_t631.fn(_t631.env, NULL, 0));
                } else {
#line 262 "compiler/main.gem"
    GemVal _t632 = gem_v_key_tok;
    GemVal _t633 = gem_v_key_tok;
    GemVal _t634 = gem_v_key_tok;
    GemVal _t635[] = {gem_table_get(_t634, gem_string("value"))};
    GemVal _t636 = gem_v_key_tok;
    GemVal _t637[] = {gem_table_get(_t636, gem_string("type"))};
    GemVal _t638[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t632, gem_string("line")), gem_table_get(_t633, gem_string("col")), gem_len_fn(NULL, _t635, 1), gem_add(gem_add(gem_string("expected '}' or table key but got '"), gem_to_string_fn(NULL, _t637, 1)), gem_string("'")), GEM_NIL};
    GemVal _t639 = (*gem_v__mod_parser_compile_error);
                    (void)(_t639.fn(_t639.env, _t638, 7));
                }
            }
#line 264 "compiler/main.gem"
    GemVal _t640[] = {gem_string(":")};
    GemVal _t641 = (*gem_v_expect);
            (void)(_t641.fn(_t641.env, _t640, 1));
#line 265 "compiler/main.gem"
    GemVal _t642 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t642.fn(_t642.env, NULL, 0);
#line 266 "compiler/main.gem"
    GemVal _t643 = gem_v_key_tok;
    GemVal _t644[] = {gem_table_get(_t643, gem_string("value")), gem_v_val};
    GemVal _t645 = (*gem_v__mod_parser_make_table_entry);
    GemVal _t646[] = {gem_v_entries, _t645.fn(_t645.env, _t644, 2)};
            (void)(gem_push_fn(NULL, _t646, 2));
#line 267 "compiler/main.gem"
    GemVal _t647 = (*gem_v_skip_nl);
            (void)(_t647.fn(_t647.env, NULL, 0));
#line 268 "compiler/main.gem"
    GemVal _t648 = (*gem_v_peek);
    GemVal _t649 = _t648.fn(_t648.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t649, gem_string("type")), gem_string(",")))) {
#line 269 "compiler/main.gem"
    GemVal _t650 = (*gem_v_advance);
                (void)(_t650.fn(_t650.env, NULL, 0));
#line 270 "compiler/main.gem"
    GemVal _t651 = (*gem_v_skip_nl);
                (void)(_t651.fn(_t651.env, NULL, 0));
            }
        }
#line 273 "compiler/main.gem"
    GemVal _t652[] = {gem_string("}")};
    GemVal _t653 = (*gem_v_expect);
        (void)(_t653.fn(_t653.env, _t652, 1));
#line 274 "compiler/main.gem"
    GemVal _t654[] = {gem_v_entries};
    GemVal _t655 = (*gem_v__mod_parser_make_table);
        GemVal _t656 = _t655.fn(_t655.env, _t654, 1);
        gem_pop_frame();
        return _t656;
    }
#line 278 "compiler/main.gem"
    GemVal _t657 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t657, gem_string("type")), gem_string("[")))) {
#line 279 "compiler/main.gem"
    GemVal _t658 = (*gem_v_advance);
        (void)(_t658.fn(_t658.env, NULL, 0));
#line 280 "compiler/main.gem"
    GemVal _t659 = (*gem_v_skip_nl);
        (void)(_t659.fn(_t659.env, NULL, 0));
#line 281 "compiler/main.gem"
    GemVal _t660 = gem_table_new();
        GemVal gem_v_elements = _t660;
#line 282 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t661 = (*gem_v_peek);
            GemVal _t662 = _t661.fn(_t661.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t662, gem_string("type")), gem_string("]")))) break;
#line 283 "compiler/main.gem"
    GemVal _t663 = (*gem_v_parse_expr);
    GemVal _t664[] = {gem_v_elements, _t663.fn(_t663.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t664, 2));
#line 284 "compiler/main.gem"
    GemVal _t665 = (*gem_v_skip_nl);
            (void)(_t665.fn(_t665.env, NULL, 0));
#line 285 "compiler/main.gem"
    GemVal _t666 = (*gem_v_peek);
    GemVal _t667 = _t666.fn(_t666.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t667, gem_string("type")), gem_string(",")))) {
#line 286 "compiler/main.gem"
    GemVal _t668 = (*gem_v_advance);
                (void)(_t668.fn(_t668.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t669 = (*gem_v_skip_nl);
                (void)(_t669.fn(_t669.env, NULL, 0));
            }
        }
#line 290 "compiler/main.gem"
    GemVal _t670[] = {gem_string("]")};
    GemVal _t671 = (*gem_v_expect);
        (void)(_t671.fn(_t671.env, _t670, 1));
#line 291 "compiler/main.gem"
    GemVal _t672[] = {gem_v_elements};
    GemVal _t673 = (*gem_v__mod_parser_make_array);
        GemVal _t674 = _t673.fn(_t673.env, _t672, 1);
        gem_pop_frame();
        return _t674;
    }
#line 295 "compiler/main.gem"
    GemVal _t675 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t675, gem_string("type")), gem_string("(")))) {
#line 296 "compiler/main.gem"
    GemVal _t676 = (*gem_v_advance);
        (void)(_t676.fn(_t676.env, NULL, 0));
#line 297 "compiler/main.gem"
    GemVal _t677 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t677.fn(_t677.env, NULL, 0);
#line 298 "compiler/main.gem"
    GemVal _t678[] = {gem_string(")")};
    GemVal _t679 = (*gem_v_expect);
        (void)(_t679.fn(_t679.env, _t678, 1));
#line 299 "compiler/main.gem"
        GemVal _t680 = gem_v_e;
        gem_pop_frame();
        return _t680;
    }
#line 303 "compiler/main.gem"
    GemVal _t681 = gem_v_t;
    GemVal _t683;
    if (!gem_truthy(gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("NAME")))) {
        _t683 = gem_eq(gem_table_get(_t681, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t682 = gem_v_t;
        _t683 = gem_eq(gem_table_get(_t682, gem_string("value")), gem_string("pcall"));
    }
    GemVal _t687;
    if (!gem_truthy(_t683)) {
        _t687 = _t683;
    } else {
        GemVal _t684[] = {gem_int(1)};
        GemVal _t685 = (*gem_v_peek_at);
        GemVal _t686 = _t685.fn(_t685.env, _t684, 1);
        _t687 = gem_neq(gem_table_get(_t686, gem_string("type")), gem_string("("));
    }
    if (gem_truthy(_t687)) {
#line 304 "compiler/main.gem"
    GemVal _t688 = (*gem_v_advance);
        (void)(_t688.fn(_t688.env, NULL, 0));
#line 305 "compiler/main.gem"
    GemVal _t689 = (*gem_v_parse_expr);
        GemVal gem_v_expr = _t689.fn(_t689.env, NULL, 0);
#line 306 "compiler/main.gem"
    GemVal _t690[] = {gem_string("pcall")};
    GemVal _t691 = (*gem_v__mod_parser_make_var);
    GemVal _t692 = gem_table_new();
    GemVal _t693 = gem_table_new();
    GemVal _t694 = gem_table_new();
    gem_table_set(_t694, gem_int(0), gem_v_expr);
    GemVal _t695[] = {_t693, GEM_NIL, GEM_NIL, _t694};
    GemVal _t696 = (*gem_v__mod_parser_make_anon_fn);
    gem_table_set(_t692, gem_int(0), _t696.fn(_t696.env, _t695, 4));
    GemVal _t697 = gem_v_t;
    GemVal _t698[] = {_t691.fn(_t691.env, _t690, 1), _t692, gem_table_get(_t697, gem_string("line"))};
    GemVal _t699 = (*gem_v__mod_parser_make_call);
        GemVal _t700 = _t699.fn(_t699.env, _t698, 3);
        gem_pop_frame();
        return _t700;
    }
#line 310 "compiler/main.gem"
    GemVal _t701 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t701, gem_string("type")), gem_string("NAME")))) {
#line 311 "compiler/main.gem"
    GemVal _t702 = (*gem_v_advance);
        (void)(_t702.fn(_t702.env, NULL, 0));
#line 312 "compiler/main.gem"
    GemVal _t703 = gem_v_t;
    GemVal _t704[] = {gem_table_get(_t703, gem_string("value"))};
    GemVal _t705 = (*gem_v__mod_parser_make_var);
        GemVal _t706 = _t705.fn(_t705.env, _t704, 1);
        gem_pop_frame();
        return _t706;
    }
#line 315 "compiler/main.gem"
    GemVal _t707 = gem_v_t;
    GemVal _t708 = gem_v_t;
    GemVal _t709 = gem_v_t;
    GemVal _t710[] = {gem_table_get(_t709, gem_string("value"))};
    GemVal _t711 = gem_v_t;
    GemVal _t712[] = {gem_table_get(_t711, gem_string("type"))};
    GemVal _t713[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t707, gem_string("line")), gem_table_get(_t708, gem_string("col")), gem_len_fn(NULL, _t710, 1), gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t712, 1)), gem_string("' (expected expression)")), GEM_NIL};
    GemVal _t714 = (*gem_v__mod_parser_compile_error);
    GemVal _t715 = _t714.fn(_t714.env, _t713, 7);
    gem_pop_frame();
    return _t715;
}

struct _closure__anon_11 {
    GemVal *gem_v__mod_parser_make_anon_fn;
    GemVal *gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_dot;
    GemVal *gem_v__mod_parser_make_index;
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
    GemVal *gem_v__mod_parser_make_anon_fn = _cls->gem_v__mod_parser_make_anon_fn;
    GemVal *gem_v__mod_parser_make_call = _cls->gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_dot = _cls->gem_v__mod_parser_make_dot;
    GemVal *gem_v__mod_parser_make_index = _cls->gem_v__mod_parser_make_index;
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
#line 320 "compiler/main.gem"
    GemVal _t717 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t717.fn(_t717.env, NULL, 0);
#line 322 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 324 "compiler/main.gem"
    GemVal _t718 = (*gem_v_peek);
    GemVal _t719 = _t718.fn(_t718.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t719, gem_string("type")), gem_string("(")))) {
#line 325 "compiler/main.gem"
    GemVal _t720 = (*gem_v_peek);
    GemVal _t721 = _t720.fn(_t720.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t721, gem_string("line"));
#line 326 "compiler/main.gem"
    GemVal _t722 = (*gem_v_advance);
            (void)(_t722.fn(_t722.env, NULL, 0));
#line 327 "compiler/main.gem"
    GemVal _t723 = (*gem_v_skip_nl);
            (void)(_t723.fn(_t723.env, NULL, 0));
#line 328 "compiler/main.gem"
    GemVal _t724 = gem_table_new();
            GemVal gem_v_args = _t724;
#line 329 "compiler/main.gem"
    GemVal _t725 = (*gem_v_peek);
    GemVal _t726 = _t725.fn(_t725.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t726, gem_string("type")), gem_string(")")))) {
#line 330 "compiler/main.gem"
    GemVal _t727 = (*gem_v_parse_expr);
    GemVal _t728[] = {gem_v_args, _t727.fn(_t727.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t728, 2));
#line 331 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t729 = (*gem_v_peek);
                    GemVal _t730 = _t729.fn(_t729.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t730, gem_string("type")), gem_string(",")))) break;
#line 332 "compiler/main.gem"
    GemVal _t731 = (*gem_v_advance);
                    (void)(_t731.fn(_t731.env, NULL, 0));
#line 333 "compiler/main.gem"
    GemVal _t732 = (*gem_v_skip_nl);
                    (void)(_t732.fn(_t732.env, NULL, 0));
#line 334 "compiler/main.gem"
    GemVal _t733 = (*gem_v_parse_expr);
    GemVal _t734[] = {gem_v_args, _t733.fn(_t733.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t734, 2));
                }
            }
#line 337 "compiler/main.gem"
    GemVal _t735 = (*gem_v_skip_nl);
            (void)(_t735.fn(_t735.env, NULL, 0));
#line 338 "compiler/main.gem"
    GemVal _t736[] = {gem_string(")")};
    GemVal _t737 = (*gem_v_expect);
            (void)(_t737.fn(_t737.env, _t736, 1));
#line 341 "compiler/main.gem"
    GemVal _t738 = (*gem_v_peek);
    GemVal _t739 = _t738.fn(_t738.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t739, gem_string("type")), gem_string("do")))) {
#line 342 "compiler/main.gem"
    GemVal _t740 = (*gem_v_advance);
                (void)(_t740.fn(_t740.env, NULL, 0));
#line 343 "compiler/main.gem"
    GemVal _t741 = gem_table_new();
                GemVal gem_v_bparams = _t741;
#line 344 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 345 "compiler/main.gem"
                GemVal gem_v_bblock = GEM_NIL;
#line 346 "compiler/main.gem"
    GemVal _t742 = (*gem_v_peek);
    GemVal _t743 = _t742.fn(_t742.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t743, gem_string("type")), gem_string("|")))) {
#line 347 "compiler/main.gem"
    GemVal _t744 = (*gem_v_advance);
                    (void)(_t744.fn(_t744.env, NULL, 0));
#line 348 "compiler/main.gem"
    GemVal _t745 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t745.fn(_t745.env, NULL, 0);
#line 349 "compiler/main.gem"
    GemVal _t746 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t746, gem_string("params"));
#line 350 "compiler/main.gem"
    GemVal _t747 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t747, gem_string("rest_param"));
#line 351 "compiler/main.gem"
    GemVal _t748 = gem_v_bpr;
                    gem_v_bblock = gem_table_get(_t748, gem_string("block_param"));
#line 352 "compiler/main.gem"
    GemVal _t749[] = {gem_string("|")};
    GemVal _t750 = (*gem_v_expect);
                    (void)(_t750.fn(_t750.env, _t749, 1));
                }
#line 354 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 355 "compiler/main.gem"
    GemVal _t751 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t751.fn(_t751.env, NULL, 0);
#line 356 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t752[] = {gem_string("end")};
    GemVal _t753 = (*gem_v_expect);
                (void)(_t753.fn(_t753.env, _t752, 1));
#line 358 "compiler/main.gem"
    GemVal _t754[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, gem_v_body};
    GemVal _t755 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t756[] = {gem_v_args, _t755.fn(_t755.env, _t754, 4)};
                (void)(gem_push_fn(NULL, _t756, 2));
            } else {
#line 362 "compiler/main.gem"
    GemVal _t757 = (*gem_v_peek);
    GemVal _t758 = _t757.fn(_t757.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t758, gem_string("type")), gem_string("{")))) {
#line 363 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 364 "compiler/main.gem"
    GemVal _t759[] = {gem_int(1)};
    GemVal _t760 = (*gem_v_peek_at);
    GemVal _t761 = _t760.fn(_t760.env, _t759, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t761, gem_string("type")), gem_string("|")))) {
#line 365 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 366 "compiler/main.gem"
    GemVal _t762[] = {gem_int(1)};
    GemVal _t763 = (*gem_v_peek_at);
    GemVal _t764 = _t763.fn(_t763.env, _t762, 1);
    GemVal _t772;
    if (gem_truthy(gem_eq(gem_table_get(_t764, gem_string("type")), gem_string("}")))) {
        _t772 = gem_eq(gem_table_get(_t764, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t765[] = {gem_int(1)};
        GemVal _t766 = (*gem_v_peek_at);
        GemVal _t767 = _t766.fn(_t766.env, _t765, 1);
        GemVal _t771;
        if (!gem_truthy(gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("NAME")))) {
                _t771 = gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t768[] = {gem_int(2)};
                GemVal _t769 = (*gem_v_peek_at);
                GemVal _t770 = _t769.fn(_t769.env, _t768, 1);
                _t771 = gem_eq(gem_table_get(_t770, gem_string("type")), gem_string(":"));
        }
        _t772 = _t771;
    }
                        if (gem_truthy(_t772)) {
#line 367 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 369 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 371 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 372 "compiler/main.gem"
    GemVal _t773 = (*gem_v_advance);
                        (void)(_t773.fn(_t773.env, NULL, 0));
#line 373 "compiler/main.gem"
    GemVal _t774 = gem_table_new();
                        GemVal gem_v_bparams = _t774;
#line 374 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 375 "compiler/main.gem"
                        GemVal gem_v_bblock = GEM_NIL;
#line 376 "compiler/main.gem"
    GemVal _t775 = (*gem_v_peek);
    GemVal _t776 = _t775.fn(_t775.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t776, gem_string("type")), gem_string("|")))) {
#line 377 "compiler/main.gem"
    GemVal _t777 = (*gem_v_advance);
                            (void)(_t777.fn(_t777.env, NULL, 0));
#line 378 "compiler/main.gem"
    GemVal _t778 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t778.fn(_t778.env, NULL, 0);
#line 379 "compiler/main.gem"
    GemVal _t779 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t779, gem_string("params"));
#line 380 "compiler/main.gem"
    GemVal _t780 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t780, gem_string("rest_param"));
#line 381 "compiler/main.gem"
    GemVal _t781 = gem_v_bpr;
                            gem_v_bblock = gem_table_get(_t781, gem_string("block_param"));
#line 382 "compiler/main.gem"
    GemVal _t782[] = {gem_string("|")};
    GemVal _t783 = (*gem_v_expect);
                            (void)(_t783.fn(_t783.env, _t782, 1));
                        }
#line 384 "compiler/main.gem"
    GemVal _t784 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t784.fn(_t784.env, NULL, 0);
#line 385 "compiler/main.gem"
    GemVal _t785[] = {gem_string("}")};
    GemVal _t786 = (*gem_v_expect);
                        (void)(_t786.fn(_t786.env, _t785, 1));
#line 386 "compiler/main.gem"
    GemVal _t787 = gem_table_new();
    gem_table_set(_t787, gem_int(0), gem_v_bexpr);
    GemVal _t788[] = {gem_v_bparams, gem_v_brest, gem_v_bblock, _t787};
    GemVal _t789 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t790[] = {gem_v_args, _t789.fn(_t789.env, _t788, 4)};
                        (void)(gem_push_fn(NULL, _t790, 2));
                    }
                }
            }
#line 391 "compiler/main.gem"
    GemVal _t791[] = {gem_v_node, gem_v_args, gem_v_call_line};
    GemVal _t792 = (*gem_v__mod_parser_make_call);
            gem_v_node = _t792.fn(_t792.env, _t791, 3);
            continue;
        }
#line 396 "compiler/main.gem"
    GemVal _t793 = (*gem_v_peek);
    GemVal _t794 = _t793.fn(_t793.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t794, gem_string("type")), gem_string(".")))) {
#line 397 "compiler/main.gem"
    GemVal _t795 = (*gem_v_advance);
            (void)(_t795.fn(_t795.env, NULL, 0));
#line 398 "compiler/main.gem"
    GemVal _t796 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t796.fn(_t796.env, NULL, 0);
#line 399 "compiler/main.gem"
    GemVal _t797 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t797, gem_string("type")), gem_string("NAME")))) {
#line 400 "compiler/main.gem"
    GemVal _t798 = (*gem_v_advance);
                (void)(_t798.fn(_t798.env, NULL, 0));
            } else {
#line 403 "compiler/main.gem"
    GemVal _t799 = (*gem_v_advance);
                (void)(_t799.fn(_t799.env, NULL, 0));
            }
#line 405 "compiler/main.gem"
    GemVal _t800 = gem_v_field_tok;
    GemVal _t801[] = {gem_v_node, gem_table_get(_t800, gem_string("value"))};
    GemVal _t802 = (*gem_v__mod_parser_make_dot);
            gem_v_node = _t802.fn(_t802.env, _t801, 2);
            continue;
        }
#line 410 "compiler/main.gem"
    GemVal _t803 = (*gem_v_peek);
    GemVal _t804 = _t803.fn(_t803.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t804, gem_string("type")), gem_string("[")))) {
#line 411 "compiler/main.gem"
    GemVal _t805 = (*gem_v_advance);
            (void)(_t805.fn(_t805.env, NULL, 0));
#line 412 "compiler/main.gem"
    GemVal _t806 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t806.fn(_t806.env, NULL, 0);
#line 413 "compiler/main.gem"
    GemVal _t807[] = {gem_string("]")};
    GemVal _t808 = (*gem_v_expect);
            (void)(_t808.fn(_t808.env, _t807, 1));
#line 414 "compiler/main.gem"
    GemVal _t809[] = {gem_v_node, gem_v_key};
    GemVal _t810 = (*gem_v__mod_parser_make_index);
            gem_v_node = _t810.fn(_t810.env, _t809, 2);
            continue;
        }
        break;
    }
    GemVal _t811 = gem_v_node;
    gem_pop_frame();
    return _t811;
}

struct _closure__anon_12 {
    GemVal *gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_call;
    GemVal *gem_v_parse_unary;
    GemVal *gem_v_peek;
};
static GemVal _anon_12(void *_env, GemVal *args, int argc) {
    struct _closure__anon_12 *_cls = (struct _closure__anon_12 *)_env;
    GemVal *gem_v__mod_parser_make_unop = _cls->gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_call = _cls->gem_v_parse_call;
    GemVal *gem_v_parse_unary = _cls->gem_v_parse_unary;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_12", "compiler/main.gem", 0);
#line 426 "compiler/main.gem"
    GemVal _t813 = (*gem_v_peek);
    GemVal _t814 = _t813.fn(_t813.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t814, gem_string("type")), gem_string("-")))) {
#line 427 "compiler/main.gem"
    GemVal _t815 = (*gem_v_advance);
        (void)(_t815.fn(_t815.env, NULL, 0));
#line 428 "compiler/main.gem"
    GemVal _t816 = (*gem_v_parse_unary);
    GemVal _t817[] = {gem_string("-"), _t816.fn(_t816.env, NULL, 0)};
    GemVal _t818 = (*gem_v__mod_parser_make_unop);
        GemVal _t819 = _t818.fn(_t818.env, _t817, 2);
        gem_pop_frame();
        return _t819;
    }
#line 430 "compiler/main.gem"
    GemVal _t820 = (*gem_v_parse_call);
    GemVal _t821 = _t820.fn(_t820.env, NULL, 0);
    gem_pop_frame();
    return _t821;
}

struct _closure__anon_13 {
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_unary;
    GemVal *gem_v_peek;
};
static GemVal _anon_13(void *_env, GemVal *args, int argc) {
    struct _closure__anon_13 *_cls = (struct _closure__anon_13 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_unary = _cls->gem_v_parse_unary;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_13", "compiler/main.gem", 0);
#line 435 "compiler/main.gem"
    GemVal _t823 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t823.fn(_t823.env, NULL, 0);
#line 436 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t824 = (*gem_v_peek);
        GemVal _t825 = _t824.fn(_t824.env, NULL, 0);
        GemVal _t828;
        if (gem_truthy(gem_eq(gem_table_get(_t825, gem_string("type")), gem_string("*")))) {
                _t828 = gem_eq(gem_table_get(_t825, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t826 = (*gem_v_peek);
                GemVal _t827 = _t826.fn(_t826.env, NULL, 0);
                _t828 = gem_eq(gem_table_get(_t827, gem_string("type")), gem_string("/"));
        }
        GemVal _t831;
        if (gem_truthy(_t828)) {
                _t831 = _t828;
        } else {
                GemVal _t829 = (*gem_v_peek);
                GemVal _t830 = _t829.fn(_t829.env, NULL, 0);
                _t831 = gem_eq(gem_table_get(_t830, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t831)) break;
#line 437 "compiler/main.gem"
    GemVal _t832 = (*gem_v_advance);
    GemVal _t833 = _t832.fn(_t832.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t833, gem_string("type"));
#line 438 "compiler/main.gem"
    GemVal _t834 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t834.fn(_t834.env, NULL, 0);
#line 439 "compiler/main.gem"
    GemVal _t835[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t836 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t836.fn(_t836.env, _t835, 3);
    }
    GemVal _t837 = gem_v_left;
    gem_pop_frame();
    return _t837;
}

struct _closure__anon_14 {
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_mul;
    GemVal *gem_v_peek;
};
static GemVal _anon_14(void *_env, GemVal *args, int argc) {
    struct _closure__anon_14 *_cls = (struct _closure__anon_14 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_mul = _cls->gem_v_parse_mul;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_14", "compiler/main.gem", 0);
#line 446 "compiler/main.gem"
    GemVal _t839 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t839.fn(_t839.env, NULL, 0);
#line 447 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t840 = (*gem_v_peek);
        GemVal _t841 = _t840.fn(_t840.env, NULL, 0);
        GemVal _t844;
        if (gem_truthy(gem_eq(gem_table_get(_t841, gem_string("type")), gem_string("+")))) {
                _t844 = gem_eq(gem_table_get(_t841, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t842 = (*gem_v_peek);
                GemVal _t843 = _t842.fn(_t842.env, NULL, 0);
                _t844 = gem_eq(gem_table_get(_t843, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t844)) break;
#line 448 "compiler/main.gem"
    GemVal _t845 = (*gem_v_advance);
    GemVal _t846 = _t845.fn(_t845.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t846, gem_string("type"));
#line 449 "compiler/main.gem"
    GemVal _t847 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t847.fn(_t847.env, NULL, 0);
#line 450 "compiler/main.gem"
    GemVal _t848[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t849 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t849.fn(_t849.env, _t848, 3);
    }
    GemVal _t850 = gem_v_left;
    gem_pop_frame();
    return _t850;
}

struct _closure__anon_15 {
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_add;
    GemVal *gem_v_peek;
};
static GemVal _anon_15(void *_env, GemVal *args, int argc) {
    struct _closure__anon_15 *_cls = (struct _closure__anon_15 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_add = _cls->gem_v_parse_add;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_15", "compiler/main.gem", 0);
#line 457 "compiler/main.gem"
    GemVal _t852 = (*gem_v_parse_add);
    GemVal gem_v_left = _t852.fn(_t852.env, NULL, 0);
#line 458 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t853 = (*gem_v_peek);
        GemVal _t854 = _t853.fn(_t853.env, NULL, 0);
        GemVal _t857;
        if (gem_truthy(gem_eq(gem_table_get(_t854, gem_string("type")), gem_string("==")))) {
                _t857 = gem_eq(gem_table_get(_t854, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t855 = (*gem_v_peek);
                GemVal _t856 = _t855.fn(_t855.env, NULL, 0);
                _t857 = gem_eq(gem_table_get(_t856, gem_string("type")), gem_string("!="));
        }
        GemVal _t860;
        if (gem_truthy(_t857)) {
                _t860 = _t857;
        } else {
                GemVal _t858 = (*gem_v_peek);
                GemVal _t859 = _t858.fn(_t858.env, NULL, 0);
                _t860 = gem_eq(gem_table_get(_t859, gem_string("type")), gem_string("<"));
        }
        GemVal _t863;
        if (gem_truthy(_t860)) {
                _t863 = _t860;
        } else {
                GemVal _t861 = (*gem_v_peek);
                GemVal _t862 = _t861.fn(_t861.env, NULL, 0);
                _t863 = gem_eq(gem_table_get(_t862, gem_string("type")), gem_string(">"));
        }
        GemVal _t866;
        if (gem_truthy(_t863)) {
                _t866 = _t863;
        } else {
                GemVal _t864 = (*gem_v_peek);
                GemVal _t865 = _t864.fn(_t864.env, NULL, 0);
                _t866 = gem_eq(gem_table_get(_t865, gem_string("type")), gem_string("<="));
        }
        GemVal _t869;
        if (gem_truthy(_t866)) {
                _t869 = _t866;
        } else {
                GemVal _t867 = (*gem_v_peek);
                GemVal _t868 = _t867.fn(_t867.env, NULL, 0);
                _t869 = gem_eq(gem_table_get(_t868, gem_string("type")), gem_string(">="));
        }
        GemVal _t872;
        if (gem_truthy(_t869)) {
                _t872 = _t869;
        } else {
                GemVal _t870 = (*gem_v_peek);
                GemVal _t871 = _t870.fn(_t870.env, NULL, 0);
                _t872 = gem_eq(gem_table_get(_t871, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t872)) break;
#line 459 "compiler/main.gem"
    GemVal _t873 = (*gem_v_advance);
    GemVal _t874 = _t873.fn(_t873.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t874, gem_string("type"));
#line 460 "compiler/main.gem"
    GemVal _t875 = (*gem_v_parse_add);
        GemVal gem_v_right = _t875.fn(_t875.env, NULL, 0);
#line 461 "compiler/main.gem"
    GemVal _t876[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t877 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t877.fn(_t877.env, _t876, 3);
    }
    GemVal _t878 = gem_v_left;
    gem_pop_frame();
    return _t878;
}

struct _closure__anon_16 {
    GemVal *gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_compare;
    GemVal *gem_v_parse_not;
    GemVal *gem_v_peek;
};
static GemVal _anon_16(void *_env, GemVal *args, int argc) {
    struct _closure__anon_16 *_cls = (struct _closure__anon_16 *)_env;
    GemVal *gem_v__mod_parser_make_unop = _cls->gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_compare = _cls->gem_v_parse_compare;
    GemVal *gem_v_parse_not = _cls->gem_v_parse_not;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_16", "compiler/main.gem", 0);
#line 468 "compiler/main.gem"
    GemVal _t880 = (*gem_v_peek);
    GemVal _t881 = _t880.fn(_t880.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t881, gem_string("type")), gem_string("not")))) {
#line 469 "compiler/main.gem"
    GemVal _t882 = (*gem_v_advance);
        (void)(_t882.fn(_t882.env, NULL, 0));
#line 470 "compiler/main.gem"
    GemVal _t883 = (*gem_v_parse_not);
    GemVal _t884[] = {gem_string("not"), _t883.fn(_t883.env, NULL, 0)};
    GemVal _t885 = (*gem_v__mod_parser_make_unop);
        GemVal _t886 = _t885.fn(_t885.env, _t884, 2);
        gem_pop_frame();
        return _t886;
    }
#line 472 "compiler/main.gem"
    GemVal _t887 = (*gem_v_parse_compare);
    GemVal _t888 = _t887.fn(_t887.env, NULL, 0);
    gem_pop_frame();
    return _t888;
}

struct _closure__anon_17 {
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_not;
    GemVal *gem_v_peek;
};
static GemVal _anon_17(void *_env, GemVal *args, int argc) {
    struct _closure__anon_17 *_cls = (struct _closure__anon_17 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_not = _cls->gem_v_parse_not;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_17", "compiler/main.gem", 0);
#line 477 "compiler/main.gem"
    GemVal _t890 = (*gem_v_parse_not);
    GemVal gem_v_left = _t890.fn(_t890.env, NULL, 0);
#line 478 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t891 = (*gem_v_peek);
        GemVal _t892 = _t891.fn(_t891.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t892, gem_string("type")), gem_string("and")))) break;
#line 479 "compiler/main.gem"
    GemVal _t893 = (*gem_v_advance);
        (void)(_t893.fn(_t893.env, NULL, 0));
#line 480 "compiler/main.gem"
    GemVal _t894 = (*gem_v_parse_not);
        GemVal gem_v_right = _t894.fn(_t894.env, NULL, 0);
#line 481 "compiler/main.gem"
    GemVal _t895[] = {gem_string("and"), gem_v_left, gem_v_right};
    GemVal _t896 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t896.fn(_t896.env, _t895, 3);
    }
    GemVal _t897 = gem_v_left;
    gem_pop_frame();
    return _t897;
}

struct _closure__anon_18 {
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_and;
    GemVal *gem_v_peek;
};
static GemVal _anon_18(void *_env, GemVal *args, int argc) {
    struct _closure__anon_18 *_cls = (struct _closure__anon_18 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_and = _cls->gem_v_parse_and;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_18", "compiler/main.gem", 0);
#line 488 "compiler/main.gem"
    GemVal _t899 = (*gem_v_parse_and);
    GemVal gem_v_left = _t899.fn(_t899.env, NULL, 0);
#line 489 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t900 = (*gem_v_peek);
        GemVal _t901 = _t900.fn(_t900.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t901, gem_string("type")), gem_string("or")))) break;
#line 490 "compiler/main.gem"
    GemVal _t902 = (*gem_v_advance);
        (void)(_t902.fn(_t902.env, NULL, 0));
#line 491 "compiler/main.gem"
    GemVal _t903 = (*gem_v_parse_and);
        GemVal gem_v_right = _t903.fn(_t903.env, NULL, 0);
#line 492 "compiler/main.gem"
    GemVal _t904[] = {gem_string("or"), gem_v_left, gem_v_right};
    GemVal _t905 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t905.fn(_t905.env, _t904, 3);
    }
    GemVal _t906 = gem_v_left;
    gem_pop_frame();
    return _t906;
}

struct _closure__anon_19 {
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_assign;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_dot_assign;
    GemVal *gem_v__mod_parser_make_index_assign;
    GemVal *gem_v__mod_parser_make_var;
    GemVal *gem_v_advance;
    GemVal *gem_v_file;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_parse_or;
    GemVal *gem_v_peek;
    GemVal *gem_v_source;
};
static GemVal _anon_19(void *_env, GemVal *args, int argc) {
    struct _closure__anon_19 *_cls = (struct _closure__anon_19 *)_env;
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_assign = _cls->gem_v__mod_parser_make_assign;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_dot_assign = _cls->gem_v__mod_parser_make_dot_assign;
    GemVal *gem_v__mod_parser_make_index_assign = _cls->gem_v__mod_parser_make_index_assign;
    GemVal *gem_v__mod_parser_make_var = _cls->gem_v__mod_parser_make_var;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_parse_or = _cls->gem_v_parse_or;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_19", "compiler/main.gem", 0);
#line 499 "compiler/main.gem"
    GemVal _t908 = (*gem_v_peek);
    GemVal _t909 = _t908.fn(_t908.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t909, gem_string("line"));
#line 500 "compiler/main.gem"
    GemVal _t910 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t910.fn(_t910.env, NULL, 0);
#line 503 "compiler/main.gem"
    GemVal _t911 = (*gem_v_peek);
    GemVal _t912 = _t911.fn(_t911.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t912, gem_string("type")), gem_string("=")))) {
#line 504 "compiler/main.gem"
    GemVal _t913 = (*gem_v_advance);
        (void)(_t913.fn(_t913.env, NULL, 0));
#line 505 "compiler/main.gem"
    GemVal _t914 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t914.fn(_t914.env, NULL, 0);
#line 506 "compiler/main.gem"
    GemVal _t915 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t915, gem_string("tag")), gem_string("var")))) {
#line 507 "compiler/main.gem"
    GemVal _t916 = gem_v_lhs;
    GemVal _t917[] = {gem_table_get(_t916, gem_string("name")), gem_v_value, gem_v_start_line};
    GemVal _t918 = (*gem_v__mod_parser_make_assign);
            GemVal _t919 = _t918.fn(_t918.env, _t917, 3);
            gem_pop_frame();
            return _t919;
        }
#line 509 "compiler/main.gem"
    GemVal _t920 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t920, gem_string("tag")), gem_string("dot")))) {
#line 510 "compiler/main.gem"
    GemVal _t921 = gem_v_lhs;
    GemVal _t922 = gem_v_lhs;
    GemVal _t923[] = {gem_table_get(_t921, gem_string("object")), gem_table_get(_t922, gem_string("field")), gem_v_value, gem_v_start_line};
    GemVal _t924 = (*gem_v__mod_parser_make_dot_assign);
            GemVal _t925 = _t924.fn(_t924.env, _t923, 4);
            gem_pop_frame();
            return _t925;
        }
#line 512 "compiler/main.gem"
    GemVal _t926 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t926, gem_string("tag")), gem_string("index")))) {
#line 513 "compiler/main.gem"
    GemVal _t927 = gem_v_lhs;
    GemVal _t928 = gem_v_lhs;
    GemVal _t929[] = {gem_table_get(_t927, gem_string("object")), gem_table_get(_t928, gem_string("key")), gem_v_value, gem_v_start_line};
    GemVal _t930 = (*gem_v__mod_parser_make_index_assign);
            GemVal _t931 = _t930.fn(_t930.env, _t929, 4);
            gem_pop_frame();
            return _t931;
        }
#line 515 "compiler/main.gem"
    GemVal _t932[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
    GemVal _t933 = (*gem_v__mod_parser_compile_error);
        (void)(_t933.fn(_t933.env, _t932, 7));
    }
#line 519 "compiler/main.gem"
    GemVal _t934 = (*gem_v_peek);
    GemVal _t935 = _t934.fn(_t934.env, NULL, 0);
    GemVal _t938;
    if (gem_truthy(gem_eq(gem_table_get(_t935, gem_string("type")), gem_string("+=")))) {
        _t938 = gem_eq(gem_table_get(_t935, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t936 = (*gem_v_peek);
        GemVal _t937 = _t936.fn(_t936.env, NULL, 0);
        _t938 = gem_eq(gem_table_get(_t937, gem_string("type")), gem_string("-="));
    }
    GemVal _t941;
    if (gem_truthy(_t938)) {
        _t941 = _t938;
    } else {
        GemVal _t939 = (*gem_v_peek);
        GemVal _t940 = _t939.fn(_t939.env, NULL, 0);
        _t941 = gem_eq(gem_table_get(_t940, gem_string("type")), gem_string("*="));
    }
    GemVal _t944;
    if (gem_truthy(_t941)) {
        _t944 = _t941;
    } else {
        GemVal _t942 = (*gem_v_peek);
        GemVal _t943 = _t942.fn(_t942.env, NULL, 0);
        _t944 = gem_eq(gem_table_get(_t943, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t944)) {
#line 520 "compiler/main.gem"
    GemVal _t945 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t945.fn(_t945.env, NULL, 0);
#line 521 "compiler/main.gem"
    GemVal _t946 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t946, gem_string("value")), gem_int(0));
#line 522 "compiler/main.gem"
    GemVal _t947 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t947.fn(_t947.env, NULL, 0);
#line 523 "compiler/main.gem"
    GemVal _t948 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t948, gem_string("tag")), gem_string("var")))) {
#line 524 "compiler/main.gem"
    GemVal _t949 = gem_v_op_tok;
    GemVal _t950 = gem_v_op_tok;
    GemVal _t951 = gem_v_op_tok;
    GemVal _t952[] = {gem_table_get(_t951, gem_string("value"))};
    GemVal _t953[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t949, gem_string("line")), gem_table_get(_t950, gem_string("col")), gem_len_fn(NULL, _t952, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
    GemVal _t954 = (*gem_v__mod_parser_compile_error);
            (void)(_t954.fn(_t954.env, _t953, 7));
        }
#line 526 "compiler/main.gem"
    GemVal _t955 = gem_v_lhs;
    GemVal _t956 = gem_v_lhs;
    GemVal _t957[] = {gem_table_get(_t956, gem_string("name"))};
    GemVal _t958 = (*gem_v__mod_parser_make_var);
    GemVal _t959[] = {gem_v_base_op, _t958.fn(_t958.env, _t957, 1), gem_v_value};
    GemVal _t960 = (*gem_v__mod_parser_make_binop);
    GemVal _t961[] = {gem_table_get(_t955, gem_string("name")), _t960.fn(_t960.env, _t959, 3), gem_v_start_line};
    GemVal _t962 = (*gem_v__mod_parser_make_assign);
        GemVal _t963 = _t962.fn(_t962.env, _t961, 3);
        gem_pop_frame();
        return _t963;
    }
    GemVal _t964 = gem_v_lhs;
    gem_pop_frame();
    return _t964;
}

struct _closure__anon_21 {
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_bool;
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    struct _closure__anon_21 *_cls = (struct _closure__anon_21 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_bool = _cls->gem_v__mod_parser_make_bool;
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 539 "compiler/main.gem"
    GemVal _t966[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t966, 1), gem_int(0)))) {
#line 540 "compiler/main.gem"
    GemVal _t967[] = {gem_bool(1)};
    GemVal _t968 = (*gem_v__mod_parser_make_bool);
        GemVal _t969 = _t968.fn(_t968.env, _t967, 1);
        gem_pop_frame();
        return _t969;
    }
#line 542 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 543 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(1);
#line 543 "compiler/main.gem"
    GemVal _t970[] = {gem_v_conditions};
    GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t970, 1);
#line 543 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 543 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_4;
#line 543 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 544 "compiler/main.gem"
    GemVal _t971[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
    GemVal _t972 = (*gem_v__mod_parser_make_binop);
        gem_v_result = _t972.fn(_t972.env, _t971, 3);
    }

    GemVal _t973 = gem_v_result;
    gem_pop_frame();
    return _t973;
}

struct _closure__anon_20 {
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_bool;
    GemVal *gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_float;
    GemVal *gem_v__mod_parser_make_index;
    GemVal *gem_v__mod_parser_make_int;
    GemVal *gem_v__mod_parser_make_let;
    GemVal *gem_v__mod_parser_make_nil_node;
    GemVal *gem_v__mod_parser_make_string;
    GemVal *gem_v__mod_parser_make_var;
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
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_bool = _cls->gem_v__mod_parser_make_bool;
    GemVal *gem_v__mod_parser_make_call = _cls->gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_float = _cls->gem_v__mod_parser_make_float;
    GemVal *gem_v__mod_parser_make_index = _cls->gem_v__mod_parser_make_index;
    GemVal *gem_v__mod_parser_make_int = _cls->gem_v__mod_parser_make_int;
    GemVal *gem_v__mod_parser_make_let = _cls->gem_v__mod_parser_make_let;
    GemVal *gem_v__mod_parser_make_nil_node = _cls->gem_v__mod_parser_make_nil_node;
    GemVal *gem_v__mod_parser_make_string = _cls->gem_v__mod_parser_make_string;
    GemVal *gem_v__mod_parser_make_var = _cls->gem_v__mod_parser_make_var;
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
#line 538 "compiler/main.gem"
    struct _closure__anon_21 *_t974 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t974->gem_v__mod_parser_make_binop = gem_v__mod_parser_make_binop;
    _t974->gem_v__mod_parser_make_bool = gem_v__mod_parser_make_bool;
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, _t974);
#line 548 "compiler/main.gem"
    GemVal _t975 = (*gem_v_peek);
    GemVal gem_v_t = _t975.fn(_t975.env, NULL, 0);
#line 551 "compiler/main.gem"
    GemVal _t976 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t976, gem_string("type")), gem_string("{")))) {
#line 552 "compiler/main.gem"
    GemVal _t977 = (*gem_v_advance);
        (void)(_t977.fn(_t977.env, NULL, 0));
#line 553 "compiler/main.gem"
    GemVal _t978 = (*gem_v_skip_nl);
        (void)(_t978.fn(_t978.env, NULL, 0));
#line 554 "compiler/main.gem"
    GemVal _t979 = gem_table_new();
    GemVal _t980[] = {gem_string("type")};
    GemVal _t981 = (*gem_v__mod_parser_make_var);
    GemVal _t982 = gem_table_new();
    gem_table_set(_t982, gem_int(0), gem_v_target_expr);
    GemVal _t983[] = {_t981.fn(_t981.env, _t980, 1), _t982, gem_int(0)};
    GemVal _t984 = (*gem_v__mod_parser_make_call);
    GemVal _t985[] = {gem_string("table")};
    GemVal _t986 = (*gem_v__mod_parser_make_string);
    GemVal _t987[] = {gem_string("=="), _t984.fn(_t984.env, _t983, 3), _t986.fn(_t986.env, _t985, 1)};
    GemVal _t988 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t979, gem_int(0), _t988.fn(_t988.env, _t987, 3));
        GemVal gem_v_conditions = _t979;
#line 555 "compiler/main.gem"
    GemVal _t989 = gem_table_new();
        GemVal gem_v_bindings = _t989;
#line 556 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t990 = (*gem_v_peek);
            GemVal _t991 = _t990.fn(_t990.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t991, gem_string("type")), gem_string("}")))) break;
#line 557 "compiler/main.gem"
    GemVal _t992 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t992.fn(_t992.env, NULL, 0);
#line 558 "compiler/main.gem"
    GemVal _t993 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t993, gem_string("type")), gem_string("NAME")))) {
#line 559 "compiler/main.gem"
    GemVal _t994 = (*gem_v_advance);
                (void)(_t994.fn(_t994.env, NULL, 0));
            } else {
#line 560 "compiler/main.gem"
    GemVal _t995[] = {gem_int(1)};
    GemVal _t996 = (*gem_v_peek_at);
    GemVal _t997 = _t996.fn(_t996.env, _t995, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t997, gem_string("type")), gem_string(":")))) {
#line 562 "compiler/main.gem"
    GemVal _t998 = (*gem_v_advance);
                    (void)(_t998.fn(_t998.env, NULL, 0));
                } else {
#line 564 "compiler/main.gem"
    GemVal _t999 = gem_v_key_tok;
    GemVal _t1000 = gem_v_key_tok;
    GemVal _t1001 = gem_v_key_tok;
    GemVal _t1002[] = {gem_table_get(_t1001, gem_string("value"))};
    GemVal _t1003[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t999, gem_string("line")), gem_table_get(_t1000, gem_string("col")), gem_len_fn(NULL, _t1002, 1), gem_string("expected key name in table pattern"), GEM_NIL};
    GemVal _t1004 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1004.fn(_t1004.env, _t1003, 7));
                }
            }
#line 566 "compiler/main.gem"
    GemVal _t1005 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t1005, gem_string("value"));
#line 567 "compiler/main.gem"
    GemVal _t1006[] = {gem_v_key_name};
    GemVal _t1007 = (*gem_v__mod_parser_make_string);
            GemVal gem_v_key_str = _t1007.fn(_t1007.env, _t1006, 1);
#line 568 "compiler/main.gem"
    GemVal _t1008[] = {gem_string("has_key")};
    GemVal _t1009 = (*gem_v__mod_parser_make_var);
    GemVal _t1010 = gem_table_new();
    gem_table_set(_t1010, gem_int(0), gem_v_target_expr);
    gem_table_set(_t1010, gem_int(1), gem_v_key_str);
    GemVal _t1011[] = {_t1009.fn(_t1009.env, _t1008, 1), _t1010, gem_int(0)};
    GemVal _t1012 = (*gem_v__mod_parser_make_call);
    GemVal _t1013[] = {gem_v_conditions, _t1012.fn(_t1012.env, _t1011, 3)};
            (void)(gem_push_fn(NULL, _t1013, 2));
#line 569 "compiler/main.gem"
    GemVal _t1014[] = {gem_v_target_expr, gem_v_key_str};
    GemVal _t1015 = (*gem_v__mod_parser_make_index);
            GemVal gem_v_sub_target = _t1015.fn(_t1015.env, _t1014, 2);
#line 570 "compiler/main.gem"
    GemVal _t1016 = (*gem_v_peek);
    GemVal _t1017 = _t1016.fn(_t1016.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1017, gem_string("type")), gem_string(":")))) {
#line 571 "compiler/main.gem"
    GemVal _t1018 = (*gem_v_advance);
                (void)(_t1018.fn(_t1018.env, NULL, 0));
#line 572 "compiler/main.gem"
    GemVal _t1019 = (*gem_v_skip_nl);
                (void)(_t1019.fn(_t1019.env, NULL, 0));
#line 573 "compiler/main.gem"
    GemVal _t1020[] = {gem_v_sub_target};
    GemVal _t1021 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t1021.fn(_t1021.env, _t1020, 1);
#line 574 "compiler/main.gem"
    GemVal _t1022 = gem_v_sub;
    GemVal _t1023 = gem_table_get(_t1022, gem_string("condition"));
    GemVal _t1026;
    if (gem_truthy(gem_neq(gem_table_get(_t1023, gem_string("tag")), gem_string("bool")))) {
        _t1026 = gem_neq(gem_table_get(_t1023, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t1024 = gem_v_sub;
        GemVal _t1025 = gem_table_get(_t1024, gem_string("condition"));
        _t1026 = gem_neq(gem_table_get(_t1025, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t1026)) {
#line 575 "compiler/main.gem"
    GemVal _t1027 = gem_v_sub;
    GemVal _t1028[] = {gem_v_conditions, gem_table_get(_t1027, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t1028, 2));
                }
#line 577 "compiler/main.gem"
    GemVal _t1029 = gem_v_sub;
                GemVal gem_v__for_items_5 = gem_table_get(_t1029, gem_string("bindings"));
#line 577 "compiler/main.gem"
                GemVal gem_v__for_i_5 = gem_int(0);
#line 577 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1030[] = {gem_v__for_items_5};
                    if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1030, 1)))) break;
#line 577 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 577 "compiler/main.gem"
                    gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 578 "compiler/main.gem"
    GemVal _t1031[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t1031, 2));
                }

            } else {
#line 582 "compiler/main.gem"
    GemVal _t1032[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t1033 = (*gem_v__mod_parser_make_let);
    GemVal _t1034[] = {gem_v_bindings, _t1033.fn(_t1033.env, _t1032, 3)};
                (void)(gem_push_fn(NULL, _t1034, 2));
            }
#line 584 "compiler/main.gem"
    GemVal _t1035 = (*gem_v_skip_nl);
            (void)(_t1035.fn(_t1035.env, NULL, 0));
#line 585 "compiler/main.gem"
    GemVal _t1036 = (*gem_v_peek);
    GemVal _t1037 = _t1036.fn(_t1036.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1037, gem_string("type")), gem_string(",")))) {
#line 586 "compiler/main.gem"
    GemVal _t1038 = (*gem_v_advance);
                (void)(_t1038.fn(_t1038.env, NULL, 0));
#line 587 "compiler/main.gem"
    GemVal _t1039 = (*gem_v_skip_nl);
                (void)(_t1039.fn(_t1039.env, NULL, 0));
            }
        }
#line 590 "compiler/main.gem"
    GemVal _t1040[] = {gem_string("}")};
    GemVal _t1041 = (*gem_v_expect);
        (void)(_t1041.fn(_t1041.env, _t1040, 1));
#line 591 "compiler/main.gem"
    GemVal _t1042 = gem_table_new();
    GemVal _t1043[] = {gem_v_conditions};
    GemVal _t1044 = gem_v_and_chain;
    gem_table_set(_t1042, gem_string("condition"), _t1044.fn(_t1044.env, _t1043, 1));
    gem_table_set(_t1042, gem_string("bindings"), gem_v_bindings);
        GemVal _t1045 = _t1042;
        gem_pop_frame();
        return _t1045;
    }
#line 595 "compiler/main.gem"
    GemVal _t1046 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1046, gem_string("type")), gem_string("[")))) {
#line 596 "compiler/main.gem"
    GemVal _t1047 = (*gem_v_advance);
        (void)(_t1047.fn(_t1047.env, NULL, 0));
#line 597 "compiler/main.gem"
    GemVal _t1048 = (*gem_v_skip_nl);
        (void)(_t1048.fn(_t1048.env, NULL, 0));
#line 598 "compiler/main.gem"
    GemVal _t1049 = gem_table_new();
        GemVal gem_v_sub_patterns = _t1049;
#line 599 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 600 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1050 = (*gem_v_peek);
            GemVal _t1051 = _t1050.fn(_t1050.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t1051, gem_string("type")), gem_string("]")))) break;
#line 601 "compiler/main.gem"
    GemVal _t1052[] = {gem_v_idx};
    GemVal _t1053 = (*gem_v__mod_parser_make_int);
    GemVal _t1054[] = {gem_v_target_expr, _t1053.fn(_t1053.env, _t1052, 1)};
    GemVal _t1055 = (*gem_v__mod_parser_make_index);
    GemVal _t1056[] = {_t1055.fn(_t1055.env, _t1054, 2)};
    GemVal _t1057 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t1057.fn(_t1057.env, _t1056, 1);
#line 602 "compiler/main.gem"
    GemVal _t1058[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t1058, 2));
#line 603 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 604 "compiler/main.gem"
    GemVal _t1059 = (*gem_v_skip_nl);
            (void)(_t1059.fn(_t1059.env, NULL, 0));
#line 605 "compiler/main.gem"
    GemVal _t1060 = (*gem_v_peek);
    GemVal _t1061 = _t1060.fn(_t1060.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1061, gem_string("type")), gem_string(",")))) {
#line 606 "compiler/main.gem"
    GemVal _t1062 = (*gem_v_advance);
                (void)(_t1062.fn(_t1062.env, NULL, 0));
#line 607 "compiler/main.gem"
    GemVal _t1063 = (*gem_v_skip_nl);
                (void)(_t1063.fn(_t1063.env, NULL, 0));
            }
        }
#line 610 "compiler/main.gem"
    GemVal _t1064[] = {gem_string("]")};
    GemVal _t1065 = (*gem_v_expect);
        (void)(_t1065.fn(_t1065.env, _t1064, 1));
#line 611 "compiler/main.gem"
    GemVal _t1066 = gem_table_new();
    GemVal _t1067[] = {gem_string("type")};
    GemVal _t1068 = (*gem_v__mod_parser_make_var);
    GemVal _t1069 = gem_table_new();
    gem_table_set(_t1069, gem_int(0), gem_v_target_expr);
    GemVal _t1070[] = {_t1068.fn(_t1068.env, _t1067, 1), _t1069, gem_int(0)};
    GemVal _t1071 = (*gem_v__mod_parser_make_call);
    GemVal _t1072[] = {gem_string("table")};
    GemVal _t1073 = (*gem_v__mod_parser_make_string);
    GemVal _t1074[] = {gem_string("=="), _t1071.fn(_t1071.env, _t1070, 3), _t1073.fn(_t1073.env, _t1072, 1)};
    GemVal _t1075 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1066, gem_int(0), _t1075.fn(_t1075.env, _t1074, 3));
        GemVal gem_v_conditions = _t1066;
#line 612 "compiler/main.gem"
    GemVal _t1076[] = {gem_string("len")};
    GemVal _t1077 = (*gem_v__mod_parser_make_var);
    GemVal _t1078 = gem_table_new();
    gem_table_set(_t1078, gem_int(0), gem_v_target_expr);
    GemVal _t1079[] = {_t1077.fn(_t1077.env, _t1076, 1), _t1078, gem_int(0)};
    GemVal _t1080 = (*gem_v__mod_parser_make_call);
    GemVal _t1081[] = {gem_v_idx};
    GemVal _t1082 = (*gem_v__mod_parser_make_int);
    GemVal _t1083[] = {gem_string("=="), _t1080.fn(_t1080.env, _t1079, 3), _t1082.fn(_t1082.env, _t1081, 1)};
    GemVal _t1084 = (*gem_v__mod_parser_make_binop);
    GemVal _t1085[] = {gem_v_conditions, _t1084.fn(_t1084.env, _t1083, 3)};
        (void)(gem_push_fn(NULL, _t1085, 2));
#line 613 "compiler/main.gem"
    GemVal _t1086 = gem_table_new();
        GemVal gem_v_bindings = _t1086;
#line 614 "compiler/main.gem"
        GemVal gem_v__for_i_7 = gem_int(0);
#line 614 "compiler/main.gem"
    GemVal _t1087[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1087, 1);
#line 614 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 614 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_7;
#line 614 "compiler/main.gem"
            gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 615 "compiler/main.gem"
            GemVal gem_v_sub = gem_table_get(gem_v_sub_patterns, gem_v_i);
#line 616 "compiler/main.gem"
    GemVal _t1088 = gem_v_sub;
    GemVal _t1089 = gem_table_get(_t1088, gem_string("condition"));
    GemVal _t1092;
    if (gem_truthy(gem_neq(gem_table_get(_t1089, gem_string("tag")), gem_string("bool")))) {
        _t1092 = gem_neq(gem_table_get(_t1089, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t1090 = gem_v_sub;
        GemVal _t1091 = gem_table_get(_t1090, gem_string("condition"));
        _t1092 = gem_neq(gem_table_get(_t1091, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t1092)) {
#line 617 "compiler/main.gem"
    GemVal _t1093 = gem_v_sub;
    GemVal _t1094[] = {gem_v_conditions, gem_table_get(_t1093, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t1094, 2));
            }
#line 619 "compiler/main.gem"
    GemVal _t1095 = gem_v_sub;
            GemVal gem_v__for_items_6 = gem_table_get(_t1095, gem_string("bindings"));
#line 619 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 619 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1096[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1096, 1)))) break;
#line 619 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 619 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 620 "compiler/main.gem"
    GemVal _t1097[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t1097, 2));
            }

        }

#line 623 "compiler/main.gem"
    GemVal _t1098 = gem_table_new();
    GemVal _t1099[] = {gem_v_conditions};
    GemVal _t1100 = gem_v_and_chain;
    gem_table_set(_t1098, gem_string("condition"), _t1100.fn(_t1100.env, _t1099, 1));
    gem_table_set(_t1098, gem_string("bindings"), gem_v_bindings);
        GemVal _t1101 = _t1098;
        gem_pop_frame();
        return _t1101;
    }
#line 627 "compiler/main.gem"
    GemVal _t1102 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1102, gem_string("type")), gem_string("NUMBER")))) {
#line 628 "compiler/main.gem"
    GemVal _t1103 = (*gem_v_advance);
        (void)(_t1103.fn(_t1103.env, NULL, 0));
#line 629 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 630 "compiler/main.gem"
    GemVal _t1104 = gem_v_t;
    GemVal _t1105[] = {gem_table_get(_t1104, gem_string("value"))};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t1105, 1))) {
#line 631 "compiler/main.gem"
    GemVal _t1106 = gem_v_t;
    GemVal _t1107[] = {gem_table_get(_t1106, gem_string("value"))};
    GemVal _t1108[] = {gem_fn_atof(NULL, _t1107, 1)};
    GemVal _t1109 = (*gem_v__mod_parser_make_float);
            gem_v_lit = _t1109.fn(_t1109.env, _t1108, 1);
        } else {
#line 633 "compiler/main.gem"
    GemVal _t1110 = gem_v_t;
    GemVal _t1111[] = {gem_table_get(_t1110, gem_string("value"))};
    GemVal _t1112[] = {gem_fn__mod_parser_str_to_int(NULL, _t1111, 1)};
    GemVal _t1113 = (*gem_v__mod_parser_make_int);
            gem_v_lit = _t1113.fn(_t1113.env, _t1112, 1);
        }
#line 635 "compiler/main.gem"
    GemVal _t1114 = gem_table_new();
    GemVal _t1115[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    GemVal _t1116 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1114, gem_string("condition"), _t1116.fn(_t1116.env, _t1115, 3));
    GemVal _t1117 = gem_table_new();
    gem_table_set(_t1114, gem_string("bindings"), _t1117);
        GemVal _t1118 = _t1114;
        gem_pop_frame();
        return _t1118;
    }
#line 637 "compiler/main.gem"
    GemVal _t1119 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1119, gem_string("type")), gem_string("STRING")))) {
#line 638 "compiler/main.gem"
    GemVal _t1120 = (*gem_v_advance);
        (void)(_t1120.fn(_t1120.env, NULL, 0));
#line 639 "compiler/main.gem"
    GemVal _t1121 = gem_table_new();
    GemVal _t1122 = gem_v_t;
    GemVal _t1123[] = {gem_table_get(_t1122, gem_string("value"))};
    GemVal _t1124 = (*gem_v__mod_parser_make_string);
    GemVal _t1125[] = {gem_string("=="), gem_v_target_expr, _t1124.fn(_t1124.env, _t1123, 1)};
    GemVal _t1126 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1121, gem_string("condition"), _t1126.fn(_t1126.env, _t1125, 3));
    GemVal _t1127 = gem_table_new();
    gem_table_set(_t1121, gem_string("bindings"), _t1127);
        GemVal _t1128 = _t1121;
        gem_pop_frame();
        return _t1128;
    }
#line 641 "compiler/main.gem"
    GemVal _t1129 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1129, gem_string("type")), gem_string("true")))) {
#line 642 "compiler/main.gem"
    GemVal _t1130 = (*gem_v_advance);
        (void)(_t1130.fn(_t1130.env, NULL, 0));
#line 643 "compiler/main.gem"
    GemVal _t1131 = gem_table_new();
    GemVal _t1132[] = {gem_bool(1)};
    GemVal _t1133 = (*gem_v__mod_parser_make_bool);
    GemVal _t1134[] = {gem_string("=="), gem_v_target_expr, _t1133.fn(_t1133.env, _t1132, 1)};
    GemVal _t1135 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1131, gem_string("condition"), _t1135.fn(_t1135.env, _t1134, 3));
    GemVal _t1136 = gem_table_new();
    gem_table_set(_t1131, gem_string("bindings"), _t1136);
        GemVal _t1137 = _t1131;
        gem_pop_frame();
        return _t1137;
    }
#line 645 "compiler/main.gem"
    GemVal _t1138 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1138, gem_string("type")), gem_string("false")))) {
#line 646 "compiler/main.gem"
    GemVal _t1139 = (*gem_v_advance);
        (void)(_t1139.fn(_t1139.env, NULL, 0));
#line 647 "compiler/main.gem"
    GemVal _t1140 = gem_table_new();
    GemVal _t1141[] = {gem_bool(0)};
    GemVal _t1142 = (*gem_v__mod_parser_make_bool);
    GemVal _t1143[] = {gem_string("=="), gem_v_target_expr, _t1142.fn(_t1142.env, _t1141, 1)};
    GemVal _t1144 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1140, gem_string("condition"), _t1144.fn(_t1144.env, _t1143, 3));
    GemVal _t1145 = gem_table_new();
    gem_table_set(_t1140, gem_string("bindings"), _t1145);
        GemVal _t1146 = _t1140;
        gem_pop_frame();
        return _t1146;
    }
#line 649 "compiler/main.gem"
    GemVal _t1147 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1147, gem_string("type")), gem_string("nil")))) {
#line 650 "compiler/main.gem"
    GemVal _t1148 = (*gem_v_advance);
        (void)(_t1148.fn(_t1148.env, NULL, 0));
#line 651 "compiler/main.gem"
    GemVal _t1149 = gem_table_new();
    GemVal _t1150 = (*gem_v__mod_parser_make_nil_node);
    GemVal _t1151[] = {gem_string("=="), gem_v_target_expr, _t1150.fn(_t1150.env, NULL, 0)};
    GemVal _t1152 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1149, gem_string("condition"), _t1152.fn(_t1152.env, _t1151, 3));
    GemVal _t1153 = gem_table_new();
    gem_table_set(_t1149, gem_string("bindings"), _t1153);
        GemVal _t1154 = _t1149;
        gem_pop_frame();
        return _t1154;
    }
#line 655 "compiler/main.gem"
    GemVal _t1155 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1155, gem_string("type")), gem_string("NAME")))) {
#line 656 "compiler/main.gem"
    GemVal _t1156 = (*gem_v_advance);
        (void)(_t1156.fn(_t1156.env, NULL, 0));
#line 657 "compiler/main.gem"
    GemVal _t1157 = gem_table_new();
    GemVal _t1158[] = {gem_bool(1)};
    GemVal _t1159 = (*gem_v__mod_parser_make_bool);
    gem_table_set(_t1157, gem_string("condition"), _t1159.fn(_t1159.env, _t1158, 1));
    GemVal _t1160 = gem_table_new();
    GemVal _t1161 = gem_v_t;
    GemVal _t1162[] = {gem_table_get(_t1161, gem_string("value")), gem_v_target_expr, gem_int(0)};
    GemVal _t1163 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1160, gem_int(0), _t1163.fn(_t1163.env, _t1162, 3));
    gem_table_set(_t1157, gem_string("bindings"), _t1160);
        GemVal _t1164 = _t1157;
        gem_pop_frame();
        return _t1164;
    }
#line 660 "compiler/main.gem"
    GemVal _t1165 = gem_v_t;
    GemVal _t1166 = gem_v_t;
    GemVal _t1167 = gem_v_t;
    GemVal _t1168[] = {gem_table_get(_t1167, gem_string("value"))};
    GemVal _t1169 = gem_v_t;
    GemVal _t1170[] = {gem_table_get(_t1169, gem_string("type"))};
    GemVal _t1171[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1165, gem_string("line")), gem_table_get(_t1166, gem_string("col")), gem_len_fn(NULL, _t1168, 1), gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t1170, 1)), gem_string("'")), GEM_NIL};
    GemVal _t1172 = (*gem_v__mod_parser_compile_error);
    GemVal _t1173 = _t1172.fn(_t1172.env, _t1171, 7);
    gem_pop_frame();
    return _t1173;
}

struct _closure__anon_22 {
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_assign;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_break;
    GemVal *gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_continue;
    GemVal *gem_v__mod_parser_make_dot;
    GemVal *gem_v__mod_parser_make_export;
    GemVal *gem_v__mod_parser_make_extern_fn;
    GemVal *gem_v__mod_parser_make_extern_include;
    GemVal *gem_v__mod_parser_make_extern_param;
    GemVal *gem_v__mod_parser_make_fn_def;
    GemVal *gem_v__mod_parser_make_if;
    GemVal *gem_v__mod_parser_make_index;
    GemVal *gem_v__mod_parser_make_int;
    GemVal *gem_v__mod_parser_make_let;
    GemVal *gem_v__mod_parser_make_match;
    GemVal *gem_v__mod_parser_make_receive_match;
    GemVal *gem_v__mod_parser_make_return;
    GemVal *gem_v__mod_parser_make_var;
    GemVal *gem_v__mod_parser_make_when;
    GemVal *gem_v__mod_parser_make_while;
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
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v__mod_parser_make_assign = _cls->gem_v__mod_parser_make_assign;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_break = _cls->gem_v__mod_parser_make_break;
    GemVal *gem_v__mod_parser_make_call = _cls->gem_v__mod_parser_make_call;
    GemVal *gem_v__mod_parser_make_continue = _cls->gem_v__mod_parser_make_continue;
    GemVal *gem_v__mod_parser_make_dot = _cls->gem_v__mod_parser_make_dot;
    GemVal *gem_v__mod_parser_make_export = _cls->gem_v__mod_parser_make_export;
    GemVal *gem_v__mod_parser_make_extern_fn = _cls->gem_v__mod_parser_make_extern_fn;
    GemVal *gem_v__mod_parser_make_extern_include = _cls->gem_v__mod_parser_make_extern_include;
    GemVal *gem_v__mod_parser_make_extern_param = _cls->gem_v__mod_parser_make_extern_param;
    GemVal *gem_v__mod_parser_make_fn_def = _cls->gem_v__mod_parser_make_fn_def;
    GemVal *gem_v__mod_parser_make_if = _cls->gem_v__mod_parser_make_if;
    GemVal *gem_v__mod_parser_make_index = _cls->gem_v__mod_parser_make_index;
    GemVal *gem_v__mod_parser_make_int = _cls->gem_v__mod_parser_make_int;
    GemVal *gem_v__mod_parser_make_let = _cls->gem_v__mod_parser_make_let;
    GemVal *gem_v__mod_parser_make_match = _cls->gem_v__mod_parser_make_match;
    GemVal *gem_v__mod_parser_make_receive_match = _cls->gem_v__mod_parser_make_receive_match;
    GemVal *gem_v__mod_parser_make_return = _cls->gem_v__mod_parser_make_return;
    GemVal *gem_v__mod_parser_make_var = _cls->gem_v__mod_parser_make_var;
    GemVal *gem_v__mod_parser_make_when = _cls->gem_v__mod_parser_make_when;
    GemVal *gem_v__mod_parser_make_while = _cls->gem_v__mod_parser_make_while;
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
#line 666 "compiler/main.gem"
    GemVal _t1175 = (*gem_v_peek);
    GemVal gem_v_t = _t1175.fn(_t1175.env, NULL, 0);
#line 669 "compiler/main.gem"
    GemVal _t1176 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1176, gem_string("type")), gem_string("let")))) {
#line 670 "compiler/main.gem"
    GemVal _t1177 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1177, gem_string("line"));
#line 671 "compiler/main.gem"
    GemVal _t1178 = (*gem_v_advance);
        (void)(_t1178.fn(_t1178.env, NULL, 0));
#line 674 "compiler/main.gem"
    GemVal _t1179 = (*gem_v_peek);
    GemVal _t1180 = _t1179.fn(_t1179.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1180, gem_string("type")), gem_string("{")))) {
#line 675 "compiler/main.gem"
    GemVal _t1181 = (*gem_v_advance);
            (void)(_t1181.fn(_t1181.env, NULL, 0));
#line 676 "compiler/main.gem"
    GemVal _t1182 = gem_table_new();
            GemVal gem_v_names = _t1182;
#line 677 "compiler/main.gem"
    GemVal _t1183 = (*gem_v_skip_nl);
            (void)(_t1183.fn(_t1183.env, NULL, 0));
#line 678 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1184 = (*gem_v_peek);
                GemVal _t1185 = _t1184.fn(_t1184.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1185, gem_string("type")), gem_string("}")))) break;
#line 679 "compiler/main.gem"
    GemVal _t1186 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1186.fn(_t1186.env, NULL, 0);
#line 680 "compiler/main.gem"
    GemVal _t1187 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t1187, gem_string("type")), gem_string("NAME")))) {
#line 681 "compiler/main.gem"
    GemVal _t1188 = (*gem_v_advance);
                    (void)(_t1188.fn(_t1188.env, NULL, 0));
                } else {
#line 684 "compiler/main.gem"
    GemVal _t1189 = (*gem_v_advance);
                    (void)(_t1189.fn(_t1189.env, NULL, 0));
                }
#line 686 "compiler/main.gem"
    GemVal _t1190 = gem_v_field_tok;
    GemVal _t1191[] = {gem_v_names, gem_table_get(_t1190, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1191, 2));
#line 687 "compiler/main.gem"
    GemVal _t1192 = (*gem_v_skip_nl);
                (void)(_t1192.fn(_t1192.env, NULL, 0));
#line 688 "compiler/main.gem"
    GemVal _t1193 = (*gem_v_peek);
    GemVal _t1194 = _t1193.fn(_t1193.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1194, gem_string("type")), gem_string(",")))) {
#line 689 "compiler/main.gem"
    GemVal _t1195 = (*gem_v_advance);
                    (void)(_t1195.fn(_t1195.env, NULL, 0));
#line 690 "compiler/main.gem"
    GemVal _t1196 = (*gem_v_skip_nl);
                    (void)(_t1196.fn(_t1196.env, NULL, 0));
                }
            }
#line 693 "compiler/main.gem"
    GemVal _t1197[] = {gem_string("}")};
    GemVal _t1198 = (*gem_v_expect);
            (void)(_t1198.fn(_t1198.env, _t1197, 1));
#line 694 "compiler/main.gem"
    GemVal _t1199[] = {gem_string("=")};
    GemVal _t1200 = (*gem_v_expect);
            (void)(_t1200.fn(_t1200.env, _t1199, 1));
#line 695 "compiler/main.gem"
    GemVal _t1201 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1201.fn(_t1201.env, NULL, 0);
#line 696 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 697 "compiler/main.gem"
    GemVal _t1202[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1202, 1));
#line 698 "compiler/main.gem"
    GemVal _t1203 = gem_table_new();
    GemVal _t1204[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1205 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1203, gem_int(0), _t1205.fn(_t1205.env, _t1204, 3));
            GemVal gem_v_stmts = _t1203;
#line 699 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 699 "compiler/main.gem"
    GemVal _t1206[] = {gem_v_names};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1206, 1);
#line 699 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 699 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_8;
#line 699 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 700 "compiler/main.gem"
    GemVal _t1207[] = {gem_v_tmp};
    GemVal _t1208 = (*gem_v__mod_parser_make_var);
    GemVal _t1209[] = {_t1208.fn(_t1208.env, _t1207, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1210 = (*gem_v__mod_parser_make_dot);
    GemVal _t1211[] = {gem_table_get(gem_v_names, gem_v_di), _t1210.fn(_t1210.env, _t1209, 2), gem_v_line};
    GemVal _t1212 = (*gem_v__mod_parser_make_let);
    GemVal _t1213[] = {gem_v_stmts, _t1212.fn(_t1212.env, _t1211, 3)};
                (void)(gem_push_fn(NULL, _t1213, 2));
            }

#line 702 "compiler/main.gem"
    GemVal _t1214 = gem_table_new();
    gem_table_set(_t1214, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1214, gem_string("stmts"), gem_v_stmts);
            GemVal _t1215 = _t1214;
            gem_pop_frame();
            return _t1215;
        }
#line 706 "compiler/main.gem"
    GemVal _t1216 = (*gem_v_peek);
    GemVal _t1217 = _t1216.fn(_t1216.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1217, gem_string("type")), gem_string("[")))) {
#line 707 "compiler/main.gem"
    GemVal _t1218 = (*gem_v_advance);
            (void)(_t1218.fn(_t1218.env, NULL, 0));
#line 708 "compiler/main.gem"
    GemVal _t1219 = gem_table_new();
            GemVal gem_v_names = _t1219;
#line 709 "compiler/main.gem"
    GemVal _t1220 = (*gem_v_skip_nl);
            (void)(_t1220.fn(_t1220.env, NULL, 0));
#line 710 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1221 = (*gem_v_peek);
                GemVal _t1222 = _t1221.fn(_t1221.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1222, gem_string("type")), gem_string("]")))) break;
#line 711 "compiler/main.gem"
    GemVal _t1223[] = {gem_string("NAME")};
    GemVal _t1224 = (*gem_v_expect);
    GemVal _t1225 = _t1224.fn(_t1224.env, _t1223, 1);
    GemVal _t1226[] = {gem_v_names, gem_table_get(_t1225, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1226, 2));
#line 712 "compiler/main.gem"
    GemVal _t1227 = (*gem_v_skip_nl);
                (void)(_t1227.fn(_t1227.env, NULL, 0));
#line 713 "compiler/main.gem"
    GemVal _t1228 = (*gem_v_peek);
    GemVal _t1229 = _t1228.fn(_t1228.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1229, gem_string("type")), gem_string(",")))) {
#line 714 "compiler/main.gem"
    GemVal _t1230 = (*gem_v_advance);
                    (void)(_t1230.fn(_t1230.env, NULL, 0));
#line 715 "compiler/main.gem"
    GemVal _t1231 = (*gem_v_skip_nl);
                    (void)(_t1231.fn(_t1231.env, NULL, 0));
                }
            }
#line 718 "compiler/main.gem"
    GemVal _t1232[] = {gem_string("]")};
    GemVal _t1233 = (*gem_v_expect);
            (void)(_t1233.fn(_t1233.env, _t1232, 1));
#line 719 "compiler/main.gem"
    GemVal _t1234[] = {gem_string("=")};
    GemVal _t1235 = (*gem_v_expect);
            (void)(_t1235.fn(_t1235.env, _t1234, 1));
#line 720 "compiler/main.gem"
    GemVal _t1236 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1236.fn(_t1236.env, NULL, 0);
#line 721 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 722 "compiler/main.gem"
    GemVal _t1237[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1237, 1));
#line 723 "compiler/main.gem"
    GemVal _t1238 = gem_table_new();
    GemVal _t1239[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1240 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1238, gem_int(0), _t1240.fn(_t1240.env, _t1239, 3));
            GemVal gem_v_stmts = _t1238;
#line 724 "compiler/main.gem"
            GemVal gem_v__for_i_9 = gem_int(0);
#line 724 "compiler/main.gem"
    GemVal _t1241[] = {gem_v_names};
            GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1241, 1);
#line 724 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 724 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_9;
#line 724 "compiler/main.gem"
                gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 725 "compiler/main.gem"
    GemVal _t1242[] = {gem_v_tmp};
    GemVal _t1243 = (*gem_v__mod_parser_make_var);
    GemVal _t1244[] = {gem_v_di};
    GemVal _t1245 = (*gem_v__mod_parser_make_int);
    GemVal _t1246[] = {_t1243.fn(_t1243.env, _t1242, 1), _t1245.fn(_t1245.env, _t1244, 1)};
    GemVal _t1247 = (*gem_v__mod_parser_make_index);
    GemVal _t1248[] = {gem_table_get(gem_v_names, gem_v_di), _t1247.fn(_t1247.env, _t1246, 2), gem_v_line};
    GemVal _t1249 = (*gem_v__mod_parser_make_let);
    GemVal _t1250[] = {gem_v_stmts, _t1249.fn(_t1249.env, _t1248, 3)};
                (void)(gem_push_fn(NULL, _t1250, 2));
            }

#line 727 "compiler/main.gem"
    GemVal _t1251 = gem_table_new();
    gem_table_set(_t1251, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1251, gem_string("stmts"), gem_v_stmts);
            GemVal _t1252 = _t1251;
            gem_pop_frame();
            return _t1252;
        }
#line 730 "compiler/main.gem"
    GemVal _t1253[] = {gem_string("NAME")};
    GemVal _t1254 = (*gem_v_expect);
    GemVal _t1255 = _t1254.fn(_t1254.env, _t1253, 1);
        GemVal gem_v_name = gem_table_get(_t1255, gem_string("value"));
#line 731 "compiler/main.gem"
    GemVal _t1256[] = {gem_string("=")};
    GemVal _t1257 = (*gem_v_expect);
        (void)(_t1257.fn(_t1257.env, _t1256, 1));
#line 732 "compiler/main.gem"
    GemVal _t1258 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1258.fn(_t1258.env, NULL, 0);
#line 733 "compiler/main.gem"
    GemVal _t1259[] = {gem_v_name, gem_v_value, gem_v_line};
    GemVal _t1260 = (*gem_v__mod_parser_make_let);
        GemVal _t1261 = _t1260.fn(_t1260.env, _t1259, 3);
        gem_pop_frame();
        return _t1261;
    }
#line 737 "compiler/main.gem"
    GemVal _t1262 = gem_v_t;
    GemVal _t1266;
    if (!gem_truthy(gem_eq(gem_table_get(_t1262, gem_string("type")), gem_string("fn")))) {
        _t1266 = gem_eq(gem_table_get(_t1262, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1263[] = {gem_int(1)};
        GemVal _t1264 = (*gem_v_peek_at);
        GemVal _t1265 = _t1264.fn(_t1264.env, _t1263, 1);
        _t1266 = gem_eq(gem_table_get(_t1265, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1266)) {
#line 738 "compiler/main.gem"
    GemVal _t1267 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1267, gem_string("line"));
#line 739 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_advance);
        (void)(_t1268.fn(_t1268.env, NULL, 0));
#line 740 "compiler/main.gem"
    GemVal _t1269[] = {gem_string("NAME")};
    GemVal _t1270 = (*gem_v_expect);
    GemVal _t1271 = _t1270.fn(_t1270.env, _t1269, 1);
        GemVal gem_v_name = gem_table_get(_t1271, gem_string("value"));
#line 741 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 742 "compiler/main.gem"
    GemVal _t1272[] = {gem_v_name};
    GemVal _t1273[] = {gem_v_name};
    GemVal _t1274[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1272, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_add(gem_add(gem_string("use: let "), gem_to_string_fn(NULL, _t1273, 1)), gem_string(" = fn(...) ... end"))};
    GemVal _t1275 = (*gem_v__mod_parser_compile_error);
            (void)(_t1275.fn(_t1275.env, _t1274, 7));
        }
#line 744 "compiler/main.gem"
    GemVal _t1276[] = {gem_string("(")};
    GemVal _t1277 = (*gem_v_expect);
        (void)(_t1277.fn(_t1277.env, _t1276, 1));
#line 745 "compiler/main.gem"
    GemVal _t1278 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1278.fn(_t1278.env, NULL, 0);
#line 746 "compiler/main.gem"
    GemVal _t1279[] = {gem_string(")")};
    GemVal _t1280 = (*gem_v_expect);
        (void)(_t1280.fn(_t1280.env, _t1279, 1));
#line 747 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 748 "compiler/main.gem"
    GemVal _t1281 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1281.fn(_t1281.env, NULL, 0);
#line 749 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 750 "compiler/main.gem"
    GemVal _t1282[] = {gem_string("end")};
    GemVal _t1283 = (*gem_v_expect);
        (void)(_t1283.fn(_t1283.env, _t1282, 1));
#line 751 "compiler/main.gem"
    GemVal _t1284 = gem_v_pr;
    GemVal _t1285 = gem_v_pr;
    GemVal _t1286 = gem_v_pr;
    GemVal _t1287[] = {gem_v_name, gem_table_get(_t1284, gem_string("params")), gem_table_get(_t1285, gem_string("rest_param")), gem_table_get(_t1286, gem_string("block_param")), gem_v_body, gem_v_line};
    GemVal _t1288 = (*gem_v__mod_parser_make_fn_def);
        GemVal _t1289 = _t1288.fn(_t1288.env, _t1287, 6);
        gem_pop_frame();
        return _t1289;
    }
#line 755 "compiler/main.gem"
    GemVal _t1290 = gem_v_t;
    GemVal _t1292;
    if (gem_truthy(gem_eq(gem_table_get(_t1290, gem_string("type")), gem_string("if")))) {
        _t1292 = gem_eq(gem_table_get(_t1290, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1291 = gem_v_t;
        _t1292 = gem_eq(gem_table_get(_t1291, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1292)) {
#line 756 "compiler/main.gem"
    GemVal _t1293 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1293, gem_string("line"));
#line 757 "compiler/main.gem"
    GemVal _t1294 = (*gem_v_advance);
        (void)(_t1294.fn(_t1294.env, NULL, 0));
#line 758 "compiler/main.gem"
    GemVal _t1295 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1295.fn(_t1295.env, NULL, 0);
#line 759 "compiler/main.gem"
    GemVal _t1296 = (*gem_v_peek);
    GemVal _t1297 = _t1296.fn(_t1296.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1297, gem_string("type")), gem_string("then")))) {
#line 760 "compiler/main.gem"
    GemVal _t1298 = (*gem_v_advance);
            (void)(_t1298.fn(_t1298.env, NULL, 0));
        }
#line 762 "compiler/main.gem"
    GemVal _t1299 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1299.fn(_t1299.env, NULL, 0);
#line 763 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 764 "compiler/main.gem"
    GemVal _t1300 = (*gem_v_peek);
    GemVal _t1301 = _t1300.fn(_t1300.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1301, gem_string("type")), gem_string("elif")))) {
#line 766 "compiler/main.gem"
    GemVal _t1302 = gem_table_new();
    GemVal _t1303 = (*gem_v_parse_stmt);
    gem_table_set(_t1302, gem_int(0), _t1303.fn(_t1303.env, NULL, 0));
            gem_v_else_body = _t1302;
        } else {
#line 767 "compiler/main.gem"
    GemVal _t1304 = (*gem_v_peek);
    GemVal _t1305 = _t1304.fn(_t1304.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1305, gem_string("type")), gem_string("else")))) {
#line 768 "compiler/main.gem"
    GemVal _t1306 = (*gem_v_advance);
                (void)(_t1306.fn(_t1306.env, NULL, 0));
#line 769 "compiler/main.gem"
    GemVal _t1307 = (*gem_v_parse_body);
                gem_v_else_body = _t1307.fn(_t1307.env, NULL, 0);
            }
        }
#line 772 "compiler/main.gem"
    GemVal _t1308 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1308, gem_string("type")), gem_string("if")))) {
#line 773 "compiler/main.gem"
    GemVal _t1309[] = {gem_string("end")};
    GemVal _t1310 = (*gem_v_expect);
            (void)(_t1310.fn(_t1310.env, _t1309, 1));
        }
#line 775 "compiler/main.gem"
    GemVal _t1311[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
    GemVal _t1312 = (*gem_v__mod_parser_make_if);
        GemVal _t1313 = _t1312.fn(_t1312.env, _t1311, 4);
        gem_pop_frame();
        return _t1313;
    }
#line 779 "compiler/main.gem"
    GemVal _t1314 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1314, gem_string("type")), gem_string("while")))) {
#line 780 "compiler/main.gem"
    GemVal _t1315 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1315, gem_string("line"));
#line 781 "compiler/main.gem"
    GemVal _t1316 = (*gem_v_advance);
        (void)(_t1316.fn(_t1316.env, NULL, 0));
#line 782 "compiler/main.gem"
    GemVal _t1317 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1317.fn(_t1317.env, NULL, 0);
#line 783 "compiler/main.gem"
    GemVal _t1318 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1318.fn(_t1318.env, NULL, 0);
#line 784 "compiler/main.gem"
    GemVal _t1319[] = {gem_string("end")};
    GemVal _t1320 = (*gem_v_expect);
        (void)(_t1320.fn(_t1320.env, _t1319, 1));
#line 785 "compiler/main.gem"
    GemVal _t1321[] = {gem_v_cond, gem_v_wbody, gem_v_line};
    GemVal _t1322 = (*gem_v__mod_parser_make_while);
        GemVal _t1323 = _t1322.fn(_t1322.env, _t1321, 3);
        gem_pop_frame();
        return _t1323;
    }
#line 789 "compiler/main.gem"
    GemVal _t1324 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1324, gem_string("type")), gem_string("for")))) {
#line 790 "compiler/main.gem"
    GemVal _t1325 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1325, gem_string("line"));
#line 791 "compiler/main.gem"
    GemVal _t1326 = (*gem_v_advance);
        (void)(_t1326.fn(_t1326.env, NULL, 0));
#line 792 "compiler/main.gem"
    GemVal _t1327[] = {gem_string("NAME")};
    GemVal _t1328 = (*gem_v_expect);
    GemVal _t1329 = _t1328.fn(_t1328.env, _t1327, 1);
        GemVal gem_v_var_name = gem_table_get(_t1329, gem_string("value"));
#line 794 "compiler/main.gem"
    GemVal _t1330 = (*gem_v_peek);
    GemVal _t1331 = _t1330.fn(_t1330.env, NULL, 0);
    GemVal _t1335;
    if (!gem_truthy(gem_eq(gem_table_get(_t1331, gem_string("type")), gem_string(",")))) {
        _t1335 = gem_eq(gem_table_get(_t1331, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1332[] = {gem_int(1)};
        GemVal _t1333 = (*gem_v_peek_at);
        GemVal _t1334 = _t1333.fn(_t1333.env, _t1332, 1);
        _t1335 = gem_eq(gem_table_get(_t1334, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1335)) {
#line 796 "compiler/main.gem"
    GemVal _t1336 = (*gem_v_advance);
            (void)(_t1336.fn(_t1336.env, NULL, 0));
#line 797 "compiler/main.gem"
    GemVal _t1337[] = {gem_string("NAME")};
    GemVal _t1338 = (*gem_v_expect);
    GemVal _t1339 = _t1338.fn(_t1338.env, _t1337, 1);
            GemVal gem_v_val_name = gem_table_get(_t1339, gem_string("value"));
#line 798 "compiler/main.gem"
    GemVal _t1340[] = {gem_string("in")};
    GemVal _t1341 = (*gem_v_expect);
            (void)(_t1341.fn(_t1341.env, _t1340, 1));
#line 799 "compiler/main.gem"
    GemVal _t1342 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1342.fn(_t1342.env, NULL, 0);
#line 800 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 801 "compiler/main.gem"
    GemVal _t1343[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1343, 1));
#line 802 "compiler/main.gem"
    GemVal _t1344[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_keys_var = gem_add(gem_string("_for_keys_"), gem_to_string_fn(NULL, _t1344, 1));
#line 803 "compiler/main.gem"
    GemVal _t1345[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1345, 1));
#line 804 "compiler/main.gem"
    GemVal _t1346 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1346.fn(_t1346.env, NULL, 0);
#line 805 "compiler/main.gem"
    GemVal _t1347[] = {gem_string("end")};
    GemVal _t1348 = (*gem_v_expect);
            (void)(_t1348.fn(_t1348.env, _t1347, 1));
#line 806 "compiler/main.gem"
    GemVal _t1349 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1349;
#line 807 "compiler/main.gem"
    GemVal _t1350[] = {gem_v_keys_var};
    GemVal _t1351 = (*gem_v__mod_parser_make_var);
    GemVal _t1352[] = {gem_v_idx_var};
    GemVal _t1353 = (*gem_v__mod_parser_make_var);
    GemVal _t1354[] = {_t1351.fn(_t1351.env, _t1350, 1), _t1353.fn(_t1353.env, _t1352, 1)};
    GemVal _t1355 = (*gem_v__mod_parser_make_index);
    GemVal _t1356[] = {gem_v_var_name, _t1355.fn(_t1355.env, _t1354, 2), gem_v_line};
    GemVal _t1357 = (*gem_v__mod_parser_make_let);
    GemVal _t1358[] = {gem_v_inner_stmts, _t1357.fn(_t1357.env, _t1356, 3)};
            (void)(gem_push_fn(NULL, _t1358, 2));
#line 808 "compiler/main.gem"
    GemVal _t1359[] = {gem_v_tbl_var};
    GemVal _t1360 = (*gem_v__mod_parser_make_var);
    GemVal _t1361[] = {gem_v_keys_var};
    GemVal _t1362 = (*gem_v__mod_parser_make_var);
    GemVal _t1363[] = {gem_v_idx_var};
    GemVal _t1364 = (*gem_v__mod_parser_make_var);
    GemVal _t1365[] = {_t1362.fn(_t1362.env, _t1361, 1), _t1364.fn(_t1364.env, _t1363, 1)};
    GemVal _t1366 = (*gem_v__mod_parser_make_index);
    GemVal _t1367[] = {_t1360.fn(_t1360.env, _t1359, 1), _t1366.fn(_t1366.env, _t1365, 2)};
    GemVal _t1368 = (*gem_v__mod_parser_make_index);
    GemVal _t1369[] = {gem_v_val_name, _t1368.fn(_t1368.env, _t1367, 2), gem_v_line};
    GemVal _t1370 = (*gem_v__mod_parser_make_let);
    GemVal _t1371[] = {gem_v_inner_stmts, _t1370.fn(_t1370.env, _t1369, 3)};
            (void)(gem_push_fn(NULL, _t1371, 2));
#line 809 "compiler/main.gem"
    GemVal _t1372[] = {gem_v_idx_var};
    GemVal _t1373 = (*gem_v__mod_parser_make_var);
    GemVal _t1374[] = {gem_int(1)};
    GemVal _t1375 = (*gem_v__mod_parser_make_int);
    GemVal _t1376[] = {gem_string("+"), _t1373.fn(_t1373.env, _t1372, 1), _t1375.fn(_t1375.env, _t1374, 1)};
    GemVal _t1377 = (*gem_v__mod_parser_make_binop);
    GemVal _t1378[] = {gem_v_idx_var, _t1377.fn(_t1377.env, _t1376, 3), gem_v_line};
    GemVal _t1379 = (*gem_v__mod_parser_make_assign);
    GemVal _t1380[] = {gem_v_inner_stmts, _t1379.fn(_t1379.env, _t1378, 3)};
            (void)(gem_push_fn(NULL, _t1380, 2));
#line 810 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 810 "compiler/main.gem"
    GemVal _t1381[] = {gem_v_fbody};
            GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1381, 1);
#line 810 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 810 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_10;
#line 810 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 811 "compiler/main.gem"
    GemVal _t1382[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1382, 2));
            }

#line 813 "compiler/main.gem"
    GemVal _t1383[] = {gem_v_idx_var};
    GemVal _t1384 = (*gem_v__mod_parser_make_var);
    GemVal _t1385[] = {gem_string("len")};
    GemVal _t1386 = (*gem_v__mod_parser_make_var);
    GemVal _t1387 = gem_table_new();
    GemVal _t1388[] = {gem_v_keys_var};
    GemVal _t1389 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1387, gem_int(0), _t1389.fn(_t1389.env, _t1388, 1));
    GemVal _t1390[] = {_t1386.fn(_t1386.env, _t1385, 1), _t1387, gem_int(0)};
    GemVal _t1391 = (*gem_v__mod_parser_make_call);
    GemVal _t1392[] = {gem_string("<"), _t1384.fn(_t1384.env, _t1383, 1), _t1391.fn(_t1391.env, _t1390, 3)};
    GemVal _t1393 = (*gem_v__mod_parser_make_binop);
    GemVal _t1394[] = {_t1393.fn(_t1393.env, _t1392, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1395 = (*gem_v__mod_parser_make_while);
            GemVal gem_v_while_node = _t1395.fn(_t1395.env, _t1394, 3);
#line 818 "compiler/main.gem"
    GemVal _t1396 = gem_table_new();
    gem_table_set(_t1396, gem_string("tag"), gem_string("block"));
    GemVal _t1397 = gem_table_new();
    GemVal _t1398[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    GemVal _t1399 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1397, gem_int(0), _t1399.fn(_t1399.env, _t1398, 3));
    GemVal _t1400[] = {gem_string("keys")};
    GemVal _t1401 = (*gem_v__mod_parser_make_var);
    GemVal _t1402 = gem_table_new();
    GemVal _t1403[] = {gem_v_tbl_var};
    GemVal _t1404 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1402, gem_int(0), _t1404.fn(_t1404.env, _t1403, 1));
    GemVal _t1405[] = {_t1401.fn(_t1401.env, _t1400, 1), _t1402, gem_int(0)};
    GemVal _t1406 = (*gem_v__mod_parser_make_call);
    GemVal _t1407[] = {gem_v_keys_var, _t1406.fn(_t1406.env, _t1405, 3), gem_v_line};
    GemVal _t1408 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1397, gem_int(1), _t1408.fn(_t1408.env, _t1407, 3));
    GemVal _t1409[] = {gem_int(0)};
    GemVal _t1410 = (*gem_v__mod_parser_make_int);
    GemVal _t1411[] = {gem_v_idx_var, _t1410.fn(_t1410.env, _t1409, 1), gem_v_line};
    GemVal _t1412 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1397, gem_int(2), _t1412.fn(_t1412.env, _t1411, 3));
    gem_table_set(_t1397, gem_int(3), gem_v_while_node);
    gem_table_set(_t1396, gem_string("stmts"), _t1397);
            GemVal _t1413 = _t1396;
            gem_pop_frame();
            return _t1413;
        } else {
#line 824 "compiler/main.gem"
    GemVal _t1414 = (*gem_v_peek);
    GemVal _t1415 = _t1414.fn(_t1414.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1415, gem_string("type")), gem_string("in")))) {
#line 826 "compiler/main.gem"
    GemVal _t1416 = (*gem_v_advance);
                (void)(_t1416.fn(_t1416.env, NULL, 0));
#line 827 "compiler/main.gem"
    GemVal _t1417 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1417.fn(_t1417.env, NULL, 0);
#line 828 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 829 "compiler/main.gem"
    GemVal _t1418[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1418, 1));
#line 830 "compiler/main.gem"
    GemVal _t1419[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1419, 1));
#line 831 "compiler/main.gem"
    GemVal _t1420 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1420.fn(_t1420.env, NULL, 0);
#line 832 "compiler/main.gem"
    GemVal _t1421[] = {gem_string("end")};
    GemVal _t1422 = (*gem_v_expect);
                (void)(_t1422.fn(_t1422.env, _t1421, 1));
#line 833 "compiler/main.gem"
    GemVal _t1423 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1423;
#line 834 "compiler/main.gem"
    GemVal _t1424[] = {gem_v_items_var};
    GemVal _t1425 = (*gem_v__mod_parser_make_var);
    GemVal _t1426[] = {gem_v_idx_var};
    GemVal _t1427 = (*gem_v__mod_parser_make_var);
    GemVal _t1428[] = {_t1425.fn(_t1425.env, _t1424, 1), _t1427.fn(_t1427.env, _t1426, 1)};
    GemVal _t1429 = (*gem_v__mod_parser_make_index);
    GemVal _t1430[] = {gem_v_var_name, _t1429.fn(_t1429.env, _t1428, 2), gem_v_line};
    GemVal _t1431 = (*gem_v__mod_parser_make_let);
    GemVal _t1432[] = {gem_v_inner_stmts, _t1431.fn(_t1431.env, _t1430, 3)};
                (void)(gem_push_fn(NULL, _t1432, 2));
#line 835 "compiler/main.gem"
    GemVal _t1433[] = {gem_v_idx_var};
    GemVal _t1434 = (*gem_v__mod_parser_make_var);
    GemVal _t1435[] = {gem_int(1)};
    GemVal _t1436 = (*gem_v__mod_parser_make_int);
    GemVal _t1437[] = {gem_string("+"), _t1434.fn(_t1434.env, _t1433, 1), _t1436.fn(_t1436.env, _t1435, 1)};
    GemVal _t1438 = (*gem_v__mod_parser_make_binop);
    GemVal _t1439[] = {gem_v_idx_var, _t1438.fn(_t1438.env, _t1437, 3), gem_v_line};
    GemVal _t1440 = (*gem_v__mod_parser_make_assign);
    GemVal _t1441[] = {gem_v_inner_stmts, _t1440.fn(_t1440.env, _t1439, 3)};
                (void)(gem_push_fn(NULL, _t1441, 2));
#line 836 "compiler/main.gem"
                GemVal gem_v__for_i_11 = gem_int(0);
#line 836 "compiler/main.gem"
    GemVal _t1442[] = {gem_v_fbody};
                GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1442, 1);
#line 836 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 836 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_11;
#line 836 "compiler/main.gem"
                    gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 837 "compiler/main.gem"
    GemVal _t1443[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1443, 2));
                }

#line 839 "compiler/main.gem"
    GemVal _t1444[] = {gem_v_idx_var};
    GemVal _t1445 = (*gem_v__mod_parser_make_var);
    GemVal _t1446[] = {gem_string("len")};
    GemVal _t1447 = (*gem_v__mod_parser_make_var);
    GemVal _t1448 = gem_table_new();
    GemVal _t1449[] = {gem_v_items_var};
    GemVal _t1450 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1448, gem_int(0), _t1450.fn(_t1450.env, _t1449, 1));
    GemVal _t1451[] = {_t1447.fn(_t1447.env, _t1446, 1), _t1448, gem_int(0)};
    GemVal _t1452 = (*gem_v__mod_parser_make_call);
    GemVal _t1453[] = {gem_string("<"), _t1445.fn(_t1445.env, _t1444, 1), _t1452.fn(_t1452.env, _t1451, 3)};
    GemVal _t1454 = (*gem_v__mod_parser_make_binop);
    GemVal _t1455[] = {_t1454.fn(_t1454.env, _t1453, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1456 = (*gem_v__mod_parser_make_while);
                GemVal gem_v_while_node = _t1456.fn(_t1456.env, _t1455, 3);
#line 844 "compiler/main.gem"
    GemVal _t1457 = gem_table_new();
    gem_table_set(_t1457, gem_string("tag"), gem_string("block"));
    GemVal _t1458 = gem_table_new();
    GemVal _t1459[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    GemVal _t1460 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1458, gem_int(0), _t1460.fn(_t1460.env, _t1459, 3));
    GemVal _t1461[] = {gem_int(0)};
    GemVal _t1462 = (*gem_v__mod_parser_make_int);
    GemVal _t1463[] = {gem_v_idx_var, _t1462.fn(_t1462.env, _t1461, 1), gem_v_line};
    GemVal _t1464 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1458, gem_int(1), _t1464.fn(_t1464.env, _t1463, 3));
    gem_table_set(_t1458, gem_int(2), gem_v_while_node);
    gem_table_set(_t1457, gem_string("stmts"), _t1458);
                GemVal _t1465 = _t1457;
                gem_pop_frame();
                return _t1465;
            } else {
#line 849 "compiler/main.gem"
    GemVal _t1466 = (*gem_v_peek);
    GemVal _t1467 = _t1466.fn(_t1466.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1467, gem_string("type")), gem_string("=")))) {
#line 851 "compiler/main.gem"
    GemVal _t1468 = (*gem_v_advance);
                    (void)(_t1468.fn(_t1468.env, NULL, 0));
#line 852 "compiler/main.gem"
    GemVal _t1469 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1469.fn(_t1469.env, NULL, 0);
#line 853 "compiler/main.gem"
    GemVal _t1470[] = {gem_string(",")};
    GemVal _t1471 = (*gem_v_expect);
                    (void)(_t1471.fn(_t1471.env, _t1470, 1));
#line 854 "compiler/main.gem"
    GemVal _t1472 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1472.fn(_t1472.env, NULL, 0);
#line 855 "compiler/main.gem"
    GemVal _t1473 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1473.fn(_t1473.env, NULL, 0);
#line 856 "compiler/main.gem"
    GemVal _t1474[] = {gem_string("end")};
    GemVal _t1475 = (*gem_v_expect);
                    (void)(_t1475.fn(_t1475.env, _t1474, 1));
#line 857 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 858 "compiler/main.gem"
    GemVal _t1476[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1476, 1));
#line 859 "compiler/main.gem"
    GemVal _t1477[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1477, 1));
#line 860 "compiler/main.gem"
    GemVal _t1478 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1478;
#line 861 "compiler/main.gem"
    GemVal _t1479[] = {gem_v_idx_var};
    GemVal _t1480 = (*gem_v__mod_parser_make_var);
    GemVal _t1481[] = {gem_v_var_name, _t1480.fn(_t1480.env, _t1479, 1), gem_v_line};
    GemVal _t1482 = (*gem_v__mod_parser_make_let);
    GemVal _t1483[] = {gem_v_inner_stmts, _t1482.fn(_t1482.env, _t1481, 3)};
                    (void)(gem_push_fn(NULL, _t1483, 2));
#line 862 "compiler/main.gem"
    GemVal _t1484[] = {gem_v_idx_var};
    GemVal _t1485 = (*gem_v__mod_parser_make_var);
    GemVal _t1486[] = {gem_int(1)};
    GemVal _t1487 = (*gem_v__mod_parser_make_int);
    GemVal _t1488[] = {gem_string("+"), _t1485.fn(_t1485.env, _t1484, 1), _t1487.fn(_t1487.env, _t1486, 1)};
    GemVal _t1489 = (*gem_v__mod_parser_make_binop);
    GemVal _t1490[] = {gem_v_idx_var, _t1489.fn(_t1489.env, _t1488, 3), gem_v_line};
    GemVal _t1491 = (*gem_v__mod_parser_make_assign);
    GemVal _t1492[] = {gem_v_inner_stmts, _t1491.fn(_t1491.env, _t1490, 3)};
                    (void)(gem_push_fn(NULL, _t1492, 2));
#line 863 "compiler/main.gem"
                    GemVal gem_v__for_i_12 = gem_int(0);
#line 863 "compiler/main.gem"
    GemVal _t1493[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1493, 1);
#line 863 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 863 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_12;
#line 863 "compiler/main.gem"
                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 864 "compiler/main.gem"
    GemVal _t1494[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1494, 2));
                    }

#line 866 "compiler/main.gem"
    GemVal _t1495[] = {gem_v_idx_var};
    GemVal _t1496 = (*gem_v__mod_parser_make_var);
    GemVal _t1497[] = {gem_v_limit_var};
    GemVal _t1498 = (*gem_v__mod_parser_make_var);
    GemVal _t1499[] = {gem_string("<"), _t1496.fn(_t1496.env, _t1495, 1), _t1498.fn(_t1498.env, _t1497, 1)};
    GemVal _t1500 = (*gem_v__mod_parser_make_binop);
    GemVal _t1501[] = {_t1500.fn(_t1500.env, _t1499, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1502 = (*gem_v__mod_parser_make_while);
                    GemVal gem_v_while_node = _t1502.fn(_t1502.env, _t1501, 3);
#line 871 "compiler/main.gem"
    GemVal _t1503 = gem_table_new();
    gem_table_set(_t1503, gem_string("tag"), gem_string("block"));
    GemVal _t1504 = gem_table_new();
    GemVal _t1505[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    GemVal _t1506 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1504, gem_int(0), _t1506.fn(_t1506.env, _t1505, 3));
    GemVal _t1507[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    GemVal _t1508 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1504, gem_int(1), _t1508.fn(_t1508.env, _t1507, 3));
    gem_table_set(_t1504, gem_int(2), gem_v_while_node);
    gem_table_set(_t1503, gem_string("stmts"), _t1504);
                    GemVal _t1509 = _t1503;
                    gem_pop_frame();
                    return _t1509;
                } else {
#line 877 "compiler/main.gem"
    GemVal _t1510 = (*gem_v_peek);
    GemVal _t1511 = _t1510.fn(_t1510.env, NULL, 0);
    GemVal _t1512 = (*gem_v_peek);
    GemVal _t1513 = _t1512.fn(_t1512.env, NULL, 0);
    GemVal _t1514 = (*gem_v_peek);
    GemVal _t1515 = _t1514.fn(_t1514.env, NULL, 0);
    GemVal _t1516[] = {gem_table_get(_t1515, gem_string("value"))};
    GemVal _t1517[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1511, gem_string("line")), gem_table_get(_t1513, gem_string("col")), gem_len_fn(NULL, _t1516, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
    GemVal _t1518 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1518.fn(_t1518.env, _t1517, 7));
                }
            }
        }
    }
#line 882 "compiler/main.gem"
    GemVal _t1519 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1519, gem_string("type")), gem_string("match")))) {
#line 883 "compiler/main.gem"
    GemVal _t1520 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1520, gem_string("line"));
#line 884 "compiler/main.gem"
    GemVal _t1521 = (*gem_v_advance);
        (void)(_t1521.fn(_t1521.env, NULL, 0));
#line 885 "compiler/main.gem"
    GemVal _t1522 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1522.fn(_t1522.env, NULL, 0);
#line 886 "compiler/main.gem"
    GemVal _t1523 = (*gem_v_skip_nl);
        (void)(_t1523.fn(_t1523.env, NULL, 0));
#line 887 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 888 "compiler/main.gem"
    GemVal _t1524[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1524, 1));
#line 889 "compiler/main.gem"
    GemVal _t1525[] = {gem_v_match_var};
    GemVal _t1526 = (*gem_v__mod_parser_make_var);
        GemVal gem_v_target_var_expr = _t1526.fn(_t1526.env, _t1525, 1);
#line 890 "compiler/main.gem"
    GemVal _t1527 = gem_table_new();
        GemVal gem_v_whens = _t1527;
#line 891 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1528 = (*gem_v_peek);
            GemVal _t1529 = _t1528.fn(_t1528.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1529, gem_string("type")), gem_string("when")))) break;
#line 892 "compiler/main.gem"
    GemVal _t1530 = (*gem_v_advance);
            (void)(_t1530.fn(_t1530.env, NULL, 0));
#line 894 "compiler/main.gem"
    GemVal _t1531 = (*gem_v_peek);
    GemVal _t1532 = _t1531.fn(_t1531.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1532, gem_string("type"));
#line 895 "compiler/main.gem"
    GemVal _t1533[] = {gem_int(1)};
    GemVal _t1534 = (*gem_v_peek_at);
    GemVal _t1535 = _t1534.fn(_t1534.env, _t1533, 1);
            GemVal gem_v_next = gem_table_get(_t1535, gem_string("type"));
#line 896 "compiler/main.gem"
    GemVal _t1536;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1536 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1536 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1536)) {
#line 897 "compiler/main.gem"
    GemVal _t1537[] = {gem_v_target_var_expr};
    GemVal _t1538 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1538.fn(_t1538.env, _t1537, 1);
#line 898 "compiler/main.gem"
    GemVal _t1539 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1539.fn(_t1539.env, NULL, 0);
#line 899 "compiler/main.gem"
    GemVal _t1540 = gem_v_pat;
    GemVal _t1541 = gem_v_pat;
    GemVal _t1542[] = {gem_table_get(_t1540, gem_string("condition")), gem_v_wbody, gem_table_get(_t1541, gem_string("bindings"))};
    GemVal _t1543 = (*gem_v__mod_parser_make_when);
    GemVal _t1544[] = {gem_v_whens, _t1543.fn(_t1543.env, _t1542, 3)};
                (void)(gem_push_fn(NULL, _t1544, 2));
            } else {
#line 900 "compiler/main.gem"
    GemVal _t1549;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1549 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1545;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1545 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1545 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1546;
        if (gem_truthy(_t1545)) {
                _t1546 = _t1545;
        } else {
                _t1546 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1547;
        if (gem_truthy(_t1546)) {
                _t1547 = _t1546;
        } else {
                _t1547 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1548;
        if (gem_truthy(_t1547)) {
                _t1548 = _t1547;
        } else {
                _t1548 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1549 = _t1548;
    }
                if (gem_truthy(_t1549)) {
#line 902 "compiler/main.gem"
    GemVal _t1550[] = {gem_v_target_var_expr};
    GemVal _t1551 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1551.fn(_t1551.env, _t1550, 1);
#line 903 "compiler/main.gem"
    GemVal _t1552 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1552.fn(_t1552.env, NULL, 0);
#line 904 "compiler/main.gem"
    GemVal _t1553 = gem_v_pat;
    GemVal _t1554 = gem_v_pat;
    GemVal _t1555[] = {gem_table_get(_t1553, gem_string("condition")), gem_v_wbody, gem_table_get(_t1554, gem_string("bindings"))};
    GemVal _t1556 = (*gem_v__mod_parser_make_when);
    GemVal _t1557[] = {gem_v_whens, _t1556.fn(_t1556.env, _t1555, 3)};
                    (void)(gem_push_fn(NULL, _t1557, 2));
                } else {
#line 907 "compiler/main.gem"
    GemVal _t1558 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1558.fn(_t1558.env, NULL, 0);
#line 908 "compiler/main.gem"
    GemVal _t1559 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1559.fn(_t1559.env, NULL, 0);
#line 909 "compiler/main.gem"
    GemVal _t1560[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1561 = (*gem_v__mod_parser_make_binop);
    GemVal _t1562 = gem_table_new();
    GemVal _t1563[] = {_t1561.fn(_t1561.env, _t1560, 3), gem_v_wbody, _t1562};
    GemVal _t1564 = (*gem_v__mod_parser_make_when);
    GemVal _t1565[] = {gem_v_whens, _t1564.fn(_t1564.env, _t1563, 3)};
                    (void)(gem_push_fn(NULL, _t1565, 2));
                }
            }
#line 911 "compiler/main.gem"
    GemVal _t1566 = (*gem_v_skip_nl);
            (void)(_t1566.fn(_t1566.env, NULL, 0));
        }
#line 913 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 914 "compiler/main.gem"
    GemVal _t1567 = (*gem_v_peek);
    GemVal _t1568 = _t1567.fn(_t1567.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1568, gem_string("type")), gem_string("else")))) {
#line 915 "compiler/main.gem"
    GemVal _t1569 = (*gem_v_advance);
            (void)(_t1569.fn(_t1569.env, NULL, 0));
#line 916 "compiler/main.gem"
    GemVal _t1570 = (*gem_v_parse_body);
            gem_v_else_body = _t1570.fn(_t1570.env, NULL, 0);
        }
#line 918 "compiler/main.gem"
    GemVal _t1571[] = {gem_string("end")};
    GemVal _t1572 = (*gem_v_expect);
        (void)(_t1572.fn(_t1572.env, _t1571, 1));
#line 919 "compiler/main.gem"
    GemVal _t1573[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
    GemVal _t1574 = (*gem_v__mod_parser_make_match);
        GemVal _t1575 = _t1574.fn(_t1574.env, _t1573, 5);
        gem_pop_frame();
        return _t1575;
    }
#line 923 "compiler/main.gem"
    GemVal _t1576 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1576, gem_string("type")), gem_string("return")))) {
#line 924 "compiler/main.gem"
    GemVal _t1577 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1577, gem_string("line"));
#line 925 "compiler/main.gem"
    GemVal _t1578 = (*gem_v_advance);
        (void)(_t1578.fn(_t1578.env, NULL, 0));
#line 926 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 927 "compiler/main.gem"
    GemVal _t1579 = (*gem_v_peek);
    GemVal _t1580 = _t1579.fn(_t1579.env, NULL, 0);
    GemVal _t1583;
    if (!gem_truthy(gem_neq(gem_table_get(_t1580, gem_string("type")), gem_string("NEWLINE")))) {
        _t1583 = gem_neq(gem_table_get(_t1580, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1581 = (*gem_v_peek);
        GemVal _t1582 = _t1581.fn(_t1581.env, NULL, 0);
        _t1583 = gem_neq(gem_table_get(_t1582, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1586;
    if (!gem_truthy(_t1583)) {
        _t1586 = _t1583;
    } else {
        GemVal _t1584 = (*gem_v_peek);
        GemVal _t1585 = _t1584.fn(_t1584.env, NULL, 0);
        _t1586 = gem_neq(gem_table_get(_t1585, gem_string("type")), gem_string("end"));
    }
    GemVal _t1589;
    if (!gem_truthy(_t1586)) {
        _t1589 = _t1586;
    } else {
        GemVal _t1587 = (*gem_v_peek);
        GemVal _t1588 = _t1587.fn(_t1587.env, NULL, 0);
        _t1589 = gem_neq(gem_table_get(_t1588, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1592;
    if (!gem_truthy(_t1589)) {
        _t1592 = _t1589;
    } else {
        GemVal _t1590 = (*gem_v_peek);
        GemVal _t1591 = _t1590.fn(_t1590.env, NULL, 0);
        _t1592 = gem_neq(gem_table_get(_t1591, gem_string("type")), gem_string("else"));
    }
    GemVal _t1595;
    if (!gem_truthy(_t1592)) {
        _t1595 = _t1592;
    } else {
        GemVal _t1593 = (*gem_v_peek);
        GemVal _t1594 = _t1593.fn(_t1593.env, NULL, 0);
        _t1595 = gem_neq(gem_table_get(_t1594, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1595)) {
#line 928 "compiler/main.gem"
    GemVal _t1596 = (*gem_v_parse_expr);
            gem_v_value = _t1596.fn(_t1596.env, NULL, 0);
        }
#line 930 "compiler/main.gem"
    GemVal _t1597[] = {gem_v_value, gem_v_line};
    GemVal _t1598 = (*gem_v__mod_parser_make_return);
        GemVal _t1599 = _t1598.fn(_t1598.env, _t1597, 2);
        gem_pop_frame();
        return _t1599;
    }
#line 934 "compiler/main.gem"
    GemVal _t1600 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1600, gem_string("type")), gem_string("break")))) {
#line 935 "compiler/main.gem"
    GemVal _t1601 = (*gem_v_advance);
        (void)(_t1601.fn(_t1601.env, NULL, 0));
#line 936 "compiler/main.gem"
    GemVal _t1602 = (*gem_v__mod_parser_make_break);
        GemVal _t1603 = _t1602.fn(_t1602.env, NULL, 0);
        gem_pop_frame();
        return _t1603;
    }
#line 940 "compiler/main.gem"
    GemVal _t1604 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1604, gem_string("type")), gem_string("continue")))) {
#line 941 "compiler/main.gem"
    GemVal _t1605 = (*gem_v_advance);
        (void)(_t1605.fn(_t1605.env, NULL, 0));
#line 942 "compiler/main.gem"
    GemVal _t1606 = (*gem_v__mod_parser_make_continue);
        GemVal _t1607 = _t1606.fn(_t1606.env, NULL, 0);
        gem_pop_frame();
        return _t1607;
    }
#line 946 "compiler/main.gem"
    GemVal _t1608 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1608, gem_string("type")), gem_string("load")))) {
#line 947 "compiler/main.gem"
    GemVal _t1609 = (*gem_v_advance);
        (void)(_t1609.fn(_t1609.env, NULL, 0));
#line 948 "compiler/main.gem"
    GemVal _t1610[] = {gem_string("STRING")};
    GemVal _t1611 = (*gem_v_expect);
    GemVal _t1612 = _t1611.fn(_t1611.env, _t1610, 1);
        GemVal gem_v_path = gem_table_get(_t1612, gem_string("value"));
#line 949 "compiler/main.gem"
        GemVal gem_v_alias_name = GEM_NIL;
#line 950 "compiler/main.gem"
        GemVal gem_v_selective = GEM_NIL;
#line 951 "compiler/main.gem"
    GemVal _t1613 = (*gem_v_peek);
    GemVal _t1614 = _t1613.fn(_t1613.env, NULL, 0);
    GemVal _t1617;
    if (!gem_truthy(gem_eq(gem_table_get(_t1614, gem_string("type")), gem_string("NAME")))) {
        _t1617 = gem_eq(gem_table_get(_t1614, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1615 = (*gem_v_peek);
        GemVal _t1616 = _t1615.fn(_t1615.env, NULL, 0);
        _t1617 = gem_eq(gem_table_get(_t1616, gem_string("value")), gem_string("as"));
    }
        if (gem_truthy(_t1617)) {
#line 952 "compiler/main.gem"
    GemVal _t1618 = (*gem_v_advance);
            (void)(_t1618.fn(_t1618.env, NULL, 0));
#line 953 "compiler/main.gem"
    GemVal _t1619[] = {gem_string("NAME")};
    GemVal _t1620 = (*gem_v_expect);
    GemVal _t1621 = _t1620.fn(_t1620.env, _t1619, 1);
            gem_v_alias_name = gem_table_get(_t1621, gem_string("value"));
        } else {
#line 954 "compiler/main.gem"
    GemVal _t1622 = (*gem_v_peek);
    GemVal _t1623 = _t1622.fn(_t1622.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1623, gem_string("type")), gem_string("(")))) {
#line 955 "compiler/main.gem"
    GemVal _t1624 = (*gem_v_advance);
                (void)(_t1624.fn(_t1624.env, NULL, 0));
#line 956 "compiler/main.gem"
    GemVal _t1625 = gem_table_new();
    GemVal _t1626[] = {gem_string("NAME")};
    GemVal _t1627 = (*gem_v_expect);
    GemVal _t1628 = _t1627.fn(_t1627.env, _t1626, 1);
    gem_table_set(_t1625, gem_int(0), gem_table_get(_t1628, gem_string("value")));
                gem_v_selective = _t1625;
#line 957 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1629 = (*gem_v_peek);
                    GemVal _t1630 = _t1629.fn(_t1629.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1630, gem_string("type")), gem_string(",")))) break;
#line 958 "compiler/main.gem"
    GemVal _t1631 = (*gem_v_advance);
                    (void)(_t1631.fn(_t1631.env, NULL, 0));
#line 959 "compiler/main.gem"
    GemVal _t1632[] = {gem_string("NAME")};
    GemVal _t1633 = (*gem_v_expect);
    GemVal _t1634 = _t1633.fn(_t1633.env, _t1632, 1);
    GemVal _t1635[] = {gem_v_selective, gem_table_get(_t1634, gem_string("value"))};
                    (void)(gem_push_fn(NULL, _t1635, 2));
                }
#line 961 "compiler/main.gem"
    GemVal _t1636[] = {gem_string(")")};
    GemVal _t1637 = (*gem_v_expect);
                (void)(_t1637.fn(_t1637.env, _t1636, 1));
            }
        }
#line 963 "compiler/main.gem"
    GemVal _t1638 = gem_table_new();
    gem_table_set(_t1638, gem_string("tag"), gem_string("load"));
    gem_table_set(_t1638, gem_string("path"), gem_v_path);
    gem_table_set(_t1638, gem_string("alias"), gem_v_alias_name);
    gem_table_set(_t1638, gem_string("selective"), gem_v_selective);
        GemVal _t1639 = _t1638;
        gem_pop_frame();
        return _t1639;
    }
#line 967 "compiler/main.gem"
    GemVal _t1640 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1640, gem_string("type")), gem_string("export")))) {
#line 968 "compiler/main.gem"
    GemVal _t1641 = (*gem_v_advance);
        (void)(_t1641.fn(_t1641.env, NULL, 0));
#line 969 "compiler/main.gem"
    GemVal _t1642 = gem_table_new();
    GemVal _t1643[] = {gem_string("NAME")};
    GemVal _t1644 = (*gem_v_expect);
    GemVal _t1645 = _t1644.fn(_t1644.env, _t1643, 1);
    gem_table_set(_t1642, gem_int(0), gem_table_get(_t1645, gem_string("value")));
        GemVal gem_v_names = _t1642;
#line 970 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1646 = (*gem_v_peek);
            GemVal _t1647 = _t1646.fn(_t1646.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1647, gem_string("type")), gem_string(",")))) break;
#line 971 "compiler/main.gem"
    GemVal _t1648 = (*gem_v_advance);
            (void)(_t1648.fn(_t1648.env, NULL, 0));
#line 972 "compiler/main.gem"
    GemVal _t1649[] = {gem_string("NAME")};
    GemVal _t1650 = (*gem_v_expect);
    GemVal _t1651 = _t1650.fn(_t1650.env, _t1649, 1);
    GemVal _t1652[] = {gem_v_names, gem_table_get(_t1651, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t1652, 2));
        }
#line 974 "compiler/main.gem"
    GemVal _t1653[] = {gem_v_names};
    GemVal _t1654 = (*gem_v__mod_parser_make_export);
        GemVal _t1655 = _t1654.fn(_t1654.env, _t1653, 1);
        gem_pop_frame();
        return _t1655;
    }
#line 978 "compiler/main.gem"
    GemVal _t1656 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1656, gem_string("type")), gem_string("extern")))) {
#line 979 "compiler/main.gem"
    GemVal _t1657 = (*gem_v_advance);
        (void)(_t1657.fn(_t1657.env, NULL, 0));
#line 980 "compiler/main.gem"
        GemVal gem_v_is_blocking = gem_bool(0);
#line 981 "compiler/main.gem"
    GemVal _t1658 = (*gem_v_peek);
    GemVal _t1659 = _t1658.fn(_t1658.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1659, gem_string("type")), gem_string("blocking")))) {
#line 982 "compiler/main.gem"
            gem_v_is_blocking = gem_bool(1);
#line 983 "compiler/main.gem"
    GemVal _t1660 = (*gem_v_advance);
            (void)(_t1660.fn(_t1660.env, NULL, 0));
        }
#line 985 "compiler/main.gem"
    GemVal _t1661 = (*gem_v_peek);
    GemVal _t1662 = _t1661.fn(_t1661.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1662, gem_string("type")), gem_string("fn")))) {
#line 986 "compiler/main.gem"
    GemVal _t1663 = (*gem_v_advance);
            (void)(_t1663.fn(_t1663.env, NULL, 0));
#line 987 "compiler/main.gem"
    GemVal _t1664[] = {gem_string("NAME")};
    GemVal _t1665 = (*gem_v_expect);
    GemVal _t1666 = _t1665.fn(_t1665.env, _t1664, 1);
            GemVal gem_v_name = gem_table_get(_t1666, gem_string("value"));
#line 988 "compiler/main.gem"
    GemVal _t1667[] = {gem_string("(")};
    GemVal _t1668 = (*gem_v_expect);
            (void)(_t1668.fn(_t1668.env, _t1667, 1));
#line 989 "compiler/main.gem"
    GemVal _t1669 = gem_table_new();
            GemVal gem_v_eparams = _t1669;
#line 990 "compiler/main.gem"
    GemVal _t1670 = (*gem_v_peek);
    GemVal _t1671 = _t1670.fn(_t1670.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1671, gem_string("type")), gem_string(")")))) {
#line 991 "compiler/main.gem"
    GemVal _t1672[] = {gem_string("NAME")};
    GemVal _t1673 = (*gem_v_expect);
    GemVal _t1674 = _t1673.fn(_t1673.env, _t1672, 1);
                GemVal gem_v_pname = gem_table_get(_t1674, gem_string("value"));
#line 992 "compiler/main.gem"
    GemVal _t1675[] = {gem_string(":")};
    GemVal _t1676 = (*gem_v_expect);
                (void)(_t1676.fn(_t1676.env, _t1675, 1));
#line 993 "compiler/main.gem"
    GemVal _t1677[] = {gem_string("NAME")};
    GemVal _t1678 = (*gem_v_expect);
    GemVal _t1679 = _t1678.fn(_t1678.env, _t1677, 1);
                GemVal gem_v_ptype = gem_table_get(_t1679, gem_string("value"));
#line 994 "compiler/main.gem"
    GemVal _t1680[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1681 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1682[] = {gem_v_eparams, _t1681.fn(_t1681.env, _t1680, 2)};
                (void)(gem_push_fn(NULL, _t1682, 2));
#line 995 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1683 = (*gem_v_peek);
                    GemVal _t1684 = _t1683.fn(_t1683.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1684, gem_string("type")), gem_string(",")))) break;
#line 996 "compiler/main.gem"
    GemVal _t1685 = (*gem_v_advance);
                    (void)(_t1685.fn(_t1685.env, NULL, 0));
#line 997 "compiler/main.gem"
    GemVal _t1686[] = {gem_string("NAME")};
    GemVal _t1687 = (*gem_v_expect);
    GemVal _t1688 = _t1687.fn(_t1687.env, _t1686, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1688, gem_string("value"));
#line 998 "compiler/main.gem"
    GemVal _t1689[] = {gem_string(":")};
    GemVal _t1690 = (*gem_v_expect);
                    (void)(_t1690.fn(_t1690.env, _t1689, 1));
#line 999 "compiler/main.gem"
    GemVal _t1691[] = {gem_string("NAME")};
    GemVal _t1692 = (*gem_v_expect);
    GemVal _t1693 = _t1692.fn(_t1692.env, _t1691, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1693, gem_string("value"));
#line 1000 "compiler/main.gem"
    GemVal _t1694[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1695 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1696[] = {gem_v_eparams, _t1695.fn(_t1695.env, _t1694, 2)};
                    (void)(gem_push_fn(NULL, _t1696, 2));
                }
            }
#line 1003 "compiler/main.gem"
    GemVal _t1697[] = {gem_string(")")};
    GemVal _t1698 = (*gem_v_expect);
            (void)(_t1698.fn(_t1698.env, _t1697, 1));
#line 1004 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 1005 "compiler/main.gem"
    GemVal _t1699 = (*gem_v_peek);
    GemVal _t1700 = _t1699.fn(_t1699.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1700, gem_string("type")), gem_string("->")))) {
#line 1006 "compiler/main.gem"
    GemVal _t1701 = (*gem_v_advance);
                (void)(_t1701.fn(_t1701.env, NULL, 0));
#line 1007 "compiler/main.gem"
    GemVal _t1702[] = {gem_string("NAME")};
    GemVal _t1703 = (*gem_v_expect);
    GemVal _t1704 = _t1703.fn(_t1703.env, _t1702, 1);
                gem_v_ret_type = gem_table_get(_t1704, gem_string("value"));
            }
#line 1009 "compiler/main.gem"
    GemVal _t1705[] = {gem_v_name, gem_v_eparams, gem_v_ret_type, gem_v_is_blocking};
    GemVal _t1706 = (*gem_v__mod_parser_make_extern_fn);
            GemVal _t1707 = _t1706.fn(_t1706.env, _t1705, 4);
            gem_pop_frame();
            return _t1707;
        } else {
#line 1010 "compiler/main.gem"
    GemVal _t1710;
    if (!gem_truthy(gem_not(gem_v_is_blocking))) {
        _t1710 = gem_not(gem_v_is_blocking);
    } else {
        GemVal _t1708 = (*gem_v_peek);
        GemVal _t1709 = _t1708.fn(_t1708.env, NULL, 0);
        _t1710 = gem_eq(gem_table_get(_t1709, gem_string("type")), gem_string("NAME"));
    }
    GemVal _t1713;
    if (!gem_truthy(_t1710)) {
        _t1713 = _t1710;
    } else {
        GemVal _t1711 = (*gem_v_peek);
        GemVal _t1712 = _t1711.fn(_t1711.env, NULL, 0);
        _t1713 = gem_eq(gem_table_get(_t1712, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1713)) {
#line 1011 "compiler/main.gem"
    GemVal _t1714 = (*gem_v_advance);
                (void)(_t1714.fn(_t1714.env, NULL, 0));
#line 1012 "compiler/main.gem"
    GemVal _t1715[] = {gem_string("STRING")};
    GemVal _t1716 = (*gem_v_expect);
    GemVal _t1717 = _t1716.fn(_t1716.env, _t1715, 1);
                GemVal gem_v_path = gem_table_get(_t1717, gem_string("value"));
#line 1013 "compiler/main.gem"
    GemVal _t1718[] = {gem_v_path};
    GemVal _t1719 = (*gem_v__mod_parser_make_extern_include);
                GemVal _t1720 = _t1719.fn(_t1719.env, _t1718, 1);
                gem_pop_frame();
                return _t1720;
            } else {
#line 1015 "compiler/main.gem"
    GemVal _t1721 = (*gem_v_peek);
    GemVal _t1722 = _t1721.fn(_t1721.env, NULL, 0);
    GemVal _t1723 = (*gem_v_peek);
    GemVal _t1724 = _t1723.fn(_t1723.env, NULL, 0);
    GemVal _t1725 = (*gem_v_peek);
    GemVal _t1726 = _t1725.fn(_t1725.env, NULL, 0);
    GemVal _t1727[] = {gem_table_get(_t1726, gem_string("value"))};
    GemVal _t1728[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1722, gem_string("line")), gem_table_get(_t1724, gem_string("col")), gem_len_fn(NULL, _t1727, 1), gem_string("expected 'fn' after 'extern blocking'"), GEM_NIL};
    GemVal _t1729 = (*gem_v__mod_parser_compile_error);
                (void)(_t1729.fn(_t1729.env, _t1728, 7));
            }
        }
    }
#line 1020 "compiler/main.gem"
    GemVal _t1730 = gem_v_t;
    GemVal _t1732;
    if (!gem_truthy(gem_eq(gem_table_get(_t1730, gem_string("type")), gem_string("NAME")))) {
        _t1732 = gem_eq(gem_table_get(_t1730, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1731 = gem_v_t;
        _t1732 = gem_eq(gem_table_get(_t1731, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1732)) {
#line 1021 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 1022 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1733[] = {gem_v_rla};
            GemVal _t1734 = (*gem_v_peek_at);
            GemVal _t1735 = _t1734.fn(_t1734.env, _t1733, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1735, gem_string("type")), gem_string("NEWLINE")))) break;
#line 1023 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 1025 "compiler/main.gem"
    GemVal _t1736[] = {gem_v_rla};
    GemVal _t1737 = (*gem_v_peek_at);
    GemVal _t1738 = _t1737.fn(_t1737.env, _t1736, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1738, gem_string("type")), gem_string("when")))) {
#line 1026 "compiler/main.gem"
    GemVal _t1739 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1739, gem_string("line"));
#line 1027 "compiler/main.gem"
    GemVal _t1740 = (*gem_v_advance);
            (void)(_t1740.fn(_t1740.env, NULL, 0));
#line 1028 "compiler/main.gem"
    GemVal _t1741 = (*gem_v_skip_nl);
            (void)(_t1741.fn(_t1741.env, NULL, 0));
#line 1029 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 1030 "compiler/main.gem"
    GemVal _t1742[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_add(gem_string("_recv_"), gem_to_string_fn(NULL, _t1742, 1));
#line 1031 "compiler/main.gem"
    GemVal _t1743[] = {gem_v_recv_var};
    GemVal _t1744 = (*gem_v__mod_parser_make_var);
            GemVal gem_v_recv_var_expr = _t1744.fn(_t1744.env, _t1743, 1);
#line 1032 "compiler/main.gem"
    GemVal _t1745 = gem_table_new();
            GemVal gem_v_rarms = _t1745;
#line 1033 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1746 = (*gem_v_peek);
                GemVal _t1747 = _t1746.fn(_t1746.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1747, gem_string("type")), gem_string("when")))) break;
#line 1034 "compiler/main.gem"
    GemVal _t1748 = (*gem_v_advance);
                (void)(_t1748.fn(_t1748.env, NULL, 0));
#line 1035 "compiler/main.gem"
    GemVal _t1749 = (*gem_v_peek);
    GemVal _t1750 = _t1749.fn(_t1749.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1750, gem_string("type"));
#line 1036 "compiler/main.gem"
    GemVal _t1751[] = {gem_int(1)};
    GemVal _t1752 = (*gem_v_peek_at);
    GemVal _t1753 = _t1752.fn(_t1752.env, _t1751, 1);
                GemVal gem_v_rnext = gem_table_get(_t1753, gem_string("type"));
#line 1037 "compiler/main.gem"
    GemVal _t1754;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1754 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1754 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1754)) {
#line 1038 "compiler/main.gem"
    GemVal _t1755[] = {gem_v_recv_var_expr};
    GemVal _t1756 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1756.fn(_t1756.env, _t1755, 1);
#line 1039 "compiler/main.gem"
    GemVal _t1757 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1757.fn(_t1757.env, NULL, 0);
#line 1040 "compiler/main.gem"
    GemVal _t1758 = gem_table_new();
    gem_table_set(_t1758, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1758, gem_string("body"), gem_v_rwbody);
    GemVal _t1759[] = {gem_v_rarms, _t1758};
                    (void)(gem_push_fn(NULL, _t1759, 2));
                } else {
#line 1041 "compiler/main.gem"
    GemVal _t1765;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1765 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1760;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1760 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1760 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1761;
        if (gem_truthy(_t1760)) {
                _t1761 = _t1760;
        } else {
                _t1761 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1762;
        if (gem_truthy(_t1761)) {
                _t1762 = _t1761;
        } else {
                _t1762 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1763;
        if (gem_truthy(_t1762)) {
                _t1763 = _t1762;
        } else {
                _t1763 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1764;
        if (gem_truthy(_t1763)) {
                _t1764 = _t1763;
        } else {
                _t1764 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1765 = _t1764;
    }
                    if (gem_truthy(_t1765)) {
#line 1042 "compiler/main.gem"
    GemVal _t1766[] = {gem_v_recv_var_expr};
    GemVal _t1767 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1767.fn(_t1767.env, _t1766, 1);
#line 1043 "compiler/main.gem"
    GemVal _t1768 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1768.fn(_t1768.env, NULL, 0);
#line 1044 "compiler/main.gem"
    GemVal _t1769 = gem_table_new();
    gem_table_set(_t1769, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1769, gem_string("body"), gem_v_rwbody);
    GemVal _t1770[] = {gem_v_rarms, _t1769};
                        (void)(gem_push_fn(NULL, _t1770, 2));
                    } else {
#line 1046 "compiler/main.gem"
    GemVal _t1771 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1771.fn(_t1771.env, NULL, 0);
#line 1047 "compiler/main.gem"
    GemVal _t1772 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1772.fn(_t1772.env, NULL, 0);
#line 1048 "compiler/main.gem"
    GemVal _t1773 = gem_table_new();
    GemVal _t1774 = gem_table_new();
    GemVal _t1775[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    GemVal _t1776 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1774, gem_string("condition"), _t1776.fn(_t1776.env, _t1775, 3));
    GemVal _t1777 = gem_table_new();
    gem_table_set(_t1774, gem_string("bindings"), _t1777);
    gem_table_set(_t1773, gem_string("pattern"), _t1774);
    gem_table_set(_t1773, gem_string("body"), gem_v_rwbody);
    GemVal _t1778[] = {gem_v_rarms, _t1773};
                        (void)(gem_push_fn(NULL, _t1778, 2));
                    }
                }
#line 1050 "compiler/main.gem"
    GemVal _t1779 = (*gem_v_skip_nl);
                (void)(_t1779.fn(_t1779.env, NULL, 0));
            }
#line 1052 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 1053 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 1054 "compiler/main.gem"
    GemVal _t1780 = (*gem_v_peek);
    GemVal _t1781 = _t1780.fn(_t1780.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1781, gem_string("type")), gem_string("after")))) {
#line 1055 "compiler/main.gem"
    GemVal _t1782 = (*gem_v_advance);
                (void)(_t1782.fn(_t1782.env, NULL, 0));
#line 1056 "compiler/main.gem"
    GemVal _t1783 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1783.fn(_t1783.env, NULL, 0);
#line 1057 "compiler/main.gem"
    GemVal _t1784 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1784.fn(_t1784.env, NULL, 0);
            }
#line 1059 "compiler/main.gem"
    GemVal _t1785[] = {gem_string("end")};
    GemVal _t1786 = (*gem_v_expect);
            (void)(_t1786.fn(_t1786.env, _t1785, 1));
#line 1060 "compiler/main.gem"
    GemVal _t1787[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
    GemVal _t1788 = (*gem_v__mod_parser_make_receive_match);
            GemVal _t1789 = _t1788.fn(_t1788.env, _t1787, 5);
            gem_pop_frame();
            return _t1789;
        }
    }
#line 1065 "compiler/main.gem"
    GemVal _t1790 = (*gem_v_parse_expr);
    GemVal _t1791 = _t1790.fn(_t1790.env, NULL, 0);
    gem_pop_frame();
    return _t1791;
}

struct _closure__anon_23 {
    GemVal *gem_v__mod_parser_make_program;
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
    GemVal *gem_v__mod_parser_make_program = _cls->gem_v__mod_parser_make_program;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 1074 "compiler/main.gem"
    GemVal _t1793 = gem_table_new();
    GemVal gem_v_stmts = _t1793;
#line 1075 "compiler/main.gem"
    GemVal _t1794 = (*gem_v_skip_nl);
    (void)(_t1794.fn(_t1794.env, NULL, 0));
#line 1076 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1795 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1795.fn(_t1795.env, NULL, 0)))) break;
#line 1077 "compiler/main.gem"
    GemVal _t1796 = (*gem_v_parse_stmt);
    GemVal _t1797[] = {gem_v_stmts, _t1796.fn(_t1796.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1797, 2));
#line 1078 "compiler/main.gem"
    GemVal _t1798 = (*gem_v_skip_nl);
        (void)(_t1798.fn(_t1798.env, NULL, 0));
    }
#line 1080 "compiler/main.gem"
    GemVal _t1799[] = {gem_v_stmts};
    GemVal _t1800 = (*gem_v__mod_parser_make_program);
    GemVal _t1801 = _t1800.fn(_t1800.env, _t1799, 1);
    gem_pop_frame();
    return _t1801;
}

static GemVal gem_fn__mod_parser_make_parser(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal *gem_v_tokens = GC_MALLOC(sizeof(GemVal));
    *gem_v_tokens = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_source = GC_MALLOC(sizeof(GemVal));
    *gem_v_source = (argc > 1) ? args[1] : GEM_NIL;
    GemVal *gem_v_file = GC_MALLOC(sizeof(GemVal));
    *gem_v_file = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_parser_make_parser", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal *gem_v_pos = GC_MALLOC(sizeof(GemVal));
    *gem_v_pos = gem_int(0);
#line 68 "compiler/main.gem"
    GemVal *gem_v_gensym_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_gensym_counter = gem_int(0);
#line 69 "compiler/main.gem"
    GemVal *gem_v_fn_depth = GC_MALLOC(sizeof(GemVal));
    *gem_v_fn_depth = gem_int(0);
#line 73 "compiler/main.gem"
    struct _closure__anon_1 *_t437 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t437->gem_v_pos = gem_v_pos;
    _t437->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_1, _t437);
#line 77 "compiler/main.gem"
    struct _closure__anon_2 *_t439 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t439->gem_v_pos = gem_v_pos;
    _t439->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_2, _t439);
#line 81 "compiler/main.gem"
    struct _closure__anon_3 *_t441 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t441->gem_v_pos = gem_v_pos;
    _t441->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_3, _t441);
#line 87 "compiler/main.gem"
    struct _closure__anon_4 *_t444 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t444->gem_v_pos = gem_v_pos;
    _t444->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_4, _t444);
#line 91 "compiler/main.gem"
    struct _closure__anon_5 *_t456 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t456->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t456->gem_v_file = gem_v_file;
    _t456->gem_v_pos = gem_v_pos;
    _t456->gem_v_source = gem_v_source;
    _t456->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t456);
#line 100 "compiler/main.gem"
    struct _closure__anon_6 *_t458 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t458->gem_v_pos = gem_v_pos;
    _t458->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t458);
#line 112 "compiler/main.gem"
    struct _closure__anon_7 *_t506 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t506->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t506->gem_v_advance = gem_v_advance;
    _t506->gem_v_expect = gem_v_expect;
    _t506->gem_v_file = gem_v_file;
    _t506->gem_v_peek = gem_v_peek;
    _t506->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t506);
#line 145 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 146 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 147 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 148 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 149 "compiler/main.gem"
    GemVal *gem_v_parse_pattern = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_pattern = GEM_NIL;
#line 155 "compiler/main.gem"
    struct _closure__anon_8 *_t532 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t532->gem_v_at_end = gem_v_at_end;
    _t532->gem_v_parse_stmt = gem_v_parse_stmt;
    _t532->gem_v_peek = gem_v_peek;
    _t532->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t532);
#line 168 "compiler/main.gem"
    struct _closure__anon_9 *_t543 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t543->gem_v_at_end = gem_v_at_end;
    _t543->gem_v_parse_stmt = gem_v_parse_stmt;
    _t543->gem_v_peek = gem_v_peek;
    _t543->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t543);
#line 181 "compiler/main.gem"
    struct _closure__anon_10 *_t716 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t716->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t716->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t716->gem_v__mod_parser_make_array = &gem_v__mod_parser_make_array;
    _t716->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t716->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t716->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t716->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t716->gem_v__mod_parser_make_interp = &gem_v__mod_parser_make_interp;
    _t716->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t716->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t716->gem_v__mod_parser_make_table = &gem_v__mod_parser_make_table;
    _t716->gem_v__mod_parser_make_table_entry = &gem_v__mod_parser_make_table_entry;
    _t716->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t716->gem_v_advance = gem_v_advance;
    _t716->gem_v_at_end = gem_v_at_end;
    _t716->gem_v_expect = gem_v_expect;
    _t716->gem_v_file = gem_v_file;
    _t716->gem_v_fn_depth = gem_v_fn_depth;
    _t716->gem_v_parse_expr = gem_v_parse_expr;
    _t716->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t716->gem_v_parse_params = gem_v_parse_params;
    _t716->gem_v_peek = gem_v_peek;
    _t716->gem_v_peek_at = gem_v_peek_at;
    _t716->gem_v_skip_nl = gem_v_skip_nl;
    _t716->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t716);
#line 319 "compiler/main.gem"
    struct _closure__anon_11 *_t812 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t812->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t812->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t812->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t812->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t812->gem_v_advance = gem_v_advance;
    _t812->gem_v_expect = gem_v_expect;
    _t812->gem_v_fn_depth = gem_v_fn_depth;
    _t812->gem_v_parse_atom = gem_v_parse_atom;
    _t812->gem_v_parse_expr = gem_v_parse_expr;
    _t812->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t812->gem_v_parse_params = gem_v_parse_params;
    _t812->gem_v_peek = gem_v_peek;
    _t812->gem_v_peek_at = gem_v_peek_at;
    _t812->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t812);
#line 425 "compiler/main.gem"
    struct _closure__anon_12 *_t822 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t822->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t822->gem_v_advance = gem_v_advance;
    _t822->gem_v_parse_call = gem_v_parse_call;
    _t822->gem_v_parse_unary = gem_v_parse_unary;
    _t822->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t822);
#line 434 "compiler/main.gem"
    struct _closure__anon_13 *_t838 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t838->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t838->gem_v_advance = gem_v_advance;
    _t838->gem_v_parse_unary = gem_v_parse_unary;
    _t838->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t838);
#line 445 "compiler/main.gem"
    struct _closure__anon_14 *_t851 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t851->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t851->gem_v_advance = gem_v_advance;
    _t851->gem_v_parse_mul = gem_v_parse_mul;
    _t851->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t851);
#line 456 "compiler/main.gem"
    struct _closure__anon_15 *_t879 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t879->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t879->gem_v_advance = gem_v_advance;
    _t879->gem_v_parse_add = gem_v_parse_add;
    _t879->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t879);
#line 467 "compiler/main.gem"
    struct _closure__anon_16 *_t889 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t889->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t889->gem_v_advance = gem_v_advance;
    _t889->gem_v_parse_compare = gem_v_parse_compare;
    _t889->gem_v_parse_not = gem_v_parse_not;
    _t889->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t889);
#line 476 "compiler/main.gem"
    struct _closure__anon_17 *_t898 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t898->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t898->gem_v_advance = gem_v_advance;
    _t898->gem_v_parse_not = gem_v_parse_not;
    _t898->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t898);
#line 487 "compiler/main.gem"
    struct _closure__anon_18 *_t907 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t907->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t907->gem_v_advance = gem_v_advance;
    _t907->gem_v_parse_and = gem_v_parse_and;
    _t907->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t907);
#line 498 "compiler/main.gem"
    struct _closure__anon_19 *_t965 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t965->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t965->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t965->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t965->gem_v__mod_parser_make_dot_assign = &gem_v__mod_parser_make_dot_assign;
    _t965->gem_v__mod_parser_make_index_assign = &gem_v__mod_parser_make_index_assign;
    _t965->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t965->gem_v_advance = gem_v_advance;
    _t965->gem_v_file = gem_v_file;
    _t965->gem_v_parse_expr = gem_v_parse_expr;
    _t965->gem_v_parse_or = gem_v_parse_or;
    _t965->gem_v_peek = gem_v_peek;
    _t965->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t965);
#line 533 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 537 "compiler/main.gem"
    struct _closure__anon_20 *_t1174 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1174->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1174->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1174->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t1174->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1174->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t1174->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1174->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1174->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1174->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t1174->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t1174->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1174->gem_v_advance = gem_v_advance;
    _t1174->gem_v_expect = gem_v_expect;
    _t1174->gem_v_file = gem_v_file;
    _t1174->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1174->gem_v_peek = gem_v_peek;
    _t1174->gem_v_peek_at = gem_v_peek_at;
    _t1174->gem_v_skip_nl = gem_v_skip_nl;
    _t1174->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t1174);
#line 665 "compiler/main.gem"
    struct _closure__anon_22 *_t1792 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1792->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1792->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t1792->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1792->gem_v__mod_parser_make_break = &gem_v__mod_parser_make_break;
    _t1792->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1792->gem_v__mod_parser_make_continue = &gem_v__mod_parser_make_continue;
    _t1792->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t1792->gem_v__mod_parser_make_export = &gem_v__mod_parser_make_export;
    _t1792->gem_v__mod_parser_make_extern_fn = &gem_v__mod_parser_make_extern_fn;
    _t1792->gem_v__mod_parser_make_extern_include = &gem_v__mod_parser_make_extern_include;
    _t1792->gem_v__mod_parser_make_extern_param = &gem_v__mod_parser_make_extern_param;
    _t1792->gem_v__mod_parser_make_fn_def = &gem_v__mod_parser_make_fn_def;
    _t1792->gem_v__mod_parser_make_if = &gem_v__mod_parser_make_if;
    _t1792->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1792->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1792->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1792->gem_v__mod_parser_make_match = &gem_v__mod_parser_make_match;
    _t1792->gem_v__mod_parser_make_receive_match = &gem_v__mod_parser_make_receive_match;
    _t1792->gem_v__mod_parser_make_return = &gem_v__mod_parser_make_return;
    _t1792->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1792->gem_v__mod_parser_make_when = &gem_v__mod_parser_make_when;
    _t1792->gem_v__mod_parser_make_while = &gem_v__mod_parser_make_while;
    _t1792->gem_v_advance = gem_v_advance;
    _t1792->gem_v_expect = gem_v_expect;
    _t1792->gem_v_file = gem_v_file;
    _t1792->gem_v_fn_depth = gem_v_fn_depth;
    _t1792->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1792->gem_v_parse_body = gem_v_parse_body;
    _t1792->gem_v_parse_expr = gem_v_parse_expr;
    _t1792->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1792->gem_v_parse_params = gem_v_parse_params;
    _t1792->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1792->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1792->gem_v_peek = gem_v_peek;
    _t1792->gem_v_peek_at = gem_v_peek_at;
    _t1792->gem_v_skip_nl = gem_v_skip_nl;
    _t1792->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1792);
#line 1069 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 1073 "compiler/main.gem"
    struct _closure__anon_23 *_t1802 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1802->gem_v__mod_parser_make_program = &gem_v__mod_parser_make_program;
    _t1802->gem_v_at_end = gem_v_at_end;
    _t1802->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1802->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1802);
    GemVal _t1803 = gem_table_new();
    gem_table_set(_t1803, gem_string("parse"), gem_v_parse);
    GemVal _t1804 = _t1803;
    gem_pop_frame();
    return _t1804;
}

static GemVal gem_fn__mod_codegen_escape_c_string(void *_env, GemVal *args, int argc) {
#line 7 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_escape_c_string", "compiler/main.gem", 7);
#line 8 "compiler/main.gem"
    GemVal _t1805[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1805, 1);
#line 9 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 10 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 10 "compiler/main.gem"
    GemVal _t1806[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1806, 1);
#line 10 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 10 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_1;
#line 10 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 11 "compiler/main.gem"
        GemVal gem_v_ch = gem_table_get(gem_v_s, gem_v_i);
#line 12 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\\")))) {
#line 13 "compiler/main.gem"
    GemVal _t1807[] = {gem_v_b, gem_string("\\\\")};
            (void)(gem_buf_push_fn(NULL, _t1807, 2));
        } else {
#line 14 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 15 "compiler/main.gem"
    GemVal _t1808[] = {gem_v_b, gem_string("\\\"")};
                (void)(gem_buf_push_fn(NULL, _t1808, 2));
            } else {
#line 16 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 17 "compiler/main.gem"
    GemVal _t1809[] = {gem_v_b, gem_string("\\n")};
                    (void)(gem_buf_push_fn(NULL, _t1809, 2));
                } else {
#line 18 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 19 "compiler/main.gem"
    GemVal _t1810[] = {gem_v_b, gem_string("\\r")};
                        (void)(gem_buf_push_fn(NULL, _t1810, 2));
                    } else {
#line 20 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 21 "compiler/main.gem"
    GemVal _t1811[] = {gem_v_b, gem_string("\\t")};
                            (void)(gem_buf_push_fn(NULL, _t1811, 2));
                        } else {
#line 22 "compiler/main.gem"
    GemVal _t1812[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1812, 1), gem_int(0)))) {
#line 23 "compiler/main.gem"
    GemVal _t1813[] = {gem_v_b, gem_string("\\0")};
                                (void)(gem_buf_push_fn(NULL, _t1813, 2));
                            } else {
#line 25 "compiler/main.gem"
    GemVal _t1814[] = {gem_v_b, gem_v_ch};
                                (void)(gem_buf_push_fn(NULL, _t1814, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 28 "compiler/main.gem"
    GemVal _t1815[] = {gem_v_b};
    GemVal _t1816 = gem_buf_str_fn(NULL, _t1815, 1);
    gem_pop_frame();
    return _t1816;
}

static GemVal gem_fn__mod_codegen_repeat_str(void *_env, GemVal *args, int argc) {
#line 31 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_repeat_str", "compiler/main.gem", 31);
#line 32 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 33 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 33 "compiler/main.gem"
    GemVal gem_v__for_limit_2 = gem_v_n;
#line 33 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 33 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 33 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 34 "compiler/main.gem"
    GemVal _t1817[] = {gem_v_b, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t1817, 2));
    }

#line 36 "compiler/main.gem"
    GemVal _t1818[] = {gem_v_b};
    GemVal _t1819 = gem_buf_str_fn(NULL, _t1818, 1);
    gem_pop_frame();
    return _t1819;
}

static GemVal gem_fn__mod_codegen_pad_for(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_pad_for", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal _t1820[] = {gem_string("    "), gem_v_indent};
    GemVal _t1821 = gem_fn__mod_codegen_repeat_str(NULL, _t1820, 2);
    gem_pop_frame();
    return _t1821;
}

static GemVal gem_fn__mod_codegen_format_float(void *_env, GemVal *args, int argc) {
#line 43 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_format_float", "compiler/main.gem", 43);
#line 44 "compiler/main.gem"
    GemVal _t1822[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1822, 1);
#line 46 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 47 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 47 "compiler/main.gem"
    GemVal _t1823[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1823, 1);
#line 47 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 47 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_3;
#line 47 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 48 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_v_i), gem_string(".")))) {
#line 49 "compiler/main.gem"
            gem_v_has_dot = gem_bool(1);
        }
    }

#line 52 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_has_dot))) {
#line 53 "compiler/main.gem"
        gem_v_s = gem_add(gem_v_s, gem_string(".0"));
    }
    GemVal _t1824 = gem_v_s;
    gem_pop_frame();
    return _t1824;
}

static GemVal gem_fn__mod_codegen_set_add(void *_env, GemVal *args, int argc) {
#line 60 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_add", "compiler/main.gem", 60);
#line 61 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1)))) {
#line 62 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_val, gem_bool(1));
        gem_pop_frame();
        return GEM_NIL;
    } else {
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn__mod_codegen_set_contains(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_val = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_contains", "compiler/main.gem", 66);
    GemVal _t1825 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1825;
}

static GemVal gem_fn__mod_codegen_set_union(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_union", "compiler/main.gem", 70);
#line 71 "compiler/main.gem"
    GemVal _t1826 = gem_table_new();
    GemVal gem_v_result = _t1826;
#line 72 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 72 "compiler/main.gem"
    GemVal _t1827[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_keys_4 = gem_keys_fn(NULL, _t1827, 1);
#line 72 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 72 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1828[] = {gem_v__for_keys_4};
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_len_fn(NULL, _t1828, 1)))) break;
#line 72 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_4, gem_v__for_i_4);
#line 72 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_4, gem_table_get(gem_v__for_keys_4, gem_v__for_i_4));
#line 72 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 73 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

#line 75 "compiler/main.gem"
    GemVal gem_v__for_tbl_5 = gem_v_b;
#line 75 "compiler/main.gem"
    GemVal _t1829[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_keys_5 = gem_keys_fn(NULL, _t1829, 1);
#line 75 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 75 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1830[] = {gem_v__for_keys_5};
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1830, 1)))) break;
#line 75 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_5, gem_v__for_i_5);
#line 75 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_5, gem_table_get(gem_v__for_keys_5, gem_v__for_i_5));
#line 75 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 76 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1831 = gem_v_result;
    gem_pop_frame();
    return _t1831;
}

static GemVal gem_fn__mod_codegen_set_intersect(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_intersect", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t1832 = gem_table_new();
    GemVal gem_v_result = _t1832;
#line 83 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 83 "compiler/main.gem"
    GemVal _t1833[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_keys_6 = gem_keys_fn(NULL, _t1833, 1);
#line 83 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 83 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1834[] = {gem_v__for_keys_6};
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1834, 1)))) break;
#line 83 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_6, gem_v__for_i_6);
#line 83 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_6, gem_table_get(gem_v__for_keys_6, gem_v__for_i_6));
#line 83 "compiler/main.gem"
        gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 84 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_v_k), gem_bool(1)))) {
#line 85 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
        }
    }

    GemVal _t1835 = gem_v_result;
    gem_pop_frame();
    return _t1835;
}

static GemVal gem_fn__mod_codegen_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 91 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_sorted_array_to_set", "compiler/main.gem", 91);
#line 92 "compiler/main.gem"
    GemVal _t1836 = gem_table_new();
    GemVal gem_v_s = _t1836;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1837[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1837, 1)))) break;
#line 93 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 93 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 94 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1838 = gem_v_s;
    gem_pop_frame();
    return _t1838;
}

static GemVal gem_fn__mod_codegen_is_node(void *_env, GemVal *args, int argc) {
#line 99 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_is_node", "compiler/main.gem", 99);
    GemVal _t1839[] = {gem_v_node};
    GemVal _t1841;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1839, 1), gem_string("table")))) {
        _t1841 = gem_eq(gem_type_fn(NULL, _t1839, 1), gem_string("table"));
    } else {
        GemVal _t1840 = gem_v_node;
        _t1841 = gem_eq(gem_table_get(_t1840, gem_string("tag")), gem_v_tag);
    }
    GemVal _t1842 = _t1841;
    gem_pop_frame();
    return _t1842;
}

static GemVal gem_fn__mod_codegen_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_collect_block_lets", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t1843[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1843, 2))) {
        {
#line 107 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal _t1844 = gem_v_stmt;
    GemVal _t1845[] = {gem_table_get(_t1844, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1845, 1);
#line 107 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 107 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 107 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 108 "compiler/main.gem"
    GemVal _t1846 = gem_v_stmt;
    GemVal _t1847[] = {gem_table_get(gem_table_get(_t1846, gem_string("stmts")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1847, 2))) {
#line 109 "compiler/main.gem"
    GemVal _t1848 = gem_v_stmt;
    GemVal _t1849 = gem_table_get(gem_table_get(_t1848, gem_string("stmts")), gem_v_j);
    GemVal _t1850[] = {gem_v_result, gem_table_get(_t1849, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1850, 2));
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

static GemVal gem_fn__mod_codegen_collect_top_let_names(void *_env, GemVal *args, int argc) {
#line 115 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_collect_top_let_names", "compiler/main.gem", 115);
    {
#line 116 "compiler/main.gem"
        GemVal gem_v__for_i_9 = gem_int(0);
#line 116 "compiler/main.gem"
    GemVal _t1851[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1851, 1);
#line 116 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 116 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_9;
#line 116 "compiler/main.gem"
            gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 117 "compiler/main.gem"
            GemVal gem_v_s = gem_table_get(gem_v_stmts, gem_v_i);
#line 118 "compiler/main.gem"
    GemVal _t1852[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1852, 2))) {
#line 119 "compiler/main.gem"
    GemVal _t1853 = gem_v_s;
    GemVal _t1854[] = {gem_v_result, gem_table_get(_t1853, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1854, 2));
            }
#line 121 "compiler/main.gem"
    GemVal _t1855[] = {gem_v_s, gem_v_result};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t1855, 2));
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
#line 228 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1868[] = {(*gem_v_tmp_counter)};
    GemVal _t1869 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1868, 1));
    gem_pop_frame();
    return _t1869;
}

struct _closure__anon_25 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 233 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1871[] = {(*gem_v_anon_counter)};
    GemVal _t1872 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1871, 1));
    gem_pop_frame();
    return _t1872;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1874[] = {gem_v_name};
    GemVal _t1875 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1874, 1));
    gem_pop_frame();
    return _t1875;
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
#line 247 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 248 "compiler/main.gem"
        GemVal _t1876 = GEM_NIL;
        gem_pop_frame();
        return _t1876;
    }
#line 250 "compiler/main.gem"
    GemVal _t1877[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1877, 1), gem_string("table")))) {
#line 251 "compiler/main.gem"
        GemVal _t1878 = GEM_NIL;
        gem_pop_frame();
        return _t1878;
    }
#line 253 "compiler/main.gem"
    GemVal _t1879 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1879, gem_string("tag")), GEM_NIL))) {
#line 254 "compiler/main.gem"
        GemVal _t1880 = GEM_NIL;
        gem_pop_frame();
        return _t1880;
    }
#line 257 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1881[] = {gem_v__match_11};
    GemVal _t1883;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1881, 1), gem_string("table")))) {
        _t1883 = gem_eq(gem_type_fn(NULL, _t1881, 1), gem_string("table"));
    } else {
        GemVal _t1882[] = {gem_v__match_11, gem_string("tag")};
        _t1883 = gem_has_key_fn(NULL, _t1882, 2);
    }
    GemVal _t1884;
    if (!gem_truthy(_t1883)) {
        _t1884 = _t1883;
    } else {
        _t1884 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1886;
    if (!gem_truthy(_t1884)) {
        _t1886 = _t1884;
    } else {
        GemVal _t1885[] = {gem_v__match_11, gem_string("name")};
        _t1886 = gem_has_key_fn(NULL, _t1885, 2);
    }
    if (gem_truthy(_t1886)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 259 "compiler/main.gem"
    GemVal _t1887[] = {gem_v_defined, gem_v_name};
    GemVal _t1891;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1887, 2)))) {
        _t1891 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1887, 2));
    } else {
        GemVal _t1888[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1890;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1888, 2)))) {
                _t1890 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1888, 2));
        } else {
                GemVal _t1889[] = {(*gem_v_local_names), gem_v_name};
                _t1890 = gem_fn__mod_codegen_set_contains(NULL, _t1889, 2);
        }
        _t1891 = _t1890;
    }
    GemVal _t1893;
    if (!gem_truthy(_t1891)) {
        _t1893 = _t1891;
    } else {
        GemVal _t1892[] = {(*gem_v_defined_fns), gem_v_name};
        _t1893 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1892, 2));
    }
        if (gem_truthy(_t1893)) {
#line 260 "compiler/main.gem"
    GemVal _t1894[] = {gem_v_free, gem_v_name};
            GemVal _t1895 = gem_fn__mod_codegen_set_add(NULL, _t1894, 2);
            gem_pop_frame();
            return _t1895;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1896[] = {gem_v__match_11};
    GemVal _t1898;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1896, 1), gem_string("table")))) {
        _t1898 = gem_eq(gem_type_fn(NULL, _t1896, 1), gem_string("table"));
    } else {
        GemVal _t1897[] = {gem_v__match_11, gem_string("tag")};
        _t1898 = gem_has_key_fn(NULL, _t1897, 2);
    }
    GemVal _t1899;
    if (!gem_truthy(_t1898)) {
        _t1899 = _t1898;
    } else {
        _t1899 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1901;
    if (!gem_truthy(_t1899)) {
        _t1901 = _t1899;
    } else {
        GemVal _t1900[] = {gem_v__match_11, gem_string("value")};
        _t1901 = gem_has_key_fn(NULL, _t1900, 2);
    }
    if (gem_truthy(_t1901)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 263 "compiler/main.gem"
    GemVal _t1902[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1903 = (*gem_v_collect_free_node);
        GemVal _t1904 = _t1903.fn(_t1903.env, _t1902, 3);
        gem_pop_frame();
        return _t1904;
    } else {
    GemVal _t1905[] = {gem_v__match_11};
    GemVal _t1907;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1905, 1), gem_string("table")))) {
        _t1907 = gem_eq(gem_type_fn(NULL, _t1905, 1), gem_string("table"));
    } else {
        GemVal _t1906[] = {gem_v__match_11, gem_string("tag")};
        _t1907 = gem_has_key_fn(NULL, _t1906, 2);
    }
    GemVal _t1908;
    if (!gem_truthy(_t1907)) {
        _t1908 = _t1907;
    } else {
        _t1908 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1910;
    if (!gem_truthy(_t1908)) {
        _t1910 = _t1908;
    } else {
        GemVal _t1909[] = {gem_v__match_11, gem_string("name")};
        _t1910 = gem_has_key_fn(NULL, _t1909, 2);
    }
    GemVal _t1912;
    if (!gem_truthy(_t1910)) {
        _t1912 = _t1910;
    } else {
        GemVal _t1911[] = {gem_v__match_11, gem_string("value")};
        _t1912 = gem_has_key_fn(NULL, _t1911, 2);
    }
    if (gem_truthy(_t1912)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 265 "compiler/main.gem"
    GemVal _t1913[] = {gem_v_defined, gem_v_name};
    GemVal _t1917;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1913, 2)))) {
        _t1917 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1913, 2));
    } else {
        GemVal _t1914[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1916;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1914, 2)))) {
                _t1916 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1914, 2));
        } else {
                GemVal _t1915[] = {(*gem_v_local_names), gem_v_name};
                _t1916 = gem_fn__mod_codegen_set_contains(NULL, _t1915, 2);
        }
        _t1917 = _t1916;
    }
    GemVal _t1919;
    if (!gem_truthy(_t1917)) {
        _t1919 = _t1917;
    } else {
        GemVal _t1918[] = {(*gem_v_defined_fns), gem_v_name};
        _t1919 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1918, 2));
    }
        if (gem_truthy(_t1919)) {
#line 266 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_free, gem_v_name};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1920, 2));
        }
#line 268 "compiler/main.gem"
    GemVal _t1921[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1922 = (*gem_v_collect_free_node);
        GemVal _t1923 = _t1922.fn(_t1922.env, _t1921, 3);
        gem_pop_frame();
        return _t1923;
    } else {
    GemVal _t1924[] = {gem_v__match_11};
    GemVal _t1926;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1924, 1), gem_string("table")))) {
        _t1926 = gem_eq(gem_type_fn(NULL, _t1924, 1), gem_string("table"));
    } else {
        GemVal _t1925[] = {gem_v__match_11, gem_string("tag")};
        _t1926 = gem_has_key_fn(NULL, _t1925, 2);
    }
    GemVal _t1927;
    if (!gem_truthy(_t1926)) {
        _t1927 = _t1926;
    } else {
        _t1927 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1929;
    if (!gem_truthy(_t1927)) {
        _t1929 = _t1927;
    } else {
        GemVal _t1928[] = {gem_v__match_11, gem_string("params")};
        _t1929 = gem_has_key_fn(NULL, _t1928, 2);
    }
    GemVal _t1931;
    if (!gem_truthy(_t1929)) {
        _t1931 = _t1929;
    } else {
        GemVal _t1930[] = {gem_v__match_11, gem_string("rest_param")};
        _t1931 = gem_has_key_fn(NULL, _t1930, 2);
    }
    GemVal _t1933;
    if (!gem_truthy(_t1931)) {
        _t1933 = _t1931;
    } else {
        GemVal _t1932[] = {gem_v__match_11, gem_string("block_param")};
        _t1933 = gem_has_key_fn(NULL, _t1932, 2);
    }
    GemVal _t1935;
    if (!gem_truthy(_t1933)) {
        _t1935 = _t1933;
    } else {
        GemVal _t1934[] = {gem_v__match_11, gem_string("body")};
        _t1935 = gem_has_key_fn(NULL, _t1934, 2);
    }
    if (gem_truthy(_t1935)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_11, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 270 "compiler/main.gem"
    GemVal _t1936[] = {gem_v_params};
    GemVal _t1937[] = {gem_v_defined, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t1936, 1)};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_set_union(NULL, _t1937, 2);
#line 271 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 272 "compiler/main.gem"
    GemVal _t1938[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1938, 2));
        }
#line 274 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 275 "compiler/main.gem"
    GemVal _t1939[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1939, 2));
        }
#line 277 "compiler/main.gem"
    GemVal _t1940[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1941 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1941.fn(_t1941.env, _t1940, 2);
        {
#line 278 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 278 "compiler/main.gem"
    GemVal _t1942[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_keys_12 = gem_keys_fn(NULL, _t1942, 1);
#line 278 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 278 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1943[] = {gem_v__for_keys_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1943, 1)))) break;
#line 278 "compiler/main.gem"
                GemVal gem_v_ifk = gem_table_get(gem_v__for_keys_12, gem_v__for_i_12);
#line 278 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_12, gem_table_get(gem_v__for_keys_12, gem_v__for_i_12));
#line 278 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 279 "compiler/main.gem"
    GemVal _t1944[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1944, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1945[] = {gem_v__match_11};
    GemVal _t1947;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1945, 1), gem_string("table")))) {
        _t1947 = gem_eq(gem_type_fn(NULL, _t1945, 1), gem_string("table"));
    } else {
        GemVal _t1946[] = {gem_v__match_11, gem_string("tag")};
        _t1947 = gem_has_key_fn(NULL, _t1946, 2);
    }
    GemVal _t1948;
    if (!gem_truthy(_t1947)) {
        _t1948 = _t1947;
    } else {
        _t1948 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1948)) {
        gem_pop_frame();
        return GEM_NIL;
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
        _t1952 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1954;
    if (!gem_truthy(_t1952)) {
        _t1954 = _t1952;
    } else {
        GemVal _t1953[] = {gem_v__match_11, gem_string("cond")};
        _t1954 = gem_has_key_fn(NULL, _t1953, 2);
    }
    GemVal _t1956;
    if (!gem_truthy(_t1954)) {
        _t1956 = _t1954;
    } else {
        GemVal _t1955[] = {gem_v__match_11, gem_string("then")};
        _t1956 = gem_has_key_fn(NULL, _t1955, 2);
    }
    GemVal _t1958;
    if (!gem_truthy(_t1956)) {
        _t1958 = _t1956;
    } else {
        GemVal _t1957[] = {gem_v__match_11, gem_string("else")};
        _t1958 = gem_has_key_fn(NULL, _t1957, 2);
    }
    if (gem_truthy(_t1958)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 284 "compiler/main.gem"
    GemVal _t1959[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1960 = (*gem_v_collect_free_node);
        (void)(_t1960.fn(_t1960.env, _t1959, 3));
#line 285 "compiler/main.gem"
    GemVal _t1961 = gem_table_new();
    GemVal _t1962[] = {gem_v_defined, _t1961};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1962, 2);
#line 286 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 286 "compiler/main.gem"
    GemVal _t1963[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1963, 1);
#line 286 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 286 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 286 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 287 "compiler/main.gem"
    GemVal _t1964[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1965 = (*gem_v_collect_free_node);
            (void)(_t1965.fn(_t1965.env, _t1964, 3));
#line 288 "compiler/main.gem"
    GemVal _t1966[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1966, 2))) {
#line 289 "compiler/main.gem"
    GemVal _t1967 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1968[] = {gem_v_d, gem_table_get(_t1967, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1968, 2));
            }
        }

#line 292 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 293 "compiler/main.gem"
    GemVal _t1969 = gem_table_new();
    GemVal _t1970[] = {gem_v_defined, _t1969};
            gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1970, 2);
            {
#line 294 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 294 "compiler/main.gem"
    GemVal _t1971[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1971, 1);
#line 294 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 294 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 294 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 295 "compiler/main.gem"
    GemVal _t1972[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1973 = (*gem_v_collect_free_node);
                    (void)(_t1973.fn(_t1973.env, _t1972, 3));
#line 296 "compiler/main.gem"
    GemVal _t1974[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1974, 2))) {
#line 297 "compiler/main.gem"
    GemVal _t1975 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1976[] = {gem_v_d, gem_table_get(_t1975, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1976, 2));
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
    GemVal _t1977[] = {gem_v__match_11};
    GemVal _t1979;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1977, 1), gem_string("table")))) {
        _t1979 = gem_eq(gem_type_fn(NULL, _t1977, 1), gem_string("table"));
    } else {
        GemVal _t1978[] = {gem_v__match_11, gem_string("tag")};
        _t1979 = gem_has_key_fn(NULL, _t1978, 2);
    }
    GemVal _t1980;
    if (!gem_truthy(_t1979)) {
        _t1980 = _t1979;
    } else {
        _t1980 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1982;
    if (!gem_truthy(_t1980)) {
        _t1982 = _t1980;
    } else {
        GemVal _t1981[] = {gem_v__match_11, gem_string("cond")};
        _t1982 = gem_has_key_fn(NULL, _t1981, 2);
    }
    GemVal _t1984;
    if (!gem_truthy(_t1982)) {
        _t1984 = _t1982;
    } else {
        GemVal _t1983[] = {gem_v__match_11, gem_string("body")};
        _t1984 = gem_has_key_fn(NULL, _t1983, 2);
    }
    if (gem_truthy(_t1984)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 302 "compiler/main.gem"
    GemVal _t1985[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1986 = (*gem_v_collect_free_node);
        (void)(_t1986.fn(_t1986.env, _t1985, 3));
#line 303 "compiler/main.gem"
    GemVal _t1987 = gem_table_new();
    GemVal _t1988[] = {gem_v_defined, _t1987};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1988, 2);
        {
#line 304 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 304 "compiler/main.gem"
    GemVal _t1989[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1989, 1);
#line 304 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 304 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 304 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 305 "compiler/main.gem"
    GemVal _t1990[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1991 = (*gem_v_collect_free_node);
                (void)(_t1991.fn(_t1991.env, _t1990, 3));
#line 306 "compiler/main.gem"
    GemVal _t1992[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1992, 2))) {
#line 307 "compiler/main.gem"
    GemVal _t1993 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1994[] = {gem_v_d, gem_table_get(_t1993, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1994, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1995[] = {gem_v__match_11};
    GemVal _t1997;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1995, 1), gem_string("table")))) {
        _t1997 = gem_eq(gem_type_fn(NULL, _t1995, 1), gem_string("table"));
    } else {
        GemVal _t1996[] = {gem_v__match_11, gem_string("tag")};
        _t1997 = gem_has_key_fn(NULL, _t1996, 2);
    }
    GemVal _t1998;
    if (!gem_truthy(_t1997)) {
        _t1998 = _t1997;
    } else {
        _t1998 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2000;
    if (!gem_truthy(_t1998)) {
        _t2000 = _t1998;
    } else {
        GemVal _t1999[] = {gem_v__match_11, gem_string("target")};
        _t2000 = gem_has_key_fn(NULL, _t1999, 2);
    }
    GemVal _t2002;
    if (!gem_truthy(_t2000)) {
        _t2002 = _t2000;
    } else {
        GemVal _t2001[] = {gem_v__match_11, gem_string("target_var")};
        _t2002 = gem_has_key_fn(NULL, _t2001, 2);
    }
    GemVal _t2004;
    if (!gem_truthy(_t2002)) {
        _t2004 = _t2002;
    } else {
        GemVal _t2003[] = {gem_v__match_11, gem_string("whens")};
        _t2004 = gem_has_key_fn(NULL, _t2003, 2);
    }
    GemVal _t2006;
    if (!gem_truthy(_t2004)) {
        _t2006 = _t2004;
    } else {
        GemVal _t2005[] = {gem_v__match_11, gem_string("else")};
        _t2006 = gem_has_key_fn(NULL, _t2005, 2);
    }
    if (gem_truthy(_t2006)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 311 "compiler/main.gem"
    GemVal _t2007[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t2008 = (*gem_v_collect_free_node);
        (void)(_t2008.fn(_t2008.env, _t2007, 3));
#line 312 "compiler/main.gem"
    GemVal _t2009 = gem_table_new();
    GemVal _t2010[] = {gem_v_defined, _t2009};
        GemVal gem_v_match_def = gem_fn__mod_codegen_set_union(NULL, _t2010, 2);
#line 313 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 314 "compiler/main.gem"
    GemVal _t2011[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2011, 2));
        }
#line 316 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 316 "compiler/main.gem"
    GemVal _t2012[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t2012, 1);
#line 316 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 316 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 316 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 317 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 318 "compiler/main.gem"
    GemVal _t2013 = gem_v_w;
    GemVal _t2014[] = {gem_table_get(_t2013, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t2015 = (*gem_v_collect_free_node);
            (void)(_t2015.fn(_t2015.env, _t2014, 3));
#line 319 "compiler/main.gem"
    GemVal _t2016 = gem_table_new();
    GemVal _t2017[] = {gem_v_match_def, _t2016};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2017, 2);
#line 320 "compiler/main.gem"
    GemVal _t2018 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t2018, gem_string("bindings")), GEM_NIL))) {
#line 321 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 321 "compiler/main.gem"
    GemVal _t2019 = gem_v_w;
    GemVal _t2020[] = {gem_table_get(_t2019, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t2020, 1);
#line 321 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 321 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 321 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 322 "compiler/main.gem"
    GemVal _t2021 = gem_v_w;
    GemVal _t2022[] = {gem_table_get(gem_table_get(_t2021, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2023 = (*gem_v_collect_free_node);
                    (void)(_t2023.fn(_t2023.env, _t2022, 3));
#line 323 "compiler/main.gem"
    GemVal _t2024 = gem_v_w;
    GemVal _t2025[] = {gem_table_get(gem_table_get(_t2024, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2025, 2))) {
#line 324 "compiler/main.gem"
    GemVal _t2026 = gem_v_w;
    GemVal _t2027 = gem_table_get(gem_table_get(_t2026, gem_string("bindings")), gem_v_bi);
    GemVal _t2028[] = {gem_v_d, gem_table_get(_t2027, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2028, 2));
                    }
                }

            }
#line 328 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 328 "compiler/main.gem"
    GemVal _t2029 = gem_v_w;
    GemVal _t2030[] = {gem_table_get(_t2029, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t2030, 1);
#line 328 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 328 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 328 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 329 "compiler/main.gem"
    GemVal _t2031 = gem_v_w;
    GemVal _t2032[] = {gem_table_get(gem_table_get(_t2031, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2033 = (*gem_v_collect_free_node);
                (void)(_t2033.fn(_t2033.env, _t2032, 3));
#line 330 "compiler/main.gem"
    GemVal _t2034 = gem_v_w;
    GemVal _t2035[] = {gem_table_get(gem_table_get(_t2034, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2035, 2))) {
#line 331 "compiler/main.gem"
    GemVal _t2036 = gem_v_w;
    GemVal _t2037 = gem_table_get(gem_table_get(_t2036, gem_string("body")), gem_v_j);
    GemVal _t2038[] = {gem_v_d, gem_table_get(_t2037, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2038, 2));
                }
            }

        }

#line 335 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 336 "compiler/main.gem"
    GemVal _t2039 = gem_table_new();
    GemVal _t2040[] = {gem_v_defined, _t2039};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2040, 2);
            {
#line 337 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 337 "compiler/main.gem"
    GemVal _t2041[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t2041, 1);
#line 337 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 337 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 337 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 338 "compiler/main.gem"
    GemVal _t2042[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2043 = (*gem_v_collect_free_node);
                    (void)(_t2043.fn(_t2043.env, _t2042, 3));
#line 339 "compiler/main.gem"
    GemVal _t2044[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2044, 2))) {
#line 340 "compiler/main.gem"
    GemVal _t2045 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t2046[] = {gem_v_d, gem_table_get(_t2045, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2046, 2));
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
    GemVal _t2047[] = {gem_v__match_11};
    GemVal _t2049;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2047, 1), gem_string("table")))) {
        _t2049 = gem_eq(gem_type_fn(NULL, _t2047, 1), gem_string("table"));
    } else {
        GemVal _t2048[] = {gem_v__match_11, gem_string("tag")};
        _t2049 = gem_has_key_fn(NULL, _t2048, 2);
    }
    GemVal _t2050;
    if (!gem_truthy(_t2049)) {
        _t2050 = _t2049;
    } else {
        _t2050 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t2052;
    if (!gem_truthy(_t2050)) {
        _t2052 = _t2050;
    } else {
        GemVal _t2051[] = {gem_v__match_11, gem_string("value")};
        _t2052 = gem_has_key_fn(NULL, _t2051, 2);
    }
    GemVal _t2054;
    if (!gem_truthy(_t2052)) {
        _t2054 = _t2052;
    } else {
        GemVal _t2053[] = {gem_v__match_11, gem_string("bindings")};
        _t2054 = gem_has_key_fn(NULL, _t2053, 2);
    }
    GemVal _t2056;
    if (!gem_truthy(_t2054)) {
        _t2056 = _t2054;
    } else {
        GemVal _t2055[] = {gem_v__match_11, gem_string("body")};
        _t2056 = gem_has_key_fn(NULL, _t2055, 2);
    }
    if (gem_truthy(_t2056)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 345 "compiler/main.gem"
    GemVal _t2057[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2058 = (*gem_v_collect_free_node);
        (void)(_t2058.fn(_t2058.env, _t2057, 3));
#line 346 "compiler/main.gem"
    GemVal _t2059 = gem_table_new();
    GemVal _t2060[] = {gem_v_defined, _t2059};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2060, 2);
#line 347 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 348 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 348 "compiler/main.gem"
    GemVal _t2061[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t2061, 1);
#line 348 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 348 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 348 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 349 "compiler/main.gem"
    GemVal _t2062[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2063 = (*gem_v_collect_free_node);
                (void)(_t2063.fn(_t2063.env, _t2062, 3));
#line 350 "compiler/main.gem"
    GemVal _t2064[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2064, 2))) {
#line 351 "compiler/main.gem"
    GemVal _t2065 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t2066[] = {gem_v_d, gem_table_get(_t2065, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2066, 2));
                }
            }

        }
        {
#line 355 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 355 "compiler/main.gem"
    GemVal _t2067[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t2067, 1);
#line 355 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 355 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 355 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 356 "compiler/main.gem"
    GemVal _t2068[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2069 = (*gem_v_collect_free_node);
                (void)(_t2069.fn(_t2069.env, _t2068, 3));
#line 357 "compiler/main.gem"
    GemVal _t2070[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2070, 2))) {
#line 358 "compiler/main.gem"
    GemVal _t2071 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2072[] = {gem_v_d, gem_table_get(_t2071, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2072, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2073[] = {gem_v__match_11};
    GemVal _t2075;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2073, 1), gem_string("table")))) {
        _t2075 = gem_eq(gem_type_fn(NULL, _t2073, 1), gem_string("table"));
    } else {
        GemVal _t2074[] = {gem_v__match_11, gem_string("tag")};
        _t2075 = gem_has_key_fn(NULL, _t2074, 2);
    }
    GemVal _t2076;
    if (!gem_truthy(_t2075)) {
        _t2076 = _t2075;
    } else {
        _t2076 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2078;
    if (!gem_truthy(_t2076)) {
        _t2078 = _t2076;
    } else {
        GemVal _t2077[] = {gem_v__match_11, gem_string("arms")};
        _t2078 = gem_has_key_fn(NULL, _t2077, 2);
    }
    GemVal _t2080;
    if (!gem_truthy(_t2078)) {
        _t2080 = _t2078;
    } else {
        GemVal _t2079[] = {gem_v__match_11, gem_string("after_ms")};
        _t2080 = gem_has_key_fn(NULL, _t2079, 2);
    }
    GemVal _t2082;
    if (!gem_truthy(_t2080)) {
        _t2082 = _t2080;
    } else {
        GemVal _t2081[] = {gem_v__match_11, gem_string("after_body")};
        _t2082 = gem_has_key_fn(NULL, _t2081, 2);
    }
    if (gem_truthy(_t2082)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 362 "compiler/main.gem"
    GemVal _t2083 = gem_table_new();
    GemVal _t2084[] = {gem_v_defined, _t2083};
        GemVal gem_v_recv_def = gem_fn__mod_codegen_set_union(NULL, _t2084, 2);
#line 363 "compiler/main.gem"
    GemVal _t2085 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2085, gem_string("recv_var")), GEM_NIL))) {
#line 364 "compiler/main.gem"
    GemVal _t2086 = gem_v_node;
    GemVal _t2087[] = {gem_v_recv_def, gem_table_get(_t2086, gem_string("recv_var"))};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2087, 2));
        }
#line 366 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 366 "compiler/main.gem"
    GemVal _t2088[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t2088, 1);
#line 366 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 366 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 366 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 367 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 368 "compiler/main.gem"
    GemVal _t2089 = gem_v_arm;
    GemVal _t2090 = gem_table_get(_t2089, gem_string("pattern"));
    GemVal _t2091[] = {gem_table_get(_t2090, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t2092 = (*gem_v_collect_free_node);
            (void)(_t2092.fn(_t2092.env, _t2091, 3));
#line 369 "compiler/main.gem"
    GemVal _t2093 = gem_table_new();
    GemVal _t2094[] = {gem_v_recv_def, _t2093};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2094, 2);
#line 370 "compiler/main.gem"
    GemVal _t2095 = gem_v_arm;
    GemVal _t2096 = gem_table_get(_t2095, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2096, gem_string("bindings")), GEM_NIL))) {
#line 371 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 371 "compiler/main.gem"
    GemVal _t2097 = gem_v_arm;
    GemVal _t2098 = gem_table_get(_t2097, gem_string("pattern"));
    GemVal _t2099[] = {gem_table_get(_t2098, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t2099, 1);
#line 371 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 371 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 371 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 372 "compiler/main.gem"
    GemVal _t2100 = gem_v_arm;
    GemVal _t2101 = gem_table_get(_t2100, gem_string("pattern"));
    GemVal _t2102[] = {gem_table_get(gem_table_get(_t2101, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2103 = (*gem_v_collect_free_node);
                    (void)(_t2103.fn(_t2103.env, _t2102, 3));
#line 373 "compiler/main.gem"
    GemVal _t2104 = gem_v_arm;
    GemVal _t2105 = gem_table_get(_t2104, gem_string("pattern"));
    GemVal _t2106[] = {gem_table_get(gem_table_get(_t2105, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2106, 2))) {
#line 374 "compiler/main.gem"
    GemVal _t2107 = gem_v_arm;
    GemVal _t2108 = gem_table_get(_t2107, gem_string("pattern"));
    GemVal _t2109 = gem_table_get(gem_table_get(_t2108, gem_string("bindings")), gem_v_bi);
    GemVal _t2110[] = {gem_v_d, gem_table_get(_t2109, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2110, 2));
                    }
                }

            }
#line 378 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 378 "compiler/main.gem"
    GemVal _t2111 = gem_v_arm;
    GemVal _t2112[] = {gem_table_get(_t2111, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t2112, 1);
#line 378 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 378 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 378 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 379 "compiler/main.gem"
    GemVal _t2113 = gem_v_arm;
    GemVal _t2114[] = {gem_table_get(gem_table_get(_t2113, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2115 = (*gem_v_collect_free_node);
                (void)(_t2115.fn(_t2115.env, _t2114, 3));
#line 380 "compiler/main.gem"
    GemVal _t2116 = gem_v_arm;
    GemVal _t2117[] = {gem_table_get(gem_table_get(_t2116, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2117, 2))) {
#line 381 "compiler/main.gem"
    GemVal _t2118 = gem_v_arm;
    GemVal _t2119 = gem_table_get(gem_table_get(_t2118, gem_string("body")), gem_v_j);
    GemVal _t2120[] = {gem_v_d, gem_table_get(_t2119, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2120, 2));
                }
            }

        }

#line 385 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 386 "compiler/main.gem"
    GemVal _t2121[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t2122 = (*gem_v_collect_free_node);
            (void)(_t2122.fn(_t2122.env, _t2121, 3));
        }
#line 388 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 389 "compiler/main.gem"
    GemVal _t2123 = gem_table_new();
    GemVal _t2124[] = {gem_v_recv_def, _t2123};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2124, 2);
            {
#line 390 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 390 "compiler/main.gem"
    GemVal _t2125[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t2125, 1);
#line 390 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 390 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 390 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 391 "compiler/main.gem"
    GemVal _t2126[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2127 = (*gem_v_collect_free_node);
                    (void)(_t2127.fn(_t2127.env, _t2126, 3));
#line 392 "compiler/main.gem"
    GemVal _t2128[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2128, 2))) {
#line 393 "compiler/main.gem"
    GemVal _t2129 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t2130[] = {gem_v_d, gem_table_get(_t2129, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2130, 2));
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
    GemVal _t2131[] = {gem_v__match_11};
    GemVal _t2133;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2131, 1), gem_string("table")))) {
        _t2133 = gem_eq(gem_type_fn(NULL, _t2131, 1), gem_string("table"));
    } else {
        GemVal _t2132[] = {gem_v__match_11, gem_string("tag")};
        _t2133 = gem_has_key_fn(NULL, _t2132, 2);
    }
    GemVal _t2134;
    if (!gem_truthy(_t2133)) {
        _t2134 = _t2133;
    } else {
        _t2134 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2136;
    if (!gem_truthy(_t2134)) {
        _t2136 = _t2134;
    } else {
        GemVal _t2135[] = {gem_v__match_11, gem_string("entries")};
        _t2136 = gem_has_key_fn(NULL, _t2135, 2);
    }
    if (gem_truthy(_t2136)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 398 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 398 "compiler/main.gem"
    GemVal _t2137[] = {gem_v_entries};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t2137, 1);
#line 398 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 398 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 398 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 399 "compiler/main.gem"
    GemVal _t2138 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2139[] = {gem_table_get(_t2138, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t2140 = (*gem_v_collect_free_node);
                (void)(_t2140.fn(_t2140.env, _t2139, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2141[] = {gem_v__match_11};
    GemVal _t2143;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2141, 1), gem_string("table")))) {
        _t2143 = gem_eq(gem_type_fn(NULL, _t2141, 1), gem_string("table"));
    } else {
        GemVal _t2142[] = {gem_v__match_11, gem_string("tag")};
        _t2143 = gem_has_key_fn(NULL, _t2142, 2);
    }
    GemVal _t2144;
    if (!gem_truthy(_t2143)) {
        _t2144 = _t2143;
    } else {
        _t2144 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2146;
    if (!gem_truthy(_t2144)) {
        _t2146 = _t2144;
    } else {
        GemVal _t2145[] = {gem_v__match_11, gem_string("elements")};
        _t2146 = gem_has_key_fn(NULL, _t2145, 2);
    }
    if (gem_truthy(_t2146)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 402 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 402 "compiler/main.gem"
    GemVal _t2147[] = {gem_v_elements};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t2147, 1);
#line 402 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 402 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 402 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 403 "compiler/main.gem"
    GemVal _t2148[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t2149 = (*gem_v_collect_free_node);
                (void)(_t2149.fn(_t2149.env, _t2148, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2150[] = {gem_v__match_11};
    GemVal _t2152;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2150, 1), gem_string("table")))) {
        _t2152 = gem_eq(gem_type_fn(NULL, _t2150, 1), gem_string("table"));
    } else {
        GemVal _t2151[] = {gem_v__match_11, gem_string("tag")};
        _t2152 = gem_has_key_fn(NULL, _t2151, 2);
    }
    GemVal _t2153;
    if (!gem_truthy(_t2152)) {
        _t2153 = _t2152;
    } else {
        _t2153 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2155;
    if (!gem_truthy(_t2153)) {
        _t2155 = _t2153;
    } else {
        GemVal _t2154[] = {gem_v__match_11, gem_string("stmts")};
        _t2155 = gem_has_key_fn(NULL, _t2154, 2);
    }
    if (gem_truthy(_t2155)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 406 "compiler/main.gem"
    GemVal _t2156 = gem_table_new();
    GemVal _t2157[] = {gem_v_defined, _t2156};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2157, 2);
        {
#line 407 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 407 "compiler/main.gem"
    GemVal _t2158[] = {gem_v_stmts};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t2158, 1);
#line 407 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 407 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 407 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 408 "compiler/main.gem"
    GemVal _t2159[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2160 = (*gem_v_collect_free_node);
                (void)(_t2160.fn(_t2160.env, _t2159, 3));
#line 409 "compiler/main.gem"
    GemVal _t2161[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2161, 2))) {
#line 410 "compiler/main.gem"
    GemVal _t2162 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2163[] = {gem_v_d, gem_table_get(_t2162, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2163, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2164[] = {gem_v__match_11};
    GemVal _t2166;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2164, 1), gem_string("table")))) {
        _t2166 = gem_eq(gem_type_fn(NULL, _t2164, 1), gem_string("table"));
    } else {
        GemVal _t2165[] = {gem_v__match_11, gem_string("tag")};
        _t2166 = gem_has_key_fn(NULL, _t2165, 2);
    }
    GemVal _t2167;
    if (!gem_truthy(_t2166)) {
        _t2167 = _t2166;
    } else {
        _t2167 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2169;
    if (!gem_truthy(_t2167)) {
        _t2169 = _t2167;
    } else {
        GemVal _t2168[] = {gem_v__match_11, gem_string("value")};
        _t2169 = gem_has_key_fn(NULL, _t2168, 2);
    }
    if (gem_truthy(_t2169)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 414 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 415 "compiler/main.gem"
    GemVal _t2170[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2171 = (*gem_v_collect_free_node);
            GemVal _t2172 = _t2171.fn(_t2171.env, _t2170, 3);
            gem_pop_frame();
            return _t2172;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 419 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 419 "compiler/main.gem"
    GemVal _t2173[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_keys_29 = gem_keys_fn(NULL, _t2173, 1);
#line 419 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 419 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2174[] = {gem_v__for_keys_29};
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_len_fn(NULL, _t2174, 1)))) break;
#line 419 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_29, gem_v__for_i_29);
#line 419 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_29, gem_table_get(gem_v__for_keys_29, gem_v__for_i_29));
#line 419 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 420 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 421 "compiler/main.gem"
    GemVal _t2175[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2175, 1), gem_string("table")))) {
#line 422 "compiler/main.gem"
    GemVal _t2176[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2176, 1);
#line 423 "compiler/main.gem"
    GemVal _t2177[] = {gem_v_vks};
    GemVal _t2179;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2177, 1), gem_int(0)))) {
        _t2179 = gem_gt(gem_len_fn(NULL, _t2177, 1), gem_int(0));
    } else {
        GemVal _t2178[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2179 = gem_eq(gem_type_fn(NULL, _t2178, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2179)) {
#line 424 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 425 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2180[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2180, 1)))) break;
#line 426 "compiler/main.gem"
    GemVal _t2181[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2181, 1), gem_string("table")))) {
#line 427 "compiler/main.gem"
    GemVal _t2182[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t2183 = (*gem_v_collect_free_node);
                                    (void)(_t2183.fn(_t2183.env, _t2182, 3));
                                }
#line 429 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 432 "compiler/main.gem"
    GemVal _t2184[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t2185 = (*gem_v_collect_free_node);
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
#line 441 "compiler/main.gem"
    GemVal _t2187 = gem_table_new();
    GemVal gem_v_free = _t2187;
#line 442 "compiler/main.gem"
    GemVal _t2188 = gem_table_new();
    GemVal _t2189[] = {gem_v_defined, _t2188};
    GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2189, 2);
#line 443 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 443 "compiler/main.gem"
    GemVal _t2190[] = {gem_v_stmts};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t2190, 1);
#line 443 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 443 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 443 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 444 "compiler/main.gem"
    GemVal _t2191[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2192 = (*gem_v_collect_free_node);
        (void)(_t2192.fn(_t2192.env, _t2191, 3));
#line 445 "compiler/main.gem"
    GemVal _t2193[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2193, 2))) {
#line 446 "compiler/main.gem"
    GemVal _t2194 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2195[] = {gem_v_d, gem_table_get(_t2194, gem_string("name"))};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2195, 2));
        }
#line 448 "compiler/main.gem"
    GemVal _t2196[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2196, 2));
    }

    GemVal _t2197 = gem_v_free;
    gem_pop_frame();
    return _t2197;
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
#line 459 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 460 "compiler/main.gem"
        GemVal _t2199 = GEM_NIL;
        gem_pop_frame();
        return _t2199;
    }
#line 462 "compiler/main.gem"
    GemVal _t2200[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2200, 1), gem_string("table")))) {
#line 463 "compiler/main.gem"
        GemVal _t2201 = GEM_NIL;
        gem_pop_frame();
        return _t2201;
    }
#line 465 "compiler/main.gem"
    GemVal _t2202 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t2202, gem_string("tag")), GEM_NIL))) {
#line 466 "compiler/main.gem"
        GemVal _t2203 = GEM_NIL;
        gem_pop_frame();
        return _t2203;
    }
#line 469 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
    GemVal _t2204[] = {gem_v__match_31};
    GemVal _t2206;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2204, 1), gem_string("table")))) {
        _t2206 = gem_eq(gem_type_fn(NULL, _t2204, 1), gem_string("table"));
    } else {
        GemVal _t2205[] = {gem_v__match_31, gem_string("tag")};
        _t2206 = gem_has_key_fn(NULL, _t2205, 2);
    }
    GemVal _t2207;
    if (!gem_truthy(_t2206)) {
        _t2207 = _t2206;
    } else {
        _t2207 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t2209;
    if (!gem_truthy(_t2207)) {
        _t2209 = _t2207;
    } else {
        GemVal _t2208[] = {gem_v__match_31, gem_string("params")};
        _t2209 = gem_has_key_fn(NULL, _t2208, 2);
    }
    GemVal _t2211;
    if (!gem_truthy(_t2209)) {
        _t2211 = _t2209;
    } else {
        GemVal _t2210[] = {gem_v__match_31, gem_string("rest_param")};
        _t2211 = gem_has_key_fn(NULL, _t2210, 2);
    }
    GemVal _t2213;
    if (!gem_truthy(_t2211)) {
        _t2213 = _t2211;
    } else {
        GemVal _t2212[] = {gem_v__match_31, gem_string("block_param")};
        _t2213 = gem_has_key_fn(NULL, _t2212, 2);
    }
    GemVal _t2215;
    if (!gem_truthy(_t2213)) {
        _t2215 = _t2213;
    } else {
        GemVal _t2214[] = {gem_v__match_31, gem_string("body")};
        _t2215 = gem_has_key_fn(NULL, _t2214, 2);
    }
    if (gem_truthy(_t2215)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_31, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_31, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_block_param = gem_table_get(gem_v__match_31, gem_string("block_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 471 "compiler/main.gem"
    GemVal _t2216[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2216, 1);
#line 472 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 473 "compiler/main.gem"
    GemVal _t2217[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2217, 2));
        }
#line 475 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_block_param, GEM_NIL))) {
#line 476 "compiler/main.gem"
    GemVal _t2218[] = {gem_v_inner_defined, gem_v_block_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2218, 2));
        }
#line 483 "compiler/main.gem"
    GemVal _t2219[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t2220 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t2220.fn(_t2220.env, _t2219, 2);
#line 484 "compiler/main.gem"
    GemVal _t2221[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn__mod_codegen_set_intersect(NULL, _t2221, 2);
        {
#line 485 "compiler/main.gem"
            GemVal gem_v__for_tbl_32 = gem_v_inter;
#line 485 "compiler/main.gem"
    GemVal _t2222[] = {gem_v__for_tbl_32};
            GemVal gem_v__for_keys_32 = gem_keys_fn(NULL, _t2222, 1);
#line 485 "compiler/main.gem"
            GemVal gem_v__for_i_32 = gem_int(0);
#line 485 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2223[] = {gem_v__for_keys_32};
                if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_len_fn(NULL, _t2223, 1)))) break;
#line 485 "compiler/main.gem"
                GemVal gem_v_ik = gem_table_get(gem_v__for_keys_32, gem_v__for_i_32);
#line 485 "compiler/main.gem"
                GemVal gem_v__ = gem_table_get(gem_v__for_tbl_32, gem_table_get(gem_v__for_keys_32, gem_v__for_i_32));
#line 485 "compiler/main.gem"
                gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 486 "compiler/main.gem"
    GemVal _t2224[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2224, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2225[] = {gem_v__match_31};
    GemVal _t2227;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2225, 1), gem_string("table")))) {
        _t2227 = gem_eq(gem_type_fn(NULL, _t2225, 1), gem_string("table"));
    } else {
        GemVal _t2226[] = {gem_v__match_31, gem_string("tag")};
        _t2227 = gem_has_key_fn(NULL, _t2226, 2);
    }
    GemVal _t2228;
    if (!gem_truthy(_t2227)) {
        _t2228 = _t2227;
    } else {
        _t2228 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2228)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t2229[] = {gem_v__match_31};
    GemVal _t2231;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2229, 1), gem_string("table")))) {
        _t2231 = gem_eq(gem_type_fn(NULL, _t2229, 1), gem_string("table"));
    } else {
        GemVal _t2230[] = {gem_v__match_31, gem_string("tag")};
        _t2231 = gem_has_key_fn(NULL, _t2230, 2);
    }
    GemVal _t2232;
    if (!gem_truthy(_t2231)) {
        _t2232 = _t2231;
    } else {
        _t2232 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2234;
    if (!gem_truthy(_t2232)) {
        _t2234 = _t2232;
    } else {
        GemVal _t2233[] = {gem_v__match_31, gem_string("cond")};
        _t2234 = gem_has_key_fn(NULL, _t2233, 2);
    }
    GemVal _t2236;
    if (!gem_truthy(_t2234)) {
        _t2236 = _t2234;
    } else {
        GemVal _t2235[] = {gem_v__match_31, gem_string("then")};
        _t2236 = gem_has_key_fn(NULL, _t2235, 2);
    }
    GemVal _t2238;
    if (!gem_truthy(_t2236)) {
        _t2238 = _t2236;
    } else {
        GemVal _t2237[] = {gem_v__match_31, gem_string("else")};
        _t2238 = gem_has_key_fn(NULL, _t2237, 2);
    }
    if (gem_truthy(_t2238)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_31, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 491 "compiler/main.gem"
    GemVal _t2239[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2240 = (*gem_v_walk_captures_node);
        (void)(_t2240.fn(_t2240.env, _t2239, 3));
#line 492 "compiler/main.gem"
    GemVal _t2241[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t2242 = (*gem_v_walk_captures);
        (void)(_t2242.fn(_t2242.env, _t2241, 3));
#line 493 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 494 "compiler/main.gem"
    GemVal _t2243[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2244 = (*gem_v_walk_captures);
            GemVal _t2245 = _t2244.fn(_t2244.env, _t2243, 3);
            gem_pop_frame();
            return _t2245;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2246[] = {gem_v__match_31};
    GemVal _t2248;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2246, 1), gem_string("table")))) {
        _t2248 = gem_eq(gem_type_fn(NULL, _t2246, 1), gem_string("table"));
    } else {
        GemVal _t2247[] = {gem_v__match_31, gem_string("tag")};
        _t2248 = gem_has_key_fn(NULL, _t2247, 2);
    }
    GemVal _t2249;
    if (!gem_truthy(_t2248)) {
        _t2249 = _t2248;
    } else {
        _t2249 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("while"));
    }
    GemVal _t2251;
    if (!gem_truthy(_t2249)) {
        _t2251 = _t2249;
    } else {
        GemVal _t2250[] = {gem_v__match_31, gem_string("cond")};
        _t2251 = gem_has_key_fn(NULL, _t2250, 2);
    }
    GemVal _t2253;
    if (!gem_truthy(_t2251)) {
        _t2253 = _t2251;
    } else {
        GemVal _t2252[] = {gem_v__match_31, gem_string("body")};
        _t2253 = gem_has_key_fn(NULL, _t2252, 2);
    }
    if (gem_truthy(_t2253)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 497 "compiler/main.gem"
    GemVal _t2254[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2255 = (*gem_v_walk_captures_node);
        (void)(_t2255.fn(_t2255.env, _t2254, 3));
#line 498 "compiler/main.gem"
    GemVal _t2256[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2257 = (*gem_v_walk_captures);
        GemVal _t2258 = _t2257.fn(_t2257.env, _t2256, 3);
        gem_pop_frame();
        return _t2258;
    } else {
    GemVal _t2259[] = {gem_v__match_31};
    GemVal _t2261;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2259, 1), gem_string("table")))) {
        _t2261 = gem_eq(gem_type_fn(NULL, _t2259, 1), gem_string("table"));
    } else {
        GemVal _t2260[] = {gem_v__match_31, gem_string("tag")};
        _t2261 = gem_has_key_fn(NULL, _t2260, 2);
    }
    GemVal _t2262;
    if (!gem_truthy(_t2261)) {
        _t2262 = _t2261;
    } else {
        _t2262 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2264;
    if (!gem_truthy(_t2262)) {
        _t2264 = _t2262;
    } else {
        GemVal _t2263[] = {gem_v__match_31, gem_string("target")};
        _t2264 = gem_has_key_fn(NULL, _t2263, 2);
    }
    GemVal _t2266;
    if (!gem_truthy(_t2264)) {
        _t2266 = _t2264;
    } else {
        GemVal _t2265[] = {gem_v__match_31, gem_string("whens")};
        _t2266 = gem_has_key_fn(NULL, _t2265, 2);
    }
    GemVal _t2268;
    if (!gem_truthy(_t2266)) {
        _t2268 = _t2266;
    } else {
        GemVal _t2267[] = {gem_v__match_31, gem_string("else")};
        _t2268 = gem_has_key_fn(NULL, _t2267, 2);
    }
    if (gem_truthy(_t2268)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_31, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_31, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 500 "compiler/main.gem"
    GemVal _t2269[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2270 = (*gem_v_walk_captures_node);
        (void)(_t2270.fn(_t2270.env, _t2269, 3));
#line 501 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 501 "compiler/main.gem"
    GemVal _t2271[] = {gem_v_whens};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t2271, 1);
#line 501 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 501 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 501 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 502 "compiler/main.gem"
    GemVal _t2272 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2273[] = {gem_table_get(_t2272, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2274 = (*gem_v_walk_captures_node);
            (void)(_t2274.fn(_t2274.env, _t2273, 3));
#line 503 "compiler/main.gem"
    GemVal _t2275 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t2275, gem_string("bindings")), GEM_NIL))) {
#line 504 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 504 "compiler/main.gem"
    GemVal _t2276 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2277[] = {gem_table_get(_t2276, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t2277, 1);
#line 504 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 504 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 504 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 505 "compiler/main.gem"
    GemVal _t2278 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2279[] = {gem_table_get(gem_table_get(_t2278, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2280 = (*gem_v_walk_captures_node);
                    (void)(_t2280.fn(_t2280.env, _t2279, 3));
                }

            }
#line 508 "compiler/main.gem"
    GemVal _t2281 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2282[] = {gem_table_get(_t2281, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2283 = (*gem_v_walk_captures);
            (void)(_t2283.fn(_t2283.env, _t2282, 3));
        }

#line 510 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 511 "compiler/main.gem"
    GemVal _t2284[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2285 = (*gem_v_walk_captures);
            GemVal _t2286 = _t2285.fn(_t2285.env, _t2284, 3);
            gem_pop_frame();
            return _t2286;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2287[] = {gem_v__match_31};
    GemVal _t2289;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2287, 1), gem_string("table")))) {
        _t2289 = gem_eq(gem_type_fn(NULL, _t2287, 1), gem_string("table"));
    } else {
        GemVal _t2288[] = {gem_v__match_31, gem_string("tag")};
        _t2289 = gem_has_key_fn(NULL, _t2288, 2);
    }
    GemVal _t2290;
    if (!gem_truthy(_t2289)) {
        _t2290 = _t2289;
    } else {
        _t2290 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2292;
    if (!gem_truthy(_t2290)) {
        _t2292 = _t2290;
    } else {
        GemVal _t2291[] = {gem_v__match_31, gem_string("arms")};
        _t2292 = gem_has_key_fn(NULL, _t2291, 2);
    }
    GemVal _t2294;
    if (!gem_truthy(_t2292)) {
        _t2294 = _t2292;
    } else {
        GemVal _t2293[] = {gem_v__match_31, gem_string("after_ms")};
        _t2294 = gem_has_key_fn(NULL, _t2293, 2);
    }
    GemVal _t2296;
    if (!gem_truthy(_t2294)) {
        _t2296 = _t2294;
    } else {
        GemVal _t2295[] = {gem_v__match_31, gem_string("after_body")};
        _t2296 = gem_has_key_fn(NULL, _t2295, 2);
    }
    if (gem_truthy(_t2296)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_31, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_31, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_31, gem_string("after_body"));
#line 514 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 514 "compiler/main.gem"
    GemVal _t2297[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2297, 1);
#line 514 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 514 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 514 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 515 "compiler/main.gem"
    GemVal _t2298 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2299 = gem_table_get(_t2298, gem_string("pattern"));
    GemVal _t2300[] = {gem_table_get(_t2299, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2301 = (*gem_v_walk_captures_node);
            (void)(_t2301.fn(_t2301.env, _t2300, 3));
#line 516 "compiler/main.gem"
    GemVal _t2302 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2303 = gem_table_get(_t2302, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2303, gem_string("bindings")), GEM_NIL))) {
#line 517 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 517 "compiler/main.gem"
    GemVal _t2304 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2305 = gem_table_get(_t2304, gem_string("pattern"));
    GemVal _t2306[] = {gem_table_get(_t2305, gem_string("bindings"))};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2306, 1);
#line 517 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 517 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 517 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 518 "compiler/main.gem"
    GemVal _t2307 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2308 = gem_table_get(_t2307, gem_string("pattern"));
    GemVal _t2309[] = {gem_table_get(gem_table_get(_t2308, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2310 = (*gem_v_walk_captures_node);
                    (void)(_t2310.fn(_t2310.env, _t2309, 3));
                }

            }
#line 521 "compiler/main.gem"
    GemVal _t2311 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2312[] = {gem_table_get(_t2311, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2313 = (*gem_v_walk_captures);
            (void)(_t2313.fn(_t2313.env, _t2312, 3));
        }

#line 523 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 524 "compiler/main.gem"
    GemVal _t2314[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2315 = (*gem_v_walk_captures_node);
            (void)(_t2315.fn(_t2315.env, _t2314, 3));
        }
#line 526 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 527 "compiler/main.gem"
    GemVal _t2316[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2317 = (*gem_v_walk_captures);
            GemVal _t2318 = _t2317.fn(_t2317.env, _t2316, 3);
            gem_pop_frame();
            return _t2318;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2319[] = {gem_v__match_31};
    GemVal _t2321;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2319, 1), gem_string("table")))) {
        _t2321 = gem_eq(gem_type_fn(NULL, _t2319, 1), gem_string("table"));
    } else {
        GemVal _t2320[] = {gem_v__match_31, gem_string("tag")};
        _t2321 = gem_has_key_fn(NULL, _t2320, 2);
    }
    GemVal _t2322;
    if (!gem_truthy(_t2321)) {
        _t2322 = _t2321;
    } else {
        _t2322 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2324;
    if (!gem_truthy(_t2322)) {
        _t2324 = _t2322;
    } else {
        GemVal _t2323[] = {gem_v__match_31, gem_string("entries")};
        _t2324 = gem_has_key_fn(NULL, _t2323, 2);
    }
    if (gem_truthy(_t2324)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_31, gem_string("entries"));
        {
#line 530 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 530 "compiler/main.gem"
    GemVal _t2325[] = {gem_v_entries};
            GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2325, 1);
#line 530 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 530 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 530 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 531 "compiler/main.gem"
    GemVal _t2326 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2327[] = {gem_table_get(_t2326, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2328 = (*gem_v_walk_captures_node);
                (void)(_t2328.fn(_t2328.env, _t2327, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2329[] = {gem_v__match_31};
    GemVal _t2331;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2329, 1), gem_string("table")))) {
        _t2331 = gem_eq(gem_type_fn(NULL, _t2329, 1), gem_string("table"));
    } else {
        GemVal _t2330[] = {gem_v__match_31, gem_string("tag")};
        _t2331 = gem_has_key_fn(NULL, _t2330, 2);
    }
    GemVal _t2332;
    if (!gem_truthy(_t2331)) {
        _t2332 = _t2331;
    } else {
        _t2332 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2334;
    if (!gem_truthy(_t2332)) {
        _t2334 = _t2332;
    } else {
        GemVal _t2333[] = {gem_v__match_31, gem_string("elements")};
        _t2334 = gem_has_key_fn(NULL, _t2333, 2);
    }
    if (gem_truthy(_t2334)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_31, gem_string("elements"));
        {
#line 534 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 534 "compiler/main.gem"
    GemVal _t2335[] = {gem_v_elements};
            GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2335, 1);
#line 534 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 534 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 534 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 535 "compiler/main.gem"
    GemVal _t2336[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2337 = (*gem_v_walk_captures_node);
                (void)(_t2337.fn(_t2337.env, _t2336, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2338[] = {gem_v__match_31};
    GemVal _t2340;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2338, 1), gem_string("table")))) {
        _t2340 = gem_eq(gem_type_fn(NULL, _t2338, 1), gem_string("table"));
    } else {
        GemVal _t2339[] = {gem_v__match_31, gem_string("tag")};
        _t2340 = gem_has_key_fn(NULL, _t2339, 2);
    }
    GemVal _t2341;
    if (!gem_truthy(_t2340)) {
        _t2341 = _t2340;
    } else {
        _t2341 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2343;
    if (!gem_truthy(_t2341)) {
        _t2343 = _t2341;
    } else {
        GemVal _t2342[] = {gem_v__match_31, gem_string("stmts")};
        _t2343 = gem_has_key_fn(NULL, _t2342, 2);
    }
    if (gem_truthy(_t2343)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_31, gem_string("stmts"));
#line 538 "compiler/main.gem"
    GemVal _t2344[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2345 = (*gem_v_walk_captures);
        GemVal _t2346 = _t2345.fn(_t2345.env, _t2344, 3);
        gem_pop_frame();
        return _t2346;
    } else {
    GemVal _t2347[] = {gem_v__match_31};
    GemVal _t2349;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2347, 1), gem_string("table")))) {
        _t2349 = gem_eq(gem_type_fn(NULL, _t2347, 1), gem_string("table"));
    } else {
        GemVal _t2348[] = {gem_v__match_31, gem_string("tag")};
        _t2349 = gem_has_key_fn(NULL, _t2348, 2);
    }
    GemVal _t2350;
    if (!gem_truthy(_t2349)) {
        _t2350 = _t2349;
    } else {
        _t2350 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2352;
    if (!gem_truthy(_t2350)) {
        _t2352 = _t2350;
    } else {
        GemVal _t2351[] = {gem_v__match_31, gem_string("value")};
        _t2352 = gem_has_key_fn(NULL, _t2351, 2);
    }
    if (gem_truthy(_t2352)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 540 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 541 "compiler/main.gem"
    GemVal _t2353[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2354 = (*gem_v_walk_captures_node);
            GemVal _t2355 = _t2354.fn(_t2354.env, _t2353, 3);
            gem_pop_frame();
            return _t2355;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2356[] = {gem_v__match_31};
    GemVal _t2358;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2356, 1), gem_string("table")))) {
        _t2358 = gem_eq(gem_type_fn(NULL, _t2356, 1), gem_string("table"));
    } else {
        GemVal _t2357[] = {gem_v__match_31, gem_string("tag")};
        _t2358 = gem_has_key_fn(NULL, _t2357, 2);
    }
    GemVal _t2359;
    if (!gem_truthy(_t2358)) {
        _t2359 = _t2358;
    } else {
        _t2359 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2361;
    if (!gem_truthy(_t2359)) {
        _t2361 = _t2359;
    } else {
        GemVal _t2360[] = {gem_v__match_31, gem_string("value")};
        _t2361 = gem_has_key_fn(NULL, _t2360, 2);
    }
    if (gem_truthy(_t2361)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 544 "compiler/main.gem"
    GemVal _t2362[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2363 = (*gem_v_walk_captures_node);
        GemVal _t2364 = _t2363.fn(_t2363.env, _t2362, 3);
        gem_pop_frame();
        return _t2364;
    } else {
    GemVal _t2365[] = {gem_v__match_31};
    GemVal _t2367;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2365, 1), gem_string("table")))) {
        _t2367 = gem_eq(gem_type_fn(NULL, _t2365, 1), gem_string("table"));
    } else {
        GemVal _t2366[] = {gem_v__match_31, gem_string("tag")};
        _t2367 = gem_has_key_fn(NULL, _t2366, 2);
    }
    GemVal _t2368;
    if (!gem_truthy(_t2367)) {
        _t2368 = _t2367;
    } else {
        _t2368 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2370;
    if (!gem_truthy(_t2368)) {
        _t2370 = _t2368;
    } else {
        GemVal _t2369[] = {gem_v__match_31, gem_string("value")};
        _t2370 = gem_has_key_fn(NULL, _t2369, 2);
    }
    if (gem_truthy(_t2370)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 546 "compiler/main.gem"
    GemVal _t2371[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2372 = (*gem_v_walk_captures_node);
        GemVal _t2373 = _t2372.fn(_t2372.env, _t2371, 3);
        gem_pop_frame();
        return _t2373;
    } else {
        {
#line 549 "compiler/main.gem"
            GemVal gem_v__for_tbl_39 = gem_v_node;
#line 549 "compiler/main.gem"
    GemVal _t2374[] = {gem_v__for_tbl_39};
            GemVal gem_v__for_keys_39 = gem_keys_fn(NULL, _t2374, 1);
#line 549 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 549 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2375[] = {gem_v__for_keys_39};
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_len_fn(NULL, _t2375, 1)))) break;
#line 549 "compiler/main.gem"
                GemVal gem_v_nk = gem_table_get(gem_v__for_keys_39, gem_v__for_i_39);
#line 549 "compiler/main.gem"
                GemVal gem_v_v = gem_table_get(gem_v__for_tbl_39, gem_table_get(gem_v__for_keys_39, gem_v__for_i_39));
#line 549 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 550 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 551 "compiler/main.gem"
    GemVal _t2376[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2376, 1), gem_string("table")))) {
#line 552 "compiler/main.gem"
    GemVal _t2377[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2377, 1);
#line 553 "compiler/main.gem"
    GemVal _t2378[] = {gem_v_vks};
    GemVal _t2380;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2378, 1), gem_int(0)))) {
        _t2380 = gem_gt(gem_len_fn(NULL, _t2378, 1), gem_int(0));
    } else {
        GemVal _t2379[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2380 = gem_eq(gem_type_fn(NULL, _t2379, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2380)) {
#line 554 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 555 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2381[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2381, 1)))) break;
#line 556 "compiler/main.gem"
    GemVal _t2382[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2382, 1), gem_string("table")))) {
#line 557 "compiler/main.gem"
    GemVal _t2383[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2384 = (*gem_v_walk_captures_node);
                                    (void)(_t2384.fn(_t2384.env, _t2383, 3));
                                }
#line 559 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 562 "compiler/main.gem"
    GemVal _t2385[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2386 = (*gem_v_walk_captures_node);
                            (void)(_t2386.fn(_t2386.env, _t2385, 3));
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
#line 571 "compiler/main.gem"
    GemVal _t2388 = gem_table_new();
    GemVal _t2389[] = {gem_v_scope_vars, _t2388};
    GemVal gem_v_sv = gem_fn__mod_codegen_set_union(NULL, _t2389, 2);
    {
#line 572 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 572 "compiler/main.gem"
    GemVal _t2390[] = {gem_v_stmts};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2390, 1);
#line 572 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 572 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 572 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 573 "compiler/main.gem"
    GemVal _t2391[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2392 = (*gem_v_walk_captures_node);
            (void)(_t2392.fn(_t2392.env, _t2391, 3));
#line 574 "compiler/main.gem"
    GemVal _t2393[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2393, 2))) {
#line 575 "compiler/main.gem"
    GemVal _t2394 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2395[] = {gem_v_sv, gem_table_get(_t2394, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2395, 2));
            }
#line 577 "compiler/main.gem"
    GemVal _t2396[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2396, 2));
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
#line 582 "compiler/main.gem"
    GemVal _t2398 = gem_table_new();
    GemVal gem_v_captured = _t2398;
#line 583 "compiler/main.gem"
    GemVal _t2399[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2400 = (*gem_v_walk_captures);
    (void)(_t2400.fn(_t2400.env, _t2399, 3));
    GemVal _t2401 = gem_v_captured;
    gem_pop_frame();
    return _t2401;
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
#line 614 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 615 "compiler/main.gem"
        GemVal _t2403 = gem_bool(0);
        gem_pop_frame();
        return _t2403;
    }
#line 617 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_node};
    GemVal _t2406;
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2404, 1), gem_string("table")))) {
        _t2406 = gem_neq(gem_type_fn(NULL, _t2404, 1), gem_string("table"));
    } else {
        GemVal _t2405 = gem_v_node;
        _t2406 = gem_eq(gem_table_get(_t2405, gem_string("tag")), GEM_NIL);
    }
    if (gem_truthy(_t2406)) {
#line 618 "compiler/main.gem"
        GemVal _t2407 = gem_bool(0);
        gem_pop_frame();
        return _t2407;
    }
#line 620 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2408[] = {gem_v__match_41};
    GemVal _t2410;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2408, 1), gem_string("table")))) {
        _t2410 = gem_eq(gem_type_fn(NULL, _t2408, 1), gem_string("table"));
    } else {
        GemVal _t2409[] = {gem_v__match_41, gem_string("tag")};
        _t2410 = gem_has_key_fn(NULL, _t2409, 2);
    }
    GemVal _t2411;
    if (!gem_truthy(_t2410)) {
        _t2411 = _t2410;
    } else {
        _t2411 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    GemVal _t2413;
    if (!gem_truthy(_t2411)) {
        _t2413 = _t2411;
    } else {
        GemVal _t2412[] = {gem_v__match_41, gem_string("func")};
        _t2413 = gem_has_key_fn(NULL, _t2412, 2);
    }
    if (gem_truthy(_t2413)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_func = gem_table_get(gem_v__match_41, gem_string("func"));
#line 622 "compiler/main.gem"
    GemVal _t2414[] = {gem_v_func, gem_string("var")};
    GemVal _t2416;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2414, 2))) {
        _t2416 = gem_fn__mod_codegen_is_node(NULL, _t2414, 2);
    } else {
        GemVal _t2415 = gem_v_func;
        _t2416 = gem_eq(gem_table_get(_t2415, gem_string("name")), gem_v_fn_name);
    }
        GemVal _t2417 = _t2416;
        gem_pop_frame();
        return _t2417;
    } else {
    GemVal _t2418[] = {gem_v__match_41};
    GemVal _t2420;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2418, 1), gem_string("table")))) {
        _t2420 = gem_eq(gem_type_fn(NULL, _t2418, 1), gem_string("table"));
    } else {
        GemVal _t2419[] = {gem_v__match_41, gem_string("tag")};
        _t2420 = gem_has_key_fn(NULL, _t2419, 2);
    }
    GemVal _t2421;
    if (!gem_truthy(_t2420)) {
        _t2421 = _t2420;
    } else {
        _t2421 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("return"));
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
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 624 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 625 "compiler/main.gem"
    GemVal _t2424[] = {gem_v_fn_name, gem_v_value};
    GemVal _t2425 = (*gem_v_is_self_tail_call);
            GemVal _t2426 = _t2425.fn(_t2425.env, _t2424, 2);
            gem_pop_frame();
            return _t2426;
        }
#line 627 "compiler/main.gem"
        GemVal _t2427 = gem_bool(0);
        gem_pop_frame();
        return _t2427;
    } else {
    GemVal _t2428[] = {gem_v__match_41};
    GemVal _t2430;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2428, 1), gem_string("table")))) {
        _t2430 = gem_eq(gem_type_fn(NULL, _t2428, 1), gem_string("table"));
    } else {
        GemVal _t2429[] = {gem_v__match_41, gem_string("tag")};
        _t2430 = gem_has_key_fn(NULL, _t2429, 2);
    }
    GemVal _t2431;
    if (!gem_truthy(_t2430)) {
        _t2431 = _t2430;
    } else {
        _t2431 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2433;
    if (!gem_truthy(_t2431)) {
        _t2433 = _t2431;
    } else {
        GemVal _t2432[] = {gem_v__match_41, gem_string("then")};
        _t2433 = gem_has_key_fn(NULL, _t2432, 2);
    }
    GemVal _t2435;
    if (!gem_truthy(_t2433)) {
        _t2435 = _t2433;
    } else {
        GemVal _t2434[] = {gem_v__match_41, gem_string("else")};
        _t2435 = gem_has_key_fn(NULL, _t2434, 2);
    }
    if (gem_truthy(_t2435)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_then_body = gem_table_get(gem_v__match_41, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 629 "compiler/main.gem"
    GemVal _t2436[] = {gem_v_fn_name, gem_v_then_body};
    GemVal _t2437 = (*gem_v_has_self_tail_call);
        if (gem_truthy(_t2437.fn(_t2437.env, _t2436, 2))) {
#line 630 "compiler/main.gem"
            GemVal _t2438 = gem_bool(1);
            gem_pop_frame();
            return _t2438;
        }
#line 632 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 633 "compiler/main.gem"
    GemVal _t2439[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2440 = (*gem_v_has_self_tail_call);
            GemVal _t2441 = _t2440.fn(_t2440.env, _t2439, 2);
            gem_pop_frame();
            return _t2441;
        }
#line 635 "compiler/main.gem"
        GemVal _t2442 = gem_bool(0);
        gem_pop_frame();
        return _t2442;
    } else {
    GemVal _t2443[] = {gem_v__match_41};
    GemVal _t2445;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2443, 1), gem_string("table")))) {
        _t2445 = gem_eq(gem_type_fn(NULL, _t2443, 1), gem_string("table"));
    } else {
        GemVal _t2444[] = {gem_v__match_41, gem_string("tag")};
        _t2445 = gem_has_key_fn(NULL, _t2444, 2);
    }
    GemVal _t2446;
    if (!gem_truthy(_t2445)) {
        _t2446 = _t2445;
    } else {
        _t2446 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2448;
    if (!gem_truthy(_t2446)) {
        _t2448 = _t2446;
    } else {
        GemVal _t2447[] = {gem_v__match_41, gem_string("whens")};
        _t2448 = gem_has_key_fn(NULL, _t2447, 2);
    }
    GemVal _t2450;
    if (!gem_truthy(_t2448)) {
        _t2450 = _t2448;
    } else {
        GemVal _t2449[] = {gem_v__match_41, gem_string("else")};
        _t2450 = gem_has_key_fn(NULL, _t2449, 2);
    }
    if (gem_truthy(_t2450)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_41, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 637 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 637 "compiler/main.gem"
    GemVal _t2451[] = {gem_v_whens};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2451, 1);
#line 637 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 637 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 637 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 638 "compiler/main.gem"
    GemVal _t2452 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2453[] = {gem_v_fn_name, gem_table_get(_t2452, gem_string("body"))};
    GemVal _t2454 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2454.fn(_t2454.env, _t2453, 2))) {
#line 639 "compiler/main.gem"
                GemVal _t2455 = gem_bool(1);
                gem_pop_frame();
                return _t2455;
            }
        }

#line 642 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 643 "compiler/main.gem"
    GemVal _t2456[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2457 = (*gem_v_has_self_tail_call);
            GemVal _t2458 = _t2457.fn(_t2457.env, _t2456, 2);
            gem_pop_frame();
            return _t2458;
        }
#line 645 "compiler/main.gem"
        GemVal _t2459 = gem_bool(0);
        gem_pop_frame();
        return _t2459;
    } else {
    GemVal _t2460[] = {gem_v__match_41};
    GemVal _t2462;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2460, 1), gem_string("table")))) {
        _t2462 = gem_eq(gem_type_fn(NULL, _t2460, 1), gem_string("table"));
    } else {
        GemVal _t2461[] = {gem_v__match_41, gem_string("tag")};
        _t2462 = gem_has_key_fn(NULL, _t2461, 2);
    }
    GemVal _t2463;
    if (!gem_truthy(_t2462)) {
        _t2463 = _t2462;
    } else {
        _t2463 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2465;
    if (!gem_truthy(_t2463)) {
        _t2465 = _t2463;
    } else {
        GemVal _t2464[] = {gem_v__match_41, gem_string("arms")};
        _t2465 = gem_has_key_fn(NULL, _t2464, 2);
    }
    GemVal _t2467;
    if (!gem_truthy(_t2465)) {
        _t2467 = _t2465;
    } else {
        GemVal _t2466[] = {gem_v__match_41, gem_string("after_body")};
        _t2467 = gem_has_key_fn(NULL, _t2466, 2);
    }
    if (gem_truthy(_t2467)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_arms = gem_table_get(gem_v__match_41, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_after_body = gem_table_get(gem_v__match_41, gem_string("after_body"));
#line 647 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(0);
#line 647 "compiler/main.gem"
    GemVal _t2468[] = {gem_v_arms};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2468, 1);
#line 647 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 647 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 647 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 648 "compiler/main.gem"
    GemVal _t2469 = gem_table_get(gem_v_arms, gem_v_i);
    GemVal _t2470[] = {gem_v_fn_name, gem_table_get(_t2469, gem_string("body"))};
    GemVal _t2471 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2471.fn(_t2471.env, _t2470, 2))) {
#line 649 "compiler/main.gem"
                GemVal _t2472 = gem_bool(1);
                gem_pop_frame();
                return _t2472;
            }
        }

#line 652 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 653 "compiler/main.gem"
    GemVal _t2473[] = {gem_v_fn_name, gem_v_after_body};
    GemVal _t2474 = (*gem_v_has_self_tail_call);
            GemVal _t2475 = _t2474.fn(_t2474.env, _t2473, 2);
            gem_pop_frame();
            return _t2475;
        }
#line 655 "compiler/main.gem"
        GemVal _t2476 = gem_bool(0);
        gem_pop_frame();
        return _t2476;
    } else {
    GemVal _t2477[] = {gem_v__match_41};
    GemVal _t2479;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2477, 1), gem_string("table")))) {
        _t2479 = gem_eq(gem_type_fn(NULL, _t2477, 1), gem_string("table"));
    } else {
        GemVal _t2478[] = {gem_v__match_41, gem_string("tag")};
        _t2479 = gem_has_key_fn(NULL, _t2478, 2);
    }
    GemVal _t2480;
    if (!gem_truthy(_t2479)) {
        _t2480 = _t2479;
    } else {
        _t2480 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2482;
    if (!gem_truthy(_t2480)) {
        _t2482 = _t2480;
    } else {
        GemVal _t2481[] = {gem_v__match_41, gem_string("stmts")};
        _t2482 = gem_has_key_fn(NULL, _t2481, 2);
    }
    if (gem_truthy(_t2482)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_41, gem_string("stmts"));
#line 657 "compiler/main.gem"
    GemVal _t2483[] = {gem_v_fn_name, gem_v_stmts};
    GemVal _t2484 = (*gem_v_has_self_tail_call);
        GemVal _t2485 = _t2484.fn(_t2484.env, _t2483, 2);
        gem_pop_frame();
        return _t2485;
    }
    }
    }
    }
    }
    }
#line 659 "compiler/main.gem"
    GemVal _t2486 = gem_bool(0);
    gem_pop_frame();
    return _t2486;
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
#line 663 "compiler/main.gem"
    GemVal _t2488[] = {gem_v_stmts};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2488, 1), gem_int(0)))) {
#line 664 "compiler/main.gem"
        GemVal _t2489 = gem_bool(0);
        gem_pop_frame();
        return _t2489;
    }
#line 666 "compiler/main.gem"
    GemVal _t2490[] = {gem_v_fn_name, gem_table_get(gem_v_stmts, gem_neg(gem_int(1)))};
    GemVal _t2491 = (*gem_v_is_self_tail_call);
    GemVal _t2492 = _t2491.fn(_t2491.env, _t2490, 2);
    gem_pop_frame();
    return _t2492;
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
#line 670 "compiler/main.gem"
    GemVal _t2494[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t2494, 1);
#line 671 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 672 "compiler/main.gem"
    GemVal _t2495 = gem_v_call_node;
    GemVal gem_v_args = gem_table_get(_t2495, gem_string("args"));
#line 673 "compiler/main.gem"
    GemVal _t2496 = gem_table_new();
    GemVal gem_v_arg_temps = _t2496;
#line 674 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 674 "compiler/main.gem"
    GemVal _t2497[] = {gem_v_args};
    GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2497, 1);
#line 674 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 674 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_44;
#line 674 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 675 "compiler/main.gem"
    GemVal _t2498[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2499 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2499.fn(_t2499.env, _t2498, 1);
#line 676 "compiler/main.gem"
    GemVal _t2500 = (*gem_v_tmp);
        GemVal gem_v_t = _t2500.fn(_t2500.env, NULL, 0);
#line 677 "compiler/main.gem"
    GemVal _t2501 = gem_v_r;
    GemVal _t2502[] = {gem_v_b, gem_table_get(_t2501, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2502, 2));
#line 678 "compiler/main.gem"
    GemVal _t2503[] = {gem_v_p};
    GemVal _t2504[] = {gem_v_t};
    GemVal _t2505 = gem_v_r;
    GemVal _t2506[] = {gem_table_get(_t2505, gem_string("expr"))};
    GemVal _t2507[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2503, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2504, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2506, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t2507, 2));
#line 679 "compiler/main.gem"
    GemVal _t2508[] = {gem_v_arg_temps, gem_v_t};
        (void)(gem_push_fn(NULL, _t2508, 2));
    }

#line 681 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 681 "compiler/main.gem"
    GemVal _t2509[] = {(*gem_v_tco_params)};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2509, 1);
#line 681 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 681 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 681 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 682 "compiler/main.gem"
    GemVal _t2510[] = {gem_table_get((*gem_v_tco_params), gem_v_i)};
    GemVal _t2511 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2511.fn(_t2511.env, _t2510, 1);
#line 683 "compiler/main.gem"
        GemVal gem_v_val = gem_string("GEM_NIL");
#line 684 "compiler/main.gem"
    GemVal _t2512[] = {gem_v_arg_temps};
        if (gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t2512, 1)))) {
#line 685 "compiler/main.gem"
            gem_v_val = gem_table_get(gem_v_arg_temps, gem_v_i);
        }
#line 687 "compiler/main.gem"
    GemVal _t2513[] = {(*gem_v_tco_captured), gem_table_get((*gem_v_tco_params), gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2513, 2))) {
#line 688 "compiler/main.gem"
    GemVal _t2514[] = {gem_v_p};
    GemVal _t2515[] = {gem_v_mp};
    GemVal _t2516[] = {gem_v_val};
    GemVal _t2517[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2514, 1), gem_string("*")), gem_to_string_fn(NULL, _t2515, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2516, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2517, 2));
        } else {
#line 690 "compiler/main.gem"
    GemVal _t2518[] = {gem_v_p};
    GemVal _t2519[] = {gem_v_mp};
    GemVal _t2520[] = {gem_v_val};
    GemVal _t2521[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2518, 1), gem_to_string_fn(NULL, _t2519, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2520, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2521, 2));
        }
    }

#line 693 "compiler/main.gem"
    GemVal _t2522[] = {gem_v_p};
    GemVal _t2523[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t2522, 1), gem_string("continue;"))};
    (void)(gem_buf_push_fn(NULL, _t2523, 2));
#line 694 "compiler/main.gem"
    GemVal _t2524[] = {gem_v_b};
    GemVal _t2525 = gem_buf_str_fn(NULL, _t2524, 1);
    gem_pop_frame();
    return _t2525;
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
#line 700 "compiler/main.gem"
    GemVal gem_v__match_46 = gem_v_node;
    GemVal _t2527[] = {gem_v__match_46};
    GemVal _t2529;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2527, 1), gem_string("table")))) {
        _t2529 = gem_eq(gem_type_fn(NULL, _t2527, 1), gem_string("table"));
    } else {
        GemVal _t2528[] = {gem_v__match_46, gem_string("tag")};
        _t2529 = gem_has_key_fn(NULL, _t2528, 2);
    }
    GemVal _t2530;
    if (!gem_truthy(_t2529)) {
        _t2530 = _t2529;
    } else {
        _t2530 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2532;
    if (!gem_truthy(_t2530)) {
        _t2532 = _t2530;
    } else {
        GemVal _t2531[] = {gem_v__match_46, gem_string("value")};
        _t2532 = gem_has_key_fn(NULL, _t2531, 2);
    }
    if (gem_truthy(_t2532)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 702 "compiler/main.gem"
    GemVal _t2533 = gem_table_new();
    GemVal _t2534[] = {gem_v_value};
    gem_table_set(_t2533, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t2534, 1)), gem_string(")")));
    gem_table_set(_t2533, gem_string("setup"), gem_string(""));
        GemVal _t2535 = _t2533;
        gem_pop_frame();
        return _t2535;
    } else {
    GemVal _t2536[] = {gem_v__match_46};
    GemVal _t2538;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2536, 1), gem_string("table")))) {
        _t2538 = gem_eq(gem_type_fn(NULL, _t2536, 1), gem_string("table"));
    } else {
        GemVal _t2537[] = {gem_v__match_46, gem_string("tag")};
        _t2538 = gem_has_key_fn(NULL, _t2537, 2);
    }
    GemVal _t2539;
    if (!gem_truthy(_t2538)) {
        _t2539 = _t2538;
    } else {
        _t2539 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2541;
    if (!gem_truthy(_t2539)) {
        _t2541 = _t2539;
    } else {
        GemVal _t2540[] = {gem_v__match_46, gem_string("value")};
        _t2541 = gem_has_key_fn(NULL, _t2540, 2);
    }
    if (gem_truthy(_t2541)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 704 "compiler/main.gem"
    GemVal _t2542 = gem_table_new();
    GemVal _t2543[] = {gem_v_value};
    GemVal _t2544[] = {gem_fn__mod_codegen_format_float(NULL, _t2543, 1)};
    gem_table_set(_t2542, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t2544, 1)), gem_string(")")));
    gem_table_set(_t2542, gem_string("setup"), gem_string(""));
        GemVal _t2545 = _t2542;
        gem_pop_frame();
        return _t2545;
    } else {
    GemVal _t2546[] = {gem_v__match_46};
    GemVal _t2548;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2546, 1), gem_string("table")))) {
        _t2548 = gem_eq(gem_type_fn(NULL, _t2546, 1), gem_string("table"));
    } else {
        GemVal _t2547[] = {gem_v__match_46, gem_string("tag")};
        _t2548 = gem_has_key_fn(NULL, _t2547, 2);
    }
    GemVal _t2549;
    if (!gem_truthy(_t2548)) {
        _t2549 = _t2548;
    } else {
        _t2549 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2551;
    if (!gem_truthy(_t2549)) {
        _t2551 = _t2549;
    } else {
        GemVal _t2550[] = {gem_v__match_46, gem_string("value")};
        _t2551 = gem_has_key_fn(NULL, _t2550, 2);
    }
    if (gem_truthy(_t2551)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 706 "compiler/main.gem"
    GemVal _t2552[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2552, 1);
#line 707 "compiler/main.gem"
    GemVal _t2553 = gem_table_new();
    GemVal _t2554[] = {gem_v_escaped};
    gem_table_set(_t2553, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t2554, 1)), gem_string("\")")));
    gem_table_set(_t2553, gem_string("setup"), gem_string(""));
        GemVal _t2555 = _t2553;
        gem_pop_frame();
        return _t2555;
    } else {
    GemVal _t2556[] = {gem_v__match_46};
    GemVal _t2558;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2556, 1), gem_string("table")))) {
        _t2558 = gem_eq(gem_type_fn(NULL, _t2556, 1), gem_string("table"));
    } else {
        GemVal _t2557[] = {gem_v__match_46, gem_string("tag")};
        _t2558 = gem_has_key_fn(NULL, _t2557, 2);
    }
    GemVal _t2559;
    if (!gem_truthy(_t2558)) {
        _t2559 = _t2558;
    } else {
        _t2559 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2561;
    if (!gem_truthy(_t2559)) {
        _t2561 = _t2559;
    } else {
        GemVal _t2560[] = {gem_v__match_46, gem_string("parts")};
        _t2561 = gem_has_key_fn(NULL, _t2560, 2);
    }
    if (gem_truthy(_t2561)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_46, gem_string("parts"));
#line 709 "compiler/main.gem"
    GemVal _t2562[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2562, 1), gem_int(0)))) {
#line 710 "compiler/main.gem"
    GemVal _t2563 = gem_table_new();
    gem_table_set(_t2563, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2563, gem_string("setup"), gem_string(""));
            GemVal _t2564 = _t2563;
            gem_pop_frame();
            return _t2564;
        }
#line 712 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 713 "compiler/main.gem"
    GemVal _t2565 = gem_table_new();
        GemVal gem_v_compiled = _t2565;
#line 714 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 714 "compiler/main.gem"
    GemVal _t2566[] = {gem_v_parts};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2566, 1);
#line 714 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 714 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_47;
#line 714 "compiler/main.gem"
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 715 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 716 "compiler/main.gem"
    GemVal _t2567[] = {gem_v_part};
    GemVal _t2568 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2568.fn(_t2568.env, _t2567, 1);
#line 717 "compiler/main.gem"
    GemVal _t2569 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2569, gem_string("setup")));
#line 718 "compiler/main.gem"
    GemVal _t2570 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t2570, gem_string("tag")), gem_string("string")))) {
#line 719 "compiler/main.gem"
    GemVal _t2571 = gem_v_r;
    GemVal _t2572[] = {gem_v_compiled, gem_table_get(_t2571, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t2572, 2));
            } else {
#line 721 "compiler/main.gem"
    GemVal _t2573 = (*gem_v_tmp);
                GemVal gem_v_ts = _t2573.fn(_t2573.env, NULL, 0);
#line 722 "compiler/main.gem"
    GemVal _t2574[] = {gem_v_ts};
    GemVal _t2575 = gem_v_r;
    GemVal _t2576[] = {gem_table_get(_t2575, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2574, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2576, 1)), gem_string("};\n")));
#line 723 "compiler/main.gem"
    GemVal _t2577[] = {gem_v_ts};
    GemVal _t2578[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t2577, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t2578, 2));
            }
        }

#line 726 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 727 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(1);
#line 727 "compiler/main.gem"
    GemVal _t2579[] = {gem_v_compiled};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2579, 1);
#line 727 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 727 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 727 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 728 "compiler/main.gem"
    GemVal _t2580[] = {gem_v_acc};
    GemVal _t2581[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2580, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2581, 1)), gem_string(")"));
        }

#line 730 "compiler/main.gem"
    GemVal _t2582 = gem_table_new();
    gem_table_set(_t2582, gem_string("expr"), gem_v_acc);
    gem_table_set(_t2582, gem_string("setup"), gem_v_setup);
        GemVal _t2583 = _t2582;
        gem_pop_frame();
        return _t2583;
    } else {
    GemVal _t2584[] = {gem_v__match_46};
    GemVal _t2586;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2584, 1), gem_string("table")))) {
        _t2586 = gem_eq(gem_type_fn(NULL, _t2584, 1), gem_string("table"));
    } else {
        GemVal _t2585[] = {gem_v__match_46, gem_string("tag")};
        _t2586 = gem_has_key_fn(NULL, _t2585, 2);
    }
    GemVal _t2587;
    if (!gem_truthy(_t2586)) {
        _t2587 = _t2586;
    } else {
        _t2587 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2589;
    if (!gem_truthy(_t2587)) {
        _t2589 = _t2587;
    } else {
        GemVal _t2588[] = {gem_v__match_46, gem_string("value")};
        _t2589 = gem_has_key_fn(NULL, _t2588, 2);
    }
    if (gem_truthy(_t2589)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 732 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 733 "compiler/main.gem"
    GemVal _t2590 = gem_table_new();
    gem_table_set(_t2590, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2590, gem_string("setup"), gem_string(""));
            GemVal _t2591 = _t2590;
            gem_pop_frame();
            return _t2591;
        }
#line 735 "compiler/main.gem"
    GemVal _t2592 = gem_table_new();
    gem_table_set(_t2592, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2592, gem_string("setup"), gem_string(""));
        GemVal _t2593 = _t2592;
        gem_pop_frame();
        return _t2593;
    } else {
    GemVal _t2594[] = {gem_v__match_46};
    GemVal _t2596;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2594, 1), gem_string("table")))) {
        _t2596 = gem_eq(gem_type_fn(NULL, _t2594, 1), gem_string("table"));
    } else {
        GemVal _t2595[] = {gem_v__match_46, gem_string("tag")};
        _t2596 = gem_has_key_fn(NULL, _t2595, 2);
    }
    GemVal _t2597;
    if (!gem_truthy(_t2596)) {
        _t2597 = _t2596;
    } else {
        _t2597 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2597)) {
#line 737 "compiler/main.gem"
    GemVal _t2598 = gem_table_new();
    gem_table_set(_t2598, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2598, gem_string("setup"), gem_string(""));
        GemVal _t2599 = _t2598;
        gem_pop_frame();
        return _t2599;
    } else {
    GemVal _t2600[] = {gem_v__match_46};
    GemVal _t2602;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2600, 1), gem_string("table")))) {
        _t2602 = gem_eq(gem_type_fn(NULL, _t2600, 1), gem_string("table"));
    } else {
        GemVal _t2601[] = {gem_v__match_46, gem_string("tag")};
        _t2602 = gem_has_key_fn(NULL, _t2601, 2);
    }
    GemVal _t2603;
    if (!gem_truthy(_t2602)) {
        _t2603 = _t2602;
    } else {
        _t2603 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2605;
    if (!gem_truthy(_t2603)) {
        _t2605 = _t2603;
    } else {
        GemVal _t2604[] = {gem_v__match_46, gem_string("name")};
        _t2605 = gem_has_key_fn(NULL, _t2604, 2);
    }
    if (gem_truthy(_t2605)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_46, gem_string("name"));
#line 739 "compiler/main.gem"
    GemVal _t2606[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2606, 2)))) {
#line 740 "compiler/main.gem"
    GemVal _t2607[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2607, 2))) {
#line 741 "compiler/main.gem"
    GemVal _t2608 = gem_table_new();
    GemVal _t2609[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t2608, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2609, 1)), gem_string(", NULL)")));
    gem_table_set(_t2608, gem_string("setup"), gem_string(""));
                GemVal _t2610 = _t2608;
                gem_pop_frame();
                return _t2610;
            } else {
#line 742 "compiler/main.gem"
    GemVal _t2611[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2611, 2))) {
#line 743 "compiler/main.gem"
    GemVal _t2612 = gem_table_new();
    GemVal _t2613[] = {gem_v_name};
    gem_table_set(_t2612, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t2613, 1)), gem_string(", NULL)")));
    gem_table_set(_t2612, gem_string("setup"), gem_string(""));
                    GemVal _t2614 = _t2612;
                    gem_pop_frame();
                    return _t2614;
                }
            }
        }
#line 746 "compiler/main.gem"
    GemVal _t2615[] = {gem_v_name};
    GemVal _t2616 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2616.fn(_t2616.env, _t2615, 1);
#line 747 "compiler/main.gem"
    GemVal _t2617[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2619;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2617, 2))) {
        _t2619 = gem_fn__mod_codegen_set_contains(NULL, _t2617, 2);
    } else {
        GemVal _t2618[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2619 = gem_fn__mod_codegen_set_contains(NULL, _t2618, 2);
    }
        if (gem_truthy(_t2619)) {
#line 748 "compiler/main.gem"
    GemVal _t2620 = gem_table_new();
    GemVal _t2621[] = {gem_v_mname};
    gem_table_set(_t2620, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t2621, 1)), gem_string(")")));
    gem_table_set(_t2620, gem_string("setup"), gem_string(""));
            GemVal _t2622 = _t2620;
            gem_pop_frame();
            return _t2622;
        }
#line 750 "compiler/main.gem"
    GemVal _t2623 = gem_table_new();
    gem_table_set(_t2623, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2623, gem_string("setup"), gem_string(""));
        GemVal _t2624 = _t2623;
        gem_pop_frame();
        return _t2624;
    } else {
    GemVal _t2625[] = {gem_v__match_46};
    GemVal _t2627;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2625, 1), gem_string("table")))) {
        _t2627 = gem_eq(gem_type_fn(NULL, _t2625, 1), gem_string("table"));
    } else {
        GemVal _t2626[] = {gem_v__match_46, gem_string("tag")};
        _t2627 = gem_has_key_fn(NULL, _t2626, 2);
    }
    GemVal _t2628;
    if (!gem_truthy(_t2627)) {
        _t2628 = _t2627;
    } else {
        _t2628 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2628)) {
#line 752 "compiler/main.gem"
    GemVal _t2629[] = {gem_v_node};
    GemVal _t2630 = (*gem_v_compile_call);
        GemVal _t2631 = _t2630.fn(_t2630.env, _t2629, 1);
        gem_pop_frame();
        return _t2631;
    } else {
    GemVal _t2632[] = {gem_v__match_46};
    GemVal _t2634;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2632, 1), gem_string("table")))) {
        _t2634 = gem_eq(gem_type_fn(NULL, _t2632, 1), gem_string("table"));
    } else {
        GemVal _t2633[] = {gem_v__match_46, gem_string("tag")};
        _t2634 = gem_has_key_fn(NULL, _t2633, 2);
    }
    GemVal _t2635;
    if (!gem_truthy(_t2634)) {
        _t2635 = _t2634;
    } else {
        _t2635 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2635)) {
#line 754 "compiler/main.gem"
    GemVal _t2636[] = {gem_v_node};
    GemVal _t2637 = (*gem_v_compile_binop);
        GemVal _t2638 = _t2637.fn(_t2637.env, _t2636, 1);
        gem_pop_frame();
        return _t2638;
    } else {
    GemVal _t2639[] = {gem_v__match_46};
    GemVal _t2641;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2639, 1), gem_string("table")))) {
        _t2641 = gem_eq(gem_type_fn(NULL, _t2639, 1), gem_string("table"));
    } else {
        GemVal _t2640[] = {gem_v__match_46, gem_string("tag")};
        _t2641 = gem_has_key_fn(NULL, _t2640, 2);
    }
    GemVal _t2642;
    if (!gem_truthy(_t2641)) {
        _t2642 = _t2641;
    } else {
        _t2642 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2644;
    if (!gem_truthy(_t2642)) {
        _t2644 = _t2642;
    } else {
        GemVal _t2643[] = {gem_v__match_46, gem_string("expr")};
        _t2644 = gem_has_key_fn(NULL, _t2643, 2);
    }
    GemVal _t2646;
    if (!gem_truthy(_t2644)) {
        _t2646 = _t2644;
    } else {
        GemVal _t2645[] = {gem_v__match_46, gem_string("op")};
        _t2646 = gem_has_key_fn(NULL, _t2645, 2);
    }
    if (gem_truthy(_t2646)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_46, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_46, gem_string("op"));
#line 756 "compiler/main.gem"
    GemVal _t2647[] = {gem_v_uexpr};
    GemVal _t2648 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2648.fn(_t2648.env, _t2647, 1);
#line 757 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 758 "compiler/main.gem"
    GemVal _t2649 = gem_table_new();
    GemVal _t2650 = gem_v_r;
    GemVal _t2651[] = {gem_table_get(_t2650, gem_string("expr"))};
    gem_table_set(_t2649, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t2651, 1)), gem_string(")")));
    GemVal _t2652 = gem_v_r;
    gem_table_set(_t2649, gem_string("setup"), gem_table_get(_t2652, gem_string("setup")));
            GemVal _t2653 = _t2649;
            gem_pop_frame();
            return _t2653;
        } else {
#line 759 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 760 "compiler/main.gem"
    GemVal _t2654 = gem_table_new();
    GemVal _t2655 = gem_v_r;
    GemVal _t2656[] = {gem_table_get(_t2655, gem_string("expr"))};
    gem_table_set(_t2654, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2656, 1)), gem_string(")")));
    GemVal _t2657 = gem_v_r;
    gem_table_set(_t2654, gem_string("setup"), gem_table_get(_t2657, gem_string("setup")));
                GemVal _t2658 = _t2654;
                gem_pop_frame();
                return _t2658;
            }
        }
#line 762 "compiler/main.gem"
    GemVal _t2659[] = {gem_v_op};
    GemVal _t2660[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2659, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 762, _t2660, 1));
    } else {
    GemVal _t2661[] = {gem_v__match_46};
    GemVal _t2663;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2661, 1), gem_string("table")))) {
        _t2663 = gem_eq(gem_type_fn(NULL, _t2661, 1), gem_string("table"));
    } else {
        GemVal _t2662[] = {gem_v__match_46, gem_string("tag")};
        _t2663 = gem_has_key_fn(NULL, _t2662, 2);
    }
    GemVal _t2664;
    if (!gem_truthy(_t2663)) {
        _t2664 = _t2663;
    } else {
        _t2664 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2664)) {
#line 764 "compiler/main.gem"
    GemVal _t2665[] = {gem_v_node};
    GemVal _t2666 = (*gem_v_compile_anon_fn);
        GemVal _t2667 = _t2666.fn(_t2666.env, _t2665, 1);
        gem_pop_frame();
        return _t2667;
    } else {
    GemVal _t2668[] = {gem_v__match_46};
    GemVal _t2670;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2668, 1), gem_string("table")))) {
        _t2670 = gem_eq(gem_type_fn(NULL, _t2668, 1), gem_string("table"));
    } else {
        GemVal _t2669[] = {gem_v__match_46, gem_string("tag")};
        _t2670 = gem_has_key_fn(NULL, _t2669, 2);
    }
    GemVal _t2671;
    if (!gem_truthy(_t2670)) {
        _t2671 = _t2670;
    } else {
        _t2671 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2671)) {
#line 766 "compiler/main.gem"
    GemVal _t2672[] = {gem_v_node};
    GemVal _t2673 = (*gem_v_compile_table);
        GemVal _t2674 = _t2673.fn(_t2673.env, _t2672, 1);
        gem_pop_frame();
        return _t2674;
    } else {
    GemVal _t2675[] = {gem_v__match_46};
    GemVal _t2677;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2675, 1), gem_string("table")))) {
        _t2677 = gem_eq(gem_type_fn(NULL, _t2675, 1), gem_string("table"));
    } else {
        GemVal _t2676[] = {gem_v__match_46, gem_string("tag")};
        _t2677 = gem_has_key_fn(NULL, _t2676, 2);
    }
    GemVal _t2678;
    if (!gem_truthy(_t2677)) {
        _t2678 = _t2677;
    } else {
        _t2678 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2678)) {
#line 768 "compiler/main.gem"
    GemVal _t2679[] = {gem_v_node};
    GemVal _t2680 = (*gem_v_compile_array);
        GemVal _t2681 = _t2680.fn(_t2680.env, _t2679, 1);
        gem_pop_frame();
        return _t2681;
    } else {
    GemVal _t2682[] = {gem_v__match_46};
    GemVal _t2684;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2682, 1), gem_string("table")))) {
        _t2684 = gem_eq(gem_type_fn(NULL, _t2682, 1), gem_string("table"));
    } else {
        GemVal _t2683[] = {gem_v__match_46, gem_string("tag")};
        _t2684 = gem_has_key_fn(NULL, _t2683, 2);
    }
    GemVal _t2685;
    if (!gem_truthy(_t2684)) {
        _t2685 = _t2684;
    } else {
        _t2685 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2687;
    if (!gem_truthy(_t2685)) {
        _t2687 = _t2685;
    } else {
        GemVal _t2686[] = {gem_v__match_46, gem_string("object")};
        _t2687 = gem_has_key_fn(NULL, _t2686, 2);
    }
    GemVal _t2689;
    if (!gem_truthy(_t2687)) {
        _t2689 = _t2687;
    } else {
        GemVal _t2688[] = {gem_v__match_46, gem_string("field")};
        _t2689 = gem_has_key_fn(NULL, _t2688, 2);
    }
    if (gem_truthy(_t2689)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_46, gem_string("field"));
#line 770 "compiler/main.gem"
    GemVal _t2690[] = {gem_v_object};
    GemVal _t2691 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2691.fn(_t2691.env, _t2690, 1);
#line 771 "compiler/main.gem"
    GemVal _t2692 = (*gem_v_tmp);
        GemVal gem_v_t = _t2692.fn(_t2692.env, NULL, 0);
#line 772 "compiler/main.gem"
    GemVal _t2693 = gem_v_r;
    GemVal _t2694[] = {gem_v_t};
    GemVal _t2695 = gem_v_r;
    GemVal _t2696[] = {gem_table_get(_t2695, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2693, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2694, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2696, 1)), gem_string(";\n")));
#line 773 "compiler/main.gem"
    GemVal _t2697[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2697, 1);
#line 774 "compiler/main.gem"
    GemVal _t2698 = gem_table_new();
    GemVal _t2699[] = {gem_v_t};
    GemVal _t2700[] = {gem_v_escaped};
    gem_table_set(_t2698, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2699, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2700, 1)), gem_string("\"))")));
    gem_table_set(_t2698, gem_string("setup"), gem_v_setup);
        GemVal _t2701 = _t2698;
        gem_pop_frame();
        return _t2701;
    } else {
    GemVal _t2702[] = {gem_v__match_46};
    GemVal _t2704;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2702, 1), gem_string("table")))) {
        _t2704 = gem_eq(gem_type_fn(NULL, _t2702, 1), gem_string("table"));
    } else {
        GemVal _t2703[] = {gem_v__match_46, gem_string("tag")};
        _t2704 = gem_has_key_fn(NULL, _t2703, 2);
    }
    GemVal _t2705;
    if (!gem_truthy(_t2704)) {
        _t2705 = _t2704;
    } else {
        _t2705 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2707;
    if (!gem_truthy(_t2705)) {
        _t2707 = _t2705;
    } else {
        GemVal _t2706[] = {gem_v__match_46, gem_string("object")};
        _t2707 = gem_has_key_fn(NULL, _t2706, 2);
    }
    GemVal _t2709;
    if (!gem_truthy(_t2707)) {
        _t2709 = _t2707;
    } else {
        GemVal _t2708[] = {gem_v__match_46, gem_string("key")};
        _t2709 = gem_has_key_fn(NULL, _t2708, 2);
    }
    if (gem_truthy(_t2709)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_46, gem_string("key"));
#line 776 "compiler/main.gem"
    GemVal _t2710[] = {gem_v_object};
    GemVal _t2711 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2711.fn(_t2711.env, _t2710, 1);
#line 777 "compiler/main.gem"
    GemVal _t2712[] = {gem_v_key};
    GemVal _t2713 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2713.fn(_t2713.env, _t2712, 1);
#line 778 "compiler/main.gem"
    GemVal _t2714 = gem_table_new();
    GemVal _t2715 = gem_v_obj_r;
    GemVal _t2716[] = {gem_table_get(_t2715, gem_string("expr"))};
    GemVal _t2717 = gem_v_key_r;
    GemVal _t2718[] = {gem_table_get(_t2717, gem_string("expr"))};
    gem_table_set(_t2714, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2716, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2718, 1)), gem_string(")")));
    GemVal _t2719 = gem_v_obj_r;
    GemVal _t2720 = gem_v_key_r;
    gem_table_set(_t2714, gem_string("setup"), gem_add(gem_table_get(_t2719, gem_string("setup")), gem_table_get(_t2720, gem_string("setup"))));
        GemVal _t2721 = _t2714;
        gem_pop_frame();
        return _t2721;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 781 "compiler/main.gem"
    GemVal _t2722 = gem_v_node;
    GemVal _t2723[] = {gem_table_get(_t2722, gem_string("tag"))};
    GemVal _t2724[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2723, 1))};
    GemVal _t2725 = gem_error_at_fn("compiler/main.gem", 781, _t2724, 1);
    gem_pop_frame();
    return _t2725;
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
#line 787 "compiler/main.gem"
    GemVal _t2727 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2727, gem_string("entries"));
#line 788 "compiler/main.gem"
    GemVal _t2728 = (*gem_v_tmp);
    GemVal gem_v_t = _t2728.fn(_t2728.env, NULL, 0);
#line 789 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 790 "compiler/main.gem"
    GemVal _t2729[] = {gem_v_t};
    GemVal _t2730[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2729, 1)), gem_string(" = gem_table_new();\n"))};
    (void)(gem_buf_push_fn(NULL, _t2730, 2));
#line 791 "compiler/main.gem"
    GemVal gem_v__for_items_49 = gem_v_entries;
#line 791 "compiler/main.gem"
    GemVal gem_v__for_i_49 = gem_int(0);
#line 791 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2731[] = {gem_v__for_items_49};
        if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2731, 1)))) break;
#line 791 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 791 "compiler/main.gem"
        gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 792 "compiler/main.gem"
    GemVal _t2732 = gem_v_entry;
    GemVal _t2733[] = {gem_table_get(_t2732, gem_string("value"))};
    GemVal _t2734 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2734.fn(_t2734.env, _t2733, 1);
#line 793 "compiler/main.gem"
    GemVal _t2735 = gem_v_entry;
    GemVal _t2736[] = {gem_table_get(_t2735, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2736, 1);
#line 794 "compiler/main.gem"
    GemVal _t2737 = gem_v_val_r;
    GemVal _t2738[] = {gem_v_b, gem_table_get(_t2737, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2738, 2));
#line 795 "compiler/main.gem"
    GemVal _t2739[] = {gem_v_t};
    GemVal _t2740[] = {gem_v_escaped};
    GemVal _t2741 = gem_v_val_r;
    GemVal _t2742[] = {gem_table_get(_t2741, gem_string("expr"))};
    GemVal _t2743[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2739, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2740, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2742, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t2743, 2));
    }

    GemVal _t2744 = gem_table_new();
    gem_table_set(_t2744, gem_string("expr"), gem_v_t);
    GemVal _t2745[] = {gem_v_b};
    gem_table_set(_t2744, gem_string("setup"), gem_buf_str_fn(NULL, _t2745, 1));
    GemVal _t2746 = _t2744;
    gem_pop_frame();
    return _t2746;
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
#line 803 "compiler/main.gem"
    GemVal _t2748 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2748, gem_string("elements"));
#line 804 "compiler/main.gem"
    GemVal _t2749 = (*gem_v_tmp);
    GemVal gem_v_t = _t2749.fn(_t2749.env, NULL, 0);
#line 805 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 806 "compiler/main.gem"
    GemVal _t2750[] = {gem_v_t};
    GemVal _t2751[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2750, 1)), gem_string(" = gem_table_new();\n"))};
    (void)(gem_buf_push_fn(NULL, _t2751, 2));
#line 807 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 807 "compiler/main.gem"
    GemVal _t2752[] = {gem_v_elements};
    GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2752, 1);
#line 807 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 807 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_50;
#line 807 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 808 "compiler/main.gem"
    GemVal _t2753[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2754 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2754.fn(_t2754.env, _t2753, 1);
#line 809 "compiler/main.gem"
    GemVal _t2755 = gem_v_elem_r;
    GemVal _t2756[] = {gem_v_b, gem_table_get(_t2755, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2756, 2));
#line 810 "compiler/main.gem"
    GemVal _t2757[] = {gem_v_t};
    GemVal _t2758[] = {gem_v_i};
    GemVal _t2759 = gem_v_elem_r;
    GemVal _t2760[] = {gem_table_get(_t2759, gem_string("expr"))};
    GemVal _t2761[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2757, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2758, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2760, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t2761, 2));
    }

    GemVal _t2762 = gem_table_new();
    gem_table_set(_t2762, gem_string("expr"), gem_v_t);
    GemVal _t2763[] = {gem_v_b};
    gem_table_set(_t2762, gem_string("setup"), gem_buf_str_fn(NULL, _t2763, 1));
    GemVal _t2764 = _t2762;
    gem_pop_frame();
    return _t2764;
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
#line 818 "compiler/main.gem"
    GemVal _t2766 = gem_v_node;
    GemVal _t2767[] = {gem_table_get(_t2766, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2767, 1);
#line 819 "compiler/main.gem"
    GemVal _t2768 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2768, gem_string("rest_param")), GEM_NIL))) {
#line 820 "compiler/main.gem"
    GemVal _t2769 = gem_v_node;
    GemVal _t2770[] = {gem_v_inner_defined, gem_table_get(_t2769, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t2770, 2));
    }
#line 822 "compiler/main.gem"
    GemVal _t2771 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2771, gem_string("block_param")), GEM_NIL))) {
#line 823 "compiler/main.gem"
    GemVal _t2772 = gem_v_node;
    GemVal _t2773[] = {gem_v_inner_defined, gem_table_get(_t2772, gem_string("block_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t2773, 2));
    }
#line 825 "compiler/main.gem"
    GemVal _t2774 = gem_v_node;
    GemVal _t2775[] = {gem_table_get(_t2774, gem_string("body")), gem_v_inner_defined};
    GemVal _t2776 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2776.fn(_t2776.env, _t2775, 2);
#line 826 "compiler/main.gem"
    GemVal _t2777[] = {gem_v_free};
    GemVal _t2778[] = {gem_keys_fn(NULL, _t2777, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2778, 1);
#line 828 "compiler/main.gem"
    GemVal _t2779[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2779, 1), gem_int(0)))) {
#line 829 "compiler/main.gem"
    GemVal _t2780 = gem_table_new();
    GemVal _t2781[] = {gem_v_node, _t2780};
    GemVal _t2782 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2782.fn(_t2782.env, _t2781, 2);
#line 830 "compiler/main.gem"
    GemVal _t2783 = gem_table_new();
    GemVal _t2784 = gem_v_result;
    GemVal _t2785[] = {gem_table_get(_t2784, gem_string("fn_name"))};
    gem_table_set(_t2783, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2785, 1)), gem_string(", NULL)")));
    gem_table_set(_t2783, gem_string("setup"), gem_string(""));
        GemVal _t2786 = _t2783;
        gem_pop_frame();
        return _t2786;
    }
#line 833 "compiler/main.gem"
    GemVal _t2787[] = {gem_v_node, gem_v_captures};
    GemVal _t2788 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2788.fn(_t2788.env, _t2787, 2);
#line 835 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 836 "compiler/main.gem"
    GemVal _t2789 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2789.fn(_t2789.env, NULL, 0);
#line 837 "compiler/main.gem"
    GemVal _t2790 = gem_v_result;
    GemVal _t2791[] = {gem_table_get(_t2790, gem_string("struct_name"))};
    GemVal _t2792[] = {gem_v_env_tmp};
    GemVal _t2793 = gem_v_result;
    GemVal _t2794[] = {gem_table_get(_t2793, gem_string("struct_name"))};
    GemVal _t2795[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2791, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2792, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2794, 1)), gem_string("));\n"))};
    (void)(gem_buf_push_fn(NULL, _t2795, 2));
#line 838 "compiler/main.gem"
    GemVal gem_v__for_items_51 = gem_v_captures;
#line 838 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 838 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2796[] = {gem_v__for_items_51};
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t2796, 1)))) break;
#line 838 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 838 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 839 "compiler/main.gem"
    GemVal _t2797[] = {gem_v_cap};
    GemVal _t2798 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2798.fn(_t2798.env, _t2797, 1);
#line 840 "compiler/main.gem"
    GemVal _t2799[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2799, 2))) {
#line 841 "compiler/main.gem"
    GemVal _t2800[] = {gem_v_env_tmp};
    GemVal _t2801[] = {gem_v_mc};
    GemVal _t2802[] = {gem_v_mc};
    GemVal _t2803[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2800, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2801, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2802, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2803, 2));
        } else {
#line 843 "compiler/main.gem"
    GemVal _t2804[] = {gem_v_env_tmp};
    GemVal _t2805[] = {gem_v_mc};
    GemVal _t2806[] = {gem_v_mc};
    GemVal _t2807[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2804, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2805, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2806, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2807, 2));
        }
    }

    GemVal _t2808 = gem_table_new();
    GemVal _t2809 = gem_v_result;
    GemVal _t2810[] = {gem_table_get(_t2809, gem_string("fn_name"))};
    GemVal _t2811[] = {gem_v_env_tmp};
    gem_table_set(_t2808, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2810, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2811, 1)), gem_string(")")));
    GemVal _t2812[] = {gem_v_b};
    gem_table_set(_t2808, gem_string("setup"), gem_buf_str_fn(NULL, _t2812, 1));
    GemVal _t2813 = _t2808;
    gem_pop_frame();
    return _t2813;
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
#line 853 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 854 "compiler/main.gem"
    GemVal _t2815 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2815;
#line 855 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 855 "compiler/main.gem"
    GemVal _t2816[] = {gem_v_args};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2816, 1);
#line 855 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 855 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 855 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 856 "compiler/main.gem"
    GemVal _t2817[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2818 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2818.fn(_t2818.env, _t2817, 1);
#line 857 "compiler/main.gem"
    GemVal _t2819 = gem_v_r;
    GemVal _t2820[] = {gem_v_b, gem_table_get(_t2819, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2820, 2));
#line 858 "compiler/main.gem"
    GemVal _t2821 = gem_v_r;
    GemVal _t2822[] = {gem_v_arg_exprs, gem_table_get(_t2821, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2822, 2));
    }

#line 860 "compiler/main.gem"
    GemVal _t2823[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2823, 1);
#line 861 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 862 "compiler/main.gem"
    GemVal _t2824 = gem_table_new();
    GemVal _t2825[] = {gem_v_b};
    gem_table_set(_t2824, gem_string("setup"), gem_buf_str_fn(NULL, _t2825, 1));
    gem_table_set(_t2824, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2824, gem_string("argc"), gem_int(0));
        GemVal _t2826 = _t2824;
        gem_pop_frame();
        return _t2826;
    }
#line 864 "compiler/main.gem"
    GemVal _t2827 = (*gem_v_tmp);
    GemVal gem_v_t = _t2827.fn(_t2827.env, NULL, 0);
#line 865 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 866 "compiler/main.gem"
    GemVal gem_v__for_i_53 = gem_int(1);
#line 866 "compiler/main.gem"
    GemVal gem_v__for_limit_53 = gem_v_argc;
#line 866 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_v__for_limit_53))) break;
#line 866 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_53;
#line 866 "compiler/main.gem"
        gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 867 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2828 = gem_table_new();
    GemVal _t2829[] = {gem_v_b};
    GemVal _t2830[] = {gem_v_t};
    GemVal _t2831[] = {gem_v_arr};
    gem_table_set(_t2828, gem_string("setup"), gem_add(gem_buf_str_fn(NULL, _t2829, 1), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2830, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2831, 1)), gem_string("};\n"))));
    gem_table_set(_t2828, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2828, gem_string("argc"), gem_v_argc);
    GemVal _t2832 = _t2828;
    gem_pop_frame();
    return _t2832;
}

struct _closure__anon_40 {
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 874 "compiler/main.gem"
    GemVal _t2834 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2834, gem_string("argc")), gem_int(0)))) {
#line 875 "compiler/main.gem"
    GemVal _t2835 = gem_table_new();
    GemVal _t2836[] = {gem_v_fn_name};
    GemVal _t2837[] = {gem_v_env};
    gem_table_set(_t2835, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2836, 1), gem_string("(")), gem_to_string_fn(NULL, _t2837, 1)), gem_string(", NULL, 0)")));
    GemVal _t2838 = gem_v_ca;
    gem_table_set(_t2835, gem_string("setup"), gem_table_get(_t2838, gem_string("setup")));
        GemVal _t2839 = _t2835;
        gem_pop_frame();
        return _t2839;
    }
    GemVal _t2840 = gem_table_new();
    GemVal _t2841[] = {gem_v_fn_name};
    GemVal _t2842[] = {gem_v_env};
    GemVal _t2843 = gem_v_ca;
    GemVal _t2844[] = {gem_table_get(_t2843, gem_string("arr_name"))};
    GemVal _t2845 = gem_v_ca;
    GemVal _t2846[] = {gem_table_get(_t2845, gem_string("argc"))};
    gem_table_set(_t2840, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2841, 1), gem_string("(")), gem_to_string_fn(NULL, _t2842, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2844, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2846, 1)), gem_string(")")));
    GemVal _t2847 = gem_v_ca;
    gem_table_set(_t2840, gem_string("setup"), gem_table_get(_t2847, gem_string("setup")));
    GemVal _t2848 = _t2840;
    gem_pop_frame();
    return _t2848;
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
#line 883 "compiler/main.gem"
    GemVal gem_v__d54 = gem_v_node;
#line 883 "compiler/main.gem"
    GemVal _t2849 = gem_v__d54;
    GemVal gem_v_func = gem_table_get(_t2849, gem_string("func"));
#line 883 "compiler/main.gem"
    GemVal _t2850 = gem_v__d54;
    GemVal gem_v_args = gem_table_get(_t2850, gem_string("args"));

#line 886 "compiler/main.gem"
    GemVal _t2851 = gem_v_func;
    GemVal _t2853;
    if (!gem_truthy(gem_eq(gem_table_get(_t2851, gem_string("tag")), gem_string("dot")))) {
        _t2853 = gem_eq(gem_table_get(_t2851, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2852 = gem_v_func;
        _t2853 = gem_eq(gem_table_get(_t2852, gem_string("field")), gem_string("len"));
    }
    GemVal _t2855;
    if (!gem_truthy(_t2853)) {
        _t2855 = _t2853;
    } else {
        GemVal _t2854[] = {gem_v_args};
        _t2855 = gem_eq(gem_len_fn(NULL, _t2854, 1), gem_int(0));
    }
    if (gem_truthy(_t2855)) {
#line 887 "compiler/main.gem"
    GemVal _t2856 = gem_v_func;
    GemVal _t2857[] = {gem_table_get(_t2856, gem_string("object"))};
    GemVal _t2858 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2858.fn(_t2858.env, _t2857, 1);
#line 888 "compiler/main.gem"
    GemVal _t2859 = gem_table_new();
    GemVal _t2860 = gem_v_obj_r;
    GemVal _t2861[] = {gem_table_get(_t2860, gem_string("expr"))};
    gem_table_set(_t2859, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2861, 1)), gem_string(")")));
    GemVal _t2862 = gem_v_obj_r;
    gem_table_set(_t2859, gem_string("setup"), gem_table_get(_t2862, gem_string("setup")));
        GemVal _t2863 = _t2859;
        gem_pop_frame();
        return _t2863;
    }
#line 892 "compiler/main.gem"
    GemVal _t2864 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2864, gem_string("tag")), gem_string("var")))) {
#line 893 "compiler/main.gem"
    GemVal _t2865 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2865, gem_string("name"));
#line 895 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 896 "compiler/main.gem"
    GemVal _t2866 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2866, gem_string("line"));
#line 897 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 898 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 900 "compiler/main.gem"
    GemVal _t2867[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn__mod_codegen_escape_c_string(NULL, _t2867, 1);
#line 901 "compiler/main.gem"
    GemVal _t2868[] = {gem_v_args};
    GemVal _t2869 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2869.fn(_t2869.env, _t2868, 1);
#line 902 "compiler/main.gem"
    GemVal _t2870 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2870, gem_string("argc")), gem_int(0)))) {
#line 903 "compiler/main.gem"
    GemVal _t2871 = gem_table_new();
    GemVal _t2872[] = {gem_v_escaped_file};
    GemVal _t2873[] = {gem_v_line};
    gem_table_set(_t2871, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2872, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2873, 1)), gem_string(", NULL, 0)")));
    GemVal _t2874 = gem_v_ca;
    gem_table_set(_t2871, gem_string("setup"), gem_table_get(_t2874, gem_string("setup")));
                GemVal _t2875 = _t2871;
                gem_pop_frame();
                return _t2875;
            }
#line 905 "compiler/main.gem"
    GemVal _t2876 = gem_table_new();
    GemVal _t2877[] = {gem_v_escaped_file};
    GemVal _t2878[] = {gem_v_line};
    GemVal _t2879 = gem_v_ca;
    GemVal _t2880[] = {gem_table_get(_t2879, gem_string("arr_name"))};
    GemVal _t2881 = gem_v_ca;
    GemVal _t2882[] = {gem_table_get(_t2881, gem_string("argc"))};
    gem_table_set(_t2876, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2877, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2878, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2880, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2882, 1)), gem_string(")")));
    GemVal _t2883 = gem_v_ca;
    gem_table_set(_t2876, gem_string("setup"), gem_table_get(_t2883, gem_string("setup")));
            GemVal _t2884 = _t2876;
            gem_pop_frame();
            return _t2884;
        } else {
#line 906 "compiler/main.gem"
    GemVal _t2885[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2887;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2885, 2))) {
        _t2887 = gem_has_key_fn(NULL, _t2885, 2);
    } else {
        GemVal _t2886[] = {(*gem_v_local_names), gem_v_name};
        _t2887 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2886, 2));
    }
            if (gem_truthy(_t2887)) {
#line 907 "compiler/main.gem"
    GemVal _t2888[] = {gem_v_args};
    GemVal _t2889 = (*gem_v_compile_args);
    GemVal _t2890[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2889.fn(_t2889.env, _t2888, 1)};
    GemVal _t2891 = (*gem_v_emit_direct_call);
                GemVal _t2892 = _t2891.fn(_t2891.env, _t2890, 3);
                gem_pop_frame();
                return _t2892;
            } else {
#line 908 "compiler/main.gem"
    GemVal _t2893[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2895;
    if (!gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2893, 2))) {
        _t2895 = gem_fn__mod_codegen_set_contains(NULL, _t2893, 2);
    } else {
        GemVal _t2894[] = {(*gem_v_local_names), gem_v_name};
        _t2895 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2894, 2));
    }
                if (gem_truthy(_t2895)) {
#line 909 "compiler/main.gem"
    GemVal _t2896[] = {gem_v_name};
    GemVal _t2897[] = {gem_v_args};
    GemVal _t2898 = (*gem_v_compile_args);
    GemVal _t2899[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2896, 1)), gem_string("NULL"), _t2898.fn(_t2898.env, _t2897, 1)};
    GemVal _t2900 = (*gem_v_emit_direct_call);
                    GemVal _t2901 = _t2900.fn(_t2900.env, _t2899, 3);
                    gem_pop_frame();
                    return _t2901;
                }
            }
        }
    }
#line 914 "compiler/main.gem"
    GemVal _t2902[] = {gem_v_func};
    GemVal _t2903 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2903.fn(_t2903.env, _t2902, 1);
#line 915 "compiler/main.gem"
    GemVal _t2904[] = {gem_v_args};
    GemVal _t2905 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2905.fn(_t2905.env, _t2904, 1);
#line 916 "compiler/main.gem"
    GemVal _t2906 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2906.fn(_t2906.env, NULL, 0);
#line 917 "compiler/main.gem"
    GemVal _t2907 = gem_v_func_r;
    GemVal _t2908 = gem_v_ca;
    GemVal _t2909[] = {gem_v_tmp_fn};
    GemVal _t2910 = gem_v_func_r;
    GemVal _t2911[] = {gem_table_get(_t2910, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2907, gem_string("setup")), gem_table_get(_t2908, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2909, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2911, 1)), gem_string(";\n")));
#line 918 "compiler/main.gem"
    GemVal _t2912 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2912, gem_string("argc")), gem_int(0)))) {
#line 919 "compiler/main.gem"
    GemVal _t2913 = gem_table_new();
    GemVal _t2914[] = {gem_v_tmp_fn};
    GemVal _t2915[] = {gem_v_tmp_fn};
    gem_table_set(_t2913, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2914, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2915, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2913, gem_string("setup"), gem_v_setup);
        GemVal _t2916 = _t2913;
        gem_pop_frame();
        return _t2916;
    }
    GemVal _t2917 = gem_table_new();
    GemVal _t2918[] = {gem_v_tmp_fn};
    GemVal _t2919[] = {gem_v_tmp_fn};
    GemVal _t2920 = gem_v_ca;
    GemVal _t2921[] = {gem_table_get(_t2920, gem_string("arr_name"))};
    GemVal _t2922 = gem_v_ca;
    GemVal _t2923[] = {gem_table_get(_t2922, gem_string("argc"))};
    gem_table_set(_t2917, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2918, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2919, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2921, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2923, 1)), gem_string(")")));
    gem_table_set(_t2917, gem_string("setup"), gem_v_setup);
    GemVal _t2924 = _t2917;
    gem_pop_frame();
    return _t2924;
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
#line 927 "compiler/main.gem"
    GemVal _t2926 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2926, gem_string("op"));
#line 928 "compiler/main.gem"
    GemVal _t2927 = gem_v_node;
    GemVal _t2928[] = {gem_table_get(_t2927, gem_string("left"))};
    GemVal _t2929 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2929.fn(_t2929.env, _t2928, 1);
#line 929 "compiler/main.gem"
    GemVal _t2930 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2930, gem_string("expr"));
#line 930 "compiler/main.gem"
    GemVal _t2931 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2931, gem_string("setup"));
#line 932 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 933 "compiler/main.gem"
    GemVal _t2932 = gem_v_node;
    GemVal _t2933[] = {gem_table_get(_t2932, gem_string("right"))};
    GemVal _t2934 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2934.fn(_t2934.env, _t2933, 1);
#line 934 "compiler/main.gem"
    GemVal _t2935 = gem_table_new();
    GemVal _t2936[] = {gem_v_lc};
    GemVal _t2937 = gem_v_rc_r;
    GemVal _t2938[] = {gem_table_get(_t2937, gem_string("expr"))};
    gem_table_set(_t2935, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2936, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2938, 1)), gem_string(")")));
    GemVal _t2939 = gem_v_rc_r;
    gem_table_set(_t2935, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2939, gem_string("setup"))));
        GemVal _t2940 = _t2935;
        gem_pop_frame();
        return _t2940;
    } else {
#line 935 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 936 "compiler/main.gem"
    GemVal _t2941 = gem_v_node;
    GemVal _t2942[] = {gem_table_get(_t2941, gem_string("right"))};
    GemVal _t2943 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2943.fn(_t2943.env, _t2942, 1);
#line 937 "compiler/main.gem"
    GemVal _t2944 = gem_table_new();
    GemVal _t2945[] = {gem_v_lc};
    GemVal _t2946 = gem_v_rc_r;
    GemVal _t2947[] = {gem_table_get(_t2946, gem_string("expr"))};
    gem_table_set(_t2944, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2945, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2947, 1)), gem_string(")")));
    GemVal _t2948 = gem_v_rc_r;
    gem_table_set(_t2944, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2948, gem_string("setup"))));
            GemVal _t2949 = _t2944;
            gem_pop_frame();
            return _t2949;
        } else {
#line 938 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 939 "compiler/main.gem"
    GemVal _t2950 = gem_v_node;
    GemVal _t2951[] = {gem_table_get(_t2950, gem_string("right"))};
    GemVal _t2952 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2952.fn(_t2952.env, _t2951, 1);
#line 940 "compiler/main.gem"
    GemVal _t2953 = gem_table_new();
    GemVal _t2954[] = {gem_v_lc};
    GemVal _t2955 = gem_v_rc_r;
    GemVal _t2956[] = {gem_table_get(_t2955, gem_string("expr"))};
    gem_table_set(_t2953, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2954, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2956, 1)), gem_string(")")));
    GemVal _t2957 = gem_v_rc_r;
    gem_table_set(_t2953, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2957, gem_string("setup"))));
                GemVal _t2958 = _t2953;
                gem_pop_frame();
                return _t2958;
            } else {
#line 941 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 942 "compiler/main.gem"
    GemVal _t2959 = gem_v_node;
    GemVal _t2960[] = {gem_table_get(_t2959, gem_string("right"))};
    GemVal _t2961 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2961.fn(_t2961.env, _t2960, 1);
#line 943 "compiler/main.gem"
    GemVal _t2962 = gem_table_new();
    GemVal _t2963[] = {gem_v_lc};
    GemVal _t2964 = gem_v_rc_r;
    GemVal _t2965[] = {gem_table_get(_t2964, gem_string("expr"))};
    gem_table_set(_t2962, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2963, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2965, 1)), gem_string(")")));
    GemVal _t2966 = gem_v_rc_r;
    gem_table_set(_t2962, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2966, gem_string("setup"))));
                    GemVal _t2967 = _t2962;
                    gem_pop_frame();
                    return _t2967;
                } else {
#line 944 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 945 "compiler/main.gem"
    GemVal _t2968 = gem_v_node;
    GemVal _t2969[] = {gem_table_get(_t2968, gem_string("right"))};
    GemVal _t2970 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2970.fn(_t2970.env, _t2969, 1);
#line 946 "compiler/main.gem"
    GemVal _t2971 = gem_table_new();
    GemVal _t2972[] = {gem_v_lc};
    GemVal _t2973 = gem_v_rc_r;
    GemVal _t2974[] = {gem_table_get(_t2973, gem_string("expr"))};
    gem_table_set(_t2971, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2972, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2974, 1)), gem_string(")")));
    GemVal _t2975 = gem_v_rc_r;
    gem_table_set(_t2971, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2975, gem_string("setup"))));
                        GemVal _t2976 = _t2971;
                        gem_pop_frame();
                        return _t2976;
                    } else {
#line 947 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 948 "compiler/main.gem"
    GemVal _t2977 = gem_v_node;
    GemVal _t2978[] = {gem_table_get(_t2977, gem_string("right"))};
    GemVal _t2979 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2979.fn(_t2979.env, _t2978, 1);
#line 949 "compiler/main.gem"
    GemVal _t2980 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2980.fn(_t2980.env, NULL, 0);
#line 950 "compiler/main.gem"
    GemVal _t2981 = gem_v_rc_r;
    GemVal _t2982[] = {gem_v_t};
    GemVal _t2983 = gem_v_rc_r;
    GemVal _t2984[] = {gem_table_get(_t2983, gem_string("expr"))};
    GemVal _t2985[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2981, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2982, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2984, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2985, 1)), gem_string("};\n")));
#line 951 "compiler/main.gem"
    GemVal _t2986 = gem_table_new();
    GemVal _t2987[] = {gem_v_t};
    gem_table_set(_t2986, gem_string("expr"), gem_add(gem_add(gem_string("gem_in_fn(NULL, "), gem_to_string_fn(NULL, _t2987, 1)), gem_string(", 2)")));
    gem_table_set(_t2986, gem_string("setup"), gem_v_setup);
                            GemVal _t2988 = _t2986;
                            gem_pop_frame();
                            return _t2988;
                        } else {
#line 952 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 953 "compiler/main.gem"
    GemVal _t2989 = gem_v_node;
    GemVal _t2990[] = {gem_table_get(_t2989, gem_string("right"))};
    GemVal _t2991 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2991.fn(_t2991.env, _t2990, 1);
#line 954 "compiler/main.gem"
    GemVal _t2992 = gem_table_new();
    GemVal _t2993[] = {gem_v_lc};
    GemVal _t2994 = gem_v_rc_r;
    GemVal _t2995[] = {gem_table_get(_t2994, gem_string("expr"))};
    gem_table_set(_t2992, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2993, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2995, 1)), gem_string(")")));
    GemVal _t2996 = gem_v_rc_r;
    gem_table_set(_t2992, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2996, gem_string("setup"))));
                                GemVal _t2997 = _t2992;
                                gem_pop_frame();
                                return _t2997;
                            } else {
#line 955 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 956 "compiler/main.gem"
    GemVal _t2998 = gem_v_node;
    GemVal _t2999[] = {gem_table_get(_t2998, gem_string("right"))};
    GemVal _t3000 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t3000.fn(_t3000.env, _t2999, 1);
#line 957 "compiler/main.gem"
    GemVal _t3001 = gem_table_new();
    GemVal _t3002[] = {gem_v_lc};
    GemVal _t3003 = gem_v_rc_r;
    GemVal _t3004[] = {gem_table_get(_t3003, gem_string("expr"))};
    gem_table_set(_t3001, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t3002, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3004, 1)), gem_string(")")));
    GemVal _t3005 = gem_v_rc_r;
    gem_table_set(_t3001, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t3005, gem_string("setup"))));
                                    GemVal _t3006 = _t3001;
                                    gem_pop_frame();
                                    return _t3006;
                                } else {
#line 958 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 959 "compiler/main.gem"
    GemVal _t3007 = gem_v_node;
    GemVal _t3008[] = {gem_table_get(_t3007, gem_string("right"))};
    GemVal _t3009 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t3009.fn(_t3009.env, _t3008, 1);
#line 960 "compiler/main.gem"
    GemVal _t3010 = gem_table_new();
    GemVal _t3011[] = {gem_v_lc};
    GemVal _t3012 = gem_v_rc_r;
    GemVal _t3013[] = {gem_table_get(_t3012, gem_string("expr"))};
    gem_table_set(_t3010, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t3011, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3013, 1)), gem_string(")")));
    GemVal _t3014 = gem_v_rc_r;
    gem_table_set(_t3010, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t3014, gem_string("setup"))));
                                        GemVal _t3015 = _t3010;
                                        gem_pop_frame();
                                        return _t3015;
                                    } else {
#line 961 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 962 "compiler/main.gem"
    GemVal _t3016 = gem_v_node;
    GemVal _t3017[] = {gem_table_get(_t3016, gem_string("right"))};
    GemVal _t3018 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t3018.fn(_t3018.env, _t3017, 1);
#line 963 "compiler/main.gem"
    GemVal _t3019 = gem_table_new();
    GemVal _t3020[] = {gem_v_lc};
    GemVal _t3021 = gem_v_rc_r;
    GemVal _t3022[] = {gem_table_get(_t3021, gem_string("expr"))};
    gem_table_set(_t3019, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t3020, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3022, 1)), gem_string(")")));
    GemVal _t3023 = gem_v_rc_r;
    gem_table_set(_t3019, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t3023, gem_string("setup"))));
                                            GemVal _t3024 = _t3019;
                                            gem_pop_frame();
                                            return _t3024;
                                        } else {
#line 964 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 965 "compiler/main.gem"
    GemVal _t3025 = gem_v_node;
    GemVal _t3026[] = {gem_table_get(_t3025, gem_string("right"))};
    GemVal _t3027 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t3027.fn(_t3027.env, _t3026, 1);
#line 966 "compiler/main.gem"
    GemVal _t3028 = gem_table_new();
    GemVal _t3029[] = {gem_v_lc};
    GemVal _t3030 = gem_v_rc_r;
    GemVal _t3031[] = {gem_table_get(_t3030, gem_string("expr"))};
    gem_table_set(_t3028, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t3029, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3031, 1)), gem_string(")")));
    GemVal _t3032 = gem_v_rc_r;
    gem_table_set(_t3028, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t3032, gem_string("setup"))));
                                                GemVal _t3033 = _t3028;
                                                gem_pop_frame();
                                                return _t3033;
                                            } else {
#line 967 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 968 "compiler/main.gem"
    GemVal _t3034 = gem_v_node;
    GemVal _t3035[] = {gem_table_get(_t3034, gem_string("right"))};
    GemVal _t3036 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t3036.fn(_t3036.env, _t3035, 1);
#line 969 "compiler/main.gem"
    GemVal _t3037 = gem_table_new();
    GemVal _t3038[] = {gem_v_lc};
    GemVal _t3039 = gem_v_rc_r;
    GemVal _t3040[] = {gem_table_get(_t3039, gem_string("expr"))};
    gem_table_set(_t3037, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t3038, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3040, 1)), gem_string(")")));
    GemVal _t3041 = gem_v_rc_r;
    gem_table_set(_t3037, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t3041, gem_string("setup"))));
                                                    GemVal _t3042 = _t3037;
                                                    gem_pop_frame();
                                                    return _t3042;
                                                } else {
#line 970 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 971 "compiler/main.gem"
    GemVal _t3043 = gem_v_node;
    GemVal _t3044[] = {gem_table_get(_t3043, gem_string("right"))};
    GemVal _t3045 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t3045.fn(_t3045.env, _t3044, 1);
#line 972 "compiler/main.gem"
    GemVal _t3046 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t3046.fn(_t3046.env, NULL, 0);
#line 973 "compiler/main.gem"
                                                        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 974 "compiler/main.gem"
    GemVal _t3047[] = {gem_v_b, gem_v_ls};
                                                        (void)(gem_buf_push_fn(NULL, _t3047, 2));
#line 975 "compiler/main.gem"
    GemVal _t3048[] = {gem_v_t};
    GemVal _t3049[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3048, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3049, 2));
#line 976 "compiler/main.gem"
    GemVal _t3050[] = {gem_v_lc};
    GemVal _t3051[] = {gem_v_b, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t3050, 1)), gem_string(")) {\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3051, 2));
#line 977 "compiler/main.gem"
    GemVal _t3052[] = {gem_v_t};
    GemVal _t3053[] = {gem_v_lc};
    GemVal _t3054[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3052, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3053, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3054, 2));
#line 978 "compiler/main.gem"
    GemVal _t3055[] = {gem_v_b, gem_string("    } else {\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3055, 2));
#line 979 "compiler/main.gem"
    GemVal _t3056 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t3056, gem_string("setup")), gem_string("")))) {
#line 980 "compiler/main.gem"
    GemVal _t3057 = gem_v_rc_r;
    GemVal _t3058[] = {gem_table_get(_t3057, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t3059[] = {gem_v_b, gem_str_replace_fn(NULL, _t3058, 3)};
                                                            (void)(gem_buf_push_fn(NULL, _t3059, 2));
                                                        }
#line 982 "compiler/main.gem"
    GemVal _t3060[] = {gem_v_t};
    GemVal _t3061 = gem_v_rc_r;
    GemVal _t3062[] = {gem_table_get(_t3061, gem_string("expr"))};
    GemVal _t3063[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3060, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3062, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3063, 2));
#line 983 "compiler/main.gem"
    GemVal _t3064[] = {gem_v_b, gem_string("    }\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3064, 2));
#line 984 "compiler/main.gem"
    GemVal _t3065 = gem_table_new();
    gem_table_set(_t3065, gem_string("expr"), gem_v_t);
    GemVal _t3066[] = {gem_v_b};
    gem_table_set(_t3065, gem_string("setup"), gem_buf_str_fn(NULL, _t3066, 1));
                                                        GemVal _t3067 = _t3065;
                                                        gem_pop_frame();
                                                        return _t3067;
                                                    } else {
#line 985 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 986 "compiler/main.gem"
    GemVal _t3068 = gem_v_node;
    GemVal _t3069[] = {gem_table_get(_t3068, gem_string("right"))};
    GemVal _t3070 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t3070.fn(_t3070.env, _t3069, 1);
#line 987 "compiler/main.gem"
    GemVal _t3071 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t3071.fn(_t3071.env, NULL, 0);
#line 988 "compiler/main.gem"
                                                            GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 989 "compiler/main.gem"
    GemVal _t3072[] = {gem_v_b, gem_v_ls};
                                                            (void)(gem_buf_push_fn(NULL, _t3072, 2));
#line 990 "compiler/main.gem"
    GemVal _t3073[] = {gem_v_t};
    GemVal _t3074[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3073, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3074, 2));
#line 991 "compiler/main.gem"
    GemVal _t3075[] = {gem_v_lc};
    GemVal _t3076[] = {gem_v_b, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t3075, 1)), gem_string(")) {\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3076, 2));
#line 992 "compiler/main.gem"
    GemVal _t3077[] = {gem_v_t};
    GemVal _t3078[] = {gem_v_lc};
    GemVal _t3079[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3077, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3078, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3079, 2));
#line 993 "compiler/main.gem"
    GemVal _t3080[] = {gem_v_b, gem_string("    } else {\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3080, 2));
#line 994 "compiler/main.gem"
    GemVal _t3081 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t3081, gem_string("setup")), gem_string("")))) {
#line 995 "compiler/main.gem"
    GemVal _t3082 = gem_v_rc_r;
    GemVal _t3083[] = {gem_table_get(_t3082, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t3084[] = {gem_v_b, gem_str_replace_fn(NULL, _t3083, 3)};
                                                                (void)(gem_buf_push_fn(NULL, _t3084, 2));
                                                            }
#line 997 "compiler/main.gem"
    GemVal _t3085[] = {gem_v_t};
    GemVal _t3086 = gem_v_rc_r;
    GemVal _t3087[] = {gem_table_get(_t3086, gem_string("expr"))};
    GemVal _t3088[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3085, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3087, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3088, 2));
#line 998 "compiler/main.gem"
    GemVal _t3089[] = {gem_v_b, gem_string("    }\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3089, 2));
#line 999 "compiler/main.gem"
    GemVal _t3090 = gem_table_new();
    gem_table_set(_t3090, gem_string("expr"), gem_v_t);
    GemVal _t3091[] = {gem_v_b};
    gem_table_set(_t3090, gem_string("setup"), gem_buf_str_fn(NULL, _t3091, 1));
                                                            GemVal _t3092 = _t3090;
                                                            gem_pop_frame();
                                                            return _t3092;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1002 "compiler/main.gem"
    GemVal _t3093[] = {gem_v_op};
    GemVal _t3094[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t3093, 1))};
    GemVal _t3095 = gem_error_at_fn("compiler/main.gem", 1002, _t3094, 1);
    gem_pop_frame();
    return _t3095;
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
#line 1008 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1009 "compiler/main.gem"
    GemVal gem_v__for_items_55 = gem_v_stmts;
#line 1009 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 1009 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3097[] = {gem_v__for_items_55};
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t3097, 1)))) break;
#line 1009 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1009 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1010 "compiler/main.gem"
    GemVal _t3098[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3099 = (*gem_v_compile_stmt);
    GemVal _t3100[] = {gem_v_b, _t3099.fn(_t3099.env, _t3098, 2)};
        (void)(gem_buf_push_fn(NULL, _t3100, 2));
#line 1011 "compiler/main.gem"
    GemVal _t3101[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3101, 2));
    }

#line 1013 "compiler/main.gem"
    GemVal _t3102[] = {gem_v_b};
    GemVal _t3103 = gem_buf_str_fn(NULL, _t3102, 1);
    gem_pop_frame();
    return _t3103;
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
#line 1017 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_return_pos))) {
#line 1018 "compiler/main.gem"
    GemVal _t3105[] = {gem_v_stmts, gem_v_indent};
    GemVal _t3106 = (*gem_v_compile_stmts);
        GemVal _t3107 = _t3106.fn(_t3106.env, _t3105, 2);
        gem_pop_frame();
        return _t3107;
    }
#line 1020 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1021 "compiler/main.gem"
    GemVal _t3108[] = {gem_v_stmts};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3108, 1), gem_int(0)))) {
#line 1022 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1022 "compiler/main.gem"
    GemVal _t3109[] = {gem_v_stmts};
        GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t3109, 1), gem_int(1));
#line 1022 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1022 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_56;
#line 1022 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1023 "compiler/main.gem"
    GemVal _t3110[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_indent};
    GemVal _t3111 = (*gem_v_compile_stmt);
    GemVal _t3112[] = {gem_v_b, _t3111.fn(_t3111.env, _t3110, 2)};
            (void)(gem_buf_push_fn(NULL, _t3112, 2));
#line 1024 "compiler/main.gem"
    GemVal _t3113[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3113, 2));
        }

#line 1026 "compiler/main.gem"
    GemVal _t3114[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t3115 = (*gem_v_compile_stmt_return);
    GemVal _t3116[] = {gem_v_b, _t3115.fn(_t3115.env, _t3114, 2)};
        (void)(gem_buf_push_fn(NULL, _t3116, 2));
#line 1027 "compiler/main.gem"
    GemVal _t3117[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3117, 2));
    } else {
#line 1029 "compiler/main.gem"
    GemVal _t3118[] = {gem_v_indent};
        GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3118, 1);
#line 1030 "compiler/main.gem"
    GemVal _t3119[] = {gem_v_p};
    GemVal _t3120[] = {gem_v_p};
    GemVal _t3121[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3119, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3120, 1)), gem_string("return GEM_NIL;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3121, 2));
    }
#line 1032 "compiler/main.gem"
    GemVal _t3122[] = {gem_v_b};
    GemVal _t3123 = gem_buf_str_fn(NULL, _t3122, 1);
    gem_pop_frame();
    return _t3123;
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
#line 1038 "compiler/main.gem"
    GemVal _t3125[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3125, 1);
#line 1039 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1040 "compiler/main.gem"
    GemVal _t3126 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3126, gem_string("line")), GEM_NIL))) {
#line 1041 "compiler/main.gem"
    GemVal _t3127 = gem_v_node;
    GemVal _t3128[] = {gem_table_get(_t3127, gem_string("line"))};
    GemVal _t3129[] = {(*gem_v_source_name)};
    GemVal _t3130[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t3129, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3128, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3130, 1)), gem_string("\"\n"));
    }
#line 1044 "compiler/main.gem"
    GemVal gem_v__match_57 = gem_v_node;
    GemVal _t3131[] = {gem_v__match_57};
    GemVal _t3133;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3131, 1), gem_string("table")))) {
        _t3133 = gem_eq(gem_type_fn(NULL, _t3131, 1), gem_string("table"));
    } else {
        GemVal _t3132[] = {gem_v__match_57, gem_string("tag")};
        _t3133 = gem_has_key_fn(NULL, _t3132, 2);
    }
    GemVal _t3134;
    if (!gem_truthy(_t3133)) {
        _t3134 = _t3133;
    } else {
        _t3134 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3136;
    if (!gem_truthy(_t3134)) {
        _t3136 = _t3134;
    } else {
        GemVal _t3135[] = {gem_v__match_57, gem_string("name")};
        _t3136 = gem_has_key_fn(NULL, _t3135, 2);
    }
    GemVal _t3138;
    if (!gem_truthy(_t3136)) {
        _t3138 = _t3136;
    } else {
        GemVal _t3137[] = {gem_v__match_57, gem_string("value")};
        _t3138 = gem_has_key_fn(NULL, _t3137, 2);
    }
    if (gem_truthy(_t3138)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1046 "compiler/main.gem"
    GemVal _t3139[] = {gem_v_value};
    GemVal _t3140 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3140.fn(_t3140.env, _t3139, 1);
#line 1047 "compiler/main.gem"
    GemVal _t3141[] = {gem_v_name};
    GemVal _t3142 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3142.fn(_t3142.env, _t3141, 1);
#line 1048 "compiler/main.gem"
    GemVal _t3144;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t3144 = (*gem_v_in_top_level);
    } else {
        GemVal _t3143[] = {(*gem_v_top_level_vars), gem_v_name};
        _t3144 = gem_fn__mod_codegen_set_contains(NULL, _t3143, 2);
    }
        if (gem_truthy(_t3144)) {
#line 1049 "compiler/main.gem"
    GemVal _t3145[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3145, 2))) {
#line 1050 "compiler/main.gem"
    GemVal _t3146[] = {gem_v_line_dir};
    GemVal _t3147 = gem_v_r;
    GemVal _t3148[] = {gem_table_get(_t3147, gem_string("setup"))};
    GemVal _t3149[] = {gem_v_p};
    GemVal _t3150[] = {gem_v_mname};
    GemVal _t3151[] = {gem_v_p};
    GemVal _t3152[] = {gem_v_mname};
    GemVal _t3153 = gem_v_r;
    GemVal _t3154[] = {gem_table_get(_t3153, gem_string("expr"))};
                GemVal _t3155 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3146, 1), gem_to_string_fn(NULL, _t3148, 1)), gem_to_string_fn(NULL, _t3149, 1)), gem_to_string_fn(NULL, _t3150, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3151, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3152, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3154, 1)), gem_string(";"));
                gem_pop_frame();
                return _t3155;
            }
#line 1052 "compiler/main.gem"
    GemVal _t3156[] = {gem_v_line_dir};
    GemVal _t3157 = gem_v_r;
    GemVal _t3158[] = {gem_table_get(_t3157, gem_string("setup"))};
    GemVal _t3159[] = {gem_v_p};
    GemVal _t3160[] = {gem_v_mname};
    GemVal _t3161 = gem_v_r;
    GemVal _t3162[] = {gem_table_get(_t3161, gem_string("expr"))};
            GemVal _t3163 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3156, 1), gem_to_string_fn(NULL, _t3158, 1)), gem_to_string_fn(NULL, _t3159, 1)), gem_to_string_fn(NULL, _t3160, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3162, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3163;
        }
#line 1054 "compiler/main.gem"
    GemVal _t3164[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3164, 2))) {
#line 1055 "compiler/main.gem"
    GemVal _t3165[] = {gem_v_line_dir};
    GemVal _t3166 = gem_v_r;
    GemVal _t3167[] = {gem_table_get(_t3166, gem_string("setup"))};
    GemVal _t3168[] = {gem_v_p};
    GemVal _t3169[] = {gem_v_mname};
    GemVal _t3170[] = {gem_v_p};
    GemVal _t3171[] = {gem_v_mname};
    GemVal _t3172 = gem_v_r;
    GemVal _t3173[] = {gem_table_get(_t3172, gem_string("expr"))};
            GemVal _t3174 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3165, 1), gem_to_string_fn(NULL, _t3167, 1)), gem_to_string_fn(NULL, _t3168, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t3169, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3170, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3171, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3173, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3174;
        }
#line 1057 "compiler/main.gem"
    GemVal _t3175[] = {gem_v_line_dir};
    GemVal _t3176 = gem_v_r;
    GemVal _t3177[] = {gem_table_get(_t3176, gem_string("setup"))};
    GemVal _t3178[] = {gem_v_p};
    GemVal _t3179[] = {gem_v_mname};
    GemVal _t3180 = gem_v_r;
    GemVal _t3181[] = {gem_table_get(_t3180, gem_string("expr"))};
        GemVal _t3182 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3175, 1), gem_to_string_fn(NULL, _t3177, 1)), gem_to_string_fn(NULL, _t3178, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3179, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3181, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3182;
    } else {
    GemVal _t3183[] = {gem_v__match_57};
    GemVal _t3185;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3183, 1), gem_string("table")))) {
        _t3185 = gem_eq(gem_type_fn(NULL, _t3183, 1), gem_string("table"));
    } else {
        GemVal _t3184[] = {gem_v__match_57, gem_string("tag")};
        _t3185 = gem_has_key_fn(NULL, _t3184, 2);
    }
    GemVal _t3186;
    if (!gem_truthy(_t3185)) {
        _t3186 = _t3185;
    } else {
        _t3186 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3188;
    if (!gem_truthy(_t3186)) {
        _t3188 = _t3186;
    } else {
        GemVal _t3187[] = {gem_v__match_57, gem_string("name")};
        _t3188 = gem_has_key_fn(NULL, _t3187, 2);
    }
    GemVal _t3190;
    if (!gem_truthy(_t3188)) {
        _t3190 = _t3188;
    } else {
        GemVal _t3189[] = {gem_v__match_57, gem_string("value")};
        _t3190 = gem_has_key_fn(NULL, _t3189, 2);
    }
    if (gem_truthy(_t3190)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1059 "compiler/main.gem"
    GemVal _t3191[] = {gem_v_value};
    GemVal _t3192 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3192.fn(_t3192.env, _t3191, 1);
#line 1060 "compiler/main.gem"
    GemVal _t3193[] = {gem_v_name};
    GemVal _t3194 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3194.fn(_t3194.env, _t3193, 1);
#line 1061 "compiler/main.gem"
    GemVal _t3195[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3197;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3195, 2))) {
        _t3197 = gem_fn__mod_codegen_set_contains(NULL, _t3195, 2);
    } else {
        GemVal _t3196[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3197 = gem_fn__mod_codegen_set_contains(NULL, _t3196, 2);
    }
        if (gem_truthy(_t3197)) {
#line 1062 "compiler/main.gem"
    GemVal _t3198[] = {gem_v_line_dir};
    GemVal _t3199 = gem_v_r;
    GemVal _t3200[] = {gem_table_get(_t3199, gem_string("setup"))};
    GemVal _t3201[] = {gem_v_p};
    GemVal _t3202[] = {gem_v_mname};
    GemVal _t3203 = gem_v_r;
    GemVal _t3204[] = {gem_table_get(_t3203, gem_string("expr"))};
            GemVal _t3205 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3198, 1), gem_to_string_fn(NULL, _t3200, 1)), gem_to_string_fn(NULL, _t3201, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3202, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3204, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3205;
        }
#line 1064 "compiler/main.gem"
    GemVal _t3206[] = {gem_v_line_dir};
    GemVal _t3207 = gem_v_r;
    GemVal _t3208[] = {gem_table_get(_t3207, gem_string("setup"))};
    GemVal _t3209[] = {gem_v_p};
    GemVal _t3210[] = {gem_v_mname};
    GemVal _t3211 = gem_v_r;
    GemVal _t3212[] = {gem_table_get(_t3211, gem_string("expr"))};
        GemVal _t3213 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3206, 1), gem_to_string_fn(NULL, _t3208, 1)), gem_to_string_fn(NULL, _t3209, 1)), gem_to_string_fn(NULL, _t3210, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3212, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3213;
    } else {
    GemVal _t3214[] = {gem_v__match_57};
    GemVal _t3216;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3214, 1), gem_string("table")))) {
        _t3216 = gem_eq(gem_type_fn(NULL, _t3214, 1), gem_string("table"));
    } else {
        GemVal _t3215[] = {gem_v__match_57, gem_string("tag")};
        _t3216 = gem_has_key_fn(NULL, _t3215, 2);
    }
    GemVal _t3217;
    if (!gem_truthy(_t3216)) {
        _t3217 = _t3216;
    } else {
        _t3217 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t3219;
    if (!gem_truthy(_t3217)) {
        _t3219 = _t3217;
    } else {
        GemVal _t3218[] = {gem_v__match_57, gem_string("object")};
        _t3219 = gem_has_key_fn(NULL, _t3218, 2);
    }
    GemVal _t3221;
    if (!gem_truthy(_t3219)) {
        _t3221 = _t3219;
    } else {
        GemVal _t3220[] = {gem_v__match_57, gem_string("value")};
        _t3221 = gem_has_key_fn(NULL, _t3220, 2);
    }
    GemVal _t3223;
    if (!gem_truthy(_t3221)) {
        _t3223 = _t3221;
    } else {
        GemVal _t3222[] = {gem_v__match_57, gem_string("field")};
        _t3223 = gem_has_key_fn(NULL, _t3222, 2);
    }
    if (gem_truthy(_t3223)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_57, gem_string("field"));
#line 1066 "compiler/main.gem"
    GemVal _t3224[] = {gem_v_object};
    GemVal _t3225 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3225.fn(_t3225.env, _t3224, 1);
#line 1067 "compiler/main.gem"
    GemVal _t3226[] = {gem_v_value};
    GemVal _t3227 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3227.fn(_t3227.env, _t3226, 1);
#line 1068 "compiler/main.gem"
    GemVal _t3228[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t3228, 1);
#line 1069 "compiler/main.gem"
    GemVal _t3229[] = {gem_v_line_dir};
    GemVal _t3230 = gem_v_obj_r;
    GemVal _t3231[] = {gem_table_get(_t3230, gem_string("setup"))};
    GemVal _t3232 = gem_v_val_r;
    GemVal _t3233[] = {gem_table_get(_t3232, gem_string("setup"))};
    GemVal _t3234[] = {gem_v_p};
    GemVal _t3235 = gem_v_obj_r;
    GemVal _t3236[] = {gem_table_get(_t3235, gem_string("expr"))};
    GemVal _t3237[] = {gem_v_escaped};
    GemVal _t3238 = gem_v_val_r;
    GemVal _t3239[] = {gem_table_get(_t3238, gem_string("expr"))};
        GemVal _t3240 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3229, 1), gem_to_string_fn(NULL, _t3231, 1)), gem_to_string_fn(NULL, _t3233, 1)), gem_to_string_fn(NULL, _t3234, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t3236, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t3237, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t3239, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3240;
    } else {
    GemVal _t3241[] = {gem_v__match_57};
    GemVal _t3243;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3241, 1), gem_string("table")))) {
        _t3243 = gem_eq(gem_type_fn(NULL, _t3241, 1), gem_string("table"));
    } else {
        GemVal _t3242[] = {gem_v__match_57, gem_string("tag")};
        _t3243 = gem_has_key_fn(NULL, _t3242, 2);
    }
    GemVal _t3244;
    if (!gem_truthy(_t3243)) {
        _t3244 = _t3243;
    } else {
        _t3244 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t3246;
    if (!gem_truthy(_t3244)) {
        _t3246 = _t3244;
    } else {
        GemVal _t3245[] = {gem_v__match_57, gem_string("object")};
        _t3246 = gem_has_key_fn(NULL, _t3245, 2);
    }
    GemVal _t3248;
    if (!gem_truthy(_t3246)) {
        _t3248 = _t3246;
    } else {
        GemVal _t3247[] = {gem_v__match_57, gem_string("key")};
        _t3248 = gem_has_key_fn(NULL, _t3247, 2);
    }
    GemVal _t3250;
    if (!gem_truthy(_t3248)) {
        _t3250 = _t3248;
    } else {
        GemVal _t3249[] = {gem_v__match_57, gem_string("value")};
        _t3250 = gem_has_key_fn(NULL, _t3249, 2);
    }
    if (gem_truthy(_t3250)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_57, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1071 "compiler/main.gem"
    GemVal _t3251[] = {gem_v_object};
    GemVal _t3252 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3252.fn(_t3252.env, _t3251, 1);
#line 1072 "compiler/main.gem"
    GemVal _t3253[] = {gem_v_key};
    GemVal _t3254 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t3254.fn(_t3254.env, _t3253, 1);
#line 1073 "compiler/main.gem"
    GemVal _t3255[] = {gem_v_value};
    GemVal _t3256 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3256.fn(_t3256.env, _t3255, 1);
#line 1074 "compiler/main.gem"
    GemVal _t3257[] = {gem_v_line_dir};
    GemVal _t3258 = gem_v_obj_r;
    GemVal _t3259[] = {gem_table_get(_t3258, gem_string("setup"))};
    GemVal _t3260 = gem_v_key_r;
    GemVal _t3261[] = {gem_table_get(_t3260, gem_string("setup"))};
    GemVal _t3262 = gem_v_val_r;
    GemVal _t3263[] = {gem_table_get(_t3262, gem_string("setup"))};
    GemVal _t3264[] = {gem_v_p};
    GemVal _t3265 = gem_v_obj_r;
    GemVal _t3266[] = {gem_table_get(_t3265, gem_string("expr"))};
    GemVal _t3267 = gem_v_key_r;
    GemVal _t3268[] = {gem_table_get(_t3267, gem_string("expr"))};
    GemVal _t3269 = gem_v_val_r;
    GemVal _t3270[] = {gem_table_get(_t3269, gem_string("expr"))};
        GemVal _t3271 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3257, 1), gem_to_string_fn(NULL, _t3259, 1)), gem_to_string_fn(NULL, _t3261, 1)), gem_to_string_fn(NULL, _t3263, 1)), gem_to_string_fn(NULL, _t3264, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t3266, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3268, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3270, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3271;
    } else {
    GemVal _t3272[] = {gem_v__match_57};
    GemVal _t3274;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3272, 1), gem_string("table")))) {
        _t3274 = gem_eq(gem_type_fn(NULL, _t3272, 1), gem_string("table"));
    } else {
        GemVal _t3273[] = {gem_v__match_57, gem_string("tag")};
        _t3274 = gem_has_key_fn(NULL, _t3273, 2);
    }
    GemVal _t3275;
    if (!gem_truthy(_t3274)) {
        _t3275 = _t3274;
    } else {
        _t3275 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3275)) {
#line 1076 "compiler/main.gem"
    GemVal _t3276[] = {gem_v_line_dir};
    GemVal _t3277[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3278 = (*gem_v_compile_if);
        GemVal _t3279 = gem_add(gem_to_string_fn(NULL, _t3276, 1), _t3278.fn(_t3278.env, _t3277, 3));
        gem_pop_frame();
        return _t3279;
    } else {
    GemVal _t3280[] = {gem_v__match_57};
    GemVal _t3282;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3280, 1), gem_string("table")))) {
        _t3282 = gem_eq(gem_type_fn(NULL, _t3280, 1), gem_string("table"));
    } else {
        GemVal _t3281[] = {gem_v__match_57, gem_string("tag")};
        _t3282 = gem_has_key_fn(NULL, _t3281, 2);
    }
    GemVal _t3283;
    if (!gem_truthy(_t3282)) {
        _t3283 = _t3282;
    } else {
        _t3283 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3283)) {
#line 1078 "compiler/main.gem"
    GemVal _t3284[] = {gem_v_line_dir};
    GemVal _t3285[] = {gem_v_node, gem_v_indent};
    GemVal _t3286 = (*gem_v_compile_while);
        GemVal _t3287 = gem_add(gem_to_string_fn(NULL, _t3284, 1), _t3286.fn(_t3286.env, _t3285, 2));
        gem_pop_frame();
        return _t3287;
    } else {
    GemVal _t3288[] = {gem_v__match_57};
    GemVal _t3290;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3288, 1), gem_string("table")))) {
        _t3290 = gem_eq(gem_type_fn(NULL, _t3288, 1), gem_string("table"));
    } else {
        GemVal _t3289[] = {gem_v__match_57, gem_string("tag")};
        _t3290 = gem_has_key_fn(NULL, _t3289, 2);
    }
    GemVal _t3291;
    if (!gem_truthy(_t3290)) {
        _t3291 = _t3290;
    } else {
        _t3291 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3291)) {
#line 1080 "compiler/main.gem"
    GemVal _t3292[] = {gem_v_line_dir};
    GemVal _t3293[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3294 = (*gem_v_compile_match);
        GemVal _t3295 = gem_add(gem_to_string_fn(NULL, _t3292, 1), _t3294.fn(_t3294.env, _t3293, 3));
        gem_pop_frame();
        return _t3295;
    } else {
    GemVal _t3296[] = {gem_v__match_57};
    GemVal _t3298;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3296, 1), gem_string("table")))) {
        _t3298 = gem_eq(gem_type_fn(NULL, _t3296, 1), gem_string("table"));
    } else {
        GemVal _t3297[] = {gem_v__match_57, gem_string("tag")};
        _t3298 = gem_has_key_fn(NULL, _t3297, 2);
    }
    GemVal _t3299;
    if (!gem_truthy(_t3298)) {
        _t3299 = _t3298;
    } else {
        _t3299 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3299)) {
#line 1082 "compiler/main.gem"
    GemVal _t3300[] = {gem_v_line_dir};
    GemVal _t3301[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3302 = (*gem_v_compile_receive_match);
        GemVal _t3303 = gem_add(gem_to_string_fn(NULL, _t3300, 1), _t3302.fn(_t3302.env, _t3301, 3));
        gem_pop_frame();
        return _t3303;
    } else {
    GemVal _t3304[] = {gem_v__match_57};
    GemVal _t3306;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3304, 1), gem_string("table")))) {
        _t3306 = gem_eq(gem_type_fn(NULL, _t3304, 1), gem_string("table"));
    } else {
        GemVal _t3305[] = {gem_v__match_57, gem_string("tag")};
        _t3306 = gem_has_key_fn(NULL, _t3305, 2);
    }
    GemVal _t3307;
    if (!gem_truthy(_t3306)) {
        _t3307 = _t3306;
    } else {
        _t3307 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3307)) {
#line 1084 "compiler/main.gem"
        GemVal _t3308 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t3308;
    } else {
    GemVal _t3309[] = {gem_v__match_57};
    GemVal _t3311;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3309, 1), gem_string("table")))) {
        _t3311 = gem_eq(gem_type_fn(NULL, _t3309, 1), gem_string("table"));
    } else {
        GemVal _t3310[] = {gem_v__match_57, gem_string("tag")};
        _t3311 = gem_has_key_fn(NULL, _t3310, 2);
    }
    GemVal _t3312;
    if (!gem_truthy(_t3311)) {
        _t3312 = _t3311;
    } else {
        _t3312 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3312)) {
#line 1086 "compiler/main.gem"
        GemVal _t3313 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t3313;
    } else {
    GemVal _t3314[] = {gem_v__match_57};
    GemVal _t3316;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3314, 1), gem_string("table")))) {
        _t3316 = gem_eq(gem_type_fn(NULL, _t3314, 1), gem_string("table"));
    } else {
        GemVal _t3315[] = {gem_v__match_57, gem_string("tag")};
        _t3316 = gem_has_key_fn(NULL, _t3315, 2);
    }
    GemVal _t3317;
    if (!gem_truthy(_t3316)) {
        _t3317 = _t3316;
    } else {
        _t3317 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3319;
    if (!gem_truthy(_t3317)) {
        _t3319 = _t3317;
    } else {
        GemVal _t3318[] = {gem_v__match_57, gem_string("value")};
        _t3319 = gem_has_key_fn(NULL, _t3318, 2);
    }
    if (gem_truthy(_t3319)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1088 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1089 "compiler/main.gem"
    GemVal _t3320[] = {gem_v_value};
    GemVal _t3321 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3321.fn(_t3321.env, _t3320, 1);
#line 1090 "compiler/main.gem"
    GemVal _t3322 = (*gem_v_tmp);
            GemVal gem_v_t = _t3322.fn(_t3322.env, NULL, 0);
#line 1091 "compiler/main.gem"
    GemVal _t3323[] = {gem_v_line_dir};
    GemVal _t3324 = gem_v_r;
    GemVal _t3325[] = {gem_table_get(_t3324, gem_string("setup"))};
    GemVal _t3326[] = {gem_v_p};
    GemVal _t3327[] = {gem_v_t};
    GemVal _t3328 = gem_v_r;
    GemVal _t3329[] = {gem_table_get(_t3328, gem_string("expr"))};
    GemVal _t3330[] = {gem_v_p};
    GemVal _t3331[] = {gem_v_p};
    GemVal _t3332[] = {gem_v_t};
            GemVal _t3333 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3323, 1), gem_to_string_fn(NULL, _t3325, 1)), gem_to_string_fn(NULL, _t3326, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3327, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3329, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3330, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3331, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3332, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3333;
        }
#line 1093 "compiler/main.gem"
    GemVal _t3334[] = {gem_v_line_dir};
    GemVal _t3335[] = {gem_v_p};
    GemVal _t3336[] = {gem_v_p};
        GemVal _t3337 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3334, 1), gem_to_string_fn(NULL, _t3335, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3336, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3337;
    } else {
    GemVal _t3338[] = {gem_v__match_57};
    GemVal _t3340;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3338, 1), gem_string("table")))) {
        _t3340 = gem_eq(gem_type_fn(NULL, _t3338, 1), gem_string("table"));
    } else {
        GemVal _t3339[] = {gem_v__match_57, gem_string("tag")};
        _t3340 = gem_has_key_fn(NULL, _t3339, 2);
    }
    GemVal _t3341;
    if (!gem_truthy(_t3340)) {
        _t3341 = _t3340;
    } else {
        _t3341 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t3341)) {
#line 1095 "compiler/main.gem"
    GemVal _t3342[] = {gem_v_node};
    GemVal _t3343 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3343.fn(_t3343.env, _t3342, 1);
#line 1096 "compiler/main.gem"
    GemVal _t3344[] = {gem_v_line_dir};
    GemVal _t3345 = gem_v_r;
    GemVal _t3346[] = {gem_table_get(_t3345, gem_string("setup"))};
    GemVal _t3347[] = {gem_v_p};
    GemVal _t3348 = gem_v_r;
    GemVal _t3349[] = {gem_table_get(_t3348, gem_string("expr"))};
        GemVal _t3350 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3344, 1), gem_to_string_fn(NULL, _t3346, 1)), gem_to_string_fn(NULL, _t3347, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t3349, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3350;
    } else {
    GemVal _t3351[] = {gem_v__match_57};
    GemVal _t3353;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3351, 1), gem_string("table")))) {
        _t3353 = gem_eq(gem_type_fn(NULL, _t3351, 1), gem_string("table"));
    } else {
        GemVal _t3352[] = {gem_v__match_57, gem_string("tag")};
        _t3353 = gem_has_key_fn(NULL, _t3352, 2);
    }
    GemVal _t3354;
    if (!gem_truthy(_t3353)) {
        _t3354 = _t3353;
    } else {
        _t3354 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3354)) {
#line 1098 "compiler/main.gem"
        GemVal _t3355 = gem_string("");
        gem_pop_frame();
        return _t3355;
    } else {
    GemVal _t3356[] = {gem_v__match_57};
    GemVal _t3358;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3356, 1), gem_string("table")))) {
        _t3358 = gem_eq(gem_type_fn(NULL, _t3356, 1), gem_string("table"));
    } else {
        GemVal _t3357[] = {gem_v__match_57, gem_string("tag")};
        _t3358 = gem_has_key_fn(NULL, _t3357, 2);
    }
    GemVal _t3359;
    if (!gem_truthy(_t3358)) {
        _t3359 = _t3358;
    } else {
        _t3359 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3361;
    if (!gem_truthy(_t3359)) {
        _t3361 = _t3359;
    } else {
        GemVal _t3360[] = {gem_v__match_57, gem_string("stmts")};
        _t3361 = gem_has_key_fn(NULL, _t3360, 2);
    }
    if (gem_truthy(_t3361)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_57, gem_string("stmts"));
#line 1100 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1101 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_stmts;
#line 1101 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1101 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3362[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t3362, 1)))) break;
#line 1101 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1101 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1102 "compiler/main.gem"
    GemVal _t3363[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3364 = (*gem_v_compile_stmt);
    GemVal _t3365[] = {gem_v_b, _t3364.fn(_t3364.env, _t3363, 2)};
            (void)(gem_buf_push_fn(NULL, _t3365, 2));
#line 1103 "compiler/main.gem"
    GemVal _t3366[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3366, 2));
        }

#line 1105 "compiler/main.gem"
    GemVal _t3367[] = {gem_v_b};
        GemVal _t3368 = gem_buf_str_fn(NULL, _t3367, 1);
        gem_pop_frame();
        return _t3368;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1108 "compiler/main.gem"
    GemVal _t3369[] = {gem_v_node};
    GemVal _t3370 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3370.fn(_t3370.env, _t3369, 1);
    GemVal _t3371[] = {gem_v_line_dir};
    GemVal _t3372 = gem_v_r;
    GemVal _t3373[] = {gem_table_get(_t3372, gem_string("setup"))};
    GemVal _t3374[] = {gem_v_p};
    GemVal _t3375 = gem_v_r;
    GemVal _t3376[] = {gem_table_get(_t3375, gem_string("expr"))};
    GemVal _t3377 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3371, 1), gem_to_string_fn(NULL, _t3373, 1)), gem_to_string_fn(NULL, _t3374, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t3376, 1)), gem_string(");"));
    gem_pop_frame();
    return _t3377;
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
#line 1115 "compiler/main.gem"
    GemVal _t3379[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3379, 1);
#line 1116 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1117 "compiler/main.gem"
    GemVal _t3380 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3380, gem_string("line")), GEM_NIL))) {
#line 1118 "compiler/main.gem"
    GemVal _t3381 = gem_v_node;
    GemVal _t3382[] = {gem_table_get(_t3381, gem_string("line"))};
    GemVal _t3383[] = {(*gem_v_source_name)};
    GemVal _t3384[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t3383, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3382, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3384, 1)), gem_string("\"\n"));
    }
#line 1121 "compiler/main.gem"
    if (gem_truthy(gem_neq((*gem_v_tco_fn_name), GEM_NIL))) {
#line 1122 "compiler/main.gem"
    GemVal _t3385[] = {gem_v_node, gem_string("call")};
    GemVal _t3388;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3385, 2))) {
        _t3388 = gem_fn__mod_codegen_is_node(NULL, _t3385, 2);
    } else {
        GemVal _t3386 = gem_v_node;
        GemVal _t3387[] = {gem_table_get(_t3386, gem_string("func")), gem_string("var")};
        _t3388 = gem_fn__mod_codegen_is_node(NULL, _t3387, 2);
    }
    GemVal _t3391;
    if (!gem_truthy(_t3388)) {
        _t3391 = _t3388;
    } else {
        GemVal _t3389 = gem_v_node;
        GemVal _t3390 = gem_table_get(_t3389, gem_string("func"));
        _t3391 = gem_eq(gem_table_get(_t3390, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3393;
    if (!gem_truthy(_t3391)) {
        _t3393 = _t3391;
    } else {
        GemVal _t3392[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3393 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3392, 2));
    }
        if (gem_truthy(_t3393)) {
#line 1123 "compiler/main.gem"
    GemVal _t3394[] = {gem_v_node, gem_v_indent};
    GemVal _t3395 = (*gem_v_emit_tco_continue);
            GemVal _t3396 = gem_add(gem_v_line_dir, _t3395.fn(_t3395.env, _t3394, 2));
            gem_pop_frame();
            return _t3396;
        }
#line 1125 "compiler/main.gem"
    GemVal _t3397[] = {gem_v_node, gem_string("return")};
    GemVal _t3399;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3397, 2))) {
        _t3399 = gem_fn__mod_codegen_is_node(NULL, _t3397, 2);
    } else {
        GemVal _t3398 = gem_v_node;
        _t3399 = gem_neq(gem_table_get(_t3398, gem_string("value")), GEM_NIL);
    }
    GemVal _t3402;
    if (!gem_truthy(_t3399)) {
        _t3402 = _t3399;
    } else {
        GemVal _t3400 = gem_v_node;
        GemVal _t3401[] = {gem_table_get(_t3400, gem_string("value")), gem_string("call")};
        _t3402 = gem_fn__mod_codegen_is_node(NULL, _t3401, 2);
    }
    GemVal _t3406;
    if (!gem_truthy(_t3402)) {
        _t3406 = _t3402;
    } else {
        GemVal _t3403 = gem_v_node;
        GemVal _t3404 = gem_table_get(_t3403, gem_string("value"));
        GemVal _t3405[] = {gem_table_get(_t3404, gem_string("func")), gem_string("var")};
        _t3406 = gem_fn__mod_codegen_is_node(NULL, _t3405, 2);
    }
    GemVal _t3410;
    if (!gem_truthy(_t3406)) {
        _t3410 = _t3406;
    } else {
        GemVal _t3407 = gem_v_node;
        GemVal _t3408 = gem_table_get(_t3407, gem_string("value"));
        GemVal _t3409 = gem_table_get(_t3408, gem_string("func"));
        _t3410 = gem_eq(gem_table_get(_t3409, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3412;
    if (!gem_truthy(_t3410)) {
        _t3412 = _t3410;
    } else {
        GemVal _t3411[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3412 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3411, 2));
    }
        if (gem_truthy(_t3412)) {
#line 1126 "compiler/main.gem"
    GemVal _t3413 = gem_v_node;
    GemVal _t3414[] = {gem_table_get(_t3413, gem_string("value")), gem_v_indent};
    GemVal _t3415 = (*gem_v_emit_tco_continue);
            GemVal _t3416 = gem_add(gem_v_line_dir, _t3415.fn(_t3415.env, _t3414, 2));
            gem_pop_frame();
            return _t3416;
        }
    }
#line 1130 "compiler/main.gem"
    GemVal gem_v__match_59 = gem_v_node;
    GemVal _t3417[] = {gem_v__match_59};
    GemVal _t3419;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3417, 1), gem_string("table")))) {
        _t3419 = gem_eq(gem_type_fn(NULL, _t3417, 1), gem_string("table"));
    } else {
        GemVal _t3418[] = {gem_v__match_59, gem_string("tag")};
        _t3419 = gem_has_key_fn(NULL, _t3418, 2);
    }
    GemVal _t3420;
    if (!gem_truthy(_t3419)) {
        _t3420 = _t3419;
    } else {
        _t3420 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3422;
    if (!gem_truthy(_t3420)) {
        _t3422 = _t3420;
    } else {
        GemVal _t3421[] = {gem_v__match_59, gem_string("name")};
        _t3422 = gem_has_key_fn(NULL, _t3421, 2);
    }
    GemVal _t3424;
    if (!gem_truthy(_t3422)) {
        _t3424 = _t3422;
    } else {
        GemVal _t3423[] = {gem_v__match_59, gem_string("value")};
        _t3424 = gem_has_key_fn(NULL, _t3423, 2);
    }
    if (gem_truthy(_t3424)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1132 "compiler/main.gem"
    GemVal _t3425[] = {gem_v_value};
    GemVal _t3426 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3426.fn(_t3426.env, _t3425, 1);
#line 1133 "compiler/main.gem"
    GemVal _t3427[] = {gem_v_name};
    GemVal _t3428 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3428.fn(_t3428.env, _t3427, 1);
#line 1134 "compiler/main.gem"
    GemVal _t3429[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3429, 2))) {
#line 1135 "compiler/main.gem"
    GemVal _t3430[] = {gem_v_line_dir};
    GemVal _t3431 = gem_v_r;
    GemVal _t3432[] = {gem_table_get(_t3431, gem_string("setup"))};
    GemVal _t3433[] = {gem_v_p};
    GemVal _t3434[] = {gem_v_mname};
    GemVal _t3435[] = {gem_v_p};
    GemVal _t3436[] = {gem_v_mname};
    GemVal _t3437 = gem_v_r;
    GemVal _t3438[] = {gem_table_get(_t3437, gem_string("expr"))};
    GemVal _t3439[] = {gem_v_p};
    GemVal _t3440[] = {gem_v_p};
            GemVal _t3441 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3430, 1), gem_to_string_fn(NULL, _t3432, 1)), gem_to_string_fn(NULL, _t3433, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t3434, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3435, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3436, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3438, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3439, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3440, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t3441;
        }
#line 1137 "compiler/main.gem"
    GemVal _t3442[] = {gem_v_line_dir};
    GemVal _t3443 = gem_v_r;
    GemVal _t3444[] = {gem_table_get(_t3443, gem_string("setup"))};
    GemVal _t3445[] = {gem_v_p};
    GemVal _t3446[] = {gem_v_mname};
    GemVal _t3447 = gem_v_r;
    GemVal _t3448[] = {gem_table_get(_t3447, gem_string("expr"))};
    GemVal _t3449[] = {gem_v_p};
    GemVal _t3450[] = {gem_v_p};
        GemVal _t3451 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3442, 1), gem_to_string_fn(NULL, _t3444, 1)), gem_to_string_fn(NULL, _t3445, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3446, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3448, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3449, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3450, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3451;
    } else {
    GemVal _t3452[] = {gem_v__match_59};
    GemVal _t3454;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3452, 1), gem_string("table")))) {
        _t3454 = gem_eq(gem_type_fn(NULL, _t3452, 1), gem_string("table"));
    } else {
        GemVal _t3453[] = {gem_v__match_59, gem_string("tag")};
        _t3454 = gem_has_key_fn(NULL, _t3453, 2);
    }
    GemVal _t3455;
    if (!gem_truthy(_t3454)) {
        _t3455 = _t3454;
    } else {
        _t3455 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3457;
    if (!gem_truthy(_t3455)) {
        _t3457 = _t3455;
    } else {
        GemVal _t3456[] = {gem_v__match_59, gem_string("name")};
        _t3457 = gem_has_key_fn(NULL, _t3456, 2);
    }
    GemVal _t3459;
    if (!gem_truthy(_t3457)) {
        _t3459 = _t3457;
    } else {
        GemVal _t3458[] = {gem_v__match_59, gem_string("value")};
        _t3459 = gem_has_key_fn(NULL, _t3458, 2);
    }
    if (gem_truthy(_t3459)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1139 "compiler/main.gem"
    GemVal _t3460[] = {gem_v_value};
    GemVal _t3461 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3461.fn(_t3461.env, _t3460, 1);
#line 1140 "compiler/main.gem"
    GemVal _t3462 = (*gem_v_tmp);
        GemVal gem_v_t = _t3462.fn(_t3462.env, NULL, 0);
#line 1141 "compiler/main.gem"
    GemVal _t3463[] = {gem_v_name};
    GemVal _t3464 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3464.fn(_t3464.env, _t3463, 1);
#line 1142 "compiler/main.gem"
    GemVal _t3465[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3467;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3465, 2))) {
        _t3467 = gem_fn__mod_codegen_set_contains(NULL, _t3465, 2);
    } else {
        GemVal _t3466[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3467 = gem_fn__mod_codegen_set_contains(NULL, _t3466, 2);
    }
        if (gem_truthy(_t3467)) {
#line 1143 "compiler/main.gem"
    GemVal _t3468[] = {gem_v_line_dir};
    GemVal _t3469 = gem_v_r;
    GemVal _t3470[] = {gem_table_get(_t3469, gem_string("setup"))};
    GemVal _t3471[] = {gem_v_p};
    GemVal _t3472[] = {gem_v_t};
    GemVal _t3473 = gem_v_r;
    GemVal _t3474[] = {gem_table_get(_t3473, gem_string("expr"))};
    GemVal _t3475[] = {gem_v_p};
    GemVal _t3476[] = {gem_v_mname};
    GemVal _t3477[] = {gem_v_t};
    GemVal _t3478[] = {gem_v_p};
    GemVal _t3479[] = {gem_v_p};
    GemVal _t3480[] = {gem_v_t};
            GemVal _t3481 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3468, 1), gem_to_string_fn(NULL, _t3470, 1)), gem_to_string_fn(NULL, _t3471, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3472, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3474, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3475, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3476, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3477, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3478, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3479, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3480, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3481;
        }
#line 1145 "compiler/main.gem"
    GemVal _t3482[] = {gem_v_line_dir};
    GemVal _t3483 = gem_v_r;
    GemVal _t3484[] = {gem_table_get(_t3483, gem_string("setup"))};
    GemVal _t3485[] = {gem_v_p};
    GemVal _t3486[] = {gem_v_t};
    GemVal _t3487 = gem_v_r;
    GemVal _t3488[] = {gem_table_get(_t3487, gem_string("expr"))};
    GemVal _t3489[] = {gem_v_p};
    GemVal _t3490[] = {gem_v_mname};
    GemVal _t3491[] = {gem_v_t};
    GemVal _t3492[] = {gem_v_p};
    GemVal _t3493[] = {gem_v_p};
    GemVal _t3494[] = {gem_v_t};
        GemVal _t3495 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3482, 1), gem_to_string_fn(NULL, _t3484, 1)), gem_to_string_fn(NULL, _t3485, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3486, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3488, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3489, 1)), gem_to_string_fn(NULL, _t3490, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3491, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3492, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3493, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3494, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3495;
    } else {
    GemVal _t3496[] = {gem_v__match_59};
    GemVal _t3498;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3496, 1), gem_string("table")))) {
        _t3498 = gem_eq(gem_type_fn(NULL, _t3496, 1), gem_string("table"));
    } else {
        GemVal _t3497[] = {gem_v__match_59, gem_string("tag")};
        _t3498 = gem_has_key_fn(NULL, _t3497, 2);
    }
    GemVal _t3499;
    if (!gem_truthy(_t3498)) {
        _t3499 = _t3498;
    } else {
        _t3499 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3501;
    if (!gem_truthy(_t3499)) {
        _t3501 = _t3499;
    } else {
        GemVal _t3500[] = {gem_v__match_59, gem_string("value")};
        _t3501 = gem_has_key_fn(NULL, _t3500, 2);
    }
    if (gem_truthy(_t3501)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1147 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1148 "compiler/main.gem"
    GemVal _t3502[] = {gem_v_value};
    GemVal _t3503 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3503.fn(_t3503.env, _t3502, 1);
#line 1149 "compiler/main.gem"
    GemVal _t3504 = (*gem_v_tmp);
            GemVal gem_v_t = _t3504.fn(_t3504.env, NULL, 0);
#line 1150 "compiler/main.gem"
    GemVal _t3505[] = {gem_v_line_dir};
    GemVal _t3506 = gem_v_r;
    GemVal _t3507[] = {gem_table_get(_t3506, gem_string("setup"))};
    GemVal _t3508[] = {gem_v_p};
    GemVal _t3509[] = {gem_v_t};
    GemVal _t3510 = gem_v_r;
    GemVal _t3511[] = {gem_table_get(_t3510, gem_string("expr"))};
    GemVal _t3512[] = {gem_v_p};
    GemVal _t3513[] = {gem_v_p};
    GemVal _t3514[] = {gem_v_t};
            GemVal _t3515 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3505, 1), gem_to_string_fn(NULL, _t3507, 1)), gem_to_string_fn(NULL, _t3508, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3509, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3511, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3512, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3513, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3514, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3515;
        }
#line 1152 "compiler/main.gem"
    GemVal _t3516[] = {gem_v_line_dir};
    GemVal _t3517[] = {gem_v_p};
    GemVal _t3518[] = {gem_v_p};
        GemVal _t3519 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3516, 1), gem_to_string_fn(NULL, _t3517, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3518, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3519;
    } else {
    GemVal _t3520[] = {gem_v__match_59};
    GemVal _t3522;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3520, 1), gem_string("table")))) {
        _t3522 = gem_eq(gem_type_fn(NULL, _t3520, 1), gem_string("table"));
    } else {
        GemVal _t3521[] = {gem_v__match_59, gem_string("tag")};
        _t3522 = gem_has_key_fn(NULL, _t3521, 2);
    }
    GemVal _t3523;
    if (!gem_truthy(_t3522)) {
        _t3523 = _t3522;
    } else {
        _t3523 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3523)) {
#line 1154 "compiler/main.gem"
    GemVal _t3524[] = {gem_v_line_dir};
    GemVal _t3525[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3526 = (*gem_v_compile_if);
        GemVal _t3527 = gem_add(gem_to_string_fn(NULL, _t3524, 1), _t3526.fn(_t3526.env, _t3525, 3));
        gem_pop_frame();
        return _t3527;
    } else {
    GemVal _t3528[] = {gem_v__match_59};
    GemVal _t3530;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3528, 1), gem_string("table")))) {
        _t3530 = gem_eq(gem_type_fn(NULL, _t3528, 1), gem_string("table"));
    } else {
        GemVal _t3529[] = {gem_v__match_59, gem_string("tag")};
        _t3530 = gem_has_key_fn(NULL, _t3529, 2);
    }
    GemVal _t3531;
    if (!gem_truthy(_t3530)) {
        _t3531 = _t3530;
    } else {
        _t3531 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3531)) {
#line 1156 "compiler/main.gem"
    GemVal _t3532[] = {gem_v_line_dir};
    GemVal _t3533[] = {gem_v_node, gem_v_indent};
    GemVal _t3534 = (*gem_v_compile_while);
    GemVal _t3535[] = {_t3534.fn(_t3534.env, _t3533, 2)};
    GemVal _t3536[] = {gem_v_p};
    GemVal _t3537[] = {gem_v_p};
        GemVal _t3538 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3532, 1), gem_to_string_fn(NULL, _t3535, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t3536, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3537, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3538;
    } else {
    GemVal _t3539[] = {gem_v__match_59};
    GemVal _t3541;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3539, 1), gem_string("table")))) {
        _t3541 = gem_eq(gem_type_fn(NULL, _t3539, 1), gem_string("table"));
    } else {
        GemVal _t3540[] = {gem_v__match_59, gem_string("tag")};
        _t3541 = gem_has_key_fn(NULL, _t3540, 2);
    }
    GemVal _t3542;
    if (!gem_truthy(_t3541)) {
        _t3542 = _t3541;
    } else {
        _t3542 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3542)) {
#line 1158 "compiler/main.gem"
    GemVal _t3543[] = {gem_v_line_dir};
    GemVal _t3544[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3545 = (*gem_v_compile_match);
        GemVal _t3546 = gem_add(gem_to_string_fn(NULL, _t3543, 1), _t3545.fn(_t3545.env, _t3544, 3));
        gem_pop_frame();
        return _t3546;
    } else {
    GemVal _t3547[] = {gem_v__match_59};
    GemVal _t3549;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3547, 1), gem_string("table")))) {
        _t3549 = gem_eq(gem_type_fn(NULL, _t3547, 1), gem_string("table"));
    } else {
        GemVal _t3548[] = {gem_v__match_59, gem_string("tag")};
        _t3549 = gem_has_key_fn(NULL, _t3548, 2);
    }
    GemVal _t3550;
    if (!gem_truthy(_t3549)) {
        _t3550 = _t3549;
    } else {
        _t3550 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3550)) {
#line 1160 "compiler/main.gem"
    GemVal _t3551[] = {gem_v_line_dir};
    GemVal _t3552[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3553 = (*gem_v_compile_receive_match);
        GemVal _t3554 = gem_add(gem_to_string_fn(NULL, _t3551, 1), _t3553.fn(_t3553.env, _t3552, 3));
        gem_pop_frame();
        return _t3554;
    } else {
    GemVal _t3555[] = {gem_v__match_59};
    GemVal _t3557;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3555, 1), gem_string("table")))) {
        _t3557 = gem_eq(gem_type_fn(NULL, _t3555, 1), gem_string("table"));
    } else {
        GemVal _t3556[] = {gem_v__match_59, gem_string("tag")};
        _t3557 = gem_has_key_fn(NULL, _t3556, 2);
    }
    GemVal _t3558;
    if (!gem_truthy(_t3557)) {
        _t3558 = _t3557;
    } else {
        _t3558 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3560;
    if (!gem_truthy(_t3558)) {
        _t3560 = _t3558;
    } else {
        GemVal _t3559[] = {gem_v__match_59, gem_string("stmts")};
        _t3560 = gem_has_key_fn(NULL, _t3559, 2);
    }
    if (gem_truthy(_t3560)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_59, gem_string("stmts"));
#line 1162 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1163 "compiler/main.gem"
    GemVal _t3561[] = {gem_v_p};
    GemVal _t3562[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3561, 1), gem_string("{\n"))};
        (void)(gem_buf_push_fn(NULL, _t3562, 2));
#line 1164 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1164 "compiler/main.gem"
    GemVal _t3563[] = {gem_v_stmts};
        GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3563, 1), gem_int(1));
#line 1164 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1164 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1164 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1165 "compiler/main.gem"
    GemVal _t3564[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3565 = (*gem_v_compile_stmt);
    GemVal _t3566[] = {gem_v_b, _t3565.fn(_t3565.env, _t3564, 2)};
            (void)(gem_buf_push_fn(NULL, _t3566, 2));
#line 1166 "compiler/main.gem"
    GemVal _t3567[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3567, 2));
        }

#line 1168 "compiler/main.gem"
    GemVal _t3568[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3568, 1), gem_int(0)))) {
#line 1169 "compiler/main.gem"
    GemVal _t3569[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3570 = (*gem_v_compile_stmt_return);
    GemVal _t3571[] = {gem_v_b, _t3570.fn(_t3570.env, _t3569, 2)};
            (void)(gem_buf_push_fn(NULL, _t3571, 2));
#line 1170 "compiler/main.gem"
    GemVal _t3572[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3572, 2));
        }
#line 1172 "compiler/main.gem"
    GemVal _t3573[] = {gem_v_p};
    GemVal _t3574[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3573, 1), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3574, 2));
#line 1173 "compiler/main.gem"
    GemVal _t3575[] = {gem_v_b};
        GemVal _t3576 = gem_buf_str_fn(NULL, _t3575, 1);
        gem_pop_frame();
        return _t3576;
    } else {
    GemVal _t3577[] = {gem_v__match_59};
    GemVal _t3579;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3577, 1), gem_string("table")))) {
        _t3579 = gem_eq(gem_type_fn(NULL, _t3577, 1), gem_string("table"));
    } else {
        GemVal _t3578[] = {gem_v__match_59, gem_string("tag")};
        _t3579 = gem_has_key_fn(NULL, _t3578, 2);
    }
    GemVal _t3580;
    if (!gem_truthy(_t3579)) {
        _t3580 = _t3579;
    } else {
        _t3580 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3580)) {
#line 1175 "compiler/main.gem"
    GemVal _t3581[] = {gem_v_node, gem_v_indent};
    GemVal _t3582 = (*gem_v_compile_stmt);
    GemVal _t3583[] = {_t3582.fn(_t3582.env, _t3581, 2)};
    GemVal _t3584[] = {gem_v_p};
    GemVal _t3585[] = {gem_v_p};
        GemVal _t3586 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3583, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3584, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3585, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3586;
    } else {
    GemVal _t3587[] = {gem_v__match_59};
    GemVal _t3589;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3587, 1), gem_string("table")))) {
        _t3589 = gem_eq(gem_type_fn(NULL, _t3587, 1), gem_string("table"));
    } else {
        GemVal _t3588[] = {gem_v__match_59, gem_string("tag")};
        _t3589 = gem_has_key_fn(NULL, _t3588, 2);
    }
    GemVal _t3590;
    if (!gem_truthy(_t3589)) {
        _t3590 = _t3589;
    } else {
        _t3590 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3590)) {
#line 1177 "compiler/main.gem"
    GemVal _t3591[] = {gem_v_node, gem_v_indent};
    GemVal _t3592 = (*gem_v_compile_stmt);
    GemVal _t3593[] = {_t3592.fn(_t3592.env, _t3591, 2)};
    GemVal _t3594[] = {gem_v_p};
    GemVal _t3595[] = {gem_v_p};
        GemVal _t3596 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3593, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3594, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3595, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3596;
    } else {
    GemVal _t3597[] = {gem_v__match_59};
    GemVal _t3599;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3597, 1), gem_string("table")))) {
        _t3599 = gem_eq(gem_type_fn(NULL, _t3597, 1), gem_string("table"));
    } else {
        GemVal _t3598[] = {gem_v__match_59, gem_string("tag")};
        _t3599 = gem_has_key_fn(NULL, _t3598, 2);
    }
    GemVal _t3600;
    if (!gem_truthy(_t3599)) {
        _t3600 = _t3599;
    } else {
        _t3600 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3600)) {
#line 1179 "compiler/main.gem"
    GemVal _t3601[] = {gem_v_node, gem_v_indent};
    GemVal _t3602 = (*gem_v_compile_stmt);
    GemVal _t3603[] = {_t3602.fn(_t3602.env, _t3601, 2)};
    GemVal _t3604[] = {gem_v_p};
    GemVal _t3605[] = {gem_v_p};
        GemVal _t3606 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3603, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3604, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3605, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3606;
    } else {
    GemVal _t3607[] = {gem_v__match_59};
    GemVal _t3609;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3607, 1), gem_string("table")))) {
        _t3609 = gem_eq(gem_type_fn(NULL, _t3607, 1), gem_string("table"));
    } else {
        GemVal _t3608[] = {gem_v__match_59, gem_string("tag")};
        _t3609 = gem_has_key_fn(NULL, _t3608, 2);
    }
    GemVal _t3610;
    if (!gem_truthy(_t3609)) {
        _t3610 = _t3609;
    } else {
        _t3610 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3610)) {
#line 1181 "compiler/main.gem"
    GemVal _t3611[] = {gem_v_node, gem_v_indent};
    GemVal _t3612 = (*gem_v_compile_stmt);
    GemVal _t3613[] = {_t3612.fn(_t3612.env, _t3611, 2)};
    GemVal _t3614[] = {gem_v_p};
    GemVal _t3615[] = {gem_v_p};
        GemVal _t3616 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3613, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3614, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3615, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3616;
    } else {
    GemVal _t3617[] = {gem_v__match_59};
    GemVal _t3619;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3617, 1), gem_string("table")))) {
        _t3619 = gem_eq(gem_type_fn(NULL, _t3617, 1), gem_string("table"));
    } else {
        GemVal _t3618[] = {gem_v__match_59, gem_string("tag")};
        _t3619 = gem_has_key_fn(NULL, _t3618, 2);
    }
    GemVal _t3620;
    if (!gem_truthy(_t3619)) {
        _t3620 = _t3619;
    } else {
        _t3620 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3620)) {
#line 1183 "compiler/main.gem"
    GemVal _t3621[] = {gem_v_node, gem_v_indent};
    GemVal _t3622 = (*gem_v_compile_stmt);
    GemVal _t3623[] = {_t3622.fn(_t3622.env, _t3621, 2)};
    GemVal _t3624[] = {gem_v_p};
    GemVal _t3625[] = {gem_v_p};
        GemVal _t3626 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3623, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3624, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3625, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3626;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1186 "compiler/main.gem"
    GemVal _t3627[] = {gem_v_node};
    GemVal _t3628 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3628.fn(_t3628.env, _t3627, 1);
#line 1187 "compiler/main.gem"
    GemVal _t3629 = (*gem_v_tmp);
    GemVal gem_v_t = _t3629.fn(_t3629.env, NULL, 0);
    GemVal _t3630[] = {gem_v_line_dir};
    GemVal _t3631 = gem_v_r;
    GemVal _t3632[] = {gem_table_get(_t3631, gem_string("setup"))};
    GemVal _t3633[] = {gem_v_p};
    GemVal _t3634[] = {gem_v_t};
    GemVal _t3635 = gem_v_r;
    GemVal _t3636[] = {gem_table_get(_t3635, gem_string("expr"))};
    GemVal _t3637[] = {gem_v_p};
    GemVal _t3638[] = {gem_v_p};
    GemVal _t3639[] = {gem_v_t};
    GemVal _t3640 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3630, 1), gem_to_string_fn(NULL, _t3632, 1)), gem_to_string_fn(NULL, _t3633, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3634, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3636, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3637, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3638, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3639, 1)), gem_string(";"));
    gem_pop_frame();
    return _t3640;
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
#line 1194 "compiler/main.gem"
    GemVal _t3642[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3642, 1);
#line 1195 "compiler/main.gem"
    GemVal _t3643 = gem_v_node;
    GemVal _t3644[] = {gem_table_get(_t3643, gem_string("cond"))};
    GemVal _t3645 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3645.fn(_t3645.env, _t3644, 1);
#line 1196 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1197 "compiler/main.gem"
    GemVal _t3646 = gem_v_cond_r;
    GemVal _t3647[] = {gem_v_b, gem_table_get(_t3646, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3647, 2));
#line 1198 "compiler/main.gem"
    GemVal _t3648[] = {gem_v_p};
    GemVal _t3649 = gem_v_cond_r;
    GemVal _t3650[] = {gem_table_get(_t3649, gem_string("expr"))};
    GemVal _t3651[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3648, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3650, 1)), gem_string(")) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3651, 2));
#line 1199 "compiler/main.gem"
    GemVal _t3652 = gem_v_node;
    GemVal _t3653[] = {gem_table_get(_t3652, gem_string("then")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3654 = (*gem_v_compile_stmts_tail);
    GemVal _t3655[] = {gem_v_b, _t3654.fn(_t3654.env, _t3653, 3)};
    (void)(gem_buf_push_fn(NULL, _t3655, 2));
#line 1200 "compiler/main.gem"
    GemVal _t3656 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3656, gem_string("else")), GEM_NIL))) {
#line 1201 "compiler/main.gem"
    GemVal _t3657[] = {gem_v_p};
    GemVal _t3658[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3657, 1), gem_string("} else {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3658, 2));
#line 1202 "compiler/main.gem"
    GemVal _t3659 = gem_v_node;
    GemVal _t3660[] = {gem_table_get(_t3659, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3661 = (*gem_v_compile_stmts_tail);
    GemVal _t3662[] = {gem_v_b, _t3661.fn(_t3661.env, _t3660, 3)};
        (void)(gem_buf_push_fn(NULL, _t3662, 2));
    } else {
#line 1203 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1204 "compiler/main.gem"
    GemVal _t3663[] = {gem_v_p};
    GemVal _t3664[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3663, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3664, 2));
#line 1205 "compiler/main.gem"
    GemVal _t3665 = gem_table_new();
    GemVal _t3666[] = {_t3665, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3667 = (*gem_v_compile_stmts_tail);
    GemVal _t3668[] = {gem_v_b, _t3667.fn(_t3667.env, _t3666, 3)};
            (void)(gem_buf_push_fn(NULL, _t3668, 2));
        }
    }
#line 1207 "compiler/main.gem"
    GemVal _t3669[] = {gem_v_p};
    GemVal _t3670[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3669, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3670, 2));
#line 1208 "compiler/main.gem"
    GemVal _t3671[] = {gem_v_b};
    GemVal _t3672 = gem_buf_str_fn(NULL, _t3671, 1);
    gem_pop_frame();
    return _t3672;
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
#line 1214 "compiler/main.gem"
    GemVal _t3674[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3674, 1);
#line 1215 "compiler/main.gem"
    GemVal _t3675 = gem_v_node;
    GemVal _t3676[] = {gem_table_get(_t3675, gem_string("cond"))};
    GemVal _t3677 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3677.fn(_t3677.env, _t3676, 1);
#line 1216 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1217 "compiler/main.gem"
    GemVal _t3678[] = {gem_v_p};
    GemVal _t3679[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3678, 1), gem_string("while (1) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3679, 2));
#line 1218 "compiler/main.gem"
    GemVal _t3680[] = {gem_v_p};
    GemVal _t3681[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3680, 1), gem_string("    gem_yield_check();\n"))};
    (void)(gem_buf_push_fn(NULL, _t3681, 2));
#line 1219 "compiler/main.gem"
    GemVal _t3682 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3682, gem_string("setup")), gem_string("")))) {
#line 1220 "compiler/main.gem"
    GemVal _t3683 = gem_v_cond_r;
    GemVal _t3684[] = {gem_v_p};
    GemVal _t3685[] = {gem_table_get(_t3683, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t3684, 1), gem_string("    "))};
    GemVal _t3686[] = {gem_v_b, gem_str_replace_fn(NULL, _t3685, 3)};
        (void)(gem_buf_push_fn(NULL, _t3686, 2));
    }
#line 1222 "compiler/main.gem"
    GemVal _t3687[] = {gem_v_p};
    GemVal _t3688 = gem_v_cond_r;
    GemVal _t3689[] = {gem_table_get(_t3688, gem_string("expr"))};
    GemVal _t3690[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3687, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t3689, 1)), gem_string(")) break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3690, 2));
#line 1223 "compiler/main.gem"
    GemVal _t3691 = gem_v_node;
    GemVal _t3692[] = {gem_table_get(_t3691, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3693 = (*gem_v_compile_stmts);
    GemVal _t3694[] = {gem_v_b, _t3693.fn(_t3693.env, _t3692, 2)};
    (void)(gem_buf_push_fn(NULL, _t3694, 2));
#line 1224 "compiler/main.gem"
    GemVal _t3695[] = {gem_v_p};
    GemVal _t3696[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3695, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3696, 2));
#line 1225 "compiler/main.gem"
    GemVal _t3697[] = {gem_v_b};
    GemVal _t3698 = gem_buf_str_fn(NULL, _t3697, 1);
    gem_pop_frame();
    return _t3698;
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
#line 1231 "compiler/main.gem"
    GemVal _t3700[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3700, 1);
#line 1232 "compiler/main.gem"
    GemVal _t3701 = gem_v_node;
    GemVal _t3702[] = {gem_table_get(_t3701, gem_string("target"))};
    GemVal _t3703 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3703.fn(_t3703.env, _t3702, 1);
#line 1233 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1234 "compiler/main.gem"
    GemVal _t3704 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3704, gem_string("target_var")), GEM_NIL))) {
#line 1235 "compiler/main.gem"
    GemVal _t3705 = gem_v_node;
    GemVal _t3706[] = {gem_table_get(_t3705, gem_string("target_var"))};
    GemVal _t3707 = (*gem_v_mangle);
        gem_v_t = _t3707.fn(_t3707.env, _t3706, 1);
    } else {
#line 1237 "compiler/main.gem"
    GemVal _t3708 = (*gem_v_tmp);
        gem_v_t = _t3708.fn(_t3708.env, NULL, 0);
    }
#line 1239 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1240 "compiler/main.gem"
    GemVal _t3709 = gem_v_target_r;
    GemVal _t3710[] = {gem_v_b, gem_table_get(_t3709, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3710, 2));
#line 1241 "compiler/main.gem"
    GemVal _t3711[] = {gem_v_p};
    GemVal _t3712[] = {gem_v_t};
    GemVal _t3713 = gem_v_target_r;
    GemVal _t3714[] = {gem_table_get(_t3713, gem_string("expr"))};
    GemVal _t3715[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3711, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3712, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3714, 1)), gem_string(";\n"))};
    (void)(gem_buf_push_fn(NULL, _t3715, 2));
#line 1242 "compiler/main.gem"
    GemVal _t3716 = gem_v_node;
    GemVal _t3717[] = {gem_table_get(_t3716, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3717, 1), gem_int(0)))) {
#line 1243 "compiler/main.gem"
    GemVal _t3718 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3718, gem_string("else")), GEM_NIL))) {
#line 1244 "compiler/main.gem"
    GemVal _t3719 = gem_v_node;
    GemVal _t3720[] = {gem_table_get(_t3719, gem_string("else")), gem_v_indent, gem_v_return_pos};
    GemVal _t3721 = (*gem_v_compile_stmts_tail);
    GemVal _t3722[] = {gem_v_b, _t3721.fn(_t3721.env, _t3720, 3)};
            (void)(gem_buf_push_fn(NULL, _t3722, 2));
        } else {
#line 1245 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1246 "compiler/main.gem"
    GemVal _t3723[] = {gem_v_p};
    GemVal _t3724[] = {gem_v_p};
    GemVal _t3725[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3723, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3724, 1)), gem_string("return GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3725, 2));
            }
        }
#line 1248 "compiler/main.gem"
    GemVal _t3726[] = {gem_v_p};
    GemVal _t3727[] = {gem_v_t};
    GemVal _t3728[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3726, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3727, 1)), gem_string(";"))};
        (void)(gem_buf_push_fn(NULL, _t3728, 2));
#line 1249 "compiler/main.gem"
    GemVal _t3729[] = {gem_v_b};
        GemVal _t3730 = gem_buf_str_fn(NULL, _t3729, 1);
        gem_pop_frame();
        return _t3730;
    }
#line 1251 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1251 "compiler/main.gem"
    GemVal _t3731 = gem_v_node;
    GemVal _t3732[] = {gem_table_get(_t3731, gem_string("whens"))};
    GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t3732, 1);
#line 1251 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1251 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_62;
#line 1251 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1252 "compiler/main.gem"
    GemVal _t3733 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3733, gem_string("whens")), gem_v_i);
#line 1253 "compiler/main.gem"
    GemVal _t3734 = gem_v_w;
    GemVal _t3735[] = {gem_table_get(_t3734, gem_string("value"))};
    GemVal _t3736 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3736.fn(_t3736.env, _t3735, 1);
#line 1254 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1255 "compiler/main.gem"
    GemVal _t3737 = gem_v_val_r;
    GemVal _t3738[] = {gem_v_b, gem_table_get(_t3737, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3738, 2));
#line 1256 "compiler/main.gem"
    GemVal _t3739[] = {gem_v_p};
    GemVal _t3740 = gem_v_val_r;
    GemVal _t3741[] = {gem_table_get(_t3740, gem_string("expr"))};
    GemVal _t3742[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3739, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3741, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3742, 2));
        } else {
#line 1258 "compiler/main.gem"
    GemVal _t3743[] = {gem_v_p};
    GemVal _t3744[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3743, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3744, 2));
#line 1259 "compiler/main.gem"
    GemVal _t3745 = gem_v_val_r;
    GemVal _t3746[] = {gem_v_b, gem_table_get(_t3745, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3746, 2));
#line 1260 "compiler/main.gem"
    GemVal _t3747[] = {gem_v_p};
    GemVal _t3748 = gem_v_val_r;
    GemVal _t3749[] = {gem_table_get(_t3748, gem_string("expr"))};
    GemVal _t3750[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3747, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3749, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3750, 2));
        }
#line 1262 "compiler/main.gem"
    GemVal _t3751 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3751, gem_string("bindings")), GEM_NIL))) {
#line 1263 "compiler/main.gem"
    GemVal _t3752 = gem_v_w;
            GemVal gem_v__for_items_61 = gem_table_get(_t3752, gem_string("bindings"));
#line 1263 "compiler/main.gem"
            GemVal gem_v__for_i_61 = gem_int(0);
#line 1263 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3753[] = {gem_v__for_items_61};
                if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3753, 1)))) break;
#line 1263 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1263 "compiler/main.gem"
                gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1264 "compiler/main.gem"
    GemVal _t3754[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3755 = (*gem_v_compile_stmt);
    GemVal _t3756[] = {gem_v_b, _t3755.fn(_t3755.env, _t3754, 2)};
                (void)(gem_buf_push_fn(NULL, _t3756, 2));
#line 1265 "compiler/main.gem"
    GemVal _t3757[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3757, 2));
            }

        }
#line 1268 "compiler/main.gem"
    GemVal _t3758 = gem_v_w;
    GemVal _t3759[] = {gem_table_get(_t3758, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3760 = (*gem_v_compile_stmts_tail);
    GemVal _t3761[] = {gem_v_b, _t3760.fn(_t3760.env, _t3759, 3)};
        (void)(gem_buf_push_fn(NULL, _t3761, 2));
    }

#line 1270 "compiler/main.gem"
    GemVal _t3762 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3762, gem_string("else")), GEM_NIL))) {
#line 1271 "compiler/main.gem"
    GemVal _t3763[] = {gem_v_p};
    GemVal _t3764[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3763, 1), gem_string("} else {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3764, 2));
#line 1272 "compiler/main.gem"
    GemVal _t3765 = gem_v_node;
    GemVal _t3766[] = {gem_table_get(_t3765, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3767 = (*gem_v_compile_stmts_tail);
    GemVal _t3768[] = {gem_v_b, _t3767.fn(_t3767.env, _t3766, 3)};
        (void)(gem_buf_push_fn(NULL, _t3768, 2));
    } else {
#line 1273 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1274 "compiler/main.gem"
    GemVal _t3769[] = {gem_v_p};
    GemVal _t3770[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3769, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3770, 2));
#line 1275 "compiler/main.gem"
    GemVal _t3771 = gem_table_new();
    GemVal _t3772[] = {_t3771, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3773 = (*gem_v_compile_stmts_tail);
    GemVal _t3774[] = {gem_v_b, _t3773.fn(_t3773.env, _t3772, 3)};
            (void)(gem_buf_push_fn(NULL, _t3774, 2));
        }
    }
#line 1277 "compiler/main.gem"
    GemVal _t3775[] = {gem_v_p};
    GemVal _t3776[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3775, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3776, 2));
#line 1278 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(1);
#line 1278 "compiler/main.gem"
    GemVal _t3777 = gem_v_node;
    GemVal _t3778[] = {gem_table_get(_t3777, gem_string("whens"))};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3778, 1);
#line 1278 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1278 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1278 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1279 "compiler/main.gem"
    GemVal _t3779[] = {gem_v_p};
    GemVal _t3780[] = {gem_v_b, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3779, 1)), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3780, 2));
    }

#line 1281 "compiler/main.gem"
    GemVal _t3781[] = {gem_v_b};
    GemVal _t3782 = gem_buf_str_fn(NULL, _t3781, 1);
    gem_pop_frame();
    return _t3782;
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
#line 1287 "compiler/main.gem"
    GemVal _t3784[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3784, 1);
#line 1288 "compiler/main.gem"
    GemVal _t3785 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3785, gem_string("arms"));
#line 1289 "compiler/main.gem"
    GemVal _t3786 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3786, gem_string("after_ms"));
#line 1290 "compiler/main.gem"
    GemVal _t3787 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3787, gem_string("after_body"));
#line 1292 "compiler/main.gem"
    GemVal _t3788 = gem_v_node;
    GemVal _t3789[] = {gem_table_get(_t3788, gem_string("recv_var"))};
    GemVal _t3790 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3790.fn(_t3790.env, _t3789, 1);
#line 1293 "compiler/main.gem"
    GemVal _t3791 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3791.fn(_t3791.env, NULL, 0);
#line 1294 "compiler/main.gem"
    GemVal _t3792 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3792.fn(_t3792.env, NULL, 0);
#line 1295 "compiler/main.gem"
    GemVal _t3793 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3793.fn(_t3793.env, NULL, 0);
#line 1296 "compiler/main.gem"
    GemVal _t3794 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3794.fn(_t3794.env, NULL, 0);
#line 1297 "compiler/main.gem"
    GemVal _t3795 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3795.fn(_t3795.env, NULL, 0);
#line 1299 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1301 "compiler/main.gem"
    GemVal _t3796[] = {gem_v_p};
    GemVal _t3797[] = {gem_v_recv_tmp};
    GemVal _t3798[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3796, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3797, 1)), gem_string(" = GEM_NIL;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3798, 2));
#line 1302 "compiler/main.gem"
    GemVal _t3799[] = {gem_v_p};
    GemVal _t3800[] = {gem_v_matched_tmp};
    GemVal _t3801[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3799, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3800, 1)), gem_string(" = -1;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3801, 2));
#line 1304 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1305 "compiler/main.gem"
    GemVal _t3802[] = {gem_v_after_ms};
    GemVal _t3803 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3803.fn(_t3803.env, _t3802, 1);
#line 1306 "compiler/main.gem"
    GemVal _t3804 = gem_v_ms_r;
    GemVal _t3805[] = {gem_v_b, gem_table_get(_t3804, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t3805, 2));
#line 1307 "compiler/main.gem"
    GemVal _t3806[] = {gem_v_p};
    GemVal _t3807[] = {gem_v_deadline_tmp};
    GemVal _t3808 = gem_v_ms_r;
    GemVal _t3809[] = {gem_table_get(_t3808, gem_string("expr"))};
    GemVal _t3810[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3806, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3807, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3809, 1)), gem_string(").ival;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3810, 2));
    }
#line 1310 "compiler/main.gem"
    GemVal _t3811[] = {gem_v_p};
    GemVal _t3812[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3811, 1), gem_string("while (1) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3812, 2));
#line 1311 "compiler/main.gem"
    GemVal _t3813[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn__mod_codegen_pad_for(NULL, _t3813, 1);
#line 1313 "compiler/main.gem"
    GemVal _t3814[] = {gem_v_p2};
    GemVal _t3815[] = {gem_v_proc_tmp};
    GemVal _t3816[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3814, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3815, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n"))};
    (void)(gem_buf_push_fn(NULL, _t3816, 2));
#line 1314 "compiler/main.gem"
    GemVal _t3817[] = {gem_v_p2};
    GemVal _t3818[] = {gem_v_prev_tmp};
    GemVal _t3819[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3817, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3818, 1)), gem_string(" = NULL;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3819, 2));
#line 1315 "compiler/main.gem"
    GemVal _t3820[] = {gem_v_p2};
    GemVal _t3821[] = {gem_v_cur_tmp};
    GemVal _t3822[] = {gem_v_proc_tmp};
    GemVal _t3823[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3820, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3821, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3822, 1)), gem_string("->mailbox.head;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3823, 2));
#line 1316 "compiler/main.gem"
    GemVal _t3824[] = {gem_v_p2};
    GemVal _t3825[] = {gem_v_matched_tmp};
    GemVal _t3826[] = {gem_v_b, gem_add(gem_add(gem_to_string_fn(NULL, _t3824, 1), gem_to_string_fn(NULL, _t3825, 1)), gem_string(" = -1;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3826, 2));
#line 1317 "compiler/main.gem"
    GemVal _t3827[] = {gem_v_p2};
    GemVal _t3828[] = {gem_v_cur_tmp};
    GemVal _t3829[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3827, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3828, 1)), gem_string(") {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3829, 2));
#line 1318 "compiler/main.gem"
    GemVal _t3830[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn__mod_codegen_pad_for(NULL, _t3830, 1);
#line 1319 "compiler/main.gem"
    GemVal _t3831[] = {gem_v_p3};
    GemVal _t3832[] = {gem_v_recv_tmp};
    GemVal _t3833[] = {gem_v_cur_tmp};
    GemVal _t3834[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3831, 1), gem_to_string_fn(NULL, _t3832, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3833, 1)), gem_string("->value;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3834, 2));
#line 1321 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1321 "compiler/main.gem"
    GemVal _t3835[] = {gem_v_arms};
    GemVal gem_v__for_limit_64 = gem_len_fn(NULL, _t3835, 1);
#line 1321 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1321 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_64;
#line 1321 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1322 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1323 "compiler/main.gem"
    GemVal _t3836 = gem_v_arm;
    GemVal _t3837 = gem_table_get(_t3836, gem_string("pattern"));
    GemVal _t3838[] = {gem_table_get(_t3837, gem_string("condition"))};
    GemVal _t3839 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3839.fn(_t3839.env, _t3838, 1);
#line 1324 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1325 "compiler/main.gem"
    GemVal _t3840 = gem_v_cond_r;
    GemVal _t3841[] = {gem_v_b, gem_table_get(_t3840, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3841, 2));
#line 1326 "compiler/main.gem"
    GemVal _t3842[] = {gem_v_p3};
    GemVal _t3843 = gem_v_cond_r;
    GemVal _t3844[] = {gem_table_get(_t3843, gem_string("expr"))};
    GemVal _t3845[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3842, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3844, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3845, 2));
        } else {
#line 1328 "compiler/main.gem"
    GemVal _t3846[] = {gem_v_p3};
    GemVal _t3847[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3846, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3847, 2));
#line 1329 "compiler/main.gem"
    GemVal _t3848 = gem_v_cond_r;
    GemVal _t3849[] = {gem_v_b, gem_table_get(_t3848, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3849, 2));
#line 1330 "compiler/main.gem"
    GemVal _t3850[] = {gem_v_p3};
    GemVal _t3851 = gem_v_cond_r;
    GemVal _t3852[] = {gem_table_get(_t3851, gem_string("expr"))};
    GemVal _t3853[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3850, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3852, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3853, 2));
        }
#line 1332 "compiler/main.gem"
    GemVal _t3854[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3855[] = {gem_fn__mod_codegen_pad_for(NULL, _t3854, 1)};
    GemVal _t3856[] = {gem_v_matched_tmp};
    GemVal _t3857[] = {gem_v_i};
    GemVal _t3858[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3855, 1), gem_to_string_fn(NULL, _t3856, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3857, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3858, 2));
    }

#line 1334 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1334 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_arms};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3859, 1);
#line 1334 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1334 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1334 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1335 "compiler/main.gem"
    GemVal _t3860[] = {gem_v_p3};
    GemVal _t3861[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3860, 1), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3861, 2));
#line 1336 "compiler/main.gem"
    GemVal _t3862[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3862, 1), gem_int(1))))) {
#line 1337 "compiler/main.gem"
    GemVal _t3863[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3863, 2));
        }
    }

#line 1340 "compiler/main.gem"
    GemVal _t3864[] = {gem_v_b, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t3864, 2));
#line 1342 "compiler/main.gem"
    GemVal _t3865[] = {gem_v_p3};
    GemVal _t3866[] = {gem_v_matched_tmp};
    GemVal _t3867[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3865, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3866, 1)), gem_string(" >= 0) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3867, 2));
#line 1343 "compiler/main.gem"
    GemVal _t3868[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn__mod_codegen_pad_for(NULL, _t3868, 1);
#line 1344 "compiler/main.gem"
    GemVal _t3869[] = {gem_v_p4};
    GemVal _t3870[] = {gem_v_proc_tmp};
    GemVal _t3871[] = {gem_v_prev_tmp};
    GemVal _t3872[] = {gem_v_cur_tmp};
    GemVal _t3873[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3869, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3870, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3871, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3872, 1)), gem_string(");\n"))};
    (void)(gem_buf_push_fn(NULL, _t3873, 2));
#line 1345 "compiler/main.gem"
    GemVal _t3874[] = {gem_v_p4};
    GemVal _t3875[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3874, 1), gem_string("break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3875, 2));
#line 1346 "compiler/main.gem"
    GemVal _t3876[] = {gem_v_p3};
    GemVal _t3877[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3876, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3877, 2));
#line 1348 "compiler/main.gem"
    GemVal _t3878[] = {gem_v_p3};
    GemVal _t3879[] = {gem_v_prev_tmp};
    GemVal _t3880[] = {gem_v_cur_tmp};
    GemVal _t3881[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3878, 1), gem_to_string_fn(NULL, _t3879, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3880, 1)), gem_string(";\n"))};
    (void)(gem_buf_push_fn(NULL, _t3881, 2));
#line 1349 "compiler/main.gem"
    GemVal _t3882[] = {gem_v_p3};
    GemVal _t3883[] = {gem_v_cur_tmp};
    GemVal _t3884[] = {gem_v_cur_tmp};
    GemVal _t3885[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3882, 1), gem_to_string_fn(NULL, _t3883, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3884, 1)), gem_string("->next;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3885, 2));
#line 1350 "compiler/main.gem"
    GemVal _t3886[] = {gem_v_p2};
    GemVal _t3887[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3886, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3887, 2));
#line 1352 "compiler/main.gem"
    GemVal _t3888[] = {gem_v_p2};
    GemVal _t3889[] = {gem_v_matched_tmp};
    GemVal _t3890[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3888, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3889, 1)), gem_string(" >= 0) break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3890, 2));
#line 1353 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1354 "compiler/main.gem"
    GemVal _t3891[] = {gem_v_p2};
    GemVal _t3892[] = {gem_v_deadline_tmp};
    GemVal _t3893[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3891, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3892, 1)), gem_string(") {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3893, 2));
#line 1355 "compiler/main.gem"
    GemVal _t3894[] = {gem_v_p2};
    GemVal _t3895[] = {gem_v_matched_tmp};
    GemVal _t3896[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3894, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3895, 1)), gem_string(" = -2;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3896, 2));
#line 1356 "compiler/main.gem"
    GemVal _t3897[] = {gem_v_p2};
    GemVal _t3898[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3897, 1), gem_string("    break;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3898, 2));
#line 1357 "compiler/main.gem"
    GemVal _t3899[] = {gem_v_p2};
    GemVal _t3900[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3899, 1), gem_string("}\n"))};
        (void)(gem_buf_push_fn(NULL, _t3900, 2));
#line 1358 "compiler/main.gem"
    GemVal _t3901[] = {gem_v_p2};
    GemVal _t3902[] = {gem_v_deadline_tmp};
    GemVal _t3903[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3901, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3902, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t3903, 2));
#line 1359 "compiler/main.gem"
    GemVal _t3904[] = {gem_v_p2};
    GemVal _t3905[] = {gem_v_proc_tmp};
    GemVal _t3906[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3904, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3905, 1)), gem_string("->timed_out) {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3906, 2));
#line 1360 "compiler/main.gem"
    GemVal _t3907[] = {gem_v_p2};
    GemVal _t3908[] = {gem_v_matched_tmp};
    GemVal _t3909[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3907, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3908, 1)), gem_string(" = -2;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3909, 2));
#line 1361 "compiler/main.gem"
    GemVal _t3910[] = {gem_v_p2};
    GemVal _t3911[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3910, 1), gem_string("    break;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3911, 2));
#line 1362 "compiler/main.gem"
    GemVal _t3912[] = {gem_v_p2};
    GemVal _t3913[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3912, 1), gem_string("}\n"))};
        (void)(gem_buf_push_fn(NULL, _t3913, 2));
    } else {
#line 1364 "compiler/main.gem"
    GemVal _t3914[] = {gem_v_p2};
    GemVal _t3915[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3914, 1), gem_string("gem_selective_yield(-1);\n"))};
        (void)(gem_buf_push_fn(NULL, _t3915, 2));
    }
#line 1367 "compiler/main.gem"
    GemVal _t3916[] = {gem_v_p};
    GemVal _t3917[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3916, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3917, 2));
#line 1370 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1370 "compiler/main.gem"
    GemVal _t3918[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3918, 1);
#line 1370 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1370 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1370 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1371 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1372 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1373 "compiler/main.gem"
    GemVal _t3919[] = {gem_v_p};
    GemVal _t3920[] = {gem_v_matched_tmp};
    GemVal _t3921[] = {gem_v_i};
    GemVal _t3922[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3919, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3920, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3921, 1)), gem_string(") {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3922, 2));
        } else {
#line 1375 "compiler/main.gem"
    GemVal _t3923[] = {gem_v_p};
    GemVal _t3924[] = {gem_v_matched_tmp};
    GemVal _t3925[] = {gem_v_i};
    GemVal _t3926[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3923, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3924, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3925, 1)), gem_string(") {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3926, 2));
        }
#line 1377 "compiler/main.gem"
    GemVal _t3927 = gem_v_arm;
    GemVal _t3928 = gem_table_get(_t3927, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3928, gem_string("bindings")), GEM_NIL))) {
#line 1378 "compiler/main.gem"
    GemVal _t3929 = gem_v_arm;
    GemVal _t3930 = gem_table_get(_t3929, gem_string("pattern"));
            GemVal gem_v__for_items_66 = gem_table_get(_t3930, gem_string("bindings"));
#line 1378 "compiler/main.gem"
            GemVal gem_v__for_i_66 = gem_int(0);
#line 1378 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3931[] = {gem_v__for_items_66};
                if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t3931, 1)))) break;
#line 1378 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1378 "compiler/main.gem"
                gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1379 "compiler/main.gem"
    GemVal _t3932[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3933 = (*gem_v_compile_stmt);
    GemVal _t3934[] = {gem_v_b, _t3933.fn(_t3933.env, _t3932, 2)};
                (void)(gem_buf_push_fn(NULL, _t3934, 2));
#line 1380 "compiler/main.gem"
    GemVal _t3935[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3935, 2));
            }

        }
#line 1383 "compiler/main.gem"
    GemVal _t3936 = gem_v_arm;
    GemVal _t3937[] = {gem_table_get(_t3936, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3938 = (*gem_v_compile_stmts_tail);
    GemVal _t3939[] = {gem_v_b, _t3938.fn(_t3938.env, _t3937, 3)};
        (void)(gem_buf_push_fn(NULL, _t3939, 2));
    }

#line 1387 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1388 "compiler/main.gem"
    GemVal _t3940[] = {gem_v_p};
    GemVal _t3941[] = {gem_v_matched_tmp};
    GemVal _t3942[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3940, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3941, 1)), gem_string(" == -2) {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3942, 2));
#line 1389 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1390 "compiler/main.gem"
    GemVal _t3943[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3944 = (*gem_v_compile_stmts_tail);
    GemVal _t3945[] = {gem_v_b, _t3944.fn(_t3944.env, _t3943, 3)};
            (void)(gem_buf_push_fn(NULL, _t3945, 2));
        } else {
#line 1391 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1392 "compiler/main.gem"
    GemVal _t3946 = gem_table_new();
    GemVal _t3947[] = {_t3946, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3948 = (*gem_v_compile_stmts_tail);
    GemVal _t3949[] = {gem_v_b, _t3948.fn(_t3948.env, _t3947, 3)};
                (void)(gem_buf_push_fn(NULL, _t3949, 2));
            }
        }
    } else {
#line 1394 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1395 "compiler/main.gem"
    GemVal _t3950[] = {gem_v_p};
    GemVal _t3951[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3950, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3951, 2));
#line 1396 "compiler/main.gem"
    GemVal _t3952 = gem_table_new();
    GemVal _t3953[] = {_t3952, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3954 = (*gem_v_compile_stmts_tail);
    GemVal _t3955[] = {gem_v_b, _t3954.fn(_t3954.env, _t3953, 3)};
            (void)(gem_buf_push_fn(NULL, _t3955, 2));
        }
    }
#line 1399 "compiler/main.gem"
    GemVal _t3956[] = {gem_v_p};
    GemVal _t3957[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3956, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3957, 2));
#line 1400 "compiler/main.gem"
    GemVal _t3958[] = {gem_v_b};
    GemVal _t3959 = gem_buf_str_fn(NULL, _t3958, 1);
    gem_pop_frame();
    return _t3959;
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
#line 1407 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1408 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1409 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1410 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1411 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1412 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1413 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1414 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1415 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1416 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1417 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1418 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1419 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1422 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1423 "compiler/main.gem"
    GemVal _t3961[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3961, 1), gem_int(0)))) {
#line 1424 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1426 "compiler/main.gem"
        GemVal gem_v__for_i_68 = gem_int(0);
#line 1426 "compiler/main.gem"
    GemVal _t3962[] = {gem_v_params};
        GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t3962, 1);
#line 1426 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1426 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_68;
#line 1426 "compiler/main.gem"
            gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1427 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1428 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1430 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1431 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1432 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1433 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1434 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1435 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1436 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1437 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1438 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1439 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1440 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1441 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1442 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1444 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1449 "compiler/main.gem"
    GemVal _t3963[] = {gem_v_c_ret};
    GemVal _t3964[] = {gem_v_name};
    GemVal _t3965[] = {gem_v_c_params};
    GemVal _t3966[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3963, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3964, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3965, 1)), gem_string(");"))};
    GemVal _t3967 = gem_push_fn(NULL, _t3966, 2);
    gem_pop_frame();
    return _t3967;
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
#line 1453 "compiler/main.gem"
    GemVal gem_v__d69 = gem_v_node;
#line 1453 "compiler/main.gem"
    GemVal _t3969 = gem_v__d69;
    GemVal gem_v_name = gem_table_get(_t3969, gem_string("name"));
#line 1453 "compiler/main.gem"
    GemVal _t3970 = gem_v__d69;
    GemVal gem_v_params = gem_table_get(_t3970, gem_string("params"));
#line 1453 "compiler/main.gem"
    GemVal _t3971 = gem_v__d69;
    GemVal gem_v_ret_type = gem_table_get(_t3971, gem_string("ret_type"));

#line 1455 "compiler/main.gem"
    GemVal _t3972 = gem_v_node;
    if (gem_truthy(gem_table_get(_t3972, gem_string("blocking")))) {
#line 1456 "compiler/main.gem"
    GemVal _t3973[] = {gem_v_node};
    GemVal _t3974 = (*gem_v_compile_blocking_extern_fn);
        GemVal _t3975 = _t3974.fn(_t3974.env, _t3973, 1);
        gem_pop_frame();
        return _t3975;
    }
#line 1459 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1460 "compiler/main.gem"
    GemVal _t3976[] = {gem_v_name};
    GemVal _t3977[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3976, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3977, 2));
#line 1462 "compiler/main.gem"
    GemVal _t3978 = gem_table_new();
    GemVal gem_v_c_args = _t3978;
#line 1463 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1463 "compiler/main.gem"
    GemVal _t3979[] = {gem_v_params};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3979, 1);
#line 1463 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1463 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1463 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1464 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1465 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1466 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1467 "compiler/main.gem"
    GemVal _t3980[] = {gem_v_i};
    GemVal _t3981[] = {gem_v_i};
    GemVal _t3982[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3980, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3981, 1)), gem_string("].ival;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3982, 2));
#line 1468 "compiler/main.gem"
    GemVal _t3983[] = {gem_v_i};
    GemVal _t3984[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3983, 1))};
            (void)(gem_push_fn(NULL, _t3984, 2));
        } else {
#line 1469 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1470 "compiler/main.gem"
    GemVal _t3985[] = {gem_v_i};
    GemVal _t3986[] = {gem_v_i};
    GemVal _t3987[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3985, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3986, 1)), gem_string("].fval;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3987, 2));
#line 1471 "compiler/main.gem"
    GemVal _t3988[] = {gem_v_i};
    GemVal _t3989[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3988, 1))};
                (void)(gem_push_fn(NULL, _t3989, 2));
            } else {
#line 1472 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1473 "compiler/main.gem"
    GemVal _t3990[] = {gem_v_i};
    GemVal _t3991[] = {gem_v_i};
    GemVal _t3992[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3990, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3991, 1)), gem_string("].sval;\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3992, 2));
#line 1474 "compiler/main.gem"
    GemVal _t3993[] = {gem_v_i};
    GemVal _t3994[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3993, 1))};
                    (void)(gem_push_fn(NULL, _t3994, 2));
                } else {
#line 1475 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1476 "compiler/main.gem"
    GemVal _t3995[] = {gem_v_i};
    GemVal _t3996[] = {gem_v_i};
    GemVal _t3997[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3995, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3996, 1)), gem_string("].bval;\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3997, 2));
#line 1477 "compiler/main.gem"
    GemVal _t3998[] = {gem_v_i};
    GemVal _t3999[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3998, 1))};
                        (void)(gem_push_fn(NULL, _t3999, 2));
                    } else {
#line 1478 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1479 "compiler/main.gem"
    GemVal _t4000[] = {gem_v_i};
    GemVal _t4001[] = {gem_v_i};
    GemVal _t4002[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t4000, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t4001, 1)), gem_string("].ival;\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4002, 2));
#line 1480 "compiler/main.gem"
    GemVal _t4003[] = {gem_v_i};
    GemVal _t4004[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t4003, 1))};
                            (void)(gem_push_fn(NULL, _t4004, 2));
                        } else {
#line 1481 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1482 "compiler/main.gem"
    GemVal _t4005[] = {gem_v_i};
    GemVal _t4006[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t4005, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t4006, 2));
                            } else {
#line 1484 "compiler/main.gem"
    GemVal _t4007[] = {gem_v_i};
    GemVal _t4008[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t4007, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t4008, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1489 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1490 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1490 "compiler/main.gem"
    GemVal _t4009[] = {gem_v_c_args};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t4009, 1);
#line 1490 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1490 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1490 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1491 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1492 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1494 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1497 "compiler/main.gem"
    GemVal _t4010[] = {gem_v_name};
    GemVal _t4011[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t4010, 1), gem_string("(")), gem_to_string_fn(NULL, _t4011, 1)), gem_string(")"));
#line 1499 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1500 "compiler/main.gem"
    GemVal _t4012[] = {gem_v_call_expr};
    GemVal _t4013[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4012, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4013, 2));
#line 1501 "compiler/main.gem"
    GemVal _t4014[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4014, 2));
    } else {
#line 1502 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1503 "compiler/main.gem"
    GemVal _t4015[] = {gem_v_call_expr};
    GemVal _t4016[] = {gem_v_b, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t4015, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4016, 2));
        } else {
#line 1504 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1505 "compiler/main.gem"
    GemVal _t4017[] = {gem_v_call_expr};
    GemVal _t4018[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t4017, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t4018, 2));
#line 1506 "compiler/main.gem"
    GemVal _t4019[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t4019, 2));
            } else {
#line 1507 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1508 "compiler/main.gem"
    GemVal _t4020[] = {gem_v_call_expr};
    GemVal _t4021[] = {gem_v_b, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t4020, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4021, 2));
#line 1509 "compiler/main.gem"
    GemVal _t4022[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t4022, 2));
                } else {
#line 1510 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1511 "compiler/main.gem"
    GemVal _t4023[] = {gem_v_call_expr};
    GemVal _t4024[] = {gem_v_b, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t4023, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4024, 2));
#line 1512 "compiler/main.gem"
    GemVal _t4025[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t4025, 2));
                    } else {
#line 1513 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1514 "compiler/main.gem"
    GemVal _t4026[] = {gem_v_call_expr};
    GemVal _t4027[] = {gem_v_b, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t4026, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4027, 2));
#line 1515 "compiler/main.gem"
    GemVal _t4028[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                            (void)(gem_buf_push_fn(NULL, _t4028, 2));
                        } else {
#line 1516 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1517 "compiler/main.gem"
    GemVal _t4029[] = {gem_v_call_expr};
    GemVal _t4030[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t4029, 1)), gem_string(";\n"))};
                                (void)(gem_buf_push_fn(NULL, _t4030, 2));
#line 1518 "compiler/main.gem"
    GemVal _t4031[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                                (void)(gem_buf_push_fn(NULL, _t4031, 2));
                            } else {
#line 1520 "compiler/main.gem"
    GemVal _t4032[] = {gem_v_call_expr};
    GemVal _t4033[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4032, 1)), gem_string(";\n"))};
                                (void)(gem_buf_push_fn(NULL, _t4033, 2));
#line 1521 "compiler/main.gem"
    GemVal _t4034[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                                (void)(gem_buf_push_fn(NULL, _t4034, 2));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1524 "compiler/main.gem"
    GemVal _t4035[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4035, 2));
#line 1526 "compiler/main.gem"
    GemVal _t4036[] = {gem_v_name};
    GemVal _t4037[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4036, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t4037, 2));
#line 1527 "compiler/main.gem"
    GemVal _t4038[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t4039 = (*gem_v_emit_extern_c_decl);
    (void)(_t4039.fn(_t4039.env, _t4038, 3));
#line 1529 "compiler/main.gem"
    GemVal _t4040[] = {gem_v_b};
    GemVal _t4041 = gem_buf_str_fn(NULL, _t4040, 1);
    gem_pop_frame();
    return _t4041;
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
#line 1536 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1536 "compiler/main.gem"
    GemVal _t4043 = gem_v__d72;
    GemVal gem_v_name = gem_table_get(_t4043, gem_string("name"));
#line 1536 "compiler/main.gem"
    GemVal _t4044 = gem_v__d72;
    GemVal gem_v_params = gem_table_get(_t4044, gem_string("params"));
#line 1536 "compiler/main.gem"
    GemVal _t4045 = gem_v__d72;
    GemVal gem_v_ret_type = gem_table_get(_t4045, gem_string("ret_type"));

#line 1537 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1540 "compiler/main.gem"
    GemVal _t4046[] = {gem_v_name};
    GemVal _t4047[] = {gem_v_b, gem_add(gem_add(gem_string("struct _blk_"), gem_to_string_fn(NULL, _t4046, 1)), gem_string("_args {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4047, 2));
#line 1541 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1541 "compiler/main.gem"
    GemVal _t4048[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t4048, 1);
#line 1541 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1541 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1541 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1542 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1543 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1544 "compiler/main.gem"
    GemVal _t4049[] = {gem_v_i};
    GemVal _t4050[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t4049, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4050, 2));
        } else {
#line 1545 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1546 "compiler/main.gem"
    GemVal _t4051[] = {gem_v_i};
    GemVal _t4052[] = {gem_v_b, gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t4051, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t4052, 2));
            } else {
#line 1547 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1548 "compiler/main.gem"
    GemVal _t4053[] = {gem_v_i};
    GemVal _t4054[] = {gem_v_b, gem_add(gem_add(gem_string("    char *_p"), gem_to_string_fn(NULL, _t4053, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4054, 2));
                } else {
#line 1549 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1550 "compiler/main.gem"
    GemVal _t4055[] = {gem_v_i};
    GemVal _t4056[] = {gem_v_b, gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t4055, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4056, 2));
                    } else {
#line 1551 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1552 "compiler/main.gem"
    GemVal _t4057[] = {gem_v_i};
    GemVal _t4058[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_p"), gem_to_string_fn(NULL, _t4057, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4058, 2));
                        }
                    }
                }
            }
        }
    }

#line 1555 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1556 "compiler/main.gem"
    GemVal _t4059[] = {gem_v_b, gem_string("    int64_t _result;\n")};
        (void)(gem_buf_push_fn(NULL, _t4059, 2));
    } else {
#line 1557 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1558 "compiler/main.gem"
    GemVal _t4060[] = {gem_v_b, gem_string("    double _result;\n")};
            (void)(gem_buf_push_fn(NULL, _t4060, 2));
        } else {
#line 1559 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1560 "compiler/main.gem"
    GemVal _t4061[] = {gem_v_b, gem_string("    char *_result;\n")};
                (void)(gem_buf_push_fn(NULL, _t4061, 2));
            } else {
#line 1561 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1562 "compiler/main.gem"
    GemVal _t4062[] = {gem_v_b, gem_string("    int _result;\n")};
                    (void)(gem_buf_push_fn(NULL, _t4062, 2));
                } else {
#line 1563 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1564 "compiler/main.gem"
    GemVal _t4063[] = {gem_v_b, gem_string("    void *_result;\n")};
                        (void)(gem_buf_push_fn(NULL, _t4063, 2));
                    }
                }
            }
        }
    }
#line 1566 "compiler/main.gem"
    GemVal _t4064[] = {gem_v_b, gem_string("};\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4064, 2));
#line 1569 "compiler/main.gem"
    GemVal _t4065[] = {gem_v_name};
    GemVal _t4066[] = {gem_v_b, gem_add(gem_add(gem_string("static void _blk_"), gem_to_string_fn(NULL, _t4065, 1)), gem_string("_worker(void *_a) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4066, 2));
#line 1570 "compiler/main.gem"
    GemVal _t4067[] = {gem_v_name};
    GemVal _t4068[] = {gem_v_name};
    GemVal _t4069[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    struct _blk_"), gem_to_string_fn(NULL, _t4067, 1)), gem_string("_args *a = (struct _blk_")), gem_to_string_fn(NULL, _t4068, 1)), gem_string("_args *)_a;\n"))};
    (void)(gem_buf_push_fn(NULL, _t4069, 2));
#line 1572 "compiler/main.gem"
    GemVal gem_v_w_args = gem_string("");
#line 1573 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1573 "compiler/main.gem"
    GemVal _t4070[] = {gem_v_params};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t4070, 1);
#line 1573 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1573 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1573 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1574 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1575 "compiler/main.gem"
            gem_v_w_args = gem_add(gem_v_w_args, gem_string(", "));
        }
#line 1577 "compiler/main.gem"
    GemVal _t4071[] = {gem_v_i};
        gem_v_w_args = gem_add(gem_v_w_args, gem_add(gem_string("a->_p"), gem_to_string_fn(NULL, _t4071, 1)));
    }

#line 1579 "compiler/main.gem"
    GemVal _t4072[] = {gem_v_name};
    GemVal _t4073[] = {gem_v_w_args};
    GemVal gem_v_w_call = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t4072, 1), gem_string("(")), gem_to_string_fn(NULL, _t4073, 1)), gem_string(")"));
#line 1581 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1582 "compiler/main.gem"
    GemVal _t4074[] = {gem_v_w_call};
    GemVal _t4075[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4074, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4075, 2));
    } else {
#line 1584 "compiler/main.gem"
    GemVal _t4076[] = {gem_v_w_call};
    GemVal _t4077[] = {gem_v_b, gem_add(gem_add(gem_string("    a->_result = "), gem_to_string_fn(NULL, _t4076, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4077, 2));
    }
#line 1586 "compiler/main.gem"
    GemVal _t4078[] = {gem_v_b, gem_string("}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4078, 2));
#line 1589 "compiler/main.gem"
    GemVal _t4079[] = {gem_v_name};
    GemVal _t4080[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4079, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4080, 2));
#line 1592 "compiler/main.gem"
    GemVal gem_v__for_i_75 = gem_int(0);
#line 1592 "compiler/main.gem"
    GemVal _t4081[] = {gem_v_params};
    GemVal gem_v__for_limit_75 = gem_len_fn(NULL, _t4081, 1);
#line 1592 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1592 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_75;
#line 1592 "compiler/main.gem"
        gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1593 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1594 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1595 "compiler/main.gem"
    GemVal _t4082[] = {gem_v_i};
    GemVal _t4083[] = {gem_v_i};
    GemVal _t4084[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t4082, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4083, 1)), gem_string("].ival;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4084, 2));
        } else {
#line 1596 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1597 "compiler/main.gem"
    GemVal _t4085[] = {gem_v_i};
    GemVal _t4086[] = {gem_v_i};
    GemVal _t4087[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t4085, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4086, 1)), gem_string("].fval;\n"))};
                (void)(gem_buf_push_fn(NULL, _t4087, 2));
            } else {
#line 1598 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1599 "compiler/main.gem"
    GemVal _t4088[] = {gem_v_i};
    GemVal _t4089[] = {gem_v_i};
    GemVal _t4090[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t4088, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4089, 1)), gem_string("].sval;\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4090, 2));
                } else {
#line 1600 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1601 "compiler/main.gem"
    GemVal _t4091[] = {gem_v_i};
    GemVal _t4092[] = {gem_v_i};
    GemVal _t4093[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t4091, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4092, 1)), gem_string("].bval;\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4093, 2));
                    } else {
#line 1602 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1603 "compiler/main.gem"
    GemVal _t4094[] = {gem_v_i};
    GemVal _t4095[] = {gem_v_i};
    GemVal _t4096[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t4094, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t4095, 1)), gem_string("].ival;\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4096, 2));
                        }
                    }
                }
            }
        }
    }

#line 1608 "compiler/main.gem"
    GemVal _t4097[] = {gem_v_b, gem_string("    if (gem_current_pid >= 0) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4097, 2));
#line 1609 "compiler/main.gem"
    GemVal _t4098[] = {gem_v_name};
    GemVal _t4099[] = {gem_v_name};
    GemVal _t4100[] = {gem_v_name};
    GemVal _t4101[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("        struct _blk_"), gem_to_string_fn(NULL, _t4098, 1)), gem_string("_args *_ba = (struct _blk_")), gem_to_string_fn(NULL, _t4099, 1)), gem_string("_args *)malloc(sizeof(struct _blk_")), gem_to_string_fn(NULL, _t4100, 1)), gem_string("_args));\n"))};
    (void)(gem_buf_push_fn(NULL, _t4101, 2));
#line 1612 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1612 "compiler/main.gem"
    GemVal _t4102[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t4102, 1);
#line 1612 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1612 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1612 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1613 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1614 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1615 "compiler/main.gem"
    GemVal _t4103[] = {gem_v_i};
    GemVal _t4104[] = {gem_v_i};
    GemVal _t4105[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        _ba->_p"), gem_to_string_fn(NULL, _t4103, 1)), gem_string(" = strdup(_p")), gem_to_string_fn(NULL, _t4104, 1)), gem_string(");\n"))};
            (void)(gem_buf_push_fn(NULL, _t4105, 2));
        } else {
#line 1617 "compiler/main.gem"
    GemVal _t4106[] = {gem_v_i};
    GemVal _t4107[] = {gem_v_i};
    GemVal _t4108[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        _ba->_p"), gem_to_string_fn(NULL, _t4106, 1)), gem_string(" = _p")), gem_to_string_fn(NULL, _t4107, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4108, 2));
        }
    }

#line 1621 "compiler/main.gem"
    GemVal _t4109[] = {gem_v_name};
    GemVal _t4110[] = {gem_v_b, gem_add(gem_add(gem_string("        GemIORequest *_req = gem_io_submit_extern(_blk_"), gem_to_string_fn(NULL, _t4109, 1)), gem_string("_worker, _ba);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4110, 2));
#line 1622 "compiler/main.gem"
    GemVal _t4111[] = {gem_v_name};
    GemVal _t4112[] = {gem_v_b, gem_add(gem_add(gem_string("        if (!_req) { free(_ba); gem_error(\""), gem_to_string_fn(NULL, _t4111, 1)), gem_string(": I/O queue full\"); }\n"))};
    (void)(gem_buf_push_fn(NULL, _t4112, 2));
#line 1623 "compiler/main.gem"
    GemVal _t4113[] = {gem_v_b, gem_string("        GemProcess *_proc = &gem_proc_table[gem_current_pid];\n")};
    (void)(gem_buf_push_fn(NULL, _t4113, 2));
#line 1624 "compiler/main.gem"
    GemVal _t4114[] = {gem_v_b, gem_string("        _proc->io_request = _req;\n")};
    (void)(gem_buf_push_fn(NULL, _t4114, 2));
#line 1625 "compiler/main.gem"
    GemVal _t4115[] = {gem_v_b, gem_string("        gem_io_pool_yield();\n")};
    (void)(gem_buf_push_fn(NULL, _t4115, 2));
#line 1626 "compiler/main.gem"
    GemVal _t4116[] = {gem_v_b, gem_string("        _proc->io_request = NULL;\n")};
    (void)(gem_buf_push_fn(NULL, _t4116, 2));
#line 1629 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1630 "compiler/main.gem"
    GemVal _t4117[] = {gem_v_b, gem_string("        char *_raw = _ba->_result;\n")};
        (void)(gem_buf_push_fn(NULL, _t4117, 2));
    } else {
#line 1631 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_ret_type, gem_string("Nil")))) {
#line 1632 "compiler/main.gem"
            GemVal gem_v_c_type = gem_string("int64_t");
#line 1633 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1634 "compiler/main.gem"
                gem_v_c_type = gem_string("double");
            } else {
#line 1635 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1636 "compiler/main.gem"
                    gem_v_c_type = gem_string("int");
                } else {
#line 1637 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1638 "compiler/main.gem"
                        gem_v_c_type = gem_string("void *");
                    }
                }
            }
#line 1640 "compiler/main.gem"
    GemVal _t4118[] = {gem_v_c_type};
    GemVal _t4119[] = {gem_v_b, gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t4118, 1)), gem_string(" _ret = _ba->_result;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4119, 2));
        }
    }
#line 1644 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1644 "compiler/main.gem"
    GemVal _t4120[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t4120, 1);
#line 1644 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1644 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1644 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1645 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type")), gem_string("String")))) {
#line 1646 "compiler/main.gem"
    GemVal _t4121[] = {gem_v_i};
    GemVal _t4122[] = {gem_v_b, gem_add(gem_add(gem_string("        free(_ba->_p"), gem_to_string_fn(NULL, _t4121, 1)), gem_string(");\n"))};
            (void)(gem_buf_push_fn(NULL, _t4122, 2));
        }
    }

#line 1650 "compiler/main.gem"
    GemVal _t4123[] = {gem_v_b, gem_string("        free(_ba);\n")};
    (void)(gem_buf_push_fn(NULL, _t4123, 2));
#line 1651 "compiler/main.gem"
    GemVal _t4124[] = {gem_v_b, gem_string("        gem_io_free_request(_req);\n")};
    (void)(gem_buf_push_fn(NULL, _t4124, 2));
#line 1654 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1655 "compiler/main.gem"
    GemVal _t4125[] = {gem_v_b, gem_string("        return gem_int(_ret);\n")};
        (void)(gem_buf_push_fn(NULL, _t4125, 2));
    } else {
#line 1656 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1657 "compiler/main.gem"
    GemVal _t4126[] = {gem_v_b, gem_string("        return gem_float(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t4126, 2));
        } else {
#line 1658 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1659 "compiler/main.gem"
    GemVal _t4127[] = {gem_v_b, gem_string("        size_t _len = _raw ? strlen(_raw) : 0;\n")};
                (void)(gem_buf_push_fn(NULL, _t4127, 2));
#line 1660 "compiler/main.gem"
    GemVal _t4128[] = {gem_v_b, gem_string("        char *_gc = (char *)GC_MALLOC_ATOMIC(_len + 1);\n")};
                (void)(gem_buf_push_fn(NULL, _t4128, 2));
#line 1661 "compiler/main.gem"
    GemVal _t4129[] = {gem_v_b, gem_string("        if (_raw) { memcpy(_gc, _raw, _len + 1); free(_raw); } else { _gc[0] = '\\0'; }\n")};
                (void)(gem_buf_push_fn(NULL, _t4129, 2));
#line 1662 "compiler/main.gem"
    GemVal _t4130[] = {gem_v_b, gem_string("        return gem_string(_gc);\n")};
                (void)(gem_buf_push_fn(NULL, _t4130, 2));
            } else {
#line 1663 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1664 "compiler/main.gem"
    GemVal _t4131[] = {gem_v_b, gem_string("        return gem_bool(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t4131, 2));
                } else {
#line 1665 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1666 "compiler/main.gem"
    GemVal _t4132[] = {gem_v_b, gem_string("        GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                        (void)(gem_buf_push_fn(NULL, _t4132, 2));
                    } else {
#line 1668 "compiler/main.gem"
    GemVal _t4133[] = {gem_v_b, gem_string("        return GEM_NIL;\n")};
                        (void)(gem_buf_push_fn(NULL, _t4133, 2));
                    }
                }
            }
        }
    }
#line 1670 "compiler/main.gem"
    GemVal _t4134[] = {gem_v_b, gem_string("    }\n")};
    (void)(gem_buf_push_fn(NULL, _t4134, 2));
#line 1673 "compiler/main.gem"
    GemVal gem_v_d_args = gem_string("");
#line 1674 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1674 "compiler/main.gem"
    GemVal _t4135[] = {gem_v_params};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t4135, 1);
#line 1674 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1674 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1674 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1675 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1676 "compiler/main.gem"
            gem_v_d_args = gem_add(gem_v_d_args, gem_string(", "));
        }
#line 1678 "compiler/main.gem"
    GemVal _t4136[] = {gem_v_i};
        gem_v_d_args = gem_add(gem_v_d_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t4136, 1)));
    }

#line 1680 "compiler/main.gem"
    GemVal _t4137[] = {gem_v_name};
    GemVal _t4138[] = {gem_v_d_args};
    GemVal gem_v_d_call = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t4137, 1), gem_string("(")), gem_to_string_fn(NULL, _t4138, 1)), gem_string(")"));
#line 1682 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1683 "compiler/main.gem"
    GemVal _t4139[] = {gem_v_d_call};
    GemVal _t4140[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4139, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4140, 2));
#line 1684 "compiler/main.gem"
    GemVal _t4141[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4141, 2));
    } else {
#line 1685 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1686 "compiler/main.gem"
    GemVal _t4142[] = {gem_v_d_call};
    GemVal _t4143[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t4142, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4143, 2));
#line 1687 "compiler/main.gem"
    GemVal _t4144[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t4144, 2));
        } else {
#line 1688 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1689 "compiler/main.gem"
    GemVal _t4145[] = {gem_v_d_call};
    GemVal _t4146[] = {gem_v_b, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t4145, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t4146, 2));
#line 1690 "compiler/main.gem"
    GemVal _t4147[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t4147, 2));
            } else {
#line 1691 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1692 "compiler/main.gem"
    GemVal _t4148[] = {gem_v_d_call};
    GemVal _t4149[] = {gem_v_b, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t4148, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4149, 2));
#line 1693 "compiler/main.gem"
    GemVal _t4150[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t4150, 2));
                } else {
#line 1694 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1695 "compiler/main.gem"
    GemVal _t4151[] = {gem_v_d_call};
    GemVal _t4152[] = {gem_v_b, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t4151, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4152, 2));
#line 1696 "compiler/main.gem"
    GemVal _t4153[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t4153, 2));
                    } else {
#line 1697 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1698 "compiler/main.gem"
    GemVal _t4154[] = {gem_v_d_call};
    GemVal _t4155[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t4154, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4155, 2));
#line 1699 "compiler/main.gem"
    GemVal _t4156[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                            (void)(gem_buf_push_fn(NULL, _t4156, 2));
                        } else {
#line 1701 "compiler/main.gem"
    GemVal _t4157[] = {gem_v_d_call};
    GemVal _t4158[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4157, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4158, 2));
#line 1702 "compiler/main.gem"
    GemVal _t4159[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                            (void)(gem_buf_push_fn(NULL, _t4159, 2));
                        }
                    }
                }
            }
        }
    }
#line 1705 "compiler/main.gem"
    GemVal _t4160[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4160, 2));
#line 1707 "compiler/main.gem"
    GemVal _t4161[] = {gem_v_name};
    GemVal _t4162[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4161, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t4162, 2));
#line 1708 "compiler/main.gem"
    GemVal _t4163[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t4164 = (*gem_v_emit_extern_c_decl);
    (void)(_t4164.fn(_t4164.env, _t4163, 3));
#line 1710 "compiler/main.gem"
    GemVal _t4165[] = {gem_v_b};
    GemVal _t4166 = gem_buf_str_fn(NULL, _t4165, 1);
    gem_pop_frame();
    return _t4166;
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
#line 1716 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1718 "compiler/main.gem"
    GemVal gem_v__for_i_79 = gem_int(0);
#line 1718 "compiler/main.gem"
    GemVal _t4168[] = {gem_v_params};
    GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t4168, 1);
#line 1718 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1718 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_79;
#line 1718 "compiler/main.gem"
        gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1719 "compiler/main.gem"
    GemVal _t4169[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t4170 = (*gem_v_mangle);
        GemVal gem_v_mp = _t4170.fn(_t4170.env, _t4169, 1);
#line 1720 "compiler/main.gem"
    GemVal _t4171[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4171, 2))) {
#line 1721 "compiler/main.gem"
    GemVal _t4172[] = {gem_v_mp};
    GemVal _t4173[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4172, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
            (void)(gem_buf_push_fn(NULL, _t4173, 2));
#line 1722 "compiler/main.gem"
    GemVal _t4174[] = {gem_v_mp};
    GemVal _t4175[] = {gem_v_i};
    GemVal _t4176[] = {gem_v_i};
    GemVal _t4177[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t4174, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t4175, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t4176, 1)), gem_string("] : GEM_NIL;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4177, 2));
        } else {
#line 1724 "compiler/main.gem"
    GemVal _t4178[] = {gem_v_mp};
    GemVal _t4179[] = {gem_v_i};
    GemVal _t4180[] = {gem_v_i};
    GemVal _t4181[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t4178, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t4179, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t4180, 1)), gem_string("] : GEM_NIL;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4181, 2));
        }
    }

#line 1729 "compiler/main.gem"
    GemVal _t4182 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4182, gem_string("rest_param")), GEM_NIL))) {
#line 1730 "compiler/main.gem"
    GemVal _t4183[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t4183, 1);
#line 1731 "compiler/main.gem"
        GemVal gem_v_end_count = gem_int(0);
#line 1732 "compiler/main.gem"
    GemVal _t4184 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4184, gem_string("block_param")), GEM_NIL))) {
#line 1733 "compiler/main.gem"
            gem_v_end_count = gem_int(1);
        }
#line 1735 "compiler/main.gem"
    GemVal _t4185 = gem_v_node;
    GemVal _t4186[] = {gem_table_get(_t4185, gem_string("rest_param"))};
    GemVal _t4187 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t4187.fn(_t4187.env, _t4186, 1);
#line 1736 "compiler/main.gem"
    GemVal _t4188 = gem_v_node;
    GemVal _t4189[] = {gem_v_cap_set, gem_table_get(_t4188, gem_string("rest_param"))};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4189, 2))) {
#line 1737 "compiler/main.gem"
    GemVal _t4190[] = {gem_v_mp_rest};
    GemVal _t4191[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4190, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
            (void)(gem_buf_push_fn(NULL, _t4191, 2));
#line 1738 "compiler/main.gem"
    GemVal _t4192[] = {gem_v_mp_rest};
    GemVal _t4193[] = {gem_v_b, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t4192, 1)), gem_string(" = gem_table_new();\n"))};
            (void)(gem_buf_push_fn(NULL, _t4193, 2));
#line 1739 "compiler/main.gem"
    GemVal _t4194[] = {gem_v_N};
    GemVal _t4195[] = {gem_v_end_count};
    GemVal _t4196[] = {gem_v_mp_rest};
    GemVal _t4197[] = {gem_v_N};
    GemVal _t4198[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t4194, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t4195, 1)), gem_string("; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t4196, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t4197, 1)), gem_string("), args[_ri]);\n"))};
            (void)(gem_buf_push_fn(NULL, _t4198, 2));
        } else {
#line 1741 "compiler/main.gem"
    GemVal _t4199[] = {gem_v_mp_rest};
    GemVal _t4200[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t4199, 1)), gem_string(" = gem_table_new();\n"))};
            (void)(gem_buf_push_fn(NULL, _t4200, 2));
#line 1742 "compiler/main.gem"
    GemVal _t4201[] = {gem_v_N};
    GemVal _t4202[] = {gem_v_end_count};
    GemVal _t4203[] = {gem_v_mp_rest};
    GemVal _t4204[] = {gem_v_N};
    GemVal _t4205[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t4201, 1)), gem_string("; _ri < argc - ")), gem_to_string_fn(NULL, _t4202, 1)), gem_string("; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t4203, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t4204, 1)), gem_string("), args[_ri]);\n"))};
            (void)(gem_buf_push_fn(NULL, _t4205, 2));
        }
#line 1744 "compiler/main.gem"
    GemVal _t4206 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4206, gem_string("block_param")), GEM_NIL))) {
#line 1745 "compiler/main.gem"
    GemVal _t4207 = gem_v_node;
    GemVal _t4208[] = {gem_table_get(_t4207, gem_string("block_param"))};
    GemVal _t4209 = (*gem_v_mangle);
            GemVal gem_v_mp_block = _t4209.fn(_t4209.env, _t4208, 1);
#line 1746 "compiler/main.gem"
    GemVal _t4210 = gem_v_node;
    GemVal _t4211[] = {gem_v_cap_set, gem_table_get(_t4210, gem_string("block_param"))};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4211, 2))) {
#line 1747 "compiler/main.gem"
    GemVal _t4212[] = {gem_v_mp_block};
    GemVal _t4213[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4212, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
                (void)(gem_buf_push_fn(NULL, _t4213, 2));
#line 1748 "compiler/main.gem"
    GemVal _t4214[] = {gem_v_mp_block};
    GemVal _t4215[] = {gem_v_b, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t4214, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t4215, 2));
            } else {
#line 1750 "compiler/main.gem"
    GemVal _t4216[] = {gem_v_mp_block};
    GemVal _t4217[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t4216, 1)), gem_string(" = (argc > 0) ? args[argc - 1] : GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t4217, 2));
            }
        }
    }
#line 1754 "compiler/main.gem"
    GemVal _t4218[] = {gem_v_b};
    GemVal _t4219 = gem_buf_str_fn(NULL, _t4218, 1);
    gem_pop_frame();
    return _t4219;
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
#line 1760 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1761 "compiler/main.gem"
    GemVal _t4221[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4221, 1), gem_int(0)))) {
#line 1762 "compiler/main.gem"
        GemVal gem_v__for_i_80 = gem_int(0);
#line 1762 "compiler/main.gem"
    GemVal _t4222[] = {gem_v_body};
        GemVal gem_v__for_limit_80 = gem_sub(gem_len_fn(NULL, _t4222, 1), gem_int(1));
#line 1762 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1762 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_80;
#line 1762 "compiler/main.gem"
            gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1763 "compiler/main.gem"
    GemVal _t4223[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t4224 = (*gem_v_compile_stmt);
    GemVal _t4225[] = {gem_v_b, _t4224.fn(_t4224.env, _t4223, 2)};
            (void)(gem_buf_push_fn(NULL, _t4225, 2));
#line 1764 "compiler/main.gem"
    GemVal _t4226[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4226, 2));
        }

#line 1766 "compiler/main.gem"
    GemVal _t4227[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t4228 = (*gem_v_compile_stmt_return);
    GemVal _t4229[] = {gem_v_b, _t4228.fn(_t4228.env, _t4227, 2)};
        (void)(gem_buf_push_fn(NULL, _t4229, 2));
#line 1767 "compiler/main.gem"
    GemVal _t4230[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t4230, 2));
    } else {
#line 1769 "compiler/main.gem"
    GemVal _t4231[] = {gem_v_b, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4231, 2));
    }
#line 1771 "compiler/main.gem"
    GemVal _t4232[] = {gem_v_b};
    GemVal _t4233 = gem_buf_str_fn(NULL, _t4232, 1);
    gem_pop_frame();
    return _t4233;
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
#line 1777 "compiler/main.gem"
    GemVal gem_v__d81 = gem_v_node;
#line 1777 "compiler/main.gem"
    GemVal _t4235 = gem_v__d81;
    GemVal gem_v_params = gem_table_get(_t4235, gem_string("params"));
#line 1777 "compiler/main.gem"
    GemVal _t4236 = gem_v__d81;
    GemVal gem_v_body = gem_table_get(_t4236, gem_string("body"));
#line 1777 "compiler/main.gem"
    GemVal _t4237 = gem_v__d81;
    GemVal gem_v_name = gem_table_get(_t4237, gem_string("name"));

#line 1780 "compiler/main.gem"
    GemVal _t4238[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4238, 1);
#line 1781 "compiler/main.gem"
    GemVal _t4239 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4239, gem_string("rest_param")), GEM_NIL))) {
#line 1782 "compiler/main.gem"
    GemVal _t4240 = gem_v_node;
    GemVal _t4241[] = {gem_v_scope_vars, gem_table_get(_t4240, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4241, 2));
    }
#line 1784 "compiler/main.gem"
    GemVal _t4242 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4242, gem_string("block_param")), GEM_NIL))) {
#line 1785 "compiler/main.gem"
    GemVal _t4243 = gem_v_node;
    GemVal _t4244[] = {gem_v_scope_vars, gem_table_get(_t4243, gem_string("block_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4244, 2));
    }
#line 1787 "compiler/main.gem"
    GemVal _t4245[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4245, 2));
#line 1789 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1790 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1791 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1792 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1794 "compiler/main.gem"
    GemVal _t4246[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t4247 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t4247.fn(_t4247.env, _t4246, 2);
#line 1796 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1797 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1799 "compiler/main.gem"
    GemVal gem_v_use_tco = gem_bool(0);
#line 1800 "compiler/main.gem"
    GemVal _t4248 = gem_v_node;
    GemVal _t4250;
    if (!gem_truthy(gem_eq(gem_table_get(_t4248, gem_string("rest_param")), GEM_NIL))) {
        _t4250 = gem_eq(gem_table_get(_t4248, gem_string("rest_param")), GEM_NIL);
    } else {
        GemVal _t4249 = gem_v_node;
        _t4250 = gem_eq(gem_table_get(_t4249, gem_string("block_param")), GEM_NIL);
    }
    GemVal _t4252;
    if (!gem_truthy(_t4250)) {
        _t4252 = _t4250;
    } else {
        GemVal _t4251[] = {gem_v_scope_vars, gem_v_name};
        _t4252 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t4251, 2));
    }
    if (gem_truthy(_t4252)) {
#line 1801 "compiler/main.gem"
    GemVal _t4253[] = {gem_v_name, gem_v_body};
    GemVal _t4254 = (*gem_v_has_self_tail_call);
        gem_v_use_tco = _t4254.fn(_t4254.env, _t4253, 2);
    }
#line 1803 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1804 "compiler/main.gem"
        *gem_v_tco_fn_name = gem_v_name;
#line 1805 "compiler/main.gem"
        *gem_v_tco_params = gem_v_params;
#line 1806 "compiler/main.gem"
        *gem_v_tco_captured = gem_v_captured;
    }
#line 1809 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1810 "compiler/main.gem"
    GemVal _t4255[] = {gem_v_name};
    GemVal _t4256[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4255, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4256, 2));
#line 1812 "compiler/main.gem"
    GemVal _t4257 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4257, gem_string("line")), GEM_NIL))) {
#line 1813 "compiler/main.gem"
    GemVal _t4258 = gem_v_node;
    GemVal _t4259[] = {gem_table_get(_t4258, gem_string("line"))};
    GemVal _t4260[] = {(*gem_v_source_name)};
    GemVal _t4261[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4260, 1)};
    GemVal _t4262[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t4259, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t4261, 1)), gem_string("\"\n"))};
        (void)(gem_buf_push_fn(NULL, _t4262, 2));
    }
#line 1816 "compiler/main.gem"
    GemVal _t4263[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t4264 = (*gem_v_emit_param_bindings);
    GemVal _t4265[] = {gem_v_b, _t4264.fn(_t4264.env, _t4263, 3)};
    (void)(gem_buf_push_fn(NULL, _t4265, 2));
#line 1818 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1819 "compiler/main.gem"
    GemVal _t4266 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4266, gem_string("line")), GEM_NIL))) {
#line 1820 "compiler/main.gem"
    GemVal _t4267 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t4267, gem_string("line"));
    }
#line 1822 "compiler/main.gem"
    GemVal _t4268[] = {gem_v_name};
    GemVal _t4269[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4268, 1)};
    GemVal _t4270[] = {(*gem_v_source_name)};
    GemVal _t4271[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4270, 1)};
    GemVal _t4272[] = {gem_v_fn_line};
    GemVal _t4273[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t4269, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t4271, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t4272, 1)), gem_string(");\n"))};
    (void)(gem_buf_push_fn(NULL, _t4273, 2));
#line 1824 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1825 "compiler/main.gem"
    GemVal _t4274[] = {gem_v_b, gem_string("    while(1) {\n")};
        (void)(gem_buf_push_fn(NULL, _t4274, 2));
#line 1826 "compiler/main.gem"
    GemVal _t4275[] = {gem_v_b, gem_string("        gem_yield_check();\n")};
        (void)(gem_buf_push_fn(NULL, _t4275, 2));
#line 1827 "compiler/main.gem"
    GemVal _t4276[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4276, 1), gem_int(0)))) {
#line 1828 "compiler/main.gem"
            GemVal gem_v__for_i_82 = gem_int(0);
#line 1828 "compiler/main.gem"
    GemVal _t4277[] = {gem_v_body};
            GemVal gem_v__for_limit_82 = gem_sub(gem_len_fn(NULL, _t4277, 1), gem_int(1));
#line 1828 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_v__for_limit_82))) break;
#line 1828 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_82;
#line 1828 "compiler/main.gem"
                gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1829 "compiler/main.gem"
    GemVal _t4278[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(2)};
    GemVal _t4279 = (*gem_v_compile_stmt);
    GemVal _t4280[] = {gem_v_b, _t4279.fn(_t4279.env, _t4278, 2)};
                (void)(gem_buf_push_fn(NULL, _t4280, 2));
#line 1830 "compiler/main.gem"
    GemVal _t4281[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t4281, 2));
            }

#line 1832 "compiler/main.gem"
    GemVal _t4282[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(2)};
    GemVal _t4283 = (*gem_v_compile_stmt_return);
    GemVal _t4284[] = {gem_v_b, _t4283.fn(_t4283.env, _t4282, 2)};
            (void)(gem_buf_push_fn(NULL, _t4284, 2));
#line 1833 "compiler/main.gem"
    GemVal _t4285[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4285, 2));
        } else {
#line 1835 "compiler/main.gem"
    GemVal _t4286[] = {gem_v_b, gem_string("        gem_pop_frame();\n        return GEM_NIL;\n")};
            (void)(gem_buf_push_fn(NULL, _t4286, 2));
        }
#line 1837 "compiler/main.gem"
    GemVal _t4287[] = {gem_v_b, gem_string("    }\n")};
        (void)(gem_buf_push_fn(NULL, _t4287, 2));
    } else {
#line 1839 "compiler/main.gem"
    GemVal _t4288[] = {gem_v_body};
    GemVal _t4289 = (*gem_v_emit_fn_body);
    GemVal _t4290[] = {gem_v_b, _t4289.fn(_t4289.env, _t4288, 1)};
        (void)(gem_buf_push_fn(NULL, _t4290, 2));
    }
#line 1842 "compiler/main.gem"
    GemVal _t4291[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4291, 2));
#line 1844 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1845 "compiler/main.gem"
        *gem_v_tco_fn_name = GEM_NIL;
#line 1846 "compiler/main.gem"
        *gem_v_tco_params = GEM_NIL;
#line 1847 "compiler/main.gem"
        *gem_v_tco_captured = GEM_NIL;
    }
#line 1850 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1851 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1852 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1853 "compiler/main.gem"
    GemVal _t4292[] = {gem_v_b};
    GemVal _t4293 = gem_buf_str_fn(NULL, _t4292, 1);
    gem_pop_frame();
    return _t4293;
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
#line 1859 "compiler/main.gem"
    GemVal gem_v__d83 = gem_v_node;
#line 1859 "compiler/main.gem"
    GemVal _t4295 = gem_v__d83;
    GemVal gem_v_params = gem_table_get(_t4295, gem_string("params"));
#line 1859 "compiler/main.gem"
    GemVal _t4296 = gem_v__d83;
    GemVal gem_v_body = gem_table_get(_t4296, gem_string("body"));

#line 1860 "compiler/main.gem"
    GemVal _t4297 = (*gem_v_anon_name);
    GemVal gem_v_name = _t4297.fn(_t4297.env, NULL, 0);
#line 1863 "compiler/main.gem"
    GemVal _t4298[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4298, 1);
#line 1864 "compiler/main.gem"
    GemVal _t4299 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4299, gem_string("rest_param")), GEM_NIL))) {
#line 1865 "compiler/main.gem"
    GemVal _t4300 = gem_v_node;
    GemVal _t4301[] = {gem_v_inner_scope, gem_table_get(_t4300, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4301, 2));
    }
#line 1867 "compiler/main.gem"
    GemVal _t4302 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4302, gem_string("block_param")), GEM_NIL))) {
#line 1868 "compiler/main.gem"
    GemVal _t4303 = gem_v_node;
    GemVal _t4304[] = {gem_v_inner_scope, gem_table_get(_t4303, gem_string("block_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4304, 2));
    }
#line 1870 "compiler/main.gem"
    GemVal _t4305[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4305, 2));
#line 1872 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1873 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1874 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1875 "compiler/main.gem"
    GemVal _t4306[] = {gem_v_captures};
    GemVal _t4307[] = {gem_v_inner_scope, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4306, 1)};
    *gem_v_local_names = gem_fn__mod_codegen_set_union(NULL, _t4307, 2);
#line 1877 "compiler/main.gem"
    GemVal _t4308[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t4309 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t4309.fn(_t4309.env, _t4308, 2);
#line 1879 "compiler/main.gem"
    GemVal _t4310[] = {gem_v_captures};
    GemVal _t4311[] = {gem_v_inner_captured, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4310, 1)};
    *gem_v_boxed_vars = gem_fn__mod_codegen_set_union(NULL, _t4311, 2);
#line 1880 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1883 "compiler/main.gem"
    GemVal _t4312[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t4312, 1));
#line 1884 "compiler/main.gem"
    GemVal gem_v_sb = gem_buf_new_fn(NULL, NULL, 0);
#line 1885 "compiler/main.gem"
    GemVal _t4313[] = {gem_v_struct_name};
    GemVal _t4314[] = {gem_v_sb, gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t4313, 1)), gem_string(" {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4314, 2));
#line 1886 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_captures;
#line 1886 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1886 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4315[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t4315, 1)))) break;
#line 1886 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1886 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1887 "compiler/main.gem"
    GemVal _t4316[] = {gem_v_cap};
    GemVal _t4317 = (*gem_v_mangle);
    GemVal _t4318[] = {_t4317.fn(_t4317.env, _t4316, 1)};
    GemVal _t4319[] = {gem_v_sb, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4318, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4319, 2));
    }

#line 1889 "compiler/main.gem"
    GemVal _t4320[] = {gem_v_sb, gem_string("};\n")};
    (void)(gem_buf_push_fn(NULL, _t4320, 2));
#line 1890 "compiler/main.gem"
    GemVal _t4321[] = {gem_v_sb};
    GemVal gem_v_struct_def = gem_buf_str_fn(NULL, _t4321, 1);
#line 1893 "compiler/main.gem"
    GemVal gem_v_fb = gem_buf_new_fn(NULL, NULL, 0);
#line 1894 "compiler/main.gem"
    GemVal _t4322[] = {gem_v_name};
    GemVal _t4323[] = {gem_v_fb, gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t4322, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4323, 2));
#line 1895 "compiler/main.gem"
    GemVal _t4324[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4324, 1), gem_int(0)))) {
#line 1896 "compiler/main.gem"
    GemVal _t4325[] = {gem_v_struct_name};
    GemVal _t4326[] = {gem_v_struct_name};
    GemVal _t4327[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t4325, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t4326, 1)), gem_string(" *)_env;\n"))};
        (void)(gem_buf_push_fn(NULL, _t4327, 2));
#line 1897 "compiler/main.gem"
        GemVal gem_v__for_items_85 = gem_v_captures;
#line 1897 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1897 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4328[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t4328, 1)))) break;
#line 1897 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1897 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1898 "compiler/main.gem"
    GemVal _t4329[] = {gem_v_cap};
    GemVal _t4330 = (*gem_v_mangle);
            GemVal gem_v_mc = _t4330.fn(_t4330.env, _t4329, 1);
#line 1899 "compiler/main.gem"
    GemVal _t4331[] = {gem_v_mc};
    GemVal _t4332[] = {gem_v_mc};
    GemVal _t4333[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4331, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t4332, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4333, 2));
        }

    }
#line 1903 "compiler/main.gem"
    GemVal _t4334[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t4335 = (*gem_v_emit_param_bindings);
    GemVal _t4336[] = {gem_v_fb, _t4335.fn(_t4335.env, _t4334, 3)};
    (void)(gem_buf_push_fn(NULL, _t4336, 2));
#line 1904 "compiler/main.gem"
    GemVal _t4337[] = {gem_v_name};
    GemVal _t4338[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4337, 1)};
    GemVal _t4339[] = {(*gem_v_source_name)};
    GemVal _t4340[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4339, 1)};
    GemVal _t4341[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t4338, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t4340, 1)), gem_string("\", 0);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4341, 2));
#line 1905 "compiler/main.gem"
    GemVal _t4342[] = {gem_v_body};
    GemVal _t4343 = (*gem_v_emit_fn_body);
    GemVal _t4344[] = {gem_v_fb, _t4343.fn(_t4343.env, _t4342, 1)};
    (void)(gem_buf_push_fn(NULL, _t4344, 2));
#line 1906 "compiler/main.gem"
    GemVal _t4345[] = {gem_v_fb, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4345, 2));
#line 1908 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1909 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1910 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1913 "compiler/main.gem"
    GemVal _t4346[] = {gem_v_fb};
    GemVal _t4347[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_buf_str_fn(NULL, _t4346, 1))};
    (void)(gem_push_fn(NULL, _t4347, 2));
    GemVal _t4348 = gem_table_new();
    gem_table_set(_t4348, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t4348, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t4348, gem_string("captures"), gem_v_captures);
    GemVal _t4349 = _t4348;
    gem_pop_frame();
    return _t4349;
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
#line 1922 "compiler/main.gem"
    GemVal _t4351 = gem_table_new();
    GemVal gem_v_fn_defs = _t4351;
#line 1923 "compiler/main.gem"
    GemVal _t4352 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t4352;
#line 1924 "compiler/main.gem"
    GemVal _t4353 = gem_table_new();
    GemVal gem_v_extern_includes = _t4353;
#line 1925 "compiler/main.gem"
    GemVal _t4354 = gem_table_new();
    GemVal gem_v_top_stmts = _t4354;
#line 1927 "compiler/main.gem"
    GemVal _t4355 = gem_v_ast;
    GemVal gem_v__for_items_86 = gem_table_get(_t4355, gem_string("stmts"));
#line 1927 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1927 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4356[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t4356, 1)))) break;
#line 1927 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1927 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1928 "compiler/main.gem"
    GemVal _t4357 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t4357, gem_string("tag")), gem_string("fn_def")))) {
#line 1929 "compiler/main.gem"
    GemVal _t4358[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t4358, 2));
        } else {
#line 1930 "compiler/main.gem"
    GemVal _t4359 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t4359, gem_string("tag")), gem_string("extern_fn")))) {
#line 1931 "compiler/main.gem"
    GemVal _t4360[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t4360, 2));
            } else {
#line 1932 "compiler/main.gem"
    GemVal _t4361 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t4361, gem_string("tag")), gem_string("extern_include")))) {
#line 1933 "compiler/main.gem"
    GemVal _t4362[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t4362, 2));
                } else {
#line 1934 "compiler/main.gem"
    GemVal _t4363 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t4363, gem_string("tag")), gem_string("export")))) {
                    } else {
#line 1937 "compiler/main.gem"
    GemVal _t4364[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t4364, 2));
                    }
                }
            }
        }
    }

#line 1942 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_extern_fns_list;
#line 1942 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1942 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4365[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t4365, 1)))) break;
#line 1942 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1942 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1943 "compiler/main.gem"
    GemVal _t4366 = gem_v_ef;
    GemVal _t4367[] = {(*gem_v_defined_fns), gem_table_get(_t4366, gem_string("name"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4367, 2));
    }

#line 1947 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_fn_defs;
#line 1947 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1947 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4368[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t4368, 1)))) break;
#line 1947 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1947 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1948 "compiler/main.gem"
    GemVal _t4369 = gem_v_fd;
    GemVal _t4370[] = {(*gem_v_defined_fns), gem_table_get(_t4369, gem_string("name"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4370, 2));
#line 1949 "compiler/main.gem"
    GemVal _t4371 = gem_v_fd;
    GemVal _t4372[] = {gem_table_get(_t4371, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4372, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1950 "compiler/main.gem"
    GemVal _t4373[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t4373, 2));
    }

#line 1954 "compiler/main.gem"
    GemVal _t4374[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4374, 2));
#line 1955 "compiler/main.gem"
    GemVal _t4375 = gem_table_new();
    GemVal _t4376[] = {gem_v_top_stmts, _t4375};
    GemVal _t4377 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t4377.fn(_t4377.env, _t4376, 2);
#line 1958 "compiler/main.gem"
    GemVal gem_v__for_items_89 = gem_v_extern_fns_list;
#line 1958 "compiler/main.gem"
    GemVal gem_v__for_i_89 = gem_int(0);
#line 1958 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4378[] = {gem_v__for_items_89};
        if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t4378, 1)))) break;
#line 1958 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1958 "compiler/main.gem"
        gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1959 "compiler/main.gem"
    GemVal _t4379[] = {gem_v_ef};
    GemVal _t4380 = (*gem_v_compile_extern_fn);
    GemVal _t4381[] = {(*gem_v_functions), _t4380.fn(_t4380.env, _t4379, 1)};
        (void)(gem_push_fn(NULL, _t4381, 2));
    }

#line 1963 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_fn_defs;
#line 1963 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1963 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4382[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t4382, 1)))) break;
#line 1963 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1963 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1964 "compiler/main.gem"
    GemVal _t4383[] = {gem_v_fd};
    GemVal _t4384 = (*gem_v_compile_fn);
    GemVal _t4385[] = {(*gem_v_functions), _t4384.fn(_t4384.env, _t4383, 1)};
        (void)(gem_push_fn(NULL, _t4385, 2));
    }

#line 1968 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1969 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1970 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1971 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1972 "compiler/main.gem"
    GemVal _t4386 = gem_table_new();
    *gem_v_local_names = _t4386;
#line 1973 "compiler/main.gem"
    GemVal gem_v__for_tbl_91 = (*gem_v_top_level_vars);
#line 1973 "compiler/main.gem"
    GemVal _t4387[] = {gem_v__for_tbl_91};
    GemVal gem_v__for_keys_91 = gem_keys_fn(NULL, _t4387, 1);
#line 1973 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1973 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4388[] = {gem_v__for_keys_91};
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_len_fn(NULL, _t4388, 1)))) break;
#line 1973 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_91, gem_v__for_i_91);
#line 1973 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_91, gem_table_get(gem_v__for_keys_91, gem_v__for_i_91));
#line 1973 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1974 "compiler/main.gem"
    GemVal _t4389[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4389, 2));
    }

#line 1976 "compiler/main.gem"
    GemVal _t4390[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t4391 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t4391.fn(_t4391.env, _t4390, 2);
#line 1977 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1978 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1979 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1981 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1982 "compiler/main.gem"
    GemVal gem_v__for_items_92 = gem_v_fn_defs;
#line 1982 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1982 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4392[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t4392, 1)))) break;
#line 1982 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1982 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1983 "compiler/main.gem"
    GemVal _t4393 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t4393, gem_string("name")), gem_string("main")))) {
#line 1984 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1988 "compiler/main.gem"
    GemVal gem_v_mb = gem_buf_new_fn(NULL, NULL, 0);
#line 1989 "compiler/main.gem"
    GemVal _t4394[] = {gem_v_mb, gem_string("int main(int argc, char **argv) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4394, 2));
#line 1990 "compiler/main.gem"
    GemVal _t4395[] = {gem_v_mb, gem_string("    GC_INIT();\n")};
    (void)(gem_buf_push_fn(NULL, _t4395, 2));
#line 1991 "compiler/main.gem"
    GemVal _t4396[] = {gem_v_mb, gem_string("    gem_init(argc, argv);\n")};
    (void)(gem_buf_push_fn(NULL, _t4396, 2));
#line 1992 "compiler/main.gem"
    GemVal _t4397[] = {gem_v_mb, gem_string("    gem_scheduler_init();\n")};
    (void)(gem_buf_push_fn(NULL, _t4397, 2));
#line 1993 "compiler/main.gem"
    GemVal _t4398[] = {(*gem_v_source_name)};
    GemVal _t4399[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4398, 1)};
    GemVal _t4400[] = {gem_v_mb, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t4399, 1)), gem_string("\", 0);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4400, 2));
#line 1994 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1995 "compiler/main.gem"
    GemVal _t4401[] = {gem_v_mb, gem_v_main_body};
        (void)(gem_buf_push_fn(NULL, _t4401, 2));
    }
#line 1997 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1998 "compiler/main.gem"
    GemVal _t4402[] = {gem_v_mb, gem_string("    GemVal _margs[1] = {GEM_NIL};\n")};
        (void)(gem_buf_push_fn(NULL, _t4402, 2));
#line 1999 "compiler/main.gem"
    GemVal _t4403[] = {gem_v_mb, gem_string("    gem_fn_main(NULL, _margs, 0);\n")};
        (void)(gem_buf_push_fn(NULL, _t4403, 2));
    }
#line 2001 "compiler/main.gem"
    GemVal _t4404[] = {gem_v_mb, gem_string("    gem_run_scheduler();\n")};
    (void)(gem_buf_push_fn(NULL, _t4404, 2));
#line 2002 "compiler/main.gem"
    GemVal _t4405[] = {gem_v_mb, gem_string("    gem_pop_frame();\n    return 0;\n}\n")};
    (void)(gem_buf_push_fn(NULL, _t4405, 2));
#line 2003 "compiler/main.gem"
    GemVal _t4406[] = {gem_v_mb};
    GemVal gem_v_main_c = gem_buf_str_fn(NULL, _t4406, 1);
#line 2006 "compiler/main.gem"
    GemVal gem_v_ob = gem_buf_new_fn(NULL, NULL, 0);
#line 2007 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_includes;
#line 2007 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 2007 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4407[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t4407, 1)))) break;
#line 2007 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 2007 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 2008 "compiler/main.gem"
    GemVal _t4408 = gem_v_ei;
    GemVal _t4409[] = {gem_table_get(_t4408, gem_string("path"))};
    GemVal _t4410[] = {gem_v_ob, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t4409, 1)), gem_string("\"\n"))};
        (void)(gem_buf_push_fn(NULL, _t4410, 2));
    }

#line 2010 "compiler/main.gem"
    GemVal _t4411[] = {gem_v_ob, gem_string("#include \"gem.h\"\n")};
    (void)(gem_buf_push_fn(NULL, _t4411, 2));
#line 2014 "compiler/main.gem"
    GemVal _t4412[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t4412, 1), gem_int(0)))) {
#line 2015 "compiler/main.gem"
        GemVal gem_v__for_i_94 = gem_int(0);
#line 2015 "compiler/main.gem"
    GemVal _t4413[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_94 = gem_len_fn(NULL, _t4413, 1);
#line 2015 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_v__for_limit_94))) break;
#line 2015 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_94;
#line 2015 "compiler/main.gem"
            gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 2016 "compiler/main.gem"
    GemVal _t4414[] = {gem_v_ob, gem_table_get((*gem_v_extern_c_decls), gem_v_i)};
            (void)(gem_buf_push_fn(NULL, _t4414, 2));
#line 2017 "compiler/main.gem"
    GemVal _t4415[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4415, 2));
        }

    }
#line 2022 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 2022 "compiler/main.gem"
    GemVal _t4416[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_95 = gem_len_fn(NULL, _t4416, 1);
#line 2022 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_v__for_limit_95))) break;
#line 2022 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_95;
#line 2022 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 2023 "compiler/main.gem"
    GemVal _t4417[] = {gem_v_ob, gem_table_get((*gem_v_forward_decls), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4417, 2));
#line 2024 "compiler/main.gem"
    GemVal _t4418[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4418, 1), gem_int(1))))) {
#line 2025 "compiler/main.gem"
    GemVal _t4419[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4419, 2));
        }
    }

#line 2028 "compiler/main.gem"
    GemVal _t4420[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4420, 2));
#line 2031 "compiler/main.gem"
    GemVal _t4421[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t4421, 1);
#line 2032 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 2032 "compiler/main.gem"
    GemVal _t4422[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_96 = gem_len_fn(NULL, _t4422, 1);
#line 2032 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_v__for_limit_96))) break;
#line 2032 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_96;
#line 2032 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 2033 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 2034 "compiler/main.gem"
    GemVal _t4423[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4423, 2))) {
#line 2035 "compiler/main.gem"
    GemVal _t4424[] = {gem_v_vname};
    GemVal _t4425[] = {gem_v_ob, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t4424, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4425, 2));
        } else {
#line 2037 "compiler/main.gem"
    GemVal _t4426[] = {gem_v_vname};
    GemVal _t4427[] = {gem_v_ob, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t4426, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4427, 2));
        }
    }

#line 2040 "compiler/main.gem"
    GemVal _t4428[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4428, 2));
#line 2043 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 2043 "compiler/main.gem"
    GemVal _t4429[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_97 = gem_len_fn(NULL, _t4429, 1);
#line 2043 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_limit_97))) break;
#line 2043 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_97;
#line 2043 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 2044 "compiler/main.gem"
    GemVal _t4430[] = {gem_v_ob, gem_table_get((*gem_v_functions), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4430, 2));
#line 2045 "compiler/main.gem"
    GemVal _t4431[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4431, 1), gem_int(1))))) {
#line 2046 "compiler/main.gem"
    GemVal _t4432[] = {gem_v_ob, gem_string("\n\n")};
            (void)(gem_buf_push_fn(NULL, _t4432, 2));
        }
    }

#line 2049 "compiler/main.gem"
    GemVal _t4433[] = {gem_v_ob, gem_string("\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4433, 2));
#line 2051 "compiler/main.gem"
    GemVal _t4434[] = {gem_v_ob, gem_v_main_c};
    (void)(gem_buf_push_fn(NULL, _t4434, 2));
#line 2052 "compiler/main.gem"
    GemVal _t4435[] = {gem_v_ob};
    GemVal _t4436 = gem_buf_str_fn(NULL, _t4435, 1);
    gem_pop_frame();
    return _t4436;
}

static GemVal gem_fn__mod_codegen_make_codegen(void *_env, GemVal *args, int argc) {
#line 125 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_make_codegen", "compiler/main.gem", 125);
#line 126 "compiler/main.gem"
    GemVal _t1856 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1856;
#line 127 "compiler/main.gem"
    GemVal _t1857 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1857;
#line 128 "compiler/main.gem"
    GemVal _t1858 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1858;
#line 129 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 130 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t1859 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1859;
#line 132 "compiler/main.gem"
    GemVal _t1860 = gem_table_new();
    gem_table_set(_t1860, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1860, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1860, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1860, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1860, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1860, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1860, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1860, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1860, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1860, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1860, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1860, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1860, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1860, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1860, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1860, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1860, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1860, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1860, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1860, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1860, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1860, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1860, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1860, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1860, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1860, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1860, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1860, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1860, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1860, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1860, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1860, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1860, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1860, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1860, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1860, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1860, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1860, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1860, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1860, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1860, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1860, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1860, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1860, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1860, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1860, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1860, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1860, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1860, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1860, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1860, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1860, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1860, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1860, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1860, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1860, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1860, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1860, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1860, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1860, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1860, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1860, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1860, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1860, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1860, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1860, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1860, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1860, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1860, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1860, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1860, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    gem_table_set(_t1860, gem_string("exec"), gem_string("gem_exec_fn"));
    gem_table_set(_t1860, gem_string("tcp_connect"), gem_string("gem_tcp_connect_fn"));
    gem_table_set(_t1860, gem_string("tcp_listen"), gem_string("gem_tcp_listen_fn"));
    gem_table_set(_t1860, gem_string("tcp_accept"), gem_string("gem_tcp_accept_fn"));
    gem_table_set(_t1860, gem_string("tcp_read"), gem_string("gem_tcp_read_fn"));
    gem_table_set(_t1860, gem_string("tcp_write"), gem_string("gem_tcp_write_fn"));
    gem_table_set(_t1860, gem_string("tcp_close"), gem_string("gem_tcp_close_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1860;
#line 212 "compiler/main.gem"
    GemVal _t1861 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1861;
#line 213 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 213 "compiler/main.gem"
    GemVal _t1862[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_keys_10 = gem_keys_fn(NULL, _t1862, 1);
#line 213 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 213 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1863[] = {gem_v__for_keys_10};
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t1863, 1)))) break;
#line 213 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_10, gem_v__for_i_10);
#line 213 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_10, gem_table_get(gem_v__for_keys_10, gem_v__for_i_10));
#line 213 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 214 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 216 "compiler/main.gem"
    GemVal _t1864 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1864;
#line 217 "compiler/main.gem"
    GemVal _t1865 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1865;
#line 218 "compiler/main.gem"
    GemVal _t1866 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1866;
#line 219 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 220 "compiler/main.gem"
    GemVal _t1867 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1867;
#line 221 "compiler/main.gem"
    GemVal *gem_v_tco_fn_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_fn_name = GEM_NIL;
#line 222 "compiler/main.gem"
    GemVal *gem_v_tco_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_params = GEM_NIL;
#line 223 "compiler/main.gem"
    GemVal *gem_v_tco_captured = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_captured = GEM_NIL;
#line 227 "compiler/main.gem"
    struct _closure__anon_24 *_t1870 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1870->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1870);
#line 232 "compiler/main.gem"
    struct _closure__anon_25 *_t1873 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1873->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1873);
#line 237 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_26, NULL);
#line 243 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 244 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 246 "compiler/main.gem"
    struct _closure__anon_27 *_t2186 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t2186->gem_v_builtins = gem_v_builtins;
    _t2186->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2186->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2186->gem_v_defined_fns = gem_v_defined_fns;
    _t2186->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t2186);
#line 440 "compiler/main.gem"
    struct _closure__anon_28 *_t2198 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t2198->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t2198);
#line 455 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 456 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 458 "compiler/main.gem"
    struct _closure__anon_29 *_t2387 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2387->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2387->gem_v_walk_captures = gem_v_walk_captures;
    _t2387->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2387);
#line 570 "compiler/main.gem"
    struct _closure__anon_30 *_t2397 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2397->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2397);
#line 581 "compiler/main.gem"
    struct _closure__anon_31 *_t2402 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2402->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2402);
#line 589 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 590 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 591 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 592 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 593 "compiler/main.gem"
    GemVal *gem_v_compile_stmts_tail = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts_tail = GEM_NIL;
#line 594 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 595 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 596 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 597 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 598 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 599 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 600 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 601 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 602 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 603 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 604 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 605 "compiler/main.gem"
    GemVal *gem_v_compile_blocking_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_blocking_extern_fn = GEM_NIL;
#line 606 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 607 "compiler/main.gem"
    GemVal *gem_v_is_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_self_tail_call = GEM_NIL;
#line 608 "compiler/main.gem"
    GemVal *gem_v_has_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_has_self_tail_call = GEM_NIL;
#line 609 "compiler/main.gem"
    GemVal *gem_v_emit_tco_continue = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_tco_continue = GEM_NIL;
#line 613 "compiler/main.gem"
    struct _closure__anon_32 *_t2487 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2487->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t2487->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_is_self_tail_call = gem_make_fn(_anon_32, _t2487);
#line 662 "compiler/main.gem"
    struct _closure__anon_33 *_t2493 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2493->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_has_self_tail_call = gem_make_fn(_anon_33, _t2493);
#line 669 "compiler/main.gem"
    struct _closure__anon_34 *_t2526 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2526->gem_v_compile_expr = gem_v_compile_expr;
    _t2526->gem_v_mangle = gem_v_mangle;
    _t2526->gem_v_tco_captured = gem_v_tco_captured;
    _t2526->gem_v_tco_params = gem_v_tco_params;
    _t2526->gem_v_tmp = gem_v_tmp;
    *gem_v_emit_tco_continue = gem_make_fn(_anon_34, _t2526);
#line 699 "compiler/main.gem"
    struct _closure__anon_35 *_t2726 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2726->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2726->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2726->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2726->gem_v_compile_array = gem_v_compile_array;
    _t2726->gem_v_compile_binop = gem_v_compile_binop;
    _t2726->gem_v_compile_call = gem_v_compile_call;
    _t2726->gem_v_compile_expr = gem_v_compile_expr;
    _t2726->gem_v_compile_table = gem_v_compile_table;
    _t2726->gem_v_defined_fns = gem_v_defined_fns;
    _t2726->gem_v_local_names = gem_v_local_names;
    _t2726->gem_v_mangle = gem_v_mangle;
    _t2726->gem_v_tmp = gem_v_tmp;
    _t2726->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_35, _t2726);
#line 786 "compiler/main.gem"
    struct _closure__anon_36 *_t2747 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2747->gem_v_compile_expr = gem_v_compile_expr;
    _t2747->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_36, _t2747);
#line 802 "compiler/main.gem"
    struct _closure__anon_37 *_t2765 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2765->gem_v_compile_expr = gem_v_compile_expr;
    _t2765->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_37, _t2765);
#line 817 "compiler/main.gem"
    struct _closure__anon_38 *_t2814 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2814->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2814->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2814->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2814->gem_v_mangle = gem_v_mangle;
    _t2814->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_38, _t2814);
#line 852 "compiler/main.gem"
    struct _closure__anon_39 *_t2833 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2833->gem_v_compile_expr = gem_v_compile_expr;
    _t2833->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_39, _t2833);
#line 873 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_40, NULL);
#line 882 "compiler/main.gem"
    struct _closure__anon_41 *_t2925 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2925->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2925->gem_v_compile_args = gem_v_compile_args;
    _t2925->gem_v_compile_expr = gem_v_compile_expr;
    _t2925->gem_v_defined_fns = gem_v_defined_fns;
    _t2925->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2925->gem_v_local_names = gem_v_local_names;
    _t2925->gem_v_source_name = gem_v_source_name;
    _t2925->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_41, _t2925);
#line 926 "compiler/main.gem"
    struct _closure__anon_42 *_t3096 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t3096->gem_v_compile_expr = gem_v_compile_expr;
    _t3096->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_42, _t3096);
#line 1007 "compiler/main.gem"
    struct _closure__anon_43 *_t3104 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3104->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_43, _t3104);
#line 1016 "compiler/main.gem"
    struct _closure__anon_44 *_t3124 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3124->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3124->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3124->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_stmts_tail = gem_make_fn(_anon_44, _t3124);
#line 1037 "compiler/main.gem"
    struct _closure__anon_45 *_t3378 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3378->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3378->gem_v_compile_expr = gem_v_compile_expr;
    _t3378->gem_v_compile_if = gem_v_compile_if;
    _t3378->gem_v_compile_match = gem_v_compile_match;
    _t3378->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3378->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3378->gem_v_compile_while = gem_v_compile_while;
    _t3378->gem_v_in_top_level = gem_v_in_top_level;
    _t3378->gem_v_mangle = gem_v_mangle;
    _t3378->gem_v_source_name = gem_v_source_name;
    _t3378->gem_v_tmp = gem_v_tmp;
    _t3378->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3378->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_45, _t3378);
#line 1114 "compiler/main.gem"
    struct _closure__anon_46 *_t3641 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3641->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3641->gem_v_compile_expr = gem_v_compile_expr;
    _t3641->gem_v_compile_if = gem_v_compile_if;
    _t3641->gem_v_compile_match = gem_v_compile_match;
    _t3641->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3641->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3641->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3641->gem_v_compile_while = gem_v_compile_while;
    _t3641->gem_v_emit_tco_continue = gem_v_emit_tco_continue;
    _t3641->gem_v_local_names = gem_v_local_names;
    _t3641->gem_v_mangle = gem_v_mangle;
    _t3641->gem_v_source_name = gem_v_source_name;
    _t3641->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3641->gem_v_tmp = gem_v_tmp;
    _t3641->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_46, _t3641);
#line 1193 "compiler/main.gem"
    struct _closure__anon_47 *_t3673 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3673->gem_v_compile_expr = gem_v_compile_expr;
    _t3673->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    *gem_v_compile_if = gem_make_fn(_anon_47, _t3673);
#line 1213 "compiler/main.gem"
    struct _closure__anon_48 *_t3699 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3699->gem_v_compile_expr = gem_v_compile_expr;
    _t3699->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_48, _t3699);
#line 1230 "compiler/main.gem"
    struct _closure__anon_49 *_t3783 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3783->gem_v_compile_expr = gem_v_compile_expr;
    _t3783->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3783->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3783->gem_v_mangle = gem_v_mangle;
    _t3783->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_49, _t3783);
#line 1286 "compiler/main.gem"
    struct _closure__anon_50 *_t3960 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3960->gem_v_compile_expr = gem_v_compile_expr;
    _t3960->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3960->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3960->gem_v_mangle = gem_v_mangle;
    _t3960->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_50, _t3960);
#line 1406 "compiler/main.gem"
    struct _closure__anon_51 *_t3968 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3968->gem_v_extern_c_decls = gem_v_extern_c_decls;
    GemVal *gem_v_emit_extern_c_decl = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_extern_c_decl = gem_make_fn(_anon_51, _t3968);
#line 1452 "compiler/main.gem"
    struct _closure__anon_52 *_t4042 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t4042->gem_v_compile_blocking_extern_fn = gem_v_compile_blocking_extern_fn;
    _t4042->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t4042->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_52, _t4042);
#line 1535 "compiler/main.gem"
    struct _closure__anon_53 *_t4167 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t4167->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t4167->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_blocking_extern_fn = gem_make_fn(_anon_53, _t4167);
#line 1715 "compiler/main.gem"
    struct _closure__anon_54 *_t4220 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t4220->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_54, _t4220);
#line 1759 "compiler/main.gem"
    struct _closure__anon_55 *_t4234 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t4234->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4234->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_55, _t4234);
#line 1776 "compiler/main.gem"
    struct _closure__anon_56 *_t4294 = GC_MALLOC(sizeof(struct _closure__anon_56));
    _t4294->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4294->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4294->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t4294->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4294->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4294->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4294->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t4294->gem_v_in_top_level = gem_v_in_top_level;
    _t4294->gem_v_local_names = gem_v_local_names;
    _t4294->gem_v_source_name = gem_v_source_name;
    _t4294->gem_v_tco_captured = gem_v_tco_captured;
    _t4294->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t4294->gem_v_tco_params = gem_v_tco_params;
    *gem_v_compile_fn = gem_make_fn(_anon_56, _t4294);
#line 1858 "compiler/main.gem"
    struct _closure__anon_57 *_t4350 = GC_MALLOC(sizeof(struct _closure__anon_57));
    _t4350->gem_v_anon_name = gem_v_anon_name;
    _t4350->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4350->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4350->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4350->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4350->gem_v_functions = gem_v_functions;
    _t4350->gem_v_in_top_level = gem_v_in_top_level;
    _t4350->gem_v_local_names = gem_v_local_names;
    _t4350->gem_v_mangle = gem_v_mangle;
    _t4350->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_57, _t4350);
#line 1920 "compiler/main.gem"
    struct _closure__anon_58 *_t4437 = GC_MALLOC(sizeof(struct _closure__anon_58));
    _t4437->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4437->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t4437->gem_v_compile_fn = gem_v_compile_fn;
    _t4437->gem_v_compile_stmts = gem_v_compile_stmts;
    _t4437->gem_v_defined_fns = gem_v_defined_fns;
    _t4437->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t4437->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4437->gem_v_forward_decls = gem_v_forward_decls;
    _t4437->gem_v_functions = gem_v_functions;
    _t4437->gem_v_in_top_level = gem_v_in_top_level;
    _t4437->gem_v_local_names = gem_v_local_names;
    _t4437->gem_v_source_name = gem_v_source_name;
    _t4437->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t4437->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_58, _t4437);
    GemVal _t4438 = gem_table_new();
    gem_table_set(_t4438, gem_string("compile"), gem_v_compile);
    GemVal _t4439 = _t4438;
    gem_pop_frame();
    return _t4439;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 20 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 20);
    GemVal _t4440[] = {gem_v_node};
    GemVal _t4442;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4440, 1), gem_string("table")))) {
        _t4442 = gem_eq(gem_type_fn(NULL, _t4440, 1), gem_string("table"));
    } else {
        GemVal _t4441 = gem_v_node;
        _t4442 = gem_eq(gem_table_get(_t4441, gem_string("tag")), gem_v_tag);
    }
    GemVal _t4443 = _t4442;
    gem_pop_frame();
    return _t4443;
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_program", "compiler/main.gem", 24);
    GemVal _t4444 = gem_table_new();
    gem_table_set(_t4444, gem_string("tag"), gem_string("program"));
    gem_table_set(_t4444, gem_string("stmts"), gem_v_stmts);
    GemVal _t4445 = _t4444;
    gem_pop_frame();
    return _t4445;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 28);
    GemVal _t4446 = gem_table_new();
    gem_table_set(_t4446, gem_string("tag"), gem_string("let"));
    gem_table_set(_t4446, gem_string("name"), gem_v_name);
    gem_table_set(_t4446, gem_string("value"), gem_v_value);
    gem_table_set(_t4446, gem_string("line"), gem_v_line);
    GemVal _t4447 = _t4446;
    gem_pop_frame();
    return _t4447;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 32);
    GemVal _t4448 = gem_table_new();
    gem_table_set(_t4448, gem_string("tag"), gem_string("var"));
    gem_table_set(_t4448, gem_string("name"), gem_v_name);
    GemVal _t4449 = _t4448;
    gem_pop_frame();
    return _t4449;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 36);
    GemVal _t4450 = gem_table_new();
    gem_table_set(_t4450, gem_string("tag"), gem_string("table"));
    gem_table_set(_t4450, gem_string("entries"), gem_v_entries);
    GemVal _t4451 = _t4450;
    gem_pop_frame();
    return _t4451;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 40);
    GemVal _t4452 = gem_table_new();
    gem_table_set(_t4452, gem_string("key"), gem_v_key);
    gem_table_set(_t4452, gem_string("value"), gem_v_value);
    GemVal _t4453 = _t4452;
    gem_pop_frame();
    return _t4453;
}

static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("module_name_from_path", "compiler/main.gem", 46);
#line 47 "compiler/main.gem"
    GemVal _t4454[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4454, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 49 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 50 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 51 "compiler/main.gem"
    GemVal _t4455[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4455, 2), gem_int(47)))) {
#line 52 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 54 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 56 "compiler/main.gem"
    GemVal _t4456[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4456, 3);
#line 57 "compiler/main.gem"
    GemVal _t4457[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4457, 1);
#line 58 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 59 "compiler/main.gem"
    GemVal _t4458 = gem_v_string;
    GemVal _t4459[] = {gem_v_name, gem_string(".gem")};
    GemVal _t4460 = gem_table_get(_t4458, gem_string("ends_with"));
        if (gem_truthy(_t4460.fn(_t4460.env, _t4459, 2))) {
#line 60 "compiler/main.gem"
    GemVal _t4461[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4461, 3);
        }
    }
    GemVal _t4462 = gem_v_name;
    gem_pop_frame();
    return _t4462;
}

static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("find_export_node", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal _t4463[] = {gem_v_stmts};
    GemVal gem_v_i = gem_sub(gem_len_fn(NULL, _t4463, 1), gem_int(1));
#line 68 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_i, gem_int(0)))) break;
#line 69 "compiler/main.gem"
    GemVal _t4464[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("export")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4464, 2))) {
#line 70 "compiler/main.gem"
            GemVal _t4465 = gem_table_get(gem_v_stmts, gem_v_i);
            gem_pop_frame();
            return _t4465;
        }
#line 72 "compiler/main.gem"
        gem_v_i = gem_sub(gem_v_i, gem_int(1));
    }
    GemVal _t4466 = GEM_NIL;
    gem_pop_frame();
    return _t4466;
}

static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc) {
#line 77 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("collect_module_top_names", "compiler/main.gem", 77);
#line 78 "compiler/main.gem"
    GemVal _t4467 = gem_table_new();
    GemVal gem_v_names = _t4467;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_items_1 = gem_v_stmts;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 79 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4468[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t4468, 1)))) break;
#line 79 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 79 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 80 "compiler/main.gem"
    GemVal _t4469[] = {gem_v_stmt, gem_string("fn_def")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4469, 2))) {
#line 81 "compiler/main.gem"
    GemVal _t4470 = gem_v_string;
    GemVal _t4471 = gem_v_stmt;
    GemVal _t4472[] = {gem_table_get(_t4471, gem_string("name")), gem_string("_mod_")};
    GemVal _t4473 = gem_table_get(_t4470, gem_string("starts_with"));
            if (gem_truthy(gem_not(_t4473.fn(_t4473.env, _t4472, 2)))) {
#line 82 "compiler/main.gem"
    GemVal _t4474 = gem_v_stmt;
                gem_table_set(gem_v_names, gem_table_get(_t4474, gem_string("name")), gem_bool(1));
            }
        } else {
#line 84 "compiler/main.gem"
    GemVal _t4475[] = {gem_v_stmt, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t4475, 2))) {
#line 85 "compiler/main.gem"
    GemVal _t4476 = gem_v_string;
    GemVal _t4477 = gem_v_stmt;
    GemVal _t4478[] = {gem_table_get(_t4477, gem_string("name")), gem_string("_mod_")};
    GemVal _t4479 = gem_table_get(_t4476, gem_string("starts_with"));
                if (gem_truthy(gem_not(_t4479.fn(_t4479.env, _t4478, 2)))) {
#line 86 "compiler/main.gem"
    GemVal _t4480 = gem_v_stmt;
                    gem_table_set(gem_v_names, gem_table_get(_t4480, gem_string("name")), gem_bool(1));
                }
            }
        }
    }

    GemVal _t4481 = gem_v_names;
    gem_pop_frame();
    return _t4481;
}

static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_top_names = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("build_rename_map", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t4482 = gem_table_new();
    GemVal gem_v_rmap = _t4482;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_tbl_2 = gem_v_top_names;
#line 95 "compiler/main.gem"
    GemVal _t4483[] = {gem_v__for_tbl_2};
    GemVal gem_v__for_keys_2 = gem_keys_fn(NULL, _t4483, 1);
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4484[] = {gem_v__for_keys_2};
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_len_fn(NULL, _t4484, 1)))) break;
#line 95 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__for_keys_2, gem_v__for_i_2);
#line 95 "compiler/main.gem"
        GemVal gem_v__ = gem_table_get(gem_v__for_tbl_2, gem_table_get(gem_v__for_keys_2, gem_v__for_i_2));
#line 95 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_rmap, gem_v_name, gem_add(gem_v_prefix, gem_v_name));
    }

    GemVal _t4485 = gem_v_rmap;
    gem_pop_frame();
    return _t4485;
}

static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("copy_set", "compiler/main.gem", 101);
#line 102 "compiler/main.gem"
    GemVal _t4486 = gem_table_new();
    GemVal gem_v_result = _t4486;
#line 103 "compiler/main.gem"
    GemVal gem_v__for_tbl_3 = gem_v_s;
#line 103 "compiler/main.gem"
    GemVal _t4487[] = {gem_v__for_tbl_3};
    GemVal gem_v__for_keys_3 = gem_keys_fn(NULL, _t4487, 1);
#line 103 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 103 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4488[] = {gem_v__for_keys_3};
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_len_fn(NULL, _t4488, 1)))) break;
#line 103 "compiler/main.gem"
        GemVal gem_v_k = gem_table_get(gem_v__for_keys_3, gem_v__for_i_3);
#line 103 "compiler/main.gem"
        GemVal gem_v_v = gem_table_get(gem_v__for_tbl_3, gem_table_get(gem_v__for_keys_3, gem_v__for_i_3));
#line 103 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 104 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_v_v);
    }

    GemVal _t4489 = gem_v_result;
    gem_pop_frame();
    return _t4489;
}

static GemVal gem_fn_rename_node(void *_env, GemVal *args, int argc) {
#line 109 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rmap = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_shadowed = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("rename_node", "compiler/main.gem", 109);
    while(1) {
        gem_yield_check();
#line 110 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 111 "compiler/main.gem"
            GemVal _t4490 = GEM_NIL;
            gem_pop_frame();
            return _t4490;
        }
#line 113 "compiler/main.gem"
    GemVal _t4491[] = {gem_v_node};
        if (gem_truthy(gem_neq(gem_type_fn(NULL, _t4491, 1), gem_string("table")))) {
#line 114 "compiler/main.gem"
            GemVal _t4492 = GEM_NIL;
            gem_pop_frame();
            return _t4492;
        }
#line 116 "compiler/main.gem"
    GemVal _t4493 = gem_v_node;
        if (gem_truthy(gem_eq(gem_table_get(_t4493, gem_string("tag")), GEM_NIL))) {
#line 117 "compiler/main.gem"
            GemVal _t4494 = GEM_NIL;
            gem_pop_frame();
            return _t4494;
        }
#line 120 "compiler/main.gem"
        GemVal gem_v__match_4 = gem_v_node;
    GemVal _t4495[] = {gem_v__match_4};
    GemVal _t4497;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4495, 1), gem_string("table")))) {
        _t4497 = gem_eq(gem_type_fn(NULL, _t4495, 1), gem_string("table"));
    } else {
        GemVal _t4496[] = {gem_v__match_4, gem_string("tag")};
        _t4497 = gem_has_key_fn(NULL, _t4496, 2);
    }
    GemVal _t4498;
    if (!gem_truthy(_t4497)) {
        _t4498 = _t4497;
    } else {
        _t4498 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("var"));
    }
    GemVal _t4500;
    if (!gem_truthy(_t4498)) {
        _t4500 = _t4498;
    } else {
        GemVal _t4499[] = {gem_v__match_4, gem_string("name")};
        _t4500 = gem_has_key_fn(NULL, _t4499, 2);
    }
        if (gem_truthy(_t4500)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 122 "compiler/main.gem"
    GemVal _t4501[] = {gem_v_rmap, gem_v_name};
    GemVal _t4503;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4501, 2))) {
        _t4503 = gem_has_key_fn(NULL, _t4501, 2);
    } else {
        GemVal _t4502[] = {gem_v_shadowed, gem_v_name};
        _t4503 = gem_not(gem_has_key_fn(NULL, _t4502, 2));
    }
            if (gem_truthy(_t4503)) {
#line 123 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4504[] = {gem_v__match_4};
    GemVal _t4506;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4504, 1), gem_string("table")))) {
        _t4506 = gem_eq(gem_type_fn(NULL, _t4504, 1), gem_string("table"));
    } else {
        GemVal _t4505[] = {gem_v__match_4, gem_string("tag")};
        _t4506 = gem_has_key_fn(NULL, _t4505, 2);
    }
    GemVal _t4507;
    if (!gem_truthy(_t4506)) {
        _t4507 = _t4506;
    } else {
        _t4507 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("fn_def"));
    }
    GemVal _t4509;
    if (!gem_truthy(_t4507)) {
        _t4509 = _t4507;
    } else {
        GemVal _t4508[] = {gem_v__match_4, gem_string("name")};
        _t4509 = gem_has_key_fn(NULL, _t4508, 2);
    }
    GemVal _t4511;
    if (!gem_truthy(_t4509)) {
        _t4511 = _t4509;
    } else {
        GemVal _t4510[] = {gem_v__match_4, gem_string("params")};
        _t4511 = gem_has_key_fn(NULL, _t4510, 2);
    }
    GemVal _t4513;
    if (!gem_truthy(_t4511)) {
        _t4513 = _t4511;
    } else {
        GemVal _t4512[] = {gem_v__match_4, gem_string("body")};
        _t4513 = gem_has_key_fn(NULL, _t4512, 2);
    }
        if (gem_truthy(_t4513)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 126 "compiler/main.gem"
    GemVal _t4514[] = {gem_v_rmap, gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t4514, 2))) {
#line 127 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
            }
#line 129 "compiler/main.gem"
    GemVal _t4515[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4515, 1);
#line 130 "compiler/main.gem"
            GemVal gem_v__for_items_5 = gem_v_params;
#line 130 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4516[] = {gem_v__for_items_5};
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t4516, 1)))) break;
#line 130 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 133 "compiler/main.gem"
    GemVal _t4517 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4517, gem_string("rest_param")), GEM_NIL))) {
#line 134 "compiler/main.gem"
    GemVal _t4518 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4518, gem_string("rest_param")), gem_bool(1));
            }
#line 136 "compiler/main.gem"
    GemVal _t4519 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4519, gem_string("block_param")), GEM_NIL))) {
#line 137 "compiler/main.gem"
    GemVal _t4520 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4520, gem_string("block_param")), gem_bool(1));
            }
#line 139 "compiler/main.gem"
    GemVal _t4521[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4522 = gem_fn_rename_stmts(NULL, _t4521, 3);
            gem_pop_frame();
            return _t4522;
        } else {
    GemVal _t4523[] = {gem_v__match_4};
    GemVal _t4525;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4523, 1), gem_string("table")))) {
        _t4525 = gem_eq(gem_type_fn(NULL, _t4523, 1), gem_string("table"));
    } else {
        GemVal _t4524[] = {gem_v__match_4, gem_string("tag")};
        _t4525 = gem_has_key_fn(NULL, _t4524, 2);
    }
    GemVal _t4526;
    if (!gem_truthy(_t4525)) {
        _t4526 = _t4525;
    } else {
        _t4526 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t4528;
    if (!gem_truthy(_t4526)) {
        _t4528 = _t4526;
    } else {
        GemVal _t4527[] = {gem_v__match_4, gem_string("params")};
        _t4528 = gem_has_key_fn(NULL, _t4527, 2);
    }
    GemVal _t4530;
    if (!gem_truthy(_t4528)) {
        _t4530 = _t4528;
    } else {
        GemVal _t4529[] = {gem_v__match_4, gem_string("body")};
        _t4530 = gem_has_key_fn(NULL, _t4529, 2);
    }
        if (gem_truthy(_t4530)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 141 "compiler/main.gem"
    GemVal _t4531[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4531, 1);
#line 142 "compiler/main.gem"
            GemVal gem_v__for_items_6 = gem_v_params;
#line 142 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 142 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4532[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t4532, 1)))) break;
#line 142 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 142 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 143 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 145 "compiler/main.gem"
    GemVal _t4533 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4533, gem_string("rest_param")), GEM_NIL))) {
#line 146 "compiler/main.gem"
    GemVal _t4534 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4534, gem_string("rest_param")), gem_bool(1));
            }
#line 148 "compiler/main.gem"
    GemVal _t4535 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4535, gem_string("block_param")), GEM_NIL))) {
#line 149 "compiler/main.gem"
    GemVal _t4536 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4536, gem_string("block_param")), gem_bool(1));
            }
#line 151 "compiler/main.gem"
    GemVal _t4537[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4538 = gem_fn_rename_stmts(NULL, _t4537, 3);
            gem_pop_frame();
            return _t4538;
        } else {
    GemVal _t4539[] = {gem_v__match_4};
    GemVal _t4541;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4539, 1), gem_string("table")))) {
        _t4541 = gem_eq(gem_type_fn(NULL, _t4539, 1), gem_string("table"));
    } else {
        GemVal _t4540[] = {gem_v__match_4, gem_string("tag")};
        _t4541 = gem_has_key_fn(NULL, _t4540, 2);
    }
    GemVal _t4542;
    if (!gem_truthy(_t4541)) {
        _t4542 = _t4541;
    } else {
        _t4542 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("let"));
    }
    GemVal _t4544;
    if (!gem_truthy(_t4542)) {
        _t4544 = _t4542;
    } else {
        GemVal _t4543[] = {gem_v__match_4, gem_string("name")};
        _t4544 = gem_has_key_fn(NULL, _t4543, 2);
    }
    GemVal _t4546;
    if (!gem_truthy(_t4544)) {
        _t4546 = _t4544;
    } else {
        GemVal _t4545[] = {gem_v__match_4, gem_string("value")};
        _t4546 = gem_has_key_fn(NULL, _t4545, 2);
    }
        if (gem_truthy(_t4546)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 153 "compiler/main.gem"
    GemVal _t4547[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4547, 3));
#line 154 "compiler/main.gem"
    GemVal _t4548[] = {gem_v_rmap, gem_v_name};
    GemVal _t4550;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4548, 2))) {
        _t4550 = gem_has_key_fn(NULL, _t4548, 2);
    } else {
        GemVal _t4549[] = {gem_v_shadowed, gem_v_name};
        _t4550 = gem_not(gem_has_key_fn(NULL, _t4549, 2));
    }
            if (gem_truthy(_t4550)) {
#line 155 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4551[] = {gem_v__match_4};
    GemVal _t4553;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4551, 1), gem_string("table")))) {
        _t4553 = gem_eq(gem_type_fn(NULL, _t4551, 1), gem_string("table"));
    } else {
        GemVal _t4552[] = {gem_v__match_4, gem_string("tag")};
        _t4553 = gem_has_key_fn(NULL, _t4552, 2);
    }
    GemVal _t4554;
    if (!gem_truthy(_t4553)) {
        _t4554 = _t4553;
    } else {
        _t4554 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t4556;
    if (!gem_truthy(_t4554)) {
        _t4556 = _t4554;
    } else {
        GemVal _t4555[] = {gem_v__match_4, gem_string("name")};
        _t4556 = gem_has_key_fn(NULL, _t4555, 2);
    }
    GemVal _t4558;
    if (!gem_truthy(_t4556)) {
        _t4558 = _t4556;
    } else {
        GemVal _t4557[] = {gem_v__match_4, gem_string("value")};
        _t4558 = gem_has_key_fn(NULL, _t4557, 2);
    }
        if (gem_truthy(_t4558)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 158 "compiler/main.gem"
    GemVal _t4559[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4559, 3));
#line 159 "compiler/main.gem"
    GemVal _t4560[] = {gem_v_rmap, gem_v_name};
    GemVal _t4562;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4560, 2))) {
        _t4562 = gem_has_key_fn(NULL, _t4560, 2);
    } else {
        GemVal _t4561[] = {gem_v_shadowed, gem_v_name};
        _t4562 = gem_not(gem_has_key_fn(NULL, _t4561, 2));
    }
            if (gem_truthy(_t4562)) {
#line 160 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4563[] = {gem_v__match_4};
    GemVal _t4565;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4563, 1), gem_string("table")))) {
        _t4565 = gem_eq(gem_type_fn(NULL, _t4563, 1), gem_string("table"));
    } else {
        GemVal _t4564[] = {gem_v__match_4, gem_string("tag")};
        _t4565 = gem_has_key_fn(NULL, _t4564, 2);
    }
    GemVal _t4566;
    if (!gem_truthy(_t4565)) {
        _t4566 = _t4565;
    } else {
        _t4566 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("call"));
    }
    GemVal _t4568;
    if (!gem_truthy(_t4566)) {
        _t4568 = _t4566;
    } else {
        GemVal _t4567[] = {gem_v__match_4, gem_string("func")};
        _t4568 = gem_has_key_fn(NULL, _t4567, 2);
    }
    GemVal _t4570;
    if (!gem_truthy(_t4568)) {
        _t4570 = _t4568;
    } else {
        GemVal _t4569[] = {gem_v__match_4, gem_string("args")};
        _t4570 = gem_has_key_fn(NULL, _t4569, 2);
    }
        if (gem_truthy(_t4570)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_func = gem_table_get(gem_v__match_4, gem_string("func"));
#line 0 "compiler/main.gem"
            GemVal gem_v_args = gem_table_get(gem_v__match_4, gem_string("args"));
#line 163 "compiler/main.gem"
    GemVal _t4571[] = {gem_v_func, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4571, 3));
            {
#line 164 "compiler/main.gem"
                GemVal gem_v__for_items_7 = gem_v_args;
#line 164 "compiler/main.gem"
                GemVal gem_v__for_i_7 = gem_int(0);
#line 164 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4572[] = {gem_v__for_items_7};
                    if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t4572, 1)))) break;
#line 164 "compiler/main.gem"
                    GemVal gem_v_a = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 164 "compiler/main.gem"
                    gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 165 "compiler/main.gem"
    GemVal _t4573[] = {gem_v_a, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4573, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4574[] = {gem_v__match_4};
    GemVal _t4576;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4574, 1), gem_string("table")))) {
        _t4576 = gem_eq(gem_type_fn(NULL, _t4574, 1), gem_string("table"));
    } else {
        GemVal _t4575[] = {gem_v__match_4, gem_string("tag")};
        _t4576 = gem_has_key_fn(NULL, _t4575, 2);
    }
    GemVal _t4577;
    if (!gem_truthy(_t4576)) {
        _t4577 = _t4576;
    } else {
        _t4577 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("binop"));
    }
    GemVal _t4579;
    if (!gem_truthy(_t4577)) {
        _t4579 = _t4577;
    } else {
        GemVal _t4578[] = {gem_v__match_4, gem_string("left")};
        _t4579 = gem_has_key_fn(NULL, _t4578, 2);
    }
    GemVal _t4581;
    if (!gem_truthy(_t4579)) {
        _t4581 = _t4579;
    } else {
        GemVal _t4580[] = {gem_v__match_4, gem_string("right")};
        _t4581 = gem_has_key_fn(NULL, _t4580, 2);
    }
        if (gem_truthy(_t4581)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_left = gem_table_get(gem_v__match_4, gem_string("left"));
#line 0 "compiler/main.gem"
            GemVal gem_v_right = gem_table_get(gem_v__match_4, gem_string("right"));
#line 168 "compiler/main.gem"
    GemVal _t4582[] = {gem_v_left, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4582, 3));
#line 169 "compiler/main.gem"
            GemVal _t4583 = gem_v_right;
            GemVal _t4584 = gem_v_rmap;
            GemVal _t4585 = gem_v_shadowed;
            gem_v_node = _t4583;
            gem_v_rmap = _t4584;
            gem_v_shadowed = _t4585;
            continue;
        } else {
    GemVal _t4586[] = {gem_v__match_4};
    GemVal _t4588;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4586, 1), gem_string("table")))) {
        _t4588 = gem_eq(gem_type_fn(NULL, _t4586, 1), gem_string("table"));
    } else {
        GemVal _t4587[] = {gem_v__match_4, gem_string("tag")};
        _t4588 = gem_has_key_fn(NULL, _t4587, 2);
    }
    GemVal _t4589;
    if (!gem_truthy(_t4588)) {
        _t4589 = _t4588;
    } else {
        _t4589 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t4591;
    if (!gem_truthy(_t4589)) {
        _t4591 = _t4589;
    } else {
        GemVal _t4590[] = {gem_v__match_4, gem_string("expr")};
        _t4591 = gem_has_key_fn(NULL, _t4590, 2);
    }
        if (gem_truthy(_t4591)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_expr = gem_table_get(gem_v__match_4, gem_string("expr"));
#line 171 "compiler/main.gem"
            GemVal _t4592 = gem_v_expr;
            GemVal _t4593 = gem_v_rmap;
            GemVal _t4594 = gem_v_shadowed;
            gem_v_node = _t4592;
            gem_v_rmap = _t4593;
            gem_v_shadowed = _t4594;
            continue;
        } else {
    GemVal _t4595[] = {gem_v__match_4};
    GemVal _t4597;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4595, 1), gem_string("table")))) {
        _t4597 = gem_eq(gem_type_fn(NULL, _t4595, 1), gem_string("table"));
    } else {
        GemVal _t4596[] = {gem_v__match_4, gem_string("tag")};
        _t4597 = gem_has_key_fn(NULL, _t4596, 2);
    }
    GemVal _t4598;
    if (!gem_truthy(_t4597)) {
        _t4598 = _t4597;
    } else {
        _t4598 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t4600;
    if (!gem_truthy(_t4598)) {
        _t4600 = _t4598;
    } else {
        GemVal _t4599[] = {gem_v__match_4, gem_string("object")};
        _t4600 = gem_has_key_fn(NULL, _t4599, 2);
    }
        if (gem_truthy(_t4600)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 173 "compiler/main.gem"
            GemVal _t4601 = gem_v_object;
            GemVal _t4602 = gem_v_rmap;
            GemVal _t4603 = gem_v_shadowed;
            gem_v_node = _t4601;
            gem_v_rmap = _t4602;
            gem_v_shadowed = _t4603;
            continue;
        } else {
    GemVal _t4604[] = {gem_v__match_4};
    GemVal _t4606;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4604, 1), gem_string("table")))) {
        _t4606 = gem_eq(gem_type_fn(NULL, _t4604, 1), gem_string("table"));
    } else {
        GemVal _t4605[] = {gem_v__match_4, gem_string("tag")};
        _t4606 = gem_has_key_fn(NULL, _t4605, 2);
    }
    GemVal _t4607;
    if (!gem_truthy(_t4606)) {
        _t4607 = _t4606;
    } else {
        _t4607 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index"));
    }
    GemVal _t4609;
    if (!gem_truthy(_t4607)) {
        _t4609 = _t4607;
    } else {
        GemVal _t4608[] = {gem_v__match_4, gem_string("object")};
        _t4609 = gem_has_key_fn(NULL, _t4608, 2);
    }
    GemVal _t4611;
    if (!gem_truthy(_t4609)) {
        _t4611 = _t4609;
    } else {
        GemVal _t4610[] = {gem_v__match_4, gem_string("key")};
        _t4611 = gem_has_key_fn(NULL, _t4610, 2);
    }
        if (gem_truthy(_t4611)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 175 "compiler/main.gem"
    GemVal _t4612[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4612, 3));
#line 176 "compiler/main.gem"
            GemVal _t4613 = gem_v_key;
            GemVal _t4614 = gem_v_rmap;
            GemVal _t4615 = gem_v_shadowed;
            gem_v_node = _t4613;
            gem_v_rmap = _t4614;
            gem_v_shadowed = _t4615;
            continue;
        } else {
    GemVal _t4616[] = {gem_v__match_4};
    GemVal _t4618;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4616, 1), gem_string("table")))) {
        _t4618 = gem_eq(gem_type_fn(NULL, _t4616, 1), gem_string("table"));
    } else {
        GemVal _t4617[] = {gem_v__match_4, gem_string("tag")};
        _t4618 = gem_has_key_fn(NULL, _t4617, 2);
    }
    GemVal _t4619;
    if (!gem_truthy(_t4618)) {
        _t4619 = _t4618;
    } else {
        _t4619 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t4621;
    if (!gem_truthy(_t4619)) {
        _t4621 = _t4619;
    } else {
        GemVal _t4620[] = {gem_v__match_4, gem_string("object")};
        _t4621 = gem_has_key_fn(NULL, _t4620, 2);
    }
    GemVal _t4623;
    if (!gem_truthy(_t4621)) {
        _t4623 = _t4621;
    } else {
        GemVal _t4622[] = {gem_v__match_4, gem_string("value")};
        _t4623 = gem_has_key_fn(NULL, _t4622, 2);
    }
        if (gem_truthy(_t4623)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 178 "compiler/main.gem"
    GemVal _t4624[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4624, 3));
#line 179 "compiler/main.gem"
            GemVal _t4625 = gem_v_value;
            GemVal _t4626 = gem_v_rmap;
            GemVal _t4627 = gem_v_shadowed;
            gem_v_node = _t4625;
            gem_v_rmap = _t4626;
            gem_v_shadowed = _t4627;
            continue;
        } else {
    GemVal _t4628[] = {gem_v__match_4};
    GemVal _t4630;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4628, 1), gem_string("table")))) {
        _t4630 = gem_eq(gem_type_fn(NULL, _t4628, 1), gem_string("table"));
    } else {
        GemVal _t4629[] = {gem_v__match_4, gem_string("tag")};
        _t4630 = gem_has_key_fn(NULL, _t4629, 2);
    }
    GemVal _t4631;
    if (!gem_truthy(_t4630)) {
        _t4631 = _t4630;
    } else {
        _t4631 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t4633;
    if (!gem_truthy(_t4631)) {
        _t4633 = _t4631;
    } else {
        GemVal _t4632[] = {gem_v__match_4, gem_string("object")};
        _t4633 = gem_has_key_fn(NULL, _t4632, 2);
    }
    GemVal _t4635;
    if (!gem_truthy(_t4633)) {
        _t4635 = _t4633;
    } else {
        GemVal _t4634[] = {gem_v__match_4, gem_string("key")};
        _t4635 = gem_has_key_fn(NULL, _t4634, 2);
    }
    GemVal _t4637;
    if (!gem_truthy(_t4635)) {
        _t4637 = _t4635;
    } else {
        GemVal _t4636[] = {gem_v__match_4, gem_string("value")};
        _t4637 = gem_has_key_fn(NULL, _t4636, 2);
    }
        if (gem_truthy(_t4637)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 181 "compiler/main.gem"
    GemVal _t4638[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4638, 3));
#line 182 "compiler/main.gem"
    GemVal _t4639[] = {gem_v_key, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4639, 3));
#line 183 "compiler/main.gem"
            GemVal _t4640 = gem_v_value;
            GemVal _t4641 = gem_v_rmap;
            GemVal _t4642 = gem_v_shadowed;
            gem_v_node = _t4640;
            gem_v_rmap = _t4641;
            gem_v_shadowed = _t4642;
            continue;
        } else {
    GemVal _t4643[] = {gem_v__match_4};
    GemVal _t4645;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4643, 1), gem_string("table")))) {
        _t4645 = gem_eq(gem_type_fn(NULL, _t4643, 1), gem_string("table"));
    } else {
        GemVal _t4644[] = {gem_v__match_4, gem_string("tag")};
        _t4645 = gem_has_key_fn(NULL, _t4644, 2);
    }
    GemVal _t4646;
    if (!gem_truthy(_t4645)) {
        _t4646 = _t4645;
    } else {
        _t4646 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("if"));
    }
    GemVal _t4648;
    if (!gem_truthy(_t4646)) {
        _t4648 = _t4646;
    } else {
        GemVal _t4647[] = {gem_v__match_4, gem_string("cond")};
        _t4648 = gem_has_key_fn(NULL, _t4647, 2);
    }
        if (gem_truthy(_t4648)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 185 "compiler/main.gem"
    GemVal _t4649[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4649, 3));
#line 186 "compiler/main.gem"
    GemVal _t4650 = gem_v_node;
    GemVal _t4651[] = {gem_table_get(_t4650, gem_string("then")), gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_stmts(NULL, _t4651, 3));
#line 187 "compiler/main.gem"
    GemVal _t4652 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4652, gem_string("else")), GEM_NIL))) {
#line 188 "compiler/main.gem"
    GemVal _t4653 = gem_v_node;
    GemVal _t4654[] = {gem_table_get(_t4653, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4655 = gem_fn_rename_stmts(NULL, _t4654, 3);
                gem_pop_frame();
                return _t4655;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4656[] = {gem_v__match_4};
    GemVal _t4658;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4656, 1), gem_string("table")))) {
        _t4658 = gem_eq(gem_type_fn(NULL, _t4656, 1), gem_string("table"));
    } else {
        GemVal _t4657[] = {gem_v__match_4, gem_string("tag")};
        _t4658 = gem_has_key_fn(NULL, _t4657, 2);
    }
    GemVal _t4659;
    if (!gem_truthy(_t4658)) {
        _t4659 = _t4658;
    } else {
        _t4659 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("while"));
    }
    GemVal _t4661;
    if (!gem_truthy(_t4659)) {
        _t4661 = _t4659;
    } else {
        GemVal _t4660[] = {gem_v__match_4, gem_string("cond")};
        _t4661 = gem_has_key_fn(NULL, _t4660, 2);
    }
    GemVal _t4663;
    if (!gem_truthy(_t4661)) {
        _t4663 = _t4661;
    } else {
        GemVal _t4662[] = {gem_v__match_4, gem_string("body")};
        _t4663 = gem_has_key_fn(NULL, _t4662, 2);
    }
        if (gem_truthy(_t4663)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 191 "compiler/main.gem"
    GemVal _t4664[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4664, 3));
#line 192 "compiler/main.gem"
    GemVal _t4665[] = {gem_v_body, gem_v_rmap, gem_v_shadowed};
            GemVal _t4666 = gem_fn_rename_stmts(NULL, _t4665, 3);
            gem_pop_frame();
            return _t4666;
        } else {
    GemVal _t4667[] = {gem_v__match_4};
    GemVal _t4669;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4667, 1), gem_string("table")))) {
        _t4669 = gem_eq(gem_type_fn(NULL, _t4667, 1), gem_string("table"));
    } else {
        GemVal _t4668[] = {gem_v__match_4, gem_string("tag")};
        _t4669 = gem_has_key_fn(NULL, _t4668, 2);
    }
    GemVal _t4670;
    if (!gem_truthy(_t4669)) {
        _t4670 = _t4669;
    } else {
        _t4670 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("match"));
    }
    GemVal _t4672;
    if (!gem_truthy(_t4670)) {
        _t4672 = _t4670;
    } else {
        GemVal _t4671[] = {gem_v__match_4, gem_string("target")};
        _t4672 = gem_has_key_fn(NULL, _t4671, 2);
    }
    GemVal _t4674;
    if (!gem_truthy(_t4672)) {
        _t4674 = _t4672;
    } else {
        GemVal _t4673[] = {gem_v__match_4, gem_string("whens")};
        _t4674 = gem_has_key_fn(NULL, _t4673, 2);
    }
        if (gem_truthy(_t4674)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_target = gem_table_get(gem_v__match_4, gem_string("target"));
#line 0 "compiler/main.gem"
            GemVal gem_v_whens = gem_table_get(gem_v__match_4, gem_string("whens"));
#line 194 "compiler/main.gem"
    GemVal _t4675[] = {gem_v_target, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4675, 3));
#line 195 "compiler/main.gem"
            GemVal gem_v__for_items_8 = gem_v_whens;
#line 195 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 195 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4676[] = {gem_v__for_items_8};
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_len_fn(NULL, _t4676, 1)))) break;
#line 195 "compiler/main.gem"
                GemVal gem_v_w = gem_table_get(gem_v__for_items_8, gem_v__for_i_8);
#line 195 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 196 "compiler/main.gem"
    GemVal _t4677 = gem_v_w;
    GemVal _t4678[] = {gem_table_get(_t4677, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4678, 3));
#line 197 "compiler/main.gem"
    GemVal _t4679 = gem_v_w;
                if (gem_truthy(gem_neq(gem_table_get(_t4679, gem_string("bindings")), GEM_NIL))) {
#line 198 "compiler/main.gem"
    GemVal _t4680 = gem_v_w;
                    GemVal gem_v__for_items_9 = gem_table_get(_t4680, gem_string("bindings"));
#line 198 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 198 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4681[] = {gem_v__for_items_9};
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t4681, 1)))) break;
#line 198 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_9, gem_v__for_i_9);
#line 198 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 199 "compiler/main.gem"
    GemVal _t4682[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4682, 3));
                    }

                }
#line 202 "compiler/main.gem"
    GemVal _t4683 = gem_v_w;
    GemVal _t4684[] = {gem_table_get(_t4683, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4684, 3));
            }

#line 204 "compiler/main.gem"
    GemVal _t4685 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4685, gem_string("else")), GEM_NIL))) {
#line 205 "compiler/main.gem"
    GemVal _t4686 = gem_v_node;
    GemVal _t4687[] = {gem_table_get(_t4686, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4688 = gem_fn_rename_stmts(NULL, _t4687, 3);
                gem_pop_frame();
                return _t4688;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4689[] = {gem_v__match_4};
    GemVal _t4691;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4689, 1), gem_string("table")))) {
        _t4691 = gem_eq(gem_type_fn(NULL, _t4689, 1), gem_string("table"));
    } else {
        GemVal _t4690[] = {gem_v__match_4, gem_string("tag")};
        _t4691 = gem_has_key_fn(NULL, _t4690, 2);
    }
    GemVal _t4692;
    if (!gem_truthy(_t4691)) {
        _t4692 = _t4691;
    } else {
        _t4692 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t4694;
    if (!gem_truthy(_t4692)) {
        _t4694 = _t4692;
    } else {
        GemVal _t4693[] = {gem_v__match_4, gem_string("arms")};
        _t4694 = gem_has_key_fn(NULL, _t4693, 2);
    }
        if (gem_truthy(_t4694)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_arms = gem_table_get(gem_v__match_4, gem_string("arms"));
#line 208 "compiler/main.gem"
            GemVal gem_v__for_items_10 = gem_v_arms;
#line 208 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 208 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4695[] = {gem_v__for_items_10};
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t4695, 1)))) break;
#line 208 "compiler/main.gem"
                GemVal gem_v_arm = gem_table_get(gem_v__for_items_10, gem_v__for_i_10);
#line 208 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 209 "compiler/main.gem"
    GemVal _t4696 = gem_v_arm;
    GemVal _t4697 = gem_table_get(_t4696, gem_string("pattern"));
    GemVal _t4698[] = {gem_table_get(_t4697, gem_string("condition")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4698, 3));
#line 210 "compiler/main.gem"
    GemVal _t4699 = gem_v_arm;
    GemVal _t4700 = gem_table_get(_t4699, gem_string("pattern"));
                if (gem_truthy(gem_neq(gem_table_get(_t4700, gem_string("bindings")), GEM_NIL))) {
#line 211 "compiler/main.gem"
    GemVal _t4701 = gem_v_arm;
    GemVal _t4702 = gem_table_get(_t4701, gem_string("pattern"));
                    GemVal gem_v__for_items_11 = gem_table_get(_t4702, gem_string("bindings"));
#line 211 "compiler/main.gem"
                    GemVal gem_v__for_i_11 = gem_int(0);
#line 211 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4703[] = {gem_v__for_items_11};
                        if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t4703, 1)))) break;
#line 211 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_11, gem_v__for_i_11);
#line 211 "compiler/main.gem"
                        gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 212 "compiler/main.gem"
    GemVal _t4704[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4704, 3));
                    }

                }
#line 215 "compiler/main.gem"
    GemVal _t4705 = gem_v_arm;
    GemVal _t4706[] = {gem_table_get(_t4705, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4706, 3));
            }

#line 217 "compiler/main.gem"
    GemVal _t4707 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4707, gem_string("after_ms")), GEM_NIL))) {
#line 218 "compiler/main.gem"
    GemVal _t4708 = gem_v_node;
    GemVal _t4709[] = {gem_table_get(_t4708, gem_string("after_ms")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4709, 3));
            }
#line 220 "compiler/main.gem"
    GemVal _t4710 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4710, gem_string("after_body")), GEM_NIL))) {
#line 221 "compiler/main.gem"
    GemVal _t4711 = gem_v_node;
    GemVal _t4712[] = {gem_table_get(_t4711, gem_string("after_body")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4713 = gem_fn_rename_stmts(NULL, _t4712, 3);
                gem_pop_frame();
                return _t4713;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4714[] = {gem_v__match_4};
    GemVal _t4716;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4714, 1), gem_string("table")))) {
        _t4716 = gem_eq(gem_type_fn(NULL, _t4714, 1), gem_string("table"));
    } else {
        GemVal _t4715[] = {gem_v__match_4, gem_string("tag")};
        _t4716 = gem_has_key_fn(NULL, _t4715, 2);
    }
    GemVal _t4717;
    if (!gem_truthy(_t4716)) {
        _t4717 = _t4716;
    } else {
        _t4717 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("return"));
    }
    GemVal _t4719;
    if (!gem_truthy(_t4717)) {
        _t4719 = _t4717;
    } else {
        GemVal _t4718[] = {gem_v__match_4, gem_string("value")};
        _t4719 = gem_has_key_fn(NULL, _t4718, 2);
    }
        if (gem_truthy(_t4719)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 224 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 225 "compiler/main.gem"
                GemVal _t4720 = gem_v_value;
                GemVal _t4721 = gem_v_rmap;
                GemVal _t4722 = gem_v_shadowed;
                gem_v_node = _t4720;
                gem_v_rmap = _t4721;
                gem_v_shadowed = _t4722;
                continue;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4723[] = {gem_v__match_4};
    GemVal _t4725;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4723, 1), gem_string("table")))) {
        _t4725 = gem_eq(gem_type_fn(NULL, _t4723, 1), gem_string("table"));
    } else {
        GemVal _t4724[] = {gem_v__match_4, gem_string("tag")};
        _t4725 = gem_has_key_fn(NULL, _t4724, 2);
    }
    GemVal _t4726;
    if (!gem_truthy(_t4725)) {
        _t4726 = _t4725;
    } else {
        _t4726 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("table"));
    }
    GemVal _t4728;
    if (!gem_truthy(_t4726)) {
        _t4728 = _t4726;
    } else {
        GemVal _t4727[] = {gem_v__match_4, gem_string("entries")};
        _t4728 = gem_has_key_fn(NULL, _t4727, 2);
    }
        if (gem_truthy(_t4728)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_entries = gem_table_get(gem_v__match_4, gem_string("entries"));
            {
#line 228 "compiler/main.gem"
                GemVal gem_v__for_items_12 = gem_v_entries;
#line 228 "compiler/main.gem"
                GemVal gem_v__for_i_12 = gem_int(0);
#line 228 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4729[] = {gem_v__for_items_12};
                    if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t4729, 1)))) break;
#line 228 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 228 "compiler/main.gem"
                    gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 229 "compiler/main.gem"
    GemVal _t4730 = gem_v_e;
    GemVal _t4731[] = {gem_table_get(_t4730, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4731, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4732[] = {gem_v__match_4};
    GemVal _t4734;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4732, 1), gem_string("table")))) {
        _t4734 = gem_eq(gem_type_fn(NULL, _t4732, 1), gem_string("table"));
    } else {
        GemVal _t4733[] = {gem_v__match_4, gem_string("tag")};
        _t4734 = gem_has_key_fn(NULL, _t4733, 2);
    }
    GemVal _t4735;
    if (!gem_truthy(_t4734)) {
        _t4735 = _t4734;
    } else {
        _t4735 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("array"));
    }
    GemVal _t4737;
    if (!gem_truthy(_t4735)) {
        _t4737 = _t4735;
    } else {
        GemVal _t4736[] = {gem_v__match_4, gem_string("elements")};
        _t4737 = gem_has_key_fn(NULL, _t4736, 2);
    }
        if (gem_truthy(_t4737)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_elements = gem_table_get(gem_v__match_4, gem_string("elements"));
            {
#line 232 "compiler/main.gem"
                GemVal gem_v__for_items_13 = gem_v_elements;
#line 232 "compiler/main.gem"
                GemVal gem_v__for_i_13 = gem_int(0);
#line 232 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4738[] = {gem_v__for_items_13};
                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_len_fn(NULL, _t4738, 1)))) break;
#line 232 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_13, gem_v__for_i_13);
#line 232 "compiler/main.gem"
                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 233 "compiler/main.gem"
    GemVal _t4739[] = {gem_v_e, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4739, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4740[] = {gem_v__match_4};
    GemVal _t4742;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4740, 1), gem_string("table")))) {
        _t4742 = gem_eq(gem_type_fn(NULL, _t4740, 1), gem_string("table"));
    } else {
        GemVal _t4741[] = {gem_v__match_4, gem_string("tag")};
        _t4742 = gem_has_key_fn(NULL, _t4741, 2);
    }
    GemVal _t4743;
    if (!gem_truthy(_t4742)) {
        _t4743 = _t4742;
    } else {
        _t4743 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t4745;
    if (!gem_truthy(_t4743)) {
        _t4745 = _t4743;
    } else {
        GemVal _t4744[] = {gem_v__match_4, gem_string("parts")};
        _t4745 = gem_has_key_fn(NULL, _t4744, 2);
    }
        if (gem_truthy(_t4745)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_parts = gem_table_get(gem_v__match_4, gem_string("parts"));
            {
#line 236 "compiler/main.gem"
                GemVal gem_v__for_items_14 = gem_v_parts;
#line 236 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 236 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4746[] = {gem_v__for_items_14};
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_len_fn(NULL, _t4746, 1)))) break;
#line 236 "compiler/main.gem"
                    GemVal gem_v_p = gem_table_get(gem_v__for_items_14, gem_v__for_i_14);
#line 236 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 237 "compiler/main.gem"
    GemVal _t4747[] = {gem_v_p, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4747, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4748[] = {gem_v__match_4};
    GemVal _t4750;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4748, 1), gem_string("table")))) {
        _t4750 = gem_eq(gem_type_fn(NULL, _t4748, 1), gem_string("table"));
    } else {
        GemVal _t4749[] = {gem_v__match_4, gem_string("tag")};
        _t4750 = gem_has_key_fn(NULL, _t4749, 2);
    }
    GemVal _t4751;
    if (!gem_truthy(_t4750)) {
        _t4751 = _t4750;
    } else {
        _t4751 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("block"));
    }
    GemVal _t4753;
    if (!gem_truthy(_t4751)) {
        _t4753 = _t4751;
    } else {
        GemVal _t4752[] = {gem_v__match_4, gem_string("stmts")};
        _t4753 = gem_has_key_fn(NULL, _t4752, 2);
    }
        if (gem_truthy(_t4753)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_stmts = gem_table_get(gem_v__match_4, gem_string("stmts"));
#line 240 "compiler/main.gem"
    GemVal _t4754[] = {gem_v_stmts, gem_v_rmap, gem_v_shadowed};
            GemVal _t4755 = gem_fn_rename_stmts(NULL, _t4754, 3);
            gem_pop_frame();
            return _t4755;
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
#line 244 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rmap = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_shadowed = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("rename_stmts", "compiler/main.gem", 244);
#line 245 "compiler/main.gem"
    GemVal _t4756[] = {gem_v_shadowed};
    GemVal gem_v_current = gem_fn_copy_set(NULL, _t4756, 1);
    {
#line 246 "compiler/main.gem"
        GemVal gem_v__for_items_15 = gem_v_stmts;
#line 246 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 246 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4757[] = {gem_v__for_items_15};
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_len_fn(NULL, _t4757, 1)))) break;
#line 246 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_15, gem_v__for_i_15);
#line 246 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 247 "compiler/main.gem"
    GemVal _t4758[] = {gem_v_stmt, gem_v_rmap, gem_v_current};
            (void)(gem_fn_rename_node(NULL, _t4758, 3));
#line 248 "compiler/main.gem"
    GemVal _t4759[] = {gem_v_stmt, gem_string("let")};
    GemVal _t4762;
    if (!gem_truthy(gem_fn_is_node(NULL, _t4759, 2))) {
        _t4762 = gem_fn_is_node(NULL, _t4759, 2);
    } else {
        GemVal _t4760 = gem_v_stmt;
        GemVal _t4761[] = {gem_v_rmap, gem_table_get(_t4760, gem_string("name"))};
        _t4762 = gem_not(gem_has_key_fn(NULL, _t4761, 2));
    }
            if (gem_truthy(_t4762)) {
#line 249 "compiler/main.gem"
    GemVal _t4763 = gem_v_stmt;
                gem_table_set(gem_v_current, gem_table_get(_t4763, gem_string("name")), gem_bool(1));
            }
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_transform_module(void *_env, GemVal *args, int argc) {
#line 254 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mod_name = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_export_node = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_load_node = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("transform_module", "compiler/main.gem", 254);
#line 255 "compiler/main.gem"
    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_v_mod_name), gem_string("_"));
#line 256 "compiler/main.gem"
    GemVal _t4764[] = {gem_v_stmts};
    GemVal gem_v_top_names = gem_fn_collect_module_top_names(NULL, _t4764, 1);
#line 257 "compiler/main.gem"
    GemVal _t4765[] = {gem_v_top_names, gem_v_prefix};
    GemVal gem_v_rmap = gem_fn_build_rename_map(NULL, _t4765, 2);
#line 259 "compiler/main.gem"
    GemVal _t4766 = gem_table_new();
    GemVal _t4767[] = {gem_v_stmts, gem_v_rmap, _t4766};
    (void)(gem_fn_rename_stmts(NULL, _t4767, 3));
#line 261 "compiler/main.gem"
    GemVal _t4768 = gem_table_new();
    GemVal gem_v_result = _t4768;
#line 262 "compiler/main.gem"
    GemVal gem_v__for_items_16 = gem_v_stmts;
#line 262 "compiler/main.gem"
    GemVal gem_v__for_i_16 = gem_int(0);
#line 262 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4769[] = {gem_v__for_items_16};
        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t4769, 1)))) break;
#line 262 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_16, gem_v__for_i_16);
#line 262 "compiler/main.gem"
        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 263 "compiler/main.gem"
    GemVal _t4770[] = {gem_v_stmt, gem_string("export")};
        if (gem_truthy(gem_not(gem_fn_is_node(NULL, _t4770, 2)))) {
#line 264 "compiler/main.gem"
    GemVal _t4771[] = {gem_v_result, gem_v_stmt};
            (void)(gem_push_fn(NULL, _t4771, 2));
        }
    }

#line 268 "compiler/main.gem"
    GemVal _t4772 = gem_v_export_node;
    GemVal gem_v_exported = gem_table_get(_t4772, gem_string("names"));
#line 270 "compiler/main.gem"
    GemVal _t4773 = gem_v_load_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4773, gem_string("selective")), GEM_NIL))) {
#line 271 "compiler/main.gem"
    GemVal _t4774 = gem_v_load_node;
        GemVal gem_v__for_items_17 = gem_table_get(_t4774, gem_string("selective"));
#line 271 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 271 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4775[] = {gem_v__for_items_17};
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_len_fn(NULL, _t4775, 1)))) break;
#line 271 "compiler/main.gem"
            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_17, gem_v__for_i_17);
#line 271 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 272 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_sel_name);
#line 273 "compiler/main.gem"
    GemVal _t4776[] = {gem_v_prefixed};
    GemVal _t4777[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4776, 1), gem_int(0)};
    GemVal _t4778[] = {gem_v_result, gem_fn_make_let(NULL, _t4777, 3)};
            (void)(gem_push_fn(NULL, _t4778, 2));
        }

    } else {
#line 276 "compiler/main.gem"
        GemVal gem_v_binding_name = gem_v_mod_name;
#line 277 "compiler/main.gem"
    GemVal _t4779 = gem_v_load_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4779, gem_string("alias")), GEM_NIL))) {
#line 278 "compiler/main.gem"
    GemVal _t4780 = gem_v_load_node;
            gem_v_binding_name = gem_table_get(_t4780, gem_string("alias"));
        }
#line 280 "compiler/main.gem"
    GemVal _t4781 = gem_table_new();
        GemVal gem_v_entries = _t4781;
#line 281 "compiler/main.gem"
        GemVal gem_v__for_items_18 = gem_v_exported;
#line 281 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 281 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4782[] = {gem_v__for_items_18};
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_len_fn(NULL, _t4782, 1)))) break;
#line 281 "compiler/main.gem"
            GemVal gem_v_ename = gem_table_get(gem_v__for_items_18, gem_v__for_i_18);
#line 281 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 282 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_ename);
#line 283 "compiler/main.gem"
    GemVal _t4783[] = {gem_v_prefixed};
    GemVal _t4784[] = {gem_v_ename, gem_fn_make_var(NULL, _t4783, 1)};
    GemVal _t4785[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4784, 2)};
            (void)(gem_push_fn(NULL, _t4785, 2));
        }

#line 285 "compiler/main.gem"
    GemVal _t4786[] = {gem_v_entries};
    GemVal _t4787[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4786, 1), gem_int(0)};
    GemVal _t4788[] = {gem_v_result, gem_fn_make_let(NULL, _t4787, 3)};
        (void)(gem_push_fn(NULL, _t4788, 2));
    }
    GemVal _t4789 = gem_v_result;
    gem_pop_frame();
    return _t4789;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 293 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 293);
#line 294 "compiler/main.gem"
    GemVal _t4790 = gem_table_new();
    GemVal gem_v_new_stmts = _t4790;
#line 295 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 296 "compiler/main.gem"
    GemVal _t4791 = gem_v_ast;
    GemVal gem_v__for_items_19 = gem_table_get(_t4791, gem_string("stmts"));
#line 296 "compiler/main.gem"
    GemVal gem_v__for_i_19 = gem_int(0);
#line 296 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4792[] = {gem_v__for_items_19};
        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t4792, 1)))) break;
#line 296 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
#line 296 "compiler/main.gem"
        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 297 "compiler/main.gem"
    GemVal _t4793[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4793, 2))) {
#line 298 "compiler/main.gem"
    GemVal _t4794 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t4794, gem_string("path"));
#line 299 "compiler/main.gem"
    GemVal _t4795 = gem_v_string;
    GemVal _t4796[] = {gem_v_path, gem_string(".gem")};
    GemVal _t4797 = gem_table_get(_t4795, gem_string("ends_with"));
            if (gem_truthy(gem_not(_t4797.fn(_t4797.env, _t4796, 2)))) {
#line 300 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 303 "compiler/main.gem"
    GemVal _t4798[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t4798, 2);
#line 304 "compiler/main.gem"
    GemVal _t4799[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t4799, 1)))) {
#line 305 "compiler/main.gem"
    GemVal _t4800[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t4800, 2);
            }
#line 308 "compiler/main.gem"
    GemVal _t4801[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t4801, 1);
#line 310 "compiler/main.gem"
            GemVal gem_v_cached = gem_table_get(gem_v_loaded, gem_v_full_path);
#line 311 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_cached, GEM_NIL))) {
#line 312 "compiler/main.gem"
    GemVal _t4802 = gem_v_cached;
                if (gem_truthy(gem_neq(gem_table_get(_t4802, gem_string("exported")), GEM_NIL))) {
#line 313 "compiler/main.gem"
    GemVal _t4803 = gem_v_cached;
                    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_table_get(_t4803, gem_string("mod_name"))), gem_string("_"));
#line 314 "compiler/main.gem"
    GemVal _t4804 = gem_v_stmt;
                    if (gem_truthy(gem_neq(gem_table_get(_t4804, gem_string("selective")), GEM_NIL))) {
#line 315 "compiler/main.gem"
    GemVal _t4805 = gem_v_stmt;
                        GemVal gem_v__for_items_20 = gem_table_get(_t4805, gem_string("selective"));
#line 315 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(0);
#line 315 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4806[] = {gem_v__for_items_20};
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t4806, 1)))) break;
#line 315 "compiler/main.gem"
                            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_20, gem_v__for_i_20);
#line 315 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 316 "compiler/main.gem"
    GemVal _t4807[] = {gem_add(gem_v_prefix, gem_v_sel_name)};
    GemVal _t4808[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4807, 1), gem_int(0)};
                            gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4808, 3));
#line 317 "compiler/main.gem"
                            gem_v_count = gem_add(gem_v_count, gem_int(1));
                        }

                    } else {
#line 320 "compiler/main.gem"
    GemVal _t4809 = gem_v_cached;
                        GemVal gem_v_binding_name = gem_table_get(_t4809, gem_string("mod_name"));
#line 321 "compiler/main.gem"
    GemVal _t4810 = gem_v_stmt;
                        if (gem_truthy(gem_neq(gem_table_get(_t4810, gem_string("alias")), GEM_NIL))) {
#line 322 "compiler/main.gem"
    GemVal _t4811 = gem_v_stmt;
                            gem_v_binding_name = gem_table_get(_t4811, gem_string("alias"));
                        }
#line 324 "compiler/main.gem"
    GemVal _t4812 = gem_table_new();
                        GemVal gem_v_entries = _t4812;
#line 325 "compiler/main.gem"
    GemVal _t4813 = gem_v_cached;
                        GemVal gem_v__for_items_21 = gem_table_get(_t4813, gem_string("exported"));
#line 325 "compiler/main.gem"
                        GemVal gem_v__for_i_21 = gem_int(0);
#line 325 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4814[] = {gem_v__for_items_21};
                            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t4814, 1)))) break;
#line 325 "compiler/main.gem"
                            GemVal gem_v_ename = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 325 "compiler/main.gem"
                            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 326 "compiler/main.gem"
    GemVal _t4815[] = {gem_add(gem_v_prefix, gem_v_ename)};
    GemVal _t4816[] = {gem_v_ename, gem_fn_make_var(NULL, _t4815, 1)};
    GemVal _t4817[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4816, 2)};
                            (void)(gem_push_fn(NULL, _t4817, 2));
                        }

#line 328 "compiler/main.gem"
    GemVal _t4818[] = {gem_v_entries};
    GemVal _t4819[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4818, 1), gem_int(0)};
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4819, 3));
#line 329 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
                continue;
            }
#line 335 "compiler/main.gem"
    GemVal _t4820 = gem_table_new();
    gem_table_set(_t4820, gem_string("exported"), GEM_NIL);
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4820);
#line 337 "compiler/main.gem"
    GemVal _t4821[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t4821, 1);
#line 338 "compiler/main.gem"
    GemVal _t4822[] = {gem_v_source, gem_v_full_path};
    GemVal _t4823 = gem_v_parse_source;
            GemVal gem_v_loaded_ast = _t4823.fn(_t4823.env, _t4822, 2);
#line 339 "compiler/main.gem"
    GemVal _t4824[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t4824, 1);
#line 340 "compiler/main.gem"
    GemVal _t4825[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t4825, 3);
#line 342 "compiler/main.gem"
    GemVal _t4826 = gem_v_resolved;
    GemVal _t4827[] = {gem_table_get(_t4826, gem_string("stmts"))};
            GemVal gem_v_export_node = gem_fn_find_export_node(NULL, _t4827, 1);
#line 343 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_export_node, GEM_NIL))) {
#line 344 "compiler/main.gem"
    GemVal _t4828[] = {gem_add(gem_string("gem: loaded file has no export statement: "), gem_v_full_path)};
                (void)(gem_error_at_fn("compiler/main.gem", 344, _t4828, 1));
            }
#line 347 "compiler/main.gem"
    GemVal _t4829 = gem_v_stmt;
    GemVal _t4830[] = {gem_table_get(_t4829, gem_string("path"))};
            GemVal gem_v_mod_name = gem_fn_module_name_from_path(NULL, _t4830, 1);
#line 348 "compiler/main.gem"
    GemVal _t4831 = gem_table_new();
    gem_table_set(_t4831, gem_string("mod_name"), gem_v_mod_name);
    GemVal _t4832 = gem_v_export_node;
    gem_table_set(_t4831, gem_string("exported"), gem_table_get(_t4832, gem_string("names")));
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4831);
#line 349 "compiler/main.gem"
    GemVal _t4833 = gem_v_resolved;
    GemVal _t4834[] = {gem_table_get(_t4833, gem_string("stmts")), gem_v_mod_name, gem_v_export_node, gem_v_stmt};
            GemVal gem_v_mod_stmts = gem_fn_transform_module(NULL, _t4834, 4);
#line 350 "compiler/main.gem"
            GemVal gem_v__for_items_22 = gem_v_mod_stmts;
#line 350 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 350 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4835[] = {gem_v__for_items_22};
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t4835, 1)))) break;
#line 350 "compiler/main.gem"
                GemVal gem_v_ms = gem_table_get(gem_v__for_items_22, gem_v__for_i_22);
#line 350 "compiler/main.gem"
                gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 351 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_ms);
#line 352 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }

        } else {
#line 355 "compiler/main.gem"
            gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 356 "compiler/main.gem"
            gem_v_count = gem_add(gem_v_count, gem_int(1));
        }
    }

#line 359 "compiler/main.gem"
    GemVal _t4836[] = {gem_v_new_stmts};
    GemVal _t4837 = gem_fn_make_program(NULL, _t4836, 1);
    gem_pop_frame();
    return _t4837;
}

static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc) {
#line 364 "compiler/main.gem"
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_args", "compiler/main.gem", 364);
#line 365 "compiler/main.gem"
    GemVal _t4838 = gem_table_new();
    gem_table_set(_t4838, gem_string("src_path"), GEM_NIL);
    gem_table_set(_t4838, gem_string("out_name"), GEM_NIL);
    gem_table_set(_t4838, gem_string("emit_c"), gem_bool(0));
    gem_table_set(_t4838, gem_string("run"), gem_bool(0));
    GemVal _t4839 = gem_table_new();
    gem_table_set(_t4838, gem_string("run_args"), _t4839);
    GemVal gem_v_result = _t4838;
#line 373 "compiler/main.gem"
    GemVal _t4840[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t4840, 1);
#line 374 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 375 "compiler/main.gem"
    GemVal gem_v_run_arg_count = gem_int(0);
#line 376 "compiler/main.gem"
    GemVal gem_v_found_run = gem_bool(0);
#line 378 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_argc))) break;
#line 379 "compiler/main.gem"
        GemVal gem_v_arg = gem_table_get(gem_v_args, gem_v_i);
#line 380 "compiler/main.gem"
        if (gem_truthy(gem_v_found_run)) {
#line 381 "compiler/main.gem"
    GemVal _t4841 = gem_v_result;
            gem_table_set(gem_table_get(_t4841, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
#line 382 "compiler/main.gem"
            gem_v_run_arg_count = gem_add(gem_v_run_arg_count, gem_int(1));
        } else {
#line 383 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_arg, gem_string("--emit-c")))) {
#line 384 "compiler/main.gem"
                gem_table_set(gem_v_result, gem_string("emit_c"), gem_bool(1));
            } else {
#line 385 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_arg, gem_string("--run")))) {
#line 386 "compiler/main.gem"
                    gem_table_set(gem_v_result, gem_string("run"), gem_bool(1));
#line 387 "compiler/main.gem"
                    gem_v_found_run = gem_bool(1);
                } else {
#line 388 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_arg, gem_string("-o")))) {
#line 389 "compiler/main.gem"
                        gem_v_i = gem_add(gem_v_i, gem_int(1));
#line 390 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_i, gem_v_argc))) {
#line 391 "compiler/main.gem"
    GemVal _t4842[] = {gem_string("gem: -o requires an argument")};
                            (void)(gem_error_at_fn("compiler/main.gem", 391, _t4842, 1));
                        }
#line 393 "compiler/main.gem"
                        gem_table_set(gem_v_result, gem_string("out_name"), gem_table_get(gem_v_args, gem_v_i));
                    } else {
#line 394 "compiler/main.gem"
    GemVal _t4843 = gem_v_result;
                        if (gem_truthy(gem_eq(gem_table_get(_t4843, gem_string("src_path")), GEM_NIL))) {
#line 395 "compiler/main.gem"
                            gem_table_set(gem_v_result, gem_string("src_path"), gem_v_arg);
                        } else {
#line 398 "compiler/main.gem"
    GemVal _t4844 = gem_v_result;
                            gem_table_set(gem_table_get(_t4844, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
#line 399 "compiler/main.gem"
                            gem_v_run_arg_count = gem_add(gem_v_run_arg_count, gem_int(1));
                        }
                    }
                }
            }
        }
#line 401 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t4845 = gem_v_result;
    gem_pop_frame();
    return _t4845;
}

static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc) {
#line 409 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("basename_no_ext", "compiler/main.gem", 409);
#line 411 "compiler/main.gem"
    GemVal _t4846[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4846, 1);
#line 412 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 413 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 414 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 415 "compiler/main.gem"
    GemVal _t4847[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4847, 2), gem_int(47)))) {
#line 416 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 418 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 420 "compiler/main.gem"
    GemVal _t4848[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4848, 3);
#line 422 "compiler/main.gem"
    GemVal _t4849[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4849, 1);
#line 423 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 424 "compiler/main.gem"
    GemVal _t4850[] = {gem_v_name, gem_sub(gem_v_nlen, gem_int(4)), gem_int(4)};
        GemVal gem_v_maybe_ext = gem_substr_fn(NULL, _t4850, 3);
#line 425 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_maybe_ext, gem_string(".gem")))) {
#line 426 "compiler/main.gem"
    GemVal _t4851[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4851, 3);
        }
    }
    GemVal _t4852 = gem_v_name;
    gem_pop_frame();
    return _t4852;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_push_frame("main", "compiler/main.gem", 0);
#line 0 "compiler/main.gem"
    GemVal _t4853 = gem_table_new();
    gem_table_set(_t4853, gem_string("split"), gem_make_fn(gem_fn__mod_string_split, NULL));
    gem_table_set(_t4853, gem_string("index_of"), gem_make_fn(gem_fn__mod_string_index_of, NULL));
    gem_table_set(_t4853, gem_string("join"), gem_make_fn(gem_fn__mod_string_join, NULL));
    gem_table_set(_t4853, gem_string("trim"), gem_make_fn(gem_fn__mod_string_trim, NULL));
    gem_table_set(_t4853, gem_string("starts_with"), gem_make_fn(gem_fn__mod_string_starts_with, NULL));
    gem_table_set(_t4853, gem_string("ends_with"), gem_make_fn(gem_fn__mod_string_ends_with, NULL));
    gem_table_set(_t4853, gem_string("upper"), gem_make_fn(gem_fn__mod_string_upper, NULL));
    gem_table_set(_t4853, gem_string("lower"), gem_make_fn(gem_fn__mod_string_lower, NULL));
    gem_table_set(_t4853, gem_string("contains"), gem_make_fn(gem_fn__mod_string_contains, NULL));
    gem_table_set(_t4853, gem_string("repeat"), gem_make_fn(gem_fn__mod_string_repeat, NULL));
    gem_v_string = _t4853;
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_program = gem_make_fn(gem_fn__mod_ast_make_program, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_int = gem_make_fn(gem_fn__mod_ast_make_int, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_float = gem_make_fn(gem_fn__mod_ast_make_float, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_string = gem_make_fn(gem_fn__mod_ast_make_string, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_bool = gem_make_fn(gem_fn__mod_ast_make_bool, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_nil_node = gem_make_fn(gem_fn__mod_ast_make_nil_node, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_var = gem_make_fn(gem_fn__mod_ast_make_var, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_binop = gem_make_fn(gem_fn__mod_ast_make_binop, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_unop = gem_make_fn(gem_fn__mod_ast_make_unop, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_call = gem_make_fn(gem_fn__mod_ast_make_call, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_dot = gem_make_fn(gem_fn__mod_ast_make_dot, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_index = gem_make_fn(gem_fn__mod_ast_make_index, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_anon_fn = gem_make_fn(gem_fn__mod_ast_make_anon_fn, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_table = gem_make_fn(gem_fn__mod_ast_make_table, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_table_entry = gem_make_fn(gem_fn__mod_ast_make_table_entry, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_array = gem_make_fn(gem_fn__mod_ast_make_array, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_interp = gem_make_fn(gem_fn__mod_ast_make_interp, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_let = gem_make_fn(gem_fn__mod_ast_make_let, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_assign = gem_make_fn(gem_fn__mod_ast_make_assign, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_dot_assign = gem_make_fn(gem_fn__mod_ast_make_dot_assign, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_index_assign = gem_make_fn(gem_fn__mod_ast_make_index_assign, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_fn_def = gem_make_fn(gem_fn__mod_ast_make_fn_def, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_if = gem_make_fn(gem_fn__mod_ast_make_if, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_while = gem_make_fn(gem_fn__mod_ast_make_while, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_match = gem_make_fn(gem_fn__mod_ast_make_match, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_when = gem_make_fn(gem_fn__mod_ast_make_when, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_receive_match = gem_make_fn(gem_fn__mod_ast_make_receive_match, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_return = gem_make_fn(gem_fn__mod_ast_make_return, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_break = gem_make_fn(gem_fn__mod_ast_make_break, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_continue = gem_make_fn(gem_fn__mod_ast_make_continue, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_export = gem_make_fn(gem_fn__mod_ast_make_export, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_extern_fn = gem_make_fn(gem_fn__mod_ast_make_extern_fn, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_extern_param = gem_make_fn(gem_fn__mod_ast_make_extern_param, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_make_extern_include = gem_make_fn(gem_fn__mod_ast_make_extern_include, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_lexer_compile_error = gem_make_fn(gem_fn__mod_errors_compile_error, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_tokenize = gem_make_fn(gem_fn__mod_lexer_tokenize, NULL);
#line 0 "compiler/main.gem"
    gem_v__mod_parser_compile_error = gem_v__mod_lexer_compile_error;
#line 0 "compiler/main.gem"
    gem_v_parse_source = gem_make_fn(gem_fn__mod_parser_parse_source, NULL);
#line 0 "compiler/main.gem"
    gem_v_make_codegen = gem_make_fn(gem_fn__mod_codegen_make_codegen, NULL);
#line 16 "compiler/main.gem"
    GemVal _t4854[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t4855[] = {gem_dirname_fn(NULL, _t4854, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t4855, 1);
#line 434 "compiler/main.gem"
    GemVal _t4856[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t4856, 1);
#line 435 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 436 "compiler/main.gem"
    GemVal _t4857[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 436, _t4857, 1));
    }
#line 439 "compiler/main.gem"
    GemVal _t4858[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_cli = gem_fn_parse_args(NULL, _t4858, 1);
#line 441 "compiler/main.gem"
    GemVal _t4859 = gem_v_cli;
    if (gem_truthy(gem_eq(gem_table_get(_t4859, gem_string("src_path")), GEM_NIL))) {
#line 442 "compiler/main.gem"
    GemVal _t4860[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 442, _t4860, 1));
    }
#line 445 "compiler/main.gem"
    GemVal _t4861 = gem_v_cli;
    gem_v_src_path = gem_table_get(_t4861, gem_string("src_path"));
#line 446 "compiler/main.gem"
    GemVal _t4862[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t4862, 1);
#line 448 "compiler/main.gem"
    GemVal _t4863[] = {gem_v_source, gem_v_src_path};
    GemVal _t4864 = gem_v_parse_source;
    gem_v_ast = _t4864.fn(_t4864.env, _t4863, 2);
#line 449 "compiler/main.gem"
    GemVal _t4865[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t4865, 1);
#line 450 "compiler/main.gem"
    GemVal _t4866 = gem_table_new();
    gem_v_loaded = _t4866;
#line 451 "compiler/main.gem"
    GemVal _t4867[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t4867, 3);
#line 453 "compiler/main.gem"
    GemVal _t4868[] = {gem_v_src_path};
    GemVal _t4869 = gem_v_make_codegen;
    gem_v_cg = _t4869.fn(_t4869.env, _t4868, 1);
#line 454 "compiler/main.gem"
    GemVal _t4870 = gem_v_cg;
    GemVal _t4871[] = {gem_v_resolved_ast};
    GemVal _t4872 = gem_table_get(_t4870, gem_string("compile"));
    gem_v_c_code = _t4872.fn(_t4872.env, _t4871, 1);
#line 458 "compiler/main.gem"
    GemVal _t4873 = gem_v_cli;
    if (gem_truthy(gem_table_get(_t4873, gem_string("emit_c")))) {
#line 459 "compiler/main.gem"
    GemVal _t4874[] = {gem_v_c_code};
        (void)(gem_print(NULL, _t4874, 1));
    } else {
#line 463 "compiler/main.gem"
    GemVal _t4875[] = {gem_v_src_path};
        GemVal gem_v_base = gem_fn_basename_no_ext(NULL, _t4875, 1);
#line 464 "compiler/main.gem"
        GemVal gem_v_tmp_c = gem_add(gem_add(gem_string("/tmp/gem_"), gem_v_base), gem_string(".c"));
#line 467 "compiler/main.gem"
    GemVal _t4876 = gem_v_cli;
        GemVal gem_v_out_name = gem_table_get(_t4876, gem_string("out_name"));
#line 468 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_out_name, GEM_NIL))) {
#line 469 "compiler/main.gem"
            gem_v_out_name = gem_add(gem_string("./"), gem_v_base);
        }
#line 473 "compiler/main.gem"
    GemVal _t4877[] = {gem_v_tmp_c, gem_v_c_code};
        (void)(gem_write_file_fn(NULL, _t4877, 2));
#line 476 "compiler/main.gem"
    GemVal _t4878[] = {gem_v_install_root, gem_string("runtime")};
        GemVal gem_v_runtime_dir = gem_path_join_fn(NULL, _t4878, 2);
#line 479 "compiler/main.gem"
        GemVal gem_v_gc_flags = gem_string("-lgc");
#line 480 "compiler/main.gem"
    GemVal _t4879[] = {gem_string("pkg-config --cflags --libs bdw-gc > /tmp/gem_gc_flags.txt 2>/dev/null")};
        GemVal gem_v_pkg_ret = gem_exec_fn(NULL, _t4879, 1);
#line 481 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_pkg_ret, gem_int(0)))) {
#line 482 "compiler/main.gem"
    GemVal _t4880[] = {gem_string("/tmp/gem_gc_flags.txt")};
            gem_v_gc_flags = gem_read_file_fn(NULL, _t4880, 1);
#line 484 "compiler/main.gem"
    GemVal _t4881[] = {gem_v_gc_flags, gem_string("\n"), gem_string("")};
            gem_v_gc_flags = gem_str_replace_fn(NULL, _t4881, 3);
        }
#line 487 "compiler/main.gem"
        GemVal gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" ")), gem_v_runtime_dir), gem_string("/gem_*.c -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread"));
#line 489 "compiler/main.gem"
    GemVal _t4882[] = {gem_v_cc_cmd};
        GemVal gem_v_cc_ret = gem_exec_fn(NULL, _t4882, 1);
#line 490 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_cc_ret, gem_int(0)))) {
#line 491 "compiler/main.gem"
    GemVal _t4883[] = {gem_v_cc_ret};
    GemVal _t4884[] = {gem_add(gem_add(gem_string("gem: compilation failed (cc exited with "), gem_to_string_fn(NULL, _t4883, 1)), gem_string(")"))};
            (void)(gem_error_at_fn("compiler/main.gem", 491, _t4884, 1));
        }
#line 496 "compiler/main.gem"
    GemVal _t4885 = gem_v_cli;
        if (gem_truthy(gem_table_get(_t4885, gem_string("run")))) {
#line 497 "compiler/main.gem"
            GemVal gem_v_run_cmd = gem_v_out_name;
#line 498 "compiler/main.gem"
            GemVal gem_v_i = gem_int(0);
#line 499 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4886 = gem_v_cli;
                GemVal _t4887[] = {gem_table_get(_t4886, gem_string("run_args"))};
                if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t4887, 1)))) break;
#line 500 "compiler/main.gem"
    GemVal _t4888 = gem_v_cli;
                gem_v_run_cmd = gem_add(gem_add(gem_v_run_cmd, gem_string(" ")), gem_table_get(gem_table_get(_t4888, gem_string("run_args")), gem_v_i));
#line 501 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 503 "compiler/main.gem"
    GemVal _t4889[] = {gem_v_run_cmd};
            GemVal gem_v_run_ret = gem_exec_fn(NULL, _t4889, 1);
#line 504 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_run_ret, gem_int(0)))) {
#line 505 "compiler/main.gem"
    GemVal _t4890[] = {gem_v_run_ret};
                (void)(gem_exit_process_fn(NULL, _t4890, 1));
            }
        }
    }
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

