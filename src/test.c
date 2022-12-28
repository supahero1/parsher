#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include <shnet/time.h>

const uint8_t lut2[] =
{
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
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    3, 3, 3, 3, 3, 3, 3, 3,-1,-1,-1,-1,-1,-1,-1,-1
};

#define LEN (1ULL << 32ULL)

int main() {
    const uint8_t lut[] = { 0xff, 0x3f, 0x1f, 0x0f };

    const uint8_t _test[] = { 0x24, 0xc2, 0xa3, 0xe0, 0xa4, 0xb9, 0xe2, 0x82, 0xac, 0xed, 0x95, 0x9c, 0xf0, 0x90, 0x8d, 0x88 };

    uint8_t* const test = calloc(LEN, 1);
    assert(test);

    for(uint64_t i = 0; i < LEN; i += 16) {
        memcpy(test + i, _test + (i & 15), 16);
    }

    const uint8_t* in_ptr = test;
    const uintptr_t last = (uintptr_t) test + LEN;

    uint32_t sum = 0;

    const uint64_t start = time_get_time();

    while((uintptr_t) in_ptr < last)
    {
        const uint8_t len = lut2[*in_ptr];

        if(__builtin_expect(len == -1, 0)) {
            goto assert_ez;
        }

        uint32_t num = *(in_ptr++) & lut[len];
        for(uint8_t k = 0; k < len; ++k) {
            num <<= 6;
            num |= *(in_ptr++) & 0x3f;
        }

        sum += num;
    }

    const uint64_t end = time_get_time();

    printf("%5.2lfms\n", (end - start) / 1000000.0);

    return 0;

    assert_ez:

    assert(!"len was -1 rip");

    return 0;
}
