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

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_CCP1_Module (void);
void Init_CCP2_Module (void);
void Init_Timer2_Module (void);

/* Main */
int main(void)
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    /* Configurar el TIMER2 a un prescaler de 16 */
    Init_Timer2_Module();
    /* Configurar el CCP1 como PWM al 50% */
    Init_CCP1_Module();
    /* Configurar el CCP2 como PWM al 80% */
    Init_CCP2_Module();
    /* Led del sistema */
    TRISDbits.RD0 = 0;
    LATDbits.LATD0 = 0;
    /* Bucle principal */
    while(true)
    {
        LATD ^= (1 << _LATD_LATD0_POSITION);
        __delay_ms(250);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Timer2_Module (void)
{
    /* Limpiar los registros */
    T2CON = 0x00;
    TMR2 = 0x00;
    
    /* Configurar el timer2 */
    T2CONbits.TMR2ON = 0; // Timer2 apagado
    T2CONbits.T2CKPS = 0b11; // Prescaler a 16
    
    /* Establecer el periodo en el registros PR2 
     * Para lo cual se considera lo siguiente:
     * 
     * PR2 = (periodo / (4 * Tosc * prescaler)) - 1
     * 
     * Además:
     * 
     * Periodo = 1ms
     * Tosc = 125ns
     * Prescaler = 16
     * 
     * Entonces: 
     * 
     * PR2 = (1ms / (4 * 125ns * 16)) - 1
     * PR2 = 124
     * 
     */
    PR2 = 124;
    
    /* Encender el timer 2 */
    T2CONbits.TMR2ON = 1;
}

void Init_CCP1_Module (void)
{
    /* Limpiar los registros */
    CCP1CON = 0x00;
    
    /* Configurar el CCP1 como PWM */
    CCP1CONbits.CCP1M = 0b1100;
    
    /* Configuraremos el DC del PWM1 a 50% 
     * Considerando lo siguiente:
     * Prescaler_TMR2 = 16
     * Tosc = 125ns
     * Periodo = 1ms
     * 
     * (CCPR1L : CCP1CON<5:4>) = (DC / (Tosc * Prescaler))
     * (CCPR1L : CCP1CON<5:4>) = (0.5*ms / (125ns * 16))
     * (CCPR1L : CCP1CON<5:4>) = 250 DEC =  0b00 1001 0110 BIN
     */
    
    /* Cargar los valores a los registros correspondientes */
    CCPR1L = 0b00111110;
    CCP1CONbits.DC1B = 0b00;
    
    /* Pin del CCP1 como salida */
    TRISCbits.RC2 = 0;
}

void Init_CCP2_Module (void)
{
    /* Limpiar los registros */
    CCP2CON = 0x00;
    /* Configurar el CCP2 como PWM */
    CCP2CONbits.CCP2M = 0b1100;
    /* Configuraremos el DC del PWM2 a 80% 
     * Considerando lo siguiente:
     * Prescaler_TMR2 = 16
     * Tosc = 125ns
     * Periodo = 1ms
     * 
     * (CCPR2L : CCP2CON<5:4>) = (DC / (Tosc * Prescaler))
     * (CCPR2L : CCP2CON<5:4>) = (0.8*ms / (125ns * 16))
     * (CCPR2L : CCP2CON<5:4>) = 400 DEC =  0b01 1001 0000 BIN
     */
    CCPR2L = 0b01100100;
    CCP2CONbits.DC2B = 0b00;
    
     /* Pin del CCP2 como salida */
    TRISCbits.RC1 = 0;
}

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    /* Oscilador interno a 8mhz */
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
}