/*
 * si7021.h
 *
 *  Created on: Feb 9, 2018
 *      Author: Shreya
 */

#ifndef SI7021_H_
#define SI7021_H_


#define SI7021_I2CADDR  (0x39)
#define MEASURE_TEMP_COMMAND (0x8C)
#define REGISTER_WRITE_COMMAND (0xE6)
#define REGISTER_READ_COMMAND   (0xE7)
#define SENSOR_SETUP_USER_REG1  (0x00) //14 bit temp sensor with heater disabled
#define TEMP_THRESHOLD          (15)  //in celcuis
#define LIGHT_THRESHOLD			(50)
#define DISTANCE_THRESHOLD		(10)

/*function to get temp data from sensor*/
float get_temp_si7021();
unsigned int TempSensor_Enabled();
int temp_convert (float value);
float get_temp_code();

#endif /* SI7021_H_ */
