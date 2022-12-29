#include <stdio.h>
#include <assert.h>

#include <parsher/source.h>

int
main()
{
    const char* str = "\xEF\xBB\xBF#!/bin/bash\na";

    assert(psh_sanitize(
        &(
            (struct psh_source)
            {
                .in = (uint8_t*) str,
                .len = sizeof(str) - 1
            }
        ),
        &(
            (struct psh_source)
            {
                .in = (uint8_t*) str + 15,
                .len = 1
            }
        )
    ) == psh_ok);

    str = "abcd";

    assert(psh_sanitize(
        &(
            (struct psh_source)
            {
                .in = (uint8_t*) str,
                .len = sizeof(str) - 1
            }
        ),
        &(
            (struct psh_source)
            {
                .in = (uint8_t*) str,
                .len = sizeof(str) - 1
            }
        )
    ) == psh_ok);

    str = "a";

    assert(psh_sanitize(
        &(
            (struct psh_source)
            {
                .in = (uint8_t*) str,
                .len = sizeof(str) - 1
            }
        ),
        &(
            (struct psh_source)
            {
                .in = (uint8_t*) str,
                .len = sizeof(str) - 1
            }
        )
    ) == psh_ok);

    return 0;
}
