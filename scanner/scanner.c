#include <stdio.h>
#include <stdlib.h>
#include "scan.h"
#include "text.h"

void scan(char *source)
{
	struct token token;
	char c;

	/* init handling of source file*/
	inittexthandle(source);

	/* explain and handle token */
	do {
		inittoken(&token);
		gettoken(&token);
		handletoken(&token);
	} while (token.type != tokeneof);
}

static void usage(void)
{
	fprintf(stderr, "Usage: scan sourcefile\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc != 2)
		usage();
	scan(argv[1]);
	return 0;
}
