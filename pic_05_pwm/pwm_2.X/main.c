/* 
 * File:   main.c
 * Author: Julio Francisco Motta
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

/* Macros */

#define led_sys     LATDbits.LATD0

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Gpio_System (void);
void Init_PWM2_Config (void);

/* Main */
int main(void)
{
    /* < NOTAS DEL DESARROLLADOR:
     * Se debe tener en cuenta el estado del bit
     * de configuración CCP2MX ya que depende de
     * este la salida del módulo CCP2, en este 
     * caso, para que la salida sea por RC1 se 
     * debe de colocar en ON :D 
     */
    /* Configurar el cristal intenro a 8MHZ */
    Init_Internal_Oscillator();
    /* Configurar el GPIO del sistema */
    Init_Gpio_System();
    /* Configurar el PWM */
    Init_PWM2_Config();
    /* Bucle principal */
    while(true)
    {
        led_sys = !led_sys;
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_PWM2_Config (void)
{
    /* Pin del CCP2 como salida */
    TRISCbits.RC1 = 0;
    
    /* Limpiar los registros */
    T2CON   = 0x00;
    CCP2CON = 0x00;
    TMR2    = 0x00;
    
    /* Se desea un periodo de 1ms  = 1000hz a un duty cicle de 50%
     * Considerando lo siguiente:
     * Fosc = 8MHZ
     * Prescaler = 16
     * DC (Duty Cicle) = 50%
     * PR2 = (PeriodoPWM / (4 * prescaler * Tosc)) - 1 
     * PR2 = (1ms / (4 * 16 * 125ns)) - 1;
     * PR2 = 124
     */
    PR2 = 124;
    /* Asimismo, se calcula el valor del (CCPR2 : CCP2CON<5:4>) 
     * (CCPR2 : CCP2CON<5:4>) = DC / (Tosc * Prescaler)
     * Nota: 
     * DC = 50%*Periodo = 0.5ms = 5*10^-4
     * Entonces:
     * (CCPR2L : CCP2CON<5:4>) = 5*10^-4 / (125ns * 16)
     * (CCPR2L : CCP2CON<5:4>) = 250
     * Además:
     * 250 DEC = 0x0FA HEX = 0b0011111010 BIN
     * Por lo tanto:
     * CCPR2L = 0b00111110
     * CCP2CON<5:4> = 0b10
     */
    
    CCPR2L           = 0b00111110;
    CCP2CONbits.DC2B = 0b10;
    
    /* Módulo CCP2 como PWM */
    CCP2CONbits.CCP2M = 0b1111;
    
    /* Configurar el timer2 */
    T2CONbits.T2CKPS = 0b11; // Prescaler 16
    
    /* Encender el timer 2 */
    T2CONbits.TMR2ON = 1;
}

void Init_Gpio_System (void)
{
    /* Configurar el LED RD0 como salida */
    TRISDbits.RD0 = 0;
}

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    /* Cristal interno a 8MHZ */
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
}
