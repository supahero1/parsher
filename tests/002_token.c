#include <assert.h>
#include <inttypes.h>

#include <parsher/token.h>


struct psh_tokens out = {0};


static void
_test(char* str, uint64_t len, enum psh_status ret, int line,
	enum psh_token_type* expected, uint64_t expected_len)
{
	printf("Testing line %d... ", line);
	fflush(stdout);

	struct psh_source source =
	(struct psh_source)
	{
		.arr = (uint8_t*) str,
		.len = len
	};

#define log_output()								\
	do												\
	{												\
		printf("expected output:");					\
													\
		for(uint64_t i = 0; i < expected_len; ++i)	\
		{											\
			printf(" %d", expected[i]);				\
		}											\
													\
		printf("\n actual  output:");				\
													\
		for(uint64_t i = 0; i < expected_len; ++i)	\
		{											\
			printf(" %d", out.tokens[i].type);		\
		}											\
													\
		puts("");									\
	}												\
	while(0)

	psh_tokens_reset(&out);

	enum psh_status stat = psh_tokenize(&source, &out);

	if(ret != stat)
	{
		printf("ret mismatch, expected %d, got %d\n", ret, stat);

		log_output();

		assert(0);
	}

	if(out.used != expected_len)
	{
		printf("token len mismatch, expected %" PRIu64 ", got %" PRIu64 "\n",
			expected_len, out.used);

		log_output();

		assert(0);
	}

	for(uint64_t i = 0; i < out.used; ++i)
	{
		if(out.tokens[i].type != expected[i])
		{
			printf("token type mismatch, expected %d, got %d\n",
				expected[i], out.tokens[i].type);

			assert(0);
		}
	}

	puts("done");
}


#define test(s, r, ...) \
_test((s), sizeof(s) - 1, r, __LINE__, __VA_ARGS__, \
	sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))


int
main()
{
	psh_tokens_init(&out);

	test("", psh_ok,
		(enum psh_token_type[])
		{  }
	);

	test(" ", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace
		}
	);

	test("\n\r \t \n \r ", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace
		}
	);

	test("//", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_comment
		}
	);

	test("// abcdefg\n//", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_comment,
			psh_token_comment
		}
	);

	test("// abcdefg\n\n//\r\t\n//\t", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_comment,
			psh_token_whitespace,
			psh_token_comment,
			psh_token_comment
		}
	);

	test("/", psh_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("/ /", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_regexp
		}
	);

	test(" / \\/\\/* abc 123 \r\t\n\t\r \\\\///", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace,
			psh_token_regexp,
			psh_token_comment
		}
	);

	test("/*  /", psh_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("/*/", psh_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("/*//*\\*/", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_comment
		}
	);

	test("await / /", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_word,
			psh_token_whitespace,
			psh_token_regexp
		}
	);

	test("/ / / / /", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_regexp,
			psh_token_whitespace,
			psh_token_symbol,
			psh_token_whitespace,
			psh_token_regexp
		}
	);

	test("0 0x0 0X0 0n 0N 0o0 0O0 0b0 0B0 0_0", psh_ok,
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

	test("0.0 .0 0.0e+2 0.E-2 0e+2 .0E-2 .0+2e-0", psh_ok,
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

	test("' abc def \\' '", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_string
		}
	);

	test("'", psh_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("\"", psh_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("' /* */ \n \t \" // / / \\''", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_string
		}
	);

	test(" \" /* */ \n \t ' // / / \\\"\" ", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_whitespace,
			psh_token_string,
			psh_token_whitespace
		}
	);

	test("`", psh_unexpected_end,
		(enum psh_token_type[])
		{  }
	);

	test("` abc ' \" \\` /* */ // / /`", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_template_end
		}
	);

	test("` \\${} ${ } \\${} `", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_template,
			psh_token_whitespace,
			psh_token_template_end
		}
	);

	test("` ${ await 2+3 } `", psh_ok,
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

	test("`${`${'abc${}``\"\"/**///'}\\${`}\\${`", psh_ok,
		(enum psh_token_type[])
		{
			psh_token_template,
			psh_token_template,
			psh_token_string,
			psh_token_template_end,
			psh_token_template_end
		}
	);

	psh_tokens_free(&out);

	return 0;
}
