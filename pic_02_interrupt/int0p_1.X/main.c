/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 24, 2023, 2:26 PM
 */

#define _XTAL_FREQ  8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"

/* Macros */

#define dms(x)      __delay_ms(x)

/* Rutina de servicio a interrupciones */

void __interrupt(high_priority) INT0_ISR (void)
{
    /* Interrupciones de alta prioridad */
    if(INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        /* Rutina */
        LATDbits.LATD0 = 1;
        LATDbits.LATD1 = 0;
        /* Bloqueamos el flujo */
        while(PORTBbits.RB0); // <- hacemos esto para parar el programa 
        /* Se limpia la bandera */
        INTCONbits.INT0IF = 0;
    }
}

void __interrupt(low_priority) INT1_ISR (void)
{
    /* Interrupciones de baja prioridad */
    if(INTCON3bits.INT1IE && INTCON3bits.INT1IF)
    {
        /* Rutina */
        LATDbits.LATD0 = 0;
        LATDbits.LATD1 = 1;
        /* Bloqueamos el flujo */
        while(PORTBbits.RB1); // <- hacemos esto para parar el programa 
        /* Se limpia la bandera */
        INTCON3bits.INT1IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_External_Interrupt(void);

/* Main */
int main(void)
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configura el GPIO */
    Init_Gpio_System();
    /* Se configura las interrupciones */
    Init_External_Interrupt();
    while(true)
    {
        LATDbits.LATD2 = !LATDbits.LATD2;
        dms(250);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */


void Init_External_Interrupt(void)
{
    /* Se limpian los registros */
    RCON = 0x00;
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    
    /* Se configura las interrupciones */
    RCONbits.IPEN = 1; // Prioridades activadas
    INTCONbits.GIEH = 1; // Interrupciones de alta prioridad activada
    INTCONbits.GIEL = 1; // Interrupciones de baja prioridad activada
    
    /* Int0 de alta prioridad */
    INTCONbits.INT0IE = 1; // INT0 enabled
    INTCONbits.INT0IF = 0; // Bandera limpiada
    INTCON2bits.INTEDG0 = 1; // Activado por flanco de subida
    
    /* Int1 de baja prioridad */
    INTCON3bits.INT1IE = 1; // INT1 enabled
    INTCON3bits.INT1IF = 0; // Bandera limpiada
    INTCON2bits.INTEDG1 = 1; // Flanco de subida
    INTCON3bits.INT1IP = 0; // Int1 de baja prioridad
}

void Init_Gpio_System (void)
{
    /* RD0, RD1 y RD2 como salidas */
    TRISDbits.RD0 = 0;
    TRISDbits.RD1 = 0;
    TRISDbits.RD2 = 0;
    
    /* RB0 y RB1 como entradas */
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
}

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    
    /* Se configura el reloj interno */
    OSCCONbits.IDLEN = 1; // CPU Activa
    OSCCONbits.IRCF = 0b111; // Frec. 8mhz
    OSCCONbits.SCS = 0b11; // Clock interno
}