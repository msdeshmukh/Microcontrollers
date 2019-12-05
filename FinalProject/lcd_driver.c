/*
 * lcd_driver.c
 *
 *  Created on: Sep 28, 2019
 *      Author: ryanmyers
 */

#include "msp.h"
#include "lcd_driver.h"
#include "delay.h"

// Look up table for DDRAM addresses on LCD
const uint8_t DDRAM_Addr[32] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                                 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
                                 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F };

// Clears all LCD contents and returns cursor to home
void Clear_LCD() {
    Write_command_LCD(CLEAR_DISPLAY);
}

// Puts the LCD cursor in the home position
void Home_LCD() {
    Write_command_LCD(RETURN_HOME);
}

void Write_string_LCD(char *str, uint32_t size) {
    int i;
    for (i = 0; i < size; i++) {
        Write_char_LCD(str[i]);
    }
}

// Writes a character to where the LCD cursor currently is and moves
// the cursor to the next position
void Write_char_LCD(char to_write) {
    static uint8_t row = 0;
    static uint8_t col = 0;
    uint8_t new_addr = 0;
    // Do not actually write a new line, just adjust cursor
    if (to_write != '\n') {
        // Clear all LCD control pins, put upper nibble of character on data lines
        LCD_CTL &= ~LCD_PIN_MASK;
        LCD_CTL |= LCD_RS_BIT;
        LCD_CTL |= to_write & LCD_DATA_MASK;

        // Enable cycle
        LCD_CTL |= LCD_EN_BIT;
        delay_us(2);
        LCD_CTL &= ~LCD_EN_BIT;

        // Clear data lines and put on lower nibble of character
        LCD_CTL &= ~LCD_DATA_MASK;
        LCD_CTL |= (to_write << 4) & LCD_DATA_MASK;

        // Enable cycle
        LCD_CTL |= LCD_EN_BIT;
        delay_us(2);
        LCD_CTL &= ~LCD_EN_BIT;

        // Wait for character write command to finish
        delay_us(40);
    }

    // If we have reached the end of the display or been
    // given a newline character, go to the next line
    if (++col > 15 || to_write == '\n') {
        col = 0;
        row ^= 0x01;
    }

    // Find new DDRAM address to jump to from LUT
    new_addr = DDRAM_Addr[col + (16 * row)];

    // Clear LCD control pins, put upper nibble of new DDRAM address
    // on data lines
    LCD_CTL &= ~LCD_PIN_MASK;
    LCD_CTL |= 0x80 | (new_addr & LCD_DATA_MASK);

    // Enable cycle
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    // Clear LCD data lines, put lower nibble on data lines
    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= (new_addr << 4) & LCD_DATA_MASK;

    // Enable cycle
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    // Wait for new DDRAM address command to finish
    delay_us(40);

}

void Write_command_LCD(uint8_t command) {
    // Clear all control pins, put upper nibble of command on LCD data lines
    LCD_CTL &= ~LCD_PIN_MASK;
    LCD_CTL |= command & LCD_DATA_MASK;
    delay_us(2);

    // Enable cycle
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    // Clear LCD data lines and put on lower nibble of command
    LCD_CTL &= ~LCD_DATA_MASK;
    LCD_CTL |= (command << 4) & LCD_DATA_MASK;
    delay_us(2);

    // Enable cycle
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    // Wait for instruction execution time of specific command
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
    // Set up pins P4.1 to P4.7 to control LCD
    P4->DIR |= LCD_PIN_MASK;    // Set P4.1-7 to output
    P4->REN &= ~LCD_PIN_MASK;   // Disable P4.1-7 pull up/down resistor
    P4->SEL0 &= ~LCD_PIN_MASK;  // Setup P4.1-7 as GPIO
    P4->SEL1 &= ~LCD_PIN_MASK;

    // Wait > 40ms after Vcc is applied to LCD
    delay_us(42000);

    // Write only upper nibble of wake up command
    LCD_CTL &= ~LCD_PIN_MASK;
    LCD_CTL |= WAKE_UP & LCD_DATA_MASK;
    delay_us(2);

    // Enable cycle
    LCD_CTL |= LCD_EN_BIT;
    delay_us(2);
    LCD_CTL &= ~LCD_EN_BIT;

    // Wait given time for wake up command to take place
    delay_us(40);

    // Call specified initialization sequence
    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(FUNCTION_SET);
    Write_command_LCD(DISPLAY_ON);
    Write_command_LCD(CLEAR_DISPLAY);
    Write_command_LCD(ENTRY_MODE_SET);

}
