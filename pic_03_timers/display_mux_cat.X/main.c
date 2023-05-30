/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on 30 de mayo de 2023, 11:30 AM
 */

#define _XTAL_FREQ 8000000UL

/* Includes */

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "system_config.h"

/* variables globales */

uint8_t timer_counter;

/* Rutina de servicio a interrupciones */

void __interrupt()  TIMER0_ISR (void)
{
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF)
    {
        /* Rutina */
        timer_counter++;
        /* Cargamos el dato al TMR0 */
        TMR0L = 6;
        /* Limpiamos la bandera */
        INTCONbits.TMR0IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Timer0_Interrupt (void);
void Init_Gpio_System (void);
void Init_Timer0_As_Timer (void);

/* Main */
int main(void) 
{
    /* Configuramos el reloj interno */
    Init_Internal_Oscillator();
    /* Configuramos las interrupciones */
    Init_Timer0_Interrupt();
    /* Configuramos los GPIOs del sistema */
    Init_Gpio_System();
    /* Configuramos el timer0 como timer */
    Init_Timer0_As_Timer();
    while(true)
    {
        uint8_t mux_control = (timer_counter & 0x03);
        LATD = (1 << mux_control);
        switch(mux_control)
        {
            case 0:
                break;
            case 1:
                break;
            case 2:
                break;
            default:
                break;
        }
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer0_As_Timer (void)
{
    /* Limpiamos los registros */
    T0CON = 0x00;
    
    /* Configuramos */
    T0CONbits.TMR0ON = 0; // Apagamos para configurar el timer0
    T0CONbits.T08BIT = 1; // timer a 8 bits
    T0CONbits.T0CS = 0; // Reloj interno
    T0CONbits.T0SE = 0; // Flanco, no importa cuando trabaja FOSC
    T0CONbits.PSA = 0; // Asignamos el prescaler
    T0CONbits.T0PS = 0b010; // Prescaler a 8
    
    /* Cargamos el registros TMR0
     * para este caso, para los 1ms sería TMR0 = 6 */
    TMR0L = 6;
    
    /* Encendemos el timer0 */
    T0CONbits.TMR0ON = 1;
}

void Init_Gpio_System (void)
{
    /* Puerto B como salida */
    TRISB = 0x00;
    /* 4 bits lsb del puerto D como salida para los transistores */
    TRISD &= ~0x0F;
    
    /* Apagamos los puertos */
    LATB = 0x00;
    LATD &= ~0x0F;
}

void Init_Timer0_Interrupt (void)
{
    /* Limpiamos los registros */
    RCON = 0x00;
    INTCON = 0x00;
    
    /* Configuramos ints timer0 */
    RCONbits.IPEN = 0; // Sin prioridade
    INTCONbits.GIE = 1; // Int globales activadas
    INTCONbits.PEIE = 1; // Int perif activadas < No es necesario
    INTCONbits.TMR0IE = 1; // Int timer0 activada
    INTCONbits.TMR0IF = 0; // bandera del timer0 limpiada
}

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00;
    
    /* Configuramos el reloj interno a 8mhz */
    OSCCONbits.IRCF = 0b111; // Fosc 8Mhz
    OSCCONbits.SCS = 0b11; // INTOSC
}