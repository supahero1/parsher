#ifndef _parsher_state_h_
#define _parsher_state_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <parsher/input.h>


struct psh_options
{
	uint64_t lexer_ignore_trailing_code:1;
};


enum psh_level
{
	psh_level_info,
	psh_level_warning,
	psh_level_error
};


struct psh_message
{
	enum psh_level level;
	enum psh_status status;
	uint32_t start;
	uint32_t end;
};


struct psh_state
{
	struct psh_input* input;
	struct psh_options* opt;
	struct psh_memory messages;
};


extern void
psh_default_options(struct psh_options* opt);


extern void
psh_state_init(struct psh_state* state);


extern void
psh_state_free(struct psh_state* state);


extern void
psh_state_msg(struct psh_state* state, struct psh_message msg);


#define psh_max_status_str_len UINT32_C(4096)
#define psh_max_status_occlusion UINT32_C(30)

extern void
psh_state_str(struct psh_state* state,
	struct psh_message* msg, char out[psh_max_status_str_len]);


#ifdef __cplusplus
}
#endif

#endif
