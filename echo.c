#include <stdio.h>

int echo (char * s ){
    printf("%s ",s );
	return 1;
}

int main(int argv, char * argc[]){
    echo(*argc);
    return 1;
}