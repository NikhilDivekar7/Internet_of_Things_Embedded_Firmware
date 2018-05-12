/*
 * I2C.h
 *
 *  Created on: Feb 6, 2018
 *      Author: Shreya
 */

#ifndef I2C_H_
#define I2C_H_

#include "sleep.h"
#include "stdint.h"
#define MIN_I2C_ENERGYSTATE (EM2)// according to my sleep routine BLOCK/UNBLOCK EM2 will actually set the device in EM1 ie THE MIN ENERGY STATE OF I2C

#define I2C0_SCL_PIN 	(AF_I2C0_SCL_PIN(_I2C_ROUTELOC0_SCLLOC_LOC14))
#define I2C0_SDA_PIN 	(AF_I2C0_SDA_PIN(_I2C_ROUTELOC0_SDALOC_LOC16))
#define I2C0_SCL_PORT	(AF_I2C0_SCL_PORT(_I2C_ROUTELOC0_SCLLOC_LOC14))
#define I2C0_SDA_PORT 	(AF_I2C0_SDA_PORT(_I2C_ROUTELOC0_SDALOC_LOC16))

 void setup_I2C();
 uint32_t i2c_reg_read(uint8_t slaveAddress, uint8_t regAddress, uint8_t readDouble);
 void i2c_reg_write(uint8_t slaveAddress, uint8_t regAddress, uint8_t data);
 void reset_I2C();

#endif /* I2C_H_ */
