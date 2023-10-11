#ifndef _parsher_assert_h_
#define _parsher_assert_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#define psh_printf_type(X)	\
_Generic((X),				\
	int8_t:			PRId8,	\
	int16_t:		PRId16,	\
	int32_t:		PRId32,	\
	int64_t:		PRId64,	\
	uint8_t:		PRIu8,	\
	uint16_t:		PRIu16,	\
	uint32_t:		PRIu32,	\
	uint64_t:		PRIu64,	\
	float:			"f",	\
	double:			"lf",	\
	long double:	"Lf",	\
	default:		"p"		\
)

#define psh_assert_msg(Op, Desc)	\
"Assertion \"%%s " Op " %%s\" failed: '%%%s' " Desc " '%%%s', at %%s:%%d.\n"

#define psh_assert_eq(A, B)								\
do														\
{														\
	__typeof__(A) a = (A);								\
	__typeof__(A) b = (B);								\
														\
	if(__builtin_expect(a != b, 0))						\
	{													\
		char str[4096];									\
		sprintf(str, psh_assert_msg("==", "is not"),	\
			psh_printf_type(A), psh_printf_type(B));	\
		fprintf(stderr, str, #A, #B,					\
			a, b, __FILE__, __LINE__);					\
		abort();										\
	}													\
}														\
while(0)

#define psh_assert_neq(A, B)							\
do														\
{														\
	__typeof__(A) a = (A);								\
	__typeof__(A) b = (B);								\
														\
	if(__builtin_expect(a == b, 0))						\
	{													\
		char str[4096];									\
		sprintf(str, psh_assert_msg("!=", "is"),		\
			psh_printf_type(A), psh_printf_type(B));	\
		fprintf(stderr, str, #A, #B,					\
			a, b, __FILE__, __LINE__);					\
		abort();										\
	}													\
}														\
while(0)

#ifdef __cplusplus
}
#endif

#endif
