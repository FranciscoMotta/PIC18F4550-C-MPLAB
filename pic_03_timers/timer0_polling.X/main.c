/*
 * File:   main.c
 * Author: Administrador
 *
 * Created on 3 de junio de 2023, 07:11 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "system_config.h"

/* Declaración de funciones */

void Init_Internal_Oscilator (void);
void Init_Timer0_As_Timer (void);

/* Main */

int main(void) 
{
    /* Se configura el oscilador interno */
    Init_Internal_Oscilator();
    /* Se configura el Timer0 como contador */
    Init_Timer0_As_Timer();
    /* RB0 como salida */
    TRISBbits.RB0 = 0;
    /* Variables */
    uint16_t contador_ms = 0;
    /* Bucle principal */
    while(true)
    {
        if(INTCONbits.TMR0IF)
        {
            /* Rutina */
            contador_ms++;
            /* Volvemos a cargar el dato */
            TMR0 = 65286;
            /* Limpiamos la bandera */
            INTCONbits.TMR0IF = 0;
        }
        if(contador_ms == 499)
        {
            LATBbits.LATB0 =! LATBbits.LATB0;
            contador_ms = 0;
        }
    }
    return EXIT_SUCCESS;
}

/* Definición de proyectos */

void Init_Timer0_As_Timer (void)
{
    /* Se limpian los registros */
    T0CON = 0x00;
    /* Se configura el timer0 */
    T0CONbits.TMR0ON = 0; // Apagamos el timer0
    T0CONbits.T08BIT = 0; // Trabajamos a 16bits (nbits)
    T0CONbits.T0CS   = 0; // Elegimos la fuente de reloj de la CPU
    T0CONbits.T0SE   = 0; // Flanco de cuenta para freq externa
    T0CONbits.PSA    = 0; // Seleccionamos el prescaler
    T0CONbits.T0PS   = 0b010; // Elegimos un prescaler de 8 (PRESCALER)
    
    /* Formula:
     * TMR0 = 2^nbits - ((T * FOSC)/(4 * PRESCALER)) 
     * Supongamos que queremos un tiempo de 1ms
     * Entonces:
     * T = 1ms FOSC = 8MHZ PRESCALER = 8
     * TMR0 = 2^16 - ((1*10^-3 * 8 * 10^6)/(4 * 8))
     * TMR0 = 65536 - 250
     * TMR0 = 65286
     */
    TMR0 = 65286;
    /* Encendemos el timer */
    T0CONbits.TMR0ON = 1;
}

void Init_Internal_Oscilator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    /* Se configura el oscilador interno */
    OSCCONbits.IRCF = 0b111; // 8Mhz freq
    OSCCONbits.SCS = 0b11; // Intosc
}