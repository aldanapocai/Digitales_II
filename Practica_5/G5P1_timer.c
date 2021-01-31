#include <stdio.h>
#include "xil_exception.h"

//Defino puerto interrupciones segun arquitectura
#define BUTTONS_AND_LEDS_INTNUM	0
#define PMODS_INTNUM            1
#define SHIELDS_INTNUM          2
#define UART_INTNUM             3
#define TIMER_INTNUM            4

//***************** Declaracion de estructuras ****************************************//
struct intc {
	int isr;             // Interrupt Status Register (ISR)
	int ipr;             // Interrupt Pending Register (IPR)
	int ier;             // Interrupt Enable Register (IER)
	int iar;             // Interrupt Acknowledge Register (IAR)
	int sie;             // Set Interrupt Enables (SIE)
	int cie;             // Clear Interrupt Enables (CIE)
	int ivr;             // Interrupt Vector Register (IVR)
	int mer;             // Master Enable Register (MER)
	int imr;             // Interrupt Mode Register (IMR)
	int ilr;             // Interrupt Level Register (ILR)
	int reserved1[54];
	int ivar[32];        // Interrupt Vector Address Register (IVAR)
	int reserved2[32];
	int ivear[32];       // Interrupt Vector Extended Address Register (IVEAR)
};

struct gpio {
	int gpio_data;
	int gpio_tri;
	int gpio2_data;
	int gpio2_tri;
	int reserved1[67];
	int gier;
	int ip_isr;
	int reserved2[1];
	int ip_ier;
};
struct timer{
	int TCSR0;
	int TLR0;
	int TCR0;
	int RSVD0[3];
	int TCSR1;
	int TLR1;
	int TCR1;
	int RSVD1[3];
};

//Vectores para recorrer
volatile struct timer *const tim = (struct timer *) 0x41C00000;
volatile struct intc *const intp_ctlr = (struct intc *) 0x41200000;
volatile int numero=0;

//Controlador de interrupcion
void InterruptHandler(void *CallbackRef)
{
	int i;
	xil_printf("interrupcion %d\n\r",numero++);
	/* Clear the Interrupt */
	for (i=0;i<99999;i++)
	tim->TCSR0|=(1<<8);
	intp_ctlr->iar = (1 << (TIMER_INTNUM));
}

//***************** Funcion principal ************************************************************************************//

int main(void){

	intp_ctlr->mer = 0; 				//Deshabilita todas las interrupciones. 
	intp_ctlr->ier = 0;					//Deshabilita que las interrupciones pasen al procesador. 
	intp_ctlr->iar = 0xffffffff;		//Contesta los posibles llamados a interrupciones, al setear ISR=0. 
	intp_ctlr->imr = 0;					// Procesa las interrupciones en modo normal y no en modo rapido. 

	intp_ctlr->ier  &= ~(1 << (TIMER_INTNUM)); // disable interrupt
	intp_ctlr->ier |= (1 << (TIMER_INTNUM)); // enable interrupt
	intp_ctlr->mer |= (0x2 | 0x01);			// Setea en 1 al Hardware Interrupt Enable (HIE) bit, 
											//habilitando las interrupciones por hardware.

	tim->TLR0=0x0000ffff;
	//tim->TCR0=0x00000000;
	tim->TCSR0|=(1<<5);	// cargo el valor del TLR
	tim->TCSR0=((1<<6)|(1<<4)|(1<<1));
	tim->TCSR0&=~(1<<5);//saco la traba
	tim->TCSR0|=(1<<7);

	xil_printf(Inicio");

	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) InterruptHandler, (void *) 0); //Llamada al controlador de interrupciones
	Xil_ExceptionEnable();

	for(;;);
}
