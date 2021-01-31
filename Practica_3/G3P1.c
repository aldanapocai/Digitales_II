/*Crear un programa en C que prenda un solo led del kit (se puede utilizar cualquiera de los
existentes, tanto comunes como RGB) utilizando las funciones XGpio_Initialize (utilizada para
inicializar el GPIO), XGpio_SetDataDirection (utilizada para establecer si el periférico es de entrada
o salida) y XGpio_DiscreteWrite (utilizada para escribir en el periférico). */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xparameters.h"
#include "xgpio.h"

int main()
{
    XGpio push;
    
    init_platform();
    XGpio_Initialize(&push, XPAR_AXI_GPIO_0_DEVICE_ID);
    XGpio_SetDataDirection(&push, 2, 0x00000000);

    while (1)
    XGpio_DiscreteWrite(&push, 2, 0x00000008);
  

    cleanup_platform();
    return 0;
}
