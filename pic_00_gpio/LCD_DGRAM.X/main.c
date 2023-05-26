/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 25, 2023, 4:29 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"
#include "lcd_16x2.h"

/* Declaración de funciones */

void Init_Internal_Oscillator (void);

/* Main */

int main(void)
{
    /* Se configura el oscilador interno */
    Init_Internal_Oscillator();
    /* Se inicia el LCD */
    Init_Lcd_16x2();
    /* Seteamos el cursor a 0,0 */
    Lcd_Send_Command(0x82);
    Lcd_Send_Character('A');
    /* Bucle principal */
    while(true)
    {
        
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    /* Se configura el oscilador interno a 8MHZ */
    OSCCONbits.IRCF = 0b111; // Freq 8mhz
    OSCCONbits.SCS = 0B11; // Intosc
}