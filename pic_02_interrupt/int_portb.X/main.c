/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 24, 2023, 5:25 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"

/* Macros */

#define dms(x)  __delay_ms(x)

/* Rutina de sericio a interrupciones */

void __interrupt()  RBPI_ISR (void)
{
    if(INTCONbits.RBIE && INTCONbits.RBIF)
    {
        /* Rutina */
        LATD |= ~PORTB;
        /* Limpiamos la bandera */
        INTCONbits.RBIF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_Interrupt_Config (void);
/* Main */
int main(void)
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configura el GPIO del sistema */
    Init_Gpio_System();
    /* Se configura las interrupciones */
    Init_Interrupt_Config ();
    while(true)
    {
        LATCbits.LATC0 = !LATCbits.LATC0;
        dms(250);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */


void Init_Interrupt_Config (void)
{
    /* Se limpian los registros a usar */
    RCON = 0x00;
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    
    /* Se configura la interrupción del PUERTOB */
    RCONbits.IPEN = 0; // Prioridades desactivadas
    INTCONbits.GIE = 1; // Interrupciones globales activadas
    INTCONbits.RBIE = 1; // PortB int activada
    INTCONbits.RBIF = 0; // Bandera limpiada
    INTCON2bits.nRBPU = 0; // pull-ups activadas
    
}

void Init_Gpio_System (void)
{
    /* Puerto B como entrada */
    TRISB = 0xFF; 
    
    /* Puerto D como salida */
    TRISD = 0x00;
    
    /* LED EN RC0 */
    TRISCbits.RC0 = 0;
}

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros a usar */
    OSCCON = 0x00;
    
    /* Configuramos el cristal interno */
    OSCCONbits.IRCF = 0b111; // Frecuencia de 8MHZ
    OSCCONbits.SCS = 0b11; // Oscilador interno
}

