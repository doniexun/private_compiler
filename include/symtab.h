#ifndef __SYMTAB_H
#define __SYMTAB_H

#include <parse.h>
#include <list.h>

struct linenr {
	int lineno;
	struct list_head list;
};

struct symbol {
	char *name;
	int location;
	struct list_head linelist;
	struct list_head symlist;
};

extern void buildsymtab(struct syntaxnode *);

#endif	/* symtab.h */
