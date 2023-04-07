#include <parsher/source.h>


int
psh_sanitize(struct psh_source* src)
{
	const uint8_t* in = src->arr;

	uint64_t len = src->len;

	/* Unicode BOM mark */

	if(len >= 3 && *in == 0xEF && *(in + 1) == 0xBB && *(in + 2) == 0xBF)
	{
		in += 3;
		len -= 3;
	}

	/* Shebang */

	if(len >= 2 && *in == '#' && *(in + 1) == '!')
	{
		uint64_t idx = 2;

		in += 2;

		while(idx < len)
		{
			int stop = (*in == '\n');

			++idx;
			++in;

			if(stop)
			{
				break;
			}
		}

		len -= idx;
	}

	src->arr = in;
	src->len = len;

	return psh_ok;
}
