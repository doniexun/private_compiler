#include <common.h>
#include <parse.h>

static char *typestr(enum datatype type)
{
	char *ret;
	switch (type) {
	case dtvoid:
		ret = "void";
		break;
	case dtboolean:
		ret = "boolean";
		break;
	case dtinteger:
		ret = "integer";
		break;
	default:
		ret = "unknown";
		break;
	}
	return ret;
}

/* if error, we continu run, not exiting */
static int __typecheck(struct syntaxnode *node, enum datatype right)
{
	if (node->datatype != right) {
		fprintf(stderr,
			"[type error]at line %d: type %s should be %s\n",
					node->lineno,
					typestr(node->datatype),
					typestr(right));
		return -1;
	}
	return 0;
}

/*
 * type checking traverse internal auxiliary method
 * traverse order: postorder(bottom-up)
 * 	first: type assingment
 * 	next : type checking
 */
void typecheck(struct syntaxnode *node, int depth)
{
	if (node->type == syntaxstmt) {
                switch (node->subtype.stmt) {
                case stmtif:
			/* if _exp_ then-part [else-part] */
			__typecheck(node->child[0], dtboolean);
                        break;
                case stmtrepeat:
			/* repeat body _exp_ */
			__typecheck(node->child[1], dtboolean);
                        break;
                case stmtassign:
			/* id := exp */
			__typecheck(node->child[0], dtinteger);
			node->datatype = node->child[0]->datatype;
                        break;
                case stmtread:
			/* first assignment? */
			if (node->datatype == dtvoid)
				node->datatype = dtinteger;
                        break;
                case stmtwrite:
			__typecheck(node->child[0], dtinteger);
                        break;
                default:
                        break;
		}
	} else if (node->type == syntaxexp) {
		switch (node->subtype.exp) {
                case expop:
			__typecheck(node->child[0], dtinteger);
			__typecheck(node->child[1], dtinteger);
			switch (node->attr.tktype) {
			case tkadd:
			case tksub:
			case tkmul:
			case tkdiv:
			case tkmod:
				node->datatype = dtinteger;
				break;
			case tkeq:
			case tkneq:
			case tklt:
				node->datatype = dtboolean;
				break;
			default:
				break;
			}
                        break;
                case expconst:
			node->datatype = dtinteger;
                        break;
                case expid:
			/* FIXME: how to detect whether the id has been assigned
			we must assign id before referring it!
			if (__typecheck(node, dtinteger) == -1)
				fprintf(stderr,
				"[type error]not assigned identifier:%s\n",
							node->attr.name);
			*/
			node->datatype = dtinteger;
                        break;
                default:
                        printf("unknown");
                        break;
		}
	}

}

void checktype(struct syntaxnode *root)
{
	traverse(root, 0, nullproc, typecheck);
}
