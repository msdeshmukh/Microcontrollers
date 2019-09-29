/*
 * lcd_driver.c
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "lcd_driver.h"
#include "delay.h"


Clear_LCD() {

}

Home_LCD() {

}

Write_char_LCD(char to_write, uint32_t line, uint32_t col) {

}

Initialize_LCD() {
    P2->DIR |= LCD_PIN_MASK;    // Set P2.0-6 to output
    P2->REN &= ~LCD_PIN_MASK;   // Disable P2.0-6 pull up/down resistor
    P2->SEL0 &= ~LCD_PIN_MASK;  // Setup P2.0-6 as GPIO
    P2->SEL1 &= ~LCD_PIN_MASK;

}
