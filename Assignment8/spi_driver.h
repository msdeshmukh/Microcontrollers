/*
 * spi_driver.h
 *
 *  Created on: Oct 16, 2019
 *      Author: ryanmyers
 */

#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#define DAC_RESOLUTION 0.00080586079365f

#define SPI_PORT P1
#define SIMO    BIT6
#define SOMI    BIT7
#define SPI_CLK BIT5
#define CHIP_SEL_PORT P3
#define CHIP_SEL BIT0

void Initialize_SPI(void);

void Send_DAC_Voltage(float v);
void Send_DAC_Value(uint16_t out_val);


#endif /* SPI_DRIVER_H_ */
