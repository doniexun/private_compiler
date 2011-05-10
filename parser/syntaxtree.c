#include <common.h>
#include <parse.h>
#include <scan.h>
#include <string.h>
/* auxiliary dummy method for traverse parameter */
void nullproc(struct syntaxnode *node, int depth)
{
	/* do nothing */
}

/* general syntax node traversing method */
void traverse(struct syntaxnode *node, int depth,
			void (*preproc)(struct syntaxnode *, int),
			void (*postproc)(struct syntaxnode *, int))
{
	int i;
	while (node) {
		/* itself: pre */
		preproc(node, depth);
		/* children */
		for (i = 0; node->child[i]; i++)
			traverse(node->child[i], depth + 1,
						preproc, postproc);
		/* itself: post */
		postproc(node, depth);
		/* next sibling */
		node = node->sibling;
	}
}

char *stringdup(char *str)
{
	char *p;
	int len;
	len = strlen(str);
	p = (char *)xmalloc(len + 1);
	memcpy(p, str, len);
	p[len] = '\0';
	return p;
}

struct syntaxnode *allocnode(enum syntaxtype type,
				int subtype,
				struct token *token)
{
	struct syntaxnode *node;
	node = (struct syntaxnode *)xmalloc(sizeof(*node));
	memset(node, 0x0, sizeof(*node));
	node->type = type;
	node->datatype = dtvoid;
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
