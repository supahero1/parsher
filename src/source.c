#include <parsher/source.h>


void
psh_sanitize(struct psh_source* src)
{
	const uint8_t* in = src->arr;
	uint32_t len = src->len;
	const uint8_t* end = in + len;

	/* Unicode BOM mark */

	if(len >= 3 && *in == 0xEF && in[1] == 0xBB && in[2] == 0xBF)
	{
		in += 3;
		len -= 3;
	}

	/* Shebang */

	if(len >= 2 && *in == '#' && in[1] == '!')
	{
		in += 2;

		while(in != end)
		{
			int stop = (*in == '\n');

			++in;

			if(stop)
			{
				break;
			}
		}

		len = end - in;
	}

	src->arr = in;
	src->len = len;
}
