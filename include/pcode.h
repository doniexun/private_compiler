#ifndef __PCODE_H
#define __PCODE_H

struct instruction {
	unsigned int no;		/* instruction number */
	unsigned int opcode;
	unsigned int operand;
	char *opcodestr;
	char *operandstr;
	struct instruction *next;
};

struct label {
	unsigned int labno;
	char *name;
	struct instruction *addr;
	struct label *next;
};

struct labelsubset {
	struct instruction *addr;
	struct labelsubset *next;
};

struct labelset {
	char *name;
	struct labelsubset *subset;	/* instructions waiting for transition*/
	struct labelset *next;
};

struct sym {
	unsigned int memidx;
	char *name;
	struct sym *next;
};

enum opcodes {
	OPCODE_NOP,	/* 0 */
	OPCODE_LAB,
	OPCODE_FJP,
	OPCODE_UJP,
	OPCODE_ADI,
	OPCODE_SBI,	/* 5 */
	OPCODE_MPI,
	OPCODE_DVI,
	OPCODE_EQU,
	OPCODE_NEQ,
	OPCODE_GRT,	/* 10 */
	OPCODE_LT,
	OPCODE_STP,
	OPCODE_LOD,
	OPCODE_LDA,
	OPCODE_LDC,	/* 15 */
	OPCODE_STO,
	OPCODE_RDI,
	OPCODE_WRI
};

#define FALSE 0
#define TRUE 1

#endif	/* pcode.h */