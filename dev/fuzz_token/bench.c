#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

#include <parsher/token.h>


static uint64_t
time_get_ns(void)
{
  struct timespec tp;

  (void) clock_gettime(CLOCK_REALTIME, &tp);

  return tp.tv_sec * 1000000000 + tp.tv_nsec;
}


int
main()
{
#define LEN 100000000

	char* map = mmap(NULL, LEN, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);

	if(map == MAP_FAILED)
	{
		return 1;
	}

	//(void) memset(map, 'a', LEN);

	const struct psh_source src =
	(struct psh_source)
	{
		.arr = map,
		.len = LEN
	};

	struct psh_tokens tokens = {0};

	const uint64_t start = time_get_ns();

	const int ret = psh_tokenize(&src, &tokens) != psh_ok;

	const uint64_t end = time_get_ns();

	printf("%.2Lfsec\n", (long double)(end - start) / 1000000000);

	munmap(map, LEN);
	free(tokens.tokens);

	return ret;
}
