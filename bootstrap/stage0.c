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
    GemVal _t459 = gem_table_new();
    GemVal gem_v_params = _t459;
#line 113 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 115 "compiler/main.gem"
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
#line 116 "compiler/main.gem"
    GemVal _t465 = (*gem_v_peek);
    GemVal _t466 = _t465.fn(_t465.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t466, gem_string("type")), gem_string("...")))) {
#line 117 "compiler/main.gem"
    GemVal _t467 = (*gem_v_advance);
            (void)(_t467.fn(_t467.env, NULL, 0));
#line 118 "compiler/main.gem"
    GemVal _t468[] = {gem_string("NAME")};
    GemVal _t469 = (*gem_v_expect);
    GemVal _t470 = _t469.fn(_t469.env, _t468, 1);
            gem_v_rest_param = gem_table_get(_t470, gem_string("value"));
            break;
        }
#line 121 "compiler/main.gem"
    GemVal _t471 = (*gem_v_advance);
    GemVal _t472 = _t471.fn(_t471.env, NULL, 0);
    GemVal _t473[] = {gem_v_params, gem_table_get(_t472, gem_string("value"))};
        (void)(gem_push_fn(NULL, _t473, 2));
#line 122 "compiler/main.gem"
    GemVal _t474 = (*gem_v_peek);
    GemVal _t475 = _t474.fn(_t474.env, NULL, 0);
        if (gem_truthy(gem_neq(gem_table_get(_t475, gem_string("type")), gem_string(",")))) {
            break;
        }
#line 125 "compiler/main.gem"
    GemVal _t476 = (*gem_v_advance);
        (void)(_t476.fn(_t476.env, NULL, 0));
    }
    GemVal _t477 = gem_table_new();
    gem_table_set(_t477, gem_string("params"), gem_v_params);
    gem_table_set(_t477, gem_string("rest_param"), gem_v_rest_param);
    GemVal _t478 = _t477;
    gem_pop_frame();
    return _t478;
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
    GemVal _t480 = gem_table_new();
    GemVal gem_v_stmts = _t480;
#line 145 "compiler/main.gem"
    GemVal _t481 = (*gem_v_skip_nl);
    (void)(_t481.fn(_t481.env, NULL, 0));
#line 146 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t482 = (*gem_v_peek);
        GemVal _t483 = _t482.fn(_t482.env, NULL, 0);
        GemVal _t486;
        if (!gem_truthy(gem_neq(gem_table_get(_t483, gem_string("type")), gem_string("end")))) {
                _t486 = gem_neq(gem_table_get(_t483, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t484 = (*gem_v_peek);
                GemVal _t485 = _t484.fn(_t484.env, NULL, 0);
                _t486 = gem_neq(gem_table_get(_t485, gem_string("type")), gem_string("elif"));
        }
        GemVal _t489;
        if (!gem_truthy(_t486)) {
                _t489 = _t486;
        } else {
                GemVal _t487 = (*gem_v_peek);
                GemVal _t488 = _t487.fn(_t487.env, NULL, 0);
                _t489 = gem_neq(gem_table_get(_t488, gem_string("type")), gem_string("else"));
        }
        GemVal _t492;
        if (!gem_truthy(_t489)) {
                _t492 = _t489;
        } else {
                GemVal _t490 = (*gem_v_peek);
                GemVal _t491 = _t490.fn(_t490.env, NULL, 0);
                _t492 = gem_neq(gem_table_get(_t491, gem_string("type")), gem_string("when"));
        }
        GemVal _t495;
        if (!gem_truthy(_t492)) {
                _t495 = _t492;
        } else {
                GemVal _t493 = (*gem_v_peek);
                GemVal _t494 = _t493.fn(_t493.env, NULL, 0);
                _t495 = gem_neq(gem_table_get(_t494, gem_string("type")), gem_string("after"));
        }
        GemVal _t498;
        if (!gem_truthy(_t495)) {
                _t498 = _t495;
        } else {
                GemVal _t496 = (*gem_v_peek);
                GemVal _t497 = _t496.fn(_t496.env, NULL, 0);
                _t498 = gem_neq(gem_table_get(_t497, gem_string("type")), gem_string("then"));
        }
        GemVal _t500;
        if (!gem_truthy(_t498)) {
                _t500 = _t498;
        } else {
                GemVal _t499 = (*gem_v_at_end);
                _t500 = gem_not(_t499.fn(_t499.env, NULL, 0));
        }
        if (!gem_truthy(_t500)) break;
#line 147 "compiler/main.gem"
    GemVal _t501 = (*gem_v_parse_stmt);
    GemVal _t502[] = {gem_v_stmts, _t501.fn(_t501.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t502, 2));
#line 148 "compiler/main.gem"
    GemVal _t503 = (*gem_v_skip_nl);
        (void)(_t503.fn(_t503.env, NULL, 0));
    }
    GemVal _t504 = gem_v_stmts;
    gem_pop_frame();
    return _t504;
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
    GemVal _t506 = gem_table_new();
    GemVal gem_v_stmts = _t506;
#line 158 "compiler/main.gem"
    GemVal _t507 = (*gem_v_skip_nl);
    (void)(_t507.fn(_t507.env, NULL, 0));
#line 159 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t508 = (*gem_v_peek);
        GemVal _t509 = _t508.fn(_t508.env, NULL, 0);
        GemVal _t511;
        if (!gem_truthy(gem_neq(gem_table_get(_t509, gem_string("type")), gem_string("end")))) {
                _t511 = gem_neq(gem_table_get(_t509, gem_string("type")), gem_string("end"));
        } else {
                GemVal _t510 = (*gem_v_at_end);
                _t511 = gem_not(_t510.fn(_t510.env, NULL, 0));
        }
        if (!gem_truthy(_t511)) break;
#line 160 "compiler/main.gem"
    GemVal _t512 = (*gem_v_parse_stmt);
    GemVal _t513[] = {gem_v_stmts, _t512.fn(_t512.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t513, 2));
#line 161 "compiler/main.gem"
    GemVal _t514 = (*gem_v_skip_nl);
        (void)(_t514.fn(_t514.env, NULL, 0));
    }
    GemVal _t515 = gem_v_stmts;
    gem_pop_frame();
    return _t515;
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
    GemVal _t517 = (*gem_v_peek);
    GemVal gem_v_t = _t517.fn(_t517.env, NULL, 0);
#line 173 "compiler/main.gem"
    GemVal _t518 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t518, gem_string("type")), gem_string("NUMBER")))) {
#line 174 "compiler/main.gem"
    GemVal _t519 = (*gem_v_advance);
        (void)(_t519.fn(_t519.env, NULL, 0));
#line 175 "compiler/main.gem"
    GemVal _t520 = gem_v_t;
    GemVal _t521[] = {gem_table_get(_t520, gem_string("value"))};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t521, 1))) {
#line 176 "compiler/main.gem"
    GemVal _t522 = gem_v_t;
    GemVal _t523[] = {gem_table_get(_t522, gem_string("value"))};
    GemVal _t524[] = {gem_fn_atof(NULL, _t523, 1)};
    GemVal _t525 = (*gem_v__mod_parser_make_float);
            GemVal _t526 = _t525.fn(_t525.env, _t524, 1);
            gem_pop_frame();
            return _t526;
        }
#line 178 "compiler/main.gem"
    GemVal _t527 = gem_v_t;
    GemVal _t528[] = {gem_table_get(_t527, gem_string("value"))};
    GemVal _t529[] = {gem_fn__mod_parser_str_to_int(NULL, _t528, 1)};
    GemVal _t530 = (*gem_v__mod_parser_make_int);
        GemVal _t531 = _t530.fn(_t530.env, _t529, 1);
        gem_pop_frame();
        return _t531;
    }
#line 182 "compiler/main.gem"
    GemVal _t532 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t532, gem_string("type")), gem_string("STRING")))) {
#line 183 "compiler/main.gem"
    GemVal _t533 = (*gem_v_advance);
        (void)(_t533.fn(_t533.env, NULL, 0));
#line 184 "compiler/main.gem"
    GemVal _t534 = gem_v_t;
    GemVal _t535[] = {gem_table_get(_t534, gem_string("value"))};
    GemVal _t536 = (*gem_v__mod_parser_make_string);
        GemVal _t537 = _t536.fn(_t536.env, _t535, 1);
        gem_pop_frame();
        return _t537;
    }
#line 188 "compiler/main.gem"
    GemVal _t538 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t538, gem_string("type")), gem_string("INTERP_START")))) {
#line 189 "compiler/main.gem"
    GemVal _t539 = (*gem_v_advance);
        (void)(_t539.fn(_t539.env, NULL, 0));
#line 190 "compiler/main.gem"
    GemVal _t540 = gem_table_new();
        GemVal gem_v_parts = _t540;
#line 191 "compiler/main.gem"
    GemVal _t541 = (*gem_v_skip_nl);
        (void)(_t541.fn(_t541.env, NULL, 0));
#line 192 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t542 = (*gem_v_peek);
            GemVal _t543 = _t542.fn(_t542.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t543, gem_string("type")), gem_string("INTERP_END")))) break;
#line 193 "compiler/main.gem"
    GemVal _t544 = (*gem_v_peek);
    GemVal _t545 = _t544.fn(_t544.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t545, gem_string("type")), gem_string("STRING")))) {
#line 194 "compiler/main.gem"
    GemVal _t546 = (*gem_v_advance);
    GemVal _t547 = _t546.fn(_t546.env, NULL, 0);
                GemVal gem_v_sval = gem_table_get(_t547, gem_string("value"));
#line 195 "compiler/main.gem"
    GemVal _t548[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t548, 1), gem_int(0)))) {
#line 196 "compiler/main.gem"
    GemVal _t549[] = {gem_v_sval};
    GemVal _t550 = (*gem_v__mod_parser_make_string);
    GemVal _t551[] = {gem_v_parts, _t550.fn(_t550.env, _t549, 1)};
                    (void)(gem_push_fn(NULL, _t551, 2));
                }
            } else {
#line 199 "compiler/main.gem"
    GemVal _t552 = (*gem_v_parse_expr);
    GemVal _t553[] = {gem_v_parts, _t552.fn(_t552.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t553, 2));
            }
#line 201 "compiler/main.gem"
    GemVal _t554 = (*gem_v_skip_nl);
            (void)(_t554.fn(_t554.env, NULL, 0));
        }
#line 203 "compiler/main.gem"
    GemVal _t555[] = {gem_string("INTERP_END")};
    GemVal _t556 = (*gem_v_expect);
        (void)(_t556.fn(_t556.env, _t555, 1));
#line 204 "compiler/main.gem"
    GemVal _t557[] = {gem_v_parts};
    GemVal _t558 = (*gem_v__mod_parser_make_interp);
        GemVal _t559 = _t558.fn(_t558.env, _t557, 1);
        gem_pop_frame();
        return _t559;
    }
#line 208 "compiler/main.gem"
    GemVal _t560 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t560, gem_string("type")), gem_string("true")))) {
#line 209 "compiler/main.gem"
    GemVal _t561 = (*gem_v_advance);
        (void)(_t561.fn(_t561.env, NULL, 0));
#line 210 "compiler/main.gem"
    GemVal _t562[] = {gem_bool(1)};
    GemVal _t563 = (*gem_v__mod_parser_make_bool);
        GemVal _t564 = _t563.fn(_t563.env, _t562, 1);
        gem_pop_frame();
        return _t564;
    }
#line 212 "compiler/main.gem"
    GemVal _t565 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t565, gem_string("type")), gem_string("false")))) {
#line 213 "compiler/main.gem"
    GemVal _t566 = (*gem_v_advance);
        (void)(_t566.fn(_t566.env, NULL, 0));
#line 214 "compiler/main.gem"
    GemVal _t567[] = {gem_bool(0)};
    GemVal _t568 = (*gem_v__mod_parser_make_bool);
        GemVal _t569 = _t568.fn(_t568.env, _t567, 1);
        gem_pop_frame();
        return _t569;
    }
#line 218 "compiler/main.gem"
    GemVal _t570 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t570, gem_string("type")), gem_string("nil")))) {
#line 219 "compiler/main.gem"
    GemVal _t571 = (*gem_v_advance);
        (void)(_t571.fn(_t571.env, NULL, 0));
#line 220 "compiler/main.gem"
    GemVal _t572 = (*gem_v__mod_parser_make_nil_node);
        GemVal _t573 = _t572.fn(_t572.env, NULL, 0);
        gem_pop_frame();
        return _t573;
    }
#line 224 "compiler/main.gem"
    GemVal _t574 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t574, gem_string("type")), gem_string("fn")))) {
#line 225 "compiler/main.gem"
    GemVal _t575 = (*gem_v_advance);
        (void)(_t575.fn(_t575.env, NULL, 0));
#line 226 "compiler/main.gem"
    GemVal _t576[] = {gem_string("(")};
    GemVal _t577 = (*gem_v_expect);
        (void)(_t577.fn(_t577.env, _t576, 1));
#line 227 "compiler/main.gem"
    GemVal _t578 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t578.fn(_t578.env, NULL, 0);
#line 228 "compiler/main.gem"
    GemVal _t579[] = {gem_string(")")};
    GemVal _t580 = (*gem_v_expect);
        (void)(_t580.fn(_t580.env, _t579, 1));
#line 229 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 230 "compiler/main.gem"
    GemVal _t581 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t581.fn(_t581.env, NULL, 0);
#line 231 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 232 "compiler/main.gem"
    GemVal _t582[] = {gem_string("end")};
    GemVal _t583 = (*gem_v_expect);
        (void)(_t583.fn(_t583.env, _t582, 1));
#line 233 "compiler/main.gem"
    GemVal _t584 = gem_v_pr;
    GemVal _t585 = gem_v_pr;
    GemVal _t586[] = {gem_table_get(_t584, gem_string("params")), gem_table_get(_t585, gem_string("rest_param")), gem_v_body};
    GemVal _t587 = (*gem_v__mod_parser_make_anon_fn);
        GemVal _t588 = _t587.fn(_t587.env, _t586, 3);
        gem_pop_frame();
        return _t588;
    }
#line 238 "compiler/main.gem"
    GemVal _t589 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t589, gem_string("type")), gem_string("{")))) {
#line 239 "compiler/main.gem"
    GemVal _t590 = (*gem_v_advance);
        (void)(_t590.fn(_t590.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t591 = (*gem_v_skip_nl);
        (void)(_t591.fn(_t591.env, NULL, 0));
#line 241 "compiler/main.gem"
    GemVal _t592 = gem_table_new();
        GemVal gem_v_entries = _t592;
#line 242 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t593 = (*gem_v_peek);
            GemVal _t594 = _t593.fn(_t593.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t594, gem_string("type")), gem_string("}")))) break;
#line 243 "compiler/main.gem"
    GemVal _t595 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t595.fn(_t595.env, NULL, 0);
#line 244 "compiler/main.gem"
    GemVal _t596 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t596, gem_string("type")), gem_string("NAME")))) {
#line 245 "compiler/main.gem"
    GemVal _t597 = (*gem_v_advance);
                (void)(_t597.fn(_t597.env, NULL, 0));
            } else {
#line 246 "compiler/main.gem"
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
#line 248 "compiler/main.gem"
    GemVal _t603 = (*gem_v_advance);
                    (void)(_t603.fn(_t603.env, NULL, 0));
                } else {
#line 250 "compiler/main.gem"
    GemVal _t604 = gem_v_key_tok;
    GemVal _t605 = gem_v_key_tok;
    GemVal _t606 = gem_v_key_tok;
    GemVal _t607[] = {gem_table_get(_t606, gem_string("value"))};
    GemVal _t608 = gem_v_key_tok;
    GemVal _t609[] = {gem_table_get(_t608, gem_string("type"))};
    GemVal _t610[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t604, gem_string("line")), gem_table_get(_t605, gem_string("col")), gem_len_fn(NULL, _t607, 1), gem_add(gem_add(gem_string("expected '}' or table key but got '"), gem_to_string_fn(NULL, _t609, 1)), gem_string("'")), GEM_NIL};
    GemVal _t611 = (*gem_v__mod_parser_compile_error);
                    (void)(_t611.fn(_t611.env, _t610, 7));
                }
            }
#line 252 "compiler/main.gem"
    GemVal _t612[] = {gem_string(":")};
    GemVal _t613 = (*gem_v_expect);
            (void)(_t613.fn(_t613.env, _t612, 1));
#line 253 "compiler/main.gem"
    GemVal _t614 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t614.fn(_t614.env, NULL, 0);
#line 254 "compiler/main.gem"
    GemVal _t615 = gem_v_key_tok;
    GemVal _t616[] = {gem_table_get(_t615, gem_string("value")), gem_v_val};
    GemVal _t617 = (*gem_v__mod_parser_make_table_entry);
    GemVal _t618[] = {gem_v_entries, _t617.fn(_t617.env, _t616, 2)};
            (void)(gem_push_fn(NULL, _t618, 2));
#line 255 "compiler/main.gem"
    GemVal _t619 = (*gem_v_skip_nl);
            (void)(_t619.fn(_t619.env, NULL, 0));
#line 256 "compiler/main.gem"
    GemVal _t620 = (*gem_v_peek);
    GemVal _t621 = _t620.fn(_t620.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t621, gem_string("type")), gem_string(",")))) {
#line 257 "compiler/main.gem"
    GemVal _t622 = (*gem_v_advance);
                (void)(_t622.fn(_t622.env, NULL, 0));
#line 258 "compiler/main.gem"
    GemVal _t623 = (*gem_v_skip_nl);
                (void)(_t623.fn(_t623.env, NULL, 0));
            }
        }
#line 261 "compiler/main.gem"
    GemVal _t624[] = {gem_string("}")};
    GemVal _t625 = (*gem_v_expect);
        (void)(_t625.fn(_t625.env, _t624, 1));
#line 262 "compiler/main.gem"
    GemVal _t626[] = {gem_v_entries};
    GemVal _t627 = (*gem_v__mod_parser_make_table);
        GemVal _t628 = _t627.fn(_t627.env, _t626, 1);
        gem_pop_frame();
        return _t628;
    }
#line 266 "compiler/main.gem"
    GemVal _t629 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t629, gem_string("type")), gem_string("[")))) {
#line 267 "compiler/main.gem"
    GemVal _t630 = (*gem_v_advance);
        (void)(_t630.fn(_t630.env, NULL, 0));
#line 268 "compiler/main.gem"
    GemVal _t631 = (*gem_v_skip_nl);
        (void)(_t631.fn(_t631.env, NULL, 0));
#line 269 "compiler/main.gem"
    GemVal _t632 = gem_table_new();
        GemVal gem_v_elements = _t632;
#line 270 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t633 = (*gem_v_peek);
            GemVal _t634 = _t633.fn(_t633.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t634, gem_string("type")), gem_string("]")))) break;
#line 271 "compiler/main.gem"
    GemVal _t635 = (*gem_v_parse_expr);
    GemVal _t636[] = {gem_v_elements, _t635.fn(_t635.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t636, 2));
#line 272 "compiler/main.gem"
    GemVal _t637 = (*gem_v_skip_nl);
            (void)(_t637.fn(_t637.env, NULL, 0));
#line 273 "compiler/main.gem"
    GemVal _t638 = (*gem_v_peek);
    GemVal _t639 = _t638.fn(_t638.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t639, gem_string("type")), gem_string(",")))) {
#line 274 "compiler/main.gem"
    GemVal _t640 = (*gem_v_advance);
                (void)(_t640.fn(_t640.env, NULL, 0));
#line 275 "compiler/main.gem"
    GemVal _t641 = (*gem_v_skip_nl);
                (void)(_t641.fn(_t641.env, NULL, 0));
            }
        }
#line 278 "compiler/main.gem"
    GemVal _t642[] = {gem_string("]")};
    GemVal _t643 = (*gem_v_expect);
        (void)(_t643.fn(_t643.env, _t642, 1));
#line 279 "compiler/main.gem"
    GemVal _t644[] = {gem_v_elements};
    GemVal _t645 = (*gem_v__mod_parser_make_array);
        GemVal _t646 = _t645.fn(_t645.env, _t644, 1);
        gem_pop_frame();
        return _t646;
    }
#line 283 "compiler/main.gem"
    GemVal _t647 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t647, gem_string("type")), gem_string("(")))) {
#line 284 "compiler/main.gem"
    GemVal _t648 = (*gem_v_advance);
        (void)(_t648.fn(_t648.env, NULL, 0));
#line 285 "compiler/main.gem"
    GemVal _t649 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t649.fn(_t649.env, NULL, 0);
#line 286 "compiler/main.gem"
    GemVal _t650[] = {gem_string(")")};
    GemVal _t651 = (*gem_v_expect);
        (void)(_t651.fn(_t651.env, _t650, 1));
#line 287 "compiler/main.gem"
        GemVal _t652 = gem_v_e;
        gem_pop_frame();
        return _t652;
    }
#line 291 "compiler/main.gem"
    GemVal _t653 = gem_v_t;
    GemVal _t655;
    if (!gem_truthy(gem_eq(gem_table_get(_t653, gem_string("type")), gem_string("NAME")))) {
        _t655 = gem_eq(gem_table_get(_t653, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t654 = gem_v_t;
        _t655 = gem_eq(gem_table_get(_t654, gem_string("value")), gem_string("pcall"));
    }
    GemVal _t659;
    if (!gem_truthy(_t655)) {
        _t659 = _t655;
    } else {
        GemVal _t656[] = {gem_int(1)};
        GemVal _t657 = (*gem_v_peek_at);
        GemVal _t658 = _t657.fn(_t657.env, _t656, 1);
        _t659 = gem_neq(gem_table_get(_t658, gem_string("type")), gem_string("("));
    }
    if (gem_truthy(_t659)) {
#line 292 "compiler/main.gem"
    GemVal _t660 = (*gem_v_advance);
        (void)(_t660.fn(_t660.env, NULL, 0));
#line 293 "compiler/main.gem"
    GemVal _t661 = (*gem_v_parse_expr);
        GemVal gem_v_expr = _t661.fn(_t661.env, NULL, 0);
#line 294 "compiler/main.gem"
    GemVal _t662[] = {gem_string("pcall")};
    GemVal _t663 = (*gem_v__mod_parser_make_var);
    GemVal _t664 = gem_table_new();
    GemVal _t665 = gem_table_new();
    GemVal _t666 = gem_table_new();
    gem_table_set(_t666, gem_int(0), gem_v_expr);
    GemVal _t667[] = {_t665, GEM_NIL, _t666};
    GemVal _t668 = (*gem_v__mod_parser_make_anon_fn);
    gem_table_set(_t664, gem_int(0), _t668.fn(_t668.env, _t667, 3));
    GemVal _t669 = gem_v_t;
    GemVal _t670[] = {_t663.fn(_t663.env, _t662, 1), _t664, gem_table_get(_t669, gem_string("line"))};
    GemVal _t671 = (*gem_v__mod_parser_make_call);
        GemVal _t672 = _t671.fn(_t671.env, _t670, 3);
        gem_pop_frame();
        return _t672;
    }
#line 298 "compiler/main.gem"
    GemVal _t673 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t673, gem_string("type")), gem_string("NAME")))) {
#line 299 "compiler/main.gem"
    GemVal _t674 = (*gem_v_advance);
        (void)(_t674.fn(_t674.env, NULL, 0));
#line 300 "compiler/main.gem"
    GemVal _t675 = gem_v_t;
    GemVal _t676[] = {gem_table_get(_t675, gem_string("value"))};
    GemVal _t677 = (*gem_v__mod_parser_make_var);
        GemVal _t678 = _t677.fn(_t677.env, _t676, 1);
        gem_pop_frame();
        return _t678;
    }
#line 303 "compiler/main.gem"
    GemVal _t679 = gem_v_t;
    GemVal _t680 = gem_v_t;
    GemVal _t681 = gem_v_t;
    GemVal _t682[] = {gem_table_get(_t681, gem_string("value"))};
    GemVal _t683 = gem_v_t;
    GemVal _t684[] = {gem_table_get(_t683, gem_string("type"))};
    GemVal _t685[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t679, gem_string("line")), gem_table_get(_t680, gem_string("col")), gem_len_fn(NULL, _t682, 1), gem_add(gem_add(gem_string("unexpected token '"), gem_to_string_fn(NULL, _t684, 1)), gem_string("' (expected expression)")), GEM_NIL};
    GemVal _t686 = (*gem_v__mod_parser_compile_error);
    GemVal _t687 = _t686.fn(_t686.env, _t685, 7);
    gem_pop_frame();
    return _t687;
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
    GemVal _t689 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t689.fn(_t689.env, NULL, 0);
#line 310 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 312 "compiler/main.gem"
    GemVal _t690 = (*gem_v_peek);
    GemVal _t691 = _t690.fn(_t690.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t691, gem_string("type")), gem_string("(")))) {
#line 313 "compiler/main.gem"
    GemVal _t692 = (*gem_v_peek);
    GemVal _t693 = _t692.fn(_t692.env, NULL, 0);
            GemVal gem_v_call_line = gem_table_get(_t693, gem_string("line"));
#line 314 "compiler/main.gem"
    GemVal _t694 = (*gem_v_advance);
            (void)(_t694.fn(_t694.env, NULL, 0));
#line 315 "compiler/main.gem"
    GemVal _t695 = (*gem_v_skip_nl);
            (void)(_t695.fn(_t695.env, NULL, 0));
#line 316 "compiler/main.gem"
    GemVal _t696 = gem_table_new();
            GemVal gem_v_args = _t696;
#line 317 "compiler/main.gem"
    GemVal _t697 = (*gem_v_peek);
    GemVal _t698 = _t697.fn(_t697.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t698, gem_string("type")), gem_string(")")))) {
#line 318 "compiler/main.gem"
    GemVal _t699 = (*gem_v_parse_expr);
    GemVal _t700[] = {gem_v_args, _t699.fn(_t699.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t700, 2));
#line 319 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t701 = (*gem_v_peek);
                    GemVal _t702 = _t701.fn(_t701.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t702, gem_string("type")), gem_string(",")))) break;
#line 320 "compiler/main.gem"
    GemVal _t703 = (*gem_v_advance);
                    (void)(_t703.fn(_t703.env, NULL, 0));
#line 321 "compiler/main.gem"
    GemVal _t704 = (*gem_v_skip_nl);
                    (void)(_t704.fn(_t704.env, NULL, 0));
#line 322 "compiler/main.gem"
    GemVal _t705 = (*gem_v_parse_expr);
    GemVal _t706[] = {gem_v_args, _t705.fn(_t705.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t706, 2));
                }
            }
#line 325 "compiler/main.gem"
    GemVal _t707 = (*gem_v_skip_nl);
            (void)(_t707.fn(_t707.env, NULL, 0));
#line 326 "compiler/main.gem"
    GemVal _t708[] = {gem_string(")")};
    GemVal _t709 = (*gem_v_expect);
            (void)(_t709.fn(_t709.env, _t708, 1));
#line 329 "compiler/main.gem"
    GemVal _t710 = (*gem_v_peek);
    GemVal _t711 = _t710.fn(_t710.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t711, gem_string("type")), gem_string("do")))) {
#line 330 "compiler/main.gem"
    GemVal _t712 = (*gem_v_advance);
                (void)(_t712.fn(_t712.env, NULL, 0));
#line 331 "compiler/main.gem"
    GemVal _t713 = gem_table_new();
                GemVal gem_v_bparams = _t713;
#line 332 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 333 "compiler/main.gem"
    GemVal _t714 = (*gem_v_peek);
    GemVal _t715 = _t714.fn(_t714.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t715, gem_string("type")), gem_string("|")))) {
#line 334 "compiler/main.gem"
    GemVal _t716 = (*gem_v_advance);
                    (void)(_t716.fn(_t716.env, NULL, 0));
#line 335 "compiler/main.gem"
    GemVal _t717 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t717.fn(_t717.env, NULL, 0);
#line 336 "compiler/main.gem"
    GemVal _t718 = gem_v_bpr;
                    gem_v_bparams = gem_table_get(_t718, gem_string("params"));
#line 337 "compiler/main.gem"
    GemVal _t719 = gem_v_bpr;
                    gem_v_brest = gem_table_get(_t719, gem_string("rest_param"));
#line 338 "compiler/main.gem"
    GemVal _t720[] = {gem_string("|")};
    GemVal _t721 = (*gem_v_expect);
                    (void)(_t721.fn(_t721.env, _t720, 1));
                }
#line 340 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 341 "compiler/main.gem"
    GemVal _t722 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t722.fn(_t722.env, NULL, 0);
#line 342 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 343 "compiler/main.gem"
    GemVal _t723[] = {gem_string("end")};
    GemVal _t724 = (*gem_v_expect);
                (void)(_t724.fn(_t724.env, _t723, 1));
#line 344 "compiler/main.gem"
    GemVal _t725[] = {gem_v_bparams, gem_v_brest, gem_v_body};
    GemVal _t726 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t727[] = {gem_v_args, _t726.fn(_t726.env, _t725, 3)};
                (void)(gem_push_fn(NULL, _t727, 2));
            } else {
#line 348 "compiler/main.gem"
    GemVal _t728 = (*gem_v_peek);
    GemVal _t729 = _t728.fn(_t728.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t729, gem_string("type")), gem_string("{")))) {
#line 349 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 350 "compiler/main.gem"
    GemVal _t730[] = {gem_int(1)};
    GemVal _t731 = (*gem_v_peek_at);
    GemVal _t732 = _t731.fn(_t731.env, _t730, 1);
                    if (gem_truthy(gem_eq(gem_table_get(_t732, gem_string("type")), gem_string("|")))) {
#line 351 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 352 "compiler/main.gem"
    GemVal _t733[] = {gem_int(1)};
    GemVal _t734 = (*gem_v_peek_at);
    GemVal _t735 = _t734.fn(_t734.env, _t733, 1);
    GemVal _t743;
    if (gem_truthy(gem_eq(gem_table_get(_t735, gem_string("type")), gem_string("}")))) {
        _t743 = gem_eq(gem_table_get(_t735, gem_string("type")), gem_string("}"));
    } else {
        GemVal _t736[] = {gem_int(1)};
        GemVal _t737 = (*gem_v_peek_at);
        GemVal _t738 = _t737.fn(_t737.env, _t736, 1);
        GemVal _t742;
        if (!gem_truthy(gem_eq(gem_table_get(_t738, gem_string("type")), gem_string("NAME")))) {
                _t742 = gem_eq(gem_table_get(_t738, gem_string("type")), gem_string("NAME"));
        } else {
                GemVal _t739[] = {gem_int(2)};
                GemVal _t740 = (*gem_v_peek_at);
                GemVal _t741 = _t740.fn(_t740.env, _t739, 1);
                _t742 = gem_eq(gem_table_get(_t741, gem_string("type")), gem_string(":"));
        }
        _t743 = _t742;
    }
                        if (gem_truthy(_t743)) {
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
    GemVal _t744 = (*gem_v_advance);
                        (void)(_t744.fn(_t744.env, NULL, 0));
#line 359 "compiler/main.gem"
    GemVal _t745 = gem_table_new();
                        GemVal gem_v_bparams = _t745;
#line 360 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 361 "compiler/main.gem"
    GemVal _t746 = (*gem_v_peek);
    GemVal _t747 = _t746.fn(_t746.env, NULL, 0);
                        if (gem_truthy(gem_eq(gem_table_get(_t747, gem_string("type")), gem_string("|")))) {
#line 362 "compiler/main.gem"
    GemVal _t748 = (*gem_v_advance);
                            (void)(_t748.fn(_t748.env, NULL, 0));
#line 363 "compiler/main.gem"
    GemVal _t749 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t749.fn(_t749.env, NULL, 0);
#line 364 "compiler/main.gem"
    GemVal _t750 = gem_v_bpr;
                            gem_v_bparams = gem_table_get(_t750, gem_string("params"));
#line 365 "compiler/main.gem"
    GemVal _t751 = gem_v_bpr;
                            gem_v_brest = gem_table_get(_t751, gem_string("rest_param"));
#line 366 "compiler/main.gem"
    GemVal _t752[] = {gem_string("|")};
    GemVal _t753 = (*gem_v_expect);
                            (void)(_t753.fn(_t753.env, _t752, 1));
                        }
#line 368 "compiler/main.gem"
    GemVal _t754 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t754.fn(_t754.env, NULL, 0);
#line 369 "compiler/main.gem"
    GemVal _t755[] = {gem_string("}")};
    GemVal _t756 = (*gem_v_expect);
                        (void)(_t756.fn(_t756.env, _t755, 1));
#line 370 "compiler/main.gem"
    GemVal _t757 = gem_table_new();
    gem_table_set(_t757, gem_int(0), gem_v_bexpr);
    GemVal _t758[] = {gem_v_bparams, gem_v_brest, _t757};
    GemVal _t759 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t760[] = {gem_v_args, _t759.fn(_t759.env, _t758, 3)};
                        (void)(gem_push_fn(NULL, _t760, 2));
                    }
                }
            }
#line 375 "compiler/main.gem"
    GemVal _t761[] = {gem_v_node, gem_v_args, gem_v_call_line};
    GemVal _t762 = (*gem_v__mod_parser_make_call);
            gem_v_node = _t762.fn(_t762.env, _t761, 3);
            continue;
        }
#line 380 "compiler/main.gem"
    GemVal _t763 = (*gem_v_peek);
    GemVal _t764 = _t763.fn(_t763.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t764, gem_string("type")), gem_string(".")))) {
#line 381 "compiler/main.gem"
    GemVal _t765 = (*gem_v_advance);
            (void)(_t765.fn(_t765.env, NULL, 0));
#line 382 "compiler/main.gem"
    GemVal _t766 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t766.fn(_t766.env, NULL, 0);
#line 383 "compiler/main.gem"
    GemVal _t767 = gem_v_field_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t767, gem_string("type")), gem_string("NAME")))) {
#line 384 "compiler/main.gem"
    GemVal _t768 = (*gem_v_advance);
                (void)(_t768.fn(_t768.env, NULL, 0));
            } else {
#line 387 "compiler/main.gem"
    GemVal _t769 = (*gem_v_advance);
                (void)(_t769.fn(_t769.env, NULL, 0));
            }
#line 389 "compiler/main.gem"
    GemVal _t770 = gem_v_field_tok;
    GemVal _t771[] = {gem_v_node, gem_table_get(_t770, gem_string("value"))};
    GemVal _t772 = (*gem_v__mod_parser_make_dot);
            gem_v_node = _t772.fn(_t772.env, _t771, 2);
            continue;
        }
#line 394 "compiler/main.gem"
    GemVal _t773 = (*gem_v_peek);
    GemVal _t774 = _t773.fn(_t773.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t774, gem_string("type")), gem_string("[")))) {
#line 395 "compiler/main.gem"
    GemVal _t775 = (*gem_v_advance);
            (void)(_t775.fn(_t775.env, NULL, 0));
#line 396 "compiler/main.gem"
    GemVal _t776 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t776.fn(_t776.env, NULL, 0);
#line 397 "compiler/main.gem"
    GemVal _t777[] = {gem_string("]")};
    GemVal _t778 = (*gem_v_expect);
            (void)(_t778.fn(_t778.env, _t777, 1));
#line 398 "compiler/main.gem"
    GemVal _t779[] = {gem_v_node, gem_v_key};
    GemVal _t780 = (*gem_v__mod_parser_make_index);
            gem_v_node = _t780.fn(_t780.env, _t779, 2);
            continue;
        }
        break;
    }
    GemVal _t781 = gem_v_node;
    gem_pop_frame();
    return _t781;
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
    GemVal _t783 = (*gem_v_peek);
    GemVal _t784 = _t783.fn(_t783.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t784, gem_string("type")), gem_string("-")))) {
#line 411 "compiler/main.gem"
    GemVal _t785 = (*gem_v_advance);
        (void)(_t785.fn(_t785.env, NULL, 0));
#line 412 "compiler/main.gem"
    GemVal _t786 = (*gem_v_parse_unary);
    GemVal _t787[] = {gem_string("-"), _t786.fn(_t786.env, NULL, 0)};
    GemVal _t788 = (*gem_v__mod_parser_make_unop);
        GemVal _t789 = _t788.fn(_t788.env, _t787, 2);
        gem_pop_frame();
        return _t789;
    }
#line 414 "compiler/main.gem"
    GemVal _t790 = (*gem_v_parse_call);
    GemVal _t791 = _t790.fn(_t790.env, NULL, 0);
    gem_pop_frame();
    return _t791;
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
    GemVal _t793 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t793.fn(_t793.env, NULL, 0);
#line 420 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t794 = (*gem_v_peek);
        GemVal _t795 = _t794.fn(_t794.env, NULL, 0);
        GemVal _t798;
        if (gem_truthy(gem_eq(gem_table_get(_t795, gem_string("type")), gem_string("*")))) {
                _t798 = gem_eq(gem_table_get(_t795, gem_string("type")), gem_string("*"));
        } else {
                GemVal _t796 = (*gem_v_peek);
                GemVal _t797 = _t796.fn(_t796.env, NULL, 0);
                _t798 = gem_eq(gem_table_get(_t797, gem_string("type")), gem_string("/"));
        }
        GemVal _t801;
        if (gem_truthy(_t798)) {
                _t801 = _t798;
        } else {
                GemVal _t799 = (*gem_v_peek);
                GemVal _t800 = _t799.fn(_t799.env, NULL, 0);
                _t801 = gem_eq(gem_table_get(_t800, gem_string("type")), gem_string("%"));
        }
        if (!gem_truthy(_t801)) break;
#line 421 "compiler/main.gem"
    GemVal _t802 = (*gem_v_advance);
    GemVal _t803 = _t802.fn(_t802.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t803, gem_string("type"));
#line 422 "compiler/main.gem"
    GemVal _t804 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t804.fn(_t804.env, NULL, 0);
#line 423 "compiler/main.gem"
    GemVal _t805[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t806 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t806.fn(_t806.env, _t805, 3);
    }
    GemVal _t807 = gem_v_left;
    gem_pop_frame();
    return _t807;
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
    GemVal _t809 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t809.fn(_t809.env, NULL, 0);
#line 431 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t810 = (*gem_v_peek);
        GemVal _t811 = _t810.fn(_t810.env, NULL, 0);
        GemVal _t814;
        if (gem_truthy(gem_eq(gem_table_get(_t811, gem_string("type")), gem_string("+")))) {
                _t814 = gem_eq(gem_table_get(_t811, gem_string("type")), gem_string("+"));
        } else {
                GemVal _t812 = (*gem_v_peek);
                GemVal _t813 = _t812.fn(_t812.env, NULL, 0);
                _t814 = gem_eq(gem_table_get(_t813, gem_string("type")), gem_string("-"));
        }
        if (!gem_truthy(_t814)) break;
#line 432 "compiler/main.gem"
    GemVal _t815 = (*gem_v_advance);
    GemVal _t816 = _t815.fn(_t815.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t816, gem_string("type"));
#line 433 "compiler/main.gem"
    GemVal _t817 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t817.fn(_t817.env, NULL, 0);
#line 434 "compiler/main.gem"
    GemVal _t818[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t819 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t819.fn(_t819.env, _t818, 3);
    }
    GemVal _t820 = gem_v_left;
    gem_pop_frame();
    return _t820;
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
    GemVal _t822 = (*gem_v_parse_add);
    GemVal gem_v_left = _t822.fn(_t822.env, NULL, 0);
#line 442 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t823 = (*gem_v_peek);
        GemVal _t824 = _t823.fn(_t823.env, NULL, 0);
        GemVal _t827;
        if (gem_truthy(gem_eq(gem_table_get(_t824, gem_string("type")), gem_string("==")))) {
                _t827 = gem_eq(gem_table_get(_t824, gem_string("type")), gem_string("=="));
        } else {
                GemVal _t825 = (*gem_v_peek);
                GemVal _t826 = _t825.fn(_t825.env, NULL, 0);
                _t827 = gem_eq(gem_table_get(_t826, gem_string("type")), gem_string("!="));
        }
        GemVal _t830;
        if (gem_truthy(_t827)) {
                _t830 = _t827;
        } else {
                GemVal _t828 = (*gem_v_peek);
                GemVal _t829 = _t828.fn(_t828.env, NULL, 0);
                _t830 = gem_eq(gem_table_get(_t829, gem_string("type")), gem_string("<"));
        }
        GemVal _t833;
        if (gem_truthy(_t830)) {
                _t833 = _t830;
        } else {
                GemVal _t831 = (*gem_v_peek);
                GemVal _t832 = _t831.fn(_t831.env, NULL, 0);
                _t833 = gem_eq(gem_table_get(_t832, gem_string("type")), gem_string(">"));
        }
        GemVal _t836;
        if (gem_truthy(_t833)) {
                _t836 = _t833;
        } else {
                GemVal _t834 = (*gem_v_peek);
                GemVal _t835 = _t834.fn(_t834.env, NULL, 0);
                _t836 = gem_eq(gem_table_get(_t835, gem_string("type")), gem_string("<="));
        }
        GemVal _t839;
        if (gem_truthy(_t836)) {
                _t839 = _t836;
        } else {
                GemVal _t837 = (*gem_v_peek);
                GemVal _t838 = _t837.fn(_t837.env, NULL, 0);
                _t839 = gem_eq(gem_table_get(_t838, gem_string("type")), gem_string(">="));
        }
        GemVal _t842;
        if (gem_truthy(_t839)) {
                _t842 = _t839;
        } else {
                GemVal _t840 = (*gem_v_peek);
                GemVal _t841 = _t840.fn(_t840.env, NULL, 0);
                _t842 = gem_eq(gem_table_get(_t841, gem_string("type")), gem_string("in"));
        }
        if (!gem_truthy(_t842)) break;
#line 443 "compiler/main.gem"
    GemVal _t843 = (*gem_v_advance);
    GemVal _t844 = _t843.fn(_t843.env, NULL, 0);
        GemVal gem_v_op = gem_table_get(_t844, gem_string("type"));
#line 444 "compiler/main.gem"
    GemVal _t845 = (*gem_v_parse_add);
        GemVal gem_v_right = _t845.fn(_t845.env, NULL, 0);
#line 445 "compiler/main.gem"
    GemVal _t846[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t847 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t847.fn(_t847.env, _t846, 3);
    }
    GemVal _t848 = gem_v_left;
    gem_pop_frame();
    return _t848;
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
    GemVal _t850 = (*gem_v_peek);
    GemVal _t851 = _t850.fn(_t850.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t851, gem_string("type")), gem_string("not")))) {
#line 453 "compiler/main.gem"
    GemVal _t852 = (*gem_v_advance);
        (void)(_t852.fn(_t852.env, NULL, 0));
#line 454 "compiler/main.gem"
    GemVal _t853 = (*gem_v_parse_not);
    GemVal _t854[] = {gem_string("not"), _t853.fn(_t853.env, NULL, 0)};
    GemVal _t855 = (*gem_v__mod_parser_make_unop);
        GemVal _t856 = _t855.fn(_t855.env, _t854, 2);
        gem_pop_frame();
        return _t856;
    }
#line 456 "compiler/main.gem"
    GemVal _t857 = (*gem_v_parse_compare);
    GemVal _t858 = _t857.fn(_t857.env, NULL, 0);
    gem_pop_frame();
    return _t858;
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
    GemVal _t860 = (*gem_v_parse_not);
    GemVal gem_v_left = _t860.fn(_t860.env, NULL, 0);
#line 462 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t861 = (*gem_v_peek);
        GemVal _t862 = _t861.fn(_t861.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t862, gem_string("type")), gem_string("and")))) break;
#line 463 "compiler/main.gem"
    GemVal _t863 = (*gem_v_advance);
        (void)(_t863.fn(_t863.env, NULL, 0));
#line 464 "compiler/main.gem"
    GemVal _t864 = (*gem_v_parse_not);
        GemVal gem_v_right = _t864.fn(_t864.env, NULL, 0);
#line 465 "compiler/main.gem"
    GemVal _t865[] = {gem_string("and"), gem_v_left, gem_v_right};
    GemVal _t866 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t866.fn(_t866.env, _t865, 3);
    }
    GemVal _t867 = gem_v_left;
    gem_pop_frame();
    return _t867;
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
    GemVal _t869 = (*gem_v_parse_and);
    GemVal gem_v_left = _t869.fn(_t869.env, NULL, 0);
#line 473 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t870 = (*gem_v_peek);
        GemVal _t871 = _t870.fn(_t870.env, NULL, 0);
        if (!gem_truthy(gem_eq(gem_table_get(_t871, gem_string("type")), gem_string("or")))) break;
#line 474 "compiler/main.gem"
    GemVal _t872 = (*gem_v_advance);
        (void)(_t872.fn(_t872.env, NULL, 0));
#line 475 "compiler/main.gem"
    GemVal _t873 = (*gem_v_parse_and);
        GemVal gem_v_right = _t873.fn(_t873.env, NULL, 0);
#line 476 "compiler/main.gem"
    GemVal _t874[] = {gem_string("or"), gem_v_left, gem_v_right};
    GemVal _t875 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t875.fn(_t875.env, _t874, 3);
    }
    GemVal _t876 = gem_v_left;
    gem_pop_frame();
    return _t876;
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
    GemVal _t878 = (*gem_v_peek);
    GemVal _t879 = _t878.fn(_t878.env, NULL, 0);
    GemVal gem_v_start_line = gem_table_get(_t879, gem_string("line"));
#line 484 "compiler/main.gem"
    GemVal _t880 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t880.fn(_t880.env, NULL, 0);
#line 487 "compiler/main.gem"
    GemVal _t881 = (*gem_v_peek);
    GemVal _t882 = _t881.fn(_t881.env, NULL, 0);
    if (gem_truthy(gem_eq(gem_table_get(_t882, gem_string("type")), gem_string("=")))) {
#line 488 "compiler/main.gem"
    GemVal _t883 = (*gem_v_advance);
        (void)(_t883.fn(_t883.env, NULL, 0));
#line 489 "compiler/main.gem"
    GemVal _t884 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t884.fn(_t884.env, NULL, 0);
#line 490 "compiler/main.gem"
    GemVal _t885 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t885, gem_string("tag")), gem_string("var")))) {
#line 491 "compiler/main.gem"
    GemVal _t886 = gem_v_lhs;
    GemVal _t887[] = {gem_table_get(_t886, gem_string("name")), gem_v_value, gem_v_start_line};
    GemVal _t888 = (*gem_v__mod_parser_make_assign);
            GemVal _t889 = _t888.fn(_t888.env, _t887, 3);
            gem_pop_frame();
            return _t889;
        }
#line 493 "compiler/main.gem"
    GemVal _t890 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t890, gem_string("tag")), gem_string("dot")))) {
#line 494 "compiler/main.gem"
    GemVal _t891 = gem_v_lhs;
    GemVal _t892 = gem_v_lhs;
    GemVal _t893[] = {gem_table_get(_t891, gem_string("object")), gem_table_get(_t892, gem_string("field")), gem_v_value, gem_v_start_line};
    GemVal _t894 = (*gem_v__mod_parser_make_dot_assign);
            GemVal _t895 = _t894.fn(_t894.env, _t893, 4);
            gem_pop_frame();
            return _t895;
        }
#line 496 "compiler/main.gem"
    GemVal _t896 = gem_v_lhs;
        if (gem_truthy(gem_eq(gem_table_get(_t896, gem_string("tag")), gem_string("index")))) {
#line 497 "compiler/main.gem"
    GemVal _t897 = gem_v_lhs;
    GemVal _t898 = gem_v_lhs;
    GemVal _t899[] = {gem_table_get(_t897, gem_string("object")), gem_table_get(_t898, gem_string("key")), gem_v_value, gem_v_start_line};
    GemVal _t900 = (*gem_v__mod_parser_make_index_assign);
            GemVal _t901 = _t900.fn(_t900.env, _t899, 4);
            gem_pop_frame();
            return _t901;
        }
#line 499 "compiler/main.gem"
    GemVal _t902[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
    GemVal _t903 = (*gem_v__mod_parser_compile_error);
        (void)(_t903.fn(_t903.env, _t902, 7));
    }
#line 503 "compiler/main.gem"
    GemVal _t904 = (*gem_v_peek);
    GemVal _t905 = _t904.fn(_t904.env, NULL, 0);
    GemVal _t908;
    if (gem_truthy(gem_eq(gem_table_get(_t905, gem_string("type")), gem_string("+=")))) {
        _t908 = gem_eq(gem_table_get(_t905, gem_string("type")), gem_string("+="));
    } else {
        GemVal _t906 = (*gem_v_peek);
        GemVal _t907 = _t906.fn(_t906.env, NULL, 0);
        _t908 = gem_eq(gem_table_get(_t907, gem_string("type")), gem_string("-="));
    }
    GemVal _t911;
    if (gem_truthy(_t908)) {
        _t911 = _t908;
    } else {
        GemVal _t909 = (*gem_v_peek);
        GemVal _t910 = _t909.fn(_t909.env, NULL, 0);
        _t911 = gem_eq(gem_table_get(_t910, gem_string("type")), gem_string("*="));
    }
    GemVal _t914;
    if (gem_truthy(_t911)) {
        _t914 = _t911;
    } else {
        GemVal _t912 = (*gem_v_peek);
        GemVal _t913 = _t912.fn(_t912.env, NULL, 0);
        _t914 = gem_eq(gem_table_get(_t913, gem_string("type")), gem_string("/="));
    }
    if (gem_truthy(_t914)) {
#line 504 "compiler/main.gem"
    GemVal _t915 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t915.fn(_t915.env, NULL, 0);
#line 505 "compiler/main.gem"
    GemVal _t916 = gem_v_op_tok;
        GemVal gem_v_base_op = gem_table_get(gem_table_get(_t916, gem_string("value")), gem_int(0));
#line 506 "compiler/main.gem"
    GemVal _t917 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t917.fn(_t917.env, NULL, 0);
#line 507 "compiler/main.gem"
    GemVal _t918 = gem_v_lhs;
        if (gem_truthy(gem_neq(gem_table_get(_t918, gem_string("tag")), gem_string("var")))) {
#line 508 "compiler/main.gem"
    GemVal _t919 = gem_v_op_tok;
    GemVal _t920 = gem_v_op_tok;
    GemVal _t921 = gem_v_op_tok;
    GemVal _t922[] = {gem_table_get(_t921, gem_string("value"))};
    GemVal _t923[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t919, gem_string("line")), gem_table_get(_t920, gem_string("col")), gem_len_fn(NULL, _t922, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
    GemVal _t924 = (*gem_v__mod_parser_compile_error);
            (void)(_t924.fn(_t924.env, _t923, 7));
        }
#line 510 "compiler/main.gem"
    GemVal _t925 = gem_v_lhs;
    GemVal _t926 = gem_v_lhs;
    GemVal _t927[] = {gem_table_get(_t926, gem_string("name"))};
    GemVal _t928 = (*gem_v__mod_parser_make_var);
    GemVal _t929[] = {gem_v_base_op, _t928.fn(_t928.env, _t927, 1), gem_v_value};
    GemVal _t930 = (*gem_v__mod_parser_make_binop);
    GemVal _t931[] = {gem_table_get(_t925, gem_string("name")), _t930.fn(_t930.env, _t929, 3), gem_v_start_line};
    GemVal _t932 = (*gem_v__mod_parser_make_assign);
        GemVal _t933 = _t932.fn(_t932.env, _t931, 3);
        gem_pop_frame();
        return _t933;
    }
    GemVal _t934 = gem_v_lhs;
    gem_pop_frame();
    return _t934;
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
    GemVal _t936[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t936, 1), gem_int(0)))) {
#line 524 "compiler/main.gem"
    GemVal _t937[] = {gem_bool(1)};
    GemVal _t938 = (*gem_v__mod_parser_make_bool);
        GemVal _t939 = _t938.fn(_t938.env, _t937, 1);
        gem_pop_frame();
        return _t939;
    }
#line 526 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 527 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(1);
#line 527 "compiler/main.gem"
    GemVal _t940[] = {gem_v_conditions};
    GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t940, 1);
#line 527 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 527 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_4;
#line 527 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 528 "compiler/main.gem"
    GemVal _t941[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
    GemVal _t942 = (*gem_v__mod_parser_make_binop);
        gem_v_result = _t942.fn(_t942.env, _t941, 3);
    }

    GemVal _t943 = gem_v_result;
    gem_pop_frame();
    return _t943;
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
    struct _closure__anon_21 *_t944 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t944->gem_v__mod_parser_make_binop = gem_v__mod_parser_make_binop;
    _t944->gem_v__mod_parser_make_bool = gem_v__mod_parser_make_bool;
    GemVal gem_v_and_chain = gem_make_fn(_anon_21, _t944);
#line 532 "compiler/main.gem"
    GemVal _t945 = (*gem_v_peek);
    GemVal gem_v_t = _t945.fn(_t945.env, NULL, 0);
#line 535 "compiler/main.gem"
    GemVal _t946 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t946, gem_string("type")), gem_string("{")))) {
#line 536 "compiler/main.gem"
    GemVal _t947 = (*gem_v_advance);
        (void)(_t947.fn(_t947.env, NULL, 0));
#line 537 "compiler/main.gem"
    GemVal _t948 = (*gem_v_skip_nl);
        (void)(_t948.fn(_t948.env, NULL, 0));
#line 538 "compiler/main.gem"
    GemVal _t949 = gem_table_new();
    GemVal _t950[] = {gem_string("type")};
    GemVal _t951 = (*gem_v__mod_parser_make_var);
    GemVal _t952 = gem_table_new();
    gem_table_set(_t952, gem_int(0), gem_v_target_expr);
    GemVal _t953[] = {_t951.fn(_t951.env, _t950, 1), _t952, gem_int(0)};
    GemVal _t954 = (*gem_v__mod_parser_make_call);
    GemVal _t955[] = {gem_string("table")};
    GemVal _t956 = (*gem_v__mod_parser_make_string);
    GemVal _t957[] = {gem_string("=="), _t954.fn(_t954.env, _t953, 3), _t956.fn(_t956.env, _t955, 1)};
    GemVal _t958 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t949, gem_int(0), _t958.fn(_t958.env, _t957, 3));
        GemVal gem_v_conditions = _t949;
#line 539 "compiler/main.gem"
    GemVal _t959 = gem_table_new();
        GemVal gem_v_bindings = _t959;
#line 540 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t960 = (*gem_v_peek);
            GemVal _t961 = _t960.fn(_t960.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t961, gem_string("type")), gem_string("}")))) break;
#line 541 "compiler/main.gem"
    GemVal _t962 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t962.fn(_t962.env, NULL, 0);
#line 542 "compiler/main.gem"
    GemVal _t963 = gem_v_key_tok;
            if (gem_truthy(gem_eq(gem_table_get(_t963, gem_string("type")), gem_string("NAME")))) {
#line 543 "compiler/main.gem"
    GemVal _t964 = (*gem_v_advance);
                (void)(_t964.fn(_t964.env, NULL, 0));
            } else {
#line 544 "compiler/main.gem"
    GemVal _t965[] = {gem_int(1)};
    GemVal _t966 = (*gem_v_peek_at);
    GemVal _t967 = _t966.fn(_t966.env, _t965, 1);
                if (gem_truthy(gem_eq(gem_table_get(_t967, gem_string("type")), gem_string(":")))) {
#line 546 "compiler/main.gem"
    GemVal _t968 = (*gem_v_advance);
                    (void)(_t968.fn(_t968.env, NULL, 0));
                } else {
#line 548 "compiler/main.gem"
    GemVal _t969 = gem_v_key_tok;
    GemVal _t970 = gem_v_key_tok;
    GemVal _t971 = gem_v_key_tok;
    GemVal _t972[] = {gem_table_get(_t971, gem_string("value"))};
    GemVal _t973[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t969, gem_string("line")), gem_table_get(_t970, gem_string("col")), gem_len_fn(NULL, _t972, 1), gem_string("expected key name in table pattern"), GEM_NIL};
    GemVal _t974 = (*gem_v__mod_parser_compile_error);
                    (void)(_t974.fn(_t974.env, _t973, 7));
                }
            }
#line 550 "compiler/main.gem"
    GemVal _t975 = gem_v_key_tok;
            GemVal gem_v_key_name = gem_table_get(_t975, gem_string("value"));
#line 551 "compiler/main.gem"
    GemVal _t976[] = {gem_v_key_name};
    GemVal _t977 = (*gem_v__mod_parser_make_string);
            GemVal gem_v_key_str = _t977.fn(_t977.env, _t976, 1);
#line 552 "compiler/main.gem"
    GemVal _t978[] = {gem_string("has_key")};
    GemVal _t979 = (*gem_v__mod_parser_make_var);
    GemVal _t980 = gem_table_new();
    gem_table_set(_t980, gem_int(0), gem_v_target_expr);
    gem_table_set(_t980, gem_int(1), gem_v_key_str);
    GemVal _t981[] = {_t979.fn(_t979.env, _t978, 1), _t980, gem_int(0)};
    GemVal _t982 = (*gem_v__mod_parser_make_call);
    GemVal _t983[] = {gem_v_conditions, _t982.fn(_t982.env, _t981, 3)};
            (void)(gem_push_fn(NULL, _t983, 2));
#line 553 "compiler/main.gem"
    GemVal _t984[] = {gem_v_target_expr, gem_v_key_str};
    GemVal _t985 = (*gem_v__mod_parser_make_index);
            GemVal gem_v_sub_target = _t985.fn(_t985.env, _t984, 2);
#line 554 "compiler/main.gem"
    GemVal _t986 = (*gem_v_peek);
    GemVal _t987 = _t986.fn(_t986.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t987, gem_string("type")), gem_string(":")))) {
#line 555 "compiler/main.gem"
    GemVal _t988 = (*gem_v_advance);
                (void)(_t988.fn(_t988.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t989 = (*gem_v_skip_nl);
                (void)(_t989.fn(_t989.env, NULL, 0));
#line 557 "compiler/main.gem"
    GemVal _t990[] = {gem_v_sub_target};
    GemVal _t991 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t991.fn(_t991.env, _t990, 1);
#line 558 "compiler/main.gem"
    GemVal _t992 = gem_v_sub;
    GemVal _t993 = gem_table_get(_t992, gem_string("condition"));
    GemVal _t996;
    if (gem_truthy(gem_neq(gem_table_get(_t993, gem_string("tag")), gem_string("bool")))) {
        _t996 = gem_neq(gem_table_get(_t993, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t994 = gem_v_sub;
        GemVal _t995 = gem_table_get(_t994, gem_string("condition"));
        _t996 = gem_neq(gem_table_get(_t995, gem_string("value")), gem_bool(1));
    }
                if (gem_truthy(_t996)) {
#line 559 "compiler/main.gem"
    GemVal _t997 = gem_v_sub;
    GemVal _t998[] = {gem_v_conditions, gem_table_get(_t997, gem_string("condition"))};
                    (void)(gem_push_fn(NULL, _t998, 2));
                }
#line 561 "compiler/main.gem"
    GemVal _t999 = gem_v_sub;
                GemVal gem_v__for_items_5 = gem_table_get(_t999, gem_string("bindings"));
#line 561 "compiler/main.gem"
                GemVal gem_v__for_i_5 = gem_int(0);
#line 561 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1000[] = {gem_v__for_items_5};
                    if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1000, 1)))) break;
#line 561 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 561 "compiler/main.gem"
                    gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 562 "compiler/main.gem"
    GemVal _t1001[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t1001, 2));
                }

            } else {
#line 566 "compiler/main.gem"
    GemVal _t1002[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t1003 = (*gem_v__mod_parser_make_let);
    GemVal _t1004[] = {gem_v_bindings, _t1003.fn(_t1003.env, _t1002, 3)};
                (void)(gem_push_fn(NULL, _t1004, 2));
            }
#line 568 "compiler/main.gem"
    GemVal _t1005 = (*gem_v_skip_nl);
            (void)(_t1005.fn(_t1005.env, NULL, 0));
#line 569 "compiler/main.gem"
    GemVal _t1006 = (*gem_v_peek);
    GemVal _t1007 = _t1006.fn(_t1006.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1007, gem_string("type")), gem_string(",")))) {
#line 570 "compiler/main.gem"
    GemVal _t1008 = (*gem_v_advance);
                (void)(_t1008.fn(_t1008.env, NULL, 0));
#line 571 "compiler/main.gem"
    GemVal _t1009 = (*gem_v_skip_nl);
                (void)(_t1009.fn(_t1009.env, NULL, 0));
            }
        }
#line 574 "compiler/main.gem"
    GemVal _t1010[] = {gem_string("}")};
    GemVal _t1011 = (*gem_v_expect);
        (void)(_t1011.fn(_t1011.env, _t1010, 1));
#line 575 "compiler/main.gem"
    GemVal _t1012 = gem_table_new();
    GemVal _t1013[] = {gem_v_conditions};
    GemVal _t1014 = gem_v_and_chain;
    gem_table_set(_t1012, gem_string("condition"), _t1014.fn(_t1014.env, _t1013, 1));
    gem_table_set(_t1012, gem_string("bindings"), gem_v_bindings);
        GemVal _t1015 = _t1012;
        gem_pop_frame();
        return _t1015;
    }
#line 579 "compiler/main.gem"
    GemVal _t1016 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1016, gem_string("type")), gem_string("[")))) {
#line 580 "compiler/main.gem"
    GemVal _t1017 = (*gem_v_advance);
        (void)(_t1017.fn(_t1017.env, NULL, 0));
#line 581 "compiler/main.gem"
    GemVal _t1018 = (*gem_v_skip_nl);
        (void)(_t1018.fn(_t1018.env, NULL, 0));
#line 582 "compiler/main.gem"
    GemVal _t1019 = gem_table_new();
        GemVal gem_v_sub_patterns = _t1019;
#line 583 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 584 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1020 = (*gem_v_peek);
            GemVal _t1021 = _t1020.fn(_t1020.env, NULL, 0);
            if (!gem_truthy(gem_neq(gem_table_get(_t1021, gem_string("type")), gem_string("]")))) break;
#line 585 "compiler/main.gem"
    GemVal _t1022[] = {gem_v_idx};
    GemVal _t1023 = (*gem_v__mod_parser_make_int);
    GemVal _t1024[] = {gem_v_target_expr, _t1023.fn(_t1023.env, _t1022, 1)};
    GemVal _t1025 = (*gem_v__mod_parser_make_index);
    GemVal _t1026[] = {_t1025.fn(_t1025.env, _t1024, 2)};
    GemVal _t1027 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t1027.fn(_t1027.env, _t1026, 1);
#line 586 "compiler/main.gem"
    GemVal _t1028[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t1028, 2));
#line 587 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 588 "compiler/main.gem"
    GemVal _t1029 = (*gem_v_skip_nl);
            (void)(_t1029.fn(_t1029.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t1030 = (*gem_v_peek);
    GemVal _t1031 = _t1030.fn(_t1030.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1031, gem_string("type")), gem_string(",")))) {
#line 590 "compiler/main.gem"
    GemVal _t1032 = (*gem_v_advance);
                (void)(_t1032.fn(_t1032.env, NULL, 0));
#line 591 "compiler/main.gem"
    GemVal _t1033 = (*gem_v_skip_nl);
                (void)(_t1033.fn(_t1033.env, NULL, 0));
            }
        }
#line 594 "compiler/main.gem"
    GemVal _t1034[] = {gem_string("]")};
    GemVal _t1035 = (*gem_v_expect);
        (void)(_t1035.fn(_t1035.env, _t1034, 1));
#line 595 "compiler/main.gem"
    GemVal _t1036 = gem_table_new();
    GemVal _t1037[] = {gem_string("type")};
    GemVal _t1038 = (*gem_v__mod_parser_make_var);
    GemVal _t1039 = gem_table_new();
    gem_table_set(_t1039, gem_int(0), gem_v_target_expr);
    GemVal _t1040[] = {_t1038.fn(_t1038.env, _t1037, 1), _t1039, gem_int(0)};
    GemVal _t1041 = (*gem_v__mod_parser_make_call);
    GemVal _t1042[] = {gem_string("table")};
    GemVal _t1043 = (*gem_v__mod_parser_make_string);
    GemVal _t1044[] = {gem_string("=="), _t1041.fn(_t1041.env, _t1040, 3), _t1043.fn(_t1043.env, _t1042, 1)};
    GemVal _t1045 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1036, gem_int(0), _t1045.fn(_t1045.env, _t1044, 3));
        GemVal gem_v_conditions = _t1036;
#line 596 "compiler/main.gem"
    GemVal _t1046[] = {gem_string("len")};
    GemVal _t1047 = (*gem_v__mod_parser_make_var);
    GemVal _t1048 = gem_table_new();
    gem_table_set(_t1048, gem_int(0), gem_v_target_expr);
    GemVal _t1049[] = {_t1047.fn(_t1047.env, _t1046, 1), _t1048, gem_int(0)};
    GemVal _t1050 = (*gem_v__mod_parser_make_call);
    GemVal _t1051[] = {gem_v_idx};
    GemVal _t1052 = (*gem_v__mod_parser_make_int);
    GemVal _t1053[] = {gem_string("=="), _t1050.fn(_t1050.env, _t1049, 3), _t1052.fn(_t1052.env, _t1051, 1)};
    GemVal _t1054 = (*gem_v__mod_parser_make_binop);
    GemVal _t1055[] = {gem_v_conditions, _t1054.fn(_t1054.env, _t1053, 3)};
        (void)(gem_push_fn(NULL, _t1055, 2));
#line 597 "compiler/main.gem"
    GemVal _t1056 = gem_table_new();
        GemVal gem_v_bindings = _t1056;
#line 598 "compiler/main.gem"
        GemVal gem_v__for_i_7 = gem_int(0);
#line 598 "compiler/main.gem"
    GemVal _t1057[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1057, 1);
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
    GemVal _t1058 = gem_v_sub;
    GemVal _t1059 = gem_table_get(_t1058, gem_string("condition"));
    GemVal _t1062;
    if (gem_truthy(gem_neq(gem_table_get(_t1059, gem_string("tag")), gem_string("bool")))) {
        _t1062 = gem_neq(gem_table_get(_t1059, gem_string("tag")), gem_string("bool"));
    } else {
        GemVal _t1060 = gem_v_sub;
        GemVal _t1061 = gem_table_get(_t1060, gem_string("condition"));
        _t1062 = gem_neq(gem_table_get(_t1061, gem_string("value")), gem_bool(1));
    }
            if (gem_truthy(_t1062)) {
#line 601 "compiler/main.gem"
    GemVal _t1063 = gem_v_sub;
    GemVal _t1064[] = {gem_v_conditions, gem_table_get(_t1063, gem_string("condition"))};
                (void)(gem_push_fn(NULL, _t1064, 2));
            }
#line 603 "compiler/main.gem"
    GemVal _t1065 = gem_v_sub;
            GemVal gem_v__for_items_6 = gem_table_get(_t1065, gem_string("bindings"));
#line 603 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 603 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1066[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1066, 1)))) break;
#line 603 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 603 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 604 "compiler/main.gem"
    GemVal _t1067[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t1067, 2));
            }

        }

#line 607 "compiler/main.gem"
    GemVal _t1068 = gem_table_new();
    GemVal _t1069[] = {gem_v_conditions};
    GemVal _t1070 = gem_v_and_chain;
    gem_table_set(_t1068, gem_string("condition"), _t1070.fn(_t1070.env, _t1069, 1));
    gem_table_set(_t1068, gem_string("bindings"), gem_v_bindings);
        GemVal _t1071 = _t1068;
        gem_pop_frame();
        return _t1071;
    }
#line 611 "compiler/main.gem"
    GemVal _t1072 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1072, gem_string("type")), gem_string("NUMBER")))) {
#line 612 "compiler/main.gem"
    GemVal _t1073 = (*gem_v_advance);
        (void)(_t1073.fn(_t1073.env, NULL, 0));
#line 613 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 614 "compiler/main.gem"
    GemVal _t1074 = gem_v_t;
    GemVal _t1075[] = {gem_table_get(_t1074, gem_string("value"))};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t1075, 1))) {
#line 615 "compiler/main.gem"
    GemVal _t1076 = gem_v_t;
    GemVal _t1077[] = {gem_table_get(_t1076, gem_string("value"))};
    GemVal _t1078[] = {gem_fn_atof(NULL, _t1077, 1)};
    GemVal _t1079 = (*gem_v__mod_parser_make_float);
            gem_v_lit = _t1079.fn(_t1079.env, _t1078, 1);
        } else {
#line 617 "compiler/main.gem"
    GemVal _t1080 = gem_v_t;
    GemVal _t1081[] = {gem_table_get(_t1080, gem_string("value"))};
    GemVal _t1082[] = {gem_fn__mod_parser_str_to_int(NULL, _t1081, 1)};
    GemVal _t1083 = (*gem_v__mod_parser_make_int);
            gem_v_lit = _t1083.fn(_t1083.env, _t1082, 1);
        }
#line 619 "compiler/main.gem"
    GemVal _t1084 = gem_table_new();
    GemVal _t1085[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    GemVal _t1086 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1084, gem_string("condition"), _t1086.fn(_t1086.env, _t1085, 3));
    GemVal _t1087 = gem_table_new();
    gem_table_set(_t1084, gem_string("bindings"), _t1087);
        GemVal _t1088 = _t1084;
        gem_pop_frame();
        return _t1088;
    }
#line 621 "compiler/main.gem"
    GemVal _t1089 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1089, gem_string("type")), gem_string("STRING")))) {
#line 622 "compiler/main.gem"
    GemVal _t1090 = (*gem_v_advance);
        (void)(_t1090.fn(_t1090.env, NULL, 0));
#line 623 "compiler/main.gem"
    GemVal _t1091 = gem_table_new();
    GemVal _t1092 = gem_v_t;
    GemVal _t1093[] = {gem_table_get(_t1092, gem_string("value"))};
    GemVal _t1094 = (*gem_v__mod_parser_make_string);
    GemVal _t1095[] = {gem_string("=="), gem_v_target_expr, _t1094.fn(_t1094.env, _t1093, 1)};
    GemVal _t1096 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1091, gem_string("condition"), _t1096.fn(_t1096.env, _t1095, 3));
    GemVal _t1097 = gem_table_new();
    gem_table_set(_t1091, gem_string("bindings"), _t1097);
        GemVal _t1098 = _t1091;
        gem_pop_frame();
        return _t1098;
    }
#line 625 "compiler/main.gem"
    GemVal _t1099 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1099, gem_string("type")), gem_string("true")))) {
#line 626 "compiler/main.gem"
    GemVal _t1100 = (*gem_v_advance);
        (void)(_t1100.fn(_t1100.env, NULL, 0));
#line 627 "compiler/main.gem"
    GemVal _t1101 = gem_table_new();
    GemVal _t1102[] = {gem_bool(1)};
    GemVal _t1103 = (*gem_v__mod_parser_make_bool);
    GemVal _t1104[] = {gem_string("=="), gem_v_target_expr, _t1103.fn(_t1103.env, _t1102, 1)};
    GemVal _t1105 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1101, gem_string("condition"), _t1105.fn(_t1105.env, _t1104, 3));
    GemVal _t1106 = gem_table_new();
    gem_table_set(_t1101, gem_string("bindings"), _t1106);
        GemVal _t1107 = _t1101;
        gem_pop_frame();
        return _t1107;
    }
#line 629 "compiler/main.gem"
    GemVal _t1108 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1108, gem_string("type")), gem_string("false")))) {
#line 630 "compiler/main.gem"
    GemVal _t1109 = (*gem_v_advance);
        (void)(_t1109.fn(_t1109.env, NULL, 0));
#line 631 "compiler/main.gem"
    GemVal _t1110 = gem_table_new();
    GemVal _t1111[] = {gem_bool(0)};
    GemVal _t1112 = (*gem_v__mod_parser_make_bool);
    GemVal _t1113[] = {gem_string("=="), gem_v_target_expr, _t1112.fn(_t1112.env, _t1111, 1)};
    GemVal _t1114 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1110, gem_string("condition"), _t1114.fn(_t1114.env, _t1113, 3));
    GemVal _t1115 = gem_table_new();
    gem_table_set(_t1110, gem_string("bindings"), _t1115);
        GemVal _t1116 = _t1110;
        gem_pop_frame();
        return _t1116;
    }
#line 633 "compiler/main.gem"
    GemVal _t1117 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1117, gem_string("type")), gem_string("nil")))) {
#line 634 "compiler/main.gem"
    GemVal _t1118 = (*gem_v_advance);
        (void)(_t1118.fn(_t1118.env, NULL, 0));
#line 635 "compiler/main.gem"
    GemVal _t1119 = gem_table_new();
    GemVal _t1120 = (*gem_v__mod_parser_make_nil_node);
    GemVal _t1121[] = {gem_string("=="), gem_v_target_expr, _t1120.fn(_t1120.env, NULL, 0)};
    GemVal _t1122 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1119, gem_string("condition"), _t1122.fn(_t1122.env, _t1121, 3));
    GemVal _t1123 = gem_table_new();
    gem_table_set(_t1119, gem_string("bindings"), _t1123);
        GemVal _t1124 = _t1119;
        gem_pop_frame();
        return _t1124;
    }
#line 639 "compiler/main.gem"
    GemVal _t1125 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1125, gem_string("type")), gem_string("NAME")))) {
#line 640 "compiler/main.gem"
    GemVal _t1126 = (*gem_v_advance);
        (void)(_t1126.fn(_t1126.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t1127 = gem_table_new();
    GemVal _t1128[] = {gem_bool(1)};
    GemVal _t1129 = (*gem_v__mod_parser_make_bool);
    gem_table_set(_t1127, gem_string("condition"), _t1129.fn(_t1129.env, _t1128, 1));
    GemVal _t1130 = gem_table_new();
    GemVal _t1131 = gem_v_t;
    GemVal _t1132[] = {gem_table_get(_t1131, gem_string("value")), gem_v_target_expr, gem_int(0)};
    GemVal _t1133 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1130, gem_int(0), _t1133.fn(_t1133.env, _t1132, 3));
    gem_table_set(_t1127, gem_string("bindings"), _t1130);
        GemVal _t1134 = _t1127;
        gem_pop_frame();
        return _t1134;
    }
#line 644 "compiler/main.gem"
    GemVal _t1135 = gem_v_t;
    GemVal _t1136 = gem_v_t;
    GemVal _t1137 = gem_v_t;
    GemVal _t1138[] = {gem_table_get(_t1137, gem_string("value"))};
    GemVal _t1139 = gem_v_t;
    GemVal _t1140[] = {gem_table_get(_t1139, gem_string("type"))};
    GemVal _t1141[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1135, gem_string("line")), gem_table_get(_t1136, gem_string("col")), gem_len_fn(NULL, _t1138, 1), gem_add(gem_add(gem_string("expected pattern but got '"), gem_to_string_fn(NULL, _t1140, 1)), gem_string("'")), GEM_NIL};
    GemVal _t1142 = (*gem_v__mod_parser_compile_error);
    GemVal _t1143 = _t1142.fn(_t1142.env, _t1141, 7);
    gem_pop_frame();
    return _t1143;
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
    GemVal _t1145 = (*gem_v_peek);
    GemVal gem_v_t = _t1145.fn(_t1145.env, NULL, 0);
#line 653 "compiler/main.gem"
    GemVal _t1146 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1146, gem_string("type")), gem_string("let")))) {
#line 654 "compiler/main.gem"
    GemVal _t1147 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1147, gem_string("line"));
#line 655 "compiler/main.gem"
    GemVal _t1148 = (*gem_v_advance);
        (void)(_t1148.fn(_t1148.env, NULL, 0));
#line 658 "compiler/main.gem"
    GemVal _t1149 = (*gem_v_peek);
    GemVal _t1150 = _t1149.fn(_t1149.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1150, gem_string("type")), gem_string("{")))) {
#line 659 "compiler/main.gem"
    GemVal _t1151 = (*gem_v_advance);
            (void)(_t1151.fn(_t1151.env, NULL, 0));
#line 660 "compiler/main.gem"
    GemVal _t1152 = gem_table_new();
            GemVal gem_v_names = _t1152;
#line 661 "compiler/main.gem"
    GemVal _t1153 = (*gem_v_skip_nl);
            (void)(_t1153.fn(_t1153.env, NULL, 0));
#line 662 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1154 = (*gem_v_peek);
                GemVal _t1155 = _t1154.fn(_t1154.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1155, gem_string("type")), gem_string("}")))) break;
#line 663 "compiler/main.gem"
    GemVal _t1156 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1156.fn(_t1156.env, NULL, 0);
#line 664 "compiler/main.gem"
    GemVal _t1157 = gem_v_field_tok;
                if (gem_truthy(gem_eq(gem_table_get(_t1157, gem_string("type")), gem_string("NAME")))) {
#line 665 "compiler/main.gem"
    GemVal _t1158 = (*gem_v_advance);
                    (void)(_t1158.fn(_t1158.env, NULL, 0));
                } else {
#line 668 "compiler/main.gem"
    GemVal _t1159 = (*gem_v_advance);
                    (void)(_t1159.fn(_t1159.env, NULL, 0));
                }
#line 670 "compiler/main.gem"
    GemVal _t1160 = gem_v_field_tok;
    GemVal _t1161[] = {gem_v_names, gem_table_get(_t1160, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1161, 2));
#line 671 "compiler/main.gem"
    GemVal _t1162 = (*gem_v_skip_nl);
                (void)(_t1162.fn(_t1162.env, NULL, 0));
#line 672 "compiler/main.gem"
    GemVal _t1163 = (*gem_v_peek);
    GemVal _t1164 = _t1163.fn(_t1163.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1164, gem_string("type")), gem_string(",")))) {
#line 673 "compiler/main.gem"
    GemVal _t1165 = (*gem_v_advance);
                    (void)(_t1165.fn(_t1165.env, NULL, 0));
#line 674 "compiler/main.gem"
    GemVal _t1166 = (*gem_v_skip_nl);
                    (void)(_t1166.fn(_t1166.env, NULL, 0));
                }
            }
#line 677 "compiler/main.gem"
    GemVal _t1167[] = {gem_string("}")};
    GemVal _t1168 = (*gem_v_expect);
            (void)(_t1168.fn(_t1168.env, _t1167, 1));
#line 678 "compiler/main.gem"
    GemVal _t1169[] = {gem_string("=")};
    GemVal _t1170 = (*gem_v_expect);
            (void)(_t1170.fn(_t1170.env, _t1169, 1));
#line 679 "compiler/main.gem"
    GemVal _t1171 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1171.fn(_t1171.env, NULL, 0);
#line 680 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 681 "compiler/main.gem"
    GemVal _t1172[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1172, 1));
#line 682 "compiler/main.gem"
    GemVal _t1173 = gem_table_new();
    GemVal _t1174[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1175 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1173, gem_int(0), _t1175.fn(_t1175.env, _t1174, 3));
            GemVal gem_v_stmts = _t1173;
#line 683 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 683 "compiler/main.gem"
    GemVal _t1176[] = {gem_v_names};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1176, 1);
#line 683 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 683 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_8;
#line 683 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 684 "compiler/main.gem"
    GemVal _t1177[] = {gem_v_tmp};
    GemVal _t1178 = (*gem_v__mod_parser_make_var);
    GemVal _t1179[] = {_t1178.fn(_t1178.env, _t1177, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1180 = (*gem_v__mod_parser_make_dot);
    GemVal _t1181[] = {gem_table_get(gem_v_names, gem_v_di), _t1180.fn(_t1180.env, _t1179, 2), gem_v_line};
    GemVal _t1182 = (*gem_v__mod_parser_make_let);
    GemVal _t1183[] = {gem_v_stmts, _t1182.fn(_t1182.env, _t1181, 3)};
                (void)(gem_push_fn(NULL, _t1183, 2));
            }

#line 686 "compiler/main.gem"
    GemVal _t1184 = gem_table_new();
    gem_table_set(_t1184, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1184, gem_string("stmts"), gem_v_stmts);
            GemVal _t1185 = _t1184;
            gem_pop_frame();
            return _t1185;
        }
#line 690 "compiler/main.gem"
    GemVal _t1186 = (*gem_v_peek);
    GemVal _t1187 = _t1186.fn(_t1186.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1187, gem_string("type")), gem_string("[")))) {
#line 691 "compiler/main.gem"
    GemVal _t1188 = (*gem_v_advance);
            (void)(_t1188.fn(_t1188.env, NULL, 0));
#line 692 "compiler/main.gem"
    GemVal _t1189 = gem_table_new();
            GemVal gem_v_names = _t1189;
#line 693 "compiler/main.gem"
    GemVal _t1190 = (*gem_v_skip_nl);
            (void)(_t1190.fn(_t1190.env, NULL, 0));
#line 694 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1191 = (*gem_v_peek);
                GemVal _t1192 = _t1191.fn(_t1191.env, NULL, 0);
                if (!gem_truthy(gem_neq(gem_table_get(_t1192, gem_string("type")), gem_string("]")))) break;
#line 695 "compiler/main.gem"
    GemVal _t1193[] = {gem_string("NAME")};
    GemVal _t1194 = (*gem_v_expect);
    GemVal _t1195 = _t1194.fn(_t1194.env, _t1193, 1);
    GemVal _t1196[] = {gem_v_names, gem_table_get(_t1195, gem_string("value"))};
                (void)(gem_push_fn(NULL, _t1196, 2));
#line 696 "compiler/main.gem"
    GemVal _t1197 = (*gem_v_skip_nl);
                (void)(_t1197.fn(_t1197.env, NULL, 0));
#line 697 "compiler/main.gem"
    GemVal _t1198 = (*gem_v_peek);
    GemVal _t1199 = _t1198.fn(_t1198.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1199, gem_string("type")), gem_string(",")))) {
#line 698 "compiler/main.gem"
    GemVal _t1200 = (*gem_v_advance);
                    (void)(_t1200.fn(_t1200.env, NULL, 0));
#line 699 "compiler/main.gem"
    GemVal _t1201 = (*gem_v_skip_nl);
                    (void)(_t1201.fn(_t1201.env, NULL, 0));
                }
            }
#line 702 "compiler/main.gem"
    GemVal _t1202[] = {gem_string("]")};
    GemVal _t1203 = (*gem_v_expect);
            (void)(_t1203.fn(_t1203.env, _t1202, 1));
#line 703 "compiler/main.gem"
    GemVal _t1204[] = {gem_string("=")};
    GemVal _t1205 = (*gem_v_expect);
            (void)(_t1205.fn(_t1205.env, _t1204, 1));
#line 704 "compiler/main.gem"
    GemVal _t1206 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1206.fn(_t1206.env, NULL, 0);
#line 705 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 706 "compiler/main.gem"
    GemVal _t1207[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_add(gem_string("_d"), gem_to_string_fn(NULL, _t1207, 1));
#line 707 "compiler/main.gem"
    GemVal _t1208 = gem_table_new();
    GemVal _t1209[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1210 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1208, gem_int(0), _t1210.fn(_t1210.env, _t1209, 3));
            GemVal gem_v_stmts = _t1208;
#line 708 "compiler/main.gem"
            GemVal gem_v__for_i_9 = gem_int(0);
#line 708 "compiler/main.gem"
    GemVal _t1211[] = {gem_v_names};
            GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1211, 1);
#line 708 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 708 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_9;
#line 708 "compiler/main.gem"
                gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 709 "compiler/main.gem"
    GemVal _t1212[] = {gem_v_tmp};
    GemVal _t1213 = (*gem_v__mod_parser_make_var);
    GemVal _t1214[] = {gem_v_di};
    GemVal _t1215 = (*gem_v__mod_parser_make_int);
    GemVal _t1216[] = {_t1213.fn(_t1213.env, _t1212, 1), _t1215.fn(_t1215.env, _t1214, 1)};
    GemVal _t1217 = (*gem_v__mod_parser_make_index);
    GemVal _t1218[] = {gem_table_get(gem_v_names, gem_v_di), _t1217.fn(_t1217.env, _t1216, 2), gem_v_line};
    GemVal _t1219 = (*gem_v__mod_parser_make_let);
    GemVal _t1220[] = {gem_v_stmts, _t1219.fn(_t1219.env, _t1218, 3)};
                (void)(gem_push_fn(NULL, _t1220, 2));
            }

#line 711 "compiler/main.gem"
    GemVal _t1221 = gem_table_new();
    gem_table_set(_t1221, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1221, gem_string("stmts"), gem_v_stmts);
            GemVal _t1222 = _t1221;
            gem_pop_frame();
            return _t1222;
        }
#line 714 "compiler/main.gem"
    GemVal _t1223[] = {gem_string("NAME")};
    GemVal _t1224 = (*gem_v_expect);
    GemVal _t1225 = _t1224.fn(_t1224.env, _t1223, 1);
        GemVal gem_v_name = gem_table_get(_t1225, gem_string("value"));
#line 715 "compiler/main.gem"
    GemVal _t1226[] = {gem_string("=")};
    GemVal _t1227 = (*gem_v_expect);
        (void)(_t1227.fn(_t1227.env, _t1226, 1));
#line 716 "compiler/main.gem"
    GemVal _t1228 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1228.fn(_t1228.env, NULL, 0);
#line 717 "compiler/main.gem"
    GemVal _t1229[] = {gem_v_name, gem_v_value, gem_v_line};
    GemVal _t1230 = (*gem_v__mod_parser_make_let);
        GemVal _t1231 = _t1230.fn(_t1230.env, _t1229, 3);
        gem_pop_frame();
        return _t1231;
    }
#line 721 "compiler/main.gem"
    GemVal _t1232 = gem_v_t;
    GemVal _t1236;
    if (!gem_truthy(gem_eq(gem_table_get(_t1232, gem_string("type")), gem_string("fn")))) {
        _t1236 = gem_eq(gem_table_get(_t1232, gem_string("type")), gem_string("fn"));
    } else {
        GemVal _t1233[] = {gem_int(1)};
        GemVal _t1234 = (*gem_v_peek_at);
        GemVal _t1235 = _t1234.fn(_t1234.env, _t1233, 1);
        _t1236 = gem_eq(gem_table_get(_t1235, gem_string("type")), gem_string("NAME"));
    }
    if (gem_truthy(_t1236)) {
#line 722 "compiler/main.gem"
    GemVal _t1237 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1237, gem_string("line"));
#line 723 "compiler/main.gem"
    GemVal _t1238 = (*gem_v_advance);
        (void)(_t1238.fn(_t1238.env, NULL, 0));
#line 724 "compiler/main.gem"
    GemVal _t1239[] = {gem_string("NAME")};
    GemVal _t1240 = (*gem_v_expect);
    GemVal _t1241 = _t1240.fn(_t1240.env, _t1239, 1);
        GemVal gem_v_name = gem_table_get(_t1241, gem_string("value"));
#line 725 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 726 "compiler/main.gem"
    GemVal _t1242[] = {gem_v_name};
    GemVal _t1243[] = {gem_v_name};
    GemVal _t1244[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1242, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_add(gem_add(gem_string("use: let "), gem_to_string_fn(NULL, _t1243, 1)), gem_string(" = fn(...) ... end"))};
    GemVal _t1245 = (*gem_v__mod_parser_compile_error);
            (void)(_t1245.fn(_t1245.env, _t1244, 7));
        }
#line 728 "compiler/main.gem"
    GemVal _t1246[] = {gem_string("(")};
    GemVal _t1247 = (*gem_v_expect);
        (void)(_t1247.fn(_t1247.env, _t1246, 1));
#line 729 "compiler/main.gem"
    GemVal _t1248 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1248.fn(_t1248.env, NULL, 0);
#line 730 "compiler/main.gem"
    GemVal _t1249[] = {gem_string(")")};
    GemVal _t1250 = (*gem_v_expect);
        (void)(_t1250.fn(_t1250.env, _t1249, 1));
#line 731 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 732 "compiler/main.gem"
    GemVal _t1251 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1251.fn(_t1251.env, NULL, 0);
#line 733 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 734 "compiler/main.gem"
    GemVal _t1252[] = {gem_string("end")};
    GemVal _t1253 = (*gem_v_expect);
        (void)(_t1253.fn(_t1253.env, _t1252, 1));
#line 735 "compiler/main.gem"
    GemVal _t1254 = gem_v_pr;
    GemVal _t1255 = gem_v_pr;
    GemVal _t1256[] = {gem_v_name, gem_table_get(_t1254, gem_string("params")), gem_table_get(_t1255, gem_string("rest_param")), gem_v_body, gem_v_line};
    GemVal _t1257 = (*gem_v__mod_parser_make_fn_def);
        GemVal _t1258 = _t1257.fn(_t1257.env, _t1256, 5);
        gem_pop_frame();
        return _t1258;
    }
#line 739 "compiler/main.gem"
    GemVal _t1259 = gem_v_t;
    GemVal _t1261;
    if (gem_truthy(gem_eq(gem_table_get(_t1259, gem_string("type")), gem_string("if")))) {
        _t1261 = gem_eq(gem_table_get(_t1259, gem_string("type")), gem_string("if"));
    } else {
        GemVal _t1260 = gem_v_t;
        _t1261 = gem_eq(gem_table_get(_t1260, gem_string("type")), gem_string("elif"));
    }
    if (gem_truthy(_t1261)) {
#line 740 "compiler/main.gem"
    GemVal _t1262 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1262, gem_string("line"));
#line 741 "compiler/main.gem"
    GemVal _t1263 = (*gem_v_advance);
        (void)(_t1263.fn(_t1263.env, NULL, 0));
#line 742 "compiler/main.gem"
    GemVal _t1264 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1264.fn(_t1264.env, NULL, 0);
#line 743 "compiler/main.gem"
    GemVal _t1265 = (*gem_v_peek);
    GemVal _t1266 = _t1265.fn(_t1265.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1266, gem_string("type")), gem_string("then")))) {
#line 744 "compiler/main.gem"
    GemVal _t1267 = (*gem_v_advance);
            (void)(_t1267.fn(_t1267.env, NULL, 0));
        }
#line 746 "compiler/main.gem"
    GemVal _t1268 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1268.fn(_t1268.env, NULL, 0);
#line 747 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 748 "compiler/main.gem"
    GemVal _t1269 = (*gem_v_peek);
    GemVal _t1270 = _t1269.fn(_t1269.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1270, gem_string("type")), gem_string("elif")))) {
#line 750 "compiler/main.gem"
    GemVal _t1271 = gem_table_new();
    GemVal _t1272 = (*gem_v_parse_stmt);
    gem_table_set(_t1271, gem_int(0), _t1272.fn(_t1272.env, NULL, 0));
            gem_v_else_body = _t1271;
        } else {
#line 751 "compiler/main.gem"
    GemVal _t1273 = (*gem_v_peek);
    GemVal _t1274 = _t1273.fn(_t1273.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1274, gem_string("type")), gem_string("else")))) {
#line 752 "compiler/main.gem"
    GemVal _t1275 = (*gem_v_advance);
                (void)(_t1275.fn(_t1275.env, NULL, 0));
#line 753 "compiler/main.gem"
    GemVal _t1276 = (*gem_v_parse_body);
                gem_v_else_body = _t1276.fn(_t1276.env, NULL, 0);
            }
        }
#line 756 "compiler/main.gem"
    GemVal _t1277 = gem_v_t;
        if (gem_truthy(gem_eq(gem_table_get(_t1277, gem_string("type")), gem_string("if")))) {
#line 757 "compiler/main.gem"
    GemVal _t1278[] = {gem_string("end")};
    GemVal _t1279 = (*gem_v_expect);
            (void)(_t1279.fn(_t1279.env, _t1278, 1));
        }
#line 759 "compiler/main.gem"
    GemVal _t1280[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
    GemVal _t1281 = (*gem_v__mod_parser_make_if);
        GemVal _t1282 = _t1281.fn(_t1281.env, _t1280, 4);
        gem_pop_frame();
        return _t1282;
    }
#line 763 "compiler/main.gem"
    GemVal _t1283 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1283, gem_string("type")), gem_string("while")))) {
#line 764 "compiler/main.gem"
    GemVal _t1284 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1284, gem_string("line"));
#line 765 "compiler/main.gem"
    GemVal _t1285 = (*gem_v_advance);
        (void)(_t1285.fn(_t1285.env, NULL, 0));
#line 766 "compiler/main.gem"
    GemVal _t1286 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1286.fn(_t1286.env, NULL, 0);
#line 767 "compiler/main.gem"
    GemVal _t1287 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1287.fn(_t1287.env, NULL, 0);
#line 768 "compiler/main.gem"
    GemVal _t1288[] = {gem_string("end")};
    GemVal _t1289 = (*gem_v_expect);
        (void)(_t1289.fn(_t1289.env, _t1288, 1));
#line 769 "compiler/main.gem"
    GemVal _t1290[] = {gem_v_cond, gem_v_wbody, gem_v_line};
    GemVal _t1291 = (*gem_v__mod_parser_make_while);
        GemVal _t1292 = _t1291.fn(_t1291.env, _t1290, 3);
        gem_pop_frame();
        return _t1292;
    }
#line 773 "compiler/main.gem"
    GemVal _t1293 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1293, gem_string("type")), gem_string("for")))) {
#line 774 "compiler/main.gem"
    GemVal _t1294 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1294, gem_string("line"));
#line 775 "compiler/main.gem"
    GemVal _t1295 = (*gem_v_advance);
        (void)(_t1295.fn(_t1295.env, NULL, 0));
#line 776 "compiler/main.gem"
    GemVal _t1296[] = {gem_string("NAME")};
    GemVal _t1297 = (*gem_v_expect);
    GemVal _t1298 = _t1297.fn(_t1297.env, _t1296, 1);
        GemVal gem_v_var_name = gem_table_get(_t1298, gem_string("value"));
#line 778 "compiler/main.gem"
    GemVal _t1299 = (*gem_v_peek);
    GemVal _t1300 = _t1299.fn(_t1299.env, NULL, 0);
    GemVal _t1304;
    if (!gem_truthy(gem_eq(gem_table_get(_t1300, gem_string("type")), gem_string(",")))) {
        _t1304 = gem_eq(gem_table_get(_t1300, gem_string("type")), gem_string(","));
    } else {
        GemVal _t1301[] = {gem_int(1)};
        GemVal _t1302 = (*gem_v_peek_at);
        GemVal _t1303 = _t1302.fn(_t1302.env, _t1301, 1);
        _t1304 = gem_eq(gem_table_get(_t1303, gem_string("type")), gem_string("NAME"));
    }
        if (gem_truthy(_t1304)) {
#line 780 "compiler/main.gem"
    GemVal _t1305 = (*gem_v_advance);
            (void)(_t1305.fn(_t1305.env, NULL, 0));
#line 781 "compiler/main.gem"
    GemVal _t1306[] = {gem_string("NAME")};
    GemVal _t1307 = (*gem_v_expect);
    GemVal _t1308 = _t1307.fn(_t1307.env, _t1306, 1);
            GemVal gem_v_val_name = gem_table_get(_t1308, gem_string("value"));
#line 782 "compiler/main.gem"
    GemVal _t1309[] = {gem_string("in")};
    GemVal _t1310 = (*gem_v_expect);
            (void)(_t1310.fn(_t1310.env, _t1309, 1));
#line 783 "compiler/main.gem"
    GemVal _t1311 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1311.fn(_t1311.env, NULL, 0);
#line 784 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 785 "compiler/main.gem"
    GemVal _t1312[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_add(gem_string("_for_tbl_"), gem_to_string_fn(NULL, _t1312, 1));
#line 786 "compiler/main.gem"
    GemVal _t1313[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_len_var = gem_add(gem_string("_for_len_"), gem_to_string_fn(NULL, _t1313, 1));
#line 787 "compiler/main.gem"
    GemVal _t1314[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1314, 1));
#line 788 "compiler/main.gem"
    GemVal _t1315 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1315.fn(_t1315.env, NULL, 0);
#line 789 "compiler/main.gem"
    GemVal _t1316[] = {gem_string("end")};
    GemVal _t1317 = (*gem_v_expect);
            (void)(_t1317.fn(_t1317.env, _t1316, 1));
#line 790 "compiler/main.gem"
    GemVal _t1318 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1318;
#line 791 "compiler/main.gem"
    GemVal _t1319[] = {gem_string("__table_key_at")};
    GemVal _t1320 = (*gem_v__mod_parser_make_var);
    GemVal _t1321 = gem_table_new();
    GemVal _t1322[] = {gem_v_tbl_var};
    GemVal _t1323 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1321, gem_int(0), _t1323.fn(_t1323.env, _t1322, 1));
    GemVal _t1324[] = {gem_v_idx_var};
    GemVal _t1325 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1321, gem_int(1), _t1325.fn(_t1325.env, _t1324, 1));
    GemVal _t1326[] = {_t1320.fn(_t1320.env, _t1319, 1), _t1321, gem_int(0)};
    GemVal _t1327 = (*gem_v__mod_parser_make_call);
    GemVal _t1328[] = {gem_v_var_name, _t1327.fn(_t1327.env, _t1326, 3), gem_v_line};
    GemVal _t1329 = (*gem_v__mod_parser_make_let);
    GemVal _t1330[] = {gem_v_inner_stmts, _t1329.fn(_t1329.env, _t1328, 3)};
            (void)(gem_push_fn(NULL, _t1330, 2));
#line 792 "compiler/main.gem"
    GemVal _t1331[] = {gem_string("__table_val_at")};
    GemVal _t1332 = (*gem_v__mod_parser_make_var);
    GemVal _t1333 = gem_table_new();
    GemVal _t1334[] = {gem_v_tbl_var};
    GemVal _t1335 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1333, gem_int(0), _t1335.fn(_t1335.env, _t1334, 1));
    GemVal _t1336[] = {gem_v_idx_var};
    GemVal _t1337 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1333, gem_int(1), _t1337.fn(_t1337.env, _t1336, 1));
    GemVal _t1338[] = {_t1332.fn(_t1332.env, _t1331, 1), _t1333, gem_int(0)};
    GemVal _t1339 = (*gem_v__mod_parser_make_call);
    GemVal _t1340[] = {gem_v_val_name, _t1339.fn(_t1339.env, _t1338, 3), gem_v_line};
    GemVal _t1341 = (*gem_v__mod_parser_make_let);
    GemVal _t1342[] = {gem_v_inner_stmts, _t1341.fn(_t1341.env, _t1340, 3)};
            (void)(gem_push_fn(NULL, _t1342, 2));
#line 793 "compiler/main.gem"
    GemVal _t1343[] = {gem_v_idx_var};
    GemVal _t1344 = (*gem_v__mod_parser_make_var);
    GemVal _t1345[] = {gem_int(1)};
    GemVal _t1346 = (*gem_v__mod_parser_make_int);
    GemVal _t1347[] = {gem_string("+"), _t1344.fn(_t1344.env, _t1343, 1), _t1346.fn(_t1346.env, _t1345, 1)};
    GemVal _t1348 = (*gem_v__mod_parser_make_binop);
    GemVal _t1349[] = {gem_v_idx_var, _t1348.fn(_t1348.env, _t1347, 3), gem_v_line};
    GemVal _t1350 = (*gem_v__mod_parser_make_assign);
    GemVal _t1351[] = {gem_v_inner_stmts, _t1350.fn(_t1350.env, _t1349, 3)};
            (void)(gem_push_fn(NULL, _t1351, 2));
#line 794 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 794 "compiler/main.gem"
    GemVal _t1352[] = {gem_v_fbody};
            GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1352, 1);
#line 794 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 794 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_10;
#line 794 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 795 "compiler/main.gem"
    GemVal _t1353[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1353, 2));
            }

#line 797 "compiler/main.gem"
    GemVal _t1354[] = {gem_v_idx_var};
    GemVal _t1355 = (*gem_v__mod_parser_make_var);
    GemVal _t1356[] = {gem_v_len_var};
    GemVal _t1357 = (*gem_v__mod_parser_make_var);
    GemVal _t1358[] = {gem_string("<"), _t1355.fn(_t1355.env, _t1354, 1), _t1357.fn(_t1357.env, _t1356, 1)};
    GemVal _t1359 = (*gem_v__mod_parser_make_binop);
    GemVal _t1360[] = {_t1359.fn(_t1359.env, _t1358, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1361 = (*gem_v__mod_parser_make_while);
            GemVal gem_v_while_node = _t1361.fn(_t1361.env, _t1360, 3);
#line 802 "compiler/main.gem"
    GemVal _t1362 = gem_table_new();
    gem_table_set(_t1362, gem_string("tag"), gem_string("block"));
    GemVal _t1363 = gem_table_new();
    GemVal _t1364[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    GemVal _t1365 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1363, gem_int(0), _t1365.fn(_t1365.env, _t1364, 3));
    GemVal _t1366[] = {gem_string("len")};
    GemVal _t1367 = (*gem_v__mod_parser_make_var);
    GemVal _t1368 = gem_table_new();
    GemVal _t1369[] = {gem_v_tbl_var};
    GemVal _t1370 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1368, gem_int(0), _t1370.fn(_t1370.env, _t1369, 1));
    GemVal _t1371[] = {_t1367.fn(_t1367.env, _t1366, 1), _t1368, gem_int(0)};
    GemVal _t1372 = (*gem_v__mod_parser_make_call);
    GemVal _t1373[] = {gem_v_len_var, _t1372.fn(_t1372.env, _t1371, 3), gem_v_line};
    GemVal _t1374 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1363, gem_int(1), _t1374.fn(_t1374.env, _t1373, 3));
    GemVal _t1375[] = {gem_int(0)};
    GemVal _t1376 = (*gem_v__mod_parser_make_int);
    GemVal _t1377[] = {gem_v_idx_var, _t1376.fn(_t1376.env, _t1375, 1), gem_v_line};
    GemVal _t1378 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1363, gem_int(2), _t1378.fn(_t1378.env, _t1377, 3));
    gem_table_set(_t1363, gem_int(3), gem_v_while_node);
    gem_table_set(_t1362, gem_string("stmts"), _t1363);
            GemVal _t1379 = _t1362;
            gem_pop_frame();
            return _t1379;
        } else {
#line 808 "compiler/main.gem"
    GemVal _t1380 = (*gem_v_peek);
    GemVal _t1381 = _t1380.fn(_t1380.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1381, gem_string("type")), gem_string("in")))) {
#line 810 "compiler/main.gem"
    GemVal _t1382 = (*gem_v_advance);
                (void)(_t1382.fn(_t1382.env, NULL, 0));
#line 811 "compiler/main.gem"
    GemVal _t1383 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1383.fn(_t1383.env, NULL, 0);
#line 812 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 813 "compiler/main.gem"
    GemVal _t1384[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_add(gem_string("_for_items_"), gem_to_string_fn(NULL, _t1384, 1));
#line 814 "compiler/main.gem"
    GemVal _t1385[] = {(*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1385, 1));
#line 815 "compiler/main.gem"
    GemVal _t1386 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1386.fn(_t1386.env, NULL, 0);
#line 816 "compiler/main.gem"
    GemVal _t1387[] = {gem_string("end")};
    GemVal _t1388 = (*gem_v_expect);
                (void)(_t1388.fn(_t1388.env, _t1387, 1));
#line 817 "compiler/main.gem"
    GemVal _t1389 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1389;
#line 818 "compiler/main.gem"
    GemVal _t1390[] = {gem_v_items_var};
    GemVal _t1391 = (*gem_v__mod_parser_make_var);
    GemVal _t1392[] = {gem_v_idx_var};
    GemVal _t1393 = (*gem_v__mod_parser_make_var);
    GemVal _t1394[] = {_t1391.fn(_t1391.env, _t1390, 1), _t1393.fn(_t1393.env, _t1392, 1)};
    GemVal _t1395 = (*gem_v__mod_parser_make_index);
    GemVal _t1396[] = {gem_v_var_name, _t1395.fn(_t1395.env, _t1394, 2), gem_v_line};
    GemVal _t1397 = (*gem_v__mod_parser_make_let);
    GemVal _t1398[] = {gem_v_inner_stmts, _t1397.fn(_t1397.env, _t1396, 3)};
                (void)(gem_push_fn(NULL, _t1398, 2));
#line 819 "compiler/main.gem"
    GemVal _t1399[] = {gem_v_idx_var};
    GemVal _t1400 = (*gem_v__mod_parser_make_var);
    GemVal _t1401[] = {gem_int(1)};
    GemVal _t1402 = (*gem_v__mod_parser_make_int);
    GemVal _t1403[] = {gem_string("+"), _t1400.fn(_t1400.env, _t1399, 1), _t1402.fn(_t1402.env, _t1401, 1)};
    GemVal _t1404 = (*gem_v__mod_parser_make_binop);
    GemVal _t1405[] = {gem_v_idx_var, _t1404.fn(_t1404.env, _t1403, 3), gem_v_line};
    GemVal _t1406 = (*gem_v__mod_parser_make_assign);
    GemVal _t1407[] = {gem_v_inner_stmts, _t1406.fn(_t1406.env, _t1405, 3)};
                (void)(gem_push_fn(NULL, _t1407, 2));
#line 820 "compiler/main.gem"
                GemVal gem_v__for_i_11 = gem_int(0);
#line 820 "compiler/main.gem"
    GemVal _t1408[] = {gem_v_fbody};
                GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1408, 1);
#line 820 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 820 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_11;
#line 820 "compiler/main.gem"
                    gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 821 "compiler/main.gem"
    GemVal _t1409[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1409, 2));
                }

#line 823 "compiler/main.gem"
    GemVal _t1410[] = {gem_v_idx_var};
    GemVal _t1411 = (*gem_v__mod_parser_make_var);
    GemVal _t1412[] = {gem_string("len")};
    GemVal _t1413 = (*gem_v__mod_parser_make_var);
    GemVal _t1414 = gem_table_new();
    GemVal _t1415[] = {gem_v_items_var};
    GemVal _t1416 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1414, gem_int(0), _t1416.fn(_t1416.env, _t1415, 1));
    GemVal _t1417[] = {_t1413.fn(_t1413.env, _t1412, 1), _t1414, gem_int(0)};
    GemVal _t1418 = (*gem_v__mod_parser_make_call);
    GemVal _t1419[] = {gem_string("<"), _t1411.fn(_t1411.env, _t1410, 1), _t1418.fn(_t1418.env, _t1417, 3)};
    GemVal _t1420 = (*gem_v__mod_parser_make_binop);
    GemVal _t1421[] = {_t1420.fn(_t1420.env, _t1419, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1422 = (*gem_v__mod_parser_make_while);
                GemVal gem_v_while_node = _t1422.fn(_t1422.env, _t1421, 3);
#line 828 "compiler/main.gem"
    GemVal _t1423 = gem_table_new();
    gem_table_set(_t1423, gem_string("tag"), gem_string("block"));
    GemVal _t1424 = gem_table_new();
    GemVal _t1425[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    GemVal _t1426 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1424, gem_int(0), _t1426.fn(_t1426.env, _t1425, 3));
    GemVal _t1427[] = {gem_int(0)};
    GemVal _t1428 = (*gem_v__mod_parser_make_int);
    GemVal _t1429[] = {gem_v_idx_var, _t1428.fn(_t1428.env, _t1427, 1), gem_v_line};
    GemVal _t1430 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1424, gem_int(1), _t1430.fn(_t1430.env, _t1429, 3));
    gem_table_set(_t1424, gem_int(2), gem_v_while_node);
    gem_table_set(_t1423, gem_string("stmts"), _t1424);
                GemVal _t1431 = _t1423;
                gem_pop_frame();
                return _t1431;
            } else {
#line 833 "compiler/main.gem"
    GemVal _t1432 = (*gem_v_peek);
    GemVal _t1433 = _t1432.fn(_t1432.env, NULL, 0);
                if (gem_truthy(gem_eq(gem_table_get(_t1433, gem_string("type")), gem_string("=")))) {
#line 835 "compiler/main.gem"
    GemVal _t1434 = (*gem_v_advance);
                    (void)(_t1434.fn(_t1434.env, NULL, 0));
#line 836 "compiler/main.gem"
    GemVal _t1435 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1435.fn(_t1435.env, NULL, 0);
#line 837 "compiler/main.gem"
    GemVal _t1436[] = {gem_string(",")};
    GemVal _t1437 = (*gem_v_expect);
                    (void)(_t1437.fn(_t1437.env, _t1436, 1));
#line 838 "compiler/main.gem"
    GemVal _t1438 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1438.fn(_t1438.env, NULL, 0);
#line 839 "compiler/main.gem"
    GemVal _t1439 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1439.fn(_t1439.env, NULL, 0);
#line 840 "compiler/main.gem"
    GemVal _t1440[] = {gem_string("end")};
    GemVal _t1441 = (*gem_v_expect);
                    (void)(_t1441.fn(_t1441.env, _t1440, 1));
#line 841 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 842 "compiler/main.gem"
    GemVal _t1442[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_add(gem_string("_for_i_"), gem_to_string_fn(NULL, _t1442, 1));
#line 843 "compiler/main.gem"
    GemVal _t1443[] = {(*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_add(gem_string("_for_limit_"), gem_to_string_fn(NULL, _t1443, 1));
#line 844 "compiler/main.gem"
    GemVal _t1444 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1444;
#line 845 "compiler/main.gem"
    GemVal _t1445[] = {gem_v_idx_var};
    GemVal _t1446 = (*gem_v__mod_parser_make_var);
    GemVal _t1447[] = {gem_v_var_name, _t1446.fn(_t1446.env, _t1445, 1), gem_v_line};
    GemVal _t1448 = (*gem_v__mod_parser_make_let);
    GemVal _t1449[] = {gem_v_inner_stmts, _t1448.fn(_t1448.env, _t1447, 3)};
                    (void)(gem_push_fn(NULL, _t1449, 2));
#line 846 "compiler/main.gem"
    GemVal _t1450[] = {gem_v_idx_var};
    GemVal _t1451 = (*gem_v__mod_parser_make_var);
    GemVal _t1452[] = {gem_int(1)};
    GemVal _t1453 = (*gem_v__mod_parser_make_int);
    GemVal _t1454[] = {gem_string("+"), _t1451.fn(_t1451.env, _t1450, 1), _t1453.fn(_t1453.env, _t1452, 1)};
    GemVal _t1455 = (*gem_v__mod_parser_make_binop);
    GemVal _t1456[] = {gem_v_idx_var, _t1455.fn(_t1455.env, _t1454, 3), gem_v_line};
    GemVal _t1457 = (*gem_v__mod_parser_make_assign);
    GemVal _t1458[] = {gem_v_inner_stmts, _t1457.fn(_t1457.env, _t1456, 3)};
                    (void)(gem_push_fn(NULL, _t1458, 2));
#line 847 "compiler/main.gem"
                    GemVal gem_v__for_i_12 = gem_int(0);
#line 847 "compiler/main.gem"
    GemVal _t1459[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1459, 1);
#line 847 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 847 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_12;
#line 847 "compiler/main.gem"
                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 848 "compiler/main.gem"
    GemVal _t1460[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1460, 2));
                    }

#line 850 "compiler/main.gem"
    GemVal _t1461[] = {gem_v_idx_var};
    GemVal _t1462 = (*gem_v__mod_parser_make_var);
    GemVal _t1463[] = {gem_v_limit_var};
    GemVal _t1464 = (*gem_v__mod_parser_make_var);
    GemVal _t1465[] = {gem_string("<"), _t1462.fn(_t1462.env, _t1461, 1), _t1464.fn(_t1464.env, _t1463, 1)};
    GemVal _t1466 = (*gem_v__mod_parser_make_binop);
    GemVal _t1467[] = {_t1466.fn(_t1466.env, _t1465, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1468 = (*gem_v__mod_parser_make_while);
                    GemVal gem_v_while_node = _t1468.fn(_t1468.env, _t1467, 3);
#line 855 "compiler/main.gem"
    GemVal _t1469 = gem_table_new();
    gem_table_set(_t1469, gem_string("tag"), gem_string("block"));
    GemVal _t1470 = gem_table_new();
    GemVal _t1471[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    GemVal _t1472 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1470, gem_int(0), _t1472.fn(_t1472.env, _t1471, 3));
    GemVal _t1473[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    GemVal _t1474 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1470, gem_int(1), _t1474.fn(_t1474.env, _t1473, 3));
    gem_table_set(_t1470, gem_int(2), gem_v_while_node);
    gem_table_set(_t1469, gem_string("stmts"), _t1470);
                    GemVal _t1475 = _t1469;
                    gem_pop_frame();
                    return _t1475;
                } else {
#line 861 "compiler/main.gem"
    GemVal _t1476 = (*gem_v_peek);
    GemVal _t1477 = _t1476.fn(_t1476.env, NULL, 0);
    GemVal _t1478 = (*gem_v_peek);
    GemVal _t1479 = _t1478.fn(_t1478.env, NULL, 0);
    GemVal _t1480 = (*gem_v_peek);
    GemVal _t1481 = _t1480.fn(_t1480.env, NULL, 0);
    GemVal _t1482[] = {gem_table_get(_t1481, gem_string("value"))};
    GemVal _t1483[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1477, gem_string("line")), gem_table_get(_t1479, gem_string("col")), gem_len_fn(NULL, _t1482, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
    GemVal _t1484 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1484.fn(_t1484.env, _t1483, 7));
                }
            }
        }
    }
#line 866 "compiler/main.gem"
    GemVal _t1485 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1485, gem_string("type")), gem_string("match")))) {
#line 867 "compiler/main.gem"
    GemVal _t1486 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1486, gem_string("line"));
#line 868 "compiler/main.gem"
    GemVal _t1487 = (*gem_v_advance);
        (void)(_t1487.fn(_t1487.env, NULL, 0));
#line 869 "compiler/main.gem"
    GemVal _t1488 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1488.fn(_t1488.env, NULL, 0);
#line 870 "compiler/main.gem"
    GemVal _t1489 = (*gem_v_skip_nl);
        (void)(_t1489.fn(_t1489.env, NULL, 0));
#line 871 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 872 "compiler/main.gem"
    GemVal _t1490[] = {(*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_add(gem_string("_match_"), gem_to_string_fn(NULL, _t1490, 1));
#line 873 "compiler/main.gem"
    GemVal _t1491[] = {gem_v_match_var};
    GemVal _t1492 = (*gem_v__mod_parser_make_var);
        GemVal gem_v_target_var_expr = _t1492.fn(_t1492.env, _t1491, 1);
#line 874 "compiler/main.gem"
    GemVal _t1493 = gem_table_new();
        GemVal gem_v_whens = _t1493;
#line 875 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1494 = (*gem_v_peek);
            GemVal _t1495 = _t1494.fn(_t1494.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1495, gem_string("type")), gem_string("when")))) break;
#line 876 "compiler/main.gem"
    GemVal _t1496 = (*gem_v_advance);
            (void)(_t1496.fn(_t1496.env, NULL, 0));
#line 878 "compiler/main.gem"
    GemVal _t1497 = (*gem_v_peek);
    GemVal _t1498 = _t1497.fn(_t1497.env, NULL, 0);
            GemVal gem_v_pt = gem_table_get(_t1498, gem_string("type"));
#line 879 "compiler/main.gem"
    GemVal _t1499[] = {gem_int(1)};
    GemVal _t1500 = (*gem_v_peek_at);
    GemVal _t1501 = _t1500.fn(_t1500.env, _t1499, 1);
            GemVal gem_v_next = gem_table_get(_t1501, gem_string("type"));
#line 880 "compiler/main.gem"
    GemVal _t1502;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1502 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1502 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1502)) {
#line 881 "compiler/main.gem"
    GemVal _t1503[] = {gem_v_target_var_expr};
    GemVal _t1504 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1504.fn(_t1504.env, _t1503, 1);
#line 882 "compiler/main.gem"
    GemVal _t1505 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1505.fn(_t1505.env, NULL, 0);
#line 883 "compiler/main.gem"
    GemVal _t1506 = gem_v_pat;
    GemVal _t1507 = gem_v_pat;
    GemVal _t1508[] = {gem_table_get(_t1506, gem_string("condition")), gem_v_wbody, gem_table_get(_t1507, gem_string("bindings"))};
    GemVal _t1509 = (*gem_v__mod_parser_make_when);
    GemVal _t1510[] = {gem_v_whens, _t1509.fn(_t1509.env, _t1508, 3)};
                (void)(gem_push_fn(NULL, _t1510, 2));
            } else {
#line 884 "compiler/main.gem"
    GemVal _t1515;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1515 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1511;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1511 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1511 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1512;
        if (gem_truthy(_t1511)) {
                _t1512 = _t1511;
        } else {
                _t1512 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1513;
        if (gem_truthy(_t1512)) {
                _t1513 = _t1512;
        } else {
                _t1513 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1514;
        if (gem_truthy(_t1513)) {
                _t1514 = _t1513;
        } else {
                _t1514 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1515 = _t1514;
    }
                if (gem_truthy(_t1515)) {
#line 886 "compiler/main.gem"
    GemVal _t1516[] = {gem_v_target_var_expr};
    GemVal _t1517 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1517.fn(_t1517.env, _t1516, 1);
#line 887 "compiler/main.gem"
    GemVal _t1518 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1518.fn(_t1518.env, NULL, 0);
#line 888 "compiler/main.gem"
    GemVal _t1519 = gem_v_pat;
    GemVal _t1520 = gem_v_pat;
    GemVal _t1521[] = {gem_table_get(_t1519, gem_string("condition")), gem_v_wbody, gem_table_get(_t1520, gem_string("bindings"))};
    GemVal _t1522 = (*gem_v__mod_parser_make_when);
    GemVal _t1523[] = {gem_v_whens, _t1522.fn(_t1522.env, _t1521, 3)};
                    (void)(gem_push_fn(NULL, _t1523, 2));
                } else {
#line 891 "compiler/main.gem"
    GemVal _t1524 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1524.fn(_t1524.env, NULL, 0);
#line 892 "compiler/main.gem"
    GemVal _t1525 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1525.fn(_t1525.env, NULL, 0);
#line 893 "compiler/main.gem"
    GemVal _t1526[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1527 = (*gem_v__mod_parser_make_binop);
    GemVal _t1528 = gem_table_new();
    GemVal _t1529[] = {_t1527.fn(_t1527.env, _t1526, 3), gem_v_wbody, _t1528};
    GemVal _t1530 = (*gem_v__mod_parser_make_when);
    GemVal _t1531[] = {gem_v_whens, _t1530.fn(_t1530.env, _t1529, 3)};
                    (void)(gem_push_fn(NULL, _t1531, 2));
                }
            }
#line 895 "compiler/main.gem"
    GemVal _t1532 = (*gem_v_skip_nl);
            (void)(_t1532.fn(_t1532.env, NULL, 0));
        }
#line 897 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 898 "compiler/main.gem"
    GemVal _t1533 = (*gem_v_peek);
    GemVal _t1534 = _t1533.fn(_t1533.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1534, gem_string("type")), gem_string("else")))) {
#line 899 "compiler/main.gem"
    GemVal _t1535 = (*gem_v_advance);
            (void)(_t1535.fn(_t1535.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1536 = (*gem_v_parse_body);
            gem_v_else_body = _t1536.fn(_t1536.env, NULL, 0);
        }
#line 902 "compiler/main.gem"
    GemVal _t1537[] = {gem_string("end")};
    GemVal _t1538 = (*gem_v_expect);
        (void)(_t1538.fn(_t1538.env, _t1537, 1));
#line 903 "compiler/main.gem"
    GemVal _t1539[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
    GemVal _t1540 = (*gem_v__mod_parser_make_match);
        GemVal _t1541 = _t1540.fn(_t1540.env, _t1539, 5);
        gem_pop_frame();
        return _t1541;
    }
#line 907 "compiler/main.gem"
    GemVal _t1542 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1542, gem_string("type")), gem_string("return")))) {
#line 908 "compiler/main.gem"
    GemVal _t1543 = gem_v_t;
        GemVal gem_v_line = gem_table_get(_t1543, gem_string("line"));
#line 909 "compiler/main.gem"
    GemVal _t1544 = (*gem_v_advance);
        (void)(_t1544.fn(_t1544.env, NULL, 0));
#line 910 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 911 "compiler/main.gem"
    GemVal _t1545 = (*gem_v_peek);
    GemVal _t1546 = _t1545.fn(_t1545.env, NULL, 0);
    GemVal _t1549;
    if (!gem_truthy(gem_neq(gem_table_get(_t1546, gem_string("type")), gem_string("NEWLINE")))) {
        _t1549 = gem_neq(gem_table_get(_t1546, gem_string("type")), gem_string("NEWLINE"));
    } else {
        GemVal _t1547 = (*gem_v_peek);
        GemVal _t1548 = _t1547.fn(_t1547.env, NULL, 0);
        _t1549 = gem_neq(gem_table_get(_t1548, gem_string("type")), gem_string("EOF"));
    }
    GemVal _t1552;
    if (!gem_truthy(_t1549)) {
        _t1552 = _t1549;
    } else {
        GemVal _t1550 = (*gem_v_peek);
        GemVal _t1551 = _t1550.fn(_t1550.env, NULL, 0);
        _t1552 = gem_neq(gem_table_get(_t1551, gem_string("type")), gem_string("end"));
    }
    GemVal _t1555;
    if (!gem_truthy(_t1552)) {
        _t1555 = _t1552;
    } else {
        GemVal _t1553 = (*gem_v_peek);
        GemVal _t1554 = _t1553.fn(_t1553.env, NULL, 0);
        _t1555 = gem_neq(gem_table_get(_t1554, gem_string("type")), gem_string("elif"));
    }
    GemVal _t1558;
    if (!gem_truthy(_t1555)) {
        _t1558 = _t1555;
    } else {
        GemVal _t1556 = (*gem_v_peek);
        GemVal _t1557 = _t1556.fn(_t1556.env, NULL, 0);
        _t1558 = gem_neq(gem_table_get(_t1557, gem_string("type")), gem_string("else"));
    }
    GemVal _t1561;
    if (!gem_truthy(_t1558)) {
        _t1561 = _t1558;
    } else {
        GemVal _t1559 = (*gem_v_peek);
        GemVal _t1560 = _t1559.fn(_t1559.env, NULL, 0);
        _t1561 = gem_neq(gem_table_get(_t1560, gem_string("type")), gem_string("when"));
    }
        if (gem_truthy(_t1561)) {
#line 912 "compiler/main.gem"
    GemVal _t1562 = (*gem_v_parse_expr);
            gem_v_value = _t1562.fn(_t1562.env, NULL, 0);
        }
#line 914 "compiler/main.gem"
    GemVal _t1563[] = {gem_v_value, gem_v_line};
    GemVal _t1564 = (*gem_v__mod_parser_make_return);
        GemVal _t1565 = _t1564.fn(_t1564.env, _t1563, 2);
        gem_pop_frame();
        return _t1565;
    }
#line 918 "compiler/main.gem"
    GemVal _t1566 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1566, gem_string("type")), gem_string("break")))) {
#line 919 "compiler/main.gem"
    GemVal _t1567 = (*gem_v_advance);
        (void)(_t1567.fn(_t1567.env, NULL, 0));
#line 920 "compiler/main.gem"
    GemVal _t1568 = (*gem_v__mod_parser_make_break);
        GemVal _t1569 = _t1568.fn(_t1568.env, NULL, 0);
        gem_pop_frame();
        return _t1569;
    }
#line 924 "compiler/main.gem"
    GemVal _t1570 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1570, gem_string("type")), gem_string("continue")))) {
#line 925 "compiler/main.gem"
    GemVal _t1571 = (*gem_v_advance);
        (void)(_t1571.fn(_t1571.env, NULL, 0));
#line 926 "compiler/main.gem"
    GemVal _t1572 = (*gem_v__mod_parser_make_continue);
        GemVal _t1573 = _t1572.fn(_t1572.env, NULL, 0);
        gem_pop_frame();
        return _t1573;
    }
#line 930 "compiler/main.gem"
    GemVal _t1574 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1574, gem_string("type")), gem_string("load")))) {
#line 931 "compiler/main.gem"
    GemVal _t1575 = (*gem_v_advance);
        (void)(_t1575.fn(_t1575.env, NULL, 0));
#line 932 "compiler/main.gem"
    GemVal _t1576[] = {gem_string("STRING")};
    GemVal _t1577 = (*gem_v_expect);
    GemVal _t1578 = _t1577.fn(_t1577.env, _t1576, 1);
        GemVal gem_v_path = gem_table_get(_t1578, gem_string("value"));
#line 933 "compiler/main.gem"
        GemVal gem_v_alias_name = GEM_NIL;
#line 934 "compiler/main.gem"
        GemVal gem_v_selective = GEM_NIL;
#line 935 "compiler/main.gem"
    GemVal _t1579 = (*gem_v_peek);
    GemVal _t1580 = _t1579.fn(_t1579.env, NULL, 0);
    GemVal _t1583;
    if (!gem_truthy(gem_eq(gem_table_get(_t1580, gem_string("type")), gem_string("NAME")))) {
        _t1583 = gem_eq(gem_table_get(_t1580, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1581 = (*gem_v_peek);
        GemVal _t1582 = _t1581.fn(_t1581.env, NULL, 0);
        _t1583 = gem_eq(gem_table_get(_t1582, gem_string("value")), gem_string("as"));
    }
        if (gem_truthy(_t1583)) {
#line 936 "compiler/main.gem"
    GemVal _t1584 = (*gem_v_advance);
            (void)(_t1584.fn(_t1584.env, NULL, 0));
#line 937 "compiler/main.gem"
    GemVal _t1585[] = {gem_string("NAME")};
    GemVal _t1586 = (*gem_v_expect);
    GemVal _t1587 = _t1586.fn(_t1586.env, _t1585, 1);
            gem_v_alias_name = gem_table_get(_t1587, gem_string("value"));
        } else {
#line 938 "compiler/main.gem"
    GemVal _t1588 = (*gem_v_peek);
    GemVal _t1589 = _t1588.fn(_t1588.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1589, gem_string("type")), gem_string("(")))) {
#line 939 "compiler/main.gem"
    GemVal _t1590 = (*gem_v_advance);
                (void)(_t1590.fn(_t1590.env, NULL, 0));
#line 940 "compiler/main.gem"
    GemVal _t1591 = gem_table_new();
    GemVal _t1592[] = {gem_string("NAME")};
    GemVal _t1593 = (*gem_v_expect);
    GemVal _t1594 = _t1593.fn(_t1593.env, _t1592, 1);
    gem_table_set(_t1591, gem_int(0), gem_table_get(_t1594, gem_string("value")));
                gem_v_selective = _t1591;
#line 941 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1595 = (*gem_v_peek);
                    GemVal _t1596 = _t1595.fn(_t1595.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1596, gem_string("type")), gem_string(",")))) break;
#line 942 "compiler/main.gem"
    GemVal _t1597 = (*gem_v_advance);
                    (void)(_t1597.fn(_t1597.env, NULL, 0));
#line 943 "compiler/main.gem"
    GemVal _t1598[] = {gem_string("NAME")};
    GemVal _t1599 = (*gem_v_expect);
    GemVal _t1600 = _t1599.fn(_t1599.env, _t1598, 1);
    GemVal _t1601[] = {gem_v_selective, gem_table_get(_t1600, gem_string("value"))};
                    (void)(gem_push_fn(NULL, _t1601, 2));
                }
#line 945 "compiler/main.gem"
    GemVal _t1602[] = {gem_string(")")};
    GemVal _t1603 = (*gem_v_expect);
                (void)(_t1603.fn(_t1603.env, _t1602, 1));
            }
        }
#line 947 "compiler/main.gem"
    GemVal _t1604 = gem_table_new();
    gem_table_set(_t1604, gem_string("tag"), gem_string("load"));
    gem_table_set(_t1604, gem_string("path"), gem_v_path);
    gem_table_set(_t1604, gem_string("alias"), gem_v_alias_name);
    gem_table_set(_t1604, gem_string("selective"), gem_v_selective);
        GemVal _t1605 = _t1604;
        gem_pop_frame();
        return _t1605;
    }
#line 951 "compiler/main.gem"
    GemVal _t1606 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1606, gem_string("type")), gem_string("export")))) {
#line 952 "compiler/main.gem"
    GemVal _t1607 = (*gem_v_advance);
        (void)(_t1607.fn(_t1607.env, NULL, 0));
#line 953 "compiler/main.gem"
    GemVal _t1608 = gem_table_new();
    GemVal _t1609[] = {gem_string("NAME")};
    GemVal _t1610 = (*gem_v_expect);
    GemVal _t1611 = _t1610.fn(_t1610.env, _t1609, 1);
    gem_table_set(_t1608, gem_int(0), gem_table_get(_t1611, gem_string("value")));
        GemVal gem_v_names = _t1608;
#line 954 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1612 = (*gem_v_peek);
            GemVal _t1613 = _t1612.fn(_t1612.env, NULL, 0);
            if (!gem_truthy(gem_eq(gem_table_get(_t1613, gem_string("type")), gem_string(",")))) break;
#line 955 "compiler/main.gem"
    GemVal _t1614 = (*gem_v_advance);
            (void)(_t1614.fn(_t1614.env, NULL, 0));
#line 956 "compiler/main.gem"
    GemVal _t1615[] = {gem_string("NAME")};
    GemVal _t1616 = (*gem_v_expect);
    GemVal _t1617 = _t1616.fn(_t1616.env, _t1615, 1);
    GemVal _t1618[] = {gem_v_names, gem_table_get(_t1617, gem_string("value"))};
            (void)(gem_push_fn(NULL, _t1618, 2));
        }
#line 958 "compiler/main.gem"
    GemVal _t1619[] = {gem_v_names};
    GemVal _t1620 = (*gem_v__mod_parser_make_export);
        GemVal _t1621 = _t1620.fn(_t1620.env, _t1619, 1);
        gem_pop_frame();
        return _t1621;
    }
#line 962 "compiler/main.gem"
    GemVal _t1622 = gem_v_t;
    if (gem_truthy(gem_eq(gem_table_get(_t1622, gem_string("type")), gem_string("extern")))) {
#line 963 "compiler/main.gem"
    GemVal _t1623 = (*gem_v_advance);
        (void)(_t1623.fn(_t1623.env, NULL, 0));
#line 964 "compiler/main.gem"
        GemVal gem_v_is_blocking = gem_bool(0);
#line 965 "compiler/main.gem"
    GemVal _t1624 = (*gem_v_peek);
    GemVal _t1625 = _t1624.fn(_t1624.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1625, gem_string("type")), gem_string("blocking")))) {
#line 966 "compiler/main.gem"
            gem_v_is_blocking = gem_bool(1);
#line 967 "compiler/main.gem"
    GemVal _t1626 = (*gem_v_advance);
            (void)(_t1626.fn(_t1626.env, NULL, 0));
        }
#line 969 "compiler/main.gem"
    GemVal _t1627 = (*gem_v_peek);
    GemVal _t1628 = _t1627.fn(_t1627.env, NULL, 0);
        if (gem_truthy(gem_eq(gem_table_get(_t1628, gem_string("type")), gem_string("fn")))) {
#line 970 "compiler/main.gem"
    GemVal _t1629 = (*gem_v_advance);
            (void)(_t1629.fn(_t1629.env, NULL, 0));
#line 971 "compiler/main.gem"
    GemVal _t1630[] = {gem_string("NAME")};
    GemVal _t1631 = (*gem_v_expect);
    GemVal _t1632 = _t1631.fn(_t1631.env, _t1630, 1);
            GemVal gem_v_name = gem_table_get(_t1632, gem_string("value"));
#line 972 "compiler/main.gem"
    GemVal _t1633[] = {gem_string("(")};
    GemVal _t1634 = (*gem_v_expect);
            (void)(_t1634.fn(_t1634.env, _t1633, 1));
#line 973 "compiler/main.gem"
    GemVal _t1635 = gem_table_new();
            GemVal gem_v_eparams = _t1635;
#line 974 "compiler/main.gem"
    GemVal _t1636 = (*gem_v_peek);
    GemVal _t1637 = _t1636.fn(_t1636.env, NULL, 0);
            if (gem_truthy(gem_neq(gem_table_get(_t1637, gem_string("type")), gem_string(")")))) {
#line 975 "compiler/main.gem"
    GemVal _t1638[] = {gem_string("NAME")};
    GemVal _t1639 = (*gem_v_expect);
    GemVal _t1640 = _t1639.fn(_t1639.env, _t1638, 1);
                GemVal gem_v_pname = gem_table_get(_t1640, gem_string("value"));
#line 976 "compiler/main.gem"
    GemVal _t1641[] = {gem_string(":")};
    GemVal _t1642 = (*gem_v_expect);
                (void)(_t1642.fn(_t1642.env, _t1641, 1));
#line 977 "compiler/main.gem"
    GemVal _t1643[] = {gem_string("NAME")};
    GemVal _t1644 = (*gem_v_expect);
    GemVal _t1645 = _t1644.fn(_t1644.env, _t1643, 1);
                GemVal gem_v_ptype = gem_table_get(_t1645, gem_string("value"));
#line 978 "compiler/main.gem"
    GemVal _t1646[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1647 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1648[] = {gem_v_eparams, _t1647.fn(_t1647.env, _t1646, 2)};
                (void)(gem_push_fn(NULL, _t1648, 2));
#line 979 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1649 = (*gem_v_peek);
                    GemVal _t1650 = _t1649.fn(_t1649.env, NULL, 0);
                    if (!gem_truthy(gem_eq(gem_table_get(_t1650, gem_string("type")), gem_string(",")))) break;
#line 980 "compiler/main.gem"
    GemVal _t1651 = (*gem_v_advance);
                    (void)(_t1651.fn(_t1651.env, NULL, 0));
#line 981 "compiler/main.gem"
    GemVal _t1652[] = {gem_string("NAME")};
    GemVal _t1653 = (*gem_v_expect);
    GemVal _t1654 = _t1653.fn(_t1653.env, _t1652, 1);
                    GemVal gem_v_pn2 = gem_table_get(_t1654, gem_string("value"));
#line 982 "compiler/main.gem"
    GemVal _t1655[] = {gem_string(":")};
    GemVal _t1656 = (*gem_v_expect);
                    (void)(_t1656.fn(_t1656.env, _t1655, 1));
#line 983 "compiler/main.gem"
    GemVal _t1657[] = {gem_string("NAME")};
    GemVal _t1658 = (*gem_v_expect);
    GemVal _t1659 = _t1658.fn(_t1658.env, _t1657, 1);
                    GemVal gem_v_pt2 = gem_table_get(_t1659, gem_string("value"));
#line 984 "compiler/main.gem"
    GemVal _t1660[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1661 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1662[] = {gem_v_eparams, _t1661.fn(_t1661.env, _t1660, 2)};
                    (void)(gem_push_fn(NULL, _t1662, 2));
                }
            }
#line 987 "compiler/main.gem"
    GemVal _t1663[] = {gem_string(")")};
    GemVal _t1664 = (*gem_v_expect);
            (void)(_t1664.fn(_t1664.env, _t1663, 1));
#line 988 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 989 "compiler/main.gem"
    GemVal _t1665 = (*gem_v_peek);
    GemVal _t1666 = _t1665.fn(_t1665.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1666, gem_string("type")), gem_string("->")))) {
#line 990 "compiler/main.gem"
    GemVal _t1667 = (*gem_v_advance);
                (void)(_t1667.fn(_t1667.env, NULL, 0));
#line 991 "compiler/main.gem"
    GemVal _t1668[] = {gem_string("NAME")};
    GemVal _t1669 = (*gem_v_expect);
    GemVal _t1670 = _t1669.fn(_t1669.env, _t1668, 1);
                gem_v_ret_type = gem_table_get(_t1670, gem_string("value"));
            }
#line 993 "compiler/main.gem"
    GemVal _t1671[] = {gem_v_name, gem_v_eparams, gem_v_ret_type, gem_v_is_blocking};
    GemVal _t1672 = (*gem_v__mod_parser_make_extern_fn);
            GemVal _t1673 = _t1672.fn(_t1672.env, _t1671, 4);
            gem_pop_frame();
            return _t1673;
        } else {
#line 994 "compiler/main.gem"
    GemVal _t1676;
    if (!gem_truthy(gem_not(gem_v_is_blocking))) {
        _t1676 = gem_not(gem_v_is_blocking);
    } else {
        GemVal _t1674 = (*gem_v_peek);
        GemVal _t1675 = _t1674.fn(_t1674.env, NULL, 0);
        _t1676 = gem_eq(gem_table_get(_t1675, gem_string("type")), gem_string("NAME"));
    }
    GemVal _t1679;
    if (!gem_truthy(_t1676)) {
        _t1679 = _t1676;
    } else {
        GemVal _t1677 = (*gem_v_peek);
        GemVal _t1678 = _t1677.fn(_t1677.env, NULL, 0);
        _t1679 = gem_eq(gem_table_get(_t1678, gem_string("value")), gem_string("include"));
    }
            if (gem_truthy(_t1679)) {
#line 995 "compiler/main.gem"
    GemVal _t1680 = (*gem_v_advance);
                (void)(_t1680.fn(_t1680.env, NULL, 0));
#line 996 "compiler/main.gem"
    GemVal _t1681[] = {gem_string("STRING")};
    GemVal _t1682 = (*gem_v_expect);
    GemVal _t1683 = _t1682.fn(_t1682.env, _t1681, 1);
                GemVal gem_v_path = gem_table_get(_t1683, gem_string("value"));
#line 997 "compiler/main.gem"
    GemVal _t1684[] = {gem_v_path};
    GemVal _t1685 = (*gem_v__mod_parser_make_extern_include);
                GemVal _t1686 = _t1685.fn(_t1685.env, _t1684, 1);
                gem_pop_frame();
                return _t1686;
            } else {
#line 999 "compiler/main.gem"
    GemVal _t1687 = (*gem_v_peek);
    GemVal _t1688 = _t1687.fn(_t1687.env, NULL, 0);
    GemVal _t1689 = (*gem_v_peek);
    GemVal _t1690 = _t1689.fn(_t1689.env, NULL, 0);
    GemVal _t1691 = (*gem_v_peek);
    GemVal _t1692 = _t1691.fn(_t1691.env, NULL, 0);
    GemVal _t1693[] = {gem_table_get(_t1692, gem_string("value"))};
    GemVal _t1694[] = {(*gem_v_source), (*gem_v_file), gem_table_get(_t1688, gem_string("line")), gem_table_get(_t1690, gem_string("col")), gem_len_fn(NULL, _t1693, 1), gem_string("expected 'fn' after 'extern blocking'"), GEM_NIL};
    GemVal _t1695 = (*gem_v__mod_parser_compile_error);
                (void)(_t1695.fn(_t1695.env, _t1694, 7));
            }
        }
    }
#line 1004 "compiler/main.gem"
    GemVal _t1696 = gem_v_t;
    GemVal _t1698;
    if (!gem_truthy(gem_eq(gem_table_get(_t1696, gem_string("type")), gem_string("NAME")))) {
        _t1698 = gem_eq(gem_table_get(_t1696, gem_string("type")), gem_string("NAME"));
    } else {
        GemVal _t1697 = gem_v_t;
        _t1698 = gem_eq(gem_table_get(_t1697, gem_string("value")), gem_string("receive"));
    }
    if (gem_truthy(_t1698)) {
#line 1005 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 1006 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1699[] = {gem_v_rla};
            GemVal _t1700 = (*gem_v_peek_at);
            GemVal _t1701 = _t1700.fn(_t1700.env, _t1699, 1);
            if (!gem_truthy(gem_eq(gem_table_get(_t1701, gem_string("type")), gem_string("NEWLINE")))) break;
#line 1007 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 1009 "compiler/main.gem"
    GemVal _t1702[] = {gem_v_rla};
    GemVal _t1703 = (*gem_v_peek_at);
    GemVal _t1704 = _t1703.fn(_t1703.env, _t1702, 1);
        if (gem_truthy(gem_eq(gem_table_get(_t1704, gem_string("type")), gem_string("when")))) {
#line 1010 "compiler/main.gem"
    GemVal _t1705 = gem_v_t;
            GemVal gem_v_rline = gem_table_get(_t1705, gem_string("line"));
#line 1011 "compiler/main.gem"
    GemVal _t1706 = (*gem_v_advance);
            (void)(_t1706.fn(_t1706.env, NULL, 0));
#line 1012 "compiler/main.gem"
    GemVal _t1707 = (*gem_v_skip_nl);
            (void)(_t1707.fn(_t1707.env, NULL, 0));
#line 1013 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 1014 "compiler/main.gem"
    GemVal _t1708[] = {(*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_add(gem_string("_recv_"), gem_to_string_fn(NULL, _t1708, 1));
#line 1015 "compiler/main.gem"
    GemVal _t1709[] = {gem_v_recv_var};
    GemVal _t1710 = (*gem_v__mod_parser_make_var);
            GemVal gem_v_recv_var_expr = _t1710.fn(_t1710.env, _t1709, 1);
#line 1016 "compiler/main.gem"
    GemVal _t1711 = gem_table_new();
            GemVal gem_v_rarms = _t1711;
#line 1017 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1712 = (*gem_v_peek);
                GemVal _t1713 = _t1712.fn(_t1712.env, NULL, 0);
                if (!gem_truthy(gem_eq(gem_table_get(_t1713, gem_string("type")), gem_string("when")))) break;
#line 1018 "compiler/main.gem"
    GemVal _t1714 = (*gem_v_advance);
                (void)(_t1714.fn(_t1714.env, NULL, 0));
#line 1019 "compiler/main.gem"
    GemVal _t1715 = (*gem_v_peek);
    GemVal _t1716 = _t1715.fn(_t1715.env, NULL, 0);
                GemVal gem_v_rpt = gem_table_get(_t1716, gem_string("type"));
#line 1020 "compiler/main.gem"
    GemVal _t1717[] = {gem_int(1)};
    GemVal _t1718 = (*gem_v_peek_at);
    GemVal _t1719 = _t1718.fn(_t1718.env, _t1717, 1);
                GemVal gem_v_rnext = gem_table_get(_t1719, gem_string("type"));
#line 1021 "compiler/main.gem"
    GemVal _t1720;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1720 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1720 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1720)) {
#line 1022 "compiler/main.gem"
    GemVal _t1721[] = {gem_v_recv_var_expr};
    GemVal _t1722 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1722.fn(_t1722.env, _t1721, 1);
#line 1023 "compiler/main.gem"
    GemVal _t1723 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1723.fn(_t1723.env, NULL, 0);
#line 1024 "compiler/main.gem"
    GemVal _t1724 = gem_table_new();
    gem_table_set(_t1724, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1724, gem_string("body"), gem_v_rwbody);
    GemVal _t1725[] = {gem_v_rarms, _t1724};
                    (void)(gem_push_fn(NULL, _t1725, 2));
                } else {
#line 1025 "compiler/main.gem"
    GemVal _t1731;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1731 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1726;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1726 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1726 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1727;
        if (gem_truthy(_t1726)) {
                _t1727 = _t1726;
        } else {
                _t1727 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1728;
        if (gem_truthy(_t1727)) {
                _t1728 = _t1727;
        } else {
                _t1728 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1729;
        if (gem_truthy(_t1728)) {
                _t1729 = _t1728;
        } else {
                _t1729 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1730;
        if (gem_truthy(_t1729)) {
                _t1730 = _t1729;
        } else {
                _t1730 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1731 = _t1730;
    }
                    if (gem_truthy(_t1731)) {
#line 1026 "compiler/main.gem"
    GemVal _t1732[] = {gem_v_recv_var_expr};
    GemVal _t1733 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1733.fn(_t1733.env, _t1732, 1);
#line 1027 "compiler/main.gem"
    GemVal _t1734 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1734.fn(_t1734.env, NULL, 0);
#line 1028 "compiler/main.gem"
    GemVal _t1735 = gem_table_new();
    gem_table_set(_t1735, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1735, gem_string("body"), gem_v_rwbody);
    GemVal _t1736[] = {gem_v_rarms, _t1735};
                        (void)(gem_push_fn(NULL, _t1736, 2));
                    } else {
#line 1030 "compiler/main.gem"
    GemVal _t1737 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1737.fn(_t1737.env, NULL, 0);
#line 1031 "compiler/main.gem"
    GemVal _t1738 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1738.fn(_t1738.env, NULL, 0);
#line 1032 "compiler/main.gem"
    GemVal _t1739 = gem_table_new();
    GemVal _t1740 = gem_table_new();
    GemVal _t1741[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    GemVal _t1742 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1740, gem_string("condition"), _t1742.fn(_t1742.env, _t1741, 3));
    GemVal _t1743 = gem_table_new();
    gem_table_set(_t1740, gem_string("bindings"), _t1743);
    gem_table_set(_t1739, gem_string("pattern"), _t1740);
    gem_table_set(_t1739, gem_string("body"), gem_v_rwbody);
    GemVal _t1744[] = {gem_v_rarms, _t1739};
                        (void)(gem_push_fn(NULL, _t1744, 2));
                    }
                }
#line 1034 "compiler/main.gem"
    GemVal _t1745 = (*gem_v_skip_nl);
                (void)(_t1745.fn(_t1745.env, NULL, 0));
            }
#line 1036 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 1037 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 1038 "compiler/main.gem"
    GemVal _t1746 = (*gem_v_peek);
    GemVal _t1747 = _t1746.fn(_t1746.env, NULL, 0);
            if (gem_truthy(gem_eq(gem_table_get(_t1747, gem_string("type")), gem_string("after")))) {
#line 1039 "compiler/main.gem"
    GemVal _t1748 = (*gem_v_advance);
                (void)(_t1748.fn(_t1748.env, NULL, 0));
#line 1040 "compiler/main.gem"
    GemVal _t1749 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1749.fn(_t1749.env, NULL, 0);
#line 1041 "compiler/main.gem"
    GemVal _t1750 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1750.fn(_t1750.env, NULL, 0);
            }
#line 1043 "compiler/main.gem"
    GemVal _t1751[] = {gem_string("end")};
    GemVal _t1752 = (*gem_v_expect);
            (void)(_t1752.fn(_t1752.env, _t1751, 1));
#line 1044 "compiler/main.gem"
    GemVal _t1753[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
    GemVal _t1754 = (*gem_v__mod_parser_make_receive_match);
            GemVal _t1755 = _t1754.fn(_t1754.env, _t1753, 5);
            gem_pop_frame();
            return _t1755;
        }
    }
#line 1049 "compiler/main.gem"
    GemVal _t1756 = (*gem_v_parse_expr);
    GemVal _t1757 = _t1756.fn(_t1756.env, NULL, 0);
    gem_pop_frame();
    return _t1757;
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
    GemVal _t1759 = gem_table_new();
    GemVal gem_v_stmts = _t1759;
#line 1059 "compiler/main.gem"
    GemVal _t1760 = (*gem_v_skip_nl);
    (void)(_t1760.fn(_t1760.env, NULL, 0));
#line 1060 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1761 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1761.fn(_t1761.env, NULL, 0)))) break;
#line 1061 "compiler/main.gem"
    GemVal _t1762 = (*gem_v_parse_stmt);
    GemVal _t1763[] = {gem_v_stmts, _t1762.fn(_t1762.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1763, 2));
#line 1062 "compiler/main.gem"
    GemVal _t1764 = (*gem_v_skip_nl);
        (void)(_t1764.fn(_t1764.env, NULL, 0));
    }
#line 1064 "compiler/main.gem"
    GemVal _t1765[] = {gem_v_stmts};
    GemVal _t1766 = (*gem_v__mod_parser_make_program);
    GemVal _t1767 = _t1766.fn(_t1766.env, _t1765, 1);
    gem_pop_frame();
    return _t1767;
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
#line 111 "compiler/main.gem"
    struct _closure__anon_7 *_t479 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t479->gem_v_advance = gem_v_advance;
    _t479->gem_v_expect = gem_v_expect;
    _t479->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_7, _t479);
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
    struct _closure__anon_8 *_t505 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t505->gem_v_at_end = gem_v_at_end;
    _t505->gem_v_parse_stmt = gem_v_parse_stmt;
    _t505->gem_v_peek = gem_v_peek;
    _t505->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_8, _t505);
#line 156 "compiler/main.gem"
    struct _closure__anon_9 *_t516 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t516->gem_v_at_end = gem_v_at_end;
    _t516->gem_v_parse_stmt = gem_v_parse_stmt;
    _t516->gem_v_peek = gem_v_peek;
    _t516->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_9, _t516);
#line 169 "compiler/main.gem"
    struct _closure__anon_10 *_t688 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t688->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t688->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t688->gem_v__mod_parser_make_array = &gem_v__mod_parser_make_array;
    _t688->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t688->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t688->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t688->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t688->gem_v__mod_parser_make_interp = &gem_v__mod_parser_make_interp;
    _t688->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t688->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t688->gem_v__mod_parser_make_table = &gem_v__mod_parser_make_table;
    _t688->gem_v__mod_parser_make_table_entry = &gem_v__mod_parser_make_table_entry;
    _t688->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t688->gem_v_advance = gem_v_advance;
    _t688->gem_v_at_end = gem_v_at_end;
    _t688->gem_v_expect = gem_v_expect;
    _t688->gem_v_file = gem_v_file;
    _t688->gem_v_fn_depth = gem_v_fn_depth;
    _t688->gem_v_parse_expr = gem_v_parse_expr;
    _t688->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t688->gem_v_parse_params = gem_v_parse_params;
    _t688->gem_v_peek = gem_v_peek;
    _t688->gem_v_peek_at = gem_v_peek_at;
    _t688->gem_v_skip_nl = gem_v_skip_nl;
    _t688->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_10, _t688);
#line 307 "compiler/main.gem"
    struct _closure__anon_11 *_t782 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t782->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t782->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t782->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t782->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t782->gem_v_advance = gem_v_advance;
    _t782->gem_v_expect = gem_v_expect;
    _t782->gem_v_fn_depth = gem_v_fn_depth;
    _t782->gem_v_parse_atom = gem_v_parse_atom;
    _t782->gem_v_parse_expr = gem_v_parse_expr;
    _t782->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t782->gem_v_parse_params = gem_v_parse_params;
    _t782->gem_v_peek = gem_v_peek;
    _t782->gem_v_peek_at = gem_v_peek_at;
    _t782->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_11, _t782);
#line 409 "compiler/main.gem"
    struct _closure__anon_12 *_t792 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t792->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t792->gem_v_advance = gem_v_advance;
    _t792->gem_v_parse_call = gem_v_parse_call;
    _t792->gem_v_parse_unary = gem_v_parse_unary;
    _t792->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_12, _t792);
#line 418 "compiler/main.gem"
    struct _closure__anon_13 *_t808 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t808->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t808->gem_v_advance = gem_v_advance;
    _t808->gem_v_parse_unary = gem_v_parse_unary;
    _t808->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_13, _t808);
#line 429 "compiler/main.gem"
    struct _closure__anon_14 *_t821 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t821->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t821->gem_v_advance = gem_v_advance;
    _t821->gem_v_parse_mul = gem_v_parse_mul;
    _t821->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_14, _t821);
#line 440 "compiler/main.gem"
    struct _closure__anon_15 *_t849 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t849->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t849->gem_v_advance = gem_v_advance;
    _t849->gem_v_parse_add = gem_v_parse_add;
    _t849->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_15, _t849);
#line 451 "compiler/main.gem"
    struct _closure__anon_16 *_t859 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t859->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t859->gem_v_advance = gem_v_advance;
    _t859->gem_v_parse_compare = gem_v_parse_compare;
    _t859->gem_v_parse_not = gem_v_parse_not;
    _t859->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_16, _t859);
#line 460 "compiler/main.gem"
    struct _closure__anon_17 *_t868 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t868->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t868->gem_v_advance = gem_v_advance;
    _t868->gem_v_parse_not = gem_v_parse_not;
    _t868->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_17, _t868);
#line 471 "compiler/main.gem"
    struct _closure__anon_18 *_t877 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t877->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t877->gem_v_advance = gem_v_advance;
    _t877->gem_v_parse_and = gem_v_parse_and;
    _t877->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_18, _t877);
#line 482 "compiler/main.gem"
    struct _closure__anon_19 *_t935 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t935->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t935->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t935->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t935->gem_v__mod_parser_make_dot_assign = &gem_v__mod_parser_make_dot_assign;
    _t935->gem_v__mod_parser_make_index_assign = &gem_v__mod_parser_make_index_assign;
    _t935->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t935->gem_v_advance = gem_v_advance;
    _t935->gem_v_file = gem_v_file;
    _t935->gem_v_parse_expr = gem_v_parse_expr;
    _t935->gem_v_parse_or = gem_v_parse_or;
    _t935->gem_v_peek = gem_v_peek;
    _t935->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_19, _t935);
#line 517 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 521 "compiler/main.gem"
    struct _closure__anon_20 *_t1144 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t1144->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1144->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1144->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t1144->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1144->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t1144->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1144->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1144->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1144->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t1144->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t1144->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1144->gem_v_advance = gem_v_advance;
    _t1144->gem_v_expect = gem_v_expect;
    _t1144->gem_v_file = gem_v_file;
    _t1144->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1144->gem_v_peek = gem_v_peek;
    _t1144->gem_v_peek_at = gem_v_peek_at;
    _t1144->gem_v_skip_nl = gem_v_skip_nl;
    _t1144->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_20, _t1144);
#line 649 "compiler/main.gem"
    struct _closure__anon_22 *_t1758 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t1758->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1758->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t1758->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1758->gem_v__mod_parser_make_break = &gem_v__mod_parser_make_break;
    _t1758->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1758->gem_v__mod_parser_make_continue = &gem_v__mod_parser_make_continue;
    _t1758->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t1758->gem_v__mod_parser_make_export = &gem_v__mod_parser_make_export;
    _t1758->gem_v__mod_parser_make_extern_fn = &gem_v__mod_parser_make_extern_fn;
    _t1758->gem_v__mod_parser_make_extern_include = &gem_v__mod_parser_make_extern_include;
    _t1758->gem_v__mod_parser_make_extern_param = &gem_v__mod_parser_make_extern_param;
    _t1758->gem_v__mod_parser_make_fn_def = &gem_v__mod_parser_make_fn_def;
    _t1758->gem_v__mod_parser_make_if = &gem_v__mod_parser_make_if;
    _t1758->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1758->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1758->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1758->gem_v__mod_parser_make_match = &gem_v__mod_parser_make_match;
    _t1758->gem_v__mod_parser_make_receive_match = &gem_v__mod_parser_make_receive_match;
    _t1758->gem_v__mod_parser_make_return = &gem_v__mod_parser_make_return;
    _t1758->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1758->gem_v__mod_parser_make_when = &gem_v__mod_parser_make_when;
    _t1758->gem_v__mod_parser_make_while = &gem_v__mod_parser_make_while;
    _t1758->gem_v_advance = gem_v_advance;
    _t1758->gem_v_expect = gem_v_expect;
    _t1758->gem_v_file = gem_v_file;
    _t1758->gem_v_fn_depth = gem_v_fn_depth;
    _t1758->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1758->gem_v_parse_body = gem_v_parse_body;
    _t1758->gem_v_parse_expr = gem_v_parse_expr;
    _t1758->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1758->gem_v_parse_params = gem_v_parse_params;
    _t1758->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1758->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1758->gem_v_peek = gem_v_peek;
    _t1758->gem_v_peek_at = gem_v_peek_at;
    _t1758->gem_v_skip_nl = gem_v_skip_nl;
    _t1758->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_22, _t1758);
#line 1053 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 1057 "compiler/main.gem"
    struct _closure__anon_23 *_t1768 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t1768->gem_v__mod_parser_make_program = &gem_v__mod_parser_make_program;
    _t1768->gem_v_at_end = gem_v_at_end;
    _t1768->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1768->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_23, _t1768);
    GemVal _t1769 = gem_table_new();
    gem_table_set(_t1769, gem_string("parse"), gem_v_parse);
    GemVal _t1770 = _t1769;
    gem_pop_frame();
    return _t1770;
}

static GemVal gem_fn__mod_codegen_escape_c_string(void *_env, GemVal *args, int argc) {
#line 7 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_escape_c_string", "compiler/main.gem", 7);
#line 8 "compiler/main.gem"
    GemVal _t1771[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1771, 1);
#line 9 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 10 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 10 "compiler/main.gem"
    GemVal _t1772[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1772, 1);
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
    GemVal _t1773[] = {gem_v_b, gem_string("\\\\")};
            (void)(gem_buf_push_fn(NULL, _t1773, 2));
        } else {
#line 14 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 15 "compiler/main.gem"
    GemVal _t1774[] = {gem_v_b, gem_string("\\\"")};
                (void)(gem_buf_push_fn(NULL, _t1774, 2));
            } else {
#line 16 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 17 "compiler/main.gem"
    GemVal _t1775[] = {gem_v_b, gem_string("\\n")};
                    (void)(gem_buf_push_fn(NULL, _t1775, 2));
                } else {
#line 18 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 19 "compiler/main.gem"
    GemVal _t1776[] = {gem_v_b, gem_string("\\r")};
                        (void)(gem_buf_push_fn(NULL, _t1776, 2));
                    } else {
#line 20 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 21 "compiler/main.gem"
    GemVal _t1777[] = {gem_v_b, gem_string("\\t")};
                            (void)(gem_buf_push_fn(NULL, _t1777, 2));
                        } else {
#line 22 "compiler/main.gem"
    GemVal _t1778[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1778, 1), gem_int(0)))) {
#line 23 "compiler/main.gem"
    GemVal _t1779[] = {gem_v_b, gem_string("\\0")};
                                (void)(gem_buf_push_fn(NULL, _t1779, 2));
                            } else {
#line 25 "compiler/main.gem"
    GemVal _t1780[] = {gem_v_b, gem_v_ch};
                                (void)(gem_buf_push_fn(NULL, _t1780, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 28 "compiler/main.gem"
    GemVal _t1781[] = {gem_v_b};
    GemVal _t1782 = gem_buf_str_fn(NULL, _t1781, 1);
    gem_pop_frame();
    return _t1782;
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
    GemVal _t1783[] = {gem_v_b, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t1783, 2));
    }

#line 36 "compiler/main.gem"
    GemVal _t1784[] = {gem_v_b};
    GemVal _t1785 = gem_buf_str_fn(NULL, _t1784, 1);
    gem_pop_frame();
    return _t1785;
}

static GemVal gem_fn__mod_codegen_pad_for(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_pad_for", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal _t1786[] = {gem_string("    "), gem_v_indent};
    GemVal _t1787 = gem_fn__mod_codegen_repeat_str(NULL, _t1786, 2);
    gem_pop_frame();
    return _t1787;
}

static GemVal gem_fn__mod_codegen_format_float(void *_env, GemVal *args, int argc) {
#line 43 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_format_float", "compiler/main.gem", 43);
#line 44 "compiler/main.gem"
    GemVal _t1788[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1788, 1);
#line 46 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 47 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 47 "compiler/main.gem"
    GemVal _t1789[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1789, 1);
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
    GemVal _t1790 = gem_v_s;
    gem_pop_frame();
    return _t1790;
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
    GemVal _t1791 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1791;
}

static GemVal gem_fn__mod_codegen_set_union(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_union", "compiler/main.gem", 70);
#line 71 "compiler/main.gem"
    GemVal _t1792 = gem_table_new();
    GemVal gem_v_result = _t1792;
#line 72 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 72 "compiler/main.gem"
    GemVal _t1793[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_len_4 = gem_len_fn(NULL, _t1793, 1);
#line 72 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 72 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_len_4))) break;
#line 72 "compiler/main.gem"
    GemVal _t1794[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1794, 2);
#line 72 "compiler/main.gem"
    GemVal _t1795[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1795, 2);
#line 72 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 73 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

#line 75 "compiler/main.gem"
    GemVal gem_v__for_tbl_5 = gem_v_b;
#line 75 "compiler/main.gem"
    GemVal _t1796[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_len_5 = gem_len_fn(NULL, _t1796, 1);
#line 75 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 75 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_len_5))) break;
#line 75 "compiler/main.gem"
    GemVal _t1797[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1797, 2);
#line 75 "compiler/main.gem"
    GemVal _t1798[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1798, 2);
#line 75 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 76 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1799 = gem_v_result;
    gem_pop_frame();
    return _t1799;
}

static GemVal gem_fn__mod_codegen_set_intersect(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_intersect", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t1800 = gem_table_new();
    GemVal gem_v_result = _t1800;
#line 83 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 83 "compiler/main.gem"
    GemVal _t1801[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_len_6 = gem_len_fn(NULL, _t1801, 1);
#line 83 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 83 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_len_6))) break;
#line 83 "compiler/main.gem"
    GemVal _t1802[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1802, 2);
#line 83 "compiler/main.gem"
    GemVal _t1803[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1803, 2);
#line 83 "compiler/main.gem"
        gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 84 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_v_k), gem_bool(1)))) {
#line 85 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
        }
    }

    GemVal _t1804 = gem_v_result;
    gem_pop_frame();
    return _t1804;
}

static GemVal gem_fn__mod_codegen_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 91 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_sorted_array_to_set", "compiler/main.gem", 91);
#line 92 "compiler/main.gem"
    GemVal _t1805 = gem_table_new();
    GemVal gem_v_s = _t1805;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1806[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1806, 1)))) break;
#line 93 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 93 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 94 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1807 = gem_v_s;
    gem_pop_frame();
    return _t1807;
}

static GemVal gem_fn__mod_codegen_is_node(void *_env, GemVal *args, int argc) {
#line 99 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_is_node", "compiler/main.gem", 99);
    GemVal _t1808[] = {gem_v_node};
    GemVal _t1810;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1808, 1), gem_string("table")))) {
        _t1810 = gem_eq(gem_type_fn(NULL, _t1808, 1), gem_string("table"));
    } else {
        GemVal _t1809 = gem_v_node;
        _t1810 = gem_eq(gem_table_get(_t1809, gem_string("tag")), gem_v_tag);
    }
    GemVal _t1811 = _t1810;
    gem_pop_frame();
    return _t1811;
}

static GemVal gem_fn__mod_codegen_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_collect_block_lets", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t1812[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1812, 2))) {
        {
#line 107 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal _t1813 = gem_v_stmt;
    GemVal _t1814[] = {gem_table_get(_t1813, gem_string("stmts"))};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1814, 1);
#line 107 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 107 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 107 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 108 "compiler/main.gem"
    GemVal _t1815 = gem_v_stmt;
    GemVal _t1816[] = {gem_table_get(gem_table_get(_t1815, gem_string("stmts")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1816, 2))) {
#line 109 "compiler/main.gem"
    GemVal _t1817 = gem_v_stmt;
    GemVal _t1818 = gem_table_get(gem_table_get(_t1817, gem_string("stmts")), gem_v_j);
    GemVal _t1819[] = {gem_v_result, gem_table_get(_t1818, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1819, 2));
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
    GemVal _t1820[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1820, 1);
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
    GemVal _t1821[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1821, 2))) {
#line 119 "compiler/main.gem"
    GemVal _t1822 = gem_v_s;
    GemVal _t1823[] = {gem_v_result, gem_table_get(_t1822, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1823, 2));
            }
#line 121 "compiler/main.gem"
    GemVal _t1824[] = {gem_v_s, gem_v_result};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t1824, 2));
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
    GemVal _t1838[] = {(*gem_v_tmp_counter)};
    GemVal _t1839 = gem_add(gem_string("_t"), gem_to_string_fn(NULL, _t1838, 1));
    gem_pop_frame();
    return _t1839;
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
    GemVal _t1841[] = {(*gem_v_anon_counter)};
    GemVal _t1842 = gem_add(gem_string("_anon_"), gem_to_string_fn(NULL, _t1841, 1));
    gem_pop_frame();
    return _t1842;
}

struct _closure__anon_26 {
};
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
    GemVal _t1844[] = {gem_v_name};
    GemVal _t1845 = gem_add(gem_string("gem_v_"), gem_to_string_fn(NULL, _t1844, 1));
    gem_pop_frame();
    return _t1845;
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
        GemVal _t1846 = GEM_NIL;
        gem_pop_frame();
        return _t1846;
    }
#line 252 "compiler/main.gem"
    GemVal _t1847[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1847, 1), gem_string("table")))) {
#line 253 "compiler/main.gem"
        GemVal _t1848 = GEM_NIL;
        gem_pop_frame();
        return _t1848;
    }
#line 255 "compiler/main.gem"
    GemVal _t1849 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t1849, gem_string("tag")), GEM_NIL))) {
#line 256 "compiler/main.gem"
        GemVal _t1850 = GEM_NIL;
        gem_pop_frame();
        return _t1850;
    }
#line 259 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1851[] = {gem_v__match_11};
    GemVal _t1853;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1851, 1), gem_string("table")))) {
        _t1853 = gem_eq(gem_type_fn(NULL, _t1851, 1), gem_string("table"));
    } else {
        GemVal _t1852[] = {gem_v__match_11, gem_string("tag")};
        _t1853 = gem_has_key_fn(NULL, _t1852, 2);
    }
    GemVal _t1854;
    if (!gem_truthy(_t1853)) {
        _t1854 = _t1853;
    } else {
        _t1854 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1856;
    if (!gem_truthy(_t1854)) {
        _t1856 = _t1854;
    } else {
        GemVal _t1855[] = {gem_v__match_11, gem_string("name")};
        _t1856 = gem_has_key_fn(NULL, _t1855, 2);
    }
    if (gem_truthy(_t1856)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 261 "compiler/main.gem"
    GemVal _t1857[] = {gem_v_defined, gem_v_name};
    GemVal _t1861;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1857, 2)))) {
        _t1861 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1857, 2));
    } else {
        GemVal _t1858[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1860;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1858, 2)))) {
                _t1860 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1858, 2));
        } else {
                GemVal _t1859[] = {(*gem_v_local_names), gem_v_name};
                _t1860 = gem_fn__mod_codegen_set_contains(NULL, _t1859, 2);
        }
        _t1861 = _t1860;
    }
    GemVal _t1863;
    if (!gem_truthy(_t1861)) {
        _t1863 = _t1861;
    } else {
        GemVal _t1862[] = {(*gem_v_defined_fns), gem_v_name};
        _t1863 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1862, 2));
    }
        if (gem_truthy(_t1863)) {
#line 262 "compiler/main.gem"
    GemVal _t1864[] = {gem_v_free, gem_v_name};
            GemVal _t1865 = gem_fn__mod_codegen_set_add(NULL, _t1864, 2);
            gem_pop_frame();
            return _t1865;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1866[] = {gem_v__match_11};
    GemVal _t1868;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1866, 1), gem_string("table")))) {
        _t1868 = gem_eq(gem_type_fn(NULL, _t1866, 1), gem_string("table"));
    } else {
        GemVal _t1867[] = {gem_v__match_11, gem_string("tag")};
        _t1868 = gem_has_key_fn(NULL, _t1867, 2);
    }
    GemVal _t1869;
    if (!gem_truthy(_t1868)) {
        _t1869 = _t1868;
    } else {
        _t1869 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
    }
    GemVal _t1871;
    if (!gem_truthy(_t1869)) {
        _t1871 = _t1869;
    } else {
        GemVal _t1870[] = {gem_v__match_11, gem_string("value")};
        _t1871 = gem_has_key_fn(NULL, _t1870, 2);
    }
    if (gem_truthy(_t1871)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 265 "compiler/main.gem"
    GemVal _t1872[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1873 = (*gem_v_collect_free_node);
        GemVal _t1874 = _t1873.fn(_t1873.env, _t1872, 3);
        gem_pop_frame();
        return _t1874;
    } else {
    GemVal _t1875[] = {gem_v__match_11};
    GemVal _t1877;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1875, 1), gem_string("table")))) {
        _t1877 = gem_eq(gem_type_fn(NULL, _t1875, 1), gem_string("table"));
    } else {
        GemVal _t1876[] = {gem_v__match_11, gem_string("tag")};
        _t1877 = gem_has_key_fn(NULL, _t1876, 2);
    }
    GemVal _t1878;
    if (!gem_truthy(_t1877)) {
        _t1878 = _t1877;
    } else {
        _t1878 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1880;
    if (!gem_truthy(_t1878)) {
        _t1880 = _t1878;
    } else {
        GemVal _t1879[] = {gem_v__match_11, gem_string("name")};
        _t1880 = gem_has_key_fn(NULL, _t1879, 2);
    }
    GemVal _t1882;
    if (!gem_truthy(_t1880)) {
        _t1882 = _t1880;
    } else {
        GemVal _t1881[] = {gem_v__match_11, gem_string("value")};
        _t1882 = gem_has_key_fn(NULL, _t1881, 2);
    }
    if (gem_truthy(_t1882)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 267 "compiler/main.gem"
    GemVal _t1883[] = {gem_v_defined, gem_v_name};
    GemVal _t1887;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1883, 2)))) {
        _t1887 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1883, 2));
    } else {
        GemVal _t1884[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1886;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1884, 2)))) {
                _t1886 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1884, 2));
        } else {
                GemVal _t1885[] = {(*gem_v_local_names), gem_v_name};
                _t1886 = gem_fn__mod_codegen_set_contains(NULL, _t1885, 2);
        }
        _t1887 = _t1886;
    }
    GemVal _t1889;
    if (!gem_truthy(_t1887)) {
        _t1889 = _t1887;
    } else {
        GemVal _t1888[] = {(*gem_v_defined_fns), gem_v_name};
        _t1889 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1888, 2));
    }
        if (gem_truthy(_t1889)) {
#line 268 "compiler/main.gem"
    GemVal _t1890[] = {gem_v_free, gem_v_name};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1890, 2));
        }
#line 270 "compiler/main.gem"
    GemVal _t1891[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1892 = (*gem_v_collect_free_node);
        GemVal _t1893 = _t1892.fn(_t1892.env, _t1891, 3);
        gem_pop_frame();
        return _t1893;
    } else {
    GemVal _t1894[] = {gem_v__match_11};
    GemVal _t1896;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1894, 1), gem_string("table")))) {
        _t1896 = gem_eq(gem_type_fn(NULL, _t1894, 1), gem_string("table"));
    } else {
        GemVal _t1895[] = {gem_v__match_11, gem_string("tag")};
        _t1896 = gem_has_key_fn(NULL, _t1895, 2);
    }
    GemVal _t1897;
    if (!gem_truthy(_t1896)) {
        _t1897 = _t1896;
    } else {
        _t1897 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1899;
    if (!gem_truthy(_t1897)) {
        _t1899 = _t1897;
    } else {
        GemVal _t1898[] = {gem_v__match_11, gem_string("params")};
        _t1899 = gem_has_key_fn(NULL, _t1898, 2);
    }
    GemVal _t1901;
    if (!gem_truthy(_t1899)) {
        _t1901 = _t1899;
    } else {
        GemVal _t1900[] = {gem_v__match_11, gem_string("rest_param")};
        _t1901 = gem_has_key_fn(NULL, _t1900, 2);
    }
    GemVal _t1903;
    if (!gem_truthy(_t1901)) {
        _t1903 = _t1901;
    } else {
        GemVal _t1902[] = {gem_v__match_11, gem_string("body")};
        _t1903 = gem_has_key_fn(NULL, _t1902, 2);
    }
    if (gem_truthy(_t1903)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 272 "compiler/main.gem"
    GemVal _t1904[] = {gem_v_params};
    GemVal _t1905[] = {gem_v_defined, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t1904, 1)};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_set_union(NULL, _t1905, 2);
#line 273 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 274 "compiler/main.gem"
    GemVal _t1906[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1906, 2));
        }
#line 276 "compiler/main.gem"
    GemVal _t1907[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1908 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1908.fn(_t1908.env, _t1907, 2);
        {
#line 277 "compiler/main.gem"
            GemVal gem_v__for_tbl_12 = gem_v_inner_free;
#line 277 "compiler/main.gem"
    GemVal _t1909[] = {gem_v__for_tbl_12};
            GemVal gem_v__for_len_12 = gem_len_fn(NULL, _t1909, 1);
#line 277 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 277 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_len_12))) break;
#line 277 "compiler/main.gem"
    GemVal _t1910[] = {gem_v__for_tbl_12, gem_v__for_i_12};
                GemVal gem_v_ifk = gem_table_key_at_fn(NULL, _t1910, 2);
#line 277 "compiler/main.gem"
    GemVal _t1911[] = {gem_v__for_tbl_12, gem_v__for_i_12};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1911, 2);
#line 277 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 278 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1912, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1913[] = {gem_v__match_11};
    GemVal _t1915;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1913, 1), gem_string("table")))) {
        _t1915 = gem_eq(gem_type_fn(NULL, _t1913, 1), gem_string("table"));
    } else {
        GemVal _t1914[] = {gem_v__match_11, gem_string("tag")};
        _t1915 = gem_has_key_fn(NULL, _t1914, 2);
    }
    GemVal _t1916;
    if (!gem_truthy(_t1915)) {
        _t1916 = _t1915;
    } else {
        _t1916 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1916)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t1917[] = {gem_v__match_11};
    GemVal _t1919;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1917, 1), gem_string("table")))) {
        _t1919 = gem_eq(gem_type_fn(NULL, _t1917, 1), gem_string("table"));
    } else {
        GemVal _t1918[] = {gem_v__match_11, gem_string("tag")};
        _t1919 = gem_has_key_fn(NULL, _t1918, 2);
    }
    GemVal _t1920;
    if (!gem_truthy(_t1919)) {
        _t1920 = _t1919;
    } else {
        _t1920 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1922;
    if (!gem_truthy(_t1920)) {
        _t1922 = _t1920;
    } else {
        GemVal _t1921[] = {gem_v__match_11, gem_string("cond")};
        _t1922 = gem_has_key_fn(NULL, _t1921, 2);
    }
    GemVal _t1924;
    if (!gem_truthy(_t1922)) {
        _t1924 = _t1922;
    } else {
        GemVal _t1923[] = {gem_v__match_11, gem_string("then")};
        _t1924 = gem_has_key_fn(NULL, _t1923, 2);
    }
    GemVal _t1926;
    if (!gem_truthy(_t1924)) {
        _t1926 = _t1924;
    } else {
        GemVal _t1925[] = {gem_v__match_11, gem_string("else")};
        _t1926 = gem_has_key_fn(NULL, _t1925, 2);
    }
    if (gem_truthy(_t1926)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 283 "compiler/main.gem"
    GemVal _t1927[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1928 = (*gem_v_collect_free_node);
        (void)(_t1928.fn(_t1928.env, _t1927, 3));
#line 284 "compiler/main.gem"
    GemVal _t1929 = gem_table_new();
    GemVal _t1930[] = {gem_v_defined, _t1929};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1930, 2);
#line 285 "compiler/main.gem"
        GemVal gem_v__for_i_13 = gem_int(0);
#line 285 "compiler/main.gem"
    GemVal _t1931[] = {gem_v_thens};
        GemVal gem_v__for_limit_13 = gem_len_fn(NULL, _t1931, 1);
#line 285 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_limit_13))) break;
#line 285 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_13;
#line 285 "compiler/main.gem"
            gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 286 "compiler/main.gem"
    GemVal _t1932[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1933 = (*gem_v_collect_free_node);
            (void)(_t1933.fn(_t1933.env, _t1932, 3));
#line 287 "compiler/main.gem"
    GemVal _t1934[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1934, 2))) {
#line 288 "compiler/main.gem"
    GemVal _t1935 = gem_table_get(gem_v_thens, gem_v_i);
    GemVal _t1936[] = {gem_v_d, gem_table_get(_t1935, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1936, 2));
            }
        }

#line 291 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 292 "compiler/main.gem"
    GemVal _t1937 = gem_table_new();
    GemVal _t1938[] = {gem_v_defined, _t1937};
            gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1938, 2);
            {
#line 293 "compiler/main.gem"
                GemVal gem_v__for_i_14 = gem_int(0);
#line 293 "compiler/main.gem"
    GemVal _t1939[] = {gem_v_el};
                GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1939, 1);
#line 293 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 293 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_14;
#line 293 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 294 "compiler/main.gem"
    GemVal _t1940[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1941 = (*gem_v_collect_free_node);
                    (void)(_t1941.fn(_t1941.env, _t1940, 3));
#line 295 "compiler/main.gem"
    GemVal _t1942[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1942, 2))) {
#line 296 "compiler/main.gem"
    GemVal _t1943 = gem_table_get(gem_v_el, gem_v_i);
    GemVal _t1944[] = {gem_v_d, gem_table_get(_t1943, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1944, 2));
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
        _t1948 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1950;
    if (!gem_truthy(_t1948)) {
        _t1950 = _t1948;
    } else {
        GemVal _t1949[] = {gem_v__match_11, gem_string("cond")};
        _t1950 = gem_has_key_fn(NULL, _t1949, 2);
    }
    GemVal _t1952;
    if (!gem_truthy(_t1950)) {
        _t1952 = _t1950;
    } else {
        GemVal _t1951[] = {gem_v__match_11, gem_string("body")};
        _t1952 = gem_has_key_fn(NULL, _t1951, 2);
    }
    if (gem_truthy(_t1952)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 301 "compiler/main.gem"
    GemVal _t1953[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1954 = (*gem_v_collect_free_node);
        (void)(_t1954.fn(_t1954.env, _t1953, 3));
#line 302 "compiler/main.gem"
    GemVal _t1955 = gem_table_new();
    GemVal _t1956[] = {gem_v_defined, _t1955};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1956, 2);
        {
#line 303 "compiler/main.gem"
            GemVal gem_v__for_i_15 = gem_int(0);
#line 303 "compiler/main.gem"
    GemVal _t1957[] = {gem_v_body};
            GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1957, 1);
#line 303 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 303 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_15;
#line 303 "compiler/main.gem"
                gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 304 "compiler/main.gem"
    GemVal _t1958[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1959 = (*gem_v_collect_free_node);
                (void)(_t1959.fn(_t1959.env, _t1958, 3));
#line 305 "compiler/main.gem"
    GemVal _t1960[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1960, 2))) {
#line 306 "compiler/main.gem"
    GemVal _t1961 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t1962[] = {gem_v_d, gem_table_get(_t1961, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1962, 2));
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
        _t1966 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t1968;
    if (!gem_truthy(_t1966)) {
        _t1968 = _t1966;
    } else {
        GemVal _t1967[] = {gem_v__match_11, gem_string("target")};
        _t1968 = gem_has_key_fn(NULL, _t1967, 2);
    }
    GemVal _t1970;
    if (!gem_truthy(_t1968)) {
        _t1970 = _t1968;
    } else {
        GemVal _t1969[] = {gem_v__match_11, gem_string("target_var")};
        _t1970 = gem_has_key_fn(NULL, _t1969, 2);
    }
    GemVal _t1972;
    if (!gem_truthy(_t1970)) {
        _t1972 = _t1970;
    } else {
        GemVal _t1971[] = {gem_v__match_11, gem_string("whens")};
        _t1972 = gem_has_key_fn(NULL, _t1971, 2);
    }
    GemVal _t1974;
    if (!gem_truthy(_t1972)) {
        _t1974 = _t1972;
    } else {
        GemVal _t1973[] = {gem_v__match_11, gem_string("else")};
        _t1974 = gem_has_key_fn(NULL, _t1973, 2);
    }
    if (gem_truthy(_t1974)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 310 "compiler/main.gem"
    GemVal _t1975[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t1976 = (*gem_v_collect_free_node);
        (void)(_t1976.fn(_t1976.env, _t1975, 3));
#line 311 "compiler/main.gem"
    GemVal _t1977 = gem_table_new();
    GemVal _t1978[] = {gem_v_defined, _t1977};
        GemVal gem_v_match_def = gem_fn__mod_codegen_set_union(NULL, _t1978, 2);
#line 312 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 313 "compiler/main.gem"
    GemVal _t1979[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1979, 2));
        }
#line 315 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 315 "compiler/main.gem"
    GemVal _t1980[] = {gem_v_whens};
        GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t1980, 1);
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
    GemVal _t1981 = gem_v_w;
    GemVal _t1982[] = {gem_table_get(_t1981, gem_string("value")), gem_v_match_def, gem_v_free};
    GemVal _t1983 = (*gem_v_collect_free_node);
            (void)(_t1983.fn(_t1983.env, _t1982, 3));
#line 318 "compiler/main.gem"
    GemVal _t1984 = gem_table_new();
    GemVal _t1985[] = {gem_v_match_def, _t1984};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1985, 2);
#line 319 "compiler/main.gem"
    GemVal _t1986 = gem_v_w;
            if (gem_truthy(gem_neq(gem_table_get(_t1986, gem_string("bindings")), GEM_NIL))) {
#line 320 "compiler/main.gem"
                GemVal gem_v__for_i_16 = gem_int(0);
#line 320 "compiler/main.gem"
    GemVal _t1987 = gem_v_w;
    GemVal _t1988[] = {gem_table_get(_t1987, gem_string("bindings"))};
                GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t1988, 1);
#line 320 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 320 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_16;
#line 320 "compiler/main.gem"
                    gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 321 "compiler/main.gem"
    GemVal _t1989 = gem_v_w;
    GemVal _t1990[] = {gem_table_get(gem_table_get(_t1989, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t1991 = (*gem_v_collect_free_node);
                    (void)(_t1991.fn(_t1991.env, _t1990, 3));
#line 322 "compiler/main.gem"
    GemVal _t1992 = gem_v_w;
    GemVal _t1993[] = {gem_table_get(gem_table_get(_t1992, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1993, 2))) {
#line 323 "compiler/main.gem"
    GemVal _t1994 = gem_v_w;
    GemVal _t1995 = gem_table_get(gem_table_get(_t1994, gem_string("bindings")), gem_v_bi);
    GemVal _t1996[] = {gem_v_d, gem_table_get(_t1995, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1996, 2));
                    }
                }

            }
#line 327 "compiler/main.gem"
            GemVal gem_v__for_i_17 = gem_int(0);
#line 327 "compiler/main.gem"
    GemVal _t1997 = gem_v_w;
    GemVal _t1998[] = {gem_table_get(_t1997, gem_string("body"))};
            GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t1998, 1);
#line 327 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 327 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_17;
#line 327 "compiler/main.gem"
                gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 328 "compiler/main.gem"
    GemVal _t1999 = gem_v_w;
    GemVal _t2000[] = {gem_table_get(gem_table_get(_t1999, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2001 = (*gem_v_collect_free_node);
                (void)(_t2001.fn(_t2001.env, _t2000, 3));
#line 329 "compiler/main.gem"
    GemVal _t2002 = gem_v_w;
    GemVal _t2003[] = {gem_table_get(gem_table_get(_t2002, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2003, 2))) {
#line 330 "compiler/main.gem"
    GemVal _t2004 = gem_v_w;
    GemVal _t2005 = gem_table_get(gem_table_get(_t2004, gem_string("body")), gem_v_j);
    GemVal _t2006[] = {gem_v_d, gem_table_get(_t2005, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2006, 2));
                }
            }

        }

#line 334 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 335 "compiler/main.gem"
    GemVal _t2007 = gem_table_new();
    GemVal _t2008[] = {gem_v_defined, _t2007};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2008, 2);
            {
#line 336 "compiler/main.gem"
                GemVal gem_v__for_i_19 = gem_int(0);
#line 336 "compiler/main.gem"
    GemVal _t2009[] = {gem_v_el};
                GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t2009, 1);
#line 336 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 336 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_19;
#line 336 "compiler/main.gem"
                    gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 337 "compiler/main.gem"
    GemVal _t2010[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2011 = (*gem_v_collect_free_node);
                    (void)(_t2011.fn(_t2011.env, _t2010, 3));
#line 338 "compiler/main.gem"
    GemVal _t2012[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2012, 2))) {
#line 339 "compiler/main.gem"
    GemVal _t2013 = gem_table_get(gem_v_el, gem_v_j);
    GemVal _t2014[] = {gem_v_d, gem_table_get(_t2013, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2014, 2));
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
    GemVal _t2015[] = {gem_v__match_11};
    GemVal _t2017;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2015, 1), gem_string("table")))) {
        _t2017 = gem_eq(gem_type_fn(NULL, _t2015, 1), gem_string("table"));
    } else {
        GemVal _t2016[] = {gem_v__match_11, gem_string("tag")};
        _t2017 = gem_has_key_fn(NULL, _t2016, 2);
    }
    GemVal _t2018;
    if (!gem_truthy(_t2017)) {
        _t2018 = _t2017;
    } else {
        _t2018 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t2020;
    if (!gem_truthy(_t2018)) {
        _t2020 = _t2018;
    } else {
        GemVal _t2019[] = {gem_v__match_11, gem_string("value")};
        _t2020 = gem_has_key_fn(NULL, _t2019, 2);
    }
    GemVal _t2022;
    if (!gem_truthy(_t2020)) {
        _t2022 = _t2020;
    } else {
        GemVal _t2021[] = {gem_v__match_11, gem_string("bindings")};
        _t2022 = gem_has_key_fn(NULL, _t2021, 2);
    }
    GemVal _t2024;
    if (!gem_truthy(_t2022)) {
        _t2024 = _t2022;
    } else {
        GemVal _t2023[] = {gem_v__match_11, gem_string("body")};
        _t2024 = gem_has_key_fn(NULL, _t2023, 2);
    }
    if (gem_truthy(_t2024)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 344 "compiler/main.gem"
    GemVal _t2025[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2026 = (*gem_v_collect_free_node);
        (void)(_t2026.fn(_t2026.env, _t2025, 3));
#line 345 "compiler/main.gem"
    GemVal _t2027 = gem_table_new();
    GemVal _t2028[] = {gem_v_defined, _t2027};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2028, 2);
#line 346 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 347 "compiler/main.gem"
            GemVal gem_v__for_i_20 = gem_int(0);
#line 347 "compiler/main.gem"
    GemVal _t2029[] = {gem_v_bindings};
            GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t2029, 1);
#line 347 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 347 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_20;
#line 347 "compiler/main.gem"
                gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 348 "compiler/main.gem"
    GemVal _t2030[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2031 = (*gem_v_collect_free_node);
                (void)(_t2031.fn(_t2031.env, _t2030, 3));
#line 349 "compiler/main.gem"
    GemVal _t2032[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2032, 2))) {
#line 350 "compiler/main.gem"
    GemVal _t2033 = gem_table_get(gem_v_bindings, gem_v_bi);
    GemVal _t2034[] = {gem_v_d, gem_table_get(_t2033, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2034, 2));
                }
            }

        }
        {
#line 354 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 354 "compiler/main.gem"
    GemVal _t2035[] = {gem_v_body};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t2035, 1);
#line 354 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 354 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_21;
#line 354 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 355 "compiler/main.gem"
    GemVal _t2036[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2037 = (*gem_v_collect_free_node);
                (void)(_t2037.fn(_t2037.env, _t2036, 3));
#line 356 "compiler/main.gem"
    GemVal _t2038[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2038, 2))) {
#line 357 "compiler/main.gem"
    GemVal _t2039 = gem_table_get(gem_v_body, gem_v_i);
    GemVal _t2040[] = {gem_v_d, gem_table_get(_t2039, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2040, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2041[] = {gem_v__match_11};
    GemVal _t2043;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2041, 1), gem_string("table")))) {
        _t2043 = gem_eq(gem_type_fn(NULL, _t2041, 1), gem_string("table"));
    } else {
        GemVal _t2042[] = {gem_v__match_11, gem_string("tag")};
        _t2043 = gem_has_key_fn(NULL, _t2042, 2);
    }
    GemVal _t2044;
    if (!gem_truthy(_t2043)) {
        _t2044 = _t2043;
    } else {
        _t2044 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2046;
    if (!gem_truthy(_t2044)) {
        _t2046 = _t2044;
    } else {
        GemVal _t2045[] = {gem_v__match_11, gem_string("arms")};
        _t2046 = gem_has_key_fn(NULL, _t2045, 2);
    }
    GemVal _t2048;
    if (!gem_truthy(_t2046)) {
        _t2048 = _t2046;
    } else {
        GemVal _t2047[] = {gem_v__match_11, gem_string("after_ms")};
        _t2048 = gem_has_key_fn(NULL, _t2047, 2);
    }
    GemVal _t2050;
    if (!gem_truthy(_t2048)) {
        _t2050 = _t2048;
    } else {
        GemVal _t2049[] = {gem_v__match_11, gem_string("after_body")};
        _t2050 = gem_has_key_fn(NULL, _t2049, 2);
    }
    if (gem_truthy(_t2050)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 361 "compiler/main.gem"
    GemVal _t2051 = gem_table_new();
    GemVal _t2052[] = {gem_v_defined, _t2051};
        GemVal gem_v_recv_def = gem_fn__mod_codegen_set_union(NULL, _t2052, 2);
#line 362 "compiler/main.gem"
    GemVal _t2053 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t2053, gem_string("recv_var")), GEM_NIL))) {
#line 363 "compiler/main.gem"
    GemVal _t2054 = gem_v_node;
    GemVal _t2055[] = {gem_v_recv_def, gem_table_get(_t2054, gem_string("recv_var"))};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2055, 2));
        }
#line 365 "compiler/main.gem"
        GemVal gem_v__for_i_24 = gem_int(0);
#line 365 "compiler/main.gem"
    GemVal _t2056[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t2056, 1);
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
    GemVal _t2057 = gem_v_arm;
    GemVal _t2058 = gem_table_get(_t2057, gem_string("pattern"));
    GemVal _t2059[] = {gem_table_get(_t2058, gem_string("condition")), gem_v_recv_def, gem_v_free};
    GemVal _t2060 = (*gem_v_collect_free_node);
            (void)(_t2060.fn(_t2060.env, _t2059, 3));
#line 368 "compiler/main.gem"
    GemVal _t2061 = gem_table_new();
    GemVal _t2062[] = {gem_v_recv_def, _t2061};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2062, 2);
#line 369 "compiler/main.gem"
    GemVal _t2063 = gem_v_arm;
    GemVal _t2064 = gem_table_get(_t2063, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2064, gem_string("bindings")), GEM_NIL))) {
#line 370 "compiler/main.gem"
                GemVal gem_v__for_i_22 = gem_int(0);
#line 370 "compiler/main.gem"
    GemVal _t2065 = gem_v_arm;
    GemVal _t2066 = gem_table_get(_t2065, gem_string("pattern"));
    GemVal _t2067[] = {gem_table_get(_t2066, gem_string("bindings"))};
                GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t2067, 1);
#line 370 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 370 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_22;
#line 370 "compiler/main.gem"
                    gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 371 "compiler/main.gem"
    GemVal _t2068 = gem_v_arm;
    GemVal _t2069 = gem_table_get(_t2068, gem_string("pattern"));
    GemVal _t2070[] = {gem_table_get(gem_table_get(_t2069, gem_string("bindings")), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2071 = (*gem_v_collect_free_node);
                    (void)(_t2071.fn(_t2071.env, _t2070, 3));
#line 372 "compiler/main.gem"
    GemVal _t2072 = gem_v_arm;
    GemVal _t2073 = gem_table_get(_t2072, gem_string("pattern"));
    GemVal _t2074[] = {gem_table_get(gem_table_get(_t2073, gem_string("bindings")), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2074, 2))) {
#line 373 "compiler/main.gem"
    GemVal _t2075 = gem_v_arm;
    GemVal _t2076 = gem_table_get(_t2075, gem_string("pattern"));
    GemVal _t2077 = gem_table_get(gem_table_get(_t2076, gem_string("bindings")), gem_v_bi);
    GemVal _t2078[] = {gem_v_d, gem_table_get(_t2077, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2078, 2));
                    }
                }

            }
#line 377 "compiler/main.gem"
            GemVal gem_v__for_i_23 = gem_int(0);
#line 377 "compiler/main.gem"
    GemVal _t2079 = gem_v_arm;
    GemVal _t2080[] = {gem_table_get(_t2079, gem_string("body"))};
            GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t2080, 1);
#line 377 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 377 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_23;
#line 377 "compiler/main.gem"
                gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 378 "compiler/main.gem"
    GemVal _t2081 = gem_v_arm;
    GemVal _t2082[] = {gem_table_get(gem_table_get(_t2081, gem_string("body")), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2083 = (*gem_v_collect_free_node);
                (void)(_t2083.fn(_t2083.env, _t2082, 3));
#line 379 "compiler/main.gem"
    GemVal _t2084 = gem_v_arm;
    GemVal _t2085[] = {gem_table_get(gem_table_get(_t2084, gem_string("body")), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2085, 2))) {
#line 380 "compiler/main.gem"
    GemVal _t2086 = gem_v_arm;
    GemVal _t2087 = gem_table_get(gem_table_get(_t2086, gem_string("body")), gem_v_j);
    GemVal _t2088[] = {gem_v_d, gem_table_get(_t2087, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2088, 2));
                }
            }

        }

#line 384 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 385 "compiler/main.gem"
    GemVal _t2089[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t2090 = (*gem_v_collect_free_node);
            (void)(_t2090.fn(_t2090.env, _t2089, 3));
        }
#line 387 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 388 "compiler/main.gem"
    GemVal _t2091 = gem_table_new();
    GemVal _t2092[] = {gem_v_recv_def, _t2091};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2092, 2);
            {
#line 389 "compiler/main.gem"
                GemVal gem_v__for_i_25 = gem_int(0);
#line 389 "compiler/main.gem"
    GemVal _t2093[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t2093, 1);
#line 389 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 389 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_25;
#line 389 "compiler/main.gem"
                    gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 390 "compiler/main.gem"
    GemVal _t2094[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2095 = (*gem_v_collect_free_node);
                    (void)(_t2095.fn(_t2095.env, _t2094, 3));
#line 391 "compiler/main.gem"
    GemVal _t2096[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2096, 2))) {
#line 392 "compiler/main.gem"
    GemVal _t2097 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    GemVal _t2098[] = {gem_v_d, gem_table_get(_t2097, gem_string("name"))};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2098, 2));
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
    GemVal _t2099[] = {gem_v__match_11};
    GemVal _t2101;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2099, 1), gem_string("table")))) {
        _t2101 = gem_eq(gem_type_fn(NULL, _t2099, 1), gem_string("table"));
    } else {
        GemVal _t2100[] = {gem_v__match_11, gem_string("tag")};
        _t2101 = gem_has_key_fn(NULL, _t2100, 2);
    }
    GemVal _t2102;
    if (!gem_truthy(_t2101)) {
        _t2102 = _t2101;
    } else {
        _t2102 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2104;
    if (!gem_truthy(_t2102)) {
        _t2104 = _t2102;
    } else {
        GemVal _t2103[] = {gem_v__match_11, gem_string("entries")};
        _t2104 = gem_has_key_fn(NULL, _t2103, 2);
    }
    if (gem_truthy(_t2104)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 397 "compiler/main.gem"
            GemVal gem_v__for_i_26 = gem_int(0);
#line 397 "compiler/main.gem"
    GemVal _t2105[] = {gem_v_entries};
            GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t2105, 1);
#line 397 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 397 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_26;
#line 397 "compiler/main.gem"
                gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 398 "compiler/main.gem"
    GemVal _t2106 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2107[] = {gem_table_get(_t2106, gem_string("value")), gem_v_defined, gem_v_free};
    GemVal _t2108 = (*gem_v_collect_free_node);
                (void)(_t2108.fn(_t2108.env, _t2107, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2109[] = {gem_v__match_11};
    GemVal _t2111;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2109, 1), gem_string("table")))) {
        _t2111 = gem_eq(gem_type_fn(NULL, _t2109, 1), gem_string("table"));
    } else {
        GemVal _t2110[] = {gem_v__match_11, gem_string("tag")};
        _t2111 = gem_has_key_fn(NULL, _t2110, 2);
    }
    GemVal _t2112;
    if (!gem_truthy(_t2111)) {
        _t2112 = _t2111;
    } else {
        _t2112 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2114;
    if (!gem_truthy(_t2112)) {
        _t2114 = _t2112;
    } else {
        GemVal _t2113[] = {gem_v__match_11, gem_string("elements")};
        _t2114 = gem_has_key_fn(NULL, _t2113, 2);
    }
    if (gem_truthy(_t2114)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 401 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 401 "compiler/main.gem"
    GemVal _t2115[] = {gem_v_elements};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t2115, 1);
#line 401 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 401 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 401 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 402 "compiler/main.gem"
    GemVal _t2116[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t2117 = (*gem_v_collect_free_node);
                (void)(_t2117.fn(_t2117.env, _t2116, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2118[] = {gem_v__match_11};
    GemVal _t2120;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2118, 1), gem_string("table")))) {
        _t2120 = gem_eq(gem_type_fn(NULL, _t2118, 1), gem_string("table"));
    } else {
        GemVal _t2119[] = {gem_v__match_11, gem_string("tag")};
        _t2120 = gem_has_key_fn(NULL, _t2119, 2);
    }
    GemVal _t2121;
    if (!gem_truthy(_t2120)) {
        _t2121 = _t2120;
    } else {
        _t2121 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2123;
    if (!gem_truthy(_t2121)) {
        _t2123 = _t2121;
    } else {
        GemVal _t2122[] = {gem_v__match_11, gem_string("stmts")};
        _t2123 = gem_has_key_fn(NULL, _t2122, 2);
    }
    if (gem_truthy(_t2123)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 405 "compiler/main.gem"
    GemVal _t2124 = gem_table_new();
    GemVal _t2125[] = {gem_v_defined, _t2124};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2125, 2);
        {
#line 406 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 406 "compiler/main.gem"
    GemVal _t2126[] = {gem_v_stmts};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t2126, 1);
#line 406 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 406 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 406 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 407 "compiler/main.gem"
    GemVal _t2127[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2128 = (*gem_v_collect_free_node);
                (void)(_t2128.fn(_t2128.env, _t2127, 3));
#line 408 "compiler/main.gem"
    GemVal _t2129[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2129, 2))) {
#line 409 "compiler/main.gem"
    GemVal _t2130 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2131[] = {gem_v_d, gem_table_get(_t2130, gem_string("name"))};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2131, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2132[] = {gem_v__match_11};
    GemVal _t2134;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2132, 1), gem_string("table")))) {
        _t2134 = gem_eq(gem_type_fn(NULL, _t2132, 1), gem_string("table"));
    } else {
        GemVal _t2133[] = {gem_v__match_11, gem_string("tag")};
        _t2134 = gem_has_key_fn(NULL, _t2133, 2);
    }
    GemVal _t2135;
    if (!gem_truthy(_t2134)) {
        _t2135 = _t2134;
    } else {
        _t2135 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2137;
    if (!gem_truthy(_t2135)) {
        _t2137 = _t2135;
    } else {
        GemVal _t2136[] = {gem_v__match_11, gem_string("value")};
        _t2137 = gem_has_key_fn(NULL, _t2136, 2);
    }
    if (gem_truthy(_t2137)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 413 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 414 "compiler/main.gem"
    GemVal _t2138[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2139 = (*gem_v_collect_free_node);
            GemVal _t2140 = _t2139.fn(_t2139.env, _t2138, 3);
            gem_pop_frame();
            return _t2140;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 418 "compiler/main.gem"
            GemVal gem_v__for_tbl_29 = gem_v_node;
#line 418 "compiler/main.gem"
    GemVal _t2141[] = {gem_v__for_tbl_29};
            GemVal gem_v__for_len_29 = gem_len_fn(NULL, _t2141, 1);
#line 418 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 418 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_len_29))) break;
#line 418 "compiler/main.gem"
    GemVal _t2142[] = {gem_v__for_tbl_29, gem_v__for_i_29};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2142, 2);
#line 418 "compiler/main.gem"
    GemVal _t2143[] = {gem_v__for_tbl_29, gem_v__for_i_29};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2143, 2);
#line 418 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 419 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 420 "compiler/main.gem"
    GemVal _t2144[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2144, 1), gem_string("table")))) {
#line 421 "compiler/main.gem"
    GemVal _t2145[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2145, 1);
#line 422 "compiler/main.gem"
    GemVal _t2146[] = {gem_v_vks};
    GemVal _t2148;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2146, 1), gem_int(0)))) {
        _t2148 = gem_gt(gem_len_fn(NULL, _t2146, 1), gem_int(0));
    } else {
        GemVal _t2147[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2148 = gem_eq(gem_type_fn(NULL, _t2147, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2148)) {
#line 423 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 424 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2149[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2149, 1)))) break;
#line 425 "compiler/main.gem"
    GemVal _t2150[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2150, 1), gem_string("table")))) {
#line 426 "compiler/main.gem"
    GemVal _t2151[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t2152 = (*gem_v_collect_free_node);
                                    (void)(_t2152.fn(_t2152.env, _t2151, 3));
                                }
#line 428 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 431 "compiler/main.gem"
    GemVal _t2153[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t2154 = (*gem_v_collect_free_node);
                            (void)(_t2154.fn(_t2154.env, _t2153, 3));
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
    GemVal _t2156 = gem_table_new();
    GemVal gem_v_free = _t2156;
#line 441 "compiler/main.gem"
    GemVal _t2157 = gem_table_new();
    GemVal _t2158[] = {gem_v_defined, _t2157};
    GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2158, 2);
#line 442 "compiler/main.gem"
    GemVal gem_v__for_i_30 = gem_int(0);
#line 442 "compiler/main.gem"
    GemVal _t2159[] = {gem_v_stmts};
    GemVal gem_v__for_limit_30 = gem_len_fn(NULL, _t2159, 1);
#line 442 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_limit_30))) break;
#line 442 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_30;
#line 442 "compiler/main.gem"
        gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 443 "compiler/main.gem"
    GemVal _t2160[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2161 = (*gem_v_collect_free_node);
        (void)(_t2161.fn(_t2161.env, _t2160, 3));
#line 444 "compiler/main.gem"
    GemVal _t2162[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2162, 2))) {
#line 445 "compiler/main.gem"
    GemVal _t2163 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2164[] = {gem_v_d, gem_table_get(_t2163, gem_string("name"))};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2164, 2));
        }
#line 447 "compiler/main.gem"
    GemVal _t2165[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2165, 2));
    }

    GemVal _t2166 = gem_v_free;
    gem_pop_frame();
    return _t2166;
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
        GemVal _t2168 = GEM_NIL;
        gem_pop_frame();
        return _t2168;
    }
#line 461 "compiler/main.gem"
    GemVal _t2169[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2169, 1), gem_string("table")))) {
#line 462 "compiler/main.gem"
        GemVal _t2170 = GEM_NIL;
        gem_pop_frame();
        return _t2170;
    }
#line 464 "compiler/main.gem"
    GemVal _t2171 = gem_v_node;
    if (gem_truthy(gem_eq(gem_table_get(_t2171, gem_string("tag")), GEM_NIL))) {
#line 465 "compiler/main.gem"
        GemVal _t2172 = GEM_NIL;
        gem_pop_frame();
        return _t2172;
    }
#line 468 "compiler/main.gem"
    GemVal gem_v__match_31 = gem_v_node;
    GemVal _t2173[] = {gem_v__match_31};
    GemVal _t2175;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2173, 1), gem_string("table")))) {
        _t2175 = gem_eq(gem_type_fn(NULL, _t2173, 1), gem_string("table"));
    } else {
        GemVal _t2174[] = {gem_v__match_31, gem_string("tag")};
        _t2175 = gem_has_key_fn(NULL, _t2174, 2);
    }
    GemVal _t2176;
    if (!gem_truthy(_t2175)) {
        _t2176 = _t2175;
    } else {
        _t2176 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t2178;
    if (!gem_truthy(_t2176)) {
        _t2178 = _t2176;
    } else {
        GemVal _t2177[] = {gem_v__match_31, gem_string("params")};
        _t2178 = gem_has_key_fn(NULL, _t2177, 2);
    }
    GemVal _t2180;
    if (!gem_truthy(_t2178)) {
        _t2180 = _t2178;
    } else {
        GemVal _t2179[] = {gem_v__match_31, gem_string("rest_param")};
        _t2180 = gem_has_key_fn(NULL, _t2179, 2);
    }
    GemVal _t2182;
    if (!gem_truthy(_t2180)) {
        _t2182 = _t2180;
    } else {
        GemVal _t2181[] = {gem_v__match_31, gem_string("body")};
        _t2182 = gem_has_key_fn(NULL, _t2181, 2);
    }
    if (gem_truthy(_t2182)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_31, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_31, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 470 "compiler/main.gem"
    GemVal _t2183[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2183, 1);
#line 471 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 472 "compiler/main.gem"
    GemVal _t2184[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2184, 2));
        }
#line 479 "compiler/main.gem"
    GemVal _t2185[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t2186 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t2186.fn(_t2186.env, _t2185, 2);
#line 480 "compiler/main.gem"
    GemVal _t2187[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn__mod_codegen_set_intersect(NULL, _t2187, 2);
        {
#line 481 "compiler/main.gem"
            GemVal gem_v__for_tbl_32 = gem_v_inter;
#line 481 "compiler/main.gem"
    GemVal _t2188[] = {gem_v__for_tbl_32};
            GemVal gem_v__for_len_32 = gem_len_fn(NULL, _t2188, 1);
#line 481 "compiler/main.gem"
            GemVal gem_v__for_i_32 = gem_int(0);
#line 481 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_32, gem_v__for_len_32))) break;
#line 481 "compiler/main.gem"
    GemVal _t2189[] = {gem_v__for_tbl_32, gem_v__for_i_32};
                GemVal gem_v_ik = gem_table_key_at_fn(NULL, _t2189, 2);
#line 481 "compiler/main.gem"
    GemVal _t2190[] = {gem_v__for_tbl_32, gem_v__for_i_32};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t2190, 2);
#line 481 "compiler/main.gem"
                gem_v__for_i_32 = gem_add(gem_v__for_i_32, gem_int(1));
#line 482 "compiler/main.gem"
    GemVal _t2191[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2191, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2192[] = {gem_v__match_31};
    GemVal _t2194;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2192, 1), gem_string("table")))) {
        _t2194 = gem_eq(gem_type_fn(NULL, _t2192, 1), gem_string("table"));
    } else {
        GemVal _t2193[] = {gem_v__match_31, gem_string("tag")};
        _t2194 = gem_has_key_fn(NULL, _t2193, 2);
    }
    GemVal _t2195;
    if (!gem_truthy(_t2194)) {
        _t2195 = _t2194;
    } else {
        _t2195 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2195)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t2196[] = {gem_v__match_31};
    GemVal _t2198;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2196, 1), gem_string("table")))) {
        _t2198 = gem_eq(gem_type_fn(NULL, _t2196, 1), gem_string("table"));
    } else {
        GemVal _t2197[] = {gem_v__match_31, gem_string("tag")};
        _t2198 = gem_has_key_fn(NULL, _t2197, 2);
    }
    GemVal _t2199;
    if (!gem_truthy(_t2198)) {
        _t2199 = _t2198;
    } else {
        _t2199 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2201;
    if (!gem_truthy(_t2199)) {
        _t2201 = _t2199;
    } else {
        GemVal _t2200[] = {gem_v__match_31, gem_string("cond")};
        _t2201 = gem_has_key_fn(NULL, _t2200, 2);
    }
    GemVal _t2203;
    if (!gem_truthy(_t2201)) {
        _t2203 = _t2201;
    } else {
        GemVal _t2202[] = {gem_v__match_31, gem_string("then")};
        _t2203 = gem_has_key_fn(NULL, _t2202, 2);
    }
    GemVal _t2205;
    if (!gem_truthy(_t2203)) {
        _t2205 = _t2203;
    } else {
        GemVal _t2204[] = {gem_v__match_31, gem_string("else")};
        _t2205 = gem_has_key_fn(NULL, _t2204, 2);
    }
    if (gem_truthy(_t2205)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_31, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 487 "compiler/main.gem"
    GemVal _t2206[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2207 = (*gem_v_walk_captures_node);
        (void)(_t2207.fn(_t2207.env, _t2206, 3));
#line 488 "compiler/main.gem"
    GemVal _t2208[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t2209 = (*gem_v_walk_captures);
        (void)(_t2209.fn(_t2209.env, _t2208, 3));
#line 489 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 490 "compiler/main.gem"
    GemVal _t2210[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2211 = (*gem_v_walk_captures);
            GemVal _t2212 = _t2211.fn(_t2211.env, _t2210, 3);
            gem_pop_frame();
            return _t2212;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2213[] = {gem_v__match_31};
    GemVal _t2215;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2213, 1), gem_string("table")))) {
        _t2215 = gem_eq(gem_type_fn(NULL, _t2213, 1), gem_string("table"));
    } else {
        GemVal _t2214[] = {gem_v__match_31, gem_string("tag")};
        _t2215 = gem_has_key_fn(NULL, _t2214, 2);
    }
    GemVal _t2216;
    if (!gem_truthy(_t2215)) {
        _t2216 = _t2215;
    } else {
        _t2216 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("while"));
    }
    GemVal _t2218;
    if (!gem_truthy(_t2216)) {
        _t2218 = _t2216;
    } else {
        GemVal _t2217[] = {gem_v__match_31, gem_string("cond")};
        _t2218 = gem_has_key_fn(NULL, _t2217, 2);
    }
    GemVal _t2220;
    if (!gem_truthy(_t2218)) {
        _t2220 = _t2218;
    } else {
        GemVal _t2219[] = {gem_v__match_31, gem_string("body")};
        _t2220 = gem_has_key_fn(NULL, _t2219, 2);
    }
    if (gem_truthy(_t2220)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_31, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_31, gem_string("body"));
#line 493 "compiler/main.gem"
    GemVal _t2221[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2222 = (*gem_v_walk_captures_node);
        (void)(_t2222.fn(_t2222.env, _t2221, 3));
#line 494 "compiler/main.gem"
    GemVal _t2223[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2224 = (*gem_v_walk_captures);
        GemVal _t2225 = _t2224.fn(_t2224.env, _t2223, 3);
        gem_pop_frame();
        return _t2225;
    } else {
    GemVal _t2226[] = {gem_v__match_31};
    GemVal _t2228;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2226, 1), gem_string("table")))) {
        _t2228 = gem_eq(gem_type_fn(NULL, _t2226, 1), gem_string("table"));
    } else {
        GemVal _t2227[] = {gem_v__match_31, gem_string("tag")};
        _t2228 = gem_has_key_fn(NULL, _t2227, 2);
    }
    GemVal _t2229;
    if (!gem_truthy(_t2228)) {
        _t2229 = _t2228;
    } else {
        _t2229 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2231;
    if (!gem_truthy(_t2229)) {
        _t2231 = _t2229;
    } else {
        GemVal _t2230[] = {gem_v__match_31, gem_string("target")};
        _t2231 = gem_has_key_fn(NULL, _t2230, 2);
    }
    GemVal _t2233;
    if (!gem_truthy(_t2231)) {
        _t2233 = _t2231;
    } else {
        GemVal _t2232[] = {gem_v__match_31, gem_string("whens")};
        _t2233 = gem_has_key_fn(NULL, _t2232, 2);
    }
    GemVal _t2235;
    if (!gem_truthy(_t2233)) {
        _t2235 = _t2233;
    } else {
        GemVal _t2234[] = {gem_v__match_31, gem_string("else")};
        _t2235 = gem_has_key_fn(NULL, _t2234, 2);
    }
    if (gem_truthy(_t2235)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_31, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_31, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_31, gem_string("else"));
#line 496 "compiler/main.gem"
    GemVal _t2236[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2237 = (*gem_v_walk_captures_node);
        (void)(_t2237.fn(_t2237.env, _t2236, 3));
#line 497 "compiler/main.gem"
        GemVal gem_v__for_i_34 = gem_int(0);
#line 497 "compiler/main.gem"
    GemVal _t2238[] = {gem_v_whens};
        GemVal gem_v__for_limit_34 = gem_len_fn(NULL, _t2238, 1);
#line 497 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_limit_34))) break;
#line 497 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_34;
#line 497 "compiler/main.gem"
            gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 498 "compiler/main.gem"
    GemVal _t2239 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2240[] = {gem_table_get(_t2239, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2241 = (*gem_v_walk_captures_node);
            (void)(_t2241.fn(_t2241.env, _t2240, 3));
#line 499 "compiler/main.gem"
    GemVal _t2242 = gem_table_get(gem_v_whens, gem_v_i);
            if (gem_truthy(gem_neq(gem_table_get(_t2242, gem_string("bindings")), GEM_NIL))) {
#line 500 "compiler/main.gem"
                GemVal gem_v__for_i_33 = gem_int(0);
#line 500 "compiler/main.gem"
    GemVal _t2243 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2244[] = {gem_table_get(_t2243, gem_string("bindings"))};
                GemVal gem_v__for_limit_33 = gem_len_fn(NULL, _t2244, 1);
#line 500 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_v__for_limit_33))) break;
#line 500 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_33;
#line 500 "compiler/main.gem"
                    gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 501 "compiler/main.gem"
    GemVal _t2245 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2246[] = {gem_table_get(gem_table_get(_t2245, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2247 = (*gem_v_walk_captures_node);
                    (void)(_t2247.fn(_t2247.env, _t2246, 3));
                }

            }
#line 504 "compiler/main.gem"
    GemVal _t2248 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2249[] = {gem_table_get(_t2248, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2250 = (*gem_v_walk_captures);
            (void)(_t2250.fn(_t2250.env, _t2249, 3));
        }

#line 506 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 507 "compiler/main.gem"
    GemVal _t2251[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2252 = (*gem_v_walk_captures);
            GemVal _t2253 = _t2252.fn(_t2252.env, _t2251, 3);
            gem_pop_frame();
            return _t2253;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2254[] = {gem_v__match_31};
    GemVal _t2256;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2254, 1), gem_string("table")))) {
        _t2256 = gem_eq(gem_type_fn(NULL, _t2254, 1), gem_string("table"));
    } else {
        GemVal _t2255[] = {gem_v__match_31, gem_string("tag")};
        _t2256 = gem_has_key_fn(NULL, _t2255, 2);
    }
    GemVal _t2257;
    if (!gem_truthy(_t2256)) {
        _t2257 = _t2256;
    } else {
        _t2257 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2259;
    if (!gem_truthy(_t2257)) {
        _t2259 = _t2257;
    } else {
        GemVal _t2258[] = {gem_v__match_31, gem_string("arms")};
        _t2259 = gem_has_key_fn(NULL, _t2258, 2);
    }
    GemVal _t2261;
    if (!gem_truthy(_t2259)) {
        _t2261 = _t2259;
    } else {
        GemVal _t2260[] = {gem_v__match_31, gem_string("after_ms")};
        _t2261 = gem_has_key_fn(NULL, _t2260, 2);
    }
    GemVal _t2263;
    if (!gem_truthy(_t2261)) {
        _t2263 = _t2261;
    } else {
        GemVal _t2262[] = {gem_v__match_31, gem_string("after_body")};
        _t2263 = gem_has_key_fn(NULL, _t2262, 2);
    }
    if (gem_truthy(_t2263)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_31, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_31, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_31, gem_string("after_body"));
#line 510 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 510 "compiler/main.gem"
    GemVal _t2264[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2264, 1);
#line 510 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 510 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 510 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 511 "compiler/main.gem"
    GemVal _t2265 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2266 = gem_table_get(_t2265, gem_string("pattern"));
    GemVal _t2267[] = {gem_table_get(_t2266, gem_string("condition")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2268 = (*gem_v_walk_captures_node);
            (void)(_t2268.fn(_t2268.env, _t2267, 3));
#line 512 "compiler/main.gem"
    GemVal _t2269 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2270 = gem_table_get(_t2269, gem_string("pattern"));
            if (gem_truthy(gem_neq(gem_table_get(_t2270, gem_string("bindings")), GEM_NIL))) {
#line 513 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 513 "compiler/main.gem"
    GemVal _t2271 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2272 = gem_table_get(_t2271, gem_string("pattern"));
    GemVal _t2273[] = {gem_table_get(_t2272, gem_string("bindings"))};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2273, 1);
#line 513 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 513 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 513 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 514 "compiler/main.gem"
    GemVal _t2274 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2275 = gem_table_get(_t2274, gem_string("pattern"));
    GemVal _t2276[] = {gem_table_get(gem_table_get(_t2275, gem_string("bindings")), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2277 = (*gem_v_walk_captures_node);
                    (void)(_t2277.fn(_t2277.env, _t2276, 3));
                }

            }
#line 517 "compiler/main.gem"
    GemVal _t2278 = gem_table_get(gem_v_rm_arms, gem_v_i);
    GemVal _t2279[] = {gem_table_get(_t2278, gem_string("body")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2280 = (*gem_v_walk_captures);
            (void)(_t2280.fn(_t2280.env, _t2279, 3));
        }

#line 519 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 520 "compiler/main.gem"
    GemVal _t2281[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2282 = (*gem_v_walk_captures_node);
            (void)(_t2282.fn(_t2282.env, _t2281, 3));
        }
#line 522 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 523 "compiler/main.gem"
    GemVal _t2283[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2284 = (*gem_v_walk_captures);
            GemVal _t2285 = _t2284.fn(_t2284.env, _t2283, 3);
            gem_pop_frame();
            return _t2285;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2286[] = {gem_v__match_31};
    GemVal _t2288;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2286, 1), gem_string("table")))) {
        _t2288 = gem_eq(gem_type_fn(NULL, _t2286, 1), gem_string("table"));
    } else {
        GemVal _t2287[] = {gem_v__match_31, gem_string("tag")};
        _t2288 = gem_has_key_fn(NULL, _t2287, 2);
    }
    GemVal _t2289;
    if (!gem_truthy(_t2288)) {
        _t2289 = _t2288;
    } else {
        _t2289 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2291;
    if (!gem_truthy(_t2289)) {
        _t2291 = _t2289;
    } else {
        GemVal _t2290[] = {gem_v__match_31, gem_string("entries")};
        _t2291 = gem_has_key_fn(NULL, _t2290, 2);
    }
    if (gem_truthy(_t2291)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_31, gem_string("entries"));
        {
#line 526 "compiler/main.gem"
            GemVal gem_v__for_i_37 = gem_int(0);
#line 526 "compiler/main.gem"
    GemVal _t2292[] = {gem_v_entries};
            GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2292, 1);
#line 526 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 526 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_37;
#line 526 "compiler/main.gem"
                gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 527 "compiler/main.gem"
    GemVal _t2293 = gem_table_get(gem_v_entries, gem_v_i);
    GemVal _t2294[] = {gem_table_get(_t2293, gem_string("value")), gem_v_scope_vars, gem_v_captured};
    GemVal _t2295 = (*gem_v_walk_captures_node);
                (void)(_t2295.fn(_t2295.env, _t2294, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2296[] = {gem_v__match_31};
    GemVal _t2298;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2296, 1), gem_string("table")))) {
        _t2298 = gem_eq(gem_type_fn(NULL, _t2296, 1), gem_string("table"));
    } else {
        GemVal _t2297[] = {gem_v__match_31, gem_string("tag")};
        _t2298 = gem_has_key_fn(NULL, _t2297, 2);
    }
    GemVal _t2299;
    if (!gem_truthy(_t2298)) {
        _t2299 = _t2298;
    } else {
        _t2299 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2301;
    if (!gem_truthy(_t2299)) {
        _t2301 = _t2299;
    } else {
        GemVal _t2300[] = {gem_v__match_31, gem_string("elements")};
        _t2301 = gem_has_key_fn(NULL, _t2300, 2);
    }
    if (gem_truthy(_t2301)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_31, gem_string("elements"));
        {
#line 530 "compiler/main.gem"
            GemVal gem_v__for_i_38 = gem_int(0);
#line 530 "compiler/main.gem"
    GemVal _t2302[] = {gem_v_elements};
            GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2302, 1);
#line 530 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 530 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_38;
#line 530 "compiler/main.gem"
                gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 531 "compiler/main.gem"
    GemVal _t2303[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2304 = (*gem_v_walk_captures_node);
                (void)(_t2304.fn(_t2304.env, _t2303, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2305[] = {gem_v__match_31};
    GemVal _t2307;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2305, 1), gem_string("table")))) {
        _t2307 = gem_eq(gem_type_fn(NULL, _t2305, 1), gem_string("table"));
    } else {
        GemVal _t2306[] = {gem_v__match_31, gem_string("tag")};
        _t2307 = gem_has_key_fn(NULL, _t2306, 2);
    }
    GemVal _t2308;
    if (!gem_truthy(_t2307)) {
        _t2308 = _t2307;
    } else {
        _t2308 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2310;
    if (!gem_truthy(_t2308)) {
        _t2310 = _t2308;
    } else {
        GemVal _t2309[] = {gem_v__match_31, gem_string("stmts")};
        _t2310 = gem_has_key_fn(NULL, _t2309, 2);
    }
    if (gem_truthy(_t2310)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_31, gem_string("stmts"));
#line 534 "compiler/main.gem"
    GemVal _t2311[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2312 = (*gem_v_walk_captures);
        GemVal _t2313 = _t2312.fn(_t2312.env, _t2311, 3);
        gem_pop_frame();
        return _t2313;
    } else {
    GemVal _t2314[] = {gem_v__match_31};
    GemVal _t2316;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2314, 1), gem_string("table")))) {
        _t2316 = gem_eq(gem_type_fn(NULL, _t2314, 1), gem_string("table"));
    } else {
        GemVal _t2315[] = {gem_v__match_31, gem_string("tag")};
        _t2316 = gem_has_key_fn(NULL, _t2315, 2);
    }
    GemVal _t2317;
    if (!gem_truthy(_t2316)) {
        _t2317 = _t2316;
    } else {
        _t2317 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2319;
    if (!gem_truthy(_t2317)) {
        _t2319 = _t2317;
    } else {
        GemVal _t2318[] = {gem_v__match_31, gem_string("value")};
        _t2319 = gem_has_key_fn(NULL, _t2318, 2);
    }
    if (gem_truthy(_t2319)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 536 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 537 "compiler/main.gem"
    GemVal _t2320[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2321 = (*gem_v_walk_captures_node);
            GemVal _t2322 = _t2321.fn(_t2321.env, _t2320, 3);
            gem_pop_frame();
            return _t2322;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2323[] = {gem_v__match_31};
    GemVal _t2325;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2323, 1), gem_string("table")))) {
        _t2325 = gem_eq(gem_type_fn(NULL, _t2323, 1), gem_string("table"));
    } else {
        GemVal _t2324[] = {gem_v__match_31, gem_string("tag")};
        _t2325 = gem_has_key_fn(NULL, _t2324, 2);
    }
    GemVal _t2326;
    if (!gem_truthy(_t2325)) {
        _t2326 = _t2325;
    } else {
        _t2326 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2328;
    if (!gem_truthy(_t2326)) {
        _t2328 = _t2326;
    } else {
        GemVal _t2327[] = {gem_v__match_31, gem_string("value")};
        _t2328 = gem_has_key_fn(NULL, _t2327, 2);
    }
    if (gem_truthy(_t2328)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 540 "compiler/main.gem"
    GemVal _t2329[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2330 = (*gem_v_walk_captures_node);
        GemVal _t2331 = _t2330.fn(_t2330.env, _t2329, 3);
        gem_pop_frame();
        return _t2331;
    } else {
    GemVal _t2332[] = {gem_v__match_31};
    GemVal _t2334;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2332, 1), gem_string("table")))) {
        _t2334 = gem_eq(gem_type_fn(NULL, _t2332, 1), gem_string("table"));
    } else {
        GemVal _t2333[] = {gem_v__match_31, gem_string("tag")};
        _t2334 = gem_has_key_fn(NULL, _t2333, 2);
    }
    GemVal _t2335;
    if (!gem_truthy(_t2334)) {
        _t2335 = _t2334;
    } else {
        _t2335 = gem_eq(gem_table_get(gem_v__match_31, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2337;
    if (!gem_truthy(_t2335)) {
        _t2337 = _t2335;
    } else {
        GemVal _t2336[] = {gem_v__match_31, gem_string("value")};
        _t2337 = gem_has_key_fn(NULL, _t2336, 2);
    }
    if (gem_truthy(_t2337)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_31, gem_string("value"));
#line 542 "compiler/main.gem"
    GemVal _t2338[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2339 = (*gem_v_walk_captures_node);
        GemVal _t2340 = _t2339.fn(_t2339.env, _t2338, 3);
        gem_pop_frame();
        return _t2340;
    } else {
        {
#line 545 "compiler/main.gem"
            GemVal gem_v__for_tbl_39 = gem_v_node;
#line 545 "compiler/main.gem"
    GemVal _t2341[] = {gem_v__for_tbl_39};
            GemVal gem_v__for_len_39 = gem_len_fn(NULL, _t2341, 1);
#line 545 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 545 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_len_39))) break;
#line 545 "compiler/main.gem"
    GemVal _t2342[] = {gem_v__for_tbl_39, gem_v__for_i_39};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2342, 2);
#line 545 "compiler/main.gem"
    GemVal _t2343[] = {gem_v__for_tbl_39, gem_v__for_i_39};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2343, 2);
#line 545 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 546 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 547 "compiler/main.gem"
    GemVal _t2344[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2344, 1), gem_string("table")))) {
#line 548 "compiler/main.gem"
    GemVal _t2345[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2345, 1);
#line 549 "compiler/main.gem"
    GemVal _t2346[] = {gem_v_vks};
    GemVal _t2348;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2346, 1), gem_int(0)))) {
        _t2348 = gem_gt(gem_len_fn(NULL, _t2346, 1), gem_int(0));
    } else {
        GemVal _t2347[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2348 = gem_eq(gem_type_fn(NULL, _t2347, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2348)) {
#line 550 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 551 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2349[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2349, 1)))) break;
#line 552 "compiler/main.gem"
    GemVal _t2350[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2350, 1), gem_string("table")))) {
#line 553 "compiler/main.gem"
    GemVal _t2351[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2352 = (*gem_v_walk_captures_node);
                                    (void)(_t2352.fn(_t2352.env, _t2351, 3));
                                }
#line 555 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 558 "compiler/main.gem"
    GemVal _t2353[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2354 = (*gem_v_walk_captures_node);
                            (void)(_t2354.fn(_t2354.env, _t2353, 3));
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
    GemVal _t2356 = gem_table_new();
    GemVal _t2357[] = {gem_v_scope_vars, _t2356};
    GemVal gem_v_sv = gem_fn__mod_codegen_set_union(NULL, _t2357, 2);
    {
#line 568 "compiler/main.gem"
        GemVal gem_v__for_i_40 = gem_int(0);
#line 568 "compiler/main.gem"
    GemVal _t2358[] = {gem_v_stmts};
        GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2358, 1);
#line 568 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 568 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_40;
#line 568 "compiler/main.gem"
            gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 569 "compiler/main.gem"
    GemVal _t2359[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2360 = (*gem_v_walk_captures_node);
            (void)(_t2360.fn(_t2360.env, _t2359, 3));
#line 570 "compiler/main.gem"
    GemVal _t2361[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2361, 2))) {
#line 571 "compiler/main.gem"
    GemVal _t2362 = gem_table_get(gem_v_stmts, gem_v_i);
    GemVal _t2363[] = {gem_v_sv, gem_table_get(_t2362, gem_string("name"))};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2363, 2));
            }
#line 573 "compiler/main.gem"
    GemVal _t2364[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2364, 2));
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
    GemVal _t2366 = gem_table_new();
    GemVal gem_v_captured = _t2366;
#line 579 "compiler/main.gem"
    GemVal _t2367[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2368 = (*gem_v_walk_captures);
    (void)(_t2368.fn(_t2368.env, _t2367, 3));
    GemVal _t2369 = gem_v_captured;
    gem_pop_frame();
    return _t2369;
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
        GemVal _t2371 = gem_bool(0);
        gem_pop_frame();
        return _t2371;
    }
#line 613 "compiler/main.gem"
    GemVal _t2372[] = {gem_v_node};
    GemVal _t2374;
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2372, 1), gem_string("table")))) {
        _t2374 = gem_neq(gem_type_fn(NULL, _t2372, 1), gem_string("table"));
    } else {
        GemVal _t2373 = gem_v_node;
        _t2374 = gem_eq(gem_table_get(_t2373, gem_string("tag")), GEM_NIL);
    }
    if (gem_truthy(_t2374)) {
#line 614 "compiler/main.gem"
        GemVal _t2375 = gem_bool(0);
        gem_pop_frame();
        return _t2375;
    }
#line 616 "compiler/main.gem"
    GemVal gem_v__match_41 = gem_v_node;
    GemVal _t2376[] = {gem_v__match_41};
    GemVal _t2378;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2376, 1), gem_string("table")))) {
        _t2378 = gem_eq(gem_type_fn(NULL, _t2376, 1), gem_string("table"));
    } else {
        GemVal _t2377[] = {gem_v__match_41, gem_string("tag")};
        _t2378 = gem_has_key_fn(NULL, _t2377, 2);
    }
    GemVal _t2379;
    if (!gem_truthy(_t2378)) {
        _t2379 = _t2378;
    } else {
        _t2379 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("call"));
    }
    GemVal _t2381;
    if (!gem_truthy(_t2379)) {
        _t2381 = _t2379;
    } else {
        GemVal _t2380[] = {gem_v__match_41, gem_string("func")};
        _t2381 = gem_has_key_fn(NULL, _t2380, 2);
    }
    if (gem_truthy(_t2381)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_func = gem_table_get(gem_v__match_41, gem_string("func"));
#line 618 "compiler/main.gem"
    GemVal _t2382[] = {gem_v_func, gem_string("var")};
    GemVal _t2384;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2382, 2))) {
        _t2384 = gem_fn__mod_codegen_is_node(NULL, _t2382, 2);
    } else {
        GemVal _t2383 = gem_v_func;
        _t2384 = gem_eq(gem_table_get(_t2383, gem_string("name")), gem_v_fn_name);
    }
        GemVal _t2385 = _t2384;
        gem_pop_frame();
        return _t2385;
    } else {
    GemVal _t2386[] = {gem_v__match_41};
    GemVal _t2388;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2386, 1), gem_string("table")))) {
        _t2388 = gem_eq(gem_type_fn(NULL, _t2386, 1), gem_string("table"));
    } else {
        GemVal _t2387[] = {gem_v__match_41, gem_string("tag")};
        _t2388 = gem_has_key_fn(NULL, _t2387, 2);
    }
    GemVal _t2389;
    if (!gem_truthy(_t2388)) {
        _t2389 = _t2388;
    } else {
        _t2389 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2391;
    if (!gem_truthy(_t2389)) {
        _t2391 = _t2389;
    } else {
        GemVal _t2390[] = {gem_v__match_41, gem_string("value")};
        _t2391 = gem_has_key_fn(NULL, _t2390, 2);
    }
    if (gem_truthy(_t2391)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_41, gem_string("value"));
#line 620 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 621 "compiler/main.gem"
    GemVal _t2392[] = {gem_v_fn_name, gem_v_value};
    GemVal _t2393 = (*gem_v_is_self_tail_call);
            GemVal _t2394 = _t2393.fn(_t2393.env, _t2392, 2);
            gem_pop_frame();
            return _t2394;
        }
#line 623 "compiler/main.gem"
        GemVal _t2395 = gem_bool(0);
        gem_pop_frame();
        return _t2395;
    } else {
    GemVal _t2396[] = {gem_v__match_41};
    GemVal _t2398;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2396, 1), gem_string("table")))) {
        _t2398 = gem_eq(gem_type_fn(NULL, _t2396, 1), gem_string("table"));
    } else {
        GemVal _t2397[] = {gem_v__match_41, gem_string("tag")};
        _t2398 = gem_has_key_fn(NULL, _t2397, 2);
    }
    GemVal _t2399;
    if (!gem_truthy(_t2398)) {
        _t2399 = _t2398;
    } else {
        _t2399 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2401;
    if (!gem_truthy(_t2399)) {
        _t2401 = _t2399;
    } else {
        GemVal _t2400[] = {gem_v__match_41, gem_string("then")};
        _t2401 = gem_has_key_fn(NULL, _t2400, 2);
    }
    GemVal _t2403;
    if (!gem_truthy(_t2401)) {
        _t2403 = _t2401;
    } else {
        GemVal _t2402[] = {gem_v__match_41, gem_string("else")};
        _t2403 = gem_has_key_fn(NULL, _t2402, 2);
    }
    if (gem_truthy(_t2403)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_then_body = gem_table_get(gem_v__match_41, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 625 "compiler/main.gem"
    GemVal _t2404[] = {gem_v_fn_name, gem_v_then_body};
    GemVal _t2405 = (*gem_v_has_self_tail_call);
        if (gem_truthy(_t2405.fn(_t2405.env, _t2404, 2))) {
#line 626 "compiler/main.gem"
            GemVal _t2406 = gem_bool(1);
            gem_pop_frame();
            return _t2406;
        }
#line 628 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 629 "compiler/main.gem"
    GemVal _t2407[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2408 = (*gem_v_has_self_tail_call);
            GemVal _t2409 = _t2408.fn(_t2408.env, _t2407, 2);
            gem_pop_frame();
            return _t2409;
        }
#line 631 "compiler/main.gem"
        GemVal _t2410 = gem_bool(0);
        gem_pop_frame();
        return _t2410;
    } else {
    GemVal _t2411[] = {gem_v__match_41};
    GemVal _t2413;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2411, 1), gem_string("table")))) {
        _t2413 = gem_eq(gem_type_fn(NULL, _t2411, 1), gem_string("table"));
    } else {
        GemVal _t2412[] = {gem_v__match_41, gem_string("tag")};
        _t2413 = gem_has_key_fn(NULL, _t2412, 2);
    }
    GemVal _t2414;
    if (!gem_truthy(_t2413)) {
        _t2414 = _t2413;
    } else {
        _t2414 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2416;
    if (!gem_truthy(_t2414)) {
        _t2416 = _t2414;
    } else {
        GemVal _t2415[] = {gem_v__match_41, gem_string("whens")};
        _t2416 = gem_has_key_fn(NULL, _t2415, 2);
    }
    GemVal _t2418;
    if (!gem_truthy(_t2416)) {
        _t2418 = _t2416;
    } else {
        GemVal _t2417[] = {gem_v__match_41, gem_string("else")};
        _t2418 = gem_has_key_fn(NULL, _t2417, 2);
    }
    if (gem_truthy(_t2418)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_41, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_41, gem_string("else"));
#line 633 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 633 "compiler/main.gem"
    GemVal _t2419[] = {gem_v_whens};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2419, 1);
#line 633 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 633 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 633 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 634 "compiler/main.gem"
    GemVal _t2420 = gem_table_get(gem_v_whens, gem_v_i);
    GemVal _t2421[] = {gem_v_fn_name, gem_table_get(_t2420, gem_string("body"))};
    GemVal _t2422 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2422.fn(_t2422.env, _t2421, 2))) {
#line 635 "compiler/main.gem"
                GemVal _t2423 = gem_bool(1);
                gem_pop_frame();
                return _t2423;
            }
        }

#line 638 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 639 "compiler/main.gem"
    GemVal _t2424[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2425 = (*gem_v_has_self_tail_call);
            GemVal _t2426 = _t2425.fn(_t2425.env, _t2424, 2);
            gem_pop_frame();
            return _t2426;
        }
#line 641 "compiler/main.gem"
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
        _t2431 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2433;
    if (!gem_truthy(_t2431)) {
        _t2433 = _t2431;
    } else {
        GemVal _t2432[] = {gem_v__match_41, gem_string("arms")};
        _t2433 = gem_has_key_fn(NULL, _t2432, 2);
    }
    GemVal _t2435;
    if (!gem_truthy(_t2433)) {
        _t2435 = _t2433;
    } else {
        GemVal _t2434[] = {gem_v__match_41, gem_string("after_body")};
        _t2435 = gem_has_key_fn(NULL, _t2434, 2);
    }
    if (gem_truthy(_t2435)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_arms = gem_table_get(gem_v__match_41, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_after_body = gem_table_get(gem_v__match_41, gem_string("after_body"));
#line 643 "compiler/main.gem"
        GemVal gem_v__for_i_43 = gem_int(0);
#line 643 "compiler/main.gem"
    GemVal _t2436[] = {gem_v_arms};
        GemVal gem_v__for_limit_43 = gem_len_fn(NULL, _t2436, 1);
#line 643 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_43, gem_v__for_limit_43))) break;
#line 643 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_43;
#line 643 "compiler/main.gem"
            gem_v__for_i_43 = gem_add(gem_v__for_i_43, gem_int(1));
#line 644 "compiler/main.gem"
    GemVal _t2437 = gem_table_get(gem_v_arms, gem_v_i);
    GemVal _t2438[] = {gem_v_fn_name, gem_table_get(_t2437, gem_string("body"))};
    GemVal _t2439 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2439.fn(_t2439.env, _t2438, 2))) {
#line 645 "compiler/main.gem"
                GemVal _t2440 = gem_bool(1);
                gem_pop_frame();
                return _t2440;
            }
        }

#line 648 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 649 "compiler/main.gem"
    GemVal _t2441[] = {gem_v_fn_name, gem_v_after_body};
    GemVal _t2442 = (*gem_v_has_self_tail_call);
            GemVal _t2443 = _t2442.fn(_t2442.env, _t2441, 2);
            gem_pop_frame();
            return _t2443;
        }
#line 651 "compiler/main.gem"
        GemVal _t2444 = gem_bool(0);
        gem_pop_frame();
        return _t2444;
    } else {
    GemVal _t2445[] = {gem_v__match_41};
    GemVal _t2447;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2445, 1), gem_string("table")))) {
        _t2447 = gem_eq(gem_type_fn(NULL, _t2445, 1), gem_string("table"));
    } else {
        GemVal _t2446[] = {gem_v__match_41, gem_string("tag")};
        _t2447 = gem_has_key_fn(NULL, _t2446, 2);
    }
    GemVal _t2448;
    if (!gem_truthy(_t2447)) {
        _t2448 = _t2447;
    } else {
        _t2448 = gem_eq(gem_table_get(gem_v__match_41, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2450;
    if (!gem_truthy(_t2448)) {
        _t2450 = _t2448;
    } else {
        GemVal _t2449[] = {gem_v__match_41, gem_string("stmts")};
        _t2450 = gem_has_key_fn(NULL, _t2449, 2);
    }
    if (gem_truthy(_t2450)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_41, gem_string("stmts"));
#line 653 "compiler/main.gem"
    GemVal _t2451[] = {gem_v_fn_name, gem_v_stmts};
    GemVal _t2452 = (*gem_v_has_self_tail_call);
        GemVal _t2453 = _t2452.fn(_t2452.env, _t2451, 2);
        gem_pop_frame();
        return _t2453;
    }
    }
    }
    }
    }
    }
#line 655 "compiler/main.gem"
    GemVal _t2454 = gem_bool(0);
    gem_pop_frame();
    return _t2454;
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
    GemVal _t2456[] = {gem_v_stmts};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2456, 1), gem_int(0)))) {
#line 660 "compiler/main.gem"
        GemVal _t2457 = gem_bool(0);
        gem_pop_frame();
        return _t2457;
    }
#line 662 "compiler/main.gem"
    GemVal _t2458[] = {gem_v_fn_name, gem_table_get(gem_v_stmts, gem_neg(gem_int(1)))};
    GemVal _t2459 = (*gem_v_is_self_tail_call);
    GemVal _t2460 = _t2459.fn(_t2459.env, _t2458, 2);
    gem_pop_frame();
    return _t2460;
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
    GemVal _t2462[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t2462, 1);
#line 667 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 668 "compiler/main.gem"
    GemVal _t2463 = gem_v_call_node;
    GemVal gem_v_args = gem_table_get(_t2463, gem_string("args"));
#line 669 "compiler/main.gem"
    GemVal _t2464 = gem_table_new();
    GemVal gem_v_arg_temps = _t2464;
#line 670 "compiler/main.gem"
    GemVal gem_v__for_i_44 = gem_int(0);
#line 670 "compiler/main.gem"
    GemVal _t2465[] = {gem_v_args};
    GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2465, 1);
#line 670 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 670 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_44;
#line 670 "compiler/main.gem"
        gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 671 "compiler/main.gem"
    GemVal _t2466[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2467 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2467.fn(_t2467.env, _t2466, 1);
#line 672 "compiler/main.gem"
    GemVal _t2468 = (*gem_v_tmp);
        GemVal gem_v_t = _t2468.fn(_t2468.env, NULL, 0);
#line 673 "compiler/main.gem"
    GemVal _t2469 = gem_v_r;
    GemVal _t2470[] = {gem_v_b, gem_table_get(_t2469, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2470, 2));
#line 674 "compiler/main.gem"
    GemVal _t2471[] = {gem_v_p};
    GemVal _t2472[] = {gem_v_t};
    GemVal _t2473 = gem_v_r;
    GemVal _t2474[] = {gem_table_get(_t2473, gem_string("expr"))};
    GemVal _t2475[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2471, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t2472, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2474, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t2475, 2));
#line 675 "compiler/main.gem"
    GemVal _t2476[] = {gem_v_arg_temps, gem_v_t};
        (void)(gem_push_fn(NULL, _t2476, 2));
    }

#line 677 "compiler/main.gem"
    GemVal gem_v__for_i_45 = gem_int(0);
#line 677 "compiler/main.gem"
    GemVal _t2477[] = {(*gem_v_tco_params)};
    GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2477, 1);
#line 677 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 677 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_45;
#line 677 "compiler/main.gem"
        gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 678 "compiler/main.gem"
    GemVal _t2478[] = {gem_table_get((*gem_v_tco_params), gem_v_i)};
    GemVal _t2479 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2479.fn(_t2479.env, _t2478, 1);
#line 679 "compiler/main.gem"
        GemVal gem_v_val = gem_string("GEM_NIL");
#line 680 "compiler/main.gem"
    GemVal _t2480[] = {gem_v_arg_temps};
        if (gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t2480, 1)))) {
#line 681 "compiler/main.gem"
            gem_v_val = gem_table_get(gem_v_arg_temps, gem_v_i);
        }
#line 683 "compiler/main.gem"
    GemVal _t2481[] = {(*gem_v_tco_captured), gem_table_get((*gem_v_tco_params), gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2481, 2))) {
#line 684 "compiler/main.gem"
    GemVal _t2482[] = {gem_v_p};
    GemVal _t2483[] = {gem_v_mp};
    GemVal _t2484[] = {gem_v_val};
    GemVal _t2485[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2482, 1), gem_string("*")), gem_to_string_fn(NULL, _t2483, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2484, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2485, 2));
        } else {
#line 686 "compiler/main.gem"
    GemVal _t2486[] = {gem_v_p};
    GemVal _t2487[] = {gem_v_mp};
    GemVal _t2488[] = {gem_v_val};
    GemVal _t2489[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2486, 1), gem_to_string_fn(NULL, _t2487, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2488, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2489, 2));
        }
    }

#line 689 "compiler/main.gem"
    GemVal _t2490[] = {gem_v_p};
    GemVal _t2491[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t2490, 1), gem_string("continue;"))};
    (void)(gem_buf_push_fn(NULL, _t2491, 2));
#line 690 "compiler/main.gem"
    GemVal _t2492[] = {gem_v_b};
    GemVal _t2493 = gem_buf_str_fn(NULL, _t2492, 1);
    gem_pop_frame();
    return _t2493;
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
    GemVal _t2495[] = {gem_v__match_46};
    GemVal _t2497;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2495, 1), gem_string("table")))) {
        _t2497 = gem_eq(gem_type_fn(NULL, _t2495, 1), gem_string("table"));
    } else {
        GemVal _t2496[] = {gem_v__match_46, gem_string("tag")};
        _t2497 = gem_has_key_fn(NULL, _t2496, 2);
    }
    GemVal _t2498;
    if (!gem_truthy(_t2497)) {
        _t2498 = _t2497;
    } else {
        _t2498 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2500;
    if (!gem_truthy(_t2498)) {
        _t2500 = _t2498;
    } else {
        GemVal _t2499[] = {gem_v__match_46, gem_string("value")};
        _t2500 = gem_has_key_fn(NULL, _t2499, 2);
    }
    if (gem_truthy(_t2500)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 698 "compiler/main.gem"
    GemVal _t2501 = gem_table_new();
    GemVal _t2502[] = {gem_v_value};
    gem_table_set(_t2501, gem_string("expr"), gem_add(gem_add(gem_string("gem_int("), gem_to_string_fn(NULL, _t2502, 1)), gem_string(")")));
    gem_table_set(_t2501, gem_string("setup"), gem_string(""));
        GemVal _t2503 = _t2501;
        gem_pop_frame();
        return _t2503;
    } else {
    GemVal _t2504[] = {gem_v__match_46};
    GemVal _t2506;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2504, 1), gem_string("table")))) {
        _t2506 = gem_eq(gem_type_fn(NULL, _t2504, 1), gem_string("table"));
    } else {
        GemVal _t2505[] = {gem_v__match_46, gem_string("tag")};
        _t2506 = gem_has_key_fn(NULL, _t2505, 2);
    }
    GemVal _t2507;
    if (!gem_truthy(_t2506)) {
        _t2507 = _t2506;
    } else {
        _t2507 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2509;
    if (!gem_truthy(_t2507)) {
        _t2509 = _t2507;
    } else {
        GemVal _t2508[] = {gem_v__match_46, gem_string("value")};
        _t2509 = gem_has_key_fn(NULL, _t2508, 2);
    }
    if (gem_truthy(_t2509)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 700 "compiler/main.gem"
    GemVal _t2510 = gem_table_new();
    GemVal _t2511[] = {gem_v_value};
    GemVal _t2512[] = {gem_fn__mod_codegen_format_float(NULL, _t2511, 1)};
    gem_table_set(_t2510, gem_string("expr"), gem_add(gem_add(gem_string("gem_float("), gem_to_string_fn(NULL, _t2512, 1)), gem_string(")")));
    gem_table_set(_t2510, gem_string("setup"), gem_string(""));
        GemVal _t2513 = _t2510;
        gem_pop_frame();
        return _t2513;
    } else {
    GemVal _t2514[] = {gem_v__match_46};
    GemVal _t2516;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2514, 1), gem_string("table")))) {
        _t2516 = gem_eq(gem_type_fn(NULL, _t2514, 1), gem_string("table"));
    } else {
        GemVal _t2515[] = {gem_v__match_46, gem_string("tag")};
        _t2516 = gem_has_key_fn(NULL, _t2515, 2);
    }
    GemVal _t2517;
    if (!gem_truthy(_t2516)) {
        _t2517 = _t2516;
    } else {
        _t2517 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2519;
    if (!gem_truthy(_t2517)) {
        _t2519 = _t2517;
    } else {
        GemVal _t2518[] = {gem_v__match_46, gem_string("value")};
        _t2519 = gem_has_key_fn(NULL, _t2518, 2);
    }
    if (gem_truthy(_t2519)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 702 "compiler/main.gem"
    GemVal _t2520[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2520, 1);
#line 703 "compiler/main.gem"
    GemVal _t2521 = gem_table_new();
    GemVal _t2522[] = {gem_v_escaped};
    gem_table_set(_t2521, gem_string("expr"), gem_add(gem_add(gem_string("gem_string(\""), gem_to_string_fn(NULL, _t2522, 1)), gem_string("\")")));
    gem_table_set(_t2521, gem_string("setup"), gem_string(""));
        GemVal _t2523 = _t2521;
        gem_pop_frame();
        return _t2523;
    } else {
    GemVal _t2524[] = {gem_v__match_46};
    GemVal _t2526;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2524, 1), gem_string("table")))) {
        _t2526 = gem_eq(gem_type_fn(NULL, _t2524, 1), gem_string("table"));
    } else {
        GemVal _t2525[] = {gem_v__match_46, gem_string("tag")};
        _t2526 = gem_has_key_fn(NULL, _t2525, 2);
    }
    GemVal _t2527;
    if (!gem_truthy(_t2526)) {
        _t2527 = _t2526;
    } else {
        _t2527 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2529;
    if (!gem_truthy(_t2527)) {
        _t2529 = _t2527;
    } else {
        GemVal _t2528[] = {gem_v__match_46, gem_string("parts")};
        _t2529 = gem_has_key_fn(NULL, _t2528, 2);
    }
    if (gem_truthy(_t2529)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_46, gem_string("parts"));
#line 705 "compiler/main.gem"
    GemVal _t2530[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2530, 1), gem_int(0)))) {
#line 706 "compiler/main.gem"
    GemVal _t2531 = gem_table_new();
    gem_table_set(_t2531, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2531, gem_string("setup"), gem_string(""));
            GemVal _t2532 = _t2531;
            gem_pop_frame();
            return _t2532;
        }
#line 708 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 709 "compiler/main.gem"
    GemVal _t2533 = gem_table_new();
        GemVal gem_v_compiled = _t2533;
#line 710 "compiler/main.gem"
        GemVal gem_v__for_i_47 = gem_int(0);
#line 710 "compiler/main.gem"
    GemVal _t2534[] = {gem_v_parts};
        GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2534, 1);
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
    GemVal _t2535[] = {gem_v_part};
    GemVal _t2536 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2536.fn(_t2536.env, _t2535, 1);
#line 713 "compiler/main.gem"
    GemVal _t2537 = gem_v_r;
            gem_v_setup = gem_add(gem_v_setup, gem_table_get(_t2537, gem_string("setup")));
#line 714 "compiler/main.gem"
    GemVal _t2538 = gem_v_part;
            if (gem_truthy(gem_eq(gem_table_get(_t2538, gem_string("tag")), gem_string("string")))) {
#line 715 "compiler/main.gem"
    GemVal _t2539 = gem_v_r;
    GemVal _t2540[] = {gem_v_compiled, gem_table_get(_t2539, gem_string("expr"))};
                (void)(gem_push_fn(NULL, _t2540, 2));
            } else {
#line 717 "compiler/main.gem"
    GemVal _t2541 = (*gem_v_tmp);
                GemVal gem_v_ts = _t2541.fn(_t2541.env, NULL, 0);
#line 718 "compiler/main.gem"
    GemVal _t2542[] = {gem_v_ts};
    GemVal _t2543 = gem_v_r;
    GemVal _t2544[] = {gem_table_get(_t2543, gem_string("expr"))};
                gem_v_setup = gem_add(gem_v_setup, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2542, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2544, 1)), gem_string("};\n")));
#line 719 "compiler/main.gem"
    GemVal _t2545[] = {gem_v_ts};
    GemVal _t2546[] = {gem_v_compiled, gem_add(gem_add(gem_string("gem_to_string_fn(NULL, "), gem_to_string_fn(NULL, _t2545, 1)), gem_string(", 1)"))};
                (void)(gem_push_fn(NULL, _t2546, 2));
            }
        }

#line 722 "compiler/main.gem"
        GemVal gem_v_acc = gem_table_get(gem_v_compiled, gem_int(0));
#line 723 "compiler/main.gem"
        GemVal gem_v__for_i_48 = gem_int(1);
#line 723 "compiler/main.gem"
    GemVal _t2547[] = {gem_v_compiled};
        GemVal gem_v__for_limit_48 = gem_len_fn(NULL, _t2547, 1);
#line 723 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_48, gem_v__for_limit_48))) break;
#line 723 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_48;
#line 723 "compiler/main.gem"
            gem_v__for_i_48 = gem_add(gem_v__for_i_48, gem_int(1));
#line 724 "compiler/main.gem"
    GemVal _t2548[] = {gem_v_acc};
    GemVal _t2549[] = {gem_table_get(gem_v_compiled, gem_v_i)};
            gem_v_acc = gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2548, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2549, 1)), gem_string(")"));
        }

#line 726 "compiler/main.gem"
    GemVal _t2550 = gem_table_new();
    gem_table_set(_t2550, gem_string("expr"), gem_v_acc);
    gem_table_set(_t2550, gem_string("setup"), gem_v_setup);
        GemVal _t2551 = _t2550;
        gem_pop_frame();
        return _t2551;
    } else {
    GemVal _t2552[] = {gem_v__match_46};
    GemVal _t2554;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2552, 1), gem_string("table")))) {
        _t2554 = gem_eq(gem_type_fn(NULL, _t2552, 1), gem_string("table"));
    } else {
        GemVal _t2553[] = {gem_v__match_46, gem_string("tag")};
        _t2554 = gem_has_key_fn(NULL, _t2553, 2);
    }
    GemVal _t2555;
    if (!gem_truthy(_t2554)) {
        _t2555 = _t2554;
    } else {
        _t2555 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2557;
    if (!gem_truthy(_t2555)) {
        _t2557 = _t2555;
    } else {
        GemVal _t2556[] = {gem_v__match_46, gem_string("value")};
        _t2557 = gem_has_key_fn(NULL, _t2556, 2);
    }
    if (gem_truthy(_t2557)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_46, gem_string("value"));
#line 728 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 729 "compiler/main.gem"
    GemVal _t2558 = gem_table_new();
    gem_table_set(_t2558, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2558, gem_string("setup"), gem_string(""));
            GemVal _t2559 = _t2558;
            gem_pop_frame();
            return _t2559;
        }
#line 731 "compiler/main.gem"
    GemVal _t2560 = gem_table_new();
    gem_table_set(_t2560, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2560, gem_string("setup"), gem_string(""));
        GemVal _t2561 = _t2560;
        gem_pop_frame();
        return _t2561;
    } else {
    GemVal _t2562[] = {gem_v__match_46};
    GemVal _t2564;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2562, 1), gem_string("table")))) {
        _t2564 = gem_eq(gem_type_fn(NULL, _t2562, 1), gem_string("table"));
    } else {
        GemVal _t2563[] = {gem_v__match_46, gem_string("tag")};
        _t2564 = gem_has_key_fn(NULL, _t2563, 2);
    }
    GemVal _t2565;
    if (!gem_truthy(_t2564)) {
        _t2565 = _t2564;
    } else {
        _t2565 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2565)) {
#line 733 "compiler/main.gem"
    GemVal _t2566 = gem_table_new();
    gem_table_set(_t2566, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2566, gem_string("setup"), gem_string(""));
        GemVal _t2567 = _t2566;
        gem_pop_frame();
        return _t2567;
    } else {
    GemVal _t2568[] = {gem_v__match_46};
    GemVal _t2570;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2568, 1), gem_string("table")))) {
        _t2570 = gem_eq(gem_type_fn(NULL, _t2568, 1), gem_string("table"));
    } else {
        GemVal _t2569[] = {gem_v__match_46, gem_string("tag")};
        _t2570 = gem_has_key_fn(NULL, _t2569, 2);
    }
    GemVal _t2571;
    if (!gem_truthy(_t2570)) {
        _t2571 = _t2570;
    } else {
        _t2571 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2573;
    if (!gem_truthy(_t2571)) {
        _t2573 = _t2571;
    } else {
        GemVal _t2572[] = {gem_v__match_46, gem_string("name")};
        _t2573 = gem_has_key_fn(NULL, _t2572, 2);
    }
    if (gem_truthy(_t2573)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_46, gem_string("name"));
#line 735 "compiler/main.gem"
    GemVal _t2574[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2574, 2)))) {
#line 736 "compiler/main.gem"
    GemVal _t2575[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2575, 2))) {
#line 737 "compiler/main.gem"
    GemVal _t2576 = gem_table_new();
    GemVal _t2577[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name)};
    gem_table_set(_t2576, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2577, 1)), gem_string(", NULL)")));
    gem_table_set(_t2576, gem_string("setup"), gem_string(""));
                GemVal _t2578 = _t2576;
                gem_pop_frame();
                return _t2578;
            } else {
#line 738 "compiler/main.gem"
    GemVal _t2579[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2579, 2))) {
#line 739 "compiler/main.gem"
    GemVal _t2580 = gem_table_new();
    GemVal _t2581[] = {gem_v_name};
    gem_table_set(_t2580, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn(gem_fn_"), gem_to_string_fn(NULL, _t2581, 1)), gem_string(", NULL)")));
    gem_table_set(_t2580, gem_string("setup"), gem_string(""));
                    GemVal _t2582 = _t2580;
                    gem_pop_frame();
                    return _t2582;
                }
            }
        }
#line 742 "compiler/main.gem"
    GemVal _t2583[] = {gem_v_name};
    GemVal _t2584 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2584.fn(_t2584.env, _t2583, 1);
#line 743 "compiler/main.gem"
    GemVal _t2585[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2587;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2585, 2))) {
        _t2587 = gem_fn__mod_codegen_set_contains(NULL, _t2585, 2);
    } else {
        GemVal _t2586[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t2587 = gem_fn__mod_codegen_set_contains(NULL, _t2586, 2);
    }
        if (gem_truthy(_t2587)) {
#line 744 "compiler/main.gem"
    GemVal _t2588 = gem_table_new();
    GemVal _t2589[] = {gem_v_mname};
    gem_table_set(_t2588, gem_string("expr"), gem_add(gem_add(gem_string("(*"), gem_to_string_fn(NULL, _t2589, 1)), gem_string(")")));
    gem_table_set(_t2588, gem_string("setup"), gem_string(""));
            GemVal _t2590 = _t2588;
            gem_pop_frame();
            return _t2590;
        }
#line 746 "compiler/main.gem"
    GemVal _t2591 = gem_table_new();
    gem_table_set(_t2591, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2591, gem_string("setup"), gem_string(""));
        GemVal _t2592 = _t2591;
        gem_pop_frame();
        return _t2592;
    } else {
    GemVal _t2593[] = {gem_v__match_46};
    GemVal _t2595;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2593, 1), gem_string("table")))) {
        _t2595 = gem_eq(gem_type_fn(NULL, _t2593, 1), gem_string("table"));
    } else {
        GemVal _t2594[] = {gem_v__match_46, gem_string("tag")};
        _t2595 = gem_has_key_fn(NULL, _t2594, 2);
    }
    GemVal _t2596;
    if (!gem_truthy(_t2595)) {
        _t2596 = _t2595;
    } else {
        _t2596 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2596)) {
#line 748 "compiler/main.gem"
    GemVal _t2597[] = {gem_v_node};
    GemVal _t2598 = (*gem_v_compile_call);
        GemVal _t2599 = _t2598.fn(_t2598.env, _t2597, 1);
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
        _t2603 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2603)) {
#line 750 "compiler/main.gem"
    GemVal _t2604[] = {gem_v_node};
    GemVal _t2605 = (*gem_v_compile_binop);
        GemVal _t2606 = _t2605.fn(_t2605.env, _t2604, 1);
        gem_pop_frame();
        return _t2606;
    } else {
    GemVal _t2607[] = {gem_v__match_46};
    GemVal _t2609;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2607, 1), gem_string("table")))) {
        _t2609 = gem_eq(gem_type_fn(NULL, _t2607, 1), gem_string("table"));
    } else {
        GemVal _t2608[] = {gem_v__match_46, gem_string("tag")};
        _t2609 = gem_has_key_fn(NULL, _t2608, 2);
    }
    GemVal _t2610;
    if (!gem_truthy(_t2609)) {
        _t2610 = _t2609;
    } else {
        _t2610 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2612;
    if (!gem_truthy(_t2610)) {
        _t2612 = _t2610;
    } else {
        GemVal _t2611[] = {gem_v__match_46, gem_string("expr")};
        _t2612 = gem_has_key_fn(NULL, _t2611, 2);
    }
    GemVal _t2614;
    if (!gem_truthy(_t2612)) {
        _t2614 = _t2612;
    } else {
        GemVal _t2613[] = {gem_v__match_46, gem_string("op")};
        _t2614 = gem_has_key_fn(NULL, _t2613, 2);
    }
    if (gem_truthy(_t2614)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_46, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_46, gem_string("op"));
#line 752 "compiler/main.gem"
    GemVal _t2615[] = {gem_v_uexpr};
    GemVal _t2616 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2616.fn(_t2616.env, _t2615, 1);
#line 753 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 754 "compiler/main.gem"
    GemVal _t2617 = gem_table_new();
    GemVal _t2618 = gem_v_r;
    GemVal _t2619[] = {gem_table_get(_t2618, gem_string("expr"))};
    gem_table_set(_t2617, gem_string("expr"), gem_add(gem_add(gem_string("gem_neg("), gem_to_string_fn(NULL, _t2619, 1)), gem_string(")")));
    GemVal _t2620 = gem_v_r;
    gem_table_set(_t2617, gem_string("setup"), gem_table_get(_t2620, gem_string("setup")));
            GemVal _t2621 = _t2617;
            gem_pop_frame();
            return _t2621;
        } else {
#line 755 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 756 "compiler/main.gem"
    GemVal _t2622 = gem_table_new();
    GemVal _t2623 = gem_v_r;
    GemVal _t2624[] = {gem_table_get(_t2623, gem_string("expr"))};
    gem_table_set(_t2622, gem_string("expr"), gem_add(gem_add(gem_string("gem_not("), gem_to_string_fn(NULL, _t2624, 1)), gem_string(")")));
    GemVal _t2625 = gem_v_r;
    gem_table_set(_t2622, gem_string("setup"), gem_table_get(_t2625, gem_string("setup")));
                GemVal _t2626 = _t2622;
                gem_pop_frame();
                return _t2626;
            }
        }
#line 758 "compiler/main.gem"
    GemVal _t2627[] = {gem_v_op};
    GemVal _t2628[] = {gem_add(gem_string("unknown unary operator: "), gem_to_string_fn(NULL, _t2627, 1))};
        (void)(gem_error_at_fn("compiler/main.gem", 758, _t2628, 1));
    } else {
    GemVal _t2629[] = {gem_v__match_46};
    GemVal _t2631;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2629, 1), gem_string("table")))) {
        _t2631 = gem_eq(gem_type_fn(NULL, _t2629, 1), gem_string("table"));
    } else {
        GemVal _t2630[] = {gem_v__match_46, gem_string("tag")};
        _t2631 = gem_has_key_fn(NULL, _t2630, 2);
    }
    GemVal _t2632;
    if (!gem_truthy(_t2631)) {
        _t2632 = _t2631;
    } else {
        _t2632 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2632)) {
#line 760 "compiler/main.gem"
    GemVal _t2633[] = {gem_v_node};
    GemVal _t2634 = (*gem_v_compile_anon_fn);
        GemVal _t2635 = _t2634.fn(_t2634.env, _t2633, 1);
        gem_pop_frame();
        return _t2635;
    } else {
    GemVal _t2636[] = {gem_v__match_46};
    GemVal _t2638;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2636, 1), gem_string("table")))) {
        _t2638 = gem_eq(gem_type_fn(NULL, _t2636, 1), gem_string("table"));
    } else {
        GemVal _t2637[] = {gem_v__match_46, gem_string("tag")};
        _t2638 = gem_has_key_fn(NULL, _t2637, 2);
    }
    GemVal _t2639;
    if (!gem_truthy(_t2638)) {
        _t2639 = _t2638;
    } else {
        _t2639 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2639)) {
#line 762 "compiler/main.gem"
    GemVal _t2640[] = {gem_v_node};
    GemVal _t2641 = (*gem_v_compile_table);
        GemVal _t2642 = _t2641.fn(_t2641.env, _t2640, 1);
        gem_pop_frame();
        return _t2642;
    } else {
    GemVal _t2643[] = {gem_v__match_46};
    GemVal _t2645;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2643, 1), gem_string("table")))) {
        _t2645 = gem_eq(gem_type_fn(NULL, _t2643, 1), gem_string("table"));
    } else {
        GemVal _t2644[] = {gem_v__match_46, gem_string("tag")};
        _t2645 = gem_has_key_fn(NULL, _t2644, 2);
    }
    GemVal _t2646;
    if (!gem_truthy(_t2645)) {
        _t2646 = _t2645;
    } else {
        _t2646 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2646)) {
#line 764 "compiler/main.gem"
    GemVal _t2647[] = {gem_v_node};
    GemVal _t2648 = (*gem_v_compile_array);
        GemVal _t2649 = _t2648.fn(_t2648.env, _t2647, 1);
        gem_pop_frame();
        return _t2649;
    } else {
    GemVal _t2650[] = {gem_v__match_46};
    GemVal _t2652;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2650, 1), gem_string("table")))) {
        _t2652 = gem_eq(gem_type_fn(NULL, _t2650, 1), gem_string("table"));
    } else {
        GemVal _t2651[] = {gem_v__match_46, gem_string("tag")};
        _t2652 = gem_has_key_fn(NULL, _t2651, 2);
    }
    GemVal _t2653;
    if (!gem_truthy(_t2652)) {
        _t2653 = _t2652;
    } else {
        _t2653 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2655;
    if (!gem_truthy(_t2653)) {
        _t2655 = _t2653;
    } else {
        GemVal _t2654[] = {gem_v__match_46, gem_string("object")};
        _t2655 = gem_has_key_fn(NULL, _t2654, 2);
    }
    GemVal _t2657;
    if (!gem_truthy(_t2655)) {
        _t2657 = _t2655;
    } else {
        GemVal _t2656[] = {gem_v__match_46, gem_string("field")};
        _t2657 = gem_has_key_fn(NULL, _t2656, 2);
    }
    if (gem_truthy(_t2657)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_46, gem_string("field"));
#line 766 "compiler/main.gem"
    GemVal _t2658[] = {gem_v_object};
    GemVal _t2659 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2659.fn(_t2659.env, _t2658, 1);
#line 767 "compiler/main.gem"
    GemVal _t2660 = (*gem_v_tmp);
        GemVal gem_v_t = _t2660.fn(_t2660.env, NULL, 0);
#line 768 "compiler/main.gem"
    GemVal _t2661 = gem_v_r;
    GemVal _t2662[] = {gem_v_t};
    GemVal _t2663 = gem_v_r;
    GemVal _t2664[] = {gem_table_get(_t2663, gem_string("expr"))};
        GemVal gem_v_setup = gem_add(gem_table_get(_t2661, gem_string("setup")), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2662, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2664, 1)), gem_string(";\n")));
#line 769 "compiler/main.gem"
    GemVal _t2665[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2665, 1);
#line 770 "compiler/main.gem"
    GemVal _t2666 = gem_table_new();
    GemVal _t2667[] = {gem_v_t};
    GemVal _t2668[] = {gem_v_escaped};
    gem_table_set(_t2666, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2667, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2668, 1)), gem_string("\"))")));
    gem_table_set(_t2666, gem_string("setup"), gem_v_setup);
        GemVal _t2669 = _t2666;
        gem_pop_frame();
        return _t2669;
    } else {
    GemVal _t2670[] = {gem_v__match_46};
    GemVal _t2672;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2670, 1), gem_string("table")))) {
        _t2672 = gem_eq(gem_type_fn(NULL, _t2670, 1), gem_string("table"));
    } else {
        GemVal _t2671[] = {gem_v__match_46, gem_string("tag")};
        _t2672 = gem_has_key_fn(NULL, _t2671, 2);
    }
    GemVal _t2673;
    if (!gem_truthy(_t2672)) {
        _t2673 = _t2672;
    } else {
        _t2673 = gem_eq(gem_table_get(gem_v__match_46, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2675;
    if (!gem_truthy(_t2673)) {
        _t2675 = _t2673;
    } else {
        GemVal _t2674[] = {gem_v__match_46, gem_string("object")};
        _t2675 = gem_has_key_fn(NULL, _t2674, 2);
    }
    GemVal _t2677;
    if (!gem_truthy(_t2675)) {
        _t2677 = _t2675;
    } else {
        GemVal _t2676[] = {gem_v__match_46, gem_string("key")};
        _t2677 = gem_has_key_fn(NULL, _t2676, 2);
    }
    if (gem_truthy(_t2677)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_46, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_46, gem_string("key"));
#line 772 "compiler/main.gem"
    GemVal _t2678[] = {gem_v_object};
    GemVal _t2679 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2679.fn(_t2679.env, _t2678, 1);
#line 773 "compiler/main.gem"
    GemVal _t2680[] = {gem_v_key};
    GemVal _t2681 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2681.fn(_t2681.env, _t2680, 1);
#line 774 "compiler/main.gem"
    GemVal _t2682 = gem_table_new();
    GemVal _t2683 = gem_v_obj_r;
    GemVal _t2684[] = {gem_table_get(_t2683, gem_string("expr"))};
    GemVal _t2685 = gem_v_key_r;
    GemVal _t2686[] = {gem_table_get(_t2685, gem_string("expr"))};
    gem_table_set(_t2682, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_table_get("), gem_to_string_fn(NULL, _t2684, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2686, 1)), gem_string(")")));
    GemVal _t2687 = gem_v_obj_r;
    GemVal _t2688 = gem_v_key_r;
    gem_table_set(_t2682, gem_string("setup"), gem_add(gem_table_get(_t2687, gem_string("setup")), gem_table_get(_t2688, gem_string("setup"))));
        GemVal _t2689 = _t2682;
        gem_pop_frame();
        return _t2689;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 777 "compiler/main.gem"
    GemVal _t2690 = gem_v_node;
    GemVal _t2691[] = {gem_table_get(_t2690, gem_string("tag"))};
    GemVal _t2692[] = {gem_add(gem_string("unknown expression node: "), gem_to_string_fn(NULL, _t2691, 1))};
    GemVal _t2693 = gem_error_at_fn("compiler/main.gem", 777, _t2692, 1);
    gem_pop_frame();
    return _t2693;
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
#line 783 "compiler/main.gem"
    GemVal _t2695 = gem_v_node;
    GemVal gem_v_entries = gem_table_get(_t2695, gem_string("entries"));
#line 784 "compiler/main.gem"
    GemVal _t2696 = (*gem_v_tmp);
    GemVal gem_v_t = _t2696.fn(_t2696.env, NULL, 0);
#line 785 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 786 "compiler/main.gem"
    GemVal _t2697[] = {gem_v_t};
    GemVal _t2698[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2697, 1)), gem_string(" = gem_table_new();\n"))};
    (void)(gem_buf_push_fn(NULL, _t2698, 2));
#line 787 "compiler/main.gem"
    GemVal gem_v__for_items_49 = gem_v_entries;
#line 787 "compiler/main.gem"
    GemVal gem_v__for_i_49 = gem_int(0);
#line 787 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2699[] = {gem_v__for_items_49};
        if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_len_fn(NULL, _t2699, 1)))) break;
#line 787 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_49, gem_v__for_i_49);
#line 787 "compiler/main.gem"
        gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 788 "compiler/main.gem"
    GemVal _t2700 = gem_v_entry;
    GemVal _t2701[] = {gem_table_get(_t2700, gem_string("value"))};
    GemVal _t2702 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2702.fn(_t2702.env, _t2701, 1);
#line 789 "compiler/main.gem"
    GemVal _t2703 = gem_v_entry;
    GemVal _t2704[] = {gem_table_get(_t2703, gem_string("key"))};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2704, 1);
#line 790 "compiler/main.gem"
    GemVal _t2705 = gem_v_val_r;
    GemVal _t2706[] = {gem_v_b, gem_table_get(_t2705, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2706, 2));
#line 791 "compiler/main.gem"
    GemVal _t2707[] = {gem_v_t};
    GemVal _t2708[] = {gem_v_escaped};
    GemVal _t2709 = gem_v_val_r;
    GemVal _t2710[] = {gem_table_get(_t2709, gem_string("expr"))};
    GemVal _t2711[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2707, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t2708, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t2710, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t2711, 2));
    }

    GemVal _t2712 = gem_table_new();
    gem_table_set(_t2712, gem_string("expr"), gem_v_t);
    GemVal _t2713[] = {gem_v_b};
    gem_table_set(_t2712, gem_string("setup"), gem_buf_str_fn(NULL, _t2713, 1));
    GemVal _t2714 = _t2712;
    gem_pop_frame();
    return _t2714;
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
#line 799 "compiler/main.gem"
    GemVal _t2716 = gem_v_node;
    GemVal gem_v_elements = gem_table_get(_t2716, gem_string("elements"));
#line 800 "compiler/main.gem"
    GemVal _t2717 = (*gem_v_tmp);
    GemVal gem_v_t = _t2717.fn(_t2717.env, NULL, 0);
#line 801 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 802 "compiler/main.gem"
    GemVal _t2718[] = {gem_v_t};
    GemVal _t2719[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2718, 1)), gem_string(" = gem_table_new();\n"))};
    (void)(gem_buf_push_fn(NULL, _t2719, 2));
#line 803 "compiler/main.gem"
    GemVal gem_v__for_i_50 = gem_int(0);
#line 803 "compiler/main.gem"
    GemVal _t2720[] = {gem_v_elements};
    GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2720, 1);
#line 803 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 803 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_50;
#line 803 "compiler/main.gem"
        gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 804 "compiler/main.gem"
    GemVal _t2721[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2722 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2722.fn(_t2722.env, _t2721, 1);
#line 805 "compiler/main.gem"
    GemVal _t2723 = gem_v_elem_r;
    GemVal _t2724[] = {gem_v_b, gem_table_get(_t2723, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2724, 2));
#line 806 "compiler/main.gem"
    GemVal _t2725[] = {gem_v_t};
    GemVal _t2726[] = {gem_v_i};
    GemVal _t2727 = gem_v_elem_r;
    GemVal _t2728[] = {gem_table_get(_t2727, gem_string("expr"))};
    GemVal _t2729[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_table_set("), gem_to_string_fn(NULL, _t2725, 1)), gem_string(", gem_int(")), gem_to_string_fn(NULL, _t2726, 1)), gem_string("), ")), gem_to_string_fn(NULL, _t2728, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t2729, 2));
    }

    GemVal _t2730 = gem_table_new();
    gem_table_set(_t2730, gem_string("expr"), gem_v_t);
    GemVal _t2731[] = {gem_v_b};
    gem_table_set(_t2730, gem_string("setup"), gem_buf_str_fn(NULL, _t2731, 1));
    GemVal _t2732 = _t2730;
    gem_pop_frame();
    return _t2732;
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
#line 814 "compiler/main.gem"
    GemVal _t2734 = gem_v_node;
    GemVal _t2735[] = {gem_table_get(_t2734, gem_string("params"))};
    GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2735, 1);
#line 815 "compiler/main.gem"
    GemVal _t2736 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t2736, gem_string("rest_param")), GEM_NIL))) {
#line 816 "compiler/main.gem"
    GemVal _t2737 = gem_v_node;
    GemVal _t2738[] = {gem_v_inner_defined, gem_table_get(_t2737, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t2738, 2));
    }
#line 818 "compiler/main.gem"
    GemVal _t2739 = gem_v_node;
    GemVal _t2740[] = {gem_table_get(_t2739, gem_string("body")), gem_v_inner_defined};
    GemVal _t2741 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2741.fn(_t2741.env, _t2740, 2);
#line 819 "compiler/main.gem"
    GemVal _t2742[] = {gem_v_free};
    GemVal _t2743[] = {gem_keys_fn(NULL, _t2742, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2743, 1);
#line 821 "compiler/main.gem"
    GemVal _t2744[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2744, 1), gem_int(0)))) {
#line 822 "compiler/main.gem"
    GemVal _t2745 = gem_table_new();
    GemVal _t2746[] = {gem_v_node, _t2745};
    GemVal _t2747 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2747.fn(_t2747.env, _t2746, 2);
#line 823 "compiler/main.gem"
    GemVal _t2748 = gem_table_new();
    GemVal _t2749 = gem_v_result;
    GemVal _t2750[] = {gem_table_get(_t2749, gem_string("fn_name"))};
    gem_table_set(_t2748, gem_string("expr"), gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2750, 1)), gem_string(", NULL)")));
    gem_table_set(_t2748, gem_string("setup"), gem_string(""));
        GemVal _t2751 = _t2748;
        gem_pop_frame();
        return _t2751;
    }
#line 826 "compiler/main.gem"
    GemVal _t2752[] = {gem_v_node, gem_v_captures};
    GemVal _t2753 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2753.fn(_t2753.env, _t2752, 2);
#line 828 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 829 "compiler/main.gem"
    GemVal _t2754 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2754.fn(_t2754.env, NULL, 0);
#line 830 "compiler/main.gem"
    GemVal _t2755 = gem_v_result;
    GemVal _t2756[] = {gem_table_get(_t2755, gem_string("struct_name"))};
    GemVal _t2757[] = {gem_v_env_tmp};
    GemVal _t2758 = gem_v_result;
    GemVal _t2759[] = {gem_table_get(_t2758, gem_string("struct_name"))};
    GemVal _t2760[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t2756, 1)), gem_string(" *")), gem_to_string_fn(NULL, _t2757, 1)), gem_string(" = GC_MALLOC(sizeof(struct ")), gem_to_string_fn(NULL, _t2759, 1)), gem_string("));\n"))};
    (void)(gem_buf_push_fn(NULL, _t2760, 2));
#line 831 "compiler/main.gem"
    GemVal gem_v__for_items_51 = gem_v_captures;
#line 831 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 831 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2761[] = {gem_v__for_items_51};
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t2761, 1)))) break;
#line 831 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 831 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 832 "compiler/main.gem"
    GemVal _t2762[] = {gem_v_cap};
    GemVal _t2763 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2763.fn(_t2763.env, _t2762, 1);
#line 833 "compiler/main.gem"
    GemVal _t2764[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2764, 2))) {
#line 834 "compiler/main.gem"
    GemVal _t2765[] = {gem_v_env_tmp};
    GemVal _t2766[] = {gem_v_mc};
    GemVal _t2767[] = {gem_v_mc};
    GemVal _t2768[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2765, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2766, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2767, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2768, 2));
        } else {
#line 836 "compiler/main.gem"
    GemVal _t2769[] = {gem_v_env_tmp};
    GemVal _t2770[] = {gem_v_mc};
    GemVal _t2771[] = {gem_v_mc};
    GemVal _t2772[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t2769, 1)), gem_string("->")), gem_to_string_fn(NULL, _t2770, 1)), gem_string(" = &")), gem_to_string_fn(NULL, _t2771, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t2772, 2));
        }
    }

    GemVal _t2773 = gem_table_new();
    GemVal _t2774 = gem_v_result;
    GemVal _t2775[] = {gem_table_get(_t2774, gem_string("fn_name"))};
    GemVal _t2776[] = {gem_v_env_tmp};
    gem_table_set(_t2773, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_make_fn("), gem_to_string_fn(NULL, _t2775, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2776, 1)), gem_string(")")));
    GemVal _t2777[] = {gem_v_b};
    gem_table_set(_t2773, gem_string("setup"), gem_buf_str_fn(NULL, _t2777, 1));
    GemVal _t2778 = _t2773;
    gem_pop_frame();
    return _t2778;
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
#line 846 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 847 "compiler/main.gem"
    GemVal _t2780 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2780;
#line 848 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 848 "compiler/main.gem"
    GemVal _t2781[] = {gem_v_args};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2781, 1);
#line 848 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 848 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 848 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 849 "compiler/main.gem"
    GemVal _t2782[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2783 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2783.fn(_t2783.env, _t2782, 1);
#line 850 "compiler/main.gem"
    GemVal _t2784 = gem_v_r;
    GemVal _t2785[] = {gem_v_b, gem_table_get(_t2784, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t2785, 2));
#line 851 "compiler/main.gem"
    GemVal _t2786 = gem_v_r;
    GemVal _t2787[] = {gem_v_arg_exprs, gem_table_get(_t2786, gem_string("expr"))};
        (void)(gem_push_fn(NULL, _t2787, 2));
    }

#line 853 "compiler/main.gem"
    GemVal _t2788[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2788, 1);
#line 854 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 855 "compiler/main.gem"
    GemVal _t2789 = gem_table_new();
    GemVal _t2790[] = {gem_v_b};
    gem_table_set(_t2789, gem_string("setup"), gem_buf_str_fn(NULL, _t2790, 1));
    gem_table_set(_t2789, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2789, gem_string("argc"), gem_int(0));
        GemVal _t2791 = _t2789;
        gem_pop_frame();
        return _t2791;
    }
#line 857 "compiler/main.gem"
    GemVal _t2792 = (*gem_v_tmp);
    GemVal gem_v_t = _t2792.fn(_t2792.env, NULL, 0);
#line 858 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 859 "compiler/main.gem"
    GemVal gem_v__for_i_53 = gem_int(1);
#line 859 "compiler/main.gem"
    GemVal gem_v__for_limit_53 = gem_v_argc;
#line 859 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_v__for_limit_53))) break;
#line 859 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_53;
#line 859 "compiler/main.gem"
        gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 860 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2793 = gem_table_new();
    GemVal _t2794[] = {gem_v_b};
    GemVal _t2795[] = {gem_v_t};
    GemVal _t2796[] = {gem_v_arr};
    gem_table_set(_t2793, gem_string("setup"), gem_add(gem_buf_str_fn(NULL, _t2794, 1), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2795, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2796, 1)), gem_string("};\n"))));
    gem_table_set(_t2793, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2793, gem_string("argc"), gem_v_argc);
    GemVal _t2797 = _t2793;
    gem_pop_frame();
    return _t2797;
}

struct _closure__anon_40 {
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 867 "compiler/main.gem"
    GemVal _t2799 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2799, gem_string("argc")), gem_int(0)))) {
#line 868 "compiler/main.gem"
    GemVal _t2800 = gem_table_new();
    GemVal _t2801[] = {gem_v_fn_name};
    GemVal _t2802[] = {gem_v_env};
    gem_table_set(_t2800, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2801, 1), gem_string("(")), gem_to_string_fn(NULL, _t2802, 1)), gem_string(", NULL, 0)")));
    GemVal _t2803 = gem_v_ca;
    gem_table_set(_t2800, gem_string("setup"), gem_table_get(_t2803, gem_string("setup")));
        GemVal _t2804 = _t2800;
        gem_pop_frame();
        return _t2804;
    }
    GemVal _t2805 = gem_table_new();
    GemVal _t2806[] = {gem_v_fn_name};
    GemVal _t2807[] = {gem_v_env};
    GemVal _t2808 = gem_v_ca;
    GemVal _t2809[] = {gem_table_get(_t2808, gem_string("arr_name"))};
    GemVal _t2810 = gem_v_ca;
    GemVal _t2811[] = {gem_table_get(_t2810, gem_string("argc"))};
    gem_table_set(_t2805, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2806, 1), gem_string("(")), gem_to_string_fn(NULL, _t2807, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2809, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2811, 1)), gem_string(")")));
    GemVal _t2812 = gem_v_ca;
    gem_table_set(_t2805, gem_string("setup"), gem_table_get(_t2812, gem_string("setup")));
    GemVal _t2813 = _t2805;
    gem_pop_frame();
    return _t2813;
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
#line 876 "compiler/main.gem"
    GemVal gem_v__d54 = gem_v_node;
#line 876 "compiler/main.gem"
    GemVal _t2814 = gem_v__d54;
    GemVal gem_v_func = gem_table_get(_t2814, gem_string("func"));
#line 876 "compiler/main.gem"
    GemVal _t2815 = gem_v__d54;
    GemVal gem_v_args = gem_table_get(_t2815, gem_string("args"));

#line 879 "compiler/main.gem"
    GemVal _t2816 = gem_v_func;
    GemVal _t2818;
    if (!gem_truthy(gem_eq(gem_table_get(_t2816, gem_string("tag")), gem_string("dot")))) {
        _t2818 = gem_eq(gem_table_get(_t2816, gem_string("tag")), gem_string("dot"));
    } else {
        GemVal _t2817 = gem_v_func;
        _t2818 = gem_eq(gem_table_get(_t2817, gem_string("field")), gem_string("len"));
    }
    GemVal _t2820;
    if (!gem_truthy(_t2818)) {
        _t2820 = _t2818;
    } else {
        GemVal _t2819[] = {gem_v_args};
        _t2820 = gem_eq(gem_len_fn(NULL, _t2819, 1), gem_int(0));
    }
    if (gem_truthy(_t2820)) {
#line 880 "compiler/main.gem"
    GemVal _t2821 = gem_v_func;
    GemVal _t2822[] = {gem_table_get(_t2821, gem_string("object"))};
    GemVal _t2823 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2823.fn(_t2823.env, _t2822, 1);
#line 881 "compiler/main.gem"
    GemVal _t2824 = gem_table_new();
    GemVal _t2825 = gem_v_obj_r;
    GemVal _t2826[] = {gem_table_get(_t2825, gem_string("expr"))};
    gem_table_set(_t2824, gem_string("expr"), gem_add(gem_add(gem_string("gem_len_val("), gem_to_string_fn(NULL, _t2826, 1)), gem_string(")")));
    GemVal _t2827 = gem_v_obj_r;
    gem_table_set(_t2824, gem_string("setup"), gem_table_get(_t2827, gem_string("setup")));
        GemVal _t2828 = _t2824;
        gem_pop_frame();
        return _t2828;
    }
#line 885 "compiler/main.gem"
    GemVal _t2829 = gem_v_func;
    if (gem_truthy(gem_eq(gem_table_get(_t2829, gem_string("tag")), gem_string("var")))) {
#line 886 "compiler/main.gem"
    GemVal _t2830 = gem_v_func;
        GemVal gem_v_name = gem_table_get(_t2830, gem_string("name"));
#line 888 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 889 "compiler/main.gem"
    GemVal _t2831 = gem_v_node;
            GemVal gem_v_line = gem_table_get(_t2831, gem_string("line"));
#line 890 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 891 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 893 "compiler/main.gem"
    GemVal _t2832[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn__mod_codegen_escape_c_string(NULL, _t2832, 1);
#line 894 "compiler/main.gem"
    GemVal _t2833[] = {gem_v_args};
    GemVal _t2834 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2834.fn(_t2834.env, _t2833, 1);
#line 895 "compiler/main.gem"
    GemVal _t2835 = gem_v_ca;
            if (gem_truthy(gem_eq(gem_table_get(_t2835, gem_string("argc")), gem_int(0)))) {
#line 896 "compiler/main.gem"
    GemVal _t2836 = gem_table_new();
    GemVal _t2837[] = {gem_v_escaped_file};
    GemVal _t2838[] = {gem_v_line};
    gem_table_set(_t2836, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2837, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2838, 1)), gem_string(", NULL, 0)")));
    GemVal _t2839 = gem_v_ca;
    gem_table_set(_t2836, gem_string("setup"), gem_table_get(_t2839, gem_string("setup")));
                GemVal _t2840 = _t2836;
                gem_pop_frame();
                return _t2840;
            }
#line 898 "compiler/main.gem"
    GemVal _t2841 = gem_table_new();
    GemVal _t2842[] = {gem_v_escaped_file};
    GemVal _t2843[] = {gem_v_line};
    GemVal _t2844 = gem_v_ca;
    GemVal _t2845[] = {gem_table_get(_t2844, gem_string("arr_name"))};
    GemVal _t2846 = gem_v_ca;
    GemVal _t2847[] = {gem_table_get(_t2846, gem_string("argc"))};
    gem_table_set(_t2841, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("gem_error_at_fn(\""), gem_to_string_fn(NULL, _t2842, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t2843, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2845, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2847, 1)), gem_string(")")));
    GemVal _t2848 = gem_v_ca;
    gem_table_set(_t2841, gem_string("setup"), gem_table_get(_t2848, gem_string("setup")));
            GemVal _t2849 = _t2841;
            gem_pop_frame();
            return _t2849;
        } else {
#line 899 "compiler/main.gem"
    GemVal _t2850[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2852;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2850, 2))) {
        _t2852 = gem_has_key_fn(NULL, _t2850, 2);
    } else {
        GemVal _t2851[] = {(*gem_v_local_names), gem_v_name};
        _t2852 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2851, 2));
    }
            if (gem_truthy(_t2852)) {
#line 900 "compiler/main.gem"
    GemVal _t2853[] = {gem_v_args};
    GemVal _t2854 = (*gem_v_compile_args);
    GemVal _t2855[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2854.fn(_t2854.env, _t2853, 1)};
    GemVal _t2856 = (*gem_v_emit_direct_call);
                GemVal _t2857 = _t2856.fn(_t2856.env, _t2855, 3);
                gem_pop_frame();
                return _t2857;
            } else {
#line 901 "compiler/main.gem"
    GemVal _t2858[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2860;
    if (!gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2858, 2))) {
        _t2860 = gem_fn__mod_codegen_set_contains(NULL, _t2858, 2);
    } else {
        GemVal _t2859[] = {(*gem_v_local_names), gem_v_name};
        _t2860 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2859, 2));
    }
                if (gem_truthy(_t2860)) {
#line 902 "compiler/main.gem"
    GemVal _t2861[] = {gem_v_name};
    GemVal _t2862[] = {gem_v_args};
    GemVal _t2863 = (*gem_v_compile_args);
    GemVal _t2864[] = {gem_add(gem_string("gem_fn_"), gem_to_string_fn(NULL, _t2861, 1)), gem_string("NULL"), _t2863.fn(_t2863.env, _t2862, 1)};
    GemVal _t2865 = (*gem_v_emit_direct_call);
                    GemVal _t2866 = _t2865.fn(_t2865.env, _t2864, 3);
                    gem_pop_frame();
                    return _t2866;
                }
            }
        }
    }
#line 907 "compiler/main.gem"
    GemVal _t2867[] = {gem_v_func};
    GemVal _t2868 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2868.fn(_t2868.env, _t2867, 1);
#line 908 "compiler/main.gem"
    GemVal _t2869[] = {gem_v_args};
    GemVal _t2870 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2870.fn(_t2870.env, _t2869, 1);
#line 909 "compiler/main.gem"
    GemVal _t2871 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2871.fn(_t2871.env, NULL, 0);
#line 910 "compiler/main.gem"
    GemVal _t2872 = gem_v_func_r;
    GemVal _t2873 = gem_v_ca;
    GemVal _t2874[] = {gem_v_tmp_fn};
    GemVal _t2875 = gem_v_func_r;
    GemVal _t2876[] = {gem_table_get(_t2875, gem_string("expr"))};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get(_t2872, gem_string("setup")), gem_table_get(_t2873, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2874, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t2876, 1)), gem_string(";\n")));
#line 911 "compiler/main.gem"
    GemVal _t2877 = gem_v_ca;
    if (gem_truthy(gem_eq(gem_table_get(_t2877, gem_string("argc")), gem_int(0)))) {
#line 912 "compiler/main.gem"
    GemVal _t2878 = gem_table_new();
    GemVal _t2879[] = {gem_v_tmp_fn};
    GemVal _t2880[] = {gem_v_tmp_fn};
    gem_table_set(_t2878, gem_string("expr"), gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2879, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2880, 1)), gem_string(".env, NULL, 0)")));
    gem_table_set(_t2878, gem_string("setup"), gem_v_setup);
        GemVal _t2881 = _t2878;
        gem_pop_frame();
        return _t2881;
    }
    GemVal _t2882 = gem_table_new();
    GemVal _t2883[] = {gem_v_tmp_fn};
    GemVal _t2884[] = {gem_v_tmp_fn};
    GemVal _t2885 = gem_v_ca;
    GemVal _t2886[] = {gem_table_get(_t2885, gem_string("arr_name"))};
    GemVal _t2887 = gem_v_ca;
    GemVal _t2888[] = {gem_table_get(_t2887, gem_string("argc"))};
    gem_table_set(_t2882, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t2883, 1), gem_string(".fn(")), gem_to_string_fn(NULL, _t2884, 1)), gem_string(".env, ")), gem_to_string_fn(NULL, _t2886, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2888, 1)), gem_string(")")));
    gem_table_set(_t2882, gem_string("setup"), gem_v_setup);
    GemVal _t2889 = _t2882;
    gem_pop_frame();
    return _t2889;
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
#line 920 "compiler/main.gem"
    GemVal _t2891 = gem_v_node;
    GemVal gem_v_op = gem_table_get(_t2891, gem_string("op"));
#line 921 "compiler/main.gem"
    GemVal _t2892 = gem_v_node;
    GemVal _t2893[] = {gem_table_get(_t2892, gem_string("left"))};
    GemVal _t2894 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2894.fn(_t2894.env, _t2893, 1);
#line 922 "compiler/main.gem"
    GemVal _t2895 = gem_v_lc_r;
    GemVal gem_v_lc = gem_table_get(_t2895, gem_string("expr"));
#line 923 "compiler/main.gem"
    GemVal _t2896 = gem_v_lc_r;
    GemVal gem_v_ls = gem_table_get(_t2896, gem_string("setup"));
#line 925 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 926 "compiler/main.gem"
    GemVal _t2897 = gem_v_node;
    GemVal _t2898[] = {gem_table_get(_t2897, gem_string("right"))};
    GemVal _t2899 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2899.fn(_t2899.env, _t2898, 1);
#line 927 "compiler/main.gem"
    GemVal _t2900 = gem_table_new();
    GemVal _t2901[] = {gem_v_lc};
    GemVal _t2902 = gem_v_rc_r;
    GemVal _t2903[] = {gem_table_get(_t2902, gem_string("expr"))};
    gem_table_set(_t2900, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_add("), gem_to_string_fn(NULL, _t2901, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2903, 1)), gem_string(")")));
    GemVal _t2904 = gem_v_rc_r;
    gem_table_set(_t2900, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2904, gem_string("setup"))));
        GemVal _t2905 = _t2900;
        gem_pop_frame();
        return _t2905;
    } else {
#line 928 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 929 "compiler/main.gem"
    GemVal _t2906 = gem_v_node;
    GemVal _t2907[] = {gem_table_get(_t2906, gem_string("right"))};
    GemVal _t2908 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2908.fn(_t2908.env, _t2907, 1);
#line 930 "compiler/main.gem"
    GemVal _t2909 = gem_table_new();
    GemVal _t2910[] = {gem_v_lc};
    GemVal _t2911 = gem_v_rc_r;
    GemVal _t2912[] = {gem_table_get(_t2911, gem_string("expr"))};
    gem_table_set(_t2909, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_sub("), gem_to_string_fn(NULL, _t2910, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2912, 1)), gem_string(")")));
    GemVal _t2913 = gem_v_rc_r;
    gem_table_set(_t2909, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2913, gem_string("setup"))));
            GemVal _t2914 = _t2909;
            gem_pop_frame();
            return _t2914;
        } else {
#line 931 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 932 "compiler/main.gem"
    GemVal _t2915 = gem_v_node;
    GemVal _t2916[] = {gem_table_get(_t2915, gem_string("right"))};
    GemVal _t2917 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2917.fn(_t2917.env, _t2916, 1);
#line 933 "compiler/main.gem"
    GemVal _t2918 = gem_table_new();
    GemVal _t2919[] = {gem_v_lc};
    GemVal _t2920 = gem_v_rc_r;
    GemVal _t2921[] = {gem_table_get(_t2920, gem_string("expr"))};
    gem_table_set(_t2918, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mul("), gem_to_string_fn(NULL, _t2919, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2921, 1)), gem_string(")")));
    GemVal _t2922 = gem_v_rc_r;
    gem_table_set(_t2918, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2922, gem_string("setup"))));
                GemVal _t2923 = _t2918;
                gem_pop_frame();
                return _t2923;
            } else {
#line 934 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 935 "compiler/main.gem"
    GemVal _t2924 = gem_v_node;
    GemVal _t2925[] = {gem_table_get(_t2924, gem_string("right"))};
    GemVal _t2926 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2926.fn(_t2926.env, _t2925, 1);
#line 936 "compiler/main.gem"
    GemVal _t2927 = gem_table_new();
    GemVal _t2928[] = {gem_v_lc};
    GemVal _t2929 = gem_v_rc_r;
    GemVal _t2930[] = {gem_table_get(_t2929, gem_string("expr"))};
    gem_table_set(_t2927, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_div("), gem_to_string_fn(NULL, _t2928, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2930, 1)), gem_string(")")));
    GemVal _t2931 = gem_v_rc_r;
    gem_table_set(_t2927, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2931, gem_string("setup"))));
                    GemVal _t2932 = _t2927;
                    gem_pop_frame();
                    return _t2932;
                } else {
#line 937 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 938 "compiler/main.gem"
    GemVal _t2933 = gem_v_node;
    GemVal _t2934[] = {gem_table_get(_t2933, gem_string("right"))};
    GemVal _t2935 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2935.fn(_t2935.env, _t2934, 1);
#line 939 "compiler/main.gem"
    GemVal _t2936 = gem_table_new();
    GemVal _t2937[] = {gem_v_lc};
    GemVal _t2938 = gem_v_rc_r;
    GemVal _t2939[] = {gem_table_get(_t2938, gem_string("expr"))};
    gem_table_set(_t2936, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_mod("), gem_to_string_fn(NULL, _t2937, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2939, 1)), gem_string(")")));
    GemVal _t2940 = gem_v_rc_r;
    gem_table_set(_t2936, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2940, gem_string("setup"))));
                        GemVal _t2941 = _t2936;
                        gem_pop_frame();
                        return _t2941;
                    } else {
#line 940 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 941 "compiler/main.gem"
    GemVal _t2942 = gem_v_node;
    GemVal _t2943[] = {gem_table_get(_t2942, gem_string("right"))};
    GemVal _t2944 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2944.fn(_t2944.env, _t2943, 1);
#line 942 "compiler/main.gem"
    GemVal _t2945 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2945.fn(_t2945.env, NULL, 0);
#line 943 "compiler/main.gem"
    GemVal _t2946 = gem_v_rc_r;
    GemVal _t2947[] = {gem_v_t};
    GemVal _t2948 = gem_v_rc_r;
    GemVal _t2949[] = {gem_table_get(_t2948, gem_string("expr"))};
    GemVal _t2950[] = {gem_v_lc};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get(_t2946, gem_string("setup"))), gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t2947, 1)), gem_string("[] = {")), gem_to_string_fn(NULL, _t2949, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2950, 1)), gem_string("};\n")));
#line 944 "compiler/main.gem"
    GemVal _t2951 = gem_table_new();
    GemVal _t2952[] = {gem_v_t};
    gem_table_set(_t2951, gem_string("expr"), gem_add(gem_add(gem_string("gem_in_fn(NULL, "), gem_to_string_fn(NULL, _t2952, 1)), gem_string(", 2)")));
    gem_table_set(_t2951, gem_string("setup"), gem_v_setup);
                            GemVal _t2953 = _t2951;
                            gem_pop_frame();
                            return _t2953;
                        } else {
#line 945 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 946 "compiler/main.gem"
    GemVal _t2954 = gem_v_node;
    GemVal _t2955[] = {gem_table_get(_t2954, gem_string("right"))};
    GemVal _t2956 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2956.fn(_t2956.env, _t2955, 1);
#line 947 "compiler/main.gem"
    GemVal _t2957 = gem_table_new();
    GemVal _t2958[] = {gem_v_lc};
    GemVal _t2959 = gem_v_rc_r;
    GemVal _t2960[] = {gem_table_get(_t2959, gem_string("expr"))};
    gem_table_set(_t2957, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_eq("), gem_to_string_fn(NULL, _t2958, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2960, 1)), gem_string(")")));
    GemVal _t2961 = gem_v_rc_r;
    gem_table_set(_t2957, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2961, gem_string("setup"))));
                                GemVal _t2962 = _t2957;
                                gem_pop_frame();
                                return _t2962;
                            } else {
#line 948 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 949 "compiler/main.gem"
    GemVal _t2963 = gem_v_node;
    GemVal _t2964[] = {gem_table_get(_t2963, gem_string("right"))};
    GemVal _t2965 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2965.fn(_t2965.env, _t2964, 1);
#line 950 "compiler/main.gem"
    GemVal _t2966 = gem_table_new();
    GemVal _t2967[] = {gem_v_lc};
    GemVal _t2968 = gem_v_rc_r;
    GemVal _t2969[] = {gem_table_get(_t2968, gem_string("expr"))};
    gem_table_set(_t2966, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_neq("), gem_to_string_fn(NULL, _t2967, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2969, 1)), gem_string(")")));
    GemVal _t2970 = gem_v_rc_r;
    gem_table_set(_t2966, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2970, gem_string("setup"))));
                                    GemVal _t2971 = _t2966;
                                    gem_pop_frame();
                                    return _t2971;
                                } else {
#line 951 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 952 "compiler/main.gem"
    GemVal _t2972 = gem_v_node;
    GemVal _t2973[] = {gem_table_get(_t2972, gem_string("right"))};
    GemVal _t2974 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2974.fn(_t2974.env, _t2973, 1);
#line 953 "compiler/main.gem"
    GemVal _t2975 = gem_table_new();
    GemVal _t2976[] = {gem_v_lc};
    GemVal _t2977 = gem_v_rc_r;
    GemVal _t2978[] = {gem_table_get(_t2977, gem_string("expr"))};
    gem_table_set(_t2975, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_lt("), gem_to_string_fn(NULL, _t2976, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2978, 1)), gem_string(")")));
    GemVal _t2979 = gem_v_rc_r;
    gem_table_set(_t2975, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2979, gem_string("setup"))));
                                        GemVal _t2980 = _t2975;
                                        gem_pop_frame();
                                        return _t2980;
                                    } else {
#line 954 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 955 "compiler/main.gem"
    GemVal _t2981 = gem_v_node;
    GemVal _t2982[] = {gem_table_get(_t2981, gem_string("right"))};
    GemVal _t2983 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t2983.fn(_t2983.env, _t2982, 1);
#line 956 "compiler/main.gem"
    GemVal _t2984 = gem_table_new();
    GemVal _t2985[] = {gem_v_lc};
    GemVal _t2986 = gem_v_rc_r;
    GemVal _t2987[] = {gem_table_get(_t2986, gem_string("expr"))};
    gem_table_set(_t2984, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_gt("), gem_to_string_fn(NULL, _t2985, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2987, 1)), gem_string(")")));
    GemVal _t2988 = gem_v_rc_r;
    gem_table_set(_t2984, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2988, gem_string("setup"))));
                                            GemVal _t2989 = _t2984;
                                            gem_pop_frame();
                                            return _t2989;
                                        } else {
#line 957 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 958 "compiler/main.gem"
    GemVal _t2990 = gem_v_node;
    GemVal _t2991[] = {gem_table_get(_t2990, gem_string("right"))};
    GemVal _t2992 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t2992.fn(_t2992.env, _t2991, 1);
#line 959 "compiler/main.gem"
    GemVal _t2993 = gem_table_new();
    GemVal _t2994[] = {gem_v_lc};
    GemVal _t2995 = gem_v_rc_r;
    GemVal _t2996[] = {gem_table_get(_t2995, gem_string("expr"))};
    gem_table_set(_t2993, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_le("), gem_to_string_fn(NULL, _t2994, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t2996, 1)), gem_string(")")));
    GemVal _t2997 = gem_v_rc_r;
    gem_table_set(_t2993, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t2997, gem_string("setup"))));
                                                GemVal _t2998 = _t2993;
                                                gem_pop_frame();
                                                return _t2998;
                                            } else {
#line 960 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 961 "compiler/main.gem"
    GemVal _t2999 = gem_v_node;
    GemVal _t3000[] = {gem_table_get(_t2999, gem_string("right"))};
    GemVal _t3001 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t3001.fn(_t3001.env, _t3000, 1);
#line 962 "compiler/main.gem"
    GemVal _t3002 = gem_table_new();
    GemVal _t3003[] = {gem_v_lc};
    GemVal _t3004 = gem_v_rc_r;
    GemVal _t3005[] = {gem_table_get(_t3004, gem_string("expr"))};
    gem_table_set(_t3002, gem_string("expr"), gem_add(gem_add(gem_add(gem_add(gem_string("gem_ge("), gem_to_string_fn(NULL, _t3003, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3005, 1)), gem_string(")")));
    GemVal _t3006 = gem_v_rc_r;
    gem_table_set(_t3002, gem_string("setup"), gem_add(gem_v_ls, gem_table_get(_t3006, gem_string("setup"))));
                                                    GemVal _t3007 = _t3002;
                                                    gem_pop_frame();
                                                    return _t3007;
                                                } else {
#line 963 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 964 "compiler/main.gem"
    GemVal _t3008 = gem_v_node;
    GemVal _t3009[] = {gem_table_get(_t3008, gem_string("right"))};
    GemVal _t3010 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t3010.fn(_t3010.env, _t3009, 1);
#line 965 "compiler/main.gem"
    GemVal _t3011 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t3011.fn(_t3011.env, NULL, 0);
#line 966 "compiler/main.gem"
                                                        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 967 "compiler/main.gem"
    GemVal _t3012[] = {gem_v_b, gem_v_ls};
                                                        (void)(gem_buf_push_fn(NULL, _t3012, 2));
#line 968 "compiler/main.gem"
    GemVal _t3013[] = {gem_v_t};
    GemVal _t3014[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3013, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3014, 2));
#line 969 "compiler/main.gem"
    GemVal _t3015[] = {gem_v_lc};
    GemVal _t3016[] = {gem_v_b, gem_add(gem_add(gem_string("    if (!gem_truthy("), gem_to_string_fn(NULL, _t3015, 1)), gem_string(")) {\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3016, 2));
#line 970 "compiler/main.gem"
    GemVal _t3017[] = {gem_v_t};
    GemVal _t3018[] = {gem_v_lc};
    GemVal _t3019[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3017, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3018, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3019, 2));
#line 971 "compiler/main.gem"
    GemVal _t3020[] = {gem_v_b, gem_string("    } else {\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3020, 2));
#line 972 "compiler/main.gem"
    GemVal _t3021 = gem_v_rc_r;
                                                        if (gem_truthy(gem_neq(gem_table_get(_t3021, gem_string("setup")), gem_string("")))) {
#line 973 "compiler/main.gem"
    GemVal _t3022 = gem_v_rc_r;
    GemVal _t3023[] = {gem_table_get(_t3022, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t3024[] = {gem_v_b, gem_str_replace_fn(NULL, _t3023, 3)};
                                                            (void)(gem_buf_push_fn(NULL, _t3024, 2));
                                                        }
#line 975 "compiler/main.gem"
    GemVal _t3025[] = {gem_v_t};
    GemVal _t3026 = gem_v_rc_r;
    GemVal _t3027[] = {gem_table_get(_t3026, gem_string("expr"))};
    GemVal _t3028[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3025, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3027, 1)), gem_string(";\n"))};
                                                        (void)(gem_buf_push_fn(NULL, _t3028, 2));
#line 976 "compiler/main.gem"
    GemVal _t3029[] = {gem_v_b, gem_string("    }\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3029, 2));
#line 977 "compiler/main.gem"
    GemVal _t3030 = gem_table_new();
    gem_table_set(_t3030, gem_string("expr"), gem_v_t);
    GemVal _t3031[] = {gem_v_b};
    gem_table_set(_t3030, gem_string("setup"), gem_buf_str_fn(NULL, _t3031, 1));
                                                        GemVal _t3032 = _t3030;
                                                        gem_pop_frame();
                                                        return _t3032;
                                                    } else {
#line 978 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 979 "compiler/main.gem"
    GemVal _t3033 = gem_v_node;
    GemVal _t3034[] = {gem_table_get(_t3033, gem_string("right"))};
    GemVal _t3035 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t3035.fn(_t3035.env, _t3034, 1);
#line 980 "compiler/main.gem"
    GemVal _t3036 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t3036.fn(_t3036.env, NULL, 0);
#line 981 "compiler/main.gem"
                                                            GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 982 "compiler/main.gem"
    GemVal _t3037[] = {gem_v_b, gem_v_ls};
                                                            (void)(gem_buf_push_fn(NULL, _t3037, 2));
#line 983 "compiler/main.gem"
    GemVal _t3038[] = {gem_v_t};
    GemVal _t3039[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t3038, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3039, 2));
#line 984 "compiler/main.gem"
    GemVal _t3040[] = {gem_v_lc};
    GemVal _t3041[] = {gem_v_b, gem_add(gem_add(gem_string("    if (gem_truthy("), gem_to_string_fn(NULL, _t3040, 1)), gem_string(")) {\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3041, 2));
#line 985 "compiler/main.gem"
    GemVal _t3042[] = {gem_v_t};
    GemVal _t3043[] = {gem_v_lc};
    GemVal _t3044[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3042, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3043, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3044, 2));
#line 986 "compiler/main.gem"
    GemVal _t3045[] = {gem_v_b, gem_string("    } else {\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3045, 2));
#line 987 "compiler/main.gem"
    GemVal _t3046 = gem_v_rc_r;
                                                            if (gem_truthy(gem_neq(gem_table_get(_t3046, gem_string("setup")), gem_string("")))) {
#line 988 "compiler/main.gem"
    GemVal _t3047 = gem_v_rc_r;
    GemVal _t3048[] = {gem_table_get(_t3047, gem_string("setup")), gem_string("    "), gem_string("        ")};
    GemVal _t3049[] = {gem_v_b, gem_str_replace_fn(NULL, _t3048, 3)};
                                                                (void)(gem_buf_push_fn(NULL, _t3049, 2));
                                                            }
#line 990 "compiler/main.gem"
    GemVal _t3050[] = {gem_v_t};
    GemVal _t3051 = gem_v_rc_r;
    GemVal _t3052[] = {gem_table_get(_t3051, gem_string("expr"))};
    GemVal _t3053[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t3050, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3052, 1)), gem_string(";\n"))};
                                                            (void)(gem_buf_push_fn(NULL, _t3053, 2));
#line 991 "compiler/main.gem"
    GemVal _t3054[] = {gem_v_b, gem_string("    }\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3054, 2));
#line 992 "compiler/main.gem"
    GemVal _t3055 = gem_table_new();
    gem_table_set(_t3055, gem_string("expr"), gem_v_t);
    GemVal _t3056[] = {gem_v_b};
    gem_table_set(_t3055, gem_string("setup"), gem_buf_str_fn(NULL, _t3056, 1));
                                                            GemVal _t3057 = _t3055;
                                                            gem_pop_frame();
                                                            return _t3057;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 995 "compiler/main.gem"
    GemVal _t3058[] = {gem_v_op};
    GemVal _t3059[] = {gem_add(gem_string("unknown binary operator: "), gem_to_string_fn(NULL, _t3058, 1))};
    GemVal _t3060 = gem_error_at_fn("compiler/main.gem", 995, _t3059, 1);
    gem_pop_frame();
    return _t3060;
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
#line 1001 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1002 "compiler/main.gem"
    GemVal gem_v__for_items_55 = gem_v_stmts;
#line 1002 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 1002 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3062[] = {gem_v__for_items_55};
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_len_fn(NULL, _t3062, 1)))) break;
#line 1002 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_55, gem_v__for_i_55);
#line 1002 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 1003 "compiler/main.gem"
    GemVal _t3063[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3064 = (*gem_v_compile_stmt);
    GemVal _t3065[] = {gem_v_b, _t3064.fn(_t3064.env, _t3063, 2)};
        (void)(gem_buf_push_fn(NULL, _t3065, 2));
#line 1004 "compiler/main.gem"
    GemVal _t3066[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3066, 2));
    }

#line 1006 "compiler/main.gem"
    GemVal _t3067[] = {gem_v_b};
    GemVal _t3068 = gem_buf_str_fn(NULL, _t3067, 1);
    gem_pop_frame();
    return _t3068;
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
#line 1010 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_return_pos))) {
#line 1011 "compiler/main.gem"
    GemVal _t3070[] = {gem_v_stmts, gem_v_indent};
    GemVal _t3071 = (*gem_v_compile_stmts);
        GemVal _t3072 = _t3071.fn(_t3071.env, _t3070, 2);
        gem_pop_frame();
        return _t3072;
    }
#line 1013 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1014 "compiler/main.gem"
    GemVal _t3073[] = {gem_v_stmts};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3073, 1), gem_int(0)))) {
#line 1015 "compiler/main.gem"
        GemVal gem_v__for_i_56 = gem_int(0);
#line 1015 "compiler/main.gem"
    GemVal _t3074[] = {gem_v_stmts};
        GemVal gem_v__for_limit_56 = gem_sub(gem_len_fn(NULL, _t3074, 1), gem_int(1));
#line 1015 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 1015 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_56;
#line 1015 "compiler/main.gem"
            gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 1016 "compiler/main.gem"
    GemVal _t3075[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_indent};
    GemVal _t3076 = (*gem_v_compile_stmt);
    GemVal _t3077[] = {gem_v_b, _t3076.fn(_t3076.env, _t3075, 2)};
            (void)(gem_buf_push_fn(NULL, _t3077, 2));
#line 1017 "compiler/main.gem"
    GemVal _t3078[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3078, 2));
        }

#line 1019 "compiler/main.gem"
    GemVal _t3079[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t3080 = (*gem_v_compile_stmt_return);
    GemVal _t3081[] = {gem_v_b, _t3080.fn(_t3080.env, _t3079, 2)};
        (void)(gem_buf_push_fn(NULL, _t3081, 2));
#line 1020 "compiler/main.gem"
    GemVal _t3082[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3082, 2));
    } else {
#line 1022 "compiler/main.gem"
    GemVal _t3083[] = {gem_v_indent};
        GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3083, 1);
#line 1023 "compiler/main.gem"
    GemVal _t3084[] = {gem_v_p};
    GemVal _t3085[] = {gem_v_p};
    GemVal _t3086[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3084, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3085, 1)), gem_string("return GEM_NIL;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3086, 2));
    }
#line 1025 "compiler/main.gem"
    GemVal _t3087[] = {gem_v_b};
    GemVal _t3088 = gem_buf_str_fn(NULL, _t3087, 1);
    gem_pop_frame();
    return _t3088;
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
#line 1031 "compiler/main.gem"
    GemVal _t3090[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3090, 1);
#line 1032 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1033 "compiler/main.gem"
    GemVal _t3091 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3091, gem_string("line")), GEM_NIL))) {
#line 1034 "compiler/main.gem"
    GemVal _t3092 = gem_v_node;
    GemVal _t3093[] = {gem_table_get(_t3092, gem_string("line"))};
    GemVal _t3094[] = {(*gem_v_source_name)};
    GemVal _t3095[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t3094, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3093, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3095, 1)), gem_string("\"\n"));
    }
#line 1037 "compiler/main.gem"
    GemVal gem_v__match_57 = gem_v_node;
    GemVal _t3096[] = {gem_v__match_57};
    GemVal _t3098;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3096, 1), gem_string("table")))) {
        _t3098 = gem_eq(gem_type_fn(NULL, _t3096, 1), gem_string("table"));
    } else {
        GemVal _t3097[] = {gem_v__match_57, gem_string("tag")};
        _t3098 = gem_has_key_fn(NULL, _t3097, 2);
    }
    GemVal _t3099;
    if (!gem_truthy(_t3098)) {
        _t3099 = _t3098;
    } else {
        _t3099 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3101;
    if (!gem_truthy(_t3099)) {
        _t3101 = _t3099;
    } else {
        GemVal _t3100[] = {gem_v__match_57, gem_string("name")};
        _t3101 = gem_has_key_fn(NULL, _t3100, 2);
    }
    GemVal _t3103;
    if (!gem_truthy(_t3101)) {
        _t3103 = _t3101;
    } else {
        GemVal _t3102[] = {gem_v__match_57, gem_string("value")};
        _t3103 = gem_has_key_fn(NULL, _t3102, 2);
    }
    if (gem_truthy(_t3103)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1039 "compiler/main.gem"
    GemVal _t3104[] = {gem_v_value};
    GemVal _t3105 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3105.fn(_t3105.env, _t3104, 1);
#line 1040 "compiler/main.gem"
    GemVal _t3106[] = {gem_v_name};
    GemVal _t3107 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3107.fn(_t3107.env, _t3106, 1);
#line 1041 "compiler/main.gem"
    GemVal _t3109;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t3109 = (*gem_v_in_top_level);
    } else {
        GemVal _t3108[] = {(*gem_v_top_level_vars), gem_v_name};
        _t3109 = gem_fn__mod_codegen_set_contains(NULL, _t3108, 2);
    }
        if (gem_truthy(_t3109)) {
#line 1042 "compiler/main.gem"
    GemVal _t3110[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3110, 2))) {
#line 1043 "compiler/main.gem"
    GemVal _t3111[] = {gem_v_line_dir};
    GemVal _t3112 = gem_v_r;
    GemVal _t3113[] = {gem_table_get(_t3112, gem_string("setup"))};
    GemVal _t3114[] = {gem_v_p};
    GemVal _t3115[] = {gem_v_mname};
    GemVal _t3116[] = {gem_v_p};
    GemVal _t3117[] = {gem_v_mname};
    GemVal _t3118 = gem_v_r;
    GemVal _t3119[] = {gem_table_get(_t3118, gem_string("expr"))};
                GemVal _t3120 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3111, 1), gem_to_string_fn(NULL, _t3113, 1)), gem_to_string_fn(NULL, _t3114, 1)), gem_to_string_fn(NULL, _t3115, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3116, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3117, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3119, 1)), gem_string(";"));
                gem_pop_frame();
                return _t3120;
            }
#line 1045 "compiler/main.gem"
    GemVal _t3121[] = {gem_v_line_dir};
    GemVal _t3122 = gem_v_r;
    GemVal _t3123[] = {gem_table_get(_t3122, gem_string("setup"))};
    GemVal _t3124[] = {gem_v_p};
    GemVal _t3125[] = {gem_v_mname};
    GemVal _t3126 = gem_v_r;
    GemVal _t3127[] = {gem_table_get(_t3126, gem_string("expr"))};
            GemVal _t3128 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3121, 1), gem_to_string_fn(NULL, _t3123, 1)), gem_to_string_fn(NULL, _t3124, 1)), gem_to_string_fn(NULL, _t3125, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3127, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3128;
        }
#line 1047 "compiler/main.gem"
    GemVal _t3129[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3129, 2))) {
#line 1048 "compiler/main.gem"
    GemVal _t3130[] = {gem_v_line_dir};
    GemVal _t3131 = gem_v_r;
    GemVal _t3132[] = {gem_table_get(_t3131, gem_string("setup"))};
    GemVal _t3133[] = {gem_v_p};
    GemVal _t3134[] = {gem_v_mname};
    GemVal _t3135[] = {gem_v_p};
    GemVal _t3136[] = {gem_v_mname};
    GemVal _t3137 = gem_v_r;
    GemVal _t3138[] = {gem_table_get(_t3137, gem_string("expr"))};
            GemVal _t3139 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3130, 1), gem_to_string_fn(NULL, _t3132, 1)), gem_to_string_fn(NULL, _t3133, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t3134, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3135, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3136, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3138, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3139;
        }
#line 1050 "compiler/main.gem"
    GemVal _t3140[] = {gem_v_line_dir};
    GemVal _t3141 = gem_v_r;
    GemVal _t3142[] = {gem_table_get(_t3141, gem_string("setup"))};
    GemVal _t3143[] = {gem_v_p};
    GemVal _t3144[] = {gem_v_mname};
    GemVal _t3145 = gem_v_r;
    GemVal _t3146[] = {gem_table_get(_t3145, gem_string("expr"))};
        GemVal _t3147 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3140, 1), gem_to_string_fn(NULL, _t3142, 1)), gem_to_string_fn(NULL, _t3143, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3144, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3146, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3147;
    } else {
    GemVal _t3148[] = {gem_v__match_57};
    GemVal _t3150;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3148, 1), gem_string("table")))) {
        _t3150 = gem_eq(gem_type_fn(NULL, _t3148, 1), gem_string("table"));
    } else {
        GemVal _t3149[] = {gem_v__match_57, gem_string("tag")};
        _t3150 = gem_has_key_fn(NULL, _t3149, 2);
    }
    GemVal _t3151;
    if (!gem_truthy(_t3150)) {
        _t3151 = _t3150;
    } else {
        _t3151 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3153;
    if (!gem_truthy(_t3151)) {
        _t3153 = _t3151;
    } else {
        GemVal _t3152[] = {gem_v__match_57, gem_string("name")};
        _t3153 = gem_has_key_fn(NULL, _t3152, 2);
    }
    GemVal _t3155;
    if (!gem_truthy(_t3153)) {
        _t3155 = _t3153;
    } else {
        GemVal _t3154[] = {gem_v__match_57, gem_string("value")};
        _t3155 = gem_has_key_fn(NULL, _t3154, 2);
    }
    if (gem_truthy(_t3155)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_57, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1052 "compiler/main.gem"
    GemVal _t3156[] = {gem_v_value};
    GemVal _t3157 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3157.fn(_t3157.env, _t3156, 1);
#line 1053 "compiler/main.gem"
    GemVal _t3158[] = {gem_v_name};
    GemVal _t3159 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3159.fn(_t3159.env, _t3158, 1);
#line 1054 "compiler/main.gem"
    GemVal _t3160[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3162;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3160, 2))) {
        _t3162 = gem_fn__mod_codegen_set_contains(NULL, _t3160, 2);
    } else {
        GemVal _t3161[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3162 = gem_fn__mod_codegen_set_contains(NULL, _t3161, 2);
    }
        if (gem_truthy(_t3162)) {
#line 1055 "compiler/main.gem"
    GemVal _t3163[] = {gem_v_line_dir};
    GemVal _t3164 = gem_v_r;
    GemVal _t3165[] = {gem_table_get(_t3164, gem_string("setup"))};
    GemVal _t3166[] = {gem_v_p};
    GemVal _t3167[] = {gem_v_mname};
    GemVal _t3168 = gem_v_r;
    GemVal _t3169[] = {gem_table_get(_t3168, gem_string("expr"))};
            GemVal _t3170 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3163, 1), gem_to_string_fn(NULL, _t3165, 1)), gem_to_string_fn(NULL, _t3166, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3167, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3169, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3170;
        }
#line 1057 "compiler/main.gem"
    GemVal _t3171[] = {gem_v_line_dir};
    GemVal _t3172 = gem_v_r;
    GemVal _t3173[] = {gem_table_get(_t3172, gem_string("setup"))};
    GemVal _t3174[] = {gem_v_p};
    GemVal _t3175[] = {gem_v_mname};
    GemVal _t3176 = gem_v_r;
    GemVal _t3177[] = {gem_table_get(_t3176, gem_string("expr"))};
        GemVal _t3178 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3171, 1), gem_to_string_fn(NULL, _t3173, 1)), gem_to_string_fn(NULL, _t3174, 1)), gem_to_string_fn(NULL, _t3175, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3177, 1)), gem_string(";"));
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
        _t3182 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t3184;
    if (!gem_truthy(_t3182)) {
        _t3184 = _t3182;
    } else {
        GemVal _t3183[] = {gem_v__match_57, gem_string("object")};
        _t3184 = gem_has_key_fn(NULL, _t3183, 2);
    }
    GemVal _t3186;
    if (!gem_truthy(_t3184)) {
        _t3186 = _t3184;
    } else {
        GemVal _t3185[] = {gem_v__match_57, gem_string("value")};
        _t3186 = gem_has_key_fn(NULL, _t3185, 2);
    }
    GemVal _t3188;
    if (!gem_truthy(_t3186)) {
        _t3188 = _t3186;
    } else {
        GemVal _t3187[] = {gem_v__match_57, gem_string("field")};
        _t3188 = gem_has_key_fn(NULL, _t3187, 2);
    }
    if (gem_truthy(_t3188)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_57, gem_string("field"));
#line 1059 "compiler/main.gem"
    GemVal _t3189[] = {gem_v_object};
    GemVal _t3190 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3190.fn(_t3190.env, _t3189, 1);
#line 1060 "compiler/main.gem"
    GemVal _t3191[] = {gem_v_value};
    GemVal _t3192 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3192.fn(_t3192.env, _t3191, 1);
#line 1061 "compiler/main.gem"
    GemVal _t3193[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t3193, 1);
#line 1062 "compiler/main.gem"
    GemVal _t3194[] = {gem_v_line_dir};
    GemVal _t3195 = gem_v_obj_r;
    GemVal _t3196[] = {gem_table_get(_t3195, gem_string("setup"))};
    GemVal _t3197 = gem_v_val_r;
    GemVal _t3198[] = {gem_table_get(_t3197, gem_string("setup"))};
    GemVal _t3199[] = {gem_v_p};
    GemVal _t3200 = gem_v_obj_r;
    GemVal _t3201[] = {gem_table_get(_t3200, gem_string("expr"))};
    GemVal _t3202[] = {gem_v_escaped};
    GemVal _t3203 = gem_v_val_r;
    GemVal _t3204[] = {gem_table_get(_t3203, gem_string("expr"))};
        GemVal _t3205 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3194, 1), gem_to_string_fn(NULL, _t3196, 1)), gem_to_string_fn(NULL, _t3198, 1)), gem_to_string_fn(NULL, _t3199, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t3201, 1)), gem_string(", gem_string(\"")), gem_to_string_fn(NULL, _t3202, 1)), gem_string("\"), ")), gem_to_string_fn(NULL, _t3204, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3205;
    } else {
    GemVal _t3206[] = {gem_v__match_57};
    GemVal _t3208;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3206, 1), gem_string("table")))) {
        _t3208 = gem_eq(gem_type_fn(NULL, _t3206, 1), gem_string("table"));
    } else {
        GemVal _t3207[] = {gem_v__match_57, gem_string("tag")};
        _t3208 = gem_has_key_fn(NULL, _t3207, 2);
    }
    GemVal _t3209;
    if (!gem_truthy(_t3208)) {
        _t3209 = _t3208;
    } else {
        _t3209 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t3211;
    if (!gem_truthy(_t3209)) {
        _t3211 = _t3209;
    } else {
        GemVal _t3210[] = {gem_v__match_57, gem_string("object")};
        _t3211 = gem_has_key_fn(NULL, _t3210, 2);
    }
    GemVal _t3213;
    if (!gem_truthy(_t3211)) {
        _t3213 = _t3211;
    } else {
        GemVal _t3212[] = {gem_v__match_57, gem_string("key")};
        _t3213 = gem_has_key_fn(NULL, _t3212, 2);
    }
    GemVal _t3215;
    if (!gem_truthy(_t3213)) {
        _t3215 = _t3213;
    } else {
        GemVal _t3214[] = {gem_v__match_57, gem_string("value")};
        _t3215 = gem_has_key_fn(NULL, _t3214, 2);
    }
    if (gem_truthy(_t3215)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_57, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_57, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1064 "compiler/main.gem"
    GemVal _t3216[] = {gem_v_object};
    GemVal _t3217 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3217.fn(_t3217.env, _t3216, 1);
#line 1065 "compiler/main.gem"
    GemVal _t3218[] = {gem_v_key};
    GemVal _t3219 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t3219.fn(_t3219.env, _t3218, 1);
#line 1066 "compiler/main.gem"
    GemVal _t3220[] = {gem_v_value};
    GemVal _t3221 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3221.fn(_t3221.env, _t3220, 1);
#line 1067 "compiler/main.gem"
    GemVal _t3222[] = {gem_v_line_dir};
    GemVal _t3223 = gem_v_obj_r;
    GemVal _t3224[] = {gem_table_get(_t3223, gem_string("setup"))};
    GemVal _t3225 = gem_v_key_r;
    GemVal _t3226[] = {gem_table_get(_t3225, gem_string("setup"))};
    GemVal _t3227 = gem_v_val_r;
    GemVal _t3228[] = {gem_table_get(_t3227, gem_string("setup"))};
    GemVal _t3229[] = {gem_v_p};
    GemVal _t3230 = gem_v_obj_r;
    GemVal _t3231[] = {gem_table_get(_t3230, gem_string("expr"))};
    GemVal _t3232 = gem_v_key_r;
    GemVal _t3233[] = {gem_table_get(_t3232, gem_string("expr"))};
    GemVal _t3234 = gem_v_val_r;
    GemVal _t3235[] = {gem_table_get(_t3234, gem_string("expr"))};
        GemVal _t3236 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3222, 1), gem_to_string_fn(NULL, _t3224, 1)), gem_to_string_fn(NULL, _t3226, 1)), gem_to_string_fn(NULL, _t3228, 1)), gem_to_string_fn(NULL, _t3229, 1)), gem_string("gem_table_set(")), gem_to_string_fn(NULL, _t3231, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3233, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3235, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3236;
    } else {
    GemVal _t3237[] = {gem_v__match_57};
    GemVal _t3239;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3237, 1), gem_string("table")))) {
        _t3239 = gem_eq(gem_type_fn(NULL, _t3237, 1), gem_string("table"));
    } else {
        GemVal _t3238[] = {gem_v__match_57, gem_string("tag")};
        _t3239 = gem_has_key_fn(NULL, _t3238, 2);
    }
    GemVal _t3240;
    if (!gem_truthy(_t3239)) {
        _t3240 = _t3239;
    } else {
        _t3240 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3240)) {
#line 1069 "compiler/main.gem"
    GemVal _t3241[] = {gem_v_line_dir};
    GemVal _t3242[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3243 = (*gem_v_compile_if);
        GemVal _t3244 = gem_add(gem_to_string_fn(NULL, _t3241, 1), _t3243.fn(_t3243.env, _t3242, 3));
        gem_pop_frame();
        return _t3244;
    } else {
    GemVal _t3245[] = {gem_v__match_57};
    GemVal _t3247;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3245, 1), gem_string("table")))) {
        _t3247 = gem_eq(gem_type_fn(NULL, _t3245, 1), gem_string("table"));
    } else {
        GemVal _t3246[] = {gem_v__match_57, gem_string("tag")};
        _t3247 = gem_has_key_fn(NULL, _t3246, 2);
    }
    GemVal _t3248;
    if (!gem_truthy(_t3247)) {
        _t3248 = _t3247;
    } else {
        _t3248 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3248)) {
#line 1071 "compiler/main.gem"
    GemVal _t3249[] = {gem_v_line_dir};
    GemVal _t3250[] = {gem_v_node, gem_v_indent};
    GemVal _t3251 = (*gem_v_compile_while);
        GemVal _t3252 = gem_add(gem_to_string_fn(NULL, _t3249, 1), _t3251.fn(_t3251.env, _t3250, 2));
        gem_pop_frame();
        return _t3252;
    } else {
    GemVal _t3253[] = {gem_v__match_57};
    GemVal _t3255;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3253, 1), gem_string("table")))) {
        _t3255 = gem_eq(gem_type_fn(NULL, _t3253, 1), gem_string("table"));
    } else {
        GemVal _t3254[] = {gem_v__match_57, gem_string("tag")};
        _t3255 = gem_has_key_fn(NULL, _t3254, 2);
    }
    GemVal _t3256;
    if (!gem_truthy(_t3255)) {
        _t3256 = _t3255;
    } else {
        _t3256 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3256)) {
#line 1073 "compiler/main.gem"
    GemVal _t3257[] = {gem_v_line_dir};
    GemVal _t3258[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3259 = (*gem_v_compile_match);
        GemVal _t3260 = gem_add(gem_to_string_fn(NULL, _t3257, 1), _t3259.fn(_t3259.env, _t3258, 3));
        gem_pop_frame();
        return _t3260;
    } else {
    GemVal _t3261[] = {gem_v__match_57};
    GemVal _t3263;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3261, 1), gem_string("table")))) {
        _t3263 = gem_eq(gem_type_fn(NULL, _t3261, 1), gem_string("table"));
    } else {
        GemVal _t3262[] = {gem_v__match_57, gem_string("tag")};
        _t3263 = gem_has_key_fn(NULL, _t3262, 2);
    }
    GemVal _t3264;
    if (!gem_truthy(_t3263)) {
        _t3264 = _t3263;
    } else {
        _t3264 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3264)) {
#line 1075 "compiler/main.gem"
    GemVal _t3265[] = {gem_v_line_dir};
    GemVal _t3266[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3267 = (*gem_v_compile_receive_match);
        GemVal _t3268 = gem_add(gem_to_string_fn(NULL, _t3265, 1), _t3267.fn(_t3267.env, _t3266, 3));
        gem_pop_frame();
        return _t3268;
    } else {
    GemVal _t3269[] = {gem_v__match_57};
    GemVal _t3271;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3269, 1), gem_string("table")))) {
        _t3271 = gem_eq(gem_type_fn(NULL, _t3269, 1), gem_string("table"));
    } else {
        GemVal _t3270[] = {gem_v__match_57, gem_string("tag")};
        _t3271 = gem_has_key_fn(NULL, _t3270, 2);
    }
    GemVal _t3272;
    if (!gem_truthy(_t3271)) {
        _t3272 = _t3271;
    } else {
        _t3272 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3272)) {
#line 1077 "compiler/main.gem"
        GemVal _t3273 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t3273;
    } else {
    GemVal _t3274[] = {gem_v__match_57};
    GemVal _t3276;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3274, 1), gem_string("table")))) {
        _t3276 = gem_eq(gem_type_fn(NULL, _t3274, 1), gem_string("table"));
    } else {
        GemVal _t3275[] = {gem_v__match_57, gem_string("tag")};
        _t3276 = gem_has_key_fn(NULL, _t3275, 2);
    }
    GemVal _t3277;
    if (!gem_truthy(_t3276)) {
        _t3277 = _t3276;
    } else {
        _t3277 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3277)) {
#line 1079 "compiler/main.gem"
        GemVal _t3278 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t3278;
    } else {
    GemVal _t3279[] = {gem_v__match_57};
    GemVal _t3281;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3279, 1), gem_string("table")))) {
        _t3281 = gem_eq(gem_type_fn(NULL, _t3279, 1), gem_string("table"));
    } else {
        GemVal _t3280[] = {gem_v__match_57, gem_string("tag")};
        _t3281 = gem_has_key_fn(NULL, _t3280, 2);
    }
    GemVal _t3282;
    if (!gem_truthy(_t3281)) {
        _t3282 = _t3281;
    } else {
        _t3282 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3284;
    if (!gem_truthy(_t3282)) {
        _t3284 = _t3282;
    } else {
        GemVal _t3283[] = {gem_v__match_57, gem_string("value")};
        _t3284 = gem_has_key_fn(NULL, _t3283, 2);
    }
    if (gem_truthy(_t3284)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_57, gem_string("value"));
#line 1081 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1082 "compiler/main.gem"
    GemVal _t3285[] = {gem_v_value};
    GemVal _t3286 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3286.fn(_t3286.env, _t3285, 1);
#line 1083 "compiler/main.gem"
    GemVal _t3287 = (*gem_v_tmp);
            GemVal gem_v_t = _t3287.fn(_t3287.env, NULL, 0);
#line 1084 "compiler/main.gem"
    GemVal _t3288[] = {gem_v_line_dir};
    GemVal _t3289 = gem_v_r;
    GemVal _t3290[] = {gem_table_get(_t3289, gem_string("setup"))};
    GemVal _t3291[] = {gem_v_p};
    GemVal _t3292[] = {gem_v_t};
    GemVal _t3293 = gem_v_r;
    GemVal _t3294[] = {gem_table_get(_t3293, gem_string("expr"))};
    GemVal _t3295[] = {gem_v_p};
    GemVal _t3296[] = {gem_v_p};
    GemVal _t3297[] = {gem_v_t};
            GemVal _t3298 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3288, 1), gem_to_string_fn(NULL, _t3290, 1)), gem_to_string_fn(NULL, _t3291, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3292, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3294, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3295, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3296, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3297, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3298;
        }
#line 1086 "compiler/main.gem"
    GemVal _t3299[] = {gem_v_line_dir};
    GemVal _t3300[] = {gem_v_p};
    GemVal _t3301[] = {gem_v_p};
        GemVal _t3302 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3299, 1), gem_to_string_fn(NULL, _t3300, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3301, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3302;
    } else {
    GemVal _t3303[] = {gem_v__match_57};
    GemVal _t3305;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3303, 1), gem_string("table")))) {
        _t3305 = gem_eq(gem_type_fn(NULL, _t3303, 1), gem_string("table"));
    } else {
        GemVal _t3304[] = {gem_v__match_57, gem_string("tag")};
        _t3305 = gem_has_key_fn(NULL, _t3304, 2);
    }
    GemVal _t3306;
    if (!gem_truthy(_t3305)) {
        _t3306 = _t3305;
    } else {
        _t3306 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t3306)) {
#line 1088 "compiler/main.gem"
    GemVal _t3307[] = {gem_v_node};
    GemVal _t3308 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3308.fn(_t3308.env, _t3307, 1);
#line 1089 "compiler/main.gem"
    GemVal _t3309[] = {gem_v_line_dir};
    GemVal _t3310 = gem_v_r;
    GemVal _t3311[] = {gem_table_get(_t3310, gem_string("setup"))};
    GemVal _t3312[] = {gem_v_p};
    GemVal _t3313 = gem_v_r;
    GemVal _t3314[] = {gem_table_get(_t3313, gem_string("expr"))};
        GemVal _t3315 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3309, 1), gem_to_string_fn(NULL, _t3311, 1)), gem_to_string_fn(NULL, _t3312, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t3314, 1)), gem_string(");"));
        gem_pop_frame();
        return _t3315;
    } else {
    GemVal _t3316[] = {gem_v__match_57};
    GemVal _t3318;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3316, 1), gem_string("table")))) {
        _t3318 = gem_eq(gem_type_fn(NULL, _t3316, 1), gem_string("table"));
    } else {
        GemVal _t3317[] = {gem_v__match_57, gem_string("tag")};
        _t3318 = gem_has_key_fn(NULL, _t3317, 2);
    }
    GemVal _t3319;
    if (!gem_truthy(_t3318)) {
        _t3319 = _t3318;
    } else {
        _t3319 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3319)) {
#line 1091 "compiler/main.gem"
        GemVal _t3320 = gem_string("");
        gem_pop_frame();
        return _t3320;
    } else {
    GemVal _t3321[] = {gem_v__match_57};
    GemVal _t3323;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3321, 1), gem_string("table")))) {
        _t3323 = gem_eq(gem_type_fn(NULL, _t3321, 1), gem_string("table"));
    } else {
        GemVal _t3322[] = {gem_v__match_57, gem_string("tag")};
        _t3323 = gem_has_key_fn(NULL, _t3322, 2);
    }
    GemVal _t3324;
    if (!gem_truthy(_t3323)) {
        _t3324 = _t3323;
    } else {
        _t3324 = gem_eq(gem_table_get(gem_v__match_57, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3326;
    if (!gem_truthy(_t3324)) {
        _t3326 = _t3324;
    } else {
        GemVal _t3325[] = {gem_v__match_57, gem_string("stmts")};
        _t3326 = gem_has_key_fn(NULL, _t3325, 2);
    }
    if (gem_truthy(_t3326)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_57, gem_string("stmts"));
#line 1093 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1094 "compiler/main.gem"
        GemVal gem_v__for_items_58 = gem_v_stmts;
#line 1094 "compiler/main.gem"
        GemVal gem_v__for_i_58 = gem_int(0);
#line 1094 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3327[] = {gem_v__for_items_58};
            if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t3327, 1)))) break;
#line 1094 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1094 "compiler/main.gem"
            gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1095 "compiler/main.gem"
    GemVal _t3328[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3329 = (*gem_v_compile_stmt);
    GemVal _t3330[] = {gem_v_b, _t3329.fn(_t3329.env, _t3328, 2)};
            (void)(gem_buf_push_fn(NULL, _t3330, 2));
#line 1096 "compiler/main.gem"
    GemVal _t3331[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3331, 2));
        }

#line 1098 "compiler/main.gem"
    GemVal _t3332[] = {gem_v_b};
        GemVal _t3333 = gem_buf_str_fn(NULL, _t3332, 1);
        gem_pop_frame();
        return _t3333;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1101 "compiler/main.gem"
    GemVal _t3334[] = {gem_v_node};
    GemVal _t3335 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3335.fn(_t3335.env, _t3334, 1);
    GemVal _t3336[] = {gem_v_line_dir};
    GemVal _t3337 = gem_v_r;
    GemVal _t3338[] = {gem_table_get(_t3337, gem_string("setup"))};
    GemVal _t3339[] = {gem_v_p};
    GemVal _t3340 = gem_v_r;
    GemVal _t3341[] = {gem_table_get(_t3340, gem_string("expr"))};
    GemVal _t3342 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3336, 1), gem_to_string_fn(NULL, _t3338, 1)), gem_to_string_fn(NULL, _t3339, 1)), gem_string("(void)(")), gem_to_string_fn(NULL, _t3341, 1)), gem_string(");"));
    gem_pop_frame();
    return _t3342;
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
#line 1108 "compiler/main.gem"
    GemVal _t3344[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3344, 1);
#line 1109 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1110 "compiler/main.gem"
    GemVal _t3345 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3345, gem_string("line")), GEM_NIL))) {
#line 1111 "compiler/main.gem"
    GemVal _t3346 = gem_v_node;
    GemVal _t3347[] = {gem_table_get(_t3346, gem_string("line"))};
    GemVal _t3348[] = {(*gem_v_source_name)};
    GemVal _t3349[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t3348, 1)};
        gem_v_line_dir = gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t3347, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t3349, 1)), gem_string("\"\n"));
    }
#line 1114 "compiler/main.gem"
    if (gem_truthy(gem_neq((*gem_v_tco_fn_name), GEM_NIL))) {
#line 1115 "compiler/main.gem"
    GemVal _t3350[] = {gem_v_node, gem_string("call")};
    GemVal _t3353;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3350, 2))) {
        _t3353 = gem_fn__mod_codegen_is_node(NULL, _t3350, 2);
    } else {
        GemVal _t3351 = gem_v_node;
        GemVal _t3352[] = {gem_table_get(_t3351, gem_string("func")), gem_string("var")};
        _t3353 = gem_fn__mod_codegen_is_node(NULL, _t3352, 2);
    }
    GemVal _t3356;
    if (!gem_truthy(_t3353)) {
        _t3356 = _t3353;
    } else {
        GemVal _t3354 = gem_v_node;
        GemVal _t3355 = gem_table_get(_t3354, gem_string("func"));
        _t3356 = gem_eq(gem_table_get(_t3355, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3358;
    if (!gem_truthy(_t3356)) {
        _t3358 = _t3356;
    } else {
        GemVal _t3357[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3358 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3357, 2));
    }
        if (gem_truthy(_t3358)) {
#line 1116 "compiler/main.gem"
    GemVal _t3359[] = {gem_v_node, gem_v_indent};
    GemVal _t3360 = (*gem_v_emit_tco_continue);
            GemVal _t3361 = gem_add(gem_v_line_dir, _t3360.fn(_t3360.env, _t3359, 2));
            gem_pop_frame();
            return _t3361;
        }
#line 1118 "compiler/main.gem"
    GemVal _t3362[] = {gem_v_node, gem_string("return")};
    GemVal _t3364;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3362, 2))) {
        _t3364 = gem_fn__mod_codegen_is_node(NULL, _t3362, 2);
    } else {
        GemVal _t3363 = gem_v_node;
        _t3364 = gem_neq(gem_table_get(_t3363, gem_string("value")), GEM_NIL);
    }
    GemVal _t3367;
    if (!gem_truthy(_t3364)) {
        _t3367 = _t3364;
    } else {
        GemVal _t3365 = gem_v_node;
        GemVal _t3366[] = {gem_table_get(_t3365, gem_string("value")), gem_string("call")};
        _t3367 = gem_fn__mod_codegen_is_node(NULL, _t3366, 2);
    }
    GemVal _t3371;
    if (!gem_truthy(_t3367)) {
        _t3371 = _t3367;
    } else {
        GemVal _t3368 = gem_v_node;
        GemVal _t3369 = gem_table_get(_t3368, gem_string("value"));
        GemVal _t3370[] = {gem_table_get(_t3369, gem_string("func")), gem_string("var")};
        _t3371 = gem_fn__mod_codegen_is_node(NULL, _t3370, 2);
    }
    GemVal _t3375;
    if (!gem_truthy(_t3371)) {
        _t3375 = _t3371;
    } else {
        GemVal _t3372 = gem_v_node;
        GemVal _t3373 = gem_table_get(_t3372, gem_string("value"));
        GemVal _t3374 = gem_table_get(_t3373, gem_string("func"));
        _t3375 = gem_eq(gem_table_get(_t3374, gem_string("name")), (*gem_v_tco_fn_name));
    }
    GemVal _t3377;
    if (!gem_truthy(_t3375)) {
        _t3377 = _t3375;
    } else {
        GemVal _t3376[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3377 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3376, 2));
    }
        if (gem_truthy(_t3377)) {
#line 1119 "compiler/main.gem"
    GemVal _t3378 = gem_v_node;
    GemVal _t3379[] = {gem_table_get(_t3378, gem_string("value")), gem_v_indent};
    GemVal _t3380 = (*gem_v_emit_tco_continue);
            GemVal _t3381 = gem_add(gem_v_line_dir, _t3380.fn(_t3380.env, _t3379, 2));
            gem_pop_frame();
            return _t3381;
        }
    }
#line 1123 "compiler/main.gem"
    GemVal gem_v__match_59 = gem_v_node;
    GemVal _t3382[] = {gem_v__match_59};
    GemVal _t3384;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3382, 1), gem_string("table")))) {
        _t3384 = gem_eq(gem_type_fn(NULL, _t3382, 1), gem_string("table"));
    } else {
        GemVal _t3383[] = {gem_v__match_59, gem_string("tag")};
        _t3384 = gem_has_key_fn(NULL, _t3383, 2);
    }
    GemVal _t3385;
    if (!gem_truthy(_t3384)) {
        _t3385 = _t3384;
    } else {
        _t3385 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3387;
    if (!gem_truthy(_t3385)) {
        _t3387 = _t3385;
    } else {
        GemVal _t3386[] = {gem_v__match_59, gem_string("name")};
        _t3387 = gem_has_key_fn(NULL, _t3386, 2);
    }
    GemVal _t3389;
    if (!gem_truthy(_t3387)) {
        _t3389 = _t3387;
    } else {
        GemVal _t3388[] = {gem_v__match_59, gem_string("value")};
        _t3389 = gem_has_key_fn(NULL, _t3388, 2);
    }
    if (gem_truthy(_t3389)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_59, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1125 "compiler/main.gem"
    GemVal _t3390[] = {gem_v_value};
    GemVal _t3391 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3391.fn(_t3391.env, _t3390, 1);
#line 1126 "compiler/main.gem"
    GemVal _t3392[] = {gem_v_name};
    GemVal _t3393 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3393.fn(_t3393.env, _t3392, 1);
#line 1127 "compiler/main.gem"
    GemVal _t3394[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3394, 2))) {
#line 1128 "compiler/main.gem"
    GemVal _t3395[] = {gem_v_line_dir};
    GemVal _t3396 = gem_v_r;
    GemVal _t3397[] = {gem_table_get(_t3396, gem_string("setup"))};
    GemVal _t3398[] = {gem_v_p};
    GemVal _t3399[] = {gem_v_mname};
    GemVal _t3400[] = {gem_v_p};
    GemVal _t3401[] = {gem_v_mname};
    GemVal _t3402 = gem_v_r;
    GemVal _t3403[] = {gem_table_get(_t3402, gem_string("expr"))};
    GemVal _t3404[] = {gem_v_p};
    GemVal _t3405[] = {gem_v_p};
            GemVal _t3406 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3395, 1), gem_to_string_fn(NULL, _t3397, 1)), gem_to_string_fn(NULL, _t3398, 1)), gem_string("GemVal *")), gem_to_string_fn(NULL, _t3399, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n")), gem_to_string_fn(NULL, _t3400, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3401, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3403, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3404, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3405, 1)), gem_string("return GEM_NIL;"));
            gem_pop_frame();
            return _t3406;
        }
#line 1130 "compiler/main.gem"
    GemVal _t3407[] = {gem_v_line_dir};
    GemVal _t3408 = gem_v_r;
    GemVal _t3409[] = {gem_table_get(_t3408, gem_string("setup"))};
    GemVal _t3410[] = {gem_v_p};
    GemVal _t3411[] = {gem_v_mname};
    GemVal _t3412 = gem_v_r;
    GemVal _t3413[] = {gem_table_get(_t3412, gem_string("expr"))};
    GemVal _t3414[] = {gem_v_p};
    GemVal _t3415[] = {gem_v_p};
        GemVal _t3416 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3407, 1), gem_to_string_fn(NULL, _t3409, 1)), gem_to_string_fn(NULL, _t3410, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3411, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3413, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3414, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3415, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3416;
    } else {
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
        _t3420 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("assign"));
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
    GemVal _t3427 = (*gem_v_tmp);
        GemVal gem_v_t = _t3427.fn(_t3427.env, NULL, 0);
#line 1134 "compiler/main.gem"
    GemVal _t3428[] = {gem_v_name};
    GemVal _t3429 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3429.fn(_t3429.env, _t3428, 1);
#line 1135 "compiler/main.gem"
    GemVal _t3430[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3432;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3430, 2))) {
        _t3432 = gem_fn__mod_codegen_set_contains(NULL, _t3430, 2);
    } else {
        GemVal _t3431[] = {(*gem_v_top_level_boxed), gem_v_name};
        _t3432 = gem_fn__mod_codegen_set_contains(NULL, _t3431, 2);
    }
        if (gem_truthy(_t3432)) {
#line 1136 "compiler/main.gem"
    GemVal _t3433[] = {gem_v_line_dir};
    GemVal _t3434 = gem_v_r;
    GemVal _t3435[] = {gem_table_get(_t3434, gem_string("setup"))};
    GemVal _t3436[] = {gem_v_p};
    GemVal _t3437[] = {gem_v_t};
    GemVal _t3438 = gem_v_r;
    GemVal _t3439[] = {gem_table_get(_t3438, gem_string("expr"))};
    GemVal _t3440[] = {gem_v_p};
    GemVal _t3441[] = {gem_v_mname};
    GemVal _t3442[] = {gem_v_t};
    GemVal _t3443[] = {gem_v_p};
    GemVal _t3444[] = {gem_v_p};
    GemVal _t3445[] = {gem_v_t};
            GemVal _t3446 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3433, 1), gem_to_string_fn(NULL, _t3435, 1)), gem_to_string_fn(NULL, _t3436, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3437, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3439, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3440, 1)), gem_string("*")), gem_to_string_fn(NULL, _t3441, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3442, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3443, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3444, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3445, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3446;
        }
#line 1138 "compiler/main.gem"
    GemVal _t3447[] = {gem_v_line_dir};
    GemVal _t3448 = gem_v_r;
    GemVal _t3449[] = {gem_table_get(_t3448, gem_string("setup"))};
    GemVal _t3450[] = {gem_v_p};
    GemVal _t3451[] = {gem_v_t};
    GemVal _t3452 = gem_v_r;
    GemVal _t3453[] = {gem_table_get(_t3452, gem_string("expr"))};
    GemVal _t3454[] = {gem_v_p};
    GemVal _t3455[] = {gem_v_mname};
    GemVal _t3456[] = {gem_v_t};
    GemVal _t3457[] = {gem_v_p};
    GemVal _t3458[] = {gem_v_p};
    GemVal _t3459[] = {gem_v_t};
        GemVal _t3460 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3447, 1), gem_to_string_fn(NULL, _t3449, 1)), gem_to_string_fn(NULL, _t3450, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3451, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3453, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3454, 1)), gem_to_string_fn(NULL, _t3455, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3456, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3457, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3458, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3459, 1)), gem_string(";"));
        gem_pop_frame();
        return _t3460;
    } else {
    GemVal _t3461[] = {gem_v__match_59};
    GemVal _t3463;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3461, 1), gem_string("table")))) {
        _t3463 = gem_eq(gem_type_fn(NULL, _t3461, 1), gem_string("table"));
    } else {
        GemVal _t3462[] = {gem_v__match_59, gem_string("tag")};
        _t3463 = gem_has_key_fn(NULL, _t3462, 2);
    }
    GemVal _t3464;
    if (!gem_truthy(_t3463)) {
        _t3464 = _t3463;
    } else {
        _t3464 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3466;
    if (!gem_truthy(_t3464)) {
        _t3466 = _t3464;
    } else {
        GemVal _t3465[] = {gem_v__match_59, gem_string("value")};
        _t3466 = gem_has_key_fn(NULL, _t3465, 2);
    }
    if (gem_truthy(_t3466)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_59, gem_string("value"));
#line 1140 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1141 "compiler/main.gem"
    GemVal _t3467[] = {gem_v_value};
    GemVal _t3468 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3468.fn(_t3468.env, _t3467, 1);
#line 1142 "compiler/main.gem"
    GemVal _t3469 = (*gem_v_tmp);
            GemVal gem_v_t = _t3469.fn(_t3469.env, NULL, 0);
#line 1143 "compiler/main.gem"
    GemVal _t3470[] = {gem_v_line_dir};
    GemVal _t3471 = gem_v_r;
    GemVal _t3472[] = {gem_table_get(_t3471, gem_string("setup"))};
    GemVal _t3473[] = {gem_v_p};
    GemVal _t3474[] = {gem_v_t};
    GemVal _t3475 = gem_v_r;
    GemVal _t3476[] = {gem_table_get(_t3475, gem_string("expr"))};
    GemVal _t3477[] = {gem_v_p};
    GemVal _t3478[] = {gem_v_p};
    GemVal _t3479[] = {gem_v_t};
            GemVal _t3480 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3470, 1), gem_to_string_fn(NULL, _t3472, 1)), gem_to_string_fn(NULL, _t3473, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3474, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3476, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3477, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3478, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3479, 1)), gem_string(";"));
            gem_pop_frame();
            return _t3480;
        }
#line 1145 "compiler/main.gem"
    GemVal _t3481[] = {gem_v_line_dir};
    GemVal _t3482[] = {gem_v_p};
    GemVal _t3483[] = {gem_v_p};
        GemVal _t3484 = gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3481, 1), gem_to_string_fn(NULL, _t3482, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3483, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3484;
    } else {
    GemVal _t3485[] = {gem_v__match_59};
    GemVal _t3487;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3485, 1), gem_string("table")))) {
        _t3487 = gem_eq(gem_type_fn(NULL, _t3485, 1), gem_string("table"));
    } else {
        GemVal _t3486[] = {gem_v__match_59, gem_string("tag")};
        _t3487 = gem_has_key_fn(NULL, _t3486, 2);
    }
    GemVal _t3488;
    if (!gem_truthy(_t3487)) {
        _t3488 = _t3487;
    } else {
        _t3488 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3488)) {
#line 1147 "compiler/main.gem"
    GemVal _t3489[] = {gem_v_line_dir};
    GemVal _t3490[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3491 = (*gem_v_compile_if);
        GemVal _t3492 = gem_add(gem_to_string_fn(NULL, _t3489, 1), _t3491.fn(_t3491.env, _t3490, 3));
        gem_pop_frame();
        return _t3492;
    } else {
    GemVal _t3493[] = {gem_v__match_59};
    GemVal _t3495;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3493, 1), gem_string("table")))) {
        _t3495 = gem_eq(gem_type_fn(NULL, _t3493, 1), gem_string("table"));
    } else {
        GemVal _t3494[] = {gem_v__match_59, gem_string("tag")};
        _t3495 = gem_has_key_fn(NULL, _t3494, 2);
    }
    GemVal _t3496;
    if (!gem_truthy(_t3495)) {
        _t3496 = _t3495;
    } else {
        _t3496 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3496)) {
#line 1149 "compiler/main.gem"
    GemVal _t3497[] = {gem_v_line_dir};
    GemVal _t3498[] = {gem_v_node, gem_v_indent};
    GemVal _t3499 = (*gem_v_compile_while);
    GemVal _t3500[] = {_t3499.fn(_t3499.env, _t3498, 2)};
    GemVal _t3501[] = {gem_v_p};
    GemVal _t3502[] = {gem_v_p};
        GemVal _t3503 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3497, 1), gem_to_string_fn(NULL, _t3500, 1)), gem_string("\n")), gem_to_string_fn(NULL, _t3501, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3502, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3503;
    } else {
    GemVal _t3504[] = {gem_v__match_59};
    GemVal _t3506;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3504, 1), gem_string("table")))) {
        _t3506 = gem_eq(gem_type_fn(NULL, _t3504, 1), gem_string("table"));
    } else {
        GemVal _t3505[] = {gem_v__match_59, gem_string("tag")};
        _t3506 = gem_has_key_fn(NULL, _t3505, 2);
    }
    GemVal _t3507;
    if (!gem_truthy(_t3506)) {
        _t3507 = _t3506;
    } else {
        _t3507 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3507)) {
#line 1151 "compiler/main.gem"
    GemVal _t3508[] = {gem_v_line_dir};
    GemVal _t3509[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3510 = (*gem_v_compile_match);
        GemVal _t3511 = gem_add(gem_to_string_fn(NULL, _t3508, 1), _t3510.fn(_t3510.env, _t3509, 3));
        gem_pop_frame();
        return _t3511;
    } else {
    GemVal _t3512[] = {gem_v__match_59};
    GemVal _t3514;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3512, 1), gem_string("table")))) {
        _t3514 = gem_eq(gem_type_fn(NULL, _t3512, 1), gem_string("table"));
    } else {
        GemVal _t3513[] = {gem_v__match_59, gem_string("tag")};
        _t3514 = gem_has_key_fn(NULL, _t3513, 2);
    }
    GemVal _t3515;
    if (!gem_truthy(_t3514)) {
        _t3515 = _t3514;
    } else {
        _t3515 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3515)) {
#line 1153 "compiler/main.gem"
    GemVal _t3516[] = {gem_v_line_dir};
    GemVal _t3517[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3518 = (*gem_v_compile_receive_match);
        GemVal _t3519 = gem_add(gem_to_string_fn(NULL, _t3516, 1), _t3518.fn(_t3518.env, _t3517, 3));
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
        _t3523 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3525;
    if (!gem_truthy(_t3523)) {
        _t3525 = _t3523;
    } else {
        GemVal _t3524[] = {gem_v__match_59, gem_string("stmts")};
        _t3525 = gem_has_key_fn(NULL, _t3524, 2);
    }
    if (gem_truthy(_t3525)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_59, gem_string("stmts"));
#line 1155 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1156 "compiler/main.gem"
    GemVal _t3526[] = {gem_v_p};
    GemVal _t3527[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3526, 1), gem_string("{\n"))};
        (void)(gem_buf_push_fn(NULL, _t3527, 2));
#line 1157 "compiler/main.gem"
        GemVal gem_v__for_i_60 = gem_int(0);
#line 1157 "compiler/main.gem"
    GemVal _t3528[] = {gem_v_stmts};
        GemVal gem_v__for_limit_60 = gem_sub(gem_len_fn(NULL, _t3528, 1), gem_int(1));
#line 1157 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_60, gem_v__for_limit_60))) break;
#line 1157 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_60;
#line 1157 "compiler/main.gem"
            gem_v__for_i_60 = gem_add(gem_v__for_i_60, gem_int(1));
#line 1158 "compiler/main.gem"
    GemVal _t3529[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3530 = (*gem_v_compile_stmt);
    GemVal _t3531[] = {gem_v_b, _t3530.fn(_t3530.env, _t3529, 2)};
            (void)(gem_buf_push_fn(NULL, _t3531, 2));
#line 1159 "compiler/main.gem"
    GemVal _t3532[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3532, 2));
        }

#line 1161 "compiler/main.gem"
    GemVal _t3533[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3533, 1), gem_int(0)))) {
#line 1162 "compiler/main.gem"
    GemVal _t3534[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3535 = (*gem_v_compile_stmt_return);
    GemVal _t3536[] = {gem_v_b, _t3535.fn(_t3535.env, _t3534, 2)};
            (void)(gem_buf_push_fn(NULL, _t3536, 2));
#line 1163 "compiler/main.gem"
    GemVal _t3537[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3537, 2));
        }
#line 1165 "compiler/main.gem"
    GemVal _t3538[] = {gem_v_p};
    GemVal _t3539[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3538, 1), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3539, 2));
#line 1166 "compiler/main.gem"
    GemVal _t3540[] = {gem_v_b};
        GemVal _t3541 = gem_buf_str_fn(NULL, _t3540, 1);
        gem_pop_frame();
        return _t3541;
    } else {
    GemVal _t3542[] = {gem_v__match_59};
    GemVal _t3544;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3542, 1), gem_string("table")))) {
        _t3544 = gem_eq(gem_type_fn(NULL, _t3542, 1), gem_string("table"));
    } else {
        GemVal _t3543[] = {gem_v__match_59, gem_string("tag")};
        _t3544 = gem_has_key_fn(NULL, _t3543, 2);
    }
    GemVal _t3545;
    if (!gem_truthy(_t3544)) {
        _t3545 = _t3544;
    } else {
        _t3545 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3545)) {
#line 1168 "compiler/main.gem"
    GemVal _t3546[] = {gem_v_node, gem_v_indent};
    GemVal _t3547 = (*gem_v_compile_stmt);
    GemVal _t3548[] = {_t3547.fn(_t3547.env, _t3546, 2)};
    GemVal _t3549[] = {gem_v_p};
    GemVal _t3550[] = {gem_v_p};
        GemVal _t3551 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3548, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3549, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3550, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3551;
    } else {
    GemVal _t3552[] = {gem_v__match_59};
    GemVal _t3554;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3552, 1), gem_string("table")))) {
        _t3554 = gem_eq(gem_type_fn(NULL, _t3552, 1), gem_string("table"));
    } else {
        GemVal _t3553[] = {gem_v__match_59, gem_string("tag")};
        _t3554 = gem_has_key_fn(NULL, _t3553, 2);
    }
    GemVal _t3555;
    if (!gem_truthy(_t3554)) {
        _t3555 = _t3554;
    } else {
        _t3555 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3555)) {
#line 1170 "compiler/main.gem"
    GemVal _t3556[] = {gem_v_node, gem_v_indent};
    GemVal _t3557 = (*gem_v_compile_stmt);
    GemVal _t3558[] = {_t3557.fn(_t3557.env, _t3556, 2)};
    GemVal _t3559[] = {gem_v_p};
    GemVal _t3560[] = {gem_v_p};
        GemVal _t3561 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3558, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3559, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3560, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3561;
    } else {
    GemVal _t3562[] = {gem_v__match_59};
    GemVal _t3564;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3562, 1), gem_string("table")))) {
        _t3564 = gem_eq(gem_type_fn(NULL, _t3562, 1), gem_string("table"));
    } else {
        GemVal _t3563[] = {gem_v__match_59, gem_string("tag")};
        _t3564 = gem_has_key_fn(NULL, _t3563, 2);
    }
    GemVal _t3565;
    if (!gem_truthy(_t3564)) {
        _t3565 = _t3564;
    } else {
        _t3565 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3565)) {
#line 1172 "compiler/main.gem"
    GemVal _t3566[] = {gem_v_node, gem_v_indent};
    GemVal _t3567 = (*gem_v_compile_stmt);
    GemVal _t3568[] = {_t3567.fn(_t3567.env, _t3566, 2)};
    GemVal _t3569[] = {gem_v_p};
    GemVal _t3570[] = {gem_v_p};
        GemVal _t3571 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3568, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3569, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3570, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3571;
    } else {
    GemVal _t3572[] = {gem_v__match_59};
    GemVal _t3574;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3572, 1), gem_string("table")))) {
        _t3574 = gem_eq(gem_type_fn(NULL, _t3572, 1), gem_string("table"));
    } else {
        GemVal _t3573[] = {gem_v__match_59, gem_string("tag")};
        _t3574 = gem_has_key_fn(NULL, _t3573, 2);
    }
    GemVal _t3575;
    if (!gem_truthy(_t3574)) {
        _t3575 = _t3574;
    } else {
        _t3575 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3575)) {
#line 1174 "compiler/main.gem"
    GemVal _t3576[] = {gem_v_node, gem_v_indent};
    GemVal _t3577 = (*gem_v_compile_stmt);
    GemVal _t3578[] = {_t3577.fn(_t3577.env, _t3576, 2)};
    GemVal _t3579[] = {gem_v_p};
    GemVal _t3580[] = {gem_v_p};
        GemVal _t3581 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3578, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3579, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3580, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3581;
    } else {
    GemVal _t3582[] = {gem_v__match_59};
    GemVal _t3584;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3582, 1), gem_string("table")))) {
        _t3584 = gem_eq(gem_type_fn(NULL, _t3582, 1), gem_string("table"));
    } else {
        GemVal _t3583[] = {gem_v__match_59, gem_string("tag")};
        _t3584 = gem_has_key_fn(NULL, _t3583, 2);
    }
    GemVal _t3585;
    if (!gem_truthy(_t3584)) {
        _t3585 = _t3584;
    } else {
        _t3585 = gem_eq(gem_table_get(gem_v__match_59, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3585)) {
#line 1176 "compiler/main.gem"
    GemVal _t3586[] = {gem_v_node, gem_v_indent};
    GemVal _t3587 = (*gem_v_compile_stmt);
    GemVal _t3588[] = {_t3587.fn(_t3587.env, _t3586, 2)};
    GemVal _t3589[] = {gem_v_p};
    GemVal _t3590[] = {gem_v_p};
        GemVal _t3591 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3588, 1), gem_string("\n")), gem_to_string_fn(NULL, _t3589, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3590, 1)), gem_string("return GEM_NIL;"));
        gem_pop_frame();
        return _t3591;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1179 "compiler/main.gem"
    GemVal _t3592[] = {gem_v_node};
    GemVal _t3593 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3593.fn(_t3593.env, _t3592, 1);
#line 1180 "compiler/main.gem"
    GemVal _t3594 = (*gem_v_tmp);
    GemVal gem_v_t = _t3594.fn(_t3594.env, NULL, 0);
    GemVal _t3595[] = {gem_v_line_dir};
    GemVal _t3596 = gem_v_r;
    GemVal _t3597[] = {gem_table_get(_t3596, gem_string("setup"))};
    GemVal _t3598[] = {gem_v_p};
    GemVal _t3599[] = {gem_v_t};
    GemVal _t3600 = gem_v_r;
    GemVal _t3601[] = {gem_table_get(_t3600, gem_string("expr"))};
    GemVal _t3602[] = {gem_v_p};
    GemVal _t3603[] = {gem_v_p};
    GemVal _t3604[] = {gem_v_t};
    GemVal _t3605 = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3595, 1), gem_to_string_fn(NULL, _t3597, 1)), gem_to_string_fn(NULL, _t3598, 1)), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3599, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3601, 1)), gem_string(";\n")), gem_to_string_fn(NULL, _t3602, 1)), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3603, 1)), gem_string("return ")), gem_to_string_fn(NULL, _t3604, 1)), gem_string(";"));
    gem_pop_frame();
    return _t3605;
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
#line 1187 "compiler/main.gem"
    GemVal _t3607[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3607, 1);
#line 1188 "compiler/main.gem"
    GemVal _t3608 = gem_v_node;
    GemVal _t3609[] = {gem_table_get(_t3608, gem_string("cond"))};
    GemVal _t3610 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3610.fn(_t3610.env, _t3609, 1);
#line 1189 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1190 "compiler/main.gem"
    GemVal _t3611 = gem_v_cond_r;
    GemVal _t3612[] = {gem_v_b, gem_table_get(_t3611, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3612, 2));
#line 1191 "compiler/main.gem"
    GemVal _t3613[] = {gem_v_p};
    GemVal _t3614 = gem_v_cond_r;
    GemVal _t3615[] = {gem_table_get(_t3614, gem_string("expr"))};
    GemVal _t3616[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3613, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3615, 1)), gem_string(")) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3616, 2));
#line 1192 "compiler/main.gem"
    GemVal _t3617 = gem_v_node;
    GemVal _t3618[] = {gem_table_get(_t3617, gem_string("then")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3619 = (*gem_v_compile_stmts_tail);
    GemVal _t3620[] = {gem_v_b, _t3619.fn(_t3619.env, _t3618, 3)};
    (void)(gem_buf_push_fn(NULL, _t3620, 2));
#line 1193 "compiler/main.gem"
    GemVal _t3621 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3621, gem_string("else")), GEM_NIL))) {
#line 1194 "compiler/main.gem"
    GemVal _t3622[] = {gem_v_p};
    GemVal _t3623[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3622, 1), gem_string("} else {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3623, 2));
#line 1195 "compiler/main.gem"
    GemVal _t3624 = gem_v_node;
    GemVal _t3625[] = {gem_table_get(_t3624, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3626 = (*gem_v_compile_stmts_tail);
    GemVal _t3627[] = {gem_v_b, _t3626.fn(_t3626.env, _t3625, 3)};
        (void)(gem_buf_push_fn(NULL, _t3627, 2));
    } else {
#line 1196 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1197 "compiler/main.gem"
    GemVal _t3628[] = {gem_v_p};
    GemVal _t3629[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3628, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3629, 2));
#line 1198 "compiler/main.gem"
    GemVal _t3630 = gem_table_new();
    GemVal _t3631[] = {_t3630, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3632 = (*gem_v_compile_stmts_tail);
    GemVal _t3633[] = {gem_v_b, _t3632.fn(_t3632.env, _t3631, 3)};
            (void)(gem_buf_push_fn(NULL, _t3633, 2));
        }
    }
#line 1200 "compiler/main.gem"
    GemVal _t3634[] = {gem_v_p};
    GemVal _t3635[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3634, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3635, 2));
#line 1201 "compiler/main.gem"
    GemVal _t3636[] = {gem_v_b};
    GemVal _t3637 = gem_buf_str_fn(NULL, _t3636, 1);
    gem_pop_frame();
    return _t3637;
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
#line 1207 "compiler/main.gem"
    GemVal _t3639[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3639, 1);
#line 1208 "compiler/main.gem"
    GemVal _t3640 = gem_v_node;
    GemVal _t3641[] = {gem_table_get(_t3640, gem_string("cond"))};
    GemVal _t3642 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3642.fn(_t3642.env, _t3641, 1);
#line 1209 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1210 "compiler/main.gem"
    GemVal _t3643[] = {gem_v_p};
    GemVal _t3644[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3643, 1), gem_string("while (1) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3644, 2));
#line 1211 "compiler/main.gem"
    GemVal _t3645[] = {gem_v_p};
    GemVal _t3646[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3645, 1), gem_string("    gem_yield_check();\n"))};
    (void)(gem_buf_push_fn(NULL, _t3646, 2));
#line 1212 "compiler/main.gem"
    GemVal _t3647 = gem_v_cond_r;
    if (gem_truthy(gem_neq(gem_table_get(_t3647, gem_string("setup")), gem_string("")))) {
#line 1213 "compiler/main.gem"
    GemVal _t3648 = gem_v_cond_r;
    GemVal _t3649[] = {gem_v_p};
    GemVal _t3650[] = {gem_table_get(_t3648, gem_string("setup")), gem_string("    "), gem_add(gem_to_string_fn(NULL, _t3649, 1), gem_string("    "))};
    GemVal _t3651[] = {gem_v_b, gem_str_replace_fn(NULL, _t3650, 3)};
        (void)(gem_buf_push_fn(NULL, _t3651, 2));
    }
#line 1215 "compiler/main.gem"
    GemVal _t3652[] = {gem_v_p};
    GemVal _t3653 = gem_v_cond_r;
    GemVal _t3654[] = {gem_table_get(_t3653, gem_string("expr"))};
    GemVal _t3655[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3652, 1), gem_string("    if (!gem_truthy(")), gem_to_string_fn(NULL, _t3654, 1)), gem_string(")) break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3655, 2));
#line 1216 "compiler/main.gem"
    GemVal _t3656 = gem_v_node;
    GemVal _t3657[] = {gem_table_get(_t3656, gem_string("body")), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3658 = (*gem_v_compile_stmts);
    GemVal _t3659[] = {gem_v_b, _t3658.fn(_t3658.env, _t3657, 2)};
    (void)(gem_buf_push_fn(NULL, _t3659, 2));
#line 1217 "compiler/main.gem"
    GemVal _t3660[] = {gem_v_p};
    GemVal _t3661[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3660, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3661, 2));
#line 1218 "compiler/main.gem"
    GemVal _t3662[] = {gem_v_b};
    GemVal _t3663 = gem_buf_str_fn(NULL, _t3662, 1);
    gem_pop_frame();
    return _t3663;
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
#line 1224 "compiler/main.gem"
    GemVal _t3665[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3665, 1);
#line 1225 "compiler/main.gem"
    GemVal _t3666 = gem_v_node;
    GemVal _t3667[] = {gem_table_get(_t3666, gem_string("target"))};
    GemVal _t3668 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3668.fn(_t3668.env, _t3667, 1);
#line 1226 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1227 "compiler/main.gem"
    GemVal _t3669 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3669, gem_string("target_var")), GEM_NIL))) {
#line 1228 "compiler/main.gem"
    GemVal _t3670 = gem_v_node;
    GemVal _t3671[] = {gem_table_get(_t3670, gem_string("target_var"))};
    GemVal _t3672 = (*gem_v_mangle);
        gem_v_t = _t3672.fn(_t3672.env, _t3671, 1);
    } else {
#line 1230 "compiler/main.gem"
    GemVal _t3673 = (*gem_v_tmp);
        gem_v_t = _t3673.fn(_t3673.env, NULL, 0);
    }
#line 1232 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1233 "compiler/main.gem"
    GemVal _t3674 = gem_v_target_r;
    GemVal _t3675[] = {gem_v_b, gem_table_get(_t3674, gem_string("setup"))};
    (void)(gem_buf_push_fn(NULL, _t3675, 2));
#line 1234 "compiler/main.gem"
    GemVal _t3676[] = {gem_v_p};
    GemVal _t3677[] = {gem_v_t};
    GemVal _t3678 = gem_v_target_r;
    GemVal _t3679[] = {gem_table_get(_t3678, gem_string("expr"))};
    GemVal _t3680[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3676, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3677, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3679, 1)), gem_string(";\n"))};
    (void)(gem_buf_push_fn(NULL, _t3680, 2));
#line 1235 "compiler/main.gem"
    GemVal _t3681 = gem_v_node;
    GemVal _t3682[] = {gem_table_get(_t3681, gem_string("whens"))};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3682, 1), gem_int(0)))) {
#line 1236 "compiler/main.gem"
    GemVal _t3683 = gem_v_node;
        if (gem_truthy(gem_neq(gem_table_get(_t3683, gem_string("else")), GEM_NIL))) {
#line 1237 "compiler/main.gem"
    GemVal _t3684 = gem_v_node;
    GemVal _t3685[] = {gem_table_get(_t3684, gem_string("else")), gem_v_indent, gem_v_return_pos};
    GemVal _t3686 = (*gem_v_compile_stmts_tail);
    GemVal _t3687[] = {gem_v_b, _t3686.fn(_t3686.env, _t3685, 3)};
            (void)(gem_buf_push_fn(NULL, _t3687, 2));
        } else {
#line 1238 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1239 "compiler/main.gem"
    GemVal _t3688[] = {gem_v_p};
    GemVal _t3689[] = {gem_v_p};
    GemVal _t3690[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3688, 1), gem_string("gem_pop_frame();\n")), gem_to_string_fn(NULL, _t3689, 1)), gem_string("return GEM_NIL;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3690, 2));
            }
        }
#line 1241 "compiler/main.gem"
    GemVal _t3691[] = {gem_v_p};
    GemVal _t3692[] = {gem_v_t};
    GemVal _t3693[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3691, 1), gem_string("(void)")), gem_to_string_fn(NULL, _t3692, 1)), gem_string(";"))};
        (void)(gem_buf_push_fn(NULL, _t3693, 2));
#line 1242 "compiler/main.gem"
    GemVal _t3694[] = {gem_v_b};
        GemVal _t3695 = gem_buf_str_fn(NULL, _t3694, 1);
        gem_pop_frame();
        return _t3695;
    }
#line 1244 "compiler/main.gem"
    GemVal gem_v__for_i_62 = gem_int(0);
#line 1244 "compiler/main.gem"
    GemVal _t3696 = gem_v_node;
    GemVal _t3697[] = {gem_table_get(_t3696, gem_string("whens"))};
    GemVal gem_v__for_limit_62 = gem_len_fn(NULL, _t3697, 1);
#line 1244 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_62, gem_v__for_limit_62))) break;
#line 1244 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_62;
#line 1244 "compiler/main.gem"
        gem_v__for_i_62 = gem_add(gem_v__for_i_62, gem_int(1));
#line 1245 "compiler/main.gem"
    GemVal _t3698 = gem_v_node;
        GemVal gem_v_w = gem_table_get(gem_table_get(_t3698, gem_string("whens")), gem_v_i);
#line 1246 "compiler/main.gem"
    GemVal _t3699 = gem_v_w;
    GemVal _t3700[] = {gem_table_get(_t3699, gem_string("value"))};
    GemVal _t3701 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3701.fn(_t3701.env, _t3700, 1);
#line 1247 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1248 "compiler/main.gem"
    GemVal _t3702 = gem_v_val_r;
    GemVal _t3703[] = {gem_v_b, gem_table_get(_t3702, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3703, 2));
#line 1249 "compiler/main.gem"
    GemVal _t3704[] = {gem_v_p};
    GemVal _t3705 = gem_v_val_r;
    GemVal _t3706[] = {gem_table_get(_t3705, gem_string("expr"))};
    GemVal _t3707[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3704, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3706, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3707, 2));
        } else {
#line 1251 "compiler/main.gem"
    GemVal _t3708[] = {gem_v_p};
    GemVal _t3709[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3708, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3709, 2));
#line 1252 "compiler/main.gem"
    GemVal _t3710 = gem_v_val_r;
    GemVal _t3711[] = {gem_v_b, gem_table_get(_t3710, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3711, 2));
#line 1253 "compiler/main.gem"
    GemVal _t3712[] = {gem_v_p};
    GemVal _t3713 = gem_v_val_r;
    GemVal _t3714[] = {gem_table_get(_t3713, gem_string("expr"))};
    GemVal _t3715[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3712, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3714, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3715, 2));
        }
#line 1255 "compiler/main.gem"
    GemVal _t3716 = gem_v_w;
        if (gem_truthy(gem_neq(gem_table_get(_t3716, gem_string("bindings")), GEM_NIL))) {
#line 1256 "compiler/main.gem"
    GemVal _t3717 = gem_v_w;
            GemVal gem_v__for_items_61 = gem_table_get(_t3717, gem_string("bindings"));
#line 1256 "compiler/main.gem"
            GemVal gem_v__for_i_61 = gem_int(0);
#line 1256 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3718[] = {gem_v__for_items_61};
                if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3718, 1)))) break;
#line 1256 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1256 "compiler/main.gem"
                gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1257 "compiler/main.gem"
    GemVal _t3719[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3720 = (*gem_v_compile_stmt);
    GemVal _t3721[] = {gem_v_b, _t3720.fn(_t3720.env, _t3719, 2)};
                (void)(gem_buf_push_fn(NULL, _t3721, 2));
#line 1258 "compiler/main.gem"
    GemVal _t3722[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3722, 2));
            }

        }
#line 1261 "compiler/main.gem"
    GemVal _t3723 = gem_v_w;
    GemVal _t3724[] = {gem_table_get(_t3723, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3725 = (*gem_v_compile_stmts_tail);
    GemVal _t3726[] = {gem_v_b, _t3725.fn(_t3725.env, _t3724, 3)};
        (void)(gem_buf_push_fn(NULL, _t3726, 2));
    }

#line 1263 "compiler/main.gem"
    GemVal _t3727 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t3727, gem_string("else")), GEM_NIL))) {
#line 1264 "compiler/main.gem"
    GemVal _t3728[] = {gem_v_p};
    GemVal _t3729[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3728, 1), gem_string("} else {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3729, 2));
#line 1265 "compiler/main.gem"
    GemVal _t3730 = gem_v_node;
    GemVal _t3731[] = {gem_table_get(_t3730, gem_string("else")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3732 = (*gem_v_compile_stmts_tail);
    GemVal _t3733[] = {gem_v_b, _t3732.fn(_t3732.env, _t3731, 3)};
        (void)(gem_buf_push_fn(NULL, _t3733, 2));
    } else {
#line 1266 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1267 "compiler/main.gem"
    GemVal _t3734[] = {gem_v_p};
    GemVal _t3735[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3734, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3735, 2));
#line 1268 "compiler/main.gem"
    GemVal _t3736 = gem_table_new();
    GemVal _t3737[] = {_t3736, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3738 = (*gem_v_compile_stmts_tail);
    GemVal _t3739[] = {gem_v_b, _t3738.fn(_t3738.env, _t3737, 3)};
            (void)(gem_buf_push_fn(NULL, _t3739, 2));
        }
    }
#line 1270 "compiler/main.gem"
    GemVal _t3740[] = {gem_v_p};
    GemVal _t3741[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3740, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3741, 2));
#line 1271 "compiler/main.gem"
    GemVal gem_v__for_i_63 = gem_int(1);
#line 1271 "compiler/main.gem"
    GemVal _t3742 = gem_v_node;
    GemVal _t3743[] = {gem_table_get(_t3742, gem_string("whens"))};
    GemVal gem_v__for_limit_63 = gem_len_fn(NULL, _t3743, 1);
#line 1271 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1271 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_63;
#line 1271 "compiler/main.gem"
        gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1272 "compiler/main.gem"
    GemVal _t3744[] = {gem_v_p};
    GemVal _t3745[] = {gem_v_b, gem_add(gem_add(gem_string("\n"), gem_to_string_fn(NULL, _t3744, 1)), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3745, 2));
    }

#line 1274 "compiler/main.gem"
    GemVal _t3746[] = {gem_v_b};
    GemVal _t3747 = gem_buf_str_fn(NULL, _t3746, 1);
    gem_pop_frame();
    return _t3747;
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
#line 1280 "compiler/main.gem"
    GemVal _t3749[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3749, 1);
#line 1281 "compiler/main.gem"
    GemVal _t3750 = gem_v_node;
    GemVal gem_v_arms = gem_table_get(_t3750, gem_string("arms"));
#line 1282 "compiler/main.gem"
    GemVal _t3751 = gem_v_node;
    GemVal gem_v_after_ms = gem_table_get(_t3751, gem_string("after_ms"));
#line 1283 "compiler/main.gem"
    GemVal _t3752 = gem_v_node;
    GemVal gem_v_after_body = gem_table_get(_t3752, gem_string("after_body"));
#line 1285 "compiler/main.gem"
    GemVal _t3753 = gem_v_node;
    GemVal _t3754[] = {gem_table_get(_t3753, gem_string("recv_var"))};
    GemVal _t3755 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3755.fn(_t3755.env, _t3754, 1);
#line 1286 "compiler/main.gem"
    GemVal _t3756 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3756.fn(_t3756.env, NULL, 0);
#line 1287 "compiler/main.gem"
    GemVal _t3757 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3757.fn(_t3757.env, NULL, 0);
#line 1288 "compiler/main.gem"
    GemVal _t3758 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3758.fn(_t3758.env, NULL, 0);
#line 1289 "compiler/main.gem"
    GemVal _t3759 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3759.fn(_t3759.env, NULL, 0);
#line 1290 "compiler/main.gem"
    GemVal _t3760 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3760.fn(_t3760.env, NULL, 0);
#line 1292 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1294 "compiler/main.gem"
    GemVal _t3761[] = {gem_v_p};
    GemVal _t3762[] = {gem_v_recv_tmp};
    GemVal _t3763[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3761, 1), gem_string("GemVal ")), gem_to_string_fn(NULL, _t3762, 1)), gem_string(" = GEM_NIL;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3763, 2));
#line 1295 "compiler/main.gem"
    GemVal _t3764[] = {gem_v_p};
    GemVal _t3765[] = {gem_v_matched_tmp};
    GemVal _t3766[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3764, 1), gem_string("int ")), gem_to_string_fn(NULL, _t3765, 1)), gem_string(" = -1;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3766, 2));
#line 1297 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1298 "compiler/main.gem"
    GemVal _t3767[] = {gem_v_after_ms};
    GemVal _t3768 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3768.fn(_t3768.env, _t3767, 1);
#line 1299 "compiler/main.gem"
    GemVal _t3769 = gem_v_ms_r;
    GemVal _t3770[] = {gem_v_b, gem_table_get(_t3769, gem_string("setup"))};
        (void)(gem_buf_push_fn(NULL, _t3770, 2));
#line 1300 "compiler/main.gem"
    GemVal _t3771[] = {gem_v_p};
    GemVal _t3772[] = {gem_v_deadline_tmp};
    GemVal _t3773 = gem_v_ms_r;
    GemVal _t3774[] = {gem_table_get(_t3773, gem_string("expr"))};
    GemVal _t3775[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3771, 1), gem_string("int64_t ")), gem_to_string_fn(NULL, _t3772, 1)), gem_string(" = gem_now_ms() + (int64_t)(")), gem_to_string_fn(NULL, _t3774, 1)), gem_string(").ival;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3775, 2));
    }
#line 1303 "compiler/main.gem"
    GemVal _t3776[] = {gem_v_p};
    GemVal _t3777[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3776, 1), gem_string("while (1) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3777, 2));
#line 1304 "compiler/main.gem"
    GemVal _t3778[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn__mod_codegen_pad_for(NULL, _t3778, 1);
#line 1306 "compiler/main.gem"
    GemVal _t3779[] = {gem_v_p2};
    GemVal _t3780[] = {gem_v_proc_tmp};
    GemVal _t3781[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3779, 1), gem_string("GemProcess *")), gem_to_string_fn(NULL, _t3780, 1)), gem_string(" = &gem_proc_table[gem_current_pid];\n"))};
    (void)(gem_buf_push_fn(NULL, _t3781, 2));
#line 1307 "compiler/main.gem"
    GemVal _t3782[] = {gem_v_p2};
    GemVal _t3783[] = {gem_v_prev_tmp};
    GemVal _t3784[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3782, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3783, 1)), gem_string(" = NULL;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3784, 2));
#line 1308 "compiler/main.gem"
    GemVal _t3785[] = {gem_v_p2};
    GemVal _t3786[] = {gem_v_cur_tmp};
    GemVal _t3787[] = {gem_v_proc_tmp};
    GemVal _t3788[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3785, 1), gem_string("GemMsgNode *")), gem_to_string_fn(NULL, _t3786, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3787, 1)), gem_string("->mailbox.head;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3788, 2));
#line 1309 "compiler/main.gem"
    GemVal _t3789[] = {gem_v_p2};
    GemVal _t3790[] = {gem_v_matched_tmp};
    GemVal _t3791[] = {gem_v_b, gem_add(gem_add(gem_to_string_fn(NULL, _t3789, 1), gem_to_string_fn(NULL, _t3790, 1)), gem_string(" = -1;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3791, 2));
#line 1310 "compiler/main.gem"
    GemVal _t3792[] = {gem_v_p2};
    GemVal _t3793[] = {gem_v_cur_tmp};
    GemVal _t3794[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3792, 1), gem_string("while (")), gem_to_string_fn(NULL, _t3793, 1)), gem_string(") {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3794, 2));
#line 1311 "compiler/main.gem"
    GemVal _t3795[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn__mod_codegen_pad_for(NULL, _t3795, 1);
#line 1312 "compiler/main.gem"
    GemVal _t3796[] = {gem_v_p3};
    GemVal _t3797[] = {gem_v_recv_tmp};
    GemVal _t3798[] = {gem_v_cur_tmp};
    GemVal _t3799[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3796, 1), gem_to_string_fn(NULL, _t3797, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3798, 1)), gem_string("->value;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3799, 2));
#line 1314 "compiler/main.gem"
    GemVal gem_v__for_i_64 = gem_int(0);
#line 1314 "compiler/main.gem"
    GemVal _t3800[] = {gem_v_arms};
    GemVal gem_v__for_limit_64 = gem_len_fn(NULL, _t3800, 1);
#line 1314 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_v__for_limit_64))) break;
#line 1314 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_64;
#line 1314 "compiler/main.gem"
        gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1315 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1316 "compiler/main.gem"
    GemVal _t3801 = gem_v_arm;
    GemVal _t3802 = gem_table_get(_t3801, gem_string("pattern"));
    GemVal _t3803[] = {gem_table_get(_t3802, gem_string("condition"))};
    GemVal _t3804 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3804.fn(_t3804.env, _t3803, 1);
#line 1317 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1318 "compiler/main.gem"
    GemVal _t3805 = gem_v_cond_r;
    GemVal _t3806[] = {gem_v_b, gem_table_get(_t3805, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3806, 2));
#line 1319 "compiler/main.gem"
    GemVal _t3807[] = {gem_v_p3};
    GemVal _t3808 = gem_v_cond_r;
    GemVal _t3809[] = {gem_table_get(_t3808, gem_string("expr"))};
    GemVal _t3810[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3807, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3809, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3810, 2));
        } else {
#line 1321 "compiler/main.gem"
    GemVal _t3811[] = {gem_v_p3};
    GemVal _t3812[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3811, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3812, 2));
#line 1322 "compiler/main.gem"
    GemVal _t3813 = gem_v_cond_r;
    GemVal _t3814[] = {gem_v_b, gem_table_get(_t3813, gem_string("setup"))};
            (void)(gem_buf_push_fn(NULL, _t3814, 2));
#line 1323 "compiler/main.gem"
    GemVal _t3815[] = {gem_v_p3};
    GemVal _t3816 = gem_v_cond_r;
    GemVal _t3817[] = {gem_table_get(_t3816, gem_string("expr"))};
    GemVal _t3818[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3815, 1), gem_string("if (gem_truthy(")), gem_to_string_fn(NULL, _t3817, 1)), gem_string(")) {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3818, 2));
        }
#line 1325 "compiler/main.gem"
    GemVal _t3819[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3820[] = {gem_fn__mod_codegen_pad_for(NULL, _t3819, 1)};
    GemVal _t3821[] = {gem_v_matched_tmp};
    GemVal _t3822[] = {gem_v_i};
    GemVal _t3823[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3820, 1), gem_to_string_fn(NULL, _t3821, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3822, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3823, 2));
    }

#line 1327 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1327 "compiler/main.gem"
    GemVal _t3824[] = {gem_v_arms};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3824, 1);
#line 1327 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1327 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1327 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1328 "compiler/main.gem"
    GemVal _t3825[] = {gem_v_p3};
    GemVal _t3826[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3825, 1), gem_string("}"))};
        (void)(gem_buf_push_fn(NULL, _t3826, 2));
#line 1329 "compiler/main.gem"
    GemVal _t3827[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3827, 1), gem_int(1))))) {
#line 1330 "compiler/main.gem"
    GemVal _t3828[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3828, 2));
        }
    }

#line 1333 "compiler/main.gem"
    GemVal _t3829[] = {gem_v_b, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t3829, 2));
#line 1335 "compiler/main.gem"
    GemVal _t3830[] = {gem_v_p3};
    GemVal _t3831[] = {gem_v_matched_tmp};
    GemVal _t3832[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3830, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3831, 1)), gem_string(" >= 0) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3832, 2));
#line 1336 "compiler/main.gem"
    GemVal _t3833[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn__mod_codegen_pad_for(NULL, _t3833, 1);
#line 1337 "compiler/main.gem"
    GemVal _t3834[] = {gem_v_p4};
    GemVal _t3835[] = {gem_v_proc_tmp};
    GemVal _t3836[] = {gem_v_prev_tmp};
    GemVal _t3837[] = {gem_v_cur_tmp};
    GemVal _t3838[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3834, 1), gem_string("gem_mailbox_remove(&")), gem_to_string_fn(NULL, _t3835, 1)), gem_string("->mailbox, ")), gem_to_string_fn(NULL, _t3836, 1)), gem_string(", ")), gem_to_string_fn(NULL, _t3837, 1)), gem_string(");\n"))};
    (void)(gem_buf_push_fn(NULL, _t3838, 2));
#line 1338 "compiler/main.gem"
    GemVal _t3839[] = {gem_v_p4};
    GemVal _t3840[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3839, 1), gem_string("break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3840, 2));
#line 1339 "compiler/main.gem"
    GemVal _t3841[] = {gem_v_p3};
    GemVal _t3842[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3841, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3842, 2));
#line 1341 "compiler/main.gem"
    GemVal _t3843[] = {gem_v_p3};
    GemVal _t3844[] = {gem_v_prev_tmp};
    GemVal _t3845[] = {gem_v_cur_tmp};
    GemVal _t3846[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3843, 1), gem_to_string_fn(NULL, _t3844, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3845, 1)), gem_string(";\n"))};
    (void)(gem_buf_push_fn(NULL, _t3846, 2));
#line 1342 "compiler/main.gem"
    GemVal _t3847[] = {gem_v_p3};
    GemVal _t3848[] = {gem_v_cur_tmp};
    GemVal _t3849[] = {gem_v_cur_tmp};
    GemVal _t3850[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3847, 1), gem_to_string_fn(NULL, _t3848, 1)), gem_string(" = ")), gem_to_string_fn(NULL, _t3849, 1)), gem_string("->next;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3850, 2));
#line 1343 "compiler/main.gem"
    GemVal _t3851[] = {gem_v_p2};
    GemVal _t3852[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3851, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3852, 2));
#line 1345 "compiler/main.gem"
    GemVal _t3853[] = {gem_v_p2};
    GemVal _t3854[] = {gem_v_matched_tmp};
    GemVal _t3855[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3853, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3854, 1)), gem_string(" >= 0) break;\n"))};
    (void)(gem_buf_push_fn(NULL, _t3855, 2));
#line 1346 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1347 "compiler/main.gem"
    GemVal _t3856[] = {gem_v_p2};
    GemVal _t3857[] = {gem_v_deadline_tmp};
    GemVal _t3858[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3856, 1), gem_string("if (gem_now_ms() >= ")), gem_to_string_fn(NULL, _t3857, 1)), gem_string(") {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3858, 2));
#line 1348 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_p2};
    GemVal _t3860[] = {gem_v_matched_tmp};
    GemVal _t3861[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3859, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3860, 1)), gem_string(" = -2;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3861, 2));
#line 1349 "compiler/main.gem"
    GemVal _t3862[] = {gem_v_p2};
    GemVal _t3863[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3862, 1), gem_string("    break;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3863, 2));
#line 1350 "compiler/main.gem"
    GemVal _t3864[] = {gem_v_p2};
    GemVal _t3865[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3864, 1), gem_string("}\n"))};
        (void)(gem_buf_push_fn(NULL, _t3865, 2));
#line 1351 "compiler/main.gem"
    GemVal _t3866[] = {gem_v_p2};
    GemVal _t3867[] = {gem_v_deadline_tmp};
    GemVal _t3868[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3866, 1), gem_string("gem_selective_yield(")), gem_to_string_fn(NULL, _t3867, 1)), gem_string(");\n"))};
        (void)(gem_buf_push_fn(NULL, _t3868, 2));
#line 1352 "compiler/main.gem"
    GemVal _t3869[] = {gem_v_p2};
    GemVal _t3870[] = {gem_v_proc_tmp};
    GemVal _t3871[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3869, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3870, 1)), gem_string("->timed_out) {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3871, 2));
#line 1353 "compiler/main.gem"
    GemVal _t3872[] = {gem_v_p2};
    GemVal _t3873[] = {gem_v_matched_tmp};
    GemVal _t3874[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3872, 1), gem_string("    ")), gem_to_string_fn(NULL, _t3873, 1)), gem_string(" = -2;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3874, 2));
#line 1354 "compiler/main.gem"
    GemVal _t3875[] = {gem_v_p2};
    GemVal _t3876[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3875, 1), gem_string("    break;\n"))};
        (void)(gem_buf_push_fn(NULL, _t3876, 2));
#line 1355 "compiler/main.gem"
    GemVal _t3877[] = {gem_v_p2};
    GemVal _t3878[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3877, 1), gem_string("}\n"))};
        (void)(gem_buf_push_fn(NULL, _t3878, 2));
    } else {
#line 1357 "compiler/main.gem"
    GemVal _t3879[] = {gem_v_p2};
    GemVal _t3880[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3879, 1), gem_string("gem_selective_yield(-1);\n"))};
        (void)(gem_buf_push_fn(NULL, _t3880, 2));
    }
#line 1360 "compiler/main.gem"
    GemVal _t3881[] = {gem_v_p};
    GemVal _t3882[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3881, 1), gem_string("}\n"))};
    (void)(gem_buf_push_fn(NULL, _t3882, 2));
#line 1363 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1363 "compiler/main.gem"
    GemVal _t3883[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3883, 1);
#line 1363 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_67, gem_v__for_limit_67))) break;
#line 1363 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_67;
#line 1363 "compiler/main.gem"
        gem_v__for_i_67 = gem_add(gem_v__for_i_67, gem_int(1));
#line 1364 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1365 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1366 "compiler/main.gem"
    GemVal _t3884[] = {gem_v_p};
    GemVal _t3885[] = {gem_v_matched_tmp};
    GemVal _t3886[] = {gem_v_i};
    GemVal _t3887[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3884, 1), gem_string("if (")), gem_to_string_fn(NULL, _t3885, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3886, 1)), gem_string(") {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3887, 2));
        } else {
#line 1368 "compiler/main.gem"
    GemVal _t3888[] = {gem_v_p};
    GemVal _t3889[] = {gem_v_matched_tmp};
    GemVal _t3890[] = {gem_v_i};
    GemVal _t3891[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3888, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3889, 1)), gem_string(" == ")), gem_to_string_fn(NULL, _t3890, 1)), gem_string(") {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3891, 2));
        }
#line 1370 "compiler/main.gem"
    GemVal _t3892 = gem_v_arm;
    GemVal _t3893 = gem_table_get(_t3892, gem_string("pattern"));
        if (gem_truthy(gem_neq(gem_table_get(_t3893, gem_string("bindings")), GEM_NIL))) {
#line 1371 "compiler/main.gem"
    GemVal _t3894 = gem_v_arm;
    GemVal _t3895 = gem_table_get(_t3894, gem_string("pattern"));
            GemVal gem_v__for_items_66 = gem_table_get(_t3895, gem_string("bindings"));
#line 1371 "compiler/main.gem"
            GemVal gem_v__for_i_66 = gem_int(0);
#line 1371 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3896[] = {gem_v__for_items_66};
                if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_len_fn(NULL, _t3896, 1)))) break;
#line 1371 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_66, gem_v__for_i_66);
#line 1371 "compiler/main.gem"
                gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1372 "compiler/main.gem"
    GemVal _t3897[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3898 = (*gem_v_compile_stmt);
    GemVal _t3899[] = {gem_v_b, _t3898.fn(_t3898.env, _t3897, 2)};
                (void)(gem_buf_push_fn(NULL, _t3899, 2));
#line 1373 "compiler/main.gem"
    GemVal _t3900[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3900, 2));
            }

        }
#line 1376 "compiler/main.gem"
    GemVal _t3901 = gem_v_arm;
    GemVal _t3902[] = {gem_table_get(_t3901, gem_string("body")), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3903 = (*gem_v_compile_stmts_tail);
    GemVal _t3904[] = {gem_v_b, _t3903.fn(_t3903.env, _t3902, 3)};
        (void)(gem_buf_push_fn(NULL, _t3904, 2));
    }

#line 1380 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1381 "compiler/main.gem"
    GemVal _t3905[] = {gem_v_p};
    GemVal _t3906[] = {gem_v_matched_tmp};
    GemVal _t3907[] = {gem_v_b, gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3905, 1), gem_string("} else if (")), gem_to_string_fn(NULL, _t3906, 1)), gem_string(" == -2) {\n"))};
        (void)(gem_buf_push_fn(NULL, _t3907, 2));
#line 1382 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1383 "compiler/main.gem"
    GemVal _t3908[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3909 = (*gem_v_compile_stmts_tail);
    GemVal _t3910[] = {gem_v_b, _t3909.fn(_t3909.env, _t3908, 3)};
            (void)(gem_buf_push_fn(NULL, _t3910, 2));
        } else {
#line 1384 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1385 "compiler/main.gem"
    GemVal _t3911 = gem_table_new();
    GemVal _t3912[] = {_t3911, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3913 = (*gem_v_compile_stmts_tail);
    GemVal _t3914[] = {gem_v_b, _t3913.fn(_t3913.env, _t3912, 3)};
                (void)(gem_buf_push_fn(NULL, _t3914, 2));
            }
        }
    } else {
#line 1387 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1388 "compiler/main.gem"
    GemVal _t3915[] = {gem_v_p};
    GemVal _t3916[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3915, 1), gem_string("} else {\n"))};
            (void)(gem_buf_push_fn(NULL, _t3916, 2));
#line 1389 "compiler/main.gem"
    GemVal _t3917 = gem_table_new();
    GemVal _t3918[] = {_t3917, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3919 = (*gem_v_compile_stmts_tail);
    GemVal _t3920[] = {gem_v_b, _t3919.fn(_t3919.env, _t3918, 3)};
            (void)(gem_buf_push_fn(NULL, _t3920, 2));
        }
    }
#line 1392 "compiler/main.gem"
    GemVal _t3921[] = {gem_v_p};
    GemVal _t3922[] = {gem_v_b, gem_add(gem_to_string_fn(NULL, _t3921, 1), gem_string("}"))};
    (void)(gem_buf_push_fn(NULL, _t3922, 2));
#line 1393 "compiler/main.gem"
    GemVal _t3923[] = {gem_v_b};
    GemVal _t3924 = gem_buf_str_fn(NULL, _t3923, 1);
    gem_pop_frame();
    return _t3924;
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
#line 1400 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1401 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1402 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1403 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1404 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1405 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1406 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1407 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1408 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1409 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1410 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1411 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1412 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1415 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1416 "compiler/main.gem"
    GemVal _t3926[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3926, 1), gem_int(0)))) {
#line 1417 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1419 "compiler/main.gem"
        GemVal gem_v__for_i_68 = gem_int(0);
#line 1419 "compiler/main.gem"
    GemVal _t3927[] = {gem_v_params};
        GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t3927, 1);
#line 1419 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1419 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_68;
#line 1419 "compiler/main.gem"
            gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1420 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1421 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1423 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1424 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1425 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1426 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1427 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1428 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1429 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1430 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1431 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1432 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1433 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1434 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1435 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1437 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1442 "compiler/main.gem"
    GemVal _t3928[] = {gem_v_c_ret};
    GemVal _t3929[] = {gem_v_name};
    GemVal _t3930[] = {gem_v_c_params};
    GemVal _t3931[] = {(*gem_v_extern_c_decls), gem_add(gem_add(gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3928, 1), gem_string(" ")), gem_to_string_fn(NULL, _t3929, 1)), gem_string("(")), gem_to_string_fn(NULL, _t3930, 1)), gem_string(");"))};
    GemVal _t3932 = gem_push_fn(NULL, _t3931, 2);
    gem_pop_frame();
    return _t3932;
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
#line 1446 "compiler/main.gem"
    GemVal gem_v__d69 = gem_v_node;
#line 1446 "compiler/main.gem"
    GemVal _t3934 = gem_v__d69;
    GemVal gem_v_name = gem_table_get(_t3934, gem_string("name"));
#line 1446 "compiler/main.gem"
    GemVal _t3935 = gem_v__d69;
    GemVal gem_v_params = gem_table_get(_t3935, gem_string("params"));
#line 1446 "compiler/main.gem"
    GemVal _t3936 = gem_v__d69;
    GemVal gem_v_ret_type = gem_table_get(_t3936, gem_string("ret_type"));

#line 1448 "compiler/main.gem"
    GemVal _t3937 = gem_v_node;
    if (gem_truthy(gem_table_get(_t3937, gem_string("blocking")))) {
#line 1449 "compiler/main.gem"
    GemVal _t3938[] = {gem_v_node};
    GemVal _t3939 = (*gem_v_compile_blocking_extern_fn);
        GemVal _t3940 = _t3939.fn(_t3939.env, _t3938, 1);
        gem_pop_frame();
        return _t3940;
    }
#line 1452 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1453 "compiler/main.gem"
    GemVal _t3941[] = {gem_v_name};
    GemVal _t3942[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t3941, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t3942, 2));
#line 1455 "compiler/main.gem"
    GemVal _t3943 = gem_table_new();
    GemVal gem_v_c_args = _t3943;
#line 1456 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1456 "compiler/main.gem"
    GemVal _t3944[] = {gem_v_params};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3944, 1);
#line 1456 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1456 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1456 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1457 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1458 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1459 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1460 "compiler/main.gem"
    GemVal _t3945[] = {gem_v_i};
    GemVal _t3946[] = {gem_v_i};
    GemVal _t3947[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t3945, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3946, 1)), gem_string("].ival;\n"))};
            (void)(gem_buf_push_fn(NULL, _t3947, 2));
#line 1461 "compiler/main.gem"
    GemVal _t3948[] = {gem_v_i};
    GemVal _t3949[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3948, 1))};
            (void)(gem_push_fn(NULL, _t3949, 2));
        } else {
#line 1462 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1463 "compiler/main.gem"
    GemVal _t3950[] = {gem_v_i};
    GemVal _t3951[] = {gem_v_i};
    GemVal _t3952[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t3950, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3951, 1)), gem_string("].fval;\n"))};
                (void)(gem_buf_push_fn(NULL, _t3952, 2));
#line 1464 "compiler/main.gem"
    GemVal _t3953[] = {gem_v_i};
    GemVal _t3954[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3953, 1))};
                (void)(gem_push_fn(NULL, _t3954, 2));
            } else {
#line 1465 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1466 "compiler/main.gem"
    GemVal _t3955[] = {gem_v_i};
    GemVal _t3956[] = {gem_v_i};
    GemVal _t3957[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t3955, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3956, 1)), gem_string("].sval;\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3957, 2));
#line 1467 "compiler/main.gem"
    GemVal _t3958[] = {gem_v_i};
    GemVal _t3959[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3958, 1))};
                    (void)(gem_push_fn(NULL, _t3959, 2));
                } else {
#line 1468 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1469 "compiler/main.gem"
    GemVal _t3960[] = {gem_v_i};
    GemVal _t3961[] = {gem_v_i};
    GemVal _t3962[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t3960, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t3961, 1)), gem_string("].bval;\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3962, 2));
#line 1470 "compiler/main.gem"
    GemVal _t3963[] = {gem_v_i};
    GemVal _t3964[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3963, 1))};
                        (void)(gem_push_fn(NULL, _t3964, 2));
                    } else {
#line 1471 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1472 "compiler/main.gem"
    GemVal _t3965[] = {gem_v_i};
    GemVal _t3966[] = {gem_v_i};
    GemVal _t3967[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t3965, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t3966, 1)), gem_string("].ival;\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3967, 2));
#line 1473 "compiler/main.gem"
    GemVal _t3968[] = {gem_v_i};
    GemVal _t3969[] = {gem_v_c_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t3968, 1))};
                            (void)(gem_push_fn(NULL, _t3969, 2));
                        } else {
#line 1474 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1475 "compiler/main.gem"
    GemVal _t3970[] = {gem_v_i};
    GemVal _t3971[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3970, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3971, 2));
                            } else {
#line 1477 "compiler/main.gem"
    GemVal _t3972[] = {gem_v_i};
    GemVal _t3973[] = {gem_v_c_args, gem_add(gem_add(gem_string("args["), gem_to_string_fn(NULL, _t3972, 1)), gem_string("]"))};
                                (void)(gem_push_fn(NULL, _t3973, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1482 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1483 "compiler/main.gem"
    GemVal gem_v__for_i_71 = gem_int(0);
#line 1483 "compiler/main.gem"
    GemVal _t3974[] = {gem_v_c_args};
    GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3974, 1);
#line 1483 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1483 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_71;
#line 1483 "compiler/main.gem"
        gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1484 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1485 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1487 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1490 "compiler/main.gem"
    GemVal _t3975[] = {gem_v_name};
    GemVal _t3976[] = {gem_v_call_args};
    GemVal gem_v_call_expr = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t3975, 1), gem_string("(")), gem_to_string_fn(NULL, _t3976, 1)), gem_string(")"));
#line 1492 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1493 "compiler/main.gem"
    GemVal _t3977[] = {gem_v_call_expr};
    GemVal _t3978[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3977, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t3978, 2));
#line 1494 "compiler/main.gem"
    GemVal _t3979[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3979, 2));
    } else {
#line 1495 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1496 "compiler/main.gem"
    GemVal _t3980[] = {gem_v_call_expr};
    GemVal _t3981[] = {gem_v_b, gem_add(gem_add(gem_string("    return "), gem_to_string_fn(NULL, _t3980, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t3981, 2));
        } else {
#line 1497 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1498 "compiler/main.gem"
    GemVal _t3982[] = {gem_v_call_expr};
    GemVal _t3983[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t3982, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t3983, 2));
#line 1499 "compiler/main.gem"
    GemVal _t3984[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3984, 2));
            } else {
#line 1500 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1501 "compiler/main.gem"
    GemVal _t3985[] = {gem_v_call_expr};
    GemVal _t3986[] = {gem_v_b, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t3985, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t3986, 2));
#line 1502 "compiler/main.gem"
    GemVal _t3987[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3987, 2));
                } else {
#line 1503 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1504 "compiler/main.gem"
    GemVal _t3988[] = {gem_v_call_expr};
    GemVal _t3989[] = {gem_v_b, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t3988, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t3989, 2));
#line 1505 "compiler/main.gem"
    GemVal _t3990[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3990, 2));
                    } else {
#line 1506 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1507 "compiler/main.gem"
    GemVal _t3991[] = {gem_v_call_expr};
    GemVal _t3992[] = {gem_v_b, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t3991, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t3992, 2));
#line 1508 "compiler/main.gem"
    GemVal _t3993[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                            (void)(gem_buf_push_fn(NULL, _t3993, 2));
                        } else {
#line 1509 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1510 "compiler/main.gem"
    GemVal _t3994[] = {gem_v_call_expr};
    GemVal _t3995[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t3994, 1)), gem_string(";\n"))};
                                (void)(gem_buf_push_fn(NULL, _t3995, 2));
#line 1511 "compiler/main.gem"
    GemVal _t3996[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3996, 2));
                            } else {
#line 1513 "compiler/main.gem"
    GemVal _t3997[] = {gem_v_call_expr};
    GemVal _t3998[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t3997, 1)), gem_string(";\n"))};
                                (void)(gem_buf_push_fn(NULL, _t3998, 2));
#line 1514 "compiler/main.gem"
    GemVal _t3999[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3999, 2));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1517 "compiler/main.gem"
    GemVal _t4000[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4000, 2));
#line 1519 "compiler/main.gem"
    GemVal _t4001[] = {gem_v_name};
    GemVal _t4002[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4001, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t4002, 2));
#line 1520 "compiler/main.gem"
    GemVal _t4003[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t4004 = (*gem_v_emit_extern_c_decl);
    (void)(_t4004.fn(_t4004.env, _t4003, 3));
#line 1522 "compiler/main.gem"
    GemVal _t4005[] = {gem_v_b};
    GemVal _t4006 = gem_buf_str_fn(NULL, _t4005, 1);
    gem_pop_frame();
    return _t4006;
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
#line 1529 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1529 "compiler/main.gem"
    GemVal _t4008 = gem_v__d72;
    GemVal gem_v_name = gem_table_get(_t4008, gem_string("name"));
#line 1529 "compiler/main.gem"
    GemVal _t4009 = gem_v__d72;
    GemVal gem_v_params = gem_table_get(_t4009, gem_string("params"));
#line 1529 "compiler/main.gem"
    GemVal _t4010 = gem_v__d72;
    GemVal gem_v_ret_type = gem_table_get(_t4010, gem_string("ret_type"));

#line 1530 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1533 "compiler/main.gem"
    GemVal _t4011[] = {gem_v_name};
    GemVal _t4012[] = {gem_v_b, gem_add(gem_add(gem_string("struct _blk_"), gem_to_string_fn(NULL, _t4011, 1)), gem_string("_args {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4012, 2));
#line 1534 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1534 "compiler/main.gem"
    GemVal _t4013[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t4013, 1);
#line 1534 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1534 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1534 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1535 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1536 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1537 "compiler/main.gem"
    GemVal _t4014[] = {gem_v_i};
    GemVal _t4015[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t4014, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4015, 2));
        } else {
#line 1538 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1539 "compiler/main.gem"
    GemVal _t4016[] = {gem_v_i};
    GemVal _t4017[] = {gem_v_b, gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t4016, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t4017, 2));
            } else {
#line 1540 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1541 "compiler/main.gem"
    GemVal _t4018[] = {gem_v_i};
    GemVal _t4019[] = {gem_v_b, gem_add(gem_add(gem_string("    char *_p"), gem_to_string_fn(NULL, _t4018, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4019, 2));
                } else {
#line 1542 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1543 "compiler/main.gem"
    GemVal _t4020[] = {gem_v_i};
    GemVal _t4021[] = {gem_v_b, gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t4020, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4021, 2));
                    } else {
#line 1544 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1545 "compiler/main.gem"
    GemVal _t4022[] = {gem_v_i};
    GemVal _t4023[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_p"), gem_to_string_fn(NULL, _t4022, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4023, 2));
                        }
                    }
                }
            }
        }
    }

#line 1548 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1549 "compiler/main.gem"
    GemVal _t4024[] = {gem_v_b, gem_string("    int64_t _result;\n")};
        (void)(gem_buf_push_fn(NULL, _t4024, 2));
    } else {
#line 1550 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1551 "compiler/main.gem"
    GemVal _t4025[] = {gem_v_b, gem_string("    double _result;\n")};
            (void)(gem_buf_push_fn(NULL, _t4025, 2));
        } else {
#line 1552 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1553 "compiler/main.gem"
    GemVal _t4026[] = {gem_v_b, gem_string("    char *_result;\n")};
                (void)(gem_buf_push_fn(NULL, _t4026, 2));
            } else {
#line 1554 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1555 "compiler/main.gem"
    GemVal _t4027[] = {gem_v_b, gem_string("    int _result;\n")};
                    (void)(gem_buf_push_fn(NULL, _t4027, 2));
                } else {
#line 1556 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1557 "compiler/main.gem"
    GemVal _t4028[] = {gem_v_b, gem_string("    void *_result;\n")};
                        (void)(gem_buf_push_fn(NULL, _t4028, 2));
                    }
                }
            }
        }
    }
#line 1559 "compiler/main.gem"
    GemVal _t4029[] = {gem_v_b, gem_string("};\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4029, 2));
#line 1562 "compiler/main.gem"
    GemVal _t4030[] = {gem_v_name};
    GemVal _t4031[] = {gem_v_b, gem_add(gem_add(gem_string("static void _blk_"), gem_to_string_fn(NULL, _t4030, 1)), gem_string("_worker(void *_a) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4031, 2));
#line 1563 "compiler/main.gem"
    GemVal _t4032[] = {gem_v_name};
    GemVal _t4033[] = {gem_v_name};
    GemVal _t4034[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    struct _blk_"), gem_to_string_fn(NULL, _t4032, 1)), gem_string("_args *a = (struct _blk_")), gem_to_string_fn(NULL, _t4033, 1)), gem_string("_args *)_a;\n"))};
    (void)(gem_buf_push_fn(NULL, _t4034, 2));
#line 1565 "compiler/main.gem"
    GemVal gem_v_w_args = gem_string("");
#line 1566 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1566 "compiler/main.gem"
    GemVal _t4035[] = {gem_v_params};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t4035, 1);
#line 1566 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1566 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1566 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1567 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1568 "compiler/main.gem"
            gem_v_w_args = gem_add(gem_v_w_args, gem_string(", "));
        }
#line 1570 "compiler/main.gem"
    GemVal _t4036[] = {gem_v_i};
        gem_v_w_args = gem_add(gem_v_w_args, gem_add(gem_string("a->_p"), gem_to_string_fn(NULL, _t4036, 1)));
    }

#line 1572 "compiler/main.gem"
    GemVal _t4037[] = {gem_v_name};
    GemVal _t4038[] = {gem_v_w_args};
    GemVal gem_v_w_call = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t4037, 1), gem_string("(")), gem_to_string_fn(NULL, _t4038, 1)), gem_string(")"));
#line 1574 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1575 "compiler/main.gem"
    GemVal _t4039[] = {gem_v_w_call};
    GemVal _t4040[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4039, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4040, 2));
    } else {
#line 1577 "compiler/main.gem"
    GemVal _t4041[] = {gem_v_w_call};
    GemVal _t4042[] = {gem_v_b, gem_add(gem_add(gem_string("    a->_result = "), gem_to_string_fn(NULL, _t4041, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4042, 2));
    }
#line 1579 "compiler/main.gem"
    GemVal _t4043[] = {gem_v_b, gem_string("}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4043, 2));
#line 1582 "compiler/main.gem"
    GemVal _t4044[] = {gem_v_name};
    GemVal _t4045[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4044, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4045, 2));
#line 1585 "compiler/main.gem"
    GemVal gem_v__for_i_75 = gem_int(0);
#line 1585 "compiler/main.gem"
    GemVal _t4046[] = {gem_v_params};
    GemVal gem_v__for_limit_75 = gem_len_fn(NULL, _t4046, 1);
#line 1585 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_75, gem_v__for_limit_75))) break;
#line 1585 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_75;
#line 1585 "compiler/main.gem"
        gem_v__for_i_75 = gem_add(gem_v__for_i_75, gem_int(1));
#line 1586 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1587 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1588 "compiler/main.gem"
    GemVal _t4047[] = {gem_v_i};
    GemVal _t4048[] = {gem_v_i};
    GemVal _t4049[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int64_t _p"), gem_to_string_fn(NULL, _t4047, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4048, 1)), gem_string("].ival;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4049, 2));
        } else {
#line 1589 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1590 "compiler/main.gem"
    GemVal _t4050[] = {gem_v_i};
    GemVal _t4051[] = {gem_v_i};
    GemVal _t4052[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    double _p"), gem_to_string_fn(NULL, _t4050, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4051, 1)), gem_string("].fval;\n"))};
                (void)(gem_buf_push_fn(NULL, _t4052, 2));
            } else {
#line 1591 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1592 "compiler/main.gem"
    GemVal _t4053[] = {gem_v_i};
    GemVal _t4054[] = {gem_v_i};
    GemVal _t4055[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    char* _p"), gem_to_string_fn(NULL, _t4053, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4054, 1)), gem_string("].sval;\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4055, 2));
                } else {
#line 1593 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1594 "compiler/main.gem"
    GemVal _t4056[] = {gem_v_i};
    GemVal _t4057[] = {gem_v_i};
    GemVal _t4058[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    int _p"), gem_to_string_fn(NULL, _t4056, 1)), gem_string(" = args[")), gem_to_string_fn(NULL, _t4057, 1)), gem_string("].bval;\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4058, 2));
                    } else {
#line 1595 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1596 "compiler/main.gem"
    GemVal _t4059[] = {gem_v_i};
    GemVal _t4060[] = {gem_v_i};
    GemVal _t4061[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("    void* _p"), gem_to_string_fn(NULL, _t4059, 1)), gem_string(" = (void*)(intptr_t)args[")), gem_to_string_fn(NULL, _t4060, 1)), gem_string("].ival;\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4061, 2));
                        }
                    }
                }
            }
        }
    }

#line 1601 "compiler/main.gem"
    GemVal _t4062[] = {gem_v_b, gem_string("    if (gem_current_pid >= 0) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4062, 2));
#line 1602 "compiler/main.gem"
    GemVal _t4063[] = {gem_v_name};
    GemVal _t4064[] = {gem_v_name};
    GemVal _t4065[] = {gem_v_name};
    GemVal _t4066[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("        struct _blk_"), gem_to_string_fn(NULL, _t4063, 1)), gem_string("_args *_ba = (struct _blk_")), gem_to_string_fn(NULL, _t4064, 1)), gem_string("_args *)malloc(sizeof(struct _blk_")), gem_to_string_fn(NULL, _t4065, 1)), gem_string("_args));\n"))};
    (void)(gem_buf_push_fn(NULL, _t4066, 2));
#line 1605 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1605 "compiler/main.gem"
    GemVal _t4067[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t4067, 1);
#line 1605 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1605 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1605 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1606 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1607 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1608 "compiler/main.gem"
    GemVal _t4068[] = {gem_v_i};
    GemVal _t4069[] = {gem_v_i};
    GemVal _t4070[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        _ba->_p"), gem_to_string_fn(NULL, _t4068, 1)), gem_string(" = strdup(_p")), gem_to_string_fn(NULL, _t4069, 1)), gem_string(");\n"))};
            (void)(gem_buf_push_fn(NULL, _t4070, 2));
        } else {
#line 1610 "compiler/main.gem"
    GemVal _t4071[] = {gem_v_i};
    GemVal _t4072[] = {gem_v_i};
    GemVal _t4073[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("        _ba->_p"), gem_to_string_fn(NULL, _t4071, 1)), gem_string(" = _p")), gem_to_string_fn(NULL, _t4072, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4073, 2));
        }
    }

#line 1614 "compiler/main.gem"
    GemVal _t4074[] = {gem_v_name};
    GemVal _t4075[] = {gem_v_b, gem_add(gem_add(gem_string("        GemIORequest *_req = gem_io_submit_extern(_blk_"), gem_to_string_fn(NULL, _t4074, 1)), gem_string("_worker, _ba);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4075, 2));
#line 1615 "compiler/main.gem"
    GemVal _t4076[] = {gem_v_name};
    GemVal _t4077[] = {gem_v_b, gem_add(gem_add(gem_string("        if (!_req) { free(_ba); gem_error(\""), gem_to_string_fn(NULL, _t4076, 1)), gem_string(": I/O queue full\"); }\n"))};
    (void)(gem_buf_push_fn(NULL, _t4077, 2));
#line 1616 "compiler/main.gem"
    GemVal _t4078[] = {gem_v_b, gem_string("        GemProcess *_proc = &gem_proc_table[gem_current_pid];\n")};
    (void)(gem_buf_push_fn(NULL, _t4078, 2));
#line 1617 "compiler/main.gem"
    GemVal _t4079[] = {gem_v_b, gem_string("        _proc->io_request = _req;\n")};
    (void)(gem_buf_push_fn(NULL, _t4079, 2));
#line 1618 "compiler/main.gem"
    GemVal _t4080[] = {gem_v_b, gem_string("        gem_io_pool_yield();\n")};
    (void)(gem_buf_push_fn(NULL, _t4080, 2));
#line 1619 "compiler/main.gem"
    GemVal _t4081[] = {gem_v_b, gem_string("        _proc->io_request = NULL;\n")};
    (void)(gem_buf_push_fn(NULL, _t4081, 2));
#line 1622 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1623 "compiler/main.gem"
    GemVal _t4082[] = {gem_v_b, gem_string("        char *_raw = _ba->_result;\n")};
        (void)(gem_buf_push_fn(NULL, _t4082, 2));
    } else {
#line 1624 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_ret_type, gem_string("Nil")))) {
#line 1625 "compiler/main.gem"
            GemVal gem_v_c_type = gem_string("int64_t");
#line 1626 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1627 "compiler/main.gem"
                gem_v_c_type = gem_string("double");
            } else {
#line 1628 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1629 "compiler/main.gem"
                    gem_v_c_type = gem_string("int");
                } else {
#line 1630 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1631 "compiler/main.gem"
                        gem_v_c_type = gem_string("void *");
                    }
                }
            }
#line 1633 "compiler/main.gem"
    GemVal _t4083[] = {gem_v_c_type};
    GemVal _t4084[] = {gem_v_b, gem_add(gem_add(gem_string("        "), gem_to_string_fn(NULL, _t4083, 1)), gem_string(" _ret = _ba->_result;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4084, 2));
        }
    }
#line 1637 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1637 "compiler/main.gem"
    GemVal _t4085[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t4085, 1);
#line 1637 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1637 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1637 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1638 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type")), gem_string("String")))) {
#line 1639 "compiler/main.gem"
    GemVal _t4086[] = {gem_v_i};
    GemVal _t4087[] = {gem_v_b, gem_add(gem_add(gem_string("        free(_ba->_p"), gem_to_string_fn(NULL, _t4086, 1)), gem_string(");\n"))};
            (void)(gem_buf_push_fn(NULL, _t4087, 2));
        }
    }

#line 1643 "compiler/main.gem"
    GemVal _t4088[] = {gem_v_b, gem_string("        free(_ba);\n")};
    (void)(gem_buf_push_fn(NULL, _t4088, 2));
#line 1644 "compiler/main.gem"
    GemVal _t4089[] = {gem_v_b, gem_string("        gem_io_free_request(_req);\n")};
    (void)(gem_buf_push_fn(NULL, _t4089, 2));
#line 1647 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1648 "compiler/main.gem"
    GemVal _t4090[] = {gem_v_b, gem_string("        return gem_int(_ret);\n")};
        (void)(gem_buf_push_fn(NULL, _t4090, 2));
    } else {
#line 1649 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1650 "compiler/main.gem"
    GemVal _t4091[] = {gem_v_b, gem_string("        return gem_float(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t4091, 2));
        } else {
#line 1651 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1652 "compiler/main.gem"
    GemVal _t4092[] = {gem_v_b, gem_string("        size_t _len = _raw ? strlen(_raw) : 0;\n")};
                (void)(gem_buf_push_fn(NULL, _t4092, 2));
#line 1653 "compiler/main.gem"
    GemVal _t4093[] = {gem_v_b, gem_string("        char *_gc = (char *)GC_MALLOC_ATOMIC(_len + 1);\n")};
                (void)(gem_buf_push_fn(NULL, _t4093, 2));
#line 1654 "compiler/main.gem"
    GemVal _t4094[] = {gem_v_b, gem_string("        if (_raw) { memcpy(_gc, _raw, _len + 1); free(_raw); } else { _gc[0] = '\\0'; }\n")};
                (void)(gem_buf_push_fn(NULL, _t4094, 2));
#line 1655 "compiler/main.gem"
    GemVal _t4095[] = {gem_v_b, gem_string("        return gem_string(_gc);\n")};
                (void)(gem_buf_push_fn(NULL, _t4095, 2));
            } else {
#line 1656 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1657 "compiler/main.gem"
    GemVal _t4096[] = {gem_v_b, gem_string("        return gem_bool(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t4096, 2));
                } else {
#line 1658 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1659 "compiler/main.gem"
    GemVal _t4097[] = {gem_v_b, gem_string("        GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                        (void)(gem_buf_push_fn(NULL, _t4097, 2));
                    } else {
#line 1661 "compiler/main.gem"
    GemVal _t4098[] = {gem_v_b, gem_string("        return GEM_NIL;\n")};
                        (void)(gem_buf_push_fn(NULL, _t4098, 2));
                    }
                }
            }
        }
    }
#line 1663 "compiler/main.gem"
    GemVal _t4099[] = {gem_v_b, gem_string("    }\n")};
    (void)(gem_buf_push_fn(NULL, _t4099, 2));
#line 1666 "compiler/main.gem"
    GemVal gem_v_d_args = gem_string("");
#line 1667 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1667 "compiler/main.gem"
    GemVal _t4100[] = {gem_v_params};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t4100, 1);
#line 1667 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1667 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1667 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1668 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1669 "compiler/main.gem"
            gem_v_d_args = gem_add(gem_v_d_args, gem_string(", "));
        }
#line 1671 "compiler/main.gem"
    GemVal _t4101[] = {gem_v_i};
        gem_v_d_args = gem_add(gem_v_d_args, gem_add(gem_string("_p"), gem_to_string_fn(NULL, _t4101, 1)));
    }

#line 1673 "compiler/main.gem"
    GemVal _t4102[] = {gem_v_name};
    GemVal _t4103[] = {gem_v_d_args};
    GemVal gem_v_d_call = gem_add(gem_add(gem_add(gem_to_string_fn(NULL, _t4102, 1), gem_string("(")), gem_to_string_fn(NULL, _t4103, 1)), gem_string(")"));
#line 1675 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1676 "compiler/main.gem"
    GemVal _t4104[] = {gem_v_d_call};
    GemVal _t4105[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4104, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4105, 2));
#line 1677 "compiler/main.gem"
    GemVal _t4106[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4106, 2));
    } else {
#line 1678 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1679 "compiler/main.gem"
    GemVal _t4107[] = {gem_v_d_call};
    GemVal _t4108[] = {gem_v_b, gem_add(gem_add(gem_string("    int64_t _ret = "), gem_to_string_fn(NULL, _t4107, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4108, 2));
#line 1680 "compiler/main.gem"
    GemVal _t4109[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t4109, 2));
        } else {
#line 1681 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1682 "compiler/main.gem"
    GemVal _t4110[] = {gem_v_d_call};
    GemVal _t4111[] = {gem_v_b, gem_add(gem_add(gem_string("    double _ret = "), gem_to_string_fn(NULL, _t4110, 1)), gem_string(";\n"))};
                (void)(gem_buf_push_fn(NULL, _t4111, 2));
#line 1683 "compiler/main.gem"
    GemVal _t4112[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t4112, 2));
            } else {
#line 1684 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1685 "compiler/main.gem"
    GemVal _t4113[] = {gem_v_d_call};
    GemVal _t4114[] = {gem_v_b, gem_add(gem_add(gem_string("    char* _ret = "), gem_to_string_fn(NULL, _t4113, 1)), gem_string(";\n"))};
                    (void)(gem_buf_push_fn(NULL, _t4114, 2));
#line 1686 "compiler/main.gem"
    GemVal _t4115[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t4115, 2));
                } else {
#line 1687 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1688 "compiler/main.gem"
    GemVal _t4116[] = {gem_v_d_call};
    GemVal _t4117[] = {gem_v_b, gem_add(gem_add(gem_string("    int _ret = "), gem_to_string_fn(NULL, _t4116, 1)), gem_string(";\n"))};
                        (void)(gem_buf_push_fn(NULL, _t4117, 2));
#line 1689 "compiler/main.gem"
    GemVal _t4118[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t4118, 2));
                    } else {
#line 1690 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1691 "compiler/main.gem"
    GemVal _t4119[] = {gem_v_d_call};
    GemVal _t4120[] = {gem_v_b, gem_add(gem_add(gem_string("    void *_ret = "), gem_to_string_fn(NULL, _t4119, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4120, 2));
#line 1692 "compiler/main.gem"
    GemVal _t4121[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                            (void)(gem_buf_push_fn(NULL, _t4121, 2));
                        } else {
#line 1694 "compiler/main.gem"
    GemVal _t4122[] = {gem_v_d_call};
    GemVal _t4123[] = {gem_v_b, gem_add(gem_add(gem_string("    "), gem_to_string_fn(NULL, _t4122, 1)), gem_string(";\n"))};
                            (void)(gem_buf_push_fn(NULL, _t4123, 2));
#line 1695 "compiler/main.gem"
    GemVal _t4124[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                            (void)(gem_buf_push_fn(NULL, _t4124, 2));
                        }
                    }
                }
            }
        }
    }
#line 1698 "compiler/main.gem"
    GemVal _t4125[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4125, 2));
#line 1700 "compiler/main.gem"
    GemVal _t4126[] = {gem_v_name};
    GemVal _t4127[] = {(*gem_v_forward_decls), gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4126, 1)), gem_string("(void *_env, GemVal *args, int argc);"))};
    (void)(gem_push_fn(NULL, _t4127, 2));
#line 1701 "compiler/main.gem"
    GemVal _t4128[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t4129 = (*gem_v_emit_extern_c_decl);
    (void)(_t4129.fn(_t4129.env, _t4128, 3));
#line 1703 "compiler/main.gem"
    GemVal _t4130[] = {gem_v_b};
    GemVal _t4131 = gem_buf_str_fn(NULL, _t4130, 1);
    gem_pop_frame();
    return _t4131;
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
#line 1709 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1711 "compiler/main.gem"
    GemVal gem_v__for_i_79 = gem_int(0);
#line 1711 "compiler/main.gem"
    GemVal _t4133[] = {gem_v_params};
    GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t4133, 1);
#line 1711 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1711 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_79;
#line 1711 "compiler/main.gem"
        gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1712 "compiler/main.gem"
    GemVal _t4134[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t4135 = (*gem_v_mangle);
        GemVal gem_v_mp = _t4135.fn(_t4135.env, _t4134, 1);
#line 1713 "compiler/main.gem"
    GemVal _t4136[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4136, 2))) {
#line 1714 "compiler/main.gem"
    GemVal _t4137[] = {gem_v_mp};
    GemVal _t4138[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4137, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
            (void)(gem_buf_push_fn(NULL, _t4138, 2));
#line 1715 "compiler/main.gem"
    GemVal _t4139[] = {gem_v_mp};
    GemVal _t4140[] = {gem_v_i};
    GemVal _t4141[] = {gem_v_i};
    GemVal _t4142[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t4139, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t4140, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t4141, 1)), gem_string("] : GEM_NIL;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4142, 2));
        } else {
#line 1717 "compiler/main.gem"
    GemVal _t4143[] = {gem_v_mp};
    GemVal _t4144[] = {gem_v_i};
    GemVal _t4145[] = {gem_v_i};
    GemVal _t4146[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t4143, 1)), gem_string(" = (argc > ")), gem_to_string_fn(NULL, _t4144, 1)), gem_string(") ? args[")), gem_to_string_fn(NULL, _t4145, 1)), gem_string("] : GEM_NIL;\n"))};
            (void)(gem_buf_push_fn(NULL, _t4146, 2));
        }
    }

#line 1722 "compiler/main.gem"
    GemVal _t4147 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4147, gem_string("rest_param")), GEM_NIL))) {
#line 1723 "compiler/main.gem"
    GemVal _t4148[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t4148, 1);
#line 1724 "compiler/main.gem"
    GemVal _t4149 = gem_v_node;
    GemVal _t4150[] = {gem_table_get(_t4149, gem_string("rest_param"))};
    GemVal _t4151 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t4151.fn(_t4151.env, _t4150, 1);
#line 1725 "compiler/main.gem"
    GemVal _t4152 = gem_v_node;
    GemVal _t4153[] = {gem_v_cap_set, gem_table_get(_t4152, gem_string("rest_param"))};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4153, 2))) {
#line 1726 "compiler/main.gem"
    GemVal _t4154[] = {gem_v_mp_rest};
    GemVal _t4155[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4154, 1)), gem_string(" = GC_MALLOC(sizeof(GemVal));\n"))};
            (void)(gem_buf_push_fn(NULL, _t4155, 2));
#line 1727 "compiler/main.gem"
    GemVal _t4156[] = {gem_v_mp_rest};
    GemVal _t4157[] = {gem_v_b, gem_add(gem_add(gem_string("    *"), gem_to_string_fn(NULL, _t4156, 1)), gem_string(" = gem_table_new();\n"))};
            (void)(gem_buf_push_fn(NULL, _t4157, 2));
#line 1728 "compiler/main.gem"
    GemVal _t4158[] = {gem_v_N};
    GemVal _t4159[] = {gem_v_mp_rest};
    GemVal _t4160[] = {gem_v_N};
    GemVal _t4161[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t4158, 1)), gem_string("; _ri < argc; _ri++) gem_table_set(*")), gem_to_string_fn(NULL, _t4159, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t4160, 1)), gem_string("), args[_ri]);\n"))};
            (void)(gem_buf_push_fn(NULL, _t4161, 2));
        } else {
#line 1730 "compiler/main.gem"
    GemVal _t4162[] = {gem_v_mp_rest};
    GemVal _t4163[] = {gem_v_b, gem_add(gem_add(gem_string("    GemVal "), gem_to_string_fn(NULL, _t4162, 1)), gem_string(" = gem_table_new();\n"))};
            (void)(gem_buf_push_fn(NULL, _t4163, 2));
#line 1731 "compiler/main.gem"
    GemVal _t4164[] = {gem_v_N};
    GemVal _t4165[] = {gem_v_mp_rest};
    GemVal _t4166[] = {gem_v_N};
    GemVal _t4167[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    for (int _ri = "), gem_to_string_fn(NULL, _t4164, 1)), gem_string("; _ri < argc; _ri++) gem_table_set(")), gem_to_string_fn(NULL, _t4165, 1)), gem_string(", gem_int(_ri - ")), gem_to_string_fn(NULL, _t4166, 1)), gem_string("), args[_ri]);\n"))};
            (void)(gem_buf_push_fn(NULL, _t4167, 2));
        }
    }
#line 1734 "compiler/main.gem"
    GemVal _t4168[] = {gem_v_b};
    GemVal _t4169 = gem_buf_str_fn(NULL, _t4168, 1);
    gem_pop_frame();
    return _t4169;
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
#line 1740 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1741 "compiler/main.gem"
    GemVal _t4171[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4171, 1), gem_int(0)))) {
#line 1742 "compiler/main.gem"
        GemVal gem_v__for_i_80 = gem_int(0);
#line 1742 "compiler/main.gem"
    GemVal _t4172[] = {gem_v_body};
        GemVal gem_v__for_limit_80 = gem_sub(gem_len_fn(NULL, _t4172, 1), gem_int(1));
#line 1742 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1742 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_80;
#line 1742 "compiler/main.gem"
            gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1743 "compiler/main.gem"
    GemVal _t4173[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t4174 = (*gem_v_compile_stmt);
    GemVal _t4175[] = {gem_v_b, _t4174.fn(_t4174.env, _t4173, 2)};
            (void)(gem_buf_push_fn(NULL, _t4175, 2));
#line 1744 "compiler/main.gem"
    GemVal _t4176[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4176, 2));
        }

#line 1746 "compiler/main.gem"
    GemVal _t4177[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t4178 = (*gem_v_compile_stmt_return);
    GemVal _t4179[] = {gem_v_b, _t4178.fn(_t4178.env, _t4177, 2)};
        (void)(gem_buf_push_fn(NULL, _t4179, 2));
#line 1747 "compiler/main.gem"
    GemVal _t4180[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t4180, 2));
    } else {
#line 1749 "compiler/main.gem"
    GemVal _t4181[] = {gem_v_b, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4181, 2));
    }
#line 1751 "compiler/main.gem"
    GemVal _t4182[] = {gem_v_b};
    GemVal _t4183 = gem_buf_str_fn(NULL, _t4182, 1);
    gem_pop_frame();
    return _t4183;
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
#line 1757 "compiler/main.gem"
    GemVal gem_v__d81 = gem_v_node;
#line 1757 "compiler/main.gem"
    GemVal _t4185 = gem_v__d81;
    GemVal gem_v_params = gem_table_get(_t4185, gem_string("params"));
#line 1757 "compiler/main.gem"
    GemVal _t4186 = gem_v__d81;
    GemVal gem_v_body = gem_table_get(_t4186, gem_string("body"));
#line 1757 "compiler/main.gem"
    GemVal _t4187 = gem_v__d81;
    GemVal gem_v_name = gem_table_get(_t4187, gem_string("name"));

#line 1760 "compiler/main.gem"
    GemVal _t4188[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4188, 1);
#line 1761 "compiler/main.gem"
    GemVal _t4189 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4189, gem_string("rest_param")), GEM_NIL))) {
#line 1762 "compiler/main.gem"
    GemVal _t4190 = gem_v_node;
    GemVal _t4191[] = {gem_v_scope_vars, gem_table_get(_t4190, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4191, 2));
    }
#line 1764 "compiler/main.gem"
    GemVal _t4192[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4192, 2));
#line 1766 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1767 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1768 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1769 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1771 "compiler/main.gem"
    GemVal _t4193[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t4194 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t4194.fn(_t4194.env, _t4193, 2);
#line 1773 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1774 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1776 "compiler/main.gem"
    GemVal gem_v_use_tco = gem_bool(0);
#line 1777 "compiler/main.gem"
    GemVal _t4195 = gem_v_node;
    GemVal _t4197;
    if (!gem_truthy(gem_eq(gem_table_get(_t4195, gem_string("rest_param")), GEM_NIL))) {
        _t4197 = gem_eq(gem_table_get(_t4195, gem_string("rest_param")), GEM_NIL);
    } else {
        GemVal _t4196[] = {gem_v_scope_vars, gem_v_name};
        _t4197 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t4196, 2));
    }
    if (gem_truthy(_t4197)) {
#line 1778 "compiler/main.gem"
    GemVal _t4198[] = {gem_v_name, gem_v_body};
    GemVal _t4199 = (*gem_v_has_self_tail_call);
        gem_v_use_tco = _t4199.fn(_t4199.env, _t4198, 2);
    }
#line 1780 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1781 "compiler/main.gem"
        *gem_v_tco_fn_name = gem_v_name;
#line 1782 "compiler/main.gem"
        *gem_v_tco_params = gem_v_params;
#line 1783 "compiler/main.gem"
        *gem_v_tco_captured = gem_v_captured;
    }
#line 1786 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1787 "compiler/main.gem"
    GemVal _t4200[] = {gem_v_name};
    GemVal _t4201[] = {gem_v_b, gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4200, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4201, 2));
#line 1789 "compiler/main.gem"
    GemVal _t4202 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4202, gem_string("line")), GEM_NIL))) {
#line 1790 "compiler/main.gem"
    GemVal _t4203 = gem_v_node;
    GemVal _t4204[] = {gem_table_get(_t4203, gem_string("line"))};
    GemVal _t4205[] = {(*gem_v_source_name)};
    GemVal _t4206[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4205, 1)};
    GemVal _t4207[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_string("#line "), gem_to_string_fn(NULL, _t4204, 1)), gem_string(" \"")), gem_to_string_fn(NULL, _t4206, 1)), gem_string("\"\n"))};
        (void)(gem_buf_push_fn(NULL, _t4207, 2));
    }
#line 1793 "compiler/main.gem"
    GemVal _t4208[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t4209 = (*gem_v_emit_param_bindings);
    GemVal _t4210[] = {gem_v_b, _t4209.fn(_t4209.env, _t4208, 3)};
    (void)(gem_buf_push_fn(NULL, _t4210, 2));
#line 1795 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1796 "compiler/main.gem"
    GemVal _t4211 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4211, gem_string("line")), GEM_NIL))) {
#line 1797 "compiler/main.gem"
    GemVal _t4212 = gem_v_node;
        gem_v_fn_line = gem_table_get(_t4212, gem_string("line"));
    }
#line 1799 "compiler/main.gem"
    GemVal _t4213[] = {gem_v_name};
    GemVal _t4214[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4213, 1)};
    GemVal _t4215[] = {(*gem_v_source_name)};
    GemVal _t4216[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4215, 1)};
    GemVal _t4217[] = {gem_v_fn_line};
    GemVal _t4218[] = {gem_v_b, gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t4214, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t4216, 1)), gem_string("\", ")), gem_to_string_fn(NULL, _t4217, 1)), gem_string(");\n"))};
    (void)(gem_buf_push_fn(NULL, _t4218, 2));
#line 1801 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1802 "compiler/main.gem"
    GemVal _t4219[] = {gem_v_b, gem_string("    while(1) {\n")};
        (void)(gem_buf_push_fn(NULL, _t4219, 2));
#line 1803 "compiler/main.gem"
    GemVal _t4220[] = {gem_v_b, gem_string("        gem_yield_check();\n")};
        (void)(gem_buf_push_fn(NULL, _t4220, 2));
#line 1804 "compiler/main.gem"
    GemVal _t4221[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4221, 1), gem_int(0)))) {
#line 1805 "compiler/main.gem"
            GemVal gem_v__for_i_82 = gem_int(0);
#line 1805 "compiler/main.gem"
    GemVal _t4222[] = {gem_v_body};
            GemVal gem_v__for_limit_82 = gem_sub(gem_len_fn(NULL, _t4222, 1), gem_int(1));
#line 1805 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_v__for_limit_82))) break;
#line 1805 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_82;
#line 1805 "compiler/main.gem"
                gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1806 "compiler/main.gem"
    GemVal _t4223[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(2)};
    GemVal _t4224 = (*gem_v_compile_stmt);
    GemVal _t4225[] = {gem_v_b, _t4224.fn(_t4224.env, _t4223, 2)};
                (void)(gem_buf_push_fn(NULL, _t4225, 2));
#line 1807 "compiler/main.gem"
    GemVal _t4226[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t4226, 2));
            }

#line 1809 "compiler/main.gem"
    GemVal _t4227[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(2)};
    GemVal _t4228 = (*gem_v_compile_stmt_return);
    GemVal _t4229[] = {gem_v_b, _t4228.fn(_t4228.env, _t4227, 2)};
            (void)(gem_buf_push_fn(NULL, _t4229, 2));
#line 1810 "compiler/main.gem"
    GemVal _t4230[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4230, 2));
        } else {
#line 1812 "compiler/main.gem"
    GemVal _t4231[] = {gem_v_b, gem_string("        gem_pop_frame();\n        return GEM_NIL;\n")};
            (void)(gem_buf_push_fn(NULL, _t4231, 2));
        }
#line 1814 "compiler/main.gem"
    GemVal _t4232[] = {gem_v_b, gem_string("    }\n")};
        (void)(gem_buf_push_fn(NULL, _t4232, 2));
    } else {
#line 1816 "compiler/main.gem"
    GemVal _t4233[] = {gem_v_body};
    GemVal _t4234 = (*gem_v_emit_fn_body);
    GemVal _t4235[] = {gem_v_b, _t4234.fn(_t4234.env, _t4233, 1)};
        (void)(gem_buf_push_fn(NULL, _t4235, 2));
    }
#line 1819 "compiler/main.gem"
    GemVal _t4236[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4236, 2));
#line 1821 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1822 "compiler/main.gem"
        *gem_v_tco_fn_name = GEM_NIL;
#line 1823 "compiler/main.gem"
        *gem_v_tco_params = GEM_NIL;
#line 1824 "compiler/main.gem"
        *gem_v_tco_captured = GEM_NIL;
    }
#line 1827 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1828 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1829 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1830 "compiler/main.gem"
    GemVal _t4237[] = {gem_v_b};
    GemVal _t4238 = gem_buf_str_fn(NULL, _t4237, 1);
    gem_pop_frame();
    return _t4238;
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
#line 1836 "compiler/main.gem"
    GemVal gem_v__d83 = gem_v_node;
#line 1836 "compiler/main.gem"
    GemVal _t4240 = gem_v__d83;
    GemVal gem_v_params = gem_table_get(_t4240, gem_string("params"));
#line 1836 "compiler/main.gem"
    GemVal _t4241 = gem_v__d83;
    GemVal gem_v_body = gem_table_get(_t4241, gem_string("body"));

#line 1837 "compiler/main.gem"
    GemVal _t4242 = (*gem_v_anon_name);
    GemVal gem_v_name = _t4242.fn(_t4242.env, NULL, 0);
#line 1840 "compiler/main.gem"
    GemVal _t4243[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4243, 1);
#line 1841 "compiler/main.gem"
    GemVal _t4244 = gem_v_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4244, gem_string("rest_param")), GEM_NIL))) {
#line 1842 "compiler/main.gem"
    GemVal _t4245 = gem_v_node;
    GemVal _t4246[] = {gem_v_inner_scope, gem_table_get(_t4245, gem_string("rest_param"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4246, 2));
    }
#line 1844 "compiler/main.gem"
    GemVal _t4247[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4247, 2));
#line 1846 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1847 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1848 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1849 "compiler/main.gem"
    GemVal _t4248[] = {gem_v_captures};
    GemVal _t4249[] = {gem_v_inner_scope, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4248, 1)};
    *gem_v_local_names = gem_fn__mod_codegen_set_union(NULL, _t4249, 2);
#line 1851 "compiler/main.gem"
    GemVal _t4250[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t4251 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t4251.fn(_t4251.env, _t4250, 2);
#line 1853 "compiler/main.gem"
    GemVal _t4252[] = {gem_v_captures};
    GemVal _t4253[] = {gem_v_inner_captured, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4252, 1)};
    *gem_v_boxed_vars = gem_fn__mod_codegen_set_union(NULL, _t4253, 2);
#line 1854 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1857 "compiler/main.gem"
    GemVal _t4254[] = {gem_v_name};
    GemVal gem_v_struct_name = gem_add(gem_string("_closure_"), gem_to_string_fn(NULL, _t4254, 1));
#line 1858 "compiler/main.gem"
    GemVal gem_v_sb = gem_buf_new_fn(NULL, NULL, 0);
#line 1859 "compiler/main.gem"
    GemVal _t4255[] = {gem_v_struct_name};
    GemVal _t4256[] = {gem_v_sb, gem_add(gem_add(gem_string("struct "), gem_to_string_fn(NULL, _t4255, 1)), gem_string(" {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4256, 2));
#line 1860 "compiler/main.gem"
    GemVal gem_v__for_items_84 = gem_v_captures;
#line 1860 "compiler/main.gem"
    GemVal gem_v__for_i_84 = gem_int(0);
#line 1860 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4257[] = {gem_v__for_items_84};
        if (!gem_truthy(gem_lt(gem_v__for_i_84, gem_len_fn(NULL, _t4257, 1)))) break;
#line 1860 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_84, gem_v__for_i_84);
#line 1860 "compiler/main.gem"
        gem_v__for_i_84 = gem_add(gem_v__for_i_84, gem_int(1));
#line 1861 "compiler/main.gem"
    GemVal _t4258[] = {gem_v_cap};
    GemVal _t4259 = (*gem_v_mangle);
    GemVal _t4260[] = {_t4259.fn(_t4259.env, _t4258, 1)};
    GemVal _t4261[] = {gem_v_sb, gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4260, 1)), gem_string(";\n"))};
        (void)(gem_buf_push_fn(NULL, _t4261, 2));
    }

#line 1863 "compiler/main.gem"
    GemVal _t4262[] = {gem_v_sb, gem_string("};\n")};
    (void)(gem_buf_push_fn(NULL, _t4262, 2));
#line 1864 "compiler/main.gem"
    GemVal _t4263[] = {gem_v_sb};
    GemVal gem_v_struct_def = gem_buf_str_fn(NULL, _t4263, 1);
#line 1867 "compiler/main.gem"
    GemVal gem_v_fb = gem_buf_new_fn(NULL, NULL, 0);
#line 1868 "compiler/main.gem"
    GemVal _t4264[] = {gem_v_name};
    GemVal _t4265[] = {gem_v_fb, gem_add(gem_add(gem_string("static GemVal "), gem_to_string_fn(NULL, _t4264, 1)), gem_string("(void *_env, GemVal *args, int argc) {\n"))};
    (void)(gem_buf_push_fn(NULL, _t4265, 2));
#line 1869 "compiler/main.gem"
    GemVal _t4266[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4266, 1), gem_int(0)))) {
#line 1870 "compiler/main.gem"
    GemVal _t4267[] = {gem_v_struct_name};
    GemVal _t4268[] = {gem_v_struct_name};
    GemVal _t4269[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    struct "), gem_to_string_fn(NULL, _t4267, 1)), gem_string(" *_cls = (struct ")), gem_to_string_fn(NULL, _t4268, 1)), gem_string(" *)_env;\n"))};
        (void)(gem_buf_push_fn(NULL, _t4269, 2));
#line 1871 "compiler/main.gem"
        GemVal gem_v__for_items_85 = gem_v_captures;
#line 1871 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1871 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4270[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t4270, 1)))) break;
#line 1871 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1871 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1872 "compiler/main.gem"
    GemVal _t4271[] = {gem_v_cap};
    GemVal _t4272 = (*gem_v_mangle);
            GemVal gem_v_mc = _t4272.fn(_t4272.env, _t4271, 1);
#line 1873 "compiler/main.gem"
    GemVal _t4273[] = {gem_v_mc};
    GemVal _t4274[] = {gem_v_mc};
    GemVal _t4275[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    GemVal *"), gem_to_string_fn(NULL, _t4273, 1)), gem_string(" = _cls->")), gem_to_string_fn(NULL, _t4274, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4275, 2));
        }

    }
#line 1877 "compiler/main.gem"
    GemVal _t4276[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t4277 = (*gem_v_emit_param_bindings);
    GemVal _t4278[] = {gem_v_fb, _t4277.fn(_t4277.env, _t4276, 3)};
    (void)(gem_buf_push_fn(NULL, _t4278, 2));
#line 1878 "compiler/main.gem"
    GemVal _t4279[] = {gem_v_name};
    GemVal _t4280[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4279, 1)};
    GemVal _t4281[] = {(*gem_v_source_name)};
    GemVal _t4282[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4281, 1)};
    GemVal _t4283[] = {gem_v_fb, gem_add(gem_add(gem_add(gem_add(gem_string("    gem_push_frame(\""), gem_to_string_fn(NULL, _t4280, 1)), gem_string("\", \"")), gem_to_string_fn(NULL, _t4282, 1)), gem_string("\", 0);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4283, 2));
#line 1879 "compiler/main.gem"
    GemVal _t4284[] = {gem_v_body};
    GemVal _t4285 = (*gem_v_emit_fn_body);
    GemVal _t4286[] = {gem_v_fb, _t4285.fn(_t4285.env, _t4284, 1)};
    (void)(gem_buf_push_fn(NULL, _t4286, 2));
#line 1880 "compiler/main.gem"
    GemVal _t4287[] = {gem_v_fb, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4287, 2));
#line 1882 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1883 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1884 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1887 "compiler/main.gem"
    GemVal _t4288[] = {gem_v_fb};
    GemVal _t4289[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_buf_str_fn(NULL, _t4288, 1))};
    (void)(gem_push_fn(NULL, _t4289, 2));
    GemVal _t4290 = gem_table_new();
    gem_table_set(_t4290, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t4290, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t4290, gem_string("captures"), gem_v_captures);
    GemVal _t4291 = _t4290;
    gem_pop_frame();
    return _t4291;
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
#line 1896 "compiler/main.gem"
    GemVal _t4293 = gem_table_new();
    GemVal gem_v_fn_defs = _t4293;
#line 1897 "compiler/main.gem"
    GemVal _t4294 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t4294;
#line 1898 "compiler/main.gem"
    GemVal _t4295 = gem_table_new();
    GemVal gem_v_extern_includes = _t4295;
#line 1899 "compiler/main.gem"
    GemVal _t4296 = gem_table_new();
    GemVal gem_v_top_stmts = _t4296;
#line 1901 "compiler/main.gem"
    GemVal _t4297 = gem_v_ast;
    GemVal gem_v__for_items_86 = gem_table_get(_t4297, gem_string("stmts"));
#line 1901 "compiler/main.gem"
    GemVal gem_v__for_i_86 = gem_int(0);
#line 1901 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4298[] = {gem_v__for_items_86};
        if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t4298, 1)))) break;
#line 1901 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1901 "compiler/main.gem"
        gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1902 "compiler/main.gem"
    GemVal _t4299 = gem_v_s;
        if (gem_truthy(gem_eq(gem_table_get(_t4299, gem_string("tag")), gem_string("fn_def")))) {
#line 1903 "compiler/main.gem"
    GemVal _t4300[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t4300, 2));
        } else {
#line 1904 "compiler/main.gem"
    GemVal _t4301 = gem_v_s;
            if (gem_truthy(gem_eq(gem_table_get(_t4301, gem_string("tag")), gem_string("extern_fn")))) {
#line 1905 "compiler/main.gem"
    GemVal _t4302[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t4302, 2));
            } else {
#line 1906 "compiler/main.gem"
    GemVal _t4303 = gem_v_s;
                if (gem_truthy(gem_eq(gem_table_get(_t4303, gem_string("tag")), gem_string("extern_include")))) {
#line 1907 "compiler/main.gem"
    GemVal _t4304[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t4304, 2));
                } else {
#line 1908 "compiler/main.gem"
    GemVal _t4305 = gem_v_s;
                    if (gem_truthy(gem_eq(gem_table_get(_t4305, gem_string("tag")), gem_string("export")))) {
                    } else {
#line 1911 "compiler/main.gem"
    GemVal _t4306[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t4306, 2));
                    }
                }
            }
        }
    }

#line 1916 "compiler/main.gem"
    GemVal gem_v__for_items_87 = gem_v_extern_fns_list;
#line 1916 "compiler/main.gem"
    GemVal gem_v__for_i_87 = gem_int(0);
#line 1916 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4307[] = {gem_v__for_items_87};
        if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_len_fn(NULL, _t4307, 1)))) break;
#line 1916 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_87, gem_v__for_i_87);
#line 1916 "compiler/main.gem"
        gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1917 "compiler/main.gem"
    GemVal _t4308 = gem_v_ef;
    GemVal _t4309[] = {(*gem_v_defined_fns), gem_table_get(_t4308, gem_string("name"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4309, 2));
    }

#line 1921 "compiler/main.gem"
    GemVal gem_v__for_items_88 = gem_v_fn_defs;
#line 1921 "compiler/main.gem"
    GemVal gem_v__for_i_88 = gem_int(0);
#line 1921 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4310[] = {gem_v__for_items_88};
        if (!gem_truthy(gem_lt(gem_v__for_i_88, gem_len_fn(NULL, _t4310, 1)))) break;
#line 1921 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_88, gem_v__for_i_88);
#line 1921 "compiler/main.gem"
        gem_v__for_i_88 = gem_add(gem_v__for_i_88, gem_int(1));
#line 1922 "compiler/main.gem"
    GemVal _t4311 = gem_v_fd;
    GemVal _t4312[] = {(*gem_v_defined_fns), gem_table_get(_t4311, gem_string("name"))};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4312, 2));
#line 1923 "compiler/main.gem"
    GemVal _t4313 = gem_v_fd;
    GemVal _t4314[] = {gem_table_get(_t4313, gem_string("name"))};
        GemVal gem_v_decl = gem_add(gem_add(gem_string("static GemVal gem_fn_"), gem_to_string_fn(NULL, _t4314, 1)), gem_string("(void *_env, GemVal *args, int argc);"));
#line 1924 "compiler/main.gem"
    GemVal _t4315[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t4315, 2));
    }

#line 1928 "compiler/main.gem"
    GemVal _t4316[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4316, 2));
#line 1929 "compiler/main.gem"
    GemVal _t4317 = gem_table_new();
    GemVal _t4318[] = {gem_v_top_stmts, _t4317};
    GemVal _t4319 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t4319.fn(_t4319.env, _t4318, 2);
#line 1932 "compiler/main.gem"
    GemVal gem_v__for_items_89 = gem_v_extern_fns_list;
#line 1932 "compiler/main.gem"
    GemVal gem_v__for_i_89 = gem_int(0);
#line 1932 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4320[] = {gem_v__for_items_89};
        if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t4320, 1)))) break;
#line 1932 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1932 "compiler/main.gem"
        gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1933 "compiler/main.gem"
    GemVal _t4321[] = {gem_v_ef};
    GemVal _t4322 = (*gem_v_compile_extern_fn);
    GemVal _t4323[] = {(*gem_v_functions), _t4322.fn(_t4322.env, _t4321, 1)};
        (void)(gem_push_fn(NULL, _t4323, 2));
    }

#line 1937 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_fn_defs;
#line 1937 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1937 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4324[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t4324, 1)))) break;
#line 1937 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1937 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1938 "compiler/main.gem"
    GemVal _t4325[] = {gem_v_fd};
    GemVal _t4326 = (*gem_v_compile_fn);
    GemVal _t4327[] = {(*gem_v_functions), _t4326.fn(_t4326.env, _t4325, 1)};
        (void)(gem_push_fn(NULL, _t4327, 2));
    }

#line 1942 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1943 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1944 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 1945 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 1946 "compiler/main.gem"
    GemVal _t4328 = gem_table_new();
    *gem_v_local_names = _t4328;
#line 1947 "compiler/main.gem"
    GemVal gem_v__for_tbl_91 = (*gem_v_top_level_vars);
#line 1947 "compiler/main.gem"
    GemVal _t4329[] = {gem_v__for_tbl_91};
    GemVal gem_v__for_len_91 = gem_len_fn(NULL, _t4329, 1);
#line 1947 "compiler/main.gem"
    GemVal gem_v__for_i_91 = gem_int(0);
#line 1947 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_v__for_len_91))) break;
#line 1947 "compiler/main.gem"
    GemVal _t4330[] = {gem_v__for_tbl_91, gem_v__for_i_91};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4330, 2);
#line 1947 "compiler/main.gem"
    GemVal _t4331[] = {gem_v__for_tbl_91, gem_v__for_i_91};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4331, 2);
#line 1947 "compiler/main.gem"
        gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1948 "compiler/main.gem"
    GemVal _t4332[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4332, 2));
    }

#line 1950 "compiler/main.gem"
    GemVal _t4333[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t4334 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t4334.fn(_t4334.env, _t4333, 2);
#line 1951 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1952 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1953 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1955 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 1956 "compiler/main.gem"
    GemVal gem_v__for_items_92 = gem_v_fn_defs;
#line 1956 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 1956 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4335[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t4335, 1)))) break;
#line 1956 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 1956 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 1957 "compiler/main.gem"
    GemVal _t4336 = gem_v_fd;
        if (gem_truthy(gem_eq(gem_table_get(_t4336, gem_string("name")), gem_string("main")))) {
#line 1958 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 1962 "compiler/main.gem"
    GemVal gem_v_mb = gem_buf_new_fn(NULL, NULL, 0);
#line 1963 "compiler/main.gem"
    GemVal _t4337[] = {gem_v_mb, gem_string("int main(int argc, char **argv) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4337, 2));
#line 1964 "compiler/main.gem"
    GemVal _t4338[] = {gem_v_mb, gem_string("    GC_INIT();\n")};
    (void)(gem_buf_push_fn(NULL, _t4338, 2));
#line 1965 "compiler/main.gem"
    GemVal _t4339[] = {gem_v_mb, gem_string("    gem_init(argc, argv);\n")};
    (void)(gem_buf_push_fn(NULL, _t4339, 2));
#line 1966 "compiler/main.gem"
    GemVal _t4340[] = {gem_v_mb, gem_string("    gem_scheduler_init();\n")};
    (void)(gem_buf_push_fn(NULL, _t4340, 2));
#line 1967 "compiler/main.gem"
    GemVal _t4341[] = {(*gem_v_source_name)};
    GemVal _t4342[] = {gem_fn__mod_codegen_escape_c_string(NULL, _t4341, 1)};
    GemVal _t4343[] = {gem_v_mb, gem_add(gem_add(gem_string("    gem_push_frame(\"main\", \""), gem_to_string_fn(NULL, _t4342, 1)), gem_string("\", 0);\n"))};
    (void)(gem_buf_push_fn(NULL, _t4343, 2));
#line 1968 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 1969 "compiler/main.gem"
    GemVal _t4344[] = {gem_v_mb, gem_v_main_body};
        (void)(gem_buf_push_fn(NULL, _t4344, 2));
    }
#line 1971 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 1972 "compiler/main.gem"
    GemVal _t4345[] = {gem_v_mb, gem_string("    GemVal _margs[1] = {GEM_NIL};\n")};
        (void)(gem_buf_push_fn(NULL, _t4345, 2));
#line 1973 "compiler/main.gem"
    GemVal _t4346[] = {gem_v_mb, gem_string("    gem_fn_main(NULL, _margs, 0);\n")};
        (void)(gem_buf_push_fn(NULL, _t4346, 2));
    }
#line 1975 "compiler/main.gem"
    GemVal _t4347[] = {gem_v_mb, gem_string("    gem_run_scheduler();\n")};
    (void)(gem_buf_push_fn(NULL, _t4347, 2));
#line 1976 "compiler/main.gem"
    GemVal _t4348[] = {gem_v_mb, gem_string("    gem_pop_frame();\n    return 0;\n}\n")};
    (void)(gem_buf_push_fn(NULL, _t4348, 2));
#line 1977 "compiler/main.gem"
    GemVal _t4349[] = {gem_v_mb};
    GemVal gem_v_main_c = gem_buf_str_fn(NULL, _t4349, 1);
#line 1980 "compiler/main.gem"
    GemVal gem_v_ob = gem_buf_new_fn(NULL, NULL, 0);
#line 1981 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_includes;
#line 1981 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 1981 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4350[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t4350, 1)))) break;
#line 1981 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 1981 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 1982 "compiler/main.gem"
    GemVal _t4351 = gem_v_ei;
    GemVal _t4352[] = {gem_table_get(_t4351, gem_string("path"))};
    GemVal _t4353[] = {gem_v_ob, gem_add(gem_add(gem_string("#include \""), gem_to_string_fn(NULL, _t4352, 1)), gem_string("\"\n"))};
        (void)(gem_buf_push_fn(NULL, _t4353, 2));
    }

#line 1984 "compiler/main.gem"
    GemVal _t4354[] = {gem_v_ob, gem_string("#include \"gem.h\"\n")};
    (void)(gem_buf_push_fn(NULL, _t4354, 2));
#line 1988 "compiler/main.gem"
    GemVal _t4355[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t4355, 1), gem_int(0)))) {
#line 1989 "compiler/main.gem"
        GemVal gem_v__for_i_94 = gem_int(0);
#line 1989 "compiler/main.gem"
    GemVal _t4356[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_94 = gem_len_fn(NULL, _t4356, 1);
#line 1989 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_v__for_limit_94))) break;
#line 1989 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_94;
#line 1989 "compiler/main.gem"
            gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 1990 "compiler/main.gem"
    GemVal _t4357[] = {gem_v_ob, gem_table_get((*gem_v_extern_c_decls), gem_v_i)};
            (void)(gem_buf_push_fn(NULL, _t4357, 2));
#line 1991 "compiler/main.gem"
    GemVal _t4358[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4358, 2));
        }

    }
#line 1996 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 1996 "compiler/main.gem"
    GemVal _t4359[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_95 = gem_len_fn(NULL, _t4359, 1);
#line 1996 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_v__for_limit_95))) break;
#line 1996 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_95;
#line 1996 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 1997 "compiler/main.gem"
    GemVal _t4360[] = {gem_v_ob, gem_table_get((*gem_v_forward_decls), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4360, 2));
#line 1998 "compiler/main.gem"
    GemVal _t4361[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4361, 1), gem_int(1))))) {
#line 1999 "compiler/main.gem"
    GemVal _t4362[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4362, 2));
        }
    }

#line 2002 "compiler/main.gem"
    GemVal _t4363[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4363, 2));
#line 2005 "compiler/main.gem"
    GemVal _t4364[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t4364, 1);
#line 2006 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 2006 "compiler/main.gem"
    GemVal _t4365[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_96 = gem_len_fn(NULL, _t4365, 1);
#line 2006 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_v__for_limit_96))) break;
#line 2006 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_96;
#line 2006 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 2007 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 2008 "compiler/main.gem"
    GemVal _t4366[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4366, 2))) {
#line 2009 "compiler/main.gem"
    GemVal _t4367[] = {gem_v_vname};
    GemVal _t4368[] = {gem_v_ob, gem_add(gem_add(gem_string("GemVal *gem_v_"), gem_to_string_fn(NULL, _t4367, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4368, 2));
        } else {
#line 2011 "compiler/main.gem"
    GemVal _t4369[] = {gem_v_vname};
    GemVal _t4370[] = {gem_v_ob, gem_add(gem_add(gem_string("GemVal gem_v_"), gem_to_string_fn(NULL, _t4369, 1)), gem_string(";\n"))};
            (void)(gem_buf_push_fn(NULL, _t4370, 2));
        }
    }

#line 2014 "compiler/main.gem"
    GemVal _t4371[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4371, 2));
#line 2017 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 2017 "compiler/main.gem"
    GemVal _t4372[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_97 = gem_len_fn(NULL, _t4372, 1);
#line 2017 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_limit_97))) break;
#line 2017 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_97;
#line 2017 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 2018 "compiler/main.gem"
    GemVal _t4373[] = {gem_v_ob, gem_table_get((*gem_v_functions), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4373, 2));
#line 2019 "compiler/main.gem"
    GemVal _t4374[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4374, 1), gem_int(1))))) {
#line 2020 "compiler/main.gem"
    GemVal _t4375[] = {gem_v_ob, gem_string("\n\n")};
            (void)(gem_buf_push_fn(NULL, _t4375, 2));
        }
    }

#line 2023 "compiler/main.gem"
    GemVal _t4376[] = {gem_v_ob, gem_string("\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4376, 2));
#line 2025 "compiler/main.gem"
    GemVal _t4377[] = {gem_v_ob, gem_v_main_c};
    (void)(gem_buf_push_fn(NULL, _t4377, 2));
#line 2026 "compiler/main.gem"
    GemVal _t4378[] = {gem_v_ob};
    GemVal _t4379 = gem_buf_str_fn(NULL, _t4378, 1);
    gem_pop_frame();
    return _t4379;
}

static GemVal gem_fn__mod_codegen_make_codegen(void *_env, GemVal *args, int argc) {
#line 125 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_make_codegen", "compiler/main.gem", 125);
#line 126 "compiler/main.gem"
    GemVal _t1825 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1825;
#line 127 "compiler/main.gem"
    GemVal _t1826 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1826;
#line 128 "compiler/main.gem"
    GemVal _t1827 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1827;
#line 129 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 130 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t1828 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1828;
#line 132 "compiler/main.gem"
    GemVal _t1829 = gem_table_new();
    gem_table_set(_t1829, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1829, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1829, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1829, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1829, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1829, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1829, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1829, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1829, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1829, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1829, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1829, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1829, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1829, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1829, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1829, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1829, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1829, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1829, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1829, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1829, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1829, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1829, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1829, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1829, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1829, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1829, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1829, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1829, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1829, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1829, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1829, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1829, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1829, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1829, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1829, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1829, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1829, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1829, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1829, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1829, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1829, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1829, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1829, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1829, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1829, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1829, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1829, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1829, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1829, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1829, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1829, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1829, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1829, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1829, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1829, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1829, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1829, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1829, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1829, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1829, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1829, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1829, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1829, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1829, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1829, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1829, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1829, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1829, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1829, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1829, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    gem_table_set(_t1829, gem_string("exec"), gem_string("gem_exec_fn"));
    gem_table_set(_t1829, gem_string("tcp_connect"), gem_string("gem_tcp_connect_fn"));
    gem_table_set(_t1829, gem_string("tcp_listen"), gem_string("gem_tcp_listen_fn"));
    gem_table_set(_t1829, gem_string("tcp_accept"), gem_string("gem_tcp_accept_fn"));
    gem_table_set(_t1829, gem_string("tcp_read"), gem_string("gem_tcp_read_fn"));
    gem_table_set(_t1829, gem_string("tcp_write"), gem_string("gem_tcp_write_fn"));
    gem_table_set(_t1829, gem_string("tcp_close"), gem_string("gem_tcp_close_fn"));
    gem_table_set(_t1829, gem_string("__table_key_at"), gem_string("gem_table_key_at_fn"));
    gem_table_set(_t1829, gem_string("__table_val_at"), gem_string("gem_table_val_at_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1829;
#line 214 "compiler/main.gem"
    GemVal _t1830 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1830;
#line 215 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 215 "compiler/main.gem"
    GemVal _t1831[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_len_10 = gem_len_fn(NULL, _t1831, 1);
#line 215 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 215 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_len_10))) break;
#line 215 "compiler/main.gem"
    GemVal _t1832[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1832, 2);
#line 215 "compiler/main.gem"
    GemVal _t1833[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1833, 2);
#line 215 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 216 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 218 "compiler/main.gem"
    GemVal _t1834 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1834;
#line 219 "compiler/main.gem"
    GemVal _t1835 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1835;
#line 220 "compiler/main.gem"
    GemVal _t1836 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1836;
#line 221 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 222 "compiler/main.gem"
    GemVal _t1837 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1837;
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
    struct _closure__anon_24 *_t1840 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1840->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_24, _t1840);
#line 234 "compiler/main.gem"
    struct _closure__anon_25 *_t1843 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t1843->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_25, _t1843);
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
    struct _closure__anon_27 *_t2155 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t2155->gem_v_builtins = gem_v_builtins;
    _t2155->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2155->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2155->gem_v_defined_fns = gem_v_defined_fns;
    _t2155->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_27, _t2155);
#line 439 "compiler/main.gem"
    struct _closure__anon_28 *_t2167 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t2167->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_28, _t2167);
#line 454 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 455 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 457 "compiler/main.gem"
    struct _closure__anon_29 *_t2355 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t2355->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2355->gem_v_walk_captures = gem_v_walk_captures;
    _t2355->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_29, _t2355);
#line 566 "compiler/main.gem"
    struct _closure__anon_30 *_t2365 = GC_MALLOC(sizeof(struct _closure__anon_30));
    _t2365->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_30, _t2365);
#line 577 "compiler/main.gem"
    struct _closure__anon_31 *_t2370 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2370->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_31, _t2370);
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
    struct _closure__anon_32 *_t2455 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2455->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t2455->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_is_self_tail_call = gem_make_fn(_anon_32, _t2455);
#line 658 "compiler/main.gem"
    struct _closure__anon_33 *_t2461 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2461->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_has_self_tail_call = gem_make_fn(_anon_33, _t2461);
#line 665 "compiler/main.gem"
    struct _closure__anon_34 *_t2494 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2494->gem_v_compile_expr = gem_v_compile_expr;
    _t2494->gem_v_mangle = gem_v_mangle;
    _t2494->gem_v_tco_captured = gem_v_tco_captured;
    _t2494->gem_v_tco_params = gem_v_tco_params;
    _t2494->gem_v_tmp = gem_v_tmp;
    *gem_v_emit_tco_continue = gem_make_fn(_anon_34, _t2494);
#line 695 "compiler/main.gem"
    struct _closure__anon_35 *_t2694 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2694->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2694->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2694->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2694->gem_v_compile_array = gem_v_compile_array;
    _t2694->gem_v_compile_binop = gem_v_compile_binop;
    _t2694->gem_v_compile_call = gem_v_compile_call;
    _t2694->gem_v_compile_expr = gem_v_compile_expr;
    _t2694->gem_v_compile_table = gem_v_compile_table;
    _t2694->gem_v_defined_fns = gem_v_defined_fns;
    _t2694->gem_v_local_names = gem_v_local_names;
    _t2694->gem_v_mangle = gem_v_mangle;
    _t2694->gem_v_tmp = gem_v_tmp;
    _t2694->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_35, _t2694);
#line 782 "compiler/main.gem"
    struct _closure__anon_36 *_t2715 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2715->gem_v_compile_expr = gem_v_compile_expr;
    _t2715->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_36, _t2715);
#line 798 "compiler/main.gem"
    struct _closure__anon_37 *_t2733 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2733->gem_v_compile_expr = gem_v_compile_expr;
    _t2733->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_37, _t2733);
#line 813 "compiler/main.gem"
    struct _closure__anon_38 *_t2779 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2779->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2779->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2779->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2779->gem_v_mangle = gem_v_mangle;
    _t2779->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_38, _t2779);
#line 845 "compiler/main.gem"
    struct _closure__anon_39 *_t2798 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2798->gem_v_compile_expr = gem_v_compile_expr;
    _t2798->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_39, _t2798);
#line 866 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_40, NULL);
#line 875 "compiler/main.gem"
    struct _closure__anon_41 *_t2890 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2890->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2890->gem_v_compile_args = gem_v_compile_args;
    _t2890->gem_v_compile_expr = gem_v_compile_expr;
    _t2890->gem_v_defined_fns = gem_v_defined_fns;
    _t2890->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2890->gem_v_local_names = gem_v_local_names;
    _t2890->gem_v_source_name = gem_v_source_name;
    _t2890->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_41, _t2890);
#line 919 "compiler/main.gem"
    struct _closure__anon_42 *_t3061 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t3061->gem_v_compile_expr = gem_v_compile_expr;
    _t3061->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_42, _t3061);
#line 1000 "compiler/main.gem"
    struct _closure__anon_43 *_t3069 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t3069->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_43, _t3069);
#line 1009 "compiler/main.gem"
    struct _closure__anon_44 *_t3089 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t3089->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3089->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3089->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_stmts_tail = gem_make_fn(_anon_44, _t3089);
#line 1030 "compiler/main.gem"
    struct _closure__anon_45 *_t3343 = GC_MALLOC(sizeof(struct _closure__anon_45));
    _t3343->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3343->gem_v_compile_expr = gem_v_compile_expr;
    _t3343->gem_v_compile_if = gem_v_compile_if;
    _t3343->gem_v_compile_match = gem_v_compile_match;
    _t3343->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3343->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3343->gem_v_compile_while = gem_v_compile_while;
    _t3343->gem_v_in_top_level = gem_v_in_top_level;
    _t3343->gem_v_mangle = gem_v_mangle;
    _t3343->gem_v_source_name = gem_v_source_name;
    _t3343->gem_v_tmp = gem_v_tmp;
    _t3343->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3343->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_45, _t3343);
#line 1107 "compiler/main.gem"
    struct _closure__anon_46 *_t3606 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t3606->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3606->gem_v_compile_expr = gem_v_compile_expr;
    _t3606->gem_v_compile_if = gem_v_compile_if;
    _t3606->gem_v_compile_match = gem_v_compile_match;
    _t3606->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3606->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3606->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3606->gem_v_compile_while = gem_v_compile_while;
    _t3606->gem_v_emit_tco_continue = gem_v_emit_tco_continue;
    _t3606->gem_v_local_names = gem_v_local_names;
    _t3606->gem_v_mangle = gem_v_mangle;
    _t3606->gem_v_source_name = gem_v_source_name;
    _t3606->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3606->gem_v_tmp = gem_v_tmp;
    _t3606->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_46, _t3606);
#line 1186 "compiler/main.gem"
    struct _closure__anon_47 *_t3638 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3638->gem_v_compile_expr = gem_v_compile_expr;
    _t3638->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    *gem_v_compile_if = gem_make_fn(_anon_47, _t3638);
#line 1206 "compiler/main.gem"
    struct _closure__anon_48 *_t3664 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3664->gem_v_compile_expr = gem_v_compile_expr;
    _t3664->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_48, _t3664);
#line 1223 "compiler/main.gem"
    struct _closure__anon_49 *_t3748 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3748->gem_v_compile_expr = gem_v_compile_expr;
    _t3748->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3748->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3748->gem_v_mangle = gem_v_mangle;
    _t3748->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_49, _t3748);
#line 1279 "compiler/main.gem"
    struct _closure__anon_50 *_t3925 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3925->gem_v_compile_expr = gem_v_compile_expr;
    _t3925->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3925->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3925->gem_v_mangle = gem_v_mangle;
    _t3925->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_50, _t3925);
#line 1399 "compiler/main.gem"
    struct _closure__anon_51 *_t3933 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3933->gem_v_extern_c_decls = gem_v_extern_c_decls;
    GemVal *gem_v_emit_extern_c_decl = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_extern_c_decl = gem_make_fn(_anon_51, _t3933);
#line 1445 "compiler/main.gem"
    struct _closure__anon_52 *_t4007 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t4007->gem_v_compile_blocking_extern_fn = gem_v_compile_blocking_extern_fn;
    _t4007->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t4007->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_52, _t4007);
#line 1528 "compiler/main.gem"
    struct _closure__anon_53 *_t4132 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t4132->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t4132->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_blocking_extern_fn = gem_make_fn(_anon_53, _t4132);
#line 1708 "compiler/main.gem"
    struct _closure__anon_54 *_t4170 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t4170->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_54, _t4170);
#line 1739 "compiler/main.gem"
    struct _closure__anon_55 *_t4184 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t4184->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4184->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_55, _t4184);
#line 1756 "compiler/main.gem"
    struct _closure__anon_56 *_t4239 = GC_MALLOC(sizeof(struct _closure__anon_56));
    _t4239->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4239->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4239->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t4239->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4239->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4239->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4239->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t4239->gem_v_in_top_level = gem_v_in_top_level;
    _t4239->gem_v_local_names = gem_v_local_names;
    _t4239->gem_v_source_name = gem_v_source_name;
    _t4239->gem_v_tco_captured = gem_v_tco_captured;
    _t4239->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t4239->gem_v_tco_params = gem_v_tco_params;
    *gem_v_compile_fn = gem_make_fn(_anon_56, _t4239);
#line 1835 "compiler/main.gem"
    struct _closure__anon_57 *_t4292 = GC_MALLOC(sizeof(struct _closure__anon_57));
    _t4292->gem_v_anon_name = gem_v_anon_name;
    _t4292->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4292->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4292->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4292->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4292->gem_v_functions = gem_v_functions;
    _t4292->gem_v_in_top_level = gem_v_in_top_level;
    _t4292->gem_v_local_names = gem_v_local_names;
    _t4292->gem_v_mangle = gem_v_mangle;
    _t4292->gem_v_source_name = gem_v_source_name;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_57, _t4292);
#line 1894 "compiler/main.gem"
    struct _closure__anon_58 *_t4380 = GC_MALLOC(sizeof(struct _closure__anon_58));
    _t4380->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4380->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t4380->gem_v_compile_fn = gem_v_compile_fn;
    _t4380->gem_v_compile_stmts = gem_v_compile_stmts;
    _t4380->gem_v_defined_fns = gem_v_defined_fns;
    _t4380->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t4380->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4380->gem_v_forward_decls = gem_v_forward_decls;
    _t4380->gem_v_functions = gem_v_functions;
    _t4380->gem_v_in_top_level = gem_v_in_top_level;
    _t4380->gem_v_local_names = gem_v_local_names;
    _t4380->gem_v_source_name = gem_v_source_name;
    _t4380->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t4380->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_58, _t4380);
    GemVal _t4381 = gem_table_new();
    gem_table_set(_t4381, gem_string("compile"), gem_v_compile);
    GemVal _t4382 = _t4381;
    gem_pop_frame();
    return _t4382;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 20 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 20);
    GemVal _t4383[] = {gem_v_node};
    GemVal _t4385;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4383, 1), gem_string("table")))) {
        _t4385 = gem_eq(gem_type_fn(NULL, _t4383, 1), gem_string("table"));
    } else {
        GemVal _t4384 = gem_v_node;
        _t4385 = gem_eq(gem_table_get(_t4384, gem_string("tag")), gem_v_tag);
    }
    GemVal _t4386 = _t4385;
    gem_pop_frame();
    return _t4386;
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_program", "compiler/main.gem", 24);
    GemVal _t4387 = gem_table_new();
    gem_table_set(_t4387, gem_string("tag"), gem_string("program"));
    gem_table_set(_t4387, gem_string("stmts"), gem_v_stmts);
    GemVal _t4388 = _t4387;
    gem_pop_frame();
    return _t4388;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 28);
    GemVal _t4389 = gem_table_new();
    gem_table_set(_t4389, gem_string("tag"), gem_string("let"));
    gem_table_set(_t4389, gem_string("name"), gem_v_name);
    gem_table_set(_t4389, gem_string("value"), gem_v_value);
    gem_table_set(_t4389, gem_string("line"), gem_v_line);
    GemVal _t4390 = _t4389;
    gem_pop_frame();
    return _t4390;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 32);
    GemVal _t4391 = gem_table_new();
    gem_table_set(_t4391, gem_string("tag"), gem_string("var"));
    gem_table_set(_t4391, gem_string("name"), gem_v_name);
    GemVal _t4392 = _t4391;
    gem_pop_frame();
    return _t4392;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 36);
    GemVal _t4393 = gem_table_new();
    gem_table_set(_t4393, gem_string("tag"), gem_string("table"));
    gem_table_set(_t4393, gem_string("entries"), gem_v_entries);
    GemVal _t4394 = _t4393;
    gem_pop_frame();
    return _t4394;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 40);
    GemVal _t4395 = gem_table_new();
    gem_table_set(_t4395, gem_string("key"), gem_v_key);
    gem_table_set(_t4395, gem_string("value"), gem_v_value);
    GemVal _t4396 = _t4395;
    gem_pop_frame();
    return _t4396;
}

static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("module_name_from_path", "compiler/main.gem", 46);
#line 47 "compiler/main.gem"
    GemVal _t4397[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4397, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 49 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 50 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 51 "compiler/main.gem"
    GemVal _t4398[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4398, 2), gem_int(47)))) {
#line 52 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 54 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 56 "compiler/main.gem"
    GemVal _t4399[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4399, 3);
#line 57 "compiler/main.gem"
    GemVal _t4400[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4400, 1);
#line 58 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 59 "compiler/main.gem"
    GemVal _t4401 = gem_v_string;
    GemVal _t4402[] = {gem_v_name, gem_string(".gem")};
    GemVal _t4403 = gem_table_get(_t4401, gem_string("ends_with"));
        if (gem_truthy(_t4403.fn(_t4403.env, _t4402, 2))) {
#line 60 "compiler/main.gem"
    GemVal _t4404[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4404, 3);
        }
    }
    GemVal _t4405 = gem_v_name;
    gem_pop_frame();
    return _t4405;
}

static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("find_export_node", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal _t4406[] = {gem_v_stmts};
    GemVal gem_v_i = gem_sub(gem_len_fn(NULL, _t4406, 1), gem_int(1));
#line 68 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_i, gem_int(0)))) break;
#line 69 "compiler/main.gem"
    GemVal _t4407[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("export")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4407, 2))) {
#line 70 "compiler/main.gem"
            GemVal _t4408 = gem_table_get(gem_v_stmts, gem_v_i);
            gem_pop_frame();
            return _t4408;
        }
#line 72 "compiler/main.gem"
        gem_v_i = gem_sub(gem_v_i, gem_int(1));
    }
    GemVal _t4409 = GEM_NIL;
    gem_pop_frame();
    return _t4409;
}

static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc) {
#line 77 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("collect_module_top_names", "compiler/main.gem", 77);
#line 78 "compiler/main.gem"
    GemVal _t4410 = gem_table_new();
    GemVal gem_v_names = _t4410;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_items_1 = gem_v_stmts;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 79 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4411[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t4411, 1)))) break;
#line 79 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 79 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 80 "compiler/main.gem"
    GemVal _t4412[] = {gem_v_stmt, gem_string("fn_def")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4412, 2))) {
#line 81 "compiler/main.gem"
    GemVal _t4413 = gem_v_string;
    GemVal _t4414 = gem_v_stmt;
    GemVal _t4415[] = {gem_table_get(_t4414, gem_string("name")), gem_string("_mod_")};
    GemVal _t4416 = gem_table_get(_t4413, gem_string("starts_with"));
            if (gem_truthy(gem_not(_t4416.fn(_t4416.env, _t4415, 2)))) {
#line 82 "compiler/main.gem"
    GemVal _t4417 = gem_v_stmt;
                gem_table_set(gem_v_names, gem_table_get(_t4417, gem_string("name")), gem_bool(1));
            }
        } else {
#line 84 "compiler/main.gem"
    GemVal _t4418[] = {gem_v_stmt, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t4418, 2))) {
#line 85 "compiler/main.gem"
    GemVal _t4419 = gem_v_string;
    GemVal _t4420 = gem_v_stmt;
    GemVal _t4421[] = {gem_table_get(_t4420, gem_string("name")), gem_string("_mod_")};
    GemVal _t4422 = gem_table_get(_t4419, gem_string("starts_with"));
                if (gem_truthy(gem_not(_t4422.fn(_t4422.env, _t4421, 2)))) {
#line 86 "compiler/main.gem"
    GemVal _t4423 = gem_v_stmt;
                    gem_table_set(gem_v_names, gem_table_get(_t4423, gem_string("name")), gem_bool(1));
                }
            }
        }
    }

    GemVal _t4424 = gem_v_names;
    gem_pop_frame();
    return _t4424;
}

static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_top_names = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("build_rename_map", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t4425 = gem_table_new();
    GemVal gem_v_rmap = _t4425;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_tbl_2 = gem_v_top_names;
#line 95 "compiler/main.gem"
    GemVal _t4426[] = {gem_v__for_tbl_2};
    GemVal gem_v__for_len_2 = gem_len_fn(NULL, _t4426, 1);
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_len_2))) break;
#line 95 "compiler/main.gem"
    GemVal _t4427[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v_name = gem_table_key_at_fn(NULL, _t4427, 2);
#line 95 "compiler/main.gem"
    GemVal _t4428[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4428, 2);
#line 95 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_rmap, gem_v_name, gem_add(gem_v_prefix, gem_v_name));
    }

    GemVal _t4429 = gem_v_rmap;
    gem_pop_frame();
    return _t4429;
}

static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("copy_set", "compiler/main.gem", 101);
#line 102 "compiler/main.gem"
    GemVal _t4430 = gem_table_new();
    GemVal gem_v_result = _t4430;
#line 103 "compiler/main.gem"
    GemVal gem_v__for_tbl_3 = gem_v_s;
#line 103 "compiler/main.gem"
    GemVal _t4431[] = {gem_v__for_tbl_3};
    GemVal gem_v__for_len_3 = gem_len_fn(NULL, _t4431, 1);
#line 103 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 103 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_len_3))) break;
#line 103 "compiler/main.gem"
    GemVal _t4432[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4432, 2);
#line 103 "compiler/main.gem"
    GemVal _t4433[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_v = gem_table_val_at_fn(NULL, _t4433, 2);
#line 103 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 104 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_v_v);
    }

    GemVal _t4434 = gem_v_result;
    gem_pop_frame();
    return _t4434;
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
            GemVal _t4435 = GEM_NIL;
            gem_pop_frame();
            return _t4435;
        }
#line 113 "compiler/main.gem"
    GemVal _t4436[] = {gem_v_node};
        if (gem_truthy(gem_neq(gem_type_fn(NULL, _t4436, 1), gem_string("table")))) {
#line 114 "compiler/main.gem"
            GemVal _t4437 = GEM_NIL;
            gem_pop_frame();
            return _t4437;
        }
#line 116 "compiler/main.gem"
    GemVal _t4438 = gem_v_node;
        if (gem_truthy(gem_eq(gem_table_get(_t4438, gem_string("tag")), GEM_NIL))) {
#line 117 "compiler/main.gem"
            GemVal _t4439 = GEM_NIL;
            gem_pop_frame();
            return _t4439;
        }
#line 120 "compiler/main.gem"
        GemVal gem_v__match_4 = gem_v_node;
    GemVal _t4440[] = {gem_v__match_4};
    GemVal _t4442;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4440, 1), gem_string("table")))) {
        _t4442 = gem_eq(gem_type_fn(NULL, _t4440, 1), gem_string("table"));
    } else {
        GemVal _t4441[] = {gem_v__match_4, gem_string("tag")};
        _t4442 = gem_has_key_fn(NULL, _t4441, 2);
    }
    GemVal _t4443;
    if (!gem_truthy(_t4442)) {
        _t4443 = _t4442;
    } else {
        _t4443 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("var"));
    }
    GemVal _t4445;
    if (!gem_truthy(_t4443)) {
        _t4445 = _t4443;
    } else {
        GemVal _t4444[] = {gem_v__match_4, gem_string("name")};
        _t4445 = gem_has_key_fn(NULL, _t4444, 2);
    }
        if (gem_truthy(_t4445)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 122 "compiler/main.gem"
    GemVal _t4446[] = {gem_v_rmap, gem_v_name};
    GemVal _t4448;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4446, 2))) {
        _t4448 = gem_has_key_fn(NULL, _t4446, 2);
    } else {
        GemVal _t4447[] = {gem_v_shadowed, gem_v_name};
        _t4448 = gem_not(gem_has_key_fn(NULL, _t4447, 2));
    }
            if (gem_truthy(_t4448)) {
#line 123 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4449[] = {gem_v__match_4};
    GemVal _t4451;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4449, 1), gem_string("table")))) {
        _t4451 = gem_eq(gem_type_fn(NULL, _t4449, 1), gem_string("table"));
    } else {
        GemVal _t4450[] = {gem_v__match_4, gem_string("tag")};
        _t4451 = gem_has_key_fn(NULL, _t4450, 2);
    }
    GemVal _t4452;
    if (!gem_truthy(_t4451)) {
        _t4452 = _t4451;
    } else {
        _t4452 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("fn_def"));
    }
    GemVal _t4454;
    if (!gem_truthy(_t4452)) {
        _t4454 = _t4452;
    } else {
        GemVal _t4453[] = {gem_v__match_4, gem_string("name")};
        _t4454 = gem_has_key_fn(NULL, _t4453, 2);
    }
    GemVal _t4456;
    if (!gem_truthy(_t4454)) {
        _t4456 = _t4454;
    } else {
        GemVal _t4455[] = {gem_v__match_4, gem_string("params")};
        _t4456 = gem_has_key_fn(NULL, _t4455, 2);
    }
    GemVal _t4458;
    if (!gem_truthy(_t4456)) {
        _t4458 = _t4456;
    } else {
        GemVal _t4457[] = {gem_v__match_4, gem_string("body")};
        _t4458 = gem_has_key_fn(NULL, _t4457, 2);
    }
        if (gem_truthy(_t4458)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 126 "compiler/main.gem"
    GemVal _t4459[] = {gem_v_rmap, gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t4459, 2))) {
#line 127 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
            }
#line 129 "compiler/main.gem"
    GemVal _t4460[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4460, 1);
#line 130 "compiler/main.gem"
            GemVal gem_v__for_items_5 = gem_v_params;
#line 130 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4461[] = {gem_v__for_items_5};
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t4461, 1)))) break;
#line 130 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 133 "compiler/main.gem"
    GemVal _t4462 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4462, gem_string("rest_param")), GEM_NIL))) {
#line 134 "compiler/main.gem"
    GemVal _t4463 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4463, gem_string("rest_param")), gem_bool(1));
            }
#line 136 "compiler/main.gem"
    GemVal _t4464[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4465 = gem_fn_rename_stmts(NULL, _t4464, 3);
            gem_pop_frame();
            return _t4465;
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
        _t4469 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t4471;
    if (!gem_truthy(_t4469)) {
        _t4471 = _t4469;
    } else {
        GemVal _t4470[] = {gem_v__match_4, gem_string("params")};
        _t4471 = gem_has_key_fn(NULL, _t4470, 2);
    }
    GemVal _t4473;
    if (!gem_truthy(_t4471)) {
        _t4473 = _t4471;
    } else {
        GemVal _t4472[] = {gem_v__match_4, gem_string("body")};
        _t4473 = gem_has_key_fn(NULL, _t4472, 2);
    }
        if (gem_truthy(_t4473)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 138 "compiler/main.gem"
    GemVal _t4474[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4474, 1);
#line 139 "compiler/main.gem"
            GemVal gem_v__for_items_6 = gem_v_params;
#line 139 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 139 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4475[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t4475, 1)))) break;
#line 139 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 139 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 140 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 142 "compiler/main.gem"
    GemVal _t4476 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4476, gem_string("rest_param")), GEM_NIL))) {
#line 143 "compiler/main.gem"
    GemVal _t4477 = gem_v_node;
                gem_table_set(gem_v_inner, gem_table_get(_t4477, gem_string("rest_param")), gem_bool(1));
            }
#line 145 "compiler/main.gem"
    GemVal _t4478[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4479 = gem_fn_rename_stmts(NULL, _t4478, 3);
            gem_pop_frame();
            return _t4479;
        } else {
    GemVal _t4480[] = {gem_v__match_4};
    GemVal _t4482;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4480, 1), gem_string("table")))) {
        _t4482 = gem_eq(gem_type_fn(NULL, _t4480, 1), gem_string("table"));
    } else {
        GemVal _t4481[] = {gem_v__match_4, gem_string("tag")};
        _t4482 = gem_has_key_fn(NULL, _t4481, 2);
    }
    GemVal _t4483;
    if (!gem_truthy(_t4482)) {
        _t4483 = _t4482;
    } else {
        _t4483 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("let"));
    }
    GemVal _t4485;
    if (!gem_truthy(_t4483)) {
        _t4485 = _t4483;
    } else {
        GemVal _t4484[] = {gem_v__match_4, gem_string("name")};
        _t4485 = gem_has_key_fn(NULL, _t4484, 2);
    }
    GemVal _t4487;
    if (!gem_truthy(_t4485)) {
        _t4487 = _t4485;
    } else {
        GemVal _t4486[] = {gem_v__match_4, gem_string("value")};
        _t4487 = gem_has_key_fn(NULL, _t4486, 2);
    }
        if (gem_truthy(_t4487)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 147 "compiler/main.gem"
    GemVal _t4488[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4488, 3));
#line 148 "compiler/main.gem"
    GemVal _t4489[] = {gem_v_rmap, gem_v_name};
    GemVal _t4491;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4489, 2))) {
        _t4491 = gem_has_key_fn(NULL, _t4489, 2);
    } else {
        GemVal _t4490[] = {gem_v_shadowed, gem_v_name};
        _t4491 = gem_not(gem_has_key_fn(NULL, _t4490, 2));
    }
            if (gem_truthy(_t4491)) {
#line 149 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4492[] = {gem_v__match_4};
    GemVal _t4494;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4492, 1), gem_string("table")))) {
        _t4494 = gem_eq(gem_type_fn(NULL, _t4492, 1), gem_string("table"));
    } else {
        GemVal _t4493[] = {gem_v__match_4, gem_string("tag")};
        _t4494 = gem_has_key_fn(NULL, _t4493, 2);
    }
    GemVal _t4495;
    if (!gem_truthy(_t4494)) {
        _t4495 = _t4494;
    } else {
        _t4495 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t4497;
    if (!gem_truthy(_t4495)) {
        _t4497 = _t4495;
    } else {
        GemVal _t4496[] = {gem_v__match_4, gem_string("name")};
        _t4497 = gem_has_key_fn(NULL, _t4496, 2);
    }
    GemVal _t4499;
    if (!gem_truthy(_t4497)) {
        _t4499 = _t4497;
    } else {
        GemVal _t4498[] = {gem_v__match_4, gem_string("value")};
        _t4499 = gem_has_key_fn(NULL, _t4498, 2);
    }
        if (gem_truthy(_t4499)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 152 "compiler/main.gem"
    GemVal _t4500[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4500, 3));
#line 153 "compiler/main.gem"
    GemVal _t4501[] = {gem_v_rmap, gem_v_name};
    GemVal _t4503;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4501, 2))) {
        _t4503 = gem_has_key_fn(NULL, _t4501, 2);
    } else {
        GemVal _t4502[] = {gem_v_shadowed, gem_v_name};
        _t4503 = gem_not(gem_has_key_fn(NULL, _t4502, 2));
    }
            if (gem_truthy(_t4503)) {
#line 154 "compiler/main.gem"
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
        _t4507 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("call"));
    }
    GemVal _t4509;
    if (!gem_truthy(_t4507)) {
        _t4509 = _t4507;
    } else {
        GemVal _t4508[] = {gem_v__match_4, gem_string("func")};
        _t4509 = gem_has_key_fn(NULL, _t4508, 2);
    }
    GemVal _t4511;
    if (!gem_truthy(_t4509)) {
        _t4511 = _t4509;
    } else {
        GemVal _t4510[] = {gem_v__match_4, gem_string("args")};
        _t4511 = gem_has_key_fn(NULL, _t4510, 2);
    }
        if (gem_truthy(_t4511)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_func = gem_table_get(gem_v__match_4, gem_string("func"));
#line 0 "compiler/main.gem"
            GemVal gem_v_args = gem_table_get(gem_v__match_4, gem_string("args"));
#line 157 "compiler/main.gem"
    GemVal _t4512[] = {gem_v_func, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4512, 3));
            {
#line 158 "compiler/main.gem"
                GemVal gem_v__for_items_7 = gem_v_args;
#line 158 "compiler/main.gem"
                GemVal gem_v__for_i_7 = gem_int(0);
#line 158 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4513[] = {gem_v__for_items_7};
                    if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t4513, 1)))) break;
#line 158 "compiler/main.gem"
                    GemVal gem_v_a = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 158 "compiler/main.gem"
                    gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 159 "compiler/main.gem"
    GemVal _t4514[] = {gem_v_a, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4514, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4515[] = {gem_v__match_4};
    GemVal _t4517;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4515, 1), gem_string("table")))) {
        _t4517 = gem_eq(gem_type_fn(NULL, _t4515, 1), gem_string("table"));
    } else {
        GemVal _t4516[] = {gem_v__match_4, gem_string("tag")};
        _t4517 = gem_has_key_fn(NULL, _t4516, 2);
    }
    GemVal _t4518;
    if (!gem_truthy(_t4517)) {
        _t4518 = _t4517;
    } else {
        _t4518 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("binop"));
    }
    GemVal _t4520;
    if (!gem_truthy(_t4518)) {
        _t4520 = _t4518;
    } else {
        GemVal _t4519[] = {gem_v__match_4, gem_string("left")};
        _t4520 = gem_has_key_fn(NULL, _t4519, 2);
    }
    GemVal _t4522;
    if (!gem_truthy(_t4520)) {
        _t4522 = _t4520;
    } else {
        GemVal _t4521[] = {gem_v__match_4, gem_string("right")};
        _t4522 = gem_has_key_fn(NULL, _t4521, 2);
    }
        if (gem_truthy(_t4522)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_left = gem_table_get(gem_v__match_4, gem_string("left"));
#line 0 "compiler/main.gem"
            GemVal gem_v_right = gem_table_get(gem_v__match_4, gem_string("right"));
#line 162 "compiler/main.gem"
    GemVal _t4523[] = {gem_v_left, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4523, 3));
#line 163 "compiler/main.gem"
            GemVal _t4524 = gem_v_right;
            GemVal _t4525 = gem_v_rmap;
            GemVal _t4526 = gem_v_shadowed;
            gem_v_node = _t4524;
            gem_v_rmap = _t4525;
            gem_v_shadowed = _t4526;
            continue;
        } else {
    GemVal _t4527[] = {gem_v__match_4};
    GemVal _t4529;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4527, 1), gem_string("table")))) {
        _t4529 = gem_eq(gem_type_fn(NULL, _t4527, 1), gem_string("table"));
    } else {
        GemVal _t4528[] = {gem_v__match_4, gem_string("tag")};
        _t4529 = gem_has_key_fn(NULL, _t4528, 2);
    }
    GemVal _t4530;
    if (!gem_truthy(_t4529)) {
        _t4530 = _t4529;
    } else {
        _t4530 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t4532;
    if (!gem_truthy(_t4530)) {
        _t4532 = _t4530;
    } else {
        GemVal _t4531[] = {gem_v__match_4, gem_string("expr")};
        _t4532 = gem_has_key_fn(NULL, _t4531, 2);
    }
        if (gem_truthy(_t4532)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_expr = gem_table_get(gem_v__match_4, gem_string("expr"));
#line 165 "compiler/main.gem"
            GemVal _t4533 = gem_v_expr;
            GemVal _t4534 = gem_v_rmap;
            GemVal _t4535 = gem_v_shadowed;
            gem_v_node = _t4533;
            gem_v_rmap = _t4534;
            gem_v_shadowed = _t4535;
            continue;
        } else {
    GemVal _t4536[] = {gem_v__match_4};
    GemVal _t4538;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4536, 1), gem_string("table")))) {
        _t4538 = gem_eq(gem_type_fn(NULL, _t4536, 1), gem_string("table"));
    } else {
        GemVal _t4537[] = {gem_v__match_4, gem_string("tag")};
        _t4538 = gem_has_key_fn(NULL, _t4537, 2);
    }
    GemVal _t4539;
    if (!gem_truthy(_t4538)) {
        _t4539 = _t4538;
    } else {
        _t4539 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t4541;
    if (!gem_truthy(_t4539)) {
        _t4541 = _t4539;
    } else {
        GemVal _t4540[] = {gem_v__match_4, gem_string("object")};
        _t4541 = gem_has_key_fn(NULL, _t4540, 2);
    }
        if (gem_truthy(_t4541)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 167 "compiler/main.gem"
            GemVal _t4542 = gem_v_object;
            GemVal _t4543 = gem_v_rmap;
            GemVal _t4544 = gem_v_shadowed;
            gem_v_node = _t4542;
            gem_v_rmap = _t4543;
            gem_v_shadowed = _t4544;
            continue;
        } else {
    GemVal _t4545[] = {gem_v__match_4};
    GemVal _t4547;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4545, 1), gem_string("table")))) {
        _t4547 = gem_eq(gem_type_fn(NULL, _t4545, 1), gem_string("table"));
    } else {
        GemVal _t4546[] = {gem_v__match_4, gem_string("tag")};
        _t4547 = gem_has_key_fn(NULL, _t4546, 2);
    }
    GemVal _t4548;
    if (!gem_truthy(_t4547)) {
        _t4548 = _t4547;
    } else {
        _t4548 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index"));
    }
    GemVal _t4550;
    if (!gem_truthy(_t4548)) {
        _t4550 = _t4548;
    } else {
        GemVal _t4549[] = {gem_v__match_4, gem_string("object")};
        _t4550 = gem_has_key_fn(NULL, _t4549, 2);
    }
    GemVal _t4552;
    if (!gem_truthy(_t4550)) {
        _t4552 = _t4550;
    } else {
        GemVal _t4551[] = {gem_v__match_4, gem_string("key")};
        _t4552 = gem_has_key_fn(NULL, _t4551, 2);
    }
        if (gem_truthy(_t4552)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 169 "compiler/main.gem"
    GemVal _t4553[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4553, 3));
#line 170 "compiler/main.gem"
            GemVal _t4554 = gem_v_key;
            GemVal _t4555 = gem_v_rmap;
            GemVal _t4556 = gem_v_shadowed;
            gem_v_node = _t4554;
            gem_v_rmap = _t4555;
            gem_v_shadowed = _t4556;
            continue;
        } else {
    GemVal _t4557[] = {gem_v__match_4};
    GemVal _t4559;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4557, 1), gem_string("table")))) {
        _t4559 = gem_eq(gem_type_fn(NULL, _t4557, 1), gem_string("table"));
    } else {
        GemVal _t4558[] = {gem_v__match_4, gem_string("tag")};
        _t4559 = gem_has_key_fn(NULL, _t4558, 2);
    }
    GemVal _t4560;
    if (!gem_truthy(_t4559)) {
        _t4560 = _t4559;
    } else {
        _t4560 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t4562;
    if (!gem_truthy(_t4560)) {
        _t4562 = _t4560;
    } else {
        GemVal _t4561[] = {gem_v__match_4, gem_string("object")};
        _t4562 = gem_has_key_fn(NULL, _t4561, 2);
    }
    GemVal _t4564;
    if (!gem_truthy(_t4562)) {
        _t4564 = _t4562;
    } else {
        GemVal _t4563[] = {gem_v__match_4, gem_string("value")};
        _t4564 = gem_has_key_fn(NULL, _t4563, 2);
    }
        if (gem_truthy(_t4564)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 172 "compiler/main.gem"
    GemVal _t4565[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4565, 3));
#line 173 "compiler/main.gem"
            GemVal _t4566 = gem_v_value;
            GemVal _t4567 = gem_v_rmap;
            GemVal _t4568 = gem_v_shadowed;
            gem_v_node = _t4566;
            gem_v_rmap = _t4567;
            gem_v_shadowed = _t4568;
            continue;
        } else {
    GemVal _t4569[] = {gem_v__match_4};
    GemVal _t4571;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4569, 1), gem_string("table")))) {
        _t4571 = gem_eq(gem_type_fn(NULL, _t4569, 1), gem_string("table"));
    } else {
        GemVal _t4570[] = {gem_v__match_4, gem_string("tag")};
        _t4571 = gem_has_key_fn(NULL, _t4570, 2);
    }
    GemVal _t4572;
    if (!gem_truthy(_t4571)) {
        _t4572 = _t4571;
    } else {
        _t4572 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t4574;
    if (!gem_truthy(_t4572)) {
        _t4574 = _t4572;
    } else {
        GemVal _t4573[] = {gem_v__match_4, gem_string("object")};
        _t4574 = gem_has_key_fn(NULL, _t4573, 2);
    }
    GemVal _t4576;
    if (!gem_truthy(_t4574)) {
        _t4576 = _t4574;
    } else {
        GemVal _t4575[] = {gem_v__match_4, gem_string("key")};
        _t4576 = gem_has_key_fn(NULL, _t4575, 2);
    }
    GemVal _t4578;
    if (!gem_truthy(_t4576)) {
        _t4578 = _t4576;
    } else {
        GemVal _t4577[] = {gem_v__match_4, gem_string("value")};
        _t4578 = gem_has_key_fn(NULL, _t4577, 2);
    }
        if (gem_truthy(_t4578)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 175 "compiler/main.gem"
    GemVal _t4579[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4579, 3));
#line 176 "compiler/main.gem"
    GemVal _t4580[] = {gem_v_key, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4580, 3));
#line 177 "compiler/main.gem"
            GemVal _t4581 = gem_v_value;
            GemVal _t4582 = gem_v_rmap;
            GemVal _t4583 = gem_v_shadowed;
            gem_v_node = _t4581;
            gem_v_rmap = _t4582;
            gem_v_shadowed = _t4583;
            continue;
        } else {
    GemVal _t4584[] = {gem_v__match_4};
    GemVal _t4586;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4584, 1), gem_string("table")))) {
        _t4586 = gem_eq(gem_type_fn(NULL, _t4584, 1), gem_string("table"));
    } else {
        GemVal _t4585[] = {gem_v__match_4, gem_string("tag")};
        _t4586 = gem_has_key_fn(NULL, _t4585, 2);
    }
    GemVal _t4587;
    if (!gem_truthy(_t4586)) {
        _t4587 = _t4586;
    } else {
        _t4587 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("if"));
    }
    GemVal _t4589;
    if (!gem_truthy(_t4587)) {
        _t4589 = _t4587;
    } else {
        GemVal _t4588[] = {gem_v__match_4, gem_string("cond")};
        _t4589 = gem_has_key_fn(NULL, _t4588, 2);
    }
        if (gem_truthy(_t4589)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 179 "compiler/main.gem"
    GemVal _t4590[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4590, 3));
#line 180 "compiler/main.gem"
    GemVal _t4591 = gem_v_node;
    GemVal _t4592[] = {gem_table_get(_t4591, gem_string("then")), gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_stmts(NULL, _t4592, 3));
#line 181 "compiler/main.gem"
    GemVal _t4593 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4593, gem_string("else")), GEM_NIL))) {
#line 182 "compiler/main.gem"
    GemVal _t4594 = gem_v_node;
    GemVal _t4595[] = {gem_table_get(_t4594, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4596 = gem_fn_rename_stmts(NULL, _t4595, 3);
                gem_pop_frame();
                return _t4596;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4597[] = {gem_v__match_4};
    GemVal _t4599;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4597, 1), gem_string("table")))) {
        _t4599 = gem_eq(gem_type_fn(NULL, _t4597, 1), gem_string("table"));
    } else {
        GemVal _t4598[] = {gem_v__match_4, gem_string("tag")};
        _t4599 = gem_has_key_fn(NULL, _t4598, 2);
    }
    GemVal _t4600;
    if (!gem_truthy(_t4599)) {
        _t4600 = _t4599;
    } else {
        _t4600 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("while"));
    }
    GemVal _t4602;
    if (!gem_truthy(_t4600)) {
        _t4602 = _t4600;
    } else {
        GemVal _t4601[] = {gem_v__match_4, gem_string("cond")};
        _t4602 = gem_has_key_fn(NULL, _t4601, 2);
    }
    GemVal _t4604;
    if (!gem_truthy(_t4602)) {
        _t4604 = _t4602;
    } else {
        GemVal _t4603[] = {gem_v__match_4, gem_string("body")};
        _t4604 = gem_has_key_fn(NULL, _t4603, 2);
    }
        if (gem_truthy(_t4604)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 185 "compiler/main.gem"
    GemVal _t4605[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4605, 3));
#line 186 "compiler/main.gem"
    GemVal _t4606[] = {gem_v_body, gem_v_rmap, gem_v_shadowed};
            GemVal _t4607 = gem_fn_rename_stmts(NULL, _t4606, 3);
            gem_pop_frame();
            return _t4607;
        } else {
    GemVal _t4608[] = {gem_v__match_4};
    GemVal _t4610;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4608, 1), gem_string("table")))) {
        _t4610 = gem_eq(gem_type_fn(NULL, _t4608, 1), gem_string("table"));
    } else {
        GemVal _t4609[] = {gem_v__match_4, gem_string("tag")};
        _t4610 = gem_has_key_fn(NULL, _t4609, 2);
    }
    GemVal _t4611;
    if (!gem_truthy(_t4610)) {
        _t4611 = _t4610;
    } else {
        _t4611 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("match"));
    }
    GemVal _t4613;
    if (!gem_truthy(_t4611)) {
        _t4613 = _t4611;
    } else {
        GemVal _t4612[] = {gem_v__match_4, gem_string("target")};
        _t4613 = gem_has_key_fn(NULL, _t4612, 2);
    }
    GemVal _t4615;
    if (!gem_truthy(_t4613)) {
        _t4615 = _t4613;
    } else {
        GemVal _t4614[] = {gem_v__match_4, gem_string("whens")};
        _t4615 = gem_has_key_fn(NULL, _t4614, 2);
    }
        if (gem_truthy(_t4615)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_target = gem_table_get(gem_v__match_4, gem_string("target"));
#line 0 "compiler/main.gem"
            GemVal gem_v_whens = gem_table_get(gem_v__match_4, gem_string("whens"));
#line 188 "compiler/main.gem"
    GemVal _t4616[] = {gem_v_target, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4616, 3));
#line 189 "compiler/main.gem"
            GemVal gem_v__for_items_8 = gem_v_whens;
#line 189 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 189 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4617[] = {gem_v__for_items_8};
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_len_fn(NULL, _t4617, 1)))) break;
#line 189 "compiler/main.gem"
                GemVal gem_v_w = gem_table_get(gem_v__for_items_8, gem_v__for_i_8);
#line 189 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 190 "compiler/main.gem"
    GemVal _t4618 = gem_v_w;
    GemVal _t4619[] = {gem_table_get(_t4618, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4619, 3));
#line 191 "compiler/main.gem"
    GemVal _t4620 = gem_v_w;
                if (gem_truthy(gem_neq(gem_table_get(_t4620, gem_string("bindings")), GEM_NIL))) {
#line 192 "compiler/main.gem"
    GemVal _t4621 = gem_v_w;
                    GemVal gem_v__for_items_9 = gem_table_get(_t4621, gem_string("bindings"));
#line 192 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 192 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4622[] = {gem_v__for_items_9};
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t4622, 1)))) break;
#line 192 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_9, gem_v__for_i_9);
#line 192 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t4623[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4623, 3));
                    }

                }
#line 196 "compiler/main.gem"
    GemVal _t4624 = gem_v_w;
    GemVal _t4625[] = {gem_table_get(_t4624, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4625, 3));
            }

#line 198 "compiler/main.gem"
    GemVal _t4626 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4626, gem_string("else")), GEM_NIL))) {
#line 199 "compiler/main.gem"
    GemVal _t4627 = gem_v_node;
    GemVal _t4628[] = {gem_table_get(_t4627, gem_string("else")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4629 = gem_fn_rename_stmts(NULL, _t4628, 3);
                gem_pop_frame();
                return _t4629;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4630[] = {gem_v__match_4};
    GemVal _t4632;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4630, 1), gem_string("table")))) {
        _t4632 = gem_eq(gem_type_fn(NULL, _t4630, 1), gem_string("table"));
    } else {
        GemVal _t4631[] = {gem_v__match_4, gem_string("tag")};
        _t4632 = gem_has_key_fn(NULL, _t4631, 2);
    }
    GemVal _t4633;
    if (!gem_truthy(_t4632)) {
        _t4633 = _t4632;
    } else {
        _t4633 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t4635;
    if (!gem_truthy(_t4633)) {
        _t4635 = _t4633;
    } else {
        GemVal _t4634[] = {gem_v__match_4, gem_string("arms")};
        _t4635 = gem_has_key_fn(NULL, _t4634, 2);
    }
        if (gem_truthy(_t4635)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_arms = gem_table_get(gem_v__match_4, gem_string("arms"));
#line 202 "compiler/main.gem"
            GemVal gem_v__for_items_10 = gem_v_arms;
#line 202 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 202 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4636[] = {gem_v__for_items_10};
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t4636, 1)))) break;
#line 202 "compiler/main.gem"
                GemVal gem_v_arm = gem_table_get(gem_v__for_items_10, gem_v__for_i_10);
#line 202 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 203 "compiler/main.gem"
    GemVal _t4637 = gem_v_arm;
    GemVal _t4638 = gem_table_get(_t4637, gem_string("pattern"));
    GemVal _t4639[] = {gem_table_get(_t4638, gem_string("condition")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4639, 3));
#line 204 "compiler/main.gem"
    GemVal _t4640 = gem_v_arm;
    GemVal _t4641 = gem_table_get(_t4640, gem_string("pattern"));
                if (gem_truthy(gem_neq(gem_table_get(_t4641, gem_string("bindings")), GEM_NIL))) {
#line 205 "compiler/main.gem"
    GemVal _t4642 = gem_v_arm;
    GemVal _t4643 = gem_table_get(_t4642, gem_string("pattern"));
                    GemVal gem_v__for_items_11 = gem_table_get(_t4643, gem_string("bindings"));
#line 205 "compiler/main.gem"
                    GemVal gem_v__for_i_11 = gem_int(0);
#line 205 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4644[] = {gem_v__for_items_11};
                        if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t4644, 1)))) break;
#line 205 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_11, gem_v__for_i_11);
#line 205 "compiler/main.gem"
                        gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 206 "compiler/main.gem"
    GemVal _t4645[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4645, 3));
                    }

                }
#line 209 "compiler/main.gem"
    GemVal _t4646 = gem_v_arm;
    GemVal _t4647[] = {gem_table_get(_t4646, gem_string("body")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4647, 3));
            }

#line 211 "compiler/main.gem"
    GemVal _t4648 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4648, gem_string("after_ms")), GEM_NIL))) {
#line 212 "compiler/main.gem"
    GemVal _t4649 = gem_v_node;
    GemVal _t4650[] = {gem_table_get(_t4649, gem_string("after_ms")), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4650, 3));
            }
#line 214 "compiler/main.gem"
    GemVal _t4651 = gem_v_node;
            if (gem_truthy(gem_neq(gem_table_get(_t4651, gem_string("after_body")), GEM_NIL))) {
#line 215 "compiler/main.gem"
    GemVal _t4652 = gem_v_node;
    GemVal _t4653[] = {gem_table_get(_t4652, gem_string("after_body")), gem_v_rmap, gem_v_shadowed};
                GemVal _t4654 = gem_fn_rename_stmts(NULL, _t4653, 3);
                gem_pop_frame();
                return _t4654;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4655[] = {gem_v__match_4};
    GemVal _t4657;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4655, 1), gem_string("table")))) {
        _t4657 = gem_eq(gem_type_fn(NULL, _t4655, 1), gem_string("table"));
    } else {
        GemVal _t4656[] = {gem_v__match_4, gem_string("tag")};
        _t4657 = gem_has_key_fn(NULL, _t4656, 2);
    }
    GemVal _t4658;
    if (!gem_truthy(_t4657)) {
        _t4658 = _t4657;
    } else {
        _t4658 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("return"));
    }
    GemVal _t4660;
    if (!gem_truthy(_t4658)) {
        _t4660 = _t4658;
    } else {
        GemVal _t4659[] = {gem_v__match_4, gem_string("value")};
        _t4660 = gem_has_key_fn(NULL, _t4659, 2);
    }
        if (gem_truthy(_t4660)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 218 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 219 "compiler/main.gem"
                GemVal _t4661 = gem_v_value;
                GemVal _t4662 = gem_v_rmap;
                GemVal _t4663 = gem_v_shadowed;
                gem_v_node = _t4661;
                gem_v_rmap = _t4662;
                gem_v_shadowed = _t4663;
                continue;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4664[] = {gem_v__match_4};
    GemVal _t4666;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4664, 1), gem_string("table")))) {
        _t4666 = gem_eq(gem_type_fn(NULL, _t4664, 1), gem_string("table"));
    } else {
        GemVal _t4665[] = {gem_v__match_4, gem_string("tag")};
        _t4666 = gem_has_key_fn(NULL, _t4665, 2);
    }
    GemVal _t4667;
    if (!gem_truthy(_t4666)) {
        _t4667 = _t4666;
    } else {
        _t4667 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("table"));
    }
    GemVal _t4669;
    if (!gem_truthy(_t4667)) {
        _t4669 = _t4667;
    } else {
        GemVal _t4668[] = {gem_v__match_4, gem_string("entries")};
        _t4669 = gem_has_key_fn(NULL, _t4668, 2);
    }
        if (gem_truthy(_t4669)) {
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
                    GemVal _t4670[] = {gem_v__for_items_12};
                    if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t4670, 1)))) break;
#line 222 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 222 "compiler/main.gem"
                    gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 223 "compiler/main.gem"
    GemVal _t4671 = gem_v_e;
    GemVal _t4672[] = {gem_table_get(_t4671, gem_string("value")), gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4672, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4673[] = {gem_v__match_4};
    GemVal _t4675;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4673, 1), gem_string("table")))) {
        _t4675 = gem_eq(gem_type_fn(NULL, _t4673, 1), gem_string("table"));
    } else {
        GemVal _t4674[] = {gem_v__match_4, gem_string("tag")};
        _t4675 = gem_has_key_fn(NULL, _t4674, 2);
    }
    GemVal _t4676;
    if (!gem_truthy(_t4675)) {
        _t4676 = _t4675;
    } else {
        _t4676 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("array"));
    }
    GemVal _t4678;
    if (!gem_truthy(_t4676)) {
        _t4678 = _t4676;
    } else {
        GemVal _t4677[] = {gem_v__match_4, gem_string("elements")};
        _t4678 = gem_has_key_fn(NULL, _t4677, 2);
    }
        if (gem_truthy(_t4678)) {
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
                    GemVal _t4679[] = {gem_v__for_items_13};
                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_len_fn(NULL, _t4679, 1)))) break;
#line 226 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_13, gem_v__for_i_13);
#line 226 "compiler/main.gem"
                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 227 "compiler/main.gem"
    GemVal _t4680[] = {gem_v_e, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4680, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4681[] = {gem_v__match_4};
    GemVal _t4683;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4681, 1), gem_string("table")))) {
        _t4683 = gem_eq(gem_type_fn(NULL, _t4681, 1), gem_string("table"));
    } else {
        GemVal _t4682[] = {gem_v__match_4, gem_string("tag")};
        _t4683 = gem_has_key_fn(NULL, _t4682, 2);
    }
    GemVal _t4684;
    if (!gem_truthy(_t4683)) {
        _t4684 = _t4683;
    } else {
        _t4684 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t4686;
    if (!gem_truthy(_t4684)) {
        _t4686 = _t4684;
    } else {
        GemVal _t4685[] = {gem_v__match_4, gem_string("parts")};
        _t4686 = gem_has_key_fn(NULL, _t4685, 2);
    }
        if (gem_truthy(_t4686)) {
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
                    GemVal _t4687[] = {gem_v__for_items_14};
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_len_fn(NULL, _t4687, 1)))) break;
#line 230 "compiler/main.gem"
                    GemVal gem_v_p = gem_table_get(gem_v__for_items_14, gem_v__for_i_14);
#line 230 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 231 "compiler/main.gem"
    GemVal _t4688[] = {gem_v_p, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4688, 3));
                }
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
        _t4692 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("block"));
    }
    GemVal _t4694;
    if (!gem_truthy(_t4692)) {
        _t4694 = _t4692;
    } else {
        GemVal _t4693[] = {gem_v__match_4, gem_string("stmts")};
        _t4694 = gem_has_key_fn(NULL, _t4693, 2);
    }
        if (gem_truthy(_t4694)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_stmts = gem_table_get(gem_v__match_4, gem_string("stmts"));
#line 234 "compiler/main.gem"
    GemVal _t4695[] = {gem_v_stmts, gem_v_rmap, gem_v_shadowed};
            GemVal _t4696 = gem_fn_rename_stmts(NULL, _t4695, 3);
            gem_pop_frame();
            return _t4696;
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
    GemVal _t4697[] = {gem_v_shadowed};
    GemVal gem_v_current = gem_fn_copy_set(NULL, _t4697, 1);
    {
#line 240 "compiler/main.gem"
        GemVal gem_v__for_items_15 = gem_v_stmts;
#line 240 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 240 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4698[] = {gem_v__for_items_15};
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_len_fn(NULL, _t4698, 1)))) break;
#line 240 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_15, gem_v__for_i_15);
#line 240 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 241 "compiler/main.gem"
    GemVal _t4699[] = {gem_v_stmt, gem_v_rmap, gem_v_current};
            (void)(gem_fn_rename_node(NULL, _t4699, 3));
#line 242 "compiler/main.gem"
    GemVal _t4700[] = {gem_v_stmt, gem_string("let")};
    GemVal _t4703;
    if (!gem_truthy(gem_fn_is_node(NULL, _t4700, 2))) {
        _t4703 = gem_fn_is_node(NULL, _t4700, 2);
    } else {
        GemVal _t4701 = gem_v_stmt;
        GemVal _t4702[] = {gem_v_rmap, gem_table_get(_t4701, gem_string("name"))};
        _t4703 = gem_not(gem_has_key_fn(NULL, _t4702, 2));
    }
            if (gem_truthy(_t4703)) {
#line 243 "compiler/main.gem"
    GemVal _t4704 = gem_v_stmt;
                gem_table_set(gem_v_current, gem_table_get(_t4704, gem_string("name")), gem_bool(1));
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
    GemVal _t4705[] = {gem_v_stmts};
    GemVal gem_v_top_names = gem_fn_collect_module_top_names(NULL, _t4705, 1);
#line 251 "compiler/main.gem"
    GemVal _t4706[] = {gem_v_top_names, gem_v_prefix};
    GemVal gem_v_rmap = gem_fn_build_rename_map(NULL, _t4706, 2);
#line 253 "compiler/main.gem"
    GemVal _t4707 = gem_table_new();
    GemVal _t4708[] = {gem_v_stmts, gem_v_rmap, _t4707};
    (void)(gem_fn_rename_stmts(NULL, _t4708, 3));
#line 255 "compiler/main.gem"
    GemVal _t4709 = gem_table_new();
    GemVal gem_v_result = _t4709;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_items_16 = gem_v_stmts;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_i_16 = gem_int(0);
#line 256 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4710[] = {gem_v__for_items_16};
        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t4710, 1)))) break;
#line 256 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_16, gem_v__for_i_16);
#line 256 "compiler/main.gem"
        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 257 "compiler/main.gem"
    GemVal _t4711[] = {gem_v_stmt, gem_string("export")};
        if (gem_truthy(gem_not(gem_fn_is_node(NULL, _t4711, 2)))) {
#line 258 "compiler/main.gem"
    GemVal _t4712[] = {gem_v_result, gem_v_stmt};
            (void)(gem_push_fn(NULL, _t4712, 2));
        }
    }

#line 262 "compiler/main.gem"
    GemVal _t4713 = gem_v_export_node;
    GemVal gem_v_exported = gem_table_get(_t4713, gem_string("names"));
#line 264 "compiler/main.gem"
    GemVal _t4714 = gem_v_load_node;
    if (gem_truthy(gem_neq(gem_table_get(_t4714, gem_string("selective")), GEM_NIL))) {
#line 265 "compiler/main.gem"
    GemVal _t4715 = gem_v_load_node;
        GemVal gem_v__for_items_17 = gem_table_get(_t4715, gem_string("selective"));
#line 265 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 265 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4716[] = {gem_v__for_items_17};
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_len_fn(NULL, _t4716, 1)))) break;
#line 265 "compiler/main.gem"
            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_17, gem_v__for_i_17);
#line 265 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 266 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_sel_name);
#line 267 "compiler/main.gem"
    GemVal _t4717[] = {gem_v_prefixed};
    GemVal _t4718[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4717, 1), gem_int(0)};
    GemVal _t4719[] = {gem_v_result, gem_fn_make_let(NULL, _t4718, 3)};
            (void)(gem_push_fn(NULL, _t4719, 2));
        }

    } else {
#line 270 "compiler/main.gem"
        GemVal gem_v_binding_name = gem_v_mod_name;
#line 271 "compiler/main.gem"
    GemVal _t4720 = gem_v_load_node;
        if (gem_truthy(gem_neq(gem_table_get(_t4720, gem_string("alias")), GEM_NIL))) {
#line 272 "compiler/main.gem"
    GemVal _t4721 = gem_v_load_node;
            gem_v_binding_name = gem_table_get(_t4721, gem_string("alias"));
        }
#line 274 "compiler/main.gem"
    GemVal _t4722 = gem_table_new();
        GemVal gem_v_entries = _t4722;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_items_18 = gem_v_exported;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 275 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4723[] = {gem_v__for_items_18};
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_len_fn(NULL, _t4723, 1)))) break;
#line 275 "compiler/main.gem"
            GemVal gem_v_ename = gem_table_get(gem_v__for_items_18, gem_v__for_i_18);
#line 275 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 276 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_ename);
#line 277 "compiler/main.gem"
    GemVal _t4724[] = {gem_v_prefixed};
    GemVal _t4725[] = {gem_v_ename, gem_fn_make_var(NULL, _t4724, 1)};
    GemVal _t4726[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4725, 2)};
            (void)(gem_push_fn(NULL, _t4726, 2));
        }

#line 279 "compiler/main.gem"
    GemVal _t4727[] = {gem_v_entries};
    GemVal _t4728[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4727, 1), gem_int(0)};
    GemVal _t4729[] = {gem_v_result, gem_fn_make_let(NULL, _t4728, 3)};
        (void)(gem_push_fn(NULL, _t4729, 2));
    }
    GemVal _t4730 = gem_v_result;
    gem_pop_frame();
    return _t4730;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 287 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 287);
#line 288 "compiler/main.gem"
    GemVal _t4731 = gem_table_new();
    GemVal gem_v_new_stmts = _t4731;
#line 289 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 290 "compiler/main.gem"
    GemVal _t4732 = gem_v_ast;
    GemVal gem_v__for_items_19 = gem_table_get(_t4732, gem_string("stmts"));
#line 290 "compiler/main.gem"
    GemVal gem_v__for_i_19 = gem_int(0);
#line 290 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4733[] = {gem_v__for_items_19};
        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t4733, 1)))) break;
#line 290 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
#line 290 "compiler/main.gem"
        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 291 "compiler/main.gem"
    GemVal _t4734[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4734, 2))) {
#line 292 "compiler/main.gem"
    GemVal _t4735 = gem_v_stmt;
            GemVal gem_v_path = gem_table_get(_t4735, gem_string("path"));
#line 293 "compiler/main.gem"
    GemVal _t4736 = gem_v_string;
    GemVal _t4737[] = {gem_v_path, gem_string(".gem")};
    GemVal _t4738 = gem_table_get(_t4736, gem_string("ends_with"));
            if (gem_truthy(gem_not(_t4738.fn(_t4738.env, _t4737, 2)))) {
#line 294 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 297 "compiler/main.gem"
    GemVal _t4739[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t4739, 2);
#line 298 "compiler/main.gem"
    GemVal _t4740[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t4740, 1)))) {
#line 299 "compiler/main.gem"
    GemVal _t4741[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t4741, 2);
            }
#line 302 "compiler/main.gem"
    GemVal _t4742[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t4742, 1);
#line 304 "compiler/main.gem"
            GemVal gem_v_cached = gem_table_get(gem_v_loaded, gem_v_full_path);
#line 305 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_cached, GEM_NIL))) {
#line 306 "compiler/main.gem"
    GemVal _t4743 = gem_v_cached;
                if (gem_truthy(gem_neq(gem_table_get(_t4743, gem_string("exported")), GEM_NIL))) {
#line 307 "compiler/main.gem"
    GemVal _t4744 = gem_v_cached;
                    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_table_get(_t4744, gem_string("mod_name"))), gem_string("_"));
#line 308 "compiler/main.gem"
    GemVal _t4745 = gem_v_stmt;
                    if (gem_truthy(gem_neq(gem_table_get(_t4745, gem_string("selective")), GEM_NIL))) {
#line 309 "compiler/main.gem"
    GemVal _t4746 = gem_v_stmt;
                        GemVal gem_v__for_items_20 = gem_table_get(_t4746, gem_string("selective"));
#line 309 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(0);
#line 309 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4747[] = {gem_v__for_items_20};
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t4747, 1)))) break;
#line 309 "compiler/main.gem"
                            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_20, gem_v__for_i_20);
#line 309 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 310 "compiler/main.gem"
    GemVal _t4748[] = {gem_add(gem_v_prefix, gem_v_sel_name)};
    GemVal _t4749[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4748, 1), gem_int(0)};
                            gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4749, 3));
#line 311 "compiler/main.gem"
                            gem_v_count = gem_add(gem_v_count, gem_int(1));
                        }

                    } else {
#line 314 "compiler/main.gem"
    GemVal _t4750 = gem_v_cached;
                        GemVal gem_v_binding_name = gem_table_get(_t4750, gem_string("mod_name"));
#line 315 "compiler/main.gem"
    GemVal _t4751 = gem_v_stmt;
                        if (gem_truthy(gem_neq(gem_table_get(_t4751, gem_string("alias")), GEM_NIL))) {
#line 316 "compiler/main.gem"
    GemVal _t4752 = gem_v_stmt;
                            gem_v_binding_name = gem_table_get(_t4752, gem_string("alias"));
                        }
#line 318 "compiler/main.gem"
    GemVal _t4753 = gem_table_new();
                        GemVal gem_v_entries = _t4753;
#line 319 "compiler/main.gem"
    GemVal _t4754 = gem_v_cached;
                        GemVal gem_v__for_items_21 = gem_table_get(_t4754, gem_string("exported"));
#line 319 "compiler/main.gem"
                        GemVal gem_v__for_i_21 = gem_int(0);
#line 319 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4755[] = {gem_v__for_items_21};
                            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t4755, 1)))) break;
#line 319 "compiler/main.gem"
                            GemVal gem_v_ename = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 319 "compiler/main.gem"
                            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 320 "compiler/main.gem"
    GemVal _t4756[] = {gem_add(gem_v_prefix, gem_v_ename)};
    GemVal _t4757[] = {gem_v_ename, gem_fn_make_var(NULL, _t4756, 1)};
    GemVal _t4758[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4757, 2)};
                            (void)(gem_push_fn(NULL, _t4758, 2));
                        }

#line 322 "compiler/main.gem"
    GemVal _t4759[] = {gem_v_entries};
    GemVal _t4760[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4759, 1), gem_int(0)};
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4760, 3));
#line 323 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
                continue;
            }
#line 329 "compiler/main.gem"
    GemVal _t4761 = gem_table_new();
    gem_table_set(_t4761, gem_string("exported"), GEM_NIL);
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4761);
#line 331 "compiler/main.gem"
    GemVal _t4762[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t4762, 1);
#line 332 "compiler/main.gem"
    GemVal _t4763[] = {gem_v_source, gem_v_full_path};
    GemVal _t4764 = gem_v_parse_source;
            GemVal gem_v_loaded_ast = _t4764.fn(_t4764.env, _t4763, 2);
#line 333 "compiler/main.gem"
    GemVal _t4765[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t4765, 1);
#line 334 "compiler/main.gem"
    GemVal _t4766[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t4766, 3);
#line 336 "compiler/main.gem"
    GemVal _t4767 = gem_v_resolved;
    GemVal _t4768[] = {gem_table_get(_t4767, gem_string("stmts"))};
            GemVal gem_v_export_node = gem_fn_find_export_node(NULL, _t4768, 1);
#line 337 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_export_node, GEM_NIL))) {
#line 338 "compiler/main.gem"
    GemVal _t4769[] = {gem_add(gem_string("gem: loaded file has no export statement: "), gem_v_full_path)};
                (void)(gem_error_at_fn("compiler/main.gem", 338, _t4769, 1));
            }
#line 341 "compiler/main.gem"
    GemVal _t4770 = gem_v_stmt;
    GemVal _t4771[] = {gem_table_get(_t4770, gem_string("path"))};
            GemVal gem_v_mod_name = gem_fn_module_name_from_path(NULL, _t4771, 1);
#line 342 "compiler/main.gem"
    GemVal _t4772 = gem_table_new();
    gem_table_set(_t4772, gem_string("mod_name"), gem_v_mod_name);
    GemVal _t4773 = gem_v_export_node;
    gem_table_set(_t4772, gem_string("exported"), gem_table_get(_t4773, gem_string("names")));
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4772);
#line 343 "compiler/main.gem"
    GemVal _t4774 = gem_v_resolved;
    GemVal _t4775[] = {gem_table_get(_t4774, gem_string("stmts")), gem_v_mod_name, gem_v_export_node, gem_v_stmt};
            GemVal gem_v_mod_stmts = gem_fn_transform_module(NULL, _t4775, 4);
#line 344 "compiler/main.gem"
            GemVal gem_v__for_items_22 = gem_v_mod_stmts;
#line 344 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 344 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4776[] = {gem_v__for_items_22};
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t4776, 1)))) break;
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
    GemVal _t4777[] = {gem_v_new_stmts};
    GemVal _t4778 = gem_fn_make_program(NULL, _t4777, 1);
    gem_pop_frame();
    return _t4778;
}

static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc) {
#line 358 "compiler/main.gem"
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_args", "compiler/main.gem", 358);
#line 359 "compiler/main.gem"
    GemVal _t4779 = gem_table_new();
    gem_table_set(_t4779, gem_string("src_path"), GEM_NIL);
    gem_table_set(_t4779, gem_string("out_name"), GEM_NIL);
    gem_table_set(_t4779, gem_string("emit_c"), gem_bool(0));
    gem_table_set(_t4779, gem_string("run"), gem_bool(0));
    GemVal _t4780 = gem_table_new();
    gem_table_set(_t4779, gem_string("run_args"), _t4780);
    GemVal gem_v_result = _t4779;
#line 367 "compiler/main.gem"
    GemVal _t4781[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t4781, 1);
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
    GemVal _t4782 = gem_v_result;
            gem_table_set(gem_table_get(_t4782, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
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
    GemVal _t4783[] = {gem_string("gem: -o requires an argument")};
                            (void)(gem_error_at_fn("compiler/main.gem", 385, _t4783, 1));
                        }
#line 387 "compiler/main.gem"
                        gem_table_set(gem_v_result, gem_string("out_name"), gem_table_get(gem_v_args, gem_v_i));
                    } else {
#line 388 "compiler/main.gem"
    GemVal _t4784 = gem_v_result;
                        if (gem_truthy(gem_eq(gem_table_get(_t4784, gem_string("src_path")), GEM_NIL))) {
#line 389 "compiler/main.gem"
                            gem_table_set(gem_v_result, gem_string("src_path"), gem_v_arg);
                        } else {
#line 392 "compiler/main.gem"
    GemVal _t4785 = gem_v_result;
                            gem_table_set(gem_table_get(_t4785, gem_string("run_args")), gem_v_run_arg_count, gem_v_arg);
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
    GemVal _t4786 = gem_v_result;
    gem_pop_frame();
    return _t4786;
}

static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc) {
#line 403 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("basename_no_ext", "compiler/main.gem", 403);
#line 405 "compiler/main.gem"
    GemVal _t4787[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4787, 1);
#line 406 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 407 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 408 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 409 "compiler/main.gem"
    GemVal _t4788[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4788, 2), gem_int(47)))) {
#line 410 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 412 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 414 "compiler/main.gem"
    GemVal _t4789[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4789, 3);
#line 416 "compiler/main.gem"
    GemVal _t4790[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4790, 1);
#line 417 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 418 "compiler/main.gem"
    GemVal _t4791[] = {gem_v_name, gem_sub(gem_v_nlen, gem_int(4)), gem_int(4)};
        GemVal gem_v_maybe_ext = gem_substr_fn(NULL, _t4791, 3);
#line 419 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_maybe_ext, gem_string(".gem")))) {
#line 420 "compiler/main.gem"
    GemVal _t4792[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4792, 3);
        }
    }
    GemVal _t4793 = gem_v_name;
    gem_pop_frame();
    return _t4793;
}

int main(int argc, char **argv) {
    GC_INIT();
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_push_frame("main", "compiler/main.gem", 0);
#line 0 "compiler/main.gem"
    GemVal _t4794 = gem_table_new();
    gem_table_set(_t4794, gem_string("split"), gem_make_fn(gem_fn__mod_string_split, NULL));
    gem_table_set(_t4794, gem_string("index_of"), gem_make_fn(gem_fn__mod_string_index_of, NULL));
    gem_table_set(_t4794, gem_string("join"), gem_make_fn(gem_fn__mod_string_join, NULL));
    gem_table_set(_t4794, gem_string("trim"), gem_make_fn(gem_fn__mod_string_trim, NULL));
    gem_table_set(_t4794, gem_string("starts_with"), gem_make_fn(gem_fn__mod_string_starts_with, NULL));
    gem_table_set(_t4794, gem_string("ends_with"), gem_make_fn(gem_fn__mod_string_ends_with, NULL));
    gem_table_set(_t4794, gem_string("upper"), gem_make_fn(gem_fn__mod_string_upper, NULL));
    gem_table_set(_t4794, gem_string("lower"), gem_make_fn(gem_fn__mod_string_lower, NULL));
    gem_table_set(_t4794, gem_string("contains"), gem_make_fn(gem_fn__mod_string_contains, NULL));
    gem_table_set(_t4794, gem_string("repeat"), gem_make_fn(gem_fn__mod_string_repeat, NULL));
    gem_v_string = _t4794;
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
    GemVal _t4795[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t4796[] = {gem_dirname_fn(NULL, _t4795, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t4796, 1);
#line 428 "compiler/main.gem"
    GemVal _t4797[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t4797, 1);
#line 429 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 430 "compiler/main.gem"
    GemVal _t4798[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 430, _t4798, 1));
    }
#line 433 "compiler/main.gem"
    GemVal _t4799[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_cli = gem_fn_parse_args(NULL, _t4799, 1);
#line 435 "compiler/main.gem"
    GemVal _t4800 = gem_v_cli;
    if (gem_truthy(gem_eq(gem_table_get(_t4800, gem_string("src_path")), GEM_NIL))) {
#line 436 "compiler/main.gem"
    GemVal _t4801[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 436, _t4801, 1));
    }
#line 439 "compiler/main.gem"
    GemVal _t4802 = gem_v_cli;
    gem_v_src_path = gem_table_get(_t4802, gem_string("src_path"));
#line 440 "compiler/main.gem"
    GemVal _t4803[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t4803, 1);
#line 442 "compiler/main.gem"
    GemVal _t4804[] = {gem_v_source, gem_v_src_path};
    GemVal _t4805 = gem_v_parse_source;
    gem_v_ast = _t4805.fn(_t4805.env, _t4804, 2);
#line 443 "compiler/main.gem"
    GemVal _t4806[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t4806, 1);
#line 444 "compiler/main.gem"
    GemVal _t4807 = gem_table_new();
    gem_v_loaded = _t4807;
#line 445 "compiler/main.gem"
    GemVal _t4808[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t4808, 3);
#line 447 "compiler/main.gem"
    GemVal _t4809[] = {gem_v_src_path};
    GemVal _t4810 = gem_v_make_codegen;
    gem_v_cg = _t4810.fn(_t4810.env, _t4809, 1);
#line 448 "compiler/main.gem"
    GemVal _t4811 = gem_v_cg;
    GemVal _t4812[] = {gem_v_resolved_ast};
    GemVal _t4813 = gem_table_get(_t4811, gem_string("compile"));
    gem_v_c_code = _t4813.fn(_t4813.env, _t4812, 1);
#line 452 "compiler/main.gem"
    GemVal _t4814 = gem_v_cli;
    if (gem_truthy(gem_table_get(_t4814, gem_string("emit_c")))) {
#line 453 "compiler/main.gem"
    GemVal _t4815[] = {gem_v_c_code};
        (void)(gem_print(NULL, _t4815, 1));
    } else {
#line 457 "compiler/main.gem"
    GemVal _t4816[] = {gem_v_src_path};
        GemVal gem_v_base = gem_fn_basename_no_ext(NULL, _t4816, 1);
#line 458 "compiler/main.gem"
        GemVal gem_v_tmp_c = gem_add(gem_add(gem_string("/tmp/gem_"), gem_v_base), gem_string(".c"));
#line 461 "compiler/main.gem"
    GemVal _t4817 = gem_v_cli;
        GemVal gem_v_out_name = gem_table_get(_t4817, gem_string("out_name"));
#line 462 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_out_name, GEM_NIL))) {
#line 463 "compiler/main.gem"
            gem_v_out_name = gem_add(gem_string("./"), gem_v_base);
        }
#line 467 "compiler/main.gem"
    GemVal _t4818[] = {gem_v_tmp_c, gem_v_c_code};
        (void)(gem_write_file_fn(NULL, _t4818, 2));
#line 470 "compiler/main.gem"
    GemVal _t4819[] = {gem_v_install_root, gem_string("runtime")};
        GemVal gem_v_runtime_dir = gem_path_join_fn(NULL, _t4819, 2);
#line 473 "compiler/main.gem"
        GemVal gem_v_gc_flags = gem_string("-lgc");
#line 474 "compiler/main.gem"
    GemVal _t4820[] = {gem_string("pkg-config --cflags --libs bdw-gc > /tmp/gem_gc_flags.txt 2>/dev/null")};
        GemVal gem_v_pkg_ret = gem_exec_fn(NULL, _t4820, 1);
#line 475 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_pkg_ret, gem_int(0)))) {
#line 476 "compiler/main.gem"
    GemVal _t4821[] = {gem_string("/tmp/gem_gc_flags.txt")};
            gem_v_gc_flags = gem_read_file_fn(NULL, _t4821, 1);
#line 478 "compiler/main.gem"
    GemVal _t4822[] = {gem_v_gc_flags, gem_string("\n"), gem_string("")};
            gem_v_gc_flags = gem_str_replace_fn(NULL, _t4822, 3);
        }
#line 481 "compiler/main.gem"
        GemVal gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" ")), gem_v_runtime_dir), gem_string("/gem_*.c -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread"));
#line 483 "compiler/main.gem"
    GemVal _t4823[] = {gem_v_cc_cmd};
        GemVal gem_v_cc_ret = gem_exec_fn(NULL, _t4823, 1);
#line 484 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_cc_ret, gem_int(0)))) {
#line 485 "compiler/main.gem"
    GemVal _t4824[] = {gem_v_cc_ret};
    GemVal _t4825[] = {gem_add(gem_add(gem_string("gem: compilation failed (cc exited with "), gem_to_string_fn(NULL, _t4824, 1)), gem_string(")"))};
            (void)(gem_error_at_fn("compiler/main.gem", 485, _t4825, 1));
        }
#line 490 "compiler/main.gem"
    GemVal _t4826 = gem_v_cli;
        if (gem_truthy(gem_table_get(_t4826, gem_string("run")))) {
#line 491 "compiler/main.gem"
            GemVal gem_v_run_cmd = gem_v_out_name;
#line 492 "compiler/main.gem"
            GemVal gem_v_i = gem_int(0);
#line 493 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4827 = gem_v_cli;
                GemVal _t4828[] = {gem_table_get(_t4827, gem_string("run_args"))};
                if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t4828, 1)))) break;
#line 494 "compiler/main.gem"
    GemVal _t4829 = gem_v_cli;
                gem_v_run_cmd = gem_add(gem_add(gem_v_run_cmd, gem_string(" ")), gem_table_get(gem_table_get(_t4829, gem_string("run_args")), gem_v_i));
#line 495 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 497 "compiler/main.gem"
    GemVal _t4830[] = {gem_v_run_cmd};
            GemVal gem_v_run_ret = gem_exec_fn(NULL, _t4830, 1);
#line 498 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_run_ret, gem_int(0)))) {
#line 499 "compiler/main.gem"
    GemVal _t4831[] = {gem_v_run_ret};
                (void)(gem_exit_process_fn(NULL, _t4831, 1));
            }
        }
    }
    gem_run_scheduler();
    gem_pop_frame();
    return 0;
}

