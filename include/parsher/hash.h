#ifndef _parsher_hash_h_
#define _parsher_hash_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <cmph.h>


struct psh_hash
{
	uint32_t len;

	int32_t idx;
};


struct psh_hashes
{
	cmph_io_adapter_t* source;
	cmph_t* cmph;

	char** dict;

	struct psh_hash* hashes;
};


extern void
psh_hash_init(struct psh_hashes* hashes, char** dict, uint32_t count);


extern void
psh_hash_free(const struct psh_hashes* hashes);


extern int32_t
psh_hash_lookup(const struct psh_hashes* hashes, const char* str, uint32_t len);


#ifdef __cplusplus
}
#endif

#endif
