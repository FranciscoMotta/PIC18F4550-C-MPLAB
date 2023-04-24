/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on April 23, 2023, 10:38 PM
 */

/*
 * Includes
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "system_config.h"
#include "project_defines.h"
#include "FM_LCD16X2_Easy.h"

/*
 * Macros
 */

/*
 * Declaracion de funciones
 */

void Init_Led_System (void);

/*
 * Main
 */
int main(void) 
{
    Init_Led_System(); // Iniciamos el led
    FM_Lcd_Easy_Init(); // Iniciamos el LCD
    FM_Lcd_Set_Cursor(ROW_2, COL_1);
    FM_Lcd_Send_String("HOLA");
    while(1)
    {
        Led_System_Lat ^= Led_System_Mask;
        __delay_ms(100);
    }
    return (EXIT_SUCCESS);
}

/*
 * Definicion de funciones
 */

void Init_Led_System (void)
{
    // Led del sistema como salida
    Led_System_Tris  &= ~Led_System_Mask;
    Led_System_Lat &= ~Led_System_Mask;  
}