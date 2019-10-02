/*
 * keypad_driver.h
 *
 *  Created on: Oct 1, 2019
 *      Author: ryanmyers
 */

#ifndef KEYPAD_DRIVER_H_
#define KEYPAD_DRIVER_H_

#define NUM_ROWS 4
#define NUM_COLS 3
#define KEYPAD_READ_REG P3->IN
#define KEYPAD_WRITE_REG P5->OUT
#define KEYPAD_READ_MASK 0x0F
#define KEYPAD_WRITE_MASK 0x07
#define KEYPAD_ROW_0 0x01
#define KEYPAD_ROW_1 0x02
#define KEYPAD_ROW_2 0x04
#define KEYPAD_ROW_3 0x08
#define KEYPAD_COL_0 0x01
#define KEYPAD_COL_1 0x02
#define KEYPAD_COL_2 0x04

uint8_t detect_key_press(void);
void Initialize_keypad(void);


#endif /* KEYPAD_DRIVER_H_ */
