/*Crear un programa en C que tome el estado de los switches y lo refleje en los leds (switch = 1
implica led encendido, switch = 0 implica led apagado).*/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

int main(){
	XGpio A;
	XGpio B;
	int psb_check;

	init_platform();
	XGpio_Initialize(&A, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_Initialize(&B, XPAR_AXI_GPIO_0_DEVICE_ID);
	XGpio_SetDataDirection(&A, 1, 0xffffffff); //switches in
	XGpio_SetDataDirection(&B, 2, 0x00000000); //out to leds

	while (1){
	   	psb_check = XGpio_DiscreteRead(&A, 1);
		if (psb_check != 0){ 							// Si algun switch se acciona..
	    		XGpio_DiscreteWrite(&B, 2, 0x0000000f); // Salida por leds 
	    	}else
	    		XGpio_DiscreteWrite(&B, 2, 0x00000000); //necesario para volver al estado inicial 
	}

 cleanup_platform();
 return 0;
}