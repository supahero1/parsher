#include <parsher/assert.h>
#include <parsher/source.h>


int
main()
{
	struct psh_source in;
	struct psh_source out;

	in.arr = (uint8_t*) "\xEF\xBB\xBF#!/bin/bash\na";
	in.len = 16;
	out = in;

	psh_sanitize(&out);
	psh_assert_eq(out.arr, in.arr + 15);
	psh_assert_eq(out.len, 1);

	in.arr = (uint8_t*) "abcd";
	in.len = 4;
	out = in;

	psh_sanitize(&out);
	psh_assert_eq(out.arr, in.arr);
	psh_assert_eq(out.len, in.len);

	in.arr = NULL;
	in.len = 0;
	out = in;

	psh_sanitize(&out);
	psh_assert_eq(out.arr, in.arr);
	psh_assert_eq(out.len, in.len);

	return 0;
}
