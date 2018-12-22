#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>

#define OP_NONE 0
#define OP_L 101
#define OP_R 102
#define OP_D 103
#define OP_I 104

#define OP_A 200

#define OP_AL (OP_A+OP_L)
#define OP_AI (OP_A+OP_I)


void fichier_info(char* filename, struct stat* info_p);
void tran_mode_chrac(int mode, char str[]);
char* uid_to_name(uid_t uid);
char* gid_to_name(gid_t gid);
void do_ls(char dirname[],int mode);
int analyzeParam(char* input);
extern int fonctionls_main(int ac,char* av[]);
