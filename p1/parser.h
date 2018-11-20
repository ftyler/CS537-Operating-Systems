#include <stdio.h>

extern char state_info;
extern char utime[BUFSIZ];
extern char stime[BUFSIZ];
extern char vmem[BUFSIZ];
extern char cmd[BUFSIZ];

extern void parse_files(char*);
//extern void parse_files(char*, int, int, int, int, int);
