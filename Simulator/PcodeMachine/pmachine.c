#include <common.h>
#include <pcode.h>

extern void exit_pm_stack(void);
extern void init_pm_stack(void);
extern void exit_pm_memory(void);
extern void init_pm_memory(void);
extern struct instruction *gen_instruction(FILE *);
extern void free_instruction(struct instruction *);

FILE *load_file(char *file)
{
	FILE *f;
	f = xfopen(file, "r");
	return f;
}

void exit_machine(struct instruction *flow)
{
	/* free P-machine resources */
	exit_pm_memory();
	exit_pm_stack();
	free_instruction(flow);
}

struct instruction *init_machine(FILE *f)
{
	struct instruction *execflow;
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
	fprintf(stderr, "pmachine pcodefile\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	struct instruction *flow;
	FILE *pcodefile = NULL;
	if (argc != 2)
		usage();
	/* load pcode file */
	pcodefile = load_file(argv[1]);
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
