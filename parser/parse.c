#include <stdio.h>
#include <stdlib.h>
#include "../scanner/scan.h"
#include "parse.h"

/* internal token stream */
static struct token __token;
static struct token __tokenahead;
static int lookahead = 0;
#define tokenahead (lookahead_token())

static inline struct token *__get_token(struct token *token)
{
	inittoken(token);
	gettoken(token);
	return token;
}

static inline struct token *get_token(void)
{
	if (lookahead) {
		lookahead = 0;
		return &__tokenahead;
	}
	return __get_token(&__token);
}

static inline struct token *lookahead_token(void)
{
	if (lookahead)
		return &__tokenahead;
	lookahead = 1;
	return __get_token(&__tokenahead);
}

/* auxiliary method: match and errorhandling  */
void __syntaxerror(void)
{
	struct token *next;
	fprintf(stderr, "syntax error:%d:%s\n", __token.lineno, __token.buf);
	next = get_token();
	fprintf(stderr, "syntax next :%d:%s\n", next->lineno, next->buf);
	next = get_token();
	fprintf(stderr, "syntax next :%d:%s\n", next->lineno, next->buf);
	exit(EXIT_FAILURE);
}
#define DEBUG
#ifdef DEBUG
#define syntaxerror()		\
do {				\
	fprintf(stderr, "\n\n");\
	pdbg(" syntax error");	\
	__syntaxerror();	\
} while (0)
#else
#define syntaxerror() __syntaxerror()

#endif

/* match token and error handling */
void matchtoken(struct token *token, enum tokentype tktype)
{
	/* syntax error handling */
	if (token->type != tktype)
		syntaxerror();
}

/* advance token stream and match token */
void match(enum tokentype tktype)
{
	struct token *token;
	token = get_token();
	matchtoken(token, tktype);
}

/* 
 * main parse method
 *
 * EBNF: stmt-sequence -> statement { ; statment }
 *       statement -> if-stmt | repeat-stmt | assign-stmt
 *                            | read-stmt | write-stmt
 *       if-stmt -> if exp then stmt-sequence [ else stmt-sequence ] end
 *       repeat-stmt -> repeat stmt-sequence until exp
 *       assign-stmt -> identifier := exp
 *       read-stmt -> read identifier
 *       write-stmt -> write exp
 *       exp -> simple-exp [ comparison-op simple-exp ]
 *       simple-exp -> term { addop term }
 *       term -> factor { mulop factor }
 *       factor -> ( exp ) | number | identifier
 */

/* extern */
struct syntaxnode *exp(void);
struct syntaxnode *stmt_sequence(void);
/*
 * EBNF: factor -> ( exp ) | number | identifier
 */
struct syntaxnode *factor(void)
{
	struct syntaxnode *node;
	struct token *token;

	token = get_token();
	switch (token->type) {
	/* We assert token type, so not use match. */
	case tklparen:
		/*
		 * Syntax tree implies expression presedence,
		 * so we ignore parentheses`()` in syntax tree.
		 */
		node = exp();
		match(tkrparen);
		break;
	case tknum:
		node = allocnode(syntaxexp, expconst, token);
		break;
	case tkid:
		node = allocnode(syntaxexp, expid, token);
		break;
	default:
		syntaxerror();
		break;
	}

	return node;
}

/*
 * EBNF: term -> factor { mulop factor }
 */
struct syntaxnode *term(void)
{
	struct syntaxnode *node, *parent;
	struct token *token;
	
	/* fator */
	node = factor();
	parent = NULL;
	/* { mulop factor} */
	while (tokenahead->type == tkmul || tokenahead->type == tkdiv) {
		token = get_token();
		/* assert type is tkmul or tkdiv, so not matchtoken */
		parent = allocnode(syntaxexp, expop, token);
		parent->child[0] = node;
		parent->child[1] = factor();
		/* for next loop: bottom-up */
		node = parent;
		parent = NULL;
	}

	return node;
}

/*
 * EBNF: simple-exp -> term { addop term }
 */
struct syntaxnode *simple_exp(void)
{
	struct syntaxnode *node, *parent;
	struct token *token;
	
	/* term */
	node = term();
	parent = NULL;
	/* { addop term } */
	while (tokenahead->type == tkadd || tokenahead->type == tksub) {
		token = get_token();
		/* assert type is tkadd or tksub, so not matchtoken */
		parent = allocnode(syntaxexp, expop, token);
		parent->child[0] = node;
		parent->child[1] = term();
		/* for next loop: bottom-up */
		node = parent;
		parent = NULL;
	}

	return node;
}

/*
 * EBNF: exp -> simple-exp [ comparison-op simple-exp ]
 */
struct syntaxnode *exp(void)
{
	struct syntaxnode *node, *parent;
	struct token *token;
	
	node = simple_exp();
	switch (tokenahead->type) {
	case tkeq:
	case tklt:
		token = get_token();	
		parent = allocnode(syntaxexp, expop, token);
		parent->child[0] = node;
		parent->child[1] = simple_exp();
		node = parent;
		parent = NULL;
		break;
	default:
		break;
	}
	
	return node;
}

/*
 * EBNF: write-stmt -> write exp
 */
struct syntaxnode *write_stmt(void)
{
	struct syntaxnode *node;
	struct token *token;
	
	match(tkwrite);
	node = allocnode(syntaxstmt, stmtwrite, NULL);
	node->child[0] = exp();
	
	return node;
}

/*
 * EBNF: read-stmt -> read identifier
 */
struct syntaxnode *read_stmt(void)
{
	struct syntaxnode *node;
	struct token *token;
	
	match(tkread);
	token = get_token();
	matchtoken(token, tkid);
	node = allocnode(syntaxstmt, stmtread, token);
	
	return node;
}

/*
 * EBNF: assign-stmt -> identifier := exp
 */
struct syntaxnode *assign_stmt(void)
{
	struct syntaxnode *node;
	struct token *token;

	/* identifier */
	token = get_token();
	matchtoken(token, tkid);
	node = allocnode(syntaxstmt, stmtassign, token);
	/* := */
	match(tkassign);
	/* exp */
	node->child[0] = exp();
	
	return node;
}

/*
 * EBNF: repeat-stmt -> repeat stmt-sequence until exp
 */
struct syntaxnode *repeat_stmt(void)
{
	struct syntaxnode *node;
	struct token *token;
	
	/* parse `repeat stmt-sequence` */
	token = get_token();
	matchtoken(token, tkrepeat);
	node = allocnode(syntaxstmt, stmtrepeat, token);
	node->child[0] = stmt_sequence();
	/* parse `until exp` */
	match(tkuntil);
	node->child[1] = exp();

	return node;
}

/*
 * EBNF: if-stmt -> if exp then stmt-sequence [ else stmt-sequence ] end
 */
struct syntaxnode *if_stmt(void)
{
	struct syntaxnode *node;
	struct token *token;
	
	/* parse `if exp` */
	token = get_token();
	matchtoken(token, tkif);
	node = allocnode(syntaxstmt, stmtif, token);
	node->child[0] = exp();
	/* parse `then stmt-sequence` */
	
	match(tkthen);
	
	node->child[1] = stmt_sequence();
	/* parse `[ else stmt-sequence]` */
	if (tokenahead->type == tkelse) {
		match(tkelse);
		node->child[2] = stmt_sequence();
	}
	/* parse `end` */
	match(tkend);
	
	return node;
}

/*
 * EBNF: statement -> if-stmt | repeat-stmt | assign-stmt
 * 			| read-stmt | write-stmt
 */
struct syntaxnode *statement(void)
{
	struct syntaxnode *node;
	
	switch (tokenahead->type) {
	case tkif:
		node = if_stmt();
		break;
	case tkrepeat:
		node = repeat_stmt();
		break;
	case tkread:
		node = read_stmt();
		break;
	case tkwrite:
		node = write_stmt();
		break;
	/* NOTE: assign-stmt prefix is identifier */
	case tkid:
		node = assign_stmt();
		break;
	default:
		syntaxerror();
		break;
	}
	
	return node;
}

/*
 * EBNF: stmt-sequence -> statement { ; statment }
 */
struct syntaxnode *stmt_sequence(void)
{
	struct syntaxnode *node, *prev;
	
	node = statement();
	/* parse `{ ; statement }` */
	prev = node;
	while (tokenahead->type == tksemi) {
		match(tksemi);
		prev->sibling = statement();
		/* for next loop */
		prev = prev->sibling;
	}
	return node;
}

struct syntaxnode *program(void)
{
	return stmt_sequence();
}

struct syntaxnode *parse(void)
{
	return program();
}
