#include <stdio.h>

int echo (char * s ){
    printf("%s ",s );
	return 1;
}


int main(int argv, char * argc[]){
	int i;
	for (i = 1;i<argv;i++){
		echo(argc[i]);
	}
	printf("\n");
}