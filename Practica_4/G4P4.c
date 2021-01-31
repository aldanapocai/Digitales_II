/* Crear un programa en C que tome el estado de los switches y lo refleje en los leds (switch = 1
implica led encendido, switch = 0 implica led apagado).
*/

int main()
{
	while(1){
		*(int*)(0x40000008)=*(int*)(0x40000000);
	}
	return 0;
}
