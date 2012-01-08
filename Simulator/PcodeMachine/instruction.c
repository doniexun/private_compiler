#include <common.h>
#include <pcode.h>

extern unsigned int memindex;
extern unsigned int *memory;

struct sym *getsym(struct sym *head, char *name)
{
	for (; head; head = head->next) {
		if (strcmp(head->name, name) == 0)
			return head;
	}
	return NULL;
}

struct sym *allocsym(int memidx, char *name)
{
	struct sym *symbol;
	symbol = xmalloc(sizeof(*symbol));
	symbol->next = NULL;
	symbol->name = stringdup(name);
	symbol->memidx = memidx;
	return symbol;
}

void insertsym(struct sym *head, struct sym *new)
{
	new->next = head->next;
	head->next = new;
}

/* transite parameter into associated memory address(memory index) */
unsigned int symtrans(char *operand)
{
	static struct sym head = { 0, NULL, NULL };
	struct sym *symbol;
	/* if existed symbol, return memory index directly */
	if ((symbol = getsym(head.next, operand)) == NULL) {
		/* nonexist symbol, insert it into symbol table */
		symbol = allocsym(memindex, operand);
		insertsym(&head, symbol);
		/* for next new parameter */
		memindex++;
	}
	return symbol->memidx;
}

struct instruction *alloc_instruction(char *opcode, char *operand)
{
	static int insno = 0;
	struct instruction *ins;

	ins = xmalloc(sizeof(*ins));
	ins->no = insno++;
	ins->operand = 0;	/* not set here */
	ins->next = NULL;
	ins->operandstr = stringdup(operand);
	/* dummy opcode */
	if (!opcode) {
		ins->opcode = OPCODE_NOP;
		ins->operand = 0;
		/* unified with other opcodes*/
		ins->opcodestr = stringdup("nop");
		return ins;
	}

	ins->opcodestr = stringdup(opcode);
	if (strcmp(opcode, "lab") == 0)
		ins->opcode = OPCODE_LAB;
	else if (strcmp(opcode, "fjp") == 0)
		ins->opcode = OPCODE_FJP;
	else if (strcmp(opcode, "ujp") == 0)
		ins->opcode = OPCODE_UJP;
	else if (strcmp(opcode, "adi") == 0)
		ins->opcode = OPCODE_ADI;
	else if (strcmp(opcode, "sbi") == 0)
		ins->opcode = OPCODE_SBI;
	else if (strcmp(opcode, "mpi") == 0)
		ins->opcode = OPCODE_MPI;
	else if (strcmp(opcode, "dvi") == 0)
		ins->opcode = OPCODE_DVI;
	else if (strcmp(opcode, "mod") == 0)
		ins->opcode = OPCODE_MOD;
	else if (strcmp(opcode, "equ") == 0)
		ins->opcode = OPCODE_EQU;
	else if (strcmp(opcode, "neq") == 0)
		ins->opcode = OPCODE_NEQ;
	else if (strcmp(opcode, "grt") == 0)
		ins->opcode = OPCODE_GRT;
	else if (strcmp(opcode, "lt") == 0)
		ins->opcode = OPCODE_LT;
	else if (strcmp(opcode, "stp") == 0)
		ins->opcode = OPCODE_STP;
	else if (strcmp(opcode, "lod") == 0)
		ins->opcode = OPCODE_LOD;
	else if (strcmp(opcode, "lda") == 0)
		ins->opcode = OPCODE_LDA;
	else if (strcmp(opcode, "ldc") == 0)
		ins->opcode = OPCODE_LDC;
	else if (strcmp(opcode, "sto") == 0)
		ins->opcode = OPCODE_STO;
	else if (strcmp(opcode, "rdi") == 0)
		ins->opcode = OPCODE_RDI;
	else if (strcmp(opcode, "wri") == 0)
		ins->opcode = OPCODE_WRI;
	else
		ins->opcode = OPCODE_NOP;
	return ins;
}

struct instruction *alloc_dummy_instruction(void)
{
	return alloc_instruction(NULL, NULL);
}

/*
 * @table: label table, logging label address
 * @set:   label set, containing instruction
 * 		waiting for label-to-address transition
 */
void labtrans(struct label *table, struct labelset *labset)
{
	struct label *lab;
	struct labelset *set;
	struct labelsubset *subset;
	/* skip dummy head */
	lab = table->next;
	/* FIXME: checking table and set association */
	while (lab) {
		/* get label set */
		for (set = labset->next; set; set = set->next)
			if (strcmp(set->name ,lab->name) == 0)
				break;
		if (set) {
			for (subset = set->subset;	/* skip dummy head */
					subset;
					subset = subset->next) {
				/* real transition */
				subset->addr->operand = (unsigned int)lab->addr;
			}
		} else {
			fprintf(stderr, "unused lable: %s\n", lab->name);
		}
		lab = lab->next;
	}

}

void labfree(struct label *table, struct labelset *labset)
{
	struct label *lab;
	struct labelset *set;
	struct labelsubset *subset, *next;
	/* free label table */
	/* skip dummy head */
	table = table->next;
	while (table) {
		lab = table->next;
		if (table->name)
			free(table->name);
		free(table);
		table = lab;
	}

	/* free label set */
	/* skip dummy head */
	labset = labset->next;
	while (labset) {
		set = labset->next;
		/* free label subset */
		subset = labset->subset;
		while (subset) {
			next = subset->next;
			free(subset);
			subset = next;
		}
		/* free label set */
		if (labset->name)
			free(labset->name);
		free(labset);
		labset = set;
	}
}

struct label *alloclabel(char *operand, struct instruction *addr, int labno)
{
	struct label *lab;
	lab = xmalloc(sizeof(*lab));
	lab->addr = addr;
	lab->next = NULL;
	lab->name = stringdup(operand);
	lab->labno = labno;
	return lab;
}

void insertlabel(struct label *head, struct label *new)
{
	new->next = head->next;
	head->next = new;
}

void insertlabtbl(struct label *labtbl, char *operand, struct instruction *addr)
{
	static int labno = 0;
	struct label *new;
	/* reduplication checking */
	for (new = labtbl->next; new; new = new->next) {
		if (new->addr == addr) {
			fprintf(stderr, "reduplicated label:%s definition\n",
				operand);
			exit(EXIT_FAILURE);
		}
	}
	/* alloc and init it */
	new = alloclabel(operand, addr, labno);
	insertlabel(labtbl, new);
	/* for next new label */
	labno++;
}

struct labelsubset *alloclabelsubset(struct instruction *addr)
{
	struct labelsubset *subset;
	subset = xmalloc(sizeof(*subset));
	subset->addr = addr;
	subset->next = NULL;
	return subset;
}

void insertlabelsubset(struct labelset *set, struct labelsubset *subset)
{
	subset->next = set->subset;
	set->subset = subset;
}

struct labelset *alloclabelset(char *name)
{
	struct labelset *set;
	set = xmalloc(sizeof(*set));
	set->name = stringdup(name);
	set->next = NULL;
	return set;
}

void insertlabelset(struct labelset *labset, struct labelset *new)
{
	new->next = labset->next;
	labset->next = new;
}

void insertlabset(struct labelset *labset, char *operand, struct instruction *addr)
{
	struct labelset *set;
	struct labelsubset *subset;
	for (set = labset->next; set; set = set->next)
		if (strcmp(set->name, operand) == 0)
			break;
	if (!set) {
		set = alloclabelset(operand);
		insertlabelset(labset, set);
	}
	subset = alloclabelsubset(addr);
	insertlabelsubset(set, subset);
}

void skip_whitespace(char **p)
{
	char *str = *p;
	while (*str == ' ' || *str == '\t')
		str++;
	*p = str;
}

void skip_op(char **p)
{
	char *str = *p;
	while (isalpha(*str) || isdigit(*str))
		str++;
	*p = str;
}

/* exclusive function, which can be used once per time */
int getcode(FILE *f, char **opcode, char **operand)
{
	static char line[256];
	char *p, *opc = NULL, *opr = NULL;

	while (p = fgets(line, 256, f)) {
		skip_whitespace(&p);
		/* comment line? empty line? */
		if (*p != ';' && *p != '\n' && *p != '\0')
			break;
	}

	if (p) {
		/* get opcode */
		opc = p;
		/* get operand */
		skip_op(&p);
		if (*p != '\0') {
			/* make opcode null-terminated */
			*p++ = '\0';
			skip_whitespace(&p);
			if (*p != ';' && *p != '\n' && *p != '\0') {
				opr = p;
				skip_op(&p);
				/* make operand null-terminated */
				*p = '\0';
			}
		}
		/* set return value */
		if (opcode)
			*opcode = opc;
		if (operand)
			*operand = opr;
		return 0;
	} else {
		return -1;
	}
}

void debug_instruction(struct instruction *ins)
{
	fprintf(stderr, " [%d] addr: %#8x\n"
			"      %8s %8s\n"
			"      %8d %#8x\n",
		ins->no, ins, ins->opcodestr, ins->operandstr ? : "",
		ins->opcode, ins->operand);
}

void debug_flow(struct instruction *head)
{
	fprintf(stderr, ".-----------[debug instruciton]---------.\n");
	while (head) {
		debug_instruction(head);
		head = head->next;
	}
	fprintf(stderr, "'-----------[end debug        ]---------'\n");
}

/* only one traversal of pcode file */
struct instruction *gen_instruction(FILE *f)
{
	struct label labtbl;
	struct labelset labset;
	struct instruction *current, *prev, *head;
	char *opcode, *operand;

	/* init first one(dummy) instruction */
	head = prev = alloc_dummy_instruction();
	current = NULL;
	/* init label table head node */
	labtbl.labno = 0;
	labtbl.addr = NULL;
	labtbl.next = NULL;
	/* init label set head node */
	labset.subset = NULL;
	labset.next = NULL;
	labset.name = NULL;

	/* traversal of pcode file */
	while (getcode(f, &opcode, &operand) == 0) {
		current = alloc_instruction(opcode, operand);
		/* special opcode parsing */
		switch (current->opcode) {
		case OPCODE_LAB:
			/* FIXME: duplicate label definition checking */
			insertlabtbl(&labtbl, operand, current);
			break;
		case OPCODE_FJP:
		case OPCODE_UJP:
			insertlabset(&labset, operand, current);
			break;
		case OPCODE_LOD:
		case OPCODE_LDA:
			current->operand = symtrans(operand);
			break;
		case OPCODE_LDC:
			current->operand = atoi(operand);
			break;
		default:
			/* donot handle others */
			break;
		}
		/* debug */
		if (debug)
			debug_instruction(current);
		/* for next loop */
		prev->next = current;
		prev = current;
		current = NULL;
	}
	/* label transsition */
	labtrans(&labtbl, &labset);
	/* free labels */
	labfree(&labtbl, &labset);
	if (debug)
		debug_flow(head);
	printf("[init] instructs have been loaded\n");
	return head;
}

void free_instruction(struct instruction *flow)
{
	struct instruction *next;

	while (flow) {
		next = flow->next;
		free(flow);
		flow = next;
	}
}
