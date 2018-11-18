#include <stdio.h>

int echo (char * s ){
    printf("%s ",s );
	return 1;
}

int main(int argv, char * argc[]){
	int i=1;
	for (i;i<argv;i++){
		echo(argc[i]);
	}
	printf("\n");

}