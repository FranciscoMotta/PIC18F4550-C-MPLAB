/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 23, 2023, 7:00 PM
 */

/* Macros */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"

/* Rutina de servicio a interrupciones */

void __interrupt() INT2_ISR (void)
{
    if(INTCON3bits.INT2IE && INTCON3bits.INT2IF)
    {
        /* Rutina */
        LATD ^= (1 << _LATD_LATD1_POSITION);
        /* Limpiamos la bandera */
        INTCON3bits.INT2IF = 0;
    }
}

/* Declaracion de funciones */

void Init_Internal_Oscillator (void);
void Init_GPIO_System (void);
void Init_External_Interrupt3 (void);
/* Main */

int main(void)
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configuran los GPIOs */
    Init_GPIO_System();
    /* Se configura la int2 */
    Init_External_Interrupt3();
    while (true)
    {
        LATD ^= (1 << _LATD_LATD0_POSITION);
        __delay_ms(150);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_External_Interrupt3 (void)
{
    /* Se limpian los registros a usar */
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    RCON = 0x00;
    
    /* Configuramos la interrupción externa 3 - INT2 */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Interrupciones globales activadas
    INTCON2bits.INTEDG2 = 1; // Interrupcion por flanco de subida
    INTCON3bits.INT2IE = 1; // Interrupción 3 activada
    INTCON3bits.INT2IF = 0; // Bandera de interrupción apagada
}

void Init_GPIO_System (void)
{
    /* RD0 y RD1 como salida */
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    
    /* RB2 como entrada */
    TRISBbits.RB2 = 1;
}

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    
    /* Se configuran el cristal interno */
    OSCCONbits.IDLEN = 0; // CPU en modo ocupado
    OSCCONbits.IRCF = 0b111; // Frecuencia a 8mhz
    OSCCONbits.SCS = 0b11; // Cristal interno elegido
}