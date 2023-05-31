/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 31 de mayo de 2023, 01:43 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "system_config.h"

/* Variables globales */

uint16_t timer2_counter = 0;
bool state_led;

/* Rutina de servicio a interrupciones */

void __interrupt() TMR2_ISR (void)
{
    if(PIR1bits.TMR2IF && PIE1bits.TMR2IE)
    {
        /* Rutina */
        if(timer2_counter++ == 500)
        {
            timer2_counter = 0;
            state_led = !state_led;
        }
        /* Se carga el valor */
        TMR2 = 131;
        /* Se limpia la bandera */
        PIR1bits.TMR2IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Timer2_Interrupts (void);
void Init_Timer2_As_Timer (void);

/* Main */
int main(void) 
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configura las interrpciones del timer2 */
    Init_Timer2_Interrupts();
    /* Se configura el timer 2 como temporizador */
    Init_Timer2_As_Timer();
    /* RB0 como salida */
    TRISBbits.RB0 = 0;
    while(true)
    {
        LATBbits.LATB0 = state_led;
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer2_As_Timer (void)
{
    /*Se limpia el registro*/
    T2CON = 0x00;
    
    /* Se configura el timer0 */
    T2CONbits.TMR2ON = 0; // Apagamos el timer2
    T2CONbits.T2CKPS = 0b11; // Asignamos un prescaler de 1:16
    T2CONbits.TOUTPS = 0x0; // No asignamos poscaler
    /* Notas:
     * Estamos trabajando con un cristal de 8Mhz, es decir
     * FOSC = 8MHZ, por consiguiente el tiempo que conseguiríamos
     * sería de:
     * 
     * tiempo = 256 * (prescaler * 4) / FOSC
     * tiempo = 256 * (16 * 4) / 8*10^6
     * tiempo = 0.002048 seg = 2.0048ms
     * 
     * Pero queremos un tiempo de 1ms, por lo tanto:
     * 
     * TMR2 = 256 - ((tiempo * FOSC)/(prescaler * 4))
     * TMR2 = 256 - ((1 * 10^-3 * 8 * 10^6)/(16 * 4))
     * TMR2 = 131
     * 
     * Cargando ese valor al registro aseguramos un tiempo de 1ms
     * 
     *  */
    
    TMR2 = 131; // Valor de carga para el registro
    
    /* Encendemos el timer */
    T2CONbits.TMR2ON = 1;
}

void Init_Timer2_Interrupts (void)
{
    /* Se limpian los registros */
    INTCON = 0x00;
    PIR1 = 0x00;
    PIE1 = 0x00;
    RCON = 0x00;
    
    /* Se configuran las interrupciones del tmr2 */
    RCONbits.IPEN = 0; // Sin interrupciones
    INTCONbits.GIE = 1; // Ints globales activadas
    INTCONbits.PEIE = 1; // Ints de perifericos activadas
    
    PIR1bits.TMR2IF = 0; // Bandera de interrupción del timer2 en 0
    PIE1bits.TMR2IE = 1; // Interrupción del timer2 habilitada
}

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    /* Se configura el reloj interno */
    OSCCONbits.IRCF = 0b111; // 8MHz
    OSCCONbits.SCS = 0b11; // Intosc
}