#ifndef _parsher_words_h_
#define _parsher_words_h_ 1

#ifdef __cplusplus
extern "C" {
#endif


enum psh_regexp_word
{
	psh_regexp_word_await,
	psh_regexp_word_case,
	psh_regexp_word_delete,
	psh_regexp_word_export,
	psh_regexp_word_in,
	psh_regexp_word_return,
	psh_regexp_word_throw,
	psh_regexp_word_typeof,
	psh_regexp_word_void,
	psh_regexp_word_yield,
	psh_regexp_word__count
};


extern char* psh_regexp_word_dict[psh_regexp_word__count];


enum psh_keyword
{
	psh_keyword_as,
	psh_keyword_async,
	psh_keyword_break,
	psh_keyword_catch,
	psh_keyword_class,
	psh_keyword_const,
	psh_keyword_continue,
	psh_keyword_debugger,
	psh_keyword_default,
	psh_keyword_else,
	psh_keyword_extends,
	psh_keyword_finally,
	psh_keyword_for,
	psh_keyword_from,
	psh_keyword_function,
	psh_keyword_get,
	psh_keyword_if,
	psh_keyword_import,
	psh_keyword_instanceof,
	psh_keyword_let,
	psh_keyword_new,
	psh_keyword_of,
	psh_keyword_set,
	psh_keyword_static,
	psh_keyword_switch,
	psh_keyword_try,
	psh_keyword_var,
	psh_keyword_while,
	psh_keyword_with,
	psh_keyword__count
};


extern char* psh_keyword_dict[psh_keyword__count];


#ifdef __cplusplus
}
#endif

#endif
