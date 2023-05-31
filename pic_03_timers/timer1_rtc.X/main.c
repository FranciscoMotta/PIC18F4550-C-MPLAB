/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 30 de mayo de 2023, 06:03 PM
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

void __interrupt()  TMR1_ISR (void)
{
    if(PIR1bits.TMR1IF && PIE1bits.TMR1IE)
    {
        /* Rutina */
        LATBbits.LATB0 = !LATBbits.LATB0;
        /* Cargamos el valor de comp */
        TMR1H = 0x80;
        TMR1L = 0x00;
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
    /* Configuramos el reloj interno */
    Init_Internal_Oscillator();
    /* Configuramos las interrupciones del timer */
    Init_Timer1_Interrupts();
    /* Configuramos el timer1 como temporizador */
    Init_Timer1_As_Timer();
    /* PIM RB0 como salida */
    TRISBbits.RB0 = 0;
    while(true)
    {
        __nop();
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer1_As_Timer (void)
{
    /* Limpiamos los registros */
    T1CON = 0x00;
    
    /* Configuramos */
    T1CONbits.TMR1ON  = 0; // Apagamos el timer para configurar
    T1CONbits.RD16    = 0; // Timer1 trabajando a 16bits
    T1CONbits.T1RUN   = 0; // Bit de lectura
    T1CONbits.T1CKPS  = 0; // prescaler a 1
    T1CONbits.T1OSCEN = 1; // Oscilador activado
    T1CONbits.T1SYNC  = 1; // No sync
    T1CONbits.TMR1CS  = 1; // reloj externo
     
    /* Cargamos el valor de comparación */
    TMR1H = 0x80;
    TMR1L = 0x00;
    
    /* Encendemos el timer1 */
    T1CONbits.TMR1ON = 1;
}

void Init_Timer1_Interrupts (void)
{
    /* Limpiamos los registros */
    RCON = 0x00;
    INTCON = 0x00;
    PIR1 = 0x00;
    PIE1 = 0x00;
    
    /* Configuramos las interrupciones del timer1 */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Globales activadas
    INTCONbits.PEIE = 1; // Interrupciones de perifs
    
    /* Ints timer1 */
    PIE1bits.TMR1IE = 1; // Interrupcion activada
    PIR1bits.TMR1IF = 0; // Bandera limpiada
}

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00;
    
    /* Configuramos el reloj interno a 8MHz */
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 0b11; // INTOSC
}
