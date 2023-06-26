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

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    
}

/* Variables globales y tipos de datos */

typedef enum
{
    CCP1_AS_PWM_OFF = 0,
    CCP1_AS_PWM_ON
}ccp1_status_t;

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_CCP1_As_PWM (void);
void Init_Timer2_Module (void);
void Run_CPP1_As_PWM (ccp1_status_t status);

/* < Nota:
 * El presente proyecto trata acerca de controlar
 * un motor DC por medio de PWM, para ello, se usará
 * el CCP1 configurado a un periodo de 1 / 10khz.
 * Además, se tendrá el control sobre el sentido de giro 
 * del mismo por medio de un pin RD0. 
 * Se usará el módulo L298D y un motor DC de 5-12V
 */

/* Main */
int main(void)
{
    /* Configurar el oscilador interno a 8MHZ */
    Init_Internal_Oscillator();
    /* Configurar el Timer2 */
    Init_Timer2_Module();
    /* Configurar el CCP1 como PWM */
    Init_CCP1_As_PWM();
    /* Apagamos el PWM por mientras */
    Run_CPP1_As_PWM(CCP1_AS_PWM_OFF);
    /* Calculamos el valor de CCPR2L para un 0% */
    CCPR1L = (uint8_t)((float)(PR2)*(0.0 / 100.0));
    /* Encendemos el PWM */
    Run_CPP1_As_PWM(CCP1_AS_PWM_ON);
    /* Led del sistema */
    TRISDbits.RD0 = 0;
    LATDbits.LATD0 = 0;
    uint8_t val_percent = 0;
    /* Bucle principal */
    while(true)
    {
        CCPR1L = (uint8_t)((float)(PR2)*(val_percent*10.0 / 100.0));
        __delay_ms(3000);
        if(val_percent++ == 10)
        {
            val_percent = 0;
            LATDbits.LATD0 = !LATDbits.LATD0;
        }
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Run_CPP1_As_PWM (ccp1_status_t status)
{
    if(status == CCP1_AS_PWM_ON)
    {
        T2CONbits.TMR2ON = 1;
    }
    else
    {
        T2CONbits.TMR2ON = 0;
    }
}

void Init_Timer2_Module (void)
{
    /* Limpiar los registros */
    T2CON = 0x00;
    TMR2 = 0x00;
    PR2 = 0x00;
    
    /* Configuramos el timer2 */
    T2CONbits.TMR2ON = 0; // Timer apagado
    T2CONbits.T2CKPS = 0b01; // Prescaler a 16
    
    /* Se calcula PR2 a 10khz:
     * Considerando:
     * - Prescaler = 4
     * - Periodo = 1 / 10khz
     * - Tosc = 125ns
     * 
     * PR2 = (periodo / (4 * prescaler * Tosc)) - 1
     * PR2 = (0.1ms/(4 * 4 * 125ns)) - 1
     * PR2 = 49
     */
    PR2 = 49; // <- valor para los 10KHZ
}

void Init_CCP1_As_PWM (void)
{
    /* Limpiar los registros */
    CCP1CON = 0x00;
    
    /* Configurar el CCP1 como pwm */
    CCP1CONbits.CCP1M = 0b1100;
    
    /* Limpiamos los valores de los bits LSB del DC */
    CCP1CONbits.DC1B = 0b00;
    
    /* Pin del CCP1 como salida */
    TRISCbits.RC2 = 0;
}

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    /* Oscilador a 8mhz */
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
}