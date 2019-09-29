/*
 * lcd_driver.c
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "lcd_driver.h"
#include "delay.h"


void Clear_LCD() {
    Write_command_LCD(CLEAR_DISPLAY);
}

void Home_LCD() {
    Write_command_LCD(RETURN_HOME);
}

void Write_char_LCD(char to_write, uint32_t line, uint32_t col) {

}

void Write_command_LCD(uint8_t command) {
    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= command & LCD_DATA_MASK;
    LCD_CTL &= ~LCD_RW_BIT;

    LCD_CTL |= LCD_EN_BIT;
    delay_us(1);
    LCD_CTL &= ~LCD_EN_BIT;

    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= (command << 4) & LCD_DATA_MASK;

    LCD_CTL |= LCD_EN_BIT;
    delay_us(1);
    LCD_CTL &= ~LCD_EN_BIT;

    switch (command) {
    case WAKE_UP:
        delay_us(40);
        break;
    case CLEAR_DISPLAY:
        delay_us(16000);
        break;
    case RETURN_HOME:
        delay_us(16000);
        break;
    case ENTRY_MODE_SET:
        delay_us(40);
        break;
    case DISPLAY_ON:
        delay_us(40);
        break;
    case CURSOR_SHIFT:
        delay_us(40);
        break;
    case FUNCTION_SET:
        delay_us(40);
        break;
    default:
        break;
    }
}

void Initialize_LCD() {
    P4->DIR |= LCD_PIN_MASK;    // Set P2.0-6 to output
    P4->REN &= ~LCD_PIN_MASK;   // Disable P2.0-6 pull up/down resistor
    P4->SEL0 &= ~LCD_PIN_MASK;  // Setup P2.0-6 as GPIO
    P4->SEL1 &= ~LCD_PIN_MASK;

    delay_us(42000);

    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= WAKE_UP & LCD_DATA_MASK;
    LCD_CTL &= ~LCD_RW_BIT;

    LCD_CTL |= LCD_EN_BIT;
    delay_us(1);
    LCD_CTL &= ~LCD_EN_BIT;
    delay_us(37);

    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(DISPLAY_ON);
    Write_command_LCD(CLEAR_DISPLAY);
    Write_command_LCD(ENTRY_MODE_SET);

}
