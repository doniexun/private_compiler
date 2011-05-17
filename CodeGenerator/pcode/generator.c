#include <common.h>
#include <parse.h>
#include <symtab.h>

extern void checktype(struct syntaxnode *);

void usage(void)
{
	fprintf(stderr, 
		"P-code generator!\n"
		"Usage: generator sourcefile [> pcodefile]\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct syntaxnode *root;
	if (argc != 2)
		usage();
	/* prepare for token scanning */
	inittexthandle(argv[1]);
	/* syntax parsing */
	root = parse();
	/* sematic analasis */
	printf("\n");
	/* build symbol table */
	buildsymtab(root);
	printf("\n");
	/* type checking */
	checktype(root);
	/* generate pcode file */
	generate(root, NULL);
	return 0;
}
