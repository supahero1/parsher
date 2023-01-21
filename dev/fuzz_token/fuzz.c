#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <parsher/token.h>


int
main()
{
	char input[4096] = {0};

	ssize_t num = read(STDIN_FILENO, input, sizeof(input));

	if(num < 1)
	{
		return 0;
	}

	const struct psh_source src =
	(struct psh_source)
	{
		.arr = input,
		.len = num
	};

	struct psh_tokens tokens = {0};

	const int ret = psh_tokenize(&src, &tokens) != psh_ok;

	free(tokens.tokens);

	return ret;
}
