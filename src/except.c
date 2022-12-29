#include <stdio.h>
#include <stdlib.h>

#include <parsher/except.h>

void
psh_nomem(void)
{
    fprintf(stderr, "parsher: no memory\n");

    exit(EXIT_FAILURE);
}
