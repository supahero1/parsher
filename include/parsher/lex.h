#ifndef _parsher_lex_h_
#define _parsher_lex_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <parsher/state.h>
#include <parsher/words.h>


enum psh_token_type
{
	psh_token_none,
	psh_token_whitespace,
	psh_token_comment,
	psh_token_regexp,
	psh_token_string,
	psh_token_template,
	psh_token_template_end,
	psh_token_number,
	psh_token_word,
	psh_token_symbol
};


struct psh_token
{
	const uint8_t* token;
	uint32_t len;

	enum psh_token_type type;

	union
	{
		int subtype;

		int newline;
		enum psh_keyword keyword;
		enum psh_symbol symbol;
	};
};


struct psh_lexer
{
	struct psh_memory tokens;
};


extern void
psh_lexer_init(struct psh_lexer* lexer);


extern void
psh_lexer_free(struct psh_lexer* lexer);


extern enum psh_status
psh_lex(struct psh_state* state, struct psh_lexer* out);


#ifdef __cplusplus
}
#endif

#endif
