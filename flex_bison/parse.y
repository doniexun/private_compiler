%{
/* NOTE:
if_stmt:	IF
		{
			$$ = allocnode(syntaxstmt, stmtif, token);
		}
		exp THEN stmt_seq END
		{
	WRONG: here $$ --> null
			$$->child[0] = $2;
			$$->child[1] = $4;
		}


*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <scan.h>
#include <parse.h>

#define YYSTYPE struct syntaxnode *

extern struct token *get_token(void);
static struct token *token;
static struct token assign_dummy;
static struct syntaxnode *tree;
%}
%token	_EOF	1
%token  IF	3
%token  THEN	4
%token  ELSE	5
%token  END	6
%token  REPEAT	7
%token  UNTIL	8
%token  READ	9
%token  WRITE	10
%token  EQ	11
%token  LT	12
%token  ADD 	13
%token  SUB	14
%token  MUL	15
%token  DIV	16
%token  ASSIGN	17
%token  SEMI	18
%token  LP	19
%token  RP	20
%token  NUM	21
%token  ID	22


%%

program:	_EOF { $$ = NULL; return 0; }
		| stmt_seq _EOF { $$ = tree = $1; return 0; }
;

stmt_seq:	stmt_seq SEMI stmt
		{
			struct syntaxnode *t;
			t = $1;
			if (!t) {
				fprintf(stderr, "error NULL t\n");
				exit(EXIT_FAILURE);
			}
			while (t->sibling)
				t = t->sibling;
			t->sibling = $3;
			$$ = $1;
		}
		| stmt		{ $$ = $1; }
;


stmt:		if_stmt { $$ = $1; }
		| repeat_stmt { $$ = $1; }
		| assign_stmt { $$ = $1; }
		| read_stmt { $$ = $1; }
		| write_stmt { $$ = $1; }
;

if_stmt:	IF exp THEN stmt_seq END
		{
			struct token dummy;
			dummy.type = tkif;
			$$ = allocnode(syntaxstmt, stmtif, &dummy);
			$$->child[0] = $2;
			$$->child[1] = $4;
		}
		| IF exp THEN stmt_seq ELSE stmt_seq END
		{
			struct token dummy;
			dummy.type = tkif;
			$$ = allocnode(syntaxstmt, stmtif, &dummy);
			$$->child[0] = $2;
			$$->child[1] = $4;
			$$->child[2] = $6;
		}
;

repeat_stmt:	REPEAT stmt_seq UNTIL exp
		{
			struct token dummy;
			dummy.type = tkrepeat;
			$$ = allocnode(syntaxstmt, stmtrepeat, &dummy);
			$$->child[0] = $2;
			$$->child[2] = $4;
		}
;

assign_stmt:	ID
		{	/* TODO: minimal copy */
			memcpy(&assign_dummy, token, sizeof(*token));
		}
		ASSIGN exp
		{
			$$ = allocnode(syntaxstmt, stmtassign, &assign_dummy);
			$$->child[0] = $4;
		}
;

read_stmt:	READ ID { $$ = allocnode(syntaxstmt, stmtread, token); }
;

write_stmt:	WRITE exp
		{
			$$ = allocnode(syntaxstmt, stmtwrite, NULL);
			$$->child[0] = $2;
		}
;

exp:		simple_exp { $$ = $1; }
		| simple_exp EQ simple_exp
		{
			struct token dummy;
			dummy.type = tkeq;
			$$ = allocnode(syntaxexp, expop, &dummy);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		| simple_exp LT simple_exp
		{
			struct token dummy;
			dummy.type = tklt;
			$$ = allocnode(syntaxexp, expop, &dummy);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
;

simple_exp:	term { $$ = $1; }
		| simple_exp ADD term
		{
			struct token dummy;
			dummy.type = tkadd;
			$$ = allocnode(syntaxexp, expop, &dummy);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		| simple_exp SUB term
		{
			struct token dummy;
			dummy.type = tksub;
			$$ = allocnode(syntaxexp, expop, &dummy);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
;

term:		factor { $$ = $1; }
		| term MUL factor
		{
			struct token dummy;
			dummy.type = tkmul;
			$$ = allocnode(syntaxexp, expop, &dummy);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
		| term DIV factor
		{
			struct token dummy;
			dummy.type = tkdiv;
			$$ = allocnode(syntaxexp, expop, &dummy);
			$$->child[0] = $1;
			$$->child[1] = $3;
		}
;

factor:		LP exp RP { $$ = $2; }
		| NUM { $$ = allocnode(syntaxexp, expconst, token); }
		| ID { $$ = allocnode(syntaxexp, expid, token); }
;

%%

struct syntaxnode *parse(void)
{
	yyparse();
	return tree;
}

int yylex(void)
{
	token = get_token();
	return token->type;
}

int yyerror(char *s)
{
	fprintf(stderr, "error token[%s]\n", token->buf);
	return 0;
}
