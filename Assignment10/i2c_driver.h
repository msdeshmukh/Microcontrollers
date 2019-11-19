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
#define RW_BIT BIT1
#define EEPROM_SIZE 32767

#define EEPROM_ADDR 0x50

void Initialize_I2C(void);
void WriteEEPROM(uint16_t addr, uint8_t data);
uint8_t ReadEEPROM(uint16_t addr);


#endif /* I2C_DRIVER_H_ */
