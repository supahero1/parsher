#include <parsher/token.h>


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


#define psh_add_token(type)             \
do                                      \
{                                       \
    out->tokens[out->tokens_len++] =    \
    (struct psh_token)                  \
    {                                   \
        .start = start,                 \
        .end = end,                     \
        .type = (type)                  \
    };                                  \
                                        \
    goto start;                         \
}                                       \
while(0)


/**
 * Make sure there's at least `num` bytes of input available.
 *
 * Call this with `0` to make sure a loop prior
 * to this did not quit because of lack of input.
 */
#define psh_atleast(num)                \
do                                      \
{                                       \
    if(idx + num >= src->len)           \
    {                                   \
        status = psh_unexpected_end;    \
                                        \
        goto out;                       \
    }                                   \
}                                       \
while(0)


/**
 * Move onto the next input byte.
 */
#define psh_next_byte() \
do                      \
{                       \
    ++in;               \
    ++idx;              \
    ++end;              \
}                       \
while(0)


enum psh_status
psh_tokenize(const struct psh_source* const src, struct psh_tokens* const out)
{
    const char* in = src->in;

    uint64_t idx = 0;

    enum psh_status status = psh_ok;

    enum psh_token_type last_type = psh_token_none;

    start:

    /**
     * Detection order:
     *
     * 1. psh_token_whitespace
     *
     * 2. psh_token_comment (contains psh_token_regexp)
     *
     * After that, any order (preferably that yields max avg perf):
     *
     * 3. psh_token_symbol
     *
     * 4. psh_token_number
     *
     * 5. psh_token_string
     *
     * 6. psh_token_word
     */

    while(idx < src->len)
    {
        uint64_t start = idx;
        uint64_t end = idx;

        while(psh_lut_whitespace[*in] == 1)
        {
            /* Drain until no whitespace. */

            psh_next_byte();

            if(idx == src->len)
            {
                break;
            }
        }

        if(start != end)
        {
            psh_add_token(psh_token_whitespace);
        }

        if(*in == '/')
        {
            /**
             * There are a few options here:
             *
             * 1. Next char is a '/' - this is a single-line comment,
             *
             * 2. Next char is a '*' - scan until the next unescaped '/'.
             *    If there's an unescaped '*' at the end as well, this is a
             *    multi-line comment. Otherwise, this is a regexp.
             *
             * 3. Previous token was a symbol or none - this is a regexp,
             *
             * 4. This is a division symbol.
             *
             * Escaping the '/' is an error in every case, so it's not checked.
             */

            psh_atleast(1);
            psh_next_byte();

            if(*in == '/')
            {
                while(idx < src->len)
                {
                    const int stop = (*in == '\n');

                    psh_next_byte();

                    if(stop)
                    {
                        break;
                    }
                }

                psh_add_token(psh_token_comment);
            }

            if(*in == '*')
            {
                while(idx < src->len)
                {
                    if(*in == '/' && *(in - 1) != '\\')
                    {
                        break;
                    }

                    psh_next_byte();
                }

                psh_atleast(0);

                const int is_regexp =
                    end - start == 3 || *(in - 1) != '*' || *(in - 2) == '\\';

                psh_next_byte();

                if(is_regexp)
                {
                    psh_add_token(psh_token_regexp);
                }

                psh_add_token(psh_token_comment);
            }

            if(last_type == psh_token_symbol || last_type == psh_token_none)
            {
                while(idx < src->len)
                {
                    const int stop = (*in == '/' && *(in - 1) != '\\');

                    psh_next_byte();

                    if(stop)
                    {
                        break;
                    }
                }

                psh_atleast(0);

                psh_add_token(psh_token_regexp);
            }

            psh_add_token(psh_token_symbol);
        }

        if(psh_lut_symbols[*in] == 1)
        {
            psh_next_byte();
            psh_add_token(psh_token_symbol);
        }
    }

    out:

    return status;
}

#undef psh_add_token
