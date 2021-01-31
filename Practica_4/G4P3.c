/*Crear un programa en C que tome el estado de los switches y lo muestre por una terminal.*/

#include <stdio.h>

int main()
{
	int i;
	int sw;
	while(1){
		for(i=0;i<1000000;i++)
		sw=*(int*)(0x40000000);
		printf("Estado de los switches: %d\r\n",sw);
	}
	return 0;
}
