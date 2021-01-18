/* Practica 2 Codigo C */

#include<stdio.h>

int main() {
	
	int intType;
    float floatType;
    double doubleType;
    char charType;
	
	printf("Tamanios de distintos tipos de datos.\n");
	printf("------------------------------------\n");
	printf("Un int tiene %d bytes.\n", sizeof(intType));
	printf("Un float tiene %d bytes.\n", sizeof(floatType));	
	printf("Un double tiene %d bytes.\n", sizeof(doubleType));	
	printf("Un char tiene %d bytes.\n", sizeof(charType));	
	
	return 0;
	
	
}

