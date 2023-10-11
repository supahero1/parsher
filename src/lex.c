#include <parsher/lex.h>


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


static const uint8_t psh_lut_regexp[] =
{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0,
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
	0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
	0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
	0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1,
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
psh_lexer_init(struct psh_lexer* lexer)
{
	psh_memory_init(&lexer->tokens, sizeof(struct psh_token));
}


void
psh_lexer_free(struct psh_lexer* lexer)
{
	psh_memory_free(&lexer->tokens);
}


static struct psh_token token_none = { .type = psh_token_none };

#define CASE_INSENSITIVE(code) ((code) & ~32)

enum psh_status
psh_lex(struct psh_state* state, struct psh_lexer* lexer)
{
	const uint8_t* arr = state->input->src->arr;
	uint32_t len = state->input->src->len;

	const uint8_t* in = arr;
	const uint8_t* in_prev;
	const uint8_t* in_end = in + len;

#define get_location(pos) psh_input_query(state->input, pos - arr)

	struct psh_token* last_token = &token_none;
	struct psh_token* cur_token = &token_none;

	struct psh_memory nest_stack = {0};
	psh_memory_init(&nest_stack, sizeof(uint32_t));

	psh_memory_atleast(&lexer->tokens, len >> 4);

	enum psh_status status = psh_status_ok;

#define set_as_last_token() (last_token = cur_token)

	/**
	 * Make sure there's at least `num` bytes of input available.
	 *
	 * Call this with `0` to make sure a loop prior
	 * to this did not quit because of lack of input.
	 */
#define atleast(num)				\
	do								\
	{								\
		if(in + num >= in_end)		\
		{							\
			goto goto_unexpected;	\
		}							\
	}								\
	while(0)

#define next_byte() (++in)

#define token_len (in - in_prev)

#define eof (in == in_end)

#define extend_token(_type)				\
	do									\
	{									\
		last_token->type = _type;		\
		last_token->len += token_len;	\
	}									\
	while(0)

#define add_token_ext(_type, _subtype)	\
	do									\
	{									\
		*cur_token =					\
		(struct psh_token)				\
		{								\
			.token = in_prev,			\
			.len = token_len,			\
			.type = _type,				\
			_subtype					\
		};								\
										\
		++lexer->tokens.used;			\
	}									\
	while(0)

#define add_token(_type)				\
	add_token_ext(_type, .subtype = -1)

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
	 */

	while(!eof)
	{
		cur_token =	psh_memory_push(&lexer->tokens);

		if(last_token != &token_none)
		{
			psh_memory_reload(&lexer->tokens, last_token);
		}

		in_prev = in;

		/* Whitespace */

		int newline = 0;

		while(psh_lut_whitespace[*in] == 1)
		{
			/* Drain until no whitespace. */

			newline += (*in == '\n');

			next_byte();

			if(eof)
			{
				break;
			}
		}

		if(token_len != 0)
		{
			add_token_ext(psh_token_whitespace, .newline = newline);

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

			next_byte();
			atleast(0);

			if(*in == '/')
			{
				next_byte();

				while(!eof)
				{
					int stop = (*in == '\n');

					next_byte();

					if(stop)
					{
						break;
					}
				}

				add_token(psh_token_comment);

				continue;
			}

			if(*in == '*')
			{
				next_byte();
				atleast(0);
				next_byte();
				atleast(0);

				while(*in != '/' || in[-1] != '*')
				{
					next_byte();
					atleast(0);
				}

				next_byte();
				add_token(psh_token_comment);

				continue;
			}

			if(
				(
					last_token->type == psh_token_symbol &&
					psh_lut_regexp[last_token->token[0]] == 1
				) ||
				(
					last_token->type == psh_token_word &&
					(int)(
						last_token->keyword =
							psh_hash_lookup(&psh_keyword_hashes, (char*)
								last_token->token, last_token->len)
					) != -1
				) ||
				last_token->type == psh_token_none
			)
			{
				uint8_t in_group = 0;
				uint8_t escaped = 0;

				do
				{
					if(*in == '\n')
					{
						goto goto_newline_in_regexp;
					}

					if(*in == '\\')
					{
						escaped ^= 1;
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

					next_byte();
				}
				while(!eof);

				atleast(0);
				next_byte();
				add_token(psh_token_regexp);
				set_as_last_token();

				continue;
			}

			/* pass through */
		}

		/* Symbol */ /* TODO this and '/' above - gather all symbols and then collectively hash them. so just put a goto above and we good. */

		if(psh_lut_symbols[*in] == 1)
		{
			if(nest_stack.used != 0)
			{
				uint32_t* nest =
					(uint32_t*) nest_stack.ptr + nest_stack.used - 1;

				if(*in == '}')
				{
					if(*nest == 0)
					{
						psh_memory_ret(&nest_stack, 1);

						goto goto_template_string;
					}

					*nest -= 1;
				}
				else if(*in == '{')
				{
					*nest += 1;
				}
			}

			next_byte();
			add_token(psh_token_symbol);
			set_as_last_token();

			continue;
		}

		/* Number */ /* TODO use strtod to convert to double. also verify the number here, while parsing. but only use the function for floating point!!!!! */

		uint8_t char_code = *in - '0';

		if(char_code <= 9)
		{
			do
			{
				next_byte();
			}
			while(!eof && (
				psh_lut_number_chars[*in] == 1 ||
				( /* Check if a plus/minus sign is allowed */
					CASE_INSENSITIVE(in[-1]) == 'E' &&
					(*in == '+' || *in == '-')
				)
			));

			if(
				last_token->type == psh_token_symbol &&
				last_token->token[0] == '.'
			)
			{
				/**
				 * Checking for a symbol occurs earlier than checking for
				 * a number, so if a dot ocurred, it wouldn't belong here.
				 */

				extend_token(psh_token_number);
			}
			else
			{
				add_token(psh_token_number);
			}

			set_as_last_token();

			continue;
		}

		/* String */

		if(*in == '"' || *in == '\'')
		{
			uint8_t end = *in;
			uint8_t escaped = 0;

			while(1)
			{
				if(*in == '\n')
				{
					goto goto_newline_in_string;
				}

				if(*in == '\\')
				{
					escaped ^= 1;
				}
				else
				{
					escaped = 0;
				}

				next_byte();
				atleast(0);

				if(*in == end && !escaped)
				{
					next_byte();
					break;
				}
			}

			add_token(psh_token_string);
			set_as_last_token();

			continue;
		}

		if(*in == '`')
		{
			goto_template_string:;

			uint8_t do_nest = 0;
			uint8_t escaped = 0;

			while(1)
			{
				if(*in == '\\')
				{
					escaped ^= 1;
				}
				else
				{
					escaped = 0;
				}

				next_byte();
				atleast(0);

				if(*in == '`' && !escaped)
				{
					next_byte();
					break;
				}

				if(*in == '$')
				{
					atleast(1);

					if(in[1] == '{' && !escaped)
					{
						do_nest = 1;
						in += 2;

						break;
					}
				}
			}

			if(do_nest)
			{
				*(uint32_t*) psh_memory_get(&nest_stack, 1) = 0;

				add_token(psh_token_template);
			}
			else
			{
				add_token(psh_token_template_end);
			}

			set_as_last_token();

			continue;
		}

		/* Word */

		do
		{
			next_byte();
		}
		while(!eof && psh_lut_word[*in] == 1);

		add_token(psh_token_word);
		set_as_last_token();
	}

#undef add_token
#undef add_token_ext
#undef extend_token
#undef eof
#undef token_len
#undef next_byte
#undef atleast
#undef set_as_last_token

	goto goto_return;


	goto_unexpected:

	in = in_end;

	if(state->opt->lexer_ignore_trailing_code)
	{
		psh_state_msg(state,
		(struct psh_message)
		{
			.level = psh_level_info,
			.status = psh_status_unexpected_end,
			.start = in_prev - arr,
			.end = in - arr
		}
		);

		status = psh_status_ok;
		goto goto_return;
	}

	psh_state_msg(state,
	(struct psh_message)
	{
		.level = psh_level_error,
		.status = psh_status_unexpected_end,
		.start = in_prev - arr,
		.end = in - arr
	}
	);

	status = psh_status_unexpected_end;
	goto goto_return;


	goto_newline_in_string:

	psh_state_msg(state,
	(struct psh_message)
	{
		.level = psh_level_error,
		.status = psh_status_newline_in_string,
		.start = in_prev - arr,
		.end = in - arr
	}
	);

	status = psh_status_newline_in_string;
	goto goto_return;


	goto_newline_in_regexp:

	psh_state_msg(state,
	(struct psh_message)
	{
		.level = psh_level_error,
		.status = psh_status_newline_in_regexp,
		.start = in_prev - arr,
		.end = in - arr
	}
	);

	status = psh_status_newline_in_regexp;
	goto goto_return;


	goto_return:

	psh_memory_free(&nest_stack);

	return status;

#undef get_location
}

#undef CASE_INSENSITIVE
