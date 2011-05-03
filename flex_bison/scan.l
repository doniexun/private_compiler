%{
#include <scan.h>
#include <stdlib.h>
#define YY_DECL int __yylex(void)
static int lineno = 1;
static int number;

%}

%option noyywrap

digit		[0-9]
number		{digit}+
letter		[a-zA-Z]
identifier	{letter}+
newline		\n
whitespace	[ \t]+

%%
"if"		{ return tkif; }
"then"		{ return tkthen; }
"else"		{ return tkelse; }
"end"		{ return tkend; }
"repeat"	{ return tkrepeat; }
"until"		{ return tkuntil; }
"read"		{ return tkread; }
"write"		{ return tkwrite; }
"+"		{ return tkadd; }
"-"		{ return tksub; }
"*"		{ return tkmul; }
"/"		{ return tkdiv; }
"("		{ return tklparen; }
")"		{ return tkrparen; }
";"		{ return tksemi; }
":="		{ return tkassign; }
"="		{ return tkeq; }
"<"		{ return tklt; }
{number}	{ return tknum; }
{identifier}	{ return tkid; }
{newline}	{ lineno++;  }
{whitespace}	{ /*skip whitespace*/ }
"{"		{
			char c;
			/* skip "{ comment }" */
			while (c = input()) {
			if (c == '\n')
				lineno++;
			else if (c == '}')
				break;
			}
		}
<<EOF>>		{ return tkeof; }
.		{ return tkerr; }

%%

struct token *get_token(void)
{
	static struct token token;
	int type;

	memset(&token, 0x0, sizeof(token));
	switch (type = __yylex()) {
	/* reserved word */
	case tkif:
	case tkthen:
	case tkelse:
	case tkend:
	case tkrepeat:
	case tkuntil:
	case tkread:
	case tkwrite:
	/* special symbol */
	case tkadd:
	case tksub:
	case tkmul:
	case tkdiv:
	case tkassign:
	case tkeq:
	case tklt:
	case tklparen:
	case tkrparen:
	case tksemi:
	/* identifier */
	case tkid:
	/* number */
	case tknum:
		settokentype(&token, type);
		settokenstr(&token, yytext);
		break;
	/* unknown char */
	case tkerr:
		settokenerror(&token);
		break;
	/* end of line */
	case tkeof:
		settokeneof(&token);
		break;
	default:
		/* shoud it happen? */
		fprintf(stderr, "uuuuuu!\n");
		exit(EXIT_FAILURE);
	}
	token.lineno = lineno;
	return &token;
}
