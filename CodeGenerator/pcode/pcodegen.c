#include <common.h>
#include <parse.h>

extern void pcodegen(struct syntaxnode *);

/* default is standard output */
static FILE *fout = NULL;

/*
 * P-code Set:
 *
 * lod load value of parameter into @top
 * lda load address of parameter into @top
 * ldc load constant into @top
 * sto store @top to address of second and pop
 *
 * adi pop the remaining two values ,add them ,and push the result
 * sbi pop the remaining two values ,sub them ,and push the result
 * mpi pop the remaining two values ,mul them ,and push the result
 * dvi pop the remaining two values ,div them ,and push the result
 * equ ==
 * neq !=
 * grt >  great
 * lt  <  litte
 *
 * ujp unconditional jump
 * fjp jump when top of stack is false
 * lab set label
 *
 * stp program stop
 *
 * (@top: top of stack in P-machine)
 */

/* create temp label */
char *getlabel(void)
{
	static char __label[32] = {"Label "};
	static int labno = 0;
	char *ret;
	labno++;
	sprintf(&__label[5], "%d", labno);
	ret = stringdup(__label);
	return ret;
}

/* generate comment output: `; ....` */
void comment(const char *cmt)
{
	fprintf(fout, "; %s\n", cmt);
}

/* generate instruction associated with immediate */
void pcodeimm(const char *op, int immediate, const char *cmt)
{
	fprintf(fout, "\t%5s", op);
	fprintf(fout, "%8d", immediate);
	/* comment? */
	if (cmt) {
		fprintf(fout, "\t\t");
		comment(cmt);
	} else {
		fprintf(fout, "\n");
	}
}

/* generate instruction associated with string */
void pcodestr(const char *op, const char *str, const char *cmt)
{
	/* emphasize label */
	if (strcmp(op, "lab") == 0)
		comment(str);
	fprintf(fout, "\t%5s", op);
	fprintf(fout, "%8s", str);
	/* comment? */
	if (cmt) {
		fprintf(fout, "\t\t");
		comment(cmt);
	} else {
		fprintf(fout, "\n");
	}
}

/* generate expression operation pcode */
void pcodeexpop(struct syntaxnode *node)
{
	pcodegen(node->child[0]);
	pcodegen(node->child[1]);
	switch (node->attr.tktype) {
	case tkeq:
		pcodestr("equ", "", "=");
		break;
	case tklt:
		/* litte: < */
		pcodestr("lt", "", "<");
		break;
	case tkadd:
		pcodestr("adi", "", NULL);
		break;
	case tksub:
		pcodestr("sbi", "", NULL);
		break;
	case tkmul:
		pcodestr("mpi", "", NULL);
		break;
	case tkdiv:
		pcodestr("dvi", "", NULL);
		break;
	default:
		comment("unrecognized exp operation");
		break;
	}
}

/* generate if-control-statement pcode */
void pcodeif(struct syntaxnode *node)
{
	char *lab1 = NULL, *lab2 = NULL;
	/* comment promote */
	comment("if (exp) ... else ... end");
	lab1 = getlabel();
	/* exp */
	pcodegen(node->child[0]);
	/* jump end when exp is false*/
	pcodestr("fjp", lab1, "jump if false");
	/* if-part */
	pcodegen(node->child[1]);
	/* else-part */
	if (node->child[2]) {
		lab2 = getlabel();
		pcodestr("ujp", lab2, "end if-part");
		pcodestr("lab", lab1, "else-part");
		pcodegen(node->child[2]);
		pcodestr("lab", lab2, "end if");
		/* free label */
		free(lab2);
	} else {
		pcodestr("lab", lab1, "end if");
	}
	/* free label */
	free(lab1);
	/* comment promote */
	comment("end if");
}

/* generate repeat-control-statement pcode */
void pcoderepeat(struct syntaxnode *node)
{
	char *lab = NULL;
	/* comment promote */
	comment("repeat ... until exp");
	lab = getlabel();
	pcodestr("lab", lab, "repeat start");
	/* body */
	pcodegen(node->child[0]);
	/* exp */
	pcodegen(node->child[1]);
	pcodestr("fjp", lab, "repeat if false");
	/* free label */
	free(lab);
	/* comment promote */
	comment("end repeat");
}

/* real trasition from syntax node to P-code instruction */
void nodegen(struct syntaxnode *node)
{
	char *lab1 = NULL, *lab2 = NULL;
	if (!node)
		return;
	switch (node->type) {
	case syntaxstmt:
		switch (node->subtype.stmt) {
		case stmtif:
			pcodeif(node);
			break;
		case stmtrepeat:
			pcoderepeat(node);
			break;
		case stmtassign:
			/* load id address */
			pcodestr("lda", node->attr.name, "assign start");
			/* exp */
			pcodegen(node->child[0]);
			/* `id =` */
			pcodestr("sto", "", "assign end");
			break;
		case stmtread:
			pcodestr("lda", node->attr.name,
				"id address of read instruction");
			pcodestr("rdi", "", "read");
			break;
		case stmtwrite:
			/* load written value:exp */
			pcodegen(node->child[0]);
			/* exp */
			pcodestr("wri", "", "write");
			break;
		default:
			comment("unrecognized statement opcode");
			break;
		}
		break;
	case syntaxexp:
		switch (node->subtype.exp) {
		case expop:
			pcodeexpop(node);
			break;
		case expconst:
			pcodeimm("ldc", node->attr.val, NULL);
			break;
		case expid:
			pcodestr("lod", node->attr.name, NULL);
			break;
		default:
			comment("unrecognized expression opcode");
			break;
		}
		break;
	defalut:
		comment("unrecognized opcode");
		break;
	}
}

/* 
 * preoder traversal of node tree
 * traversing node's sibling, not children
 */
void pcodegen(struct syntaxnode *node)
{
	while (node) {
		nodegen(node);
		node = node->sibling;
		/*
		 * not handle children of node
		 * nodegen will do it for us
		 */
	}
}

/*
 * generate p-code via traversal of syntax tree
 */
void generate(struct syntaxnode *root, FILE *out)
{
	if (!root) {
		fprintf(stderr, "NULL syntax tree");
		return;
	}
	if (!out)
		fout = stdout;
	comment("program start");
	pcodegen(root);
	/* program stop */
	pcodestr("stp", "", "program stop");
}
