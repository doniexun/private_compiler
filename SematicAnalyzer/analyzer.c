#include <common.h>
#include <parse.h>
#include <symtab.h>

extern void checktype(struct syntaxnode *);

void usage(void)
{
	fprintf(stderr, "Usage: analyzer sourcefile\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct syntaxnode *root;
	if (argc != 2)
		usage();
	/* prepare for token scanning */
	inittexthandle(argv[1]);
	root = parse();
	printf("\n");
	buildsymtab(root);
	printf("\n");
	checktype(root);
	return 0;
}
