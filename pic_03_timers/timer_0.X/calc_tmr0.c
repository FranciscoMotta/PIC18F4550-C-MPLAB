/* Calculadora del registro TMR0 */

/* Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

/* Enumeraciones */

typedef enum
{
	prescaler_2   = 2,
	prescaler_4   = 4,
	prescaler_8   = 8,
	prescaler_16  = 16,
	prescaler_32  = 32,
	prescaler_64  = 64,
	prescaler_128 = 128,
	prescaler_256 = 256
}prescaler_val_t;

typedef enum
{
	bits_8 = 8,
	bits_16 = 16
}n_bits_t;

/* declaración de funciones */

uint16_t calculate_TMR0_val (uint16_t t_ms, prescaler_val_t pres, n_bits_t bits);

/* IMPORTANTE TIENES QUE DEFINIR FOSC */ 

#define FOSC 8000000

/* main */

int main (void)
{
	uint16_t val_cal = calculate_TMR0_val(1, prescaler_8, bits_8);
	printf("El valor del TMR0 es: %i\n", val_cal);
	uint8_t upper_8, lower_8 = 0;
	upper_8 = (val_cal & 0xFF00) >> 8;
	lower_8 = (val_cal & 0x00FF);
	printf("TMR0H : %i\nTMR0L : %i\n", upper_8, lower_8); 
	return EXIT_SUCCESS;
}

/* definición de funciones */
uint16_t calculate_TMR0_val (uint16_t t_ms, 
							 prescaler_val_t pres, 
							 n_bits_t bits)
{
	int16_t tmr0_val = 0;
	/* Preguntamos por el nbits */
	if(bits == bits_16)
	{
		tmr0_val = 65536 - ((t_ms*FOSC)/(4000*pres));
	}
	else
	{
		tmr0_val = 256 - ((t_ms*FOSC)/(4000*pres));
	}
	
	/* Es posible que el valor de tmr0_val sea negativo */
	if(tmr0_val < 0)
	{
		printf("ERROR! Valor del TMR0 negativo, prueba cambiar el prescaler o el tiempo");
		tmr0_val = 0;
	}
	else
	{
		// No hacemos nada
	}
	return tmr0_val;
	
}
							 
