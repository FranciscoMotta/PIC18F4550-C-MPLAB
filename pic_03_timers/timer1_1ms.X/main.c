/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 30 de mayo de 2023, 08:30 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "system_config.h"

/* Rutina de servicio a interrupciones */

void __interrupt() TMR1_ISR (void)
{
    if(PIE1bits.TMR1IE && PIR1bits.TMR1IF)
    {
        /* Rutina */
        LATBbits.LATB0 = !LATBbits.LATB0;
        /* Cargamos el dato */
        TMR1 = 3036;
        /* Limpiamos la bandera */
        PIR1bits.TMR1IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Timer1_Interrupts (void);
void Init_Timer1_As_Timer (void);

/* Main */
int main(void) 
{
    /* Configuramos el oscilador interno */
    Init_Internal_Oscillator();
    /* Configuramos las interrupciones del timer1 */
    Init_Timer1_Interrupts();
    /* Configuramos el timer1 como tiempo */
    Init_Timer1_As_Timer();
    /* Led como salida */
    TRISBbits.RB0 = 0;
    while(true)
    {
        __nop(); // No hacemos nada
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer1_As_Timer (void)
{
    /* Limpiamos los registros */
    T1CON = 0x00;
    /* Configuramos el timer1 */
    T1CONbits.TMR1ON  = 0; // Apagamos el timer
    T1CONbits.RD16    = 1; // Trabajamos a carga de 16bits
    T1CONbits.T1CKPS  = 0b11; // Prescaler a 8
    T1CONbits.T1OSCEN = 0; // Osc desactivado
    T1CONbits.T1SYNC  = 0; // No importa
    T1CONbits.TMR1CS  = 0; // Frecuencia interna
    
    /* Cargamos el dato */
    TMR1 = 3036;
    /* Prendemos el timer */
    T1CONbits.TMR1ON = 1;
}

void Init_Timer1_Interrupts (void)
{
    /* Limpiamos los registros */
    RCON = 0x00;
    INTCON = 0x00;
    PIR1 = 0x00;
    PIE1 = 0x00;
    
    /* Configuramos */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Ints globales activadas
    INTCONbits.PEIE = 1; // Ints perifs activadas
    
    PIE1bits.TMR1IE = 1; // Timer1 activado
    PIR1bits.TMR1IF = 0; // Bandera limpiada
}

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00;
    /* Configuramos el oscilador interno a 8mhz */
    OSCCONbits.IRCF = 0b111; // 8mhz
    OSCCONbits.SCS = 0b11; // Intosc
}