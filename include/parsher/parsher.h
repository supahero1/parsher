#ifndef _parsher_parsher_h_
#define _parsher_parsher_h_

#ifdef __cplusplus
extern "C" {
#endif

#include <parsher/lex.h>


enum psh_stage
{
	psh_stage_lex = 1
};


struct parsher
{
	enum psh_stage stage;
	enum psh_stage current_stage;

	struct psh_state* state;
	struct psh_lexer lexer;
};


extern void
parsher_init(struct parsher* psh);


extern void
parsher_free(struct parsher* psh);


extern enum psh_status
parsher_execute(struct parsher* psh);


#ifdef __cplusplus
}
#endif

#endif
