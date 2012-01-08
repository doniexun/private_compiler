#include <common.h>
#include <scan.h>
#include <text.h>

void handletoken(struct token *token)
{
	printf("\t%-4d", token->lineno);
	switch (token->type) {
	case tkunknown:
		printf("[UNKNOWN]\n");
		break;
	case tkeof:
		printf("[EOF]\n");
		break;
	case tkeq:
	case tklt:
	case tkadd:
	case tksub:
	case tkmul:
	case tkdiv:
	case tkmod:
	case tksemi:
	case tklparen:
	case tkrparen:
	case tkassign:
		printf("[symbol] %s\n", token->buf);
		break;
	case tknum:
		printf("[number] val=%s\n", token->buf);
		break;
	case tkid:
		printf("[identifier] name=%s\n", token->buf);
		break;
	case tkif:
	case tkthen:
	case tkelse:
	case tkend:
	case tkrepeat:
	case tkuntil:
	case tkread:
	case tkwrite:
		printf("[reserved word] %s\n", token->buf);
		break;
	case tkerr:
		printf("[ERROR] %s\n", token->buf);
		break;
	default:
		printf("[ERROR] oops!\n");
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
	} while (token.type != tkeof);
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
