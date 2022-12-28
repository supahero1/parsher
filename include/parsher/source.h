#ifndef _parsher_source_h_
#define _parsher_source_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct psh_source
{
    char* in;

    uint64_t len;
};

enum psh_status
{
    psh_ok,
    psh_unexpected_end
};

#ifdef __cplusplus
}
#endif

#endif
