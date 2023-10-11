#ifndef _parsher_input_h_
#define _parsher_input_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include <parsher/memory.h>
#include <parsher/source.h>


enum psh_status
{
    psh_status_ok,
    psh_status_unexpected_end,
	psh_status_newline_in_string,
	psh_status_newline_in_regexp
};


struct psh_position
{
	uint32_t idx;
	uint32_t line;
	uint32_t column;
	uint32_t column_width;
};


struct psh_input
{
	struct psh_source* src;
	struct psh_memory delimiters;
};


extern void
psh_input_init(struct psh_input* input);


extern void
psh_input_free(struct psh_input* input);


extern struct psh_position
psh_input_query(struct psh_input* input, uint32_t idx);


#ifdef __cplusplus
}
#endif

#endif
