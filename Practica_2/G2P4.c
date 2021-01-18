 /* Practica 2 Codigo C Ejercicio 2
4. Indicar los valores de x e y (y sus direcciones de memoria) en cada sentencia del siguiente
fragmento de código (mencionar cómo se llega a la obtención de los mismos). 
*/

#include <stdio.h>


int main() {
	
 int x = 1, y = 2;
 int *ptr;
 ptr = &x;
 y = *ptr;
 *ptr = 0; 
	
	printf("El valor de x es %d y su direccion en memoria: \n", x, *x);

	return 0;

}