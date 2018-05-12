
/*
 * si7021.c
 *
 *  Created on: April 23, 2018
 *      Author: Nikhil and Shreya
 *      code reference: Lec 6 by prof Keith Graham & AN0011 I2C Master and Slave operation example code silabs.
 */


#include "si7021.h"
#include "I2C.h"
#include "gpio.h"
#include "sleep.h"
#include "em_cmu.h"

float temp = 0;
uint32_t tempcode;

float get_temp_si7021()
{

	float converted_temp = 0;
	//i2c_reg_write(SI7021_I2CADDR,REGISTER_WRITE_COMMAND, SENSOR_SETUP_USER_REG1);
	uint32_t temp_code;
	temp_code = i2c_reg_read(SI7021_I2CADDR,MEASURE_TEMP_COMMAND,1) ;// command to measure temperature hold master mode
	converted_temp = (((175.72 * temp_code)/65536) - 46.85);
	if (converted_temp < TEMP_THRESHOLD)
	{
		CMU_ClockEnable(cmuClock_GPIO, true);
		GPIO_PinOutSet(LED1_port, LED1_pin);
		CMU_ClockEnable(cmuClock_GPIO, false);
	}
	return converted_temp;
}

unsigned int TempSensor_Enabled()
{
	return GPIO_PinOutGet(gpioPortD, 9);
}

float get_temp_code()
{
	return tempcode = i2c_reg_read(SI7021_I2CADDR,MEASURE_TEMP_COMMAND,1) ;// command to measure temperature hold master mode
}

int temp_convert (float value)
{
	return temp = (((175.72 * value)/65536) - 46.85);
}
