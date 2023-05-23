/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 23, 2023, 4:34 PM
 */

/*
 * Includes
 */

#define _XTAL_FREQ   8000000UL

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"
/*
 * Rutina de servicio a la interrupción
 */

void __interrupt() INT0_IRS (void)
{
    if(INTCONbits.INT0IF && INTCONbits.INT0IF)
    {
        /* Rutina */
        LATDbits.LATD1 = !LATDbits.LATD1;
        /* Limpiamos la bandera */
        INTCONbits.INT0IF = 0; 
    }
}

/*
 * Declaración de funciones
 */

void Internal_Oscillator_Init (void);
void Interrupt_External_Init (void);

/*
 * Main
 */
int main(void)
{
    /* Configuramos el reloj intenro */
    Internal_Oscillator_Init(); 
    /* Configuramos la interrupción externa */
    Interrupt_External_Init();
    /* Led D0 como salida */
    TRISDbits.RD0 = 0;
    /* Led D1 como salida */
    TRISDbits.RD1 = 0;
    while(true)
    {
        LATDbits.LATD0 = 1;
        __delay_ms(250);
        LATDbits.LATD0 = 0;
        __delay_ms(250);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definición de funciones
 */

void Interrupt_External_Init (void)
{
    /* Limpiamos los registros */
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    RCON = 0x00;
    
    /* Configuramos la interrupción interna */
    RCONbits.IPEN = 0; // No usamos interrupciones
    INTCONbits.GIE = 1; // Activamos las interrupciones
    INTCONbits.INT0IE = 1; // Activamos el INT0
    INTCONbits.INT0IF = 0; // Bandera apagada
    INTCON2bits.INTEDG0 = 1;// Flanco de subida
    
    /* Definimos el INT0 como entrada */
    TRISBbits.RB0 = 1; // RB0 como entrada
}

void Internal_Oscillator_Init (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00; // Todos los bits del registro a 0
    
    /* Configuramos */
    OSCCONbits.IDLEN = 1; // CPU funcionando
    OSCCONbits.IRCF = 0b111; // Frecuencia a 8MHZ
    OSCCONbits.SCS = 0b11; // Seleccionamos el clock interno
}