/*Tomando de base el programa anterior escribir uno nuevo que prenda y apague un led
indefinidamente. */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

int main()
{
    int i;
    XGpio A;
    XGpio_Initialize(&A,XPAR_AXI_GPIO_0_DEVICE_ID);
    XGpio_SetDataDirection(&A,2,0x00000000); //Out to leds

    while(1){
         for(i=0;i<25000;i++) 			//25000 iteraciones para titileo visible
        XGpio_DiscreteWrite(&A, 2, 0x00000010); //prende luz azul
         for(i=0;i<25000;i++)
        XGpio_DiscreteWrite(&A, 2, 0x00000000); // apagado
      }
      return 0;
}