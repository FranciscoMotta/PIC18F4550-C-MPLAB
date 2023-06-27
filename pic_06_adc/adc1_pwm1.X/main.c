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


/* Rutinas de servicio a interrupciones */

void __interrupt() ISR (void)
{
    
}

/* Declaración de funciones */

void Init_Internal_Oscillator (void);

/* Main */
int main(void)
{
    /* Configurar el reloj intenro */
    Init_Internal_Oscillator();
    /* Configurar la LCD */
    FM_Lcd_Easy_Init();
    /* Mostrar mensaje */
    FM_Lcd_Set_Cursor(ROW_1, COL_4);
    FM_Lcd_Send_String("PWM - MOTOR");
    /* Bucle principal */
    while(true)
    {
        __nop();
    }
    return (EXIT_SUCCESS);
}

/* Definición de funciones */

void Init_Internal_Oscillator (void)
{
    /* Limpiar los registros */
    OSCCON = 0x00;
    
    /* Reloj interno a 8MHz*/
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b11;
}