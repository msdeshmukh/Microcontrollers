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

Write_command_LCD(uint8_t command) {
    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= command & LCD_DATA_MASK;
    LCD_CTL &= ~LCD_RW_BIT;

    LCD_CTL |= LCD_EN_BIT;
    us_delay(1);
    LCD_CTL &= ~LCD_EN_BIT;

    command = command << 4;
    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= command & LCD_DATA_MASK;

    LCD_CTL |= LCD_EN_BIT;
    us_delay(1);
    LCD_CTL &= ~LCD_EN_BIT;
}

Initialize_LCD() {
    P4->DIR |= LCD_PIN_MASK;    // Set P2.0-6 to output
    P4->REN &= ~LCD_PIN_MASK;   // Disable P2.0-6 pull up/down resistor
    P4->SEL0 &= ~LCD_PIN_MASK;  // Setup P2.0-6 as GPIO
    P4->SEL1 &= ~LCD_PIN_MASK;
    LCD_CTL = 0;

    us_delay(40000);
    LCD_CTL |= LCD_DATA_MASK & 0x30;
    us_delay(5000);

    LCD_CTL |= LCD_EN_BIT;
    us_delay(1);
    LCD_CTL &= ~LCD_EN_BIT;

    us_delay(160);

    LCD_CTL |= LCD_EN_BIT;
    us_delay(1);
    LCD_CTL &= ~LCD_EN_BIT;

    us_delay(160);

    LCD_CTL |= LCD_EN_BIT;
    us_delay(1);
    LCD_CTL &= ~LCD_EN_BIT;

    us_delay(160);

    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= LCD_DATA_MASK & 0x20;

    LCD_CTL |= LCD_EN_BIT;
    us_delay(1);
    LCD_CTL &= ~LCD_EN_BIT;

    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(SET_CURSOR);
    Write_command_LCD(DISPLAY_ON);
    Write_command_LCD(SET_ENTRY_MODE);

}
