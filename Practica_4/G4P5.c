/*Crear un programa en C que cargue 10 valores arbitrarios en la memoria DDR y luego los lea e
imprima por la terminal.
 */

int main(){
	int k=0;
	int *vector=(int*) 0x80000000;
	int *RX= (int*) 0x40600000;
	int *TX= (int*) 0x40600004;
	int *status = (int*) 0x40600008;

	while(k<10){
		if(*status & 0x00000001){
			*vector=*RX;
			vector++;
			k++;
		}
	}

	for(k=0;k<10;k++){
		vector--;
		*TX=*vector;
	}

return 0;
}