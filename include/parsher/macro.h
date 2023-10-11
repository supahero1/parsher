#ifndef _parsher_macro_h_
#define _parsher_macro_h_

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b)               \
({                              \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a > _b ? _b : _a;          \
})

#define MAX(a, b)               \
({                              \
    __typeof__ (a) _a = (a);    \
    __typeof__ (b) _b = (b);    \
    _a > _b ? _a : _b;          \
})

#ifdef __cplusplus
}
#endif

#endif
