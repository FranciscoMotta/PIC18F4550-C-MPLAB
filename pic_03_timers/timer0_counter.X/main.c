/*
 * File:   main.c
 * Author: Administrador
 *
 * Created on 29 de mayo de 2023, 08:37 PM
 */

#define _XTAL_FREQ 8000000UL // Frecuencia del cristal

/* Includes */

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "system_config.h"

/* Rutina de servicio a las interrupciones */

void __interrupt() TMR0_ISR (void)
{
    if(INTCONbits.TMR0IF && INTCONbits.TMR0IE)
    {
        /* Rutina */
        LATBbits.LATB1 = !LATBbits.LATB1;
        /* Cargamos el dato de comparación */
        TMR0L = 245; // Debe ser el complemento para el desborde
        /* Limpiamos la bandera */
        INTCONbits.TMR0IF = 0;
    }
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_Interrupt_Timer0 (void);
void Init_Timer0_As_Counter (void);
void Init_Gpio_System (void);

/* Main */
int main(void) 
{
    /* Configuramos el reloj interno */
    Init_Internal_Oscillator();
    /* Configuramos el GPIO del sistema */
    Init_Gpio_System();
    /* Configuramos las interrupciones del timer0 */
    Init_Interrupt_Timer0(); 
    /* Configuramos el timer 0 como contador */
    Init_Timer0_As_Counter();
    /* Bucle principal */
    while(true)
    {
        /* Hacemos parpadear un led */
        LATBbits.LATB0 = !LATBbits.LATB0;
        __delay_ms(250);
    }
    return EXIT_SUCCESS;
}

/* Definición de funciones */

void Init_Gpio_System (void)
{
    /* RB0 y RB1 como salidas */
    TRISBbits.RB0 = 0;
    TRISBbits.RB1 = 0;
    
    /* RB0 y RB1 apagados */
    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    
    /* RA4 como entrada */
    ADCON1 = 0x0F; // Pines digitales
    TRISAbits.RA4 = 1; // Entrada
}

void Init_Timer0_As_Counter (void)
{
    /* Limpiamos los registros */
    T0CON = 0x00;
    
    /* Configuramos el timer0 como contador */
    T0CONbits.TMR0ON = 0; // Lo apagamos para config
    T0CONbits.T08BIT = 1; // Configurado a 8 bits
    T0CONbits.T0CS = 1; // Entrada externa del reloj (generador de pulsos)
    T0CONbits.T0SE = 0; // El flanco activo es del subida
    T0CONbits.PSA = 1; // Prescaler no asignado
    T0CONbits.T0PS = 0; // < NO IMPORTA!!!
    
    /* En este caso, usamos los registros TMR0 para el valor de comparación
     * para este caso quiero que cuente 10 eventos y que llegado a ese 
     * número, se active la interrupción*/
    TMR0L = 245; // Debe ser el complemento para el desborde
    
    /* Encendemos el timer0 */
    T0CONbits.TMR0ON = 1;
}
void Init_Interrupt_Timer0 (void)
{
    /* Limpiamos los registros */
    INTCON = 0x00;
    
    /* Configuramos las interrupciones */
    INTCONbits.GIE = 1; // Interrupciones globales activadas
    INTCONbits.PEIE = 1; // Interrupciones globales
    INTCONbits.TMR0IE = 1; // Interrupciones del timer0
    INTCONbits.TMR0IF = 0; // Bandera del timer0 apagada
}

void Init_Internal_Oscillator (void)
{
    /* Limpiamos los registros */
    OSCCON = 0x00;
    
    /* Configuramos el cristal interno a 8mhz */
    OSCCONbits.IRCF = 0b111; // Freq 8mhz
    OSCCONbits.SCS = 0b11; // Intosc select
}
