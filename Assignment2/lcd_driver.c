/*
 * lcd_driver.c
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "lcd_driver.h"
#include "delay.h"


const uint8_t DDRAM_Addr[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                                 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F };

void Clear_LCD() {
    Write_command_LCD(CLEAR_DISPLAY);
}

void Home_LCD() {
    Write_command_LCD(RETURN_HOME);
}

void Write_char_LCD(char to_write) {
    static uint8_t row = 0;
    static uint8_t col = 0;
    uint8_t new_addr = 0;

    LCD_CTL &= ~LCD_PIN_MASK;
    LCD_CTL |= LCD_RS_BIT;
    LCD_CTL |= to_write & LCD_DATA_MASK;

    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= (to_write << 4) & LCD_DATA_MASK;

    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    delay_us(37);

    if (++col > 15) {
        col = 0;
        row ^= 0x01;
    }

    new_addr = DDRAM_Addr[col + (16 * row)];


}

void Write_command_LCD(uint8_t command) {
    LCD_CTL &= ~LCD_PIN_MASK;
    LCD_CTL |= command & LCD_DATA_MASK;
    LCD_CTL &= ~LCD_RW_BIT;
    delay_us(2);
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= (command << 4) & LCD_DATA_MASK;
    delay_us(2);
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    switch (command) {
    case WAKE_UP:
        delay_us(40);
        break;
    case CLEAR_DISPLAY:
        delay_us(1600);
        break;
    case RETURN_HOME:
        delay_us(1600);
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
    P4->DIR |= LCD_PIN_MASK;    // Set P2.1-7 to output
    P4->REN &= ~LCD_PIN_MASK;   // Disable P2.0-6 pull up/down resistor
    P4->SEL0 &= ~LCD_PIN_MASK;  // Setup P2.0-6 as GPIO
    P4->SEL1 &= ~LCD_PIN_MASK;

    delay_us(42000);

    LCD_CTL &= ~LCD_PIN_MASK;
    LCD_CTL |= WAKE_UP & LCD_DATA_MASK;
    LCD_CTL &= ~LCD_RW_BIT;
    delay_us(2);
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;
    delay_us(40);

    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(DISPLAY_ON);
    Write_command_LCD(CLEAR_DISPLAY);
    Write_command_LCD(ENTRY_MODE_SET);

}
