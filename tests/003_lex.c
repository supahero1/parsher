#include <parsher/lex.h>
#include <parsher/assert.h>


struct psh_lexer lexer = {0};


static void
_test(char* str, uint32_t len, enum psh_status ret, int line,
	enum psh_token_type* expected, uint32_t expected_len)
{
	printf("Testing line %d... ", line);
	fflush(stdout);

	struct psh_source source =
	(struct psh_source)
	{
		.arr = (uint8_t*) str,
		.len = len
	};
	psh_sanitize(&source);

	struct psh_input input =
	(struct psh_input)
	{
		.src = &source,
		.delimiters = {0}
	};
	psh_input_init(&input);

	struct psh_options opt;
	psh_default_options(&opt);

	struct psh_state state =
	(struct psh_state)
	{
		.input = &input,
		.opt = &opt,
		.messages = {0}
	};
	psh_state_init(&state);

	psh_lexer_init(&lexer);

	enum psh_status stat = psh_lex(&state, &lexer);

	struct psh_token* tokens = lexer.tokens.ptr;

#define log_output()									\
	do													\
	{													\
		printf("expected output:");						\
														\
		for(uint64_t i = 0; i < expected_len; ++i)		\
		{												\
			printf(" %d", expected[i]);					\
		}												\
														\
		printf("\n actual  output:");					\
														\
		for(uint64_t i = 0; i < lexer.tokens.used; ++i)	\
		{												\
			printf(" %d", tokens[i].type);				\
		}												\
														\
		puts("");										\
	}													\
	while(0)

	if(ret != stat)
	{
		puts("return value mismatch");
		log_output();
		psh_assert_eq(ret, stat);
	}

	if(lexer.tokens.used != expected_len)
	{
		puts("token len mismatch");
		log_output();
		psh_assert_eq(expected_len, lexer.tokens.used);
	}

	for(uint64_t i = 0; i < lexer.tokens.used; ++i)
	{
		if(tokens[i].type != expected[i])
		{
			puts("token type mismatch");
			psh_assert_eq(expected[i], tokens[i].type);
		}
	}

	psh_state_free(&state);
	psh_input_free(&input);

	puts("done");
}


#define test(s, r, ...) \
_test((s), sizeof(s) - 1, r, __LINE__, __VA_ARGS__, \
	sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))


int
main()
{
	test("", psh_status_ok,
		(enum psh_token_type[])
		{  }
	);

	test(" ", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace
		}
	);

	test("\n\r \t \n \r ", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace
		}
	);

	test("//", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_comment
		}
	);

	test("// abcdefg\n//", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_comment,
			psh_token_comment
		}
	);

	test("// abcdefg\n\n//\r\t\n//\t", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_comment,
			psh_token_whitespace,
			psh_token_comment,
			psh_token_comment
		}
	);

	test("/", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("/ /", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_regexp
		}
	);

	test(" / \\/\\/* abc 123 \r\t\\n\t\r \\\\///", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace,
			psh_token_regexp,
			psh_token_comment
		}
	);

	test("/*  /", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("/*/", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("/*//*\\*/", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_comment
		}
	);

	test("await / /", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_word,
			psh_token_whitespace,
			psh_token_regexp
		}
	);

	test("/ / / / /", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_regexp,
			psh_token_whitespace,
			psh_token_symbol,
			psh_token_whitespace,
			psh_token_regexp
		}
	);

	test("0 0x0 0X0 0n 0N 0o0 0O0 0b0 0B0 0_0", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number
		}
	);

	test("0.0 .0 0.0e+2 0.E-2 0e+2 .0E-2 .0+2e-0", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_whitespace,

			psh_token_number,
			psh_token_symbol,
			psh_token_number
		}
	);

	test("' abc def \\' '''", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_string,
			psh_token_string
		}
	);

	test("'", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("\"", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("' /* */ \\n \t \" // / / \\''", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_string
		}
	);

	test(" \" /* */ \\n \t ' // / / \\\"\" ", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace,
			psh_token_string,
			psh_token_whitespace
		}
	);

	test("`", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("` abc ' \" \\` /* */ // / /`", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template_end
		}
	);

	test("` \\${} ${ } \\\\${ } `", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template,
			psh_token_whitespace,
			psh_token_template,
			psh_token_whitespace,
			psh_token_template_end
		}
	);

	test("` ${ await 2+3 } `", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template,

			psh_token_whitespace,
			psh_token_word,
			psh_token_whitespace,
			psh_token_number,
			psh_token_symbol,
			psh_token_number,
			psh_token_whitespace,

			psh_token_template_end
		}
	);

	test("`${`${'abc${}``\"\"/**///'}\\${`}\\${`", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template,
			psh_token_template,
			psh_token_string,
			psh_token_template_end,
			psh_token_template_end
		}
	);

	test("'\\\\'", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_string
		}
	);

	test("'\\\\\\'", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("`\\${a}\\\\${a}`", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template,
			psh_token_word,
			psh_token_template_end
		}
	);

	test("`\\\\\\`", psh_status_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("`\\\\\\${ 2 }`", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template_end
		}
	);

	test("'\n", psh_status_newline_in_string,
		(enum psh_token_type[])
		{  }
	);

	test("`${{`${ }`}}`", psh_status_ok,
		(enum psh_token_type[])
		{
			psh_token_template,
			psh_token_symbol,
			psh_token_template,
			psh_token_whitespace,
			psh_token_template_end,
			psh_token_symbol,
			psh_token_template_end
		}
	);

	test("/\n/", psh_status_newline_in_regexp,
		(enum psh_token_type[])
		{  }
	);

	psh_lexer_free(&lexer);

	return 0;
}
