/* Includes */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Variables globales */

char vector[] = "Hola Mundo";
char *ptr;
/* Main */

int main (void)
{
	ptr = &vector[5]; // Asignamos la posici�n del 6to elemento del vector "M"
	/* Ahora el puntero tiene la direcci�n de 'M' 
	   Entonces, si queremos cambiar el contenido
	   que hay en esa posici�n de memoria hacemos:
	*/
	*ptr = 'F'; // Es decir, reemplazamos el contenido de la posici�n al que apunta ptr
	/* Ahora, podemos desplazarnos sobre la direcci�n elegida */
	ptr++; // Aumentamos en 1 la direcci�n apuntada
	*ptr = 'o';
	/* Ahora reemplazamos la nueva direcci�n a la que apunta ptr 
	   Es decir, vamos a reemplazar 'u' con 'o'
	*/
	ptr[2] = 'c';
	/* Ahora sabemos que ptr tiene una direcci�n apuntada
	   al hacer ptr[2] indicamos que sumaremos en 2 a la 
	   posici�n apuntada. Por lo cual, cambiamos el valor
	   de dicha posici�n
	*/
	printf("El valor nuevo del vector es: %s", vector);
	return EXIT_SUCCESS;
}
