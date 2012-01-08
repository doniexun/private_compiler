#include <common.h>
#include <scan.h>
#include <text.h>

void inittoken(struct token *token)
{
	token->type = tkunknown;
	token->lineno = 0;
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
	/* set token line number */
	token->lineno = getlineno();
	/* eof */
	if (!c) {
		settokeneof(token);
	/* symbol */
	} else if (issymbol(c)) {
		/* set token char firstly */
		settokenchar(token, c);
		switch (c) {
		case '!':
			if ((c = nextchar()) == '=') {
				settokentype(token, tkneq);
			} else {
				settokenerror(token);
				backchar(c);
			}
			break;
		case ':':
			if ((c = nextchar()) == '=') {
				settokentype(token, tkassign);
				settokenchar(token, '=');
			} else {
				settokenerror(token);
				backchar(c);
			}
			break;
		case ';':
			settokentype(token, tksemi);
			break;
		case '<':
			settokentype(token, tklt);
			break;
		case '=':
			settokentype(token, tkeq);
			break;
		case '+':
			settokentype(token, tkadd);
			break;
		case '-':
			settokentype(token, tksub);
			break;
		case '*':
			settokentype(token, tkmul);
			break;
		case '/':
			settokentype(token, tkdiv);
			break;
		case '%':
			settokentype(token, tkmod);
			break;
		case '(':
			settokentype(token, tklparen);
			break;
		case ')':
			settokentype(token, tkrparen);
			break;
		default:
			settokenerror(token);
			backchar(c);
			break;
		}
	/* number */
	} else if (isdigit(c)) {
		settokentype(token, tknum);
		settokenchar(token, c);
		while ((c = nextchar()) && isdigit(c))
			settokenchar(token, c);
		backchar(c);
	/* identifier */
	} else if (isletter(c)) {
		settokentype(token, tkid);
		settokenchar(token, c);
		while ((c = nextchar()) && isletter(c))
			settokenchar(token, c);
		/* Is token reserved word? */
		settokenreservedword(token);
		backchar(c);
	/* error */
	} else {
		settokenerror(token);
	}
}
