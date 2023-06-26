/* 
 * File:   FM_LCD16X2_Easy.h
 * Author: jmott
 *
 * Created on April 23, 2023, 10:46 PM
 */

#ifndef FM_LCD16X2_EASY_H
#define	FM_LCD16X2_EASY_H

#include <xc.h>
#include <stdint.h>

#ifndef _XTAL_FREQ
#define _XTAL_FREQ 8000000UL
#endif

/* Pines de control */

/*
 * La disposici�n del control de este LCD
 * a trav�s de un solo puerto, es decir, los
 * pines de control como los pines de datos 
 * estar�n en un puerto definido por el usuario
 */

#define PORT_LCD_CONTROL

#ifdef PORT_LCD_CONTROL
#define Lcd_Data_Ctrl_Lat  LATD
#define Lcd_Data_Ctrl_Tris  TRISD
#else
#warning "Tienes que definir tu puerto del LCD"
#define Lcd_Data_Ctrl_Lat  LATX <--- El puerto que uses
#define Lcd_Data_Ctrl_Tris  TRISX
#endif

/*
 * Ac� definimos que los bits de datos estar�n
 * en los 4 bits m�s significativos del puerto elegido
 * y los pines de control RS, RW y E en los menos significativos
 */

#define FOUR_BITS_HIGHER               

#ifdef FOUR_BITS_HIGHER
#define Lcd_Rs_Pin          1  
#define Lcd_Rw_Pin          2  
#define Lcd_En_Pin          3  
#else
#define Lcd_Rs_Pin          5
#define Lcd_Rw_Pin          6
#define Lcd_En_Pin          7
#endif

#define Bits_Control_Mask   ((1 << Lcd_Rs_Pin)|(1 << Lcd_Rw_Pin)|(1 << Lcd_En_Pin))

/*
 * Acciones de control del LCD
 */

#define Lcd_Control_Mode()      Lcd_Data_Ctrl_Lat &= ~(1 << Lcd_Rs_Pin);
#define Lcd_Character_Mode()    Lcd_Data_Ctrl_Lat |= (1 << Lcd_Rs_Pin);

#define En_On()                 Lcd_Data_Ctrl_Lat |= (1 << Lcd_En_Pin);
#define En_Off()                Lcd_Data_Ctrl_Lat &= ~(1 << Lcd_En_Pin);
#define Lcd_Enable_Pulse()      En_On(); __delay_ms(1); En_Off(); __delay_ms(1);

/* Posiciones de cursores */

typedef enum
{
    ROW_1 = 0,
    ROW_2
}_row_lcd_t;

typedef enum
{
    COL_1 = 0,
    COL_2,            
    COL_3,            
    COL_4,            
    COL_5,            
    COL_6,            
    COL_7,            
    COL_8,            
    COL_9,            
    COL_10,            
    COL_11,            
    COL_12,            
    COL_13,            
    COL_14,            
    COL_15            
}_column_lcd_t;

/* Declaracion de funciones */

void FM_Lcd_Easy_Gpio_Init (void);
void FM_Lcd_Send_Nibble (char byte_to_send);
void FM_Lcd_Send_Command (char command_to_send);
void FM_Lcd_Send_Character (char character_to_send);
void FM_Lcd_Easy_Init (void);
void FM_Lcd_Send_String (char *cadena);
void FM_Lcd_Set_Cursor (_row_lcd_t filas, _column_lcd_t columna);
#endif	/* FM_LCD16X2_EASY_H */

