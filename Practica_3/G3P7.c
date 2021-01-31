/*Crear un programa en C que reciba bytes (caracteres presionados en el teclado) a través de una
conexión serie (UART) y los muestre a través de la terminal y de los leds.*/

#include <stdio.h>


int main(){

	int *RX= (int*) 0x40600000;
	int *TX= (int*) 0x40600004;
	int *status = (int*) 0x40600008;

	while(1){
		if(*status & 0x00000001){
			if((*TX & 0x00000008) == 0)
				*TX=*RX;
			}
		*(int*) 0x40000008 = *RX;
	}

return 0;
}
