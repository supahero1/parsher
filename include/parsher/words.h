#ifndef _parsher_words_h_
#define _parsher_words_h_ 1

#ifdef __cplusplus
extern "C" {
#endif


#include <stddef.h>


/**
 * Place a comment instead of a tab before a word
 * to exclude it from the gperf hash generation.
 */

enum psh_word
{
/**/psh_word_unknown,

	/**
	 * This first "group" are tokens after which
	 * a slash symbol means beginning of a regexp.
	 */

	psh_word_await,
	psh_word_case,
	psh_word_delete,
	psh_word_do,
	psh_word_export,
	psh_word_in,
	psh_word_return,
	psh_word_throw,
	psh_word_typeof,
	psh_word_void,
	psh_word_yield,

/**/psh_word_preregexp_end,

	psh_word_Infinity,
	psh_word_NaN,
	psh_word_arguments,
	psh_word_as,
	psh_word_async,
	psh_word_break,
	psh_word_catch,
	psh_word_class,
	psh_word_const,
	psh_word_continue,
	psh_word_debugger,
	psh_word_default,
	psh_word_else,
	psh_word_eval,
	psh_word_extends,
	psh_word_false,
	psh_word_finally,
	psh_word_for,
	psh_word_from,
	psh_word_function,
	psh_word_get,
	psh_word_if,
	psh_word_import,
	psh_word_instanceof,
	psh_word_let,
	psh_word_new,
	psh_word_null,
	psh_word_of,
	psh_word_set,
	psh_word_static,
	psh_word_super,
	psh_word_switch,
	psh_word_this,
	psh_word_true,
	psh_word_try,
	psh_word_undefined,
	psh_word_var,
	psh_word_while,
	psh_word_with
};


struct psh_word_lut
{
	const char* str;

	const enum psh_word id;
};


extern const size_t psh_max_word_len;


extern const struct psh_word_lut*
psh_lookup_word(const char*, size_t);


#ifdef __cplusplus
}
#endif

#endif
