#include <common.h>

char *stringdup(char *str)
{
        char *p;
        int len;
	if (!str)
		return NULL;
        len = strlen(str);
        p = (char *)xmalloc(len + 1);
        memcpy(p, str, len);
        p[len] = '\0';
        return p;
}

void errexit(char *str)
{
	if (errno)
		perror(str);
	else
		fprintf(stderr, "ERROR %s\n", str ? str : "unknown");
	exit(EXIT_FAILURE);
}

int xopen(const char *path, int mode)
{
	int fd;
	fd = open(path, mode);
	if (fd == -1)
		errexit("open");
	return fd;
}

void *xmalloc(size_t size)
{
	void *p;
	p = malloc(size);
	if (!p)
		errexit("malloc");
	return p;
}

FILE *xfopen(const char *path, const char *mode)
{
	FILE *f;
	f = fopen(path, mode);
	if (!f)
		errexit("fopen");
	return f;
}
