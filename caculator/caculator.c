#include <stdio.h>
#include <stdlib.h>
#include "../scanner/scan.h"
#include "../scanner/text.h"

/* some auxiliary function */
static inline int isend(struct token *token)
{
	if (token->type == tkeof)
		return 1;
	return 0;
}

static inline int tokennumber(struct token *token)
{
	return atoi(token->buf);
}

/* internal token stream handling */
#define DEBUG
static struct token __token;
static int backtoken = 0;

static void back_token(void)
{
	if (backtoken) {
		fprintf(stderr, "double back_token calls\n");
		exit(EXIT_FAILURE);
	}
	backtoken = 1;
}

static struct token *get_token(void)
{
	if (backtoken) {
		backtoken = 0;
		return &__token;
	}

	inittoken(&__token);
	gettoken(&__token);
#ifdef DEBUG
	fprintf(stdout, "%s", isend(&__token) ? "" : __token.buf);
	fflush(stdout);
#endif
	return &__token;
}

static void experror(void)
{
	fprintf(stderr, "\n[error token:%s]\n",
			isend(&__token) ? "EOF" : __token.buf);
	exit(EXIT_FAILURE);
}

/*
 * Top-down parsing by Recursive-Descent
 *
 * BNF for expression:
 *   exp   -> exp addop term | term
 *   addop -> + | -
 *   term  -> term mulop factor | factor
 *   factor -> (exp) | number
 *
 *   ====translate BNF to EBNF====>
 *
 * EBNF for expression:
 *   exp   -> term { addop term }
 *   term  -> factor { mulop factor }
 *   factor -> (exp) | number
 */
static int exp(void);
static int factor(void)
{
	struct token *token;
	int ret;

	token = get_token();
	switch (token->type) {
	case tkend:
		/*
		 * factor() is only called by term().
		 * So factor() returns 1, which has bad effect on term().
		 */
		ret = 1;
		break;
	case tklparen:
		/* exp will get next token */
		ret = exp();
		token = get_token();
		if (token->type != tkrparen)
			experror();
		break;
	case tknum:
		ret = tokennumber(token);
		break;
	default:
		experror();
		break;
	}

	return ret;
}

static int term(void)
{
	struct token *token;
	int ret, cont;

	ret = factor();
	cont = 1;
	while (cont && !isend(token = get_token())) {
		switch (token->type) {
		case tkmul:
			ret *= factor();
			break;
		case tkdiv:
			ret /= factor();
			break;
		default:
			back_token();
			cont = 0;
			break;
		}
	}

	return ret;
}

static int exp(void)
{
	struct token *token;
	int ret, cont;

	ret = term();
	cont = 1;
	while (cont && !isend(token = get_token())) {
		switch (token->type) {
		case tkadd:
			ret += term();
			break;
		case tksub:
			ret -= term();
			break;
		default:
			back_token();
			cont = 0;
			break;
		}
	}

	return ret;
}

static void caculate(void)
{
	printf("\nanswer=%d\n", exp());
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: caculator expressionfile\n");
		exit(EXIT_FAILURE);
	}
	inittexthandle(argv[1]);
	caculate();
	return 0;
}
