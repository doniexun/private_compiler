#include <stdio.h>
#include <stdlib.h>
#include "scan.h"
#include "text.h"

void handletoken(struct token *token)
{
	printf("\t%d", token->lineno);
	switch (token->type) {
	case tokenunknown:
		printf("[UNKNOWN]\n");
		break;
	case tokeneof:
		printf("[EOF]\n");
		break;
	case tokenerr:
		printf("[ERROR] %s\n", token->buf);
		break;
	case tokensym:
		printf("[symbol] %s\n", token->buf);
		break;
	case tokennum:
		printf("[number] val=%s\n", token->buf);
		break;
	case tokenid:
		if (reservedword(token->buf))
			printf("[reserved word] %s\n", token->buf);
		else
			printf("[identifier] name=%s\n", token->buf);

		break;
	default:
		printf("[token handling error]\n");
		break;
	}
}

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
