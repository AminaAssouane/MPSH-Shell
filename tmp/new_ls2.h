#ifndef NEW_LS2_H
#define NEW_LS2_H

#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>

#define LS_NONE 0
#define LS_L 101
#define LS_R 102
#define LS_D 103
#define LS_I 104

#define LS_A 200

#define LS_AL (LS_A+LS_L)
#define LS_AI (LS_A+LS_I)

extern int main_ls(int argc, char* argv[]);

#endif
