/* Practica 2 Codigo C Ejercicio 2
2. Crear un vector de enteros de 10 posiciones, recorrerlo e imprimir por pantalla cada uno de sus
valores. ¿Qué tamaño ocupa en memoria? 
*/


#include <stdio.h>

int main() {
	
	int vector[10];
	printf("El vector ocupa %d bytes en memoria, 10 datos tipo int de 4 bytes cada uno. \n", sizeof(vector));
	for (int i=0; i<sizeof(vector)/4; i++){
	printf("En la posicion %d se encuentra el valor: %d.\n", i, vector[i]);
	}
	return 0;

}