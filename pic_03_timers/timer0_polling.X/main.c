/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 3 de junio de 2023, 06:27 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "system_config.h"

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Timer_0_As_Timer (void);

/* Main */
int main(void)
{
    /* Se configura el oscilador interno */
    Init_Internal_Oscillator();
    /*  */
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer_0_As_Timer (void)
{
    /* Limpiamos los registros */
    T0CON = 0x00;
    
    /* Configuramos el timer0 a 1ms  */
}
void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    
    /* Configuramos el oscilador interno a 8Mhz */
    OSCCONbits.IRCF = 0b111; // Freq 8mhz
    OSCCONbits.SCS = 0b11; // Intosc
}