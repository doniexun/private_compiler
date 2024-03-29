#include <common.h>
#include <text.h>

/* only one source file at one time */
static int sourcefd;
static char textbuf[CHARBUFSIZE];
static int textbufpos;
static int textbufend;

static void fillbuf(void)
{
	int n;
	memset(textbuf, 0, CHARBUFSIZE);
	n = read(sourcefd, textbuf, CHARBUFSIZE);
	if (n == -1)
		errexit("read source file");
	textbufpos = 0;
	textbufend = n - 1;
}

#define LINE_BUF
#ifdef LINE_BUF

static char linebuf[LINEBUFSIZE];
static char *line;
static int lineno;

int getlineno(void)
{
	return lineno;
}

static char *nextline(void)
{
	char *p = linebuf - 1;
	do {
		if (++p > &linebuf[LINEBUFSIZE - 2])
			errexit("line buffer overflows");
		if (textbufpos > textbufend)
			fillbuf();
		*p = textbuf[textbufpos++];
	} while (*p != '\0' && *p != '\n');
	/* update line number */
	if (*p)
		lineno++;
	/* add '\0' to tail of line */
	*++p = '\0';
	return linebuf;
}

char nextchar(void)
{
	/* end of file */
	if (!line || !*line) {
		line = nextline();
#define OUTPUT_LINE
#ifdef OUTPUT_LINE
		if (*line)
			printf("%-4d %s", lineno, line);
#endif
	}
	return *line++;
}

void backchar(char c)
{
	line--;
	if (*line != c)
		errexit("text buf corrupts");
}

#else	/* LINE_BUF end */
char nextchar(void)
{
	if (textbufpos > textbufend)
		fillbuf();
	return textbuf[textbufpos++];
}

void backchar(char c)
{
	textbufpos--;
	if (textbuf[textbufpos] != c)
		errexit("text buf corrupts");
}

#endif	/* !LINE_BUF end */

void inittexthandle(char *sourcefile)
{
	if (!sourcefile)
		errexit("no source file");
	sourcefd = xopen(sourcefile, O_RDONLY);
	/* init text buf */
	memset(textbuf, 0, CHARBUFSIZE);
	textbufpos = 0;
	textbufend = -1;	/* nextchar() will read the source file */
#ifdef LINE_BUF
	/* init line buf */
	memset(linebuf, 0, LINEBUFSIZE);
	line = NULL;
	lineno = 0;
#endif
}
