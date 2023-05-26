/* 
 * File:   lcd_16x2.h
 * Author: Administrador
 *
 * Created on May 25, 2023, 5:15 PM
 */

#ifndef LCD_16X2_H
#define	LCD_16X2_H

#include <xc.h>
#include "system_config.h"

/* Notas:
 * La presente será una librería más completa de las que 
 * usualmente uso en los códigos que subo al respecto,
 * pero estará limitada a usar los 4 bits MSB de un determinado 
 * puerto E/S de un microcontrolador. Asimismo, los bits de 
 * control estarán en los bits LSB de un puerto E/S del
 * microcontrolador.
 * 
 * Nota2: No soy fan de usar el "LATXbits.LATXCY" pero 
 * creo que mucha gente lo entiende mejor así.
 * 
 *  */

#define lcd_rs_lat_pin      LATCbits.LATC0
#define lcd_en_lat_pin      LATCbits.LATC1
#define lcd_rw_lat_pin      LATCbits.LATC2

#define lcd_rs_tris_pin     TRISCbits.RC0
#define lcd_en_tris_pin     TRISCbits.RC1
#define lcd_rw_tris_pin     TRISCbits.RC2

#define lcd_port_data_lat   LATD
#define lcd_port_data_tris  TRISD

/* Modos de control */

#define lcd_control_mode()  lcd_rs_lat_pin = 0;
#define lcd_charact_mode()  lcd_rs_lat_pin = 1;

/* Rutina de enable */

#define en_on()     lcd_en_lat_pin = 1;
#define en_off()    lcd_en_lat_pin = 0;
#define lcd_enable_tog()    en_on(); __delay_ms(1); en_off(); __delay_ms(1);

/* Modos de lectura escritura */

#define lcd_write_mode()    lcd_rw_lat_pin = 0;
#define lcd_read_mode()     lcd_rw_lat_pin = 1;

/* Bandera del estado CPU */

#define lcd_busy_pin_port       PORTDbits.RD7
#define lcd_busy_pin_tris       TRISDbits.RD7

/* Macros de comandos básicos */

#define COMMAND_LCD_CLEAR           0x01 // Comando para borrar la LCD
#define COMMAND_LCD_RET_HOME        0x02 // Comando para retornar al home
#define COMMAND_LCD_ENTRY_MODE      0x07 // Comando para incrementar en 1 el ddram
#define COMMAND_LCD_DON_COFF_BOFF   0x0C // Comando Display On - Cursor Off - Blink Off
#define COMMAND_FUNCTION_SET        0x28 // Comando 4bits op - 2 lines - 5x8 dots

/* Declaración de funciones */

void Init_Lcd_Gpio (void);
void Lcd_Send_Nibble (char byte);
void Lcd_Send_Command (char command);
void Lcd_Send_Character (char character);
bool Lcd_Driver_Busy (void);

// Funciones de configuración
void Init_Lcd_16x2 (void);

#endif	/* LCD_16X2_H */

