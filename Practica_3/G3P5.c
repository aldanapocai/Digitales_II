/*Crear un programa en C que realice las operaciones de suma, resta, multiplicación y división de
dos números enteros especificados en el código. La operación debe ser elegida a través de dos de
los switches del kit y el resultado debe ser mostrado por la terminal.*/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

int main(){

	int a=3;
	int b=2;
	int res=0;
	int aux;
	XGpio SW;
	XGpio_Initialize(&SW,XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&SW,1,0xFFFFFFFF);

	while(1){
		aux=XGpio_DiscreteRead(&SW, 1);

		if( aux == 0x00000010 ) // Suma
			res=a+b;
		if( aux == 0x00000020 ) // Resta
			res=a-b;
		if( aux == 0x00000040 )	// Multiplicacion
			res=a*b;
		if( aux == 0x00000080 )	// Division
			res=a/b;
		
		xil_printf("resultado:%d\r\n",res);
	}
	return 0;
}