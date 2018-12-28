#include "mpsh.h"


int main(){

	char username[80];
	char currentPath[80];
	getcwd(currentPath,sizeof(currentPath));

	char filename[100] = "/home/";
	struct passwd * pwd = getpwuid(getuid());
	strcpy(username, pwd->pw_name);

	const char * bash_name = "/.bash_profile";
	strcat(filename,username);
	strcat(filename,bash_name);

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
