/* 
 * File:   main.c
 * Author: jmott
 *
 * Created on April 23, 2023, 9:39 PM
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

#include "system_config.h"

/*
 * Macros
 */

#define User_Led_Gpio   0
#define User_Led_Tris   TRISD
#define User_Led_Lat    LATD

/*
 * Main
 */
int main(void) 
{
    User_Led_Tris &= ~(1 << User_Led_Gpio); // Pin como salida
    User_Led_Lat &= ~(1 << User_Led_Gpio); // Led prendido
    while(1)
    {
        User_Led_Lat ^= (1 << User_Led_Gpio);
        __delay_ms(250);
    }
    return (EXIT_SUCCESS);
}

