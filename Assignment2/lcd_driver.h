/*
 * lcd_driver.h
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#ifndef LCD_DRIVER_H_
#define LCD_DRIVER_H_

#include <stdint.h>
#include "delay.h"

#define LCD_DATA P2->OUT
#define LCD_PIN_MASK 0x7F
#define LCD_DATA_MASK 0x0F
#define LCD_EN_BIT 0x40
#define LCD_RS_BIT 0x20
#define LCD_RW_BIT 0x10

Clear_LCD(); // clear the display
Home_LCD(); // move the cursor to the top left of the LCD
Write_char_LCD(char to_write, uint32_t line, uint32_t col); // write a character to the LCD
Initialize_LCD(); // run initialization commands on LCD


#endif /* LCD_DRIVER_H_ */
