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
#include "system_config.h"

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR(void) 
{

}

/* Declaración de funciones */

void Init_Internal_Oscillator(void);
void Init_PWM(void);

/* Main */
int main(void) 
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    /* Configurar el PWM1 */
    Init_PWM();
    /* Led del sistema */
    TRISDbits.RD0 = 0;
    /* Bucle principal */
    while (true) {
        LATDbits.LATD0 = !LATDbits.LATD0;
        __delay_ms(250);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_PWM(void) 
{
    /* PIN CCP1 como salida */
    TRISCbits.RC2 = 0;
    /* Consideramos que:
     * Periodo = (PR2 + 1)*Tosc*(TMR2_Prescaler)*4
     * Periodo = (PR2 + 1)*(1 / Fosc)*(TMR2_Prescaler)*4
     * 
     * PR2 = (Periodo / (Tosc * TMR2_Prescaler * 4)) - 1
     * 
     * Para este caso:
     * - periodo = 1 / freq = 1 / 500hz
     * - periodo = 1 / 10^4 = 2ms
     * - Fosc = 8Mhz
     * - TMR2_Prescaler = 16
     * Por lo tanto:
     * PR2 = (2ms) / (4*(1 / 8*10^6)*(16)) - 1
     * PR2 = 249
     */
    // Cargar el dato al registro
    PR2 = 249;
    /* Cálculo del duty cicle 
     * Nota: Duty Cicle = (CCPRXL : CCPXCON<5:4>) * Tosc * TMR2_Prescaler 
     * Para este caso, queremos una duty cicle del 40%, por lo tanto:
     * Datos: 
     * - Tosc = 8MHZ
     * - TMR2_Prescaler = 16
     * - DC = 50% (1ms)
     * Entonces:
     * DC = 1ms
     * (CCPRXL : CCPXCON<5:4>) = Duty Cicle / (Tosc * TMR2_Prescaler)
     * (CCPRXL : CCPXCON<5:4>) = 1ms / ((1 / 8Mhz) * 16)
        * (CCPRXL : CCPXCON<5:4>) = 500 (0x1F4 - 0b01 1111 0100)
     * Por lo tanto:
     */

    CCPR1L = 0b01111101;
    CCP1CONbits.DC1B = 0b00;

    /* Configurar el módulo CCP como PWM */
    // PWM mode
    CCP1CONbits.CCP1M = 0b1111;

    /* Configurar el timer2 */
    T2CONbits.T2CKPS = 0b11; // Prescaler 16

    // Limpiar el registro de cuenta
    TMR2 = 0;

    /* Encendemos el TIMER2 */
    T2CONbits.TMR2ON = 1;
}

void Init_Internal_Oscillator(void) 
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    /* Se configura el cristal intenro */
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
}