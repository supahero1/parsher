#ifndef _parsher_memory_h_
#define _parsher_memory_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


struct psh_memory
{
	void* ptr;
	uintptr_t diff;
	uint32_t used;
	uint32_t size;
	uint32_t el_size;
};


#define psh_memory_reload(mem, ptr_) ((ptr_) = (void*)(ptr_) + (mem)->diff)


__attribute__((noreturn))
extern void
psh_nomem(void);


extern void
psh_memory_init(struct psh_memory* mem, uint32_t el_size);


extern void
psh_memory_free(struct psh_memory* mem);


extern void
psh_memory_atleast(struct psh_memory* mem, uint32_t count);


extern void*
psh_memory_get(struct psh_memory* mem, uint32_t count);


extern void*
psh_memory_push(struct psh_memory* mem);


extern void
psh_memory_ret(struct psh_memory* mem, uint32_t count);


extern void*
psh_memory_last(struct psh_memory* mem);


#ifdef __cplusplus
}
#endif

#endif
