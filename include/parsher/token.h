#ifndef _parsher_async_h_
#define _parsher_async_h_ 1

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>

#include <parsher/words.h>
#include <parsher/source.h>


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
	uint64_t len;

	enum psh_token_type type;
	enum psh_word word;
};


struct psh_tokens
{
	struct psh_token* tokens;

	uint64_t used;
	uint64_t size;
};


extern enum psh_status
psh_tokenize(const struct psh_source* const, struct psh_tokens* const);


#ifdef __cplusplus
}
#endif

#endif
