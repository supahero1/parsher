#ifndef _parsher_words_h_
#define _parsher_words_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <parsher/hash.h>

#define CREATE_HASH(name)								\
extern struct psh_hashes psh_##name##_hashes ;			\
extern char* psh_##name##_dict [ psh_##name##__count ];


enum psh_keyword
{
	psh_keyword_as,
	psh_keyword_async,
	psh_keyword_await,
	psh_keyword_break,
	psh_keyword_case,
	psh_keyword_catch,
	psh_keyword_class,
	psh_keyword_const,
	psh_keyword_continue,
	psh_keyword_debugger,
	psh_keyword_default,
	psh_keyword_delete,
	psh_keyword_do,
	psh_keyword_else,
	psh_keyword_export,
	psh_keyword_extends,
	psh_keyword_finally,
	psh_keyword_for,
	psh_keyword_from,
	psh_keyword_function,
	psh_keyword_get,
	psh_keyword_if,
	psh_keyword_import,
	psh_keyword_in,
	psh_keyword_instanceof,
	psh_keyword_let,
	psh_keyword_new,
	psh_keyword_of,
	psh_keyword_return,
	psh_keyword_set,
	psh_keyword_static,
	psh_keyword_switch,
	psh_keyword_throw,
	psh_keyword_try,
	psh_keyword_typeof,
	psh_keyword_var,
	psh_keyword_void,
	psh_keyword_while,
	psh_keyword_with,
	psh_keyword_yield,
	psh_keyword__count
};

CREATE_HASH(keyword)


enum psh_symbol
{
	psh_symbol_inc,
	psh_symbol_dec,
	psh_symbol_add,
	psh_symbol_sub,
	psh_symbol_mul,
	psh_symbol_div,
	psh_symbol_mod,
	psh_symbol_exp,
	psh_symbol_sar,
	psh_symbol_sal,
	psh_symbol_shr,

	psh_symbol_lt,
	psh_symbol_gt,
	psh_symbol_le,
	psh_symbol_ge,
	psh_symbol_eq,
	psh_symbol_seq,
	psh_symbol_neq,
	psh_symbol_sneq,

	psh_symbol_if,
	psh_symbol_colon,
	psh_symbol_comma,
	psh_symbol_dot,
	psh_symbol_semicolon,
	psh_symbol_curly_brace_open,
	psh_symbol_curly_brace_close,
	psh_symbol_parenthesis_open,
	psh_symbol_parenthesis_close,
	psh_symbol_bracket_open,
	psh_symbol_bracket_close,

	psh_symbol_set,
	psh_symbol_add_set,
	psh_symbol_sub_set,
	psh_symbol_mul_set,
	psh_symbol_div_set,
	psh_symbol_mod_set,
	psh_symbol_exp_set,
	psh_symbol_sar_set,
	psh_symbol_sal_set,
	psh_symbol_shr_set,

	psh_symbol_bitwise_not,
	psh_symbol_bitwise_and,
	psh_symbol_bitwise_or,
	psh_symbol_bitwise_xor,
	psh_symbol_bitwise_and_set,
	psh_symbol_bitwise_or_set,
	psh_symbol_bitwise_xor_set,

	psh_symbol_logical_not,
	psh_symbol_logical_bool,
	psh_symbol_logical_and,
	psh_symbol_logical_or,
	psh_symbol_logical_if_null,
	psh_symbol_logical_and_set,
	psh_symbol_logical_or_set,
	psh_symbol_logical_if_null_set,

	psh_symbol__count
};

CREATE_HASH(symbol)


#undef CREATE_HASH

#ifdef __cplusplus
}
#endif

#endif
