/*
 * spi_driver.h
 *
 *  Created on: Oct 16, 2019
 *      Author: ryanmyers
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#define DAC_RESOLUTION 0.00080566406f

#define SPI_PORT P1
#define SIMO    BIT6
#define SOMI    BIT7
#define SPI_CLK BIT5
#define CHIP_SEL_PORT P5
#define CHIP_SEL BIT0

void Initialize_SPI(void);

void Send_DAC_Voltage(float v);


#endif /* SPI_DRIVER_H_ */
