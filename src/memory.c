#include <parsher/memory.h>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>


void
psh_nomem(void)
{
	fputs("parsher: no memory", stderr);
	exit(EXIT_FAILURE);
}


void
psh_memory_init(struct psh_memory* mem, uint32_t el_size)
{
	mem->used = 0;

	if(mem->ptr)
	{
		assert(mem->el_size == el_size &&
			"Reinitialization of struct psh_memory"
			" must use the same element size.");
	}

	mem->el_size = el_size;
}


void
psh_memory_free(struct psh_memory* mem)
{
	free(mem->ptr);
}


static int
psh_memory_realloc(struct psh_memory* mem, uint32_t new_size)
{
	uint32_t allocation_size;

	if(__builtin_mul_overflow(new_size, mem->el_size, &allocation_size))
	{
		return 1;
	}

	void* ptr = realloc(mem->ptr, allocation_size);

	if(!ptr)
	{
		return 1;
	}

	mem->diff = ptr - mem->ptr;
	mem->ptr = ptr;
	mem->size = new_size;

	return 0;
}


void
psh_memory_atleast(struct psh_memory* mem, uint32_t new_used)
{
	if(new_used <= mem->size)
	{
		mem->diff = 0;

		return;
	}

	uint32_t i = 0;
	double mul = 2.0f;

	while(1)
	{
		uint32_t new_size = new_used * mul;

		if(new_size >= new_used && !psh_memory_realloc(mem, ++new_size))
		{
			break;
		}

		if(++i == 21)
		{
			psh_nomem();
		}

		mul *= 0.9659363289248455f;
	}
}


void*
psh_memory_get(struct psh_memory* mem, uint32_t count)
{
	uint32_t new_used = mem->used + count;
	psh_memory_atleast(mem, new_used);

	void* ret = mem->ptr + mem->used * mem->el_size;
	mem->used = new_used;

	return ret;
}


void*
psh_memory_push(struct psh_memory* mem)
{
	psh_memory_atleast(mem, mem->used + 1);

	return mem->ptr + mem->used * mem->el_size;
}


void
psh_memory_ret(struct psh_memory* mem, uint32_t count)
{
	mem->used -= count;

	if(mem->used >= (mem->size >> 2))
	{
		mem->diff = 0;

		return;
	}

	(void) psh_memory_realloc(mem, mem->size >> 1);
}


void*
psh_memory_last(struct psh_memory* mem)
{
	return mem->ptr + (mem->used - 1) * mem->el_size;
}
