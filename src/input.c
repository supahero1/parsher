#include <parsher/input.h>

#include <stdlib.h>


void
psh_input_init(struct psh_input* input)
{
	psh_memory_init(&input->delimiters, sizeof(uint32_t));
}


void
psh_input_free(struct psh_input* input)
{
	psh_memory_free(&input->delimiters);
}


static void
psh_input_create_positions(struct psh_input* input)
{
	const uint8_t* in_start = input->src->arr;
	const uint8_t* in = in_start;
	const uint8_t* in_end = in + input->src->len;

	psh_memory_atleast(&input->delimiters, 2);

	*(uint32_t*) psh_memory_get(&input->delimiters, 1) = 0;

	while(in != in_end)
	{
		if(*in == '\n')
		{
			*(uint32_t*) psh_memory_get(&input->delimiters, 1) = in - in_start + 1;
		}

		++in;
	}

	if(input->src->len != *(uint32_t*) psh_memory_last(&input->delimiters))
	{
		*(uint32_t*) psh_memory_get(&input->delimiters, 1) = input->src->len + 1;
	}
}


struct psh_position
psh_input_query(struct psh_input* input, uint32_t idx)
{
	if(!input->delimiters.used)
	{
		psh_input_create_positions(input);
	}

	uint32_t low = 0;
	uint32_t mid;
	uint32_t high = input->delimiters.used - 1;
	uint32_t* ptr = input->delimiters.ptr;

	while(1)
	{
		mid = (low >> 1) + (high >> 1);

		if(low >= high)
		{
			break;
		}

		if(idx < ptr[mid])
		{
			high = mid - 1;
		}
		else if(idx >= ptr[mid + 1])
		{
			low = mid + 1;
		}
		else
		{
			break;
		}
	}

	return
	(struct psh_position)
	{
		.idx = idx,
		.line = mid + 1,
		.column = idx - ptr[mid],
		.column_width = ptr[mid + 1] - ptr[mid] - 1
	};
}
