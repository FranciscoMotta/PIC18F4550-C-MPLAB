/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on 2 de junio de 2023, 02:47 PM
 */

#define _XTAL_FREQ 4000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "system_config.h"

/* Variables globales */

uint16_t val_compi = 0;

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    if(PIR1bits.TMR1IF == 1)
    {
        PIR1bits.TMR1IF = 0;
    }
    
    if (PIR2bits.CCP2IF == 1)
    {
        val_compi = CCPR2; // Capturamos el valor
        TMR1L = 0;
        TMR1H = 0;
        PIR2bits.CCP2IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_CCP_Interrupt (void);
void Init_Capture_Module (void);

/* Main */
int main(void)
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    /* Configurar el módulo de captura */
    Init_Capture_Module();
    /* Configurar el módulo de captura */
    Init_CCP_Interrupt();
    /* Bucle principal */
    while(true)
    {
        // Con el dato capturado hacemos algo
        __nop(); // Hacemos el ese algo
        val_compi = 0;
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_CCP_Interrupt (void)
{
    /* Se limpian los registros */
    INTCON = 0x00;
    RCON = 0x00;
    PIE1 = 0x00;
    PIE2 = 0x00;
    
    /* Se configura las interrupciones */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Interrupciones globales
    INTCONbits.PEIE = 1; // Interrupciones de periféricos
    PIE1bits.TMR1IE = 1;
    PIE2bits.CCP2IE = 1;
    PIR1bits.TMR1IF = 0;
    PIR2bits.CCP2IF = 0; 
}

void Init_Capture_Module (void)
{
    /* Limpiamos los registros */
    T1CON = 0x00;
    CCP1CON = 0x00;
    /* Configuramos el timer 1 */
    T1CONbits.RD16   = 1; // A 16bits
    T1CONbits.T1CKPS = 0b11; // Prescaler a 8
    T1CONbits.TMR1ON = 1; // Timer1 encendido

    /* Configuramos el CCP1 */
    CCP1CONbits.CCP1M = 0b0101; // Captura en flanco de subida
}

void Init_Internal_Oscillator (void)
{
    /* Se limpia los registros */
    OSCCON = 0x00;
    /* Se configura el oscilador interno */
    OSCCONbits.IRCF = 0b110; // 4Mhz
    OSCCONbits.SCS = 0b11; // Intosc
}
