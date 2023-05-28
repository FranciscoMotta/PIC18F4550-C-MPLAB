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
	ptr = &vector[5]; // Asignamos la posición del 6to elemento del vector "M"
	/* Ahora el puntero tiene la dirección de 'M' 
	   Entonces, si queremos cambiar el contenido
	   que hay en esa posición de memoria hacemos:
	*/
	*ptr = 'F'; // Es decir, reemplazamos el contenido de la posición al que apunta ptr
	/* Ahora, podemos desplazarnos sobre la dirección elegida */
	ptr++; // Aumentamos en 1 la dirección apuntada
	*ptr = 'o';
	/* Ahora reemplazamos la nueva dirección a la que apunta ptr 
	   Es decir, vamos a reemplazar 'u' con 'o'
	*/
	ptr[2] = 'c';
	/* Ahora sabemos que ptr tiene una dirección apuntada
	   al hacer ptr[2] indicamos que sumaremos en 2 a la 
	   posición apuntada. Por lo cual, cambiamos el valor
	   de dicha posición
	*/
	printf("El valor nuevo del vector es: %s", vector);
	return EXIT_SUCCESS;
}
