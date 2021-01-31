/*Crear un programa en C que realice un conteo desde 0 a 15 indefinidamente mostrando el
resultado en los leds RGB (se deberá reducir la velocidad de conteo para que se puedan observar
los cambios). Utilizar uno de los switches para habilitar/deshabilitar la cuenta y uno de los botones
para reiniciar la cuenta. */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"


int main()
{
    int i,k,led;
    int v[16]={0x10, 0x20, 0x40, 0x70, 0x80, 0x100, 0x200, 0x380, 0x400, 0x800, 0x1000, 0x1c00, 0x2000, 0x4000, 0x8000, 0xe000};
    int v1[16]={0x10, 0x90, 0x490, 0x2490, 0x20, 0x120, 0x920, 0x4920, 0x40, 0x240, 0x1240, 0x9240, 0x70, 0x3f0, 0x1ff0, 0xfff0};
    XGpio A,B;
    XGpio_Initialize(&A,XPAR_AXI_GPIO_0_DEVICE_ID);
    XGpio_Initialize(&B,XPAR_AXI_GPIO_0_DEVICE_ID);
    XGpio_SetDataDirection(&A,2,0x00000000);
    XGpio_SetDataDirection(&B,1,0xffffffff);

   xil_printf("\r\nMueva el primer switch para desactivar el conteo.\r\nToque el primer boton para reiniciar el conteo.");

   while(1){ 												//Conteo por tiempo indefinido
	   k=0;

	   for(i=0; i<50000;i++)
	   while (k<16){
		if ( XGpio_DiscreteRead(&B, 1) != 0x00000010 ){	

			led=v1[k];
			for(i=0; i<50000;i++) 							// 50000 iteraciones para poder ver los cambios
				XGpio_DiscreteWrite(&A, 2, led);
				k++;
			if (XGpio_DiscreteRead(&B, 1) == 0x000000001){
				k=0;
			}
			
		}
	}
  }

 return 0;
}