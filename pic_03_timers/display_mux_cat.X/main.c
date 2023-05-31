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
//                      H             O            L            A
char mensaje[4] = {~0b00110111, ~0b01111110, ~0b00001110, ~0b01110111};

// Valores de los números decoficados a display de 7 segmentos
char num_disp[10] = {0b01111110, // 0
                     0b00110000, // 1
                     0b01101101, // 2
                     0b01001111, // 3
                     0b00100111, // 4
                     0b01011011, // 5
                     0b01011111, // 6
                     0b01000110, // 7
                     0b01111111, // 8
                     0b01101111};// 9

uint8_t num_decode[4];

/* Rutina de servicio a interrupciones */

void __interrupt()  TIMER0_ISR (void)
{
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF)
    {
        /* Rutina */
        if (timer_counter++ == 3) 
        {
            timer_counter = 0;
        }

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

/* Funciones de decodificado del DISPLAY */

void decodifica_0000_9999(uint16_t valor);

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
    /* Bucle principal */
    while(true)
    {
        uint8_t mux_control = (timer_counter & 0x03);
        LATD = ~(1 << mux_control);
        uint16_t valor = 4550;
        // < ALGO ESTÁ MAL PENDIENTE DE REVISARLO
//        num_decode[0] = valor / 1000;
//        num_decode[1] = (valor % 1000) / 100;
//        num_decode[2] = ((valor % 1000) % 100) / 10;
//        num_decode[3] = ((valor % 1000) % 100) % 10;
        LATB = mensaje[mux_control];
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

/*
 * Nota:
 * 
 *      num_decode[0] = valor / 1000;
 *      num_decode[1] = (valor % 1000) / 100;
 *      num_decode[2] = ((valor % 1000) % 100) / 10;
 *      num_decode[3] = ((valor % 1000) % 100) % 10;
 */

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