/*Crear un programa en C que prenda un solo led del kit accediendo al registro de datos del GPIO
correspondiente.*/

int main()
{
	while(1){
		*(int*)(0x40000008)=0x00000001;
	}
	return 0;
}
