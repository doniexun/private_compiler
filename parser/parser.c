#include <common.h>
#include <parse.h>
#include <text.h>

char *nodeop(struct syntaxnode *node)
{
	switch (node->attr.tktype) {
	case tkeq:
		return "=";
	case tklt:
		return "<";
	case tkadd:
		return "+";
	case tksub:
		return "-";
	case tkmul:
		return "*";
	case tkdiv:
		return "/";
	case tkmod:
		return "%";
	default:
		return "[unknown op]";
	}
}

void __printsyntaxnode(struct syntaxnode *node, int depth)
{
	if (!node)
		errexit("NULL node");
	/* fill space */
	for (; depth; depth--)
		printf("    ");
	if (node->type == syntaxstmt) {
		printf("stmt:");
		switch (node->subtype.stmt) {
		case stmtif:
			printf("if");
			break;
		case stmtrepeat:
			printf("repeat");
			break;
		case stmtassign:
			printf("assign id: %s", node->attr.name);
			break;
		case stmtread:
			printf("read id: %s", node->attr.name);
			break;
		case stmtwrite:
			printf("write");
			break;
		default:
			printf("unknown");
			break;
		}
	} else if (node->type == syntaxexp) {
		printf("exp:");
		switch (node->subtype.exp) {
		case expop:
			printf("op: %s", nodeop(node));
			break;
		case expconst:
			printf("const: %d", node->attr.val);
			break;
		case expid:
			printf("id: %s", node->attr.name);
			break;
		default:
			printf("unknown");
			break;
		}
	} else {
		printf("Unknown");
	}
	printf("\n");
}

void printsyntaxnode(struct syntaxnode *node, int depth)
{
	int i;
	/* traverse the sibling */
	while (node) {
		/* itself */
		__printsyntaxnode(node, depth);
		/* child */
		for (i = 0; node->child[i]; i++)
			printsyntaxnode(node->child[i], depth + 1);
		node = node->sibling;
	}
}

void printsyntaxtree(struct syntaxnode *root)
{
#ifdef BISON_PARSER
	printf("bison syntax tree\n");
#endif
#ifdef GENERAL_TRAVERSE
	traverse(root, 0, __printsyntaxnode, nullproc);
#else
	printsyntaxnode(root, 0);
#endif
}

#ifdef GENERAL_TRAVERSE
void __freesyntaxnode(struct syntaxnode *node, int dummydepth)
#else
void __freesyntaxnode(struct syntaxnode *node)
#endif
{
	if ((node->type == syntaxstmt && node->subtype.stmt == stmtread)
		|| (node->type == syntaxexp && node->subtype.exp == expid))
		free(node->attr.name);
	free(node);
}

void freesyntaxnode(struct syntaxnode *node)
{
	struct syntaxnode *tmp;
	int i;
	/* traverse the sibling */
	while (node) {
		/* child */
		for (i = 0; node->child[i]; i++)
			freesyntaxnode(node->child[i]);
		/* itself */
#ifdef GENERAL_TRAVERSE
		/* for successful compiling */
		__freesyntaxnode(node, 0);
#else
		__freesyntaxnode(node);
#endif
		node = node->sibling;
	}
}

void freesyntaxtree(struct syntaxnode *root)
{
#ifdef GENERAL_TRAVERSE
	traverse(root, 0, nullproc, __freesyntaxnode);
#else
	freesyntaxnode(root);
#endif
}

void usage(void)
{
	fprintf(stderr, "Usage: parser sourcefile\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct syntaxnode *root;
#ifndef BISON_PARSER
	if (argc != 2)
		usage();
	/* init text handling of source file */
	inittexthandle(argv[1]);
#endif
	/* real parse */
	root = parse();
	/* traverse syntax tree */
	printsyntaxtree(root);
#ifndef BISON_PARSER
	/* free the syntax tree */
	freesyntaxtree(root);
#endif
	return 0;
}
