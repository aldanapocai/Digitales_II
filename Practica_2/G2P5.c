/* 5. Codificar un procedimiento que intercambie dos enteros, por medio de la utilización de
punteros. Verificarlo mediante el llamado del mismo desde un código externo con impresión del
resultado (valores antes y después del intercambio). */

#include <stdio.h>

void swap(int *a, int *b){

 int temp; //Declaracion de una variable temporal. 

 temp = *b; //Asigno a temp el valor de b. 

 *b = *a; // Equivalente a b = a 

 *a = temp;

return;

}

int main(){
 int x = 1, y = 2;

 printf("Valores originales:\tx = %d, y = %d\n", x, y);
 swap(&x, &y);
 printf("Valores nuevos:\t\tx = %d, y = %d\n", x, y);
 
 return 0;
} 