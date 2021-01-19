//7. Compilar el siguiente código y explicar lo obtenido en la corrida.

#include <stdio.h>

int main(void){
 //Declaracion de variables. 
 int a = 0x12345678;
 short int b = 0xABCD;
 char c = 'a';

//Declaracion y asignacion de punteros.
 
 int * ptr_a = &a;
 short int * ptr_b = &b;
 char * ptr_c = &c;

//Salida en pantalla.

//Direccion de memoria de a.
 printf("\nValor de ptr_a:\t\t %p\n", ptr_a); 

//Direccion de memoria siguiente a a. Como a es un int ocupa 4 bytes, haciendo que la siguiente direccion 
//disponible se encuentre a 4 bytes de ptr_a. 
 printf("Valor de ptr_a + 1:\t %p\n", ++ptr_a); 

//Direccion de memoria de b. 
 printf("\nValor de ptr_b:\t\t %p\n", ptr_b); 
 
//Direccion de memoria siguiente a b. Como b es un short ocupa 2 bytes, ++ptr_b se encuentra a 2 posiciones de ptr_b.
 printf("Valor de ptr_b + 1:\t %p\n", ++ptr_b);    
 
//Direccion de memoria de c. 
 printf("\nValor de ptr_c:\t\t %p\n", ptr_c);  

//Direccion de memoria siguiente a c. Como c es un char ocupa 1 byte, ++ptr_c es consecutivo a ptr_c.
 printf("Valor de ptr_c + 1:\t %p\n", ++ptr_c);   
 
 return 0;
} 