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
    
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Capture_Module (void);

/* Main */
int main(void)
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    /* Configurar el módulo de captura */
    
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

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
