#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
extern void cat(char *path);
extern int nblines(FILE *f);
extern void cat_n(int argc, char* argv[]);
