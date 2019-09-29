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
#define LCD_EN_BIT 0x40
#define LCD_RS_BIT 0x20
#define LCD_RW_BIT 0x10

#define WAKE_UP 0x30
#define FUNCTION_SET 0x23
#define SET_CURSOR 0x10
#define DISPLAY_ON 0x0F
#define SET_ENTRY_MODE 0x06

Clear_LCD(); // clear the display
Home_LCD(); // move the cursor to the top left of the LCD
Write_char_LCD(char to_write, uint32_t line, uint32_t col); // write a character to the LCD
Write_command_LCD(uint8_t command);
Initialize_LCD(); // run initialization commands on LCD


#endif /* LCD_DRIVER_H_ */
