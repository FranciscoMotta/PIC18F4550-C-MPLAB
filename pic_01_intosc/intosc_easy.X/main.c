/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 12 de mayo de 2023, 07:58 PM
 */

#define _XTAL_FREQ 4000000ul

/* Librerías */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "system_config.h"

/* Macros */

/* Declaración de funciones */

void Internal_Oscillator_Init (void);

/*
 * Main
 */
int main() 
{
    Internal_Oscillator_Init(); // Iniciamos el reloj intenro
    TRISD &= ~(1 << _TRISD_TRISD0_POSITION); // LD0 como salida
    LATD &= ~(1 << _LATD_LATD0_POSITION); // Led en LD0 apagado
    while(true)
    {
        LATD ^= (1 << _LATD_LATD0_POSITION);
        __delay_ms(200);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Internal_Oscillator_Init (void)
{
    /* Limpiamos el registro OSCCON */
    OSCCON = 0x00;
    /* Frecuencia de 4 Mhz */
    OSCCON |= (0b110 << _OSCCON_IRCF0_POSITION); // OSCCON = 0B01110000
    /* Internal Oscillator */
    OSCCON |= (0b10 << _OSCCON_SCS0_POSITION); // OSCCON = 0B01110010
}