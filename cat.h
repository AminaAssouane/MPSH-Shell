#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
extern int cat(char *path);
extern int nblines(FILE *f);
extern int cat_n(int argc, char* argv[]);
