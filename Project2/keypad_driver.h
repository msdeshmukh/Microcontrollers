/*
 *  keypad_driver.h
 *  Created on: Oct 1, 2019
 *  Authors: Mihir Deshmukh, Ryan Myers
 */
#ifndef KEYPAD_DRIVER_H_
#define KEYPAD_DRIVER_H_

#define NUM_ROWS 4
#define NUM_COLS 3
#define KEYPAD_READ_REG P6->IN
#define KEYPAD_WRITE_REG P5->OUT
#define KEYPAD_READ_MASK 0xF0
#define KEYPAD_WRITE_MASK 0x07
#define KEYPAD_ROW_0 0x10
#define KEYPAD_COL_0 0x01
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53
#define SIX 54
#define SEVEN 55
#define EIGHT 56
#define NINE 57
#define ZERO 48
#define HASH 35
#define STAR 42

uint8_t detect_key_press(void);
void Initialize_keypad(void);


#endif /* KEYPAD_DRIVER_H_ */
