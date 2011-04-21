#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scan.h"
#include "text.h"

void inittoken(struct token *token)
{
	token->type = tokenunknown;
	token->pos = 0;
	memset(token->buf, 0x0, sizeof(token->buf));
}

void gettoken(struct token *token)
{
	char c;
skip:
	/* skip whitespace */
	while ((c = nextchar()) && iswhitespace(c))
		;
	/* skip comment */
	if (c == '{') {
		while ((c = nextchar()) && c != '}')
			;
		goto skip;
	}

	/* eof */
	if (!c) {
		settokeneof(token);
	/* symbol */
	} else if (issymbol(c)) {
		settokentype(token, tokensym);
		if (c == ':') {
			if ((c = nextchar()) == '=') {
				settokenstr(token, ":=");
			} else {
				settokenerror(token);
				backchar(c);
			}
		} else {
			settokenchar(token, c);
		}
	/* number */
	} else if (isdigit(c)) {
		settokentype(token, tokennum);
		settokenchar(token, c);
		while ((c = nextchar()) && isdigit(c))
			settokenchar(token, c);
		backchar(c);
	/* identifier */
	} else if (isletter(c)) {
		settokentype(token, tokenid);
		settokenchar(token, c);
		while ((c = nextchar()) && isletter(c))
			settokenchar(token, c);
		backchar(c);
	/* error */
	} else {
		settokenerror(token);
	}
}

void handletoken(struct token *token)
{
	printf("\t");
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
