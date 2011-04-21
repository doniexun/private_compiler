#include <stdio.h>
#include <stdlib.h>
#include "../scanner/scan.h"
#include "../scanner/text.h"

#define DEBUG

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

void debugerror(void)
{
	fprintf(stderr, "error\n");
	exit(EXIT_FAILURE);
}

static inline int isend(struct token *token)
{
	if (token->type == tokeneof)
		return 1;
	return 0;
}

static inline int isleftparenthesis(struct token *token)
{
	if (token->type == tokensym && token->buf[0] == '(')
		return 1;
	return 0;
}

static inline int isrightparenthesis(struct token *token)
{
	if (token->type == tokensym && token->buf[0] == ')')
		return 1;
	return 0;
}
static inline int ismulop(struct token *token)
{
	if (token->type == tokensym &&
		(token->buf[0] == '*' || token->buf[0] == '/'))
		return 1;
	return 0;
}
static inline int isaddop(struct token *token)
{
	if (token->type == tokensym &&
		(token->buf[0] == '+' || token->buf[0] == '-'))
		return 1;
	return 0;
}

static inline int isnumber(struct token *token)
{
	if (token->type == tokennum)
		return 1;
	return 0;
}

static inline int tokennumber(struct token *token)
{
	return atoi(token->buf);
}

/* internal token */
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
	fprintf(stdout, "[%s]\n", isend(&__token) ? "EOF" : __token.buf);
#endif
	return &__token;
}

int factor(void)
{
	struct token *token;
	int ret;

	dbg("");

	token = get_token();
	if (isend(token)){
		/* 
		 * factor is only called by term() 
		 * int term(): ret *= 1(factor()) has no side-effect.
		 */
		ret = 1;
	} else if (isleftparenthesis(token)) {
		/* exp will get next token */
		ret = exp();
		token = get_token();
		if (!isrightparenthesis(token))
			debugerror();
	} else if (isnumber(token)){
		ret = tokennumber(token);
	} else {
		debugerror();
	}
	return ret;
}

int term(void)
{
	struct token *token;
	int ret;

	dbg("");
	ret = factor();
	dbg("");

	while (!isend(token = get_token())) {
		if (!ismulop(token)) {
			back_token();
			break;
		}
		switch (token->buf[0]) {
		case '*':
			ret *= factor();
			break;
		case '/':
			ret /= factor();
			break;
		default:
			debugerror();
			break;
		}
	}
	return ret;
}

int exp(void)
{
	struct token *token;
	int ret;

	dbg("");
	ret = term();
	dbg("");

	while (!isend(token = get_token())) {
		if (!isaddop(token)) {
			back_token();
			break;
		}
		switch (token->buf[0]) {
		case '+':
			ret += term();
			break;
		case '-':
			ret -= term();
			break;
		default:
			debugerror();
			break;
		}
	}
	return ret;
}

static void caculate(void)
{
	printf("answer=%d\n", exp());
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
