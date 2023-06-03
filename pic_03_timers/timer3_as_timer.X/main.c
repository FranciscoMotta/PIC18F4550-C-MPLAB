/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 2 de junio de 2023, 02:47 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "system_config.h"

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);

/* Main */
int main(void)
{

    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
}

