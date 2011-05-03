#include <common.h>

void errexit(char *str)
{
	if (errno)
		perror(str);
	else
		fprintf(stderr, "ERROR %s\n", str ? str : "unknown");
}

int *xopen(const char *path, int mode)
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
