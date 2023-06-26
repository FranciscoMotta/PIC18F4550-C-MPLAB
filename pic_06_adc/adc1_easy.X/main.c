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
void Init_ADC_Module (void);

/* Main */
int main(void)
{
    /* Configurar el oscilador interno */
    Init_Internal_Oscillator();
    /* Configurar el ADC */
    Init_ADC_Module();
    /* Configurar el GPIO */
    TRISD = 0x00;
    LATD = 0x00;
    /* Bucle principal */
    while(true)
    {
        /* Iniciar la conversi�n */
        ADCON0bits.GO_NOT_DONE = 1;
        /* Esperar a que termine la conversi�n */
        while(ADCON0bits.GO_NOT_DONE);
        /* Usar los datos le�dos */
        LATD = ADRESH;
        /* Dar un tiempo */
        __delay_ms(1);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_ADC_Module (void)
{
    /* Limpiar los registros */
    ADCON0 = 0x00;
    ADCON1 = 0x00;
    ADCON2 = 0x00;
    
    /* Configuramos el ADC */
    ADCON0bits.ADON = 0;      // Apagamos el ADC
    ADCON0bits.CHS  = 0b0000; // Selecionar el Canal 0
    
    ADCON1bits.VCFG = 0b00;   // Voltaje de referencia VDD y GND
    ADCON1bits.PCFG = 0b1110; // Solo en Canal 0 como anal�gico
    
    ADCON2bits.ADFM = 0;      // Justificaci�n a la izquierda
    ADCON2bits.ADCS = 0b001;  // Fadc = Fosc / 8 = 8mhz / 8 = 1mhz
    
    /* Por lo tanto:
     * Tad = 1 / Fadc = 1 / 1mhz = 1us
     * Tad = 1us, por lo tanto se debe 
     * garantizar los 6.4us m�nimos
     * Por lo tanto: ADQTIME > 6.4us
     *  */
    
    ADCON2bits.ACQT = 0b110;  // 16 TAD
    
    /* Encendemos el ADC */
    ADCON0bits.ADON = 1;
}

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    
    /* Oscilator interno a 8mhz */
    OSCCONbits.IRCF = 0b111; // 8mhz
    OSCCONbits.SCS = 0b11; // INTOSC
}