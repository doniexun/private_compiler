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

char nextchar(void)
{
	if (textbufpos > textbufend)
		fillbuf();
	return textbuf[textbufpos++];
}

char backchar(char c)
{
	textbufpos--;
	if (textbuf[textbufpos] != c)
		errexit("text buf corrupts");
}

void inittexthandle(char *sourcefile)
{
	if (!sourcefile)
		errexit("no source file");
	sourcefd = xopen(sourcefile, O_RDONLY);
	/* init text buf */
	memset(textbuf, 0, CHARBUFSIZE);
	textbufpos = 0;
	textbufend = -1;	/* nextchar() will read the source file */
}
