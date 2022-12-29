#ifndef _parsher_source_h_
#define _parsher_source_h_ 1

#ifdef __cplusplus
extern "C" {
#endif


#include <stdint.h>


struct psh_source
{
    const uint8_t* in;

    uint64_t len;
};


enum psh_status
{
    psh_ok,
    psh_unexpected_end
};


extern int
psh_sanitize(const struct psh_source* const, struct psh_source* const);


#ifdef __cplusplus
}
#endif

#endif
