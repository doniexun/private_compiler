#ifndef __TEXT_H
#define __TEXT_H

#define CHARBUFSIZE 4096
#define LINEBUFSIZE 256

extern char nextchar(void);
extern void backchar(char);
extern void inittexthandle(char *);
extern int getlineno(void);

#define dbg(fmt, arg...) fprintf(stderr, "%s "fmt"\n", __FUNCTION__, ##arg)

#endif
