/* Includes */

#include "lcd_16x2.h"

/* Definicion de funciones */

void Init_Lcd_Gpio (void)
{
    lcd_port_data_tris = 0x00; // Puerto de datos como salida
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
    Lcd_Send_Command(COMMAND_LCD_CLEAR);
    Lcd_Send_Command(COMMAND_LCD_RET_HOME);
    Lcd_Send_Command(COMMAND_FUNCTION_SET);
    Lcd_Send_Command(COMMAND_LCD_ENTRY_MODE);
    Lcd_Send_Command(COMMAND_LCD_DON_COFF_BOFF);
    Lcd_Send_Command(COMMAND_LCD_RET_HOME);
}

void Lcd_Set_Cursor_XY (_row_t fila, _column_t columna)
{
    Lcd_Send_Command(fila + columna);
}

void Lcd_Send_String (char *cadena)
{
    uint16_t string_index = 0;
    while(cadena[string_index] != '\0')
    {
        Lcd_Send_Character(cadena[string_index]);
        string_index++;
    }
}

void Lcd_Shift_Text (_right_left_mov_t dir)
{
    volatile uint8_t shift_com = 0x10;
    if(dir == SHIFT_RIGHT)
    {
        shift_com |= (1 << 2); // Pin RL en 1
    }
    else
    {
        __nop();
    }
    shift_com ^= (1 << 3); // Hacemos una xor al pin SC
    Lcd_Send_Command(shift_com); // Mandamos el comando
}

void Lcd_Save_Character (_dir_to_save_t dir, char *caracter)
{
    Lcd_Send_Command(0X40 + dir);
    uint8_t index;
    for(index = 0 ; index < 8 ; index++)
    {
        Lcd_Send_Character(caracter[index]);
    }
    Lcd_Send_Command(0x80);
}