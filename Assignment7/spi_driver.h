/*
 * spi_driver.h
 *
 *  Created on: Oct 16, 2019
 *      Author: ryanmyers
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#define DAC_RESOLUTION 0.00080566406f

#define SPI_PORT P3
#define SIMO    BIT3
#define SOMI    BIT2
#define SPI_CLK_PORT P4
#define SPI_CLK BIT4
#define CHIP_SEL BIT6

void Initialize_SPI(void);

void Send_DAC_Voltage(float v);


#endif /* SPI_DRIVER_H_ */
