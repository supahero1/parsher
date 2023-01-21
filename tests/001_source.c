#include <stdio.h>
#include <assert.h>

#include <parsher/source.h>


int
main()
{
	struct psh_source in;
	struct psh_source out;

	in.arr = (uint8_t*) "\xEF\xBB\xBF#!/bin/bash\na";
	in.len = 16;
	out = in;

	assert(psh_sanitize(&out) == psh_ok);
	assert(out.arr == in.arr + 15);
	assert(out.len == 1);

	in.arr = (uint8_t*) "abcd";
	in.len = 4;
	out = in;

	assert(psh_sanitize(&out) == psh_ok);
	assert(out.arr == in.arr);
	assert(out.len == in.len);

	in.arr = NULL;
	in.len = 0;
	out = in;

	assert(psh_sanitize(&out) == psh_ok);
	assert(out.arr == in.arr);
	assert(out.len == in.len);

	return 0;
}
