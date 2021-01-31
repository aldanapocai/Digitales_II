/*Tomando de base el programa anterior escribir uno nuevo que prenda y apague un led
indefinidamente.
*/
int main()
{
	int i;
	while(1){
		for(i=0;i<50000;i++)
			*(int*)(0x40000008)=0x00000001;
		for(i=0;i<50000;i++)
			*(int*)(0x40000008)=0x00000000;
	}
	return 0;
}
