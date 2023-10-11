#include <parsher/parsher.h>


void
parsher_init(struct parsher* psh)
{
	if(psh->stage >= psh_stage_lex)
	{
		psh_lexer_init(&psh->lexer);
	}
}


void
parsher_free(struct parsher* psh)
{
	if(psh->stage >= psh_stage_lex)
	{
		psh_lexer_free(&psh->lexer);
	}
}


enum psh_status
parsher_execute(struct parsher* psh)
{
	psh->current_stage = psh_stage_lex;
	enum psh_status status;

	if(psh->stage >= psh_stage_lex)
	{
		status = psh_lex(psh->state, &psh->lexer);

		if(status != psh_status_ok)
		{
			return status;
		}
	}

	return psh_status_ok;
}
