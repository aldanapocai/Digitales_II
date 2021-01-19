//10. Compilar y ejecutar el siguiente código


#include <stdio.h>
int main(void){
 char a;
 int b = 0x12345678;
 short int c;
 printf("\n\nDireccion asignada para la variable a:\t %p\n", &a);
 printf("\nDireccion asignada para la variable b:\t %p\n", &b);
 printf("\nDireccion asignada para la variable c:\t %p\n", &c);
}

/*a) Analizar los valores de salida y marcar en el gráfico siguiente (esquema de la memoria) las
ubicaciones asignadas a cada variable (las posiciones de memoria son descendientes de
arriba hacia abajo). 

b) Analizar la asignación de la memoria en el caso en que la declaración de las variables hubiera
sido hecha en el siguiente orden:
 char a;
 short int c;
 int b = 0x12345678;
¿Se logra alguna mejora en la utilización de la memoria? */