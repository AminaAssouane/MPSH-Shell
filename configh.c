#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void){
	char chemin[512];
	memset(chemin, 0, sizeof(chemin));
	getcwd(chemin, sizeof(chemin));

	char str[] = " export PATH=$PATH:";
	strcat(str,chemin);
	str[strlen(str)] = 0;
    	system(str);
    	return 0;
}
