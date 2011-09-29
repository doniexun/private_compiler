#include <common.h>
#include <pcode.h>

extern void exit_pm_stack(void);
extern void init_pm_stack(void);
extern void exit_pm_memory(void);
extern void init_pm_memory(void);
extern struct instruction *gen_instruction(FILE *);
extern void free_instruction(struct instruction *);

char *sourcefile;
int debug;

FILE *load_file(char *file)
{
	FILE *f;
	f = xfopen(file, "r");
	return f;
}

void exit_machine(struct instruction *flow)
{
	printf("\nP-machine: exiting...\n");
	/* free P-machine resources */
	exit_pm_memory();
	exit_pm_stack();
	free_instruction(flow);
}

struct instruction *init_machine(FILE *f)
{
	struct instruction *execflow;
	printf("\nP-machine: initing...\n");
	/* generate machine codes: executable instruction */
	execflow = gen_instruction(f);
	/* init stack */
	init_pm_stack();
	/* init memory */
	init_pm_memory();
	return execflow;
}

void usage(void)
{
	fprintf(stderr,
		"pmachine [OPTIONS] pcodefile\n"
		"OPTIONS:\n"
		"   -h    display help information\n"
		"   -d    display debug information\n"
	);
	exit(EXIT_FAILURE);
}

void parse_args(int argc, char **argv)
{
	int c;
	sourcefile = NULL;
	debug = 0;
	opterr = 0;
	while ((c = getopt(argc, argv, "dh")) != -1) {
		switch (c) {
		case 'd':
			debug = 1;
			break;
		case 'h':
		default:
			usage();
			break;
		}
	}
	if (optind != argc - 1)
		usage();
	sourcefile = argv[optind];
}

int main(int argc, char **argv)
{
	struct instruction *flow;
	FILE *pcodefile = NULL;
	parse_args(argc, argv);
	/* load pcode file */
	pcodefile = load_file(sourcefile);
	/* init P-machine runtime environment */
	flow = init_machine(pcodefile);
	/* run P-machine */
	run_machine(flow);
	/* free P-machine resources */
	exit_machine(flow);
	/* close pcode file */
	fclose(pcodefile);
	return 0;
}
