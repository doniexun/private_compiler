#ifndef __SCAN_H
#define __SCAN_H

enum tokentype {
	tokenunknown,
	tokeneof,
	tokenerr,
	tokensym,
	tokennum,
	tokenid
};

#define tokenbufsize 41
#define tokenbufend (tokenbufsize - 1)

struct token {
	enum tokentype type;		/* token type */
	char buf[tokenbufsize];		/* internal token string buf */
	char pos;			/* current position of buf */
};

static inline void settokentype(struct token *token, enum tokentype type)
{
	token->type = type;
}

static inline void settokeneof(struct token *token)
{
	settokentype(token, tokeneof);
}

static inline void settokenerror(struct token *token)
{
	settokentype(token, tokenerr);
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

static inline int reservedword(char *str)
{
	if (!strcmp(str, "if") ||
		!strcmp(str, "then") ||
		!strcmp(str, "else") ||
		!strcmp(str, "end") ||
		!strcmp(str, "repeat") ||
		!strcmp(str, "until") ||
		!strcmp(str, "read") ||
		!strcmp(str, "write"))
		return 1;
	return 0;
}

extern void gettoken(struct token *);
extern void inittoken(struct token *);

#endif	/* scan.h */
