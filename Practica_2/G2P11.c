/* 11. Implementar por código una multiplicación con enteros de 8 bits (emulando lo realizado en la
práctica de Assembly). */ 

#include <stdio.h>

int main(){
	int x = 3;
	int y = 5;
	int sum=0;
	int i = x;
	
	
	
	while(i!=0){
		sum+=y;
		i--;
	}
	
	printf("La multiplicacion de x=%d e y=%d es igual a: %d.\n", x, y , sum);
	
	return 0;
}