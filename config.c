#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


int main(){

	char currentPath[80];
	getcwd(currentPath,sizeof(currentPath));


	const char *filename = "/home/yuchen/.bash_profile";
	int p = open(filename,O_WRONLY | O_APPEND,0766);
	
	if(p!=-1){
		printf("Perfect ! \n");
	}

	char res[100] = "export PATH=\"";
	strcat(res,currentPath);
	char *tail = ":$PATH\"";
	strcat(res,tail);

	write(p,res,strlen(res));
	close(p);

	execlp("source","source","~/.bash_profile",NULL);
	return 0;
}
