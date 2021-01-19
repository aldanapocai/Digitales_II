// 8. La siguiente función calcula el largo de una cadena de caracteres. Reformularla para que la
// funcionalidad sea resuelta por medio del uso de un puntero auxiliar en lugar de la variable n.

#include <stdio.h>

int strlen(char *s){
 
 char *aux;

 for (aux=&s[0]; *aux!= '\0'; aux++){
 }
 
 return (aux-s);

}

int main(){
	
	char cadena[] = "hola que tal";
	
	int x = strlen(cadena);
	
	printf("El largo de la cadena '%s' es %d.\n", cadena, x);
	
	return 0;
}
