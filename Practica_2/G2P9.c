/*9. Puntero a una estructura 

Salida de la ejecucion:
// a) Analizar los distintos valores presentes en la misma indicando claramente el significado de
cada uno. 

Valores iniciales de la estructura
 id1: B //char.
 id2: C	 //char.
 id3: Sensible //string.
 Nombre: Pedro //string almacenado usando el puntero nombre.
 Direccion: Av. Carlos Calvo 1234  //string almacenado usando el puntero domicilio.
 Edad: 23 //int.
 Varios: 68 //int.

Direccion de la estructura: 0x0022FEF4 //Direccion dl puntero a la estructura pru_struct. 
Direccion del miembro id1: 0x0022FEF4 (offset: 0 bytes) //
Direccion del miembro id2: 0x0022FEF5 (offset: 1 bytes)
Direccion del miembro id3: 0x0022FEF6 (offset: 2 bytes)
Direccion del miembro nombre: 0x0022FF00 (offset: 12 bytes)
Direccion del miembro domicilio: 0x0022FF04 (offset: 16 bytes)
Direccion del miembro edad: 0x0022FF08 (offset: 20 bytes)
Direccion del miembro varios: 0x0022FF0C (offset: 24 bytes)
Dirección de la primera posición de memoria después de la estructura: 0x0022FF10 

*/

#include <stdio.h>
#include <stddef.h>

struct pru_struct {
 char id1;
 char id2;
 char id3[10];
 char *nombre;
 char *domicilio;
 int edad;
 int varios;
};

void showinfo(struct pru_struct *empl);

void showinfo();

int main(){
	
 int i;
 int tmp;
 struct pru_struct empleados = {
 'B',
 'C',
 "Sensible",
 "Pedro",
 "Av. Carlos Calvo 1234",
 23,
 68,
 };

 showinfo(&empleados);

 return 0;
}

void showinfo(struct pru_struct* empl){
				printf("\nValores iniciales de la estructura\n");
				printf("\tid1:\t\t %c\n", empl->id1 );
				printf("\tid2:\t\t %c\n", empl->id2 );
				printf("\tid3:\t\t %s\n", empl->id3 );
				printf("\tNombre:\t\t %s\n", empl->nombre );
				printf("\tDireccion:\t %s\n", empl->domicilio);
				printf("\tEdad:\t\t %d\n", empl->edad );
				printf("\tVarios:\t\t %d\n", empl->varios );

				printf("\nDireccion de la estructura: 0x%p\n", empl);
				printf("\nDireccion del miembro id1:\t 0x%p (offset: %d bytes)\n", &empl->id1, (int)offsetof(struct pru_struct, id1));
				printf("\nDireccion del miembro id2:\t 0x%p (offset: %d bytes)\n", &empl->id2, (int)offsetof(struct pru_struct, id2));
				printf("\nDireccion del miembro id3:\t 0x%p (offset: %d bytes)\n", &empl->id3, (int)offsetof(struct pru_struct, id3));
				printf("\nDireccion del miembro nombre:\t 0x%p (offset: %d bytes)\n", &empl->nombre, (int)offsetof(struct pru_struct, nombre));
				printf("\nDireccion del miembro domicilio: 0x%p (offset: %d bytes)\n", &empl->domicilio, (int)offsetof(struct pru_struct, domicilio));
				printf("\nDireccion del miembro edad:\t 0x%p (offset: %d bytes)\n", &empl->edad, (int)offsetof(struct pru_struct, edad));
				printf("\nDireccion del miembro varios:\t 0x%p (offset: %d bytes)\n", &empl->varios, (int)offsetof(struct pru_struct, varios));
				printf("\nDireccion de la primera posicion de memoria despues de la estructura: 0x%p\n", ++empl);
}