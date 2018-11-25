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


void show_file_info(char* filename, struct stat* info_p);
void mode_to_letters(int mode, char str[]);
char* uid_to_name(uid_t uid);
char* gid_to_name(gid_t gid);
void do_ls(char dirname[],int mode);
int analyzeParam(char* input);
extern int fonctionls_main(int ac,char* av[]);
