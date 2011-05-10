#include <common.h>
#include <parse.h>
#include <list.h>
#include <symtab.h>

static struct list_head symtab;
static int location;

struct symbol *allocsymbol(char *name, int location, int lineno)
{
	struct symbol *sym;
	struct linenr *ln;
	sym = xmalloc(sizeof(*sym));
	sym->name = stringdup(name);
	sym->location = location;
	list_init(&sym->symlist);
	list_init(&sym->linelist);
	/* init line number */
	ln = xmalloc(sizeof(*ln));
	list_init(&ln->list);
	ln->lineno = lineno;
	/* insert line number entry into symbol */
	list_add(&ln->list, &sym->linelist);
	return sym;
}

void printsymtab(struct list_head *st_head)
{
	struct symbol *sym;
	struct linenr *line;
	printf("name     location    line nr\n");
	printf("--------|--------|------------------\n");
	list_for_each_entry(st_head, sym, symlist) {
		printf("%-8s|%-8d|", sym->name, sym->location);
		/* line number */
		list_for_each_entry(&sym->linelist, line, list)
			printf("%-4d", line->lineno);
		printf("\n");
	}
}

struct symbol *st_lookup(struct list_head *head, char *name)
{
	struct symbol *sym;
	list_for_each_entry(head, sym, symlist)
		if (strcmp(name, sym->name) == 0)
			return sym;
	return NULL;
}

void st_reinsert(struct symbol *sym, int lineno)
{
	struct linenr *ln;
	ln = xmalloc(sizeof(*ln));
	ln->lineno = lineno;
	list_add_tail(&ln->list, &sym->linelist);
}

void st_insert(char *name, int lineno, int location)
{
	struct symbol *sym;
	sym = allocsymbol(name, location, lineno);
	list_add_tail(&sym->symlist, &symtab);
}

void insertsymtab(char *name, int lineno)
{
	struct symbol *sym;
	sym = st_lookup(&symtab, name);
	if (sym)
		st_reinsert(sym, lineno);
	else
		st_insert(name, lineno, location++);
}

/* assert node != NULL */
void insertnode(struct syntaxnode *node, int depth)
{
	if (node->type == syntaxstmt) {
		switch (node->subtype.stmt) {
		case stmtassign:
		case stmtread:
			insertsymtab(node->attr.name, node->lineno);
			break;
		default:
			break;
		}
	} else if (node->type == syntaxexp) {
		switch (node->subtype.exp) {
		case expid:
			insertsymtab(node->attr.name, node->lineno);
			break;
		default:
			break;
		}
	}
}

void buildsymtab(struct syntaxnode *root)
{
	/* init */
	location = 0;
	list_init(&symtab);
	/* build */
	traverse(root, 0, insertnode, nullproc);
	/* debug */
	printsymtab(&symtab);
}
