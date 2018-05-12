/*
 * I2C.c
 *
 *  Created on: Feb 6, 2018
 *      Author: Nikhil and Shreya
 *      code reference: Lec 6 by prof Keith Graham & AN0011 I2C Master and Slave operation example code silabs.
 *
 */

/* include statements */

#include <stdbool.h>
#include "em_device.h"
#include "em_i2c.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "i2cspm.h"
#include "I2C.h"
#include "gpio.h"
#include "cmu.h"


 void setup_I2C()
{
	CMU_I2C0_Init();

	I2C_Init_TypeDef i2c_init = I2C_INIT_DEFAULT;

	i2c_init.clhr = i2cClockHLRStandard;
	i2c_init.master = true;
	i2c_init.refFreq = 0;
	i2c_init.freq = 5000; //normal mode
	i2c_init.enable = false; //will start when required. Not on Init

    GPIO_PinModeSet(gpioPortC, 10, gpioModeWiredAnd, 1);
    GPIO_PinModeSet(gpioPortC, 11, gpioModeWiredAnd, 1);

	I2C0->ROUTELOC0 = I2C_ROUTELOC0_SDALOC_LOC16 | I2C_ROUTELOC0_SCLLOC_LOC14;
	I2C0->ROUTEPEN = (I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN);

	I2C_Init(I2C0, &i2c_init);

	for (int i=0; i<9; i++)
	{
		GPIO_PinOutClear(gpioPortC, 10);
		GPIO_PinOutSet(gpioPortC, 10);
	}

	if(I2C0->STATE & I2C_STATE_BUSY)
	{
			I2C0->CMD = I2C_CMD_ABORT;
	}
	//BlockSleepMode(MIN_I2C_ENERGYSTATE);
}

uint32_t i2c_reg_read(uint8_t slaveAddress, uint8_t regAddress, uint8_t readDouble)
{
	uint32_t data = 0xFF;
	//I2C0->SADDR = (uint32_t)((slaveAddress) & 0xFE);
	I2C0->CMD = I2C_CMD_START; // send start command
	I2C0->TXDATA = (uint32_t)((slaveAddress << 1) & 0xFE) ;  // signifying write of address to the slave, primes tx buffer for start command;

	//I2C0->CMD = I2C_CMD_START; // send start command
	 while ((I2C0->IF & I2C_IF_ACK) ==  0); //waiting for slave to respond
	 	 I2C0->IFC = I2C_IFC_ACK;
	 I2C0->TXDATA = regAddress;

	 while ((I2C0->IF & I2C_IF_ACK) ==  0); //waiting for slave to respond
	 	 I2C0->IFC = I2C_IFC_ACK;
	 	I2C0->CMD = I2C_CMD_START; // send restart command
	 I2C0->TXDATA = (uint32_t)(slaveAddress << 1 )| 0x01 ; // read bit high

	 while ((I2C0->IF & I2C_IF_ACK) ==  0); //waiting for slave to respond
	 	 I2C0->IFC = I2C_IFC_ACK;

	 if(readDouble == 1)
	 {
		while((I2C0->IF & I2C_IF_RXDATAV) == 0); // checking receive flag for msb
		data = I2C0->RXDATA;
		data <<= 8;

		I2C0->CMD = I2C_CMD_ACK;

		while((I2C0->IF & I2C_IF_RXDATAV )== 0); // checking receive flag for lsb
		data |= I2C0->RXDATA;

	 }
	 else
	 {
		 while((I2C0->IF & I2C_IF_RXDATAV) == 0); // checking receive flag for msb
		 data = I2C0->RXDATA;
	 }


	 I2C0->CMD = I2C_CMD_NACK;


	 I2C0->CMD = I2C_CMD_STOP; // send stop command


	 return data;

 }

 void i2c_reg_write(uint8_t slaveAddress, uint8_t regAddress, uint8_t data)
  {

 	 I2C0->TXDATA = (uint32_t)(slaveAddress << 1) & 0xFE ;  // signifying write of address to the slave, primes tx buffer for start command;
 	 I2C0->CMD = I2C_CMD_START; // send start command

 	 while ((I2C0->IF & I2C_IF_ACK) ==  0); //waiting for slave to respond
 	 	 I2C0->IFC = I2C_IFC_ACK;
 	 I2C0->TXDATA = regAddress;

 	 while ((I2C0->IF & I2C_IF_ACK) ==  0); //waiting for slave to respond
 	 	 I2C0->IFC = I2C_IFC_ACK;


 	 //write data
		 I2C0->TXDATA = data ;  // signifying write of address to the slave, primes tx buffer for start command;
 	 	 I2C0->CMD = I2C_CMD_ACK; // send start command


 	 I2C0->CMD = I2C_CMD_STOP; // send stop command

  }


void reset_I2C()
{
	for(int i=0;i<9;i++)
	{
		GPIO_PinOutClear(I2C0_SCL_PORT, I2C0_SCL_PIN);
		GPIO_PinOutSet(I2C0_SCL_PORT, I2C0_SCL_PIN);
	}
}




