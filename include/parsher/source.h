#ifndef _parsher_source_h_
#define _parsher_source_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


struct psh_source
{
    const uint8_t* arr;
    uint32_t len;
};


extern void
psh_sanitize(struct psh_source* src);


#ifdef __cplusplus
}
#endif

#endif
