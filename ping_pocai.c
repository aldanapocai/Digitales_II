/*                                                                            */
/* Author(s):                                                                 */
/*   Miguel Angel Sagreras                                                    */
/*                                                                            */
/* Copyright (C) 2020                                                         */
/*    Miguel Angel Sagreras                                                   */
/*                                                                            */
/* This source file may be used and distributed without restriction provided  */
/* that this copyright statement is not removed from the file and that any    */
/* derivative work contains  the original copyright notice and the associated */
/* disclaimer.                                                                */
/*                                                                            */
/* This source file is free software; you can redistribute it and/or modify   */
/* it under the terms of the GNU General Public License as published by the   */
/* Free Software Foundation, either version 3 of the License, or (at your     */
/* option) any later version.                                                 */
/*                                                                            */
/* This source is distributed in the hope that it will be useful, but WITHOUT */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or      */
/* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for   */
/* more details at http://www.gnu.org/licenses/.                              */
/*                                                                            */

////Objetivo general del archivo: al enviarle un ping a todos los dispositivos conectados, mediante ethernet, en este caso solo a la placa
//// la misma envia una respuesta con su direccion MAC, necesaria para luego poder generar la conexion. 



#include "xil_exception.h"
#include <stdio.h>
#include <string.h>

#define MY_MAC		{0x00, 0x40, 0x00, 0x01, 0x02, 0x03}
#define MY_IPV4		{192, 168, 0, 14}

#define SERVER_LOG  0x03
#define STATE_LOG   0x04
#define DMA_LOG     0x08
#define ETH_LOG     0x10
#define INTC_LOG    0x20
#define INTH_LOG    0x40

const int loglevel = SERVER_LOG; // STATE_LOG; // | ETH_LOG; // | INTH_LOG; 

/* ********************* */
/* MicroBlaze enviroment */
/* ********************* */

/* Interrupt controller register map */
/* --------------------------------- */

#define INTNUM_BUTTONS_AND_LEDS 0
#define INTNUM_PMODS            1
#define INTNUM_UART             2
#define INTNUM_DMA              3
#define INTNUM_ETHERNET         4
#define INTNUM_TIMER            5

#define IE_MASK	((1 << INTNUM_ETHERNET) | ((1 << INTNUM_DMA))) //// Mascara de habilitacion de las interrupciones de Ethernet y DMA. 

////Para las direcciones de memoria absolutas detalladas a continuacion se utilizo la documentacion pg099-axi-intc, pagina 15.////

struct intc {
	int isr;             // Interrupt Status Register (ISR) //// Direccion absoluta: 0x41200000. 
	int ipr;             // Interrupt Pending Register (IPR) //// Direccion absoluta: 0x41200004. 
	int ier;             // Interrupt Enable Register (IER) //// Direccion absoluta: 0x41200008. 
	int iar;             // Interrupt Acknowledge Register (IAR) //// Direccion absoluta: 0x4120000C. 
	int sie;             // Set Interrupt Enables (SIE) //// Direccion absoluta: 0x41200010. 
	int cie;             // Clear Interrupt Enables (CIE) //// Direccion absoluta: 0x41200014. 
	int ivr;             // Interrupt Vector Register (IVR) //// Direccion absoluta: 0x41200018.
	int mer;             // Master Enable Register (MER) //// Direccion absoluta: 0x4120001C. 
	int imr;             // Interrupt Mode Register (IMR) //// Direccion absoluta: 0x41200020.
	int ilr;             // Interrupt Level Register (ILR) //// Direccion absoluta: 0x41200024.
	int reserved1[54];
	int ivar[32];        // Interrupt Vector Address Register (IVAR) //// Direcciones absolutas: 0x41200100 - 0x4120017C.
	int reserved2[32];
	int ivear[32];       // Interrupt Vector Extended Address Register (IVEAR) //// Direcciones absolutas: 0x41200200 - 0x412002FC.
};

/* CDMA controller register map */
/* ---------------------------- */


//// Mascaras utilizadas en Controller Register y Status Register del DMA. 

#define DMA_CRBIT_IRQEN		12 //// Para utilizar bit de interrupcion (en la documentacion = IOC_IrqEn).
#define DMA_CRBIT_IRQERR	14 //// Para utilizar bit de interrupcion por errores en la transferencia (documentacion = Err_IrqEn).

#define DMA_CRMSK_IRQEN		(1 << DMA_CRBIT_IRQEN)
#define DMA_CRMSK_IRQERR	(1 << DMA_CRBIT_IRQERR) 

#define DMA_SRBIT_IDLE		1 //// Se utilizara para verificar si el IDLE del Status Register esta habilitado. 
#define DMA_SRMSK_IDLE		(1 << DMA_SRBIT_IDLE)


//// Mapa de registros del DMA (- Direct Memory Access- redirecciona la informacion recibida por ethernet a la ram de forma simultanea).//// 

////Para las direcciones de memoria absolutas detalladas a continuacion se utilizo la documentacion: pg034-axi-cdma, pagina 13.////

struct dma {
	int cdmacr;            // CDMA Control //// Direccion absoluta: 0x44a00000
	int cdmasr;            // CDMA Control //// Direccion absoluta: 0x44a00004
	int curdesc_pntr;      // Current Descriptor Pointer //// Direccion absoluta: 0x44a00008
	int curdesc_pntr_msb;  // Current Descriptor Pointer MSB if address space is greater than 32bits
	int taildesc_pntr;     // Tail description Pointer //// Direccion absoluta: 0x44a00010
	int taildesc_pntr_msb; // Tail description Pointer MSB if address space is greater than 32bits
	int sa;                // Source Address //// Direccion absoluta: 0x44a00018
	int sa_msb;            // Source Address MSB if address space is greater than 32bits
	int da;                // Source Address  //// Direccion absoluta: 0x44a00020 Destination Address. 
	int da_msb;            // Source Address MSB if address space is greater than 32bits ////Destination Address.
	int btt;               // Byte to transfer //// Cantidad de bits a transmitir. Direccion absoluta: 0x44a00028
};

/* GPIO controller register map */
/* ---------------------------- */

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

/* Ethenert Lite controller register map */
/* ------------------------------------- */
////Para las direcciones de memoria absolutas detalladas a continuacion se utilizo la documentacion: pg135-xi-ethernetlite, pagina 17.////
struct ethernet {
	int tx_ping_data[505];	 	
	int mdioaddr;			//// Direccion absoluta: 0x40e007E4.
	int mdiowr;				//// Direccion absoluta: 0x40e007E8.
	int mdiord;				//// Direccion absoluta: 0x40e007EC.
	int mdioctrl;			//// Direccion absoluta: 0x40e007F0.

	int tx_ping_length;	   // 0x07f4 Transmit length register for ping buffer	//// Direccion absoluta: 0x40e007F4 (max 1500 bytes). 
	int gie;               // 0x07f8 Global interrupt register ////habilita las interrupciones. //// Direccion absoluta: 0x40e007F8.
	int tx_ping_control;   // 0x07fc Transmit length register for ping buffer 	//// Direccion absoluta: 0x40e007FC.
	int tx_pong_data[509];
	int tx_pong_length;	   // 0x0ff4 Transmit length register for pong buffer	//// Direccion absoluta: 0x40e00FF4.
	int reserved3[1];
	int tx_pong_control;   // 0x0ffc Transmit length register for pong buffer  //// Direccion absoluta: 0x40e00FFC.
	int rx_ping_data[511];
	int rx_ping_control;   // 0x17fc Receive  length register for ping buffer  //// Direccion absoluta: 0x40e017FC.
	int rx_pong_data[511];
	int rx_pong_control;   // 0x1ffc Receive  length register for pong buffer  //// Direccion absoluta: 0x40e01FFC.
};

/* Devices' memory map */
/* ------------------- */

//// Para las direcciones absolutas de este archivo me baso en las direcciones a la que apuntan los siguientes punteros,
//// provenientes del mapa de memoria.
volatile struct gpio     *const inputs_and_outputs = (struct gpio *)     0x40000000; 
volatile struct intc     *const intp_ctlr          = (struct intc *)     0x41200000;
volatile struct dma      *const dma_ctlr           = (struct dma *)      0x44a00000;
volatile struct ethernet *const eth_ctlr           = (struct ethernet *) 0x40e00000;

/* **************************************** */
/* Header requirement for Internet Protocol */
/* **************************************** */

/* Ethernet Frame Header */
/* --------------------- */

struct eth_hdr {
	char unsigned dest[6]; 		//// Direccion destino. 
	char unsigned src[6];		//// Direccion fuente. 
	char unsigned type[2];
};

#define ETHTYP_IP	0x800
#define ETHTYP_ARP	0x806 		//// Protocolo para conocer direccion MAC de cierta direccion IP. 

/* Arp Frame Header for ipv4 */
/* ------------------------- */

struct arp4_hdr {
	char unsigned htype[2];	// Hardware type
	char unsigned ptype[2];	// Protocol type
	char unsigned hlen[1];	// Hardware address length
	char unsigned plen[1];	// Protocol address length
	char unsigned open[2];	// Operation
	char unsigned sha[6];	// Sender hardware address
	char unsigned spa[4];	// Sender protocol address
	char unsigned tha[6];	// Target hardware address
	char unsigned tpa[4];	// Target protocol address
};

/* Arp Over ethernet frame */
/* ----------------------- */

struct arp_frm {
	struct eth_hdr  eth_hdr;
	struct arp4_hdr arp4_hdr;
};

/* Internet Header version 4 */
/* ------------------------- */

struct ipv4_hdr {
	char unsigned verihl[1];	// Version and Header Length
	char unsigned tos[1];		// Type Of Service
	char unsigned len[2];		// Total Length
	char unsigned ident[2];		// Identification
	char unsigned flgsfrg[2];	// Flags and fragment
	char unsigned ttl[1];		// Time To Live
	char unsigned proto[1];		// Protocol
	char unsigned chksum[2];	// Header Checksum
	char unsigned sa[4];		// Source Address
	char unsigned da[4];		// Destination Address
};

const unsigned icmp_proto = 0x01;

/* ICMP Header version */
/* ------------------- */

struct icmp_hdr {
	char unsigned type[1];		// ICMP Type
	char unsigned code[1];		// ICMP Code
	char unsigned chksum[2];	
};

/* ICMP over Ethernet frame */
/* ------------------------ */

struct icmp_frm {
	struct eth_hdr  eth_hdr;
	struct ipv4_hdr ipv4_hdr;
	struct icmp_hdr icmp_hdr;
};

const char unsigned my_mac[]  = MY_MAC;
const char unsigned my_ipv4[] = MY_IPV4;

void print_mac(const char unsigned *mac)
{
	int i;

	for (i = 0; i < 5; i++)
		xil_printf("%02x:", mac[i]);
	xil_printf("%02x", mac[i]);
}

void print_ipv4(const char unsigned *ipv4)
{
	int i;

	for (i = 0; i < 3; i++)
		xil_printf("%d.", ipv4[i]);
	xil_printf("%d", ipv4[i]);
}


const struct arp_frm my_arp = {
	{ 
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},	// Destination MAC    //// Tipo Broadcast ( a todos conectados).
		MY_MAC,									// Source MAC 		  //// Origen del paquete. 
		{0x08, 0x06} 							// Arp protocol
	},
	{ 
		{0x00, 0x01}, 
		{0x08, 0x00}, 
		{0x06},
		{0x04}, 
		{0x00, 0x02},
		MY_MAC,
		MY_IPV4,
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
		MY_IPV4
	}
};

#define RX_PKT	0
#define TX_PKT	1

int eth_state;

void ethctlr_handler ()			////Documentación usada: AXI pg135-axi-ethernetlite
{
	if (eth_ctlr->rx_ping_control & (1<< 0)) {		//// Si Receive control register (0x40e017FC) for ping buffer indica que 
													////hay un nuevo paquete en el ping buffer para recibir. Pag 21. 
		if ((loglevel & INTH_LOG) && (loglevel & ETH_LOG)) xil_printf("recevied packet\r\n");
		eth_state |= (1 << RX_PKT);
		/* Clear the Interrupt */
		eth_ctlr->rx_ping_control &= ~(1<< 0);		//// Limpia el registro Receive control register (0x40e017FC).
	}
	if (eth_ctlr->tx_ping_control & (1<< 3)) { 		//// Si se transmitio un paquete, avisa que ya fue transmitido. Pag 19. 
													//// Transmit Control Register (Ping) (0x40e007FC).
		if ((loglevel & INTH_LOG) && (loglevel & ETH_LOG)) xil_printf("transmitted packet\r\n");
		eth_state |= (1 << TX_PKT);
		/* Clear the Interrupt */
		eth_ctlr->tx_ping_control &= ~(1<< 3);		//// Limpia el resgistro Transmit Control Register (Ping)  (0x40e007FC).
	} 
	
}

enum dma_states { dma_busy, dma_ready };
enum dma_states dma_state;

void dmactlr_handler () //// Documentacion en la siguiente funcion: pg034-axi-cdma.
{
	dma_state = dma_ready; 						//// Libera el DMA para que se pueda volver a utilizar. 

	/* Clear the Interrupt */
	dma_ctlr->cdmasr &= ~DMA_CRMSK_IRQEN; 		//// Limpia los bits de interrupciones, 12 y 14, del registro 
	dma_ctlr->cdmasr &= ~DMA_CRMSK_IRQERR;		////  CDMA Status. Pag 18. 

}
//// Documentacion usada en la sig funcion: pg099-axi-intc.
void InterruptHandler(void *CallbackRef)
{
	for (int irq = 0; irq < 32; irq++) {		////32 interrupciones posibles, ciclo for para leer uno a uno. 
		if (intp_ctlr->isr & (1 << irq)) {		////los contenidos de ISR (Interrupt Status Register, 0x41200000) indican la presencia  
												//// o absencia de una senal activa de interrupcion. Pag 15.
			switch(irq) {					
			case INTNUM_ETHERNET:				
				ethctlr_handler();				////Se llama al controlador de interrupciones de Ethernet.
				break;
			case INTNUM_TIMER:
				timer_handler();				////Se llama al controlador de interrupciones de Timer.
				break;
			case INTNUM_DMA:
				dmactlr_handler();				////Se llama al controlador de interrupciones del DMA.
				break;
			default:
				break;
			}
		}
		intp_ctlr->iar = (1 << irq); 			//// Limpieza de ISR, para que, una vez que fueron manejadas, deje de indicar una interrupcion. 
	}
}

void setup_intctlr(void);
void setup_ethctlr(void);
void eth_tx(size_t);
void dmacpy(void volatile *, const void volatile *, size_t);

char buffer[2048];

enum states { 
	idle,				 //// Modo para cuando el procesador no tiene tareas a realizar, esta en espera de un paquete. 
	rxpkt_mii,			 //// Se recibio un paquete de Ethernet. 
	rxpkt_waitdma,		 //// Esperando a que termine la transferencia por DMA.
	rxpkt_dmacpied,		 //// Los datos fueron copiados y se puede responder por ARP.
	txpkt_waitdma,		 //// Transmision de retorno preparada por DMA a traves de Ethernet.  
	txpkt_dmacpied };	 //// Finalizacion de la transmision de retorno. 

enum states state = idle;

int main(void)
{
	xil_printf("\r\n\r\n");
	xil_printf("/* pico ping server */\r\n");
	xil_printf("/* **** **** ****** */\r\n");

	setup_intctlr();
	setup_ethctlr();

	for(;;) {							//// Loop para que no termine la ejecucion y este siempre esperando y recibiendo paquetes. 
		unsigned chksum;
		unsigned length;
		unsigned tx_length;

		switch(state) {					//// Maquina de estados por software.
		case idle:
			if (eth_state & (1 << RX_PKT) || (eth_ctlr->rx_ping_control & (1<< 0))) { 	//// Si se recibio un paquete por ethernet,
																						////Receive control register (0x40e017FC) = 1. 
				eth_state &= ~(1 << RX_PKT);											////Se procesa, permitiendo ingresar otro paquete,
				eth_ctlr->rx_ping_control &= ~(1<< 0);									//// y se limpia el registro. 
				if (loglevel & STATE_LOG) xil_printf("rxpkt_mii\r\n");
				state = rxpkt_mii;													//// Paso al siguiente estado de paquete recibido,
			} 																		//// De no haber recibido un paquete se queda en idle esperando
			break;
		case rxpkt_mii:
			dmacpy(buffer, (void *) eth_ctlr->rx_ping_data, sizeof(eth_ctlr->rx_ping_data));  ////Se copia en buffer el paquete que tiene ethernet.
			if (loglevel & STATE_LOG) xil_printf("rxpkt_waitdma\r\n");
			state = rxpkt_waitdma;													//// Paso al estado que espera a que el DMA termine.
			break;
		case rxpkt_waitdma:
			switch(dma_state) {
			case dma_busy:															////Si el DMA sigue ocupado:
				state = rxpkt_waitdma;												//// me mantengo en el mismo estado.
				break;
			case dma_ready:															////Si el DMA se libera:
				if (loglevel & STATE_LOG) xil_printf("rxpkt_dmacpied\r\n");			//// se pasa al estado de procesamiento del paquete. 
				state = rxpkt_dmacpied;												
				break;
			};
			break;
		case rxpkt_dmacpied:
			switch (													//// Distingue entre si es un paquete ARP o IP. 
				(((struct eth_hdr *) buffer)->type[0] << 8) |
				(((struct eth_hdr *) buffer)->type[1] << 0)) {
			case ETHTYP_ARP:	
				if ((loglevel & SERVER_LOG) > 0) {
					xil_printf("GRATUITOUS ARP reply to : MAC => ");
					print_mac(((struct arp_frm *) buffer)->eth_hdr.src);
					xil_printf(" IP => ");
					print_ipv4(((struct arp_frm *) buffer)->arp4_hdr.tpa);
					xil_printf("\r\n");
				}
				if (!memcmp(my_ipv4, ((struct arp_frm *) buffer)->arp4_hdr.tpa, sizeof(my_ipv4))) {
					tx_length = sizeof(my_arp);
					dmacpy(eth_ctlr->tx_ping_data, &my_arp, sizeof(int)*((tx_length+sizeof(int)-1)/sizeof(int)));
					if (loglevel & STATE_LOG) xil_printf("txpkg_waitdma\r\n");
					state = txpkt_waitdma;
				} else {
					if (loglevel & STATE_LOG) xil_printf("idle\r\n");
					state = idle;
				}
				break;
			case ETHTYP_IP:

				if (!memcmp(my_ipv4, ((struct icmp_frm *) buffer)->ipv4_hdr.da, sizeof(my_ipv4)) &&		//// Verifico que la direccion de destino del paquete
					((struct icmp_frm *) buffer)->ipv4_hdr.proto[0] == icmp_proto) {					//// sea la propia. 

					chksum = 0;
					chksum |= ((struct icmp_frm *) buffer)->icmp_hdr.chksum[0];
					chksum <<= 8;
					chksum |= ((struct icmp_frm *) buffer)->icmp_hdr.chksum[1];

					chksum += 0x0800;
					if (chksum > 0xffff) chksum++;

					((struct icmp_frm *) buffer)->icmp_hdr.chksum[1] = (chksum & 0xff);
					chksum >>= 8;
					((struct icmp_frm *) buffer)->icmp_hdr.chksum[0] = (chksum & 0xff);

					((struct icmp_frm *) buffer)->icmp_hdr.code[0] = 0;

					length = 0;
					length |= ((struct icmp_frm *) buffer)->ipv4_hdr.len[0];
				   	length <<= 8;
					length |= ((struct icmp_frm *) buffer)->ipv4_hdr.len[1];
					length += sizeof(struct eth_hdr);

					for (int i = 0; i < 6; i++) {
						char unsigned aux;

						aux = ((struct icmp_frm *) buffer)->eth_hdr.src[i];
						((struct icmp_frm *) buffer)->eth_hdr.src[i]  = ((struct icmp_frm *) buffer)->eth_hdr.dest[i];
						((struct icmp_frm *) buffer)->eth_hdr.dest[i] = aux;
					}

					for (int i = 0; i < 4; i++) {
						char unsigned aux;

						aux = ((struct icmp_frm *) buffer)->ipv4_hdr.sa[i];
						((struct icmp_frm *) buffer)->ipv4_hdr.sa[i]  = ((struct icmp_frm *) buffer)->ipv4_hdr.da[i];
						((struct icmp_frm *) buffer)->ipv4_hdr.da[i] = aux;
					}
					((struct icmp_frm *) buffer)->icmp_hdr.type[0] = 0x00;
 
					if ((loglevel & SERVER_LOG) > 0) {
						xil_printf("ICMP reply to : MAC => ");
						print_mac(((struct icmp_frm *) buffer)->eth_hdr.dest);
						xil_printf(" IP => ");
						print_ipv4(((struct icmp_frm *) buffer)->ipv4_hdr.da);
						xil_printf("\r\n");
					}

					tx_length = length;
					dmacpy(eth_ctlr->tx_ping_data, buffer, sizeof(int)*((tx_length+sizeof(int)-1)/sizeof(int)));
					if (loglevel & STATE_LOG) xil_printf("txpkg_waitdma\r\n");
					state = txpkt_waitdma;
				} else {
					if (loglevel & STATE_LOG) xil_printf("idle\r\n");
					state = idle;
				}
				break;
			default:
				if (loglevel & STATE_LOG) xil_printf("idle\r\n");
				state = idle;
				break;
			};
			break;
		case txpkt_waitdma:
			switch(dma_state) {
			case dma_busy:
				state = txpkt_waitdma;
				break;
			case dma_ready:
				if (loglevel & STATE_LOG) xil_printf("txpkt_dmacpied\r\n");
				eth_tx(tx_length);
				state = txpkt_dmacpied;
				break;
			};
			break;
		case txpkt_dmacpied:
			if (loglevel & STATE_LOG) xil_printf("idle\r\n");
			state = idle;
			break;
		default:
			state = idle;
			break;
		}
	}


}
//// Documentacion para las siguientes dos funciones: pg135-xi-ethernetlite. /////
void setup_ethctlr() ////Inicializacion del controlador de Ethernet
{
	eth_ctlr->tx_ping_data[0] = *(((int *) my_mac)+0); 			//// Ingresa la direccion MAC del dispositivo en los dos primeros bytes
	eth_ctlr->tx_ping_data[1] = *(((int *) my_mac)+1);			//// del registro tx_ping_data. Pag 29. 

	eth_ctlr->tx_ping_control |= (0x2 | 0x1); 					//// Programa: utilizando una mascara en el registro Transmit 
																//// Control Register (Ping) (0x40e007FC), selecciona los dos ultimos bits 
																//// (0,1) comenzando la transmision. Pag 19. 

	while(eth_ctlr->tx_ping_control & (0x2 | 0x1));				//// Espera que la programacion termine. (0x40e007FC). 

	eth_ctlr->gie |= (1 << 31); // Global Interrupt Enable  	//// Habilita las interrupciones generales con el registro 
																//// GIE (0x40e007F8). Pag 18. 

	eth_ctlr->rx_ping_control |= ((1<< 3));						//// Habilita las interrupciones de recepcion mediante el registro
																//// Receive control register for ping buffer (0x40e017FC). Pag 21. 
}

void eth_tx(size_t len)		//// Transmision por ethernet
{
	while(eth_ctlr->tx_ping_control & (0x2 | 0x1));				//// Se asegura que no haya paquetes transmitiendose, al mirar los 
																//// bits 1 y 2 del registro Transmit Control Register (Ping) (0x40e007FC)
																////Pag 19. 

	eth_ctlr->tx_ping_length  = len; 							//// Ingresa la cantidad de datos a transmitir en el registro
																//// Transmit length register for ping buffer (0x40e007F4). 

	eth_ctlr->tx_ping_control |= ((1<< 3) | (1 << 0));			//// Habilita la interrupcion, registro (0x40e007FC)
																//// para que avise cuando deje de transmitir y comienza a transmitir.
																////Pag 19. 


	if (loglevel & ETH_LOG) xil_printf("eth_ctlr->tx_ping_control 0x%08x\n\r", eth_ctlr->tx_ping_control);
	if (loglevel & ETH_LOG) xil_printf("eth_ctlr->tx_ping_length  0x%08x\n\r", eth_ctlr->tx_ping_length);
}


//// Documentacion usada a continuacion: pg099-axi-intc. ////

void setup_intctlr(void)
{

	if (loglevel & INTC_LOG) xil_printf("Init Interrupt Controller\r\n");
	if (loglevel & INTC_LOG) xil_printf("*************************\r\n");

	intp_ctlr->mer = 0; ////Deshabilita todas las interrupciones. Direccion absoluta MER ( Master Enable Register): 0x4120001C. Pag 23. 
	
	intp_ctlr->ier = 0; //// Deshabilita que las interrupciones pasen al procesador. 
					   	////Direccion absoluta IER ( Interrupt Enable Register): 0x41200008. Pag 18.

	intp_ctlr->iar = 0xffffffff; //// Contesta los posibles llamados a interrupciones, al setear ISR=0. 
								////Direccion absoluta IAR (Interrupt Acknowledge Register): 0x4120000C. Pag 19. 

	intp_ctlr->imr = 0; //// Procesa las interrupciones en modo normal y no en modo rapido. 
						//// Direccion absoluta IAR (Interrupt Mode Register): 0x41200020. Pag 24.

	// Interrupt Vector Register Reset for Fast Interrupt
	// for (int i = 0; i < 32 ; i++) {
	//  intp_ctlr->ivar[i] = Xil_ExceptionHandler InterruptHandler;
	// }
	
	// Initialize the exception table and register the interrupt
	// controller handler with the exception table

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT, (Xil_ExceptionHandler) InterruptHandler, (void *) 0);
	Xil_ExceptionEnable();

	intp_ctlr->ier |= IE_MASK; ////Habiltacion del paso de las interrupciones, 
							   ////provenientes de los perifericos ethernet y dma, al procesador. Pag 18.  
	
	if (loglevel & INTC_LOG) xil_printf("intp_ctlr->ier 0x%08x\n\r", intp_ctlr->ier);

	intp_ctlr->mer |= (0x2 | 0x01); //// Setea en 1 al Hardware Interrupt Enable (HIE) bit, 
									////habilitando las interrupciones por hardware. Pag 23.

}

//// Documentacion usada a continuacion: pg034-axi-cdma. ////

void dmacpy(void volatile *dst, const void volatile *src, size_t len)
{
	if (loglevel & DMA_LOG) xil_printf("set DMA Controller\r\n");

	dma_state = dma_busy; //// Setea la variable de estado dma_state como dma_busy asi se mantiene en el estado actual. 

	while(!(dma_ctlr->cdmasr & DMA_SRMSK_IDLE)); //// Mientras que hayan finalizado las transferencias programadas,
												////la CDMA espera por una nueva transferencia a realizar. Pag 22. 

	dma_ctlr->cdmacr |=  DMA_CRMSK_IRQEN;  // DMA Ctlr's Enable Interrupt //// Utilizando el bit 12 del CDMACR permite que 
																		  //// permite a CDMASR.IOC_Irq generar una interrupcion de salida para
																		  //// transferencias DMA completadas. Pag 16. 
	
	dma_ctlr->cdmacr &= ~DMA_CRMSK_IRQERR; // DMA Ctlr's Disable Error Interrupt //// Utilizando el bit 14 del CDMACR deshabilita 
																				//// las interrumpciones generadas por error. Pag 15.
	

	dma_ctlr->sa  = (int) src;			// Set DMA Source      Address  //// Ingresa la direccion de origen del paquete a transferir
																	    //// al registro SA. Pag 27. 
	dma_ctlr->da  = (int) dst;			// Set DMA Destiantion Address  //// Ingresa la direccion de destino del paquete a transferir
																		//// al registro DA. Pag 29. 

	dma_ctlr->btt = len; 				// Set DMA Byte Transfer 		//// Ingresa el tamano (distinto al largo) del paquete a 
																		//// transferir al registro BTT.Pag 31. Max de 
																		//// transferencia: 8MB. Luego de esta instruccion 
																		////la transferencia inicia inmediatamente. 

}

