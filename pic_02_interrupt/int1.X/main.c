/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 23, 2023, 6:40 PM
 */

#define _XTAL_FREQ 8000000UL

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"

/* Rutina de servicio a interrupciones */

void __interrupt()  INT1_IRQ (void)
{
    if(INTCON3bits.INT1IE && INTCON3bits.INT1IF)
    {
        /* Rutina */
        LATDbits.LATD1 = !LATDbits.LATD1;
        /* Se limpia la bandera de interrupción */
        INTCON3bits.INT1IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_External_Interrupt2 (void);
/* Main */
int main(void)
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configura los GPIOs del sistema */
    Init_Gpio_System();
    /* Se configura la interrupción externa 2 */
    Init_External_Interrupt2();
    while(true)
    {
        LATDbits.LATD0 = !LATDbits.LATD0;
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/* Definicion de funciones */

void Init_External_Interrupt2 (void)
{
    /* Se limpian los registros a usar */
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    RCON = 0x00;
    
    /* Se configura el INT1 - Interrupción externa 2 */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Ints. globales enabled
    INTCON2bits.INTEDG1 = 1; // INT1 por flanco de subida
    INTCON3bits.INT1IE = 1; // INT1 activada
    INTCON3bits.INT1IF = 0; // Bandera del INT1 apagada
}

void Init_Gpio_System (void)
{
    /* RD0 y RD1 como salidas */
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    /* RB1 como entrada */
    TRISBbits.RB1 = 1;
}
void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros a usar */
    OSCCON = 0x00;
    
    /* Configuramos el reloj interno a 8MHZ */
    OSCCONbits.IRCF = 0b111; // Frecuencia a 8MHZ
    OSCCONbits.SCS = 0b11; // Elegimos el clock interno
}
