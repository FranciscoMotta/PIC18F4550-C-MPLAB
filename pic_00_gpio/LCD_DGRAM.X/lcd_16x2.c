/* Includes */

#include "lcd_16x2.h"

/* Definicion de funciones */

void Init_Lcd_Gpio (void)
{
    lcd_port_data_tris = 0x0F; // Puerto de datos como salida
    lcd_rs_tris_pin = 0; // Pin de RS como salida
    lcd_en_tris_pin = 0; // Pin de ENABLE como salida
    lcd_rw_tris_pin = 0; // Pin de RW como salida
}

void Lcd_Send_Nibble (char byte)
{
    char upper_bits, lower_bits = 0x00;
    upper_bits = byte & 0xF0;
    lower_bits = (byte & 0x0F) << 4;
    /* Sacamos los datos por el puerto */
    
    // Mandamos los bits MSB
    lcd_port_data_lat |= upper_bits;
    lcd_enable_tog(); // Secuencia de enable
    lcd_port_data_lat &= ~upper_bits;
    
    // Mandamos los bits LSB
    lcd_port_data_lat |= lower_bits;
    lcd_enable_tog(); // Secuencia de enable
    lcd_port_data_lat &= ~lower_bits;
} 

void Lcd_Send_Command (char command)
{
    lcd_control_mode();
    Lcd_Send_Nibble(command);
}

void Lcd_Send_Character (char character)
{
    lcd_charact_mode();
    Lcd_Send_Nibble(character);
}

bool Lcd_Driver_Busy (void)
{
    /* Colocamos el PB7 como entrada */
    lcd_busy_pin_tris = 1;
    /* Leemos el valor del RB7 */
    bool driver_busy_flag = lcd_busy_pin_port;
    /* Devolvemos el puerto a salida */
    lcd_busy_pin_tris = 0;
    /* Devolvemos el valor */
    return driver_busy_flag;
}

void Init_Lcd_16x2 (void)
{
    /* Se inicia el GPIO del LCD */
    Init_Lcd_Gpio();
    lcd_write_mode();
    /* Se envía los comandos de config */
    Lcd_Send_Nibble(COMMAND_LCD_CLEAR);
    Lcd_Send_Nibble(COMMAND_LCD_RET_HOME);
    Lcd_Send_Nibble(COMMAND_FUNCTION_SET);
    Lcd_Send_Nibble(COMMAND_LCD_ENTRY_MODE);
    Lcd_Send_Nibble(COMMAND_LCD_DON_COFF_BOFF);
}