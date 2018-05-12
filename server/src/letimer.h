/*
 * letimer.h
 *
 *  Created on: Jan 30, 2018
 *      Author: Shreya
 */


#ifndef LETIMER_H
#define LETIMER_H

#include "em_cmu.h"      /* include statements */
#include "em_letimer.h"
#include "sleep.h"
#include "gpio.h"

uint8_t Schedule_event;

#define PERIOD_MSEC 		(4000)            // on time OF LED in seconds
#define DELAY_PERIOD_MSEC 	(80)            //Delay to be given after enabling the sensor for stabilization
#define LETIMER_EVENT     	(1)				// go through the scheduler
#define I2C_STAB_EVENT     	(2)			// go to sleep
#define letimer0_comp1_flag  (0)
#define letimer0_comp0_flag  (0)
#define LETIMER0_EXT_EVT_COMP (0x01)

extern volatile uint32_t external_event_status;




unsigned int FreqofClock;                           //Variable to store current tree clock frequency

void LETIMER_init();							//function to initialize letimer


void LETIMER0_IRQHandler(void);						//function to handle interrupts



#endif /* SRC_LETIMER_H_ */
