#include <parsher/rates.h>
#include <parsher/token.h>
#include <parsher/except.h>

#define CASE_INSENSITIVE(code) ((code) & ~32)


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


void
psh_tokens_init(struct psh_tokens* tokens)
{
	sem_init(&tokens->producer);
	psh_hash_init(&tokens->regexp_word_hashes, psh_regexp_word_dict,
		sizeof(psh_regexp_word_dict) / sizeof(psh_regexp_word_dict[0]));
}


void
psh_tokens_reset(struct psh_tokens* tokens)
{
	tokens->used = 0;
}


void
psh_tokens_free(struct psh_tokens* tokens)
{
	free(tokens->tokens);
	psh_hash_free(&tokens->regexp_word_hashes);
	sem_destroy(&tokens->producer);

	tokens->tokens = NULL;
	tokens->used = 0;
	tokens->size = 0;
}


static void
psh_alloc_token(struct psh_tokens* out)
{
	if(out->size == 0)
	{
		out->size = 16;
	}

	out->size <<= 1;

	out->tokens = realloc(out->tokens, sizeof(*out->tokens) * out->size);

	if(out->tokens != NULL)
	{
		return;
	}

	out->size >>= 1;
	out->size += 1;

	out->tokens = realloc(out->tokens, sizeof(*out->tokens) * out->size);

	if(out->tokens != NULL)
	{
		return;
	}

	psh_nomem();
}


static struct psh_token token_none = { .type = psh_token_none };


static struct psh_token*
psh_add_token_(struct psh_tokens* out,
	struct psh_token* last_token, const struct psh_token token)
{
	struct psh_token* ret = last_token;

	if(out->used >= out->size)
	{
		void* old = out->tokens;

		psh_alloc_token(out);

		if(last_token != &token_none && out->tokens != old)
		{
			ret = (struct psh_token*)(
				+ (uintptr_t) last_token
				- (uintptr_t) old
				+ (uintptr_t) out->tokens
			);
		}
	}

	out->tokens[out->used++] = token;

	if(out->used % psh_rate_tokenizer == 0)
	{
		sem_post(&out->producer, psh_rate_tokenizer);
	}

	return ret;
}


enum psh_status
psh_tokenize(const struct psh_source* src, struct psh_tokens* out)
{
	const uint8_t* in = src->arr;
	const uint8_t* in_prev;
	const uint8_t* in_end = in + src->len;

	struct psh_token* psh_last_token = &token_none;

	uint64_t psh_nest = 0;
	uint64_t psh_template_nest = 0;

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
			goto goto_unexpected;	\
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

#define psh_add_token(_type)		\
	(psh_last_token =				\
		psh_add_token_(out,			\
		psh_last_token,				\
		(struct psh_token)			\
		{							\
			.token = in_prev,		\
			.len = psh_token_len,	\
			.type = _type,			\
			.regexp_word = -1		\
		}							\
		)							\
	)

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
	 * 7. psh_token_template(_end)
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
					int stop = (*in == '\n');

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
					psh_last_token->regexp_word != -1
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
							/* Escaped escape escapes no more. */

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

						goto goto_template_string;
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

		uint8_t char_code = *in - '0';

		if(char_code <= 9)
		{
			do
			{
				psh_next_byte();
			}
			while(!psh_eof() && (
				psh_lut_number_chars[*in] == 1 ||
				( /* Check if a plus/minus sign is allowed */
					CASE_INSENSITIVE(*(in - 1)) == 'E' &&
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
			uint8_t end = *in;

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
			goto_template_string:;

			int interrupt = 0;

			do
			{
				psh_next_byte();
				psh_atleast(0);

				/**
				 * This is not undefined behavior, because if
				 * the previous character is '$' and not '`',
				 * it means this is not the first iteration.
				 */

				interrupt =
					(*in == '{' && *(in - 1) == '$' && *(in - 2) != '\\');
			}
			while(
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
		psh_last_token->regexp_word = psh_hash_lookup(&out->regexp_word_hashes,
			(char*) psh_last_token->token, psh_last_token->len);
	}

#undef psh_add_token
#undef psh_extend_token
#undef psh_eof
#undef psh_token_len
#undef psh_next_byte
#undef psh_atleast
#undef psh_set_as_last_token

	sem_post(&out->producer, out->used % psh_rate_tokenizer);

	return psh_ok;


	goto_unexpected:

	return psh_unexpected_end;
}

#undef psh_add_token
