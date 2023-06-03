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

void __interrupt() ISR (void)
{
    if(PIR2bits.TMR3IF && PIE2bits.TMR3IE)
    {
        /* Rutina */
        LATBbits.LATB0 = !LATBbits.LATB0;
        /* Cargamos el dato */
        TMR3 = 65036;
        /* Limpiamos la bandera */
        PIR2bits.TMR3IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Timer3_Interrupts (void);
void Init_Timer3_As_Timer (void);

/* Main */
int main(void)
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    /* Configurar las interrupciones del timer3 */
    Init_Timer3_Interrupts();
    /* Configurar el timer3 como temporizador */
    Init_Timer3_As_Timer();
    /* RB0 como salida */
    TRISBbits.RB0 = 0;
    /* Bucle principal */
    while(true)
    {
        __nop(); // < No hacemos nada :D >
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer3_As_Timer (void)
{
    /* Limpiar los registros */
    T3CON = 0x00;
    /* Configurar el timer3 */
    T3CONbits.TMR3ON = 0;    // Apagamos el Timer3
    T3CONbits.TMR3CS = 0;    // Seleccionamos el FOSC/4
    T3CONbits.T3CKPS = 0b11; // Prescaler a 1:8
    T3CONbits.RD16   = 1;    // 16bits de acceso
    
    /* Calculamos el valor del TMR3 para 2ms */
    /* Para ello, aplicamos la siguiente fórmula
     * TMR3(T) = 2^nbits - ((T * Fosc) / (4 * Prescaler))
     * Para este caso:
     * nbits = 16   Fosc = 8mhz   Prescaler = 1:8   T = 2ms
     * TMR3 = 2^16 - ((2ms * 8mhz)/(4 * 8))
     * TMR3 = 65536 - ((2*10^-3 * 8*10^6)/(4 * 8))
     * TMR3 = 65036
     */
    TMR3 = 65036;
    
    /* Encendemos el timer */
    T3CONbits.TMR3ON = 1;
}

void Init_Timer3_Interrupts (void)
{
    /* Limpiar los registros */
    RCON   = 0x00;
    INTCON = 0x00;
    PIR2   = 0x00;
    PIE2   = 0x00;
    
    /* Configurar las interrupciones del timer3 */
    RCONbits.IPEN   = 0; // Sin prioridades
    INTCONbits.GIE  = 1; // Interrupciones globales
    INTCONbits.PEIE = 1; // Interrupciones de perifericos
    
    PIR2bits.TMR3IF = 0; // Bandera del timer3 apagada
    PIE2bits.TMR3IE = 1; // Interrupción del timer3 encendida
}

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00;
    
    /* Configurar el intosc a 8MHZ */
    OSCCONbits.IRCF = 0b111; // Freq 8mhz
    OSCCONbits.SCS = 0b11; // Intosc
}

