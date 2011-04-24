#ifndef __SCAN_H
#define __SCAN_H

enum tokentype {
	/* book-keeping tokens */
	tkunknown, tkeof, tkerr,
	/* revered words */
	tkif, tkthen, tkelse, tkend,	/* if then else end */
	tkrepeat, tkuntil,		/* repeat until */
	tkread, tkwrite,		/* read write */
	/* special symbol */
	tkeq, tklt,			/* = < */
	tkadd, tksub, tkmul, tkdiv,	/* + - * / */
	tkassign, tksemi,		/* := ; */
	tklparen, tkrparen,		/* ( ) */
	/* multicharacter tokens */
	tknum, tkid
};

#define tokenbufsize 41
#define tokenbufend (tokenbufsize - 1)

struct token {
	enum tokentype type;		/* token type */
	int lineno;			/* line number */
	char pos;			/* current postion of buffer */
	char buf[tokenbufsize];		/* internal token string buf */
};

static inline void settokentype(struct token *token, enum tokentype type)
{
	token->type = type;
}

static inline void settokeneof(struct token *token)
{
	settokentype(token, tkeof);
}

static inline void settokenerror(struct token *token)
{
	settokentype(token, tkerr);
}

static inline void settokenchar(struct token *token, char c)
{
	/* be careful of buf overflow */
	if (token->pos < tokenbufend)
		token->buf[token->pos++] = c;
}

static inline void settokenstr(struct token *token, char *str)
{
	/* be careful of buf overflow */
	while (*str && token->pos < tokenbufend)
		token->buf[token->pos++] = *str++;
}

static inline int isdigit(char c)
{
	return (c >= '0' && c <= '9') ? 1 : 0;
}

static inline int isletter(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) ? 1 : 0;

}

static inline int iswhitespace(char c)
{
	return (c == '\t' || c == '\n' || c == ' ') ? 1 : 0;
}

static inline int issymbol(char c)
{
	switch (c) {
	case '+':
	case '-':
	case '*':
	case '/':
	case ';':
	case ':':
	case '=':
	case '<':
	case '(':
	case ')':
		return 1;
	default:
		return 0;
	}
}

static inline int settokenreservedword(struct token *token)
{
	if (strcmp(token->buf, "if") == 0)
		settokentype(token, tkif);
	else if (strcmp(token->buf, "then"))
		settokentype(token, tkthen);
	else if (strcmp(token->buf, "else"))
		settokentype(token, tkelse);
	else if (strcmp(token->buf, "end"))
		settokentype(token, tkend);
	else if (strcmp(token->buf, "repeat"))
		settokentype(token, tkrepeat);
	else if (strcmp(token->buf, "until"))
		settokentype(token, tkuntil);
	else if (strcmp(token->buf, "read"))
		settokentype(token, tkread);
	else if (strcmp(token->buf, "write"))
		settokentype(token, tkwrite);
}


extern void gettoken(struct token *);
extern void inittoken(struct token *);

#endif	/* scan.h */
