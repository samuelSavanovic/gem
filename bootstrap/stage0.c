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
    GemVal gem_v_body = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_anon_fn", "compiler/main.gem", 58);
    GemVal _t101 = gem_table_new();
    gem_table_set(_t101, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t101, gem_string("params"), gem_v_params);
    gem_table_set(_t101, gem_string("rest_param"), gem_v_rest_param);
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
    GemVal gem_v_body = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_line = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("_mod_ast_make_fn_def", "compiler/main.gem", 96);
    GemVal _t119 = gem_table_new();
    gem_table_set(_t119, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t119, gem_string("name"), gem_v_name);
    gem_table_set(_t119, gem_string("params"), gem_v_params);
    gem_table_set(_t119, gem_string("rest_param"), gem_v_rest_param);
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
    GemVal _t405[] = {gem_string("unexpected character: '"), gem_v_ch, gem_string("'")};
    GemVal _t406[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_interp(3, _t405), GEM_NIL};
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
    GemVal _t421[] = {gem_string("not a digit: "), gem_v_ch};
    GemVal _t422[] = {gem_interp(2, _t421)};
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
    GemVal _t450 = gem_v_t;
    GemVal _t451[] = {gem_string("expected '"), gem_v_tp, gem_string("' but got '"), gem_table_get(_t450, gem_string("type")), gem_string("'")};
    GemVal _t452[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t446, gem_string("line")), gem_table_get(_t447, gem_string("col")), gem_len_fn(NULL, _t449, 1), gem_interp(5, _t451), GEM_NIL};
    GemVal _t453 = (*gem_v__mod_parser_compile_error);
        (void)(_t453.fn(_t453.env, _t452, 7));
    }
#line 96 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t454 = gem_v_t;
    gem_pop_frame();
    return _t454;
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
        GemVal _t456 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        if (!gem_truthy(gem_eq(gem_table_get(_t456, gem_string("type")), gem_string("NEWLINE")))) break;
#line 102 "compiler/main.gem"
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
#line 112 "compiler/main.gem"
    GemVal _t458 = gem_table_new();
    GemVal gem_v_params = _t458;
#line 113 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 115 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t459 = (*gem_v_peek);
        GemVal _t460 = _t459.fn(_t459.env, NULL, 0);
        GemVal _t463;
        if (gem_truthy(gem_eq(gem_table_get(_t460, gem_string("type")), gem_string("NAME")))) {
                _t463 = gem_eq(gem_table_get(_t460, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t461 = (*gem_v_peek);
                GemVal _t462 = _t461.fn(_t461.env, NULL, 0);
                _t463 = gem_eq(gem_table_get(_t462, gem_string("type")), gem_string("..."));
        }
        if (!gem_truthy(_t463)) break;
#line 116 "compiler/main.gem"
    GemVal _t464 = (*gem_v_peek);
    GemVal _t465 = _t464.fn(_t464.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t465, gem_string("type")), gem_string("...")))) {
#line 117 "compiler/main.gem"
    GemVal _t466 = (*gem_v_advance);
            (void)(_t466.fn(_t466.env, NULL, 0));
#line 118 "compiler/main.gem"
    GemVal _t467[] = {gem_string("NAME")};
    GemVal _t468 = (*gem_v_expect);
    GemVal _t469 = _t468.fn(_t468.env, _t467, 1);
            gem_v_rest_param = gem_table_get(_t469, gem_string("value"));
            break;
        }
#line 121 "compiler/main.gem"
    GemVal _t470 = (*gem_v_advance);
    GemVal _t471 = _t470.fn(_t470.env, NULL, 0);
    GemVal _t472[] = {gem_v_params, gem_table_get(_t471, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t472, 2));
#line 122 "compiler/main.gem"
    GemVal _t473 = (*gem_v_peek);
    GemVal _t474 = _t473.fn(_t473.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t474, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 125 "compiler/main.gem"
    GemVal _t475 = (*gem_v_advance);
        (void)(_t475.fn(_t475.env, NULL, 0));
    }
    GemVal _t476 = gem_table_new();
    gem_table_set(_t476, gem_string("params"), gem_v_params);
    gem_table_set(_t476, gem_string("rest_param"), gem_v_rest_param);
    GemVal _t477 = _t476;
    gem_pop_frame();
    return _t477;
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
#line 144 "compiler/main.gem"
    GemVal _t479 = gem_table_new();
    GemVal gem_v_stmts = _t479;
#line 145 "compiler/main.gem"
    GemVal _t480 = (*gem_v_skip_nl);
    (void)(_t480.fn(_t480.env, NULL, 0));
#line 146 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t481 = (*gem_v_peek);
        GemVal _t482 = _t481.fn(_t481.env, NULL, 0);
        GemVal _t485;
        if (!gem_truthy(gem_neq(gem_table_get(_t482, gem_string("type")), gem_string("end")))) {
                _t485 = gem_neq(gem_table_get(_t482, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t483 = (*gem_v_peek);
                GemVal _t484 = _t483.fn(_t483.env, NULL, 0);
                _t485 = gem_neq(gem_table_get(_t484, gem_string("type")), gem_string("elif"));
        }
        GemVal _t488;
        if (!gem_truthy(_t485)) {
                _t488 = _t485;
        } else {
                GemVal _t486 = (*gem_v_peek);
                GemVal _t487 = _t486.fn(_t486.env, NULL, 0);
                _t488 = gem_neq(gem_table_get(_t487, gem_string("type")), gem_string("else"));
        }
        GemVal _t491;
        if (!gem_truthy(_t488)) {
                _t491 = _t488;
        } else {
                GemVal _t489 = (*gem_v_peek);
                GemVal _t490 = _t489.fn(_t489.env, NULL, 0);
                _t491 = gem_neq(gem_table_get(_t490, gem_string("type")), gem_string("when"));
        }
        GemVal _t494;
        if (!gem_truthy(_t491)) {
                _t494 = _t491;
        } else {
                GemVal _t492 = (*gem_v_peek);
                GemVal _t493 = _t492.fn(_t492.env, NULL, 0);
                _t494 = gem_neq(gem_table_get(_t493, gem_string("type")), gem_string("after"));
        }
        GemVal _t497;
        if (!gem_truthy(_t494)) {
                _t497 = _t494;
        } else {
                GemVal _t495 = (*gem_v_peek);
                GemVal _t496 = _t495.fn(_t495.env, NULL, 0);
                _t497 = gem_neq(gem_table_get(_t496, gem_string("type")), gem_string("then"));
        }
        GemVal _t499;
        if (!gem_truthy(_t497)) {
                _t499 = _t497;
        } else {
                GemVal _t498 = (*gem_v_at_end);
                _t499 = gem_not(_t498.fn(_t498.env, NULL, 0));
        }
        if (!gem_truthy(_t499)) break;
#line 147 "compiler/main.gem"
    GemVal _t500 = (*gem_v_parse_stmt);
    GemVal _t501[] = {gem_v_stmts, _t500.fn(_t500.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t501, 2));
#line 148 "compiler/main.gem"
    GemVal _t502 = (*gem_v_skip_nl);
        (void)(_t502.fn(_t502.env, NULL, 0));
    }
    GemVal _t503 = gem_v_stmts;
    gem_pop_frame();
    return _t503;
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
#line 157 "compiler/main.gem"
    GemVal _t505 = gem_table_new();
    GemVal gem_v_stmts = _t505;
#line 158 "compiler/main.gem"
    GemVal _t506 = (*gem_v_skip_nl);
    (void)(_t506.fn(_t506.env, NULL, 0));
#line 159 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t507 = (*gem_v_peek);
        GemVal _t508 = _t507.fn(_t507.env, NULL, 0);
        GemVal _t510;
        if (!gem_truthy(gem_neq(gem_table_get(_t508, gem_string("type")), gem_string("end")))) {
                _t510 = gem_neq(gem_table_get(_t508, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t509 = (*gem_v_at_end);
                _t510 = gem_not(_t509.fn(_t509.env, NULL, 0));
        }
        if (!gem_truthy(_t510)) break;
#line 160 "compiler/main.gem"
    GemVal _t511 = (*gem_v_parse_stmt);
    GemVal _t512[] = {gem_v_stmts, _t511.fn(_t511.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t512, 2));
#line 161 "compiler/main.gem"
    GemVal _t513 = (*gem_v_skip_nl);
        (void)(_t513.fn(_t513.env, NULL, 0));
    }
    GemVal _t514 = gem_v_stmts;
    gem_pop_frame();
    return _t514;
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
#line 170 "compiler/main.gem"
    GemVal _t516 = (*gem_v_peek);
    GemVal gem_v_t = _t516.fn(_t516.env, NULL, 0);
#line 173 "compiler/main.gem"
    GemVal _t517 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t517, gem_string("type")), gem_string("NUMBER")))) {
#line 174 "compiler/main.gem"
    GemVal _t518 = (*gem_v_advance);
        (void)(_t518.fn(_t518.env, NULL, 0));
#line 175 "compiler/main.gem"
    GemVal _t519 = gem_v_t;
    GemVal _t520[] = {gem_table_get(_t519, gem_string("value"))};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t520, 1))) {
#line 176 "compiler/main.gem"
    GemVal _t521 = gem_v_t;
    GemVal _t522[] = {gem_table_get(_t521, gem_string("value"))};
    GemVal _t523[] = {gem_fn_atof(NULL, _t522, 1)};
    GemVal _t524 = (*gem_v__mod_parser_make_float);
            GemVal _t525 = _t524.fn(_t524.env, _t523, 1);
            gem_pop_frame();
            return _t525;
        }
#line 178 "compiler/main.gem"
    GemVal _t526 = gem_v_t;
    GemVal _t527[] = {gem_table_get(_t526, gem_string("value"))};
    GemVal _t528[] = {gem_fn__mod_parser_str_to_int(NULL, _t527, 1)};
    GemVal _t529 = (*gem_v__mod_parser_make_int);
        GemVal _t530 = _t529.fn(_t529.env, _t528, 1);
        gem_pop_frame();
        return _t530;
    }
#line 182 "compiler/main.gem"
    GemVal _t531 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t531, gem_string("type")), gem_string("STRING")))) {
#line 183 "compiler/main.gem"
    GemVal _t532 = (*gem_v_advance);
        (void)(_t532.fn(_t532.env, NULL, 0));
#line 184 "compiler/main.gem"
    GemVal _t533 = gem_v_t;
    GemVal _t534[] = {gem_table_get(_t533, gem_string("value"))};
    GemVal _t535 = (*gem_v__mod_parser_make_string);
        GemVal _t536 = _t535.fn(_t535.env, _t534, 1);
        gem_pop_frame();
        return _t536;
    }
#line 188 "compiler/main.gem"
    GemVal _t537 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t537, gem_string("type")), gem_string("INTERP_START")))) {
#line 189 "compiler/main.gem"
    GemVal _t538 = (*gem_v_advance);
        (void)(_t538.fn(_t538.env, NULL, 0));
#line 190 "compiler/main.gem"
    GemVal _t539 = gem_table_new();
        GemVal gem_v_parts = _t539;
#line 191 "compiler/main.gem"
    GemVal _t540 = (*gem_v_skip_nl);
        (void)(_t540.fn(_t540.env, NULL, 0));
#line 192 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t541 = (*gem_v_peek);
            GemVal _t542 = _t541.fn(_t541.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t542, gem_string("type")), gem_string("INTERP_END")))) break;
#line 193 "compiler/main.gem"
    GemVal _t543 = (*gem_v_peek);
    GemVal _t544 = _t543.fn(_t543.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t544, gem_string("type")), gem_string("STRING")))) {
#line 194 "compiler/main.gem"
    GemVal _t545 = (*gem_v_advance);
    GemVal _t546 = _t545.fn(_t545.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t546, gem_string("value"));
#line 195 "compiler/main.gem"
    GemVal _t547[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t547, 1), gem_int(0)))) {
#line 196 "compiler/main.gem"
    GemVal _t548[] = {gem_v_sval};
    GemVal _t549 = (*gem_v__mod_parser_make_string);
    GemVal _t550[] = {gem_v_parts, _t549.fn(_t549.env, _t548, 1)};
                    (void)(gem_push_fn(NULL, _t550, 2));
                }
            } else {
#line 199 "compiler/main.gem"
    GemVal _t551 = (*gem_v_parse_expr);
    GemVal _t552[] = {gem_v_parts, _t551.fn(_t551.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t552, 2));
            }
#line 201 "compiler/main.gem"
    GemVal _t553 = (*gem_v_skip_nl);
            (void)(_t553.fn(_t553.env, NULL, 0));
        }
#line 203 "compiler/main.gem"
    GemVal _t554[] = {gem_string("INTERP_END")};
    GemVal _t555 = (*gem_v_expect);
        (void)(_t555.fn(_t555.env, _t554, 1));
#line 204 "compiler/main.gem"
    GemVal _t556[] = {gem_v_parts};
    GemVal _t557 = (*gem_v__mod_parser_make_interp);
        GemVal _t558 = _t557.fn(_t557.env, _t556, 1);
        gem_pop_frame();
        return _t558;
    }
#line 208 "compiler/main.gem"
    GemVal _t559 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t559, gem_string("type")), gem_string("true")))) {
#line 209 "compiler/main.gem"
    GemVal _t560 = (*gem_v_advance);
        (void)(_t560.fn(_t560.env, NULL, 0));
#line 210 "compiler/main.gem"
    GemVal _t561[] = {gem_bool(1)};
    GemVal _t562 = (*gem_v__mod_parser_make_bool);
        GemVal _t563 = _t562.fn(_t562.env, _t561, 1);
        gem_pop_frame();
        return _t563;
    }
#line 212 "compiler/main.gem"
    GemVal _t564 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t564, gem_string("type")), gem_string("false")))) {
#line 213 "compiler/main.gem"
    GemVal _t565 = (*gem_v_advance);
        (void)(_t565.fn(_t565.env, NULL, 0));
#line 214 "compiler/main.gem"
    GemVal _t566[] = {gem_bool(0)};
    GemVal _t567 = (*gem_v__mod_parser_make_bool);
        GemVal _t568 = _t567.fn(_t567.env, _t566, 1);
        gem_pop_frame();
        return _t568;
    }
#line 218 "compiler/main.gem"
    GemVal _t569 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t569, gem_string("type")), gem_string("nil")))) {
#line 219 "compiler/main.gem"
    GemVal _t570 = (*gem_v_advance);
        (void)(_t570.fn(_t570.env, NULL, 0));
#line 220 "compiler/main.gem"
    GemVal _t571 = (*gem_v__mod_parser_make_nil_node);
        GemVal _t572 = _t571.fn(_t571.env, NULL, 0);
        gem_pop_frame();
        return _t572;
    }
#line 224 "compiler/main.gem"
    GemVal _t573 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t573, gem_string("type")), gem_string("fn")))) {
#line 225 "compiler/main.gem"
    GemVal _t574 = (*gem_v_advance);
        (void)(_t574.fn(_t574.env, NULL, 0));
#line 226 "compiler/main.gem"
    GemVal _t575[] = {gem_string("(")};
    GemVal _t576 = (*gem_v_expect);
        (void)(_t576.fn(_t576.env, _t575, 1));
#line 227 "compiler/main.gem"
    GemVal _t577 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t577.fn(_t577.env, NULL, 0);
#line 228 "compiler/main.gem"
    GemVal _t578[] = {gem_string(")")};
    GemVal _t579 = (*gem_v_expect);
        (void)(_t579.fn(_t579.env, _t578, 1));
#line 229 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 230 "compiler/main.gem"
    GemVal _t580 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t580.fn(_t580.env, NULL, 0);
#line 231 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 232 "compiler/main.gem"
    GemVal _t581[] = {gem_string("end")};
    GemVal _t582 = (*gem_v_expect);
        (void)(_t582.fn(_t582.env, _t581, 1));
#line 233 "compiler/main.gem"
    GemVal _t583 = gem_v_pr;
    GemVal _t584 = gem_v_pr;
    GemVal _t585[] = {gem_table_get(_t583, gem_string("params")), gem_table_get(_t584, gem_string("rest_param")), gem_v_body};
    GemVal _t586 = (*gem_v__mod_parser_make_anon_fn);
        GemVal _t587 = _t586.fn(_t586.env, _t585, 3);
        gem_pop_frame();
        return _t587;
    }
#line 238 "compiler/main.gem"
    GemVal _t588 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t588, gem_string("type")), gem_string("{")))) {
#line 239 "compiler/main.gem"
    GemVal _t589 = (*gem_v_advance);
        (void)(_t589.fn(_t589.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t590 = (*gem_v_skip_nl);
        (void)(_t590.fn(_t590.env, NULL, 0));
#line 241 "compiler/main.gem"
    GemVal _t591 = gem_table_new();
        GemVal gem_v_entries = _t591;
#line 242 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t592 = (*gem_v_peek);
            GemVal _t593 = _t592.fn(_t592.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t593, gem_string("type")), gem_string("}")))) break;
#line 243 "compiler/main.gem"
    GemVal _t594 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t594.fn(_t594.env, NULL, 0);
#line 244 "compiler/main.gem"
    GemVal _t595 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t595, gem_string("type")), gem_string("NAME")))) {
#line 245 "compiler/main.gem"
    GemVal _t596 = (*gem_v_advance);
                (void)(_t596.fn(_t596.env, NULL, 0));
            } else {
#line 246 "compiler/main.gem"
    GemVal _t597 = (*gem_v_at_end);
    GemVal _t601;
    if (!gem_truthy(gem_not(_t597.fn(_t597.env, NULL, 0)))) {
        _t601 = gem_not(_t597.fn(_t597.env, NULL, 0));
    } else {
        GemVal _t598[] = {gem_int(1)};
        GemVal _t599 = (*gem_v_peek_at);
        GemVal _t600 = _t599.fn(_t599.env, _t598, 1);
        _t601 = gem_eq(gem_table_get(_t600, gem_string("type")), gem_string(":"));
    }
                if (gem_truthy(_t601)) {
#line 248 "compiler/main.gem"
    GemVal _t602 = (*gem_v_advance);
                    (void)(_t602.fn(_t602.env, NULL, 0));
                } else {
#line 250 "compiler/main.gem"
    GemVal _t603 = gem_v_key_tok;
    GemVal _t604 = gem_v_key_tok;
    GemVal _t605 = gem_v_key_tok;
    GemVal _t606[] = {gem_table_get(_t605, gem_string("value"))};
    GemVal _t607 = gem_v_key_tok;
    GemVal _t608[] = {gem_string("expected '}' or table key but got '"), gem_table_get(_t607, gem_string("type")), gem_string("'")};
    GemVal _t609[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t603, gem_string("line")), gem_table_get(_t604, gem_string("col")), gem_len_fn(NULL, _t606, 1), gem_interp(3, _t608), GEM_NIL};
    GemVal _t610 = (*gem_v__mod_parser_compile_error);
                    (void)(_t610.fn(_t610.env, _t609, 7));
                }
            }
#line 252 "compiler/main.gem"
    GemVal _t611[] = {gem_string(":")};
    GemVal _t612 = (*gem_v_expect);
            (void)(_t612.fn(_t612.env, _t611, 1));
#line 253 "compiler/main.gem"
    GemVal _t613 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t613.fn(_t613.env, NULL, 0);
#line 254 "compiler/main.gem"
    GemVal _t614 = gem_v_key_tok;
    GemVal _t615[] = {gem_table_get(_t614, gem_string("value")), gem_v_val};
    GemVal _t616 = (*gem_v__mod_parser_make_table_entry);
    GemVal _t617[] = {gem_v_entries, _t616.fn(_t616.env, _t615, 2)};
            (void)(gem_push_fn(NULL, _t617, 2));
#line 255 "compiler/main.gem"
    GemVal _t618 = (*gem_v_skip_nl);
            (void)(_t618.fn(_t618.env, NULL, 0));
#line 256 "compiler/main.gem"
    GemVal _t619 = (*gem_v_peek);
    GemVal _t620 = _t619.fn(_t619.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t620, gem_string("type")), gem_string(",")))) {
#line 257 "compiler/main.gem"
    GemVal _t621 = (*gem_v_advance);
                (void)(_t621.fn(_t621.env, NULL, 0));
#line 258 "compiler/main.gem"
    GemVal _t622 = (*gem_v_skip_nl);
                (void)(_t622.fn(_t622.env, NULL, 0));
            }
        }
#line 261 "compiler/main.gem"
    GemVal _t623[] = {gem_string("}")};
    GemVal _t624 = (*gem_v_expect);
        (void)(_t624.fn(_t624.env, _t623, 1));
#line 262 "compiler/main.gem"
    GemVal _t625[] = {gem_v_entries};
    GemVal _t626 = (*gem_v__mod_parser_make_table);
        GemVal _t627 = _t626.fn(_t626.env, _t625, 1);
        gem_pop_frame();
        return _t627;
    }
#line 266 "compiler/main.gem"
    GemVal _t628 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t628, gem_string("type")), gem_string("[")))) {
#line 267 "compiler/main.gem"
    GemVal _t629 = (*gem_v_advance);
        (void)(_t629.fn(_t629.env, NULL, 0));
#line 268 "compiler/main.gem"
    GemVal _t630 = (*gem_v_skip_nl);
        (void)(_t630.fn(_t630.env, NULL, 0));
#line 269 "compiler/main.gem"
    GemVal _t631 = gem_table_new();
        GemVal gem_v_elements = _t631;
#line 270 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t632 = (*gem_v_peek);
            GemVal _t633 = _t632.fn(_t632.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t633, gem_string("type")), gem_string("]")))) break;
#line 271 "compiler/main.gem"
    GemVal _t634 = (*gem_v_parse_expr);
    GemVal _t635[] = {gem_v_elements, _t634.fn(_t634.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t635, 2));
#line 272 "compiler/main.gem"
    GemVal _t636 = (*gem_v_skip_nl);
            (void)(_t636.fn(_t636.env, NULL, 0));
#line 273 "compiler/main.gem"
    GemVal _t637 = (*gem_v_peek);
    GemVal _t638 = _t637.fn(_t637.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t638, gem_string("type")), gem_string(",")))) {
#line 274 "compiler/main.gem"
    GemVal _t639 = (*gem_v_advance);
                (void)(_t639.fn(_t639.env, NULL, 0));
#line 275 "compiler/main.gem"
    GemVal _t640 = (*gem_v_skip_nl);
                (void)(_t640.fn(_t640.env, NULL, 0));
            }
        }
#line 278 "compiler/main.gem"
    GemVal _t641[] = {gem_string("]")};
    GemVal _t642 = (*gem_v_expect);
        (void)(_t642.fn(_t642.env, _t641, 1));
#line 279 "compiler/main.gem"
    GemVal _t643[] = {gem_v_elements};
    GemVal _t644 = (*gem_v__mod_parser_make_array);
        GemVal _t645 = _t644.fn(_t644.env, _t643, 1);
        gem_pop_frame();
        return _t645;
    }
#line 283 "compiler/main.gem"
    GemVal _t646 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t646, gem_string("type")), gem_string("(")))) {
#line 284 "compiler/main.gem"
    GemVal _t647 = (*gem_v_advance);
        (void)(_t647.fn(_t647.env, NULL, 0));
#line 285 "compiler/main.gem"
    GemVal _t648 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t648.fn(_t648.env, NULL, 0);
#line 286 "compiler/main.gem"
    GemVal _t649[] = {gem_string(")")};
    GemVal _t650 = (*gem_v_expect);
        (void)(_t650.fn(_t650.env, _t649, 1));
#line 287 "compiler/main.gem"
        GemVal _t651 = gem_v_e;
        gem_pop_frame();
        return _t651;
    }
#line 291 "compiler/main.gem"
    GemVal _t652 = gem_v_t;
    GemVal _t654;
    if (!gem_truthy(gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("NAME")))) {
        _t654 = gem_eq(gem_table_get(_t652, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t653 = gem_v_t;
        _t654 = gem_eq(gem_table_get(_t653, gem_string("value")), gem_string("pcall"));
    }
    GemVal _t658;
    if (!gem_truthy(_t654)) {
        _t658 = _t654;
    } else {
        GemVal _t655[] = {gem_int(1)};
        GemVal _t656 = (*gem_v_peek_at);
        GemVal _t657 = _t656.fn(_t656.env, _t655, 1);
        _t658 = gem_neq(gem_table_get(_t657, gem_string("type")), gem_string("("));
    }
    if (gem_truthy(_t658)) {
#line 292 "compiler/main.gem"
    GemVal _t659 = (*gem_v_advance);
        (void)(_t659.fn(_t659.env, NULL, 0));
#line 293 "compiler/main.gem"
    GemVal _t660 = (*gem_v_parse_expr);
        GemVal gem_v_expr = _t660.fn(_t660.env, NULL, 0);
#line 294 "compiler/main.gem"
    GemVal _t661[] = {gem_string("pcall")};
    GemVal _t662 = (*gem_v__mod_parser_make_var);
    GemVal _t663 = gem_table_new();
    GemVal _t664 = gem_table_new();
    GemVal _t665 = gem_table_new();
    gem_table_set(_t665, gem_int(0), gem_v_expr);
    GemVal _t666[] = {_t664, GEM_NIL, _t665};
    GemVal _t667 = (*gem_v__mod_parser_make_anon_fn);
    gem_table_set(_t663, gem_int(0), _t667.fn(_t667.env, _t666, 3));
    GemVal _t668 = gem_v_t;
    GemVal _t669[] = {_t662.fn(_t662.env, _t661, 1), _t663, gem_table_get(_t668, gem_string("line"))};
    GemVal _t670 = (*gem_v__mod_parser_make_call);
        GemVal _t671 = _t670.fn(_t670.env, _t669, 3);
        gem_pop_frame();
        return _t671;
    }
#line 298 "compiler/main.gem"
    GemVal _t672 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t672, gem_string("type")), gem_string("NAME")))) {
#line 299 "compiler/main.gem"
    GemVal _t673 = (*gem_v_advance);
        (void)(_t673.fn(_t673.env, NULL, 0));
#line 300 "compiler/main.gem"
    GemVal _t674 = gem_v_t;
    GemVal _t675[] = {gem_table_get(_t674, gem_string("value"))};
    GemVal _t676 = (*gem_v__mod_parser_make_var);
        GemVal _t677 = _t676.fn(_t676.env, _t675, 1);
        gem_pop_frame();
        return _t677;
    }
#line 303 "compiler/main.gem"
    GemVal _t678 = gem_v_t;
    GemVal _t679 = gem_v_t;
    GemVal _t680 = gem_v_t;
    GemVal _t681[] = {gem_table_get(_t680, gem_string("value"))};
    GemVal _t682 = gem_v_t;
    GemVal _t683[] = {gem_string("unexpected token '"), gem_table_get(_t682, gem_string("type")), gem_string("' (expected expression)")};
    GemVal _t684[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t678, gem_string("line")), gem_table_get(_t679, gem_string("col")), gem_len_fn(NULL, _t681, 1), gem_interp(3, _t683), GEM_NIL};
    GemVal _t685 = (*gem_v__mod_parser_compile_error);
    GemVal _t686 = _t685.fn(_t685.env, _t684, 7);
    gem_pop_frame();
    return _t686;
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
#line 308 "compiler/main.gem"
    GemVal _t688 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t688.fn(_t688.env, NULL, 0);
#line 310 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 312 "compiler/main.gem"
    GemVal _t689 = (*gem_v_peek);
    GemVal _t690 = _t689.fn(_t689.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t690, gem_string("type")), gem_string("(")))) {
#line 313 "compiler/main.gem"
    GemVal _t691 = (*gem_v_peek);
    GemVal _t692 = _t691.fn(_t691.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t692, gem_string("line"));
#line 314 "compiler/main.gem"
    GemVal _t693 = (*gem_v_advance);
            (void)(_t693.fn(_t693.env, NULL, 0));
#line 315 "compiler/main.gem"
    GemVal _t694 = (*gem_v_skip_nl);
            (void)(_t694.fn(_t694.env, NULL, 0));
#line 316 "compiler/main.gem"
    GemVal _t695 = gem_table_new();
            GemVal gem_v_args = _t695;
#line 317 "compiler/main.gem"
    GemVal _t696 = (*gem_v_peek);
    GemVal _t697 = _t696.fn(_t696.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t697, gem_string("type")), gem_string(")")))) {
#line 318 "compiler/main.gem"
    GemVal _t698 = (*gem_v_parse_expr);
    GemVal _t699[] = {gem_v_args, _t698.fn(_t698.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t699, 2));
#line 319 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t700 = (*gem_v_peek);
                    GemVal _t701 = _t700.fn(_t700.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t701, gem_string("type")), gem_string(",")))) break;
#line 320 "compiler/main.gem"
    GemVal _t702 = (*gem_v_advance);
                    (void)(_t702.fn(_t702.env, NULL, 0));
#line 321 "compiler/main.gem"
    GemVal _t703 = (*gem_v_skip_nl);
                    (void)(_t703.fn(_t703.env, NULL, 0));
#line 322 "compiler/main.gem"
    GemVal _t704 = (*gem_v_parse_expr);
    GemVal _t705[] = {gem_v_args, _t704.fn(_t704.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t705, 2));
                }
            }
#line 325 "compiler/main.gem"
    GemVal _t706 = (*gem_v_skip_nl);
            (void)(_t706.fn(_t706.env, NULL, 0));
#line 326 "compiler/main.gem"
    GemVal _t707[] = {gem_string(")")};
    GemVal _t708 = (*gem_v_expect);
            (void)(_t708.fn(_t708.env, _t707, 1));
#line 329 "compiler/main.gem"
    GemVal _t709 = (*gem_v_peek);
    GemVal _t710 = _t709.fn(_t709.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t710, gem_string("type")), gem_string("do")))) {
#line 330 "compiler/main.gem"
    GemVal _t711 = (*gem_v_advance);
                (void)(_t711.fn(_t711.env, NULL, 0));
#line 331 "compiler/main.gem"
    GemVal _t712 = gem_table_new();
                GemVal gem_v_bparams = _t712;
#line 332 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 333 "compiler/main.gem"
    GemVal _t713 = (*gem_v_peek);
    GemVal _t714 = _t713.fn(_t713.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t714, gem_string("type")), gem_string("|")))) {
#line 334 "compiler/main.gem"
    GemVal _t715 = (*gem_v_advance);
                    (void)(_t715.fn(_t715.env, NULL, 0));
#line 335 "compiler/main.gem"
    GemVal _t716 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t716.fn(_t716.env, NULL, 0);
#line 336 "compiler/main.gem"
    GemVal _t717 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t717, gem_string("params"));
#line 337 "compiler/main.gem"
    GemVal _t718 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t718, gem_string("rest_param"));
#line 338 "compiler/main.gem"
    GemVal _t719[] = {gem_string("|")};
    GemVal _t720 = (*gem_v_expect);
                    (void)(_t720.fn(_t720.env, _t719, 1));
                }
#line 340 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 341 "compiler/main.gem"
    GemVal _t721 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t721.fn(_t721.env, NULL, 0);
#line 342 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 343 "compiler/main.gem"
    GemVal _t722[] = {gem_string("end")};
    GemVal _t723 = (*gem_v_expect);
                (void)(_t723.fn(_t723.env, _t722, 1));
#line 344 "compiler/main.gem"
    GemVal _t724[] = {gem_v_bparams, gem_v_brest, gem_v_body};
    GemVal _t725 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t726[] = {gem_v_args, _t725.fn(_t725.env, _t724, 3)};
                (void)(gem_push_fn(NULL, _t726, 2));
            } else {
#line 348 "compiler/main.gem"
    GemVal _t727 = (*gem_v_peek);
    GemVal _t728 = _t727.fn(_t727.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t728, gem_string("type")), gem_string("{")))) {
#line 349 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 350 "compiler/main.gem"
    GemVal _t729[] = {gem_int(1)};
    GemVal _t730 = (*gem_v_peek_at);
    GemVal _t731 = _t730.fn(_t730.env, _t729, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t731, gem_string("type")), gem_string("|")))) {
#line 351 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 352 "compiler/main.gem"
    GemVal _t732[] = {gem_int(1)};
    GemVal _t733 = (*gem_v_peek_at);
    GemVal _t734 = _t733.fn(_t733.env, _t732, 1);
    GemVal _t742;
    if (gem_truthy(gem_eq(gem_table_get(_t734, gem_string("type")), gem_string("}")))) {
        _t742 = gem_eq(gem_table_get(_t734, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t735[] = {gem_int(1)};
        GemVal _t736 = (*gem_v_peek_at);
        GemVal _t737 = _t736.fn(_t736.env, _t735, 1);
        GemVal _t741;
        if (!gem_truthy(gem_eq(gem_table_get(_t737, gem_string("type")), gem_string("NAME")))) {
                _t741 = gem_eq(gem_table_get(_t737, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t738[] = {gem_int(2)};
                GemVal _t739 = (*gem_v_peek_at);
                GemVal _t740 = _t739.fn(_t739.env, _t738, 1);
                _t741 = gem_eq(gem_table_get(_t740, gem_string("type")), gem_string(":"));
        }
        _t742 = _t741;
    }
                        if (gem_truthy(_t742)) {
#line 353 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 355 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 357 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 358 "compiler/main.gem"
    GemVal _t743 = (*gem_v_advance);
                        (void)(_t743.fn(_t743.env, NULL, 0));
#line 359 "compiler/main.gem"
    GemVal _t744 = gem_table_new();
                        GemVal gem_v_bparams = _t744;
#line 360 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 361 "compiler/main.gem"
    GemVal _t745 = (*gem_v_peek);
    GemVal _t746 = _t745.fn(_t745.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t746, gem_string("type")), gem_string("|")))) {
#line 362 "compiler/main.gem"
    GemVal _t747 = (*gem_v_advance);
                            (void)(_t747.fn(_t747.env, NULL, 0));
#line 363 "compiler/main.gem"
    GemVal _t748 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t748.fn(_t748.env, NULL, 0);
#line 364 "compiler/main.gem"
    GemVal _t749 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t749, gem_string("params"));
#line 365 "compiler/main.gem"
    GemVal _t750 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t750, gem_string("rest_param"));
#line 366 "compiler/main.gem"
    GemVal _t751[] = {gem_string("|")};
    GemVal _t752 = (*gem_v_expect);
                            (void)(_t752.fn(_t752.env, _t751, 1));
                        }
#line 368 "compiler/main.gem"
    GemVal _t753 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t753.fn(_t753.env, NULL, 0);
#line 369 "compiler/main.gem"
    GemVal _t754[] = {gem_string("}")};
    GemVal _t755 = (*gem_v_expect);
                        (void)(_t755.fn(_t755.env, _t754, 1));
#line 370 "compiler/main.gem"
    GemVal _t756 = gem_table_new();
    gem_table_set(_t756, gem_int(0), gem_v_bexpr);
    GemVal _t757[] = {gem_v_bparams, gem_v_brest, _t756};
    GemVal _t758 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t759[] = {gem_v_args, _t758.fn(_t758.env, _t757, 3)};
                        (void)(gem_push_fn(NULL, _t759, 2));
                    }
                }
            }
#line 375 "compiler/main.gem"
    GemVal _t760[] = {gem_v_node, gem_v_args, gem_v_call_line};
    GemVal _t761 = (*gem_v__mod_parser_make_call);
            gem_v_node = _t761.fn(_t761.env, _t760, 3);
            continue;
        }
#line 380 "compiler/main.gem"
    GemVal _t762 = (*gem_v_peek);
    GemVal _t763 = _t762.fn(_t762.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t763, gem_string("type")), gem_string(".")))) {
#line 381 "compiler/main.gem"
    GemVal _t764 = (*gem_v_advance);
            (void)(_t764.fn(_t764.env, NULL, 0));
#line 382 "compiler/main.gem"
    GemVal _t765 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t765.fn(_t765.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t766 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t766, gem_string("type")), gem_string("NAME")))) {
#line 384 "compiler/main.gem"
    GemVal _t767 = (*gem_v_advance);
                (void)(_t767.fn(_t767.env, NULL, 0));
            } else {
#line 387 "compiler/main.gem"
    GemVal _t768 = (*gem_v_advance);
                (void)(_t768.fn(_t768.env, NULL, 0));
            }
#line 389 "compiler/main.gem"
    GemVal _t769 = gem_v_field_tok;
    GemVal _t770[] = {gem_v_node, gem_table_get(_t769, gem_string("value"))};
    GemVal _t771 = (*gem_v__mod_parser_make_dot);
            gem_v_node = _t771.fn(_t771.env, _t770, 2);
            continue;
        }
#line 394 "compiler/main.gem"
    GemVal _t772 = (*gem_v_peek);
    GemVal _t773 = _t772.fn(_t772.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t773, gem_string("type")), gem_string("[")))) {
#line 395 "compiler/main.gem"
    GemVal _t774 = (*gem_v_advance);
            (void)(_t774.fn(_t774.env, NULL, 0));
#line 396 "compiler/main.gem"
    GemVal _t775 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t775.fn(_t775.env, NULL, 0);
#line 397 "compiler/main.gem"
    GemVal _t776[] = {gem_string("]")};
    GemVal _t777 = (*gem_v_expect);
            (void)(_t777.fn(_t777.env, _t776, 1));
#line 398 "compiler/main.gem"
    GemVal _t778[] = {gem_v_node, gem_v_key};
    GemVal _t779 = (*gem_v__mod_parser_make_index);
            gem_v_node = _t779.fn(_t779.env, _t778, 2);
            continue;
        }
        break;
    }
    GemVal _t780 = gem_v_node;
    gem_pop_frame();
    return _t780;
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
#line 410 "compiler/main.gem"
    GemVal _t782 = (*gem_v_peek);
    GemVal _t783 = _t782.fn(_t782.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t783, gem_string("type")), gem_string("-")))) {
#line 411 "compiler/main.gem"
    GemVal _t784 = (*gem_v_advance);
        (void)(_t784.fn(_t784.env, NULL, 0));
#line 412 "compiler/main.gem"
    GemVal _t785 = (*gem_v_parse_unary);
    GemVal _t786[] = {gem_string("-"), _t785.fn(_t785.env, NULL, 0)};
    GemVal _t787 = (*gem_v__mod_parser_make_unop);
        GemVal _t788 = _t787.fn(_t787.env, _t786, 2);
        gem_pop_frame();
        return _t788;
    }
#line 414 "compiler/main.gem"
    GemVal _t789 = (*gem_v_parse_call);
    GemVal _t790 = _t789.fn(_t789.env, NULL, 0);
    gem_pop_frame();
    return _t790;
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
#line 419 "compiler/main.gem"
    GemVal _t792 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t792.fn(_t792.env, NULL, 0);
#line 420 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t793 = (*gem_v_peek);
        GemVal _t794 = _t793.fn(_t793.env, NULL, 0);
        GemVal _t797;
        if (gem_truthy(gem_eq(gem_table_get(_t794, gem_string("type")), gem_string("*")))) {
                _t797 = gem_eq(gem_table_get(_t794, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t795 = (*gem_v_peek);
                GemVal _t796 = _t795.fn(_t795.env, NULL, 0);
                _t797 = gem_eq(gem_table_get(_t796, gem_string("type")), gem_string("/"));
        }
        GemVal _t800;
        if (gem_truthy(_t797)) {
                _t800 = _t797;
        } else {
                GemVal _t798 = (*gem_v_peek);
                GemVal _t799 = _t798.fn(_t798.env, NULL, 0);
                _t800 = gem_eq(gem_table_get(_t799, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t800)) break;
#line 421 "compiler/main.gem"
    GemVal _t801 = (*gem_v_advance);
    GemVal _t802 = _t801.fn(_t801.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t802, gem_string("type"));
#line 422 "compiler/main.gem"
    GemVal _t803 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t803.fn(_t803.env, NULL, 0);
#line 423 "compiler/main.gem"
    GemVal _t804[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t805 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t805.fn(_t805.env, _t804, 3);
    }
    GemVal _t806 = gem_v_left;
    gem_pop_frame();
    return _t806;
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
#line 430 "compiler/main.gem"
    GemVal _t808 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t808.fn(_t808.env, NULL, 0);
#line 431 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t809 = (*gem_v_peek);
        GemVal _t810 = _t809.fn(_t809.env, NULL, 0);
        GemVal _t813;
        if (gem_truthy(gem_eq(gem_table_get(_t810, gem_string("type")), gem_string("+")))) {
                _t813 = gem_eq(gem_table_get(_t810, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t811 = (*gem_v_peek);
                GemVal _t812 = _t811.fn(_t811.env, NULL, 0);
                _t813 = gem_eq(gem_table_get(_t812, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t813)) break;
#line 432 "compiler/main.gem"
    GemVal _t814 = (*gem_v_advance);
    GemVal _t815 = _t814.fn(_t814.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t815, gem_string("type"));
#line 433 "compiler/main.gem"
    GemVal _t816 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t816.fn(_t816.env, NULL, 0);
#line 434 "compiler/main.gem"
    GemVal _t817[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t818 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t818.fn(_t818.env, _t817, 3);
    }
    GemVal _t819 = gem_v_left;
    gem_pop_frame();
    return _t819;
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
#line 441 "compiler/main.gem"
    GemVal _t821 = (*gem_v_parse_add);
    GemVal gem_v_left = _t821.fn(_t821.env, NULL, 0);
#line 442 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t822 = (*gem_v_peek);
        GemVal _t823 = _t822.fn(_t822.env, NULL, 0);
        GemVal _t826;
        if (gem_truthy(gem_eq(gem_table_get(_t823, gem_string("type")), gem_string("==")))) {
                _t826 = gem_eq(gem_table_get(_t823, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t824 = (*gem_v_peek);
                GemVal _t825 = _t824.fn(_t824.env, NULL, 0);
                _t826 = gem_eq(gem_table_get(_t825, gem_string("type")), gem_string("!="));
        }
        GemVal _t829;
        if (gem_truthy(_t826)) {
                _t829 = _t826;
        } else {
                GemVal _t827 = (*gem_v_peek);
                GemVal _t828 = _t827.fn(_t827.env, NULL, 0);
                _t829 = gem_eq(gem_table_get(_t828, gem_string("type")), gem_string("<"));
        }
        GemVal _t832;
        if (gem_truthy(_t829)) {
                _t832 = _t829;
        } else {
                GemVal _t830 = (*gem_v_peek);
                GemVal _t831 = _t830.fn(_t830.env, NULL, 0);
                _t832 = gem_eq(gem_table_get(_t831, gem_string("type")), gem_string(">"));
        }
        GemVal _t835;
        if (gem_truthy(_t832)) {
                _t835 = _t832;
        } else {
                GemVal _t833 = (*gem_v_peek);
                GemVal _t834 = _t833.fn(_t833.env, NULL, 0);
                _t835 = gem_eq(gem_table_get(_t834, gem_string("type")), gem_string("<="));
        }
        GemVal _t838;
        if (gem_truthy(_t835)) {
                _t838 = _t835;
        } else {
                GemVal _t836 = (*gem_v_peek);
                GemVal _t837 = _t836.fn(_t836.env, NULL, 0);
                _t838 = gem_eq(gem_table_get(_t837, gem_string("type")), gem_string(">="));
        }
        GemVal _t841;
        if (gem_truthy(_t838)) {
                _t841 = _t838;
        } else {
                GemVal _t839 = (*gem_v_peek);
                GemVal _t840 = _t839.fn(_t839.env, NULL, 0);
                _t841 = gem_eq(gem_table_get(_t840, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t841)) break;
#line 443 "compiler/main.gem"
    GemVal _t842 = (*gem_v_advance);
    GemVal _t843 = _t842.fn(_t842.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t843, gem_string("type"));
#line 444 "compiler/main.gem"
    GemVal _t844 = (*gem_v_parse_add);
        GemVal gem_v_right = _t844.fn(_t844.env, NULL, 0);
#line 445 "compiler/main.gem"
    GemVal _t845[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t846 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t846.fn(_t846.env, _t845, 3);
    }
    GemVal _t847 = gem_v_left;
    gem_pop_frame();
    return _t847;
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
#line 452 "compiler/main.gem"
    GemVal _t849 = (*gem_v_peek);
    GemVal _t850 = _t849.fn(_t849.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t850, gem_string("type")), gem_string("not")))) {
#line 453 "compiler/main.gem"
    GemVal _t851 = (*gem_v_advance);
        (void)(_t851.fn(_t851.env, NULL, 0));
#line 454 "compiler/main.gem"
    GemVal _t852 = (*gem_v_parse_not);
    GemVal _t853[] = {gem_string("not"), _t852.fn(_t852.env, NULL, 0)};
    GemVal _t854 = (*gem_v__mod_parser_make_unop);
        GemVal _t855 = _t854.fn(_t854.env, _t853, 2);
        gem_pop_frame();
        return _t855;
    }
#line 456 "compiler/main.gem"
    GemVal _t856 = (*gem_v_parse_compare);
    GemVal _t857 = _t856.fn(_t856.env, NULL, 0);
    gem_pop_frame();
    return _t857;
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
#line 461 "compiler/main.gem"
    GemVal _t859 = (*gem_v_parse_not);
    GemVal gem_v_left = _t859.fn(_t859.env, NULL, 0);
#line 462 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t860 = (*gem_v_peek);
        GemVal _t861 = _t860.fn(_t860.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t861, gem_string("type")), gem_string("and")))) break;
#line 463 "compiler/main.gem"
    GemVal _t862 = (*gem_v_advance);
        (void)(_t862.fn(_t862.env, NULL, 0));
#line 464 "compiler/main.gem"
    GemVal _t863 = (*gem_v_parse_not);
        GemVal gem_v_right = _t863.fn(_t863.env, NULL, 0);
#line 465 "compiler/main.gem"
    GemVal _t864[] = {gem_string("and"), gem_v_left, gem_v_right};
    GemVal _t865 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t865.fn(_t865.env, _t864, 3);
    }
    GemVal _t866 = gem_v_left;
    gem_pop_frame();
    return _t866;
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
#line 472 "compiler/main.gem"
    GemVal _t868 = (*gem_v_parse_and);
    GemVal gem_v_left = _t868.fn(_t868.env, NULL, 0);
#line 473 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t869 = (*gem_v_peek);
        GemVal _t870 = _t869.fn(_t869.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t870, gem_string("type")), gem_string("or")))) break;
#line 474 "compiler/main.gem"
    GemVal _t871 = (*gem_v_advance);
        (void)(_t871.fn(_t871.env, NULL, 0));
#line 475 "compiler/main.gem"
    GemVal _t872 = (*gem_v_parse_and);
        GemVal gem_v_right = _t872.fn(_t872.env, NULL, 0);
#line 476 "compiler/main.gem"
    GemVal _t873[] = {gem_string("or"), gem_v_left, gem_v_right};
    GemVal _t874 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t874.fn(_t874.env, _t873, 3);
    }
    GemVal _t875 = gem_v_left;
    gem_pop_frame();
    return _t875;
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
#line 483 "compiler/main.gem"
    GemVal _t877 = (*gem_v_peek);
    GemVal _t878 = _t877.fn(_t877.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t878, gem_string("line"));
#line 484 "compiler/main.gem"
    GemVal _t879 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t879.fn(_t879.env, NULL, 0);
#line 487 "compiler/main.gem"
    GemVal _t880 = (*gem_v_peek);
    GemVal _t881 = _t880.fn(_t880.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t881, gem_string("type")), gem_string("=")))) {
#line 488 "compiler/main.gem"
    GemVal _t882 = (*gem_v_advance);
        (void)(_t882.fn(_t882.env, NULL, 0));
#line 489 "compiler/main.gem"
    GemVal _t883 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t883.fn(_t883.env, NULL, 0);
#line 490 "compiler/main.gem"
    GemVal _t884 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t884, gem_string("tag")), gem_string("var")))) {
#line 491 "compiler/main.gem"
    GemVal _t885 = gem_v_lhs;
    GemVal _t886[] = {gem_table_get(_t885, gem_string("name")), gem_v_value, gem_v_start_line};
    GemVal _t887 = (*gem_v__mod_parser_make_assign);
            GemVal _t888 = _t887.fn(_t887.env, _t886, 3);
            gem_pop_frame();
            return _t888;
        }
#line 493 "compiler/main.gem"
    GemVal _t889 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t889, gem_string("tag")), gem_string("dot")))) {
#line 494 "compiler/main.gem"
    GemVal _t890 = gem_v_lhs;
    GemVal _t891 = gem_v_lhs;
    GemVal _t892[] = {gem_table_get(_t890, gem_string("object")), gem_table_get(_t891, gem_string("field")), gem_v_value, gem_v_start_line};
    GemVal _t893 = (*gem_v__mod_parser_make_dot_assign);
            GemVal _t894 = _t893.fn(_t893.env, _t892, 4);
            gem_pop_frame();
            return _t894;
        }
#line 496 "compiler/main.gem"
    GemVal _t895 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t895, gem_string("tag")), gem_string("index")))) {
#line 497 "compiler/main.gem"
    GemVal _t896 = gem_v_lhs;
    GemVal _t897 = gem_v_lhs;
    GemVal _t898[] = {gem_table_get(_t896, gem_string("object")), gem_table_get(_t897, gem_string("key")), gem_v_value, gem_v_start_line};
    GemVal _t899 = (*gem_v__mod_parser_make_index_assign);
            GemVal _t900 = _t899.fn(_t899.env, _t898, 4);
            gem_pop_frame();
            return _t900;
        }
#line 499 "compiler/main.gem"
    GemVal _t901[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
    GemVal _t902 = (*gem_v__mod_parser_compile_error);
        (void)(_t902.fn(_t902.env, _t901, 7));
    }
#line 503 "compiler/main.gem"
    GemVal _t903 = (*gem_v_peek);
    GemVal _t904 = _t903.fn(_t903.env, NULL, 0);
    GemVal _t907;
    if (gem_truthy(gem_eq(gem_table_get(_t904, gem_string("type")), gem_string("+=")))) {
        _t907 = gem_eq(gem_table_get(_t904, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t905 = (*gem_v_peek);
        GemVal _t906 = _t905.fn(_t905.env, NULL, 0);
        _t907 = gem_eq(gem_table_get(_t906, gem_string("type")), gem_string("-="));
    }
    GemVal _t910;
    if (gem_truthy(_t907)) {
        _t910 = _t907;
    } else {
        GemVal _t908 = (*gem_v_peek);
        GemVal _t909 = _t908.fn(_t908.env, NULL, 0);
        _t910 = gem_eq(gem_table_get(_t909, gem_string("type")), gem_string("*="));
    }
    GemVal _t913;
    if (gem_truthy(_t910)) {
        _t913 = _t910;
    } else {
        GemVal _t911 = (*gem_v_peek);
        GemVal _t912 = _t911.fn(_t911.env, NULL, 0);
        _t913 = gem_eq(gem_table_get(_t912, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t913)) {
#line 504 "compiler/main.gem"
    GemVal _t914 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t914.fn(_t914.env, NULL, 0);
#line 505 "compiler/main.gem"
    GemVal _t915 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t915, gem_string("value")), gem_int(0));
#line 506 "compiler/main.gem"
    GemVal _t916 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t916.fn(_t916.env, NULL, 0);
#line 507 "compiler/main.gem"
    GemVal _t917 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t917, gem_string("tag")), gem_string("var")))) {
#line 508 "compiler/main.gem"
    GemVal _t918 = gem_v_op_tok;
    GemVal _t919 = gem_v_op_tok;
    GemVal _t920 = gem_v_op_tok;
    GemVal _t921[] = {gem_table_get(_t920, gem_string("value"))};
    GemVal _t922[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t918, gem_string("line")), gem_table_get(_t919, gem_string("col")), gem_len_fn(NULL, _t921, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
    GemVal _t923 = (*gem_v__mod_parser_compile_error);
            (void)(_t923.fn(_t923.env, _t922, 7));
        }
#line 510 "compiler/main.gem"
    GemVal _t924 = gem_v_lhs;
    GemVal _t925 = gem_v_lhs;
    GemVal _t926[] = {gem_table_get(_t925, gem_string("name"))};
    GemVal _t927 = (*gem_v__mod_parser_make_var);
    GemVal _t928[] = {gem_v_base_op, _t927.fn(_t927.env, _t926, 1), gem_v_value};
    GemVal _t929 = (*gem_v__mod_parser_make_binop);
    GemVal _t930[] = {gem_table_get(_t924, gem_string("name")), _t929.fn(_t929.env, _t928, 3), gem_v_start_line};
    GemVal _t931 = (*gem_v__mod_parser_make_assign);
        GemVal _t932 = _t931.fn(_t931.env, _t930, 3);
        gem_pop_frame();
        return _t932;
    }
    GemVal _t933 = gem_v_lhs;
    gem_pop_frame();
    return _t933;
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
#line 523 "compiler/main.gem"
    GemVal _t935[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t935, 1), gem_int(0)))) {
#line 524 "compiler/main.gem"
    GemVal _t936[] = {gem_bool(1)};
    GemVal _t937 = (*gem_v__mod_parser_make_bool);
        GemVal _t938 = _t937.fn(_t937.env, _t936, 1);
        gem_pop_frame();
        return _t938;
    }
#line 526 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 527 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(1);
#line 527 "compiler/main.gem"
    GemVal _t939[] = {gem_v_conditions};
    GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t939, 1);
#line 527 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 527 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_4;
#line 527 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 528 "compiler/main.gem"
    GemVal _t940[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
    GemVal _t941 = (*gem_v__mod_parser_make_binop);
        gem_v_result = _t941.fn(_t941.env, _t940, 3);
    }

    GemVal _t942 = gem_v_result;
    gem_pop_frame();
    return _t942;
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
#line 522 "compiler/main.gem"
    struct _closure__anon_21 *_t943 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t943->gem_v__mod_parser_make_binop = gem_v__mod_parser_make_binop;
    _t943->gem_v__mod_parser_make_bool = gem_v__mod_parser_make_bool;
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, _t943);
#line 532 "compiler/main.gem"
    GemVal _t944 = (*gem_v_peek);
    GemVal gem_v_t = _t944.fn(_t944.env, NULL, 0);
#line 535 "compiler/main.gem"
    GemVal _t945 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t945, gem_string("type")), gem_string("{")))) {
#line 536 "compiler/main.gem"
    GemVal _t946 = (*gem_v_advance);
        (void)(_t946.fn(_t946.env, NULL, 0));
#line 537 "compiler/main.gem"
    GemVal _t947 = (*gem_v_skip_nl);
        (void)(_t947.fn(_t947.env, NULL, 0));
#line 538 "compiler/main.gem"
    GemVal _t948 = gem_table_new();
    GemVal _t949[] = {gem_string("type")};
    GemVal _t950 = (*gem_v__mod_parser_make_var);
    GemVal _t951 = gem_table_new();
    gem_table_set(_t951, gem_int(0), gem_v_target_expr);
    GemVal _t952[] = {_t950.fn(_t950.env, _t949, 1), _t951, gem_int(0)};
    GemVal _t953 = (*gem_v__mod_parser_make_call);
    GemVal _t954[] = {gem_string("table")};
    GemVal _t955 = (*gem_v__mod_parser_make_string);
    GemVal _t956[] = {gem_string("=="), _t953.fn(_t953.env, _t952, 3), _t955.fn(_t955.env, _t954, 1)};
    GemVal _t957 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t948, gem_int(0), _t957.fn(_t957.env, _t956, 3));
        GemVal gem_v_conditions = _t948;
#line 539 "compiler/main.gem"
    GemVal _t958 = gem_table_new();
        GemVal gem_v_bindings = _t958;
#line 540 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t959 = (*gem_v_peek);
            GemVal _t960 = _t959.fn(_t959.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t960, gem_string("type")), gem_string("}")))) break;
#line 541 "compiler/main.gem"
    GemVal _t961 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t961.fn(_t961.env, NULL, 0);
#line 542 "compiler/main.gem"
    GemVal _t962 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t962, gem_string("type")), gem_string("NAME")))) {
#line 543 "compiler/main.gem"
    GemVal _t963 = (*gem_v_advance);
                (void)(_t963.fn(_t963.env, NULL, 0));
            } else {
#line 544 "compiler/main.gem"
    GemVal _t964[] = {gem_int(1)};
    GemVal _t965 = (*gem_v_peek_at);
    GemVal _t966 = _t965.fn(_t965.env, _t964, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t966, gem_string("type")), gem_string(":")))) {
#line 546 "compiler/main.gem"
    GemVal _t967 = (*gem_v_advance);
                    (void)(_t967.fn(_t967.env, NULL, 0));
                } else {
#line 548 "compiler/main.gem"
    GemVal _t968 = gem_v_key_tok;
    GemVal _t969 = gem_v_key_tok;
    GemVal _t970 = gem_v_key_tok;
    GemVal _t971[] = {gem_table_get(_t970, gem_string("value"))};
    GemVal _t972[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t968, gem_string("line")), gem_table_get(_t969, gem_string("col")), gem_len_fn(NULL, _t971, 1), gem_string("expected key name in table pattern"), GEM_NIL};
    GemVal _t973 = (*gem_v__mod_parser_compile_error);
                    (void)(_t973.fn(_t973.env, _t972, 7));
                }
            }
#line 550 "compiler/main.gem"
    GemVal _t974 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t974, gem_string("value"));
#line 551 "compiler/main.gem"
    GemVal _t975[] = {gem_v_key_name};
    GemVal _t976 = (*gem_v__mod_parser_make_string);
            GemVal gem_v_key_str = _t976.fn(_t976.env, _t975, 1);
#line 552 "compiler/main.gem"
    GemVal _t977[] = {gem_string("has_key")};
    GemVal _t978 = (*gem_v__mod_parser_make_var);
    GemVal _t979 = gem_table_new();
    gem_table_set(_t979, gem_int(0), gem_v_target_expr);
    gem_table_set(_t979, gem_int(1), gem_v_key_str);
    GemVal _t980[] = {_t978.fn(_t978.env, _t977, 1), _t979, gem_int(0)};
    GemVal _t981 = (*gem_v__mod_parser_make_call);
    GemVal _t982[] = {gem_v_conditions, _t981.fn(_t981.env, _t980, 3)};
            (void)(gem_push_fn(NULL, _t982, 2));
#line 553 "compiler/main.gem"
    GemVal _t983[] = {gem_v_target_expr, gem_v_key_str};
    GemVal _t984 = (*gem_v__mod_parser_make_index);
            GemVal gem_v_sub_target = _t984.fn(_t984.env, _t983, 2);
#line 554 "compiler/main.gem"
    GemVal _t985 = (*gem_v_peek);
    GemVal _t986 = _t985.fn(_t985.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t986, gem_string("type")), gem_string(":")))) {
#line 555 "compiler/main.gem"
    GemVal _t987 = (*gem_v_advance);
                (void)(_t987.fn(_t987.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t988 = (*gem_v_skip_nl);
                (void)(_t988.fn(_t988.env, NULL, 0));
#line 557 "compiler/main.gem"
    GemVal _t989[] = {gem_v_sub_target};
    GemVal _t990 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t990.fn(_t990.env, _t989, 1);
#line 558 "compiler/main.gem"
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
#line 559 "compiler/main.gem"
    GemVal _t996 = gem_v_sub;
    GemVal _t997[] = {gem_v_conditions, gem_table_get(_t996, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t997, 2));
                }
#line 561 "compiler/main.gem"
    GemVal _t998 = gem_v_sub;
                GemVal gem_v__for_items_5 = gem_table_get(_t998, gem_string("bindings"));
#line 561 "compiler/main.gem"
                GemVal gem_v__for_i_5 = gem_int(0);
#line 561 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t999[] = {gem_v__for_items_5};
                    if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t999, 1)))) break;
#line 561 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 561 "compiler/main.gem"
                    gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 562 "compiler/main.gem"
    GemVal _t1000[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t1000, 2));
                }

            } else {
#line 566 "compiler/main.gem"
    GemVal _t1001[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t1002 = (*gem_v__mod_parser_make_let);
    GemVal _t1003[] = {gem_v_bindings, _t1002.fn(_t1002.env, _t1001, 3)};
                (void)(gem_push_fn(NULL, _t1003, 2));
            }
#line 568 "compiler/main.gem"
    GemVal _t1004 = (*gem_v_skip_nl);
            (void)(_t1004.fn(_t1004.env, NULL, 0));
#line 569 "compiler/main.gem"
    GemVal _t1005 = (*gem_v_peek);
    GemVal _t1006 = _t1005.fn(_t1005.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1006, gem_string("type")), gem_string(",")))) {
#line 570 "compiler/main.gem"
    GemVal _t1007 = (*gem_v_advance);
                (void)(_t1007.fn(_t1007.env, NULL, 0));
#line 571 "compiler/main.gem"
    GemVal _t1008 = (*gem_v_skip_nl);
                (void)(_t1008.fn(_t1008.env, NULL, 0));
            }
        }
#line 574 "compiler/main.gem"
    GemVal _t1009[] = {gem_string("}")};
    GemVal _t1010 = (*gem_v_expect);
        (void)(_t1010.fn(_t1010.env, _t1009, 1));
#line 575 "compiler/main.gem"
    GemVal _t1011 = gem_table_new();
    GemVal _t1012[] = {gem_v_conditions};
    GemVal _t1013 = gem_v_and_chain;
    gem_table_set(_t1011, gem_string("condition"), _t1013.fn(_t1013.env, _t1012, 1));
    gem_table_set(_t1011, gem_string("bindings"), gem_v_bindings);
        GemVal _t1014 = _t1011;
        gem_pop_frame();
        return _t1014;
    }
#line 579 "compiler/main.gem"
    GemVal _t1015 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1015, gem_string("type")), gem_string("[")))) {
#line 580 "compiler/main.gem"
    GemVal _t1016 = (*gem_v_advance);
        (void)(_t1016.fn(_t1016.env, NULL, 0));
#line 581 "compiler/main.gem"
    GemVal _t1017 = (*gem_v_skip_nl);
        (void)(_t1017.fn(_t1017.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t1018 = gem_table_new();
        GemVal gem_v_sub_patterns = _t1018;
#line 583 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 584 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1019 = (*gem_v_peek);
            GemVal _t1020 = _t1019.fn(_t1019.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t1020, gem_string("type")), gem_string("]")))) break;
#line 585 "compiler/main.gem"
    GemVal _t1021[] = {gem_v_idx};
    GemVal _t1022 = (*gem_v__mod_parser_make_int);
    GemVal _t1023[] = {gem_v_target_expr, _t1022.fn(_t1022.env, _t1021, 1)};
    GemVal _t1024 = (*gem_v__mod_parser_make_index);
    GemVal _t1025[] = {_t1024.fn(_t1024.env, _t1023, 2)};
    GemVal _t1026 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t1026.fn(_t1026.env, _t1025, 1);
#line 586 "compiler/main.gem"
    GemVal _t1027[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t1027, 2));
#line 587 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 588 "compiler/main.gem"
    GemVal _t1028 = (*gem_v_skip_nl);
            (void)(_t1028.fn(_t1028.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t1029 = (*gem_v_peek);
    GemVal _t1030 = _t1029.fn(_t1029.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1030, gem_string("type")), gem_string(",")))) {
#line 590 "compiler/main.gem"
    GemVal _t1031 = (*gem_v_advance);
                (void)(_t1031.fn(_t1031.env, NULL, 0));
#line 591 "compiler/main.gem"
    GemVal _t1032 = (*gem_v_skip_nl);
                (void)(_t1032.fn(_t1032.env, NULL, 0));
            }
        }
#line 594 "compiler/main.gem"
    GemVal _t1033[] = {gem_string("]")};
    GemVal _t1034 = (*gem_v_expect);
        (void)(_t1034.fn(_t1034.env, _t1033, 1));
#line 595 "compiler/main.gem"
    GemVal _t1035 = gem_table_new();
    GemVal _t1036[] = {gem_string("type")};
    GemVal _t1037 = (*gem_v__mod_parser_make_var);
    GemVal _t1038 = gem_table_new();
    gem_table_set(_t1038, gem_int(0), gem_v_target_expr);
    GemVal _t1039[] = {_t1037.fn(_t1037.env, _t1036, 1), _t1038, gem_int(0)};
    GemVal _t1040 = (*gem_v__mod_parser_make_call);
    GemVal _t1041[] = {gem_string("table")};
    GemVal _t1042 = (*gem_v__mod_parser_make_string);
    GemVal _t1043[] = {gem_string("=="), _t1040.fn(_t1040.env, _t1039, 3), _t1042.fn(_t1042.env, _t1041, 1)};
    GemVal _t1044 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1035, gem_int(0), _t1044.fn(_t1044.env, _t1043, 3));
        GemVal gem_v_conditions = _t1035;
#line 596 "compiler/main.gem"
    GemVal _t1045[] = {gem_string("len")};
    GemVal _t1046 = (*gem_v__mod_parser_make_var);
    GemVal _t1047 = gem_table_new();
    gem_table_set(_t1047, gem_int(0), gem_v_target_expr);
    GemVal _t1048[] = {_t1046.fn(_t1046.env, _t1045, 1), _t1047, gem_int(0)};
    GemVal _t1049 = (*gem_v__mod_parser_make_call);
    GemVal _t1050[] = {gem_v_idx};
    GemVal _t1051 = (*gem_v__mod_parser_make_int);
    GemVal _t1052[] = {gem_string("=="), _t1049.fn(_t1049.env, _t1048, 3), _t1051.fn(_t1051.env, _t1050, 1)};
    GemVal _t1053 = (*gem_v__mod_parser_make_binop);
    GemVal _t1054[] = {gem_v_conditions, _t1053.fn(_t1053.env, _t1052, 3)};
        (void)(gem_push_fn(NULL, _t1054, 2));
#line 597 "compiler/main.gem"
    GemVal _t1055 = gem_table_new();
        GemVal gem_v_bindings = _t1055;
#line 598 "compiler/main.gem"
        GemVal gem_v__for_i_7 = gem_int(0);
#line 598 "compiler/main.gem"
    GemVal _t1056[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1056, 1);
#line 598 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 598 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_7;
#line 598 "compiler/main.gem"
            gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 599 "compiler/main.gem"
            GemVal gem_v_sub = gem_table_get(gem_v_sub_patterns, gem_v_i);
#line 600 "compiler/main.gem"
    GemVal _t1057 = gem_v_sub;
    GemVal _t1058 = gem_table_get(_t1057, gem_string("condition"));
    GemVal _t1061;
    if (gem_truthy(gem_neq(gem_table_get(_t1058, gem_string("tag")), gem_string("bool")))) {
        _t1061 = gem_neq(gem_table_get(_t1058, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t1059 = gem_v_sub;
        GemVal _t1060 = gem_table_get(_t1059, gem_string("condition"));
        _t1061 = gem_neq(gem_table_get(_t1060, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t1061)) {
#line 601 "compiler/main.gem"
    GemVal _t1062 = gem_v_sub;
    GemVal _t1063[] = {gem_v_conditions, gem_table_get(_t1062, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t1063, 2));
            }
#line 603 "compiler/main.gem"
    GemVal _t1064 = gem_v_sub;
            GemVal gem_v__for_items_6 = gem_table_get(_t1064, gem_string("bindings"));
#line 603 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 603 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1065[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1065, 1)))) break;
#line 603 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 603 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 604 "compiler/main.gem"
    GemVal _t1066[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t1066, 2));
            }

        }

#line 607 "compiler/main.gem"
    GemVal _t1067 = gem_table_new();
    GemVal _t1068[] = {gem_v_conditions};
    GemVal _t1069 = gem_v_and_chain;
    gem_table_set(_t1067, gem_string("condition"), _t1069.fn(_t1069.env, _t1068, 1));
    gem_table_set(_t1067, gem_string("bindings"), gem_v_bindings);
        GemVal _t1070 = _t1067;
        gem_pop_frame();
        return _t1070;
    }
#line 611 "compiler/main.gem"
    GemVal _t1071 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1071, gem_string("type")), gem_string("NUMBER")))) {
#line 612 "compiler/main.gem"
    GemVal _t1072 = (*gem_v_advance);
        (void)(_t1072.fn(_t1072.env, NULL, 0));
#line 613 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 614 "compiler/main.gem"
    GemVal _t1073 = gem_v_t;
    GemVal _t1074[] = {gem_table_get(_t1073, gem_string("value"))};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t1074, 1))) {
#line 615 "compiler/main.gem"
    GemVal _t1075 = gem_v_t;
    GemVal _t1076[] = {gem_table_get(_t1075, gem_string("value"))};
    GemVal _t1077[] = {gem_fn_atof(NULL, _t1076, 1)};
    GemVal _t1078 = (*gem_v__mod_parser_make_float);
            gem_v_lit = _t1078.fn(_t1078.env, _t1077, 1);
        } else {
#line 617 "compiler/main.gem"
    GemVal _t1079 = gem_v_t;
    GemVal _t1080[] = {gem_table_get(_t1079, gem_string("value"))};
    GemVal _t1081[] = {gem_fn__mod_parser_str_to_int(NULL, _t1080, 1)};
    GemVal _t1082 = (*gem_v__mod_parser_make_int);
            gem_v_lit = _t1082.fn(_t1082.env, _t1081, 1);
        }
#line 619 "compiler/main.gem"
    GemVal _t1083 = gem_table_new();
    GemVal _t1084[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    GemVal _t1085 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1083, gem_string("condition"), _t1085.fn(_t1085.env, _t1084, 3));
    GemVal _t1086 = gem_table_new();
    gem_table_set(_t1083, gem_string("bindings"), _t1086);
        GemVal _t1087 = _t1083;
        gem_pop_frame();
        return _t1087;
    }
#line 621 "compiler/main.gem"
    GemVal _t1088 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1088, gem_string("type")), gem_string("STRING")))) {
#line 622 "compiler/main.gem"
    GemVal _t1089 = (*gem_v_advance);
        (void)(_t1089.fn(_t1089.env, NULL, 0));
#line 623 "compiler/main.gem"
    GemVal _t1090 = gem_table_new();
    GemVal _t1091 = gem_v_t;
    GemVal _t1092[] = {gem_table_get(_t1091, gem_string("value"))};
    GemVal _t1093 = (*gem_v__mod_parser_make_string);
    GemVal _t1094[] = {gem_string("=="), gem_v_target_expr, _t1093.fn(_t1093.env, _t1092, 1)};
    GemVal _t1095 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1090, gem_string("condition"), _t1095.fn(_t1095.env, _t1094, 3));
    GemVal _t1096 = gem_table_new();
    gem_table_set(_t1090, gem_string("bindings"), _t1096);
        GemVal _t1097 = _t1090;
        gem_pop_frame();
        return _t1097;
    }
#line 625 "compiler/main.gem"
    GemVal _t1098 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1098, gem_string("type")), gem_string("true")))) {
#line 626 "compiler/main.gem"
    GemVal _t1099 = (*gem_v_advance);
        (void)(_t1099.fn(_t1099.env, NULL, 0));
#line 627 "compiler/main.gem"
    GemVal _t1100 = gem_table_new();
    GemVal _t1101[] = {gem_bool(1)};
    GemVal _t1102 = (*gem_v__mod_parser_make_bool);
    GemVal _t1103[] = {gem_string("=="), gem_v_target_expr, _t1102.fn(_t1102.env, _t1101, 1)};
    GemVal _t1104 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1100, gem_string("condition"), _t1104.fn(_t1104.env, _t1103, 3));
    GemVal _t1105 = gem_table_new();
    gem_table_set(_t1100, gem_string("bindings"), _t1105);
        GemVal _t1106 = _t1100;
        gem_pop_frame();
        return _t1106;
    }
#line 629 "compiler/main.gem"
    GemVal _t1107 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1107, gem_string("type")), gem_string("false")))) {
#line 630 "compiler/main.gem"
    GemVal _t1108 = (*gem_v_advance);
        (void)(_t1108.fn(_t1108.env, NULL, 0));
#line 631 "compiler/main.gem"
    GemVal _t1109 = gem_table_new();
    GemVal _t1110[] = {gem_bool(0)};
    GemVal _t1111 = (*gem_v__mod_parser_make_bool);
    GemVal _t1112[] = {gem_string("=="), gem_v_target_expr, _t1111.fn(_t1111.env, _t1110, 1)};
    GemVal _t1113 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1109, gem_string("condition"), _t1113.fn(_t1113.env, _t1112, 3));
    GemVal _t1114 = gem_table_new();
    gem_table_set(_t1109, gem_string("bindings"), _t1114);
        GemVal _t1115 = _t1109;
        gem_pop_frame();
        return _t1115;
    }
#line 633 "compiler/main.gem"
    GemVal _t1116 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1116, gem_string("type")), gem_string("nil")))) {
#line 634 "compiler/main.gem"
    GemVal _t1117 = (*gem_v_advance);
        (void)(_t1117.fn(_t1117.env, NULL, 0));
#line 635 "compiler/main.gem"
    GemVal _t1118 = gem_table_new();
    GemVal _t1119 = (*gem_v__mod_parser_make_nil_node);
    GemVal _t1120[] = {gem_string("=="), gem_v_target_expr, _t1119.fn(_t1119.env, NULL, 0)};
    GemVal _t1121 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1118, gem_string("condition"), _t1121.fn(_t1121.env, _t1120, 3));
    GemVal _t1122 = gem_table_new();
    gem_table_set(_t1118, gem_string("bindings"), _t1122);
        GemVal _t1123 = _t1118;
        gem_pop_frame();
        return _t1123;
    }
#line 639 "compiler/main.gem"
    GemVal _t1124 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1124, gem_string("type")), gem_string("NAME")))) {
#line 640 "compiler/main.gem"
    GemVal _t1125 = (*gem_v_advance);
        (void)(_t1125.fn(_t1125.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t1126 = gem_table_new();
    GemVal _t1127[] = {gem_bool(1)};
    GemVal _t1128 = (*gem_v__mod_parser_make_bool);
    gem_table_set(_t1126, gem_string("condition"), _t1128.fn(_t1128.env, _t1127, 1));
    GemVal _t1129 = gem_table_new();
    GemVal _t1130 = gem_v_t;
    GemVal _t1131[] = {gem_table_get(_t1130, gem_string("value")), gem_v_target_expr, gem_int(0)};
    GemVal _t1132 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1129, gem_int(0), _t1132.fn(_t1132.env, _t1131, 3));
    gem_table_set(_t1126, gem_string("bindings"), _t1129);
        GemVal _t1133 = _t1126;
        gem_pop_frame();
        return _t1133;
    }
#line 644 "compiler/main.gem"
    GemVal _t1134 = gem_v_t;
    GemVal _t1135 = gem_v_t;
    GemVal _t1136 = gem_v_t;
    GemVal _t1137[] = {gem_table_get(_t1136, gem_string("value"))};
    GemVal _t1138 = gem_v_t;
    GemVal _t1139[] = {gem_string("expected pattern but got '"), gem_table_get(_t1138, gem_string("type")), gem_string("'")};
    GemVal _t1140[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1134, gem_string("line")), gem_table_get(_t1135, gem_string("col")), gem_len_fn(NULL, _t1137, 1), gem_interp(3, _t1139), GEM_NIL};
    GemVal _t1141 = (*gem_v__mod_parser_compile_error);
    GemVal _t1142 = _t1141.fn(_t1141.env, _t1140, 7);
    gem_pop_frame();
    return _t1142;
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
#line 650 "compiler/main.gem"
    GemVal _t1144 = (*gem_v_peek);
    GemVal gem_v_t = _t1144.fn(_t1144.env, NULL, 0);
#line 653 "compiler/main.gem"
    GemVal _t1145 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1145, gem_string("type")), gem_string("let")))) {
#line 654 "compiler/main.gem"
    GemVal _t1146 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1146, gem_string("line"));
#line 655 "compiler/main.gem"
    GemVal _t1147 = (*gem_v_advance);
        (void)(_t1147.fn(_t1147.env, NULL, 0));
#line 658 "compiler/main.gem"
    GemVal _t1148 = (*gem_v_peek);
    GemVal _t1149 = _t1148.fn(_t1148.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1149, gem_string("type")), gem_string("{")))) {
#line 659 "compiler/main.gem"
    GemVal _t1150 = (*gem_v_advance);
            (void)(_t1150.fn(_t1150.env, NULL, 0));
#line 660 "compiler/main.gem"
    GemVal _t1151 = gem_table_new();
            GemVal gem_v_names = _t1151;
#line 661 "compiler/main.gem"
    GemVal _t1152 = (*gem_v_skip_nl);
            (void)(_t1152.fn(_t1152.env, NULL, 0));
#line 662 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1153 = (*gem_v_peek);
                GemVal _t1154 = _t1153.fn(_t1153.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1154, gem_string("type")), gem_string("}")))) break;
#line 663 "compiler/main.gem"
    GemVal _t1155 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1155.fn(_t1155.env, NULL, 0);
#line 664 "compiler/main.gem"
    GemVal _t1156 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t1156, gem_string("type")), gem_string("NAME")))) {
#line 665 "compiler/main.gem"
    GemVal _t1157 = (*gem_v_advance);
                    (void)(_t1157.fn(_t1157.env, NULL, 0));
                } else {
#line 668 "compiler/main.gem"
    GemVal _t1158 = (*gem_v_advance);
                    (void)(_t1158.fn(_t1158.env, NULL, 0));
                }
#line 670 "compiler/main.gem"
    GemVal _t1159 = gem_v_field_tok;
    GemVal _t1160[] = {gem_v_names, gem_table_get(_t1159, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1160, 2));
#line 671 "compiler/main.gem"
    GemVal _t1161 = (*gem_v_skip_nl);
                (void)(_t1161.fn(_t1161.env, NULL, 0));
#line 672 "compiler/main.gem"
    GemVal _t1162 = (*gem_v_peek);
    GemVal _t1163 = _t1162.fn(_t1162.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1163, gem_string("type")), gem_string(",")))) {
#line 673 "compiler/main.gem"
    GemVal _t1164 = (*gem_v_advance);
                    (void)(_t1164.fn(_t1164.env, NULL, 0));
#line 674 "compiler/main.gem"
    GemVal _t1165 = (*gem_v_skip_nl);
                    (void)(_t1165.fn(_t1165.env, NULL, 0));
                }
            }
#line 677 "compiler/main.gem"
    GemVal _t1166[] = {gem_string("}")};
    GemVal _t1167 = (*gem_v_expect);
            (void)(_t1167.fn(_t1167.env, _t1166, 1));
#line 678 "compiler/main.gem"
    GemVal _t1168[] = {gem_string("=")};
    GemVal _t1169 = (*gem_v_expect);
            (void)(_t1169.fn(_t1169.env, _t1168, 1));
#line 679 "compiler/main.gem"
    GemVal _t1170 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1170.fn(_t1170.env, NULL, 0);
#line 680 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 681 "compiler/main.gem"
    GemVal _t1171[] = {gem_string("_d"), (*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_interp(2, _t1171);
#line 682 "compiler/main.gem"
    GemVal _t1172 = gem_table_new();
    GemVal _t1173[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1174 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1172, gem_int(0), _t1174.fn(_t1174.env, _t1173, 3));
            GemVal gem_v_stmts = _t1172;
#line 683 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 683 "compiler/main.gem"
    GemVal _t1175[] = {gem_v_names};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1175, 1);
#line 683 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 683 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_8;
#line 683 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 684 "compiler/main.gem"
    GemVal _t1176[] = {gem_v_tmp};
    GemVal _t1177 = (*gem_v__mod_parser_make_var);
    GemVal _t1178[] = {_t1177.fn(_t1177.env, _t1176, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1179 = (*gem_v__mod_parser_make_dot);
    GemVal _t1180[] = {gem_table_get(gem_v_names, gem_v_di), _t1179.fn(_t1179.env, _t1178, 2), gem_v_line};
    GemVal _t1181 = (*gem_v__mod_parser_make_let);
    GemVal _t1182[] = {gem_v_stmts, _t1181.fn(_t1181.env, _t1180, 3)};
                (void)(gem_push_fn(NULL, _t1182, 2));
            }

#line 686 "compiler/main.gem"
    GemVal _t1183 = gem_table_new();
    gem_table_set(_t1183, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1183, gem_string("stmts"), gem_v_stmts);
            GemVal _t1184 = _t1183;
            gem_pop_frame();
            return _t1184;
        }
#line 690 "compiler/main.gem"
    GemVal _t1185 = (*gem_v_peek);
    GemVal _t1186 = _t1185.fn(_t1185.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1186, gem_string("type")), gem_string("[")))) {
#line 691 "compiler/main.gem"
    GemVal _t1187 = (*gem_v_advance);
            (void)(_t1187.fn(_t1187.env, NULL, 0));
#line 692 "compiler/main.gem"
    GemVal _t1188 = gem_table_new();
            GemVal gem_v_names = _t1188;
#line 693 "compiler/main.gem"
    GemVal _t1189 = (*gem_v_skip_nl);
            (void)(_t1189.fn(_t1189.env, NULL, 0));
#line 694 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1190 = (*gem_v_peek);
                GemVal _t1191 = _t1190.fn(_t1190.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1191, gem_string("type")), gem_string("]")))) break;
#line 695 "compiler/main.gem"
    GemVal _t1192[] = {gem_string("NAME")};
    GemVal _t1193 = (*gem_v_expect);
    GemVal _t1194 = _t1193.fn(_t1193.env, _t1192, 1);
    GemVal _t1195[] = {gem_v_names, gem_table_get(_t1194, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1195, 2));
#line 696 "compiler/main.gem"
    GemVal _t1196 = (*gem_v_skip_nl);
                (void)(_t1196.fn(_t1196.env, NULL, 0));
#line 697 "compiler/main.gem"
    GemVal _t1197 = (*gem_v_peek);
    GemVal _t1198 = _t1197.fn(_t1197.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1198, gem_string("type")), gem_string(",")))) {
#line 698 "compiler/main.gem"
    GemVal _t1199 = (*gem_v_advance);
                    (void)(_t1199.fn(_t1199.env, NULL, 0));
#line 699 "compiler/main.gem"
    GemVal _t1200 = (*gem_v_skip_nl);
                    (void)(_t1200.fn(_t1200.env, NULL, 0));
                }
            }
#line 702 "compiler/main.gem"
    GemVal _t1201[] = {gem_string("]")};
    GemVal _t1202 = (*gem_v_expect);
            (void)(_t1202.fn(_t1202.env, _t1201, 1));
#line 703 "compiler/main.gem"
    GemVal _t1203[] = {gem_string("=")};
    GemVal _t1204 = (*gem_v_expect);
            (void)(_t1204.fn(_t1204.env, _t1203, 1));
#line 704 "compiler/main.gem"
    GemVal _t1205 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1205.fn(_t1205.env, NULL, 0);
#line 705 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 706 "compiler/main.gem"
    GemVal _t1206[] = {gem_string("_d"), (*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_interp(2, _t1206);
#line 707 "compiler/main.gem"
    GemVal _t1207 = gem_table_new();
    GemVal _t1208[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1209 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1207, gem_int(0), _t1209.fn(_t1209.env, _t1208, 3));
            GemVal gem_v_stmts = _t1207;
#line 708 "compiler/main.gem"
            GemVal gem_v__for_i_9 = gem_int(0);
#line 708 "compiler/main.gem"
    GemVal _t1210[] = {gem_v_names};
            GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1210, 1);
#line 708 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 708 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_9;
#line 708 "compiler/main.gem"
                gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 709 "compiler/main.gem"
    GemVal _t1211[] = {gem_v_tmp};
    GemVal _t1212 = (*gem_v__mod_parser_make_var);
    GemVal _t1213[] = {gem_v_di};
    GemVal _t1214 = (*gem_v__mod_parser_make_int);
    GemVal _t1215[] = {_t1212.fn(_t1212.env, _t1211, 1), _t1214.fn(_t1214.env, _t1213, 1)};
    GemVal _t1216 = (*gem_v__mod_parser_make_index);
    GemVal _t1217[] = {gem_table_get(gem_v_names, gem_v_di), _t1216.fn(_t1216.env, _t1215, 2), gem_v_line};
    GemVal _t1218 = (*gem_v__mod_parser_make_let);
    GemVal _t1219[] = {gem_v_stmts, _t1218.fn(_t1218.env, _t1217, 3)};
                (void)(gem_push_fn(NULL, _t1219, 2));
            }

#line 711 "compiler/main.gem"
    GemVal _t1220 = gem_table_new();
    gem_table_set(_t1220, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1220, gem_string("stmts"), gem_v_stmts);
            GemVal _t1221 = _t1220;
            gem_pop_frame();
            return _t1221;
        }
#line 714 "compiler/main.gem"
    GemVal _t1222[] = {gem_string("NAME")};
    GemVal _t1223 = (*gem_v_expect);
    GemVal _t1224 = _t1223.fn(_t1223.env, _t1222, 1);
        GemVal gem_v_name = gem_table_get(_t1224, gem_string("value"));
#line 715 "compiler/main.gem"
    GemVal _t1225[] = {gem_string("=")};
    GemVal _t1226 = (*gem_v_expect);
        (void)(_t1226.fn(_t1226.env, _t1225, 1));
#line 716 "compiler/main.gem"
    GemVal _t1227 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1227.fn(_t1227.env, NULL, 0);
#line 717 "compiler/main.gem"
    GemVal _t1228[] = {gem_v_name, gem_v_value, gem_v_line};
    GemVal _t1229 = (*gem_v__mod_parser_make_let);
        GemVal _t1230 = _t1229.fn(_t1229.env, _t1228, 3);
        gem_pop_frame();
        return _t1230;
    }
#line 721 "compiler/main.gem"
    GemVal _t1231 = gem_v_t;
    GemVal _t1235;
    if (!gem_truthy(gem_eq(gem_table_get(_t1231, gem_string("type")), gem_string("fn")))) {
        _t1235 = gem_eq(gem_table_get(_t1231, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1232[] = {gem_int(1)};
        GemVal _t1233 = (*gem_v_peek_at);
        GemVal _t1234 = _t1233.fn(_t1233.env, _t1232, 1);
        _t1235 = gem_eq(gem_table_get(_t1234, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1235)) {
#line 722 "compiler/main.gem"
    GemVal _t1236 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1236, gem_string("line"));
#line 723 "compiler/main.gem"
    GemVal _t1237 = (*gem_v_advance);
        (void)(_t1237.fn(_t1237.env, NULL, 0));
#line 724 "compiler/main.gem"
    GemVal _t1238[] = {gem_string("NAME")};
    GemVal _t1239 = (*gem_v_expect);
    GemVal _t1240 = _t1239.fn(_t1239.env, _t1238, 1);
        GemVal gem_v_name = gem_table_get(_t1240, gem_string("value"));
#line 725 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 726 "compiler/main.gem"
    GemVal _t1241[] = {gem_v_name};
    GemVal _t1242[] = {gem_string("use: let "), gem_v_name, gem_string(" = fn(...) ... end")};
    GemVal _t1243[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1241, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_interp(3, _t1242)};
    GemVal _t1244 = (*gem_v__mod_parser_compile_error);
            (void)(_t1244.fn(_t1244.env, _t1243, 7));
        }
#line 728 "compiler/main.gem"
    GemVal _t1245[] = {gem_string("(")};
    GemVal _t1246 = (*gem_v_expect);
        (void)(_t1246.fn(_t1246.env, _t1245, 1));
#line 729 "compiler/main.gem"
    GemVal _t1247 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1247.fn(_t1247.env, NULL, 0);
#line 730 "compiler/main.gem"
    GemVal _t1248[] = {gem_string(")")};
    GemVal _t1249 = (*gem_v_expect);
        (void)(_t1249.fn(_t1249.env, _t1248, 1));
#line 731 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 732 "compiler/main.gem"
    GemVal _t1250 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1250.fn(_t1250.env, NULL, 0);
#line 733 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 734 "compiler/main.gem"
    GemVal _t1251[] = {gem_string("end")};
    GemVal _t1252 = (*gem_v_expect);
        (void)(_t1252.fn(_t1252.env, _t1251, 1));
#line 735 "compiler/main.gem"
    GemVal _t1253 = gem_v_pr;
    GemVal _t1254 = gem_v_pr;
    GemVal _t1255[] = {gem_v_name, gem_table_get(_t1253, gem_string("params")), gem_table_get(_t1254, gem_string("rest_param")), gem_v_body, gem_v_line};
    GemVal _t1256 = (*gem_v__mod_parser_make_fn_def);
        GemVal _t1257 = _t1256.fn(_t1256.env, _t1255, 5);
        gem_pop_frame();
        return _t1257;
    }
#line 739 "compiler/main.gem"
    GemVal _t1258 = gem_v_t;
    GemVal _t1260;
    if (gem_truthy(gem_eq(gem_table_get(_t1258, gem_string("type")), gem_string("if")))) {
        _t1260 = gem_eq(gem_table_get(_t1258, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1259 = gem_v_t;
        _t1260 = gem_eq(gem_table_get(_t1259, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1260)) {
#line 740 "compiler/main.gem"
    GemVal _t1261 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1261, gem_string("line"));
#line 741 "compiler/main.gem"
    GemVal _t1262 = (*gem_v_advance);
        (void)(_t1262.fn(_t1262.env, NULL, 0));
#line 742 "compiler/main.gem"
    GemVal _t1263 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1263.fn(_t1263.env, NULL, 0);
#line 743 "compiler/main.gem"
    GemVal _t1264 = (*gem_v_peek);
    GemVal _t1265 = _t1264.fn(_t1264.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1265, gem_string("type")), gem_string("then")))) {
#line 744 "compiler/main.gem"
    GemVal _t1266 = (*gem_v_advance);
            (void)(_t1266.fn(_t1266.env, NULL, 0));
        }
#line 746 "compiler/main.gem"
    GemVal _t1267 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1267.fn(_t1267.env, NULL, 0);
#line 747 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 748 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_peek);
    GemVal _t1269 = _t1268.fn(_t1268.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1269, gem_string("type")), gem_string("elif")))) {
#line 750 "compiler/main.gem"
    GemVal _t1270 = gem_table_new();
    GemVal _t1271 = (*gem_v_parse_stmt);
    gem_table_set(_t1270, gem_int(0), _t1271.fn(_t1271.env, NULL, 0));
            gem_v_else_body = _t1270;
        } else {
#line 751 "compiler/main.gem"
    GemVal _t1272 = (*gem_v_peek);
    GemVal _t1273 = _t1272.fn(_t1272.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1273, gem_string("type")), gem_string("else")))) {
#line 752 "compiler/main.gem"
    GemVal _t1274 = (*gem_v_advance);
                (void)(_t1274.fn(_t1274.env, NULL, 0));
#line 753 "compiler/main.gem"
    GemVal _t1275 = (*gem_v_parse_body);
                gem_v_else_body = _t1275.fn(_t1275.env, NULL, 0);
            }
        }
#line 756 "compiler/main.gem"
    GemVal _t1276 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1276, gem_string("type")), gem_string("if")))) {
#line 757 "compiler/main.gem"
    GemVal _t1277[] = {gem_string("end")};
    GemVal _t1278 = (*gem_v_expect);
            (void)(_t1278.fn(_t1278.env, _t1277, 1));
        }
#line 759 "compiler/main.gem"
    GemVal _t1279[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
    GemVal _t1280 = (*gem_v__mod_parser_make_if);
        GemVal _t1281 = _t1280.fn(_t1280.env, _t1279, 4);
        gem_pop_frame();
        return _t1281;
    }
#line 763 "compiler/main.gem"
    GemVal _t1282 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1282, gem_string("type")), gem_string("while")))) {
#line 764 "compiler/main.gem"
    GemVal _t1283 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1283, gem_string("line"));
#line 765 "compiler/main.gem"
    GemVal _t1284 = (*gem_v_advance);
        (void)(_t1284.fn(_t1284.env, NULL, 0));
#line 766 "compiler/main.gem"
    GemVal _t1285 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1285.fn(_t1285.env, NULL, 0);
#line 767 "compiler/main.gem"
    GemVal _t1286 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1286.fn(_t1286.env, NULL, 0);
#line 768 "compiler/main.gem"
    GemVal _t1287[] = {gem_string("end")};
    GemVal _t1288 = (*gem_v_expect);
        (void)(_t1288.fn(_t1288.env, _t1287, 1));
#line 769 "compiler/main.gem"
    GemVal _t1289[] = {gem_v_cond, gem_v_wbody, gem_v_line};
    GemVal _t1290 = (*gem_v__mod_parser_make_while);
        GemVal _t1291 = _t1290.fn(_t1290.env, _t1289, 3);
        gem_pop_frame();
        return _t1291;
    }
#line 773 "compiler/main.gem"
    GemVal _t1292 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1292, gem_string("type")), gem_string("for")))) {
#line 774 "compiler/main.gem"
    GemVal _t1293 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1293, gem_string("line"));
#line 775 "compiler/main.gem"
    GemVal _t1294 = (*gem_v_advance);
        (void)(_t1294.fn(_t1294.env, NULL, 0));
#line 776 "compiler/main.gem"
    GemVal _t1295[] = {gem_string("NAME")};
    GemVal _t1296 = (*gem_v_expect);
    GemVal _t1297 = _t1296.fn(_t1296.env, _t1295, 1);
        GemVal gem_v_var_name = gem_table_get(_t1297, gem_string("value"));
#line 778 "compiler/main.gem"
    GemVal _t1298 = (*gem_v_peek);
    GemVal _t1299 = _t1298.fn(_t1298.env, NULL, 0);
    GemVal _t1303;
    if (!gem_truthy(gem_eq(gem_table_get(_t1299, gem_string("type")), gem_string(",")))) {
        _t1303 = gem_eq(gem_table_get(_t1299, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1300[] = {gem_int(1)};
        GemVal _t1301 = (*gem_v_peek_at);
        GemVal _t1302 = _t1301.fn(_t1301.env, _t1300, 1);
        _t1303 = gem_eq(gem_table_get(_t1302, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1303)) {
#line 780 "compiler/main.gem"
    GemVal _t1304 = (*gem_v_advance);
            (void)(_t1304.fn(_t1304.env, NULL, 0));
#line 781 "compiler/main.gem"
    GemVal _t1305[] = {gem_string("NAME")};
    GemVal _t1306 = (*gem_v_expect);
    GemVal _t1307 = _t1306.fn(_t1306.env, _t1305, 1);
            GemVal gem_v_val_name = gem_table_get(_t1307, gem_string("value"));
#line 782 "compiler/main.gem"
    GemVal _t1308[] = {gem_string("in")};
    GemVal _t1309 = (*gem_v_expect);
            (void)(_t1309.fn(_t1309.env, _t1308, 1));
#line 783 "compiler/main.gem"
    GemVal _t1310 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1310.fn(_t1310.env, NULL, 0);
#line 784 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 785 "compiler/main.gem"
    GemVal _t1311[] = {gem_string("_for_tbl_"), (*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_interp(2, _t1311);
#line 786 "compiler/main.gem"
    GemVal _t1312[] = {gem_string("_for_len_"), (*gem_v_gensym_counter)};
            GemVal gem_v_len_var = gem_interp(2, _t1312);
#line 787 "compiler/main.gem"
    GemVal _t1313[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_interp(2, _t1313);
#line 788 "compiler/main.gem"
    GemVal _t1314 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1314.fn(_t1314.env, NULL, 0);
#line 789 "compiler/main.gem"
    GemVal _t1315[] = {gem_string("end")};
    GemVal _t1316 = (*gem_v_expect);
            (void)(_t1316.fn(_t1316.env, _t1315, 1));
#line 790 "compiler/main.gem"
    GemVal _t1317 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1317;
#line 791 "compiler/main.gem"
    GemVal _t1318[] = {gem_string("__table_key_at")};
    GemVal _t1319 = (*gem_v__mod_parser_make_var);
    GemVal _t1320 = gem_table_new();
    GemVal _t1321[] = {gem_v_tbl_var};
    GemVal _t1322 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1320, gem_int(0), _t1322.fn(_t1322.env, _t1321, 1));
    GemVal _t1323[] = {gem_v_idx_var};
    GemVal _t1324 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1320, gem_int(1), _t1324.fn(_t1324.env, _t1323, 1));
    GemVal _t1325[] = {_t1319.fn(_t1319.env, _t1318, 1), _t1320, gem_int(0)};
    GemVal _t1326 = (*gem_v__mod_parser_make_call);
    GemVal _t1327[] = {gem_v_var_name, _t1326.fn(_t1326.env, _t1325, 3), gem_v_line};
    GemVal _t1328 = (*gem_v__mod_parser_make_let);
    GemVal _t1329[] = {gem_v_inner_stmts, _t1328.fn(_t1328.env, _t1327, 3)};
            (void)(gem_push_fn(NULL, _t1329, 2));
#line 792 "compiler/main.gem"
    GemVal _t1330[] = {gem_string("__table_val_at")};
    GemVal _t1331 = (*gem_v__mod_parser_make_var);
    GemVal _t1332 = gem_table_new();
    GemVal _t1333[] = {gem_v_tbl_var};
    GemVal _t1334 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1332, gem_int(0), _t1334.fn(_t1334.env, _t1333, 1));
    GemVal _t1335[] = {gem_v_idx_var};
    GemVal _t1336 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1332, gem_int(1), _t1336.fn(_t1336.env, _t1335, 1));
    GemVal _t1337[] = {_t1331.fn(_t1331.env, _t1330, 1), _t1332, gem_int(0)};
    GemVal _t1338 = (*gem_v__mod_parser_make_call);
    GemVal _t1339[] = {gem_v_val_name, _t1338.fn(_t1338.env, _t1337, 3), gem_v_line};
    GemVal _t1340 = (*gem_v__mod_parser_make_let);
    GemVal _t1341[] = {gem_v_inner_stmts, _t1340.fn(_t1340.env, _t1339, 3)};
            (void)(gem_push_fn(NULL, _t1341, 2));
#line 793 "compiler/main.gem"
    GemVal _t1342[] = {gem_v_idx_var};
    GemVal _t1343 = (*gem_v__mod_parser_make_var);
    GemVal _t1344[] = {gem_int(1)};
    GemVal _t1345 = (*gem_v__mod_parser_make_int);
    GemVal _t1346[] = {gem_string("+"), _t1343.fn(_t1343.env, _t1342, 1), _t1345.fn(_t1345.env, _t1344, 1)};
    GemVal _t1347 = (*gem_v__mod_parser_make_binop);
    GemVal _t1348[] = {gem_v_idx_var, _t1347.fn(_t1347.env, _t1346, 3), gem_v_line};
    GemVal _t1349 = (*gem_v__mod_parser_make_assign);
    GemVal _t1350[] = {gem_v_inner_stmts, _t1349.fn(_t1349.env, _t1348, 3)};
            (void)(gem_push_fn(NULL, _t1350, 2));
#line 794 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 794 "compiler/main.gem"
    GemVal _t1351[] = {gem_v_fbody};
            GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1351, 1);
#line 794 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 794 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_10;
#line 794 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 795 "compiler/main.gem"
    GemVal _t1352[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1352, 2));
            }

#line 797 "compiler/main.gem"
    GemVal _t1353[] = {gem_v_idx_var};
    GemVal _t1354 = (*gem_v__mod_parser_make_var);
    GemVal _t1355[] = {gem_v_len_var};
    GemVal _t1356 = (*gem_v__mod_parser_make_var);
    GemVal _t1357[] = {gem_string("<"), _t1354.fn(_t1354.env, _t1353, 1), _t1356.fn(_t1356.env, _t1355, 1)};
    GemVal _t1358 = (*gem_v__mod_parser_make_binop);
    GemVal _t1359[] = {_t1358.fn(_t1358.env, _t1357, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1360 = (*gem_v__mod_parser_make_while);
            GemVal gem_v_while_node = _t1360.fn(_t1360.env, _t1359, 3);
#line 802 "compiler/main.gem"
    GemVal _t1361 = gem_table_new();
    gem_table_set(_t1361, gem_string("tag"), gem_string("block"));
    GemVal _t1362 = gem_table_new();
    GemVal _t1363[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    GemVal _t1364 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1362, gem_int(0), _t1364.fn(_t1364.env, _t1363, 3));
    GemVal _t1365[] = {gem_string("len")};
    GemVal _t1366 = (*gem_v__mod_parser_make_var);
    GemVal _t1367 = gem_table_new();
    GemVal _t1368[] = {gem_v_tbl_var};
    GemVal _t1369 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1367, gem_int(0), _t1369.fn(_t1369.env, _t1368, 1));
    GemVal _t1370[] = {_t1366.fn(_t1366.env, _t1365, 1), _t1367, gem_int(0)};
    GemVal _t1371 = (*gem_v__mod_parser_make_call);
    GemVal _t1372[] = {gem_v_len_var, _t1371.fn(_t1371.env, _t1370, 3), gem_v_line};
    GemVal _t1373 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1362, gem_int(1), _t1373.fn(_t1373.env, _t1372, 3));
    GemVal _t1374[] = {gem_int(0)};
    GemVal _t1375 = (*gem_v__mod_parser_make_int);
    GemVal _t1376[] = {gem_v_idx_var, _t1375.fn(_t1375.env, _t1374, 1), gem_v_line};
    GemVal _t1377 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1362, gem_int(2), _t1377.fn(_t1377.env, _t1376, 3));
    gem_table_set(_t1362, gem_int(3), gem_v_while_node);
    gem_table_set(_t1361, gem_string("stmts"), _t1362);
            GemVal _t1378 = _t1361;
            gem_pop_frame();
            return _t1378;
        } else {
#line 808 "compiler/main.gem"
    GemVal _t1379 = (*gem_v_peek);
    GemVal _t1380 = _t1379.fn(_t1379.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1380, gem_string("type")), gem_string("in")))) {
#line 810 "compiler/main.gem"
    GemVal _t1381 = (*gem_v_advance);
                (void)(_t1381.fn(_t1381.env, NULL, 0));
#line 811 "compiler/main.gem"
    GemVal _t1382 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1382.fn(_t1382.env, NULL, 0);
#line 812 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 813 "compiler/main.gem"
    GemVal _t1383[] = {gem_string("_for_items_"), (*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_interp(2, _t1383);
#line 814 "compiler/main.gem"
    GemVal _t1384[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_interp(2, _t1384);
#line 815 "compiler/main.gem"
    GemVal _t1385 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1385.fn(_t1385.env, NULL, 0);
#line 816 "compiler/main.gem"
    GemVal _t1386[] = {gem_string("end")};
    GemVal _t1387 = (*gem_v_expect);
                (void)(_t1387.fn(_t1387.env, _t1386, 1));
#line 817 "compiler/main.gem"
    GemVal _t1388 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1388;
#line 818 "compiler/main.gem"
    GemVal _t1389[] = {gem_v_items_var};
    GemVal _t1390 = (*gem_v__mod_parser_make_var);
    GemVal _t1391[] = {gem_v_idx_var};
    GemVal _t1392 = (*gem_v__mod_parser_make_var);
    GemVal _t1393[] = {_t1390.fn(_t1390.env, _t1389, 1), _t1392.fn(_t1392.env, _t1391, 1)};
    GemVal _t1394 = (*gem_v__mod_parser_make_index);
    GemVal _t1395[] = {gem_v_var_name, _t1394.fn(_t1394.env, _t1393, 2), gem_v_line};
    GemVal _t1396 = (*gem_v__mod_parser_make_let);
    GemVal _t1397[] = {gem_v_inner_stmts, _t1396.fn(_t1396.env, _t1395, 3)};
                (void)(gem_push_fn(NULL, _t1397, 2));
#line 819 "compiler/main.gem"
    GemVal _t1398[] = {gem_v_idx_var};
    GemVal _t1399 = (*gem_v__mod_parser_make_var);
    GemVal _t1400[] = {gem_int(1)};
    GemVal _t1401 = (*gem_v__mod_parser_make_int);
    GemVal _t1402[] = {gem_string("+"), _t1399.fn(_t1399.env, _t1398, 1), _t1401.fn(_t1401.env, _t1400, 1)};
    GemVal _t1403 = (*gem_v__mod_parser_make_binop);
    GemVal _t1404[] = {gem_v_idx_var, _t1403.fn(_t1403.env, _t1402, 3), gem_v_line};
    GemVal _t1405 = (*gem_v__mod_parser_make_assign);
    GemVal _t1406[] = {gem_v_inner_stmts, _t1405.fn(_t1405.env, _t1404, 3)};
                (void)(gem_push_fn(NULL, _t1406, 2));
#line 820 "compiler/main.gem"
                GemVal gem_v__for_i_11 = gem_int(0);
#line 820 "compiler/main.gem"
    GemVal _t1407[] = {gem_v_fbody};
                GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1407, 1);
#line 820 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 820 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_11;
#line 820 "compiler/main.gem"
                    gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 821 "compiler/main.gem"
    GemVal _t1408[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1408, 2));
                }

#line 823 "compiler/main.gem"
    GemVal _t1409[] = {gem_v_idx_var};
    GemVal _t1410 = (*gem_v__mod_parser_make_var);
    GemVal _t1411[] = {gem_string("len")};
    GemVal _t1412 = (*gem_v__mod_parser_make_var);
    GemVal _t1413 = gem_table_new();
    GemVal _t1414[] = {gem_v_items_var};
    GemVal _t1415 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1413, gem_int(0), _t1415.fn(_t1415.env, _t1414, 1));
    GemVal _t1416[] = {_t1412.fn(_t1412.env, _t1411, 1), _t1413, gem_int(0)};
    GemVal _t1417 = (*gem_v__mod_parser_make_call);
    GemVal _t1418[] = {gem_string("<"), _t1410.fn(_t1410.env, _t1409, 1), _t1417.fn(_t1417.env, _t1416, 3)};
    GemVal _t1419 = (*gem_v__mod_parser_make_binop);
    GemVal _t1420[] = {_t1419.fn(_t1419.env, _t1418, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1421 = (*gem_v__mod_parser_make_while);
                GemVal gem_v_while_node = _t1421.fn(_t1421.env, _t1420, 3);
#line 828 "compiler/main.gem"
    GemVal _t1422 = gem_table_new();
    gem_table_set(_t1422, gem_string("tag"), gem_string("block"));
    GemVal _t1423 = gem_table_new();
    GemVal _t1424[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    GemVal _t1425 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1423, gem_int(0), _t1425.fn(_t1425.env, _t1424, 3));
    GemVal _t1426[] = {gem_int(0)};
    GemVal _t1427 = (*gem_v__mod_parser_make_int);
    GemVal _t1428[] = {gem_v_idx_var, _t1427.fn(_t1427.env, _t1426, 1), gem_v_line};
    GemVal _t1429 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1423, gem_int(1), _t1429.fn(_t1429.env, _t1428, 3));
    gem_table_set(_t1423, gem_int(2), gem_v_while_node);
    gem_table_set(_t1422, gem_string("stmts"), _t1423);
                GemVal _t1430 = _t1422;
                gem_pop_frame();
                return _t1430;
            } else {
#line 833 "compiler/main.gem"
    GemVal _t1431 = (*gem_v_peek);
    GemVal _t1432 = _t1431.fn(_t1431.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1432, gem_string("type")), gem_string("=")))) {
#line 835 "compiler/main.gem"
    GemVal _t1433 = (*gem_v_advance);
                    (void)(_t1433.fn(_t1433.env, NULL, 0));
#line 836 "compiler/main.gem"
    GemVal _t1434 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1434.fn(_t1434.env, NULL, 0);
#line 837 "compiler/main.gem"
    GemVal _t1435[] = {gem_string(",")};
    GemVal _t1436 = (*gem_v_expect);
                    (void)(_t1436.fn(_t1436.env, _t1435, 1));
#line 838 "compiler/main.gem"
    GemVal _t1437 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1437.fn(_t1437.env, NULL, 0);
#line 839 "compiler/main.gem"
    GemVal _t1438 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1438.fn(_t1438.env, NULL, 0);
#line 840 "compiler/main.gem"
    GemVal _t1439[] = {gem_string("end")};
    GemVal _t1440 = (*gem_v_expect);
                    (void)(_t1440.fn(_t1440.env, _t1439, 1));
#line 841 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 842 "compiler/main.gem"
    GemVal _t1441[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_interp(2, _t1441);
#line 843 "compiler/main.gem"
    GemVal _t1442[] = {gem_string("_for_limit_"), (*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_interp(2, _t1442);
#line 844 "compiler/main.gem"
    GemVal _t1443 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1443;
#line 845 "compiler/main.gem"
    GemVal _t1444[] = {gem_v_idx_var};
    GemVal _t1445 = (*gem_v__mod_parser_make_var);
    GemVal _t1446[] = {gem_v_var_name, _t1445.fn(_t1445.env, _t1444, 1), gem_v_line};
    GemVal _t1447 = (*gem_v__mod_parser_make_let);
    GemVal _t1448[] = {gem_v_inner_stmts, _t1447.fn(_t1447.env, _t1446, 3)};
                    (void)(gem_push_fn(NULL, _t1448, 2));
#line 846 "compiler/main.gem"
    GemVal _t1449[] = {gem_v_idx_var};
    GemVal _t1450 = (*gem_v__mod_parser_make_var);
    GemVal _t1451[] = {gem_int(1)};
    GemVal _t1452 = (*gem_v__mod_parser_make_int);
    GemVal _t1453[] = {gem_string("+"), _t1450.fn(_t1450.env, _t1449, 1), _t1452.fn(_t1452.env, _t1451, 1)};
    GemVal _t1454 = (*gem_v__mod_parser_make_binop);
    GemVal _t1455[] = {gem_v_idx_var, _t1454.fn(_t1454.env, _t1453, 3), gem_v_line};
    GemVal _t1456 = (*gem_v__mod_parser_make_assign);
    GemVal _t1457[] = {gem_v_inner_stmts, _t1456.fn(_t1456.env, _t1455, 3)};
                    (void)(gem_push_fn(NULL, _t1457, 2));
#line 847 "compiler/main.gem"
                    GemVal gem_v__for_i_12 = gem_int(0);
#line 847 "compiler/main.gem"
    GemVal _t1458[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1458, 1);
#line 847 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 847 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_12;
#line 847 "compiler/main.gem"
                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 848 "compiler/main.gem"
    GemVal _t1459[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1459, 2));
                    }

#line 850 "compiler/main.gem"
    GemVal _t1460[] = {gem_v_idx_var};
    GemVal _t1461 = (*gem_v__mod_parser_make_var);
    GemVal _t1462[] = {gem_v_limit_var};
    GemVal _t1463 = (*gem_v__mod_parser_make_var);
    GemVal _t1464[] = {gem_string("<"), _t1461.fn(_t1461.env, _t1460, 1), _t1463.fn(_t1463.env, _t1462, 1)};
    GemVal _t1465 = (*gem_v__mod_parser_make_binop);
    GemVal _t1466[] = {_t1465.fn(_t1465.env, _t1464, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1467 = (*gem_v__mod_parser_make_while);
                    GemVal gem_v_while_node = _t1467.fn(_t1467.env, _t1466, 3);
#line 855 "compiler/main.gem"
    GemVal _t1468 = gem_table_new();
    gem_table_set(_t1468, gem_string("tag"), gem_string("block"));
    GemVal _t1469 = gem_table_new();
    GemVal _t1470[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    GemVal _t1471 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1469, gem_int(0), _t1471.fn(_t1471.env, _t1470, 3));
    GemVal _t1472[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    GemVal _t1473 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1469, gem_int(1), _t1473.fn(_t1473.env, _t1472, 3));
    gem_table_set(_t1469, gem_int(2), gem_v_while_node);
    gem_table_set(_t1468, gem_string("stmts"), _t1469);
                    GemVal _t1474 = _t1468;
                    gem_pop_frame();
                    return _t1474;
                } else {
#line 861 "compiler/main.gem"
    GemVal _t1475 = (*gem_v_peek);
    GemVal _t1476 = _t1475.fn(_t1475.env, NULL, 0);
    GemVal _t1477 = (*gem_v_peek);
    GemVal _t1478 = _t1477.fn(_t1477.env, NULL, 0);
    GemVal _t1479 = (*gem_v_peek);
    GemVal _t1480 = _t1479.fn(_t1479.env, NULL, 0);
    GemVal _t1481[] = {gem_table_get(_t1480, gem_string("value"))};
    GemVal _t1482[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1476, gem_string("line")), gem_table_get(_t1478, gem_string("col")), gem_len_fn(NULL, _t1481, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
    GemVal _t1483 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1483.fn(_t1483.env, _t1482, 7));
                }
            }
        }
    }
#line 866 "compiler/main.gem"
    GemVal _t1484 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1484, gem_string("type")), gem_string("match")))) {
#line 867 "compiler/main.gem"
    GemVal _t1485 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1485, gem_string("line"));
#line 868 "compiler/main.gem"
    GemVal _t1486 = (*gem_v_advance);
        (void)(_t1486.fn(_t1486.env, NULL, 0));
#line 869 "compiler/main.gem"
    GemVal _t1487 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1487.fn(_t1487.env, NULL, 0);
#line 870 "compiler/main.gem"
    GemVal _t1488 = (*gem_v_skip_nl);
        (void)(_t1488.fn(_t1488.env, NULL, 0));
#line 871 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 872 "compiler/main.gem"
    GemVal _t1489[] = {gem_string("_match_"), (*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_interp(2, _t1489);
#line 873 "compiler/main.gem"
    GemVal _t1490[] = {gem_v_match_var};
    GemVal _t1491 = (*gem_v__mod_parser_make_var);
        GemVal gem_v_target_var_expr = _t1491.fn(_t1491.env, _t1490, 1);
#line 874 "compiler/main.gem"
    GemVal _t1492 = gem_table_new();
        GemVal gem_v_whens = _t1492;
#line 875 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1493 = (*gem_v_peek);
            GemVal _t1494 = _t1493.fn(_t1493.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1494, gem_string("type")), gem_string("when")))) break;
#line 876 "compiler/main.gem"
    GemVal _t1495 = (*gem_v_advance);
            (void)(_t1495.fn(_t1495.env, NULL, 0));
#line 878 "compiler/main.gem"
    GemVal _t1496 = (*gem_v_peek);
    GemVal _t1497 = _t1496.fn(_t1496.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1497, gem_string("type"));
#line 879 "compiler/main.gem"
    GemVal _t1498[] = {gem_int(1)};
    GemVal _t1499 = (*gem_v_peek_at);
    GemVal _t1500 = _t1499.fn(_t1499.env, _t1498, 1);
            GemVal gem_v_next = gem_table_get(_t1500, gem_string("type"));
#line 880 "compiler/main.gem"
    GemVal _t1501;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1501 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1501 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1501)) {
#line 881 "compiler/main.gem"
    GemVal _t1502[] = {gem_v_target_var_expr};
    GemVal _t1503 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1503.fn(_t1503.env, _t1502, 1);
#line 882 "compiler/main.gem"
    GemVal _t1504 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1504.fn(_t1504.env, NULL, 0);
#line 883 "compiler/main.gem"
    GemVal _t1505 = gem_v_pat;
    GemVal _t1506 = gem_v_pat;
    GemVal _t1507[] = {gem_table_get(_t1505, gem_string("condition")), gem_v_wbody, gem_table_get(_t1506, gem_string("bindings"))};
    GemVal _t1508 = (*gem_v__mod_parser_make_when);
    GemVal _t1509[] = {gem_v_whens, _t1508.fn(_t1508.env, _t1507, 3)};
                (void)(gem_push_fn(NULL, _t1509, 2));
            } else {
#line 884 "compiler/main.gem"
    GemVal _t1514;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1514 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1510;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1510 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1510 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1511;
        if (gem_truthy(_t1510)) {
                _t1511 = _t1510;
        } else {
                _t1511 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1512;
        if (gem_truthy(_t1511)) {
                _t1512 = _t1511;
        } else {
                _t1512 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1513;
        if (gem_truthy(_t1512)) {
                _t1513 = _t1512;
        } else {
                _t1513 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1514 = _t1513;
    }
                if (gem_truthy(_t1514)) {
#line 886 "compiler/main.gem"
    GemVal _t1515[] = {gem_v_target_var_expr};
    GemVal _t1516 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1516.fn(_t1516.env, _t1515, 1);
#line 887 "compiler/main.gem"
    GemVal _t1517 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1517.fn(_t1517.env, NULL, 0);
#line 888 "compiler/main.gem"
    GemVal _t1518 = gem_v_pat;
    GemVal _t1519 = gem_v_pat;
    GemVal _t1520[] = {gem_table_get(_t1518, gem_string("condition")), gem_v_wbody, gem_table_get(_t1519, gem_string("bindings"))};
    GemVal _t1521 = (*gem_v__mod_parser_make_when);
    GemVal _t1522[] = {gem_v_whens, _t1521.fn(_t1521.env, _t1520, 3)};
                    (void)(gem_push_fn(NULL, _t1522, 2));
                } else {
#line 891 "compiler/main.gem"
    GemVal _t1523 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1523.fn(_t1523.env, NULL, 0);
#line 892 "compiler/main.gem"
    GemVal _t1524 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1524.fn(_t1524.env, NULL, 0);
#line 893 "compiler/main.gem"
    GemVal _t1525[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1526 = (*gem_v__mod_parser_make_binop);
    GemVal _t1527 = gem_table_new();
    GemVal _t1528[] = {_t1526.fn(_t1526.env, _t1525, 3), gem_v_wbody, _t1527};
    GemVal _t1529 = (*gem_v__mod_parser_make_when);
    GemVal _t1530[] = {gem_v_whens, _t1529.fn(_t1529.env, _t1528, 3)};
                    (void)(gem_push_fn(NULL, _t1530, 2));
                }
            }
#line 895 "compiler/main.gem"
    GemVal _t1531 = (*gem_v_skip_nl);
            (void)(_t1531.fn(_t1531.env, NULL, 0));
        }
#line 897 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 898 "compiler/main.gem"
    GemVal _t1532 = (*gem_v_peek);
    GemVal _t1533 = _t1532.fn(_t1532.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1533, gem_string("type")), gem_string("else")))) {
#line 899 "compiler/main.gem"
    GemVal _t1534 = (*gem_v_advance);
            (void)(_t1534.fn(_t1534.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1535 = (*gem_v_parse_body);
            gem_v_else_body = _t1535.fn(_t1535.env, NULL, 0);
        }
#line 902 "compiler/main.gem"
    GemVal _t1536[] = {gem_string("end")};
    GemVal _t1537 = (*gem_v_expect);
        (void)(_t1537.fn(_t1537.env, _t1536, 1));
#line 903 "compiler/main.gem"
    GemVal _t1538[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
    GemVal _t1539 = (*gem_v__mod_parser_make_match);
        GemVal _t1540 = _t1539.fn(_t1539.env, _t1538, 5);
        gem_pop_frame();
        return _t1540;
    }
#line 907 "compiler/main.gem"
    GemVal _t1541 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1541, gem_string("type")), gem_string("return")))) {
#line 908 "compiler/main.gem"
    GemVal _t1542 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1542, gem_string("line"));
#line 909 "compiler/main.gem"
    GemVal _t1543 = (*gem_v_advance);
        (void)(_t1543.fn(_t1543.env, NULL, 0));
#line 910 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 911 "compiler/main.gem"
    GemVal _t1544 = (*gem_v_peek);
    GemVal _t1545 = _t1544.fn(_t1544.env, NULL, 0);
    GemVal _t1548;
    if (!gem_truthy(gem_neq(gem_table_get(_t1545, gem_string("type")), gem_string("NEWLINE")))) {
        _t1548 = gem_neq(gem_table_get(_t1545, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1546 = (*gem_v_peek);
        GemVal _t1547 = _t1546.fn(_t1546.env, NULL, 0);
        _t1548 = gem_neq(gem_table_get(_t1547, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1551;
    if (!gem_truthy(_t1548)) {
        _t1551 = _t1548;
    } else {
        GemVal _t1549 = (*gem_v_peek);
        GemVal _t1550 = _t1549.fn(_t1549.env, NULL, 0);
        _t1551 = gem_neq(gem_table_get(_t1550, gem_string("type")), gem_string("end"));
    }
    GemVal _t1554;
    if (!gem_truthy(_t1551)) {
        _t1554 = _t1551;
    } else {
        GemVal _t1552 = (*gem_v_peek);
        GemVal _t1553 = _t1552.fn(_t1552.env, NULL, 0);
        _t1554 = gem_neq(gem_table_get(_t1553, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1557;
    if (!gem_truthy(_t1554)) {
        _t1557 = _t1554;
    } else {
        GemVal _t1555 = (*gem_v_peek);
        GemVal _t1556 = _t1555.fn(_t1555.env, NULL, 0);
        _t1557 = gem_neq(gem_table_get(_t1556, gem_string("type")), gem_string("else"));
    }
    GemVal _t1560;
    if (!gem_truthy(_t1557)) {
        _t1560 = _t1557;
    } else {
        GemVal _t1558 = (*gem_v_peek);
        GemVal _t1559 = _t1558.fn(_t1558.env, NULL, 0);
        _t1560 = gem_neq(gem_table_get(_t1559, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1560)) {
#line 912 "compiler/main.gem"
    GemVal _t1561 = (*gem_v_parse_expr);
            gem_v_value = _t1561.fn(_t1561.env, NULL, 0);
        }
#line 914 "compiler/main.gem"
    GemVal _t1562[] = {gem_v_value, gem_v_line};
    GemVal _t1563 = (*gem_v__mod_parser_make_return);
        GemVal _t1564 = _t1563.fn(_t1563.env, _t1562, 2);
        gem_pop_frame();
        return _t1564;
    }
#line 918 "compiler/main.gem"
    GemVal _t1565 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1565, gem_string("type")), gem_string("break")))) {
#line 919 "compiler/main.gem"
    GemVal _t1566 = (*gem_v_advance);
        (void)(_t1566.fn(_t1566.env, NULL, 0));
#line 920 "compiler/main.gem"
    GemVal _t1567 = (*gem_v__mod_parser_make_break);
        GemVal _t1568 = _t1567.fn(_t1567.env, NULL, 0);
        gem_pop_frame();
        return _t1568;
    }
#line 924 "compiler/main.gem"
    GemVal _t1569 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1569, gem_string("type")), gem_string("continue")))) {
#line 925 "compiler/main.gem"
    GemVal _t1570 = (*gem_v_advance);
        (void)(_t1570.fn(_t1570.env, NULL, 0));
#line 926 "compiler/main.gem"
    GemVal _t1571 = (*gem_v__mod_parser_make_continue);
        GemVal _t1572 = _t1571.fn(_t1571.env, NULL, 0);
        gem_pop_frame();
        return _t1572;
    }
#line 930 "compiler/main.gem"
    GemVal _t1573 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1573, gem_string("type")), gem_string("load")))) {
#line 931 "compiler/main.gem"
    GemVal _t1574 = (*gem_v_advance);
        (void)(_t1574.fn(_t1574.env, NULL, 0));
#line 932 "compiler/main.gem"
    GemVal _t1575[] = {gem_string("STRING")};
    GemVal _t1576 = (*gem_v_expect);
    GemVal _t1577 = _t1576.fn(_t1576.env, _t1575, 1);
        GemVal gem_v_path = gem_table_get(_t1577, gem_string("value"));
#line 933 "compiler/main.gem"
        GemVal gem_v_alias_name = GEM_NIL;
#line 934 "compiler/main.gem"
        GemVal gem_v_selective = GEM_NIL;
#line 935 "compiler/main.gem"
    GemVal _t1578 = (*gem_v_peek);
    GemVal _t1579 = _t1578.fn(_t1578.env, NULL, 0);
    GemVal _t1582;
    if (!gem_truthy(gem_eq(gem_table_get(_t1579, gem_string("type")), gem_string("NAME")))) {
        _t1582 = gem_eq(gem_table_get(_t1579, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1580 = (*gem_v_peek);
        GemVal _t1581 = _t1580.fn(_t1580.env, NULL, 0);
        _t1582 = gem_eq(gem_table_get(_t1581, gem_string("value")), gem_string("as"));
    }
        if (gem_truthy(_t1582)) {
#line 936 "compiler/main.gem"
    GemVal _t1583 = (*gem_v_advance);
            (void)(_t1583.fn(_t1583.env, NULL, 0));
#line 937 "compiler/main.gem"
    GemVal _t1584[] = {gem_string("NAME")};
    GemVal _t1585 = (*gem_v_expect);
    GemVal _t1586 = _t1585.fn(_t1585.env, _t1584, 1);
            gem_v_alias_name = gem_table_get(_t1586, gem_string("value"));
        } else {
#line 938 "compiler/main.gem"
    GemVal _t1587 = (*gem_v_peek);
    GemVal _t1588 = _t1587.fn(_t1587.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1588, gem_string("type")), gem_string("(")))) {
#line 939 "compiler/main.gem"
    GemVal _t1589 = (*gem_v_advance);
                (void)(_t1589.fn(_t1589.env, NULL, 0));
#line 940 "compiler/main.gem"
    GemVal _t1590 = gem_table_new();
    GemVal _t1591[] = {gem_string("NAME")};
    GemVal _t1592 = (*gem_v_expect);
    GemVal _t1593 = _t1592.fn(_t1592.env, _t1591, 1);
    gem_table_set(_t1590, gem_int(0), gem_table_get(_t1593, gem_string("value")));
                gem_v_selective = _t1590;
#line 941 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1594 = (*gem_v_peek);
                    GemVal _t1595 = _t1594.fn(_t1594.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1595, gem_string("type")), gem_string(",")))) break;
#line 942 "compiler/main.gem"
    GemVal _t1596 = (*gem_v_advance);
                    (void)(_t1596.fn(_t1596.env, NULL, 0));
#line 943 "compiler/main.gem"
    GemVal _t1597[] = {gem_string("NAME")};
    GemVal _t1598 = (*gem_v_expect);
    GemVal _t1599 = _t1598.fn(_t1598.env, _t1597, 1);
    GemVal _t1600[] = {gem_v_selective, gem_table_get(_t1599, gem_string("value"))};
                    (void)(gem_push_fn(NULL, _t1600, 2));
                }
#line 945 "compiler/main.gem"
    GemVal _t1601[] = {gem_string(")")};
    GemVal _t1602 = (*gem_v_expect);
                (void)(_t1602.fn(_t1602.env, _t1601, 1));
            }
        }
#line 947 "compiler/main.gem"
    GemVal _t1603 = gem_table_new();
    gem_table_set(_t1603, gem_string("tag"), gem_string("load"));
    gem_table_set(_t1603, gem_string("path"), gem_v_path);
    gem_table_set(_t1603, gem_string("alias"), gem_v_alias_name);
    gem_table_set(_t1603, gem_string("selective"), gem_v_selective);
        GemVal _t1604 = _t1603;
        gem_pop_frame();
        return _t1604;
    }
#line 951 "compiler/main.gem"
    GemVal _t1605 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1605, gem_string("type")), gem_string("export")))) {
#line 952 "compiler/main.gem"
    GemVal _t1606 = (*gem_v_advance);
        (void)(_t1606.fn(_t1606.env, NULL, 0));
#line 953 "compiler/main.gem"
    GemVal _t1607 = gem_table_new();
    GemVal _t1608[] = {gem_string("NAME")};
    GemVal _t1609 = (*gem_v_expect);
    GemVal _t1610 = _t1609.fn(_t1609.env, _t1608, 1);
    gem_table_set(_t1607, gem_int(0), gem_table_get(_t1610, gem_string("value")));
        GemVal gem_v_names = _t1607;
#line 954 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1611 = (*gem_v_peek);
            GemVal _t1612 = _t1611.fn(_t1611.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1612, gem_string("type")), gem_string(",")))) break;
#line 955 "compiler/main.gem"
    GemVal _t1613 = (*gem_v_advance);
            (void)(_t1613.fn(_t1613.env, NULL, 0));
#line 956 "compiler/main.gem"
    GemVal _t1614[] = {gem_string("NAME")};
    GemVal _t1615 = (*gem_v_expect);
    GemVal _t1616 = _t1615.fn(_t1615.env, _t1614, 1);
    GemVal _t1617[] = {gem_v_names, gem_table_get(_t1616, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t1617, 2));
        }
#line 958 "compiler/main.gem"
    GemVal _t1618[] = {gem_v_names};
    GemVal _t1619 = (*gem_v__mod_parser_make_export);
        GemVal _t1620 = _t1619.fn(_t1619.env, _t1618, 1);
        gem_pop_frame();
        return _t1620;
    }
#line 962 "compiler/main.gem"
    GemVal _t1621 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1621, gem_string("type")), gem_string("extern")))) {
#line 963 "compiler/main.gem"
    GemVal _t1622 = (*gem_v_advance);
        (void)(_t1622.fn(_t1622.env, NULL, 0));
#line 964 "compiler/main.gem"
        GemVal gem_v_is_blocking = gem_bool(0);
#line 965 "compiler/main.gem"
    GemVal _t1623 = (*gem_v_peek);
    GemVal _t1624 = _t1623.fn(_t1623.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1624, gem_string("type")), gem_string("blocking")))) {
#line 966 "compiler/main.gem"
            gem_v_is_blocking = gem_bool(1);
#line 967 "compiler/main.gem"
    GemVal _t1625 = (*gem_v_advance);
            (void)(_t1625.fn(_t1625.env, NULL, 0));
        }
#line 969 "compiler/main.gem"
    GemVal _t1626 = (*gem_v_peek);
    GemVal _t1627 = _t1626.fn(_t1626.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1627, gem_string("type")), gem_string("fn")))) {
#line 970 "compiler/main.gem"
    GemVal _t1628 = (*gem_v_advance);
            (void)(_t1628.fn(_t1628.env, NULL, 0));
#line 971 "compiler/main.gem"
    GemVal _t1629[] = {gem_string("NAME")};
    GemVal _t1630 = (*gem_v_expect);
    GemVal _t1631 = _t1630.fn(_t1630.env, _t1629, 1);
            GemVal gem_v_name = gem_table_get(_t1631, gem_string("value"));
#line 972 "compiler/main.gem"
    GemVal _t1632[] = {gem_string("(")};
    GemVal _t1633 = (*gem_v_expect);
            (void)(_t1633.fn(_t1633.env, _t1632, 1));
#line 973 "compiler/main.gem"
    GemVal _t1634 = gem_table_new();
            GemVal gem_v_eparams = _t1634;
#line 974 "compiler/main.gem"
    GemVal _t1635 = (*gem_v_peek);
    GemVal _t1636 = _t1635.fn(_t1635.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1636, gem_string("type")), gem_string(")")))) {
#line 975 "compiler/main.gem"
    GemVal _t1637[] = {gem_string("NAME")};
    GemVal _t1638 = (*gem_v_expect);
    GemVal _t1639 = _t1638.fn(_t1638.env, _t1637, 1);
                GemVal gem_v_pname = gem_table_get(_t1639, gem_string("value"));
#line 976 "compiler/main.gem"
    GemVal _t1640[] = {gem_string(":")};
    GemVal _t1641 = (*gem_v_expect);
                (void)(_t1641.fn(_t1641.env, _t1640, 1));
#line 977 "compiler/main.gem"
    GemVal _t1642[] = {gem_string("NAME")};
    GemVal _t1643 = (*gem_v_expect);
    GemVal _t1644 = _t1643.fn(_t1643.env, _t1642, 1);
                GemVal gem_v_ptype = gem_table_get(_t1644, gem_string("value"));
#line 978 "compiler/main.gem"
    GemVal _t1645[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1646 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1647[] = {gem_v_eparams, _t1646.fn(_t1646.env, _t1645, 2)};
                (void)(gem_push_fn(NULL, _t1647, 2));
#line 979 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1648 = (*gem_v_peek);
                    GemVal _t1649 = _t1648.fn(_t1648.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1649, gem_string("type")), gem_string(",")))) break;
#line 980 "compiler/main.gem"
    GemVal _t1650 = (*gem_v_advance);
                    (void)(_t1650.fn(_t1650.env, NULL, 0));
#line 981 "compiler/main.gem"
    GemVal _t1651[] = {gem_string("NAME")};
    GemVal _t1652 = (*gem_v_expect);
    GemVal _t1653 = _t1652.fn(_t1652.env, _t1651, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1653, gem_string("value"));
#line 982 "compiler/main.gem"
    GemVal _t1654[] = {gem_string(":")};
    GemVal _t1655 = (*gem_v_expect);
                    (void)(_t1655.fn(_t1655.env, _t1654, 1));
#line 983 "compiler/main.gem"
    GemVal _t1656[] = {gem_string("NAME")};
    GemVal _t1657 = (*gem_v_expect);
    GemVal _t1658 = _t1657.fn(_t1657.env, _t1656, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1658, gem_string("value"));
#line 984 "compiler/main.gem"
    GemVal _t1659[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1660 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1661[] = {gem_v_eparams, _t1660.fn(_t1660.env, _t1659, 2)};
                    (void)(gem_push_fn(NULL, _t1661, 2));
                }
            }
#line 987 "compiler/main.gem"
    GemVal _t1662[] = {gem_string(")")};
    GemVal _t1663 = (*gem_v_expect);
            (void)(_t1663.fn(_t1663.env, _t1662, 1));
#line 988 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 989 "compiler/main.gem"
    GemVal _t1664 = (*gem_v_peek);
    GemVal _t1665 = _t1664.fn(_t1664.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1665, gem_string("type")), gem_string("->")))) {
#line 990 "compiler/main.gem"
    GemVal _t1666 = (*gem_v_advance);
                (void)(_t1666.fn(_t1666.env, NULL, 0));
#line 991 "compiler/main.gem"
    GemVal _t1667[] = {gem_string("NAME")};
    GemVal _t1668 = (*gem_v_expect);
    GemVal _t1669 = _t1668.fn(_t1668.env, _t1667, 1);
                gem_v_ret_type = gem_table_get(_t1669, gem_string("value"));
            }
#line 993 "compiler/main.gem"
    GemVal _t1670[] = {gem_v_name, gem_v_eparams, gem_v_ret_type, gem_v_is_blocking};
    GemVal _t1671 = (*gem_v__mod_parser_make_extern_fn);
            GemVal _t1672 = _t1671.fn(_t1671.env, _t1670, 4);
            gem_pop_frame();
            return _t1672;
        } else {
#line 994 "compiler/main.gem"
    GemVal _t1675;
    if (!gem_truthy(gem_not(gem_v_is_blocking))) {
        _t1675 = gem_not(gem_v_is_blocking);
    } else {
        GemVal _t1673 = (*gem_v_peek);
        GemVal _t1674 = _t1673.fn(_t1673.env, NULL, 0);
        _t1675 = gem_eq(gem_table_get(_t1674, gem_string("type")), gem_string("NAME"));
    }
    GemVal _t1678;
    if (!gem_truthy(_t1675)) {
        _t1678 = _t1675;
    } else {
        GemVal _t1676 = (*gem_v_peek);
        GemVal _t1677 = _t1676.fn(_t1676.env, NULL, 0);
        _t1678 = gem_eq(gem_table_get(_t1677, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1678)) {
#line 995 "compiler/main.gem"
    GemVal _t1679 = (*gem_v_advance);
                (void)(_t1679.fn(_t1679.env, NULL, 0));
#line 996 "compiler/main.gem"
    GemVal _t1680[] = {gem_string("STRING")};
    GemVal _t1681 = (*gem_v_expect);
    GemVal _t1682 = _t1681.fn(_t1681.env, _t1680, 1);
                GemVal gem_v_path = gem_table_get(_t1682, gem_string("value"));
#line 997 "compiler/main.gem"
    GemVal _t1683[] = {gem_v_path};
    GemVal _t1684 = (*gem_v__mod_parser_make_extern_include);
                GemVal _t1685 = _t1684.fn(_t1684.env, _t1683, 1);
                gem_pop_frame();
                return _t1685;
            } else {
#line 999 "compiler/main.gem"
    GemVal _t1686 = (*gem_v_peek);
    GemVal _t1687 = _t1686.fn(_t1686.env, NULL, 0);
    GemVal _t1688 = (*gem_v_peek);
    GemVal _t1689 = _t1688.fn(_t1688.env, NULL, 0);
    GemVal _t1690 = (*gem_v_peek);
    GemVal _t1691 = _t1690.fn(_t1690.env, NULL, 0);
    GemVal _t1692[] = {gem_table_get(_t1691, gem_string("value"))};
    GemVal _t1693[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1687, gem_string("line")), gem_table_get(_t1689, gem_string("col")), gem_len_fn(NULL, _t1692, 1), gem_string("expected 'fn' after 'extern blocking'"), GEM_NIL};
    GemVal _t1694 = (*gem_v__mod_parser_compile_error);
                (void)(_t1694.fn(_t1694.env, _t1693, 7));
            }
        }
    }
#line 1004 "compiler/main.gem"
    GemVal _t1695 = gem_v_t;
    GemVal _t1697;
    if (!gem_truthy(gem_eq(gem_table_get(_t1695, gem_string("type")), gem_string("NAME")))) {
        _t1697 = gem_eq(gem_table_get(_t1695, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1696 = gem_v_t;
        _t1697 = gem_eq(gem_table_get(_t1696, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1697)) {
#line 1005 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 1006 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1698[] = {gem_v_rla};
            GemVal _t1699 = (*gem_v_peek_at);
            GemVal _t1700 = _t1699.fn(_t1699.env, _t1698, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1700, gem_string("type")), gem_string("NEWLINE")))) break;
#line 1007 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 1009 "compiler/main.gem"
    GemVal _t1701[] = {gem_v_rla};
    GemVal _t1702 = (*gem_v_peek_at);
    GemVal _t1703 = _t1702.fn(_t1702.env, _t1701, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1703, gem_string("type")), gem_string("when")))) {
#line 1010 "compiler/main.gem"
    GemVal _t1704 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1704, gem_string("line"));
#line 1011 "compiler/main.gem"
    GemVal _t1705 = (*gem_v_advance);
            (void)(_t1705.fn(_t1705.env, NULL, 0));
#line 1012 "compiler/main.gem"
    GemVal _t1706 = (*gem_v_skip_nl);
            (void)(_t1706.fn(_t1706.env, NULL, 0));
#line 1013 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 1014 "compiler/main.gem"
    GemVal _t1707[] = {gem_string("_recv_"), (*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_interp(2, _t1707);
#line 1015 "compiler/main.gem"
    GemVal _t1708[] = {gem_v_recv_var};
    GemVal _t1709 = (*gem_v__mod_parser_make_var);
            GemVal gem_v_recv_var_expr = _t1709.fn(_t1709.env, _t1708, 1);
#line 1016 "compiler/main.gem"
    GemVal _t1710 = gem_table_new();
            GemVal gem_v_rarms = _t1710;
#line 1017 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1711 = (*gem_v_peek);
                GemVal _t1712 = _t1711.fn(_t1711.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1712, gem_string("type")), gem_string("when")))) break;
#line 1018 "compiler/main.gem"
    GemVal _t1713 = (*gem_v_advance);
                (void)(_t1713.fn(_t1713.env, NULL, 0));
#line 1019 "compiler/main.gem"
    GemVal _t1714 = (*gem_v_peek);
    GemVal _t1715 = _t1714.fn(_t1714.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1715, gem_string("type"));
#line 1020 "compiler/main.gem"
    GemVal _t1716[] = {gem_int(1)};
    GemVal _t1717 = (*gem_v_peek_at);
    GemVal _t1718 = _t1717.fn(_t1717.env, _t1716, 1);
                GemVal gem_v_rnext = gem_table_get(_t1718, gem_string("type"));
#line 1021 "compiler/main.gem"
    GemVal _t1719;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1719 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1719 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1719)) {
#line 1022 "compiler/main.gem"
    GemVal _t1720[] = {gem_v_recv_var_expr};
    GemVal _t1721 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1721.fn(_t1721.env, _t1720, 1);
#line 1023 "compiler/main.gem"
    GemVal _t1722 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1722.fn(_t1722.env, NULL, 0);
#line 1024 "compiler/main.gem"
    GemVal _t1723 = gem_table_new();
    gem_table_set(_t1723, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1723, gem_string("body"), gem_v_rwbody);
    GemVal _t1724[] = {gem_v_rarms, _t1723};
                    (void)(gem_push_fn(NULL, _t1724, 2));
                } else {
#line 1025 "compiler/main.gem"
    GemVal _t1730;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1730 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1725;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1725 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1725 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1726;
        if (gem_truthy(_t1725)) {
                _t1726 = _t1725;
        } else {
                _t1726 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1727;
        if (gem_truthy(_t1726)) {
                _t1727 = _t1726;
        } else {
                _t1727 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1728;
        if (gem_truthy(_t1727)) {
                _t1728 = _t1727;
        } else {
                _t1728 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1729;
        if (gem_truthy(_t1728)) {
                _t1729 = _t1728;
        } else {
                _t1729 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1730 = _t1729;
    }
                    if (gem_truthy(_t1730)) {
#line 1026 "compiler/main.gem"
    GemVal _t1731[] = {gem_v_recv_var_expr};
    GemVal _t1732 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1732.fn(_t1732.env, _t1731, 1);
#line 1027 "compiler/main.gem"
    GemVal _t1733 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1733.fn(_t1733.env, NULL, 0);
#line 1028 "compiler/main.gem"
    GemVal _t1734 = gem_table_new();
    gem_table_set(_t1734, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1734, gem_string("body"), gem_v_rwbody);
    GemVal _t1735[] = {gem_v_rarms, _t1734};
                        (void)(gem_push_fn(NULL, _t1735, 2));
                    } else {
#line 1030 "compiler/main.gem"
    GemVal _t1736 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1736.fn(_t1736.env, NULL, 0);
#line 1031 "compiler/main.gem"
    GemVal _t1737 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1737.fn(_t1737.env, NULL, 0);
#line 1032 "compiler/main.gem"
    GemVal _t1738 = gem_table_new();
    GemVal _t1739 = gem_table_new();
    GemVal _t1740[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    GemVal _t1741 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1739, gem_string("condition"), _t1741.fn(_t1741.env, _t1740, 3));
    GemVal _t1742 = gem_table_new();
    gem_table_set(_t1739, gem_string("bindings"), _t1742);
    gem_table_set(_t1738, gem_string("pattern"), _t1739);
    gem_table_set(_t1738, gem_string("body"), gem_v_rwbody);
    GemVal _t1743[] = {gem_v_rarms, _t1738};
                        (void)(gem_push_fn(NULL, _t1743, 2));
                    }
                }
#line 1034 "compiler/main.gem"
    GemVal _t1744 = (*gem_v_skip_nl);
                (void)(_t1744.fn(_t1744.env, NULL, 0));
            }
#line 1036 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 1037 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 1038 "compiler/main.gem"
    GemVal _t1745 = (*gem_v_peek);
    GemVal _t1746 = _t1745.fn(_t1745.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1746, gem_string("type")), gem_string("after")))) {
#line 1039 "compiler/main.gem"
    GemVal _t1747 = (*gem_v_advance);
                (void)(_t1747.fn(_t1747.env, NULL, 0));
#line 1040 "compiler/main.gem"
    GemVal _t1748 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1748.fn(_t1748.env, NULL, 0);
#line 1041 "compiler/main.gem"
    GemVal _t1749 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1749.fn(_t1749.env, NULL, 0);
            }
#line 1043 "compiler/main.gem"
    GemVal _t1750[] = {gem_string("end")};
    GemVal _t1751 = (*gem_v_expect);
            (void)(_t1751.fn(_t1751.env, _t1750, 1));
#line 1044 "compiler/main.gem"
    GemVal _t1752[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
    GemVal _t1753 = (*gem_v__mod_parser_make_receive_match);
            GemVal _t1754 = _t1753.fn(_t1753.env, _t1752, 5);
            gem_pop_frame();
            return _t1754;
        }
    }
#line 1049 "compiler/main.gem"
    GemVal _t1755 = (*gem_v_parse_expr);
    GemVal _t1756 = _t1755.fn(_t1755.env, NULL, 0);
    gem_pop_frame();
    return _t1756;
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
#line 1058 "compiler/main.gem"
    GemVal _t1758 = gem_table_new();
    GemVal gem_v_stmts = _t1758;
#line 1059 "compiler/main.gem"
    GemVal _t1759 = (*gem_v_skip_nl);
    (void)(_t1759.fn(_t1759.env, NULL, 0));
#line 1060 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1760 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1760.fn(_t1760.env, NULL, 0)))) break;
#line 1061 "compiler/main.gem"
    GemVal _t1761 = (*gem_v_parse_stmt);
    GemVal _t1762[] = {gem_v_stmts, _t1761.fn(_t1761.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1762, 2));
#line 1062 "compiler/main.gem"
    GemVal _t1763 = (*gem_v_skip_nl);
        (void)(_t1763.fn(_t1763.env, NULL, 0));
    }
#line 1064 "compiler/main.gem"
    GemVal _t1764[] = {gem_v_stmts};
    GemVal _t1765 = (*gem_v__mod_parser_make_program);
    GemVal _t1766 = _t1765.fn(_t1765.env, _t1764, 1);
    gem_pop_frame();
    return _t1766;
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
    struct _closure__anon_5 *_t455 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t455->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t455->gem_v_file = gem_v_file;
    _t455->gem_v_pos = gem_v_pos;
    _t455->gem_v_source = gem_v_source;
    _t455->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_5, _t455);
#line 100 "compiler/main.gem"
    struct _closure__anon_6 *_t457 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t457->gem_v_pos = gem_v_pos;
    _t457->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_6, _t457);
#line 111 "compiler/main.gem"
    struct _closure__anon_7 *_t478 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t478->gem_v_advance = gem_v_advance;
    _t478->gem_v_expect = gem_v_expect;
    _t478->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t478);
#line 133 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 134 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 135 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 136 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 137 "compiler/main.gem"
    GemVal *gem_v_parse_pattern = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_pattern = GEM_NIL;
#line 143 "compiler/main.gem"
    struct _closure__anon_8 *_t504 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t504->gem_v_at_end = gem_v_at_end;
    _t504->gem_v_parse_stmt = gem_v_parse_stmt;
    _t504->gem_v_peek = gem_v_peek;
    _t504->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t504);
#line 156 "compiler/main.gem"
    struct _closure__anon_9 *_t515 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t515->gem_v_at_end = gem_v_at_end;
    _t515->gem_v_parse_stmt = gem_v_parse_stmt;
    _t515->gem_v_peek = gem_v_peek;
    _t515->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t515);
#line 169 "compiler/main.gem"
    struct _closure__anon_10 *_t687 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t687->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t687->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t687->gem_v__mod_parser_make_array = &gem_v__mod_parser_make_array;
    _t687->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t687->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t687->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t687->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t687->gem_v__mod_parser_make_interp = &gem_v__mod_parser_make_interp;
    _t687->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t687->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t687->gem_v__mod_parser_make_table = &gem_v__mod_parser_make_table;
    _t687->gem_v__mod_parser_make_table_entry = &gem_v__mod_parser_make_table_entry;
    _t687->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t687->gem_v_advance = gem_v_advance;
    _t687->gem_v_at_end = gem_v_at_end;
    _t687->gem_v_expect = gem_v_expect;
    _t687->gem_v_file = gem_v_file;
    _t687->gem_v_fn_depth = gem_v_fn_depth;
    _t687->gem_v_parse_expr = gem_v_parse_expr;
    _t687->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t687->gem_v_parse_params = gem_v_parse_params;
    _t687->gem_v_peek = gem_v_peek;
    _t687->gem_v_peek_at = gem_v_peek_at;
    _t687->gem_v_skip_nl = gem_v_skip_nl;
    _t687->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t687);
#line 307 "compiler/main.gem"
    struct _closure__anon_11 *_t781 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t781->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t781->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t781->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t781->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t781->gem_v_advance = gem_v_advance;
    _t781->gem_v_expect = gem_v_expect;
    _t781->gem_v_fn_depth = gem_v_fn_depth;
    _t781->gem_v_parse_atom = gem_v_parse_atom;
    _t781->gem_v_parse_expr = gem_v_parse_expr;
    _t781->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t781->gem_v_parse_params = gem_v_parse_params;
    _t781->gem_v_peek = gem_v_peek;
    _t781->gem_v_peek_at = gem_v_peek_at;
    _t781->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t781);
#line 409 "compiler/main.gem"
    struct _closure__anon_12 *_t791 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t791->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t791->gem_v_advance = gem_v_advance;
    _t791->gem_v_parse_call = gem_v_parse_call;
    _t791->gem_v_parse_unary = gem_v_parse_unary;
    _t791->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t791);
#line 418 "compiler/main.gem"
    struct _closure__anon_13 *_t807 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t807->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t807->gem_v_advance = gem_v_advance;
    _t807->gem_v_parse_unary = gem_v_parse_unary;
    _t807->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t807);
#line 429 "compiler/main.gem"
    struct _closure__anon_14 *_t820 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t820->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t820->gem_v_advance = gem_v_advance;
    _t820->gem_v_parse_mul = gem_v_parse_mul;
    _t820->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t820);
#line 440 "compiler/main.gem"
    struct _closure__anon_15 *_t848 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t848->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t848->gem_v_advance = gem_v_advance;
    _t848->gem_v_parse_add = gem_v_parse_add;
    _t848->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t848);
#line 451 "compiler/main.gem"
    struct _closure__anon_16 *_t858 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t858->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t858->gem_v_advance = gem_v_advance;
    _t858->gem_v_parse_compare = gem_v_parse_compare;
    _t858->gem_v_parse_not = gem_v_parse_not;
    _t858->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t858);
#line 460 "compiler/main.gem"
    struct _closure__anon_17 *_t867 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t867->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t867->gem_v_advance = gem_v_advance;
    _t867->gem_v_parse_not = gem_v_parse_not;
    _t867->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t867);
#line 471 "compiler/main.gem"
    struct _closure__anon_18 *_t876 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t876->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t876->gem_v_advance = gem_v_advance;
    _t876->gem_v_parse_and = gem_v_parse_and;
    _t876->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t876);
#line 482 "compiler/main.gem"
    struct _closure__anon_19 *_t934 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t934->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t934->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t934->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t934->gem_v__mod_parser_make_dot_assign = &gem_v__mod_parser_make_dot_assign;
    _t934->gem_v__mod_parser_make_index_assign = &gem_v__mod_parser_make_index_assign;
    _t934->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t934->gem_v_advance = gem_v_advance;
    _t934->gem_v_file = gem_v_file;
    _t934->gem_v_parse_expr = gem_v_parse_expr;
    _t934->gem_v_parse_or = gem_v_parse_or;
    _t934->gem_v_peek = gem_v_peek;
    _t934->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t934);
#line 517 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 521 "compiler/main.gem"
    struct _closure__anon_20 *_t1143 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1143->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1143->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1143->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t1143->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1143->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t1143->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1143->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1143->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1143->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t1143->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t1143->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1143->gem_v_advance = gem_v_advance;
    _t1143->gem_v_expect = gem_v_expect;
    _t1143->gem_v_file = gem_v_file;
    _t1143->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1143->gem_v_peek = gem_v_peek;
    _t1143->gem_v_peek_at = gem_v_peek_at;
    _t1143->gem_v_skip_nl = gem_v_skip_nl;
    _t1143->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t1143);
#line 649 "compiler/main.gem"
    struct _closure__anon_22 *_t1757 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1757->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1757->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t1757->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1757->gem_v__mod_parser_make_break = &gem_v__mod_parser_make_break;
    _t1757->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1757->gem_v__mod_parser_make_continue = &gem_v__mod_parser_make_continue;
    _t1757->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t1757->gem_v__mod_parser_make_export = &gem_v__mod_parser_make_export;
    _t1757->gem_v__mod_parser_make_extern_fn = &gem_v__mod_parser_make_extern_fn;
    _t1757->gem_v__mod_parser_make_extern_include = &gem_v__mod_parser_make_extern_include;
    _t1757->gem_v__mod_parser_make_extern_param = &gem_v__mod_parser_make_extern_param;
    _t1757->gem_v__mod_parser_make_fn_def = &gem_v__mod_parser_make_fn_def;
    _t1757->gem_v__mod_parser_make_if = &gem_v__mod_parser_make_if;
    _t1757->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1757->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1757->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1757->gem_v__mod_parser_make_match = &gem_v__mod_parser_make_match;
    _t1757->gem_v__mod_parser_make_receive_match = &gem_v__mod_parser_make_receive_match;
    _t1757->gem_v__mod_parser_make_return = &gem_v__mod_parser_make_return;
    _t1757->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1757->gem_v__mod_parser_make_when = &gem_v__mod_parser_make_when;
    _t1757->gem_v__mod_parser_make_while = &gem_v__mod_parser_make_while;
    _t1757->gem_v_advance = gem_v_advance;
    _t1757->gem_v_expect = gem_v_expect;
    _t1757->gem_v_file = gem_v_file;
    _t1757->gem_v_fn_depth = gem_v_fn_depth;
    _t1757->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1757->gem_v_parse_body = gem_v_parse_body;
    _t1757->gem_v_parse_expr = gem_v_parse_expr;
    _t1757->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1757->gem_v_parse_params = gem_v_parse_params;
    _t1757->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1757->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1757->gem_v_peek = gem_v_peek;
    _t1757->gem_v_peek_at = gem_v_peek_at;
    _t1757->gem_v_skip_nl = gem_v_skip_nl;
    _t1757->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1757);
#line 1053 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 1057 "compiler/main.gem"
    struct _closure__anon_23 *_t1767 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1767->gem_v__mod_parser_make_program = &gem_v__mod_parser_make_program;
    _t1767->gem_v_at_end = gem_v_at_end;
    _t1767->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1767->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1767);
    GemVal _t1768 = gem_table_new();
    gem_table_set(_t1768, gem_string("parse"), gem_v_parse);
    GemVal _t1769 = _t1768;
    gem_pop_frame();
    return _t1769;
}

static GemVal gem_fn__mod_codegen_escape_c_string(void *_env, GemVal *args, int argc) {
#line 7 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_escape_c_string", "compiler/main.gem", 7);
#line 8 "compiler/main.gem"
    GemVal _t1770[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1770, 1);
#line 9 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 10 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 10 "compiler/main.gem"
    GemVal _t1771[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1771, 1);
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
    GemVal _t1772[] = {gem_v_b, gem_string("\\\\")};
            (void)(gem_buf_push_fn(NULL, _t1772, 2));
        } else {
#line 14 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 15 "compiler/main.gem"
    GemVal _t1773[] = {gem_v_b, gem_string("\\\"")};
                (void)(gem_buf_push_fn(NULL, _t1773, 2));
            } else {
#line 16 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 17 "compiler/main.gem"
    GemVal _t1774[] = {gem_v_b, gem_string("\\n")};
                    (void)(gem_buf_push_fn(NULL, _t1774, 2));
                } else {
#line 18 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 19 "compiler/main.gem"
    GemVal _t1775[] = {gem_v_b, gem_string("\\r")};
                        (void)(gem_buf_push_fn(NULL, _t1775, 2));
                    } else {
#line 20 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 21 "compiler/main.gem"
    GemVal _t1776[] = {gem_v_b, gem_string("\\t")};
                            (void)(gem_buf_push_fn(NULL, _t1776, 2));
                        } else {
#line 22 "compiler/main.gem"
    GemVal _t1777[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1777, 1), gem_int(0)))) {
#line 23 "compiler/main.gem"
    GemVal _t1778[] = {gem_v_b, gem_string("\\0")};
                                (void)(gem_buf_push_fn(NULL, _t1778, 2));
                            } else {
#line 25 "compiler/main.gem"
    GemVal _t1779[] = {gem_v_b, gem_v_ch};
                                (void)(gem_buf_push_fn(NULL, _t1779, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 28 "compiler/main.gem"
    GemVal _t1780[] = {gem_v_b};
    GemVal _t1781 = gem_buf_str_fn(NULL, _t1780, 1);
    gem_pop_frame();
    return _t1781;
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
    GemVal _t1782[] = {gem_v_b, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t1782, 2));
    }

#line 36 "compiler/main.gem"
    GemVal _t1783[] = {gem_v_b};
    GemVal _t1784 = gem_buf_str_fn(NULL, _t1783, 1);
    gem_pop_frame();
    return _t1784;
}

static GemVal gem_fn__mod_codegen_pad_for(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_pad_for", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal _t1785[] = {gem_string("    "), gem_v_indent};
    GemVal _t1786 = gem_fn__mod_codegen_repeat_str(NULL, _t1785, 2);
    gem_pop_frame();
    return _t1786;
}

static GemVal gem_fn__mod_codegen_format_float(void *_env, GemVal *args, int argc) {
#line 43 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_format_float", "compiler/main.gem", 43);
#line 44 "compiler/main.gem"
    GemVal _t1787[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1787, 1);
#line 46 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 47 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 47 "compiler/main.gem"
    GemVal _t1788[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1788, 1);
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
    GemVal _t1789 = gem_v_s;
    gem_pop_frame();
    return _t1789;
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
    GemVal _t1790 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1790;
}

static GemVal gem_fn__mod_codegen_set_union(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_union", "compiler/main.gem", 70);
#line 71 "compiler/main.gem"
    GemVal _t1791 = gem_table_new();
    GemVal gem_v_result = _t1791;
#line 72 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 72 "compiler/main.gem"
    GemVal _t1792[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_len_4 = gem_len_fn(NULL, _t1792, 1);
#line 72 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 72 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_len_4))) break;
#line 72 "compiler/main.gem"
    GemVal _t1793[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1793, 2);
#line 72 "compiler/main.gem"
    GemVal _t1794[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1794, 2);
#line 72 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 73 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

#line 75 "compiler/main.gem"
    GemVal gem_v__for_tbl_5 = gem_v_b;
#line 75 "compiler/main.gem"
    GemVal _t1795[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_len_5 = gem_len_fn(NULL, _t1795, 1);
#line 75 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 75 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_len_5))) break;
#line 75 "compiler/main.gem"
    GemVal _t1796[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1796, 2);
#line 75 "compiler/main.gem"
    GemVal _t1797[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1797, 2);
#line 75 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 76 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1798 = gem_v_result;
    gem_pop_frame();
    return _t1798;
}

static GemVal gem_fn__mod_codegen_set_intersect(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_intersect", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t1799 = gem_table_new();
    GemVal gem_v_result = _t1799;
#line 83 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 83 "compiler/main.gem"
    GemVal _t1800[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_len_6 = gem_len_fn(NULL, _t1800, 1);
#line 83 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 83 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_len_6))) break;
#line 83 "compiler/main.gem"
    GemVal _t1801[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1801, 2);
#line 83 "compiler/main.gem"
    GemVal _t1802[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1802, 2);
#line 83 "compiler/main.gem"
        gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 84 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_v_k), gem_bool(1)))) {
#line 85 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
        }
    }

    GemVal _t1803 = gem_v_result;
    gem_pop_frame();
    return _t1803;
}

static GemVal gem_fn__mod_codegen_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 91 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_sorted_array_to_set", "compiler/main.gem", 91);
#line 92 "compiler/main.gem"
    GemVal _t1804 = gem_table_new();
    GemVal gem_v_s = _t1804;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1805[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1805, 1)))) break;
#line 93 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 93 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 94 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1806 = gem_v_s;
    gem_pop_frame();
    return _t1806;
}

static GemVal gem_fn__mod_codegen_is_node(void *_env, GemVal *args, int argc) {
#line 99 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_is_node", "compiler/main.gem", 99);
    GemVal _t1807[] = {gem_v_node};
    GemVal _t1809;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table")))) {
        _t1809 = gem_eq(gem_type_fn(NULL, _t1807, 1), gem_string("table"));
    } else {
        GemVal _t1808 = gem_v_node;
        _t1809 = gem_eq(gem_table_get(_t1808, gem_string("tag")), gem_v_tag);
    }
    GemVal _t1810 = _t1809;
    gem_pop_frame();
    return _t1810;
}

static GemVal gem_fn__mod_codegen_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_collect_block_lets", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t1811[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1811, 2))) {
        {
#line 107 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal _t1812 = gem_v_stmt;
    GemVal _t1813[] = {gem_table_get(_t1812, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1813, 1);
#line 107 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 107 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 107 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 108 "compiler/main.gem"
    GemVal _t1814 = gem_v_stmt;
    GemVal _t1815[] = {gem_table_get(gem_table_get(_t1814, gem_string("stmts")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1815, 2))) {
#line 109 "compiler/main.gem"
    GemVal _t1816 = gem_v_stmt;
    GemVal _t1817 = gem_table_get(gem_table_get(_t1816, gem_string("stmts")), gem_v_j);
    GemVal _t1818[] = {gem_v_result, gem_table_get(_t1817, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1818, 2));
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
    GemVal _t1819[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1819, 1);
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
    GemVal _t1820[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1820, 2))) {
#line 119 "compiler/main.gem"
    GemVal _t1821 = gem_v_s;
    GemVal _t1822[] = {gem_v_result, gem_table_get(_t1821, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1822, 2));
            }
#line 121 "compiler/main.gem"
    GemVal _t1823[] = {gem_v_s, gem_v_result};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t1823, 2));
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
    GemVal _t1837[] = {gem_string("_t"), (*gem_v_tmp_counter)};
    GemVal _t1838 = gem_interp(2, _t1837);
    gem_pop_frame();
    return _t1838;
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
    GemVal _t1840[] = {gem_string("_anon_"), (*gem_v_anon_counter)};
    GemVal _t1841 = gem_interp(2, _t1840);
    gem_pop_frame();
    return _t1841;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1843[] = {gem_string("gem_v_"), gem_v_name};
    GemVal _t1844 = gem_interp(2, _t1843);
    gem_pop_frame();
    return _t1844;
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
        GemVal _t1845 = GEM_NIL;
        gem_pop_frame();
        return _t1845;
    }
#line 252 "compiler/main.gem"
    GemVal _t1846[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1846, 1), gem_string("table")))) {
#line 253 "compiler/main.gem"
        GemVal _t1847 = GEM_NIL;
        gem_pop_frame();
        return _t1847;
    }
#line 255 "compiler/main.gem"
    GemVal _t1848 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1848, gem_string("tag")), GEM_NIL))) {
#line 256 "compiler/main.gem"
        GemVal _t1849 = GEM_NIL;
        gem_pop_frame();
        return _t1849;
    }
#line 259 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1850[] = {gem_v__match_11};
    GemVal _t1852;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1850, 1), gem_string("table")))) {
        _t1852 = gem_eq(gem_type_fn(NULL, _t1850, 1), gem_string("table"));
    } else {
        GemVal _t1851[] = {gem_v__match_11, gem_string("tag")};
        _t1852 = gem_has_key_fn(NULL, _t1851, 2);
    }
    GemVal _t1853;
    if (!gem_truthy(_t1852)) {
        _t1853 = _t1852;
    } else {
        _t1853 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1855;
    if (!gem_truthy(_t1853)) {
        _t1855 = _t1853;
    } else {
        GemVal _t1854[] = {gem_v__match_11, gem_string("name")};
        _t1855 = gem_has_key_fn(NULL, _t1854, 2);
    }
    if (gem_truthy(_t1855)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 261 "compiler/main.gem"
    GemVal _t1856[] = {gem_v_defined, gem_v_name};
    GemVal _t1860;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1856, 2)))) {
        _t1860 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1856, 2));
    } else {
        GemVal _t1857[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1859;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1857, 2)))) {
                _t1859 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1857, 2));
        } else {
                GemVal _t1858[] = {(*gem_v_local_names), gem_v_name};
                _t1859 = gem_fn__mod_codegen_set_contains(NULL, _t1858, 2);
        }
        _t1860 = _t1859;
    }
    GemVal _t1862;
    if (!gem_truthy(_t1860)) {
        _t1862 = _t1860;
    } else {
        GemVal _t1861[] = {(*gem_v_defined_fns), gem_v_name};
        _t1862 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1861, 2));
    }
        if (gem_truthy(_t1862)) {
#line 262 "compiler/main.gem"
    GemVal _t1863[] = {gem_v_free, gem_v_name};
            GemVal _t1864 = gem_fn__mod_codegen_set_add(NULL, _t1863, 2);
            gem_pop_frame();
            return _t1864;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1865[] = {gem_v__match_11};
    GemVal _t1867;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1865, 1), gem_string("table")))) {
        _t1867 = gem_eq(gem_type_fn(NULL, _t1865, 1), gem_string("table"));
    } else {
        GemVal _t1866[] = {gem_v__match_11, gem_string("tag")};
        _t1867 = gem_has_key_fn(NULL, _t1866, 2);
    }
    GemVal _t1868;
    if (!gem_truthy(_t1867)) {
        _t1868 = _t1867;
    } else {
        _t1868 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1870;
    if (!gem_truthy(_t1868)) {
        _t1870 = _t1868;
    } else {
        GemVal _t1869[] = {gem_v__match_11, gem_string("value")};
        _t1870 = gem_has_key_fn(NULL, _t1869, 2);
    }
    if (gem_truthy(_t1870)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 265 "compiler/main.gem"
    GemVal _t1871[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1872 = (*gem_v_collect_free_node);
        GemVal _t1873 = _t1872.fn(_t1872.env, _t1871, 3);
        gem_pop_frame();
        return _t1873;
    } else {
    GemVal _t1874[] = {gem_v__match_11};
    GemVal _t1876;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1874, 1), gem_string("table")))) {
        _t1876 = gem_eq(gem_type_fn(NULL, _t1874, 1), gem_string("table"));
    } else {
        GemVal _t1875[] = {gem_v__match_11, gem_string("tag")};
        _t1876 = gem_has_key_fn(NULL, _t1875, 2);
    }
    GemVal _t1877;
    if (!gem_truthy(_t1876)) {
        _t1877 = _t1876;
    } else {
        _t1877 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1879;
    if (!gem_truthy(_t1877)) {
        _t1879 = _t1877;
    } else {
        GemVal _t1878[] = {gem_v__match_11, gem_string("name")};
        _t1879 = gem_has_key_fn(NULL, _t1878, 2);
    }
    GemVal _t1881;
    if (!gem_truthy(_t1879)) {
        _t1881 = _t1879;
    } else {
        GemVal _t1880[] = {gem_v__match_11, gem_string("value")};
        _t1881 = gem_has_key_fn(NULL, _t1880, 2);
    }
    if (gem_truthy(_t1881)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 267 "compiler/main.gem"
    GemVal _t1882[] = {gem_v_defined, gem_v_name};
    GemVal _t1886;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1882, 2)))) {
        _t1886 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1882, 2));
    } else {
        GemVal _t1883[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1885;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1883, 2)))) {
                _t1885 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1883, 2));
        } else {
                GemVal _t1884[] = {(*gem_v_local_names), gem_v_name};
                _t1885 = gem_fn__mod_codegen_set_contains(NULL, _t1884, 2);
        }
        _t1886 = _t1885;
    }
    GemVal _t1888;
    if (!gem_truthy(_t1886)) {
        _t1888 = _t1886;
    } else {
        GemVal _t1887[] = {(*gem_v_defined_fns), gem_v_name};
        _t1888 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1887, 2));
    }
        if (gem_truthy(_t1888)) {
#line 268 "compiler/main.gem"
    GemVal _t1889[] = {gem_v_free, gem_v_name};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1889, 2));
        }
#line 270 "compiler/main.gem"
    GemVal _t1890[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1891 = (*gem_v_collect_free_node);
        GemVal _t1892 = _t1891.fn(_t1891.env, _t1890, 3);
        gem_pop_frame();
        return _t1892;
    } else {
    GemVal _t1893[] = {gem_v__match_11};
    GemVal _t1895;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1893, 1), gem_string("table")))) {
        _t1895 = gem_eq(gem_type_fn(NULL, _t1893, 1), gem_string("table"));
    } else {
        GemVal _t1894[] = {gem_v__match_11, gem_string("tag")};
        _t1895 = gem_has_key_fn(NULL, _t1894, 2);
    }
    GemVal _t1896;
    if (!gem_truthy(_t1895)) {
        _t1896 = _t1895;
    } else {
        _t1896 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1898;
    if (!gem_truthy(_t1896)) {
        _t1898 = _t1896;
    } else {
        GemVal _t1897[] = {gem_v__match_11, gem_string("params")};
        _t1898 = gem_has_key_fn(NULL, _t1897, 2);
    }
    GemVal _t1900;
    if (!gem_truthy(_t1898)) {
        _t1900 = _t1898;
    } else {
        GemVal _t1899[] = {gem_v__match_11, gem_string("rest_param")};
        _t1900 = gem_has_key_fn(NULL, _t1899, 2);
    }
    GemVal _t1902;
    if (!gem_truthy(_t1900)) {
        _t1902 = _t1900;
    } else {
        GemVal _t1901[] = {gem_v__match_11, gem_string("body")};
        _t1902 = gem_has_key_fn(NULL, _t1901, 2);
    }
    if (gem_truthy(_t1902)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 272 "compiler/main.gem"
    GemVal _t1903[] = {gem_v_params};
    GemVal _t1904[] = {gem_v_defined, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t1903, 1)};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_set_union(NULL, _t1904, 2);
#line 273 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 274 "compiler/main.gem"
    GemVal _t1905[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1905, 2));
        }
#line 276 "compiler/main.gem"
    GemVal _t1906[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1907 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1907.fn(_t1907.env, _t1906, 2);
        {
#line 277 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 277 "compiler/main.gem"
    GemVal _t1908[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_len_12 = gem_len_fn(NULL, _t1908, 1);
#line 277 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 277 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_len_12))) break;
#line 277 "compiler/main.gem"
    GemVal _t1909[] = {gem_v__for_tbl_12, gem_v__for_i_12};
                GemVal gem_v_ifk = gem_table_key_at_fn(NULL, _t1909, 2);
#line 277 "compiler/main.gem"
    GemVal _t1910[] = {gem_v__for_tbl_12, gem_v__for_i_12};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1910, 2);
#line 277 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 278 "compiler/main.gem"
    GemVal _t1911[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1911, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1912[] = {gem_v__match_11};
    GemVal _t1914;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1912, 1), gem_string("table")))) {
        _t1914 = gem_eq(gem_type_fn(NULL, _t1912, 1), gem_string("table"));
    } else {
        GemVal _t1913[] = {gem_v__match_11, gem_string("tag")};
        _t1914 = gem_has_key_fn(NULL, _t1913, 2);
    }
    GemVal _t1915;
    if (!gem_truthy(_t1914)) {
        _t1915 = _t1914;
    } else {
        _t1915 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1915)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1916[] = {gem_v__match_11};
    GemVal _t1918;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1916, 1), gem_string("table")))) {
        _t1918 = gem_eq(gem_type_fn(NULL, _t1916, 1), gem_string("table"));
    } else {
        GemVal _t1917[] = {gem_v__match_11, gem_string("tag")};
        _t1918 = gem_has_key_fn(NULL, _t1917, 2);
    }
    GemVal _t1919;
    if (!gem_truthy(_t1918)) {
        _t1919 = _t1918;
    } else {
        _t1919 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1921;
    if (!gem_truthy(_t1919)) {
        _t1921 = _t1919;
    } else {
        GemVal _t1920[] = {gem_v__match_11, gem_string("cond")};
        _t1921 = gem_has_key_fn(NULL, _t1920, 2);
    }
    GemVal _t1923;
    if (!gem_truthy(_t1921)) {
        _t1923 = _t1921;
    } else {
        GemVal _t1922[] = {gem_v__match_11, gem_string("then")};
        _t1923 = gem_has_key_fn(NULL, _t1922, 2);
    }
    GemVal _t1925;
    if (!gem_truthy(_t1923)) {
        _t1925 = _t1923;
    } else {
        GemVal _t1924[] = {gem_v__match_11, gem_string("else")};
        _t1925 = gem_has_key_fn(NULL, _t1924, 2);
    }
    if (gem_truthy(_t1925)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 283 "compiler/main.gem"
    GemVal _t1926[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1927 = (*gem_v_collect_free_node);
        (void)(_t1927.fn(_t1927.env, _t1926, 3));
#line 284 "compiler/main.gem"
    GemVal _t1928 = gem_table_new();
    GemVal _t1929[] = {gem_v_defined, _t1928};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1929, 2);
#line 285 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 285 "compiler/main.gem"
    GemVal _t1930[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1930, 1);
#line 285 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 285 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 285 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 286 "compiler/main.gem"
    GemVal _t1931[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1932 = (*gem_v_collect_free_node);
            (void)(_t1932.fn(_t1932.env, _t1931, 3));
#line 287 "compiler/main.gem"
    GemVal _t1933[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1933, 2))) {
#line 288 "compiler/main.gem"
    GemVal _t1934 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1935[] = {gem_v_d, gem_table_get(_t1934, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1935, 2));
            }
        }

#line 291 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 292 "compiler/main.gem"
    GemVal _t1936 = gem_table_new();
    GemVal _t1937[] = {gem_v_defined, _t1936};
            gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1937, 2);
            {
#line 293 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 293 "compiler/main.gem"
    GemVal _t1938[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1938, 1);
#line 293 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 293 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 293 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 294 "compiler/main.gem"
    GemVal _t1939[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1940 = (*gem_v_collect_free_node);
                    (void)(_t1940.fn(_t1940.env, _t1939, 3));
#line 295 "compiler/main.gem"
    GemVal _t1941[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1941, 2))) {
#line 296 "compiler/main.gem"
    GemVal _t1942 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1943[] = {gem_v_d, gem_table_get(_t1942, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1943, 2));
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
    GemVal _t1944[] = {gem_v__match_11};
    GemVal _t1946;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1944, 1), gem_string("table")))) {
        _t1946 = gem_eq(gem_type_fn(NULL, _t1944, 1), gem_string("table"));
    } else {
        GemVal _t1945[] = {gem_v__match_11, gem_string("tag")};
        _t1946 = gem_has_key_fn(NULL, _t1945, 2);
    }
    GemVal _t1947;
    if (!gem_truthy(_t1946)) {
        _t1947 = _t1946;
    } else {
        _t1947 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1949;
    if (!gem_truthy(_t1947)) {
        _t1949 = _t1947;
    } else {
        GemVal _t1948[] = {gem_v__match_11, gem_string("cond")};
        _t1949 = gem_has_key_fn(NULL, _t1948, 2);
    }
    GemVal _t1951;
    if (!gem_truthy(_t1949)) {
        _t1951 = _t1949;
    } else {
        GemVal _t1950[] = {gem_v__match_11, gem_string("body")};
        _t1951 = gem_has_key_fn(NULL, _t1950, 2);
    }
    if (gem_truthy(_t1951)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 301 "compiler/main.gem"
    GemVal _t1952[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1953 = (*gem_v_collect_free_node);
        (void)(_t1953.fn(_t1953.env, _t1952, 3));
#line 302 "compiler/main.gem"
    GemVal _t1954 = gem_table_new();
    GemVal _t1955[] = {gem_v_defined, _t1954};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1955, 2);
        {
#line 303 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 303 "compiler/main.gem"
    GemVal _t1956[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1956, 1);
#line 303 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 303 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 303 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 304 "compiler/main.gem"
    GemVal _t1957[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1958 = (*gem_v_collect_free_node);
                (void)(_t1958.fn(_t1958.env, _t1957, 3));
#line 305 "compiler/main.gem"
    GemVal _t1959[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1959, 2))) {
#line 306 "compiler/main.gem"
    GemVal _t1960 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1961[] = {gem_v_d, gem_table_get(_t1960, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1961, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1962[] = {gem_v__match_11};
    GemVal _t1964;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1962, 1), gem_string("table")))) {
        _t1964 = gem_eq(gem_type_fn(NULL, _t1962, 1), gem_string("table"));
    } else {
        GemVal _t1963[] = {gem_v__match_11, gem_string("tag")};
        _t1964 = gem_has_key_fn(NULL, _t1963, 2);
    }
    GemVal _t1965;
    if (!gem_truthy(_t1964)) {
        _t1965 = _t1964;
    } else {
        _t1965 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1967;
    if (!gem_truthy(_t1965)) {
        _t1967 = _t1965;
    } else {
        GemVal _t1966[] = {gem_v__match_11, gem_string("target")};
        _t1967 = gem_has_key_fn(NULL, _t1966, 2);
    }
    GemVal _t1969;
    if (!gem_truthy(_t1967)) {
        _t1969 = _t1967;
    } else {
        GemVal _t1968[] = {gem_v__match_11, gem_string("target_var")};
        _t1969 = gem_has_key_fn(NULL, _t1968, 2);
    }
    GemVal _t1971;
    if (!gem_truthy(_t1969)) {
        _t1971 = _t1969;
    } else {
        GemVal _t1970[] = {gem_v__match_11, gem_string("whens")};
        _t1971 = gem_has_key_fn(NULL, _t1970, 2);
    }
    GemVal _t1973;
    if (!gem_truthy(_t1971)) {
        _t1973 = _t1971;
    } else {
        GemVal _t1972[] = {gem_v__match_11, gem_string("else")};
        _t1973 = gem_has_key_fn(NULL, _t1972, 2);
    }
    if (gem_truthy(_t1973)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 310 "compiler/main.gem"
    GemVal _t1974[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1975 = (*gem_v_collect_free_node);
        (void)(_t1975.fn(_t1975.env, _t1974, 3));
#line 311 "compiler/main.gem"
    GemVal _t1976 = gem_table_new();
    GemVal _t1977[] = {gem_v_defined, _t1976};
        GemVal gem_v_match_def = gem_fn__mod_codegen_set_union(NULL, _t1977, 2);
#line 312 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 313 "compiler/main.gem"
    GemVal _t1978[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1978, 2));
        }
#line 315 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 315 "compiler/main.gem"
    GemVal _t1979[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1979, 1);
#line 315 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 315 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_18;
#line 315 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 316 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 317 "compiler/main.gem"
    GemVal _t1980 = gem_v_w;
    GemVal _t1981[] = {gem_table_get(_t1980, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1982 = (*gem_v_collect_free_node);
            (void)(_t1982.fn(_t1982.env, _t1981, 3));
#line 318 "compiler/main.gem"
    GemVal _t1983 = gem_table_new();
    GemVal _t1984[] = {gem_v_match_def, _t1983};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1984, 2);
#line 319 "compiler/main.gem"
    GemVal _t1985 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1985, gem_string("bindings")), GEM_NIL))) {
#line 320 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 320 "compiler/main.gem"
    GemVal _t1986 = gem_v_w;
    GemVal _t1987[] = {gem_table_get(_t1986, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1987, 1);
#line 320 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 320 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 320 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 321 "compiler/main.gem"
    GemVal _t1988 = gem_v_w;
    GemVal _t1989[] = {gem_table_get(gem_table_get(_t1988, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1990 = (*gem_v_collect_free_node);
                    (void)(_t1990.fn(_t1990.env, _t1989, 3));
#line 322 "compiler/main.gem"
    GemVal _t1991 = gem_v_w;
    GemVal _t1992[] = {gem_table_get(gem_table_get(_t1991, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1992, 2))) {
#line 323 "compiler/main.gem"
    GemVal _t1993 = gem_v_w;
    GemVal _t1994 = gem_table_get(gem_table_get(_t1993, gem_string("bindings")), gem_v_bi);
    GemVal _t1995[] = {gem_v_d, gem_table_get(_t1994, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1995, 2));
                    }
                }

            }
#line 327 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 327 "compiler/main.gem"
    GemVal _t1996 = gem_v_w;
    GemVal _t1997[] = {gem_table_get(_t1996, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1997, 1);
#line 327 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 327 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 327 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 328 "compiler/main.gem"
    GemVal _t1998 = gem_v_w;
    GemVal _t1999[] = {gem_table_get(gem_table_get(_t1998, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2000 = (*gem_v_collect_free_node);
                (void)(_t2000.fn(_t2000.env, _t1999, 3));
#line 329 "compiler/main.gem"
    GemVal _t2001 = gem_v_w;
    GemVal _t2002[] = {gem_table_get(gem_table_get(_t2001, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2002, 2))) {
#line 330 "compiler/main.gem"
    GemVal _t2003 = gem_v_w;
    GemVal _t2004 = gem_table_get(gem_table_get(_t2003, gem_string("body")), gem_v_j);
    GemVal _t2005[] = {gem_v_d, gem_table_get(_t2004, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2005, 2));
                }
            }

        }

#line 334 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 335 "compiler/main.gem"
    GemVal _t2006 = gem_table_new();
    GemVal _t2007[] = {gem_v_defined, _t2006};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2007, 2);
            {
#line 336 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 336 "compiler/main.gem"
    GemVal _t2008[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t2008, 1);
#line 336 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 336 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 336 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 337 "compiler/main.gem"
    GemVal _t2009[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2010 = (*gem_v_collect_free_node);
                    (void)(_t2010.fn(_t2010.env, _t2009, 3));
#line 338 "compiler/main.gem"
    GemVal _t2011[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2011, 2))) {
#line 339 "compiler/main.gem"
    GemVal _t2012 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t2013[] = {gem_v_d, gem_table_get(_t2012, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2013, 2));
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
    GemVal _t2014[] = {gem_v__match_11};
    GemVal _t2016;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2014, 1), gem_string("table")))) {
        _t2016 = gem_eq(gem_type_fn(NULL, _t2014, 1), gem_string("table"));
    } else {
        GemVal _t2015[] = {gem_v__match_11, gem_string("tag")};
        _t2016 = gem_has_key_fn(NULL, _t2015, 2);
    }
    GemVal _t2017;
    if (!gem_truthy(_t2016)) {
        _t2017 = _t2016;
    } else {
        _t2017 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t2019;
    if (!gem_truthy(_t2017)) {
        _t2019 = _t2017;
    } else {
        GemVal _t2018[] = {gem_v__match_11, gem_string("value")};
        _t2019 = gem_has_key_fn(NULL, _t2018, 2);
    }
    GemVal _t2021;
    if (!gem_truthy(_t2019)) {
        _t2021 = _t2019;
    } else {
        GemVal _t2020[] = {gem_v__match_11, gem_string("bindings")};
        _t2021 = gem_has_key_fn(NULL, _t2020, 2);
    }
    GemVal _t2023;
    if (!gem_truthy(_t2021)) {
        _t2023 = _t2021;
    } else {
        GemVal _t2022[] = {gem_v__match_11, gem_string("body")};
        _t2023 = gem_has_key_fn(NULL, _t2022, 2);
    }
    if (gem_truthy(_t2023)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 344 "compiler/main.gem"
    GemVal _t2024[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2025 = (*gem_v_collect_free_node);
        (void)(_t2025.fn(_t2025.env, _t2024, 3));
#line 345 "compiler/main.gem"
    GemVal _t2026 = gem_table_new();
    GemVal _t2027[] = {gem_v_defined, _t2026};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2027, 2);
#line 346 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 347 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 347 "compiler/main.gem"
    GemVal _t2028[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t2028, 1);
#line 347 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 347 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 347 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 348 "compiler/main.gem"
    GemVal _t2029[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2030 = (*gem_v_collect_free_node);
                (void)(_t2030.fn(_t2030.env, _t2029, 3));
#line 349 "compiler/main.gem"
    GemVal _t2031[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2031, 2))) {
#line 350 "compiler/main.gem"
    GemVal _t2032 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t2033[] = {gem_v_d, gem_table_get(_t2032, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2033, 2));
                }
            }

        }
        {
#line 354 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 354 "compiler/main.gem"
    GemVal _t2034[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t2034, 1);
#line 354 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 354 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 354 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 355 "compiler/main.gem"
    GemVal _t2035[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2036 = (*gem_v_collect_free_node);
                (void)(_t2036.fn(_t2036.env, _t2035, 3));
#line 356 "compiler/main.gem"
    GemVal _t2037[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2037, 2))) {
#line 357 "compiler/main.gem"
    GemVal _t2038 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2039[] = {gem_v_d, gem_table_get(_t2038, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2039, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2040[] = {gem_v__match_11};
    GemVal _t2042;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2040, 1), gem_string("table")))) {
        _t2042 = gem_eq(gem_type_fn(NULL, _t2040, 1), gem_string("table"));
    } else {
        GemVal _t2041[] = {gem_v__match_11, gem_string("tag")};
        _t2042 = gem_has_key_fn(NULL, _t2041, 2);
    }
    GemVal _t2043;
    if (!gem_truthy(_t2042)) {
        _t2043 = _t2042;
    } else {
        _t2043 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2045;
    if (!gem_truthy(_t2043)) {
        _t2045 = _t2043;
    } else {
        GemVal _t2044[] = {gem_v__match_11, gem_string("arms")};
        _t2045 = gem_has_key_fn(NULL, _t2044, 2);
    }
    GemVal _t2047;
    if (!gem_truthy(_t2045)) {
        _t2047 = _t2045;
    } else {
        GemVal _t2046[] = {gem_v__match_11, gem_string("after_ms")};
        _t2047 = gem_has_key_fn(NULL, _t2046, 2);
    }
    GemVal _t2049;
    if (!gem_truthy(_t2047)) {
        _t2049 = _t2047;
    } else {
        GemVal _t2048[] = {gem_v__match_11, gem_string("after_body")};
        _t2049 = gem_has_key_fn(NULL, _t2048, 2);
    }
    if (gem_truthy(_t2049)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 361 "compiler/main.gem"
    GemVal _t2050 = gem_table_new();
    GemVal _t2051[] = {gem_v_defined, _t2050};
        GemVal gem_v_recv_def = gem_fn__mod_codegen_set_union(NULL, _t2051, 2);
#line 362 "compiler/main.gem"
    GemVal _t2052 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2052, gem_string("recv_var")), GEM_NIL))) {
#line 363 "compiler/main.gem"
    GemVal _t2053 = gem_v_node;
    GemVal _t2054[] = {gem_v_recv_def, gem_table_get(_t2053, gem_string("recv_var"))};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2054, 2));
        }
#line 365 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 365 "compiler/main.gem"
    GemVal _t2055[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t2055, 1);
#line 365 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 365 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_24;
#line 365 "compiler/main.gem"
            gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 366 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 367 "compiler/main.gem"
    GemVal _t2056 = gem_v_arm;
    GemVal _t2057 = gem_table_get(_t2056, gem_string("pattern"));
    GemVal _t2058[] = {gem_table_get(_t2057, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t2059 = (*gem_v_collect_free_node);
            (void)(_t2059.fn(_t2059.env, _t2058, 3));
#line 368 "compiler/main.gem"
    GemVal _t2060 = gem_table_new();
    GemVal _t2061[] = {gem_v_recv_def, _t2060};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2061, 2);
#line 369 "compiler/main.gem"
    GemVal _t2062 = gem_v_arm;
    GemVal _t2063 = gem_table_get(_t2062, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2063, gem_string("bindings")), GEM_NIL))) {
#line 370 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 370 "compiler/main.gem"
    GemVal _t2064 = gem_v_arm;
    GemVal _t2065 = gem_table_get(_t2064, gem_string("pattern"));
    GemVal _t2066[] = {gem_table_get(_t2065, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t2066, 1);
#line 370 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 370 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 370 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 371 "compiler/main.gem"
    GemVal _t2067 = gem_v_arm;
    GemVal _t2068 = gem_table_get(_t2067, gem_string("pattern"));
    GemVal _t2069[] = {gem_table_get(gem_table_get(_t2068, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2070 = (*gem_v_collect_free_node);
                    (void)(_t2070.fn(_t2070.env, _t2069, 3));
#line 372 "compiler/main.gem"
    GemVal _t2071 = gem_v_arm;
    GemVal _t2072 = gem_table_get(_t2071, gem_string("pattern"));
    GemVal _t2073[] = {gem_table_get(gem_table_get(_t2072, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2073, 2))) {
#line 373 "compiler/main.gem"
    GemVal _t2074 = gem_v_arm;
    GemVal _t2075 = gem_table_get(_t2074, gem_string("pattern"));
    GemVal _t2076 = gem_table_get(gem_table_get(_t2075, gem_string("bindings")), gem_v_bi);
    GemVal _t2077[] = {gem_v_d, gem_table_get(_t2076, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2077, 2));
                    }
                }

            }
#line 377 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 377 "compiler/main.gem"
    GemVal _t2078 = gem_v_arm;
    GemVal _t2079[] = {gem_table_get(_t2078, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t2079, 1);
#line 377 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 377 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 377 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 378 "compiler/main.gem"
    GemVal _t2080 = gem_v_arm;
    GemVal _t2081[] = {gem_table_get(gem_table_get(_t2080, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2082 = (*gem_v_collect_free_node);
                (void)(_t2082.fn(_t2082.env, _t2081, 3));
#line 379 "compiler/main.gem"
    GemVal _t2083 = gem_v_arm;
    GemVal _t2084[] = {gem_table_get(gem_table_get(_t2083, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2084, 2))) {
#line 380 "compiler/main.gem"
    GemVal _t2085 = gem_v_arm;
    GemVal _t2086 = gem_table_get(gem_table_get(_t2085, gem_string("body")), gem_v_j);
    GemVal _t2087[] = {gem_v_d, gem_table_get(_t2086, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2087, 2));
                }
            }

        }

#line 384 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 385 "compiler/main.gem"
    GemVal _t2088[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t2089 = (*gem_v_collect_free_node);
            (void)(_t2089.fn(_t2089.env, _t2088, 3));
        }
#line 387 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 388 "compiler/main.gem"
    GemVal _t2090 = gem_table_new();
    GemVal _t2091[] = {gem_v_recv_def, _t2090};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2091, 2);
            {
#line 389 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 389 "compiler/main.gem"
    GemVal _t2092[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t2092, 1);
#line 389 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 389 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 389 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 390 "compiler/main.gem"
    GemVal _t2093[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2094 = (*gem_v_collect_free_node);
                    (void)(_t2094.fn(_t2094.env, _t2093, 3));
#line 391 "compiler/main.gem"
    GemVal _t2095[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2095, 2))) {
#line 392 "compiler/main.gem"
    GemVal _t2096 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t2097[] = {gem_v_d, gem_table_get(_t2096, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2097, 2));
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
    GemVal _t2098[] = {gem_v__match_11};
    GemVal _t2100;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2098, 1), gem_string("table")))) {
        _t2100 = gem_eq(gem_type_fn(NULL, _t2098, 1), gem_string("table"));
    } else {
        GemVal _t2099[] = {gem_v__match_11, gem_string("tag")};
        _t2100 = gem_has_key_fn(NULL, _t2099, 2);
    }
    GemVal _t2101;
    if (!gem_truthy(_t2100)) {
        _t2101 = _t2100;
    } else {
        _t2101 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2103;
    if (!gem_truthy(_t2101)) {
        _t2103 = _t2101;
    } else {
        GemVal _t2102[] = {gem_v__match_11, gem_string("entries")};
        _t2103 = gem_has_key_fn(NULL, _t2102, 2);
    }
    if (gem_truthy(_t2103)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 397 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 397 "compiler/main.gem"
    GemVal _t2104[] = {gem_v_entries};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t2104, 1);
#line 397 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 397 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 397 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 398 "compiler/main.gem"
    GemVal _t2105 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2106[] = {gem_table_get(_t2105, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t2107 = (*gem_v_collect_free_node);
                (void)(_t2107.fn(_t2107.env, _t2106, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2108[] = {gem_v__match_11};
    GemVal _t2110;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2108, 1), gem_string("table")))) {
        _t2110 = gem_eq(gem_type_fn(NULL, _t2108, 1), gem_string("table"));
    } else {
        GemVal _t2109[] = {gem_v__match_11, gem_string("tag")};
        _t2110 = gem_has_key_fn(NULL, _t2109, 2);
    }
    GemVal _t2111;
    if (!gem_truthy(_t2110)) {
        _t2111 = _t2110;
    } else {
        _t2111 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2113;
    if (!gem_truthy(_t2111)) {
        _t2113 = _t2111;
    } else {
        GemVal _t2112[] = {gem_v__match_11, gem_string("elements")};
        _t2113 = gem_has_key_fn(NULL, _t2112, 2);
    }
    if (gem_truthy(_t2113)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 401 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 401 "compiler/main.gem"
    GemVal _t2114[] = {gem_v_elements};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t2114, 1);
#line 401 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 401 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 401 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 402 "compiler/main.gem"
    GemVal _t2115[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t2116 = (*gem_v_collect_free_node);
                (void)(_t2116.fn(_t2116.env, _t2115, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2117[] = {gem_v__match_11};
    GemVal _t2119;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2117, 1), gem_string("table")))) {
        _t2119 = gem_eq(gem_type_fn(NULL, _t2117, 1), gem_string("table"));
    } else {
        GemVal _t2118[] = {gem_v__match_11, gem_string("tag")};
        _t2119 = gem_has_key_fn(NULL, _t2118, 2);
    }
    GemVal _t2120;
    if (!gem_truthy(_t2119)) {
        _t2120 = _t2119;
    } else {
        _t2120 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2122;
    if (!gem_truthy(_t2120)) {
        _t2122 = _t2120;
    } else {
        GemVal _t2121[] = {gem_v__match_11, gem_string("stmts")};
        _t2122 = gem_has_key_fn(NULL, _t2121, 2);
    }
    if (gem_truthy(_t2122)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 405 "compiler/main.gem"
    GemVal _t2123 = gem_table_new();
    GemVal _t2124[] = {gem_v_defined, _t2123};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2124, 2);
        {
#line 406 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 406 "compiler/main.gem"
    GemVal _t2125[] = {gem_v_stmts};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t2125, 1);
#line 406 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 406 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 406 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 407 "compiler/main.gem"
    GemVal _t2126[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2127 = (*gem_v_collect_free_node);
                (void)(_t2127.fn(_t2127.env, _t2126, 3));
#line 408 "compiler/main.gem"
    GemVal _t2128[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2128, 2))) {
#line 409 "compiler/main.gem"
    GemVal _t2129 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2130[] = {gem_v_d, gem_table_get(_t2129, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2130, 2));
                }
            }
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
        _t2134 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2136;
    if (!gem_truthy(_t2134)) {
        _t2136 = _t2134;
    } else {
        GemVal _t2135[] = {gem_v__match_11, gem_string("value")};
        _t2136 = gem_has_key_fn(NULL, _t2135, 2);
    }
    if (gem_truthy(_t2136)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 413 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 414 "compiler/main.gem"
    GemVal _t2137[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2138 = (*gem_v_collect_free_node);
            GemVal _t2139 = _t2138.fn(_t2138.env, _t2137, 3);
            gem_pop_frame();
            return _t2139;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 418 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 418 "compiler/main.gem"
    GemVal _t2140[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_len_29 = gem_len_fn(NULL, _t2140, 1);
#line 418 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 418 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_len_29))) break;
#line 418 "compiler/main.gem"
    GemVal _t2141[] = {gem_v__for_tbl_29, gem_v__for_i_29};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2141, 2);
#line 418 "compiler/main.gem"
    GemVal _t2142[] = {gem_v__for_tbl_29, gem_v__for_i_29};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2142, 2);
#line 418 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 419 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 420 "compiler/main.gem"
    GemVal _t2143[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2143, 1), gem_string("table")))) {
#line 421 "compiler/main.gem"
    GemVal _t2144[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2144, 1);
#line 422 "compiler/main.gem"
    GemVal _t2145[] = {gem_v_vks};
    GemVal _t2147;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2145, 1), gem_int(0)))) {
        _t2147 = gem_gt(gem_len_fn(NULL, _t2145, 1), gem_int(0));
    } else {
        GemVal _t2146[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2147 = gem_eq(gem_type_fn(NULL, _t2146, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2147)) {
#line 423 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 424 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2148[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2148, 1)))) break;
#line 425 "compiler/main.gem"
    GemVal _t2149[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2149, 1), gem_string("table")))) {
#line 426 "compiler/main.gem"
    GemVal _t2150[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t2151 = (*gem_v_collect_free_node);
                                    (void)(_t2151.fn(_t2151.env, _t2150, 3));
                                }
#line 428 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 431 "compiler/main.gem"
    GemVal _t2152[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t2153 = (*gem_v_collect_free_node);
                            (void)(_t2153.fn(_t2153.env, _t2152, 3));
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
#line 440 "compiler/main.gem"
    GemVal _t2155 = gem_table_new();
    GemVal gem_v_free = _t2155;
#line 441 "compiler/main.gem"
    GemVal _t2156 = gem_table_new();
    GemVal _t2157[] = {gem_v_defined, _t2156};
    GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2157, 2);
#line 442 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 442 "compiler/main.gem"
    GemVal _t2158[] = {gem_v_stmts};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t2158, 1);
#line 442 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 442 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 442 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 443 "compiler/main.gem"
    GemVal _t2159[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2160 = (*gem_v_collect_free_node);
        (void)(_t2160.fn(_t2160.env, _t2159, 3));
#line 444 "compiler/main.gem"
    GemVal _t2161[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2161, 2))) {
#line 445 "compiler/main.gem"
    GemVal _t2162 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2163[] = {gem_v_d, gem_table_get(_t2162, gem_string("name"))};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2163, 2));
        }
#line 447 "compiler/main.gem"
    GemVal _t2164[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2164, 2));
    }

    GemVal _t2165 = gem_v_free;
    gem_pop_frame();
    return _t2165;
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
#line 458 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 459 "compiler/main.gem"
        GemVal _t2167 = GEM_NIL;
        gem_pop_frame();
        return _t2167;
    }
#line 461 "compiler/main.gem"
    GemVal _t2168[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2168, 1), gem_string("table")))) {
#line 462 "compiler/main.gem"
        GemVal _t2169 = GEM_NIL;
        gem_pop_frame();
        return _t2169;
    }
#line 464 "compiler/main.gem"
    GemVal _t2170 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t2170, gem_string("tag")), GEM_NIL))) {
#line 465 "compiler/main.gem"
        GemVal _t2171 = GEM_NIL;
        gem_pop_frame();
        return _t2171;
    }
#line 468 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
    GemVal _t2172[] = {gem_v__match_31};
    GemVal _t2174;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2172, 1), gem_string("table")))) {
        _t2174 = gem_eq(gem_type_fn(NULL, _t2172, 1), gem_string("table"));
    } else {
        GemVal _t2173[] = {gem_v__match_31, gem_string("tag")};
        _t2174 = gem_has_key_fn(NULL, _t2173, 2);
    }
    GemVal _t2175;
    if (!gem_truthy(_t2174)) {
        _t2175 = _t2174;
    } else {
        _t2175 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t2177;
    if (!gem_truthy(_t2175)) {
        _t2177 = _t2175;
    } else {
        GemVal _t2176[] = {gem_v__match_31, gem_string("params")};
        _t2177 = gem_has_key_fn(NULL, _t2176, 2);
    }
    GemVal _t2179;
    if (!gem_truthy(_t2177)) {
        _t2179 = _t2177;
    } else {
        GemVal _t2178[] = {gem_v__match_31, gem_string("rest_param")};
        _t2179 = gem_has_key_fn(NULL, _t2178, 2);
    }
    GemVal _t2181;
    if (!gem_truthy(_t2179)) {
        _t2181 = _t2179;
    } else {
        GemVal _t2180[] = {gem_v__match_31, gem_string("body")};
        _t2181 = gem_has_key_fn(NULL, _t2180, 2);
    }
    if (gem_truthy(_t2181)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_31, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_31, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 470 "compiler/main.gem"
    GemVal _t2182[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2182, 1);
#line 471 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 472 "compiler/main.gem"
    GemVal _t2183[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2183, 2));
        }
#line 479 "compiler/main.gem"
    GemVal _t2184[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t2185 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t2185.fn(_t2185.env, _t2184, 2);
#line 480 "compiler/main.gem"
    GemVal _t2186[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn__mod_codegen_set_intersect(NULL, _t2186, 2);
        {
#line 481 "compiler/main.gem"
            GemVal gem_v__for_tbl_32 = gem_v_inter;
#line 481 "compiler/main.gem"
    GemVal _t2187[] = {gem_v__for_tbl_32};
            GemVal gem_v__for_len_32 = gem_len_fn(NULL, _t2187, 1);
#line 481 "compiler/main.gem"
            GemVal gem_v__for_i_32 = gem_int(0);
#line 481 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_len_32))) break;
#line 481 "compiler/main.gem"
    GemVal _t2188[] = {gem_v__for_tbl_32, gem_v__for_i_32};
                GemVal gem_v_ik = gem_table_key_at_fn(NULL, _t2188, 2);
#line 481 "compiler/main.gem"
    GemVal _t2189[] = {gem_v__for_tbl_32, gem_v__for_i_32};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t2189, 2);
#line 481 "compiler/main.gem"
                gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 482 "compiler/main.gem"
    GemVal _t2190[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2190, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2191[] = {gem_v__match_31};
    GemVal _t2193;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2191, 1), gem_string("table")))) {
        _t2193 = gem_eq(gem_type_fn(NULL, _t2191, 1), gem_string("table"));
    } else {
        GemVal _t2192[] = {gem_v__match_31, gem_string("tag")};
        _t2193 = gem_has_key_fn(NULL, _t2192, 2);
    }
    GemVal _t2194;
    if (!gem_truthy(_t2193)) {
        _t2194 = _t2193;
    } else {
        _t2194 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2194)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t2195[] = {gem_v__match_31};
    GemVal _t2197;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2195, 1), gem_string("table")))) {
        _t2197 = gem_eq(gem_type_fn(NULL, _t2195, 1), gem_string("table"));
    } else {
        GemVal _t2196[] = {gem_v__match_31, gem_string("tag")};
        _t2197 = gem_has_key_fn(NULL, _t2196, 2);
    }
    GemVal _t2198;
    if (!gem_truthy(_t2197)) {
        _t2198 = _t2197;
    } else {
        _t2198 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2200;
    if (!gem_truthy(_t2198)) {
        _t2200 = _t2198;
    } else {
        GemVal _t2199[] = {gem_v__match_31, gem_string("cond")};
        _t2200 = gem_has_key_fn(NULL, _t2199, 2);
    }
    GemVal _t2202;
    if (!gem_truthy(_t2200)) {
        _t2202 = _t2200;
    } else {
        GemVal _t2201[] = {gem_v__match_31, gem_string("then")};
        _t2202 = gem_has_key_fn(NULL, _t2201, 2);
    }
    GemVal _t2204;
    if (!gem_truthy(_t2202)) {
        _t2204 = _t2202;
    } else {
        GemVal _t2203[] = {gem_v__match_31, gem_string("else")};
        _t2204 = gem_has_key_fn(NULL, _t2203, 2);
    }
    if (gem_truthy(_t2204)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_31, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 487 "compiler/main.gem"
    GemVal _t2205[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2206 = (*gem_v_walk_captures_node);
        (void)(_t2206.fn(_t2206.env, _t2205, 3));
#line 488 "compiler/main.gem"
    GemVal _t2207[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t2208 = (*gem_v_walk_captures);
        (void)(_t2208.fn(_t2208.env, _t2207, 3));
#line 489 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 490 "compiler/main.gem"
    GemVal _t2209[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2210 = (*gem_v_walk_captures);
            GemVal _t2211 = _t2210.fn(_t2210.env, _t2209, 3);
            gem_pop_frame();
            return _t2211;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2212[] = {gem_v__match_31};
    GemVal _t2214;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2212, 1), gem_string("table")))) {
        _t2214 = gem_eq(gem_type_fn(NULL, _t2212, 1), gem_string("table"));
    } else {
        GemVal _t2213[] = {gem_v__match_31, gem_string("tag")};
        _t2214 = gem_has_key_fn(NULL, _t2213, 2);
    }
    GemVal _t2215;
    if (!gem_truthy(_t2214)) {
        _t2215 = _t2214;
    } else {
        _t2215 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("while"));
    }
    GemVal _t2217;
    if (!gem_truthy(_t2215)) {
        _t2217 = _t2215;
    } else {
        GemVal _t2216[] = {gem_v__match_31, gem_string("cond")};
        _t2217 = gem_has_key_fn(NULL, _t2216, 2);
    }
    GemVal _t2219;
    if (!gem_truthy(_t2217)) {
        _t2219 = _t2217;
    } else {
        GemVal _t2218[] = {gem_v__match_31, gem_string("body")};
        _t2219 = gem_has_key_fn(NULL, _t2218, 2);
    }
    if (gem_truthy(_t2219)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 493 "compiler/main.gem"
    GemVal _t2220[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2221 = (*gem_v_walk_captures_node);
        (void)(_t2221.fn(_t2221.env, _t2220, 3));
#line 494 "compiler/main.gem"
    GemVal _t2222[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2223 = (*gem_v_walk_captures);
        GemVal _t2224 = _t2223.fn(_t2223.env, _t2222, 3);
        gem_pop_frame();
        return _t2224;
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
        _t2228 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2230;
    if (!gem_truthy(_t2228)) {
        _t2230 = _t2228;
    } else {
        GemVal _t2229[] = {gem_v__match_31, gem_string("target")};
        _t2230 = gem_has_key_fn(NULL, _t2229, 2);
    }
    GemVal _t2232;
    if (!gem_truthy(_t2230)) {
        _t2232 = _t2230;
    } else {
        GemVal _t2231[] = {gem_v__match_31, gem_string("whens")};
        _t2232 = gem_has_key_fn(NULL, _t2231, 2);
    }
    GemVal _t2234;
    if (!gem_truthy(_t2232)) {
        _t2234 = _t2232;
    } else {
        GemVal _t2233[] = {gem_v__match_31, gem_string("else")};
        _t2234 = gem_has_key_fn(NULL, _t2233, 2);
    }
    if (gem_truthy(_t2234)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_31, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_31, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 496 "compiler/main.gem"
    GemVal _t2235[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2236 = (*gem_v_walk_captures_node);
        (void)(_t2236.fn(_t2236.env, _t2235, 3));
#line 497 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 497 "compiler/main.gem"
    GemVal _t2237[] = {gem_v_whens};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t2237, 1);
#line 497 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 497 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 497 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 498 "compiler/main.gem"
    GemVal _t2238 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2239[] = {gem_table_get(_t2238, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2240 = (*gem_v_walk_captures_node);
            (void)(_t2240.fn(_t2240.env, _t2239, 3));
#line 499 "compiler/main.gem"
    GemVal _t2241 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t2241, gem_string("bindings")), GEM_NIL))) {
#line 500 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 500 "compiler/main.gem"
    GemVal _t2242 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2243[] = {gem_table_get(_t2242, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t2243, 1);
#line 500 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 500 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 500 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 501 "compiler/main.gem"
    GemVal _t2244 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2245[] = {gem_table_get(gem_table_get(_t2244, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2246 = (*gem_v_walk_captures_node);
                    (void)(_t2246.fn(_t2246.env, _t2245, 3));
                }

            }
#line 504 "compiler/main.gem"
    GemVal _t2247 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2248[] = {gem_table_get(_t2247, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2249 = (*gem_v_walk_captures);
            (void)(_t2249.fn(_t2249.env, _t2248, 3));
        }

#line 506 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 507 "compiler/main.gem"
    GemVal _t2250[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2251 = (*gem_v_walk_captures);
            GemVal _t2252 = _t2251.fn(_t2251.env, _t2250, 3);
            gem_pop_frame();
            return _t2252;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2253[] = {gem_v__match_31};
    GemVal _t2255;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2253, 1), gem_string("table")))) {
        _t2255 = gem_eq(gem_type_fn(NULL, _t2253, 1), gem_string("table"));
    } else {
        GemVal _t2254[] = {gem_v__match_31, gem_string("tag")};
        _t2255 = gem_has_key_fn(NULL, _t2254, 2);
    }
    GemVal _t2256;
    if (!gem_truthy(_t2255)) {
        _t2256 = _t2255;
    } else {
        _t2256 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2258;
    if (!gem_truthy(_t2256)) {
        _t2258 = _t2256;
    } else {
        GemVal _t2257[] = {gem_v__match_31, gem_string("arms")};
        _t2258 = gem_has_key_fn(NULL, _t2257, 2);
    }
    GemVal _t2260;
    if (!gem_truthy(_t2258)) {
        _t2260 = _t2258;
    } else {
        GemVal _t2259[] = {gem_v__match_31, gem_string("after_ms")};
        _t2260 = gem_has_key_fn(NULL, _t2259, 2);
    }
    GemVal _t2262;
    if (!gem_truthy(_t2260)) {
        _t2262 = _t2260;
    } else {
        GemVal _t2261[] = {gem_v__match_31, gem_string("after_body")};
        _t2262 = gem_has_key_fn(NULL, _t2261, 2);
    }
    if (gem_truthy(_t2262)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_31, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_31, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_31, gem_string("after_body"));
#line 510 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 510 "compiler/main.gem"
    GemVal _t2263[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2263, 1);
#line 510 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 510 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 510 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 511 "compiler/main.gem"
    GemVal _t2264 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2265 = gem_table_get(_t2264, gem_string("pattern"));
    GemVal _t2266[] = {gem_table_get(_t2265, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2267 = (*gem_v_walk_captures_node);
            (void)(_t2267.fn(_t2267.env, _t2266, 3));
#line 512 "compiler/main.gem"
    GemVal _t2268 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2269 = gem_table_get(_t2268, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2269, gem_string("bindings")), GEM_NIL))) {
#line 513 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 513 "compiler/main.gem"
    GemVal _t2270 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2271 = gem_table_get(_t2270, gem_string("pattern"));
    GemVal _t2272[] = {gem_table_get(_t2271, gem_string("bindings"))};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2272, 1);
#line 513 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 513 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 513 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 514 "compiler/main.gem"
    GemVal _t2273 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2274 = gem_table_get(_t2273, gem_string("pattern"));
    GemVal _t2275[] = {gem_table_get(gem_table_get(_t2274, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2276 = (*gem_v_walk_captures_node);
                    (void)(_t2276.fn(_t2276.env, _t2275, 3));
                }

            }
#line 517 "compiler/main.gem"
    GemVal _t2277 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2278[] = {gem_table_get(_t2277, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2279 = (*gem_v_walk_captures);
            (void)(_t2279.fn(_t2279.env, _t2278, 3));
        }

#line 519 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 520 "compiler/main.gem"
    GemVal _t2280[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2281 = (*gem_v_walk_captures_node);
            (void)(_t2281.fn(_t2281.env, _t2280, 3));
        }
#line 522 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 523 "compiler/main.gem"
    GemVal _t2282[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2283 = (*gem_v_walk_captures);
            GemVal _t2284 = _t2283.fn(_t2283.env, _t2282, 3);
            gem_pop_frame();
            return _t2284;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2285[] = {gem_v__match_31};
    GemVal _t2287;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2285, 1), gem_string("table")))) {
        _t2287 = gem_eq(gem_type_fn(NULL, _t2285, 1), gem_string("table"));
    } else {
        GemVal _t2286[] = {gem_v__match_31, gem_string("tag")};
        _t2287 = gem_has_key_fn(NULL, _t2286, 2);
    }
    GemVal _t2288;
    if (!gem_truthy(_t2287)) {
        _t2288 = _t2287;
    } else {
        _t2288 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2290;
    if (!gem_truthy(_t2288)) {
        _t2290 = _t2288;
    } else {
        GemVal _t2289[] = {gem_v__match_31, gem_string("entries")};
        _t2290 = gem_has_key_fn(NULL, _t2289, 2);
    }
    if (gem_truthy(_t2290)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_31, gem_string("entries"));
        {
#line 526 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 526 "compiler/main.gem"
    GemVal _t2291[] = {gem_v_entries};
            GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2291, 1);
#line 526 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 526 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 526 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 527 "compiler/main.gem"
    GemVal _t2292 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2293[] = {gem_table_get(_t2292, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2294 = (*gem_v_walk_captures_node);
                (void)(_t2294.fn(_t2294.env, _t2293, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2295[] = {gem_v__match_31};
    GemVal _t2297;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2295, 1), gem_string("table")))) {
        _t2297 = gem_eq(gem_type_fn(NULL, _t2295, 1), gem_string("table"));
    } else {
        GemVal _t2296[] = {gem_v__match_31, gem_string("tag")};
        _t2297 = gem_has_key_fn(NULL, _t2296, 2);
    }
    GemVal _t2298;
    if (!gem_truthy(_t2297)) {
        _t2298 = _t2297;
    } else {
        _t2298 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2300;
    if (!gem_truthy(_t2298)) {
        _t2300 = _t2298;
    } else {
        GemVal _t2299[] = {gem_v__match_31, gem_string("elements")};
        _t2300 = gem_has_key_fn(NULL, _t2299, 2);
    }
    if (gem_truthy(_t2300)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_31, gem_string("elements"));
        {
#line 530 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 530 "compiler/main.gem"
    GemVal _t2301[] = {gem_v_elements};
            GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2301, 1);
#line 530 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 530 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 530 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 531 "compiler/main.gem"
    GemVal _t2302[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2303 = (*gem_v_walk_captures_node);
                (void)(_t2303.fn(_t2303.env, _t2302, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2304[] = {gem_v__match_31};
    GemVal _t2306;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2304, 1), gem_string("table")))) {
        _t2306 = gem_eq(gem_type_fn(NULL, _t2304, 1), gem_string("table"));
    } else {
        GemVal _t2305[] = {gem_v__match_31, gem_string("tag")};
        _t2306 = gem_has_key_fn(NULL, _t2305, 2);
    }
    GemVal _t2307;
    if (!gem_truthy(_t2306)) {
        _t2307 = _t2306;
    } else {
        _t2307 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2309;
    if (!gem_truthy(_t2307)) {
        _t2309 = _t2307;
    } else {
        GemVal _t2308[] = {gem_v__match_31, gem_string("stmts")};
        _t2309 = gem_has_key_fn(NULL, _t2308, 2);
    }
    if (gem_truthy(_t2309)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_31, gem_string("stmts"));
#line 534 "compiler/main.gem"
    GemVal _t2310[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2311 = (*gem_v_walk_captures);
        GemVal _t2312 = _t2311.fn(_t2311.env, _t2310, 3);
        gem_pop_frame();
        return _t2312;
    } else {
    GemVal _t2313[] = {gem_v__match_31};
    GemVal _t2315;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2313, 1), gem_string("table")))) {
        _t2315 = gem_eq(gem_type_fn(NULL, _t2313, 1), gem_string("table"));
    } else {
        GemVal _t2314[] = {gem_v__match_31, gem_string("tag")};
        _t2315 = gem_has_key_fn(NULL, _t2314, 2);
    }
    GemVal _t2316;
    if (!gem_truthy(_t2315)) {
        _t2316 = _t2315;
    } else {
        _t2316 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2318;
    if (!gem_truthy(_t2316)) {
        _t2318 = _t2316;
    } else {
        GemVal _t2317[] = {gem_v__match_31, gem_string("value")};
        _t2318 = gem_has_key_fn(NULL, _t2317, 2);
    }
    if (gem_truthy(_t2318)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 536 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 537 "compiler/main.gem"
    GemVal _t2319[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2320 = (*gem_v_walk_captures_node);
            GemVal _t2321 = _t2320.fn(_t2320.env, _t2319, 3);
            gem_pop_frame();
            return _t2321;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2322[] = {gem_v__match_31};
    GemVal _t2324;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2322, 1), gem_string("table")))) {
        _t2324 = gem_eq(gem_type_fn(NULL, _t2322, 1), gem_string("table"));
    } else {
        GemVal _t2323[] = {gem_v__match_31, gem_string("tag")};
        _t2324 = gem_has_key_fn(NULL, _t2323, 2);
    }
    GemVal _t2325;
    if (!gem_truthy(_t2324)) {
        _t2325 = _t2324;
    } else {
        _t2325 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2327;
    if (!gem_truthy(_t2325)) {
        _t2327 = _t2325;
    } else {
        GemVal _t2326[] = {gem_v__match_31, gem_string("value")};
        _t2327 = gem_has_key_fn(NULL, _t2326, 2);
    }
    if (gem_truthy(_t2327)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 540 "compiler/main.gem"
    GemVal _t2328[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2329 = (*gem_v_walk_captures_node);
        GemVal _t2330 = _t2329.fn(_t2329.env, _t2328, 3);
        gem_pop_frame();
        return _t2330;
    } else {
    GemVal _t2331[] = {gem_v__match_31};
    GemVal _t2333;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2331, 1), gem_string("table")))) {
        _t2333 = gem_eq(gem_type_fn(NULL, _t2331, 1), gem_string("table"));
    } else {
        GemVal _t2332[] = {gem_v__match_31, gem_string("tag")};
        _t2333 = gem_has_key_fn(NULL, _t2332, 2);
    }
    GemVal _t2334;
    if (!gem_truthy(_t2333)) {
        _t2334 = _t2333;
    } else {
        _t2334 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2336;
    if (!gem_truthy(_t2334)) {
        _t2336 = _t2334;
    } else {
        GemVal _t2335[] = {gem_v__match_31, gem_string("value")};
        _t2336 = gem_has_key_fn(NULL, _t2335, 2);
    }
    if (gem_truthy(_t2336)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 542 "compiler/main.gem"
    GemVal _t2337[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2338 = (*gem_v_walk_captures_node);
        GemVal _t2339 = _t2338.fn(_t2338.env, _t2337, 3);
        gem_pop_frame();
        return _t2339;
    } else {
        {
#line 545 "compiler/main.gem"
            GemVal gem_v__for_tbl_39 = gem_v_node;
#line 545 "compiler/main.gem"
    GemVal _t2340[] = {gem_v__for_tbl_39};
            GemVal gem_v__for_len_39 = gem_len_fn(NULL, _t2340, 1);
#line 545 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 545 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_len_39))) break;
#line 545 "compiler/main.gem"
    GemVal _t2341[] = {gem_v__for_tbl_39, gem_v__for_i_39};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2341, 2);
#line 545 "compiler/main.gem"
    GemVal _t2342[] = {gem_v__for_tbl_39, gem_v__for_i_39};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2342, 2);
#line 545 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 546 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 547 "compiler/main.gem"
    GemVal _t2343[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2343, 1), gem_string("table")))) {
#line 548 "compiler/main.gem"
    GemVal _t2344[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2344, 1);
#line 549 "compiler/main.gem"
    GemVal _t2345[] = {gem_v_vks};
    GemVal _t2347;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2345, 1), gem_int(0)))) {
        _t2347 = gem_gt(gem_len_fn(NULL, _t2345, 1), gem_int(0));
    } else {
        GemVal _t2346[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2347 = gem_eq(gem_type_fn(NULL, _t2346, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2347)) {
#line 550 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 551 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2348[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2348, 1)))) break;
#line 552 "compiler/main.gem"
    GemVal _t2349[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2349, 1), gem_string("table")))) {
#line 553 "compiler/main.gem"
    GemVal _t2350[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2351 = (*gem_v_walk_captures_node);
                                    (void)(_t2351.fn(_t2351.env, _t2350, 3));
                                }
#line 555 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 558 "compiler/main.gem"
    GemVal _t2352[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2353 = (*gem_v_walk_captures_node);
                            (void)(_t2353.fn(_t2353.env, _t2352, 3));
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
#line 567 "compiler/main.gem"
    GemVal _t2355 = gem_table_new();
    GemVal _t2356[] = {gem_v_scope_vars, _t2355};
    GemVal gem_v_sv = gem_fn__mod_codegen_set_union(NULL, _t2356, 2);
    {
#line 568 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 568 "compiler/main.gem"
    GemVal _t2357[] = {gem_v_stmts};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2357, 1);
#line 568 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 568 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 568 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 569 "compiler/main.gem"
    GemVal _t2358[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2359 = (*gem_v_walk_captures_node);
            (void)(_t2359.fn(_t2359.env, _t2358, 3));
#line 570 "compiler/main.gem"
    GemVal _t2360[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2360, 2))) {
#line 571 "compiler/main.gem"
    GemVal _t2361 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2362[] = {gem_v_sv, gem_table_get(_t2361, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2362, 2));
            }
#line 573 "compiler/main.gem"
    GemVal _t2363[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2363, 2));
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
#line 578 "compiler/main.gem"
    GemVal _t2365 = gem_table_new();
    GemVal gem_v_captured = _t2365;
#line 579 "compiler/main.gem"
    GemVal _t2366[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2367 = (*gem_v_walk_captures);
    (void)(_t2367.fn(_t2367.env, _t2366, 3));
    GemVal _t2368 = gem_v_captured;
    gem_pop_frame();
    return _t2368;
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
#line 610 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 611 "compiler/main.gem"
        GemVal _t2370 = gem_bool(0);
        gem_pop_frame();
        return _t2370;
    }
#line 613 "compiler/main.gem"
    GemVal _t2371[] = {gem_v_node};
    GemVal _t2373;
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2371, 1), gem_string("table")))) {
        _t2373 = gem_neq(gem_type_fn(NULL, _t2371, 1), gem_string("table"));
    } else {
        GemVal _t2372 = gem_v_node;
        _t2373 = gem_eq(gem_table_get(_t2372, gem_string("tag")), GEM_NIL);
    }
    if (gem_truthy(_t2373)) {
#line 614 "compiler/main.gem"
        GemVal _t2374 = gem_bool(0);
        gem_pop_frame();
        return _t2374;
    }
#line 616 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2375[] = {gem_v__match_41};
    GemVal _t2377;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2375, 1), gem_string("table")))) {
        _t2377 = gem_eq(gem_type_fn(NULL, _t2375, 1), gem_string("table"));
    } else {
        GemVal _t2376[] = {gem_v__match_41, gem_string("tag")};
        _t2377 = gem_has_key_fn(NULL, _t2376, 2);
    }
    GemVal _t2378;
    if (!gem_truthy(_t2377)) {
        _t2378 = _t2377;
    } else {
        _t2378 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    GemVal _t2380;
    if (!gem_truthy(_t2378)) {
        _t2380 = _t2378;
    } else {
        GemVal _t2379[] = {gem_v__match_41, gem_string("func")};
        _t2380 = gem_has_key_fn(NULL, _t2379, 2);
    }
    if (gem_truthy(_t2380)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_func = gem_table_get(gem_v__match_41, gem_string("func"));
#line 618 "compiler/main.gem"
    GemVal _t2381[] = {gem_v_func, gem_string("var")};
    GemVal _t2383;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2381, 2))) {
        _t2383 = gem_fn__mod_codegen_is_node(NULL, _t2381, 2);
    } else {
        GemVal _t2382 = gem_v_func;
        _t2383 = gem_eq(gem_table_get(_t2382, gem_string("name")), gem_v_fn_name);
    }
        GemVal _t2384 = _t2383;
        gem_pop_frame();
        return _t2384;
    } else {
    GemVal _t2385[] = {gem_v__match_41};
    GemVal _t2387;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2385, 1), gem_string("table")))) {
        _t2387 = gem_eq(gem_type_fn(NULL, _t2385, 1), gem_string("table"));
    } else {
        GemVal _t2386[] = {gem_v__match_41, gem_string("tag")};
        _t2387 = gem_has_key_fn(NULL, _t2386, 2);
    }
    GemVal _t2388;
    if (!gem_truthy(_t2387)) {
        _t2388 = _t2387;
    } else {
        _t2388 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2390;
    if (!gem_truthy(_t2388)) {
        _t2390 = _t2388;
    } else {
        GemVal _t2389[] = {gem_v__match_41, gem_string("value")};
        _t2390 = gem_has_key_fn(NULL, _t2389, 2);
    }
    if (gem_truthy(_t2390)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 620 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 621 "compiler/main.gem"
    GemVal _t2391[] = {gem_v_fn_name, gem_v_value};
    GemVal _t2392 = (*gem_v_is_self_tail_call);
            GemVal _t2393 = _t2392.fn(_t2392.env, _t2391, 2);
            gem_pop_frame();
            return _t2393;
        }
#line 623 "compiler/main.gem"
        GemVal _t2394 = gem_bool(0);
        gem_pop_frame();
        return _t2394;
    } else {
    GemVal _t2395[] = {gem_v__match_41};
    GemVal _t2397;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2395, 1), gem_string("table")))) {
        _t2397 = gem_eq(gem_type_fn(NULL, _t2395, 1), gem_string("table"));
    } else {
        GemVal _t2396[] = {gem_v__match_41, gem_string("tag")};
        _t2397 = gem_has_key_fn(NULL, _t2396, 2);
    }
    GemVal _t2398;
    if (!gem_truthy(_t2397)) {
        _t2398 = _t2397;
    } else {
        _t2398 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2400;
    if (!gem_truthy(_t2398)) {
        _t2400 = _t2398;
    } else {
        GemVal _t2399[] = {gem_v__match_41, gem_string("then")};
        _t2400 = gem_has_key_fn(NULL, _t2399, 2);
    }
    GemVal _t2402;
    if (!gem_truthy(_t2400)) {
        _t2402 = _t2400;
    } else {
        GemVal _t2401[] = {gem_v__match_41, gem_string("else")};
        _t2402 = gem_has_key_fn(NULL, _t2401, 2);
    }
    if (gem_truthy(_t2402)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_then_body = gem_table_get(gem_v__match_41, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 625 "compiler/main.gem"
    GemVal _t2403[] = {gem_v_fn_name, gem_v_then_body};
    GemVal _t2404 = (*gem_v_has_self_tail_call);
        if (gem_truthy(_t2404.fn(_t2404.env, _t2403, 2))) {
#line 626 "compiler/main.gem"
            GemVal _t2405 = gem_bool(1);
            gem_pop_frame();
            return _t2405;
        }
#line 628 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 629 "compiler/main.gem"
    GemVal _t2406[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2407 = (*gem_v_has_self_tail_call);
            GemVal _t2408 = _t2407.fn(_t2407.env, _t2406, 2);
            gem_pop_frame();
            return _t2408;
        }
#line 631 "compiler/main.gem"
        GemVal _t2409 = gem_bool(0);
        gem_pop_frame();
        return _t2409;
    } else {
    GemVal _t2410[] = {gem_v__match_41};
    GemVal _t2412;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2410, 1), gem_string("table")))) {
        _t2412 = gem_eq(gem_type_fn(NULL, _t2410, 1), gem_string("table"));
    } else {
        GemVal _t2411[] = {gem_v__match_41, gem_string("tag")};
        _t2412 = gem_has_key_fn(NULL, _t2411, 2);
    }
    GemVal _t2413;
    if (!gem_truthy(_t2412)) {
        _t2413 = _t2412;
    } else {
        _t2413 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2415;
    if (!gem_truthy(_t2413)) {
        _t2415 = _t2413;
    } else {
        GemVal _t2414[] = {gem_v__match_41, gem_string("whens")};
        _t2415 = gem_has_key_fn(NULL, _t2414, 2);
    }
    GemVal _t2417;
    if (!gem_truthy(_t2415)) {
        _t2417 = _t2415;
    } else {
        GemVal _t2416[] = {gem_v__match_41, gem_string("else")};
        _t2417 = gem_has_key_fn(NULL, _t2416, 2);
    }
    if (gem_truthy(_t2417)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_41, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 633 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 633 "compiler/main.gem"
    GemVal _t2418[] = {gem_v_whens};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2418, 1);
#line 633 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 633 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 633 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 634 "compiler/main.gem"
    GemVal _t2419 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2420[] = {gem_v_fn_name, gem_table_get(_t2419, gem_string("body"))};
    GemVal _t2421 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2421.fn(_t2421.env, _t2420, 2))) {
#line 635 "compiler/main.gem"
                GemVal _t2422 = gem_bool(1);
                gem_pop_frame();
                return _t2422;
            }
        }

#line 638 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 639 "compiler/main.gem"
    GemVal _t2423[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2424 = (*gem_v_has_self_tail_call);
            GemVal _t2425 = _t2424.fn(_t2424.env, _t2423, 2);
            gem_pop_frame();
            return _t2425;
        }
#line 641 "compiler/main.gem"
        GemVal _t2426 = gem_bool(0);
        gem_pop_frame();
        return _t2426;
    } else {
    GemVal _t2427[] = {gem_v__match_41};
    GemVal _t2429;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2427, 1), gem_string("table")))) {
        _t2429 = gem_eq(gem_type_fn(NULL, _t2427, 1), gem_string("table"));
    } else {
        GemVal _t2428[] = {gem_v__match_41, gem_string("tag")};
        _t2429 = gem_has_key_fn(NULL, _t2428, 2);
    }
    GemVal _t2430;
    if (!gem_truthy(_t2429)) {
        _t2430 = _t2429;
    } else {
        _t2430 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2432;
    if (!gem_truthy(_t2430)) {
        _t2432 = _t2430;
    } else {
        GemVal _t2431[] = {gem_v__match_41, gem_string("arms")};
        _t2432 = gem_has_key_fn(NULL, _t2431, 2);
    }
    GemVal _t2434;
    if (!gem_truthy(_t2432)) {
        _t2434 = _t2432;
    } else {
        GemVal _t2433[] = {gem_v__match_41, gem_string("after_body")};
        _t2434 = gem_has_key_fn(NULL, _t2433, 2);
    }
    if (gem_truthy(_t2434)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_arms = gem_table_get(gem_v__match_41, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_after_body = gem_table_get(gem_v__match_41, gem_string("after_body"));
#line 643 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(0);
#line 643 "compiler/main.gem"
    GemVal _t2435[] = {gem_v_arms};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2435, 1);
#line 643 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 643 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 643 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 644 "compiler/main.gem"
    GemVal _t2436 = gem_table_get(gem_v_arms, gem_v_i);
    GemVal _t2437[] = {gem_v_fn_name, gem_table_get(_t2436, gem_string("body"))};
    GemVal _t2438 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2438.fn(_t2438.env, _t2437, 2))) {
#line 645 "compiler/main.gem"
                GemVal _t2439 = gem_bool(1);
                gem_pop_frame();
                return _t2439;
            }
        }

#line 648 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 649 "compiler/main.gem"
    GemVal _t2440[] = {gem_v_fn_name, gem_v_after_body};
    GemVal _t2441 = (*gem_v_has_self_tail_call);
            GemVal _t2442 = _t2441.fn(_t2441.env, _t2440, 2);
            gem_pop_frame();
            return _t2442;
        }
#line 651 "compiler/main.gem"
        GemVal _t2443 = gem_bool(0);
        gem_pop_frame();
        return _t2443;
    } else {
    GemVal _t2444[] = {gem_v__match_41};
    GemVal _t2446;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2444, 1), gem_string("table")))) {
        _t2446 = gem_eq(gem_type_fn(NULL, _t2444, 1), gem_string("table"));
    } else {
        GemVal _t2445[] = {gem_v__match_41, gem_string("tag")};
        _t2446 = gem_has_key_fn(NULL, _t2445, 2);
    }
    GemVal _t2447;
    if (!gem_truthy(_t2446)) {
        _t2447 = _t2446;
    } else {
        _t2447 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2449;
    if (!gem_truthy(_t2447)) {
        _t2449 = _t2447;
    } else {
        GemVal _t2448[] = {gem_v__match_41, gem_string("stmts")};
        _t2449 = gem_has_key_fn(NULL, _t2448, 2);
    }
    if (gem_truthy(_t2449)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_41, gem_string("stmts"));
#line 653 "compiler/main.gem"
    GemVal _t2450[] = {gem_v_fn_name, gem_v_stmts};
    GemVal _t2451 = (*gem_v_has_self_tail_call);
        GemVal _t2452 = _t2451.fn(_t2451.env, _t2450, 2);
        gem_pop_frame();
        return _t2452;
    }
    }
    }
    }
    }
    }
#line 655 "compiler/main.gem"
    GemVal _t2453 = gem_bool(0);
    gem_pop_frame();
    return _t2453;
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
#line 659 "compiler/main.gem"
    GemVal _t2455[] = {gem_v_stmts};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2455, 1), gem_int(0)))) {
#line 660 "compiler/main.gem"
        GemVal _t2456 = gem_bool(0);
        gem_pop_frame();
        return _t2456;
    }
#line 662 "compiler/main.gem"
    GemVal _t2457[] = {gem_v_fn_name, gem_table_get(gem_v_stmts, gem_neg(gem_int(1)))};
    GemVal _t2458 = (*gem_v_is_self_tail_call);
    GemVal _t2459 = _t2458.fn(_t2458.env, _t2457, 2);
    gem_pop_frame();
    return _t2459;
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
#line 666 "compiler/main.gem"
    GemVal _t2461[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t2461, 1);
#line 667 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 668 "compiler/main.gem"
    GemVal _t2462 = gem_v_call_node;
    GemVal gem_v_args = gem_table_get(_t2462, gem_string("args"));
#line 669 "compiler/main.gem"
    GemVal _t2463 = gem_table_new();
    GemVal gem_v_arg_temps = _t2463;
#line 670 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 670 "compiler/main.gem"
    GemVal _t2464[] = {gem_v_args};
    GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2464, 1);
#line 670 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 670 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_44;
#line 670 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t2465[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2466 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2466.fn(_t2466.env, _t2465, 1);
#line 672 "compiler/main.gem"
    GemVal _t2467 = (*gem_v_tmp);
        GemVal gem_v_t = _t2467.fn(_t2467.env, NULL, 0);
#line 673 "compiler/main.gem"
    GemVal _t2468 = gem_v_r;
    GemVal _t2469[] = {gem_v_b, gem_table_get(_t2468, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2469, 2));
#line 674 "compiler/main.gem"
    GemVal _t2470 = gem_v_r;
    GemVal _t2471[] = {gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t2470, gem_string("expr")), gem_string(";\n")};
    GemVal _t2472[] = {gem_v_b, gem_interp(6, _t2471)};
        (void)(gem_buf_push_fn(NULL, _t2472, 2));
#line 675 "compiler/main.gem"
    GemVal _t2473[] = {gem_v_arg_temps, gem_v_t};
        (void)(gem_push_fn(NULL, _t2473, 2));
    }

#line 677 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 677 "compiler/main.gem"
    GemVal _t2474[] = {(*gem_v_tco_params)};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2474, 1);
#line 677 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 677 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 677 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 678 "compiler/main.gem"
    GemVal _t2475[] = {gem_table_get((*gem_v_tco_params), gem_v_i)};
    GemVal _t2476 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2476.fn(_t2476.env, _t2475, 1);
#line 679 "compiler/main.gem"
        GemVal gem_v_val = gem_string("GEM_NIL");
#line 680 "compiler/main.gem"
    GemVal _t2477[] = {gem_v_arg_temps};
        if (gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t2477, 1)))) {
#line 681 "compiler/main.gem"
            gem_v_val = gem_table_get(gem_v_arg_temps, gem_v_i);
        }
#line 683 "compiler/main.gem"
    GemVal _t2478[] = {(*gem_v_tco_captured), gem_table_get((*gem_v_tco_params), gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2478, 2))) {
#line 684 "compiler/main.gem"
    GemVal _t2479[] = {gem_v_p, gem_string("*"), gem_v_mp, gem_string(" = "), gem_v_val, gem_string(";\n")};
    GemVal _t2480[] = {gem_v_b, gem_interp(6, _t2479)};
            (void)(gem_buf_push_fn(NULL, _t2480, 2));
        } else {
#line 686 "compiler/main.gem"
    GemVal _t2481[] = {gem_v_p, gem_v_mp, gem_string(" = "), gem_v_val, gem_string(";\n")};
    GemVal _t2482[] = {gem_v_b, gem_interp(5, _t2481)};
            (void)(gem_buf_push_fn(NULL, _t2482, 2));
        }
    }

#line 689 "compiler/main.gem"
    GemVal _t2483[] = {gem_v_p, gem_string("continue;")};
    GemVal _t2484[] = {gem_v_b, gem_interp(2, _t2483)};
    (void)(gem_buf_push_fn(NULL, _t2484, 2));
#line 690 "compiler/main.gem"
    GemVal _t2485[] = {gem_v_b};
    GemVal _t2486 = gem_buf_str_fn(NULL, _t2485, 1);
    gem_pop_frame();
    return _t2486;
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
#line 696 "compiler/main.gem"
    GemVal gem_v__match_46 = gem_v_node;
    GemVal _t2488[] = {gem_v__match_46};
    GemVal _t2490;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2488, 1), gem_string("table")))) {
        _t2490 = gem_eq(gem_type_fn(NULL, _t2488, 1), gem_string("table"));
    } else {
        GemVal _t2489[] = {gem_v__match_46, gem_string("tag")};
        _t2490 = gem_has_key_fn(NULL, _t2489, 2);
    }
    GemVal _t2491;
    if (!gem_truthy(_t2490)) {
        _t2491 = _t2490;
    } else {
        _t2491 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2493;
    if (!gem_truthy(_t2491)) {
        _t2493 = _t2491;
    } else {
        GemVal _t2492[] = {gem_v__match_46, gem_string("value")};
        _t2493 = gem_has_key_fn(NULL, _t2492, 2);
    }
    if (gem_truthy(_t2493)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 698 "compiler/main.gem"
    GemVal _t2494 = gem_table_new();
    GemVal _t2495[] = {gem_string("gem_int("), gem_v_value, gem_string(")")};
    gem_table_set(_t2494, gem_string("expr"), gem_interp(3, _t2495));
    gem_table_set(_t2494, gem_string("setup"), gem_string(""));
        GemVal _t2496 = _t2494;
        gem_pop_frame();
        return _t2496;
    } else {
    GemVal _t2497[] = {gem_v__match_46};
    GemVal _t2499;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2497, 1), gem_string("table")))) {
        _t2499 = gem_eq(gem_type_fn(NULL, _t2497, 1), gem_string("table"));
    } else {
        GemVal _t2498[] = {gem_v__match_46, gem_string("tag")};
        _t2499 = gem_has_key_fn(NULL, _t2498, 2);
    }
    GemVal _t2500;
    if (!gem_truthy(_t2499)) {
        _t2500 = _t2499;
    } else {
        _t2500 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2502;
    if (!gem_truthy(_t2500)) {
        _t2502 = _t2500;
    } else {
        GemVal _t2501[] = {gem_v__match_46, gem_string("value")};
        _t2502 = gem_has_key_fn(NULL, _t2501, 2);
    }
    if (gem_truthy(_t2502)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 700 "compiler/main.gem"
    GemVal _t2503 = gem_table_new();
    GemVal _t2504[] = {gem_v_value};
    GemVal _t2505[] = {gem_string("gem_float("), gem_fn__mod_codegen_format_float(NULL, _t2504, 1), gem_string(")")};
    gem_table_set(_t2503, gem_string("expr"), gem_interp(3, _t2505));
    gem_table_set(_t2503, gem_string("setup"), gem_string(""));
        GemVal _t2506 = _t2503;
        gem_pop_frame();
        return _t2506;
    } else {
    GemVal _t2507[] = {gem_v__match_46};
    GemVal _t2509;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2507, 1), gem_string("table")))) {
        _t2509 = gem_eq(gem_type_fn(NULL, _t2507, 1), gem_string("table"));
    } else {
        GemVal _t2508[] = {gem_v__match_46, gem_string("tag")};
        _t2509 = gem_has_key_fn(NULL, _t2508, 2);
    }
    GemVal _t2510;
    if (!gem_truthy(_t2509)) {
        _t2510 = _t2509;
    } else {
        _t2510 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2512;
    if (!gem_truthy(_t2510)) {
        _t2512 = _t2510;
    } else {
        GemVal _t2511[] = {gem_v__match_46, gem_string("value")};
        _t2512 = gem_has_key_fn(NULL, _t2511, 2);
    }
    if (gem_truthy(_t2512)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 702 "compiler/main.gem"
    GemVal _t2513[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2513, 1);
#line 703 "compiler/main.gem"
    GemVal _t2514 = gem_table_new();
    GemVal _t2515[] = {gem_string("gem_string(\""), gem_v_escaped, gem_string("\")")};
    gem_table_set(_t2514, gem_string("expr"), gem_interp(3, _t2515));
    gem_table_set(_t2514, gem_string("setup"), gem_string(""));
        GemVal _t2516 = _t2514;
        gem_pop_frame();
        return _t2516;
    } else {
    GemVal _t2517[] = {gem_v__match_46};
    GemVal _t2519;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2517, 1), gem_string("table")))) {
        _t2519 = gem_eq(gem_type_fn(NULL, _t2517, 1), gem_string("table"));
    } else {
        GemVal _t2518[] = {gem_v__match_46, gem_string("tag")};
        _t2519 = gem_has_key_fn(NULL, _t2518, 2);
    }
    GemVal _t2520;
    if (!gem_truthy(_t2519)) {
        _t2520 = _t2519;
    } else {
        _t2520 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2522;
    if (!gem_truthy(_t2520)) {
        _t2522 = _t2520;
    } else {
        GemVal _t2521[] = {gem_v__match_46, gem_string("parts")};
        _t2522 = gem_has_key_fn(NULL, _t2521, 2);
    }
    if (gem_truthy(_t2522)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_46, gem_string("parts"));
#line 705 "compiler/main.gem"
    GemVal _t2523[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2523, 1), gem_int(0)))) {
#line 706 "compiler/main.gem"
    GemVal _t2524 = gem_table_new();
    gem_table_set(_t2524, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2524, gem_string("setup"), gem_string(""));
            GemVal _t2525 = _t2524;
            gem_pop_frame();
            return _t2525;
        }
#line 708 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 709 "compiler/main.gem"
    GemVal _t2526 = gem_table_new();
        GemVal gem_v_compiled = _t2526;
#line 710 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 710 "compiler/main.gem"
    GemVal _t2527[] = {gem_v_parts};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2527, 1);
#line 710 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 710 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_47;
#line 710 "compiler/main.gem"
            gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 711 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 712 "compiler/main.gem"
    GemVal _t2528[] = {gem_v_part};
    GemVal _t2529 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2529.fn(_t2529.env, _t2528, 1);
#line 713 "compiler/main.gem"
    GemVal _t2530 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2530, gem_string("setup")));
#line 714 "compiler/main.gem"
    GemVal _t2531 = gem_v_r;
    GemVal _t2532[] = {gem_v_compiled, gem_table_get(_t2531, gem_string("expr"))};
            (void)(gem_push_fn(NULL, _t2532, 2));
        }

#line 716 "compiler/main.gem"
    GemVal _t2533 = (*gem_v_tmp);
        GemVal gem_v_arr_tmp = _t2533.fn(_t2533.env, NULL, 0);
#line 717 "compiler/main.gem"
        GemVal gem_v_items = gem_table_get(gem_v_compiled, gem_int(0));
#line 718 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(1);
#line 718 "compiler/main.gem"
    GemVal _t2534[] = {gem_v_compiled};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2534, 1);
#line 718 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 718 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 718 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 719 "compiler/main.gem"
            gem_v_items = gem_add(gem_add(gem_v_items, gem_string(", ")), gem_table_get(gem_v_compiled, gem_v_i));
        }

#line 721 "compiler/main.gem"
    GemVal _t2535[] = {gem_string("    GemVal "), gem_v_arr_tmp, gem_string("[] = {"), gem_v_items, gem_string("};\n")};
        gem_v_setup = gem_add(gem_v_setup, gem_interp(5, _t2535));
#line 722 "compiler/main.gem"
    GemVal _t2536 = gem_table_new();
    GemVal _t2537[] = {gem_v_compiled};
    GemVal _t2538[] = {gem_string("gem_interp("), gem_len_fn(NULL, _t2537, 1), gem_string(", "), gem_v_arr_tmp, gem_string(")")};
    gem_table_set(_t2536, gem_string("expr"), gem_interp(5, _t2538));
    gem_table_set(_t2536, gem_string("setup"), gem_v_setup);
        GemVal _t2539 = _t2536;
        gem_pop_frame();
        return _t2539;
    } else {
    GemVal _t2540[] = {gem_v__match_46};
    GemVal _t2542;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2540, 1), gem_string("table")))) {
        _t2542 = gem_eq(gem_type_fn(NULL, _t2540, 1), gem_string("table"));
    } else {
        GemVal _t2541[] = {gem_v__match_46, gem_string("tag")};
        _t2542 = gem_has_key_fn(NULL, _t2541, 2);
    }
    GemVal _t2543;
    if (!gem_truthy(_t2542)) {
        _t2543 = _t2542;
    } else {
        _t2543 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2545;
    if (!gem_truthy(_t2543)) {
        _t2545 = _t2543;
    } else {
        GemVal _t2544[] = {gem_v__match_46, gem_string("value")};
        _t2545 = gem_has_key_fn(NULL, _t2544, 2);
    }
    if (gem_truthy(_t2545)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 724 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 725 "compiler/main.gem"
    GemVal _t2546 = gem_table_new();
    gem_table_set(_t2546, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2546, gem_string("setup"), gem_string(""));
            GemVal _t2547 = _t2546;
            gem_pop_frame();
            return _t2547;
        }
#line 727 "compiler/main.gem"
    GemVal _t2548 = gem_table_new();
    gem_table_set(_t2548, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2548, gem_string("setup"), gem_string(""));
        GemVal _t2549 = _t2548;
        gem_pop_frame();
        return _t2549;
    } else {
    GemVal _t2550[] = {gem_v__match_46};
    GemVal _t2552;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2550, 1), gem_string("table")))) {
        _t2552 = gem_eq(gem_type_fn(NULL, _t2550, 1), gem_string("table"));
    } else {
        GemVal _t2551[] = {gem_v__match_46, gem_string("tag")};
        _t2552 = gem_has_key_fn(NULL, _t2551, 2);
    }
    GemVal _t2553;
    if (!gem_truthy(_t2552)) {
        _t2553 = _t2552;
    } else {
        _t2553 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2553)) {
#line 729 "compiler/main.gem"
    GemVal _t2554 = gem_table_new();
    gem_table_set(_t2554, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2554, gem_string("setup"), gem_string(""));
        GemVal _t2555 = _t2554;
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
        _t2559 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2561;
    if (!gem_truthy(_t2559)) {
        _t2561 = _t2559;
    } else {
        GemVal _t2560[] = {gem_v__match_46, gem_string("name")};
        _t2561 = gem_has_key_fn(NULL, _t2560, 2);
    }
    if (gem_truthy(_t2561)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_46, gem_string("name"));
#line 731 "compiler/main.gem"
    GemVal _t2562[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2562, 2)))) {
#line 732 "compiler/main.gem"
    GemVal _t2563[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2563, 2))) {
#line 733 "compiler/main.gem"
    GemVal _t2564 = gem_table_new();
    GemVal _t2565[] = {gem_string("gem_make_fn("), gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string(", NULL)")};
    gem_table_set(_t2564, gem_string("expr"), gem_interp(3, _t2565));
    gem_table_set(_t2564, gem_string("setup"), gem_string(""));
                GemVal _t2566 = _t2564;
                gem_pop_frame();
                return _t2566;
            } else {
#line 734 "compiler/main.gem"
    GemVal _t2567[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2567, 2))) {
#line 735 "compiler/main.gem"
    GemVal _t2568 = gem_table_new();
    GemVal _t2569[] = {gem_string("gem_make_fn(gem_fn_"), gem_v_name, gem_string(", NULL)")};
    gem_table_set(_t2568, gem_string("expr"), gem_interp(3, _t2569));
    gem_table_set(_t2568, gem_string("setup"), gem_string(""));
                    GemVal _t2570 = _t2568;
                    gem_pop_frame();
                    return _t2570;
                }
            }
        }
#line 738 "compiler/main.gem"
    GemVal _t2571[] = {gem_v_name};
    GemVal _t2572 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2572.fn(_t2572.env, _t2571, 1);
#line 739 "compiler/main.gem"
    GemVal _t2573[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2575;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2573, 2))) {
        _t2575 = gem_fn__mod_codegen_set_contains(NULL, _t2573, 2);
    } else {
        GemVal _t2574[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2575 = gem_fn__mod_codegen_set_contains(NULL, _t2574, 2);
    }
        if (gem_truthy(_t2575)) {
#line 740 "compiler/main.gem"
    GemVal _t2576 = gem_table_new();
    GemVal _t2577[] = {gem_string("(*"), gem_v_mname, gem_string(")")};
    gem_table_set(_t2576, gem_string("expr"), gem_interp(3, _t2577));
    gem_table_set(_t2576, gem_string("setup"), gem_string(""));
            GemVal _t2578 = _t2576;
            gem_pop_frame();
            return _t2578;
        }
#line 742 "compiler/main.gem"
    GemVal _t2579 = gem_table_new();
    gem_table_set(_t2579, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2579, gem_string("setup"), gem_string(""));
        GemVal _t2580 = _t2579;
        gem_pop_frame();
        return _t2580;
    } else {
    GemVal _t2581[] = {gem_v__match_46};
    GemVal _t2583;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2581, 1), gem_string("table")))) {
        _t2583 = gem_eq(gem_type_fn(NULL, _t2581, 1), gem_string("table"));
    } else {
        GemVal _t2582[] = {gem_v__match_46, gem_string("tag")};
        _t2583 = gem_has_key_fn(NULL, _t2582, 2);
    }
    GemVal _t2584;
    if (!gem_truthy(_t2583)) {
        _t2584 = _t2583;
    } else {
        _t2584 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2584)) {
#line 744 "compiler/main.gem"
    GemVal _t2585[] = {gem_v_node};
    GemVal _t2586 = (*gem_v_compile_call);
        GemVal _t2587 = _t2586.fn(_t2586.env, _t2585, 1);
        gem_pop_frame();
        return _t2587;
    } else {
    GemVal _t2588[] = {gem_v__match_46};
    GemVal _t2590;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2588, 1), gem_string("table")))) {
        _t2590 = gem_eq(gem_type_fn(NULL, _t2588, 1), gem_string("table"));
    } else {
        GemVal _t2589[] = {gem_v__match_46, gem_string("tag")};
        _t2590 = gem_has_key_fn(NULL, _t2589, 2);
    }
    GemVal _t2591;
    if (!gem_truthy(_t2590)) {
        _t2591 = _t2590;
    } else {
        _t2591 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2591)) {
#line 746 "compiler/main.gem"
    GemVal _t2592[] = {gem_v_node};
    GemVal _t2593 = (*gem_v_compile_binop);
        GemVal _t2594 = _t2593.fn(_t2593.env, _t2592, 1);
        gem_pop_frame();
        return _t2594;
    } else {
    GemVal _t2595[] = {gem_v__match_46};
    GemVal _t2597;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2595, 1), gem_string("table")))) {
        _t2597 = gem_eq(gem_type_fn(NULL, _t2595, 1), gem_string("table"));
    } else {
        GemVal _t2596[] = {gem_v__match_46, gem_string("tag")};
        _t2597 = gem_has_key_fn(NULL, _t2596, 2);
    }
    GemVal _t2598;
    if (!gem_truthy(_t2597)) {
        _t2598 = _t2597;
    } else {
        _t2598 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2600;
    if (!gem_truthy(_t2598)) {
        _t2600 = _t2598;
    } else {
        GemVal _t2599[] = {gem_v__match_46, gem_string("expr")};
        _t2600 = gem_has_key_fn(NULL, _t2599, 2);
    }
    GemVal _t2602;
    if (!gem_truthy(_t2600)) {
        _t2602 = _t2600;
    } else {
        GemVal _t2601[] = {gem_v__match_46, gem_string("op")};
        _t2602 = gem_has_key_fn(NULL, _t2601, 2);
    }
    if (gem_truthy(_t2602)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_46, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_46, gem_string("op"));
#line 748 "compiler/main.gem"
    GemVal _t2603[] = {gem_v_uexpr};
    GemVal _t2604 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2604.fn(_t2604.env, _t2603, 1);
#line 749 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 750 "compiler/main.gem"
    GemVal _t2605 = gem_table_new();
    GemVal _t2606 = gem_v_r;
    GemVal _t2607[] = {gem_string("gem_neg("), gem_table_get(_t2606, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2605, gem_string("expr"), gem_interp(3, _t2607));
    GemVal _t2608 = gem_v_r;
    gem_table_set(_t2605, gem_string("setup"), gem_table_get(_t2608, gem_string("setup")));
            GemVal _t2609 = _t2605;
            gem_pop_frame();
            return _t2609;
        } else {
#line 751 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 752 "compiler/main.gem"
    GemVal _t2610 = gem_table_new();
    GemVal _t2611 = gem_v_r;
    GemVal _t2612[] = {gem_string("gem_not("), gem_table_get(_t2611, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2610, gem_string("expr"), gem_interp(3, _t2612));
    GemVal _t2613 = gem_v_r;
    gem_table_set(_t2610, gem_string("setup"), gem_table_get(_t2613, gem_string("setup")));
                GemVal _t2614 = _t2610;
                gem_pop_frame();
                return _t2614;
            }
        }
#line 754 "compiler/main.gem"
    GemVal _t2615[] = {gem_string("unknown unary operator: "), gem_v_op};
    GemVal _t2616[] = {gem_interp(2, _t2615)};
        (void)(gem_error_at_fn("compiler/main.gem", 754, _t2616, 1));
    } else {
    GemVal _t2617[] = {gem_v__match_46};
    GemVal _t2619;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2617, 1), gem_string("table")))) {
        _t2619 = gem_eq(gem_type_fn(NULL, _t2617, 1), gem_string("table"));
    } else {
        GemVal _t2618[] = {gem_v__match_46, gem_string("tag")};
        _t2619 = gem_has_key_fn(NULL, _t2618, 2);
    }
    GemVal _t2620;
    if (!gem_truthy(_t2619)) {
        _t2620 = _t2619;
    } else {
        _t2620 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2620)) {
#line 756 "compiler/main.gem"
    GemVal _t2621[] = {gem_v_node};
    GemVal _t2622 = (*gem_v_compile_anon_fn);
        GemVal _t2623 = _t2622.fn(_t2622.env, _t2621, 1);
        gem_pop_frame();
        return _t2623;
    } else {
    GemVal _t2624[] = {gem_v__match_46};
    GemVal _t2626;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2624, 1), gem_string("table")))) {
        _t2626 = gem_eq(gem_type_fn(NULL, _t2624, 1), gem_string("table"));
    } else {
        GemVal _t2625[] = {gem_v__match_46, gem_string("tag")};
        _t2626 = gem_has_key_fn(NULL, _t2625, 2);
    }
    GemVal _t2627;
    if (!gem_truthy(_t2626)) {
        _t2627 = _t2626;
    } else {
        _t2627 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2627)) {
#line 758 "compiler/main.gem"
    GemVal _t2628[] = {gem_v_node};
    GemVal _t2629 = (*gem_v_compile_table);
        GemVal _t2630 = _t2629.fn(_t2629.env, _t2628, 1);
        gem_pop_frame();
        return _t2630;
    } else {
    GemVal _t2631[] = {gem_v__match_46};
    GemVal _t2633;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2631, 1), gem_string("table")))) {
        _t2633 = gem_eq(gem_type_fn(NULL, _t2631, 1), gem_string("table"));
    } else {
        GemVal _t2632[] = {gem_v__match_46, gem_string("tag")};
        _t2633 = gem_has_key_fn(NULL, _t2632, 2);
    }
    GemVal _t2634;
    if (!gem_truthy(_t2633)) {
        _t2634 = _t2633;
    } else {
        _t2634 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2634)) {
#line 760 "compiler/main.gem"
    GemVal _t2635[] = {gem_v_node};
    GemVal _t2636 = (*gem_v_compile_array);
        GemVal _t2637 = _t2636.fn(_t2636.env, _t2635, 1);
        gem_pop_frame();
        return _t2637;
    } else {
    GemVal _t2638[] = {gem_v__match_46};
    GemVal _t2640;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2638, 1), gem_string("table")))) {
        _t2640 = gem_eq(gem_type_fn(NULL, _t2638, 1), gem_string("table"));
    } else {
        GemVal _t2639[] = {gem_v__match_46, gem_string("tag")};
        _t2640 = gem_has_key_fn(NULL, _t2639, 2);
    }
    GemVal _t2641;
    if (!gem_truthy(_t2640)) {
        _t2641 = _t2640;
    } else {
        _t2641 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2643;
    if (!gem_truthy(_t2641)) {
        _t2643 = _t2641;
    } else {
        GemVal _t2642[] = {gem_v__match_46, gem_string("object")};
        _t2643 = gem_has_key_fn(NULL, _t2642, 2);
    }
    GemVal _t2645;
    if (!gem_truthy(_t2643)) {
        _t2645 = _t2643;
    } else {
        GemVal _t2644[] = {gem_v__match_46, gem_string("field")};
        _t2645 = gem_has_key_fn(NULL, _t2644, 2);
    }
    if (gem_truthy(_t2645)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_46, gem_string("field"));
#line 762 "compiler/main.gem"
    GemVal _t2646[] = {gem_v_object};
    GemVal _t2647 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2647.fn(_t2647.env, _t2646, 1);
#line 763 "compiler/main.gem"
    GemVal _t2648 = (*gem_v_tmp);
        GemVal gem_v_t = _t2648.fn(_t2648.env, NULL, 0);
#line 764 "compiler/main.gem"
    GemVal _t2649 = gem_v_r;
    GemVal _t2650 = gem_v_r;
    GemVal _t2651[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t2650, gem_string("expr")), gem_string(";\n")};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2649, gem_string("setup")), gem_interp(5, _t2651));
#line 765 "compiler/main.gem"
    GemVal _t2652[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2652, 1);
#line 766 "compiler/main.gem"
    GemVal _t2653 = gem_table_new();
    GemVal _t2654[] = {gem_string("gem_table_get("), gem_v_t, gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"))")};
    gem_table_set(_t2653, gem_string("expr"), gem_interp(5, _t2654));
    gem_table_set(_t2653, gem_string("setup"), gem_v_setup);
        GemVal _t2655 = _t2653;
        gem_pop_frame();
        return _t2655;
    } else {
    GemVal _t2656[] = {gem_v__match_46};
    GemVal _t2658;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2656, 1), gem_string("table")))) {
        _t2658 = gem_eq(gem_type_fn(NULL, _t2656, 1), gem_string("table"));
    } else {
        GemVal _t2657[] = {gem_v__match_46, gem_string("tag")};
        _t2658 = gem_has_key_fn(NULL, _t2657, 2);
    }
    GemVal _t2659;
    if (!gem_truthy(_t2658)) {
        _t2659 = _t2658;
    } else {
        _t2659 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2661;
    if (!gem_truthy(_t2659)) {
        _t2661 = _t2659;
    } else {
        GemVal _t2660[] = {gem_v__match_46, gem_string("object")};
        _t2661 = gem_has_key_fn(NULL, _t2660, 2);
    }
    GemVal _t2663;
    if (!gem_truthy(_t2661)) {
        _t2663 = _t2661;
    } else {
        GemVal _t2662[] = {gem_v__match_46, gem_string("key")};
        _t2663 = gem_has_key_fn(NULL, _t2662, 2);
    }
    if (gem_truthy(_t2663)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_46, gem_string("key"));
#line 768 "compiler/main.gem"
    GemVal _t2664[] = {gem_v_object};
    GemVal _t2665 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2665.fn(_t2665.env, _t2664, 1);
#line 769 "compiler/main.gem"
    GemVal _t2666[] = {gem_v_key};
    GemVal _t2667 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2667.fn(_t2667.env, _t2666, 1);
#line 770 "compiler/main.gem"
    GemVal _t2668 = gem_table_new();
    GemVal _t2669 = gem_v_obj_r;
    GemVal _t2670 = gem_v_key_r;
    GemVal _t2671[] = {gem_string("gem_table_get("), gem_table_get(_t2669, gem_string("expr")), gem_string(", "), gem_table_get(_t2670, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2668, gem_string("expr"), gem_interp(5, _t2671));
    GemVal _t2672 = gem_v_obj_r;
    GemVal _t2673 = gem_v_key_r;
    gem_table_set(_t2668, gem_string("setup"), gem_add(gem_table_get(_t2672, gem_string("setup")), gem_table_get(_t2673, gem_string("setup"))));
        GemVal _t2674 = _t2668;
        gem_pop_frame();
        return _t2674;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 773 "compiler/main.gem"
    GemVal _t2675 = gem_v_node;
    GemVal _t2676[] = {gem_string("unknown expression node: "), gem_table_get(_t2675, gem_string("tag"))};
    GemVal _t2677[] = {gem_interp(2, _t2676)};
    GemVal _t2678 = gem_error_at_fn("compiler/main.gem", 773, _t2677, 1);
    gem_pop_frame();
    return _t2678;
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
#line 779 "compiler/main.gem"
    GemVal _t2680 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2680, gem_string("entries"));
#line 780 "compiler/main.gem"
    GemVal _t2681 = (*gem_v_tmp);
    GemVal gem_v_t = _t2681.fn(_t2681.env, NULL, 0);
#line 781 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 782 "compiler/main.gem"
    GemVal _t2682[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = gem_table_new();\n")};
    GemVal _t2683[] = {gem_v_b, gem_interp(3, _t2682)};
    (void)(gem_buf_push_fn(NULL, _t2683, 2));
#line 783 "compiler/main.gem"
    GemVal gem_v__for_items_49 = gem_v_entries;
#line 783 "compiler/main.gem"
    GemVal gem_v__for_i_49 = gem_int(0);
#line 783 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2684[] = {gem_v__for_items_49};
        if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2684, 1)))) break;
#line 783 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 783 "compiler/main.gem"
        gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 784 "compiler/main.gem"
    GemVal _t2685 = gem_v_entry;
    GemVal _t2686[] = {gem_table_get(_t2685, gem_string("value"))};
    GemVal _t2687 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2687.fn(_t2687.env, _t2686, 1);
#line 785 "compiler/main.gem"
    GemVal _t2688 = gem_v_entry;
    GemVal _t2689[] = {gem_table_get(_t2688, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2689, 1);
#line 786 "compiler/main.gem"
    GemVal _t2690 = gem_v_val_r;
    GemVal _t2691[] = {gem_v_b, gem_table_get(_t2690, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2691, 2));
#line 787 "compiler/main.gem"
    GemVal _t2692 = gem_v_val_r;
    GemVal _t2693[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"), "), gem_table_get(_t2692, gem_string("expr")), gem_string(");\n")};
    GemVal _t2694[] = {gem_v_b, gem_interp(7, _t2693)};
        (void)(gem_buf_push_fn(NULL, _t2694, 2));
    }

    GemVal _t2695 = gem_table_new();
    gem_table_set(_t2695, gem_string("expr"), gem_v_t);
    GemVal _t2696[] = {gem_v_b};
    gem_table_set(_t2695, gem_string("setup"), gem_buf_str_fn(NULL, _t2696, 1));
    GemVal _t2697 = _t2695;
    gem_pop_frame();
    return _t2697;
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
#line 795 "compiler/main.gem"
    GemVal _t2699 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2699, gem_string("elements"));
#line 796 "compiler/main.gem"
    GemVal _t2700 = (*gem_v_tmp);
    GemVal gem_v_t = _t2700.fn(_t2700.env, NULL, 0);
#line 797 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 798 "compiler/main.gem"
    GemVal _t2701[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = gem_table_new();\n")};
    GemVal _t2702[] = {gem_v_b, gem_interp(3, _t2701)};
    (void)(gem_buf_push_fn(NULL, _t2702, 2));
#line 799 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 799 "compiler/main.gem"
    GemVal _t2703[] = {gem_v_elements};
    GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2703, 1);
#line 799 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 799 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_50;
#line 799 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 800 "compiler/main.gem"
    GemVal _t2704[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2705 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2705.fn(_t2705.env, _t2704, 1);
#line 801 "compiler/main.gem"
    GemVal _t2706 = gem_v_elem_r;
    GemVal _t2707[] = {gem_v_b, gem_table_get(_t2706, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2707, 2));
#line 802 "compiler/main.gem"
    GemVal _t2708 = gem_v_elem_r;
    GemVal _t2709[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_int("), gem_v_i, gem_string("), "), gem_table_get(_t2708, gem_string("expr")), gem_string(");\n")};
    GemVal _t2710[] = {gem_v_b, gem_interp(7, _t2709)};
        (void)(gem_buf_push_fn(NULL, _t2710, 2));
    }

    GemVal _t2711 = gem_table_new();
    gem_table_set(_t2711, gem_string("expr"), gem_v_t);
    GemVal _t2712[] = {gem_v_b};
    gem_table_set(_t2711, gem_string("setup"), gem_buf_str_fn(NULL, _t2712, 1));
    GemVal _t2713 = _t2711;
    gem_pop_frame();
    return _t2713;
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
#line 810 "compiler/main.gem"
    GemVal _t2715 = gem_v_node;
    GemVal _t2716[] = {gem_table_get(_t2715, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2716, 1);
#line 811 "compiler/main.gem"
    GemVal _t2717 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2717, gem_string("rest_param")), GEM_NIL))) {
#line 812 "compiler/main.gem"
    GemVal _t2718 = gem_v_node;
    GemVal _t2719[] = {gem_v_inner_defined, gem_table_get(_t2718, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t2719, 2));
    }
#line 814 "compiler/main.gem"
    GemVal _t2720 = gem_v_node;
    GemVal _t2721[] = {gem_table_get(_t2720, gem_string("body")), gem_v_inner_defined};
    GemVal _t2722 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2722.fn(_t2722.env, _t2721, 2);
#line 815 "compiler/main.gem"
    GemVal _t2723[] = {gem_v_free};
    GemVal _t2724[] = {gem_keys_fn(NULL, _t2723, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2724, 1);
#line 817 "compiler/main.gem"
    GemVal _t2725[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2725, 1), gem_int(0)))) {
#line 818 "compiler/main.gem"
    GemVal _t2726 = gem_table_new();
    GemVal _t2727[] = {gem_v_node, _t2726};
    GemVal _t2728 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2728.fn(_t2728.env, _t2727, 2);
#line 819 "compiler/main.gem"
    GemVal _t2729 = gem_table_new();
    GemVal _t2730 = gem_v_result;
    GemVal _t2731[] = {gem_string("gem_make_fn("), gem_table_get(_t2730, gem_string("fn_name")), gem_string(", NULL)")};
    gem_table_set(_t2729, gem_string("expr"), gem_interp(3, _t2731));
    gem_table_set(_t2729, gem_string("setup"), gem_string(""));
        GemVal _t2732 = _t2729;
        gem_pop_frame();
        return _t2732;
    }
#line 822 "compiler/main.gem"
    GemVal _t2733[] = {gem_v_node, gem_v_captures};
    GemVal _t2734 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2734.fn(_t2734.env, _t2733, 2);
#line 824 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 825 "compiler/main.gem"
    GemVal _t2735 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2735.fn(_t2735.env, NULL, 0);
#line 826 "compiler/main.gem"
    GemVal _t2736 = gem_v_result;
    GemVal _t2737 = gem_v_result;
    GemVal _t2738[] = {gem_string("    struct "), gem_table_get(_t2736, gem_string("struct_name")), gem_string(" *"), gem_v_env_tmp, gem_string(" = GC_MALLOC(sizeof(struct "), gem_table_get(_t2737, gem_string("struct_name")), gem_string("));\n")};
    GemVal _t2739[] = {gem_v_b, gem_interp(7, _t2738)};
    (void)(gem_buf_push_fn(NULL, _t2739, 2));
#line 827 "compiler/main.gem"
    GemVal gem_v__for_items_51 = gem_v_captures;
#line 827 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 827 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2740[] = {gem_v__for_items_51};
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t2740, 1)))) break;
#line 827 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 827 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 828 "compiler/main.gem"
    GemVal _t2741[] = {gem_v_cap};
    GemVal _t2742 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2742.fn(_t2742.env, _t2741, 1);
#line 829 "compiler/main.gem"
    GemVal _t2743[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2743, 2))) {
#line 830 "compiler/main.gem"
    GemVal _t2744[] = {gem_string("    "), gem_v_env_tmp, gem_string("->"), gem_v_mc, gem_string(" = "), gem_v_mc, gem_string(";\n")};
    GemVal _t2745[] = {gem_v_b, gem_interp(7, _t2744)};
            (void)(gem_buf_push_fn(NULL, _t2745, 2));
        } else {
#line 832 "compiler/main.gem"
    GemVal _t2746[] = {gem_string("    "), gem_v_env_tmp, gem_string("->"), gem_v_mc, gem_string(" = &"), gem_v_mc, gem_string(";\n")};
    GemVal _t2747[] = {gem_v_b, gem_interp(7, _t2746)};
            (void)(gem_buf_push_fn(NULL, _t2747, 2));
        }
    }

    GemVal _t2748 = gem_table_new();
    GemVal _t2749 = gem_v_result;
    GemVal _t2750[] = {gem_string("gem_make_fn("), gem_table_get(_t2749, gem_string("fn_name")), gem_string(", "), gem_v_env_tmp, gem_string(")")};
    gem_table_set(_t2748, gem_string("expr"), gem_interp(5, _t2750));
    GemVal _t2751[] = {gem_v_b};
    gem_table_set(_t2748, gem_string("setup"), gem_buf_str_fn(NULL, _t2751, 1));
    GemVal _t2752 = _t2748;
    gem_pop_frame();
    return _t2752;
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
#line 842 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 843 "compiler/main.gem"
    GemVal _t2754 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2754;
#line 844 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 844 "compiler/main.gem"
    GemVal _t2755[] = {gem_v_args};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2755, 1);
#line 844 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 844 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 844 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 845 "compiler/main.gem"
    GemVal _t2756[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2757 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2757.fn(_t2757.env, _t2756, 1);
#line 846 "compiler/main.gem"
    GemVal _t2758 = gem_v_r;
    GemVal _t2759[] = {gem_v_b, gem_table_get(_t2758, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2759, 2));
#line 847 "compiler/main.gem"
    GemVal _t2760 = gem_v_r;
    GemVal _t2761[] = {gem_v_arg_exprs, gem_table_get(_t2760, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2761, 2));
    }

#line 849 "compiler/main.gem"
    GemVal _t2762[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2762, 1);
#line 850 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 851 "compiler/main.gem"
    GemVal _t2763 = gem_table_new();
    GemVal _t2764[] = {gem_v_b};
    gem_table_set(_t2763, gem_string("setup"), gem_buf_str_fn(NULL, _t2764, 1));
    gem_table_set(_t2763, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2763, gem_string("argc"), gem_int(0));
        GemVal _t2765 = _t2763;
        gem_pop_frame();
        return _t2765;
    }
#line 853 "compiler/main.gem"
    GemVal _t2766 = (*gem_v_tmp);
    GemVal gem_v_t = _t2766.fn(_t2766.env, NULL, 0);
#line 854 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 855 "compiler/main.gem"
    GemVal gem_v__for_i_53 = gem_int(1);
#line 855 "compiler/main.gem"
    GemVal gem_v__for_limit_53 = gem_v_argc;
#line 855 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_v__for_limit_53))) break;
#line 855 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_53;
#line 855 "compiler/main.gem"
        gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 856 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2767 = gem_table_new();
    GemVal _t2768[] = {gem_v_b};
    GemVal _t2769[] = {gem_string("    GemVal "), gem_v_t, gem_string("[] = {"), gem_v_arr, gem_string("};\n")};
    gem_table_set(_t2767, gem_string("setup"), gem_add(gem_buf_str_fn(NULL, _t2768, 1), gem_interp(5, _t2769)));
    gem_table_set(_t2767, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2767, gem_string("argc"), gem_v_argc);
    GemVal _t2770 = _t2767;
    gem_pop_frame();
    return _t2770;
}

struct _closure__anon_40 {
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 863 "compiler/main.gem"
    GemVal _t2772 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2772, gem_string("argc")), gem_int(0)))) {
#line 864 "compiler/main.gem"
    GemVal _t2773 = gem_table_new();
    GemVal _t2774[] = {gem_v_fn_name, gem_string("("), gem_v_env, gem_string(", NULL, 0)")};
    gem_table_set(_t2773, gem_string("expr"), gem_interp(4, _t2774));
    GemVal _t2775 = gem_v_ca;
    gem_table_set(_t2773, gem_string("setup"), gem_table_get(_t2775, gem_string("setup")));
        GemVal _t2776 = _t2773;
        gem_pop_frame();
        return _t2776;
    }
    GemVal _t2777 = gem_table_new();
    GemVal _t2778 = gem_v_ca;
    GemVal _t2779 = gem_v_ca;
    GemVal _t2780[] = {gem_v_fn_name, gem_string("("), gem_v_env, gem_string(", "), gem_table_get(_t2778, gem_string("arr_name")), gem_string(", "), gem_table_get(_t2779, gem_string("argc")), gem_string(")")};
    gem_table_set(_t2777, gem_string("expr"), gem_interp(8, _t2780));
    GemVal _t2781 = gem_v_ca;
    gem_table_set(_t2777, gem_string("setup"), gem_table_get(_t2781, gem_string("setup")));
    GemVal _t2782 = _t2777;
    gem_pop_frame();
    return _t2782;
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
#line 872 "compiler/main.gem"
    GemVal gem_v__d54 = gem_v_node;
#line 872 "compiler/main.gem"
    GemVal _t2783 = gem_v__d54;
    GemVal gem_v_func = gem_table_get(_t2783, gem_string("func"));
#line 872 "compiler/main.gem"
    GemVal _t2784 = gem_v__d54;
    GemVal gem_v_args = gem_table_get(_t2784, gem_string("args"));

#line 875 "compiler/main.gem"
    GemVal _t2785 = gem_v_func;
    GemVal _t2787;
    if (!gem_truthy(gem_eq(gem_table_get(_t2785, gem_string("tag")), gem_string("dot")))) {
        _t2787 = gem_eq(gem_table_get(_t2785, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2786 = gem_v_func;
        _t2787 = gem_eq(gem_table_get(_t2786, gem_string("field")), gem_string("len"));
    }
    GemVal _t2789;
    if (!gem_truthy(_t2787)) {
        _t2789 = _t2787;
    } else {
        GemVal _t2788[] = {gem_v_args};
        _t2789 = gem_eq(gem_len_fn(NULL, _t2788, 1), gem_int(0));
    }
    if (gem_truthy(_t2789)) {
#line 876 "compiler/main.gem"
    GemVal _t2790 = gem_v_func;
    GemVal _t2791[] = {gem_table_get(_t2790, gem_string("object"))};
    GemVal _t2792 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2792.fn(_t2792.env, _t2791, 1);
#line 877 "compiler/main.gem"
    GemVal _t2793 = gem_table_new();
    GemVal _t2794 = gem_v_obj_r;
    GemVal _t2795[] = {gem_string("gem_len_val("), gem_table_get(_t2794, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2793, gem_string("expr"), gem_interp(3, _t2795));
    GemVal _t2796 = gem_v_obj_r;
    gem_table_set(_t2793, gem_string("setup"), gem_table_get(_t2796, gem_string("setup")));
        GemVal _t2797 = _t2793;
        gem_pop_frame();
        return _t2797;
    }
#line 881 "compiler/main.gem"
    GemVal _t2798 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2798, gem_string("tag")), gem_string("var")))) {
#line 882 "compiler/main.gem"
    GemVal _t2799 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2799, gem_string("name"));
#line 884 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 885 "compiler/main.gem"
    GemVal _t2800 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2800, gem_string("line"));
#line 886 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 887 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 889 "compiler/main.gem"
    GemVal _t2801[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn__mod_codegen_escape_c_string(NULL, _t2801, 1);
#line 890 "compiler/main.gem"
    GemVal _t2802[] = {gem_v_args};
    GemVal _t2803 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2803.fn(_t2803.env, _t2802, 1);
#line 891 "compiler/main.gem"
    GemVal _t2804 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2804, gem_string("argc")), gem_int(0)))) {
#line 892 "compiler/main.gem"
    GemVal _t2805 = gem_table_new();
    GemVal _t2806[] = {gem_string("gem_error_at_fn(\""), gem_v_escaped_file, gem_string("\", "), gem_v_line, gem_string(", NULL, 0)")};
    gem_table_set(_t2805, gem_string("expr"), gem_interp(5, _t2806));
    GemVal _t2807 = gem_v_ca;
    gem_table_set(_t2805, gem_string("setup"), gem_table_get(_t2807, gem_string("setup")));
                GemVal _t2808 = _t2805;
                gem_pop_frame();
                return _t2808;
            }
#line 894 "compiler/main.gem"
    GemVal _t2809 = gem_table_new();
    GemVal _t2810 = gem_v_ca;
    GemVal _t2811 = gem_v_ca;
    GemVal _t2812[] = {gem_string("gem_error_at_fn(\""), gem_v_escaped_file, gem_string("\", "), gem_v_line, gem_string(", "), gem_table_get(_t2810, gem_string("arr_name")), gem_string(", "), gem_table_get(_t2811, gem_string("argc")), gem_string(")")};
    gem_table_set(_t2809, gem_string("expr"), gem_interp(9, _t2812));
    GemVal _t2813 = gem_v_ca;
    gem_table_set(_t2809, gem_string("setup"), gem_table_get(_t2813, gem_string("setup")));
            GemVal _t2814 = _t2809;
            gem_pop_frame();
            return _t2814;
        } else {
#line 895 "compiler/main.gem"
    GemVal _t2815[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2817;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2815, 2))) {
        _t2817 = gem_has_key_fn(NULL, _t2815, 2);
    } else {
        GemVal _t2816[] = {(*gem_v_local_names), gem_v_name};
        _t2817 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2816, 2));
    }
            if (gem_truthy(_t2817)) {
#line 896 "compiler/main.gem"
    GemVal _t2818[] = {gem_v_args};
    GemVal _t2819 = (*gem_v_compile_args);
    GemVal _t2820[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2819.fn(_t2819.env, _t2818, 1)};
    GemVal _t2821 = (*gem_v_emit_direct_call);
                GemVal _t2822 = _t2821.fn(_t2821.env, _t2820, 3);
                gem_pop_frame();
                return _t2822;
            } else {
#line 897 "compiler/main.gem"
    GemVal _t2823[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2825;
    if (!gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2823, 2))) {
        _t2825 = gem_fn__mod_codegen_set_contains(NULL, _t2823, 2);
    } else {
        GemVal _t2824[] = {(*gem_v_local_names), gem_v_name};
        _t2825 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2824, 2));
    }
                if (gem_truthy(_t2825)) {
#line 898 "compiler/main.gem"
    GemVal _t2826[] = {gem_string("gem_fn_"), gem_v_name};
    GemVal _t2827[] = {gem_v_args};
    GemVal _t2828 = (*gem_v_compile_args);
    GemVal _t2829[] = {gem_interp(2, _t2826), gem_string("NULL"), _t2828.fn(_t2828.env, _t2827, 1)};
    GemVal _t2830 = (*gem_v_emit_direct_call);
                    GemVal _t2831 = _t2830.fn(_t2830.env, _t2829, 3);
                    gem_pop_frame();
                    return _t2831;
                }
            }
        }
    }
#line 903 "compiler/main.gem"
    GemVal _t2832[] = {gem_v_func};
    GemVal _t2833 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2833.fn(_t2833.env, _t2832, 1);
#line 904 "compiler/main.gem"
    GemVal _t2834[] = {gem_v_args};
    GemVal _t2835 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2835.fn(_t2835.env, _t2834, 1);
#line 905 "compiler/main.gem"
    GemVal _t2836 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2836.fn(_t2836.env, NULL, 0);
#line 906 "compiler/main.gem"
    GemVal _t2837 = gem_v_func_r;
    GemVal _t2838 = gem_v_ca;
    GemVal _t2839 = gem_v_func_r;
    GemVal _t2840[] = {gem_string("    GemVal "), gem_v_tmp_fn, gem_string(" = "), gem_table_get(_t2839, gem_string("expr")), gem_string(";\n")};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2837, gem_string("setup")), gem_table_get(_t2838, gem_string("setup"))), gem_interp(5, _t2840));
#line 907 "compiler/main.gem"
    GemVal _t2841 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2841, gem_string("argc")), gem_int(0)))) {
#line 908 "compiler/main.gem"
    GemVal _t2842 = gem_table_new();
    GemVal _t2843[] = {gem_v_tmp_fn, gem_string(".fn("), gem_v_tmp_fn, gem_string(".env, NULL, 0)")};
    gem_table_set(_t2842, gem_string("expr"), gem_interp(4, _t2843));
    gem_table_set(_t2842, gem_string("setup"), gem_v_setup);
        GemVal _t2844 = _t2842;
        gem_pop_frame();
        return _t2844;
    }
    GemVal _t2845 = gem_table_new();
    GemVal _t2846 = gem_v_ca;
    GemVal _t2847 = gem_v_ca;
    GemVal _t2848[] = {gem_v_tmp_fn, gem_string(".fn("), gem_v_tmp_fn, gem_string(".env, "), gem_table_get(_t2846, gem_string("arr_name")), gem_string(", "), gem_table_get(_t2847, gem_string("argc")), gem_string(")")};
    gem_table_set(_t2845, gem_string("expr"), gem_interp(8, _t2848));
    gem_table_set(_t2845, gem_string("setup"), gem_v_setup);
    GemVal _t2849 = _t2845;
    gem_pop_frame();
    return _t2849;
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
#line 916 "compiler/main.gem"
    GemVal _t2851 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2851, gem_string("op"));
#line 917 "compiler/main.gem"
    GemVal _t2852 = gem_v_node;
    GemVal _t2853[] = {gem_table_get(_t2852, gem_string("left"))};
    GemVal _t2854 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2854.fn(_t2854.env, _t2853, 1);
#line 918 "compiler/main.gem"
    GemVal _t2855 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2855, gem_string("expr"));
#line 919 "compiler/main.gem"
    GemVal _t2856 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2856, gem_string("setup"));
#line 921 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 922 "compiler/main.gem"
    GemVal _t2857 = gem_v_node;
    GemVal _t2858[] = {gem_table_get(_t2857, gem_string("right"))};
    GemVal _t2859 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2859.fn(_t2859.env, _t2858, 1);
#line 923 "compiler/main.gem"
    GemVal _t2860 = gem_table_new();
    GemVal _t2861 = gem_v_rc_r;
    GemVal _t2862[] = {gem_string("gem_add("), gem_v_lc, gem_string(", "), gem_table_get(_t2861, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2860, gem_string("expr"), gem_interp(5, _t2862));
    GemVal _t2863 = gem_v_rc_r;
    gem_table_set(_t2860, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2863, gem_string("setup"))));
        GemVal _t2864 = _t2860;
        gem_pop_frame();
        return _t2864;
    } else {
#line 924 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 925 "compiler/main.gem"
    GemVal _t2865 = gem_v_node;
    GemVal _t2866[] = {gem_table_get(_t2865, gem_string("right"))};
    GemVal _t2867 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2867.fn(_t2867.env, _t2866, 1);
#line 926 "compiler/main.gem"
    GemVal _t2868 = gem_table_new();
    GemVal _t2869 = gem_v_rc_r;
    GemVal _t2870[] = {gem_string("gem_sub("), gem_v_lc, gem_string(", "), gem_table_get(_t2869, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2868, gem_string("expr"), gem_interp(5, _t2870));
    GemVal _t2871 = gem_v_rc_r;
    gem_table_set(_t2868, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2871, gem_string("setup"))));
            GemVal _t2872 = _t2868;
            gem_pop_frame();
            return _t2872;
        } else {
#line 927 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 928 "compiler/main.gem"
    GemVal _t2873 = gem_v_node;
    GemVal _t2874[] = {gem_table_get(_t2873, gem_string("right"))};
    GemVal _t2875 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2875.fn(_t2875.env, _t2874, 1);
#line 929 "compiler/main.gem"
    GemVal _t2876 = gem_table_new();
    GemVal _t2877 = gem_v_rc_r;
    GemVal _t2878[] = {gem_string("gem_mul("), gem_v_lc, gem_string(", "), gem_table_get(_t2877, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2876, gem_string("expr"), gem_interp(5, _t2878));
    GemVal _t2879 = gem_v_rc_r;
    gem_table_set(_t2876, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2879, gem_string("setup"))));
                GemVal _t2880 = _t2876;
                gem_pop_frame();
                return _t2880;
            } else {
#line 930 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 931 "compiler/main.gem"
    GemVal _t2881 = gem_v_node;
    GemVal _t2882[] = {gem_table_get(_t2881, gem_string("right"))};
    GemVal _t2883 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2883.fn(_t2883.env, _t2882, 1);
#line 932 "compiler/main.gem"
    GemVal _t2884 = gem_table_new();
    GemVal _t2885 = gem_v_rc_r;
    GemVal _t2886[] = {gem_string("gem_div("), gem_v_lc, gem_string(", "), gem_table_get(_t2885, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2884, gem_string("expr"), gem_interp(5, _t2886));
    GemVal _t2887 = gem_v_rc_r;
    gem_table_set(_t2884, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2887, gem_string("setup"))));
                    GemVal _t2888 = _t2884;
                    gem_pop_frame();
                    return _t2888;
                } else {
#line 933 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 934 "compiler/main.gem"
    GemVal _t2889 = gem_v_node;
    GemVal _t2890[] = {gem_table_get(_t2889, gem_string("right"))};
    GemVal _t2891 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2891.fn(_t2891.env, _t2890, 1);
#line 935 "compiler/main.gem"
    GemVal _t2892 = gem_table_new();
    GemVal _t2893 = gem_v_rc_r;
    GemVal _t2894[] = {gem_string("gem_mod("), gem_v_lc, gem_string(", "), gem_table_get(_t2893, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2892, gem_string("expr"), gem_interp(5, _t2894));
    GemVal _t2895 = gem_v_rc_r;
    gem_table_set(_t2892, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2895, gem_string("setup"))));
                        GemVal _t2896 = _t2892;
                        gem_pop_frame();
                        return _t2896;
                    } else {
#line 936 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 937 "compiler/main.gem"
    GemVal _t2897 = gem_v_node;
    GemVal _t2898[] = {gem_table_get(_t2897, gem_string("right"))};
    GemVal _t2899 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2899.fn(_t2899.env, _t2898, 1);
#line 938 "compiler/main.gem"
    GemVal _t2900 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2900.fn(_t2900.env, NULL, 0);
#line 939 "compiler/main.gem"
    GemVal _t2901 = gem_v_rc_r;
    GemVal _t2902 = gem_v_rc_r;
    GemVal _t2903[] = {gem_string("    GemVal "), gem_v_t, gem_string("[] = {"), gem_table_get(_t2902, gem_string("expr")), gem_string(", "), gem_v_lc, gem_string("};\n")};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2901, gem_string("setup"))), gem_interp(7, _t2903));
#line 940 "compiler/main.gem"
    GemVal _t2904 = gem_table_new();
    GemVal _t2905[] = {gem_string("gem_in_fn(NULL, "), gem_v_t, gem_string(", 2)")};
    gem_table_set(_t2904, gem_string("expr"), gem_interp(3, _t2905));
    gem_table_set(_t2904, gem_string("setup"), gem_v_setup);
                            GemVal _t2906 = _t2904;
                            gem_pop_frame();
                            return _t2906;
                        } else {
#line 941 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 942 "compiler/main.gem"
    GemVal _t2907 = gem_v_node;
    GemVal _t2908[] = {gem_table_get(_t2907, gem_string("right"))};
    GemVal _t2909 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2909.fn(_t2909.env, _t2908, 1);
#line 943 "compiler/main.gem"
    GemVal _t2910 = gem_table_new();
    GemVal _t2911 = gem_v_rc_r;
    GemVal _t2912[] = {gem_string("gem_eq("), gem_v_lc, gem_string(", "), gem_table_get(_t2911, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2910, gem_string("expr"), gem_interp(5, _t2912));
    GemVal _t2913 = gem_v_rc_r;
    gem_table_set(_t2910, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2913, gem_string("setup"))));
                                GemVal _t2914 = _t2910;
                                gem_pop_frame();
                                return _t2914;
                            } else {
#line 944 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 945 "compiler/main.gem"
    GemVal _t2915 = gem_v_node;
    GemVal _t2916[] = {gem_table_get(_t2915, gem_string("right"))};
    GemVal _t2917 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2917.fn(_t2917.env, _t2916, 1);
#line 946 "compiler/main.gem"
    GemVal _t2918 = gem_table_new();
    GemVal _t2919 = gem_v_rc_r;
    GemVal _t2920[] = {gem_string("gem_neq("), gem_v_lc, gem_string(", "), gem_table_get(_t2919, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2918, gem_string("expr"), gem_interp(5, _t2920));
    GemVal _t2921 = gem_v_rc_r;
    gem_table_set(_t2918, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2921, gem_string("setup"))));
                                    GemVal _t2922 = _t2918;
                                    gem_pop_frame();
                                    return _t2922;
                                } else {
#line 947 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 948 "compiler/main.gem"
    GemVal _t2923 = gem_v_node;
    GemVal _t2924[] = {gem_table_get(_t2923, gem_string("right"))};
    GemVal _t2925 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2925.fn(_t2925.env, _t2924, 1);
#line 949 "compiler/main.gem"
    GemVal _t2926 = gem_table_new();
    GemVal _t2927 = gem_v_rc_r;
    GemVal _t2928[] = {gem_string("gem_lt("), gem_v_lc, gem_string(", "), gem_table_get(_t2927, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2926, gem_string("expr"), gem_interp(5, _t2928));
    GemVal _t2929 = gem_v_rc_r;
    gem_table_set(_t2926, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2929, gem_string("setup"))));
                                        GemVal _t2930 = _t2926;
                                        gem_pop_frame();
                                        return _t2930;
                                    } else {
#line 950 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 951 "compiler/main.gem"
    GemVal _t2931 = gem_v_node;
    GemVal _t2932[] = {gem_table_get(_t2931, gem_string("right"))};
    GemVal _t2933 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2933.fn(_t2933.env, _t2932, 1);
#line 952 "compiler/main.gem"
    GemVal _t2934 = gem_table_new();
    GemVal _t2935 = gem_v_rc_r;
    GemVal _t2936[] = {gem_string("gem_gt("), gem_v_lc, gem_string(", "), gem_table_get(_t2935, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2934, gem_string("expr"), gem_interp(5, _t2936));
    GemVal _t2937 = gem_v_rc_r;
    gem_table_set(_t2934, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2937, gem_string("setup"))));
                                            GemVal _t2938 = _t2934;
                                            gem_pop_frame();
                                            return _t2938;
                                        } else {
#line 953 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 954 "compiler/main.gem"
    GemVal _t2939 = gem_v_node;
    GemVal _t2940[] = {gem_table_get(_t2939, gem_string("right"))};
    GemVal _t2941 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2941.fn(_t2941.env, _t2940, 1);
#line 955 "compiler/main.gem"
    GemVal _t2942 = gem_table_new();
    GemVal _t2943 = gem_v_rc_r;
    GemVal _t2944[] = {gem_string("gem_le("), gem_v_lc, gem_string(", "), gem_table_get(_t2943, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2942, gem_string("expr"), gem_interp(5, _t2944));
    GemVal _t2945 = gem_v_rc_r;
    gem_table_set(_t2942, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2945, gem_string("setup"))));
                                                GemVal _t2946 = _t2942;
                                                gem_pop_frame();
                                                return _t2946;
                                            } else {
#line 956 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 957 "compiler/main.gem"
    GemVal _t2947 = gem_v_node;
    GemVal _t2948[] = {gem_table_get(_t2947, gem_string("right"))};
    GemVal _t2949 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t2949.fn(_t2949.env, _t2948, 1);
#line 958 "compiler/main.gem"
    GemVal _t2950 = gem_table_new();
    GemVal _t2951 = gem_v_rc_r;
    GemVal _t2952[] = {gem_string("gem_ge("), gem_v_lc, gem_string(", "), gem_table_get(_t2951, gem_string("expr")), gem_string(")")};
    gem_table_set(_t2950, gem_string("expr"), gem_interp(5, _t2952));
    GemVal _t2953 = gem_v_rc_r;
    gem_table_set(_t2950, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2953, gem_string("setup"))));
                                                    GemVal _t2954 = _t2950;
                                                    gem_pop_frame();
                                                    return _t2954;
                                                } else {
#line 959 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 960 "compiler/main.gem"
    GemVal _t2955 = gem_v_node;
    GemVal _t2956[] = {gem_table_get(_t2955, gem_string("right"))};
    GemVal _t2957 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t2957.fn(_t2957.env, _t2956, 1);
#line 961 "compiler/main.gem"
    GemVal _t2958 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t2958.fn(_t2958.env, NULL, 0);
#line 962 "compiler/main.gem"
                                                        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 963 "compiler/main.gem"
    GemVal _t2959[] = {gem_v_b, gem_v_ls};
                                                        (void)(gem_buf_push_fn(NULL, _t2959, 2));
#line 964 "compiler/main.gem"
    GemVal _t2960[] = {gem_string("    GemVal "), gem_v_t, gem_string(";\n")};
    GemVal _t2961[] = {gem_v_b, gem_interp(3, _t2960)};
                                                        (void)(gem_buf_push_fn(NULL, _t2961, 2));
#line 965 "compiler/main.gem"
    GemVal _t2962[] = {gem_string("    if (!gem_truthy("), gem_v_lc, gem_string(")) {\n")};
    GemVal _t2963[] = {gem_v_b, gem_interp(3, _t2962)};
                                                        (void)(gem_buf_push_fn(NULL, _t2963, 2));
#line 966 "compiler/main.gem"
    GemVal _t2964[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_v_lc, gem_string(";\n")};
    GemVal _t2965[] = {gem_v_b, gem_interp(5, _t2964)};
                                                        (void)(gem_buf_push_fn(NULL, _t2965, 2));
#line 967 "compiler/main.gem"
    GemVal _t2966[] = {gem_v_b, gem_string("    } else {\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t2966, 2));
#line 968 "compiler/main.gem"
    GemVal _t2967 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t2967, gem_string("setup")), gem_string("")))) {
#line 969 "compiler/main.gem"
    GemVal _t2968 = gem_v_rc_r;
    GemVal _t2969[] = {gem_table_get(_t2968, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t2970[] = {gem_v_b, gem_str_replace_fn(NULL, _t2969, 3)};
                                                            (void)(gem_buf_push_fn(NULL, _t2970, 2));
                                                        }
#line 971 "compiler/main.gem"
    GemVal _t2971 = gem_v_rc_r;
    GemVal _t2972[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_table_get(_t2971, gem_string("expr")), gem_string(";\n")};
    GemVal _t2973[] = {gem_v_b, gem_interp(5, _t2972)};
                                                        (void)(gem_buf_push_fn(NULL, _t2973, 2));
#line 972 "compiler/main.gem"
    GemVal _t2974[] = {gem_v_b, gem_string("    }\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t2974, 2));
#line 973 "compiler/main.gem"
    GemVal _t2975 = gem_table_new();
    gem_table_set(_t2975, gem_string("expr"), gem_v_t);
    GemVal _t2976[] = {gem_v_b};
    gem_table_set(_t2975, gem_string("setup"), gem_buf_str_fn(NULL, _t2976, 1));
                                                        GemVal _t2977 = _t2975;
                                                        gem_pop_frame();
                                                        return _t2977;
                                                    } else {
#line 974 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 975 "compiler/main.gem"
    GemVal _t2978 = gem_v_node;
    GemVal _t2979[] = {gem_table_get(_t2978, gem_string("right"))};
    GemVal _t2980 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t2980.fn(_t2980.env, _t2979, 1);
#line 976 "compiler/main.gem"
    GemVal _t2981 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t2981.fn(_t2981.env, NULL, 0);
#line 977 "compiler/main.gem"
                                                            GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 978 "compiler/main.gem"
    GemVal _t2982[] = {gem_v_b, gem_v_ls};
                                                            (void)(gem_buf_push_fn(NULL, _t2982, 2));
#line 979 "compiler/main.gem"
    GemVal _t2983[] = {gem_string("    GemVal "), gem_v_t, gem_string(";\n")};
    GemVal _t2984[] = {gem_v_b, gem_interp(3, _t2983)};
                                                            (void)(gem_buf_push_fn(NULL, _t2984, 2));
#line 980 "compiler/main.gem"
    GemVal _t2985[] = {gem_string("    if (gem_truthy("), gem_v_lc, gem_string(")) {\n")};
    GemVal _t2986[] = {gem_v_b, gem_interp(3, _t2985)};
                                                            (void)(gem_buf_push_fn(NULL, _t2986, 2));
#line 981 "compiler/main.gem"
    GemVal _t2987[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_v_lc, gem_string(";\n")};
    GemVal _t2988[] = {gem_v_b, gem_interp(5, _t2987)};
                                                            (void)(gem_buf_push_fn(NULL, _t2988, 2));
#line 982 "compiler/main.gem"
    GemVal _t2989[] = {gem_v_b, gem_string("    } else {\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t2989, 2));
#line 983 "compiler/main.gem"
    GemVal _t2990 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t2990, gem_string("setup")), gem_string("")))) {
#line 984 "compiler/main.gem"
    GemVal _t2991 = gem_v_rc_r;
    GemVal _t2992[] = {gem_table_get(_t2991, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t2993[] = {gem_v_b, gem_str_replace_fn(NULL, _t2992, 3)};
                                                                (void)(gem_buf_push_fn(NULL, _t2993, 2));
                                                            }
#line 986 "compiler/main.gem"
    GemVal _t2994 = gem_v_rc_r;
    GemVal _t2995[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_table_get(_t2994, gem_string("expr")), gem_string(";\n")};
    GemVal _t2996[] = {gem_v_b, gem_interp(5, _t2995)};
                                                            (void)(gem_buf_push_fn(NULL, _t2996, 2));
#line 987 "compiler/main.gem"
    GemVal _t2997[] = {gem_v_b, gem_string("    }\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t2997, 2));
#line 988 "compiler/main.gem"
    GemVal _t2998 = gem_table_new();
    gem_table_set(_t2998, gem_string("expr"), gem_v_t);
    GemVal _t2999[] = {gem_v_b};
    gem_table_set(_t2998, gem_string("setup"), gem_buf_str_fn(NULL, _t2999, 1));
                                                            GemVal _t3000 = _t2998;
                                                            gem_pop_frame();
                                                            return _t3000;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 991 "compiler/main.gem"
    GemVal _t3001[] = {gem_string("unknown binary operator: "), gem_v_op};
    GemVal _t3002[] = {gem_interp(2, _t3001)};
    GemVal _t3003 = gem_error_at_fn("compiler/main.gem", 991, _t3002, 1);
    gem_pop_frame();
    return _t3003;
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
#line 997 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 998 "compiler/main.gem"
    GemVal gem_v__for_items_55 = gem_v_stmts;
#line 998 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 998 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3005[] = {gem_v__for_items_55};
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t3005, 1)))) break;
#line 998 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 998 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 999 "compiler/main.gem"
    GemVal _t3006[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3007 = (*gem_v_compile_stmt);
    GemVal _t3008[] = {gem_v_b, _t3007.fn(_t3007.env, _t3006, 2)};
        (void)(gem_buf_push_fn(NULL, _t3008, 2));
#line 1000 "compiler/main.gem"
    GemVal _t3009[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3009, 2));
    }

#line 1002 "compiler/main.gem"
    GemVal _t3010[] = {gem_v_b};
    GemVal _t3011 = gem_buf_str_fn(NULL, _t3010, 1);
    gem_pop_frame();
    return _t3011;
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
#line 1006 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_return_pos))) {
#line 1007 "compiler/main.gem"
    GemVal _t3013[] = {gem_v_stmts, gem_v_indent};
    GemVal _t3014 = (*gem_v_compile_stmts);
        GemVal _t3015 = _t3014.fn(_t3014.env, _t3013, 2);
        gem_pop_frame();
        return _t3015;
    }
#line 1009 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1010 "compiler/main.gem"
    GemVal _t3016[] = {gem_v_stmts};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3016, 1), gem_int(0)))) {
#line 1011 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1011 "compiler/main.gem"
    GemVal _t3017[] = {gem_v_stmts};
        GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t3017, 1), gem_int(1));
#line 1011 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1011 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_56;
#line 1011 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1012 "compiler/main.gem"
    GemVal _t3018[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_indent};
    GemVal _t3019 = (*gem_v_compile_stmt);
    GemVal _t3020[] = {gem_v_b, _t3019.fn(_t3019.env, _t3018, 2)};
            (void)(gem_buf_push_fn(NULL, _t3020, 2));
#line 1013 "compiler/main.gem"
    GemVal _t3021[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3021, 2));
        }

#line 1015 "compiler/main.gem"
    GemVal _t3022[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t3023 = (*gem_v_compile_stmt_return);
    GemVal _t3024[] = {gem_v_b, _t3023.fn(_t3023.env, _t3022, 2)};
        (void)(gem_buf_push_fn(NULL, _t3024, 2));
#line 1016 "compiler/main.gem"
    GemVal _t3025[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3025, 2));
    } else {
#line 1018 "compiler/main.gem"
    GemVal _t3026[] = {gem_v_indent};
        GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3026, 1);
#line 1019 "compiler/main.gem"
    GemVal _t3027[] = {gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;\n")};
    GemVal _t3028[] = {gem_v_b, gem_interp(4, _t3027)};
        (void)(gem_buf_push_fn(NULL, _t3028, 2));
    }
#line 1021 "compiler/main.gem"
    GemVal _t3029[] = {gem_v_b};
    GemVal _t3030 = gem_buf_str_fn(NULL, _t3029, 1);
    gem_pop_frame();
    return _t3030;
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
#line 1027 "compiler/main.gem"
    GemVal _t3032[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3032, 1);
#line 1028 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1029 "compiler/main.gem"
    GemVal _t3033 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3033, gem_string("line")), GEM_NIL))) {
#line 1030 "compiler/main.gem"
    GemVal _t3034 = gem_v_node;
    GemVal _t3035[] = {(*gem_v_source_name)};
    GemVal _t3036[] = {gem_string("#line "), gem_table_get(_t3034, gem_string("line")), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3035, 1), gem_string("\"\n")};
        gem_v_line_dir = gem_interp(5, _t3036);
    }
#line 1033 "compiler/main.gem"
    GemVal gem_v__match_57 = gem_v_node;
    GemVal _t3037[] = {gem_v__match_57};
    GemVal _t3039;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3037, 1), gem_string("table")))) {
        _t3039 = gem_eq(gem_type_fn(NULL, _t3037, 1), gem_string("table"));
    } else {
        GemVal _t3038[] = {gem_v__match_57, gem_string("tag")};
        _t3039 = gem_has_key_fn(NULL, _t3038, 2);
    }
    GemVal _t3040;
    if (!gem_truthy(_t3039)) {
        _t3040 = _t3039;
    } else {
        _t3040 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3042;
    if (!gem_truthy(_t3040)) {
        _t3042 = _t3040;
    } else {
        GemVal _t3041[] = {gem_v__match_57, gem_string("name")};
        _t3042 = gem_has_key_fn(NULL, _t3041, 2);
    }
    GemVal _t3044;
    if (!gem_truthy(_t3042)) {
        _t3044 = _t3042;
    } else {
        GemVal _t3043[] = {gem_v__match_57, gem_string("value")};
        _t3044 = gem_has_key_fn(NULL, _t3043, 2);
    }
    if (gem_truthy(_t3044)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1035 "compiler/main.gem"
    GemVal _t3045[] = {gem_v_value};
    GemVal _t3046 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3046.fn(_t3046.env, _t3045, 1);
#line 1036 "compiler/main.gem"
    GemVal _t3047[] = {gem_v_name};
    GemVal _t3048 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3048.fn(_t3048.env, _t3047, 1);
#line 1037 "compiler/main.gem"
    GemVal _t3050;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t3050 = (*gem_v_in_top_level);
    } else {
        GemVal _t3049[] = {(*gem_v_top_level_vars), gem_v_name};
        _t3050 = gem_fn__mod_codegen_set_contains(NULL, _t3049, 2);
    }
        if (gem_truthy(_t3050)) {
#line 1038 "compiler/main.gem"
    GemVal _t3051[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3051, 2))) {
#line 1039 "compiler/main.gem"
    GemVal _t3052 = gem_v_r;
    GemVal _t3053 = gem_v_r;
    GemVal _t3054[] = {gem_v_line_dir, gem_table_get(_t3052, gem_string("setup")), gem_v_p, gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get(_t3053, gem_string("expr")), gem_string(";")};
                GemVal _t3055 = gem_interp(11, _t3054);
                gem_pop_frame();
                return _t3055;
            }
#line 1041 "compiler/main.gem"
    GemVal _t3056 = gem_v_r;
    GemVal _t3057 = gem_v_r;
    GemVal _t3058[] = {gem_v_line_dir, gem_table_get(_t3056, gem_string("setup")), gem_v_p, gem_v_mname, gem_string(" = "), gem_table_get(_t3057, gem_string("expr")), gem_string(";")};
            GemVal _t3059 = gem_interp(7, _t3058);
            gem_pop_frame();
            return _t3059;
        }
#line 1043 "compiler/main.gem"
    GemVal _t3060[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3060, 2))) {
#line 1044 "compiler/main.gem"
    GemVal _t3061 = gem_v_r;
    GemVal _t3062 = gem_v_r;
    GemVal _t3063[] = {gem_v_line_dir, gem_table_get(_t3061, gem_string("setup")), gem_v_p, gem_string("GemVal *"), gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get(_t3062, gem_string("expr")), gem_string(";")};
            GemVal _t3064 = gem_interp(12, _t3063);
            gem_pop_frame();
            return _t3064;
        }
#line 1046 "compiler/main.gem"
    GemVal _t3065 = gem_v_r;
    GemVal _t3066 = gem_v_r;
    GemVal _t3067[] = {gem_v_line_dir, gem_table_get(_t3065, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_mname, gem_string(" = "), gem_table_get(_t3066, gem_string("expr")), gem_string(";")};
        GemVal _t3068 = gem_interp(8, _t3067);
        gem_pop_frame();
        return _t3068;
    } else {
    GemVal _t3069[] = {gem_v__match_57};
    GemVal _t3071;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3069, 1), gem_string("table")))) {
        _t3071 = gem_eq(gem_type_fn(NULL, _t3069, 1), gem_string("table"));
    } else {
        GemVal _t3070[] = {gem_v__match_57, gem_string("tag")};
        _t3071 = gem_has_key_fn(NULL, _t3070, 2);
    }
    GemVal _t3072;
    if (!gem_truthy(_t3071)) {
        _t3072 = _t3071;
    } else {
        _t3072 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3074;
    if (!gem_truthy(_t3072)) {
        _t3074 = _t3072;
    } else {
        GemVal _t3073[] = {gem_v__match_57, gem_string("name")};
        _t3074 = gem_has_key_fn(NULL, _t3073, 2);
    }
    GemVal _t3076;
    if (!gem_truthy(_t3074)) {
        _t3076 = _t3074;
    } else {
        GemVal _t3075[] = {gem_v__match_57, gem_string("value")};
        _t3076 = gem_has_key_fn(NULL, _t3075, 2);
    }
    if (gem_truthy(_t3076)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1048 "compiler/main.gem"
    GemVal _t3077[] = {gem_v_value};
    GemVal _t3078 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3078.fn(_t3078.env, _t3077, 1);
#line 1049 "compiler/main.gem"
    GemVal _t3079[] = {gem_v_name};
    GemVal _t3080 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3080.fn(_t3080.env, _t3079, 1);
#line 1050 "compiler/main.gem"
    GemVal _t3081[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3083;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3081, 2))) {
        _t3083 = gem_fn__mod_codegen_set_contains(NULL, _t3081, 2);
    } else {
        GemVal _t3082[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3083 = gem_fn__mod_codegen_set_contains(NULL, _t3082, 2);
    }
        if (gem_truthy(_t3083)) {
#line 1051 "compiler/main.gem"
    GemVal _t3084 = gem_v_r;
    GemVal _t3085 = gem_v_r;
    GemVal _t3086[] = {gem_v_line_dir, gem_table_get(_t3084, gem_string("setup")), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get(_t3085, gem_string("expr")), gem_string(";")};
            GemVal _t3087 = gem_interp(8, _t3086);
            gem_pop_frame();
            return _t3087;
        }
#line 1053 "compiler/main.gem"
    GemVal _t3088 = gem_v_r;
    GemVal _t3089 = gem_v_r;
    GemVal _t3090[] = {gem_v_line_dir, gem_table_get(_t3088, gem_string("setup")), gem_v_p, gem_v_mname, gem_string(" = "), gem_table_get(_t3089, gem_string("expr")), gem_string(";")};
        GemVal _t3091 = gem_interp(7, _t3090);
        gem_pop_frame();
        return _t3091;
    } else {
    GemVal _t3092[] = {gem_v__match_57};
    GemVal _t3094;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3092, 1), gem_string("table")))) {
        _t3094 = gem_eq(gem_type_fn(NULL, _t3092, 1), gem_string("table"));
    } else {
        GemVal _t3093[] = {gem_v__match_57, gem_string("tag")};
        _t3094 = gem_has_key_fn(NULL, _t3093, 2);
    }
    GemVal _t3095;
    if (!gem_truthy(_t3094)) {
        _t3095 = _t3094;
    } else {
        _t3095 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t3097;
    if (!gem_truthy(_t3095)) {
        _t3097 = _t3095;
    } else {
        GemVal _t3096[] = {gem_v__match_57, gem_string("object")};
        _t3097 = gem_has_key_fn(NULL, _t3096, 2);
    }
    GemVal _t3099;
    if (!gem_truthy(_t3097)) {
        _t3099 = _t3097;
    } else {
        GemVal _t3098[] = {gem_v__match_57, gem_string("value")};
        _t3099 = gem_has_key_fn(NULL, _t3098, 2);
    }
    GemVal _t3101;
    if (!gem_truthy(_t3099)) {
        _t3101 = _t3099;
    } else {
        GemVal _t3100[] = {gem_v__match_57, gem_string("field")};
        _t3101 = gem_has_key_fn(NULL, _t3100, 2);
    }
    if (gem_truthy(_t3101)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_57, gem_string("field"));
#line 1055 "compiler/main.gem"
    GemVal _t3102[] = {gem_v_object};
    GemVal _t3103 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3103.fn(_t3103.env, _t3102, 1);
#line 1056 "compiler/main.gem"
    GemVal _t3104[] = {gem_v_value};
    GemVal _t3105 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3105.fn(_t3105.env, _t3104, 1);
#line 1057 "compiler/main.gem"
    GemVal _t3106[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t3106, 1);
#line 1058 "compiler/main.gem"
    GemVal _t3107 = gem_v_obj_r;
    GemVal _t3108 = gem_v_val_r;
    GemVal _t3109 = gem_v_obj_r;
    GemVal _t3110 = gem_v_val_r;
    GemVal _t3111[] = {gem_v_line_dir, gem_table_get(_t3107, gem_string("setup")), gem_table_get(_t3108, gem_string("setup")), gem_v_p, gem_string("gem_table_set("), gem_table_get(_t3109, gem_string("expr")), gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"), "), gem_table_get(_t3110, gem_string("expr")), gem_string(");")};
        GemVal _t3112 = gem_interp(11, _t3111);
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
        _t3116 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t3118;
    if (!gem_truthy(_t3116)) {
        _t3118 = _t3116;
    } else {
        GemVal _t3117[] = {gem_v__match_57, gem_string("object")};
        _t3118 = gem_has_key_fn(NULL, _t3117, 2);
    }
    GemVal _t3120;
    if (!gem_truthy(_t3118)) {
        _t3120 = _t3118;
    } else {
        GemVal _t3119[] = {gem_v__match_57, gem_string("key")};
        _t3120 = gem_has_key_fn(NULL, _t3119, 2);
    }
    GemVal _t3122;
    if (!gem_truthy(_t3120)) {
        _t3122 = _t3120;
    } else {
        GemVal _t3121[] = {gem_v__match_57, gem_string("value")};
        _t3122 = gem_has_key_fn(NULL, _t3121, 2);
    }
    if (gem_truthy(_t3122)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_57, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1060 "compiler/main.gem"
    GemVal _t3123[] = {gem_v_object};
    GemVal _t3124 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3124.fn(_t3124.env, _t3123, 1);
#line 1061 "compiler/main.gem"
    GemVal _t3125[] = {gem_v_key};
    GemVal _t3126 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t3126.fn(_t3126.env, _t3125, 1);
#line 1062 "compiler/main.gem"
    GemVal _t3127[] = {gem_v_value};
    GemVal _t3128 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3128.fn(_t3128.env, _t3127, 1);
#line 1063 "compiler/main.gem"
    GemVal _t3129 = gem_v_obj_r;
    GemVal _t3130 = gem_v_key_r;
    GemVal _t3131 = gem_v_val_r;
    GemVal _t3132 = gem_v_obj_r;
    GemVal _t3133 = gem_v_key_r;
    GemVal _t3134 = gem_v_val_r;
    GemVal _t3135[] = {gem_v_line_dir, gem_table_get(_t3129, gem_string("setup")), gem_table_get(_t3130, gem_string("setup")), gem_table_get(_t3131, gem_string("setup")), gem_v_p, gem_string("gem_table_set("), gem_table_get(_t3132, gem_string("expr")), gem_string(", "), gem_table_get(_t3133, gem_string("expr")), gem_string(", "), gem_table_get(_t3134, gem_string("expr")), gem_string(");")};
        GemVal _t3136 = gem_interp(12, _t3135);
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
        _t3140 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3140)) {
#line 1065 "compiler/main.gem"
    GemVal _t3141[] = {gem_v_line_dir};
    GemVal _t3142[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3143 = (*gem_v_compile_if);
        GemVal _t3144 = gem_add(gem_interp(1, _t3141), _t3143.fn(_t3143.env, _t3142, 3));
        gem_pop_frame();
        return _t3144;
    } else {
    GemVal _t3145[] = {gem_v__match_57};
    GemVal _t3147;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3145, 1), gem_string("table")))) {
        _t3147 = gem_eq(gem_type_fn(NULL, _t3145, 1), gem_string("table"));
    } else {
        GemVal _t3146[] = {gem_v__match_57, gem_string("tag")};
        _t3147 = gem_has_key_fn(NULL, _t3146, 2);
    }
    GemVal _t3148;
    if (!gem_truthy(_t3147)) {
        _t3148 = _t3147;
    } else {
        _t3148 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3148)) {
#line 1067 "compiler/main.gem"
    GemVal _t3149[] = {gem_v_line_dir};
    GemVal _t3150[] = {gem_v_node, gem_v_indent};
    GemVal _t3151 = (*gem_v_compile_while);
        GemVal _t3152 = gem_add(gem_interp(1, _t3149), _t3151.fn(_t3151.env, _t3150, 2));
        gem_pop_frame();
        return _t3152;
    } else {
    GemVal _t3153[] = {gem_v__match_57};
    GemVal _t3155;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3153, 1), gem_string("table")))) {
        _t3155 = gem_eq(gem_type_fn(NULL, _t3153, 1), gem_string("table"));
    } else {
        GemVal _t3154[] = {gem_v__match_57, gem_string("tag")};
        _t3155 = gem_has_key_fn(NULL, _t3154, 2);
    }
    GemVal _t3156;
    if (!gem_truthy(_t3155)) {
        _t3156 = _t3155;
    } else {
        _t3156 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3156)) {
#line 1069 "compiler/main.gem"
    GemVal _t3157[] = {gem_v_line_dir};
    GemVal _t3158[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3159 = (*gem_v_compile_match);
        GemVal _t3160 = gem_add(gem_interp(1, _t3157), _t3159.fn(_t3159.env, _t3158, 3));
        gem_pop_frame();
        return _t3160;
    } else {
    GemVal _t3161[] = {gem_v__match_57};
    GemVal _t3163;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3161, 1), gem_string("table")))) {
        _t3163 = gem_eq(gem_type_fn(NULL, _t3161, 1), gem_string("table"));
    } else {
        GemVal _t3162[] = {gem_v__match_57, gem_string("tag")};
        _t3163 = gem_has_key_fn(NULL, _t3162, 2);
    }
    GemVal _t3164;
    if (!gem_truthy(_t3163)) {
        _t3164 = _t3163;
    } else {
        _t3164 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3164)) {
#line 1071 "compiler/main.gem"
    GemVal _t3165[] = {gem_v_line_dir};
    GemVal _t3166[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3167 = (*gem_v_compile_receive_match);
        GemVal _t3168 = gem_add(gem_interp(1, _t3165), _t3167.fn(_t3167.env, _t3166, 3));
        gem_pop_frame();
        return _t3168;
    } else {
    GemVal _t3169[] = {gem_v__match_57};
    GemVal _t3171;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3169, 1), gem_string("table")))) {
        _t3171 = gem_eq(gem_type_fn(NULL, _t3169, 1), gem_string("table"));
    } else {
        GemVal _t3170[] = {gem_v__match_57, gem_string("tag")};
        _t3171 = gem_has_key_fn(NULL, _t3170, 2);
    }
    GemVal _t3172;
    if (!gem_truthy(_t3171)) {
        _t3172 = _t3171;
    } else {
        _t3172 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3172)) {
#line 1073 "compiler/main.gem"
        GemVal _t3173 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t3173;
    } else {
    GemVal _t3174[] = {gem_v__match_57};
    GemVal _t3176;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3174, 1), gem_string("table")))) {
        _t3176 = gem_eq(gem_type_fn(NULL, _t3174, 1), gem_string("table"));
    } else {
        GemVal _t3175[] = {gem_v__match_57, gem_string("tag")};
        _t3176 = gem_has_key_fn(NULL, _t3175, 2);
    }
    GemVal _t3177;
    if (!gem_truthy(_t3176)) {
        _t3177 = _t3176;
    } else {
        _t3177 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3177)) {
#line 1075 "compiler/main.gem"
        GemVal _t3178 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t3178;
    } else {
    GemVal _t3179[] = {gem_v__match_57};
    GemVal _t3181;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3179, 1), gem_string("table")))) {
        _t3181 = gem_eq(gem_type_fn(NULL, _t3179, 1), gem_string("table"));
    } else {
        GemVal _t3180[] = {gem_v__match_57, gem_string("tag")};
        _t3181 = gem_has_key_fn(NULL, _t3180, 2);
    }
    GemVal _t3182;
    if (!gem_truthy(_t3181)) {
        _t3182 = _t3181;
    } else {
        _t3182 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3184;
    if (!gem_truthy(_t3182)) {
        _t3184 = _t3182;
    } else {
        GemVal _t3183[] = {gem_v__match_57, gem_string("value")};
        _t3184 = gem_has_key_fn(NULL, _t3183, 2);
    }
    if (gem_truthy(_t3184)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1077 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1078 "compiler/main.gem"
    GemVal _t3185[] = {gem_v_value};
    GemVal _t3186 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3186.fn(_t3186.env, _t3185, 1);
#line 1079 "compiler/main.gem"
    GemVal _t3187 = (*gem_v_tmp);
            GemVal gem_v_t = _t3187.fn(_t3187.env, NULL, 0);
#line 1080 "compiler/main.gem"
    GemVal _t3188 = gem_v_r;
    GemVal _t3189 = gem_v_r;
    GemVal _t3190[] = {gem_v_line_dir, gem_table_get(_t3188, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t3189, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3191 = gem_interp(14, _t3190);
            gem_pop_frame();
            return _t3191;
        }
#line 1082 "compiler/main.gem"
    GemVal _t3192[] = {gem_v_line_dir, gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3193 = gem_interp(5, _t3192);
        gem_pop_frame();
        return _t3193;
    } else {
    GemVal _t3194[] = {gem_v__match_57};
    GemVal _t3196;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3194, 1), gem_string("table")))) {
        _t3196 = gem_eq(gem_type_fn(NULL, _t3194, 1), gem_string("table"));
    } else {
        GemVal _t3195[] = {gem_v__match_57, gem_string("tag")};
        _t3196 = gem_has_key_fn(NULL, _t3195, 2);
    }
    GemVal _t3197;
    if (!gem_truthy(_t3196)) {
        _t3197 = _t3196;
    } else {
        _t3197 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t3197)) {
#line 1084 "compiler/main.gem"
    GemVal _t3198[] = {gem_v_node};
    GemVal _t3199 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3199.fn(_t3199.env, _t3198, 1);
#line 1085 "compiler/main.gem"
    GemVal _t3200 = gem_v_r;
    GemVal _t3201 = gem_v_r;
    GemVal _t3202[] = {gem_v_line_dir, gem_table_get(_t3200, gem_string("setup")), gem_v_p, gem_string("(void)("), gem_table_get(_t3201, gem_string("expr")), gem_string(");")};
        GemVal _t3203 = gem_interp(6, _t3202);
        gem_pop_frame();
        return _t3203;
    } else {
    GemVal _t3204[] = {gem_v__match_57};
    GemVal _t3206;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3204, 1), gem_string("table")))) {
        _t3206 = gem_eq(gem_type_fn(NULL, _t3204, 1), gem_string("table"));
    } else {
        GemVal _t3205[] = {gem_v__match_57, gem_string("tag")};
        _t3206 = gem_has_key_fn(NULL, _t3205, 2);
    }
    GemVal _t3207;
    if (!gem_truthy(_t3206)) {
        _t3207 = _t3206;
    } else {
        _t3207 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3207)) {
#line 1087 "compiler/main.gem"
        GemVal _t3208 = gem_string("");
        gem_pop_frame();
        return _t3208;
    } else {
    GemVal _t3209[] = {gem_v__match_57};
    GemVal _t3211;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3209, 1), gem_string("table")))) {
        _t3211 = gem_eq(gem_type_fn(NULL, _t3209, 1), gem_string("table"));
    } else {
        GemVal _t3210[] = {gem_v__match_57, gem_string("tag")};
        _t3211 = gem_has_key_fn(NULL, _t3210, 2);
    }
    GemVal _t3212;
    if (!gem_truthy(_t3211)) {
        _t3212 = _t3211;
    } else {
        _t3212 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3214;
    if (!gem_truthy(_t3212)) {
        _t3214 = _t3212;
    } else {
        GemVal _t3213[] = {gem_v__match_57, gem_string("stmts")};
        _t3214 = gem_has_key_fn(NULL, _t3213, 2);
    }
    if (gem_truthy(_t3214)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_57, gem_string("stmts"));
#line 1089 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1090 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_stmts;
#line 1090 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1090 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3215[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t3215, 1)))) break;
#line 1090 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1090 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1091 "compiler/main.gem"
    GemVal _t3216[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3217 = (*gem_v_compile_stmt);
    GemVal _t3218[] = {gem_v_b, _t3217.fn(_t3217.env, _t3216, 2)};
            (void)(gem_buf_push_fn(NULL, _t3218, 2));
#line 1092 "compiler/main.gem"
    GemVal _t3219[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3219, 2));
        }

#line 1094 "compiler/main.gem"
    GemVal _t3220[] = {gem_v_b};
        GemVal _t3221 = gem_buf_str_fn(NULL, _t3220, 1);
        gem_pop_frame();
        return _t3221;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1097 "compiler/main.gem"
    GemVal _t3222[] = {gem_v_node};
    GemVal _t3223 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3223.fn(_t3223.env, _t3222, 1);
    GemVal _t3224 = gem_v_r;
    GemVal _t3225 = gem_v_r;
    GemVal _t3226[] = {gem_v_line_dir, gem_table_get(_t3224, gem_string("setup")), gem_v_p, gem_string("(void)("), gem_table_get(_t3225, gem_string("expr")), gem_string(");")};
    GemVal _t3227 = gem_interp(6, _t3226);
    gem_pop_frame();
    return _t3227;
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
#line 1104 "compiler/main.gem"
    GemVal _t3229[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3229, 1);
#line 1105 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1106 "compiler/main.gem"
    GemVal _t3230 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3230, gem_string("line")), GEM_NIL))) {
#line 1107 "compiler/main.gem"
    GemVal _t3231 = gem_v_node;
    GemVal _t3232[] = {(*gem_v_source_name)};
    GemVal _t3233[] = {gem_string("#line "), gem_table_get(_t3231, gem_string("line")), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3232, 1), gem_string("\"\n")};
        gem_v_line_dir = gem_interp(5, _t3233);
    }
#line 1110 "compiler/main.gem"
    if (gem_truthy(gem_neq((*gem_v_tco_fn_name), GEM_NIL))) {
#line 1111 "compiler/main.gem"
    GemVal _t3234[] = {gem_v_node, gem_string("call")};
    GemVal _t3237;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3234, 2))) {
        _t3237 = gem_fn__mod_codegen_is_node(NULL, _t3234, 2);
    } else {
        GemVal _t3235 = gem_v_node;
        GemVal _t3236[] = {gem_table_get(_t3235, gem_string("func")), gem_string("var")};
        _t3237 = gem_fn__mod_codegen_is_node(NULL, _t3236, 2);
    }
    GemVal _t3240;
    if (!gem_truthy(_t3237)) {
        _t3240 = _t3237;
    } else {
        GemVal _t3238 = gem_v_node;
        GemVal _t3239 = gem_table_get(_t3238, gem_string("func"));
        _t3240 = gem_eq(gem_table_get(_t3239, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3242;
    if (!gem_truthy(_t3240)) {
        _t3242 = _t3240;
    } else {
        GemVal _t3241[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3242 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3241, 2));
    }
        if (gem_truthy(_t3242)) {
#line 1112 "compiler/main.gem"
    GemVal _t3243[] = {gem_v_node, gem_v_indent};
    GemVal _t3244 = (*gem_v_emit_tco_continue);
            GemVal _t3245 = gem_add(gem_v_line_dir, _t3244.fn(_t3244.env, _t3243, 2));
            gem_pop_frame();
            return _t3245;
        }
#line 1114 "compiler/main.gem"
    GemVal _t3246[] = {gem_v_node, gem_string("return")};
    GemVal _t3248;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3246, 2))) {
        _t3248 = gem_fn__mod_codegen_is_node(NULL, _t3246, 2);
    } else {
        GemVal _t3247 = gem_v_node;
        _t3248 = gem_neq(gem_table_get(_t3247, gem_string("value")), GEM_NIL);
    }
    GemVal _t3251;
    if (!gem_truthy(_t3248)) {
        _t3251 = _t3248;
    } else {
        GemVal _t3249 = gem_v_node;
        GemVal _t3250[] = {gem_table_get(_t3249, gem_string("value")), gem_string("call")};
        _t3251 = gem_fn__mod_codegen_is_node(NULL, _t3250, 2);
    }
    GemVal _t3255;
    if (!gem_truthy(_t3251)) {
        _t3255 = _t3251;
    } else {
        GemVal _t3252 = gem_v_node;
        GemVal _t3253 = gem_table_get(_t3252, gem_string("value"));
        GemVal _t3254[] = {gem_table_get(_t3253, gem_string("func")), gem_string("var")};
        _t3255 = gem_fn__mod_codegen_is_node(NULL, _t3254, 2);
    }
    GemVal _t3259;
    if (!gem_truthy(_t3255)) {
        _t3259 = _t3255;
    } else {
        GemVal _t3256 = gem_v_node;
        GemVal _t3257 = gem_table_get(_t3256, gem_string("value"));
        GemVal _t3258 = gem_table_get(_t3257, gem_string("func"));
        _t3259 = gem_eq(gem_table_get(_t3258, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3261;
    if (!gem_truthy(_t3259)) {
        _t3261 = _t3259;
    } else {
        GemVal _t3260[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3261 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3260, 2));
    }
        if (gem_truthy(_t3261)) {
#line 1115 "compiler/main.gem"
    GemVal _t3262 = gem_v_node;
    GemVal _t3263[] = {gem_table_get(_t3262, gem_string("value")), gem_v_indent};
    GemVal _t3264 = (*gem_v_emit_tco_continue);
            GemVal _t3265 = gem_add(gem_v_line_dir, _t3264.fn(_t3264.env, _t3263, 2));
            gem_pop_frame();
            return _t3265;
        }
    }
#line 1119 "compiler/main.gem"
    GemVal gem_v__match_59 = gem_v_node;
    GemVal _t3266[] = {gem_v__match_59};
    GemVal _t3268;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3266, 1), gem_string("table")))) {
        _t3268 = gem_eq(gem_type_fn(NULL, _t3266, 1), gem_string("table"));
    } else {
        GemVal _t3267[] = {gem_v__match_59, gem_string("tag")};
        _t3268 = gem_has_key_fn(NULL, _t3267, 2);
    }
    GemVal _t3269;
    if (!gem_truthy(_t3268)) {
        _t3269 = _t3268;
    } else {
        _t3269 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3271;
    if (!gem_truthy(_t3269)) {
        _t3271 = _t3269;
    } else {
        GemVal _t3270[] = {gem_v__match_59, gem_string("name")};
        _t3271 = gem_has_key_fn(NULL, _t3270, 2);
    }
    GemVal _t3273;
    if (!gem_truthy(_t3271)) {
        _t3273 = _t3271;
    } else {
        GemVal _t3272[] = {gem_v__match_59, gem_string("value")};
        _t3273 = gem_has_key_fn(NULL, _t3272, 2);
    }
    if (gem_truthy(_t3273)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1121 "compiler/main.gem"
    GemVal _t3274[] = {gem_v_value};
    GemVal _t3275 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3275.fn(_t3275.env, _t3274, 1);
#line 1122 "compiler/main.gem"
    GemVal _t3276[] = {gem_v_name};
    GemVal _t3277 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3277.fn(_t3277.env, _t3276, 1);
#line 1123 "compiler/main.gem"
    GemVal _t3278[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3278, 2))) {
#line 1124 "compiler/main.gem"
    GemVal _t3279 = gem_v_r;
    GemVal _t3280 = gem_v_r;
    GemVal _t3281[] = {gem_v_line_dir, gem_table_get(_t3279, gem_string("setup")), gem_v_p, gem_string("GemVal *"), gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get(_t3280, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
            GemVal _t3282 = gem_interp(16, _t3281);
            gem_pop_frame();
            return _t3282;
        }
#line 1126 "compiler/main.gem"
    GemVal _t3283 = gem_v_r;
    GemVal _t3284 = gem_v_r;
    GemVal _t3285[] = {gem_v_line_dir, gem_table_get(_t3283, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_mname, gem_string(" = "), gem_table_get(_t3284, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3286 = gem_interp(12, _t3285);
        gem_pop_frame();
        return _t3286;
    } else {
    GemVal _t3287[] = {gem_v__match_59};
    GemVal _t3289;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3287, 1), gem_string("table")))) {
        _t3289 = gem_eq(gem_type_fn(NULL, _t3287, 1), gem_string("table"));
    } else {
        GemVal _t3288[] = {gem_v__match_59, gem_string("tag")};
        _t3289 = gem_has_key_fn(NULL, _t3288, 2);
    }
    GemVal _t3290;
    if (!gem_truthy(_t3289)) {
        _t3290 = _t3289;
    } else {
        _t3290 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3292;
    if (!gem_truthy(_t3290)) {
        _t3292 = _t3290;
    } else {
        GemVal _t3291[] = {gem_v__match_59, gem_string("name")};
        _t3292 = gem_has_key_fn(NULL, _t3291, 2);
    }
    GemVal _t3294;
    if (!gem_truthy(_t3292)) {
        _t3294 = _t3292;
    } else {
        GemVal _t3293[] = {gem_v__match_59, gem_string("value")};
        _t3294 = gem_has_key_fn(NULL, _t3293, 2);
    }
    if (gem_truthy(_t3294)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1128 "compiler/main.gem"
    GemVal _t3295[] = {gem_v_value};
    GemVal _t3296 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3296.fn(_t3296.env, _t3295, 1);
#line 1129 "compiler/main.gem"
    GemVal _t3297 = (*gem_v_tmp);
        GemVal gem_v_t = _t3297.fn(_t3297.env, NULL, 0);
#line 1130 "compiler/main.gem"
    GemVal _t3298[] = {gem_v_name};
    GemVal _t3299 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3299.fn(_t3299.env, _t3298, 1);
#line 1131 "compiler/main.gem"
    GemVal _t3300[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3302;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3300, 2))) {
        _t3302 = gem_fn__mod_codegen_set_contains(NULL, _t3300, 2);
    } else {
        GemVal _t3301[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3302 = gem_fn__mod_codegen_set_contains(NULL, _t3301, 2);
    }
        if (gem_truthy(_t3302)) {
#line 1132 "compiler/main.gem"
    GemVal _t3303 = gem_v_r;
    GemVal _t3304 = gem_v_r;
    GemVal _t3305[] = {gem_v_line_dir, gem_table_get(_t3303, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t3304, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_v_t, gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3306 = gem_interp(20, _t3305);
            gem_pop_frame();
            return _t3306;
        }
#line 1134 "compiler/main.gem"
    GemVal _t3307 = gem_v_r;
    GemVal _t3308 = gem_v_r;
    GemVal _t3309[] = {gem_v_line_dir, gem_table_get(_t3307, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t3308, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_v_mname, gem_string(" = "), gem_v_t, gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
        GemVal _t3310 = gem_interp(19, _t3309);
        gem_pop_frame();
        return _t3310;
    } else {
    GemVal _t3311[] = {gem_v__match_59};
    GemVal _t3313;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3311, 1), gem_string("table")))) {
        _t3313 = gem_eq(gem_type_fn(NULL, _t3311, 1), gem_string("table"));
    } else {
        GemVal _t3312[] = {gem_v__match_59, gem_string("tag")};
        _t3313 = gem_has_key_fn(NULL, _t3312, 2);
    }
    GemVal _t3314;
    if (!gem_truthy(_t3313)) {
        _t3314 = _t3313;
    } else {
        _t3314 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3316;
    if (!gem_truthy(_t3314)) {
        _t3316 = _t3314;
    } else {
        GemVal _t3315[] = {gem_v__match_59, gem_string("value")};
        _t3316 = gem_has_key_fn(NULL, _t3315, 2);
    }
    if (gem_truthy(_t3316)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1136 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1137 "compiler/main.gem"
    GemVal _t3317[] = {gem_v_value};
    GemVal _t3318 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3318.fn(_t3318.env, _t3317, 1);
#line 1138 "compiler/main.gem"
    GemVal _t3319 = (*gem_v_tmp);
            GemVal gem_v_t = _t3319.fn(_t3319.env, NULL, 0);
#line 1139 "compiler/main.gem"
    GemVal _t3320 = gem_v_r;
    GemVal _t3321 = gem_v_r;
    GemVal _t3322[] = {gem_v_line_dir, gem_table_get(_t3320, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t3321, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3323 = gem_interp(14, _t3322);
            gem_pop_frame();
            return _t3323;
        }
#line 1141 "compiler/main.gem"
    GemVal _t3324[] = {gem_v_line_dir, gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3325 = gem_interp(5, _t3324);
        gem_pop_frame();
        return _t3325;
    } else {
    GemVal _t3326[] = {gem_v__match_59};
    GemVal _t3328;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3326, 1), gem_string("table")))) {
        _t3328 = gem_eq(gem_type_fn(NULL, _t3326, 1), gem_string("table"));
    } else {
        GemVal _t3327[] = {gem_v__match_59, gem_string("tag")};
        _t3328 = gem_has_key_fn(NULL, _t3327, 2);
    }
    GemVal _t3329;
    if (!gem_truthy(_t3328)) {
        _t3329 = _t3328;
    } else {
        _t3329 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3329)) {
#line 1143 "compiler/main.gem"
    GemVal _t3330[] = {gem_v_line_dir};
    GemVal _t3331[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3332 = (*gem_v_compile_if);
        GemVal _t3333 = gem_add(gem_interp(1, _t3330), _t3332.fn(_t3332.env, _t3331, 3));
        gem_pop_frame();
        return _t3333;
    } else {
    GemVal _t3334[] = {gem_v__match_59};
    GemVal _t3336;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3334, 1), gem_string("table")))) {
        _t3336 = gem_eq(gem_type_fn(NULL, _t3334, 1), gem_string("table"));
    } else {
        GemVal _t3335[] = {gem_v__match_59, gem_string("tag")};
        _t3336 = gem_has_key_fn(NULL, _t3335, 2);
    }
    GemVal _t3337;
    if (!gem_truthy(_t3336)) {
        _t3337 = _t3336;
    } else {
        _t3337 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3337)) {
#line 1145 "compiler/main.gem"
    GemVal _t3338[] = {gem_v_node, gem_v_indent};
    GemVal _t3339 = (*gem_v_compile_while);
    GemVal _t3340[] = {gem_v_line_dir, _t3339.fn(_t3339.env, _t3338, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3341 = gem_interp(7, _t3340);
        gem_pop_frame();
        return _t3341;
    } else {
    GemVal _t3342[] = {gem_v__match_59};
    GemVal _t3344;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3342, 1), gem_string("table")))) {
        _t3344 = gem_eq(gem_type_fn(NULL, _t3342, 1), gem_string("table"));
    } else {
        GemVal _t3343[] = {gem_v__match_59, gem_string("tag")};
        _t3344 = gem_has_key_fn(NULL, _t3343, 2);
    }
    GemVal _t3345;
    if (!gem_truthy(_t3344)) {
        _t3345 = _t3344;
    } else {
        _t3345 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3345)) {
#line 1147 "compiler/main.gem"
    GemVal _t3346[] = {gem_v_line_dir};
    GemVal _t3347[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3348 = (*gem_v_compile_match);
        GemVal _t3349 = gem_add(gem_interp(1, _t3346), _t3348.fn(_t3348.env, _t3347, 3));
        gem_pop_frame();
        return _t3349;
    } else {
    GemVal _t3350[] = {gem_v__match_59};
    GemVal _t3352;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3350, 1), gem_string("table")))) {
        _t3352 = gem_eq(gem_type_fn(NULL, _t3350, 1), gem_string("table"));
    } else {
        GemVal _t3351[] = {gem_v__match_59, gem_string("tag")};
        _t3352 = gem_has_key_fn(NULL, _t3351, 2);
    }
    GemVal _t3353;
    if (!gem_truthy(_t3352)) {
        _t3353 = _t3352;
    } else {
        _t3353 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3353)) {
#line 1149 "compiler/main.gem"
    GemVal _t3354[] = {gem_v_line_dir};
    GemVal _t3355[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3356 = (*gem_v_compile_receive_match);
        GemVal _t3357 = gem_add(gem_interp(1, _t3354), _t3356.fn(_t3356.env, _t3355, 3));
        gem_pop_frame();
        return _t3357;
    } else {
    GemVal _t3358[] = {gem_v__match_59};
    GemVal _t3360;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3358, 1), gem_string("table")))) {
        _t3360 = gem_eq(gem_type_fn(NULL, _t3358, 1), gem_string("table"));
    } else {
        GemVal _t3359[] = {gem_v__match_59, gem_string("tag")};
        _t3360 = gem_has_key_fn(NULL, _t3359, 2);
    }
    GemVal _t3361;
    if (!gem_truthy(_t3360)) {
        _t3361 = _t3360;
    } else {
        _t3361 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3363;
    if (!gem_truthy(_t3361)) {
        _t3363 = _t3361;
    } else {
        GemVal _t3362[] = {gem_v__match_59, gem_string("stmts")};
        _t3363 = gem_has_key_fn(NULL, _t3362, 2);
    }
    if (gem_truthy(_t3363)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_59, gem_string("stmts"));
#line 1151 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1152 "compiler/main.gem"
    GemVal _t3364[] = {gem_v_p, gem_string("{\n")};
    GemVal _t3365[] = {gem_v_b, gem_interp(2, _t3364)};
        (void)(gem_buf_push_fn(NULL, _t3365, 2));
#line 1153 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1153 "compiler/main.gem"
    GemVal _t3366[] = {gem_v_stmts};
        GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3366, 1), gem_int(1));
#line 1153 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1153 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1153 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1154 "compiler/main.gem"
    GemVal _t3367[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3368 = (*gem_v_compile_stmt);
    GemVal _t3369[] = {gem_v_b, _t3368.fn(_t3368.env, _t3367, 2)};
            (void)(gem_buf_push_fn(NULL, _t3369, 2));
#line 1155 "compiler/main.gem"
    GemVal _t3370[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3370, 2));
        }

#line 1157 "compiler/main.gem"
    GemVal _t3371[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3371, 1), gem_int(0)))) {
#line 1158 "compiler/main.gem"
    GemVal _t3372[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3373 = (*gem_v_compile_stmt_return);
    GemVal _t3374[] = {gem_v_b, _t3373.fn(_t3373.env, _t3372, 2)};
            (void)(gem_buf_push_fn(NULL, _t3374, 2));
#line 1159 "compiler/main.gem"
    GemVal _t3375[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3375, 2));
        }
#line 1161 "compiler/main.gem"
    GemVal _t3376[] = {gem_v_p, gem_string("}")};
    GemVal _t3377[] = {gem_v_b, gem_interp(2, _t3376)};
        (void)(gem_buf_push_fn(NULL, _t3377, 2));
#line 1162 "compiler/main.gem"
    GemVal _t3378[] = {gem_v_b};
        GemVal _t3379 = gem_buf_str_fn(NULL, _t3378, 1);
        gem_pop_frame();
        return _t3379;
    } else {
    GemVal _t3380[] = {gem_v__match_59};
    GemVal _t3382;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3380, 1), gem_string("table")))) {
        _t3382 = gem_eq(gem_type_fn(NULL, _t3380, 1), gem_string("table"));
    } else {
        GemVal _t3381[] = {gem_v__match_59, gem_string("tag")};
        _t3382 = gem_has_key_fn(NULL, _t3381, 2);
    }
    GemVal _t3383;
    if (!gem_truthy(_t3382)) {
        _t3383 = _t3382;
    } else {
        _t3383 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3383)) {
#line 1164 "compiler/main.gem"
    GemVal _t3384[] = {gem_v_node, gem_v_indent};
    GemVal _t3385 = (*gem_v_compile_stmt);
    GemVal _t3386[] = {_t3385.fn(_t3385.env, _t3384, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3387 = gem_interp(6, _t3386);
        gem_pop_frame();
        return _t3387;
    } else {
    GemVal _t3388[] = {gem_v__match_59};
    GemVal _t3390;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3388, 1), gem_string("table")))) {
        _t3390 = gem_eq(gem_type_fn(NULL, _t3388, 1), gem_string("table"));
    } else {
        GemVal _t3389[] = {gem_v__match_59, gem_string("tag")};
        _t3390 = gem_has_key_fn(NULL, _t3389, 2);
    }
    GemVal _t3391;
    if (!gem_truthy(_t3390)) {
        _t3391 = _t3390;
    } else {
        _t3391 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3391)) {
#line 1166 "compiler/main.gem"
    GemVal _t3392[] = {gem_v_node, gem_v_indent};
    GemVal _t3393 = (*gem_v_compile_stmt);
    GemVal _t3394[] = {_t3393.fn(_t3393.env, _t3392, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3395 = gem_interp(6, _t3394);
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
#line 1168 "compiler/main.gem"
    GemVal _t3400[] = {gem_v_node, gem_v_indent};
    GemVal _t3401 = (*gem_v_compile_stmt);
    GemVal _t3402[] = {_t3401.fn(_t3401.env, _t3400, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3403 = gem_interp(6, _t3402);
        gem_pop_frame();
        return _t3403;
    } else {
    GemVal _t3404[] = {gem_v__match_59};
    GemVal _t3406;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3404, 1), gem_string("table")))) {
        _t3406 = gem_eq(gem_type_fn(NULL, _t3404, 1), gem_string("table"));
    } else {
        GemVal _t3405[] = {gem_v__match_59, gem_string("tag")};
        _t3406 = gem_has_key_fn(NULL, _t3405, 2);
    }
    GemVal _t3407;
    if (!gem_truthy(_t3406)) {
        _t3407 = _t3406;
    } else {
        _t3407 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3407)) {
#line 1170 "compiler/main.gem"
    GemVal _t3408[] = {gem_v_node, gem_v_indent};
    GemVal _t3409 = (*gem_v_compile_stmt);
    GemVal _t3410[] = {_t3409.fn(_t3409.env, _t3408, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3411 = gem_interp(6, _t3410);
        gem_pop_frame();
        return _t3411;
    } else {
    GemVal _t3412[] = {gem_v__match_59};
    GemVal _t3414;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3412, 1), gem_string("table")))) {
        _t3414 = gem_eq(gem_type_fn(NULL, _t3412, 1), gem_string("table"));
    } else {
        GemVal _t3413[] = {gem_v__match_59, gem_string("tag")};
        _t3414 = gem_has_key_fn(NULL, _t3413, 2);
    }
    GemVal _t3415;
    if (!gem_truthy(_t3414)) {
        _t3415 = _t3414;
    } else {
        _t3415 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3415)) {
#line 1172 "compiler/main.gem"
    GemVal _t3416[] = {gem_v_node, gem_v_indent};
    GemVal _t3417 = (*gem_v_compile_stmt);
    GemVal _t3418[] = {_t3417.fn(_t3417.env, _t3416, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3419 = gem_interp(6, _t3418);
        gem_pop_frame();
        return _t3419;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1175 "compiler/main.gem"
    GemVal _t3420[] = {gem_v_node};
    GemVal _t3421 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3421.fn(_t3421.env, _t3420, 1);
#line 1176 "compiler/main.gem"
    GemVal _t3422 = (*gem_v_tmp);
    GemVal gem_v_t = _t3422.fn(_t3422.env, NULL, 0);
    GemVal _t3423 = gem_v_r;
    GemVal _t3424 = gem_v_r;
    GemVal _t3425[] = {gem_v_line_dir, gem_table_get(_t3423, gem_string("setup")), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t3424, gem_string("expr")), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
    GemVal _t3426 = gem_interp(14, _t3425);
    gem_pop_frame();
    return _t3426;
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
#line 1183 "compiler/main.gem"
    GemVal _t3428[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3428, 1);
#line 1184 "compiler/main.gem"
    GemVal _t3429 = gem_v_node;
    GemVal _t3430[] = {gem_table_get(_t3429, gem_string("cond"))};
    GemVal _t3431 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3431.fn(_t3431.env, _t3430, 1);
#line 1185 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1186 "compiler/main.gem"
    GemVal _t3432 = gem_v_cond_r;
    GemVal _t3433[] = {gem_v_b, gem_table_get(_t3432, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3433, 2));
#line 1187 "compiler/main.gem"
    GemVal _t3434 = gem_v_cond_r;
    GemVal _t3435[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get(_t3434, gem_string("expr")), gem_string(")) {\n")};
    GemVal _t3436[] = {gem_v_b, gem_interp(4, _t3435)};
    (void)(gem_buf_push_fn(NULL, _t3436, 2));
#line 1188 "compiler/main.gem"
    GemVal _t3437 = gem_v_node;
    GemVal _t3438[] = {gem_table_get(_t3437, gem_string("then")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3439 = (*gem_v_compile_stmts_tail);
    GemVal _t3440[] = {gem_v_b, _t3439.fn(_t3439.env, _t3438, 3)};
    (void)(gem_buf_push_fn(NULL, _t3440, 2));
#line 1189 "compiler/main.gem"
    GemVal _t3441 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3441, gem_string("else")), GEM_NIL))) {
#line 1190 "compiler/main.gem"
    GemVal _t3442[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3443[] = {gem_v_b, gem_interp(2, _t3442)};
        (void)(gem_buf_push_fn(NULL, _t3443, 2));
#line 1191 "compiler/main.gem"
    GemVal _t3444 = gem_v_node;
    GemVal _t3445[] = {gem_table_get(_t3444, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3446 = (*gem_v_compile_stmts_tail);
    GemVal _t3447[] = {gem_v_b, _t3446.fn(_t3446.env, _t3445, 3)};
        (void)(gem_buf_push_fn(NULL, _t3447, 2));
    } else {
#line 1192 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1193 "compiler/main.gem"
    GemVal _t3448[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3449[] = {gem_v_b, gem_interp(2, _t3448)};
            (void)(gem_buf_push_fn(NULL, _t3449, 2));
#line 1194 "compiler/main.gem"
    GemVal _t3450 = gem_table_new();
    GemVal _t3451[] = {_t3450, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3452 = (*gem_v_compile_stmts_tail);
    GemVal _t3453[] = {gem_v_b, _t3452.fn(_t3452.env, _t3451, 3)};
            (void)(gem_buf_push_fn(NULL, _t3453, 2));
        }
    }
#line 1196 "compiler/main.gem"
    GemVal _t3454[] = {gem_v_p, gem_string("}")};
    GemVal _t3455[] = {gem_v_b, gem_interp(2, _t3454)};
    (void)(gem_buf_push_fn(NULL, _t3455, 2));
#line 1197 "compiler/main.gem"
    GemVal _t3456[] = {gem_v_b};
    GemVal _t3457 = gem_buf_str_fn(NULL, _t3456, 1);
    gem_pop_frame();
    return _t3457;
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
#line 1203 "compiler/main.gem"
    GemVal _t3459[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3459, 1);
#line 1204 "compiler/main.gem"
    GemVal _t3460 = gem_v_node;
    GemVal _t3461[] = {gem_table_get(_t3460, gem_string("cond"))};
    GemVal _t3462 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3462.fn(_t3462.env, _t3461, 1);
#line 1205 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1206 "compiler/main.gem"
    GemVal _t3463[] = {gem_v_p, gem_string("while (1) {\n")};
    GemVal _t3464[] = {gem_v_b, gem_interp(2, _t3463)};
    (void)(gem_buf_push_fn(NULL, _t3464, 2));
#line 1207 "compiler/main.gem"
    GemVal _t3465[] = {gem_v_p, gem_string("    gem_yield_check();\n")};
    GemVal _t3466[] = {gem_v_b, gem_interp(2, _t3465)};
    (void)(gem_buf_push_fn(NULL, _t3466, 2));
#line 1208 "compiler/main.gem"
    GemVal _t3467 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3467, gem_string("setup")), gem_string("")))) {
#line 1209 "compiler/main.gem"
    GemVal _t3468 = gem_v_cond_r;
    GemVal _t3469[] = {gem_v_p, gem_string("    ")};
    GemVal _t3470[] = {gem_table_get(_t3468, gem_string("setup")), gem_string("    "), gem_interp(2, _t3469)};
    GemVal _t3471[] = {gem_v_b, gem_str_replace_fn(NULL, _t3470, 3)};
        (void)(gem_buf_push_fn(NULL, _t3471, 2));
    }
#line 1211 "compiler/main.gem"
    GemVal _t3472 = gem_v_cond_r;
    GemVal _t3473[] = {gem_v_p, gem_string("    if (!gem_truthy("), gem_table_get(_t3472, gem_string("expr")), gem_string(")) break;\n")};
    GemVal _t3474[] = {gem_v_b, gem_interp(4, _t3473)};
    (void)(gem_buf_push_fn(NULL, _t3474, 2));
#line 1212 "compiler/main.gem"
    GemVal _t3475 = gem_v_node;
    GemVal _t3476[] = {gem_table_get(_t3475, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3477 = (*gem_v_compile_stmts);
    GemVal _t3478[] = {gem_v_b, _t3477.fn(_t3477.env, _t3476, 2)};
    (void)(gem_buf_push_fn(NULL, _t3478, 2));
#line 1213 "compiler/main.gem"
    GemVal _t3479[] = {gem_v_p, gem_string("}")};
    GemVal _t3480[] = {gem_v_b, gem_interp(2, _t3479)};
    (void)(gem_buf_push_fn(NULL, _t3480, 2));
#line 1214 "compiler/main.gem"
    GemVal _t3481[] = {gem_v_b};
    GemVal _t3482 = gem_buf_str_fn(NULL, _t3481, 1);
    gem_pop_frame();
    return _t3482;
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
#line 1220 "compiler/main.gem"
    GemVal _t3484[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3484, 1);
#line 1221 "compiler/main.gem"
    GemVal _t3485 = gem_v_node;
    GemVal _t3486[] = {gem_table_get(_t3485, gem_string("target"))};
    GemVal _t3487 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3487.fn(_t3487.env, _t3486, 1);
#line 1222 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1223 "compiler/main.gem"
    GemVal _t3488 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3488, gem_string("target_var")), GEM_NIL))) {
#line 1224 "compiler/main.gem"
    GemVal _t3489 = gem_v_node;
    GemVal _t3490[] = {gem_table_get(_t3489, gem_string("target_var"))};
    GemVal _t3491 = (*gem_v_mangle);
        gem_v_t = _t3491.fn(_t3491.env, _t3490, 1);
    } else {
#line 1226 "compiler/main.gem"
    GemVal _t3492 = (*gem_v_tmp);
        gem_v_t = _t3492.fn(_t3492.env, NULL, 0);
    }
#line 1228 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1229 "compiler/main.gem"
    GemVal _t3493 = gem_v_target_r;
    GemVal _t3494[] = {gem_v_b, gem_table_get(_t3493, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3494, 2));
#line 1230 "compiler/main.gem"
    GemVal _t3495 = gem_v_target_r;
    GemVal _t3496[] = {gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get(_t3495, gem_string("expr")), gem_string(";\n")};
    GemVal _t3497[] = {gem_v_b, gem_interp(6, _t3496)};
    (void)(gem_buf_push_fn(NULL, _t3497, 2));
#line 1231 "compiler/main.gem"
    GemVal _t3498 = gem_v_node;
    GemVal _t3499[] = {gem_table_get(_t3498, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3499, 1), gem_int(0)))) {
#line 1232 "compiler/main.gem"
    GemVal _t3500 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3500, gem_string("else")), GEM_NIL))) {
#line 1233 "compiler/main.gem"
    GemVal _t3501 = gem_v_node;
    GemVal _t3502[] = {gem_table_get(_t3501, gem_string("else")), gem_v_indent, gem_v_return_pos};
    GemVal _t3503 = (*gem_v_compile_stmts_tail);
    GemVal _t3504[] = {gem_v_b, _t3503.fn(_t3503.env, _t3502, 3)};
            (void)(gem_buf_push_fn(NULL, _t3504, 2));
        } else {
#line 1234 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1235 "compiler/main.gem"
    GemVal _t3505[] = {gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;\n")};
    GemVal _t3506[] = {gem_v_b, gem_interp(4, _t3505)};
                (void)(gem_buf_push_fn(NULL, _t3506, 2));
            }
        }
#line 1237 "compiler/main.gem"
    GemVal _t3507[] = {gem_v_p, gem_string("(void)"), gem_v_t, gem_string(";")};
    GemVal _t3508[] = {gem_v_b, gem_interp(4, _t3507)};
        (void)(gem_buf_push_fn(NULL, _t3508, 2));
#line 1238 "compiler/main.gem"
    GemVal _t3509[] = {gem_v_b};
        GemVal _t3510 = gem_buf_str_fn(NULL, _t3509, 1);
        gem_pop_frame();
        return _t3510;
    }
#line 1240 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1240 "compiler/main.gem"
    GemVal _t3511 = gem_v_node;
    GemVal _t3512[] = {gem_table_get(_t3511, gem_string("whens"))};
    GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t3512, 1);
#line 1240 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1240 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_62;
#line 1240 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1241 "compiler/main.gem"
    GemVal _t3513 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3513, gem_string("whens")), gem_v_i);
#line 1242 "compiler/main.gem"
    GemVal _t3514 = gem_v_w;
    GemVal _t3515[] = {gem_table_get(_t3514, gem_string("value"))};
    GemVal _t3516 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3516.fn(_t3516.env, _t3515, 1);
#line 1243 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1244 "compiler/main.gem"
    GemVal _t3517 = gem_v_val_r;
    GemVal _t3518[] = {gem_v_b, gem_table_get(_t3517, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3518, 2));
#line 1245 "compiler/main.gem"
    GemVal _t3519 = gem_v_val_r;
    GemVal _t3520[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get(_t3519, gem_string("expr")), gem_string(")) {\n")};
    GemVal _t3521[] = {gem_v_b, gem_interp(4, _t3520)};
            (void)(gem_buf_push_fn(NULL, _t3521, 2));
        } else {
#line 1247 "compiler/main.gem"
    GemVal _t3522[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3523[] = {gem_v_b, gem_interp(2, _t3522)};
            (void)(gem_buf_push_fn(NULL, _t3523, 2));
#line 1248 "compiler/main.gem"
    GemVal _t3524 = gem_v_val_r;
    GemVal _t3525[] = {gem_v_b, gem_table_get(_t3524, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3525, 2));
#line 1249 "compiler/main.gem"
    GemVal _t3526 = gem_v_val_r;
    GemVal _t3527[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get(_t3526, gem_string("expr")), gem_string(")) {\n")};
    GemVal _t3528[] = {gem_v_b, gem_interp(4, _t3527)};
            (void)(gem_buf_push_fn(NULL, _t3528, 2));
        }
#line 1251 "compiler/main.gem"
    GemVal _t3529 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3529, gem_string("bindings")), GEM_NIL))) {
#line 1252 "compiler/main.gem"
    GemVal _t3530 = gem_v_w;
            GemVal gem_v__for_items_61 = gem_table_get(_t3530, gem_string("bindings"));
#line 1252 "compiler/main.gem"
            GemVal gem_v__for_i_61 = gem_int(0);
#line 1252 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3531[] = {gem_v__for_items_61};
                if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3531, 1)))) break;
#line 1252 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1252 "compiler/main.gem"
                gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1253 "compiler/main.gem"
    GemVal _t3532[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3533 = (*gem_v_compile_stmt);
    GemVal _t3534[] = {gem_v_b, _t3533.fn(_t3533.env, _t3532, 2)};
                (void)(gem_buf_push_fn(NULL, _t3534, 2));
#line 1254 "compiler/main.gem"
    GemVal _t3535[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3535, 2));
            }

        }
#line 1257 "compiler/main.gem"
    GemVal _t3536 = gem_v_w;
    GemVal _t3537[] = {gem_table_get(_t3536, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3538 = (*gem_v_compile_stmts_tail);
    GemVal _t3539[] = {gem_v_b, _t3538.fn(_t3538.env, _t3537, 3)};
        (void)(gem_buf_push_fn(NULL, _t3539, 2));
    }

#line 1259 "compiler/main.gem"
    GemVal _t3540 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3540, gem_string("else")), GEM_NIL))) {
#line 1260 "compiler/main.gem"
    GemVal _t3541[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3542[] = {gem_v_b, gem_interp(2, _t3541)};
        (void)(gem_buf_push_fn(NULL, _t3542, 2));
#line 1261 "compiler/main.gem"
    GemVal _t3543 = gem_v_node;
    GemVal _t3544[] = {gem_table_get(_t3543, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3545 = (*gem_v_compile_stmts_tail);
    GemVal _t3546[] = {gem_v_b, _t3545.fn(_t3545.env, _t3544, 3)};
        (void)(gem_buf_push_fn(NULL, _t3546, 2));
    } else {
#line 1262 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1263 "compiler/main.gem"
    GemVal _t3547[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3548[] = {gem_v_b, gem_interp(2, _t3547)};
            (void)(gem_buf_push_fn(NULL, _t3548, 2));
#line 1264 "compiler/main.gem"
    GemVal _t3549 = gem_table_new();
    GemVal _t3550[] = {_t3549, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3551 = (*gem_v_compile_stmts_tail);
    GemVal _t3552[] = {gem_v_b, _t3551.fn(_t3551.env, _t3550, 3)};
            (void)(gem_buf_push_fn(NULL, _t3552, 2));
        }
    }
#line 1266 "compiler/main.gem"
    GemVal _t3553[] = {gem_v_p, gem_string("}")};
    GemVal _t3554[] = {gem_v_b, gem_interp(2, _t3553)};
    (void)(gem_buf_push_fn(NULL, _t3554, 2));
#line 1267 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(1);
#line 1267 "compiler/main.gem"
    GemVal _t3555 = gem_v_node;
    GemVal _t3556[] = {gem_table_get(_t3555, gem_string("whens"))};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3556, 1);
#line 1267 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1267 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1267 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1268 "compiler/main.gem"
    GemVal _t3557[] = {gem_string("\n"), gem_v_p, gem_string("}")};
    GemVal _t3558[] = {gem_v_b, gem_interp(3, _t3557)};
        (void)(gem_buf_push_fn(NULL, _t3558, 2));
    }

#line 1270 "compiler/main.gem"
    GemVal _t3559[] = {gem_v_b};
    GemVal _t3560 = gem_buf_str_fn(NULL, _t3559, 1);
    gem_pop_frame();
    return _t3560;
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
#line 1276 "compiler/main.gem"
    GemVal _t3562[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3562, 1);
#line 1277 "compiler/main.gem"
    GemVal _t3563 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3563, gem_string("arms"));
#line 1278 "compiler/main.gem"
    GemVal _t3564 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3564, gem_string("after_ms"));
#line 1279 "compiler/main.gem"
    GemVal _t3565 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3565, gem_string("after_body"));
#line 1281 "compiler/main.gem"
    GemVal _t3566 = gem_v_node;
    GemVal _t3567[] = {gem_table_get(_t3566, gem_string("recv_var"))};
    GemVal _t3568 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3568.fn(_t3568.env, _t3567, 1);
#line 1282 "compiler/main.gem"
    GemVal _t3569 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3569.fn(_t3569.env, NULL, 0);
#line 1283 "compiler/main.gem"
    GemVal _t3570 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3570.fn(_t3570.env, NULL, 0);
#line 1284 "compiler/main.gem"
    GemVal _t3571 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3571.fn(_t3571.env, NULL, 0);
#line 1285 "compiler/main.gem"
    GemVal _t3572 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3572.fn(_t3572.env, NULL, 0);
#line 1286 "compiler/main.gem"
    GemVal _t3573 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3573.fn(_t3573.env, NULL, 0);
#line 1288 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1290 "compiler/main.gem"
    GemVal _t3574[] = {gem_v_p, gem_string("GemVal "), gem_v_recv_tmp, gem_string(" = GEM_NIL;\n")};
    GemVal _t3575[] = {gem_v_b, gem_interp(4, _t3574)};
    (void)(gem_buf_push_fn(NULL, _t3575, 2));
#line 1291 "compiler/main.gem"
    GemVal _t3576[] = {gem_v_p, gem_string("int "), gem_v_matched_tmp, gem_string(" = -1;\n")};
    GemVal _t3577[] = {gem_v_b, gem_interp(4, _t3576)};
    (void)(gem_buf_push_fn(NULL, _t3577, 2));
#line 1293 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1294 "compiler/main.gem"
    GemVal _t3578[] = {gem_v_after_ms};
    GemVal _t3579 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3579.fn(_t3579.env, _t3578, 1);
#line 1295 "compiler/main.gem"
    GemVal _t3580 = gem_v_ms_r;
    GemVal _t3581[] = {gem_v_b, gem_table_get(_t3580, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t3581, 2));
#line 1296 "compiler/main.gem"
    GemVal _t3582 = gem_v_ms_r;
    GemVal _t3583[] = {gem_v_p, gem_string("int64_t "), gem_v_deadline_tmp, gem_string(" = gem_now_ms() + (int64_t)("), gem_table_get(_t3582, gem_string("expr")), gem_string(").ival;\n")};
    GemVal _t3584[] = {gem_v_b, gem_interp(6, _t3583)};
        (void)(gem_buf_push_fn(NULL, _t3584, 2));
    }
#line 1299 "compiler/main.gem"
    GemVal _t3585[] = {gem_v_p, gem_string("while (1) {\n")};
    GemVal _t3586[] = {gem_v_b, gem_interp(2, _t3585)};
    (void)(gem_buf_push_fn(NULL, _t3586, 2));
#line 1300 "compiler/main.gem"
    GemVal _t3587[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn__mod_codegen_pad_for(NULL, _t3587, 1);
#line 1302 "compiler/main.gem"
    GemVal _t3588[] = {gem_v_p2, gem_string("GemProcess *"), gem_v_proc_tmp, gem_string(" = &gem_proc_table[gem_current_pid];\n")};
    GemVal _t3589[] = {gem_v_b, gem_interp(4, _t3588)};
    (void)(gem_buf_push_fn(NULL, _t3589, 2));
#line 1303 "compiler/main.gem"
    GemVal _t3590[] = {gem_v_p2, gem_string("GemMsgNode *"), gem_v_prev_tmp, gem_string(" = NULL;\n")};
    GemVal _t3591[] = {gem_v_b, gem_interp(4, _t3590)};
    (void)(gem_buf_push_fn(NULL, _t3591, 2));
#line 1304 "compiler/main.gem"
    GemVal _t3592[] = {gem_v_p2, gem_string("GemMsgNode *"), gem_v_cur_tmp, gem_string(" = "), gem_v_proc_tmp, gem_string("->mailbox.head;\n")};
    GemVal _t3593[] = {gem_v_b, gem_interp(6, _t3592)};
    (void)(gem_buf_push_fn(NULL, _t3593, 2));
#line 1305 "compiler/main.gem"
    GemVal _t3594[] = {gem_v_p2, gem_v_matched_tmp, gem_string(" = -1;\n")};
    GemVal _t3595[] = {gem_v_b, gem_interp(3, _t3594)};
    (void)(gem_buf_push_fn(NULL, _t3595, 2));
#line 1306 "compiler/main.gem"
    GemVal _t3596[] = {gem_v_p2, gem_string("while ("), gem_v_cur_tmp, gem_string(") {\n")};
    GemVal _t3597[] = {gem_v_b, gem_interp(4, _t3596)};
    (void)(gem_buf_push_fn(NULL, _t3597, 2));
#line 1307 "compiler/main.gem"
    GemVal _t3598[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn__mod_codegen_pad_for(NULL, _t3598, 1);
#line 1308 "compiler/main.gem"
    GemVal _t3599[] = {gem_v_p3, gem_v_recv_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string("->value;\n")};
    GemVal _t3600[] = {gem_v_b, gem_interp(5, _t3599)};
    (void)(gem_buf_push_fn(NULL, _t3600, 2));
#line 1310 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1310 "compiler/main.gem"
    GemVal _t3601[] = {gem_v_arms};
    GemVal gem_v__for_limit_64 = gem_len_fn(NULL, _t3601, 1);
#line 1310 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1310 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_64;
#line 1310 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1311 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1312 "compiler/main.gem"
    GemVal _t3602 = gem_v_arm;
    GemVal _t3603 = gem_table_get(_t3602, gem_string("pattern"));
    GemVal _t3604[] = {gem_table_get(_t3603, gem_string("condition"))};
    GemVal _t3605 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3605.fn(_t3605.env, _t3604, 1);
#line 1313 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1314 "compiler/main.gem"
    GemVal _t3606 = gem_v_cond_r;
    GemVal _t3607[] = {gem_v_b, gem_table_get(_t3606, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3607, 2));
#line 1315 "compiler/main.gem"
    GemVal _t3608 = gem_v_cond_r;
    GemVal _t3609[] = {gem_v_p3, gem_string("if (gem_truthy("), gem_table_get(_t3608, gem_string("expr")), gem_string(")) {\n")};
    GemVal _t3610[] = {gem_v_b, gem_interp(4, _t3609)};
            (void)(gem_buf_push_fn(NULL, _t3610, 2));
        } else {
#line 1317 "compiler/main.gem"
    GemVal _t3611[] = {gem_v_p3, gem_string("} else {\n")};
    GemVal _t3612[] = {gem_v_b, gem_interp(2, _t3611)};
            (void)(gem_buf_push_fn(NULL, _t3612, 2));
#line 1318 "compiler/main.gem"
    GemVal _t3613 = gem_v_cond_r;
    GemVal _t3614[] = {gem_v_b, gem_table_get(_t3613, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3614, 2));
#line 1319 "compiler/main.gem"
    GemVal _t3615 = gem_v_cond_r;
    GemVal _t3616[] = {gem_v_p3, gem_string("if (gem_truthy("), gem_table_get(_t3615, gem_string("expr")), gem_string(")) {\n")};
    GemVal _t3617[] = {gem_v_b, gem_interp(4, _t3616)};
            (void)(gem_buf_push_fn(NULL, _t3617, 2));
        }
#line 1321 "compiler/main.gem"
    GemVal _t3618[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3619[] = {gem_fn__mod_codegen_pad_for(NULL, _t3618, 1), gem_v_matched_tmp, gem_string(" = "), gem_v_i, gem_string(";\n")};
    GemVal _t3620[] = {gem_v_b, gem_interp(5, _t3619)};
        (void)(gem_buf_push_fn(NULL, _t3620, 2));
    }

#line 1323 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1323 "compiler/main.gem"
    GemVal _t3621[] = {gem_v_arms};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3621, 1);
#line 1323 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1323 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1323 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1324 "compiler/main.gem"
    GemVal _t3622[] = {gem_v_p3, gem_string("}")};
    GemVal _t3623[] = {gem_v_b, gem_interp(2, _t3622)};
        (void)(gem_buf_push_fn(NULL, _t3623, 2));
#line 1325 "compiler/main.gem"
    GemVal _t3624[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3624, 1), gem_int(1))))) {
#line 1326 "compiler/main.gem"
    GemVal _t3625[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3625, 2));
        }
    }

#line 1329 "compiler/main.gem"
    GemVal _t3626[] = {gem_v_b, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t3626, 2));
#line 1331 "compiler/main.gem"
    GemVal _t3627[] = {gem_v_p3, gem_string("if ("), gem_v_matched_tmp, gem_string(" >= 0) {\n")};
    GemVal _t3628[] = {gem_v_b, gem_interp(4, _t3627)};
    (void)(gem_buf_push_fn(NULL, _t3628, 2));
#line 1332 "compiler/main.gem"
    GemVal _t3629[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn__mod_codegen_pad_for(NULL, _t3629, 1);
#line 1333 "compiler/main.gem"
    GemVal _t3630[] = {gem_v_p4, gem_string("gem_mailbox_remove(&"), gem_v_proc_tmp, gem_string("->mailbox, "), gem_v_prev_tmp, gem_string(", "), gem_v_cur_tmp, gem_string(");\n")};
    GemVal _t3631[] = {gem_v_b, gem_interp(8, _t3630)};
    (void)(gem_buf_push_fn(NULL, _t3631, 2));
#line 1334 "compiler/main.gem"
    GemVal _t3632[] = {gem_v_p4, gem_string("break;\n")};
    GemVal _t3633[] = {gem_v_b, gem_interp(2, _t3632)};
    (void)(gem_buf_push_fn(NULL, _t3633, 2));
#line 1335 "compiler/main.gem"
    GemVal _t3634[] = {gem_v_p3, gem_string("}\n")};
    GemVal _t3635[] = {gem_v_b, gem_interp(2, _t3634)};
    (void)(gem_buf_push_fn(NULL, _t3635, 2));
#line 1337 "compiler/main.gem"
    GemVal _t3636[] = {gem_v_p3, gem_v_prev_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string(";\n")};
    GemVal _t3637[] = {gem_v_b, gem_interp(5, _t3636)};
    (void)(gem_buf_push_fn(NULL, _t3637, 2));
#line 1338 "compiler/main.gem"
    GemVal _t3638[] = {gem_v_p3, gem_v_cur_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string("->next;\n")};
    GemVal _t3639[] = {gem_v_b, gem_interp(5, _t3638)};
    (void)(gem_buf_push_fn(NULL, _t3639, 2));
#line 1339 "compiler/main.gem"
    GemVal _t3640[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3641[] = {gem_v_b, gem_interp(2, _t3640)};
    (void)(gem_buf_push_fn(NULL, _t3641, 2));
#line 1341 "compiler/main.gem"
    GemVal _t3642[] = {gem_v_p2, gem_string("if ("), gem_v_matched_tmp, gem_string(" >= 0) break;\n")};
    GemVal _t3643[] = {gem_v_b, gem_interp(4, _t3642)};
    (void)(gem_buf_push_fn(NULL, _t3643, 2));
#line 1342 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1343 "compiler/main.gem"
    GemVal _t3644[] = {gem_v_p2, gem_string("if (gem_now_ms() >= "), gem_v_deadline_tmp, gem_string(") {\n")};
    GemVal _t3645[] = {gem_v_b, gem_interp(4, _t3644)};
        (void)(gem_buf_push_fn(NULL, _t3645, 2));
#line 1344 "compiler/main.gem"
    GemVal _t3646[] = {gem_v_p2, gem_string("    "), gem_v_matched_tmp, gem_string(" = -2;\n")};
    GemVal _t3647[] = {gem_v_b, gem_interp(4, _t3646)};
        (void)(gem_buf_push_fn(NULL, _t3647, 2));
#line 1345 "compiler/main.gem"
    GemVal _t3648[] = {gem_v_p2, gem_string("    break;\n")};
    GemVal _t3649[] = {gem_v_b, gem_interp(2, _t3648)};
        (void)(gem_buf_push_fn(NULL, _t3649, 2));
#line 1346 "compiler/main.gem"
    GemVal _t3650[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3651[] = {gem_v_b, gem_interp(2, _t3650)};
        (void)(gem_buf_push_fn(NULL, _t3651, 2));
#line 1347 "compiler/main.gem"
    GemVal _t3652[] = {gem_v_p2, gem_string("gem_selective_yield("), gem_v_deadline_tmp, gem_string(");\n")};
    GemVal _t3653[] = {gem_v_b, gem_interp(4, _t3652)};
        (void)(gem_buf_push_fn(NULL, _t3653, 2));
#line 1348 "compiler/main.gem"
    GemVal _t3654[] = {gem_v_p2, gem_string("if ("), gem_v_proc_tmp, gem_string("->timed_out) {\n")};
    GemVal _t3655[] = {gem_v_b, gem_interp(4, _t3654)};
        (void)(gem_buf_push_fn(NULL, _t3655, 2));
#line 1349 "compiler/main.gem"
    GemVal _t3656[] = {gem_v_p2, gem_string("    "), gem_v_matched_tmp, gem_string(" = -2;\n")};
    GemVal _t3657[] = {gem_v_b, gem_interp(4, _t3656)};
        (void)(gem_buf_push_fn(NULL, _t3657, 2));
#line 1350 "compiler/main.gem"
    GemVal _t3658[] = {gem_v_p2, gem_string("    break;\n")};
    GemVal _t3659[] = {gem_v_b, gem_interp(2, _t3658)};
        (void)(gem_buf_push_fn(NULL, _t3659, 2));
#line 1351 "compiler/main.gem"
    GemVal _t3660[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3661[] = {gem_v_b, gem_interp(2, _t3660)};
        (void)(gem_buf_push_fn(NULL, _t3661, 2));
    } else {
#line 1353 "compiler/main.gem"
    GemVal _t3662[] = {gem_v_p2, gem_string("gem_selective_yield(-1);\n")};
    GemVal _t3663[] = {gem_v_b, gem_interp(2, _t3662)};
        (void)(gem_buf_push_fn(NULL, _t3663, 2));
    }
#line 1356 "compiler/main.gem"
    GemVal _t3664[] = {gem_v_p, gem_string("}\n")};
    GemVal _t3665[] = {gem_v_b, gem_interp(2, _t3664)};
    (void)(gem_buf_push_fn(NULL, _t3665, 2));
#line 1359 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1359 "compiler/main.gem"
    GemVal _t3666[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3666, 1);
#line 1359 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1359 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1359 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1360 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1361 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1362 "compiler/main.gem"
    GemVal _t3667[] = {gem_v_p, gem_string("if ("), gem_v_matched_tmp, gem_string(" == "), gem_v_i, gem_string(") {\n")};
    GemVal _t3668[] = {gem_v_b, gem_interp(6, _t3667)};
            (void)(gem_buf_push_fn(NULL, _t3668, 2));
        } else {
#line 1364 "compiler/main.gem"
    GemVal _t3669[] = {gem_v_p, gem_string("} else if ("), gem_v_matched_tmp, gem_string(" == "), gem_v_i, gem_string(") {\n")};
    GemVal _t3670[] = {gem_v_b, gem_interp(6, _t3669)};
            (void)(gem_buf_push_fn(NULL, _t3670, 2));
        }
#line 1366 "compiler/main.gem"
    GemVal _t3671 = gem_v_arm;
    GemVal _t3672 = gem_table_get(_t3671, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3672, gem_string("bindings")), GEM_NIL))) {
#line 1367 "compiler/main.gem"
    GemVal _t3673 = gem_v_arm;
    GemVal _t3674 = gem_table_get(_t3673, gem_string("pattern"));
            GemVal gem_v__for_items_66 = gem_table_get(_t3674, gem_string("bindings"));
#line 1367 "compiler/main.gem"
            GemVal gem_v__for_i_66 = gem_int(0);
#line 1367 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3675[] = {gem_v__for_items_66};
                if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t3675, 1)))) break;
#line 1367 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1367 "compiler/main.gem"
                gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1368 "compiler/main.gem"
    GemVal _t3676[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3677 = (*gem_v_compile_stmt);
    GemVal _t3678[] = {gem_v_b, _t3677.fn(_t3677.env, _t3676, 2)};
                (void)(gem_buf_push_fn(NULL, _t3678, 2));
#line 1369 "compiler/main.gem"
    GemVal _t3679[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3679, 2));
            }

        }
#line 1372 "compiler/main.gem"
    GemVal _t3680 = gem_v_arm;
    GemVal _t3681[] = {gem_table_get(_t3680, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3682 = (*gem_v_compile_stmts_tail);
    GemVal _t3683[] = {gem_v_b, _t3682.fn(_t3682.env, _t3681, 3)};
        (void)(gem_buf_push_fn(NULL, _t3683, 2));
    }

#line 1376 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1377 "compiler/main.gem"
    GemVal _t3684[] = {gem_v_p, gem_string("} else if ("), gem_v_matched_tmp, gem_string(" == -2) {\n")};
    GemVal _t3685[] = {gem_v_b, gem_interp(4, _t3684)};
        (void)(gem_buf_push_fn(NULL, _t3685, 2));
#line 1378 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1379 "compiler/main.gem"
    GemVal _t3686[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3687 = (*gem_v_compile_stmts_tail);
    GemVal _t3688[] = {gem_v_b, _t3687.fn(_t3687.env, _t3686, 3)};
            (void)(gem_buf_push_fn(NULL, _t3688, 2));
        } else {
#line 1380 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1381 "compiler/main.gem"
    GemVal _t3689 = gem_table_new();
    GemVal _t3690[] = {_t3689, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3691 = (*gem_v_compile_stmts_tail);
    GemVal _t3692[] = {gem_v_b, _t3691.fn(_t3691.env, _t3690, 3)};
                (void)(gem_buf_push_fn(NULL, _t3692, 2));
            }
        }
    } else {
#line 1383 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1384 "compiler/main.gem"
    GemVal _t3693[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3694[] = {gem_v_b, gem_interp(2, _t3693)};
            (void)(gem_buf_push_fn(NULL, _t3694, 2));
#line 1385 "compiler/main.gem"
    GemVal _t3695 = gem_table_new();
    GemVal _t3696[] = {_t3695, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3697 = (*gem_v_compile_stmts_tail);
    GemVal _t3698[] = {gem_v_b, _t3697.fn(_t3697.env, _t3696, 3)};
            (void)(gem_buf_push_fn(NULL, _t3698, 2));
        }
    }
#line 1388 "compiler/main.gem"
    GemVal _t3699[] = {gem_v_p, gem_string("}")};
    GemVal _t3700[] = {gem_v_b, gem_interp(2, _t3699)};
    (void)(gem_buf_push_fn(NULL, _t3700, 2));
#line 1389 "compiler/main.gem"
    GemVal _t3701[] = {gem_v_b};
    GemVal _t3702 = gem_buf_str_fn(NULL, _t3701, 1);
    gem_pop_frame();
    return _t3702;
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
#line 1396 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1397 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1398 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1399 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1400 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1401 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1402 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1403 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1404 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1405 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1406 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1407 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1408 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1411 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1412 "compiler/main.gem"
    GemVal _t3704[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3704, 1), gem_int(0)))) {
#line 1413 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1415 "compiler/main.gem"
        GemVal gem_v__for_i_68 = gem_int(0);
#line 1415 "compiler/main.gem"
    GemVal _t3705[] = {gem_v_params};
        GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t3705, 1);
#line 1415 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1415 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_68;
#line 1415 "compiler/main.gem"
            gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1416 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1417 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1419 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1420 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1421 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1422 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1423 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1424 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1425 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1426 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1427 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1428 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1429 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1430 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1431 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1433 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1438 "compiler/main.gem"
    GemVal _t3706[] = {gem_v_c_ret, gem_string(" "), gem_v_name, gem_string("("), gem_v_c_params, gem_string(");")};
    GemVal _t3707[] = {(*gem_v_extern_c_decls), gem_interp(6, _t3706)};
    GemVal _t3708 = gem_push_fn(NULL, _t3707, 2);
    gem_pop_frame();
    return _t3708;
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
#line 1442 "compiler/main.gem"
    GemVal gem_v__d69 = gem_v_node;
#line 1442 "compiler/main.gem"
    GemVal _t3710 = gem_v__d69;
    GemVal gem_v_name = gem_table_get(_t3710, gem_string("name"));
#line 1442 "compiler/main.gem"
    GemVal _t3711 = gem_v__d69;
    GemVal gem_v_params = gem_table_get(_t3711, gem_string("params"));
#line 1442 "compiler/main.gem"
    GemVal _t3712 = gem_v__d69;
    GemVal gem_v_ret_type = gem_table_get(_t3712, gem_string("ret_type"));

#line 1444 "compiler/main.gem"
    GemVal _t3713 = gem_v_node;
    if (gem_truthy(gem_table_get(_t3713, gem_string("blocking")))) {
#line 1445 "compiler/main.gem"
    GemVal _t3714[] = {gem_v_node};
    GemVal _t3715 = (*gem_v_compile_blocking_extern_fn);
        GemVal _t3716 = _t3715.fn(_t3715.env, _t3714, 1);
        gem_pop_frame();
        return _t3716;
    }
#line 1448 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1449 "compiler/main.gem"
    GemVal _t3717[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3718[] = {gem_v_b, gem_interp(3, _t3717)};
    (void)(gem_buf_push_fn(NULL, _t3718, 2));
#line 1451 "compiler/main.gem"
    GemVal _t3719 = gem_table_new();
    GemVal gem_v_c_args = _t3719;
#line 1452 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1452 "compiler/main.gem"
    GemVal _t3720[] = {gem_v_params};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3720, 1);
#line 1452 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1452 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1452 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1453 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1454 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1455 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1456 "compiler/main.gem"
    GemVal _t3721[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3722[] = {gem_v_b, gem_interp(5, _t3721)};
            (void)(gem_buf_push_fn(NULL, _t3722, 2));
#line 1457 "compiler/main.gem"
    GemVal _t3723[] = {gem_string("_p"), gem_v_i};
    GemVal _t3724[] = {gem_v_c_args, gem_interp(2, _t3723)};
            (void)(gem_push_fn(NULL, _t3724, 2));
        } else {
#line 1458 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1459 "compiler/main.gem"
    GemVal _t3725[] = {gem_string("    double _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].fval;\n")};
    GemVal _t3726[] = {gem_v_b, gem_interp(5, _t3725)};
                (void)(gem_buf_push_fn(NULL, _t3726, 2));
#line 1460 "compiler/main.gem"
    GemVal _t3727[] = {gem_string("_p"), gem_v_i};
    GemVal _t3728[] = {gem_v_c_args, gem_interp(2, _t3727)};
                (void)(gem_push_fn(NULL, _t3728, 2));
            } else {
#line 1461 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1462 "compiler/main.gem"
    GemVal _t3729[] = {gem_string("    char* _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].sval;\n")};
    GemVal _t3730[] = {gem_v_b, gem_interp(5, _t3729)};
                    (void)(gem_buf_push_fn(NULL, _t3730, 2));
#line 1463 "compiler/main.gem"
    GemVal _t3731[] = {gem_string("_p"), gem_v_i};
    GemVal _t3732[] = {gem_v_c_args, gem_interp(2, _t3731)};
                    (void)(gem_push_fn(NULL, _t3732, 2));
                } else {
#line 1464 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1465 "compiler/main.gem"
    GemVal _t3733[] = {gem_string("    int _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].bval;\n")};
    GemVal _t3734[] = {gem_v_b, gem_interp(5, _t3733)};
                        (void)(gem_buf_push_fn(NULL, _t3734, 2));
#line 1466 "compiler/main.gem"
    GemVal _t3735[] = {gem_string("_p"), gem_v_i};
    GemVal _t3736[] = {gem_v_c_args, gem_interp(2, _t3735)};
                        (void)(gem_push_fn(NULL, _t3736, 2));
                    } else {
#line 1467 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1468 "compiler/main.gem"
    GemVal _t3737[] = {gem_string("    void* _p"), gem_v_i, gem_string(" = (void*)(intptr_t)args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3738[] = {gem_v_b, gem_interp(5, _t3737)};
                            (void)(gem_buf_push_fn(NULL, _t3738, 2));
#line 1469 "compiler/main.gem"
    GemVal _t3739[] = {gem_string("_p"), gem_v_i};
    GemVal _t3740[] = {gem_v_c_args, gem_interp(2, _t3739)};
                            (void)(gem_push_fn(NULL, _t3740, 2));
                        } else {
#line 1470 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1471 "compiler/main.gem"
    GemVal _t3741[] = {gem_string("args["), gem_v_i, gem_string("]")};
    GemVal _t3742[] = {gem_v_c_args, gem_interp(3, _t3741)};
                                (void)(gem_push_fn(NULL, _t3742, 2));
                            } else {
#line 1473 "compiler/main.gem"
    GemVal _t3743[] = {gem_string("args["), gem_v_i, gem_string("]")};
    GemVal _t3744[] = {gem_v_c_args, gem_interp(3, _t3743)};
                                (void)(gem_push_fn(NULL, _t3744, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1478 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1479 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1479 "compiler/main.gem"
    GemVal _t3745[] = {gem_v_c_args};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3745, 1);
#line 1479 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1479 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1479 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1480 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1481 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1483 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1486 "compiler/main.gem"
    GemVal _t3746[] = {gem_v_name, gem_string("("), gem_v_call_args, gem_string(")")};
    GemVal gem_v_call_expr = gem_interp(4, _t3746);
#line 1488 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1489 "compiler/main.gem"
    GemVal _t3747[] = {gem_string("    "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3748[] = {gem_v_b, gem_interp(3, _t3747)};
        (void)(gem_buf_push_fn(NULL, _t3748, 2));
#line 1490 "compiler/main.gem"
    GemVal _t3749[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3749, 2));
    } else {
#line 1491 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1492 "compiler/main.gem"
    GemVal _t3750[] = {gem_string("    return "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3751[] = {gem_v_b, gem_interp(3, _t3750)};
            (void)(gem_buf_push_fn(NULL, _t3751, 2));
        } else {
#line 1493 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1494 "compiler/main.gem"
    GemVal _t3752[] = {gem_string("    int64_t _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3753[] = {gem_v_b, gem_interp(3, _t3752)};
                (void)(gem_buf_push_fn(NULL, _t3753, 2));
#line 1495 "compiler/main.gem"
    GemVal _t3754[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3754, 2));
            } else {
#line 1496 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1497 "compiler/main.gem"
    GemVal _t3755[] = {gem_string("    double _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3756[] = {gem_v_b, gem_interp(3, _t3755)};
                    (void)(gem_buf_push_fn(NULL, _t3756, 2));
#line 1498 "compiler/main.gem"
    GemVal _t3757[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3757, 2));
                } else {
#line 1499 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1500 "compiler/main.gem"
    GemVal _t3758[] = {gem_string("    char* _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3759[] = {gem_v_b, gem_interp(3, _t3758)};
                        (void)(gem_buf_push_fn(NULL, _t3759, 2));
#line 1501 "compiler/main.gem"
    GemVal _t3760[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3760, 2));
                    } else {
#line 1502 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1503 "compiler/main.gem"
    GemVal _t3761[] = {gem_string("    int _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3762[] = {gem_v_b, gem_interp(3, _t3761)};
                            (void)(gem_buf_push_fn(NULL, _t3762, 2));
#line 1504 "compiler/main.gem"
    GemVal _t3763[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                            (void)(gem_buf_push_fn(NULL, _t3763, 2));
                        } else {
#line 1505 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1506 "compiler/main.gem"
    GemVal _t3764[] = {gem_string("    void *_ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3765[] = {gem_v_b, gem_interp(3, _t3764)};
                                (void)(gem_buf_push_fn(NULL, _t3765, 2));
#line 1507 "compiler/main.gem"
    GemVal _t3766[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3766, 2));
                            } else {
#line 1509 "compiler/main.gem"
    GemVal _t3767[] = {gem_string("    "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3768[] = {gem_v_b, gem_interp(3, _t3767)};
                                (void)(gem_buf_push_fn(NULL, _t3768, 2));
#line 1510 "compiler/main.gem"
    GemVal _t3769[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3769, 2));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1513 "compiler/main.gem"
    GemVal _t3770[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3770, 2));
#line 1515 "compiler/main.gem"
    GemVal _t3771[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc);")};
    GemVal _t3772[] = {(*gem_v_forward_decls), gem_interp(3, _t3771)};
    (void)(gem_push_fn(NULL, _t3772, 2));
#line 1516 "compiler/main.gem"
    GemVal _t3773[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3774 = (*gem_v_emit_extern_c_decl);
    (void)(_t3774.fn(_t3774.env, _t3773, 3));
#line 1518 "compiler/main.gem"
    GemVal _t3775[] = {gem_v_b};
    GemVal _t3776 = gem_buf_str_fn(NULL, _t3775, 1);
    gem_pop_frame();
    return _t3776;
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
#line 1525 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1525 "compiler/main.gem"
    GemVal _t3778 = gem_v__d72;
    GemVal gem_v_name = gem_table_get(_t3778, gem_string("name"));
#line 1525 "compiler/main.gem"
    GemVal _t3779 = gem_v__d72;
    GemVal gem_v_params = gem_table_get(_t3779, gem_string("params"));
#line 1525 "compiler/main.gem"
    GemVal _t3780 = gem_v__d72;
    GemVal gem_v_ret_type = gem_table_get(_t3780, gem_string("ret_type"));

#line 1526 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1529 "compiler/main.gem"
    GemVal _t3781[] = {gem_string("struct _blk_"), gem_v_name, gem_string("_args {\n")};
    GemVal _t3782[] = {gem_v_b, gem_interp(3, _t3781)};
    (void)(gem_buf_push_fn(NULL, _t3782, 2));
#line 1530 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1530 "compiler/main.gem"
    GemVal _t3783[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t3783, 1);
#line 1530 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1530 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1530 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1531 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1532 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1533 "compiler/main.gem"
    GemVal _t3784[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3785[] = {gem_v_b, gem_interp(3, _t3784)};
            (void)(gem_buf_push_fn(NULL, _t3785, 2));
        } else {
#line 1534 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1535 "compiler/main.gem"
    GemVal _t3786[] = {gem_string("    double _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3787[] = {gem_v_b, gem_interp(3, _t3786)};
                (void)(gem_buf_push_fn(NULL, _t3787, 2));
            } else {
#line 1536 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1537 "compiler/main.gem"
    GemVal _t3788[] = {gem_string("    char *_p"), gem_v_i, gem_string(";\n")};
    GemVal _t3789[] = {gem_v_b, gem_interp(3, _t3788)};
                    (void)(gem_buf_push_fn(NULL, _t3789, 2));
                } else {
#line 1538 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1539 "compiler/main.gem"
    GemVal _t3790[] = {gem_string("    int _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3791[] = {gem_v_b, gem_interp(3, _t3790)};
                        (void)(gem_buf_push_fn(NULL, _t3791, 2));
                    } else {
#line 1540 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1541 "compiler/main.gem"
    GemVal _t3792[] = {gem_string("    void *_p"), gem_v_i, gem_string(";\n")};
    GemVal _t3793[] = {gem_v_b, gem_interp(3, _t3792)};
                            (void)(gem_buf_push_fn(NULL, _t3793, 2));
                        }
                    }
                }
            }
        }
    }

#line 1544 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1545 "compiler/main.gem"
    GemVal _t3794[] = {gem_v_b, gem_string("    int64_t _result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3794, 2));
    } else {
#line 1546 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1547 "compiler/main.gem"
    GemVal _t3795[] = {gem_v_b, gem_string("    double _result;\n")};
            (void)(gem_buf_push_fn(NULL, _t3795, 2));
        } else {
#line 1548 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1549 "compiler/main.gem"
    GemVal _t3796[] = {gem_v_b, gem_string("    char *_result;\n")};
                (void)(gem_buf_push_fn(NULL, _t3796, 2));
            } else {
#line 1550 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1551 "compiler/main.gem"
    GemVal _t3797[] = {gem_v_b, gem_string("    int _result;\n")};
                    (void)(gem_buf_push_fn(NULL, _t3797, 2));
                } else {
#line 1552 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1553 "compiler/main.gem"
    GemVal _t3798[] = {gem_v_b, gem_string("    void *_result;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3798, 2));
                    }
                }
            }
        }
    }
#line 1555 "compiler/main.gem"
    GemVal _t3799[] = {gem_v_b, gem_string("};\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3799, 2));
#line 1558 "compiler/main.gem"
    GemVal _t3800[] = {gem_string("static void _blk_"), gem_v_name, gem_string("_worker(void *_a) {\n")};
    GemVal _t3801[] = {gem_v_b, gem_interp(3, _t3800)};
    (void)(gem_buf_push_fn(NULL, _t3801, 2));
#line 1559 "compiler/main.gem"
    GemVal _t3802[] = {gem_string("    struct _blk_"), gem_v_name, gem_string("_args *a = (struct _blk_"), gem_v_name, gem_string("_args *)_a;\n")};
    GemVal _t3803[] = {gem_v_b, gem_interp(5, _t3802)};
    (void)(gem_buf_push_fn(NULL, _t3803, 2));
#line 1561 "compiler/main.gem"
    GemVal gem_v_w_args = gem_string("");
#line 1562 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1562 "compiler/main.gem"
    GemVal _t3804[] = {gem_v_params};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3804, 1);
#line 1562 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1562 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1562 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1563 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1564 "compiler/main.gem"
            gem_v_w_args = gem_add(gem_v_w_args, gem_string(", "));
        }
#line 1566 "compiler/main.gem"
    GemVal _t3805[] = {gem_string("a->_p"), gem_v_i};
        gem_v_w_args = gem_add(gem_v_w_args, gem_interp(2, _t3805));
    }

#line 1568 "compiler/main.gem"
    GemVal _t3806[] = {gem_v_name, gem_string("("), gem_v_w_args, gem_string(")")};
    GemVal gem_v_w_call = gem_interp(4, _t3806);
#line 1570 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1571 "compiler/main.gem"
    GemVal _t3807[] = {gem_string("    "), gem_v_w_call, gem_string(";\n")};
    GemVal _t3808[] = {gem_v_b, gem_interp(3, _t3807)};
        (void)(gem_buf_push_fn(NULL, _t3808, 2));
    } else {
#line 1573 "compiler/main.gem"
    GemVal _t3809[] = {gem_string("    a->_result = "), gem_v_w_call, gem_string(";\n")};
    GemVal _t3810[] = {gem_v_b, gem_interp(3, _t3809)};
        (void)(gem_buf_push_fn(NULL, _t3810, 2));
    }
#line 1575 "compiler/main.gem"
    GemVal _t3811[] = {gem_v_b, gem_string("}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3811, 2));
#line 1578 "compiler/main.gem"
    GemVal _t3812[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3813[] = {gem_v_b, gem_interp(3, _t3812)};
    (void)(gem_buf_push_fn(NULL, _t3813, 2));
#line 1581 "compiler/main.gem"
    GemVal gem_v__for_i_75 = gem_int(0);
#line 1581 "compiler/main.gem"
    GemVal _t3814[] = {gem_v_params};
    GemVal gem_v__for_limit_75 = gem_len_fn(NULL, _t3814, 1);
#line 1581 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1581 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_75;
#line 1581 "compiler/main.gem"
        gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1582 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1583 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1584 "compiler/main.gem"
    GemVal _t3815[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3816[] = {gem_v_b, gem_interp(5, _t3815)};
            (void)(gem_buf_push_fn(NULL, _t3816, 2));
        } else {
#line 1585 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1586 "compiler/main.gem"
    GemVal _t3817[] = {gem_string("    double _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].fval;\n")};
    GemVal _t3818[] = {gem_v_b, gem_interp(5, _t3817)};
                (void)(gem_buf_push_fn(NULL, _t3818, 2));
            } else {
#line 1587 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1588 "compiler/main.gem"
    GemVal _t3819[] = {gem_string("    char* _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].sval;\n")};
    GemVal _t3820[] = {gem_v_b, gem_interp(5, _t3819)};
                    (void)(gem_buf_push_fn(NULL, _t3820, 2));
                } else {
#line 1589 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1590 "compiler/main.gem"
    GemVal _t3821[] = {gem_string("    int _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].bval;\n")};
    GemVal _t3822[] = {gem_v_b, gem_interp(5, _t3821)};
                        (void)(gem_buf_push_fn(NULL, _t3822, 2));
                    } else {
#line 1591 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1592 "compiler/main.gem"
    GemVal _t3823[] = {gem_string("    void* _p"), gem_v_i, gem_string(" = (void*)(intptr_t)args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3824[] = {gem_v_b, gem_interp(5, _t3823)};
                            (void)(gem_buf_push_fn(NULL, _t3824, 2));
                        }
                    }
                }
            }
        }
    }

#line 1597 "compiler/main.gem"
    GemVal _t3825[] = {gem_v_b, gem_string("    if (gem_current_pid >= 0) {\n")};
    (void)(gem_buf_push_fn(NULL, _t3825, 2));
#line 1598 "compiler/main.gem"
    GemVal _t3826[] = {gem_string("        struct _blk_"), gem_v_name, gem_string("_args *_ba = (struct _blk_"), gem_v_name, gem_string("_args *)malloc(sizeof(struct _blk_"), gem_v_name, gem_string("_args));\n")};
    GemVal _t3827[] = {gem_v_b, gem_interp(7, _t3826)};
    (void)(gem_buf_push_fn(NULL, _t3827, 2));
#line 1601 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1601 "compiler/main.gem"
    GemVal _t3828[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t3828, 1);
#line 1601 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1601 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1601 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1602 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1603 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1604 "compiler/main.gem"
    GemVal _t3829[] = {gem_string("        _ba->_p"), gem_v_i, gem_string(" = strdup(_p"), gem_v_i, gem_string(");\n")};
    GemVal _t3830[] = {gem_v_b, gem_interp(5, _t3829)};
            (void)(gem_buf_push_fn(NULL, _t3830, 2));
        } else {
#line 1606 "compiler/main.gem"
    GemVal _t3831[] = {gem_string("        _ba->_p"), gem_v_i, gem_string(" = _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3832[] = {gem_v_b, gem_interp(5, _t3831)};
            (void)(gem_buf_push_fn(NULL, _t3832, 2));
        }
    }

#line 1610 "compiler/main.gem"
    GemVal _t3833[] = {gem_string("        GemIORequest *_req = gem_io_submit_extern(_blk_"), gem_v_name, gem_string("_worker, _ba);\n")};
    GemVal _t3834[] = {gem_v_b, gem_interp(3, _t3833)};
    (void)(gem_buf_push_fn(NULL, _t3834, 2));
#line 1611 "compiler/main.gem"
    GemVal _t3835[] = {gem_string("        if (!_req) { free(_ba); gem_error(\""), gem_v_name, gem_string(": I/O queue full\"); }\n")};
    GemVal _t3836[] = {gem_v_b, gem_interp(3, _t3835)};
    (void)(gem_buf_push_fn(NULL, _t3836, 2));
#line 1612 "compiler/main.gem"
    GemVal _t3837[] = {gem_v_b, gem_string("        GemProcess *_proc = &gem_proc_table[gem_current_pid];\n")};
    (void)(gem_buf_push_fn(NULL, _t3837, 2));
#line 1613 "compiler/main.gem"
    GemVal _t3838[] = {gem_v_b, gem_string("        _proc->io_request = _req;\n")};
    (void)(gem_buf_push_fn(NULL, _t3838, 2));
#line 1614 "compiler/main.gem"
    GemVal _t3839[] = {gem_v_b, gem_string("        gem_io_pool_yield();\n")};
    (void)(gem_buf_push_fn(NULL, _t3839, 2));
#line 1615 "compiler/main.gem"
    GemVal _t3840[] = {gem_v_b, gem_string("        _proc->io_request = NULL;\n")};
    (void)(gem_buf_push_fn(NULL, _t3840, 2));
#line 1618 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1619 "compiler/main.gem"
    GemVal _t3841[] = {gem_v_b, gem_string("        char *_raw = _ba->_result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3841, 2));
    } else {
#line 1620 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_ret_type, gem_string("Nil")))) {
#line 1621 "compiler/main.gem"
            GemVal gem_v_c_type = gem_string("int64_t");
#line 1622 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1623 "compiler/main.gem"
                gem_v_c_type = gem_string("double");
            } else {
#line 1624 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1625 "compiler/main.gem"
                    gem_v_c_type = gem_string("int");
                } else {
#line 1626 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1627 "compiler/main.gem"
                        gem_v_c_type = gem_string("void *");
                    }
                }
            }
#line 1629 "compiler/main.gem"
    GemVal _t3842[] = {gem_string("        "), gem_v_c_type, gem_string(" _ret = _ba->_result;\n")};
    GemVal _t3843[] = {gem_v_b, gem_interp(3, _t3842)};
            (void)(gem_buf_push_fn(NULL, _t3843, 2));
        }
    }
#line 1633 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1633 "compiler/main.gem"
    GemVal _t3844[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t3844, 1);
#line 1633 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1633 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1633 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1634 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type")), gem_string("String")))) {
#line 1635 "compiler/main.gem"
    GemVal _t3845[] = {gem_string("        free(_ba->_p"), gem_v_i, gem_string(");\n")};
    GemVal _t3846[] = {gem_v_b, gem_interp(3, _t3845)};
            (void)(gem_buf_push_fn(NULL, _t3846, 2));
        }
    }

#line 1639 "compiler/main.gem"
    GemVal _t3847[] = {gem_v_b, gem_string("        free(_ba);\n")};
    (void)(gem_buf_push_fn(NULL, _t3847, 2));
#line 1640 "compiler/main.gem"
    GemVal _t3848[] = {gem_v_b, gem_string("        gem_io_free_request(_req);\n")};
    (void)(gem_buf_push_fn(NULL, _t3848, 2));
#line 1643 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1644 "compiler/main.gem"
    GemVal _t3849[] = {gem_v_b, gem_string("        return gem_int(_ret);\n")};
        (void)(gem_buf_push_fn(NULL, _t3849, 2));
    } else {
#line 1645 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1646 "compiler/main.gem"
    GemVal _t3850[] = {gem_v_b, gem_string("        return gem_float(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3850, 2));
        } else {
#line 1647 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1648 "compiler/main.gem"
    GemVal _t3851[] = {gem_v_b, gem_string("        size_t _len = _raw ? strlen(_raw) : 0;\n")};
                (void)(gem_buf_push_fn(NULL, _t3851, 2));
#line 1649 "compiler/main.gem"
    GemVal _t3852[] = {gem_v_b, gem_string("        char *_gc = (char *)GC_MALLOC_ATOMIC(_len + 1);\n")};
                (void)(gem_buf_push_fn(NULL, _t3852, 2));
#line 1650 "compiler/main.gem"
    GemVal _t3853[] = {gem_v_b, gem_string("        if (_raw) { memcpy(_gc, _raw, _len + 1); free(_raw); } else { _gc[0] = '\\0'; }\n")};
                (void)(gem_buf_push_fn(NULL, _t3853, 2));
#line 1651 "compiler/main.gem"
    GemVal _t3854[] = {gem_v_b, gem_string("        return gem_string(_gc);\n")};
                (void)(gem_buf_push_fn(NULL, _t3854, 2));
            } else {
#line 1652 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1653 "compiler/main.gem"
    GemVal _t3855[] = {gem_v_b, gem_string("        return gem_bool(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3855, 2));
                } else {
#line 1654 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1655 "compiler/main.gem"
    GemVal _t3856[] = {gem_v_b, gem_string("        GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3856, 2));
                    } else {
#line 1657 "compiler/main.gem"
    GemVal _t3857[] = {gem_v_b, gem_string("        return GEM_NIL;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3857, 2));
                    }
                }
            }
        }
    }
#line 1659 "compiler/main.gem"
    GemVal _t3858[] = {gem_v_b, gem_string("    }\n")};
    (void)(gem_buf_push_fn(NULL, _t3858, 2));
#line 1662 "compiler/main.gem"
    GemVal gem_v_d_args = gem_string("");
#line 1663 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1663 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_params};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t3859, 1);
#line 1663 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1663 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1663 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1664 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1665 "compiler/main.gem"
            gem_v_d_args = gem_add(gem_v_d_args, gem_string(", "));
        }
#line 1667 "compiler/main.gem"
    GemVal _t3860[] = {gem_string("_p"), gem_v_i};
        gem_v_d_args = gem_add(gem_v_d_args, gem_interp(2, _t3860));
    }

#line 1669 "compiler/main.gem"
    GemVal _t3861[] = {gem_v_name, gem_string("("), gem_v_d_args, gem_string(")")};
    GemVal gem_v_d_call = gem_interp(4, _t3861);
#line 1671 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1672 "compiler/main.gem"
    GemVal _t3862[] = {gem_string("    "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3863[] = {gem_v_b, gem_interp(3, _t3862)};
        (void)(gem_buf_push_fn(NULL, _t3863, 2));
#line 1673 "compiler/main.gem"
    GemVal _t3864[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3864, 2));
    } else {
#line 1674 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1675 "compiler/main.gem"
    GemVal _t3865[] = {gem_string("    int64_t _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3866[] = {gem_v_b, gem_interp(3, _t3865)};
            (void)(gem_buf_push_fn(NULL, _t3866, 2));
#line 1676 "compiler/main.gem"
    GemVal _t3867[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3867, 2));
        } else {
#line 1677 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1678 "compiler/main.gem"
    GemVal _t3868[] = {gem_string("    double _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3869[] = {gem_v_b, gem_interp(3, _t3868)};
                (void)(gem_buf_push_fn(NULL, _t3869, 2));
#line 1679 "compiler/main.gem"
    GemVal _t3870[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3870, 2));
            } else {
#line 1680 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1681 "compiler/main.gem"
    GemVal _t3871[] = {gem_string("    char* _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3872[] = {gem_v_b, gem_interp(3, _t3871)};
                    (void)(gem_buf_push_fn(NULL, _t3872, 2));
#line 1682 "compiler/main.gem"
    GemVal _t3873[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3873, 2));
                } else {
#line 1683 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1684 "compiler/main.gem"
    GemVal _t3874[] = {gem_string("    int _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3875[] = {gem_v_b, gem_interp(3, _t3874)};
                        (void)(gem_buf_push_fn(NULL, _t3875, 2));
#line 1685 "compiler/main.gem"
    GemVal _t3876[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3876, 2));
                    } else {
#line 1686 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1687 "compiler/main.gem"
    GemVal _t3877[] = {gem_string("    void *_ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3878[] = {gem_v_b, gem_interp(3, _t3877)};
                            (void)(gem_buf_push_fn(NULL, _t3878, 2));
#line 1688 "compiler/main.gem"
    GemVal _t3879[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3879, 2));
                        } else {
#line 1690 "compiler/main.gem"
    GemVal _t3880[] = {gem_string("    "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3881[] = {gem_v_b, gem_interp(3, _t3880)};
                            (void)(gem_buf_push_fn(NULL, _t3881, 2));
#line 1691 "compiler/main.gem"
    GemVal _t3882[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3882, 2));
                        }
                    }
                }
            }
        }
    }
#line 1694 "compiler/main.gem"
    GemVal _t3883[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3883, 2));
#line 1696 "compiler/main.gem"
    GemVal _t3884[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc);")};
    GemVal _t3885[] = {(*gem_v_forward_decls), gem_interp(3, _t3884)};
    (void)(gem_push_fn(NULL, _t3885, 2));
#line 1697 "compiler/main.gem"
    GemVal _t3886[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3887 = (*gem_v_emit_extern_c_decl);
    (void)(_t3887.fn(_t3887.env, _t3886, 3));
#line 1699 "compiler/main.gem"
    GemVal _t3888[] = {gem_v_b};
    GemVal _t3889 = gem_buf_str_fn(NULL, _t3888, 1);
    gem_pop_frame();
    return _t3889;
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
#line 1705 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1707 "compiler/main.gem"
    GemVal gem_v__for_i_79 = gem_int(0);
#line 1707 "compiler/main.gem"
    GemVal _t3891[] = {gem_v_params};
    GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t3891, 1);
#line 1707 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1707 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_79;
#line 1707 "compiler/main.gem"
        gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1708 "compiler/main.gem"
    GemVal _t3892[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3893 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3893.fn(_t3893.env, _t3892, 1);
#line 1709 "compiler/main.gem"
    GemVal _t3894[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3894, 2))) {
#line 1710 "compiler/main.gem"
    GemVal _t3895[] = {gem_string("    GemVal *"), gem_v_mp, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3896[] = {gem_v_b, gem_interp(3, _t3895)};
            (void)(gem_buf_push_fn(NULL, _t3896, 2));
#line 1711 "compiler/main.gem"
    GemVal _t3897[] = {gem_string("    *"), gem_v_mp, gem_string(" = (argc > "), gem_v_i, gem_string(") ? args["), gem_v_i, gem_string("] : GEM_NIL;\n")};
    GemVal _t3898[] = {gem_v_b, gem_interp(7, _t3897)};
            (void)(gem_buf_push_fn(NULL, _t3898, 2));
        } else {
#line 1713 "compiler/main.gem"
    GemVal _t3899[] = {gem_string("    GemVal "), gem_v_mp, gem_string(" = (argc > "), gem_v_i, gem_string(") ? args["), gem_v_i, gem_string("] : GEM_NIL;\n")};
    GemVal _t3900[] = {gem_v_b, gem_interp(7, _t3899)};
            (void)(gem_buf_push_fn(NULL, _t3900, 2));
        }
    }

#line 1718 "compiler/main.gem"
    GemVal _t3901 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3901, gem_string("rest_param")), GEM_NIL))) {
#line 1719 "compiler/main.gem"
    GemVal _t3902[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3902, 1);
#line 1720 "compiler/main.gem"
    GemVal _t3903 = gem_v_node;
    GemVal _t3904[] = {gem_table_get(_t3903, gem_string("rest_param"))};
    GemVal _t3905 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3905.fn(_t3905.env, _t3904, 1);
#line 1721 "compiler/main.gem"
    GemVal _t3906 = gem_v_node;
    GemVal _t3907[] = {gem_v_cap_set, gem_table_get(_t3906, gem_string("rest_param"))};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3907, 2))) {
#line 1722 "compiler/main.gem"
    GemVal _t3908[] = {gem_string("    GemVal *"), gem_v_mp_rest, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3909[] = {gem_v_b, gem_interp(3, _t3908)};
            (void)(gem_buf_push_fn(NULL, _t3909, 2));
#line 1723 "compiler/main.gem"
    GemVal _t3910[] = {gem_string("    *"), gem_v_mp_rest, gem_string(" = gem_table_new();\n")};
    GemVal _t3911[] = {gem_v_b, gem_interp(3, _t3910)};
            (void)(gem_buf_push_fn(NULL, _t3911, 2));
#line 1724 "compiler/main.gem"
    GemVal _t3912[] = {gem_string("    for (int _ri = "), gem_v_N, gem_string("; _ri < argc; _ri++) gem_table_set(*"), gem_v_mp_rest, gem_string(", gem_int(_ri - "), gem_v_N, gem_string("), args[_ri]);\n")};
    GemVal _t3913[] = {gem_v_b, gem_interp(7, _t3912)};
            (void)(gem_buf_push_fn(NULL, _t3913, 2));
        } else {
#line 1726 "compiler/main.gem"
    GemVal _t3914[] = {gem_string("    GemVal "), gem_v_mp_rest, gem_string(" = gem_table_new();\n")};
    GemVal _t3915[] = {gem_v_b, gem_interp(3, _t3914)};
            (void)(gem_buf_push_fn(NULL, _t3915, 2));
#line 1727 "compiler/main.gem"
    GemVal _t3916[] = {gem_string("    for (int _ri = "), gem_v_N, gem_string("; _ri < argc; _ri++) gem_table_set("), gem_v_mp_rest, gem_string(", gem_int(_ri - "), gem_v_N, gem_string("), args[_ri]);\n")};
    GemVal _t3917[] = {gem_v_b, gem_interp(7, _t3916)};
            (void)(gem_buf_push_fn(NULL, _t3917, 2));
        }
    }
#line 1730 "compiler/main.gem"
    GemVal _t3918[] = {gem_v_b};
    GemVal _t3919 = gem_buf_str_fn(NULL, _t3918, 1);
    gem_pop_frame();
    return _t3919;
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
#line 1736 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1737 "compiler/main.gem"
    GemVal _t3921[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3921, 1), gem_int(0)))) {
#line 1738 "compiler/main.gem"
        GemVal gem_v__for_i_80 = gem_int(0);
#line 1738 "compiler/main.gem"
    GemVal _t3922[] = {gem_v_body};
        GemVal gem_v__for_limit_80 = gem_sub(gem_len_fn(NULL, _t3922, 1), gem_int(1));
#line 1738 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1738 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_80;
#line 1738 "compiler/main.gem"
            gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1739 "compiler/main.gem"
    GemVal _t3923[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t3924 = (*gem_v_compile_stmt);
    GemVal _t3925[] = {gem_v_b, _t3924.fn(_t3924.env, _t3923, 2)};
            (void)(gem_buf_push_fn(NULL, _t3925, 2));
#line 1740 "compiler/main.gem"
    GemVal _t3926[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3926, 2));
        }

#line 1742 "compiler/main.gem"
    GemVal _t3927[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t3928 = (*gem_v_compile_stmt_return);
    GemVal _t3929[] = {gem_v_b, _t3928.fn(_t3928.env, _t3927, 2)};
        (void)(gem_buf_push_fn(NULL, _t3929, 2));
#line 1743 "compiler/main.gem"
    GemVal _t3930[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3930, 2));
    } else {
#line 1745 "compiler/main.gem"
    GemVal _t3931[] = {gem_v_b, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3931, 2));
    }
#line 1747 "compiler/main.gem"
    GemVal _t3932[] = {gem_v_b};
    GemVal _t3933 = gem_buf_str_fn(NULL, _t3932, 1);
    gem_pop_frame();
    return _t3933;
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
#line 1753 "compiler/main.gem"
    GemVal gem_v__d81 = gem_v_node;
#line 1753 "compiler/main.gem"
    GemVal _t3935 = gem_v__d81;
    GemVal gem_v_params = gem_table_get(_t3935, gem_string("params"));
#line 1753 "compiler/main.gem"
    GemVal _t3936 = gem_v__d81;
    GemVal gem_v_body = gem_table_get(_t3936, gem_string("body"));
#line 1753 "compiler/main.gem"
    GemVal _t3937 = gem_v__d81;
    GemVal gem_v_name = gem_table_get(_t3937, gem_string("name"));

#line 1756 "compiler/main.gem"
    GemVal _t3938[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t3938, 1);
#line 1757 "compiler/main.gem"
    GemVal _t3939 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3939, gem_string("rest_param")), GEM_NIL))) {
#line 1758 "compiler/main.gem"
    GemVal _t3940 = gem_v_node;
    GemVal _t3941[] = {gem_v_scope_vars, gem_table_get(_t3940, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t3941, 2));
    }
#line 1760 "compiler/main.gem"
    GemVal _t3942[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t3942, 2));
#line 1762 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1763 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1764 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1765 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1767 "compiler/main.gem"
    GemVal _t3943[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t3944 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t3944.fn(_t3944.env, _t3943, 2);
#line 1769 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1770 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1772 "compiler/main.gem"
    GemVal gem_v_use_tco = gem_bool(0);
#line 1773 "compiler/main.gem"
    GemVal _t3945 = gem_v_node;
    GemVal _t3947;
    if (!gem_truthy(gem_eq(gem_table_get(_t3945, gem_string("rest_param")), GEM_NIL))) {
        _t3947 = gem_eq(gem_table_get(_t3945, gem_string("rest_param")), GEM_NIL);
    } else {
        GemVal _t3946[] = {gem_v_scope_vars, gem_v_name};
        _t3947 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3946, 2));
    }
    if (gem_truthy(_t3947)) {
#line 1774 "compiler/main.gem"
    GemVal _t3948[] = {gem_v_name, gem_v_body};
    GemVal _t3949 = (*gem_v_has_self_tail_call);
        gem_v_use_tco = _t3949.fn(_t3949.env, _t3948, 2);
    }
#line 1776 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1777 "compiler/main.gem"
        *gem_v_tco_fn_name = gem_v_name;
#line 1778 "compiler/main.gem"
        *gem_v_tco_params = gem_v_params;
#line 1779 "compiler/main.gem"
        *gem_v_tco_captured = gem_v_captured;
    }
#line 1782 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1783 "compiler/main.gem"
    GemVal _t3950[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3951[] = {gem_v_b, gem_interp(3, _t3950)};
    (void)(gem_buf_push_fn(NULL, _t3951, 2));
#line 1785 "compiler/main.gem"
    GemVal _t3952 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3952, gem_string("line")), GEM_NIL))) {
#line 1786 "compiler/main.gem"
    GemVal _t3953 = gem_v_node;
    GemVal _t3954[] = {(*gem_v_source_name)};
    GemVal _t3955[] = {gem_string("#line "), gem_table_get(_t3953, gem_string("line")), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3954, 1), gem_string("\"\n")};
    GemVal _t3956[] = {gem_v_b, gem_interp(5, _t3955)};
        (void)(gem_buf_push_fn(NULL, _t3956, 2));
    }
#line 1789 "compiler/main.gem"
    GemVal _t3957[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t3958 = (*gem_v_emit_param_bindings);
    GemVal _t3959[] = {gem_v_b, _t3958.fn(_t3958.env, _t3957, 3)};
    (void)(gem_buf_push_fn(NULL, _t3959, 2));
#line 1791 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1792 "compiler/main.gem"
    GemVal _t3960 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3960, gem_string("line")), GEM_NIL))) {
#line 1793 "compiler/main.gem"
    GemVal _t3961 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t3961, gem_string("line"));
    }
#line 1795 "compiler/main.gem"
    GemVal _t3962[] = {gem_v_name};
    GemVal _t3963[] = {(*gem_v_source_name)};
    GemVal _t3964[] = {gem_string("    gem_push_frame(\""), gem_fn__mod_codegen_escape_c_string(NULL, _t3962, 1), gem_string("\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3963, 1), gem_string("\", "), gem_v_fn_line, gem_string(");\n")};
    GemVal _t3965[] = {gem_v_b, gem_interp(7, _t3964)};
    (void)(gem_buf_push_fn(NULL, _t3965, 2));
#line 1797 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1798 "compiler/main.gem"
    GemVal _t3966[] = {gem_v_b, gem_string("    while(1) {\n")};
        (void)(gem_buf_push_fn(NULL, _t3966, 2));
#line 1799 "compiler/main.gem"
    GemVal _t3967[] = {gem_v_b, gem_string("        gem_yield_check();\n")};
        (void)(gem_buf_push_fn(NULL, _t3967, 2));
#line 1800 "compiler/main.gem"
    GemVal _t3968[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3968, 1), gem_int(0)))) {
#line 1801 "compiler/main.gem"
            GemVal gem_v__for_i_82 = gem_int(0);
#line 1801 "compiler/main.gem"
    GemVal _t3969[] = {gem_v_body};
            GemVal gem_v__for_limit_82 = gem_sub(gem_len_fn(NULL, _t3969, 1), gem_int(1));
#line 1801 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_v__for_limit_82))) break;
#line 1801 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_82;
#line 1801 "compiler/main.gem"
                gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1802 "compiler/main.gem"
    GemVal _t3970[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(2)};
    GemVal _t3971 = (*gem_v_compile_stmt);
    GemVal _t3972[] = {gem_v_b, _t3971.fn(_t3971.env, _t3970, 2)};
                (void)(gem_buf_push_fn(NULL, _t3972, 2));
#line 1803 "compiler/main.gem"
    GemVal _t3973[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3973, 2));
            }

#line 1805 "compiler/main.gem"
    GemVal _t3974[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(2)};
    GemVal _t3975 = (*gem_v_compile_stmt_return);
    GemVal _t3976[] = {gem_v_b, _t3975.fn(_t3975.env, _t3974, 2)};
            (void)(gem_buf_push_fn(NULL, _t3976, 2));
#line 1806 "compiler/main.gem"
    GemVal _t3977[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3977, 2));
        } else {
#line 1808 "compiler/main.gem"
    GemVal _t3978[] = {gem_v_b, gem_string("        gem_pop_frame();\n        return GEM_NIL;\n")};
            (void)(gem_buf_push_fn(NULL, _t3978, 2));
        }
#line 1810 "compiler/main.gem"
    GemVal _t3979[] = {gem_v_b, gem_string("    }\n")};
        (void)(gem_buf_push_fn(NULL, _t3979, 2));
    } else {
#line 1812 "compiler/main.gem"
    GemVal _t3980[] = {gem_v_body};
    GemVal _t3981 = (*gem_v_emit_fn_body);
    GemVal _t3982[] = {gem_v_b, _t3981.fn(_t3981.env, _t3980, 1)};
        (void)(gem_buf_push_fn(NULL, _t3982, 2));
    }
#line 1815 "compiler/main.gem"
    GemVal _t3983[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3983, 2));
#line 1817 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1818 "compiler/main.gem"
        *gem_v_tco_fn_name = GEM_NIL;
#line 1819 "compiler/main.gem"
        *gem_v_tco_params = GEM_NIL;
#line 1820 "compiler/main.gem"
        *gem_v_tco_captured = GEM_NIL;
    }
#line 1823 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1824 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1825 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1826 "compiler/main.gem"
    GemVal _t3984[] = {gem_v_b};
    GemVal _t3985 = gem_buf_str_fn(NULL, _t3984, 1);
    gem_pop_frame();
    return _t3985;
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
#line 1832 "compiler/main.gem"
    GemVal gem_v__d83 = gem_v_node;
#line 1832 "compiler/main.gem"
    GemVal _t3987 = gem_v__d83;
    GemVal gem_v_params = gem_table_get(_t3987, gem_string("params"));
#line 1832 "compiler/main.gem"
    GemVal _t3988 = gem_v__d83;
    GemVal gem_v_body = gem_table_get(_t3988, gem_string("body"));

#line 1833 "compiler/main.gem"
    GemVal _t3989 = (*gem_v_anon_name);
    GemVal gem_v_name = _t3989.fn(_t3989.env, NULL, 0);
#line 1836 "compiler/main.gem"
    GemVal _t3990[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t3990, 1);
#line 1837 "compiler/main.gem"
    GemVal _t3991 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3991, gem_string("rest_param")), GEM_NIL))) {
#line 1838 "compiler/main.gem"
    GemVal _t3992 = gem_v_node;
    GemVal _t3993[] = {gem_v_inner_scope, gem_table_get(_t3992, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t3993, 2));
    }
#line 1840 "compiler/main.gem"
    GemVal _t3994[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t3994, 2));
#line 1842 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1843 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1844 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1845 "compiler/main.gem"
    GemVal _t3995[] = {gem_v_captures};
    GemVal _t3996[] = {gem_v_inner_scope, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t3995, 1)};
    *gem_v_local_names = gem_fn__mod_codegen_set_union(NULL, _t3996, 2);
#line 1847 "compiler/main.gem"
    GemVal _t3997[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t3998 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t3998.fn(_t3998.env, _t3997, 2);
#line 1849 "compiler/main.gem"
    GemVal _t3999[] = {gem_v_captures};
    GemVal _t4000[] = {gem_v_inner_captured, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t3999, 1)};
    *gem_v_boxed_vars = gem_fn__mod_codegen_set_union(NULL, _t4000, 2);
#line 1850 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1853 "compiler/main.gem"
    GemVal _t4001[] = {gem_string("_closure_"), gem_v_name};
    GemVal gem_v_struct_name = gem_interp(2, _t4001);
#line 1854 "compiler/main.gem"
    GemVal gem_v_sb = gem_buf_new_fn(NULL, NULL, 0);
#line 1855 "compiler/main.gem"
    GemVal _t4002[] = {gem_string("struct "), gem_v_struct_name, gem_string(" {\n")};
    GemVal _t4003[] = {gem_v_sb, gem_interp(3, _t4002)};
    (void)(gem_buf_push_fn(NULL, _t4003, 2));
#line 1856 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_captures;
#line 1856 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1856 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4004[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t4004, 1)))) break;
#line 1856 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1856 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1857 "compiler/main.gem"
    GemVal _t4005[] = {gem_v_cap};
    GemVal _t4006 = (*gem_v_mangle);
    GemVal _t4007[] = {gem_string("    GemVal *"), _t4006.fn(_t4006.env, _t4005, 1), gem_string(";\n")};
    GemVal _t4008[] = {gem_v_sb, gem_interp(3, _t4007)};
        (void)(gem_buf_push_fn(NULL, _t4008, 2));
    }

#line 1859 "compiler/main.gem"
    GemVal _t4009[] = {gem_v_sb, gem_string("};\n")};
    (void)(gem_buf_push_fn(NULL, _t4009, 2));
#line 1860 "compiler/main.gem"
    GemVal _t4010[] = {gem_v_sb};
    GemVal gem_v_struct_def = gem_buf_str_fn(NULL, _t4010, 1);
#line 1863 "compiler/main.gem"
    GemVal gem_v_fb = gem_buf_new_fn(NULL, NULL, 0);
#line 1864 "compiler/main.gem"
    GemVal _t4011[] = {gem_string("static GemVal "), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t4012[] = {gem_v_fb, gem_interp(3, _t4011)};
    (void)(gem_buf_push_fn(NULL, _t4012, 2));
#line 1865 "compiler/main.gem"
    GemVal _t4013[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4013, 1), gem_int(0)))) {
#line 1866 "compiler/main.gem"
    GemVal _t4014[] = {gem_string("    struct "), gem_v_struct_name, gem_string(" *_cls = (struct "), gem_v_struct_name, gem_string(" *)_env;\n")};
    GemVal _t4015[] = {gem_v_fb, gem_interp(5, _t4014)};
        (void)(gem_buf_push_fn(NULL, _t4015, 2));
#line 1867 "compiler/main.gem"
        GemVal gem_v__for_items_85 = gem_v_captures;
#line 1867 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1867 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4016[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t4016, 1)))) break;
#line 1867 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1867 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1868 "compiler/main.gem"
    GemVal _t4017[] = {gem_v_cap};
    GemVal _t4018 = (*gem_v_mangle);
            GemVal gem_v_mc = _t4018.fn(_t4018.env, _t4017, 1);
#line 1869 "compiler/main.gem"
    GemVal _t4019[] = {gem_string("    GemVal *"), gem_v_mc, gem_string(" = _cls->"), gem_v_mc, gem_string(";\n")};
    GemVal _t4020[] = {gem_v_fb, gem_interp(5, _t4019)};
            (void)(gem_buf_push_fn(NULL, _t4020, 2));
        }

    }
#line 1873 "compiler/main.gem"
    GemVal _t4021[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t4022 = (*gem_v_emit_param_bindings);
    GemVal _t4023[] = {gem_v_fb, _t4022.fn(_t4022.env, _t4021, 3)};
    (void)(gem_buf_push_fn(NULL, _t4023, 2));
#line 1874 "compiler/main.gem"
    GemVal _t4024[] = {gem_v_name};
    GemVal _t4025[] = {(*gem_v_source_name)};
    GemVal _t4026[] = {gem_string("    gem_push_frame(\""), gem_fn__mod_codegen_escape_c_string(NULL, _t4024, 1), gem_string("\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4025, 1), gem_string("\", 0);\n")};
    GemVal _t4027[] = {gem_v_fb, gem_interp(5, _t4026)};
    (void)(gem_buf_push_fn(NULL, _t4027, 2));
#line 1875 "compiler/main.gem"
    GemVal _t4028[] = {gem_v_body};
    GemVal _t4029 = (*gem_v_emit_fn_body);
    GemVal _t4030[] = {gem_v_fb, _t4029.fn(_t4029.env, _t4028, 1)};
    (void)(gem_buf_push_fn(NULL, _t4030, 2));
#line 1876 "compiler/main.gem"
    GemVal _t4031[] = {gem_v_fb, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4031, 2));
#line 1878 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1879 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1880 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1883 "compiler/main.gem"
    GemVal _t4032[] = {gem_v_fb};
    GemVal _t4033[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_buf_str_fn(NULL, _t4032, 1))};
    (void)(gem_push_fn(NULL, _t4033, 2));
    GemVal _t4034 = gem_table_new();
    gem_table_set(_t4034, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t4034, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t4034, gem_string("captures"), gem_v_captures);
    GemVal _t4035 = _t4034;
    gem_pop_frame();
    return _t4035;
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
#line 1892 "compiler/main.gem"
    GemVal _t4037 = gem_table_new();
    GemVal gem_v_fn_defs = _t4037;
#line 1893 "compiler/main.gem"
    GemVal _t4038 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t4038;
#line 1894 "compiler/main.gem"
    GemVal _t4039 = gem_table_new();
    GemVal gem_v_extern_includes = _t4039;
#line 1895 "compiler/main.gem"
    GemVal _t4040 = gem_table_new();
    GemVal gem_v_top_stmts = _t4040;
#line 1897 "compiler/main.gem"
    GemVal _t4041 = gem_v_ast;
    GemVal gem_v__for_items_86 = gem_table_get(_t4041, gem_string("stmts"));
#line 1897 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1897 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4042[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t4042, 1)))) break;
#line 1897 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1897 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1898 "compiler/main.gem"
    GemVal _t4043 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t4043, gem_string("tag")), gem_string("fn_def")))) {
#line 1899 "compiler/main.gem"
    GemVal _t4044[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t4044, 2));
        } else {
#line 1900 "compiler/main.gem"
    GemVal _t4045 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t4045, gem_string("tag")), gem_string("extern_fn")))) {
#line 1901 "compiler/main.gem"
    GemVal _t4046[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t4046, 2));
            } else {
#line 1902 "compiler/main.gem"
    GemVal _t4047 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t4047, gem_string("tag")), gem_string("extern_include")))) {
#line 1903 "compiler/main.gem"
    GemVal _t4048[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t4048, 2));
                } else {
#line 1904 "compiler/main.gem"
    GemVal _t4049 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t4049, gem_string("tag")), gem_string("export")))) {
                    } else {
#line 1907 "compiler/main.gem"
    GemVal _t4050[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t4050, 2));
                    }
                }
            }
        }
    }

#line 1912 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_extern_fns_list;
#line 1912 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1912 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4051[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t4051, 1)))) break;
#line 1912 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1912 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1913 "compiler/main.gem"
    GemVal _t4052 = gem_v_ef;
    GemVal _t4053[] = {(*gem_v_defined_fns), gem_table_get(_t4052, gem_string("name"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4053, 2));
    }

#line 1917 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_fn_defs;
#line 1917 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1917 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4054[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t4054, 1)))) break;
#line 1917 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1917 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1918 "compiler/main.gem"
    GemVal _t4055 = gem_v_fd;
    GemVal _t4056[] = {(*gem_v_defined_fns), gem_table_get(_t4055, gem_string("name"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4056, 2));
#line 1919 "compiler/main.gem"
    GemVal _t4057 = gem_v_fd;
    GemVal _t4058[] = {gem_string("static GemVal gem_fn_"), gem_table_get(_t4057, gem_string("name")), gem_string("(void *_env, GemVal *args, int argc);")};
        GemVal gem_v_decl = gem_interp(3, _t4058);
#line 1920 "compiler/main.gem"
    GemVal _t4059[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t4059, 2));
    }

#line 1924 "compiler/main.gem"
    GemVal _t4060[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4060, 2));
#line 1925 "compiler/main.gem"
    GemVal _t4061 = gem_table_new();
    GemVal _t4062[] = {gem_v_top_stmts, _t4061};
    GemVal _t4063 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t4063.fn(_t4063.env, _t4062, 2);
#line 1928 "compiler/main.gem"
    GemVal gem_v__for_items_89 = gem_v_extern_fns_list;
#line 1928 "compiler/main.gem"
    GemVal gem_v__for_i_89 = gem_int(0);
#line 1928 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4064[] = {gem_v__for_items_89};
        if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t4064, 1)))) break;
#line 1928 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1928 "compiler/main.gem"
        gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1929 "compiler/main.gem"
    GemVal _t4065[] = {gem_v_ef};
    GemVal _t4066 = (*gem_v_compile_extern_fn);
    GemVal _t4067[] = {(*gem_v_functions), _t4066.fn(_t4066.env, _t4065, 1)};
        (void)(gem_push_fn(NULL, _t4067, 2));
    }

#line 1933 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_fn_defs;
#line 1933 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1933 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4068[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t4068, 1)))) break;
#line 1933 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1933 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1934 "compiler/main.gem"
    GemVal _t4069[] = {gem_v_fd};
    GemVal _t4070 = (*gem_v_compile_fn);
    GemVal _t4071[] = {(*gem_v_functions), _t4070.fn(_t4070.env, _t4069, 1)};
        (void)(gem_push_fn(NULL, _t4071, 2));
    }

#line 1938 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1939 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1940 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1941 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1942 "compiler/main.gem"
    GemVal _t4072 = gem_table_new();
    *gem_v_local_names = _t4072;
#line 1943 "compiler/main.gem"
    GemVal gem_v__for_tbl_91 = (*gem_v_top_level_vars);
#line 1943 "compiler/main.gem"
    GemVal _t4073[] = {gem_v__for_tbl_91};
    GemVal gem_v__for_len_91 = gem_len_fn(NULL, _t4073, 1);
#line 1943 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1943 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_v__for_len_91))) break;
#line 1943 "compiler/main.gem"
    GemVal _t4074[] = {gem_v__for_tbl_91, gem_v__for_i_91};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4074, 2);
#line 1943 "compiler/main.gem"
    GemVal _t4075[] = {gem_v__for_tbl_91, gem_v__for_i_91};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4075, 2);
#line 1943 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1944 "compiler/main.gem"
    GemVal _t4076[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4076, 2));
    }

#line 1946 "compiler/main.gem"
    GemVal _t4077[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t4078 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t4078.fn(_t4078.env, _t4077, 2);
#line 1947 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1948 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1949 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1951 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1952 "compiler/main.gem"
    GemVal gem_v__for_items_92 = gem_v_fn_defs;
#line 1952 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1952 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4079[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t4079, 1)))) break;
#line 1952 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1952 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1953 "compiler/main.gem"
    GemVal _t4080 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t4080, gem_string("name")), gem_string("main")))) {
#line 1954 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1958 "compiler/main.gem"
    GemVal gem_v_mb = gem_buf_new_fn(NULL, NULL, 0);
#line 1959 "compiler/main.gem"
    GemVal _t4081[] = {gem_v_mb, gem_string("int main(int argc, char **argv) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4081, 2));
#line 1960 "compiler/main.gem"
    GemVal _t4082[] = {gem_v_mb, gem_string("    GC_INIT();\n")};
    (void)(gem_buf_push_fn(NULL, _t4082, 2));
#line 1961 "compiler/main.gem"
    GemVal _t4083[] = {gem_v_mb, gem_string("    gem_init(argc, argv);\n")};
    (void)(gem_buf_push_fn(NULL, _t4083, 2));
#line 1962 "compiler/main.gem"
    GemVal _t4084[] = {gem_v_mb, gem_string("    gem_scheduler_init();\n")};
    (void)(gem_buf_push_fn(NULL, _t4084, 2));
#line 1963 "compiler/main.gem"
    GemVal _t4085[] = {(*gem_v_source_name)};
    GemVal _t4086[] = {gem_string("    gem_push_frame(\"main\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4085, 1), gem_string("\", 0);\n")};
    GemVal _t4087[] = {gem_v_mb, gem_interp(3, _t4086)};
    (void)(gem_buf_push_fn(NULL, _t4087, 2));
#line 1964 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1965 "compiler/main.gem"
    GemVal _t4088[] = {gem_v_mb, gem_v_main_body};
        (void)(gem_buf_push_fn(NULL, _t4088, 2));
    }
#line 1967 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1968 "compiler/main.gem"
    GemVal _t4089[] = {gem_v_mb, gem_string("    GemVal _margs[1] = {GEM_NIL};\n")};
        (void)(gem_buf_push_fn(NULL, _t4089, 2));
#line 1969 "compiler/main.gem"
    GemVal _t4090[] = {gem_v_mb, gem_string("    gem_fn_main(NULL, _margs, 0);\n")};
        (void)(gem_buf_push_fn(NULL, _t4090, 2));
    }
#line 1971 "compiler/main.gem"
    GemVal _t4091[] = {gem_v_mb, gem_string("    gem_run_scheduler();\n")};
    (void)(gem_buf_push_fn(NULL, _t4091, 2));
#line 1972 "compiler/main.gem"
    GemVal _t4092[] = {gem_v_mb, gem_string("    gem_pop_frame();\n    return 0;\n}\n")};
    (void)(gem_buf_push_fn(NULL, _t4092, 2));
#line 1973 "compiler/main.gem"
    GemVal _t4093[] = {gem_v_mb};
    GemVal gem_v_main_c = gem_buf_str_fn(NULL, _t4093, 1);
#line 1976 "compiler/main.gem"
    GemVal gem_v_ob = gem_buf_new_fn(NULL, NULL, 0);
#line 1977 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_includes;
#line 1977 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 1977 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4094[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t4094, 1)))) break;
#line 1977 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 1977 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 1978 "compiler/main.gem"
    GemVal _t4095 = gem_v_ei;
    GemVal _t4096[] = {gem_string("#include \""), gem_table_get(_t4095, gem_string("path")), gem_string("\"\n")};
    GemVal _t4097[] = {gem_v_ob, gem_interp(3, _t4096)};
        (void)(gem_buf_push_fn(NULL, _t4097, 2));
    }

#line 1980 "compiler/main.gem"
    GemVal _t4098[] = {gem_v_ob, gem_string("#include \"gem.h\"\n")};
    (void)(gem_buf_push_fn(NULL, _t4098, 2));
#line 1984 "compiler/main.gem"
    GemVal _t4099[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t4099, 1), gem_int(0)))) {
#line 1985 "compiler/main.gem"
        GemVal gem_v__for_i_94 = gem_int(0);
#line 1985 "compiler/main.gem"
    GemVal _t4100[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_94 = gem_len_fn(NULL, _t4100, 1);
#line 1985 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_v__for_limit_94))) break;
#line 1985 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_94;
#line 1985 "compiler/main.gem"
            gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 1986 "compiler/main.gem"
    GemVal _t4101[] = {gem_v_ob, gem_table_get((*gem_v_extern_c_decls), gem_v_i)};
            (void)(gem_buf_push_fn(NULL, _t4101, 2));
#line 1987 "compiler/main.gem"
    GemVal _t4102[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4102, 2));
        }

    }
#line 1992 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 1992 "compiler/main.gem"
    GemVal _t4103[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_95 = gem_len_fn(NULL, _t4103, 1);
#line 1992 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_v__for_limit_95))) break;
#line 1992 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_95;
#line 1992 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 1993 "compiler/main.gem"
    GemVal _t4104[] = {gem_v_ob, gem_table_get((*gem_v_forward_decls), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4104, 2));
#line 1994 "compiler/main.gem"
    GemVal _t4105[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4105, 1), gem_int(1))))) {
#line 1995 "compiler/main.gem"
    GemVal _t4106[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4106, 2));
        }
    }

#line 1998 "compiler/main.gem"
    GemVal _t4107[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4107, 2));
#line 2001 "compiler/main.gem"
    GemVal _t4108[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t4108, 1);
#line 2002 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 2002 "compiler/main.gem"
    GemVal _t4109[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_96 = gem_len_fn(NULL, _t4109, 1);
#line 2002 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_v__for_limit_96))) break;
#line 2002 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_96;
#line 2002 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 2003 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 2004 "compiler/main.gem"
    GemVal _t4110[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4110, 2))) {
#line 2005 "compiler/main.gem"
    GemVal _t4111[] = {gem_string("GemVal *gem_v_"), gem_v_vname, gem_string(";\n")};
    GemVal _t4112[] = {gem_v_ob, gem_interp(3, _t4111)};
            (void)(gem_buf_push_fn(NULL, _t4112, 2));
        } else {
#line 2007 "compiler/main.gem"
    GemVal _t4113[] = {gem_string("GemVal gem_v_"), gem_v_vname, gem_string(";\n")};
    GemVal _t4114[] = {gem_v_ob, gem_interp(3, _t4113)};
            (void)(gem_buf_push_fn(NULL, _t4114, 2));
        }
    }

#line 2010 "compiler/main.gem"
    GemVal _t4115[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4115, 2));
#line 2013 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 2013 "compiler/main.gem"
    GemVal _t4116[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_97 = gem_len_fn(NULL, _t4116, 1);
#line 2013 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_limit_97))) break;
#line 2013 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_97;
#line 2013 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 2014 "compiler/main.gem"
    GemVal _t4117[] = {gem_v_ob, gem_table_get((*gem_v_functions), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4117, 2));
#line 2015 "compiler/main.gem"
    GemVal _t4118[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4118, 1), gem_int(1))))) {
#line 2016 "compiler/main.gem"
    GemVal _t4119[] = {gem_v_ob, gem_string("\n\n")};
            (void)(gem_buf_push_fn(NULL, _t4119, 2));
        }
    }

#line 2019 "compiler/main.gem"
    GemVal _t4120[] = {gem_v_ob, gem_string("\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4120, 2));
#line 2021 "compiler/main.gem"
    GemVal _t4121[] = {gem_v_ob, gem_v_main_c};
    (void)(gem_buf_push_fn(NULL, _t4121, 2));
#line 2022 "compiler/main.gem"
    GemVal _t4122[] = {gem_v_ob};
    GemVal _t4123 = gem_buf_str_fn(NULL, _t4122, 1);
    gem_pop_frame();
    return _t4123;
}

static GemVal gem_fn__mod_codegen_make_codegen(void *_env, GemVal *args, int argc) {
#line 125 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_make_codegen", "compiler/main.gem", 125);
#line 126 "compiler/main.gem"
    GemVal _t1824 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1824;
#line 127 "compiler/main.gem"
    GemVal _t1825 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1825;
#line 128 "compiler/main.gem"
    GemVal _t1826 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1826;
#line 129 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 130 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t1827 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1827;
#line 132 "compiler/main.gem"
    GemVal _t1828 = gem_table_new();
    gem_table_set(_t1828, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1828, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1828, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1828, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1828, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1828, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1828, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1828, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1828, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1828, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1828, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1828, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1828, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1828, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1828, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1828, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1828, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1828, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1828, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1828, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1828, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1828, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1828, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1828, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1828, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1828, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1828, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1828, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1828, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1828, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1828, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1828, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1828, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1828, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1828, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1828, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1828, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1828, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1828, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1828, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1828, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1828, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1828, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1828, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1828, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1828, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1828, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1828, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1828, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1828, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1828, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1828, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1828, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1828, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1828, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1828, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1828, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1828, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1828, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1828, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1828, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1828, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1828, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1828, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1828, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1828, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1828, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1828, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1828, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1828, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1828, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    gem_table_set(_t1828, gem_string("exec"), gem_string("gem_exec_fn"));
    gem_table_set(_t1828, gem_string("tcp_connect"), gem_string("gem_tcp_connect_fn"));
    gem_table_set(_t1828, gem_string("tcp_listen"), gem_string("gem_tcp_listen_fn"));
    gem_table_set(_t1828, gem_string("tcp_accept"), gem_string("gem_tcp_accept_fn"));
    gem_table_set(_t1828, gem_string("tcp_read"), gem_string("gem_tcp_read_fn"));
    gem_table_set(_t1828, gem_string("tcp_write"), gem_string("gem_tcp_write_fn"));
    gem_table_set(_t1828, gem_string("tcp_close"), gem_string("gem_tcp_close_fn"));
    gem_table_set(_t1828, gem_string("__table_key_at"), gem_string("gem_table_key_at_fn"));
    gem_table_set(_t1828, gem_string("__table_val_at"), gem_string("gem_table_val_at_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1828;
#line 214 "compiler/main.gem"
    GemVal _t1829 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1829;
#line 215 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 215 "compiler/main.gem"
    GemVal _t1830[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_len_10 = gem_len_fn(NULL, _t1830, 1);
#line 215 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 215 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_len_10))) break;
#line 215 "compiler/main.gem"
    GemVal _t1831[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1831, 2);
#line 215 "compiler/main.gem"
    GemVal _t1832[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1832, 2);
#line 215 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 216 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 218 "compiler/main.gem"
    GemVal _t1833 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1833;
#line 219 "compiler/main.gem"
    GemVal _t1834 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1834;
#line 220 "compiler/main.gem"
    GemVal _t1835 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1835;
#line 221 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 222 "compiler/main.gem"
    GemVal _t1836 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1836;
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
    struct _closure__anon_24 *_t1839 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1839->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1839);
#line 234 "compiler/main.gem"
    struct _closure__anon_25 *_t1842 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1842->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1842);
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
    struct _closure__anon_27 *_t2154 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t2154->gem_v_builtins = gem_v_builtins;
    _t2154->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2154->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2154->gem_v_defined_fns = gem_v_defined_fns;
    _t2154->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t2154);
#line 439 "compiler/main.gem"
    struct _closure__anon_28 *_t2166 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t2166->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t2166);
#line 454 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 455 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 457 "compiler/main.gem"
    struct _closure__anon_29 *_t2354 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2354->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2354->gem_v_walk_captures = gem_v_walk_captures;
    _t2354->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2354);
#line 566 "compiler/main.gem"
    struct _closure__anon_30 *_t2364 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2364->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2364);
#line 577 "compiler/main.gem"
    struct _closure__anon_31 *_t2369 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2369->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2369);
#line 585 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 586 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 587 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 588 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 589 "compiler/main.gem"
    GemVal *gem_v_compile_stmts_tail = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts_tail = GEM_NIL;
#line 590 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 591 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 592 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 593 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 594 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 595 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 596 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 597 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 598 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 599 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 600 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 601 "compiler/main.gem"
    GemVal *gem_v_compile_blocking_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_blocking_extern_fn = GEM_NIL;
#line 602 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 603 "compiler/main.gem"
    GemVal *gem_v_is_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_self_tail_call = GEM_NIL;
#line 604 "compiler/main.gem"
    GemVal *gem_v_has_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_has_self_tail_call = GEM_NIL;
#line 605 "compiler/main.gem"
    GemVal *gem_v_emit_tco_continue = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_tco_continue = GEM_NIL;
#line 609 "compiler/main.gem"
    struct _closure__anon_32 *_t2454 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2454->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t2454->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_is_self_tail_call = gem_make_fn(_anon_32, _t2454);
#line 658 "compiler/main.gem"
    struct _closure__anon_33 *_t2460 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2460->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_has_self_tail_call = gem_make_fn(_anon_33, _t2460);
#line 665 "compiler/main.gem"
    struct _closure__anon_34 *_t2487 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2487->gem_v_compile_expr = gem_v_compile_expr;
    _t2487->gem_v_mangle = gem_v_mangle;
    _t2487->gem_v_tco_captured = gem_v_tco_captured;
    _t2487->gem_v_tco_params = gem_v_tco_params;
    _t2487->gem_v_tmp = gem_v_tmp;
    *gem_v_emit_tco_continue = gem_make_fn(_anon_34, _t2487);
#line 695 "compiler/main.gem"
    struct _closure__anon_35 *_t2679 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2679->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2679->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2679->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2679->gem_v_compile_array = gem_v_compile_array;
    _t2679->gem_v_compile_binop = gem_v_compile_binop;
    _t2679->gem_v_compile_call = gem_v_compile_call;
    _t2679->gem_v_compile_expr = gem_v_compile_expr;
    _t2679->gem_v_compile_table = gem_v_compile_table;
    _t2679->gem_v_defined_fns = gem_v_defined_fns;
    _t2679->gem_v_local_names = gem_v_local_names;
    _t2679->gem_v_mangle = gem_v_mangle;
    _t2679->gem_v_tmp = gem_v_tmp;
    _t2679->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_35, _t2679);
#line 778 "compiler/main.gem"
    struct _closure__anon_36 *_t2698 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2698->gem_v_compile_expr = gem_v_compile_expr;
    _t2698->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_36, _t2698);
#line 794 "compiler/main.gem"
    struct _closure__anon_37 *_t2714 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2714->gem_v_compile_expr = gem_v_compile_expr;
    _t2714->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_37, _t2714);
#line 809 "compiler/main.gem"
    struct _closure__anon_38 *_t2753 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2753->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2753->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2753->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2753->gem_v_mangle = gem_v_mangle;
    _t2753->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_38, _t2753);
#line 841 "compiler/main.gem"
    struct _closure__anon_39 *_t2771 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2771->gem_v_compile_expr = gem_v_compile_expr;
    _t2771->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_39, _t2771);
#line 862 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_40, NULL);
#line 871 "compiler/main.gem"
    struct _closure__anon_41 *_t2850 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2850->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2850->gem_v_compile_args = gem_v_compile_args;
    _t2850->gem_v_compile_expr = gem_v_compile_expr;
    _t2850->gem_v_defined_fns = gem_v_defined_fns;
    _t2850->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2850->gem_v_local_names = gem_v_local_names;
    _t2850->gem_v_source_name = gem_v_source_name;
    _t2850->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_41, _t2850);
#line 915 "compiler/main.gem"
    struct _closure__anon_42 *_t3004 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t3004->gem_v_compile_expr = gem_v_compile_expr;
    _t3004->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_42, _t3004);
#line 996 "compiler/main.gem"
    struct _closure__anon_43 *_t3012 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3012->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_43, _t3012);
#line 1005 "compiler/main.gem"
    struct _closure__anon_44 *_t3031 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3031->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3031->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3031->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_stmts_tail = gem_make_fn(_anon_44, _t3031);
#line 1026 "compiler/main.gem"
    struct _closure__anon_45 *_t3228 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3228->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3228->gem_v_compile_expr = gem_v_compile_expr;
    _t3228->gem_v_compile_if = gem_v_compile_if;
    _t3228->gem_v_compile_match = gem_v_compile_match;
    _t3228->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3228->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3228->gem_v_compile_while = gem_v_compile_while;
    _t3228->gem_v_in_top_level = gem_v_in_top_level;
    _t3228->gem_v_mangle = gem_v_mangle;
    _t3228->gem_v_source_name = gem_v_source_name;
    _t3228->gem_v_tmp = gem_v_tmp;
    _t3228->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3228->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_45, _t3228);
#line 1103 "compiler/main.gem"
    struct _closure__anon_46 *_t3427 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3427->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3427->gem_v_compile_expr = gem_v_compile_expr;
    _t3427->gem_v_compile_if = gem_v_compile_if;
    _t3427->gem_v_compile_match = gem_v_compile_match;
    _t3427->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3427->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3427->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3427->gem_v_compile_while = gem_v_compile_while;
    _t3427->gem_v_emit_tco_continue = gem_v_emit_tco_continue;
    _t3427->gem_v_local_names = gem_v_local_names;
    _t3427->gem_v_mangle = gem_v_mangle;
    _t3427->gem_v_source_name = gem_v_source_name;
    _t3427->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3427->gem_v_tmp = gem_v_tmp;
    _t3427->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_46, _t3427);
#line 1182 "compiler/main.gem"
    struct _closure__anon_47 *_t3458 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3458->gem_v_compile_expr = gem_v_compile_expr;
    _t3458->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    *gem_v_compile_if = gem_make_fn(_anon_47, _t3458);
#line 1202 "compiler/main.gem"
    struct _closure__anon_48 *_t3483 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3483->gem_v_compile_expr = gem_v_compile_expr;
    _t3483->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_48, _t3483);
#line 1219 "compiler/main.gem"
    struct _closure__anon_49 *_t3561 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3561->gem_v_compile_expr = gem_v_compile_expr;
    _t3561->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3561->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3561->gem_v_mangle = gem_v_mangle;
    _t3561->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_49, _t3561);
#line 1275 "compiler/main.gem"
    struct _closure__anon_50 *_t3703 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3703->gem_v_compile_expr = gem_v_compile_expr;
    _t3703->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3703->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3703->gem_v_mangle = gem_v_mangle;
    _t3703->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_50, _t3703);
#line 1395 "compiler/main.gem"
    struct _closure__anon_51 *_t3709 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3709->gem_v_extern_c_decls = gem_v_extern_c_decls;
    GemVal *gem_v_emit_extern_c_decl = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_extern_c_decl = gem_make_fn(_anon_51, _t3709);
#line 1441 "compiler/main.gem"
    struct _closure__anon_52 *_t3777 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3777->gem_v_compile_blocking_extern_fn = gem_v_compile_blocking_extern_fn;
    _t3777->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3777->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_52, _t3777);
#line 1524 "compiler/main.gem"
    struct _closure__anon_53 *_t3890 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3890->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3890->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_blocking_extern_fn = gem_make_fn(_anon_53, _t3890);
#line 1704 "compiler/main.gem"
    struct _closure__anon_54 *_t3920 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t3920->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_54, _t3920);
#line 1735 "compiler/main.gem"
    struct _closure__anon_55 *_t3934 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t3934->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3934->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_55, _t3934);
#line 1752 "compiler/main.gem"
    struct _closure__anon_56 *_t3986 = GC_MALLOC(sizeof(struct _closure__anon_56));
    _t3986->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3986->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3986->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3986->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t3986->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t3986->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t3986->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t3986->gem_v_in_top_level = gem_v_in_top_level;
    _t3986->gem_v_local_names = gem_v_local_names;
    _t3986->gem_v_source_name = gem_v_source_name;
    _t3986->gem_v_tco_captured = gem_v_tco_captured;
    _t3986->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3986->gem_v_tco_params = gem_v_tco_params;
    *gem_v_compile_fn = gem_make_fn(_anon_56, _t3986);
#line 1831 "compiler/main.gem"
    struct _closure__anon_57 *_t4036 = GC_MALLOC(sizeof(struct _closure__anon_57));
    _t4036->gem_v_anon_name = gem_v_anon_name;
    _t4036->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4036->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4036->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4036->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4036->gem_v_functions = gem_v_functions;
    _t4036->gem_v_in_top_level = gem_v_in_top_level;
    _t4036->gem_v_local_names = gem_v_local_names;
    _t4036->gem_v_mangle = gem_v_mangle;
    _t4036->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_57, _t4036);
#line 1890 "compiler/main.gem"
    struct _closure__anon_58 *_t4124 = GC_MALLOC(sizeof(struct _closure__anon_58));
    _t4124->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4124->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t4124->gem_v_compile_fn = gem_v_compile_fn;
    _t4124->gem_v_compile_stmts = gem_v_compile_stmts;
    _t4124->gem_v_defined_fns = gem_v_defined_fns;
    _t4124->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t4124->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4124->gem_v_forward_decls = gem_v_forward_decls;
    _t4124->gem_v_functions = gem_v_functions;
    _t4124->gem_v_in_top_level = gem_v_in_top_level;
    _t4124->gem_v_local_names = gem_v_local_names;
    _t4124->gem_v_source_name = gem_v_source_name;
    _t4124->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t4124->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_58, _t4124);
    GemVal _t4125 = gem_table_new();
    gem_table_set(_t4125, gem_string("compile"), gem_v_compile);
    GemVal _t4126 = _t4125;
    gem_pop_frame();
    return _t4126;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 20 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 20);
    GemVal _t4127[] = {gem_v_node};
    GemVal _t4129;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4127, 1), gem_string("table")))) {
        _t4129 = gem_eq(gem_type_fn(NULL, _t4127, 1), gem_string("table"));
    } else {
        GemVal _t4128 = gem_v_node;
        _t4129 = gem_eq(gem_table_get(_t4128, gem_string("tag")), gem_v_tag);
    }
    GemVal _t4130 = _t4129;
    gem_pop_frame();
    return _t4130;
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_program", "compiler/main.gem", 24);
    GemVal _t4131 = gem_table_new();
    gem_table_set(_t4131, gem_string("tag"), gem_string("program"));
    gem_table_set(_t4131, gem_string("stmts"), gem_v_stmts);
    GemVal _t4132 = _t4131;
    gem_pop_frame();
    return _t4132;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 28);
    GemVal _t4133 = gem_table_new();
    gem_table_set(_t4133, gem_string("tag"), gem_string("let"));
    gem_table_set(_t4133, gem_string("name"), gem_v_name);
    gem_table_set(_t4133, gem_string("value"), gem_v_value);
    gem_table_set(_t4133, gem_string("line"), gem_v_line);
    GemVal _t4134 = _t4133;
    gem_pop_frame();
    return _t4134;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 32);
    GemVal _t4135 = gem_table_new();
    gem_table_set(_t4135, gem_string("tag"), gem_string("var"));
    gem_table_set(_t4135, gem_string("name"), gem_v_name);
    GemVal _t4136 = _t4135;
    gem_pop_frame();
    return _t4136;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 36);
    GemVal _t4137 = gem_table_new();
    gem_table_set(_t4137, gem_string("tag"), gem_string("table"));
    gem_table_set(_t4137, gem_string("entries"), gem_v_entries);
    GemVal _t4138 = _t4137;
    gem_pop_frame();
    return _t4138;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 40);
    GemVal _t4139 = gem_table_new();
    gem_table_set(_t4139, gem_string("key"), gem_v_key);
    gem_table_set(_t4139, gem_string("value"), gem_v_value);
    GemVal _t4140 = _t4139;
    gem_pop_frame();
    return _t4140;
}

static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("module_name_from_path", "compiler/main.gem", 46);
#line 47 "compiler/main.gem"
    GemVal _t4141[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4141, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 49 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 50 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 51 "compiler/main.gem"
    GemVal _t4142[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4142, 2), gem_int(47)))) {
#line 52 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 54 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 56 "compiler/main.gem"
    GemVal _t4143[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4143, 3);
#line 57 "compiler/main.gem"
    GemVal _t4144[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4144, 1);
#line 58 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 59 "compiler/main.gem"
    GemVal _t4145 = gem_v_string;
    GemVal _t4146[] = {gem_v_name, gem_string(".gem")};
    GemVal _t4147 = gem_table_get(_t4145, gem_string("ends_with"));
        if (gem_truthy(_t4147.fn(_t4147.env, _t4146, 2))) {
#line 60 "compiler/main.gem"
    GemVal _t4148[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4148, 3);
        }
    }
    GemVal _t4149 = gem_v_name;
    gem_pop_frame();
    return _t4149;
}

static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("find_export_node", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal _t4150[] = {gem_v_stmts};
    GemVal gem_v_i = gem_sub(gem_len_fn(NULL, _t4150, 1), gem_int(1));
#line 68 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_i, gem_int(0)))) break;
#line 69 "compiler/main.gem"
    GemVal _t4151[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("export")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4151, 2))) {
#line 70 "compiler/main.gem"
            GemVal _t4152 = gem_table_get(gem_v_stmts, gem_v_i);
            gem_pop_frame();
            return _t4152;
        }
#line 72 "compiler/main.gem"
        gem_v_i = gem_sub(gem_v_i, gem_int(1));
    }
    GemVal _t4153 = GEM_NIL;
    gem_pop_frame();
    return _t4153;
}

static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc) {
#line 77 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("collect_module_top_names", "compiler/main.gem", 77);
#line 78 "compiler/main.gem"
    GemVal _t4154 = gem_table_new();
    GemVal gem_v_names = _t4154;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_items_1 = gem_v_stmts;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 79 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4155[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t4155, 1)))) break;
#line 79 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 79 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 80 "compiler/main.gem"
    GemVal _t4156[] = {gem_v_stmt, gem_string("fn_def")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4156, 2))) {
#line 81 "compiler/main.gem"
    GemVal _t4157 = gem_v_string;
    GemVal _t4158 = gem_v_stmt;
    GemVal _t4159[] = {gem_table_get(_t4158, gem_string("name")), gem_string("_mod_")};
    GemVal _t4160 = gem_table_get(_t4157, gem_string("starts_with"));
            if (gem_truthy(gem_not(_t4160.fn(_t4160.env, _t4159, 2)))) {
#line 82 "compiler/main.gem"
    GemVal _t4161 = gem_v_stmt;
                gem_table_set(gem_v_names, gem_table_get(_t4161, gem_string("name")), gem_bool(1));
            }
        } else {
#line 84 "compiler/main.gem"
    GemVal _t4162[] = {gem_v_stmt, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t4162, 2))) {
#line 85 "compiler/main.gem"
    GemVal _t4163 = gem_v_string;
    GemVal _t4164 = gem_v_stmt;
    GemVal _t4165[] = {gem_table_get(_t4164, gem_string("name")), gem_string("_mod_")};
    GemVal _t4166 = gem_table_get(_t4163, gem_string("starts_with"));
                if (gem_truthy(gem_not(_t4166.fn(_t4166.env, _t4165, 2)))) {
#line 86 "compiler/main.gem"
    GemVal _t4167 = gem_v_stmt;
                    gem_table_set(gem_v_names, gem_table_get(_t4167, gem_string("name")), gem_bool(1));
                }
            }
        }
    }

    GemVal _t4168 = gem_v_names;
    gem_pop_frame();
    return _t4168;
}

static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_top_names = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("build_rename_map", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t4169 = gem_table_new();
    GemVal gem_v_rmap = _t4169;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_tbl_2 = gem_v_top_names;
#line 95 "compiler/main.gem"
    GemVal _t4170[] = {gem_v__for_tbl_2};
    GemVal gem_v__for_len_2 = gem_len_fn(NULL, _t4170, 1);
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_len_2))) break;
#line 95 "compiler/main.gem"
    GemVal _t4171[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v_name = gem_table_key_at_fn(NULL, _t4171, 2);
#line 95 "compiler/main.gem"
    GemVal _t4172[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4172, 2);
#line 95 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_rmap, gem_v_name, gem_add(gem_v_prefix, gem_v_name));
    }

    GemVal _t4173 = gem_v_rmap;
    gem_pop_frame();
    return _t4173;
}

static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("copy_set", "compiler/main.gem", 101);
#line 102 "compiler/main.gem"
    GemVal _t4174 = gem_table_new();
    GemVal gem_v_result = _t4174;
#line 103 "compiler/main.gem"
    GemVal gem_v__for_tbl_3 = gem_v_s;
#line 103 "compiler/main.gem"
    GemVal _t4175[] = {gem_v__for_tbl_3};
    GemVal gem_v__for_len_3 = gem_len_fn(NULL, _t4175, 1);
#line 103 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 103 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_len_3))) break;
#line 103 "compiler/main.gem"
    GemVal _t4176[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4176, 2);
#line 103 "compiler/main.gem"
    GemVal _t4177[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_v = gem_table_val_at_fn(NULL, _t4177, 2);
#line 103 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 104 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_v_v);
    }

    GemVal _t4178 = gem_v_result;
    gem_pop_frame();
    return _t4178;
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
            GemVal _t4179 = GEM_NIL;
            gem_pop_frame();
            return _t4179;
        }
#line 113 "compiler/main.gem"
    GemVal _t4180[] = {gem_v_node};
        if (gem_truthy(gem_neq(gem_type_fn(NULL, _t4180, 1), gem_string("table")))) {
#line 114 "compiler/main.gem"
            GemVal _t4181 = GEM_NIL;
            gem_pop_frame();
            return _t4181;
        }
#line 116 "compiler/main.gem"
    GemVal _t4182 = gem_v_node;
        if (gem_truthy(gem_eq(gem_table_get(_t4182, gem_string("tag")), GEM_NIL))) {
#line 117 "compiler/main.gem"
            GemVal _t4183 = GEM_NIL;
            gem_pop_frame();
            return _t4183;
        }
#line 120 "compiler/main.gem"
        GemVal gem_v__match_4 = gem_v_node;
    GemVal _t4184[] = {gem_v__match_4};
    GemVal _t4186;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4184, 1), gem_string("table")))) {
        _t4186 = gem_eq(gem_type_fn(NULL, _t4184, 1), gem_string("table"));
    } else {
        GemVal _t4185[] = {gem_v__match_4, gem_string("tag")};
        _t4186 = gem_has_key_fn(NULL, _t4185, 2);
    }
    GemVal _t4187;
    if (!gem_truthy(_t4186)) {
        _t4187 = _t4186;
    } else {
        _t4187 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("var"));
    }
    GemVal _t4189;
    if (!gem_truthy(_t4187)) {
        _t4189 = _t4187;
    } else {
        GemVal _t4188[] = {gem_v__match_4, gem_string("name")};
        _t4189 = gem_has_key_fn(NULL, _t4188, 2);
    }
        if (gem_truthy(_t4189)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 122 "compiler/main.gem"
    GemVal _t4190[] = {gem_v_rmap, gem_v_name};
    GemVal _t4192;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4190, 2))) {
        _t4192 = gem_has_key_fn(NULL, _t4190, 2);
    } else {
        GemVal _t4191[] = {gem_v_shadowed, gem_v_name};
        _t4192 = gem_not(gem_has_key_fn(NULL, _t4191, 2));
    }
            if (gem_truthy(_t4192)) {
#line 123 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4193[] = {gem_v__match_4};
    GemVal _t4195;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4193, 1), gem_string("table")))) {
        _t4195 = gem_eq(gem_type_fn(NULL, _t4193, 1), gem_string("table"));
    } else {
        GemVal _t4194[] = {gem_v__match_4, gem_string("tag")};
        _t4195 = gem_has_key_fn(NULL, _t4194, 2);
    }
    GemVal _t4196;
    if (!gem_truthy(_t4195)) {
        _t4196 = _t4195;
    } else {
        _t4196 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("fn_def"));
    }
    GemVal _t4198;
    if (!gem_truthy(_t4196)) {
        _t4198 = _t4196;
    } else {
        GemVal _t4197[] = {gem_v__match_4, gem_string("name")};
        _t4198 = gem_has_key_fn(NULL, _t4197, 2);
    }
    GemVal _t4200;
    if (!gem_truthy(_t4198)) {
        _t4200 = _t4198;
    } else {
        GemVal _t4199[] = {gem_v__match_4, gem_string("params")};
        _t4200 = gem_has_key_fn(NULL, _t4199, 2);
    }
    GemVal _t4202;
    if (!gem_truthy(_t4200)) {
        _t4202 = _t4200;
    } else {
        GemVal _t4201[] = {gem_v__match_4, gem_string("body")};
        _t4202 = gem_has_key_fn(NULL, _t4201, 2);
    }
        if (gem_truthy(_t4202)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 126 "compiler/main.gem"
    GemVal _t4203[] = {gem_v_rmap, gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t4203, 2))) {
#line 127 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
            }
#line 129 "compiler/main.gem"
    GemVal _t4204[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4204, 1);
#line 130 "compiler/main.gem"
            GemVal gem_v__for_items_5 = gem_v_params;
#line 130 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4205[] = {gem_v__for_items_5};
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t4205, 1)))) break;
#line 130 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 133 "compiler/main.gem"
    GemVal _t4206 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4206, gem_string("rest_param")), GEM_NIL))) {
#line 134 "compiler/main.gem"
    GemVal _t4207 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4207, gem_string("rest_param")), gem_bool(1));
            }
#line 136 "compiler/main.gem"
    GemVal _t4208[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4209 = gem_fn_rename_stmts(NULL, _t4208, 3);
            gem_pop_frame();
            return _t4209;
        } else {
    GemVal _t4210[] = {gem_v__match_4};
    GemVal _t4212;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4210, 1), gem_string("table")))) {
        _t4212 = gem_eq(gem_type_fn(NULL, _t4210, 1), gem_string("table"));
    } else {
        GemVal _t4211[] = {gem_v__match_4, gem_string("tag")};
        _t4212 = gem_has_key_fn(NULL, _t4211, 2);
    }
    GemVal _t4213;
    if (!gem_truthy(_t4212)) {
        _t4213 = _t4212;
    } else {
        _t4213 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t4215;
    if (!gem_truthy(_t4213)) {
        _t4215 = _t4213;
    } else {
        GemVal _t4214[] = {gem_v__match_4, gem_string("params")};
        _t4215 = gem_has_key_fn(NULL, _t4214, 2);
    }
    GemVal _t4217;
    if (!gem_truthy(_t4215)) {
        _t4217 = _t4215;
    } else {
        GemVal _t4216[] = {gem_v__match_4, gem_string("body")};
        _t4217 = gem_has_key_fn(NULL, _t4216, 2);
    }
        if (gem_truthy(_t4217)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 138 "compiler/main.gem"
    GemVal _t4218[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4218, 1);
#line 139 "compiler/main.gem"
            GemVal gem_v__for_items_6 = gem_v_params;
#line 139 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 139 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4219[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t4219, 1)))) break;
#line 139 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 139 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 140 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 142 "compiler/main.gem"
    GemVal _t4220 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4220, gem_string("rest_param")), GEM_NIL))) {
#line 143 "compiler/main.gem"
    GemVal _t4221 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4221, gem_string("rest_param")), gem_bool(1));
            }
#line 145 "compiler/main.gem"
    GemVal _t4222[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4223 = gem_fn_rename_stmts(NULL, _t4222, 3);
            gem_pop_frame();
            return _t4223;
        } else {
    GemVal _t4224[] = {gem_v__match_4};
    GemVal _t4226;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4224, 1), gem_string("table")))) {
        _t4226 = gem_eq(gem_type_fn(NULL, _t4224, 1), gem_string("table"));
    } else {
        GemVal _t4225[] = {gem_v__match_4, gem_string("tag")};
        _t4226 = gem_has_key_fn(NULL, _t4225, 2);
    }
    GemVal _t4227;
    if (!gem_truthy(_t4226)) {
        _t4227 = _t4226;
    } else {
        _t4227 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("let"));
    }
    GemVal _t4229;
    if (!gem_truthy(_t4227)) {
        _t4229 = _t4227;
    } else {
        GemVal _t4228[] = {gem_v__match_4, gem_string("name")};
        _t4229 = gem_has_key_fn(NULL, _t4228, 2);
    }
    GemVal _t4231;
    if (!gem_truthy(_t4229)) {
        _t4231 = _t4229;
    } else {
        GemVal _t4230[] = {gem_v__match_4, gem_string("value")};
        _t4231 = gem_has_key_fn(NULL, _t4230, 2);
    }
        if (gem_truthy(_t4231)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 147 "compiler/main.gem"
    GemVal _t4232[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4232, 3));
#line 148 "compiler/main.gem"
    GemVal _t4233[] = {gem_v_rmap, gem_v_name};
    GemVal _t4235;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4233, 2))) {
        _t4235 = gem_has_key_fn(NULL, _t4233, 2);
    } else {
        GemVal _t4234[] = {gem_v_shadowed, gem_v_name};
        _t4235 = gem_not(gem_has_key_fn(NULL, _t4234, 2));
    }
            if (gem_truthy(_t4235)) {
#line 149 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4236[] = {gem_v__match_4};
    GemVal _t4238;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4236, 1), gem_string("table")))) {
        _t4238 = gem_eq(gem_type_fn(NULL, _t4236, 1), gem_string("table"));
    } else {
        GemVal _t4237[] = {gem_v__match_4, gem_string("tag")};
        _t4238 = gem_has_key_fn(NULL, _t4237, 2);
    }
    GemVal _t4239;
    if (!gem_truthy(_t4238)) {
        _t4239 = _t4238;
    } else {
        _t4239 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t4241;
    if (!gem_truthy(_t4239)) {
        _t4241 = _t4239;
    } else {
        GemVal _t4240[] = {gem_v__match_4, gem_string("name")};
        _t4241 = gem_has_key_fn(NULL, _t4240, 2);
    }
    GemVal _t4243;
    if (!gem_truthy(_t4241)) {
        _t4243 = _t4241;
    } else {
        GemVal _t4242[] = {gem_v__match_4, gem_string("value")};
        _t4243 = gem_has_key_fn(NULL, _t4242, 2);
    }
        if (gem_truthy(_t4243)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 152 "compiler/main.gem"
    GemVal _t4244[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4244, 3));
#line 153 "compiler/main.gem"
    GemVal _t4245[] = {gem_v_rmap, gem_v_name};
    GemVal _t4247;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4245, 2))) {
        _t4247 = gem_has_key_fn(NULL, _t4245, 2);
    } else {
        GemVal _t4246[] = {gem_v_shadowed, gem_v_name};
        _t4247 = gem_not(gem_has_key_fn(NULL, _t4246, 2));
    }
            if (gem_truthy(_t4247)) {
#line 154 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4248[] = {gem_v__match_4};
    GemVal _t4250;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4248, 1), gem_string("table")))) {
        _t4250 = gem_eq(gem_type_fn(NULL, _t4248, 1), gem_string("table"));
    } else {
        GemVal _t4249[] = {gem_v__match_4, gem_string("tag")};
        _t4250 = gem_has_key_fn(NULL, _t4249, 2);
    }
    GemVal _t4251;
    if (!gem_truthy(_t4250)) {
        _t4251 = _t4250;
    } else {
        _t4251 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("call"));
    }
    GemVal _t4253;
    if (!gem_truthy(_t4251)) {
        _t4253 = _t4251;
    } else {
        GemVal _t4252[] = {gem_v__match_4, gem_string("func")};
        _t4253 = gem_has_key_fn(NULL, _t4252, 2);
    }
    GemVal _t4255;
    if (!gem_truthy(_t4253)) {
        _t4255 = _t4253;
    } else {
        GemVal _t4254[] = {gem_v__match_4, gem_string("args")};
        _t4255 = gem_has_key_fn(NULL, _t4254, 2);
    }
        if (gem_truthy(_t4255)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_func = gem_table_get(gem_v__match_4, gem_string("func"));
#line 0 "compiler/main.gem"
            GemVal gem_v_args = gem_table_get(gem_v__match_4, gem_string("args"));
#line 157 "compiler/main.gem"
    GemVal _t4256[] = {gem_v_func, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4256, 3));
            {
#line 158 "compiler/main.gem"
                GemVal gem_v__for_items_7 = gem_v_args;
#line 158 "compiler/main.gem"
                GemVal gem_v__for_i_7 = gem_int(0);
#line 158 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4257[] = {gem_v__for_items_7};
                    if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t4257, 1)))) break;
#line 158 "compiler/main.gem"
                    GemVal gem_v_a = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 158 "compiler/main.gem"
                    gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 159 "compiler/main.gem"
    GemVal _t4258[] = {gem_v_a, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4258, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4259[] = {gem_v__match_4};
    GemVal _t4261;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4259, 1), gem_string("table")))) {
        _t4261 = gem_eq(gem_type_fn(NULL, _t4259, 1), gem_string("table"));
    } else {
        GemVal _t4260[] = {gem_v__match_4, gem_string("tag")};
        _t4261 = gem_has_key_fn(NULL, _t4260, 2);
    }
    GemVal _t4262;
    if (!gem_truthy(_t4261)) {
        _t4262 = _t4261;
    } else {
        _t4262 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("binop"));
    }
    GemVal _t4264;
    if (!gem_truthy(_t4262)) {
        _t4264 = _t4262;
    } else {
        GemVal _t4263[] = {gem_v__match_4, gem_string("left")};
        _t4264 = gem_has_key_fn(NULL, _t4263, 2);
    }
    GemVal _t4266;
    if (!gem_truthy(_t4264)) {
        _t4266 = _t4264;
    } else {
        GemVal _t4265[] = {gem_v__match_4, gem_string("right")};
        _t4266 = gem_has_key_fn(NULL, _t4265, 2);
    }
        if (gem_truthy(_t4266)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_left = gem_table_get(gem_v__match_4, gem_string("left"));
#line 0 "compiler/main.gem"
            GemVal gem_v_right = gem_table_get(gem_v__match_4, gem_string("right"));
#line 162 "compiler/main.gem"
    GemVal _t4267[] = {gem_v_left, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4267, 3));
#line 163 "compiler/main.gem"
            GemVal _t4268 = gem_v_right;
            GemVal _t4269 = gem_v_rmap;
            GemVal _t4270 = gem_v_shadowed;
            gem_v_node = _t4268;
            gem_v_rmap = _t4269;
            gem_v_shadowed = _t4270;
            continue;
        } else {
    GemVal _t4271[] = {gem_v__match_4};
    GemVal _t4273;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4271, 1), gem_string("table")))) {
        _t4273 = gem_eq(gem_type_fn(NULL, _t4271, 1), gem_string("table"));
    } else {
        GemVal _t4272[] = {gem_v__match_4, gem_string("tag")};
        _t4273 = gem_has_key_fn(NULL, _t4272, 2);
    }
    GemVal _t4274;
    if (!gem_truthy(_t4273)) {
        _t4274 = _t4273;
    } else {
        _t4274 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t4276;
    if (!gem_truthy(_t4274)) {
        _t4276 = _t4274;
    } else {
        GemVal _t4275[] = {gem_v__match_4, gem_string("expr")};
        _t4276 = gem_has_key_fn(NULL, _t4275, 2);
    }
        if (gem_truthy(_t4276)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_expr = gem_table_get(gem_v__match_4, gem_string("expr"));
#line 165 "compiler/main.gem"
            GemVal _t4277 = gem_v_expr;
            GemVal _t4278 = gem_v_rmap;
            GemVal _t4279 = gem_v_shadowed;
            gem_v_node = _t4277;
            gem_v_rmap = _t4278;
            gem_v_shadowed = _t4279;
            continue;
        } else {
    GemVal _t4280[] = {gem_v__match_4};
    GemVal _t4282;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4280, 1), gem_string("table")))) {
        _t4282 = gem_eq(gem_type_fn(NULL, _t4280, 1), gem_string("table"));
    } else {
        GemVal _t4281[] = {gem_v__match_4, gem_string("tag")};
        _t4282 = gem_has_key_fn(NULL, _t4281, 2);
    }
    GemVal _t4283;
    if (!gem_truthy(_t4282)) {
        _t4283 = _t4282;
    } else {
        _t4283 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t4285;
    if (!gem_truthy(_t4283)) {
        _t4285 = _t4283;
    } else {
        GemVal _t4284[] = {gem_v__match_4, gem_string("object")};
        _t4285 = gem_has_key_fn(NULL, _t4284, 2);
    }
        if (gem_truthy(_t4285)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 167 "compiler/main.gem"
            GemVal _t4286 = gem_v_object;
            GemVal _t4287 = gem_v_rmap;
            GemVal _t4288 = gem_v_shadowed;
            gem_v_node = _t4286;
            gem_v_rmap = _t4287;
            gem_v_shadowed = _t4288;
            continue;
        } else {
    GemVal _t4289[] = {gem_v__match_4};
    GemVal _t4291;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4289, 1), gem_string("table")))) {
        _t4291 = gem_eq(gem_type_fn(NULL, _t4289, 1), gem_string("table"));
    } else {
        GemVal _t4290[] = {gem_v__match_4, gem_string("tag")};
        _t4291 = gem_has_key_fn(NULL, _t4290, 2);
    }
    GemVal _t4292;
    if (!gem_truthy(_t4291)) {
        _t4292 = _t4291;
    } else {
        _t4292 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index"));
    }
    GemVal _t4294;
    if (!gem_truthy(_t4292)) {
        _t4294 = _t4292;
    } else {
        GemVal _t4293[] = {gem_v__match_4, gem_string("object")};
        _t4294 = gem_has_key_fn(NULL, _t4293, 2);
    }
    GemVal _t4296;
    if (!gem_truthy(_t4294)) {
        _t4296 = _t4294;
    } else {
        GemVal _t4295[] = {gem_v__match_4, gem_string("key")};
        _t4296 = gem_has_key_fn(NULL, _t4295, 2);
    }
        if (gem_truthy(_t4296)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 169 "compiler/main.gem"
    GemVal _t4297[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4297, 3));
#line 170 "compiler/main.gem"
            GemVal _t4298 = gem_v_key;
            GemVal _t4299 = gem_v_rmap;
            GemVal _t4300 = gem_v_shadowed;
            gem_v_node = _t4298;
            gem_v_rmap = _t4299;
            gem_v_shadowed = _t4300;
            continue;
        } else {
    GemVal _t4301[] = {gem_v__match_4};
    GemVal _t4303;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4301, 1), gem_string("table")))) {
        _t4303 = gem_eq(gem_type_fn(NULL, _t4301, 1), gem_string("table"));
    } else {
        GemVal _t4302[] = {gem_v__match_4, gem_string("tag")};
        _t4303 = gem_has_key_fn(NULL, _t4302, 2);
    }
    GemVal _t4304;
    if (!gem_truthy(_t4303)) {
        _t4304 = _t4303;
    } else {
        _t4304 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t4306;
    if (!gem_truthy(_t4304)) {
        _t4306 = _t4304;
    } else {
        GemVal _t4305[] = {gem_v__match_4, gem_string("object")};
        _t4306 = gem_has_key_fn(NULL, _t4305, 2);
    }
    GemVal _t4308;
    if (!gem_truthy(_t4306)) {
        _t4308 = _t4306;
    } else {
        GemVal _t4307[] = {gem_v__match_4, gem_string("value")};
        _t4308 = gem_has_key_fn(NULL, _t4307, 2);
    }
        if (gem_truthy(_t4308)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 172 "compiler/main.gem"
    GemVal _t4309[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4309, 3));
#line 173 "compiler/main.gem"
            GemVal _t4310 = gem_v_value;
            GemVal _t4311 = gem_v_rmap;
            GemVal _t4312 = gem_v_shadowed;
            gem_v_node = _t4310;
            gem_v_rmap = _t4311;
            gem_v_shadowed = _t4312;
            continue;
        } else {
    GemVal _t4313[] = {gem_v__match_4};
    GemVal _t4315;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4313, 1), gem_string("table")))) {
        _t4315 = gem_eq(gem_type_fn(NULL, _t4313, 1), gem_string("table"));
    } else {
        GemVal _t4314[] = {gem_v__match_4, gem_string("tag")};
        _t4315 = gem_has_key_fn(NULL, _t4314, 2);
    }
    GemVal _t4316;
    if (!gem_truthy(_t4315)) {
        _t4316 = _t4315;
    } else {
        _t4316 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t4318;
    if (!gem_truthy(_t4316)) {
        _t4318 = _t4316;
    } else {
        GemVal _t4317[] = {gem_v__match_4, gem_string("object")};
        _t4318 = gem_has_key_fn(NULL, _t4317, 2);
    }
    GemVal _t4320;
    if (!gem_truthy(_t4318)) {
        _t4320 = _t4318;
    } else {
        GemVal _t4319[] = {gem_v__match_4, gem_string("key")};
        _t4320 = gem_has_key_fn(NULL, _t4319, 2);
    }
    GemVal _t4322;
    if (!gem_truthy(_t4320)) {
        _t4322 = _t4320;
    } else {
        GemVal _t4321[] = {gem_v__match_4, gem_string("value")};
        _t4322 = gem_has_key_fn(NULL, _t4321, 2);
    }
        if (gem_truthy(_t4322)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 175 "compiler/main.gem"
    GemVal _t4323[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4323, 3));
#line 176 "compiler/main.gem"
    GemVal _t4324[] = {gem_v_key, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4324, 3));
#line 177 "compiler/main.gem"
            GemVal _t4325 = gem_v_value;
            GemVal _t4326 = gem_v_rmap;
            GemVal _t4327 = gem_v_shadowed;
            gem_v_node = _t4325;
            gem_v_rmap = _t4326;
            gem_v_shadowed = _t4327;
            continue;
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
        _t4331 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("if"));
    }
    GemVal _t4333;
    if (!gem_truthy(_t4331)) {
        _t4333 = _t4331;
    } else {
        GemVal _t4332[] = {gem_v__match_4, gem_string("cond")};
        _t4333 = gem_has_key_fn(NULL, _t4332, 2);
    }
        if (gem_truthy(_t4333)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 179 "compiler/main.gem"
    GemVal _t4334[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4334, 3));
#line 180 "compiler/main.gem"
    GemVal _t4335 = gem_v_node;
    GemVal _t4336[] = {gem_table_get(_t4335, gem_string("then")), gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_stmts(NULL, _t4336, 3));
#line 181 "compiler/main.gem"
    GemVal _t4337 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4337, gem_string("else")), GEM_NIL))) {
#line 182 "compiler/main.gem"
    GemVal _t4338 = gem_v_node;
    GemVal _t4339[] = {gem_table_get(_t4338, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4340 = gem_fn_rename_stmts(NULL, _t4339, 3);
                gem_pop_frame();
                return _t4340;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4341[] = {gem_v__match_4};
    GemVal _t4343;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4341, 1), gem_string("table")))) {
        _t4343 = gem_eq(gem_type_fn(NULL, _t4341, 1), gem_string("table"));
    } else {
        GemVal _t4342[] = {gem_v__match_4, gem_string("tag")};
        _t4343 = gem_has_key_fn(NULL, _t4342, 2);
    }
    GemVal _t4344;
    if (!gem_truthy(_t4343)) {
        _t4344 = _t4343;
    } else {
        _t4344 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("while"));
    }
    GemVal _t4346;
    if (!gem_truthy(_t4344)) {
        _t4346 = _t4344;
    } else {
        GemVal _t4345[] = {gem_v__match_4, gem_string("cond")};
        _t4346 = gem_has_key_fn(NULL, _t4345, 2);
    }
    GemVal _t4348;
    if (!gem_truthy(_t4346)) {
        _t4348 = _t4346;
    } else {
        GemVal _t4347[] = {gem_v__match_4, gem_string("body")};
        _t4348 = gem_has_key_fn(NULL, _t4347, 2);
    }
        if (gem_truthy(_t4348)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 185 "compiler/main.gem"
    GemVal _t4349[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4349, 3));
#line 186 "compiler/main.gem"
    GemVal _t4350[] = {gem_v_body, gem_v_rmap, gem_v_shadowed};
            GemVal _t4351 = gem_fn_rename_stmts(NULL, _t4350, 3);
            gem_pop_frame();
            return _t4351;
        } else {
    GemVal _t4352[] = {gem_v__match_4};
    GemVal _t4354;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4352, 1), gem_string("table")))) {
        _t4354 = gem_eq(gem_type_fn(NULL, _t4352, 1), gem_string("table"));
    } else {
        GemVal _t4353[] = {gem_v__match_4, gem_string("tag")};
        _t4354 = gem_has_key_fn(NULL, _t4353, 2);
    }
    GemVal _t4355;
    if (!gem_truthy(_t4354)) {
        _t4355 = _t4354;
    } else {
        _t4355 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("match"));
    }
    GemVal _t4357;
    if (!gem_truthy(_t4355)) {
        _t4357 = _t4355;
    } else {
        GemVal _t4356[] = {gem_v__match_4, gem_string("target")};
        _t4357 = gem_has_key_fn(NULL, _t4356, 2);
    }
    GemVal _t4359;
    if (!gem_truthy(_t4357)) {
        _t4359 = _t4357;
    } else {
        GemVal _t4358[] = {gem_v__match_4, gem_string("whens")};
        _t4359 = gem_has_key_fn(NULL, _t4358, 2);
    }
        if (gem_truthy(_t4359)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_target = gem_table_get(gem_v__match_4, gem_string("target"));
#line 0 "compiler/main.gem"
            GemVal gem_v_whens = gem_table_get(gem_v__match_4, gem_string("whens"));
#line 188 "compiler/main.gem"
    GemVal _t4360[] = {gem_v_target, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4360, 3));
#line 189 "compiler/main.gem"
            GemVal gem_v__for_items_8 = gem_v_whens;
#line 189 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 189 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4361[] = {gem_v__for_items_8};
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_len_fn(NULL, _t4361, 1)))) break;
#line 189 "compiler/main.gem"
                GemVal gem_v_w = gem_table_get(gem_v__for_items_8, gem_v__for_i_8);
#line 189 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 190 "compiler/main.gem"
    GemVal _t4362 = gem_v_w;
    GemVal _t4363[] = {gem_table_get(_t4362, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4363, 3));
#line 191 "compiler/main.gem"
    GemVal _t4364 = gem_v_w;
                if (gem_truthy(gem_neq(gem_table_get(_t4364, gem_string("bindings")), GEM_NIL))) {
#line 192 "compiler/main.gem"
    GemVal _t4365 = gem_v_w;
                    GemVal gem_v__for_items_9 = gem_table_get(_t4365, gem_string("bindings"));
#line 192 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 192 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4366[] = {gem_v__for_items_9};
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t4366, 1)))) break;
#line 192 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_9, gem_v__for_i_9);
#line 192 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t4367[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4367, 3));
                    }

                }
#line 196 "compiler/main.gem"
    GemVal _t4368 = gem_v_w;
    GemVal _t4369[] = {gem_table_get(_t4368, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4369, 3));
            }

#line 198 "compiler/main.gem"
    GemVal _t4370 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4370, gem_string("else")), GEM_NIL))) {
#line 199 "compiler/main.gem"
    GemVal _t4371 = gem_v_node;
    GemVal _t4372[] = {gem_table_get(_t4371, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4373 = gem_fn_rename_stmts(NULL, _t4372, 3);
                gem_pop_frame();
                return _t4373;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4374[] = {gem_v__match_4};
    GemVal _t4376;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4374, 1), gem_string("table")))) {
        _t4376 = gem_eq(gem_type_fn(NULL, _t4374, 1), gem_string("table"));
    } else {
        GemVal _t4375[] = {gem_v__match_4, gem_string("tag")};
        _t4376 = gem_has_key_fn(NULL, _t4375, 2);
    }
    GemVal _t4377;
    if (!gem_truthy(_t4376)) {
        _t4377 = _t4376;
    } else {
        _t4377 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t4379;
    if (!gem_truthy(_t4377)) {
        _t4379 = _t4377;
    } else {
        GemVal _t4378[] = {gem_v__match_4, gem_string("arms")};
        _t4379 = gem_has_key_fn(NULL, _t4378, 2);
    }
        if (gem_truthy(_t4379)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_arms = gem_table_get(gem_v__match_4, gem_string("arms"));
#line 202 "compiler/main.gem"
            GemVal gem_v__for_items_10 = gem_v_arms;
#line 202 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 202 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4380[] = {gem_v__for_items_10};
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t4380, 1)))) break;
#line 202 "compiler/main.gem"
                GemVal gem_v_arm = gem_table_get(gem_v__for_items_10, gem_v__for_i_10);
#line 202 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 203 "compiler/main.gem"
    GemVal _t4381 = gem_v_arm;
    GemVal _t4382 = gem_table_get(_t4381, gem_string("pattern"));
    GemVal _t4383[] = {gem_table_get(_t4382, gem_string("condition")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4383, 3));
#line 204 "compiler/main.gem"
    GemVal _t4384 = gem_v_arm;
    GemVal _t4385 = gem_table_get(_t4384, gem_string("pattern"));
                if (gem_truthy(gem_neq(gem_table_get(_t4385, gem_string("bindings")), GEM_NIL))) {
#line 205 "compiler/main.gem"
    GemVal _t4386 = gem_v_arm;
    GemVal _t4387 = gem_table_get(_t4386, gem_string("pattern"));
                    GemVal gem_v__for_items_11 = gem_table_get(_t4387, gem_string("bindings"));
#line 205 "compiler/main.gem"
                    GemVal gem_v__for_i_11 = gem_int(0);
#line 205 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4388[] = {gem_v__for_items_11};
                        if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t4388, 1)))) break;
#line 205 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_11, gem_v__for_i_11);
#line 205 "compiler/main.gem"
                        gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 206 "compiler/main.gem"
    GemVal _t4389[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4389, 3));
                    }

                }
#line 209 "compiler/main.gem"
    GemVal _t4390 = gem_v_arm;
    GemVal _t4391[] = {gem_table_get(_t4390, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4391, 3));
            }

#line 211 "compiler/main.gem"
    GemVal _t4392 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4392, gem_string("after_ms")), GEM_NIL))) {
#line 212 "compiler/main.gem"
    GemVal _t4393 = gem_v_node;
    GemVal _t4394[] = {gem_table_get(_t4393, gem_string("after_ms")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4394, 3));
            }
#line 214 "compiler/main.gem"
    GemVal _t4395 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4395, gem_string("after_body")), GEM_NIL))) {
#line 215 "compiler/main.gem"
    GemVal _t4396 = gem_v_node;
    GemVal _t4397[] = {gem_table_get(_t4396, gem_string("after_body")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4398 = gem_fn_rename_stmts(NULL, _t4397, 3);
                gem_pop_frame();
                return _t4398;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4399[] = {gem_v__match_4};
    GemVal _t4401;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4399, 1), gem_string("table")))) {
        _t4401 = gem_eq(gem_type_fn(NULL, _t4399, 1), gem_string("table"));
    } else {
        GemVal _t4400[] = {gem_v__match_4, gem_string("tag")};
        _t4401 = gem_has_key_fn(NULL, _t4400, 2);
    }
    GemVal _t4402;
    if (!gem_truthy(_t4401)) {
        _t4402 = _t4401;
    } else {
        _t4402 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("return"));
    }
    GemVal _t4404;
    if (!gem_truthy(_t4402)) {
        _t4404 = _t4402;
    } else {
        GemVal _t4403[] = {gem_v__match_4, gem_string("value")};
        _t4404 = gem_has_key_fn(NULL, _t4403, 2);
    }
        if (gem_truthy(_t4404)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 218 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 219 "compiler/main.gem"
                GemVal _t4405 = gem_v_value;
                GemVal _t4406 = gem_v_rmap;
                GemVal _t4407 = gem_v_shadowed;
                gem_v_node = _t4405;
                gem_v_rmap = _t4406;
                gem_v_shadowed = _t4407;
                continue;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4408[] = {gem_v__match_4};
    GemVal _t4410;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4408, 1), gem_string("table")))) {
        _t4410 = gem_eq(gem_type_fn(NULL, _t4408, 1), gem_string("table"));
    } else {
        GemVal _t4409[] = {gem_v__match_4, gem_string("tag")};
        _t4410 = gem_has_key_fn(NULL, _t4409, 2);
    }
    GemVal _t4411;
    if (!gem_truthy(_t4410)) {
        _t4411 = _t4410;
    } else {
        _t4411 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("table"));
    }
    GemVal _t4413;
    if (!gem_truthy(_t4411)) {
        _t4413 = _t4411;
    } else {
        GemVal _t4412[] = {gem_v__match_4, gem_string("entries")};
        _t4413 = gem_has_key_fn(NULL, _t4412, 2);
    }
        if (gem_truthy(_t4413)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_entries = gem_table_get(gem_v__match_4, gem_string("entries"));
            {
#line 222 "compiler/main.gem"
                GemVal gem_v__for_items_12 = gem_v_entries;
#line 222 "compiler/main.gem"
                GemVal gem_v__for_i_12 = gem_int(0);
#line 222 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4414[] = {gem_v__for_items_12};
                    if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t4414, 1)))) break;
#line 222 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 222 "compiler/main.gem"
                    gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 223 "compiler/main.gem"
    GemVal _t4415 = gem_v_e;
    GemVal _t4416[] = {gem_table_get(_t4415, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4416, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4417[] = {gem_v__match_4};
    GemVal _t4419;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4417, 1), gem_string("table")))) {
        _t4419 = gem_eq(gem_type_fn(NULL, _t4417, 1), gem_string("table"));
    } else {
        GemVal _t4418[] = {gem_v__match_4, gem_string("tag")};
        _t4419 = gem_has_key_fn(NULL, _t4418, 2);
    }
    GemVal _t4420;
    if (!gem_truthy(_t4419)) {
        _t4420 = _t4419;
    } else {
        _t4420 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("array"));
    }
    GemVal _t4422;
    if (!gem_truthy(_t4420)) {
        _t4422 = _t4420;
    } else {
        GemVal _t4421[] = {gem_v__match_4, gem_string("elements")};
        _t4422 = gem_has_key_fn(NULL, _t4421, 2);
    }
        if (gem_truthy(_t4422)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_elements = gem_table_get(gem_v__match_4, gem_string("elements"));
            {
#line 226 "compiler/main.gem"
                GemVal gem_v__for_items_13 = gem_v_elements;
#line 226 "compiler/main.gem"
                GemVal gem_v__for_i_13 = gem_int(0);
#line 226 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4423[] = {gem_v__for_items_13};
                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_len_fn(NULL, _t4423, 1)))) break;
#line 226 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_13, gem_v__for_i_13);
#line 226 "compiler/main.gem"
                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 227 "compiler/main.gem"
    GemVal _t4424[] = {gem_v_e, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4424, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4425[] = {gem_v__match_4};
    GemVal _t4427;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4425, 1), gem_string("table")))) {
        _t4427 = gem_eq(gem_type_fn(NULL, _t4425, 1), gem_string("table"));
    } else {
        GemVal _t4426[] = {gem_v__match_4, gem_string("tag")};
        _t4427 = gem_has_key_fn(NULL, _t4426, 2);
    }
    GemVal _t4428;
    if (!gem_truthy(_t4427)) {
        _t4428 = _t4427;
    } else {
        _t4428 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t4430;
    if (!gem_truthy(_t4428)) {
        _t4430 = _t4428;
    } else {
        GemVal _t4429[] = {gem_v__match_4, gem_string("parts")};
        _t4430 = gem_has_key_fn(NULL, _t4429, 2);
    }
        if (gem_truthy(_t4430)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_parts = gem_table_get(gem_v__match_4, gem_string("parts"));
            {
#line 230 "compiler/main.gem"
                GemVal gem_v__for_items_14 = gem_v_parts;
#line 230 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 230 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4431[] = {gem_v__for_items_14};
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_len_fn(NULL, _t4431, 1)))) break;
#line 230 "compiler/main.gem"
                    GemVal gem_v_p = gem_table_get(gem_v__for_items_14, gem_v__for_i_14);
#line 230 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 231 "compiler/main.gem"
    GemVal _t4432[] = {gem_v_p, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4432, 3));
                }
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
        _t4436 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("block"));
    }
    GemVal _t4438;
    if (!gem_truthy(_t4436)) {
        _t4438 = _t4436;
    } else {
        GemVal _t4437[] = {gem_v__match_4, gem_string("stmts")};
        _t4438 = gem_has_key_fn(NULL, _t4437, 2);
    }
        if (gem_truthy(_t4438)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_stmts = gem_table_get(gem_v__match_4, gem_string("stmts"));
#line 234 "compiler/main.gem"
    GemVal _t4439[] = {gem_v_stmts, gem_v_rmap, gem_v_shadowed};
            GemVal _t4440 = gem_fn_rename_stmts(NULL, _t4439, 3);
            gem_pop_frame();
            return _t4440;
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
#line 238 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rmap = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_shadowed = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("rename_stmts", "compiler/main.gem", 238);
#line 239 "compiler/main.gem"
    GemVal _t4441[] = {gem_v_shadowed};
    GemVal gem_v_current = gem_fn_copy_set(NULL, _t4441, 1);
    {
#line 240 "compiler/main.gem"
        GemVal gem_v__for_items_15 = gem_v_stmts;
#line 240 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 240 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4442[] = {gem_v__for_items_15};
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_len_fn(NULL, _t4442, 1)))) break;
#line 240 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_15, gem_v__for_i_15);
#line 240 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 241 "compiler/main.gem"
    GemVal _t4443[] = {gem_v_stmt, gem_v_rmap, gem_v_current};
            (void)(gem_fn_rename_node(NULL, _t4443, 3));
#line 242 "compiler/main.gem"
    GemVal _t4444[] = {gem_v_stmt, gem_string("let")};
    GemVal _t4447;
    if (!gem_truthy(gem_fn_is_node(NULL, _t4444, 2))) {
        _t4447 = gem_fn_is_node(NULL, _t4444, 2);
    } else {
        GemVal _t4445 = gem_v_stmt;
        GemVal _t4446[] = {gem_v_rmap, gem_table_get(_t4445, gem_string("name"))};
        _t4447 = gem_not(gem_has_key_fn(NULL, _t4446, 2));
    }
            if (gem_truthy(_t4447)) {
#line 243 "compiler/main.gem"
    GemVal _t4448 = gem_v_stmt;
                gem_table_set(gem_v_current, gem_table_get(_t4448, gem_string("name")), gem_bool(1));
            }
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn_transform_module(void *_env, GemVal *args, int argc) {
#line 248 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mod_name = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_export_node = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_load_node = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("transform_module", "compiler/main.gem", 248);
#line 249 "compiler/main.gem"
    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_v_mod_name), gem_string("_"));
#line 250 "compiler/main.gem"
    GemVal _t4449[] = {gem_v_stmts};
    GemVal gem_v_top_names = gem_fn_collect_module_top_names(NULL, _t4449, 1);
#line 251 "compiler/main.gem"
    GemVal _t4450[] = {gem_v_top_names, gem_v_prefix};
    GemVal gem_v_rmap = gem_fn_build_rename_map(NULL, _t4450, 2);
#line 253 "compiler/main.gem"
    GemVal _t4451 = gem_table_new();
    GemVal _t4452[] = {gem_v_stmts, gem_v_rmap, _t4451};
    (void)(gem_fn_rename_stmts(NULL, _t4452, 3));
#line 255 "compiler/main.gem"
    GemVal _t4453 = gem_table_new();
    GemVal gem_v_result = _t4453;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_items_16 = gem_v_stmts;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_i_16 = gem_int(0);
#line 256 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4454[] = {gem_v__for_items_16};
        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t4454, 1)))) break;
#line 256 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_16, gem_v__for_i_16);
#line 256 "compiler/main.gem"
        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 257 "compiler/main.gem"
    GemVal _t4455[] = {gem_v_stmt, gem_string("export")};
        if (gem_truthy(gem_not(gem_fn_is_node(NULL, _t4455, 2)))) {
#line 258 "compiler/main.gem"
    GemVal _t4456[] = {gem_v_result, gem_v_stmt};
            (void)(gem_push_fn(NULL, _t4456, 2));
        }
    }

#line 262 "compiler/main.gem"
    GemVal _t4457 = gem_v_export_node;
    GemVal gem_v_exported = gem_table_get(_t4457, gem_string("names"));
#line 264 "compiler/main.gem"
    GemVal _t4458 = gem_v_load_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4458, gem_string("selective")), GEM_NIL))) {
#line 265 "compiler/main.gem"
    GemVal _t4459 = gem_v_load_node;
        GemVal gem_v__for_items_17 = gem_table_get(_t4459, gem_string("selective"));
#line 265 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 265 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4460[] = {gem_v__for_items_17};
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_len_fn(NULL, _t4460, 1)))) break;
#line 265 "compiler/main.gem"
            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_17, gem_v__for_i_17);
#line 265 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 266 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_sel_name);
#line 267 "compiler/main.gem"
    GemVal _t4461[] = {gem_v_prefixed};
    GemVal _t4462[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4461, 1), gem_int(0)};
    GemVal _t4463[] = {gem_v_result, gem_fn_make_let(NULL, _t4462, 3)};
            (void)(gem_push_fn(NULL, _t4463, 2));
        }

    } else {
#line 270 "compiler/main.gem"
        GemVal gem_v_binding_name = gem_v_mod_name;
#line 271 "compiler/main.gem"
    GemVal _t4464 = gem_v_load_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4464, gem_string("alias")), GEM_NIL))) {
#line 272 "compiler/main.gem"
    GemVal _t4465 = gem_v_load_node;
            gem_v_binding_name = gem_table_get(_t4465, gem_string("alias"));
        }
#line 274 "compiler/main.gem"
    GemVal _t4466 = gem_table_new();
        GemVal gem_v_entries = _t4466;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_items_18 = gem_v_exported;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 275 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4467[] = {gem_v__for_items_18};
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_len_fn(NULL, _t4467, 1)))) break;
#line 275 "compiler/main.gem"
            GemVal gem_v_ename = gem_table_get(gem_v__for_items_18, gem_v__for_i_18);
#line 275 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 276 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_ename);
#line 277 "compiler/main.gem"
    GemVal _t4468[] = {gem_v_prefixed};
    GemVal _t4469[] = {gem_v_ename, gem_fn_make_var(NULL, _t4468, 1)};
    GemVal _t4470[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4469, 2)};
            (void)(gem_push_fn(NULL, _t4470, 2));
        }

#line 279 "compiler/main.gem"
    GemVal _t4471[] = {gem_v_entries};
    GemVal _t4472[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4471, 1), gem_int(0)};
    GemVal _t4473[] = {gem_v_result, gem_fn_make_let(NULL, _t4472, 3)};
        (void)(gem_push_fn(NULL, _t4473, 2));
    }
    GemVal _t4474 = gem_v_result;
    gem_pop_frame();
    return _t4474;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 287 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 287);
#line 288 "compiler/main.gem"
    GemVal _t4475 = gem_table_new();
    GemVal gem_v_new_stmts = _t4475;
#line 289 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 290 "compiler/main.gem"
    GemVal _t4476 = gem_v_ast;
    GemVal gem_v__for_items_19 = gem_table_get(_t4476, gem_string("stmts"));
#line 290 "compiler/main.gem"
    GemVal gem_v__for_i_19 = gem_int(0);
#line 290 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4477[] = {gem_v__for_items_19};
        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t4477, 1)))) break;
#line 290 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
#line 290 "compiler/main.gem"
        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 291 "compiler/main.gem"
    GemVal _t4478[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4478, 2))) {
#line 292 "compiler/main.gem"
    GemVal _t4479 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t4479, gem_string("path"));
#line 293 "compiler/main.gem"
    GemVal _t4480 = gem_v_string;
    GemVal _t4481[] = {gem_v_path, gem_string(".gem")};
    GemVal _t4482 = gem_table_get(_t4480, gem_string("ends_with"));
            if (gem_truthy(gem_not(_t4482.fn(_t4482.env, _t4481, 2)))) {
#line 294 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 297 "compiler/main.gem"
    GemVal _t4483[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t4483, 2);
#line 298 "compiler/main.gem"
    GemVal _t4484[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t4484, 1)))) {
#line 299 "compiler/main.gem"
    GemVal _t4485[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t4485, 2);
            }
#line 302 "compiler/main.gem"
    GemVal _t4486[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t4486, 1);
#line 304 "compiler/main.gem"
            GemVal gem_v_cached = gem_table_get(gem_v_loaded, gem_v_full_path);
#line 305 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_cached, GEM_NIL))) {
#line 306 "compiler/main.gem"
    GemVal _t4487 = gem_v_cached;
                if (gem_truthy(gem_neq(gem_table_get(_t4487, gem_string("exported")), GEM_NIL))) {
#line 307 "compiler/main.gem"
    GemVal _t4488 = gem_v_cached;
                    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_table_get(_t4488, gem_string("mod_name"))), gem_string("_"));
#line 308 "compiler/main.gem"
    GemVal _t4489 = gem_v_stmt;
                    if (gem_truthy(gem_neq(gem_table_get(_t4489, gem_string("selective")), GEM_NIL))) {
#line 309 "compiler/main.gem"
    GemVal _t4490 = gem_v_stmt;
                        GemVal gem_v__for_items_20 = gem_table_get(_t4490, gem_string("selective"));
#line 309 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(0);
#line 309 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4491[] = {gem_v__for_items_20};
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t4491, 1)))) break;
#line 309 "compiler/main.gem"
                            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_20, gem_v__for_i_20);
#line 309 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 310 "compiler/main.gem"
    GemVal _t4492[] = {gem_add(gem_v_prefix, gem_v_sel_name)};
    GemVal _t4493[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4492, 1), gem_int(0)};
                            gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4493, 3));
#line 311 "compiler/main.gem"
                            gem_v_count = gem_add(gem_v_count, gem_int(1));
                        }

                    } else {
#line 314 "compiler/main.gem"
    GemVal _t4494 = gem_v_cached;
                        GemVal gem_v_binding_name = gem_table_get(_t4494, gem_string("mod_name"));
#line 315 "compiler/main.gem"
    GemVal _t4495 = gem_v_stmt;
                        if (gem_truthy(gem_neq(gem_table_get(_t4495, gem_string("alias")), GEM_NIL))) {
#line 316 "compiler/main.gem"
    GemVal _t4496 = gem_v_stmt;
                            gem_v_binding_name = gem_table_get(_t4496, gem_string("alias"));
                        }
#line 318 "compiler/main.gem"
    GemVal _t4497 = gem_table_new();
                        GemVal gem_v_entries = _t4497;
#line 319 "compiler/main.gem"
    GemVal _t4498 = gem_v_cached;
                        GemVal gem_v__for_items_21 = gem_table_get(_t4498, gem_string("exported"));
#line 319 "compiler/main.gem"
                        GemVal gem_v__for_i_21 = gem_int(0);
#line 319 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4499[] = {gem_v__for_items_21};
                            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t4499, 1)))) break;
#line 319 "compiler/main.gem"
                            GemVal gem_v_ename = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 319 "compiler/main.gem"
                            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 320 "compiler/main.gem"
    GemVal _t4500[] = {gem_add(gem_v_prefix, gem_v_ename)};
    GemVal _t4501[] = {gem_v_ename, gem_fn_make_var(NULL, _t4500, 1)};
    GemVal _t4502[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4501, 2)};
                            (void)(gem_push_fn(NULL, _t4502, 2));
                        }

#line 322 "compiler/main.gem"
    GemVal _t4503[] = {gem_v_entries};
    GemVal _t4504[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4503, 1), gem_int(0)};
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4504, 3));
#line 323 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
                continue;
            }
#line 329 "compiler/main.gem"
    GemVal _t4505 = gem_table_new();
    gem_table_set(_t4505, gem_string("exported"), GEM_NIL);
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4505);
#line 331 "compiler/main.gem"
    GemVal _t4506[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t4506, 1);
#line 332 "compiler/main.gem"
    GemVal _t4507[] = {gem_v_source, gem_v_full_path};
    GemVal _t4508 = gem_v_parse_source;
            GemVal gem_v_loaded_ast = _t4508.fn(_t4508.env, _t4507, 2);
#line 333 "compiler/main.gem"
    GemVal _t4509[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t4509, 1);
#line 334 "compiler/main.gem"
    GemVal _t4510[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t4510, 3);
#line 336 "compiler/main.gem"
    GemVal _t4511 = gem_v_resolved;
    GemVal _t4512[] = {gem_table_get(_t4511, gem_string("stmts"))};
            GemVal gem_v_export_node = gem_fn_find_export_node(NULL, _t4512, 1);
#line 337 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_export_node, GEM_NIL))) {
#line 338 "compiler/main.gem"
    GemVal _t4513[] = {gem_add(gem_string("gem: loaded file has no export statement: "), gem_v_full_path)};
                (void)(gem_error_at_fn("compiler/main.gem", 338, _t4513, 1));
            }
#line 341 "compiler/main.gem"
    GemVal _t4514 = gem_v_stmt;
    GemVal _t4515[] = {gem_table_get(_t4514, gem_string("path"))};
            GemVal gem_v_mod_name = gem_fn_module_name_from_path(NULL, _t4515, 1);
#line 342 "compiler/main.gem"
    GemVal _t4516 = gem_table_new();
    gem_table_set(_t4516, gem_string("mod_name"), gem_v_mod_name);
    GemVal _t4517 = gem_v_export_node;
    gem_table_set(_t4516, gem_string("exported"), gem_table_get(_t4517, gem_string("names")));
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4516);
#line 343 "compiler/main.gem"
    GemVal _t4518 = gem_v_resolved;
    GemVal _t4519[] = {gem_table_get(_t4518, gem_string("stmts")), gem_v_mod_name, gem_v_export_node, gem_v_stmt};
            GemVal gem_v_mod_stmts = gem_fn_transform_module(NULL, _t4519, 4);
#line 344 "compiler/main.gem"
            GemVal gem_v__for_items_22 = gem_v_mod_stmts;
#line 344 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 344 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4520[] = {gem_v__for_items_22};
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t4520, 1)))) break;
#line 344 "compiler/main.gem"
                GemVal gem_v_ms = gem_table_get(gem_v__for_items_22, gem_v__for_i_22);
#line 344 "compiler/main.gem"
                gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 345 "compiler/main.gem"
                gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_ms);
#line 346 "compiler/main.gem"
                gem_v_count = gem_add(gem_v_count, gem_int(1));
            }

        } else {
#line 349 "compiler/main.gem"
            gem_table_set(gem_v_new_stmts, gem_v_count, gem_v_stmt);
#line 350 "compiler/main.gem"
            gem_v_count = gem_add(gem_v_count, gem_int(1));
        }
    }

#line 353 "compiler/main.gem"
    GemVal _t4521[] = {gem_v_new_stmts};
    GemVal _t4522 = gem_fn_make_program(NULL, _t4521, 1);
    gem_pop_frame();
    return _t4522;
}

static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc) {
#line 358 "compiler/main.gem"
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_args", "compiler/main.gem", 358);
#line 359 "compiler/main.gem"
    GemVal _t4523 = gem_table_new();
    gem_table_set(_t4523, gem_string("src_path"), GEM_NIL);
    gem_table_set(_t4523, gem_string("out_name"), GEM_NIL);
    gem_table_set(_t4523, gem_string("emit_c"), gem_bool(0));
    gem_table_set(_t4523, gem_string("run"), gem_bool(0));
    GemVal _t4524 = gem_table_new();
    gem_table_set(_t4523, gem_string("run_args"), _t4524);
    GemVal gem_v_result = _t4523;
#line 367 "compiler/main.gem"
    GemVal _t4525[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t4525, 1);
#line 368 "compiler/main.gem"
    GemVal gem_v_i = gem_int(1);
#line 369 "compiler/main.gem"
    GemVal gem_v_run_arg_count = gem_int(0);
#line 370 "compiler/main.gem"
    GemVal gem_v_found_run = gem_bool(0);
#line 372 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_argc))) break;
#line 373 "compiler/main.gem"
        GemVal gem_v_arg = gem_table_get(gem_v_args, gem_v_i);
#line 374 "compiler/main.gem"
        if (gem_truthy(gem_v_found_run)) {
#line 375 "compiler/main.gem"
    GemVal _t4526 = gem_v_result;
            gem_table_set(gem_table_get(_t4526, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
#line 376 "compiler/main.gem"
            gem_v_run_arg_count = gem_add(gem_v_run_arg_count, gem_int(1));
        } else {
#line 377 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_arg, gem_string("--emit-c")))) {
#line 378 "compiler/main.gem"
                gem_table_set(gem_v_result, gem_string("emit_c"), gem_bool(1));
            } else {
#line 379 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_arg, gem_string("--run")))) {
#line 380 "compiler/main.gem"
                    gem_table_set(gem_v_result, gem_string("run"), gem_bool(1));
#line 381 "compiler/main.gem"
                    gem_v_found_run = gem_bool(1);
                } else {
#line 382 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_arg, gem_string("-o")))) {
#line 383 "compiler/main.gem"
                        gem_v_i = gem_add(gem_v_i, gem_int(1));
#line 384 "compiler/main.gem"
                        if (gem_truthy(gem_ge(gem_v_i, gem_v_argc))) {
#line 385 "compiler/main.gem"
    GemVal _t4527[] = {gem_string("gem: -o requires an argument")};
                            (void)(gem_error_at_fn("compiler/main.gem", 385, _t4527, 1));
                        }
#line 387 "compiler/main.gem"
                        gem_table_set(gem_v_result, gem_string("out_name"), gem_table_get(gem_v_args, gem_v_i));
                    } else {
#line 388 "compiler/main.gem"
    GemVal _t4528 = gem_v_result;
                        if (gem_truthy(gem_eq(gem_table_get(_t4528, gem_string("src_path")), GEM_NIL))) {
#line 389 "compiler/main.gem"
                            gem_table_set(gem_v_result, gem_string("src_path"), gem_v_arg);
                        } else {
#line 392 "compiler/main.gem"
    GemVal _t4529 = gem_v_result;
                            gem_table_set(gem_table_get(_t4529, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
#line 393 "compiler/main.gem"
                            gem_v_run_arg_count = gem_add(gem_v_run_arg_count, gem_int(1));
                        }
                    }
                }
            }
        }
#line 395 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t4530 = gem_v_result;
    gem_pop_frame();
    return _t4530;
}

static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc) {
#line 403 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("basename_no_ext", "compiler/main.gem", 403);
#line 405 "compiler/main.gem"
    GemVal _t4531[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4531, 1);
#line 406 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 407 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 408 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 409 "compiler/main.gem"
    GemVal _t4532[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4532, 2), gem_int(47)))) {
#line 410 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 412 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 414 "compiler/main.gem"
    GemVal _t4533[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4533, 3);
#line 416 "compiler/main.gem"
    GemVal _t4534[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4534, 1);
#line 417 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 418 "compiler/main.gem"
    GemVal _t4535[] = {gem_v_name, gem_sub(gem_v_nlen, gem_int(4)), gem_int(4)};
        GemVal gem_v_maybe_ext = gem_substr_fn(NULL, _t4535, 3);
#line 419 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_maybe_ext, gem_string(".gem")))) {
#line 420 "compiler/main.gem"
    GemVal _t4536[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4536, 3);
        }
    }
    GemVal _t4537 = gem_v_name;
    gem_pop_frame();
    return _t4537;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_push_frame("main", "compiler/main.gem", 0);
#line 0 "compiler/main.gem"
    GemVal _t4538 = gem_table_new();
    gem_table_set(_t4538, gem_string("split"), gem_make_fn(gem_fn__mod_string_split, NULL));
    gem_table_set(_t4538, gem_string("index_of"), gem_make_fn(gem_fn__mod_string_index_of, NULL));
    gem_table_set(_t4538, gem_string("join"), gem_make_fn(gem_fn__mod_string_join, NULL));
    gem_table_set(_t4538, gem_string("trim"), gem_make_fn(gem_fn__mod_string_trim, NULL));
    gem_table_set(_t4538, gem_string("starts_with"), gem_make_fn(gem_fn__mod_string_starts_with, NULL));
    gem_table_set(_t4538, gem_string("ends_with"), gem_make_fn(gem_fn__mod_string_ends_with, NULL));
    gem_table_set(_t4538, gem_string("upper"), gem_make_fn(gem_fn__mod_string_upper, NULL));
    gem_table_set(_t4538, gem_string("lower"), gem_make_fn(gem_fn__mod_string_lower, NULL));
    gem_table_set(_t4538, gem_string("contains"), gem_make_fn(gem_fn__mod_string_contains, NULL));
    gem_table_set(_t4538, gem_string("repeat"), gem_make_fn(gem_fn__mod_string_repeat, NULL));
    gem_v_string = _t4538;
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
    GemVal _t4539[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t4540[] = {gem_dirname_fn(NULL, _t4539, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t4540, 1);
#line 428 "compiler/main.gem"
    GemVal _t4541[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t4541, 1);
#line 429 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 430 "compiler/main.gem"
    GemVal _t4542[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 430, _t4542, 1));
    }
#line 433 "compiler/main.gem"
    GemVal _t4543[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_cli = gem_fn_parse_args(NULL, _t4543, 1);
#line 435 "compiler/main.gem"
    GemVal _t4544 = gem_v_cli;
    if (gem_truthy(gem_eq(gem_table_get(_t4544, gem_string("src_path")), GEM_NIL))) {
#line 436 "compiler/main.gem"
    GemVal _t4545[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 436, _t4545, 1));
    }
#line 439 "compiler/main.gem"
    GemVal _t4546 = gem_v_cli;
    gem_v_src_path = gem_table_get(_t4546, gem_string("src_path"));
#line 440 "compiler/main.gem"
    GemVal _t4547[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t4547, 1);
#line 442 "compiler/main.gem"
    GemVal _t4548[] = {gem_v_source, gem_v_src_path};
    GemVal _t4549 = gem_v_parse_source;
    gem_v_ast = _t4549.fn(_t4549.env, _t4548, 2);
#line 443 "compiler/main.gem"
    GemVal _t4550[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t4550, 1);
#line 444 "compiler/main.gem"
    GemVal _t4551 = gem_table_new();
    gem_v_loaded = _t4551;
#line 445 "compiler/main.gem"
    GemVal _t4552[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t4552, 3);
#line 447 "compiler/main.gem"
    GemVal _t4553[] = {gem_v_src_path};
    GemVal _t4554 = gem_v_make_codegen;
    gem_v_cg = _t4554.fn(_t4554.env, _t4553, 1);
#line 448 "compiler/main.gem"
    GemVal _t4555 = gem_v_cg;
    GemVal _t4556[] = {gem_v_resolved_ast};
    GemVal _t4557 = gem_table_get(_t4555, gem_string("compile"));
    gem_v_c_code = _t4557.fn(_t4557.env, _t4556, 1);
#line 452 "compiler/main.gem"
    GemVal _t4558 = gem_v_cli;
    if (gem_truthy(gem_table_get(_t4558, gem_string("emit_c")))) {
#line 453 "compiler/main.gem"
    GemVal _t4559[] = {gem_v_c_code};
        (void)(gem_print(NULL, _t4559, 1));
    } else {
#line 457 "compiler/main.gem"
    GemVal _t4560[] = {gem_v_src_path};
        GemVal gem_v_base = gem_fn_basename_no_ext(NULL, _t4560, 1);
#line 458 "compiler/main.gem"
        GemVal gem_v_tmp_c = gem_add(gem_add(gem_string("/tmp/gem_"), gem_v_base), gem_string(".c"));
#line 461 "compiler/main.gem"
    GemVal _t4561 = gem_v_cli;
        GemVal gem_v_out_name = gem_table_get(_t4561, gem_string("out_name"));
#line 462 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_out_name, GEM_NIL))) {
#line 463 "compiler/main.gem"
            gem_v_out_name = gem_add(gem_string("./"), gem_v_base);
        }
#line 467 "compiler/main.gem"
    GemVal _t4562[] = {gem_v_tmp_c, gem_v_c_code};
        (void)(gem_write_file_fn(NULL, _t4562, 2));
#line 470 "compiler/main.gem"
    GemVal _t4563[] = {gem_v_install_root, gem_string("runtime")};
        GemVal gem_v_runtime_dir = gem_path_join_fn(NULL, _t4563, 2);
#line 473 "compiler/main.gem"
        GemVal gem_v_gc_flags = gem_string("-lgc");
#line 474 "compiler/main.gem"
    GemVal _t4564[] = {gem_string("pkg-config --cflags --libs bdw-gc > /tmp/gem_gc_flags.txt 2>/dev/null")};
        GemVal gem_v_pkg_ret = gem_exec_fn(NULL, _t4564, 1);
#line 475 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_pkg_ret, gem_int(0)))) {
#line 476 "compiler/main.gem"
    GemVal _t4565[] = {gem_string("/tmp/gem_gc_flags.txt")};
            gem_v_gc_flags = gem_read_file_fn(NULL, _t4565, 1);
#line 478 "compiler/main.gem"
    GemVal _t4566[] = {gem_v_gc_flags, gem_string("\n"), gem_string("")};
            gem_v_gc_flags = gem_str_replace_fn(NULL, _t4566, 3);
        }
#line 481 "compiler/main.gem"
        GemVal gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" ")), gem_v_runtime_dir), gem_string("/gem_*.c -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread"));
#line 483 "compiler/main.gem"
    GemVal _t4567[] = {gem_v_cc_cmd};
        GemVal gem_v_cc_ret = gem_exec_fn(NULL, _t4567, 1);
#line 484 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_cc_ret, gem_int(0)))) {
#line 485 "compiler/main.gem"
    GemVal _t4568[] = {gem_v_cc_ret};
    GemVal _t4569[] = {gem_add(gem_add(gem_string("gem: compilation failed (cc exited with "), gem_to_string_fn(NULL, _t4568, 1)), gem_string(")"))};
            (void)(gem_error_at_fn("compiler/main.gem", 485, _t4569, 1));
        }
#line 490 "compiler/main.gem"
    GemVal _t4570 = gem_v_cli;
        if (gem_truthy(gem_table_get(_t4570, gem_string("run")))) {
#line 491 "compiler/main.gem"
            GemVal gem_v_run_cmd = gem_v_out_name;
#line 492 "compiler/main.gem"
            GemVal gem_v_i = gem_int(0);
#line 493 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4571 = gem_v_cli;
                GemVal _t4572[] = {gem_table_get(_t4571, gem_string("run_args"))};
                if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t4572, 1)))) break;
#line 494 "compiler/main.gem"
    GemVal _t4573 = gem_v_cli;
                gem_v_run_cmd = gem_add(gem_add(gem_v_run_cmd, gem_string(" ")), gem_table_get(gem_table_get(_t4573, gem_string("run_args")), gem_v_i));
#line 495 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 497 "compiler/main.gem"
    GemVal _t4574[] = {gem_v_run_cmd};
            GemVal gem_v_run_ret = gem_exec_fn(NULL, _t4574, 1);
#line 498 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_run_ret, gem_int(0)))) {
#line 499 "compiler/main.gem"
    GemVal _t4575[] = {gem_v_run_ret};
                (void)(gem_exit_process_fn(NULL, _t4575, 1));
            }
        }
    }
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

