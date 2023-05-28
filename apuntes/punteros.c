/* PUNTEROS */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Declaraci�n de variables */

uint8_t variable = 60; // <- SE ASIGNA UNA POS DE MEMORIA
uint8_t *puntero;

/* Main */

int main (void)
{
	/* Apuntamos "puntero" a la direcci�n que deseemos */
	puntero = &variable;
	printf("La variable tiene direcci�n : 0x%x ", puntero);
	printf("Y tiene el valor : %i\n", variable);
	/* Cambiamos el valor de variable */
	*puntero = 50;
	printf("Ahora el valor es : %i", variable);
	return EXIT_SUCCESS;
}


