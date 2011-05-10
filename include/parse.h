#ifndef __PARSE_H
#define __PARSE_H

#include <scan.h>

enum syntaxtype {
	syntaxunknown,
	syntaxstmt,	/* statement */
	syntaxexp	/* expression */
};

/* statement subtype */
enum stmttype {
	stmtunknown,
	stmtif,
	stmtrepeat,
	stmtassign,
	stmtread,
	stmtwrite
};

/* expression subtype */
enum exptype {
	expunknown,
	expop,		/* operation */
	expconst,	/* constant */
	expid		/* identifier */
};

union subtypes {
	enum stmttype stmt;
	enum exptype exp;
};

enum datatype {
	dtvoid,
	dtinteger,
	dtboolean
};

#define MAX_CHILDS 3
/* syntax tree node */
struct syntaxnode {
	enum syntaxtype type;
	union subtypes subtype;
	/* attribution */
	union {
		int val;			/* id: integer value */
		char *name;			/* id: parameter name */
		enum tokentype tktype;		/* token type */
	} attr;
	enum datatype datatype;			/* data structure type */
	int lineno;				/* line number */
	struct syntaxnode *child[MAX_CHILDS];	/* children node */
	struct syntaxnode *sibling;		/* next sibling */
};

extern struct syntaxnode *parse(void);
extern void traverse(struct syntaxnode *, int,
			void (*preproc)(struct syntaxnode *, int),
			void (*postproc)(struct syntaxnode *, int));
extern void nullproc(struct syntaxnode *, int);
extern struct syntaxnode *allocnode(enum syntaxtype, int, struct token *);
#define pdbg(fmt, arg...) fprintf(stderr, "%s "fmt"\n", __FUNCTION__, ##arg)

#endif
