/* PUNTEROS */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Declaración de variables */

uint8_t variable = 60; // <- SE ASIGNA UNA POS DE MEMORIA
uint8_t *puntero;

/* Main */

int main (void)
{
	/* Apuntamos "puntero" a la dirección que deseemos */
	puntero = &variable;
	printf("La variable tiene dirección : 0x%x ", puntero);
	printf("Y tiene el valor : %i\n", variable);
	/* Cambiamos el valor de variable */
	*puntero = 50;
	printf("Ahora el valor es : %i", variable);
	return EXIT_SUCCESS;
}


