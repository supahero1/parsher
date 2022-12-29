#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <inttypes.h>

#include <parsher/token.h>

static void
_test(char* str, uint64_t len, enum psh_status ret, int line,
    enum psh_token_type* expected, uint64_t expected_len)
{
    printf("Testing line %d... ", line);
    fflush(stdout);

    struct psh_source source =
    (struct psh_source)
    {
        .in = (uint8_t*) str,
        .len = len
    };

    struct psh_tokens out = {0};

    enum psh_status stat = psh_tokenize(&source, &out);

    if(ret != stat)
    {
        printf("ret mismatch, expected %d, got %d\n", ret, stat);
        assert(0);
    }

    if(out.used != expected_len)
    {
        printf("token len mismatch, expected %" PRIu64 ", got %" PRIu64 "\n",
            expected_len, out.used);
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

    free(out.tokens);

    puts("done");
}

#define test(s, r, ...) \
_test((s), sizeof(s) - 1, r, __LINE__, __VA_ARGS__, \
    sizeof(__VA_ARGS__) / sizeof((__VA_ARGS__)[0]))

int
main()
{
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

    test("// abcdefg\n\n//\n//", psh_ok,
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

    test(" / \\/ * abc 123 \\\\///", psh_ok,
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

    test("/* \\*/", psh_ok,
        (enum psh_token_type[])
        {
            psh_token_comment
        }
    );

    // test("/", psh_ok, /* this needs some string or token in front first */
    //     (enum psh_token_type[])
    //     {
    //         psh_token_symbol
    //     }
    // );

    return 0;
}
