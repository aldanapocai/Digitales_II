 /* Practica 2 Codigo C Ejercicio 2
4. Indicar los valores de x e y (y sus direcciones de memoria) en cada sentencia del siguiente
fragmento de código (mencionar cómo se llega a la obtención de los mismos). 
*/

#include <stdio.h>


int main() {
	
 int x = 1, y = 2; //declaracion de x e y con los valores enteros de 1 y 2 respectivamente. 
 
 int *ptr; //declaracion del puntero de tipo entero.
 
 ptr = &x; //asignacion de la direccion de memoria de x al puntero ptr.
 
 printf("El valor de x es: %d, coincide con %d,  y su direccion en memoria:%p, que coincide con %p. \n", x, *ptr, ptr, &x);
 
 printf("El valor de y es %d y su direccion en memoria: %p.\n", y, &y);
 
 y = *ptr; // y = valor almacenado en la direccion de memoria a la que apunta ptr, osea x = 1. 
 
 printf("Luego de y=*ptr, el valor de y pasa a ser: %d, y la del puntero ptr sigue siendo: %p.\n", y, ptr);
 
 *ptr = 0; //se iguala a 0 el valor almacenado en el puntero, x=0. 
	
 printf("Luego de *ptr=0, el valor de x: %d, el valor de ptr: %d, la direccion de ptr: %p.\n", x, *ptr, ptr);

	return 0;

}