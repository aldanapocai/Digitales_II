//10. Compilar y ejecutar el siguiente código


#include <stdio.h>
int main(void){
	
 // char a;
 // int b = 0x12345678;
 // short int c;
 // printf("\n\nDireccion asignada para la variable a:\t %p\n", &a); //0061FF1F 0x1F 31'd
 // printf("\nDireccion asignada para la variable b:\t %p\n", &b);	  //0060FF18 0x18 24'd
 // printf("\nDireccion asignada para la variable c:\t %p\n", &c);	 //060FF16 0X16 22'd


/*a) Analizar los valores de salida y marcar en el gráfico siguiente (esquema de la memoria) las
ubicaciones asignadas a cada variable (las posiciones de memoria son descendientes de
arriba hacia abajo). 

b) Analizar la asignación de la memoria en el caso en que la declaración de las variables hubiera
sido hecha en el siguiente orden:
 char a;
 short int c;
 int b = 0x12345678;
¿Se logra alguna mejora en la utilización de la memoria? */
//Si, al cambiar el orden de la declaración de variables se aprovechan los bits no utilizados 
//en la declaración de 'char a' por 'short int c'.  

char a;
    short int c;
    int b = 0x12345678;
    printf("\n\nDireccion asignada para la variable a:\t %p\n", &a);    //0061FF1F 0X1F 31'd
    printf("\nDireccion asignada para la variable c:\t %p\n", &c);      //0061FF1C 0X1C 28'd
    printf("\nDireccion asignada para la variable b:\t %p\n", &b);     //0061FF18 0X18 24'd
	
}