/* 
 * File:   main.c
 * Author: Administrador
 *
 * Created on May 24, 2023, 7:03 PM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "system_config.h"

/* Macros */

/* Variables globales */

uint8_t lookup_table[] = {7, 8, 9, 0,
                          4, 5, 6, 0,
                          1, 2, 3, 0,
                          0, 0, 0, 0};

/* Rutina de servicio a interrupciones */

void __interrupt() RPBI_ISR (void)
{
    if(INTCONbits.INT0IE && INTCONbits.INT0IF)
    {
        /* Rutina */
        uint8_t num_rec = (PORTD & 0xF0) >> 4;
        LATD = lookup_table[num_rec];
        /* Limpiamos la bandera */
        INTCONbits.INT0IF = 0;
    }
}
/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_External_InterruptB (void);
void Init_Gpio_System (void);

/* Main */
int main(void)
{
    /* Se configura el oscilador interno */
    Init_Internal_Oscillator();
    /* Se configura la interrupción por cambio de estado del PB */
    Init_External_InterruptB();
    /* Se configura los GPIO del sistema como salida */
    Init_Gpio_System();
    /* Bucle principal */
    while(true)
    {
        /* Hacemos el parpadeo del LED */
        LATCbits.LATC0 = !LATCbits.LATC0;
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Gpio_System (void)
{
    /* Bits <RD7 : RD4> como entrada 
     * Bits <RD3 : RD0> como salida
     * Bit  <RB0> como entrada
     */
    TRISD = 0xF0;
    TRISBbits.RB0 = 1; 
}

void Init_External_InterruptB (void)
{
    /* Se limpian los registros a usar */
    RCON = 0x00;
    INTCON = 0x00;
    INTCON2 = 0x00;
    INTCON3 = 0x00;
    
    /* Se configura la interrupción por cambio de <RB7 : RB4> */
    RCONbits.IPEN = 0; // Sin prioridades
    INTCONbits.GIE = 1; // Interrupciones globales activadas
    INTCONbits.INT0IE = 1; // Int0 habilitada
    INTCONbits.INT0IF = 0; // Bandera apagada
    INTCON2bits.INTEDG0 = 1; // Activado por flanco de subida
}

void Init_Internal_Oscillator (void)
{
    /* Se limpian los registros */
    OSCCON = 0x00;
    /* Se configura el reloj interno */
    OSCCONbits.IDLEN = 0; // <- ESTO NO IMPORTA EN REALIDAD
    OSCCONbits.IRCF = 0b111; // Frecuencia a 8MHZ
    OSCCONbits.SCS = 0b11; // INTOSC select
}