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
    GemVal gem_v_defaults = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_anon_fn", "compiler/main.gem", 58);
    GemVal _t101 = gem_table_new();
    gem_table_set(_t101, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t101, gem_string("params"), gem_v_params);
    gem_table_set(_t101, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t101, gem_string("defaults"), gem_v_defaults);
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
    GemVal gem_v_defaults = (argc > 5) ? args[5] : GEM_NIL;
    gem_push_frame("_mod_ast_make_fn_def", "compiler/main.gem", 96);
    GemVal _t119 = gem_table_new();
    gem_table_set(_t119, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t119, gem_string("name"), gem_v_name);
    gem_table_set(_t119, gem_string("params"), gem_v_params);
    gem_table_set(_t119, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t119, gem_string("defaults"), gem_v_defaults);
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
        static GemICacheSlot _ic_1 = {0};
        _t208 = gem_eq(gem_table_get_cached(_t207, "type", &_ic_1), gem_string("NEWLINE"));
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
    static GemICacheSlot _ic_2 = {0};
    GemVal _t434 = gem_table_get_cached(_t433, "parse", &_ic_2);
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
    static GemICacheSlot _ic_3 = {0};
    GemVal _t443 = gem_eq(gem_table_get_cached(_t442, "type", &_ic_3), gem_string("EOF"));
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
    static GemICacheSlot _ic_4 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t445, "type", &_ic_4), gem_v_tp))) {
#line 94 "compiler/main.gem"
    GemVal _t446 = gem_v_t;
    static GemICacheSlot _ic_5 = {0};
    GemVal _t447 = gem_v_t;
    static GemICacheSlot _ic_6 = {0};
    GemVal _t448 = gem_v_t;
    static GemICacheSlot _ic_7 = {0};
    GemVal _t449[] = {gem_table_get_cached(_t448, "value", &_ic_7)};
    GemVal _t450 = gem_v_t;
    static GemICacheSlot _ic_8 = {0};
    GemVal _t451[] = {gem_string("expected '"), gem_v_tp, gem_string("' but got '"), gem_table_get_cached(_t450, "type", &_ic_8), gem_string("'")};
    GemVal _t452[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t446, "line", &_ic_5), gem_table_get_cached(_t447, "col", &_ic_6), gem_len_fn(NULL, _t449, 1), gem_interp(5, _t451), GEM_NIL};
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
        static GemICacheSlot _ic_9 = {0};
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t456, "type", &_ic_9), gem_string("NEWLINE")))) break;
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
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_peek;
    GemVal *gem_v_source;
};
static GemVal _anon_7(void *_env, GemVal *args, int argc) {
    struct _closure__anon_7 *_cls = (struct _closure__anon_7 *)_env;
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_7", "compiler/main.gem", 0);
#line 121 "compiler/main.gem"
    GemVal _t458 = gem_table_new();
    GemVal gem_v_params = _t458;
#line 122 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 123 "compiler/main.gem"
    GemVal _t459 = gem_table_new();
    GemVal gem_v_defaults = _t459;
#line 124 "compiler/main.gem"
    GemVal gem_v_has_default = gem_bool(0);
#line 126 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t460 = (*gem_v_peek);
        GemVal _t461 = _t460.fn(_t460.env, NULL, 0);
        static GemICacheSlot _ic_10 = {0};
        GemVal _t464;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t461, "type", &_ic_10), gem_string("NAME")))) {
                _t464 = gem_eq(gem_table_get_cached(_t461, "type", &_ic_10), gem_string("NAME"));
        } else {
                GemVal _t462 = (*gem_v_peek);
                GemVal _t463 = _t462.fn(_t462.env, NULL, 0);
                static GemICacheSlot _ic_11 = {0};
                _t464 = gem_eq(gem_table_get_cached(_t463, "type", &_ic_11), gem_string("..."));
        }
        if (!gem_truthy(_t464)) break;
#line 127 "compiler/main.gem"
    GemVal _t465 = (*gem_v_peek);
    GemVal _t466 = _t465.fn(_t465.env, NULL, 0);
    static GemICacheSlot _ic_12 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t466, "type", &_ic_12), gem_string("...")))) {
#line 128 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
            (void)(_t467.fn(_t467.env, NULL, 0));
#line 129 "compiler/main.gem"
    GemVal _t468[] = {gem_string("NAME")};
    GemVal _t469 = (*gem_v_expect);
    GemVal _t470 = _t469.fn(_t469.env, _t468, 1);
    static GemICacheSlot _ic_13 = {0};
            gem_v_rest_param = gem_table_get_cached(_t470, "value", &_ic_13);
            break;
        }
#line 132 "compiler/main.gem"
    GemVal _t471 = (*gem_v_advance);
        GemVal gem_v_name_tok = _t471.fn(_t471.env, NULL, 0);
#line 133 "compiler/main.gem"
    GemVal _t472 = gem_v_name_tok;
    static GemICacheSlot _ic_14 = {0};
    GemVal _t473[] = {gem_v_params, gem_table_get_cached(_t472, "value", &_ic_14)};
        (void)(gem_push_fn(NULL, _t473, 2));
#line 134 "compiler/main.gem"
    GemVal _t474 = (*gem_v_peek);
    GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
    static GemICacheSlot _ic_15 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t475, "type", &_ic_15), gem_string("=")))) {
#line 135 "compiler/main.gem"
    GemVal _t476 = (*gem_v_advance);
            (void)(_t476.fn(_t476.env, NULL, 0));
#line 136 "compiler/main.gem"
    GemVal _t477 = (*gem_v_parse_expr);
    GemVal _t478[] = {gem_v_defaults, _t477.fn(_t477.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t478, 2));
#line 137 "compiler/main.gem"
            gem_v_has_default = gem_bool(1);
        } else {
#line 139 "compiler/main.gem"
            if (gem_truthy(gem_v_has_default)) {
#line 140 "compiler/main.gem"
    GemVal _t479 = gem_v_name_tok;
    static GemICacheSlot _ic_16 = {0};
    GemVal _t480 = gem_v_name_tok;
    static GemICacheSlot _ic_17 = {0};
    GemVal _t481 = gem_v_name_tok;
    static GemICacheSlot _ic_18 = {0};
    GemVal _t482[] = {gem_table_get_cached(_t481, "value", &_ic_18)};
    GemVal _t483[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t479, "line", &_ic_16), gem_table_get_cached(_t480, "col", &_ic_17), gem_len_fn(NULL, _t482, 1), gem_string("required parameter after optional parameter"), GEM_NIL};
    GemVal _t484 = (*gem_v__mod_parser_compile_error);
                (void)(_t484.fn(_t484.env, _t483, 7));
            }
#line 142 "compiler/main.gem"
    GemVal _t485[] = {gem_v_defaults, GEM_NIL};
            (void)(gem_push_fn(NULL, _t485, 2));
        }
#line 144 "compiler/main.gem"
    GemVal _t486 = (*gem_v_peek);
    GemVal _t487 = _t486.fn(_t486.env, NULL, 0);
    static GemICacheSlot _ic_19 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t487, "type", &_ic_19), gem_string(",")))) {
            break;
        }
#line 147 "compiler/main.gem"
    GemVal _t488 = (*gem_v_advance);
        (void)(_t488.fn(_t488.env, NULL, 0));
    }
    GemVal _t489 = gem_table_new();
    gem_table_set(_t489, gem_string("params"), gem_v_params);
    gem_table_set(_t489, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t489, gem_string("defaults"), gem_v_defaults);
    GemVal _t490 = _t489;
    gem_pop_frame();
    return _t490;
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
#line 158 "compiler/main.gem"
    GemVal _t492 = gem_table_new();
    GemVal gem_v_stmts = _t492;
#line 159 "compiler/main.gem"
    GemVal _t493 = (*gem_v_skip_nl);
    (void)(_t493.fn(_t493.env, NULL, 0));
#line 160 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t494 = (*gem_v_peek);
        GemVal _t495 = _t494.fn(_t494.env, NULL, 0);
        static GemICacheSlot _ic_20 = {0};
        GemVal _t498;
        if (!gem_truthy(gem_neq(gem_table_get_cached(_t495, "type", &_ic_20), gem_string("end")))) {
                _t498 = gem_neq(gem_table_get_cached(_t495, "type", &_ic_20), gem_string("end"));
        } else {
                GemVal _t496 = (*gem_v_peek);
                GemVal _t497 = _t496.fn(_t496.env, NULL, 0);
                static GemICacheSlot _ic_21 = {0};
                _t498 = gem_neq(gem_table_get_cached(_t497, "type", &_ic_21), gem_string("elif"));
        }
        GemVal _t501;
        if (!gem_truthy(_t498)) {
                _t501 = _t498;
        } else {
                GemVal _t499 = (*gem_v_peek);
                GemVal _t500 = _t499.fn(_t499.env, NULL, 0);
                static GemICacheSlot _ic_22 = {0};
                _t501 = gem_neq(gem_table_get_cached(_t500, "type", &_ic_22), gem_string("else"));
        }
        GemVal _t504;
        if (!gem_truthy(_t501)) {
                _t504 = _t501;
        } else {
                GemVal _t502 = (*gem_v_peek);
                GemVal _t503 = _t502.fn(_t502.env, NULL, 0);
                static GemICacheSlot _ic_23 = {0};
                _t504 = gem_neq(gem_table_get_cached(_t503, "type", &_ic_23), gem_string("when"));
        }
        GemVal _t507;
        if (!gem_truthy(_t504)) {
                _t507 = _t504;
        } else {
                GemVal _t505 = (*gem_v_peek);
                GemVal _t506 = _t505.fn(_t505.env, NULL, 0);
                static GemICacheSlot _ic_24 = {0};
                _t507 = gem_neq(gem_table_get_cached(_t506, "type", &_ic_24), gem_string("after"));
        }
        GemVal _t510;
        if (!gem_truthy(_t507)) {
                _t510 = _t507;
        } else {
                GemVal _t508 = (*gem_v_peek);
                GemVal _t509 = _t508.fn(_t508.env, NULL, 0);
                static GemICacheSlot _ic_25 = {0};
                _t510 = gem_neq(gem_table_get_cached(_t509, "type", &_ic_25), gem_string("then"));
        }
        GemVal _t512;
        if (!gem_truthy(_t510)) {
                _t512 = _t510;
        } else {
                GemVal _t511 = (*gem_v_at_end);
                _t512 = gem_not(_t511.fn(_t511.env, NULL, 0));
        }
        if (!gem_truthy(_t512)) break;
#line 161 "compiler/main.gem"
    GemVal _t513 = (*gem_v_parse_stmt);
    GemVal _t514[] = {gem_v_stmts, _t513.fn(_t513.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t514, 2));
#line 162 "compiler/main.gem"
    GemVal _t515 = (*gem_v_skip_nl);
        (void)(_t515.fn(_t515.env, NULL, 0));
    }
    GemVal _t516 = gem_v_stmts;
    gem_pop_frame();
    return _t516;
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
#line 171 "compiler/main.gem"
    GemVal _t518 = gem_table_new();
    GemVal gem_v_stmts = _t518;
#line 172 "compiler/main.gem"
    GemVal _t519 = (*gem_v_skip_nl);
    (void)(_t519.fn(_t519.env, NULL, 0));
#line 173 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t520 = (*gem_v_peek);
        GemVal _t521 = _t520.fn(_t520.env, NULL, 0);
        static GemICacheSlot _ic_26 = {0};
        GemVal _t523;
        if (!gem_truthy(gem_neq(gem_table_get_cached(_t521, "type", &_ic_26), gem_string("end")))) {
                _t523 = gem_neq(gem_table_get_cached(_t521, "type", &_ic_26), gem_string("end"));
        } else {
                GemVal _t522 = (*gem_v_at_end);
                _t523 = gem_not(_t522.fn(_t522.env, NULL, 0));
        }
        if (!gem_truthy(_t523)) break;
#line 174 "compiler/main.gem"
    GemVal _t524 = (*gem_v_parse_stmt);
    GemVal _t525[] = {gem_v_stmts, _t524.fn(_t524.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t525, 2));
#line 175 "compiler/main.gem"
    GemVal _t526 = (*gem_v_skip_nl);
        (void)(_t526.fn(_t526.env, NULL, 0));
    }
    GemVal _t527 = gem_v_stmts;
    gem_pop_frame();
    return _t527;
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
#line 184 "compiler/main.gem"
    GemVal _t529 = (*gem_v_peek);
    GemVal gem_v_t = _t529.fn(_t529.env, NULL, 0);
#line 187 "compiler/main.gem"
    GemVal _t530 = gem_v_t;
    static GemICacheSlot _ic_27 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t530, "type", &_ic_27), gem_string("NUMBER")))) {
#line 188 "compiler/main.gem"
    GemVal _t531 = (*gem_v_advance);
        (void)(_t531.fn(_t531.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t532 = gem_v_t;
    static GemICacheSlot _ic_28 = {0};
    GemVal _t533[] = {gem_table_get_cached(_t532, "value", &_ic_28)};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t533, 1))) {
#line 190 "compiler/main.gem"
    GemVal _t534 = gem_v_t;
    static GemICacheSlot _ic_29 = {0};
    GemVal _t535[] = {gem_table_get_cached(_t534, "value", &_ic_29)};
    GemVal _t536[] = {gem_fn_atof(NULL, _t535, 1)};
    GemVal _t537 = (*gem_v__mod_parser_make_float);
            GemVal _t538 = _t537.fn(_t537.env, _t536, 1);
            gem_pop_frame();
            return _t538;
        }
#line 192 "compiler/main.gem"
    GemVal _t539 = gem_v_t;
    static GemICacheSlot _ic_30 = {0};
    GemVal _t540[] = {gem_table_get_cached(_t539, "value", &_ic_30)};
    GemVal _t541[] = {gem_fn__mod_parser_str_to_int(NULL, _t540, 1)};
    GemVal _t542 = (*gem_v__mod_parser_make_int);
        GemVal _t543 = _t542.fn(_t542.env, _t541, 1);
        gem_pop_frame();
        return _t543;
    }
#line 196 "compiler/main.gem"
    GemVal _t544 = gem_v_t;
    static GemICacheSlot _ic_31 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t544, "type", &_ic_31), gem_string("STRING")))) {
#line 197 "compiler/main.gem"
    GemVal _t545 = (*gem_v_advance);
        (void)(_t545.fn(_t545.env, NULL, 0));
#line 198 "compiler/main.gem"
    GemVal _t546 = gem_v_t;
    static GemICacheSlot _ic_32 = {0};
    GemVal _t547[] = {gem_table_get_cached(_t546, "value", &_ic_32)};
    GemVal _t548 = (*gem_v__mod_parser_make_string);
        GemVal _t549 = _t548.fn(_t548.env, _t547, 1);
        gem_pop_frame();
        return _t549;
    }
#line 202 "compiler/main.gem"
    GemVal _t550 = gem_v_t;
    static GemICacheSlot _ic_33 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t550, "type", &_ic_33), gem_string("INTERP_START")))) {
#line 203 "compiler/main.gem"
    GemVal _t551 = (*gem_v_advance);
        (void)(_t551.fn(_t551.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t552 = gem_table_new();
        GemVal gem_v_parts = _t552;
#line 205 "compiler/main.gem"
    GemVal _t553 = (*gem_v_skip_nl);
        (void)(_t553.fn(_t553.env, NULL, 0));
#line 206 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t554 = (*gem_v_peek);
            GemVal _t555 = _t554.fn(_t554.env, NULL, 0);
            static GemICacheSlot _ic_34 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t555, "type", &_ic_34), gem_string("INTERP_END")))) break;
#line 207 "compiler/main.gem"
    GemVal _t556 = (*gem_v_peek);
    GemVal _t557 = _t556.fn(_t556.env, NULL, 0);
    static GemICacheSlot _ic_35 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t557, "type", &_ic_35), gem_string("STRING")))) {
#line 208 "compiler/main.gem"
    GemVal _t558 = (*gem_v_advance);
    GemVal _t559 = _t558.fn(_t558.env, NULL, 0);
    static GemICacheSlot _ic_36 = {0};
                GemVal gem_v_sval = gem_table_get_cached(_t559, "value", &_ic_36);
#line 209 "compiler/main.gem"
    GemVal _t560[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t560, 1), gem_int(0)))) {
#line 210 "compiler/main.gem"
    GemVal _t561[] = {gem_v_sval};
    GemVal _t562 = (*gem_v__mod_parser_make_string);
    GemVal _t563[] = {gem_v_parts, _t562.fn(_t562.env, _t561, 1)};
                    (void)(gem_push_fn(NULL, _t563, 2));
                }
            } else {
#line 213 "compiler/main.gem"
    GemVal _t564 = (*gem_v_parse_expr);
    GemVal _t565[] = {gem_v_parts, _t564.fn(_t564.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t565, 2));
            }
#line 215 "compiler/main.gem"
    GemVal _t566 = (*gem_v_skip_nl);
            (void)(_t566.fn(_t566.env, NULL, 0));
        }
#line 217 "compiler/main.gem"
    GemVal _t567[] = {gem_string("INTERP_END")};
    GemVal _t568 = (*gem_v_expect);
        (void)(_t568.fn(_t568.env, _t567, 1));
#line 218 "compiler/main.gem"
    GemVal _t569[] = {gem_v_parts};
    GemVal _t570 = (*gem_v__mod_parser_make_interp);
        GemVal _t571 = _t570.fn(_t570.env, _t569, 1);
        gem_pop_frame();
        return _t571;
    }
#line 222 "compiler/main.gem"
    GemVal _t572 = gem_v_t;
    static GemICacheSlot _ic_37 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t572, "type", &_ic_37), gem_string("true")))) {
#line 223 "compiler/main.gem"
    GemVal _t573 = (*gem_v_advance);
        (void)(_t573.fn(_t573.env, NULL, 0));
#line 224 "compiler/main.gem"
    GemVal _t574[] = {gem_bool(1)};
    GemVal _t575 = (*gem_v__mod_parser_make_bool);
        GemVal _t576 = _t575.fn(_t575.env, _t574, 1);
        gem_pop_frame();
        return _t576;
    }
#line 226 "compiler/main.gem"
    GemVal _t577 = gem_v_t;
    static GemICacheSlot _ic_38 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t577, "type", &_ic_38), gem_string("false")))) {
#line 227 "compiler/main.gem"
    GemVal _t578 = (*gem_v_advance);
        (void)(_t578.fn(_t578.env, NULL, 0));
#line 228 "compiler/main.gem"
    GemVal _t579[] = {gem_bool(0)};
    GemVal _t580 = (*gem_v__mod_parser_make_bool);
        GemVal _t581 = _t580.fn(_t580.env, _t579, 1);
        gem_pop_frame();
        return _t581;
    }
#line 232 "compiler/main.gem"
    GemVal _t582 = gem_v_t;
    static GemICacheSlot _ic_39 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t582, "type", &_ic_39), gem_string("nil")))) {
#line 233 "compiler/main.gem"
    GemVal _t583 = (*gem_v_advance);
        (void)(_t583.fn(_t583.env, NULL, 0));
#line 234 "compiler/main.gem"
    GemVal _t584 = (*gem_v__mod_parser_make_nil_node);
        GemVal _t585 = _t584.fn(_t584.env, NULL, 0);
        gem_pop_frame();
        return _t585;
    }
#line 238 "compiler/main.gem"
    GemVal _t586 = gem_v_t;
    static GemICacheSlot _ic_40 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t586, "type", &_ic_40), gem_string("fn")))) {
#line 239 "compiler/main.gem"
    GemVal _t587 = (*gem_v_advance);
        (void)(_t587.fn(_t587.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t588[] = {gem_string("(")};
    GemVal _t589 = (*gem_v_expect);
        (void)(_t589.fn(_t589.env, _t588, 1));
#line 241 "compiler/main.gem"
    GemVal _t590 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t590.fn(_t590.env, NULL, 0);
#line 242 "compiler/main.gem"
    GemVal _t591[] = {gem_string(")")};
    GemVal _t592 = (*gem_v_expect);
        (void)(_t592.fn(_t592.env, _t591, 1));
#line 243 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 244 "compiler/main.gem"
    GemVal _t593 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t593.fn(_t593.env, NULL, 0);
#line 245 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 246 "compiler/main.gem"
    GemVal _t594[] = {gem_string("end")};
    GemVal _t595 = (*gem_v_expect);
        (void)(_t595.fn(_t595.env, _t594, 1));
#line 247 "compiler/main.gem"
    GemVal _t596 = gem_v_pr;
    static GemICacheSlot _ic_41 = {0};
    GemVal _t597 = gem_v_pr;
    static GemICacheSlot _ic_42 = {0};
    GemVal _t598 = gem_v_pr;
    static GemICacheSlot _ic_43 = {0};
    GemVal _t599[] = {gem_table_get_cached(_t596, "params", &_ic_41), gem_table_get_cached(_t597, "rest_param", &_ic_42), gem_v_body, gem_table_get_cached(_t598, "defaults", &_ic_43)};
    GemVal _t600 = (*gem_v__mod_parser_make_anon_fn);
        GemVal _t601 = _t600.fn(_t600.env, _t599, 4);
        gem_pop_frame();
        return _t601;
    }
#line 252 "compiler/main.gem"
    GemVal _t602 = gem_v_t;
    static GemICacheSlot _ic_44 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t602, "type", &_ic_44), gem_string("{")))) {
#line 253 "compiler/main.gem"
    GemVal _t603 = (*gem_v_advance);
        (void)(_t603.fn(_t603.env, NULL, 0));
#line 254 "compiler/main.gem"
    GemVal _t604 = (*gem_v_skip_nl);
        (void)(_t604.fn(_t604.env, NULL, 0));
#line 255 "compiler/main.gem"
    GemVal _t605 = gem_table_new();
        GemVal gem_v_entries = _t605;
#line 256 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t606 = (*gem_v_peek);
            GemVal _t607 = _t606.fn(_t606.env, NULL, 0);
            static GemICacheSlot _ic_45 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t607, "type", &_ic_45), gem_string("}")))) break;
#line 257 "compiler/main.gem"
    GemVal _t608 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t608.fn(_t608.env, NULL, 0);
#line 258 "compiler/main.gem"
    GemVal _t609 = gem_v_key_tok;
    static GemICacheSlot _ic_46 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t609, "type", &_ic_46), gem_string("NAME")))) {
#line 259 "compiler/main.gem"
    GemVal _t610 = (*gem_v_advance);
                (void)(_t610.fn(_t610.env, NULL, 0));
            } else {
#line 260 "compiler/main.gem"
    GemVal _t611 = (*gem_v_at_end);
    GemVal _t615;
    if (!gem_truthy(gem_not(_t611.fn(_t611.env, NULL, 0)))) {
        _t615 = gem_not(_t611.fn(_t611.env, NULL, 0));
    } else {
        GemVal _t612[] = {gem_int(1)};
        GemVal _t613 = (*gem_v_peek_at);
        GemVal _t614 = _t613.fn(_t613.env, _t612, 1);
        static GemICacheSlot _ic_47 = {0};
        _t615 = gem_eq(gem_table_get_cached(_t614, "type", &_ic_47), gem_string(":"));
    }
                if (gem_truthy(_t615)) {
#line 262 "compiler/main.gem"
    GemVal _t616 = (*gem_v_advance);
                    (void)(_t616.fn(_t616.env, NULL, 0));
                } else {
#line 264 "compiler/main.gem"
    GemVal _t617 = gem_v_key_tok;
    static GemICacheSlot _ic_48 = {0};
    GemVal _t618 = gem_v_key_tok;
    static GemICacheSlot _ic_49 = {0};
    GemVal _t619 = gem_v_key_tok;
    static GemICacheSlot _ic_50 = {0};
    GemVal _t620[] = {gem_table_get_cached(_t619, "value", &_ic_50)};
    GemVal _t621 = gem_v_key_tok;
    static GemICacheSlot _ic_51 = {0};
    GemVal _t622[] = {gem_string("expected '}' or table key but got '"), gem_table_get_cached(_t621, "type", &_ic_51), gem_string("'")};
    GemVal _t623[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t617, "line", &_ic_48), gem_table_get_cached(_t618, "col", &_ic_49), gem_len_fn(NULL, _t620, 1), gem_interp(3, _t622), GEM_NIL};
    GemVal _t624 = (*gem_v__mod_parser_compile_error);
                    (void)(_t624.fn(_t624.env, _t623, 7));
                }
            }
#line 266 "compiler/main.gem"
    GemVal _t625[] = {gem_string(":")};
    GemVal _t626 = (*gem_v_expect);
            (void)(_t626.fn(_t626.env, _t625, 1));
#line 267 "compiler/main.gem"
    GemVal _t627 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t627.fn(_t627.env, NULL, 0);
#line 268 "compiler/main.gem"
    GemVal _t628 = gem_v_key_tok;
    static GemICacheSlot _ic_52 = {0};
    GemVal _t629[] = {gem_table_get_cached(_t628, "value", &_ic_52), gem_v_val};
    GemVal _t630 = (*gem_v__mod_parser_make_table_entry);
    GemVal _t631[] = {gem_v_entries, _t630.fn(_t630.env, _t629, 2)};
            (void)(gem_push_fn(NULL, _t631, 2));
#line 269 "compiler/main.gem"
    GemVal _t632 = (*gem_v_skip_nl);
            (void)(_t632.fn(_t632.env, NULL, 0));
#line 270 "compiler/main.gem"
    GemVal _t633 = (*gem_v_peek);
    GemVal _t634 = _t633.fn(_t633.env, NULL, 0);
    static GemICacheSlot _ic_53 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t634, "type", &_ic_53), gem_string(",")))) {
#line 271 "compiler/main.gem"
    GemVal _t635 = (*gem_v_advance);
                (void)(_t635.fn(_t635.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t636 = (*gem_v_skip_nl);
                (void)(_t636.fn(_t636.env, NULL, 0));
            }
        }
#line 275 "compiler/main.gem"
    GemVal _t637[] = {gem_string("}")};
    GemVal _t638 = (*gem_v_expect);
        (void)(_t638.fn(_t638.env, _t637, 1));
#line 276 "compiler/main.gem"
    GemVal _t639[] = {gem_v_entries};
    GemVal _t640 = (*gem_v__mod_parser_make_table);
        GemVal _t641 = _t640.fn(_t640.env, _t639, 1);
        gem_pop_frame();
        return _t641;
    }
#line 280 "compiler/main.gem"
    GemVal _t642 = gem_v_t;
    static GemICacheSlot _ic_54 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t642, "type", &_ic_54), gem_string("[")))) {
#line 281 "compiler/main.gem"
    GemVal _t643 = (*gem_v_advance);
        (void)(_t643.fn(_t643.env, NULL, 0));
#line 282 "compiler/main.gem"
    GemVal _t644 = (*gem_v_skip_nl);
        (void)(_t644.fn(_t644.env, NULL, 0));
#line 283 "compiler/main.gem"
    GemVal _t645 = gem_table_new();
        GemVal gem_v_elements = _t645;
#line 284 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t646 = (*gem_v_peek);
            GemVal _t647 = _t646.fn(_t646.env, NULL, 0);
            static GemICacheSlot _ic_55 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t647, "type", &_ic_55), gem_string("]")))) break;
#line 285 "compiler/main.gem"
    GemVal _t648 = (*gem_v_parse_expr);
    GemVal _t649[] = {gem_v_elements, _t648.fn(_t648.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t649, 2));
#line 286 "compiler/main.gem"
    GemVal _t650 = (*gem_v_skip_nl);
            (void)(_t650.fn(_t650.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t651 = (*gem_v_peek);
    GemVal _t652 = _t651.fn(_t651.env, NULL, 0);
    static GemICacheSlot _ic_56 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t652, "type", &_ic_56), gem_string(",")))) {
#line 288 "compiler/main.gem"
    GemVal _t653 = (*gem_v_advance);
                (void)(_t653.fn(_t653.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t654 = (*gem_v_skip_nl);
                (void)(_t654.fn(_t654.env, NULL, 0));
            }
        }
#line 292 "compiler/main.gem"
    GemVal _t655[] = {gem_string("]")};
    GemVal _t656 = (*gem_v_expect);
        (void)(_t656.fn(_t656.env, _t655, 1));
#line 293 "compiler/main.gem"
    GemVal _t657[] = {gem_v_elements};
    GemVal _t658 = (*gem_v__mod_parser_make_array);
        GemVal _t659 = _t658.fn(_t658.env, _t657, 1);
        gem_pop_frame();
        return _t659;
    }
#line 297 "compiler/main.gem"
    GemVal _t660 = gem_v_t;
    static GemICacheSlot _ic_57 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t660, "type", &_ic_57), gem_string("(")))) {
#line 298 "compiler/main.gem"
    GemVal _t661 = (*gem_v_advance);
        (void)(_t661.fn(_t661.env, NULL, 0));
#line 299 "compiler/main.gem"
    GemVal _t662 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t662.fn(_t662.env, NULL, 0);
#line 300 "compiler/main.gem"
    GemVal _t663[] = {gem_string(")")};
    GemVal _t664 = (*gem_v_expect);
        (void)(_t664.fn(_t664.env, _t663, 1));
#line 301 "compiler/main.gem"
        GemVal _t665 = gem_v_e;
        gem_pop_frame();
        return _t665;
    }
#line 305 "compiler/main.gem"
    GemVal _t666 = gem_v_t;
    static GemICacheSlot _ic_58 = {0};
    GemVal _t668;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t666, "type", &_ic_58), gem_string("NAME")))) {
        _t668 = gem_eq(gem_table_get_cached(_t666, "type", &_ic_58), gem_string("NAME"));
    } else {
        GemVal _t667 = gem_v_t;
        static GemICacheSlot _ic_59 = {0};
        _t668 = gem_eq(gem_table_get_cached(_t667, "value", &_ic_59), gem_string("pcall"));
    }
    GemVal _t672;
    if (!gem_truthy(_t668)) {
        _t672 = _t668;
    } else {
        GemVal _t669[] = {gem_int(1)};
        GemVal _t670 = (*gem_v_peek_at);
        GemVal _t671 = _t670.fn(_t670.env, _t669, 1);
        static GemICacheSlot _ic_60 = {0};
        _t672 = gem_neq(gem_table_get_cached(_t671, "type", &_ic_60), gem_string("("));
    }
    if (gem_truthy(_t672)) {
#line 306 "compiler/main.gem"
    GemVal _t673 = (*gem_v_advance);
        (void)(_t673.fn(_t673.env, NULL, 0));
#line 307 "compiler/main.gem"
    GemVal _t674 = (*gem_v_parse_expr);
        GemVal gem_v_expr = _t674.fn(_t674.env, NULL, 0);
#line 308 "compiler/main.gem"
    GemVal _t675[] = {gem_string("pcall")};
    GemVal _t676 = (*gem_v__mod_parser_make_var);
    GemVal _t677 = gem_table_new();
    GemVal _t678 = gem_table_new();
    GemVal _t679 = gem_table_new();
    gem_table_set(_t679, gem_int(0), gem_v_expr);
    GemVal _t680 = gem_table_new();
    GemVal _t681[] = {_t678, GEM_NIL, _t679, _t680};
    GemVal _t682 = (*gem_v__mod_parser_make_anon_fn);
    gem_table_set(_t677, gem_int(0), _t682.fn(_t682.env, _t681, 4));
    GemVal _t683 = gem_v_t;
    static GemICacheSlot _ic_61 = {0};
    GemVal _t684[] = {_t676.fn(_t676.env, _t675, 1), _t677, gem_table_get_cached(_t683, "line", &_ic_61)};
    GemVal _t685 = (*gem_v__mod_parser_make_call);
        GemVal _t686 = _t685.fn(_t685.env, _t684, 3);
        gem_pop_frame();
        return _t686;
    }
#line 312 "compiler/main.gem"
    GemVal _t687 = gem_v_t;
    static GemICacheSlot _ic_62 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t687, "type", &_ic_62), gem_string("NAME")))) {
#line 313 "compiler/main.gem"
    GemVal _t688 = (*gem_v_advance);
        (void)(_t688.fn(_t688.env, NULL, 0));
#line 314 "compiler/main.gem"
    GemVal _t689 = gem_v_t;
    static GemICacheSlot _ic_63 = {0};
    GemVal _t690[] = {gem_table_get_cached(_t689, "value", &_ic_63)};
    GemVal _t691 = (*gem_v__mod_parser_make_var);
        GemVal _t692 = _t691.fn(_t691.env, _t690, 1);
        gem_pop_frame();
        return _t692;
    }
#line 317 "compiler/main.gem"
    GemVal _t693 = gem_v_t;
    static GemICacheSlot _ic_64 = {0};
    GemVal _t694 = gem_v_t;
    static GemICacheSlot _ic_65 = {0};
    GemVal _t695 = gem_v_t;
    static GemICacheSlot _ic_66 = {0};
    GemVal _t696[] = {gem_table_get_cached(_t695, "value", &_ic_66)};
    GemVal _t697 = gem_v_t;
    static GemICacheSlot _ic_67 = {0};
    GemVal _t698[] = {gem_string("unexpected token '"), gem_table_get_cached(_t697, "type", &_ic_67), gem_string("' (expected expression)")};
    GemVal _t699[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t693, "line", &_ic_64), gem_table_get_cached(_t694, "col", &_ic_65), gem_len_fn(NULL, _t696, 1), gem_interp(3, _t698), GEM_NIL};
    GemVal _t700 = (*gem_v__mod_parser_compile_error);
    GemVal _t701 = _t700.fn(_t700.env, _t699, 7);
    gem_pop_frame();
    return _t701;
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
#line 322 "compiler/main.gem"
    GemVal _t703 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t703.fn(_t703.env, NULL, 0);
#line 324 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 326 "compiler/main.gem"
    GemVal _t704 = (*gem_v_peek);
    GemVal _t705 = _t704.fn(_t704.env, NULL, 0);
    static GemICacheSlot _ic_68 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t705, "type", &_ic_68), gem_string("(")))) {
#line 327 "compiler/main.gem"
    GemVal _t706 = (*gem_v_peek);
    GemVal _t707 = _t706.fn(_t706.env, NULL, 0);
    static GemICacheSlot _ic_69 = {0};
            GemVal gem_v_call_line = gem_table_get_cached(_t707, "line", &_ic_69);
#line 328 "compiler/main.gem"
    GemVal _t708 = (*gem_v_advance);
            (void)(_t708.fn(_t708.env, NULL, 0));
#line 329 "compiler/main.gem"
    GemVal _t709 = (*gem_v_skip_nl);
            (void)(_t709.fn(_t709.env, NULL, 0));
#line 330 "compiler/main.gem"
    GemVal _t710 = gem_table_new();
            GemVal gem_v_args = _t710;
#line 331 "compiler/main.gem"
    GemVal _t711 = (*gem_v_peek);
    GemVal _t712 = _t711.fn(_t711.env, NULL, 0);
    static GemICacheSlot _ic_70 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t712, "type", &_ic_70), gem_string(")")))) {
#line 332 "compiler/main.gem"
    GemVal _t713 = (*gem_v_parse_expr);
    GemVal _t714[] = {gem_v_args, _t713.fn(_t713.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t714, 2));
#line 333 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t715 = (*gem_v_peek);
                    GemVal _t716 = _t715.fn(_t715.env, NULL, 0);
                    static GemICacheSlot _ic_71 = {0};
                    if (!gem_truthy(gem_eq(gem_table_get_cached(_t716, "type", &_ic_71), gem_string(",")))) break;
#line 334 "compiler/main.gem"
    GemVal _t717 = (*gem_v_advance);
                    (void)(_t717.fn(_t717.env, NULL, 0));
#line 335 "compiler/main.gem"
    GemVal _t718 = (*gem_v_skip_nl);
                    (void)(_t718.fn(_t718.env, NULL, 0));
#line 336 "compiler/main.gem"
    GemVal _t719 = (*gem_v_parse_expr);
    GemVal _t720[] = {gem_v_args, _t719.fn(_t719.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t720, 2));
                }
            }
#line 339 "compiler/main.gem"
    GemVal _t721 = (*gem_v_skip_nl);
            (void)(_t721.fn(_t721.env, NULL, 0));
#line 340 "compiler/main.gem"
    GemVal _t722[] = {gem_string(")")};
    GemVal _t723 = (*gem_v_expect);
            (void)(_t723.fn(_t723.env, _t722, 1));
#line 343 "compiler/main.gem"
    GemVal _t724 = (*gem_v_peek);
    GemVal _t725 = _t724.fn(_t724.env, NULL, 0);
    static GemICacheSlot _ic_72 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t725, "type", &_ic_72), gem_string("do")))) {
#line 344 "compiler/main.gem"
    GemVal _t726 = (*gem_v_advance);
                (void)(_t726.fn(_t726.env, NULL, 0));
#line 345 "compiler/main.gem"
    GemVal _t727 = gem_table_new();
                GemVal gem_v_bparams = _t727;
#line 346 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 347 "compiler/main.gem"
    GemVal _t728 = gem_table_new();
                GemVal gem_v_bdefaults = _t728;
#line 348 "compiler/main.gem"
    GemVal _t729 = (*gem_v_peek);
    GemVal _t730 = _t729.fn(_t729.env, NULL, 0);
    static GemICacheSlot _ic_73 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t730, "type", &_ic_73), gem_string("|")))) {
#line 349 "compiler/main.gem"
    GemVal _t731 = (*gem_v_advance);
                    (void)(_t731.fn(_t731.env, NULL, 0));
#line 350 "compiler/main.gem"
    GemVal _t732 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t732.fn(_t732.env, NULL, 0);
#line 351 "compiler/main.gem"
    GemVal _t733 = gem_v_bpr;
    static GemICacheSlot _ic_74 = {0};
                    gem_v_bparams = gem_table_get_cached(_t733, "params", &_ic_74);
#line 352 "compiler/main.gem"
    GemVal _t734 = gem_v_bpr;
    static GemICacheSlot _ic_75 = {0};
                    gem_v_brest = gem_table_get_cached(_t734, "rest_param", &_ic_75);
#line 353 "compiler/main.gem"
    GemVal _t735 = gem_v_bpr;
    static GemICacheSlot _ic_76 = {0};
                    gem_v_bdefaults = gem_table_get_cached(_t735, "defaults", &_ic_76);
#line 354 "compiler/main.gem"
    GemVal _t736[] = {gem_string("|")};
    GemVal _t737 = (*gem_v_expect);
                    (void)(_t737.fn(_t737.env, _t736, 1));
                }
#line 356 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t738 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t738.fn(_t738.env, NULL, 0);
#line 358 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 359 "compiler/main.gem"
    GemVal _t739[] = {gem_string("end")};
    GemVal _t740 = (*gem_v_expect);
                (void)(_t740.fn(_t740.env, _t739, 1));
#line 360 "compiler/main.gem"
    GemVal _t741[] = {gem_v_bparams, gem_v_brest, gem_v_body, gem_v_bdefaults};
    GemVal _t742 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t743[] = {gem_v_args, _t742.fn(_t742.env, _t741, 4)};
                (void)(gem_push_fn(NULL, _t743, 2));
            } else {
#line 364 "compiler/main.gem"
    GemVal _t744 = (*gem_v_peek);
    GemVal _t745 = _t744.fn(_t744.env, NULL, 0);
    static GemICacheSlot _ic_77 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t745, "type", &_ic_77), gem_string("{")))) {
#line 365 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 366 "compiler/main.gem"
    GemVal _t746[] = {gem_int(1)};
    GemVal _t747 = (*gem_v_peek_at);
    GemVal _t748 = _t747.fn(_t747.env, _t746, 1);
    static GemICacheSlot _ic_78 = {0};
                    if (gem_truthy(gem_eq(gem_table_get_cached(_t748, "type", &_ic_78), gem_string("|")))) {
#line 367 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 368 "compiler/main.gem"
    GemVal _t749[] = {gem_int(1)};
    GemVal _t750 = (*gem_v_peek_at);
    GemVal _t751 = _t750.fn(_t750.env, _t749, 1);
    static GemICacheSlot _ic_79 = {0};
    GemVal _t759;
    if (gem_truthy(gem_eq(gem_table_get_cached(_t751, "type", &_ic_79), gem_string("}")))) {
        _t759 = gem_eq(gem_table_get_cached(_t751, "type", &_ic_79), gem_string("}"));
    } else {
        GemVal _t752[] = {gem_int(1)};
        GemVal _t753 = (*gem_v_peek_at);
        GemVal _t754 = _t753.fn(_t753.env, _t752, 1);
        static GemICacheSlot _ic_80 = {0};
        GemVal _t758;
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t754, "type", &_ic_80), gem_string("NAME")))) {
                _t758 = gem_eq(gem_table_get_cached(_t754, "type", &_ic_80), gem_string("NAME"));
        } else {
                GemVal _t755[] = {gem_int(2)};
                GemVal _t756 = (*gem_v_peek_at);
                GemVal _t757 = _t756.fn(_t756.env, _t755, 1);
                static GemICacheSlot _ic_81 = {0};
                _t758 = gem_eq(gem_table_get_cached(_t757, "type", &_ic_81), gem_string(":"));
        }
        _t759 = _t758;
    }
                        if (gem_truthy(_t759)) {
#line 369 "compiler/main.gem"
                            gem_v_is_block = gem_bool(0);
                        } else {
#line 371 "compiler/main.gem"
                            gem_v_is_block = gem_bool(1);
                        }
                    }
#line 373 "compiler/main.gem"
                    if (gem_truthy(gem_v_is_block)) {
#line 374 "compiler/main.gem"
    GemVal _t760 = (*gem_v_advance);
                        (void)(_t760.fn(_t760.env, NULL, 0));
#line 375 "compiler/main.gem"
    GemVal _t761 = gem_table_new();
                        GemVal gem_v_bparams = _t761;
#line 376 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 377 "compiler/main.gem"
    GemVal _t762 = gem_table_new();
                        GemVal gem_v_bdefaults = _t762;
#line 378 "compiler/main.gem"
    GemVal _t763 = (*gem_v_peek);
    GemVal _t764 = _t763.fn(_t763.env, NULL, 0);
    static GemICacheSlot _ic_82 = {0};
                        if (gem_truthy(gem_eq(gem_table_get_cached(_t764, "type", &_ic_82), gem_string("|")))) {
#line 379 "compiler/main.gem"
    GemVal _t765 = (*gem_v_advance);
                            (void)(_t765.fn(_t765.env, NULL, 0));
#line 380 "compiler/main.gem"
    GemVal _t766 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t766.fn(_t766.env, NULL, 0);
#line 381 "compiler/main.gem"
    GemVal _t767 = gem_v_bpr;
    static GemICacheSlot _ic_83 = {0};
                            gem_v_bparams = gem_table_get_cached(_t767, "params", &_ic_83);
#line 382 "compiler/main.gem"
    GemVal _t768 = gem_v_bpr;
    static GemICacheSlot _ic_84 = {0};
                            gem_v_brest = gem_table_get_cached(_t768, "rest_param", &_ic_84);
#line 383 "compiler/main.gem"
    GemVal _t769 = gem_v_bpr;
    static GemICacheSlot _ic_85 = {0};
                            gem_v_bdefaults = gem_table_get_cached(_t769, "defaults", &_ic_85);
#line 384 "compiler/main.gem"
    GemVal _t770[] = {gem_string("|")};
    GemVal _t771 = (*gem_v_expect);
                            (void)(_t771.fn(_t771.env, _t770, 1));
                        }
#line 386 "compiler/main.gem"
    GemVal _t772 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t772.fn(_t772.env, NULL, 0);
#line 387 "compiler/main.gem"
    GemVal _t773[] = {gem_string("}")};
    GemVal _t774 = (*gem_v_expect);
                        (void)(_t774.fn(_t774.env, _t773, 1));
#line 388 "compiler/main.gem"
    GemVal _t775 = gem_table_new();
    gem_table_set(_t775, gem_int(0), gem_v_bexpr);
    GemVal _t776[] = {gem_v_bparams, gem_v_brest, _t775, gem_v_bdefaults};
    GemVal _t777 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t778[] = {gem_v_args, _t777.fn(_t777.env, _t776, 4)};
                        (void)(gem_push_fn(NULL, _t778, 2));
                    }
                }
            }
#line 393 "compiler/main.gem"
    GemVal _t779[] = {gem_v_node, gem_v_args, gem_v_call_line};
    GemVal _t780 = (*gem_v__mod_parser_make_call);
            gem_v_node = _t780.fn(_t780.env, _t779, 3);
            continue;
        }
#line 398 "compiler/main.gem"
    GemVal _t781 = (*gem_v_peek);
    GemVal _t782 = _t781.fn(_t781.env, NULL, 0);
    static GemICacheSlot _ic_86 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t782, "type", &_ic_86), gem_string(".")))) {
#line 399 "compiler/main.gem"
    GemVal _t783 = (*gem_v_advance);
            (void)(_t783.fn(_t783.env, NULL, 0));
#line 400 "compiler/main.gem"
    GemVal _t784 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t784.fn(_t784.env, NULL, 0);
#line 401 "compiler/main.gem"
    GemVal _t785 = gem_v_field_tok;
    static GemICacheSlot _ic_87 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t785, "type", &_ic_87), gem_string("NAME")))) {
#line 402 "compiler/main.gem"
    GemVal _t786 = (*gem_v_advance);
                (void)(_t786.fn(_t786.env, NULL, 0));
            } else {
#line 405 "compiler/main.gem"
    GemVal _t787 = (*gem_v_advance);
                (void)(_t787.fn(_t787.env, NULL, 0));
            }
#line 407 "compiler/main.gem"
    GemVal _t788 = gem_v_field_tok;
    static GemICacheSlot _ic_88 = {0};
    GemVal _t789[] = {gem_v_node, gem_table_get_cached(_t788, "value", &_ic_88)};
    GemVal _t790 = (*gem_v__mod_parser_make_dot);
            gem_v_node = _t790.fn(_t790.env, _t789, 2);
            continue;
        }
#line 412 "compiler/main.gem"
    GemVal _t791 = (*gem_v_peek);
    GemVal _t792 = _t791.fn(_t791.env, NULL, 0);
    static GemICacheSlot _ic_89 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t792, "type", &_ic_89), gem_string("[")))) {
#line 413 "compiler/main.gem"
    GemVal _t793 = (*gem_v_advance);
            (void)(_t793.fn(_t793.env, NULL, 0));
#line 414 "compiler/main.gem"
    GemVal _t794 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t794.fn(_t794.env, NULL, 0);
#line 415 "compiler/main.gem"
    GemVal _t795[] = {gem_string("]")};
    GemVal _t796 = (*gem_v_expect);
            (void)(_t796.fn(_t796.env, _t795, 1));
#line 416 "compiler/main.gem"
    GemVal _t797[] = {gem_v_node, gem_v_key};
    GemVal _t798 = (*gem_v__mod_parser_make_index);
            gem_v_node = _t798.fn(_t798.env, _t797, 2);
            continue;
        }
        break;
    }
    GemVal _t799 = gem_v_node;
    gem_pop_frame();
    return _t799;
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
#line 428 "compiler/main.gem"
    GemVal _t801 = (*gem_v_peek);
    GemVal _t802 = _t801.fn(_t801.env, NULL, 0);
    static GemICacheSlot _ic_90 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t802, "type", &_ic_90), gem_string("-")))) {
#line 429 "compiler/main.gem"
    GemVal _t803 = (*gem_v_advance);
        (void)(_t803.fn(_t803.env, NULL, 0));
#line 430 "compiler/main.gem"
    GemVal _t804 = (*gem_v_parse_unary);
    GemVal _t805[] = {gem_string("-"), _t804.fn(_t804.env, NULL, 0)};
    GemVal _t806 = (*gem_v__mod_parser_make_unop);
        GemVal _t807 = _t806.fn(_t806.env, _t805, 2);
        gem_pop_frame();
        return _t807;
    }
#line 432 "compiler/main.gem"
    GemVal _t808 = (*gem_v_parse_call);
    GemVal _t809 = _t808.fn(_t808.env, NULL, 0);
    gem_pop_frame();
    return _t809;
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
#line 437 "compiler/main.gem"
    GemVal _t811 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t811.fn(_t811.env, NULL, 0);
#line 438 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t812 = (*gem_v_peek);
        GemVal _t813 = _t812.fn(_t812.env, NULL, 0);
        static GemICacheSlot _ic_91 = {0};
        GemVal _t816;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t813, "type", &_ic_91), gem_string("*")))) {
                _t816 = gem_eq(gem_table_get_cached(_t813, "type", &_ic_91), gem_string("*"));
        } else {
                GemVal _t814 = (*gem_v_peek);
                GemVal _t815 = _t814.fn(_t814.env, NULL, 0);
                static GemICacheSlot _ic_92 = {0};
                _t816 = gem_eq(gem_table_get_cached(_t815, "type", &_ic_92), gem_string("/"));
        }
        GemVal _t819;
        if (gem_truthy(_t816)) {
                _t819 = _t816;
        } else {
                GemVal _t817 = (*gem_v_peek);
                GemVal _t818 = _t817.fn(_t817.env, NULL, 0);
                static GemICacheSlot _ic_93 = {0};
                _t819 = gem_eq(gem_table_get_cached(_t818, "type", &_ic_93), gem_string("%"));
        }
        if (!gem_truthy(_t819)) break;
#line 439 "compiler/main.gem"
    GemVal _t820 = (*gem_v_advance);
    GemVal _t821 = _t820.fn(_t820.env, NULL, 0);
    static GemICacheSlot _ic_94 = {0};
        GemVal gem_v_op = gem_table_get_cached(_t821, "type", &_ic_94);
#line 440 "compiler/main.gem"
    GemVal _t822 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t822.fn(_t822.env, NULL, 0);
#line 441 "compiler/main.gem"
    GemVal _t823[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t824 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t824.fn(_t824.env, _t823, 3);
    }
    GemVal _t825 = gem_v_left;
    gem_pop_frame();
    return _t825;
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
#line 448 "compiler/main.gem"
    GemVal _t827 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t827.fn(_t827.env, NULL, 0);
#line 449 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t828 = (*gem_v_peek);
        GemVal _t829 = _t828.fn(_t828.env, NULL, 0);
        static GemICacheSlot _ic_95 = {0};
        GemVal _t832;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t829, "type", &_ic_95), gem_string("+")))) {
                _t832 = gem_eq(gem_table_get_cached(_t829, "type", &_ic_95), gem_string("+"));
        } else {
                GemVal _t830 = (*gem_v_peek);
                GemVal _t831 = _t830.fn(_t830.env, NULL, 0);
                static GemICacheSlot _ic_96 = {0};
                _t832 = gem_eq(gem_table_get_cached(_t831, "type", &_ic_96), gem_string("-"));
        }
        if (!gem_truthy(_t832)) break;
#line 450 "compiler/main.gem"
    GemVal _t833 = (*gem_v_advance);
    GemVal _t834 = _t833.fn(_t833.env, NULL, 0);
    static GemICacheSlot _ic_97 = {0};
        GemVal gem_v_op = gem_table_get_cached(_t834, "type", &_ic_97);
#line 451 "compiler/main.gem"
    GemVal _t835 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t835.fn(_t835.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t836[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t837 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t837.fn(_t837.env, _t836, 3);
    }
    GemVal _t838 = gem_v_left;
    gem_pop_frame();
    return _t838;
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
#line 459 "compiler/main.gem"
    GemVal _t840 = (*gem_v_parse_add);
    GemVal gem_v_left = _t840.fn(_t840.env, NULL, 0);
#line 460 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t841 = (*gem_v_peek);
        GemVal _t842 = _t841.fn(_t841.env, NULL, 0);
        static GemICacheSlot _ic_98 = {0};
        GemVal _t845;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t842, "type", &_ic_98), gem_string("==")))) {
                _t845 = gem_eq(gem_table_get_cached(_t842, "type", &_ic_98), gem_string("=="));
        } else {
                GemVal _t843 = (*gem_v_peek);
                GemVal _t844 = _t843.fn(_t843.env, NULL, 0);
                static GemICacheSlot _ic_99 = {0};
                _t845 = gem_eq(gem_table_get_cached(_t844, "type", &_ic_99), gem_string("!="));
        }
        GemVal _t848;
        if (gem_truthy(_t845)) {
                _t848 = _t845;
        } else {
                GemVal _t846 = (*gem_v_peek);
                GemVal _t847 = _t846.fn(_t846.env, NULL, 0);
                static GemICacheSlot _ic_100 = {0};
                _t848 = gem_eq(gem_table_get_cached(_t847, "type", &_ic_100), gem_string("<"));
        }
        GemVal _t851;
        if (gem_truthy(_t848)) {
                _t851 = _t848;
        } else {
                GemVal _t849 = (*gem_v_peek);
                GemVal _t850 = _t849.fn(_t849.env, NULL, 0);
                static GemICacheSlot _ic_101 = {0};
                _t851 = gem_eq(gem_table_get_cached(_t850, "type", &_ic_101), gem_string(">"));
        }
        GemVal _t854;
        if (gem_truthy(_t851)) {
                _t854 = _t851;
        } else {
                GemVal _t852 = (*gem_v_peek);
                GemVal _t853 = _t852.fn(_t852.env, NULL, 0);
                static GemICacheSlot _ic_102 = {0};
                _t854 = gem_eq(gem_table_get_cached(_t853, "type", &_ic_102), gem_string("<="));
        }
        GemVal _t857;
        if (gem_truthy(_t854)) {
                _t857 = _t854;
        } else {
                GemVal _t855 = (*gem_v_peek);
                GemVal _t856 = _t855.fn(_t855.env, NULL, 0);
                static GemICacheSlot _ic_103 = {0};
                _t857 = gem_eq(gem_table_get_cached(_t856, "type", &_ic_103), gem_string(">="));
        }
        GemVal _t860;
        if (gem_truthy(_t857)) {
                _t860 = _t857;
        } else {
                GemVal _t858 = (*gem_v_peek);
                GemVal _t859 = _t858.fn(_t858.env, NULL, 0);
                static GemICacheSlot _ic_104 = {0};
                _t860 = gem_eq(gem_table_get_cached(_t859, "type", &_ic_104), gem_string("in"));
        }
        if (!gem_truthy(_t860)) break;
#line 461 "compiler/main.gem"
    GemVal _t861 = (*gem_v_advance);
    GemVal _t862 = _t861.fn(_t861.env, NULL, 0);
    static GemICacheSlot _ic_105 = {0};
        GemVal gem_v_op = gem_table_get_cached(_t862, "type", &_ic_105);
#line 462 "compiler/main.gem"
    GemVal _t863 = (*gem_v_parse_add);
        GemVal gem_v_right = _t863.fn(_t863.env, NULL, 0);
#line 463 "compiler/main.gem"
    GemVal _t864[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t865 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t865.fn(_t865.env, _t864, 3);
    }
    GemVal _t866 = gem_v_left;
    gem_pop_frame();
    return _t866;
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
#line 470 "compiler/main.gem"
    GemVal _t868 = (*gem_v_peek);
    GemVal _t869 = _t868.fn(_t868.env, NULL, 0);
    static GemICacheSlot _ic_106 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t869, "type", &_ic_106), gem_string("not")))) {
#line 471 "compiler/main.gem"
    GemVal _t870 = (*gem_v_advance);
        (void)(_t870.fn(_t870.env, NULL, 0));
#line 472 "compiler/main.gem"
    GemVal _t871 = (*gem_v_parse_not);
    GemVal _t872[] = {gem_string("not"), _t871.fn(_t871.env, NULL, 0)};
    GemVal _t873 = (*gem_v__mod_parser_make_unop);
        GemVal _t874 = _t873.fn(_t873.env, _t872, 2);
        gem_pop_frame();
        return _t874;
    }
#line 474 "compiler/main.gem"
    GemVal _t875 = (*gem_v_parse_compare);
    GemVal _t876 = _t875.fn(_t875.env, NULL, 0);
    gem_pop_frame();
    return _t876;
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
#line 479 "compiler/main.gem"
    GemVal _t878 = (*gem_v_parse_not);
    GemVal gem_v_left = _t878.fn(_t878.env, NULL, 0);
#line 480 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t879 = (*gem_v_peek);
        GemVal _t880 = _t879.fn(_t879.env, NULL, 0);
        static GemICacheSlot _ic_107 = {0};
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t880, "type", &_ic_107), gem_string("and")))) break;
#line 481 "compiler/main.gem"
    GemVal _t881 = (*gem_v_advance);
        (void)(_t881.fn(_t881.env, NULL, 0));
#line 482 "compiler/main.gem"
    GemVal _t882 = (*gem_v_parse_not);
        GemVal gem_v_right = _t882.fn(_t882.env, NULL, 0);
#line 483 "compiler/main.gem"
    GemVal _t883[] = {gem_string("and"), gem_v_left, gem_v_right};
    GemVal _t884 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t884.fn(_t884.env, _t883, 3);
    }
    GemVal _t885 = gem_v_left;
    gem_pop_frame();
    return _t885;
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
#line 490 "compiler/main.gem"
    GemVal _t887 = (*gem_v_parse_and);
    GemVal gem_v_left = _t887.fn(_t887.env, NULL, 0);
#line 491 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t888 = (*gem_v_peek);
        GemVal _t889 = _t888.fn(_t888.env, NULL, 0);
        static GemICacheSlot _ic_108 = {0};
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t889, "type", &_ic_108), gem_string("or")))) break;
#line 492 "compiler/main.gem"
    GemVal _t890 = (*gem_v_advance);
        (void)(_t890.fn(_t890.env, NULL, 0));
#line 493 "compiler/main.gem"
    GemVal _t891 = (*gem_v_parse_and);
        GemVal gem_v_right = _t891.fn(_t891.env, NULL, 0);
#line 494 "compiler/main.gem"
    GemVal _t892[] = {gem_string("or"), gem_v_left, gem_v_right};
    GemVal _t893 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t893.fn(_t893.env, _t892, 3);
    }
    GemVal _t894 = gem_v_left;
    gem_pop_frame();
    return _t894;
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
#line 501 "compiler/main.gem"
    GemVal _t896 = (*gem_v_peek);
    GemVal _t897 = _t896.fn(_t896.env, NULL, 0);
    static GemICacheSlot _ic_109 = {0};
    GemVal gem_v_start_line = gem_table_get_cached(_t897, "line", &_ic_109);
#line 502 "compiler/main.gem"
    GemVal _t898 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t898.fn(_t898.env, NULL, 0);
#line 505 "compiler/main.gem"
    GemVal _t899 = (*gem_v_peek);
    GemVal _t900 = _t899.fn(_t899.env, NULL, 0);
    static GemICacheSlot _ic_110 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t900, "type", &_ic_110), gem_string("=")))) {
#line 506 "compiler/main.gem"
    GemVal _t901 = (*gem_v_advance);
        (void)(_t901.fn(_t901.env, NULL, 0));
#line 507 "compiler/main.gem"
    GemVal _t902 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t902.fn(_t902.env, NULL, 0);
#line 508 "compiler/main.gem"
    GemVal _t903 = gem_v_lhs;
    static GemICacheSlot _ic_111 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t903, "tag", &_ic_111), gem_string("var")))) {
#line 509 "compiler/main.gem"
    GemVal _t904 = gem_v_lhs;
    static GemICacheSlot _ic_112 = {0};
    GemVal _t905[] = {gem_table_get_cached(_t904, "name", &_ic_112), gem_v_value, gem_v_start_line};
    GemVal _t906 = (*gem_v__mod_parser_make_assign);
            GemVal _t907 = _t906.fn(_t906.env, _t905, 3);
            gem_pop_frame();
            return _t907;
        }
#line 511 "compiler/main.gem"
    GemVal _t908 = gem_v_lhs;
    static GemICacheSlot _ic_113 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t908, "tag", &_ic_113), gem_string("dot")))) {
#line 512 "compiler/main.gem"
    GemVal _t909 = gem_v_lhs;
    static GemICacheSlot _ic_114 = {0};
    GemVal _t910 = gem_v_lhs;
    static GemICacheSlot _ic_115 = {0};
    GemVal _t911[] = {gem_table_get_cached(_t909, "object", &_ic_114), gem_table_get_cached(_t910, "field", &_ic_115), gem_v_value, gem_v_start_line};
    GemVal _t912 = (*gem_v__mod_parser_make_dot_assign);
            GemVal _t913 = _t912.fn(_t912.env, _t911, 4);
            gem_pop_frame();
            return _t913;
        }
#line 514 "compiler/main.gem"
    GemVal _t914 = gem_v_lhs;
    static GemICacheSlot _ic_116 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t914, "tag", &_ic_116), gem_string("index")))) {
#line 515 "compiler/main.gem"
    GemVal _t915 = gem_v_lhs;
    static GemICacheSlot _ic_117 = {0};
    GemVal _t916 = gem_v_lhs;
    static GemICacheSlot _ic_118 = {0};
    GemVal _t917[] = {gem_table_get_cached(_t915, "object", &_ic_117), gem_table_get_cached(_t916, "key", &_ic_118), gem_v_value, gem_v_start_line};
    GemVal _t918 = (*gem_v__mod_parser_make_index_assign);
            GemVal _t919 = _t918.fn(_t918.env, _t917, 4);
            gem_pop_frame();
            return _t919;
        }
#line 517 "compiler/main.gem"
    GemVal _t920[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
    GemVal _t921 = (*gem_v__mod_parser_compile_error);
        (void)(_t921.fn(_t921.env, _t920, 7));
    }
#line 521 "compiler/main.gem"
    GemVal _t922 = (*gem_v_peek);
    GemVal _t923 = _t922.fn(_t922.env, NULL, 0);
    static GemICacheSlot _ic_119 = {0};
    GemVal _t926;
    if (gem_truthy(gem_eq(gem_table_get_cached(_t923, "type", &_ic_119), gem_string("+=")))) {
        _t926 = gem_eq(gem_table_get_cached(_t923, "type", &_ic_119), gem_string("+="));
    } else {
        GemVal _t924 = (*gem_v_peek);
        GemVal _t925 = _t924.fn(_t924.env, NULL, 0);
        static GemICacheSlot _ic_120 = {0};
        _t926 = gem_eq(gem_table_get_cached(_t925, "type", &_ic_120), gem_string("-="));
    }
    GemVal _t929;
    if (gem_truthy(_t926)) {
        _t929 = _t926;
    } else {
        GemVal _t927 = (*gem_v_peek);
        GemVal _t928 = _t927.fn(_t927.env, NULL, 0);
        static GemICacheSlot _ic_121 = {0};
        _t929 = gem_eq(gem_table_get_cached(_t928, "type", &_ic_121), gem_string("*="));
    }
    GemVal _t932;
    if (gem_truthy(_t929)) {
        _t932 = _t929;
    } else {
        GemVal _t930 = (*gem_v_peek);
        GemVal _t931 = _t930.fn(_t930.env, NULL, 0);
        static GemICacheSlot _ic_122 = {0};
        _t932 = gem_eq(gem_table_get_cached(_t931, "type", &_ic_122), gem_string("/="));
    }
    if (gem_truthy(_t932)) {
#line 522 "compiler/main.gem"
    GemVal _t933 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t933.fn(_t933.env, NULL, 0);
#line 523 "compiler/main.gem"
    GemVal _t934 = gem_v_op_tok;
    static GemICacheSlot _ic_123 = {0};
        GemVal gem_v_base_op = gem_table_get(gem_table_get_cached(_t934, "value", &_ic_123), gem_int(0));
#line 524 "compiler/main.gem"
    GemVal _t935 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t935.fn(_t935.env, NULL, 0);
#line 525 "compiler/main.gem"
    GemVal _t936 = gem_v_lhs;
    static GemICacheSlot _ic_124 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t936, "tag", &_ic_124), gem_string("var")))) {
#line 526 "compiler/main.gem"
    GemVal _t937 = gem_v_op_tok;
    static GemICacheSlot _ic_125 = {0};
    GemVal _t938 = gem_v_op_tok;
    static GemICacheSlot _ic_126 = {0};
    GemVal _t939 = gem_v_op_tok;
    static GemICacheSlot _ic_127 = {0};
    GemVal _t940[] = {gem_table_get_cached(_t939, "value", &_ic_127)};
    GemVal _t941[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t937, "line", &_ic_125), gem_table_get_cached(_t938, "col", &_ic_126), gem_len_fn(NULL, _t940, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
    GemVal _t942 = (*gem_v__mod_parser_compile_error);
            (void)(_t942.fn(_t942.env, _t941, 7));
        }
#line 528 "compiler/main.gem"
    GemVal _t943 = gem_v_lhs;
    static GemICacheSlot _ic_128 = {0};
    GemVal _t944 = gem_v_lhs;
    static GemICacheSlot _ic_129 = {0};
    GemVal _t945[] = {gem_table_get_cached(_t944, "name", &_ic_129)};
    GemVal _t946 = (*gem_v__mod_parser_make_var);
    GemVal _t947[] = {gem_v_base_op, _t946.fn(_t946.env, _t945, 1), gem_v_value};
    GemVal _t948 = (*gem_v__mod_parser_make_binop);
    GemVal _t949[] = {gem_table_get_cached(_t943, "name", &_ic_128), _t948.fn(_t948.env, _t947, 3), gem_v_start_line};
    GemVal _t950 = (*gem_v__mod_parser_make_assign);
        GemVal _t951 = _t950.fn(_t950.env, _t949, 3);
        gem_pop_frame();
        return _t951;
    }
    GemVal _t952 = gem_v_lhs;
    gem_pop_frame();
    return _t952;
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
#line 541 "compiler/main.gem"
    GemVal _t954[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t954, 1), gem_int(0)))) {
#line 542 "compiler/main.gem"
    GemVal _t955[] = {gem_bool(1)};
    GemVal _t956 = (*gem_v__mod_parser_make_bool);
        GemVal _t957 = _t956.fn(_t956.env, _t955, 1);
        gem_pop_frame();
        return _t957;
    }
#line 544 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 545 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(1);
#line 545 "compiler/main.gem"
    GemVal _t958[] = {gem_v_conditions};
    GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t958, 1);
#line 545 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 545 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_4;
#line 545 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 546 "compiler/main.gem"
    GemVal _t959[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
    GemVal _t960 = (*gem_v__mod_parser_make_binop);
        gem_v_result = _t960.fn(_t960.env, _t959, 3);
    }

    GemVal _t961 = gem_v_result;
    gem_pop_frame();
    return _t961;
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
#line 540 "compiler/main.gem"
    struct _closure__anon_21 *_t962 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t962->gem_v__mod_parser_make_binop = gem_v__mod_parser_make_binop;
    _t962->gem_v__mod_parser_make_bool = gem_v__mod_parser_make_bool;
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, _t962);
#line 550 "compiler/main.gem"
    GemVal _t963 = (*gem_v_peek);
    GemVal gem_v_t = _t963.fn(_t963.env, NULL, 0);
#line 553 "compiler/main.gem"
    GemVal _t964 = gem_v_t;
    static GemICacheSlot _ic_130 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t964, "type", &_ic_130), gem_string("{")))) {
#line 554 "compiler/main.gem"
    GemVal _t965 = (*gem_v_advance);
        (void)(_t965.fn(_t965.env, NULL, 0));
#line 555 "compiler/main.gem"
    GemVal _t966 = (*gem_v_skip_nl);
        (void)(_t966.fn(_t966.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t967 = gem_table_new();
    GemVal _t968[] = {gem_string("type")};
    GemVal _t969 = (*gem_v__mod_parser_make_var);
    GemVal _t970 = gem_table_new();
    gem_table_set(_t970, gem_int(0), gem_v_target_expr);
    GemVal _t971[] = {_t969.fn(_t969.env, _t968, 1), _t970, gem_int(0)};
    GemVal _t972 = (*gem_v__mod_parser_make_call);
    GemVal _t973[] = {gem_string("table")};
    GemVal _t974 = (*gem_v__mod_parser_make_string);
    GemVal _t975[] = {gem_string("=="), _t972.fn(_t972.env, _t971, 3), _t974.fn(_t974.env, _t973, 1)};
    GemVal _t976 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t967, gem_int(0), _t976.fn(_t976.env, _t975, 3));
        GemVal gem_v_conditions = _t967;
#line 557 "compiler/main.gem"
    GemVal _t977 = gem_table_new();
        GemVal gem_v_bindings = _t977;
#line 558 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t978 = (*gem_v_peek);
            GemVal _t979 = _t978.fn(_t978.env, NULL, 0);
            static GemICacheSlot _ic_131 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t979, "type", &_ic_131), gem_string("}")))) break;
#line 559 "compiler/main.gem"
    GemVal _t980 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t980.fn(_t980.env, NULL, 0);
#line 560 "compiler/main.gem"
    GemVal _t981 = gem_v_key_tok;
    static GemICacheSlot _ic_132 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t981, "type", &_ic_132), gem_string("NAME")))) {
#line 561 "compiler/main.gem"
    GemVal _t982 = (*gem_v_advance);
                (void)(_t982.fn(_t982.env, NULL, 0));
            } else {
#line 562 "compiler/main.gem"
    GemVal _t983[] = {gem_int(1)};
    GemVal _t984 = (*gem_v_peek_at);
    GemVal _t985 = _t984.fn(_t984.env, _t983, 1);
    static GemICacheSlot _ic_133 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t985, "type", &_ic_133), gem_string(":")))) {
#line 564 "compiler/main.gem"
    GemVal _t986 = (*gem_v_advance);
                    (void)(_t986.fn(_t986.env, NULL, 0));
                } else {
#line 566 "compiler/main.gem"
    GemVal _t987 = gem_v_key_tok;
    static GemICacheSlot _ic_134 = {0};
    GemVal _t988 = gem_v_key_tok;
    static GemICacheSlot _ic_135 = {0};
    GemVal _t989 = gem_v_key_tok;
    static GemICacheSlot _ic_136 = {0};
    GemVal _t990[] = {gem_table_get_cached(_t989, "value", &_ic_136)};
    GemVal _t991[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t987, "line", &_ic_134), gem_table_get_cached(_t988, "col", &_ic_135), gem_len_fn(NULL, _t990, 1), gem_string("expected key name in table pattern"), GEM_NIL};
    GemVal _t992 = (*gem_v__mod_parser_compile_error);
                    (void)(_t992.fn(_t992.env, _t991, 7));
                }
            }
#line 568 "compiler/main.gem"
    GemVal _t993 = gem_v_key_tok;
    static GemICacheSlot _ic_137 = {0};
            GemVal gem_v_key_name = gem_table_get_cached(_t993, "value", &_ic_137);
#line 569 "compiler/main.gem"
    GemVal _t994[] = {gem_v_key_name};
    GemVal _t995 = (*gem_v__mod_parser_make_string);
            GemVal gem_v_key_str = _t995.fn(_t995.env, _t994, 1);
#line 570 "compiler/main.gem"
    GemVal _t996[] = {gem_string("has_key")};
    GemVal _t997 = (*gem_v__mod_parser_make_var);
    GemVal _t998 = gem_table_new();
    gem_table_set(_t998, gem_int(0), gem_v_target_expr);
    gem_table_set(_t998, gem_int(1), gem_v_key_str);
    GemVal _t999[] = {_t997.fn(_t997.env, _t996, 1), _t998, gem_int(0)};
    GemVal _t1000 = (*gem_v__mod_parser_make_call);
    GemVal _t1001[] = {gem_v_conditions, _t1000.fn(_t1000.env, _t999, 3)};
            (void)(gem_push_fn(NULL, _t1001, 2));
#line 571 "compiler/main.gem"
    GemVal _t1002[] = {gem_v_target_expr, gem_v_key_str};
    GemVal _t1003 = (*gem_v__mod_parser_make_index);
            GemVal gem_v_sub_target = _t1003.fn(_t1003.env, _t1002, 2);
#line 572 "compiler/main.gem"
    GemVal _t1004 = (*gem_v_peek);
    GemVal _t1005 = _t1004.fn(_t1004.env, NULL, 0);
    static GemICacheSlot _ic_138 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1005, "type", &_ic_138), gem_string(":")))) {
#line 573 "compiler/main.gem"
    GemVal _t1006 = (*gem_v_advance);
                (void)(_t1006.fn(_t1006.env, NULL, 0));
#line 574 "compiler/main.gem"
    GemVal _t1007 = (*gem_v_skip_nl);
                (void)(_t1007.fn(_t1007.env, NULL, 0));
#line 575 "compiler/main.gem"
    GemVal _t1008[] = {gem_v_sub_target};
    GemVal _t1009 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t1009.fn(_t1009.env, _t1008, 1);
#line 576 "compiler/main.gem"
    GemVal _t1010 = gem_v_sub;
    static GemICacheSlot _ic_139 = {0};
    GemVal _t1011 = gem_table_get_cached(_t1010, "condition", &_ic_139);
    static GemICacheSlot _ic_140 = {0};
    GemVal _t1014;
    if (gem_truthy(gem_neq(gem_table_get_cached(_t1011, "tag", &_ic_140), gem_string("bool")))) {
        _t1014 = gem_neq(gem_table_get_cached(_t1011, "tag", &_ic_140), gem_string("bool"));
    } else {
        GemVal _t1012 = gem_v_sub;
        static GemICacheSlot _ic_141 = {0};
        GemVal _t1013 = gem_table_get_cached(_t1012, "condition", &_ic_141);
        static GemICacheSlot _ic_142 = {0};
        _t1014 = gem_neq(gem_table_get_cached(_t1013, "value", &_ic_142), gem_bool(1));
    }
                if (gem_truthy(_t1014)) {
#line 577 "compiler/main.gem"
    GemVal _t1015 = gem_v_sub;
    static GemICacheSlot _ic_143 = {0};
    GemVal _t1016[] = {gem_v_conditions, gem_table_get_cached(_t1015, "condition", &_ic_143)};
                    (void)(gem_push_fn(NULL, _t1016, 2));
                }
#line 579 "compiler/main.gem"
    GemVal _t1017 = gem_v_sub;
    static GemICacheSlot _ic_144 = {0};
                GemVal gem_v__for_items_5 = gem_table_get_cached(_t1017, "bindings", &_ic_144);
#line 579 "compiler/main.gem"
                GemVal gem_v__for_i_5 = gem_int(0);
#line 579 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1018[] = {gem_v__for_items_5};
                    if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1018, 1)))) break;
#line 579 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 579 "compiler/main.gem"
                    gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 580 "compiler/main.gem"
    GemVal _t1019[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t1019, 2));
                }

            } else {
#line 584 "compiler/main.gem"
    GemVal _t1020[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t1021 = (*gem_v__mod_parser_make_let);
    GemVal _t1022[] = {gem_v_bindings, _t1021.fn(_t1021.env, _t1020, 3)};
                (void)(gem_push_fn(NULL, _t1022, 2));
            }
#line 586 "compiler/main.gem"
    GemVal _t1023 = (*gem_v_skip_nl);
            (void)(_t1023.fn(_t1023.env, NULL, 0));
#line 587 "compiler/main.gem"
    GemVal _t1024 = (*gem_v_peek);
    GemVal _t1025 = _t1024.fn(_t1024.env, NULL, 0);
    static GemICacheSlot _ic_145 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1025, "type", &_ic_145), gem_string(",")))) {
#line 588 "compiler/main.gem"
    GemVal _t1026 = (*gem_v_advance);
                (void)(_t1026.fn(_t1026.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t1027 = (*gem_v_skip_nl);
                (void)(_t1027.fn(_t1027.env, NULL, 0));
            }
        }
#line 592 "compiler/main.gem"
    GemVal _t1028[] = {gem_string("}")};
    GemVal _t1029 = (*gem_v_expect);
        (void)(_t1029.fn(_t1029.env, _t1028, 1));
#line 593 "compiler/main.gem"
    GemVal _t1030 = gem_table_new();
    GemVal _t1031[] = {gem_v_conditions};
    GemVal _t1032 = gem_v_and_chain;
    gem_table_set(_t1030, gem_string("condition"), _t1032.fn(_t1032.env, _t1031, 1));
    gem_table_set(_t1030, gem_string("bindings"), gem_v_bindings);
        GemVal _t1033 = _t1030;
        gem_pop_frame();
        return _t1033;
    }
#line 597 "compiler/main.gem"
    GemVal _t1034 = gem_v_t;
    static GemICacheSlot _ic_146 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1034, "type", &_ic_146), gem_string("[")))) {
#line 598 "compiler/main.gem"
    GemVal _t1035 = (*gem_v_advance);
        (void)(_t1035.fn(_t1035.env, NULL, 0));
#line 599 "compiler/main.gem"
    GemVal _t1036 = (*gem_v_skip_nl);
        (void)(_t1036.fn(_t1036.env, NULL, 0));
#line 600 "compiler/main.gem"
    GemVal _t1037 = gem_table_new();
        GemVal gem_v_sub_patterns = _t1037;
#line 601 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 602 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1038 = (*gem_v_peek);
            GemVal _t1039 = _t1038.fn(_t1038.env, NULL, 0);
            static GemICacheSlot _ic_147 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t1039, "type", &_ic_147), gem_string("]")))) break;
#line 603 "compiler/main.gem"
    GemVal _t1040[] = {gem_v_idx};
    GemVal _t1041 = (*gem_v__mod_parser_make_int);
    GemVal _t1042[] = {gem_v_target_expr, _t1041.fn(_t1041.env, _t1040, 1)};
    GemVal _t1043 = (*gem_v__mod_parser_make_index);
    GemVal _t1044[] = {_t1043.fn(_t1043.env, _t1042, 2)};
    GemVal _t1045 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t1045.fn(_t1045.env, _t1044, 1);
#line 604 "compiler/main.gem"
    GemVal _t1046[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t1046, 2));
#line 605 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 606 "compiler/main.gem"
    GemVal _t1047 = (*gem_v_skip_nl);
            (void)(_t1047.fn(_t1047.env, NULL, 0));
#line 607 "compiler/main.gem"
    GemVal _t1048 = (*gem_v_peek);
    GemVal _t1049 = _t1048.fn(_t1048.env, NULL, 0);
    static GemICacheSlot _ic_148 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1049, "type", &_ic_148), gem_string(",")))) {
#line 608 "compiler/main.gem"
    GemVal _t1050 = (*gem_v_advance);
                (void)(_t1050.fn(_t1050.env, NULL, 0));
#line 609 "compiler/main.gem"
    GemVal _t1051 = (*gem_v_skip_nl);
                (void)(_t1051.fn(_t1051.env, NULL, 0));
            }
        }
#line 612 "compiler/main.gem"
    GemVal _t1052[] = {gem_string("]")};
    GemVal _t1053 = (*gem_v_expect);
        (void)(_t1053.fn(_t1053.env, _t1052, 1));
#line 613 "compiler/main.gem"
    GemVal _t1054 = gem_table_new();
    GemVal _t1055[] = {gem_string("type")};
    GemVal _t1056 = (*gem_v__mod_parser_make_var);
    GemVal _t1057 = gem_table_new();
    gem_table_set(_t1057, gem_int(0), gem_v_target_expr);
    GemVal _t1058[] = {_t1056.fn(_t1056.env, _t1055, 1), _t1057, gem_int(0)};
    GemVal _t1059 = (*gem_v__mod_parser_make_call);
    GemVal _t1060[] = {gem_string("table")};
    GemVal _t1061 = (*gem_v__mod_parser_make_string);
    GemVal _t1062[] = {gem_string("=="), _t1059.fn(_t1059.env, _t1058, 3), _t1061.fn(_t1061.env, _t1060, 1)};
    GemVal _t1063 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1054, gem_int(0), _t1063.fn(_t1063.env, _t1062, 3));
        GemVal gem_v_conditions = _t1054;
#line 614 "compiler/main.gem"
    GemVal _t1064[] = {gem_string("len")};
    GemVal _t1065 = (*gem_v__mod_parser_make_var);
    GemVal _t1066 = gem_table_new();
    gem_table_set(_t1066, gem_int(0), gem_v_target_expr);
    GemVal _t1067[] = {_t1065.fn(_t1065.env, _t1064, 1), _t1066, gem_int(0)};
    GemVal _t1068 = (*gem_v__mod_parser_make_call);
    GemVal _t1069[] = {gem_v_idx};
    GemVal _t1070 = (*gem_v__mod_parser_make_int);
    GemVal _t1071[] = {gem_string("=="), _t1068.fn(_t1068.env, _t1067, 3), _t1070.fn(_t1070.env, _t1069, 1)};
    GemVal _t1072 = (*gem_v__mod_parser_make_binop);
    GemVal _t1073[] = {gem_v_conditions, _t1072.fn(_t1072.env, _t1071, 3)};
        (void)(gem_push_fn(NULL, _t1073, 2));
#line 615 "compiler/main.gem"
    GemVal _t1074 = gem_table_new();
        GemVal gem_v_bindings = _t1074;
#line 616 "compiler/main.gem"
        GemVal gem_v__for_i_7 = gem_int(0);
#line 616 "compiler/main.gem"
    GemVal _t1075[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1075, 1);
#line 616 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_v__for_limit_7))) break;
#line 616 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_7;
#line 616 "compiler/main.gem"
            gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 617 "compiler/main.gem"
            GemVal gem_v_sub = gem_table_get(gem_v_sub_patterns, gem_v_i);
#line 618 "compiler/main.gem"
    GemVal _t1076 = gem_v_sub;
    static GemICacheSlot _ic_149 = {0};
    GemVal _t1077 = gem_table_get_cached(_t1076, "condition", &_ic_149);
    static GemICacheSlot _ic_150 = {0};
    GemVal _t1080;
    if (gem_truthy(gem_neq(gem_table_get_cached(_t1077, "tag", &_ic_150), gem_string("bool")))) {
        _t1080 = gem_neq(gem_table_get_cached(_t1077, "tag", &_ic_150), gem_string("bool"));
    } else {
        GemVal _t1078 = gem_v_sub;
        static GemICacheSlot _ic_151 = {0};
        GemVal _t1079 = gem_table_get_cached(_t1078, "condition", &_ic_151);
        static GemICacheSlot _ic_152 = {0};
        _t1080 = gem_neq(gem_table_get_cached(_t1079, "value", &_ic_152), gem_bool(1));
    }
            if (gem_truthy(_t1080)) {
#line 619 "compiler/main.gem"
    GemVal _t1081 = gem_v_sub;
    static GemICacheSlot _ic_153 = {0};
    GemVal _t1082[] = {gem_v_conditions, gem_table_get_cached(_t1081, "condition", &_ic_153)};
                (void)(gem_push_fn(NULL, _t1082, 2));
            }
#line 621 "compiler/main.gem"
    GemVal _t1083 = gem_v_sub;
    static GemICacheSlot _ic_154 = {0};
            GemVal gem_v__for_items_6 = gem_table_get_cached(_t1083, "bindings", &_ic_154);
#line 621 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 621 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1084[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1084, 1)))) break;
#line 621 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 621 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 622 "compiler/main.gem"
    GemVal _t1085[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t1085, 2));
            }

        }

#line 625 "compiler/main.gem"
    GemVal _t1086 = gem_table_new();
    GemVal _t1087[] = {gem_v_conditions};
    GemVal _t1088 = gem_v_and_chain;
    gem_table_set(_t1086, gem_string("condition"), _t1088.fn(_t1088.env, _t1087, 1));
    gem_table_set(_t1086, gem_string("bindings"), gem_v_bindings);
        GemVal _t1089 = _t1086;
        gem_pop_frame();
        return _t1089;
    }
#line 629 "compiler/main.gem"
    GemVal _t1090 = gem_v_t;
    static GemICacheSlot _ic_155 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1090, "type", &_ic_155), gem_string("NUMBER")))) {
#line 630 "compiler/main.gem"
    GemVal _t1091 = (*gem_v_advance);
        (void)(_t1091.fn(_t1091.env, NULL, 0));
#line 631 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 632 "compiler/main.gem"
    GemVal _t1092 = gem_v_t;
    static GemICacheSlot _ic_156 = {0};
    GemVal _t1093[] = {gem_table_get_cached(_t1092, "value", &_ic_156)};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t1093, 1))) {
#line 633 "compiler/main.gem"
    GemVal _t1094 = gem_v_t;
    static GemICacheSlot _ic_157 = {0};
    GemVal _t1095[] = {gem_table_get_cached(_t1094, "value", &_ic_157)};
    GemVal _t1096[] = {gem_fn_atof(NULL, _t1095, 1)};
    GemVal _t1097 = (*gem_v__mod_parser_make_float);
            gem_v_lit = _t1097.fn(_t1097.env, _t1096, 1);
        } else {
#line 635 "compiler/main.gem"
    GemVal _t1098 = gem_v_t;
    static GemICacheSlot _ic_158 = {0};
    GemVal _t1099[] = {gem_table_get_cached(_t1098, "value", &_ic_158)};
    GemVal _t1100[] = {gem_fn__mod_parser_str_to_int(NULL, _t1099, 1)};
    GemVal _t1101 = (*gem_v__mod_parser_make_int);
            gem_v_lit = _t1101.fn(_t1101.env, _t1100, 1);
        }
#line 637 "compiler/main.gem"
    GemVal _t1102 = gem_table_new();
    GemVal _t1103[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    GemVal _t1104 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1102, gem_string("condition"), _t1104.fn(_t1104.env, _t1103, 3));
    GemVal _t1105 = gem_table_new();
    gem_table_set(_t1102, gem_string("bindings"), _t1105);
        GemVal _t1106 = _t1102;
        gem_pop_frame();
        return _t1106;
    }
#line 639 "compiler/main.gem"
    GemVal _t1107 = gem_v_t;
    static GemICacheSlot _ic_159 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1107, "type", &_ic_159), gem_string("STRING")))) {
#line 640 "compiler/main.gem"
    GemVal _t1108 = (*gem_v_advance);
        (void)(_t1108.fn(_t1108.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t1109 = gem_table_new();
    GemVal _t1110 = gem_v_t;
    static GemICacheSlot _ic_160 = {0};
    GemVal _t1111[] = {gem_table_get_cached(_t1110, "value", &_ic_160)};
    GemVal _t1112 = (*gem_v__mod_parser_make_string);
    GemVal _t1113[] = {gem_string("=="), gem_v_target_expr, _t1112.fn(_t1112.env, _t1111, 1)};
    GemVal _t1114 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1109, gem_string("condition"), _t1114.fn(_t1114.env, _t1113, 3));
    GemVal _t1115 = gem_table_new();
    gem_table_set(_t1109, gem_string("bindings"), _t1115);
        GemVal _t1116 = _t1109;
        gem_pop_frame();
        return _t1116;
    }
#line 643 "compiler/main.gem"
    GemVal _t1117 = gem_v_t;
    static GemICacheSlot _ic_161 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1117, "type", &_ic_161), gem_string("true")))) {
#line 644 "compiler/main.gem"
    GemVal _t1118 = (*gem_v_advance);
        (void)(_t1118.fn(_t1118.env, NULL, 0));
#line 645 "compiler/main.gem"
    GemVal _t1119 = gem_table_new();
    GemVal _t1120[] = {gem_bool(1)};
    GemVal _t1121 = (*gem_v__mod_parser_make_bool);
    GemVal _t1122[] = {gem_string("=="), gem_v_target_expr, _t1121.fn(_t1121.env, _t1120, 1)};
    GemVal _t1123 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1119, gem_string("condition"), _t1123.fn(_t1123.env, _t1122, 3));
    GemVal _t1124 = gem_table_new();
    gem_table_set(_t1119, gem_string("bindings"), _t1124);
        GemVal _t1125 = _t1119;
        gem_pop_frame();
        return _t1125;
    }
#line 647 "compiler/main.gem"
    GemVal _t1126 = gem_v_t;
    static GemICacheSlot _ic_162 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1126, "type", &_ic_162), gem_string("false")))) {
#line 648 "compiler/main.gem"
    GemVal _t1127 = (*gem_v_advance);
        (void)(_t1127.fn(_t1127.env, NULL, 0));
#line 649 "compiler/main.gem"
    GemVal _t1128 = gem_table_new();
    GemVal _t1129[] = {gem_bool(0)};
    GemVal _t1130 = (*gem_v__mod_parser_make_bool);
    GemVal _t1131[] = {gem_string("=="), gem_v_target_expr, _t1130.fn(_t1130.env, _t1129, 1)};
    GemVal _t1132 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1128, gem_string("condition"), _t1132.fn(_t1132.env, _t1131, 3));
    GemVal _t1133 = gem_table_new();
    gem_table_set(_t1128, gem_string("bindings"), _t1133);
        GemVal _t1134 = _t1128;
        gem_pop_frame();
        return _t1134;
    }
#line 651 "compiler/main.gem"
    GemVal _t1135 = gem_v_t;
    static GemICacheSlot _ic_163 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1135, "type", &_ic_163), gem_string("nil")))) {
#line 652 "compiler/main.gem"
    GemVal _t1136 = (*gem_v_advance);
        (void)(_t1136.fn(_t1136.env, NULL, 0));
#line 653 "compiler/main.gem"
    GemVal _t1137 = gem_table_new();
    GemVal _t1138 = (*gem_v__mod_parser_make_nil_node);
    GemVal _t1139[] = {gem_string("=="), gem_v_target_expr, _t1138.fn(_t1138.env, NULL, 0)};
    GemVal _t1140 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1137, gem_string("condition"), _t1140.fn(_t1140.env, _t1139, 3));
    GemVal _t1141 = gem_table_new();
    gem_table_set(_t1137, gem_string("bindings"), _t1141);
        GemVal _t1142 = _t1137;
        gem_pop_frame();
        return _t1142;
    }
#line 657 "compiler/main.gem"
    GemVal _t1143 = gem_v_t;
    static GemICacheSlot _ic_164 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1143, "type", &_ic_164), gem_string("NAME")))) {
#line 658 "compiler/main.gem"
    GemVal _t1144 = (*gem_v_advance);
        (void)(_t1144.fn(_t1144.env, NULL, 0));
#line 659 "compiler/main.gem"
    GemVal _t1145 = gem_table_new();
    GemVal _t1146[] = {gem_bool(1)};
    GemVal _t1147 = (*gem_v__mod_parser_make_bool);
    gem_table_set(_t1145, gem_string("condition"), _t1147.fn(_t1147.env, _t1146, 1));
    GemVal _t1148 = gem_table_new();
    GemVal _t1149 = gem_v_t;
    static GemICacheSlot _ic_165 = {0};
    GemVal _t1150[] = {gem_table_get_cached(_t1149, "value", &_ic_165), gem_v_target_expr, gem_int(0)};
    GemVal _t1151 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1148, gem_int(0), _t1151.fn(_t1151.env, _t1150, 3));
    gem_table_set(_t1145, gem_string("bindings"), _t1148);
        GemVal _t1152 = _t1145;
        gem_pop_frame();
        return _t1152;
    }
#line 662 "compiler/main.gem"
    GemVal _t1153 = gem_v_t;
    static GemICacheSlot _ic_166 = {0};
    GemVal _t1154 = gem_v_t;
    static GemICacheSlot _ic_167 = {0};
    GemVal _t1155 = gem_v_t;
    static GemICacheSlot _ic_168 = {0};
    GemVal _t1156[] = {gem_table_get_cached(_t1155, "value", &_ic_168)};
    GemVal _t1157 = gem_v_t;
    static GemICacheSlot _ic_169 = {0};
    GemVal _t1158[] = {gem_string("expected pattern but got '"), gem_table_get_cached(_t1157, "type", &_ic_169), gem_string("'")};
    GemVal _t1159[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1153, "line", &_ic_166), gem_table_get_cached(_t1154, "col", &_ic_167), gem_len_fn(NULL, _t1156, 1), gem_interp(3, _t1158), GEM_NIL};
    GemVal _t1160 = (*gem_v__mod_parser_compile_error);
    GemVal _t1161 = _t1160.fn(_t1160.env, _t1159, 7);
    gem_pop_frame();
    return _t1161;
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
#line 668 "compiler/main.gem"
    GemVal _t1163 = (*gem_v_peek);
    GemVal gem_v_t = _t1163.fn(_t1163.env, NULL, 0);
#line 671 "compiler/main.gem"
    GemVal _t1164 = gem_v_t;
    static GemICacheSlot _ic_170 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1164, "type", &_ic_170), gem_string("let")))) {
#line 672 "compiler/main.gem"
    GemVal _t1165 = gem_v_t;
    static GemICacheSlot _ic_171 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1165, "line", &_ic_171);
#line 673 "compiler/main.gem"
    GemVal _t1166 = (*gem_v_advance);
        (void)(_t1166.fn(_t1166.env, NULL, 0));
#line 676 "compiler/main.gem"
    GemVal _t1167 = (*gem_v_peek);
    GemVal _t1168 = _t1167.fn(_t1167.env, NULL, 0);
    static GemICacheSlot _ic_172 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1168, "type", &_ic_172), gem_string("{")))) {
#line 677 "compiler/main.gem"
    GemVal _t1169 = (*gem_v_advance);
            (void)(_t1169.fn(_t1169.env, NULL, 0));
#line 678 "compiler/main.gem"
    GemVal _t1170 = gem_table_new();
            GemVal gem_v_names = _t1170;
#line 679 "compiler/main.gem"
    GemVal _t1171 = (*gem_v_skip_nl);
            (void)(_t1171.fn(_t1171.env, NULL, 0));
#line 680 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1172 = (*gem_v_peek);
                GemVal _t1173 = _t1172.fn(_t1172.env, NULL, 0);
                static GemICacheSlot _ic_173 = {0};
                if (!gem_truthy(gem_neq(gem_table_get_cached(_t1173, "type", &_ic_173), gem_string("}")))) break;
#line 681 "compiler/main.gem"
    GemVal _t1174 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1174.fn(_t1174.env, NULL, 0);
#line 682 "compiler/main.gem"
    GemVal _t1175 = gem_v_field_tok;
    static GemICacheSlot _ic_174 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1175, "type", &_ic_174), gem_string("NAME")))) {
#line 683 "compiler/main.gem"
    GemVal _t1176 = (*gem_v_advance);
                    (void)(_t1176.fn(_t1176.env, NULL, 0));
                } else {
#line 686 "compiler/main.gem"
    GemVal _t1177 = (*gem_v_advance);
                    (void)(_t1177.fn(_t1177.env, NULL, 0));
                }
#line 688 "compiler/main.gem"
    GemVal _t1178 = gem_v_field_tok;
    static GemICacheSlot _ic_175 = {0};
    GemVal _t1179[] = {gem_v_names, gem_table_get_cached(_t1178, "value", &_ic_175)};
                (void)(gem_push_fn(NULL, _t1179, 2));
#line 689 "compiler/main.gem"
    GemVal _t1180 = (*gem_v_skip_nl);
                (void)(_t1180.fn(_t1180.env, NULL, 0));
#line 690 "compiler/main.gem"
    GemVal _t1181 = (*gem_v_peek);
    GemVal _t1182 = _t1181.fn(_t1181.env, NULL, 0);
    static GemICacheSlot _ic_176 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1182, "type", &_ic_176), gem_string(",")))) {
#line 691 "compiler/main.gem"
    GemVal _t1183 = (*gem_v_advance);
                    (void)(_t1183.fn(_t1183.env, NULL, 0));
#line 692 "compiler/main.gem"
    GemVal _t1184 = (*gem_v_skip_nl);
                    (void)(_t1184.fn(_t1184.env, NULL, 0));
                }
            }
#line 695 "compiler/main.gem"
    GemVal _t1185[] = {gem_string("}")};
    GemVal _t1186 = (*gem_v_expect);
            (void)(_t1186.fn(_t1186.env, _t1185, 1));
#line 696 "compiler/main.gem"
    GemVal _t1187[] = {gem_string("=")};
    GemVal _t1188 = (*gem_v_expect);
            (void)(_t1188.fn(_t1188.env, _t1187, 1));
#line 697 "compiler/main.gem"
    GemVal _t1189 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1189.fn(_t1189.env, NULL, 0);
#line 698 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 699 "compiler/main.gem"
    GemVal _t1190[] = {gem_string("_d"), (*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_interp(2, _t1190);
#line 700 "compiler/main.gem"
    GemVal _t1191 = gem_table_new();
    GemVal _t1192[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1193 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1191, gem_int(0), _t1193.fn(_t1193.env, _t1192, 3));
            GemVal gem_v_stmts = _t1191;
#line 701 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 701 "compiler/main.gem"
    GemVal _t1194[] = {gem_v_names};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1194, 1);
#line 701 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 701 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_8;
#line 701 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 702 "compiler/main.gem"
    GemVal _t1195[] = {gem_v_tmp};
    GemVal _t1196 = (*gem_v__mod_parser_make_var);
    GemVal _t1197[] = {_t1196.fn(_t1196.env, _t1195, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1198 = (*gem_v__mod_parser_make_dot);
    GemVal _t1199[] = {gem_table_get(gem_v_names, gem_v_di), _t1198.fn(_t1198.env, _t1197, 2), gem_v_line};
    GemVal _t1200 = (*gem_v__mod_parser_make_let);
    GemVal _t1201[] = {gem_v_stmts, _t1200.fn(_t1200.env, _t1199, 3)};
                (void)(gem_push_fn(NULL, _t1201, 2));
            }

#line 704 "compiler/main.gem"
    GemVal _t1202 = gem_table_new();
    gem_table_set(_t1202, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1202, gem_string("stmts"), gem_v_stmts);
            GemVal _t1203 = _t1202;
            gem_pop_frame();
            return _t1203;
        }
#line 708 "compiler/main.gem"
    GemVal _t1204 = (*gem_v_peek);
    GemVal _t1205 = _t1204.fn(_t1204.env, NULL, 0);
    static GemICacheSlot _ic_177 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1205, "type", &_ic_177), gem_string("[")))) {
#line 709 "compiler/main.gem"
    GemVal _t1206 = (*gem_v_advance);
            (void)(_t1206.fn(_t1206.env, NULL, 0));
#line 710 "compiler/main.gem"
    GemVal _t1207 = gem_table_new();
            GemVal gem_v_names = _t1207;
#line 711 "compiler/main.gem"
    GemVal _t1208 = (*gem_v_skip_nl);
            (void)(_t1208.fn(_t1208.env, NULL, 0));
#line 712 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1209 = (*gem_v_peek);
                GemVal _t1210 = _t1209.fn(_t1209.env, NULL, 0);
                static GemICacheSlot _ic_178 = {0};
                if (!gem_truthy(gem_neq(gem_table_get_cached(_t1210, "type", &_ic_178), gem_string("]")))) break;
#line 713 "compiler/main.gem"
    GemVal _t1211[] = {gem_string("NAME")};
    GemVal _t1212 = (*gem_v_expect);
    GemVal _t1213 = _t1212.fn(_t1212.env, _t1211, 1);
    static GemICacheSlot _ic_179 = {0};
    GemVal _t1214[] = {gem_v_names, gem_table_get_cached(_t1213, "value", &_ic_179)};
                (void)(gem_push_fn(NULL, _t1214, 2));
#line 714 "compiler/main.gem"
    GemVal _t1215 = (*gem_v_skip_nl);
                (void)(_t1215.fn(_t1215.env, NULL, 0));
#line 715 "compiler/main.gem"
    GemVal _t1216 = (*gem_v_peek);
    GemVal _t1217 = _t1216.fn(_t1216.env, NULL, 0);
    static GemICacheSlot _ic_180 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1217, "type", &_ic_180), gem_string(",")))) {
#line 716 "compiler/main.gem"
    GemVal _t1218 = (*gem_v_advance);
                    (void)(_t1218.fn(_t1218.env, NULL, 0));
#line 717 "compiler/main.gem"
    GemVal _t1219 = (*gem_v_skip_nl);
                    (void)(_t1219.fn(_t1219.env, NULL, 0));
                }
            }
#line 720 "compiler/main.gem"
    GemVal _t1220[] = {gem_string("]")};
    GemVal _t1221 = (*gem_v_expect);
            (void)(_t1221.fn(_t1221.env, _t1220, 1));
#line 721 "compiler/main.gem"
    GemVal _t1222[] = {gem_string("=")};
    GemVal _t1223 = (*gem_v_expect);
            (void)(_t1223.fn(_t1223.env, _t1222, 1));
#line 722 "compiler/main.gem"
    GemVal _t1224 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1224.fn(_t1224.env, NULL, 0);
#line 723 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 724 "compiler/main.gem"
    GemVal _t1225[] = {gem_string("_d"), (*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_interp(2, _t1225);
#line 725 "compiler/main.gem"
    GemVal _t1226 = gem_table_new();
    GemVal _t1227[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1228 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1226, gem_int(0), _t1228.fn(_t1228.env, _t1227, 3));
            GemVal gem_v_stmts = _t1226;
#line 726 "compiler/main.gem"
            GemVal gem_v__for_i_9 = gem_int(0);
#line 726 "compiler/main.gem"
    GemVal _t1229[] = {gem_v_names};
            GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1229, 1);
#line 726 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 726 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_9;
#line 726 "compiler/main.gem"
                gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 727 "compiler/main.gem"
    GemVal _t1230[] = {gem_v_tmp};
    GemVal _t1231 = (*gem_v__mod_parser_make_var);
    GemVal _t1232[] = {gem_v_di};
    GemVal _t1233 = (*gem_v__mod_parser_make_int);
    GemVal _t1234[] = {_t1231.fn(_t1231.env, _t1230, 1), _t1233.fn(_t1233.env, _t1232, 1)};
    GemVal _t1235 = (*gem_v__mod_parser_make_index);
    GemVal _t1236[] = {gem_table_get(gem_v_names, gem_v_di), _t1235.fn(_t1235.env, _t1234, 2), gem_v_line};
    GemVal _t1237 = (*gem_v__mod_parser_make_let);
    GemVal _t1238[] = {gem_v_stmts, _t1237.fn(_t1237.env, _t1236, 3)};
                (void)(gem_push_fn(NULL, _t1238, 2));
            }

#line 729 "compiler/main.gem"
    GemVal _t1239 = gem_table_new();
    gem_table_set(_t1239, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1239, gem_string("stmts"), gem_v_stmts);
            GemVal _t1240 = _t1239;
            gem_pop_frame();
            return _t1240;
        }
#line 732 "compiler/main.gem"
    GemVal _t1241[] = {gem_string("NAME")};
    GemVal _t1242 = (*gem_v_expect);
    GemVal _t1243 = _t1242.fn(_t1242.env, _t1241, 1);
    static GemICacheSlot _ic_181 = {0};
        GemVal gem_v_name = gem_table_get_cached(_t1243, "value", &_ic_181);
#line 733 "compiler/main.gem"
    GemVal _t1244[] = {gem_string("=")};
    GemVal _t1245 = (*gem_v_expect);
        (void)(_t1245.fn(_t1245.env, _t1244, 1));
#line 734 "compiler/main.gem"
    GemVal _t1246 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1246.fn(_t1246.env, NULL, 0);
#line 735 "compiler/main.gem"
    GemVal _t1247[] = {gem_v_name, gem_v_value, gem_v_line};
    GemVal _t1248 = (*gem_v__mod_parser_make_let);
        GemVal _t1249 = _t1248.fn(_t1248.env, _t1247, 3);
        gem_pop_frame();
        return _t1249;
    }
#line 739 "compiler/main.gem"
    GemVal _t1250 = gem_v_t;
    static GemICacheSlot _ic_182 = {0};
    GemVal _t1254;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1250, "type", &_ic_182), gem_string("fn")))) {
        _t1254 = gem_eq(gem_table_get_cached(_t1250, "type", &_ic_182), gem_string("fn"));
    } else {
        GemVal _t1251[] = {gem_int(1)};
        GemVal _t1252 = (*gem_v_peek_at);
        GemVal _t1253 = _t1252.fn(_t1252.env, _t1251, 1);
        static GemICacheSlot _ic_183 = {0};
        _t1254 = gem_eq(gem_table_get_cached(_t1253, "type", &_ic_183), gem_string("NAME"));
    }
    if (gem_truthy(_t1254)) {
#line 740 "compiler/main.gem"
    GemVal _t1255 = gem_v_t;
    static GemICacheSlot _ic_184 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1255, "line", &_ic_184);
#line 741 "compiler/main.gem"
    GemVal _t1256 = (*gem_v_advance);
        (void)(_t1256.fn(_t1256.env, NULL, 0));
#line 742 "compiler/main.gem"
    GemVal _t1257[] = {gem_string("NAME")};
    GemVal _t1258 = (*gem_v_expect);
    GemVal _t1259 = _t1258.fn(_t1258.env, _t1257, 1);
    static GemICacheSlot _ic_185 = {0};
        GemVal gem_v_name = gem_table_get_cached(_t1259, "value", &_ic_185);
#line 743 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 744 "compiler/main.gem"
    GemVal _t1260[] = {gem_v_name};
    GemVal _t1261[] = {gem_string("use: let "), gem_v_name, gem_string(" = fn(...) ... end")};
    GemVal _t1262[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1260, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_interp(3, _t1261)};
    GemVal _t1263 = (*gem_v__mod_parser_compile_error);
            (void)(_t1263.fn(_t1263.env, _t1262, 7));
        }
#line 746 "compiler/main.gem"
    GemVal _t1264[] = {gem_string("(")};
    GemVal _t1265 = (*gem_v_expect);
        (void)(_t1265.fn(_t1265.env, _t1264, 1));
#line 747 "compiler/main.gem"
    GemVal _t1266 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1266.fn(_t1266.env, NULL, 0);
#line 748 "compiler/main.gem"
    GemVal _t1267[] = {gem_string(")")};
    GemVal _t1268 = (*gem_v_expect);
        (void)(_t1268.fn(_t1268.env, _t1267, 1));
#line 749 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 750 "compiler/main.gem"
    GemVal _t1269 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1269.fn(_t1269.env, NULL, 0);
#line 751 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 752 "compiler/main.gem"
    GemVal _t1270[] = {gem_string("end")};
    GemVal _t1271 = (*gem_v_expect);
        (void)(_t1271.fn(_t1271.env, _t1270, 1));
#line 753 "compiler/main.gem"
    GemVal _t1272 = gem_v_pr;
    static GemICacheSlot _ic_186 = {0};
    GemVal _t1273 = gem_v_pr;
    static GemICacheSlot _ic_187 = {0};
    GemVal _t1274 = gem_v_pr;
    static GemICacheSlot _ic_188 = {0};
    GemVal _t1275[] = {gem_v_name, gem_table_get_cached(_t1272, "params", &_ic_186), gem_table_get_cached(_t1273, "rest_param", &_ic_187), gem_v_body, gem_v_line, gem_table_get_cached(_t1274, "defaults", &_ic_188)};
    GemVal _t1276 = (*gem_v__mod_parser_make_fn_def);
        GemVal _t1277 = _t1276.fn(_t1276.env, _t1275, 6);
        gem_pop_frame();
        return _t1277;
    }
#line 757 "compiler/main.gem"
    GemVal _t1278 = gem_v_t;
    static GemICacheSlot _ic_189 = {0};
    GemVal _t1280;
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1278, "type", &_ic_189), gem_string("if")))) {
        _t1280 = gem_eq(gem_table_get_cached(_t1278, "type", &_ic_189), gem_string("if"));
    } else {
        GemVal _t1279 = gem_v_t;
        static GemICacheSlot _ic_190 = {0};
        _t1280 = gem_eq(gem_table_get_cached(_t1279, "type", &_ic_190), gem_string("elif"));
    }
    if (gem_truthy(_t1280)) {
#line 758 "compiler/main.gem"
    GemVal _t1281 = gem_v_t;
    static GemICacheSlot _ic_191 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1281, "line", &_ic_191);
#line 759 "compiler/main.gem"
    GemVal _t1282 = (*gem_v_advance);
        (void)(_t1282.fn(_t1282.env, NULL, 0));
#line 760 "compiler/main.gem"
    GemVal _t1283 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1283.fn(_t1283.env, NULL, 0);
#line 761 "compiler/main.gem"
    GemVal _t1284 = (*gem_v_peek);
    GemVal _t1285 = _t1284.fn(_t1284.env, NULL, 0);
    static GemICacheSlot _ic_192 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1285, "type", &_ic_192), gem_string("then")))) {
#line 762 "compiler/main.gem"
    GemVal _t1286 = (*gem_v_advance);
            (void)(_t1286.fn(_t1286.env, NULL, 0));
        }
#line 764 "compiler/main.gem"
    GemVal _t1287 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1287.fn(_t1287.env, NULL, 0);
#line 765 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 766 "compiler/main.gem"
    GemVal _t1288 = (*gem_v_peek);
    GemVal _t1289 = _t1288.fn(_t1288.env, NULL, 0);
    static GemICacheSlot _ic_193 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1289, "type", &_ic_193), gem_string("elif")))) {
#line 768 "compiler/main.gem"
    GemVal _t1290 = gem_table_new();
    GemVal _t1291 = (*gem_v_parse_stmt);
    gem_table_set(_t1290, gem_int(0), _t1291.fn(_t1291.env, NULL, 0));
            gem_v_else_body = _t1290;
        } else {
#line 769 "compiler/main.gem"
    GemVal _t1292 = (*gem_v_peek);
    GemVal _t1293 = _t1292.fn(_t1292.env, NULL, 0);
    static GemICacheSlot _ic_194 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1293, "type", &_ic_194), gem_string("else")))) {
#line 770 "compiler/main.gem"
    GemVal _t1294 = (*gem_v_advance);
                (void)(_t1294.fn(_t1294.env, NULL, 0));
#line 771 "compiler/main.gem"
    GemVal _t1295 = (*gem_v_peek);
    GemVal _t1296 = _t1295.fn(_t1295.env, NULL, 0);
    static GemICacheSlot _ic_195 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1296, "type", &_ic_195), gem_string("if")))) {
#line 772 "compiler/main.gem"
    GemVal _t1297 = (*gem_v_peek);
                    GemVal gem_v_err_tok = _t1297.fn(_t1297.env, NULL, 0);
#line 773 "compiler/main.gem"
    GemVal _t1298 = gem_v_err_tok;
    static GemICacheSlot _ic_196 = {0};
    GemVal _t1299 = gem_v_err_tok;
    static GemICacheSlot _ic_197 = {0};
    GemVal _t1300[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1298, "line", &_ic_196), gem_table_get_cached(_t1299, "col", &_ic_197), gem_int(2), gem_string("use 'elif' instead of 'else if'"), gem_string("Gem uses 'elif' for chained conditions")};
    GemVal _t1301 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1301.fn(_t1301.env, _t1300, 7));
                }
#line 775 "compiler/main.gem"
    GemVal _t1302 = (*gem_v_parse_body);
                gem_v_else_body = _t1302.fn(_t1302.env, NULL, 0);
            }
        }
#line 778 "compiler/main.gem"
    GemVal _t1303 = gem_v_t;
    static GemICacheSlot _ic_198 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1303, "type", &_ic_198), gem_string("if")))) {
#line 779 "compiler/main.gem"
    GemVal _t1304[] = {gem_string("end")};
    GemVal _t1305 = (*gem_v_expect);
            (void)(_t1305.fn(_t1305.env, _t1304, 1));
        }
#line 781 "compiler/main.gem"
    GemVal _t1306[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
    GemVal _t1307 = (*gem_v__mod_parser_make_if);
        GemVal _t1308 = _t1307.fn(_t1307.env, _t1306, 4);
        gem_pop_frame();
        return _t1308;
    }
#line 785 "compiler/main.gem"
    GemVal _t1309 = gem_v_t;
    static GemICacheSlot _ic_199 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1309, "type", &_ic_199), gem_string("while")))) {
#line 786 "compiler/main.gem"
    GemVal _t1310 = gem_v_t;
    static GemICacheSlot _ic_200 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1310, "line", &_ic_200);
#line 787 "compiler/main.gem"
    GemVal _t1311 = (*gem_v_advance);
        (void)(_t1311.fn(_t1311.env, NULL, 0));
#line 788 "compiler/main.gem"
    GemVal _t1312 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1312.fn(_t1312.env, NULL, 0);
#line 789 "compiler/main.gem"
    GemVal _t1313 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1313.fn(_t1313.env, NULL, 0);
#line 790 "compiler/main.gem"
    GemVal _t1314[] = {gem_string("end")};
    GemVal _t1315 = (*gem_v_expect);
        (void)(_t1315.fn(_t1315.env, _t1314, 1));
#line 791 "compiler/main.gem"
    GemVal _t1316[] = {gem_v_cond, gem_v_wbody, gem_v_line};
    GemVal _t1317 = (*gem_v__mod_parser_make_while);
        GemVal _t1318 = _t1317.fn(_t1317.env, _t1316, 3);
        gem_pop_frame();
        return _t1318;
    }
#line 795 "compiler/main.gem"
    GemVal _t1319 = gem_v_t;
    static GemICacheSlot _ic_201 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1319, "type", &_ic_201), gem_string("for")))) {
#line 796 "compiler/main.gem"
    GemVal _t1320 = gem_v_t;
    static GemICacheSlot _ic_202 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1320, "line", &_ic_202);
#line 797 "compiler/main.gem"
    GemVal _t1321 = (*gem_v_advance);
        (void)(_t1321.fn(_t1321.env, NULL, 0));
#line 798 "compiler/main.gem"
    GemVal _t1322[] = {gem_string("NAME")};
    GemVal _t1323 = (*gem_v_expect);
    GemVal _t1324 = _t1323.fn(_t1323.env, _t1322, 1);
    static GemICacheSlot _ic_203 = {0};
        GemVal gem_v_var_name = gem_table_get_cached(_t1324, "value", &_ic_203);
#line 800 "compiler/main.gem"
    GemVal _t1325 = (*gem_v_peek);
    GemVal _t1326 = _t1325.fn(_t1325.env, NULL, 0);
    static GemICacheSlot _ic_204 = {0};
    GemVal _t1330;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1326, "type", &_ic_204), gem_string(",")))) {
        _t1330 = gem_eq(gem_table_get_cached(_t1326, "type", &_ic_204), gem_string(","));
    } else {
        GemVal _t1327[] = {gem_int(1)};
        GemVal _t1328 = (*gem_v_peek_at);
        GemVal _t1329 = _t1328.fn(_t1328.env, _t1327, 1);
        static GemICacheSlot _ic_205 = {0};
        _t1330 = gem_eq(gem_table_get_cached(_t1329, "type", &_ic_205), gem_string("NAME"));
    }
        if (gem_truthy(_t1330)) {
#line 802 "compiler/main.gem"
    GemVal _t1331 = (*gem_v_advance);
            (void)(_t1331.fn(_t1331.env, NULL, 0));
#line 803 "compiler/main.gem"
    GemVal _t1332[] = {gem_string("NAME")};
    GemVal _t1333 = (*gem_v_expect);
    GemVal _t1334 = _t1333.fn(_t1333.env, _t1332, 1);
    static GemICacheSlot _ic_206 = {0};
            GemVal gem_v_val_name = gem_table_get_cached(_t1334, "value", &_ic_206);
#line 804 "compiler/main.gem"
    GemVal _t1335[] = {gem_string("in")};
    GemVal _t1336 = (*gem_v_expect);
            (void)(_t1336.fn(_t1336.env, _t1335, 1));
#line 805 "compiler/main.gem"
    GemVal _t1337 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1337.fn(_t1337.env, NULL, 0);
#line 806 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 807 "compiler/main.gem"
    GemVal _t1338[] = {gem_string("_for_tbl_"), (*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_interp(2, _t1338);
#line 808 "compiler/main.gem"
    GemVal _t1339[] = {gem_string("_for_len_"), (*gem_v_gensym_counter)};
            GemVal gem_v_len_var = gem_interp(2, _t1339);
#line 809 "compiler/main.gem"
    GemVal _t1340[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_interp(2, _t1340);
#line 810 "compiler/main.gem"
    GemVal _t1341 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1341.fn(_t1341.env, NULL, 0);
#line 811 "compiler/main.gem"
    GemVal _t1342[] = {gem_string("end")};
    GemVal _t1343 = (*gem_v_expect);
            (void)(_t1343.fn(_t1343.env, _t1342, 1));
#line 812 "compiler/main.gem"
    GemVal _t1344 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1344;
#line 813 "compiler/main.gem"
    GemVal _t1345[] = {gem_string("__table_key_at")};
    GemVal _t1346 = (*gem_v__mod_parser_make_var);
    GemVal _t1347 = gem_table_new();
    GemVal _t1348[] = {gem_v_tbl_var};
    GemVal _t1349 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1347, gem_int(0), _t1349.fn(_t1349.env, _t1348, 1));
    GemVal _t1350[] = {gem_v_idx_var};
    GemVal _t1351 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1347, gem_int(1), _t1351.fn(_t1351.env, _t1350, 1));
    GemVal _t1352[] = {_t1346.fn(_t1346.env, _t1345, 1), _t1347, gem_int(0)};
    GemVal _t1353 = (*gem_v__mod_parser_make_call);
    GemVal _t1354[] = {gem_v_var_name, _t1353.fn(_t1353.env, _t1352, 3), gem_v_line};
    GemVal _t1355 = (*gem_v__mod_parser_make_let);
    GemVal _t1356[] = {gem_v_inner_stmts, _t1355.fn(_t1355.env, _t1354, 3)};
            (void)(gem_push_fn(NULL, _t1356, 2));
#line 814 "compiler/main.gem"
    GemVal _t1357[] = {gem_string("__table_val_at")};
    GemVal _t1358 = (*gem_v__mod_parser_make_var);
    GemVal _t1359 = gem_table_new();
    GemVal _t1360[] = {gem_v_tbl_var};
    GemVal _t1361 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1359, gem_int(0), _t1361.fn(_t1361.env, _t1360, 1));
    GemVal _t1362[] = {gem_v_idx_var};
    GemVal _t1363 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1359, gem_int(1), _t1363.fn(_t1363.env, _t1362, 1));
    GemVal _t1364[] = {_t1358.fn(_t1358.env, _t1357, 1), _t1359, gem_int(0)};
    GemVal _t1365 = (*gem_v__mod_parser_make_call);
    GemVal _t1366[] = {gem_v_val_name, _t1365.fn(_t1365.env, _t1364, 3), gem_v_line};
    GemVal _t1367 = (*gem_v__mod_parser_make_let);
    GemVal _t1368[] = {gem_v_inner_stmts, _t1367.fn(_t1367.env, _t1366, 3)};
            (void)(gem_push_fn(NULL, _t1368, 2));
#line 815 "compiler/main.gem"
    GemVal _t1369[] = {gem_v_idx_var};
    GemVal _t1370 = (*gem_v__mod_parser_make_var);
    GemVal _t1371[] = {gem_int(1)};
    GemVal _t1372 = (*gem_v__mod_parser_make_int);
    GemVal _t1373[] = {gem_string("+"), _t1370.fn(_t1370.env, _t1369, 1), _t1372.fn(_t1372.env, _t1371, 1)};
    GemVal _t1374 = (*gem_v__mod_parser_make_binop);
    GemVal _t1375[] = {gem_v_idx_var, _t1374.fn(_t1374.env, _t1373, 3), gem_v_line};
    GemVal _t1376 = (*gem_v__mod_parser_make_assign);
    GemVal _t1377[] = {gem_v_inner_stmts, _t1376.fn(_t1376.env, _t1375, 3)};
            (void)(gem_push_fn(NULL, _t1377, 2));
#line 816 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 816 "compiler/main.gem"
    GemVal _t1378[] = {gem_v_fbody};
            GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1378, 1);
#line 816 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 816 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_10;
#line 816 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 817 "compiler/main.gem"
    GemVal _t1379[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1379, 2));
            }

#line 819 "compiler/main.gem"
    GemVal _t1380[] = {gem_v_idx_var};
    GemVal _t1381 = (*gem_v__mod_parser_make_var);
    GemVal _t1382[] = {gem_v_len_var};
    GemVal _t1383 = (*gem_v__mod_parser_make_var);
    GemVal _t1384[] = {gem_string("<"), _t1381.fn(_t1381.env, _t1380, 1), _t1383.fn(_t1383.env, _t1382, 1)};
    GemVal _t1385 = (*gem_v__mod_parser_make_binop);
    GemVal _t1386[] = {_t1385.fn(_t1385.env, _t1384, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1387 = (*gem_v__mod_parser_make_while);
            GemVal gem_v_while_node = _t1387.fn(_t1387.env, _t1386, 3);
#line 824 "compiler/main.gem"
    GemVal _t1388 = gem_table_new();
    gem_table_set(_t1388, gem_string("tag"), gem_string("block"));
    GemVal _t1389 = gem_table_new();
    GemVal _t1390[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    GemVal _t1391 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1389, gem_int(0), _t1391.fn(_t1391.env, _t1390, 3));
    GemVal _t1392[] = {gem_string("len")};
    GemVal _t1393 = (*gem_v__mod_parser_make_var);
    GemVal _t1394 = gem_table_new();
    GemVal _t1395[] = {gem_v_tbl_var};
    GemVal _t1396 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1394, gem_int(0), _t1396.fn(_t1396.env, _t1395, 1));
    GemVal _t1397[] = {_t1393.fn(_t1393.env, _t1392, 1), _t1394, gem_int(0)};
    GemVal _t1398 = (*gem_v__mod_parser_make_call);
    GemVal _t1399[] = {gem_v_len_var, _t1398.fn(_t1398.env, _t1397, 3), gem_v_line};
    GemVal _t1400 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1389, gem_int(1), _t1400.fn(_t1400.env, _t1399, 3));
    GemVal _t1401[] = {gem_int(0)};
    GemVal _t1402 = (*gem_v__mod_parser_make_int);
    GemVal _t1403[] = {gem_v_idx_var, _t1402.fn(_t1402.env, _t1401, 1), gem_v_line};
    GemVal _t1404 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1389, gem_int(2), _t1404.fn(_t1404.env, _t1403, 3));
    gem_table_set(_t1389, gem_int(3), gem_v_while_node);
    gem_table_set(_t1388, gem_string("stmts"), _t1389);
            GemVal _t1405 = _t1388;
            gem_pop_frame();
            return _t1405;
        } else {
#line 830 "compiler/main.gem"
    GemVal _t1406 = (*gem_v_peek);
    GemVal _t1407 = _t1406.fn(_t1406.env, NULL, 0);
    static GemICacheSlot _ic_207 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1407, "type", &_ic_207), gem_string("in")))) {
#line 832 "compiler/main.gem"
    GemVal _t1408 = (*gem_v_advance);
                (void)(_t1408.fn(_t1408.env, NULL, 0));
#line 833 "compiler/main.gem"
    GemVal _t1409 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1409.fn(_t1409.env, NULL, 0);
#line 834 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 835 "compiler/main.gem"
    GemVal _t1410[] = {gem_string("_for_items_"), (*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_interp(2, _t1410);
#line 836 "compiler/main.gem"
    GemVal _t1411[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_interp(2, _t1411);
#line 837 "compiler/main.gem"
    GemVal _t1412 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1412.fn(_t1412.env, NULL, 0);
#line 838 "compiler/main.gem"
    GemVal _t1413[] = {gem_string("end")};
    GemVal _t1414 = (*gem_v_expect);
                (void)(_t1414.fn(_t1414.env, _t1413, 1));
#line 839 "compiler/main.gem"
    GemVal _t1415 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1415;
#line 840 "compiler/main.gem"
    GemVal _t1416[] = {gem_v_items_var};
    GemVal _t1417 = (*gem_v__mod_parser_make_var);
    GemVal _t1418[] = {gem_v_idx_var};
    GemVal _t1419 = (*gem_v__mod_parser_make_var);
    GemVal _t1420[] = {_t1417.fn(_t1417.env, _t1416, 1), _t1419.fn(_t1419.env, _t1418, 1)};
    GemVal _t1421 = (*gem_v__mod_parser_make_index);
    GemVal _t1422[] = {gem_v_var_name, _t1421.fn(_t1421.env, _t1420, 2), gem_v_line};
    GemVal _t1423 = (*gem_v__mod_parser_make_let);
    GemVal _t1424[] = {gem_v_inner_stmts, _t1423.fn(_t1423.env, _t1422, 3)};
                (void)(gem_push_fn(NULL, _t1424, 2));
#line 841 "compiler/main.gem"
    GemVal _t1425[] = {gem_v_idx_var};
    GemVal _t1426 = (*gem_v__mod_parser_make_var);
    GemVal _t1427[] = {gem_int(1)};
    GemVal _t1428 = (*gem_v__mod_parser_make_int);
    GemVal _t1429[] = {gem_string("+"), _t1426.fn(_t1426.env, _t1425, 1), _t1428.fn(_t1428.env, _t1427, 1)};
    GemVal _t1430 = (*gem_v__mod_parser_make_binop);
    GemVal _t1431[] = {gem_v_idx_var, _t1430.fn(_t1430.env, _t1429, 3), gem_v_line};
    GemVal _t1432 = (*gem_v__mod_parser_make_assign);
    GemVal _t1433[] = {gem_v_inner_stmts, _t1432.fn(_t1432.env, _t1431, 3)};
                (void)(gem_push_fn(NULL, _t1433, 2));
#line 842 "compiler/main.gem"
                GemVal gem_v__for_i_11 = gem_int(0);
#line 842 "compiler/main.gem"
    GemVal _t1434[] = {gem_v_fbody};
                GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1434, 1);
#line 842 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 842 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_11;
#line 842 "compiler/main.gem"
                    gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 843 "compiler/main.gem"
    GemVal _t1435[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1435, 2));
                }

#line 845 "compiler/main.gem"
    GemVal _t1436[] = {gem_v_idx_var};
    GemVal _t1437 = (*gem_v__mod_parser_make_var);
    GemVal _t1438[] = {gem_string("len")};
    GemVal _t1439 = (*gem_v__mod_parser_make_var);
    GemVal _t1440 = gem_table_new();
    GemVal _t1441[] = {gem_v_items_var};
    GemVal _t1442 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1440, gem_int(0), _t1442.fn(_t1442.env, _t1441, 1));
    GemVal _t1443[] = {_t1439.fn(_t1439.env, _t1438, 1), _t1440, gem_int(0)};
    GemVal _t1444 = (*gem_v__mod_parser_make_call);
    GemVal _t1445[] = {gem_string("<"), _t1437.fn(_t1437.env, _t1436, 1), _t1444.fn(_t1444.env, _t1443, 3)};
    GemVal _t1446 = (*gem_v__mod_parser_make_binop);
    GemVal _t1447[] = {_t1446.fn(_t1446.env, _t1445, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1448 = (*gem_v__mod_parser_make_while);
                GemVal gem_v_while_node = _t1448.fn(_t1448.env, _t1447, 3);
#line 850 "compiler/main.gem"
    GemVal _t1449 = gem_table_new();
    gem_table_set(_t1449, gem_string("tag"), gem_string("block"));
    GemVal _t1450 = gem_table_new();
    GemVal _t1451[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    GemVal _t1452 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1450, gem_int(0), _t1452.fn(_t1452.env, _t1451, 3));
    GemVal _t1453[] = {gem_int(0)};
    GemVal _t1454 = (*gem_v__mod_parser_make_int);
    GemVal _t1455[] = {gem_v_idx_var, _t1454.fn(_t1454.env, _t1453, 1), gem_v_line};
    GemVal _t1456 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1450, gem_int(1), _t1456.fn(_t1456.env, _t1455, 3));
    gem_table_set(_t1450, gem_int(2), gem_v_while_node);
    gem_table_set(_t1449, gem_string("stmts"), _t1450);
                GemVal _t1457 = _t1449;
                gem_pop_frame();
                return _t1457;
            } else {
#line 855 "compiler/main.gem"
    GemVal _t1458 = (*gem_v_peek);
    GemVal _t1459 = _t1458.fn(_t1458.env, NULL, 0);
    static GemICacheSlot _ic_208 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1459, "type", &_ic_208), gem_string("=")))) {
#line 857 "compiler/main.gem"
    GemVal _t1460 = (*gem_v_advance);
                    (void)(_t1460.fn(_t1460.env, NULL, 0));
#line 858 "compiler/main.gem"
    GemVal _t1461 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1461.fn(_t1461.env, NULL, 0);
#line 859 "compiler/main.gem"
    GemVal _t1462[] = {gem_string(",")};
    GemVal _t1463 = (*gem_v_expect);
                    (void)(_t1463.fn(_t1463.env, _t1462, 1));
#line 860 "compiler/main.gem"
    GemVal _t1464 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1464.fn(_t1464.env, NULL, 0);
#line 861 "compiler/main.gem"
    GemVal _t1465 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1465.fn(_t1465.env, NULL, 0);
#line 862 "compiler/main.gem"
    GemVal _t1466[] = {gem_string("end")};
    GemVal _t1467 = (*gem_v_expect);
                    (void)(_t1467.fn(_t1467.env, _t1466, 1));
#line 863 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 864 "compiler/main.gem"
    GemVal _t1468[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_interp(2, _t1468);
#line 865 "compiler/main.gem"
    GemVal _t1469[] = {gem_string("_for_limit_"), (*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_interp(2, _t1469);
#line 866 "compiler/main.gem"
    GemVal _t1470 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1470;
#line 867 "compiler/main.gem"
    GemVal _t1471[] = {gem_v_idx_var};
    GemVal _t1472 = (*gem_v__mod_parser_make_var);
    GemVal _t1473[] = {gem_v_var_name, _t1472.fn(_t1472.env, _t1471, 1), gem_v_line};
    GemVal _t1474 = (*gem_v__mod_parser_make_let);
    GemVal _t1475[] = {gem_v_inner_stmts, _t1474.fn(_t1474.env, _t1473, 3)};
                    (void)(gem_push_fn(NULL, _t1475, 2));
#line 868 "compiler/main.gem"
    GemVal _t1476[] = {gem_v_idx_var};
    GemVal _t1477 = (*gem_v__mod_parser_make_var);
    GemVal _t1478[] = {gem_int(1)};
    GemVal _t1479 = (*gem_v__mod_parser_make_int);
    GemVal _t1480[] = {gem_string("+"), _t1477.fn(_t1477.env, _t1476, 1), _t1479.fn(_t1479.env, _t1478, 1)};
    GemVal _t1481 = (*gem_v__mod_parser_make_binop);
    GemVal _t1482[] = {gem_v_idx_var, _t1481.fn(_t1481.env, _t1480, 3), gem_v_line};
    GemVal _t1483 = (*gem_v__mod_parser_make_assign);
    GemVal _t1484[] = {gem_v_inner_stmts, _t1483.fn(_t1483.env, _t1482, 3)};
                    (void)(gem_push_fn(NULL, _t1484, 2));
#line 869 "compiler/main.gem"
                    GemVal gem_v__for_i_12 = gem_int(0);
#line 869 "compiler/main.gem"
    GemVal _t1485[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1485, 1);
#line 869 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 869 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_12;
#line 869 "compiler/main.gem"
                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 870 "compiler/main.gem"
    GemVal _t1486[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1486, 2));
                    }

#line 872 "compiler/main.gem"
    GemVal _t1487[] = {gem_v_idx_var};
    GemVal _t1488 = (*gem_v__mod_parser_make_var);
    GemVal _t1489[] = {gem_v_limit_var};
    GemVal _t1490 = (*gem_v__mod_parser_make_var);
    GemVal _t1491[] = {gem_string("<"), _t1488.fn(_t1488.env, _t1487, 1), _t1490.fn(_t1490.env, _t1489, 1)};
    GemVal _t1492 = (*gem_v__mod_parser_make_binop);
    GemVal _t1493[] = {_t1492.fn(_t1492.env, _t1491, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1494 = (*gem_v__mod_parser_make_while);
                    GemVal gem_v_while_node = _t1494.fn(_t1494.env, _t1493, 3);
#line 877 "compiler/main.gem"
    GemVal _t1495 = gem_table_new();
    gem_table_set(_t1495, gem_string("tag"), gem_string("block"));
    GemVal _t1496 = gem_table_new();
    GemVal _t1497[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    GemVal _t1498 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1496, gem_int(0), _t1498.fn(_t1498.env, _t1497, 3));
    GemVal _t1499[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    GemVal _t1500 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1496, gem_int(1), _t1500.fn(_t1500.env, _t1499, 3));
    gem_table_set(_t1496, gem_int(2), gem_v_while_node);
    gem_table_set(_t1495, gem_string("stmts"), _t1496);
                    GemVal _t1501 = _t1495;
                    gem_pop_frame();
                    return _t1501;
                } else {
#line 883 "compiler/main.gem"
    GemVal _t1502 = (*gem_v_peek);
    GemVal _t1503 = _t1502.fn(_t1502.env, NULL, 0);
    static GemICacheSlot _ic_209 = {0};
    GemVal _t1504 = (*gem_v_peek);
    GemVal _t1505 = _t1504.fn(_t1504.env, NULL, 0);
    static GemICacheSlot _ic_210 = {0};
    GemVal _t1506 = (*gem_v_peek);
    GemVal _t1507 = _t1506.fn(_t1506.env, NULL, 0);
    static GemICacheSlot _ic_211 = {0};
    GemVal _t1508[] = {gem_table_get_cached(_t1507, "value", &_ic_211)};
    GemVal _t1509[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1503, "line", &_ic_209), gem_table_get_cached(_t1505, "col", &_ic_210), gem_len_fn(NULL, _t1508, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
    GemVal _t1510 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1510.fn(_t1510.env, _t1509, 7));
                }
            }
        }
    }
#line 888 "compiler/main.gem"
    GemVal _t1511 = gem_v_t;
    static GemICacheSlot _ic_212 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1511, "type", &_ic_212), gem_string("match")))) {
#line 889 "compiler/main.gem"
    GemVal _t1512 = gem_v_t;
    static GemICacheSlot _ic_213 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1512, "line", &_ic_213);
#line 890 "compiler/main.gem"
    GemVal _t1513 = (*gem_v_advance);
        (void)(_t1513.fn(_t1513.env, NULL, 0));
#line 891 "compiler/main.gem"
    GemVal _t1514 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1514.fn(_t1514.env, NULL, 0);
#line 892 "compiler/main.gem"
    GemVal _t1515 = (*gem_v_skip_nl);
        (void)(_t1515.fn(_t1515.env, NULL, 0));
#line 893 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 894 "compiler/main.gem"
    GemVal _t1516[] = {gem_string("_match_"), (*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_interp(2, _t1516);
#line 895 "compiler/main.gem"
    GemVal _t1517[] = {gem_v_match_var};
    GemVal _t1518 = (*gem_v__mod_parser_make_var);
        GemVal gem_v_target_var_expr = _t1518.fn(_t1518.env, _t1517, 1);
#line 896 "compiler/main.gem"
    GemVal _t1519 = gem_table_new();
        GemVal gem_v_whens = _t1519;
#line 897 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1520 = (*gem_v_peek);
            GemVal _t1521 = _t1520.fn(_t1520.env, NULL, 0);
            static GemICacheSlot _ic_214 = {0};
            if (!gem_truthy(gem_eq(gem_table_get_cached(_t1521, "type", &_ic_214), gem_string("when")))) break;
#line 898 "compiler/main.gem"
    GemVal _t1522 = (*gem_v_advance);
            (void)(_t1522.fn(_t1522.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1523 = (*gem_v_peek);
    GemVal _t1524 = _t1523.fn(_t1523.env, NULL, 0);
    static GemICacheSlot _ic_215 = {0};
            GemVal gem_v_pt = gem_table_get_cached(_t1524, "type", &_ic_215);
#line 901 "compiler/main.gem"
    GemVal _t1525[] = {gem_int(1)};
    GemVal _t1526 = (*gem_v_peek_at);
    GemVal _t1527 = _t1526.fn(_t1526.env, _t1525, 1);
    static GemICacheSlot _ic_216 = {0};
            GemVal gem_v_next = gem_table_get_cached(_t1527, "type", &_ic_216);
#line 902 "compiler/main.gem"
    GemVal _t1528;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1528 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1528 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1528)) {
#line 903 "compiler/main.gem"
    GemVal _t1529[] = {gem_v_target_var_expr};
    GemVal _t1530 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1530.fn(_t1530.env, _t1529, 1);
#line 904 "compiler/main.gem"
    GemVal _t1531 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1531.fn(_t1531.env, NULL, 0);
#line 905 "compiler/main.gem"
    GemVal _t1532 = gem_v_pat;
    static GemICacheSlot _ic_217 = {0};
    GemVal _t1533 = gem_v_pat;
    static GemICacheSlot _ic_218 = {0};
    GemVal _t1534[] = {gem_table_get_cached(_t1532, "condition", &_ic_217), gem_v_wbody, gem_table_get_cached(_t1533, "bindings", &_ic_218)};
    GemVal _t1535 = (*gem_v__mod_parser_make_when);
    GemVal _t1536[] = {gem_v_whens, _t1535.fn(_t1535.env, _t1534, 3)};
                (void)(gem_push_fn(NULL, _t1536, 2));
            } else {
#line 906 "compiler/main.gem"
    GemVal _t1541;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1541 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1537;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1537 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1537 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1538;
        if (gem_truthy(_t1537)) {
                _t1538 = _t1537;
        } else {
                _t1538 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1539;
        if (gem_truthy(_t1538)) {
                _t1539 = _t1538;
        } else {
                _t1539 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1540;
        if (gem_truthy(_t1539)) {
                _t1540 = _t1539;
        } else {
                _t1540 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1541 = _t1540;
    }
                if (gem_truthy(_t1541)) {
#line 908 "compiler/main.gem"
    GemVal _t1542[] = {gem_v_target_var_expr};
    GemVal _t1543 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1543.fn(_t1543.env, _t1542, 1);
#line 909 "compiler/main.gem"
    GemVal _t1544 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1544.fn(_t1544.env, NULL, 0);
#line 910 "compiler/main.gem"
    GemVal _t1545 = gem_v_pat;
    static GemICacheSlot _ic_219 = {0};
    GemVal _t1546 = gem_v_pat;
    static GemICacheSlot _ic_220 = {0};
    GemVal _t1547[] = {gem_table_get_cached(_t1545, "condition", &_ic_219), gem_v_wbody, gem_table_get_cached(_t1546, "bindings", &_ic_220)};
    GemVal _t1548 = (*gem_v__mod_parser_make_when);
    GemVal _t1549[] = {gem_v_whens, _t1548.fn(_t1548.env, _t1547, 3)};
                    (void)(gem_push_fn(NULL, _t1549, 2));
                } else {
#line 913 "compiler/main.gem"
    GemVal _t1550 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1550.fn(_t1550.env, NULL, 0);
#line 914 "compiler/main.gem"
    GemVal _t1551 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1551.fn(_t1551.env, NULL, 0);
#line 915 "compiler/main.gem"
    GemVal _t1552[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1553 = (*gem_v__mod_parser_make_binop);
    GemVal _t1554 = gem_table_new();
    GemVal _t1555[] = {_t1553.fn(_t1553.env, _t1552, 3), gem_v_wbody, _t1554};
    GemVal _t1556 = (*gem_v__mod_parser_make_when);
    GemVal _t1557[] = {gem_v_whens, _t1556.fn(_t1556.env, _t1555, 3)};
                    (void)(gem_push_fn(NULL, _t1557, 2));
                }
            }
#line 917 "compiler/main.gem"
    GemVal _t1558 = (*gem_v_skip_nl);
            (void)(_t1558.fn(_t1558.env, NULL, 0));
        }
#line 919 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 920 "compiler/main.gem"
    GemVal _t1559 = (*gem_v_peek);
    GemVal _t1560 = _t1559.fn(_t1559.env, NULL, 0);
    static GemICacheSlot _ic_221 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1560, "type", &_ic_221), gem_string("else")))) {
#line 921 "compiler/main.gem"
    GemVal _t1561 = (*gem_v_advance);
            (void)(_t1561.fn(_t1561.env, NULL, 0));
#line 922 "compiler/main.gem"
    GemVal _t1562 = (*gem_v_parse_body);
            gem_v_else_body = _t1562.fn(_t1562.env, NULL, 0);
        }
#line 924 "compiler/main.gem"
    GemVal _t1563[] = {gem_string("end")};
    GemVal _t1564 = (*gem_v_expect);
        (void)(_t1564.fn(_t1564.env, _t1563, 1));
#line 925 "compiler/main.gem"
    GemVal _t1565[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
    GemVal _t1566 = (*gem_v__mod_parser_make_match);
        GemVal _t1567 = _t1566.fn(_t1566.env, _t1565, 5);
        gem_pop_frame();
        return _t1567;
    }
#line 929 "compiler/main.gem"
    GemVal _t1568 = gem_v_t;
    static GemICacheSlot _ic_222 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1568, "type", &_ic_222), gem_string("return")))) {
#line 930 "compiler/main.gem"
    GemVal _t1569 = gem_v_t;
    static GemICacheSlot _ic_223 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1569, "line", &_ic_223);
#line 931 "compiler/main.gem"
    GemVal _t1570 = (*gem_v_advance);
        (void)(_t1570.fn(_t1570.env, NULL, 0));
#line 932 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 933 "compiler/main.gem"
    GemVal _t1571 = (*gem_v_peek);
    GemVal _t1572 = _t1571.fn(_t1571.env, NULL, 0);
    static GemICacheSlot _ic_224 = {0};
    GemVal _t1575;
    if (!gem_truthy(gem_neq(gem_table_get_cached(_t1572, "type", &_ic_224), gem_string("NEWLINE")))) {
        _t1575 = gem_neq(gem_table_get_cached(_t1572, "type", &_ic_224), gem_string("NEWLINE"));
    } else {
        GemVal _t1573 = (*gem_v_peek);
        GemVal _t1574 = _t1573.fn(_t1573.env, NULL, 0);
        static GemICacheSlot _ic_225 = {0};
        _t1575 = gem_neq(gem_table_get_cached(_t1574, "type", &_ic_225), gem_string("EOF"));
    }
    GemVal _t1578;
    if (!gem_truthy(_t1575)) {
        _t1578 = _t1575;
    } else {
        GemVal _t1576 = (*gem_v_peek);
        GemVal _t1577 = _t1576.fn(_t1576.env, NULL, 0);
        static GemICacheSlot _ic_226 = {0};
        _t1578 = gem_neq(gem_table_get_cached(_t1577, "type", &_ic_226), gem_string("end"));
    }
    GemVal _t1581;
    if (!gem_truthy(_t1578)) {
        _t1581 = _t1578;
    } else {
        GemVal _t1579 = (*gem_v_peek);
        GemVal _t1580 = _t1579.fn(_t1579.env, NULL, 0);
        static GemICacheSlot _ic_227 = {0};
        _t1581 = gem_neq(gem_table_get_cached(_t1580, "type", &_ic_227), gem_string("elif"));
    }
    GemVal _t1584;
    if (!gem_truthy(_t1581)) {
        _t1584 = _t1581;
    } else {
        GemVal _t1582 = (*gem_v_peek);
        GemVal _t1583 = _t1582.fn(_t1582.env, NULL, 0);
        static GemICacheSlot _ic_228 = {0};
        _t1584 = gem_neq(gem_table_get_cached(_t1583, "type", &_ic_228), gem_string("else"));
    }
    GemVal _t1587;
    if (!gem_truthy(_t1584)) {
        _t1587 = _t1584;
    } else {
        GemVal _t1585 = (*gem_v_peek);
        GemVal _t1586 = _t1585.fn(_t1585.env, NULL, 0);
        static GemICacheSlot _ic_229 = {0};
        _t1587 = gem_neq(gem_table_get_cached(_t1586, "type", &_ic_229), gem_string("when"));
    }
        if (gem_truthy(_t1587)) {
#line 934 "compiler/main.gem"
    GemVal _t1588 = (*gem_v_parse_expr);
            gem_v_value = _t1588.fn(_t1588.env, NULL, 0);
        }
#line 936 "compiler/main.gem"
    GemVal _t1589[] = {gem_v_value, gem_v_line};
    GemVal _t1590 = (*gem_v__mod_parser_make_return);
        GemVal _t1591 = _t1590.fn(_t1590.env, _t1589, 2);
        gem_pop_frame();
        return _t1591;
    }
#line 940 "compiler/main.gem"
    GemVal _t1592 = gem_v_t;
    static GemICacheSlot _ic_230 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1592, "type", &_ic_230), gem_string("break")))) {
#line 941 "compiler/main.gem"
    GemVal _t1593 = (*gem_v_advance);
        (void)(_t1593.fn(_t1593.env, NULL, 0));
#line 942 "compiler/main.gem"
    GemVal _t1594 = (*gem_v__mod_parser_make_break);
        GemVal _t1595 = _t1594.fn(_t1594.env, NULL, 0);
        gem_pop_frame();
        return _t1595;
    }
#line 946 "compiler/main.gem"
    GemVal _t1596 = gem_v_t;
    static GemICacheSlot _ic_231 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1596, "type", &_ic_231), gem_string("continue")))) {
#line 947 "compiler/main.gem"
    GemVal _t1597 = (*gem_v_advance);
        (void)(_t1597.fn(_t1597.env, NULL, 0));
#line 948 "compiler/main.gem"
    GemVal _t1598 = (*gem_v__mod_parser_make_continue);
        GemVal _t1599 = _t1598.fn(_t1598.env, NULL, 0);
        gem_pop_frame();
        return _t1599;
    }
#line 952 "compiler/main.gem"
    GemVal _t1600 = gem_v_t;
    static GemICacheSlot _ic_232 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1600, "type", &_ic_232), gem_string("load")))) {
#line 953 "compiler/main.gem"
    GemVal _t1601 = (*gem_v_advance);
        (void)(_t1601.fn(_t1601.env, NULL, 0));
#line 954 "compiler/main.gem"
    GemVal _t1602[] = {gem_string("STRING")};
    GemVal _t1603 = (*gem_v_expect);
    GemVal _t1604 = _t1603.fn(_t1603.env, _t1602, 1);
    static GemICacheSlot _ic_233 = {0};
        GemVal gem_v_path = gem_table_get_cached(_t1604, "value", &_ic_233);
#line 955 "compiler/main.gem"
        GemVal gem_v_alias_name = GEM_NIL;
#line 956 "compiler/main.gem"
        GemVal gem_v_selective = GEM_NIL;
#line 957 "compiler/main.gem"
    GemVal _t1605 = (*gem_v_peek);
    GemVal _t1606 = _t1605.fn(_t1605.env, NULL, 0);
    static GemICacheSlot _ic_234 = {0};
    GemVal _t1609;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1606, "type", &_ic_234), gem_string("NAME")))) {
        _t1609 = gem_eq(gem_table_get_cached(_t1606, "type", &_ic_234), gem_string("NAME"));
    } else {
        GemVal _t1607 = (*gem_v_peek);
        GemVal _t1608 = _t1607.fn(_t1607.env, NULL, 0);
        static GemICacheSlot _ic_235 = {0};
        _t1609 = gem_eq(gem_table_get_cached(_t1608, "value", &_ic_235), gem_string("as"));
    }
        if (gem_truthy(_t1609)) {
#line 958 "compiler/main.gem"
    GemVal _t1610 = (*gem_v_advance);
            (void)(_t1610.fn(_t1610.env, NULL, 0));
#line 959 "compiler/main.gem"
    GemVal _t1611[] = {gem_string("NAME")};
    GemVal _t1612 = (*gem_v_expect);
    GemVal _t1613 = _t1612.fn(_t1612.env, _t1611, 1);
    static GemICacheSlot _ic_236 = {0};
            gem_v_alias_name = gem_table_get_cached(_t1613, "value", &_ic_236);
        } else {
#line 960 "compiler/main.gem"
    GemVal _t1614 = (*gem_v_peek);
    GemVal _t1615 = _t1614.fn(_t1614.env, NULL, 0);
    static GemICacheSlot _ic_237 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1615, "type", &_ic_237), gem_string("(")))) {
#line 961 "compiler/main.gem"
    GemVal _t1616 = (*gem_v_advance);
                (void)(_t1616.fn(_t1616.env, NULL, 0));
#line 962 "compiler/main.gem"
    GemVal _t1617 = gem_table_new();
    GemVal _t1618[] = {gem_string("NAME")};
    GemVal _t1619 = (*gem_v_expect);
    GemVal _t1620 = _t1619.fn(_t1619.env, _t1618, 1);
    static GemICacheSlot _ic_238 = {0};
    gem_table_set(_t1617, gem_int(0), gem_table_get_cached(_t1620, "value", &_ic_238));
                gem_v_selective = _t1617;
#line 963 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1621 = (*gem_v_peek);
                    GemVal _t1622 = _t1621.fn(_t1621.env, NULL, 0);
                    static GemICacheSlot _ic_239 = {0};
                    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1622, "type", &_ic_239), gem_string(",")))) break;
#line 964 "compiler/main.gem"
    GemVal _t1623 = (*gem_v_advance);
                    (void)(_t1623.fn(_t1623.env, NULL, 0));
#line 965 "compiler/main.gem"
    GemVal _t1624[] = {gem_string("NAME")};
    GemVal _t1625 = (*gem_v_expect);
    GemVal _t1626 = _t1625.fn(_t1625.env, _t1624, 1);
    static GemICacheSlot _ic_240 = {0};
    GemVal _t1627[] = {gem_v_selective, gem_table_get_cached(_t1626, "value", &_ic_240)};
                    (void)(gem_push_fn(NULL, _t1627, 2));
                }
#line 967 "compiler/main.gem"
    GemVal _t1628[] = {gem_string(")")};
    GemVal _t1629 = (*gem_v_expect);
                (void)(_t1629.fn(_t1629.env, _t1628, 1));
            }
        }
#line 969 "compiler/main.gem"
    GemVal _t1630 = gem_table_new();
    gem_table_set(_t1630, gem_string("tag"), gem_string("load"));
    gem_table_set(_t1630, gem_string("path"), gem_v_path);
    gem_table_set(_t1630, gem_string("alias"), gem_v_alias_name);
    gem_table_set(_t1630, gem_string("selective"), gem_v_selective);
        GemVal _t1631 = _t1630;
        gem_pop_frame();
        return _t1631;
    }
#line 973 "compiler/main.gem"
    GemVal _t1632 = gem_v_t;
    static GemICacheSlot _ic_241 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1632, "type", &_ic_241), gem_string("export")))) {
#line 974 "compiler/main.gem"
    GemVal _t1633 = (*gem_v_advance);
        (void)(_t1633.fn(_t1633.env, NULL, 0));
#line 975 "compiler/main.gem"
    GemVal _t1634 = gem_table_new();
    GemVal _t1635[] = {gem_string("NAME")};
    GemVal _t1636 = (*gem_v_expect);
    GemVal _t1637 = _t1636.fn(_t1636.env, _t1635, 1);
    static GemICacheSlot _ic_242 = {0};
    gem_table_set(_t1634, gem_int(0), gem_table_get_cached(_t1637, "value", &_ic_242));
        GemVal gem_v_names = _t1634;
#line 976 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1638 = (*gem_v_peek);
            GemVal _t1639 = _t1638.fn(_t1638.env, NULL, 0);
            static GemICacheSlot _ic_243 = {0};
            if (!gem_truthy(gem_eq(gem_table_get_cached(_t1639, "type", &_ic_243), gem_string(",")))) break;
#line 977 "compiler/main.gem"
    GemVal _t1640 = (*gem_v_advance);
            (void)(_t1640.fn(_t1640.env, NULL, 0));
#line 978 "compiler/main.gem"
    GemVal _t1641 = (*gem_v_skip_nl);
            (void)(_t1641.fn(_t1641.env, NULL, 0));
#line 979 "compiler/main.gem"
    GemVal _t1642[] = {gem_string("NAME")};
    GemVal _t1643 = (*gem_v_expect);
    GemVal _t1644 = _t1643.fn(_t1643.env, _t1642, 1);
    static GemICacheSlot _ic_244 = {0};
    GemVal _t1645[] = {gem_v_names, gem_table_get_cached(_t1644, "value", &_ic_244)};
            (void)(gem_push_fn(NULL, _t1645, 2));
        }
#line 981 "compiler/main.gem"
    GemVal _t1646[] = {gem_v_names};
    GemVal _t1647 = (*gem_v__mod_parser_make_export);
        GemVal _t1648 = _t1647.fn(_t1647.env, _t1646, 1);
        gem_pop_frame();
        return _t1648;
    }
#line 985 "compiler/main.gem"
    GemVal _t1649 = gem_v_t;
    static GemICacheSlot _ic_245 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1649, "type", &_ic_245), gem_string("extern")))) {
#line 986 "compiler/main.gem"
    GemVal _t1650 = (*gem_v_advance);
        (void)(_t1650.fn(_t1650.env, NULL, 0));
#line 987 "compiler/main.gem"
        GemVal gem_v_is_blocking = gem_bool(0);
#line 988 "compiler/main.gem"
    GemVal _t1651 = (*gem_v_peek);
    GemVal _t1652 = _t1651.fn(_t1651.env, NULL, 0);
    static GemICacheSlot _ic_246 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1652, "type", &_ic_246), gem_string("blocking")))) {
#line 989 "compiler/main.gem"
            gem_v_is_blocking = gem_bool(1);
#line 990 "compiler/main.gem"
    GemVal _t1653 = (*gem_v_advance);
            (void)(_t1653.fn(_t1653.env, NULL, 0));
        }
#line 992 "compiler/main.gem"
    GemVal _t1654 = (*gem_v_peek);
    GemVal _t1655 = _t1654.fn(_t1654.env, NULL, 0);
    static GemICacheSlot _ic_247 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1655, "type", &_ic_247), gem_string("fn")))) {
#line 993 "compiler/main.gem"
    GemVal _t1656 = (*gem_v_advance);
            (void)(_t1656.fn(_t1656.env, NULL, 0));
#line 994 "compiler/main.gem"
    GemVal _t1657[] = {gem_string("NAME")};
    GemVal _t1658 = (*gem_v_expect);
    GemVal _t1659 = _t1658.fn(_t1658.env, _t1657, 1);
    static GemICacheSlot _ic_248 = {0};
            GemVal gem_v_name = gem_table_get_cached(_t1659, "value", &_ic_248);
#line 995 "compiler/main.gem"
    GemVal _t1660[] = {gem_string("(")};
    GemVal _t1661 = (*gem_v_expect);
            (void)(_t1661.fn(_t1661.env, _t1660, 1));
#line 996 "compiler/main.gem"
    GemVal _t1662 = gem_table_new();
            GemVal gem_v_eparams = _t1662;
#line 997 "compiler/main.gem"
    GemVal _t1663 = (*gem_v_peek);
    GemVal _t1664 = _t1663.fn(_t1663.env, NULL, 0);
    static GemICacheSlot _ic_249 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t1664, "type", &_ic_249), gem_string(")")))) {
#line 998 "compiler/main.gem"
    GemVal _t1665[] = {gem_string("NAME")};
    GemVal _t1666 = (*gem_v_expect);
    GemVal _t1667 = _t1666.fn(_t1666.env, _t1665, 1);
    static GemICacheSlot _ic_250 = {0};
                GemVal gem_v_pname = gem_table_get_cached(_t1667, "value", &_ic_250);
#line 999 "compiler/main.gem"
    GemVal _t1668[] = {gem_string(":")};
    GemVal _t1669 = (*gem_v_expect);
                (void)(_t1669.fn(_t1669.env, _t1668, 1));
#line 1000 "compiler/main.gem"
    GemVal _t1670[] = {gem_string("NAME")};
    GemVal _t1671 = (*gem_v_expect);
    GemVal _t1672 = _t1671.fn(_t1671.env, _t1670, 1);
    static GemICacheSlot _ic_251 = {0};
                GemVal gem_v_ptype = gem_table_get_cached(_t1672, "value", &_ic_251);
#line 1001 "compiler/main.gem"
    GemVal _t1673[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1674 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1675[] = {gem_v_eparams, _t1674.fn(_t1674.env, _t1673, 2)};
                (void)(gem_push_fn(NULL, _t1675, 2));
#line 1002 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1676 = (*gem_v_peek);
                    GemVal _t1677 = _t1676.fn(_t1676.env, NULL, 0);
                    static GemICacheSlot _ic_252 = {0};
                    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1677, "type", &_ic_252), gem_string(",")))) break;
#line 1003 "compiler/main.gem"
    GemVal _t1678 = (*gem_v_advance);
                    (void)(_t1678.fn(_t1678.env, NULL, 0));
#line 1004 "compiler/main.gem"
    GemVal _t1679[] = {gem_string("NAME")};
    GemVal _t1680 = (*gem_v_expect);
    GemVal _t1681 = _t1680.fn(_t1680.env, _t1679, 1);
    static GemICacheSlot _ic_253 = {0};
                    GemVal gem_v_pn2 = gem_table_get_cached(_t1681, "value", &_ic_253);
#line 1005 "compiler/main.gem"
    GemVal _t1682[] = {gem_string(":")};
    GemVal _t1683 = (*gem_v_expect);
                    (void)(_t1683.fn(_t1683.env, _t1682, 1));
#line 1006 "compiler/main.gem"
    GemVal _t1684[] = {gem_string("NAME")};
    GemVal _t1685 = (*gem_v_expect);
    GemVal _t1686 = _t1685.fn(_t1685.env, _t1684, 1);
    static GemICacheSlot _ic_254 = {0};
                    GemVal gem_v_pt2 = gem_table_get_cached(_t1686, "value", &_ic_254);
#line 1007 "compiler/main.gem"
    GemVal _t1687[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1688 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1689[] = {gem_v_eparams, _t1688.fn(_t1688.env, _t1687, 2)};
                    (void)(gem_push_fn(NULL, _t1689, 2));
                }
            }
#line 1010 "compiler/main.gem"
    GemVal _t1690[] = {gem_string(")")};
    GemVal _t1691 = (*gem_v_expect);
            (void)(_t1691.fn(_t1691.env, _t1690, 1));
#line 1011 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 1012 "compiler/main.gem"
    GemVal _t1692 = (*gem_v_peek);
    GemVal _t1693 = _t1692.fn(_t1692.env, NULL, 0);
    static GemICacheSlot _ic_255 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1693, "type", &_ic_255), gem_string("->")))) {
#line 1013 "compiler/main.gem"
    GemVal _t1694 = (*gem_v_advance);
                (void)(_t1694.fn(_t1694.env, NULL, 0));
#line 1014 "compiler/main.gem"
    GemVal _t1695[] = {gem_string("NAME")};
    GemVal _t1696 = (*gem_v_expect);
    GemVal _t1697 = _t1696.fn(_t1696.env, _t1695, 1);
    static GemICacheSlot _ic_256 = {0};
                gem_v_ret_type = gem_table_get_cached(_t1697, "value", &_ic_256);
            }
#line 1016 "compiler/main.gem"
    GemVal _t1698[] = {gem_v_name, gem_v_eparams, gem_v_ret_type, gem_v_is_blocking};
    GemVal _t1699 = (*gem_v__mod_parser_make_extern_fn);
            GemVal _t1700 = _t1699.fn(_t1699.env, _t1698, 4);
            gem_pop_frame();
            return _t1700;
        } else {
#line 1017 "compiler/main.gem"
    GemVal _t1703;
    if (!gem_truthy(gem_not(gem_v_is_blocking))) {
        _t1703 = gem_not(gem_v_is_blocking);
    } else {
        GemVal _t1701 = (*gem_v_peek);
        GemVal _t1702 = _t1701.fn(_t1701.env, NULL, 0);
        static GemICacheSlot _ic_257 = {0};
        _t1703 = gem_eq(gem_table_get_cached(_t1702, "type", &_ic_257), gem_string("NAME"));
    }
    GemVal _t1706;
    if (!gem_truthy(_t1703)) {
        _t1706 = _t1703;
    } else {
        GemVal _t1704 = (*gem_v_peek);
        GemVal _t1705 = _t1704.fn(_t1704.env, NULL, 0);
        static GemICacheSlot _ic_258 = {0};
        _t1706 = gem_eq(gem_table_get_cached(_t1705, "value", &_ic_258), gem_string("include"));
    }
            if (gem_truthy(_t1706)) {
#line 1018 "compiler/main.gem"
    GemVal _t1707 = (*gem_v_advance);
                (void)(_t1707.fn(_t1707.env, NULL, 0));
#line 1019 "compiler/main.gem"
    GemVal _t1708[] = {gem_string("STRING")};
    GemVal _t1709 = (*gem_v_expect);
    GemVal _t1710 = _t1709.fn(_t1709.env, _t1708, 1);
    static GemICacheSlot _ic_259 = {0};
                GemVal gem_v_path = gem_table_get_cached(_t1710, "value", &_ic_259);
#line 1020 "compiler/main.gem"
    GemVal _t1711[] = {gem_v_path};
    GemVal _t1712 = (*gem_v__mod_parser_make_extern_include);
                GemVal _t1713 = _t1712.fn(_t1712.env, _t1711, 1);
                gem_pop_frame();
                return _t1713;
            } else {
#line 1022 "compiler/main.gem"
    GemVal _t1714 = (*gem_v_peek);
    GemVal _t1715 = _t1714.fn(_t1714.env, NULL, 0);
    static GemICacheSlot _ic_260 = {0};
    GemVal _t1716 = (*gem_v_peek);
    GemVal _t1717 = _t1716.fn(_t1716.env, NULL, 0);
    static GemICacheSlot _ic_261 = {0};
    GemVal _t1718 = (*gem_v_peek);
    GemVal _t1719 = _t1718.fn(_t1718.env, NULL, 0);
    static GemICacheSlot _ic_262 = {0};
    GemVal _t1720[] = {gem_table_get_cached(_t1719, "value", &_ic_262)};
    GemVal _t1721[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1715, "line", &_ic_260), gem_table_get_cached(_t1717, "col", &_ic_261), gem_len_fn(NULL, _t1720, 1), gem_string("expected 'fn' after 'extern blocking'"), GEM_NIL};
    GemVal _t1722 = (*gem_v__mod_parser_compile_error);
                (void)(_t1722.fn(_t1722.env, _t1721, 7));
            }
        }
    }
#line 1027 "compiler/main.gem"
    GemVal _t1723 = gem_v_t;
    static GemICacheSlot _ic_263 = {0};
    GemVal _t1725;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1723, "type", &_ic_263), gem_string("NAME")))) {
        _t1725 = gem_eq(gem_table_get_cached(_t1723, "type", &_ic_263), gem_string("NAME"));
    } else {
        GemVal _t1724 = gem_v_t;
        static GemICacheSlot _ic_264 = {0};
        _t1725 = gem_eq(gem_table_get_cached(_t1724, "value", &_ic_264), gem_string("receive"));
    }
    if (gem_truthy(_t1725)) {
#line 1028 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 1029 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1726[] = {gem_v_rla};
            GemVal _t1727 = (*gem_v_peek_at);
            GemVal _t1728 = _t1727.fn(_t1727.env, _t1726, 1);
            static GemICacheSlot _ic_265 = {0};
            if (!gem_truthy(gem_eq(gem_table_get_cached(_t1728, "type", &_ic_265), gem_string("NEWLINE")))) break;
#line 1030 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 1032 "compiler/main.gem"
    GemVal _t1729[] = {gem_v_rla};
    GemVal _t1730 = (*gem_v_peek_at);
    GemVal _t1731 = _t1730.fn(_t1730.env, _t1729, 1);
    static GemICacheSlot _ic_266 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1731, "type", &_ic_266), gem_string("when")))) {
#line 1033 "compiler/main.gem"
    GemVal _t1732 = gem_v_t;
    static GemICacheSlot _ic_267 = {0};
            GemVal gem_v_rline = gem_table_get_cached(_t1732, "line", &_ic_267);
#line 1034 "compiler/main.gem"
    GemVal _t1733 = (*gem_v_advance);
            (void)(_t1733.fn(_t1733.env, NULL, 0));
#line 1035 "compiler/main.gem"
    GemVal _t1734 = (*gem_v_skip_nl);
            (void)(_t1734.fn(_t1734.env, NULL, 0));
#line 1036 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 1037 "compiler/main.gem"
    GemVal _t1735[] = {gem_string("_recv_"), (*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_interp(2, _t1735);
#line 1038 "compiler/main.gem"
    GemVal _t1736[] = {gem_v_recv_var};
    GemVal _t1737 = (*gem_v__mod_parser_make_var);
            GemVal gem_v_recv_var_expr = _t1737.fn(_t1737.env, _t1736, 1);
#line 1039 "compiler/main.gem"
    GemVal _t1738 = gem_table_new();
            GemVal gem_v_rarms = _t1738;
#line 1040 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1739 = (*gem_v_peek);
                GemVal _t1740 = _t1739.fn(_t1739.env, NULL, 0);
                static GemICacheSlot _ic_268 = {0};
                if (!gem_truthy(gem_eq(gem_table_get_cached(_t1740, "type", &_ic_268), gem_string("when")))) break;
#line 1041 "compiler/main.gem"
    GemVal _t1741 = (*gem_v_advance);
                (void)(_t1741.fn(_t1741.env, NULL, 0));
#line 1042 "compiler/main.gem"
    GemVal _t1742 = (*gem_v_peek);
    GemVal _t1743 = _t1742.fn(_t1742.env, NULL, 0);
    static GemICacheSlot _ic_269 = {0};
                GemVal gem_v_rpt = gem_table_get_cached(_t1743, "type", &_ic_269);
#line 1043 "compiler/main.gem"
    GemVal _t1744[] = {gem_int(1)};
    GemVal _t1745 = (*gem_v_peek_at);
    GemVal _t1746 = _t1745.fn(_t1745.env, _t1744, 1);
    static GemICacheSlot _ic_270 = {0};
                GemVal gem_v_rnext = gem_table_get_cached(_t1746, "type", &_ic_270);
#line 1044 "compiler/main.gem"
    GemVal _t1747;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1747 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1747 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1747)) {
#line 1045 "compiler/main.gem"
    GemVal _t1748[] = {gem_v_recv_var_expr};
    GemVal _t1749 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1749.fn(_t1749.env, _t1748, 1);
#line 1046 "compiler/main.gem"
    GemVal _t1750 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1750.fn(_t1750.env, NULL, 0);
#line 1047 "compiler/main.gem"
    GemVal _t1751 = gem_table_new();
    gem_table_set(_t1751, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1751, gem_string("body"), gem_v_rwbody);
    GemVal _t1752[] = {gem_v_rarms, _t1751};
                    (void)(gem_push_fn(NULL, _t1752, 2));
                } else {
#line 1048 "compiler/main.gem"
    GemVal _t1758;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1758 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1753;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1753 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1753 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1754;
        if (gem_truthy(_t1753)) {
                _t1754 = _t1753;
        } else {
                _t1754 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1755;
        if (gem_truthy(_t1754)) {
                _t1755 = _t1754;
        } else {
                _t1755 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1756;
        if (gem_truthy(_t1755)) {
                _t1756 = _t1755;
        } else {
                _t1756 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1757;
        if (gem_truthy(_t1756)) {
                _t1757 = _t1756;
        } else {
                _t1757 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1758 = _t1757;
    }
                    if (gem_truthy(_t1758)) {
#line 1049 "compiler/main.gem"
    GemVal _t1759[] = {gem_v_recv_var_expr};
    GemVal _t1760 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1760.fn(_t1760.env, _t1759, 1);
#line 1050 "compiler/main.gem"
    GemVal _t1761 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1761.fn(_t1761.env, NULL, 0);
#line 1051 "compiler/main.gem"
    GemVal _t1762 = gem_table_new();
    gem_table_set(_t1762, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1762, gem_string("body"), gem_v_rwbody);
    GemVal _t1763[] = {gem_v_rarms, _t1762};
                        (void)(gem_push_fn(NULL, _t1763, 2));
                    } else {
#line 1053 "compiler/main.gem"
    GemVal _t1764 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1764.fn(_t1764.env, NULL, 0);
#line 1054 "compiler/main.gem"
    GemVal _t1765 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1765.fn(_t1765.env, NULL, 0);
#line 1055 "compiler/main.gem"
    GemVal _t1766 = gem_table_new();
    GemVal _t1767 = gem_table_new();
    GemVal _t1768[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    GemVal _t1769 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1767, gem_string("condition"), _t1769.fn(_t1769.env, _t1768, 3));
    GemVal _t1770 = gem_table_new();
    gem_table_set(_t1767, gem_string("bindings"), _t1770);
    gem_table_set(_t1766, gem_string("pattern"), _t1767);
    gem_table_set(_t1766, gem_string("body"), gem_v_rwbody);
    GemVal _t1771[] = {gem_v_rarms, _t1766};
                        (void)(gem_push_fn(NULL, _t1771, 2));
                    }
                }
#line 1057 "compiler/main.gem"
    GemVal _t1772 = (*gem_v_skip_nl);
                (void)(_t1772.fn(_t1772.env, NULL, 0));
            }
#line 1059 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 1060 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 1061 "compiler/main.gem"
    GemVal _t1773 = (*gem_v_peek);
    GemVal _t1774 = _t1773.fn(_t1773.env, NULL, 0);
    static GemICacheSlot _ic_271 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1774, "type", &_ic_271), gem_string("after")))) {
#line 1062 "compiler/main.gem"
    GemVal _t1775 = (*gem_v_advance);
                (void)(_t1775.fn(_t1775.env, NULL, 0));
#line 1063 "compiler/main.gem"
    GemVal _t1776 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1776.fn(_t1776.env, NULL, 0);
#line 1064 "compiler/main.gem"
    GemVal _t1777 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1777.fn(_t1777.env, NULL, 0);
            }
#line 1066 "compiler/main.gem"
    GemVal _t1778[] = {gem_string("end")};
    GemVal _t1779 = (*gem_v_expect);
            (void)(_t1779.fn(_t1779.env, _t1778, 1));
#line 1067 "compiler/main.gem"
    GemVal _t1780[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
    GemVal _t1781 = (*gem_v__mod_parser_make_receive_match);
            GemVal _t1782 = _t1781.fn(_t1781.env, _t1780, 5);
            gem_pop_frame();
            return _t1782;
        }
    }
#line 1072 "compiler/main.gem"
    GemVal _t1783 = (*gem_v_parse_expr);
    GemVal _t1784 = _t1783.fn(_t1783.env, NULL, 0);
    gem_pop_frame();
    return _t1784;
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
#line 1081 "compiler/main.gem"
    GemVal _t1786 = gem_table_new();
    GemVal gem_v_stmts = _t1786;
#line 1082 "compiler/main.gem"
    GemVal _t1787 = (*gem_v_skip_nl);
    (void)(_t1787.fn(_t1787.env, NULL, 0));
#line 1083 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1788 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1788.fn(_t1788.env, NULL, 0)))) break;
#line 1084 "compiler/main.gem"
    GemVal _t1789 = (*gem_v_parse_stmt);
    GemVal _t1790[] = {gem_v_stmts, _t1789.fn(_t1789.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1790, 2));
#line 1085 "compiler/main.gem"
    GemVal _t1791 = (*gem_v_skip_nl);
        (void)(_t1791.fn(_t1791.env, NULL, 0));
    }
#line 1087 "compiler/main.gem"
    GemVal _t1792[] = {gem_v_stmts};
    GemVal _t1793 = (*gem_v__mod_parser_make_program);
    GemVal _t1794 = _t1793.fn(_t1793.env, _t1792, 1);
    gem_pop_frame();
    return _t1794;
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
#line 108 "compiler/main.gem"
    GemVal *gem_v_parse_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_expr = GEM_NIL;
#line 109 "compiler/main.gem"
    GemVal *gem_v_parse_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_stmt = GEM_NIL;
#line 110 "compiler/main.gem"
    GemVal *gem_v_parse_unary = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_unary = GEM_NIL;
#line 111 "compiler/main.gem"
    GemVal *gem_v_parse_not = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_not = GEM_NIL;
#line 112 "compiler/main.gem"
    GemVal *gem_v_parse_pattern = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_pattern = GEM_NIL;
#line 120 "compiler/main.gem"
    struct _closure__anon_7 *_t491 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t491->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t491->gem_v_advance = gem_v_advance;
    _t491->gem_v_expect = gem_v_expect;
    _t491->gem_v_file = gem_v_file;
    _t491->gem_v_parse_expr = gem_v_parse_expr;
    _t491->gem_v_peek = gem_v_peek;
    _t491->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t491);
#line 157 "compiler/main.gem"
    struct _closure__anon_8 *_t517 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t517->gem_v_at_end = gem_v_at_end;
    _t517->gem_v_parse_stmt = gem_v_parse_stmt;
    _t517->gem_v_peek = gem_v_peek;
    _t517->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t517);
#line 170 "compiler/main.gem"
    struct _closure__anon_9 *_t528 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t528->gem_v_at_end = gem_v_at_end;
    _t528->gem_v_parse_stmt = gem_v_parse_stmt;
    _t528->gem_v_peek = gem_v_peek;
    _t528->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t528);
#line 183 "compiler/main.gem"
    struct _closure__anon_10 *_t702 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t702->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t702->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t702->gem_v__mod_parser_make_array = &gem_v__mod_parser_make_array;
    _t702->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t702->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t702->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t702->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t702->gem_v__mod_parser_make_interp = &gem_v__mod_parser_make_interp;
    _t702->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t702->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t702->gem_v__mod_parser_make_table = &gem_v__mod_parser_make_table;
    _t702->gem_v__mod_parser_make_table_entry = &gem_v__mod_parser_make_table_entry;
    _t702->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t702->gem_v_advance = gem_v_advance;
    _t702->gem_v_at_end = gem_v_at_end;
    _t702->gem_v_expect = gem_v_expect;
    _t702->gem_v_file = gem_v_file;
    _t702->gem_v_fn_depth = gem_v_fn_depth;
    _t702->gem_v_parse_expr = gem_v_parse_expr;
    _t702->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t702->gem_v_parse_params = gem_v_parse_params;
    _t702->gem_v_peek = gem_v_peek;
    _t702->gem_v_peek_at = gem_v_peek_at;
    _t702->gem_v_skip_nl = gem_v_skip_nl;
    _t702->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t702);
#line 321 "compiler/main.gem"
    struct _closure__anon_11 *_t800 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t800->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t800->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t800->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t800->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t800->gem_v_advance = gem_v_advance;
    _t800->gem_v_expect = gem_v_expect;
    _t800->gem_v_fn_depth = gem_v_fn_depth;
    _t800->gem_v_parse_atom = gem_v_parse_atom;
    _t800->gem_v_parse_expr = gem_v_parse_expr;
    _t800->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t800->gem_v_parse_params = gem_v_parse_params;
    _t800->gem_v_peek = gem_v_peek;
    _t800->gem_v_peek_at = gem_v_peek_at;
    _t800->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t800);
#line 427 "compiler/main.gem"
    struct _closure__anon_12 *_t810 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t810->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t810->gem_v_advance = gem_v_advance;
    _t810->gem_v_parse_call = gem_v_parse_call;
    _t810->gem_v_parse_unary = gem_v_parse_unary;
    _t810->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t810);
#line 436 "compiler/main.gem"
    struct _closure__anon_13 *_t826 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t826->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t826->gem_v_advance = gem_v_advance;
    _t826->gem_v_parse_unary = gem_v_parse_unary;
    _t826->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t826);
#line 447 "compiler/main.gem"
    struct _closure__anon_14 *_t839 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t839->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t839->gem_v_advance = gem_v_advance;
    _t839->gem_v_parse_mul = gem_v_parse_mul;
    _t839->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t839);
#line 458 "compiler/main.gem"
    struct _closure__anon_15 *_t867 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t867->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t867->gem_v_advance = gem_v_advance;
    _t867->gem_v_parse_add = gem_v_parse_add;
    _t867->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t867);
#line 469 "compiler/main.gem"
    struct _closure__anon_16 *_t877 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t877->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t877->gem_v_advance = gem_v_advance;
    _t877->gem_v_parse_compare = gem_v_parse_compare;
    _t877->gem_v_parse_not = gem_v_parse_not;
    _t877->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t877);
#line 478 "compiler/main.gem"
    struct _closure__anon_17 *_t886 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t886->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t886->gem_v_advance = gem_v_advance;
    _t886->gem_v_parse_not = gem_v_parse_not;
    _t886->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t886);
#line 489 "compiler/main.gem"
    struct _closure__anon_18 *_t895 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t895->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t895->gem_v_advance = gem_v_advance;
    _t895->gem_v_parse_and = gem_v_parse_and;
    _t895->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t895);
#line 500 "compiler/main.gem"
    struct _closure__anon_19 *_t953 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t953->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t953->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t953->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t953->gem_v__mod_parser_make_dot_assign = &gem_v__mod_parser_make_dot_assign;
    _t953->gem_v__mod_parser_make_index_assign = &gem_v__mod_parser_make_index_assign;
    _t953->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t953->gem_v_advance = gem_v_advance;
    _t953->gem_v_file = gem_v_file;
    _t953->gem_v_parse_expr = gem_v_parse_expr;
    _t953->gem_v_parse_or = gem_v_parse_or;
    _t953->gem_v_peek = gem_v_peek;
    _t953->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t953);
#line 535 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 539 "compiler/main.gem"
    struct _closure__anon_20 *_t1162 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1162->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1162->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1162->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t1162->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1162->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t1162->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1162->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1162->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1162->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t1162->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t1162->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1162->gem_v_advance = gem_v_advance;
    _t1162->gem_v_expect = gem_v_expect;
    _t1162->gem_v_file = gem_v_file;
    _t1162->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1162->gem_v_peek = gem_v_peek;
    _t1162->gem_v_peek_at = gem_v_peek_at;
    _t1162->gem_v_skip_nl = gem_v_skip_nl;
    _t1162->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t1162);
#line 667 "compiler/main.gem"
    struct _closure__anon_22 *_t1785 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1785->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1785->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t1785->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1785->gem_v__mod_parser_make_break = &gem_v__mod_parser_make_break;
    _t1785->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1785->gem_v__mod_parser_make_continue = &gem_v__mod_parser_make_continue;
    _t1785->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t1785->gem_v__mod_parser_make_export = &gem_v__mod_parser_make_export;
    _t1785->gem_v__mod_parser_make_extern_fn = &gem_v__mod_parser_make_extern_fn;
    _t1785->gem_v__mod_parser_make_extern_include = &gem_v__mod_parser_make_extern_include;
    _t1785->gem_v__mod_parser_make_extern_param = &gem_v__mod_parser_make_extern_param;
    _t1785->gem_v__mod_parser_make_fn_def = &gem_v__mod_parser_make_fn_def;
    _t1785->gem_v__mod_parser_make_if = &gem_v__mod_parser_make_if;
    _t1785->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1785->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1785->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1785->gem_v__mod_parser_make_match = &gem_v__mod_parser_make_match;
    _t1785->gem_v__mod_parser_make_receive_match = &gem_v__mod_parser_make_receive_match;
    _t1785->gem_v__mod_parser_make_return = &gem_v__mod_parser_make_return;
    _t1785->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1785->gem_v__mod_parser_make_when = &gem_v__mod_parser_make_when;
    _t1785->gem_v__mod_parser_make_while = &gem_v__mod_parser_make_while;
    _t1785->gem_v_advance = gem_v_advance;
    _t1785->gem_v_expect = gem_v_expect;
    _t1785->gem_v_file = gem_v_file;
    _t1785->gem_v_fn_depth = gem_v_fn_depth;
    _t1785->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1785->gem_v_parse_body = gem_v_parse_body;
    _t1785->gem_v_parse_expr = gem_v_parse_expr;
    _t1785->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1785->gem_v_parse_params = gem_v_parse_params;
    _t1785->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1785->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1785->gem_v_peek = gem_v_peek;
    _t1785->gem_v_peek_at = gem_v_peek_at;
    _t1785->gem_v_skip_nl = gem_v_skip_nl;
    _t1785->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1785);
#line 1076 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 1080 "compiler/main.gem"
    struct _closure__anon_23 *_t1795 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1795->gem_v__mod_parser_make_program = &gem_v__mod_parser_make_program;
    _t1795->gem_v_at_end = gem_v_at_end;
    _t1795->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1795->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1795);
    GemVal _t1796 = gem_table_new();
    gem_table_set(_t1796, gem_string("parse"), gem_v_parse);
    GemVal _t1797 = _t1796;
    gem_pop_frame();
    return _t1797;
}

static GemVal gem_fn__mod_codegen_escape_c_string(void *_env, GemVal *args, int argc) {
#line 7 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_escape_c_string", "compiler/main.gem", 7);
#line 8 "compiler/main.gem"
    GemVal _t1798[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1798, 1);
#line 9 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 10 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 10 "compiler/main.gem"
    GemVal _t1799[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1799, 1);
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
    GemVal _t1800[] = {gem_v_b, gem_string("\\\\")};
            (void)(gem_buf_push_fn(NULL, _t1800, 2));
        } else {
#line 14 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 15 "compiler/main.gem"
    GemVal _t1801[] = {gem_v_b, gem_string("\\\"")};
                (void)(gem_buf_push_fn(NULL, _t1801, 2));
            } else {
#line 16 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 17 "compiler/main.gem"
    GemVal _t1802[] = {gem_v_b, gem_string("\\n")};
                    (void)(gem_buf_push_fn(NULL, _t1802, 2));
                } else {
#line 18 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 19 "compiler/main.gem"
    GemVal _t1803[] = {gem_v_b, gem_string("\\r")};
                        (void)(gem_buf_push_fn(NULL, _t1803, 2));
                    } else {
#line 20 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 21 "compiler/main.gem"
    GemVal _t1804[] = {gem_v_b, gem_string("\\t")};
                            (void)(gem_buf_push_fn(NULL, _t1804, 2));
                        } else {
#line 22 "compiler/main.gem"
    GemVal _t1805[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1805, 1), gem_int(0)))) {
#line 23 "compiler/main.gem"
    GemVal _t1806[] = {gem_v_b, gem_string("\\0")};
                                (void)(gem_buf_push_fn(NULL, _t1806, 2));
                            } else {
#line 25 "compiler/main.gem"
    GemVal _t1807[] = {gem_v_b, gem_v_ch};
                                (void)(gem_buf_push_fn(NULL, _t1807, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 28 "compiler/main.gem"
    GemVal _t1808[] = {gem_v_b};
    GemVal _t1809 = gem_buf_str_fn(NULL, _t1808, 1);
    gem_pop_frame();
    return _t1809;
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
    GemVal _t1810[] = {gem_v_b, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t1810, 2));
    }

#line 36 "compiler/main.gem"
    GemVal _t1811[] = {gem_v_b};
    GemVal _t1812 = gem_buf_str_fn(NULL, _t1811, 1);
    gem_pop_frame();
    return _t1812;
}

static GemVal gem_fn__mod_codegen_pad_for(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_pad_for", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal _t1813[] = {gem_string("    "), gem_v_indent};
    GemVal _t1814 = gem_fn__mod_codegen_repeat_str(NULL, _t1813, 2);
    gem_pop_frame();
    return _t1814;
}

static GemVal gem_fn__mod_codegen_format_float(void *_env, GemVal *args, int argc) {
#line 43 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_format_float", "compiler/main.gem", 43);
#line 44 "compiler/main.gem"
    GemVal _t1815[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1815, 1);
#line 46 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 47 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 47 "compiler/main.gem"
    GemVal _t1816[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1816, 1);
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
    GemVal _t1817 = gem_v_s;
    gem_pop_frame();
    return _t1817;
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
    GemVal _t1818 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1818;
}

static GemVal gem_fn__mod_codegen_set_union(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_union", "compiler/main.gem", 70);
#line 71 "compiler/main.gem"
    GemVal _t1819 = gem_table_new();
    GemVal gem_v_result = _t1819;
#line 72 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 72 "compiler/main.gem"
    GemVal _t1820[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_len_4 = gem_len_fn(NULL, _t1820, 1);
#line 72 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 72 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_len_4))) break;
#line 72 "compiler/main.gem"
    GemVal _t1821[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1821, 2);
#line 72 "compiler/main.gem"
    GemVal _t1822[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1822, 2);
#line 72 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 73 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

#line 75 "compiler/main.gem"
    GemVal gem_v__for_tbl_5 = gem_v_b;
#line 75 "compiler/main.gem"
    GemVal _t1823[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_len_5 = gem_len_fn(NULL, _t1823, 1);
#line 75 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 75 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_len_5))) break;
#line 75 "compiler/main.gem"
    GemVal _t1824[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1824, 2);
#line 75 "compiler/main.gem"
    GemVal _t1825[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1825, 2);
#line 75 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 76 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1826 = gem_v_result;
    gem_pop_frame();
    return _t1826;
}

static GemVal gem_fn__mod_codegen_set_intersect(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_intersect", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t1827 = gem_table_new();
    GemVal gem_v_result = _t1827;
#line 83 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 83 "compiler/main.gem"
    GemVal _t1828[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_len_6 = gem_len_fn(NULL, _t1828, 1);
#line 83 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 83 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_len_6))) break;
#line 83 "compiler/main.gem"
    GemVal _t1829[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1829, 2);
#line 83 "compiler/main.gem"
    GemVal _t1830[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1830, 2);
#line 83 "compiler/main.gem"
        gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 84 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_v_k), gem_bool(1)))) {
#line 85 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
        }
    }

    GemVal _t1831 = gem_v_result;
    gem_pop_frame();
    return _t1831;
}

static GemVal gem_fn__mod_codegen_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 91 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_sorted_array_to_set", "compiler/main.gem", 91);
#line 92 "compiler/main.gem"
    GemVal _t1832 = gem_table_new();
    GemVal gem_v_s = _t1832;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1833[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1833, 1)))) break;
#line 93 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 93 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 94 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1834 = gem_v_s;
    gem_pop_frame();
    return _t1834;
}

static GemVal gem_fn__mod_codegen_is_node(void *_env, GemVal *args, int argc) {
#line 99 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_is_node", "compiler/main.gem", 99);
    GemVal _t1835[] = {gem_v_node};
    GemVal _t1837;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1835, 1), gem_string("table")))) {
        _t1837 = gem_eq(gem_type_fn(NULL, _t1835, 1), gem_string("table"));
    } else {
        GemVal _t1836 = gem_v_node;
        static GemICacheSlot _ic_272 = {0};
        _t1837 = gem_eq(gem_table_get_cached(_t1836, "tag", &_ic_272), gem_v_tag);
    }
    GemVal _t1838 = _t1837;
    gem_pop_frame();
    return _t1838;
}

static GemVal gem_fn__mod_codegen_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_collect_block_lets", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t1839[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1839, 2))) {
        {
#line 107 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal _t1840 = gem_v_stmt;
    static GemICacheSlot _ic_273 = {0};
    GemVal _t1841[] = {gem_table_get_cached(_t1840, "stmts", &_ic_273)};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1841, 1);
#line 107 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 107 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 107 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 108 "compiler/main.gem"
    GemVal _t1842 = gem_v_stmt;
    static GemICacheSlot _ic_274 = {0};
    GemVal _t1843[] = {gem_table_get(gem_table_get_cached(_t1842, "stmts", &_ic_274), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1843, 2))) {
#line 109 "compiler/main.gem"
    GemVal _t1844 = gem_v_stmt;
    static GemICacheSlot _ic_275 = {0};
    GemVal _t1845 = gem_table_get(gem_table_get_cached(_t1844, "stmts", &_ic_275), gem_v_j);
    static GemICacheSlot _ic_276 = {0};
    GemVal _t1846[] = {gem_v_result, gem_table_get_cached(_t1845, "name", &_ic_276)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1846, 2));
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
    GemVal _t1847[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1847, 1);
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
    GemVal _t1848[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1848, 2))) {
#line 119 "compiler/main.gem"
    GemVal _t1849 = gem_v_s;
    static GemICacheSlot _ic_277 = {0};
    GemVal _t1850[] = {gem_v_result, gem_table_get_cached(_t1849, "name", &_ic_277)};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1850, 2));
            }
#line 121 "compiler/main.gem"
    GemVal _t1851[] = {gem_v_s, gem_v_result};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t1851, 2));
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
#line 243 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1867[] = {gem_string("_t"), (*gem_v_tmp_counter)};
    GemVal _t1868 = gem_interp(2, _t1867);
    gem_pop_frame();
    return _t1868;
}

struct _closure__anon_25 {
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 248 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1870[] = {gem_string("_anon_"), (*gem_v_anon_counter)};
    GemVal _t1871 = gem_interp(2, _t1870);
    gem_pop_frame();
    return _t1871;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1873[] = {gem_string("gem_v_"), gem_v_name};
    GemVal _t1874 = gem_interp(2, _t1873);
    gem_pop_frame();
    return _t1874;
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
#line 262 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 263 "compiler/main.gem"
        GemVal _t1875 = GEM_NIL;
        gem_pop_frame();
        return _t1875;
    }
#line 265 "compiler/main.gem"
    GemVal _t1876[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1876, 1), gem_string("table")))) {
#line 266 "compiler/main.gem"
        GemVal _t1877 = GEM_NIL;
        gem_pop_frame();
        return _t1877;
    }
#line 268 "compiler/main.gem"
    GemVal _t1878 = gem_v_node;
    static GemICacheSlot _ic_278 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1878, "tag", &_ic_278), GEM_NIL))) {
#line 269 "compiler/main.gem"
        GemVal _t1879 = GEM_NIL;
        gem_pop_frame();
        return _t1879;
    }
#line 272 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1880[] = {gem_v__match_11};
    GemVal _t1882;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1880, 1), gem_string("table")))) {
        _t1882 = gem_eq(gem_type_fn(NULL, _t1880, 1), gem_string("table"));
    } else {
        GemVal _t1881[] = {gem_v__match_11, gem_string("tag")};
        _t1882 = gem_has_key_fn(NULL, _t1881, 2);
    }
    GemVal _t1883;
    if (!gem_truthy(_t1882)) {
        _t1883 = _t1882;
    } else {
        _t1883 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1885;
    if (!gem_truthy(_t1883)) {
        _t1885 = _t1883;
    } else {
        GemVal _t1884[] = {gem_v__match_11, gem_string("name")};
        _t1885 = gem_has_key_fn(NULL, _t1884, 2);
    }
    if (gem_truthy(_t1885)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 274 "compiler/main.gem"
    GemVal _t1886[] = {gem_v_defined, gem_v_name};
    GemVal _t1890;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1886, 2)))) {
        _t1890 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1886, 2));
    } else {
        GemVal _t1887[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1889;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1887, 2)))) {
                _t1889 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1887, 2));
        } else {
                GemVal _t1888[] = {(*gem_v_local_names), gem_v_name};
                _t1889 = gem_fn__mod_codegen_set_contains(NULL, _t1888, 2);
        }
        _t1890 = _t1889;
    }
    GemVal _t1892;
    if (!gem_truthy(_t1890)) {
        _t1892 = _t1890;
    } else {
        GemVal _t1891[] = {(*gem_v_defined_fns), gem_v_name};
        _t1892 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1891, 2));
    }
        if (gem_truthy(_t1892)) {
#line 275 "compiler/main.gem"
    GemVal _t1893[] = {gem_v_free, gem_v_name};
            GemVal _t1894 = gem_fn__mod_codegen_set_add(NULL, _t1893, 2);
            gem_pop_frame();
            return _t1894;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1895[] = {gem_v__match_11};
    GemVal _t1897;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1895, 1), gem_string("table")))) {
        _t1897 = gem_eq(gem_type_fn(NULL, _t1895, 1), gem_string("table"));
    } else {
        GemVal _t1896[] = {gem_v__match_11, gem_string("tag")};
        _t1897 = gem_has_key_fn(NULL, _t1896, 2);
    }
    GemVal _t1898;
    if (!gem_truthy(_t1897)) {
        _t1898 = _t1897;
    } else {
        _t1898 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1900;
    if (!gem_truthy(_t1898)) {
        _t1900 = _t1898;
    } else {
        GemVal _t1899[] = {gem_v__match_11, gem_string("value")};
        _t1900 = gem_has_key_fn(NULL, _t1899, 2);
    }
    if (gem_truthy(_t1900)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 278 "compiler/main.gem"
    GemVal _t1901[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1902 = (*gem_v_collect_free_node);
        GemVal _t1903 = _t1902.fn(_t1902.env, _t1901, 3);
        gem_pop_frame();
        return _t1903;
    } else {
    GemVal _t1904[] = {gem_v__match_11};
    GemVal _t1906;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1904, 1), gem_string("table")))) {
        _t1906 = gem_eq(gem_type_fn(NULL, _t1904, 1), gem_string("table"));
    } else {
        GemVal _t1905[] = {gem_v__match_11, gem_string("tag")};
        _t1906 = gem_has_key_fn(NULL, _t1905, 2);
    }
    GemVal _t1907;
    if (!gem_truthy(_t1906)) {
        _t1907 = _t1906;
    } else {
        _t1907 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1909;
    if (!gem_truthy(_t1907)) {
        _t1909 = _t1907;
    } else {
        GemVal _t1908[] = {gem_v__match_11, gem_string("name")};
        _t1909 = gem_has_key_fn(NULL, _t1908, 2);
    }
    GemVal _t1911;
    if (!gem_truthy(_t1909)) {
        _t1911 = _t1909;
    } else {
        GemVal _t1910[] = {gem_v__match_11, gem_string("value")};
        _t1911 = gem_has_key_fn(NULL, _t1910, 2);
    }
    if (gem_truthy(_t1911)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 280 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_defined, gem_v_name};
    GemVal _t1916;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1912, 2)))) {
        _t1916 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1912, 2));
    } else {
        GemVal _t1913[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1915;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1913, 2)))) {
                _t1915 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1913, 2));
        } else {
                GemVal _t1914[] = {(*gem_v_local_names), gem_v_name};
                _t1915 = gem_fn__mod_codegen_set_contains(NULL, _t1914, 2);
        }
        _t1916 = _t1915;
    }
    GemVal _t1918;
    if (!gem_truthy(_t1916)) {
        _t1918 = _t1916;
    } else {
        GemVal _t1917[] = {(*gem_v_defined_fns), gem_v_name};
        _t1918 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1917, 2));
    }
        if (gem_truthy(_t1918)) {
#line 281 "compiler/main.gem"
    GemVal _t1919[] = {gem_v_free, gem_v_name};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1919, 2));
        }
#line 283 "compiler/main.gem"
    GemVal _t1920[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1921 = (*gem_v_collect_free_node);
        GemVal _t1922 = _t1921.fn(_t1921.env, _t1920, 3);
        gem_pop_frame();
        return _t1922;
    } else {
    GemVal _t1923[] = {gem_v__match_11};
    GemVal _t1925;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1923, 1), gem_string("table")))) {
        _t1925 = gem_eq(gem_type_fn(NULL, _t1923, 1), gem_string("table"));
    } else {
        GemVal _t1924[] = {gem_v__match_11, gem_string("tag")};
        _t1925 = gem_has_key_fn(NULL, _t1924, 2);
    }
    GemVal _t1926;
    if (!gem_truthy(_t1925)) {
        _t1926 = _t1925;
    } else {
        _t1926 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1928;
    if (!gem_truthy(_t1926)) {
        _t1928 = _t1926;
    } else {
        GemVal _t1927[] = {gem_v__match_11, gem_string("params")};
        _t1928 = gem_has_key_fn(NULL, _t1927, 2);
    }
    GemVal _t1930;
    if (!gem_truthy(_t1928)) {
        _t1930 = _t1928;
    } else {
        GemVal _t1929[] = {gem_v__match_11, gem_string("rest_param")};
        _t1930 = gem_has_key_fn(NULL, _t1929, 2);
    }
    GemVal _t1932;
    if (!gem_truthy(_t1930)) {
        _t1932 = _t1930;
    } else {
        GemVal _t1931[] = {gem_v__match_11, gem_string("body")};
        _t1932 = gem_has_key_fn(NULL, _t1931, 2);
    }
    if (gem_truthy(_t1932)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 285 "compiler/main.gem"
    GemVal _t1933[] = {gem_v_params};
    GemVal _t1934[] = {gem_v_defined, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t1933, 1)};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_set_union(NULL, _t1934, 2);
#line 286 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 287 "compiler/main.gem"
    GemVal _t1935[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1935, 2));
        }
#line 289 "compiler/main.gem"
    GemVal _t1936[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1937 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1937.fn(_t1937.env, _t1936, 2);
#line 290 "compiler/main.gem"
    GemVal _t1938 = gem_v_node;
    static GemICacheSlot _ic_279 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t1938, "defaults", &_ic_279), GEM_NIL))) {
#line 291 "compiler/main.gem"
    GemVal _t1939 = gem_v_node;
    static GemICacheSlot _ic_280 = {0};
            GemVal gem_v__for_items_12 = gem_table_get_cached(_t1939, "defaults", &_ic_280);
#line 291 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 291 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1940[] = {gem_v__for_items_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1940, 1)))) break;
#line 291 "compiler/main.gem"
                GemVal gem_v_d = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 291 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 292 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 293 "compiler/main.gem"
    GemVal _t1941[] = {gem_v_d, gem_v_inner_defined, gem_v_inner_free};
    GemVal _t1942 = (*gem_v_collect_free_node);
                    (void)(_t1942.fn(_t1942.env, _t1941, 3));
                }
            }

        }
        {
#line 297 "compiler/main.gem"
            GemVal gem_v__for_tbl_13 = gem_v_inner_free;
#line 297 "compiler/main.gem"
    GemVal _t1943[] = {gem_v__for_tbl_13};
            GemVal gem_v__for_len_13 = gem_len_fn(NULL, _t1943, 1);
#line 297 "compiler/main.gem"
            GemVal gem_v__for_i_13 = gem_int(0);
#line 297 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_len_13))) break;
#line 297 "compiler/main.gem"
    GemVal _t1944[] = {gem_v__for_tbl_13, gem_v__for_i_13};
                GemVal gem_v_ifk = gem_table_key_at_fn(NULL, _t1944, 2);
#line 297 "compiler/main.gem"
    GemVal _t1945[] = {gem_v__for_tbl_13, gem_v__for_i_13};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1945, 2);
#line 297 "compiler/main.gem"
                gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 298 "compiler/main.gem"
    GemVal _t1946[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1946, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1947[] = {gem_v__match_11};
    GemVal _t1949;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1947, 1), gem_string("table")))) {
        _t1949 = gem_eq(gem_type_fn(NULL, _t1947, 1), gem_string("table"));
    } else {
        GemVal _t1948[] = {gem_v__match_11, gem_string("tag")};
        _t1949 = gem_has_key_fn(NULL, _t1948, 2);
    }
    GemVal _t1950;
    if (!gem_truthy(_t1949)) {
        _t1950 = _t1949;
    } else {
        _t1950 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1950)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1951[] = {gem_v__match_11};
    GemVal _t1953;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1951, 1), gem_string("table")))) {
        _t1953 = gem_eq(gem_type_fn(NULL, _t1951, 1), gem_string("table"));
    } else {
        GemVal _t1952[] = {gem_v__match_11, gem_string("tag")};
        _t1953 = gem_has_key_fn(NULL, _t1952, 2);
    }
    GemVal _t1954;
    if (!gem_truthy(_t1953)) {
        _t1954 = _t1953;
    } else {
        _t1954 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1956;
    if (!gem_truthy(_t1954)) {
        _t1956 = _t1954;
    } else {
        GemVal _t1955[] = {gem_v__match_11, gem_string("cond")};
        _t1956 = gem_has_key_fn(NULL, _t1955, 2);
    }
    GemVal _t1958;
    if (!gem_truthy(_t1956)) {
        _t1958 = _t1956;
    } else {
        GemVal _t1957[] = {gem_v__match_11, gem_string("then")};
        _t1958 = gem_has_key_fn(NULL, _t1957, 2);
    }
    GemVal _t1960;
    if (!gem_truthy(_t1958)) {
        _t1960 = _t1958;
    } else {
        GemVal _t1959[] = {gem_v__match_11, gem_string("else")};
        _t1960 = gem_has_key_fn(NULL, _t1959, 2);
    }
    if (gem_truthy(_t1960)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 303 "compiler/main.gem"
    GemVal _t1961[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1962 = (*gem_v_collect_free_node);
        (void)(_t1962.fn(_t1962.env, _t1961, 3));
#line 304 "compiler/main.gem"
    GemVal _t1963 = gem_table_new();
    GemVal _t1964[] = {gem_v_defined, _t1963};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1964, 2);
#line 305 "compiler/main.gem"
        GemVal gem_v__for_i_14 = gem_int(0);
#line 305 "compiler/main.gem"
    GemVal _t1965[] = {gem_v_thens};
        GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1965, 1);
#line 305 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 305 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_14;
#line 305 "compiler/main.gem"
            gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 306 "compiler/main.gem"
    GemVal _t1966[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1967 = (*gem_v_collect_free_node);
            (void)(_t1967.fn(_t1967.env, _t1966, 3));
#line 307 "compiler/main.gem"
    GemVal _t1968[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1968, 2))) {
#line 308 "compiler/main.gem"
    GemVal _t1969 = gem_table_get(gem_v_thens, gem_v_i);
    static GemICacheSlot _ic_281 = {0};
    GemVal _t1970[] = {gem_v_d, gem_table_get_cached(_t1969, "name", &_ic_281)};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1970, 2));
            }
        }

#line 311 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 312 "compiler/main.gem"
    GemVal _t1971 = gem_table_new();
    GemVal _t1972[] = {gem_v_defined, _t1971};
            gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1972, 2);
            {
#line 313 "compiler/main.gem"
                GemVal gem_v__for_i_15 = gem_int(0);
#line 313 "compiler/main.gem"
    GemVal _t1973[] = {gem_v_el};
                GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1973, 1);
#line 313 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 313 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_15;
#line 313 "compiler/main.gem"
                    gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 314 "compiler/main.gem"
    GemVal _t1974[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1975 = (*gem_v_collect_free_node);
                    (void)(_t1975.fn(_t1975.env, _t1974, 3));
#line 315 "compiler/main.gem"
    GemVal _t1976[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1976, 2))) {
#line 316 "compiler/main.gem"
    GemVal _t1977 = gem_table_get(gem_v_el, gem_v_i);
    static GemICacheSlot _ic_282 = {0};
    GemVal _t1978[] = {gem_v_d, gem_table_get_cached(_t1977, "name", &_ic_282)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1978, 2));
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
    GemVal _t1979[] = {gem_v__match_11};
    GemVal _t1981;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1979, 1), gem_string("table")))) {
        _t1981 = gem_eq(gem_type_fn(NULL, _t1979, 1), gem_string("table"));
    } else {
        GemVal _t1980[] = {gem_v__match_11, gem_string("tag")};
        _t1981 = gem_has_key_fn(NULL, _t1980, 2);
    }
    GemVal _t1982;
    if (!gem_truthy(_t1981)) {
        _t1982 = _t1981;
    } else {
        _t1982 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1984;
    if (!gem_truthy(_t1982)) {
        _t1984 = _t1982;
    } else {
        GemVal _t1983[] = {gem_v__match_11, gem_string("cond")};
        _t1984 = gem_has_key_fn(NULL, _t1983, 2);
    }
    GemVal _t1986;
    if (!gem_truthy(_t1984)) {
        _t1986 = _t1984;
    } else {
        GemVal _t1985[] = {gem_v__match_11, gem_string("body")};
        _t1986 = gem_has_key_fn(NULL, _t1985, 2);
    }
    if (gem_truthy(_t1986)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 321 "compiler/main.gem"
    GemVal _t1987[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1988 = (*gem_v_collect_free_node);
        (void)(_t1988.fn(_t1988.env, _t1987, 3));
#line 322 "compiler/main.gem"
    GemVal _t1989 = gem_table_new();
    GemVal _t1990[] = {gem_v_defined, _t1989};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1990, 2);
        {
#line 323 "compiler/main.gem"
            GemVal gem_v__for_i_16 = gem_int(0);
#line 323 "compiler/main.gem"
    GemVal _t1991[] = {gem_v_body};
            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1991, 1);
#line 323 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 323 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_16;
#line 323 "compiler/main.gem"
                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 324 "compiler/main.gem"
    GemVal _t1992[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1993 = (*gem_v_collect_free_node);
                (void)(_t1993.fn(_t1993.env, _t1992, 3));
#line 325 "compiler/main.gem"
    GemVal _t1994[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1994, 2))) {
#line 326 "compiler/main.gem"
    GemVal _t1995 = gem_table_get(gem_v_body, gem_v_i);
    static GemICacheSlot _ic_283 = {0};
    GemVal _t1996[] = {gem_v_d, gem_table_get_cached(_t1995, "name", &_ic_283)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1996, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1997[] = {gem_v__match_11};
    GemVal _t1999;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1997, 1), gem_string("table")))) {
        _t1999 = gem_eq(gem_type_fn(NULL, _t1997, 1), gem_string("table"));
    } else {
        GemVal _t1998[] = {gem_v__match_11, gem_string("tag")};
        _t1999 = gem_has_key_fn(NULL, _t1998, 2);
    }
    GemVal _t2000;
    if (!gem_truthy(_t1999)) {
        _t2000 = _t1999;
    } else {
        _t2000 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2002;
    if (!gem_truthy(_t2000)) {
        _t2002 = _t2000;
    } else {
        GemVal _t2001[] = {gem_v__match_11, gem_string("target")};
        _t2002 = gem_has_key_fn(NULL, _t2001, 2);
    }
    GemVal _t2004;
    if (!gem_truthy(_t2002)) {
        _t2004 = _t2002;
    } else {
        GemVal _t2003[] = {gem_v__match_11, gem_string("target_var")};
        _t2004 = gem_has_key_fn(NULL, _t2003, 2);
    }
    GemVal _t2006;
    if (!gem_truthy(_t2004)) {
        _t2006 = _t2004;
    } else {
        GemVal _t2005[] = {gem_v__match_11, gem_string("whens")};
        _t2006 = gem_has_key_fn(NULL, _t2005, 2);
    }
    GemVal _t2008;
    if (!gem_truthy(_t2006)) {
        _t2008 = _t2006;
    } else {
        GemVal _t2007[] = {gem_v__match_11, gem_string("else")};
        _t2008 = gem_has_key_fn(NULL, _t2007, 2);
    }
    if (gem_truthy(_t2008)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 330 "compiler/main.gem"
    GemVal _t2009[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t2010 = (*gem_v_collect_free_node);
        (void)(_t2010.fn(_t2010.env, _t2009, 3));
#line 331 "compiler/main.gem"
    GemVal _t2011 = gem_table_new();
    GemVal _t2012[] = {gem_v_defined, _t2011};
        GemVal gem_v_match_def = gem_fn__mod_codegen_set_union(NULL, _t2012, 2);
#line 332 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 333 "compiler/main.gem"
    GemVal _t2013[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2013, 2));
        }
#line 335 "compiler/main.gem"
        GemVal gem_v__for_i_19 = gem_int(0);
#line 335 "compiler/main.gem"
    GemVal _t2014[] = {gem_v_whens};
        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t2014, 1);
#line 335 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 335 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_19;
#line 335 "compiler/main.gem"
            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 336 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 337 "compiler/main.gem"
    GemVal _t2015 = gem_v_w;
    static GemICacheSlot _ic_284 = {0};
    GemVal _t2016[] = {gem_table_get_cached(_t2015, "value", &_ic_284), gem_v_match_def, gem_v_free};
    GemVal _t2017 = (*gem_v_collect_free_node);
            (void)(_t2017.fn(_t2017.env, _t2016, 3));
#line 338 "compiler/main.gem"
    GemVal _t2018 = gem_table_new();
    GemVal _t2019[] = {gem_v_match_def, _t2018};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2019, 2);
#line 339 "compiler/main.gem"
    GemVal _t2020 = gem_v_w;
    static GemICacheSlot _ic_285 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2020, "bindings", &_ic_285), GEM_NIL))) {
#line 340 "compiler/main.gem"
                GemVal gem_v__for_i_17 = gem_int(0);
#line 340 "compiler/main.gem"
    GemVal _t2021 = gem_v_w;
    static GemICacheSlot _ic_286 = {0};
    GemVal _t2022[] = {gem_table_get_cached(_t2021, "bindings", &_ic_286)};
                GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t2022, 1);
#line 340 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 340 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_17;
#line 340 "compiler/main.gem"
                    gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 341 "compiler/main.gem"
    GemVal _t2023 = gem_v_w;
    static GemICacheSlot _ic_287 = {0};
    GemVal _t2024[] = {gem_table_get(gem_table_get_cached(_t2023, "bindings", &_ic_287), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2025 = (*gem_v_collect_free_node);
                    (void)(_t2025.fn(_t2025.env, _t2024, 3));
#line 342 "compiler/main.gem"
    GemVal _t2026 = gem_v_w;
    static GemICacheSlot _ic_288 = {0};
    GemVal _t2027[] = {gem_table_get(gem_table_get_cached(_t2026, "bindings", &_ic_288), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2027, 2))) {
#line 343 "compiler/main.gem"
    GemVal _t2028 = gem_v_w;
    static GemICacheSlot _ic_289 = {0};
    GemVal _t2029 = gem_table_get(gem_table_get_cached(_t2028, "bindings", &_ic_289), gem_v_bi);
    static GemICacheSlot _ic_290 = {0};
    GemVal _t2030[] = {gem_v_d, gem_table_get_cached(_t2029, "name", &_ic_290)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2030, 2));
                    }
                }

            }
#line 347 "compiler/main.gem"
            GemVal gem_v__for_i_18 = gem_int(0);
#line 347 "compiler/main.gem"
    GemVal _t2031 = gem_v_w;
    static GemICacheSlot _ic_291 = {0};
    GemVal _t2032[] = {gem_table_get_cached(_t2031, "body", &_ic_291)};
            GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t2032, 1);
#line 347 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 347 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_18;
#line 347 "compiler/main.gem"
                gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 348 "compiler/main.gem"
    GemVal _t2033 = gem_v_w;
    static GemICacheSlot _ic_292 = {0};
    GemVal _t2034[] = {gem_table_get(gem_table_get_cached(_t2033, "body", &_ic_292), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2035 = (*gem_v_collect_free_node);
                (void)(_t2035.fn(_t2035.env, _t2034, 3));
#line 349 "compiler/main.gem"
    GemVal _t2036 = gem_v_w;
    static GemICacheSlot _ic_293 = {0};
    GemVal _t2037[] = {gem_table_get(gem_table_get_cached(_t2036, "body", &_ic_293), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2037, 2))) {
#line 350 "compiler/main.gem"
    GemVal _t2038 = gem_v_w;
    static GemICacheSlot _ic_294 = {0};
    GemVal _t2039 = gem_table_get(gem_table_get_cached(_t2038, "body", &_ic_294), gem_v_j);
    static GemICacheSlot _ic_295 = {0};
    GemVal _t2040[] = {gem_v_d, gem_table_get_cached(_t2039, "name", &_ic_295)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2040, 2));
                }
            }

        }

#line 354 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 355 "compiler/main.gem"
    GemVal _t2041 = gem_table_new();
    GemVal _t2042[] = {gem_v_defined, _t2041};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2042, 2);
            {
#line 356 "compiler/main.gem"
                GemVal gem_v__for_i_20 = gem_int(0);
#line 356 "compiler/main.gem"
    GemVal _t2043[] = {gem_v_el};
                GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t2043, 1);
#line 356 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 356 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_20;
#line 356 "compiler/main.gem"
                    gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t2044[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2045 = (*gem_v_collect_free_node);
                    (void)(_t2045.fn(_t2045.env, _t2044, 3));
#line 358 "compiler/main.gem"
    GemVal _t2046[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2046, 2))) {
#line 359 "compiler/main.gem"
    GemVal _t2047 = gem_table_get(gem_v_el, gem_v_j);
    static GemICacheSlot _ic_296 = {0};
    GemVal _t2048[] = {gem_v_d, gem_table_get_cached(_t2047, "name", &_ic_296)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2048, 2));
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
    GemVal _t2049[] = {gem_v__match_11};
    GemVal _t2051;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2049, 1), gem_string("table")))) {
        _t2051 = gem_eq(gem_type_fn(NULL, _t2049, 1), gem_string("table"));
    } else {
        GemVal _t2050[] = {gem_v__match_11, gem_string("tag")};
        _t2051 = gem_has_key_fn(NULL, _t2050, 2);
    }
    GemVal _t2052;
    if (!gem_truthy(_t2051)) {
        _t2052 = _t2051;
    } else {
        _t2052 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t2054;
    if (!gem_truthy(_t2052)) {
        _t2054 = _t2052;
    } else {
        GemVal _t2053[] = {gem_v__match_11, gem_string("value")};
        _t2054 = gem_has_key_fn(NULL, _t2053, 2);
    }
    GemVal _t2056;
    if (!gem_truthy(_t2054)) {
        _t2056 = _t2054;
    } else {
        GemVal _t2055[] = {gem_v__match_11, gem_string("bindings")};
        _t2056 = gem_has_key_fn(NULL, _t2055, 2);
    }
    GemVal _t2058;
    if (!gem_truthy(_t2056)) {
        _t2058 = _t2056;
    } else {
        GemVal _t2057[] = {gem_v__match_11, gem_string("body")};
        _t2058 = gem_has_key_fn(NULL, _t2057, 2);
    }
    if (gem_truthy(_t2058)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 364 "compiler/main.gem"
    GemVal _t2059[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2060 = (*gem_v_collect_free_node);
        (void)(_t2060.fn(_t2060.env, _t2059, 3));
#line 365 "compiler/main.gem"
    GemVal _t2061 = gem_table_new();
    GemVal _t2062[] = {gem_v_defined, _t2061};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2062, 2);
#line 366 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 367 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 367 "compiler/main.gem"
    GemVal _t2063[] = {gem_v_bindings};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t2063, 1);
#line 367 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 367 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_21;
#line 367 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 368 "compiler/main.gem"
    GemVal _t2064[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2065 = (*gem_v_collect_free_node);
                (void)(_t2065.fn(_t2065.env, _t2064, 3));
#line 369 "compiler/main.gem"
    GemVal _t2066[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2066, 2))) {
#line 370 "compiler/main.gem"
    GemVal _t2067 = gem_table_get(gem_v_bindings, gem_v_bi);
    static GemICacheSlot _ic_297 = {0};
    GemVal _t2068[] = {gem_v_d, gem_table_get_cached(_t2067, "name", &_ic_297)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2068, 2));
                }
            }

        }
        {
#line 374 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 374 "compiler/main.gem"
    GemVal _t2069[] = {gem_v_body};
            GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t2069, 1);
#line 374 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 374 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_22;
#line 374 "compiler/main.gem"
                gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 375 "compiler/main.gem"
    GemVal _t2070[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2071 = (*gem_v_collect_free_node);
                (void)(_t2071.fn(_t2071.env, _t2070, 3));
#line 376 "compiler/main.gem"
    GemVal _t2072[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2072, 2))) {
#line 377 "compiler/main.gem"
    GemVal _t2073 = gem_table_get(gem_v_body, gem_v_i);
    static GemICacheSlot _ic_298 = {0};
    GemVal _t2074[] = {gem_v_d, gem_table_get_cached(_t2073, "name", &_ic_298)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2074, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2075[] = {gem_v__match_11};
    GemVal _t2077;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2075, 1), gem_string("table")))) {
        _t2077 = gem_eq(gem_type_fn(NULL, _t2075, 1), gem_string("table"));
    } else {
        GemVal _t2076[] = {gem_v__match_11, gem_string("tag")};
        _t2077 = gem_has_key_fn(NULL, _t2076, 2);
    }
    GemVal _t2078;
    if (!gem_truthy(_t2077)) {
        _t2078 = _t2077;
    } else {
        _t2078 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2080;
    if (!gem_truthy(_t2078)) {
        _t2080 = _t2078;
    } else {
        GemVal _t2079[] = {gem_v__match_11, gem_string("arms")};
        _t2080 = gem_has_key_fn(NULL, _t2079, 2);
    }
    GemVal _t2082;
    if (!gem_truthy(_t2080)) {
        _t2082 = _t2080;
    } else {
        GemVal _t2081[] = {gem_v__match_11, gem_string("after_ms")};
        _t2082 = gem_has_key_fn(NULL, _t2081, 2);
    }
    GemVal _t2084;
    if (!gem_truthy(_t2082)) {
        _t2084 = _t2082;
    } else {
        GemVal _t2083[] = {gem_v__match_11, gem_string("after_body")};
        _t2084 = gem_has_key_fn(NULL, _t2083, 2);
    }
    if (gem_truthy(_t2084)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 381 "compiler/main.gem"
    GemVal _t2085 = gem_table_new();
    GemVal _t2086[] = {gem_v_defined, _t2085};
        GemVal gem_v_recv_def = gem_fn__mod_codegen_set_union(NULL, _t2086, 2);
#line 382 "compiler/main.gem"
    GemVal _t2087 = gem_v_node;
    static GemICacheSlot _ic_299 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t2087, "recv_var", &_ic_299), GEM_NIL))) {
#line 383 "compiler/main.gem"
    GemVal _t2088 = gem_v_node;
    static GemICacheSlot _ic_300 = {0};
    GemVal _t2089[] = {gem_v_recv_def, gem_table_get_cached(_t2088, "recv_var", &_ic_300)};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2089, 2));
        }
#line 385 "compiler/main.gem"
        GemVal gem_v__for_i_25 = gem_int(0);
#line 385 "compiler/main.gem"
    GemVal _t2090[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t2090, 1);
#line 385 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 385 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_25;
#line 385 "compiler/main.gem"
            gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 386 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 387 "compiler/main.gem"
    GemVal _t2091 = gem_v_arm;
    static GemICacheSlot _ic_301 = {0};
    GemVal _t2092 = gem_table_get_cached(_t2091, "pattern", &_ic_301);
    static GemICacheSlot _ic_302 = {0};
    GemVal _t2093[] = {gem_table_get_cached(_t2092, "condition", &_ic_302), gem_v_recv_def, gem_v_free};
    GemVal _t2094 = (*gem_v_collect_free_node);
            (void)(_t2094.fn(_t2094.env, _t2093, 3));
#line 388 "compiler/main.gem"
    GemVal _t2095 = gem_table_new();
    GemVal _t2096[] = {gem_v_recv_def, _t2095};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2096, 2);
#line 389 "compiler/main.gem"
    GemVal _t2097 = gem_v_arm;
    static GemICacheSlot _ic_303 = {0};
    GemVal _t2098 = gem_table_get_cached(_t2097, "pattern", &_ic_303);
    static GemICacheSlot _ic_304 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2098, "bindings", &_ic_304), GEM_NIL))) {
#line 390 "compiler/main.gem"
                GemVal gem_v__for_i_23 = gem_int(0);
#line 390 "compiler/main.gem"
    GemVal _t2099 = gem_v_arm;
    static GemICacheSlot _ic_305 = {0};
    GemVal _t2100 = gem_table_get_cached(_t2099, "pattern", &_ic_305);
    static GemICacheSlot _ic_306 = {0};
    GemVal _t2101[] = {gem_table_get_cached(_t2100, "bindings", &_ic_306)};
                GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t2101, 1);
#line 390 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 390 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_23;
#line 390 "compiler/main.gem"
                    gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 391 "compiler/main.gem"
    GemVal _t2102 = gem_v_arm;
    static GemICacheSlot _ic_307 = {0};
    GemVal _t2103 = gem_table_get_cached(_t2102, "pattern", &_ic_307);
    static GemICacheSlot _ic_308 = {0};
    GemVal _t2104[] = {gem_table_get(gem_table_get_cached(_t2103, "bindings", &_ic_308), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2105 = (*gem_v_collect_free_node);
                    (void)(_t2105.fn(_t2105.env, _t2104, 3));
#line 392 "compiler/main.gem"
    GemVal _t2106 = gem_v_arm;
    static GemICacheSlot _ic_309 = {0};
    GemVal _t2107 = gem_table_get_cached(_t2106, "pattern", &_ic_309);
    static GemICacheSlot _ic_310 = {0};
    GemVal _t2108[] = {gem_table_get(gem_table_get_cached(_t2107, "bindings", &_ic_310), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2108, 2))) {
#line 393 "compiler/main.gem"
    GemVal _t2109 = gem_v_arm;
    static GemICacheSlot _ic_311 = {0};
    GemVal _t2110 = gem_table_get_cached(_t2109, "pattern", &_ic_311);
    static GemICacheSlot _ic_312 = {0};
    GemVal _t2111 = gem_table_get(gem_table_get_cached(_t2110, "bindings", &_ic_312), gem_v_bi);
    static GemICacheSlot _ic_313 = {0};
    GemVal _t2112[] = {gem_v_d, gem_table_get_cached(_t2111, "name", &_ic_313)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2112, 2));
                    }
                }

            }
#line 397 "compiler/main.gem"
            GemVal gem_v__for_i_24 = gem_int(0);
#line 397 "compiler/main.gem"
    GemVal _t2113 = gem_v_arm;
    static GemICacheSlot _ic_314 = {0};
    GemVal _t2114[] = {gem_table_get_cached(_t2113, "body", &_ic_314)};
            GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t2114, 1);
#line 397 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 397 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_24;
#line 397 "compiler/main.gem"
                gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 398 "compiler/main.gem"
    GemVal _t2115 = gem_v_arm;
    static GemICacheSlot _ic_315 = {0};
    GemVal _t2116[] = {gem_table_get(gem_table_get_cached(_t2115, "body", &_ic_315), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2117 = (*gem_v_collect_free_node);
                (void)(_t2117.fn(_t2117.env, _t2116, 3));
#line 399 "compiler/main.gem"
    GemVal _t2118 = gem_v_arm;
    static GemICacheSlot _ic_316 = {0};
    GemVal _t2119[] = {gem_table_get(gem_table_get_cached(_t2118, "body", &_ic_316), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2119, 2))) {
#line 400 "compiler/main.gem"
    GemVal _t2120 = gem_v_arm;
    static GemICacheSlot _ic_317 = {0};
    GemVal _t2121 = gem_table_get(gem_table_get_cached(_t2120, "body", &_ic_317), gem_v_j);
    static GemICacheSlot _ic_318 = {0};
    GemVal _t2122[] = {gem_v_d, gem_table_get_cached(_t2121, "name", &_ic_318)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2122, 2));
                }
            }

        }

#line 404 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 405 "compiler/main.gem"
    GemVal _t2123[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t2124 = (*gem_v_collect_free_node);
            (void)(_t2124.fn(_t2124.env, _t2123, 3));
        }
#line 407 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 408 "compiler/main.gem"
    GemVal _t2125 = gem_table_new();
    GemVal _t2126[] = {gem_v_recv_def, _t2125};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2126, 2);
            {
#line 409 "compiler/main.gem"
                GemVal gem_v__for_i_26 = gem_int(0);
#line 409 "compiler/main.gem"
    GemVal _t2127[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t2127, 1);
#line 409 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 409 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_26;
#line 409 "compiler/main.gem"
                    gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 410 "compiler/main.gem"
    GemVal _t2128[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2129 = (*gem_v_collect_free_node);
                    (void)(_t2129.fn(_t2129.env, _t2128, 3));
#line 411 "compiler/main.gem"
    GemVal _t2130[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2130, 2))) {
#line 412 "compiler/main.gem"
    GemVal _t2131 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    static GemICacheSlot _ic_319 = {0};
    GemVal _t2132[] = {gem_v_d, gem_table_get_cached(_t2131, "name", &_ic_319)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2132, 2));
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
    GemVal _t2133[] = {gem_v__match_11};
    GemVal _t2135;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2133, 1), gem_string("table")))) {
        _t2135 = gem_eq(gem_type_fn(NULL, _t2133, 1), gem_string("table"));
    } else {
        GemVal _t2134[] = {gem_v__match_11, gem_string("tag")};
        _t2135 = gem_has_key_fn(NULL, _t2134, 2);
    }
    GemVal _t2136;
    if (!gem_truthy(_t2135)) {
        _t2136 = _t2135;
    } else {
        _t2136 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2138;
    if (!gem_truthy(_t2136)) {
        _t2138 = _t2136;
    } else {
        GemVal _t2137[] = {gem_v__match_11, gem_string("entries")};
        _t2138 = gem_has_key_fn(NULL, _t2137, 2);
    }
    if (gem_truthy(_t2138)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 417 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 417 "compiler/main.gem"
    GemVal _t2139[] = {gem_v_entries};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t2139, 1);
#line 417 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 417 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 417 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 418 "compiler/main.gem"
    GemVal _t2140 = gem_table_get(gem_v_entries, gem_v_i);
    static GemICacheSlot _ic_320 = {0};
    GemVal _t2141[] = {gem_table_get_cached(_t2140, "value", &_ic_320), gem_v_defined, gem_v_free};
    GemVal _t2142 = (*gem_v_collect_free_node);
                (void)(_t2142.fn(_t2142.env, _t2141, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2143[] = {gem_v__match_11};
    GemVal _t2145;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2143, 1), gem_string("table")))) {
        _t2145 = gem_eq(gem_type_fn(NULL, _t2143, 1), gem_string("table"));
    } else {
        GemVal _t2144[] = {gem_v__match_11, gem_string("tag")};
        _t2145 = gem_has_key_fn(NULL, _t2144, 2);
    }
    GemVal _t2146;
    if (!gem_truthy(_t2145)) {
        _t2146 = _t2145;
    } else {
        _t2146 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2148;
    if (!gem_truthy(_t2146)) {
        _t2148 = _t2146;
    } else {
        GemVal _t2147[] = {gem_v__match_11, gem_string("elements")};
        _t2148 = gem_has_key_fn(NULL, _t2147, 2);
    }
    if (gem_truthy(_t2148)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 421 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 421 "compiler/main.gem"
    GemVal _t2149[] = {gem_v_elements};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t2149, 1);
#line 421 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 421 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 421 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 422 "compiler/main.gem"
    GemVal _t2150[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t2151 = (*gem_v_collect_free_node);
                (void)(_t2151.fn(_t2151.env, _t2150, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2152[] = {gem_v__match_11};
    GemVal _t2154;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2152, 1), gem_string("table")))) {
        _t2154 = gem_eq(gem_type_fn(NULL, _t2152, 1), gem_string("table"));
    } else {
        GemVal _t2153[] = {gem_v__match_11, gem_string("tag")};
        _t2154 = gem_has_key_fn(NULL, _t2153, 2);
    }
    GemVal _t2155;
    if (!gem_truthy(_t2154)) {
        _t2155 = _t2154;
    } else {
        _t2155 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2157;
    if (!gem_truthy(_t2155)) {
        _t2157 = _t2155;
    } else {
        GemVal _t2156[] = {gem_v__match_11, gem_string("stmts")};
        _t2157 = gem_has_key_fn(NULL, _t2156, 2);
    }
    if (gem_truthy(_t2157)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 425 "compiler/main.gem"
    GemVal _t2158 = gem_table_new();
    GemVal _t2159[] = {gem_v_defined, _t2158};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2159, 2);
        {
#line 426 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 426 "compiler/main.gem"
    GemVal _t2160[] = {gem_v_stmts};
            GemVal gem_v__for_limit_29 = gem_len_fn(NULL, _t2160, 1);
#line 426 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 426 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_29;
#line 426 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 427 "compiler/main.gem"
    GemVal _t2161[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2162 = (*gem_v_collect_free_node);
                (void)(_t2162.fn(_t2162.env, _t2161, 3));
#line 428 "compiler/main.gem"
    GemVal _t2163[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2163, 2))) {
#line 429 "compiler/main.gem"
    GemVal _t2164 = gem_table_get(gem_v_stmts, gem_v_i);
    static GemICacheSlot _ic_321 = {0};
    GemVal _t2165[] = {gem_v_d, gem_table_get_cached(_t2164, "name", &_ic_321)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2165, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2166[] = {gem_v__match_11};
    GemVal _t2168;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2166, 1), gem_string("table")))) {
        _t2168 = gem_eq(gem_type_fn(NULL, _t2166, 1), gem_string("table"));
    } else {
        GemVal _t2167[] = {gem_v__match_11, gem_string("tag")};
        _t2168 = gem_has_key_fn(NULL, _t2167, 2);
    }
    GemVal _t2169;
    if (!gem_truthy(_t2168)) {
        _t2169 = _t2168;
    } else {
        _t2169 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2171;
    if (!gem_truthy(_t2169)) {
        _t2171 = _t2169;
    } else {
        GemVal _t2170[] = {gem_v__match_11, gem_string("value")};
        _t2171 = gem_has_key_fn(NULL, _t2170, 2);
    }
    if (gem_truthy(_t2171)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 433 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 434 "compiler/main.gem"
    GemVal _t2172[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2173 = (*gem_v_collect_free_node);
            GemVal _t2174 = _t2173.fn(_t2173.env, _t2172, 3);
            gem_pop_frame();
            return _t2174;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 438 "compiler/main.gem"
            GemVal gem_v__for_tbl_30 = gem_v_node;
#line 438 "compiler/main.gem"
    GemVal _t2175[] = {gem_v__for_tbl_30};
            GemVal gem_v__for_len_30 = gem_len_fn(NULL, _t2175, 1);
#line 438 "compiler/main.gem"
            GemVal gem_v__for_i_30 = gem_int(0);
#line 438 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_len_30))) break;
#line 438 "compiler/main.gem"
    GemVal _t2176[] = {gem_v__for_tbl_30, gem_v__for_i_30};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2176, 2);
#line 438 "compiler/main.gem"
    GemVal _t2177[] = {gem_v__for_tbl_30, gem_v__for_i_30};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2177, 2);
#line 438 "compiler/main.gem"
                gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 439 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 440 "compiler/main.gem"
    GemVal _t2178[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2178, 1), gem_string("table")))) {
#line 441 "compiler/main.gem"
    GemVal _t2179[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2179, 1);
#line 442 "compiler/main.gem"
    GemVal _t2180[] = {gem_v_vks};
    GemVal _t2182;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2180, 1), gem_int(0)))) {
        _t2182 = gem_gt(gem_len_fn(NULL, _t2180, 1), gem_int(0));
    } else {
        GemVal _t2181[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2182 = gem_eq(gem_type_fn(NULL, _t2181, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2182)) {
#line 443 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 444 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2183[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2183, 1)))) break;
#line 445 "compiler/main.gem"
    GemVal _t2184[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2184, 1), gem_string("table")))) {
#line 446 "compiler/main.gem"
    GemVal _t2185[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t2186 = (*gem_v_collect_free_node);
                                    (void)(_t2186.fn(_t2186.env, _t2185, 3));
                                }
#line 448 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 451 "compiler/main.gem"
    GemVal _t2187[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t2188 = (*gem_v_collect_free_node);
                            (void)(_t2188.fn(_t2188.env, _t2187, 3));
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
#line 460 "compiler/main.gem"
    GemVal _t2190 = gem_table_new();
    GemVal gem_v_free = _t2190;
#line 461 "compiler/main.gem"
    GemVal _t2191 = gem_table_new();
    GemVal _t2192[] = {gem_v_defined, _t2191};
    GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2192, 2);
#line 462 "compiler/main.gem"
    GemVal gem_v__for_i_31 = gem_int(0);
#line 462 "compiler/main.gem"
    GemVal _t2193[] = {gem_v_stmts};
    GemVal gem_v__for_limit_31 = gem_len_fn(NULL, _t2193, 1);
#line 462 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 462 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_31;
#line 462 "compiler/main.gem"
        gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 463 "compiler/main.gem"
    GemVal _t2194[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2195 = (*gem_v_collect_free_node);
        (void)(_t2195.fn(_t2195.env, _t2194, 3));
#line 464 "compiler/main.gem"
    GemVal _t2196[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2196, 2))) {
#line 465 "compiler/main.gem"
    GemVal _t2197 = gem_table_get(gem_v_stmts, gem_v_i);
    static GemICacheSlot _ic_322 = {0};
    GemVal _t2198[] = {gem_v_d, gem_table_get_cached(_t2197, "name", &_ic_322)};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2198, 2));
        }
#line 467 "compiler/main.gem"
    GemVal _t2199[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2199, 2));
    }

    GemVal _t2200 = gem_v_free;
    gem_pop_frame();
    return _t2200;
}

struct _closure__anon_29 {
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_29(void *_env, GemVal *args, int argc) {
    struct _closure__anon_29 *_cls = (struct _closure__anon_29 *)_env;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_29", "compiler/main.gem", 0);
#line 478 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 479 "compiler/main.gem"
        GemVal _t2202 = GEM_NIL;
        gem_pop_frame();
        return _t2202;
    }
#line 481 "compiler/main.gem"
    GemVal _t2203[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2203, 1), gem_string("table")))) {
#line 482 "compiler/main.gem"
        GemVal _t2204 = GEM_NIL;
        gem_pop_frame();
        return _t2204;
    }
#line 484 "compiler/main.gem"
    GemVal _t2205 = gem_v_node;
    static GemICacheSlot _ic_323 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2205, "tag", &_ic_323), GEM_NIL))) {
#line 485 "compiler/main.gem"
        GemVal _t2206 = GEM_NIL;
        gem_pop_frame();
        return _t2206;
    }
#line 488 "compiler/main.gem"
    GemVal gem_v__match_32 = gem_v_node;
    GemVal _t2207[] = {gem_v__match_32};
    GemVal _t2209;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2207, 1), gem_string("table")))) {
        _t2209 = gem_eq(gem_type_fn(NULL, _t2207, 1), gem_string("table"));
    } else {
        GemVal _t2208[] = {gem_v__match_32, gem_string("tag")};
        _t2209 = gem_has_key_fn(NULL, _t2208, 2);
    }
    GemVal _t2210;
    if (!gem_truthy(_t2209)) {
        _t2210 = _t2209;
    } else {
        _t2210 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t2212;
    if (!gem_truthy(_t2210)) {
        _t2212 = _t2210;
    } else {
        GemVal _t2211[] = {gem_v__match_32, gem_string("params")};
        _t2212 = gem_has_key_fn(NULL, _t2211, 2);
    }
    GemVal _t2214;
    if (!gem_truthy(_t2212)) {
        _t2214 = _t2212;
    } else {
        GemVal _t2213[] = {gem_v__match_32, gem_string("rest_param")};
        _t2214 = gem_has_key_fn(NULL, _t2213, 2);
    }
    GemVal _t2216;
    if (!gem_truthy(_t2214)) {
        _t2216 = _t2214;
    } else {
        GemVal _t2215[] = {gem_v__match_32, gem_string("body")};
        _t2216 = gem_has_key_fn(NULL, _t2215, 2);
    }
    if (gem_truthy(_t2216)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_32, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_32, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_32, gem_string("body"));
#line 490 "compiler/main.gem"
    GemVal _t2217[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2217, 1);
#line 491 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 492 "compiler/main.gem"
    GemVal _t2218[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2218, 2));
        }
#line 494 "compiler/main.gem"
    GemVal _t2219[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t2220 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t2220.fn(_t2220.env, _t2219, 2);
#line 495 "compiler/main.gem"
    GemVal _t2221 = gem_v_node;
    static GemICacheSlot _ic_324 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t2221, "defaults", &_ic_324), GEM_NIL))) {
#line 496 "compiler/main.gem"
    GemVal _t2222 = gem_v_node;
    static GemICacheSlot _ic_325 = {0};
            GemVal gem_v__for_items_33 = gem_table_get_cached(_t2222, "defaults", &_ic_325);
#line 496 "compiler/main.gem"
            GemVal gem_v__for_i_33 = gem_int(0);
#line 496 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2223[] = {gem_v__for_items_33};
                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_len_fn(NULL, _t2223, 1)))) break;
#line 496 "compiler/main.gem"
                GemVal gem_v_d = gem_table_get(gem_v__for_items_33, gem_v__for_i_33);
#line 496 "compiler/main.gem"
                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 497 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 498 "compiler/main.gem"
    GemVal _t2224[] = {gem_v_d, gem_v_inner_defined, gem_v_free};
    GemVal _t2225 = (*gem_v_collect_free_node);
                    (void)(_t2225.fn(_t2225.env, _t2224, 3));
                }
            }

        }
#line 502 "compiler/main.gem"
    GemVal _t2226[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn__mod_codegen_set_intersect(NULL, _t2226, 2);
        {
#line 503 "compiler/main.gem"
            GemVal gem_v__for_tbl_34 = gem_v_inter;
#line 503 "compiler/main.gem"
    GemVal _t2227[] = {gem_v__for_tbl_34};
            GemVal gem_v__for_len_34 = gem_len_fn(NULL, _t2227, 1);
#line 503 "compiler/main.gem"
            GemVal gem_v__for_i_34 = gem_int(0);
#line 503 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_len_34))) break;
#line 503 "compiler/main.gem"
    GemVal _t2228[] = {gem_v__for_tbl_34, gem_v__for_i_34};
                GemVal gem_v_ik = gem_table_key_at_fn(NULL, _t2228, 2);
#line 503 "compiler/main.gem"
    GemVal _t2229[] = {gem_v__for_tbl_34, gem_v__for_i_34};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t2229, 2);
#line 503 "compiler/main.gem"
                gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 504 "compiler/main.gem"
    GemVal _t2230[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2230, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2231[] = {gem_v__match_32};
    GemVal _t2233;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2231, 1), gem_string("table")))) {
        _t2233 = gem_eq(gem_type_fn(NULL, _t2231, 1), gem_string("table"));
    } else {
        GemVal _t2232[] = {gem_v__match_32, gem_string("tag")};
        _t2233 = gem_has_key_fn(NULL, _t2232, 2);
    }
    GemVal _t2234;
    if (!gem_truthy(_t2233)) {
        _t2234 = _t2233;
    } else {
        _t2234 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2234)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t2235[] = {gem_v__match_32};
    GemVal _t2237;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2235, 1), gem_string("table")))) {
        _t2237 = gem_eq(gem_type_fn(NULL, _t2235, 1), gem_string("table"));
    } else {
        GemVal _t2236[] = {gem_v__match_32, gem_string("tag")};
        _t2237 = gem_has_key_fn(NULL, _t2236, 2);
    }
    GemVal _t2238;
    if (!gem_truthy(_t2237)) {
        _t2238 = _t2237;
    } else {
        _t2238 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2240;
    if (!gem_truthy(_t2238)) {
        _t2240 = _t2238;
    } else {
        GemVal _t2239[] = {gem_v__match_32, gem_string("cond")};
        _t2240 = gem_has_key_fn(NULL, _t2239, 2);
    }
    GemVal _t2242;
    if (!gem_truthy(_t2240)) {
        _t2242 = _t2240;
    } else {
        GemVal _t2241[] = {gem_v__match_32, gem_string("then")};
        _t2242 = gem_has_key_fn(NULL, _t2241, 2);
    }
    GemVal _t2244;
    if (!gem_truthy(_t2242)) {
        _t2244 = _t2242;
    } else {
        GemVal _t2243[] = {gem_v__match_32, gem_string("else")};
        _t2244 = gem_has_key_fn(NULL, _t2243, 2);
    }
    if (gem_truthy(_t2244)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_32, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_32, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_32, gem_string("else"));
#line 509 "compiler/main.gem"
    GemVal _t2245[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2246 = (*gem_v_walk_captures_node);
        (void)(_t2246.fn(_t2246.env, _t2245, 3));
#line 510 "compiler/main.gem"
    GemVal _t2247[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t2248 = (*gem_v_walk_captures);
        (void)(_t2248.fn(_t2248.env, _t2247, 3));
#line 511 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 512 "compiler/main.gem"
    GemVal _t2249[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2250 = (*gem_v_walk_captures);
            GemVal _t2251 = _t2250.fn(_t2250.env, _t2249, 3);
            gem_pop_frame();
            return _t2251;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2252[] = {gem_v__match_32};
    GemVal _t2254;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2252, 1), gem_string("table")))) {
        _t2254 = gem_eq(gem_type_fn(NULL, _t2252, 1), gem_string("table"));
    } else {
        GemVal _t2253[] = {gem_v__match_32, gem_string("tag")};
        _t2254 = gem_has_key_fn(NULL, _t2253, 2);
    }
    GemVal _t2255;
    if (!gem_truthy(_t2254)) {
        _t2255 = _t2254;
    } else {
        _t2255 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("while"));
    }
    GemVal _t2257;
    if (!gem_truthy(_t2255)) {
        _t2257 = _t2255;
    } else {
        GemVal _t2256[] = {gem_v__match_32, gem_string("cond")};
        _t2257 = gem_has_key_fn(NULL, _t2256, 2);
    }
    GemVal _t2259;
    if (!gem_truthy(_t2257)) {
        _t2259 = _t2257;
    } else {
        GemVal _t2258[] = {gem_v__match_32, gem_string("body")};
        _t2259 = gem_has_key_fn(NULL, _t2258, 2);
    }
    if (gem_truthy(_t2259)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_32, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_32, gem_string("body"));
#line 515 "compiler/main.gem"
    GemVal _t2260[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2261 = (*gem_v_walk_captures_node);
        (void)(_t2261.fn(_t2261.env, _t2260, 3));
#line 516 "compiler/main.gem"
    GemVal _t2262[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2263 = (*gem_v_walk_captures);
        GemVal _t2264 = _t2263.fn(_t2263.env, _t2262, 3);
        gem_pop_frame();
        return _t2264;
    } else {
    GemVal _t2265[] = {gem_v__match_32};
    GemVal _t2267;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2265, 1), gem_string("table")))) {
        _t2267 = gem_eq(gem_type_fn(NULL, _t2265, 1), gem_string("table"));
    } else {
        GemVal _t2266[] = {gem_v__match_32, gem_string("tag")};
        _t2267 = gem_has_key_fn(NULL, _t2266, 2);
    }
    GemVal _t2268;
    if (!gem_truthy(_t2267)) {
        _t2268 = _t2267;
    } else {
        _t2268 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2270;
    if (!gem_truthy(_t2268)) {
        _t2270 = _t2268;
    } else {
        GemVal _t2269[] = {gem_v__match_32, gem_string("target")};
        _t2270 = gem_has_key_fn(NULL, _t2269, 2);
    }
    GemVal _t2272;
    if (!gem_truthy(_t2270)) {
        _t2272 = _t2270;
    } else {
        GemVal _t2271[] = {gem_v__match_32, gem_string("whens")};
        _t2272 = gem_has_key_fn(NULL, _t2271, 2);
    }
    GemVal _t2274;
    if (!gem_truthy(_t2272)) {
        _t2274 = _t2272;
    } else {
        GemVal _t2273[] = {gem_v__match_32, gem_string("else")};
        _t2274 = gem_has_key_fn(NULL, _t2273, 2);
    }
    if (gem_truthy(_t2274)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_32, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_32, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_32, gem_string("else"));
#line 518 "compiler/main.gem"
    GemVal _t2275[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2276 = (*gem_v_walk_captures_node);
        (void)(_t2276.fn(_t2276.env, _t2275, 3));
#line 519 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 519 "compiler/main.gem"
    GemVal _t2277[] = {gem_v_whens};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2277, 1);
#line 519 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 519 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 519 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 520 "compiler/main.gem"
    GemVal _t2278 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_326 = {0};
    GemVal _t2279[] = {gem_table_get_cached(_t2278, "value", &_ic_326), gem_v_scope_vars, gem_v_captured};
    GemVal _t2280 = (*gem_v_walk_captures_node);
            (void)(_t2280.fn(_t2280.env, _t2279, 3));
#line 521 "compiler/main.gem"
    GemVal _t2281 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_327 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2281, "bindings", &_ic_327), GEM_NIL))) {
#line 522 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 522 "compiler/main.gem"
    GemVal _t2282 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_328 = {0};
    GemVal _t2283[] = {gem_table_get_cached(_t2282, "bindings", &_ic_328)};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2283, 1);
#line 522 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 522 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 522 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 523 "compiler/main.gem"
    GemVal _t2284 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_329 = {0};
    GemVal _t2285[] = {gem_table_get(gem_table_get_cached(_t2284, "bindings", &_ic_329), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2286 = (*gem_v_walk_captures_node);
                    (void)(_t2286.fn(_t2286.env, _t2285, 3));
                }

            }
#line 526 "compiler/main.gem"
    GemVal _t2287 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_330 = {0};
    GemVal _t2288[] = {gem_table_get_cached(_t2287, "body", &_ic_330), gem_v_scope_vars, gem_v_captured};
    GemVal _t2289 = (*gem_v_walk_captures);
            (void)(_t2289.fn(_t2289.env, _t2288, 3));
        }

#line 528 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 529 "compiler/main.gem"
    GemVal _t2290[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2291 = (*gem_v_walk_captures);
            GemVal _t2292 = _t2291.fn(_t2291.env, _t2290, 3);
            gem_pop_frame();
            return _t2292;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2293[] = {gem_v__match_32};
    GemVal _t2295;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2293, 1), gem_string("table")))) {
        _t2295 = gem_eq(gem_type_fn(NULL, _t2293, 1), gem_string("table"));
    } else {
        GemVal _t2294[] = {gem_v__match_32, gem_string("tag")};
        _t2295 = gem_has_key_fn(NULL, _t2294, 2);
    }
    GemVal _t2296;
    if (!gem_truthy(_t2295)) {
        _t2296 = _t2295;
    } else {
        _t2296 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2298;
    if (!gem_truthy(_t2296)) {
        _t2298 = _t2296;
    } else {
        GemVal _t2297[] = {gem_v__match_32, gem_string("arms")};
        _t2298 = gem_has_key_fn(NULL, _t2297, 2);
    }
    GemVal _t2300;
    if (!gem_truthy(_t2298)) {
        _t2300 = _t2298;
    } else {
        GemVal _t2299[] = {gem_v__match_32, gem_string("after_ms")};
        _t2300 = gem_has_key_fn(NULL, _t2299, 2);
    }
    GemVal _t2302;
    if (!gem_truthy(_t2300)) {
        _t2302 = _t2300;
    } else {
        GemVal _t2301[] = {gem_v__match_32, gem_string("after_body")};
        _t2302 = gem_has_key_fn(NULL, _t2301, 2);
    }
    if (gem_truthy(_t2302)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_32, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_32, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_32, gem_string("after_body"));
#line 532 "compiler/main.gem"
        GemVal gem_v__for_i_38 = gem_int(0);
#line 532 "compiler/main.gem"
    GemVal _t2303[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2303, 1);
#line 532 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 532 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_38;
#line 532 "compiler/main.gem"
            gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 533 "compiler/main.gem"
    GemVal _t2304 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_331 = {0};
    GemVal _t2305 = gem_table_get_cached(_t2304, "pattern", &_ic_331);
    static GemICacheSlot _ic_332 = {0};
    GemVal _t2306[] = {gem_table_get_cached(_t2305, "condition", &_ic_332), gem_v_scope_vars, gem_v_captured};
    GemVal _t2307 = (*gem_v_walk_captures_node);
            (void)(_t2307.fn(_t2307.env, _t2306, 3));
#line 534 "compiler/main.gem"
    GemVal _t2308 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_333 = {0};
    GemVal _t2309 = gem_table_get_cached(_t2308, "pattern", &_ic_333);
    static GemICacheSlot _ic_334 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2309, "bindings", &_ic_334), GEM_NIL))) {
#line 535 "compiler/main.gem"
                GemVal gem_v__for_i_37 = gem_int(0);
#line 535 "compiler/main.gem"
    GemVal _t2310 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_335 = {0};
    GemVal _t2311 = gem_table_get_cached(_t2310, "pattern", &_ic_335);
    static GemICacheSlot _ic_336 = {0};
    GemVal _t2312[] = {gem_table_get_cached(_t2311, "bindings", &_ic_336)};
                GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2312, 1);
#line 535 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 535 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_37;
#line 535 "compiler/main.gem"
                    gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 536 "compiler/main.gem"
    GemVal _t2313 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_337 = {0};
    GemVal _t2314 = gem_table_get_cached(_t2313, "pattern", &_ic_337);
    static GemICacheSlot _ic_338 = {0};
    GemVal _t2315[] = {gem_table_get(gem_table_get_cached(_t2314, "bindings", &_ic_338), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2316 = (*gem_v_walk_captures_node);
                    (void)(_t2316.fn(_t2316.env, _t2315, 3));
                }

            }
#line 539 "compiler/main.gem"
    GemVal _t2317 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_339 = {0};
    GemVal _t2318[] = {gem_table_get_cached(_t2317, "body", &_ic_339), gem_v_scope_vars, gem_v_captured};
    GemVal _t2319 = (*gem_v_walk_captures);
            (void)(_t2319.fn(_t2319.env, _t2318, 3));
        }

#line 541 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 542 "compiler/main.gem"
    GemVal _t2320[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2321 = (*gem_v_walk_captures_node);
            (void)(_t2321.fn(_t2321.env, _t2320, 3));
        }
#line 544 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 545 "compiler/main.gem"
    GemVal _t2322[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2323 = (*gem_v_walk_captures);
            GemVal _t2324 = _t2323.fn(_t2323.env, _t2322, 3);
            gem_pop_frame();
            return _t2324;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2325[] = {gem_v__match_32};
    GemVal _t2327;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2325, 1), gem_string("table")))) {
        _t2327 = gem_eq(gem_type_fn(NULL, _t2325, 1), gem_string("table"));
    } else {
        GemVal _t2326[] = {gem_v__match_32, gem_string("tag")};
        _t2327 = gem_has_key_fn(NULL, _t2326, 2);
    }
    GemVal _t2328;
    if (!gem_truthy(_t2327)) {
        _t2328 = _t2327;
    } else {
        _t2328 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2330;
    if (!gem_truthy(_t2328)) {
        _t2330 = _t2328;
    } else {
        GemVal _t2329[] = {gem_v__match_32, gem_string("entries")};
        _t2330 = gem_has_key_fn(NULL, _t2329, 2);
    }
    if (gem_truthy(_t2330)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_32, gem_string("entries"));
        {
#line 548 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 548 "compiler/main.gem"
    GemVal _t2331[] = {gem_v_entries};
            GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2331, 1);
#line 548 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 548 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_39;
#line 548 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 549 "compiler/main.gem"
    GemVal _t2332 = gem_table_get(gem_v_entries, gem_v_i);
    static GemICacheSlot _ic_340 = {0};
    GemVal _t2333[] = {gem_table_get_cached(_t2332, "value", &_ic_340), gem_v_scope_vars, gem_v_captured};
    GemVal _t2334 = (*gem_v_walk_captures_node);
                (void)(_t2334.fn(_t2334.env, _t2333, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2335[] = {gem_v__match_32};
    GemVal _t2337;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2335, 1), gem_string("table")))) {
        _t2337 = gem_eq(gem_type_fn(NULL, _t2335, 1), gem_string("table"));
    } else {
        GemVal _t2336[] = {gem_v__match_32, gem_string("tag")};
        _t2337 = gem_has_key_fn(NULL, _t2336, 2);
    }
    GemVal _t2338;
    if (!gem_truthy(_t2337)) {
        _t2338 = _t2337;
    } else {
        _t2338 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2340;
    if (!gem_truthy(_t2338)) {
        _t2340 = _t2338;
    } else {
        GemVal _t2339[] = {gem_v__match_32, gem_string("elements")};
        _t2340 = gem_has_key_fn(NULL, _t2339, 2);
    }
    if (gem_truthy(_t2340)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_32, gem_string("elements"));
        {
#line 552 "compiler/main.gem"
            GemVal gem_v__for_i_40 = gem_int(0);
#line 552 "compiler/main.gem"
    GemVal _t2341[] = {gem_v_elements};
            GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2341, 1);
#line 552 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 552 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_40;
#line 552 "compiler/main.gem"
                gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 553 "compiler/main.gem"
    GemVal _t2342[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2343 = (*gem_v_walk_captures_node);
                (void)(_t2343.fn(_t2343.env, _t2342, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2344[] = {gem_v__match_32};
    GemVal _t2346;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2344, 1), gem_string("table")))) {
        _t2346 = gem_eq(gem_type_fn(NULL, _t2344, 1), gem_string("table"));
    } else {
        GemVal _t2345[] = {gem_v__match_32, gem_string("tag")};
        _t2346 = gem_has_key_fn(NULL, _t2345, 2);
    }
    GemVal _t2347;
    if (!gem_truthy(_t2346)) {
        _t2347 = _t2346;
    } else {
        _t2347 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2349;
    if (!gem_truthy(_t2347)) {
        _t2349 = _t2347;
    } else {
        GemVal _t2348[] = {gem_v__match_32, gem_string("stmts")};
        _t2349 = gem_has_key_fn(NULL, _t2348, 2);
    }
    if (gem_truthy(_t2349)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_32, gem_string("stmts"));
#line 556 "compiler/main.gem"
    GemVal _t2350[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2351 = (*gem_v_walk_captures);
        GemVal _t2352 = _t2351.fn(_t2351.env, _t2350, 3);
        gem_pop_frame();
        return _t2352;
    } else {
    GemVal _t2353[] = {gem_v__match_32};
    GemVal _t2355;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2353, 1), gem_string("table")))) {
        _t2355 = gem_eq(gem_type_fn(NULL, _t2353, 1), gem_string("table"));
    } else {
        GemVal _t2354[] = {gem_v__match_32, gem_string("tag")};
        _t2355 = gem_has_key_fn(NULL, _t2354, 2);
    }
    GemVal _t2356;
    if (!gem_truthy(_t2355)) {
        _t2356 = _t2355;
    } else {
        _t2356 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2358;
    if (!gem_truthy(_t2356)) {
        _t2358 = _t2356;
    } else {
        GemVal _t2357[] = {gem_v__match_32, gem_string("value")};
        _t2358 = gem_has_key_fn(NULL, _t2357, 2);
    }
    if (gem_truthy(_t2358)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_32, gem_string("value"));
#line 558 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 559 "compiler/main.gem"
    GemVal _t2359[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2360 = (*gem_v_walk_captures_node);
            GemVal _t2361 = _t2360.fn(_t2360.env, _t2359, 3);
            gem_pop_frame();
            return _t2361;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2362[] = {gem_v__match_32};
    GemVal _t2364;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2362, 1), gem_string("table")))) {
        _t2364 = gem_eq(gem_type_fn(NULL, _t2362, 1), gem_string("table"));
    } else {
        GemVal _t2363[] = {gem_v__match_32, gem_string("tag")};
        _t2364 = gem_has_key_fn(NULL, _t2363, 2);
    }
    GemVal _t2365;
    if (!gem_truthy(_t2364)) {
        _t2365 = _t2364;
    } else {
        _t2365 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2367;
    if (!gem_truthy(_t2365)) {
        _t2367 = _t2365;
    } else {
        GemVal _t2366[] = {gem_v__match_32, gem_string("value")};
        _t2367 = gem_has_key_fn(NULL, _t2366, 2);
    }
    if (gem_truthy(_t2367)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_32, gem_string("value"));
#line 562 "compiler/main.gem"
    GemVal _t2368[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2369 = (*gem_v_walk_captures_node);
        GemVal _t2370 = _t2369.fn(_t2369.env, _t2368, 3);
        gem_pop_frame();
        return _t2370;
    } else {
    GemVal _t2371[] = {gem_v__match_32};
    GemVal _t2373;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2371, 1), gem_string("table")))) {
        _t2373 = gem_eq(gem_type_fn(NULL, _t2371, 1), gem_string("table"));
    } else {
        GemVal _t2372[] = {gem_v__match_32, gem_string("tag")};
        _t2373 = gem_has_key_fn(NULL, _t2372, 2);
    }
    GemVal _t2374;
    if (!gem_truthy(_t2373)) {
        _t2374 = _t2373;
    } else {
        _t2374 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2376;
    if (!gem_truthy(_t2374)) {
        _t2376 = _t2374;
    } else {
        GemVal _t2375[] = {gem_v__match_32, gem_string("value")};
        _t2376 = gem_has_key_fn(NULL, _t2375, 2);
    }
    if (gem_truthy(_t2376)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_32, gem_string("value"));
#line 564 "compiler/main.gem"
    GemVal _t2377[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2378 = (*gem_v_walk_captures_node);
        GemVal _t2379 = _t2378.fn(_t2378.env, _t2377, 3);
        gem_pop_frame();
        return _t2379;
    } else {
        {
#line 567 "compiler/main.gem"
            GemVal gem_v__for_tbl_41 = gem_v_node;
#line 567 "compiler/main.gem"
    GemVal _t2380[] = {gem_v__for_tbl_41};
            GemVal gem_v__for_len_41 = gem_len_fn(NULL, _t2380, 1);
#line 567 "compiler/main.gem"
            GemVal gem_v__for_i_41 = gem_int(0);
#line 567 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_len_41))) break;
#line 567 "compiler/main.gem"
    GemVal _t2381[] = {gem_v__for_tbl_41, gem_v__for_i_41};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2381, 2);
#line 567 "compiler/main.gem"
    GemVal _t2382[] = {gem_v__for_tbl_41, gem_v__for_i_41};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2382, 2);
#line 567 "compiler/main.gem"
                gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 568 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 569 "compiler/main.gem"
    GemVal _t2383[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2383, 1), gem_string("table")))) {
#line 570 "compiler/main.gem"
    GemVal _t2384[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2384, 1);
#line 571 "compiler/main.gem"
    GemVal _t2385[] = {gem_v_vks};
    GemVal _t2387;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2385, 1), gem_int(0)))) {
        _t2387 = gem_gt(gem_len_fn(NULL, _t2385, 1), gem_int(0));
    } else {
        GemVal _t2386[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2387 = gem_eq(gem_type_fn(NULL, _t2386, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2387)) {
#line 572 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 573 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2388[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2388, 1)))) break;
#line 574 "compiler/main.gem"
    GemVal _t2389[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2389, 1), gem_string("table")))) {
#line 575 "compiler/main.gem"
    GemVal _t2390[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2391 = (*gem_v_walk_captures_node);
                                    (void)(_t2391.fn(_t2391.env, _t2390, 3));
                                }
#line 577 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 580 "compiler/main.gem"
    GemVal _t2392[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2393 = (*gem_v_walk_captures_node);
                            (void)(_t2393.fn(_t2393.env, _t2392, 3));
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
#line 589 "compiler/main.gem"
    GemVal _t2395 = gem_table_new();
    GemVal _t2396[] = {gem_v_scope_vars, _t2395};
    GemVal gem_v_sv = gem_fn__mod_codegen_set_union(NULL, _t2396, 2);
    {
#line 590 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 590 "compiler/main.gem"
    GemVal _t2397[] = {gem_v_stmts};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2397, 1);
#line 590 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 590 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 590 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 591 "compiler/main.gem"
    GemVal _t2398[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2399 = (*gem_v_walk_captures_node);
            (void)(_t2399.fn(_t2399.env, _t2398, 3));
#line 592 "compiler/main.gem"
    GemVal _t2400[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2400, 2))) {
#line 593 "compiler/main.gem"
    GemVal _t2401 = gem_table_get(gem_v_stmts, gem_v_i);
    static GemICacheSlot _ic_341 = {0};
    GemVal _t2402[] = {gem_v_sv, gem_table_get_cached(_t2401, "name", &_ic_341)};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2402, 2));
            }
#line 595 "compiler/main.gem"
    GemVal _t2403[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2403, 2));
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
#line 600 "compiler/main.gem"
    GemVal _t2405 = gem_table_new();
    GemVal gem_v_captured = _t2405;
#line 601 "compiler/main.gem"
    GemVal _t2406[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2407 = (*gem_v_walk_captures);
    (void)(_t2407.fn(_t2407.env, _t2406, 3));
    GemVal _t2408 = gem_v_captured;
    gem_pop_frame();
    return _t2408;
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
#line 632 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 633 "compiler/main.gem"
        GemVal _t2410 = gem_bool(0);
        gem_pop_frame();
        return _t2410;
    }
#line 635 "compiler/main.gem"
    GemVal _t2411[] = {gem_v_node};
    GemVal _t2413;
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2411, 1), gem_string("table")))) {
        _t2413 = gem_neq(gem_type_fn(NULL, _t2411, 1), gem_string("table"));
    } else {
        GemVal _t2412 = gem_v_node;
        static GemICacheSlot _ic_342 = {0};
        _t2413 = gem_eq(gem_table_get_cached(_t2412, "tag", &_ic_342), GEM_NIL);
    }
    if (gem_truthy(_t2413)) {
#line 636 "compiler/main.gem"
        GemVal _t2414 = gem_bool(0);
        gem_pop_frame();
        return _t2414;
    }
#line 638 "compiler/main.gem"
    GemVal gem_v__match_43 = gem_v_node;
    GemVal _t2415[] = {gem_v__match_43};
    GemVal _t2417;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2415, 1), gem_string("table")))) {
        _t2417 = gem_eq(gem_type_fn(NULL, _t2415, 1), gem_string("table"));
    } else {
        GemVal _t2416[] = {gem_v__match_43, gem_string("tag")};
        _t2417 = gem_has_key_fn(NULL, _t2416, 2);
    }
    GemVal _t2418;
    if (!gem_truthy(_t2417)) {
        _t2418 = _t2417;
    } else {
        _t2418 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("call"));
    }
    GemVal _t2420;
    if (!gem_truthy(_t2418)) {
        _t2420 = _t2418;
    } else {
        GemVal _t2419[] = {gem_v__match_43, gem_string("func")};
        _t2420 = gem_has_key_fn(NULL, _t2419, 2);
    }
    if (gem_truthy(_t2420)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_func = gem_table_get(gem_v__match_43, gem_string("func"));
#line 640 "compiler/main.gem"
    GemVal _t2421[] = {gem_v_func, gem_string("var")};
    GemVal _t2423;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2421, 2))) {
        _t2423 = gem_fn__mod_codegen_is_node(NULL, _t2421, 2);
    } else {
        GemVal _t2422 = gem_v_func;
        static GemICacheSlot _ic_343 = {0};
        _t2423 = gem_eq(gem_table_get_cached(_t2422, "name", &_ic_343), gem_v_fn_name);
    }
        GemVal _t2424 = _t2423;
        gem_pop_frame();
        return _t2424;
    } else {
    GemVal _t2425[] = {gem_v__match_43};
    GemVal _t2427;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2425, 1), gem_string("table")))) {
        _t2427 = gem_eq(gem_type_fn(NULL, _t2425, 1), gem_string("table"));
    } else {
        GemVal _t2426[] = {gem_v__match_43, gem_string("tag")};
        _t2427 = gem_has_key_fn(NULL, _t2426, 2);
    }
    GemVal _t2428;
    if (!gem_truthy(_t2427)) {
        _t2428 = _t2427;
    } else {
        _t2428 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2430;
    if (!gem_truthy(_t2428)) {
        _t2430 = _t2428;
    } else {
        GemVal _t2429[] = {gem_v__match_43, gem_string("value")};
        _t2430 = gem_has_key_fn(NULL, _t2429, 2);
    }
    if (gem_truthy(_t2430)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_43, gem_string("value"));
#line 642 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 643 "compiler/main.gem"
    GemVal _t2431[] = {gem_v_fn_name, gem_v_value};
    GemVal _t2432 = (*gem_v_is_self_tail_call);
            GemVal _t2433 = _t2432.fn(_t2432.env, _t2431, 2);
            gem_pop_frame();
            return _t2433;
        }
#line 645 "compiler/main.gem"
        GemVal _t2434 = gem_bool(0);
        gem_pop_frame();
        return _t2434;
    } else {
    GemVal _t2435[] = {gem_v__match_43};
    GemVal _t2437;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2435, 1), gem_string("table")))) {
        _t2437 = gem_eq(gem_type_fn(NULL, _t2435, 1), gem_string("table"));
    } else {
        GemVal _t2436[] = {gem_v__match_43, gem_string("tag")};
        _t2437 = gem_has_key_fn(NULL, _t2436, 2);
    }
    GemVal _t2438;
    if (!gem_truthy(_t2437)) {
        _t2438 = _t2437;
    } else {
        _t2438 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2440;
    if (!gem_truthy(_t2438)) {
        _t2440 = _t2438;
    } else {
        GemVal _t2439[] = {gem_v__match_43, gem_string("then")};
        _t2440 = gem_has_key_fn(NULL, _t2439, 2);
    }
    GemVal _t2442;
    if (!gem_truthy(_t2440)) {
        _t2442 = _t2440;
    } else {
        GemVal _t2441[] = {gem_v__match_43, gem_string("else")};
        _t2442 = gem_has_key_fn(NULL, _t2441, 2);
    }
    if (gem_truthy(_t2442)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_then_body = gem_table_get(gem_v__match_43, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_43, gem_string("else"));
#line 647 "compiler/main.gem"
    GemVal _t2443[] = {gem_v_fn_name, gem_v_then_body};
    GemVal _t2444 = (*gem_v_has_self_tail_call);
        if (gem_truthy(_t2444.fn(_t2444.env, _t2443, 2))) {
#line 648 "compiler/main.gem"
            GemVal _t2445 = gem_bool(1);
            gem_pop_frame();
            return _t2445;
        }
#line 650 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 651 "compiler/main.gem"
    GemVal _t2446[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2447 = (*gem_v_has_self_tail_call);
            GemVal _t2448 = _t2447.fn(_t2447.env, _t2446, 2);
            gem_pop_frame();
            return _t2448;
        }
#line 653 "compiler/main.gem"
        GemVal _t2449 = gem_bool(0);
        gem_pop_frame();
        return _t2449;
    } else {
    GemVal _t2450[] = {gem_v__match_43};
    GemVal _t2452;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2450, 1), gem_string("table")))) {
        _t2452 = gem_eq(gem_type_fn(NULL, _t2450, 1), gem_string("table"));
    } else {
        GemVal _t2451[] = {gem_v__match_43, gem_string("tag")};
        _t2452 = gem_has_key_fn(NULL, _t2451, 2);
    }
    GemVal _t2453;
    if (!gem_truthy(_t2452)) {
        _t2453 = _t2452;
    } else {
        _t2453 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2455;
    if (!gem_truthy(_t2453)) {
        _t2455 = _t2453;
    } else {
        GemVal _t2454[] = {gem_v__match_43, gem_string("whens")};
        _t2455 = gem_has_key_fn(NULL, _t2454, 2);
    }
    GemVal _t2457;
    if (!gem_truthy(_t2455)) {
        _t2457 = _t2455;
    } else {
        GemVal _t2456[] = {gem_v__match_43, gem_string("else")};
        _t2457 = gem_has_key_fn(NULL, _t2456, 2);
    }
    if (gem_truthy(_t2457)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_43, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_43, gem_string("else"));
#line 655 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 655 "compiler/main.gem"
    GemVal _t2458[] = {gem_v_whens};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2458, 1);
#line 655 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 655 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 655 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 656 "compiler/main.gem"
    GemVal _t2459 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_344 = {0};
    GemVal _t2460[] = {gem_v_fn_name, gem_table_get_cached(_t2459, "body", &_ic_344)};
    GemVal _t2461 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2461.fn(_t2461.env, _t2460, 2))) {
#line 657 "compiler/main.gem"
                GemVal _t2462 = gem_bool(1);
                gem_pop_frame();
                return _t2462;
            }
        }

#line 660 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 661 "compiler/main.gem"
    GemVal _t2463[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2464 = (*gem_v_has_self_tail_call);
            GemVal _t2465 = _t2464.fn(_t2464.env, _t2463, 2);
            gem_pop_frame();
            return _t2465;
        }
#line 663 "compiler/main.gem"
        GemVal _t2466 = gem_bool(0);
        gem_pop_frame();
        return _t2466;
    } else {
    GemVal _t2467[] = {gem_v__match_43};
    GemVal _t2469;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2467, 1), gem_string("table")))) {
        _t2469 = gem_eq(gem_type_fn(NULL, _t2467, 1), gem_string("table"));
    } else {
        GemVal _t2468[] = {gem_v__match_43, gem_string("tag")};
        _t2469 = gem_has_key_fn(NULL, _t2468, 2);
    }
    GemVal _t2470;
    if (!gem_truthy(_t2469)) {
        _t2470 = _t2469;
    } else {
        _t2470 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2472;
    if (!gem_truthy(_t2470)) {
        _t2472 = _t2470;
    } else {
        GemVal _t2471[] = {gem_v__match_43, gem_string("arms")};
        _t2472 = gem_has_key_fn(NULL, _t2471, 2);
    }
    GemVal _t2474;
    if (!gem_truthy(_t2472)) {
        _t2474 = _t2472;
    } else {
        GemVal _t2473[] = {gem_v__match_43, gem_string("after_body")};
        _t2474 = gem_has_key_fn(NULL, _t2473, 2);
    }
    if (gem_truthy(_t2474)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_arms = gem_table_get(gem_v__match_43, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_after_body = gem_table_get(gem_v__match_43, gem_string("after_body"));
#line 665 "compiler/main.gem"
        GemVal gem_v__for_i_45 = gem_int(0);
#line 665 "compiler/main.gem"
    GemVal _t2475[] = {gem_v_arms};
        GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2475, 1);
#line 665 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 665 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_45;
#line 665 "compiler/main.gem"
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 666 "compiler/main.gem"
    GemVal _t2476 = gem_table_get(gem_v_arms, gem_v_i);
    static GemICacheSlot _ic_345 = {0};
    GemVal _t2477[] = {gem_v_fn_name, gem_table_get_cached(_t2476, "body", &_ic_345)};
    GemVal _t2478 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2478.fn(_t2478.env, _t2477, 2))) {
#line 667 "compiler/main.gem"
                GemVal _t2479 = gem_bool(1);
                gem_pop_frame();
                return _t2479;
            }
        }

#line 670 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 671 "compiler/main.gem"
    GemVal _t2480[] = {gem_v_fn_name, gem_v_after_body};
    GemVal _t2481 = (*gem_v_has_self_tail_call);
            GemVal _t2482 = _t2481.fn(_t2481.env, _t2480, 2);
            gem_pop_frame();
            return _t2482;
        }
#line 673 "compiler/main.gem"
        GemVal _t2483 = gem_bool(0);
        gem_pop_frame();
        return _t2483;
    } else {
    GemVal _t2484[] = {gem_v__match_43};
    GemVal _t2486;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2484, 1), gem_string("table")))) {
        _t2486 = gem_eq(gem_type_fn(NULL, _t2484, 1), gem_string("table"));
    } else {
        GemVal _t2485[] = {gem_v__match_43, gem_string("tag")};
        _t2486 = gem_has_key_fn(NULL, _t2485, 2);
    }
    GemVal _t2487;
    if (!gem_truthy(_t2486)) {
        _t2487 = _t2486;
    } else {
        _t2487 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2489;
    if (!gem_truthy(_t2487)) {
        _t2489 = _t2487;
    } else {
        GemVal _t2488[] = {gem_v__match_43, gem_string("stmts")};
        _t2489 = gem_has_key_fn(NULL, _t2488, 2);
    }
    if (gem_truthy(_t2489)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_43, gem_string("stmts"));
#line 675 "compiler/main.gem"
    GemVal _t2490[] = {gem_v_fn_name, gem_v_stmts};
    GemVal _t2491 = (*gem_v_has_self_tail_call);
        GemVal _t2492 = _t2491.fn(_t2491.env, _t2490, 2);
        gem_pop_frame();
        return _t2492;
    }
    }
    }
    }
    }
    }
#line 677 "compiler/main.gem"
    GemVal _t2493 = gem_bool(0);
    gem_pop_frame();
    return _t2493;
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
#line 681 "compiler/main.gem"
    GemVal _t2495[] = {gem_v_stmts};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2495, 1), gem_int(0)))) {
#line 682 "compiler/main.gem"
        GemVal _t2496 = gem_bool(0);
        gem_pop_frame();
        return _t2496;
    }
#line 684 "compiler/main.gem"
    GemVal _t2497[] = {gem_v_fn_name, gem_table_get(gem_v_stmts, gem_neg(gem_int(1)))};
    GemVal _t2498 = (*gem_v_is_self_tail_call);
    GemVal _t2499 = _t2498.fn(_t2498.env, _t2497, 2);
    gem_pop_frame();
    return _t2499;
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
#line 688 "compiler/main.gem"
    GemVal _t2501[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t2501, 1);
#line 689 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 690 "compiler/main.gem"
    GemVal _t2502 = gem_v_call_node;
    static GemICacheSlot _ic_346 = {0};
    GemVal gem_v_args = gem_table_get_cached(_t2502, "args", &_ic_346);
#line 691 "compiler/main.gem"
    GemVal _t2503 = gem_table_new();
    GemVal gem_v_arg_temps = _t2503;
#line 692 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 692 "compiler/main.gem"
    GemVal _t2504[] = {gem_v_args};
    GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t2504, 1);
#line 692 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 692 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_46;
#line 692 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 693 "compiler/main.gem"
    GemVal _t2505[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2506 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2506.fn(_t2506.env, _t2505, 1);
#line 694 "compiler/main.gem"
    GemVal _t2507 = (*gem_v_tmp);
        GemVal gem_v_t = _t2507.fn(_t2507.env, NULL, 0);
#line 695 "compiler/main.gem"
    GemVal _t2508 = gem_v_r;
    static GemICacheSlot _ic_347 = {0};
    GemVal _t2509[] = {gem_v_b, gem_table_get_cached(_t2508, "setup", &_ic_347)};
        (void)(gem_buf_push_fn(NULL, _t2509, 2));
#line 696 "compiler/main.gem"
    GemVal _t2510 = gem_v_r;
    static GemICacheSlot _ic_348 = {0};
    GemVal _t2511[] = {gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t2510, "expr", &_ic_348), gem_string(";\n")};
    GemVal _t2512[] = {gem_v_b, gem_interp(6, _t2511)};
        (void)(gem_buf_push_fn(NULL, _t2512, 2));
#line 697 "compiler/main.gem"
    GemVal _t2513[] = {gem_v_arg_temps, gem_v_t};
        (void)(gem_push_fn(NULL, _t2513, 2));
    }

#line 699 "compiler/main.gem"
    GemVal gem_v__for_i_47 = gem_int(0);
#line 699 "compiler/main.gem"
    GemVal _t2514[] = {(*gem_v_tco_params)};
    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2514, 1);
#line 699 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 699 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_47;
#line 699 "compiler/main.gem"
        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 700 "compiler/main.gem"
    GemVal _t2515[] = {gem_table_get((*gem_v_tco_params), gem_v_i)};
    GemVal _t2516 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2516.fn(_t2516.env, _t2515, 1);
#line 701 "compiler/main.gem"
        GemVal gem_v_val = gem_string("GEM_NIL");
#line 702 "compiler/main.gem"
    GemVal _t2517[] = {gem_v_arg_temps};
        if (gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t2517, 1)))) {
#line 703 "compiler/main.gem"
            gem_v_val = gem_table_get(gem_v_arg_temps, gem_v_i);
        }
#line 705 "compiler/main.gem"
    GemVal _t2518[] = {(*gem_v_tco_captured), gem_table_get((*gem_v_tco_params), gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2518, 2))) {
#line 706 "compiler/main.gem"
    GemVal _t2519[] = {gem_v_p, gem_string("*"), gem_v_mp, gem_string(" = "), gem_v_val, gem_string(";\n")};
    GemVal _t2520[] = {gem_v_b, gem_interp(6, _t2519)};
            (void)(gem_buf_push_fn(NULL, _t2520, 2));
        } else {
#line 708 "compiler/main.gem"
    GemVal _t2521[] = {gem_v_p, gem_v_mp, gem_string(" = "), gem_v_val, gem_string(";\n")};
    GemVal _t2522[] = {gem_v_b, gem_interp(5, _t2521)};
            (void)(gem_buf_push_fn(NULL, _t2522, 2));
        }
    }

#line 711 "compiler/main.gem"
    GemVal _t2523[] = {gem_v_p, gem_string("continue;")};
    GemVal _t2524[] = {gem_v_b, gem_interp(2, _t2523)};
    (void)(gem_buf_push_fn(NULL, _t2524, 2));
#line 712 "compiler/main.gem"
    GemVal _t2525[] = {gem_v_b};
    GemVal _t2526 = gem_buf_str_fn(NULL, _t2525, 1);
    gem_pop_frame();
    return _t2526;
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
    GemVal *gem_v_ic_counter;
    GemVal *gem_v_ic_init;
    GemVal *gem_v_in_top_level;
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
    GemVal *gem_v_ic_counter = _cls->gem_v_ic_counter;
    GemVal *gem_v_ic_init = _cls->gem_v_ic_init;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 718 "compiler/main.gem"
    GemVal gem_v__match_48 = gem_v_node;
    GemVal _t2528[] = {gem_v__match_48};
    GemVal _t2530;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2528, 1), gem_string("table")))) {
        _t2530 = gem_eq(gem_type_fn(NULL, _t2528, 1), gem_string("table"));
    } else {
        GemVal _t2529[] = {gem_v__match_48, gem_string("tag")};
        _t2530 = gem_has_key_fn(NULL, _t2529, 2);
    }
    GemVal _t2531;
    if (!gem_truthy(_t2530)) {
        _t2531 = _t2530;
    } else {
        _t2531 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2533;
    if (!gem_truthy(_t2531)) {
        _t2533 = _t2531;
    } else {
        GemVal _t2532[] = {gem_v__match_48, gem_string("value")};
        _t2533 = gem_has_key_fn(NULL, _t2532, 2);
    }
    if (gem_truthy(_t2533)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 720 "compiler/main.gem"
    GemVal _t2534 = gem_table_new();
    GemVal _t2535[] = {gem_string("gem_int("), gem_v_value, gem_string(")")};
    gem_table_set(_t2534, gem_string("expr"), gem_interp(3, _t2535));
    gem_table_set(_t2534, gem_string("setup"), gem_string(""));
        GemVal _t2536 = _t2534;
        gem_pop_frame();
        return _t2536;
    } else {
    GemVal _t2537[] = {gem_v__match_48};
    GemVal _t2539;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2537, 1), gem_string("table")))) {
        _t2539 = gem_eq(gem_type_fn(NULL, _t2537, 1), gem_string("table"));
    } else {
        GemVal _t2538[] = {gem_v__match_48, gem_string("tag")};
        _t2539 = gem_has_key_fn(NULL, _t2538, 2);
    }
    GemVal _t2540;
    if (!gem_truthy(_t2539)) {
        _t2540 = _t2539;
    } else {
        _t2540 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2542;
    if (!gem_truthy(_t2540)) {
        _t2542 = _t2540;
    } else {
        GemVal _t2541[] = {gem_v__match_48, gem_string("value")};
        _t2542 = gem_has_key_fn(NULL, _t2541, 2);
    }
    if (gem_truthy(_t2542)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 722 "compiler/main.gem"
    GemVal _t2543 = gem_table_new();
    GemVal _t2544[] = {gem_v_value};
    GemVal _t2545[] = {gem_string("gem_float("), gem_fn__mod_codegen_format_float(NULL, _t2544, 1), gem_string(")")};
    gem_table_set(_t2543, gem_string("expr"), gem_interp(3, _t2545));
    gem_table_set(_t2543, gem_string("setup"), gem_string(""));
        GemVal _t2546 = _t2543;
        gem_pop_frame();
        return _t2546;
    } else {
    GemVal _t2547[] = {gem_v__match_48};
    GemVal _t2549;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2547, 1), gem_string("table")))) {
        _t2549 = gem_eq(gem_type_fn(NULL, _t2547, 1), gem_string("table"));
    } else {
        GemVal _t2548[] = {gem_v__match_48, gem_string("tag")};
        _t2549 = gem_has_key_fn(NULL, _t2548, 2);
    }
    GemVal _t2550;
    if (!gem_truthy(_t2549)) {
        _t2550 = _t2549;
    } else {
        _t2550 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2552;
    if (!gem_truthy(_t2550)) {
        _t2552 = _t2550;
    } else {
        GemVal _t2551[] = {gem_v__match_48, gem_string("value")};
        _t2552 = gem_has_key_fn(NULL, _t2551, 2);
    }
    if (gem_truthy(_t2552)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 724 "compiler/main.gem"
    GemVal _t2553[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2553, 1);
#line 725 "compiler/main.gem"
    GemVal _t2554 = gem_table_new();
    GemVal _t2555[] = {gem_string("gem_string(\""), gem_v_escaped, gem_string("\")")};
    gem_table_set(_t2554, gem_string("expr"), gem_interp(3, _t2555));
    gem_table_set(_t2554, gem_string("setup"), gem_string(""));
        GemVal _t2556 = _t2554;
        gem_pop_frame();
        return _t2556;
    } else {
    GemVal _t2557[] = {gem_v__match_48};
    GemVal _t2559;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2557, 1), gem_string("table")))) {
        _t2559 = gem_eq(gem_type_fn(NULL, _t2557, 1), gem_string("table"));
    } else {
        GemVal _t2558[] = {gem_v__match_48, gem_string("tag")};
        _t2559 = gem_has_key_fn(NULL, _t2558, 2);
    }
    GemVal _t2560;
    if (!gem_truthy(_t2559)) {
        _t2560 = _t2559;
    } else {
        _t2560 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2562;
    if (!gem_truthy(_t2560)) {
        _t2562 = _t2560;
    } else {
        GemVal _t2561[] = {gem_v__match_48, gem_string("parts")};
        _t2562 = gem_has_key_fn(NULL, _t2561, 2);
    }
    if (gem_truthy(_t2562)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_48, gem_string("parts"));
#line 727 "compiler/main.gem"
    GemVal _t2563[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2563, 1), gem_int(0)))) {
#line 728 "compiler/main.gem"
    GemVal _t2564 = gem_table_new();
    gem_table_set(_t2564, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2564, gem_string("setup"), gem_string(""));
            GemVal _t2565 = _t2564;
            gem_pop_frame();
            return _t2565;
        }
#line 730 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 731 "compiler/main.gem"
    GemVal _t2566 = gem_table_new();
        GemVal gem_v_compiled = _t2566;
#line 732 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 732 "compiler/main.gem"
    GemVal _t2567[] = {gem_v_parts};
        GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t2567, 1);
#line 732 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 732 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_49;
#line 732 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 733 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 734 "compiler/main.gem"
    GemVal _t2568[] = {gem_v_part};
    GemVal _t2569 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2569.fn(_t2569.env, _t2568, 1);
#line 735 "compiler/main.gem"
    GemVal _t2570 = gem_v_r;
    static GemICacheSlot _ic_349 = {0};
            gem_v_setup = gem_add(gem_v_setup, gem_table_get_cached(_t2570, "setup", &_ic_349));
#line 736 "compiler/main.gem"
    GemVal _t2571 = gem_v_r;
    static GemICacheSlot _ic_350 = {0};
    GemVal _t2572[] = {gem_v_compiled, gem_table_get_cached(_t2571, "expr", &_ic_350)};
            (void)(gem_push_fn(NULL, _t2572, 2));
        }

#line 738 "compiler/main.gem"
    GemVal _t2573 = (*gem_v_tmp);
        GemVal gem_v_arr_tmp = _t2573.fn(_t2573.env, NULL, 0);
#line 739 "compiler/main.gem"
        GemVal gem_v_items = gem_table_get(gem_v_compiled, gem_int(0));
#line 740 "compiler/main.gem"
        GemVal gem_v__for_i_50 = gem_int(1);
#line 740 "compiler/main.gem"
    GemVal _t2574[] = {gem_v_compiled};
        GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2574, 1);
#line 740 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 740 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_50;
#line 740 "compiler/main.gem"
            gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 741 "compiler/main.gem"
            gem_v_items = gem_add(gem_add(gem_v_items, gem_string(", ")), gem_table_get(gem_v_compiled, gem_v_i));
        }

#line 743 "compiler/main.gem"
    GemVal _t2575[] = {gem_string("    GemVal "), gem_v_arr_tmp, gem_string("[] = {"), gem_v_items, gem_string("};\n")};
        gem_v_setup = gem_add(gem_v_setup, gem_interp(5, _t2575));
#line 744 "compiler/main.gem"
    GemVal _t2576 = gem_table_new();
    GemVal _t2577[] = {gem_v_compiled};
    GemVal _t2578[] = {gem_string("gem_interp("), gem_len_fn(NULL, _t2577, 1), gem_string(", "), gem_v_arr_tmp, gem_string(")")};
    gem_table_set(_t2576, gem_string("expr"), gem_interp(5, _t2578));
    gem_table_set(_t2576, gem_string("setup"), gem_v_setup);
        GemVal _t2579 = _t2576;
        gem_pop_frame();
        return _t2579;
    } else {
    GemVal _t2580[] = {gem_v__match_48};
    GemVal _t2582;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2580, 1), gem_string("table")))) {
        _t2582 = gem_eq(gem_type_fn(NULL, _t2580, 1), gem_string("table"));
    } else {
        GemVal _t2581[] = {gem_v__match_48, gem_string("tag")};
        _t2582 = gem_has_key_fn(NULL, _t2581, 2);
    }
    GemVal _t2583;
    if (!gem_truthy(_t2582)) {
        _t2583 = _t2582;
    } else {
        _t2583 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2585;
    if (!gem_truthy(_t2583)) {
        _t2585 = _t2583;
    } else {
        GemVal _t2584[] = {gem_v__match_48, gem_string("value")};
        _t2585 = gem_has_key_fn(NULL, _t2584, 2);
    }
    if (gem_truthy(_t2585)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 746 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 747 "compiler/main.gem"
    GemVal _t2586 = gem_table_new();
    gem_table_set(_t2586, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2586, gem_string("setup"), gem_string(""));
            GemVal _t2587 = _t2586;
            gem_pop_frame();
            return _t2587;
        }
#line 749 "compiler/main.gem"
    GemVal _t2588 = gem_table_new();
    gem_table_set(_t2588, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2588, gem_string("setup"), gem_string(""));
        GemVal _t2589 = _t2588;
        gem_pop_frame();
        return _t2589;
    } else {
    GemVal _t2590[] = {gem_v__match_48};
    GemVal _t2592;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2590, 1), gem_string("table")))) {
        _t2592 = gem_eq(gem_type_fn(NULL, _t2590, 1), gem_string("table"));
    } else {
        GemVal _t2591[] = {gem_v__match_48, gem_string("tag")};
        _t2592 = gem_has_key_fn(NULL, _t2591, 2);
    }
    GemVal _t2593;
    if (!gem_truthy(_t2592)) {
        _t2593 = _t2592;
    } else {
        _t2593 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2593)) {
#line 751 "compiler/main.gem"
    GemVal _t2594 = gem_table_new();
    gem_table_set(_t2594, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2594, gem_string("setup"), gem_string(""));
        GemVal _t2595 = _t2594;
        gem_pop_frame();
        return _t2595;
    } else {
    GemVal _t2596[] = {gem_v__match_48};
    GemVal _t2598;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2596, 1), gem_string("table")))) {
        _t2598 = gem_eq(gem_type_fn(NULL, _t2596, 1), gem_string("table"));
    } else {
        GemVal _t2597[] = {gem_v__match_48, gem_string("tag")};
        _t2598 = gem_has_key_fn(NULL, _t2597, 2);
    }
    GemVal _t2599;
    if (!gem_truthy(_t2598)) {
        _t2599 = _t2598;
    } else {
        _t2599 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2601;
    if (!gem_truthy(_t2599)) {
        _t2601 = _t2599;
    } else {
        GemVal _t2600[] = {gem_v__match_48, gem_string("name")};
        _t2601 = gem_has_key_fn(NULL, _t2600, 2);
    }
    if (gem_truthy(_t2601)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_48, gem_string("name"));
#line 753 "compiler/main.gem"
    GemVal _t2602[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2602, 2)))) {
#line 754 "compiler/main.gem"
    GemVal _t2603[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2603, 2))) {
#line 755 "compiler/main.gem"
    GemVal _t2604 = gem_table_new();
    GemVal _t2605[] = {gem_string("gem_make_fn("), gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string(", NULL)")};
    gem_table_set(_t2604, gem_string("expr"), gem_interp(3, _t2605));
    gem_table_set(_t2604, gem_string("setup"), gem_string(""));
                GemVal _t2606 = _t2604;
                gem_pop_frame();
                return _t2606;
            } else {
#line 756 "compiler/main.gem"
    GemVal _t2607[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2607, 2))) {
#line 757 "compiler/main.gem"
    GemVal _t2608 = gem_table_new();
    GemVal _t2609[] = {gem_string("gem_make_fn(gem_fn_"), gem_v_name, gem_string(", NULL)")};
    gem_table_set(_t2608, gem_string("expr"), gem_interp(3, _t2609));
    gem_table_set(_t2608, gem_string("setup"), gem_string(""));
                    GemVal _t2610 = _t2608;
                    gem_pop_frame();
                    return _t2610;
                }
            }
        }
#line 760 "compiler/main.gem"
    GemVal _t2611[] = {gem_v_name};
    GemVal _t2612 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2612.fn(_t2612.env, _t2611, 1);
#line 761 "compiler/main.gem"
    GemVal _t2613[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2616;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2613, 2))) {
        _t2616 = gem_fn__mod_codegen_set_contains(NULL, _t2613, 2);
    } else {
        GemVal _t2615;
        if (!gem_truthy((*gem_v_in_top_level))) {
                _t2615 = (*gem_v_in_top_level);
        } else {
                GemVal _t2614[] = {(*gem_v_top_level_boxed), gem_v_name};
                _t2615 = gem_fn__mod_codegen_set_contains(NULL, _t2614, 2);
        }
        _t2616 = _t2615;
    }
        if (gem_truthy(_t2616)) {
#line 762 "compiler/main.gem"
    GemVal _t2617 = gem_table_new();
    GemVal _t2618[] = {gem_string("(*"), gem_v_mname, gem_string(")")};
    gem_table_set(_t2617, gem_string("expr"), gem_interp(3, _t2618));
    gem_table_set(_t2617, gem_string("setup"), gem_string(""));
            GemVal _t2619 = _t2617;
            gem_pop_frame();
            return _t2619;
        }
#line 764 "compiler/main.gem"
    GemVal _t2620 = gem_table_new();
    gem_table_set(_t2620, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2620, gem_string("setup"), gem_string(""));
        GemVal _t2621 = _t2620;
        gem_pop_frame();
        return _t2621;
    } else {
    GemVal _t2622[] = {gem_v__match_48};
    GemVal _t2624;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2622, 1), gem_string("table")))) {
        _t2624 = gem_eq(gem_type_fn(NULL, _t2622, 1), gem_string("table"));
    } else {
        GemVal _t2623[] = {gem_v__match_48, gem_string("tag")};
        _t2624 = gem_has_key_fn(NULL, _t2623, 2);
    }
    GemVal _t2625;
    if (!gem_truthy(_t2624)) {
        _t2625 = _t2624;
    } else {
        _t2625 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2625)) {
#line 766 "compiler/main.gem"
    GemVal _t2626[] = {gem_v_node};
    GemVal _t2627 = (*gem_v_compile_call);
        GemVal _t2628 = _t2627.fn(_t2627.env, _t2626, 1);
        gem_pop_frame();
        return _t2628;
    } else {
    GemVal _t2629[] = {gem_v__match_48};
    GemVal _t2631;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2629, 1), gem_string("table")))) {
        _t2631 = gem_eq(gem_type_fn(NULL, _t2629, 1), gem_string("table"));
    } else {
        GemVal _t2630[] = {gem_v__match_48, gem_string("tag")};
        _t2631 = gem_has_key_fn(NULL, _t2630, 2);
    }
    GemVal _t2632;
    if (!gem_truthy(_t2631)) {
        _t2632 = _t2631;
    } else {
        _t2632 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2632)) {
#line 768 "compiler/main.gem"
    GemVal _t2633[] = {gem_v_node};
    GemVal _t2634 = (*gem_v_compile_binop);
        GemVal _t2635 = _t2634.fn(_t2634.env, _t2633, 1);
        gem_pop_frame();
        return _t2635;
    } else {
    GemVal _t2636[] = {gem_v__match_48};
    GemVal _t2638;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2636, 1), gem_string("table")))) {
        _t2638 = gem_eq(gem_type_fn(NULL, _t2636, 1), gem_string("table"));
    } else {
        GemVal _t2637[] = {gem_v__match_48, gem_string("tag")};
        _t2638 = gem_has_key_fn(NULL, _t2637, 2);
    }
    GemVal _t2639;
    if (!gem_truthy(_t2638)) {
        _t2639 = _t2638;
    } else {
        _t2639 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2641;
    if (!gem_truthy(_t2639)) {
        _t2641 = _t2639;
    } else {
        GemVal _t2640[] = {gem_v__match_48, gem_string("expr")};
        _t2641 = gem_has_key_fn(NULL, _t2640, 2);
    }
    GemVal _t2643;
    if (!gem_truthy(_t2641)) {
        _t2643 = _t2641;
    } else {
        GemVal _t2642[] = {gem_v__match_48, gem_string("op")};
        _t2643 = gem_has_key_fn(NULL, _t2642, 2);
    }
    if (gem_truthy(_t2643)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_48, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_48, gem_string("op"));
#line 770 "compiler/main.gem"
    GemVal _t2644[] = {gem_v_uexpr};
    GemVal _t2645 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2645.fn(_t2645.env, _t2644, 1);
#line 771 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 772 "compiler/main.gem"
    GemVal _t2646 = gem_table_new();
    GemVal _t2647 = gem_v_r;
    static GemICacheSlot _ic_351 = {0};
    GemVal _t2648[] = {gem_string("gem_neg("), gem_table_get_cached(_t2647, "expr", &_ic_351), gem_string(")")};
    gem_table_set(_t2646, gem_string("expr"), gem_interp(3, _t2648));
    GemVal _t2649 = gem_v_r;
    static GemICacheSlot _ic_352 = {0};
    gem_table_set(_t2646, gem_string("setup"), gem_table_get_cached(_t2649, "setup", &_ic_352));
            GemVal _t2650 = _t2646;
            gem_pop_frame();
            return _t2650;
        } else {
#line 773 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 774 "compiler/main.gem"
    GemVal _t2651 = gem_table_new();
    GemVal _t2652 = gem_v_r;
    static GemICacheSlot _ic_353 = {0};
    GemVal _t2653[] = {gem_string("gem_not("), gem_table_get_cached(_t2652, "expr", &_ic_353), gem_string(")")};
    gem_table_set(_t2651, gem_string("expr"), gem_interp(3, _t2653));
    GemVal _t2654 = gem_v_r;
    static GemICacheSlot _ic_354 = {0};
    gem_table_set(_t2651, gem_string("setup"), gem_table_get_cached(_t2654, "setup", &_ic_354));
                GemVal _t2655 = _t2651;
                gem_pop_frame();
                return _t2655;
            }
        }
#line 776 "compiler/main.gem"
    GemVal _t2656[] = {gem_string("unknown unary operator: "), gem_v_op};
    GemVal _t2657[] = {gem_interp(2, _t2656)};
        (void)(gem_error_at_fn("compiler/main.gem", 776, _t2657, 1));
    } else {
    GemVal _t2658[] = {gem_v__match_48};
    GemVal _t2660;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2658, 1), gem_string("table")))) {
        _t2660 = gem_eq(gem_type_fn(NULL, _t2658, 1), gem_string("table"));
    } else {
        GemVal _t2659[] = {gem_v__match_48, gem_string("tag")};
        _t2660 = gem_has_key_fn(NULL, _t2659, 2);
    }
    GemVal _t2661;
    if (!gem_truthy(_t2660)) {
        _t2661 = _t2660;
    } else {
        _t2661 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2661)) {
#line 778 "compiler/main.gem"
    GemVal _t2662[] = {gem_v_node};
    GemVal _t2663 = (*gem_v_compile_anon_fn);
        GemVal _t2664 = _t2663.fn(_t2663.env, _t2662, 1);
        gem_pop_frame();
        return _t2664;
    } else {
    GemVal _t2665[] = {gem_v__match_48};
    GemVal _t2667;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2665, 1), gem_string("table")))) {
        _t2667 = gem_eq(gem_type_fn(NULL, _t2665, 1), gem_string("table"));
    } else {
        GemVal _t2666[] = {gem_v__match_48, gem_string("tag")};
        _t2667 = gem_has_key_fn(NULL, _t2666, 2);
    }
    GemVal _t2668;
    if (!gem_truthy(_t2667)) {
        _t2668 = _t2667;
    } else {
        _t2668 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2668)) {
#line 780 "compiler/main.gem"
    GemVal _t2669[] = {gem_v_node};
    GemVal _t2670 = (*gem_v_compile_table);
        GemVal _t2671 = _t2670.fn(_t2670.env, _t2669, 1);
        gem_pop_frame();
        return _t2671;
    } else {
    GemVal _t2672[] = {gem_v__match_48};
    GemVal _t2674;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2672, 1), gem_string("table")))) {
        _t2674 = gem_eq(gem_type_fn(NULL, _t2672, 1), gem_string("table"));
    } else {
        GemVal _t2673[] = {gem_v__match_48, gem_string("tag")};
        _t2674 = gem_has_key_fn(NULL, _t2673, 2);
    }
    GemVal _t2675;
    if (!gem_truthy(_t2674)) {
        _t2675 = _t2674;
    } else {
        _t2675 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2675)) {
#line 782 "compiler/main.gem"
    GemVal _t2676[] = {gem_v_node};
    GemVal _t2677 = (*gem_v_compile_array);
        GemVal _t2678 = _t2677.fn(_t2677.env, _t2676, 1);
        gem_pop_frame();
        return _t2678;
    } else {
    GemVal _t2679[] = {gem_v__match_48};
    GemVal _t2681;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2679, 1), gem_string("table")))) {
        _t2681 = gem_eq(gem_type_fn(NULL, _t2679, 1), gem_string("table"));
    } else {
        GemVal _t2680[] = {gem_v__match_48, gem_string("tag")};
        _t2681 = gem_has_key_fn(NULL, _t2680, 2);
    }
    GemVal _t2682;
    if (!gem_truthy(_t2681)) {
        _t2682 = _t2681;
    } else {
        _t2682 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2684;
    if (!gem_truthy(_t2682)) {
        _t2684 = _t2682;
    } else {
        GemVal _t2683[] = {gem_v__match_48, gem_string("object")};
        _t2684 = gem_has_key_fn(NULL, _t2683, 2);
    }
    GemVal _t2686;
    if (!gem_truthy(_t2684)) {
        _t2686 = _t2684;
    } else {
        GemVal _t2685[] = {gem_v__match_48, gem_string("field")};
        _t2686 = gem_has_key_fn(NULL, _t2685, 2);
    }
    if (gem_truthy(_t2686)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_48, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_48, gem_string("field"));
#line 784 "compiler/main.gem"
    GemVal _t2687[] = {gem_v_object};
    GemVal _t2688 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2688.fn(_t2688.env, _t2687, 1);
#line 785 "compiler/main.gem"
    GemVal _t2689 = (*gem_v_tmp);
        GemVal gem_v_t = _t2689.fn(_t2689.env, NULL, 0);
#line 786 "compiler/main.gem"
        *gem_v_ic_counter = gem_add((*gem_v_ic_counter), gem_int(1));
#line 787 "compiler/main.gem"
        GemVal gem_v_ic = (*gem_v_ic_counter);
#line 788 "compiler/main.gem"
    GemVal _t2690[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2690, 1);
#line 789 "compiler/main.gem"
    GemVal _t2691 = gem_v_r;
    static GemICacheSlot _ic_355 = {0};
    GemVal _t2692 = gem_v_r;
    static GemICacheSlot _ic_356 = {0};
    GemVal _t2693[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t2692, "expr", &_ic_356), gem_string(";\n    static GemICacheSlot _ic_"), gem_v_ic, gem_string(" = "), (*gem_v_ic_init), gem_string(";\n")};
        GemVal gem_v_setup = gem_add(gem_table_get_cached(_t2691, "setup", &_ic_355), gem_interp(9, _t2693));
#line 790 "compiler/main.gem"
    GemVal _t2694 = gem_table_new();
    GemVal _t2695[] = {gem_string("gem_table_get_cached("), gem_v_t, gem_string(", \""), gem_v_escaped, gem_string("\", &_ic_"), gem_v_ic, gem_string(")")};
    gem_table_set(_t2694, gem_string("expr"), gem_interp(7, _t2695));
    gem_table_set(_t2694, gem_string("setup"), gem_v_setup);
        GemVal _t2696 = _t2694;
        gem_pop_frame();
        return _t2696;
    } else {
    GemVal _t2697[] = {gem_v__match_48};
    GemVal _t2699;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2697, 1), gem_string("table")))) {
        _t2699 = gem_eq(gem_type_fn(NULL, _t2697, 1), gem_string("table"));
    } else {
        GemVal _t2698[] = {gem_v__match_48, gem_string("tag")};
        _t2699 = gem_has_key_fn(NULL, _t2698, 2);
    }
    GemVal _t2700;
    if (!gem_truthy(_t2699)) {
        _t2700 = _t2699;
    } else {
        _t2700 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2702;
    if (!gem_truthy(_t2700)) {
        _t2702 = _t2700;
    } else {
        GemVal _t2701[] = {gem_v__match_48, gem_string("object")};
        _t2702 = gem_has_key_fn(NULL, _t2701, 2);
    }
    GemVal _t2704;
    if (!gem_truthy(_t2702)) {
        _t2704 = _t2702;
    } else {
        GemVal _t2703[] = {gem_v__match_48, gem_string("key")};
        _t2704 = gem_has_key_fn(NULL, _t2703, 2);
    }
    if (gem_truthy(_t2704)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_48, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_48, gem_string("key"));
#line 792 "compiler/main.gem"
    GemVal _t2705[] = {gem_v_object};
    GemVal _t2706 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2706.fn(_t2706.env, _t2705, 1);
#line 793 "compiler/main.gem"
    GemVal _t2707[] = {gem_v_key};
    GemVal _t2708 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2708.fn(_t2708.env, _t2707, 1);
#line 794 "compiler/main.gem"
    GemVal _t2709 = gem_table_new();
    GemVal _t2710 = gem_v_obj_r;
    static GemICacheSlot _ic_357 = {0};
    GemVal _t2711 = gem_v_key_r;
    static GemICacheSlot _ic_358 = {0};
    GemVal _t2712[] = {gem_string("gem_table_get("), gem_table_get_cached(_t2710, "expr", &_ic_357), gem_string(", "), gem_table_get_cached(_t2711, "expr", &_ic_358), gem_string(")")};
    gem_table_set(_t2709, gem_string("expr"), gem_interp(5, _t2712));
    GemVal _t2713 = gem_v_obj_r;
    static GemICacheSlot _ic_359 = {0};
    GemVal _t2714 = gem_v_key_r;
    static GemICacheSlot _ic_360 = {0};
    gem_table_set(_t2709, gem_string("setup"), gem_add(gem_table_get_cached(_t2713, "setup", &_ic_359), gem_table_get_cached(_t2714, "setup", &_ic_360)));
        GemVal _t2715 = _t2709;
        gem_pop_frame();
        return _t2715;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 797 "compiler/main.gem"
    GemVal _t2716 = gem_v_node;
    static GemICacheSlot _ic_361 = {0};
    GemVal _t2717[] = {gem_string("unknown expression node: "), gem_table_get_cached(_t2716, "tag", &_ic_361)};
    GemVal _t2718[] = {gem_interp(2, _t2717)};
    GemVal _t2719 = gem_error_at_fn("compiler/main.gem", 797, _t2718, 1);
    gem_pop_frame();
    return _t2719;
}

struct _closure__anon_36 {
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_36", "compiler/main.gem", 0);
#line 803 "compiler/main.gem"
    GemVal _t2721[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t2721, 1);
#line 804 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_slen, gem_int(0)))) {
#line 805 "compiler/main.gem"
        GemVal _t2722 = gem_bool(0);
        gem_pop_frame();
        return _t2722;
    }
#line 807 "compiler/main.gem"
    GemVal gem_v_start = gem_int(0);
#line 808 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_int(0)), gem_string("-")))) {
#line 809 "compiler/main.gem"
        gem_v_start = gem_int(1);
#line 810 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_slen, gem_int(1)))) {
#line 811 "compiler/main.gem"
            GemVal _t2723 = gem_bool(0);
            gem_pop_frame();
            return _t2723;
        }
    }
#line 814 "compiler/main.gem"
    GemVal gem_v_i = gem_v_start;
#line 815 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) break;
#line 816 "compiler/main.gem"
    GemVal _t2724[] = {gem_v_s, gem_v_i};
        GemVal gem_v_c = gem_ord_fn(NULL, _t2724, 2);
#line 817 "compiler/main.gem"
    GemVal _t2725;
    if (gem_truthy(gem_lt(gem_v_c, gem_int(48)))) {
        _t2725 = gem_lt(gem_v_c, gem_int(48));
    } else {
        _t2725 = gem_gt(gem_v_c, gem_int(57));
    }
        if (gem_truthy(_t2725)) {
#line 818 "compiler/main.gem"
            GemVal _t2726 = gem_bool(0);
            gem_pop_frame();
            return _t2726;
        }
#line 820 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2727 = gem_bool(1);
    gem_pop_frame();
    return _t2727;
}

struct _closure__anon_37 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_is_int_key;
    GemVal *gem_v_tmp;
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    struct _closure__anon_37 *_cls = (struct _closure__anon_37 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_is_int_key = _cls->gem_v_is_int_key;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 826 "compiler/main.gem"
    GemVal _t2728 = gem_v_node;
    static GemICacheSlot _ic_362 = {0};
    GemVal gem_v_entries = gem_table_get_cached(_t2728, "entries", &_ic_362);
#line 827 "compiler/main.gem"
    GemVal _t2729 = (*gem_v_tmp);
    GemVal gem_v_t = _t2729.fn(_t2729.env, NULL, 0);
#line 828 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 829 "compiler/main.gem"
    GemVal _t2730[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = gem_table_new();\n")};
    GemVal _t2731[] = {gem_v_b, gem_interp(3, _t2730)};
    (void)(gem_buf_push_fn(NULL, _t2731, 2));
#line 830 "compiler/main.gem"
    GemVal gem_v__for_items_51 = gem_v_entries;
#line 830 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 830 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2732[] = {gem_v__for_items_51};
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t2732, 1)))) break;
#line 830 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 830 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 831 "compiler/main.gem"
    GemVal _t2733 = gem_v_entry;
    static GemICacheSlot _ic_363 = {0};
    GemVal _t2734[] = {gem_table_get_cached(_t2733, "value", &_ic_363)};
    GemVal _t2735 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2735.fn(_t2735.env, _t2734, 1);
#line 832 "compiler/main.gem"
    GemVal _t2736 = gem_v_val_r;
    static GemICacheSlot _ic_364 = {0};
    GemVal _t2737[] = {gem_v_b, gem_table_get_cached(_t2736, "setup", &_ic_364)};
        (void)(gem_buf_push_fn(NULL, _t2737, 2));
#line 833 "compiler/main.gem"
    GemVal _t2738 = gem_v_entry;
    static GemICacheSlot _ic_365 = {0};
    GemVal _t2739[] = {gem_table_get_cached(_t2738, "key", &_ic_365)};
    GemVal _t2740 = (*gem_v_is_int_key);
        if (gem_truthy(_t2740.fn(_t2740.env, _t2739, 1))) {
#line 834 "compiler/main.gem"
    GemVal _t2741 = gem_v_entry;
    static GemICacheSlot _ic_366 = {0};
    GemVal _t2742 = gem_v_val_r;
    static GemICacheSlot _ic_367 = {0};
    GemVal _t2743[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_int("), gem_table_get_cached(_t2741, "key", &_ic_366), gem_string("), "), gem_table_get_cached(_t2742, "expr", &_ic_367), gem_string(");\n")};
    GemVal _t2744[] = {gem_v_b, gem_interp(7, _t2743)};
            (void)(gem_buf_push_fn(NULL, _t2744, 2));
        } else {
#line 836 "compiler/main.gem"
    GemVal _t2745 = gem_v_entry;
    static GemICacheSlot _ic_368 = {0};
    GemVal _t2746[] = {gem_table_get_cached(_t2745, "key", &_ic_368)};
            GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2746, 1);
#line 837 "compiler/main.gem"
    GemVal _t2747 = gem_v_val_r;
    static GemICacheSlot _ic_369 = {0};
    GemVal _t2748[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"), "), gem_table_get_cached(_t2747, "expr", &_ic_369), gem_string(");\n")};
    GemVal _t2749[] = {gem_v_b, gem_interp(7, _t2748)};
            (void)(gem_buf_push_fn(NULL, _t2749, 2));
        }
    }

    GemVal _t2750 = gem_table_new();
    gem_table_set(_t2750, gem_string("expr"), gem_v_t);
    GemVal _t2751[] = {gem_v_b};
    gem_table_set(_t2750, gem_string("setup"), gem_buf_str_fn(NULL, _t2751, 1));
    GemVal _t2752 = _t2750;
    gem_pop_frame();
    return _t2752;
}

struct _closure__anon_38 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_38(void *_env, GemVal *args, int argc) {
    struct _closure__anon_38 *_cls = (struct _closure__anon_38 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 846 "compiler/main.gem"
    GemVal _t2754 = gem_v_node;
    static GemICacheSlot _ic_370 = {0};
    GemVal gem_v_elements = gem_table_get_cached(_t2754, "elements", &_ic_370);
#line 847 "compiler/main.gem"
    GemVal _t2755 = (*gem_v_tmp);
    GemVal gem_v_t = _t2755.fn(_t2755.env, NULL, 0);
#line 848 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 849 "compiler/main.gem"
    GemVal _t2756[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = gem_table_new();\n")};
    GemVal _t2757[] = {gem_v_b, gem_interp(3, _t2756)};
    (void)(gem_buf_push_fn(NULL, _t2757, 2));
#line 850 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 850 "compiler/main.gem"
    GemVal _t2758[] = {gem_v_elements};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2758, 1);
#line 850 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 850 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 850 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 851 "compiler/main.gem"
    GemVal _t2759[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2760 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2760.fn(_t2760.env, _t2759, 1);
#line 852 "compiler/main.gem"
    GemVal _t2761 = gem_v_elem_r;
    static GemICacheSlot _ic_371 = {0};
    GemVal _t2762[] = {gem_v_b, gem_table_get_cached(_t2761, "setup", &_ic_371)};
        (void)(gem_buf_push_fn(NULL, _t2762, 2));
#line 853 "compiler/main.gem"
    GemVal _t2763 = gem_v_elem_r;
    static GemICacheSlot _ic_372 = {0};
    GemVal _t2764[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_int("), gem_v_i, gem_string("), "), gem_table_get_cached(_t2763, "expr", &_ic_372), gem_string(");\n")};
    GemVal _t2765[] = {gem_v_b, gem_interp(7, _t2764)};
        (void)(gem_buf_push_fn(NULL, _t2765, 2));
    }

    GemVal _t2766 = gem_table_new();
    gem_table_set(_t2766, gem_string("expr"), gem_v_t);
    GemVal _t2767[] = {gem_v_b};
    gem_table_set(_t2766, gem_string("setup"), gem_buf_str_fn(NULL, _t2767, 1));
    GemVal _t2768 = _t2766;
    gem_pop_frame();
    return _t2768;
}

struct _closure__anon_39 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_39(void *_env, GemVal *args, int argc) {
    struct _closure__anon_39 *_cls = (struct _closure__anon_39 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn = _cls->gem_v_compile_closure_fn;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_39", "compiler/main.gem", 0);
#line 861 "compiler/main.gem"
    GemVal _t2770 = gem_v_node;
    static GemICacheSlot _ic_373 = {0};
    GemVal _t2771[] = {gem_table_get_cached(_t2770, "params", &_ic_373)};
    GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2771, 1);
#line 862 "compiler/main.gem"
    GemVal _t2772 = gem_v_node;
    static GemICacheSlot _ic_374 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t2772, "rest_param", &_ic_374), GEM_NIL))) {
#line 863 "compiler/main.gem"
    GemVal _t2773 = gem_v_node;
    static GemICacheSlot _ic_375 = {0};
    GemVal _t2774[] = {gem_v_inner_defined, gem_table_get_cached(_t2773, "rest_param", &_ic_375)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t2774, 2));
    }
#line 865 "compiler/main.gem"
    GemVal _t2775 = gem_v_node;
    static GemICacheSlot _ic_376 = {0};
    GemVal _t2776[] = {gem_table_get_cached(_t2775, "body", &_ic_376), gem_v_inner_defined};
    GemVal _t2777 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2777.fn(_t2777.env, _t2776, 2);
#line 866 "compiler/main.gem"
    GemVal _t2778 = gem_v_node;
    static GemICacheSlot _ic_377 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t2778, "defaults", &_ic_377), GEM_NIL))) {
#line 867 "compiler/main.gem"
    GemVal _t2779 = gem_v_node;
    static GemICacheSlot _ic_378 = {0};
        GemVal gem_v__for_items_53 = gem_table_get_cached(_t2779, "defaults", &_ic_378);
#line 867 "compiler/main.gem"
        GemVal gem_v__for_i_53 = gem_int(0);
#line 867 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t2780[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2780, 1)))) break;
#line 867 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 867 "compiler/main.gem"
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 868 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 869 "compiler/main.gem"
    GemVal _t2781[] = {gem_v_d, gem_v_inner_defined, gem_v_free};
    GemVal _t2782 = (*gem_v_collect_free_node);
                (void)(_t2782.fn(_t2782.env, _t2781, 3));
            }
        }

    }
#line 873 "compiler/main.gem"
    GemVal _t2783[] = {gem_v_free};
    GemVal _t2784[] = {gem_keys_fn(NULL, _t2783, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2784, 1);
#line 875 "compiler/main.gem"
    GemVal _t2785[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2785, 1), gem_int(0)))) {
#line 876 "compiler/main.gem"
    GemVal _t2786 = gem_table_new();
    GemVal _t2787[] = {gem_v_node, _t2786};
    GemVal _t2788 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2788.fn(_t2788.env, _t2787, 2);
#line 877 "compiler/main.gem"
    GemVal _t2789 = gem_table_new();
    GemVal _t2790 = gem_v_result;
    static GemICacheSlot _ic_379 = {0};
    GemVal _t2791[] = {gem_string("gem_make_fn("), gem_table_get_cached(_t2790, "fn_name", &_ic_379), gem_string(", NULL)")};
    gem_table_set(_t2789, gem_string("expr"), gem_interp(3, _t2791));
    gem_table_set(_t2789, gem_string("setup"), gem_string(""));
        GemVal _t2792 = _t2789;
        gem_pop_frame();
        return _t2792;
    }
#line 880 "compiler/main.gem"
    GemVal _t2793[] = {gem_v_node, gem_v_captures};
    GemVal _t2794 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2794.fn(_t2794.env, _t2793, 2);
#line 882 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 883 "compiler/main.gem"
    GemVal _t2795 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2795.fn(_t2795.env, NULL, 0);
#line 884 "compiler/main.gem"
    GemVal _t2796 = gem_v_result;
    static GemICacheSlot _ic_380 = {0};
    GemVal _t2797 = gem_v_result;
    static GemICacheSlot _ic_381 = {0};
    GemVal _t2798[] = {gem_string("    struct "), gem_table_get_cached(_t2796, "struct_name", &_ic_380), gem_string(" *"), gem_v_env_tmp, gem_string(" = GC_MALLOC(sizeof(struct "), gem_table_get_cached(_t2797, "struct_name", &_ic_381), gem_string("));\n")};
    GemVal _t2799[] = {gem_v_b, gem_interp(7, _t2798)};
    (void)(gem_buf_push_fn(NULL, _t2799, 2));
#line 885 "compiler/main.gem"
    GemVal gem_v__for_items_54 = gem_v_captures;
#line 885 "compiler/main.gem"
    GemVal gem_v__for_i_54 = gem_int(0);
#line 885 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2800[] = {gem_v__for_items_54};
        if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2800, 1)))) break;
#line 885 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
#line 885 "compiler/main.gem"
        gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 886 "compiler/main.gem"
    GemVal _t2801[] = {gem_v_cap};
    GemVal _t2802 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2802.fn(_t2802.env, _t2801, 1);
#line 887 "compiler/main.gem"
    GemVal _t2803[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2803, 2))) {
#line 888 "compiler/main.gem"
    GemVal _t2804[] = {gem_string("    "), gem_v_env_tmp, gem_string("->"), gem_v_mc, gem_string(" = "), gem_v_mc, gem_string(";\n")};
    GemVal _t2805[] = {gem_v_b, gem_interp(7, _t2804)};
            (void)(gem_buf_push_fn(NULL, _t2805, 2));
        } else {
#line 890 "compiler/main.gem"
    GemVal _t2806[] = {gem_string("    "), gem_v_env_tmp, gem_string("->"), gem_v_mc, gem_string(" = &"), gem_v_mc, gem_string(";\n")};
    GemVal _t2807[] = {gem_v_b, gem_interp(7, _t2806)};
            (void)(gem_buf_push_fn(NULL, _t2807, 2));
        }
    }

    GemVal _t2808 = gem_table_new();
    GemVal _t2809 = gem_v_result;
    static GemICacheSlot _ic_382 = {0};
    GemVal _t2810[] = {gem_string("gem_make_fn("), gem_table_get_cached(_t2809, "fn_name", &_ic_382), gem_string(", "), gem_v_env_tmp, gem_string(")")};
    gem_table_set(_t2808, gem_string("expr"), gem_interp(5, _t2810));
    GemVal _t2811[] = {gem_v_b};
    gem_table_set(_t2808, gem_string("setup"), gem_buf_str_fn(NULL, _t2811, 1));
    GemVal _t2812 = _t2808;
    gem_pop_frame();
    return _t2812;
}

struct _closure__anon_40 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    struct _closure__anon_40 *_cls = (struct _closure__anon_40 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 900 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 901 "compiler/main.gem"
    GemVal _t2814 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2814;
#line 902 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 902 "compiler/main.gem"
    GemVal _t2815[] = {gem_v_args};
    GemVal gem_v__for_limit_55 = gem_len_fn(NULL, _t2815, 1);
#line 902 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 902 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_55;
#line 902 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 903 "compiler/main.gem"
    GemVal _t2816[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2817 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2817.fn(_t2817.env, _t2816, 1);
#line 904 "compiler/main.gem"
    GemVal _t2818 = gem_v_r;
    static GemICacheSlot _ic_383 = {0};
    GemVal _t2819[] = {gem_v_b, gem_table_get_cached(_t2818, "setup", &_ic_383)};
        (void)(gem_buf_push_fn(NULL, _t2819, 2));
#line 905 "compiler/main.gem"
    GemVal _t2820 = gem_v_r;
    static GemICacheSlot _ic_384 = {0};
    GemVal _t2821[] = {gem_v_arg_exprs, gem_table_get_cached(_t2820, "expr", &_ic_384)};
        (void)(gem_push_fn(NULL, _t2821, 2));
    }

#line 907 "compiler/main.gem"
    GemVal _t2822[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2822, 1);
#line 908 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 909 "compiler/main.gem"
    GemVal _t2823 = gem_table_new();
    GemVal _t2824[] = {gem_v_b};
    gem_table_set(_t2823, gem_string("setup"), gem_buf_str_fn(NULL, _t2824, 1));
    gem_table_set(_t2823, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2823, gem_string("argc"), gem_int(0));
        GemVal _t2825 = _t2823;
        gem_pop_frame();
        return _t2825;
    }
#line 911 "compiler/main.gem"
    GemVal _t2826 = (*gem_v_tmp);
    GemVal gem_v_t = _t2826.fn(_t2826.env, NULL, 0);
#line 912 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 913 "compiler/main.gem"
    GemVal gem_v__for_i_56 = gem_int(1);
#line 913 "compiler/main.gem"
    GemVal gem_v__for_limit_56 = gem_v_argc;
#line 913 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 913 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_56;
#line 913 "compiler/main.gem"
        gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 914 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2827 = gem_table_new();
    GemVal _t2828[] = {gem_v_b};
    GemVal _t2829[] = {gem_string("    GemVal "), gem_v_t, gem_string("[] = {"), gem_v_arr, gem_string("};\n")};
    gem_table_set(_t2827, gem_string("setup"), gem_add(gem_buf_str_fn(NULL, _t2828, 1), gem_interp(5, _t2829)));
    gem_table_set(_t2827, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2827, gem_string("argc"), gem_v_argc);
    GemVal _t2830 = _t2827;
    gem_pop_frame();
    return _t2830;
}

struct _closure__anon_41 {
};
static GemVal _anon_41(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_41", "compiler/main.gem", 0);
#line 921 "compiler/main.gem"
    GemVal _t2832 = gem_v_ca;
    static GemICacheSlot _ic_385 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2832, "argc", &_ic_385), gem_int(0)))) {
#line 922 "compiler/main.gem"
    GemVal _t2833 = gem_table_new();
    GemVal _t2834[] = {gem_v_fn_name, gem_string("("), gem_v_env, gem_string(", NULL, 0)")};
    gem_table_set(_t2833, gem_string("expr"), gem_interp(4, _t2834));
    GemVal _t2835 = gem_v_ca;
    static GemICacheSlot _ic_386 = {0};
    gem_table_set(_t2833, gem_string("setup"), gem_table_get_cached(_t2835, "setup", &_ic_386));
        GemVal _t2836 = _t2833;
        gem_pop_frame();
        return _t2836;
    }
    GemVal _t2837 = gem_table_new();
    GemVal _t2838 = gem_v_ca;
    static GemICacheSlot _ic_387 = {0};
    GemVal _t2839 = gem_v_ca;
    static GemICacheSlot _ic_388 = {0};
    GemVal _t2840[] = {gem_v_fn_name, gem_string("("), gem_v_env, gem_string(", "), gem_table_get_cached(_t2838, "arr_name", &_ic_387), gem_string(", "), gem_table_get_cached(_t2839, "argc", &_ic_388), gem_string(")")};
    gem_table_set(_t2837, gem_string("expr"), gem_interp(8, _t2840));
    GemVal _t2841 = gem_v_ca;
    static GemICacheSlot _ic_389 = {0};
    gem_table_set(_t2837, gem_string("setup"), gem_table_get_cached(_t2841, "setup", &_ic_389));
    GemVal _t2842 = _t2837;
    gem_pop_frame();
    return _t2842;
}

struct _closure__anon_42 {
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_args;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call;
    GemVal *gem_v_local_names;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_42(void *_env, GemVal *args, int argc) {
    struct _closure__anon_42 *_cls = (struct _closure__anon_42 *)_env;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_args = _cls->gem_v_compile_args;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call = _cls->gem_v_emit_direct_call;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_42", "compiler/main.gem", 0);
#line 930 "compiler/main.gem"
    GemVal gem_v__d57 = gem_v_node;
#line 930 "compiler/main.gem"
    GemVal _t2843 = gem_v__d57;
    static GemICacheSlot _ic_390 = {0};
    GemVal gem_v_func = gem_table_get_cached(_t2843, "func", &_ic_390);
#line 930 "compiler/main.gem"
    GemVal _t2844 = gem_v__d57;
    static GemICacheSlot _ic_391 = {0};
    GemVal gem_v_args = gem_table_get_cached(_t2844, "args", &_ic_391);

#line 933 "compiler/main.gem"
    GemVal _t2845 = gem_v_func;
    static GemICacheSlot _ic_392 = {0};
    GemVal _t2847;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t2845, "tag", &_ic_392), gem_string("dot")))) {
        _t2847 = gem_eq(gem_table_get_cached(_t2845, "tag", &_ic_392), gem_string("dot"));
    } else {
        GemVal _t2846 = gem_v_func;
        static GemICacheSlot _ic_393 = {0};
        _t2847 = gem_eq(gem_table_get_cached(_t2846, "field", &_ic_393), gem_string("len"));
    }
    GemVal _t2849;
    if (!gem_truthy(_t2847)) {
        _t2849 = _t2847;
    } else {
        GemVal _t2848[] = {gem_v_args};
        _t2849 = gem_eq(gem_len_fn(NULL, _t2848, 1), gem_int(0));
    }
    if (gem_truthy(_t2849)) {
#line 934 "compiler/main.gem"
    GemVal _t2850 = gem_v_func;
    static GemICacheSlot _ic_394 = {0};
    GemVal _t2851[] = {gem_table_get_cached(_t2850, "object", &_ic_394)};
    GemVal _t2852 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2852.fn(_t2852.env, _t2851, 1);
#line 935 "compiler/main.gem"
    GemVal _t2853 = gem_table_new();
    GemVal _t2854 = gem_v_obj_r;
    static GemICacheSlot _ic_395 = {0};
    GemVal _t2855[] = {gem_string("gem_len_val("), gem_table_get_cached(_t2854, "expr", &_ic_395), gem_string(")")};
    gem_table_set(_t2853, gem_string("expr"), gem_interp(3, _t2855));
    GemVal _t2856 = gem_v_obj_r;
    static GemICacheSlot _ic_396 = {0};
    gem_table_set(_t2853, gem_string("setup"), gem_table_get_cached(_t2856, "setup", &_ic_396));
        GemVal _t2857 = _t2853;
        gem_pop_frame();
        return _t2857;
    }
#line 939 "compiler/main.gem"
    GemVal _t2858 = gem_v_func;
    static GemICacheSlot _ic_397 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2858, "tag", &_ic_397), gem_string("var")))) {
#line 940 "compiler/main.gem"
    GemVal _t2859 = gem_v_func;
    static GemICacheSlot _ic_398 = {0};
        GemVal gem_v_name = gem_table_get_cached(_t2859, "name", &_ic_398);
#line 942 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 943 "compiler/main.gem"
    GemVal _t2860 = gem_v_node;
    static GemICacheSlot _ic_399 = {0};
            GemVal gem_v_line = gem_table_get_cached(_t2860, "line", &_ic_399);
#line 944 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 945 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 947 "compiler/main.gem"
    GemVal _t2861[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn__mod_codegen_escape_c_string(NULL, _t2861, 1);
#line 948 "compiler/main.gem"
    GemVal _t2862[] = {gem_v_args};
    GemVal _t2863 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2863.fn(_t2863.env, _t2862, 1);
#line 949 "compiler/main.gem"
    GemVal _t2864 = gem_v_ca;
    static GemICacheSlot _ic_400 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t2864, "argc", &_ic_400), gem_int(0)))) {
#line 950 "compiler/main.gem"
    GemVal _t2865 = gem_table_new();
    GemVal _t2866[] = {gem_string("gem_error_at_fn(\""), gem_v_escaped_file, gem_string("\", "), gem_v_line, gem_string(", NULL, 0)")};
    gem_table_set(_t2865, gem_string("expr"), gem_interp(5, _t2866));
    GemVal _t2867 = gem_v_ca;
    static GemICacheSlot _ic_401 = {0};
    gem_table_set(_t2865, gem_string("setup"), gem_table_get_cached(_t2867, "setup", &_ic_401));
                GemVal _t2868 = _t2865;
                gem_pop_frame();
                return _t2868;
            }
#line 952 "compiler/main.gem"
    GemVal _t2869 = gem_table_new();
    GemVal _t2870 = gem_v_ca;
    static GemICacheSlot _ic_402 = {0};
    GemVal _t2871 = gem_v_ca;
    static GemICacheSlot _ic_403 = {0};
    GemVal _t2872[] = {gem_string("gem_error_at_fn(\""), gem_v_escaped_file, gem_string("\", "), gem_v_line, gem_string(", "), gem_table_get_cached(_t2870, "arr_name", &_ic_402), gem_string(", "), gem_table_get_cached(_t2871, "argc", &_ic_403), gem_string(")")};
    gem_table_set(_t2869, gem_string("expr"), gem_interp(9, _t2872));
    GemVal _t2873 = gem_v_ca;
    static GemICacheSlot _ic_404 = {0};
    gem_table_set(_t2869, gem_string("setup"), gem_table_get_cached(_t2873, "setup", &_ic_404));
            GemVal _t2874 = _t2869;
            gem_pop_frame();
            return _t2874;
        } else {
#line 953 "compiler/main.gem"
    GemVal _t2875[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2877;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2875, 2))) {
        _t2877 = gem_has_key_fn(NULL, _t2875, 2);
    } else {
        GemVal _t2876[] = {(*gem_v_local_names), gem_v_name};
        _t2877 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2876, 2));
    }
            if (gem_truthy(_t2877)) {
#line 954 "compiler/main.gem"
    GemVal _t2878[] = {gem_v_args};
    GemVal _t2879 = (*gem_v_compile_args);
    GemVal _t2880[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2879.fn(_t2879.env, _t2878, 1)};
    GemVal _t2881 = (*gem_v_emit_direct_call);
                GemVal _t2882 = _t2881.fn(_t2881.env, _t2880, 3);
                gem_pop_frame();
                return _t2882;
            } else {
#line 955 "compiler/main.gem"
    GemVal _t2883[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2885;
    if (!gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2883, 2))) {
        _t2885 = gem_fn__mod_codegen_set_contains(NULL, _t2883, 2);
    } else {
        GemVal _t2884[] = {(*gem_v_local_names), gem_v_name};
        _t2885 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2884, 2));
    }
                if (gem_truthy(_t2885)) {
#line 956 "compiler/main.gem"
    GemVal _t2886[] = {gem_string("gem_fn_"), gem_v_name};
    GemVal _t2887[] = {gem_v_args};
    GemVal _t2888 = (*gem_v_compile_args);
    GemVal _t2889[] = {gem_interp(2, _t2886), gem_string("NULL"), _t2888.fn(_t2888.env, _t2887, 1)};
    GemVal _t2890 = (*gem_v_emit_direct_call);
                    GemVal _t2891 = _t2890.fn(_t2890.env, _t2889, 3);
                    gem_pop_frame();
                    return _t2891;
                }
            }
        }
    }
#line 961 "compiler/main.gem"
    GemVal _t2892[] = {gem_v_func};
    GemVal _t2893 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2893.fn(_t2893.env, _t2892, 1);
#line 962 "compiler/main.gem"
    GemVal _t2894[] = {gem_v_args};
    GemVal _t2895 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2895.fn(_t2895.env, _t2894, 1);
#line 963 "compiler/main.gem"
    GemVal _t2896 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2896.fn(_t2896.env, NULL, 0);
#line 964 "compiler/main.gem"
    GemVal _t2897 = gem_v_func_r;
    static GemICacheSlot _ic_405 = {0};
    GemVal _t2898 = gem_v_ca;
    static GemICacheSlot _ic_406 = {0};
    GemVal _t2899 = gem_v_func_r;
    static GemICacheSlot _ic_407 = {0};
    GemVal _t2900[] = {gem_string("    GemVal "), gem_v_tmp_fn, gem_string(" = "), gem_table_get_cached(_t2899, "expr", &_ic_407), gem_string(";\n")};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get_cached(_t2897, "setup", &_ic_405), gem_table_get_cached(_t2898, "setup", &_ic_406)), gem_interp(5, _t2900));
#line 965 "compiler/main.gem"
    GemVal _t2901 = gem_v_ca;
    static GemICacheSlot _ic_408 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2901, "argc", &_ic_408), gem_int(0)))) {
#line 966 "compiler/main.gem"
    GemVal _t2902 = gem_table_new();
    GemVal _t2903[] = {gem_v_tmp_fn, gem_string(".fn("), gem_v_tmp_fn, gem_string(".env, NULL, 0)")};
    gem_table_set(_t2902, gem_string("expr"), gem_interp(4, _t2903));
    gem_table_set(_t2902, gem_string("setup"), gem_v_setup);
        GemVal _t2904 = _t2902;
        gem_pop_frame();
        return _t2904;
    }
    GemVal _t2905 = gem_table_new();
    GemVal _t2906 = gem_v_ca;
    static GemICacheSlot _ic_409 = {0};
    GemVal _t2907 = gem_v_ca;
    static GemICacheSlot _ic_410 = {0};
    GemVal _t2908[] = {gem_v_tmp_fn, gem_string(".fn("), gem_v_tmp_fn, gem_string(".env, "), gem_table_get_cached(_t2906, "arr_name", &_ic_409), gem_string(", "), gem_table_get_cached(_t2907, "argc", &_ic_410), gem_string(")")};
    gem_table_set(_t2905, gem_string("expr"), gem_interp(8, _t2908));
    gem_table_set(_t2905, gem_string("setup"), gem_v_setup);
    GemVal _t2909 = _t2905;
    gem_pop_frame();
    return _t2909;
}

struct _closure__anon_43 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_43(void *_env, GemVal *args, int argc) {
    struct _closure__anon_43 *_cls = (struct _closure__anon_43 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_43", "compiler/main.gem", 0);
#line 974 "compiler/main.gem"
    GemVal _t2911 = gem_v_node;
    static GemICacheSlot _ic_411 = {0};
    GemVal gem_v_op = gem_table_get_cached(_t2911, "op", &_ic_411);
#line 975 "compiler/main.gem"
    GemVal _t2912 = gem_v_node;
    static GemICacheSlot _ic_412 = {0};
    GemVal _t2913[] = {gem_table_get_cached(_t2912, "left", &_ic_412)};
    GemVal _t2914 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2914.fn(_t2914.env, _t2913, 1);
#line 976 "compiler/main.gem"
    GemVal _t2915 = gem_v_lc_r;
    static GemICacheSlot _ic_413 = {0};
    GemVal gem_v_lc = gem_table_get_cached(_t2915, "expr", &_ic_413);
#line 977 "compiler/main.gem"
    GemVal _t2916 = gem_v_lc_r;
    static GemICacheSlot _ic_414 = {0};
    GemVal gem_v_ls = gem_table_get_cached(_t2916, "setup", &_ic_414);
#line 979 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 980 "compiler/main.gem"
    GemVal _t2917 = gem_v_node;
    static GemICacheSlot _ic_415 = {0};
    GemVal _t2918[] = {gem_table_get_cached(_t2917, "right", &_ic_415)};
    GemVal _t2919 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2919.fn(_t2919.env, _t2918, 1);
#line 981 "compiler/main.gem"
    GemVal _t2920 = gem_table_new();
    GemVal _t2921 = gem_v_rc_r;
    static GemICacheSlot _ic_416 = {0};
    GemVal _t2922[] = {gem_string("gem_add("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2921, "expr", &_ic_416), gem_string(")")};
    gem_table_set(_t2920, gem_string("expr"), gem_interp(5, _t2922));
    GemVal _t2923 = gem_v_rc_r;
    static GemICacheSlot _ic_417 = {0};
    gem_table_set(_t2920, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2923, "setup", &_ic_417)));
        GemVal _t2924 = _t2920;
        gem_pop_frame();
        return _t2924;
    } else {
#line 982 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 983 "compiler/main.gem"
    GemVal _t2925 = gem_v_node;
    static GemICacheSlot _ic_418 = {0};
    GemVal _t2926[] = {gem_table_get_cached(_t2925, "right", &_ic_418)};
    GemVal _t2927 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2927.fn(_t2927.env, _t2926, 1);
#line 984 "compiler/main.gem"
    GemVal _t2928 = gem_table_new();
    GemVal _t2929 = gem_v_rc_r;
    static GemICacheSlot _ic_419 = {0};
    GemVal _t2930[] = {gem_string("gem_sub("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2929, "expr", &_ic_419), gem_string(")")};
    gem_table_set(_t2928, gem_string("expr"), gem_interp(5, _t2930));
    GemVal _t2931 = gem_v_rc_r;
    static GemICacheSlot _ic_420 = {0};
    gem_table_set(_t2928, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2931, "setup", &_ic_420)));
            GemVal _t2932 = _t2928;
            gem_pop_frame();
            return _t2932;
        } else {
#line 985 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 986 "compiler/main.gem"
    GemVal _t2933 = gem_v_node;
    static GemICacheSlot _ic_421 = {0};
    GemVal _t2934[] = {gem_table_get_cached(_t2933, "right", &_ic_421)};
    GemVal _t2935 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2935.fn(_t2935.env, _t2934, 1);
#line 987 "compiler/main.gem"
    GemVal _t2936 = gem_table_new();
    GemVal _t2937 = gem_v_rc_r;
    static GemICacheSlot _ic_422 = {0};
    GemVal _t2938[] = {gem_string("gem_mul("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2937, "expr", &_ic_422), gem_string(")")};
    gem_table_set(_t2936, gem_string("expr"), gem_interp(5, _t2938));
    GemVal _t2939 = gem_v_rc_r;
    static GemICacheSlot _ic_423 = {0};
    gem_table_set(_t2936, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2939, "setup", &_ic_423)));
                GemVal _t2940 = _t2936;
                gem_pop_frame();
                return _t2940;
            } else {
#line 988 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 989 "compiler/main.gem"
    GemVal _t2941 = gem_v_node;
    static GemICacheSlot _ic_424 = {0};
    GemVal _t2942[] = {gem_table_get_cached(_t2941, "right", &_ic_424)};
    GemVal _t2943 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2943.fn(_t2943.env, _t2942, 1);
#line 990 "compiler/main.gem"
    GemVal _t2944 = gem_table_new();
    GemVal _t2945 = gem_v_rc_r;
    static GemICacheSlot _ic_425 = {0};
    GemVal _t2946[] = {gem_string("gem_div("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2945, "expr", &_ic_425), gem_string(")")};
    gem_table_set(_t2944, gem_string("expr"), gem_interp(5, _t2946));
    GemVal _t2947 = gem_v_rc_r;
    static GemICacheSlot _ic_426 = {0};
    gem_table_set(_t2944, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2947, "setup", &_ic_426)));
                    GemVal _t2948 = _t2944;
                    gem_pop_frame();
                    return _t2948;
                } else {
#line 991 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 992 "compiler/main.gem"
    GemVal _t2949 = gem_v_node;
    static GemICacheSlot _ic_427 = {0};
    GemVal _t2950[] = {gem_table_get_cached(_t2949, "right", &_ic_427)};
    GemVal _t2951 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2951.fn(_t2951.env, _t2950, 1);
#line 993 "compiler/main.gem"
    GemVal _t2952 = gem_table_new();
    GemVal _t2953 = gem_v_rc_r;
    static GemICacheSlot _ic_428 = {0};
    GemVal _t2954[] = {gem_string("gem_mod("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2953, "expr", &_ic_428), gem_string(")")};
    gem_table_set(_t2952, gem_string("expr"), gem_interp(5, _t2954));
    GemVal _t2955 = gem_v_rc_r;
    static GemICacheSlot _ic_429 = {0};
    gem_table_set(_t2952, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2955, "setup", &_ic_429)));
                        GemVal _t2956 = _t2952;
                        gem_pop_frame();
                        return _t2956;
                    } else {
#line 994 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 995 "compiler/main.gem"
    GemVal _t2957 = gem_v_node;
    static GemICacheSlot _ic_430 = {0};
    GemVal _t2958[] = {gem_table_get_cached(_t2957, "right", &_ic_430)};
    GemVal _t2959 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2959.fn(_t2959.env, _t2958, 1);
#line 996 "compiler/main.gem"
    GemVal _t2960 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2960.fn(_t2960.env, NULL, 0);
#line 997 "compiler/main.gem"
    GemVal _t2961 = gem_v_rc_r;
    static GemICacheSlot _ic_431 = {0};
    GemVal _t2962 = gem_v_rc_r;
    static GemICacheSlot _ic_432 = {0};
    GemVal _t2963[] = {gem_string("    GemVal "), gem_v_t, gem_string("[] = {"), gem_table_get_cached(_t2962, "expr", &_ic_432), gem_string(", "), gem_v_lc, gem_string("};\n")};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get_cached(_t2961, "setup", &_ic_431)), gem_interp(7, _t2963));
#line 998 "compiler/main.gem"
    GemVal _t2964 = gem_table_new();
    GemVal _t2965[] = {gem_string("gem_in_fn(NULL, "), gem_v_t, gem_string(", 2)")};
    gem_table_set(_t2964, gem_string("expr"), gem_interp(3, _t2965));
    gem_table_set(_t2964, gem_string("setup"), gem_v_setup);
                            GemVal _t2966 = _t2964;
                            gem_pop_frame();
                            return _t2966;
                        } else {
#line 999 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 1000 "compiler/main.gem"
    GemVal _t2967 = gem_v_node;
    static GemICacheSlot _ic_433 = {0};
    GemVal _t2968[] = {gem_table_get_cached(_t2967, "right", &_ic_433)};
    GemVal _t2969 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2969.fn(_t2969.env, _t2968, 1);
#line 1001 "compiler/main.gem"
    GemVal _t2970 = gem_table_new();
    GemVal _t2971 = gem_v_rc_r;
    static GemICacheSlot _ic_434 = {0};
    GemVal _t2972[] = {gem_string("gem_eq("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2971, "expr", &_ic_434), gem_string(")")};
    gem_table_set(_t2970, gem_string("expr"), gem_interp(5, _t2972));
    GemVal _t2973 = gem_v_rc_r;
    static GemICacheSlot _ic_435 = {0};
    gem_table_set(_t2970, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2973, "setup", &_ic_435)));
                                GemVal _t2974 = _t2970;
                                gem_pop_frame();
                                return _t2974;
                            } else {
#line 1002 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 1003 "compiler/main.gem"
    GemVal _t2975 = gem_v_node;
    static GemICacheSlot _ic_436 = {0};
    GemVal _t2976[] = {gem_table_get_cached(_t2975, "right", &_ic_436)};
    GemVal _t2977 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2977.fn(_t2977.env, _t2976, 1);
#line 1004 "compiler/main.gem"
    GemVal _t2978 = gem_table_new();
    GemVal _t2979 = gem_v_rc_r;
    static GemICacheSlot _ic_437 = {0};
    GemVal _t2980[] = {gem_string("gem_neq("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2979, "expr", &_ic_437), gem_string(")")};
    gem_table_set(_t2978, gem_string("expr"), gem_interp(5, _t2980));
    GemVal _t2981 = gem_v_rc_r;
    static GemICacheSlot _ic_438 = {0};
    gem_table_set(_t2978, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2981, "setup", &_ic_438)));
                                    GemVal _t2982 = _t2978;
                                    gem_pop_frame();
                                    return _t2982;
                                } else {
#line 1005 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 1006 "compiler/main.gem"
    GemVal _t2983 = gem_v_node;
    static GemICacheSlot _ic_439 = {0};
    GemVal _t2984[] = {gem_table_get_cached(_t2983, "right", &_ic_439)};
    GemVal _t2985 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2985.fn(_t2985.env, _t2984, 1);
#line 1007 "compiler/main.gem"
    GemVal _t2986 = gem_table_new();
    GemVal _t2987 = gem_v_rc_r;
    static GemICacheSlot _ic_440 = {0};
    GemVal _t2988[] = {gem_string("gem_lt("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2987, "expr", &_ic_440), gem_string(")")};
    gem_table_set(_t2986, gem_string("expr"), gem_interp(5, _t2988));
    GemVal _t2989 = gem_v_rc_r;
    static GemICacheSlot _ic_441 = {0};
    gem_table_set(_t2986, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2989, "setup", &_ic_441)));
                                        GemVal _t2990 = _t2986;
                                        gem_pop_frame();
                                        return _t2990;
                                    } else {
#line 1008 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 1009 "compiler/main.gem"
    GemVal _t2991 = gem_v_node;
    static GemICacheSlot _ic_442 = {0};
    GemVal _t2992[] = {gem_table_get_cached(_t2991, "right", &_ic_442)};
    GemVal _t2993 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2993.fn(_t2993.env, _t2992, 1);
#line 1010 "compiler/main.gem"
    GemVal _t2994 = gem_table_new();
    GemVal _t2995 = gem_v_rc_r;
    static GemICacheSlot _ic_443 = {0};
    GemVal _t2996[] = {gem_string("gem_gt("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2995, "expr", &_ic_443), gem_string(")")};
    gem_table_set(_t2994, gem_string("expr"), gem_interp(5, _t2996));
    GemVal _t2997 = gem_v_rc_r;
    static GemICacheSlot _ic_444 = {0};
    gem_table_set(_t2994, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2997, "setup", &_ic_444)));
                                            GemVal _t2998 = _t2994;
                                            gem_pop_frame();
                                            return _t2998;
                                        } else {
#line 1011 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 1012 "compiler/main.gem"
    GemVal _t2999 = gem_v_node;
    static GemICacheSlot _ic_445 = {0};
    GemVal _t3000[] = {gem_table_get_cached(_t2999, "right", &_ic_445)};
    GemVal _t3001 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t3001.fn(_t3001.env, _t3000, 1);
#line 1013 "compiler/main.gem"
    GemVal _t3002 = gem_table_new();
    GemVal _t3003 = gem_v_rc_r;
    static GemICacheSlot _ic_446 = {0};
    GemVal _t3004[] = {gem_string("gem_le("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t3003, "expr", &_ic_446), gem_string(")")};
    gem_table_set(_t3002, gem_string("expr"), gem_interp(5, _t3004));
    GemVal _t3005 = gem_v_rc_r;
    static GemICacheSlot _ic_447 = {0};
    gem_table_set(_t3002, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t3005, "setup", &_ic_447)));
                                                GemVal _t3006 = _t3002;
                                                gem_pop_frame();
                                                return _t3006;
                                            } else {
#line 1014 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 1015 "compiler/main.gem"
    GemVal _t3007 = gem_v_node;
    static GemICacheSlot _ic_448 = {0};
    GemVal _t3008[] = {gem_table_get_cached(_t3007, "right", &_ic_448)};
    GemVal _t3009 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t3009.fn(_t3009.env, _t3008, 1);
#line 1016 "compiler/main.gem"
    GemVal _t3010 = gem_table_new();
    GemVal _t3011 = gem_v_rc_r;
    static GemICacheSlot _ic_449 = {0};
    GemVal _t3012[] = {gem_string("gem_ge("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t3011, "expr", &_ic_449), gem_string(")")};
    gem_table_set(_t3010, gem_string("expr"), gem_interp(5, _t3012));
    GemVal _t3013 = gem_v_rc_r;
    static GemICacheSlot _ic_450 = {0};
    gem_table_set(_t3010, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t3013, "setup", &_ic_450)));
                                                    GemVal _t3014 = _t3010;
                                                    gem_pop_frame();
                                                    return _t3014;
                                                } else {
#line 1017 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 1018 "compiler/main.gem"
    GemVal _t3015 = gem_v_node;
    static GemICacheSlot _ic_451 = {0};
    GemVal _t3016[] = {gem_table_get_cached(_t3015, "right", &_ic_451)};
    GemVal _t3017 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t3017.fn(_t3017.env, _t3016, 1);
#line 1019 "compiler/main.gem"
    GemVal _t3018 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t3018.fn(_t3018.env, NULL, 0);
#line 1020 "compiler/main.gem"
                                                        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1021 "compiler/main.gem"
    GemVal _t3019[] = {gem_v_b, gem_v_ls};
                                                        (void)(gem_buf_push_fn(NULL, _t3019, 2));
#line 1022 "compiler/main.gem"
    GemVal _t3020[] = {gem_string("    GemVal "), gem_v_t, gem_string(";\n")};
    GemVal _t3021[] = {gem_v_b, gem_interp(3, _t3020)};
                                                        (void)(gem_buf_push_fn(NULL, _t3021, 2));
#line 1023 "compiler/main.gem"
    GemVal _t3022[] = {gem_string("    if (!gem_truthy("), gem_v_lc, gem_string(")) {\n")};
    GemVal _t3023[] = {gem_v_b, gem_interp(3, _t3022)};
                                                        (void)(gem_buf_push_fn(NULL, _t3023, 2));
#line 1024 "compiler/main.gem"
    GemVal _t3024[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_v_lc, gem_string(";\n")};
    GemVal _t3025[] = {gem_v_b, gem_interp(5, _t3024)};
                                                        (void)(gem_buf_push_fn(NULL, _t3025, 2));
#line 1025 "compiler/main.gem"
    GemVal _t3026[] = {gem_v_b, gem_string("    } else {\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3026, 2));
#line 1026 "compiler/main.gem"
    GemVal _t3027 = gem_v_rc_r;
    static GemICacheSlot _ic_452 = {0};
                                                        if (gem_truthy(gem_neq(gem_table_get_cached(_t3027, "setup", &_ic_452), gem_string("")))) {
#line 1027 "compiler/main.gem"
    GemVal _t3028 = gem_v_rc_r;
    static GemICacheSlot _ic_453 = {0};
    GemVal _t3029[] = {gem_table_get_cached(_t3028, "setup", &_ic_453), gem_string("    "), gem_string("        ")};
    GemVal _t3030[] = {gem_v_b, gem_str_replace_fn(NULL, _t3029, 3)};
                                                            (void)(gem_buf_push_fn(NULL, _t3030, 2));
                                                        }
#line 1029 "compiler/main.gem"
    GemVal _t3031 = gem_v_rc_r;
    static GemICacheSlot _ic_454 = {0};
    GemVal _t3032[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3031, "expr", &_ic_454), gem_string(";\n")};
    GemVal _t3033[] = {gem_v_b, gem_interp(5, _t3032)};
                                                        (void)(gem_buf_push_fn(NULL, _t3033, 2));
#line 1030 "compiler/main.gem"
    GemVal _t3034[] = {gem_v_b, gem_string("    }\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3034, 2));
#line 1031 "compiler/main.gem"
    GemVal _t3035 = gem_table_new();
    gem_table_set(_t3035, gem_string("expr"), gem_v_t);
    GemVal _t3036[] = {gem_v_b};
    gem_table_set(_t3035, gem_string("setup"), gem_buf_str_fn(NULL, _t3036, 1));
                                                        GemVal _t3037 = _t3035;
                                                        gem_pop_frame();
                                                        return _t3037;
                                                    } else {
#line 1032 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 1033 "compiler/main.gem"
    GemVal _t3038 = gem_v_node;
    static GemICacheSlot _ic_455 = {0};
    GemVal _t3039[] = {gem_table_get_cached(_t3038, "right", &_ic_455)};
    GemVal _t3040 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t3040.fn(_t3040.env, _t3039, 1);
#line 1034 "compiler/main.gem"
    GemVal _t3041 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t3041.fn(_t3041.env, NULL, 0);
#line 1035 "compiler/main.gem"
                                                            GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1036 "compiler/main.gem"
    GemVal _t3042[] = {gem_v_b, gem_v_ls};
                                                            (void)(gem_buf_push_fn(NULL, _t3042, 2));
#line 1037 "compiler/main.gem"
    GemVal _t3043[] = {gem_string("    GemVal "), gem_v_t, gem_string(";\n")};
    GemVal _t3044[] = {gem_v_b, gem_interp(3, _t3043)};
                                                            (void)(gem_buf_push_fn(NULL, _t3044, 2));
#line 1038 "compiler/main.gem"
    GemVal _t3045[] = {gem_string("    if (gem_truthy("), gem_v_lc, gem_string(")) {\n")};
    GemVal _t3046[] = {gem_v_b, gem_interp(3, _t3045)};
                                                            (void)(gem_buf_push_fn(NULL, _t3046, 2));
#line 1039 "compiler/main.gem"
    GemVal _t3047[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_v_lc, gem_string(";\n")};
    GemVal _t3048[] = {gem_v_b, gem_interp(5, _t3047)};
                                                            (void)(gem_buf_push_fn(NULL, _t3048, 2));
#line 1040 "compiler/main.gem"
    GemVal _t3049[] = {gem_v_b, gem_string("    } else {\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3049, 2));
#line 1041 "compiler/main.gem"
    GemVal _t3050 = gem_v_rc_r;
    static GemICacheSlot _ic_456 = {0};
                                                            if (gem_truthy(gem_neq(gem_table_get_cached(_t3050, "setup", &_ic_456), gem_string("")))) {
#line 1042 "compiler/main.gem"
    GemVal _t3051 = gem_v_rc_r;
    static GemICacheSlot _ic_457 = {0};
    GemVal _t3052[] = {gem_table_get_cached(_t3051, "setup", &_ic_457), gem_string("    "), gem_string("        ")};
    GemVal _t3053[] = {gem_v_b, gem_str_replace_fn(NULL, _t3052, 3)};
                                                                (void)(gem_buf_push_fn(NULL, _t3053, 2));
                                                            }
#line 1044 "compiler/main.gem"
    GemVal _t3054 = gem_v_rc_r;
    static GemICacheSlot _ic_458 = {0};
    GemVal _t3055[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3054, "expr", &_ic_458), gem_string(";\n")};
    GemVal _t3056[] = {gem_v_b, gem_interp(5, _t3055)};
                                                            (void)(gem_buf_push_fn(NULL, _t3056, 2));
#line 1045 "compiler/main.gem"
    GemVal _t3057[] = {gem_v_b, gem_string("    }\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3057, 2));
#line 1046 "compiler/main.gem"
    GemVal _t3058 = gem_table_new();
    gem_table_set(_t3058, gem_string("expr"), gem_v_t);
    GemVal _t3059[] = {gem_v_b};
    gem_table_set(_t3058, gem_string("setup"), gem_buf_str_fn(NULL, _t3059, 1));
                                                            GemVal _t3060 = _t3058;
                                                            gem_pop_frame();
                                                            return _t3060;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1049 "compiler/main.gem"
    GemVal _t3061[] = {gem_string("unknown binary operator: "), gem_v_op};
    GemVal _t3062[] = {gem_interp(2, _t3061)};
    GemVal _t3063 = gem_error_at_fn("compiler/main.gem", 1049, _t3062, 1);
    gem_pop_frame();
    return _t3063;
}

struct _closure__anon_44 {
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 1055 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1056 "compiler/main.gem"
    GemVal gem_v__for_items_58 = gem_v_stmts;
#line 1056 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1056 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3065[] = {gem_v__for_items_58};
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t3065, 1)))) break;
#line 1056 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1056 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1057 "compiler/main.gem"
    GemVal _t3066[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3067 = (*gem_v_compile_stmt);
    GemVal _t3068[] = {gem_v_b, _t3067.fn(_t3067.env, _t3066, 2)};
        (void)(gem_buf_push_fn(NULL, _t3068, 2));
#line 1058 "compiler/main.gem"
    GemVal _t3069[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3069, 2));
    }

#line 1060 "compiler/main.gem"
    GemVal _t3070[] = {gem_v_b};
    GemVal _t3071 = gem_buf_str_fn(NULL, _t3070, 1);
    gem_pop_frame();
    return _t3071;
}

struct _closure__anon_45 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    struct _closure__anon_45 *_cls = (struct _closure__anon_45 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 1064 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_return_pos))) {
#line 1065 "compiler/main.gem"
    GemVal _t3073[] = {gem_v_stmts, gem_v_indent};
    GemVal _t3074 = (*gem_v_compile_stmts);
        GemVal _t3075 = _t3074.fn(_t3074.env, _t3073, 2);
        gem_pop_frame();
        return _t3075;
    }
#line 1067 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1068 "compiler/main.gem"
    GemVal _t3076[] = {gem_v_stmts};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3076, 1), gem_int(0)))) {
#line 1069 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1069 "compiler/main.gem"
    GemVal _t3077[] = {gem_v_stmts};
        GemVal gem_v__for_limit_59 = gem_sub(gem_len_fn(NULL, _t3077, 1), gem_int(1));
#line 1069 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1069 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_59;
#line 1069 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1070 "compiler/main.gem"
    GemVal _t3078[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_indent};
    GemVal _t3079 = (*gem_v_compile_stmt);
    GemVal _t3080[] = {gem_v_b, _t3079.fn(_t3079.env, _t3078, 2)};
            (void)(gem_buf_push_fn(NULL, _t3080, 2));
#line 1071 "compiler/main.gem"
    GemVal _t3081[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3081, 2));
        }

#line 1073 "compiler/main.gem"
    GemVal _t3082[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t3083 = (*gem_v_compile_stmt_return);
    GemVal _t3084[] = {gem_v_b, _t3083.fn(_t3083.env, _t3082, 2)};
        (void)(gem_buf_push_fn(NULL, _t3084, 2));
#line 1074 "compiler/main.gem"
    GemVal _t3085[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3085, 2));
    } else {
#line 1076 "compiler/main.gem"
    GemVal _t3086[] = {gem_v_indent};
        GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3086, 1);
#line 1077 "compiler/main.gem"
    GemVal _t3087[] = {gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;\n")};
    GemVal _t3088[] = {gem_v_b, gem_interp(4, _t3087)};
        (void)(gem_buf_push_fn(NULL, _t3088, 2));
    }
#line 1079 "compiler/main.gem"
    GemVal _t3089[] = {gem_v_b};
    GemVal _t3090 = gem_buf_str_fn(NULL, _t3089, 1);
    gem_pop_frame();
    return _t3090;
}

struct _closure__anon_46 {
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
static GemVal _anon_46(void *_env, GemVal *args, int argc) {
    struct _closure__anon_46 *_cls = (struct _closure__anon_46 *)_env;
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
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 1085 "compiler/main.gem"
    GemVal _t3092[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3092, 1);
#line 1086 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1087 "compiler/main.gem"
    GemVal _t3093 = gem_v_node;
    static GemICacheSlot _ic_459 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3093, "line", &_ic_459), GEM_NIL))) {
#line 1088 "compiler/main.gem"
    GemVal _t3094 = gem_v_node;
    static GemICacheSlot _ic_460 = {0};
    GemVal _t3095[] = {(*gem_v_source_name)};
    GemVal _t3096[] = {gem_string("#line "), gem_table_get_cached(_t3094, "line", &_ic_460), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3095, 1), gem_string("\"\n")};
        gem_v_line_dir = gem_interp(5, _t3096);
    }
#line 1091 "compiler/main.gem"
    GemVal gem_v__match_60 = gem_v_node;
    GemVal _t3097[] = {gem_v__match_60};
    GemVal _t3099;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3097, 1), gem_string("table")))) {
        _t3099 = gem_eq(gem_type_fn(NULL, _t3097, 1), gem_string("table"));
    } else {
        GemVal _t3098[] = {gem_v__match_60, gem_string("tag")};
        _t3099 = gem_has_key_fn(NULL, _t3098, 2);
    }
    GemVal _t3100;
    if (!gem_truthy(_t3099)) {
        _t3100 = _t3099;
    } else {
        _t3100 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3102;
    if (!gem_truthy(_t3100)) {
        _t3102 = _t3100;
    } else {
        GemVal _t3101[] = {gem_v__match_60, gem_string("name")};
        _t3102 = gem_has_key_fn(NULL, _t3101, 2);
    }
    GemVal _t3104;
    if (!gem_truthy(_t3102)) {
        _t3104 = _t3102;
    } else {
        GemVal _t3103[] = {gem_v__match_60, gem_string("value")};
        _t3104 = gem_has_key_fn(NULL, _t3103, 2);
    }
    if (gem_truthy(_t3104)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_60, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1093 "compiler/main.gem"
    GemVal _t3105[] = {gem_v_value};
    GemVal _t3106 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3106.fn(_t3106.env, _t3105, 1);
#line 1094 "compiler/main.gem"
    GemVal _t3107[] = {gem_v_name};
    GemVal _t3108 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3108.fn(_t3108.env, _t3107, 1);
#line 1095 "compiler/main.gem"
    GemVal _t3110;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t3110 = (*gem_v_in_top_level);
    } else {
        GemVal _t3109[] = {(*gem_v_top_level_vars), gem_v_name};
        _t3110 = gem_fn__mod_codegen_set_contains(NULL, _t3109, 2);
    }
        if (gem_truthy(_t3110)) {
#line 1096 "compiler/main.gem"
    GemVal _t3111[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3111, 2))) {
#line 1097 "compiler/main.gem"
    GemVal _t3112 = gem_v_r;
    static GemICacheSlot _ic_461 = {0};
    GemVal _t3113 = gem_v_r;
    static GemICacheSlot _ic_462 = {0};
    GemVal _t3114[] = {gem_v_line_dir, gem_table_get_cached(_t3112, "setup", &_ic_461), gem_v_p, gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3113, "expr", &_ic_462), gem_string(";")};
                GemVal _t3115 = gem_interp(11, _t3114);
                gem_pop_frame();
                return _t3115;
            }
#line 1099 "compiler/main.gem"
    GemVal _t3116 = gem_v_r;
    static GemICacheSlot _ic_463 = {0};
    GemVal _t3117 = gem_v_r;
    static GemICacheSlot _ic_464 = {0};
    GemVal _t3118[] = {gem_v_line_dir, gem_table_get_cached(_t3116, "setup", &_ic_463), gem_v_p, gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3117, "expr", &_ic_464), gem_string(";")};
            GemVal _t3119 = gem_interp(7, _t3118);
            gem_pop_frame();
            return _t3119;
        }
#line 1101 "compiler/main.gem"
    GemVal _t3120[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3120, 2))) {
#line 1102 "compiler/main.gem"
    GemVal _t3121 = gem_v_r;
    static GemICacheSlot _ic_465 = {0};
    GemVal _t3122 = gem_v_r;
    static GemICacheSlot _ic_466 = {0};
    GemVal _t3123[] = {gem_v_line_dir, gem_table_get_cached(_t3121, "setup", &_ic_465), gem_v_p, gem_string("GemVal *"), gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3122, "expr", &_ic_466), gem_string(";")};
            GemVal _t3124 = gem_interp(12, _t3123);
            gem_pop_frame();
            return _t3124;
        }
#line 1104 "compiler/main.gem"
    GemVal _t3125 = gem_v_r;
    static GemICacheSlot _ic_467 = {0};
    GemVal _t3126 = gem_v_r;
    static GemICacheSlot _ic_468 = {0};
    GemVal _t3127[] = {gem_v_line_dir, gem_table_get_cached(_t3125, "setup", &_ic_467), gem_v_p, gem_string("GemVal "), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3126, "expr", &_ic_468), gem_string(";")};
        GemVal _t3128 = gem_interp(8, _t3127);
        gem_pop_frame();
        return _t3128;
    } else {
    GemVal _t3129[] = {gem_v__match_60};
    GemVal _t3131;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3129, 1), gem_string("table")))) {
        _t3131 = gem_eq(gem_type_fn(NULL, _t3129, 1), gem_string("table"));
    } else {
        GemVal _t3130[] = {gem_v__match_60, gem_string("tag")};
        _t3131 = gem_has_key_fn(NULL, _t3130, 2);
    }
    GemVal _t3132;
    if (!gem_truthy(_t3131)) {
        _t3132 = _t3131;
    } else {
        _t3132 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3134;
    if (!gem_truthy(_t3132)) {
        _t3134 = _t3132;
    } else {
        GemVal _t3133[] = {gem_v__match_60, gem_string("name")};
        _t3134 = gem_has_key_fn(NULL, _t3133, 2);
    }
    GemVal _t3136;
    if (!gem_truthy(_t3134)) {
        _t3136 = _t3134;
    } else {
        GemVal _t3135[] = {gem_v__match_60, gem_string("value")};
        _t3136 = gem_has_key_fn(NULL, _t3135, 2);
    }
    if (gem_truthy(_t3136)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_60, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1106 "compiler/main.gem"
    GemVal _t3137[] = {gem_v_value};
    GemVal _t3138 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3138.fn(_t3138.env, _t3137, 1);
#line 1107 "compiler/main.gem"
    GemVal _t3139[] = {gem_v_name};
    GemVal _t3140 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3140.fn(_t3140.env, _t3139, 1);
#line 1108 "compiler/main.gem"
    GemVal _t3141[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3144;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3141, 2))) {
        _t3144 = gem_fn__mod_codegen_set_contains(NULL, _t3141, 2);
    } else {
        GemVal _t3143;
        if (!gem_truthy((*gem_v_in_top_level))) {
                _t3143 = (*gem_v_in_top_level);
        } else {
                GemVal _t3142[] = {(*gem_v_top_level_boxed), gem_v_name};
                _t3143 = gem_fn__mod_codegen_set_contains(NULL, _t3142, 2);
        }
        _t3144 = _t3143;
    }
        if (gem_truthy(_t3144)) {
#line 1109 "compiler/main.gem"
    GemVal _t3145 = gem_v_r;
    static GemICacheSlot _ic_469 = {0};
    GemVal _t3146 = gem_v_r;
    static GemICacheSlot _ic_470 = {0};
    GemVal _t3147[] = {gem_v_line_dir, gem_table_get_cached(_t3145, "setup", &_ic_469), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3146, "expr", &_ic_470), gem_string(";")};
            GemVal _t3148 = gem_interp(8, _t3147);
            gem_pop_frame();
            return _t3148;
        }
#line 1111 "compiler/main.gem"
    GemVal _t3149 = gem_v_r;
    static GemICacheSlot _ic_471 = {0};
    GemVal _t3150 = gem_v_r;
    static GemICacheSlot _ic_472 = {0};
    GemVal _t3151[] = {gem_v_line_dir, gem_table_get_cached(_t3149, "setup", &_ic_471), gem_v_p, gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3150, "expr", &_ic_472), gem_string(";")};
        GemVal _t3152 = gem_interp(7, _t3151);
        gem_pop_frame();
        return _t3152;
    } else {
    GemVal _t3153[] = {gem_v__match_60};
    GemVal _t3155;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3153, 1), gem_string("table")))) {
        _t3155 = gem_eq(gem_type_fn(NULL, _t3153, 1), gem_string("table"));
    } else {
        GemVal _t3154[] = {gem_v__match_60, gem_string("tag")};
        _t3155 = gem_has_key_fn(NULL, _t3154, 2);
    }
    GemVal _t3156;
    if (!gem_truthy(_t3155)) {
        _t3156 = _t3155;
    } else {
        _t3156 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t3158;
    if (!gem_truthy(_t3156)) {
        _t3158 = _t3156;
    } else {
        GemVal _t3157[] = {gem_v__match_60, gem_string("object")};
        _t3158 = gem_has_key_fn(NULL, _t3157, 2);
    }
    GemVal _t3160;
    if (!gem_truthy(_t3158)) {
        _t3160 = _t3158;
    } else {
        GemVal _t3159[] = {gem_v__match_60, gem_string("value")};
        _t3160 = gem_has_key_fn(NULL, _t3159, 2);
    }
    GemVal _t3162;
    if (!gem_truthy(_t3160)) {
        _t3162 = _t3160;
    } else {
        GemVal _t3161[] = {gem_v__match_60, gem_string("field")};
        _t3162 = gem_has_key_fn(NULL, _t3161, 2);
    }
    if (gem_truthy(_t3162)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_60, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_60, gem_string("field"));
#line 1113 "compiler/main.gem"
    GemVal _t3163[] = {gem_v_object};
    GemVal _t3164 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3164.fn(_t3164.env, _t3163, 1);
#line 1114 "compiler/main.gem"
    GemVal _t3165[] = {gem_v_value};
    GemVal _t3166 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3166.fn(_t3166.env, _t3165, 1);
#line 1115 "compiler/main.gem"
    GemVal _t3167[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t3167, 1);
#line 1116 "compiler/main.gem"
    GemVal _t3168 = gem_v_obj_r;
    static GemICacheSlot _ic_473 = {0};
    GemVal _t3169 = gem_v_val_r;
    static GemICacheSlot _ic_474 = {0};
    GemVal _t3170 = gem_v_obj_r;
    static GemICacheSlot _ic_475 = {0};
    GemVal _t3171 = gem_v_val_r;
    static GemICacheSlot _ic_476 = {0};
    GemVal _t3172[] = {gem_v_line_dir, gem_table_get_cached(_t3168, "setup", &_ic_473), gem_table_get_cached(_t3169, "setup", &_ic_474), gem_v_p, gem_string("gem_table_set("), gem_table_get_cached(_t3170, "expr", &_ic_475), gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"), "), gem_table_get_cached(_t3171, "expr", &_ic_476), gem_string(");")};
        GemVal _t3173 = gem_interp(11, _t3172);
        gem_pop_frame();
        return _t3173;
    } else {
    GemVal _t3174[] = {gem_v__match_60};
    GemVal _t3176;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3174, 1), gem_string("table")))) {
        _t3176 = gem_eq(gem_type_fn(NULL, _t3174, 1), gem_string("table"));
    } else {
        GemVal _t3175[] = {gem_v__match_60, gem_string("tag")};
        _t3176 = gem_has_key_fn(NULL, _t3175, 2);
    }
    GemVal _t3177;
    if (!gem_truthy(_t3176)) {
        _t3177 = _t3176;
    } else {
        _t3177 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t3179;
    if (!gem_truthy(_t3177)) {
        _t3179 = _t3177;
    } else {
        GemVal _t3178[] = {gem_v__match_60, gem_string("object")};
        _t3179 = gem_has_key_fn(NULL, _t3178, 2);
    }
    GemVal _t3181;
    if (!gem_truthy(_t3179)) {
        _t3181 = _t3179;
    } else {
        GemVal _t3180[] = {gem_v__match_60, gem_string("key")};
        _t3181 = gem_has_key_fn(NULL, _t3180, 2);
    }
    GemVal _t3183;
    if (!gem_truthy(_t3181)) {
        _t3183 = _t3181;
    } else {
        GemVal _t3182[] = {gem_v__match_60, gem_string("value")};
        _t3183 = gem_has_key_fn(NULL, _t3182, 2);
    }
    if (gem_truthy(_t3183)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_60, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_60, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1118 "compiler/main.gem"
    GemVal _t3184[] = {gem_v_object};
    GemVal _t3185 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3185.fn(_t3185.env, _t3184, 1);
#line 1119 "compiler/main.gem"
    GemVal _t3186[] = {gem_v_key};
    GemVal _t3187 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t3187.fn(_t3187.env, _t3186, 1);
#line 1120 "compiler/main.gem"
    GemVal _t3188[] = {gem_v_value};
    GemVal _t3189 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3189.fn(_t3189.env, _t3188, 1);
#line 1121 "compiler/main.gem"
    GemVal _t3190 = gem_v_obj_r;
    static GemICacheSlot _ic_477 = {0};
    GemVal _t3191 = gem_v_key_r;
    static GemICacheSlot _ic_478 = {0};
    GemVal _t3192 = gem_v_val_r;
    static GemICacheSlot _ic_479 = {0};
    GemVal _t3193 = gem_v_obj_r;
    static GemICacheSlot _ic_480 = {0};
    GemVal _t3194 = gem_v_key_r;
    static GemICacheSlot _ic_481 = {0};
    GemVal _t3195 = gem_v_val_r;
    static GemICacheSlot _ic_482 = {0};
    GemVal _t3196[] = {gem_v_line_dir, gem_table_get_cached(_t3190, "setup", &_ic_477), gem_table_get_cached(_t3191, "setup", &_ic_478), gem_table_get_cached(_t3192, "setup", &_ic_479), gem_v_p, gem_string("gem_table_set("), gem_table_get_cached(_t3193, "expr", &_ic_480), gem_string(", "), gem_table_get_cached(_t3194, "expr", &_ic_481), gem_string(", "), gem_table_get_cached(_t3195, "expr", &_ic_482), gem_string(");")};
        GemVal _t3197 = gem_interp(12, _t3196);
        gem_pop_frame();
        return _t3197;
    } else {
    GemVal _t3198[] = {gem_v__match_60};
    GemVal _t3200;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3198, 1), gem_string("table")))) {
        _t3200 = gem_eq(gem_type_fn(NULL, _t3198, 1), gem_string("table"));
    } else {
        GemVal _t3199[] = {gem_v__match_60, gem_string("tag")};
        _t3200 = gem_has_key_fn(NULL, _t3199, 2);
    }
    GemVal _t3201;
    if (!gem_truthy(_t3200)) {
        _t3201 = _t3200;
    } else {
        _t3201 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3201)) {
#line 1123 "compiler/main.gem"
    GemVal _t3202[] = {gem_v_line_dir};
    GemVal _t3203[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3204 = (*gem_v_compile_if);
        GemVal _t3205 = gem_add(gem_interp(1, _t3202), _t3204.fn(_t3204.env, _t3203, 3));
        gem_pop_frame();
        return _t3205;
    } else {
    GemVal _t3206[] = {gem_v__match_60};
    GemVal _t3208;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3206, 1), gem_string("table")))) {
        _t3208 = gem_eq(gem_type_fn(NULL, _t3206, 1), gem_string("table"));
    } else {
        GemVal _t3207[] = {gem_v__match_60, gem_string("tag")};
        _t3208 = gem_has_key_fn(NULL, _t3207, 2);
    }
    GemVal _t3209;
    if (!gem_truthy(_t3208)) {
        _t3209 = _t3208;
    } else {
        _t3209 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3209)) {
#line 1125 "compiler/main.gem"
    GemVal _t3210[] = {gem_v_line_dir};
    GemVal _t3211[] = {gem_v_node, gem_v_indent};
    GemVal _t3212 = (*gem_v_compile_while);
        GemVal _t3213 = gem_add(gem_interp(1, _t3210), _t3212.fn(_t3212.env, _t3211, 2));
        gem_pop_frame();
        return _t3213;
    } else {
    GemVal _t3214[] = {gem_v__match_60};
    GemVal _t3216;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3214, 1), gem_string("table")))) {
        _t3216 = gem_eq(gem_type_fn(NULL, _t3214, 1), gem_string("table"));
    } else {
        GemVal _t3215[] = {gem_v__match_60, gem_string("tag")};
        _t3216 = gem_has_key_fn(NULL, _t3215, 2);
    }
    GemVal _t3217;
    if (!gem_truthy(_t3216)) {
        _t3217 = _t3216;
    } else {
        _t3217 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3217)) {
#line 1127 "compiler/main.gem"
    GemVal _t3218[] = {gem_v_line_dir};
    GemVal _t3219[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3220 = (*gem_v_compile_match);
        GemVal _t3221 = gem_add(gem_interp(1, _t3218), _t3220.fn(_t3220.env, _t3219, 3));
        gem_pop_frame();
        return _t3221;
    } else {
    GemVal _t3222[] = {gem_v__match_60};
    GemVal _t3224;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3222, 1), gem_string("table")))) {
        _t3224 = gem_eq(gem_type_fn(NULL, _t3222, 1), gem_string("table"));
    } else {
        GemVal _t3223[] = {gem_v__match_60, gem_string("tag")};
        _t3224 = gem_has_key_fn(NULL, _t3223, 2);
    }
    GemVal _t3225;
    if (!gem_truthy(_t3224)) {
        _t3225 = _t3224;
    } else {
        _t3225 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3225)) {
#line 1129 "compiler/main.gem"
    GemVal _t3226[] = {gem_v_line_dir};
    GemVal _t3227[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3228 = (*gem_v_compile_receive_match);
        GemVal _t3229 = gem_add(gem_interp(1, _t3226), _t3228.fn(_t3228.env, _t3227, 3));
        gem_pop_frame();
        return _t3229;
    } else {
    GemVal _t3230[] = {gem_v__match_60};
    GemVal _t3232;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3230, 1), gem_string("table")))) {
        _t3232 = gem_eq(gem_type_fn(NULL, _t3230, 1), gem_string("table"));
    } else {
        GemVal _t3231[] = {gem_v__match_60, gem_string("tag")};
        _t3232 = gem_has_key_fn(NULL, _t3231, 2);
    }
    GemVal _t3233;
    if (!gem_truthy(_t3232)) {
        _t3233 = _t3232;
    } else {
        _t3233 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3233)) {
#line 1131 "compiler/main.gem"
        GemVal _t3234 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t3234;
    } else {
    GemVal _t3235[] = {gem_v__match_60};
    GemVal _t3237;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3235, 1), gem_string("table")))) {
        _t3237 = gem_eq(gem_type_fn(NULL, _t3235, 1), gem_string("table"));
    } else {
        GemVal _t3236[] = {gem_v__match_60, gem_string("tag")};
        _t3237 = gem_has_key_fn(NULL, _t3236, 2);
    }
    GemVal _t3238;
    if (!gem_truthy(_t3237)) {
        _t3238 = _t3237;
    } else {
        _t3238 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3238)) {
#line 1133 "compiler/main.gem"
        GemVal _t3239 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t3239;
    } else {
    GemVal _t3240[] = {gem_v__match_60};
    GemVal _t3242;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3240, 1), gem_string("table")))) {
        _t3242 = gem_eq(gem_type_fn(NULL, _t3240, 1), gem_string("table"));
    } else {
        GemVal _t3241[] = {gem_v__match_60, gem_string("tag")};
        _t3242 = gem_has_key_fn(NULL, _t3241, 2);
    }
    GemVal _t3243;
    if (!gem_truthy(_t3242)) {
        _t3243 = _t3242;
    } else {
        _t3243 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3245;
    if (!gem_truthy(_t3243)) {
        _t3245 = _t3243;
    } else {
        GemVal _t3244[] = {gem_v__match_60, gem_string("value")};
        _t3245 = gem_has_key_fn(NULL, _t3244, 2);
    }
    if (gem_truthy(_t3245)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1135 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1136 "compiler/main.gem"
    GemVal _t3246[] = {gem_v_value};
    GemVal _t3247 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3247.fn(_t3247.env, _t3246, 1);
#line 1137 "compiler/main.gem"
    GemVal _t3248 = (*gem_v_tmp);
            GemVal gem_v_t = _t3248.fn(_t3248.env, NULL, 0);
#line 1138 "compiler/main.gem"
    GemVal _t3249 = gem_v_r;
    static GemICacheSlot _ic_483 = {0};
    GemVal _t3250 = gem_v_r;
    static GemICacheSlot _ic_484 = {0};
    GemVal _t3251[] = {gem_v_line_dir, gem_table_get_cached(_t3249, "setup", &_ic_483), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3250, "expr", &_ic_484), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3252 = gem_interp(14, _t3251);
            gem_pop_frame();
            return _t3252;
        }
#line 1140 "compiler/main.gem"
    GemVal _t3253[] = {gem_v_line_dir, gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3254 = gem_interp(5, _t3253);
        gem_pop_frame();
        return _t3254;
    } else {
    GemVal _t3255[] = {gem_v__match_60};
    GemVal _t3257;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3255, 1), gem_string("table")))) {
        _t3257 = gem_eq(gem_type_fn(NULL, _t3255, 1), gem_string("table"));
    } else {
        GemVal _t3256[] = {gem_v__match_60, gem_string("tag")};
        _t3257 = gem_has_key_fn(NULL, _t3256, 2);
    }
    GemVal _t3258;
    if (!gem_truthy(_t3257)) {
        _t3258 = _t3257;
    } else {
        _t3258 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t3258)) {
#line 1142 "compiler/main.gem"
    GemVal _t3259[] = {gem_v_node};
    GemVal _t3260 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3260.fn(_t3260.env, _t3259, 1);
#line 1143 "compiler/main.gem"
    GemVal _t3261 = gem_v_r;
    static GemICacheSlot _ic_485 = {0};
    GemVal _t3262 = gem_v_r;
    static GemICacheSlot _ic_486 = {0};
    GemVal _t3263[] = {gem_v_line_dir, gem_table_get_cached(_t3261, "setup", &_ic_485), gem_v_p, gem_string("(void)("), gem_table_get_cached(_t3262, "expr", &_ic_486), gem_string(");")};
        GemVal _t3264 = gem_interp(6, _t3263);
        gem_pop_frame();
        return _t3264;
    } else {
    GemVal _t3265[] = {gem_v__match_60};
    GemVal _t3267;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3265, 1), gem_string("table")))) {
        _t3267 = gem_eq(gem_type_fn(NULL, _t3265, 1), gem_string("table"));
    } else {
        GemVal _t3266[] = {gem_v__match_60, gem_string("tag")};
        _t3267 = gem_has_key_fn(NULL, _t3266, 2);
    }
    GemVal _t3268;
    if (!gem_truthy(_t3267)) {
        _t3268 = _t3267;
    } else {
        _t3268 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3268)) {
#line 1145 "compiler/main.gem"
        GemVal _t3269 = gem_string("");
        gem_pop_frame();
        return _t3269;
    } else {
    GemVal _t3270[] = {gem_v__match_60};
    GemVal _t3272;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3270, 1), gem_string("table")))) {
        _t3272 = gem_eq(gem_type_fn(NULL, _t3270, 1), gem_string("table"));
    } else {
        GemVal _t3271[] = {gem_v__match_60, gem_string("tag")};
        _t3272 = gem_has_key_fn(NULL, _t3271, 2);
    }
    GemVal _t3273;
    if (!gem_truthy(_t3272)) {
        _t3273 = _t3272;
    } else {
        _t3273 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3275;
    if (!gem_truthy(_t3273)) {
        _t3275 = _t3273;
    } else {
        GemVal _t3274[] = {gem_v__match_60, gem_string("stmts")};
        _t3275 = gem_has_key_fn(NULL, _t3274, 2);
    }
    if (gem_truthy(_t3275)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_60, gem_string("stmts"));
#line 1147 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1148 "compiler/main.gem"
        GemVal gem_v__for_items_61 = gem_v_stmts;
#line 1148 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1148 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3276[] = {gem_v__for_items_61};
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3276, 1)))) break;
#line 1148 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1148 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1149 "compiler/main.gem"
    GemVal _t3277[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3278 = (*gem_v_compile_stmt);
    GemVal _t3279[] = {gem_v_b, _t3278.fn(_t3278.env, _t3277, 2)};
            (void)(gem_buf_push_fn(NULL, _t3279, 2));
#line 1150 "compiler/main.gem"
    GemVal _t3280[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3280, 2));
        }

#line 1152 "compiler/main.gem"
    GemVal _t3281[] = {gem_v_b};
        GemVal _t3282 = gem_buf_str_fn(NULL, _t3281, 1);
        gem_pop_frame();
        return _t3282;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1155 "compiler/main.gem"
    GemVal _t3283[] = {gem_v_node};
    GemVal _t3284 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3284.fn(_t3284.env, _t3283, 1);
    GemVal _t3285 = gem_v_r;
    static GemICacheSlot _ic_487 = {0};
    GemVal _t3286 = gem_v_r;
    static GemICacheSlot _ic_488 = {0};
    GemVal _t3287[] = {gem_v_line_dir, gem_table_get_cached(_t3285, "setup", &_ic_487), gem_v_p, gem_string("(void)("), gem_table_get_cached(_t3286, "expr", &_ic_488), gem_string(");")};
    GemVal _t3288 = gem_interp(6, _t3287);
    gem_pop_frame();
    return _t3288;
}

struct _closure__anon_47 {
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_if;
    GemVal *gem_v_compile_match;
    GemVal *gem_v_compile_receive_match;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_compile_while;
    GemVal *gem_v_emit_tco_continue;
    GemVal *gem_v_in_top_level;
    GemVal *gem_v_local_names;
    GemVal *gem_v_mangle;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tco_fn_name;
    GemVal *gem_v_tmp;
    GemVal *gem_v_top_level_boxed;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_if = _cls->gem_v_compile_if;
    GemVal *gem_v_compile_match = _cls->gem_v_compile_match;
    GemVal *gem_v_compile_receive_match = _cls->gem_v_compile_receive_match;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_compile_while = _cls->gem_v_compile_while;
    GemVal *gem_v_emit_tco_continue = _cls->gem_v_emit_tco_continue;
    GemVal *gem_v_in_top_level = _cls->gem_v_in_top_level;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tco_fn_name = _cls->gem_v_tco_fn_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal *gem_v_top_level_boxed = _cls->gem_v_top_level_boxed;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 1162 "compiler/main.gem"
    GemVal _t3290[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3290, 1);
#line 1163 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1164 "compiler/main.gem"
    GemVal _t3291 = gem_v_node;
    static GemICacheSlot _ic_489 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3291, "line", &_ic_489), GEM_NIL))) {
#line 1165 "compiler/main.gem"
    GemVal _t3292 = gem_v_node;
    static GemICacheSlot _ic_490 = {0};
    GemVal _t3293[] = {(*gem_v_source_name)};
    GemVal _t3294[] = {gem_string("#line "), gem_table_get_cached(_t3292, "line", &_ic_490), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3293, 1), gem_string("\"\n")};
        gem_v_line_dir = gem_interp(5, _t3294);
    }
#line 1168 "compiler/main.gem"
    if (gem_truthy(gem_neq((*gem_v_tco_fn_name), GEM_NIL))) {
#line 1169 "compiler/main.gem"
    GemVal _t3295[] = {gem_v_node, gem_string("call")};
    GemVal _t3298;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3295, 2))) {
        _t3298 = gem_fn__mod_codegen_is_node(NULL, _t3295, 2);
    } else {
        GemVal _t3296 = gem_v_node;
        static GemICacheSlot _ic_491 = {0};
        GemVal _t3297[] = {gem_table_get_cached(_t3296, "func", &_ic_491), gem_string("var")};
        _t3298 = gem_fn__mod_codegen_is_node(NULL, _t3297, 2);
    }
    GemVal _t3301;
    if (!gem_truthy(_t3298)) {
        _t3301 = _t3298;
    } else {
        GemVal _t3299 = gem_v_node;
        static GemICacheSlot _ic_492 = {0};
        GemVal _t3300 = gem_table_get_cached(_t3299, "func", &_ic_492);
        static GemICacheSlot _ic_493 = {0};
        _t3301 = gem_eq(gem_table_get_cached(_t3300, "name", &_ic_493), (*gem_v_tco_fn_name));
    }
    GemVal _t3303;
    if (!gem_truthy(_t3301)) {
        _t3303 = _t3301;
    } else {
        GemVal _t3302[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3303 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3302, 2));
    }
        if (gem_truthy(_t3303)) {
#line 1170 "compiler/main.gem"
    GemVal _t3304[] = {gem_v_node, gem_v_indent};
    GemVal _t3305 = (*gem_v_emit_tco_continue);
            GemVal _t3306 = gem_add(gem_v_line_dir, _t3305.fn(_t3305.env, _t3304, 2));
            gem_pop_frame();
            return _t3306;
        }
#line 1172 "compiler/main.gem"
    GemVal _t3307[] = {gem_v_node, gem_string("return")};
    GemVal _t3309;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3307, 2))) {
        _t3309 = gem_fn__mod_codegen_is_node(NULL, _t3307, 2);
    } else {
        GemVal _t3308 = gem_v_node;
        static GemICacheSlot _ic_494 = {0};
        _t3309 = gem_neq(gem_table_get_cached(_t3308, "value", &_ic_494), GEM_NIL);
    }
    GemVal _t3312;
    if (!gem_truthy(_t3309)) {
        _t3312 = _t3309;
    } else {
        GemVal _t3310 = gem_v_node;
        static GemICacheSlot _ic_495 = {0};
        GemVal _t3311[] = {gem_table_get_cached(_t3310, "value", &_ic_495), gem_string("call")};
        _t3312 = gem_fn__mod_codegen_is_node(NULL, _t3311, 2);
    }
    GemVal _t3316;
    if (!gem_truthy(_t3312)) {
        _t3316 = _t3312;
    } else {
        GemVal _t3313 = gem_v_node;
        static GemICacheSlot _ic_496 = {0};
        GemVal _t3314 = gem_table_get_cached(_t3313, "value", &_ic_496);
        static GemICacheSlot _ic_497 = {0};
        GemVal _t3315[] = {gem_table_get_cached(_t3314, "func", &_ic_497), gem_string("var")};
        _t3316 = gem_fn__mod_codegen_is_node(NULL, _t3315, 2);
    }
    GemVal _t3320;
    if (!gem_truthy(_t3316)) {
        _t3320 = _t3316;
    } else {
        GemVal _t3317 = gem_v_node;
        static GemICacheSlot _ic_498 = {0};
        GemVal _t3318 = gem_table_get_cached(_t3317, "value", &_ic_498);
        static GemICacheSlot _ic_499 = {0};
        GemVal _t3319 = gem_table_get_cached(_t3318, "func", &_ic_499);
        static GemICacheSlot _ic_500 = {0};
        _t3320 = gem_eq(gem_table_get_cached(_t3319, "name", &_ic_500), (*gem_v_tco_fn_name));
    }
    GemVal _t3322;
    if (!gem_truthy(_t3320)) {
        _t3322 = _t3320;
    } else {
        GemVal _t3321[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3322 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3321, 2));
    }
        if (gem_truthy(_t3322)) {
#line 1173 "compiler/main.gem"
    GemVal _t3323 = gem_v_node;
    static GemICacheSlot _ic_501 = {0};
    GemVal _t3324[] = {gem_table_get_cached(_t3323, "value", &_ic_501), gem_v_indent};
    GemVal _t3325 = (*gem_v_emit_tco_continue);
            GemVal _t3326 = gem_add(gem_v_line_dir, _t3325.fn(_t3325.env, _t3324, 2));
            gem_pop_frame();
            return _t3326;
        }
    }
#line 1177 "compiler/main.gem"
    GemVal gem_v__match_62 = gem_v_node;
    GemVal _t3327[] = {gem_v__match_62};
    GemVal _t3329;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3327, 1), gem_string("table")))) {
        _t3329 = gem_eq(gem_type_fn(NULL, _t3327, 1), gem_string("table"));
    } else {
        GemVal _t3328[] = {gem_v__match_62, gem_string("tag")};
        _t3329 = gem_has_key_fn(NULL, _t3328, 2);
    }
    GemVal _t3330;
    if (!gem_truthy(_t3329)) {
        _t3330 = _t3329;
    } else {
        _t3330 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3332;
    if (!gem_truthy(_t3330)) {
        _t3332 = _t3330;
    } else {
        GemVal _t3331[] = {gem_v__match_62, gem_string("name")};
        _t3332 = gem_has_key_fn(NULL, _t3331, 2);
    }
    GemVal _t3334;
    if (!gem_truthy(_t3332)) {
        _t3334 = _t3332;
    } else {
        GemVal _t3333[] = {gem_v__match_62, gem_string("value")};
        _t3334 = gem_has_key_fn(NULL, _t3333, 2);
    }
    if (gem_truthy(_t3334)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_62, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_62, gem_string("value"));
#line 1179 "compiler/main.gem"
    GemVal _t3335[] = {gem_v_value};
    GemVal _t3336 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3336.fn(_t3336.env, _t3335, 1);
#line 1180 "compiler/main.gem"
    GemVal _t3337[] = {gem_v_name};
    GemVal _t3338 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3338.fn(_t3338.env, _t3337, 1);
#line 1181 "compiler/main.gem"
    GemVal _t3339[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3339, 2))) {
#line 1182 "compiler/main.gem"
    GemVal _t3340 = gem_v_r;
    static GemICacheSlot _ic_502 = {0};
    GemVal _t3341 = gem_v_r;
    static GemICacheSlot _ic_503 = {0};
    GemVal _t3342[] = {gem_v_line_dir, gem_table_get_cached(_t3340, "setup", &_ic_502), gem_v_p, gem_string("GemVal *"), gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3341, "expr", &_ic_503), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
            GemVal _t3343 = gem_interp(16, _t3342);
            gem_pop_frame();
            return _t3343;
        }
#line 1184 "compiler/main.gem"
    GemVal _t3344 = gem_v_r;
    static GemICacheSlot _ic_504 = {0};
    GemVal _t3345 = gem_v_r;
    static GemICacheSlot _ic_505 = {0};
    GemVal _t3346[] = {gem_v_line_dir, gem_table_get_cached(_t3344, "setup", &_ic_504), gem_v_p, gem_string("GemVal "), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3345, "expr", &_ic_505), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3347 = gem_interp(12, _t3346);
        gem_pop_frame();
        return _t3347;
    } else {
    GemVal _t3348[] = {gem_v__match_62};
    GemVal _t3350;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3348, 1), gem_string("table")))) {
        _t3350 = gem_eq(gem_type_fn(NULL, _t3348, 1), gem_string("table"));
    } else {
        GemVal _t3349[] = {gem_v__match_62, gem_string("tag")};
        _t3350 = gem_has_key_fn(NULL, _t3349, 2);
    }
    GemVal _t3351;
    if (!gem_truthy(_t3350)) {
        _t3351 = _t3350;
    } else {
        _t3351 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3353;
    if (!gem_truthy(_t3351)) {
        _t3353 = _t3351;
    } else {
        GemVal _t3352[] = {gem_v__match_62, gem_string("name")};
        _t3353 = gem_has_key_fn(NULL, _t3352, 2);
    }
    GemVal _t3355;
    if (!gem_truthy(_t3353)) {
        _t3355 = _t3353;
    } else {
        GemVal _t3354[] = {gem_v__match_62, gem_string("value")};
        _t3355 = gem_has_key_fn(NULL, _t3354, 2);
    }
    if (gem_truthy(_t3355)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_62, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_62, gem_string("value"));
#line 1186 "compiler/main.gem"
    GemVal _t3356[] = {gem_v_value};
    GemVal _t3357 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3357.fn(_t3357.env, _t3356, 1);
#line 1187 "compiler/main.gem"
    GemVal _t3358 = (*gem_v_tmp);
        GemVal gem_v_t = _t3358.fn(_t3358.env, NULL, 0);
#line 1188 "compiler/main.gem"
    GemVal _t3359[] = {gem_v_name};
    GemVal _t3360 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3360.fn(_t3360.env, _t3359, 1);
#line 1189 "compiler/main.gem"
    GemVal _t3361[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3364;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3361, 2))) {
        _t3364 = gem_fn__mod_codegen_set_contains(NULL, _t3361, 2);
    } else {
        GemVal _t3363;
        if (!gem_truthy((*gem_v_in_top_level))) {
                _t3363 = (*gem_v_in_top_level);
        } else {
                GemVal _t3362[] = {(*gem_v_top_level_boxed), gem_v_name};
                _t3363 = gem_fn__mod_codegen_set_contains(NULL, _t3362, 2);
        }
        _t3364 = _t3363;
    }
        if (gem_truthy(_t3364)) {
#line 1190 "compiler/main.gem"
    GemVal _t3365 = gem_v_r;
    static GemICacheSlot _ic_506 = {0};
    GemVal _t3366 = gem_v_r;
    static GemICacheSlot _ic_507 = {0};
    GemVal _t3367[] = {gem_v_line_dir, gem_table_get_cached(_t3365, "setup", &_ic_506), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3366, "expr", &_ic_507), gem_string(";\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_v_t, gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3368 = gem_interp(20, _t3367);
            gem_pop_frame();
            return _t3368;
        }
#line 1192 "compiler/main.gem"
    GemVal _t3369 = gem_v_r;
    static GemICacheSlot _ic_508 = {0};
    GemVal _t3370 = gem_v_r;
    static GemICacheSlot _ic_509 = {0};
    GemVal _t3371[] = {gem_v_line_dir, gem_table_get_cached(_t3369, "setup", &_ic_508), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3370, "expr", &_ic_509), gem_string(";\n"), gem_v_p, gem_v_mname, gem_string(" = "), gem_v_t, gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
        GemVal _t3372 = gem_interp(19, _t3371);
        gem_pop_frame();
        return _t3372;
    } else {
    GemVal _t3373[] = {gem_v__match_62};
    GemVal _t3375;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3373, 1), gem_string("table")))) {
        _t3375 = gem_eq(gem_type_fn(NULL, _t3373, 1), gem_string("table"));
    } else {
        GemVal _t3374[] = {gem_v__match_62, gem_string("tag")};
        _t3375 = gem_has_key_fn(NULL, _t3374, 2);
    }
    GemVal _t3376;
    if (!gem_truthy(_t3375)) {
        _t3376 = _t3375;
    } else {
        _t3376 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3378;
    if (!gem_truthy(_t3376)) {
        _t3378 = _t3376;
    } else {
        GemVal _t3377[] = {gem_v__match_62, gem_string("value")};
        _t3378 = gem_has_key_fn(NULL, _t3377, 2);
    }
    if (gem_truthy(_t3378)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_62, gem_string("value"));
#line 1194 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1195 "compiler/main.gem"
    GemVal _t3379[] = {gem_v_value};
    GemVal _t3380 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3380.fn(_t3380.env, _t3379, 1);
#line 1196 "compiler/main.gem"
    GemVal _t3381 = (*gem_v_tmp);
            GemVal gem_v_t = _t3381.fn(_t3381.env, NULL, 0);
#line 1197 "compiler/main.gem"
    GemVal _t3382 = gem_v_r;
    static GemICacheSlot _ic_510 = {0};
    GemVal _t3383 = gem_v_r;
    static GemICacheSlot _ic_511 = {0};
    GemVal _t3384[] = {gem_v_line_dir, gem_table_get_cached(_t3382, "setup", &_ic_510), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3383, "expr", &_ic_511), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3385 = gem_interp(14, _t3384);
            gem_pop_frame();
            return _t3385;
        }
#line 1199 "compiler/main.gem"
    GemVal _t3386[] = {gem_v_line_dir, gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3387 = gem_interp(5, _t3386);
        gem_pop_frame();
        return _t3387;
    } else {
    GemVal _t3388[] = {gem_v__match_62};
    GemVal _t3390;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3388, 1), gem_string("table")))) {
        _t3390 = gem_eq(gem_type_fn(NULL, _t3388, 1), gem_string("table"));
    } else {
        GemVal _t3389[] = {gem_v__match_62, gem_string("tag")};
        _t3390 = gem_has_key_fn(NULL, _t3389, 2);
    }
    GemVal _t3391;
    if (!gem_truthy(_t3390)) {
        _t3391 = _t3390;
    } else {
        _t3391 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3391)) {
#line 1201 "compiler/main.gem"
    GemVal _t3392[] = {gem_v_line_dir};
    GemVal _t3393[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3394 = (*gem_v_compile_if);
        GemVal _t3395 = gem_add(gem_interp(1, _t3392), _t3394.fn(_t3394.env, _t3393, 3));
        gem_pop_frame();
        return _t3395;
    } else {
    GemVal _t3396[] = {gem_v__match_62};
    GemVal _t3398;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3396, 1), gem_string("table")))) {
        _t3398 = gem_eq(gem_type_fn(NULL, _t3396, 1), gem_string("table"));
    } else {
        GemVal _t3397[] = {gem_v__match_62, gem_string("tag")};
        _t3398 = gem_has_key_fn(NULL, _t3397, 2);
    }
    GemVal _t3399;
    if (!gem_truthy(_t3398)) {
        _t3399 = _t3398;
    } else {
        _t3399 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3399)) {
#line 1203 "compiler/main.gem"
    GemVal _t3400[] = {gem_v_node, gem_v_indent};
    GemVal _t3401 = (*gem_v_compile_while);
    GemVal _t3402[] = {gem_v_line_dir, _t3401.fn(_t3401.env, _t3400, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3403 = gem_interp(7, _t3402);
        gem_pop_frame();
        return _t3403;
    } else {
    GemVal _t3404[] = {gem_v__match_62};
    GemVal _t3406;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3404, 1), gem_string("table")))) {
        _t3406 = gem_eq(gem_type_fn(NULL, _t3404, 1), gem_string("table"));
    } else {
        GemVal _t3405[] = {gem_v__match_62, gem_string("tag")};
        _t3406 = gem_has_key_fn(NULL, _t3405, 2);
    }
    GemVal _t3407;
    if (!gem_truthy(_t3406)) {
        _t3407 = _t3406;
    } else {
        _t3407 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3407)) {
#line 1205 "compiler/main.gem"
    GemVal _t3408[] = {gem_v_line_dir};
    GemVal _t3409[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3410 = (*gem_v_compile_match);
        GemVal _t3411 = gem_add(gem_interp(1, _t3408), _t3410.fn(_t3410.env, _t3409, 3));
        gem_pop_frame();
        return _t3411;
    } else {
    GemVal _t3412[] = {gem_v__match_62};
    GemVal _t3414;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3412, 1), gem_string("table")))) {
        _t3414 = gem_eq(gem_type_fn(NULL, _t3412, 1), gem_string("table"));
    } else {
        GemVal _t3413[] = {gem_v__match_62, gem_string("tag")};
        _t3414 = gem_has_key_fn(NULL, _t3413, 2);
    }
    GemVal _t3415;
    if (!gem_truthy(_t3414)) {
        _t3415 = _t3414;
    } else {
        _t3415 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3415)) {
#line 1207 "compiler/main.gem"
    GemVal _t3416[] = {gem_v_line_dir};
    GemVal _t3417[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3418 = (*gem_v_compile_receive_match);
        GemVal _t3419 = gem_add(gem_interp(1, _t3416), _t3418.fn(_t3418.env, _t3417, 3));
        gem_pop_frame();
        return _t3419;
    } else {
    GemVal _t3420[] = {gem_v__match_62};
    GemVal _t3422;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3420, 1), gem_string("table")))) {
        _t3422 = gem_eq(gem_type_fn(NULL, _t3420, 1), gem_string("table"));
    } else {
        GemVal _t3421[] = {gem_v__match_62, gem_string("tag")};
        _t3422 = gem_has_key_fn(NULL, _t3421, 2);
    }
    GemVal _t3423;
    if (!gem_truthy(_t3422)) {
        _t3423 = _t3422;
    } else {
        _t3423 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3425;
    if (!gem_truthy(_t3423)) {
        _t3425 = _t3423;
    } else {
        GemVal _t3424[] = {gem_v__match_62, gem_string("stmts")};
        _t3425 = gem_has_key_fn(NULL, _t3424, 2);
    }
    if (gem_truthy(_t3425)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_62, gem_string("stmts"));
#line 1209 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1210 "compiler/main.gem"
    GemVal _t3426[] = {gem_v_p, gem_string("{\n")};
    GemVal _t3427[] = {gem_v_b, gem_interp(2, _t3426)};
        (void)(gem_buf_push_fn(NULL, _t3427, 2));
#line 1211 "compiler/main.gem"
        GemVal gem_v__for_i_63 = gem_int(0);
#line 1211 "compiler/main.gem"
    GemVal _t3428[] = {gem_v_stmts};
        GemVal gem_v__for_limit_63 = gem_sub(gem_len_fn(NULL, _t3428, 1), gem_int(1));
#line 1211 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1211 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_63;
#line 1211 "compiler/main.gem"
            gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1212 "compiler/main.gem"
    GemVal _t3429[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3430 = (*gem_v_compile_stmt);
    GemVal _t3431[] = {gem_v_b, _t3430.fn(_t3430.env, _t3429, 2)};
            (void)(gem_buf_push_fn(NULL, _t3431, 2));
#line 1213 "compiler/main.gem"
    GemVal _t3432[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3432, 2));
        }

#line 1215 "compiler/main.gem"
    GemVal _t3433[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3433, 1), gem_int(0)))) {
#line 1216 "compiler/main.gem"
    GemVal _t3434[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3435 = (*gem_v_compile_stmt_return);
    GemVal _t3436[] = {gem_v_b, _t3435.fn(_t3435.env, _t3434, 2)};
            (void)(gem_buf_push_fn(NULL, _t3436, 2));
#line 1217 "compiler/main.gem"
    GemVal _t3437[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3437, 2));
        }
#line 1219 "compiler/main.gem"
    GemVal _t3438[] = {gem_v_p, gem_string("}")};
    GemVal _t3439[] = {gem_v_b, gem_interp(2, _t3438)};
        (void)(gem_buf_push_fn(NULL, _t3439, 2));
#line 1220 "compiler/main.gem"
    GemVal _t3440[] = {gem_v_b};
        GemVal _t3441 = gem_buf_str_fn(NULL, _t3440, 1);
        gem_pop_frame();
        return _t3441;
    } else {
    GemVal _t3442[] = {gem_v__match_62};
    GemVal _t3444;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3442, 1), gem_string("table")))) {
        _t3444 = gem_eq(gem_type_fn(NULL, _t3442, 1), gem_string("table"));
    } else {
        GemVal _t3443[] = {gem_v__match_62, gem_string("tag")};
        _t3444 = gem_has_key_fn(NULL, _t3443, 2);
    }
    GemVal _t3445;
    if (!gem_truthy(_t3444)) {
        _t3445 = _t3444;
    } else {
        _t3445 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3445)) {
#line 1222 "compiler/main.gem"
    GemVal _t3446[] = {gem_v_node, gem_v_indent};
    GemVal _t3447 = (*gem_v_compile_stmt);
    GemVal _t3448[] = {_t3447.fn(_t3447.env, _t3446, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3449 = gem_interp(6, _t3448);
        gem_pop_frame();
        return _t3449;
    } else {
    GemVal _t3450[] = {gem_v__match_62};
    GemVal _t3452;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3450, 1), gem_string("table")))) {
        _t3452 = gem_eq(gem_type_fn(NULL, _t3450, 1), gem_string("table"));
    } else {
        GemVal _t3451[] = {gem_v__match_62, gem_string("tag")};
        _t3452 = gem_has_key_fn(NULL, _t3451, 2);
    }
    GemVal _t3453;
    if (!gem_truthy(_t3452)) {
        _t3453 = _t3452;
    } else {
        _t3453 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3453)) {
#line 1224 "compiler/main.gem"
    GemVal _t3454[] = {gem_v_node, gem_v_indent};
    GemVal _t3455 = (*gem_v_compile_stmt);
    GemVal _t3456[] = {_t3455.fn(_t3455.env, _t3454, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3457 = gem_interp(6, _t3456);
        gem_pop_frame();
        return _t3457;
    } else {
    GemVal _t3458[] = {gem_v__match_62};
    GemVal _t3460;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3458, 1), gem_string("table")))) {
        _t3460 = gem_eq(gem_type_fn(NULL, _t3458, 1), gem_string("table"));
    } else {
        GemVal _t3459[] = {gem_v__match_62, gem_string("tag")};
        _t3460 = gem_has_key_fn(NULL, _t3459, 2);
    }
    GemVal _t3461;
    if (!gem_truthy(_t3460)) {
        _t3461 = _t3460;
    } else {
        _t3461 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3461)) {
#line 1226 "compiler/main.gem"
    GemVal _t3462[] = {gem_v_node, gem_v_indent};
    GemVal _t3463 = (*gem_v_compile_stmt);
    GemVal _t3464[] = {_t3463.fn(_t3463.env, _t3462, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3465 = gem_interp(6, _t3464);
        gem_pop_frame();
        return _t3465;
    } else {
    GemVal _t3466[] = {gem_v__match_62};
    GemVal _t3468;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3466, 1), gem_string("table")))) {
        _t3468 = gem_eq(gem_type_fn(NULL, _t3466, 1), gem_string("table"));
    } else {
        GemVal _t3467[] = {gem_v__match_62, gem_string("tag")};
        _t3468 = gem_has_key_fn(NULL, _t3467, 2);
    }
    GemVal _t3469;
    if (!gem_truthy(_t3468)) {
        _t3469 = _t3468;
    } else {
        _t3469 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3469)) {
#line 1228 "compiler/main.gem"
    GemVal _t3470[] = {gem_v_node, gem_v_indent};
    GemVal _t3471 = (*gem_v_compile_stmt);
    GemVal _t3472[] = {_t3471.fn(_t3471.env, _t3470, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3473 = gem_interp(6, _t3472);
        gem_pop_frame();
        return _t3473;
    } else {
    GemVal _t3474[] = {gem_v__match_62};
    GemVal _t3476;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3474, 1), gem_string("table")))) {
        _t3476 = gem_eq(gem_type_fn(NULL, _t3474, 1), gem_string("table"));
    } else {
        GemVal _t3475[] = {gem_v__match_62, gem_string("tag")};
        _t3476 = gem_has_key_fn(NULL, _t3475, 2);
    }
    GemVal _t3477;
    if (!gem_truthy(_t3476)) {
        _t3477 = _t3476;
    } else {
        _t3477 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3477)) {
#line 1230 "compiler/main.gem"
    GemVal _t3478[] = {gem_v_node, gem_v_indent};
    GemVal _t3479 = (*gem_v_compile_stmt);
    GemVal _t3480[] = {_t3479.fn(_t3479.env, _t3478, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3481 = gem_interp(6, _t3480);
        gem_pop_frame();
        return _t3481;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1233 "compiler/main.gem"
    GemVal _t3482[] = {gem_v_node};
    GemVal _t3483 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3483.fn(_t3483.env, _t3482, 1);
#line 1234 "compiler/main.gem"
    GemVal _t3484 = (*gem_v_tmp);
    GemVal gem_v_t = _t3484.fn(_t3484.env, NULL, 0);
    GemVal _t3485 = gem_v_r;
    static GemICacheSlot _ic_512 = {0};
    GemVal _t3486 = gem_v_r;
    static GemICacheSlot _ic_513 = {0};
    GemVal _t3487[] = {gem_v_line_dir, gem_table_get_cached(_t3485, "setup", &_ic_512), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3486, "expr", &_ic_513), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
    GemVal _t3488 = gem_interp(14, _t3487);
    gem_pop_frame();
    return _t3488;
}

struct _closure__anon_48 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts_tail;
};
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1241 "compiler/main.gem"
    GemVal _t3490[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3490, 1);
#line 1242 "compiler/main.gem"
    GemVal _t3491 = gem_v_node;
    static GemICacheSlot _ic_514 = {0};
    GemVal _t3492[] = {gem_table_get_cached(_t3491, "cond", &_ic_514)};
    GemVal _t3493 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3493.fn(_t3493.env, _t3492, 1);
#line 1243 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1244 "compiler/main.gem"
    GemVal _t3494 = gem_v_cond_r;
    static GemICacheSlot _ic_515 = {0};
    GemVal _t3495[] = {gem_v_b, gem_table_get_cached(_t3494, "setup", &_ic_515)};
    (void)(gem_buf_push_fn(NULL, _t3495, 2));
#line 1245 "compiler/main.gem"
    GemVal _t3496 = gem_v_cond_r;
    static GemICacheSlot _ic_516 = {0};
    GemVal _t3497[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get_cached(_t3496, "expr", &_ic_516), gem_string(")) {\n")};
    GemVal _t3498[] = {gem_v_b, gem_interp(4, _t3497)};
    (void)(gem_buf_push_fn(NULL, _t3498, 2));
#line 1246 "compiler/main.gem"
    GemVal _t3499 = gem_v_node;
    static GemICacheSlot _ic_517 = {0};
    GemVal _t3500[] = {gem_table_get_cached(_t3499, "then", &_ic_517), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3501 = (*gem_v_compile_stmts_tail);
    GemVal _t3502[] = {gem_v_b, _t3501.fn(_t3501.env, _t3500, 3)};
    (void)(gem_buf_push_fn(NULL, _t3502, 2));
#line 1247 "compiler/main.gem"
    GemVal _t3503 = gem_v_node;
    static GemICacheSlot _ic_518 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3503, "else", &_ic_518), GEM_NIL))) {
#line 1248 "compiler/main.gem"
    GemVal _t3504[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3505[] = {gem_v_b, gem_interp(2, _t3504)};
        (void)(gem_buf_push_fn(NULL, _t3505, 2));
#line 1249 "compiler/main.gem"
    GemVal _t3506 = gem_v_node;
    static GemICacheSlot _ic_519 = {0};
    GemVal _t3507[] = {gem_table_get_cached(_t3506, "else", &_ic_519), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3508 = (*gem_v_compile_stmts_tail);
    GemVal _t3509[] = {gem_v_b, _t3508.fn(_t3508.env, _t3507, 3)};
        (void)(gem_buf_push_fn(NULL, _t3509, 2));
    } else {
#line 1250 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1251 "compiler/main.gem"
    GemVal _t3510[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3511[] = {gem_v_b, gem_interp(2, _t3510)};
            (void)(gem_buf_push_fn(NULL, _t3511, 2));
#line 1252 "compiler/main.gem"
    GemVal _t3512 = gem_table_new();
    GemVal _t3513[] = {_t3512, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3514 = (*gem_v_compile_stmts_tail);
    GemVal _t3515[] = {gem_v_b, _t3514.fn(_t3514.env, _t3513, 3)};
            (void)(gem_buf_push_fn(NULL, _t3515, 2));
        }
    }
#line 1254 "compiler/main.gem"
    GemVal _t3516[] = {gem_v_p, gem_string("}")};
    GemVal _t3517[] = {gem_v_b, gem_interp(2, _t3516)};
    (void)(gem_buf_push_fn(NULL, _t3517, 2));
#line 1255 "compiler/main.gem"
    GemVal _t3518[] = {gem_v_b};
    GemVal _t3519 = gem_buf_str_fn(NULL, _t3518, 1);
    gem_pop_frame();
    return _t3519;
}

struct _closure__anon_49 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1261 "compiler/main.gem"
    GemVal _t3521[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3521, 1);
#line 1262 "compiler/main.gem"
    GemVal _t3522 = gem_v_node;
    static GemICacheSlot _ic_520 = {0};
    GemVal _t3523[] = {gem_table_get_cached(_t3522, "cond", &_ic_520)};
    GemVal _t3524 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3524.fn(_t3524.env, _t3523, 1);
#line 1263 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1264 "compiler/main.gem"
    GemVal _t3525[] = {gem_v_p, gem_string("while (1) {\n")};
    GemVal _t3526[] = {gem_v_b, gem_interp(2, _t3525)};
    (void)(gem_buf_push_fn(NULL, _t3526, 2));
#line 1265 "compiler/main.gem"
    GemVal _t3527[] = {gem_v_p, gem_string("    gem_yield_check();\n")};
    GemVal _t3528[] = {gem_v_b, gem_interp(2, _t3527)};
    (void)(gem_buf_push_fn(NULL, _t3528, 2));
#line 1266 "compiler/main.gem"
    GemVal _t3529 = gem_v_cond_r;
    static GemICacheSlot _ic_521 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3529, "setup", &_ic_521), gem_string("")))) {
#line 1267 "compiler/main.gem"
    GemVal _t3530 = gem_v_cond_r;
    static GemICacheSlot _ic_522 = {0};
    GemVal _t3531[] = {gem_v_p, gem_string("    ")};
    GemVal _t3532[] = {gem_table_get_cached(_t3530, "setup", &_ic_522), gem_string("    "), gem_interp(2, _t3531)};
    GemVal _t3533[] = {gem_v_b, gem_str_replace_fn(NULL, _t3532, 3)};
        (void)(gem_buf_push_fn(NULL, _t3533, 2));
    }
#line 1269 "compiler/main.gem"
    GemVal _t3534 = gem_v_cond_r;
    static GemICacheSlot _ic_523 = {0};
    GemVal _t3535[] = {gem_v_p, gem_string("    if (!gem_truthy("), gem_table_get_cached(_t3534, "expr", &_ic_523), gem_string(")) break;\n")};
    GemVal _t3536[] = {gem_v_b, gem_interp(4, _t3535)};
    (void)(gem_buf_push_fn(NULL, _t3536, 2));
#line 1270 "compiler/main.gem"
    GemVal _t3537 = gem_v_node;
    static GemICacheSlot _ic_524 = {0};
    GemVal _t3538[] = {gem_table_get_cached(_t3537, "body", &_ic_524), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3539 = (*gem_v_compile_stmts);
    GemVal _t3540[] = {gem_v_b, _t3539.fn(_t3539.env, _t3538, 2)};
    (void)(gem_buf_push_fn(NULL, _t3540, 2));
#line 1271 "compiler/main.gem"
    GemVal _t3541[] = {gem_v_p, gem_string("}")};
    GemVal _t3542[] = {gem_v_b, gem_interp(2, _t3541)};
    (void)(gem_buf_push_fn(NULL, _t3542, 2));
#line 1272 "compiler/main.gem"
    GemVal _t3543[] = {gem_v_b};
    GemVal _t3544 = gem_buf_str_fn(NULL, _t3543, 1);
    gem_pop_frame();
    return _t3544;
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
#line 1278 "compiler/main.gem"
    GemVal _t3546[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3546, 1);
#line 1279 "compiler/main.gem"
    GemVal _t3547 = gem_v_node;
    static GemICacheSlot _ic_525 = {0};
    GemVal _t3548[] = {gem_table_get_cached(_t3547, "target", &_ic_525)};
    GemVal _t3549 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3549.fn(_t3549.env, _t3548, 1);
#line 1280 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1281 "compiler/main.gem"
    GemVal _t3550 = gem_v_node;
    static GemICacheSlot _ic_526 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3550, "target_var", &_ic_526), GEM_NIL))) {
#line 1282 "compiler/main.gem"
    GemVal _t3551 = gem_v_node;
    static GemICacheSlot _ic_527 = {0};
    GemVal _t3552[] = {gem_table_get_cached(_t3551, "target_var", &_ic_527)};
    GemVal _t3553 = (*gem_v_mangle);
        gem_v_t = _t3553.fn(_t3553.env, _t3552, 1);
    } else {
#line 1284 "compiler/main.gem"
    GemVal _t3554 = (*gem_v_tmp);
        gem_v_t = _t3554.fn(_t3554.env, NULL, 0);
    }
#line 1286 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1287 "compiler/main.gem"
    GemVal _t3555 = gem_v_target_r;
    static GemICacheSlot _ic_528 = {0};
    GemVal _t3556[] = {gem_v_b, gem_table_get_cached(_t3555, "setup", &_ic_528)};
    (void)(gem_buf_push_fn(NULL, _t3556, 2));
#line 1288 "compiler/main.gem"
    GemVal _t3557 = gem_v_target_r;
    static GemICacheSlot _ic_529 = {0};
    GemVal _t3558[] = {gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3557, "expr", &_ic_529), gem_string(";\n")};
    GemVal _t3559[] = {gem_v_b, gem_interp(6, _t3558)};
    (void)(gem_buf_push_fn(NULL, _t3559, 2));
#line 1289 "compiler/main.gem"
    GemVal _t3560 = gem_v_node;
    static GemICacheSlot _ic_530 = {0};
    GemVal _t3561[] = {gem_table_get_cached(_t3560, "whens", &_ic_530)};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3561, 1), gem_int(0)))) {
#line 1290 "compiler/main.gem"
    GemVal _t3562 = gem_v_node;
    static GemICacheSlot _ic_531 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t3562, "else", &_ic_531), GEM_NIL))) {
#line 1291 "compiler/main.gem"
    GemVal _t3563 = gem_v_node;
    static GemICacheSlot _ic_532 = {0};
    GemVal _t3564[] = {gem_table_get_cached(_t3563, "else", &_ic_532), gem_v_indent, gem_v_return_pos};
    GemVal _t3565 = (*gem_v_compile_stmts_tail);
    GemVal _t3566[] = {gem_v_b, _t3565.fn(_t3565.env, _t3564, 3)};
            (void)(gem_buf_push_fn(NULL, _t3566, 2));
        } else {
#line 1292 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1293 "compiler/main.gem"
    GemVal _t3567[] = {gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;\n")};
    GemVal _t3568[] = {gem_v_b, gem_interp(4, _t3567)};
                (void)(gem_buf_push_fn(NULL, _t3568, 2));
            }
        }
#line 1295 "compiler/main.gem"
    GemVal _t3569[] = {gem_v_p, gem_string("(void)"), gem_v_t, gem_string(";")};
    GemVal _t3570[] = {gem_v_b, gem_interp(4, _t3569)};
        (void)(gem_buf_push_fn(NULL, _t3570, 2));
#line 1296 "compiler/main.gem"
    GemVal _t3571[] = {gem_v_b};
        GemVal _t3572 = gem_buf_str_fn(NULL, _t3571, 1);
        gem_pop_frame();
        return _t3572;
    }
#line 1298 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1298 "compiler/main.gem"
    GemVal _t3573 = gem_v_node;
    static GemICacheSlot _ic_533 = {0};
    GemVal _t3574[] = {gem_table_get_cached(_t3573, "whens", &_ic_533)};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3574, 1);
#line 1298 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1298 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1298 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1299 "compiler/main.gem"
    GemVal _t3575 = gem_v_node;
    static GemICacheSlot _ic_534 = {0};
        GemVal gem_v_w = gem_table_get(gem_table_get_cached(_t3575, "whens", &_ic_534), gem_v_i);
#line 1300 "compiler/main.gem"
    GemVal _t3576 = gem_v_w;
    static GemICacheSlot _ic_535 = {0};
    GemVal _t3577[] = {gem_table_get_cached(_t3576, "value", &_ic_535)};
    GemVal _t3578 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3578.fn(_t3578.env, _t3577, 1);
#line 1301 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1302 "compiler/main.gem"
    GemVal _t3579 = gem_v_val_r;
    static GemICacheSlot _ic_536 = {0};
    GemVal _t3580[] = {gem_v_b, gem_table_get_cached(_t3579, "setup", &_ic_536)};
            (void)(gem_buf_push_fn(NULL, _t3580, 2));
#line 1303 "compiler/main.gem"
    GemVal _t3581 = gem_v_val_r;
    static GemICacheSlot _ic_537 = {0};
    GemVal _t3582[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get_cached(_t3581, "expr", &_ic_537), gem_string(")) {\n")};
    GemVal _t3583[] = {gem_v_b, gem_interp(4, _t3582)};
            (void)(gem_buf_push_fn(NULL, _t3583, 2));
        } else {
#line 1305 "compiler/main.gem"
    GemVal _t3584[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3585[] = {gem_v_b, gem_interp(2, _t3584)};
            (void)(gem_buf_push_fn(NULL, _t3585, 2));
#line 1306 "compiler/main.gem"
    GemVal _t3586 = gem_v_val_r;
    static GemICacheSlot _ic_538 = {0};
    GemVal _t3587[] = {gem_v_b, gem_table_get_cached(_t3586, "setup", &_ic_538)};
            (void)(gem_buf_push_fn(NULL, _t3587, 2));
#line 1307 "compiler/main.gem"
    GemVal _t3588 = gem_v_val_r;
    static GemICacheSlot _ic_539 = {0};
    GemVal _t3589[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get_cached(_t3588, "expr", &_ic_539), gem_string(")) {\n")};
    GemVal _t3590[] = {gem_v_b, gem_interp(4, _t3589)};
            (void)(gem_buf_push_fn(NULL, _t3590, 2));
        }
#line 1309 "compiler/main.gem"
    GemVal _t3591 = gem_v_w;
    static GemICacheSlot _ic_540 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t3591, "bindings", &_ic_540), GEM_NIL))) {
#line 1310 "compiler/main.gem"
    GemVal _t3592 = gem_v_w;
    static GemICacheSlot _ic_541 = {0};
            GemVal gem_v__for_items_64 = gem_table_get_cached(_t3592, "bindings", &_ic_541);
#line 1310 "compiler/main.gem"
            GemVal gem_v__for_i_64 = gem_int(0);
#line 1310 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3593[] = {gem_v__for_items_64};
                if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_len_fn(NULL, _t3593, 1)))) break;
#line 1310 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_64, gem_v__for_i_64);
#line 1310 "compiler/main.gem"
                gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1311 "compiler/main.gem"
    GemVal _t3594[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3595 = (*gem_v_compile_stmt);
    GemVal _t3596[] = {gem_v_b, _t3595.fn(_t3595.env, _t3594, 2)};
                (void)(gem_buf_push_fn(NULL, _t3596, 2));
#line 1312 "compiler/main.gem"
    GemVal _t3597[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3597, 2));
            }

        }
#line 1315 "compiler/main.gem"
    GemVal _t3598 = gem_v_w;
    static GemICacheSlot _ic_542 = {0};
    GemVal _t3599[] = {gem_table_get_cached(_t3598, "body", &_ic_542), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3600 = (*gem_v_compile_stmts_tail);
    GemVal _t3601[] = {gem_v_b, _t3600.fn(_t3600.env, _t3599, 3)};
        (void)(gem_buf_push_fn(NULL, _t3601, 2));
    }

#line 1317 "compiler/main.gem"
    GemVal _t3602 = gem_v_node;
    static GemICacheSlot _ic_543 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3602, "else", &_ic_543), GEM_NIL))) {
#line 1318 "compiler/main.gem"
    GemVal _t3603[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3604[] = {gem_v_b, gem_interp(2, _t3603)};
        (void)(gem_buf_push_fn(NULL, _t3604, 2));
#line 1319 "compiler/main.gem"
    GemVal _t3605 = gem_v_node;
    static GemICacheSlot _ic_544 = {0};
    GemVal _t3606[] = {gem_table_get_cached(_t3605, "else", &_ic_544), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3607 = (*gem_v_compile_stmts_tail);
    GemVal _t3608[] = {gem_v_b, _t3607.fn(_t3607.env, _t3606, 3)};
        (void)(gem_buf_push_fn(NULL, _t3608, 2));
    } else {
#line 1320 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1321 "compiler/main.gem"
    GemVal _t3609[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3610[] = {gem_v_b, gem_interp(2, _t3609)};
            (void)(gem_buf_push_fn(NULL, _t3610, 2));
#line 1322 "compiler/main.gem"
    GemVal _t3611 = gem_table_new();
    GemVal _t3612[] = {_t3611, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3613 = (*gem_v_compile_stmts_tail);
    GemVal _t3614[] = {gem_v_b, _t3613.fn(_t3613.env, _t3612, 3)};
            (void)(gem_buf_push_fn(NULL, _t3614, 2));
        }
    }
#line 1324 "compiler/main.gem"
    GemVal _t3615[] = {gem_v_p, gem_string("}")};
    GemVal _t3616[] = {gem_v_b, gem_interp(2, _t3615)};
    (void)(gem_buf_push_fn(NULL, _t3616, 2));
#line 1325 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(1);
#line 1325 "compiler/main.gem"
    GemVal _t3617 = gem_v_node;
    static GemICacheSlot _ic_545 = {0};
    GemVal _t3618[] = {gem_table_get_cached(_t3617, "whens", &_ic_545)};
    GemVal gem_v__for_limit_66 = gem_len_fn(NULL, _t3618, 1);
#line 1325 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_v__for_limit_66))) break;
#line 1325 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_66;
#line 1325 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1326 "compiler/main.gem"
    GemVal _t3619[] = {gem_string("\n"), gem_v_p, gem_string("}")};
    GemVal _t3620[] = {gem_v_b, gem_interp(3, _t3619)};
        (void)(gem_buf_push_fn(NULL, _t3620, 2));
    }

#line 1328 "compiler/main.gem"
    GemVal _t3621[] = {gem_v_b};
    GemVal _t3622 = gem_buf_str_fn(NULL, _t3621, 1);
    gem_pop_frame();
    return _t3622;
}

struct _closure__anon_51 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_51(void *_env, GemVal *args, int argc) {
    struct _closure__anon_51 *_cls = (struct _closure__anon_51 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_51", "compiler/main.gem", 0);
#line 1334 "compiler/main.gem"
    GemVal _t3624[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3624, 1);
#line 1335 "compiler/main.gem"
    GemVal _t3625 = gem_v_node;
    static GemICacheSlot _ic_546 = {0};
    GemVal gem_v_arms = gem_table_get_cached(_t3625, "arms", &_ic_546);
#line 1336 "compiler/main.gem"
    GemVal _t3626 = gem_v_node;
    static GemICacheSlot _ic_547 = {0};
    GemVal gem_v_after_ms = gem_table_get_cached(_t3626, "after_ms", &_ic_547);
#line 1337 "compiler/main.gem"
    GemVal _t3627 = gem_v_node;
    static GemICacheSlot _ic_548 = {0};
    GemVal gem_v_after_body = gem_table_get_cached(_t3627, "after_body", &_ic_548);
#line 1339 "compiler/main.gem"
    GemVal _t3628 = gem_v_node;
    static GemICacheSlot _ic_549 = {0};
    GemVal _t3629[] = {gem_table_get_cached(_t3628, "recv_var", &_ic_549)};
    GemVal _t3630 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3630.fn(_t3630.env, _t3629, 1);
#line 1340 "compiler/main.gem"
    GemVal _t3631 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3631.fn(_t3631.env, NULL, 0);
#line 1341 "compiler/main.gem"
    GemVal _t3632 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3632.fn(_t3632.env, NULL, 0);
#line 1342 "compiler/main.gem"
    GemVal _t3633 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3633.fn(_t3633.env, NULL, 0);
#line 1343 "compiler/main.gem"
    GemVal _t3634 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3634.fn(_t3634.env, NULL, 0);
#line 1344 "compiler/main.gem"
    GemVal _t3635 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3635.fn(_t3635.env, NULL, 0);
#line 1346 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1348 "compiler/main.gem"
    GemVal _t3636[] = {gem_v_p, gem_string("GemVal "), gem_v_recv_tmp, gem_string(" = GEM_NIL;\n")};
    GemVal _t3637[] = {gem_v_b, gem_interp(4, _t3636)};
    (void)(gem_buf_push_fn(NULL, _t3637, 2));
#line 1349 "compiler/main.gem"
    GemVal _t3638[] = {gem_v_p, gem_string("int "), gem_v_matched_tmp, gem_string(" = -1;\n")};
    GemVal _t3639[] = {gem_v_b, gem_interp(4, _t3638)};
    (void)(gem_buf_push_fn(NULL, _t3639, 2));
#line 1351 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1352 "compiler/main.gem"
    GemVal _t3640[] = {gem_v_after_ms};
    GemVal _t3641 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3641.fn(_t3641.env, _t3640, 1);
#line 1353 "compiler/main.gem"
    GemVal _t3642 = gem_v_ms_r;
    static GemICacheSlot _ic_550 = {0};
    GemVal _t3643[] = {gem_v_b, gem_table_get_cached(_t3642, "setup", &_ic_550)};
        (void)(gem_buf_push_fn(NULL, _t3643, 2));
#line 1354 "compiler/main.gem"
    GemVal _t3644 = gem_v_ms_r;
    static GemICacheSlot _ic_551 = {0};
    GemVal _t3645[] = {gem_v_p, gem_string("int64_t "), gem_v_deadline_tmp, gem_string(" = gem_now_ms() + (int64_t)("), gem_table_get_cached(_t3644, "expr", &_ic_551), gem_string(").ival;\n")};
    GemVal _t3646[] = {gem_v_b, gem_interp(6, _t3645)};
        (void)(gem_buf_push_fn(NULL, _t3646, 2));
    }
#line 1357 "compiler/main.gem"
    GemVal _t3647[] = {gem_v_p, gem_string("while (1) {\n")};
    GemVal _t3648[] = {gem_v_b, gem_interp(2, _t3647)};
    (void)(gem_buf_push_fn(NULL, _t3648, 2));
#line 1358 "compiler/main.gem"
    GemVal _t3649[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn__mod_codegen_pad_for(NULL, _t3649, 1);
#line 1360 "compiler/main.gem"
    GemVal _t3650[] = {gem_v_p2, gem_string("GemProcess *"), gem_v_proc_tmp, gem_string(" = &gem_proc_table[gem_current_pid];\n")};
    GemVal _t3651[] = {gem_v_b, gem_interp(4, _t3650)};
    (void)(gem_buf_push_fn(NULL, _t3651, 2));
#line 1361 "compiler/main.gem"
    GemVal _t3652[] = {gem_v_p2, gem_string("GemMsgNode *"), gem_v_prev_tmp, gem_string(" = NULL;\n")};
    GemVal _t3653[] = {gem_v_b, gem_interp(4, _t3652)};
    (void)(gem_buf_push_fn(NULL, _t3653, 2));
#line 1362 "compiler/main.gem"
    GemVal _t3654[] = {gem_v_p2, gem_string("GemMsgNode *"), gem_v_cur_tmp, gem_string(" = "), gem_v_proc_tmp, gem_string("->mailbox.head;\n")};
    GemVal _t3655[] = {gem_v_b, gem_interp(6, _t3654)};
    (void)(gem_buf_push_fn(NULL, _t3655, 2));
#line 1363 "compiler/main.gem"
    GemVal _t3656[] = {gem_v_p2, gem_v_matched_tmp, gem_string(" = -1;\n")};
    GemVal _t3657[] = {gem_v_b, gem_interp(3, _t3656)};
    (void)(gem_buf_push_fn(NULL, _t3657, 2));
#line 1364 "compiler/main.gem"
    GemVal _t3658[] = {gem_v_p2, gem_string("while ("), gem_v_cur_tmp, gem_string(") {\n")};
    GemVal _t3659[] = {gem_v_b, gem_interp(4, _t3658)};
    (void)(gem_buf_push_fn(NULL, _t3659, 2));
#line 1365 "compiler/main.gem"
    GemVal _t3660[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn__mod_codegen_pad_for(NULL, _t3660, 1);
#line 1366 "compiler/main.gem"
    GemVal _t3661[] = {gem_v_p3, gem_v_recv_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string("->value;\n")};
    GemVal _t3662[] = {gem_v_b, gem_interp(5, _t3661)};
    (void)(gem_buf_push_fn(NULL, _t3662, 2));
#line 1368 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1368 "compiler/main.gem"
    GemVal _t3663[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3663, 1);
#line 1368 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1368 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1368 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1369 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1370 "compiler/main.gem"
    GemVal _t3664 = gem_v_arm;
    static GemICacheSlot _ic_552 = {0};
    GemVal _t3665 = gem_table_get_cached(_t3664, "pattern", &_ic_552);
    static GemICacheSlot _ic_553 = {0};
    GemVal _t3666[] = {gem_table_get_cached(_t3665, "condition", &_ic_553)};
    GemVal _t3667 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3667.fn(_t3667.env, _t3666, 1);
#line 1371 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1372 "compiler/main.gem"
    GemVal _t3668 = gem_v_cond_r;
    static GemICacheSlot _ic_554 = {0};
    GemVal _t3669[] = {gem_v_b, gem_table_get_cached(_t3668, "setup", &_ic_554)};
            (void)(gem_buf_push_fn(NULL, _t3669, 2));
#line 1373 "compiler/main.gem"
    GemVal _t3670 = gem_v_cond_r;
    static GemICacheSlot _ic_555 = {0};
    GemVal _t3671[] = {gem_v_p3, gem_string("if (gem_truthy("), gem_table_get_cached(_t3670, "expr", &_ic_555), gem_string(")) {\n")};
    GemVal _t3672[] = {gem_v_b, gem_interp(4, _t3671)};
            (void)(gem_buf_push_fn(NULL, _t3672, 2));
        } else {
#line 1375 "compiler/main.gem"
    GemVal _t3673[] = {gem_v_p3, gem_string("} else {\n")};
    GemVal _t3674[] = {gem_v_b, gem_interp(2, _t3673)};
            (void)(gem_buf_push_fn(NULL, _t3674, 2));
#line 1376 "compiler/main.gem"
    GemVal _t3675 = gem_v_cond_r;
    static GemICacheSlot _ic_556 = {0};
    GemVal _t3676[] = {gem_v_b, gem_table_get_cached(_t3675, "setup", &_ic_556)};
            (void)(gem_buf_push_fn(NULL, _t3676, 2));
#line 1377 "compiler/main.gem"
    GemVal _t3677 = gem_v_cond_r;
    static GemICacheSlot _ic_557 = {0};
    GemVal _t3678[] = {gem_v_p3, gem_string("if (gem_truthy("), gem_table_get_cached(_t3677, "expr", &_ic_557), gem_string(")) {\n")};
    GemVal _t3679[] = {gem_v_b, gem_interp(4, _t3678)};
            (void)(gem_buf_push_fn(NULL, _t3679, 2));
        }
#line 1379 "compiler/main.gem"
    GemVal _t3680[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3681[] = {gem_fn__mod_codegen_pad_for(NULL, _t3680, 1), gem_v_matched_tmp, gem_string(" = "), gem_v_i, gem_string(";\n")};
    GemVal _t3682[] = {gem_v_b, gem_interp(5, _t3681)};
        (void)(gem_buf_push_fn(NULL, _t3682, 2));
    }

#line 1381 "compiler/main.gem"
    GemVal gem_v__for_i_68 = gem_int(0);
#line 1381 "compiler/main.gem"
    GemVal _t3683[] = {gem_v_arms};
    GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t3683, 1);
#line 1381 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1381 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_68;
#line 1381 "compiler/main.gem"
        gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1382 "compiler/main.gem"
    GemVal _t3684[] = {gem_v_p3, gem_string("}")};
    GemVal _t3685[] = {gem_v_b, gem_interp(2, _t3684)};
        (void)(gem_buf_push_fn(NULL, _t3685, 2));
#line 1383 "compiler/main.gem"
    GemVal _t3686[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3686, 1), gem_int(1))))) {
#line 1384 "compiler/main.gem"
    GemVal _t3687[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3687, 2));
        }
    }

#line 1387 "compiler/main.gem"
    GemVal _t3688[] = {gem_v_b, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t3688, 2));
#line 1389 "compiler/main.gem"
    GemVal _t3689[] = {gem_v_p3, gem_string("if ("), gem_v_matched_tmp, gem_string(" >= 0) {\n")};
    GemVal _t3690[] = {gem_v_b, gem_interp(4, _t3689)};
    (void)(gem_buf_push_fn(NULL, _t3690, 2));
#line 1390 "compiler/main.gem"
    GemVal _t3691[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn__mod_codegen_pad_for(NULL, _t3691, 1);
#line 1391 "compiler/main.gem"
    GemVal _t3692[] = {gem_v_p4, gem_string("gem_mailbox_remove(&"), gem_v_proc_tmp, gem_string("->mailbox, "), gem_v_prev_tmp, gem_string(", "), gem_v_cur_tmp, gem_string(");\n")};
    GemVal _t3693[] = {gem_v_b, gem_interp(8, _t3692)};
    (void)(gem_buf_push_fn(NULL, _t3693, 2));
#line 1392 "compiler/main.gem"
    GemVal _t3694[] = {gem_v_p4, gem_string("break;\n")};
    GemVal _t3695[] = {gem_v_b, gem_interp(2, _t3694)};
    (void)(gem_buf_push_fn(NULL, _t3695, 2));
#line 1393 "compiler/main.gem"
    GemVal _t3696[] = {gem_v_p3, gem_string("}\n")};
    GemVal _t3697[] = {gem_v_b, gem_interp(2, _t3696)};
    (void)(gem_buf_push_fn(NULL, _t3697, 2));
#line 1395 "compiler/main.gem"
    GemVal _t3698[] = {gem_v_p3, gem_v_prev_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string(";\n")};
    GemVal _t3699[] = {gem_v_b, gem_interp(5, _t3698)};
    (void)(gem_buf_push_fn(NULL, _t3699, 2));
#line 1396 "compiler/main.gem"
    GemVal _t3700[] = {gem_v_p3, gem_v_cur_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string("->next;\n")};
    GemVal _t3701[] = {gem_v_b, gem_interp(5, _t3700)};
    (void)(gem_buf_push_fn(NULL, _t3701, 2));
#line 1397 "compiler/main.gem"
    GemVal _t3702[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3703[] = {gem_v_b, gem_interp(2, _t3702)};
    (void)(gem_buf_push_fn(NULL, _t3703, 2));
#line 1399 "compiler/main.gem"
    GemVal _t3704[] = {gem_v_p2, gem_string("if ("), gem_v_matched_tmp, gem_string(" >= 0) break;\n")};
    GemVal _t3705[] = {gem_v_b, gem_interp(4, _t3704)};
    (void)(gem_buf_push_fn(NULL, _t3705, 2));
#line 1400 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1401 "compiler/main.gem"
    GemVal _t3706[] = {gem_v_p2, gem_string("if (gem_now_ms() >= "), gem_v_deadline_tmp, gem_string(") {\n")};
    GemVal _t3707[] = {gem_v_b, gem_interp(4, _t3706)};
        (void)(gem_buf_push_fn(NULL, _t3707, 2));
#line 1402 "compiler/main.gem"
    GemVal _t3708[] = {gem_v_p2, gem_string("    "), gem_v_matched_tmp, gem_string(" = -2;\n")};
    GemVal _t3709[] = {gem_v_b, gem_interp(4, _t3708)};
        (void)(gem_buf_push_fn(NULL, _t3709, 2));
#line 1403 "compiler/main.gem"
    GemVal _t3710[] = {gem_v_p2, gem_string("    break;\n")};
    GemVal _t3711[] = {gem_v_b, gem_interp(2, _t3710)};
        (void)(gem_buf_push_fn(NULL, _t3711, 2));
#line 1404 "compiler/main.gem"
    GemVal _t3712[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3713[] = {gem_v_b, gem_interp(2, _t3712)};
        (void)(gem_buf_push_fn(NULL, _t3713, 2));
#line 1405 "compiler/main.gem"
    GemVal _t3714[] = {gem_v_p2, gem_string("gem_selective_yield("), gem_v_deadline_tmp, gem_string(");\n")};
    GemVal _t3715[] = {gem_v_b, gem_interp(4, _t3714)};
        (void)(gem_buf_push_fn(NULL, _t3715, 2));
#line 1406 "compiler/main.gem"
    GemVal _t3716[] = {gem_v_p2, gem_string("if ("), gem_v_proc_tmp, gem_string("->timed_out) {\n")};
    GemVal _t3717[] = {gem_v_b, gem_interp(4, _t3716)};
        (void)(gem_buf_push_fn(NULL, _t3717, 2));
#line 1407 "compiler/main.gem"
    GemVal _t3718[] = {gem_v_p2, gem_string("    "), gem_v_matched_tmp, gem_string(" = -2;\n")};
    GemVal _t3719[] = {gem_v_b, gem_interp(4, _t3718)};
        (void)(gem_buf_push_fn(NULL, _t3719, 2));
#line 1408 "compiler/main.gem"
    GemVal _t3720[] = {gem_v_p2, gem_string("    break;\n")};
    GemVal _t3721[] = {gem_v_b, gem_interp(2, _t3720)};
        (void)(gem_buf_push_fn(NULL, _t3721, 2));
#line 1409 "compiler/main.gem"
    GemVal _t3722[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3723[] = {gem_v_b, gem_interp(2, _t3722)};
        (void)(gem_buf_push_fn(NULL, _t3723, 2));
    } else {
#line 1411 "compiler/main.gem"
    GemVal _t3724[] = {gem_v_p2, gem_string("gem_selective_yield(-1);\n")};
    GemVal _t3725[] = {gem_v_b, gem_interp(2, _t3724)};
        (void)(gem_buf_push_fn(NULL, _t3725, 2));
    }
#line 1414 "compiler/main.gem"
    GemVal _t3726[] = {gem_v_p, gem_string("}\n")};
    GemVal _t3727[] = {gem_v_b, gem_interp(2, _t3726)};
    (void)(gem_buf_push_fn(NULL, _t3727, 2));
#line 1417 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1417 "compiler/main.gem"
    GemVal _t3728[] = {gem_v_arms};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3728, 1);
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
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1420 "compiler/main.gem"
    GemVal _t3729[] = {gem_v_p, gem_string("if ("), gem_v_matched_tmp, gem_string(" == "), gem_v_i, gem_string(") {\n")};
    GemVal _t3730[] = {gem_v_b, gem_interp(6, _t3729)};
            (void)(gem_buf_push_fn(NULL, _t3730, 2));
        } else {
#line 1422 "compiler/main.gem"
    GemVal _t3731[] = {gem_v_p, gem_string("} else if ("), gem_v_matched_tmp, gem_string(" == "), gem_v_i, gem_string(") {\n")};
    GemVal _t3732[] = {gem_v_b, gem_interp(6, _t3731)};
            (void)(gem_buf_push_fn(NULL, _t3732, 2));
        }
#line 1424 "compiler/main.gem"
    GemVal _t3733 = gem_v_arm;
    static GemICacheSlot _ic_558 = {0};
    GemVal _t3734 = gem_table_get_cached(_t3733, "pattern", &_ic_558);
    static GemICacheSlot _ic_559 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t3734, "bindings", &_ic_559), GEM_NIL))) {
#line 1425 "compiler/main.gem"
    GemVal _t3735 = gem_v_arm;
    static GemICacheSlot _ic_560 = {0};
    GemVal _t3736 = gem_table_get_cached(_t3735, "pattern", &_ic_560);
    static GemICacheSlot _ic_561 = {0};
            GemVal gem_v__for_items_69 = gem_table_get_cached(_t3736, "bindings", &_ic_561);
#line 1425 "compiler/main.gem"
            GemVal gem_v__for_i_69 = gem_int(0);
#line 1425 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3737[] = {gem_v__for_items_69};
                if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_len_fn(NULL, _t3737, 1)))) break;
#line 1425 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_69, gem_v__for_i_69);
#line 1425 "compiler/main.gem"
                gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1426 "compiler/main.gem"
    GemVal _t3738[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3739 = (*gem_v_compile_stmt);
    GemVal _t3740[] = {gem_v_b, _t3739.fn(_t3739.env, _t3738, 2)};
                (void)(gem_buf_push_fn(NULL, _t3740, 2));
#line 1427 "compiler/main.gem"
    GemVal _t3741[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3741, 2));
            }

        }
#line 1430 "compiler/main.gem"
    GemVal _t3742 = gem_v_arm;
    static GemICacheSlot _ic_562 = {0};
    GemVal _t3743[] = {gem_table_get_cached(_t3742, "body", &_ic_562), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3744 = (*gem_v_compile_stmts_tail);
    GemVal _t3745[] = {gem_v_b, _t3744.fn(_t3744.env, _t3743, 3)};
        (void)(gem_buf_push_fn(NULL, _t3745, 2));
    }

#line 1434 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1435 "compiler/main.gem"
    GemVal _t3746[] = {gem_v_p, gem_string("} else if ("), gem_v_matched_tmp, gem_string(" == -2) {\n")};
    GemVal _t3747[] = {gem_v_b, gem_interp(4, _t3746)};
        (void)(gem_buf_push_fn(NULL, _t3747, 2));
#line 1436 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1437 "compiler/main.gem"
    GemVal _t3748[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3749 = (*gem_v_compile_stmts_tail);
    GemVal _t3750[] = {gem_v_b, _t3749.fn(_t3749.env, _t3748, 3)};
            (void)(gem_buf_push_fn(NULL, _t3750, 2));
        } else {
#line 1438 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1439 "compiler/main.gem"
    GemVal _t3751 = gem_table_new();
    GemVal _t3752[] = {_t3751, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3753 = (*gem_v_compile_stmts_tail);
    GemVal _t3754[] = {gem_v_b, _t3753.fn(_t3753.env, _t3752, 3)};
                (void)(gem_buf_push_fn(NULL, _t3754, 2));
            }
        }
    } else {
#line 1441 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1442 "compiler/main.gem"
    GemVal _t3755[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3756[] = {gem_v_b, gem_interp(2, _t3755)};
            (void)(gem_buf_push_fn(NULL, _t3756, 2));
#line 1443 "compiler/main.gem"
    GemVal _t3757 = gem_table_new();
    GemVal _t3758[] = {_t3757, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3759 = (*gem_v_compile_stmts_tail);
    GemVal _t3760[] = {gem_v_b, _t3759.fn(_t3759.env, _t3758, 3)};
            (void)(gem_buf_push_fn(NULL, _t3760, 2));
        }
    }
#line 1446 "compiler/main.gem"
    GemVal _t3761[] = {gem_v_p, gem_string("}")};
    GemVal _t3762[] = {gem_v_b, gem_interp(2, _t3761)};
    (void)(gem_buf_push_fn(NULL, _t3762, 2));
#line 1447 "compiler/main.gem"
    GemVal _t3763[] = {gem_v_b};
    GemVal _t3764 = gem_buf_str_fn(NULL, _t3763, 1);
    gem_pop_frame();
    return _t3764;
}

struct _closure__anon_52 {
    GemVal *gem_v_extern_c_decls;
};
static GemVal _anon_52(void *_env, GemVal *args, int argc) {
    struct _closure__anon_52 *_cls = (struct _closure__anon_52 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_52", "compiler/main.gem", 0);
#line 1454 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1455 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1456 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1457 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1458 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1459 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1460 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1461 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1462 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1463 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1464 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1465 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1466 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1469 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1470 "compiler/main.gem"
    GemVal _t3766[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3766, 1), gem_int(0)))) {
#line 1471 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1473 "compiler/main.gem"
        GemVal gem_v__for_i_71 = gem_int(0);
#line 1473 "compiler/main.gem"
    GemVal _t3767[] = {gem_v_params};
        GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3767, 1);
#line 1473 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1473 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_71;
#line 1473 "compiler/main.gem"
            gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1474 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1475 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1477 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1478 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1479 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1480 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1481 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1482 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1483 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1484 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1485 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1486 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1487 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1488 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1489 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1491 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1496 "compiler/main.gem"
    GemVal _t3768[] = {gem_v_c_ret, gem_string(" "), gem_v_name, gem_string("("), gem_v_c_params, gem_string(");")};
    GemVal _t3769[] = {(*gem_v_extern_c_decls), gem_interp(6, _t3768)};
    GemVal _t3770 = gem_push_fn(NULL, _t3769, 2);
    gem_pop_frame();
    return _t3770;
}

struct _closure__anon_53 {
    GemVal *gem_v_compile_blocking_extern_fn;
    GemVal *gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_53(void *_env, GemVal *args, int argc) {
    struct _closure__anon_53 *_cls = (struct _closure__anon_53 *)_env;
    GemVal *gem_v_compile_blocking_extern_fn = _cls->gem_v_compile_blocking_extern_fn;
    GemVal *gem_v_emit_extern_c_decl = _cls->gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_53", "compiler/main.gem", 0);
#line 1500 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1500 "compiler/main.gem"
    GemVal _t3772 = gem_v__d72;
    static GemICacheSlot _ic_563 = {0};
    GemVal gem_v_name = gem_table_get_cached(_t3772, "name", &_ic_563);
#line 1500 "compiler/main.gem"
    GemVal _t3773 = gem_v__d72;
    static GemICacheSlot _ic_564 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t3773, "params", &_ic_564);
#line 1500 "compiler/main.gem"
    GemVal _t3774 = gem_v__d72;
    static GemICacheSlot _ic_565 = {0};
    GemVal gem_v_ret_type = gem_table_get_cached(_t3774, "ret_type", &_ic_565);

#line 1502 "compiler/main.gem"
    GemVal _t3775 = gem_v_node;
    static GemICacheSlot _ic_566 = {0};
    if (gem_truthy(gem_table_get_cached(_t3775, "blocking", &_ic_566))) {
#line 1503 "compiler/main.gem"
    GemVal _t3776[] = {gem_v_node};
    GemVal _t3777 = (*gem_v_compile_blocking_extern_fn);
        GemVal _t3778 = _t3777.fn(_t3777.env, _t3776, 1);
        gem_pop_frame();
        return _t3778;
    }
#line 1506 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1507 "compiler/main.gem"
    GemVal _t3779[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3780[] = {gem_v_b, gem_interp(3, _t3779)};
    (void)(gem_buf_push_fn(NULL, _t3780, 2));
#line 1509 "compiler/main.gem"
    GemVal _t3781 = gem_table_new();
    GemVal gem_v_c_args = _t3781;
#line 1510 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1510 "compiler/main.gem"
    GemVal _t3782[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t3782, 1);
#line 1510 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1510 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1510 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1511 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1512 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1513 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1514 "compiler/main.gem"
    GemVal _t3783[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3784[] = {gem_v_b, gem_interp(5, _t3783)};
            (void)(gem_buf_push_fn(NULL, _t3784, 2));
#line 1515 "compiler/main.gem"
    GemVal _t3785[] = {gem_string("_p"), gem_v_i};
    GemVal _t3786[] = {gem_v_c_args, gem_interp(2, _t3785)};
            (void)(gem_push_fn(NULL, _t3786, 2));
        } else {
#line 1516 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1517 "compiler/main.gem"
    GemVal _t3787[] = {gem_string("    double _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].fval;\n")};
    GemVal _t3788[] = {gem_v_b, gem_interp(5, _t3787)};
                (void)(gem_buf_push_fn(NULL, _t3788, 2));
#line 1518 "compiler/main.gem"
    GemVal _t3789[] = {gem_string("_p"), gem_v_i};
    GemVal _t3790[] = {gem_v_c_args, gem_interp(2, _t3789)};
                (void)(gem_push_fn(NULL, _t3790, 2));
            } else {
#line 1519 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1520 "compiler/main.gem"
    GemVal _t3791[] = {gem_string("    char* _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].sval;\n")};
    GemVal _t3792[] = {gem_v_b, gem_interp(5, _t3791)};
                    (void)(gem_buf_push_fn(NULL, _t3792, 2));
#line 1521 "compiler/main.gem"
    GemVal _t3793[] = {gem_string("_p"), gem_v_i};
    GemVal _t3794[] = {gem_v_c_args, gem_interp(2, _t3793)};
                    (void)(gem_push_fn(NULL, _t3794, 2));
                } else {
#line 1522 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1523 "compiler/main.gem"
    GemVal _t3795[] = {gem_string("    int _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].bval;\n")};
    GemVal _t3796[] = {gem_v_b, gem_interp(5, _t3795)};
                        (void)(gem_buf_push_fn(NULL, _t3796, 2));
#line 1524 "compiler/main.gem"
    GemVal _t3797[] = {gem_string("_p"), gem_v_i};
    GemVal _t3798[] = {gem_v_c_args, gem_interp(2, _t3797)};
                        (void)(gem_push_fn(NULL, _t3798, 2));
                    } else {
#line 1525 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1526 "compiler/main.gem"
    GemVal _t3799[] = {gem_string("    void* _p"), gem_v_i, gem_string(" = (void*)(intptr_t)args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3800[] = {gem_v_b, gem_interp(5, _t3799)};
                            (void)(gem_buf_push_fn(NULL, _t3800, 2));
#line 1527 "compiler/main.gem"
    GemVal _t3801[] = {gem_string("_p"), gem_v_i};
    GemVal _t3802[] = {gem_v_c_args, gem_interp(2, _t3801)};
                            (void)(gem_push_fn(NULL, _t3802, 2));
                        } else {
#line 1528 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1529 "compiler/main.gem"
    GemVal _t3803[] = {gem_string("args["), gem_v_i, gem_string("]")};
    GemVal _t3804[] = {gem_v_c_args, gem_interp(3, _t3803)};
                                (void)(gem_push_fn(NULL, _t3804, 2));
                            } else {
#line 1531 "compiler/main.gem"
    GemVal _t3805[] = {gem_string("args["), gem_v_i, gem_string("]")};
    GemVal _t3806[] = {gem_v_c_args, gem_interp(3, _t3805)};
                                (void)(gem_push_fn(NULL, _t3806, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1536 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1537 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1537 "compiler/main.gem"
    GemVal _t3807[] = {gem_v_c_args};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3807, 1);
#line 1537 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1537 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1537 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1538 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1539 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1541 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1544 "compiler/main.gem"
    GemVal _t3808[] = {gem_v_name, gem_string("("), gem_v_call_args, gem_string(")")};
    GemVal gem_v_call_expr = gem_interp(4, _t3808);
#line 1546 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1547 "compiler/main.gem"
    GemVal _t3809[] = {gem_string("    "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3810[] = {gem_v_b, gem_interp(3, _t3809)};
        (void)(gem_buf_push_fn(NULL, _t3810, 2));
#line 1548 "compiler/main.gem"
    GemVal _t3811[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3811, 2));
    } else {
#line 1549 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1550 "compiler/main.gem"
    GemVal _t3812[] = {gem_string("    return "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3813[] = {gem_v_b, gem_interp(3, _t3812)};
            (void)(gem_buf_push_fn(NULL, _t3813, 2));
        } else {
#line 1551 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1552 "compiler/main.gem"
    GemVal _t3814[] = {gem_string("    int64_t _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3815[] = {gem_v_b, gem_interp(3, _t3814)};
                (void)(gem_buf_push_fn(NULL, _t3815, 2));
#line 1553 "compiler/main.gem"
    GemVal _t3816[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3816, 2));
            } else {
#line 1554 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1555 "compiler/main.gem"
    GemVal _t3817[] = {gem_string("    double _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3818[] = {gem_v_b, gem_interp(3, _t3817)};
                    (void)(gem_buf_push_fn(NULL, _t3818, 2));
#line 1556 "compiler/main.gem"
    GemVal _t3819[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3819, 2));
                } else {
#line 1557 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1558 "compiler/main.gem"
    GemVal _t3820[] = {gem_string("    char* _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3821[] = {gem_v_b, gem_interp(3, _t3820)};
                        (void)(gem_buf_push_fn(NULL, _t3821, 2));
#line 1559 "compiler/main.gem"
    GemVal _t3822[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3822, 2));
                    } else {
#line 1560 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1561 "compiler/main.gem"
    GemVal _t3823[] = {gem_string("    int _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3824[] = {gem_v_b, gem_interp(3, _t3823)};
                            (void)(gem_buf_push_fn(NULL, _t3824, 2));
#line 1562 "compiler/main.gem"
    GemVal _t3825[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                            (void)(gem_buf_push_fn(NULL, _t3825, 2));
                        } else {
#line 1563 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1564 "compiler/main.gem"
    GemVal _t3826[] = {gem_string("    void *_ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3827[] = {gem_v_b, gem_interp(3, _t3826)};
                                (void)(gem_buf_push_fn(NULL, _t3827, 2));
#line 1565 "compiler/main.gem"
    GemVal _t3828[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3828, 2));
                            } else {
#line 1567 "compiler/main.gem"
    GemVal _t3829[] = {gem_string("    "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3830[] = {gem_v_b, gem_interp(3, _t3829)};
                                (void)(gem_buf_push_fn(NULL, _t3830, 2));
#line 1568 "compiler/main.gem"
    GemVal _t3831[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3831, 2));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1571 "compiler/main.gem"
    GemVal _t3832[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3832, 2));
#line 1573 "compiler/main.gem"
    GemVal _t3833[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc);")};
    GemVal _t3834[] = {(*gem_v_forward_decls), gem_interp(3, _t3833)};
    (void)(gem_push_fn(NULL, _t3834, 2));
#line 1574 "compiler/main.gem"
    GemVal _t3835[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3836 = (*gem_v_emit_extern_c_decl);
    (void)(_t3836.fn(_t3836.env, _t3835, 3));
#line 1576 "compiler/main.gem"
    GemVal _t3837[] = {gem_v_b};
    GemVal _t3838 = gem_buf_str_fn(NULL, _t3837, 1);
    gem_pop_frame();
    return _t3838;
}

struct _closure__anon_54 {
    GemVal *gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_54(void *_env, GemVal *args, int argc) {
    struct _closure__anon_54 *_cls = (struct _closure__anon_54 *)_env;
    GemVal *gem_v_emit_extern_c_decl = _cls->gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_54", "compiler/main.gem", 0);
#line 1583 "compiler/main.gem"
    GemVal gem_v__d75 = gem_v_node;
#line 1583 "compiler/main.gem"
    GemVal _t3840 = gem_v__d75;
    static GemICacheSlot _ic_567 = {0};
    GemVal gem_v_name = gem_table_get_cached(_t3840, "name", &_ic_567);
#line 1583 "compiler/main.gem"
    GemVal _t3841 = gem_v__d75;
    static GemICacheSlot _ic_568 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t3841, "params", &_ic_568);
#line 1583 "compiler/main.gem"
    GemVal _t3842 = gem_v__d75;
    static GemICacheSlot _ic_569 = {0};
    GemVal gem_v_ret_type = gem_table_get_cached(_t3842, "ret_type", &_ic_569);

#line 1584 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1587 "compiler/main.gem"
    GemVal _t3843[] = {gem_string("struct _blk_"), gem_v_name, gem_string("_args {\n")};
    GemVal _t3844[] = {gem_v_b, gem_interp(3, _t3843)};
    (void)(gem_buf_push_fn(NULL, _t3844, 2));
#line 1588 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1588 "compiler/main.gem"
    GemVal _t3845[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t3845, 1);
#line 1588 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1588 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1588 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1589 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1590 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1591 "compiler/main.gem"
    GemVal _t3846[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3847[] = {gem_v_b, gem_interp(3, _t3846)};
            (void)(gem_buf_push_fn(NULL, _t3847, 2));
        } else {
#line 1592 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1593 "compiler/main.gem"
    GemVal _t3848[] = {gem_string("    double _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3849[] = {gem_v_b, gem_interp(3, _t3848)};
                (void)(gem_buf_push_fn(NULL, _t3849, 2));
            } else {
#line 1594 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1595 "compiler/main.gem"
    GemVal _t3850[] = {gem_string("    char *_p"), gem_v_i, gem_string(";\n")};
    GemVal _t3851[] = {gem_v_b, gem_interp(3, _t3850)};
                    (void)(gem_buf_push_fn(NULL, _t3851, 2));
                } else {
#line 1596 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1597 "compiler/main.gem"
    GemVal _t3852[] = {gem_string("    int _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3853[] = {gem_v_b, gem_interp(3, _t3852)};
                        (void)(gem_buf_push_fn(NULL, _t3853, 2));
                    } else {
#line 1598 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1599 "compiler/main.gem"
    GemVal _t3854[] = {gem_string("    void *_p"), gem_v_i, gem_string(";\n")};
    GemVal _t3855[] = {gem_v_b, gem_interp(3, _t3854)};
                            (void)(gem_buf_push_fn(NULL, _t3855, 2));
                        }
                    }
                }
            }
        }
    }

#line 1602 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1603 "compiler/main.gem"
    GemVal _t3856[] = {gem_v_b, gem_string("    int64_t _result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3856, 2));
    } else {
#line 1604 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1605 "compiler/main.gem"
    GemVal _t3857[] = {gem_v_b, gem_string("    double _result;\n")};
            (void)(gem_buf_push_fn(NULL, _t3857, 2));
        } else {
#line 1606 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1607 "compiler/main.gem"
    GemVal _t3858[] = {gem_v_b, gem_string("    char *_result;\n")};
                (void)(gem_buf_push_fn(NULL, _t3858, 2));
            } else {
#line 1608 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1609 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_b, gem_string("    int _result;\n")};
                    (void)(gem_buf_push_fn(NULL, _t3859, 2));
                } else {
#line 1610 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1611 "compiler/main.gem"
    GemVal _t3860[] = {gem_v_b, gem_string("    void *_result;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3860, 2));
                    }
                }
            }
        }
    }
#line 1613 "compiler/main.gem"
    GemVal _t3861[] = {gem_v_b, gem_string("};\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3861, 2));
#line 1616 "compiler/main.gem"
    GemVal _t3862[] = {gem_string("static void _blk_"), gem_v_name, gem_string("_worker(void *_a) {\n")};
    GemVal _t3863[] = {gem_v_b, gem_interp(3, _t3862)};
    (void)(gem_buf_push_fn(NULL, _t3863, 2));
#line 1617 "compiler/main.gem"
    GemVal _t3864[] = {gem_string("    struct _blk_"), gem_v_name, gem_string("_args *a = (struct _blk_"), gem_v_name, gem_string("_args *)_a;\n")};
    GemVal _t3865[] = {gem_v_b, gem_interp(5, _t3864)};
    (void)(gem_buf_push_fn(NULL, _t3865, 2));
#line 1619 "compiler/main.gem"
    GemVal gem_v_w_args = gem_string("");
#line 1620 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1620 "compiler/main.gem"
    GemVal _t3866[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t3866, 1);
#line 1620 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1620 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1620 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1621 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1622 "compiler/main.gem"
            gem_v_w_args = gem_add(gem_v_w_args, gem_string(", "));
        }
#line 1624 "compiler/main.gem"
    GemVal _t3867[] = {gem_string("a->_p"), gem_v_i};
        gem_v_w_args = gem_add(gem_v_w_args, gem_interp(2, _t3867));
    }

#line 1626 "compiler/main.gem"
    GemVal _t3868[] = {gem_v_name, gem_string("("), gem_v_w_args, gem_string(")")};
    GemVal gem_v_w_call = gem_interp(4, _t3868);
#line 1628 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1629 "compiler/main.gem"
    GemVal _t3869[] = {gem_string("    "), gem_v_w_call, gem_string(";\n")};
    GemVal _t3870[] = {gem_v_b, gem_interp(3, _t3869)};
        (void)(gem_buf_push_fn(NULL, _t3870, 2));
    } else {
#line 1631 "compiler/main.gem"
    GemVal _t3871[] = {gem_string("    a->_result = "), gem_v_w_call, gem_string(";\n")};
    GemVal _t3872[] = {gem_v_b, gem_interp(3, _t3871)};
        (void)(gem_buf_push_fn(NULL, _t3872, 2));
    }
#line 1633 "compiler/main.gem"
    GemVal _t3873[] = {gem_v_b, gem_string("}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3873, 2));
#line 1636 "compiler/main.gem"
    GemVal _t3874[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3875[] = {gem_v_b, gem_interp(3, _t3874)};
    (void)(gem_buf_push_fn(NULL, _t3875, 2));
#line 1639 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1639 "compiler/main.gem"
    GemVal _t3876[] = {gem_v_params};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t3876, 1);
#line 1639 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1639 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1639 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1640 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1641 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1642 "compiler/main.gem"
    GemVal _t3877[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3878[] = {gem_v_b, gem_interp(5, _t3877)};
            (void)(gem_buf_push_fn(NULL, _t3878, 2));
        } else {
#line 1643 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1644 "compiler/main.gem"
    GemVal _t3879[] = {gem_string("    double _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].fval;\n")};
    GemVal _t3880[] = {gem_v_b, gem_interp(5, _t3879)};
                (void)(gem_buf_push_fn(NULL, _t3880, 2));
            } else {
#line 1645 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1646 "compiler/main.gem"
    GemVal _t3881[] = {gem_string("    char* _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].sval;\n")};
    GemVal _t3882[] = {gem_v_b, gem_interp(5, _t3881)};
                    (void)(gem_buf_push_fn(NULL, _t3882, 2));
                } else {
#line 1647 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1648 "compiler/main.gem"
    GemVal _t3883[] = {gem_string("    int _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].bval;\n")};
    GemVal _t3884[] = {gem_v_b, gem_interp(5, _t3883)};
                        (void)(gem_buf_push_fn(NULL, _t3884, 2));
                    } else {
#line 1649 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1650 "compiler/main.gem"
    GemVal _t3885[] = {gem_string("    void* _p"), gem_v_i, gem_string(" = (void*)(intptr_t)args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3886[] = {gem_v_b, gem_interp(5, _t3885)};
                            (void)(gem_buf_push_fn(NULL, _t3886, 2));
                        }
                    }
                }
            }
        }
    }

#line 1655 "compiler/main.gem"
    GemVal _t3887[] = {gem_v_b, gem_string("    if (gem_current_pid >= 0) {\n")};
    (void)(gem_buf_push_fn(NULL, _t3887, 2));
#line 1656 "compiler/main.gem"
    GemVal _t3888[] = {gem_string("        struct _blk_"), gem_v_name, gem_string("_args *_ba = (struct _blk_"), gem_v_name, gem_string("_args *)malloc(sizeof(struct _blk_"), gem_v_name, gem_string("_args));\n")};
    GemVal _t3889[] = {gem_v_b, gem_interp(7, _t3888)};
    (void)(gem_buf_push_fn(NULL, _t3889, 2));
#line 1659 "compiler/main.gem"
    GemVal gem_v__for_i_79 = gem_int(0);
#line 1659 "compiler/main.gem"
    GemVal _t3890[] = {gem_v_params};
    GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t3890, 1);
#line 1659 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1659 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_79;
#line 1659 "compiler/main.gem"
        gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1660 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1661 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1662 "compiler/main.gem"
    GemVal _t3891[] = {gem_string("        _ba->_p"), gem_v_i, gem_string(" = strdup(_p"), gem_v_i, gem_string(");\n")};
    GemVal _t3892[] = {gem_v_b, gem_interp(5, _t3891)};
            (void)(gem_buf_push_fn(NULL, _t3892, 2));
        } else {
#line 1664 "compiler/main.gem"
    GemVal _t3893[] = {gem_string("        _ba->_p"), gem_v_i, gem_string(" = _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3894[] = {gem_v_b, gem_interp(5, _t3893)};
            (void)(gem_buf_push_fn(NULL, _t3894, 2));
        }
    }

#line 1668 "compiler/main.gem"
    GemVal _t3895[] = {gem_string("        GemIORequest *_req = gem_io_submit_extern(_blk_"), gem_v_name, gem_string("_worker, _ba);\n")};
    GemVal _t3896[] = {gem_v_b, gem_interp(3, _t3895)};
    (void)(gem_buf_push_fn(NULL, _t3896, 2));
#line 1669 "compiler/main.gem"
    GemVal _t3897[] = {gem_string("        if (!_req) { free(_ba); gem_error(\""), gem_v_name, gem_string(": I/O queue full\"); }\n")};
    GemVal _t3898[] = {gem_v_b, gem_interp(3, _t3897)};
    (void)(gem_buf_push_fn(NULL, _t3898, 2));
#line 1670 "compiler/main.gem"
    GemVal _t3899[] = {gem_v_b, gem_string("        GemProcess *_proc = &gem_proc_table[gem_current_pid];\n")};
    (void)(gem_buf_push_fn(NULL, _t3899, 2));
#line 1671 "compiler/main.gem"
    GemVal _t3900[] = {gem_v_b, gem_string("        _proc->io_request = _req;\n")};
    (void)(gem_buf_push_fn(NULL, _t3900, 2));
#line 1672 "compiler/main.gem"
    GemVal _t3901[] = {gem_v_b, gem_string("        gem_io_pool_yield();\n")};
    (void)(gem_buf_push_fn(NULL, _t3901, 2));
#line 1673 "compiler/main.gem"
    GemVal _t3902[] = {gem_v_b, gem_string("        _proc->io_request = NULL;\n")};
    (void)(gem_buf_push_fn(NULL, _t3902, 2));
#line 1676 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1677 "compiler/main.gem"
    GemVal _t3903[] = {gem_v_b, gem_string("        char *_raw = _ba->_result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3903, 2));
    } else {
#line 1678 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_ret_type, gem_string("Nil")))) {
#line 1679 "compiler/main.gem"
            GemVal gem_v_c_type = gem_string("int64_t");
#line 1680 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1681 "compiler/main.gem"
                gem_v_c_type = gem_string("double");
            } else {
#line 1682 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1683 "compiler/main.gem"
                    gem_v_c_type = gem_string("int");
                } else {
#line 1684 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1685 "compiler/main.gem"
                        gem_v_c_type = gem_string("void *");
                    }
                }
            }
#line 1687 "compiler/main.gem"
    GemVal _t3904[] = {gem_string("        "), gem_v_c_type, gem_string(" _ret = _ba->_result;\n")};
    GemVal _t3905[] = {gem_v_b, gem_interp(3, _t3904)};
            (void)(gem_buf_push_fn(NULL, _t3905, 2));
        }
    }
#line 1691 "compiler/main.gem"
    GemVal gem_v__for_i_80 = gem_int(0);
#line 1691 "compiler/main.gem"
    GemVal _t3906[] = {gem_v_params};
    GemVal gem_v__for_limit_80 = gem_len_fn(NULL, _t3906, 1);
#line 1691 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1691 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_80;
#line 1691 "compiler/main.gem"
        gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1692 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type")), gem_string("String")))) {
#line 1693 "compiler/main.gem"
    GemVal _t3907[] = {gem_string("        free(_ba->_p"), gem_v_i, gem_string(");\n")};
    GemVal _t3908[] = {gem_v_b, gem_interp(3, _t3907)};
            (void)(gem_buf_push_fn(NULL, _t3908, 2));
        }
    }

#line 1697 "compiler/main.gem"
    GemVal _t3909[] = {gem_v_b, gem_string("        free(_ba);\n")};
    (void)(gem_buf_push_fn(NULL, _t3909, 2));
#line 1698 "compiler/main.gem"
    GemVal _t3910[] = {gem_v_b, gem_string("        gem_io_free_request(_req);\n")};
    (void)(gem_buf_push_fn(NULL, _t3910, 2));
#line 1701 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1702 "compiler/main.gem"
    GemVal _t3911[] = {gem_v_b, gem_string("        return gem_int(_ret);\n")};
        (void)(gem_buf_push_fn(NULL, _t3911, 2));
    } else {
#line 1703 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1704 "compiler/main.gem"
    GemVal _t3912[] = {gem_v_b, gem_string("        return gem_float(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3912, 2));
        } else {
#line 1705 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1706 "compiler/main.gem"
    GemVal _t3913[] = {gem_v_b, gem_string("        size_t _len = _raw ? strlen(_raw) : 0;\n")};
                (void)(gem_buf_push_fn(NULL, _t3913, 2));
#line 1707 "compiler/main.gem"
    GemVal _t3914[] = {gem_v_b, gem_string("        char *_gc = (char *)GC_MALLOC_ATOMIC(_len + 1);\n")};
                (void)(gem_buf_push_fn(NULL, _t3914, 2));
#line 1708 "compiler/main.gem"
    GemVal _t3915[] = {gem_v_b, gem_string("        if (_raw) { memcpy(_gc, _raw, _len + 1); free(_raw); } else { _gc[0] = '\\0'; }\n")};
                (void)(gem_buf_push_fn(NULL, _t3915, 2));
#line 1709 "compiler/main.gem"
    GemVal _t3916[] = {gem_v_b, gem_string("        return gem_string(_gc);\n")};
                (void)(gem_buf_push_fn(NULL, _t3916, 2));
            } else {
#line 1710 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1711 "compiler/main.gem"
    GemVal _t3917[] = {gem_v_b, gem_string("        return gem_bool(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3917, 2));
                } else {
#line 1712 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1713 "compiler/main.gem"
    GemVal _t3918[] = {gem_v_b, gem_string("        GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3918, 2));
                    } else {
#line 1715 "compiler/main.gem"
    GemVal _t3919[] = {gem_v_b, gem_string("        return GEM_NIL;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3919, 2));
                    }
                }
            }
        }
    }
#line 1717 "compiler/main.gem"
    GemVal _t3920[] = {gem_v_b, gem_string("    }\n")};
    (void)(gem_buf_push_fn(NULL, _t3920, 2));
#line 1720 "compiler/main.gem"
    GemVal gem_v_d_args = gem_string("");
#line 1721 "compiler/main.gem"
    GemVal gem_v__for_i_81 = gem_int(0);
#line 1721 "compiler/main.gem"
    GemVal _t3921[] = {gem_v_params};
    GemVal gem_v__for_limit_81 = gem_len_fn(NULL, _t3921, 1);
#line 1721 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_81, gem_v__for_limit_81))) break;
#line 1721 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_81;
#line 1721 "compiler/main.gem"
        gem_v__for_i_81 = gem_add(gem_v__for_i_81, gem_int(1));
#line 1722 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1723 "compiler/main.gem"
            gem_v_d_args = gem_add(gem_v_d_args, gem_string(", "));
        }
#line 1725 "compiler/main.gem"
    GemVal _t3922[] = {gem_string("_p"), gem_v_i};
        gem_v_d_args = gem_add(gem_v_d_args, gem_interp(2, _t3922));
    }

#line 1727 "compiler/main.gem"
    GemVal _t3923[] = {gem_v_name, gem_string("("), gem_v_d_args, gem_string(")")};
    GemVal gem_v_d_call = gem_interp(4, _t3923);
#line 1729 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1730 "compiler/main.gem"
    GemVal _t3924[] = {gem_string("    "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3925[] = {gem_v_b, gem_interp(3, _t3924)};
        (void)(gem_buf_push_fn(NULL, _t3925, 2));
#line 1731 "compiler/main.gem"
    GemVal _t3926[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3926, 2));
    } else {
#line 1732 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1733 "compiler/main.gem"
    GemVal _t3927[] = {gem_string("    int64_t _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3928[] = {gem_v_b, gem_interp(3, _t3927)};
            (void)(gem_buf_push_fn(NULL, _t3928, 2));
#line 1734 "compiler/main.gem"
    GemVal _t3929[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3929, 2));
        } else {
#line 1735 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1736 "compiler/main.gem"
    GemVal _t3930[] = {gem_string("    double _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3931[] = {gem_v_b, gem_interp(3, _t3930)};
                (void)(gem_buf_push_fn(NULL, _t3931, 2));
#line 1737 "compiler/main.gem"
    GemVal _t3932[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3932, 2));
            } else {
#line 1738 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1739 "compiler/main.gem"
    GemVal _t3933[] = {gem_string("    char* _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3934[] = {gem_v_b, gem_interp(3, _t3933)};
                    (void)(gem_buf_push_fn(NULL, _t3934, 2));
#line 1740 "compiler/main.gem"
    GemVal _t3935[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3935, 2));
                } else {
#line 1741 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1742 "compiler/main.gem"
    GemVal _t3936[] = {gem_string("    int _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3937[] = {gem_v_b, gem_interp(3, _t3936)};
                        (void)(gem_buf_push_fn(NULL, _t3937, 2));
#line 1743 "compiler/main.gem"
    GemVal _t3938[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3938, 2));
                    } else {
#line 1744 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1745 "compiler/main.gem"
    GemVal _t3939[] = {gem_string("    void *_ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3940[] = {gem_v_b, gem_interp(3, _t3939)};
                            (void)(gem_buf_push_fn(NULL, _t3940, 2));
#line 1746 "compiler/main.gem"
    GemVal _t3941[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3941, 2));
                        } else {
#line 1748 "compiler/main.gem"
    GemVal _t3942[] = {gem_string("    "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3943[] = {gem_v_b, gem_interp(3, _t3942)};
                            (void)(gem_buf_push_fn(NULL, _t3943, 2));
#line 1749 "compiler/main.gem"
    GemVal _t3944[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3944, 2));
                        }
                    }
                }
            }
        }
    }
#line 1752 "compiler/main.gem"
    GemVal _t3945[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3945, 2));
#line 1754 "compiler/main.gem"
    GemVal _t3946[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc);")};
    GemVal _t3947[] = {(*gem_v_forward_decls), gem_interp(3, _t3946)};
    (void)(gem_push_fn(NULL, _t3947, 2));
#line 1755 "compiler/main.gem"
    GemVal _t3948[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3949 = (*gem_v_emit_extern_c_decl);
    (void)(_t3949.fn(_t3949.env, _t3948, 3));
#line 1757 "compiler/main.gem"
    GemVal _t3950[] = {gem_v_b};
    GemVal _t3951 = gem_buf_str_fn(NULL, _t3950, 1);
    gem_pop_frame();
    return _t3951;
}

struct _closure__anon_55 {
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_mangle;
};
static GemVal _anon_55(void *_env, GemVal *args, int argc) {
    struct _closure__anon_55 *_cls = (struct _closure__anon_55 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_cap_set = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_55", "compiler/main.gem", 0);
#line 1763 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1764 "compiler/main.gem"
    GemVal _t3953 = gem_v_node;
    static GemICacheSlot _ic_570 = {0};
    GemVal gem_v_defaults = gem_table_get_cached(_t3953, "defaults", &_ic_570);
#line 1765 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_defaults, GEM_NIL))) {
#line 1766 "compiler/main.gem"
    GemVal _t3954 = gem_table_new();
        gem_v_defaults = _t3954;
    }
#line 1768 "compiler/main.gem"
    GemVal gem_v__for_i_82 = gem_int(0);
#line 1768 "compiler/main.gem"
    GemVal _t3955[] = {gem_v_params};
    GemVal gem_v__for_limit_82 = gem_len_fn(NULL, _t3955, 1);
#line 1768 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_v__for_limit_82))) break;
#line 1768 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_82;
#line 1768 "compiler/main.gem"
        gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1769 "compiler/main.gem"
    GemVal _t3956[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3957 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3957.fn(_t3957.env, _t3956, 1);
#line 1770 "compiler/main.gem"
    GemVal _t3958[] = {gem_v_defaults};
    GemVal _t3959;
    if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t3958, 1)))) {
        _t3959 = gem_lt(gem_v_i, gem_len_fn(NULL, _t3958, 1));
    } else {
        _t3959 = gem_neq(gem_table_get(gem_v_defaults, gem_v_i), GEM_NIL);
    }
        GemVal gem_v_has_def = _t3959;
#line 1771 "compiler/main.gem"
        if (gem_truthy(gem_v_has_def)) {
#line 1772 "compiler/main.gem"
    GemVal _t3960[] = {gem_table_get(gem_v_defaults, gem_v_i)};
    GemVal _t3961 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3961.fn(_t3961.env, _t3960, 1);
#line 1773 "compiler/main.gem"
    GemVal _t3962[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3962, 2))) {
#line 1774 "compiler/main.gem"
    GemVal _t3963[] = {gem_string("    GemVal *"), gem_v_mp, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3964[] = {gem_v_b, gem_interp(3, _t3963)};
                (void)(gem_buf_push_fn(NULL, _t3964, 2));
#line 1775 "compiler/main.gem"
    GemVal _t3965[] = {gem_string("    if (argc > "), gem_v_i, gem_string(") { *"), gem_v_mp, gem_string(" = args["), gem_v_i, gem_string("]; } else {\n")};
    GemVal _t3966[] = {gem_v_b, gem_interp(7, _t3965)};
                (void)(gem_buf_push_fn(NULL, _t3966, 2));
#line 1776 "compiler/main.gem"
    GemVal _t3967 = gem_v_r;
    static GemICacheSlot _ic_571 = {0};
    GemVal _t3968[] = {gem_v_b, gem_table_get_cached(_t3967, "setup", &_ic_571)};
                (void)(gem_buf_push_fn(NULL, _t3968, 2));
#line 1777 "compiler/main.gem"
    GemVal _t3969 = gem_v_r;
    static GemICacheSlot _ic_572 = {0};
    GemVal _t3970[] = {gem_string("    *"), gem_v_mp, gem_string(" = "), gem_table_get_cached(_t3969, "expr", &_ic_572), gem_string(";\n    }\n")};
    GemVal _t3971[] = {gem_v_b, gem_interp(5, _t3970)};
                (void)(gem_buf_push_fn(NULL, _t3971, 2));
            } else {
#line 1779 "compiler/main.gem"
    GemVal _t3972[] = {gem_string("    GemVal "), gem_v_mp, gem_string(";\n")};
    GemVal _t3973[] = {gem_v_b, gem_interp(3, _t3972)};
                (void)(gem_buf_push_fn(NULL, _t3973, 2));
#line 1780 "compiler/main.gem"
    GemVal _t3974[] = {gem_string("    if (argc > "), gem_v_i, gem_string(") { "), gem_v_mp, gem_string(" = args["), gem_v_i, gem_string("]; } else {\n")};
    GemVal _t3975[] = {gem_v_b, gem_interp(7, _t3974)};
                (void)(gem_buf_push_fn(NULL, _t3975, 2));
#line 1781 "compiler/main.gem"
    GemVal _t3976 = gem_v_r;
    static GemICacheSlot _ic_573 = {0};
    GemVal _t3977[] = {gem_v_b, gem_table_get_cached(_t3976, "setup", &_ic_573)};
                (void)(gem_buf_push_fn(NULL, _t3977, 2));
#line 1782 "compiler/main.gem"
    GemVal _t3978 = gem_v_r;
    static GemICacheSlot _ic_574 = {0};
    GemVal _t3979[] = {gem_string("    "), gem_v_mp, gem_string(" = "), gem_table_get_cached(_t3978, "expr", &_ic_574), gem_string(";\n    }\n")};
    GemVal _t3980[] = {gem_v_b, gem_interp(5, _t3979)};
                (void)(gem_buf_push_fn(NULL, _t3980, 2));
            }
        } else {
#line 1785 "compiler/main.gem"
    GemVal _t3981[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3981, 2))) {
#line 1786 "compiler/main.gem"
    GemVal _t3982[] = {gem_string("    GemVal *"), gem_v_mp, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3983[] = {gem_v_b, gem_interp(3, _t3982)};
                (void)(gem_buf_push_fn(NULL, _t3983, 2));
#line 1787 "compiler/main.gem"
    GemVal _t3984[] = {gem_string("    *"), gem_v_mp, gem_string(" = (argc > "), gem_v_i, gem_string(") ? args["), gem_v_i, gem_string("] : GEM_NIL;\n")};
    GemVal _t3985[] = {gem_v_b, gem_interp(7, _t3984)};
                (void)(gem_buf_push_fn(NULL, _t3985, 2));
            } else {
#line 1789 "compiler/main.gem"
    GemVal _t3986[] = {gem_string("    GemVal "), gem_v_mp, gem_string(" = (argc > "), gem_v_i, gem_string(") ? args["), gem_v_i, gem_string("] : GEM_NIL;\n")};
    GemVal _t3987[] = {gem_v_b, gem_interp(7, _t3986)};
                (void)(gem_buf_push_fn(NULL, _t3987, 2));
            }
        }
    }

#line 1795 "compiler/main.gem"
    GemVal _t3988 = gem_v_node;
    static GemICacheSlot _ic_575 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3988, "rest_param", &_ic_575), GEM_NIL))) {
#line 1796 "compiler/main.gem"
    GemVal _t3989[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t3989, 1);
#line 1797 "compiler/main.gem"
    GemVal _t3990 = gem_v_node;
    static GemICacheSlot _ic_576 = {0};
    GemVal _t3991[] = {gem_table_get_cached(_t3990, "rest_param", &_ic_576)};
    GemVal _t3992 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t3992.fn(_t3992.env, _t3991, 1);
#line 1798 "compiler/main.gem"
    GemVal _t3993 = gem_v_node;
    static GemICacheSlot _ic_577 = {0};
    GemVal _t3994[] = {gem_v_cap_set, gem_table_get_cached(_t3993, "rest_param", &_ic_577)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3994, 2))) {
#line 1799 "compiler/main.gem"
    GemVal _t3995[] = {gem_string("    GemVal *"), gem_v_mp_rest, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3996[] = {gem_v_b, gem_interp(3, _t3995)};
            (void)(gem_buf_push_fn(NULL, _t3996, 2));
#line 1800 "compiler/main.gem"
    GemVal _t3997[] = {gem_string("    *"), gem_v_mp_rest, gem_string(" = gem_table_new();\n")};
    GemVal _t3998[] = {gem_v_b, gem_interp(3, _t3997)};
            (void)(gem_buf_push_fn(NULL, _t3998, 2));
#line 1801 "compiler/main.gem"
    GemVal _t3999[] = {gem_string("    for (int _ri = "), gem_v_N, gem_string("; _ri < argc; _ri++) gem_table_set(*"), gem_v_mp_rest, gem_string(", gem_int(_ri - "), gem_v_N, gem_string("), args[_ri]);\n")};
    GemVal _t4000[] = {gem_v_b, gem_interp(7, _t3999)};
            (void)(gem_buf_push_fn(NULL, _t4000, 2));
        } else {
#line 1803 "compiler/main.gem"
    GemVal _t4001[] = {gem_string("    GemVal "), gem_v_mp_rest, gem_string(" = gem_table_new();\n")};
    GemVal _t4002[] = {gem_v_b, gem_interp(3, _t4001)};
            (void)(gem_buf_push_fn(NULL, _t4002, 2));
#line 1804 "compiler/main.gem"
    GemVal _t4003[] = {gem_string("    for (int _ri = "), gem_v_N, gem_string("; _ri < argc; _ri++) gem_table_set("), gem_v_mp_rest, gem_string(", gem_int(_ri - "), gem_v_N, gem_string("), args[_ri]);\n")};
    GemVal _t4004[] = {gem_v_b, gem_interp(7, _t4003)};
            (void)(gem_buf_push_fn(NULL, _t4004, 2));
        }
    }
#line 1807 "compiler/main.gem"
    GemVal _t4005[] = {gem_v_b};
    GemVal _t4006 = gem_buf_str_fn(NULL, _t4005, 1);
    gem_pop_frame();
    return _t4006;
}

struct _closure__anon_56 {
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_56(void *_env, GemVal *args, int argc) {
    struct _closure__anon_56 *_cls = (struct _closure__anon_56 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_56", "compiler/main.gem", 0);
#line 1813 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1814 "compiler/main.gem"
    GemVal _t4008[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4008, 1), gem_int(0)))) {
#line 1815 "compiler/main.gem"
        GemVal gem_v__for_i_83 = gem_int(0);
#line 1815 "compiler/main.gem"
    GemVal _t4009[] = {gem_v_body};
        GemVal gem_v__for_limit_83 = gem_sub(gem_len_fn(NULL, _t4009, 1), gem_int(1));
#line 1815 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_83, gem_v__for_limit_83))) break;
#line 1815 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_83;
#line 1815 "compiler/main.gem"
            gem_v__for_i_83 = gem_add(gem_v__for_i_83, gem_int(1));
#line 1816 "compiler/main.gem"
    GemVal _t4010[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t4011 = (*gem_v_compile_stmt);
    GemVal _t4012[] = {gem_v_b, _t4011.fn(_t4011.env, _t4010, 2)};
            (void)(gem_buf_push_fn(NULL, _t4012, 2));
#line 1817 "compiler/main.gem"
    GemVal _t4013[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4013, 2));
        }

#line 1819 "compiler/main.gem"
    GemVal _t4014[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t4015 = (*gem_v_compile_stmt_return);
    GemVal _t4016[] = {gem_v_b, _t4015.fn(_t4015.env, _t4014, 2)};
        (void)(gem_buf_push_fn(NULL, _t4016, 2));
#line 1820 "compiler/main.gem"
    GemVal _t4017[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t4017, 2));
    } else {
#line 1822 "compiler/main.gem"
    GemVal _t4018[] = {gem_v_b, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4018, 2));
    }
#line 1824 "compiler/main.gem"
    GemVal _t4019[] = {gem_v_b};
    GemVal _t4020 = gem_buf_str_fn(NULL, _t4019, 1);
    gem_pop_frame();
    return _t4020;
}

struct _closure__anon_57 {
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
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_57(void *_env, GemVal *args, int argc) {
    struct _closure__anon_57 *_cls = (struct _closure__anon_57 *)_env;
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
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_57", "compiler/main.gem", 0);
#line 1830 "compiler/main.gem"
    GemVal gem_v__d84 = gem_v_node;
#line 1830 "compiler/main.gem"
    GemVal _t4022 = gem_v__d84;
    static GemICacheSlot _ic_578 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t4022, "params", &_ic_578);
#line 1830 "compiler/main.gem"
    GemVal _t4023 = gem_v__d84;
    static GemICacheSlot _ic_579 = {0};
    GemVal gem_v_body = gem_table_get_cached(_t4023, "body", &_ic_579);
#line 1830 "compiler/main.gem"
    GemVal _t4024 = gem_v__d84;
    static GemICacheSlot _ic_580 = {0};
    GemVal gem_v_name = gem_table_get_cached(_t4024, "name", &_ic_580);

#line 1833 "compiler/main.gem"
    GemVal _t4025[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4025, 1);
#line 1834 "compiler/main.gem"
    GemVal _t4026 = gem_v_node;
    static GemICacheSlot _ic_581 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4026, "rest_param", &_ic_581), GEM_NIL))) {
#line 1835 "compiler/main.gem"
    GemVal _t4027 = gem_v_node;
    static GemICacheSlot _ic_582 = {0};
    GemVal _t4028[] = {gem_v_scope_vars, gem_table_get_cached(_t4027, "rest_param", &_ic_582)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4028, 2));
    }
#line 1837 "compiler/main.gem"
    GemVal _t4029[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4029, 2));
#line 1839 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1840 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1841 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1842 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1844 "compiler/main.gem"
    GemVal _t4030[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t4031 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t4031.fn(_t4031.env, _t4030, 2);
#line 1845 "compiler/main.gem"
    GemVal _t4032 = gem_v_node;
    static GemICacheSlot _ic_583 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4032, "defaults", &_ic_583), GEM_NIL))) {
#line 1846 "compiler/main.gem"
    GemVal _t4033 = gem_v_node;
    static GemICacheSlot _ic_584 = {0};
        GemVal gem_v__for_items_85 = gem_table_get_cached(_t4033, "defaults", &_ic_584);
#line 1846 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1846 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4034[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t4034, 1)))) break;
#line 1846 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1846 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1847 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 1848 "compiler/main.gem"
    GemVal _t4035[] = {gem_v_d, gem_v_scope_vars, gem_v_captured};
    GemVal _t4036 = (*gem_v_walk_captures_node);
                (void)(_t4036.fn(_t4036.env, _t4035, 3));
            }
        }

    }
#line 1853 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1854 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1856 "compiler/main.gem"
    GemVal gem_v_has_defs = gem_bool(0);
#line 1857 "compiler/main.gem"
    GemVal _t4037 = gem_v_node;
    static GemICacheSlot _ic_585 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4037, "defaults", &_ic_585), GEM_NIL))) {
#line 1858 "compiler/main.gem"
    GemVal _t4038 = gem_v_node;
    static GemICacheSlot _ic_586 = {0};
        GemVal gem_v__for_items_86 = gem_table_get_cached(_t4038, "defaults", &_ic_586);
#line 1858 "compiler/main.gem"
        GemVal gem_v__for_i_86 = gem_int(0);
#line 1858 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4039[] = {gem_v__for_items_86};
            if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t4039, 1)))) break;
#line 1858 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1858 "compiler/main.gem"
            gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1859 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 1860 "compiler/main.gem"
                gem_v_has_defs = gem_bool(1);
                break;
            }
        }

    }
#line 1865 "compiler/main.gem"
    GemVal gem_v_use_tco = gem_bool(0);
#line 1866 "compiler/main.gem"
    GemVal _t4040 = gem_v_node;
    static GemICacheSlot _ic_587 = {0};
    GemVal _t4041;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t4040, "rest_param", &_ic_587), GEM_NIL))) {
        _t4041 = gem_eq(gem_table_get_cached(_t4040, "rest_param", &_ic_587), GEM_NIL);
    } else {
        _t4041 = gem_not(gem_v_has_defs);
    }
    GemVal _t4043;
    if (!gem_truthy(_t4041)) {
        _t4043 = _t4041;
    } else {
        GemVal _t4042[] = {gem_v_scope_vars, gem_v_name};
        _t4043 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t4042, 2));
    }
    if (gem_truthy(_t4043)) {
#line 1867 "compiler/main.gem"
    GemVal _t4044[] = {gem_v_name, gem_v_body};
    GemVal _t4045 = (*gem_v_has_self_tail_call);
        gem_v_use_tco = _t4045.fn(_t4045.env, _t4044, 2);
    }
#line 1869 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1870 "compiler/main.gem"
        *gem_v_tco_fn_name = gem_v_name;
#line 1871 "compiler/main.gem"
        *gem_v_tco_params = gem_v_params;
#line 1872 "compiler/main.gem"
        *gem_v_tco_captured = gem_v_captured;
    }
#line 1875 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1876 "compiler/main.gem"
    GemVal _t4046[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t4047[] = {gem_v_b, gem_interp(3, _t4046)};
    (void)(gem_buf_push_fn(NULL, _t4047, 2));
#line 1878 "compiler/main.gem"
    GemVal _t4048 = gem_v_node;
    static GemICacheSlot _ic_588 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4048, "line", &_ic_588), GEM_NIL))) {
#line 1879 "compiler/main.gem"
    GemVal _t4049 = gem_v_node;
    static GemICacheSlot _ic_589 = {0};
    GemVal _t4050[] = {(*gem_v_source_name)};
    GemVal _t4051[] = {gem_string("#line "), gem_table_get_cached(_t4049, "line", &_ic_589), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4050, 1), gem_string("\"\n")};
    GemVal _t4052[] = {gem_v_b, gem_interp(5, _t4051)};
        (void)(gem_buf_push_fn(NULL, _t4052, 2));
    }
#line 1882 "compiler/main.gem"
    GemVal _t4053[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t4054 = (*gem_v_emit_param_bindings);
    GemVal _t4055[] = {gem_v_b, _t4054.fn(_t4054.env, _t4053, 3)};
    (void)(gem_buf_push_fn(NULL, _t4055, 2));
#line 1884 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1885 "compiler/main.gem"
    GemVal _t4056 = gem_v_node;
    static GemICacheSlot _ic_590 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4056, "line", &_ic_590), GEM_NIL))) {
#line 1886 "compiler/main.gem"
    GemVal _t4057 = gem_v_node;
    static GemICacheSlot _ic_591 = {0};
        gem_v_fn_line = gem_table_get_cached(_t4057, "line", &_ic_591);
    }
#line 1888 "compiler/main.gem"
    GemVal _t4058[] = {gem_v_name};
    GemVal _t4059[] = {(*gem_v_source_name)};
    GemVal _t4060[] = {gem_string("    gem_push_frame(\""), gem_fn__mod_codegen_escape_c_string(NULL, _t4058, 1), gem_string("\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4059, 1), gem_string("\", "), gem_v_fn_line, gem_string(");\n")};
    GemVal _t4061[] = {gem_v_b, gem_interp(7, _t4060)};
    (void)(gem_buf_push_fn(NULL, _t4061, 2));
#line 1890 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1891 "compiler/main.gem"
    GemVal _t4062[] = {gem_v_b, gem_string("    while(1) {\n")};
        (void)(gem_buf_push_fn(NULL, _t4062, 2));
#line 1892 "compiler/main.gem"
    GemVal _t4063[] = {gem_v_b, gem_string("        gem_yield_check();\n")};
        (void)(gem_buf_push_fn(NULL, _t4063, 2));
#line 1893 "compiler/main.gem"
    GemVal _t4064[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4064, 1), gem_int(0)))) {
#line 1894 "compiler/main.gem"
            GemVal gem_v__for_i_87 = gem_int(0);
#line 1894 "compiler/main.gem"
    GemVal _t4065[] = {gem_v_body};
            GemVal gem_v__for_limit_87 = gem_sub(gem_len_fn(NULL, _t4065, 1), gem_int(1));
#line 1894 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_v__for_limit_87))) break;
#line 1894 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_87;
#line 1894 "compiler/main.gem"
                gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1895 "compiler/main.gem"
    GemVal _t4066[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(2)};
    GemVal _t4067 = (*gem_v_compile_stmt);
    GemVal _t4068[] = {gem_v_b, _t4067.fn(_t4067.env, _t4066, 2)};
                (void)(gem_buf_push_fn(NULL, _t4068, 2));
#line 1896 "compiler/main.gem"
    GemVal _t4069[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t4069, 2));
            }

#line 1898 "compiler/main.gem"
    GemVal _t4070[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(2)};
    GemVal _t4071 = (*gem_v_compile_stmt_return);
    GemVal _t4072[] = {gem_v_b, _t4071.fn(_t4071.env, _t4070, 2)};
            (void)(gem_buf_push_fn(NULL, _t4072, 2));
#line 1899 "compiler/main.gem"
    GemVal _t4073[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4073, 2));
        } else {
#line 1901 "compiler/main.gem"
    GemVal _t4074[] = {gem_v_b, gem_string("        gem_pop_frame();\n        return GEM_NIL;\n")};
            (void)(gem_buf_push_fn(NULL, _t4074, 2));
        }
#line 1903 "compiler/main.gem"
    GemVal _t4075[] = {gem_v_b, gem_string("    }\n")};
        (void)(gem_buf_push_fn(NULL, _t4075, 2));
    } else {
#line 1905 "compiler/main.gem"
    GemVal _t4076[] = {gem_v_body};
    GemVal _t4077 = (*gem_v_emit_fn_body);
    GemVal _t4078[] = {gem_v_b, _t4077.fn(_t4077.env, _t4076, 1)};
        (void)(gem_buf_push_fn(NULL, _t4078, 2));
    }
#line 1908 "compiler/main.gem"
    GemVal _t4079[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4079, 2));
#line 1910 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1911 "compiler/main.gem"
        *gem_v_tco_fn_name = GEM_NIL;
#line 1912 "compiler/main.gem"
        *gem_v_tco_params = GEM_NIL;
#line 1913 "compiler/main.gem"
        *gem_v_tco_captured = GEM_NIL;
    }
#line 1916 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1917 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1918 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1919 "compiler/main.gem"
    GemVal _t4080[] = {gem_v_b};
    GemVal _t4081 = gem_buf_str_fn(NULL, _t4080, 1);
    gem_pop_frame();
    return _t4081;
}

struct _closure__anon_58 {
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
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_58(void *_env, GemVal *args, int argc) {
    struct _closure__anon_58 *_cls = (struct _closure__anon_58 *)_env;
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
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_captures = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_58", "compiler/main.gem", 0);
#line 1925 "compiler/main.gem"
    GemVal gem_v__d88 = gem_v_node;
#line 1925 "compiler/main.gem"
    GemVal _t4083 = gem_v__d88;
    static GemICacheSlot _ic_592 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t4083, "params", &_ic_592);
#line 1925 "compiler/main.gem"
    GemVal _t4084 = gem_v__d88;
    static GemICacheSlot _ic_593 = {0};
    GemVal gem_v_body = gem_table_get_cached(_t4084, "body", &_ic_593);

#line 1926 "compiler/main.gem"
    GemVal _t4085 = (*gem_v_anon_name);
    GemVal gem_v_name = _t4085.fn(_t4085.env, NULL, 0);
#line 1929 "compiler/main.gem"
    GemVal _t4086[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4086, 1);
#line 1930 "compiler/main.gem"
    GemVal _t4087 = gem_v_node;
    static GemICacheSlot _ic_594 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4087, "rest_param", &_ic_594), GEM_NIL))) {
#line 1931 "compiler/main.gem"
    GemVal _t4088 = gem_v_node;
    static GemICacheSlot _ic_595 = {0};
    GemVal _t4089[] = {gem_v_inner_scope, gem_table_get_cached(_t4088, "rest_param", &_ic_595)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4089, 2));
    }
#line 1933 "compiler/main.gem"
    GemVal _t4090[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4090, 2));
#line 1935 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1936 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1937 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1938 "compiler/main.gem"
    GemVal _t4091[] = {gem_v_captures};
    GemVal _t4092[] = {gem_v_inner_scope, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4091, 1)};
    *gem_v_local_names = gem_fn__mod_codegen_set_union(NULL, _t4092, 2);
#line 1940 "compiler/main.gem"
    GemVal _t4093[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t4094 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t4094.fn(_t4094.env, _t4093, 2);
#line 1941 "compiler/main.gem"
    GemVal _t4095 = gem_v_node;
    static GemICacheSlot _ic_596 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4095, "defaults", &_ic_596), GEM_NIL))) {
#line 1942 "compiler/main.gem"
    GemVal _t4096 = gem_v_node;
    static GemICacheSlot _ic_597 = {0};
        GemVal gem_v__for_items_89 = gem_table_get_cached(_t4096, "defaults", &_ic_597);
#line 1942 "compiler/main.gem"
        GemVal gem_v__for_i_89 = gem_int(0);
#line 1942 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4097[] = {gem_v__for_items_89};
            if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t4097, 1)))) break;
#line 1942 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1942 "compiler/main.gem"
            gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1943 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 1944 "compiler/main.gem"
    GemVal _t4098[] = {gem_v_d, gem_v_inner_scope, gem_v_inner_captured};
    GemVal _t4099 = (*gem_v_walk_captures_node);
                (void)(_t4099.fn(_t4099.env, _t4098, 3));
            }
        }

    }
#line 1949 "compiler/main.gem"
    GemVal _t4100[] = {gem_v_captures};
    GemVal _t4101[] = {gem_v_inner_captured, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4100, 1)};
    *gem_v_boxed_vars = gem_fn__mod_codegen_set_union(NULL, _t4101, 2);
#line 1950 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1953 "compiler/main.gem"
    GemVal _t4102[] = {gem_string("_closure_"), gem_v_name};
    GemVal gem_v_struct_name = gem_interp(2, _t4102);
#line 1954 "compiler/main.gem"
    GemVal gem_v_sb = gem_buf_new_fn(NULL, NULL, 0);
#line 1955 "compiler/main.gem"
    GemVal _t4103[] = {gem_string("struct "), gem_v_struct_name, gem_string(" {\n")};
    GemVal _t4104[] = {gem_v_sb, gem_interp(3, _t4103)};
    (void)(gem_buf_push_fn(NULL, _t4104, 2));
#line 1956 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_captures;
#line 1956 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1956 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4105[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t4105, 1)))) break;
#line 1956 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1956 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1957 "compiler/main.gem"
    GemVal _t4106[] = {gem_v_cap};
    GemVal _t4107 = (*gem_v_mangle);
    GemVal _t4108[] = {gem_string("    GemVal *"), _t4107.fn(_t4107.env, _t4106, 1), gem_string(";\n")};
    GemVal _t4109[] = {gem_v_sb, gem_interp(3, _t4108)};
        (void)(gem_buf_push_fn(NULL, _t4109, 2));
    }

#line 1959 "compiler/main.gem"
    GemVal _t4110[] = {gem_v_sb, gem_string("};\n")};
    (void)(gem_buf_push_fn(NULL, _t4110, 2));
#line 1960 "compiler/main.gem"
    GemVal _t4111[] = {gem_v_sb};
    GemVal gem_v_struct_def = gem_buf_str_fn(NULL, _t4111, 1);
#line 1963 "compiler/main.gem"
    GemVal gem_v_fb = gem_buf_new_fn(NULL, NULL, 0);
#line 1964 "compiler/main.gem"
    GemVal _t4112[] = {gem_string("static GemVal "), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t4113[] = {gem_v_fb, gem_interp(3, _t4112)};
    (void)(gem_buf_push_fn(NULL, _t4113, 2));
#line 1965 "compiler/main.gem"
    GemVal _t4114[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4114, 1), gem_int(0)))) {
#line 1966 "compiler/main.gem"
    GemVal _t4115[] = {gem_string("    struct "), gem_v_struct_name, gem_string(" *_cls = (struct "), gem_v_struct_name, gem_string(" *)_env;\n")};
    GemVal _t4116[] = {gem_v_fb, gem_interp(5, _t4115)};
        (void)(gem_buf_push_fn(NULL, _t4116, 2));
#line 1967 "compiler/main.gem"
        GemVal gem_v__for_items_91 = gem_v_captures;
#line 1967 "compiler/main.gem"
        GemVal gem_v__for_i_91 = gem_int(0);
#line 1967 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4117[] = {gem_v__for_items_91};
            if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_len_fn(NULL, _t4117, 1)))) break;
#line 1967 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_91, gem_v__for_i_91);
#line 1967 "compiler/main.gem"
            gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1968 "compiler/main.gem"
    GemVal _t4118[] = {gem_v_cap};
    GemVal _t4119 = (*gem_v_mangle);
            GemVal gem_v_mc = _t4119.fn(_t4119.env, _t4118, 1);
#line 1969 "compiler/main.gem"
    GemVal _t4120[] = {gem_string("    GemVal *"), gem_v_mc, gem_string(" = _cls->"), gem_v_mc, gem_string(";\n")};
    GemVal _t4121[] = {gem_v_fb, gem_interp(5, _t4120)};
            (void)(gem_buf_push_fn(NULL, _t4121, 2));
        }

    }
#line 1973 "compiler/main.gem"
    GemVal _t4122[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t4123 = (*gem_v_emit_param_bindings);
    GemVal _t4124[] = {gem_v_fb, _t4123.fn(_t4123.env, _t4122, 3)};
    (void)(gem_buf_push_fn(NULL, _t4124, 2));
#line 1974 "compiler/main.gem"
    GemVal _t4125[] = {gem_v_name};
    GemVal _t4126[] = {(*gem_v_source_name)};
    GemVal _t4127[] = {gem_string("    gem_push_frame(\""), gem_fn__mod_codegen_escape_c_string(NULL, _t4125, 1), gem_string("\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4126, 1), gem_string("\", 0);\n")};
    GemVal _t4128[] = {gem_v_fb, gem_interp(5, _t4127)};
    (void)(gem_buf_push_fn(NULL, _t4128, 2));
#line 1975 "compiler/main.gem"
    GemVal _t4129[] = {gem_v_body};
    GemVal _t4130 = (*gem_v_emit_fn_body);
    GemVal _t4131[] = {gem_v_fb, _t4130.fn(_t4130.env, _t4129, 1)};
    (void)(gem_buf_push_fn(NULL, _t4131, 2));
#line 1976 "compiler/main.gem"
    GemVal _t4132[] = {gem_v_fb, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4132, 2));
#line 1978 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1979 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1980 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1983 "compiler/main.gem"
    GemVal _t4133[] = {gem_v_fb};
    GemVal _t4134[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_buf_str_fn(NULL, _t4133, 1))};
    (void)(gem_push_fn(NULL, _t4134, 2));
    GemVal _t4135 = gem_table_new();
    gem_table_set(_t4135, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t4135, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t4135, gem_string("captures"), gem_v_captures);
    GemVal _t4136 = _t4135;
    gem_pop_frame();
    return _t4136;
}

struct _closure__anon_59 {
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
static GemVal _anon_59(void *_env, GemVal *args, int argc) {
    struct _closure__anon_59 *_cls = (struct _closure__anon_59 *)_env;
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
    gem_push_frame("_anon_59", "compiler/main.gem", 0);
#line 1992 "compiler/main.gem"
    GemVal _t4138 = gem_table_new();
    GemVal gem_v_fn_defs = _t4138;
#line 1993 "compiler/main.gem"
    GemVal _t4139 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t4139;
#line 1994 "compiler/main.gem"
    GemVal _t4140 = gem_table_new();
    GemVal gem_v_extern_includes = _t4140;
#line 1995 "compiler/main.gem"
    GemVal _t4141 = gem_table_new();
    GemVal gem_v_top_stmts = _t4141;
#line 1997 "compiler/main.gem"
    GemVal _t4142 = gem_v_ast;
    static GemICacheSlot _ic_598 = {0};
    GemVal gem_v__for_items_92 = gem_table_get_cached(_t4142, "stmts", &_ic_598);
#line 1997 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1997 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4143[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t4143, 1)))) break;
#line 1997 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1997 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1998 "compiler/main.gem"
    GemVal _t4144 = gem_v_s;
    static GemICacheSlot _ic_599 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t4144, "tag", &_ic_599), gem_string("fn_def")))) {
#line 1999 "compiler/main.gem"
    GemVal _t4145[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t4145, 2));
        } else {
#line 2000 "compiler/main.gem"
    GemVal _t4146 = gem_v_s;
    static GemICacheSlot _ic_600 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t4146, "tag", &_ic_600), gem_string("extern_fn")))) {
#line 2001 "compiler/main.gem"
    GemVal _t4147[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t4147, 2));
            } else {
#line 2002 "compiler/main.gem"
    GemVal _t4148 = gem_v_s;
    static GemICacheSlot _ic_601 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t4148, "tag", &_ic_601), gem_string("extern_include")))) {
#line 2003 "compiler/main.gem"
    GemVal _t4149[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t4149, 2));
                } else {
#line 2004 "compiler/main.gem"
    GemVal _t4150 = gem_v_s;
    static GemICacheSlot _ic_602 = {0};
                    if (gem_truthy(gem_eq(gem_table_get_cached(_t4150, "tag", &_ic_602), gem_string("export")))) {
                    } else {
#line 2007 "compiler/main.gem"
    GemVal _t4151[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t4151, 2));
                    }
                }
            }
        }
    }

#line 2012 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_fns_list;
#line 2012 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 2012 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4152[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t4152, 1)))) break;
#line 2012 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 2012 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 2013 "compiler/main.gem"
    GemVal _t4153 = gem_v_ef;
    static GemICacheSlot _ic_603 = {0};
    GemVal _t4154[] = {(*gem_v_defined_fns), gem_table_get_cached(_t4153, "name", &_ic_603)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4154, 2));
    }

#line 2017 "compiler/main.gem"
    GemVal gem_v__for_items_94 = gem_v_fn_defs;
#line 2017 "compiler/main.gem"
    GemVal gem_v__for_i_94 = gem_int(0);
#line 2017 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4155[] = {gem_v__for_items_94};
        if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_len_fn(NULL, _t4155, 1)))) break;
#line 2017 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_94, gem_v__for_i_94);
#line 2017 "compiler/main.gem"
        gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 2018 "compiler/main.gem"
    GemVal _t4156 = gem_v_fd;
    static GemICacheSlot _ic_604 = {0};
    GemVal _t4157[] = {(*gem_v_defined_fns), gem_table_get_cached(_t4156, "name", &_ic_604)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4157, 2));
#line 2019 "compiler/main.gem"
    GemVal _t4158 = gem_v_fd;
    static GemICacheSlot _ic_605 = {0};
    GemVal _t4159[] = {gem_string("static GemVal gem_fn_"), gem_table_get_cached(_t4158, "name", &_ic_605), gem_string("(void *_env, GemVal *args, int argc);")};
        GemVal gem_v_decl = gem_interp(3, _t4159);
#line 2020 "compiler/main.gem"
    GemVal _t4160[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t4160, 2));
    }

#line 2024 "compiler/main.gem"
    GemVal _t4161[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4161, 2));
#line 2025 "compiler/main.gem"
    GemVal _t4162 = gem_table_new();
    GemVal _t4163[] = {gem_v_top_stmts, _t4162};
    GemVal _t4164 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t4164.fn(_t4164.env, _t4163, 2);
#line 2028 "compiler/main.gem"
    GemVal gem_v__for_items_95 = gem_v_extern_fns_list;
#line 2028 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 2028 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4165[] = {gem_v__for_items_95};
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_len_fn(NULL, _t4165, 1)))) break;
#line 2028 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_95, gem_v__for_i_95);
#line 2028 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 2029 "compiler/main.gem"
    GemVal _t4166[] = {gem_v_ef};
    GemVal _t4167 = (*gem_v_compile_extern_fn);
    GemVal _t4168[] = {(*gem_v_functions), _t4167.fn(_t4167.env, _t4166, 1)};
        (void)(gem_push_fn(NULL, _t4168, 2));
    }

#line 2033 "compiler/main.gem"
    GemVal gem_v__for_items_96 = gem_v_fn_defs;
#line 2033 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 2033 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4169[] = {gem_v__for_items_96};
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_len_fn(NULL, _t4169, 1)))) break;
#line 2033 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_96, gem_v__for_i_96);
#line 2033 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 2034 "compiler/main.gem"
    GemVal _t4170[] = {gem_v_fd};
    GemVal _t4171 = (*gem_v_compile_fn);
    GemVal _t4172[] = {(*gem_v_functions), _t4171.fn(_t4171.env, _t4170, 1)};
        (void)(gem_push_fn(NULL, _t4172, 2));
    }

#line 2038 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 2039 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 2040 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 2041 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 2042 "compiler/main.gem"
    GemVal _t4173 = gem_table_new();
    *gem_v_local_names = _t4173;
#line 2043 "compiler/main.gem"
    GemVal gem_v__for_tbl_97 = (*gem_v_top_level_vars);
#line 2043 "compiler/main.gem"
    GemVal _t4174[] = {gem_v__for_tbl_97};
    GemVal gem_v__for_len_97 = gem_len_fn(NULL, _t4174, 1);
#line 2043 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 2043 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_len_97))) break;
#line 2043 "compiler/main.gem"
    GemVal _t4175[] = {gem_v__for_tbl_97, gem_v__for_i_97};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4175, 2);
#line 2043 "compiler/main.gem"
    GemVal _t4176[] = {gem_v__for_tbl_97, gem_v__for_i_97};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4176, 2);
#line 2043 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 2044 "compiler/main.gem"
    GemVal _t4177[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4177, 2));
    }

#line 2046 "compiler/main.gem"
    GemVal _t4178[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t4179 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t4179.fn(_t4179.env, _t4178, 2);
#line 2047 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 2048 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 2049 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 2051 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 2052 "compiler/main.gem"
    GemVal gem_v__for_items_98 = gem_v_fn_defs;
#line 2052 "compiler/main.gem"
    GemVal gem_v__for_i_98 = gem_int(0);
#line 2052 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4180[] = {gem_v__for_items_98};
        if (!gem_truthy(gem_lt(gem_v__for_i_98, gem_len_fn(NULL, _t4180, 1)))) break;
#line 2052 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_98, gem_v__for_i_98);
#line 2052 "compiler/main.gem"
        gem_v__for_i_98 = gem_add(gem_v__for_i_98, gem_int(1));
#line 2053 "compiler/main.gem"
    GemVal _t4181 = gem_v_fd;
    static GemICacheSlot _ic_606 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t4181, "name", &_ic_606), gem_string("main")))) {
#line 2054 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 2058 "compiler/main.gem"
    GemVal gem_v_mb = gem_buf_new_fn(NULL, NULL, 0);
#line 2059 "compiler/main.gem"
    GemVal _t4182[] = {gem_v_mb, gem_string("static GemVal gem_user_main(void *_env, GemVal *_args, int _argc) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4182, 2));
#line 2060 "compiler/main.gem"
    GemVal _t4183[] = {gem_v_mb, gem_string("    (void)_env; (void)_args; (void)_argc;\n")};
    (void)(gem_buf_push_fn(NULL, _t4183, 2));
#line 2061 "compiler/main.gem"
    GemVal _t4184[] = {(*gem_v_source_name)};
    GemVal _t4185[] = {gem_string("    gem_push_frame(\"main\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4184, 1), gem_string("\", 0);\n")};
    GemVal _t4186[] = {gem_v_mb, gem_interp(3, _t4185)};
    (void)(gem_buf_push_fn(NULL, _t4186, 2));
#line 2062 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 2063 "compiler/main.gem"
    GemVal _t4187[] = {gem_v_mb, gem_v_main_body};
        (void)(gem_buf_push_fn(NULL, _t4187, 2));
    }
#line 2065 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 2066 "compiler/main.gem"
    GemVal _t4188[] = {gem_v_mb, gem_string("    GemVal _margs[1] = {GEM_NIL};\n")};
        (void)(gem_buf_push_fn(NULL, _t4188, 2));
#line 2067 "compiler/main.gem"
    GemVal _t4189[] = {gem_v_mb, gem_string("    gem_fn_main(NULL, _margs, 0);\n")};
        (void)(gem_buf_push_fn(NULL, _t4189, 2));
    }
#line 2069 "compiler/main.gem"
    GemVal _t4190[] = {gem_v_mb, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4190, 2));
#line 2070 "compiler/main.gem"
    GemVal _t4191[] = {gem_v_mb, gem_string("int main(int argc, char **argv) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4191, 2));
#line 2071 "compiler/main.gem"
    GemVal _t4192[] = {gem_v_mb, gem_string("    GC_INIT();\n")};
    (void)(gem_buf_push_fn(NULL, _t4192, 2));
#line 2072 "compiler/main.gem"
    GemVal _t4193[] = {gem_v_mb, gem_string("    gem_init(argc, argv);\n")};
    (void)(gem_buf_push_fn(NULL, _t4193, 2));
#line 2073 "compiler/main.gem"
    GemVal _t4194[] = {gem_v_mb, gem_string("    gem_scheduler_init();\n")};
    (void)(gem_buf_push_fn(NULL, _t4194, 2));
#line 2074 "compiler/main.gem"
    GemVal _t4195[] = {gem_v_mb, gem_string("    gem_run_main(gem_user_main, NULL);\n")};
    (void)(gem_buf_push_fn(NULL, _t4195, 2));
#line 2075 "compiler/main.gem"
    GemVal _t4196[] = {gem_v_mb, gem_string("    return 0;\n}\n")};
    (void)(gem_buf_push_fn(NULL, _t4196, 2));
#line 2076 "compiler/main.gem"
    GemVal _t4197[] = {gem_v_mb};
    GemVal gem_v_main_c = gem_buf_str_fn(NULL, _t4197, 1);
#line 2079 "compiler/main.gem"
    GemVal gem_v_ob = gem_buf_new_fn(NULL, NULL, 0);
#line 2080 "compiler/main.gem"
    GemVal gem_v__for_items_99 = gem_v_extern_includes;
#line 2080 "compiler/main.gem"
    GemVal gem_v__for_i_99 = gem_int(0);
#line 2080 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4198[] = {gem_v__for_items_99};
        if (!gem_truthy(gem_lt(gem_v__for_i_99, gem_len_fn(NULL, _t4198, 1)))) break;
#line 2080 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_99, gem_v__for_i_99);
#line 2080 "compiler/main.gem"
        gem_v__for_i_99 = gem_add(gem_v__for_i_99, gem_int(1));
#line 2081 "compiler/main.gem"
    GemVal _t4199 = gem_v_ei;
    static GemICacheSlot _ic_607 = {0};
    GemVal _t4200[] = {gem_string("#include \""), gem_table_get_cached(_t4199, "path", &_ic_607), gem_string("\"\n")};
    GemVal _t4201[] = {gem_v_ob, gem_interp(3, _t4200)};
        (void)(gem_buf_push_fn(NULL, _t4201, 2));
    }

#line 2083 "compiler/main.gem"
    GemVal _t4202[] = {gem_v_ob, gem_string("#include \"gem.h\"\n")};
    (void)(gem_buf_push_fn(NULL, _t4202, 2));
#line 2087 "compiler/main.gem"
    GemVal _t4203[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t4203, 1), gem_int(0)))) {
#line 2088 "compiler/main.gem"
        GemVal gem_v__for_i_100 = gem_int(0);
#line 2088 "compiler/main.gem"
    GemVal _t4204[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_100 = gem_len_fn(NULL, _t4204, 1);
#line 2088 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_100, gem_v__for_limit_100))) break;
#line 2088 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_100;
#line 2088 "compiler/main.gem"
            gem_v__for_i_100 = gem_add(gem_v__for_i_100, gem_int(1));
#line 2089 "compiler/main.gem"
    GemVal _t4205[] = {gem_v_ob, gem_table_get((*gem_v_extern_c_decls), gem_v_i)};
            (void)(gem_buf_push_fn(NULL, _t4205, 2));
#line 2090 "compiler/main.gem"
    GemVal _t4206[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4206, 2));
        }

    }
#line 2095 "compiler/main.gem"
    GemVal gem_v__for_i_101 = gem_int(0);
#line 2095 "compiler/main.gem"
    GemVal _t4207[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_101 = gem_len_fn(NULL, _t4207, 1);
#line 2095 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_101, gem_v__for_limit_101))) break;
#line 2095 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_101;
#line 2095 "compiler/main.gem"
        gem_v__for_i_101 = gem_add(gem_v__for_i_101, gem_int(1));
#line 2096 "compiler/main.gem"
    GemVal _t4208[] = {gem_v_ob, gem_table_get((*gem_v_forward_decls), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4208, 2));
#line 2097 "compiler/main.gem"
    GemVal _t4209[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4209, 1), gem_int(1))))) {
#line 2098 "compiler/main.gem"
    GemVal _t4210[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4210, 2));
        }
    }

#line 2101 "compiler/main.gem"
    GemVal _t4211[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4211, 2));
#line 2104 "compiler/main.gem"
    GemVal _t4212[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t4212, 1);
#line 2105 "compiler/main.gem"
    GemVal gem_v__for_i_102 = gem_int(0);
#line 2105 "compiler/main.gem"
    GemVal _t4213[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_102 = gem_len_fn(NULL, _t4213, 1);
#line 2105 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_102, gem_v__for_limit_102))) break;
#line 2105 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_102;
#line 2105 "compiler/main.gem"
        gem_v__for_i_102 = gem_add(gem_v__for_i_102, gem_int(1));
#line 2106 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 2107 "compiler/main.gem"
    GemVal _t4214[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4214, 2))) {
#line 2108 "compiler/main.gem"
    GemVal _t4215[] = {gem_string("GemVal *gem_v_"), gem_v_vname, gem_string(";\n")};
    GemVal _t4216[] = {gem_v_ob, gem_interp(3, _t4215)};
            (void)(gem_buf_push_fn(NULL, _t4216, 2));
        } else {
#line 2110 "compiler/main.gem"
    GemVal _t4217[] = {gem_string("GemVal gem_v_"), gem_v_vname, gem_string(";\n")};
    GemVal _t4218[] = {gem_v_ob, gem_interp(3, _t4217)};
            (void)(gem_buf_push_fn(NULL, _t4218, 2));
        }
    }

#line 2113 "compiler/main.gem"
    GemVal _t4219[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4219, 2));
#line 2116 "compiler/main.gem"
    GemVal gem_v__for_i_103 = gem_int(0);
#line 2116 "compiler/main.gem"
    GemVal _t4220[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_103 = gem_len_fn(NULL, _t4220, 1);
#line 2116 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_103, gem_v__for_limit_103))) break;
#line 2116 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_103;
#line 2116 "compiler/main.gem"
        gem_v__for_i_103 = gem_add(gem_v__for_i_103, gem_int(1));
#line 2117 "compiler/main.gem"
    GemVal _t4221[] = {gem_v_ob, gem_table_get((*gem_v_functions), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4221, 2));
#line 2118 "compiler/main.gem"
    GemVal _t4222[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4222, 1), gem_int(1))))) {
#line 2119 "compiler/main.gem"
    GemVal _t4223[] = {gem_v_ob, gem_string("\n\n")};
            (void)(gem_buf_push_fn(NULL, _t4223, 2));
        }
    }

#line 2122 "compiler/main.gem"
    GemVal _t4224[] = {gem_v_ob, gem_string("\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4224, 2));
#line 2124 "compiler/main.gem"
    GemVal _t4225[] = {gem_v_ob, gem_v_main_c};
    (void)(gem_buf_push_fn(NULL, _t4225, 2));
#line 2125 "compiler/main.gem"
    GemVal _t4226[] = {gem_v_ob};
    GemVal _t4227 = gem_buf_str_fn(NULL, _t4226, 1);
    gem_pop_frame();
    return _t4227;
}

static GemVal gem_fn__mod_codegen_make_codegen(void *_env, GemVal *args, int argc) {
#line 125 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_make_codegen", "compiler/main.gem", 125);
#line 126 "compiler/main.gem"
    GemVal _t1852 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1852;
#line 127 "compiler/main.gem"
    GemVal _t1853 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1853;
#line 128 "compiler/main.gem"
    GemVal _t1854 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1854;
#line 129 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 130 "compiler/main.gem"
    GemVal *gem_v_ic_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_ic_counter = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t1855[] = {gem_int(123)};
    GemVal _t1856[] = {gem_int(125)};
    GemVal *gem_v_ic_init = GC_MALLOC(sizeof(GemVal));
    *gem_v_ic_init = gem_add(gem_add(gem_chr_fn(NULL, _t1855, 1), gem_string("0")), gem_chr_fn(NULL, _t1856, 1));
#line 132 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 133 "compiler/main.gem"
    GemVal _t1857 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1857;
#line 134 "compiler/main.gem"
    GemVal _t1858 = gem_table_new();
    gem_table_set(_t1858, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1858, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1858, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1858, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1858, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1858, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1858, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1858, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1858, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1858, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1858, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1858, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1858, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1858, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1858, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1858, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1858, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1858, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1858, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1858, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1858, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1858, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1858, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1858, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1858, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1858, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1858, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1858, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1858, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1858, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1858, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1858, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1858, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1858, gem_string("sleep"), gem_string("gem_sleep_builtin"));
    gem_table_set(_t1858, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1858, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1858, gem_string("processes"), gem_string("gem_processes_builtin"));
    gem_table_set(_t1858, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1858, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1858, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1858, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1858, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1858, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1858, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1858, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1858, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1858, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1858, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1858, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1858, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1858, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1858, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1858, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1858, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1858, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1858, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1858, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1858, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1858, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1858, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1858, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1858, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1858, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1858, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1858, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1858, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1858, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1858, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1858, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1858, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1858, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1858, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1858, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    gem_table_set(_t1858, gem_string("exec"), gem_string("gem_exec_fn"));
    gem_table_set(_t1858, gem_string("tcp_connect"), gem_string("gem_tcp_connect_fn"));
    gem_table_set(_t1858, gem_string("tcp_listen"), gem_string("gem_tcp_listen_fn"));
    gem_table_set(_t1858, gem_string("tcp_accept"), gem_string("gem_tcp_accept_fn"));
    gem_table_set(_t1858, gem_string("tcp_read"), gem_string("gem_tcp_read_fn"));
    gem_table_set(_t1858, gem_string("tcp_write"), gem_string("gem_tcp_write_fn"));
    gem_table_set(_t1858, gem_string("tcp_close"), gem_string("gem_tcp_close_fn"));
    gem_table_set(_t1858, gem_string("epoch_ms"), gem_string("gem_epoch_ms_fn"));
    gem_table_set(_t1858, gem_string("format_time"), gem_string("gem_format_time_fn"));
    gem_table_set(_t1858, gem_string("format_time_local"), gem_string("gem_format_time_local_fn"));
    gem_table_set(_t1858, gem_string("sqlite_open"), gem_string("gem_sqlite_open_fn"));
    gem_table_set(_t1858, gem_string("sqlite_close"), gem_string("gem_sqlite_close_fn"));
    gem_table_set(_t1858, gem_string("sqlite_exec"), gem_string("gem_sqlite_exec_fn"));
    gem_table_set(_t1858, gem_string("sqlite_query"), gem_string("gem_sqlite_query_fn"));
    gem_table_set(_t1858, gem_string("sqlite_last_insert_id"), gem_string("gem_sqlite_last_insert_id_fn"));
    gem_table_set(_t1858, gem_string("sqlite_changes"), gem_string("gem_sqlite_changes_fn"));
    gem_table_set(_t1858, gem_string("__table_key_at"), gem_string("gem_table_key_at_fn"));
    gem_table_set(_t1858, gem_string("__table_val_at"), gem_string("gem_table_val_at_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1858;
#line 227 "compiler/main.gem"
    GemVal _t1859 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1859;
#line 228 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 228 "compiler/main.gem"
    GemVal _t1860[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_len_10 = gem_len_fn(NULL, _t1860, 1);
#line 228 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 228 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_len_10))) break;
#line 228 "compiler/main.gem"
    GemVal _t1861[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1861, 2);
#line 228 "compiler/main.gem"
    GemVal _t1862[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1862, 2);
#line 228 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 229 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 231 "compiler/main.gem"
    GemVal _t1863 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1863;
#line 232 "compiler/main.gem"
    GemVal _t1864 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1864;
#line 233 "compiler/main.gem"
    GemVal _t1865 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1865;
#line 234 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 235 "compiler/main.gem"
    GemVal _t1866 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1866;
#line 236 "compiler/main.gem"
    GemVal *gem_v_tco_fn_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_fn_name = GEM_NIL;
#line 237 "compiler/main.gem"
    GemVal *gem_v_tco_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_params = GEM_NIL;
#line 238 "compiler/main.gem"
    GemVal *gem_v_tco_captured = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_captured = GEM_NIL;
#line 242 "compiler/main.gem"
    struct _closure__anon_24 *_t1869 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1869->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1869);
#line 247 "compiler/main.gem"
    struct _closure__anon_25 *_t1872 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1872->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1872);
#line 252 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_26, NULL);
#line 258 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 259 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 261 "compiler/main.gem"
    struct _closure__anon_27 *_t2189 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t2189->gem_v_builtins = gem_v_builtins;
    _t2189->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2189->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2189->gem_v_defined_fns = gem_v_defined_fns;
    _t2189->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t2189);
#line 459 "compiler/main.gem"
    struct _closure__anon_28 *_t2201 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t2201->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t2201);
#line 474 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 475 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 477 "compiler/main.gem"
    struct _closure__anon_29 *_t2394 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2394->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2394->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2394->gem_v_walk_captures = gem_v_walk_captures;
    _t2394->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2394);
#line 588 "compiler/main.gem"
    struct _closure__anon_30 *_t2404 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2404->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2404);
#line 599 "compiler/main.gem"
    struct _closure__anon_31 *_t2409 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2409->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2409);
#line 607 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 608 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 609 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 610 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 611 "compiler/main.gem"
    GemVal *gem_v_compile_stmts_tail = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts_tail = GEM_NIL;
#line 612 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 613 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 614 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 615 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 616 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 617 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 618 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 619 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 620 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 621 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 622 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 623 "compiler/main.gem"
    GemVal *gem_v_compile_blocking_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_blocking_extern_fn = GEM_NIL;
#line 624 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 625 "compiler/main.gem"
    GemVal *gem_v_is_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_self_tail_call = GEM_NIL;
#line 626 "compiler/main.gem"
    GemVal *gem_v_has_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_has_self_tail_call = GEM_NIL;
#line 627 "compiler/main.gem"
    GemVal *gem_v_emit_tco_continue = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_tco_continue = GEM_NIL;
#line 631 "compiler/main.gem"
    struct _closure__anon_32 *_t2494 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2494->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t2494->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_is_self_tail_call = gem_make_fn(_anon_32, _t2494);
#line 680 "compiler/main.gem"
    struct _closure__anon_33 *_t2500 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2500->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_has_self_tail_call = gem_make_fn(_anon_33, _t2500);
#line 687 "compiler/main.gem"
    struct _closure__anon_34 *_t2527 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2527->gem_v_compile_expr = gem_v_compile_expr;
    _t2527->gem_v_mangle = gem_v_mangle;
    _t2527->gem_v_tco_captured = gem_v_tco_captured;
    _t2527->gem_v_tco_params = gem_v_tco_params;
    _t2527->gem_v_tmp = gem_v_tmp;
    *gem_v_emit_tco_continue = gem_make_fn(_anon_34, _t2527);
#line 717 "compiler/main.gem"
    struct _closure__anon_35 *_t2720 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2720->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2720->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2720->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2720->gem_v_compile_array = gem_v_compile_array;
    _t2720->gem_v_compile_binop = gem_v_compile_binop;
    _t2720->gem_v_compile_call = gem_v_compile_call;
    _t2720->gem_v_compile_expr = gem_v_compile_expr;
    _t2720->gem_v_compile_table = gem_v_compile_table;
    _t2720->gem_v_defined_fns = gem_v_defined_fns;
    _t2720->gem_v_ic_counter = gem_v_ic_counter;
    _t2720->gem_v_ic_init = gem_v_ic_init;
    _t2720->gem_v_in_top_level = gem_v_in_top_level;
    _t2720->gem_v_local_names = gem_v_local_names;
    _t2720->gem_v_mangle = gem_v_mangle;
    _t2720->gem_v_tmp = gem_v_tmp;
    _t2720->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_35, _t2720);
#line 802 "compiler/main.gem"
    GemVal *gem_v_is_int_key = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_int_key = gem_make_fn(_anon_36, NULL);
#line 825 "compiler/main.gem"
    struct _closure__anon_37 *_t2753 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2753->gem_v_compile_expr = gem_v_compile_expr;
    _t2753->gem_v_is_int_key = gem_v_is_int_key;
    _t2753->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_37, _t2753);
#line 845 "compiler/main.gem"
    struct _closure__anon_38 *_t2769 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2769->gem_v_compile_expr = gem_v_compile_expr;
    _t2769->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_38, _t2769);
#line 860 "compiler/main.gem"
    struct _closure__anon_39 *_t2813 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2813->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2813->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2813->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2813->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2813->gem_v_mangle = gem_v_mangle;
    _t2813->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_39, _t2813);
#line 899 "compiler/main.gem"
    struct _closure__anon_40 *_t2831 = GC_MALLOC(sizeof(struct _closure__anon_40));
    _t2831->gem_v_compile_expr = gem_v_compile_expr;
    _t2831->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_40, _t2831);
#line 920 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_41, NULL);
#line 929 "compiler/main.gem"
    struct _closure__anon_42 *_t2910 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2910->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2910->gem_v_compile_args = gem_v_compile_args;
    _t2910->gem_v_compile_expr = gem_v_compile_expr;
    _t2910->gem_v_defined_fns = gem_v_defined_fns;
    _t2910->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2910->gem_v_local_names = gem_v_local_names;
    _t2910->gem_v_source_name = gem_v_source_name;
    _t2910->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_42, _t2910);
#line 973 "compiler/main.gem"
    struct _closure__anon_43 *_t3064 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3064->gem_v_compile_expr = gem_v_compile_expr;
    _t3064->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_43, _t3064);
#line 1054 "compiler/main.gem"
    struct _closure__anon_44 *_t3072 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3072->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_44, _t3072);
#line 1063 "compiler/main.gem"
    struct _closure__anon_45 *_t3091 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3091->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3091->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3091->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_stmts_tail = gem_make_fn(_anon_45, _t3091);
#line 1084 "compiler/main.gem"
    struct _closure__anon_46 *_t3289 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3289->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3289->gem_v_compile_expr = gem_v_compile_expr;
    _t3289->gem_v_compile_if = gem_v_compile_if;
    _t3289->gem_v_compile_match = gem_v_compile_match;
    _t3289->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3289->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3289->gem_v_compile_while = gem_v_compile_while;
    _t3289->gem_v_in_top_level = gem_v_in_top_level;
    _t3289->gem_v_mangle = gem_v_mangle;
    _t3289->gem_v_source_name = gem_v_source_name;
    _t3289->gem_v_tmp = gem_v_tmp;
    _t3289->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3289->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_46, _t3289);
#line 1161 "compiler/main.gem"
    struct _closure__anon_47 *_t3489 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3489->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3489->gem_v_compile_expr = gem_v_compile_expr;
    _t3489->gem_v_compile_if = gem_v_compile_if;
    _t3489->gem_v_compile_match = gem_v_compile_match;
    _t3489->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3489->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3489->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3489->gem_v_compile_while = gem_v_compile_while;
    _t3489->gem_v_emit_tco_continue = gem_v_emit_tco_continue;
    _t3489->gem_v_in_top_level = gem_v_in_top_level;
    _t3489->gem_v_local_names = gem_v_local_names;
    _t3489->gem_v_mangle = gem_v_mangle;
    _t3489->gem_v_source_name = gem_v_source_name;
    _t3489->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3489->gem_v_tmp = gem_v_tmp;
    _t3489->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_47, _t3489);
#line 1240 "compiler/main.gem"
    struct _closure__anon_48 *_t3520 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3520->gem_v_compile_expr = gem_v_compile_expr;
    _t3520->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    *gem_v_compile_if = gem_make_fn(_anon_48, _t3520);
#line 1260 "compiler/main.gem"
    struct _closure__anon_49 *_t3545 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3545->gem_v_compile_expr = gem_v_compile_expr;
    _t3545->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_49, _t3545);
#line 1277 "compiler/main.gem"
    struct _closure__anon_50 *_t3623 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3623->gem_v_compile_expr = gem_v_compile_expr;
    _t3623->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3623->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3623->gem_v_mangle = gem_v_mangle;
    _t3623->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_50, _t3623);
#line 1333 "compiler/main.gem"
    struct _closure__anon_51 *_t3765 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3765->gem_v_compile_expr = gem_v_compile_expr;
    _t3765->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3765->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3765->gem_v_mangle = gem_v_mangle;
    _t3765->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_51, _t3765);
#line 1453 "compiler/main.gem"
    struct _closure__anon_52 *_t3771 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3771->gem_v_extern_c_decls = gem_v_extern_c_decls;
    GemVal *gem_v_emit_extern_c_decl = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_extern_c_decl = gem_make_fn(_anon_52, _t3771);
#line 1499 "compiler/main.gem"
    struct _closure__anon_53 *_t3839 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3839->gem_v_compile_blocking_extern_fn = gem_v_compile_blocking_extern_fn;
    _t3839->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3839->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_53, _t3839);
#line 1582 "compiler/main.gem"
    struct _closure__anon_54 *_t3952 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t3952->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3952->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_blocking_extern_fn = gem_make_fn(_anon_54, _t3952);
#line 1762 "compiler/main.gem"
    struct _closure__anon_55 *_t4007 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t4007->gem_v_compile_expr = gem_v_compile_expr;
    _t4007->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_55, _t4007);
#line 1812 "compiler/main.gem"
    struct _closure__anon_56 *_t4021 = GC_MALLOC(sizeof(struct _closure__anon_56));
    _t4021->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4021->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_56, _t4021);
#line 1829 "compiler/main.gem"
    struct _closure__anon_57 *_t4082 = GC_MALLOC(sizeof(struct _closure__anon_57));
    _t4082->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4082->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4082->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t4082->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4082->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4082->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4082->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t4082->gem_v_in_top_level = gem_v_in_top_level;
    _t4082->gem_v_local_names = gem_v_local_names;
    _t4082->gem_v_source_name = gem_v_source_name;
    _t4082->gem_v_tco_captured = gem_v_tco_captured;
    _t4082->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t4082->gem_v_tco_params = gem_v_tco_params;
    _t4082->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_compile_fn = gem_make_fn(_anon_57, _t4082);
#line 1924 "compiler/main.gem"
    struct _closure__anon_58 *_t4137 = GC_MALLOC(sizeof(struct _closure__anon_58));
    _t4137->gem_v_anon_name = gem_v_anon_name;
    _t4137->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4137->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4137->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4137->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4137->gem_v_functions = gem_v_functions;
    _t4137->gem_v_in_top_level = gem_v_in_top_level;
    _t4137->gem_v_local_names = gem_v_local_names;
    _t4137->gem_v_mangle = gem_v_mangle;
    _t4137->gem_v_source_name = gem_v_source_name;
    _t4137->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_58, _t4137);
#line 1990 "compiler/main.gem"
    struct _closure__anon_59 *_t4228 = GC_MALLOC(sizeof(struct _closure__anon_59));
    _t4228->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4228->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t4228->gem_v_compile_fn = gem_v_compile_fn;
    _t4228->gem_v_compile_stmts = gem_v_compile_stmts;
    _t4228->gem_v_defined_fns = gem_v_defined_fns;
    _t4228->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t4228->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4228->gem_v_forward_decls = gem_v_forward_decls;
    _t4228->gem_v_functions = gem_v_functions;
    _t4228->gem_v_in_top_level = gem_v_in_top_level;
    _t4228->gem_v_local_names = gem_v_local_names;
    _t4228->gem_v_source_name = gem_v_source_name;
    _t4228->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t4228->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_59, _t4228);
    GemVal _t4229 = gem_table_new();
    gem_table_set(_t4229, gem_string("compile"), gem_v_compile);
    GemVal _t4230 = _t4229;
    gem_pop_frame();
    return _t4230;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 20 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 20);
    GemVal _t4231[] = {gem_v_node};
    GemVal _t4233;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4231, 1), gem_string("table")))) {
        _t4233 = gem_eq(gem_type_fn(NULL, _t4231, 1), gem_string("table"));
    } else {
        GemVal _t4232 = gem_v_node;
        static GemICacheSlot _ic_608 = {0};
        _t4233 = gem_eq(gem_table_get_cached(_t4232, "tag", &_ic_608), gem_v_tag);
    }
    GemVal _t4234 = _t4233;
    gem_pop_frame();
    return _t4234;
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_program", "compiler/main.gem", 24);
    GemVal _t4235 = gem_table_new();
    gem_table_set(_t4235, gem_string("tag"), gem_string("program"));
    gem_table_set(_t4235, gem_string("stmts"), gem_v_stmts);
    GemVal _t4236 = _t4235;
    gem_pop_frame();
    return _t4236;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 28);
    GemVal _t4237 = gem_table_new();
    gem_table_set(_t4237, gem_string("tag"), gem_string("let"));
    gem_table_set(_t4237, gem_string("name"), gem_v_name);
    gem_table_set(_t4237, gem_string("value"), gem_v_value);
    gem_table_set(_t4237, gem_string("line"), gem_v_line);
    GemVal _t4238 = _t4237;
    gem_pop_frame();
    return _t4238;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 32);
    GemVal _t4239 = gem_table_new();
    gem_table_set(_t4239, gem_string("tag"), gem_string("var"));
    gem_table_set(_t4239, gem_string("name"), gem_v_name);
    GemVal _t4240 = _t4239;
    gem_pop_frame();
    return _t4240;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 36);
    GemVal _t4241 = gem_table_new();
    gem_table_set(_t4241, gem_string("tag"), gem_string("table"));
    gem_table_set(_t4241, gem_string("entries"), gem_v_entries);
    GemVal _t4242 = _t4241;
    gem_pop_frame();
    return _t4242;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 40);
    GemVal _t4243 = gem_table_new();
    gem_table_set(_t4243, gem_string("key"), gem_v_key);
    gem_table_set(_t4243, gem_string("value"), gem_v_value);
    GemVal _t4244 = _t4243;
    gem_pop_frame();
    return _t4244;
}

static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("module_name_from_path", "compiler/main.gem", 46);
#line 47 "compiler/main.gem"
    GemVal _t4245[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4245, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 49 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 50 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 51 "compiler/main.gem"
    GemVal _t4246[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4246, 2), gem_int(47)))) {
#line 52 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 54 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 56 "compiler/main.gem"
    GemVal _t4247[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4247, 3);
#line 57 "compiler/main.gem"
    GemVal _t4248[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4248, 1);
#line 58 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 59 "compiler/main.gem"
    GemVal _t4249 = gem_v_string;
    static GemICacheSlot _ic_609 = {0};
    GemVal _t4250[] = {gem_v_name, gem_string(".gem")};
    GemVal _t4251 = gem_table_get_cached(_t4249, "ends_with", &_ic_609);
        if (gem_truthy(_t4251.fn(_t4251.env, _t4250, 2))) {
#line 60 "compiler/main.gem"
    GemVal _t4252[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4252, 3);
        }
    }
    GemVal _t4253 = gem_v_name;
    gem_pop_frame();
    return _t4253;
}

static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("find_export_node", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal _t4254[] = {gem_v_stmts};
    GemVal gem_v_i = gem_sub(gem_len_fn(NULL, _t4254, 1), gem_int(1));
#line 68 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_i, gem_int(0)))) break;
#line 69 "compiler/main.gem"
    GemVal _t4255[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("export")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4255, 2))) {
#line 70 "compiler/main.gem"
            GemVal _t4256 = gem_table_get(gem_v_stmts, gem_v_i);
            gem_pop_frame();
            return _t4256;
        }
#line 72 "compiler/main.gem"
        gem_v_i = gem_sub(gem_v_i, gem_int(1));
    }
    GemVal _t4257 = GEM_NIL;
    gem_pop_frame();
    return _t4257;
}

static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc) {
#line 77 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("collect_module_top_names", "compiler/main.gem", 77);
#line 78 "compiler/main.gem"
    GemVal _t4258 = gem_table_new();
    GemVal gem_v_names = _t4258;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_items_1 = gem_v_stmts;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 79 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4259[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t4259, 1)))) break;
#line 79 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 79 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 80 "compiler/main.gem"
    GemVal _t4260[] = {gem_v_stmt, gem_string("fn_def")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4260, 2))) {
#line 81 "compiler/main.gem"
    GemVal _t4261 = gem_v_string;
    static GemICacheSlot _ic_610 = {0};
    GemVal _t4262 = gem_v_stmt;
    static GemICacheSlot _ic_611 = {0};
    GemVal _t4263[] = {gem_table_get_cached(_t4262, "name", &_ic_611), gem_string("_mod_")};
    GemVal _t4264 = gem_table_get_cached(_t4261, "starts_with", &_ic_610);
            if (gem_truthy(gem_not(_t4264.fn(_t4264.env, _t4263, 2)))) {
#line 82 "compiler/main.gem"
    GemVal _t4265 = gem_v_stmt;
    static GemICacheSlot _ic_612 = {0};
                gem_table_set(gem_v_names, gem_table_get_cached(_t4265, "name", &_ic_612), gem_bool(1));
            }
        } else {
#line 84 "compiler/main.gem"
    GemVal _t4266[] = {gem_v_stmt, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t4266, 2))) {
#line 85 "compiler/main.gem"
    GemVal _t4267 = gem_v_string;
    static GemICacheSlot _ic_613 = {0};
    GemVal _t4268 = gem_v_stmt;
    static GemICacheSlot _ic_614 = {0};
    GemVal _t4269[] = {gem_table_get_cached(_t4268, "name", &_ic_614), gem_string("_mod_")};
    GemVal _t4270 = gem_table_get_cached(_t4267, "starts_with", &_ic_613);
                if (gem_truthy(gem_not(_t4270.fn(_t4270.env, _t4269, 2)))) {
#line 86 "compiler/main.gem"
    GemVal _t4271 = gem_v_stmt;
    static GemICacheSlot _ic_615 = {0};
                    gem_table_set(gem_v_names, gem_table_get_cached(_t4271, "name", &_ic_615), gem_bool(1));
                }
            }
        }
    }

    GemVal _t4272 = gem_v_names;
    gem_pop_frame();
    return _t4272;
}

static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_top_names = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("build_rename_map", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t4273 = gem_table_new();
    GemVal gem_v_rmap = _t4273;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_tbl_2 = gem_v_top_names;
#line 95 "compiler/main.gem"
    GemVal _t4274[] = {gem_v__for_tbl_2};
    GemVal gem_v__for_len_2 = gem_len_fn(NULL, _t4274, 1);
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_len_2))) break;
#line 95 "compiler/main.gem"
    GemVal _t4275[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v_name = gem_table_key_at_fn(NULL, _t4275, 2);
#line 95 "compiler/main.gem"
    GemVal _t4276[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4276, 2);
#line 95 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_rmap, gem_v_name, gem_add(gem_v_prefix, gem_v_name));
    }

    GemVal _t4277 = gem_v_rmap;
    gem_pop_frame();
    return _t4277;
}

static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("copy_set", "compiler/main.gem", 101);
#line 102 "compiler/main.gem"
    GemVal _t4278 = gem_table_new();
    GemVal gem_v_result = _t4278;
#line 103 "compiler/main.gem"
    GemVal gem_v__for_tbl_3 = gem_v_s;
#line 103 "compiler/main.gem"
    GemVal _t4279[] = {gem_v__for_tbl_3};
    GemVal gem_v__for_len_3 = gem_len_fn(NULL, _t4279, 1);
#line 103 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 103 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_len_3))) break;
#line 103 "compiler/main.gem"
    GemVal _t4280[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4280, 2);
#line 103 "compiler/main.gem"
    GemVal _t4281[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_v = gem_table_val_at_fn(NULL, _t4281, 2);
#line 103 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 104 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_v_v);
    }

    GemVal _t4282 = gem_v_result;
    gem_pop_frame();
    return _t4282;
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
            GemVal _t4283 = GEM_NIL;
            gem_pop_frame();
            return _t4283;
        }
#line 113 "compiler/main.gem"
    GemVal _t4284[] = {gem_v_node};
        if (gem_truthy(gem_neq(gem_type_fn(NULL, _t4284, 1), gem_string("table")))) {
#line 114 "compiler/main.gem"
            GemVal _t4285 = GEM_NIL;
            gem_pop_frame();
            return _t4285;
        }
#line 116 "compiler/main.gem"
    GemVal _t4286 = gem_v_node;
    static GemICacheSlot _ic_616 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t4286, "tag", &_ic_616), GEM_NIL))) {
#line 117 "compiler/main.gem"
            GemVal _t4287 = GEM_NIL;
            gem_pop_frame();
            return _t4287;
        }
#line 120 "compiler/main.gem"
        GemVal gem_v__match_4 = gem_v_node;
    GemVal _t4288[] = {gem_v__match_4};
    GemVal _t4290;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4288, 1), gem_string("table")))) {
        _t4290 = gem_eq(gem_type_fn(NULL, _t4288, 1), gem_string("table"));
    } else {
        GemVal _t4289[] = {gem_v__match_4, gem_string("tag")};
        _t4290 = gem_has_key_fn(NULL, _t4289, 2);
    }
    GemVal _t4291;
    if (!gem_truthy(_t4290)) {
        _t4291 = _t4290;
    } else {
        _t4291 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("var"));
    }
    GemVal _t4293;
    if (!gem_truthy(_t4291)) {
        _t4293 = _t4291;
    } else {
        GemVal _t4292[] = {gem_v__match_4, gem_string("name")};
        _t4293 = gem_has_key_fn(NULL, _t4292, 2);
    }
        if (gem_truthy(_t4293)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 122 "compiler/main.gem"
    GemVal _t4294[] = {gem_v_rmap, gem_v_name};
    GemVal _t4296;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4294, 2))) {
        _t4296 = gem_has_key_fn(NULL, _t4294, 2);
    } else {
        GemVal _t4295[] = {gem_v_shadowed, gem_v_name};
        _t4296 = gem_not(gem_has_key_fn(NULL, _t4295, 2));
    }
            if (gem_truthy(_t4296)) {
#line 123 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4297[] = {gem_v__match_4};
    GemVal _t4299;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4297, 1), gem_string("table")))) {
        _t4299 = gem_eq(gem_type_fn(NULL, _t4297, 1), gem_string("table"));
    } else {
        GemVal _t4298[] = {gem_v__match_4, gem_string("tag")};
        _t4299 = gem_has_key_fn(NULL, _t4298, 2);
    }
    GemVal _t4300;
    if (!gem_truthy(_t4299)) {
        _t4300 = _t4299;
    } else {
        _t4300 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("fn_def"));
    }
    GemVal _t4302;
    if (!gem_truthy(_t4300)) {
        _t4302 = _t4300;
    } else {
        GemVal _t4301[] = {gem_v__match_4, gem_string("name")};
        _t4302 = gem_has_key_fn(NULL, _t4301, 2);
    }
    GemVal _t4304;
    if (!gem_truthy(_t4302)) {
        _t4304 = _t4302;
    } else {
        GemVal _t4303[] = {gem_v__match_4, gem_string("params")};
        _t4304 = gem_has_key_fn(NULL, _t4303, 2);
    }
    GemVal _t4306;
    if (!gem_truthy(_t4304)) {
        _t4306 = _t4304;
    } else {
        GemVal _t4305[] = {gem_v__match_4, gem_string("body")};
        _t4306 = gem_has_key_fn(NULL, _t4305, 2);
    }
        if (gem_truthy(_t4306)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 126 "compiler/main.gem"
    GemVal _t4307[] = {gem_v_rmap, gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t4307, 2))) {
#line 127 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
            }
#line 129 "compiler/main.gem"
    GemVal _t4308[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4308, 1);
#line 130 "compiler/main.gem"
            GemVal gem_v__for_items_5 = gem_v_params;
#line 130 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4309[] = {gem_v__for_items_5};
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t4309, 1)))) break;
#line 130 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 133 "compiler/main.gem"
    GemVal _t4310 = gem_v_node;
    static GemICacheSlot _ic_617 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4310, "rest_param", &_ic_617), GEM_NIL))) {
#line 134 "compiler/main.gem"
    GemVal _t4311 = gem_v_node;
    static GemICacheSlot _ic_618 = {0};
                gem_table_set(gem_v_inner, gem_table_get_cached(_t4311, "rest_param", &_ic_618), gem_bool(1));
            }
#line 136 "compiler/main.gem"
    GemVal _t4312[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4313 = gem_fn_rename_stmts(NULL, _t4312, 3);
            gem_pop_frame();
            return _t4313;
        } else {
    GemVal _t4314[] = {gem_v__match_4};
    GemVal _t4316;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4314, 1), gem_string("table")))) {
        _t4316 = gem_eq(gem_type_fn(NULL, _t4314, 1), gem_string("table"));
    } else {
        GemVal _t4315[] = {gem_v__match_4, gem_string("tag")};
        _t4316 = gem_has_key_fn(NULL, _t4315, 2);
    }
    GemVal _t4317;
    if (!gem_truthy(_t4316)) {
        _t4317 = _t4316;
    } else {
        _t4317 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t4319;
    if (!gem_truthy(_t4317)) {
        _t4319 = _t4317;
    } else {
        GemVal _t4318[] = {gem_v__match_4, gem_string("params")};
        _t4319 = gem_has_key_fn(NULL, _t4318, 2);
    }
    GemVal _t4321;
    if (!gem_truthy(_t4319)) {
        _t4321 = _t4319;
    } else {
        GemVal _t4320[] = {gem_v__match_4, gem_string("body")};
        _t4321 = gem_has_key_fn(NULL, _t4320, 2);
    }
        if (gem_truthy(_t4321)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 138 "compiler/main.gem"
    GemVal _t4322[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4322, 1);
#line 139 "compiler/main.gem"
            GemVal gem_v__for_items_6 = gem_v_params;
#line 139 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 139 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4323[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t4323, 1)))) break;
#line 139 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 139 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 140 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 142 "compiler/main.gem"
    GemVal _t4324 = gem_v_node;
    static GemICacheSlot _ic_619 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4324, "rest_param", &_ic_619), GEM_NIL))) {
#line 143 "compiler/main.gem"
    GemVal _t4325 = gem_v_node;
    static GemICacheSlot _ic_620 = {0};
                gem_table_set(gem_v_inner, gem_table_get_cached(_t4325, "rest_param", &_ic_620), gem_bool(1));
            }
#line 145 "compiler/main.gem"
    GemVal _t4326[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4327 = gem_fn_rename_stmts(NULL, _t4326, 3);
            gem_pop_frame();
            return _t4327;
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
        _t4331 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("let"));
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
#line 147 "compiler/main.gem"
    GemVal _t4336[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4336, 3));
#line 148 "compiler/main.gem"
    GemVal _t4337[] = {gem_v_rmap, gem_v_name};
    GemVal _t4339;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4337, 2))) {
        _t4339 = gem_has_key_fn(NULL, _t4337, 2);
    } else {
        GemVal _t4338[] = {gem_v_shadowed, gem_v_name};
        _t4339 = gem_not(gem_has_key_fn(NULL, _t4338, 2));
    }
            if (gem_truthy(_t4339)) {
#line 149 "compiler/main.gem"
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
        _t4343 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t4345;
    if (!gem_truthy(_t4343)) {
        _t4345 = _t4343;
    } else {
        GemVal _t4344[] = {gem_v__match_4, gem_string("name")};
        _t4345 = gem_has_key_fn(NULL, _t4344, 2);
    }
    GemVal _t4347;
    if (!gem_truthy(_t4345)) {
        _t4347 = _t4345;
    } else {
        GemVal _t4346[] = {gem_v__match_4, gem_string("value")};
        _t4347 = gem_has_key_fn(NULL, _t4346, 2);
    }
        if (gem_truthy(_t4347)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 152 "compiler/main.gem"
    GemVal _t4348[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4348, 3));
#line 153 "compiler/main.gem"
    GemVal _t4349[] = {gem_v_rmap, gem_v_name};
    GemVal _t4351;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4349, 2))) {
        _t4351 = gem_has_key_fn(NULL, _t4349, 2);
    } else {
        GemVal _t4350[] = {gem_v_shadowed, gem_v_name};
        _t4351 = gem_not(gem_has_key_fn(NULL, _t4350, 2));
    }
            if (gem_truthy(_t4351)) {
#line 154 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
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
        _t4355 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("call"));
    }
    GemVal _t4357;
    if (!gem_truthy(_t4355)) {
        _t4357 = _t4355;
    } else {
        GemVal _t4356[] = {gem_v__match_4, gem_string("func")};
        _t4357 = gem_has_key_fn(NULL, _t4356, 2);
    }
    GemVal _t4359;
    if (!gem_truthy(_t4357)) {
        _t4359 = _t4357;
    } else {
        GemVal _t4358[] = {gem_v__match_4, gem_string("args")};
        _t4359 = gem_has_key_fn(NULL, _t4358, 2);
    }
        if (gem_truthy(_t4359)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_func = gem_table_get(gem_v__match_4, gem_string("func"));
#line 0 "compiler/main.gem"
            GemVal gem_v_args = gem_table_get(gem_v__match_4, gem_string("args"));
#line 157 "compiler/main.gem"
    GemVal _t4360[] = {gem_v_func, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4360, 3));
            {
#line 158 "compiler/main.gem"
                GemVal gem_v__for_items_7 = gem_v_args;
#line 158 "compiler/main.gem"
                GemVal gem_v__for_i_7 = gem_int(0);
#line 158 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4361[] = {gem_v__for_items_7};
                    if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t4361, 1)))) break;
#line 158 "compiler/main.gem"
                    GemVal gem_v_a = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 158 "compiler/main.gem"
                    gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 159 "compiler/main.gem"
    GemVal _t4362[] = {gem_v_a, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4362, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
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
        _t4366 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("binop"));
    }
    GemVal _t4368;
    if (!gem_truthy(_t4366)) {
        _t4368 = _t4366;
    } else {
        GemVal _t4367[] = {gem_v__match_4, gem_string("left")};
        _t4368 = gem_has_key_fn(NULL, _t4367, 2);
    }
    GemVal _t4370;
    if (!gem_truthy(_t4368)) {
        _t4370 = _t4368;
    } else {
        GemVal _t4369[] = {gem_v__match_4, gem_string("right")};
        _t4370 = gem_has_key_fn(NULL, _t4369, 2);
    }
        if (gem_truthy(_t4370)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_left = gem_table_get(gem_v__match_4, gem_string("left"));
#line 0 "compiler/main.gem"
            GemVal gem_v_right = gem_table_get(gem_v__match_4, gem_string("right"));
#line 162 "compiler/main.gem"
    GemVal _t4371[] = {gem_v_left, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4371, 3));
#line 163 "compiler/main.gem"
            GemVal _t4372 = gem_v_right;
            GemVal _t4373 = gem_v_rmap;
            GemVal _t4374 = gem_v_shadowed;
            gem_v_node = _t4372;
            gem_v_rmap = _t4373;
            gem_v_shadowed = _t4374;
            continue;
        } else {
    GemVal _t4375[] = {gem_v__match_4};
    GemVal _t4377;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4375, 1), gem_string("table")))) {
        _t4377 = gem_eq(gem_type_fn(NULL, _t4375, 1), gem_string("table"));
    } else {
        GemVal _t4376[] = {gem_v__match_4, gem_string("tag")};
        _t4377 = gem_has_key_fn(NULL, _t4376, 2);
    }
    GemVal _t4378;
    if (!gem_truthy(_t4377)) {
        _t4378 = _t4377;
    } else {
        _t4378 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t4380;
    if (!gem_truthy(_t4378)) {
        _t4380 = _t4378;
    } else {
        GemVal _t4379[] = {gem_v__match_4, gem_string("expr")};
        _t4380 = gem_has_key_fn(NULL, _t4379, 2);
    }
        if (gem_truthy(_t4380)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_expr = gem_table_get(gem_v__match_4, gem_string("expr"));
#line 165 "compiler/main.gem"
            GemVal _t4381 = gem_v_expr;
            GemVal _t4382 = gem_v_rmap;
            GemVal _t4383 = gem_v_shadowed;
            gem_v_node = _t4381;
            gem_v_rmap = _t4382;
            gem_v_shadowed = _t4383;
            continue;
        } else {
    GemVal _t4384[] = {gem_v__match_4};
    GemVal _t4386;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4384, 1), gem_string("table")))) {
        _t4386 = gem_eq(gem_type_fn(NULL, _t4384, 1), gem_string("table"));
    } else {
        GemVal _t4385[] = {gem_v__match_4, gem_string("tag")};
        _t4386 = gem_has_key_fn(NULL, _t4385, 2);
    }
    GemVal _t4387;
    if (!gem_truthy(_t4386)) {
        _t4387 = _t4386;
    } else {
        _t4387 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t4389;
    if (!gem_truthy(_t4387)) {
        _t4389 = _t4387;
    } else {
        GemVal _t4388[] = {gem_v__match_4, gem_string("object")};
        _t4389 = gem_has_key_fn(NULL, _t4388, 2);
    }
        if (gem_truthy(_t4389)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 167 "compiler/main.gem"
            GemVal _t4390 = gem_v_object;
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
        _t4396 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index"));
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
        GemVal _t4399[] = {gem_v__match_4, gem_string("key")};
        _t4400 = gem_has_key_fn(NULL, _t4399, 2);
    }
        if (gem_truthy(_t4400)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 169 "compiler/main.gem"
    GemVal _t4401[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4401, 3));
#line 170 "compiler/main.gem"
            GemVal _t4402 = gem_v_key;
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
        _t4408 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot_assign"));
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
        GemVal _t4411[] = {gem_v__match_4, gem_string("value")};
        _t4412 = gem_has_key_fn(NULL, _t4411, 2);
    }
        if (gem_truthy(_t4412)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 172 "compiler/main.gem"
    GemVal _t4413[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4413, 3));
#line 173 "compiler/main.gem"
            GemVal _t4414 = gem_v_value;
            GemVal _t4415 = gem_v_rmap;
            GemVal _t4416 = gem_v_shadowed;
            gem_v_node = _t4414;
            gem_v_rmap = _t4415;
            gem_v_shadowed = _t4416;
            continue;
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
        _t4420 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t4422;
    if (!gem_truthy(_t4420)) {
        _t4422 = _t4420;
    } else {
        GemVal _t4421[] = {gem_v__match_4, gem_string("object")};
        _t4422 = gem_has_key_fn(NULL, _t4421, 2);
    }
    GemVal _t4424;
    if (!gem_truthy(_t4422)) {
        _t4424 = _t4422;
    } else {
        GemVal _t4423[] = {gem_v__match_4, gem_string("key")};
        _t4424 = gem_has_key_fn(NULL, _t4423, 2);
    }
    GemVal _t4426;
    if (!gem_truthy(_t4424)) {
        _t4426 = _t4424;
    } else {
        GemVal _t4425[] = {gem_v__match_4, gem_string("value")};
        _t4426 = gem_has_key_fn(NULL, _t4425, 2);
    }
        if (gem_truthy(_t4426)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 175 "compiler/main.gem"
    GemVal _t4427[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4427, 3));
#line 176 "compiler/main.gem"
    GemVal _t4428[] = {gem_v_key, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4428, 3));
#line 177 "compiler/main.gem"
            GemVal _t4429 = gem_v_value;
            GemVal _t4430 = gem_v_rmap;
            GemVal _t4431 = gem_v_shadowed;
            gem_v_node = _t4429;
            gem_v_rmap = _t4430;
            gem_v_shadowed = _t4431;
            continue;
        } else {
    GemVal _t4432[] = {gem_v__match_4};
    GemVal _t4434;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4432, 1), gem_string("table")))) {
        _t4434 = gem_eq(gem_type_fn(NULL, _t4432, 1), gem_string("table"));
    } else {
        GemVal _t4433[] = {gem_v__match_4, gem_string("tag")};
        _t4434 = gem_has_key_fn(NULL, _t4433, 2);
    }
    GemVal _t4435;
    if (!gem_truthy(_t4434)) {
        _t4435 = _t4434;
    } else {
        _t4435 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("if"));
    }
    GemVal _t4437;
    if (!gem_truthy(_t4435)) {
        _t4437 = _t4435;
    } else {
        GemVal _t4436[] = {gem_v__match_4, gem_string("cond")};
        _t4437 = gem_has_key_fn(NULL, _t4436, 2);
    }
        if (gem_truthy(_t4437)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 179 "compiler/main.gem"
    GemVal _t4438[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4438, 3));
#line 180 "compiler/main.gem"
    GemVal _t4439 = gem_v_node;
    static GemICacheSlot _ic_621 = {0};
    GemVal _t4440[] = {gem_table_get_cached(_t4439, "then", &_ic_621), gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_stmts(NULL, _t4440, 3));
#line 181 "compiler/main.gem"
    GemVal _t4441 = gem_v_node;
    static GemICacheSlot _ic_622 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4441, "else", &_ic_622), GEM_NIL))) {
#line 182 "compiler/main.gem"
    GemVal _t4442 = gem_v_node;
    static GemICacheSlot _ic_623 = {0};
    GemVal _t4443[] = {gem_table_get_cached(_t4442, "else", &_ic_623), gem_v_rmap, gem_v_shadowed};
                GemVal _t4444 = gem_fn_rename_stmts(NULL, _t4443, 3);
                gem_pop_frame();
                return _t4444;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4445[] = {gem_v__match_4};
    GemVal _t4447;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4445, 1), gem_string("table")))) {
        _t4447 = gem_eq(gem_type_fn(NULL, _t4445, 1), gem_string("table"));
    } else {
        GemVal _t4446[] = {gem_v__match_4, gem_string("tag")};
        _t4447 = gem_has_key_fn(NULL, _t4446, 2);
    }
    GemVal _t4448;
    if (!gem_truthy(_t4447)) {
        _t4448 = _t4447;
    } else {
        _t4448 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("while"));
    }
    GemVal _t4450;
    if (!gem_truthy(_t4448)) {
        _t4450 = _t4448;
    } else {
        GemVal _t4449[] = {gem_v__match_4, gem_string("cond")};
        _t4450 = gem_has_key_fn(NULL, _t4449, 2);
    }
    GemVal _t4452;
    if (!gem_truthy(_t4450)) {
        _t4452 = _t4450;
    } else {
        GemVal _t4451[] = {gem_v__match_4, gem_string("body")};
        _t4452 = gem_has_key_fn(NULL, _t4451, 2);
    }
        if (gem_truthy(_t4452)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 185 "compiler/main.gem"
    GemVal _t4453[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4453, 3));
#line 186 "compiler/main.gem"
    GemVal _t4454[] = {gem_v_body, gem_v_rmap, gem_v_shadowed};
            GemVal _t4455 = gem_fn_rename_stmts(NULL, _t4454, 3);
            gem_pop_frame();
            return _t4455;
        } else {
    GemVal _t4456[] = {gem_v__match_4};
    GemVal _t4458;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4456, 1), gem_string("table")))) {
        _t4458 = gem_eq(gem_type_fn(NULL, _t4456, 1), gem_string("table"));
    } else {
        GemVal _t4457[] = {gem_v__match_4, gem_string("tag")};
        _t4458 = gem_has_key_fn(NULL, _t4457, 2);
    }
    GemVal _t4459;
    if (!gem_truthy(_t4458)) {
        _t4459 = _t4458;
    } else {
        _t4459 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("match"));
    }
    GemVal _t4461;
    if (!gem_truthy(_t4459)) {
        _t4461 = _t4459;
    } else {
        GemVal _t4460[] = {gem_v__match_4, gem_string("target")};
        _t4461 = gem_has_key_fn(NULL, _t4460, 2);
    }
    GemVal _t4463;
    if (!gem_truthy(_t4461)) {
        _t4463 = _t4461;
    } else {
        GemVal _t4462[] = {gem_v__match_4, gem_string("whens")};
        _t4463 = gem_has_key_fn(NULL, _t4462, 2);
    }
        if (gem_truthy(_t4463)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_target = gem_table_get(gem_v__match_4, gem_string("target"));
#line 0 "compiler/main.gem"
            GemVal gem_v_whens = gem_table_get(gem_v__match_4, gem_string("whens"));
#line 188 "compiler/main.gem"
    GemVal _t4464[] = {gem_v_target, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4464, 3));
#line 189 "compiler/main.gem"
            GemVal gem_v__for_items_8 = gem_v_whens;
#line 189 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 189 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4465[] = {gem_v__for_items_8};
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_len_fn(NULL, _t4465, 1)))) break;
#line 189 "compiler/main.gem"
                GemVal gem_v_w = gem_table_get(gem_v__for_items_8, gem_v__for_i_8);
#line 189 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 190 "compiler/main.gem"
    GemVal _t4466 = gem_v_w;
    static GemICacheSlot _ic_624 = {0};
    GemVal _t4467[] = {gem_table_get_cached(_t4466, "value", &_ic_624), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4467, 3));
#line 191 "compiler/main.gem"
    GemVal _t4468 = gem_v_w;
    static GemICacheSlot _ic_625 = {0};
                if (gem_truthy(gem_neq(gem_table_get_cached(_t4468, "bindings", &_ic_625), GEM_NIL))) {
#line 192 "compiler/main.gem"
    GemVal _t4469 = gem_v_w;
    static GemICacheSlot _ic_626 = {0};
                    GemVal gem_v__for_items_9 = gem_table_get_cached(_t4469, "bindings", &_ic_626);
#line 192 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 192 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4470[] = {gem_v__for_items_9};
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t4470, 1)))) break;
#line 192 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_9, gem_v__for_i_9);
#line 192 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t4471[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4471, 3));
                    }

                }
#line 196 "compiler/main.gem"
    GemVal _t4472 = gem_v_w;
    static GemICacheSlot _ic_627 = {0};
    GemVal _t4473[] = {gem_table_get_cached(_t4472, "body", &_ic_627), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4473, 3));
            }

#line 198 "compiler/main.gem"
    GemVal _t4474 = gem_v_node;
    static GemICacheSlot _ic_628 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4474, "else", &_ic_628), GEM_NIL))) {
#line 199 "compiler/main.gem"
    GemVal _t4475 = gem_v_node;
    static GemICacheSlot _ic_629 = {0};
    GemVal _t4476[] = {gem_table_get_cached(_t4475, "else", &_ic_629), gem_v_rmap, gem_v_shadowed};
                GemVal _t4477 = gem_fn_rename_stmts(NULL, _t4476, 3);
                gem_pop_frame();
                return _t4477;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4478[] = {gem_v__match_4};
    GemVal _t4480;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4478, 1), gem_string("table")))) {
        _t4480 = gem_eq(gem_type_fn(NULL, _t4478, 1), gem_string("table"));
    } else {
        GemVal _t4479[] = {gem_v__match_4, gem_string("tag")};
        _t4480 = gem_has_key_fn(NULL, _t4479, 2);
    }
    GemVal _t4481;
    if (!gem_truthy(_t4480)) {
        _t4481 = _t4480;
    } else {
        _t4481 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t4483;
    if (!gem_truthy(_t4481)) {
        _t4483 = _t4481;
    } else {
        GemVal _t4482[] = {gem_v__match_4, gem_string("arms")};
        _t4483 = gem_has_key_fn(NULL, _t4482, 2);
    }
        if (gem_truthy(_t4483)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_arms = gem_table_get(gem_v__match_4, gem_string("arms"));
#line 202 "compiler/main.gem"
            GemVal gem_v__for_items_10 = gem_v_arms;
#line 202 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 202 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4484[] = {gem_v__for_items_10};
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t4484, 1)))) break;
#line 202 "compiler/main.gem"
                GemVal gem_v_arm = gem_table_get(gem_v__for_items_10, gem_v__for_i_10);
#line 202 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 203 "compiler/main.gem"
    GemVal _t4485 = gem_v_arm;
    static GemICacheSlot _ic_630 = {0};
    GemVal _t4486 = gem_table_get_cached(_t4485, "pattern", &_ic_630);
    static GemICacheSlot _ic_631 = {0};
    GemVal _t4487[] = {gem_table_get_cached(_t4486, "condition", &_ic_631), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4487, 3));
#line 204 "compiler/main.gem"
    GemVal _t4488 = gem_v_arm;
    static GemICacheSlot _ic_632 = {0};
    GemVal _t4489 = gem_table_get_cached(_t4488, "pattern", &_ic_632);
    static GemICacheSlot _ic_633 = {0};
                if (gem_truthy(gem_neq(gem_table_get_cached(_t4489, "bindings", &_ic_633), GEM_NIL))) {
#line 205 "compiler/main.gem"
    GemVal _t4490 = gem_v_arm;
    static GemICacheSlot _ic_634 = {0};
    GemVal _t4491 = gem_table_get_cached(_t4490, "pattern", &_ic_634);
    static GemICacheSlot _ic_635 = {0};
                    GemVal gem_v__for_items_11 = gem_table_get_cached(_t4491, "bindings", &_ic_635);
#line 205 "compiler/main.gem"
                    GemVal gem_v__for_i_11 = gem_int(0);
#line 205 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4492[] = {gem_v__for_items_11};
                        if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t4492, 1)))) break;
#line 205 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_11, gem_v__for_i_11);
#line 205 "compiler/main.gem"
                        gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 206 "compiler/main.gem"
    GemVal _t4493[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4493, 3));
                    }

                }
#line 209 "compiler/main.gem"
    GemVal _t4494 = gem_v_arm;
    static GemICacheSlot _ic_636 = {0};
    GemVal _t4495[] = {gem_table_get_cached(_t4494, "body", &_ic_636), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4495, 3));
            }

#line 211 "compiler/main.gem"
    GemVal _t4496 = gem_v_node;
    static GemICacheSlot _ic_637 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4496, "after_ms", &_ic_637), GEM_NIL))) {
#line 212 "compiler/main.gem"
    GemVal _t4497 = gem_v_node;
    static GemICacheSlot _ic_638 = {0};
    GemVal _t4498[] = {gem_table_get_cached(_t4497, "after_ms", &_ic_638), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4498, 3));
            }
#line 214 "compiler/main.gem"
    GemVal _t4499 = gem_v_node;
    static GemICacheSlot _ic_639 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4499, "after_body", &_ic_639), GEM_NIL))) {
#line 215 "compiler/main.gem"
    GemVal _t4500 = gem_v_node;
    static GemICacheSlot _ic_640 = {0};
    GemVal _t4501[] = {gem_table_get_cached(_t4500, "after_body", &_ic_640), gem_v_rmap, gem_v_shadowed};
                GemVal _t4502 = gem_fn_rename_stmts(NULL, _t4501, 3);
                gem_pop_frame();
                return _t4502;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4503[] = {gem_v__match_4};
    GemVal _t4505;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4503, 1), gem_string("table")))) {
        _t4505 = gem_eq(gem_type_fn(NULL, _t4503, 1), gem_string("table"));
    } else {
        GemVal _t4504[] = {gem_v__match_4, gem_string("tag")};
        _t4505 = gem_has_key_fn(NULL, _t4504, 2);
    }
    GemVal _t4506;
    if (!gem_truthy(_t4505)) {
        _t4506 = _t4505;
    } else {
        _t4506 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("return"));
    }
    GemVal _t4508;
    if (!gem_truthy(_t4506)) {
        _t4508 = _t4506;
    } else {
        GemVal _t4507[] = {gem_v__match_4, gem_string("value")};
        _t4508 = gem_has_key_fn(NULL, _t4507, 2);
    }
        if (gem_truthy(_t4508)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 218 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 219 "compiler/main.gem"
                GemVal _t4509 = gem_v_value;
                GemVal _t4510 = gem_v_rmap;
                GemVal _t4511 = gem_v_shadowed;
                gem_v_node = _t4509;
                gem_v_rmap = _t4510;
                gem_v_shadowed = _t4511;
                continue;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4512[] = {gem_v__match_4};
    GemVal _t4514;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4512, 1), gem_string("table")))) {
        _t4514 = gem_eq(gem_type_fn(NULL, _t4512, 1), gem_string("table"));
    } else {
        GemVal _t4513[] = {gem_v__match_4, gem_string("tag")};
        _t4514 = gem_has_key_fn(NULL, _t4513, 2);
    }
    GemVal _t4515;
    if (!gem_truthy(_t4514)) {
        _t4515 = _t4514;
    } else {
        _t4515 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("table"));
    }
    GemVal _t4517;
    if (!gem_truthy(_t4515)) {
        _t4517 = _t4515;
    } else {
        GemVal _t4516[] = {gem_v__match_4, gem_string("entries")};
        _t4517 = gem_has_key_fn(NULL, _t4516, 2);
    }
        if (gem_truthy(_t4517)) {
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
                    GemVal _t4518[] = {gem_v__for_items_12};
                    if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t4518, 1)))) break;
#line 222 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 222 "compiler/main.gem"
                    gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 223 "compiler/main.gem"
    GemVal _t4519 = gem_v_e;
    static GemICacheSlot _ic_641 = {0};
    GemVal _t4520[] = {gem_table_get_cached(_t4519, "value", &_ic_641), gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4520, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4521[] = {gem_v__match_4};
    GemVal _t4523;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4521, 1), gem_string("table")))) {
        _t4523 = gem_eq(gem_type_fn(NULL, _t4521, 1), gem_string("table"));
    } else {
        GemVal _t4522[] = {gem_v__match_4, gem_string("tag")};
        _t4523 = gem_has_key_fn(NULL, _t4522, 2);
    }
    GemVal _t4524;
    if (!gem_truthy(_t4523)) {
        _t4524 = _t4523;
    } else {
        _t4524 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("array"));
    }
    GemVal _t4526;
    if (!gem_truthy(_t4524)) {
        _t4526 = _t4524;
    } else {
        GemVal _t4525[] = {gem_v__match_4, gem_string("elements")};
        _t4526 = gem_has_key_fn(NULL, _t4525, 2);
    }
        if (gem_truthy(_t4526)) {
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
                    GemVal _t4527[] = {gem_v__for_items_13};
                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_len_fn(NULL, _t4527, 1)))) break;
#line 226 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_13, gem_v__for_i_13);
#line 226 "compiler/main.gem"
                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 227 "compiler/main.gem"
    GemVal _t4528[] = {gem_v_e, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4528, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4529[] = {gem_v__match_4};
    GemVal _t4531;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4529, 1), gem_string("table")))) {
        _t4531 = gem_eq(gem_type_fn(NULL, _t4529, 1), gem_string("table"));
    } else {
        GemVal _t4530[] = {gem_v__match_4, gem_string("tag")};
        _t4531 = gem_has_key_fn(NULL, _t4530, 2);
    }
    GemVal _t4532;
    if (!gem_truthy(_t4531)) {
        _t4532 = _t4531;
    } else {
        _t4532 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t4534;
    if (!gem_truthy(_t4532)) {
        _t4534 = _t4532;
    } else {
        GemVal _t4533[] = {gem_v__match_4, gem_string("parts")};
        _t4534 = gem_has_key_fn(NULL, _t4533, 2);
    }
        if (gem_truthy(_t4534)) {
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
                    GemVal _t4535[] = {gem_v__for_items_14};
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_len_fn(NULL, _t4535, 1)))) break;
#line 230 "compiler/main.gem"
                    GemVal gem_v_p = gem_table_get(gem_v__for_items_14, gem_v__for_i_14);
#line 230 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 231 "compiler/main.gem"
    GemVal _t4536[] = {gem_v_p, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4536, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4537[] = {gem_v__match_4};
    GemVal _t4539;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4537, 1), gem_string("table")))) {
        _t4539 = gem_eq(gem_type_fn(NULL, _t4537, 1), gem_string("table"));
    } else {
        GemVal _t4538[] = {gem_v__match_4, gem_string("tag")};
        _t4539 = gem_has_key_fn(NULL, _t4538, 2);
    }
    GemVal _t4540;
    if (!gem_truthy(_t4539)) {
        _t4540 = _t4539;
    } else {
        _t4540 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("block"));
    }
    GemVal _t4542;
    if (!gem_truthy(_t4540)) {
        _t4542 = _t4540;
    } else {
        GemVal _t4541[] = {gem_v__match_4, gem_string("stmts")};
        _t4542 = gem_has_key_fn(NULL, _t4541, 2);
    }
        if (gem_truthy(_t4542)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_stmts = gem_table_get(gem_v__match_4, gem_string("stmts"));
#line 234 "compiler/main.gem"
    GemVal _t4543[] = {gem_v_stmts, gem_v_rmap, gem_v_shadowed};
            GemVal _t4544 = gem_fn_rename_stmts(NULL, _t4543, 3);
            gem_pop_frame();
            return _t4544;
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
    GemVal _t4545[] = {gem_v_shadowed};
    GemVal gem_v_current = gem_fn_copy_set(NULL, _t4545, 1);
    {
#line 240 "compiler/main.gem"
        GemVal gem_v__for_items_15 = gem_v_stmts;
#line 240 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 240 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4546[] = {gem_v__for_items_15};
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_len_fn(NULL, _t4546, 1)))) break;
#line 240 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_15, gem_v__for_i_15);
#line 240 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 241 "compiler/main.gem"
    GemVal _t4547[] = {gem_v_stmt, gem_v_rmap, gem_v_current};
            (void)(gem_fn_rename_node(NULL, _t4547, 3));
#line 242 "compiler/main.gem"
    GemVal _t4548[] = {gem_v_stmt, gem_string("let")};
    GemVal _t4551;
    if (!gem_truthy(gem_fn_is_node(NULL, _t4548, 2))) {
        _t4551 = gem_fn_is_node(NULL, _t4548, 2);
    } else {
        GemVal _t4549 = gem_v_stmt;
        static GemICacheSlot _ic_642 = {0};
        GemVal _t4550[] = {gem_v_rmap, gem_table_get_cached(_t4549, "name", &_ic_642)};
        _t4551 = gem_not(gem_has_key_fn(NULL, _t4550, 2));
    }
            if (gem_truthy(_t4551)) {
#line 243 "compiler/main.gem"
    GemVal _t4552 = gem_v_stmt;
    static GemICacheSlot _ic_643 = {0};
                gem_table_set(gem_v_current, gem_table_get_cached(_t4552, "name", &_ic_643), gem_bool(1));
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
    GemVal _t4553[] = {gem_v_stmts};
    GemVal gem_v_top_names = gem_fn_collect_module_top_names(NULL, _t4553, 1);
#line 251 "compiler/main.gem"
    GemVal _t4554[] = {gem_v_top_names, gem_v_prefix};
    GemVal gem_v_rmap = gem_fn_build_rename_map(NULL, _t4554, 2);
#line 253 "compiler/main.gem"
    GemVal _t4555 = gem_table_new();
    GemVal _t4556[] = {gem_v_stmts, gem_v_rmap, _t4555};
    (void)(gem_fn_rename_stmts(NULL, _t4556, 3));
#line 255 "compiler/main.gem"
    GemVal _t4557 = gem_table_new();
    GemVal gem_v_result = _t4557;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_items_16 = gem_v_stmts;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_i_16 = gem_int(0);
#line 256 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4558[] = {gem_v__for_items_16};
        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t4558, 1)))) break;
#line 256 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_16, gem_v__for_i_16);
#line 256 "compiler/main.gem"
        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 257 "compiler/main.gem"
    GemVal _t4559[] = {gem_v_stmt, gem_string("export")};
        if (gem_truthy(gem_not(gem_fn_is_node(NULL, _t4559, 2)))) {
#line 258 "compiler/main.gem"
    GemVal _t4560[] = {gem_v_result, gem_v_stmt};
            (void)(gem_push_fn(NULL, _t4560, 2));
        }
    }

#line 262 "compiler/main.gem"
    GemVal _t4561 = gem_v_export_node;
    static GemICacheSlot _ic_644 = {0};
    GemVal gem_v_exported = gem_table_get_cached(_t4561, "names", &_ic_644);
#line 264 "compiler/main.gem"
    GemVal _t4562 = gem_v_load_node;
    static GemICacheSlot _ic_645 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4562, "selective", &_ic_645), GEM_NIL))) {
#line 265 "compiler/main.gem"
    GemVal _t4563 = gem_v_load_node;
    static GemICacheSlot _ic_646 = {0};
        GemVal gem_v__for_items_17 = gem_table_get_cached(_t4563, "selective", &_ic_646);
#line 265 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 265 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4564[] = {gem_v__for_items_17};
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_len_fn(NULL, _t4564, 1)))) break;
#line 265 "compiler/main.gem"
            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_17, gem_v__for_i_17);
#line 265 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 266 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_sel_name);
#line 267 "compiler/main.gem"
    GemVal _t4565[] = {gem_v_prefixed};
    GemVal _t4566[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4565, 1), gem_int(0)};
    GemVal _t4567[] = {gem_v_result, gem_fn_make_let(NULL, _t4566, 3)};
            (void)(gem_push_fn(NULL, _t4567, 2));
        }

    } else {
#line 270 "compiler/main.gem"
        GemVal gem_v_binding_name = gem_v_mod_name;
#line 271 "compiler/main.gem"
    GemVal _t4568 = gem_v_load_node;
    static GemICacheSlot _ic_647 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t4568, "alias", &_ic_647), GEM_NIL))) {
#line 272 "compiler/main.gem"
    GemVal _t4569 = gem_v_load_node;
    static GemICacheSlot _ic_648 = {0};
            gem_v_binding_name = gem_table_get_cached(_t4569, "alias", &_ic_648);
        }
#line 274 "compiler/main.gem"
    GemVal _t4570 = gem_table_new();
        GemVal gem_v_entries = _t4570;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_items_18 = gem_v_exported;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 275 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4571[] = {gem_v__for_items_18};
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_len_fn(NULL, _t4571, 1)))) break;
#line 275 "compiler/main.gem"
            GemVal gem_v_ename = gem_table_get(gem_v__for_items_18, gem_v__for_i_18);
#line 275 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 276 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_ename);
#line 277 "compiler/main.gem"
    GemVal _t4572[] = {gem_v_prefixed};
    GemVal _t4573[] = {gem_v_ename, gem_fn_make_var(NULL, _t4572, 1)};
    GemVal _t4574[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4573, 2)};
            (void)(gem_push_fn(NULL, _t4574, 2));
        }

#line 279 "compiler/main.gem"
    GemVal _t4575[] = {gem_v_entries};
    GemVal _t4576[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4575, 1), gem_int(0)};
    GemVal _t4577[] = {gem_v_result, gem_fn_make_let(NULL, _t4576, 3)};
        (void)(gem_push_fn(NULL, _t4577, 2));
    }
    GemVal _t4578 = gem_v_result;
    gem_pop_frame();
    return _t4578;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 287 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 287);
#line 288 "compiler/main.gem"
    GemVal _t4579 = gem_table_new();
    GemVal gem_v_new_stmts = _t4579;
#line 289 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 290 "compiler/main.gem"
    GemVal _t4580 = gem_v_ast;
    static GemICacheSlot _ic_649 = {0};
    GemVal gem_v__for_items_19 = gem_table_get_cached(_t4580, "stmts", &_ic_649);
#line 290 "compiler/main.gem"
    GemVal gem_v__for_i_19 = gem_int(0);
#line 290 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4581[] = {gem_v__for_items_19};
        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t4581, 1)))) break;
#line 290 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
#line 290 "compiler/main.gem"
        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 291 "compiler/main.gem"
    GemVal _t4582[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4582, 2))) {
#line 292 "compiler/main.gem"
    GemVal _t4583 = gem_v_stmt;
    static GemICacheSlot _ic_650 = {0};
            GemVal gem_v_path = gem_table_get_cached(_t4583, "path", &_ic_650);
#line 293 "compiler/main.gem"
    GemVal _t4584 = gem_v_string;
    static GemICacheSlot _ic_651 = {0};
    GemVal _t4585[] = {gem_v_path, gem_string(".gem")};
    GemVal _t4586 = gem_table_get_cached(_t4584, "ends_with", &_ic_651);
            if (gem_truthy(gem_not(_t4586.fn(_t4586.env, _t4585, 2)))) {
#line 294 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 297 "compiler/main.gem"
    GemVal _t4587[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t4587, 2);
#line 298 "compiler/main.gem"
    GemVal _t4588[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t4588, 1)))) {
#line 299 "compiler/main.gem"
    GemVal _t4589[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t4589, 2);
            }
#line 302 "compiler/main.gem"
    GemVal _t4590[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t4590, 1);
#line 304 "compiler/main.gem"
            GemVal gem_v_cached = gem_table_get(gem_v_loaded, gem_v_full_path);
#line 305 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_cached, GEM_NIL))) {
#line 306 "compiler/main.gem"
    GemVal _t4591 = gem_v_cached;
    static GemICacheSlot _ic_652 = {0};
                if (gem_truthy(gem_neq(gem_table_get_cached(_t4591, "exported", &_ic_652), GEM_NIL))) {
#line 307 "compiler/main.gem"
    GemVal _t4592 = gem_v_cached;
    static GemICacheSlot _ic_653 = {0};
                    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_table_get_cached(_t4592, "mod_name", &_ic_653)), gem_string("_"));
#line 308 "compiler/main.gem"
    GemVal _t4593 = gem_v_stmt;
    static GemICacheSlot _ic_654 = {0};
                    if (gem_truthy(gem_neq(gem_table_get_cached(_t4593, "selective", &_ic_654), GEM_NIL))) {
#line 309 "compiler/main.gem"
    GemVal _t4594 = gem_v_stmt;
    static GemICacheSlot _ic_655 = {0};
                        GemVal gem_v__for_items_20 = gem_table_get_cached(_t4594, "selective", &_ic_655);
#line 309 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(0);
#line 309 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4595[] = {gem_v__for_items_20};
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t4595, 1)))) break;
#line 309 "compiler/main.gem"
                            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_20, gem_v__for_i_20);
#line 309 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 310 "compiler/main.gem"
    GemVal _t4596[] = {gem_add(gem_v_prefix, gem_v_sel_name)};
    GemVal _t4597[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4596, 1), gem_int(0)};
                            gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4597, 3));
#line 311 "compiler/main.gem"
                            gem_v_count = gem_add(gem_v_count, gem_int(1));
                        }

                    } else {
#line 314 "compiler/main.gem"
    GemVal _t4598 = gem_v_cached;
    static GemICacheSlot _ic_656 = {0};
                        GemVal gem_v_binding_name = gem_table_get_cached(_t4598, "mod_name", &_ic_656);
#line 315 "compiler/main.gem"
    GemVal _t4599 = gem_v_stmt;
    static GemICacheSlot _ic_657 = {0};
                        if (gem_truthy(gem_neq(gem_table_get_cached(_t4599, "alias", &_ic_657), GEM_NIL))) {
#line 316 "compiler/main.gem"
    GemVal _t4600 = gem_v_stmt;
    static GemICacheSlot _ic_658 = {0};
                            gem_v_binding_name = gem_table_get_cached(_t4600, "alias", &_ic_658);
                        }
#line 318 "compiler/main.gem"
    GemVal _t4601 = gem_table_new();
                        GemVal gem_v_entries = _t4601;
#line 319 "compiler/main.gem"
    GemVal _t4602 = gem_v_cached;
    static GemICacheSlot _ic_659 = {0};
                        GemVal gem_v__for_items_21 = gem_table_get_cached(_t4602, "exported", &_ic_659);
#line 319 "compiler/main.gem"
                        GemVal gem_v__for_i_21 = gem_int(0);
#line 319 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4603[] = {gem_v__for_items_21};
                            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t4603, 1)))) break;
#line 319 "compiler/main.gem"
                            GemVal gem_v_ename = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 319 "compiler/main.gem"
                            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 320 "compiler/main.gem"
    GemVal _t4604[] = {gem_add(gem_v_prefix, gem_v_ename)};
    GemVal _t4605[] = {gem_v_ename, gem_fn_make_var(NULL, _t4604, 1)};
    GemVal _t4606[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4605, 2)};
                            (void)(gem_push_fn(NULL, _t4606, 2));
                        }

#line 322 "compiler/main.gem"
    GemVal _t4607[] = {gem_v_entries};
    GemVal _t4608[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4607, 1), gem_int(0)};
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4608, 3));
#line 323 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
                continue;
            }
#line 329 "compiler/main.gem"
    GemVal _t4609 = gem_table_new();
    gem_table_set(_t4609, gem_string("exported"), GEM_NIL);
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4609);
#line 331 "compiler/main.gem"
    GemVal _t4610[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t4610, 1);
#line 332 "compiler/main.gem"
    GemVal _t4611[] = {gem_v_source, gem_v_full_path};
    GemVal _t4612 = gem_v_parse_source;
            GemVal gem_v_loaded_ast = _t4612.fn(_t4612.env, _t4611, 2);
#line 333 "compiler/main.gem"
    GemVal _t4613[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t4613, 1);
#line 334 "compiler/main.gem"
    GemVal _t4614[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t4614, 3);
#line 336 "compiler/main.gem"
    GemVal _t4615 = gem_v_resolved;
    static GemICacheSlot _ic_660 = {0};
    GemVal _t4616[] = {gem_table_get_cached(_t4615, "stmts", &_ic_660)};
            GemVal gem_v_export_node = gem_fn_find_export_node(NULL, _t4616, 1);
#line 337 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_export_node, GEM_NIL))) {
#line 338 "compiler/main.gem"
    GemVal _t4617[] = {gem_add(gem_string("gem: loaded file has no export statement: "), gem_v_full_path)};
                (void)(gem_error_at_fn("compiler/main.gem", 338, _t4617, 1));
            }
#line 341 "compiler/main.gem"
    GemVal _t4618 = gem_v_stmt;
    static GemICacheSlot _ic_661 = {0};
    GemVal _t4619[] = {gem_table_get_cached(_t4618, "path", &_ic_661)};
            GemVal gem_v_mod_name = gem_fn_module_name_from_path(NULL, _t4619, 1);
#line 342 "compiler/main.gem"
    GemVal _t4620 = gem_table_new();
    gem_table_set(_t4620, gem_string("mod_name"), gem_v_mod_name);
    GemVal _t4621 = gem_v_export_node;
    static GemICacheSlot _ic_662 = {0};
    gem_table_set(_t4620, gem_string("exported"), gem_table_get_cached(_t4621, "names", &_ic_662));
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4620);
#line 343 "compiler/main.gem"
    GemVal _t4622 = gem_v_resolved;
    static GemICacheSlot _ic_663 = {0};
    GemVal _t4623[] = {gem_table_get_cached(_t4622, "stmts", &_ic_663), gem_v_mod_name, gem_v_export_node, gem_v_stmt};
            GemVal gem_v_mod_stmts = gem_fn_transform_module(NULL, _t4623, 4);
#line 344 "compiler/main.gem"
            GemVal gem_v__for_items_22 = gem_v_mod_stmts;
#line 344 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 344 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4624[] = {gem_v__for_items_22};
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t4624, 1)))) break;
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
    GemVal _t4625[] = {gem_v_new_stmts};
    GemVal _t4626 = gem_fn_make_program(NULL, _t4625, 1);
    gem_pop_frame();
    return _t4626;
}

static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc) {
#line 358 "compiler/main.gem"
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_args", "compiler/main.gem", 358);
#line 359 "compiler/main.gem"
    GemVal _t4627 = gem_table_new();
    gem_table_set(_t4627, gem_string("src_path"), GEM_NIL);
    gem_table_set(_t4627, gem_string("out_name"), GEM_NIL);
    gem_table_set(_t4627, gem_string("emit_c"), gem_bool(0));
    gem_table_set(_t4627, gem_string("run"), gem_bool(0));
    GemVal _t4628 = gem_table_new();
    gem_table_set(_t4627, gem_string("run_args"), _t4628);
    GemVal gem_v_result = _t4627;
#line 367 "compiler/main.gem"
    GemVal _t4629[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t4629, 1);
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
    GemVal _t4630 = gem_v_result;
    static GemICacheSlot _ic_664 = {0};
            gem_table_set(gem_table_get_cached(_t4630, "run_args", &_ic_664), gem_v_run_arg_count, gem_v_arg);
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
    GemVal _t4631[] = {gem_string("gem: -o requires an argument")};
                            (void)(gem_error_at_fn("compiler/main.gem", 385, _t4631, 1));
                        }
#line 387 "compiler/main.gem"
                        gem_table_set(gem_v_result, gem_string("out_name"), gem_table_get(gem_v_args, gem_v_i));
                    } else {
#line 388 "compiler/main.gem"
    GemVal _t4632 = gem_v_result;
    static GemICacheSlot _ic_665 = {0};
                        if (gem_truthy(gem_eq(gem_table_get_cached(_t4632, "src_path", &_ic_665), GEM_NIL))) {
#line 389 "compiler/main.gem"
                            gem_table_set(gem_v_result, gem_string("src_path"), gem_v_arg);
                        } else {
#line 392 "compiler/main.gem"
    GemVal _t4633 = gem_v_result;
    static GemICacheSlot _ic_666 = {0};
                            gem_table_set(gem_table_get_cached(_t4633, "run_args", &_ic_666), gem_v_run_arg_count, gem_v_arg);
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
    GemVal _t4634 = gem_v_result;
    gem_pop_frame();
    return _t4634;
}

static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc) {
#line 403 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("basename_no_ext", "compiler/main.gem", 403);
#line 405 "compiler/main.gem"
    GemVal _t4635[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4635, 1);
#line 406 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 407 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 408 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 409 "compiler/main.gem"
    GemVal _t4636[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4636, 2), gem_int(47)))) {
#line 410 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 412 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 414 "compiler/main.gem"
    GemVal _t4637[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4637, 3);
#line 416 "compiler/main.gem"
    GemVal _t4638[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4638, 1);
#line 417 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 418 "compiler/main.gem"
    GemVal _t4639[] = {gem_v_name, gem_sub(gem_v_nlen, gem_int(4)), gem_int(4)};
        GemVal gem_v_maybe_ext = gem_substr_fn(NULL, _t4639, 3);
#line 419 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_maybe_ext, gem_string(".gem")))) {
#line 420 "compiler/main.gem"
    GemVal _t4640[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4640, 3);
        }
    }
    GemVal _t4641 = gem_v_name;
    gem_pop_frame();
    return _t4641;
}

static GemVal gem_user_main(void *_env, GemVal *_args, int _argc) {
    (void)_env; (void)_args; (void)_argc;
    gem_push_frame("main", "compiler/main.gem", 0);
#line 0 "compiler/main.gem"
    GemVal _t4642 = gem_table_new();
    gem_table_set(_t4642, gem_string("split"), gem_make_fn(gem_fn__mod_string_split, NULL));
    gem_table_set(_t4642, gem_string("index_of"), gem_make_fn(gem_fn__mod_string_index_of, NULL));
    gem_table_set(_t4642, gem_string("join"), gem_make_fn(gem_fn__mod_string_join, NULL));
    gem_table_set(_t4642, gem_string("trim"), gem_make_fn(gem_fn__mod_string_trim, NULL));
    gem_table_set(_t4642, gem_string("starts_with"), gem_make_fn(gem_fn__mod_string_starts_with, NULL));
    gem_table_set(_t4642, gem_string("ends_with"), gem_make_fn(gem_fn__mod_string_ends_with, NULL));
    gem_table_set(_t4642, gem_string("upper"), gem_make_fn(gem_fn__mod_string_upper, NULL));
    gem_table_set(_t4642, gem_string("lower"), gem_make_fn(gem_fn__mod_string_lower, NULL));
    gem_table_set(_t4642, gem_string("contains"), gem_make_fn(gem_fn__mod_string_contains, NULL));
    gem_table_set(_t4642, gem_string("repeat"), gem_make_fn(gem_fn__mod_string_repeat, NULL));
    gem_v_string = _t4642;
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
    GemVal _t4643[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t4644[] = {gem_dirname_fn(NULL, _t4643, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t4644, 1);
#line 428 "compiler/main.gem"
    GemVal _t4645[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t4645, 1);
#line 429 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 430 "compiler/main.gem"
    GemVal _t4646[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 430, _t4646, 1));
    }
#line 433 "compiler/main.gem"
    GemVal _t4647[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_cli = gem_fn_parse_args(NULL, _t4647, 1);
#line 435 "compiler/main.gem"
    GemVal _t4648 = gem_v_cli;
    static GemICacheSlot _ic_667 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t4648, "src_path", &_ic_667), GEM_NIL))) {
#line 436 "compiler/main.gem"
    GemVal _t4649[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 436, _t4649, 1));
    }
#line 439 "compiler/main.gem"
    GemVal _t4650 = gem_v_cli;
    static GemICacheSlot _ic_668 = {0};
    gem_v_src_path = gem_table_get_cached(_t4650, "src_path", &_ic_668);
#line 440 "compiler/main.gem"
    GemVal _t4651[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t4651, 1);
#line 442 "compiler/main.gem"
    GemVal _t4652[] = {gem_v_source, gem_v_src_path};
    GemVal _t4653 = gem_v_parse_source;
    gem_v_ast = _t4653.fn(_t4653.env, _t4652, 2);
#line 443 "compiler/main.gem"
    GemVal _t4654[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t4654, 1);
#line 444 "compiler/main.gem"
    GemVal _t4655 = gem_table_new();
    gem_v_loaded = _t4655;
#line 445 "compiler/main.gem"
    GemVal _t4656[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t4656, 3);
#line 447 "compiler/main.gem"
    GemVal _t4657[] = {gem_v_src_path};
    GemVal _t4658 = gem_v_make_codegen;
    gem_v_cg = _t4658.fn(_t4658.env, _t4657, 1);
#line 448 "compiler/main.gem"
    GemVal _t4659 = gem_v_cg;
    static GemICacheSlot _ic_669 = {0};
    GemVal _t4660[] = {gem_v_resolved_ast};
    GemVal _t4661 = gem_table_get_cached(_t4659, "compile", &_ic_669);
    gem_v_c_code = _t4661.fn(_t4661.env, _t4660, 1);
#line 452 "compiler/main.gem"
    GemVal _t4662 = gem_v_cli;
    static GemICacheSlot _ic_670 = {0};
    if (gem_truthy(gem_table_get_cached(_t4662, "emit_c", &_ic_670))) {
#line 453 "compiler/main.gem"
    GemVal _t4663[] = {gem_v_c_code};
        (void)(gem_print(NULL, _t4663, 1));
    } else {
#line 457 "compiler/main.gem"
    GemVal _t4664[] = {gem_v_src_path};
        GemVal gem_v_base = gem_fn_basename_no_ext(NULL, _t4664, 1);
#line 458 "compiler/main.gem"
        GemVal gem_v_tmp_c = gem_add(gem_add(gem_string("/tmp/gem_"), gem_v_base), gem_string(".c"));
#line 461 "compiler/main.gem"
    GemVal _t4665 = gem_v_cli;
    static GemICacheSlot _ic_671 = {0};
        GemVal gem_v_out_name = gem_table_get_cached(_t4665, "out_name", &_ic_671);
#line 462 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_out_name, GEM_NIL))) {
#line 463 "compiler/main.gem"
            gem_v_out_name = gem_add(gem_string("./"), gem_v_base);
        }
#line 467 "compiler/main.gem"
    GemVal _t4666[] = {gem_v_tmp_c, gem_v_c_code};
        (void)(gem_write_file_fn(NULL, _t4666, 2));
#line 470 "compiler/main.gem"
    GemVal _t4667[] = {gem_v_install_root, gem_string("runtime")};
        GemVal gem_v_runtime_dir = gem_path_join_fn(NULL, _t4667, 2);
#line 473 "compiler/main.gem"
        GemVal gem_v_gc_flags = gem_string("-lgc");
#line 474 "compiler/main.gem"
    GemVal _t4668[] = {gem_string("pkg-config --cflags --libs bdw-gc > /tmp/gem_gc_flags.txt 2>/dev/null")};
        GemVal gem_v_pkg_ret = gem_exec_fn(NULL, _t4668, 1);
#line 475 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_pkg_ret, gem_int(0)))) {
#line 476 "compiler/main.gem"
    GemVal _t4669[] = {gem_string("/tmp/gem_gc_flags.txt")};
            gem_v_gc_flags = gem_read_file_fn(NULL, _t4669, 1);
#line 477 "compiler/main.gem"
    GemVal _t4670[] = {gem_v_gc_flags, gem_string("\n"), gem_string("")};
            gem_v_gc_flags = gem_str_replace_fn(NULL, _t4670, 3);
        }
#line 480 "compiler/main.gem"
    GemVal _t4671[] = {gem_v_install_root, gem_string("build/libgem_runtime.a")};
        GemVal gem_v_runtime_lib = gem_path_join_fn(NULL, _t4671, 2);
#line 481 "compiler/main.gem"
        GemVal gem_v_cc_cmd = gem_string("");
#line 482 "compiler/main.gem"
    GemVal _t4672[] = {gem_v_runtime_lib};
        if (gem_truthy(gem_file_exists_fn(NULL, _t4672, 1))) {
#line 483 "compiler/main.gem"
            gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread ")), gem_v_runtime_lib);
        } else {
#line 485 "compiler/main.gem"
            gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" ")), gem_v_runtime_dir), gem_string("/gem_*.c ")), gem_v_runtime_dir), gem_string("/sqlite3.c -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread"));
        }
#line 488 "compiler/main.gem"
    GemVal _t4673[] = {gem_v_cc_cmd};
        GemVal gem_v_cc_ret = gem_exec_fn(NULL, _t4673, 1);
#line 489 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_cc_ret, gem_int(0)))) {
#line 490 "compiler/main.gem"
    GemVal _t4674[] = {gem_v_cc_ret};
    GemVal _t4675[] = {gem_add(gem_add(gem_string("gem: compilation failed (cc exited with "), gem_to_string_fn(NULL, _t4674, 1)), gem_string(")"))};
            (void)(gem_error_at_fn("compiler/main.gem", 490, _t4675, 1));
        }
#line 495 "compiler/main.gem"
    GemVal _t4676 = gem_v_cli;
    static GemICacheSlot _ic_672 = {0};
        if (gem_truthy(gem_table_get_cached(_t4676, "run", &_ic_672))) {
#line 496 "compiler/main.gem"
            GemVal gem_v_run_cmd = gem_v_out_name;
#line 497 "compiler/main.gem"
            GemVal gem_v_i = gem_int(0);
#line 498 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4677 = gem_v_cli;
                static GemICacheSlot _ic_673 = {0};
                GemVal _t4678[] = {gem_table_get_cached(_t4677, "run_args", &_ic_673)};
                if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t4678, 1)))) break;
#line 499 "compiler/main.gem"
    GemVal _t4679 = gem_v_cli;
    static GemICacheSlot _ic_674 = {0};
                gem_v_run_cmd = gem_add(gem_add(gem_v_run_cmd, gem_string(" ")), gem_table_get(gem_table_get_cached(_t4679, "run_args", &_ic_674), gem_v_i));
#line 500 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 502 "compiler/main.gem"
    GemVal _t4680[] = {gem_v_run_cmd};
            GemVal gem_v_run_ret = gem_exec_fn(NULL, _t4680, 1);
#line 503 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_run_ret, gem_int(0)))) {
#line 504 "compiler/main.gem"
    GemVal _t4681[] = {gem_v_run_ret};
                (void)(gem_exit_process_fn(NULL, _t4681, 1));
            }
        }
    }
    gem_pop_frame();
    return GEM_NIL;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_run_main(gem_user_main, NULL);
    return 0;
}

