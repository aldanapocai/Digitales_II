/* 6. Se tiene la siguiente declaración:
 int x[5];
 int *ptr;
*/

#include <stdio.h>


int main(){
	
	int x[5] = {19, 10, 8, 17, 9};
	int *ptr;

//a) ¿Cómo haría para que el puntero ptr apunte a la primera posición del vector x?

	ptr = &x[1];

//b) ¿Recorrer el vector completo utilizando incrementos en el puntero ptr?
//c) Realizar un printf del puntero ptr para cada incremento del punto b. Indicar por qué entre
//valor y valor existe un salto en la secuencia.	

//El salto se da ya que como el puntero fue declarado como int, ocupa 4 bytes de memoria. 

	ptr = &x[0]; //Inicializo el puntero en la primera posicion del vector x. 
	
	for(int i=0; i<sizeof(x)/4; i++){
		printf("Puntero en posicion %d: %p\n", i, ptr+i);
		printf("x[%d] = %d\n", i, *(ptr+i));
	}

//d) Si en lugar de tener un vector de enteros (int) en el ejemplo utilizáramos un vector de chars,
//el salto entre valores consecutivos del punto c cambiaría? Corroborarlo. */

//Cambiaria a saltos de 1 byte.

	char y[5], *ptr_b;
	
	ptr_b = &y[0];
	
	for(int i=0; i<sizeof(y); i++){
		printf("Puntero en posicion %d: %p\n", i, ptr_b+i);
		printf("y[%d] = %d\n", i, *(ptr_b+i));
		
	}
	
	
	return 0;
}