/*
 * i2c_driver.h
 *
 *  Created on: Nov 13, 2019
 *      Author: ryanmyers
 */

#ifndef I2C_DRIVER_H_
#define I2C_DRIVER_H_

#define I2C_PORT P1
#define SDA BIT6
#define SCL BIT7

#define EEPROM_ADDR 0x50

void Initialize_I2C(void);


#endif /* I2C_DRIVER_H_ */
