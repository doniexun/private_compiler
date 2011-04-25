#ifndef __COMMON_H
#define __COMMON_H

/* common head */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

/* xlib.c extern function */
extern int xfopen(const char *, int);
extern void *xmalloc(size_t);

#endif
