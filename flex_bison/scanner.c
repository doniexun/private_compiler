#include <common.h>
#include <scan.h>
#include <text.h>

extern FILE *yyin;
extern struct token *get_token(void);

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

void scan(void)
{
	struct token *token;

	/* explain and handle token */
	do {
		token = get_token();
		handletoken(token);
	} while (token->type != tkeof);
}

void usage(void)
{
	fprintf(stderr, "Usage: scanner [sourcefile]\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if (argc == 2)
		yyin = xfopen(argv[1], "r");
	else if (argc > 2)
		usage();
	scan();
	return 0;
}
