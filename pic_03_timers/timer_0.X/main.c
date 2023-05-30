/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 29 de mayo de 2023, 07:05 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "system_config.h"

/* Rutinas de servicio a interrupciones */

void __interrupt(high_priority) TIMER0_ISR (void)
{
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF)
    {
        /* Rutina */
        LATBbits.LATB0 = !LATBbits.LATB0;
        /* Cargamos el valor para la temp */
        TMR0H = 11;
        TMR0L = 220;
        /* Limpiamo la bandera */
        INTCONbits.TMR0IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Peripheral_Interrupt (void);
void Init_Peripheral_Timer0 (void);
void Init_Peripheral_GPIO (void);

/* Main */
int main(void) 
{
    /* Configuramos el reloj interno */
    Init_Internal_Oscillator();
    /* Configuramos las interrupciones del tmr0 */
    Init_Peripheral_Interrupt();
    /* Configuramos el GPIO */
    Init_Peripheral_GPIO();
    /* Configuramos la temporización */
    Init_Peripheral_Timer0();
    while(true)
    {
        __nop(); // No hacemos nada
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Peripheral_GPIO (void)
{
    /* LEDB0 como salida */
    TRISBbits.RB0 = 0;
    /* Led apgado */
    LATBbits.LATB0 = 0;
}

void Init_Peripheral_Timer0 (void)
{
    /* Limpiamos los registros */
    T0CON = 0x00;
    
    /* Configuramos */
    T0CONbits.TMR0ON = 0; // Apagamos el timer0 para configurar
    T0CONbits.T08BIT = 0; // Timer0 trabajando a 16 bits
    T0CONbits.T0CS   = 0; // Trabajando con el reloj internO
    T0CONbits.T0SE   = 0; // <NO IMPORTA T0CS = 0
    T0CONbits.PSA    = 0; // Activamos el preescaler
    T0CONbits.T0PS   = 0b010; // Prescaler de 8
    
    /* Ahora debemos configurar el valor del registro TMR0
     * Para cargarlo y que el microcontrolador realice una 
     * temporización de acuerdo a ese valor cargado.
     * 
     * Para ello, consideramos lo siguiente:
     * 
     * < T(TMR0) = (4 / Fosc)*(Prescaler) * (2^nbits - TMR0)
     * < Pero lo que nosotros queremos es TMR0(T), por lo tanto
     * (T * Fosc) / (4 * Prescaler) = 2^nbits - TMR0
     * TMR0(T) = 2^nbits - ((T * Fosc) / (4 * Prescaler))
     * Para este caso: Queremos T = 250ms = 250*10^-3
     * Considerando: Fosc = 8MHZ, Prescaler = 8 y nbits = 16
     * OJO: ESTO DEPENDE DE LO QUE CONFIGURÉ ARRIBA
     * 
     * Entonces:
     * 
     * TMR0 = 2^16 - ((250*10^-3 * 8*10^6) / 4 * 8) 
     * TMR0 = 3036 (DEC)
     * 
     * Pero: 3036 (DEC) = 0x0BDC (HEX)
     * Entonces: 
     * 
     * TMR0H = 0x0B
     * TMR0L = 0xBD
     */
    
    // Cargamos el valor para 250ms
    TMR0H = 11;
    TMR0L = 220;
    
    /* Iniciamos la temporización */
    T0CONbits.TMR0ON = 1;
}

void Init_Peripheral_Interrupt (void)
{
    /* Limpiamos los registros */
    INTCONbits.GIE    = 1; // Habilitamos las interrupciones globales
    // < La linea de abajo no hace falta: OJO
    INTCONbits.PEIE   = 1; // Activamos las interrupciones de los perif
    INTCONbits.TMR0IE = 1; // Activamos la interrupción del timer0
    INTCONbits.TMR0IF = 0; // Limpiamos la bandera de interrupción del tmr0
}
void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    
    /* Configuramos el reloj interno a 8MHZ */
    OSCCONbits.IRCF = 0b111; // INTOSC 8MHZ
    OSCCONbits.SCS = 0b11; // INTOSC Select
}


