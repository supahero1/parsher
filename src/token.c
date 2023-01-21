#include <stdio.h>
#include <alloca.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <parsher/token.h>
#include <parsher/except.h>


static const uint8_t psh_lut_whitespace[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


static const uint8_t psh_lut_symbols[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


/* This does not include + and - on purpose. */

static const uint8_t psh_lut_number_chars[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


static const uint8_t psh_lut_word[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
	0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};



static void
psh_alloc_token(struct psh_tokens* const out)
{
	if(out->size == 0)
	{
		out->size = 16;
	}

	out->size <<= 1;

	out->tokens = realloc(out->tokens, sizeof(struct psh_token) * out->size);

	if(out->tokens != NULL)
	{
		return;
	}

	out->size >>= 1;
	out->size += 1;

	out->tokens = realloc(out->tokens, sizeof(struct psh_token) * out->size);

	if(out->tokens != NULL)
	{
		return;
	}

	psh_nomem();
}


static void
psh_hash_word(struct psh_token* const tok)
{
	if(tok->word != psh_word_unknown || tok->len > psh_max_word_len)
	{
		return;
	}

	char* const str = alloca(psh_max_word_len + 1);

	(void) memcpy(str, tok->token, tok->len);

	str[tok->len] = 0;

	const struct psh_word_lut* const word = psh_lookup_word(str, tok->len);

	if(word == NULL)
	{
		return;
	}

	tok->word = word->id;
}


static int
psh_is_regexp_keyword(const struct psh_token* const tok)
{
	return tok->word > psh_word_unknown &&
		tok->word < psh_word_preregexp_end;
}


enum psh_status
psh_tokenize(const struct psh_source* const src, struct psh_tokens* const out)
{
	const uint8_t* in = src->arr;
	const uint8_t* in_prev;
	const uint8_t* const in_end = in + src->len;

	struct psh_token psh_none_token = {0};
	struct psh_token* psh_last_token = &psh_none_token;

	uint64_t psh_nest = 0;
	uint64_t psh_template_nest = 0;


#define psh_add_token(_type)								\
	do														\
	{														\
		if(out->used >= out->size)							\
		{													\
			const uintptr_t old = (uintptr_t) out->tokens;	\
															\
			psh_alloc_token(out);							\
															\
			if(psh_last_token != &psh_none_token)			\
			{												\
				psh_last_token = (uintptr_t) out->tokens +	\
					(uintptr_t) psh_last_token - old;		\
			}												\
		}													\
															\
		out->tokens[out->used++] =							\
		(struct psh_token)									\
		{													\
			.token = in_prev,								\
			.len = (uintptr_t) in - (uintptr_t) in_prev,	\
			.type = _type,									\
			.word = psh_word_unknown						\
		};													\
	}														\
	while(0)

#define psh_set_as_last_token() (psh_last_token = out->tokens + out->used - 1)

	/**
	 * Make sure there's at least `num` bytes of input available.
	 *
	 * Call this with `0` to make sure a loop prior
	 * to this did not quit because of lack of input.
	 */
#define psh_atleast(num)			\
	do								\
	{								\
		if(in + num >= in_end)		\
		{							\
			goto err_unexpected;	\
		}							\
	}								\
	while(0)

#define psh_next_byte() (++in)

#define psh_token_len (in - in_prev)

#define psh_eof() (in == in_end)

#define psh_extend_token(_type)					\
	do											\
	{											\
		psh_last_token->type = _type;			\
		psh_last_token->len += psh_token_len;	\
	}											\
	while(0)


	/**
	 * Detection order:
	 *
	 * 1. psh_token_whitespace
	 *
	 * 2. psh_token_comment
	 *
	 * 3. psh_token_regexp
	 *
	 * 4. psh_token_symbol
	 *
	 * 5. psh_token_number
	 *
	 * 6. psh_token_string
	 *
	 * 7. psh_token_template(end)
	 *
	 * 8. psh_token_word
	 *
	 * It is only possible to place 5 before 4 or 3. Other than that,
	 * this list is immutable without considerable changes to the code.
	 */

	while(!psh_eof())
	{
		in_prev = in;

		/* Whitespace */

		while(psh_lut_whitespace[*in] == 1)
		{
			/* Drain until no whitespace. */

			psh_next_byte();

			if(psh_eof())
			{
				break;
			}
		}

		if(psh_token_len != 0)
		{
			psh_add_token(psh_token_whitespace);

			continue;
		}

		/* Comment, regexp, or division symbol */

		if(*in == '/')
		{
			/**
			 * There are a few options here:
			 *
			 * 1. Next char is a '/' - this is a single-line comment,
			 *
			 * 2. Next char is a '*' - this is a multi-line comment,
			 *
			 * 3. Previous token was none or a symbol or
			 *    a reserved keyword - this is a regexp,
			 *
			 * 4. This is a division symbol.
			 *
			 * Escaping the '/' is an error in every case, so it's not checked.
			 */

			psh_atleast(1);
			psh_next_byte();

			if(*in == '/')
			{
				while(!psh_eof())
				{
					const int stop = (*in == '\n');

					psh_next_byte();

					if(stop)
					{
						break;
					}
				}

				psh_add_token(psh_token_comment);

				continue;
			}

			if(*in == '*')
			{
				psh_atleast(2);
				psh_next_byte();

				do
				{
					psh_next_byte();
				}
				while(!psh_eof() && (*in != '/' || *(in - 1) != '*'));

				psh_atleast(0);
				psh_next_byte();
				psh_add_token(psh_token_comment);

				continue;
			}

			if(
				psh_last_token->type == psh_token_symbol ||
				(
					psh_last_token->type == psh_token_word &&
					psh_is_regexp_keyword(psh_last_token)
				) ||
				psh_last_token->type == psh_token_none
			)
			{
				uint8_t in_group = 0;
				uint8_t escaped = 0;

				do
				{
					/**
					 * Checking for a newline character is not the tokenizer's
					 * job. Overall, validating the input is not done here.
					 */

					if(*in == '\\')
					{
						if(escaped)
						{
							/* Escaped escape no more escapes. */

							escaped = 0;
						}
						else
						{
							escaped = 1;
						}
					}
					else if(escaped == 0)
					{
						if(in_group == 0)
						{
							if(*in == '[')
							{
								in_group = 1;
							}
							else if(*in == '/')
							{
								break;
							}
						}
						else if(*in == ']')
						{
							in_group = 0;
						}
					}
					else
					{
						escaped = 0;
					}

					psh_next_byte();
				}
				while(!psh_eof());

				psh_atleast(0);
				psh_next_byte();
				psh_add_token(psh_token_regexp);
				psh_set_as_last_token();

				continue;
			}

			psh_add_token(psh_token_symbol);
			psh_set_as_last_token();

			continue;
		}

		/* Symbol */

		if(psh_lut_symbols[*in] == 1)
		{
			if(psh_template_nest != 0)
			{
				if(*in == '}')
				{
					if(psh_nest == 0)
					{
						--psh_template_nest;

						goto template_string;
					}

					--psh_nest;
				}
				else if(*in == '{')
				{
					++psh_nest;
				}
			}

			psh_next_byte();
			psh_add_token(psh_token_symbol);
			psh_set_as_last_token();

			continue;
		}

		/* Number */

		const uint8_t char_code = *in - 48;

		if(char_code <= 9)
		{
			do
			{
				psh_next_byte();
			}
			while(!psh_eof() && (
				psh_lut_number_chars[*in] == 1 ||
				( /* Check if a plus/minus sign is allowed */
					(*(in - 1) == 'e' || *(in - 1) == 'E') &&
					(*in == '+' || *in == '-')
				)
			));

			if(
				psh_last_token->type == psh_token_symbol &&
				psh_last_token->token[0] == '.'
			)
			{
				/**
				 * Checking for a symbol occurs earlier than checking for
				 * a number, so if a dot ocurred, it wouldn't belong here.
				 */

				psh_extend_token(psh_token_number);
			}
			else
			{
				psh_add_token(psh_token_number);
			}

			psh_set_as_last_token();

			continue;
		}

		/* String */

		if(*in == '"' || *in == '\'')
		{
			const uint8_t end = *in;

			do
			{
				psh_next_byte();
			}
			while(!psh_eof() && (*in != end || *(in - 1) == '\\'));

			psh_atleast(0);
			psh_next_byte();
			psh_add_token(psh_token_string);
			psh_set_as_last_token();

			continue;
		}

		if(*in == '`')
		{
			template_string:

			int interrupt = 0;

			do
			{
				psh_next_byte();
				psh_atleast(1);

				/**
				 * This is not undefined behavior, because if
				 * the previous character is '$' and not '`',
				 * it means this is not the first iteration.
				 */

				interrupt =
					(*in == '{' && *(in - 1) == '$' && *(in - 2) != '\\');
			}
			while(
				!psh_eof() &&
				(*in != '`' || *(in - 1) == '\\') &&
				!interrupt
			);

			psh_atleast(0);
			psh_next_byte();

			if(interrupt)
			{
				++psh_template_nest;

				psh_add_token(psh_token_template);
			}
			else
			{
				psh_add_token(psh_token_template_end);
			}

			psh_set_as_last_token();

			continue;
		}

		/* Word */

		do
		{
			psh_next_byte();
		}
		while(!psh_eof() && psh_lut_word[*in] == 1);

		psh_add_token(psh_token_word);
		psh_set_as_last_token();
		psh_hash_word(psh_last_token);

		continue;
	}

#undef psh_extend_token
#undef psh_eof
#undef psh_token_len
#undef psh_next_byte
#undef psh_atleast
#undef psh_set_as_last_token
#undef psh_add_token

	return psh_ok;

	err_unexpected:

	return psh_unexpected_end;
}

#undef psh_add_token
