/* Practica 2 Codigo C Ejercicio 2
3. Crear una función que realice la suma de dos enteros. Utilizarla para imprimir por pantalla la
suma de dos enteros predefinidos. 
*/

#include <stdio.h>

int suma(int a, int b){
	
	int suma = a+b;
	
	return suma;
}

int main() {
	
	int sum, a=3, b=4;
	
	sum = suma(a,b);
	printf("La suma de %d y %d es: %d\n", a,b,sum);
	


	return 0;

}

