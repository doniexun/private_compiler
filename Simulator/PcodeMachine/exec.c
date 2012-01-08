#include <common.h>
#include <pcode.h>

extern unsigned int *memory;

#define opdbg(fmt, args...)\
	do {\
		if (debug)\
			fprintf(stderr, fmt, ##args);\
	} while (0)

/* a, b should be signed integer */
unsigned int pm_op(unsigned int op, int a, int b)
{
	unsigned int ret;
	switch (op) {
	case OPCODE_ADI:
		ret = a + b;
		break;
	case OPCODE_SBI:
		ret = a - b;
		break;
	case OPCODE_MPI:
		ret = a * b;
		break;
	case OPCODE_DVI:
		ret = a / b;
		break;
	case OPCODE_MOD:
		ret = a % b;
		break;
	case OPCODE_EQU:
		ret = (a == b);
		break;
	case OPCODE_NEQ:
		ret = (a != b);
		break;
	case OPCODE_GRT:
		ret = (a > b);
		break;
	case OPCODE_LT:
		ret = (a < b);
		break;
	default:
		/* should this happen? */
		fprintf(stderr, "P-machine recognization is wrong\n"
				"Urecognize calculating operation\n");
		break;
	}
	return ret;
}

void run_machine(struct instruction *flow)
{
	struct instruction *ins;
	unsigned int stop, top, toptop;
	printf("\nP-machine: running...\n");
	stop = 0;
	ins = flow;
	while (ins && !stop) {
		opdbg("OPCODE %s", ins->opcodestr);
		if (ins->operandstr)
			opdbg(" %s", ins->operandstr);
		opdbg("\n");

		switch (ins->opcode) {
		case OPCODE_NOP:
		case OPCODE_LAB:
			/* do nothing in current instruction */
			ins = ins->next;
			break;
		case OPCODE_FJP:
			top = pm_pop();
			if (top == FALSE)
				ins = (struct instruction *)ins->operand;
			else
				ins = ins->next;
			break;
		case OPCODE_UJP:
			ins = (struct instruction *)ins->operand;
			break;
		/* calculating operation */
		case OPCODE_ADI:
		case OPCODE_SBI:
		case OPCODE_MPI:
		case OPCODE_DVI:
		case OPCODE_MOD:
		case OPCODE_EQU:
		case OPCODE_NEQ:
		case OPCODE_GRT:
		case OPCODE_LT:
			top = pm_pop();
			toptop = pm_pop();
			pm_push(pm_op(ins->opcode, (int)toptop ,(int)top));
			ins = ins->next;
			break;
		case OPCODE_STP:
			stop = 1;
			break;
		case OPCODE_LOD:
			pm_push(memory[ins->operand]);
			ins = ins->next;
			break;
		case OPCODE_LDA:
			/* ins->operand means memory index */
			pm_push(ins->operand);
			ins = ins->next;
			break;
		case OPCODE_LDC:
			pm_push(ins->operand);
			ins = ins->next;
			break;
		case OPCODE_STO:
			top = pm_pop();
			toptop = pm_pop();
			memory[toptop] = top;
			ins = ins->next;
			break;
		case OPCODE_RDI:
			/* get input from stdin */
			printf("P-machine read: ");
			if (scanf("%d", &toptop) != 1) {
				fprintf(stderr, "read error\n"
						"using default value: 1");
				toptop = 1;
			}
			top = pm_pop();
			memory[top] = toptop;
			printf("\n");
			ins = ins->next;
			break;
		case OPCODE_WRI:
			/* output to stdout */
			top = pm_pop();
			printf("P-machine write: %d\n", top);
			ins = ins->next;
			break;
		default:
			fprintf(stderr, "unrecognized opcode: %d\n", ins->opcode);
			stop = 1;
			ins = NULL;
			break;
		}
	}
	if (!ins) {
		fprintf(stderr, "instruction flow disappears\n");
	} else {
		fprintf(stderr, "P-code runs out\n");
	}
}
