/*Crear un programa en C que tome el estado de los switches y lo muestre por una terminal. En este
caso se deberá agregar el uso de la función XGpio_DiscreteRead (utilizada para leer el estado de
un periférico de entrada).*/

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

int main()
{   
  XGpio push;
    int psb_check;
    
    init_platform();

    XGpio_Initialize(&push, XPAR_AXI_GPIO_0_DEVICE_ID);
    XGpio_SetDataDirection(&push, 1, 0xffffffff); //In switches
    
    while (1){
      psb_check = XGpio_DiscreteRead(&push, 1); //Leo switches
      xil_printf("Push Buttons Status %x\r\n", psb_check); //Muestro por terminal
    }
    cleanup_platform();
    return 0;
}
