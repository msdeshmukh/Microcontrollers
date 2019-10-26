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

void EUSCIA0_IRQHandler(void);
void Initialize_UART(void);
void Send_Serial_Char(unsigned char c);

#endif /* UART_DRIVER_H_ */
