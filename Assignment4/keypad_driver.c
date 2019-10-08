/*
 *  keypad_driver.c
 *  Created on: Oct 1, 2019
 *  Authors: Mihir Deshmukh, Ryan Myers
 */
#include "msp.h"
#include "keypad_driver.h"


uint8_t detect_key_press() {
    int row;
    int col;
    uint32_t keypad_row_select = KEYPAD_ROW_0;
    uint32_t keypad_col_select = KEYPAD_COL_0;
    KEYPAD_WRITE_REG |= KEYPAD_WRITE_MASK;
    for (row = 0; row < NUM_ROWS; row++) {
        if (KEYPAD_READ_REG & keypad_row_select) {
            for (col = 0; col < NUM_COLS; col++) {
                KEYPAD_WRITE_REG &= ~KEYPAD_WRITE_MASK;
                KEYPAD_WRITE_REG |= keypad_col_select;
                if (KEYPAD_READ_REG & KEYPAD_READ_MASK) {
                    while(KEYPAD_READ_REG & KEYPAD_READ_MASK){}
                    return col + (row * NUM_COLS);
                }
                keypad_col_select <<= 1;
            }
        }
        keypad_row_select <<= 1;
    }
    return 255;
}

void Initialize_keypad(void) {
    // Initialize port 3 to read row data from keypad
    P6->DIR &= ~KEYPAD_READ_MASK;
    P6->SEL0 &= ~KEYPAD_READ_MASK;
    P6->SEL1 &= ~KEYPAD_READ_MASK;
    P6->REN |= KEYPAD_READ_MASK;
    P6->OUT &= ~KEYPAD_READ_MASK;
    // Initialize port 5 to drive keypad columns
    P5->DIR |= KEYPAD_WRITE_MASK;
    P5->SEL0 &= ~KEYPAD_WRITE_MASK;
    P5->SEL1 &= ~KEYPAD_WRITE_MASK;
    P5->REN &= ~KEYPAD_WRITE_MASK;
}
