/*
 * lpm.c
 *
 *  Created on: Feb 9, 2018
 *      Author: Shreya
 */

#include "I2C.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "sleep.h"
#include "gpio.h"

void LoadPowerON()
{

	/*enabling sensor*/
    //GPIO_PinOutSet(gpioPortD, 9);
 	//delay after enabling sensor
//  	for(int i = 0; i < 6000; i++);

 	I2C_Enable(I2C0,true);
	//BlockSleepMode(MIN_I2C_ENERGYSTATE);
	/* set port and pin for sda and scl  */
    GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeWiredAnd, 1);
	GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeWiredAnd, 1);

	for (int i=0; i<9; i++)
	{
		GPIO_PinOutClear(gpioPortC, 10);
		GPIO_PinOutSet(gpioPortC, 10);
	}
    /* Exit the busy state.  The I2C0 will be in this state out of RESET */
	if (I2C0->STATE & I2C_STATE_BUSY) // if I2C is busy
	{
		I2C0->CMD = I2C_CMD_ABORT; // abort command
	}

    //UnblockSleepMode(MIN_I2C_ENERGYSTATE);

}
void LoadPowerOFF()
{
	//BlockSleepMode(MIN_I2C_ENERGYSTATE);
	/* set port and pin for sda and scl  */
	GPIO_PinModeSet(I2C0_SCL_PORT, I2C0_SCL_PIN, gpioModeDisabled, 0);
	GPIO_PinModeSet(I2C0_SDA_PORT, I2C0_SDA_PIN, gpioModeDisabled, 0);

	//GPIO_PinOutClear(gpioPortD, 9);
	GPIO_PinModeSet(gpioPortD, 9, gpioModeDisabled, 0);

	 //UnblockSleepMode(MIN_I2C_ENERGYSTATE);
}
