#ifndef __COMMON_H
#define __COMMON_H

/* common head */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

/* xlib.c extern function */
extern int xopen(const char *, int);
extern FILE *xfopen(const char *, const char *);
extern void *xmalloc(size_t);
extern void errexit(char *str);
extern char *stringdup(char *str);

#endif
