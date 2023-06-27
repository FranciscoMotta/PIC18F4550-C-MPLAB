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
#include "FM_LCD16X2_Easy.h"

/* Variables globales */

char mens[16];

/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    
}

/* Tipos de datos */

typedef enum
{
    MOD_OFF = 0,
    MOD_ON
}_module_ccp1_timer_t;

/* Declaración de funciones */

void Init_Internal_Oscillator (void);
void Init_CCP1_As_PWM (void);
void Init_Timer2_Module (void);
void Run_Timer_And_CCP1PWM (_module_ccp1_timer_t mod);
void Init_ADC1_Chanel_0 (void);

/* Main */
int main(void)
{
    /* Configurar el reloj intenro */
    Init_Internal_Oscillator();
    /* Configurar el timer2 */
    Init_Timer2_Module();
    /* Configurar el CCP1 */
    Init_CCP1_As_PWM();
    /* Configurar la LCD */
    FM_Lcd_Easy_Init();
    /* Encendemos el timer2 */
    Run_Timer_And_CCP1PWM(MOD_ON);
    /* Configurar el ADC */
    Init_ADC1_Chanel_0();
    /* Mostrar mensaje */
    FM_Lcd_Set_Cursor(ROW_1, COL_4);
    FM_Lcd_Send_String("PWM - MOTOR");
    /* Bucle principal */
    while(true)
    {
        ADCON0bits.GO_NOT_DONE = 1; // Iniciamos la conv.
        while(ADCON0bits.GO_NOT_DONE); // Esperamos
        uint16_t adc_val = (ADRESH << 8) | ADRESL;
        float percent = (adc_val / 1024.0) * 100.0;
        /* Mostrar LCD */
        FM_Lcd_Set_Cursor(ROW_2, COL_2);
        sprintf(mens, "Perc. = %.2f%%", percent);
        FM_Lcd_Send_String(mens);
        /* Cargar el valor al CCPR1L */
        CCPR1L = (uint8_t)((float)(PR2)* (percent)/(100.0));
        __delay_ms(1);
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_ADC1_Chanel_0 (void)
{
    /* Limpiar los registros */
    ADCON0 = 0x00;
    ADCON1 = 0x00;
    ADCON2 = 0x00;
    
    /* Configuramos */
    ADCON0bits.ADON = 0; // Apagamos el ADC
    ADCON0bits.CHS = 0b0000; // Elegimos el canal 0
    
    ADCON1bits.VCFG = 0b00; // Voltaje de referencia VDD y GND
    ADCON1bits.PCFG = 0b1110; // Solo RA0 como analógico
    
    /* Pin seleccionado como entrada */
    TRISAbits.RA0 = 1;
    
    ADCON2bits.ADFM = 1; // Just. Derecha [x x x x x x a b] [c d e f g h i j]
    
    /* Considerando Fosc = 8MHz
     * Hacemos:
     * F_ad = Fosc / 8 = 8MHz / 8 = 1Mhz
     * T_ad = 1us
     * 
     * Se debe garantizar un minimo de 6.7us
     * Por lo tanto:
     * Adq_time = 20TAD
     * Adq_time = 20us
     * 
     *  */
    
    ADCON2bits.ACQT = 0b111; // 20TAD
    ADCON2bits.ADCS = 0b001; // Fosc / 8
    
    /* Encender el ADC */
    ADCON0bits.ADON = 1;
}

void Run_Timer_And_CCP1PWM (_module_ccp1_timer_t mod)
{
    if(mod == MOD_ON)
    {
        T2CONbits.TMR2ON = 1;
    }
    else
    {
        T2CONbits.TMR2ON = 0;
    }
}

void Init_CCP1_As_PWM (void)
{
    /* Limpiar los registros */
    CCP1CON = 0x00;
    CCPR1L = 0x00;
    
    /* Configurar el CCP1 como PWM */
    CCP1CONbits.CCP1M = 0b1100; // No 1111 por el ECCPX
    
    /* Ponemos el CCPR1L en 0 al incio */
    CCPR1L = 0;
    
    /* Pin del CCP1 como salida */
    TRISCbits.RC2 = 0;
}

void Init_Timer2_Module (void)
{
    /* Limpiar los registros */
    T2CON = 0x00;
    TMR2 = 0x00;
    PR2 = 0x00;
    
    /* Configuramos el timer2 */
    T2CONbits.TMR2ON = 0; // Timer apago
    T2CONbits.T2CKPS = 0b01; // Prescaler 1:4
    
    /* Calculamos PR2:
     * Considerando:
     * PR2 = (periodo / (4 * Tosc * prescaler_tmr2)) - 1
     * 
     * Datos:
     * periodo = 0.1ms
     * prescaler_tmr2 = 4
     * Tosc = 125ns
     * 
     * Por lo tanto:
     * PR2 = (10^-4 / (4 * 125 * 10^-9 * 4)) - 1
     * PR2 = 49
     *  
     */
    PR2 = 49;
}

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    
    /* Reloj interno a 8MHz*/
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
}