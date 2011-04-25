#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "parse.h"
#include "../scanner/scan.h"

char *stringdup(char *str)
{
	char *p;
	int len;
	len = strlen(str);
	p = (char *)malloc(len + 1);
	if (!p) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	memcpy(p, str, len);
	p[len] = '\0';
	return p;	
}

struct syntaxnode *allocnode(enum syntaxtype type,
				int subtype,
				struct token *token)
{
	struct syntaxnode *node;
	node = (struct syntaxnode *)malloc(sizeof(*node));
	if (!node) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	node->type = type;
	if (type == syntaxstmt) {
		node->subtype.stmt = subtype;
		switch (subtype) {
		case stmtif:
		case stmtrepeat:
			node->attr.tktype = token->type;
			break;
		case stmtassign:
		case stmtread:
			node->attr.name = stringdup(token->buf);
			break;
		case stmtwrite:
			/* write needs nothing */
			break;
		default:
			/* should this happen? */
			node->subtype.stmt = stmtunknown;
			break;
		}
	} else if (type == syntaxexp) {
		node->subtype.exp = subtype;
		switch (subtype) {
		case expop:
			node->attr.tktype = token->type;
			break;
		case expconst:
			node->attr.val = atoi(token->buf);
			break;
		case expid:
			node->attr.name = stringdup(token->buf);
			break;
		default:
			/* should this happen? */
			node->subtype.exp = expunknown;
			break;
		}
	} else {
		/* stmtunknown == expunknown */
		node->subtype.stmt = stmtunknown;
	}
	return node;
}