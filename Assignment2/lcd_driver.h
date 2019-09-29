/*
 * lcd_driver.h
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include <stdint.h>

#define LCD_CTL P4->OUT
#define LCD_PIN_MASK 0xFE
#define LCD_DATA_MASK 0xF0
#define LCD_EN_BIT 0x08
#define LCD_RS_BIT 0x02
#define LCD_RW_BIT 0x04

#define WAKE_UP 0x30
#define CLEAR_DISPLAY 0x01
#define RETURN_HOME 0x0
#define ENTRY_MODE_SET 0x06
#define DISPLAY_ON 0x0F
#define CURSOR_SHIFT 0x10
#define FUNCTION_SET 0x23
#define SET_CGRAM_ADDR 0x20
#define SET_DDRAM_ADDR 0x40

void Clear_LCD(); // clear the display
void Home_LCD(); // move the cursor to the top left of the LCD
void Write_char_LCD(char to_write, uint32_t line, uint32_t col); // write a character to the LCD
void Write_command_LCD(uint8_t command);
void Initialize_LCD(); // run initialization commands on LCD


#endif /* LCD_DRIVER_H_ */
