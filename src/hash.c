#include <parsher/hash.h>
#include <parsher/except.h>

#include <string.h>


void
psh_hash_init(struct psh_hashes* hashes, char** dict, uint32_t count)
{
	hashes->source = cmph_io_vector_adapter(dict, count);

	if(!hashes->source)
	{
		psh_nomem();
	}

	cmph_config_t* config = cmph_config_new(hashes->source);

	if(!config)
	{
		psh_nomem();
	}

	cmph_config_set_algo(config, CMPH_BDZ_PH);

	hashes->cmph = cmph_new(config);

	if(!hashes->cmph)
	{
		psh_nomem();
	}

	cmph_config_destroy(config);

	uint32_t size = count << 1;

	hashes->hashes = malloc(sizeof(*hashes->hashes) * size);

	if(!hashes->hashes)
	{
		psh_nomem();
	}

	hashes->dict = dict;

	uint32_t max = 0;

	for(uint32_t i = 0; i < count; ++i)
	{
		if(i >= size)
		{
			size = i << 1;

			hashes->hashes =
				realloc(hashes->hashes, sizeof(*hashes->hashes) * size);

			if(!hashes->hashes)
			{
				psh_nomem();
			}
		}

		if(i > max)
		{
			max = i;
		}

		size_t len = strlen(dict[i]);

		unsigned idx =
			cmph_search(hashes->cmph, dict[i], len);

		hashes->hashes[idx].len = len;
		hashes->hashes[idx].idx = i;
	}

	void* ptr = realloc(hashes->hashes, sizeof(*hashes->hashes) * max);

	if(ptr)
	{
		hashes->hashes = ptr;
	}
}


void
psh_hash_free(const struct psh_hashes* hashes)
{
	free(hashes->hashes);
	cmph_destroy(hashes->cmph);
	cmph_io_vector_adapter_destroy(hashes->source);
}


int32_t
psh_hash_lookup(const struct psh_hashes* hashes, const char* str, uint32_t len)
{
	unsigned idx = cmph_search(hashes->cmph, str, len);

	const struct psh_hash* hash = hashes->hashes + idx;

	if(hash->len == len && !memcmp(hashes->dict[hash->idx], str, len))
	{
		return hash->idx;
	}

	return -1;
}
