/*
 * lcd_driver.h
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include <stdint.h>

// LCD control pin definitions
#define LCD_CTL P4->OUT
#define LCD_PIN_MASK 0xFE
#define LCD_DATA_MASK 0xF0
#define LCD_EN_BIT BIT3
#define LCD_RS_BIT BIT1
#define LCD_RW_BIT BIT2

// LCD instruction format definitions
#define WAKE_UP 0x30
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x02
#define ENTRY_MODE_SET 0x06
#define DISPLAY_ON 0x0F
#define CURSOR_SHIFT 0x10
#define FUNCTION_SET 0x2F
#define SET_CGRAM_ADDR 0x20
#define SET_DDRAM_ADDR 0x40

void Clear_LCD(); // clear the display
void Home_LCD(); // move the cursor to the top left of the LCD
void Write_char_LCD(char to_write); // write a character to the LCD
void Write_command_LCD(uint8_t command); // write an instruction to the LCD
void Write_string_LCD(char *str, uint32_t size); // write a string to the LCD
void Initialize_LCD(); // run initialization commands on LCD


#endif /* LCD_DRIVER_H_ */
