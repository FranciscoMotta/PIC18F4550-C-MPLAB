/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 1 de junio de 2023, 07:23 PM
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

void __interrupt() TIMER2_ISR (void)
{
    if(PIR1bits.TMR2IF && PIR1bits.TMR2IF)
    {
        /* Rutina */
        LATBbits.LATB0 = !LATBbits.LATB0;
        /* Limpiamos la bandera */
        PIR1bits.TMR2IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Timer2_Interrupts (void);
void Init_Timer2_As_Comparer (void);

/* Main */
int main(void) 
{
    /* Se configura el oscilador interno */
    Init_Internal_Oscillator();
    /* Configuramos las interrupciones del timer2 */
    Init_Timer2_Interrupts();
    /* Se configura el timer2 como comparador */
    Init_Timer2_As_Comparer();
    /* RB0 como salida */
    TRISBbits.RB0 = 0;
    /* Bucle principal */
    while (true) 
    {
        __nop();
    }

    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer2_As_Comparer (void)
{
    /* Limpiamos los registros */
    T2CON = 0x00;
    
    /* Configuramos el timer2 como comparador */
    T2CONbits.TMR2ON = 0; // Apagamos el timer2
    T2CONbits.T2CKPS = 0b11; // Prescaler a 1:16
    /* Es decir, a este punto tendríamos la freq/64
     * por lo cual Ftmr2 = 8MHZ/64 = 125,000hz*/
    T2CONbits.TOUTPS = 0x9; // Usamos el poscaler 1:10
    
    /* Cargamos el dato al registro de comparación */
    PR2 = 249; // <- sin contar el 0
    
    /* Esto nos daría una frencencia de:
     * Freq = FOSC / (4 *  precaler * (PR2 + 1) * poscaler)
     * Para este caso:
     * Freq = 8*10^6 / (4 * 16 * (249 + 1) * 10)
     * Freq = 50hz  
     * 
     * < Nota: Como hacemos que cada que se produzca la comparación
     * se detone la interrupción y, en ella, hacemos que el LED conectado
     * al RB0 cambie de estado, tendríamos que la frecuencia de entrada = 50hz
     * se vea divida entre 2, es decir, la frecuencia de parpadeo = 25hz
     */
    
    /* Encendemos el timer2 */
    T2CONbits.TMR2ON = 1;
}

void Init_Timer2_Interrupts (void)
{
    /* Limpiamos los registros */
    RCON   = 0x00;
    INTCON = 0x00;
    PIR1   = 0x00;
    PIE1   = 0x00;
    
    /* Configuramos las interrupciones del timer2 */
    RCONbits.IPEN = 0;
    INTCONbits.GIE = 1; // Interrupciones globales activadas
    INTCONbits.PEIE = 1; // Interrupciones de periféricos activadas
    
    PIR1bits.TMR2IF = 0; // Limpiamos la bandera de interrupción
    PIE1bits.TMR2IE = 1; // Activamos la interrupción del timer2
}

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00;
    /* Configuramos el cristal interno */
    OSCCONbits.IRCF = 0b111; // Freq 8MHZ
    OSCCONbits.SCS = 0b11; // Intosc
}