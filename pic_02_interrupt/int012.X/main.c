/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 23, 2023, 7:21 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"

/* Rutina de servicio a interrupciones */

void __interrupt() INT012_ISR (void)
{
    if(INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        /* Rutina */
        LATD &= ~0x0F;
        LATD |= 0x01; // Mandamos un 1 al disp 7 seg
        /* Limpiamos la bandera */
        INTCONbits.INT0IF = 0; 
    }
    if(INTCON3bits.INT1IE && INTCON3bits.INT1IF)
    {
        /* Rutina */
        LATD &= ~0x0F;
        LATD |= 0x02; // Mandamos un 1 al disp 7 seg
        /* Limpiamos la bandera */
        INTCON3bits.INT1IF = 0; 
    }
    if(INTCON3bits.INT2IE && INTCON3bits.INT2IF)
    {
        /* Rutina */
        LATD &= ~0x0F;
        LATD |= 0x03; // Mandamos un 1 al disp 7 seg
        /* Limpiamos la bandera */
        INTCON3bits.INT2IF = 0; 
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_External_Interrupt012 (void);
/* Main */
int main(void) 
{
    /* Se configura el reloj interno */
    Init_Internal_Oscillator();
    /* Se configura el GPIO del sistema*/
    Init_Gpio_System();
    /* Se configura las interrupciones externas del pic */
    Init_External_Interrupt012();
    while (true) 
    {
        LATDbits.LATD4 = !LATDbits.LATD4;
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_External_Interrupt012 (void)
{
    /* Se limpian los registros */
    RCON = 0x00;
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    
    /* Se configuran las interrupciones 012 */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Interrupciones globales activadas
    
    /* INT0 */
    INTCONbits.INT0IE   = 1; // int0 activada
    INTCONbits.INT0IF   = 0; // bandera int0 apagada
    INTCON2bits.INTEDG0 = 1; // int0 por flanco de subida
    
    /* INT1 */
    INTCON3bits.INT1IE  = 1; // int1 activada
    INTCON3bits.INT1IF  = 0; // bandera int1 apagada
    INTCON2bits.INTEDG1 = 1; // int1 por flanco de subida
    
    /* INT2 */
    INTCON3bits.INT2IE  = 1; // int2 activada
    INTCON3bits.INT1IF  = 0; // bandera int2 apagada
    INTCON2bits.INTEDG2 = 1;// int2 por flanco de subida
}

void Init_Gpio_System (void)
{
    /* D0 D1 D2 D3 D4 como salida para el 74LS47 */
    TRISD  = 0xE0;
    /* Nota:
     * Los bits D3:D0 irán conectados al 74ls47 para display 7seg
     * mientras que el D4 irá conectado al LED del sistema
     * el cual parpaderará de manera constante a 100ms  
     */
    
    /* RB0 RB1 y RB2 como entradas */
    TRISBbits.RB0 = 1;
    TRISBbits.RB1 = 1;
    TRISBbits.RB2 = 1;
}

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    
    /* Se configura el reloj interno a 8MHZ */
    OSCCONbits.IDLEN = 1; // CPU on
    OSCCONbits.IRCF = 0b111; // Frecuencia a 8MHZ
    OSCCONbits.SCS = 0b11; // Clock interno elegidO
}