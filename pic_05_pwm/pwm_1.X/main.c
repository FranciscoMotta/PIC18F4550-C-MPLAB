/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 2 de junio de 2023, 02:47 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <proc/pic18f57q43.h>
#include "system_config.h"

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_PWM (void);

/* Main */
int main(void)
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_PWM (void)
{
    /* Configuramos el TIMER2 */
    /* Limpiar los registros */
    T2CON = 0x00;
    /* Configurar el timer2 */
    T2CONbits.TMR2ON = 0; // Timer 2 apagado
    T2CONbits.T2CKPS = 0b11; // Prescaler 8
    /* Consideramos que:
     * Periodo = (PR2 + 1)*Tosc*(TMR2_Prescaler)
     * Periodo = (PR2 + 1)*(1 / Fosc)*(TMR2_Prescaler)
     * 
     * PR2 = (Periodo / (Tosc * TMR2_Prescaler)) - 1
     * 
     * Para este caso:
     * - periodo = 2ms
     * - Fosc = 8Mhz
     * - TMR2_Prescaler = 8
     * 2*10^-3 = (PR2 + 1)*(1 / 8*10^6)*(4)
     * PR2 = (2*10^-3) / ((1 / 8*10^6)*(8)) - 1
     */
    
}

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    /* Se configura el cristal intenro */
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
}