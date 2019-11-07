/*
 * uart_driver.h
 *
 *  Created on: Oct 24, 2019
 *      Author: ryanmyers
 */

#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#define UART_PORT P1
#define UART_RXD BIT2
#define UART_TXD BIT3

#define FBRCLK_12MHz_UCBRW 6
#define FBRCLK_12MHz_UCBRF 8
#define FBRCLK_12MHz_UCBRS 0x20
#define FBRCLK_12MHz_OS16 1

#define INPUT_READY 1
#define INPUT_UNAVAILABLE 0

#define ZERO 48
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
#define FIVE 53
#define SIX 54
#define SEVEN 55
#define EIGHT 56
#define NINE 57
#define NEW_LINE 10
#define RETURN 13

void Write_Desc_Values_To_VT100(void);
void Init_Desc_Values_To_VT100(void);
void EUSCIA0_IRQHandler(void);
void Initialize_UART(void);
void Send_Serial_Char(unsigned char c);
uint16_t GetInputValue(void);
int CheckInputFlag(void);

#endif /* UART_DRIVER_H_ */
