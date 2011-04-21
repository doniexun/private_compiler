#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "text.h"

void errexit(const char *str)
{
	if (!errno)
		fprintf(stderr, "ERROR:%s\n", str);
	else
		perror(str);
	exit(EXIT_FAILURE);
}

int xopen(const char *path, int flags)
{
	int fd;
	fd = open(path, flags);
	if (fd == -1)
		errexit("open");
	return fd;
}
FILE *xfopen(const char *path, const char *mode)
{
	FILE *file;
	file = fopen(path, mode);
	if (!file) {
		errexit("fopen");
		exit(EXIT_FAILURE);
	}
	return file;
}

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

//#define LINE_BUF
#ifdef LINE_BUF
static char linebuf[LINEBUFSIZE];
static char *line;
static int lineno;

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
	/* add '\0' to tail of line */
	*++p = '\0';
	return linebuf;
}

char nextchar(void)
{
	/* end of file */
	if (!line || !*line) {
		line = nextline();
		if (*line)
			printf("%-4d %s", lineno++, line);
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
