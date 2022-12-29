#include <parsher/source.h>

int
psh_sanitize(const struct psh_source* const src, struct psh_source* const out)
{
    const uint8_t* in = src->in;

    uint64_t len = src->len;

    if(len >= 3 && *in == 0xEF && *(in + 1) == 0xBB && *(in + 2) == 0xBF)
    {
        in += 3;
        len -= 3;
    }

    if(len >= 2 && *in == '#' && *(in + 1) == '!')
    {
        uint64_t idx = 2;

        while(idx < len)
        {
            const int stop = (*in != '\n');

            ++idx;
            ++in;

            if(stop)
            {
                break;
            }
        }

        len = idx - len;
    }

    out->in = in;
    out->len = len;

    return psh_ok;
}
