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

struct _closure__anon_1 {
    intptr_t _num_captures;
    GemVal *gem_v_arr;
    GemVal *gem_v_delim;
};
static GemVal _anon_1(void *_env, GemVal *args, int argc) {
    struct _closure__anon_1 *_cls = (struct _closure__anon_1 *)_env;
    GemVal *gem_v_arr = _cls->gem_v_arr;
    GemVal *gem_v_delim = _cls->gem_v_delim;
    GemVal gem_v_add = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_1", "compiler/main.gem", 0);
#line 64 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 65 "compiler/main.gem"
    GemVal _t29[] = {(*gem_v_arr)};
    GemVal gem_v_n = gem_len_fn(NULL, _t29, 1);
#line 66 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_n))) break;
#line 67 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 68 "compiler/main.gem"
    GemVal _t30[] = {(*gem_v_delim)};
    GemVal _t31 = gem_v_add;
            (void)(_t31.fn(_t31.env, _t30, 1));
        }
#line 70 "compiler/main.gem"
    GemVal _t32[] = {gem_table_get((*gem_v_arr), gem_v_i)};
    GemVal _t33 = gem_v_add;
        (void)(_t33.fn(_t33.env, _t32, 1));
#line 71 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    gem_pop_frame();
    return GEM_NIL;
}

static GemVal gem_fn__mod_string_join(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal *gem_v_arr = GC_MALLOC(sizeof(GemVal));
    *gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_delim = GC_MALLOC(sizeof(GemVal));
    *gem_v_delim = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_join", "compiler/main.gem", 62);
#line 63 "compiler/main.gem"
    struct _closure__anon_1 *_t34 = GC_MALLOC(sizeof(struct _closure__anon_1));
    _t34->_num_captures = 2;
    _t34->gem_v_arr = gem_v_arr;
    _t34->gem_v_delim = gem_v_delim;
    GemVal _t35[] = {gem_make_fn(_anon_1, _t34)};
    GemVal _t36 = gem_build_string_fn(NULL, _t35, 1);
    gem_pop_frame();
    return _t36;
}

static GemVal gem_fn__mod_string_trim(void *_env, GemVal *args, int argc) {
#line 76 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_string_trim", "compiler/main.gem", 76);
#line 77 "compiler/main.gem"
    GemVal _t37[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t37, 1);
#line 78 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_slen, gem_int(0)))) {
#line 79 "compiler/main.gem"
        GemVal _t38 = gem_v_s;
        gem_pop_frame();
        return _t38;
    }
#line 81 "compiler/main.gem"
    GemVal gem_v_start = gem_int(0);
#line 82 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_start, gem_v_slen))) break;
#line 83 "compiler/main.gem"
    GemVal _t39[] = {gem_v_s, gem_v_start};
        GemVal gem_v_c = gem_ord_fn(NULL, _t39, 2);
#line 84 "compiler/main.gem"
    GemVal _t40;
    if (!gem_truthy(gem_neq(gem_v_c, gem_int(32)))) {
        _t40 = gem_neq(gem_v_c, gem_int(32));
    } else {
        _t40 = gem_neq(gem_v_c, gem_int(9));
    }
    GemVal _t41;
    if (!gem_truthy(_t40)) {
        _t41 = _t40;
    } else {
        _t41 = gem_neq(gem_v_c, gem_int(10));
    }
    GemVal _t42;
    if (!gem_truthy(_t41)) {
        _t42 = _t41;
    } else {
        _t42 = gem_neq(gem_v_c, gem_int(13));
    }
        if (gem_truthy(_t42)) {
            break;
        }
#line 87 "compiler/main.gem"
        gem_v_start = gem_add(gem_v_start, gem_int(1));
    }
#line 89 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_start, gem_v_slen))) {
#line 90 "compiler/main.gem"
        GemVal _t43 = gem_string("");
        gem_pop_frame();
        return _t43;
    }
#line 92 "compiler/main.gem"
    GemVal gem_v_stop = gem_sub(gem_v_slen, gem_int(1));
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_stop, gem_v_start))) break;
#line 94 "compiler/main.gem"
    GemVal _t44[] = {gem_v_s, gem_v_stop};
        GemVal gem_v_c = gem_ord_fn(NULL, _t44, 2);
#line 95 "compiler/main.gem"
    GemVal _t45;
    if (!gem_truthy(gem_neq(gem_v_c, gem_int(32)))) {
        _t45 = gem_neq(gem_v_c, gem_int(32));
    } else {
        _t45 = gem_neq(gem_v_c, gem_int(9));
    }
    GemVal _t46;
    if (!gem_truthy(_t45)) {
        _t46 = _t45;
    } else {
        _t46 = gem_neq(gem_v_c, gem_int(10));
    }
    GemVal _t47;
    if (!gem_truthy(_t46)) {
        _t47 = _t46;
    } else {
        _t47 = gem_neq(gem_v_c, gem_int(13));
    }
        if (gem_truthy(_t47)) {
            break;
        }
#line 98 "compiler/main.gem"
        gem_v_stop = gem_sub(gem_v_stop, gem_int(1));
    }
#line 100 "compiler/main.gem"
    GemVal _t48[] = {gem_v_s, gem_v_start, gem_add(gem_sub(gem_v_stop, gem_v_start), gem_int(1))};
    GemVal _t49 = gem_substr_fn(NULL, _t48, 3);
    gem_pop_frame();
    return _t49;
}

static GemVal gem_fn__mod_string_starts_with(void *_env, GemVal *args, int argc) {
#line 103 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_starts_with", "compiler/main.gem", 103);
#line 104 "compiler/main.gem"
    GemVal _t50[] = {gem_v_s, gem_int(0), gem_v_prefix};
    GemVal _t51 = gem_fn__mod_string_str_eq_at(NULL, _t50, 3);
    gem_pop_frame();
    return _t51;
}

static GemVal gem_fn__mod_string_ends_with(void *_env, GemVal *args, int argc) {
#line 107 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_suffix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_ends_with", "compiler/main.gem", 107);
#line 108 "compiler/main.gem"
    GemVal _t52[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t52, 1);
#line 109 "compiler/main.gem"
    GemVal _t53[] = {gem_v_suffix};
    GemVal gem_v_plen = gem_len_fn(NULL, _t53, 1);
#line 110 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_plen, gem_v_slen))) {
#line 111 "compiler/main.gem"
        GemVal _t54 = gem_bool(0);
        gem_pop_frame();
        return _t54;
    }
#line 113 "compiler/main.gem"
    GemVal _t55[] = {gem_v_s, gem_sub(gem_v_slen, gem_v_plen), gem_v_suffix};
    GemVal _t56 = gem_fn__mod_string_str_eq_at(NULL, _t55, 3);
    gem_pop_frame();
    return _t56;
}

struct _closure__anon_2 {
    intptr_t _num_captures;
    GemVal *gem_v_s;
};
static GemVal _anon_2(void *_env, GemVal *args, int argc) {
    struct _closure__anon_2 *_cls = (struct _closure__anon_2 *)_env;
    GemVal *gem_v_s = _cls->gem_v_s;
    GemVal gem_v_add = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_2", "compiler/main.gem", 0);
    {
#line 118 "compiler/main.gem"
        GemVal gem_v__for_i_1 = gem_int(0);
#line 118 "compiler/main.gem"
    GemVal _t57[] = {(*gem_v_s)};
        GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t57, 1);
#line 118 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_v__for_limit_1))) break;
#line 118 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_1;
#line 118 "compiler/main.gem"
            gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 119 "compiler/main.gem"
    GemVal _t58[] = {(*gem_v_s), gem_v_i};
            GemVal gem_v_c = gem_ord_fn(NULL, _t58, 2);
#line 120 "compiler/main.gem"
    GemVal _t59;
    if (!gem_truthy(gem_ge(gem_v_c, gem_int(97)))) {
        _t59 = gem_ge(gem_v_c, gem_int(97));
    } else {
        _t59 = gem_le(gem_v_c, gem_int(122));
    }
            if (gem_truthy(_t59)) {
#line 121 "compiler/main.gem"
    GemVal _t60[] = {gem_sub(gem_v_c, gem_int(32))};
    GemVal _t61[] = {gem_chr_fn(NULL, _t60, 1)};
    GemVal _t62 = gem_v_add;
                (void)(_t62.fn(_t62.env, _t61, 1));
            } else {
#line 123 "compiler/main.gem"
    GemVal _t63[] = {gem_v_c};
    GemVal _t64[] = {gem_chr_fn(NULL, _t63, 1)};
    GemVal _t65 = gem_v_add;
                (void)(_t65.fn(_t65.env, _t64, 1));
            }
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn__mod_string_upper(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal *gem_v_s = GC_MALLOC(sizeof(GemVal));
    *gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_string_upper", "compiler/main.gem", 116);
#line 117 "compiler/main.gem"
    struct _closure__anon_2 *_t66 = GC_MALLOC(sizeof(struct _closure__anon_2));
    _t66->_num_captures = 1;
    _t66->gem_v_s = gem_v_s;
    GemVal _t67[] = {gem_make_fn(_anon_2, _t66)};
    GemVal _t68 = gem_build_string_fn(NULL, _t67, 1);
    gem_pop_frame();
    return _t68;
}

struct _closure__anon_3 {
    intptr_t _num_captures;
    GemVal *gem_v_s;
};
static GemVal _anon_3(void *_env, GemVal *args, int argc) {
    struct _closure__anon_3 *_cls = (struct _closure__anon_3 *)_env;
    GemVal *gem_v_s = _cls->gem_v_s;
    GemVal gem_v_add = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_3", "compiler/main.gem", 0);
    {
#line 131 "compiler/main.gem"
        GemVal gem_v__for_i_2 = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t69[] = {(*gem_v_s)};
        GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t69, 1);
#line 131 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 131 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_2;
#line 131 "compiler/main.gem"
            gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 132 "compiler/main.gem"
    GemVal _t70[] = {(*gem_v_s), gem_v_i};
            GemVal gem_v_c = gem_ord_fn(NULL, _t70, 2);
#line 133 "compiler/main.gem"
    GemVal _t71;
    if (!gem_truthy(gem_ge(gem_v_c, gem_int(65)))) {
        _t71 = gem_ge(gem_v_c, gem_int(65));
    } else {
        _t71 = gem_le(gem_v_c, gem_int(90));
    }
            if (gem_truthy(_t71)) {
#line 134 "compiler/main.gem"
    GemVal _t72[] = {gem_add(gem_v_c, gem_int(32))};
    GemVal _t73[] = {gem_chr_fn(NULL, _t72, 1)};
    GemVal _t74 = gem_v_add;
                (void)(_t74.fn(_t74.env, _t73, 1));
            } else {
#line 136 "compiler/main.gem"
    GemVal _t75[] = {gem_v_c};
    GemVal _t76[] = {gem_chr_fn(NULL, _t75, 1)};
    GemVal _t77 = gem_v_add;
                (void)(_t77.fn(_t77.env, _t76, 1));
            }
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn__mod_string_lower(void *_env, GemVal *args, int argc) {
#line 129 "compiler/main.gem"
    GemVal *gem_v_s = GC_MALLOC(sizeof(GemVal));
    *gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_string_lower", "compiler/main.gem", 129);
#line 130 "compiler/main.gem"
    struct _closure__anon_3 *_t78 = GC_MALLOC(sizeof(struct _closure__anon_3));
    _t78->_num_captures = 1;
    _t78->gem_v_s = gem_v_s;
    GemVal _t79[] = {gem_make_fn(_anon_3, _t78)};
    GemVal _t80 = gem_build_string_fn(NULL, _t79, 1);
    gem_pop_frame();
    return _t80;
}

static GemVal gem_fn__mod_string_contains(void *_env, GemVal *args, int argc) {
#line 142 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_needle = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_contains", "compiler/main.gem", 142);
    GemVal _t81[] = {gem_v_s, gem_v_needle};
    GemVal _t82 = gem_ge(gem_fn__mod_string_index_of(NULL, _t81, 2), gem_int(0));
    gem_pop_frame();
    return _t82;
}

struct _closure__anon_4 {
    intptr_t _num_captures;
    GemVal *gem_v_n;
    GemVal *gem_v_s;
};
static GemVal _anon_4(void *_env, GemVal *args, int argc) {
    struct _closure__anon_4 *_cls = (struct _closure__anon_4 *)_env;
    GemVal *gem_v_n = _cls->gem_v_n;
    GemVal *gem_v_s = _cls->gem_v_s;
    GemVal gem_v_add = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_4", "compiler/main.gem", 0);
    {
#line 148 "compiler/main.gem"
        GemVal gem_v__for_i_3 = gem_int(0);
#line 148 "compiler/main.gem"
        GemVal gem_v__for_limit_3 = (*gem_v_n);
#line 148 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_limit_3))) break;
#line 148 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_3;
#line 148 "compiler/main.gem"
            gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 149 "compiler/main.gem"
    GemVal _t83[] = {(*gem_v_s)};
    GemVal _t84 = gem_v_add;
            (void)(_t84.fn(_t84.env, _t83, 1));
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

static GemVal gem_fn__mod_string_repeat(void *_env, GemVal *args, int argc) {
#line 146 "compiler/main.gem"
    GemVal *gem_v_s = GC_MALLOC(sizeof(GemVal));
    *gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    GemVal *gem_v_n = GC_MALLOC(sizeof(GemVal));
    *gem_v_n = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_string_repeat", "compiler/main.gem", 146);
#line 147 "compiler/main.gem"
    struct _closure__anon_4 *_t85 = GC_MALLOC(sizeof(struct _closure__anon_4));
    _t85->_num_captures = 2;
    _t85->gem_v_n = gem_v_n;
    _t85->gem_v_s = gem_v_s;
    GemVal _t86[] = {gem_make_fn(_anon_4, _t85)};
    GemVal _t87 = gem_build_string_fn(NULL, _t86, 1);
    gem_pop_frame();
    return _t87;
}

static GemVal gem_fn__mod_ast_make_program(void *_env, GemVal *args, int argc) {
#line 8 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_program", "compiler/main.gem", 8);
    GemVal _t88 = gem_table_new();
    gem_table_set(_t88, gem_string("tag"), gem_string("program"));
    gem_table_set(_t88, gem_string("stmts"), gem_v_stmts);
    GemVal _t89 = _t88;
    gem_pop_frame();
    return _t89;
}

static GemVal gem_fn__mod_ast_make_int(void *_env, GemVal *args, int argc) {
#line 14 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_int", "compiler/main.gem", 14);
    GemVal _t90 = gem_table_new();
    gem_table_set(_t90, gem_string("tag"), gem_string("int"));
    gem_table_set(_t90, gem_string("value"), gem_v_value);
    GemVal _t91 = _t90;
    gem_pop_frame();
    return _t91;
}

static GemVal gem_fn__mod_ast_make_float(void *_env, GemVal *args, int argc) {
#line 18 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_float", "compiler/main.gem", 18);
    GemVal _t92 = gem_table_new();
    gem_table_set(_t92, gem_string("tag"), gem_string("float"));
    gem_table_set(_t92, gem_string("value"), gem_v_value);
    GemVal _t93 = _t92;
    gem_pop_frame();
    return _t93;
}

static GemVal gem_fn__mod_ast_make_string(void *_env, GemVal *args, int argc) {
#line 22 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_string", "compiler/main.gem", 22);
    GemVal _t94 = gem_table_new();
    gem_table_set(_t94, gem_string("tag"), gem_string("string"));
    gem_table_set(_t94, gem_string("value"), gem_v_value);
    GemVal _t95 = _t94;
    gem_pop_frame();
    return _t95;
}

static GemVal gem_fn__mod_ast_make_bool(void *_env, GemVal *args, int argc) {
#line 26 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_bool", "compiler/main.gem", 26);
    GemVal _t96 = gem_table_new();
    gem_table_set(_t96, gem_string("tag"), gem_string("bool"));
    gem_table_set(_t96, gem_string("value"), gem_v_value);
    GemVal _t97 = _t96;
    gem_pop_frame();
    return _t97;
}

static GemVal gem_fn__mod_ast_make_nil_node(void *_env, GemVal *args, int argc) {
#line 30 "compiler/main.gem"
    gem_push_frame("_mod_ast_make_nil_node", "compiler/main.gem", 30);
    GemVal _t98 = gem_table_new();
    gem_table_set(_t98, gem_string("tag"), gem_string("nil"));
    GemVal _t99 = _t98;
    gem_pop_frame();
    return _t99;
}

static GemVal gem_fn__mod_ast_make_var(void *_env, GemVal *args, int argc) {
#line 34 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_var", "compiler/main.gem", 34);
    GemVal _t100 = gem_table_new();
    gem_table_set(_t100, gem_string("tag"), gem_string("var"));
    gem_table_set(_t100, gem_string("name"), gem_v_name);
    GemVal _t101 = _t100;
    gem_pop_frame();
    return _t101;
}

static GemVal gem_fn__mod_ast_make_binop(void *_env, GemVal *args, int argc) {
#line 38 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_left = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_right = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_binop", "compiler/main.gem", 38);
    GemVal _t102 = gem_table_new();
    gem_table_set(_t102, gem_string("tag"), gem_string("binop"));
    gem_table_set(_t102, gem_string("op"), gem_v_op);
    gem_table_set(_t102, gem_string("left"), gem_v_left);
    gem_table_set(_t102, gem_string("right"), gem_v_right);
    GemVal _t103 = _t102;
    gem_pop_frame();
    return _t103;
}

static GemVal gem_fn__mod_ast_make_unop(void *_env, GemVal *args, int argc) {
#line 42 "compiler/main.gem"
    GemVal gem_v_op = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_expr = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_unop", "compiler/main.gem", 42);
    GemVal _t104 = gem_table_new();
    gem_table_set(_t104, gem_string("tag"), gem_string("unop"));
    gem_table_set(_t104, gem_string("op"), gem_v_op);
    gem_table_set(_t104, gem_string("expr"), gem_v_expr);
    GemVal _t105 = _t104;
    gem_pop_frame();
    return _t105;
}

static GemVal gem_fn__mod_ast_make_call(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_func = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_args = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_call", "compiler/main.gem", 46);
    GemVal _t106 = gem_table_new();
    gem_table_set(_t106, gem_string("tag"), gem_string("call"));
    gem_table_set(_t106, gem_string("func"), gem_v_func);
    gem_table_set(_t106, gem_string("args"), gem_v_args);
    gem_table_set(_t106, gem_string("line"), gem_v_line);
    GemVal _t107 = _t106;
    gem_pop_frame();
    return _t107;
}

static GemVal gem_fn__mod_ast_make_dot(void *_env, GemVal *args, int argc) {
#line 50 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_dot", "compiler/main.gem", 50);
    GemVal _t108 = gem_table_new();
    gem_table_set(_t108, gem_string("tag"), gem_string("dot"));
    gem_table_set(_t108, gem_string("object"), gem_v_object);
    gem_table_set(_t108, gem_string("field"), gem_v_field);
    GemVal _t109 = _t108;
    gem_pop_frame();
    return _t109;
}

static GemVal gem_fn__mod_ast_make_index(void *_env, GemVal *args, int argc) {
#line 54 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_index", "compiler/main.gem", 54);
    GemVal _t110 = gem_table_new();
    gem_table_set(_t110, gem_string("tag"), gem_string("index"));
    gem_table_set(_t110, gem_string("object"), gem_v_object);
    gem_table_set(_t110, gem_string("key"), gem_v_key);
    GemVal _t111 = _t110;
    gem_pop_frame();
    return _t111;
}

static GemVal gem_fn__mod_ast_make_anon_fn(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_params = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_rest_param = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_defaults = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_anon_fn", "compiler/main.gem", 58);
    GemVal _t112 = gem_table_new();
    gem_table_set(_t112, gem_string("tag"), gem_string("anon_fn"));
    gem_table_set(_t112, gem_string("params"), gem_v_params);
    gem_table_set(_t112, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t112, gem_string("defaults"), gem_v_defaults);
    gem_table_set(_t112, gem_string("body"), gem_v_body);
    GemVal _t113 = _t112;
    gem_pop_frame();
    return _t113;
}

static GemVal gem_fn__mod_ast_make_table(void *_env, GemVal *args, int argc) {
#line 62 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_table", "compiler/main.gem", 62);
    GemVal _t114 = gem_table_new();
    gem_table_set(_t114, gem_string("tag"), gem_string("table"));
    gem_table_set(_t114, gem_string("entries"), gem_v_entries);
    GemVal _t115 = _t114;
    gem_pop_frame();
    return _t115;
}

static GemVal gem_fn__mod_ast_make_table_entry(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_table_entry", "compiler/main.gem", 66);
    GemVal _t116 = gem_table_new();
    gem_table_set(_t116, gem_string("key"), gem_v_key);
    gem_table_set(_t116, gem_string("value"), gem_v_value);
    GemVal _t117 = _t116;
    gem_pop_frame();
    return _t117;
}

static GemVal gem_fn__mod_ast_make_array(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_elements = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_array", "compiler/main.gem", 70);
    GemVal _t118 = gem_table_new();
    gem_table_set(_t118, gem_string("tag"), gem_string("array"));
    gem_table_set(_t118, gem_string("elements"), gem_v_elements);
    GemVal _t119 = _t118;
    gem_pop_frame();
    return _t119;
}

static GemVal gem_fn__mod_ast_make_interp(void *_env, GemVal *args, int argc) {
#line 74 "compiler/main.gem"
    GemVal gem_v_parts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_interp", "compiler/main.gem", 74);
    GemVal _t120 = gem_table_new();
    gem_table_set(_t120, gem_string("tag"), gem_string("interp"));
    gem_table_set(_t120, gem_string("parts"), gem_v_parts);
    GemVal _t121 = _t120;
    gem_pop_frame();
    return _t121;
}

static GemVal gem_fn__mod_ast_make_let(void *_env, GemVal *args, int argc) {
#line 80 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_let", "compiler/main.gem", 80);
    GemVal _t122 = gem_table_new();
    gem_table_set(_t122, gem_string("tag"), gem_string("let"));
    gem_table_set(_t122, gem_string("name"), gem_v_name);
    gem_table_set(_t122, gem_string("value"), gem_v_value);
    gem_table_set(_t122, gem_string("line"), gem_v_line);
    GemVal _t123 = _t122;
    gem_pop_frame();
    return _t123;
}

static GemVal gem_fn__mod_ast_make_assign(void *_env, GemVal *args, int argc) {
#line 84 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_assign", "compiler/main.gem", 84);
    GemVal _t124 = gem_table_new();
    gem_table_set(_t124, gem_string("tag"), gem_string("assign"));
    gem_table_set(_t124, gem_string("name"), gem_v_name);
    gem_table_set(_t124, gem_string("value"), gem_v_value);
    gem_table_set(_t124, gem_string("line"), gem_v_line);
    GemVal _t125 = _t124;
    gem_pop_frame();
    return _t125;
}

static GemVal gem_fn__mod_ast_make_dot_assign(void *_env, GemVal *args, int argc) {
#line 88 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_field = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_dot_assign", "compiler/main.gem", 88);
    GemVal _t126 = gem_table_new();
    gem_table_set(_t126, gem_string("tag"), gem_string("dot_assign"));
    gem_table_set(_t126, gem_string("object"), gem_v_object);
    gem_table_set(_t126, gem_string("field"), gem_v_field);
    gem_table_set(_t126, gem_string("value"), gem_v_value);
    gem_table_set(_t126, gem_string("line"), gem_v_line);
    GemVal _t127 = _t126;
    gem_pop_frame();
    return _t127;
}

static GemVal gem_fn__mod_ast_make_index_assign(void *_env, GemVal *args, int argc) {
#line 92 "compiler/main.gem"
    GemVal gem_v_object = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_key = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_value = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_index_assign", "compiler/main.gem", 92);
    GemVal _t128 = gem_table_new();
    gem_table_set(_t128, gem_string("tag"), gem_string("index_assign"));
    gem_table_set(_t128, gem_string("object"), gem_v_object);
    gem_table_set(_t128, gem_string("key"), gem_v_key);
    gem_table_set(_t128, gem_string("value"), gem_v_value);
    gem_table_set(_t128, gem_string("line"), gem_v_line);
    GemVal _t129 = _t128;
    gem_pop_frame();
    return _t129;
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
    GemVal _t130 = gem_table_new();
    gem_table_set(_t130, gem_string("tag"), gem_string("fn_def"));
    gem_table_set(_t130, gem_string("name"), gem_v_name);
    gem_table_set(_t130, gem_string("params"), gem_v_params);
    gem_table_set(_t130, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t130, gem_string("defaults"), gem_v_defaults);
    gem_table_set(_t130, gem_string("body"), gem_v_body);
    gem_table_set(_t130, gem_string("line"), gem_v_line);
    GemVal _t131 = _t130;
    gem_pop_frame();
    return _t131;
}

static GemVal gem_fn__mod_ast_make_if(void *_env, GemVal *args, int argc) {
#line 100 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_then_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_if", "compiler/main.gem", 100);
    GemVal _t132 = gem_table_new();
    gem_table_set(_t132, gem_string("tag"), gem_string("if"));
    gem_table_set(_t132, gem_string("cond"), gem_v_cond);
    gem_table_set(_t132, gem_string("then"), gem_v_then_body);
    gem_table_set(_t132, gem_string("else"), gem_v_else_body);
    gem_table_set(_t132, gem_string("line"), gem_v_line);
    GemVal _t133 = _t132;
    gem_pop_frame();
    return _t133;
}

static GemVal gem_fn__mod_ast_make_while(void *_env, GemVal *args, int argc) {
#line 104 "compiler/main.gem"
    GemVal gem_v_cond = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_while", "compiler/main.gem", 104);
    GemVal _t134 = gem_table_new();
    gem_table_set(_t134, gem_string("tag"), gem_string("while"));
    gem_table_set(_t134, gem_string("cond"), gem_v_cond);
    gem_table_set(_t134, gem_string("body"), gem_v_body);
    gem_table_set(_t134, gem_string("line"), gem_v_line);
    GemVal _t135 = _t134;
    gem_pop_frame();
    return _t135;
}

static GemVal gem_fn__mod_ast_make_match(void *_env, GemVal *args, int argc) {
#line 108 "compiler/main.gem"
    GemVal gem_v_target = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_whens = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_else_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_target_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("_mod_ast_make_match", "compiler/main.gem", 108);
    GemVal _t136 = gem_table_new();
    gem_table_set(_t136, gem_string("tag"), gem_string("match"));
    gem_table_set(_t136, gem_string("target"), gem_v_target);
    gem_table_set(_t136, gem_string("whens"), gem_v_whens);
    gem_table_set(_t136, gem_string("else"), gem_v_else_body);
    gem_table_set(_t136, gem_string("line"), gem_v_line);
    gem_table_set(_t136, gem_string("target_var"), gem_v_target_var);
    GemVal _t137 = _t136;
    gem_pop_frame();
    return _t137;
}

static GemVal gem_fn__mod_ast_make_when(void *_env, GemVal *args, int argc) {
#line 112 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_body = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_bindings = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_mod_ast_make_when", "compiler/main.gem", 112);
    GemVal _t138 = gem_table_new();
    gem_table_set(_t138, gem_string("tag"), gem_string("when_clause"));
    gem_table_set(_t138, gem_string("value"), gem_v_value);
    gem_table_set(_t138, gem_string("body"), gem_v_body);
    gem_table_set(_t138, gem_string("bindings"), gem_v_bindings);
    GemVal _t139 = _t138;
    gem_pop_frame();
    return _t139;
}

static GemVal gem_fn__mod_ast_make_receive_match(void *_env, GemVal *args, int argc) {
#line 116 "compiler/main.gem"
    GemVal gem_v_arms = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_after_ms = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_after_body = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_line = (argc > 3) ? args[3] : GEM_NIL;
    GemVal gem_v_recv_var = (argc > 4) ? args[4] : GEM_NIL;
    gem_push_frame("_mod_ast_make_receive_match", "compiler/main.gem", 116);
    GemVal _t140 = gem_table_new();
    gem_table_set(_t140, gem_string("tag"), gem_string("receive_match"));
    gem_table_set(_t140, gem_string("arms"), gem_v_arms);
    gem_table_set(_t140, gem_string("after_ms"), gem_v_after_ms);
    gem_table_set(_t140, gem_string("after_body"), gem_v_after_body);
    gem_table_set(_t140, gem_string("line"), gem_v_line);
    gem_table_set(_t140, gem_string("recv_var"), gem_v_recv_var);
    GemVal _t141 = _t140;
    gem_pop_frame();
    return _t141;
}

static GemVal gem_fn__mod_ast_make_return(void *_env, GemVal *args, int argc) {
#line 120 "compiler/main.gem"
    GemVal gem_v_value = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_line = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_return", "compiler/main.gem", 120);
    GemVal _t142 = gem_table_new();
    gem_table_set(_t142, gem_string("tag"), gem_string("return"));
    gem_table_set(_t142, gem_string("value"), gem_v_value);
    gem_table_set(_t142, gem_string("line"), gem_v_line);
    GemVal _t143 = _t142;
    gem_pop_frame();
    return _t143;
}

static GemVal gem_fn__mod_ast_make_break(void *_env, GemVal *args, int argc) {
#line 124 "compiler/main.gem"
    gem_push_frame("_mod_ast_make_break", "compiler/main.gem", 124);
    GemVal _t144 = gem_table_new();
    gem_table_set(_t144, gem_string("tag"), gem_string("break"));
    GemVal _t145 = _t144;
    gem_pop_frame();
    return _t145;
}

static GemVal gem_fn__mod_ast_make_continue(void *_env, GemVal *args, int argc) {
#line 128 "compiler/main.gem"
    gem_push_frame("_mod_ast_make_continue", "compiler/main.gem", 128);
    GemVal _t146 = gem_table_new();
    gem_table_set(_t146, gem_string("tag"), gem_string("continue"));
    GemVal _t147 = _t146;
    gem_pop_frame();
    return _t147;
}

static GemVal gem_fn__mod_ast_make_load(void *_env, GemVal *args, int argc) {
#line 132 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_load", "compiler/main.gem", 132);
    GemVal _t148 = gem_table_new();
    gem_table_set(_t148, gem_string("tag"), gem_string("load"));
    gem_table_set(_t148, gem_string("path"), gem_v_path);
    GemVal _t149 = _t148;
    gem_pop_frame();
    return _t149;
}

static GemVal gem_fn__mod_ast_make_export(void *_env, GemVal *args, int argc) {
#line 136 "compiler/main.gem"
    GemVal gem_v_names = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_export", "compiler/main.gem", 136);
    GemVal _t150 = gem_table_new();
    gem_table_set(_t150, gem_string("tag"), gem_string("export"));
    gem_table_set(_t150, gem_string("names"), gem_v_names);
    GemVal _t151 = _t150;
    gem_pop_frame();
    return _t151;
}

static GemVal gem_fn__mod_ast_make_extern_fn(void *_env, GemVal *args, int argc) {
#line 140 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    GemVal gem_v_is_blocking = (argc > 3) ? args[3] : GEM_NIL;
    gem_push_frame("_mod_ast_make_extern_fn", "compiler/main.gem", 140);
    GemVal _t152 = gem_table_new();
    gem_table_set(_t152, gem_string("tag"), gem_string("extern_fn"));
    gem_table_set(_t152, gem_string("name"), gem_v_name);
    gem_table_set(_t152, gem_string("params"), gem_v_params);
    gem_table_set(_t152, gem_string("ret_type"), gem_v_ret_type);
    gem_table_set(_t152, gem_string("blocking"), gem_v_is_blocking);
    GemVal _t153 = _t152;
    gem_pop_frame();
    return _t153;
}

static GemVal gem_fn__mod_ast_make_extern_param(void *_env, GemVal *args, int argc) {
#line 144 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_type_name = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_ast_make_extern_param", "compiler/main.gem", 144);
    GemVal _t154 = gem_table_new();
    gem_table_set(_t154, gem_string("name"), gem_v_name);
    gem_table_set(_t154, gem_string("type"), gem_v_type_name);
    GemVal _t155 = _t154;
    gem_pop_frame();
    return _t155;
}

static GemVal gem_fn__mod_ast_make_extern_include(void *_env, GemVal *args, int argc) {
#line 148 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_ast_make_extern_include", "compiler/main.gem", 148);
    GemVal _t156 = gem_table_new();
    gem_table_set(_t156, gem_string("tag"), gem_string("extern_include"));
    gem_table_set(_t156, gem_string("path"), gem_v_path);
    GemVal _t157 = _t156;
    gem_pop_frame();
    return _t157;
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
    GemVal _t158[] = {gem_v_source};
    GemVal gem_v_slen = gem_len_fn(NULL, _t158, 1);
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
                GemVal _t159;
                if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) {
                                _t159 = gem_lt(gem_v_i, gem_v_slen);
                } else {
                                _t159 = gem_neq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n"));
                }
                if (!gem_truthy(_t159)) break;
#line 11 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 13 "compiler/main.gem"
    GemVal _t160[] = {gem_v_source, gem_v_start, gem_sub(gem_v_i, gem_v_start)};
            GemVal _t161 = gem_substr_fn(NULL, _t160, 3);
            gem_pop_frame();
            return _t161;
        }
#line 15 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_i), gem_string("\n")))) {
#line 16 "compiler/main.gem"
            gem_v_current = gem_add(gem_v_current, gem_int(1));
        }
#line 18 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t162 = gem_string("");
    gem_pop_frame();
    return _t162;
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

    GemVal _t163 = gem_v_result;
    gem_pop_frame();
    return _t163;
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
    GemVal _t164;
    if (gem_truthy(gem_eq(gem_v_source, GEM_NIL))) {
        _t164 = gem_eq(gem_v_source, GEM_NIL);
    } else {
        _t164 = gem_eq(gem_v_line, GEM_NIL);
    }
    if (gem_truthy(_t164)) {
#line 36 "compiler/main.gem"
    GemVal _t165[] = {gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"))};
        (void)(gem_eprint_fn(NULL, _t165, 1));
#line 37 "compiler/main.gem"
    GemVal _t166[] = {gem_int(1)};
        (void)(gem_exit_process_fn(NULL, _t166, 1));
    }
#line 39 "compiler/main.gem"
    GemVal _t167;
    if (gem_truthy(gem_eq(gem_v_col, GEM_NIL))) {
        _t167 = gem_eq(gem_v_col, GEM_NIL);
    } else {
        _t167 = gem_lt(gem_v_col, gem_int(1));
    }
    if (gem_truthy(_t167)) {
#line 40 "compiler/main.gem"
        gem_v_col = gem_int(1);
    }
#line 43 "compiler/main.gem"
    GemVal _t168[] = {gem_v_line};
    GemVal gem_v_line_str = gem_to_string_fn(NULL, _t168, 1);
#line 44 "compiler/main.gem"
    GemVal _t169[] = {gem_v_line_str};
    GemVal gem_v_gutter_width = gem_len_fn(NULL, _t169, 1);
#line 45 "compiler/main.gem"
    GemVal _t170[] = {gem_string(" "), gem_v_gutter_width};
    GemVal gem_v_padding = gem_fn__mod_errors_repeat_char(NULL, _t170, 2);
#line 46 "compiler/main.gem"
    GemVal _t171[] = {gem_v_source, gem_v_line};
    GemVal gem_v_src_line = gem_fn__mod_errors_get_source_line(NULL, _t171, 2);
#line 48 "compiler/main.gem"
    GemVal gem_v_actual_span = gem_v_span_len;
#line 49 "compiler/main.gem"
    GemVal _t172;
    if (gem_truthy(gem_eq(gem_v_actual_span, GEM_NIL))) {
        _t172 = gem_eq(gem_v_actual_span, GEM_NIL);
    } else {
        _t172 = gem_lt(gem_v_actual_span, gem_int(1));
    }
    if (gem_truthy(_t172)) {
#line 50 "compiler/main.gem"
        gem_v_actual_span = gem_int(1);
    }
#line 53 "compiler/main.gem"
    GemVal gem_v_out = gem_add(gem_add(gem_string("\n[Compile Error]: "), gem_v_msg), gem_string("\n"));
#line 54 "compiler/main.gem"
    GemVal _t173[] = {gem_v_col};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string("  --> ")), gem_v_file), gem_string(":")), gem_v_line_str), gem_string(":")), gem_to_string_fn(NULL, _t173, 1)), gem_string("\n"));
#line 55 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 56 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_line_str), gem_string(" | ")), gem_v_src_line), gem_string("\n"));
#line 57 "compiler/main.gem"
    GemVal _t174[] = {gem_string(" "), gem_sub(gem_v_col, gem_int(1))};
    GemVal _t175[] = {gem_string("^"), gem_v_actual_span};
    gem_v_out = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" | ")), gem_fn__mod_errors_repeat_char(NULL, _t174, 2)), gem_fn__mod_errors_repeat_char(NULL, _t175, 2)), gem_string("\n"));
#line 58 "compiler/main.gem"
    gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string(" ")), gem_v_padding), gem_string(" |\n"));
#line 60 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_hint, GEM_NIL))) {
#line 61 "compiler/main.gem"
        gem_v_out = gem_add(gem_add(gem_add(gem_v_out, gem_string("Hint: ")), gem_v_hint), gem_string("\n"));
    }
#line 64 "compiler/main.gem"
    GemVal _t176[] = {gem_v_out};
    (void)(gem_eprint_fn(NULL, _t176, 1));
#line 65 "compiler/main.gem"
    GemVal _t177[] = {gem_int(1)};
    GemVal _t178 = gem_exit_process_fn(NULL, _t177, 1);
    gem_pop_frame();
    return _t178;
}

static GemVal gem_fn__mod_lexer_is_digit(void *_env, GemVal *args, int argc) {
#line 15 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_lexer_is_digit", "compiler/main.gem", 15);
    GemVal _t179;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("0")))) {
        _t179 = gem_ge(gem_v_ch, gem_string("0"));
    } else {
        _t179 = gem_le(gem_v_ch, gem_string("9"));
    }
    GemVal _t180 = _t179;
    gem_pop_frame();
    return _t180;
}

static GemVal gem_fn__mod_lexer_is_alpha(void *_env, GemVal *args, int argc) {
#line 19 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_lexer_is_alpha", "compiler/main.gem", 19);
    GemVal _t181;
    if (!gem_truthy(gem_ge(gem_v_ch, gem_string("a")))) {
        _t181 = gem_ge(gem_v_ch, gem_string("a"));
    } else {
        _t181 = gem_le(gem_v_ch, gem_string("z"));
    }
    GemVal _t183;
    if (gem_truthy(_t181)) {
        _t183 = _t181;
    } else {
        GemVal _t182;
        if (!gem_truthy(gem_ge(gem_v_ch, gem_string("A")))) {
                _t182 = gem_ge(gem_v_ch, gem_string("A"));
        } else {
                _t182 = gem_le(gem_v_ch, gem_string("Z"));
        }
        _t183 = _t182;
    }
    GemVal _t184;
    if (gem_truthy(_t183)) {
        _t184 = _t183;
    } else {
        _t184 = gem_eq(gem_v_ch, gem_string("_"));
    }
    GemVal _t185 = _t184;
    gem_pop_frame();
    return _t185;
}

static GemVal gem_fn__mod_lexer_is_alnum(void *_env, GemVal *args, int argc) {
#line 23 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_lexer_is_alnum", "compiler/main.gem", 23);
    GemVal _t186[] = {gem_v_ch};
    GemVal _t188;
    if (gem_truthy(gem_fn__mod_lexer_is_alpha(NULL, _t186, 1))) {
        _t188 = gem_fn__mod_lexer_is_alpha(NULL, _t186, 1);
    } else {
        GemVal _t187[] = {gem_v_ch};
        _t188 = gem_fn__mod_lexer_is_digit(NULL, _t187, 1);
    }
    GemVal _t189 = _t188;
    gem_pop_frame();
    return _t189;
}

static GemVal gem_fn__mod_lexer_apply_escape(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_esc = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_mode = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_lexer_apply_escape", "compiler/main.gem", 32);
#line 33 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_esc, gem_string("n")))) {
#line 34 "compiler/main.gem"
        GemVal _t190 = gem_string("\n");
        gem_pop_frame();
        return _t190;
    } else {
#line 35 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_esc, gem_string("r")))) {
#line 36 "compiler/main.gem"
    GemVal _t191[] = {gem_int(13)};
            GemVal _t192 = gem_chr_fn(NULL, _t191, 1);
            gem_pop_frame();
            return _t192;
        } else {
#line 37 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_esc, gem_string("t")))) {
#line 38 "compiler/main.gem"
                GemVal _t193 = gem_string("\t");
                gem_pop_frame();
                return _t193;
            } else {
#line 39 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_esc, gem_string("\\")))) {
#line 40 "compiler/main.gem"
                    GemVal _t194 = gem_string("\\");
                    gem_pop_frame();
                    return _t194;
                } else {
#line 41 "compiler/main.gem"
    GemVal _t195;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("0")))) {
        _t195 = gem_eq(gem_v_esc, gem_string("0"));
    } else {
        _t195 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                    if (gem_truthy(_t195)) {
#line 42 "compiler/main.gem"
    GemVal _t196[] = {gem_int(0)};
                        GemVal _t197 = gem_chr_fn(NULL, _t196, 1);
                        gem_pop_frame();
                        return _t197;
                    } else {
#line 43 "compiler/main.gem"
    GemVal _t198;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("\"")))) {
        _t198 = gem_eq(gem_v_esc, gem_string("\""));
    } else {
        _t198 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                        if (gem_truthy(_t198)) {
#line 44 "compiler/main.gem"
                            GemVal _t199 = gem_string("\"");
                            gem_pop_frame();
                            return _t199;
                        } else {
#line 45 "compiler/main.gem"
    GemVal _t200;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("{")))) {
        _t200 = gem_eq(gem_v_esc, gem_string("{"));
    } else {
        _t200 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                            if (gem_truthy(_t200)) {
#line 46 "compiler/main.gem"
                                GemVal _t201 = gem_string("{");
                                gem_pop_frame();
                                return _t201;
                            } else {
#line 47 "compiler/main.gem"
    GemVal _t202;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("}")))) {
        _t202 = gem_eq(gem_v_esc, gem_string("}"));
    } else {
        _t202 = gem_eq(gem_v_mode, gem_string("dq"));
    }
                                if (gem_truthy(_t202)) {
#line 48 "compiler/main.gem"
                                    GemVal _t203 = gem_string("}");
                                    gem_pop_frame();
                                    return _t203;
                                } else {
#line 49 "compiler/main.gem"
    GemVal _t204;
    if (!gem_truthy(gem_eq(gem_v_esc, gem_string("'")))) {
        _t204 = gem_eq(gem_v_esc, gem_string("'"));
    } else {
        _t204 = gem_eq(gem_v_mode, gem_string("sq"));
    }
                                    if (gem_truthy(_t204)) {
#line 50 "compiler/main.gem"
                                        GemVal _t205 = gem_string("'");
                                        gem_pop_frame();
                                        return _t205;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    GemVal _t206 = GEM_NIL;
    gem_pop_frame();
    return _t206;
}

static GemVal gem_fn__mod_lexer_build_keyword_set(void *_env, GemVal *args, int argc) {
#line 57 "compiler/main.gem"
    gem_push_frame("_mod_lexer_build_keyword_set", "compiler/main.gem", 57);
#line 58 "compiler/main.gem"
    GemVal _t207 = gem_table_new();
    GemVal gem_v_t = _t207;
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
    GemVal _t208 = gem_v_t;
    gem_pop_frame();
    return _t208;
}

static GemVal gem_fn__mod_lexer_tokenize(void *_env, GemVal *args, int argc) {
#line 89 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_lexer_tokenize", "compiler/main.gem", 89);
#line 90 "compiler/main.gem"
    GemVal gem_v_kw = gem_fn__mod_lexer_build_keyword_set(NULL, NULL, 0);
#line 91 "compiler/main.gem"
    GemVal _t209 = gem_table_new();
    GemVal gem_v_tokens = _t209;
#line 92 "compiler/main.gem"
    GemVal gem_v_pos = gem_int(0);
#line 93 "compiler/main.gem"
    GemVal gem_v_line = gem_int(1);
#line 94 "compiler/main.gem"
    GemVal gem_v_line_start = gem_int(0);
#line 95 "compiler/main.gem"
    GemVal _t210[] = {gem_v_source};
    GemVal gem_v_length = gem_len_fn(NULL, _t210, 1);
#line 96 "compiler/main.gem"
    GemVal gem_v_interp_depth = gem_int(0);
#line 97 "compiler/main.gem"
    GemVal _t211 = gem_table_new();
    GemVal gem_v_interp_brace = _t211;
#line 98 "compiler/main.gem"
    GemVal _t212[] = {gem_int(13)};
    GemVal gem_v_CR = gem_chr_fn(NULL, _t212, 1);
#line 99 "compiler/main.gem"
    GemVal _t213[] = {gem_int(0)};
    GemVal gem_v_NUL = gem_chr_fn(NULL, _t213, 1);
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
    GemVal _t214;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(" ")))) {
        _t214 = gem_eq(gem_v_ch, gem_string(" "));
    } else {
        _t214 = gem_eq(gem_v_ch, gem_string("\t"));
    }
        if (gem_truthy(_t214)) {
#line 110 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 115 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("#")))) {
#line 116 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t215;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t215 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t215 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                }
                if (!gem_truthy(_t215)) break;
#line 117 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 123 "compiler/main.gem"
    GemVal _t216;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
        _t216 = gem_eq(gem_v_ch, gem_string("\n"));
    } else {
        _t216 = gem_eq(gem_v_ch, gem_string(";"));
    }
        if (gem_truthy(_t216)) {
#line 124 "compiler/main.gem"
            GemVal gem_v_should_add = gem_bool(1);
#line 125 "compiler/main.gem"
    GemVal _t217[] = {gem_v_tokens};
    GemVal _t219;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t217, 1), gem_int(0)))) {
        _t219 = gem_gt(gem_len_fn(NULL, _t217, 1), gem_int(0));
    } else {
        GemVal _t218 = gem_table_get(gem_v_tokens, gem_neg(gem_int(1)));
        static GemICacheSlot _ic_1 = {0};
        _t219 = gem_eq(gem_table_get_cached(_t218, "type", &_ic_1), gem_string("NEWLINE"));
    }
            if (gem_truthy(_t219)) {
#line 126 "compiler/main.gem"
                gem_v_should_add = gem_bool(0);
            }
#line 128 "compiler/main.gem"
            if (gem_truthy(gem_v_should_add)) {
#line 129 "compiler/main.gem"
    GemVal _t220 = gem_table_new();
    gem_table_set(_t220, gem_string("type"), gem_string("NEWLINE"));
    gem_table_set(_t220, gem_string("value"), gem_string("\n"));
    gem_table_set(_t220, gem_string("line"), gem_v_line);
    gem_table_set(_t220, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t221[] = {gem_v_tokens, _t220};
                (void)(gem_push_fn(NULL, _t221, 2));
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
    GemVal _t222;
    if (gem_truthy(gem_eq(gem_v_c2, gem_string(";")))) {
        _t222 = gem_eq(gem_v_c2, gem_string(";"));
    } else {
        _t222 = gem_eq(gem_v_c2, gem_string(" "));
    }
    GemVal _t223;
    if (gem_truthy(_t222)) {
        _t223 = _t222;
    } else {
        _t223 = gem_eq(gem_v_c2, gem_string("\t"));
    }
                    if (gem_truthy(_t223)) {
#line 144 "compiler/main.gem"
                        gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                    } else {
#line 145 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_c2, gem_string("#")))) {
#line 146 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t224;
                                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                                                _t224 = gem_lt(gem_v_pos, gem_v_length);
                                } else {
                                                                _t224 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
                                }
                                if (!gem_truthy(_t224)) break;
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
    GemVal _t225[] = {gem_v_ch};
        if (gem_truthy(gem_fn__mod_lexer_is_digit(NULL, _t225, 1))) {
#line 158 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 159 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t227;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t227 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t226[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t227 = gem_fn__mod_lexer_is_digit(NULL, _t226, 1);
                }
                if (!gem_truthy(_t227)) break;
#line 160 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 161 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 163 "compiler/main.gem"
    GemVal _t228;
    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
        _t228 = gem_lt(gem_v_pos, gem_v_length);
    } else {
        _t228 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("."));
    }
            GemVal gem_v_has_dot = _t228;
#line 164 "compiler/main.gem"
            if (gem_truthy(gem_v_has_dot)) {
#line 165 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_string("."));
#line 166 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 167 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t230;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t230 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t229[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                        _t230 = gem_fn__mod_lexer_is_digit(NULL, _t229, 1);
                    }
                    if (!gem_truthy(_t230)) break;
#line 168 "compiler/main.gem"
                    gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 169 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
            }
#line 172 "compiler/main.gem"
    GemVal _t231 = gem_table_new();
    gem_table_set(_t231, gem_string("type"), gem_string("NUMBER"));
    gem_table_set(_t231, gem_string("value"), gem_v_val);
    gem_table_set(_t231, gem_string("line"), gem_v_line);
    gem_table_set(_t231, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t232[] = {gem_v_tokens, _t231};
            (void)(gem_push_fn(NULL, _t232, 2));
            continue;
        }
#line 177 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 178 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 181 "compiler/main.gem"
    GemVal _t233;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t233 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t233 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t234;
    if (!gem_truthy(_t233)) {
        _t234 = _t233;
    } else {
        _t234 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
            if (gem_truthy(_t234)) {
#line 182 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 184 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t236;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t236 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t235;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t235 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t235 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t236 = _t235;
                    }
                    if (!gem_truthy(_t236)) break;
#line 185 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 187 "compiler/main.gem"
    GemVal _t237;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t237 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t237 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t237)) {
#line 188 "compiler/main.gem"
    GemVal _t238[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
    GemVal _t239 = gem_v__mod_lexer_compile_error;
                    (void)(_t239.fn(_t239.env, _t238, 7));
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
                    GemVal _t240;
                    if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                        _t240 = gem_lt(gem_v_tq_scan, gem_v_length);
                    } else {
                                        _t240 = gem_not(gem_v_tq_found);
                    }
                    if (!gem_truthy(_t240)) break;
#line 199 "compiler/main.gem"
                    GemVal gem_v_tq_ind = gem_int(0);
#line 200 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t241;
                        if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                _t241 = gem_lt(gem_v_tq_scan, gem_v_length);
                        } else {
                                                _t241 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t241)) break;
#line 201 "compiler/main.gem"
                        gem_v_tq_ind = gem_add(gem_v_tq_ind, gem_int(1));
#line 202 "compiler/main.gem"
                        gem_v_tq_scan = gem_add(gem_v_tq_scan, gem_int(1));
                    }
#line 204 "compiler/main.gem"
    GemVal _t242;
    if (!gem_truthy(gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length))) {
        _t242 = gem_lt(gem_add(gem_v_tq_scan, gem_int(2)), gem_v_length);
    } else {
        _t242 = gem_eq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\""));
    }
    GemVal _t243;
    if (!gem_truthy(_t242)) {
        _t243 = _t242;
    } else {
        _t243 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(1))), gem_string("\""));
    }
    GemVal _t244;
    if (!gem_truthy(_t243)) {
        _t244 = _t243;
    } else {
        _t244 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_tq_scan, gem_int(2))), gem_string("\""));
    }
                    if (gem_truthy(_t244)) {
#line 205 "compiler/main.gem"
                        gem_v_tq_bi = gem_v_tq_ind;
#line 206 "compiler/main.gem"
                        gem_v_tq_found = gem_bool(1);
                    } else {
#line 208 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t245;
                            if (!gem_truthy(gem_lt(gem_v_tq_scan, gem_v_length))) {
                                                        _t245 = gem_lt(gem_v_tq_scan, gem_v_length);
                            } else {
                                                        _t245 = gem_neq(gem_table_get(gem_v_source, gem_v_tq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t245)) break;
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
    GemVal _t246[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
    GemVal _t247 = gem_v__mod_lexer_compile_error;
                    (void)(_t247.fn(_t247.env, _t246, 7));
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
                            GemVal _t248;
                            if (!gem_truthy(gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent))) {
                                                        _t248 = gem_lt(gem_v_tq_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t248 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t249;
                            if (!gem_truthy(_t248)) {
                                                        _t249 = _t248;
                            } else {
                                                        _t249 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t249)) break;
#line 231 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 232 "compiler/main.gem"
                            gem_v_tq_skip = gem_add(gem_v_tq_skip, gem_int(1));
                        }
#line 235 "compiler/main.gem"
    GemVal _t250;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t250 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t250 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t251;
    if (!gem_truthy(_t250)) {
        _t251 = _t250;
    } else {
        _t251 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t252;
    if (!gem_truthy(_t251)) {
        _t252 = _t251;
    } else {
        _t252 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t252)) {
#line 236 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 238 "compiler/main.gem"
    GemVal _t253[] = {gem_v_val};
    GemVal _t254;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t253, 1), gem_int(0)))) {
        _t254 = gem_gt(gem_len_fn(NULL, _t253, 1), gem_int(0));
    } else {
        _t254 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t254)) {
#line 239 "compiler/main.gem"
    GemVal _t255[] = {gem_v_val};
    GemVal _t256[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t255, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t256, 3);
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
    GemVal _t257[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t258 = gem_v__mod_lexer_compile_error;
                            (void)(_t258.fn(_t258.env, _t257, 7));
                        }
#line 259 "compiler/main.gem"
                        GemVal gem_v_tqe = gem_table_get(gem_v_source, gem_v_pos);
#line 260 "compiler/main.gem"
    GemVal _t259[] = {gem_v_tqe, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t259, 2);
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
    GemVal _t260 = gem_table_new();
    gem_table_set(_t260, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t260, gem_string("value"), gem_string(""));
    gem_table_set(_t260, gem_string("line"), gem_v_line);
    gem_table_set(_t260, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t261[] = {gem_v_tokens, _t260};
                        (void)(gem_push_fn(NULL, _t261, 2));
#line 271 "compiler/main.gem"
    GemVal _t262 = gem_table_new();
    gem_table_set(_t262, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t262, gem_string("value"), gem_v_val);
    gem_table_set(_t262, gem_string("line"), gem_v_line);
    gem_table_set(_t262, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t263[] = {gem_v_tokens, _t262};
                        (void)(gem_push_fn(NULL, _t263, 2));
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
    GemVal _t264 = gem_table_new();
    gem_table_set(_t264, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t264, gem_string("value"), gem_v_val);
    gem_table_set(_t264, gem_string("line"), gem_v_line);
    gem_table_set(_t264, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t265[] = {gem_v_tokens, _t264};
                    (void)(gem_push_fn(NULL, _t265, 2));
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
                GemVal _t266;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t266 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t266 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t266)) break;
#line 294 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 295 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 296 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 297 "compiler/main.gem"
    GemVal _t267[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t268 = gem_v__mod_lexer_compile_error;
                        (void)(_t268.fn(_t268.env, _t267, 7));
                    }
#line 299 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 300 "compiler/main.gem"
    GemVal _t269[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t269, 2);
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
    GemVal _t270 = gem_table_new();
    gem_table_set(_t270, gem_string("type"), gem_string("INTERP_START"));
    gem_table_set(_t270, gem_string("value"), gem_string(""));
    gem_table_set(_t270, gem_string("line"), gem_v_line);
    gem_table_set(_t270, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t271[] = {gem_v_tokens, _t270};
                    (void)(gem_push_fn(NULL, _t271, 2));
#line 311 "compiler/main.gem"
    GemVal _t272 = gem_table_new();
    gem_table_set(_t272, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t272, gem_string("value"), gem_v_val);
    gem_table_set(_t272, gem_string("line"), gem_v_line);
    gem_table_set(_t272, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t273[] = {gem_v_tokens, _t272};
                    (void)(gem_push_fn(NULL, _t273, 2));
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
    GemVal _t274[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
    GemVal _t275 = gem_v__mod_lexer_compile_error;
                    (void)(_t275.fn(_t275.env, _t274, 7));
                }
#line 326 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 327 "compiler/main.gem"
    GemVal _t276 = gem_table_new();
    gem_table_set(_t276, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t276, gem_string("value"), gem_v_val);
    gem_table_set(_t276, gem_string("line"), gem_v_line);
    gem_table_set(_t276, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t277[] = {gem_v_tokens, _t276};
                (void)(gem_push_fn(NULL, _t277, 2));
            }
            continue;
        }
#line 333 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ch, gem_string("'")))) {
#line 334 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 337 "compiler/main.gem"
    GemVal _t278;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length))) {
        _t278 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    } else {
        _t278 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t279;
    if (!gem_truthy(_t278)) {
        _t279 = _t278;
    } else {
        _t279 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
            if (gem_truthy(_t279)) {
#line 338 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(2));
#line 340 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t281;
                    if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                        _t281 = gem_lt(gem_v_pos, gem_v_length);
                    } else {
                                        GemVal _t280;
                                        if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" ")))) {
                                                                                _t280 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                                        } else {
                                                                                _t280 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\t"));
                                        }
                                        _t281 = _t280;
                    }
                    if (!gem_truthy(_t281)) break;
#line 341 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
                }
#line 343 "compiler/main.gem"
    GemVal _t282;
    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
        _t282 = gem_ge(gem_v_pos, gem_v_length);
    } else {
        _t282 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\n"));
    }
                if (gem_truthy(_t282)) {
#line 344 "compiler/main.gem"
    GemVal _t283[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("triple-quoted string must be followed by a newline"), GEM_NIL};
    GemVal _t284 = gem_v__mod_lexer_compile_error;
                    (void)(_t284.fn(_t284.env, _t283, 7));
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
                    GemVal _t285;
                    if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                        _t285 = gem_lt(gem_v_sq_scan, gem_v_length);
                    } else {
                                        _t285 = gem_not(gem_v_sq_found);
                    }
                    if (!gem_truthy(_t285)) break;
#line 355 "compiler/main.gem"
                    GemVal gem_v_sq_ind = gem_int(0);
#line 356 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t286;
                        if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                _t286 = gem_lt(gem_v_sq_scan, gem_v_length);
                        } else {
                                                _t286 = gem_eq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string(" "));
                        }
                        if (!gem_truthy(_t286)) break;
#line 357 "compiler/main.gem"
                        gem_v_sq_ind = gem_add(gem_v_sq_ind, gem_int(1));
#line 358 "compiler/main.gem"
                        gem_v_sq_scan = gem_add(gem_v_sq_scan, gem_int(1));
                    }
#line 360 "compiler/main.gem"
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
#line 361 "compiler/main.gem"
                        gem_v_sq_bi = gem_v_sq_ind;
#line 362 "compiler/main.gem"
                        gem_v_sq_found = gem_bool(1);
                    } else {
#line 364 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t290;
                            if (!gem_truthy(gem_lt(gem_v_sq_scan, gem_v_length))) {
                                                        _t290 = gem_lt(gem_v_sq_scan, gem_v_length);
                            } else {
                                                        _t290 = gem_neq(gem_table_get(gem_v_source, gem_v_sq_scan), gem_string("\n"));
                            }
                            if (!gem_truthy(_t290)) break;
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
    GemVal _t291[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string"), GEM_NIL};
    GemVal _t292 = gem_v__mod_lexer_compile_error;
                    (void)(_t292.fn(_t292.env, _t291, 7));
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
                            GemVal _t293;
                            if (!gem_truthy(gem_lt(gem_v_sq_skip, gem_v_sq_bi))) {
                                                        _t293 = gem_lt(gem_v_sq_skip, gem_v_sq_bi);
                            } else {
                                                        _t293 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t294;
                            if (!gem_truthy(_t293)) {
                                                        _t294 = _t293;
                            } else {
                                                        _t294 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t294)) break;
#line 385 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 386 "compiler/main.gem"
                            gem_v_sq_skip = gem_add(gem_v_sq_skip, gem_int(1));
                        }
#line 389 "compiler/main.gem"
    GemVal _t295;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t295 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t295 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
    }
    GemVal _t296;
    if (!gem_truthy(_t295)) {
        _t296 = _t295;
    } else {
        _t296 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("'"));
    }
    GemVal _t297;
    if (!gem_truthy(_t296)) {
        _t297 = _t296;
    } else {
        _t297 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("'"));
    }
                        if (gem_truthy(_t297)) {
#line 390 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 392 "compiler/main.gem"
    GemVal _t298[] = {gem_v_val};
    GemVal _t299;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t298, 1), gem_int(0)))) {
        _t299 = gem_gt(gem_len_fn(NULL, _t298, 1), gem_int(0));
    } else {
        _t299 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t299)) {
#line 393 "compiler/main.gem"
    GemVal _t300[] = {gem_v_val};
    GemVal _t301[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t300, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t301, 3);
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
    GemVal _t302[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t303 = gem_v__mod_lexer_compile_error;
                            (void)(_t303.fn(_t303.env, _t302, 7));
                        }
#line 413 "compiler/main.gem"
                        GemVal gem_v_sqe = gem_table_get(gem_v_source, gem_v_pos);
#line 414 "compiler/main.gem"
    GemVal _t304[] = {gem_v_sqe, gem_string("sq")};
                        GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t304, 2);
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
    GemVal _t305 = gem_table_new();
    gem_table_set(_t305, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t305, gem_string("value"), gem_v_val);
    gem_table_set(_t305, gem_string("line"), gem_v_line);
    gem_table_set(_t305, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t306[] = {gem_v_tokens, _t305};
                (void)(gem_push_fn(NULL, _t306, 2));
                continue;
            }
#line 432 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 433 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t307;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t307 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t307 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("'"));
                }
                if (!gem_truthy(_t307)) break;
#line 434 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 435 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 436 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 437 "compiler/main.gem"
    GemVal _t308[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t309 = gem_v__mod_lexer_compile_error;
                        (void)(_t309.fn(_t309.env, _t308, 7));
                    }
#line 439 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 440 "compiler/main.gem"
    GemVal _t310[] = {gem_v_esc, gem_string("sq")};
                    GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t310, 2);
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
    GemVal _t311[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string"), gem_string("this string is never closed")};
    GemVal _t312 = gem_v__mod_lexer_compile_error;
                (void)(_t312.fn(_t312.env, _t311, 7));
            }
#line 455 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 456 "compiler/main.gem"
    GemVal _t313 = gem_table_new();
    gem_table_set(_t313, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t313, gem_string("value"), gem_v_val);
    gem_table_set(_t313, gem_string("line"), gem_v_line);
    gem_table_set(_t313, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t314[] = {gem_v_tokens, _t313};
            (void)(gem_push_fn(NULL, _t314, 2));
            continue;
        }
#line 461 "compiler/main.gem"
    GemVal _t315[] = {gem_v_ch};
        if (gem_truthy(gem_fn__mod_lexer_is_alpha(NULL, _t315, 1))) {
#line 462 "compiler/main.gem"
            GemVal gem_v_val = gem_string("");
#line 463 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t317;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t317 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                GemVal _t316[] = {gem_table_get(gem_v_source, gem_v_pos)};
                                _t317 = gem_fn__mod_lexer_is_alnum(NULL, _t316, 1);
                }
                if (!gem_truthy(_t317)) break;
#line 464 "compiler/main.gem"
                gem_v_val = gem_add(gem_v_val, gem_table_get(gem_v_source, gem_v_pos));
#line 465 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
#line 467 "compiler/main.gem"
    GemVal _t318[] = {gem_v_kw, gem_v_val};
            if (gem_truthy(gem_in_fn(NULL, _t318, 2))) {
#line 468 "compiler/main.gem"
    GemVal _t319 = gem_table_new();
    gem_table_set(_t319, gem_string("type"), gem_v_val);
    gem_table_set(_t319, gem_string("value"), gem_v_val);
    gem_table_set(_t319, gem_string("line"), gem_v_line);
    gem_table_set(_t319, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t320[] = {gem_v_tokens, _t319};
                (void)(gem_push_fn(NULL, _t320, 2));
            } else {
#line 470 "compiler/main.gem"
    GemVal _t321 = gem_table_new();
    gem_table_set(_t321, gem_string("type"), gem_string("NAME"));
    gem_table_set(_t321, gem_string("value"), gem_v_val);
    gem_table_set(_t321, gem_string("line"), gem_v_line);
    gem_table_set(_t321, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t322[] = {gem_v_tokens, _t321};
                (void)(gem_push_fn(NULL, _t322, 2));
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
    GemVal _t323;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("=")))) {
        _t323 = gem_eq(gem_v_ch, gem_string("="));
    } else {
        _t323 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t323)) {
#line 482 "compiler/main.gem"
    GemVal _t324 = gem_table_new();
    gem_table_set(_t324, gem_string("type"), gem_string("=="));
    gem_table_set(_t324, gem_string("value"), gem_string("=="));
    gem_table_set(_t324, gem_string("line"), gem_v_line);
    gem_table_set(_t324, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t325[] = {gem_v_tokens, _t324};
            (void)(gem_push_fn(NULL, _t325, 2));
#line 483 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 486 "compiler/main.gem"
    GemVal _t326;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("!")))) {
        _t326 = gem_eq(gem_v_ch, gem_string("!"));
    } else {
        _t326 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t326)) {
#line 487 "compiler/main.gem"
    GemVal _t327 = gem_table_new();
    gem_table_set(_t327, gem_string("type"), gem_string("!="));
    gem_table_set(_t327, gem_string("value"), gem_string("!="));
    gem_table_set(_t327, gem_string("line"), gem_v_line);
    gem_table_set(_t327, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t328[] = {gem_v_tokens, _t327};
            (void)(gem_push_fn(NULL, _t328, 2));
#line 488 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 491 "compiler/main.gem"
    GemVal _t329;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t329 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t329 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t329)) {
#line 492 "compiler/main.gem"
    GemVal _t330 = gem_table_new();
    gem_table_set(_t330, gem_string("type"), gem_string("<="));
    gem_table_set(_t330, gem_string("value"), gem_string("<="));
    gem_table_set(_t330, gem_string("line"), gem_v_line);
    gem_table_set(_t330, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t331[] = {gem_v_tokens, _t330};
            (void)(gem_push_fn(NULL, _t331, 2));
#line 493 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 496 "compiler/main.gem"
    GemVal _t332;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(">")))) {
        _t332 = gem_eq(gem_v_ch, gem_string(">"));
    } else {
        _t332 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t332)) {
#line 497 "compiler/main.gem"
    GemVal _t333 = gem_table_new();
    gem_table_set(_t333, gem_string("type"), gem_string(">="));
    gem_table_set(_t333, gem_string("value"), gem_string(">="));
    gem_table_set(_t333, gem_string("line"), gem_v_line);
    gem_table_set(_t333, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t334[] = {gem_v_tokens, _t333};
            (void)(gem_push_fn(NULL, _t334, 2));
#line 498 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 501 "compiler/main.gem"
    GemVal _t335;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t335 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t335 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t335)) {
#line 502 "compiler/main.gem"
    GemVal _t336 = gem_table_new();
    gem_table_set(_t336, gem_string("type"), gem_string("+="));
    gem_table_set(_t336, gem_string("value"), gem_string("+="));
    gem_table_set(_t336, gem_string("line"), gem_v_line);
    gem_table_set(_t336, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t337[] = {gem_v_tokens, _t336};
            (void)(gem_push_fn(NULL, _t337, 2));
#line 503 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 506 "compiler/main.gem"
    GemVal _t338;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t338 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t338 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t338)) {
#line 507 "compiler/main.gem"
    GemVal _t339 = gem_table_new();
    gem_table_set(_t339, gem_string("type"), gem_string("-="));
    gem_table_set(_t339, gem_string("value"), gem_string("-="));
    gem_table_set(_t339, gem_string("line"), gem_v_line);
    gem_table_set(_t339, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t340[] = {gem_v_tokens, _t339};
            (void)(gem_push_fn(NULL, _t340, 2));
#line 508 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 511 "compiler/main.gem"
    GemVal _t341;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("*")))) {
        _t341 = gem_eq(gem_v_ch, gem_string("*"));
    } else {
        _t341 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t341)) {
#line 512 "compiler/main.gem"
    GemVal _t342 = gem_table_new();
    gem_table_set(_t342, gem_string("type"), gem_string("*="));
    gem_table_set(_t342, gem_string("value"), gem_string("*="));
    gem_table_set(_t342, gem_string("line"), gem_v_line);
    gem_table_set(_t342, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t343[] = {gem_v_tokens, _t342};
            (void)(gem_push_fn(NULL, _t343, 2));
#line 513 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 516 "compiler/main.gem"
    GemVal _t344;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("/")))) {
        _t344 = gem_eq(gem_v_ch, gem_string("/"));
    } else {
        _t344 = gem_eq(gem_v_next, gem_string("="));
    }
        if (gem_truthy(_t344)) {
#line 517 "compiler/main.gem"
    GemVal _t345 = gem_table_new();
    gem_table_set(_t345, gem_string("type"), gem_string("/="));
    gem_table_set(_t345, gem_string("value"), gem_string("/="));
    gem_table_set(_t345, gem_string("line"), gem_v_line);
    gem_table_set(_t345, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t346[] = {gem_v_tokens, _t345};
            (void)(gem_push_fn(NULL, _t346, 2));
#line 518 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 521 "compiler/main.gem"
    GemVal _t347;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string("-")))) {
        _t347 = gem_eq(gem_v_ch, gem_string("-"));
    } else {
        _t347 = gem_eq(gem_v_next, gem_string(">"));
    }
        if (gem_truthy(_t347)) {
#line 522 "compiler/main.gem"
    GemVal _t348 = gem_table_new();
    gem_table_set(_t348, gem_string("type"), gem_string("->"));
    gem_table_set(_t348, gem_string("value"), gem_string("->"));
    gem_table_set(_t348, gem_string("line"), gem_v_line);
    gem_table_set(_t348, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t349[] = {gem_v_tokens, _t348};
            (void)(gem_push_fn(NULL, _t349, 2));
#line 523 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(2));
            continue;
        }
#line 528 "compiler/main.gem"
    GemVal _t350;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("+")))) {
        _t350 = gem_eq(gem_v_ch, gem_string("+"));
    } else {
        _t350 = gem_eq(gem_v_ch, gem_string("-"));
    }
    GemVal _t351;
    if (gem_truthy(_t350)) {
        _t351 = _t350;
    } else {
        _t351 = gem_eq(gem_v_ch, gem_string("*"));
    }
    GemVal _t352;
    if (gem_truthy(_t351)) {
        _t352 = _t351;
    } else {
        _t352 = gem_eq(gem_v_ch, gem_string("/"));
    }
    GemVal _t353;
    if (gem_truthy(_t352)) {
        _t353 = _t352;
    } else {
        _t353 = gem_eq(gem_v_ch, gem_string("%"));
    }
        if (gem_truthy(_t353)) {
#line 529 "compiler/main.gem"
    GemVal _t354 = gem_table_new();
    gem_table_set(_t354, gem_string("type"), gem_v_ch);
    gem_table_set(_t354, gem_string("value"), gem_v_ch);
    gem_table_set(_t354, gem_string("line"), gem_v_line);
    gem_table_set(_t354, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t355[] = {gem_v_tokens, _t354};
            (void)(gem_push_fn(NULL, _t355, 2));
#line 530 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 533 "compiler/main.gem"
    GemVal _t356;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("<")))) {
        _t356 = gem_eq(gem_v_ch, gem_string("<"));
    } else {
        _t356 = gem_eq(gem_v_ch, gem_string(">"));
    }
    GemVal _t357;
    if (gem_truthy(_t356)) {
        _t357 = _t356;
    } else {
        _t357 = gem_eq(gem_v_ch, gem_string("="));
    }
        if (gem_truthy(_t357)) {
#line 534 "compiler/main.gem"
    GemVal _t358 = gem_table_new();
    gem_table_set(_t358, gem_string("type"), gem_v_ch);
    gem_table_set(_t358, gem_string("value"), gem_v_ch);
    gem_table_set(_t358, gem_string("line"), gem_v_line);
    gem_table_set(_t358, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t359[] = {gem_v_tokens, _t358};
            (void)(gem_push_fn(NULL, _t359, 2));
#line 535 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 539 "compiler/main.gem"
    GemVal _t360;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t360 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t360 = gem_eq(gem_v_ch, gem_string("{"));
    }
        if (gem_truthy(_t360)) {
#line 540 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 541 "compiler/main.gem"
            gem_table_set(gem_v_interp_brace, gem_v_idx, gem_add(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 542 "compiler/main.gem"
    GemVal _t361 = gem_table_new();
    gem_table_set(_t361, gem_string("type"), gem_string("{"));
    gem_table_set(_t361, gem_string("value"), gem_string("{"));
    gem_table_set(_t361, gem_string("line"), gem_v_line);
    gem_table_set(_t361, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t362[] = {gem_v_tokens, _t361};
            (void)(gem_push_fn(NULL, _t362, 2));
#line 543 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 546 "compiler/main.gem"
    GemVal _t363;
    if (!gem_truthy(gem_gt(gem_v_interp_depth, gem_int(0)))) {
        _t363 = gem_gt(gem_v_interp_depth, gem_int(0));
    } else {
        _t363 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t363)) {
#line 547 "compiler/main.gem"
            GemVal gem_v_idx = gem_sub(gem_v_interp_depth, gem_int(1));
#line 548 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(0)))) {
#line 549 "compiler/main.gem"
                gem_table_set(gem_v_interp_brace, gem_v_idx, gem_sub(gem_table_get(gem_v_interp_brace, gem_v_idx), gem_int(1)));
#line 550 "compiler/main.gem"
    GemVal _t364 = gem_table_new();
    gem_table_set(_t364, gem_string("type"), gem_string("}"));
    gem_table_set(_t364, gem_string("value"), gem_string("}"));
    gem_table_set(_t364, gem_string("line"), gem_v_line);
    gem_table_set(_t364, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t365[] = {gem_v_tokens, _t364};
                (void)(gem_push_fn(NULL, _t365, 2));
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
                            GemVal _t366;
                            if (!gem_truthy(gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent))) {
                                                        _t366 = gem_lt(gem_v_tqr_skip, gem_v_triple_dq_indent);
                            } else {
                                                        _t366 = gem_lt(gem_v_pos, gem_v_length);
                            }
                            GemVal _t367;
                            if (!gem_truthy(_t366)) {
                                                        _t367 = _t366;
                            } else {
                                                        _t367 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string(" "));
                            }
                            if (!gem_truthy(_t367)) break;
#line 570 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 571 "compiler/main.gem"
                            gem_v_tqr_skip = gem_add(gem_v_tqr_skip, gem_int(1));
                        }
#line 574 "compiler/main.gem"
    GemVal _t368;
    if (!gem_truthy(gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length))) {
        _t368 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    } else {
        _t368 = gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
    }
    GemVal _t369;
    if (!gem_truthy(_t368)) {
        _t369 = _t368;
    } else {
        _t369 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("\""));
    }
    GemVal _t370;
    if (!gem_truthy(_t369)) {
        _t370 = _t369;
    } else {
        _t370 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("\""));
    }
                        if (gem_truthy(_t370)) {
#line 575 "compiler/main.gem"
                            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
#line 576 "compiler/main.gem"
    GemVal _t371[] = {gem_v_val};
    GemVal _t372;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t371, 1), gem_int(0)))) {
        _t372 = gem_gt(gem_len_fn(NULL, _t371, 1), gem_int(0));
    } else {
        _t372 = gem_eq(gem_table_get(gem_v_val, gem_neg(gem_int(1))), gem_string("\n"));
    }
                            if (gem_truthy(_t372)) {
#line 577 "compiler/main.gem"
    GemVal _t373[] = {gem_v_val};
    GemVal _t374[] = {gem_v_val, gem_int(0), gem_sub(gem_len_fn(NULL, _t373, 1), gem_int(1))};
                                gem_v_val = gem_substr_fn(NULL, _t374, 3);
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
    GemVal _t375[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t376 = gem_v__mod_lexer_compile_error;
                            (void)(_t376.fn(_t376.env, _t375, 7));
                        }
#line 597 "compiler/main.gem"
                        GemVal gem_v_tqre = gem_table_get(gem_v_source, gem_v_pos);
#line 598 "compiler/main.gem"
    GemVal _t377[] = {gem_v_tqre, gem_string("dq")};
                        GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t377, 2);
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
    GemVal _t378 = gem_table_new();
    gem_table_set(_t378, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t378, gem_string("value"), gem_v_val);
    gem_table_set(_t378, gem_string("line"), gem_v_line);
    gem_table_set(_t378, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t379[] = {gem_v_tokens, _t378};
                        (void)(gem_push_fn(NULL, _t379, 2));
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
    GemVal _t380[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated triple-quoted string interpolation"), GEM_NIL};
    GemVal _t381 = gem_v__mod_lexer_compile_error;
                        (void)(_t381.fn(_t381.env, _t380, 7));
                    }
#line 625 "compiler/main.gem"
    GemVal _t382 = gem_table_new();
    gem_table_set(_t382, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t382, gem_string("value"), gem_v_val);
    gem_table_set(_t382, gem_string("line"), gem_v_line);
    gem_table_set(_t382, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t383[] = {gem_v_tokens, _t382};
                    (void)(gem_push_fn(NULL, _t383, 2));
#line 626 "compiler/main.gem"
    GemVal _t384 = gem_table_new();
    gem_table_set(_t384, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t384, gem_string("value"), gem_string(""));
    gem_table_set(_t384, gem_string("line"), gem_v_line);
    gem_table_set(_t384, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t385[] = {gem_v_tokens, _t384};
                    (void)(gem_push_fn(NULL, _t385, 2));
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
                GemVal _t386;
                if (!gem_truthy(gem_lt(gem_v_pos, gem_v_length))) {
                                _t386 = gem_lt(gem_v_pos, gem_v_length);
                } else {
                                _t386 = gem_neq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\""));
                }
                if (!gem_truthy(_t386)) break;
#line 635 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_table_get(gem_v_source, gem_v_pos), gem_string("\\")))) {
#line 636 "compiler/main.gem"
                    gem_v_pos = gem_add(gem_v_pos, gem_int(1));
#line 637 "compiler/main.gem"
                    if (gem_truthy(gem_ge(gem_v_pos, gem_v_length))) {
#line 638 "compiler/main.gem"
    GemVal _t387[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string escape"), GEM_NIL};
    GemVal _t388 = gem_v__mod_lexer_compile_error;
                        (void)(_t388.fn(_t388.env, _t387, 7));
                    }
#line 640 "compiler/main.gem"
                    GemVal gem_v_esc = gem_table_get(gem_v_source, gem_v_pos);
#line 641 "compiler/main.gem"
    GemVal _t389[] = {gem_v_esc, gem_string("dq")};
                    GemVal gem_v_esc_result = gem_fn__mod_lexer_apply_escape(NULL, _t389, 2);
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
    GemVal _t390 = gem_table_new();
    gem_table_set(_t390, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t390, gem_string("value"), gem_v_val);
    gem_table_set(_t390, gem_string("line"), gem_v_line);
    gem_table_set(_t390, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t391[] = {gem_v_tokens, _t390};
                    (void)(gem_push_fn(NULL, _t391, 2));
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
    GemVal _t392[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_string("unterminated string interpolation"), GEM_NIL};
    GemVal _t393 = gem_v__mod_lexer_compile_error;
                    (void)(_t393.fn(_t393.env, _t392, 7));
                }
#line 667 "compiler/main.gem"
    GemVal _t394 = gem_table_new();
    gem_table_set(_t394, gem_string("type"), gem_string("STRING"));
    gem_table_set(_t394, gem_string("value"), gem_v_val);
    gem_table_set(_t394, gem_string("line"), gem_v_line);
    gem_table_set(_t394, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t395[] = {gem_v_tokens, _t394};
                (void)(gem_push_fn(NULL, _t395, 2));
#line 668 "compiler/main.gem"
    GemVal _t396 = gem_table_new();
    gem_table_set(_t396, gem_string("type"), gem_string("INTERP_END"));
    gem_table_set(_t396, gem_string("value"), gem_string(""));
    gem_table_set(_t396, gem_string("line"), gem_v_line);
    gem_table_set(_t396, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t397[] = {gem_v_tokens, _t396};
                (void)(gem_push_fn(NULL, _t397, 2));
#line 669 "compiler/main.gem"
                gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            }
            continue;
        }
#line 674 "compiler/main.gem"
    GemVal _t398;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string("(")))) {
        _t398 = gem_eq(gem_v_ch, gem_string("("));
    } else {
        _t398 = gem_eq(gem_v_ch, gem_string(")"));
    }
    GemVal _t399;
    if (gem_truthy(_t398)) {
        _t399 = _t398;
    } else {
        _t399 = gem_eq(gem_v_ch, gem_string("["));
    }
    GemVal _t400;
    if (gem_truthy(_t399)) {
        _t400 = _t399;
    } else {
        _t400 = gem_eq(gem_v_ch, gem_string("]"));
    }
    GemVal _t401;
    if (gem_truthy(_t400)) {
        _t401 = _t400;
    } else {
        _t401 = gem_eq(gem_v_ch, gem_string("{"));
    }
    GemVal _t402;
    if (gem_truthy(_t401)) {
        _t402 = _t401;
    } else {
        _t402 = gem_eq(gem_v_ch, gem_string("}"));
    }
        if (gem_truthy(_t402)) {
#line 675 "compiler/main.gem"
    GemVal _t403 = gem_table_new();
    gem_table_set(_t403, gem_string("type"), gem_v_ch);
    gem_table_set(_t403, gem_string("value"), gem_v_ch);
    gem_table_set(_t403, gem_string("line"), gem_v_line);
    gem_table_set(_t403, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t404[] = {gem_v_tokens, _t403};
            (void)(gem_push_fn(NULL, _t404, 2));
#line 676 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 679 "compiler/main.gem"
    GemVal _t405;
    if (!gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t405 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t405 = gem_lt(gem_add(gem_v_pos, gem_int(1)), gem_v_length);
    }
    GemVal _t406;
    if (!gem_truthy(_t405)) {
        _t406 = _t405;
    } else {
        _t406 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(1))), gem_string("."));
    }
    GemVal _t407;
    if (!gem_truthy(_t406)) {
        _t407 = _t406;
    } else {
        _t407 = gem_lt(gem_add(gem_v_pos, gem_int(2)), gem_v_length);
    }
    GemVal _t408;
    if (!gem_truthy(_t407)) {
        _t408 = _t407;
    } else {
        _t408 = gem_eq(gem_table_get(gem_v_source, gem_add(gem_v_pos, gem_int(2))), gem_string("."));
    }
        if (gem_truthy(_t408)) {
#line 680 "compiler/main.gem"
    GemVal _t409 = gem_table_new();
    gem_table_set(_t409, gem_string("type"), gem_string("..."));
    gem_table_set(_t409, gem_string("value"), gem_string("..."));
    gem_table_set(_t409, gem_string("line"), gem_v_line);
    gem_table_set(_t409, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t410[] = {gem_v_tokens, _t409};
            (void)(gem_push_fn(NULL, _t410, 2));
#line 681 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(3));
            continue;
        }
#line 684 "compiler/main.gem"
    GemVal _t411;
    if (gem_truthy(gem_eq(gem_v_ch, gem_string(".")))) {
        _t411 = gem_eq(gem_v_ch, gem_string("."));
    } else {
        _t411 = gem_eq(gem_v_ch, gem_string(","));
    }
    GemVal _t412;
    if (gem_truthy(_t411)) {
        _t412 = _t411;
    } else {
        _t412 = gem_eq(gem_v_ch, gem_string(":"));
    }
    GemVal _t413;
    if (gem_truthy(_t412)) {
        _t413 = _t412;
    } else {
        _t413 = gem_eq(gem_v_ch, gem_string("|"));
    }
        if (gem_truthy(_t413)) {
#line 685 "compiler/main.gem"
    GemVal _t414 = gem_table_new();
    gem_table_set(_t414, gem_string("type"), gem_v_ch);
    gem_table_set(_t414, gem_string("value"), gem_v_ch);
    gem_table_set(_t414, gem_string("line"), gem_v_line);
    gem_table_set(_t414, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t415[] = {gem_v_tokens, _t414};
            (void)(gem_push_fn(NULL, _t415, 2));
#line 686 "compiler/main.gem"
            gem_v_pos = gem_add(gem_v_pos, gem_int(1));
            continue;
        }
#line 690 "compiler/main.gem"
    GemVal _t416[] = {gem_string("unexpected character: '"), gem_v_ch, gem_string("'")};
    GemVal _t417[] = {gem_v_source, gem_v_file, gem_v_line, gem_add(gem_sub(gem_v_pos, gem_v_line_start), gem_int(1)), gem_int(1), gem_interp(3, _t416), GEM_NIL};
    GemVal _t418 = gem_v__mod_lexer_compile_error;
        (void)(_t418.fn(_t418.env, _t417, 7));
    }
#line 693 "compiler/main.gem"
    GemVal _t419 = gem_table_new();
    gem_table_set(_t419, gem_string("type"), gem_string("EOF"));
    gem_table_set(_t419, gem_string("value"), gem_string(""));
    gem_table_set(_t419, gem_string("line"), gem_v_line);
    gem_table_set(_t419, gem_string("col"), gem_add(gem_sub(gem_v_tstart, gem_v_line_start), gem_int(1)));
    GemVal _t420[] = {gem_v_tokens, _t419};
    (void)(gem_push_fn(NULL, _t420, 2));
    GemVal _t421 = gem_v_tokens;
    gem_pop_frame();
    return _t421;
}

static GemVal gem_fn__mod_parser_digit_val(void *_env, GemVal *args, int argc) {
#line 12 "compiler/main.gem"
    GemVal gem_v_ch = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_parser_digit_val", "compiler/main.gem", 12);
#line 13 "compiler/main.gem"
    GemVal gem_v__match_1 = gem_v_ch;
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("0")))) {
        GemVal _t422 = gem_int(0);
        gem_pop_frame();
        return _t422;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("1")))) {
        GemVal _t423 = gem_int(1);
        gem_pop_frame();
        return _t423;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("2")))) {
        GemVal _t424 = gem_int(2);
        gem_pop_frame();
        return _t424;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("3")))) {
        GemVal _t425 = gem_int(3);
        gem_pop_frame();
        return _t425;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("4")))) {
        GemVal _t426 = gem_int(4);
        gem_pop_frame();
        return _t426;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("5")))) {
        GemVal _t427 = gem_int(5);
        gem_pop_frame();
        return _t427;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("6")))) {
        GemVal _t428 = gem_int(6);
        gem_pop_frame();
        return _t428;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("7")))) {
        GemVal _t429 = gem_int(7);
        gem_pop_frame();
        return _t429;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("8")))) {
        GemVal _t430 = gem_int(8);
        gem_pop_frame();
        return _t430;
    } else {
    if (gem_truthy(gem_eq(gem_v__match_1, gem_string("9")))) {
        GemVal _t431 = gem_int(9);
        gem_pop_frame();
        return _t431;
    } else {
#line 35 "compiler/main.gem"
    GemVal _t432[] = {gem_string("not a digit: "), gem_v_ch};
    GemVal _t433[] = {gem_interp(2, _t432)};
        GemVal _t434 = gem_error_at_fn("compiler/main.gem", 35, _t433, 1);
        gem_pop_frame();
        return _t434;
    }
    }
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
    GemVal _t435[] = {gem_v_s};
    GemVal gem_v__for_limit_2 = gem_len_fn(NULL, _t435, 1);
#line 41 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_limit_2))) break;
#line 41 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_2;
#line 41 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 42 "compiler/main.gem"
    GemVal _t436[] = {gem_table_get(gem_v_s, gem_v_i)};
        gem_v_result = gem_add(gem_mul(gem_v_result, gem_int(10)), gem_fn__mod_parser_digit_val(NULL, _t436, 1));
    }

    GemVal _t437 = gem_v_result;
    gem_pop_frame();
    return _t437;
}

static GemVal gem_fn__mod_parser_contains_dot(void *_env, GemVal *args, int argc) {
#line 47 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_parser_contains_dot", "compiler/main.gem", 47);
#line 48 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 48 "compiler/main.gem"
    GemVal _t438[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t438, 1);
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
            GemVal _t439 = gem_bool(1);
            gem_pop_frame();
            return _t439;
        }
    }

    GemVal _t440 = gem_bool(0);
    gem_pop_frame();
    return _t440;
}

static GemVal gem_fn__mod_parser_parse_source(void *_env, GemVal *args, int argc) {
#line 58 "compiler/main.gem"
    GemVal gem_v_source = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_file = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_parser_parse_source", "compiler/main.gem", 58);
#line 59 "compiler/main.gem"
    GemVal _t441[] = {gem_v_source, gem_v_file};
    GemVal _t442 = gem_v__mod_parser_tokenize;
    GemVal gem_v_tokens = _t442.fn(_t442.env, _t441, 2);
#line 60 "compiler/main.gem"
    GemVal _t443[] = {gem_v_tokens, gem_v_source, gem_v_file};
    GemVal gem_v_p = gem_fn__mod_parser_make_parser(NULL, _t443, 3);
#line 61 "compiler/main.gem"
    GemVal _t444 = gem_v_p;
    static GemICacheSlot _ic_2 = {0};
    GemVal _t445 = gem_table_get_cached(_t444, "parse", &_ic_2);
    GemVal _t446 = _t445.fn(_t445.env, NULL, 0);
    gem_pop_frame();
    return _t446;
}

struct _closure__anon_5 {
    intptr_t _num_captures;
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_5(void *_env, GemVal *args, int argc) {
    struct _closure__anon_5 *_cls = (struct _closure__anon_5 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_5", "compiler/main.gem", 0);
    GemVal _t447 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    gem_pop_frame();
    return _t447;
}

struct _closure__anon_6 {
    intptr_t _num_captures;
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_6(void *_env, GemVal *args, int argc) {
    struct _closure__anon_6 *_cls = (struct _closure__anon_6 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal gem_v_n = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_6", "compiler/main.gem", 0);
    GemVal _t449 = gem_table_get((*gem_v_tokens), gem_add((*gem_v_pos), gem_v_n));
    gem_pop_frame();
    return _t449;
}

struct _closure__anon_7 {
    intptr_t _num_captures;
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_7(void *_env, GemVal *args, int argc) {
    struct _closure__anon_7 *_cls = (struct _closure__anon_7 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_7", "compiler/main.gem", 0);
#line 82 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 83 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t451 = gem_v_t;
    gem_pop_frame();
    return _t451;
}

struct _closure__anon_8 {
    intptr_t _num_captures;
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_8(void *_env, GemVal *args, int argc) {
    struct _closure__anon_8 *_cls = (struct _closure__anon_8 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_8", "compiler/main.gem", 0);
    GemVal _t453 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
    static GemICacheSlot _ic_3 = {0};
    GemVal _t454 = gem_eq(gem_table_get_cached(_t453, "type", &_ic_3), gem_string("EOF"));
    gem_pop_frame();
    return _t454;
}

struct _closure__anon_9 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v_file;
    GemVal *gem_v_pos;
    GemVal *gem_v_source;
    GemVal *gem_v_tokens;
};
static GemVal _anon_9(void *_env, GemVal *args, int argc) {
    struct _closure__anon_9 *_cls = (struct _closure__anon_9 *)_env;
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_source = _cls->gem_v_source;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    GemVal gem_v_tp = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_9", "compiler/main.gem", 0);
#line 92 "compiler/main.gem"
    GemVal gem_v_t = gem_table_get((*gem_v_tokens), (*gem_v_pos));
#line 93 "compiler/main.gem"
    GemVal _t456 = gem_v_t;
    static GemICacheSlot _ic_4 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t456, "type", &_ic_4), gem_v_tp))) {
#line 94 "compiler/main.gem"
    GemVal _t457 = gem_v_t;
    static GemICacheSlot _ic_5 = {0};
    GemVal _t458 = gem_v_t;
    static GemICacheSlot _ic_6 = {0};
    GemVal _t459 = gem_v_t;
    static GemICacheSlot _ic_7 = {0};
    GemVal _t460[] = {gem_table_get_cached(_t459, "value", &_ic_7)};
    GemVal _t461 = gem_v_t;
    static GemICacheSlot _ic_8 = {0};
    GemVal _t462[] = {gem_string("expected '"), gem_v_tp, gem_string("' but got '"), gem_table_get_cached(_t461, "type", &_ic_8), gem_string("'")};
    GemVal _t463[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t457, "line", &_ic_5), gem_table_get_cached(_t458, "col", &_ic_6), gem_len_fn(NULL, _t460, 1), gem_interp(5, _t462), GEM_NIL};
    GemVal _t464 = (*gem_v__mod_parser_compile_error);
        (void)(_t464.fn(_t464.env, _t463, 7));
    }
#line 96 "compiler/main.gem"
    *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    GemVal _t465 = gem_v_t;
    gem_pop_frame();
    return _t465;
}

struct _closure__anon_10 {
    intptr_t _num_captures;
    GemVal *gem_v_pos;
    GemVal *gem_v_tokens;
};
static GemVal _anon_10(void *_env, GemVal *args, int argc) {
    struct _closure__anon_10 *_cls = (struct _closure__anon_10 *)_env;
    GemVal *gem_v_pos = _cls->gem_v_pos;
    GemVal *gem_v_tokens = _cls->gem_v_tokens;
    gem_push_frame("_anon_10", "compiler/main.gem", 0);
#line 101 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t467 = gem_table_get((*gem_v_tokens), (*gem_v_pos));
        static GemICacheSlot _ic_9 = {0};
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t467, "type", &_ic_9), gem_string("NEWLINE")))) break;
#line 102 "compiler/main.gem"
        *gem_v_pos = gem_add((*gem_v_pos), gem_int(1));
    }
    gem_pop_frame();
    return GEM_NIL;
}

struct _closure__anon_11 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_compile_error;
    GemVal *gem_v_advance;
    GemVal *gem_v_expect;
    GemVal *gem_v_file;
    GemVal *gem_v_parse_expr;
    GemVal *gem_v_peek;
    GemVal *gem_v_source;
};
static GemVal _anon_11(void *_env, GemVal *args, int argc) {
    struct _closure__anon_11 *_cls = (struct _closure__anon_11 *)_env;
    GemVal *gem_v__mod_parser_compile_error = _cls->gem_v__mod_parser_compile_error;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_expect = _cls->gem_v_expect;
    GemVal *gem_v_file = _cls->gem_v_file;
    GemVal *gem_v_parse_expr = _cls->gem_v_parse_expr;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_source = _cls->gem_v_source;
    gem_push_frame("_anon_11", "compiler/main.gem", 0);
#line 121 "compiler/main.gem"
    GemVal _t469 = gem_table_new();
    GemVal gem_v_params = _t469;
#line 122 "compiler/main.gem"
    GemVal gem_v_rest_param = GEM_NIL;
#line 123 "compiler/main.gem"
    GemVal _t470 = gem_table_new();
    GemVal gem_v_defaults = _t470;
#line 124 "compiler/main.gem"
    GemVal gem_v_has_default = gem_bool(0);
#line 126 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t471 = (*gem_v_peek);
        GemVal _t472 = _t471.fn(_t471.env, NULL, 0);
        static GemICacheSlot _ic_10 = {0};
        GemVal _t475;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t472, "type", &_ic_10), gem_string("NAME")))) {
                _t475 = gem_eq(gem_table_get_cached(_t472, "type", &_ic_10), gem_string("NAME"));
        } else {
                GemVal _t473 = (*gem_v_peek);
                GemVal _t474 = _t473.fn(_t473.env, NULL, 0);
                static GemICacheSlot _ic_11 = {0};
                _t475 = gem_eq(gem_table_get_cached(_t474, "type", &_ic_11), gem_string("..."));
        }
        if (!gem_truthy(_t475)) break;
#line 127 "compiler/main.gem"
    GemVal _t476 = (*gem_v_peek);
    GemVal _t477 = _t476.fn(_t476.env, NULL, 0);
    static GemICacheSlot _ic_12 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t477, "type", &_ic_12), gem_string("...")))) {
#line 128 "compiler/main.gem"
    GemVal _t478 = (*gem_v_advance);
            (void)(_t478.fn(_t478.env, NULL, 0));
#line 129 "compiler/main.gem"
    GemVal _t479[] = {gem_string("NAME")};
    GemVal _t480 = (*gem_v_expect);
    GemVal _t481 = _t480.fn(_t480.env, _t479, 1);
    static GemICacheSlot _ic_13 = {0};
            gem_v_rest_param = gem_table_get_cached(_t481, "value", &_ic_13);
            break;
        }
#line 132 "compiler/main.gem"
    GemVal _t482 = (*gem_v_advance);
        GemVal gem_v_name_tok = _t482.fn(_t482.env, NULL, 0);
#line 133 "compiler/main.gem"
    GemVal _t483 = gem_v_name_tok;
    static GemICacheSlot _ic_14 = {0};
    GemVal _t484[] = {gem_v_params, gem_table_get_cached(_t483, "value", &_ic_14)};
        (void)(gem_push_fn(NULL, _t484, 2));
#line 134 "compiler/main.gem"
    GemVal _t485 = (*gem_v_peek);
    GemVal _t486 = _t485.fn(_t485.env, NULL, 0);
    static GemICacheSlot _ic_15 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t486, "type", &_ic_15), gem_string("=")))) {
#line 135 "compiler/main.gem"
    GemVal _t487 = (*gem_v_advance);
            (void)(_t487.fn(_t487.env, NULL, 0));
#line 136 "compiler/main.gem"
    GemVal _t488 = (*gem_v_parse_expr);
    GemVal _t489[] = {gem_v_defaults, _t488.fn(_t488.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t489, 2));
#line 137 "compiler/main.gem"
            gem_v_has_default = gem_bool(1);
        } else {
#line 139 "compiler/main.gem"
            if (gem_truthy(gem_v_has_default)) {
#line 140 "compiler/main.gem"
    GemVal _t490 = gem_v_name_tok;
    static GemICacheSlot _ic_16 = {0};
    GemVal _t491 = gem_v_name_tok;
    static GemICacheSlot _ic_17 = {0};
    GemVal _t492 = gem_v_name_tok;
    static GemICacheSlot _ic_18 = {0};
    GemVal _t493[] = {gem_table_get_cached(_t492, "value", &_ic_18)};
    GemVal _t494[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t490, "line", &_ic_16), gem_table_get_cached(_t491, "col", &_ic_17), gem_len_fn(NULL, _t493, 1), gem_string("required parameter after optional parameter"), GEM_NIL};
    GemVal _t495 = (*gem_v__mod_parser_compile_error);
                (void)(_t495.fn(_t495.env, _t494, 7));
            }
#line 142 "compiler/main.gem"
    GemVal _t496[] = {gem_v_defaults, GEM_NIL};
            (void)(gem_push_fn(NULL, _t496, 2));
        }
#line 144 "compiler/main.gem"
    GemVal _t497 = (*gem_v_peek);
    GemVal _t498 = _t497.fn(_t497.env, NULL, 0);
    static GemICacheSlot _ic_19 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t498, "type", &_ic_19), gem_string(",")))) {
            break;
        }
#line 147 "compiler/main.gem"
    GemVal _t499 = (*gem_v_advance);
        (void)(_t499.fn(_t499.env, NULL, 0));
    }
    GemVal _t500 = gem_table_new();
    gem_table_set(_t500, gem_string("params"), gem_v_params);
    gem_table_set(_t500, gem_string("rest_param"), gem_v_rest_param);
    gem_table_set(_t500, gem_string("defaults"), gem_v_defaults);
    GemVal _t501 = _t500;
    gem_pop_frame();
    return _t501;
}

struct _closure__anon_12 {
    intptr_t _num_captures;
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_peek;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_12(void *_env, GemVal *args, int argc) {
    struct _closure__anon_12 *_cls = (struct _closure__anon_12 *)_env;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_12", "compiler/main.gem", 0);
#line 158 "compiler/main.gem"
    GemVal _t503 = gem_table_new();
    GemVal gem_v_stmts = _t503;
#line 159 "compiler/main.gem"
    GemVal _t504 = (*gem_v_skip_nl);
    (void)(_t504.fn(_t504.env, NULL, 0));
#line 160 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t505 = (*gem_v_peek);
        GemVal _t506 = _t505.fn(_t505.env, NULL, 0);
        static GemICacheSlot _ic_20 = {0};
        GemVal _t509;
        if (!gem_truthy(gem_neq(gem_table_get_cached(_t506, "type", &_ic_20), gem_string("end")))) {
                _t509 = gem_neq(gem_table_get_cached(_t506, "type", &_ic_20), gem_string("end"));
        } else {
                GemVal _t507 = (*gem_v_peek);
                GemVal _t508 = _t507.fn(_t507.env, NULL, 0);
                static GemICacheSlot _ic_21 = {0};
                _t509 = gem_neq(gem_table_get_cached(_t508, "type", &_ic_21), gem_string("elif"));
        }
        GemVal _t512;
        if (!gem_truthy(_t509)) {
                _t512 = _t509;
        } else {
                GemVal _t510 = (*gem_v_peek);
                GemVal _t511 = _t510.fn(_t510.env, NULL, 0);
                static GemICacheSlot _ic_22 = {0};
                _t512 = gem_neq(gem_table_get_cached(_t511, "type", &_ic_22), gem_string("else"));
        }
        GemVal _t515;
        if (!gem_truthy(_t512)) {
                _t515 = _t512;
        } else {
                GemVal _t513 = (*gem_v_peek);
                GemVal _t514 = _t513.fn(_t513.env, NULL, 0);
                static GemICacheSlot _ic_23 = {0};
                _t515 = gem_neq(gem_table_get_cached(_t514, "type", &_ic_23), gem_string("when"));
        }
        GemVal _t518;
        if (!gem_truthy(_t515)) {
                _t518 = _t515;
        } else {
                GemVal _t516 = (*gem_v_peek);
                GemVal _t517 = _t516.fn(_t516.env, NULL, 0);
                static GemICacheSlot _ic_24 = {0};
                _t518 = gem_neq(gem_table_get_cached(_t517, "type", &_ic_24), gem_string("after"));
        }
        GemVal _t521;
        if (!gem_truthy(_t518)) {
                _t521 = _t518;
        } else {
                GemVal _t519 = (*gem_v_peek);
                GemVal _t520 = _t519.fn(_t519.env, NULL, 0);
                static GemICacheSlot _ic_25 = {0};
                _t521 = gem_neq(gem_table_get_cached(_t520, "type", &_ic_25), gem_string("then"));
        }
        GemVal _t523;
        if (!gem_truthy(_t521)) {
                _t523 = _t521;
        } else {
                GemVal _t522 = (*gem_v_at_end);
                _t523 = gem_not(_t522.fn(_t522.env, NULL, 0));
        }
        if (!gem_truthy(_t523)) break;
#line 161 "compiler/main.gem"
    GemVal _t524 = (*gem_v_parse_stmt);
    GemVal _t525[] = {gem_v_stmts, _t524.fn(_t524.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t525, 2));
#line 162 "compiler/main.gem"
    GemVal _t526 = (*gem_v_skip_nl);
        (void)(_t526.fn(_t526.env, NULL, 0));
    }
    GemVal _t527 = gem_v_stmts;
    gem_pop_frame();
    return _t527;
}

struct _closure__anon_13 {
    intptr_t _num_captures;
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_peek;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_13(void *_env, GemVal *args, int argc) {
    struct _closure__anon_13 *_cls = (struct _closure__anon_13 *)_env;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_13", "compiler/main.gem", 0);
#line 171 "compiler/main.gem"
    GemVal _t529 = gem_table_new();
    GemVal gem_v_stmts = _t529;
#line 172 "compiler/main.gem"
    GemVal _t530 = (*gem_v_skip_nl);
    (void)(_t530.fn(_t530.env, NULL, 0));
#line 173 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t531 = (*gem_v_peek);
        GemVal _t532 = _t531.fn(_t531.env, NULL, 0);
        static GemICacheSlot _ic_26 = {0};
        GemVal _t534;
        if (!gem_truthy(gem_neq(gem_table_get_cached(_t532, "type", &_ic_26), gem_string("end")))) {
                _t534 = gem_neq(gem_table_get_cached(_t532, "type", &_ic_26), gem_string("end"));
        } else {
                GemVal _t533 = (*gem_v_at_end);
                _t534 = gem_not(_t533.fn(_t533.env, NULL, 0));
        }
        if (!gem_truthy(_t534)) break;
#line 174 "compiler/main.gem"
    GemVal _t535 = (*gem_v_parse_stmt);
    GemVal _t536[] = {gem_v_stmts, _t535.fn(_t535.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t536, 2));
#line 175 "compiler/main.gem"
    GemVal _t537 = (*gem_v_skip_nl);
        (void)(_t537.fn(_t537.env, NULL, 0));
    }
    GemVal _t538 = gem_v_stmts;
    gem_pop_frame();
    return _t538;
}

struct _closure__anon_14 {
    intptr_t _num_captures;
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
static GemVal _anon_14(void *_env, GemVal *args, int argc) {
    struct _closure__anon_14 *_cls = (struct _closure__anon_14 *)_env;
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
    gem_push_frame("_anon_14", "compiler/main.gem", 0);
#line 184 "compiler/main.gem"
    GemVal _t540 = (*gem_v_peek);
    GemVal gem_v_t = _t540.fn(_t540.env, NULL, 0);
#line 187 "compiler/main.gem"
    GemVal _t541 = gem_v_t;
    static GemICacheSlot _ic_27 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t541, "type", &_ic_27), gem_string("NUMBER")))) {
#line 188 "compiler/main.gem"
    GemVal _t542 = (*gem_v_advance);
        (void)(_t542.fn(_t542.env, NULL, 0));
#line 189 "compiler/main.gem"
    GemVal _t543 = gem_v_t;
    static GemICacheSlot _ic_28 = {0};
    GemVal _t544[] = {gem_table_get_cached(_t543, "value", &_ic_28)};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t544, 1))) {
#line 190 "compiler/main.gem"
    GemVal _t545 = gem_v_t;
    static GemICacheSlot _ic_29 = {0};
    GemVal _t546[] = {gem_table_get_cached(_t545, "value", &_ic_29)};
    GemVal _t547[] = {gem_fn_atof(NULL, _t546, 1)};
    GemVal _t548 = (*gem_v__mod_parser_make_float);
            GemVal _t549 = _t548.fn(_t548.env, _t547, 1);
            gem_pop_frame();
            return _t549;
        }
#line 192 "compiler/main.gem"
    GemVal _t550 = gem_v_t;
    static GemICacheSlot _ic_30 = {0};
    GemVal _t551[] = {gem_table_get_cached(_t550, "value", &_ic_30)};
    GemVal _t552[] = {gem_fn__mod_parser_str_to_int(NULL, _t551, 1)};
    GemVal _t553 = (*gem_v__mod_parser_make_int);
        GemVal _t554 = _t553.fn(_t553.env, _t552, 1);
        gem_pop_frame();
        return _t554;
    }
#line 196 "compiler/main.gem"
    GemVal _t555 = gem_v_t;
    static GemICacheSlot _ic_31 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t555, "type", &_ic_31), gem_string("STRING")))) {
#line 197 "compiler/main.gem"
    GemVal _t556 = (*gem_v_advance);
        (void)(_t556.fn(_t556.env, NULL, 0));
#line 198 "compiler/main.gem"
    GemVal _t557 = gem_v_t;
    static GemICacheSlot _ic_32 = {0};
    GemVal _t558[] = {gem_table_get_cached(_t557, "value", &_ic_32)};
    GemVal _t559 = (*gem_v__mod_parser_make_string);
        GemVal _t560 = _t559.fn(_t559.env, _t558, 1);
        gem_pop_frame();
        return _t560;
    }
#line 202 "compiler/main.gem"
    GemVal _t561 = gem_v_t;
    static GemICacheSlot _ic_33 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t561, "type", &_ic_33), gem_string("INTERP_START")))) {
#line 203 "compiler/main.gem"
    GemVal _t562 = (*gem_v_advance);
        (void)(_t562.fn(_t562.env, NULL, 0));
#line 204 "compiler/main.gem"
    GemVal _t563 = gem_table_new();
        GemVal gem_v_parts = _t563;
#line 205 "compiler/main.gem"
    GemVal _t564 = (*gem_v_skip_nl);
        (void)(_t564.fn(_t564.env, NULL, 0));
#line 206 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t565 = (*gem_v_peek);
            GemVal _t566 = _t565.fn(_t565.env, NULL, 0);
            static GemICacheSlot _ic_34 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t566, "type", &_ic_34), gem_string("INTERP_END")))) break;
#line 207 "compiler/main.gem"
    GemVal _t567 = (*gem_v_peek);
    GemVal _t568 = _t567.fn(_t567.env, NULL, 0);
    static GemICacheSlot _ic_35 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t568, "type", &_ic_35), gem_string("STRING")))) {
#line 208 "compiler/main.gem"
    GemVal _t569 = (*gem_v_advance);
    GemVal _t570 = _t569.fn(_t569.env, NULL, 0);
    static GemICacheSlot _ic_36 = {0};
                GemVal gem_v_sval = gem_table_get_cached(_t570, "value", &_ic_36);
#line 209 "compiler/main.gem"
    GemVal _t571[] = {gem_v_sval};
                if (gem_truthy(gem_gt(gem_len_fn(NULL, _t571, 1), gem_int(0)))) {
#line 210 "compiler/main.gem"
    GemVal _t572[] = {gem_v_sval};
    GemVal _t573 = (*gem_v__mod_parser_make_string);
    GemVal _t574[] = {gem_v_parts, _t573.fn(_t573.env, _t572, 1)};
                    (void)(gem_push_fn(NULL, _t574, 2));
                }
            } else {
#line 213 "compiler/main.gem"
    GemVal _t575 = (*gem_v_parse_expr);
    GemVal _t576[] = {gem_v_parts, _t575.fn(_t575.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t576, 2));
            }
#line 215 "compiler/main.gem"
    GemVal _t577 = (*gem_v_skip_nl);
            (void)(_t577.fn(_t577.env, NULL, 0));
        }
#line 217 "compiler/main.gem"
    GemVal _t578[] = {gem_string("INTERP_END")};
    GemVal _t579 = (*gem_v_expect);
        (void)(_t579.fn(_t579.env, _t578, 1));
#line 218 "compiler/main.gem"
    GemVal _t580[] = {gem_v_parts};
    GemVal _t581 = (*gem_v__mod_parser_make_interp);
        GemVal _t582 = _t581.fn(_t581.env, _t580, 1);
        gem_pop_frame();
        return _t582;
    }
#line 222 "compiler/main.gem"
    GemVal _t583 = gem_v_t;
    static GemICacheSlot _ic_37 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t583, "type", &_ic_37), gem_string("true")))) {
#line 223 "compiler/main.gem"
    GemVal _t584 = (*gem_v_advance);
        (void)(_t584.fn(_t584.env, NULL, 0));
#line 224 "compiler/main.gem"
    GemVal _t585[] = {gem_bool(1)};
    GemVal _t586 = (*gem_v__mod_parser_make_bool);
        GemVal _t587 = _t586.fn(_t586.env, _t585, 1);
        gem_pop_frame();
        return _t587;
    }
#line 226 "compiler/main.gem"
    GemVal _t588 = gem_v_t;
    static GemICacheSlot _ic_38 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t588, "type", &_ic_38), gem_string("false")))) {
#line 227 "compiler/main.gem"
    GemVal _t589 = (*gem_v_advance);
        (void)(_t589.fn(_t589.env, NULL, 0));
#line 228 "compiler/main.gem"
    GemVal _t590[] = {gem_bool(0)};
    GemVal _t591 = (*gem_v__mod_parser_make_bool);
        GemVal _t592 = _t591.fn(_t591.env, _t590, 1);
        gem_pop_frame();
        return _t592;
    }
#line 232 "compiler/main.gem"
    GemVal _t593 = gem_v_t;
    static GemICacheSlot _ic_39 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t593, "type", &_ic_39), gem_string("nil")))) {
#line 233 "compiler/main.gem"
    GemVal _t594 = (*gem_v_advance);
        (void)(_t594.fn(_t594.env, NULL, 0));
#line 234 "compiler/main.gem"
    GemVal _t595 = (*gem_v__mod_parser_make_nil_node);
        GemVal _t596 = _t595.fn(_t595.env, NULL, 0);
        gem_pop_frame();
        return _t596;
    }
#line 238 "compiler/main.gem"
    GemVal _t597 = gem_v_t;
    static GemICacheSlot _ic_40 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t597, "type", &_ic_40), gem_string("fn")))) {
#line 239 "compiler/main.gem"
    GemVal _t598 = (*gem_v_advance);
        (void)(_t598.fn(_t598.env, NULL, 0));
#line 240 "compiler/main.gem"
    GemVal _t599[] = {gem_string("(")};
    GemVal _t600 = (*gem_v_expect);
        (void)(_t600.fn(_t600.env, _t599, 1));
#line 241 "compiler/main.gem"
    GemVal _t601 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t601.fn(_t601.env, NULL, 0);
#line 242 "compiler/main.gem"
    GemVal _t602[] = {gem_string(")")};
    GemVal _t603 = (*gem_v_expect);
        (void)(_t603.fn(_t603.env, _t602, 1));
#line 243 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 244 "compiler/main.gem"
    GemVal _t604 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t604.fn(_t604.env, NULL, 0);
#line 245 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 246 "compiler/main.gem"
    GemVal _t605[] = {gem_string("end")};
    GemVal _t606 = (*gem_v_expect);
        (void)(_t606.fn(_t606.env, _t605, 1));
#line 247 "compiler/main.gem"
    GemVal _t607 = gem_v_pr;
    static GemICacheSlot _ic_41 = {0};
    GemVal _t608 = gem_v_pr;
    static GemICacheSlot _ic_42 = {0};
    GemVal _t609 = gem_v_pr;
    static GemICacheSlot _ic_43 = {0};
    GemVal _t610[] = {gem_table_get_cached(_t607, "params", &_ic_41), gem_table_get_cached(_t608, "rest_param", &_ic_42), gem_v_body, gem_table_get_cached(_t609, "defaults", &_ic_43)};
    GemVal _t611 = (*gem_v__mod_parser_make_anon_fn);
        GemVal _t612 = _t611.fn(_t611.env, _t610, 4);
        gem_pop_frame();
        return _t612;
    }
#line 252 "compiler/main.gem"
    GemVal _t613 = gem_v_t;
    static GemICacheSlot _ic_44 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t613, "type", &_ic_44), gem_string("{")))) {
#line 253 "compiler/main.gem"
    GemVal _t614 = (*gem_v_advance);
        (void)(_t614.fn(_t614.env, NULL, 0));
#line 254 "compiler/main.gem"
    GemVal _t615 = (*gem_v_skip_nl);
        (void)(_t615.fn(_t615.env, NULL, 0));
#line 255 "compiler/main.gem"
    GemVal _t616 = gem_table_new();
        GemVal gem_v_entries = _t616;
#line 256 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t617 = (*gem_v_peek);
            GemVal _t618 = _t617.fn(_t617.env, NULL, 0);
            static GemICacheSlot _ic_45 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t618, "type", &_ic_45), gem_string("}")))) break;
#line 257 "compiler/main.gem"
    GemVal _t619 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t619.fn(_t619.env, NULL, 0);
#line 258 "compiler/main.gem"
    GemVal _t620 = gem_v_key_tok;
    static GemICacheSlot _ic_46 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t620, "type", &_ic_46), gem_string("NAME")))) {
#line 259 "compiler/main.gem"
    GemVal _t621 = (*gem_v_advance);
                (void)(_t621.fn(_t621.env, NULL, 0));
            } else {
#line 260 "compiler/main.gem"
    GemVal _t622 = (*gem_v_at_end);
    GemVal _t626;
    if (!gem_truthy(gem_not(_t622.fn(_t622.env, NULL, 0)))) {
        _t626 = gem_not(_t622.fn(_t622.env, NULL, 0));
    } else {
        GemVal _t623[] = {gem_int(1)};
        GemVal _t624 = (*gem_v_peek_at);
        GemVal _t625 = _t624.fn(_t624.env, _t623, 1);
        static GemICacheSlot _ic_47 = {0};
        _t626 = gem_eq(gem_table_get_cached(_t625, "type", &_ic_47), gem_string(":"));
    }
                if (gem_truthy(_t626)) {
#line 262 "compiler/main.gem"
    GemVal _t627 = (*gem_v_advance);
                    (void)(_t627.fn(_t627.env, NULL, 0));
                } else {
#line 264 "compiler/main.gem"
    GemVal _t628 = gem_v_key_tok;
    static GemICacheSlot _ic_48 = {0};
    GemVal _t629 = gem_v_key_tok;
    static GemICacheSlot _ic_49 = {0};
    GemVal _t630 = gem_v_key_tok;
    static GemICacheSlot _ic_50 = {0};
    GemVal _t631[] = {gem_table_get_cached(_t630, "value", &_ic_50)};
    GemVal _t632 = gem_v_key_tok;
    static GemICacheSlot _ic_51 = {0};
    GemVal _t633[] = {gem_string("expected '}' or table key but got '"), gem_table_get_cached(_t632, "type", &_ic_51), gem_string("'")};
    GemVal _t634[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t628, "line", &_ic_48), gem_table_get_cached(_t629, "col", &_ic_49), gem_len_fn(NULL, _t631, 1), gem_interp(3, _t633), GEM_NIL};
    GemVal _t635 = (*gem_v__mod_parser_compile_error);
                    (void)(_t635.fn(_t635.env, _t634, 7));
                }
            }
#line 266 "compiler/main.gem"
    GemVal _t636[] = {gem_string(":")};
    GemVal _t637 = (*gem_v_expect);
            (void)(_t637.fn(_t637.env, _t636, 1));
#line 267 "compiler/main.gem"
    GemVal _t638 = (*gem_v_parse_expr);
            GemVal gem_v_val = _t638.fn(_t638.env, NULL, 0);
#line 268 "compiler/main.gem"
    GemVal _t639 = gem_v_key_tok;
    static GemICacheSlot _ic_52 = {0};
    GemVal _t640[] = {gem_table_get_cached(_t639, "value", &_ic_52), gem_v_val};
    GemVal _t641 = (*gem_v__mod_parser_make_table_entry);
    GemVal _t642[] = {gem_v_entries, _t641.fn(_t641.env, _t640, 2)};
            (void)(gem_push_fn(NULL, _t642, 2));
#line 269 "compiler/main.gem"
    GemVal _t643 = (*gem_v_skip_nl);
            (void)(_t643.fn(_t643.env, NULL, 0));
#line 270 "compiler/main.gem"
    GemVal _t644 = (*gem_v_peek);
    GemVal _t645 = _t644.fn(_t644.env, NULL, 0);
    static GemICacheSlot _ic_53 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t645, "type", &_ic_53), gem_string(",")))) {
#line 271 "compiler/main.gem"
    GemVal _t646 = (*gem_v_advance);
                (void)(_t646.fn(_t646.env, NULL, 0));
#line 272 "compiler/main.gem"
    GemVal _t647 = (*gem_v_skip_nl);
                (void)(_t647.fn(_t647.env, NULL, 0));
            }
        }
#line 275 "compiler/main.gem"
    GemVal _t648[] = {gem_string("}")};
    GemVal _t649 = (*gem_v_expect);
        (void)(_t649.fn(_t649.env, _t648, 1));
#line 276 "compiler/main.gem"
    GemVal _t650[] = {gem_v_entries};
    GemVal _t651 = (*gem_v__mod_parser_make_table);
        GemVal _t652 = _t651.fn(_t651.env, _t650, 1);
        gem_pop_frame();
        return _t652;
    }
#line 280 "compiler/main.gem"
    GemVal _t653 = gem_v_t;
    static GemICacheSlot _ic_54 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t653, "type", &_ic_54), gem_string("[")))) {
#line 281 "compiler/main.gem"
    GemVal _t654 = (*gem_v_advance);
        (void)(_t654.fn(_t654.env, NULL, 0));
#line 282 "compiler/main.gem"
    GemVal _t655 = (*gem_v_skip_nl);
        (void)(_t655.fn(_t655.env, NULL, 0));
#line 283 "compiler/main.gem"
    GemVal _t656 = gem_table_new();
        GemVal gem_v_elements = _t656;
#line 284 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t657 = (*gem_v_peek);
            GemVal _t658 = _t657.fn(_t657.env, NULL, 0);
            static GemICacheSlot _ic_55 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t658, "type", &_ic_55), gem_string("]")))) break;
#line 285 "compiler/main.gem"
    GemVal _t659 = (*gem_v_parse_expr);
    GemVal _t660[] = {gem_v_elements, _t659.fn(_t659.env, NULL, 0)};
            (void)(gem_push_fn(NULL, _t660, 2));
#line 286 "compiler/main.gem"
    GemVal _t661 = (*gem_v_skip_nl);
            (void)(_t661.fn(_t661.env, NULL, 0));
#line 287 "compiler/main.gem"
    GemVal _t662 = (*gem_v_peek);
    GemVal _t663 = _t662.fn(_t662.env, NULL, 0);
    static GemICacheSlot _ic_56 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t663, "type", &_ic_56), gem_string(",")))) {
#line 288 "compiler/main.gem"
    GemVal _t664 = (*gem_v_advance);
                (void)(_t664.fn(_t664.env, NULL, 0));
#line 289 "compiler/main.gem"
    GemVal _t665 = (*gem_v_skip_nl);
                (void)(_t665.fn(_t665.env, NULL, 0));
            }
        }
#line 292 "compiler/main.gem"
    GemVal _t666[] = {gem_string("]")};
    GemVal _t667 = (*gem_v_expect);
        (void)(_t667.fn(_t667.env, _t666, 1));
#line 293 "compiler/main.gem"
    GemVal _t668[] = {gem_v_elements};
    GemVal _t669 = (*gem_v__mod_parser_make_array);
        GemVal _t670 = _t669.fn(_t669.env, _t668, 1);
        gem_pop_frame();
        return _t670;
    }
#line 297 "compiler/main.gem"
    GemVal _t671 = gem_v_t;
    static GemICacheSlot _ic_57 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t671, "type", &_ic_57), gem_string("(")))) {
#line 298 "compiler/main.gem"
    GemVal _t672 = (*gem_v_advance);
        (void)(_t672.fn(_t672.env, NULL, 0));
#line 299 "compiler/main.gem"
    GemVal _t673 = (*gem_v_parse_expr);
        GemVal gem_v_e = _t673.fn(_t673.env, NULL, 0);
#line 300 "compiler/main.gem"
    GemVal _t674[] = {gem_string(")")};
    GemVal _t675 = (*gem_v_expect);
        (void)(_t675.fn(_t675.env, _t674, 1));
#line 301 "compiler/main.gem"
        GemVal _t676 = gem_v_e;
        gem_pop_frame();
        return _t676;
    }
#line 305 "compiler/main.gem"
    GemVal _t677 = gem_v_t;
    static GemICacheSlot _ic_58 = {0};
    GemVal _t679;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t677, "type", &_ic_58), gem_string("NAME")))) {
        _t679 = gem_eq(gem_table_get_cached(_t677, "type", &_ic_58), gem_string("NAME"));
    } else {
        GemVal _t678 = gem_v_t;
        static GemICacheSlot _ic_59 = {0};
        _t679 = gem_eq(gem_table_get_cached(_t678, "value", &_ic_59), gem_string("pcall"));
    }
    GemVal _t683;
    if (!gem_truthy(_t679)) {
        _t683 = _t679;
    } else {
        GemVal _t680[] = {gem_int(1)};
        GemVal _t681 = (*gem_v_peek_at);
        GemVal _t682 = _t681.fn(_t681.env, _t680, 1);
        static GemICacheSlot _ic_60 = {0};
        _t683 = gem_neq(gem_table_get_cached(_t682, "type", &_ic_60), gem_string("("));
    }
    if (gem_truthy(_t683)) {
#line 306 "compiler/main.gem"
    GemVal _t684 = (*gem_v_advance);
        (void)(_t684.fn(_t684.env, NULL, 0));
#line 307 "compiler/main.gem"
    GemVal _t685 = (*gem_v_parse_expr);
        GemVal gem_v_expr = _t685.fn(_t685.env, NULL, 0);
#line 308 "compiler/main.gem"
    GemVal _t686[] = {gem_string("pcall")};
    GemVal _t687 = (*gem_v__mod_parser_make_var);
    GemVal _t688 = gem_table_new();
    GemVal _t689 = gem_table_new();
    GemVal _t690 = gem_table_new();
    gem_table_set(_t690, gem_int(0), gem_v_expr);
    GemVal _t691 = gem_table_new();
    GemVal _t692[] = {_t689, GEM_NIL, _t690, _t691};
    GemVal _t693 = (*gem_v__mod_parser_make_anon_fn);
    gem_table_set(_t688, gem_int(0), _t693.fn(_t693.env, _t692, 4));
    GemVal _t694 = gem_v_t;
    static GemICacheSlot _ic_61 = {0};
    GemVal _t695[] = {_t687.fn(_t687.env, _t686, 1), _t688, gem_table_get_cached(_t694, "line", &_ic_61)};
    GemVal _t696 = (*gem_v__mod_parser_make_call);
        GemVal _t697 = _t696.fn(_t696.env, _t695, 3);
        gem_pop_frame();
        return _t697;
    }
#line 312 "compiler/main.gem"
    GemVal _t698 = gem_v_t;
    static GemICacheSlot _ic_62 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t698, "type", &_ic_62), gem_string("NAME")))) {
#line 313 "compiler/main.gem"
    GemVal _t699 = (*gem_v_advance);
        (void)(_t699.fn(_t699.env, NULL, 0));
#line 314 "compiler/main.gem"
    GemVal _t700 = gem_v_t;
    static GemICacheSlot _ic_63 = {0};
    GemVal _t701[] = {gem_table_get_cached(_t700, "value", &_ic_63)};
    GemVal _t702 = (*gem_v__mod_parser_make_var);
        GemVal _t703 = _t702.fn(_t702.env, _t701, 1);
        gem_pop_frame();
        return _t703;
    }
#line 317 "compiler/main.gem"
    GemVal _t704 = gem_v_t;
    static GemICacheSlot _ic_64 = {0};
    GemVal _t705 = gem_v_t;
    static GemICacheSlot _ic_65 = {0};
    GemVal _t706 = gem_v_t;
    static GemICacheSlot _ic_66 = {0};
    GemVal _t707[] = {gem_table_get_cached(_t706, "value", &_ic_66)};
    GemVal _t708 = gem_v_t;
    static GemICacheSlot _ic_67 = {0};
    GemVal _t709[] = {gem_string("unexpected token '"), gem_table_get_cached(_t708, "type", &_ic_67), gem_string("' (expected expression)")};
    GemVal _t710[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t704, "line", &_ic_64), gem_table_get_cached(_t705, "col", &_ic_65), gem_len_fn(NULL, _t707, 1), gem_interp(3, _t709), GEM_NIL};
    GemVal _t711 = (*gem_v__mod_parser_compile_error);
    GemVal _t712 = _t711.fn(_t711.env, _t710, 7);
    gem_pop_frame();
    return _t712;
}

struct _closure__anon_15 {
    intptr_t _num_captures;
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
static GemVal _anon_15(void *_env, GemVal *args, int argc) {
    struct _closure__anon_15 *_cls = (struct _closure__anon_15 *)_env;
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
    gem_push_frame("_anon_15", "compiler/main.gem", 0);
#line 322 "compiler/main.gem"
    GemVal _t714 = (*gem_v_parse_atom);
    GemVal gem_v_node = _t714.fn(_t714.env, NULL, 0);
#line 324 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_bool(1))) break;
#line 326 "compiler/main.gem"
    GemVal _t715 = (*gem_v_peek);
    GemVal _t716 = _t715.fn(_t715.env, NULL, 0);
    static GemICacheSlot _ic_68 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t716, "type", &_ic_68), gem_string("(")))) {
#line 327 "compiler/main.gem"
    GemVal _t717 = (*gem_v_peek);
    GemVal _t718 = _t717.fn(_t717.env, NULL, 0);
    static GemICacheSlot _ic_69 = {0};
            GemVal gem_v_call_line = gem_table_get_cached(_t718, "line", &_ic_69);
#line 328 "compiler/main.gem"
    GemVal _t719 = (*gem_v_advance);
            (void)(_t719.fn(_t719.env, NULL, 0));
#line 329 "compiler/main.gem"
    GemVal _t720 = (*gem_v_skip_nl);
            (void)(_t720.fn(_t720.env, NULL, 0));
#line 330 "compiler/main.gem"
    GemVal _t721 = gem_table_new();
            GemVal gem_v_args = _t721;
#line 331 "compiler/main.gem"
    GemVal _t722 = (*gem_v_peek);
    GemVal _t723 = _t722.fn(_t722.env, NULL, 0);
    static GemICacheSlot _ic_70 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t723, "type", &_ic_70), gem_string(")")))) {
#line 332 "compiler/main.gem"
    GemVal _t724 = (*gem_v_parse_expr);
    GemVal _t725[] = {gem_v_args, _t724.fn(_t724.env, NULL, 0)};
                (void)(gem_push_fn(NULL, _t725, 2));
#line 333 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t726 = (*gem_v_peek);
                    GemVal _t727 = _t726.fn(_t726.env, NULL, 0);
                    static GemICacheSlot _ic_71 = {0};
                    if (!gem_truthy(gem_eq(gem_table_get_cached(_t727, "type", &_ic_71), gem_string(",")))) break;
#line 334 "compiler/main.gem"
    GemVal _t728 = (*gem_v_advance);
                    (void)(_t728.fn(_t728.env, NULL, 0));
#line 335 "compiler/main.gem"
    GemVal _t729 = (*gem_v_skip_nl);
                    (void)(_t729.fn(_t729.env, NULL, 0));
#line 336 "compiler/main.gem"
    GemVal _t730 = (*gem_v_parse_expr);
    GemVal _t731[] = {gem_v_args, _t730.fn(_t730.env, NULL, 0)};
                    (void)(gem_push_fn(NULL, _t731, 2));
                }
            }
#line 339 "compiler/main.gem"
    GemVal _t732 = (*gem_v_skip_nl);
            (void)(_t732.fn(_t732.env, NULL, 0));
#line 340 "compiler/main.gem"
    GemVal _t733[] = {gem_string(")")};
    GemVal _t734 = (*gem_v_expect);
            (void)(_t734.fn(_t734.env, _t733, 1));
#line 343 "compiler/main.gem"
    GemVal _t735 = (*gem_v_peek);
    GemVal _t736 = _t735.fn(_t735.env, NULL, 0);
    static GemICacheSlot _ic_72 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t736, "type", &_ic_72), gem_string("do")))) {
#line 344 "compiler/main.gem"
    GemVal _t737 = (*gem_v_advance);
                (void)(_t737.fn(_t737.env, NULL, 0));
#line 345 "compiler/main.gem"
    GemVal _t738 = gem_table_new();
                GemVal gem_v_bparams = _t738;
#line 346 "compiler/main.gem"
                GemVal gem_v_brest = GEM_NIL;
#line 347 "compiler/main.gem"
    GemVal _t739 = gem_table_new();
                GemVal gem_v_bdefaults = _t739;
#line 348 "compiler/main.gem"
    GemVal _t740 = (*gem_v_peek);
    GemVal _t741 = _t740.fn(_t740.env, NULL, 0);
    static GemICacheSlot _ic_73 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t741, "type", &_ic_73), gem_string("|")))) {
#line 349 "compiler/main.gem"
    GemVal _t742 = (*gem_v_advance);
                    (void)(_t742.fn(_t742.env, NULL, 0));
#line 350 "compiler/main.gem"
    GemVal _t743 = (*gem_v_parse_params);
                    GemVal gem_v_bpr = _t743.fn(_t743.env, NULL, 0);
#line 351 "compiler/main.gem"
    GemVal _t744 = gem_v_bpr;
    static GemICacheSlot _ic_74 = {0};
                    gem_v_bparams = gem_table_get_cached(_t744, "params", &_ic_74);
#line 352 "compiler/main.gem"
    GemVal _t745 = gem_v_bpr;
    static GemICacheSlot _ic_75 = {0};
                    gem_v_brest = gem_table_get_cached(_t745, "rest_param", &_ic_75);
#line 353 "compiler/main.gem"
    GemVal _t746 = gem_v_bpr;
    static GemICacheSlot _ic_76 = {0};
                    gem_v_bdefaults = gem_table_get_cached(_t746, "defaults", &_ic_76);
#line 354 "compiler/main.gem"
    GemVal _t747[] = {gem_string("|")};
    GemVal _t748 = (*gem_v_expect);
                    (void)(_t748.fn(_t748.env, _t747, 1));
                }
#line 356 "compiler/main.gem"
                *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 357 "compiler/main.gem"
    GemVal _t749 = (*gem_v_parse_fn_body);
                GemVal gem_v_body = _t749.fn(_t749.env, NULL, 0);
#line 358 "compiler/main.gem"
                *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 359 "compiler/main.gem"
    GemVal _t750[] = {gem_string("end")};
    GemVal _t751 = (*gem_v_expect);
                (void)(_t751.fn(_t751.env, _t750, 1));
#line 360 "compiler/main.gem"
    GemVal _t752[] = {gem_v_bparams, gem_v_brest, gem_v_body, gem_v_bdefaults};
    GemVal _t753 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t754[] = {gem_v_args, _t753.fn(_t753.env, _t752, 4)};
                (void)(gem_push_fn(NULL, _t754, 2));
            } else {
#line 364 "compiler/main.gem"
    GemVal _t755 = (*gem_v_peek);
    GemVal _t756 = _t755.fn(_t755.env, NULL, 0);
    static GemICacheSlot _ic_77 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t756, "type", &_ic_77), gem_string("{")))) {
#line 365 "compiler/main.gem"
                    GemVal gem_v_is_block = gem_bool(0);
#line 366 "compiler/main.gem"
    GemVal _t757[] = {gem_int(1)};
    GemVal _t758 = (*gem_v_peek_at);
    GemVal _t759 = _t758.fn(_t758.env, _t757, 1);
    static GemICacheSlot _ic_78 = {0};
                    if (gem_truthy(gem_eq(gem_table_get_cached(_t759, "type", &_ic_78), gem_string("|")))) {
#line 367 "compiler/main.gem"
                        gem_v_is_block = gem_bool(1);
                    } else {
#line 368 "compiler/main.gem"
    GemVal _t760[] = {gem_int(1)};
    GemVal _t761 = (*gem_v_peek_at);
    GemVal _t762 = _t761.fn(_t761.env, _t760, 1);
    static GemICacheSlot _ic_79 = {0};
    GemVal _t770;
    if (gem_truthy(gem_eq(gem_table_get_cached(_t762, "type", &_ic_79), gem_string("}")))) {
        _t770 = gem_eq(gem_table_get_cached(_t762, "type", &_ic_79), gem_string("}"));
    } else {
        GemVal _t763[] = {gem_int(1)};
        GemVal _t764 = (*gem_v_peek_at);
        GemVal _t765 = _t764.fn(_t764.env, _t763, 1);
        static GemICacheSlot _ic_80 = {0};
        GemVal _t769;
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t765, "type", &_ic_80), gem_string("NAME")))) {
                _t769 = gem_eq(gem_table_get_cached(_t765, "type", &_ic_80), gem_string("NAME"));
        } else {
                GemVal _t766[] = {gem_int(2)};
                GemVal _t767 = (*gem_v_peek_at);
                GemVal _t768 = _t767.fn(_t767.env, _t766, 1);
                static GemICacheSlot _ic_81 = {0};
                _t769 = gem_eq(gem_table_get_cached(_t768, "type", &_ic_81), gem_string(":"));
        }
        _t770 = _t769;
    }
                        if (gem_truthy(_t770)) {
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
    GemVal _t771 = (*gem_v_advance);
                        (void)(_t771.fn(_t771.env, NULL, 0));
#line 375 "compiler/main.gem"
    GemVal _t772 = gem_table_new();
                        GemVal gem_v_bparams = _t772;
#line 376 "compiler/main.gem"
                        GemVal gem_v_brest = GEM_NIL;
#line 377 "compiler/main.gem"
    GemVal _t773 = gem_table_new();
                        GemVal gem_v_bdefaults = _t773;
#line 378 "compiler/main.gem"
    GemVal _t774 = (*gem_v_peek);
    GemVal _t775 = _t774.fn(_t774.env, NULL, 0);
    static GemICacheSlot _ic_82 = {0};
                        if (gem_truthy(gem_eq(gem_table_get_cached(_t775, "type", &_ic_82), gem_string("|")))) {
#line 379 "compiler/main.gem"
    GemVal _t776 = (*gem_v_advance);
                            (void)(_t776.fn(_t776.env, NULL, 0));
#line 380 "compiler/main.gem"
    GemVal _t777 = (*gem_v_parse_params);
                            GemVal gem_v_bpr = _t777.fn(_t777.env, NULL, 0);
#line 381 "compiler/main.gem"
    GemVal _t778 = gem_v_bpr;
    static GemICacheSlot _ic_83 = {0};
                            gem_v_bparams = gem_table_get_cached(_t778, "params", &_ic_83);
#line 382 "compiler/main.gem"
    GemVal _t779 = gem_v_bpr;
    static GemICacheSlot _ic_84 = {0};
                            gem_v_brest = gem_table_get_cached(_t779, "rest_param", &_ic_84);
#line 383 "compiler/main.gem"
    GemVal _t780 = gem_v_bpr;
    static GemICacheSlot _ic_85 = {0};
                            gem_v_bdefaults = gem_table_get_cached(_t780, "defaults", &_ic_85);
#line 384 "compiler/main.gem"
    GemVal _t781[] = {gem_string("|")};
    GemVal _t782 = (*gem_v_expect);
                            (void)(_t782.fn(_t782.env, _t781, 1));
                        }
#line 386 "compiler/main.gem"
    GemVal _t783 = (*gem_v_parse_expr);
                        GemVal gem_v_bexpr = _t783.fn(_t783.env, NULL, 0);
#line 387 "compiler/main.gem"
    GemVal _t784[] = {gem_string("}")};
    GemVal _t785 = (*gem_v_expect);
                        (void)(_t785.fn(_t785.env, _t784, 1));
#line 388 "compiler/main.gem"
    GemVal _t786 = gem_table_new();
    gem_table_set(_t786, gem_int(0), gem_v_bexpr);
    GemVal _t787[] = {gem_v_bparams, gem_v_brest, _t786, gem_v_bdefaults};
    GemVal _t788 = (*gem_v__mod_parser_make_anon_fn);
    GemVal _t789[] = {gem_v_args, _t788.fn(_t788.env, _t787, 4)};
                        (void)(gem_push_fn(NULL, _t789, 2));
                    }
                }
            }
#line 393 "compiler/main.gem"
    GemVal _t790[] = {gem_v_node, gem_v_args, gem_v_call_line};
    GemVal _t791 = (*gem_v__mod_parser_make_call);
            gem_v_node = _t791.fn(_t791.env, _t790, 3);
            continue;
        }
#line 398 "compiler/main.gem"
    GemVal _t792 = (*gem_v_peek);
    GemVal _t793 = _t792.fn(_t792.env, NULL, 0);
    static GemICacheSlot _ic_86 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t793, "type", &_ic_86), gem_string(".")))) {
#line 399 "compiler/main.gem"
    GemVal _t794 = (*gem_v_advance);
            (void)(_t794.fn(_t794.env, NULL, 0));
#line 400 "compiler/main.gem"
    GemVal _t795 = (*gem_v_peek);
            GemVal gem_v_field_tok = _t795.fn(_t795.env, NULL, 0);
#line 401 "compiler/main.gem"
    GemVal _t796 = gem_v_field_tok;
    static GemICacheSlot _ic_87 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t796, "type", &_ic_87), gem_string("NAME")))) {
#line 402 "compiler/main.gem"
    GemVal _t797 = (*gem_v_advance);
                (void)(_t797.fn(_t797.env, NULL, 0));
            } else {
#line 405 "compiler/main.gem"
    GemVal _t798 = (*gem_v_advance);
                (void)(_t798.fn(_t798.env, NULL, 0));
            }
#line 407 "compiler/main.gem"
    GemVal _t799 = gem_v_field_tok;
    static GemICacheSlot _ic_88 = {0};
    GemVal _t800[] = {gem_v_node, gem_table_get_cached(_t799, "value", &_ic_88)};
    GemVal _t801 = (*gem_v__mod_parser_make_dot);
            gem_v_node = _t801.fn(_t801.env, _t800, 2);
            continue;
        }
#line 412 "compiler/main.gem"
    GemVal _t802 = (*gem_v_peek);
    GemVal _t803 = _t802.fn(_t802.env, NULL, 0);
    static GemICacheSlot _ic_89 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t803, "type", &_ic_89), gem_string("[")))) {
#line 413 "compiler/main.gem"
    GemVal _t804 = (*gem_v_advance);
            (void)(_t804.fn(_t804.env, NULL, 0));
#line 414 "compiler/main.gem"
    GemVal _t805 = (*gem_v_parse_expr);
            GemVal gem_v_key = _t805.fn(_t805.env, NULL, 0);
#line 415 "compiler/main.gem"
    GemVal _t806[] = {gem_string("]")};
    GemVal _t807 = (*gem_v_expect);
            (void)(_t807.fn(_t807.env, _t806, 1));
#line 416 "compiler/main.gem"
    GemVal _t808[] = {gem_v_node, gem_v_key};
    GemVal _t809 = (*gem_v__mod_parser_make_index);
            gem_v_node = _t809.fn(_t809.env, _t808, 2);
            continue;
        }
        break;
    }
    GemVal _t810 = gem_v_node;
    gem_pop_frame();
    return _t810;
}

struct _closure__anon_16 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_call;
    GemVal *gem_v_parse_unary;
    GemVal *gem_v_peek;
};
static GemVal _anon_16(void *_env, GemVal *args, int argc) {
    struct _closure__anon_16 *_cls = (struct _closure__anon_16 *)_env;
    GemVal *gem_v__mod_parser_make_unop = _cls->gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_call = _cls->gem_v_parse_call;
    GemVal *gem_v_parse_unary = _cls->gem_v_parse_unary;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_16", "compiler/main.gem", 0);
#line 428 "compiler/main.gem"
    GemVal _t812 = (*gem_v_peek);
    GemVal _t813 = _t812.fn(_t812.env, NULL, 0);
    static GemICacheSlot _ic_90 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t813, "type", &_ic_90), gem_string("-")))) {
#line 429 "compiler/main.gem"
    GemVal _t814 = (*gem_v_advance);
        (void)(_t814.fn(_t814.env, NULL, 0));
#line 430 "compiler/main.gem"
    GemVal _t815 = (*gem_v_parse_unary);
    GemVal _t816[] = {gem_string("-"), _t815.fn(_t815.env, NULL, 0)};
    GemVal _t817 = (*gem_v__mod_parser_make_unop);
        GemVal _t818 = _t817.fn(_t817.env, _t816, 2);
        gem_pop_frame();
        return _t818;
    }
#line 432 "compiler/main.gem"
    GemVal _t819 = (*gem_v_parse_call);
    GemVal _t820 = _t819.fn(_t819.env, NULL, 0);
    gem_pop_frame();
    return _t820;
}

struct _closure__anon_17 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_unary;
    GemVal *gem_v_peek;
};
static GemVal _anon_17(void *_env, GemVal *args, int argc) {
    struct _closure__anon_17 *_cls = (struct _closure__anon_17 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_unary = _cls->gem_v_parse_unary;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_17", "compiler/main.gem", 0);
#line 437 "compiler/main.gem"
    GemVal _t822 = (*gem_v_parse_unary);
    GemVal gem_v_left = _t822.fn(_t822.env, NULL, 0);
#line 438 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t823 = (*gem_v_peek);
        GemVal _t824 = _t823.fn(_t823.env, NULL, 0);
        static GemICacheSlot _ic_91 = {0};
        GemVal _t827;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t824, "type", &_ic_91), gem_string("*")))) {
                _t827 = gem_eq(gem_table_get_cached(_t824, "type", &_ic_91), gem_string("*"));
        } else {
                GemVal _t825 = (*gem_v_peek);
                GemVal _t826 = _t825.fn(_t825.env, NULL, 0);
                static GemICacheSlot _ic_92 = {0};
                _t827 = gem_eq(gem_table_get_cached(_t826, "type", &_ic_92), gem_string("/"));
        }
        GemVal _t830;
        if (gem_truthy(_t827)) {
                _t830 = _t827;
        } else {
                GemVal _t828 = (*gem_v_peek);
                GemVal _t829 = _t828.fn(_t828.env, NULL, 0);
                static GemICacheSlot _ic_93 = {0};
                _t830 = gem_eq(gem_table_get_cached(_t829, "type", &_ic_93), gem_string("%"));
        }
        if (!gem_truthy(_t830)) break;
#line 439 "compiler/main.gem"
    GemVal _t831 = (*gem_v_advance);
    GemVal _t832 = _t831.fn(_t831.env, NULL, 0);
    static GemICacheSlot _ic_94 = {0};
        GemVal gem_v_op = gem_table_get_cached(_t832, "type", &_ic_94);
#line 440 "compiler/main.gem"
    GemVal _t833 = (*gem_v_parse_unary);
        GemVal gem_v_right = _t833.fn(_t833.env, NULL, 0);
#line 441 "compiler/main.gem"
    GemVal _t834[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t835 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t835.fn(_t835.env, _t834, 3);
    }
    GemVal _t836 = gem_v_left;
    gem_pop_frame();
    return _t836;
}

struct _closure__anon_18 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_mul;
    GemVal *gem_v_peek;
};
static GemVal _anon_18(void *_env, GemVal *args, int argc) {
    struct _closure__anon_18 *_cls = (struct _closure__anon_18 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_mul = _cls->gem_v_parse_mul;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_18", "compiler/main.gem", 0);
#line 448 "compiler/main.gem"
    GemVal _t838 = (*gem_v_parse_mul);
    GemVal gem_v_left = _t838.fn(_t838.env, NULL, 0);
#line 449 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t839 = (*gem_v_peek);
        GemVal _t840 = _t839.fn(_t839.env, NULL, 0);
        static GemICacheSlot _ic_95 = {0};
        GemVal _t843;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t840, "type", &_ic_95), gem_string("+")))) {
                _t843 = gem_eq(gem_table_get_cached(_t840, "type", &_ic_95), gem_string("+"));
        } else {
                GemVal _t841 = (*gem_v_peek);
                GemVal _t842 = _t841.fn(_t841.env, NULL, 0);
                static GemICacheSlot _ic_96 = {0};
                _t843 = gem_eq(gem_table_get_cached(_t842, "type", &_ic_96), gem_string("-"));
        }
        if (!gem_truthy(_t843)) break;
#line 450 "compiler/main.gem"
    GemVal _t844 = (*gem_v_advance);
    GemVal _t845 = _t844.fn(_t844.env, NULL, 0);
    static GemICacheSlot _ic_97 = {0};
        GemVal gem_v_op = gem_table_get_cached(_t845, "type", &_ic_97);
#line 451 "compiler/main.gem"
    GemVal _t846 = (*gem_v_parse_mul);
        GemVal gem_v_right = _t846.fn(_t846.env, NULL, 0);
#line 452 "compiler/main.gem"
    GemVal _t847[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t848 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t848.fn(_t848.env, _t847, 3);
    }
    GemVal _t849 = gem_v_left;
    gem_pop_frame();
    return _t849;
}

struct _closure__anon_19 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_add;
    GemVal *gem_v_peek;
};
static GemVal _anon_19(void *_env, GemVal *args, int argc) {
    struct _closure__anon_19 *_cls = (struct _closure__anon_19 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_add = _cls->gem_v_parse_add;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_19", "compiler/main.gem", 0);
#line 459 "compiler/main.gem"
    GemVal _t851 = (*gem_v_parse_add);
    GemVal gem_v_left = _t851.fn(_t851.env, NULL, 0);
#line 460 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t852 = (*gem_v_peek);
        GemVal _t853 = _t852.fn(_t852.env, NULL, 0);
        static GemICacheSlot _ic_98 = {0};
        GemVal _t856;
        if (gem_truthy(gem_eq(gem_table_get_cached(_t853, "type", &_ic_98), gem_string("==")))) {
                _t856 = gem_eq(gem_table_get_cached(_t853, "type", &_ic_98), gem_string("=="));
        } else {
                GemVal _t854 = (*gem_v_peek);
                GemVal _t855 = _t854.fn(_t854.env, NULL, 0);
                static GemICacheSlot _ic_99 = {0};
                _t856 = gem_eq(gem_table_get_cached(_t855, "type", &_ic_99), gem_string("!="));
        }
        GemVal _t859;
        if (gem_truthy(_t856)) {
                _t859 = _t856;
        } else {
                GemVal _t857 = (*gem_v_peek);
                GemVal _t858 = _t857.fn(_t857.env, NULL, 0);
                static GemICacheSlot _ic_100 = {0};
                _t859 = gem_eq(gem_table_get_cached(_t858, "type", &_ic_100), gem_string("<"));
        }
        GemVal _t862;
        if (gem_truthy(_t859)) {
                _t862 = _t859;
        } else {
                GemVal _t860 = (*gem_v_peek);
                GemVal _t861 = _t860.fn(_t860.env, NULL, 0);
                static GemICacheSlot _ic_101 = {0};
                _t862 = gem_eq(gem_table_get_cached(_t861, "type", &_ic_101), gem_string(">"));
        }
        GemVal _t865;
        if (gem_truthy(_t862)) {
                _t865 = _t862;
        } else {
                GemVal _t863 = (*gem_v_peek);
                GemVal _t864 = _t863.fn(_t863.env, NULL, 0);
                static GemICacheSlot _ic_102 = {0};
                _t865 = gem_eq(gem_table_get_cached(_t864, "type", &_ic_102), gem_string("<="));
        }
        GemVal _t868;
        if (gem_truthy(_t865)) {
                _t868 = _t865;
        } else {
                GemVal _t866 = (*gem_v_peek);
                GemVal _t867 = _t866.fn(_t866.env, NULL, 0);
                static GemICacheSlot _ic_103 = {0};
                _t868 = gem_eq(gem_table_get_cached(_t867, "type", &_ic_103), gem_string(">="));
        }
        GemVal _t871;
        if (gem_truthy(_t868)) {
                _t871 = _t868;
        } else {
                GemVal _t869 = (*gem_v_peek);
                GemVal _t870 = _t869.fn(_t869.env, NULL, 0);
                static GemICacheSlot _ic_104 = {0};
                _t871 = gem_eq(gem_table_get_cached(_t870, "type", &_ic_104), gem_string("in"));
        }
        if (!gem_truthy(_t871)) break;
#line 461 "compiler/main.gem"
    GemVal _t872 = (*gem_v_advance);
    GemVal _t873 = _t872.fn(_t872.env, NULL, 0);
    static GemICacheSlot _ic_105 = {0};
        GemVal gem_v_op = gem_table_get_cached(_t873, "type", &_ic_105);
#line 462 "compiler/main.gem"
    GemVal _t874 = (*gem_v_parse_add);
        GemVal gem_v_right = _t874.fn(_t874.env, NULL, 0);
#line 463 "compiler/main.gem"
    GemVal _t875[] = {gem_v_op, gem_v_left, gem_v_right};
    GemVal _t876 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t876.fn(_t876.env, _t875, 3);
    }
    GemVal _t877 = gem_v_left;
    gem_pop_frame();
    return _t877;
}

struct _closure__anon_20 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_compare;
    GemVal *gem_v_parse_not;
    GemVal *gem_v_peek;
};
static GemVal _anon_20(void *_env, GemVal *args, int argc) {
    struct _closure__anon_20 *_cls = (struct _closure__anon_20 *)_env;
    GemVal *gem_v__mod_parser_make_unop = _cls->gem_v__mod_parser_make_unop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_compare = _cls->gem_v_parse_compare;
    GemVal *gem_v_parse_not = _cls->gem_v_parse_not;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_20", "compiler/main.gem", 0);
#line 470 "compiler/main.gem"
    GemVal _t879 = (*gem_v_peek);
    GemVal _t880 = _t879.fn(_t879.env, NULL, 0);
    static GemICacheSlot _ic_106 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t880, "type", &_ic_106), gem_string("not")))) {
#line 471 "compiler/main.gem"
    GemVal _t881 = (*gem_v_advance);
        (void)(_t881.fn(_t881.env, NULL, 0));
#line 472 "compiler/main.gem"
    GemVal _t882 = (*gem_v_parse_not);
    GemVal _t883[] = {gem_string("not"), _t882.fn(_t882.env, NULL, 0)};
    GemVal _t884 = (*gem_v__mod_parser_make_unop);
        GemVal _t885 = _t884.fn(_t884.env, _t883, 2);
        gem_pop_frame();
        return _t885;
    }
#line 474 "compiler/main.gem"
    GemVal _t886 = (*gem_v_parse_compare);
    GemVal _t887 = _t886.fn(_t886.env, NULL, 0);
    gem_pop_frame();
    return _t887;
}

struct _closure__anon_21 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_not;
    GemVal *gem_v_peek;
};
static GemVal _anon_21(void *_env, GemVal *args, int argc) {
    struct _closure__anon_21 *_cls = (struct _closure__anon_21 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_not = _cls->gem_v_parse_not;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_21", "compiler/main.gem", 0);
#line 479 "compiler/main.gem"
    GemVal _t889 = (*gem_v_parse_not);
    GemVal gem_v_left = _t889.fn(_t889.env, NULL, 0);
#line 480 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t890 = (*gem_v_peek);
        GemVal _t891 = _t890.fn(_t890.env, NULL, 0);
        static GemICacheSlot _ic_107 = {0};
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t891, "type", &_ic_107), gem_string("and")))) break;
#line 481 "compiler/main.gem"
    GemVal _t892 = (*gem_v_advance);
        (void)(_t892.fn(_t892.env, NULL, 0));
#line 482 "compiler/main.gem"
    GemVal _t893 = (*gem_v_parse_not);
        GemVal gem_v_right = _t893.fn(_t893.env, NULL, 0);
#line 483 "compiler/main.gem"
    GemVal _t894[] = {gem_string("and"), gem_v_left, gem_v_right};
    GemVal _t895 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t895.fn(_t895.env, _t894, 3);
    }
    GemVal _t896 = gem_v_left;
    gem_pop_frame();
    return _t896;
}

struct _closure__anon_22 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance;
    GemVal *gem_v_parse_and;
    GemVal *gem_v_peek;
};
static GemVal _anon_22(void *_env, GemVal *args, int argc) {
    struct _closure__anon_22 *_cls = (struct _closure__anon_22 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v_advance = _cls->gem_v_advance;
    GemVal *gem_v_parse_and = _cls->gem_v_parse_and;
    GemVal *gem_v_peek = _cls->gem_v_peek;
    gem_push_frame("_anon_22", "compiler/main.gem", 0);
#line 490 "compiler/main.gem"
    GemVal _t898 = (*gem_v_parse_and);
    GemVal gem_v_left = _t898.fn(_t898.env, NULL, 0);
#line 491 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t899 = (*gem_v_peek);
        GemVal _t900 = _t899.fn(_t899.env, NULL, 0);
        static GemICacheSlot _ic_108 = {0};
        if (!gem_truthy(gem_eq(gem_table_get_cached(_t900, "type", &_ic_108), gem_string("or")))) break;
#line 492 "compiler/main.gem"
    GemVal _t901 = (*gem_v_advance);
        (void)(_t901.fn(_t901.env, NULL, 0));
#line 493 "compiler/main.gem"
    GemVal _t902 = (*gem_v_parse_and);
        GemVal gem_v_right = _t902.fn(_t902.env, NULL, 0);
#line 494 "compiler/main.gem"
    GemVal _t903[] = {gem_string("or"), gem_v_left, gem_v_right};
    GemVal _t904 = (*gem_v__mod_parser_make_binop);
        gem_v_left = _t904.fn(_t904.env, _t903, 3);
    }
    GemVal _t905 = gem_v_left;
    gem_pop_frame();
    return _t905;
}

struct _closure__anon_23 {
    intptr_t _num_captures;
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
static GemVal _anon_23(void *_env, GemVal *args, int argc) {
    struct _closure__anon_23 *_cls = (struct _closure__anon_23 *)_env;
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
    gem_push_frame("_anon_23", "compiler/main.gem", 0);
#line 501 "compiler/main.gem"
    GemVal _t907 = (*gem_v_peek);
    GemVal _t908 = _t907.fn(_t907.env, NULL, 0);
    static GemICacheSlot _ic_109 = {0};
    GemVal gem_v_start_line = gem_table_get_cached(_t908, "line", &_ic_109);
#line 502 "compiler/main.gem"
    GemVal _t909 = (*gem_v_parse_or);
    GemVal gem_v_lhs = _t909.fn(_t909.env, NULL, 0);
#line 505 "compiler/main.gem"
    GemVal _t910 = (*gem_v_peek);
    GemVal _t911 = _t910.fn(_t910.env, NULL, 0);
    static GemICacheSlot _ic_110 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t911, "type", &_ic_110), gem_string("=")))) {
#line 506 "compiler/main.gem"
    GemVal _t912 = (*gem_v_advance);
        (void)(_t912.fn(_t912.env, NULL, 0));
#line 507 "compiler/main.gem"
    GemVal _t913 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t913.fn(_t913.env, NULL, 0);
#line 508 "compiler/main.gem"
    GemVal _t914 = gem_v_lhs;
    static GemICacheSlot _ic_111 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t914, "tag", &_ic_111), gem_string("var")))) {
#line 509 "compiler/main.gem"
    GemVal _t915 = gem_v_lhs;
    static GemICacheSlot _ic_112 = {0};
    GemVal _t916[] = {gem_table_get_cached(_t915, "name", &_ic_112), gem_v_value, gem_v_start_line};
    GemVal _t917 = (*gem_v__mod_parser_make_assign);
            GemVal _t918 = _t917.fn(_t917.env, _t916, 3);
            gem_pop_frame();
            return _t918;
        }
#line 511 "compiler/main.gem"
    GemVal _t919 = gem_v_lhs;
    static GemICacheSlot _ic_113 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t919, "tag", &_ic_113), gem_string("dot")))) {
#line 512 "compiler/main.gem"
    GemVal _t920 = gem_v_lhs;
    static GemICacheSlot _ic_114 = {0};
    GemVal _t921 = gem_v_lhs;
    static GemICacheSlot _ic_115 = {0};
    GemVal _t922[] = {gem_table_get_cached(_t920, "object", &_ic_114), gem_table_get_cached(_t921, "field", &_ic_115), gem_v_value, gem_v_start_line};
    GemVal _t923 = (*gem_v__mod_parser_make_dot_assign);
            GemVal _t924 = _t923.fn(_t923.env, _t922, 4);
            gem_pop_frame();
            return _t924;
        }
#line 514 "compiler/main.gem"
    GemVal _t925 = gem_v_lhs;
    static GemICacheSlot _ic_116 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t925, "tag", &_ic_116), gem_string("index")))) {
#line 515 "compiler/main.gem"
    GemVal _t926 = gem_v_lhs;
    static GemICacheSlot _ic_117 = {0};
    GemVal _t927 = gem_v_lhs;
    static GemICacheSlot _ic_118 = {0};
    GemVal _t928[] = {gem_table_get_cached(_t926, "object", &_ic_117), gem_table_get_cached(_t927, "key", &_ic_118), gem_v_value, gem_v_start_line};
    GemVal _t929 = (*gem_v__mod_parser_make_index_assign);
            GemVal _t930 = _t929.fn(_t929.env, _t928, 4);
            gem_pop_frame();
            return _t930;
        }
#line 517 "compiler/main.gem"
    GemVal _t931[] = {(*gem_v_source), (*gem_v_file), gem_v_start_line, gem_int(1), gem_int(1), gem_string("invalid assignment target (expected variable, field, or index)"), GEM_NIL};
    GemVal _t932 = (*gem_v__mod_parser_compile_error);
        (void)(_t932.fn(_t932.env, _t931, 7));
    }
#line 521 "compiler/main.gem"
    GemVal _t933 = (*gem_v_peek);
    GemVal _t934 = _t933.fn(_t933.env, NULL, 0);
    static GemICacheSlot _ic_119 = {0};
    GemVal _t937;
    if (gem_truthy(gem_eq(gem_table_get_cached(_t934, "type", &_ic_119), gem_string("+=")))) {
        _t937 = gem_eq(gem_table_get_cached(_t934, "type", &_ic_119), gem_string("+="));
    } else {
        GemVal _t935 = (*gem_v_peek);
        GemVal _t936 = _t935.fn(_t935.env, NULL, 0);
        static GemICacheSlot _ic_120 = {0};
        _t937 = gem_eq(gem_table_get_cached(_t936, "type", &_ic_120), gem_string("-="));
    }
    GemVal _t940;
    if (gem_truthy(_t937)) {
        _t940 = _t937;
    } else {
        GemVal _t938 = (*gem_v_peek);
        GemVal _t939 = _t938.fn(_t938.env, NULL, 0);
        static GemICacheSlot _ic_121 = {0};
        _t940 = gem_eq(gem_table_get_cached(_t939, "type", &_ic_121), gem_string("*="));
    }
    GemVal _t943;
    if (gem_truthy(_t940)) {
        _t943 = _t940;
    } else {
        GemVal _t941 = (*gem_v_peek);
        GemVal _t942 = _t941.fn(_t941.env, NULL, 0);
        static GemICacheSlot _ic_122 = {0};
        _t943 = gem_eq(gem_table_get_cached(_t942, "type", &_ic_122), gem_string("/="));
    }
    if (gem_truthy(_t943)) {
#line 522 "compiler/main.gem"
    GemVal _t944 = (*gem_v_advance);
        GemVal gem_v_op_tok = _t944.fn(_t944.env, NULL, 0);
#line 523 "compiler/main.gem"
    GemVal _t945 = gem_v_op_tok;
    static GemICacheSlot _ic_123 = {0};
        GemVal gem_v_base_op = gem_table_get(gem_table_get_cached(_t945, "value", &_ic_123), gem_int(0));
#line 524 "compiler/main.gem"
    GemVal _t946 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t946.fn(_t946.env, NULL, 0);
#line 525 "compiler/main.gem"
    GemVal _t947 = gem_v_lhs;
    static GemICacheSlot _ic_124 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t947, "tag", &_ic_124), gem_string("var")))) {
#line 526 "compiler/main.gem"
    GemVal _t948 = gem_v_op_tok;
    static GemICacheSlot _ic_125 = {0};
    GemVal _t949 = gem_v_op_tok;
    static GemICacheSlot _ic_126 = {0};
    GemVal _t950 = gem_v_op_tok;
    static GemICacheSlot _ic_127 = {0};
    GemVal _t951[] = {gem_table_get_cached(_t950, "value", &_ic_127)};
    GemVal _t952[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t948, "line", &_ic_125), gem_table_get_cached(_t949, "col", &_ic_126), gem_len_fn(NULL, _t951, 1), gem_string("compound assignment requires variable target"), GEM_NIL};
    GemVal _t953 = (*gem_v__mod_parser_compile_error);
            (void)(_t953.fn(_t953.env, _t952, 7));
        }
#line 528 "compiler/main.gem"
    GemVal _t954 = gem_v_lhs;
    static GemICacheSlot _ic_128 = {0};
    GemVal _t955 = gem_v_lhs;
    static GemICacheSlot _ic_129 = {0};
    GemVal _t956[] = {gem_table_get_cached(_t955, "name", &_ic_129)};
    GemVal _t957 = (*gem_v__mod_parser_make_var);
    GemVal _t958[] = {gem_v_base_op, _t957.fn(_t957.env, _t956, 1), gem_v_value};
    GemVal _t959 = (*gem_v__mod_parser_make_binop);
    GemVal _t960[] = {gem_table_get_cached(_t954, "name", &_ic_128), _t959.fn(_t959.env, _t958, 3), gem_v_start_line};
    GemVal _t961 = (*gem_v__mod_parser_make_assign);
        GemVal _t962 = _t961.fn(_t961.env, _t960, 3);
        gem_pop_frame();
        return _t962;
    }
    GemVal _t963 = gem_v_lhs;
    gem_pop_frame();
    return _t963;
}

struct _closure__anon_25 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_bool;
};
static GemVal _anon_25(void *_env, GemVal *args, int argc) {
    struct _closure__anon_25 *_cls = (struct _closure__anon_25 *)_env;
    GemVal *gem_v__mod_parser_make_binop = _cls->gem_v__mod_parser_make_binop;
    GemVal *gem_v__mod_parser_make_bool = _cls->gem_v__mod_parser_make_bool;
    GemVal gem_v_conditions = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_25", "compiler/main.gem", 0);
#line 541 "compiler/main.gem"
    GemVal _t965[] = {gem_v_conditions};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t965, 1), gem_int(0)))) {
#line 542 "compiler/main.gem"
    GemVal _t966[] = {gem_bool(1)};
    GemVal _t967 = (*gem_v__mod_parser_make_bool);
        GemVal _t968 = _t967.fn(_t967.env, _t966, 1);
        gem_pop_frame();
        return _t968;
    }
#line 544 "compiler/main.gem"
    GemVal gem_v_result = gem_table_get(gem_v_conditions, gem_int(0));
#line 545 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(1);
#line 545 "compiler/main.gem"
    GemVal _t969[] = {gem_v_conditions};
    GemVal gem_v__for_limit_4 = gem_len_fn(NULL, _t969, 1);
#line 545 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_limit_4))) break;
#line 545 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_4;
#line 545 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 546 "compiler/main.gem"
    GemVal _t970[] = {gem_string("and"), gem_v_result, gem_table_get(gem_v_conditions, gem_v_i)};
    GemVal _t971 = (*gem_v__mod_parser_make_binop);
        gem_v_result = _t971.fn(_t971.env, _t970, 3);
    }

    GemVal _t972 = gem_v_result;
    gem_pop_frame();
    return _t972;
}

struct _closure__anon_24 {
    intptr_t _num_captures;
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
static GemVal _anon_24(void *_env, GemVal *args, int argc) {
    struct _closure__anon_24 *_cls = (struct _closure__anon_24 *)_env;
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
    gem_push_frame("_anon_24", "compiler/main.gem", 0);
#line 540 "compiler/main.gem"
    struct _closure__anon_25 *_t973 = GC_MALLOC(sizeof(struct _closure__anon_25));
    _t973->_num_captures = 2;
    _t973->gem_v__mod_parser_make_binop = gem_v__mod_parser_make_binop;
    _t973->gem_v__mod_parser_make_bool = gem_v__mod_parser_make_bool;
    GemVal gem_v_and_chain = gem_make_fn(_anon_25, _t973);
#line 550 "compiler/main.gem"
    GemVal _t974 = (*gem_v_peek);
    GemVal gem_v_t = _t974.fn(_t974.env, NULL, 0);
#line 553 "compiler/main.gem"
    GemVal _t975 = gem_v_t;
    static GemICacheSlot _ic_130 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t975, "type", &_ic_130), gem_string("{")))) {
#line 554 "compiler/main.gem"
    GemVal _t976 = (*gem_v_advance);
        (void)(_t976.fn(_t976.env, NULL, 0));
#line 555 "compiler/main.gem"
    GemVal _t977 = (*gem_v_skip_nl);
        (void)(_t977.fn(_t977.env, NULL, 0));
#line 556 "compiler/main.gem"
    GemVal _t978 = gem_table_new();
    GemVal _t979[] = {gem_string("type")};
    GemVal _t980 = (*gem_v__mod_parser_make_var);
    GemVal _t981 = gem_table_new();
    gem_table_set(_t981, gem_int(0), gem_v_target_expr);
    GemVal _t982[] = {_t980.fn(_t980.env, _t979, 1), _t981, gem_int(0)};
    GemVal _t983 = (*gem_v__mod_parser_make_call);
    GemVal _t984[] = {gem_string("table")};
    GemVal _t985 = (*gem_v__mod_parser_make_string);
    GemVal _t986[] = {gem_string("=="), _t983.fn(_t983.env, _t982, 3), _t985.fn(_t985.env, _t984, 1)};
    GemVal _t987 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t978, gem_int(0), _t987.fn(_t987.env, _t986, 3));
        GemVal gem_v_conditions = _t978;
#line 557 "compiler/main.gem"
    GemVal _t988 = gem_table_new();
        GemVal gem_v_bindings = _t988;
#line 558 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t989 = (*gem_v_peek);
            GemVal _t990 = _t989.fn(_t989.env, NULL, 0);
            static GemICacheSlot _ic_131 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t990, "type", &_ic_131), gem_string("}")))) break;
#line 559 "compiler/main.gem"
    GemVal _t991 = (*gem_v_peek);
            GemVal gem_v_key_tok = _t991.fn(_t991.env, NULL, 0);
#line 560 "compiler/main.gem"
    GemVal _t992 = gem_v_key_tok;
    static GemICacheSlot _ic_132 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t992, "type", &_ic_132), gem_string("NAME")))) {
#line 561 "compiler/main.gem"
    GemVal _t993 = (*gem_v_advance);
                (void)(_t993.fn(_t993.env, NULL, 0));
            } else {
#line 562 "compiler/main.gem"
    GemVal _t994[] = {gem_int(1)};
    GemVal _t995 = (*gem_v_peek_at);
    GemVal _t996 = _t995.fn(_t995.env, _t994, 1);
    static GemICacheSlot _ic_133 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t996, "type", &_ic_133), gem_string(":")))) {
#line 564 "compiler/main.gem"
    GemVal _t997 = (*gem_v_advance);
                    (void)(_t997.fn(_t997.env, NULL, 0));
                } else {
#line 566 "compiler/main.gem"
    GemVal _t998 = gem_v_key_tok;
    static GemICacheSlot _ic_134 = {0};
    GemVal _t999 = gem_v_key_tok;
    static GemICacheSlot _ic_135 = {0};
    GemVal _t1000 = gem_v_key_tok;
    static GemICacheSlot _ic_136 = {0};
    GemVal _t1001[] = {gem_table_get_cached(_t1000, "value", &_ic_136)};
    GemVal _t1002[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t998, "line", &_ic_134), gem_table_get_cached(_t999, "col", &_ic_135), gem_len_fn(NULL, _t1001, 1), gem_string("expected key name in table pattern"), GEM_NIL};
    GemVal _t1003 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1003.fn(_t1003.env, _t1002, 7));
                }
            }
#line 568 "compiler/main.gem"
    GemVal _t1004 = gem_v_key_tok;
    static GemICacheSlot _ic_137 = {0};
            GemVal gem_v_key_name = gem_table_get_cached(_t1004, "value", &_ic_137);
#line 569 "compiler/main.gem"
    GemVal _t1005[] = {gem_v_key_name};
    GemVal _t1006 = (*gem_v__mod_parser_make_string);
            GemVal gem_v_key_str = _t1006.fn(_t1006.env, _t1005, 1);
#line 570 "compiler/main.gem"
    GemVal _t1007[] = {gem_string("has_key")};
    GemVal _t1008 = (*gem_v__mod_parser_make_var);
    GemVal _t1009 = gem_table_new();
    gem_table_set(_t1009, gem_int(0), gem_v_target_expr);
    gem_table_set(_t1009, gem_int(1), gem_v_key_str);
    GemVal _t1010[] = {_t1008.fn(_t1008.env, _t1007, 1), _t1009, gem_int(0)};
    GemVal _t1011 = (*gem_v__mod_parser_make_call);
    GemVal _t1012[] = {gem_v_conditions, _t1011.fn(_t1011.env, _t1010, 3)};
            (void)(gem_push_fn(NULL, _t1012, 2));
#line 571 "compiler/main.gem"
    GemVal _t1013[] = {gem_v_target_expr, gem_v_key_str};
    GemVal _t1014 = (*gem_v__mod_parser_make_index);
            GemVal gem_v_sub_target = _t1014.fn(_t1014.env, _t1013, 2);
#line 572 "compiler/main.gem"
    GemVal _t1015 = (*gem_v_peek);
    GemVal _t1016 = _t1015.fn(_t1015.env, NULL, 0);
    static GemICacheSlot _ic_138 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1016, "type", &_ic_138), gem_string(":")))) {
#line 573 "compiler/main.gem"
    GemVal _t1017 = (*gem_v_advance);
                (void)(_t1017.fn(_t1017.env, NULL, 0));
#line 574 "compiler/main.gem"
    GemVal _t1018 = (*gem_v_skip_nl);
                (void)(_t1018.fn(_t1018.env, NULL, 0));
#line 575 "compiler/main.gem"
    GemVal _t1019[] = {gem_v_sub_target};
    GemVal _t1020 = (*gem_v_parse_pattern);
                GemVal gem_v_sub = _t1020.fn(_t1020.env, _t1019, 1);
#line 576 "compiler/main.gem"
    GemVal _t1021 = gem_v_sub;
    static GemICacheSlot _ic_139 = {0};
    GemVal _t1022 = gem_table_get_cached(_t1021, "condition", &_ic_139);
    static GemICacheSlot _ic_140 = {0};
    GemVal _t1025;
    if (gem_truthy(gem_neq(gem_table_get_cached(_t1022, "tag", &_ic_140), gem_string("bool")))) {
        _t1025 = gem_neq(gem_table_get_cached(_t1022, "tag", &_ic_140), gem_string("bool"));
    } else {
        GemVal _t1023 = gem_v_sub;
        static GemICacheSlot _ic_141 = {0};
        GemVal _t1024 = gem_table_get_cached(_t1023, "condition", &_ic_141);
        static GemICacheSlot _ic_142 = {0};
        _t1025 = gem_neq(gem_table_get_cached(_t1024, "value", &_ic_142), gem_bool(1));
    }
                if (gem_truthy(_t1025)) {
#line 577 "compiler/main.gem"
    GemVal _t1026 = gem_v_sub;
    static GemICacheSlot _ic_143 = {0};
    GemVal _t1027[] = {gem_v_conditions, gem_table_get_cached(_t1026, "condition", &_ic_143)};
                    (void)(gem_push_fn(NULL, _t1027, 2));
                }
#line 579 "compiler/main.gem"
    GemVal _t1028 = gem_v_sub;
    static GemICacheSlot _ic_144 = {0};
                GemVal gem_v__for_items_5 = gem_table_get_cached(_t1028, "bindings", &_ic_144);
#line 579 "compiler/main.gem"
                GemVal gem_v__for_i_5 = gem_int(0);
#line 579 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1029[] = {gem_v__for_items_5};
                    if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t1029, 1)))) break;
#line 579 "compiler/main.gem"
                    GemVal gem_v_b = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 579 "compiler/main.gem"
                    gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 580 "compiler/main.gem"
    GemVal _t1030[] = {gem_v_bindings, gem_v_b};
                    (void)(gem_push_fn(NULL, _t1030, 2));
                }

            } else {
#line 584 "compiler/main.gem"
    GemVal _t1031[] = {gem_v_key_name, gem_v_sub_target, gem_int(0)};
    GemVal _t1032 = (*gem_v__mod_parser_make_let);
    GemVal _t1033[] = {gem_v_bindings, _t1032.fn(_t1032.env, _t1031, 3)};
                (void)(gem_push_fn(NULL, _t1033, 2));
            }
#line 586 "compiler/main.gem"
    GemVal _t1034 = (*gem_v_skip_nl);
            (void)(_t1034.fn(_t1034.env, NULL, 0));
#line 587 "compiler/main.gem"
    GemVal _t1035 = (*gem_v_peek);
    GemVal _t1036 = _t1035.fn(_t1035.env, NULL, 0);
    static GemICacheSlot _ic_145 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1036, "type", &_ic_145), gem_string(",")))) {
#line 588 "compiler/main.gem"
    GemVal _t1037 = (*gem_v_advance);
                (void)(_t1037.fn(_t1037.env, NULL, 0));
#line 589 "compiler/main.gem"
    GemVal _t1038 = (*gem_v_skip_nl);
                (void)(_t1038.fn(_t1038.env, NULL, 0));
            }
        }
#line 592 "compiler/main.gem"
    GemVal _t1039[] = {gem_string("}")};
    GemVal _t1040 = (*gem_v_expect);
        (void)(_t1040.fn(_t1040.env, _t1039, 1));
#line 593 "compiler/main.gem"
    GemVal _t1041 = gem_table_new();
    GemVal _t1042[] = {gem_v_conditions};
    GemVal _t1043 = gem_v_and_chain;
    gem_table_set(_t1041, gem_string("condition"), _t1043.fn(_t1043.env, _t1042, 1));
    gem_table_set(_t1041, gem_string("bindings"), gem_v_bindings);
        GemVal _t1044 = _t1041;
        gem_pop_frame();
        return _t1044;
    }
#line 597 "compiler/main.gem"
    GemVal _t1045 = gem_v_t;
    static GemICacheSlot _ic_146 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1045, "type", &_ic_146), gem_string("[")))) {
#line 598 "compiler/main.gem"
    GemVal _t1046 = (*gem_v_advance);
        (void)(_t1046.fn(_t1046.env, NULL, 0));
#line 599 "compiler/main.gem"
    GemVal _t1047 = (*gem_v_skip_nl);
        (void)(_t1047.fn(_t1047.env, NULL, 0));
#line 600 "compiler/main.gem"
    GemVal _t1048 = gem_table_new();
        GemVal gem_v_sub_patterns = _t1048;
#line 601 "compiler/main.gem"
        GemVal gem_v_idx = gem_int(0);
#line 602 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1049 = (*gem_v_peek);
            GemVal _t1050 = _t1049.fn(_t1049.env, NULL, 0);
            static GemICacheSlot _ic_147 = {0};
            if (!gem_truthy(gem_neq(gem_table_get_cached(_t1050, "type", &_ic_147), gem_string("]")))) break;
#line 603 "compiler/main.gem"
    GemVal _t1051[] = {gem_v_idx};
    GemVal _t1052 = (*gem_v__mod_parser_make_int);
    GemVal _t1053[] = {gem_v_target_expr, _t1052.fn(_t1052.env, _t1051, 1)};
    GemVal _t1054 = (*gem_v__mod_parser_make_index);
    GemVal _t1055[] = {_t1054.fn(_t1054.env, _t1053, 2)};
    GemVal _t1056 = (*gem_v_parse_pattern);
            GemVal gem_v_sub = _t1056.fn(_t1056.env, _t1055, 1);
#line 604 "compiler/main.gem"
    GemVal _t1057[] = {gem_v_sub_patterns, gem_v_sub};
            (void)(gem_push_fn(NULL, _t1057, 2));
#line 605 "compiler/main.gem"
            gem_v_idx = gem_add(gem_v_idx, gem_int(1));
#line 606 "compiler/main.gem"
    GemVal _t1058 = (*gem_v_skip_nl);
            (void)(_t1058.fn(_t1058.env, NULL, 0));
#line 607 "compiler/main.gem"
    GemVal _t1059 = (*gem_v_peek);
    GemVal _t1060 = _t1059.fn(_t1059.env, NULL, 0);
    static GemICacheSlot _ic_148 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1060, "type", &_ic_148), gem_string(",")))) {
#line 608 "compiler/main.gem"
    GemVal _t1061 = (*gem_v_advance);
                (void)(_t1061.fn(_t1061.env, NULL, 0));
#line 609 "compiler/main.gem"
    GemVal _t1062 = (*gem_v_skip_nl);
                (void)(_t1062.fn(_t1062.env, NULL, 0));
            }
        }
#line 612 "compiler/main.gem"
    GemVal _t1063[] = {gem_string("]")};
    GemVal _t1064 = (*gem_v_expect);
        (void)(_t1064.fn(_t1064.env, _t1063, 1));
#line 613 "compiler/main.gem"
    GemVal _t1065 = gem_table_new();
    GemVal _t1066[] = {gem_string("type")};
    GemVal _t1067 = (*gem_v__mod_parser_make_var);
    GemVal _t1068 = gem_table_new();
    gem_table_set(_t1068, gem_int(0), gem_v_target_expr);
    GemVal _t1069[] = {_t1067.fn(_t1067.env, _t1066, 1), _t1068, gem_int(0)};
    GemVal _t1070 = (*gem_v__mod_parser_make_call);
    GemVal _t1071[] = {gem_string("table")};
    GemVal _t1072 = (*gem_v__mod_parser_make_string);
    GemVal _t1073[] = {gem_string("=="), _t1070.fn(_t1070.env, _t1069, 3), _t1072.fn(_t1072.env, _t1071, 1)};
    GemVal _t1074 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1065, gem_int(0), _t1074.fn(_t1074.env, _t1073, 3));
        GemVal gem_v_conditions = _t1065;
#line 614 "compiler/main.gem"
    GemVal _t1075[] = {gem_string("len")};
    GemVal _t1076 = (*gem_v__mod_parser_make_var);
    GemVal _t1077 = gem_table_new();
    gem_table_set(_t1077, gem_int(0), gem_v_target_expr);
    GemVal _t1078[] = {_t1076.fn(_t1076.env, _t1075, 1), _t1077, gem_int(0)};
    GemVal _t1079 = (*gem_v__mod_parser_make_call);
    GemVal _t1080[] = {gem_v_idx};
    GemVal _t1081 = (*gem_v__mod_parser_make_int);
    GemVal _t1082[] = {gem_string("=="), _t1079.fn(_t1079.env, _t1078, 3), _t1081.fn(_t1081.env, _t1080, 1)};
    GemVal _t1083 = (*gem_v__mod_parser_make_binop);
    GemVal _t1084[] = {gem_v_conditions, _t1083.fn(_t1083.env, _t1082, 3)};
        (void)(gem_push_fn(NULL, _t1084, 2));
#line 615 "compiler/main.gem"
    GemVal _t1085 = gem_table_new();
        GemVal gem_v_bindings = _t1085;
#line 616 "compiler/main.gem"
        GemVal gem_v__for_i_7 = gem_int(0);
#line 616 "compiler/main.gem"
    GemVal _t1086[] = {gem_v_sub_patterns};
        GemVal gem_v__for_limit_7 = gem_len_fn(NULL, _t1086, 1);
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
    GemVal _t1087 = gem_v_sub;
    static GemICacheSlot _ic_149 = {0};
    GemVal _t1088 = gem_table_get_cached(_t1087, "condition", &_ic_149);
    static GemICacheSlot _ic_150 = {0};
    GemVal _t1091;
    if (gem_truthy(gem_neq(gem_table_get_cached(_t1088, "tag", &_ic_150), gem_string("bool")))) {
        _t1091 = gem_neq(gem_table_get_cached(_t1088, "tag", &_ic_150), gem_string("bool"));
    } else {
        GemVal _t1089 = gem_v_sub;
        static GemICacheSlot _ic_151 = {0};
        GemVal _t1090 = gem_table_get_cached(_t1089, "condition", &_ic_151);
        static GemICacheSlot _ic_152 = {0};
        _t1091 = gem_neq(gem_table_get_cached(_t1090, "value", &_ic_152), gem_bool(1));
    }
            if (gem_truthy(_t1091)) {
#line 619 "compiler/main.gem"
    GemVal _t1092 = gem_v_sub;
    static GemICacheSlot _ic_153 = {0};
    GemVal _t1093[] = {gem_v_conditions, gem_table_get_cached(_t1092, "condition", &_ic_153)};
                (void)(gem_push_fn(NULL, _t1093, 2));
            }
#line 621 "compiler/main.gem"
    GemVal _t1094 = gem_v_sub;
    static GemICacheSlot _ic_154 = {0};
            GemVal gem_v__for_items_6 = gem_table_get_cached(_t1094, "bindings", &_ic_154);
#line 621 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 621 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1095[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t1095, 1)))) break;
#line 621 "compiler/main.gem"
                GemVal gem_v_b = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 621 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 622 "compiler/main.gem"
    GemVal _t1096[] = {gem_v_bindings, gem_v_b};
                (void)(gem_push_fn(NULL, _t1096, 2));
            }

        }

#line 625 "compiler/main.gem"
    GemVal _t1097 = gem_table_new();
    GemVal _t1098[] = {gem_v_conditions};
    GemVal _t1099 = gem_v_and_chain;
    gem_table_set(_t1097, gem_string("condition"), _t1099.fn(_t1099.env, _t1098, 1));
    gem_table_set(_t1097, gem_string("bindings"), gem_v_bindings);
        GemVal _t1100 = _t1097;
        gem_pop_frame();
        return _t1100;
    }
#line 629 "compiler/main.gem"
    GemVal _t1101 = gem_v_t;
    static GemICacheSlot _ic_155 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1101, "type", &_ic_155), gem_string("NUMBER")))) {
#line 630 "compiler/main.gem"
    GemVal _t1102 = (*gem_v_advance);
        (void)(_t1102.fn(_t1102.env, NULL, 0));
#line 631 "compiler/main.gem"
        GemVal gem_v_lit = GEM_NIL;
#line 632 "compiler/main.gem"
    GemVal _t1103 = gem_v_t;
    static GemICacheSlot _ic_156 = {0};
    GemVal _t1104[] = {gem_table_get_cached(_t1103, "value", &_ic_156)};
        if (gem_truthy(gem_fn__mod_parser_contains_dot(NULL, _t1104, 1))) {
#line 633 "compiler/main.gem"
    GemVal _t1105 = gem_v_t;
    static GemICacheSlot _ic_157 = {0};
    GemVal _t1106[] = {gem_table_get_cached(_t1105, "value", &_ic_157)};
    GemVal _t1107[] = {gem_fn_atof(NULL, _t1106, 1)};
    GemVal _t1108 = (*gem_v__mod_parser_make_float);
            gem_v_lit = _t1108.fn(_t1108.env, _t1107, 1);
        } else {
#line 635 "compiler/main.gem"
    GemVal _t1109 = gem_v_t;
    static GemICacheSlot _ic_158 = {0};
    GemVal _t1110[] = {gem_table_get_cached(_t1109, "value", &_ic_158)};
    GemVal _t1111[] = {gem_fn__mod_parser_str_to_int(NULL, _t1110, 1)};
    GemVal _t1112 = (*gem_v__mod_parser_make_int);
            gem_v_lit = _t1112.fn(_t1112.env, _t1111, 1);
        }
#line 637 "compiler/main.gem"
    GemVal _t1113 = gem_table_new();
    GemVal _t1114[] = {gem_string("=="), gem_v_target_expr, gem_v_lit};
    GemVal _t1115 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1113, gem_string("condition"), _t1115.fn(_t1115.env, _t1114, 3));
    GemVal _t1116 = gem_table_new();
    gem_table_set(_t1113, gem_string("bindings"), _t1116);
        GemVal _t1117 = _t1113;
        gem_pop_frame();
        return _t1117;
    }
#line 639 "compiler/main.gem"
    GemVal _t1118 = gem_v_t;
    static GemICacheSlot _ic_159 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1118, "type", &_ic_159), gem_string("STRING")))) {
#line 640 "compiler/main.gem"
    GemVal _t1119 = (*gem_v_advance);
        (void)(_t1119.fn(_t1119.env, NULL, 0));
#line 641 "compiler/main.gem"
    GemVal _t1120 = gem_table_new();
    GemVal _t1121 = gem_v_t;
    static GemICacheSlot _ic_160 = {0};
    GemVal _t1122[] = {gem_table_get_cached(_t1121, "value", &_ic_160)};
    GemVal _t1123 = (*gem_v__mod_parser_make_string);
    GemVal _t1124[] = {gem_string("=="), gem_v_target_expr, _t1123.fn(_t1123.env, _t1122, 1)};
    GemVal _t1125 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1120, gem_string("condition"), _t1125.fn(_t1125.env, _t1124, 3));
    GemVal _t1126 = gem_table_new();
    gem_table_set(_t1120, gem_string("bindings"), _t1126);
        GemVal _t1127 = _t1120;
        gem_pop_frame();
        return _t1127;
    }
#line 643 "compiler/main.gem"
    GemVal _t1128 = gem_v_t;
    static GemICacheSlot _ic_161 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1128, "type", &_ic_161), gem_string("true")))) {
#line 644 "compiler/main.gem"
    GemVal _t1129 = (*gem_v_advance);
        (void)(_t1129.fn(_t1129.env, NULL, 0));
#line 645 "compiler/main.gem"
    GemVal _t1130 = gem_table_new();
    GemVal _t1131[] = {gem_bool(1)};
    GemVal _t1132 = (*gem_v__mod_parser_make_bool);
    GemVal _t1133[] = {gem_string("=="), gem_v_target_expr, _t1132.fn(_t1132.env, _t1131, 1)};
    GemVal _t1134 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1130, gem_string("condition"), _t1134.fn(_t1134.env, _t1133, 3));
    GemVal _t1135 = gem_table_new();
    gem_table_set(_t1130, gem_string("bindings"), _t1135);
        GemVal _t1136 = _t1130;
        gem_pop_frame();
        return _t1136;
    }
#line 647 "compiler/main.gem"
    GemVal _t1137 = gem_v_t;
    static GemICacheSlot _ic_162 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1137, "type", &_ic_162), gem_string("false")))) {
#line 648 "compiler/main.gem"
    GemVal _t1138 = (*gem_v_advance);
        (void)(_t1138.fn(_t1138.env, NULL, 0));
#line 649 "compiler/main.gem"
    GemVal _t1139 = gem_table_new();
    GemVal _t1140[] = {gem_bool(0)};
    GemVal _t1141 = (*gem_v__mod_parser_make_bool);
    GemVal _t1142[] = {gem_string("=="), gem_v_target_expr, _t1141.fn(_t1141.env, _t1140, 1)};
    GemVal _t1143 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1139, gem_string("condition"), _t1143.fn(_t1143.env, _t1142, 3));
    GemVal _t1144 = gem_table_new();
    gem_table_set(_t1139, gem_string("bindings"), _t1144);
        GemVal _t1145 = _t1139;
        gem_pop_frame();
        return _t1145;
    }
#line 651 "compiler/main.gem"
    GemVal _t1146 = gem_v_t;
    static GemICacheSlot _ic_163 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1146, "type", &_ic_163), gem_string("nil")))) {
#line 652 "compiler/main.gem"
    GemVal _t1147 = (*gem_v_advance);
        (void)(_t1147.fn(_t1147.env, NULL, 0));
#line 653 "compiler/main.gem"
    GemVal _t1148 = gem_table_new();
    GemVal _t1149 = (*gem_v__mod_parser_make_nil_node);
    GemVal _t1150[] = {gem_string("=="), gem_v_target_expr, _t1149.fn(_t1149.env, NULL, 0)};
    GemVal _t1151 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1148, gem_string("condition"), _t1151.fn(_t1151.env, _t1150, 3));
    GemVal _t1152 = gem_table_new();
    gem_table_set(_t1148, gem_string("bindings"), _t1152);
        GemVal _t1153 = _t1148;
        gem_pop_frame();
        return _t1153;
    }
#line 657 "compiler/main.gem"
    GemVal _t1154 = gem_v_t;
    static GemICacheSlot _ic_164 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1154, "type", &_ic_164), gem_string("NAME")))) {
#line 658 "compiler/main.gem"
    GemVal _t1155 = (*gem_v_advance);
        (void)(_t1155.fn(_t1155.env, NULL, 0));
#line 659 "compiler/main.gem"
    GemVal _t1156 = gem_table_new();
    GemVal _t1157[] = {gem_bool(1)};
    GemVal _t1158 = (*gem_v__mod_parser_make_bool);
    gem_table_set(_t1156, gem_string("condition"), _t1158.fn(_t1158.env, _t1157, 1));
    GemVal _t1159 = gem_table_new();
    GemVal _t1160 = gem_v_t;
    static GemICacheSlot _ic_165 = {0};
    GemVal _t1161[] = {gem_table_get_cached(_t1160, "value", &_ic_165), gem_v_target_expr, gem_int(0)};
    GemVal _t1162 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1159, gem_int(0), _t1162.fn(_t1162.env, _t1161, 3));
    gem_table_set(_t1156, gem_string("bindings"), _t1159);
        GemVal _t1163 = _t1156;
        gem_pop_frame();
        return _t1163;
    }
#line 662 "compiler/main.gem"
    GemVal _t1164 = gem_v_t;
    static GemICacheSlot _ic_166 = {0};
    GemVal _t1165 = gem_v_t;
    static GemICacheSlot _ic_167 = {0};
    GemVal _t1166 = gem_v_t;
    static GemICacheSlot _ic_168 = {0};
    GemVal _t1167[] = {gem_table_get_cached(_t1166, "value", &_ic_168)};
    GemVal _t1168 = gem_v_t;
    static GemICacheSlot _ic_169 = {0};
    GemVal _t1169[] = {gem_string("expected pattern but got '"), gem_table_get_cached(_t1168, "type", &_ic_169), gem_string("'")};
    GemVal _t1170[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1164, "line", &_ic_166), gem_table_get_cached(_t1165, "col", &_ic_167), gem_len_fn(NULL, _t1167, 1), gem_interp(3, _t1169), GEM_NIL};
    GemVal _t1171 = (*gem_v__mod_parser_compile_error);
    GemVal _t1172 = _t1171.fn(_t1171.env, _t1170, 7);
    gem_pop_frame();
    return _t1172;
}

struct _closure__anon_26 {
    intptr_t _num_captures;
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
static GemVal _anon_26(void *_env, GemVal *args, int argc) {
    struct _closure__anon_26 *_cls = (struct _closure__anon_26 *)_env;
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
    gem_push_frame("_anon_26", "compiler/main.gem", 0);
#line 668 "compiler/main.gem"
    GemVal _t1174 = (*gem_v_peek);
    GemVal gem_v_t = _t1174.fn(_t1174.env, NULL, 0);
#line 671 "compiler/main.gem"
    GemVal _t1175 = gem_v_t;
    static GemICacheSlot _ic_170 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1175, "type", &_ic_170), gem_string("let")))) {
#line 672 "compiler/main.gem"
    GemVal _t1176 = gem_v_t;
    static GemICacheSlot _ic_171 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1176, "line", &_ic_171);
#line 673 "compiler/main.gem"
    GemVal _t1177 = (*gem_v_advance);
        (void)(_t1177.fn(_t1177.env, NULL, 0));
#line 676 "compiler/main.gem"
    GemVal _t1178 = (*gem_v_peek);
    GemVal _t1179 = _t1178.fn(_t1178.env, NULL, 0);
    static GemICacheSlot _ic_172 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1179, "type", &_ic_172), gem_string("{")))) {
#line 677 "compiler/main.gem"
    GemVal _t1180 = (*gem_v_advance);
            (void)(_t1180.fn(_t1180.env, NULL, 0));
#line 678 "compiler/main.gem"
    GemVal _t1181 = gem_table_new();
            GemVal gem_v_names = _t1181;
#line 679 "compiler/main.gem"
    GemVal _t1182 = (*gem_v_skip_nl);
            (void)(_t1182.fn(_t1182.env, NULL, 0));
#line 680 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1183 = (*gem_v_peek);
                GemVal _t1184 = _t1183.fn(_t1183.env, NULL, 0);
                static GemICacheSlot _ic_173 = {0};
                if (!gem_truthy(gem_neq(gem_table_get_cached(_t1184, "type", &_ic_173), gem_string("}")))) break;
#line 681 "compiler/main.gem"
    GemVal _t1185 = (*gem_v_peek);
                GemVal gem_v_field_tok = _t1185.fn(_t1185.env, NULL, 0);
#line 682 "compiler/main.gem"
    GemVal _t1186 = gem_v_field_tok;
    static GemICacheSlot _ic_174 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1186, "type", &_ic_174), gem_string("NAME")))) {
#line 683 "compiler/main.gem"
    GemVal _t1187 = (*gem_v_advance);
                    (void)(_t1187.fn(_t1187.env, NULL, 0));
                } else {
#line 686 "compiler/main.gem"
    GemVal _t1188 = (*gem_v_advance);
                    (void)(_t1188.fn(_t1188.env, NULL, 0));
                }
#line 688 "compiler/main.gem"
    GemVal _t1189 = gem_v_field_tok;
    static GemICacheSlot _ic_175 = {0};
    GemVal _t1190[] = {gem_v_names, gem_table_get_cached(_t1189, "value", &_ic_175)};
                (void)(gem_push_fn(NULL, _t1190, 2));
#line 689 "compiler/main.gem"
    GemVal _t1191 = (*gem_v_skip_nl);
                (void)(_t1191.fn(_t1191.env, NULL, 0));
#line 690 "compiler/main.gem"
    GemVal _t1192 = (*gem_v_peek);
    GemVal _t1193 = _t1192.fn(_t1192.env, NULL, 0);
    static GemICacheSlot _ic_176 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1193, "type", &_ic_176), gem_string(",")))) {
#line 691 "compiler/main.gem"
    GemVal _t1194 = (*gem_v_advance);
                    (void)(_t1194.fn(_t1194.env, NULL, 0));
#line 692 "compiler/main.gem"
    GemVal _t1195 = (*gem_v_skip_nl);
                    (void)(_t1195.fn(_t1195.env, NULL, 0));
                }
            }
#line 695 "compiler/main.gem"
    GemVal _t1196[] = {gem_string("}")};
    GemVal _t1197 = (*gem_v_expect);
            (void)(_t1197.fn(_t1197.env, _t1196, 1));
#line 696 "compiler/main.gem"
    GemVal _t1198[] = {gem_string("=")};
    GemVal _t1199 = (*gem_v_expect);
            (void)(_t1199.fn(_t1199.env, _t1198, 1));
#line 697 "compiler/main.gem"
    GemVal _t1200 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1200.fn(_t1200.env, NULL, 0);
#line 698 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 699 "compiler/main.gem"
    GemVal _t1201[] = {gem_string("_d"), (*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_interp(2, _t1201);
#line 700 "compiler/main.gem"
    GemVal _t1202 = gem_table_new();
    GemVal _t1203[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1204 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1202, gem_int(0), _t1204.fn(_t1204.env, _t1203, 3));
            GemVal gem_v_stmts = _t1202;
#line 701 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 701 "compiler/main.gem"
    GemVal _t1205[] = {gem_v_names};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1205, 1);
#line 701 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 701 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_8;
#line 701 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 702 "compiler/main.gem"
    GemVal _t1206[] = {gem_v_tmp};
    GemVal _t1207 = (*gem_v__mod_parser_make_var);
    GemVal _t1208[] = {_t1207.fn(_t1207.env, _t1206, 1), gem_table_get(gem_v_names, gem_v_di)};
    GemVal _t1209 = (*gem_v__mod_parser_make_dot);
    GemVal _t1210[] = {gem_table_get(gem_v_names, gem_v_di), _t1209.fn(_t1209.env, _t1208, 2), gem_v_line};
    GemVal _t1211 = (*gem_v__mod_parser_make_let);
    GemVal _t1212[] = {gem_v_stmts, _t1211.fn(_t1211.env, _t1210, 3)};
                (void)(gem_push_fn(NULL, _t1212, 2));
            }

#line 704 "compiler/main.gem"
    GemVal _t1213 = gem_table_new();
    gem_table_set(_t1213, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1213, gem_string("stmts"), gem_v_stmts);
            GemVal _t1214 = _t1213;
            gem_pop_frame();
            return _t1214;
        }
#line 708 "compiler/main.gem"
    GemVal _t1215 = (*gem_v_peek);
    GemVal _t1216 = _t1215.fn(_t1215.env, NULL, 0);
    static GemICacheSlot _ic_177 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1216, "type", &_ic_177), gem_string("[")))) {
#line 709 "compiler/main.gem"
    GemVal _t1217 = (*gem_v_advance);
            (void)(_t1217.fn(_t1217.env, NULL, 0));
#line 710 "compiler/main.gem"
    GemVal _t1218 = gem_table_new();
            GemVal gem_v_names = _t1218;
#line 711 "compiler/main.gem"
    GemVal _t1219 = (*gem_v_skip_nl);
            (void)(_t1219.fn(_t1219.env, NULL, 0));
#line 712 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1220 = (*gem_v_peek);
                GemVal _t1221 = _t1220.fn(_t1220.env, NULL, 0);
                static GemICacheSlot _ic_178 = {0};
                if (!gem_truthy(gem_neq(gem_table_get_cached(_t1221, "type", &_ic_178), gem_string("]")))) break;
#line 713 "compiler/main.gem"
    GemVal _t1222[] = {gem_string("NAME")};
    GemVal _t1223 = (*gem_v_expect);
    GemVal _t1224 = _t1223.fn(_t1223.env, _t1222, 1);
    static GemICacheSlot _ic_179 = {0};
    GemVal _t1225[] = {gem_v_names, gem_table_get_cached(_t1224, "value", &_ic_179)};
                (void)(gem_push_fn(NULL, _t1225, 2));
#line 714 "compiler/main.gem"
    GemVal _t1226 = (*gem_v_skip_nl);
                (void)(_t1226.fn(_t1226.env, NULL, 0));
#line 715 "compiler/main.gem"
    GemVal _t1227 = (*gem_v_peek);
    GemVal _t1228 = _t1227.fn(_t1227.env, NULL, 0);
    static GemICacheSlot _ic_180 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1228, "type", &_ic_180), gem_string(",")))) {
#line 716 "compiler/main.gem"
    GemVal _t1229 = (*gem_v_advance);
                    (void)(_t1229.fn(_t1229.env, NULL, 0));
#line 717 "compiler/main.gem"
    GemVal _t1230 = (*gem_v_skip_nl);
                    (void)(_t1230.fn(_t1230.env, NULL, 0));
                }
            }
#line 720 "compiler/main.gem"
    GemVal _t1231[] = {gem_string("]")};
    GemVal _t1232 = (*gem_v_expect);
            (void)(_t1232.fn(_t1232.env, _t1231, 1));
#line 721 "compiler/main.gem"
    GemVal _t1233[] = {gem_string("=")};
    GemVal _t1234 = (*gem_v_expect);
            (void)(_t1234.fn(_t1234.env, _t1233, 1));
#line 722 "compiler/main.gem"
    GemVal _t1235 = (*gem_v_parse_expr);
            GemVal gem_v_rhs = _t1235.fn(_t1235.env, NULL, 0);
#line 723 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 724 "compiler/main.gem"
    GemVal _t1236[] = {gem_string("_d"), (*gem_v_gensym_counter)};
            GemVal gem_v_tmp = gem_interp(2, _t1236);
#line 725 "compiler/main.gem"
    GemVal _t1237 = gem_table_new();
    GemVal _t1238[] = {gem_v_tmp, gem_v_rhs, gem_v_line};
    GemVal _t1239 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1237, gem_int(0), _t1239.fn(_t1239.env, _t1238, 3));
            GemVal gem_v_stmts = _t1237;
#line 726 "compiler/main.gem"
            GemVal gem_v__for_i_9 = gem_int(0);
#line 726 "compiler/main.gem"
    GemVal _t1240[] = {gem_v_names};
            GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1240, 1);
#line 726 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_v__for_limit_9))) break;
#line 726 "compiler/main.gem"
                GemVal gem_v_di = gem_v__for_i_9;
#line 726 "compiler/main.gem"
                gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 727 "compiler/main.gem"
    GemVal _t1241[] = {gem_v_tmp};
    GemVal _t1242 = (*gem_v__mod_parser_make_var);
    GemVal _t1243[] = {gem_v_di};
    GemVal _t1244 = (*gem_v__mod_parser_make_int);
    GemVal _t1245[] = {_t1242.fn(_t1242.env, _t1241, 1), _t1244.fn(_t1244.env, _t1243, 1)};
    GemVal _t1246 = (*gem_v__mod_parser_make_index);
    GemVal _t1247[] = {gem_table_get(gem_v_names, gem_v_di), _t1246.fn(_t1246.env, _t1245, 2), gem_v_line};
    GemVal _t1248 = (*gem_v__mod_parser_make_let);
    GemVal _t1249[] = {gem_v_stmts, _t1248.fn(_t1248.env, _t1247, 3)};
                (void)(gem_push_fn(NULL, _t1249, 2));
            }

#line 729 "compiler/main.gem"
    GemVal _t1250 = gem_table_new();
    gem_table_set(_t1250, gem_string("tag"), gem_string("block"));
    gem_table_set(_t1250, gem_string("stmts"), gem_v_stmts);
            GemVal _t1251 = _t1250;
            gem_pop_frame();
            return _t1251;
        }
#line 732 "compiler/main.gem"
    GemVal _t1252[] = {gem_string("NAME")};
    GemVal _t1253 = (*gem_v_expect);
    GemVal _t1254 = _t1253.fn(_t1253.env, _t1252, 1);
    static GemICacheSlot _ic_181 = {0};
        GemVal gem_v_name = gem_table_get_cached(_t1254, "value", &_ic_181);
#line 733 "compiler/main.gem"
    GemVal _t1255[] = {gem_string("=")};
    GemVal _t1256 = (*gem_v_expect);
        (void)(_t1256.fn(_t1256.env, _t1255, 1));
#line 734 "compiler/main.gem"
    GemVal _t1257 = (*gem_v_parse_expr);
        GemVal gem_v_value = _t1257.fn(_t1257.env, NULL, 0);
#line 735 "compiler/main.gem"
    GemVal _t1258[] = {gem_v_name, gem_v_value, gem_v_line};
    GemVal _t1259 = (*gem_v__mod_parser_make_let);
        GemVal _t1260 = _t1259.fn(_t1259.env, _t1258, 3);
        gem_pop_frame();
        return _t1260;
    }
#line 739 "compiler/main.gem"
    GemVal _t1261 = gem_v_t;
    static GemICacheSlot _ic_182 = {0};
    GemVal _t1265;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1261, "type", &_ic_182), gem_string("fn")))) {
        _t1265 = gem_eq(gem_table_get_cached(_t1261, "type", &_ic_182), gem_string("fn"));
    } else {
        GemVal _t1262[] = {gem_int(1)};
        GemVal _t1263 = (*gem_v_peek_at);
        GemVal _t1264 = _t1263.fn(_t1263.env, _t1262, 1);
        static GemICacheSlot _ic_183 = {0};
        _t1265 = gem_eq(gem_table_get_cached(_t1264, "type", &_ic_183), gem_string("NAME"));
    }
    if (gem_truthy(_t1265)) {
#line 740 "compiler/main.gem"
    GemVal _t1266 = gem_v_t;
    static GemICacheSlot _ic_184 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1266, "line", &_ic_184);
#line 741 "compiler/main.gem"
    GemVal _t1267 = (*gem_v_advance);
        (void)(_t1267.fn(_t1267.env, NULL, 0));
#line 742 "compiler/main.gem"
    GemVal _t1268[] = {gem_string("NAME")};
    GemVal _t1269 = (*gem_v_expect);
    GemVal _t1270 = _t1269.fn(_t1269.env, _t1268, 1);
    static GemICacheSlot _ic_185 = {0};
        GemVal gem_v_name = gem_table_get_cached(_t1270, "value", &_ic_185);
#line 743 "compiler/main.gem"
        if (gem_truthy(gem_gt((*gem_v_fn_depth), gem_int(0)))) {
#line 744 "compiler/main.gem"
    GemVal _t1271[] = {gem_v_name};
    GemVal _t1272[] = {gem_string("use: let "), gem_v_name, gem_string(" = fn(...) ... end")};
    GemVal _t1273[] = {(*gem_v_source), (*gem_v_file), gem_v_line, gem_int(1), gem_add(gem_len_fn(NULL, _t1271, 1), gem_int(3)), gem_string("named fn inside function body is not supported"), gem_interp(3, _t1272)};
    GemVal _t1274 = (*gem_v__mod_parser_compile_error);
            (void)(_t1274.fn(_t1274.env, _t1273, 7));
        }
#line 746 "compiler/main.gem"
    GemVal _t1275[] = {gem_string("(")};
    GemVal _t1276 = (*gem_v_expect);
        (void)(_t1276.fn(_t1276.env, _t1275, 1));
#line 747 "compiler/main.gem"
    GemVal _t1277 = (*gem_v_parse_params);
        GemVal gem_v_pr = _t1277.fn(_t1277.env, NULL, 0);
#line 748 "compiler/main.gem"
    GemVal _t1278[] = {gem_string(")")};
    GemVal _t1279 = (*gem_v_expect);
        (void)(_t1279.fn(_t1279.env, _t1278, 1));
#line 749 "compiler/main.gem"
        *gem_v_fn_depth = gem_add((*gem_v_fn_depth), gem_int(1));
#line 750 "compiler/main.gem"
    GemVal _t1280 = (*gem_v_parse_fn_body);
        GemVal gem_v_body = _t1280.fn(_t1280.env, NULL, 0);
#line 751 "compiler/main.gem"
        *gem_v_fn_depth = gem_sub((*gem_v_fn_depth), gem_int(1));
#line 752 "compiler/main.gem"
    GemVal _t1281[] = {gem_string("end")};
    GemVal _t1282 = (*gem_v_expect);
        (void)(_t1282.fn(_t1282.env, _t1281, 1));
#line 753 "compiler/main.gem"
    GemVal _t1283 = gem_v_pr;
    static GemICacheSlot _ic_186 = {0};
    GemVal _t1284 = gem_v_pr;
    static GemICacheSlot _ic_187 = {0};
    GemVal _t1285 = gem_v_pr;
    static GemICacheSlot _ic_188 = {0};
    GemVal _t1286[] = {gem_v_name, gem_table_get_cached(_t1283, "params", &_ic_186), gem_table_get_cached(_t1284, "rest_param", &_ic_187), gem_v_body, gem_v_line, gem_table_get_cached(_t1285, "defaults", &_ic_188)};
    GemVal _t1287 = (*gem_v__mod_parser_make_fn_def);
        GemVal _t1288 = _t1287.fn(_t1287.env, _t1286, 6);
        gem_pop_frame();
        return _t1288;
    }
#line 757 "compiler/main.gem"
    GemVal _t1289 = gem_v_t;
    static GemICacheSlot _ic_189 = {0};
    GemVal _t1291;
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1289, "type", &_ic_189), gem_string("if")))) {
        _t1291 = gem_eq(gem_table_get_cached(_t1289, "type", &_ic_189), gem_string("if"));
    } else {
        GemVal _t1290 = gem_v_t;
        static GemICacheSlot _ic_190 = {0};
        _t1291 = gem_eq(gem_table_get_cached(_t1290, "type", &_ic_190), gem_string("elif"));
    }
    if (gem_truthy(_t1291)) {
#line 758 "compiler/main.gem"
    GemVal _t1292 = gem_v_t;
    static GemICacheSlot _ic_191 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1292, "line", &_ic_191);
#line 759 "compiler/main.gem"
    GemVal _t1293 = (*gem_v_advance);
        (void)(_t1293.fn(_t1293.env, NULL, 0));
#line 760 "compiler/main.gem"
    GemVal _t1294 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1294.fn(_t1294.env, NULL, 0);
#line 761 "compiler/main.gem"
    GemVal _t1295 = (*gem_v_peek);
    GemVal _t1296 = _t1295.fn(_t1295.env, NULL, 0);
    static GemICacheSlot _ic_192 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1296, "type", &_ic_192), gem_string("then")))) {
#line 762 "compiler/main.gem"
    GemVal _t1297 = (*gem_v_advance);
            (void)(_t1297.fn(_t1297.env, NULL, 0));
        }
#line 764 "compiler/main.gem"
    GemVal _t1298 = (*gem_v_parse_body);
        GemVal gem_v_then_body = _t1298.fn(_t1298.env, NULL, 0);
#line 765 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 766 "compiler/main.gem"
    GemVal _t1299 = (*gem_v_peek);
    GemVal _t1300 = _t1299.fn(_t1299.env, NULL, 0);
    static GemICacheSlot _ic_193 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1300, "type", &_ic_193), gem_string("elif")))) {
#line 768 "compiler/main.gem"
    GemVal _t1301 = gem_table_new();
    GemVal _t1302 = (*gem_v_parse_stmt);
    gem_table_set(_t1301, gem_int(0), _t1302.fn(_t1302.env, NULL, 0));
            gem_v_else_body = _t1301;
        } else {
#line 769 "compiler/main.gem"
    GemVal _t1303 = (*gem_v_peek);
    GemVal _t1304 = _t1303.fn(_t1303.env, NULL, 0);
    static GemICacheSlot _ic_194 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1304, "type", &_ic_194), gem_string("else")))) {
#line 770 "compiler/main.gem"
    GemVal _t1305 = (*gem_v_advance);
                (void)(_t1305.fn(_t1305.env, NULL, 0));
#line 771 "compiler/main.gem"
    GemVal _t1306 = (*gem_v_peek);
    GemVal _t1307 = _t1306.fn(_t1306.env, NULL, 0);
    static GemICacheSlot _ic_195 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1307, "type", &_ic_195), gem_string("if")))) {
#line 772 "compiler/main.gem"
    GemVal _t1308 = (*gem_v_peek);
                    GemVal gem_v_err_tok = _t1308.fn(_t1308.env, NULL, 0);
#line 773 "compiler/main.gem"
    GemVal _t1309 = gem_v_err_tok;
    static GemICacheSlot _ic_196 = {0};
    GemVal _t1310 = gem_v_err_tok;
    static GemICacheSlot _ic_197 = {0};
    GemVal _t1311[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1309, "line", &_ic_196), gem_table_get_cached(_t1310, "col", &_ic_197), gem_int(2), gem_string("use 'elif' instead of 'else if'"), gem_string("Gem uses 'elif' for chained conditions")};
    GemVal _t1312 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1312.fn(_t1312.env, _t1311, 7));
                }
#line 775 "compiler/main.gem"
    GemVal _t1313 = (*gem_v_parse_body);
                gem_v_else_body = _t1313.fn(_t1313.env, NULL, 0);
            }
        }
#line 778 "compiler/main.gem"
    GemVal _t1314 = gem_v_t;
    static GemICacheSlot _ic_198 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1314, "type", &_ic_198), gem_string("if")))) {
#line 779 "compiler/main.gem"
    GemVal _t1315[] = {gem_string("end")};
    GemVal _t1316 = (*gem_v_expect);
            (void)(_t1316.fn(_t1316.env, _t1315, 1));
        }
#line 781 "compiler/main.gem"
    GemVal _t1317[] = {gem_v_cond, gem_v_then_body, gem_v_else_body, gem_v_line};
    GemVal _t1318 = (*gem_v__mod_parser_make_if);
        GemVal _t1319 = _t1318.fn(_t1318.env, _t1317, 4);
        gem_pop_frame();
        return _t1319;
    }
#line 785 "compiler/main.gem"
    GemVal _t1320 = gem_v_t;
    static GemICacheSlot _ic_199 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1320, "type", &_ic_199), gem_string("while")))) {
#line 786 "compiler/main.gem"
    GemVal _t1321 = gem_v_t;
    static GemICacheSlot _ic_200 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1321, "line", &_ic_200);
#line 787 "compiler/main.gem"
    GemVal _t1322 = (*gem_v_advance);
        (void)(_t1322.fn(_t1322.env, NULL, 0));
#line 788 "compiler/main.gem"
    GemVal _t1323 = (*gem_v_parse_expr);
        GemVal gem_v_cond = _t1323.fn(_t1323.env, NULL, 0);
#line 789 "compiler/main.gem"
    GemVal _t1324 = (*gem_v_parse_body);
        GemVal gem_v_wbody = _t1324.fn(_t1324.env, NULL, 0);
#line 790 "compiler/main.gem"
    GemVal _t1325[] = {gem_string("end")};
    GemVal _t1326 = (*gem_v_expect);
        (void)(_t1326.fn(_t1326.env, _t1325, 1));
#line 791 "compiler/main.gem"
    GemVal _t1327[] = {gem_v_cond, gem_v_wbody, gem_v_line};
    GemVal _t1328 = (*gem_v__mod_parser_make_while);
        GemVal _t1329 = _t1328.fn(_t1328.env, _t1327, 3);
        gem_pop_frame();
        return _t1329;
    }
#line 795 "compiler/main.gem"
    GemVal _t1330 = gem_v_t;
    static GemICacheSlot _ic_201 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1330, "type", &_ic_201), gem_string("for")))) {
#line 796 "compiler/main.gem"
    GemVal _t1331 = gem_v_t;
    static GemICacheSlot _ic_202 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1331, "line", &_ic_202);
#line 797 "compiler/main.gem"
    GemVal _t1332 = (*gem_v_advance);
        (void)(_t1332.fn(_t1332.env, NULL, 0));
#line 798 "compiler/main.gem"
    GemVal _t1333[] = {gem_string("NAME")};
    GemVal _t1334 = (*gem_v_expect);
    GemVal _t1335 = _t1334.fn(_t1334.env, _t1333, 1);
    static GemICacheSlot _ic_203 = {0};
        GemVal gem_v_var_name = gem_table_get_cached(_t1335, "value", &_ic_203);
#line 800 "compiler/main.gem"
    GemVal _t1336 = (*gem_v_peek);
    GemVal _t1337 = _t1336.fn(_t1336.env, NULL, 0);
    static GemICacheSlot _ic_204 = {0};
    GemVal _t1341;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1337, "type", &_ic_204), gem_string(",")))) {
        _t1341 = gem_eq(gem_table_get_cached(_t1337, "type", &_ic_204), gem_string(","));
    } else {
        GemVal _t1338[] = {gem_int(1)};
        GemVal _t1339 = (*gem_v_peek_at);
        GemVal _t1340 = _t1339.fn(_t1339.env, _t1338, 1);
        static GemICacheSlot _ic_205 = {0};
        _t1341 = gem_eq(gem_table_get_cached(_t1340, "type", &_ic_205), gem_string("NAME"));
    }
        if (gem_truthy(_t1341)) {
#line 802 "compiler/main.gem"
    GemVal _t1342 = (*gem_v_advance);
            (void)(_t1342.fn(_t1342.env, NULL, 0));
#line 803 "compiler/main.gem"
    GemVal _t1343[] = {gem_string("NAME")};
    GemVal _t1344 = (*gem_v_expect);
    GemVal _t1345 = _t1344.fn(_t1344.env, _t1343, 1);
    static GemICacheSlot _ic_206 = {0};
            GemVal gem_v_val_name = gem_table_get_cached(_t1345, "value", &_ic_206);
#line 804 "compiler/main.gem"
    GemVal _t1346[] = {gem_string("in")};
    GemVal _t1347 = (*gem_v_expect);
            (void)(_t1347.fn(_t1347.env, _t1346, 1));
#line 805 "compiler/main.gem"
    GemVal _t1348 = (*gem_v_parse_expr);
            GemVal gem_v_tbl_expr = _t1348.fn(_t1348.env, NULL, 0);
#line 806 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 807 "compiler/main.gem"
    GemVal _t1349[] = {gem_string("_for_tbl_"), (*gem_v_gensym_counter)};
            GemVal gem_v_tbl_var = gem_interp(2, _t1349);
#line 808 "compiler/main.gem"
    GemVal _t1350[] = {gem_string("_for_len_"), (*gem_v_gensym_counter)};
            GemVal gem_v_len_var = gem_interp(2, _t1350);
#line 809 "compiler/main.gem"
    GemVal _t1351[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
            GemVal gem_v_idx_var = gem_interp(2, _t1351);
#line 810 "compiler/main.gem"
    GemVal _t1352 = (*gem_v_parse_body);
            GemVal gem_v_fbody = _t1352.fn(_t1352.env, NULL, 0);
#line 811 "compiler/main.gem"
    GemVal _t1353[] = {gem_string("end")};
    GemVal _t1354 = (*gem_v_expect);
            (void)(_t1354.fn(_t1354.env, _t1353, 1));
#line 812 "compiler/main.gem"
    GemVal _t1355 = gem_table_new();
            GemVal gem_v_inner_stmts = _t1355;
#line 813 "compiler/main.gem"
    GemVal _t1356[] = {gem_string("__table_key_at")};
    GemVal _t1357 = (*gem_v__mod_parser_make_var);
    GemVal _t1358 = gem_table_new();
    GemVal _t1359[] = {gem_v_tbl_var};
    GemVal _t1360 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1358, gem_int(0), _t1360.fn(_t1360.env, _t1359, 1));
    GemVal _t1361[] = {gem_v_idx_var};
    GemVal _t1362 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1358, gem_int(1), _t1362.fn(_t1362.env, _t1361, 1));
    GemVal _t1363[] = {_t1357.fn(_t1357.env, _t1356, 1), _t1358, gem_int(0)};
    GemVal _t1364 = (*gem_v__mod_parser_make_call);
    GemVal _t1365[] = {gem_v_var_name, _t1364.fn(_t1364.env, _t1363, 3), gem_v_line};
    GemVal _t1366 = (*gem_v__mod_parser_make_let);
    GemVal _t1367[] = {gem_v_inner_stmts, _t1366.fn(_t1366.env, _t1365, 3)};
            (void)(gem_push_fn(NULL, _t1367, 2));
#line 814 "compiler/main.gem"
    GemVal _t1368[] = {gem_string("__table_val_at")};
    GemVal _t1369 = (*gem_v__mod_parser_make_var);
    GemVal _t1370 = gem_table_new();
    GemVal _t1371[] = {gem_v_tbl_var};
    GemVal _t1372 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1370, gem_int(0), _t1372.fn(_t1372.env, _t1371, 1));
    GemVal _t1373[] = {gem_v_idx_var};
    GemVal _t1374 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1370, gem_int(1), _t1374.fn(_t1374.env, _t1373, 1));
    GemVal _t1375[] = {_t1369.fn(_t1369.env, _t1368, 1), _t1370, gem_int(0)};
    GemVal _t1376 = (*gem_v__mod_parser_make_call);
    GemVal _t1377[] = {gem_v_val_name, _t1376.fn(_t1376.env, _t1375, 3), gem_v_line};
    GemVal _t1378 = (*gem_v__mod_parser_make_let);
    GemVal _t1379[] = {gem_v_inner_stmts, _t1378.fn(_t1378.env, _t1377, 3)};
            (void)(gem_push_fn(NULL, _t1379, 2));
#line 815 "compiler/main.gem"
    GemVal _t1380[] = {gem_v_idx_var};
    GemVal _t1381 = (*gem_v__mod_parser_make_var);
    GemVal _t1382[] = {gem_int(1)};
    GemVal _t1383 = (*gem_v__mod_parser_make_int);
    GemVal _t1384[] = {gem_string("+"), _t1381.fn(_t1381.env, _t1380, 1), _t1383.fn(_t1383.env, _t1382, 1)};
    GemVal _t1385 = (*gem_v__mod_parser_make_binop);
    GemVal _t1386[] = {gem_v_idx_var, _t1385.fn(_t1385.env, _t1384, 3), gem_v_line};
    GemVal _t1387 = (*gem_v__mod_parser_make_assign);
    GemVal _t1388[] = {gem_v_inner_stmts, _t1387.fn(_t1387.env, _t1386, 3)};
            (void)(gem_push_fn(NULL, _t1388, 2));
#line 816 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 816 "compiler/main.gem"
    GemVal _t1389[] = {gem_v_fbody};
            GemVal gem_v__for_limit_10 = gem_len_fn(NULL, _t1389, 1);
#line 816 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_limit_10))) break;
#line 816 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_10;
#line 816 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 817 "compiler/main.gem"
    GemVal _t1390[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                (void)(gem_push_fn(NULL, _t1390, 2));
            }

#line 819 "compiler/main.gem"
    GemVal _t1391[] = {gem_v_idx_var};
    GemVal _t1392 = (*gem_v__mod_parser_make_var);
    GemVal _t1393[] = {gem_v_len_var};
    GemVal _t1394 = (*gem_v__mod_parser_make_var);
    GemVal _t1395[] = {gem_string("<"), _t1392.fn(_t1392.env, _t1391, 1), _t1394.fn(_t1394.env, _t1393, 1)};
    GemVal _t1396 = (*gem_v__mod_parser_make_binop);
    GemVal _t1397[] = {_t1396.fn(_t1396.env, _t1395, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1398 = (*gem_v__mod_parser_make_while);
            GemVal gem_v_while_node = _t1398.fn(_t1398.env, _t1397, 3);
#line 824 "compiler/main.gem"
    GemVal _t1399 = gem_table_new();
    gem_table_set(_t1399, gem_string("tag"), gem_string("block"));
    GemVal _t1400 = gem_table_new();
    GemVal _t1401[] = {gem_v_tbl_var, gem_v_tbl_expr, gem_v_line};
    GemVal _t1402 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1400, gem_int(0), _t1402.fn(_t1402.env, _t1401, 3));
    GemVal _t1403[] = {gem_string("len")};
    GemVal _t1404 = (*gem_v__mod_parser_make_var);
    GemVal _t1405 = gem_table_new();
    GemVal _t1406[] = {gem_v_tbl_var};
    GemVal _t1407 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1405, gem_int(0), _t1407.fn(_t1407.env, _t1406, 1));
    GemVal _t1408[] = {_t1404.fn(_t1404.env, _t1403, 1), _t1405, gem_int(0)};
    GemVal _t1409 = (*gem_v__mod_parser_make_call);
    GemVal _t1410[] = {gem_v_len_var, _t1409.fn(_t1409.env, _t1408, 3), gem_v_line};
    GemVal _t1411 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1400, gem_int(1), _t1411.fn(_t1411.env, _t1410, 3));
    GemVal _t1412[] = {gem_int(0)};
    GemVal _t1413 = (*gem_v__mod_parser_make_int);
    GemVal _t1414[] = {gem_v_idx_var, _t1413.fn(_t1413.env, _t1412, 1), gem_v_line};
    GemVal _t1415 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1400, gem_int(2), _t1415.fn(_t1415.env, _t1414, 3));
    gem_table_set(_t1400, gem_int(3), gem_v_while_node);
    gem_table_set(_t1399, gem_string("stmts"), _t1400);
            GemVal _t1416 = _t1399;
            gem_pop_frame();
            return _t1416;
        } else {
#line 830 "compiler/main.gem"
    GemVal _t1417 = (*gem_v_peek);
    GemVal _t1418 = _t1417.fn(_t1417.env, NULL, 0);
    static GemICacheSlot _ic_207 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1418, "type", &_ic_207), gem_string("in")))) {
#line 832 "compiler/main.gem"
    GemVal _t1419 = (*gem_v_advance);
                (void)(_t1419.fn(_t1419.env, NULL, 0));
#line 833 "compiler/main.gem"
    GemVal _t1420 = (*gem_v_parse_expr);
                GemVal gem_v_items_expr = _t1420.fn(_t1420.env, NULL, 0);
#line 834 "compiler/main.gem"
                *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 835 "compiler/main.gem"
    GemVal _t1421[] = {gem_string("_for_items_"), (*gem_v_gensym_counter)};
                GemVal gem_v_items_var = gem_interp(2, _t1421);
#line 836 "compiler/main.gem"
    GemVal _t1422[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
                GemVal gem_v_idx_var = gem_interp(2, _t1422);
#line 837 "compiler/main.gem"
    GemVal _t1423 = (*gem_v_parse_body);
                GemVal gem_v_fbody = _t1423.fn(_t1423.env, NULL, 0);
#line 838 "compiler/main.gem"
    GemVal _t1424[] = {gem_string("end")};
    GemVal _t1425 = (*gem_v_expect);
                (void)(_t1425.fn(_t1425.env, _t1424, 1));
#line 839 "compiler/main.gem"
    GemVal _t1426 = gem_table_new();
                GemVal gem_v_inner_stmts = _t1426;
#line 840 "compiler/main.gem"
    GemVal _t1427[] = {gem_v_items_var};
    GemVal _t1428 = (*gem_v__mod_parser_make_var);
    GemVal _t1429[] = {gem_v_idx_var};
    GemVal _t1430 = (*gem_v__mod_parser_make_var);
    GemVal _t1431[] = {_t1428.fn(_t1428.env, _t1427, 1), _t1430.fn(_t1430.env, _t1429, 1)};
    GemVal _t1432 = (*gem_v__mod_parser_make_index);
    GemVal _t1433[] = {gem_v_var_name, _t1432.fn(_t1432.env, _t1431, 2), gem_v_line};
    GemVal _t1434 = (*gem_v__mod_parser_make_let);
    GemVal _t1435[] = {gem_v_inner_stmts, _t1434.fn(_t1434.env, _t1433, 3)};
                (void)(gem_push_fn(NULL, _t1435, 2));
#line 841 "compiler/main.gem"
    GemVal _t1436[] = {gem_v_idx_var};
    GemVal _t1437 = (*gem_v__mod_parser_make_var);
    GemVal _t1438[] = {gem_int(1)};
    GemVal _t1439 = (*gem_v__mod_parser_make_int);
    GemVal _t1440[] = {gem_string("+"), _t1437.fn(_t1437.env, _t1436, 1), _t1439.fn(_t1439.env, _t1438, 1)};
    GemVal _t1441 = (*gem_v__mod_parser_make_binop);
    GemVal _t1442[] = {gem_v_idx_var, _t1441.fn(_t1441.env, _t1440, 3), gem_v_line};
    GemVal _t1443 = (*gem_v__mod_parser_make_assign);
    GemVal _t1444[] = {gem_v_inner_stmts, _t1443.fn(_t1443.env, _t1442, 3)};
                (void)(gem_push_fn(NULL, _t1444, 2));
#line 842 "compiler/main.gem"
                GemVal gem_v__for_i_11 = gem_int(0);
#line 842 "compiler/main.gem"
    GemVal _t1445[] = {gem_v_fbody};
                GemVal gem_v__for_limit_11 = gem_len_fn(NULL, _t1445, 1);
#line 842 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_v__for_limit_11))) break;
#line 842 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_11;
#line 842 "compiler/main.gem"
                    gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 843 "compiler/main.gem"
    GemVal _t1446[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                    (void)(gem_push_fn(NULL, _t1446, 2));
                }

#line 845 "compiler/main.gem"
    GemVal _t1447[] = {gem_v_idx_var};
    GemVal _t1448 = (*gem_v__mod_parser_make_var);
    GemVal _t1449[] = {gem_string("len")};
    GemVal _t1450 = (*gem_v__mod_parser_make_var);
    GemVal _t1451 = gem_table_new();
    GemVal _t1452[] = {gem_v_items_var};
    GemVal _t1453 = (*gem_v__mod_parser_make_var);
    gem_table_set(_t1451, gem_int(0), _t1453.fn(_t1453.env, _t1452, 1));
    GemVal _t1454[] = {_t1450.fn(_t1450.env, _t1449, 1), _t1451, gem_int(0)};
    GemVal _t1455 = (*gem_v__mod_parser_make_call);
    GemVal _t1456[] = {gem_string("<"), _t1448.fn(_t1448.env, _t1447, 1), _t1455.fn(_t1455.env, _t1454, 3)};
    GemVal _t1457 = (*gem_v__mod_parser_make_binop);
    GemVal _t1458[] = {_t1457.fn(_t1457.env, _t1456, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1459 = (*gem_v__mod_parser_make_while);
                GemVal gem_v_while_node = _t1459.fn(_t1459.env, _t1458, 3);
#line 850 "compiler/main.gem"
    GemVal _t1460 = gem_table_new();
    gem_table_set(_t1460, gem_string("tag"), gem_string("block"));
    GemVal _t1461 = gem_table_new();
    GemVal _t1462[] = {gem_v_items_var, gem_v_items_expr, gem_v_line};
    GemVal _t1463 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1461, gem_int(0), _t1463.fn(_t1463.env, _t1462, 3));
    GemVal _t1464[] = {gem_int(0)};
    GemVal _t1465 = (*gem_v__mod_parser_make_int);
    GemVal _t1466[] = {gem_v_idx_var, _t1465.fn(_t1465.env, _t1464, 1), gem_v_line};
    GemVal _t1467 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1461, gem_int(1), _t1467.fn(_t1467.env, _t1466, 3));
    gem_table_set(_t1461, gem_int(2), gem_v_while_node);
    gem_table_set(_t1460, gem_string("stmts"), _t1461);
                GemVal _t1468 = _t1460;
                gem_pop_frame();
                return _t1468;
            } else {
#line 855 "compiler/main.gem"
    GemVal _t1469 = (*gem_v_peek);
    GemVal _t1470 = _t1469.fn(_t1469.env, NULL, 0);
    static GemICacheSlot _ic_208 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t1470, "type", &_ic_208), gem_string("=")))) {
#line 857 "compiler/main.gem"
    GemVal _t1471 = (*gem_v_advance);
                    (void)(_t1471.fn(_t1471.env, NULL, 0));
#line 858 "compiler/main.gem"
    GemVal _t1472 = (*gem_v_parse_expr);
                    GemVal gem_v_start_expr = _t1472.fn(_t1472.env, NULL, 0);
#line 859 "compiler/main.gem"
    GemVal _t1473[] = {gem_string(",")};
    GemVal _t1474 = (*gem_v_expect);
                    (void)(_t1474.fn(_t1474.env, _t1473, 1));
#line 860 "compiler/main.gem"
    GemVal _t1475 = (*gem_v_parse_expr);
                    GemVal gem_v_end_expr = _t1475.fn(_t1475.env, NULL, 0);
#line 861 "compiler/main.gem"
    GemVal _t1476 = (*gem_v_parse_body);
                    GemVal gem_v_fbody = _t1476.fn(_t1476.env, NULL, 0);
#line 862 "compiler/main.gem"
    GemVal _t1477[] = {gem_string("end")};
    GemVal _t1478 = (*gem_v_expect);
                    (void)(_t1478.fn(_t1478.env, _t1477, 1));
#line 863 "compiler/main.gem"
                    *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 864 "compiler/main.gem"
    GemVal _t1479[] = {gem_string("_for_i_"), (*gem_v_gensym_counter)};
                    GemVal gem_v_idx_var = gem_interp(2, _t1479);
#line 865 "compiler/main.gem"
    GemVal _t1480[] = {gem_string("_for_limit_"), (*gem_v_gensym_counter)};
                    GemVal gem_v_limit_var = gem_interp(2, _t1480);
#line 866 "compiler/main.gem"
    GemVal _t1481 = gem_table_new();
                    GemVal gem_v_inner_stmts = _t1481;
#line 867 "compiler/main.gem"
    GemVal _t1482[] = {gem_v_idx_var};
    GemVal _t1483 = (*gem_v__mod_parser_make_var);
    GemVal _t1484[] = {gem_v_var_name, _t1483.fn(_t1483.env, _t1482, 1), gem_v_line};
    GemVal _t1485 = (*gem_v__mod_parser_make_let);
    GemVal _t1486[] = {gem_v_inner_stmts, _t1485.fn(_t1485.env, _t1484, 3)};
                    (void)(gem_push_fn(NULL, _t1486, 2));
#line 868 "compiler/main.gem"
    GemVal _t1487[] = {gem_v_idx_var};
    GemVal _t1488 = (*gem_v__mod_parser_make_var);
    GemVal _t1489[] = {gem_int(1)};
    GemVal _t1490 = (*gem_v__mod_parser_make_int);
    GemVal _t1491[] = {gem_string("+"), _t1488.fn(_t1488.env, _t1487, 1), _t1490.fn(_t1490.env, _t1489, 1)};
    GemVal _t1492 = (*gem_v__mod_parser_make_binop);
    GemVal _t1493[] = {gem_v_idx_var, _t1492.fn(_t1492.env, _t1491, 3), gem_v_line};
    GemVal _t1494 = (*gem_v__mod_parser_make_assign);
    GemVal _t1495[] = {gem_v_inner_stmts, _t1494.fn(_t1494.env, _t1493, 3)};
                    (void)(gem_push_fn(NULL, _t1495, 2));
#line 869 "compiler/main.gem"
                    GemVal gem_v__for_i_12 = gem_int(0);
#line 869 "compiler/main.gem"
    GemVal _t1496[] = {gem_v_fbody};
                    GemVal gem_v__for_limit_12 = gem_len_fn(NULL, _t1496, 1);
#line 869 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_v__for_limit_12))) break;
#line 869 "compiler/main.gem"
                        GemVal gem_v_bi = gem_v__for_i_12;
#line 869 "compiler/main.gem"
                        gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 870 "compiler/main.gem"
    GemVal _t1497[] = {gem_v_inner_stmts, gem_table_get(gem_v_fbody, gem_v_bi)};
                        (void)(gem_push_fn(NULL, _t1497, 2));
                    }

#line 872 "compiler/main.gem"
    GemVal _t1498[] = {gem_v_idx_var};
    GemVal _t1499 = (*gem_v__mod_parser_make_var);
    GemVal _t1500[] = {gem_v_limit_var};
    GemVal _t1501 = (*gem_v__mod_parser_make_var);
    GemVal _t1502[] = {gem_string("<"), _t1499.fn(_t1499.env, _t1498, 1), _t1501.fn(_t1501.env, _t1500, 1)};
    GemVal _t1503 = (*gem_v__mod_parser_make_binop);
    GemVal _t1504[] = {_t1503.fn(_t1503.env, _t1502, 3), gem_v_inner_stmts, gem_v_line};
    GemVal _t1505 = (*gem_v__mod_parser_make_while);
                    GemVal gem_v_while_node = _t1505.fn(_t1505.env, _t1504, 3);
#line 877 "compiler/main.gem"
    GemVal _t1506 = gem_table_new();
    gem_table_set(_t1506, gem_string("tag"), gem_string("block"));
    GemVal _t1507 = gem_table_new();
    GemVal _t1508[] = {gem_v_idx_var, gem_v_start_expr, gem_v_line};
    GemVal _t1509 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1507, gem_int(0), _t1509.fn(_t1509.env, _t1508, 3));
    GemVal _t1510[] = {gem_v_limit_var, gem_v_end_expr, gem_v_line};
    GemVal _t1511 = (*gem_v__mod_parser_make_let);
    gem_table_set(_t1507, gem_int(1), _t1511.fn(_t1511.env, _t1510, 3));
    gem_table_set(_t1507, gem_int(2), gem_v_while_node);
    gem_table_set(_t1506, gem_string("stmts"), _t1507);
                    GemVal _t1512 = _t1506;
                    gem_pop_frame();
                    return _t1512;
                } else {
#line 883 "compiler/main.gem"
    GemVal _t1513 = (*gem_v_peek);
    GemVal _t1514 = _t1513.fn(_t1513.env, NULL, 0);
    static GemICacheSlot _ic_209 = {0};
    GemVal _t1515 = (*gem_v_peek);
    GemVal _t1516 = _t1515.fn(_t1515.env, NULL, 0);
    static GemICacheSlot _ic_210 = {0};
    GemVal _t1517 = (*gem_v_peek);
    GemVal _t1518 = _t1517.fn(_t1517.env, NULL, 0);
    static GemICacheSlot _ic_211 = {0};
    GemVal _t1519[] = {gem_table_get_cached(_t1518, "value", &_ic_211)};
    GemVal _t1520[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1514, "line", &_ic_209), gem_table_get_cached(_t1516, "col", &_ic_210), gem_len_fn(NULL, _t1519, 1), gem_string("expected 'in' or '=' after for variable"), GEM_NIL};
    GemVal _t1521 = (*gem_v__mod_parser_compile_error);
                    (void)(_t1521.fn(_t1521.env, _t1520, 7));
                }
            }
        }
    }
#line 888 "compiler/main.gem"
    GemVal _t1522 = gem_v_t;
    static GemICacheSlot _ic_212 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1522, "type", &_ic_212), gem_string("match")))) {
#line 889 "compiler/main.gem"
    GemVal _t1523 = gem_v_t;
    static GemICacheSlot _ic_213 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1523, "line", &_ic_213);
#line 890 "compiler/main.gem"
    GemVal _t1524 = (*gem_v_advance);
        (void)(_t1524.fn(_t1524.env, NULL, 0));
#line 891 "compiler/main.gem"
    GemVal _t1525 = (*gem_v_parse_expr);
        GemVal gem_v_target = _t1525.fn(_t1525.env, NULL, 0);
#line 892 "compiler/main.gem"
    GemVal _t1526 = (*gem_v_skip_nl);
        (void)(_t1526.fn(_t1526.env, NULL, 0));
#line 893 "compiler/main.gem"
        *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 894 "compiler/main.gem"
    GemVal _t1527[] = {gem_string("_match_"), (*gem_v_gensym_counter)};
        GemVal gem_v_match_var = gem_interp(2, _t1527);
#line 895 "compiler/main.gem"
    GemVal _t1528[] = {gem_v_match_var};
    GemVal _t1529 = (*gem_v__mod_parser_make_var);
        GemVal gem_v_target_var_expr = _t1529.fn(_t1529.env, _t1528, 1);
#line 896 "compiler/main.gem"
    GemVal _t1530 = gem_table_new();
        GemVal gem_v_whens = _t1530;
#line 897 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1531 = (*gem_v_peek);
            GemVal _t1532 = _t1531.fn(_t1531.env, NULL, 0);
            static GemICacheSlot _ic_214 = {0};
            if (!gem_truthy(gem_eq(gem_table_get_cached(_t1532, "type", &_ic_214), gem_string("when")))) break;
#line 898 "compiler/main.gem"
    GemVal _t1533 = (*gem_v_advance);
            (void)(_t1533.fn(_t1533.env, NULL, 0));
#line 900 "compiler/main.gem"
    GemVal _t1534 = (*gem_v_peek);
    GemVal _t1535 = _t1534.fn(_t1534.env, NULL, 0);
    static GemICacheSlot _ic_215 = {0};
            GemVal gem_v_pt = gem_table_get_cached(_t1535, "type", &_ic_215);
#line 901 "compiler/main.gem"
    GemVal _t1536[] = {gem_int(1)};
    GemVal _t1537 = (*gem_v_peek_at);
    GemVal _t1538 = _t1537.fn(_t1537.env, _t1536, 1);
    static GemICacheSlot _ic_216 = {0};
            GemVal gem_v_next = gem_table_get_cached(_t1538, "type", &_ic_216);
#line 902 "compiler/main.gem"
    GemVal _t1539;
    if (gem_truthy(gem_eq(gem_v_pt, gem_string("{")))) {
        _t1539 = gem_eq(gem_v_pt, gem_string("{"));
    } else {
        _t1539 = gem_eq(gem_v_pt, gem_string("["));
    }
            if (gem_truthy(_t1539)) {
#line 903 "compiler/main.gem"
    GemVal _t1540[] = {gem_v_target_var_expr};
    GemVal _t1541 = (*gem_v_parse_pattern);
                GemVal gem_v_pat = _t1541.fn(_t1541.env, _t1540, 1);
#line 904 "compiler/main.gem"
    GemVal _t1542 = (*gem_v_parse_body);
                GemVal gem_v_wbody = _t1542.fn(_t1542.env, NULL, 0);
#line 905 "compiler/main.gem"
    GemVal _t1543 = gem_v_pat;
    static GemICacheSlot _ic_217 = {0};
    GemVal _t1544 = gem_v_pat;
    static GemICacheSlot _ic_218 = {0};
    GemVal _t1545[] = {gem_table_get_cached(_t1543, "condition", &_ic_217), gem_v_wbody, gem_table_get_cached(_t1544, "bindings", &_ic_218)};
    GemVal _t1546 = (*gem_v__mod_parser_make_when);
    GemVal _t1547[] = {gem_v_whens, _t1546.fn(_t1546.env, _t1545, 3)};
                (void)(gem_push_fn(NULL, _t1547, 2));
            } else {
#line 906 "compiler/main.gem"
    GemVal _t1552;
    if (!gem_truthy(gem_eq(gem_v_pt, gem_string("NAME")))) {
        _t1552 = gem_eq(gem_v_pt, gem_string("NAME"));
    } else {
        GemVal _t1548;
        if (gem_truthy(gem_eq(gem_v_next, gem_string("NEWLINE")))) {
                _t1548 = gem_eq(gem_v_next, gem_string("NEWLINE"));
        } else {
                _t1548 = gem_eq(gem_v_next, gem_string("EOF"));
        }
        GemVal _t1549;
        if (gem_truthy(_t1548)) {
                _t1549 = _t1548;
        } else {
                _t1549 = gem_eq(gem_v_next, gem_string("end"));
        }
        GemVal _t1550;
        if (gem_truthy(_t1549)) {
                _t1550 = _t1549;
        } else {
                _t1550 = gem_eq(gem_v_next, gem_string("else"));
        }
        GemVal _t1551;
        if (gem_truthy(_t1550)) {
                _t1551 = _t1550;
        } else {
                _t1551 = gem_eq(gem_v_next, gem_string("when"));
        }
        _t1552 = _t1551;
    }
                if (gem_truthy(_t1552)) {
#line 908 "compiler/main.gem"
    GemVal _t1553[] = {gem_v_target_var_expr};
    GemVal _t1554 = (*gem_v_parse_pattern);
                    GemVal gem_v_pat = _t1554.fn(_t1554.env, _t1553, 1);
#line 909 "compiler/main.gem"
    GemVal _t1555 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1555.fn(_t1555.env, NULL, 0);
#line 910 "compiler/main.gem"
    GemVal _t1556 = gem_v_pat;
    static GemICacheSlot _ic_219 = {0};
    GemVal _t1557 = gem_v_pat;
    static GemICacheSlot _ic_220 = {0};
    GemVal _t1558[] = {gem_table_get_cached(_t1556, "condition", &_ic_219), gem_v_wbody, gem_table_get_cached(_t1557, "bindings", &_ic_220)};
    GemVal _t1559 = (*gem_v__mod_parser_make_when);
    GemVal _t1560[] = {gem_v_whens, _t1559.fn(_t1559.env, _t1558, 3)};
                    (void)(gem_push_fn(NULL, _t1560, 2));
                } else {
#line 913 "compiler/main.gem"
    GemVal _t1561 = (*gem_v_parse_expr);
                    GemVal gem_v_wval = _t1561.fn(_t1561.env, NULL, 0);
#line 914 "compiler/main.gem"
    GemVal _t1562 = (*gem_v_parse_body);
                    GemVal gem_v_wbody = _t1562.fn(_t1562.env, NULL, 0);
#line 915 "compiler/main.gem"
    GemVal _t1563[] = {gem_string("=="), gem_v_target_var_expr, gem_v_wval};
    GemVal _t1564 = (*gem_v__mod_parser_make_binop);
    GemVal _t1565 = gem_table_new();
    GemVal _t1566[] = {_t1564.fn(_t1564.env, _t1563, 3), gem_v_wbody, _t1565};
    GemVal _t1567 = (*gem_v__mod_parser_make_when);
    GemVal _t1568[] = {gem_v_whens, _t1567.fn(_t1567.env, _t1566, 3)};
                    (void)(gem_push_fn(NULL, _t1568, 2));
                }
            }
#line 917 "compiler/main.gem"
    GemVal _t1569 = (*gem_v_skip_nl);
            (void)(_t1569.fn(_t1569.env, NULL, 0));
        }
#line 919 "compiler/main.gem"
        GemVal gem_v_else_body = GEM_NIL;
#line 920 "compiler/main.gem"
    GemVal _t1570 = (*gem_v_peek);
    GemVal _t1571 = _t1570.fn(_t1570.env, NULL, 0);
    static GemICacheSlot _ic_221 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1571, "type", &_ic_221), gem_string("else")))) {
#line 921 "compiler/main.gem"
    GemVal _t1572 = (*gem_v_advance);
            (void)(_t1572.fn(_t1572.env, NULL, 0));
#line 922 "compiler/main.gem"
    GemVal _t1573 = (*gem_v_parse_body);
            gem_v_else_body = _t1573.fn(_t1573.env, NULL, 0);
        }
#line 924 "compiler/main.gem"
    GemVal _t1574[] = {gem_string("end")};
    GemVal _t1575 = (*gem_v_expect);
        (void)(_t1575.fn(_t1575.env, _t1574, 1));
#line 925 "compiler/main.gem"
    GemVal _t1576[] = {gem_v_target, gem_v_whens, gem_v_else_body, gem_v_line, gem_v_match_var};
    GemVal _t1577 = (*gem_v__mod_parser_make_match);
        GemVal _t1578 = _t1577.fn(_t1577.env, _t1576, 5);
        gem_pop_frame();
        return _t1578;
    }
#line 929 "compiler/main.gem"
    GemVal _t1579 = gem_v_t;
    static GemICacheSlot _ic_222 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1579, "type", &_ic_222), gem_string("return")))) {
#line 930 "compiler/main.gem"
    GemVal _t1580 = gem_v_t;
    static GemICacheSlot _ic_223 = {0};
        GemVal gem_v_line = gem_table_get_cached(_t1580, "line", &_ic_223);
#line 931 "compiler/main.gem"
    GemVal _t1581 = (*gem_v_advance);
        (void)(_t1581.fn(_t1581.env, NULL, 0));
#line 932 "compiler/main.gem"
        GemVal gem_v_value = GEM_NIL;
#line 933 "compiler/main.gem"
    GemVal _t1582 = (*gem_v_peek);
    GemVal _t1583 = _t1582.fn(_t1582.env, NULL, 0);
    static GemICacheSlot _ic_224 = {0};
    GemVal _t1586;
    if (!gem_truthy(gem_neq(gem_table_get_cached(_t1583, "type", &_ic_224), gem_string("NEWLINE")))) {
        _t1586 = gem_neq(gem_table_get_cached(_t1583, "type", &_ic_224), gem_string("NEWLINE"));
    } else {
        GemVal _t1584 = (*gem_v_peek);
        GemVal _t1585 = _t1584.fn(_t1584.env, NULL, 0);
        static GemICacheSlot _ic_225 = {0};
        _t1586 = gem_neq(gem_table_get_cached(_t1585, "type", &_ic_225), gem_string("EOF"));
    }
    GemVal _t1589;
    if (!gem_truthy(_t1586)) {
        _t1589 = _t1586;
    } else {
        GemVal _t1587 = (*gem_v_peek);
        GemVal _t1588 = _t1587.fn(_t1587.env, NULL, 0);
        static GemICacheSlot _ic_226 = {0};
        _t1589 = gem_neq(gem_table_get_cached(_t1588, "type", &_ic_226), gem_string("end"));
    }
    GemVal _t1592;
    if (!gem_truthy(_t1589)) {
        _t1592 = _t1589;
    } else {
        GemVal _t1590 = (*gem_v_peek);
        GemVal _t1591 = _t1590.fn(_t1590.env, NULL, 0);
        static GemICacheSlot _ic_227 = {0};
        _t1592 = gem_neq(gem_table_get_cached(_t1591, "type", &_ic_227), gem_string("elif"));
    }
    GemVal _t1595;
    if (!gem_truthy(_t1592)) {
        _t1595 = _t1592;
    } else {
        GemVal _t1593 = (*gem_v_peek);
        GemVal _t1594 = _t1593.fn(_t1593.env, NULL, 0);
        static GemICacheSlot _ic_228 = {0};
        _t1595 = gem_neq(gem_table_get_cached(_t1594, "type", &_ic_228), gem_string("else"));
    }
    GemVal _t1598;
    if (!gem_truthy(_t1595)) {
        _t1598 = _t1595;
    } else {
        GemVal _t1596 = (*gem_v_peek);
        GemVal _t1597 = _t1596.fn(_t1596.env, NULL, 0);
        static GemICacheSlot _ic_229 = {0};
        _t1598 = gem_neq(gem_table_get_cached(_t1597, "type", &_ic_229), gem_string("when"));
    }
        if (gem_truthy(_t1598)) {
#line 934 "compiler/main.gem"
    GemVal _t1599 = (*gem_v_parse_expr);
            gem_v_value = _t1599.fn(_t1599.env, NULL, 0);
        }
#line 936 "compiler/main.gem"
    GemVal _t1600[] = {gem_v_value, gem_v_line};
    GemVal _t1601 = (*gem_v__mod_parser_make_return);
        GemVal _t1602 = _t1601.fn(_t1601.env, _t1600, 2);
        gem_pop_frame();
        return _t1602;
    }
#line 940 "compiler/main.gem"
    GemVal _t1603 = gem_v_t;
    static GemICacheSlot _ic_230 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1603, "type", &_ic_230), gem_string("break")))) {
#line 941 "compiler/main.gem"
    GemVal _t1604 = (*gem_v_advance);
        (void)(_t1604.fn(_t1604.env, NULL, 0));
#line 942 "compiler/main.gem"
    GemVal _t1605 = (*gem_v__mod_parser_make_break);
        GemVal _t1606 = _t1605.fn(_t1605.env, NULL, 0);
        gem_pop_frame();
        return _t1606;
    }
#line 946 "compiler/main.gem"
    GemVal _t1607 = gem_v_t;
    static GemICacheSlot _ic_231 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1607, "type", &_ic_231), gem_string("continue")))) {
#line 947 "compiler/main.gem"
    GemVal _t1608 = (*gem_v_advance);
        (void)(_t1608.fn(_t1608.env, NULL, 0));
#line 948 "compiler/main.gem"
    GemVal _t1609 = (*gem_v__mod_parser_make_continue);
        GemVal _t1610 = _t1609.fn(_t1609.env, NULL, 0);
        gem_pop_frame();
        return _t1610;
    }
#line 952 "compiler/main.gem"
    GemVal _t1611 = gem_v_t;
    static GemICacheSlot _ic_232 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1611, "type", &_ic_232), gem_string("load")))) {
#line 953 "compiler/main.gem"
    GemVal _t1612 = (*gem_v_advance);
        (void)(_t1612.fn(_t1612.env, NULL, 0));
#line 954 "compiler/main.gem"
    GemVal _t1613[] = {gem_string("STRING")};
    GemVal _t1614 = (*gem_v_expect);
    GemVal _t1615 = _t1614.fn(_t1614.env, _t1613, 1);
    static GemICacheSlot _ic_233 = {0};
        GemVal gem_v_path = gem_table_get_cached(_t1615, "value", &_ic_233);
#line 955 "compiler/main.gem"
        GemVal gem_v_alias_name = GEM_NIL;
#line 956 "compiler/main.gem"
        GemVal gem_v_selective = GEM_NIL;
#line 957 "compiler/main.gem"
    GemVal _t1616 = (*gem_v_peek);
    GemVal _t1617 = _t1616.fn(_t1616.env, NULL, 0);
    static GemICacheSlot _ic_234 = {0};
    GemVal _t1620;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1617, "type", &_ic_234), gem_string("NAME")))) {
        _t1620 = gem_eq(gem_table_get_cached(_t1617, "type", &_ic_234), gem_string("NAME"));
    } else {
        GemVal _t1618 = (*gem_v_peek);
        GemVal _t1619 = _t1618.fn(_t1618.env, NULL, 0);
        static GemICacheSlot _ic_235 = {0};
        _t1620 = gem_eq(gem_table_get_cached(_t1619, "value", &_ic_235), gem_string("as"));
    }
        if (gem_truthy(_t1620)) {
#line 958 "compiler/main.gem"
    GemVal _t1621 = (*gem_v_advance);
            (void)(_t1621.fn(_t1621.env, NULL, 0));
#line 959 "compiler/main.gem"
    GemVal _t1622[] = {gem_string("NAME")};
    GemVal _t1623 = (*gem_v_expect);
    GemVal _t1624 = _t1623.fn(_t1623.env, _t1622, 1);
    static GemICacheSlot _ic_236 = {0};
            gem_v_alias_name = gem_table_get_cached(_t1624, "value", &_ic_236);
        } else {
#line 960 "compiler/main.gem"
    GemVal _t1625 = (*gem_v_peek);
    GemVal _t1626 = _t1625.fn(_t1625.env, NULL, 0);
    static GemICacheSlot _ic_237 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1626, "type", &_ic_237), gem_string("(")))) {
#line 961 "compiler/main.gem"
    GemVal _t1627 = (*gem_v_advance);
                (void)(_t1627.fn(_t1627.env, NULL, 0));
#line 962 "compiler/main.gem"
    GemVal _t1628 = gem_table_new();
    GemVal _t1629[] = {gem_string("NAME")};
    GemVal _t1630 = (*gem_v_expect);
    GemVal _t1631 = _t1630.fn(_t1630.env, _t1629, 1);
    static GemICacheSlot _ic_238 = {0};
    gem_table_set(_t1628, gem_int(0), gem_table_get_cached(_t1631, "value", &_ic_238));
                gem_v_selective = _t1628;
#line 963 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1632 = (*gem_v_peek);
                    GemVal _t1633 = _t1632.fn(_t1632.env, NULL, 0);
                    static GemICacheSlot _ic_239 = {0};
                    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1633, "type", &_ic_239), gem_string(",")))) break;
#line 964 "compiler/main.gem"
    GemVal _t1634 = (*gem_v_advance);
                    (void)(_t1634.fn(_t1634.env, NULL, 0));
#line 965 "compiler/main.gem"
    GemVal _t1635[] = {gem_string("NAME")};
    GemVal _t1636 = (*gem_v_expect);
    GemVal _t1637 = _t1636.fn(_t1636.env, _t1635, 1);
    static GemICacheSlot _ic_240 = {0};
    GemVal _t1638[] = {gem_v_selective, gem_table_get_cached(_t1637, "value", &_ic_240)};
                    (void)(gem_push_fn(NULL, _t1638, 2));
                }
#line 967 "compiler/main.gem"
    GemVal _t1639[] = {gem_string(")")};
    GemVal _t1640 = (*gem_v_expect);
                (void)(_t1640.fn(_t1640.env, _t1639, 1));
            }
        }
#line 969 "compiler/main.gem"
    GemVal _t1641 = gem_table_new();
    gem_table_set(_t1641, gem_string("tag"), gem_string("load"));
    gem_table_set(_t1641, gem_string("path"), gem_v_path);
    gem_table_set(_t1641, gem_string("alias"), gem_v_alias_name);
    gem_table_set(_t1641, gem_string("selective"), gem_v_selective);
        GemVal _t1642 = _t1641;
        gem_pop_frame();
        return _t1642;
    }
#line 973 "compiler/main.gem"
    GemVal _t1643 = gem_v_t;
    static GemICacheSlot _ic_241 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1643, "type", &_ic_241), gem_string("export")))) {
#line 974 "compiler/main.gem"
    GemVal _t1644 = (*gem_v_advance);
        (void)(_t1644.fn(_t1644.env, NULL, 0));
#line 975 "compiler/main.gem"
    GemVal _t1645 = gem_table_new();
    GemVal _t1646[] = {gem_string("NAME")};
    GemVal _t1647 = (*gem_v_expect);
    GemVal _t1648 = _t1647.fn(_t1647.env, _t1646, 1);
    static GemICacheSlot _ic_242 = {0};
    gem_table_set(_t1645, gem_int(0), gem_table_get_cached(_t1648, "value", &_ic_242));
        GemVal gem_v_names = _t1645;
#line 976 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1649 = (*gem_v_peek);
            GemVal _t1650 = _t1649.fn(_t1649.env, NULL, 0);
            static GemICacheSlot _ic_243 = {0};
            if (!gem_truthy(gem_eq(gem_table_get_cached(_t1650, "type", &_ic_243), gem_string(",")))) break;
#line 977 "compiler/main.gem"
    GemVal _t1651 = (*gem_v_advance);
            (void)(_t1651.fn(_t1651.env, NULL, 0));
#line 978 "compiler/main.gem"
    GemVal _t1652 = (*gem_v_skip_nl);
            (void)(_t1652.fn(_t1652.env, NULL, 0));
#line 979 "compiler/main.gem"
    GemVal _t1653[] = {gem_string("NAME")};
    GemVal _t1654 = (*gem_v_expect);
    GemVal _t1655 = _t1654.fn(_t1654.env, _t1653, 1);
    static GemICacheSlot _ic_244 = {0};
    GemVal _t1656[] = {gem_v_names, gem_table_get_cached(_t1655, "value", &_ic_244)};
            (void)(gem_push_fn(NULL, _t1656, 2));
        }
#line 981 "compiler/main.gem"
    GemVal _t1657[] = {gem_v_names};
    GemVal _t1658 = (*gem_v__mod_parser_make_export);
        GemVal _t1659 = _t1658.fn(_t1658.env, _t1657, 1);
        gem_pop_frame();
        return _t1659;
    }
#line 985 "compiler/main.gem"
    GemVal _t1660 = gem_v_t;
    static GemICacheSlot _ic_245 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1660, "type", &_ic_245), gem_string("extern")))) {
#line 986 "compiler/main.gem"
    GemVal _t1661 = (*gem_v_advance);
        (void)(_t1661.fn(_t1661.env, NULL, 0));
#line 987 "compiler/main.gem"
        GemVal gem_v_is_blocking = gem_bool(0);
#line 988 "compiler/main.gem"
    GemVal _t1662 = (*gem_v_peek);
    GemVal _t1663 = _t1662.fn(_t1662.env, NULL, 0);
    static GemICacheSlot _ic_246 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1663, "type", &_ic_246), gem_string("blocking")))) {
#line 989 "compiler/main.gem"
            gem_v_is_blocking = gem_bool(1);
#line 990 "compiler/main.gem"
    GemVal _t1664 = (*gem_v_advance);
            (void)(_t1664.fn(_t1664.env, NULL, 0));
        }
#line 992 "compiler/main.gem"
    GemVal _t1665 = (*gem_v_peek);
    GemVal _t1666 = _t1665.fn(_t1665.env, NULL, 0);
    static GemICacheSlot _ic_247 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1666, "type", &_ic_247), gem_string("fn")))) {
#line 993 "compiler/main.gem"
    GemVal _t1667 = (*gem_v_advance);
            (void)(_t1667.fn(_t1667.env, NULL, 0));
#line 994 "compiler/main.gem"
    GemVal _t1668[] = {gem_string("NAME")};
    GemVal _t1669 = (*gem_v_expect);
    GemVal _t1670 = _t1669.fn(_t1669.env, _t1668, 1);
    static GemICacheSlot _ic_248 = {0};
            GemVal gem_v_name = gem_table_get_cached(_t1670, "value", &_ic_248);
#line 995 "compiler/main.gem"
    GemVal _t1671[] = {gem_string("(")};
    GemVal _t1672 = (*gem_v_expect);
            (void)(_t1672.fn(_t1672.env, _t1671, 1));
#line 996 "compiler/main.gem"
    GemVal _t1673 = gem_table_new();
            GemVal gem_v_eparams = _t1673;
#line 997 "compiler/main.gem"
    GemVal _t1674 = (*gem_v_peek);
    GemVal _t1675 = _t1674.fn(_t1674.env, NULL, 0);
    static GemICacheSlot _ic_249 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t1675, "type", &_ic_249), gem_string(")")))) {
#line 998 "compiler/main.gem"
    GemVal _t1676[] = {gem_string("NAME")};
    GemVal _t1677 = (*gem_v_expect);
    GemVal _t1678 = _t1677.fn(_t1677.env, _t1676, 1);
    static GemICacheSlot _ic_250 = {0};
                GemVal gem_v_pname = gem_table_get_cached(_t1678, "value", &_ic_250);
#line 999 "compiler/main.gem"
    GemVal _t1679[] = {gem_string(":")};
    GemVal _t1680 = (*gem_v_expect);
                (void)(_t1680.fn(_t1680.env, _t1679, 1));
#line 1000 "compiler/main.gem"
    GemVal _t1681[] = {gem_string("NAME")};
    GemVal _t1682 = (*gem_v_expect);
    GemVal _t1683 = _t1682.fn(_t1682.env, _t1681, 1);
    static GemICacheSlot _ic_251 = {0};
                GemVal gem_v_ptype = gem_table_get_cached(_t1683, "value", &_ic_251);
#line 1001 "compiler/main.gem"
    GemVal _t1684[] = {gem_v_pname, gem_v_ptype};
    GemVal _t1685 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1686[] = {gem_v_eparams, _t1685.fn(_t1685.env, _t1684, 2)};
                (void)(gem_push_fn(NULL, _t1686, 2));
#line 1002 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t1687 = (*gem_v_peek);
                    GemVal _t1688 = _t1687.fn(_t1687.env, NULL, 0);
                    static GemICacheSlot _ic_252 = {0};
                    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1688, "type", &_ic_252), gem_string(",")))) break;
#line 1003 "compiler/main.gem"
    GemVal _t1689 = (*gem_v_advance);
                    (void)(_t1689.fn(_t1689.env, NULL, 0));
#line 1004 "compiler/main.gem"
    GemVal _t1690[] = {gem_string("NAME")};
    GemVal _t1691 = (*gem_v_expect);
    GemVal _t1692 = _t1691.fn(_t1691.env, _t1690, 1);
    static GemICacheSlot _ic_253 = {0};
                    GemVal gem_v_pn2 = gem_table_get_cached(_t1692, "value", &_ic_253);
#line 1005 "compiler/main.gem"
    GemVal _t1693[] = {gem_string(":")};
    GemVal _t1694 = (*gem_v_expect);
                    (void)(_t1694.fn(_t1694.env, _t1693, 1));
#line 1006 "compiler/main.gem"
    GemVal _t1695[] = {gem_string("NAME")};
    GemVal _t1696 = (*gem_v_expect);
    GemVal _t1697 = _t1696.fn(_t1696.env, _t1695, 1);
    static GemICacheSlot _ic_254 = {0};
                    GemVal gem_v_pt2 = gem_table_get_cached(_t1697, "value", &_ic_254);
#line 1007 "compiler/main.gem"
    GemVal _t1698[] = {gem_v_pn2, gem_v_pt2};
    GemVal _t1699 = (*gem_v__mod_parser_make_extern_param);
    GemVal _t1700[] = {gem_v_eparams, _t1699.fn(_t1699.env, _t1698, 2)};
                    (void)(gem_push_fn(NULL, _t1700, 2));
                }
            }
#line 1010 "compiler/main.gem"
    GemVal _t1701[] = {gem_string(")")};
    GemVal _t1702 = (*gem_v_expect);
            (void)(_t1702.fn(_t1702.env, _t1701, 1));
#line 1011 "compiler/main.gem"
            GemVal gem_v_ret_type = gem_string("Nil");
#line 1012 "compiler/main.gem"
    GemVal _t1703 = (*gem_v_peek);
    GemVal _t1704 = _t1703.fn(_t1703.env, NULL, 0);
    static GemICacheSlot _ic_255 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1704, "type", &_ic_255), gem_string("->")))) {
#line 1013 "compiler/main.gem"
    GemVal _t1705 = (*gem_v_advance);
                (void)(_t1705.fn(_t1705.env, NULL, 0));
#line 1014 "compiler/main.gem"
    GemVal _t1706[] = {gem_string("NAME")};
    GemVal _t1707 = (*gem_v_expect);
    GemVal _t1708 = _t1707.fn(_t1707.env, _t1706, 1);
    static GemICacheSlot _ic_256 = {0};
                gem_v_ret_type = gem_table_get_cached(_t1708, "value", &_ic_256);
            }
#line 1016 "compiler/main.gem"
    GemVal _t1709[] = {gem_v_name, gem_v_eparams, gem_v_ret_type, gem_v_is_blocking};
    GemVal _t1710 = (*gem_v__mod_parser_make_extern_fn);
            GemVal _t1711 = _t1710.fn(_t1710.env, _t1709, 4);
            gem_pop_frame();
            return _t1711;
        } else {
#line 1017 "compiler/main.gem"
    GemVal _t1714;
    if (!gem_truthy(gem_not(gem_v_is_blocking))) {
        _t1714 = gem_not(gem_v_is_blocking);
    } else {
        GemVal _t1712 = (*gem_v_peek);
        GemVal _t1713 = _t1712.fn(_t1712.env, NULL, 0);
        static GemICacheSlot _ic_257 = {0};
        _t1714 = gem_eq(gem_table_get_cached(_t1713, "type", &_ic_257), gem_string("NAME"));
    }
    GemVal _t1717;
    if (!gem_truthy(_t1714)) {
        _t1717 = _t1714;
    } else {
        GemVal _t1715 = (*gem_v_peek);
        GemVal _t1716 = _t1715.fn(_t1715.env, NULL, 0);
        static GemICacheSlot _ic_258 = {0};
        _t1717 = gem_eq(gem_table_get_cached(_t1716, "value", &_ic_258), gem_string("include"));
    }
            if (gem_truthy(_t1717)) {
#line 1018 "compiler/main.gem"
    GemVal _t1718 = (*gem_v_advance);
                (void)(_t1718.fn(_t1718.env, NULL, 0));
#line 1019 "compiler/main.gem"
    GemVal _t1719[] = {gem_string("STRING")};
    GemVal _t1720 = (*gem_v_expect);
    GemVal _t1721 = _t1720.fn(_t1720.env, _t1719, 1);
    static GemICacheSlot _ic_259 = {0};
                GemVal gem_v_path = gem_table_get_cached(_t1721, "value", &_ic_259);
#line 1020 "compiler/main.gem"
    GemVal _t1722[] = {gem_v_path};
    GemVal _t1723 = (*gem_v__mod_parser_make_extern_include);
                GemVal _t1724 = _t1723.fn(_t1723.env, _t1722, 1);
                gem_pop_frame();
                return _t1724;
            } else {
#line 1022 "compiler/main.gem"
    GemVal _t1725 = (*gem_v_peek);
    GemVal _t1726 = _t1725.fn(_t1725.env, NULL, 0);
    static GemICacheSlot _ic_260 = {0};
    GemVal _t1727 = (*gem_v_peek);
    GemVal _t1728 = _t1727.fn(_t1727.env, NULL, 0);
    static GemICacheSlot _ic_261 = {0};
    GemVal _t1729 = (*gem_v_peek);
    GemVal _t1730 = _t1729.fn(_t1729.env, NULL, 0);
    static GemICacheSlot _ic_262 = {0};
    GemVal _t1731[] = {gem_table_get_cached(_t1730, "value", &_ic_262)};
    GemVal _t1732[] = {(*gem_v_source), (*gem_v_file), gem_table_get_cached(_t1726, "line", &_ic_260), gem_table_get_cached(_t1728, "col", &_ic_261), gem_len_fn(NULL, _t1731, 1), gem_string("expected 'fn' after 'extern blocking'"), GEM_NIL};
    GemVal _t1733 = (*gem_v__mod_parser_compile_error);
                (void)(_t1733.fn(_t1733.env, _t1732, 7));
            }
        }
    }
#line 1027 "compiler/main.gem"
    GemVal _t1734 = gem_v_t;
    static GemICacheSlot _ic_263 = {0};
    GemVal _t1736;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t1734, "type", &_ic_263), gem_string("NAME")))) {
        _t1736 = gem_eq(gem_table_get_cached(_t1734, "type", &_ic_263), gem_string("NAME"));
    } else {
        GemVal _t1735 = gem_v_t;
        static GemICacheSlot _ic_264 = {0};
        _t1736 = gem_eq(gem_table_get_cached(_t1735, "value", &_ic_264), gem_string("receive"));
    }
    if (gem_truthy(_t1736)) {
#line 1028 "compiler/main.gem"
        GemVal gem_v_rla = gem_int(1);
#line 1029 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t1737[] = {gem_v_rla};
            GemVal _t1738 = (*gem_v_peek_at);
            GemVal _t1739 = _t1738.fn(_t1738.env, _t1737, 1);
            static GemICacheSlot _ic_265 = {0};
            if (!gem_truthy(gem_eq(gem_table_get_cached(_t1739, "type", &_ic_265), gem_string("NEWLINE")))) break;
#line 1030 "compiler/main.gem"
            gem_v_rla = gem_add(gem_v_rla, gem_int(1));
        }
#line 1032 "compiler/main.gem"
    GemVal _t1740[] = {gem_v_rla};
    GemVal _t1741 = (*gem_v_peek_at);
    GemVal _t1742 = _t1741.fn(_t1741.env, _t1740, 1);
    static GemICacheSlot _ic_266 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t1742, "type", &_ic_266), gem_string("when")))) {
#line 1033 "compiler/main.gem"
    GemVal _t1743 = gem_v_t;
    static GemICacheSlot _ic_267 = {0};
            GemVal gem_v_rline = gem_table_get_cached(_t1743, "line", &_ic_267);
#line 1034 "compiler/main.gem"
    GemVal _t1744 = (*gem_v_advance);
            (void)(_t1744.fn(_t1744.env, NULL, 0));
#line 1035 "compiler/main.gem"
    GemVal _t1745 = (*gem_v_skip_nl);
            (void)(_t1745.fn(_t1745.env, NULL, 0));
#line 1036 "compiler/main.gem"
            *gem_v_gensym_counter = gem_add((*gem_v_gensym_counter), gem_int(1));
#line 1037 "compiler/main.gem"
    GemVal _t1746[] = {gem_string("_recv_"), (*gem_v_gensym_counter)};
            GemVal gem_v_recv_var = gem_interp(2, _t1746);
#line 1038 "compiler/main.gem"
    GemVal _t1747[] = {gem_v_recv_var};
    GemVal _t1748 = (*gem_v__mod_parser_make_var);
            GemVal gem_v_recv_var_expr = _t1748.fn(_t1748.env, _t1747, 1);
#line 1039 "compiler/main.gem"
    GemVal _t1749 = gem_table_new();
            GemVal gem_v_rarms = _t1749;
#line 1040 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1750 = (*gem_v_peek);
                GemVal _t1751 = _t1750.fn(_t1750.env, NULL, 0);
                static GemICacheSlot _ic_268 = {0};
                if (!gem_truthy(gem_eq(gem_table_get_cached(_t1751, "type", &_ic_268), gem_string("when")))) break;
#line 1041 "compiler/main.gem"
    GemVal _t1752 = (*gem_v_advance);
                (void)(_t1752.fn(_t1752.env, NULL, 0));
#line 1042 "compiler/main.gem"
    GemVal _t1753 = (*gem_v_peek);
    GemVal _t1754 = _t1753.fn(_t1753.env, NULL, 0);
    static GemICacheSlot _ic_269 = {0};
                GemVal gem_v_rpt = gem_table_get_cached(_t1754, "type", &_ic_269);
#line 1043 "compiler/main.gem"
    GemVal _t1755[] = {gem_int(1)};
    GemVal _t1756 = (*gem_v_peek_at);
    GemVal _t1757 = _t1756.fn(_t1756.env, _t1755, 1);
    static GemICacheSlot _ic_270 = {0};
                GemVal gem_v_rnext = gem_table_get_cached(_t1757, "type", &_ic_270);
#line 1044 "compiler/main.gem"
    GemVal _t1758;
    if (gem_truthy(gem_eq(gem_v_rpt, gem_string("{")))) {
        _t1758 = gem_eq(gem_v_rpt, gem_string("{"));
    } else {
        _t1758 = gem_eq(gem_v_rpt, gem_string("["));
    }
                if (gem_truthy(_t1758)) {
#line 1045 "compiler/main.gem"
    GemVal _t1759[] = {gem_v_recv_var_expr};
    GemVal _t1760 = (*gem_v_parse_pattern);
                    GemVal gem_v_rpat = _t1760.fn(_t1760.env, _t1759, 1);
#line 1046 "compiler/main.gem"
    GemVal _t1761 = (*gem_v_parse_body);
                    GemVal gem_v_rwbody = _t1761.fn(_t1761.env, NULL, 0);
#line 1047 "compiler/main.gem"
    GemVal _t1762 = gem_table_new();
    gem_table_set(_t1762, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1762, gem_string("body"), gem_v_rwbody);
    GemVal _t1763[] = {gem_v_rarms, _t1762};
                    (void)(gem_push_fn(NULL, _t1763, 2));
                } else {
#line 1048 "compiler/main.gem"
    GemVal _t1769;
    if (!gem_truthy(gem_eq(gem_v_rpt, gem_string("NAME")))) {
        _t1769 = gem_eq(gem_v_rpt, gem_string("NAME"));
    } else {
        GemVal _t1764;
        if (gem_truthy(gem_eq(gem_v_rnext, gem_string("NEWLINE")))) {
                _t1764 = gem_eq(gem_v_rnext, gem_string("NEWLINE"));
        } else {
                _t1764 = gem_eq(gem_v_rnext, gem_string("EOF"));
        }
        GemVal _t1765;
        if (gem_truthy(_t1764)) {
                _t1765 = _t1764;
        } else {
                _t1765 = gem_eq(gem_v_rnext, gem_string("end"));
        }
        GemVal _t1766;
        if (gem_truthy(_t1765)) {
                _t1766 = _t1765;
        } else {
                _t1766 = gem_eq(gem_v_rnext, gem_string("else"));
        }
        GemVal _t1767;
        if (gem_truthy(_t1766)) {
                _t1767 = _t1766;
        } else {
                _t1767 = gem_eq(gem_v_rnext, gem_string("when"));
        }
        GemVal _t1768;
        if (gem_truthy(_t1767)) {
                _t1768 = _t1767;
        } else {
                _t1768 = gem_eq(gem_v_rnext, gem_string("after"));
        }
        _t1769 = _t1768;
    }
                    if (gem_truthy(_t1769)) {
#line 1049 "compiler/main.gem"
    GemVal _t1770[] = {gem_v_recv_var_expr};
    GemVal _t1771 = (*gem_v_parse_pattern);
                        GemVal gem_v_rpat = _t1771.fn(_t1771.env, _t1770, 1);
#line 1050 "compiler/main.gem"
    GemVal _t1772 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1772.fn(_t1772.env, NULL, 0);
#line 1051 "compiler/main.gem"
    GemVal _t1773 = gem_table_new();
    gem_table_set(_t1773, gem_string("pattern"), gem_v_rpat);
    gem_table_set(_t1773, gem_string("body"), gem_v_rwbody);
    GemVal _t1774[] = {gem_v_rarms, _t1773};
                        (void)(gem_push_fn(NULL, _t1774, 2));
                    } else {
#line 1053 "compiler/main.gem"
    GemVal _t1775 = (*gem_v_parse_expr);
                        GemVal gem_v_rwval = _t1775.fn(_t1775.env, NULL, 0);
#line 1054 "compiler/main.gem"
    GemVal _t1776 = (*gem_v_parse_body);
                        GemVal gem_v_rwbody = _t1776.fn(_t1776.env, NULL, 0);
#line 1055 "compiler/main.gem"
    GemVal _t1777 = gem_table_new();
    GemVal _t1778 = gem_table_new();
    GemVal _t1779[] = {gem_string("=="), gem_v_recv_var_expr, gem_v_rwval};
    GemVal _t1780 = (*gem_v__mod_parser_make_binop);
    gem_table_set(_t1778, gem_string("condition"), _t1780.fn(_t1780.env, _t1779, 3));
    GemVal _t1781 = gem_table_new();
    gem_table_set(_t1778, gem_string("bindings"), _t1781);
    gem_table_set(_t1777, gem_string("pattern"), _t1778);
    gem_table_set(_t1777, gem_string("body"), gem_v_rwbody);
    GemVal _t1782[] = {gem_v_rarms, _t1777};
                        (void)(gem_push_fn(NULL, _t1782, 2));
                    }
                }
#line 1057 "compiler/main.gem"
    GemVal _t1783 = (*gem_v_skip_nl);
                (void)(_t1783.fn(_t1783.env, NULL, 0));
            }
#line 1059 "compiler/main.gem"
            GemVal gem_v_rafter_ms = GEM_NIL;
#line 1060 "compiler/main.gem"
            GemVal gem_v_rafter_body = GEM_NIL;
#line 1061 "compiler/main.gem"
    GemVal _t1784 = (*gem_v_peek);
    GemVal _t1785 = _t1784.fn(_t1784.env, NULL, 0);
    static GemICacheSlot _ic_271 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t1785, "type", &_ic_271), gem_string("after")))) {
#line 1062 "compiler/main.gem"
    GemVal _t1786 = (*gem_v_advance);
                (void)(_t1786.fn(_t1786.env, NULL, 0));
#line 1063 "compiler/main.gem"
    GemVal _t1787 = (*gem_v_parse_expr);
                gem_v_rafter_ms = _t1787.fn(_t1787.env, NULL, 0);
#line 1064 "compiler/main.gem"
    GemVal _t1788 = (*gem_v_parse_body);
                gem_v_rafter_body = _t1788.fn(_t1788.env, NULL, 0);
            }
#line 1066 "compiler/main.gem"
    GemVal _t1789[] = {gem_string("end")};
    GemVal _t1790 = (*gem_v_expect);
            (void)(_t1790.fn(_t1790.env, _t1789, 1));
#line 1067 "compiler/main.gem"
    GemVal _t1791[] = {gem_v_rarms, gem_v_rafter_ms, gem_v_rafter_body, gem_v_rline, gem_v_recv_var};
    GemVal _t1792 = (*gem_v__mod_parser_make_receive_match);
            GemVal _t1793 = _t1792.fn(_t1792.env, _t1791, 5);
            gem_pop_frame();
            return _t1793;
        }
    }
#line 1072 "compiler/main.gem"
    GemVal _t1794 = (*gem_v_parse_expr);
    GemVal _t1795 = _t1794.fn(_t1794.env, NULL, 0);
    gem_pop_frame();
    return _t1795;
}

struct _closure__anon_27 {
    intptr_t _num_captures;
    GemVal *gem_v__mod_parser_make_program;
    GemVal *gem_v_at_end;
    GemVal *gem_v_parse_stmt;
    GemVal *gem_v_skip_nl;
};
static GemVal _anon_27(void *_env, GemVal *args, int argc) {
    struct _closure__anon_27 *_cls = (struct _closure__anon_27 *)_env;
    GemVal *gem_v__mod_parser_make_program = _cls->gem_v__mod_parser_make_program;
    GemVal *gem_v_at_end = _cls->gem_v_at_end;
    GemVal *gem_v_parse_stmt = _cls->gem_v_parse_stmt;
    GemVal *gem_v_skip_nl = _cls->gem_v_skip_nl;
    gem_push_frame("_anon_27", "compiler/main.gem", 0);
#line 1081 "compiler/main.gem"
    GemVal _t1797 = gem_table_new();
    GemVal gem_v_stmts = _t1797;
#line 1082 "compiler/main.gem"
    GemVal _t1798 = (*gem_v_skip_nl);
    (void)(_t1798.fn(_t1798.env, NULL, 0));
#line 1083 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1799 = (*gem_v_at_end);
        if (!gem_truthy(gem_not(_t1799.fn(_t1799.env, NULL, 0)))) break;
#line 1084 "compiler/main.gem"
    GemVal _t1800 = (*gem_v_parse_stmt);
    GemVal _t1801[] = {gem_v_stmts, _t1800.fn(_t1800.env, NULL, 0)};
        (void)(gem_push_fn(NULL, _t1801, 2));
#line 1085 "compiler/main.gem"
    GemVal _t1802 = (*gem_v_skip_nl);
        (void)(_t1802.fn(_t1802.env, NULL, 0));
    }
#line 1087 "compiler/main.gem"
    GemVal _t1803[] = {gem_v_stmts};
    GemVal _t1804 = (*gem_v__mod_parser_make_program);
    GemVal _t1805 = _t1804.fn(_t1804.env, _t1803, 1);
    gem_pop_frame();
    return _t1805;
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
    struct _closure__anon_5 *_t448 = GC_MALLOC(sizeof(struct _closure__anon_5));
    _t448->_num_captures = 2;
    _t448->gem_v_pos = gem_v_pos;
    _t448->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek = gem_make_fn(_anon_5, _t448);
#line 77 "compiler/main.gem"
    struct _closure__anon_6 *_t450 = GC_MALLOC(sizeof(struct _closure__anon_6));
    _t450->_num_captures = 2;
    _t450->gem_v_pos = gem_v_pos;
    _t450->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_peek_at = GC_MALLOC(sizeof(GemVal));
    *gem_v_peek_at = gem_make_fn(_anon_6, _t450);
#line 81 "compiler/main.gem"
    struct _closure__anon_7 *_t452 = GC_MALLOC(sizeof(struct _closure__anon_7));
    _t452->_num_captures = 2;
    _t452->gem_v_pos = gem_v_pos;
    _t452->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_advance = GC_MALLOC(sizeof(GemVal));
    *gem_v_advance = gem_make_fn(_anon_7, _t452);
#line 87 "compiler/main.gem"
    struct _closure__anon_8 *_t455 = GC_MALLOC(sizeof(struct _closure__anon_8));
    _t455->_num_captures = 2;
    _t455->gem_v_pos = gem_v_pos;
    _t455->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_at_end = GC_MALLOC(sizeof(GemVal));
    *gem_v_at_end = gem_make_fn(_anon_8, _t455);
#line 91 "compiler/main.gem"
    struct _closure__anon_9 *_t466 = GC_MALLOC(sizeof(struct _closure__anon_9));
    _t466->_num_captures = 5;
    _t466->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t466->gem_v_file = gem_v_file;
    _t466->gem_v_pos = gem_v_pos;
    _t466->gem_v_source = gem_v_source;
    _t466->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_expect = GC_MALLOC(sizeof(GemVal));
    *gem_v_expect = gem_make_fn(_anon_9, _t466);
#line 100 "compiler/main.gem"
    struct _closure__anon_10 *_t468 = GC_MALLOC(sizeof(struct _closure__anon_10));
    _t468->_num_captures = 2;
    _t468->gem_v_pos = gem_v_pos;
    _t468->gem_v_tokens = gem_v_tokens;
    GemVal *gem_v_skip_nl = GC_MALLOC(sizeof(GemVal));
    *gem_v_skip_nl = gem_make_fn(_anon_10, _t468);
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
    struct _closure__anon_11 *_t502 = GC_MALLOC(sizeof(struct _closure__anon_11));
    _t502->_num_captures = 7;
    _t502->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t502->gem_v_advance = gem_v_advance;
    _t502->gem_v_expect = gem_v_expect;
    _t502->gem_v_file = gem_v_file;
    _t502->gem_v_parse_expr = gem_v_parse_expr;
    _t502->gem_v_peek = gem_v_peek;
    _t502->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_params = gem_make_fn(_anon_11, _t502);
#line 157 "compiler/main.gem"
    struct _closure__anon_12 *_t528 = GC_MALLOC(sizeof(struct _closure__anon_12));
    _t528->_num_captures = 4;
    _t528->gem_v_at_end = gem_v_at_end;
    _t528->gem_v_parse_stmt = gem_v_parse_stmt;
    _t528->gem_v_peek = gem_v_peek;
    _t528->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_body = gem_make_fn(_anon_12, _t528);
#line 170 "compiler/main.gem"
    struct _closure__anon_13 *_t539 = GC_MALLOC(sizeof(struct _closure__anon_13));
    _t539->_num_captures = 4;
    _t539->gem_v_at_end = gem_v_at_end;
    _t539->gem_v_parse_stmt = gem_v_parse_stmt;
    _t539->gem_v_peek = gem_v_peek;
    _t539->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_fn_body = gem_make_fn(_anon_13, _t539);
#line 183 "compiler/main.gem"
    struct _closure__anon_14 *_t713 = GC_MALLOC(sizeof(struct _closure__anon_14));
    _t713->_num_captures = 25;
    _t713->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t713->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t713->gem_v__mod_parser_make_array = &gem_v__mod_parser_make_array;
    _t713->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t713->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t713->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t713->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t713->gem_v__mod_parser_make_interp = &gem_v__mod_parser_make_interp;
    _t713->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t713->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t713->gem_v__mod_parser_make_table = &gem_v__mod_parser_make_table;
    _t713->gem_v__mod_parser_make_table_entry = &gem_v__mod_parser_make_table_entry;
    _t713->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t713->gem_v_advance = gem_v_advance;
    _t713->gem_v_at_end = gem_v_at_end;
    _t713->gem_v_expect = gem_v_expect;
    _t713->gem_v_file = gem_v_file;
    _t713->gem_v_fn_depth = gem_v_fn_depth;
    _t713->gem_v_parse_expr = gem_v_parse_expr;
    _t713->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t713->gem_v_parse_params = gem_v_parse_params;
    _t713->gem_v_peek = gem_v_peek;
    _t713->gem_v_peek_at = gem_v_peek_at;
    _t713->gem_v_skip_nl = gem_v_skip_nl;
    _t713->gem_v_source = gem_v_source;
    GemVal *gem_v_parse_atom = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_atom = gem_make_fn(_anon_14, _t713);
#line 321 "compiler/main.gem"
    struct _closure__anon_15 *_t811 = GC_MALLOC(sizeof(struct _closure__anon_15));
    _t811->_num_captures = 14;
    _t811->gem_v__mod_parser_make_anon_fn = &gem_v__mod_parser_make_anon_fn;
    _t811->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t811->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t811->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t811->gem_v_advance = gem_v_advance;
    _t811->gem_v_expect = gem_v_expect;
    _t811->gem_v_fn_depth = gem_v_fn_depth;
    _t811->gem_v_parse_atom = gem_v_parse_atom;
    _t811->gem_v_parse_expr = gem_v_parse_expr;
    _t811->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t811->gem_v_parse_params = gem_v_parse_params;
    _t811->gem_v_peek = gem_v_peek;
    _t811->gem_v_peek_at = gem_v_peek_at;
    _t811->gem_v_skip_nl = gem_v_skip_nl;
    GemVal *gem_v_parse_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_call = gem_make_fn(_anon_15, _t811);
#line 427 "compiler/main.gem"
    struct _closure__anon_16 *_t821 = GC_MALLOC(sizeof(struct _closure__anon_16));
    _t821->_num_captures = 5;
    _t821->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t821->gem_v_advance = gem_v_advance;
    _t821->gem_v_parse_call = gem_v_parse_call;
    _t821->gem_v_parse_unary = gem_v_parse_unary;
    _t821->gem_v_peek = gem_v_peek;
    *gem_v_parse_unary = gem_make_fn(_anon_16, _t821);
#line 436 "compiler/main.gem"
    struct _closure__anon_17 *_t837 = GC_MALLOC(sizeof(struct _closure__anon_17));
    _t837->_num_captures = 4;
    _t837->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t837->gem_v_advance = gem_v_advance;
    _t837->gem_v_parse_unary = gem_v_parse_unary;
    _t837->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_mul = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_mul = gem_make_fn(_anon_17, _t837);
#line 447 "compiler/main.gem"
    struct _closure__anon_18 *_t850 = GC_MALLOC(sizeof(struct _closure__anon_18));
    _t850->_num_captures = 4;
    _t850->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t850->gem_v_advance = gem_v_advance;
    _t850->gem_v_parse_mul = gem_v_parse_mul;
    _t850->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_add = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_add = gem_make_fn(_anon_18, _t850);
#line 458 "compiler/main.gem"
    struct _closure__anon_19 *_t878 = GC_MALLOC(sizeof(struct _closure__anon_19));
    _t878->_num_captures = 4;
    _t878->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t878->gem_v_advance = gem_v_advance;
    _t878->gem_v_parse_add = gem_v_parse_add;
    _t878->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_compare = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_compare = gem_make_fn(_anon_19, _t878);
#line 469 "compiler/main.gem"
    struct _closure__anon_20 *_t888 = GC_MALLOC(sizeof(struct _closure__anon_20));
    _t888->_num_captures = 5;
    _t888->gem_v__mod_parser_make_unop = &gem_v__mod_parser_make_unop;
    _t888->gem_v_advance = gem_v_advance;
    _t888->gem_v_parse_compare = gem_v_parse_compare;
    _t888->gem_v_parse_not = gem_v_parse_not;
    _t888->gem_v_peek = gem_v_peek;
    *gem_v_parse_not = gem_make_fn(_anon_20, _t888);
#line 478 "compiler/main.gem"
    struct _closure__anon_21 *_t897 = GC_MALLOC(sizeof(struct _closure__anon_21));
    _t897->_num_captures = 4;
    _t897->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t897->gem_v_advance = gem_v_advance;
    _t897->gem_v_parse_not = gem_v_parse_not;
    _t897->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_and = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_and = gem_make_fn(_anon_21, _t897);
#line 489 "compiler/main.gem"
    struct _closure__anon_22 *_t906 = GC_MALLOC(sizeof(struct _closure__anon_22));
    _t906->_num_captures = 4;
    _t906->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t906->gem_v_advance = gem_v_advance;
    _t906->gem_v_parse_and = gem_v_parse_and;
    _t906->gem_v_peek = gem_v_peek;
    GemVal *gem_v_parse_or = GC_MALLOC(sizeof(GemVal));
    *gem_v_parse_or = gem_make_fn(_anon_22, _t906);
#line 500 "compiler/main.gem"
    struct _closure__anon_23 *_t964 = GC_MALLOC(sizeof(struct _closure__anon_23));
    _t964->_num_captures = 12;
    _t964->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t964->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t964->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t964->gem_v__mod_parser_make_dot_assign = &gem_v__mod_parser_make_dot_assign;
    _t964->gem_v__mod_parser_make_index_assign = &gem_v__mod_parser_make_index_assign;
    _t964->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t964->gem_v_advance = gem_v_advance;
    _t964->gem_v_file = gem_v_file;
    _t964->gem_v_parse_expr = gem_v_parse_expr;
    _t964->gem_v_parse_or = gem_v_parse_or;
    _t964->gem_v_peek = gem_v_peek;
    _t964->gem_v_source = gem_v_source;
    GemVal gem_v_parse_assign_fn = gem_make_fn(_anon_23, _t964);
#line 535 "compiler/main.gem"
    *gem_v_parse_expr = gem_v_parse_assign_fn;
#line 539 "compiler/main.gem"
    struct _closure__anon_24 *_t1173 = GC_MALLOC(sizeof(struct _closure__anon_24));
    _t1173->_num_captures = 19;
    _t1173->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1173->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1173->gem_v__mod_parser_make_bool = &gem_v__mod_parser_make_bool;
    _t1173->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1173->gem_v__mod_parser_make_float = &gem_v__mod_parser_make_float;
    _t1173->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1173->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1173->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1173->gem_v__mod_parser_make_nil_node = &gem_v__mod_parser_make_nil_node;
    _t1173->gem_v__mod_parser_make_string = &gem_v__mod_parser_make_string;
    _t1173->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1173->gem_v_advance = gem_v_advance;
    _t1173->gem_v_expect = gem_v_expect;
    _t1173->gem_v_file = gem_v_file;
    _t1173->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1173->gem_v_peek = gem_v_peek;
    _t1173->gem_v_peek_at = gem_v_peek_at;
    _t1173->gem_v_skip_nl = gem_v_skip_nl;
    _t1173->gem_v_source = gem_v_source;
    *gem_v_parse_pattern = gem_make_fn(_anon_24, _t1173);
#line 667 "compiler/main.gem"
    struct _closure__anon_26 *_t1796 = GC_MALLOC(sizeof(struct _closure__anon_26));
    _t1796->_num_captures = 37;
    _t1796->gem_v__mod_parser_compile_error = &gem_v__mod_parser_compile_error;
    _t1796->gem_v__mod_parser_make_assign = &gem_v__mod_parser_make_assign;
    _t1796->gem_v__mod_parser_make_binop = &gem_v__mod_parser_make_binop;
    _t1796->gem_v__mod_parser_make_break = &gem_v__mod_parser_make_break;
    _t1796->gem_v__mod_parser_make_call = &gem_v__mod_parser_make_call;
    _t1796->gem_v__mod_parser_make_continue = &gem_v__mod_parser_make_continue;
    _t1796->gem_v__mod_parser_make_dot = &gem_v__mod_parser_make_dot;
    _t1796->gem_v__mod_parser_make_export = &gem_v__mod_parser_make_export;
    _t1796->gem_v__mod_parser_make_extern_fn = &gem_v__mod_parser_make_extern_fn;
    _t1796->gem_v__mod_parser_make_extern_include = &gem_v__mod_parser_make_extern_include;
    _t1796->gem_v__mod_parser_make_extern_param = &gem_v__mod_parser_make_extern_param;
    _t1796->gem_v__mod_parser_make_fn_def = &gem_v__mod_parser_make_fn_def;
    _t1796->gem_v__mod_parser_make_if = &gem_v__mod_parser_make_if;
    _t1796->gem_v__mod_parser_make_index = &gem_v__mod_parser_make_index;
    _t1796->gem_v__mod_parser_make_int = &gem_v__mod_parser_make_int;
    _t1796->gem_v__mod_parser_make_let = &gem_v__mod_parser_make_let;
    _t1796->gem_v__mod_parser_make_match = &gem_v__mod_parser_make_match;
    _t1796->gem_v__mod_parser_make_receive_match = &gem_v__mod_parser_make_receive_match;
    _t1796->gem_v__mod_parser_make_return = &gem_v__mod_parser_make_return;
    _t1796->gem_v__mod_parser_make_var = &gem_v__mod_parser_make_var;
    _t1796->gem_v__mod_parser_make_when = &gem_v__mod_parser_make_when;
    _t1796->gem_v__mod_parser_make_while = &gem_v__mod_parser_make_while;
    _t1796->gem_v_advance = gem_v_advance;
    _t1796->gem_v_expect = gem_v_expect;
    _t1796->gem_v_file = gem_v_file;
    _t1796->gem_v_fn_depth = gem_v_fn_depth;
    _t1796->gem_v_gensym_counter = gem_v_gensym_counter;
    _t1796->gem_v_parse_body = gem_v_parse_body;
    _t1796->gem_v_parse_expr = gem_v_parse_expr;
    _t1796->gem_v_parse_fn_body = gem_v_parse_fn_body;
    _t1796->gem_v_parse_params = gem_v_parse_params;
    _t1796->gem_v_parse_pattern = gem_v_parse_pattern;
    _t1796->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1796->gem_v_peek = gem_v_peek;
    _t1796->gem_v_peek_at = gem_v_peek_at;
    _t1796->gem_v_skip_nl = gem_v_skip_nl;
    _t1796->gem_v_source = gem_v_source;
    GemVal gem_v_parse_stmt_fn = gem_make_fn(_anon_26, _t1796);
#line 1076 "compiler/main.gem"
    *gem_v_parse_stmt = gem_v_parse_stmt_fn;
#line 1080 "compiler/main.gem"
    struct _closure__anon_27 *_t1806 = GC_MALLOC(sizeof(struct _closure__anon_27));
    _t1806->_num_captures = 4;
    _t1806->gem_v__mod_parser_make_program = &gem_v__mod_parser_make_program;
    _t1806->gem_v_at_end = gem_v_at_end;
    _t1806->gem_v_parse_stmt = gem_v_parse_stmt;
    _t1806->gem_v_skip_nl = gem_v_skip_nl;
    GemVal gem_v_parse = gem_make_fn(_anon_27, _t1806);
    GemVal _t1807 = gem_table_new();
    gem_table_set(_t1807, gem_string("parse"), gem_v_parse);
    GemVal _t1808 = _t1807;
    gem_pop_frame();
    return _t1808;
}

static GemVal gem_fn__mod_codegen_escape_c_string(void *_env, GemVal *args, int argc) {
#line 7 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_escape_c_string", "compiler/main.gem", 7);
#line 8 "compiler/main.gem"
    GemVal _t1809[] = {gem_int(13)};
    GemVal gem_v_cr = gem_chr_fn(NULL, _t1809, 1);
#line 9 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 10 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 10 "compiler/main.gem"
    GemVal _t1810[] = {gem_v_s};
    GemVal gem_v__for_limit_1 = gem_len_fn(NULL, _t1810, 1);
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
    GemVal _t1811[] = {gem_v_b, gem_string("\\\\")};
            (void)(gem_buf_push_fn(NULL, _t1811, 2));
        } else {
#line 14 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ch, gem_string("\"")))) {
#line 15 "compiler/main.gem"
    GemVal _t1812[] = {gem_v_b, gem_string("\\\"")};
                (void)(gem_buf_push_fn(NULL, _t1812, 2));
            } else {
#line 16 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ch, gem_string("\n")))) {
#line 17 "compiler/main.gem"
    GemVal _t1813[] = {gem_v_b, gem_string("\\n")};
                    (void)(gem_buf_push_fn(NULL, _t1813, 2));
                } else {
#line 18 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ch, gem_v_cr))) {
#line 19 "compiler/main.gem"
    GemVal _t1814[] = {gem_v_b, gem_string("\\r")};
                        (void)(gem_buf_push_fn(NULL, _t1814, 2));
                    } else {
#line 20 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ch, gem_string("\t")))) {
#line 21 "compiler/main.gem"
    GemVal _t1815[] = {gem_v_b, gem_string("\\t")};
                            (void)(gem_buf_push_fn(NULL, _t1815, 2));
                        } else {
#line 22 "compiler/main.gem"
    GemVal _t1816[] = {gem_v_ch};
                            if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t1816, 1), gem_int(0)))) {
#line 23 "compiler/main.gem"
    GemVal _t1817[] = {gem_v_b, gem_string("\\0")};
                                (void)(gem_buf_push_fn(NULL, _t1817, 2));
                            } else {
#line 25 "compiler/main.gem"
    GemVal _t1818[] = {gem_v_b, gem_v_ch};
                                (void)(gem_buf_push_fn(NULL, _t1818, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 28 "compiler/main.gem"
    GemVal _t1819[] = {gem_v_b};
    GemVal _t1820 = gem_buf_str_fn(NULL, _t1819, 1);
    gem_pop_frame();
    return _t1820;
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
    GemVal _t1821[] = {gem_v_b, gem_v_s};
        (void)(gem_buf_push_fn(NULL, _t1821, 2));
    }

#line 36 "compiler/main.gem"
    GemVal _t1822[] = {gem_v_b};
    GemVal _t1823 = gem_buf_str_fn(NULL, _t1822, 1);
    gem_pop_frame();
    return _t1823;
}

static GemVal gem_fn__mod_codegen_pad_for(void *_env, GemVal *args, int argc) {
#line 39 "compiler/main.gem"
    GemVal gem_v_indent = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_pad_for", "compiler/main.gem", 39);
#line 40 "compiler/main.gem"
    GemVal _t1824[] = {gem_string("    "), gem_v_indent};
    GemVal _t1825 = gem_fn__mod_codegen_repeat_str(NULL, _t1824, 2);
    gem_pop_frame();
    return _t1825;
}

static GemVal gem_fn__mod_codegen_format_float(void *_env, GemVal *args, int argc) {
#line 43 "compiler/main.gem"
    GemVal gem_v_val = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_format_float", "compiler/main.gem", 43);
#line 44 "compiler/main.gem"
    GemVal _t1826[] = {gem_v_val};
    GemVal gem_v_s = gem_to_string_fn(NULL, _t1826, 1);
#line 46 "compiler/main.gem"
    GemVal gem_v_has_dot = gem_bool(0);
#line 47 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 47 "compiler/main.gem"
    GemVal _t1827[] = {gem_v_s};
    GemVal gem_v__for_limit_3 = gem_len_fn(NULL, _t1827, 1);
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
    GemVal _t1828 = gem_v_s;
    gem_pop_frame();
    return _t1828;
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
    GemVal _t1829 = gem_eq(gem_table_get(gem_v_s, gem_v_val), gem_bool(1));
    gem_pop_frame();
    return _t1829;
}

static GemVal gem_fn__mod_codegen_set_union(void *_env, GemVal *args, int argc) {
#line 70 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_union", "compiler/main.gem", 70);
#line 71 "compiler/main.gem"
    GemVal _t1830 = gem_table_new();
    GemVal gem_v_result = _t1830;
#line 72 "compiler/main.gem"
    GemVal gem_v__for_tbl_4 = gem_v_a;
#line 72 "compiler/main.gem"
    GemVal _t1831[] = {gem_v__for_tbl_4};
    GemVal gem_v__for_len_4 = gem_len_fn(NULL, _t1831, 1);
#line 72 "compiler/main.gem"
    GemVal gem_v__for_i_4 = gem_int(0);
#line 72 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_4, gem_v__for_len_4))) break;
#line 72 "compiler/main.gem"
    GemVal _t1832[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1832, 2);
#line 72 "compiler/main.gem"
    GemVal _t1833[] = {gem_v__for_tbl_4, gem_v__for_i_4};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1833, 2);
#line 72 "compiler/main.gem"
        gem_v__for_i_4 = gem_add(gem_v__for_i_4, gem_int(1));
#line 73 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

#line 75 "compiler/main.gem"
    GemVal gem_v__for_tbl_5 = gem_v_b;
#line 75 "compiler/main.gem"
    GemVal _t1834[] = {gem_v__for_tbl_5};
    GemVal gem_v__for_len_5 = gem_len_fn(NULL, _t1834, 1);
#line 75 "compiler/main.gem"
    GemVal gem_v__for_i_5 = gem_int(0);
#line 75 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_v__for_len_5))) break;
#line 75 "compiler/main.gem"
    GemVal _t1835[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1835, 2);
#line 75 "compiler/main.gem"
    GemVal _t1836[] = {gem_v__for_tbl_5, gem_v__for_i_5};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1836, 2);
#line 75 "compiler/main.gem"
        gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 76 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
    }

    GemVal _t1837 = gem_v_result;
    gem_pop_frame();
    return _t1837;
}

static GemVal gem_fn__mod_codegen_set_intersect(void *_env, GemVal *args, int argc) {
#line 81 "compiler/main.gem"
    GemVal gem_v_a = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_b = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_set_intersect", "compiler/main.gem", 81);
#line 82 "compiler/main.gem"
    GemVal _t1838 = gem_table_new();
    GemVal gem_v_result = _t1838;
#line 83 "compiler/main.gem"
    GemVal gem_v__for_tbl_6 = gem_v_a;
#line 83 "compiler/main.gem"
    GemVal _t1839[] = {gem_v__for_tbl_6};
    GemVal gem_v__for_len_6 = gem_len_fn(NULL, _t1839, 1);
#line 83 "compiler/main.gem"
    GemVal gem_v__for_i_6 = gem_int(0);
#line 83 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_v__for_len_6))) break;
#line 83 "compiler/main.gem"
    GemVal _t1840[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1840, 2);
#line 83 "compiler/main.gem"
    GemVal _t1841[] = {gem_v__for_tbl_6, gem_v__for_i_6};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1841, 2);
#line 83 "compiler/main.gem"
        gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 84 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_v_b, gem_v_k), gem_bool(1)))) {
#line 85 "compiler/main.gem"
            gem_table_set(gem_v_result, gem_v_k, gem_bool(1));
        }
    }

    GemVal _t1842 = gem_v_result;
    gem_pop_frame();
    return _t1842;
}

static GemVal gem_fn__mod_codegen_sorted_array_to_set(void *_env, GemVal *args, int argc) {
#line 91 "compiler/main.gem"
    GemVal gem_v_arr = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_sorted_array_to_set", "compiler/main.gem", 91);
#line 92 "compiler/main.gem"
    GemVal _t1843 = gem_table_new();
    GemVal gem_v_s = _t1843;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_items_7 = gem_v_arr;
#line 93 "compiler/main.gem"
    GemVal gem_v__for_i_7 = gem_int(0);
#line 93 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t1844[] = {gem_v__for_items_7};
        if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t1844, 1)))) break;
#line 93 "compiler/main.gem"
        GemVal gem_v_item = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 93 "compiler/main.gem"
        gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 94 "compiler/main.gem"
        gem_table_set(gem_v_s, gem_v_item, gem_bool(1));
    }

    GemVal _t1845 = gem_v_s;
    gem_pop_frame();
    return _t1845;
}

static GemVal gem_fn__mod_codegen_is_node(void *_env, GemVal *args, int argc) {
#line 99 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_is_node", "compiler/main.gem", 99);
    GemVal _t1846[] = {gem_v_node};
    GemVal _t1848;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1846, 1), gem_string("table")))) {
        _t1848 = gem_eq(gem_type_fn(NULL, _t1846, 1), gem_string("table"));
    } else {
        GemVal _t1847 = gem_v_node;
        static GemICacheSlot _ic_272 = {0};
        _t1848 = gem_eq(gem_table_get_cached(_t1847, "tag", &_ic_272), gem_v_tag);
    }
    GemVal _t1849 = _t1848;
    gem_pop_frame();
    return _t1849;
}

static GemVal gem_fn__mod_codegen_collect_block_lets(void *_env, GemVal *args, int argc) {
#line 105 "compiler/main.gem"
    GemVal gem_v_stmt = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_result = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_mod_codegen_collect_block_lets", "compiler/main.gem", 105);
#line 106 "compiler/main.gem"
    GemVal _t1850[] = {gem_v_stmt, gem_string("block")};
    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1850, 2))) {
        {
#line 107 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 107 "compiler/main.gem"
    GemVal _t1851 = gem_v_stmt;
    static GemICacheSlot _ic_273 = {0};
    GemVal _t1852[] = {gem_table_get_cached(_t1851, "stmts", &_ic_273)};
            GemVal gem_v__for_limit_8 = gem_len_fn(NULL, _t1852, 1);
#line 107 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_v__for_limit_8))) break;
#line 107 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_8;
#line 107 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 108 "compiler/main.gem"
    GemVal _t1853 = gem_v_stmt;
    static GemICacheSlot _ic_274 = {0};
    GemVal _t1854[] = {gem_table_get(gem_table_get_cached(_t1853, "stmts", &_ic_274), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1854, 2))) {
#line 109 "compiler/main.gem"
    GemVal _t1855 = gem_v_stmt;
    static GemICacheSlot _ic_275 = {0};
    GemVal _t1856 = gem_table_get(gem_table_get_cached(_t1855, "stmts", &_ic_275), gem_v_j);
    static GemICacheSlot _ic_276 = {0};
    GemVal _t1857[] = {gem_v_result, gem_table_get_cached(_t1856, "name", &_ic_276)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t1857, 2));
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
    GemVal _t1858[] = {gem_v_stmts};
        GemVal gem_v__for_limit_9 = gem_len_fn(NULL, _t1858, 1);
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
    GemVal _t1859[] = {gem_v_s, gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1859, 2))) {
#line 119 "compiler/main.gem"
    GemVal _t1860 = gem_v_s;
    static GemICacheSlot _ic_277 = {0};
    GemVal _t1861[] = {gem_v_result, gem_table_get_cached(_t1860, "name", &_ic_277)};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1861, 2));
            }
#line 121 "compiler/main.gem"
    GemVal _t1862[] = {gem_v_s, gem_v_result};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t1862, 2));
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

struct _closure__anon_28 {
    intptr_t _num_captures;
    GemVal *gem_v_tmp_counter;
};
static GemVal _anon_28(void *_env, GemVal *args, int argc) {
    struct _closure__anon_28 *_cls = (struct _closure__anon_28 *)_env;
    GemVal *gem_v_tmp_counter = _cls->gem_v_tmp_counter;
    gem_push_frame("_anon_28", "compiler/main.gem", 0);
#line 244 "compiler/main.gem"
    *gem_v_tmp_counter = gem_add((*gem_v_tmp_counter), gem_int(1));
    GemVal _t1878[] = {gem_string("_t"), (*gem_v_tmp_counter)};
    GemVal _t1879 = gem_interp(2, _t1878);
    gem_pop_frame();
    return _t1879;
}

struct _closure__anon_29 {
    intptr_t _num_captures;
    GemVal *gem_v_anon_counter;
};
static GemVal _anon_29(void *_env, GemVal *args, int argc) {
    struct _closure__anon_29 *_cls = (struct _closure__anon_29 *)_env;
    GemVal *gem_v_anon_counter = _cls->gem_v_anon_counter;
    gem_push_frame("_anon_29", "compiler/main.gem", 0);
#line 249 "compiler/main.gem"
    *gem_v_anon_counter = gem_add((*gem_v_anon_counter), gem_int(1));
    GemVal _t1881[] = {gem_string("_anon_"), (*gem_v_anon_counter)};
    GemVal _t1882 = gem_interp(2, _t1881);
    gem_pop_frame();
    return _t1882;
}

struct _closure__anon_30 {
    intptr_t _num_captures;
};
static GemVal _anon_30(void *_env, GemVal *args, int argc) {
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_30", "compiler/main.gem", 0);
    GemVal _t1884[] = {gem_string("gem_v_"), gem_v_name};
    GemVal _t1885 = gem_interp(2, _t1884);
    gem_pop_frame();
    return _t1885;
}

struct _closure__anon_31 {
    intptr_t _num_captures;
    GemVal *gem_v_builtins;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_local_names;
};
static GemVal _anon_31(void *_env, GemVal *args, int argc) {
    struct _closure__anon_31 *_cls = (struct _closure__anon_31 *)_env;
    GemVal *gem_v_builtins = _cls->gem_v_builtins;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_free = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_31", "compiler/main.gem", 0);
#line 263 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 264 "compiler/main.gem"
        GemVal _t1886 = GEM_NIL;
        gem_pop_frame();
        return _t1886;
    }
#line 266 "compiler/main.gem"
    GemVal _t1887[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t1887, 1), gem_string("table")))) {
#line 267 "compiler/main.gem"
        GemVal _t1888 = GEM_NIL;
        gem_pop_frame();
        return _t1888;
    }
#line 269 "compiler/main.gem"
    GemVal _t1889 = gem_v_node;
    static GemICacheSlot _ic_278 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t1889, "tag", &_ic_278), GEM_NIL))) {
#line 270 "compiler/main.gem"
        GemVal _t1890 = GEM_NIL;
        gem_pop_frame();
        return _t1890;
    }
#line 273 "compiler/main.gem"
    GemVal gem_v__match_11 = gem_v_node;
    GemVal _t1891[] = {gem_v__match_11};
    GemVal _t1893;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1891, 1), gem_string("table")))) {
        _t1893 = gem_eq(gem_type_fn(NULL, _t1891, 1), gem_string("table"));
    } else {
        GemVal _t1892[] = {gem_v__match_11, gem_string("tag")};
        _t1893 = gem_has_key_fn(NULL, _t1892, 2);
    }
    GemVal _t1894;
    if (!gem_truthy(_t1893)) {
        _t1894 = _t1893;
    } else {
        _t1894 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("var"));
    }
    GemVal _t1896;
    if (!gem_truthy(_t1894)) {
        _t1896 = _t1894;
    } else {
        GemVal _t1895[] = {gem_v__match_11, gem_string("name")};
        _t1896 = gem_has_key_fn(NULL, _t1895, 2);
    }
    if (gem_truthy(_t1896)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 275 "compiler/main.gem"
    GemVal _t1897[] = {gem_v_defined, gem_v_name};
    GemVal _t1901;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1897, 2)))) {
        _t1901 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1897, 2));
    } else {
        GemVal _t1898[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1900;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1898, 2)))) {
                _t1900 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1898, 2));
        } else {
                GemVal _t1899[] = {(*gem_v_local_names), gem_v_name};
                _t1900 = gem_fn__mod_codegen_set_contains(NULL, _t1899, 2);
        }
        _t1901 = _t1900;
    }
    GemVal _t1903;
    if (!gem_truthy(_t1901)) {
        _t1903 = _t1901;
    } else {
        GemVal _t1902[] = {(*gem_v_defined_fns), gem_v_name};
        _t1903 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1902, 2));
    }
        if (gem_truthy(_t1903)) {
#line 276 "compiler/main.gem"
    GemVal _t1904[] = {gem_v_free, gem_v_name};
            GemVal _t1905 = gem_fn__mod_codegen_set_add(NULL, _t1904, 2);
            gem_pop_frame();
            return _t1905;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1906[] = {gem_v__match_11};
    GemVal _t1908;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1906, 1), gem_string("table")))) {
        _t1908 = gem_eq(gem_type_fn(NULL, _t1906, 1), gem_string("table"));
    } else {
        GemVal _t1907[] = {gem_v__match_11, gem_string("tag")};
        _t1908 = gem_has_key_fn(NULL, _t1907, 2);
    }
    GemVal _t1909;
    if (!gem_truthy(_t1908)) {
        _t1909 = _t1908;
    } else {
        _t1909 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("let"));
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
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 279 "compiler/main.gem"
    GemVal _t1912[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1913 = (*gem_v_collect_free_node);
        GemVal _t1914 = _t1913.fn(_t1913.env, _t1912, 3);
        gem_pop_frame();
        return _t1914;
    } else {
    GemVal _t1915[] = {gem_v__match_11};
    GemVal _t1917;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1915, 1), gem_string("table")))) {
        _t1917 = gem_eq(gem_type_fn(NULL, _t1915, 1), gem_string("table"));
    } else {
        GemVal _t1916[] = {gem_v__match_11, gem_string("tag")};
        _t1917 = gem_has_key_fn(NULL, _t1916, 2);
    }
    GemVal _t1918;
    if (!gem_truthy(_t1917)) {
        _t1918 = _t1917;
    } else {
        _t1918 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t1920;
    if (!gem_truthy(_t1918)) {
        _t1920 = _t1918;
    } else {
        GemVal _t1919[] = {gem_v__match_11, gem_string("name")};
        _t1920 = gem_has_key_fn(NULL, _t1919, 2);
    }
    GemVal _t1922;
    if (!gem_truthy(_t1920)) {
        _t1922 = _t1920;
    } else {
        GemVal _t1921[] = {gem_v__match_11, gem_string("value")};
        _t1922 = gem_has_key_fn(NULL, _t1921, 2);
    }
    if (gem_truthy(_t1922)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_11, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 281 "compiler/main.gem"
    GemVal _t1923[] = {gem_v_defined, gem_v_name};
    GemVal _t1927;
    if (!gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1923, 2)))) {
        _t1927 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1923, 2));
    } else {
        GemVal _t1924[] = {(*gem_v_builtins), gem_v_name};
        GemVal _t1926;
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1924, 2)))) {
                _t1926 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1924, 2));
        } else {
                GemVal _t1925[] = {(*gem_v_local_names), gem_v_name};
                _t1926 = gem_fn__mod_codegen_set_contains(NULL, _t1925, 2);
        }
        _t1927 = _t1926;
    }
    GemVal _t1929;
    if (!gem_truthy(_t1927)) {
        _t1929 = _t1927;
    } else {
        GemVal _t1928[] = {(*gem_v_defined_fns), gem_v_name};
        _t1929 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t1928, 2));
    }
        if (gem_truthy(_t1929)) {
#line 282 "compiler/main.gem"
    GemVal _t1930[] = {gem_v_free, gem_v_name};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1930, 2));
        }
#line 284 "compiler/main.gem"
    GemVal _t1931[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t1932 = (*gem_v_collect_free_node);
        GemVal _t1933 = _t1932.fn(_t1932.env, _t1931, 3);
        gem_pop_frame();
        return _t1933;
    } else {
    GemVal _t1934[] = {gem_v__match_11};
    GemVal _t1936;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1934, 1), gem_string("table")))) {
        _t1936 = gem_eq(gem_type_fn(NULL, _t1934, 1), gem_string("table"));
    } else {
        GemVal _t1935[] = {gem_v__match_11, gem_string("tag")};
        _t1936 = gem_has_key_fn(NULL, _t1935, 2);
    }
    GemVal _t1937;
    if (!gem_truthy(_t1936)) {
        _t1937 = _t1936;
    } else {
        _t1937 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t1939;
    if (!gem_truthy(_t1937)) {
        _t1939 = _t1937;
    } else {
        GemVal _t1938[] = {gem_v__match_11, gem_string("params")};
        _t1939 = gem_has_key_fn(NULL, _t1938, 2);
    }
    GemVal _t1941;
    if (!gem_truthy(_t1939)) {
        _t1941 = _t1939;
    } else {
        GemVal _t1940[] = {gem_v__match_11, gem_string("rest_param")};
        _t1941 = gem_has_key_fn(NULL, _t1940, 2);
    }
    GemVal _t1943;
    if (!gem_truthy(_t1941)) {
        _t1943 = _t1941;
    } else {
        GemVal _t1942[] = {gem_v__match_11, gem_string("body")};
        _t1943 = gem_has_key_fn(NULL, _t1942, 2);
    }
    if (gem_truthy(_t1943)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_11, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_11, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 286 "compiler/main.gem"
    GemVal _t1944[] = {gem_v_params};
    GemVal _t1945[] = {gem_v_defined, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t1944, 1)};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_set_union(NULL, _t1945, 2);
#line 287 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 288 "compiler/main.gem"
    GemVal _t1946[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t1946, 2));
        }
#line 290 "compiler/main.gem"
    GemVal _t1947[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t1948 = (*gem_v_collect_free_vars);
        GemVal gem_v_inner_free = _t1948.fn(_t1948.env, _t1947, 2);
#line 291 "compiler/main.gem"
    GemVal _t1949 = gem_v_node;
    static GemICacheSlot _ic_279 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t1949, "defaults", &_ic_279), GEM_NIL))) {
#line 292 "compiler/main.gem"
    GemVal _t1950 = gem_v_node;
    static GemICacheSlot _ic_280 = {0};
            GemVal gem_v__for_items_12 = gem_table_get_cached(_t1950, "defaults", &_ic_280);
#line 292 "compiler/main.gem"
            GemVal gem_v__for_i_12 = gem_int(0);
#line 292 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t1951[] = {gem_v__for_items_12};
                if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t1951, 1)))) break;
#line 292 "compiler/main.gem"
                GemVal gem_v_d = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 292 "compiler/main.gem"
                gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 293 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 294 "compiler/main.gem"
    GemVal _t1952[] = {gem_v_d, gem_v_inner_defined, gem_v_inner_free};
    GemVal _t1953 = (*gem_v_collect_free_node);
                    (void)(_t1953.fn(_t1953.env, _t1952, 3));
                }
            }

        }
        {
#line 298 "compiler/main.gem"
            GemVal gem_v__for_tbl_13 = gem_v_inner_free;
#line 298 "compiler/main.gem"
    GemVal _t1954[] = {gem_v__for_tbl_13};
            GemVal gem_v__for_len_13 = gem_len_fn(NULL, _t1954, 1);
#line 298 "compiler/main.gem"
            GemVal gem_v__for_i_13 = gem_int(0);
#line 298 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_v__for_len_13))) break;
#line 298 "compiler/main.gem"
    GemVal _t1955[] = {gem_v__for_tbl_13, gem_v__for_i_13};
                GemVal gem_v_ifk = gem_table_key_at_fn(NULL, _t1955, 2);
#line 298 "compiler/main.gem"
    GemVal _t1956[] = {gem_v__for_tbl_13, gem_v__for_i_13};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1956, 2);
#line 298 "compiler/main.gem"
                gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 299 "compiler/main.gem"
    GemVal _t1957[] = {gem_v_free, gem_v_ifk};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1957, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t1958[] = {gem_v__match_11};
    GemVal _t1960;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1958, 1), gem_string("table")))) {
        _t1960 = gem_eq(gem_type_fn(NULL, _t1958, 1), gem_string("table"));
    } else {
        GemVal _t1959[] = {gem_v__match_11, gem_string("tag")};
        _t1960 = gem_has_key_fn(NULL, _t1959, 2);
    }
    GemVal _t1961;
    if (!gem_truthy(_t1960)) {
        _t1961 = _t1960;
    } else {
        _t1961 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t1961)) {
        gem_pop_frame();
        return GEM_NIL;
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
        _t1965 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("if"));
    }
    GemVal _t1967;
    if (!gem_truthy(_t1965)) {
        _t1967 = _t1965;
    } else {
        GemVal _t1966[] = {gem_v__match_11, gem_string("cond")};
        _t1967 = gem_has_key_fn(NULL, _t1966, 2);
    }
    GemVal _t1969;
    if (!gem_truthy(_t1967)) {
        _t1969 = _t1967;
    } else {
        GemVal _t1968[] = {gem_v__match_11, gem_string("then")};
        _t1969 = gem_has_key_fn(NULL, _t1968, 2);
    }
    GemVal _t1971;
    if (!gem_truthy(_t1969)) {
        _t1971 = _t1969;
    } else {
        GemVal _t1970[] = {gem_v__match_11, gem_string("else")};
        _t1971 = gem_has_key_fn(NULL, _t1970, 2);
    }
    if (gem_truthy(_t1971)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_11, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 304 "compiler/main.gem"
    GemVal _t1972[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1973 = (*gem_v_collect_free_node);
        (void)(_t1973.fn(_t1973.env, _t1972, 3));
#line 305 "compiler/main.gem"
    GemVal _t1974 = gem_table_new();
    GemVal _t1975[] = {gem_v_defined, _t1974};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1975, 2);
#line 306 "compiler/main.gem"
        GemVal gem_v__for_i_14 = gem_int(0);
#line 306 "compiler/main.gem"
    GemVal _t1976[] = {gem_v_thens};
        GemVal gem_v__for_limit_14 = gem_len_fn(NULL, _t1976, 1);
#line 306 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_v__for_limit_14))) break;
#line 306 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_14;
#line 306 "compiler/main.gem"
            gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 307 "compiler/main.gem"
    GemVal _t1977[] = {gem_table_get(gem_v_thens, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1978 = (*gem_v_collect_free_node);
            (void)(_t1978.fn(_t1978.env, _t1977, 3));
#line 308 "compiler/main.gem"
    GemVal _t1979[] = {gem_table_get(gem_v_thens, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1979, 2))) {
#line 309 "compiler/main.gem"
    GemVal _t1980 = gem_table_get(gem_v_thens, gem_v_i);
    static GemICacheSlot _ic_281 = {0};
    GemVal _t1981[] = {gem_v_d, gem_table_get_cached(_t1980, "name", &_ic_281)};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t1981, 2));
            }
        }

#line 312 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 313 "compiler/main.gem"
    GemVal _t1982 = gem_table_new();
    GemVal _t1983[] = {gem_v_defined, _t1982};
            gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t1983, 2);
            {
#line 314 "compiler/main.gem"
                GemVal gem_v__for_i_15 = gem_int(0);
#line 314 "compiler/main.gem"
    GemVal _t1984[] = {gem_v_el};
                GemVal gem_v__for_limit_15 = gem_len_fn(NULL, _t1984, 1);
#line 314 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_v__for_limit_15))) break;
#line 314 "compiler/main.gem"
                    GemVal gem_v_i = gem_v__for_i_15;
#line 314 "compiler/main.gem"
                    gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 315 "compiler/main.gem"
    GemVal _t1985[] = {gem_table_get(gem_v_el, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t1986 = (*gem_v_collect_free_node);
                    (void)(_t1986.fn(_t1986.env, _t1985, 3));
#line 316 "compiler/main.gem"
    GemVal _t1987[] = {gem_table_get(gem_v_el, gem_v_i), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t1987, 2))) {
#line 317 "compiler/main.gem"
    GemVal _t1988 = gem_table_get(gem_v_el, gem_v_i);
    static GemICacheSlot _ic_282 = {0};
    GemVal _t1989[] = {gem_v_d, gem_table_get_cached(_t1988, "name", &_ic_282)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t1989, 2));
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
    GemVal _t1990[] = {gem_v__match_11};
    GemVal _t1992;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t1990, 1), gem_string("table")))) {
        _t1992 = gem_eq(gem_type_fn(NULL, _t1990, 1), gem_string("table"));
    } else {
        GemVal _t1991[] = {gem_v__match_11, gem_string("tag")};
        _t1992 = gem_has_key_fn(NULL, _t1991, 2);
    }
    GemVal _t1993;
    if (!gem_truthy(_t1992)) {
        _t1993 = _t1992;
    } else {
        _t1993 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("while"));
    }
    GemVal _t1995;
    if (!gem_truthy(_t1993)) {
        _t1995 = _t1993;
    } else {
        GemVal _t1994[] = {gem_v__match_11, gem_string("cond")};
        _t1995 = gem_has_key_fn(NULL, _t1994, 2);
    }
    GemVal _t1997;
    if (!gem_truthy(_t1995)) {
        _t1997 = _t1995;
    } else {
        GemVal _t1996[] = {gem_v__match_11, gem_string("body")};
        _t1997 = gem_has_key_fn(NULL, _t1996, 2);
    }
    if (gem_truthy(_t1997)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_11, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 322 "compiler/main.gem"
    GemVal _t1998[] = {gem_v_cond, gem_v_defined, gem_v_free};
    GemVal _t1999 = (*gem_v_collect_free_node);
        (void)(_t1999.fn(_t1999.env, _t1998, 3));
#line 323 "compiler/main.gem"
    GemVal _t2000 = gem_table_new();
    GemVal _t2001[] = {gem_v_defined, _t2000};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2001, 2);
        {
#line 324 "compiler/main.gem"
            GemVal gem_v__for_i_16 = gem_int(0);
#line 324 "compiler/main.gem"
    GemVal _t2002[] = {gem_v_body};
            GemVal gem_v__for_limit_16 = gem_len_fn(NULL, _t2002, 1);
#line 324 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_v__for_limit_16))) break;
#line 324 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_16;
#line 324 "compiler/main.gem"
                gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 325 "compiler/main.gem"
    GemVal _t2003[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2004 = (*gem_v_collect_free_node);
                (void)(_t2004.fn(_t2004.env, _t2003, 3));
#line 326 "compiler/main.gem"
    GemVal _t2005[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2005, 2))) {
#line 327 "compiler/main.gem"
    GemVal _t2006 = gem_table_get(gem_v_body, gem_v_i);
    static GemICacheSlot _ic_283 = {0};
    GemVal _t2007[] = {gem_v_d, gem_table_get_cached(_t2006, "name", &_ic_283)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2007, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2008[] = {gem_v__match_11};
    GemVal _t2010;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2008, 1), gem_string("table")))) {
        _t2010 = gem_eq(gem_type_fn(NULL, _t2008, 1), gem_string("table"));
    } else {
        GemVal _t2009[] = {gem_v__match_11, gem_string("tag")};
        _t2010 = gem_has_key_fn(NULL, _t2009, 2);
    }
    GemVal _t2011;
    if (!gem_truthy(_t2010)) {
        _t2011 = _t2010;
    } else {
        _t2011 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2013;
    if (!gem_truthy(_t2011)) {
        _t2013 = _t2011;
    } else {
        GemVal _t2012[] = {gem_v__match_11, gem_string("target")};
        _t2013 = gem_has_key_fn(NULL, _t2012, 2);
    }
    GemVal _t2015;
    if (!gem_truthy(_t2013)) {
        _t2015 = _t2013;
    } else {
        GemVal _t2014[] = {gem_v__match_11, gem_string("target_var")};
        _t2015 = gem_has_key_fn(NULL, _t2014, 2);
    }
    GemVal _t2017;
    if (!gem_truthy(_t2015)) {
        _t2017 = _t2015;
    } else {
        GemVal _t2016[] = {gem_v__match_11, gem_string("whens")};
        _t2017 = gem_has_key_fn(NULL, _t2016, 2);
    }
    GemVal _t2019;
    if (!gem_truthy(_t2017)) {
        _t2019 = _t2017;
    } else {
        GemVal _t2018[] = {gem_v__match_11, gem_string("else")};
        _t2019 = gem_has_key_fn(NULL, _t2018, 2);
    }
    if (gem_truthy(_t2019)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_11, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_target_var = gem_table_get(gem_v__match_11, gem_string("target_var"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_11, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_11, gem_string("else"));
#line 331 "compiler/main.gem"
    GemVal _t2020[] = {gem_v_target, gem_v_defined, gem_v_free};
    GemVal _t2021 = (*gem_v_collect_free_node);
        (void)(_t2021.fn(_t2021.env, _t2020, 3));
#line 332 "compiler/main.gem"
    GemVal _t2022 = gem_table_new();
    GemVal _t2023[] = {gem_v_defined, _t2022};
        GemVal gem_v_match_def = gem_fn__mod_codegen_set_union(NULL, _t2023, 2);
#line 333 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_target_var, GEM_NIL))) {
#line 334 "compiler/main.gem"
    GemVal _t2024[] = {gem_v_match_def, gem_v_target_var};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2024, 2));
        }
#line 336 "compiler/main.gem"
        GemVal gem_v__for_i_19 = gem_int(0);
#line 336 "compiler/main.gem"
    GemVal _t2025[] = {gem_v_whens};
        GemVal gem_v__for_limit_19 = gem_len_fn(NULL, _t2025, 1);
#line 336 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_v__for_limit_19))) break;
#line 336 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_19;
#line 336 "compiler/main.gem"
            gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 337 "compiler/main.gem"
            GemVal gem_v_w = gem_table_get(gem_v_whens, gem_v_i);
#line 338 "compiler/main.gem"
    GemVal _t2026 = gem_v_w;
    static GemICacheSlot _ic_284 = {0};
    GemVal _t2027[] = {gem_table_get_cached(_t2026, "value", &_ic_284), gem_v_match_def, gem_v_free};
    GemVal _t2028 = (*gem_v_collect_free_node);
            (void)(_t2028.fn(_t2028.env, _t2027, 3));
#line 339 "compiler/main.gem"
    GemVal _t2029 = gem_table_new();
    GemVal _t2030[] = {gem_v_match_def, _t2029};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2030, 2);
#line 340 "compiler/main.gem"
    GemVal _t2031 = gem_v_w;
    static GemICacheSlot _ic_285 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2031, "bindings", &_ic_285), GEM_NIL))) {
#line 341 "compiler/main.gem"
                GemVal gem_v__for_i_17 = gem_int(0);
#line 341 "compiler/main.gem"
    GemVal _t2032 = gem_v_w;
    static GemICacheSlot _ic_286 = {0};
    GemVal _t2033[] = {gem_table_get_cached(_t2032, "bindings", &_ic_286)};
                GemVal gem_v__for_limit_17 = gem_len_fn(NULL, _t2033, 1);
#line 341 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_v__for_limit_17))) break;
#line 341 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_17;
#line 341 "compiler/main.gem"
                    gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 342 "compiler/main.gem"
    GemVal _t2034 = gem_v_w;
    static GemICacheSlot _ic_287 = {0};
    GemVal _t2035[] = {gem_table_get(gem_table_get_cached(_t2034, "bindings", &_ic_287), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2036 = (*gem_v_collect_free_node);
                    (void)(_t2036.fn(_t2036.env, _t2035, 3));
#line 343 "compiler/main.gem"
    GemVal _t2037 = gem_v_w;
    static GemICacheSlot _ic_288 = {0};
    GemVal _t2038[] = {gem_table_get(gem_table_get_cached(_t2037, "bindings", &_ic_288), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2038, 2))) {
#line 344 "compiler/main.gem"
    GemVal _t2039 = gem_v_w;
    static GemICacheSlot _ic_289 = {0};
    GemVal _t2040 = gem_table_get(gem_table_get_cached(_t2039, "bindings", &_ic_289), gem_v_bi);
    static GemICacheSlot _ic_290 = {0};
    GemVal _t2041[] = {gem_v_d, gem_table_get_cached(_t2040, "name", &_ic_290)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2041, 2));
                    }
                }

            }
#line 348 "compiler/main.gem"
            GemVal gem_v__for_i_18 = gem_int(0);
#line 348 "compiler/main.gem"
    GemVal _t2042 = gem_v_w;
    static GemICacheSlot _ic_291 = {0};
    GemVal _t2043[] = {gem_table_get_cached(_t2042, "body", &_ic_291)};
            GemVal gem_v__for_limit_18 = gem_len_fn(NULL, _t2043, 1);
#line 348 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_v__for_limit_18))) break;
#line 348 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_18;
#line 348 "compiler/main.gem"
                gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 349 "compiler/main.gem"
    GemVal _t2044 = gem_v_w;
    static GemICacheSlot _ic_292 = {0};
    GemVal _t2045[] = {gem_table_get(gem_table_get_cached(_t2044, "body", &_ic_292), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2046 = (*gem_v_collect_free_node);
                (void)(_t2046.fn(_t2046.env, _t2045, 3));
#line 350 "compiler/main.gem"
    GemVal _t2047 = gem_v_w;
    static GemICacheSlot _ic_293 = {0};
    GemVal _t2048[] = {gem_table_get(gem_table_get_cached(_t2047, "body", &_ic_293), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2048, 2))) {
#line 351 "compiler/main.gem"
    GemVal _t2049 = gem_v_w;
    static GemICacheSlot _ic_294 = {0};
    GemVal _t2050 = gem_table_get(gem_table_get_cached(_t2049, "body", &_ic_294), gem_v_j);
    static GemICacheSlot _ic_295 = {0};
    GemVal _t2051[] = {gem_v_d, gem_table_get_cached(_t2050, "name", &_ic_295)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2051, 2));
                }
            }

        }

#line 355 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 356 "compiler/main.gem"
    GemVal _t2052 = gem_table_new();
    GemVal _t2053[] = {gem_v_defined, _t2052};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2053, 2);
            {
#line 357 "compiler/main.gem"
                GemVal gem_v__for_i_20 = gem_int(0);
#line 357 "compiler/main.gem"
    GemVal _t2054[] = {gem_v_el};
                GemVal gem_v__for_limit_20 = gem_len_fn(NULL, _t2054, 1);
#line 357 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_v__for_limit_20))) break;
#line 357 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_20;
#line 357 "compiler/main.gem"
                    gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 358 "compiler/main.gem"
    GemVal _t2055[] = {gem_table_get(gem_v_el, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2056 = (*gem_v_collect_free_node);
                    (void)(_t2056.fn(_t2056.env, _t2055, 3));
#line 359 "compiler/main.gem"
    GemVal _t2057[] = {gem_table_get(gem_v_el, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2057, 2))) {
#line 360 "compiler/main.gem"
    GemVal _t2058 = gem_table_get(gem_v_el, gem_v_j);
    static GemICacheSlot _ic_296 = {0};
    GemVal _t2059[] = {gem_v_d, gem_table_get_cached(_t2058, "name", &_ic_296)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2059, 2));
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
    GemVal _t2060[] = {gem_v__match_11};
    GemVal _t2062;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2060, 1), gem_string("table")))) {
        _t2062 = gem_eq(gem_type_fn(NULL, _t2060, 1), gem_string("table"));
    } else {
        GemVal _t2061[] = {gem_v__match_11, gem_string("tag")};
        _t2062 = gem_has_key_fn(NULL, _t2061, 2);
    }
    GemVal _t2063;
    if (!gem_truthy(_t2062)) {
        _t2063 = _t2062;
    } else {
        _t2063 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("when_clause"));
    }
    GemVal _t2065;
    if (!gem_truthy(_t2063)) {
        _t2065 = _t2063;
    } else {
        GemVal _t2064[] = {gem_v__match_11, gem_string("value")};
        _t2065 = gem_has_key_fn(NULL, _t2064, 2);
    }
    GemVal _t2067;
    if (!gem_truthy(_t2065)) {
        _t2067 = _t2065;
    } else {
        GemVal _t2066[] = {gem_v__match_11, gem_string("bindings")};
        _t2067 = gem_has_key_fn(NULL, _t2066, 2);
    }
    GemVal _t2069;
    if (!gem_truthy(_t2067)) {
        _t2069 = _t2067;
    } else {
        GemVal _t2068[] = {gem_v__match_11, gem_string("body")};
        _t2069 = gem_has_key_fn(NULL, _t2068, 2);
    }
    if (gem_truthy(_t2069)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_bindings = gem_table_get(gem_v__match_11, gem_string("bindings"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_11, gem_string("body"));
#line 365 "compiler/main.gem"
    GemVal _t2070[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2071 = (*gem_v_collect_free_node);
        (void)(_t2071.fn(_t2071.env, _t2070, 3));
#line 366 "compiler/main.gem"
    GemVal _t2072 = gem_table_new();
    GemVal _t2073[] = {gem_v_defined, _t2072};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2073, 2);
#line 367 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_bindings, GEM_NIL))) {
#line 368 "compiler/main.gem"
            GemVal gem_v__for_i_21 = gem_int(0);
#line 368 "compiler/main.gem"
    GemVal _t2074[] = {gem_v_bindings};
            GemVal gem_v__for_limit_21 = gem_len_fn(NULL, _t2074, 1);
#line 368 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_v__for_limit_21))) break;
#line 368 "compiler/main.gem"
                GemVal gem_v_bi = gem_v__for_i_21;
#line 368 "compiler/main.gem"
                gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 369 "compiler/main.gem"
    GemVal _t2075[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2076 = (*gem_v_collect_free_node);
                (void)(_t2076.fn(_t2076.env, _t2075, 3));
#line 370 "compiler/main.gem"
    GemVal _t2077[] = {gem_table_get(gem_v_bindings, gem_v_bi), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2077, 2))) {
#line 371 "compiler/main.gem"
    GemVal _t2078 = gem_table_get(gem_v_bindings, gem_v_bi);
    static GemICacheSlot _ic_297 = {0};
    GemVal _t2079[] = {gem_v_d, gem_table_get_cached(_t2078, "name", &_ic_297)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2079, 2));
                }
            }

        }
        {
#line 375 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 375 "compiler/main.gem"
    GemVal _t2080[] = {gem_v_body};
            GemVal gem_v__for_limit_22 = gem_len_fn(NULL, _t2080, 1);
#line 375 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_v__for_limit_22))) break;
#line 375 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_22;
#line 375 "compiler/main.gem"
                gem_v__for_i_22 = gem_add(gem_v__for_i_22, gem_int(1));
#line 376 "compiler/main.gem"
    GemVal _t2081[] = {gem_table_get(gem_v_body, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2082 = (*gem_v_collect_free_node);
                (void)(_t2082.fn(_t2082.env, _t2081, 3));
#line 377 "compiler/main.gem"
    GemVal _t2083[] = {gem_table_get(gem_v_body, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2083, 2))) {
#line 378 "compiler/main.gem"
    GemVal _t2084 = gem_table_get(gem_v_body, gem_v_i);
    static GemICacheSlot _ic_298 = {0};
    GemVal _t2085[] = {gem_v_d, gem_table_get_cached(_t2084, "name", &_ic_298)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2085, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2086[] = {gem_v__match_11};
    GemVal _t2088;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2086, 1), gem_string("table")))) {
        _t2088 = gem_eq(gem_type_fn(NULL, _t2086, 1), gem_string("table"));
    } else {
        GemVal _t2087[] = {gem_v__match_11, gem_string("tag")};
        _t2088 = gem_has_key_fn(NULL, _t2087, 2);
    }
    GemVal _t2089;
    if (!gem_truthy(_t2088)) {
        _t2089 = _t2088;
    } else {
        _t2089 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2091;
    if (!gem_truthy(_t2089)) {
        _t2091 = _t2089;
    } else {
        GemVal _t2090[] = {gem_v__match_11, gem_string("arms")};
        _t2091 = gem_has_key_fn(NULL, _t2090, 2);
    }
    GemVal _t2093;
    if (!gem_truthy(_t2091)) {
        _t2093 = _t2091;
    } else {
        GemVal _t2092[] = {gem_v__match_11, gem_string("after_ms")};
        _t2093 = gem_has_key_fn(NULL, _t2092, 2);
    }
    GemVal _t2095;
    if (!gem_truthy(_t2093)) {
        _t2095 = _t2093;
    } else {
        GemVal _t2094[] = {gem_v__match_11, gem_string("after_body")};
        _t2095 = gem_has_key_fn(NULL, _t2094, 2);
    }
    if (gem_truthy(_t2095)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_11, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_11, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_11, gem_string("after_body"));
#line 382 "compiler/main.gem"
    GemVal _t2096 = gem_table_new();
    GemVal _t2097[] = {gem_v_defined, _t2096};
        GemVal gem_v_recv_def = gem_fn__mod_codegen_set_union(NULL, _t2097, 2);
#line 383 "compiler/main.gem"
    GemVal _t2098 = gem_v_node;
    static GemICacheSlot _ic_299 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t2098, "recv_var", &_ic_299), GEM_NIL))) {
#line 384 "compiler/main.gem"
    GemVal _t2099 = gem_v_node;
    static GemICacheSlot _ic_300 = {0};
    GemVal _t2100[] = {gem_v_recv_def, gem_table_get_cached(_t2099, "recv_var", &_ic_300)};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2100, 2));
        }
#line 386 "compiler/main.gem"
        GemVal gem_v__for_i_25 = gem_int(0);
#line 386 "compiler/main.gem"
    GemVal _t2101[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_25 = gem_len_fn(NULL, _t2101, 1);
#line 386 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_25, gem_v__for_limit_25))) break;
#line 386 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_25;
#line 386 "compiler/main.gem"
            gem_v__for_i_25 = gem_add(gem_v__for_i_25, gem_int(1));
#line 387 "compiler/main.gem"
            GemVal gem_v_arm = gem_table_get(gem_v_rm_arms, gem_v_i);
#line 388 "compiler/main.gem"
    GemVal _t2102 = gem_v_arm;
    static GemICacheSlot _ic_301 = {0};
    GemVal _t2103 = gem_table_get_cached(_t2102, "pattern", &_ic_301);
    static GemICacheSlot _ic_302 = {0};
    GemVal _t2104[] = {gem_table_get_cached(_t2103, "condition", &_ic_302), gem_v_recv_def, gem_v_free};
    GemVal _t2105 = (*gem_v_collect_free_node);
            (void)(_t2105.fn(_t2105.env, _t2104, 3));
#line 389 "compiler/main.gem"
    GemVal _t2106 = gem_table_new();
    GemVal _t2107[] = {gem_v_recv_def, _t2106};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2107, 2);
#line 390 "compiler/main.gem"
    GemVal _t2108 = gem_v_arm;
    static GemICacheSlot _ic_303 = {0};
    GemVal _t2109 = gem_table_get_cached(_t2108, "pattern", &_ic_303);
    static GemICacheSlot _ic_304 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2109, "bindings", &_ic_304), GEM_NIL))) {
#line 391 "compiler/main.gem"
                GemVal gem_v__for_i_23 = gem_int(0);
#line 391 "compiler/main.gem"
    GemVal _t2110 = gem_v_arm;
    static GemICacheSlot _ic_305 = {0};
    GemVal _t2111 = gem_table_get_cached(_t2110, "pattern", &_ic_305);
    static GemICacheSlot _ic_306 = {0};
    GemVal _t2112[] = {gem_table_get_cached(_t2111, "bindings", &_ic_306)};
                GemVal gem_v__for_limit_23 = gem_len_fn(NULL, _t2112, 1);
#line 391 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_23, gem_v__for_limit_23))) break;
#line 391 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_23;
#line 391 "compiler/main.gem"
                    gem_v__for_i_23 = gem_add(gem_v__for_i_23, gem_int(1));
#line 392 "compiler/main.gem"
    GemVal _t2113 = gem_v_arm;
    static GemICacheSlot _ic_307 = {0};
    GemVal _t2114 = gem_table_get_cached(_t2113, "pattern", &_ic_307);
    static GemICacheSlot _ic_308 = {0};
    GemVal _t2115[] = {gem_table_get(gem_table_get_cached(_t2114, "bindings", &_ic_308), gem_v_bi), gem_v_d, gem_v_free};
    GemVal _t2116 = (*gem_v_collect_free_node);
                    (void)(_t2116.fn(_t2116.env, _t2115, 3));
#line 393 "compiler/main.gem"
    GemVal _t2117 = gem_v_arm;
    static GemICacheSlot _ic_309 = {0};
    GemVal _t2118 = gem_table_get_cached(_t2117, "pattern", &_ic_309);
    static GemICacheSlot _ic_310 = {0};
    GemVal _t2119[] = {gem_table_get(gem_table_get_cached(_t2118, "bindings", &_ic_310), gem_v_bi), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2119, 2))) {
#line 394 "compiler/main.gem"
    GemVal _t2120 = gem_v_arm;
    static GemICacheSlot _ic_311 = {0};
    GemVal _t2121 = gem_table_get_cached(_t2120, "pattern", &_ic_311);
    static GemICacheSlot _ic_312 = {0};
    GemVal _t2122 = gem_table_get(gem_table_get_cached(_t2121, "bindings", &_ic_312), gem_v_bi);
    static GemICacheSlot _ic_313 = {0};
    GemVal _t2123[] = {gem_v_d, gem_table_get_cached(_t2122, "name", &_ic_313)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2123, 2));
                    }
                }

            }
#line 398 "compiler/main.gem"
            GemVal gem_v__for_i_24 = gem_int(0);
#line 398 "compiler/main.gem"
    GemVal _t2124 = gem_v_arm;
    static GemICacheSlot _ic_314 = {0};
    GemVal _t2125[] = {gem_table_get_cached(_t2124, "body", &_ic_314)};
            GemVal gem_v__for_limit_24 = gem_len_fn(NULL, _t2125, 1);
#line 398 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_24, gem_v__for_limit_24))) break;
#line 398 "compiler/main.gem"
                GemVal gem_v_j = gem_v__for_i_24;
#line 398 "compiler/main.gem"
                gem_v__for_i_24 = gem_add(gem_v__for_i_24, gem_int(1));
#line 399 "compiler/main.gem"
    GemVal _t2126 = gem_v_arm;
    static GemICacheSlot _ic_315 = {0};
    GemVal _t2127[] = {gem_table_get(gem_table_get_cached(_t2126, "body", &_ic_315), gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2128 = (*gem_v_collect_free_node);
                (void)(_t2128.fn(_t2128.env, _t2127, 3));
#line 400 "compiler/main.gem"
    GemVal _t2129 = gem_v_arm;
    static GemICacheSlot _ic_316 = {0};
    GemVal _t2130[] = {gem_table_get(gem_table_get_cached(_t2129, "body", &_ic_316), gem_v_j), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2130, 2))) {
#line 401 "compiler/main.gem"
    GemVal _t2131 = gem_v_arm;
    static GemICacheSlot _ic_317 = {0};
    GemVal _t2132 = gem_table_get(gem_table_get_cached(_t2131, "body", &_ic_317), gem_v_j);
    static GemICacheSlot _ic_318 = {0};
    GemVal _t2133[] = {gem_v_d, gem_table_get_cached(_t2132, "name", &_ic_318)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2133, 2));
                }
            }

        }

#line 405 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 406 "compiler/main.gem"
    GemVal _t2134[] = {gem_v_rm_after_ms, gem_v_recv_def, gem_v_free};
    GemVal _t2135 = (*gem_v_collect_free_node);
            (void)(_t2135.fn(_t2135.env, _t2134, 3));
        }
#line 408 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 409 "compiler/main.gem"
    GemVal _t2136 = gem_table_new();
    GemVal _t2137[] = {gem_v_recv_def, _t2136};
            GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2137, 2);
            {
#line 410 "compiler/main.gem"
                GemVal gem_v__for_i_26 = gem_int(0);
#line 410 "compiler/main.gem"
    GemVal _t2138[] = {gem_v_rm_after_body};
                GemVal gem_v__for_limit_26 = gem_len_fn(NULL, _t2138, 1);
#line 410 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_26, gem_v__for_limit_26))) break;
#line 410 "compiler/main.gem"
                    GemVal gem_v_j = gem_v__for_i_26;
#line 410 "compiler/main.gem"
                    gem_v__for_i_26 = gem_add(gem_v__for_i_26, gem_int(1));
#line 411 "compiler/main.gem"
    GemVal _t2139[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_v_d, gem_v_free};
    GemVal _t2140 = (*gem_v_collect_free_node);
                    (void)(_t2140.fn(_t2140.env, _t2139, 3));
#line 412 "compiler/main.gem"
    GemVal _t2141[] = {gem_table_get(gem_v_rm_after_body, gem_v_j), gem_string("let")};
                    if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2141, 2))) {
#line 413 "compiler/main.gem"
    GemVal _t2142 = gem_table_get(gem_v_rm_after_body, gem_v_j);
    static GemICacheSlot _ic_319 = {0};
    GemVal _t2143[] = {gem_v_d, gem_table_get_cached(_t2142, "name", &_ic_319)};
                        (void)(gem_fn__mod_codegen_set_add(NULL, _t2143, 2));
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
    GemVal _t2144[] = {gem_v__match_11};
    GemVal _t2146;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2144, 1), gem_string("table")))) {
        _t2146 = gem_eq(gem_type_fn(NULL, _t2144, 1), gem_string("table"));
    } else {
        GemVal _t2145[] = {gem_v__match_11, gem_string("tag")};
        _t2146 = gem_has_key_fn(NULL, _t2145, 2);
    }
    GemVal _t2147;
    if (!gem_truthy(_t2146)) {
        _t2147 = _t2146;
    } else {
        _t2147 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2149;
    if (!gem_truthy(_t2147)) {
        _t2149 = _t2147;
    } else {
        GemVal _t2148[] = {gem_v__match_11, gem_string("entries")};
        _t2149 = gem_has_key_fn(NULL, _t2148, 2);
    }
    if (gem_truthy(_t2149)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_11, gem_string("entries"));
        {
#line 418 "compiler/main.gem"
            GemVal gem_v__for_i_27 = gem_int(0);
#line 418 "compiler/main.gem"
    GemVal _t2150[] = {gem_v_entries};
            GemVal gem_v__for_limit_27 = gem_len_fn(NULL, _t2150, 1);
#line 418 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_27, gem_v__for_limit_27))) break;
#line 418 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_27;
#line 418 "compiler/main.gem"
                gem_v__for_i_27 = gem_add(gem_v__for_i_27, gem_int(1));
#line 419 "compiler/main.gem"
    GemVal _t2151 = gem_table_get(gem_v_entries, gem_v_i);
    static GemICacheSlot _ic_320 = {0};
    GemVal _t2152[] = {gem_table_get_cached(_t2151, "value", &_ic_320), gem_v_defined, gem_v_free};
    GemVal _t2153 = (*gem_v_collect_free_node);
                (void)(_t2153.fn(_t2153.env, _t2152, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2154[] = {gem_v__match_11};
    GemVal _t2156;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2154, 1), gem_string("table")))) {
        _t2156 = gem_eq(gem_type_fn(NULL, _t2154, 1), gem_string("table"));
    } else {
        GemVal _t2155[] = {gem_v__match_11, gem_string("tag")};
        _t2156 = gem_has_key_fn(NULL, _t2155, 2);
    }
    GemVal _t2157;
    if (!gem_truthy(_t2156)) {
        _t2157 = _t2156;
    } else {
        _t2157 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2159;
    if (!gem_truthy(_t2157)) {
        _t2159 = _t2157;
    } else {
        GemVal _t2158[] = {gem_v__match_11, gem_string("elements")};
        _t2159 = gem_has_key_fn(NULL, _t2158, 2);
    }
    if (gem_truthy(_t2159)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_11, gem_string("elements"));
        {
#line 422 "compiler/main.gem"
            GemVal gem_v__for_i_28 = gem_int(0);
#line 422 "compiler/main.gem"
    GemVal _t2160[] = {gem_v_elements};
            GemVal gem_v__for_limit_28 = gem_len_fn(NULL, _t2160, 1);
#line 422 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_28, gem_v__for_limit_28))) break;
#line 422 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_28;
#line 422 "compiler/main.gem"
                gem_v__for_i_28 = gem_add(gem_v__for_i_28, gem_int(1));
#line 423 "compiler/main.gem"
    GemVal _t2161[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_defined, gem_v_free};
    GemVal _t2162 = (*gem_v_collect_free_node);
                (void)(_t2162.fn(_t2162.env, _t2161, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2163[] = {gem_v__match_11};
    GemVal _t2165;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2163, 1), gem_string("table")))) {
        _t2165 = gem_eq(gem_type_fn(NULL, _t2163, 1), gem_string("table"));
    } else {
        GemVal _t2164[] = {gem_v__match_11, gem_string("tag")};
        _t2165 = gem_has_key_fn(NULL, _t2164, 2);
    }
    GemVal _t2166;
    if (!gem_truthy(_t2165)) {
        _t2166 = _t2165;
    } else {
        _t2166 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2168;
    if (!gem_truthy(_t2166)) {
        _t2168 = _t2166;
    } else {
        GemVal _t2167[] = {gem_v__match_11, gem_string("stmts")};
        _t2168 = gem_has_key_fn(NULL, _t2167, 2);
    }
    if (gem_truthy(_t2168)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_11, gem_string("stmts"));
#line 426 "compiler/main.gem"
    GemVal _t2169 = gem_table_new();
    GemVal _t2170[] = {gem_v_defined, _t2169};
        GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2170, 2);
        {
#line 427 "compiler/main.gem"
            GemVal gem_v__for_i_29 = gem_int(0);
#line 427 "compiler/main.gem"
    GemVal _t2171[] = {gem_v_stmts};
            GemVal gem_v__for_limit_29 = gem_len_fn(NULL, _t2171, 1);
#line 427 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_29, gem_v__for_limit_29))) break;
#line 427 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_29;
#line 427 "compiler/main.gem"
                gem_v__for_i_29 = gem_add(gem_v__for_i_29, gem_int(1));
#line 428 "compiler/main.gem"
    GemVal _t2172[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2173 = (*gem_v_collect_free_node);
                (void)(_t2173.fn(_t2173.env, _t2172, 3));
#line 429 "compiler/main.gem"
    GemVal _t2174[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
                if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2174, 2))) {
#line 430 "compiler/main.gem"
    GemVal _t2175 = gem_table_get(gem_v_stmts, gem_v_i);
    static GemICacheSlot _ic_321 = {0};
    GemVal _t2176[] = {gem_v_d, gem_table_get_cached(_t2175, "name", &_ic_321)};
                    (void)(gem_fn__mod_codegen_set_add(NULL, _t2176, 2));
                }
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2177[] = {gem_v__match_11};
    GemVal _t2179;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2177, 1), gem_string("table")))) {
        _t2179 = gem_eq(gem_type_fn(NULL, _t2177, 1), gem_string("table"));
    } else {
        GemVal _t2178[] = {gem_v__match_11, gem_string("tag")};
        _t2179 = gem_has_key_fn(NULL, _t2178, 2);
    }
    GemVal _t2180;
    if (!gem_truthy(_t2179)) {
        _t2180 = _t2179;
    } else {
        _t2180 = gem_eq(gem_table_get(gem_v__match_11, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2182;
    if (!gem_truthy(_t2180)) {
        _t2182 = _t2180;
    } else {
        GemVal _t2181[] = {gem_v__match_11, gem_string("value")};
        _t2182 = gem_has_key_fn(NULL, _t2181, 2);
    }
    if (gem_truthy(_t2182)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_11, gem_string("value"));
#line 434 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 435 "compiler/main.gem"
    GemVal _t2183[] = {gem_v_value, gem_v_defined, gem_v_free};
    GemVal _t2184 = (*gem_v_collect_free_node);
            GemVal _t2185 = _t2184.fn(_t2184.env, _t2183, 3);
            gem_pop_frame();
            return _t2185;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
        {
#line 439 "compiler/main.gem"
            GemVal gem_v__for_tbl_30 = gem_v_node;
#line 439 "compiler/main.gem"
    GemVal _t2186[] = {gem_v__for_tbl_30};
            GemVal gem_v__for_len_30 = gem_len_fn(NULL, _t2186, 1);
#line 439 "compiler/main.gem"
            GemVal gem_v__for_i_30 = gem_int(0);
#line 439 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_30, gem_v__for_len_30))) break;
#line 439 "compiler/main.gem"
    GemVal _t2187[] = {gem_v__for_tbl_30, gem_v__for_i_30};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2187, 2);
#line 439 "compiler/main.gem"
    GemVal _t2188[] = {gem_v__for_tbl_30, gem_v__for_i_30};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2188, 2);
#line 439 "compiler/main.gem"
                gem_v__for_i_30 = gem_add(gem_v__for_i_30, gem_int(1));
#line 440 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 441 "compiler/main.gem"
    GemVal _t2189[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2189, 1), gem_string("table")))) {
#line 442 "compiler/main.gem"
    GemVal _t2190[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2190, 1);
#line 443 "compiler/main.gem"
    GemVal _t2191[] = {gem_v_vks};
    GemVal _t2193;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2191, 1), gem_int(0)))) {
        _t2193 = gem_gt(gem_len_fn(NULL, _t2191, 1), gem_int(0));
    } else {
        GemVal _t2192[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2193 = gem_eq(gem_type_fn(NULL, _t2192, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2193)) {
#line 444 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 445 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2194[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2194, 1)))) break;
#line 446 "compiler/main.gem"
    GemVal _t2195[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2195, 1), gem_string("table")))) {
#line 447 "compiler/main.gem"
    GemVal _t2196[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_defined, gem_v_free};
    GemVal _t2197 = (*gem_v_collect_free_node);
                                    (void)(_t2197.fn(_t2197.env, _t2196, 3));
                                }
#line 449 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 452 "compiler/main.gem"
    GemVal _t2198[] = {gem_v_v, gem_v_defined, gem_v_free};
    GemVal _t2199 = (*gem_v_collect_free_node);
                            (void)(_t2199.fn(_t2199.env, _t2198, 3));
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

struct _closure__anon_32 {
    intptr_t _num_captures;
    GemVal *gem_v_collect_free_node;
};
static GemVal _anon_32(void *_env, GemVal *args, int argc) {
    struct _closure__anon_32 *_cls = (struct _closure__anon_32 *)_env;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_defined = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_32", "compiler/main.gem", 0);
#line 461 "compiler/main.gem"
    GemVal _t2201 = gem_table_new();
    GemVal gem_v_free = _t2201;
#line 462 "compiler/main.gem"
    GemVal _t2202 = gem_table_new();
    GemVal _t2203[] = {gem_v_defined, _t2202};
    GemVal gem_v_d = gem_fn__mod_codegen_set_union(NULL, _t2203, 2);
#line 463 "compiler/main.gem"
    GemVal gem_v__for_i_31 = gem_int(0);
#line 463 "compiler/main.gem"
    GemVal _t2204[] = {gem_v_stmts};
    GemVal gem_v__for_limit_31 = gem_len_fn(NULL, _t2204, 1);
#line 463 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_31, gem_v__for_limit_31))) break;
#line 463 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_31;
#line 463 "compiler/main.gem"
        gem_v__for_i_31 = gem_add(gem_v__for_i_31, gem_int(1));
#line 464 "compiler/main.gem"
    GemVal _t2205[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d, gem_v_free};
    GemVal _t2206 = (*gem_v_collect_free_node);
        (void)(_t2206.fn(_t2206.env, _t2205, 3));
#line 465 "compiler/main.gem"
    GemVal _t2207[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
        if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2207, 2))) {
#line 466 "compiler/main.gem"
    GemVal _t2208 = gem_table_get(gem_v_stmts, gem_v_i);
    static GemICacheSlot _ic_322 = {0};
    GemVal _t2209[] = {gem_v_d, gem_table_get_cached(_t2208, "name", &_ic_322)};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2209, 2));
        }
#line 468 "compiler/main.gem"
    GemVal _t2210[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_d};
        (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2210, 2));
    }

    GemVal _t2211 = gem_v_free;
    gem_pop_frame();
    return _t2211;
}

struct _closure__anon_33 {
    intptr_t _num_captures;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_33(void *_env, GemVal *args, int argc) {
    struct _closure__anon_33 *_cls = (struct _closure__anon_33 *)_env;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_33", "compiler/main.gem", 0);
#line 479 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 480 "compiler/main.gem"
        GemVal _t2213 = GEM_NIL;
        gem_pop_frame();
        return _t2213;
    }
#line 482 "compiler/main.gem"
    GemVal _t2214[] = {gem_v_node};
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2214, 1), gem_string("table")))) {
#line 483 "compiler/main.gem"
        GemVal _t2215 = GEM_NIL;
        gem_pop_frame();
        return _t2215;
    }
#line 485 "compiler/main.gem"
    GemVal _t2216 = gem_v_node;
    static GemICacheSlot _ic_323 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2216, "tag", &_ic_323), GEM_NIL))) {
#line 486 "compiler/main.gem"
        GemVal _t2217 = GEM_NIL;
        gem_pop_frame();
        return _t2217;
    }
#line 489 "compiler/main.gem"
    GemVal gem_v__match_32 = gem_v_node;
    GemVal _t2218[] = {gem_v__match_32};
    GemVal _t2220;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2218, 1), gem_string("table")))) {
        _t2220 = gem_eq(gem_type_fn(NULL, _t2218, 1), gem_string("table"));
    } else {
        GemVal _t2219[] = {gem_v__match_32, gem_string("tag")};
        _t2220 = gem_has_key_fn(NULL, _t2219, 2);
    }
    GemVal _t2221;
    if (!gem_truthy(_t2220)) {
        _t2221 = _t2220;
    } else {
        _t2221 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t2223;
    if (!gem_truthy(_t2221)) {
        _t2223 = _t2221;
    } else {
        GemVal _t2222[] = {gem_v__match_32, gem_string("params")};
        _t2223 = gem_has_key_fn(NULL, _t2222, 2);
    }
    GemVal _t2225;
    if (!gem_truthy(_t2223)) {
        _t2225 = _t2223;
    } else {
        GemVal _t2224[] = {gem_v__match_32, gem_string("rest_param")};
        _t2225 = gem_has_key_fn(NULL, _t2224, 2);
    }
    GemVal _t2227;
    if (!gem_truthy(_t2225)) {
        _t2227 = _t2225;
    } else {
        GemVal _t2226[] = {gem_v__match_32, gem_string("body")};
        _t2227 = gem_has_key_fn(NULL, _t2226, 2);
    }
    if (gem_truthy(_t2227)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_params = gem_table_get(gem_v__match_32, gem_string("params"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rest_param = gem_table_get(gem_v__match_32, gem_string("rest_param"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_32, gem_string("body"));
#line 491 "compiler/main.gem"
    GemVal _t2228[] = {gem_v_params};
        GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2228, 1);
#line 492 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rest_param, GEM_NIL))) {
#line 493 "compiler/main.gem"
    GemVal _t2229[] = {gem_v_inner_defined, gem_v_rest_param};
            (void)(gem_fn__mod_codegen_set_add(NULL, _t2229, 2));
        }
#line 495 "compiler/main.gem"
    GemVal _t2230[] = {gem_v_body, gem_v_inner_defined};
    GemVal _t2231 = (*gem_v_collect_free_vars);
        GemVal gem_v_free = _t2231.fn(_t2231.env, _t2230, 2);
#line 496 "compiler/main.gem"
    GemVal _t2232 = gem_v_node;
    static GemICacheSlot _ic_324 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t2232, "defaults", &_ic_324), GEM_NIL))) {
#line 497 "compiler/main.gem"
    GemVal _t2233 = gem_v_node;
    static GemICacheSlot _ic_325 = {0};
            GemVal gem_v__for_items_33 = gem_table_get_cached(_t2233, "defaults", &_ic_325);
#line 497 "compiler/main.gem"
            GemVal gem_v__for_i_33 = gem_int(0);
#line 497 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t2234[] = {gem_v__for_items_33};
                if (!gem_truthy(gem_lt(gem_v__for_i_33, gem_len_fn(NULL, _t2234, 1)))) break;
#line 497 "compiler/main.gem"
                GemVal gem_v_d = gem_table_get(gem_v__for_items_33, gem_v__for_i_33);
#line 497 "compiler/main.gem"
                gem_v__for_i_33 = gem_add(gem_v__for_i_33, gem_int(1));
#line 498 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 499 "compiler/main.gem"
    GemVal _t2235[] = {gem_v_d, gem_v_inner_defined, gem_v_free};
    GemVal _t2236 = (*gem_v_collect_free_node);
                    (void)(_t2236.fn(_t2236.env, _t2235, 3));
                }
            }

        }
#line 503 "compiler/main.gem"
    GemVal _t2237[] = {gem_v_free, gem_v_scope_vars};
        GemVal gem_v_inter = gem_fn__mod_codegen_set_intersect(NULL, _t2237, 2);
        {
#line 504 "compiler/main.gem"
            GemVal gem_v__for_tbl_34 = gem_v_inter;
#line 504 "compiler/main.gem"
    GemVal _t2238[] = {gem_v__for_tbl_34};
            GemVal gem_v__for_len_34 = gem_len_fn(NULL, _t2238, 1);
#line 504 "compiler/main.gem"
            GemVal gem_v__for_i_34 = gem_int(0);
#line 504 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_34, gem_v__for_len_34))) break;
#line 504 "compiler/main.gem"
    GemVal _t2239[] = {gem_v__for_tbl_34, gem_v__for_i_34};
                GemVal gem_v_ik = gem_table_key_at_fn(NULL, _t2239, 2);
#line 504 "compiler/main.gem"
    GemVal _t2240[] = {gem_v__for_tbl_34, gem_v__for_i_34};
                GemVal gem_v__ = gem_table_val_at_fn(NULL, _t2240, 2);
#line 504 "compiler/main.gem"
                gem_v__for_i_34 = gem_add(gem_v__for_i_34, gem_int(1));
#line 505 "compiler/main.gem"
    GemVal _t2241[] = {gem_v_captured, gem_v_ik};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2241, 2));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2242[] = {gem_v__match_32};
    GemVal _t2244;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2242, 1), gem_string("table")))) {
        _t2244 = gem_eq(gem_type_fn(NULL, _t2242, 1), gem_string("table"));
    } else {
        GemVal _t2243[] = {gem_v__match_32, gem_string("tag")};
        _t2244 = gem_has_key_fn(NULL, _t2243, 2);
    }
    GemVal _t2245;
    if (!gem_truthy(_t2244)) {
        _t2245 = _t2244;
    } else {
        _t2245 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t2245)) {
        gem_pop_frame();
        return GEM_NIL;
    } else {
    GemVal _t2246[] = {gem_v__match_32};
    GemVal _t2248;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2246, 1), gem_string("table")))) {
        _t2248 = gem_eq(gem_type_fn(NULL, _t2246, 1), gem_string("table"));
    } else {
        GemVal _t2247[] = {gem_v__match_32, gem_string("tag")};
        _t2248 = gem_has_key_fn(NULL, _t2247, 2);
    }
    GemVal _t2249;
    if (!gem_truthy(_t2248)) {
        _t2249 = _t2248;
    } else {
        _t2249 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2251;
    if (!gem_truthy(_t2249)) {
        _t2251 = _t2249;
    } else {
        GemVal _t2250[] = {gem_v__match_32, gem_string("cond")};
        _t2251 = gem_has_key_fn(NULL, _t2250, 2);
    }
    GemVal _t2253;
    if (!gem_truthy(_t2251)) {
        _t2253 = _t2251;
    } else {
        GemVal _t2252[] = {gem_v__match_32, gem_string("then")};
        _t2253 = gem_has_key_fn(NULL, _t2252, 2);
    }
    GemVal _t2255;
    if (!gem_truthy(_t2253)) {
        _t2255 = _t2253;
    } else {
        GemVal _t2254[] = {gem_v__match_32, gem_string("else")};
        _t2255 = gem_has_key_fn(NULL, _t2254, 2);
    }
    if (gem_truthy(_t2255)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_32, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_thens = gem_table_get(gem_v__match_32, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_32, gem_string("else"));
#line 510 "compiler/main.gem"
    GemVal _t2256[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2257 = (*gem_v_walk_captures_node);
        (void)(_t2257.fn(_t2257.env, _t2256, 3));
#line 511 "compiler/main.gem"
    GemVal _t2258[] = {gem_v_thens, gem_v_scope_vars, gem_v_captured};
    GemVal _t2259 = (*gem_v_walk_captures);
        (void)(_t2259.fn(_t2259.env, _t2258, 3));
#line 512 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 513 "compiler/main.gem"
    GemVal _t2260[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2261 = (*gem_v_walk_captures);
            GemVal _t2262 = _t2261.fn(_t2261.env, _t2260, 3);
            gem_pop_frame();
            return _t2262;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2263[] = {gem_v__match_32};
    GemVal _t2265;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2263, 1), gem_string("table")))) {
        _t2265 = gem_eq(gem_type_fn(NULL, _t2263, 1), gem_string("table"));
    } else {
        GemVal _t2264[] = {gem_v__match_32, gem_string("tag")};
        _t2265 = gem_has_key_fn(NULL, _t2264, 2);
    }
    GemVal _t2266;
    if (!gem_truthy(_t2265)) {
        _t2266 = _t2265;
    } else {
        _t2266 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("while"));
    }
    GemVal _t2268;
    if (!gem_truthy(_t2266)) {
        _t2268 = _t2266;
    } else {
        GemVal _t2267[] = {gem_v__match_32, gem_string("cond")};
        _t2268 = gem_has_key_fn(NULL, _t2267, 2);
    }
    GemVal _t2270;
    if (!gem_truthy(_t2268)) {
        _t2270 = _t2268;
    } else {
        GemVal _t2269[] = {gem_v__match_32, gem_string("body")};
        _t2270 = gem_has_key_fn(NULL, _t2269, 2);
    }
    if (gem_truthy(_t2270)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_cond = gem_table_get(gem_v__match_32, gem_string("cond"));
#line 0 "compiler/main.gem"
        GemVal gem_v_body = gem_table_get(gem_v__match_32, gem_string("body"));
#line 516 "compiler/main.gem"
    GemVal _t2271[] = {gem_v_cond, gem_v_scope_vars, gem_v_captured};
    GemVal _t2272 = (*gem_v_walk_captures_node);
        (void)(_t2272.fn(_t2272.env, _t2271, 3));
#line 517 "compiler/main.gem"
    GemVal _t2273[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2274 = (*gem_v_walk_captures);
        GemVal _t2275 = _t2274.fn(_t2274.env, _t2273, 3);
        gem_pop_frame();
        return _t2275;
    } else {
    GemVal _t2276[] = {gem_v__match_32};
    GemVal _t2278;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2276, 1), gem_string("table")))) {
        _t2278 = gem_eq(gem_type_fn(NULL, _t2276, 1), gem_string("table"));
    } else {
        GemVal _t2277[] = {gem_v__match_32, gem_string("tag")};
        _t2278 = gem_has_key_fn(NULL, _t2277, 2);
    }
    GemVal _t2279;
    if (!gem_truthy(_t2278)) {
        _t2279 = _t2278;
    } else {
        _t2279 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2281;
    if (!gem_truthy(_t2279)) {
        _t2281 = _t2279;
    } else {
        GemVal _t2280[] = {gem_v__match_32, gem_string("target")};
        _t2281 = gem_has_key_fn(NULL, _t2280, 2);
    }
    GemVal _t2283;
    if (!gem_truthy(_t2281)) {
        _t2283 = _t2281;
    } else {
        GemVal _t2282[] = {gem_v__match_32, gem_string("whens")};
        _t2283 = gem_has_key_fn(NULL, _t2282, 2);
    }
    GemVal _t2285;
    if (!gem_truthy(_t2283)) {
        _t2285 = _t2283;
    } else {
        GemVal _t2284[] = {gem_v__match_32, gem_string("else")};
        _t2285 = gem_has_key_fn(NULL, _t2284, 2);
    }
    if (gem_truthy(_t2285)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_target = gem_table_get(gem_v__match_32, gem_string("target"));
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_32, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_el = gem_table_get(gem_v__match_32, gem_string("else"));
#line 519 "compiler/main.gem"
    GemVal _t2286[] = {gem_v_target, gem_v_scope_vars, gem_v_captured};
    GemVal _t2287 = (*gem_v_walk_captures_node);
        (void)(_t2287.fn(_t2287.env, _t2286, 3));
#line 520 "compiler/main.gem"
        GemVal gem_v__for_i_36 = gem_int(0);
#line 520 "compiler/main.gem"
    GemVal _t2288[] = {gem_v_whens};
        GemVal gem_v__for_limit_36 = gem_len_fn(NULL, _t2288, 1);
#line 520 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_36, gem_v__for_limit_36))) break;
#line 520 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_36;
#line 520 "compiler/main.gem"
            gem_v__for_i_36 = gem_add(gem_v__for_i_36, gem_int(1));
#line 521 "compiler/main.gem"
    GemVal _t2289 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_326 = {0};
    GemVal _t2290[] = {gem_table_get_cached(_t2289, "value", &_ic_326), gem_v_scope_vars, gem_v_captured};
    GemVal _t2291 = (*gem_v_walk_captures_node);
            (void)(_t2291.fn(_t2291.env, _t2290, 3));
#line 522 "compiler/main.gem"
    GemVal _t2292 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_327 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2292, "bindings", &_ic_327), GEM_NIL))) {
#line 523 "compiler/main.gem"
                GemVal gem_v__for_i_35 = gem_int(0);
#line 523 "compiler/main.gem"
    GemVal _t2293 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_328 = {0};
    GemVal _t2294[] = {gem_table_get_cached(_t2293, "bindings", &_ic_328)};
                GemVal gem_v__for_limit_35 = gem_len_fn(NULL, _t2294, 1);
#line 523 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_35, gem_v__for_limit_35))) break;
#line 523 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_35;
#line 523 "compiler/main.gem"
                    gem_v__for_i_35 = gem_add(gem_v__for_i_35, gem_int(1));
#line 524 "compiler/main.gem"
    GemVal _t2295 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_329 = {0};
    GemVal _t2296[] = {gem_table_get(gem_table_get_cached(_t2295, "bindings", &_ic_329), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2297 = (*gem_v_walk_captures_node);
                    (void)(_t2297.fn(_t2297.env, _t2296, 3));
                }

            }
#line 527 "compiler/main.gem"
    GemVal _t2298 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_330 = {0};
    GemVal _t2299[] = {gem_table_get_cached(_t2298, "body", &_ic_330), gem_v_scope_vars, gem_v_captured};
    GemVal _t2300 = (*gem_v_walk_captures);
            (void)(_t2300.fn(_t2300.env, _t2299, 3));
        }

#line 529 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_el, GEM_NIL))) {
#line 530 "compiler/main.gem"
    GemVal _t2301[] = {gem_v_el, gem_v_scope_vars, gem_v_captured};
    GemVal _t2302 = (*gem_v_walk_captures);
            GemVal _t2303 = _t2302.fn(_t2302.env, _t2301, 3);
            gem_pop_frame();
            return _t2303;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2304[] = {gem_v__match_32};
    GemVal _t2306;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2304, 1), gem_string("table")))) {
        _t2306 = gem_eq(gem_type_fn(NULL, _t2304, 1), gem_string("table"));
    } else {
        GemVal _t2305[] = {gem_v__match_32, gem_string("tag")};
        _t2306 = gem_has_key_fn(NULL, _t2305, 2);
    }
    GemVal _t2307;
    if (!gem_truthy(_t2306)) {
        _t2307 = _t2306;
    } else {
        _t2307 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2309;
    if (!gem_truthy(_t2307)) {
        _t2309 = _t2307;
    } else {
        GemVal _t2308[] = {gem_v__match_32, gem_string("arms")};
        _t2309 = gem_has_key_fn(NULL, _t2308, 2);
    }
    GemVal _t2311;
    if (!gem_truthy(_t2309)) {
        _t2311 = _t2309;
    } else {
        GemVal _t2310[] = {gem_v__match_32, gem_string("after_ms")};
        _t2311 = gem_has_key_fn(NULL, _t2310, 2);
    }
    GemVal _t2313;
    if (!gem_truthy(_t2311)) {
        _t2313 = _t2311;
    } else {
        GemVal _t2312[] = {gem_v__match_32, gem_string("after_body")};
        _t2313 = gem_has_key_fn(NULL, _t2312, 2);
    }
    if (gem_truthy(_t2313)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_arms = gem_table_get(gem_v__match_32, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_ms = gem_table_get(gem_v__match_32, gem_string("after_ms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_rm_after_body = gem_table_get(gem_v__match_32, gem_string("after_body"));
#line 533 "compiler/main.gem"
        GemVal gem_v__for_i_38 = gem_int(0);
#line 533 "compiler/main.gem"
    GemVal _t2314[] = {gem_v_rm_arms};
        GemVal gem_v__for_limit_38 = gem_len_fn(NULL, _t2314, 1);
#line 533 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_38, gem_v__for_limit_38))) break;
#line 533 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_38;
#line 533 "compiler/main.gem"
            gem_v__for_i_38 = gem_add(gem_v__for_i_38, gem_int(1));
#line 534 "compiler/main.gem"
    GemVal _t2315 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_331 = {0};
    GemVal _t2316 = gem_table_get_cached(_t2315, "pattern", &_ic_331);
    static GemICacheSlot _ic_332 = {0};
    GemVal _t2317[] = {gem_table_get_cached(_t2316, "condition", &_ic_332), gem_v_scope_vars, gem_v_captured};
    GemVal _t2318 = (*gem_v_walk_captures_node);
            (void)(_t2318.fn(_t2318.env, _t2317, 3));
#line 535 "compiler/main.gem"
    GemVal _t2319 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_333 = {0};
    GemVal _t2320 = gem_table_get_cached(_t2319, "pattern", &_ic_333);
    static GemICacheSlot _ic_334 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t2320, "bindings", &_ic_334), GEM_NIL))) {
#line 536 "compiler/main.gem"
                GemVal gem_v__for_i_37 = gem_int(0);
#line 536 "compiler/main.gem"
    GemVal _t2321 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_335 = {0};
    GemVal _t2322 = gem_table_get_cached(_t2321, "pattern", &_ic_335);
    static GemICacheSlot _ic_336 = {0};
    GemVal _t2323[] = {gem_table_get_cached(_t2322, "bindings", &_ic_336)};
                GemVal gem_v__for_limit_37 = gem_len_fn(NULL, _t2323, 1);
#line 536 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    if (!gem_truthy(gem_lt(gem_v__for_i_37, gem_v__for_limit_37))) break;
#line 536 "compiler/main.gem"
                    GemVal gem_v_bi = gem_v__for_i_37;
#line 536 "compiler/main.gem"
                    gem_v__for_i_37 = gem_add(gem_v__for_i_37, gem_int(1));
#line 537 "compiler/main.gem"
    GemVal _t2324 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_337 = {0};
    GemVal _t2325 = gem_table_get_cached(_t2324, "pattern", &_ic_337);
    static GemICacheSlot _ic_338 = {0};
    GemVal _t2326[] = {gem_table_get(gem_table_get_cached(_t2325, "bindings", &_ic_338), gem_v_bi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2327 = (*gem_v_walk_captures_node);
                    (void)(_t2327.fn(_t2327.env, _t2326, 3));
                }

            }
#line 540 "compiler/main.gem"
    GemVal _t2328 = gem_table_get(gem_v_rm_arms, gem_v_i);
    static GemICacheSlot _ic_339 = {0};
    GemVal _t2329[] = {gem_table_get_cached(_t2328, "body", &_ic_339), gem_v_scope_vars, gem_v_captured};
    GemVal _t2330 = (*gem_v_walk_captures);
            (void)(_t2330.fn(_t2330.env, _t2329, 3));
        }

#line 542 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_ms, GEM_NIL))) {
#line 543 "compiler/main.gem"
    GemVal _t2331[] = {gem_v_rm_after_ms, gem_v_scope_vars, gem_v_captured};
    GemVal _t2332 = (*gem_v_walk_captures_node);
            (void)(_t2332.fn(_t2332.env, _t2331, 3));
        }
#line 545 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_rm_after_body, GEM_NIL))) {
#line 546 "compiler/main.gem"
    GemVal _t2333[] = {gem_v_rm_after_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2334 = (*gem_v_walk_captures);
            GemVal _t2335 = _t2334.fn(_t2334.env, _t2333, 3);
            gem_pop_frame();
            return _t2335;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2336[] = {gem_v__match_32};
    GemVal _t2338;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2336, 1), gem_string("table")))) {
        _t2338 = gem_eq(gem_type_fn(NULL, _t2336, 1), gem_string("table"));
    } else {
        GemVal _t2337[] = {gem_v__match_32, gem_string("tag")};
        _t2338 = gem_has_key_fn(NULL, _t2337, 2);
    }
    GemVal _t2339;
    if (!gem_truthy(_t2338)) {
        _t2339 = _t2338;
    } else {
        _t2339 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("table"));
    }
    GemVal _t2341;
    if (!gem_truthy(_t2339)) {
        _t2341 = _t2339;
    } else {
        GemVal _t2340[] = {gem_v__match_32, gem_string("entries")};
        _t2341 = gem_has_key_fn(NULL, _t2340, 2);
    }
    if (gem_truthy(_t2341)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_entries = gem_table_get(gem_v__match_32, gem_string("entries"));
        {
#line 549 "compiler/main.gem"
            GemVal gem_v__for_i_39 = gem_int(0);
#line 549 "compiler/main.gem"
    GemVal _t2342[] = {gem_v_entries};
            GemVal gem_v__for_limit_39 = gem_len_fn(NULL, _t2342, 1);
#line 549 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_39, gem_v__for_limit_39))) break;
#line 549 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_39;
#line 549 "compiler/main.gem"
                gem_v__for_i_39 = gem_add(gem_v__for_i_39, gem_int(1));
#line 550 "compiler/main.gem"
    GemVal _t2343 = gem_table_get(gem_v_entries, gem_v_i);
    static GemICacheSlot _ic_340 = {0};
    GemVal _t2344[] = {gem_table_get_cached(_t2343, "value", &_ic_340), gem_v_scope_vars, gem_v_captured};
    GemVal _t2345 = (*gem_v_walk_captures_node);
                (void)(_t2345.fn(_t2345.env, _t2344, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2346[] = {gem_v__match_32};
    GemVal _t2348;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2346, 1), gem_string("table")))) {
        _t2348 = gem_eq(gem_type_fn(NULL, _t2346, 1), gem_string("table"));
    } else {
        GemVal _t2347[] = {gem_v__match_32, gem_string("tag")};
        _t2348 = gem_has_key_fn(NULL, _t2347, 2);
    }
    GemVal _t2349;
    if (!gem_truthy(_t2348)) {
        _t2349 = _t2348;
    } else {
        _t2349 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("array"));
    }
    GemVal _t2351;
    if (!gem_truthy(_t2349)) {
        _t2351 = _t2349;
    } else {
        GemVal _t2350[] = {gem_v__match_32, gem_string("elements")};
        _t2351 = gem_has_key_fn(NULL, _t2350, 2);
    }
    if (gem_truthy(_t2351)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_elements = gem_table_get(gem_v__match_32, gem_string("elements"));
        {
#line 553 "compiler/main.gem"
            GemVal gem_v__for_i_40 = gem_int(0);
#line 553 "compiler/main.gem"
    GemVal _t2352[] = {gem_v_elements};
            GemVal gem_v__for_limit_40 = gem_len_fn(NULL, _t2352, 1);
#line 553 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_40, gem_v__for_limit_40))) break;
#line 553 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_40;
#line 553 "compiler/main.gem"
                gem_v__for_i_40 = gem_add(gem_v__for_i_40, gem_int(1));
#line 554 "compiler/main.gem"
    GemVal _t2353[] = {gem_table_get(gem_v_elements, gem_v_i), gem_v_scope_vars, gem_v_captured};
    GemVal _t2354 = (*gem_v_walk_captures_node);
                (void)(_t2354.fn(_t2354.env, _t2353, 3));
            }
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2355[] = {gem_v__match_32};
    GemVal _t2357;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2355, 1), gem_string("table")))) {
        _t2357 = gem_eq(gem_type_fn(NULL, _t2355, 1), gem_string("table"));
    } else {
        GemVal _t2356[] = {gem_v__match_32, gem_string("tag")};
        _t2357 = gem_has_key_fn(NULL, _t2356, 2);
    }
    GemVal _t2358;
    if (!gem_truthy(_t2357)) {
        _t2358 = _t2357;
    } else {
        _t2358 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2360;
    if (!gem_truthy(_t2358)) {
        _t2360 = _t2358;
    } else {
        GemVal _t2359[] = {gem_v__match_32, gem_string("stmts")};
        _t2360 = gem_has_key_fn(NULL, _t2359, 2);
    }
    if (gem_truthy(_t2360)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_32, gem_string("stmts"));
#line 557 "compiler/main.gem"
    GemVal _t2361[] = {gem_v_stmts, gem_v_scope_vars, gem_v_captured};
    GemVal _t2362 = (*gem_v_walk_captures);
        GemVal _t2363 = _t2362.fn(_t2362.env, _t2361, 3);
        gem_pop_frame();
        return _t2363;
    } else {
    GemVal _t2364[] = {gem_v__match_32};
    GemVal _t2366;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2364, 1), gem_string("table")))) {
        _t2366 = gem_eq(gem_type_fn(NULL, _t2364, 1), gem_string("table"));
    } else {
        GemVal _t2365[] = {gem_v__match_32, gem_string("tag")};
        _t2366 = gem_has_key_fn(NULL, _t2365, 2);
    }
    GemVal _t2367;
    if (!gem_truthy(_t2366)) {
        _t2367 = _t2366;
    } else {
        _t2367 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2369;
    if (!gem_truthy(_t2367)) {
        _t2369 = _t2367;
    } else {
        GemVal _t2368[] = {gem_v__match_32, gem_string("value")};
        _t2369 = gem_has_key_fn(NULL, _t2368, 2);
    }
    if (gem_truthy(_t2369)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_32, gem_string("value"));
#line 559 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 560 "compiler/main.gem"
    GemVal _t2370[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2371 = (*gem_v_walk_captures_node);
            GemVal _t2372 = _t2371.fn(_t2371.env, _t2370, 3);
            gem_pop_frame();
            return _t2372;
        } else {
            gem_pop_frame();
            return GEM_NIL;
        }
    } else {
    GemVal _t2373[] = {gem_v__match_32};
    GemVal _t2375;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2373, 1), gem_string("table")))) {
        _t2375 = gem_eq(gem_type_fn(NULL, _t2373, 1), gem_string("table"));
    } else {
        GemVal _t2374[] = {gem_v__match_32, gem_string("tag")};
        _t2375 = gem_has_key_fn(NULL, _t2374, 2);
    }
    GemVal _t2376;
    if (!gem_truthy(_t2375)) {
        _t2376 = _t2375;
    } else {
        _t2376 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("let"));
    }
    GemVal _t2378;
    if (!gem_truthy(_t2376)) {
        _t2378 = _t2376;
    } else {
        GemVal _t2377[] = {gem_v__match_32, gem_string("value")};
        _t2378 = gem_has_key_fn(NULL, _t2377, 2);
    }
    if (gem_truthy(_t2378)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_32, gem_string("value"));
#line 563 "compiler/main.gem"
    GemVal _t2379[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2380 = (*gem_v_walk_captures_node);
        GemVal _t2381 = _t2380.fn(_t2380.env, _t2379, 3);
        gem_pop_frame();
        return _t2381;
    } else {
    GemVal _t2382[] = {gem_v__match_32};
    GemVal _t2384;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2382, 1), gem_string("table")))) {
        _t2384 = gem_eq(gem_type_fn(NULL, _t2382, 1), gem_string("table"));
    } else {
        GemVal _t2383[] = {gem_v__match_32, gem_string("tag")};
        _t2384 = gem_has_key_fn(NULL, _t2383, 2);
    }
    GemVal _t2385;
    if (!gem_truthy(_t2384)) {
        _t2385 = _t2384;
    } else {
        _t2385 = gem_eq(gem_table_get(gem_v__match_32, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t2387;
    if (!gem_truthy(_t2385)) {
        _t2387 = _t2385;
    } else {
        GemVal _t2386[] = {gem_v__match_32, gem_string("value")};
        _t2387 = gem_has_key_fn(NULL, _t2386, 2);
    }
    if (gem_truthy(_t2387)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_32, gem_string("value"));
#line 565 "compiler/main.gem"
    GemVal _t2388[] = {gem_v_value, gem_v_scope_vars, gem_v_captured};
    GemVal _t2389 = (*gem_v_walk_captures_node);
        GemVal _t2390 = _t2389.fn(_t2389.env, _t2388, 3);
        gem_pop_frame();
        return _t2390;
    } else {
        {
#line 568 "compiler/main.gem"
            GemVal gem_v__for_tbl_41 = gem_v_node;
#line 568 "compiler/main.gem"
    GemVal _t2391[] = {gem_v__for_tbl_41};
            GemVal gem_v__for_len_41 = gem_len_fn(NULL, _t2391, 1);
#line 568 "compiler/main.gem"
            GemVal gem_v__for_i_41 = gem_int(0);
#line 568 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_41, gem_v__for_len_41))) break;
#line 568 "compiler/main.gem"
    GemVal _t2392[] = {gem_v__for_tbl_41, gem_v__for_i_41};
                GemVal gem_v_nk = gem_table_key_at_fn(NULL, _t2392, 2);
#line 568 "compiler/main.gem"
    GemVal _t2393[] = {gem_v__for_tbl_41, gem_v__for_i_41};
                GemVal gem_v_v = gem_table_val_at_fn(NULL, _t2393, 2);
#line 568 "compiler/main.gem"
                gem_v__for_i_41 = gem_add(gem_v__for_i_41, gem_int(1));
#line 569 "compiler/main.gem"
                if (gem_truthy(gem_neq(gem_v_nk, gem_string("tag")))) {
#line 570 "compiler/main.gem"
    GemVal _t2394[] = {gem_v_v};
                    if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2394, 1), gem_string("table")))) {
#line 571 "compiler/main.gem"
    GemVal _t2395[] = {gem_v_v};
                        GemVal gem_v_vks = gem_keys_fn(NULL, _t2395, 1);
#line 572 "compiler/main.gem"
    GemVal _t2396[] = {gem_v_vks};
    GemVal _t2398;
    if (!gem_truthy(gem_gt(gem_len_fn(NULL, _t2396, 1), gem_int(0)))) {
        _t2398 = gem_gt(gem_len_fn(NULL, _t2396, 1), gem_int(0));
    } else {
        GemVal _t2397[] = {gem_table_get(gem_v_vks, gem_int(0))};
        _t2398 = gem_eq(gem_type_fn(NULL, _t2397, 1), gem_string("int"));
    }
                        if (gem_truthy(_t2398)) {
#line 573 "compiler/main.gem"
                            GemVal gem_v_vi = gem_int(0);
#line 574 "compiler/main.gem"
                            while (1) {
                                gem_yield_check();
                                GemVal _t2399[] = {gem_v_v};
                                if (!gem_truthy(gem_lt(gem_v_vi, gem_len_fn(NULL, _t2399, 1)))) break;
#line 575 "compiler/main.gem"
    GemVal _t2400[] = {gem_table_get(gem_v_v, gem_v_vi)};
                                if (gem_truthy(gem_eq(gem_type_fn(NULL, _t2400, 1), gem_string("table")))) {
#line 576 "compiler/main.gem"
    GemVal _t2401[] = {gem_table_get(gem_v_v, gem_v_vi), gem_v_scope_vars, gem_v_captured};
    GemVal _t2402 = (*gem_v_walk_captures_node);
                                    (void)(_t2402.fn(_t2402.env, _t2401, 3));
                                }
#line 578 "compiler/main.gem"
                                gem_v_vi = gem_add(gem_v_vi, gem_int(1));
                            }
                        } else {
#line 581 "compiler/main.gem"
    GemVal _t2403[] = {gem_v_v, gem_v_scope_vars, gem_v_captured};
    GemVal _t2404 = (*gem_v_walk_captures_node);
                            (void)(_t2404.fn(_t2404.env, _t2403, 3));
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

struct _closure__anon_34 {
    intptr_t _num_captures;
    GemVal *gem_v_walk_captures_node;
};
static GemVal _anon_34(void *_env, GemVal *args, int argc) {
    struct _closure__anon_34 *_cls = (struct _closure__anon_34 *)_env;
    GemVal *gem_v_walk_captures_node = _cls->gem_v_walk_captures_node;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_captured = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_34", "compiler/main.gem", 0);
#line 590 "compiler/main.gem"
    GemVal _t2406 = gem_table_new();
    GemVal _t2407[] = {gem_v_scope_vars, _t2406};
    GemVal gem_v_sv = gem_fn__mod_codegen_set_union(NULL, _t2407, 2);
    {
#line 591 "compiler/main.gem"
        GemVal gem_v__for_i_42 = gem_int(0);
#line 591 "compiler/main.gem"
    GemVal _t2408[] = {gem_v_stmts};
        GemVal gem_v__for_limit_42 = gem_len_fn(NULL, _t2408, 1);
#line 591 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_42, gem_v__for_limit_42))) break;
#line 591 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_42;
#line 591 "compiler/main.gem"
            gem_v__for_i_42 = gem_add(gem_v__for_i_42, gem_int(1));
#line 592 "compiler/main.gem"
    GemVal _t2409[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv, gem_v_captured};
    GemVal _t2410 = (*gem_v_walk_captures_node);
            (void)(_t2410.fn(_t2410.env, _t2409, 3));
#line 593 "compiler/main.gem"
    GemVal _t2411[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("let")};
            if (gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2411, 2))) {
#line 594 "compiler/main.gem"
    GemVal _t2412 = gem_table_get(gem_v_stmts, gem_v_i);
    static GemICacheSlot _ic_341 = {0};
    GemVal _t2413[] = {gem_v_sv, gem_table_get_cached(_t2412, "name", &_ic_341)};
                (void)(gem_fn__mod_codegen_set_add(NULL, _t2413, 2));
            }
#line 596 "compiler/main.gem"
    GemVal _t2414[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_sv};
            (void)(gem_fn__mod_codegen_collect_block_lets(NULL, _t2414, 2));
        }
        gem_pop_frame();
        return GEM_NIL;
    }
}

struct _closure__anon_35 {
    intptr_t _num_captures;
    GemVal *gem_v_walk_captures;
};
static GemVal _anon_35(void *_env, GemVal *args, int argc) {
    struct _closure__anon_35 *_cls = (struct _closure__anon_35 *)_env;
    GemVal *gem_v_walk_captures = _cls->gem_v_walk_captures;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_scope_vars = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_35", "compiler/main.gem", 0);
#line 601 "compiler/main.gem"
    GemVal _t2416 = gem_table_new();
    GemVal gem_v_captured = _t2416;
#line 602 "compiler/main.gem"
    GemVal _t2417[] = {gem_v_body, gem_v_scope_vars, gem_v_captured};
    GemVal _t2418 = (*gem_v_walk_captures);
    (void)(_t2418.fn(_t2418.env, _t2417, 3));
    GemVal _t2419 = gem_v_captured;
    gem_pop_frame();
    return _t2419;
}

struct _closure__anon_36 {
    intptr_t _num_captures;
    GemVal *gem_v_has_self_tail_call;
    GemVal *gem_v_is_self_tail_call;
};
static GemVal _anon_36(void *_env, GemVal *args, int argc) {
    struct _closure__anon_36 *_cls = (struct _closure__anon_36 *)_env;
    GemVal *gem_v_has_self_tail_call = _cls->gem_v_has_self_tail_call;
    GemVal *gem_v_is_self_tail_call = _cls->gem_v_is_self_tail_call;
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_node = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_36", "compiler/main.gem", 0);
#line 633 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_node, GEM_NIL))) {
#line 634 "compiler/main.gem"
        GemVal _t2421 = gem_bool(0);
        gem_pop_frame();
        return _t2421;
    }
#line 636 "compiler/main.gem"
    GemVal _t2422[] = {gem_v_node};
    GemVal _t2424;
    if (gem_truthy(gem_neq(gem_type_fn(NULL, _t2422, 1), gem_string("table")))) {
        _t2424 = gem_neq(gem_type_fn(NULL, _t2422, 1), gem_string("table"));
    } else {
        GemVal _t2423 = gem_v_node;
        static GemICacheSlot _ic_342 = {0};
        _t2424 = gem_eq(gem_table_get_cached(_t2423, "tag", &_ic_342), GEM_NIL);
    }
    if (gem_truthy(_t2424)) {
#line 637 "compiler/main.gem"
        GemVal _t2425 = gem_bool(0);
        gem_pop_frame();
        return _t2425;
    }
#line 639 "compiler/main.gem"
    GemVal gem_v__match_43 = gem_v_node;
    GemVal _t2426[] = {gem_v__match_43};
    GemVal _t2428;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2426, 1), gem_string("table")))) {
        _t2428 = gem_eq(gem_type_fn(NULL, _t2426, 1), gem_string("table"));
    } else {
        GemVal _t2427[] = {gem_v__match_43, gem_string("tag")};
        _t2428 = gem_has_key_fn(NULL, _t2427, 2);
    }
    GemVal _t2429;
    if (!gem_truthy(_t2428)) {
        _t2429 = _t2428;
    } else {
        _t2429 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("call"));
    }
    GemVal _t2431;
    if (!gem_truthy(_t2429)) {
        _t2431 = _t2429;
    } else {
        GemVal _t2430[] = {gem_v__match_43, gem_string("func")};
        _t2431 = gem_has_key_fn(NULL, _t2430, 2);
    }
    if (gem_truthy(_t2431)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_func = gem_table_get(gem_v__match_43, gem_string("func"));
#line 641 "compiler/main.gem"
    GemVal _t2432[] = {gem_v_func, gem_string("var")};
    GemVal _t2434;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t2432, 2))) {
        _t2434 = gem_fn__mod_codegen_is_node(NULL, _t2432, 2);
    } else {
        GemVal _t2433 = gem_v_func;
        static GemICacheSlot _ic_343 = {0};
        _t2434 = gem_eq(gem_table_get_cached(_t2433, "name", &_ic_343), gem_v_fn_name);
    }
        GemVal _t2435 = _t2434;
        gem_pop_frame();
        return _t2435;
    } else {
    GemVal _t2436[] = {gem_v__match_43};
    GemVal _t2438;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2436, 1), gem_string("table")))) {
        _t2438 = gem_eq(gem_type_fn(NULL, _t2436, 1), gem_string("table"));
    } else {
        GemVal _t2437[] = {gem_v__match_43, gem_string("tag")};
        _t2438 = gem_has_key_fn(NULL, _t2437, 2);
    }
    GemVal _t2439;
    if (!gem_truthy(_t2438)) {
        _t2439 = _t2438;
    } else {
        _t2439 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("return"));
    }
    GemVal _t2441;
    if (!gem_truthy(_t2439)) {
        _t2441 = _t2439;
    } else {
        GemVal _t2440[] = {gem_v__match_43, gem_string("value")};
        _t2441 = gem_has_key_fn(NULL, _t2440, 2);
    }
    if (gem_truthy(_t2441)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_43, gem_string("value"));
#line 643 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 644 "compiler/main.gem"
    GemVal _t2442[] = {gem_v_fn_name, gem_v_value};
    GemVal _t2443 = (*gem_v_is_self_tail_call);
            GemVal _t2444 = _t2443.fn(_t2443.env, _t2442, 2);
            gem_pop_frame();
            return _t2444;
        }
#line 646 "compiler/main.gem"
        GemVal _t2445 = gem_bool(0);
        gem_pop_frame();
        return _t2445;
    } else {
    GemVal _t2446[] = {gem_v__match_43};
    GemVal _t2448;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2446, 1), gem_string("table")))) {
        _t2448 = gem_eq(gem_type_fn(NULL, _t2446, 1), gem_string("table"));
    } else {
        GemVal _t2447[] = {gem_v__match_43, gem_string("tag")};
        _t2448 = gem_has_key_fn(NULL, _t2447, 2);
    }
    GemVal _t2449;
    if (!gem_truthy(_t2448)) {
        _t2449 = _t2448;
    } else {
        _t2449 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("if"));
    }
    GemVal _t2451;
    if (!gem_truthy(_t2449)) {
        _t2451 = _t2449;
    } else {
        GemVal _t2450[] = {gem_v__match_43, gem_string("then")};
        _t2451 = gem_has_key_fn(NULL, _t2450, 2);
    }
    GemVal _t2453;
    if (!gem_truthy(_t2451)) {
        _t2453 = _t2451;
    } else {
        GemVal _t2452[] = {gem_v__match_43, gem_string("else")};
        _t2453 = gem_has_key_fn(NULL, _t2452, 2);
    }
    if (gem_truthy(_t2453)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_then_body = gem_table_get(gem_v__match_43, gem_string("then"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_43, gem_string("else"));
#line 648 "compiler/main.gem"
    GemVal _t2454[] = {gem_v_fn_name, gem_v_then_body};
    GemVal _t2455 = (*gem_v_has_self_tail_call);
        if (gem_truthy(_t2455.fn(_t2455.env, _t2454, 2))) {
#line 649 "compiler/main.gem"
            GemVal _t2456 = gem_bool(1);
            gem_pop_frame();
            return _t2456;
        }
#line 651 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 652 "compiler/main.gem"
    GemVal _t2457[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2458 = (*gem_v_has_self_tail_call);
            GemVal _t2459 = _t2458.fn(_t2458.env, _t2457, 2);
            gem_pop_frame();
            return _t2459;
        }
#line 654 "compiler/main.gem"
        GemVal _t2460 = gem_bool(0);
        gem_pop_frame();
        return _t2460;
    } else {
    GemVal _t2461[] = {gem_v__match_43};
    GemVal _t2463;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2461, 1), gem_string("table")))) {
        _t2463 = gem_eq(gem_type_fn(NULL, _t2461, 1), gem_string("table"));
    } else {
        GemVal _t2462[] = {gem_v__match_43, gem_string("tag")};
        _t2463 = gem_has_key_fn(NULL, _t2462, 2);
    }
    GemVal _t2464;
    if (!gem_truthy(_t2463)) {
        _t2464 = _t2463;
    } else {
        _t2464 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("match"));
    }
    GemVal _t2466;
    if (!gem_truthy(_t2464)) {
        _t2466 = _t2464;
    } else {
        GemVal _t2465[] = {gem_v__match_43, gem_string("whens")};
        _t2466 = gem_has_key_fn(NULL, _t2465, 2);
    }
    GemVal _t2468;
    if (!gem_truthy(_t2466)) {
        _t2468 = _t2466;
    } else {
        GemVal _t2467[] = {gem_v__match_43, gem_string("else")};
        _t2468 = gem_has_key_fn(NULL, _t2467, 2);
    }
    if (gem_truthy(_t2468)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_whens = gem_table_get(gem_v__match_43, gem_string("whens"));
#line 0 "compiler/main.gem"
        GemVal gem_v_else_body = gem_table_get(gem_v__match_43, gem_string("else"));
#line 656 "compiler/main.gem"
        GemVal gem_v__for_i_44 = gem_int(0);
#line 656 "compiler/main.gem"
    GemVal _t2469[] = {gem_v_whens};
        GemVal gem_v__for_limit_44 = gem_len_fn(NULL, _t2469, 1);
#line 656 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_44, gem_v__for_limit_44))) break;
#line 656 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_44;
#line 656 "compiler/main.gem"
            gem_v__for_i_44 = gem_add(gem_v__for_i_44, gem_int(1));
#line 657 "compiler/main.gem"
    GemVal _t2470 = gem_table_get(gem_v_whens, gem_v_i);
    static GemICacheSlot _ic_344 = {0};
    GemVal _t2471[] = {gem_v_fn_name, gem_table_get_cached(_t2470, "body", &_ic_344)};
    GemVal _t2472 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2472.fn(_t2472.env, _t2471, 2))) {
#line 658 "compiler/main.gem"
                GemVal _t2473 = gem_bool(1);
                gem_pop_frame();
                return _t2473;
            }
        }

#line 661 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_else_body, GEM_NIL))) {
#line 662 "compiler/main.gem"
    GemVal _t2474[] = {gem_v_fn_name, gem_v_else_body};
    GemVal _t2475 = (*gem_v_has_self_tail_call);
            GemVal _t2476 = _t2475.fn(_t2475.env, _t2474, 2);
            gem_pop_frame();
            return _t2476;
        }
#line 664 "compiler/main.gem"
        GemVal _t2477 = gem_bool(0);
        gem_pop_frame();
        return _t2477;
    } else {
    GemVal _t2478[] = {gem_v__match_43};
    GemVal _t2480;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2478, 1), gem_string("table")))) {
        _t2480 = gem_eq(gem_type_fn(NULL, _t2478, 1), gem_string("table"));
    } else {
        GemVal _t2479[] = {gem_v__match_43, gem_string("tag")};
        _t2480 = gem_has_key_fn(NULL, _t2479, 2);
    }
    GemVal _t2481;
    if (!gem_truthy(_t2480)) {
        _t2481 = _t2480;
    } else {
        _t2481 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t2483;
    if (!gem_truthy(_t2481)) {
        _t2483 = _t2481;
    } else {
        GemVal _t2482[] = {gem_v__match_43, gem_string("arms")};
        _t2483 = gem_has_key_fn(NULL, _t2482, 2);
    }
    GemVal _t2485;
    if (!gem_truthy(_t2483)) {
        _t2485 = _t2483;
    } else {
        GemVal _t2484[] = {gem_v__match_43, gem_string("after_body")};
        _t2485 = gem_has_key_fn(NULL, _t2484, 2);
    }
    if (gem_truthy(_t2485)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_arms = gem_table_get(gem_v__match_43, gem_string("arms"));
#line 0 "compiler/main.gem"
        GemVal gem_v_after_body = gem_table_get(gem_v__match_43, gem_string("after_body"));
#line 666 "compiler/main.gem"
        GemVal gem_v__for_i_45 = gem_int(0);
#line 666 "compiler/main.gem"
    GemVal _t2486[] = {gem_v_arms};
        GemVal gem_v__for_limit_45 = gem_len_fn(NULL, _t2486, 1);
#line 666 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_45, gem_v__for_limit_45))) break;
#line 666 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_45;
#line 666 "compiler/main.gem"
            gem_v__for_i_45 = gem_add(gem_v__for_i_45, gem_int(1));
#line 667 "compiler/main.gem"
    GemVal _t2487 = gem_table_get(gem_v_arms, gem_v_i);
    static GemICacheSlot _ic_345 = {0};
    GemVal _t2488[] = {gem_v_fn_name, gem_table_get_cached(_t2487, "body", &_ic_345)};
    GemVal _t2489 = (*gem_v_has_self_tail_call);
            if (gem_truthy(_t2489.fn(_t2489.env, _t2488, 2))) {
#line 668 "compiler/main.gem"
                GemVal _t2490 = gem_bool(1);
                gem_pop_frame();
                return _t2490;
            }
        }

#line 671 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 672 "compiler/main.gem"
    GemVal _t2491[] = {gem_v_fn_name, gem_v_after_body};
    GemVal _t2492 = (*gem_v_has_self_tail_call);
            GemVal _t2493 = _t2492.fn(_t2492.env, _t2491, 2);
            gem_pop_frame();
            return _t2493;
        }
#line 674 "compiler/main.gem"
        GemVal _t2494 = gem_bool(0);
        gem_pop_frame();
        return _t2494;
    } else {
    GemVal _t2495[] = {gem_v__match_43};
    GemVal _t2497;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2495, 1), gem_string("table")))) {
        _t2497 = gem_eq(gem_type_fn(NULL, _t2495, 1), gem_string("table"));
    } else {
        GemVal _t2496[] = {gem_v__match_43, gem_string("tag")};
        _t2497 = gem_has_key_fn(NULL, _t2496, 2);
    }
    GemVal _t2498;
    if (!gem_truthy(_t2497)) {
        _t2498 = _t2497;
    } else {
        _t2498 = gem_eq(gem_table_get(gem_v__match_43, gem_string("tag")), gem_string("block"));
    }
    GemVal _t2500;
    if (!gem_truthy(_t2498)) {
        _t2500 = _t2498;
    } else {
        GemVal _t2499[] = {gem_v__match_43, gem_string("stmts")};
        _t2500 = gem_has_key_fn(NULL, _t2499, 2);
    }
    if (gem_truthy(_t2500)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_43, gem_string("stmts"));
#line 676 "compiler/main.gem"
    GemVal _t2501[] = {gem_v_fn_name, gem_v_stmts};
    GemVal _t2502 = (*gem_v_has_self_tail_call);
        GemVal _t2503 = _t2502.fn(_t2502.env, _t2501, 2);
        gem_pop_frame();
        return _t2503;
    }
    }
    }
    }
    }
    }
#line 678 "compiler/main.gem"
    GemVal _t2504 = gem_bool(0);
    gem_pop_frame();
    return _t2504;
}

struct _closure__anon_37 {
    intptr_t _num_captures;
    GemVal *gem_v_is_self_tail_call;
};
static GemVal _anon_37(void *_env, GemVal *args, int argc) {
    struct _closure__anon_37 *_cls = (struct _closure__anon_37 *)_env;
    GemVal *gem_v_is_self_tail_call = _cls->gem_v_is_self_tail_call;
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_stmts = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_37", "compiler/main.gem", 0);
#line 682 "compiler/main.gem"
    GemVal _t2506[] = {gem_v_stmts};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2506, 1), gem_int(0)))) {
#line 683 "compiler/main.gem"
        GemVal _t2507 = gem_bool(0);
        gem_pop_frame();
        return _t2507;
    }
#line 685 "compiler/main.gem"
    GemVal _t2508[] = {gem_v_fn_name, gem_table_get(gem_v_stmts, gem_neg(gem_int(1)))};
    GemVal _t2509 = (*gem_v_is_self_tail_call);
    GemVal _t2510 = _t2509.fn(_t2509.env, _t2508, 2);
    gem_pop_frame();
    return _t2510;
}

struct _closure__anon_38 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tco_captured;
    GemVal *gem_v_tco_params;
    GemVal *gem_v_tmp;
};
static GemVal _anon_38(void *_env, GemVal *args, int argc) {
    struct _closure__anon_38 *_cls = (struct _closure__anon_38 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tco_captured = _cls->gem_v_tco_captured;
    GemVal *gem_v_tco_params = _cls->gem_v_tco_params;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_call_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_38", "compiler/main.gem", 0);
#line 689 "compiler/main.gem"
    GemVal _t2512[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t2512, 1);
#line 690 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 691 "compiler/main.gem"
    GemVal _t2513 = gem_v_call_node;
    static GemICacheSlot _ic_346 = {0};
    GemVal gem_v_args = gem_table_get_cached(_t2513, "args", &_ic_346);
#line 692 "compiler/main.gem"
    GemVal _t2514 = gem_table_new();
    GemVal gem_v_arg_temps = _t2514;
#line 693 "compiler/main.gem"
    GemVal gem_v__for_i_46 = gem_int(0);
#line 693 "compiler/main.gem"
    GemVal _t2515[] = {gem_v_args};
    GemVal gem_v__for_limit_46 = gem_len_fn(NULL, _t2515, 1);
#line 693 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_46, gem_v__for_limit_46))) break;
#line 693 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_46;
#line 693 "compiler/main.gem"
        gem_v__for_i_46 = gem_add(gem_v__for_i_46, gem_int(1));
#line 694 "compiler/main.gem"
    GemVal _t2516[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2517 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2517.fn(_t2517.env, _t2516, 1);
#line 695 "compiler/main.gem"
    GemVal _t2518 = (*gem_v_tmp);
        GemVal gem_v_t = _t2518.fn(_t2518.env, NULL, 0);
#line 696 "compiler/main.gem"
    GemVal _t2519 = gem_v_r;
    static GemICacheSlot _ic_347 = {0};
    GemVal _t2520[] = {gem_v_b, gem_table_get_cached(_t2519, "setup", &_ic_347)};
        (void)(gem_buf_push_fn(NULL, _t2520, 2));
#line 697 "compiler/main.gem"
    GemVal _t2521 = gem_v_r;
    static GemICacheSlot _ic_348 = {0};
    GemVal _t2522[] = {gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t2521, "expr", &_ic_348), gem_string(";\n")};
    GemVal _t2523[] = {gem_v_b, gem_interp(6, _t2522)};
        (void)(gem_buf_push_fn(NULL, _t2523, 2));
#line 698 "compiler/main.gem"
    GemVal _t2524[] = {gem_v_arg_temps, gem_v_t};
        (void)(gem_push_fn(NULL, _t2524, 2));
    }

#line 700 "compiler/main.gem"
    GemVal gem_v__for_i_47 = gem_int(0);
#line 700 "compiler/main.gem"
    GemVal _t2525[] = {(*gem_v_tco_params)};
    GemVal gem_v__for_limit_47 = gem_len_fn(NULL, _t2525, 1);
#line 700 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_47, gem_v__for_limit_47))) break;
#line 700 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_47;
#line 700 "compiler/main.gem"
        gem_v__for_i_47 = gem_add(gem_v__for_i_47, gem_int(1));
#line 701 "compiler/main.gem"
    GemVal _t2526[] = {gem_table_get((*gem_v_tco_params), gem_v_i)};
    GemVal _t2527 = (*gem_v_mangle);
        GemVal gem_v_mp = _t2527.fn(_t2527.env, _t2526, 1);
#line 702 "compiler/main.gem"
        GemVal gem_v_val = gem_string("GEM_NIL");
#line 703 "compiler/main.gem"
    GemVal _t2528[] = {gem_v_arg_temps};
        if (gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t2528, 1)))) {
#line 704 "compiler/main.gem"
            gem_v_val = gem_table_get(gem_v_arg_temps, gem_v_i);
        }
#line 706 "compiler/main.gem"
    GemVal _t2529[] = {(*gem_v_tco_captured), gem_table_get((*gem_v_tco_params), gem_v_i)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2529, 2))) {
#line 707 "compiler/main.gem"
    GemVal _t2530[] = {gem_v_p, gem_string("*"), gem_v_mp, gem_string(" = "), gem_v_val, gem_string(";\n")};
    GemVal _t2531[] = {gem_v_b, gem_interp(6, _t2530)};
            (void)(gem_buf_push_fn(NULL, _t2531, 2));
        } else {
#line 709 "compiler/main.gem"
    GemVal _t2532[] = {gem_v_p, gem_v_mp, gem_string(" = "), gem_v_val, gem_string(";\n")};
    GemVal _t2533[] = {gem_v_b, gem_interp(5, _t2532)};
            (void)(gem_buf_push_fn(NULL, _t2533, 2));
        }
    }

#line 712 "compiler/main.gem"
    GemVal _t2534[] = {gem_v_p, gem_string("continue;")};
    GemVal _t2535[] = {gem_v_b, gem_interp(2, _t2534)};
    (void)(gem_buf_push_fn(NULL, _t2535, 2));
#line 713 "compiler/main.gem"
    GemVal _t2536[] = {gem_v_b};
    GemVal _t2537 = gem_buf_str_fn(NULL, _t2536, 1);
    gem_pop_frame();
    return _t2537;
}

struct _closure__anon_39 {
    intptr_t _num_captures;
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
static GemVal _anon_39(void *_env, GemVal *args, int argc) {
    struct _closure__anon_39 *_cls = (struct _closure__anon_39 *)_env;
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
    gem_push_frame("_anon_39", "compiler/main.gem", 0);
#line 719 "compiler/main.gem"
    GemVal gem_v__match_48 = gem_v_node;
    GemVal _t2539[] = {gem_v__match_48};
    GemVal _t2541;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2539, 1), gem_string("table")))) {
        _t2541 = gem_eq(gem_type_fn(NULL, _t2539, 1), gem_string("table"));
    } else {
        GemVal _t2540[] = {gem_v__match_48, gem_string("tag")};
        _t2541 = gem_has_key_fn(NULL, _t2540, 2);
    }
    GemVal _t2542;
    if (!gem_truthy(_t2541)) {
        _t2542 = _t2541;
    } else {
        _t2542 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("int"));
    }
    GemVal _t2544;
    if (!gem_truthy(_t2542)) {
        _t2544 = _t2542;
    } else {
        GemVal _t2543[] = {gem_v__match_48, gem_string("value")};
        _t2544 = gem_has_key_fn(NULL, _t2543, 2);
    }
    if (gem_truthy(_t2544)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 721 "compiler/main.gem"
    GemVal _t2545 = gem_table_new();
    GemVal _t2546[] = {gem_string("gem_int("), gem_v_value, gem_string(")")};
    gem_table_set(_t2545, gem_string("expr"), gem_interp(3, _t2546));
    gem_table_set(_t2545, gem_string("setup"), gem_string(""));
        GemVal _t2547 = _t2545;
        gem_pop_frame();
        return _t2547;
    } else {
    GemVal _t2548[] = {gem_v__match_48};
    GemVal _t2550;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2548, 1), gem_string("table")))) {
        _t2550 = gem_eq(gem_type_fn(NULL, _t2548, 1), gem_string("table"));
    } else {
        GemVal _t2549[] = {gem_v__match_48, gem_string("tag")};
        _t2550 = gem_has_key_fn(NULL, _t2549, 2);
    }
    GemVal _t2551;
    if (!gem_truthy(_t2550)) {
        _t2551 = _t2550;
    } else {
        _t2551 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("float"));
    }
    GemVal _t2553;
    if (!gem_truthy(_t2551)) {
        _t2553 = _t2551;
    } else {
        GemVal _t2552[] = {gem_v__match_48, gem_string("value")};
        _t2553 = gem_has_key_fn(NULL, _t2552, 2);
    }
    if (gem_truthy(_t2553)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 723 "compiler/main.gem"
    GemVal _t2554 = gem_table_new();
    GemVal _t2555[] = {gem_v_value};
    GemVal _t2556[] = {gem_string("gem_float("), gem_fn__mod_codegen_format_float(NULL, _t2555, 1), gem_string(")")};
    gem_table_set(_t2554, gem_string("expr"), gem_interp(3, _t2556));
    gem_table_set(_t2554, gem_string("setup"), gem_string(""));
        GemVal _t2557 = _t2554;
        gem_pop_frame();
        return _t2557;
    } else {
    GemVal _t2558[] = {gem_v__match_48};
    GemVal _t2560;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2558, 1), gem_string("table")))) {
        _t2560 = gem_eq(gem_type_fn(NULL, _t2558, 1), gem_string("table"));
    } else {
        GemVal _t2559[] = {gem_v__match_48, gem_string("tag")};
        _t2560 = gem_has_key_fn(NULL, _t2559, 2);
    }
    GemVal _t2561;
    if (!gem_truthy(_t2560)) {
        _t2561 = _t2560;
    } else {
        _t2561 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("string"));
    }
    GemVal _t2563;
    if (!gem_truthy(_t2561)) {
        _t2563 = _t2561;
    } else {
        GemVal _t2562[] = {gem_v__match_48, gem_string("value")};
        _t2563 = gem_has_key_fn(NULL, _t2562, 2);
    }
    if (gem_truthy(_t2563)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 725 "compiler/main.gem"
    GemVal _t2564[] = {gem_v_value};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2564, 1);
#line 726 "compiler/main.gem"
    GemVal _t2565 = gem_table_new();
    GemVal _t2566[] = {gem_string("gem_string(\""), gem_v_escaped, gem_string("\")")};
    gem_table_set(_t2565, gem_string("expr"), gem_interp(3, _t2566));
    gem_table_set(_t2565, gem_string("setup"), gem_string(""));
        GemVal _t2567 = _t2565;
        gem_pop_frame();
        return _t2567;
    } else {
    GemVal _t2568[] = {gem_v__match_48};
    GemVal _t2570;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2568, 1), gem_string("table")))) {
        _t2570 = gem_eq(gem_type_fn(NULL, _t2568, 1), gem_string("table"));
    } else {
        GemVal _t2569[] = {gem_v__match_48, gem_string("tag")};
        _t2570 = gem_has_key_fn(NULL, _t2569, 2);
    }
    GemVal _t2571;
    if (!gem_truthy(_t2570)) {
        _t2571 = _t2570;
    } else {
        _t2571 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t2573;
    if (!gem_truthy(_t2571)) {
        _t2573 = _t2571;
    } else {
        GemVal _t2572[] = {gem_v__match_48, gem_string("parts")};
        _t2573 = gem_has_key_fn(NULL, _t2572, 2);
    }
    if (gem_truthy(_t2573)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_parts = gem_table_get(gem_v__match_48, gem_string("parts"));
#line 728 "compiler/main.gem"
    GemVal _t2574[] = {gem_v_parts};
        if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2574, 1), gem_int(0)))) {
#line 729 "compiler/main.gem"
    GemVal _t2575 = gem_table_new();
    gem_table_set(_t2575, gem_string("expr"), gem_string("gem_string(\"\")"));
    gem_table_set(_t2575, gem_string("setup"), gem_string(""));
            GemVal _t2576 = _t2575;
            gem_pop_frame();
            return _t2576;
        }
#line 731 "compiler/main.gem"
        GemVal gem_v_setup = gem_string("");
#line 732 "compiler/main.gem"
    GemVal _t2577 = gem_table_new();
        GemVal gem_v_compiled = _t2577;
#line 733 "compiler/main.gem"
        GemVal gem_v__for_i_49 = gem_int(0);
#line 733 "compiler/main.gem"
    GemVal _t2578[] = {gem_v_parts};
        GemVal gem_v__for_limit_49 = gem_len_fn(NULL, _t2578, 1);
#line 733 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_49, gem_v__for_limit_49))) break;
#line 733 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_49;
#line 733 "compiler/main.gem"
            gem_v__for_i_49 = gem_add(gem_v__for_i_49, gem_int(1));
#line 734 "compiler/main.gem"
            GemVal gem_v_part = gem_table_get(gem_v_parts, gem_v_i);
#line 735 "compiler/main.gem"
    GemVal _t2579[] = {gem_v_part};
    GemVal _t2580 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t2580.fn(_t2580.env, _t2579, 1);
#line 736 "compiler/main.gem"
    GemVal _t2581 = gem_v_r;
    static GemICacheSlot _ic_349 = {0};
            gem_v_setup = gem_add(gem_v_setup, gem_table_get_cached(_t2581, "setup", &_ic_349));
#line 737 "compiler/main.gem"
    GemVal _t2582 = gem_v_r;
    static GemICacheSlot _ic_350 = {0};
    GemVal _t2583[] = {gem_v_compiled, gem_table_get_cached(_t2582, "expr", &_ic_350)};
            (void)(gem_push_fn(NULL, _t2583, 2));
        }

#line 739 "compiler/main.gem"
    GemVal _t2584 = (*gem_v_tmp);
        GemVal gem_v_arr_tmp = _t2584.fn(_t2584.env, NULL, 0);
#line 740 "compiler/main.gem"
        GemVal gem_v_items = gem_table_get(gem_v_compiled, gem_int(0));
#line 741 "compiler/main.gem"
        GemVal gem_v__for_i_50 = gem_int(1);
#line 741 "compiler/main.gem"
    GemVal _t2585[] = {gem_v_compiled};
        GemVal gem_v__for_limit_50 = gem_len_fn(NULL, _t2585, 1);
#line 741 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_50, gem_v__for_limit_50))) break;
#line 741 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_50;
#line 741 "compiler/main.gem"
            gem_v__for_i_50 = gem_add(gem_v__for_i_50, gem_int(1));
#line 742 "compiler/main.gem"
            gem_v_items = gem_add(gem_add(gem_v_items, gem_string(", ")), gem_table_get(gem_v_compiled, gem_v_i));
        }

#line 744 "compiler/main.gem"
    GemVal _t2586[] = {gem_string("    GemVal "), gem_v_arr_tmp, gem_string("[] = {"), gem_v_items, gem_string("};\n")};
        gem_v_setup = gem_add(gem_v_setup, gem_interp(5, _t2586));
#line 745 "compiler/main.gem"
    GemVal _t2587 = gem_table_new();
    GemVal _t2588[] = {gem_v_compiled};
    GemVal _t2589[] = {gem_string("gem_interp("), gem_len_fn(NULL, _t2588, 1), gem_string(", "), gem_v_arr_tmp, gem_string(")")};
    gem_table_set(_t2587, gem_string("expr"), gem_interp(5, _t2589));
    gem_table_set(_t2587, gem_string("setup"), gem_v_setup);
        GemVal _t2590 = _t2587;
        gem_pop_frame();
        return _t2590;
    } else {
    GemVal _t2591[] = {gem_v__match_48};
    GemVal _t2593;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2591, 1), gem_string("table")))) {
        _t2593 = gem_eq(gem_type_fn(NULL, _t2591, 1), gem_string("table"));
    } else {
        GemVal _t2592[] = {gem_v__match_48, gem_string("tag")};
        _t2593 = gem_has_key_fn(NULL, _t2592, 2);
    }
    GemVal _t2594;
    if (!gem_truthy(_t2593)) {
        _t2594 = _t2593;
    } else {
        _t2594 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("bool"));
    }
    GemVal _t2596;
    if (!gem_truthy(_t2594)) {
        _t2596 = _t2594;
    } else {
        GemVal _t2595[] = {gem_v__match_48, gem_string("value")};
        _t2596 = gem_has_key_fn(NULL, _t2595, 2);
    }
    if (gem_truthy(_t2596)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_48, gem_string("value"));
#line 747 "compiler/main.gem"
        if (gem_truthy(gem_v_value)) {
#line 748 "compiler/main.gem"
    GemVal _t2597 = gem_table_new();
    gem_table_set(_t2597, gem_string("expr"), gem_string("gem_bool(1)"));
    gem_table_set(_t2597, gem_string("setup"), gem_string(""));
            GemVal _t2598 = _t2597;
            gem_pop_frame();
            return _t2598;
        }
#line 750 "compiler/main.gem"
    GemVal _t2599 = gem_table_new();
    gem_table_set(_t2599, gem_string("expr"), gem_string("gem_bool(0)"));
    gem_table_set(_t2599, gem_string("setup"), gem_string(""));
        GemVal _t2600 = _t2599;
        gem_pop_frame();
        return _t2600;
    } else {
    GemVal _t2601[] = {gem_v__match_48};
    GemVal _t2603;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2601, 1), gem_string("table")))) {
        _t2603 = gem_eq(gem_type_fn(NULL, _t2601, 1), gem_string("table"));
    } else {
        GemVal _t2602[] = {gem_v__match_48, gem_string("tag")};
        _t2603 = gem_has_key_fn(NULL, _t2602, 2);
    }
    GemVal _t2604;
    if (!gem_truthy(_t2603)) {
        _t2604 = _t2603;
    } else {
        _t2604 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("nil"));
    }
    if (gem_truthy(_t2604)) {
#line 752 "compiler/main.gem"
    GemVal _t2605 = gem_table_new();
    gem_table_set(_t2605, gem_string("expr"), gem_string("GEM_NIL"));
    gem_table_set(_t2605, gem_string("setup"), gem_string(""));
        GemVal _t2606 = _t2605;
        gem_pop_frame();
        return _t2606;
    } else {
    GemVal _t2607[] = {gem_v__match_48};
    GemVal _t2609;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2607, 1), gem_string("table")))) {
        _t2609 = gem_eq(gem_type_fn(NULL, _t2607, 1), gem_string("table"));
    } else {
        GemVal _t2608[] = {gem_v__match_48, gem_string("tag")};
        _t2609 = gem_has_key_fn(NULL, _t2608, 2);
    }
    GemVal _t2610;
    if (!gem_truthy(_t2609)) {
        _t2610 = _t2609;
    } else {
        _t2610 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("var"));
    }
    GemVal _t2612;
    if (!gem_truthy(_t2610)) {
        _t2612 = _t2610;
    } else {
        GemVal _t2611[] = {gem_v__match_48, gem_string("name")};
        _t2612 = gem_has_key_fn(NULL, _t2611, 2);
    }
    if (gem_truthy(_t2612)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_48, gem_string("name"));
#line 754 "compiler/main.gem"
    GemVal _t2613[] = {(*gem_v_local_names), gem_v_name};
        if (gem_truthy(gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2613, 2)))) {
#line 755 "compiler/main.gem"
    GemVal _t2614[] = {(*gem_v_builtin_fns), gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t2614, 2))) {
#line 756 "compiler/main.gem"
    GemVal _t2615 = gem_table_new();
    GemVal _t2616[] = {gem_string("gem_make_fn("), gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string(", NULL)")};
    gem_table_set(_t2615, gem_string("expr"), gem_interp(3, _t2616));
    gem_table_set(_t2615, gem_string("setup"), gem_string(""));
                GemVal _t2617 = _t2615;
                gem_pop_frame();
                return _t2617;
            } else {
#line 757 "compiler/main.gem"
    GemVal _t2618[] = {(*gem_v_defined_fns), gem_v_name};
                if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2618, 2))) {
#line 758 "compiler/main.gem"
    GemVal _t2619 = gem_table_new();
    GemVal _t2620[] = {gem_string("gem_make_fn(gem_fn_"), gem_v_name, gem_string(", NULL)")};
    gem_table_set(_t2619, gem_string("expr"), gem_interp(3, _t2620));
    gem_table_set(_t2619, gem_string("setup"), gem_string(""));
                    GemVal _t2621 = _t2619;
                    gem_pop_frame();
                    return _t2621;
                }
            }
        }
#line 761 "compiler/main.gem"
    GemVal _t2622[] = {gem_v_name};
    GemVal _t2623 = (*gem_v_mangle);
        GemVal gem_v_mname = _t2623.fn(_t2623.env, _t2622, 1);
#line 762 "compiler/main.gem"
    GemVal _t2624[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t2627;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2624, 2))) {
        _t2627 = gem_fn__mod_codegen_set_contains(NULL, _t2624, 2);
    } else {
        GemVal _t2626;
        if (!gem_truthy((*gem_v_in_top_level))) {
                _t2626 = (*gem_v_in_top_level);
        } else {
                GemVal _t2625[] = {(*gem_v_top_level_boxed), gem_v_name};
                _t2626 = gem_fn__mod_codegen_set_contains(NULL, _t2625, 2);
        }
        _t2627 = _t2626;
    }
        if (gem_truthy(_t2627)) {
#line 763 "compiler/main.gem"
    GemVal _t2628 = gem_table_new();
    GemVal _t2629[] = {gem_string("(*"), gem_v_mname, gem_string(")")};
    gem_table_set(_t2628, gem_string("expr"), gem_interp(3, _t2629));
    gem_table_set(_t2628, gem_string("setup"), gem_string(""));
            GemVal _t2630 = _t2628;
            gem_pop_frame();
            return _t2630;
        }
#line 765 "compiler/main.gem"
    GemVal _t2631 = gem_table_new();
    gem_table_set(_t2631, gem_string("expr"), gem_v_mname);
    gem_table_set(_t2631, gem_string("setup"), gem_string(""));
        GemVal _t2632 = _t2631;
        gem_pop_frame();
        return _t2632;
    } else {
    GemVal _t2633[] = {gem_v__match_48};
    GemVal _t2635;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2633, 1), gem_string("table")))) {
        _t2635 = gem_eq(gem_type_fn(NULL, _t2633, 1), gem_string("table"));
    } else {
        GemVal _t2634[] = {gem_v__match_48, gem_string("tag")};
        _t2635 = gem_has_key_fn(NULL, _t2634, 2);
    }
    GemVal _t2636;
    if (!gem_truthy(_t2635)) {
        _t2636 = _t2635;
    } else {
        _t2636 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t2636)) {
#line 767 "compiler/main.gem"
    GemVal _t2637[] = {gem_v_node};
    GemVal _t2638 = (*gem_v_compile_call);
        GemVal _t2639 = _t2638.fn(_t2638.env, _t2637, 1);
        gem_pop_frame();
        return _t2639;
    } else {
    GemVal _t2640[] = {gem_v__match_48};
    GemVal _t2642;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2640, 1), gem_string("table")))) {
        _t2642 = gem_eq(gem_type_fn(NULL, _t2640, 1), gem_string("table"));
    } else {
        GemVal _t2641[] = {gem_v__match_48, gem_string("tag")};
        _t2642 = gem_has_key_fn(NULL, _t2641, 2);
    }
    GemVal _t2643;
    if (!gem_truthy(_t2642)) {
        _t2643 = _t2642;
    } else {
        _t2643 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("binop"));
    }
    if (gem_truthy(_t2643)) {
#line 769 "compiler/main.gem"
    GemVal _t2644[] = {gem_v_node};
    GemVal _t2645 = (*gem_v_compile_binop);
        GemVal _t2646 = _t2645.fn(_t2645.env, _t2644, 1);
        gem_pop_frame();
        return _t2646;
    } else {
    GemVal _t2647[] = {gem_v__match_48};
    GemVal _t2649;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2647, 1), gem_string("table")))) {
        _t2649 = gem_eq(gem_type_fn(NULL, _t2647, 1), gem_string("table"));
    } else {
        GemVal _t2648[] = {gem_v__match_48, gem_string("tag")};
        _t2649 = gem_has_key_fn(NULL, _t2648, 2);
    }
    GemVal _t2650;
    if (!gem_truthy(_t2649)) {
        _t2650 = _t2649;
    } else {
        _t2650 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t2652;
    if (!gem_truthy(_t2650)) {
        _t2652 = _t2650;
    } else {
        GemVal _t2651[] = {gem_v__match_48, gem_string("expr")};
        _t2652 = gem_has_key_fn(NULL, _t2651, 2);
    }
    GemVal _t2654;
    if (!gem_truthy(_t2652)) {
        _t2654 = _t2652;
    } else {
        GemVal _t2653[] = {gem_v__match_48, gem_string("op")};
        _t2654 = gem_has_key_fn(NULL, _t2653, 2);
    }
    if (gem_truthy(_t2654)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_uexpr = gem_table_get(gem_v__match_48, gem_string("expr"));
#line 0 "compiler/main.gem"
        GemVal gem_v_op = gem_table_get(gem_v__match_48, gem_string("op"));
#line 771 "compiler/main.gem"
    GemVal _t2655[] = {gem_v_uexpr};
    GemVal _t2656 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2656.fn(_t2656.env, _t2655, 1);
#line 772 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 773 "compiler/main.gem"
    GemVal _t2657 = gem_table_new();
    GemVal _t2658 = gem_v_r;
    static GemICacheSlot _ic_351 = {0};
    GemVal _t2659[] = {gem_string("gem_neg("), gem_table_get_cached(_t2658, "expr", &_ic_351), gem_string(")")};
    gem_table_set(_t2657, gem_string("expr"), gem_interp(3, _t2659));
    GemVal _t2660 = gem_v_r;
    static GemICacheSlot _ic_352 = {0};
    gem_table_set(_t2657, gem_string("setup"), gem_table_get_cached(_t2660, "setup", &_ic_352));
            GemVal _t2661 = _t2657;
            gem_pop_frame();
            return _t2661;
        } else {
#line 774 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("not")))) {
#line 775 "compiler/main.gem"
    GemVal _t2662 = gem_table_new();
    GemVal _t2663 = gem_v_r;
    static GemICacheSlot _ic_353 = {0};
    GemVal _t2664[] = {gem_string("gem_not("), gem_table_get_cached(_t2663, "expr", &_ic_353), gem_string(")")};
    gem_table_set(_t2662, gem_string("expr"), gem_interp(3, _t2664));
    GemVal _t2665 = gem_v_r;
    static GemICacheSlot _ic_354 = {0};
    gem_table_set(_t2662, gem_string("setup"), gem_table_get_cached(_t2665, "setup", &_ic_354));
                GemVal _t2666 = _t2662;
                gem_pop_frame();
                return _t2666;
            }
        }
#line 777 "compiler/main.gem"
    GemVal _t2667[] = {gem_string("unknown unary operator: "), gem_v_op};
    GemVal _t2668[] = {gem_interp(2, _t2667)};
        (void)(gem_error_at_fn("compiler/main.gem", 777, _t2668, 1));
    } else {
    GemVal _t2669[] = {gem_v__match_48};
    GemVal _t2671;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2669, 1), gem_string("table")))) {
        _t2671 = gem_eq(gem_type_fn(NULL, _t2669, 1), gem_string("table"));
    } else {
        GemVal _t2670[] = {gem_v__match_48, gem_string("tag")};
        _t2671 = gem_has_key_fn(NULL, _t2670, 2);
    }
    GemVal _t2672;
    if (!gem_truthy(_t2671)) {
        _t2672 = _t2671;
    } else {
        _t2672 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("anon_fn"));
    }
    if (gem_truthy(_t2672)) {
#line 779 "compiler/main.gem"
    GemVal _t2673[] = {gem_v_node};
    GemVal _t2674 = (*gem_v_compile_anon_fn);
        GemVal _t2675 = _t2674.fn(_t2674.env, _t2673, 1);
        gem_pop_frame();
        return _t2675;
    } else {
    GemVal _t2676[] = {gem_v__match_48};
    GemVal _t2678;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2676, 1), gem_string("table")))) {
        _t2678 = gem_eq(gem_type_fn(NULL, _t2676, 1), gem_string("table"));
    } else {
        GemVal _t2677[] = {gem_v__match_48, gem_string("tag")};
        _t2678 = gem_has_key_fn(NULL, _t2677, 2);
    }
    GemVal _t2679;
    if (!gem_truthy(_t2678)) {
        _t2679 = _t2678;
    } else {
        _t2679 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("table"));
    }
    if (gem_truthy(_t2679)) {
#line 781 "compiler/main.gem"
    GemVal _t2680[] = {gem_v_node};
    GemVal _t2681 = (*gem_v_compile_table);
        GemVal _t2682 = _t2681.fn(_t2681.env, _t2680, 1);
        gem_pop_frame();
        return _t2682;
    } else {
    GemVal _t2683[] = {gem_v__match_48};
    GemVal _t2685;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2683, 1), gem_string("table")))) {
        _t2685 = gem_eq(gem_type_fn(NULL, _t2683, 1), gem_string("table"));
    } else {
        GemVal _t2684[] = {gem_v__match_48, gem_string("tag")};
        _t2685 = gem_has_key_fn(NULL, _t2684, 2);
    }
    GemVal _t2686;
    if (!gem_truthy(_t2685)) {
        _t2686 = _t2685;
    } else {
        _t2686 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("array"));
    }
    if (gem_truthy(_t2686)) {
#line 783 "compiler/main.gem"
    GemVal _t2687[] = {gem_v_node};
    GemVal _t2688 = (*gem_v_compile_array);
        GemVal _t2689 = _t2688.fn(_t2688.env, _t2687, 1);
        gem_pop_frame();
        return _t2689;
    } else {
    GemVal _t2690[] = {gem_v__match_48};
    GemVal _t2692;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2690, 1), gem_string("table")))) {
        _t2692 = gem_eq(gem_type_fn(NULL, _t2690, 1), gem_string("table"));
    } else {
        GemVal _t2691[] = {gem_v__match_48, gem_string("tag")};
        _t2692 = gem_has_key_fn(NULL, _t2691, 2);
    }
    GemVal _t2693;
    if (!gem_truthy(_t2692)) {
        _t2693 = _t2692;
    } else {
        _t2693 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t2695;
    if (!gem_truthy(_t2693)) {
        _t2695 = _t2693;
    } else {
        GemVal _t2694[] = {gem_v__match_48, gem_string("object")};
        _t2695 = gem_has_key_fn(NULL, _t2694, 2);
    }
    GemVal _t2697;
    if (!gem_truthy(_t2695)) {
        _t2697 = _t2695;
    } else {
        GemVal _t2696[] = {gem_v__match_48, gem_string("field")};
        _t2697 = gem_has_key_fn(NULL, _t2696, 2);
    }
    if (gem_truthy(_t2697)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_48, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_48, gem_string("field"));
#line 785 "compiler/main.gem"
    GemVal _t2698[] = {gem_v_object};
    GemVal _t2699 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2699.fn(_t2699.env, _t2698, 1);
#line 786 "compiler/main.gem"
    GemVal _t2700 = (*gem_v_tmp);
        GemVal gem_v_t = _t2700.fn(_t2700.env, NULL, 0);
#line 787 "compiler/main.gem"
        *gem_v_ic_counter = gem_add((*gem_v_ic_counter), gem_int(1));
#line 788 "compiler/main.gem"
        GemVal gem_v_ic = (*gem_v_ic_counter);
#line 789 "compiler/main.gem"
    GemVal _t2701[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2701, 1);
#line 790 "compiler/main.gem"
    GemVal _t2702 = gem_v_r;
    static GemICacheSlot _ic_355 = {0};
    GemVal _t2703 = gem_v_r;
    static GemICacheSlot _ic_356 = {0};
    GemVal _t2704[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t2703, "expr", &_ic_356), gem_string(";\n    static GemICacheSlot _ic_"), gem_v_ic, gem_string(" = "), (*gem_v_ic_init), gem_string(";\n")};
        GemVal gem_v_setup = gem_add(gem_table_get_cached(_t2702, "setup", &_ic_355), gem_interp(9, _t2704));
#line 791 "compiler/main.gem"
    GemVal _t2705 = gem_table_new();
    GemVal _t2706[] = {gem_string("gem_table_get_cached("), gem_v_t, gem_string(", \""), gem_v_escaped, gem_string("\", &_ic_"), gem_v_ic, gem_string(")")};
    gem_table_set(_t2705, gem_string("expr"), gem_interp(7, _t2706));
    gem_table_set(_t2705, gem_string("setup"), gem_v_setup);
        GemVal _t2707 = _t2705;
        gem_pop_frame();
        return _t2707;
    } else {
    GemVal _t2708[] = {gem_v__match_48};
    GemVal _t2710;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t2708, 1), gem_string("table")))) {
        _t2710 = gem_eq(gem_type_fn(NULL, _t2708, 1), gem_string("table"));
    } else {
        GemVal _t2709[] = {gem_v__match_48, gem_string("tag")};
        _t2710 = gem_has_key_fn(NULL, _t2709, 2);
    }
    GemVal _t2711;
    if (!gem_truthy(_t2710)) {
        _t2711 = _t2710;
    } else {
        _t2711 = gem_eq(gem_table_get(gem_v__match_48, gem_string("tag")), gem_string("index"));
    }
    GemVal _t2713;
    if (!gem_truthy(_t2711)) {
        _t2713 = _t2711;
    } else {
        GemVal _t2712[] = {gem_v__match_48, gem_string("object")};
        _t2713 = gem_has_key_fn(NULL, _t2712, 2);
    }
    GemVal _t2715;
    if (!gem_truthy(_t2713)) {
        _t2715 = _t2713;
    } else {
        GemVal _t2714[] = {gem_v__match_48, gem_string("key")};
        _t2715 = gem_has_key_fn(NULL, _t2714, 2);
    }
    if (gem_truthy(_t2715)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_48, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_48, gem_string("key"));
#line 793 "compiler/main.gem"
    GemVal _t2716[] = {gem_v_object};
    GemVal _t2717 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2717.fn(_t2717.env, _t2716, 1);
#line 794 "compiler/main.gem"
    GemVal _t2718[] = {gem_v_key};
    GemVal _t2719 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t2719.fn(_t2719.env, _t2718, 1);
#line 795 "compiler/main.gem"
    GemVal _t2720 = gem_table_new();
    GemVal _t2721 = gem_v_obj_r;
    static GemICacheSlot _ic_357 = {0};
    GemVal _t2722 = gem_v_key_r;
    static GemICacheSlot _ic_358 = {0};
    GemVal _t2723[] = {gem_string("gem_table_get("), gem_table_get_cached(_t2721, "expr", &_ic_357), gem_string(", "), gem_table_get_cached(_t2722, "expr", &_ic_358), gem_string(")")};
    gem_table_set(_t2720, gem_string("expr"), gem_interp(5, _t2723));
    GemVal _t2724 = gem_v_obj_r;
    static GemICacheSlot _ic_359 = {0};
    GemVal _t2725 = gem_v_key_r;
    static GemICacheSlot _ic_360 = {0};
    gem_table_set(_t2720, gem_string("setup"), gem_add(gem_table_get_cached(_t2724, "setup", &_ic_359), gem_table_get_cached(_t2725, "setup", &_ic_360)));
        GemVal _t2726 = _t2720;
        gem_pop_frame();
        return _t2726;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 798 "compiler/main.gem"
    GemVal _t2727 = gem_v_node;
    static GemICacheSlot _ic_361 = {0};
    GemVal _t2728[] = {gem_string("unknown expression node: "), gem_table_get_cached(_t2727, "tag", &_ic_361)};
    GemVal _t2729[] = {gem_interp(2, _t2728)};
    GemVal _t2730 = gem_error_at_fn("compiler/main.gem", 798, _t2729, 1);
    gem_pop_frame();
    return _t2730;
}

struct _closure__anon_40 {
    intptr_t _num_captures;
};
static GemVal _anon_40(void *_env, GemVal *args, int argc) {
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_40", "compiler/main.gem", 0);
#line 804 "compiler/main.gem"
    GemVal _t2732[] = {gem_v_s};
    GemVal gem_v_slen = gem_len_fn(NULL, _t2732, 1);
#line 805 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_slen, gem_int(0)))) {
#line 806 "compiler/main.gem"
        GemVal _t2733 = gem_bool(0);
        gem_pop_frame();
        return _t2733;
    }
#line 808 "compiler/main.gem"
    GemVal gem_v_start = gem_int(0);
#line 809 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_table_get(gem_v_s, gem_int(0)), gem_string("-")))) {
#line 810 "compiler/main.gem"
        gem_v_start = gem_int(1);
#line 811 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_slen, gem_int(1)))) {
#line 812 "compiler/main.gem"
            GemVal _t2734 = gem_bool(0);
            gem_pop_frame();
            return _t2734;
        }
    }
#line 815 "compiler/main.gem"
    GemVal gem_v_i = gem_v_start;
#line 816 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_slen))) break;
#line 817 "compiler/main.gem"
    GemVal _t2735[] = {gem_v_s, gem_v_i};
        GemVal gem_v_c = gem_ord_fn(NULL, _t2735, 2);
#line 818 "compiler/main.gem"
    GemVal _t2736;
    if (gem_truthy(gem_lt(gem_v_c, gem_int(48)))) {
        _t2736 = gem_lt(gem_v_c, gem_int(48));
    } else {
        _t2736 = gem_gt(gem_v_c, gem_int(57));
    }
        if (gem_truthy(_t2736)) {
#line 819 "compiler/main.gem"
            GemVal _t2737 = gem_bool(0);
            gem_pop_frame();
            return _t2737;
        }
#line 821 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
    GemVal _t2738 = gem_bool(1);
    gem_pop_frame();
    return _t2738;
}

struct _closure__anon_41 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_is_int_key;
    GemVal *gem_v_tmp;
};
static GemVal _anon_41(void *_env, GemVal *args, int argc) {
    struct _closure__anon_41 *_cls = (struct _closure__anon_41 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_is_int_key = _cls->gem_v_is_int_key;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_41", "compiler/main.gem", 0);
#line 827 "compiler/main.gem"
    GemVal _t2739 = gem_v_node;
    static GemICacheSlot _ic_362 = {0};
    GemVal gem_v_entries = gem_table_get_cached(_t2739, "entries", &_ic_362);
#line 828 "compiler/main.gem"
    GemVal _t2740 = (*gem_v_tmp);
    GemVal gem_v_t = _t2740.fn(_t2740.env, NULL, 0);
#line 829 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 830 "compiler/main.gem"
    GemVal _t2741[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = gem_table_new();\n")};
    GemVal _t2742[] = {gem_v_b, gem_interp(3, _t2741)};
    (void)(gem_buf_push_fn(NULL, _t2742, 2));
#line 831 "compiler/main.gem"
    GemVal gem_v__for_items_51 = gem_v_entries;
#line 831 "compiler/main.gem"
    GemVal gem_v__for_i_51 = gem_int(0);
#line 831 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2743[] = {gem_v__for_items_51};
        if (!gem_truthy(gem_lt(gem_v__for_i_51, gem_len_fn(NULL, _t2743, 1)))) break;
#line 831 "compiler/main.gem"
        GemVal gem_v_entry = gem_table_get(gem_v__for_items_51, gem_v__for_i_51);
#line 831 "compiler/main.gem"
        gem_v__for_i_51 = gem_add(gem_v__for_i_51, gem_int(1));
#line 832 "compiler/main.gem"
    GemVal _t2744 = gem_v_entry;
    static GemICacheSlot _ic_363 = {0};
    GemVal _t2745[] = {gem_table_get_cached(_t2744, "value", &_ic_363)};
    GemVal _t2746 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t2746.fn(_t2746.env, _t2745, 1);
#line 833 "compiler/main.gem"
    GemVal _t2747 = gem_v_val_r;
    static GemICacheSlot _ic_364 = {0};
    GemVal _t2748[] = {gem_v_b, gem_table_get_cached(_t2747, "setup", &_ic_364)};
        (void)(gem_buf_push_fn(NULL, _t2748, 2));
#line 834 "compiler/main.gem"
    GemVal _t2749 = gem_v_entry;
    static GemICacheSlot _ic_365 = {0};
    GemVal _t2750[] = {gem_table_get_cached(_t2749, "key", &_ic_365)};
    GemVal _t2751 = (*gem_v_is_int_key);
        if (gem_truthy(_t2751.fn(_t2751.env, _t2750, 1))) {
#line 835 "compiler/main.gem"
    GemVal _t2752 = gem_v_entry;
    static GemICacheSlot _ic_366 = {0};
    GemVal _t2753 = gem_v_val_r;
    static GemICacheSlot _ic_367 = {0};
    GemVal _t2754[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_int("), gem_table_get_cached(_t2752, "key", &_ic_366), gem_string("), "), gem_table_get_cached(_t2753, "expr", &_ic_367), gem_string(");\n")};
    GemVal _t2755[] = {gem_v_b, gem_interp(7, _t2754)};
            (void)(gem_buf_push_fn(NULL, _t2755, 2));
        } else {
#line 837 "compiler/main.gem"
    GemVal _t2756 = gem_v_entry;
    static GemICacheSlot _ic_368 = {0};
    GemVal _t2757[] = {gem_table_get_cached(_t2756, "key", &_ic_368)};
            GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t2757, 1);
#line 838 "compiler/main.gem"
    GemVal _t2758 = gem_v_val_r;
    static GemICacheSlot _ic_369 = {0};
    GemVal _t2759[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"), "), gem_table_get_cached(_t2758, "expr", &_ic_369), gem_string(");\n")};
    GemVal _t2760[] = {gem_v_b, gem_interp(7, _t2759)};
            (void)(gem_buf_push_fn(NULL, _t2760, 2));
        }
    }

    GemVal _t2761 = gem_table_new();
    gem_table_set(_t2761, gem_string("expr"), gem_v_t);
    GemVal _t2762[] = {gem_v_b};
    gem_table_set(_t2761, gem_string("setup"), gem_buf_str_fn(NULL, _t2762, 1));
    GemVal _t2763 = _t2761;
    gem_pop_frame();
    return _t2763;
}

struct _closure__anon_42 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_42(void *_env, GemVal *args, int argc) {
    struct _closure__anon_42 *_cls = (struct _closure__anon_42 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_42", "compiler/main.gem", 0);
#line 847 "compiler/main.gem"
    GemVal _t2765 = gem_v_node;
    static GemICacheSlot _ic_370 = {0};
    GemVal gem_v_elements = gem_table_get_cached(_t2765, "elements", &_ic_370);
#line 848 "compiler/main.gem"
    GemVal _t2766 = (*gem_v_tmp);
    GemVal gem_v_t = _t2766.fn(_t2766.env, NULL, 0);
#line 849 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 850 "compiler/main.gem"
    GemVal _t2767[] = {gem_string("    GemVal "), gem_v_t, gem_string(" = gem_table_new();\n")};
    GemVal _t2768[] = {gem_v_b, gem_interp(3, _t2767)};
    (void)(gem_buf_push_fn(NULL, _t2768, 2));
#line 851 "compiler/main.gem"
    GemVal gem_v__for_i_52 = gem_int(0);
#line 851 "compiler/main.gem"
    GemVal _t2769[] = {gem_v_elements};
    GemVal gem_v__for_limit_52 = gem_len_fn(NULL, _t2769, 1);
#line 851 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_52, gem_v__for_limit_52))) break;
#line 851 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_52;
#line 851 "compiler/main.gem"
        gem_v__for_i_52 = gem_add(gem_v__for_i_52, gem_int(1));
#line 852 "compiler/main.gem"
    GemVal _t2770[] = {gem_table_get(gem_v_elements, gem_v_i)};
    GemVal _t2771 = (*gem_v_compile_expr);
        GemVal gem_v_elem_r = _t2771.fn(_t2771.env, _t2770, 1);
#line 853 "compiler/main.gem"
    GemVal _t2772 = gem_v_elem_r;
    static GemICacheSlot _ic_371 = {0};
    GemVal _t2773[] = {gem_v_b, gem_table_get_cached(_t2772, "setup", &_ic_371)};
        (void)(gem_buf_push_fn(NULL, _t2773, 2));
#line 854 "compiler/main.gem"
    GemVal _t2774 = gem_v_elem_r;
    static GemICacheSlot _ic_372 = {0};
    GemVal _t2775[] = {gem_string("    gem_table_set("), gem_v_t, gem_string(", gem_int("), gem_v_i, gem_string("), "), gem_table_get_cached(_t2774, "expr", &_ic_372), gem_string(");\n")};
    GemVal _t2776[] = {gem_v_b, gem_interp(7, _t2775)};
        (void)(gem_buf_push_fn(NULL, _t2776, 2));
    }

    GemVal _t2777 = gem_table_new();
    gem_table_set(_t2777, gem_string("expr"), gem_v_t);
    GemVal _t2778[] = {gem_v_b};
    gem_table_set(_t2777, gem_string("setup"), gem_buf_str_fn(NULL, _t2778, 1));
    GemVal _t2779 = _t2777;
    gem_pop_frame();
    return _t2779;
}

struct _closure__anon_43 {
    intptr_t _num_captures;
    GemVal *gem_v_boxed_vars;
    GemVal *gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_43(void *_env, GemVal *args, int argc) {
    struct _closure__anon_43 *_cls = (struct _closure__anon_43 *)_env;
    GemVal *gem_v_boxed_vars = _cls->gem_v_boxed_vars;
    GemVal *gem_v_collect_free_node = _cls->gem_v_collect_free_node;
    GemVal *gem_v_collect_free_vars = _cls->gem_v_collect_free_vars;
    GemVal *gem_v_compile_closure_fn = _cls->gem_v_compile_closure_fn;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_43", "compiler/main.gem", 0);
#line 862 "compiler/main.gem"
    GemVal _t2781 = gem_v_node;
    static GemICacheSlot _ic_373 = {0};
    GemVal _t2782[] = {gem_table_get_cached(_t2781, "params", &_ic_373)};
    GemVal gem_v_inner_defined = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t2782, 1);
#line 863 "compiler/main.gem"
    GemVal _t2783 = gem_v_node;
    static GemICacheSlot _ic_374 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t2783, "rest_param", &_ic_374), GEM_NIL))) {
#line 864 "compiler/main.gem"
    GemVal _t2784 = gem_v_node;
    static GemICacheSlot _ic_375 = {0};
    GemVal _t2785[] = {gem_v_inner_defined, gem_table_get_cached(_t2784, "rest_param", &_ic_375)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t2785, 2));
    }
#line 866 "compiler/main.gem"
    GemVal _t2786 = gem_v_node;
    static GemICacheSlot _ic_376 = {0};
    GemVal _t2787[] = {gem_table_get_cached(_t2786, "body", &_ic_376), gem_v_inner_defined};
    GemVal _t2788 = (*gem_v_collect_free_vars);
    GemVal gem_v_free = _t2788.fn(_t2788.env, _t2787, 2);
#line 867 "compiler/main.gem"
    GemVal _t2789 = gem_v_node;
    static GemICacheSlot _ic_377 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t2789, "defaults", &_ic_377), GEM_NIL))) {
#line 868 "compiler/main.gem"
    GemVal _t2790 = gem_v_node;
    static GemICacheSlot _ic_378 = {0};
        GemVal gem_v__for_items_53 = gem_table_get_cached(_t2790, "defaults", &_ic_378);
#line 868 "compiler/main.gem"
        GemVal gem_v__for_i_53 = gem_int(0);
#line 868 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t2791[] = {gem_v__for_items_53};
            if (!gem_truthy(gem_lt(gem_v__for_i_53, gem_len_fn(NULL, _t2791, 1)))) break;
#line 868 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_53, gem_v__for_i_53);
#line 868 "compiler/main.gem"
            gem_v__for_i_53 = gem_add(gem_v__for_i_53, gem_int(1));
#line 869 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 870 "compiler/main.gem"
    GemVal _t2792[] = {gem_v_d, gem_v_inner_defined, gem_v_free};
    GemVal _t2793 = (*gem_v_collect_free_node);
                (void)(_t2793.fn(_t2793.env, _t2792, 3));
            }
        }

    }
#line 874 "compiler/main.gem"
    GemVal _t2794[] = {gem_v_free};
    GemVal _t2795[] = {gem_keys_fn(NULL, _t2794, 1)};
    GemVal gem_v_captures = gem_sort_fn(NULL, _t2795, 1);
#line 876 "compiler/main.gem"
    GemVal _t2796[] = {gem_v_captures};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t2796, 1), gem_int(0)))) {
#line 877 "compiler/main.gem"
    GemVal _t2797 = gem_table_new();
    GemVal _t2798[] = {gem_v_node, _t2797};
    GemVal _t2799 = (*gem_v_compile_closure_fn);
        GemVal gem_v_result = _t2799.fn(_t2799.env, _t2798, 2);
#line 878 "compiler/main.gem"
    GemVal _t2800 = gem_table_new();
    GemVal _t2801 = gem_v_result;
    static GemICacheSlot _ic_379 = {0};
    GemVal _t2802[] = {gem_string("gem_make_fn("), gem_table_get_cached(_t2801, "fn_name", &_ic_379), gem_string(", NULL)")};
    gem_table_set(_t2800, gem_string("expr"), gem_interp(3, _t2802));
    gem_table_set(_t2800, gem_string("setup"), gem_string(""));
        GemVal _t2803 = _t2800;
        gem_pop_frame();
        return _t2803;
    }
#line 881 "compiler/main.gem"
    GemVal _t2804[] = {gem_v_node, gem_v_captures};
    GemVal _t2805 = (*gem_v_compile_closure_fn);
    GemVal gem_v_result = _t2805.fn(_t2805.env, _t2804, 2);
#line 883 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 884 "compiler/main.gem"
    GemVal _t2806 = (*gem_v_tmp);
    GemVal gem_v_env_tmp = _t2806.fn(_t2806.env, NULL, 0);
#line 885 "compiler/main.gem"
    GemVal _t2807 = gem_v_result;
    static GemICacheSlot _ic_380 = {0};
    GemVal _t2808 = gem_v_result;
    static GemICacheSlot _ic_381 = {0};
    GemVal _t2809[] = {gem_string("    struct "), gem_table_get_cached(_t2807, "struct_name", &_ic_380), gem_string(" *"), gem_v_env_tmp, gem_string(" = GC_MALLOC(sizeof(struct "), gem_table_get_cached(_t2808, "struct_name", &_ic_381), gem_string("));\n")};
    GemVal _t2810[] = {gem_v_b, gem_interp(7, _t2809)};
    (void)(gem_buf_push_fn(NULL, _t2810, 2));
#line 886 "compiler/main.gem"
    GemVal _t2811[] = {gem_v_captures};
    GemVal _t2812[] = {gem_string("    "), gem_v_env_tmp, gem_string("->_num_captures = "), gem_len_fn(NULL, _t2811, 1), gem_string(";\n")};
    GemVal _t2813[] = {gem_v_b, gem_interp(5, _t2812)};
    (void)(gem_buf_push_fn(NULL, _t2813, 2));
#line 887 "compiler/main.gem"
    GemVal gem_v__for_items_54 = gem_v_captures;
#line 887 "compiler/main.gem"
    GemVal gem_v__for_i_54 = gem_int(0);
#line 887 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t2814[] = {gem_v__for_items_54};
        if (!gem_truthy(gem_lt(gem_v__for_i_54, gem_len_fn(NULL, _t2814, 1)))) break;
#line 887 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_54, gem_v__for_i_54);
#line 887 "compiler/main.gem"
        gem_v__for_i_54 = gem_add(gem_v__for_i_54, gem_int(1));
#line 888 "compiler/main.gem"
    GemVal _t2815[] = {gem_v_cap};
    GemVal _t2816 = (*gem_v_mangle);
        GemVal gem_v_mc = _t2816.fn(_t2816.env, _t2815, 1);
#line 889 "compiler/main.gem"
    GemVal _t2817[] = {(*gem_v_boxed_vars), gem_v_cap};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2817, 2))) {
#line 890 "compiler/main.gem"
    GemVal _t2818[] = {gem_string("    "), gem_v_env_tmp, gem_string("->"), gem_v_mc, gem_string(" = "), gem_v_mc, gem_string(";\n")};
    GemVal _t2819[] = {gem_v_b, gem_interp(7, _t2818)};
            (void)(gem_buf_push_fn(NULL, _t2819, 2));
        } else {
#line 892 "compiler/main.gem"
    GemVal _t2820[] = {gem_string("    "), gem_v_env_tmp, gem_string("->"), gem_v_mc, gem_string(" = &"), gem_v_mc, gem_string(";\n")};
    GemVal _t2821[] = {gem_v_b, gem_interp(7, _t2820)};
            (void)(gem_buf_push_fn(NULL, _t2821, 2));
        }
    }

    GemVal _t2822 = gem_table_new();
    GemVal _t2823 = gem_v_result;
    static GemICacheSlot _ic_382 = {0};
    GemVal _t2824[] = {gem_string("gem_make_fn("), gem_table_get_cached(_t2823, "fn_name", &_ic_382), gem_string(", "), gem_v_env_tmp, gem_string(")")};
    gem_table_set(_t2822, gem_string("expr"), gem_interp(5, _t2824));
    GemVal _t2825[] = {gem_v_b};
    gem_table_set(_t2822, gem_string("setup"), gem_buf_str_fn(NULL, _t2825, 1));
    GemVal _t2826 = _t2822;
    gem_pop_frame();
    return _t2826;
}

struct _closure__anon_44 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_44(void *_env, GemVal *args, int argc) {
    struct _closure__anon_44 *_cls = (struct _closure__anon_44 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_44", "compiler/main.gem", 0);
#line 902 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 903 "compiler/main.gem"
    GemVal _t2828 = gem_table_new();
    GemVal gem_v_arg_exprs = _t2828;
#line 904 "compiler/main.gem"
    GemVal gem_v__for_i_55 = gem_int(0);
#line 904 "compiler/main.gem"
    GemVal _t2829[] = {gem_v_args};
    GemVal gem_v__for_limit_55 = gem_len_fn(NULL, _t2829, 1);
#line 904 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_55, gem_v__for_limit_55))) break;
#line 904 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_55;
#line 904 "compiler/main.gem"
        gem_v__for_i_55 = gem_add(gem_v__for_i_55, gem_int(1));
#line 905 "compiler/main.gem"
    GemVal _t2830[] = {gem_table_get(gem_v_args, gem_v_i)};
    GemVal _t2831 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t2831.fn(_t2831.env, _t2830, 1);
#line 906 "compiler/main.gem"
    GemVal _t2832 = gem_v_r;
    static GemICacheSlot _ic_383 = {0};
    GemVal _t2833[] = {gem_v_b, gem_table_get_cached(_t2832, "setup", &_ic_383)};
        (void)(gem_buf_push_fn(NULL, _t2833, 2));
#line 907 "compiler/main.gem"
    GemVal _t2834 = gem_v_r;
    static GemICacheSlot _ic_384 = {0};
    GemVal _t2835[] = {gem_v_arg_exprs, gem_table_get_cached(_t2834, "expr", &_ic_384)};
        (void)(gem_push_fn(NULL, _t2835, 2));
    }

#line 909 "compiler/main.gem"
    GemVal _t2836[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t2836, 1);
#line 910 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_argc, gem_int(0)))) {
#line 911 "compiler/main.gem"
    GemVal _t2837 = gem_table_new();
    GemVal _t2838[] = {gem_v_b};
    gem_table_set(_t2837, gem_string("setup"), gem_buf_str_fn(NULL, _t2838, 1));
    gem_table_set(_t2837, gem_string("arr_name"), GEM_NIL);
    gem_table_set(_t2837, gem_string("argc"), gem_int(0));
        GemVal _t2839 = _t2837;
        gem_pop_frame();
        return _t2839;
    }
#line 913 "compiler/main.gem"
    GemVal _t2840 = (*gem_v_tmp);
    GemVal gem_v_t = _t2840.fn(_t2840.env, NULL, 0);
#line 914 "compiler/main.gem"
    GemVal gem_v_arr = gem_table_get(gem_v_arg_exprs, gem_int(0));
#line 915 "compiler/main.gem"
    GemVal gem_v__for_i_56 = gem_int(1);
#line 915 "compiler/main.gem"
    GemVal gem_v__for_limit_56 = gem_v_argc;
#line 915 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_56, gem_v__for_limit_56))) break;
#line 915 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_56;
#line 915 "compiler/main.gem"
        gem_v__for_i_56 = gem_add(gem_v__for_i_56, gem_int(1));
#line 916 "compiler/main.gem"
        gem_v_arr = gem_add(gem_add(gem_v_arr, gem_string(", ")), gem_table_get(gem_v_arg_exprs, gem_v_i));
    }

    GemVal _t2841 = gem_table_new();
    GemVal _t2842[] = {gem_v_b};
    GemVal _t2843[] = {gem_string("    GemVal "), gem_v_t, gem_string("[] = {"), gem_v_arr, gem_string("};\n")};
    gem_table_set(_t2841, gem_string("setup"), gem_add(gem_buf_str_fn(NULL, _t2842, 1), gem_interp(5, _t2843)));
    gem_table_set(_t2841, gem_string("arr_name"), gem_v_t);
    gem_table_set(_t2841, gem_string("argc"), gem_v_argc);
    GemVal _t2844 = _t2841;
    gem_pop_frame();
    return _t2844;
}

struct _closure__anon_45 {
    intptr_t _num_captures;
};
static GemVal _anon_45(void *_env, GemVal *args, int argc) {
    GemVal gem_v_fn_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_env = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ca = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_45", "compiler/main.gem", 0);
#line 923 "compiler/main.gem"
    GemVal _t2846 = gem_v_ca;
    static GemICacheSlot _ic_385 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2846, "argc", &_ic_385), gem_int(0)))) {
#line 924 "compiler/main.gem"
    GemVal _t2847 = gem_table_new();
    GemVal _t2848[] = {gem_v_fn_name, gem_string("("), gem_v_env, gem_string(", NULL, 0)")};
    gem_table_set(_t2847, gem_string("expr"), gem_interp(4, _t2848));
    GemVal _t2849 = gem_v_ca;
    static GemICacheSlot _ic_386 = {0};
    gem_table_set(_t2847, gem_string("setup"), gem_table_get_cached(_t2849, "setup", &_ic_386));
        GemVal _t2850 = _t2847;
        gem_pop_frame();
        return _t2850;
    }
    GemVal _t2851 = gem_table_new();
    GemVal _t2852 = gem_v_ca;
    static GemICacheSlot _ic_387 = {0};
    GemVal _t2853 = gem_v_ca;
    static GemICacheSlot _ic_388 = {0};
    GemVal _t2854[] = {gem_v_fn_name, gem_string("("), gem_v_env, gem_string(", "), gem_table_get_cached(_t2852, "arr_name", &_ic_387), gem_string(", "), gem_table_get_cached(_t2853, "argc", &_ic_388), gem_string(")")};
    gem_table_set(_t2851, gem_string("expr"), gem_interp(8, _t2854));
    GemVal _t2855 = gem_v_ca;
    static GemICacheSlot _ic_389 = {0};
    gem_table_set(_t2851, gem_string("setup"), gem_table_get_cached(_t2855, "setup", &_ic_389));
    GemVal _t2856 = _t2851;
    gem_pop_frame();
    return _t2856;
}

struct _closure__anon_46 {
    intptr_t _num_captures;
    GemVal *gem_v_builtin_fns;
    GemVal *gem_v_compile_args;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call;
    GemVal *gem_v_local_names;
    GemVal *gem_v_source_name;
    GemVal *gem_v_tmp;
};
static GemVal _anon_46(void *_env, GemVal *args, int argc) {
    struct _closure__anon_46 *_cls = (struct _closure__anon_46 *)_env;
    GemVal *gem_v_builtin_fns = _cls->gem_v_builtin_fns;
    GemVal *gem_v_compile_args = _cls->gem_v_compile_args;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_defined_fns = _cls->gem_v_defined_fns;
    GemVal *gem_v_emit_direct_call = _cls->gem_v_emit_direct_call;
    GemVal *gem_v_local_names = _cls->gem_v_local_names;
    GemVal *gem_v_source_name = _cls->gem_v_source_name;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_46", "compiler/main.gem", 0);
#line 932 "compiler/main.gem"
    GemVal gem_v__d57 = gem_v_node;
#line 932 "compiler/main.gem"
    GemVal _t2857 = gem_v__d57;
    static GemICacheSlot _ic_390 = {0};
    GemVal gem_v_func = gem_table_get_cached(_t2857, "func", &_ic_390);
#line 932 "compiler/main.gem"
    GemVal _t2858 = gem_v__d57;
    static GemICacheSlot _ic_391 = {0};
    GemVal gem_v_args = gem_table_get_cached(_t2858, "args", &_ic_391);

#line 935 "compiler/main.gem"
    GemVal _t2859 = gem_v_func;
    static GemICacheSlot _ic_392 = {0};
    GemVal _t2861;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t2859, "tag", &_ic_392), gem_string("dot")))) {
        _t2861 = gem_eq(gem_table_get_cached(_t2859, "tag", &_ic_392), gem_string("dot"));
    } else {
        GemVal _t2860 = gem_v_func;
        static GemICacheSlot _ic_393 = {0};
        _t2861 = gem_eq(gem_table_get_cached(_t2860, "field", &_ic_393), gem_string("len"));
    }
    GemVal _t2863;
    if (!gem_truthy(_t2861)) {
        _t2863 = _t2861;
    } else {
        GemVal _t2862[] = {gem_v_args};
        _t2863 = gem_eq(gem_len_fn(NULL, _t2862, 1), gem_int(0));
    }
    if (gem_truthy(_t2863)) {
#line 936 "compiler/main.gem"
    GemVal _t2864 = gem_v_func;
    static GemICacheSlot _ic_394 = {0};
    GemVal _t2865[] = {gem_table_get_cached(_t2864, "object", &_ic_394)};
    GemVal _t2866 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t2866.fn(_t2866.env, _t2865, 1);
#line 937 "compiler/main.gem"
    GemVal _t2867 = gem_table_new();
    GemVal _t2868 = gem_v_obj_r;
    static GemICacheSlot _ic_395 = {0};
    GemVal _t2869[] = {gem_string("gem_len_val("), gem_table_get_cached(_t2868, "expr", &_ic_395), gem_string(")")};
    gem_table_set(_t2867, gem_string("expr"), gem_interp(3, _t2869));
    GemVal _t2870 = gem_v_obj_r;
    static GemICacheSlot _ic_396 = {0};
    gem_table_set(_t2867, gem_string("setup"), gem_table_get_cached(_t2870, "setup", &_ic_396));
        GemVal _t2871 = _t2867;
        gem_pop_frame();
        return _t2871;
    }
#line 941 "compiler/main.gem"
    GemVal _t2872 = gem_v_func;
    static GemICacheSlot _ic_397 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2872, "tag", &_ic_397), gem_string("var")))) {
#line 942 "compiler/main.gem"
    GemVal _t2873 = gem_v_func;
    static GemICacheSlot _ic_398 = {0};
        GemVal gem_v_name = gem_table_get_cached(_t2873, "name", &_ic_398);
#line 944 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_name, gem_string("error")))) {
#line 945 "compiler/main.gem"
    GemVal _t2874 = gem_v_node;
    static GemICacheSlot _ic_399 = {0};
            GemVal gem_v_line = gem_table_get_cached(_t2874, "line", &_ic_399);
#line 946 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_line, GEM_NIL))) {
#line 947 "compiler/main.gem"
                gem_v_line = gem_int(0);
            }
#line 949 "compiler/main.gem"
    GemVal _t2875[] = {(*gem_v_source_name)};
            GemVal gem_v_escaped_file = gem_fn__mod_codegen_escape_c_string(NULL, _t2875, 1);
#line 950 "compiler/main.gem"
    GemVal _t2876[] = {gem_v_args};
    GemVal _t2877 = (*gem_v_compile_args);
            GemVal gem_v_ca = _t2877.fn(_t2877.env, _t2876, 1);
#line 951 "compiler/main.gem"
    GemVal _t2878 = gem_v_ca;
    static GemICacheSlot _ic_400 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t2878, "argc", &_ic_400), gem_int(0)))) {
#line 952 "compiler/main.gem"
    GemVal _t2879 = gem_table_new();
    GemVal _t2880[] = {gem_string("gem_error_at_fn(\""), gem_v_escaped_file, gem_string("\", "), gem_v_line, gem_string(", NULL, 0)")};
    gem_table_set(_t2879, gem_string("expr"), gem_interp(5, _t2880));
    GemVal _t2881 = gem_v_ca;
    static GemICacheSlot _ic_401 = {0};
    gem_table_set(_t2879, gem_string("setup"), gem_table_get_cached(_t2881, "setup", &_ic_401));
                GemVal _t2882 = _t2879;
                gem_pop_frame();
                return _t2882;
            }
#line 954 "compiler/main.gem"
    GemVal _t2883 = gem_table_new();
    GemVal _t2884 = gem_v_ca;
    static GemICacheSlot _ic_402 = {0};
    GemVal _t2885 = gem_v_ca;
    static GemICacheSlot _ic_403 = {0};
    GemVal _t2886[] = {gem_string("gem_error_at_fn(\""), gem_v_escaped_file, gem_string("\", "), gem_v_line, gem_string(", "), gem_table_get_cached(_t2884, "arr_name", &_ic_402), gem_string(", "), gem_table_get_cached(_t2885, "argc", &_ic_403), gem_string(")")};
    gem_table_set(_t2883, gem_string("expr"), gem_interp(9, _t2886));
    GemVal _t2887 = gem_v_ca;
    static GemICacheSlot _ic_404 = {0};
    gem_table_set(_t2883, gem_string("setup"), gem_table_get_cached(_t2887, "setup", &_ic_404));
            GemVal _t2888 = _t2883;
            gem_pop_frame();
            return _t2888;
        } else {
#line 955 "compiler/main.gem"
    GemVal _t2889[] = {(*gem_v_builtin_fns), gem_v_name};
    GemVal _t2891;
    if (!gem_truthy(gem_has_key_fn(NULL, _t2889, 2))) {
        _t2891 = gem_has_key_fn(NULL, _t2889, 2);
    } else {
        GemVal _t2890[] = {(*gem_v_local_names), gem_v_name};
        _t2891 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2890, 2));
    }
            if (gem_truthy(_t2891)) {
#line 956 "compiler/main.gem"
    GemVal _t2892[] = {gem_v_args};
    GemVal _t2893 = (*gem_v_compile_args);
    GemVal _t2894[] = {gem_table_get((*gem_v_builtin_fns), gem_v_name), gem_string("NULL"), _t2893.fn(_t2893.env, _t2892, 1)};
    GemVal _t2895 = (*gem_v_emit_direct_call);
                GemVal _t2896 = _t2895.fn(_t2895.env, _t2894, 3);
                gem_pop_frame();
                return _t2896;
            } else {
#line 957 "compiler/main.gem"
    GemVal _t2897[] = {(*gem_v_defined_fns), gem_v_name};
    GemVal _t2899;
    if (!gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t2897, 2))) {
        _t2899 = gem_fn__mod_codegen_set_contains(NULL, _t2897, 2);
    } else {
        GemVal _t2898[] = {(*gem_v_local_names), gem_v_name};
        _t2899 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t2898, 2));
    }
                if (gem_truthy(_t2899)) {
#line 958 "compiler/main.gem"
    GemVal _t2900[] = {gem_string("gem_fn_"), gem_v_name};
    GemVal _t2901[] = {gem_v_args};
    GemVal _t2902 = (*gem_v_compile_args);
    GemVal _t2903[] = {gem_interp(2, _t2900), gem_string("NULL"), _t2902.fn(_t2902.env, _t2901, 1)};
    GemVal _t2904 = (*gem_v_emit_direct_call);
                    GemVal _t2905 = _t2904.fn(_t2904.env, _t2903, 3);
                    gem_pop_frame();
                    return _t2905;
                }
            }
        }
    }
#line 963 "compiler/main.gem"
    GemVal _t2906[] = {gem_v_func};
    GemVal _t2907 = (*gem_v_compile_expr);
    GemVal gem_v_func_r = _t2907.fn(_t2907.env, _t2906, 1);
#line 964 "compiler/main.gem"
    GemVal _t2908[] = {gem_v_args};
    GemVal _t2909 = (*gem_v_compile_args);
    GemVal gem_v_ca = _t2909.fn(_t2909.env, _t2908, 1);
#line 965 "compiler/main.gem"
    GemVal _t2910 = (*gem_v_tmp);
    GemVal gem_v_tmp_fn = _t2910.fn(_t2910.env, NULL, 0);
#line 966 "compiler/main.gem"
    GemVal _t2911 = gem_v_func_r;
    static GemICacheSlot _ic_405 = {0};
    GemVal _t2912 = gem_v_ca;
    static GemICacheSlot _ic_406 = {0};
    GemVal _t2913 = gem_v_func_r;
    static GemICacheSlot _ic_407 = {0};
    GemVal _t2914[] = {gem_string("    GemVal "), gem_v_tmp_fn, gem_string(" = "), gem_table_get_cached(_t2913, "expr", &_ic_407), gem_string(";\n")};
    GemVal gem_v_setup = gem_add(gem_add(gem_table_get_cached(_t2911, "setup", &_ic_405), gem_table_get_cached(_t2912, "setup", &_ic_406)), gem_interp(5, _t2914));
#line 967 "compiler/main.gem"
    GemVal _t2915 = gem_v_ca;
    static GemICacheSlot _ic_408 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t2915, "argc", &_ic_408), gem_int(0)))) {
#line 968 "compiler/main.gem"
    GemVal _t2916 = gem_table_new();
    GemVal _t2917[] = {gem_v_tmp_fn, gem_string(".fn("), gem_v_tmp_fn, gem_string(".env, NULL, 0)")};
    gem_table_set(_t2916, gem_string("expr"), gem_interp(4, _t2917));
    gem_table_set(_t2916, gem_string("setup"), gem_v_setup);
        GemVal _t2918 = _t2916;
        gem_pop_frame();
        return _t2918;
    }
    GemVal _t2919 = gem_table_new();
    GemVal _t2920 = gem_v_ca;
    static GemICacheSlot _ic_409 = {0};
    GemVal _t2921 = gem_v_ca;
    static GemICacheSlot _ic_410 = {0};
    GemVal _t2922[] = {gem_v_tmp_fn, gem_string(".fn("), gem_v_tmp_fn, gem_string(".env, "), gem_table_get_cached(_t2920, "arr_name", &_ic_409), gem_string(", "), gem_table_get_cached(_t2921, "argc", &_ic_410), gem_string(")")};
    gem_table_set(_t2919, gem_string("expr"), gem_interp(8, _t2922));
    gem_table_set(_t2919, gem_string("setup"), gem_v_setup);
    GemVal _t2923 = _t2919;
    gem_pop_frame();
    return _t2923;
}

struct _closure__anon_47 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_tmp;
};
static GemVal _anon_47(void *_env, GemVal *args, int argc) {
    struct _closure__anon_47 *_cls = (struct _closure__anon_47 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_47", "compiler/main.gem", 0);
#line 976 "compiler/main.gem"
    GemVal _t2925 = gem_v_node;
    static GemICacheSlot _ic_411 = {0};
    GemVal gem_v_op = gem_table_get_cached(_t2925, "op", &_ic_411);
#line 977 "compiler/main.gem"
    GemVal _t2926 = gem_v_node;
    static GemICacheSlot _ic_412 = {0};
    GemVal _t2927[] = {gem_table_get_cached(_t2926, "left", &_ic_412)};
    GemVal _t2928 = (*gem_v_compile_expr);
    GemVal gem_v_lc_r = _t2928.fn(_t2928.env, _t2927, 1);
#line 978 "compiler/main.gem"
    GemVal _t2929 = gem_v_lc_r;
    static GemICacheSlot _ic_413 = {0};
    GemVal gem_v_lc = gem_table_get_cached(_t2929, "expr", &_ic_413);
#line 979 "compiler/main.gem"
    GemVal _t2930 = gem_v_lc_r;
    static GemICacheSlot _ic_414 = {0};
    GemVal gem_v_ls = gem_table_get_cached(_t2930, "setup", &_ic_414);
#line 981 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_op, gem_string("+")))) {
#line 982 "compiler/main.gem"
    GemVal _t2931 = gem_v_node;
    static GemICacheSlot _ic_415 = {0};
    GemVal _t2932[] = {gem_table_get_cached(_t2931, "right", &_ic_415)};
    GemVal _t2933 = (*gem_v_compile_expr);
        GemVal gem_v_rc_r = _t2933.fn(_t2933.env, _t2932, 1);
#line 983 "compiler/main.gem"
    GemVal _t2934 = gem_table_new();
    GemVal _t2935 = gem_v_rc_r;
    static GemICacheSlot _ic_416 = {0};
    GemVal _t2936[] = {gem_string("gem_add("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2935, "expr", &_ic_416), gem_string(")")};
    gem_table_set(_t2934, gem_string("expr"), gem_interp(5, _t2936));
    GemVal _t2937 = gem_v_rc_r;
    static GemICacheSlot _ic_417 = {0};
    gem_table_set(_t2934, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2937, "setup", &_ic_417)));
        GemVal _t2938 = _t2934;
        gem_pop_frame();
        return _t2938;
    } else {
#line 984 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_op, gem_string("-")))) {
#line 985 "compiler/main.gem"
    GemVal _t2939 = gem_v_node;
    static GemICacheSlot _ic_418 = {0};
    GemVal _t2940[] = {gem_table_get_cached(_t2939, "right", &_ic_418)};
    GemVal _t2941 = (*gem_v_compile_expr);
            GemVal gem_v_rc_r = _t2941.fn(_t2941.env, _t2940, 1);
#line 986 "compiler/main.gem"
    GemVal _t2942 = gem_table_new();
    GemVal _t2943 = gem_v_rc_r;
    static GemICacheSlot _ic_419 = {0};
    GemVal _t2944[] = {gem_string("gem_sub("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2943, "expr", &_ic_419), gem_string(")")};
    gem_table_set(_t2942, gem_string("expr"), gem_interp(5, _t2944));
    GemVal _t2945 = gem_v_rc_r;
    static GemICacheSlot _ic_420 = {0};
    gem_table_set(_t2942, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2945, "setup", &_ic_420)));
            GemVal _t2946 = _t2942;
            gem_pop_frame();
            return _t2946;
        } else {
#line 987 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_op, gem_string("*")))) {
#line 988 "compiler/main.gem"
    GemVal _t2947 = gem_v_node;
    static GemICacheSlot _ic_421 = {0};
    GemVal _t2948[] = {gem_table_get_cached(_t2947, "right", &_ic_421)};
    GemVal _t2949 = (*gem_v_compile_expr);
                GemVal gem_v_rc_r = _t2949.fn(_t2949.env, _t2948, 1);
#line 989 "compiler/main.gem"
    GemVal _t2950 = gem_table_new();
    GemVal _t2951 = gem_v_rc_r;
    static GemICacheSlot _ic_422 = {0};
    GemVal _t2952[] = {gem_string("gem_mul("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2951, "expr", &_ic_422), gem_string(")")};
    gem_table_set(_t2950, gem_string("expr"), gem_interp(5, _t2952));
    GemVal _t2953 = gem_v_rc_r;
    static GemICacheSlot _ic_423 = {0};
    gem_table_set(_t2950, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2953, "setup", &_ic_423)));
                GemVal _t2954 = _t2950;
                gem_pop_frame();
                return _t2954;
            } else {
#line 990 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_op, gem_string("/")))) {
#line 991 "compiler/main.gem"
    GemVal _t2955 = gem_v_node;
    static GemICacheSlot _ic_424 = {0};
    GemVal _t2956[] = {gem_table_get_cached(_t2955, "right", &_ic_424)};
    GemVal _t2957 = (*gem_v_compile_expr);
                    GemVal gem_v_rc_r = _t2957.fn(_t2957.env, _t2956, 1);
#line 992 "compiler/main.gem"
    GemVal _t2958 = gem_table_new();
    GemVal _t2959 = gem_v_rc_r;
    static GemICacheSlot _ic_425 = {0};
    GemVal _t2960[] = {gem_string("gem_div("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2959, "expr", &_ic_425), gem_string(")")};
    gem_table_set(_t2958, gem_string("expr"), gem_interp(5, _t2960));
    GemVal _t2961 = gem_v_rc_r;
    static GemICacheSlot _ic_426 = {0};
    gem_table_set(_t2958, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2961, "setup", &_ic_426)));
                    GemVal _t2962 = _t2958;
                    gem_pop_frame();
                    return _t2962;
                } else {
#line 993 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_op, gem_string("%")))) {
#line 994 "compiler/main.gem"
    GemVal _t2963 = gem_v_node;
    static GemICacheSlot _ic_427 = {0};
    GemVal _t2964[] = {gem_table_get_cached(_t2963, "right", &_ic_427)};
    GemVal _t2965 = (*gem_v_compile_expr);
                        GemVal gem_v_rc_r = _t2965.fn(_t2965.env, _t2964, 1);
#line 995 "compiler/main.gem"
    GemVal _t2966 = gem_table_new();
    GemVal _t2967 = gem_v_rc_r;
    static GemICacheSlot _ic_428 = {0};
    GemVal _t2968[] = {gem_string("gem_mod("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2967, "expr", &_ic_428), gem_string(")")};
    gem_table_set(_t2966, gem_string("expr"), gem_interp(5, _t2968));
    GemVal _t2969 = gem_v_rc_r;
    static GemICacheSlot _ic_429 = {0};
    gem_table_set(_t2966, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2969, "setup", &_ic_429)));
                        GemVal _t2970 = _t2966;
                        gem_pop_frame();
                        return _t2970;
                    } else {
#line 996 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_op, gem_string("in")))) {
#line 997 "compiler/main.gem"
    GemVal _t2971 = gem_v_node;
    static GemICacheSlot _ic_430 = {0};
    GemVal _t2972[] = {gem_table_get_cached(_t2971, "right", &_ic_430)};
    GemVal _t2973 = (*gem_v_compile_expr);
                            GemVal gem_v_rc_r = _t2973.fn(_t2973.env, _t2972, 1);
#line 998 "compiler/main.gem"
    GemVal _t2974 = (*gem_v_tmp);
                            GemVal gem_v_t = _t2974.fn(_t2974.env, NULL, 0);
#line 999 "compiler/main.gem"
    GemVal _t2975 = gem_v_rc_r;
    static GemICacheSlot _ic_431 = {0};
    GemVal _t2976 = gem_v_rc_r;
    static GemICacheSlot _ic_432 = {0};
    GemVal _t2977[] = {gem_string("    GemVal "), gem_v_t, gem_string("[] = {"), gem_table_get_cached(_t2976, "expr", &_ic_432), gem_string(", "), gem_v_lc, gem_string("};\n")};
                            GemVal gem_v_setup = gem_add(gem_add(gem_v_ls, gem_table_get_cached(_t2975, "setup", &_ic_431)), gem_interp(7, _t2977));
#line 1000 "compiler/main.gem"
    GemVal _t2978 = gem_table_new();
    GemVal _t2979[] = {gem_string("gem_in_fn(NULL, "), gem_v_t, gem_string(", 2)")};
    gem_table_set(_t2978, gem_string("expr"), gem_interp(3, _t2979));
    gem_table_set(_t2978, gem_string("setup"), gem_v_setup);
                            GemVal _t2980 = _t2978;
                            gem_pop_frame();
                            return _t2980;
                        } else {
#line 1001 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_op, gem_string("==")))) {
#line 1002 "compiler/main.gem"
    GemVal _t2981 = gem_v_node;
    static GemICacheSlot _ic_433 = {0};
    GemVal _t2982[] = {gem_table_get_cached(_t2981, "right", &_ic_433)};
    GemVal _t2983 = (*gem_v_compile_expr);
                                GemVal gem_v_rc_r = _t2983.fn(_t2983.env, _t2982, 1);
#line 1003 "compiler/main.gem"
    GemVal _t2984 = gem_table_new();
    GemVal _t2985 = gem_v_rc_r;
    static GemICacheSlot _ic_434 = {0};
    GemVal _t2986[] = {gem_string("gem_eq("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2985, "expr", &_ic_434), gem_string(")")};
    gem_table_set(_t2984, gem_string("expr"), gem_interp(5, _t2986));
    GemVal _t2987 = gem_v_rc_r;
    static GemICacheSlot _ic_435 = {0};
    gem_table_set(_t2984, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2987, "setup", &_ic_435)));
                                GemVal _t2988 = _t2984;
                                gem_pop_frame();
                                return _t2988;
                            } else {
#line 1004 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_op, gem_string("!=")))) {
#line 1005 "compiler/main.gem"
    GemVal _t2989 = gem_v_node;
    static GemICacheSlot _ic_436 = {0};
    GemVal _t2990[] = {gem_table_get_cached(_t2989, "right", &_ic_436)};
    GemVal _t2991 = (*gem_v_compile_expr);
                                    GemVal gem_v_rc_r = _t2991.fn(_t2991.env, _t2990, 1);
#line 1006 "compiler/main.gem"
    GemVal _t2992 = gem_table_new();
    GemVal _t2993 = gem_v_rc_r;
    static GemICacheSlot _ic_437 = {0};
    GemVal _t2994[] = {gem_string("gem_neq("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t2993, "expr", &_ic_437), gem_string(")")};
    gem_table_set(_t2992, gem_string("expr"), gem_interp(5, _t2994));
    GemVal _t2995 = gem_v_rc_r;
    static GemICacheSlot _ic_438 = {0};
    gem_table_set(_t2992, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t2995, "setup", &_ic_438)));
                                    GemVal _t2996 = _t2992;
                                    gem_pop_frame();
                                    return _t2996;
                                } else {
#line 1007 "compiler/main.gem"
                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("<")))) {
#line 1008 "compiler/main.gem"
    GemVal _t2997 = gem_v_node;
    static GemICacheSlot _ic_439 = {0};
    GemVal _t2998[] = {gem_table_get_cached(_t2997, "right", &_ic_439)};
    GemVal _t2999 = (*gem_v_compile_expr);
                                        GemVal gem_v_rc_r = _t2999.fn(_t2999.env, _t2998, 1);
#line 1009 "compiler/main.gem"
    GemVal _t3000 = gem_table_new();
    GemVal _t3001 = gem_v_rc_r;
    static GemICacheSlot _ic_440 = {0};
    GemVal _t3002[] = {gem_string("gem_lt("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t3001, "expr", &_ic_440), gem_string(")")};
    gem_table_set(_t3000, gem_string("expr"), gem_interp(5, _t3002));
    GemVal _t3003 = gem_v_rc_r;
    static GemICacheSlot _ic_441 = {0};
    gem_table_set(_t3000, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t3003, "setup", &_ic_441)));
                                        GemVal _t3004 = _t3000;
                                        gem_pop_frame();
                                        return _t3004;
                                    } else {
#line 1010 "compiler/main.gem"
                                        if (gem_truthy(gem_eq(gem_v_op, gem_string(">")))) {
#line 1011 "compiler/main.gem"
    GemVal _t3005 = gem_v_node;
    static GemICacheSlot _ic_442 = {0};
    GemVal _t3006[] = {gem_table_get_cached(_t3005, "right", &_ic_442)};
    GemVal _t3007 = (*gem_v_compile_expr);
                                            GemVal gem_v_rc_r = _t3007.fn(_t3007.env, _t3006, 1);
#line 1012 "compiler/main.gem"
    GemVal _t3008 = gem_table_new();
    GemVal _t3009 = gem_v_rc_r;
    static GemICacheSlot _ic_443 = {0};
    GemVal _t3010[] = {gem_string("gem_gt("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t3009, "expr", &_ic_443), gem_string(")")};
    gem_table_set(_t3008, gem_string("expr"), gem_interp(5, _t3010));
    GemVal _t3011 = gem_v_rc_r;
    static GemICacheSlot _ic_444 = {0};
    gem_table_set(_t3008, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t3011, "setup", &_ic_444)));
                                            GemVal _t3012 = _t3008;
                                            gem_pop_frame();
                                            return _t3012;
                                        } else {
#line 1013 "compiler/main.gem"
                                            if (gem_truthy(gem_eq(gem_v_op, gem_string("<=")))) {
#line 1014 "compiler/main.gem"
    GemVal _t3013 = gem_v_node;
    static GemICacheSlot _ic_445 = {0};
    GemVal _t3014[] = {gem_table_get_cached(_t3013, "right", &_ic_445)};
    GemVal _t3015 = (*gem_v_compile_expr);
                                                GemVal gem_v_rc_r = _t3015.fn(_t3015.env, _t3014, 1);
#line 1015 "compiler/main.gem"
    GemVal _t3016 = gem_table_new();
    GemVal _t3017 = gem_v_rc_r;
    static GemICacheSlot _ic_446 = {0};
    GemVal _t3018[] = {gem_string("gem_le("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t3017, "expr", &_ic_446), gem_string(")")};
    gem_table_set(_t3016, gem_string("expr"), gem_interp(5, _t3018));
    GemVal _t3019 = gem_v_rc_r;
    static GemICacheSlot _ic_447 = {0};
    gem_table_set(_t3016, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t3019, "setup", &_ic_447)));
                                                GemVal _t3020 = _t3016;
                                                gem_pop_frame();
                                                return _t3020;
                                            } else {
#line 1016 "compiler/main.gem"
                                                if (gem_truthy(gem_eq(gem_v_op, gem_string(">=")))) {
#line 1017 "compiler/main.gem"
    GemVal _t3021 = gem_v_node;
    static GemICacheSlot _ic_448 = {0};
    GemVal _t3022[] = {gem_table_get_cached(_t3021, "right", &_ic_448)};
    GemVal _t3023 = (*gem_v_compile_expr);
                                                    GemVal gem_v_rc_r = _t3023.fn(_t3023.env, _t3022, 1);
#line 1018 "compiler/main.gem"
    GemVal _t3024 = gem_table_new();
    GemVal _t3025 = gem_v_rc_r;
    static GemICacheSlot _ic_449 = {0};
    GemVal _t3026[] = {gem_string("gem_ge("), gem_v_lc, gem_string(", "), gem_table_get_cached(_t3025, "expr", &_ic_449), gem_string(")")};
    gem_table_set(_t3024, gem_string("expr"), gem_interp(5, _t3026));
    GemVal _t3027 = gem_v_rc_r;
    static GemICacheSlot _ic_450 = {0};
    gem_table_set(_t3024, gem_string("setup"), gem_add(gem_v_ls, gem_table_get_cached(_t3027, "setup", &_ic_450)));
                                                    GemVal _t3028 = _t3024;
                                                    gem_pop_frame();
                                                    return _t3028;
                                                } else {
#line 1019 "compiler/main.gem"
                                                    if (gem_truthy(gem_eq(gem_v_op, gem_string("and")))) {
#line 1020 "compiler/main.gem"
    GemVal _t3029 = gem_v_node;
    static GemICacheSlot _ic_451 = {0};
    GemVal _t3030[] = {gem_table_get_cached(_t3029, "right", &_ic_451)};
    GemVal _t3031 = (*gem_v_compile_expr);
                                                        GemVal gem_v_rc_r = _t3031.fn(_t3031.env, _t3030, 1);
#line 1021 "compiler/main.gem"
    GemVal _t3032 = (*gem_v_tmp);
                                                        GemVal gem_v_t = _t3032.fn(_t3032.env, NULL, 0);
#line 1022 "compiler/main.gem"
                                                        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1023 "compiler/main.gem"
    GemVal _t3033[] = {gem_v_b, gem_v_ls};
                                                        (void)(gem_buf_push_fn(NULL, _t3033, 2));
#line 1024 "compiler/main.gem"
    GemVal _t3034[] = {gem_string("    GemVal "), gem_v_t, gem_string(";\n")};
    GemVal _t3035[] = {gem_v_b, gem_interp(3, _t3034)};
                                                        (void)(gem_buf_push_fn(NULL, _t3035, 2));
#line 1025 "compiler/main.gem"
    GemVal _t3036[] = {gem_string("    if (!gem_truthy("), gem_v_lc, gem_string(")) {\n")};
    GemVal _t3037[] = {gem_v_b, gem_interp(3, _t3036)};
                                                        (void)(gem_buf_push_fn(NULL, _t3037, 2));
#line 1026 "compiler/main.gem"
    GemVal _t3038[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_v_lc, gem_string(";\n")};
    GemVal _t3039[] = {gem_v_b, gem_interp(5, _t3038)};
                                                        (void)(gem_buf_push_fn(NULL, _t3039, 2));
#line 1027 "compiler/main.gem"
    GemVal _t3040[] = {gem_v_b, gem_string("    } else {\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3040, 2));
#line 1028 "compiler/main.gem"
    GemVal _t3041 = gem_v_rc_r;
    static GemICacheSlot _ic_452 = {0};
                                                        if (gem_truthy(gem_neq(gem_table_get_cached(_t3041, "setup", &_ic_452), gem_string("")))) {
#line 1029 "compiler/main.gem"
    GemVal _t3042 = gem_v_rc_r;
    static GemICacheSlot _ic_453 = {0};
    GemVal _t3043[] = {gem_table_get_cached(_t3042, "setup", &_ic_453), gem_string("    "), gem_string("        ")};
    GemVal _t3044[] = {gem_v_b, gem_str_replace_fn(NULL, _t3043, 3)};
                                                            (void)(gem_buf_push_fn(NULL, _t3044, 2));
                                                        }
#line 1031 "compiler/main.gem"
    GemVal _t3045 = gem_v_rc_r;
    static GemICacheSlot _ic_454 = {0};
    GemVal _t3046[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3045, "expr", &_ic_454), gem_string(";\n")};
    GemVal _t3047[] = {gem_v_b, gem_interp(5, _t3046)};
                                                        (void)(gem_buf_push_fn(NULL, _t3047, 2));
#line 1032 "compiler/main.gem"
    GemVal _t3048[] = {gem_v_b, gem_string("    }\n")};
                                                        (void)(gem_buf_push_fn(NULL, _t3048, 2));
#line 1033 "compiler/main.gem"
    GemVal _t3049 = gem_table_new();
    gem_table_set(_t3049, gem_string("expr"), gem_v_t);
    GemVal _t3050[] = {gem_v_b};
    gem_table_set(_t3049, gem_string("setup"), gem_buf_str_fn(NULL, _t3050, 1));
                                                        GemVal _t3051 = _t3049;
                                                        gem_pop_frame();
                                                        return _t3051;
                                                    } else {
#line 1034 "compiler/main.gem"
                                                        if (gem_truthy(gem_eq(gem_v_op, gem_string("or")))) {
#line 1035 "compiler/main.gem"
    GemVal _t3052 = gem_v_node;
    static GemICacheSlot _ic_455 = {0};
    GemVal _t3053[] = {gem_table_get_cached(_t3052, "right", &_ic_455)};
    GemVal _t3054 = (*gem_v_compile_expr);
                                                            GemVal gem_v_rc_r = _t3054.fn(_t3054.env, _t3053, 1);
#line 1036 "compiler/main.gem"
    GemVal _t3055 = (*gem_v_tmp);
                                                            GemVal gem_v_t = _t3055.fn(_t3055.env, NULL, 0);
#line 1037 "compiler/main.gem"
                                                            GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1038 "compiler/main.gem"
    GemVal _t3056[] = {gem_v_b, gem_v_ls};
                                                            (void)(gem_buf_push_fn(NULL, _t3056, 2));
#line 1039 "compiler/main.gem"
    GemVal _t3057[] = {gem_string("    GemVal "), gem_v_t, gem_string(";\n")};
    GemVal _t3058[] = {gem_v_b, gem_interp(3, _t3057)};
                                                            (void)(gem_buf_push_fn(NULL, _t3058, 2));
#line 1040 "compiler/main.gem"
    GemVal _t3059[] = {gem_string("    if (gem_truthy("), gem_v_lc, gem_string(")) {\n")};
    GemVal _t3060[] = {gem_v_b, gem_interp(3, _t3059)};
                                                            (void)(gem_buf_push_fn(NULL, _t3060, 2));
#line 1041 "compiler/main.gem"
    GemVal _t3061[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_v_lc, gem_string(";\n")};
    GemVal _t3062[] = {gem_v_b, gem_interp(5, _t3061)};
                                                            (void)(gem_buf_push_fn(NULL, _t3062, 2));
#line 1042 "compiler/main.gem"
    GemVal _t3063[] = {gem_v_b, gem_string("    } else {\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3063, 2));
#line 1043 "compiler/main.gem"
    GemVal _t3064 = gem_v_rc_r;
    static GemICacheSlot _ic_456 = {0};
                                                            if (gem_truthy(gem_neq(gem_table_get_cached(_t3064, "setup", &_ic_456), gem_string("")))) {
#line 1044 "compiler/main.gem"
    GemVal _t3065 = gem_v_rc_r;
    static GemICacheSlot _ic_457 = {0};
    GemVal _t3066[] = {gem_table_get_cached(_t3065, "setup", &_ic_457), gem_string("    "), gem_string("        ")};
    GemVal _t3067[] = {gem_v_b, gem_str_replace_fn(NULL, _t3066, 3)};
                                                                (void)(gem_buf_push_fn(NULL, _t3067, 2));
                                                            }
#line 1046 "compiler/main.gem"
    GemVal _t3068 = gem_v_rc_r;
    static GemICacheSlot _ic_458 = {0};
    GemVal _t3069[] = {gem_string("        "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3068, "expr", &_ic_458), gem_string(";\n")};
    GemVal _t3070[] = {gem_v_b, gem_interp(5, _t3069)};
                                                            (void)(gem_buf_push_fn(NULL, _t3070, 2));
#line 1047 "compiler/main.gem"
    GemVal _t3071[] = {gem_v_b, gem_string("    }\n")};
                                                            (void)(gem_buf_push_fn(NULL, _t3071, 2));
#line 1048 "compiler/main.gem"
    GemVal _t3072 = gem_table_new();
    gem_table_set(_t3072, gem_string("expr"), gem_v_t);
    GemVal _t3073[] = {gem_v_b};
    gem_table_set(_t3072, gem_string("setup"), gem_buf_str_fn(NULL, _t3073, 1));
                                                            GemVal _t3074 = _t3072;
                                                            gem_pop_frame();
                                                            return _t3074;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
#line 1051 "compiler/main.gem"
    GemVal _t3075[] = {gem_string("unknown binary operator: "), gem_v_op};
    GemVal _t3076[] = {gem_interp(2, _t3075)};
    GemVal _t3077 = gem_error_at_fn("compiler/main.gem", 1051, _t3076, 1);
    gem_pop_frame();
    return _t3077;
}

struct _closure__anon_48 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_stmt;
};
static GemVal _anon_48(void *_env, GemVal *args, int argc) {
    struct _closure__anon_48 *_cls = (struct _closure__anon_48 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_48", "compiler/main.gem", 0);
#line 1057 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1058 "compiler/main.gem"
    GemVal gem_v__for_items_58 = gem_v_stmts;
#line 1058 "compiler/main.gem"
    GemVal gem_v__for_i_58 = gem_int(0);
#line 1058 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t3079[] = {gem_v__for_items_58};
        if (!gem_truthy(gem_lt(gem_v__for_i_58, gem_len_fn(NULL, _t3079, 1)))) break;
#line 1058 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_58, gem_v__for_i_58);
#line 1058 "compiler/main.gem"
        gem_v__for_i_58 = gem_add(gem_v__for_i_58, gem_int(1));
#line 1059 "compiler/main.gem"
    GemVal _t3080[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3081 = (*gem_v_compile_stmt);
    GemVal _t3082[] = {gem_v_b, _t3081.fn(_t3081.env, _t3080, 2)};
        (void)(gem_buf_push_fn(NULL, _t3082, 2));
#line 1060 "compiler/main.gem"
    GemVal _t3083[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3083, 2));
    }

#line 1062 "compiler/main.gem"
    GemVal _t3084[] = {gem_v_b};
    GemVal _t3085 = gem_buf_str_fn(NULL, _t3084, 1);
    gem_pop_frame();
    return _t3085;
}

struct _closure__anon_49 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_49(void *_env, GemVal *args, int argc) {
    struct _closure__anon_49 *_cls = (struct _closure__anon_49 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_49", "compiler/main.gem", 0);
#line 1066 "compiler/main.gem"
    if (gem_truthy(gem_not(gem_v_return_pos))) {
#line 1067 "compiler/main.gem"
    GemVal _t3087[] = {gem_v_stmts, gem_v_indent};
    GemVal _t3088 = (*gem_v_compile_stmts);
        GemVal _t3089 = _t3088.fn(_t3088.env, _t3087, 2);
        gem_pop_frame();
        return _t3089;
    }
#line 1069 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1070 "compiler/main.gem"
    GemVal _t3090[] = {gem_v_stmts};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3090, 1), gem_int(0)))) {
#line 1071 "compiler/main.gem"
        GemVal gem_v__for_i_59 = gem_int(0);
#line 1071 "compiler/main.gem"
    GemVal _t3091[] = {gem_v_stmts};
        GemVal gem_v__for_limit_59 = gem_sub(gem_len_fn(NULL, _t3091, 1), gem_int(1));
#line 1071 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_59, gem_v__for_limit_59))) break;
#line 1071 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_59;
#line 1071 "compiler/main.gem"
            gem_v__for_i_59 = gem_add(gem_v__for_i_59, gem_int(1));
#line 1072 "compiler/main.gem"
    GemVal _t3092[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_v_indent};
    GemVal _t3093 = (*gem_v_compile_stmt);
    GemVal _t3094[] = {gem_v_b, _t3093.fn(_t3093.env, _t3092, 2)};
            (void)(gem_buf_push_fn(NULL, _t3094, 2));
#line 1073 "compiler/main.gem"
    GemVal _t3095[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3095, 2));
        }

#line 1075 "compiler/main.gem"
    GemVal _t3096[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_v_indent};
    GemVal _t3097 = (*gem_v_compile_stmt_return);
    GemVal _t3098[] = {gem_v_b, _t3097.fn(_t3097.env, _t3096, 2)};
        (void)(gem_buf_push_fn(NULL, _t3098, 2));
#line 1076 "compiler/main.gem"
    GemVal _t3099[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t3099, 2));
    } else {
#line 1078 "compiler/main.gem"
    GemVal _t3100[] = {gem_v_indent};
        GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3100, 1);
#line 1079 "compiler/main.gem"
    GemVal _t3101[] = {gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;\n")};
    GemVal _t3102[] = {gem_v_b, gem_interp(4, _t3101)};
        (void)(gem_buf_push_fn(NULL, _t3102, 2));
    }
#line 1081 "compiler/main.gem"
    GemVal _t3103[] = {gem_v_b};
    GemVal _t3104 = gem_buf_str_fn(NULL, _t3103, 1);
    gem_pop_frame();
    return _t3104;
}

struct _closure__anon_50 {
    intptr_t _num_captures;
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
static GemVal _anon_50(void *_env, GemVal *args, int argc) {
    struct _closure__anon_50 *_cls = (struct _closure__anon_50 *)_env;
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
    gem_push_frame("_anon_50", "compiler/main.gem", 0);
#line 1087 "compiler/main.gem"
    GemVal _t3106[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3106, 1);
#line 1088 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1089 "compiler/main.gem"
    GemVal _t3107 = gem_v_node;
    static GemICacheSlot _ic_459 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3107, "line", &_ic_459), GEM_NIL))) {
#line 1090 "compiler/main.gem"
    GemVal _t3108 = gem_v_node;
    static GemICacheSlot _ic_460 = {0};
    GemVal _t3109[] = {(*gem_v_source_name)};
    GemVal _t3110[] = {gem_string("#line "), gem_table_get_cached(_t3108, "line", &_ic_460), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3109, 1), gem_string("\"\n")};
        gem_v_line_dir = gem_interp(5, _t3110);
    }
#line 1093 "compiler/main.gem"
    GemVal gem_v__match_60 = gem_v_node;
    GemVal _t3111[] = {gem_v__match_60};
    GemVal _t3113;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3111, 1), gem_string("table")))) {
        _t3113 = gem_eq(gem_type_fn(NULL, _t3111, 1), gem_string("table"));
    } else {
        GemVal _t3112[] = {gem_v__match_60, gem_string("tag")};
        _t3113 = gem_has_key_fn(NULL, _t3112, 2);
    }
    GemVal _t3114;
    if (!gem_truthy(_t3113)) {
        _t3114 = _t3113;
    } else {
        _t3114 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3116;
    if (!gem_truthy(_t3114)) {
        _t3116 = _t3114;
    } else {
        GemVal _t3115[] = {gem_v__match_60, gem_string("name")};
        _t3116 = gem_has_key_fn(NULL, _t3115, 2);
    }
    GemVal _t3118;
    if (!gem_truthy(_t3116)) {
        _t3118 = _t3116;
    } else {
        GemVal _t3117[] = {gem_v__match_60, gem_string("value")};
        _t3118 = gem_has_key_fn(NULL, _t3117, 2);
    }
    if (gem_truthy(_t3118)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_60, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1095 "compiler/main.gem"
    GemVal _t3119[] = {gem_v_value};
    GemVal _t3120 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3120.fn(_t3120.env, _t3119, 1);
#line 1096 "compiler/main.gem"
    GemVal _t3121[] = {gem_v_name};
    GemVal _t3122 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3122.fn(_t3122.env, _t3121, 1);
#line 1097 "compiler/main.gem"
    GemVal _t3124;
    if (!gem_truthy((*gem_v_in_top_level))) {
        _t3124 = (*gem_v_in_top_level);
    } else {
        GemVal _t3123[] = {(*gem_v_top_level_vars), gem_v_name};
        _t3124 = gem_fn__mod_codegen_set_contains(NULL, _t3123, 2);
    }
        if (gem_truthy(_t3124)) {
#line 1098 "compiler/main.gem"
    GemVal _t3125[] = {(*gem_v_boxed_vars), gem_v_name};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3125, 2))) {
#line 1099 "compiler/main.gem"
    GemVal _t3126 = gem_v_r;
    static GemICacheSlot _ic_461 = {0};
    GemVal _t3127 = gem_v_r;
    static GemICacheSlot _ic_462 = {0};
    GemVal _t3128[] = {gem_v_line_dir, gem_table_get_cached(_t3126, "setup", &_ic_461), gem_v_p, gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3127, "expr", &_ic_462), gem_string(";")};
                GemVal _t3129 = gem_interp(11, _t3128);
                gem_pop_frame();
                return _t3129;
            }
#line 1101 "compiler/main.gem"
    GemVal _t3130 = gem_v_r;
    static GemICacheSlot _ic_463 = {0};
    GemVal _t3131 = gem_v_r;
    static GemICacheSlot _ic_464 = {0};
    GemVal _t3132[] = {gem_v_line_dir, gem_table_get_cached(_t3130, "setup", &_ic_463), gem_v_p, gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3131, "expr", &_ic_464), gem_string(";")};
            GemVal _t3133 = gem_interp(7, _t3132);
            gem_pop_frame();
            return _t3133;
        }
#line 1103 "compiler/main.gem"
    GemVal _t3134[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3134, 2))) {
#line 1104 "compiler/main.gem"
    GemVal _t3135 = gem_v_r;
    static GemICacheSlot _ic_465 = {0};
    GemVal _t3136 = gem_v_r;
    static GemICacheSlot _ic_466 = {0};
    GemVal _t3137[] = {gem_v_line_dir, gem_table_get_cached(_t3135, "setup", &_ic_465), gem_v_p, gem_string("GemVal *"), gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3136, "expr", &_ic_466), gem_string(";")};
            GemVal _t3138 = gem_interp(12, _t3137);
            gem_pop_frame();
            return _t3138;
        }
#line 1106 "compiler/main.gem"
    GemVal _t3139 = gem_v_r;
    static GemICacheSlot _ic_467 = {0};
    GemVal _t3140 = gem_v_r;
    static GemICacheSlot _ic_468 = {0};
    GemVal _t3141[] = {gem_v_line_dir, gem_table_get_cached(_t3139, "setup", &_ic_467), gem_v_p, gem_string("GemVal "), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3140, "expr", &_ic_468), gem_string(";")};
        GemVal _t3142 = gem_interp(8, _t3141);
        gem_pop_frame();
        return _t3142;
    } else {
    GemVal _t3143[] = {gem_v__match_60};
    GemVal _t3145;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3143, 1), gem_string("table")))) {
        _t3145 = gem_eq(gem_type_fn(NULL, _t3143, 1), gem_string("table"));
    } else {
        GemVal _t3144[] = {gem_v__match_60, gem_string("tag")};
        _t3145 = gem_has_key_fn(NULL, _t3144, 2);
    }
    GemVal _t3146;
    if (!gem_truthy(_t3145)) {
        _t3146 = _t3145;
    } else {
        _t3146 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3148;
    if (!gem_truthy(_t3146)) {
        _t3148 = _t3146;
    } else {
        GemVal _t3147[] = {gem_v__match_60, gem_string("name")};
        _t3148 = gem_has_key_fn(NULL, _t3147, 2);
    }
    GemVal _t3150;
    if (!gem_truthy(_t3148)) {
        _t3150 = _t3148;
    } else {
        GemVal _t3149[] = {gem_v__match_60, gem_string("value")};
        _t3150 = gem_has_key_fn(NULL, _t3149, 2);
    }
    if (gem_truthy(_t3150)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_60, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1108 "compiler/main.gem"
    GemVal _t3151[] = {gem_v_value};
    GemVal _t3152 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3152.fn(_t3152.env, _t3151, 1);
#line 1109 "compiler/main.gem"
    GemVal _t3153[] = {gem_v_name};
    GemVal _t3154 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3154.fn(_t3154.env, _t3153, 1);
#line 1110 "compiler/main.gem"
    GemVal _t3155[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3158;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3155, 2))) {
        _t3158 = gem_fn__mod_codegen_set_contains(NULL, _t3155, 2);
    } else {
        GemVal _t3157;
        if (!gem_truthy((*gem_v_in_top_level))) {
                _t3157 = (*gem_v_in_top_level);
        } else {
                GemVal _t3156[] = {(*gem_v_top_level_boxed), gem_v_name};
                _t3157 = gem_fn__mod_codegen_set_contains(NULL, _t3156, 2);
        }
        _t3158 = _t3157;
    }
        if (gem_truthy(_t3158)) {
#line 1111 "compiler/main.gem"
    GemVal _t3159 = gem_v_r;
    static GemICacheSlot _ic_469 = {0};
    GemVal _t3160 = gem_v_r;
    static GemICacheSlot _ic_470 = {0};
    GemVal _t3161[] = {gem_v_line_dir, gem_table_get_cached(_t3159, "setup", &_ic_469), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3160, "expr", &_ic_470), gem_string(";")};
            GemVal _t3162 = gem_interp(8, _t3161);
            gem_pop_frame();
            return _t3162;
        }
#line 1113 "compiler/main.gem"
    GemVal _t3163 = gem_v_r;
    static GemICacheSlot _ic_471 = {0};
    GemVal _t3164 = gem_v_r;
    static GemICacheSlot _ic_472 = {0};
    GemVal _t3165[] = {gem_v_line_dir, gem_table_get_cached(_t3163, "setup", &_ic_471), gem_v_p, gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3164, "expr", &_ic_472), gem_string(";")};
        GemVal _t3166 = gem_interp(7, _t3165);
        gem_pop_frame();
        return _t3166;
    } else {
    GemVal _t3167[] = {gem_v__match_60};
    GemVal _t3169;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3167, 1), gem_string("table")))) {
        _t3169 = gem_eq(gem_type_fn(NULL, _t3167, 1), gem_string("table"));
    } else {
        GemVal _t3168[] = {gem_v__match_60, gem_string("tag")};
        _t3169 = gem_has_key_fn(NULL, _t3168, 2);
    }
    GemVal _t3170;
    if (!gem_truthy(_t3169)) {
        _t3170 = _t3169;
    } else {
        _t3170 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t3172;
    if (!gem_truthy(_t3170)) {
        _t3172 = _t3170;
    } else {
        GemVal _t3171[] = {gem_v__match_60, gem_string("object")};
        _t3172 = gem_has_key_fn(NULL, _t3171, 2);
    }
    GemVal _t3174;
    if (!gem_truthy(_t3172)) {
        _t3174 = _t3172;
    } else {
        GemVal _t3173[] = {gem_v__match_60, gem_string("value")};
        _t3174 = gem_has_key_fn(NULL, _t3173, 2);
    }
    GemVal _t3176;
    if (!gem_truthy(_t3174)) {
        _t3176 = _t3174;
    } else {
        GemVal _t3175[] = {gem_v__match_60, gem_string("field")};
        _t3176 = gem_has_key_fn(NULL, _t3175, 2);
    }
    if (gem_truthy(_t3176)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_60, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 0 "compiler/main.gem"
        GemVal gem_v_field = gem_table_get(gem_v__match_60, gem_string("field"));
#line 1115 "compiler/main.gem"
    GemVal _t3177[] = {gem_v_object};
    GemVal _t3178 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3178.fn(_t3178.env, _t3177, 1);
#line 1116 "compiler/main.gem"
    GemVal _t3179[] = {gem_v_value};
    GemVal _t3180 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3180.fn(_t3180.env, _t3179, 1);
#line 1117 "compiler/main.gem"
    GemVal _t3181[] = {gem_v_field};
        GemVal gem_v_escaped = gem_fn__mod_codegen_escape_c_string(NULL, _t3181, 1);
#line 1118 "compiler/main.gem"
    GemVal _t3182 = gem_v_obj_r;
    static GemICacheSlot _ic_473 = {0};
    GemVal _t3183 = gem_v_val_r;
    static GemICacheSlot _ic_474 = {0};
    GemVal _t3184 = gem_v_obj_r;
    static GemICacheSlot _ic_475 = {0};
    GemVal _t3185 = gem_v_val_r;
    static GemICacheSlot _ic_476 = {0};
    GemVal _t3186[] = {gem_v_line_dir, gem_table_get_cached(_t3182, "setup", &_ic_473), gem_table_get_cached(_t3183, "setup", &_ic_474), gem_v_p, gem_string("gem_table_set("), gem_table_get_cached(_t3184, "expr", &_ic_475), gem_string(", gem_string(\""), gem_v_escaped, gem_string("\"), "), gem_table_get_cached(_t3185, "expr", &_ic_476), gem_string(");")};
        GemVal _t3187 = gem_interp(11, _t3186);
        gem_pop_frame();
        return _t3187;
    } else {
    GemVal _t3188[] = {gem_v__match_60};
    GemVal _t3190;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3188, 1), gem_string("table")))) {
        _t3190 = gem_eq(gem_type_fn(NULL, _t3188, 1), gem_string("table"));
    } else {
        GemVal _t3189[] = {gem_v__match_60, gem_string("tag")};
        _t3190 = gem_has_key_fn(NULL, _t3189, 2);
    }
    GemVal _t3191;
    if (!gem_truthy(_t3190)) {
        _t3191 = _t3190;
    } else {
        _t3191 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t3193;
    if (!gem_truthy(_t3191)) {
        _t3193 = _t3191;
    } else {
        GemVal _t3192[] = {gem_v__match_60, gem_string("object")};
        _t3193 = gem_has_key_fn(NULL, _t3192, 2);
    }
    GemVal _t3195;
    if (!gem_truthy(_t3193)) {
        _t3195 = _t3193;
    } else {
        GemVal _t3194[] = {gem_v__match_60, gem_string("key")};
        _t3195 = gem_has_key_fn(NULL, _t3194, 2);
    }
    GemVal _t3197;
    if (!gem_truthy(_t3195)) {
        _t3197 = _t3195;
    } else {
        GemVal _t3196[] = {gem_v__match_60, gem_string("value")};
        _t3197 = gem_has_key_fn(NULL, _t3196, 2);
    }
    if (gem_truthy(_t3197)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_object = gem_table_get(gem_v__match_60, gem_string("object"));
#line 0 "compiler/main.gem"
        GemVal gem_v_key = gem_table_get(gem_v__match_60, gem_string("key"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1120 "compiler/main.gem"
    GemVal _t3198[] = {gem_v_object};
    GemVal _t3199 = (*gem_v_compile_expr);
        GemVal gem_v_obj_r = _t3199.fn(_t3199.env, _t3198, 1);
#line 1121 "compiler/main.gem"
    GemVal _t3200[] = {gem_v_key};
    GemVal _t3201 = (*gem_v_compile_expr);
        GemVal gem_v_key_r = _t3201.fn(_t3201.env, _t3200, 1);
#line 1122 "compiler/main.gem"
    GemVal _t3202[] = {gem_v_value};
    GemVal _t3203 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3203.fn(_t3203.env, _t3202, 1);
#line 1123 "compiler/main.gem"
    GemVal _t3204 = gem_v_obj_r;
    static GemICacheSlot _ic_477 = {0};
    GemVal _t3205 = gem_v_key_r;
    static GemICacheSlot _ic_478 = {0};
    GemVal _t3206 = gem_v_val_r;
    static GemICacheSlot _ic_479 = {0};
    GemVal _t3207 = gem_v_obj_r;
    static GemICacheSlot _ic_480 = {0};
    GemVal _t3208 = gem_v_key_r;
    static GemICacheSlot _ic_481 = {0};
    GemVal _t3209 = gem_v_val_r;
    static GemICacheSlot _ic_482 = {0};
    GemVal _t3210[] = {gem_v_line_dir, gem_table_get_cached(_t3204, "setup", &_ic_477), gem_table_get_cached(_t3205, "setup", &_ic_478), gem_table_get_cached(_t3206, "setup", &_ic_479), gem_v_p, gem_string("gem_table_set("), gem_table_get_cached(_t3207, "expr", &_ic_480), gem_string(", "), gem_table_get_cached(_t3208, "expr", &_ic_481), gem_string(", "), gem_table_get_cached(_t3209, "expr", &_ic_482), gem_string(");")};
        GemVal _t3211 = gem_interp(12, _t3210);
        gem_pop_frame();
        return _t3211;
    } else {
    GemVal _t3212[] = {gem_v__match_60};
    GemVal _t3214;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3212, 1), gem_string("table")))) {
        _t3214 = gem_eq(gem_type_fn(NULL, _t3212, 1), gem_string("table"));
    } else {
        GemVal _t3213[] = {gem_v__match_60, gem_string("tag")};
        _t3214 = gem_has_key_fn(NULL, _t3213, 2);
    }
    GemVal _t3215;
    if (!gem_truthy(_t3214)) {
        _t3215 = _t3214;
    } else {
        _t3215 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3215)) {
#line 1125 "compiler/main.gem"
    GemVal _t3216[] = {gem_v_line_dir};
    GemVal _t3217[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3218 = (*gem_v_compile_if);
        GemVal _t3219 = gem_add(gem_interp(1, _t3216), _t3218.fn(_t3218.env, _t3217, 3));
        gem_pop_frame();
        return _t3219;
    } else {
    GemVal _t3220[] = {gem_v__match_60};
    GemVal _t3222;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3220, 1), gem_string("table")))) {
        _t3222 = gem_eq(gem_type_fn(NULL, _t3220, 1), gem_string("table"));
    } else {
        GemVal _t3221[] = {gem_v__match_60, gem_string("tag")};
        _t3222 = gem_has_key_fn(NULL, _t3221, 2);
    }
    GemVal _t3223;
    if (!gem_truthy(_t3222)) {
        _t3223 = _t3222;
    } else {
        _t3223 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3223)) {
#line 1127 "compiler/main.gem"
    GemVal _t3224[] = {gem_v_line_dir};
    GemVal _t3225[] = {gem_v_node, gem_v_indent};
    GemVal _t3226 = (*gem_v_compile_while);
        GemVal _t3227 = gem_add(gem_interp(1, _t3224), _t3226.fn(_t3226.env, _t3225, 2));
        gem_pop_frame();
        return _t3227;
    } else {
    GemVal _t3228[] = {gem_v__match_60};
    GemVal _t3230;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3228, 1), gem_string("table")))) {
        _t3230 = gem_eq(gem_type_fn(NULL, _t3228, 1), gem_string("table"));
    } else {
        GemVal _t3229[] = {gem_v__match_60, gem_string("tag")};
        _t3230 = gem_has_key_fn(NULL, _t3229, 2);
    }
    GemVal _t3231;
    if (!gem_truthy(_t3230)) {
        _t3231 = _t3230;
    } else {
        _t3231 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3231)) {
#line 1129 "compiler/main.gem"
    GemVal _t3232[] = {gem_v_line_dir};
    GemVal _t3233[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3234 = (*gem_v_compile_match);
        GemVal _t3235 = gem_add(gem_interp(1, _t3232), _t3234.fn(_t3234.env, _t3233, 3));
        gem_pop_frame();
        return _t3235;
    } else {
    GemVal _t3236[] = {gem_v__match_60};
    GemVal _t3238;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3236, 1), gem_string("table")))) {
        _t3238 = gem_eq(gem_type_fn(NULL, _t3236, 1), gem_string("table"));
    } else {
        GemVal _t3237[] = {gem_v__match_60, gem_string("tag")};
        _t3238 = gem_has_key_fn(NULL, _t3237, 2);
    }
    GemVal _t3239;
    if (!gem_truthy(_t3238)) {
        _t3239 = _t3238;
    } else {
        _t3239 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3239)) {
#line 1131 "compiler/main.gem"
    GemVal _t3240[] = {gem_v_line_dir};
    GemVal _t3241[] = {gem_v_node, gem_v_indent, gem_bool(0)};
    GemVal _t3242 = (*gem_v_compile_receive_match);
        GemVal _t3243 = gem_add(gem_interp(1, _t3240), _t3242.fn(_t3242.env, _t3241, 3));
        gem_pop_frame();
        return _t3243;
    } else {
    GemVal _t3244[] = {gem_v__match_60};
    GemVal _t3246;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3244, 1), gem_string("table")))) {
        _t3246 = gem_eq(gem_type_fn(NULL, _t3244, 1), gem_string("table"));
    } else {
        GemVal _t3245[] = {gem_v__match_60, gem_string("tag")};
        _t3246 = gem_has_key_fn(NULL, _t3245, 2);
    }
    GemVal _t3247;
    if (!gem_truthy(_t3246)) {
        _t3247 = _t3246;
    } else {
        _t3247 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3247)) {
#line 1133 "compiler/main.gem"
        GemVal _t3248 = gem_add(gem_v_p, gem_string("break;"));
        gem_pop_frame();
        return _t3248;
    } else {
    GemVal _t3249[] = {gem_v__match_60};
    GemVal _t3251;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3249, 1), gem_string("table")))) {
        _t3251 = gem_eq(gem_type_fn(NULL, _t3249, 1), gem_string("table"));
    } else {
        GemVal _t3250[] = {gem_v__match_60, gem_string("tag")};
        _t3251 = gem_has_key_fn(NULL, _t3250, 2);
    }
    GemVal _t3252;
    if (!gem_truthy(_t3251)) {
        _t3252 = _t3251;
    } else {
        _t3252 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3252)) {
#line 1135 "compiler/main.gem"
        GemVal _t3253 = gem_add(gem_v_p, gem_string("continue;"));
        gem_pop_frame();
        return _t3253;
    } else {
    GemVal _t3254[] = {gem_v__match_60};
    GemVal _t3256;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3254, 1), gem_string("table")))) {
        _t3256 = gem_eq(gem_type_fn(NULL, _t3254, 1), gem_string("table"));
    } else {
        GemVal _t3255[] = {gem_v__match_60, gem_string("tag")};
        _t3256 = gem_has_key_fn(NULL, _t3255, 2);
    }
    GemVal _t3257;
    if (!gem_truthy(_t3256)) {
        _t3257 = _t3256;
    } else {
        _t3257 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3259;
    if (!gem_truthy(_t3257)) {
        _t3259 = _t3257;
    } else {
        GemVal _t3258[] = {gem_v__match_60, gem_string("value")};
        _t3259 = gem_has_key_fn(NULL, _t3258, 2);
    }
    if (gem_truthy(_t3259)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_60, gem_string("value"));
#line 1137 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1138 "compiler/main.gem"
    GemVal _t3260[] = {gem_v_value};
    GemVal _t3261 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3261.fn(_t3261.env, _t3260, 1);
#line 1139 "compiler/main.gem"
    GemVal _t3262 = (*gem_v_tmp);
            GemVal gem_v_t = _t3262.fn(_t3262.env, NULL, 0);
#line 1140 "compiler/main.gem"
    GemVal _t3263 = gem_v_r;
    static GemICacheSlot _ic_483 = {0};
    GemVal _t3264 = gem_v_r;
    static GemICacheSlot _ic_484 = {0};
    GemVal _t3265[] = {gem_v_line_dir, gem_table_get_cached(_t3263, "setup", &_ic_483), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3264, "expr", &_ic_484), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3266 = gem_interp(14, _t3265);
            gem_pop_frame();
            return _t3266;
        }
#line 1142 "compiler/main.gem"
    GemVal _t3267[] = {gem_v_line_dir, gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3268 = gem_interp(5, _t3267);
        gem_pop_frame();
        return _t3268;
    } else {
    GemVal _t3269[] = {gem_v__match_60};
    GemVal _t3271;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3269, 1), gem_string("table")))) {
        _t3271 = gem_eq(gem_type_fn(NULL, _t3269, 1), gem_string("table"));
    } else {
        GemVal _t3270[] = {gem_v__match_60, gem_string("tag")};
        _t3271 = gem_has_key_fn(NULL, _t3270, 2);
    }
    GemVal _t3272;
    if (!gem_truthy(_t3271)) {
        _t3272 = _t3271;
    } else {
        _t3272 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("call"));
    }
    if (gem_truthy(_t3272)) {
#line 1144 "compiler/main.gem"
    GemVal _t3273[] = {gem_v_node};
    GemVal _t3274 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3274.fn(_t3274.env, _t3273, 1);
#line 1145 "compiler/main.gem"
    GemVal _t3275 = gem_v_r;
    static GemICacheSlot _ic_485 = {0};
    GemVal _t3276 = gem_v_r;
    static GemICacheSlot _ic_486 = {0};
    GemVal _t3277[] = {gem_v_line_dir, gem_table_get_cached(_t3275, "setup", &_ic_485), gem_v_p, gem_string("(void)("), gem_table_get_cached(_t3276, "expr", &_ic_486), gem_string(");")};
        GemVal _t3278 = gem_interp(6, _t3277);
        gem_pop_frame();
        return _t3278;
    } else {
    GemVal _t3279[] = {gem_v__match_60};
    GemVal _t3281;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3279, 1), gem_string("table")))) {
        _t3281 = gem_eq(gem_type_fn(NULL, _t3279, 1), gem_string("table"));
    } else {
        GemVal _t3280[] = {gem_v__match_60, gem_string("tag")};
        _t3281 = gem_has_key_fn(NULL, _t3280, 2);
    }
    GemVal _t3282;
    if (!gem_truthy(_t3281)) {
        _t3282 = _t3281;
    } else {
        _t3282 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3282)) {
#line 1147 "compiler/main.gem"
        GemVal _t3283 = gem_string("");
        gem_pop_frame();
        return _t3283;
    } else {
    GemVal _t3284[] = {gem_v__match_60};
    GemVal _t3286;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3284, 1), gem_string("table")))) {
        _t3286 = gem_eq(gem_type_fn(NULL, _t3284, 1), gem_string("table"));
    } else {
        GemVal _t3285[] = {gem_v__match_60, gem_string("tag")};
        _t3286 = gem_has_key_fn(NULL, _t3285, 2);
    }
    GemVal _t3287;
    if (!gem_truthy(_t3286)) {
        _t3287 = _t3286;
    } else {
        _t3287 = gem_eq(gem_table_get(gem_v__match_60, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3289;
    if (!gem_truthy(_t3287)) {
        _t3289 = _t3287;
    } else {
        GemVal _t3288[] = {gem_v__match_60, gem_string("stmts")};
        _t3289 = gem_has_key_fn(NULL, _t3288, 2);
    }
    if (gem_truthy(_t3289)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_60, gem_string("stmts"));
#line 1149 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1150 "compiler/main.gem"
        GemVal gem_v__for_items_61 = gem_v_stmts;
#line 1150 "compiler/main.gem"
        GemVal gem_v__for_i_61 = gem_int(0);
#line 1150 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t3290[] = {gem_v__for_items_61};
            if (!gem_truthy(gem_lt(gem_v__for_i_61, gem_len_fn(NULL, _t3290, 1)))) break;
#line 1150 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_61, gem_v__for_i_61);
#line 1150 "compiler/main.gem"
            gem_v__for_i_61 = gem_add(gem_v__for_i_61, gem_int(1));
#line 1151 "compiler/main.gem"
    GemVal _t3291[] = {gem_v_stmt, gem_v_indent};
    GemVal _t3292 = (*gem_v_compile_stmt);
    GemVal _t3293[] = {gem_v_b, _t3292.fn(_t3292.env, _t3291, 2)};
            (void)(gem_buf_push_fn(NULL, _t3293, 2));
#line 1152 "compiler/main.gem"
    GemVal _t3294[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3294, 2));
        }

#line 1154 "compiler/main.gem"
    GemVal _t3295[] = {gem_v_b};
        GemVal _t3296 = gem_buf_str_fn(NULL, _t3295, 1);
        gem_pop_frame();
        return _t3296;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1157 "compiler/main.gem"
    GemVal _t3297[] = {gem_v_node};
    GemVal _t3298 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3298.fn(_t3298.env, _t3297, 1);
    GemVal _t3299 = gem_v_r;
    static GemICacheSlot _ic_487 = {0};
    GemVal _t3300 = gem_v_r;
    static GemICacheSlot _ic_488 = {0};
    GemVal _t3301[] = {gem_v_line_dir, gem_table_get_cached(_t3299, "setup", &_ic_487), gem_v_p, gem_string("(void)("), gem_table_get_cached(_t3300, "expr", &_ic_488), gem_string(");")};
    GemVal _t3302 = gem_interp(6, _t3301);
    gem_pop_frame();
    return _t3302;
}

struct _closure__anon_51 {
    intptr_t _num_captures;
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
static GemVal _anon_51(void *_env, GemVal *args, int argc) {
    struct _closure__anon_51 *_cls = (struct _closure__anon_51 *)_env;
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
    gem_push_frame("_anon_51", "compiler/main.gem", 0);
#line 1164 "compiler/main.gem"
    GemVal _t3304[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3304, 1);
#line 1165 "compiler/main.gem"
    GemVal gem_v_line_dir = gem_string("");
#line 1166 "compiler/main.gem"
    GemVal _t3305 = gem_v_node;
    static GemICacheSlot _ic_489 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3305, "line", &_ic_489), GEM_NIL))) {
#line 1167 "compiler/main.gem"
    GemVal _t3306 = gem_v_node;
    static GemICacheSlot _ic_490 = {0};
    GemVal _t3307[] = {(*gem_v_source_name)};
    GemVal _t3308[] = {gem_string("#line "), gem_table_get_cached(_t3306, "line", &_ic_490), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t3307, 1), gem_string("\"\n")};
        gem_v_line_dir = gem_interp(5, _t3308);
    }
#line 1170 "compiler/main.gem"
    if (gem_truthy(gem_neq((*gem_v_tco_fn_name), GEM_NIL))) {
#line 1171 "compiler/main.gem"
    GemVal _t3309[] = {gem_v_node, gem_string("call")};
    GemVal _t3312;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3309, 2))) {
        _t3312 = gem_fn__mod_codegen_is_node(NULL, _t3309, 2);
    } else {
        GemVal _t3310 = gem_v_node;
        static GemICacheSlot _ic_491 = {0};
        GemVal _t3311[] = {gem_table_get_cached(_t3310, "func", &_ic_491), gem_string("var")};
        _t3312 = gem_fn__mod_codegen_is_node(NULL, _t3311, 2);
    }
    GemVal _t3315;
    if (!gem_truthy(_t3312)) {
        _t3315 = _t3312;
    } else {
        GemVal _t3313 = gem_v_node;
        static GemICacheSlot _ic_492 = {0};
        GemVal _t3314 = gem_table_get_cached(_t3313, "func", &_ic_492);
        static GemICacheSlot _ic_493 = {0};
        _t3315 = gem_eq(gem_table_get_cached(_t3314, "name", &_ic_493), (*gem_v_tco_fn_name));
    }
    GemVal _t3317;
    if (!gem_truthy(_t3315)) {
        _t3317 = _t3315;
    } else {
        GemVal _t3316[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3317 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3316, 2));
    }
        if (gem_truthy(_t3317)) {
#line 1172 "compiler/main.gem"
    GemVal _t3318[] = {gem_v_node, gem_v_indent};
    GemVal _t3319 = (*gem_v_emit_tco_continue);
            GemVal _t3320 = gem_add(gem_v_line_dir, _t3319.fn(_t3319.env, _t3318, 2));
            gem_pop_frame();
            return _t3320;
        }
#line 1174 "compiler/main.gem"
    GemVal _t3321[] = {gem_v_node, gem_string("return")};
    GemVal _t3323;
    if (!gem_truthy(gem_fn__mod_codegen_is_node(NULL, _t3321, 2))) {
        _t3323 = gem_fn__mod_codegen_is_node(NULL, _t3321, 2);
    } else {
        GemVal _t3322 = gem_v_node;
        static GemICacheSlot _ic_494 = {0};
        _t3323 = gem_neq(gem_table_get_cached(_t3322, "value", &_ic_494), GEM_NIL);
    }
    GemVal _t3326;
    if (!gem_truthy(_t3323)) {
        _t3326 = _t3323;
    } else {
        GemVal _t3324 = gem_v_node;
        static GemICacheSlot _ic_495 = {0};
        GemVal _t3325[] = {gem_table_get_cached(_t3324, "value", &_ic_495), gem_string("call")};
        _t3326 = gem_fn__mod_codegen_is_node(NULL, _t3325, 2);
    }
    GemVal _t3330;
    if (!gem_truthy(_t3326)) {
        _t3330 = _t3326;
    } else {
        GemVal _t3327 = gem_v_node;
        static GemICacheSlot _ic_496 = {0};
        GemVal _t3328 = gem_table_get_cached(_t3327, "value", &_ic_496);
        static GemICacheSlot _ic_497 = {0};
        GemVal _t3329[] = {gem_table_get_cached(_t3328, "func", &_ic_497), gem_string("var")};
        _t3330 = gem_fn__mod_codegen_is_node(NULL, _t3329, 2);
    }
    GemVal _t3334;
    if (!gem_truthy(_t3330)) {
        _t3334 = _t3330;
    } else {
        GemVal _t3331 = gem_v_node;
        static GemICacheSlot _ic_498 = {0};
        GemVal _t3332 = gem_table_get_cached(_t3331, "value", &_ic_498);
        static GemICacheSlot _ic_499 = {0};
        GemVal _t3333 = gem_table_get_cached(_t3332, "func", &_ic_499);
        static GemICacheSlot _ic_500 = {0};
        _t3334 = gem_eq(gem_table_get_cached(_t3333, "name", &_ic_500), (*gem_v_tco_fn_name));
    }
    GemVal _t3336;
    if (!gem_truthy(_t3334)) {
        _t3336 = _t3334;
    } else {
        GemVal _t3335[] = {(*gem_v_local_names), (*gem_v_tco_fn_name)};
        _t3336 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t3335, 2));
    }
        if (gem_truthy(_t3336)) {
#line 1175 "compiler/main.gem"
    GemVal _t3337 = gem_v_node;
    static GemICacheSlot _ic_501 = {0};
    GemVal _t3338[] = {gem_table_get_cached(_t3337, "value", &_ic_501), gem_v_indent};
    GemVal _t3339 = (*gem_v_emit_tco_continue);
            GemVal _t3340 = gem_add(gem_v_line_dir, _t3339.fn(_t3339.env, _t3338, 2));
            gem_pop_frame();
            return _t3340;
        }
    }
#line 1179 "compiler/main.gem"
    GemVal gem_v__match_62 = gem_v_node;
    GemVal _t3341[] = {gem_v__match_62};
    GemVal _t3343;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3341, 1), gem_string("table")))) {
        _t3343 = gem_eq(gem_type_fn(NULL, _t3341, 1), gem_string("table"));
    } else {
        GemVal _t3342[] = {gem_v__match_62, gem_string("tag")};
        _t3343 = gem_has_key_fn(NULL, _t3342, 2);
    }
    GemVal _t3344;
    if (!gem_truthy(_t3343)) {
        _t3344 = _t3343;
    } else {
        _t3344 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("let"));
    }
    GemVal _t3346;
    if (!gem_truthy(_t3344)) {
        _t3346 = _t3344;
    } else {
        GemVal _t3345[] = {gem_v__match_62, gem_string("name")};
        _t3346 = gem_has_key_fn(NULL, _t3345, 2);
    }
    GemVal _t3348;
    if (!gem_truthy(_t3346)) {
        _t3348 = _t3346;
    } else {
        GemVal _t3347[] = {gem_v__match_62, gem_string("value")};
        _t3348 = gem_has_key_fn(NULL, _t3347, 2);
    }
    if (gem_truthy(_t3348)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_62, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_62, gem_string("value"));
#line 1181 "compiler/main.gem"
    GemVal _t3349[] = {gem_v_value};
    GemVal _t3350 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3350.fn(_t3350.env, _t3349, 1);
#line 1182 "compiler/main.gem"
    GemVal _t3351[] = {gem_v_name};
    GemVal _t3352 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3352.fn(_t3352.env, _t3351, 1);
#line 1183 "compiler/main.gem"
    GemVal _t3353[] = {(*gem_v_boxed_vars), gem_v_name};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3353, 2))) {
#line 1184 "compiler/main.gem"
    GemVal _t3354 = gem_v_r;
    static GemICacheSlot _ic_502 = {0};
    GemVal _t3355 = gem_v_r;
    static GemICacheSlot _ic_503 = {0};
    GemVal _t3356[] = {gem_v_line_dir, gem_table_get_cached(_t3354, "setup", &_ic_502), gem_v_p, gem_string("GemVal *"), gem_v_mname, gem_string(" = GC_MALLOC(sizeof(GemVal));\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3355, "expr", &_ic_503), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
            GemVal _t3357 = gem_interp(16, _t3356);
            gem_pop_frame();
            return _t3357;
        }
#line 1186 "compiler/main.gem"
    GemVal _t3358 = gem_v_r;
    static GemICacheSlot _ic_504 = {0};
    GemVal _t3359 = gem_v_r;
    static GemICacheSlot _ic_505 = {0};
    GemVal _t3360[] = {gem_v_line_dir, gem_table_get_cached(_t3358, "setup", &_ic_504), gem_v_p, gem_string("GemVal "), gem_v_mname, gem_string(" = "), gem_table_get_cached(_t3359, "expr", &_ic_505), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3361 = gem_interp(12, _t3360);
        gem_pop_frame();
        return _t3361;
    } else {
    GemVal _t3362[] = {gem_v__match_62};
    GemVal _t3364;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3362, 1), gem_string("table")))) {
        _t3364 = gem_eq(gem_type_fn(NULL, _t3362, 1), gem_string("table"));
    } else {
        GemVal _t3363[] = {gem_v__match_62, gem_string("tag")};
        _t3364 = gem_has_key_fn(NULL, _t3363, 2);
    }
    GemVal _t3365;
    if (!gem_truthy(_t3364)) {
        _t3365 = _t3364;
    } else {
        _t3365 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t3367;
    if (!gem_truthy(_t3365)) {
        _t3367 = _t3365;
    } else {
        GemVal _t3366[] = {gem_v__match_62, gem_string("name")};
        _t3367 = gem_has_key_fn(NULL, _t3366, 2);
    }
    GemVal _t3369;
    if (!gem_truthy(_t3367)) {
        _t3369 = _t3367;
    } else {
        GemVal _t3368[] = {gem_v__match_62, gem_string("value")};
        _t3369 = gem_has_key_fn(NULL, _t3368, 2);
    }
    if (gem_truthy(_t3369)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_name = gem_table_get(gem_v__match_62, gem_string("name"));
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_62, gem_string("value"));
#line 1188 "compiler/main.gem"
    GemVal _t3370[] = {gem_v_value};
    GemVal _t3371 = (*gem_v_compile_expr);
        GemVal gem_v_r = _t3371.fn(_t3371.env, _t3370, 1);
#line 1189 "compiler/main.gem"
    GemVal _t3372 = (*gem_v_tmp);
        GemVal gem_v_t = _t3372.fn(_t3372.env, NULL, 0);
#line 1190 "compiler/main.gem"
    GemVal _t3373[] = {gem_v_name};
    GemVal _t3374 = (*gem_v_mangle);
        GemVal gem_v_mname = _t3374.fn(_t3374.env, _t3373, 1);
#line 1191 "compiler/main.gem"
    GemVal _t3375[] = {(*gem_v_boxed_vars), gem_v_name};
    GemVal _t3378;
    if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3375, 2))) {
        _t3378 = gem_fn__mod_codegen_set_contains(NULL, _t3375, 2);
    } else {
        GemVal _t3377;
        if (!gem_truthy((*gem_v_in_top_level))) {
                _t3377 = (*gem_v_in_top_level);
        } else {
                GemVal _t3376[] = {(*gem_v_top_level_boxed), gem_v_name};
                _t3377 = gem_fn__mod_codegen_set_contains(NULL, _t3376, 2);
        }
        _t3378 = _t3377;
    }
        if (gem_truthy(_t3378)) {
#line 1192 "compiler/main.gem"
    GemVal _t3379 = gem_v_r;
    static GemICacheSlot _ic_506 = {0};
    GemVal _t3380 = gem_v_r;
    static GemICacheSlot _ic_507 = {0};
    GemVal _t3381[] = {gem_v_line_dir, gem_table_get_cached(_t3379, "setup", &_ic_506), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3380, "expr", &_ic_507), gem_string(";\n"), gem_v_p, gem_string("*"), gem_v_mname, gem_string(" = "), gem_v_t, gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3382 = gem_interp(20, _t3381);
            gem_pop_frame();
            return _t3382;
        }
#line 1194 "compiler/main.gem"
    GemVal _t3383 = gem_v_r;
    static GemICacheSlot _ic_508 = {0};
    GemVal _t3384 = gem_v_r;
    static GemICacheSlot _ic_509 = {0};
    GemVal _t3385[] = {gem_v_line_dir, gem_table_get_cached(_t3383, "setup", &_ic_508), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3384, "expr", &_ic_509), gem_string(";\n"), gem_v_p, gem_v_mname, gem_string(" = "), gem_v_t, gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
        GemVal _t3386 = gem_interp(19, _t3385);
        gem_pop_frame();
        return _t3386;
    } else {
    GemVal _t3387[] = {gem_v__match_62};
    GemVal _t3389;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3387, 1), gem_string("table")))) {
        _t3389 = gem_eq(gem_type_fn(NULL, _t3387, 1), gem_string("table"));
    } else {
        GemVal _t3388[] = {gem_v__match_62, gem_string("tag")};
        _t3389 = gem_has_key_fn(NULL, _t3388, 2);
    }
    GemVal _t3390;
    if (!gem_truthy(_t3389)) {
        _t3390 = _t3389;
    } else {
        _t3390 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("return"));
    }
    GemVal _t3392;
    if (!gem_truthy(_t3390)) {
        _t3392 = _t3390;
    } else {
        GemVal _t3391[] = {gem_v__match_62, gem_string("value")};
        _t3392 = gem_has_key_fn(NULL, _t3391, 2);
    }
    if (gem_truthy(_t3392)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_value = gem_table_get(gem_v__match_62, gem_string("value"));
#line 1196 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 1197 "compiler/main.gem"
    GemVal _t3393[] = {gem_v_value};
    GemVal _t3394 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3394.fn(_t3394.env, _t3393, 1);
#line 1198 "compiler/main.gem"
    GemVal _t3395 = (*gem_v_tmp);
            GemVal gem_v_t = _t3395.fn(_t3395.env, NULL, 0);
#line 1199 "compiler/main.gem"
    GemVal _t3396 = gem_v_r;
    static GemICacheSlot _ic_510 = {0};
    GemVal _t3397 = gem_v_r;
    static GemICacheSlot _ic_511 = {0};
    GemVal _t3398[] = {gem_v_line_dir, gem_table_get_cached(_t3396, "setup", &_ic_510), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3397, "expr", &_ic_511), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
            GemVal _t3399 = gem_interp(14, _t3398);
            gem_pop_frame();
            return _t3399;
        }
#line 1201 "compiler/main.gem"
    GemVal _t3400[] = {gem_v_line_dir, gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3401 = gem_interp(5, _t3400);
        gem_pop_frame();
        return _t3401;
    } else {
    GemVal _t3402[] = {gem_v__match_62};
    GemVal _t3404;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3402, 1), gem_string("table")))) {
        _t3404 = gem_eq(gem_type_fn(NULL, _t3402, 1), gem_string("table"));
    } else {
        GemVal _t3403[] = {gem_v__match_62, gem_string("tag")};
        _t3404 = gem_has_key_fn(NULL, _t3403, 2);
    }
    GemVal _t3405;
    if (!gem_truthy(_t3404)) {
        _t3405 = _t3404;
    } else {
        _t3405 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("if"));
    }
    if (gem_truthy(_t3405)) {
#line 1203 "compiler/main.gem"
    GemVal _t3406[] = {gem_v_line_dir};
    GemVal _t3407[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3408 = (*gem_v_compile_if);
        GemVal _t3409 = gem_add(gem_interp(1, _t3406), _t3408.fn(_t3408.env, _t3407, 3));
        gem_pop_frame();
        return _t3409;
    } else {
    GemVal _t3410[] = {gem_v__match_62};
    GemVal _t3412;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3410, 1), gem_string("table")))) {
        _t3412 = gem_eq(gem_type_fn(NULL, _t3410, 1), gem_string("table"));
    } else {
        GemVal _t3411[] = {gem_v__match_62, gem_string("tag")};
        _t3412 = gem_has_key_fn(NULL, _t3411, 2);
    }
    GemVal _t3413;
    if (!gem_truthy(_t3412)) {
        _t3413 = _t3412;
    } else {
        _t3413 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("while"));
    }
    if (gem_truthy(_t3413)) {
#line 1205 "compiler/main.gem"
    GemVal _t3414[] = {gem_v_node, gem_v_indent};
    GemVal _t3415 = (*gem_v_compile_while);
    GemVal _t3416[] = {gem_v_line_dir, _t3415.fn(_t3415.env, _t3414, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3417 = gem_interp(7, _t3416);
        gem_pop_frame();
        return _t3417;
    } else {
    GemVal _t3418[] = {gem_v__match_62};
    GemVal _t3420;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3418, 1), gem_string("table")))) {
        _t3420 = gem_eq(gem_type_fn(NULL, _t3418, 1), gem_string("table"));
    } else {
        GemVal _t3419[] = {gem_v__match_62, gem_string("tag")};
        _t3420 = gem_has_key_fn(NULL, _t3419, 2);
    }
    GemVal _t3421;
    if (!gem_truthy(_t3420)) {
        _t3421 = _t3420;
    } else {
        _t3421 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("match"));
    }
    if (gem_truthy(_t3421)) {
#line 1207 "compiler/main.gem"
    GemVal _t3422[] = {gem_v_line_dir};
    GemVal _t3423[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3424 = (*gem_v_compile_match);
        GemVal _t3425 = gem_add(gem_interp(1, _t3422), _t3424.fn(_t3424.env, _t3423, 3));
        gem_pop_frame();
        return _t3425;
    } else {
    GemVal _t3426[] = {gem_v__match_62};
    GemVal _t3428;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3426, 1), gem_string("table")))) {
        _t3428 = gem_eq(gem_type_fn(NULL, _t3426, 1), gem_string("table"));
    } else {
        GemVal _t3427[] = {gem_v__match_62, gem_string("tag")};
        _t3428 = gem_has_key_fn(NULL, _t3427, 2);
    }
    GemVal _t3429;
    if (!gem_truthy(_t3428)) {
        _t3429 = _t3428;
    } else {
        _t3429 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("receive_match"));
    }
    if (gem_truthy(_t3429)) {
#line 1209 "compiler/main.gem"
    GemVal _t3430[] = {gem_v_line_dir};
    GemVal _t3431[] = {gem_v_node, gem_v_indent, gem_bool(1)};
    GemVal _t3432 = (*gem_v_compile_receive_match);
        GemVal _t3433 = gem_add(gem_interp(1, _t3430), _t3432.fn(_t3432.env, _t3431, 3));
        gem_pop_frame();
        return _t3433;
    } else {
    GemVal _t3434[] = {gem_v__match_62};
    GemVal _t3436;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3434, 1), gem_string("table")))) {
        _t3436 = gem_eq(gem_type_fn(NULL, _t3434, 1), gem_string("table"));
    } else {
        GemVal _t3435[] = {gem_v__match_62, gem_string("tag")};
        _t3436 = gem_has_key_fn(NULL, _t3435, 2);
    }
    GemVal _t3437;
    if (!gem_truthy(_t3436)) {
        _t3437 = _t3436;
    } else {
        _t3437 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("block"));
    }
    GemVal _t3439;
    if (!gem_truthy(_t3437)) {
        _t3439 = _t3437;
    } else {
        GemVal _t3438[] = {gem_v__match_62, gem_string("stmts")};
        _t3439 = gem_has_key_fn(NULL, _t3438, 2);
    }
    if (gem_truthy(_t3439)) {
#line 0 "compiler/main.gem"
        GemVal gem_v_stmts = gem_table_get(gem_v__match_62, gem_string("stmts"));
#line 1211 "compiler/main.gem"
        GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1212 "compiler/main.gem"
    GemVal _t3440[] = {gem_v_p, gem_string("{\n")};
    GemVal _t3441[] = {gem_v_b, gem_interp(2, _t3440)};
        (void)(gem_buf_push_fn(NULL, _t3441, 2));
#line 1213 "compiler/main.gem"
        GemVal gem_v__for_i_63 = gem_int(0);
#line 1213 "compiler/main.gem"
    GemVal _t3442[] = {gem_v_stmts};
        GemVal gem_v__for_limit_63 = gem_sub(gem_len_fn(NULL, _t3442, 1), gem_int(1));
#line 1213 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_63, gem_v__for_limit_63))) break;
#line 1213 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_63;
#line 1213 "compiler/main.gem"
            gem_v__for_i_63 = gem_add(gem_v__for_i_63, gem_int(1));
#line 1214 "compiler/main.gem"
    GemVal _t3443[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3444 = (*gem_v_compile_stmt);
    GemVal _t3445[] = {gem_v_b, _t3444.fn(_t3444.env, _t3443, 2)};
            (void)(gem_buf_push_fn(NULL, _t3445, 2));
#line 1215 "compiler/main.gem"
    GemVal _t3446[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3446, 2));
        }

#line 1217 "compiler/main.gem"
    GemVal _t3447[] = {gem_v_stmts};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t3447, 1), gem_int(0)))) {
#line 1218 "compiler/main.gem"
    GemVal _t3448[] = {gem_table_get(gem_v_stmts, gem_neg(gem_int(1))), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3449 = (*gem_v_compile_stmt_return);
    GemVal _t3450[] = {gem_v_b, _t3449.fn(_t3449.env, _t3448, 2)};
            (void)(gem_buf_push_fn(NULL, _t3450, 2));
#line 1219 "compiler/main.gem"
    GemVal _t3451[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3451, 2));
        }
#line 1221 "compiler/main.gem"
    GemVal _t3452[] = {gem_v_p, gem_string("}")};
    GemVal _t3453[] = {gem_v_b, gem_interp(2, _t3452)};
        (void)(gem_buf_push_fn(NULL, _t3453, 2));
#line 1222 "compiler/main.gem"
    GemVal _t3454[] = {gem_v_b};
        GemVal _t3455 = gem_buf_str_fn(NULL, _t3454, 1);
        gem_pop_frame();
        return _t3455;
    } else {
    GemVal _t3456[] = {gem_v__match_62};
    GemVal _t3458;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3456, 1), gem_string("table")))) {
        _t3458 = gem_eq(gem_type_fn(NULL, _t3456, 1), gem_string("table"));
    } else {
        GemVal _t3457[] = {gem_v__match_62, gem_string("tag")};
        _t3458 = gem_has_key_fn(NULL, _t3457, 2);
    }
    GemVal _t3459;
    if (!gem_truthy(_t3458)) {
        _t3459 = _t3458;
    } else {
        _t3459 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("dot_assign"));
    }
    if (gem_truthy(_t3459)) {
#line 1224 "compiler/main.gem"
    GemVal _t3460[] = {gem_v_node, gem_v_indent};
    GemVal _t3461 = (*gem_v_compile_stmt);
    GemVal _t3462[] = {_t3461.fn(_t3461.env, _t3460, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3463 = gem_interp(6, _t3462);
        gem_pop_frame();
        return _t3463;
    } else {
    GemVal _t3464[] = {gem_v__match_62};
    GemVal _t3466;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3464, 1), gem_string("table")))) {
        _t3466 = gem_eq(gem_type_fn(NULL, _t3464, 1), gem_string("table"));
    } else {
        GemVal _t3465[] = {gem_v__match_62, gem_string("tag")};
        _t3466 = gem_has_key_fn(NULL, _t3465, 2);
    }
    GemVal _t3467;
    if (!gem_truthy(_t3466)) {
        _t3467 = _t3466;
    } else {
        _t3467 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("index_assign"));
    }
    if (gem_truthy(_t3467)) {
#line 1226 "compiler/main.gem"
    GemVal _t3468[] = {gem_v_node, gem_v_indent};
    GemVal _t3469 = (*gem_v_compile_stmt);
    GemVal _t3470[] = {_t3469.fn(_t3469.env, _t3468, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3471 = gem_interp(6, _t3470);
        gem_pop_frame();
        return _t3471;
    } else {
    GemVal _t3472[] = {gem_v__match_62};
    GemVal _t3474;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3472, 1), gem_string("table")))) {
        _t3474 = gem_eq(gem_type_fn(NULL, _t3472, 1), gem_string("table"));
    } else {
        GemVal _t3473[] = {gem_v__match_62, gem_string("tag")};
        _t3474 = gem_has_key_fn(NULL, _t3473, 2);
    }
    GemVal _t3475;
    if (!gem_truthy(_t3474)) {
        _t3475 = _t3474;
    } else {
        _t3475 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("break"));
    }
    if (gem_truthy(_t3475)) {
#line 1228 "compiler/main.gem"
    GemVal _t3476[] = {gem_v_node, gem_v_indent};
    GemVal _t3477 = (*gem_v_compile_stmt);
    GemVal _t3478[] = {_t3477.fn(_t3477.env, _t3476, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3479 = gem_interp(6, _t3478);
        gem_pop_frame();
        return _t3479;
    } else {
    GemVal _t3480[] = {gem_v__match_62};
    GemVal _t3482;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3480, 1), gem_string("table")))) {
        _t3482 = gem_eq(gem_type_fn(NULL, _t3480, 1), gem_string("table"));
    } else {
        GemVal _t3481[] = {gem_v__match_62, gem_string("tag")};
        _t3482 = gem_has_key_fn(NULL, _t3481, 2);
    }
    GemVal _t3483;
    if (!gem_truthy(_t3482)) {
        _t3483 = _t3482;
    } else {
        _t3483 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("continue"));
    }
    if (gem_truthy(_t3483)) {
#line 1230 "compiler/main.gem"
    GemVal _t3484[] = {gem_v_node, gem_v_indent};
    GemVal _t3485 = (*gem_v_compile_stmt);
    GemVal _t3486[] = {_t3485.fn(_t3485.env, _t3484, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3487 = gem_interp(6, _t3486);
        gem_pop_frame();
        return _t3487;
    } else {
    GemVal _t3488[] = {gem_v__match_62};
    GemVal _t3490;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t3488, 1), gem_string("table")))) {
        _t3490 = gem_eq(gem_type_fn(NULL, _t3488, 1), gem_string("table"));
    } else {
        GemVal _t3489[] = {gem_v__match_62, gem_string("tag")};
        _t3490 = gem_has_key_fn(NULL, _t3489, 2);
    }
    GemVal _t3491;
    if (!gem_truthy(_t3490)) {
        _t3491 = _t3490;
    } else {
        _t3491 = gem_eq(gem_table_get(gem_v__match_62, gem_string("tag")), gem_string("fn_def"));
    }
    if (gem_truthy(_t3491)) {
#line 1232 "compiler/main.gem"
    GemVal _t3492[] = {gem_v_node, gem_v_indent};
    GemVal _t3493 = (*gem_v_compile_stmt);
    GemVal _t3494[] = {_t3493.fn(_t3493.env, _t3492, 2), gem_string("\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;")};
        GemVal _t3495 = gem_interp(6, _t3494);
        gem_pop_frame();
        return _t3495;
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
    }
#line 1235 "compiler/main.gem"
    GemVal _t3496[] = {gem_v_node};
    GemVal _t3497 = (*gem_v_compile_expr);
    GemVal gem_v_r = _t3497.fn(_t3497.env, _t3496, 1);
#line 1236 "compiler/main.gem"
    GemVal _t3498 = (*gem_v_tmp);
    GemVal gem_v_t = _t3498.fn(_t3498.env, NULL, 0);
    GemVal _t3499 = gem_v_r;
    static GemICacheSlot _ic_512 = {0};
    GemVal _t3500 = gem_v_r;
    static GemICacheSlot _ic_513 = {0};
    GemVal _t3501[] = {gem_v_line_dir, gem_table_get_cached(_t3499, "setup", &_ic_512), gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3500, "expr", &_ic_513), gem_string(";\n"), gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return "), gem_v_t, gem_string(";")};
    GemVal _t3502 = gem_interp(14, _t3501);
    gem_pop_frame();
    return _t3502;
}

struct _closure__anon_52 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts_tail;
};
static GemVal _anon_52(void *_env, GemVal *args, int argc) {
    struct _closure__anon_52 *_cls = (struct _closure__anon_52 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_52", "compiler/main.gem", 0);
#line 1243 "compiler/main.gem"
    GemVal _t3504[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3504, 1);
#line 1244 "compiler/main.gem"
    GemVal _t3505 = gem_v_node;
    static GemICacheSlot _ic_514 = {0};
    GemVal _t3506[] = {gem_table_get_cached(_t3505, "cond", &_ic_514)};
    GemVal _t3507 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3507.fn(_t3507.env, _t3506, 1);
#line 1245 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1246 "compiler/main.gem"
    GemVal _t3508 = gem_v_cond_r;
    static GemICacheSlot _ic_515 = {0};
    GemVal _t3509[] = {gem_v_b, gem_table_get_cached(_t3508, "setup", &_ic_515)};
    (void)(gem_buf_push_fn(NULL, _t3509, 2));
#line 1247 "compiler/main.gem"
    GemVal _t3510 = gem_v_cond_r;
    static GemICacheSlot _ic_516 = {0};
    GemVal _t3511[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get_cached(_t3510, "expr", &_ic_516), gem_string(")) {\n")};
    GemVal _t3512[] = {gem_v_b, gem_interp(4, _t3511)};
    (void)(gem_buf_push_fn(NULL, _t3512, 2));
#line 1248 "compiler/main.gem"
    GemVal _t3513 = gem_v_node;
    static GemICacheSlot _ic_517 = {0};
    GemVal _t3514[] = {gem_table_get_cached(_t3513, "then", &_ic_517), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3515 = (*gem_v_compile_stmts_tail);
    GemVal _t3516[] = {gem_v_b, _t3515.fn(_t3515.env, _t3514, 3)};
    (void)(gem_buf_push_fn(NULL, _t3516, 2));
#line 1249 "compiler/main.gem"
    GemVal _t3517 = gem_v_node;
    static GemICacheSlot _ic_518 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3517, "else", &_ic_518), GEM_NIL))) {
#line 1250 "compiler/main.gem"
    GemVal _t3518[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3519[] = {gem_v_b, gem_interp(2, _t3518)};
        (void)(gem_buf_push_fn(NULL, _t3519, 2));
#line 1251 "compiler/main.gem"
    GemVal _t3520 = gem_v_node;
    static GemICacheSlot _ic_519 = {0};
    GemVal _t3521[] = {gem_table_get_cached(_t3520, "else", &_ic_519), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3522 = (*gem_v_compile_stmts_tail);
    GemVal _t3523[] = {gem_v_b, _t3522.fn(_t3522.env, _t3521, 3)};
        (void)(gem_buf_push_fn(NULL, _t3523, 2));
    } else {
#line 1252 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1253 "compiler/main.gem"
    GemVal _t3524[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3525[] = {gem_v_b, gem_interp(2, _t3524)};
            (void)(gem_buf_push_fn(NULL, _t3525, 2));
#line 1254 "compiler/main.gem"
    GemVal _t3526 = gem_table_new();
    GemVal _t3527[] = {_t3526, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3528 = (*gem_v_compile_stmts_tail);
    GemVal _t3529[] = {gem_v_b, _t3528.fn(_t3528.env, _t3527, 3)};
            (void)(gem_buf_push_fn(NULL, _t3529, 2));
        }
    }
#line 1256 "compiler/main.gem"
    GemVal _t3530[] = {gem_v_p, gem_string("}")};
    GemVal _t3531[] = {gem_v_b, gem_interp(2, _t3530)};
    (void)(gem_buf_push_fn(NULL, _t3531, 2));
#line 1257 "compiler/main.gem"
    GemVal _t3532[] = {gem_v_b};
    GemVal _t3533 = gem_buf_str_fn(NULL, _t3532, 1);
    gem_pop_frame();
    return _t3533;
}

struct _closure__anon_53 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmts;
};
static GemVal _anon_53(void *_env, GemVal *args, int argc) {
    struct _closure__anon_53 *_cls = (struct _closure__anon_53 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmts = _cls->gem_v_compile_stmts;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("_anon_53", "compiler/main.gem", 0);
#line 1263 "compiler/main.gem"
    GemVal _t3535[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3535, 1);
#line 1264 "compiler/main.gem"
    GemVal _t3536 = gem_v_node;
    static GemICacheSlot _ic_520 = {0};
    GemVal _t3537[] = {gem_table_get_cached(_t3536, "cond", &_ic_520)};
    GemVal _t3538 = (*gem_v_compile_expr);
    GemVal gem_v_cond_r = _t3538.fn(_t3538.env, _t3537, 1);
#line 1265 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1266 "compiler/main.gem"
    GemVal _t3539[] = {gem_v_p, gem_string("while (1) {\n")};
    GemVal _t3540[] = {gem_v_b, gem_interp(2, _t3539)};
    (void)(gem_buf_push_fn(NULL, _t3540, 2));
#line 1267 "compiler/main.gem"
    GemVal _t3541[] = {gem_v_p, gem_string("    gem_yield_check();\n")};
    GemVal _t3542[] = {gem_v_b, gem_interp(2, _t3541)};
    (void)(gem_buf_push_fn(NULL, _t3542, 2));
#line 1268 "compiler/main.gem"
    GemVal _t3543 = gem_v_cond_r;
    static GemICacheSlot _ic_521 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3543, "setup", &_ic_521), gem_string("")))) {
#line 1269 "compiler/main.gem"
    GemVal _t3544 = gem_v_cond_r;
    static GemICacheSlot _ic_522 = {0};
    GemVal _t3545[] = {gem_v_p, gem_string("    ")};
    GemVal _t3546[] = {gem_table_get_cached(_t3544, "setup", &_ic_522), gem_string("    "), gem_interp(2, _t3545)};
    GemVal _t3547[] = {gem_v_b, gem_str_replace_fn(NULL, _t3546, 3)};
        (void)(gem_buf_push_fn(NULL, _t3547, 2));
    }
#line 1271 "compiler/main.gem"
    GemVal _t3548 = gem_v_cond_r;
    static GemICacheSlot _ic_523 = {0};
    GemVal _t3549[] = {gem_v_p, gem_string("    if (!gem_truthy("), gem_table_get_cached(_t3548, "expr", &_ic_523), gem_string(")) break;\n")};
    GemVal _t3550[] = {gem_v_b, gem_interp(4, _t3549)};
    (void)(gem_buf_push_fn(NULL, _t3550, 2));
#line 1272 "compiler/main.gem"
    GemVal _t3551 = gem_v_node;
    static GemICacheSlot _ic_524 = {0};
    GemVal _t3552[] = {gem_table_get_cached(_t3551, "body", &_ic_524), gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3553 = (*gem_v_compile_stmts);
    GemVal _t3554[] = {gem_v_b, _t3553.fn(_t3553.env, _t3552, 2)};
    (void)(gem_buf_push_fn(NULL, _t3554, 2));
#line 1273 "compiler/main.gem"
    GemVal _t3555[] = {gem_v_p, gem_string("}")};
    GemVal _t3556[] = {gem_v_b, gem_interp(2, _t3555)};
    (void)(gem_buf_push_fn(NULL, _t3556, 2));
#line 1274 "compiler/main.gem"
    GemVal _t3557[] = {gem_v_b};
    GemVal _t3558 = gem_buf_str_fn(NULL, _t3557, 1);
    gem_pop_frame();
    return _t3558;
}

struct _closure__anon_54 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_54(void *_env, GemVal *args, int argc) {
    struct _closure__anon_54 *_cls = (struct _closure__anon_54 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_54", "compiler/main.gem", 0);
#line 1280 "compiler/main.gem"
    GemVal _t3560[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3560, 1);
#line 1281 "compiler/main.gem"
    GemVal _t3561 = gem_v_node;
    static GemICacheSlot _ic_525 = {0};
    GemVal _t3562[] = {gem_table_get_cached(_t3561, "target", &_ic_525)};
    GemVal _t3563 = (*gem_v_compile_expr);
    GemVal gem_v_target_r = _t3563.fn(_t3563.env, _t3562, 1);
#line 1282 "compiler/main.gem"
    GemVal gem_v_t = GEM_NIL;
#line 1283 "compiler/main.gem"
    GemVal _t3564 = gem_v_node;
    static GemICacheSlot _ic_526 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3564, "target_var", &_ic_526), GEM_NIL))) {
#line 1284 "compiler/main.gem"
    GemVal _t3565 = gem_v_node;
    static GemICacheSlot _ic_527 = {0};
    GemVal _t3566[] = {gem_table_get_cached(_t3565, "target_var", &_ic_527)};
    GemVal _t3567 = (*gem_v_mangle);
        gem_v_t = _t3567.fn(_t3567.env, _t3566, 1);
    } else {
#line 1286 "compiler/main.gem"
    GemVal _t3568 = (*gem_v_tmp);
        gem_v_t = _t3568.fn(_t3568.env, NULL, 0);
    }
#line 1288 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1289 "compiler/main.gem"
    GemVal _t3569 = gem_v_target_r;
    static GemICacheSlot _ic_528 = {0};
    GemVal _t3570[] = {gem_v_b, gem_table_get_cached(_t3569, "setup", &_ic_528)};
    (void)(gem_buf_push_fn(NULL, _t3570, 2));
#line 1290 "compiler/main.gem"
    GemVal _t3571 = gem_v_target_r;
    static GemICacheSlot _ic_529 = {0};
    GemVal _t3572[] = {gem_v_p, gem_string("GemVal "), gem_v_t, gem_string(" = "), gem_table_get_cached(_t3571, "expr", &_ic_529), gem_string(";\n")};
    GemVal _t3573[] = {gem_v_b, gem_interp(6, _t3572)};
    (void)(gem_buf_push_fn(NULL, _t3573, 2));
#line 1291 "compiler/main.gem"
    GemVal _t3574 = gem_v_node;
    static GemICacheSlot _ic_530 = {0};
    GemVal _t3575[] = {gem_table_get_cached(_t3574, "whens", &_ic_530)};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3575, 1), gem_int(0)))) {
#line 1292 "compiler/main.gem"
    GemVal _t3576 = gem_v_node;
    static GemICacheSlot _ic_531 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t3576, "else", &_ic_531), GEM_NIL))) {
#line 1293 "compiler/main.gem"
    GemVal _t3577 = gem_v_node;
    static GemICacheSlot _ic_532 = {0};
    GemVal _t3578[] = {gem_table_get_cached(_t3577, "else", &_ic_532), gem_v_indent, gem_v_return_pos};
    GemVal _t3579 = (*gem_v_compile_stmts_tail);
    GemVal _t3580[] = {gem_v_b, _t3579.fn(_t3579.env, _t3578, 3)};
            (void)(gem_buf_push_fn(NULL, _t3580, 2));
        } else {
#line 1294 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1295 "compiler/main.gem"
    GemVal _t3581[] = {gem_v_p, gem_string("gem_pop_frame();\n"), gem_v_p, gem_string("return GEM_NIL;\n")};
    GemVal _t3582[] = {gem_v_b, gem_interp(4, _t3581)};
                (void)(gem_buf_push_fn(NULL, _t3582, 2));
            }
        }
#line 1297 "compiler/main.gem"
    GemVal _t3583[] = {gem_v_p, gem_string("(void)"), gem_v_t, gem_string(";")};
    GemVal _t3584[] = {gem_v_b, gem_interp(4, _t3583)};
        (void)(gem_buf_push_fn(NULL, _t3584, 2));
#line 1298 "compiler/main.gem"
    GemVal _t3585[] = {gem_v_b};
        GemVal _t3586 = gem_buf_str_fn(NULL, _t3585, 1);
        gem_pop_frame();
        return _t3586;
    }
#line 1300 "compiler/main.gem"
    GemVal gem_v__for_i_65 = gem_int(0);
#line 1300 "compiler/main.gem"
    GemVal _t3587 = gem_v_node;
    static GemICacheSlot _ic_533 = {0};
    GemVal _t3588[] = {gem_table_get_cached(_t3587, "whens", &_ic_533)};
    GemVal gem_v__for_limit_65 = gem_len_fn(NULL, _t3588, 1);
#line 1300 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_65, gem_v__for_limit_65))) break;
#line 1300 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_65;
#line 1300 "compiler/main.gem"
        gem_v__for_i_65 = gem_add(gem_v__for_i_65, gem_int(1));
#line 1301 "compiler/main.gem"
    GemVal _t3589 = gem_v_node;
    static GemICacheSlot _ic_534 = {0};
        GemVal gem_v_w = gem_table_get(gem_table_get_cached(_t3589, "whens", &_ic_534), gem_v_i);
#line 1302 "compiler/main.gem"
    GemVal _t3590 = gem_v_w;
    static GemICacheSlot _ic_535 = {0};
    GemVal _t3591[] = {gem_table_get_cached(_t3590, "value", &_ic_535)};
    GemVal _t3592 = (*gem_v_compile_expr);
        GemVal gem_v_val_r = _t3592.fn(_t3592.env, _t3591, 1);
#line 1303 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1304 "compiler/main.gem"
    GemVal _t3593 = gem_v_val_r;
    static GemICacheSlot _ic_536 = {0};
    GemVal _t3594[] = {gem_v_b, gem_table_get_cached(_t3593, "setup", &_ic_536)};
            (void)(gem_buf_push_fn(NULL, _t3594, 2));
#line 1305 "compiler/main.gem"
    GemVal _t3595 = gem_v_val_r;
    static GemICacheSlot _ic_537 = {0};
    GemVal _t3596[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get_cached(_t3595, "expr", &_ic_537), gem_string(")) {\n")};
    GemVal _t3597[] = {gem_v_b, gem_interp(4, _t3596)};
            (void)(gem_buf_push_fn(NULL, _t3597, 2));
        } else {
#line 1307 "compiler/main.gem"
    GemVal _t3598[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3599[] = {gem_v_b, gem_interp(2, _t3598)};
            (void)(gem_buf_push_fn(NULL, _t3599, 2));
#line 1308 "compiler/main.gem"
    GemVal _t3600 = gem_v_val_r;
    static GemICacheSlot _ic_538 = {0};
    GemVal _t3601[] = {gem_v_b, gem_table_get_cached(_t3600, "setup", &_ic_538)};
            (void)(gem_buf_push_fn(NULL, _t3601, 2));
#line 1309 "compiler/main.gem"
    GemVal _t3602 = gem_v_val_r;
    static GemICacheSlot _ic_539 = {0};
    GemVal _t3603[] = {gem_v_p, gem_string("if (gem_truthy("), gem_table_get_cached(_t3602, "expr", &_ic_539), gem_string(")) {\n")};
    GemVal _t3604[] = {gem_v_b, gem_interp(4, _t3603)};
            (void)(gem_buf_push_fn(NULL, _t3604, 2));
        }
#line 1311 "compiler/main.gem"
    GemVal _t3605 = gem_v_w;
    static GemICacheSlot _ic_540 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t3605, "bindings", &_ic_540), GEM_NIL))) {
#line 1312 "compiler/main.gem"
    GemVal _t3606 = gem_v_w;
    static GemICacheSlot _ic_541 = {0};
            GemVal gem_v__for_items_64 = gem_table_get_cached(_t3606, "bindings", &_ic_541);
#line 1312 "compiler/main.gem"
            GemVal gem_v__for_i_64 = gem_int(0);
#line 1312 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3607[] = {gem_v__for_items_64};
                if (!gem_truthy(gem_lt(gem_v__for_i_64, gem_len_fn(NULL, _t3607, 1)))) break;
#line 1312 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_64, gem_v__for_i_64);
#line 1312 "compiler/main.gem"
                gem_v__for_i_64 = gem_add(gem_v__for_i_64, gem_int(1));
#line 1313 "compiler/main.gem"
    GemVal _t3608[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3609 = (*gem_v_compile_stmt);
    GemVal _t3610[] = {gem_v_b, _t3609.fn(_t3609.env, _t3608, 2)};
                (void)(gem_buf_push_fn(NULL, _t3610, 2));
#line 1314 "compiler/main.gem"
    GemVal _t3611[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3611, 2));
            }

        }
#line 1317 "compiler/main.gem"
    GemVal _t3612 = gem_v_w;
    static GemICacheSlot _ic_542 = {0};
    GemVal _t3613[] = {gem_table_get_cached(_t3612, "body", &_ic_542), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3614 = (*gem_v_compile_stmts_tail);
    GemVal _t3615[] = {gem_v_b, _t3614.fn(_t3614.env, _t3613, 3)};
        (void)(gem_buf_push_fn(NULL, _t3615, 2));
    }

#line 1319 "compiler/main.gem"
    GemVal _t3616 = gem_v_node;
    static GemICacheSlot _ic_543 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t3616, "else", &_ic_543), GEM_NIL))) {
#line 1320 "compiler/main.gem"
    GemVal _t3617[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3618[] = {gem_v_b, gem_interp(2, _t3617)};
        (void)(gem_buf_push_fn(NULL, _t3618, 2));
#line 1321 "compiler/main.gem"
    GemVal _t3619 = gem_v_node;
    static GemICacheSlot _ic_544 = {0};
    GemVal _t3620[] = {gem_table_get_cached(_t3619, "else", &_ic_544), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3621 = (*gem_v_compile_stmts_tail);
    GemVal _t3622[] = {gem_v_b, _t3621.fn(_t3621.env, _t3620, 3)};
        (void)(gem_buf_push_fn(NULL, _t3622, 2));
    } else {
#line 1322 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1323 "compiler/main.gem"
    GemVal _t3623[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3624[] = {gem_v_b, gem_interp(2, _t3623)};
            (void)(gem_buf_push_fn(NULL, _t3624, 2));
#line 1324 "compiler/main.gem"
    GemVal _t3625 = gem_table_new();
    GemVal _t3626[] = {_t3625, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3627 = (*gem_v_compile_stmts_tail);
    GemVal _t3628[] = {gem_v_b, _t3627.fn(_t3627.env, _t3626, 3)};
            (void)(gem_buf_push_fn(NULL, _t3628, 2));
        }
    }
#line 1326 "compiler/main.gem"
    GemVal _t3629[] = {gem_v_p, gem_string("}")};
    GemVal _t3630[] = {gem_v_b, gem_interp(2, _t3629)};
    (void)(gem_buf_push_fn(NULL, _t3630, 2));
#line 1327 "compiler/main.gem"
    GemVal gem_v__for_i_66 = gem_int(1);
#line 1327 "compiler/main.gem"
    GemVal _t3631 = gem_v_node;
    static GemICacheSlot _ic_545 = {0};
    GemVal _t3632[] = {gem_table_get_cached(_t3631, "whens", &_ic_545)};
    GemVal gem_v__for_limit_66 = gem_len_fn(NULL, _t3632, 1);
#line 1327 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_66, gem_v__for_limit_66))) break;
#line 1327 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_66;
#line 1327 "compiler/main.gem"
        gem_v__for_i_66 = gem_add(gem_v__for_i_66, gem_int(1));
#line 1328 "compiler/main.gem"
    GemVal _t3633[] = {gem_string("\n"), gem_v_p, gem_string("}")};
    GemVal _t3634[] = {gem_v_b, gem_interp(3, _t3633)};
        (void)(gem_buf_push_fn(NULL, _t3634, 2));
    }

#line 1330 "compiler/main.gem"
    GemVal _t3635[] = {gem_v_b};
    GemVal _t3636 = gem_buf_str_fn(NULL, _t3635, 1);
    gem_pop_frame();
    return _t3636;
}

struct _closure__anon_55 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle;
    GemVal *gem_v_tmp;
};
static GemVal _anon_55(void *_env, GemVal *args, int argc) {
    struct _closure__anon_55 *_cls = (struct _closure__anon_55 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmts_tail = _cls->gem_v_compile_stmts_tail;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal *gem_v_tmp = _cls->gem_v_tmp;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_indent = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_return_pos = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_55", "compiler/main.gem", 0);
#line 1336 "compiler/main.gem"
    GemVal _t3638[] = {gem_v_indent};
    GemVal gem_v_p = gem_fn__mod_codegen_pad_for(NULL, _t3638, 1);
#line 1337 "compiler/main.gem"
    GemVal _t3639 = gem_v_node;
    static GemICacheSlot _ic_546 = {0};
    GemVal gem_v_arms = gem_table_get_cached(_t3639, "arms", &_ic_546);
#line 1338 "compiler/main.gem"
    GemVal _t3640 = gem_v_node;
    static GemICacheSlot _ic_547 = {0};
    GemVal gem_v_after_ms = gem_table_get_cached(_t3640, "after_ms", &_ic_547);
#line 1339 "compiler/main.gem"
    GemVal _t3641 = gem_v_node;
    static GemICacheSlot _ic_548 = {0};
    GemVal gem_v_after_body = gem_table_get_cached(_t3641, "after_body", &_ic_548);
#line 1341 "compiler/main.gem"
    GemVal _t3642 = gem_v_node;
    static GemICacheSlot _ic_549 = {0};
    GemVal _t3643[] = {gem_table_get_cached(_t3642, "recv_var", &_ic_549)};
    GemVal _t3644 = (*gem_v_mangle);
    GemVal gem_v_recv_tmp = _t3644.fn(_t3644.env, _t3643, 1);
#line 1342 "compiler/main.gem"
    GemVal _t3645 = (*gem_v_tmp);
    GemVal gem_v_matched_tmp = _t3645.fn(_t3645.env, NULL, 0);
#line 1343 "compiler/main.gem"
    GemVal _t3646 = (*gem_v_tmp);
    GemVal gem_v_prev_tmp = _t3646.fn(_t3646.env, NULL, 0);
#line 1344 "compiler/main.gem"
    GemVal _t3647 = (*gem_v_tmp);
    GemVal gem_v_cur_tmp = _t3647.fn(_t3647.env, NULL, 0);
#line 1345 "compiler/main.gem"
    GemVal _t3648 = (*gem_v_tmp);
    GemVal gem_v_deadline_tmp = _t3648.fn(_t3648.env, NULL, 0);
#line 1346 "compiler/main.gem"
    GemVal _t3649 = (*gem_v_tmp);
    GemVal gem_v_proc_tmp = _t3649.fn(_t3649.env, NULL, 0);
#line 1348 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1350 "compiler/main.gem"
    GemVal _t3650[] = {gem_v_p, gem_string("GemVal "), gem_v_recv_tmp, gem_string(" = GEM_NIL;\n")};
    GemVal _t3651[] = {gem_v_b, gem_interp(4, _t3650)};
    (void)(gem_buf_push_fn(NULL, _t3651, 2));
#line 1351 "compiler/main.gem"
    GemVal _t3652[] = {gem_v_p, gem_string("int "), gem_v_matched_tmp, gem_string(" = -1;\n")};
    GemVal _t3653[] = {gem_v_b, gem_interp(4, _t3652)};
    (void)(gem_buf_push_fn(NULL, _t3653, 2));
#line 1353 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1354 "compiler/main.gem"
    GemVal _t3654[] = {gem_v_after_ms};
    GemVal _t3655 = (*gem_v_compile_expr);
        GemVal gem_v_ms_r = _t3655.fn(_t3655.env, _t3654, 1);
#line 1355 "compiler/main.gem"
    GemVal _t3656 = gem_v_ms_r;
    static GemICacheSlot _ic_550 = {0};
    GemVal _t3657[] = {gem_v_b, gem_table_get_cached(_t3656, "setup", &_ic_550)};
        (void)(gem_buf_push_fn(NULL, _t3657, 2));
#line 1356 "compiler/main.gem"
    GemVal _t3658 = gem_v_ms_r;
    static GemICacheSlot _ic_551 = {0};
    GemVal _t3659[] = {gem_v_p, gem_string("int64_t "), gem_v_deadline_tmp, gem_string(" = gem_now_ms() + (int64_t)("), gem_table_get_cached(_t3658, "expr", &_ic_551), gem_string(").ival;\n")};
    GemVal _t3660[] = {gem_v_b, gem_interp(6, _t3659)};
        (void)(gem_buf_push_fn(NULL, _t3660, 2));
    }
#line 1359 "compiler/main.gem"
    GemVal _t3661[] = {gem_v_p, gem_string("while (1) {\n")};
    GemVal _t3662[] = {gem_v_b, gem_interp(2, _t3661)};
    (void)(gem_buf_push_fn(NULL, _t3662, 2));
#line 1360 "compiler/main.gem"
    GemVal _t3663[] = {gem_add(gem_v_indent, gem_int(1))};
    GemVal gem_v_p2 = gem_fn__mod_codegen_pad_for(NULL, _t3663, 1);
#line 1362 "compiler/main.gem"
    GemVal _t3664[] = {gem_v_p2, gem_string("GemProcess *"), gem_v_proc_tmp, gem_string(" = &gem_proc_table[gem_current_pid];\n")};
    GemVal _t3665[] = {gem_v_b, gem_interp(4, _t3664)};
    (void)(gem_buf_push_fn(NULL, _t3665, 2));
#line 1363 "compiler/main.gem"
    GemVal _t3666[] = {gem_v_p2, gem_string("GemMsgNode *"), gem_v_prev_tmp, gem_string(" = NULL;\n")};
    GemVal _t3667[] = {gem_v_b, gem_interp(4, _t3666)};
    (void)(gem_buf_push_fn(NULL, _t3667, 2));
#line 1364 "compiler/main.gem"
    GemVal _t3668[] = {gem_v_p2, gem_string("GemMsgNode *"), gem_v_cur_tmp, gem_string(" = "), gem_v_proc_tmp, gem_string("->mailbox.head;\n")};
    GemVal _t3669[] = {gem_v_b, gem_interp(6, _t3668)};
    (void)(gem_buf_push_fn(NULL, _t3669, 2));
#line 1365 "compiler/main.gem"
    GemVal _t3670[] = {gem_v_p2, gem_v_matched_tmp, gem_string(" = -1;\n")};
    GemVal _t3671[] = {gem_v_b, gem_interp(3, _t3670)};
    (void)(gem_buf_push_fn(NULL, _t3671, 2));
#line 1366 "compiler/main.gem"
    GemVal _t3672[] = {gem_v_p2, gem_string("while ("), gem_v_cur_tmp, gem_string(") {\n")};
    GemVal _t3673[] = {gem_v_b, gem_interp(4, _t3672)};
    (void)(gem_buf_push_fn(NULL, _t3673, 2));
#line 1367 "compiler/main.gem"
    GemVal _t3674[] = {gem_add(gem_v_indent, gem_int(2))};
    GemVal gem_v_p3 = gem_fn__mod_codegen_pad_for(NULL, _t3674, 1);
#line 1368 "compiler/main.gem"
    GemVal _t3675[] = {gem_v_p3, gem_v_recv_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string("->value;\n")};
    GemVal _t3676[] = {gem_v_b, gem_interp(5, _t3675)};
    (void)(gem_buf_push_fn(NULL, _t3676, 2));
#line 1370 "compiler/main.gem"
    GemVal gem_v__for_i_67 = gem_int(0);
#line 1370 "compiler/main.gem"
    GemVal _t3677[] = {gem_v_arms};
    GemVal gem_v__for_limit_67 = gem_len_fn(NULL, _t3677, 1);
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
    GemVal _t3678 = gem_v_arm;
    static GemICacheSlot _ic_552 = {0};
    GemVal _t3679 = gem_table_get_cached(_t3678, "pattern", &_ic_552);
    static GemICacheSlot _ic_553 = {0};
    GemVal _t3680[] = {gem_table_get_cached(_t3679, "condition", &_ic_553)};
    GemVal _t3681 = (*gem_v_compile_expr);
        GemVal gem_v_cond_r = _t3681.fn(_t3681.env, _t3680, 1);
#line 1373 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1374 "compiler/main.gem"
    GemVal _t3682 = gem_v_cond_r;
    static GemICacheSlot _ic_554 = {0};
    GemVal _t3683[] = {gem_v_b, gem_table_get_cached(_t3682, "setup", &_ic_554)};
            (void)(gem_buf_push_fn(NULL, _t3683, 2));
#line 1375 "compiler/main.gem"
    GemVal _t3684 = gem_v_cond_r;
    static GemICacheSlot _ic_555 = {0};
    GemVal _t3685[] = {gem_v_p3, gem_string("if (gem_truthy("), gem_table_get_cached(_t3684, "expr", &_ic_555), gem_string(")) {\n")};
    GemVal _t3686[] = {gem_v_b, gem_interp(4, _t3685)};
            (void)(gem_buf_push_fn(NULL, _t3686, 2));
        } else {
#line 1377 "compiler/main.gem"
    GemVal _t3687[] = {gem_v_p3, gem_string("} else {\n")};
    GemVal _t3688[] = {gem_v_b, gem_interp(2, _t3687)};
            (void)(gem_buf_push_fn(NULL, _t3688, 2));
#line 1378 "compiler/main.gem"
    GemVal _t3689 = gem_v_cond_r;
    static GemICacheSlot _ic_556 = {0};
    GemVal _t3690[] = {gem_v_b, gem_table_get_cached(_t3689, "setup", &_ic_556)};
            (void)(gem_buf_push_fn(NULL, _t3690, 2));
#line 1379 "compiler/main.gem"
    GemVal _t3691 = gem_v_cond_r;
    static GemICacheSlot _ic_557 = {0};
    GemVal _t3692[] = {gem_v_p3, gem_string("if (gem_truthy("), gem_table_get_cached(_t3691, "expr", &_ic_557), gem_string(")) {\n")};
    GemVal _t3693[] = {gem_v_b, gem_interp(4, _t3692)};
            (void)(gem_buf_push_fn(NULL, _t3693, 2));
        }
#line 1381 "compiler/main.gem"
    GemVal _t3694[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal _t3695[] = {gem_fn__mod_codegen_pad_for(NULL, _t3694, 1), gem_v_matched_tmp, gem_string(" = "), gem_v_i, gem_string(";\n")};
    GemVal _t3696[] = {gem_v_b, gem_interp(5, _t3695)};
        (void)(gem_buf_push_fn(NULL, _t3696, 2));
    }

#line 1383 "compiler/main.gem"
    GemVal gem_v__for_i_68 = gem_int(0);
#line 1383 "compiler/main.gem"
    GemVal _t3697[] = {gem_v_arms};
    GemVal gem_v__for_limit_68 = gem_len_fn(NULL, _t3697, 1);
#line 1383 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_68, gem_v__for_limit_68))) break;
#line 1383 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_68;
#line 1383 "compiler/main.gem"
        gem_v__for_i_68 = gem_add(gem_v__for_i_68, gem_int(1));
#line 1384 "compiler/main.gem"
    GemVal _t3698[] = {gem_v_p3, gem_string("}")};
    GemVal _t3699[] = {gem_v_b, gem_interp(2, _t3698)};
        (void)(gem_buf_push_fn(NULL, _t3699, 2));
#line 1385 "compiler/main.gem"
    GemVal _t3700[] = {gem_v_arms};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t3700, 1), gem_int(1))))) {
#line 1386 "compiler/main.gem"
    GemVal _t3701[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t3701, 2));
        }
    }

#line 1389 "compiler/main.gem"
    GemVal _t3702[] = {gem_v_b, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t3702, 2));
#line 1391 "compiler/main.gem"
    GemVal _t3703[] = {gem_v_p3, gem_string("if ("), gem_v_matched_tmp, gem_string(" >= 0) {\n")};
    GemVal _t3704[] = {gem_v_b, gem_interp(4, _t3703)};
    (void)(gem_buf_push_fn(NULL, _t3704, 2));
#line 1392 "compiler/main.gem"
    GemVal _t3705[] = {gem_add(gem_v_indent, gem_int(3))};
    GemVal gem_v_p4 = gem_fn__mod_codegen_pad_for(NULL, _t3705, 1);
#line 1393 "compiler/main.gem"
    GemVal _t3706[] = {gem_v_p4, gem_string("gem_mailbox_remove(&"), gem_v_proc_tmp, gem_string("->mailbox, "), gem_v_prev_tmp, gem_string(", "), gem_v_cur_tmp, gem_string(");\n")};
    GemVal _t3707[] = {gem_v_b, gem_interp(8, _t3706)};
    (void)(gem_buf_push_fn(NULL, _t3707, 2));
#line 1394 "compiler/main.gem"
    GemVal _t3708[] = {gem_v_p4, gem_string("break;\n")};
    GemVal _t3709[] = {gem_v_b, gem_interp(2, _t3708)};
    (void)(gem_buf_push_fn(NULL, _t3709, 2));
#line 1395 "compiler/main.gem"
    GemVal _t3710[] = {gem_v_p3, gem_string("}\n")};
    GemVal _t3711[] = {gem_v_b, gem_interp(2, _t3710)};
    (void)(gem_buf_push_fn(NULL, _t3711, 2));
#line 1397 "compiler/main.gem"
    GemVal _t3712[] = {gem_v_p3, gem_v_prev_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string(";\n")};
    GemVal _t3713[] = {gem_v_b, gem_interp(5, _t3712)};
    (void)(gem_buf_push_fn(NULL, _t3713, 2));
#line 1398 "compiler/main.gem"
    GemVal _t3714[] = {gem_v_p3, gem_v_cur_tmp, gem_string(" = "), gem_v_cur_tmp, gem_string("->next;\n")};
    GemVal _t3715[] = {gem_v_b, gem_interp(5, _t3714)};
    (void)(gem_buf_push_fn(NULL, _t3715, 2));
#line 1399 "compiler/main.gem"
    GemVal _t3716[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3717[] = {gem_v_b, gem_interp(2, _t3716)};
    (void)(gem_buf_push_fn(NULL, _t3717, 2));
#line 1401 "compiler/main.gem"
    GemVal _t3718[] = {gem_v_p2, gem_string("if ("), gem_v_matched_tmp, gem_string(" >= 0) break;\n")};
    GemVal _t3719[] = {gem_v_b, gem_interp(4, _t3718)};
    (void)(gem_buf_push_fn(NULL, _t3719, 2));
#line 1402 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1403 "compiler/main.gem"
    GemVal _t3720[] = {gem_v_p2, gem_string("if (gem_now_ms() >= "), gem_v_deadline_tmp, gem_string(") {\n")};
    GemVal _t3721[] = {gem_v_b, gem_interp(4, _t3720)};
        (void)(gem_buf_push_fn(NULL, _t3721, 2));
#line 1404 "compiler/main.gem"
    GemVal _t3722[] = {gem_v_p2, gem_string("    "), gem_v_matched_tmp, gem_string(" = -2;\n")};
    GemVal _t3723[] = {gem_v_b, gem_interp(4, _t3722)};
        (void)(gem_buf_push_fn(NULL, _t3723, 2));
#line 1405 "compiler/main.gem"
    GemVal _t3724[] = {gem_v_p2, gem_string("    break;\n")};
    GemVal _t3725[] = {gem_v_b, gem_interp(2, _t3724)};
        (void)(gem_buf_push_fn(NULL, _t3725, 2));
#line 1406 "compiler/main.gem"
    GemVal _t3726[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3727[] = {gem_v_b, gem_interp(2, _t3726)};
        (void)(gem_buf_push_fn(NULL, _t3727, 2));
#line 1407 "compiler/main.gem"
    GemVal _t3728[] = {gem_v_p2, gem_string("gem_selective_yield("), gem_v_deadline_tmp, gem_string(");\n")};
    GemVal _t3729[] = {gem_v_b, gem_interp(4, _t3728)};
        (void)(gem_buf_push_fn(NULL, _t3729, 2));
#line 1408 "compiler/main.gem"
    GemVal _t3730[] = {gem_v_p2, gem_string("if ("), gem_v_proc_tmp, gem_string("->timed_out) {\n")};
    GemVal _t3731[] = {gem_v_b, gem_interp(4, _t3730)};
        (void)(gem_buf_push_fn(NULL, _t3731, 2));
#line 1409 "compiler/main.gem"
    GemVal _t3732[] = {gem_v_p2, gem_string("    "), gem_v_matched_tmp, gem_string(" = -2;\n")};
    GemVal _t3733[] = {gem_v_b, gem_interp(4, _t3732)};
        (void)(gem_buf_push_fn(NULL, _t3733, 2));
#line 1410 "compiler/main.gem"
    GemVal _t3734[] = {gem_v_p2, gem_string("    break;\n")};
    GemVal _t3735[] = {gem_v_b, gem_interp(2, _t3734)};
        (void)(gem_buf_push_fn(NULL, _t3735, 2));
#line 1411 "compiler/main.gem"
    GemVal _t3736[] = {gem_v_p2, gem_string("}\n")};
    GemVal _t3737[] = {gem_v_b, gem_interp(2, _t3736)};
        (void)(gem_buf_push_fn(NULL, _t3737, 2));
    } else {
#line 1413 "compiler/main.gem"
    GemVal _t3738[] = {gem_v_p2, gem_string("gem_selective_yield(-1);\n")};
    GemVal _t3739[] = {gem_v_b, gem_interp(2, _t3738)};
        (void)(gem_buf_push_fn(NULL, _t3739, 2));
    }
#line 1416 "compiler/main.gem"
    GemVal _t3740[] = {gem_v_p, gem_string("}\n")};
    GemVal _t3741[] = {gem_v_b, gem_interp(2, _t3740)};
    (void)(gem_buf_push_fn(NULL, _t3741, 2));
#line 1419 "compiler/main.gem"
    GemVal gem_v__for_i_70 = gem_int(0);
#line 1419 "compiler/main.gem"
    GemVal _t3742[] = {gem_v_arms};
    GemVal gem_v__for_limit_70 = gem_len_fn(NULL, _t3742, 1);
#line 1419 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_70, gem_v__for_limit_70))) break;
#line 1419 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_70;
#line 1419 "compiler/main.gem"
        gem_v__for_i_70 = gem_add(gem_v__for_i_70, gem_int(1));
#line 1420 "compiler/main.gem"
        GemVal gem_v_arm = gem_table_get(gem_v_arms, gem_v_i);
#line 1421 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_i, gem_int(0)))) {
#line 1422 "compiler/main.gem"
    GemVal _t3743[] = {gem_v_p, gem_string("if ("), gem_v_matched_tmp, gem_string(" == "), gem_v_i, gem_string(") {\n")};
    GemVal _t3744[] = {gem_v_b, gem_interp(6, _t3743)};
            (void)(gem_buf_push_fn(NULL, _t3744, 2));
        } else {
#line 1424 "compiler/main.gem"
    GemVal _t3745[] = {gem_v_p, gem_string("} else if ("), gem_v_matched_tmp, gem_string(" == "), gem_v_i, gem_string(") {\n")};
    GemVal _t3746[] = {gem_v_b, gem_interp(6, _t3745)};
            (void)(gem_buf_push_fn(NULL, _t3746, 2));
        }
#line 1426 "compiler/main.gem"
    GemVal _t3747 = gem_v_arm;
    static GemICacheSlot _ic_558 = {0};
    GemVal _t3748 = gem_table_get_cached(_t3747, "pattern", &_ic_558);
    static GemICacheSlot _ic_559 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t3748, "bindings", &_ic_559), GEM_NIL))) {
#line 1427 "compiler/main.gem"
    GemVal _t3749 = gem_v_arm;
    static GemICacheSlot _ic_560 = {0};
    GemVal _t3750 = gem_table_get_cached(_t3749, "pattern", &_ic_560);
    static GemICacheSlot _ic_561 = {0};
            GemVal gem_v__for_items_69 = gem_table_get_cached(_t3750, "bindings", &_ic_561);
#line 1427 "compiler/main.gem"
            GemVal gem_v__for_i_69 = gem_int(0);
#line 1427 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t3751[] = {gem_v__for_items_69};
                if (!gem_truthy(gem_lt(gem_v__for_i_69, gem_len_fn(NULL, _t3751, 1)))) break;
#line 1427 "compiler/main.gem"
                GemVal gem_v_binding = gem_table_get(gem_v__for_items_69, gem_v__for_i_69);
#line 1427 "compiler/main.gem"
                gem_v__for_i_69 = gem_add(gem_v__for_i_69, gem_int(1));
#line 1428 "compiler/main.gem"
    GemVal _t3752[] = {gem_v_binding, gem_add(gem_v_indent, gem_int(1))};
    GemVal _t3753 = (*gem_v_compile_stmt);
    GemVal _t3754[] = {gem_v_b, _t3753.fn(_t3753.env, _t3752, 2)};
                (void)(gem_buf_push_fn(NULL, _t3754, 2));
#line 1429 "compiler/main.gem"
    GemVal _t3755[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t3755, 2));
            }

        }
#line 1432 "compiler/main.gem"
    GemVal _t3756 = gem_v_arm;
    static GemICacheSlot _ic_562 = {0};
    GemVal _t3757[] = {gem_table_get_cached(_t3756, "body", &_ic_562), gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3758 = (*gem_v_compile_stmts_tail);
    GemVal _t3759[] = {gem_v_b, _t3758.fn(_t3758.env, _t3757, 3)};
        (void)(gem_buf_push_fn(NULL, _t3759, 2));
    }

#line 1436 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_after_ms, GEM_NIL))) {
#line 1437 "compiler/main.gem"
    GemVal _t3760[] = {gem_v_p, gem_string("} else if ("), gem_v_matched_tmp, gem_string(" == -2) {\n")};
    GemVal _t3761[] = {gem_v_b, gem_interp(4, _t3760)};
        (void)(gem_buf_push_fn(NULL, _t3761, 2));
#line 1438 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_after_body, GEM_NIL))) {
#line 1439 "compiler/main.gem"
    GemVal _t3762[] = {gem_v_after_body, gem_add(gem_v_indent, gem_int(1)), gem_v_return_pos};
    GemVal _t3763 = (*gem_v_compile_stmts_tail);
    GemVal _t3764[] = {gem_v_b, _t3763.fn(_t3763.env, _t3762, 3)};
            (void)(gem_buf_push_fn(NULL, _t3764, 2));
        } else {
#line 1440 "compiler/main.gem"
            if (gem_truthy(gem_v_return_pos)) {
#line 1441 "compiler/main.gem"
    GemVal _t3765 = gem_table_new();
    GemVal _t3766[] = {_t3765, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3767 = (*gem_v_compile_stmts_tail);
    GemVal _t3768[] = {gem_v_b, _t3767.fn(_t3767.env, _t3766, 3)};
                (void)(gem_buf_push_fn(NULL, _t3768, 2));
            }
        }
    } else {
#line 1443 "compiler/main.gem"
        if (gem_truthy(gem_v_return_pos)) {
#line 1444 "compiler/main.gem"
    GemVal _t3769[] = {gem_v_p, gem_string("} else {\n")};
    GemVal _t3770[] = {gem_v_b, gem_interp(2, _t3769)};
            (void)(gem_buf_push_fn(NULL, _t3770, 2));
#line 1445 "compiler/main.gem"
    GemVal _t3771 = gem_table_new();
    GemVal _t3772[] = {_t3771, gem_add(gem_v_indent, gem_int(1)), gem_bool(1)};
    GemVal _t3773 = (*gem_v_compile_stmts_tail);
    GemVal _t3774[] = {gem_v_b, _t3773.fn(_t3773.env, _t3772, 3)};
            (void)(gem_buf_push_fn(NULL, _t3774, 2));
        }
    }
#line 1448 "compiler/main.gem"
    GemVal _t3775[] = {gem_v_p, gem_string("}")};
    GemVal _t3776[] = {gem_v_b, gem_interp(2, _t3775)};
    (void)(gem_buf_push_fn(NULL, _t3776, 2));
#line 1449 "compiler/main.gem"
    GemVal _t3777[] = {gem_v_b};
    GemVal _t3778 = gem_buf_str_fn(NULL, _t3777, 1);
    gem_pop_frame();
    return _t3778;
}

struct _closure__anon_56 {
    intptr_t _num_captures;
    GemVal *gem_v_extern_c_decls;
};
static GemVal _anon_56(void *_env, GemVal *args, int argc) {
    struct _closure__anon_56 *_cls = (struct _closure__anon_56 *)_env;
    GemVal *gem_v_extern_c_decls = _cls->gem_v_extern_c_decls;
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_ret_type = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_56", "compiler/main.gem", 0);
#line 1456 "compiler/main.gem"
    GemVal gem_v_c_ret = gem_string("void");
#line 1457 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1458 "compiler/main.gem"
        gem_v_c_ret = gem_string("int64_t");
    } else {
#line 1459 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1460 "compiler/main.gem"
            gem_v_c_ret = gem_string("double");
        } else {
#line 1461 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1462 "compiler/main.gem"
                gem_v_c_ret = gem_string("char*");
            } else {
#line 1463 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1464 "compiler/main.gem"
                    gem_v_c_ret = gem_string("int");
                } else {
#line 1465 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1466 "compiler/main.gem"
                        gem_v_c_ret = gem_string("void*");
                    } else {
#line 1467 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1468 "compiler/main.gem"
                            gem_v_c_ret = gem_string("GemVal");
                        }
                    }
                }
            }
        }
    }
#line 1471 "compiler/main.gem"
    GemVal gem_v_c_params = gem_string("");
#line 1472 "compiler/main.gem"
    GemVal _t3780[] = {gem_v_params};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t3780, 1), gem_int(0)))) {
#line 1473 "compiler/main.gem"
        gem_v_c_params = gem_string("void");
    } else {
#line 1475 "compiler/main.gem"
        GemVal gem_v__for_i_71 = gem_int(0);
#line 1475 "compiler/main.gem"
    GemVal _t3781[] = {gem_v_params};
        GemVal gem_v__for_limit_71 = gem_len_fn(NULL, _t3781, 1);
#line 1475 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_71, gem_v__for_limit_71))) break;
#line 1475 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_71;
#line 1475 "compiler/main.gem"
            gem_v__for_i_71 = gem_add(gem_v__for_i_71, gem_int(1));
#line 1476 "compiler/main.gem"
            if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1477 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string(", "));
            }
#line 1479 "compiler/main.gem"
            GemVal gem_v_pt = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1480 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Int")))) {
#line 1481 "compiler/main.gem"
                gem_v_c_params = gem_add(gem_v_c_params, gem_string("int64_t"));
            } else {
#line 1482 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Float")))) {
#line 1483 "compiler/main.gem"
                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("double"));
                } else {
#line 1484 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_pt, gem_string("String")))) {
#line 1485 "compiler/main.gem"
                        gem_v_c_params = gem_add(gem_v_c_params, gem_string("const char*"));
                    } else {
#line 1486 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_pt, gem_string("Bool")))) {
#line 1487 "compiler/main.gem"
                            gem_v_c_params = gem_add(gem_v_c_params, gem_string("int"));
                        } else {
#line 1488 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_pt, gem_string("Ptr")))) {
#line 1489 "compiler/main.gem"
                                gem_v_c_params = gem_add(gem_v_c_params, gem_string("void*"));
                            } else {
#line 1490 "compiler/main.gem"
                                if (gem_truthy(gem_eq(gem_v_pt, gem_string("Table")))) {
#line 1491 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                } else {
#line 1493 "compiler/main.gem"
                                    gem_v_c_params = gem_add(gem_v_c_params, gem_string("GemVal"));
                                }
                            }
                        }
                    }
                }
            }
        }

    }
#line 1498 "compiler/main.gem"
    GemVal _t3782[] = {gem_v_c_ret, gem_string(" "), gem_v_name, gem_string("("), gem_v_c_params, gem_string(");")};
    GemVal _t3783[] = {(*gem_v_extern_c_decls), gem_interp(6, _t3782)};
    GemVal _t3784 = gem_push_fn(NULL, _t3783, 2);
    gem_pop_frame();
    return _t3784;
}

struct _closure__anon_57 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_blocking_extern_fn;
    GemVal *gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_57(void *_env, GemVal *args, int argc) {
    struct _closure__anon_57 *_cls = (struct _closure__anon_57 *)_env;
    GemVal *gem_v_compile_blocking_extern_fn = _cls->gem_v_compile_blocking_extern_fn;
    GemVal *gem_v_emit_extern_c_decl = _cls->gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_57", "compiler/main.gem", 0);
#line 1502 "compiler/main.gem"
    GemVal gem_v__d72 = gem_v_node;
#line 1502 "compiler/main.gem"
    GemVal _t3786 = gem_v__d72;
    static GemICacheSlot _ic_563 = {0};
    GemVal gem_v_name = gem_table_get_cached(_t3786, "name", &_ic_563);
#line 1502 "compiler/main.gem"
    GemVal _t3787 = gem_v__d72;
    static GemICacheSlot _ic_564 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t3787, "params", &_ic_564);
#line 1502 "compiler/main.gem"
    GemVal _t3788 = gem_v__d72;
    static GemICacheSlot _ic_565 = {0};
    GemVal gem_v_ret_type = gem_table_get_cached(_t3788, "ret_type", &_ic_565);

#line 1504 "compiler/main.gem"
    GemVal _t3789 = gem_v_node;
    static GemICacheSlot _ic_566 = {0};
    if (gem_truthy(gem_table_get_cached(_t3789, "blocking", &_ic_566))) {
#line 1505 "compiler/main.gem"
    GemVal _t3790[] = {gem_v_node};
    GemVal _t3791 = (*gem_v_compile_blocking_extern_fn);
        GemVal _t3792 = _t3791.fn(_t3791.env, _t3790, 1);
        gem_pop_frame();
        return _t3792;
    }
#line 1508 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1509 "compiler/main.gem"
    GemVal _t3793[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3794[] = {gem_v_b, gem_interp(3, _t3793)};
    (void)(gem_buf_push_fn(NULL, _t3794, 2));
#line 1511 "compiler/main.gem"
    GemVal _t3795 = gem_table_new();
    GemVal gem_v_c_args = _t3795;
#line 1512 "compiler/main.gem"
    GemVal gem_v__for_i_73 = gem_int(0);
#line 1512 "compiler/main.gem"
    GemVal _t3796[] = {gem_v_params};
    GemVal gem_v__for_limit_73 = gem_len_fn(NULL, _t3796, 1);
#line 1512 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_73, gem_v__for_limit_73))) break;
#line 1512 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_73;
#line 1512 "compiler/main.gem"
        gem_v__for_i_73 = gem_add(gem_v__for_i_73, gem_int(1));
#line 1513 "compiler/main.gem"
        GemVal gem_v_p = gem_table_get(gem_v_params, gem_v_i);
#line 1514 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_v_p, gem_string("type"));
#line 1515 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1516 "compiler/main.gem"
    GemVal _t3797[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3798[] = {gem_v_b, gem_interp(5, _t3797)};
            (void)(gem_buf_push_fn(NULL, _t3798, 2));
#line 1517 "compiler/main.gem"
    GemVal _t3799[] = {gem_string("_p"), gem_v_i};
    GemVal _t3800[] = {gem_v_c_args, gem_interp(2, _t3799)};
            (void)(gem_push_fn(NULL, _t3800, 2));
        } else {
#line 1518 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1519 "compiler/main.gem"
    GemVal _t3801[] = {gem_string("    double _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].fval;\n")};
    GemVal _t3802[] = {gem_v_b, gem_interp(5, _t3801)};
                (void)(gem_buf_push_fn(NULL, _t3802, 2));
#line 1520 "compiler/main.gem"
    GemVal _t3803[] = {gem_string("_p"), gem_v_i};
    GemVal _t3804[] = {gem_v_c_args, gem_interp(2, _t3803)};
                (void)(gem_push_fn(NULL, _t3804, 2));
            } else {
#line 1521 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1522 "compiler/main.gem"
    GemVal _t3805[] = {gem_string("    char* _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].sval;\n")};
    GemVal _t3806[] = {gem_v_b, gem_interp(5, _t3805)};
                    (void)(gem_buf_push_fn(NULL, _t3806, 2));
#line 1523 "compiler/main.gem"
    GemVal _t3807[] = {gem_string("_p"), gem_v_i};
    GemVal _t3808[] = {gem_v_c_args, gem_interp(2, _t3807)};
                    (void)(gem_push_fn(NULL, _t3808, 2));
                } else {
#line 1524 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1525 "compiler/main.gem"
    GemVal _t3809[] = {gem_string("    int _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].bval;\n")};
    GemVal _t3810[] = {gem_v_b, gem_interp(5, _t3809)};
                        (void)(gem_buf_push_fn(NULL, _t3810, 2));
#line 1526 "compiler/main.gem"
    GemVal _t3811[] = {gem_string("_p"), gem_v_i};
    GemVal _t3812[] = {gem_v_c_args, gem_interp(2, _t3811)};
                        (void)(gem_push_fn(NULL, _t3812, 2));
                    } else {
#line 1527 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1528 "compiler/main.gem"
    GemVal _t3813[] = {gem_string("    void* _p"), gem_v_i, gem_string(" = (void*)(intptr_t)args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3814[] = {gem_v_b, gem_interp(5, _t3813)};
                            (void)(gem_buf_push_fn(NULL, _t3814, 2));
#line 1529 "compiler/main.gem"
    GemVal _t3815[] = {gem_string("_p"), gem_v_i};
    GemVal _t3816[] = {gem_v_c_args, gem_interp(2, _t3815)};
                            (void)(gem_push_fn(NULL, _t3816, 2));
                        } else {
#line 1530 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Table")))) {
#line 1531 "compiler/main.gem"
    GemVal _t3817[] = {gem_string("args["), gem_v_i, gem_string("]")};
    GemVal _t3818[] = {gem_v_c_args, gem_interp(3, _t3817)};
                                (void)(gem_push_fn(NULL, _t3818, 2));
                            } else {
#line 1533 "compiler/main.gem"
    GemVal _t3819[] = {gem_string("args["), gem_v_i, gem_string("]")};
    GemVal _t3820[] = {gem_v_c_args, gem_interp(3, _t3819)};
                                (void)(gem_push_fn(NULL, _t3820, 2));
                            }
                        }
                    }
                }
            }
        }
    }

#line 1538 "compiler/main.gem"
    GemVal gem_v_call_args = gem_string("");
#line 1539 "compiler/main.gem"
    GemVal gem_v__for_i_74 = gem_int(0);
#line 1539 "compiler/main.gem"
    GemVal _t3821[] = {gem_v_c_args};
    GemVal gem_v__for_limit_74 = gem_len_fn(NULL, _t3821, 1);
#line 1539 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_74, gem_v__for_limit_74))) break;
#line 1539 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_74;
#line 1539 "compiler/main.gem"
        gem_v__for_i_74 = gem_add(gem_v__for_i_74, gem_int(1));
#line 1540 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1541 "compiler/main.gem"
            gem_v_call_args = gem_add(gem_v_call_args, gem_string(", "));
        }
#line 1543 "compiler/main.gem"
        gem_v_call_args = gem_add(gem_v_call_args, gem_table_get(gem_v_c_args, gem_v_i));
    }

#line 1546 "compiler/main.gem"
    GemVal _t3822[] = {gem_v_name, gem_string("("), gem_v_call_args, gem_string(")")};
    GemVal gem_v_call_expr = gem_interp(4, _t3822);
#line 1548 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1549 "compiler/main.gem"
    GemVal _t3823[] = {gem_string("    "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3824[] = {gem_v_b, gem_interp(3, _t3823)};
        (void)(gem_buf_push_fn(NULL, _t3824, 2));
#line 1550 "compiler/main.gem"
    GemVal _t3825[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3825, 2));
    } else {
#line 1551 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Table")))) {
#line 1552 "compiler/main.gem"
    GemVal _t3826[] = {gem_string("    return "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3827[] = {gem_v_b, gem_interp(3, _t3826)};
            (void)(gem_buf_push_fn(NULL, _t3827, 2));
        } else {
#line 1553 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1554 "compiler/main.gem"
    GemVal _t3828[] = {gem_string("    int64_t _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3829[] = {gem_v_b, gem_interp(3, _t3828)};
                (void)(gem_buf_push_fn(NULL, _t3829, 2));
#line 1555 "compiler/main.gem"
    GemVal _t3830[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3830, 2));
            } else {
#line 1556 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1557 "compiler/main.gem"
    GemVal _t3831[] = {gem_string("    double _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3832[] = {gem_v_b, gem_interp(3, _t3831)};
                    (void)(gem_buf_push_fn(NULL, _t3832, 2));
#line 1558 "compiler/main.gem"
    GemVal _t3833[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3833, 2));
                } else {
#line 1559 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1560 "compiler/main.gem"
    GemVal _t3834[] = {gem_string("    char* _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3835[] = {gem_v_b, gem_interp(3, _t3834)};
                        (void)(gem_buf_push_fn(NULL, _t3835, 2));
#line 1561 "compiler/main.gem"
    GemVal _t3836[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3836, 2));
                    } else {
#line 1562 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1563 "compiler/main.gem"
    GemVal _t3837[] = {gem_string("    int _ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3838[] = {gem_v_b, gem_interp(3, _t3837)};
                            (void)(gem_buf_push_fn(NULL, _t3838, 2));
#line 1564 "compiler/main.gem"
    GemVal _t3839[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                            (void)(gem_buf_push_fn(NULL, _t3839, 2));
                        } else {
#line 1565 "compiler/main.gem"
                            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1566 "compiler/main.gem"
    GemVal _t3840[] = {gem_string("    void *_ret = "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3841[] = {gem_v_b, gem_interp(3, _t3840)};
                                (void)(gem_buf_push_fn(NULL, _t3841, 2));
#line 1567 "compiler/main.gem"
    GemVal _t3842[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3842, 2));
                            } else {
#line 1569 "compiler/main.gem"
    GemVal _t3843[] = {gem_string("    "), gem_v_call_expr, gem_string(";\n")};
    GemVal _t3844[] = {gem_v_b, gem_interp(3, _t3843)};
                                (void)(gem_buf_push_fn(NULL, _t3844, 2));
#line 1570 "compiler/main.gem"
    GemVal _t3845[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                                (void)(gem_buf_push_fn(NULL, _t3845, 2));
                            }
                        }
                    }
                }
            }
        }
    }
#line 1573 "compiler/main.gem"
    GemVal _t3846[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3846, 2));
#line 1575 "compiler/main.gem"
    GemVal _t3847[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc);")};
    GemVal _t3848[] = {(*gem_v_forward_decls), gem_interp(3, _t3847)};
    (void)(gem_push_fn(NULL, _t3848, 2));
#line 1576 "compiler/main.gem"
    GemVal _t3849[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3850 = (*gem_v_emit_extern_c_decl);
    (void)(_t3850.fn(_t3850.env, _t3849, 3));
#line 1578 "compiler/main.gem"
    GemVal _t3851[] = {gem_v_b};
    GemVal _t3852 = gem_buf_str_fn(NULL, _t3851, 1);
    gem_pop_frame();
    return _t3852;
}

struct _closure__anon_58 {
    intptr_t _num_captures;
    GemVal *gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls;
};
static GemVal _anon_58(void *_env, GemVal *args, int argc) {
    struct _closure__anon_58 *_cls = (struct _closure__anon_58 *)_env;
    GemVal *gem_v_emit_extern_c_decl = _cls->gem_v_emit_extern_c_decl;
    GemVal *gem_v_forward_decls = _cls->gem_v_forward_decls;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_58", "compiler/main.gem", 0);
#line 1585 "compiler/main.gem"
    GemVal gem_v__d75 = gem_v_node;
#line 1585 "compiler/main.gem"
    GemVal _t3854 = gem_v__d75;
    static GemICacheSlot _ic_567 = {0};
    GemVal gem_v_name = gem_table_get_cached(_t3854, "name", &_ic_567);
#line 1585 "compiler/main.gem"
    GemVal _t3855 = gem_v__d75;
    static GemICacheSlot _ic_568 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t3855, "params", &_ic_568);
#line 1585 "compiler/main.gem"
    GemVal _t3856 = gem_v__d75;
    static GemICacheSlot _ic_569 = {0};
    GemVal gem_v_ret_type = gem_table_get_cached(_t3856, "ret_type", &_ic_569);

#line 1586 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1589 "compiler/main.gem"
    GemVal _t3857[] = {gem_string("struct _blk_"), gem_v_name, gem_string("_args {\n")};
    GemVal _t3858[] = {gem_v_b, gem_interp(3, _t3857)};
    (void)(gem_buf_push_fn(NULL, _t3858, 2));
#line 1590 "compiler/main.gem"
    GemVal gem_v__for_i_76 = gem_int(0);
#line 1590 "compiler/main.gem"
    GemVal _t3859[] = {gem_v_params};
    GemVal gem_v__for_limit_76 = gem_len_fn(NULL, _t3859, 1);
#line 1590 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_76, gem_v__for_limit_76))) break;
#line 1590 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_76;
#line 1590 "compiler/main.gem"
        gem_v__for_i_76 = gem_add(gem_v__for_i_76, gem_int(1));
#line 1591 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1592 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1593 "compiler/main.gem"
    GemVal _t3860[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3861[] = {gem_v_b, gem_interp(3, _t3860)};
            (void)(gem_buf_push_fn(NULL, _t3861, 2));
        } else {
#line 1594 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1595 "compiler/main.gem"
    GemVal _t3862[] = {gem_string("    double _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3863[] = {gem_v_b, gem_interp(3, _t3862)};
                (void)(gem_buf_push_fn(NULL, _t3863, 2));
            } else {
#line 1596 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1597 "compiler/main.gem"
    GemVal _t3864[] = {gem_string("    char *_p"), gem_v_i, gem_string(";\n")};
    GemVal _t3865[] = {gem_v_b, gem_interp(3, _t3864)};
                    (void)(gem_buf_push_fn(NULL, _t3865, 2));
                } else {
#line 1598 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1599 "compiler/main.gem"
    GemVal _t3866[] = {gem_string("    int _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3867[] = {gem_v_b, gem_interp(3, _t3866)};
                        (void)(gem_buf_push_fn(NULL, _t3867, 2));
                    } else {
#line 1600 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1601 "compiler/main.gem"
    GemVal _t3868[] = {gem_string("    void *_p"), gem_v_i, gem_string(";\n")};
    GemVal _t3869[] = {gem_v_b, gem_interp(3, _t3868)};
                            (void)(gem_buf_push_fn(NULL, _t3869, 2));
                        }
                    }
                }
            }
        }
    }

#line 1604 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1605 "compiler/main.gem"
    GemVal _t3870[] = {gem_v_b, gem_string("    int64_t _result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3870, 2));
    } else {
#line 1606 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1607 "compiler/main.gem"
    GemVal _t3871[] = {gem_v_b, gem_string("    double _result;\n")};
            (void)(gem_buf_push_fn(NULL, _t3871, 2));
        } else {
#line 1608 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1609 "compiler/main.gem"
    GemVal _t3872[] = {gem_v_b, gem_string("    char *_result;\n")};
                (void)(gem_buf_push_fn(NULL, _t3872, 2));
            } else {
#line 1610 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1611 "compiler/main.gem"
    GemVal _t3873[] = {gem_v_b, gem_string("    int _result;\n")};
                    (void)(gem_buf_push_fn(NULL, _t3873, 2));
                } else {
#line 1612 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1613 "compiler/main.gem"
    GemVal _t3874[] = {gem_v_b, gem_string("    void *_result;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3874, 2));
                    }
                }
            }
        }
    }
#line 1615 "compiler/main.gem"
    GemVal _t3875[] = {gem_v_b, gem_string("};\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3875, 2));
#line 1618 "compiler/main.gem"
    GemVal _t3876[] = {gem_string("static void _blk_"), gem_v_name, gem_string("_worker(void *_a) {\n")};
    GemVal _t3877[] = {gem_v_b, gem_interp(3, _t3876)};
    (void)(gem_buf_push_fn(NULL, _t3877, 2));
#line 1619 "compiler/main.gem"
    GemVal _t3878[] = {gem_string("    struct _blk_"), gem_v_name, gem_string("_args *a = (struct _blk_"), gem_v_name, gem_string("_args *)_a;\n")};
    GemVal _t3879[] = {gem_v_b, gem_interp(5, _t3878)};
    (void)(gem_buf_push_fn(NULL, _t3879, 2));
#line 1621 "compiler/main.gem"
    GemVal gem_v_w_args = gem_string("");
#line 1622 "compiler/main.gem"
    GemVal gem_v__for_i_77 = gem_int(0);
#line 1622 "compiler/main.gem"
    GemVal _t3880[] = {gem_v_params};
    GemVal gem_v__for_limit_77 = gem_len_fn(NULL, _t3880, 1);
#line 1622 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_77, gem_v__for_limit_77))) break;
#line 1622 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_77;
#line 1622 "compiler/main.gem"
        gem_v__for_i_77 = gem_add(gem_v__for_i_77, gem_int(1));
#line 1623 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1624 "compiler/main.gem"
            gem_v_w_args = gem_add(gem_v_w_args, gem_string(", "));
        }
#line 1626 "compiler/main.gem"
    GemVal _t3881[] = {gem_string("a->_p"), gem_v_i};
        gem_v_w_args = gem_add(gem_v_w_args, gem_interp(2, _t3881));
    }

#line 1628 "compiler/main.gem"
    GemVal _t3882[] = {gem_v_name, gem_string("("), gem_v_w_args, gem_string(")")};
    GemVal gem_v_w_call = gem_interp(4, _t3882);
#line 1630 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1631 "compiler/main.gem"
    GemVal _t3883[] = {gem_string("    "), gem_v_w_call, gem_string(";\n")};
    GemVal _t3884[] = {gem_v_b, gem_interp(3, _t3883)};
        (void)(gem_buf_push_fn(NULL, _t3884, 2));
    } else {
#line 1633 "compiler/main.gem"
    GemVal _t3885[] = {gem_string("    a->_result = "), gem_v_w_call, gem_string(";\n")};
    GemVal _t3886[] = {gem_v_b, gem_interp(3, _t3885)};
        (void)(gem_buf_push_fn(NULL, _t3886, 2));
    }
#line 1635 "compiler/main.gem"
    GemVal _t3887[] = {gem_v_b, gem_string("}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t3887, 2));
#line 1638 "compiler/main.gem"
    GemVal _t3888[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t3889[] = {gem_v_b, gem_interp(3, _t3888)};
    (void)(gem_buf_push_fn(NULL, _t3889, 2));
#line 1641 "compiler/main.gem"
    GemVal gem_v__for_i_78 = gem_int(0);
#line 1641 "compiler/main.gem"
    GemVal _t3890[] = {gem_v_params};
    GemVal gem_v__for_limit_78 = gem_len_fn(NULL, _t3890, 1);
#line 1641 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_78, gem_v__for_limit_78))) break;
#line 1641 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_78;
#line 1641 "compiler/main.gem"
        gem_v__for_i_78 = gem_add(gem_v__for_i_78, gem_int(1));
#line 1642 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1643 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Int")))) {
#line 1644 "compiler/main.gem"
    GemVal _t3891[] = {gem_string("    int64_t _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3892[] = {gem_v_b, gem_interp(5, _t3891)};
            (void)(gem_buf_push_fn(NULL, _t3892, 2));
        } else {
#line 1645 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Float")))) {
#line 1646 "compiler/main.gem"
    GemVal _t3893[] = {gem_string("    double _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].fval;\n")};
    GemVal _t3894[] = {gem_v_b, gem_interp(5, _t3893)};
                (void)(gem_buf_push_fn(NULL, _t3894, 2));
            } else {
#line 1647 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1648 "compiler/main.gem"
    GemVal _t3895[] = {gem_string("    char* _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].sval;\n")};
    GemVal _t3896[] = {gem_v_b, gem_interp(5, _t3895)};
                    (void)(gem_buf_push_fn(NULL, _t3896, 2));
                } else {
#line 1649 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Bool")))) {
#line 1650 "compiler/main.gem"
    GemVal _t3897[] = {gem_string("    int _p"), gem_v_i, gem_string(" = args["), gem_v_i, gem_string("].bval;\n")};
    GemVal _t3898[] = {gem_v_b, gem_interp(5, _t3897)};
                        (void)(gem_buf_push_fn(NULL, _t3898, 2));
                    } else {
#line 1651 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("Ptr")))) {
#line 1652 "compiler/main.gem"
    GemVal _t3899[] = {gem_string("    void* _p"), gem_v_i, gem_string(" = (void*)(intptr_t)args["), gem_v_i, gem_string("].ival;\n")};
    GemVal _t3900[] = {gem_v_b, gem_interp(5, _t3899)};
                            (void)(gem_buf_push_fn(NULL, _t3900, 2));
                        }
                    }
                }
            }
        }
    }

#line 1657 "compiler/main.gem"
    GemVal _t3901[] = {gem_v_b, gem_string("    if (gem_current_pid >= 0) {\n")};
    (void)(gem_buf_push_fn(NULL, _t3901, 2));
#line 1658 "compiler/main.gem"
    GemVal _t3902[] = {gem_string("        struct _blk_"), gem_v_name, gem_string("_args *_ba = (struct _blk_"), gem_v_name, gem_string("_args *)malloc(sizeof(struct _blk_"), gem_v_name, gem_string("_args));\n")};
    GemVal _t3903[] = {gem_v_b, gem_interp(7, _t3902)};
    (void)(gem_buf_push_fn(NULL, _t3903, 2));
#line 1661 "compiler/main.gem"
    GemVal gem_v__for_i_79 = gem_int(0);
#line 1661 "compiler/main.gem"
    GemVal _t3904[] = {gem_v_params};
    GemVal gem_v__for_limit_79 = gem_len_fn(NULL, _t3904, 1);
#line 1661 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_79, gem_v__for_limit_79))) break;
#line 1661 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_79;
#line 1661 "compiler/main.gem"
        gem_v__for_i_79 = gem_add(gem_v__for_i_79, gem_int(1));
#line 1662 "compiler/main.gem"
        GemVal gem_v_ptype = gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type"));
#line 1663 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ptype, gem_string("String")))) {
#line 1664 "compiler/main.gem"
    GemVal _t3905[] = {gem_string("        _ba->_p"), gem_v_i, gem_string(" = strdup(_p"), gem_v_i, gem_string(");\n")};
    GemVal _t3906[] = {gem_v_b, gem_interp(5, _t3905)};
            (void)(gem_buf_push_fn(NULL, _t3906, 2));
        } else {
#line 1666 "compiler/main.gem"
    GemVal _t3907[] = {gem_string("        _ba->_p"), gem_v_i, gem_string(" = _p"), gem_v_i, gem_string(";\n")};
    GemVal _t3908[] = {gem_v_b, gem_interp(5, _t3907)};
            (void)(gem_buf_push_fn(NULL, _t3908, 2));
        }
    }

#line 1670 "compiler/main.gem"
    GemVal _t3909[] = {gem_string("        GemIORequest *_req = gem_io_submit_extern(_blk_"), gem_v_name, gem_string("_worker, _ba);\n")};
    GemVal _t3910[] = {gem_v_b, gem_interp(3, _t3909)};
    (void)(gem_buf_push_fn(NULL, _t3910, 2));
#line 1671 "compiler/main.gem"
    GemVal _t3911[] = {gem_string("        if (!_req) { free(_ba); gem_error(\""), gem_v_name, gem_string(": I/O queue full\"); }\n")};
    GemVal _t3912[] = {gem_v_b, gem_interp(3, _t3911)};
    (void)(gem_buf_push_fn(NULL, _t3912, 2));
#line 1672 "compiler/main.gem"
    GemVal _t3913[] = {gem_v_b, gem_string("        GemProcess *_proc = &gem_proc_table[gem_current_pid];\n")};
    (void)(gem_buf_push_fn(NULL, _t3913, 2));
#line 1673 "compiler/main.gem"
    GemVal _t3914[] = {gem_v_b, gem_string("        _proc->io_request = _req;\n")};
    (void)(gem_buf_push_fn(NULL, _t3914, 2));
#line 1674 "compiler/main.gem"
    GemVal _t3915[] = {gem_v_b, gem_string("        gem_io_pool_yield();\n")};
    (void)(gem_buf_push_fn(NULL, _t3915, 2));
#line 1675 "compiler/main.gem"
    GemVal _t3916[] = {gem_v_b, gem_string("        _proc->io_request = NULL;\n")};
    (void)(gem_buf_push_fn(NULL, _t3916, 2));
#line 1678 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1679 "compiler/main.gem"
    GemVal _t3917[] = {gem_v_b, gem_string("        char *_raw = _ba->_result;\n")};
        (void)(gem_buf_push_fn(NULL, _t3917, 2));
    } else {
#line 1680 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_ret_type, gem_string("Nil")))) {
#line 1681 "compiler/main.gem"
            GemVal gem_v_c_type = gem_string("int64_t");
#line 1682 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1683 "compiler/main.gem"
                gem_v_c_type = gem_string("double");
            } else {
#line 1684 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1685 "compiler/main.gem"
                    gem_v_c_type = gem_string("int");
                } else {
#line 1686 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1687 "compiler/main.gem"
                        gem_v_c_type = gem_string("void *");
                    }
                }
            }
#line 1689 "compiler/main.gem"
    GemVal _t3918[] = {gem_string("        "), gem_v_c_type, gem_string(" _ret = _ba->_result;\n")};
    GemVal _t3919[] = {gem_v_b, gem_interp(3, _t3918)};
            (void)(gem_buf_push_fn(NULL, _t3919, 2));
        }
    }
#line 1693 "compiler/main.gem"
    GemVal gem_v__for_i_80 = gem_int(0);
#line 1693 "compiler/main.gem"
    GemVal _t3920[] = {gem_v_params};
    GemVal gem_v__for_limit_80 = gem_len_fn(NULL, _t3920, 1);
#line 1693 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_80, gem_v__for_limit_80))) break;
#line 1693 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_80;
#line 1693 "compiler/main.gem"
        gem_v__for_i_80 = gem_add(gem_v__for_i_80, gem_int(1));
#line 1694 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_table_get(gem_table_get(gem_v_params, gem_v_i), gem_string("type")), gem_string("String")))) {
#line 1695 "compiler/main.gem"
    GemVal _t3921[] = {gem_string("        free(_ba->_p"), gem_v_i, gem_string(");\n")};
    GemVal _t3922[] = {gem_v_b, gem_interp(3, _t3921)};
            (void)(gem_buf_push_fn(NULL, _t3922, 2));
        }
    }

#line 1699 "compiler/main.gem"
    GemVal _t3923[] = {gem_v_b, gem_string("        free(_ba);\n")};
    (void)(gem_buf_push_fn(NULL, _t3923, 2));
#line 1700 "compiler/main.gem"
    GemVal _t3924[] = {gem_v_b, gem_string("        gem_io_free_request(_req);\n")};
    (void)(gem_buf_push_fn(NULL, _t3924, 2));
#line 1703 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1704 "compiler/main.gem"
    GemVal _t3925[] = {gem_v_b, gem_string("        return gem_int(_ret);\n")};
        (void)(gem_buf_push_fn(NULL, _t3925, 2));
    } else {
#line 1705 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1706 "compiler/main.gem"
    GemVal _t3926[] = {gem_v_b, gem_string("        return gem_float(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3926, 2));
        } else {
#line 1707 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1708 "compiler/main.gem"
    GemVal _t3927[] = {gem_v_b, gem_string("        size_t _len = _raw ? strlen(_raw) : 0;\n")};
                (void)(gem_buf_push_fn(NULL, _t3927, 2));
#line 1709 "compiler/main.gem"
    GemVal _t3928[] = {gem_v_b, gem_string("        char *_gc = (char *)GC_MALLOC_ATOMIC(_len + 1);\n")};
                (void)(gem_buf_push_fn(NULL, _t3928, 2));
#line 1710 "compiler/main.gem"
    GemVal _t3929[] = {gem_v_b, gem_string("        if (_raw) { memcpy(_gc, _raw, _len + 1); free(_raw); } else { _gc[0] = '\\0'; }\n")};
                (void)(gem_buf_push_fn(NULL, _t3929, 2));
#line 1711 "compiler/main.gem"
    GemVal _t3930[] = {gem_v_b, gem_string("        return gem_string(_gc);\n")};
                (void)(gem_buf_push_fn(NULL, _t3930, 2));
            } else {
#line 1712 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1713 "compiler/main.gem"
    GemVal _t3931[] = {gem_v_b, gem_string("        return gem_bool(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3931, 2));
                } else {
#line 1714 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1715 "compiler/main.gem"
    GemVal _t3932[] = {gem_v_b, gem_string("        GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3932, 2));
                    } else {
#line 1717 "compiler/main.gem"
    GemVal _t3933[] = {gem_v_b, gem_string("        return GEM_NIL;\n")};
                        (void)(gem_buf_push_fn(NULL, _t3933, 2));
                    }
                }
            }
        }
    }
#line 1719 "compiler/main.gem"
    GemVal _t3934[] = {gem_v_b, gem_string("    }\n")};
    (void)(gem_buf_push_fn(NULL, _t3934, 2));
#line 1722 "compiler/main.gem"
    GemVal gem_v_d_args = gem_string("");
#line 1723 "compiler/main.gem"
    GemVal gem_v__for_i_81 = gem_int(0);
#line 1723 "compiler/main.gem"
    GemVal _t3935[] = {gem_v_params};
    GemVal gem_v__for_limit_81 = gem_len_fn(NULL, _t3935, 1);
#line 1723 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_81, gem_v__for_limit_81))) break;
#line 1723 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_81;
#line 1723 "compiler/main.gem"
        gem_v__for_i_81 = gem_add(gem_v__for_i_81, gem_int(1));
#line 1724 "compiler/main.gem"
        if (gem_truthy(gem_gt(gem_v_i, gem_int(0)))) {
#line 1725 "compiler/main.gem"
            gem_v_d_args = gem_add(gem_v_d_args, gem_string(", "));
        }
#line 1727 "compiler/main.gem"
    GemVal _t3936[] = {gem_string("_p"), gem_v_i};
        gem_v_d_args = gem_add(gem_v_d_args, gem_interp(2, _t3936));
    }

#line 1729 "compiler/main.gem"
    GemVal _t3937[] = {gem_v_name, gem_string("("), gem_v_d_args, gem_string(")")};
    GemVal gem_v_d_call = gem_interp(4, _t3937);
#line 1731 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Nil")))) {
#line 1732 "compiler/main.gem"
    GemVal _t3938[] = {gem_string("    "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3939[] = {gem_v_b, gem_interp(3, _t3938)};
        (void)(gem_buf_push_fn(NULL, _t3939, 2));
#line 1733 "compiler/main.gem"
    GemVal _t3940[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t3940, 2));
    } else {
#line 1734 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Int")))) {
#line 1735 "compiler/main.gem"
    GemVal _t3941[] = {gem_string("    int64_t _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3942[] = {gem_v_b, gem_interp(3, _t3941)};
            (void)(gem_buf_push_fn(NULL, _t3942, 2));
#line 1736 "compiler/main.gem"
    GemVal _t3943[] = {gem_v_b, gem_string("    return gem_int(_ret);\n")};
            (void)(gem_buf_push_fn(NULL, _t3943, 2));
        } else {
#line 1737 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Float")))) {
#line 1738 "compiler/main.gem"
    GemVal _t3944[] = {gem_string("    double _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3945[] = {gem_v_b, gem_interp(3, _t3944)};
                (void)(gem_buf_push_fn(NULL, _t3945, 2));
#line 1739 "compiler/main.gem"
    GemVal _t3946[] = {gem_v_b, gem_string("    return gem_float(_ret);\n")};
                (void)(gem_buf_push_fn(NULL, _t3946, 2));
            } else {
#line 1740 "compiler/main.gem"
                if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("String")))) {
#line 1741 "compiler/main.gem"
    GemVal _t3947[] = {gem_string("    char* _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3948[] = {gem_v_b, gem_interp(3, _t3947)};
                    (void)(gem_buf_push_fn(NULL, _t3948, 2));
#line 1742 "compiler/main.gem"
    GemVal _t3949[] = {gem_v_b, gem_string("    return gem_string(_ret);\n")};
                    (void)(gem_buf_push_fn(NULL, _t3949, 2));
                } else {
#line 1743 "compiler/main.gem"
                    if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Bool")))) {
#line 1744 "compiler/main.gem"
    GemVal _t3950[] = {gem_string("    int _ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3951[] = {gem_v_b, gem_interp(3, _t3950)};
                        (void)(gem_buf_push_fn(NULL, _t3951, 2));
#line 1745 "compiler/main.gem"
    GemVal _t3952[] = {gem_v_b, gem_string("    return gem_bool(_ret);\n")};
                        (void)(gem_buf_push_fn(NULL, _t3952, 2));
                    } else {
#line 1746 "compiler/main.gem"
                        if (gem_truthy(gem_eq(gem_v_ret_type, gem_string("Ptr")))) {
#line 1747 "compiler/main.gem"
    GemVal _t3953[] = {gem_string("    void *_ret = "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3954[] = {gem_v_b, gem_interp(3, _t3953)};
                            (void)(gem_buf_push_fn(NULL, _t3954, 2));
#line 1748 "compiler/main.gem"
    GemVal _t3955[] = {gem_v_b, gem_string("    GemVal _r; _r.type = VAL_INT; _r.ival = (int64_t)(intptr_t)_ret; return _r;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3955, 2));
                        } else {
#line 1750 "compiler/main.gem"
    GemVal _t3956[] = {gem_string("    "), gem_v_d_call, gem_string(";\n")};
    GemVal _t3957[] = {gem_v_b, gem_interp(3, _t3956)};
                            (void)(gem_buf_push_fn(NULL, _t3957, 2));
#line 1751 "compiler/main.gem"
    GemVal _t3958[] = {gem_v_b, gem_string("    return GEM_NIL;\n")};
                            (void)(gem_buf_push_fn(NULL, _t3958, 2));
                        }
                    }
                }
            }
        }
    }
#line 1754 "compiler/main.gem"
    GemVal _t3959[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t3959, 2));
#line 1756 "compiler/main.gem"
    GemVal _t3960[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc);")};
    GemVal _t3961[] = {(*gem_v_forward_decls), gem_interp(3, _t3960)};
    (void)(gem_push_fn(NULL, _t3961, 2));
#line 1757 "compiler/main.gem"
    GemVal _t3962[] = {gem_v_name, gem_v_params, gem_v_ret_type};
    GemVal _t3963 = (*gem_v_emit_extern_c_decl);
    (void)(_t3963.fn(_t3963.env, _t3962, 3));
#line 1759 "compiler/main.gem"
    GemVal _t3964[] = {gem_v_b};
    GemVal _t3965 = gem_buf_str_fn(NULL, _t3964, 1);
    gem_pop_frame();
    return _t3965;
}

struct _closure__anon_59 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_expr;
    GemVal *gem_v_mangle;
};
static GemVal _anon_59(void *_env, GemVal *args, int argc) {
    struct _closure__anon_59 *_cls = (struct _closure__anon_59 *)_env;
    GemVal *gem_v_compile_expr = _cls->gem_v_compile_expr;
    GemVal *gem_v_mangle = _cls->gem_v_mangle;
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_params = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_cap_set = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("_anon_59", "compiler/main.gem", 0);
#line 1765 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1766 "compiler/main.gem"
    GemVal _t3967 = gem_v_node;
    static GemICacheSlot _ic_570 = {0};
    GemVal gem_v_defaults = gem_table_get_cached(_t3967, "defaults", &_ic_570);
#line 1767 "compiler/main.gem"
    if (gem_truthy(gem_eq(gem_v_defaults, GEM_NIL))) {
#line 1768 "compiler/main.gem"
    GemVal _t3968 = gem_table_new();
        gem_v_defaults = _t3968;
    }
#line 1770 "compiler/main.gem"
    GemVal gem_v__for_i_82 = gem_int(0);
#line 1770 "compiler/main.gem"
    GemVal _t3969[] = {gem_v_params};
    GemVal gem_v__for_limit_82 = gem_len_fn(NULL, _t3969, 1);
#line 1770 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_82, gem_v__for_limit_82))) break;
#line 1770 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_82;
#line 1770 "compiler/main.gem"
        gem_v__for_i_82 = gem_add(gem_v__for_i_82, gem_int(1));
#line 1771 "compiler/main.gem"
    GemVal _t3970[] = {gem_table_get(gem_v_params, gem_v_i)};
    GemVal _t3971 = (*gem_v_mangle);
        GemVal gem_v_mp = _t3971.fn(_t3971.env, _t3970, 1);
#line 1772 "compiler/main.gem"
    GemVal _t3972[] = {gem_v_defaults};
    GemVal _t3973;
    if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t3972, 1)))) {
        _t3973 = gem_lt(gem_v_i, gem_len_fn(NULL, _t3972, 1));
    } else {
        _t3973 = gem_neq(gem_table_get(gem_v_defaults, gem_v_i), GEM_NIL);
    }
        GemVal gem_v_has_def = _t3973;
#line 1773 "compiler/main.gem"
        if (gem_truthy(gem_v_has_def)) {
#line 1774 "compiler/main.gem"
    GemVal _t3974[] = {gem_table_get(gem_v_defaults, gem_v_i)};
    GemVal _t3975 = (*gem_v_compile_expr);
            GemVal gem_v_r = _t3975.fn(_t3975.env, _t3974, 1);
#line 1775 "compiler/main.gem"
    GemVal _t3976[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3976, 2))) {
#line 1776 "compiler/main.gem"
    GemVal _t3977[] = {gem_string("    GemVal *"), gem_v_mp, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3978[] = {gem_v_b, gem_interp(3, _t3977)};
                (void)(gem_buf_push_fn(NULL, _t3978, 2));
#line 1777 "compiler/main.gem"
    GemVal _t3979[] = {gem_string("    if (argc > "), gem_v_i, gem_string(") { *"), gem_v_mp, gem_string(" = args["), gem_v_i, gem_string("]; } else {\n")};
    GemVal _t3980[] = {gem_v_b, gem_interp(7, _t3979)};
                (void)(gem_buf_push_fn(NULL, _t3980, 2));
#line 1778 "compiler/main.gem"
    GemVal _t3981 = gem_v_r;
    static GemICacheSlot _ic_571 = {0};
    GemVal _t3982[] = {gem_v_b, gem_table_get_cached(_t3981, "setup", &_ic_571)};
                (void)(gem_buf_push_fn(NULL, _t3982, 2));
#line 1779 "compiler/main.gem"
    GemVal _t3983 = gem_v_r;
    static GemICacheSlot _ic_572 = {0};
    GemVal _t3984[] = {gem_string("    *"), gem_v_mp, gem_string(" = "), gem_table_get_cached(_t3983, "expr", &_ic_572), gem_string(";\n    }\n")};
    GemVal _t3985[] = {gem_v_b, gem_interp(5, _t3984)};
                (void)(gem_buf_push_fn(NULL, _t3985, 2));
            } else {
#line 1781 "compiler/main.gem"
    GemVal _t3986[] = {gem_string("    GemVal "), gem_v_mp, gem_string(";\n")};
    GemVal _t3987[] = {gem_v_b, gem_interp(3, _t3986)};
                (void)(gem_buf_push_fn(NULL, _t3987, 2));
#line 1782 "compiler/main.gem"
    GemVal _t3988[] = {gem_string("    if (argc > "), gem_v_i, gem_string(") { "), gem_v_mp, gem_string(" = args["), gem_v_i, gem_string("]; } else {\n")};
    GemVal _t3989[] = {gem_v_b, gem_interp(7, _t3988)};
                (void)(gem_buf_push_fn(NULL, _t3989, 2));
#line 1783 "compiler/main.gem"
    GemVal _t3990 = gem_v_r;
    static GemICacheSlot _ic_573 = {0};
    GemVal _t3991[] = {gem_v_b, gem_table_get_cached(_t3990, "setup", &_ic_573)};
                (void)(gem_buf_push_fn(NULL, _t3991, 2));
#line 1784 "compiler/main.gem"
    GemVal _t3992 = gem_v_r;
    static GemICacheSlot _ic_574 = {0};
    GemVal _t3993[] = {gem_string("    "), gem_v_mp, gem_string(" = "), gem_table_get_cached(_t3992, "expr", &_ic_574), gem_string(";\n    }\n")};
    GemVal _t3994[] = {gem_v_b, gem_interp(5, _t3993)};
                (void)(gem_buf_push_fn(NULL, _t3994, 2));
            }
        } else {
#line 1787 "compiler/main.gem"
    GemVal _t3995[] = {gem_v_cap_set, gem_table_get(gem_v_params, gem_v_i)};
            if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t3995, 2))) {
#line 1788 "compiler/main.gem"
    GemVal _t3996[] = {gem_string("    GemVal *"), gem_v_mp, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t3997[] = {gem_v_b, gem_interp(3, _t3996)};
                (void)(gem_buf_push_fn(NULL, _t3997, 2));
#line 1789 "compiler/main.gem"
    GemVal _t3998[] = {gem_string("    *"), gem_v_mp, gem_string(" = (argc > "), gem_v_i, gem_string(") ? args["), gem_v_i, gem_string("] : GEM_NIL;\n")};
    GemVal _t3999[] = {gem_v_b, gem_interp(7, _t3998)};
                (void)(gem_buf_push_fn(NULL, _t3999, 2));
            } else {
#line 1791 "compiler/main.gem"
    GemVal _t4000[] = {gem_string("    GemVal "), gem_v_mp, gem_string(" = (argc > "), gem_v_i, gem_string(") ? args["), gem_v_i, gem_string("] : GEM_NIL;\n")};
    GemVal _t4001[] = {gem_v_b, gem_interp(7, _t4000)};
                (void)(gem_buf_push_fn(NULL, _t4001, 2));
            }
        }
    }

#line 1797 "compiler/main.gem"
    GemVal _t4002 = gem_v_node;
    static GemICacheSlot _ic_575 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4002, "rest_param", &_ic_575), GEM_NIL))) {
#line 1798 "compiler/main.gem"
    GemVal _t4003[] = {gem_v_params};
        GemVal gem_v_N = gem_len_fn(NULL, _t4003, 1);
#line 1799 "compiler/main.gem"
    GemVal _t4004 = gem_v_node;
    static GemICacheSlot _ic_576 = {0};
    GemVal _t4005[] = {gem_table_get_cached(_t4004, "rest_param", &_ic_576)};
    GemVal _t4006 = (*gem_v_mangle);
        GemVal gem_v_mp_rest = _t4006.fn(_t4006.env, _t4005, 1);
#line 1800 "compiler/main.gem"
    GemVal _t4007 = gem_v_node;
    static GemICacheSlot _ic_577 = {0};
    GemVal _t4008[] = {gem_v_cap_set, gem_table_get_cached(_t4007, "rest_param", &_ic_577)};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4008, 2))) {
#line 1801 "compiler/main.gem"
    GemVal _t4009[] = {gem_string("    GemVal *"), gem_v_mp_rest, gem_string(" = GC_MALLOC(sizeof(GemVal));\n")};
    GemVal _t4010[] = {gem_v_b, gem_interp(3, _t4009)};
            (void)(gem_buf_push_fn(NULL, _t4010, 2));
#line 1802 "compiler/main.gem"
    GemVal _t4011[] = {gem_string("    *"), gem_v_mp_rest, gem_string(" = gem_table_new();\n")};
    GemVal _t4012[] = {gem_v_b, gem_interp(3, _t4011)};
            (void)(gem_buf_push_fn(NULL, _t4012, 2));
#line 1803 "compiler/main.gem"
    GemVal _t4013[] = {gem_string("    for (int _ri = "), gem_v_N, gem_string("; _ri < argc; _ri++) gem_table_set(*"), gem_v_mp_rest, gem_string(", gem_int(_ri - "), gem_v_N, gem_string("), args[_ri]);\n")};
    GemVal _t4014[] = {gem_v_b, gem_interp(7, _t4013)};
            (void)(gem_buf_push_fn(NULL, _t4014, 2));
        } else {
#line 1805 "compiler/main.gem"
    GemVal _t4015[] = {gem_string("    GemVal "), gem_v_mp_rest, gem_string(" = gem_table_new();\n")};
    GemVal _t4016[] = {gem_v_b, gem_interp(3, _t4015)};
            (void)(gem_buf_push_fn(NULL, _t4016, 2));
#line 1806 "compiler/main.gem"
    GemVal _t4017[] = {gem_string("    for (int _ri = "), gem_v_N, gem_string("; _ri < argc; _ri++) gem_table_set("), gem_v_mp_rest, gem_string(", gem_int(_ri - "), gem_v_N, gem_string("), args[_ri]);\n")};
    GemVal _t4018[] = {gem_v_b, gem_interp(7, _t4017)};
            (void)(gem_buf_push_fn(NULL, _t4018, 2));
        }
    }
#line 1809 "compiler/main.gem"
    GemVal _t4019[] = {gem_v_b};
    GemVal _t4020 = gem_buf_str_fn(NULL, _t4019, 1);
    gem_pop_frame();
    return _t4020;
}

struct _closure__anon_60 {
    intptr_t _num_captures;
    GemVal *gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return;
};
static GemVal _anon_60(void *_env, GemVal *args, int argc) {
    struct _closure__anon_60 *_cls = (struct _closure__anon_60 *)_env;
    GemVal *gem_v_compile_stmt = _cls->gem_v_compile_stmt;
    GemVal *gem_v_compile_stmt_return = _cls->gem_v_compile_stmt_return;
    GemVal gem_v_body = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_anon_60", "compiler/main.gem", 0);
#line 1815 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1816 "compiler/main.gem"
    GemVal _t4022[] = {gem_v_body};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4022, 1), gem_int(0)))) {
#line 1817 "compiler/main.gem"
        GemVal gem_v__for_i_83 = gem_int(0);
#line 1817 "compiler/main.gem"
    GemVal _t4023[] = {gem_v_body};
        GemVal gem_v__for_limit_83 = gem_sub(gem_len_fn(NULL, _t4023, 1), gem_int(1));
#line 1817 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_83, gem_v__for_limit_83))) break;
#line 1817 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_83;
#line 1817 "compiler/main.gem"
            gem_v__for_i_83 = gem_add(gem_v__for_i_83, gem_int(1));
#line 1818 "compiler/main.gem"
    GemVal _t4024[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(1)};
    GemVal _t4025 = (*gem_v_compile_stmt);
    GemVal _t4026[] = {gem_v_b, _t4025.fn(_t4025.env, _t4024, 2)};
            (void)(gem_buf_push_fn(NULL, _t4026, 2));
#line 1819 "compiler/main.gem"
    GemVal _t4027[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4027, 2));
        }

#line 1821 "compiler/main.gem"
    GemVal _t4028[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(1)};
    GemVal _t4029 = (*gem_v_compile_stmt_return);
    GemVal _t4030[] = {gem_v_b, _t4029.fn(_t4029.env, _t4028, 2)};
        (void)(gem_buf_push_fn(NULL, _t4030, 2));
#line 1822 "compiler/main.gem"
    GemVal _t4031[] = {gem_v_b, gem_string("\n")};
        (void)(gem_buf_push_fn(NULL, _t4031, 2));
    } else {
#line 1824 "compiler/main.gem"
    GemVal _t4032[] = {gem_v_b, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n")};
        (void)(gem_buf_push_fn(NULL, _t4032, 2));
    }
#line 1826 "compiler/main.gem"
    GemVal _t4033[] = {gem_v_b};
    GemVal _t4034 = gem_buf_str_fn(NULL, _t4033, 1);
    gem_pop_frame();
    return _t4034;
}

struct _closure__anon_61 {
    intptr_t _num_captures;
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
static GemVal _anon_61(void *_env, GemVal *args, int argc) {
    struct _closure__anon_61 *_cls = (struct _closure__anon_61 *)_env;
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
    gem_push_frame("_anon_61", "compiler/main.gem", 0);
#line 1832 "compiler/main.gem"
    GemVal gem_v__d84 = gem_v_node;
#line 1832 "compiler/main.gem"
    GemVal _t4036 = gem_v__d84;
    static GemICacheSlot _ic_578 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t4036, "params", &_ic_578);
#line 1832 "compiler/main.gem"
    GemVal _t4037 = gem_v__d84;
    static GemICacheSlot _ic_579 = {0};
    GemVal gem_v_body = gem_table_get_cached(_t4037, "body", &_ic_579);
#line 1832 "compiler/main.gem"
    GemVal _t4038 = gem_v__d84;
    static GemICacheSlot _ic_580 = {0};
    GemVal gem_v_name = gem_table_get_cached(_t4038, "name", &_ic_580);

#line 1835 "compiler/main.gem"
    GemVal _t4039[] = {gem_v_params};
    GemVal gem_v_scope_vars = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4039, 1);
#line 1836 "compiler/main.gem"
    GemVal _t4040 = gem_v_node;
    static GemICacheSlot _ic_581 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4040, "rest_param", &_ic_581), GEM_NIL))) {
#line 1837 "compiler/main.gem"
    GemVal _t4041 = gem_v_node;
    static GemICacheSlot _ic_582 = {0};
    GemVal _t4042[] = {gem_v_scope_vars, gem_table_get_cached(_t4041, "rest_param", &_ic_582)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4042, 2));
    }
#line 1839 "compiler/main.gem"
    GemVal _t4043[] = {gem_v_body, gem_v_scope_vars};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4043, 2));
#line 1841 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1842 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1843 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1844 "compiler/main.gem"
    *gem_v_local_names = gem_v_scope_vars;
#line 1846 "compiler/main.gem"
    GemVal _t4044[] = {gem_v_body, gem_v_scope_vars};
    GemVal _t4045 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_captured = _t4045.fn(_t4045.env, _t4044, 2);
#line 1847 "compiler/main.gem"
    GemVal _t4046 = gem_v_node;
    static GemICacheSlot _ic_583 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4046, "defaults", &_ic_583), GEM_NIL))) {
#line 1848 "compiler/main.gem"
    GemVal _t4047 = gem_v_node;
    static GemICacheSlot _ic_584 = {0};
        GemVal gem_v__for_items_85 = gem_table_get_cached(_t4047, "defaults", &_ic_584);
#line 1848 "compiler/main.gem"
        GemVal gem_v__for_i_85 = gem_int(0);
#line 1848 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4048[] = {gem_v__for_items_85};
            if (!gem_truthy(gem_lt(gem_v__for_i_85, gem_len_fn(NULL, _t4048, 1)))) break;
#line 1848 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_85, gem_v__for_i_85);
#line 1848 "compiler/main.gem"
            gem_v__for_i_85 = gem_add(gem_v__for_i_85, gem_int(1));
#line 1849 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 1850 "compiler/main.gem"
    GemVal _t4049[] = {gem_v_d, gem_v_scope_vars, gem_v_captured};
    GemVal _t4050 = (*gem_v_walk_captures_node);
                (void)(_t4050.fn(_t4050.env, _t4049, 3));
            }
        }

    }
#line 1855 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_captured;
#line 1856 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1858 "compiler/main.gem"
    GemVal gem_v_has_defs = gem_bool(0);
#line 1859 "compiler/main.gem"
    GemVal _t4051 = gem_v_node;
    static GemICacheSlot _ic_585 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4051, "defaults", &_ic_585), GEM_NIL))) {
#line 1860 "compiler/main.gem"
    GemVal _t4052 = gem_v_node;
    static GemICacheSlot _ic_586 = {0};
        GemVal gem_v__for_items_86 = gem_table_get_cached(_t4052, "defaults", &_ic_586);
#line 1860 "compiler/main.gem"
        GemVal gem_v__for_i_86 = gem_int(0);
#line 1860 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4053[] = {gem_v__for_items_86};
            if (!gem_truthy(gem_lt(gem_v__for_i_86, gem_len_fn(NULL, _t4053, 1)))) break;
#line 1860 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_86, gem_v__for_i_86);
#line 1860 "compiler/main.gem"
            gem_v__for_i_86 = gem_add(gem_v__for_i_86, gem_int(1));
#line 1861 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 1862 "compiler/main.gem"
                gem_v_has_defs = gem_bool(1);
                break;
            }
        }

    }
#line 1867 "compiler/main.gem"
    GemVal gem_v_use_tco = gem_bool(0);
#line 1868 "compiler/main.gem"
    GemVal _t4054 = gem_v_node;
    static GemICacheSlot _ic_587 = {0};
    GemVal _t4055;
    if (!gem_truthy(gem_eq(gem_table_get_cached(_t4054, "rest_param", &_ic_587), GEM_NIL))) {
        _t4055 = gem_eq(gem_table_get_cached(_t4054, "rest_param", &_ic_587), GEM_NIL);
    } else {
        _t4055 = gem_not(gem_v_has_defs);
    }
    GemVal _t4057;
    if (!gem_truthy(_t4055)) {
        _t4057 = _t4055;
    } else {
        GemVal _t4056[] = {gem_v_scope_vars, gem_v_name};
        _t4057 = gem_not(gem_fn__mod_codegen_set_contains(NULL, _t4056, 2));
    }
    if (gem_truthy(_t4057)) {
#line 1869 "compiler/main.gem"
    GemVal _t4058[] = {gem_v_name, gem_v_body};
    GemVal _t4059 = (*gem_v_has_self_tail_call);
        gem_v_use_tco = _t4059.fn(_t4059.env, _t4058, 2);
    }
#line 1871 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1872 "compiler/main.gem"
        *gem_v_tco_fn_name = gem_v_name;
#line 1873 "compiler/main.gem"
        *gem_v_tco_params = gem_v_params;
#line 1874 "compiler/main.gem"
        *gem_v_tco_captured = gem_v_captured;
    }
#line 1877 "compiler/main.gem"
    GemVal gem_v_b = gem_buf_new_fn(NULL, NULL, 0);
#line 1878 "compiler/main.gem"
    GemVal _t4060[] = {gem_string("static GemVal gem_fn_"), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t4061[] = {gem_v_b, gem_interp(3, _t4060)};
    (void)(gem_buf_push_fn(NULL, _t4061, 2));
#line 1880 "compiler/main.gem"
    GemVal _t4062 = gem_v_node;
    static GemICacheSlot _ic_588 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4062, "line", &_ic_588), GEM_NIL))) {
#line 1881 "compiler/main.gem"
    GemVal _t4063 = gem_v_node;
    static GemICacheSlot _ic_589 = {0};
    GemVal _t4064[] = {(*gem_v_source_name)};
    GemVal _t4065[] = {gem_string("#line "), gem_table_get_cached(_t4063, "line", &_ic_589), gem_string(" \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4064, 1), gem_string("\"\n")};
    GemVal _t4066[] = {gem_v_b, gem_interp(5, _t4065)};
        (void)(gem_buf_push_fn(NULL, _t4066, 2));
    }
#line 1884 "compiler/main.gem"
    GemVal _t4067[] = {gem_v_node, gem_v_params, gem_v_captured};
    GemVal _t4068 = (*gem_v_emit_param_bindings);
    GemVal _t4069[] = {gem_v_b, _t4068.fn(_t4068.env, _t4067, 3)};
    (void)(gem_buf_push_fn(NULL, _t4069, 2));
#line 1886 "compiler/main.gem"
    GemVal gem_v_fn_line = gem_int(0);
#line 1887 "compiler/main.gem"
    GemVal _t4070 = gem_v_node;
    static GemICacheSlot _ic_590 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4070, "line", &_ic_590), GEM_NIL))) {
#line 1888 "compiler/main.gem"
    GemVal _t4071 = gem_v_node;
    static GemICacheSlot _ic_591 = {0};
        gem_v_fn_line = gem_table_get_cached(_t4071, "line", &_ic_591);
    }
#line 1890 "compiler/main.gem"
    GemVal _t4072[] = {gem_v_name};
    GemVal _t4073[] = {(*gem_v_source_name)};
    GemVal _t4074[] = {gem_string("    gem_push_frame(\""), gem_fn__mod_codegen_escape_c_string(NULL, _t4072, 1), gem_string("\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4073, 1), gem_string("\", "), gem_v_fn_line, gem_string(");\n")};
    GemVal _t4075[] = {gem_v_b, gem_interp(7, _t4074)};
    (void)(gem_buf_push_fn(NULL, _t4075, 2));
#line 1892 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1893 "compiler/main.gem"
    GemVal _t4076[] = {gem_v_b, gem_string("    while(1) {\n")};
        (void)(gem_buf_push_fn(NULL, _t4076, 2));
#line 1894 "compiler/main.gem"
    GemVal _t4077[] = {gem_v_b, gem_string("        gem_yield_check();\n")};
        (void)(gem_buf_push_fn(NULL, _t4077, 2));
#line 1895 "compiler/main.gem"
    GemVal _t4078[] = {gem_v_body};
        if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4078, 1), gem_int(0)))) {
#line 1896 "compiler/main.gem"
            GemVal gem_v__for_i_87 = gem_int(0);
#line 1896 "compiler/main.gem"
    GemVal _t4079[] = {gem_v_body};
            GemVal gem_v__for_limit_87 = gem_sub(gem_len_fn(NULL, _t4079, 1), gem_int(1));
#line 1896 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                if (!gem_truthy(gem_lt(gem_v__for_i_87, gem_v__for_limit_87))) break;
#line 1896 "compiler/main.gem"
                GemVal gem_v_i = gem_v__for_i_87;
#line 1896 "compiler/main.gem"
                gem_v__for_i_87 = gem_add(gem_v__for_i_87, gem_int(1));
#line 1897 "compiler/main.gem"
    GemVal _t4080[] = {gem_table_get(gem_v_body, gem_v_i), gem_int(2)};
    GemVal _t4081 = (*gem_v_compile_stmt);
    GemVal _t4082[] = {gem_v_b, _t4081.fn(_t4081.env, _t4080, 2)};
                (void)(gem_buf_push_fn(NULL, _t4082, 2));
#line 1898 "compiler/main.gem"
    GemVal _t4083[] = {gem_v_b, gem_string("\n")};
                (void)(gem_buf_push_fn(NULL, _t4083, 2));
            }

#line 1900 "compiler/main.gem"
    GemVal _t4084[] = {gem_table_get(gem_v_body, gem_neg(gem_int(1))), gem_int(2)};
    GemVal _t4085 = (*gem_v_compile_stmt_return);
    GemVal _t4086[] = {gem_v_b, _t4085.fn(_t4085.env, _t4084, 2)};
            (void)(gem_buf_push_fn(NULL, _t4086, 2));
#line 1901 "compiler/main.gem"
    GemVal _t4087[] = {gem_v_b, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4087, 2));
        } else {
#line 1903 "compiler/main.gem"
    GemVal _t4088[] = {gem_v_b, gem_string("        gem_pop_frame();\n        return GEM_NIL;\n")};
            (void)(gem_buf_push_fn(NULL, _t4088, 2));
        }
#line 1905 "compiler/main.gem"
    GemVal _t4089[] = {gem_v_b, gem_string("    }\n")};
        (void)(gem_buf_push_fn(NULL, _t4089, 2));
    } else {
#line 1907 "compiler/main.gem"
    GemVal _t4090[] = {gem_v_body};
    GemVal _t4091 = (*gem_v_emit_fn_body);
    GemVal _t4092[] = {gem_v_b, _t4091.fn(_t4091.env, _t4090, 1)};
        (void)(gem_buf_push_fn(NULL, _t4092, 2));
    }
#line 1910 "compiler/main.gem"
    GemVal _t4093[] = {gem_v_b, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4093, 2));
#line 1912 "compiler/main.gem"
    if (gem_truthy(gem_v_use_tco)) {
#line 1913 "compiler/main.gem"
        *gem_v_tco_fn_name = GEM_NIL;
#line 1914 "compiler/main.gem"
        *gem_v_tco_params = GEM_NIL;
#line 1915 "compiler/main.gem"
        *gem_v_tco_captured = GEM_NIL;
    }
#line 1918 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1919 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1920 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1921 "compiler/main.gem"
    GemVal _t4094[] = {gem_v_b};
    GemVal _t4095 = gem_buf_str_fn(NULL, _t4094, 1);
    gem_pop_frame();
    return _t4095;
}

struct _closure__anon_62 {
    intptr_t _num_captures;
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
static GemVal _anon_62(void *_env, GemVal *args, int argc) {
    struct _closure__anon_62 *_cls = (struct _closure__anon_62 *)_env;
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
    gem_push_frame("_anon_62", "compiler/main.gem", 0);
#line 1927 "compiler/main.gem"
    GemVal gem_v__d88 = gem_v_node;
#line 1927 "compiler/main.gem"
    GemVal _t4097 = gem_v__d88;
    static GemICacheSlot _ic_592 = {0};
    GemVal gem_v_params = gem_table_get_cached(_t4097, "params", &_ic_592);
#line 1927 "compiler/main.gem"
    GemVal _t4098 = gem_v__d88;
    static GemICacheSlot _ic_593 = {0};
    GemVal gem_v_body = gem_table_get_cached(_t4098, "body", &_ic_593);

#line 1928 "compiler/main.gem"
    GemVal _t4099 = (*gem_v_anon_name);
    GemVal gem_v_name = _t4099.fn(_t4099.env, NULL, 0);
#line 1931 "compiler/main.gem"
    GemVal _t4100[] = {gem_v_params};
    GemVal gem_v_inner_scope = gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4100, 1);
#line 1932 "compiler/main.gem"
    GemVal _t4101 = gem_v_node;
    static GemICacheSlot _ic_594 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4101, "rest_param", &_ic_594), GEM_NIL))) {
#line 1933 "compiler/main.gem"
    GemVal _t4102 = gem_v_node;
    static GemICacheSlot _ic_595 = {0};
    GemVal _t4103[] = {gem_v_inner_scope, gem_table_get_cached(_t4102, "rest_param", &_ic_595)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4103, 2));
    }
#line 1935 "compiler/main.gem"
    GemVal _t4104[] = {gem_v_body, gem_v_inner_scope};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4104, 2));
#line 1937 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 1938 "compiler/main.gem"
    GemVal gem_v_saved_top = (*gem_v_in_top_level);
#line 1939 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 1940 "compiler/main.gem"
    GemVal _t4105[] = {gem_v_captures};
    GemVal _t4106[] = {gem_v_inner_scope, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4105, 1)};
    *gem_v_local_names = gem_fn__mod_codegen_set_union(NULL, _t4106, 2);
#line 1942 "compiler/main.gem"
    GemVal _t4107[] = {gem_v_body, gem_v_inner_scope};
    GemVal _t4108 = (*gem_v_find_captured_in_scope);
    GemVal gem_v_inner_captured = _t4108.fn(_t4108.env, _t4107, 2);
#line 1943 "compiler/main.gem"
    GemVal _t4109 = gem_v_node;
    static GemICacheSlot _ic_596 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4109, "defaults", &_ic_596), GEM_NIL))) {
#line 1944 "compiler/main.gem"
    GemVal _t4110 = gem_v_node;
    static GemICacheSlot _ic_597 = {0};
        GemVal gem_v__for_items_89 = gem_table_get_cached(_t4110, "defaults", &_ic_597);
#line 1944 "compiler/main.gem"
        GemVal gem_v__for_i_89 = gem_int(0);
#line 1944 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4111[] = {gem_v__for_items_89};
            if (!gem_truthy(gem_lt(gem_v__for_i_89, gem_len_fn(NULL, _t4111, 1)))) break;
#line 1944 "compiler/main.gem"
            GemVal gem_v_d = gem_table_get(gem_v__for_items_89, gem_v__for_i_89);
#line 1944 "compiler/main.gem"
            gem_v__for_i_89 = gem_add(gem_v__for_i_89, gem_int(1));
#line 1945 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_d, GEM_NIL))) {
#line 1946 "compiler/main.gem"
    GemVal _t4112[] = {gem_v_d, gem_v_inner_scope, gem_v_inner_captured};
    GemVal _t4113 = (*gem_v_walk_captures_node);
                (void)(_t4113.fn(_t4113.env, _t4112, 3));
            }
        }

    }
#line 1951 "compiler/main.gem"
    GemVal _t4114[] = {gem_v_captures};
    GemVal _t4115[] = {gem_v_inner_captured, gem_fn__mod_codegen_sorted_array_to_set(NULL, _t4114, 1)};
    *gem_v_boxed_vars = gem_fn__mod_codegen_set_union(NULL, _t4115, 2);
#line 1952 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 1955 "compiler/main.gem"
    GemVal _t4116[] = {gem_string("_closure_"), gem_v_name};
    GemVal gem_v_struct_name = gem_interp(2, _t4116);
#line 1956 "compiler/main.gem"
    GemVal gem_v_sb = gem_buf_new_fn(NULL, NULL, 0);
#line 1957 "compiler/main.gem"
    GemVal _t4117[] = {gem_string("struct "), gem_v_struct_name, gem_string(" {\n")};
    GemVal _t4118[] = {gem_v_sb, gem_interp(3, _t4117)};
    (void)(gem_buf_push_fn(NULL, _t4118, 2));
#line 1958 "compiler/main.gem"
    GemVal _t4119[] = {gem_v_sb, gem_string("    intptr_t _num_captures;\n")};
    (void)(gem_buf_push_fn(NULL, _t4119, 2));
#line 1959 "compiler/main.gem"
    GemVal gem_v__for_items_90 = gem_v_captures;
#line 1959 "compiler/main.gem"
    GemVal gem_v__for_i_90 = gem_int(0);
#line 1959 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4120[] = {gem_v__for_items_90};
        if (!gem_truthy(gem_lt(gem_v__for_i_90, gem_len_fn(NULL, _t4120, 1)))) break;
#line 1959 "compiler/main.gem"
        GemVal gem_v_cap = gem_table_get(gem_v__for_items_90, gem_v__for_i_90);
#line 1959 "compiler/main.gem"
        gem_v__for_i_90 = gem_add(gem_v__for_i_90, gem_int(1));
#line 1960 "compiler/main.gem"
    GemVal _t4121[] = {gem_v_cap};
    GemVal _t4122 = (*gem_v_mangle);
    GemVal _t4123[] = {gem_string("    GemVal *"), _t4122.fn(_t4122.env, _t4121, 1), gem_string(";\n")};
    GemVal _t4124[] = {gem_v_sb, gem_interp(3, _t4123)};
        (void)(gem_buf_push_fn(NULL, _t4124, 2));
    }

#line 1962 "compiler/main.gem"
    GemVal _t4125[] = {gem_v_sb, gem_string("};\n")};
    (void)(gem_buf_push_fn(NULL, _t4125, 2));
#line 1963 "compiler/main.gem"
    GemVal _t4126[] = {gem_v_sb};
    GemVal gem_v_struct_def = gem_buf_str_fn(NULL, _t4126, 1);
#line 1966 "compiler/main.gem"
    GemVal gem_v_fb = gem_buf_new_fn(NULL, NULL, 0);
#line 1967 "compiler/main.gem"
    GemVal _t4127[] = {gem_string("static GemVal "), gem_v_name, gem_string("(void *_env, GemVal *args, int argc) {\n")};
    GemVal _t4128[] = {gem_v_fb, gem_interp(3, _t4127)};
    (void)(gem_buf_push_fn(NULL, _t4128, 2));
#line 1968 "compiler/main.gem"
    GemVal _t4129[] = {gem_v_captures};
    if (gem_truthy(gem_gt(gem_len_fn(NULL, _t4129, 1), gem_int(0)))) {
#line 1969 "compiler/main.gem"
    GemVal _t4130[] = {gem_string("    struct "), gem_v_struct_name, gem_string(" *_cls = (struct "), gem_v_struct_name, gem_string(" *)_env;\n")};
    GemVal _t4131[] = {gem_v_fb, gem_interp(5, _t4130)};
        (void)(gem_buf_push_fn(NULL, _t4131, 2));
#line 1970 "compiler/main.gem"
        GemVal gem_v__for_items_91 = gem_v_captures;
#line 1970 "compiler/main.gem"
        GemVal gem_v__for_i_91 = gem_int(0);
#line 1970 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4132[] = {gem_v__for_items_91};
            if (!gem_truthy(gem_lt(gem_v__for_i_91, gem_len_fn(NULL, _t4132, 1)))) break;
#line 1970 "compiler/main.gem"
            GemVal gem_v_cap = gem_table_get(gem_v__for_items_91, gem_v__for_i_91);
#line 1970 "compiler/main.gem"
            gem_v__for_i_91 = gem_add(gem_v__for_i_91, gem_int(1));
#line 1971 "compiler/main.gem"
    GemVal _t4133[] = {gem_v_cap};
    GemVal _t4134 = (*gem_v_mangle);
            GemVal gem_v_mc = _t4134.fn(_t4134.env, _t4133, 1);
#line 1972 "compiler/main.gem"
    GemVal _t4135[] = {gem_string("    GemVal *"), gem_v_mc, gem_string(" = _cls->"), gem_v_mc, gem_string(";\n")};
    GemVal _t4136[] = {gem_v_fb, gem_interp(5, _t4135)};
            (void)(gem_buf_push_fn(NULL, _t4136, 2));
        }

    }
#line 1976 "compiler/main.gem"
    GemVal _t4137[] = {gem_v_node, gem_v_params, gem_v_inner_captured};
    GemVal _t4138 = (*gem_v_emit_param_bindings);
    GemVal _t4139[] = {gem_v_fb, _t4138.fn(_t4138.env, _t4137, 3)};
    (void)(gem_buf_push_fn(NULL, _t4139, 2));
#line 1977 "compiler/main.gem"
    GemVal _t4140[] = {gem_v_name};
    GemVal _t4141[] = {(*gem_v_source_name)};
    GemVal _t4142[] = {gem_string("    gem_push_frame(\""), gem_fn__mod_codegen_escape_c_string(NULL, _t4140, 1), gem_string("\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4141, 1), gem_string("\", 0);\n")};
    GemVal _t4143[] = {gem_v_fb, gem_interp(5, _t4142)};
    (void)(gem_buf_push_fn(NULL, _t4143, 2));
#line 1978 "compiler/main.gem"
    GemVal _t4144[] = {gem_v_body};
    GemVal _t4145 = (*gem_v_emit_fn_body);
    GemVal _t4146[] = {gem_v_fb, _t4145.fn(_t4145.env, _t4144, 1)};
    (void)(gem_buf_push_fn(NULL, _t4146, 2));
#line 1979 "compiler/main.gem"
    GemVal _t4147[] = {gem_v_fb, gem_string("}")};
    (void)(gem_buf_push_fn(NULL, _t4147, 2));
#line 1981 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 1982 "compiler/main.gem"
    *gem_v_in_top_level = gem_v_saved_top;
#line 1983 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 1986 "compiler/main.gem"
    GemVal _t4148[] = {gem_v_fb};
    GemVal _t4149[] = {(*gem_v_functions), gem_add(gem_v_struct_def, gem_buf_str_fn(NULL, _t4148, 1))};
    (void)(gem_push_fn(NULL, _t4149, 2));
    GemVal _t4150 = gem_table_new();
    gem_table_set(_t4150, gem_string("fn_name"), gem_v_name);
    gem_table_set(_t4150, gem_string("struct_name"), gem_v_struct_name);
    gem_table_set(_t4150, gem_string("captures"), gem_v_captures);
    GemVal _t4151 = _t4150;
    gem_pop_frame();
    return _t4151;
}

struct _closure__anon_63 {
    intptr_t _num_captures;
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
static GemVal _anon_63(void *_env, GemVal *args, int argc) {
    struct _closure__anon_63 *_cls = (struct _closure__anon_63 *)_env;
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
    gem_push_frame("_anon_63", "compiler/main.gem", 0);
#line 1995 "compiler/main.gem"
    GemVal _t4153 = gem_table_new();
    GemVal gem_v_fn_defs = _t4153;
#line 1996 "compiler/main.gem"
    GemVal _t4154 = gem_table_new();
    GemVal gem_v_extern_fns_list = _t4154;
#line 1997 "compiler/main.gem"
    GemVal _t4155 = gem_table_new();
    GemVal gem_v_extern_includes = _t4155;
#line 1998 "compiler/main.gem"
    GemVal _t4156 = gem_table_new();
    GemVal gem_v_top_stmts = _t4156;
#line 2000 "compiler/main.gem"
    GemVal _t4157 = gem_v_ast;
    static GemICacheSlot _ic_598 = {0};
    GemVal gem_v__for_items_92 = gem_table_get_cached(_t4157, "stmts", &_ic_598);
#line 2000 "compiler/main.gem"
    GemVal gem_v__for_i_92 = gem_int(0);
#line 2000 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4158[] = {gem_v__for_items_92};
        if (!gem_truthy(gem_lt(gem_v__for_i_92, gem_len_fn(NULL, _t4158, 1)))) break;
#line 2000 "compiler/main.gem"
        GemVal gem_v_s = gem_table_get(gem_v__for_items_92, gem_v__for_i_92);
#line 2000 "compiler/main.gem"
        gem_v__for_i_92 = gem_add(gem_v__for_i_92, gem_int(1));
#line 2001 "compiler/main.gem"
    GemVal _t4159 = gem_v_s;
    static GemICacheSlot _ic_599 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t4159, "tag", &_ic_599), gem_string("fn_def")))) {
#line 2002 "compiler/main.gem"
    GemVal _t4160[] = {gem_v_fn_defs, gem_v_s};
            (void)(gem_push_fn(NULL, _t4160, 2));
        } else {
#line 2003 "compiler/main.gem"
    GemVal _t4161 = gem_v_s;
    static GemICacheSlot _ic_600 = {0};
            if (gem_truthy(gem_eq(gem_table_get_cached(_t4161, "tag", &_ic_600), gem_string("extern_fn")))) {
#line 2004 "compiler/main.gem"
    GemVal _t4162[] = {gem_v_extern_fns_list, gem_v_s};
                (void)(gem_push_fn(NULL, _t4162, 2));
            } else {
#line 2005 "compiler/main.gem"
    GemVal _t4163 = gem_v_s;
    static GemICacheSlot _ic_601 = {0};
                if (gem_truthy(gem_eq(gem_table_get_cached(_t4163, "tag", &_ic_601), gem_string("extern_include")))) {
#line 2006 "compiler/main.gem"
    GemVal _t4164[] = {gem_v_extern_includes, gem_v_s};
                    (void)(gem_push_fn(NULL, _t4164, 2));
                } else {
#line 2007 "compiler/main.gem"
    GemVal _t4165 = gem_v_s;
    static GemICacheSlot _ic_602 = {0};
                    if (gem_truthy(gem_eq(gem_table_get_cached(_t4165, "tag", &_ic_602), gem_string("export")))) {
                    } else {
#line 2010 "compiler/main.gem"
    GemVal _t4166[] = {gem_v_top_stmts, gem_v_s};
                        (void)(gem_push_fn(NULL, _t4166, 2));
                    }
                }
            }
        }
    }

#line 2015 "compiler/main.gem"
    GemVal gem_v__for_items_93 = gem_v_extern_fns_list;
#line 2015 "compiler/main.gem"
    GemVal gem_v__for_i_93 = gem_int(0);
#line 2015 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4167[] = {gem_v__for_items_93};
        if (!gem_truthy(gem_lt(gem_v__for_i_93, gem_len_fn(NULL, _t4167, 1)))) break;
#line 2015 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_93, gem_v__for_i_93);
#line 2015 "compiler/main.gem"
        gem_v__for_i_93 = gem_add(gem_v__for_i_93, gem_int(1));
#line 2016 "compiler/main.gem"
    GemVal _t4168 = gem_v_ef;
    static GemICacheSlot _ic_603 = {0};
    GemVal _t4169[] = {(*gem_v_defined_fns), gem_table_get_cached(_t4168, "name", &_ic_603)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4169, 2));
    }

#line 2020 "compiler/main.gem"
    GemVal gem_v__for_items_94 = gem_v_fn_defs;
#line 2020 "compiler/main.gem"
    GemVal gem_v__for_i_94 = gem_int(0);
#line 2020 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4170[] = {gem_v__for_items_94};
        if (!gem_truthy(gem_lt(gem_v__for_i_94, gem_len_fn(NULL, _t4170, 1)))) break;
#line 2020 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_94, gem_v__for_i_94);
#line 2020 "compiler/main.gem"
        gem_v__for_i_94 = gem_add(gem_v__for_i_94, gem_int(1));
#line 2021 "compiler/main.gem"
    GemVal _t4171 = gem_v_fd;
    static GemICacheSlot _ic_604 = {0};
    GemVal _t4172[] = {(*gem_v_defined_fns), gem_table_get_cached(_t4171, "name", &_ic_604)};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4172, 2));
#line 2022 "compiler/main.gem"
    GemVal _t4173 = gem_v_fd;
    static GemICacheSlot _ic_605 = {0};
    GemVal _t4174[] = {gem_string("static GemVal gem_fn_"), gem_table_get_cached(_t4173, "name", &_ic_605), gem_string("(void *_env, GemVal *args, int argc);")};
        GemVal gem_v_decl = gem_interp(3, _t4174);
#line 2023 "compiler/main.gem"
    GemVal _t4175[] = {(*gem_v_forward_decls), gem_v_decl};
        (void)(gem_push_fn(NULL, _t4175, 2));
    }

#line 2027 "compiler/main.gem"
    GemVal _t4176[] = {gem_v_top_stmts, (*gem_v_top_level_vars)};
    (void)(gem_fn__mod_codegen_collect_top_let_names(NULL, _t4176, 2));
#line 2028 "compiler/main.gem"
    GemVal _t4177 = gem_table_new();
    GemVal _t4178[] = {gem_v_top_stmts, _t4177};
    GemVal _t4179 = (*gem_v_find_captured_in_scope);
    *gem_v_top_level_boxed = _t4179.fn(_t4179.env, _t4178, 2);
#line 2031 "compiler/main.gem"
    GemVal gem_v__for_items_95 = gem_v_extern_fns_list;
#line 2031 "compiler/main.gem"
    GemVal gem_v__for_i_95 = gem_int(0);
#line 2031 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4180[] = {gem_v__for_items_95};
        if (!gem_truthy(gem_lt(gem_v__for_i_95, gem_len_fn(NULL, _t4180, 1)))) break;
#line 2031 "compiler/main.gem"
        GemVal gem_v_ef = gem_table_get(gem_v__for_items_95, gem_v__for_i_95);
#line 2031 "compiler/main.gem"
        gem_v__for_i_95 = gem_add(gem_v__for_i_95, gem_int(1));
#line 2032 "compiler/main.gem"
    GemVal _t4181[] = {gem_v_ef};
    GemVal _t4182 = (*gem_v_compile_extern_fn);
    GemVal _t4183[] = {(*gem_v_functions), _t4182.fn(_t4182.env, _t4181, 1)};
        (void)(gem_push_fn(NULL, _t4183, 2));
    }

#line 2036 "compiler/main.gem"
    GemVal gem_v__for_items_96 = gem_v_fn_defs;
#line 2036 "compiler/main.gem"
    GemVal gem_v__for_i_96 = gem_int(0);
#line 2036 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4184[] = {gem_v__for_items_96};
        if (!gem_truthy(gem_lt(gem_v__for_i_96, gem_len_fn(NULL, _t4184, 1)))) break;
#line 2036 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_96, gem_v__for_i_96);
#line 2036 "compiler/main.gem"
        gem_v__for_i_96 = gem_add(gem_v__for_i_96, gem_int(1));
#line 2037 "compiler/main.gem"
    GemVal _t4185[] = {gem_v_fd};
    GemVal _t4186 = (*gem_v_compile_fn);
    GemVal _t4187[] = {(*gem_v_functions), _t4186.fn(_t4186.env, _t4185, 1)};
        (void)(gem_push_fn(NULL, _t4187, 2));
    }

#line 2041 "compiler/main.gem"
    GemVal gem_v_saved_boxed = (*gem_v_boxed_vars);
#line 2042 "compiler/main.gem"
    GemVal gem_v_saved_local = (*gem_v_local_names);
#line 2043 "compiler/main.gem"
    *gem_v_boxed_vars = (*gem_v_top_level_boxed);
#line 2044 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(1);
#line 2045 "compiler/main.gem"
    GemVal _t4188 = gem_table_new();
    *gem_v_local_names = _t4188;
#line 2046 "compiler/main.gem"
    GemVal gem_v__for_tbl_97 = (*gem_v_top_level_vars);
#line 2046 "compiler/main.gem"
    GemVal _t4189[] = {gem_v__for_tbl_97};
    GemVal gem_v__for_len_97 = gem_len_fn(NULL, _t4189, 1);
#line 2046 "compiler/main.gem"
    GemVal gem_v__for_i_97 = gem_int(0);
#line 2046 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_97, gem_v__for_len_97))) break;
#line 2046 "compiler/main.gem"
    GemVal _t4190[] = {gem_v__for_tbl_97, gem_v__for_i_97};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4190, 2);
#line 2046 "compiler/main.gem"
    GemVal _t4191[] = {gem_v__for_tbl_97, gem_v__for_i_97};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4191, 2);
#line 2046 "compiler/main.gem"
        gem_v__for_i_97 = gem_add(gem_v__for_i_97, gem_int(1));
#line 2047 "compiler/main.gem"
    GemVal _t4192[] = {(*gem_v_local_names), gem_v_k};
        (void)(gem_fn__mod_codegen_set_add(NULL, _t4192, 2));
    }

#line 2049 "compiler/main.gem"
    GemVal _t4193[] = {gem_v_top_stmts, gem_int(1)};
    GemVal _t4194 = (*gem_v_compile_stmts);
    GemVal gem_v_main_body = _t4194.fn(_t4194.env, _t4193, 2);
#line 2050 "compiler/main.gem"
    *gem_v_in_top_level = gem_bool(0);
#line 2051 "compiler/main.gem"
    *gem_v_boxed_vars = gem_v_saved_boxed;
#line 2052 "compiler/main.gem"
    *gem_v_local_names = gem_v_saved_local;
#line 2054 "compiler/main.gem"
    GemVal gem_v_has_main = gem_bool(0);
#line 2055 "compiler/main.gem"
    GemVal gem_v__for_items_98 = gem_v_fn_defs;
#line 2055 "compiler/main.gem"
    GemVal gem_v__for_i_98 = gem_int(0);
#line 2055 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4195[] = {gem_v__for_items_98};
        if (!gem_truthy(gem_lt(gem_v__for_i_98, gem_len_fn(NULL, _t4195, 1)))) break;
#line 2055 "compiler/main.gem"
        GemVal gem_v_fd = gem_table_get(gem_v__for_items_98, gem_v__for_i_98);
#line 2055 "compiler/main.gem"
        gem_v__for_i_98 = gem_add(gem_v__for_i_98, gem_int(1));
#line 2056 "compiler/main.gem"
    GemVal _t4196 = gem_v_fd;
    static GemICacheSlot _ic_606 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t4196, "name", &_ic_606), gem_string("main")))) {
#line 2057 "compiler/main.gem"
            gem_v_has_main = gem_bool(1);
        }
    }

#line 2061 "compiler/main.gem"
    GemVal gem_v_mb = gem_buf_new_fn(NULL, NULL, 0);
#line 2062 "compiler/main.gem"
    GemVal _t4197[] = {gem_v_mb, gem_string("static GemVal gem_user_main(void *_env, GemVal *_args, int _argc) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4197, 2));
#line 2063 "compiler/main.gem"
    GemVal _t4198[] = {gem_v_mb, gem_string("    (void)_env; (void)_args; (void)_argc;\n")};
    (void)(gem_buf_push_fn(NULL, _t4198, 2));
#line 2064 "compiler/main.gem"
    GemVal _t4199[] = {(*gem_v_source_name)};
    GemVal _t4200[] = {gem_string("    gem_push_frame(\"main\", \""), gem_fn__mod_codegen_escape_c_string(NULL, _t4199, 1), gem_string("\", 0);\n")};
    GemVal _t4201[] = {gem_v_mb, gem_interp(3, _t4200)};
    (void)(gem_buf_push_fn(NULL, _t4201, 2));
#line 2065 "compiler/main.gem"
    if (gem_truthy(gem_neq(gem_v_main_body, gem_string("")))) {
#line 2066 "compiler/main.gem"
    GemVal _t4202[] = {gem_v_mb, gem_v_main_body};
        (void)(gem_buf_push_fn(NULL, _t4202, 2));
    }
#line 2068 "compiler/main.gem"
    if (gem_truthy(gem_v_has_main)) {
#line 2069 "compiler/main.gem"
    GemVal _t4203[] = {gem_v_mb, gem_string("    GemVal _margs[1] = {GEM_NIL};\n")};
        (void)(gem_buf_push_fn(NULL, _t4203, 2));
#line 2070 "compiler/main.gem"
    GemVal _t4204[] = {gem_v_mb, gem_string("    gem_fn_main(NULL, _margs, 0);\n")};
        (void)(gem_buf_push_fn(NULL, _t4204, 2));
    }
#line 2072 "compiler/main.gem"
    GemVal _t4205[] = {gem_v_mb, gem_string("    gem_pop_frame();\n    return GEM_NIL;\n}\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4205, 2));
#line 2073 "compiler/main.gem"
    GemVal _t4206[] = {gem_v_mb, gem_string("int main(int argc, char **argv) {\n")};
    (void)(gem_buf_push_fn(NULL, _t4206, 2));
#line 2074 "compiler/main.gem"
    GemVal _t4207[] = {gem_v_mb, gem_string("    gem_init(argc, argv);\n")};
    (void)(gem_buf_push_fn(NULL, _t4207, 2));
#line 2075 "compiler/main.gem"
    GemVal _t4208[] = {gem_v_mb, gem_string("    gem_scheduler_init();\n")};
    (void)(gem_buf_push_fn(NULL, _t4208, 2));
#line 2076 "compiler/main.gem"
    GemVal _t4209[] = {gem_v_mb, gem_string("    gem_run_main(gem_user_main, NULL);\n")};
    (void)(gem_buf_push_fn(NULL, _t4209, 2));
#line 2077 "compiler/main.gem"
    GemVal _t4210[] = {gem_v_mb, gem_string("    return 0;\n}\n")};
    (void)(gem_buf_push_fn(NULL, _t4210, 2));
#line 2078 "compiler/main.gem"
    GemVal _t4211[] = {gem_v_mb};
    GemVal gem_v_main_c = gem_buf_str_fn(NULL, _t4211, 1);
#line 2081 "compiler/main.gem"
    GemVal gem_v_ob = gem_buf_new_fn(NULL, NULL, 0);
#line 2082 "compiler/main.gem"
    GemVal gem_v__for_items_99 = gem_v_extern_includes;
#line 2082 "compiler/main.gem"
    GemVal gem_v__for_i_99 = gem_int(0);
#line 2082 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4212[] = {gem_v__for_items_99};
        if (!gem_truthy(gem_lt(gem_v__for_i_99, gem_len_fn(NULL, _t4212, 1)))) break;
#line 2082 "compiler/main.gem"
        GemVal gem_v_ei = gem_table_get(gem_v__for_items_99, gem_v__for_i_99);
#line 2082 "compiler/main.gem"
        gem_v__for_i_99 = gem_add(gem_v__for_i_99, gem_int(1));
#line 2083 "compiler/main.gem"
    GemVal _t4213 = gem_v_ei;
    static GemICacheSlot _ic_607 = {0};
    GemVal _t4214[] = {gem_string("#include \""), gem_table_get_cached(_t4213, "path", &_ic_607), gem_string("\"\n")};
    GemVal _t4215[] = {gem_v_ob, gem_interp(3, _t4214)};
        (void)(gem_buf_push_fn(NULL, _t4215, 2));
    }

#line 2085 "compiler/main.gem"
    GemVal _t4216[] = {gem_v_ob, gem_string("#include \"gem.h\"\n")};
    (void)(gem_buf_push_fn(NULL, _t4216, 2));
#line 2089 "compiler/main.gem"
    GemVal _t4217[] = {gem_v_extern_includes};
    if (gem_truthy(gem_eq(gem_len_fn(NULL, _t4217, 1), gem_int(0)))) {
#line 2090 "compiler/main.gem"
        GemVal gem_v__for_i_100 = gem_int(0);
#line 2090 "compiler/main.gem"
    GemVal _t4218[] = {(*gem_v_extern_c_decls)};
        GemVal gem_v__for_limit_100 = gem_len_fn(NULL, _t4218, 1);
#line 2090 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            if (!gem_truthy(gem_lt(gem_v__for_i_100, gem_v__for_limit_100))) break;
#line 2090 "compiler/main.gem"
            GemVal gem_v_i = gem_v__for_i_100;
#line 2090 "compiler/main.gem"
            gem_v__for_i_100 = gem_add(gem_v__for_i_100, gem_int(1));
#line 2091 "compiler/main.gem"
    GemVal _t4219[] = {gem_v_ob, gem_table_get((*gem_v_extern_c_decls), gem_v_i)};
            (void)(gem_buf_push_fn(NULL, _t4219, 2));
#line 2092 "compiler/main.gem"
    GemVal _t4220[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4220, 2));
        }

    }
#line 2097 "compiler/main.gem"
    GemVal gem_v__for_i_101 = gem_int(0);
#line 2097 "compiler/main.gem"
    GemVal _t4221[] = {(*gem_v_forward_decls)};
    GemVal gem_v__for_limit_101 = gem_len_fn(NULL, _t4221, 1);
#line 2097 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_101, gem_v__for_limit_101))) break;
#line 2097 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_101;
#line 2097 "compiler/main.gem"
        gem_v__for_i_101 = gem_add(gem_v__for_i_101, gem_int(1));
#line 2098 "compiler/main.gem"
    GemVal _t4222[] = {gem_v_ob, gem_table_get((*gem_v_forward_decls), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4222, 2));
#line 2099 "compiler/main.gem"
    GemVal _t4223[] = {(*gem_v_forward_decls)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4223, 1), gem_int(1))))) {
#line 2100 "compiler/main.gem"
    GemVal _t4224[] = {gem_v_ob, gem_string("\n")};
            (void)(gem_buf_push_fn(NULL, _t4224, 2));
        }
    }

#line 2103 "compiler/main.gem"
    GemVal _t4225[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4225, 2));
#line 2106 "compiler/main.gem"
    GemVal _t4226[] = {(*gem_v_top_level_vars)};
    GemVal gem_v_tlv_keys = gem_keys_fn(NULL, _t4226, 1);
#line 2107 "compiler/main.gem"
    GemVal gem_v__for_i_102 = gem_int(0);
#line 2107 "compiler/main.gem"
    GemVal _t4227[] = {gem_v_tlv_keys};
    GemVal gem_v__for_limit_102 = gem_len_fn(NULL, _t4227, 1);
#line 2107 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_102, gem_v__for_limit_102))) break;
#line 2107 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_102;
#line 2107 "compiler/main.gem"
        gem_v__for_i_102 = gem_add(gem_v__for_i_102, gem_int(1));
#line 2108 "compiler/main.gem"
        GemVal gem_v_vname = gem_table_get(gem_v_tlv_keys, gem_v_i);
#line 2109 "compiler/main.gem"
    GemVal _t4228[] = {(*gem_v_top_level_boxed), gem_v_vname};
        if (gem_truthy(gem_fn__mod_codegen_set_contains(NULL, _t4228, 2))) {
#line 2110 "compiler/main.gem"
    GemVal _t4229[] = {gem_string("GemVal *gem_v_"), gem_v_vname, gem_string(";\n")};
    GemVal _t4230[] = {gem_v_ob, gem_interp(3, _t4229)};
            (void)(gem_buf_push_fn(NULL, _t4230, 2));
        } else {
#line 2112 "compiler/main.gem"
    GemVal _t4231[] = {gem_string("GemVal gem_v_"), gem_v_vname, gem_string(";\n")};
    GemVal _t4232[] = {gem_v_ob, gem_interp(3, _t4231)};
            (void)(gem_buf_push_fn(NULL, _t4232, 2));
        }
    }

#line 2115 "compiler/main.gem"
    GemVal _t4233[] = {gem_v_ob, gem_string("\n")};
    (void)(gem_buf_push_fn(NULL, _t4233, 2));
#line 2118 "compiler/main.gem"
    GemVal gem_v__for_i_103 = gem_int(0);
#line 2118 "compiler/main.gem"
    GemVal _t4234[] = {(*gem_v_functions)};
    GemVal gem_v__for_limit_103 = gem_len_fn(NULL, _t4234, 1);
#line 2118 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_103, gem_v__for_limit_103))) break;
#line 2118 "compiler/main.gem"
        GemVal gem_v_i = gem_v__for_i_103;
#line 2118 "compiler/main.gem"
        gem_v__for_i_103 = gem_add(gem_v__for_i_103, gem_int(1));
#line 2119 "compiler/main.gem"
    GemVal _t4235[] = {gem_v_ob, gem_table_get((*gem_v_functions), gem_v_i)};
        (void)(gem_buf_push_fn(NULL, _t4235, 2));
#line 2120 "compiler/main.gem"
    GemVal _t4236[] = {(*gem_v_functions)};
        if (gem_truthy(gem_lt(gem_v_i, gem_sub(gem_len_fn(NULL, _t4236, 1), gem_int(1))))) {
#line 2121 "compiler/main.gem"
    GemVal _t4237[] = {gem_v_ob, gem_string("\n\n")};
            (void)(gem_buf_push_fn(NULL, _t4237, 2));
        }
    }

#line 2124 "compiler/main.gem"
    GemVal _t4238[] = {gem_v_ob, gem_string("\n\n")};
    (void)(gem_buf_push_fn(NULL, _t4238, 2));
#line 2126 "compiler/main.gem"
    GemVal _t4239[] = {gem_v_ob, gem_v_main_c};
    (void)(gem_buf_push_fn(NULL, _t4239, 2));
#line 2127 "compiler/main.gem"
    GemVal _t4240[] = {gem_v_ob};
    GemVal _t4241 = gem_buf_str_fn(NULL, _t4240, 1);
    gem_pop_frame();
    return _t4241;
}

static GemVal gem_fn__mod_codegen_make_codegen(void *_env, GemVal *args, int argc) {
#line 125 "compiler/main.gem"
    GemVal *gem_v_source_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_source_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("_mod_codegen_make_codegen", "compiler/main.gem", 125);
#line 126 "compiler/main.gem"
    GemVal _t1863 = gem_table_new();
    GemVal *gem_v_functions = GC_MALLOC(sizeof(GemVal));
    *gem_v_functions = _t1863;
#line 127 "compiler/main.gem"
    GemVal _t1864 = gem_table_new();
    GemVal *gem_v_forward_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_forward_decls = _t1864;
#line 128 "compiler/main.gem"
    GemVal _t1865 = gem_table_new();
    GemVal *gem_v_extern_c_decls = GC_MALLOC(sizeof(GemVal));
    *gem_v_extern_c_decls = _t1865;
#line 129 "compiler/main.gem"
    GemVal *gem_v_tmp_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp_counter = gem_int(0);
#line 130 "compiler/main.gem"
    GemVal *gem_v_ic_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_ic_counter = gem_int(0);
#line 131 "compiler/main.gem"
    GemVal _t1866[] = {gem_int(123)};
    GemVal _t1867[] = {gem_int(125)};
    GemVal *gem_v_ic_init = GC_MALLOC(sizeof(GemVal));
    *gem_v_ic_init = gem_add(gem_add(gem_chr_fn(NULL, _t1866, 1), gem_string("0")), gem_chr_fn(NULL, _t1867, 1));
#line 132 "compiler/main.gem"
    GemVal *gem_v_anon_counter = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_counter = gem_int(0);
#line 133 "compiler/main.gem"
    GemVal _t1868 = gem_table_new();
    GemVal *gem_v_defined_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_defined_fns = _t1868;
#line 134 "compiler/main.gem"
    GemVal _t1869 = gem_table_new();
    gem_table_set(_t1869, gem_string("print"), gem_string("gem_print"));
    gem_table_set(_t1869, gem_string("error"), gem_string("gem_error_fn"));
    gem_table_set(_t1869, gem_string("len"), gem_string("gem_len_fn"));
    gem_table_set(_t1869, gem_string("type"), gem_string("gem_type_fn"));
    gem_table_set(_t1869, gem_string("to_string"), gem_string("gem_to_string_fn"));
    gem_table_set(_t1869, gem_string("push"), gem_string("gem_push_fn"));
    gem_table_set(_t1869, gem_string("pcall"), gem_string("gem_pcall_fn"));
    gem_table_set(_t1869, gem_string("keys"), gem_string("gem_keys_fn"));
    gem_table_set(_t1869, gem_string("str_replace"), gem_string("gem_str_replace_fn"));
    gem_table_set(_t1869, gem_string("has_key"), gem_string("gem_has_key_fn"));
    gem_table_set(_t1869, gem_string("substr"), gem_string("gem_substr_fn"));
    gem_table_set(_t1869, gem_string("chr"), gem_string("gem_chr_fn"));
    gem_table_set(_t1869, gem_string("ord"), gem_string("gem_ord_fn"));
    gem_table_set(_t1869, gem_string("to_int"), gem_string("gem_to_int_fn"));
    gem_table_set(_t1869, gem_string("to_float"), gem_string("gem_to_float_fn"));
    gem_table_set(_t1869, gem_string("buf_new"), gem_string("gem_buf_new_fn"));
    gem_table_set(_t1869, gem_string("buf_push"), gem_string("gem_buf_push_fn"));
    gem_table_set(_t1869, gem_string("buf_str"), gem_string("gem_buf_str_fn"));
    gem_table_set(_t1869, gem_string("build_string"), gem_string("gem_build_string_fn"));
    gem_table_set(_t1869, gem_string("spawn"), gem_string("gem_spawn_builtin"));
    gem_table_set(_t1869, gem_string("send"), gem_string("gem_send_builtin"));
    gem_table_set(_t1869, gem_string("receive"), gem_string("gem_receive_builtin"));
    gem_table_set(_t1869, gem_string("self"), gem_string("gem_self_builtin"));
    gem_table_set(_t1869, gem_string("monitor"), gem_string("gem_monitor_builtin"));
    gem_table_set(_t1869, gem_string("spawn_monitor"), gem_string("gem_spawn_monitor_builtin"));
    gem_table_set(_t1869, gem_string("register"), gem_string("gem_register_builtin"));
    gem_table_set(_t1869, gem_string("whereis"), gem_string("gem_whereis_builtin"));
    gem_table_set(_t1869, gem_string("time_ms"), gem_string("gem_time_ms_builtin"));
    gem_table_set(_t1869, gem_string("kill"), gem_string("gem_exit_builtin"));
    gem_table_set(_t1869, gem_string("link"), gem_string("gem_link_builtin"));
    gem_table_set(_t1869, gem_string("unlink"), gem_string("gem_unlink_builtin"));
    gem_table_set(_t1869, gem_string("spawn_link"), gem_string("gem_spawn_link_builtin"));
    gem_table_set(_t1869, gem_string("process_flag"), gem_string("gem_process_flag_builtin"));
    gem_table_set(_t1869, gem_string("make_ref"), gem_string("gem_make_ref_builtin"));
    gem_table_set(_t1869, gem_string("sleep"), gem_string("gem_sleep_builtin"));
    gem_table_set(_t1869, gem_string("send_after"), gem_string("gem_send_after_builtin"));
    gem_table_set(_t1869, gem_string("cancel_timer"), gem_string("gem_cancel_timer_builtin"));
    gem_table_set(_t1869, gem_string("processes"), gem_string("gem_processes_builtin"));
    gem_table_set(_t1869, gem_string("process_info"), gem_string("gem_process_info_builtin"));
    gem_table_set(_t1869, gem_string("read_file"), gem_string("gem_read_file_fn"));
    gem_table_set(_t1869, gem_string("write_file"), gem_string("gem_write_file_fn"));
    gem_table_set(_t1869, gem_string("delete"), gem_string("gem_delete_fn"));
    gem_table_set(_t1869, gem_string("pop"), gem_string("gem_pop_fn"));
    gem_table_set(_t1869, gem_string("values"), gem_string("gem_values_fn"));
    gem_table_set(_t1869, gem_string("eprint"), gem_string("gem_eprint_fn"));
    gem_table_set(_t1869, gem_string("exit"), gem_string("gem_exit_process_fn"));
    gem_table_set(_t1869, gem_string("argv"), gem_string("gem_argv_fn"));
    gem_table_set(_t1869, gem_string("sort"), gem_string("gem_sort_fn"));
    gem_table_set(_t1869, gem_string("floor"), gem_string("gem_floor_fn"));
    gem_table_set(_t1869, gem_string("ceil"), gem_string("gem_ceil_fn"));
    gem_table_set(_t1869, gem_string("round"), gem_string("gem_round_fn"));
    gem_table_set(_t1869, gem_string("abs"), gem_string("gem_abs_fn"));
    gem_table_set(_t1869, gem_string("pow"), gem_string("gem_pow_fn"));
    gem_table_set(_t1869, gem_string("sqrt"), gem_string("gem_sqrt_fn"));
    gem_table_set(_t1869, gem_string("random"), gem_string("gem_random_fn"));
    gem_table_set(_t1869, gem_string("append_file"), gem_string("gem_append_file_fn"));
    gem_table_set(_t1869, gem_string("getenv"), gem_string("gem_getenv_fn"));
    gem_table_set(_t1869, gem_string("input"), gem_string("gem_input_fn"));
    gem_table_set(_t1869, gem_string("insert"), gem_string("gem_insert_fn"));
    gem_table_set(_t1869, gem_string("remove_at"), gem_string("gem_remove_at_fn"));
    gem_table_set(_t1869, gem_string("band"), gem_string("gem_band_fn"));
    gem_table_set(_t1869, gem_string("bor"), gem_string("gem_bor_fn"));
    gem_table_set(_t1869, gem_string("bxor"), gem_string("gem_bxor_fn"));
    gem_table_set(_t1869, gem_string("bnot"), gem_string("gem_bnot_fn"));
    gem_table_set(_t1869, gem_string("bshl"), gem_string("gem_bshl_fn"));
    gem_table_set(_t1869, gem_string("bshr"), gem_string("gem_bshr_fn"));
    gem_table_set(_t1869, gem_string("file_exists"), gem_string("gem_file_exists_fn"));
    gem_table_set(_t1869, gem_string("dirname"), gem_string("gem_dirname_fn"));
    gem_table_set(_t1869, gem_string("path_join"), gem_string("gem_path_join_fn"));
    gem_table_set(_t1869, gem_string("normalize_path"), gem_string("gem_normalize_path_fn"));
    gem_table_set(_t1869, gem_string("remove_file"), gem_string("gem_remove_file_fn"));
    gem_table_set(_t1869, gem_string("mkdir"), gem_string("gem_mkdir_fn"));
    gem_table_set(_t1869, gem_string("list_dir"), gem_string("gem_list_dir_fn"));
    gem_table_set(_t1869, gem_string("is_dir"), gem_string("gem_is_dir_fn"));
    gem_table_set(_t1869, gem_string("exec"), gem_string("gem_exec_fn"));
    gem_table_set(_t1869, gem_string("tcp_connect"), gem_string("gem_tcp_connect_fn"));
    gem_table_set(_t1869, gem_string("tcp_listen"), gem_string("gem_tcp_listen_fn"));
    gem_table_set(_t1869, gem_string("tcp_accept"), gem_string("gem_tcp_accept_fn"));
    gem_table_set(_t1869, gem_string("tcp_read"), gem_string("gem_tcp_read_fn"));
    gem_table_set(_t1869, gem_string("tcp_write"), gem_string("gem_tcp_write_fn"));
    gem_table_set(_t1869, gem_string("tcp_close"), gem_string("gem_tcp_close_fn"));
    gem_table_set(_t1869, gem_string("epoch_ms"), gem_string("gem_epoch_ms_fn"));
    gem_table_set(_t1869, gem_string("format_time"), gem_string("gem_format_time_fn"));
    gem_table_set(_t1869, gem_string("format_time_local"), gem_string("gem_format_time_local_fn"));
    gem_table_set(_t1869, gem_string("sqlite_open"), gem_string("gem_sqlite_open_fn"));
    gem_table_set(_t1869, gem_string("sqlite_close"), gem_string("gem_sqlite_close_fn"));
    gem_table_set(_t1869, gem_string("sqlite_exec"), gem_string("gem_sqlite_exec_fn"));
    gem_table_set(_t1869, gem_string("sqlite_query"), gem_string("gem_sqlite_query_fn"));
    gem_table_set(_t1869, gem_string("sqlite_last_insert_id"), gem_string("gem_sqlite_last_insert_id_fn"));
    gem_table_set(_t1869, gem_string("sqlite_changes"), gem_string("gem_sqlite_changes_fn"));
    gem_table_set(_t1869, gem_string("__table_key_at"), gem_string("gem_table_key_at_fn"));
    gem_table_set(_t1869, gem_string("__table_val_at"), gem_string("gem_table_val_at_fn"));
    GemVal *gem_v_builtin_fns = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtin_fns = _t1869;
#line 228 "compiler/main.gem"
    GemVal _t1870 = gem_table_new();
    GemVal *gem_v_builtins = GC_MALLOC(sizeof(GemVal));
    *gem_v_builtins = _t1870;
#line 229 "compiler/main.gem"
    GemVal gem_v__for_tbl_10 = (*gem_v_builtin_fns);
#line 229 "compiler/main.gem"
    GemVal _t1871[] = {gem_v__for_tbl_10};
    GemVal gem_v__for_len_10 = gem_len_fn(NULL, _t1871, 1);
#line 229 "compiler/main.gem"
    GemVal gem_v__for_i_10 = gem_int(0);
#line 229 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_v__for_len_10))) break;
#line 229 "compiler/main.gem"
    GemVal _t1872[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t1872, 2);
#line 229 "compiler/main.gem"
    GemVal _t1873[] = {gem_v__for_tbl_10, gem_v__for_i_10};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t1873, 2);
#line 229 "compiler/main.gem"
        gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 230 "compiler/main.gem"
        gem_table_set((*gem_v_builtins), gem_v_k, gem_bool(1));
    }

#line 232 "compiler/main.gem"
    GemVal _t1874 = gem_table_new();
    GemVal *gem_v_boxed_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_boxed_vars = _t1874;
#line 233 "compiler/main.gem"
    GemVal _t1875 = gem_table_new();
    GemVal *gem_v_top_level_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_vars = _t1875;
#line 234 "compiler/main.gem"
    GemVal _t1876 = gem_table_new();
    GemVal *gem_v_top_level_boxed = GC_MALLOC(sizeof(GemVal));
    *gem_v_top_level_boxed = _t1876;
#line 235 "compiler/main.gem"
    GemVal *gem_v_in_top_level = GC_MALLOC(sizeof(GemVal));
    *gem_v_in_top_level = gem_bool(0);
#line 236 "compiler/main.gem"
    GemVal _t1877 = gem_table_new();
    GemVal *gem_v_local_names = GC_MALLOC(sizeof(GemVal));
    *gem_v_local_names = _t1877;
#line 237 "compiler/main.gem"
    GemVal *gem_v_tco_fn_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_fn_name = GEM_NIL;
#line 238 "compiler/main.gem"
    GemVal *gem_v_tco_params = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_params = GEM_NIL;
#line 239 "compiler/main.gem"
    GemVal *gem_v_tco_captured = GC_MALLOC(sizeof(GemVal));
    *gem_v_tco_captured = GEM_NIL;
#line 243 "compiler/main.gem"
    struct _closure__anon_28 *_t1880 = GC_MALLOC(sizeof(struct _closure__anon_28));
    _t1880->_num_captures = 1;
    _t1880->gem_v_tmp_counter = gem_v_tmp_counter;
    GemVal *gem_v_tmp = GC_MALLOC(sizeof(GemVal));
    *gem_v_tmp = gem_make_fn(_anon_28, _t1880);
#line 248 "compiler/main.gem"
    struct _closure__anon_29 *_t1883 = GC_MALLOC(sizeof(struct _closure__anon_29));
    _t1883->_num_captures = 1;
    _t1883->gem_v_anon_counter = gem_v_anon_counter;
    GemVal *gem_v_anon_name = GC_MALLOC(sizeof(GemVal));
    *gem_v_anon_name = gem_make_fn(_anon_29, _t1883);
#line 253 "compiler/main.gem"
    GemVal *gem_v_mangle = GC_MALLOC(sizeof(GemVal));
    *gem_v_mangle = gem_make_fn(_anon_30, NULL);
#line 259 "compiler/main.gem"
    GemVal *gem_v_collect_free_vars = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_vars = GEM_NIL;
#line 260 "compiler/main.gem"
    GemVal *gem_v_collect_free_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_collect_free_node = GEM_NIL;
#line 262 "compiler/main.gem"
    struct _closure__anon_31 *_t2200 = GC_MALLOC(sizeof(struct _closure__anon_31));
    _t2200->_num_captures = 5;
    _t2200->gem_v_builtins = gem_v_builtins;
    _t2200->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2200->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2200->gem_v_defined_fns = gem_v_defined_fns;
    _t2200->gem_v_local_names = gem_v_local_names;
    *gem_v_collect_free_node = gem_make_fn(_anon_31, _t2200);
#line 460 "compiler/main.gem"
    struct _closure__anon_32 *_t2212 = GC_MALLOC(sizeof(struct _closure__anon_32));
    _t2212->_num_captures = 1;
    _t2212->gem_v_collect_free_node = gem_v_collect_free_node;
    *gem_v_collect_free_vars = gem_make_fn(_anon_32, _t2212);
#line 475 "compiler/main.gem"
    GemVal *gem_v_walk_captures = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures = GEM_NIL;
#line 476 "compiler/main.gem"
    GemVal *gem_v_walk_captures_node = GC_MALLOC(sizeof(GemVal));
    *gem_v_walk_captures_node = GEM_NIL;
#line 478 "compiler/main.gem"
    struct _closure__anon_33 *_t2405 = GC_MALLOC(sizeof(struct _closure__anon_33));
    _t2405->_num_captures = 4;
    _t2405->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2405->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2405->gem_v_walk_captures = gem_v_walk_captures;
    _t2405->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures_node = gem_make_fn(_anon_33, _t2405);
#line 589 "compiler/main.gem"
    struct _closure__anon_34 *_t2415 = GC_MALLOC(sizeof(struct _closure__anon_34));
    _t2415->_num_captures = 1;
    _t2415->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_walk_captures = gem_make_fn(_anon_34, _t2415);
#line 600 "compiler/main.gem"
    struct _closure__anon_35 *_t2420 = GC_MALLOC(sizeof(struct _closure__anon_35));
    _t2420->_num_captures = 1;
    _t2420->gem_v_walk_captures = gem_v_walk_captures;
    GemVal *gem_v_find_captured_in_scope = GC_MALLOC(sizeof(GemVal));
    *gem_v_find_captured_in_scope = gem_make_fn(_anon_35, _t2420);
#line 608 "compiler/main.gem"
    GemVal *gem_v_compile_expr = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_expr = GEM_NIL;
#line 609 "compiler/main.gem"
    GemVal *gem_v_compile_stmt = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt = GEM_NIL;
#line 610 "compiler/main.gem"
    GemVal *gem_v_compile_stmt_return = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmt_return = GEM_NIL;
#line 611 "compiler/main.gem"
    GemVal *gem_v_compile_stmts = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts = GEM_NIL;
#line 612 "compiler/main.gem"
    GemVal *gem_v_compile_stmts_tail = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_stmts_tail = GEM_NIL;
#line 613 "compiler/main.gem"
    GemVal *gem_v_compile_if = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_if = GEM_NIL;
#line 614 "compiler/main.gem"
    GemVal *gem_v_compile_while = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_while = GEM_NIL;
#line 615 "compiler/main.gem"
    GemVal *gem_v_compile_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_match = GEM_NIL;
#line 616 "compiler/main.gem"
    GemVal *gem_v_compile_receive_match = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_receive_match = GEM_NIL;
#line 617 "compiler/main.gem"
    GemVal *gem_v_compile_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_call = GEM_NIL;
#line 618 "compiler/main.gem"
    GemVal *gem_v_compile_binop = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_binop = GEM_NIL;
#line 619 "compiler/main.gem"
    GemVal *gem_v_compile_anon_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_anon_fn = GEM_NIL;
#line 620 "compiler/main.gem"
    GemVal *gem_v_compile_closure_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_closure_fn = GEM_NIL;
#line 621 "compiler/main.gem"
    GemVal *gem_v_compile_table = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_table = GEM_NIL;
#line 622 "compiler/main.gem"
    GemVal *gem_v_compile_array = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_array = GEM_NIL;
#line 623 "compiler/main.gem"
    GemVal *gem_v_compile_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_extern_fn = GEM_NIL;
#line 624 "compiler/main.gem"
    GemVal *gem_v_compile_blocking_extern_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_blocking_extern_fn = GEM_NIL;
#line 625 "compiler/main.gem"
    GemVal *gem_v_compile_fn = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_fn = GEM_NIL;
#line 626 "compiler/main.gem"
    GemVal *gem_v_is_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_self_tail_call = GEM_NIL;
#line 627 "compiler/main.gem"
    GemVal *gem_v_has_self_tail_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_has_self_tail_call = GEM_NIL;
#line 628 "compiler/main.gem"
    GemVal *gem_v_emit_tco_continue = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_tco_continue = GEM_NIL;
#line 632 "compiler/main.gem"
    struct _closure__anon_36 *_t2505 = GC_MALLOC(sizeof(struct _closure__anon_36));
    _t2505->_num_captures = 2;
    _t2505->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t2505->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_is_self_tail_call = gem_make_fn(_anon_36, _t2505);
#line 681 "compiler/main.gem"
    struct _closure__anon_37 *_t2511 = GC_MALLOC(sizeof(struct _closure__anon_37));
    _t2511->_num_captures = 1;
    _t2511->gem_v_is_self_tail_call = gem_v_is_self_tail_call;
    *gem_v_has_self_tail_call = gem_make_fn(_anon_37, _t2511);
#line 688 "compiler/main.gem"
    struct _closure__anon_38 *_t2538 = GC_MALLOC(sizeof(struct _closure__anon_38));
    _t2538->_num_captures = 5;
    _t2538->gem_v_compile_expr = gem_v_compile_expr;
    _t2538->gem_v_mangle = gem_v_mangle;
    _t2538->gem_v_tco_captured = gem_v_tco_captured;
    _t2538->gem_v_tco_params = gem_v_tco_params;
    _t2538->gem_v_tmp = gem_v_tmp;
    *gem_v_emit_tco_continue = gem_make_fn(_anon_38, _t2538);
#line 718 "compiler/main.gem"
    struct _closure__anon_39 *_t2731 = GC_MALLOC(sizeof(struct _closure__anon_39));
    _t2731->_num_captures = 16;
    _t2731->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2731->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2731->gem_v_compile_anon_fn = gem_v_compile_anon_fn;
    _t2731->gem_v_compile_array = gem_v_compile_array;
    _t2731->gem_v_compile_binop = gem_v_compile_binop;
    _t2731->gem_v_compile_call = gem_v_compile_call;
    _t2731->gem_v_compile_expr = gem_v_compile_expr;
    _t2731->gem_v_compile_table = gem_v_compile_table;
    _t2731->gem_v_defined_fns = gem_v_defined_fns;
    _t2731->gem_v_ic_counter = gem_v_ic_counter;
    _t2731->gem_v_ic_init = gem_v_ic_init;
    _t2731->gem_v_in_top_level = gem_v_in_top_level;
    _t2731->gem_v_local_names = gem_v_local_names;
    _t2731->gem_v_mangle = gem_v_mangle;
    _t2731->gem_v_tmp = gem_v_tmp;
    _t2731->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_expr = gem_make_fn(_anon_39, _t2731);
#line 803 "compiler/main.gem"
    GemVal *gem_v_is_int_key = GC_MALLOC(sizeof(GemVal));
    *gem_v_is_int_key = gem_make_fn(_anon_40, NULL);
#line 826 "compiler/main.gem"
    struct _closure__anon_41 *_t2764 = GC_MALLOC(sizeof(struct _closure__anon_41));
    _t2764->_num_captures = 3;
    _t2764->gem_v_compile_expr = gem_v_compile_expr;
    _t2764->gem_v_is_int_key = gem_v_is_int_key;
    _t2764->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_table = gem_make_fn(_anon_41, _t2764);
#line 846 "compiler/main.gem"
    struct _closure__anon_42 *_t2780 = GC_MALLOC(sizeof(struct _closure__anon_42));
    _t2780->_num_captures = 2;
    _t2780->gem_v_compile_expr = gem_v_compile_expr;
    _t2780->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_array = gem_make_fn(_anon_42, _t2780);
#line 861 "compiler/main.gem"
    struct _closure__anon_43 *_t2827 = GC_MALLOC(sizeof(struct _closure__anon_43));
    _t2827->_num_captures = 6;
    _t2827->gem_v_boxed_vars = gem_v_boxed_vars;
    _t2827->gem_v_collect_free_node = gem_v_collect_free_node;
    _t2827->gem_v_collect_free_vars = gem_v_collect_free_vars;
    _t2827->gem_v_compile_closure_fn = gem_v_compile_closure_fn;
    _t2827->gem_v_mangle = gem_v_mangle;
    _t2827->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_anon_fn = gem_make_fn(_anon_43, _t2827);
#line 901 "compiler/main.gem"
    struct _closure__anon_44 *_t2845 = GC_MALLOC(sizeof(struct _closure__anon_44));
    _t2845->_num_captures = 2;
    _t2845->gem_v_compile_expr = gem_v_compile_expr;
    _t2845->gem_v_tmp = gem_v_tmp;
    GemVal *gem_v_compile_args = GC_MALLOC(sizeof(GemVal));
    *gem_v_compile_args = gem_make_fn(_anon_44, _t2845);
#line 922 "compiler/main.gem"
    GemVal *gem_v_emit_direct_call = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_direct_call = gem_make_fn(_anon_45, NULL);
#line 931 "compiler/main.gem"
    struct _closure__anon_46 *_t2924 = GC_MALLOC(sizeof(struct _closure__anon_46));
    _t2924->_num_captures = 8;
    _t2924->gem_v_builtin_fns = gem_v_builtin_fns;
    _t2924->gem_v_compile_args = gem_v_compile_args;
    _t2924->gem_v_compile_expr = gem_v_compile_expr;
    _t2924->gem_v_defined_fns = gem_v_defined_fns;
    _t2924->gem_v_emit_direct_call = gem_v_emit_direct_call;
    _t2924->gem_v_local_names = gem_v_local_names;
    _t2924->gem_v_source_name = gem_v_source_name;
    _t2924->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_call = gem_make_fn(_anon_46, _t2924);
#line 975 "compiler/main.gem"
    struct _closure__anon_47 *_t3078 = GC_MALLOC(sizeof(struct _closure__anon_47));
    _t3078->_num_captures = 2;
    _t3078->gem_v_compile_expr = gem_v_compile_expr;
    _t3078->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_binop = gem_make_fn(_anon_47, _t3078);
#line 1056 "compiler/main.gem"
    struct _closure__anon_48 *_t3086 = GC_MALLOC(sizeof(struct _closure__anon_48));
    _t3086->_num_captures = 1;
    _t3086->gem_v_compile_stmt = gem_v_compile_stmt;
    *gem_v_compile_stmts = gem_make_fn(_anon_48, _t3086);
#line 1065 "compiler/main.gem"
    struct _closure__anon_49 *_t3105 = GC_MALLOC(sizeof(struct _closure__anon_49));
    _t3105->_num_captures = 3;
    _t3105->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3105->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3105->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_stmts_tail = gem_make_fn(_anon_49, _t3105);
#line 1086 "compiler/main.gem"
    struct _closure__anon_50 *_t3303 = GC_MALLOC(sizeof(struct _closure__anon_50));
    _t3303->_num_captures = 13;
    _t3303->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3303->gem_v_compile_expr = gem_v_compile_expr;
    _t3303->gem_v_compile_if = gem_v_compile_if;
    _t3303->gem_v_compile_match = gem_v_compile_match;
    _t3303->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3303->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3303->gem_v_compile_while = gem_v_compile_while;
    _t3303->gem_v_in_top_level = gem_v_in_top_level;
    _t3303->gem_v_mangle = gem_v_mangle;
    _t3303->gem_v_source_name = gem_v_source_name;
    _t3303->gem_v_tmp = gem_v_tmp;
    _t3303->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t3303->gem_v_top_level_vars = gem_v_top_level_vars;
    *gem_v_compile_stmt = gem_make_fn(_anon_50, _t3303);
#line 1163 "compiler/main.gem"
    struct _closure__anon_51 *_t3503 = GC_MALLOC(sizeof(struct _closure__anon_51));
    _t3503->_num_captures = 16;
    _t3503->gem_v_boxed_vars = gem_v_boxed_vars;
    _t3503->gem_v_compile_expr = gem_v_compile_expr;
    _t3503->gem_v_compile_if = gem_v_compile_if;
    _t3503->gem_v_compile_match = gem_v_compile_match;
    _t3503->gem_v_compile_receive_match = gem_v_compile_receive_match;
    _t3503->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3503->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t3503->gem_v_compile_while = gem_v_compile_while;
    _t3503->gem_v_emit_tco_continue = gem_v_emit_tco_continue;
    _t3503->gem_v_in_top_level = gem_v_in_top_level;
    _t3503->gem_v_local_names = gem_v_local_names;
    _t3503->gem_v_mangle = gem_v_mangle;
    _t3503->gem_v_source_name = gem_v_source_name;
    _t3503->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t3503->gem_v_tmp = gem_v_tmp;
    _t3503->gem_v_top_level_boxed = gem_v_top_level_boxed;
    *gem_v_compile_stmt_return = gem_make_fn(_anon_51, _t3503);
#line 1242 "compiler/main.gem"
    struct _closure__anon_52 *_t3534 = GC_MALLOC(sizeof(struct _closure__anon_52));
    _t3534->_num_captures = 2;
    _t3534->gem_v_compile_expr = gem_v_compile_expr;
    _t3534->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    *gem_v_compile_if = gem_make_fn(_anon_52, _t3534);
#line 1262 "compiler/main.gem"
    struct _closure__anon_53 *_t3559 = GC_MALLOC(sizeof(struct _closure__anon_53));
    _t3559->_num_captures = 2;
    _t3559->gem_v_compile_expr = gem_v_compile_expr;
    _t3559->gem_v_compile_stmts = gem_v_compile_stmts;
    *gem_v_compile_while = gem_make_fn(_anon_53, _t3559);
#line 1279 "compiler/main.gem"
    struct _closure__anon_54 *_t3637 = GC_MALLOC(sizeof(struct _closure__anon_54));
    _t3637->_num_captures = 5;
    _t3637->gem_v_compile_expr = gem_v_compile_expr;
    _t3637->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3637->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3637->gem_v_mangle = gem_v_mangle;
    _t3637->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_match = gem_make_fn(_anon_54, _t3637);
#line 1335 "compiler/main.gem"
    struct _closure__anon_55 *_t3779 = GC_MALLOC(sizeof(struct _closure__anon_55));
    _t3779->_num_captures = 5;
    _t3779->gem_v_compile_expr = gem_v_compile_expr;
    _t3779->gem_v_compile_stmt = gem_v_compile_stmt;
    _t3779->gem_v_compile_stmts_tail = gem_v_compile_stmts_tail;
    _t3779->gem_v_mangle = gem_v_mangle;
    _t3779->gem_v_tmp = gem_v_tmp;
    *gem_v_compile_receive_match = gem_make_fn(_anon_55, _t3779);
#line 1455 "compiler/main.gem"
    struct _closure__anon_56 *_t3785 = GC_MALLOC(sizeof(struct _closure__anon_56));
    _t3785->_num_captures = 1;
    _t3785->gem_v_extern_c_decls = gem_v_extern_c_decls;
    GemVal *gem_v_emit_extern_c_decl = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_extern_c_decl = gem_make_fn(_anon_56, _t3785);
#line 1501 "compiler/main.gem"
    struct _closure__anon_57 *_t3853 = GC_MALLOC(sizeof(struct _closure__anon_57));
    _t3853->_num_captures = 3;
    _t3853->gem_v_compile_blocking_extern_fn = gem_v_compile_blocking_extern_fn;
    _t3853->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3853->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_extern_fn = gem_make_fn(_anon_57, _t3853);
#line 1584 "compiler/main.gem"
    struct _closure__anon_58 *_t3966 = GC_MALLOC(sizeof(struct _closure__anon_58));
    _t3966->_num_captures = 2;
    _t3966->gem_v_emit_extern_c_decl = gem_v_emit_extern_c_decl;
    _t3966->gem_v_forward_decls = gem_v_forward_decls;
    *gem_v_compile_blocking_extern_fn = gem_make_fn(_anon_58, _t3966);
#line 1764 "compiler/main.gem"
    struct _closure__anon_59 *_t4021 = GC_MALLOC(sizeof(struct _closure__anon_59));
    _t4021->_num_captures = 2;
    _t4021->gem_v_compile_expr = gem_v_compile_expr;
    _t4021->gem_v_mangle = gem_v_mangle;
    GemVal *gem_v_emit_param_bindings = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_param_bindings = gem_make_fn(_anon_59, _t4021);
#line 1814 "compiler/main.gem"
    struct _closure__anon_60 *_t4035 = GC_MALLOC(sizeof(struct _closure__anon_60));
    _t4035->_num_captures = 2;
    _t4035->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4035->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    GemVal *gem_v_emit_fn_body = GC_MALLOC(sizeof(GemVal));
    *gem_v_emit_fn_body = gem_make_fn(_anon_60, _t4035);
#line 1831 "compiler/main.gem"
    struct _closure__anon_61 *_t4096 = GC_MALLOC(sizeof(struct _closure__anon_61));
    _t4096->_num_captures = 14;
    _t4096->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4096->gem_v_compile_stmt = gem_v_compile_stmt;
    _t4096->gem_v_compile_stmt_return = gem_v_compile_stmt_return;
    _t4096->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4096->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4096->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4096->gem_v_has_self_tail_call = gem_v_has_self_tail_call;
    _t4096->gem_v_in_top_level = gem_v_in_top_level;
    _t4096->gem_v_local_names = gem_v_local_names;
    _t4096->gem_v_source_name = gem_v_source_name;
    _t4096->gem_v_tco_captured = gem_v_tco_captured;
    _t4096->gem_v_tco_fn_name = gem_v_tco_fn_name;
    _t4096->gem_v_tco_params = gem_v_tco_params;
    _t4096->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_compile_fn = gem_make_fn(_anon_61, _t4096);
#line 1926 "compiler/main.gem"
    struct _closure__anon_62 *_t4152 = GC_MALLOC(sizeof(struct _closure__anon_62));
    _t4152->_num_captures = 11;
    _t4152->gem_v_anon_name = gem_v_anon_name;
    _t4152->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4152->gem_v_emit_fn_body = gem_v_emit_fn_body;
    _t4152->gem_v_emit_param_bindings = gem_v_emit_param_bindings;
    _t4152->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4152->gem_v_functions = gem_v_functions;
    _t4152->gem_v_in_top_level = gem_v_in_top_level;
    _t4152->gem_v_local_names = gem_v_local_names;
    _t4152->gem_v_mangle = gem_v_mangle;
    _t4152->gem_v_source_name = gem_v_source_name;
    _t4152->gem_v_walk_captures_node = gem_v_walk_captures_node;
    *gem_v_compile_closure_fn = gem_make_fn(_anon_62, _t4152);
#line 1993 "compiler/main.gem"
    struct _closure__anon_63 *_t4242 = GC_MALLOC(sizeof(struct _closure__anon_63));
    _t4242->_num_captures = 14;
    _t4242->gem_v_boxed_vars = gem_v_boxed_vars;
    _t4242->gem_v_compile_extern_fn = gem_v_compile_extern_fn;
    _t4242->gem_v_compile_fn = gem_v_compile_fn;
    _t4242->gem_v_compile_stmts = gem_v_compile_stmts;
    _t4242->gem_v_defined_fns = gem_v_defined_fns;
    _t4242->gem_v_extern_c_decls = gem_v_extern_c_decls;
    _t4242->gem_v_find_captured_in_scope = gem_v_find_captured_in_scope;
    _t4242->gem_v_forward_decls = gem_v_forward_decls;
    _t4242->gem_v_functions = gem_v_functions;
    _t4242->gem_v_in_top_level = gem_v_in_top_level;
    _t4242->gem_v_local_names = gem_v_local_names;
    _t4242->gem_v_source_name = gem_v_source_name;
    _t4242->gem_v_top_level_boxed = gem_v_top_level_boxed;
    _t4242->gem_v_top_level_vars = gem_v_top_level_vars;
    GemVal gem_v_compile = gem_make_fn(_anon_63, _t4242);
    GemVal _t4243 = gem_table_new();
    gem_table_set(_t4243, gem_string("compile"), gem_v_compile);
    GemVal _t4244 = _t4243;
    gem_pop_frame();
    return _t4244;
}

static GemVal gem_fn_is_node(void *_env, GemVal *args, int argc) {
#line 20 "compiler/main.gem"
    GemVal gem_v_node = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_tag = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("is_node", "compiler/main.gem", 20);
    GemVal _t4245[] = {gem_v_node};
    GemVal _t4247;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4245, 1), gem_string("table")))) {
        _t4247 = gem_eq(gem_type_fn(NULL, _t4245, 1), gem_string("table"));
    } else {
        GemVal _t4246 = gem_v_node;
        static GemICacheSlot _ic_608 = {0};
        _t4247 = gem_eq(gem_table_get_cached(_t4246, "tag", &_ic_608), gem_v_tag);
    }
    GemVal _t4248 = _t4247;
    gem_pop_frame();
    return _t4248;
}

static GemVal gem_fn_make_program(void *_env, GemVal *args, int argc) {
#line 24 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_program", "compiler/main.gem", 24);
    GemVal _t4249 = gem_table_new();
    gem_table_set(_t4249, gem_string("tag"), gem_string("program"));
    gem_table_set(_t4249, gem_string("stmts"), gem_v_stmts);
    GemVal _t4250 = _t4249;
    gem_pop_frame();
    return _t4250;
}

static GemVal gem_fn_make_let(void *_env, GemVal *args, int argc) {
#line 28 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_line = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("make_let", "compiler/main.gem", 28);
    GemVal _t4251 = gem_table_new();
    gem_table_set(_t4251, gem_string("tag"), gem_string("let"));
    gem_table_set(_t4251, gem_string("name"), gem_v_name);
    gem_table_set(_t4251, gem_string("value"), gem_v_value);
    gem_table_set(_t4251, gem_string("line"), gem_v_line);
    GemVal _t4252 = _t4251;
    gem_pop_frame();
    return _t4252;
}

static GemVal gem_fn_make_var(void *_env, GemVal *args, int argc) {
#line 32 "compiler/main.gem"
    GemVal gem_v_name = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_var", "compiler/main.gem", 32);
    GemVal _t4253 = gem_table_new();
    gem_table_set(_t4253, gem_string("tag"), gem_string("var"));
    gem_table_set(_t4253, gem_string("name"), gem_v_name);
    GemVal _t4254 = _t4253;
    gem_pop_frame();
    return _t4254;
}

static GemVal gem_fn_make_table(void *_env, GemVal *args, int argc) {
#line 36 "compiler/main.gem"
    GemVal gem_v_entries = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("make_table", "compiler/main.gem", 36);
    GemVal _t4255 = gem_table_new();
    gem_table_set(_t4255, gem_string("tag"), gem_string("table"));
    gem_table_set(_t4255, gem_string("entries"), gem_v_entries);
    GemVal _t4256 = _t4255;
    gem_pop_frame();
    return _t4256;
}

static GemVal gem_fn_make_table_entry(void *_env, GemVal *args, int argc) {
#line 40 "compiler/main.gem"
    GemVal gem_v_key = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_value = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("make_table_entry", "compiler/main.gem", 40);
    GemVal _t4257 = gem_table_new();
    gem_table_set(_t4257, gem_string("key"), gem_v_key);
    gem_table_set(_t4257, gem_string("value"), gem_v_value);
    GemVal _t4258 = _t4257;
    gem_pop_frame();
    return _t4258;
}

static GemVal gem_fn_module_name_from_path(void *_env, GemVal *args, int argc) {
#line 46 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("module_name_from_path", "compiler/main.gem", 46);
#line 47 "compiler/main.gem"
    GemVal _t4259[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4259, 1);
#line 48 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 49 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 50 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 51 "compiler/main.gem"
    GemVal _t4260[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4260, 2), gem_int(47)))) {
#line 52 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 54 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 56 "compiler/main.gem"
    GemVal _t4261[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4261, 3);
#line 57 "compiler/main.gem"
    GemVal _t4262[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4262, 1);
#line 58 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 59 "compiler/main.gem"
    GemVal _t4263 = gem_v_string;
    static GemICacheSlot _ic_609 = {0};
    GemVal _t4264[] = {gem_v_name, gem_string(".gem")};
    GemVal _t4265 = gem_table_get_cached(_t4263, "ends_with", &_ic_609);
        if (gem_truthy(_t4265.fn(_t4265.env, _t4264, 2))) {
#line 60 "compiler/main.gem"
    GemVal _t4266[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4266, 3);
        }
    }
    GemVal _t4267 = gem_v_name;
    gem_pop_frame();
    return _t4267;
}

static GemVal gem_fn_find_export_node(void *_env, GemVal *args, int argc) {
#line 66 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("find_export_node", "compiler/main.gem", 66);
#line 67 "compiler/main.gem"
    GemVal _t4268[] = {gem_v_stmts};
    GemVal gem_v_i = gem_sub(gem_len_fn(NULL, _t4268, 1), gem_int(1));
#line 68 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_ge(gem_v_i, gem_int(0)))) break;
#line 69 "compiler/main.gem"
    GemVal _t4269[] = {gem_table_get(gem_v_stmts, gem_v_i), gem_string("export")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4269, 2))) {
#line 70 "compiler/main.gem"
            GemVal _t4270 = gem_table_get(gem_v_stmts, gem_v_i);
            gem_pop_frame();
            return _t4270;
        }
#line 72 "compiler/main.gem"
        gem_v_i = gem_sub(gem_v_i, gem_int(1));
    }
    GemVal _t4271 = GEM_NIL;
    gem_pop_frame();
    return _t4271;
}

static GemVal gem_fn_collect_module_top_names(void *_env, GemVal *args, int argc) {
#line 77 "compiler/main.gem"
    GemVal gem_v_stmts = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("collect_module_top_names", "compiler/main.gem", 77);
#line 78 "compiler/main.gem"
    GemVal _t4272 = gem_table_new();
    GemVal gem_v_names = _t4272;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_items_1 = gem_v_stmts;
#line 79 "compiler/main.gem"
    GemVal gem_v__for_i_1 = gem_int(0);
#line 79 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4273[] = {gem_v__for_items_1};
        if (!gem_truthy(gem_lt(gem_v__for_i_1, gem_len_fn(NULL, _t4273, 1)))) break;
#line 79 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_1, gem_v__for_i_1);
#line 79 "compiler/main.gem"
        gem_v__for_i_1 = gem_add(gem_v__for_i_1, gem_int(1));
#line 80 "compiler/main.gem"
    GemVal _t4274[] = {gem_v_stmt, gem_string("fn_def")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4274, 2))) {
#line 81 "compiler/main.gem"
    GemVal _t4275 = gem_v_string;
    static GemICacheSlot _ic_610 = {0};
    GemVal _t4276 = gem_v_stmt;
    static GemICacheSlot _ic_611 = {0};
    GemVal _t4277[] = {gem_table_get_cached(_t4276, "name", &_ic_611), gem_string("_mod_")};
    GemVal _t4278 = gem_table_get_cached(_t4275, "starts_with", &_ic_610);
            if (gem_truthy(gem_not(_t4278.fn(_t4278.env, _t4277, 2)))) {
#line 82 "compiler/main.gem"
    GemVal _t4279 = gem_v_stmt;
    static GemICacheSlot _ic_612 = {0};
                gem_table_set(gem_v_names, gem_table_get_cached(_t4279, "name", &_ic_612), gem_bool(1));
            }
        } else {
#line 84 "compiler/main.gem"
    GemVal _t4280[] = {gem_v_stmt, gem_string("let")};
            if (gem_truthy(gem_fn_is_node(NULL, _t4280, 2))) {
#line 85 "compiler/main.gem"
    GemVal _t4281 = gem_v_string;
    static GemICacheSlot _ic_613 = {0};
    GemVal _t4282 = gem_v_stmt;
    static GemICacheSlot _ic_614 = {0};
    GemVal _t4283[] = {gem_table_get_cached(_t4282, "name", &_ic_614), gem_string("_mod_")};
    GemVal _t4284 = gem_table_get_cached(_t4281, "starts_with", &_ic_613);
                if (gem_truthy(gem_not(_t4284.fn(_t4284.env, _t4283, 2)))) {
#line 86 "compiler/main.gem"
    GemVal _t4285 = gem_v_stmt;
    static GemICacheSlot _ic_615 = {0};
                    gem_table_set(gem_v_names, gem_table_get_cached(_t4285, "name", &_ic_615), gem_bool(1));
                }
            }
        }
    }

    GemVal _t4286 = gem_v_names;
    gem_pop_frame();
    return _t4286;
}

static GemVal gem_fn_build_rename_map(void *_env, GemVal *args, int argc) {
#line 93 "compiler/main.gem"
    GemVal gem_v_top_names = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_prefix = (argc > 1) ? args[1] : GEM_NIL;
    gem_push_frame("build_rename_map", "compiler/main.gem", 93);
#line 94 "compiler/main.gem"
    GemVal _t4287 = gem_table_new();
    GemVal gem_v_rmap = _t4287;
#line 95 "compiler/main.gem"
    GemVal gem_v__for_tbl_2 = gem_v_top_names;
#line 95 "compiler/main.gem"
    GemVal _t4288[] = {gem_v__for_tbl_2};
    GemVal gem_v__for_len_2 = gem_len_fn(NULL, _t4288, 1);
#line 95 "compiler/main.gem"
    GemVal gem_v__for_i_2 = gem_int(0);
#line 95 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_2, gem_v__for_len_2))) break;
#line 95 "compiler/main.gem"
    GemVal _t4289[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v_name = gem_table_key_at_fn(NULL, _t4289, 2);
#line 95 "compiler/main.gem"
    GemVal _t4290[] = {gem_v__for_tbl_2, gem_v__for_i_2};
        GemVal gem_v__ = gem_table_val_at_fn(NULL, _t4290, 2);
#line 95 "compiler/main.gem"
        gem_v__for_i_2 = gem_add(gem_v__for_i_2, gem_int(1));
#line 96 "compiler/main.gem"
        gem_table_set(gem_v_rmap, gem_v_name, gem_add(gem_v_prefix, gem_v_name));
    }

    GemVal _t4291 = gem_v_rmap;
    gem_pop_frame();
    return _t4291;
}

static GemVal gem_fn_copy_set(void *_env, GemVal *args, int argc) {
#line 101 "compiler/main.gem"
    GemVal gem_v_s = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("copy_set", "compiler/main.gem", 101);
#line 102 "compiler/main.gem"
    GemVal _t4292 = gem_table_new();
    GemVal gem_v_result = _t4292;
#line 103 "compiler/main.gem"
    GemVal gem_v__for_tbl_3 = gem_v_s;
#line 103 "compiler/main.gem"
    GemVal _t4293[] = {gem_v__for_tbl_3};
    GemVal gem_v__for_len_3 = gem_len_fn(NULL, _t4293, 1);
#line 103 "compiler/main.gem"
    GemVal gem_v__for_i_3 = gem_int(0);
#line 103 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v__for_i_3, gem_v__for_len_3))) break;
#line 103 "compiler/main.gem"
    GemVal _t4294[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_k = gem_table_key_at_fn(NULL, _t4294, 2);
#line 103 "compiler/main.gem"
    GemVal _t4295[] = {gem_v__for_tbl_3, gem_v__for_i_3};
        GemVal gem_v_v = gem_table_val_at_fn(NULL, _t4295, 2);
#line 103 "compiler/main.gem"
        gem_v__for_i_3 = gem_add(gem_v__for_i_3, gem_int(1));
#line 104 "compiler/main.gem"
        gem_table_set(gem_v_result, gem_v_k, gem_v_v);
    }

    GemVal _t4296 = gem_v_result;
    gem_pop_frame();
    return _t4296;
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
            GemVal _t4297 = GEM_NIL;
            gem_pop_frame();
            return _t4297;
        }
#line 113 "compiler/main.gem"
    GemVal _t4298[] = {gem_v_node};
        if (gem_truthy(gem_neq(gem_type_fn(NULL, _t4298, 1), gem_string("table")))) {
#line 114 "compiler/main.gem"
            GemVal _t4299 = GEM_NIL;
            gem_pop_frame();
            return _t4299;
        }
#line 116 "compiler/main.gem"
    GemVal _t4300 = gem_v_node;
    static GemICacheSlot _ic_616 = {0};
        if (gem_truthy(gem_eq(gem_table_get_cached(_t4300, "tag", &_ic_616), GEM_NIL))) {
#line 117 "compiler/main.gem"
            GemVal _t4301 = GEM_NIL;
            gem_pop_frame();
            return _t4301;
        }
#line 120 "compiler/main.gem"
        GemVal gem_v__match_4 = gem_v_node;
    GemVal _t4302[] = {gem_v__match_4};
    GemVal _t4304;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4302, 1), gem_string("table")))) {
        _t4304 = gem_eq(gem_type_fn(NULL, _t4302, 1), gem_string("table"));
    } else {
        GemVal _t4303[] = {gem_v__match_4, gem_string("tag")};
        _t4304 = gem_has_key_fn(NULL, _t4303, 2);
    }
    GemVal _t4305;
    if (!gem_truthy(_t4304)) {
        _t4305 = _t4304;
    } else {
        _t4305 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("var"));
    }
    GemVal _t4307;
    if (!gem_truthy(_t4305)) {
        _t4307 = _t4305;
    } else {
        GemVal _t4306[] = {gem_v__match_4, gem_string("name")};
        _t4307 = gem_has_key_fn(NULL, _t4306, 2);
    }
        if (gem_truthy(_t4307)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 122 "compiler/main.gem"
    GemVal _t4308[] = {gem_v_rmap, gem_v_name};
    GemVal _t4310;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4308, 2))) {
        _t4310 = gem_has_key_fn(NULL, _t4308, 2);
    } else {
        GemVal _t4309[] = {gem_v_shadowed, gem_v_name};
        _t4310 = gem_not(gem_has_key_fn(NULL, _t4309, 2));
    }
            if (gem_truthy(_t4310)) {
#line 123 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4311[] = {gem_v__match_4};
    GemVal _t4313;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4311, 1), gem_string("table")))) {
        _t4313 = gem_eq(gem_type_fn(NULL, _t4311, 1), gem_string("table"));
    } else {
        GemVal _t4312[] = {gem_v__match_4, gem_string("tag")};
        _t4313 = gem_has_key_fn(NULL, _t4312, 2);
    }
    GemVal _t4314;
    if (!gem_truthy(_t4313)) {
        _t4314 = _t4313;
    } else {
        _t4314 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("fn_def"));
    }
    GemVal _t4316;
    if (!gem_truthy(_t4314)) {
        _t4316 = _t4314;
    } else {
        GemVal _t4315[] = {gem_v__match_4, gem_string("name")};
        _t4316 = gem_has_key_fn(NULL, _t4315, 2);
    }
    GemVal _t4318;
    if (!gem_truthy(_t4316)) {
        _t4318 = _t4316;
    } else {
        GemVal _t4317[] = {gem_v__match_4, gem_string("params")};
        _t4318 = gem_has_key_fn(NULL, _t4317, 2);
    }
    GemVal _t4320;
    if (!gem_truthy(_t4318)) {
        _t4320 = _t4318;
    } else {
        GemVal _t4319[] = {gem_v__match_4, gem_string("body")};
        _t4320 = gem_has_key_fn(NULL, _t4319, 2);
    }
        if (gem_truthy(_t4320)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 126 "compiler/main.gem"
    GemVal _t4321[] = {gem_v_rmap, gem_v_name};
            if (gem_truthy(gem_has_key_fn(NULL, _t4321, 2))) {
#line 127 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
            }
#line 129 "compiler/main.gem"
    GemVal _t4322[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4322, 1);
#line 130 "compiler/main.gem"
            GemVal gem_v__for_items_5 = gem_v_params;
#line 130 "compiler/main.gem"
            GemVal gem_v__for_i_5 = gem_int(0);
#line 130 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4323[] = {gem_v__for_items_5};
                if (!gem_truthy(gem_lt(gem_v__for_i_5, gem_len_fn(NULL, _t4323, 1)))) break;
#line 130 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_5, gem_v__for_i_5);
#line 130 "compiler/main.gem"
                gem_v__for_i_5 = gem_add(gem_v__for_i_5, gem_int(1));
#line 131 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 133 "compiler/main.gem"
    GemVal _t4324 = gem_v_node;
    static GemICacheSlot _ic_617 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4324, "rest_param", &_ic_617), GEM_NIL))) {
#line 134 "compiler/main.gem"
    GemVal _t4325 = gem_v_node;
    static GemICacheSlot _ic_618 = {0};
                gem_table_set(gem_v_inner, gem_table_get_cached(_t4325, "rest_param", &_ic_618), gem_bool(1));
            }
#line 136 "compiler/main.gem"
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
        _t4331 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("anon_fn"));
    }
    GemVal _t4333;
    if (!gem_truthy(_t4331)) {
        _t4333 = _t4331;
    } else {
        GemVal _t4332[] = {gem_v__match_4, gem_string("params")};
        _t4333 = gem_has_key_fn(NULL, _t4332, 2);
    }
    GemVal _t4335;
    if (!gem_truthy(_t4333)) {
        _t4335 = _t4333;
    } else {
        GemVal _t4334[] = {gem_v__match_4, gem_string("body")};
        _t4335 = gem_has_key_fn(NULL, _t4334, 2);
    }
        if (gem_truthy(_t4335)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_params = gem_table_get(gem_v__match_4, gem_string("params"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 138 "compiler/main.gem"
    GemVal _t4336[] = {gem_v_shadowed};
            GemVal gem_v_inner = gem_fn_copy_set(NULL, _t4336, 1);
#line 139 "compiler/main.gem"
            GemVal gem_v__for_items_6 = gem_v_params;
#line 139 "compiler/main.gem"
            GemVal gem_v__for_i_6 = gem_int(0);
#line 139 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4337[] = {gem_v__for_items_6};
                if (!gem_truthy(gem_lt(gem_v__for_i_6, gem_len_fn(NULL, _t4337, 1)))) break;
#line 139 "compiler/main.gem"
                GemVal gem_v_p = gem_table_get(gem_v__for_items_6, gem_v__for_i_6);
#line 139 "compiler/main.gem"
                gem_v__for_i_6 = gem_add(gem_v__for_i_6, gem_int(1));
#line 140 "compiler/main.gem"
                gem_table_set(gem_v_inner, gem_v_p, gem_bool(1));
            }

#line 142 "compiler/main.gem"
    GemVal _t4338 = gem_v_node;
    static GemICacheSlot _ic_619 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4338, "rest_param", &_ic_619), GEM_NIL))) {
#line 143 "compiler/main.gem"
    GemVal _t4339 = gem_v_node;
    static GemICacheSlot _ic_620 = {0};
                gem_table_set(gem_v_inner, gem_table_get_cached(_t4339, "rest_param", &_ic_620), gem_bool(1));
            }
#line 145 "compiler/main.gem"
    GemVal _t4340[] = {gem_v_body, gem_v_rmap, gem_v_inner};
            GemVal _t4341 = gem_fn_rename_stmts(NULL, _t4340, 3);
            gem_pop_frame();
            return _t4341;
        } else {
    GemVal _t4342[] = {gem_v__match_4};
    GemVal _t4344;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4342, 1), gem_string("table")))) {
        _t4344 = gem_eq(gem_type_fn(NULL, _t4342, 1), gem_string("table"));
    } else {
        GemVal _t4343[] = {gem_v__match_4, gem_string("tag")};
        _t4344 = gem_has_key_fn(NULL, _t4343, 2);
    }
    GemVal _t4345;
    if (!gem_truthy(_t4344)) {
        _t4345 = _t4344;
    } else {
        _t4345 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("let"));
    }
    GemVal _t4347;
    if (!gem_truthy(_t4345)) {
        _t4347 = _t4345;
    } else {
        GemVal _t4346[] = {gem_v__match_4, gem_string("name")};
        _t4347 = gem_has_key_fn(NULL, _t4346, 2);
    }
    GemVal _t4349;
    if (!gem_truthy(_t4347)) {
        _t4349 = _t4347;
    } else {
        GemVal _t4348[] = {gem_v__match_4, gem_string("value")};
        _t4349 = gem_has_key_fn(NULL, _t4348, 2);
    }
        if (gem_truthy(_t4349)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 147 "compiler/main.gem"
    GemVal _t4350[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4350, 3));
#line 148 "compiler/main.gem"
    GemVal _t4351[] = {gem_v_rmap, gem_v_name};
    GemVal _t4353;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4351, 2))) {
        _t4353 = gem_has_key_fn(NULL, _t4351, 2);
    } else {
        GemVal _t4352[] = {gem_v_shadowed, gem_v_name};
        _t4353 = gem_not(gem_has_key_fn(NULL, _t4352, 2));
    }
            if (gem_truthy(_t4353)) {
#line 149 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4354[] = {gem_v__match_4};
    GemVal _t4356;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4354, 1), gem_string("table")))) {
        _t4356 = gem_eq(gem_type_fn(NULL, _t4354, 1), gem_string("table"));
    } else {
        GemVal _t4355[] = {gem_v__match_4, gem_string("tag")};
        _t4356 = gem_has_key_fn(NULL, _t4355, 2);
    }
    GemVal _t4357;
    if (!gem_truthy(_t4356)) {
        _t4357 = _t4356;
    } else {
        _t4357 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("assign"));
    }
    GemVal _t4359;
    if (!gem_truthy(_t4357)) {
        _t4359 = _t4357;
    } else {
        GemVal _t4358[] = {gem_v__match_4, gem_string("name")};
        _t4359 = gem_has_key_fn(NULL, _t4358, 2);
    }
    GemVal _t4361;
    if (!gem_truthy(_t4359)) {
        _t4361 = _t4359;
    } else {
        GemVal _t4360[] = {gem_v__match_4, gem_string("value")};
        _t4361 = gem_has_key_fn(NULL, _t4360, 2);
    }
        if (gem_truthy(_t4361)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_name = gem_table_get(gem_v__match_4, gem_string("name"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 152 "compiler/main.gem"
    GemVal _t4362[] = {gem_v_value, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4362, 3));
#line 153 "compiler/main.gem"
    GemVal _t4363[] = {gem_v_rmap, gem_v_name};
    GemVal _t4365;
    if (!gem_truthy(gem_has_key_fn(NULL, _t4363, 2))) {
        _t4365 = gem_has_key_fn(NULL, _t4363, 2);
    } else {
        GemVal _t4364[] = {gem_v_shadowed, gem_v_name};
        _t4365 = gem_not(gem_has_key_fn(NULL, _t4364, 2));
    }
            if (gem_truthy(_t4365)) {
#line 154 "compiler/main.gem"
                gem_table_set(gem_v_node, gem_string("name"), gem_table_get(gem_v_rmap, gem_v_name));
                gem_pop_frame();
                return GEM_NIL;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4366[] = {gem_v__match_4};
    GemVal _t4368;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4366, 1), gem_string("table")))) {
        _t4368 = gem_eq(gem_type_fn(NULL, _t4366, 1), gem_string("table"));
    } else {
        GemVal _t4367[] = {gem_v__match_4, gem_string("tag")};
        _t4368 = gem_has_key_fn(NULL, _t4367, 2);
    }
    GemVal _t4369;
    if (!gem_truthy(_t4368)) {
        _t4369 = _t4368;
    } else {
        _t4369 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("call"));
    }
    GemVal _t4371;
    if (!gem_truthy(_t4369)) {
        _t4371 = _t4369;
    } else {
        GemVal _t4370[] = {gem_v__match_4, gem_string("func")};
        _t4371 = gem_has_key_fn(NULL, _t4370, 2);
    }
    GemVal _t4373;
    if (!gem_truthy(_t4371)) {
        _t4373 = _t4371;
    } else {
        GemVal _t4372[] = {gem_v__match_4, gem_string("args")};
        _t4373 = gem_has_key_fn(NULL, _t4372, 2);
    }
        if (gem_truthy(_t4373)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_func = gem_table_get(gem_v__match_4, gem_string("func"));
#line 0 "compiler/main.gem"
            GemVal gem_v_args = gem_table_get(gem_v__match_4, gem_string("args"));
#line 157 "compiler/main.gem"
    GemVal _t4374[] = {gem_v_func, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4374, 3));
            {
#line 158 "compiler/main.gem"
                GemVal gem_v__for_items_7 = gem_v_args;
#line 158 "compiler/main.gem"
                GemVal gem_v__for_i_7 = gem_int(0);
#line 158 "compiler/main.gem"
                while (1) {
                    gem_yield_check();
                    GemVal _t4375[] = {gem_v__for_items_7};
                    if (!gem_truthy(gem_lt(gem_v__for_i_7, gem_len_fn(NULL, _t4375, 1)))) break;
#line 158 "compiler/main.gem"
                    GemVal gem_v_a = gem_table_get(gem_v__for_items_7, gem_v__for_i_7);
#line 158 "compiler/main.gem"
                    gem_v__for_i_7 = gem_add(gem_v__for_i_7, gem_int(1));
#line 159 "compiler/main.gem"
    GemVal _t4376[] = {gem_v_a, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4376, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4377[] = {gem_v__match_4};
    GemVal _t4379;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4377, 1), gem_string("table")))) {
        _t4379 = gem_eq(gem_type_fn(NULL, _t4377, 1), gem_string("table"));
    } else {
        GemVal _t4378[] = {gem_v__match_4, gem_string("tag")};
        _t4379 = gem_has_key_fn(NULL, _t4378, 2);
    }
    GemVal _t4380;
    if (!gem_truthy(_t4379)) {
        _t4380 = _t4379;
    } else {
        _t4380 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("binop"));
    }
    GemVal _t4382;
    if (!gem_truthy(_t4380)) {
        _t4382 = _t4380;
    } else {
        GemVal _t4381[] = {gem_v__match_4, gem_string("left")};
        _t4382 = gem_has_key_fn(NULL, _t4381, 2);
    }
    GemVal _t4384;
    if (!gem_truthy(_t4382)) {
        _t4384 = _t4382;
    } else {
        GemVal _t4383[] = {gem_v__match_4, gem_string("right")};
        _t4384 = gem_has_key_fn(NULL, _t4383, 2);
    }
        if (gem_truthy(_t4384)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_left = gem_table_get(gem_v__match_4, gem_string("left"));
#line 0 "compiler/main.gem"
            GemVal gem_v_right = gem_table_get(gem_v__match_4, gem_string("right"));
#line 162 "compiler/main.gem"
    GemVal _t4385[] = {gem_v_left, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4385, 3));
#line 163 "compiler/main.gem"
            GemVal _t4386 = gem_v_right;
            GemVal _t4387 = gem_v_rmap;
            GemVal _t4388 = gem_v_shadowed;
            gem_v_node = _t4386;
            gem_v_rmap = _t4387;
            gem_v_shadowed = _t4388;
            continue;
        } else {
    GemVal _t4389[] = {gem_v__match_4};
    GemVal _t4391;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4389, 1), gem_string("table")))) {
        _t4391 = gem_eq(gem_type_fn(NULL, _t4389, 1), gem_string("table"));
    } else {
        GemVal _t4390[] = {gem_v__match_4, gem_string("tag")};
        _t4391 = gem_has_key_fn(NULL, _t4390, 2);
    }
    GemVal _t4392;
    if (!gem_truthy(_t4391)) {
        _t4392 = _t4391;
    } else {
        _t4392 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("unop"));
    }
    GemVal _t4394;
    if (!gem_truthy(_t4392)) {
        _t4394 = _t4392;
    } else {
        GemVal _t4393[] = {gem_v__match_4, gem_string("expr")};
        _t4394 = gem_has_key_fn(NULL, _t4393, 2);
    }
        if (gem_truthy(_t4394)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_expr = gem_table_get(gem_v__match_4, gem_string("expr"));
#line 165 "compiler/main.gem"
            GemVal _t4395 = gem_v_expr;
            GemVal _t4396 = gem_v_rmap;
            GemVal _t4397 = gem_v_shadowed;
            gem_v_node = _t4395;
            gem_v_rmap = _t4396;
            gem_v_shadowed = _t4397;
            continue;
        } else {
    GemVal _t4398[] = {gem_v__match_4};
    GemVal _t4400;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4398, 1), gem_string("table")))) {
        _t4400 = gem_eq(gem_type_fn(NULL, _t4398, 1), gem_string("table"));
    } else {
        GemVal _t4399[] = {gem_v__match_4, gem_string("tag")};
        _t4400 = gem_has_key_fn(NULL, _t4399, 2);
    }
    GemVal _t4401;
    if (!gem_truthy(_t4400)) {
        _t4401 = _t4400;
    } else {
        _t4401 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot"));
    }
    GemVal _t4403;
    if (!gem_truthy(_t4401)) {
        _t4403 = _t4401;
    } else {
        GemVal _t4402[] = {gem_v__match_4, gem_string("object")};
        _t4403 = gem_has_key_fn(NULL, _t4402, 2);
    }
        if (gem_truthy(_t4403)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 167 "compiler/main.gem"
            GemVal _t4404 = gem_v_object;
            GemVal _t4405 = gem_v_rmap;
            GemVal _t4406 = gem_v_shadowed;
            gem_v_node = _t4404;
            gem_v_rmap = _t4405;
            gem_v_shadowed = _t4406;
            continue;
        } else {
    GemVal _t4407[] = {gem_v__match_4};
    GemVal _t4409;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4407, 1), gem_string("table")))) {
        _t4409 = gem_eq(gem_type_fn(NULL, _t4407, 1), gem_string("table"));
    } else {
        GemVal _t4408[] = {gem_v__match_4, gem_string("tag")};
        _t4409 = gem_has_key_fn(NULL, _t4408, 2);
    }
    GemVal _t4410;
    if (!gem_truthy(_t4409)) {
        _t4410 = _t4409;
    } else {
        _t4410 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index"));
    }
    GemVal _t4412;
    if (!gem_truthy(_t4410)) {
        _t4412 = _t4410;
    } else {
        GemVal _t4411[] = {gem_v__match_4, gem_string("object")};
        _t4412 = gem_has_key_fn(NULL, _t4411, 2);
    }
    GemVal _t4414;
    if (!gem_truthy(_t4412)) {
        _t4414 = _t4412;
    } else {
        GemVal _t4413[] = {gem_v__match_4, gem_string("key")};
        _t4414 = gem_has_key_fn(NULL, _t4413, 2);
    }
        if (gem_truthy(_t4414)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 169 "compiler/main.gem"
    GemVal _t4415[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4415, 3));
#line 170 "compiler/main.gem"
            GemVal _t4416 = gem_v_key;
            GemVal _t4417 = gem_v_rmap;
            GemVal _t4418 = gem_v_shadowed;
            gem_v_node = _t4416;
            gem_v_rmap = _t4417;
            gem_v_shadowed = _t4418;
            continue;
        } else {
    GemVal _t4419[] = {gem_v__match_4};
    GemVal _t4421;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4419, 1), gem_string("table")))) {
        _t4421 = gem_eq(gem_type_fn(NULL, _t4419, 1), gem_string("table"));
    } else {
        GemVal _t4420[] = {gem_v__match_4, gem_string("tag")};
        _t4421 = gem_has_key_fn(NULL, _t4420, 2);
    }
    GemVal _t4422;
    if (!gem_truthy(_t4421)) {
        _t4422 = _t4421;
    } else {
        _t4422 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("dot_assign"));
    }
    GemVal _t4424;
    if (!gem_truthy(_t4422)) {
        _t4424 = _t4422;
    } else {
        GemVal _t4423[] = {gem_v__match_4, gem_string("object")};
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
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 172 "compiler/main.gem"
    GemVal _t4427[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4427, 3));
#line 173 "compiler/main.gem"
            GemVal _t4428 = gem_v_value;
            GemVal _t4429 = gem_v_rmap;
            GemVal _t4430 = gem_v_shadowed;
            gem_v_node = _t4428;
            gem_v_rmap = _t4429;
            gem_v_shadowed = _t4430;
            continue;
        } else {
    GemVal _t4431[] = {gem_v__match_4};
    GemVal _t4433;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4431, 1), gem_string("table")))) {
        _t4433 = gem_eq(gem_type_fn(NULL, _t4431, 1), gem_string("table"));
    } else {
        GemVal _t4432[] = {gem_v__match_4, gem_string("tag")};
        _t4433 = gem_has_key_fn(NULL, _t4432, 2);
    }
    GemVal _t4434;
    if (!gem_truthy(_t4433)) {
        _t4434 = _t4433;
    } else {
        _t4434 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("index_assign"));
    }
    GemVal _t4436;
    if (!gem_truthy(_t4434)) {
        _t4436 = _t4434;
    } else {
        GemVal _t4435[] = {gem_v__match_4, gem_string("object")};
        _t4436 = gem_has_key_fn(NULL, _t4435, 2);
    }
    GemVal _t4438;
    if (!gem_truthy(_t4436)) {
        _t4438 = _t4436;
    } else {
        GemVal _t4437[] = {gem_v__match_4, gem_string("key")};
        _t4438 = gem_has_key_fn(NULL, _t4437, 2);
    }
    GemVal _t4440;
    if (!gem_truthy(_t4438)) {
        _t4440 = _t4438;
    } else {
        GemVal _t4439[] = {gem_v__match_4, gem_string("value")};
        _t4440 = gem_has_key_fn(NULL, _t4439, 2);
    }
        if (gem_truthy(_t4440)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_object = gem_table_get(gem_v__match_4, gem_string("object"));
#line 0 "compiler/main.gem"
            GemVal gem_v_key = gem_table_get(gem_v__match_4, gem_string("key"));
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 175 "compiler/main.gem"
    GemVal _t4441[] = {gem_v_object, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4441, 3));
#line 176 "compiler/main.gem"
    GemVal _t4442[] = {gem_v_key, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4442, 3));
#line 177 "compiler/main.gem"
            GemVal _t4443 = gem_v_value;
            GemVal _t4444 = gem_v_rmap;
            GemVal _t4445 = gem_v_shadowed;
            gem_v_node = _t4443;
            gem_v_rmap = _t4444;
            gem_v_shadowed = _t4445;
            continue;
        } else {
    GemVal _t4446[] = {gem_v__match_4};
    GemVal _t4448;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4446, 1), gem_string("table")))) {
        _t4448 = gem_eq(gem_type_fn(NULL, _t4446, 1), gem_string("table"));
    } else {
        GemVal _t4447[] = {gem_v__match_4, gem_string("tag")};
        _t4448 = gem_has_key_fn(NULL, _t4447, 2);
    }
    GemVal _t4449;
    if (!gem_truthy(_t4448)) {
        _t4449 = _t4448;
    } else {
        _t4449 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("if"));
    }
    GemVal _t4451;
    if (!gem_truthy(_t4449)) {
        _t4451 = _t4449;
    } else {
        GemVal _t4450[] = {gem_v__match_4, gem_string("cond")};
        _t4451 = gem_has_key_fn(NULL, _t4450, 2);
    }
        if (gem_truthy(_t4451)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 179 "compiler/main.gem"
    GemVal _t4452[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4452, 3));
#line 180 "compiler/main.gem"
    GemVal _t4453 = gem_v_node;
    static GemICacheSlot _ic_621 = {0};
    GemVal _t4454[] = {gem_table_get_cached(_t4453, "then", &_ic_621), gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_stmts(NULL, _t4454, 3));
#line 181 "compiler/main.gem"
    GemVal _t4455 = gem_v_node;
    static GemICacheSlot _ic_622 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4455, "else", &_ic_622), GEM_NIL))) {
#line 182 "compiler/main.gem"
    GemVal _t4456 = gem_v_node;
    static GemICacheSlot _ic_623 = {0};
    GemVal _t4457[] = {gem_table_get_cached(_t4456, "else", &_ic_623), gem_v_rmap, gem_v_shadowed};
                GemVal _t4458 = gem_fn_rename_stmts(NULL, _t4457, 3);
                gem_pop_frame();
                return _t4458;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4459[] = {gem_v__match_4};
    GemVal _t4461;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4459, 1), gem_string("table")))) {
        _t4461 = gem_eq(gem_type_fn(NULL, _t4459, 1), gem_string("table"));
    } else {
        GemVal _t4460[] = {gem_v__match_4, gem_string("tag")};
        _t4461 = gem_has_key_fn(NULL, _t4460, 2);
    }
    GemVal _t4462;
    if (!gem_truthy(_t4461)) {
        _t4462 = _t4461;
    } else {
        _t4462 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("while"));
    }
    GemVal _t4464;
    if (!gem_truthy(_t4462)) {
        _t4464 = _t4462;
    } else {
        GemVal _t4463[] = {gem_v__match_4, gem_string("cond")};
        _t4464 = gem_has_key_fn(NULL, _t4463, 2);
    }
    GemVal _t4466;
    if (!gem_truthy(_t4464)) {
        _t4466 = _t4464;
    } else {
        GemVal _t4465[] = {gem_v__match_4, gem_string("body")};
        _t4466 = gem_has_key_fn(NULL, _t4465, 2);
    }
        if (gem_truthy(_t4466)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_cond = gem_table_get(gem_v__match_4, gem_string("cond"));
#line 0 "compiler/main.gem"
            GemVal gem_v_body = gem_table_get(gem_v__match_4, gem_string("body"));
#line 185 "compiler/main.gem"
    GemVal _t4467[] = {gem_v_cond, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4467, 3));
#line 186 "compiler/main.gem"
    GemVal _t4468[] = {gem_v_body, gem_v_rmap, gem_v_shadowed};
            GemVal _t4469 = gem_fn_rename_stmts(NULL, _t4468, 3);
            gem_pop_frame();
            return _t4469;
        } else {
    GemVal _t4470[] = {gem_v__match_4};
    GemVal _t4472;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4470, 1), gem_string("table")))) {
        _t4472 = gem_eq(gem_type_fn(NULL, _t4470, 1), gem_string("table"));
    } else {
        GemVal _t4471[] = {gem_v__match_4, gem_string("tag")};
        _t4472 = gem_has_key_fn(NULL, _t4471, 2);
    }
    GemVal _t4473;
    if (!gem_truthy(_t4472)) {
        _t4473 = _t4472;
    } else {
        _t4473 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("match"));
    }
    GemVal _t4475;
    if (!gem_truthy(_t4473)) {
        _t4475 = _t4473;
    } else {
        GemVal _t4474[] = {gem_v__match_4, gem_string("target")};
        _t4475 = gem_has_key_fn(NULL, _t4474, 2);
    }
    GemVal _t4477;
    if (!gem_truthy(_t4475)) {
        _t4477 = _t4475;
    } else {
        GemVal _t4476[] = {gem_v__match_4, gem_string("whens")};
        _t4477 = gem_has_key_fn(NULL, _t4476, 2);
    }
        if (gem_truthy(_t4477)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_target = gem_table_get(gem_v__match_4, gem_string("target"));
#line 0 "compiler/main.gem"
            GemVal gem_v_whens = gem_table_get(gem_v__match_4, gem_string("whens"));
#line 188 "compiler/main.gem"
    GemVal _t4478[] = {gem_v_target, gem_v_rmap, gem_v_shadowed};
            (void)(gem_fn_rename_node(NULL, _t4478, 3));
#line 189 "compiler/main.gem"
            GemVal gem_v__for_items_8 = gem_v_whens;
#line 189 "compiler/main.gem"
            GemVal gem_v__for_i_8 = gem_int(0);
#line 189 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4479[] = {gem_v__for_items_8};
                if (!gem_truthy(gem_lt(gem_v__for_i_8, gem_len_fn(NULL, _t4479, 1)))) break;
#line 189 "compiler/main.gem"
                GemVal gem_v_w = gem_table_get(gem_v__for_items_8, gem_v__for_i_8);
#line 189 "compiler/main.gem"
                gem_v__for_i_8 = gem_add(gem_v__for_i_8, gem_int(1));
#line 190 "compiler/main.gem"
    GemVal _t4480 = gem_v_w;
    static GemICacheSlot _ic_624 = {0};
    GemVal _t4481[] = {gem_table_get_cached(_t4480, "value", &_ic_624), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4481, 3));
#line 191 "compiler/main.gem"
    GemVal _t4482 = gem_v_w;
    static GemICacheSlot _ic_625 = {0};
                if (gem_truthy(gem_neq(gem_table_get_cached(_t4482, "bindings", &_ic_625), GEM_NIL))) {
#line 192 "compiler/main.gem"
    GemVal _t4483 = gem_v_w;
    static GemICacheSlot _ic_626 = {0};
                    GemVal gem_v__for_items_9 = gem_table_get_cached(_t4483, "bindings", &_ic_626);
#line 192 "compiler/main.gem"
                    GemVal gem_v__for_i_9 = gem_int(0);
#line 192 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4484[] = {gem_v__for_items_9};
                        if (!gem_truthy(gem_lt(gem_v__for_i_9, gem_len_fn(NULL, _t4484, 1)))) break;
#line 192 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_9, gem_v__for_i_9);
#line 192 "compiler/main.gem"
                        gem_v__for_i_9 = gem_add(gem_v__for_i_9, gem_int(1));
#line 193 "compiler/main.gem"
    GemVal _t4485[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4485, 3));
                    }

                }
#line 196 "compiler/main.gem"
    GemVal _t4486 = gem_v_w;
    static GemICacheSlot _ic_627 = {0};
    GemVal _t4487[] = {gem_table_get_cached(_t4486, "body", &_ic_627), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4487, 3));
            }

#line 198 "compiler/main.gem"
    GemVal _t4488 = gem_v_node;
    static GemICacheSlot _ic_628 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4488, "else", &_ic_628), GEM_NIL))) {
#line 199 "compiler/main.gem"
    GemVal _t4489 = gem_v_node;
    static GemICacheSlot _ic_629 = {0};
    GemVal _t4490[] = {gem_table_get_cached(_t4489, "else", &_ic_629), gem_v_rmap, gem_v_shadowed};
                GemVal _t4491 = gem_fn_rename_stmts(NULL, _t4490, 3);
                gem_pop_frame();
                return _t4491;
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
        _t4495 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("receive_match"));
    }
    GemVal _t4497;
    if (!gem_truthy(_t4495)) {
        _t4497 = _t4495;
    } else {
        GemVal _t4496[] = {gem_v__match_4, gem_string("arms")};
        _t4497 = gem_has_key_fn(NULL, _t4496, 2);
    }
        if (gem_truthy(_t4497)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_arms = gem_table_get(gem_v__match_4, gem_string("arms"));
#line 202 "compiler/main.gem"
            GemVal gem_v__for_items_10 = gem_v_arms;
#line 202 "compiler/main.gem"
            GemVal gem_v__for_i_10 = gem_int(0);
#line 202 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4498[] = {gem_v__for_items_10};
                if (!gem_truthy(gem_lt(gem_v__for_i_10, gem_len_fn(NULL, _t4498, 1)))) break;
#line 202 "compiler/main.gem"
                GemVal gem_v_arm = gem_table_get(gem_v__for_items_10, gem_v__for_i_10);
#line 202 "compiler/main.gem"
                gem_v__for_i_10 = gem_add(gem_v__for_i_10, gem_int(1));
#line 203 "compiler/main.gem"
    GemVal _t4499 = gem_v_arm;
    static GemICacheSlot _ic_630 = {0};
    GemVal _t4500 = gem_table_get_cached(_t4499, "pattern", &_ic_630);
    static GemICacheSlot _ic_631 = {0};
    GemVal _t4501[] = {gem_table_get_cached(_t4500, "condition", &_ic_631), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4501, 3));
#line 204 "compiler/main.gem"
    GemVal _t4502 = gem_v_arm;
    static GemICacheSlot _ic_632 = {0};
    GemVal _t4503 = gem_table_get_cached(_t4502, "pattern", &_ic_632);
    static GemICacheSlot _ic_633 = {0};
                if (gem_truthy(gem_neq(gem_table_get_cached(_t4503, "bindings", &_ic_633), GEM_NIL))) {
#line 205 "compiler/main.gem"
    GemVal _t4504 = gem_v_arm;
    static GemICacheSlot _ic_634 = {0};
    GemVal _t4505 = gem_table_get_cached(_t4504, "pattern", &_ic_634);
    static GemICacheSlot _ic_635 = {0};
                    GemVal gem_v__for_items_11 = gem_table_get_cached(_t4505, "bindings", &_ic_635);
#line 205 "compiler/main.gem"
                    GemVal gem_v__for_i_11 = gem_int(0);
#line 205 "compiler/main.gem"
                    while (1) {
                        gem_yield_check();
                        GemVal _t4506[] = {gem_v__for_items_11};
                        if (!gem_truthy(gem_lt(gem_v__for_i_11, gem_len_fn(NULL, _t4506, 1)))) break;
#line 205 "compiler/main.gem"
                        GemVal gem_v_b = gem_table_get(gem_v__for_items_11, gem_v__for_i_11);
#line 205 "compiler/main.gem"
                        gem_v__for_i_11 = gem_add(gem_v__for_i_11, gem_int(1));
#line 206 "compiler/main.gem"
    GemVal _t4507[] = {gem_v_b, gem_v_rmap, gem_v_shadowed};
                        (void)(gem_fn_rename_node(NULL, _t4507, 3));
                    }

                }
#line 209 "compiler/main.gem"
    GemVal _t4508 = gem_v_arm;
    static GemICacheSlot _ic_636 = {0};
    GemVal _t4509[] = {gem_table_get_cached(_t4508, "body", &_ic_636), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_stmts(NULL, _t4509, 3));
            }

#line 211 "compiler/main.gem"
    GemVal _t4510 = gem_v_node;
    static GemICacheSlot _ic_637 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4510, "after_ms", &_ic_637), GEM_NIL))) {
#line 212 "compiler/main.gem"
    GemVal _t4511 = gem_v_node;
    static GemICacheSlot _ic_638 = {0};
    GemVal _t4512[] = {gem_table_get_cached(_t4511, "after_ms", &_ic_638), gem_v_rmap, gem_v_shadowed};
                (void)(gem_fn_rename_node(NULL, _t4512, 3));
            }
#line 214 "compiler/main.gem"
    GemVal _t4513 = gem_v_node;
    static GemICacheSlot _ic_639 = {0};
            if (gem_truthy(gem_neq(gem_table_get_cached(_t4513, "after_body", &_ic_639), GEM_NIL))) {
#line 215 "compiler/main.gem"
    GemVal _t4514 = gem_v_node;
    static GemICacheSlot _ic_640 = {0};
    GemVal _t4515[] = {gem_table_get_cached(_t4514, "after_body", &_ic_640), gem_v_rmap, gem_v_shadowed};
                GemVal _t4516 = gem_fn_rename_stmts(NULL, _t4515, 3);
                gem_pop_frame();
                return _t4516;
            } else {
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
        _t4520 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("return"));
    }
    GemVal _t4522;
    if (!gem_truthy(_t4520)) {
        _t4522 = _t4520;
    } else {
        GemVal _t4521[] = {gem_v__match_4, gem_string("value")};
        _t4522 = gem_has_key_fn(NULL, _t4521, 2);
    }
        if (gem_truthy(_t4522)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_value = gem_table_get(gem_v__match_4, gem_string("value"));
#line 218 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_value, GEM_NIL))) {
#line 219 "compiler/main.gem"
                GemVal _t4523 = gem_v_value;
                GemVal _t4524 = gem_v_rmap;
                GemVal _t4525 = gem_v_shadowed;
                gem_v_node = _t4523;
                gem_v_rmap = _t4524;
                gem_v_shadowed = _t4525;
                continue;
            } else {
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4526[] = {gem_v__match_4};
    GemVal _t4528;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4526, 1), gem_string("table")))) {
        _t4528 = gem_eq(gem_type_fn(NULL, _t4526, 1), gem_string("table"));
    } else {
        GemVal _t4527[] = {gem_v__match_4, gem_string("tag")};
        _t4528 = gem_has_key_fn(NULL, _t4527, 2);
    }
    GemVal _t4529;
    if (!gem_truthy(_t4528)) {
        _t4529 = _t4528;
    } else {
        _t4529 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("table"));
    }
    GemVal _t4531;
    if (!gem_truthy(_t4529)) {
        _t4531 = _t4529;
    } else {
        GemVal _t4530[] = {gem_v__match_4, gem_string("entries")};
        _t4531 = gem_has_key_fn(NULL, _t4530, 2);
    }
        if (gem_truthy(_t4531)) {
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
                    GemVal _t4532[] = {gem_v__for_items_12};
                    if (!gem_truthy(gem_lt(gem_v__for_i_12, gem_len_fn(NULL, _t4532, 1)))) break;
#line 222 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_12, gem_v__for_i_12);
#line 222 "compiler/main.gem"
                    gem_v__for_i_12 = gem_add(gem_v__for_i_12, gem_int(1));
#line 223 "compiler/main.gem"
    GemVal _t4533 = gem_v_e;
    static GemICacheSlot _ic_641 = {0};
    GemVal _t4534[] = {gem_table_get_cached(_t4533, "value", &_ic_641), gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4534, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4535[] = {gem_v__match_4};
    GemVal _t4537;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4535, 1), gem_string("table")))) {
        _t4537 = gem_eq(gem_type_fn(NULL, _t4535, 1), gem_string("table"));
    } else {
        GemVal _t4536[] = {gem_v__match_4, gem_string("tag")};
        _t4537 = gem_has_key_fn(NULL, _t4536, 2);
    }
    GemVal _t4538;
    if (!gem_truthy(_t4537)) {
        _t4538 = _t4537;
    } else {
        _t4538 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("array"));
    }
    GemVal _t4540;
    if (!gem_truthy(_t4538)) {
        _t4540 = _t4538;
    } else {
        GemVal _t4539[] = {gem_v__match_4, gem_string("elements")};
        _t4540 = gem_has_key_fn(NULL, _t4539, 2);
    }
        if (gem_truthy(_t4540)) {
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
                    GemVal _t4541[] = {gem_v__for_items_13};
                    if (!gem_truthy(gem_lt(gem_v__for_i_13, gem_len_fn(NULL, _t4541, 1)))) break;
#line 226 "compiler/main.gem"
                    GemVal gem_v_e = gem_table_get(gem_v__for_items_13, gem_v__for_i_13);
#line 226 "compiler/main.gem"
                    gem_v__for_i_13 = gem_add(gem_v__for_i_13, gem_int(1));
#line 227 "compiler/main.gem"
    GemVal _t4542[] = {gem_v_e, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4542, 3));
                }
                gem_pop_frame();
                return GEM_NIL;
            }
        } else {
    GemVal _t4543[] = {gem_v__match_4};
    GemVal _t4545;
    if (!gem_truthy(gem_eq(gem_type_fn(NULL, _t4543, 1), gem_string("table")))) {
        _t4545 = gem_eq(gem_type_fn(NULL, _t4543, 1), gem_string("table"));
    } else {
        GemVal _t4544[] = {gem_v__match_4, gem_string("tag")};
        _t4545 = gem_has_key_fn(NULL, _t4544, 2);
    }
    GemVal _t4546;
    if (!gem_truthy(_t4545)) {
        _t4546 = _t4545;
    } else {
        _t4546 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("interp"));
    }
    GemVal _t4548;
    if (!gem_truthy(_t4546)) {
        _t4548 = _t4546;
    } else {
        GemVal _t4547[] = {gem_v__match_4, gem_string("parts")};
        _t4548 = gem_has_key_fn(NULL, _t4547, 2);
    }
        if (gem_truthy(_t4548)) {
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
                    GemVal _t4549[] = {gem_v__for_items_14};
                    if (!gem_truthy(gem_lt(gem_v__for_i_14, gem_len_fn(NULL, _t4549, 1)))) break;
#line 230 "compiler/main.gem"
                    GemVal gem_v_p = gem_table_get(gem_v__for_items_14, gem_v__for_i_14);
#line 230 "compiler/main.gem"
                    gem_v__for_i_14 = gem_add(gem_v__for_i_14, gem_int(1));
#line 231 "compiler/main.gem"
    GemVal _t4550[] = {gem_v_p, gem_v_rmap, gem_v_shadowed};
                    (void)(gem_fn_rename_node(NULL, _t4550, 3));
                }
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
        _t4554 = gem_eq(gem_table_get(gem_v__match_4, gem_string("tag")), gem_string("block"));
    }
    GemVal _t4556;
    if (!gem_truthy(_t4554)) {
        _t4556 = _t4554;
    } else {
        GemVal _t4555[] = {gem_v__match_4, gem_string("stmts")};
        _t4556 = gem_has_key_fn(NULL, _t4555, 2);
    }
        if (gem_truthy(_t4556)) {
#line 0 "compiler/main.gem"
            GemVal gem_v_stmts = gem_table_get(gem_v__match_4, gem_string("stmts"));
#line 234 "compiler/main.gem"
    GemVal _t4557[] = {gem_v_stmts, gem_v_rmap, gem_v_shadowed};
            GemVal _t4558 = gem_fn_rename_stmts(NULL, _t4557, 3);
            gem_pop_frame();
            return _t4558;
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
    GemVal _t4559[] = {gem_v_shadowed};
    GemVal gem_v_current = gem_fn_copy_set(NULL, _t4559, 1);
    {
#line 240 "compiler/main.gem"
        GemVal gem_v__for_items_15 = gem_v_stmts;
#line 240 "compiler/main.gem"
        GemVal gem_v__for_i_15 = gem_int(0);
#line 240 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4560[] = {gem_v__for_items_15};
            if (!gem_truthy(gem_lt(gem_v__for_i_15, gem_len_fn(NULL, _t4560, 1)))) break;
#line 240 "compiler/main.gem"
            GemVal gem_v_stmt = gem_table_get(gem_v__for_items_15, gem_v__for_i_15);
#line 240 "compiler/main.gem"
            gem_v__for_i_15 = gem_add(gem_v__for_i_15, gem_int(1));
#line 241 "compiler/main.gem"
    GemVal _t4561[] = {gem_v_stmt, gem_v_rmap, gem_v_current};
            (void)(gem_fn_rename_node(NULL, _t4561, 3));
#line 242 "compiler/main.gem"
    GemVal _t4562[] = {gem_v_stmt, gem_string("let")};
    GemVal _t4565;
    if (!gem_truthy(gem_fn_is_node(NULL, _t4562, 2))) {
        _t4565 = gem_fn_is_node(NULL, _t4562, 2);
    } else {
        GemVal _t4563 = gem_v_stmt;
        static GemICacheSlot _ic_642 = {0};
        GemVal _t4564[] = {gem_v_rmap, gem_table_get_cached(_t4563, "name", &_ic_642)};
        _t4565 = gem_not(gem_has_key_fn(NULL, _t4564, 2));
    }
            if (gem_truthy(_t4565)) {
#line 243 "compiler/main.gem"
    GemVal _t4566 = gem_v_stmt;
    static GemICacheSlot _ic_643 = {0};
                gem_table_set(gem_v_current, gem_table_get_cached(_t4566, "name", &_ic_643), gem_bool(1));
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
    GemVal _t4567[] = {gem_v_stmts};
    GemVal gem_v_top_names = gem_fn_collect_module_top_names(NULL, _t4567, 1);
#line 251 "compiler/main.gem"
    GemVal _t4568[] = {gem_v_top_names, gem_v_prefix};
    GemVal gem_v_rmap = gem_fn_build_rename_map(NULL, _t4568, 2);
#line 253 "compiler/main.gem"
    GemVal _t4569 = gem_table_new();
    GemVal _t4570[] = {gem_v_stmts, gem_v_rmap, _t4569};
    (void)(gem_fn_rename_stmts(NULL, _t4570, 3));
#line 255 "compiler/main.gem"
    GemVal _t4571 = gem_table_new();
    GemVal gem_v_result = _t4571;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_items_16 = gem_v_stmts;
#line 256 "compiler/main.gem"
    GemVal gem_v__for_i_16 = gem_int(0);
#line 256 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4572[] = {gem_v__for_items_16};
        if (!gem_truthy(gem_lt(gem_v__for_i_16, gem_len_fn(NULL, _t4572, 1)))) break;
#line 256 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_16, gem_v__for_i_16);
#line 256 "compiler/main.gem"
        gem_v__for_i_16 = gem_add(gem_v__for_i_16, gem_int(1));
#line 257 "compiler/main.gem"
    GemVal _t4573[] = {gem_v_stmt, gem_string("export")};
        if (gem_truthy(gem_not(gem_fn_is_node(NULL, _t4573, 2)))) {
#line 258 "compiler/main.gem"
    GemVal _t4574[] = {gem_v_result, gem_v_stmt};
            (void)(gem_push_fn(NULL, _t4574, 2));
        }
    }

#line 262 "compiler/main.gem"
    GemVal _t4575 = gem_v_export_node;
    static GemICacheSlot _ic_644 = {0};
    GemVal gem_v_exported = gem_table_get_cached(_t4575, "names", &_ic_644);
#line 264 "compiler/main.gem"
    GemVal _t4576 = gem_v_load_node;
    static GemICacheSlot _ic_645 = {0};
    if (gem_truthy(gem_neq(gem_table_get_cached(_t4576, "selective", &_ic_645), GEM_NIL))) {
#line 265 "compiler/main.gem"
    GemVal _t4577 = gem_v_load_node;
    static GemICacheSlot _ic_646 = {0};
        GemVal gem_v__for_items_17 = gem_table_get_cached(_t4577, "selective", &_ic_646);
#line 265 "compiler/main.gem"
        GemVal gem_v__for_i_17 = gem_int(0);
#line 265 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4578[] = {gem_v__for_items_17};
            if (!gem_truthy(gem_lt(gem_v__for_i_17, gem_len_fn(NULL, _t4578, 1)))) break;
#line 265 "compiler/main.gem"
            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_17, gem_v__for_i_17);
#line 265 "compiler/main.gem"
            gem_v__for_i_17 = gem_add(gem_v__for_i_17, gem_int(1));
#line 266 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_sel_name);
#line 267 "compiler/main.gem"
    GemVal _t4579[] = {gem_v_prefixed};
    GemVal _t4580[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4579, 1), gem_int(0)};
    GemVal _t4581[] = {gem_v_result, gem_fn_make_let(NULL, _t4580, 3)};
            (void)(gem_push_fn(NULL, _t4581, 2));
        }

    } else {
#line 270 "compiler/main.gem"
        GemVal gem_v_binding_name = gem_v_mod_name;
#line 271 "compiler/main.gem"
    GemVal _t4582 = gem_v_load_node;
    static GemICacheSlot _ic_647 = {0};
        if (gem_truthy(gem_neq(gem_table_get_cached(_t4582, "alias", &_ic_647), GEM_NIL))) {
#line 272 "compiler/main.gem"
    GemVal _t4583 = gem_v_load_node;
    static GemICacheSlot _ic_648 = {0};
            gem_v_binding_name = gem_table_get_cached(_t4583, "alias", &_ic_648);
        }
#line 274 "compiler/main.gem"
    GemVal _t4584 = gem_table_new();
        GemVal gem_v_entries = _t4584;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_items_18 = gem_v_exported;
#line 275 "compiler/main.gem"
        GemVal gem_v__for_i_18 = gem_int(0);
#line 275 "compiler/main.gem"
        while (1) {
            gem_yield_check();
            GemVal _t4585[] = {gem_v__for_items_18};
            if (!gem_truthy(gem_lt(gem_v__for_i_18, gem_len_fn(NULL, _t4585, 1)))) break;
#line 275 "compiler/main.gem"
            GemVal gem_v_ename = gem_table_get(gem_v__for_items_18, gem_v__for_i_18);
#line 275 "compiler/main.gem"
            gem_v__for_i_18 = gem_add(gem_v__for_i_18, gem_int(1));
#line 276 "compiler/main.gem"
            GemVal gem_v_prefixed = gem_add(gem_v_prefix, gem_v_ename);
#line 277 "compiler/main.gem"
    GemVal _t4586[] = {gem_v_prefixed};
    GemVal _t4587[] = {gem_v_ename, gem_fn_make_var(NULL, _t4586, 1)};
    GemVal _t4588[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4587, 2)};
            (void)(gem_push_fn(NULL, _t4588, 2));
        }

#line 279 "compiler/main.gem"
    GemVal _t4589[] = {gem_v_entries};
    GemVal _t4590[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4589, 1), gem_int(0)};
    GemVal _t4591[] = {gem_v_result, gem_fn_make_let(NULL, _t4590, 3)};
        (void)(gem_push_fn(NULL, _t4591, 2));
    }
    GemVal _t4592 = gem_v_result;
    gem_pop_frame();
    return _t4592;
}

static GemVal gem_fn_resolve_loads(void *_env, GemVal *args, int argc) {
#line 287 "compiler/main.gem"
    GemVal gem_v_ast = (argc > 0) ? args[0] : GEM_NIL;
    GemVal gem_v_base_dir = (argc > 1) ? args[1] : GEM_NIL;
    GemVal gem_v_loaded = (argc > 2) ? args[2] : GEM_NIL;
    gem_push_frame("resolve_loads", "compiler/main.gem", 287);
#line 288 "compiler/main.gem"
    GemVal _t4593 = gem_table_new();
    GemVal gem_v_new_stmts = _t4593;
#line 289 "compiler/main.gem"
    GemVal gem_v_count = gem_int(0);
#line 290 "compiler/main.gem"
    GemVal _t4594 = gem_v_ast;
    static GemICacheSlot _ic_649 = {0};
    GemVal gem_v__for_items_19 = gem_table_get_cached(_t4594, "stmts", &_ic_649);
#line 290 "compiler/main.gem"
    GemVal gem_v__for_i_19 = gem_int(0);
#line 290 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        GemVal _t4595[] = {gem_v__for_items_19};
        if (!gem_truthy(gem_lt(gem_v__for_i_19, gem_len_fn(NULL, _t4595, 1)))) break;
#line 290 "compiler/main.gem"
        GemVal gem_v_stmt = gem_table_get(gem_v__for_items_19, gem_v__for_i_19);
#line 290 "compiler/main.gem"
        gem_v__for_i_19 = gem_add(gem_v__for_i_19, gem_int(1));
#line 291 "compiler/main.gem"
    GemVal _t4596[] = {gem_v_stmt, gem_string("load")};
        if (gem_truthy(gem_fn_is_node(NULL, _t4596, 2))) {
#line 292 "compiler/main.gem"
    GemVal _t4597 = gem_v_stmt;
    static GemICacheSlot _ic_650 = {0};
            GemVal gem_v_path = gem_table_get_cached(_t4597, "path", &_ic_650);
#line 293 "compiler/main.gem"
    GemVal _t4598 = gem_v_string;
    static GemICacheSlot _ic_651 = {0};
    GemVal _t4599[] = {gem_v_path, gem_string(".gem")};
    GemVal _t4600 = gem_table_get_cached(_t4598, "ends_with", &_ic_651);
            if (gem_truthy(gem_not(_t4600.fn(_t4600.env, _t4599, 2)))) {
#line 294 "compiler/main.gem"
                gem_v_path = gem_add(gem_v_path, gem_string(".gem"));
            }
#line 297 "compiler/main.gem"
    GemVal _t4601[] = {gem_v_base_dir, gem_v_path};
            GemVal gem_v_full_path = gem_path_join_fn(NULL, _t4601, 2);
#line 298 "compiler/main.gem"
    GemVal _t4602[] = {gem_v_full_path};
            if (gem_truthy(gem_not(gem_file_exists_fn(NULL, _t4602, 1)))) {
#line 299 "compiler/main.gem"
    GemVal _t4603[] = {gem_v_install_root, gem_v_path};
                gem_v_full_path = gem_path_join_fn(NULL, _t4603, 2);
            }
#line 302 "compiler/main.gem"
    GemVal _t4604[] = {gem_v_full_path};
            gem_v_full_path = gem_normalize_path_fn(NULL, _t4604, 1);
#line 304 "compiler/main.gem"
            GemVal gem_v_cached = gem_table_get(gem_v_loaded, gem_v_full_path);
#line 305 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_cached, GEM_NIL))) {
#line 306 "compiler/main.gem"
    GemVal _t4605 = gem_v_cached;
    static GemICacheSlot _ic_652 = {0};
                if (gem_truthy(gem_neq(gem_table_get_cached(_t4605, "exported", &_ic_652), GEM_NIL))) {
#line 307 "compiler/main.gem"
    GemVal _t4606 = gem_v_cached;
    static GemICacheSlot _ic_653 = {0};
                    GemVal gem_v_prefix = gem_add(gem_add(gem_string("_mod_"), gem_table_get_cached(_t4606, "mod_name", &_ic_653)), gem_string("_"));
#line 308 "compiler/main.gem"
    GemVal _t4607 = gem_v_stmt;
    static GemICacheSlot _ic_654 = {0};
                    if (gem_truthy(gem_neq(gem_table_get_cached(_t4607, "selective", &_ic_654), GEM_NIL))) {
#line 309 "compiler/main.gem"
    GemVal _t4608 = gem_v_stmt;
    static GemICacheSlot _ic_655 = {0};
                        GemVal gem_v__for_items_20 = gem_table_get_cached(_t4608, "selective", &_ic_655);
#line 309 "compiler/main.gem"
                        GemVal gem_v__for_i_20 = gem_int(0);
#line 309 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4609[] = {gem_v__for_items_20};
                            if (!gem_truthy(gem_lt(gem_v__for_i_20, gem_len_fn(NULL, _t4609, 1)))) break;
#line 309 "compiler/main.gem"
                            GemVal gem_v_sel_name = gem_table_get(gem_v__for_items_20, gem_v__for_i_20);
#line 309 "compiler/main.gem"
                            gem_v__for_i_20 = gem_add(gem_v__for_i_20, gem_int(1));
#line 310 "compiler/main.gem"
    GemVal _t4610[] = {gem_add(gem_v_prefix, gem_v_sel_name)};
    GemVal _t4611[] = {gem_v_sel_name, gem_fn_make_var(NULL, _t4610, 1), gem_int(0)};
                            gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4611, 3));
#line 311 "compiler/main.gem"
                            gem_v_count = gem_add(gem_v_count, gem_int(1));
                        }

                    } else {
#line 314 "compiler/main.gem"
    GemVal _t4612 = gem_v_cached;
    static GemICacheSlot _ic_656 = {0};
                        GemVal gem_v_binding_name = gem_table_get_cached(_t4612, "mod_name", &_ic_656);
#line 315 "compiler/main.gem"
    GemVal _t4613 = gem_v_stmt;
    static GemICacheSlot _ic_657 = {0};
                        if (gem_truthy(gem_neq(gem_table_get_cached(_t4613, "alias", &_ic_657), GEM_NIL))) {
#line 316 "compiler/main.gem"
    GemVal _t4614 = gem_v_stmt;
    static GemICacheSlot _ic_658 = {0};
                            gem_v_binding_name = gem_table_get_cached(_t4614, "alias", &_ic_658);
                        }
#line 318 "compiler/main.gem"
    GemVal _t4615 = gem_table_new();
                        GemVal gem_v_entries = _t4615;
#line 319 "compiler/main.gem"
    GemVal _t4616 = gem_v_cached;
    static GemICacheSlot _ic_659 = {0};
                        GemVal gem_v__for_items_21 = gem_table_get_cached(_t4616, "exported", &_ic_659);
#line 319 "compiler/main.gem"
                        GemVal gem_v__for_i_21 = gem_int(0);
#line 319 "compiler/main.gem"
                        while (1) {
                            gem_yield_check();
                            GemVal _t4617[] = {gem_v__for_items_21};
                            if (!gem_truthy(gem_lt(gem_v__for_i_21, gem_len_fn(NULL, _t4617, 1)))) break;
#line 319 "compiler/main.gem"
                            GemVal gem_v_ename = gem_table_get(gem_v__for_items_21, gem_v__for_i_21);
#line 319 "compiler/main.gem"
                            gem_v__for_i_21 = gem_add(gem_v__for_i_21, gem_int(1));
#line 320 "compiler/main.gem"
    GemVal _t4618[] = {gem_add(gem_v_prefix, gem_v_ename)};
    GemVal _t4619[] = {gem_v_ename, gem_fn_make_var(NULL, _t4618, 1)};
    GemVal _t4620[] = {gem_v_entries, gem_fn_make_table_entry(NULL, _t4619, 2)};
                            (void)(gem_push_fn(NULL, _t4620, 2));
                        }

#line 322 "compiler/main.gem"
    GemVal _t4621[] = {gem_v_entries};
    GemVal _t4622[] = {gem_v_binding_name, gem_fn_make_table(NULL, _t4621, 1), gem_int(0)};
                        gem_table_set(gem_v_new_stmts, gem_v_count, gem_fn_make_let(NULL, _t4622, 3));
#line 323 "compiler/main.gem"
                        gem_v_count = gem_add(gem_v_count, gem_int(1));
                    }
                }
                continue;
            }
#line 329 "compiler/main.gem"
    GemVal _t4623 = gem_table_new();
    gem_table_set(_t4623, gem_string("exported"), GEM_NIL);
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4623);
#line 331 "compiler/main.gem"
    GemVal _t4624[] = {gem_v_full_path};
            GemVal gem_v_source = gem_read_file_fn(NULL, _t4624, 1);
#line 332 "compiler/main.gem"
    GemVal _t4625[] = {gem_v_source, gem_v_full_path};
    GemVal _t4626 = gem_v_parse_source;
            GemVal gem_v_loaded_ast = _t4626.fn(_t4626.env, _t4625, 2);
#line 333 "compiler/main.gem"
    GemVal _t4627[] = {gem_v_full_path};
            GemVal gem_v_loaded_dir = gem_dirname_fn(NULL, _t4627, 1);
#line 334 "compiler/main.gem"
    GemVal _t4628[] = {gem_v_loaded_ast, gem_v_loaded_dir, gem_v_loaded};
            GemVal gem_v_resolved = gem_fn_resolve_loads(NULL, _t4628, 3);
#line 336 "compiler/main.gem"
    GemVal _t4629 = gem_v_resolved;
    static GemICacheSlot _ic_660 = {0};
    GemVal _t4630[] = {gem_table_get_cached(_t4629, "stmts", &_ic_660)};
            GemVal gem_v_export_node = gem_fn_find_export_node(NULL, _t4630, 1);
#line 337 "compiler/main.gem"
            if (gem_truthy(gem_eq(gem_v_export_node, GEM_NIL))) {
#line 338 "compiler/main.gem"
    GemVal _t4631[] = {gem_add(gem_string("gem: loaded file has no export statement: "), gem_v_full_path)};
                (void)(gem_error_at_fn("compiler/main.gem", 338, _t4631, 1));
            }
#line 341 "compiler/main.gem"
    GemVal _t4632 = gem_v_stmt;
    static GemICacheSlot _ic_661 = {0};
    GemVal _t4633[] = {gem_table_get_cached(_t4632, "path", &_ic_661)};
            GemVal gem_v_mod_name = gem_fn_module_name_from_path(NULL, _t4633, 1);
#line 342 "compiler/main.gem"
    GemVal _t4634 = gem_table_new();
    gem_table_set(_t4634, gem_string("mod_name"), gem_v_mod_name);
    GemVal _t4635 = gem_v_export_node;
    static GemICacheSlot _ic_662 = {0};
    gem_table_set(_t4634, gem_string("exported"), gem_table_get_cached(_t4635, "names", &_ic_662));
            gem_table_set(gem_v_loaded, gem_v_full_path, _t4634);
#line 343 "compiler/main.gem"
    GemVal _t4636 = gem_v_resolved;
    static GemICacheSlot _ic_663 = {0};
    GemVal _t4637[] = {gem_table_get_cached(_t4636, "stmts", &_ic_663), gem_v_mod_name, gem_v_export_node, gem_v_stmt};
            GemVal gem_v_mod_stmts = gem_fn_transform_module(NULL, _t4637, 4);
#line 344 "compiler/main.gem"
            GemVal gem_v__for_items_22 = gem_v_mod_stmts;
#line 344 "compiler/main.gem"
            GemVal gem_v__for_i_22 = gem_int(0);
#line 344 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4638[] = {gem_v__for_items_22};
                if (!gem_truthy(gem_lt(gem_v__for_i_22, gem_len_fn(NULL, _t4638, 1)))) break;
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
    GemVal _t4639[] = {gem_v_new_stmts};
    GemVal _t4640 = gem_fn_make_program(NULL, _t4639, 1);
    gem_pop_frame();
    return _t4640;
}

static GemVal gem_fn_parse_args(void *_env, GemVal *args, int argc) {
#line 358 "compiler/main.gem"
    GemVal gem_v_args = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("parse_args", "compiler/main.gem", 358);
#line 359 "compiler/main.gem"
    GemVal _t4641 = gem_table_new();
    gem_table_set(_t4641, gem_string("src_path"), GEM_NIL);
    gem_table_set(_t4641, gem_string("out_name"), GEM_NIL);
    gem_table_set(_t4641, gem_string("emit_c"), gem_bool(0));
    gem_table_set(_t4641, gem_string("run"), gem_bool(0));
    GemVal _t4642 = gem_table_new();
    gem_table_set(_t4641, gem_string("run_args"), _t4642);
    GemVal gem_v_result = _t4641;
#line 367 "compiler/main.gem"
    GemVal _t4643[] = {gem_v_args};
    GemVal gem_v_argc = gem_len_fn(NULL, _t4643, 1);
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
    GemVal _t4644 = gem_v_result;
    static GemICacheSlot _ic_664 = {0};
            gem_table_set(gem_table_get_cached(_t4644, "run_args", &_ic_664), gem_v_run_arg_count, gem_v_arg);
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
    GemVal _t4645[] = {gem_string("gem: -o requires an argument")};
                            (void)(gem_error_at_fn("compiler/main.gem", 385, _t4645, 1));
                        }
#line 387 "compiler/main.gem"
                        gem_table_set(gem_v_result, gem_string("out_name"), gem_table_get(gem_v_args, gem_v_i));
                    } else {
#line 388 "compiler/main.gem"
    GemVal _t4646 = gem_v_result;
    static GemICacheSlot _ic_665 = {0};
                        if (gem_truthy(gem_eq(gem_table_get_cached(_t4646, "src_path", &_ic_665), GEM_NIL))) {
#line 389 "compiler/main.gem"
                            gem_table_set(gem_v_result, gem_string("src_path"), gem_v_arg);
                        } else {
#line 392 "compiler/main.gem"
    GemVal _t4647 = gem_v_result;
    static GemICacheSlot _ic_666 = {0};
                            gem_table_set(gem_table_get_cached(_t4647, "run_args", &_ic_666), gem_v_run_arg_count, gem_v_arg);
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
    GemVal _t4648 = gem_v_result;
    gem_pop_frame();
    return _t4648;
}

static GemVal gem_fn_basename_no_ext(void *_env, GemVal *args, int argc) {
#line 403 "compiler/main.gem"
    GemVal gem_v_path = (argc > 0) ? args[0] : GEM_NIL;
    gem_push_frame("basename_no_ext", "compiler/main.gem", 403);
#line 405 "compiler/main.gem"
    GemVal _t4649[] = {gem_v_path};
    GemVal gem_v_plen = gem_len_fn(NULL, _t4649, 1);
#line 406 "compiler/main.gem"
    GemVal gem_v_last_slash = gem_neg(gem_int(1));
#line 407 "compiler/main.gem"
    GemVal gem_v_i = gem_int(0);
#line 408 "compiler/main.gem"
    while (1) {
        gem_yield_check();
        if (!gem_truthy(gem_lt(gem_v_i, gem_v_plen))) break;
#line 409 "compiler/main.gem"
    GemVal _t4650[] = {gem_v_path, gem_v_i};
        if (gem_truthy(gem_eq(gem_ord_fn(NULL, _t4650, 2), gem_int(47)))) {
#line 410 "compiler/main.gem"
            gem_v_last_slash = gem_v_i;
        }
#line 412 "compiler/main.gem"
        gem_v_i = gem_add(gem_v_i, gem_int(1));
    }
#line 414 "compiler/main.gem"
    GemVal _t4651[] = {gem_v_path, gem_add(gem_v_last_slash, gem_int(1)), gem_sub(gem_sub(gem_v_plen, gem_v_last_slash), gem_int(1))};
    GemVal gem_v_name = gem_substr_fn(NULL, _t4651, 3);
#line 416 "compiler/main.gem"
    GemVal _t4652[] = {gem_v_name};
    GemVal gem_v_nlen = gem_len_fn(NULL, _t4652, 1);
#line 417 "compiler/main.gem"
    if (gem_truthy(gem_gt(gem_v_nlen, gem_int(4)))) {
#line 418 "compiler/main.gem"
    GemVal _t4653[] = {gem_v_name, gem_sub(gem_v_nlen, gem_int(4)), gem_int(4)};
        GemVal gem_v_maybe_ext = gem_substr_fn(NULL, _t4653, 3);
#line 419 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_maybe_ext, gem_string(".gem")))) {
#line 420 "compiler/main.gem"
    GemVal _t4654[] = {gem_v_name, gem_int(0), gem_sub(gem_v_nlen, gem_int(4))};
            gem_v_name = gem_substr_fn(NULL, _t4654, 3);
        }
    }
    GemVal _t4655 = gem_v_name;
    gem_pop_frame();
    return _t4655;
}

static GemVal gem_user_main(void *_env, GemVal *_args, int _argc) {
    (void)_env; (void)_args; (void)_argc;
    gem_push_frame("main", "compiler/main.gem", 0);
#line 0 "compiler/main.gem"
    GemVal _t4656 = gem_table_new();
    gem_table_set(_t4656, gem_string("split"), gem_make_fn(gem_fn__mod_string_split, NULL));
    gem_table_set(_t4656, gem_string("index_of"), gem_make_fn(gem_fn__mod_string_index_of, NULL));
    gem_table_set(_t4656, gem_string("join"), gem_make_fn(gem_fn__mod_string_join, NULL));
    gem_table_set(_t4656, gem_string("trim"), gem_make_fn(gem_fn__mod_string_trim, NULL));
    gem_table_set(_t4656, gem_string("starts_with"), gem_make_fn(gem_fn__mod_string_starts_with, NULL));
    gem_table_set(_t4656, gem_string("ends_with"), gem_make_fn(gem_fn__mod_string_ends_with, NULL));
    gem_table_set(_t4656, gem_string("upper"), gem_make_fn(gem_fn__mod_string_upper, NULL));
    gem_table_set(_t4656, gem_string("lower"), gem_make_fn(gem_fn__mod_string_lower, NULL));
    gem_table_set(_t4656, gem_string("contains"), gem_make_fn(gem_fn__mod_string_contains, NULL));
    gem_table_set(_t4656, gem_string("repeat"), gem_make_fn(gem_fn__mod_string_repeat, NULL));
    gem_v_string = _t4656;
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
    GemVal _t4657[] = {gem_table_get(gem_argv_fn(NULL, NULL, 0), gem_int(0))};
    GemVal _t4658[] = {gem_dirname_fn(NULL, _t4657, 1)};
    gem_v_install_root = gem_dirname_fn(NULL, _t4658, 1);
#line 428 "compiler/main.gem"
    GemVal _t4659[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_argc = gem_len_fn(NULL, _t4659, 1);
#line 429 "compiler/main.gem"
    if (gem_truthy(gem_lt(gem_v_argc, gem_int(2)))) {
#line 430 "compiler/main.gem"
    GemVal _t4660[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 430, _t4660, 1));
    }
#line 433 "compiler/main.gem"
    GemVal _t4661[] = {gem_argv_fn(NULL, NULL, 0)};
    gem_v_cli = gem_fn_parse_args(NULL, _t4661, 1);
#line 435 "compiler/main.gem"
    GemVal _t4662 = gem_v_cli;
    static GemICacheSlot _ic_667 = {0};
    if (gem_truthy(gem_eq(gem_table_get_cached(_t4662, "src_path", &_ic_667), GEM_NIL))) {
#line 436 "compiler/main.gem"
    GemVal _t4663[] = {gem_string("usage: gem <file.gem> [--emit-c | --run | -o <name>]")};
        (void)(gem_error_at_fn("compiler/main.gem", 436, _t4663, 1));
    }
#line 439 "compiler/main.gem"
    GemVal _t4664 = gem_v_cli;
    static GemICacheSlot _ic_668 = {0};
    gem_v_src_path = gem_table_get_cached(_t4664, "src_path", &_ic_668);
#line 440 "compiler/main.gem"
    GemVal _t4665[] = {gem_v_src_path};
    gem_v_source = gem_read_file_fn(NULL, _t4665, 1);
#line 442 "compiler/main.gem"
    GemVal _t4666[] = {gem_v_source, gem_v_src_path};
    GemVal _t4667 = gem_v_parse_source;
    gem_v_ast = _t4667.fn(_t4667.env, _t4666, 2);
#line 443 "compiler/main.gem"
    GemVal _t4668[] = {gem_v_src_path};
    gem_v_base_dir = gem_dirname_fn(NULL, _t4668, 1);
#line 444 "compiler/main.gem"
    GemVal _t4669 = gem_table_new();
    gem_v_loaded = _t4669;
#line 445 "compiler/main.gem"
    GemVal _t4670[] = {gem_v_ast, gem_v_base_dir, gem_v_loaded};
    gem_v_resolved_ast = gem_fn_resolve_loads(NULL, _t4670, 3);
#line 447 "compiler/main.gem"
    GemVal _t4671[] = {gem_v_src_path};
    GemVal _t4672 = gem_v_make_codegen;
    gem_v_cg = _t4672.fn(_t4672.env, _t4671, 1);
#line 448 "compiler/main.gem"
    GemVal _t4673 = gem_v_cg;
    static GemICacheSlot _ic_669 = {0};
    GemVal _t4674[] = {gem_v_resolved_ast};
    GemVal _t4675 = gem_table_get_cached(_t4673, "compile", &_ic_669);
    gem_v_c_code = _t4675.fn(_t4675.env, _t4674, 1);
#line 452 "compiler/main.gem"
    GemVal _t4676 = gem_v_cli;
    static GemICacheSlot _ic_670 = {0};
    if (gem_truthy(gem_table_get_cached(_t4676, "emit_c", &_ic_670))) {
#line 453 "compiler/main.gem"
    GemVal _t4677[] = {gem_v_c_code};
        (void)(gem_print(NULL, _t4677, 1));
    } else {
#line 457 "compiler/main.gem"
    GemVal _t4678[] = {gem_v_src_path};
        GemVal gem_v_base = gem_fn_basename_no_ext(NULL, _t4678, 1);
#line 458 "compiler/main.gem"
        GemVal gem_v_tmp_c = gem_add(gem_add(gem_string("/tmp/gem_"), gem_v_base), gem_string(".c"));
#line 461 "compiler/main.gem"
    GemVal _t4679 = gem_v_cli;
    static GemICacheSlot _ic_671 = {0};
        GemVal gem_v_out_name = gem_table_get_cached(_t4679, "out_name", &_ic_671);
#line 462 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_out_name, GEM_NIL))) {
#line 463 "compiler/main.gem"
            gem_v_out_name = gem_add(gem_string("./"), gem_v_base);
        }
#line 467 "compiler/main.gem"
    GemVal _t4680[] = {gem_v_tmp_c, gem_v_c_code};
        (void)(gem_write_file_fn(NULL, _t4680, 2));
#line 470 "compiler/main.gem"
    GemVal _t4681[] = {gem_v_install_root, gem_string("runtime")};
        GemVal gem_v_runtime_dir = gem_path_join_fn(NULL, _t4681, 2);
#line 473 "compiler/main.gem"
        GemVal gem_v_gc_flags = gem_string("-lgc");
#line 474 "compiler/main.gem"
    GemVal _t4682[] = {gem_string("pkg-config --cflags --libs bdw-gc > /tmp/gem_gc_flags.txt 2>/dev/null")};
        GemVal gem_v_pkg_ret = gem_exec_fn(NULL, _t4682, 1);
#line 475 "compiler/main.gem"
        if (gem_truthy(gem_eq(gem_v_pkg_ret, gem_int(0)))) {
#line 476 "compiler/main.gem"
    GemVal _t4683[] = {gem_string("/tmp/gem_gc_flags.txt")};
            gem_v_gc_flags = gem_read_file_fn(NULL, _t4683, 1);
#line 477 "compiler/main.gem"
    GemVal _t4684[] = {gem_v_gc_flags, gem_string("\n"), gem_string("")};
            gem_v_gc_flags = gem_str_replace_fn(NULL, _t4684, 3);
        }
#line 480 "compiler/main.gem"
    GemVal _t4685[] = {gem_v_install_root, gem_string("build/libgem_runtime.a")};
        GemVal gem_v_runtime_lib = gem_path_join_fn(NULL, _t4685, 2);
#line 481 "compiler/main.gem"
        GemVal gem_v_cc_cmd = gem_string("");
#line 482 "compiler/main.gem"
    GemVal _t4686[] = {gem_v_runtime_lib};
        if (gem_truthy(gem_file_exists_fn(NULL, _t4686, 1))) {
#line 483 "compiler/main.gem"
            gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread ")), gem_v_runtime_lib);
        } else {
#line 485 "compiler/main.gem"
            gem_v_cc_cmd = gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_add(gem_string("cc -o "), gem_v_out_name), gem_string(" ")), gem_v_tmp_c), gem_string(" ")), gem_v_runtime_dir), gem_string("/gem_*.c ")), gem_v_runtime_dir), gem_string("/sqlite3.c -I ")), gem_v_runtime_dir), gem_string(" -std=c11 -O2 ")), gem_v_gc_flags), gem_string(" -lm -pthread"));
        }
#line 488 "compiler/main.gem"
    GemVal _t4687[] = {gem_v_cc_cmd};
        GemVal gem_v_cc_ret = gem_exec_fn(NULL, _t4687, 1);
#line 489 "compiler/main.gem"
        if (gem_truthy(gem_neq(gem_v_cc_ret, gem_int(0)))) {
#line 490 "compiler/main.gem"
    GemVal _t4688[] = {gem_v_cc_ret};
    GemVal _t4689[] = {gem_add(gem_add(gem_string("gem: compilation failed (cc exited with "), gem_to_string_fn(NULL, _t4688, 1)), gem_string(")"))};
            (void)(gem_error_at_fn("compiler/main.gem", 490, _t4689, 1));
        }
#line 495 "compiler/main.gem"
    GemVal _t4690 = gem_v_cli;
    static GemICacheSlot _ic_672 = {0};
        if (gem_truthy(gem_table_get_cached(_t4690, "run", &_ic_672))) {
#line 496 "compiler/main.gem"
            GemVal gem_v_run_cmd = gem_v_out_name;
#line 497 "compiler/main.gem"
            GemVal gem_v_i = gem_int(0);
#line 498 "compiler/main.gem"
            while (1) {
                gem_yield_check();
                GemVal _t4691 = gem_v_cli;
                static GemICacheSlot _ic_673 = {0};
                GemVal _t4692[] = {gem_table_get_cached(_t4691, "run_args", &_ic_673)};
                if (!gem_truthy(gem_lt(gem_v_i, gem_len_fn(NULL, _t4692, 1)))) break;
#line 499 "compiler/main.gem"
    GemVal _t4693 = gem_v_cli;
    static GemICacheSlot _ic_674 = {0};
                gem_v_run_cmd = gem_add(gem_add(gem_v_run_cmd, gem_string(" ")), gem_table_get(gem_table_get_cached(_t4693, "run_args", &_ic_674), gem_v_i));
#line 500 "compiler/main.gem"
                gem_v_i = gem_add(gem_v_i, gem_int(1));
            }
#line 502 "compiler/main.gem"
    GemVal _t4694[] = {gem_v_run_cmd};
            GemVal gem_v_run_ret = gem_exec_fn(NULL, _t4694, 1);
#line 503 "compiler/main.gem"
            if (gem_truthy(gem_neq(gem_v_run_ret, gem_int(0)))) {
#line 504 "compiler/main.gem"
    GemVal _t4695[] = {gem_v_run_ret};
                (void)(gem_exit_process_fn(NULL, _t4695, 1));
            }
        }
    }
    gem_pop_frame();
    return GEM_NIL;
}

int main(int argc, char **argv) {
    gem_init(argc, argv);
    gem_scheduler_init();
    gem_run_main(gem_user_main, NULL);
    return 0;
}

