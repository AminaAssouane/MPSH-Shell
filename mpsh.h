/*
 *------------Université Paris Diderot--------------------
 * Filename: mpsh.h
 * Version: 1.0 Beta
 * Created: 16h32m 02/11/18
 * Author: Equipe 33 (Yuchen BAI)
 *---------------------------------------------------------
 */

#ifndef MPSH_H
#define MPSH_H

#include "mkdir.h"
#include "cat.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <stdio.h>
#include <pwd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "fonctionls.h"


#define TRUE 1
#define SHELL_BUFFER  64
#define SIZE 512
//extern char alias_path1[128];
//extern char alias_path2[128];

//extern char* path_alias_txt(char* filename);

#endif  //fin de fichier "mpsh.h"
