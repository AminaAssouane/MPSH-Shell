#ifndef FONC_LS_H
#define FONC_LS_H

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>
#include <linux/limits.h>
#include <grp.h>
#include <sys/types.h>

#define PARM_VIDE 0
#define PARM_A 1 //-a
#define PARM_L 2 //-l
#define PARM_R 4 //-r
#define MAXROWLEN 100
void ls_R(int, char **);
int recursion(int, char *);
int g_leave_len;    //一行剩余长度，用于输出对齐
int g_maxlen;   //存放某目录下最长的文件名的长度
extern int ls_main_fonction(int argc, char** argv); 
#endif
